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
  private use List;
  private use Reflection;
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

  class chpl_LocalPtrCache {
    var procPtrToIdx = new chpl_localMap(c_ptr(void), uint);
    var idxToProcPtr = new chpl_localMap(uint, c_ptr(void));
    var lock: chpl_LocalSpinlock;

    // TODO: A gadget to let one try out different types of locks. I had
    // written a RW-lock in 'ChapelLocks' but was having trouble getting
    // it to work. If we ever want to try something besides a spinlock
    // then just swap the locks in here by adding overloads controlled
    // by where clauses.
    inline proc useSpinlock param do return true;
    inline proc withReadLock() ref where useSpinlock do return lock;
    inline proc withWriteLock() ref where useSpinlock do return lock;
  }

  // One cache of pointers guarded by a RW-lock defined per locale.
  pragma "locale private"
  var chpl_perLocalePtrCache = new unmanaged chpl_LocalPtrCache();

  if numLocales > 1 {
    forall loc in Locales[1..] with (ref chpl_perLocalePtrCache) do on loc {
      assert(chpl_perLocalePtrCache == nil);
      chpl_perLocalePtrCache = new unmanaged chpl_LocalPtrCache();
    }
  }

  // TODO: The moment I try to delete these pointers some locales will crash
  // with (what I assume is) a double-free or some other memory corruption
  // on program shutdown. You don't really see it single-locale unless
  // jemalloc allocates a large enough chunk, so set the flag
  // 'chpl_defaultProcBufferSize=512' to see. The crash seems to happen in
  // the qthreads layer, perhaps while cleaning up TLS.
  proc deinit() {

    // These should be empty (and thus trivially deleted) on all but the
    // entrypoint Chapel binary, which will work with the runtime to be
    // the authority for storing pointers. Have Locale[0] drive the loop
    // so we don't delete things multiple times.
    /*
    if here == Locales[0] {
      for loc in Locales do on loc {
        if chpl_perLocalePtrCache != nil then delete chpl_perLocalePtrCache;
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
        if _ptr == nil then halt('Ran out of memory when initializing!');
      }
    }

    proc deinit() {
      if _ptr != nil then deallocate(_ptr);
    }

    inline proc size do return _size;

    inline proc this(idx: integral) ref {
      if idx >= _size then halt('Index out of bounds!');
      return _ptr[idx];
    }
  }

  // This type is a very simple local hashtable that uses linear probing.
  // I've opted to reinvent the wheel instead of using 'ChapelHashtable'
  // to not preclude the latter from using procedure pointers in its
  // implementation. The code used to define the procedure lookup code
  // cannot itself use procedure pointers.
  //
  // The map only uses simple scalars for keys and reserves the key '0'
  // for tombstones, but I don't bother guaranteeing those things here
  // since it's only used internally.
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
    inline proc _shouldTryToResize() do return _size >= (_slots.size >> 1);
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

    proc _resize(size: uint) {
      halt('Not implemented yet!');
    }

    inline proc _findSlotIdx(in key: K, out idx: uint, resize: bool): bool {
      if _isKeyZeroBits(key) then {
        assert(false);
        return false;
      }

      if resize && _shouldTryToResize() then _resize(_nextExpandSize());

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
  // I think it will be important for performance that 'on' statements
  // and other lexically driven things that have no dynamic execution
  // requirement continue to use a simple table lookup rather than a
  // dynamic table lookup. So something will have to be done there.
  //
  // We need the ftable to get the local pointer from the static index.
  // This is safe to do since it is locale private and we know we are
  // only referring to the one in our binary (the same cannot be said
  // for the runtime code once we dynamic link it).
  //
  // TODO: If I make this inline I get an internal compiler error.
  private proc lookupPtrFromLocalFtable(idx: uint) {
    extern const chpl_ftable: c_ptr(c_ptr(void));
    extern const chpl_ftableSize: int(64);
    assert(idx < chpl_ftableSize);
    return chpl_ftable[idx];
  }

  private inline proc doFetchLocalPtrForDynamicIdx(idx: uint): c_ptr(void) {
    var ret: c_ptr(void) = nil;

    manage chpl_perLocalePtrCache.withReadLock() {
      var found = chpl_perLocalePtrCache.idxToProcPtr.get(idx, ret);
      assert(found);
    }
    assert(ret != nil);
    return ret;
  }

  private inline proc doFetchDynamicIdxForStaticIdx(idx: uint): uint {
    var ret: uint = 0;

    // Return the local pointer for this locale if it's already set.
    // Try taking the read-lock once, first, since it will always be
    // under less contention (the lock is read-favored).
    manage chpl_perLocalePtrCache.withReadLock() {
      var localPtr = lookupPtrFromLocalFtable(idx);
      if chpl_perLocalePtrCache.procPtrToIdx.get(localPtr, ret) {
        return ret;
      }
    }

    var requestedUniqueIdx = false;
    on Locales[0] {
      // Check once again since another task might have beaten us.
      // Since only one task can be the first to write-lock on
      // Locale[0] and set its pointer, we can exit the lookup if
      // the dynamic index is already set.

      manage chpl_perLocalePtrCache.withWriteLock() {
        // Use the value of the pointer on Locale[0] to fetch.
        var localPtr = lookupPtrFromLocalFtable(idx);

        if !chpl_perLocalePtrCache.procPtrToIdx.get(localPtr, ret) {
          // If we did not look up an existing entry, then we are
          // the task that will set the map entries for this
          // pointer. Set the index on Locale[0] to claim the job.
          ret = chpl_dynamicProcIdxCounter.fetchAdd(1);
          chpl_perLocalePtrCache.procPtrToIdx.set(localPtr, ret);
          chpl_perLocalePtrCache.idxToProcPtr.set(ret, localPtr);
          requestedUniqueIdx = true;
        }
      }
    }

    if !requestedUniqueIdx || numLocales == 1 {
      assert(ret != 0);
      return ret;
    }

    // If we reached here then we loop and set on all remaining locales.
    forall loc in Locales[1..] with (ref chpl_perLocalePtrCache) do on loc {
      manage chpl_perLocalePtrCache.withWriteLock() {
        var localPtr = lookupPtrFromLocalFtable(idx);
        var ok1 = chpl_perLocalePtrCache.procPtrToIdx.set(localPtr, ret);
        var ok2 = chpl_perLocalePtrCache.idxToProcPtr.set(ret, localPtr);
        assert(ok1);
        assert(ok2);
      }
    }

    return ret;
  }

  export proc chpl_dynamicProcIdxToLocalPtr(idx: uint): c_ptr(void) {
    var ret = if isDynamicLoadingEnabled
        then doFetchLocalPtrForDynamicIdx(idx)
        else lookupPtrFromLocalFtable(idx);
    return ret;
  }

  export proc chpl_staticToDynamicProcIdx(idx: uint): uint {
    var ret = if isDynamicLoadingEnabled
        then doFetchDynamicIdxForStaticIdx(idx)
        else idx;
    return ret;
  }
}
