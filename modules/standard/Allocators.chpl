
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


/*

*/
@unstable(category="experimental", reason="The Allocators module is under development and does not have a stable interface yet")
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

  @chpldoc.nodoc
  /* See docs for :proc:`~Allocators.newWithAllocator` */
  inline proc newWithAllocator(ref allocator: ?, type T): T {
    return __primitive("new with allocator", allocator, T);
  }

  /*
    Allocate a new class with type ``T`` using the given ``allocator``. This is a drop-in replacement for ``new``.

    Example:

    .. code-block:: chapel

       class MyClass { var x: int; }
       var allocator = new bumpPtrMemPool(1024);
       // The following two lines are equivalent, but the second one uses the allocator
       var x = new unmanaged MyClass(1);
       var x = newWithAllocator(allocator, unmanaged MyClass, 1);

    .. warning::
       Using a managed class type may causes memory issues with some allocators.
       It is recommend to use ``unmanaged``.
  */
  inline proc newWithAllocator(ref allocator: ?, type T, args...): T {
    return __primitive("new with allocator", allocator, T, (...args));
  }

  @chpldoc.nodoc
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
    param parSafe: bool = false;
    var size: int(64);
    var basePtr: c_ptr(int(8));
    var ptr: c_ptr(int(8));
    var lock_ = if parSafe then new _LockWrapper() else none;

    proc init(size: int(64), param parSafe: bool = false) {
      this.parSafe = parSafe;
      this.size = size;
      if boundsChecking {
        if this.size <= 0 {
          halt("bumpPtrMemPool: size must be positive");
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

    @chpldoc.nodoc
    inline proc _lock()
      do if parSafe then lock_.lock();

    @chpldoc.nodoc
    inline proc _unlock()
      do if parSafe then lock_.unlock();

    proc ref allocate(n: int): c_ptr(void) {
      _lock();
      if boundsChecking {
        if n <= 0 {
          halt("bumpPtrMemPool.allocate: n must be positive");
        }
      }

      // TODO: should this be uncommented? Maybe it should be a separate allocator
      // ptr = alignup(ptr, 16): c_ptr(int(8));
      if boundsChecking {
        if (ptr + n):c_intptr > (basePtr + size):c_intptr {
          halt("bumpPtrMemPool.allocate: out of memory");
        }
      }

      var p = ptr;
      ptr += n;

      _unlock();
      return p;
    }
    proc ref deallocate(p: c_ptr(void)) {
      // there is currently no API to call this method
      // no-op
    }
  }
}
