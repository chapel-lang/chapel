

module Allocators {
  private use CTypes;
  private use Reflection;
  import ChapelLocks;

  private inline proc alignup(ptr: c_ptr(void), param alignment): c_ptr(void) {
    return ((ptr:c_intptr + (alignment-1)) & ~(alignment-1)):c_intptr:c_ptr(void);
  }

  // TODO: this should just use interfaces
  proc chpl_isValidAllocator(allocator: ?) param {
    var dummyBytes: int;

    if ! canResolveMethod(allocator, "allocate", dummyBytes) {
      return false;
    }
    type resType = allocator.allocate(dummyBytes).type;
    if resType != c_ptr(void) {
      return false;
    }

    var dummyPtr: c_ptr(void);

    if ! canResolveMethod(allocator, "deallocate", dummyPtr) {
      return false;
    }

    return true;
  }

  inline proc newWithAllocator(ref allocator: ?, type T): T {
    return __primitive("new with allocator", allocator, T);
  }
  inline proc newWithAllocator(ref allocator: ?, type T, args...): T {
    return __primitive("new with allocator", allocator, T, (...args));
  }

  class _LockWrapper {
    type lockType = ChapelLocks.chpl_LocalSpinlock;
    var lockVar = new lockType();
    inline proc lock() {
      lockVar.lock();
    }
    inline proc unlock() {
      lockVar.unlock();
    }
  }


  record bumpPtrMemPool {
    param locking: bool = false;
    var size: int(64);
    var basePtr: c_ptr(int(8));
    var ptr: c_ptr(int(8));
    var lock_ = if locking then new _LockWrapper() else none;


    proc init(size: int(64), param locking: bool = false) {
      this.locking = locking;
      this.size = size;
      if boundsChecking {
        if this.size <= 0 {
          halt("chpl_memPool: size must be positive");
        }
      }
      basePtr = CTypes.allocate(int(8), this.size.safeCast(c_size_t));
      ptr = basePtr;
      init this;
    }
    proc deinit() {
      if basePtr then
        CTypes.deallocate(basePtr);
    }

    inline proc _lock() {
      if locking {
        lock_.lock();
      }
    }
    inline proc _unlock() {
      if locking {
        lock_.unlock();
      }
    }

    pragma "allocator"
    proc ref allocate(n: int): c_ptr(void) {
      _lock();
      if boundsChecking {
        if n <= 0 {
          halt("chpl_memPool.allocate: n must be positive");
        }
      }

      // ptr = alignup(ptr, 16): c_ptr(int(8));

      if boundsChecking {
        if (ptr + n):c_intptr > (basePtr + size):c_intptr {
          halt("chpl_memPool.allocate: out of memory");
        }
      }

      var p = ptr;
      ptr += n;

      _unlock();
      return p;
    }
    proc ref deallocate(p: c_ptr(void)) {
      // no-op
    }
  }
}
