/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

  private proc isLocalNoComm param {
    use ChplConfig;
    return compiledForSingleLocale();
  }

  param chpl_defaultProcBufferSize = 512;

  // Returns 'true' if compile-time configuration errors exist.
  proc configErrorsForDynamicLoading(param emitErrors: bool) param {
    use ChplConfig;

    if !useProcedurePointers {
      if emitErrors {
        compilerError('The experimental procedure pointer implementation ' +
                      'must be enabled to use dynamic loading. Set the ' +
                      'config param \'useProcedurePointers\' to \'true\'', 2);
      }
      return true;
    }

    return false;
  }

  proc isDynamicLoadingSupported param {
    return !configErrorsForDynamicLoading(emitErrors=false);
  }

  iter fanToAll(skip: locale) {
    for loc in Locales {
      if loc != skip then yield loc;
    }
  }

  proc shouldFanOut: bool {
    return isDynamicLoadingSupported && !isLocalNoComm && numLocales > 1;
  }

  // This counter is used to assign a unique 'wide index' to each procedure.
  // We start with '1' since the '0th' index is reserved to represent 'nil'.
  var chpl_dynamicProcIdxCounter: atomic int = 1;

  class chpl_LocalPtrCache {
    var guard: chpl_lockGuard(chpl_localBidirectionalMap(c_ptr(void), int));
  }

  // One cache of pointers guarded by a lock defined per locale. We use a
  // module-scope variable here to avoid relying on task-local storage as we
  // do for array privatization.
  //
  // TODO: Can we try changing this to be 'owned' instead of 'unmanaged'?
  //       When I do I seem to get a dereference 'nil' error on deinit...
  //
  pragma "locale private"
  var chpl_localPtrCache = new unmanaged chpl_LocalPtrCache();

  // If we are multi-locale, then we'll need to initialize the cache on the
  // remaining locales manually, since the module initializer is only
  // running on LOCALE-0.
  if shouldFanOut {
    coforall loc in fanToAll(skip=here) do on loc do local {
      // The local value will be nil despite a non-nilable type. This is OK.
      assert(chpl_localPtrCache == nil);
      chpl_localPtrCache = new unmanaged chpl_LocalPtrCache();
    }
  }

  proc deinit() {
    on Locales[0] do delete chpl_localPtrCache;

    // Behind a 'local' check to avoid throwing off compiler optimizations.
    if shouldFanOut {
      coforall loc in fanToAll(skip=here) do on loc {
        delete chpl_localPtrCache;
      }
    }
  }

  // This class wraps the errors emitted by the system 'dlopen'.
  class DynLoadError : Error {
    proc init(msg: string) {
      super.init(msg);
      init this;
    }
  }

  private inline
  proc localDynLoadWrapErrorUnlocked(): owned DynLoadError? {
    extern proc chpl_dlerror(): c_ptrConst(c_char);

    const errMsg = chpl_dlerror();
    if errMsg != nil {
      // TODO: The contents of this error can be ridiculous - we may want to
      //       parse it to produce something more legible? Also, we may want
      //       to do something if string construction fails (?) rather than
      //       just halt with a 'try!' here.
      const str = try! string.createCopyingBuffer(errMsg);
      return new DynLoadError?(str);
    }

    return nil;
  }

  private inline
  proc localDynLoadClearErrorUnlocked() do localDynLoadWrapErrorUnlocked();

  // All of the dynamic loading shims have to be behind this one single
  // lock (per locale) because they all need to access the same error
  // routine, and we need to guarantee that access to it is not racey.
  pragma "locale private"
  private var localDynLoadLock: chpl_LocalSpinlock;

  private inline
  proc localDynLoadClose(ptr: c_ptr(void), out err: owned DynLoadError?) {
    extern proc chpl_dlclose(handle: c_ptr(void)): c_int;

    manage localDynLoadLock {
      localDynLoadClearErrorUnlocked();

      const code = chpl_dlclose(ptr);
      if code != 0 {
        err = localDynLoadWrapErrorUnlocked();
        assert(err != nil);
      }
    }
  }

  private inline
  proc localDynLoadOpen(path: string, out err: owned DynLoadError?) {
    extern proc chpl_dlopen(path: c_ptrConst(c_char),
                            mode: c_int): c_ptr(void);
    extern const CHPL_RTLD_LAZY: c_int;

    var ret: c_ptr(void) = nil;

    manage localDynLoadLock {
      localDynLoadClearErrorUnlocked();

      ret = chpl_dlopen(path.c_str(), CHPL_RTLD_LAZY);
      if ret == nil {
        err = localDynLoadWrapErrorUnlocked();
        assert(err != nil);
      }
    }

    return ret;
  }

  private inline
  proc localDynLoadSymbolLookup(sym: string, handle: c_ptr(void),
                                out err: owned DynLoadError?) {
    extern proc chpl_dlsym(handle: c_ptr(void),
                           symbol: c_ptrConst(c_char)): c_ptr(void);

    var ret: c_ptr(void) = nil;

    manage localDynLoadLock {
      localDynLoadClearErrorUnlocked();

      ret = chpl_dlsym(handle, sym.c_str());
      if ret == nil {
        err = localDynLoadWrapErrorUnlocked();
        assert(err != nil);
      }
    }

    return ret;
  }

  proc checkForDynamicLoadingErrors(ref err) {
    // Note that procedure pointer errors were checked in module code.
    param errors = configErrorsForDynamicLoading(emitErrors=true);

    if errors || !isDynamicLoadingSupported {
      err = new DynLoadError('Dynamic loading is not supported');
      return true;
    }

    return false;
  }

  // A store of of all loaded binaries which lives on LOCALE-0. Each time a
  // binary is loaded an entry will be made in the store. The interface to the
  // stored info is the system pointer retrieved by 'dlopen'.
  class chpl_BinaryInfoStore {

    // The key is the 'dlopen' pointer returned on LOCALE-0. You are able to
    // 'dlopen' a symbol multiple times, so you can get the key as needed.
    // TODO: After doing so, do we need to call close to drop the internal
    //       refcount that is maintained by the OS?
    var handleToInfo: chpl_lockGuard(chpl_localMap(c_ptr(void),
                                     unmanaged chpl_BinaryInfo?));

    proc deinit() {
      ref m = handleToInfo.unsafeAccess();
      for slot in m.slots() do {
        if slot.val != nil then delete slot.val;
      }
    }
  }

  // This is one per entrypoint binary and lives on LOCALE-0.
  var chpl_binaryInfoStore = new owned chpl_BinaryInfoStore();

  // This class represents a "wide" binary. It contains the state necessary
  // to load a symbol from the binary on each locale.
  class chpl_BinaryInfo {
    var _lock: chpl_LocalSpinlock;

    // This refcount is bumped and dropped by the user-facing wrapper.
    var _refCount: atomic int = 0;

    // This is the path that was used to load the binary.
    const _path: string;

    // This is the set of loaded binary pointers, indexed by locale. It lives
    // on LOCALE-0. Since it is a local buffer it can only be accessed there!
    var _systemPtrs = new chpl_localBuffer(c_ptr(void), numLocales);

    // Local pointers for loaded symbols on LOCALE-0. These are used to evict
    // entries from the procedure pointer cache when the library is finally
    // closed. The value is a pair of "wide index" and the symbol name.
    var _procPtrToDataLocale0: chpl_localMap(c_ptr(void), (int, string));

    // A pointer to the parent store is used to coordinate load/unload.
    const _store: borrowed chpl_BinaryInfoStore;

    // Set only when '_close()' is called for the first time.
    var _closed = false;

    proc init(in path: string, in store: borrowed chpl_BinaryInfoStore) {
      this._path = path;
      this._store = store;
      init this;
    }

    proc deinit() {
      _close();
      if _procPtrToDataLocale0.needsDestroy() {
        import MemMove;
        for slot in _procPtrToDataLocale0.slots() do MemMove.destroy(slot);
      }
    }

    // Load a binary given a path.
    proc type create(path: string, out err: owned DynLoadError?) {
      var ret: unmanaged chpl_BinaryInfo? = nil;

      if checkForDynamicLoadingErrors(err) then return ret;

      on Locales[0] {
        const store = chpl_binaryInfoStore.borrow();
        var shouldCreateNewEntry = true;

        // Start by opening a system handle on LOCALE-0.
        var err0: owned DynLoadError?;
        var ptr0 = localDynLoadOpen(path, err0);

        // TODO: Need to propagate/combine this error instead.
        defer { if ptr0 then localDynLoadClose(ptr0, err0); }

        if err0 != nil || ptr0 == nil {
          if err0 != nil then err = err0;
          shouldCreateNewEntry = false;
        }

        // Check the LOCALE-0 store for an existing entry.
        if shouldCreateNewEntry {
          manage store.handleToInfo.read() as m {
            shouldCreateNewEntry = !m.get(ptr0, ret);
          }
        }

        // None found, so initialize a new binary info.
        if shouldCreateNewEntry {
          var bin = new owned chpl_BinaryInfo(path, chpl_binaryInfoStore);

          // Swap in pointer on LOCALE-0. This clears the variable.
          bin._systemPtrs[0] <=> ptr0;

          var errBuf = new chpl_localBuffer(owned DynLoadError?, numLocales);
          var numErrs: atomic int = 0;

          // Loop and attempt to open system handles on other locales.
          if shouldFanOut {
            coforall loc in fanToAll(skip=here) with (ref errBuf) do on loc {
              const n = (here.id: int);

              var err: owned DynLoadError?;
              var ptr = localDynLoadOpen(path, err);

              // TODO: Need to propagate/combine this error instead.
              defer { if ptr then localDynLoadClose(ptr, err); }

              if err {
                // The buffer is not wide! Assignment occurs on LOCALE-0.
                on Locales[0] do errBuf[n] = err;
                numErrs.add(1);
              } else if ptr {
                // Swap in pointer on LOCALE-0. This clears the variable.
                on Locales[0] do bin._systemPtrs[n] <=> ptr;

                ptr = nil;
              } else {
                // TODO: Construct an error instead.
                halt('Failed to load binary \'' + path + '\' on ' +
                     here.id:string + '!');
              }
            }
          }

          if numErrs.read() > 0 {
            // If there were errors, then report the first error.
            // TODO: Consolidate the reported errors.
            for i in 0..<errBuf.size {
              if errBuf[i] {
                err = errBuf[i];
                break;
              }
            }
          } else {
            // Otherwise, try to add the newly created info to the store on
            // LOCALE-0, or give up if another task already beat us to it.

            for i in 0..<bin._systemPtrs.size {
              // Make sure that all locales have succeeded in 'dlopen()'.
              assert(bin._systemPtrs[i] != nil);
            }

            manage store.handleToInfo.write() as m {
              var found = m.get(bin._systemPtrs[0], ret);
              if !found {
                ret = owned.release(bin);
                const ok = m.add(ret!._systemPtrs[0], ret);
                assert(ok);
              }
            }
          }
        }
      }

      return ret;
    }

    // TODO: Also need to evict pointer cache entries.
    proc _close() {
      assert(_refCount.read() == 0);
      if _closed then return;

      on Locales[0] do manage this._lock {
        if !_closed {
          _closed = true;

          for i in 0..<_systemPtrs.size {
            const ptr = _systemPtrs[i];

            if ptr != nil then on Locales[i] {
              var err: owned DynLoadError?;
              localDynLoadClose(ptr, err);

              // TODO: Figure out how to propagate this instead of halting?
              if err != nil then halt(err!.message());
            }
          }
        }
      }
    }

    inline proc bumpRefCount() do _refCount.add(1);
    inline proc dropRefCount() {
      if _refCount.read() <= 0 then return;
      _refCount.sub(1);
    }

    // This private method emplaces a new index into the local procedure
    // pointer cache on each locale, and then it returns the index. It
    // may lock on local caches but it does not ensure global consistency.
    // The caller should do that by holding 'this._lock'.
    //
    // Note that the passed in pointer only has meaning on 'this.locale'.
    inline proc _emplaceNewIndexUnlocked(sym: string, ptrOnThis: c_ptr(void),
                                         out err: owned DynLoadError?) {
      var errBuf = new chpl_localBuffer(owned DynLoadError?, numLocales);
      var numErrs: atomic int;

      // Get the wide index to use by interning on 'this.locale'. By passing
      // in '0' we tell the routine to assign us a unique index to use.
      var ret = chpl_insertExternLocalPtrNoSync(ptrOnThis, this.locale.id);
      assert(ret != 0);

      if shouldFanOut {
        coforall loc in fanToAll(skip=this.locale) do on loc {
          const n = here.id : int;
          var handle: c_ptr(void);
          on this do handle = _systemPtrs[n];

          var err;
          const ptr = localDynLoadSymbolLookup(sym, handle, err);

          if ptr == nil && err == nil {
            const msg = 'Failed to load symbol on locale ' + n:string;
            err = new DynLoadError(msg);
          }

          if err != nil {
            on this do errBuf[n] = err;
            numErrs.add(1);

          } else {
            assert(ptr != nil);
            const got = chpl_insertExternLocalPtrNoSync(ptr, ret);
            assert(got == ret);
          }
        }
      }

      if numErrs.read() > 0 {
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
        ret = 0;
      }

      return ret;
    }

    // TODO: 'T' must be a procedure type, but we cannot restrict it yet,
    // e.g., we cannot write "type t: proc". As a short-term workaround
    // we could add a primitive "any procedure type" to use instead.
    proc loadSymbol(sym: string, type t, out err: owned DynLoadError?) {
      type P = chpl_toExternProcType(chpl_toWideProcType(t));
      var idx: int = 0;

      if !isProcedure(t) || isClass(t) {
        compilerError('The type passed to \'loadSymbol\' must be ' +
                      'a procedure type');
      }

      if chpl_isLocalProc(t) {
        // Users shouldn't be able to easily construct local types right now.
        compilerError('The procedure type passed to \'loadSymbol\'' +
                      'should be wide');
      }

      if checkForDynamicLoadingErrors(err) {
        return __primitive("cast", P, 0);
      }

      on Locales[0] {
        const handle = _systemPtrs[here.id];
        assert(handle != nil);

        // Call the system lookup routine, e.g., 'dlsym'.
        var errOnThis;
        const ptrOnThis = localDynLoadSymbolLookup(sym, handle, errOnThis);

        if errOnThis != nil {
          err = errOnThis;

        } else if ptrOnThis == nil {
          // There was an error while calling the system lookup routine.
          err = new DynLoadError('Failed to locate symbol: ' + sym);

        } else manage this._lock {
          // The following section must all happen while holding the lock.
          // Check to see if the wide index is already stored stored here.
          var data;
          const found = _procPtrToDataLocale0.get(ptrOnThis, data);

          if found {
            // In the fast path there was already an entry for the symbol.
            const (got, sym) = data;
            assert(got != 0);
            idx = got;

          } else {
            // Otherwise there was no entry, so add pointers to all caches.
            idx = _emplaceNewIndexUnlocked(sym, ptrOnThis, err);

            if idx != 0 {
              // A non-nil index was returned, so we add a new entry.
              const data = (idx, sym);
              const added = _procPtrToDataLocale0.add(ptrOnThis, data);
              assert(added);
            }
          }
        }
      }

      // Fine if 'ret' is '0', that is the wide-index equivalent of 'nil'.
      const ret = __primitive("cast", P, idx);

      return ret;
    }
  }

  // This is a local buffer that only provides '[]' indexing and halts on an
  // out-of-range index value. It halts instead of failing nicely because
  // it should not fail (and perhaps we should disable the bounds checks).
  record chpl_localBuffer {
    type T;
    var _ptr: c_ptr(T) = nil;
    var _size: int = 0;

    proc init(type T) {
      this.T = T;
      init this;
    }

    // Returns a new 'chpl_localBuffer' containing the old contents.
    // The new buffer memory is 'zeroed'.
    inline proc ref reallocateAndClear(size: int) {
      assert(size >= 0);

      // Create a shallow copy of 'this' to return.
      var ret: this.type;
      ret._ptr = _ptr;
      ret._size = _size;

      // Reallocate the current buffer.
      this._size = size;
      if size > 0 then this._ptr = allocate(T, (size : c_size_t), true);
      if _ptr == nil then halt('Out of memory!');

      return ret;
    }

    proc init(type T, size: int) {
      this.T = T;
      init this;
      if size > 0 then reallocateAndClear(size);
      assert(_size == size && (_size == 0 || _ptr != nil));
    }

    proc ref deinit() {
      if _ptr != nil then deallocate(_ptr);
    }

    proc init=(rhs: chpl_localBuffer(?T)) {
      this.init(T, rhs.size);
      forall i in 0..<rhs.size do this[i] = rhs[i];
    }

    operator =(lhs: this.type, rhs: this.type) {
      compilerError('Should not be called!');
    }

    inline proc size do return _size;
    inline proc ptr do return _ptr;

    inline proc this(idx: integral) ref {
      if boundsChecking && (idx < 0 || idx >= _size) then
        halt('Out of bounds!');
      return _ptr[idx];
    }

    inline iter these() ref {
      for idx in 0..<_size do yield _ptr[idx];
    }
  }

  // This is a local hashtable that uses linear probing. I reinvent the wheel
  // instead of using 'ChapelHashtable' so that the latter can have the
  // luxury of using procedure pointers in its implementation (the code here
  // is helping to implement procedure pointers, so it cannot itself use
  // them without some difficulty).
  //
  // This hashtable does not use a tombstone field and expects key types to be
  // simple scalars or POD types. It reserves a key with all 0-bits for use as
  // the empty key. The value type may be any moveable type.
  //
  record chpl_localMap {
    type K, V;
    record slot {
      var key;
      var val;
    }

    var _slots: chpl_localBuffer(slot(K, V));
    var _size: int = 0;

    proc init(type K, type V) {
      this.K = K;
      this.V = V;
      init this;
      if !isPODType(K) then compilerError('Key type must be POD!');
    }

    inline proc size do return _size;

    inline proc needsDestroy() param do {
      import MemMove;
      return MemMove.needsDestroy(V);
    }

    inline proc _baseBufferSize param do return chpl_defaultProcBufferSize;
    inline proc _minLoadFactor param do return 0.10;
    inline proc _maxLoadFactor param do return 0.70;

    inline proc _loadFactor(): real do {
      var ret = if _slots.size <= 0 then 1.0 else _size:real / _slots.size;
      return ret;
    }

    inline proc _isKeyZeroBits(x: ?t) where isAnyCPtr(t) do return x == nil;
    inline proc _isKeyZeroBits(x: ?t) where isIntegral(t) do return x == 0;
    inline proc _isKeyZeroBits(x) do return (x: int) == 0;

    inline proc _shouldTryToExpand() do return _loadFactor() > _maxLoadFactor;
    inline proc _shouldTryToShrink() do return _loadFactor() < _minLoadFactor;

    // TODO: Switch from doubling to linear to decrease memory pressure.
    inline proc _nextBufferSize(grow: bool) {
      const num = if grow then _slots.size * 2 else _slots.size / 2;
      return if num < _baseBufferSize then _baseBufferSize else num;
    }

    // TODO: Look into adding 'c_ptr.hash()' and 'c_ptrConst.hash()'.
    inline proc _hash(x: ?t): int where isAnyCPtr(t) {
      const num = __primitive("object2int", x);
      return _hash(num, check=false);
    }

    inline proc _hash(x: ?t, param check=true): int {
      use ChapelHashing;
      const ret = (x.hash() : int) & max(int);
      return ret;
    }

    inline iter _walk(start: int=0) {
      assert(0 <= start && start < _slots.size);
      for idx in start..<_slots.size do yield idx;
      for idx in 0..<start do yield idx;
    }

    inline iter slots(liveSlotsOnly: bool=true) ref {
      if _slots.size > 0 {
        for idx in _walk() {
          ref slot = _slots[idx];
          if !liveSlotsOnly || !_isKeyZeroBits(slot.key) then yield slot;
        }
      }
    }

    proc ref _resizeIfNeeded() {
      if !_shouldTryToExpand() && !_shouldTryToShrink() then return;

      const newBufferSize = if _shouldTryToExpand()
          then _nextBufferSize(grow=true)
          else _nextBufferSize(grow=false);

      // Nothing to do.
      if newBufferSize <= _slots.size then return;

      // Reallocate a new empty buffer, set 'size' to 0 for rehashing.
      const oldBuf = _slots.reallocateAndClear(newBufferSize);
      const oldSize = _size;
      _size = 0;

      // Rehash all the live slots from the old buffer. Use 'moveFrom'
      // so that we can avoid making a copy of the existing value. The
      // key is a simple scalar type so it is trivially copied.
      for slot in oldBuf {
        if !_isKeyZeroBits(slot.key) {
          import MemMove;
          const added = add(slot.key, MemMove.moveFrom(slot.val));
          assert(added);
        }
      }

      // Consistency check.
      assert(_size == oldSize);
    }

    proc _findSlotIdx(in key: K, ref idx: int): bool {
      assert(!_isKeyZeroBits(key));

      const start = _hash(key) % _slots.size;
      for w in _walk(start) {
        const ref slot = _slots[w];
        if _isKeyZeroBits(slot.key) || slot.key == key {
          idx = w;
          return true;
        }
      }
      return false;
    }

    // Returns 'true' if the key was added in the map for the first time.
    proc ref add(in key: K, in val: V, param addOnlyIfAbsent=false): bool {
      if _isKeyZeroBits(key) then return false;

      _resizeIfNeeded();

      var idx = 0;
      var found = _findSlotIdx(key, idx);
      assert(found && (0 <= idx && idx < _slots.size));

      ref slot = _slots[idx];
      if _isKeyZeroBits(slot.key) {
        import MemMove;

        // The slot is junk memory, so perform first-time initialization.
        MemMove.moveInitialize(slot.key, key);
        MemMove.moveInitialize(slot.val, val);
        _size += 1;
        return true;

      } else if (slot.key == key) {
        // The slot is initialized, so just assign the value.
        if !addOnlyIfAbsent then slot.val = val;
      }

      return false;
    }

    // Returns 'true' if the key was found.
    proc get(in key: K, out val: V): bool {
      if _isKeyZeroBits(key) then return false;

      if _size == 0 then return false;

      var idx = 0;
      var found = _findSlotIdx(key, idx);
      if !found then return false;

      ref slot = _slots[idx];
      if slot.key == key {
        val = slot.val;
        return true;
      }

      return false;
    }
  }

  // This class is a local bidirectional map.
  record chpl_localBidirectionalMap {
    type K, V;
    var _keyToVal: chpl_localMap(K, V);
    var _valToKey: chpl_localMap(V, K);

    inline proc size do return _valToKey.size;

    proc ref add(in key: K, in val: V) {
      var ret = false;

      on this do {
        if _keyToVal.add(key, val, addOnlyIfAbsent=true) {
          const added = _valToKey.add(val, key);
          assert(added);
          ret = true;
        }
      }

      return ret;
    }

    proc get(key: K, out val: V) {
      var ret = false;
      on this do ret = _keyToVal.get(key, val);
      return ret;
    }

    proc get(val: V, out key: K) {
      var ret = false;
      on this do ret = _valToKey.get(val, key);
      return ret;
    }
  }

  private proc lookupPtrFromLocalFtable(idx: int): c_ptr(void) {
    extern proc chpl_get_ftable(): c_ptr(c_ptr(void));
    extern const chpl_ftableSize: int(64);
    assert(idx < chpl_ftableSize);
    const ftable = chpl_get_ftable();
    return ftable[idx];
  }

  inline proc fetchLocalPtrForDynamicIdx(idx: int): c_ptr(void) {
    var ret: c_ptr(void) = nil;

    // There should always be an entry for this dynamic index in the local
    // cache because the local roots are emplaced when a function value is
    // created. So just halt if this is not the case.
    if idx != 0 {
      local do manage chpl_localPtrCache.guard.read() as m {
        const found = m.get(idx, ret);
        assert(found);
        assert(ret != nil);
      }
    }

    return ret;
  }

  inline proc fetchDynamicIdxForStaticIdx(idx: int): int {
    var ret: int = 0;

    // Return the local pointer for this locale if it's already set.
    local do manage chpl_localPtrCache.guard.read() as m {
      const ptr = lookupPtrFromLocalFtable(idx);
      if m.get(ptr, ret) then return ret;
    }

    // Otherwise, synchronize on LOCALE-0...
    on Locales[0] {
      manage chpl_localPtrCache.guard.write() as m {
        var requestedUniqueIdx = false;

        // Use the value of the pointer on LOCALE-0 as the map key.
        const ptr = lookupPtrFromLocalFtable(idx);

        if !m.get(ptr, ret) {
          // If we did not look up an existing entry, then we are the task
          // that will set the map entries for this pointer. Set the index
          // on LOCALE-0 to claim the job.
          ret = chpl_dynamicProcIdxCounter.fetchAdd(1);
          m.add(ptr, ret);
          requestedUniqueIdx = true;
        }

        // While holding the LOCALE-0 lock, set on all other locales.
        if requestedUniqueIdx && numLocales > 1 {
          coforall loc in Locales[1..] do on loc {
            local do manage chpl_localPtrCache.guard.write() as m {
              const ptr = lookupPtrFromLocalFtable(idx);
              m.add(ptr, ret);
            }
          }
        }
      }
    }

    assert(ret >= 0);

    return ret;
  }

  // This function is called by the dynamic loading code. If the formal 'idx'
  // is '>' zero, it will be used as the dynamic index value. Otherwise, the
  // routine will fetch a unique value to use as the dynamic index value.
  //
  // This routine attempts to emplace a local pointer into the cache of the
  // currently executing locale, and returns a non-zero dynamic index value
  // if it has done so. It does not do synchronization on LOCALE-0 as it
  // expects the caller to do so if necessary.
  export proc
  chpl_insertExternLocalPtrNoSync(ptr: c_ptr(void), idx: int): int {
    const ret = if idx == 0
      then chpl_dynamicProcIdxCounter.fetchAdd(1)
      else idx;

    local do manage chpl_localPtrCache.guard.write() as m {
      // If 'set()' returns 'false' then the index was already in the map!
      // We just overwrote it, but it shouldn't have been set in the first
      // place so the only thing to do is halt.
      if !m.add(ptr, ret) {
        halt('Procedure pointer duplicately mapped!');
      }
    }

    return ret;
  }

  // This function is called by the compiler to lookup wide pointer indices.
  export proc chpl_dynamicProcIdxToLocalPtr(idx: int): c_ptr(void) {
    const ret = if isDynamicLoadingSupported
          then fetchLocalPtrForDynamicIdx(idx)
          else lookupPtrFromLocalFtable(idx);
    return ret;
  }

  // This function is called by the compiler to create wide pointer indices.
  export proc chpl_staticToDynamicProcIdx(idx: int): int {
    const ret = if isDynamicLoadingSupported
          then fetchDynamicIdxForStaticIdx(idx)
          else idx;
    return ret;
  }
}
