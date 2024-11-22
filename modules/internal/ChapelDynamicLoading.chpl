/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

module ChapelDynamicLoading {
  private use ChapelStandard;
  private use CTypes;
  private use Atomics;
  private use ChapelLocks;

  extern {
    #include <dlfcn.h>
  }

  /** If the mode is 'OFF', then dynamic loading cannot be used.

      If the mode is 'EAGER', then when a library is loaded, Chapel
      will load it on all locales. */
  enum chpl_dynamicLoading { OFF, EAGER }

  config param chpl_procedureLookupDebug = false;
  config param chpl_dynamicLoadingDebug = false;
  param chpl_msbUintMask = (1 << 63);
  param chpl_defaultProcBufferSize = 512;
  var chpl_dynamicLoadingSupport = chpl_dynamicLoading.EAGER;
  var chpl_dynamicProcIdxCounter: atomic uint = 1;
  type allFtablesVec = chpl_localBuffer(c_ptr(c_ptr(void)));

  // This is a privatized table of 'ftable' pointers indexed by locale.
  // Every loaded codebase needs to have this vector prepared so that it
  // can call runtime functions like 'chpl_executeOn', as well as to
  // translate local pointers to dynamic procedure indices.
  pragma "locale private"
  var chpl_privatizedFtableVec: allFtablesVec;

  // TODO: Ensure this is only called once by the "origin" Chapel binary.
  // TODO: Need to make sure that the runtime invokes these 'on' statements
  // using the 'chpl_ftable' symbols it was linked against, otherwise there
  // is no way to call 'chpl_executeOn' at all. Once this is executed then
  // the 'chpl_ftable' symbols in the runtime can be "discarded".
  export proc chpl_originBuildFtableVecAtStartup() {
    for loc in Locales do on loc {
      chpl_privatizedFtableVec = new allFtablesVec(numLocales);
    }

    for loc1 in Locales do on loc1 {
      extern const chpl_ftable: c_ptr(c_ptr(void));
      const ptr = chpl_ftable;
      const n = (here.id : int);

      coforall loc2 in Locales with (ref chpl_privatizedFtableVec) {
        on loc2 do chpl_privatizedFtableVec[n] = ptr;
      }
    }
  }

  // Ininitialize the privatized buffer of 'ftable' pointers.
  chpl_originBuildFtableVecAtStartup();

  proc chpl_getPrivatizedFtableVecPtr() {
    return chpl_privatizedFtableVec.ptr();
  }

  // TODO: A gadget to let one try out different types of locks. I had
  // written a RW-lock in 'ChapelLocks' but was having trouble getting
  // it to work. If we ever want to try something besides a spinlock
  // then just swap the locks in here by adding overloads controlled
  // by where clauses.
  record chpl_lockWrapper {
    var _lock: chpl_LocalSpinlock;

    inline proc useSpinlock param do return true;
    inline proc const withReadLock() ref where useSpinlock do return _lock;
    inline proc const withWriteLock() ref where useSpinlock do return _lock;
  }

  class chpl_LocalPtrCache {
    forwarding var _lockWrapper = new chpl_lockWrapper();
    var _procPtrToIdx = new chpl_localMap(c_ptr(void), uint);
    var _idxToProcPtr = new chpl_localMap(uint, c_ptr(void));

    inline proc set(ptr: c_ptr(void), idx: uint) {
      on this do {
        var ok1 = _procPtrToIdx.set(ptr, idx);
        var ok2 = _idxToProcPtr.set(idx, ptr);
        assert(ok1);
        assert(ok2);
      }
    }

    inline proc get(ptr: c_ptr(void), out idx: uint) {
      return _procPtrToIdx.get(ptr, idx);
    }

    inline proc get(idx: uint, out ptr: c_ptr(void)) {
      return _idxToProcPtr.get(idx, ptr);
    }
  }

  // One cache of pointers guarded by a RW-lock defined per locale.
  pragma "locale private"
  var chpl_privatizedPtrCache = new unmanaged chpl_LocalPtrCache();

  // Initialize the privatized pointer cache on all other locales.
  if numLocales > 1 {
    forall loc in Locales[1..] with (ref chpl_privatizedPtrCache) do on loc {
      local do {
        assert(chpl_privatizedPtrCache == nil);
        chpl_privatizedPtrCache = new unmanaged chpl_LocalPtrCache();
      }
    }
  }

  // A store of of all loaded binaries which lives on LOCALE-0. Each
  // time a binary is loaded an entry will be made in the store.
  // The interface to the stored info is the system pointer retrieved
  // by 'dlopen'.
  class chpl_BinaryInfoStore {
    forwarding var _lockWrapper = new chpl_lockWrapper();

    // The key is the 'dlopen' pointer returned on LOCALE-0. You are
    // permitted to 'dlopen' a symbol multiple times, so you can get
    // the address from the 'dlopen' call as needed to use as a key.
    var _handleToInfo: chpl_localMap(c_ptr(void), unmanaged chpl_BinaryInfo?);

    inline proc set(ptr: c_ptr(void), bin: unmanaged chpl_BinaryInfo?) {
      return _handleToInfo.set(ptr, bin);
    }

    inline proc get(ptr: c_ptr(void), out bin: unmanaged chpl_BinaryInfo?) {
      return _handleToInfo.get(ptr, bin);
    }
  }

  // This is one per entrypoint binary and lives on LOCALE-0.
  var chpl_binaryInfoStore = new unmanaged chpl_BinaryInfoStore();

  class DynLibError : Error {
    proc init(msg: string) {
      super.init(msg);
      init this;
    }
  }

  // This class represents a loaded binary. It contains the state
  // necessary to load a symbol from the binary on each locale.
  class chpl_BinaryInfo {
    forwarding var lockWrapper = new chpl_lockWrapper();

    // This refcount is bumped and dropped by the user-facing wrapper.
    var _refCount: atomic int = 1;

    // This is the path that was used to load the binary.
    var _path: string;

    // This is the set of loaded binary pointers, indexed by locale.
    var _systemPtrs = new chpl_localBuffer(c_ptr(void), numLocales);

    // Local pointers for loaded symbols on LOCALE-0. These are used
    // to evict entries from the procedure pointer cache when the
    // library is finally closed. The value is a pair of "wide index"
    // and the symbol name.
    var _procPtrToDataLocale0: chpl_localMap(c_ptr(void), (uint, string));

    // A pointer to the parent store is used to coordinate load/unload.
    var _store: borrowed chpl_BinaryInfoStore;

    // Set only when '_close()' is called.
    var _cleared = false;

    proc init(in path: string, in store: borrowed chpl_BinaryInfoStore) {
      this._path = path;
      this._store = store;
      init this;
    }

    proc deinit() do assert(_cleared);

    // Load a new binary given a path.
    proc type create(path: string, out err: owned DynLibError?) {
      var ret: unmanaged chpl_BinaryInfo? = nil;

      on Locales[0] {
        ref store = chpl_binaryInfoStore;
        var shouldCreateNewEntry = true;

        // Start by opening a system handle on LOCALE-0.
        var err0;
        var ptr0 = _systemDynLibOpen(path, err0);
        defer { if ptr0 then _systemDynLibClose(ptr0); }

        if err0 != nil || ptr0 == nil {
          if err0 != nil then err = err0;
          shouldCreateNewEntry = false;
        }

        // Check the LOCALE-0 store for an existing entry.
        if shouldCreateNewEntry {
          manage store.withReadLock() {
            shouldCreateNewEntry = !store.get(ptr0, ret);
          }
        }

        // None found, so initialize a new binary info.
        if shouldCreateNewEntry {
          var bin = new owned chpl_BinaryInfo(path, chpl_binaryInfoStore);

          // Swap in the LOCALE-0 pointer and clear the local variable.
          bin._systemPtrs[0] = ptr0;
          ptr0 = nil;

          type E = owned DynLibError?;
          var errBuf = new chpl_localBuffer(E, numLocales);
          var numErrs: atomic int = 0;

          // Loop and attempt to open system handles on other locales.
          if numLocales > 1 {
            coforall loc in Locales[1..] with (ref errBuf) do on loc {
              const n = (here.id: int);

              var err;
              var ptr = _systemDynLibOpen(path, err);
              defer { if ptr then _systemDynLibClose(ptr); }

              if err {
                // The buffer is not wide! Assignment occurs on LOCALE-0.
                on Locales[0] do errBuf[n] = err;
                numErrs.fetchAdd(1);
              } else if ptr {
                // The buffer is not wide!
                on Locales[0] do bin._systemPtrs[n] = ptr;
                // Clear the pointer to avoid closing it.
                ptr = nil;
              } else {
                // TODO: Construct an error instead.
                halt('Failed to load system \'dlopen()\' pointer!');
              }
            }
          }

          if numErrs.read() {
            // If there were errors, then report the first error.
            // TODO: Consolidate the reported errors.
            for i in 0..<errBuf.size {
              if errBuf[i] {
                err = errBuf[i];
                break;
              }
            }
          } else {
            // Otherwise, try to add the newly created info to the
            // store on LOCALE-0, or give up if another task already
            // beat us to the chase.

            for i in 0..<bin._systemPtrs.size {
              assert(bin._systemPtrs[i] != nil);
            }

            manage store.withWriteLock() {
              var found = store.get(bin._systemPtrs[0], ret);
              if !found {
                ret = owned.release(bin);
                var ok = store.set(ret!._systemPtrs[0], ret);
                assert(ok);
              }
            }
          }
        }
      }

      return ret;
    }

    inline proc type _systemDynLibClearError() do dlerror();

    inline proc type _systemDynLibClose(ptr: c_ptr(void)) do dlclose(ptr);

    inline proc type _systemDynLibWrapError() {
      type E = owned DynLibError?;
      const sysErr = dlerror();
      if sysErr != nil then
        return new E(try! string.createCopyingBuffer(sysErr));
      return nil;
    }

    inline proc type
    _systemDynLibOpen(path: string, out err: owned DynLibError?) {
      _systemDynLibClearError();
      var ret = dlopen(path.c_str(), RTLD_LAZY);
      if ret == nil {
        var tmp = _systemDynLibWrapError();
        if tmp != nil then err = tmp;
      }
      return ret;
    }

    inline proc type
    _systemDynLibLookup(sym: string, handle: c_ptr(void),
                        out err: owned DynLibError?) {
      _systemDynLibClearError();
      var ret = dlsym(handle, sym.c_str());
      if ret == nil {
        var tmp = _systemDynLibWrapError();
        if tmp != nil then err = tmp;
      }
      return ret;
    }

    // TODO: Also need to evict pointer cache entries.
    proc _close() {
      assert(!_cleared && _refCount.read() == 0);
      _cleared = true;
      on Locales[0] {
        for i in 0..<_systemPtrs.size {
          const ptr = _systemPtrs[i];
          if ptr != nil then this.type._systemDynLibClose(ptr);
        }
      }
    }

    inline proc bumpRefCount() do _refCount.fetchAdd(1);
    inline proc dropRefCount() {
      if _refCount.read() <= 0 then return;
      if _refCount.fetchSub(1) <= 1 then _close();
    }

    // TODO: 'T' must be a procedure type, but we cannot restrict it yet.
    // Could add a primitive "any procedure type" instead.
    proc loadSymbol(sym: string, type T, out err: owned DynLibError?): T {
      var shouldInternInPointerCache = false;
      var idx: uint = 0;

      if !__primitive("is fcf type", T) || isClassType(T) {
        compilerError('The type passed to \'loadSymbol\' must be ' +
                      'a procedure type');
      }

      // On the fast path, we check to see if the symbol exists on LOCALE-0.
      // If it does, we check to see if we have already interned it into
      // the procedure pointer cache. If we have, then we can take the local
      // address and immediately transiate it into a wide index.
      on Locales[0] {
        var shouldProceedWithLookup = true;

        // No need to grab the lock, this should not be modified.
        const handle = _systemPtrs[0];

        // And should not be nil, by construction.
        assert(handle != nil);

        var err0;
        var ptr0 = this.type._systemDynLibLookup(sym, handle, err0);
        if err0 != nil || ptr0 == nil {
          if err0 != nil then err = err0;
          shouldProceedWithLookup = false;
        }

        if shouldProceedWithLookup {
          assert(ptr0 != nil);
          manage this.withReadLock() {
            var data;
            if _procPtrToDataLocale0.get(ptr0, data) {
              const (got, sym) = data;
              idx = got;
            } else {
              shouldInternInPointerCache = true;
            }
          }
        }

        if shouldInternInPointerCache {
          type E = owned DynLibError?;
          var errBuf = new chpl_localBuffer(E, numLocales);
          var numErrs: atomic int = 0;

          // Get the unique index to use by interning on LOCALE-0.
          idx = chpl_insertExternLocalPtrNoSync(ptr0, 0);
          assert(idx != 0);

          // Loop over all locales and fetch the symbol's local pointer.
          if numLocales > 1 {
            coforall loc in Locales[1..] do on loc {
              const n = (here.id: int);

              var err;
              var ptr = this.type._systemDynLibLookup(sym, handle, err);
              if err {
                on Locales[0] do errBuf[n] = err;
                numErrs.fetchAdd(1);
              } else if ptr {
                var got = chpl_insertExternLocalPtrNoSync(ptr, idx);
                assert(got == idx);
              } else {
                // TODO: Construct an error instead.
                halt('Failed to fetch symbol from \'dlsym()\'!');
              }
            }
          }

          if numErrs.read() {
            // If there were errors, then report the first error.
            // TODO: Evict any pointers stored in the cache.
            // TODO: Consolidate the reported errors.
            for i in 0..<errBuf.size {
              if errBuf[i] {
                err = errBuf[i];
                break;
              }
            }
            // Clear the index since there was an error.
            idx = 0;
          }
        }
      }

      // Fine if 'ret' is '0', that is the wide-index equivalent of 'nil'.
      var ret = __primitive("cast", T, idx);

      return ret;
    }
  }

  // TODO: The moment I try to delete these pointers some locales will crash
  // with (what I assume is) a double-free or some other memory corruption
  // on program shutdown. You don't really see it single-locale unless
  // jemalloc allocates a large enough chunk, so set the flag
  // 'chpl_defaultProcBufferSize=512' to see. The crash seems to happen in
  // the qthreads layer, perhaps while cleaning up TLS.
  proc deinit() {
    /*
    if here == Locales[0] {
      for loc in Locales do on loc {
        if chpl_privatizedPtrCache != nil then delete chpl_privatizedPtrCache;
      }
    }
    */
  }

  private inline proc isDynamicLoadingEnabled {
    return chpl_dynamicLoadingSupport != chpl_dynamicLoading.OFF;
  }

  // This is a simple local buffer that only provides [] indexing and
  // halts on an out-of-range index value. It halts instead of having
  // a failure-friendly API because it should never fail (this type is
  // only used as a backing buffer for other types - and they should
  // request valid indices).
  record chpl_localBuffer {
    type T;
    var _ptr: c_ptr(T) = nil;
    var _size: uint = 0;

    proc init(type T) {
      this.T = T;
      init this;
    }

    proc init(type T, size: uint) {
      this.T = T;
      this._size = size;
      init this;

      if _size > 0 {
        this._ptr = allocate(T, size, true);
        if _ptr == nil then halt('Out of memory!');
      }
    }

    inline proc size do return _size;

    inline proc ptr do return _ptr;

    inline proc this(idx: integral) ref: T {
      if idx < 0 || idx >= _size then halt('Out of bounds!');
      ref ret = _ptr[idx];
      return ret;
    }

    proc deinit() {
      if _ptr != nil then deallocate(_ptr);
    }
  }

  // This type is a very simple local hashtable that uses linear probing.
  // I've opted to reinvent the wheel instead of using 'ChapelHashtable'
  // to not preclude the latter from using procedure pointers in its
  // implementation. Which, in retrospect, is probably pointless if we
  // just end up moving the whole cache to C. But hey, it was a fun
  // exercise/refresher.
  record chpl_localMap {
    type K, V;
    record slot {
      var key;
      var val;
    }
    var _slots: chpl_localBuffer(slot(K, V));
    var _size: uint = 0;

    proc init(type K, type V) {
      this.K = K;
      this.V = V;
      init this;
      this._slots = new chpl_localBuffer(slot(K, V), _baseBufferSize);
    }

    inline proc size do return _size;
    inline proc _baseBufferSize param do return chpl_defaultProcBufferSize;
    inline proc _bufferSize do return _slots.size;
    inline proc _hash(key: K): uint {
      use ChapelHashing;
      return (key : uint).hash();
    }

    inline proc _isKeyZeroBits(key: K) do return (key : uint) == 0;
    inline proc _shouldTryToExpand() do return _size >= (_slots.size >> 1);
    inline proc _shouldTryToShrink() do return false;
    inline proc _nextExpandSize(): uint do return _slots.size << 1;
    inline proc _nextShrinkSize(): uint {
      var num = _slots.size >> 1;
      return if num < _baseBufferSize then _baseBufferSize else num; 
    }

    inline iter _walk(start: uint) {
      for idx in start..<_bufferSize do yield idx;
      if start != 0 then {
        for idx in 0..<start do yield idx;
      }
    }

    proc _resizeIfNeeded() {
      if _shouldTryToExpand() then halt('Not implemented yet!');
      if _shouldTryToShrink() then halt('Not implemented yet!');
    }

    inline proc _findSlotIdx(in key: K, out idx: uint, resize: bool): bool {
      assert(!_isKeyZeroBits(key));

      if resize then _resizeIfNeeded();

      const start = _hash(key) % _bufferSize;
      for w in _walk(start) {
        const ref slot = _slots[w];
        if _isKeyZeroBits(slot.key) || slot.key == key {
          idx = w;
          return true;
        }
      }

      return false;
    }

    proc set(in key: K, in val: V): bool {
      if _isKeyZeroBits(key) then return false;

      var idx;
      var found = _findSlotIdx(key, idx, resize=true);
      assert(found);

      ref slot = _slots[idx];
      if _isKeyZeroBits(slot.key) {
        slot.key = key;
        slot.val = val;
        return true;
      } else if (slot.key == key) {
        slot.val = val;
        return false;
      }

      halt('Should not reach here!'); 
      return false;
    }

    proc get(in key: K, out val: V): bool {
      if _isKeyZeroBits(key) then return false;

      var idx;
      var found = _findSlotIdx(key, idx, resize=false);
      assert(found);

      ref slot = _slots[idx];
      if slot.key == key {
        val = slot.val;
        return true;
      }

      return false;
    }

    proc clear(in key: K): bool {
      halt('Not implemented yet!');
      return false;
    }
  }

  // TODO TODO TODO: When we compile the runtime as a dynamic library, we
  // will need to adjust how it emits ftable calls. Right now it assumes
  // the existence of a single, unified 'chpl_ftable' that is linked
  // in when the rest of the Chapel program is compiled. But in a
  // universe where each Chapel program has its own ftable, runtime
  // functions like 'chpl_ftable_call' cannot keep referring to a single
  // name but will have to be passed in a binary-specific table instead.
  //
  // We need the ftable to get the local pointer from the static index.
  // This is safe to do since it is locale private and we know we are
  // only referring to the one in our binary (the same cannot be said
  // for the runtime code once we dynamic link it).
  //
  // TODO: If I make this inline I get an internal compiler error.
  private proc lookupPtrFromLocalFtable(idx: uint): c_ptr(void) {
    extern const chpl_ftable: c_ptr(c_ptr(void));
    extern const chpl_ftableSize: int(64);
    assert(idx < chpl_ftableSize);
    return chpl_ftable[idx];
  }

  inline proc fetchLocalPtrForDynamicIdx(idx: uint): c_ptr(void) {
    var ret: c_ptr(void) = nil;

    // There should always be an entry for this dynamic index in the
    // local cache because all roots are emplaced when a function
    // value is created. So just assert/halt if this is not the case.
    local do manage chpl_privatizedPtrCache.withReadLock() {
      var found = chpl_privatizedPtrCache.get(idx, ret);
      assert(found);
    }

    assert(ret != nil);
    return ret;
  }

  inline proc fetchDynamicIdxForStaticIdx(idx: uint): uint {
    var ret: uint = 0;

    // Return the local pointer for this locale if it's already set.
    local do manage chpl_privatizedPtrCache.withReadLock() {
      var ptr = lookupPtrFromLocalFtable(idx);
      if chpl_privatizedPtrCache.get(ptr, ret) then return ret;
    }

    // Otherwise, synchronize on LOCALE-0...
    on Locales[0] {
      manage chpl_privatizedPtrCache.withWriteLock() {
        var requestedUniqueIdx = false;

        // Use the value of the pointer on LOCALE-0 as the map key.
        var ptr = lookupPtrFromLocalFtable(idx);

        if !chpl_privatizedPtrCache.get(ptr, ret) {
          // If we did not look up an existing entry, then we are
          // the task that will set the map entries for this
          // pointer. Set the index on LOCALE-0 to claim the job.
          ret = chpl_dynamicProcIdxCounter.fetchAdd(1);
          chpl_privatizedPtrCache.set(ptr, ret);
          requestedUniqueIdx = true;
        }

        // While holding the LOCALE-0 lock, set on all other locales.
        if requestedUniqueIdx && numLocales > 1 {
          coforall loc in Locales[1..] do on loc {
            local do manage chpl_privatizedPtrCache.withWriteLock() {
              var ptr = lookupPtrFromLocalFtable(idx);
              chpl_privatizedPtrCache.set(ptr, ret);
            }
          }
        }
      }
    }

    assert(ret != 0);

    return ret;
  }

  // This function is called by the dynamic loading code. If the formal
  // 'idx' is non-zero, it will be used as the dynamic index value.
  // Otherwise, the routine will fetch a unique value to use as the
  // dynamic index value.
  //
  // This routine attempts to emplace a local pointer into the cache
  // of the currently executing locale, and returns a non-zero dynamic
  // index value if it has done so. It does not do synchronization on
  // LOCALE-0 as it expects the caller to do so if necessary.
  export proc
  chpl_insertExternLocalPtrNoSync(ptr: c_ptr(void), idx: uint): uint {
    if !isDynamicLoadingEnabled then halt('Should not reach here!');

    const ret = if idx == 0
      then chpl_dynamicProcIdxCounter.fetchAdd(1)
      else idx;

    local do manage chpl_privatizedPtrCache.withWriteLock() {
      var found;
      if chpl_privatizedPtrCache.get(ptr, found) {
        halt('Multiple remaps should not be possible!');
        ret = found;
      } else {
        chpl_privatizedPtrCache.set(ptr, ret);
      }
    }

    return ret;
  }

  // This function is called by the compiler to lookup wide pointer indices.
  export proc chpl_dynamicProcIdxToLocalPtr(idx: uint): c_ptr(void) {
    var ret = if isDynamicLoadingEnabled
        then fetchLocalPtrForDynamicIdx(idx)
        else lookupPtrFromLocalFtable(idx);
    return ret;
  }

  // This function is called by the compiler to create wide pointer indices.
  export proc chpl_staticToDynamicProcIdx(idx: uint): uint {
    var ret = if isDynamicLoadingEnabled
        then fetchDynamicIdxForStaticIdx(idx)
        else idx;
    return ret;
  }
}
