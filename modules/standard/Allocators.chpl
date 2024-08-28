
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


/* Provides custom allocators for Chapel objects on the heap.

  This module provides an interface to customize how Chapel objects are
  allocated on the heap. This is opted into by changing calls to
  ``new``/``delete`` into :proc:`newWithAllocator`/:proc:`deleteWithAllocator`. This
  new functions allow Chapel code to customize how objects are allocated and
  deallocated.

  For example, the following code demonstrates how the :type:`mallocWrapper`
  allocator can be used to allocate a Chapel class.

  .. code-block:: chapel

     class MyClass { var x: int; }
     var alloc = new mallocWrapper(1024);
     var x = newWithAllocator(alloc, unmanaged MyClass, 1);
     writeln(x);
     deleteWithAllocator(alloc, x);

  Custom allocators can be defined as classes or records that implement the
  :interface:`allocator` interface. The interface has two methods:
  :proc:`~allocator.Self.allocate` and :proc:`~allocator.Self.deallocate`. The
  :proc:`~allocator.Self.allocate` method accepts an integer ``n`` and returns
  a :type:`~CTypes.c_ptr` to the allocated memory. The
  :proc:`~allocator.Self.deallocate` method accepts a :type:`~CTypes.c_ptr` to
  the allocated memory. Allocators are free to implement their own memory
  management strategies.

  Limitations:
    * The :proc:`newWithAllocator` and :proc:`deleteWithAllocator` procedures
      are meant to be stand-ins pending a more elegant syntax to use custom
      allocators.
    * This module currently only supports allocating Chapel classes. In the
      future we hope to support other heap objects like arrays.
    * Allocating managed (:type:`~OwnedObject.owned`/:type:`~SharedObject.shared`)
      Chapel classes are not supported, only ``unmanaged`` classes are supported.

*/
@unstable(category="experimental", reason="The Allocators module is under development and does not have a stable interface yet")
module Allocators {
  private use CTypes;
  private use Reflection;
  import ChapelLocks;
  private use ChplConfig;

  private inline proc alignup(ptr: c_ptr(void), param alignment): c_ptr(void)
    where alignment > 0 && (alignment & (alignment - 1)) == 0 {

    param mask = alignment - 1;
    const iptr = ptr:c_intptr;
    const alignedPtr = (iptr + mask) & ~mask;
    return alignedPtr:c_ptr(void);
  }

  /*
    All allocators must implement this interface. The interface has two
    methods: :proc:`~allocator.Self.allocate` and
    :proc:`~allocator.Self.deallocate`. The :proc:`~allocator.Self.allocate`
    method accepts an integer size and returns a pointer to the allocated
    memory. The :proc:`~allocator.Self.deallocate` method accepts a pointer
    to the allocated memory.

    Allocators may either be classes or records.
  */
  interface allocator {
    /*
      Allocate memory for ``n`` bytes.

      :arg n: The number of bytes to allocate.
      :returns: A ``c_ptr(void)`` to the allocated memory.
    */
    proc ref Self.allocate(n: int): c_ptr(void);
    /*
      Deallocate memory at ``p``.

      :arg p: The pointer to the allocated memory.
    */
    proc ref Self.deallocate(p: c_ptr(void));
  }

  private proc newTypeCheckHelper(type T) param {
    if !isClassType(T) then
      compilerError(T:string + " is not a class", 2);
    if !isUnmanagedClassType(T) then
      compilerError(T:string + " is not unmanaged", 2);
  }
  private proc checkInterfaceHelper(alloc) param {
    if __primitive("implements interface", alloc, allocator) == 2 {
      compilerError(alloc.type:string + " does not implement 'allocator'", 2);
    }
  }

  /*
    Allocate a new unmanaged class with type ``T`` by invoking the
    :proc:`~allocator.Self.allocate` method of the given ``alloc``. This is a
    drop-in replacement for ``new``.

    Example:

    .. code-block:: chapel

       class MyClass { var x: int; }
       var allocator = new bumpPtrMemPool(1024);
       // The following two lines are equivalent, but the second one uses the allocator
       var x = new unmanaged MyClass(1);
       var x = newWithAllocator(allocator, unmanaged MyClass, 1);

    .. note::

       Named argument passing will not work with this function currently.
  */
  pragma "docs only"
  inline proc newWithAllocator(alloc: allocator, type T, args...): T {
    compilerError("docs-only newWithAllocator should not be compiled");
  }

  @chpldoc.nodoc
  inline proc newWithAllocator(ref alloc: record, type T): T {
    checkInterfaceHelper(alloc);
    newTypeCheckHelper(T);
    return __primitive("new with allocator", alloc, T);
  }
  @chpldoc.nodoc
  inline proc newWithAllocator(const ref alloc: class, type T): T {
    checkInterfaceHelper(alloc);
    newTypeCheckHelper(T);
    return __primitive("new with allocator", alloc, T);
  }

  @chpldoc.nodoc
  inline proc newWithAllocator(ref alloc: record, type T, args...): T {
    checkInterfaceHelper(alloc);
    newTypeCheckHelper(T);

    return __primitive("new with allocator", alloc, T, (...args));
  }

  @chpldoc.nodoc
  inline proc newWithAllocator(const ref alloc: class, type T, args...): T {
    checkInterfaceHelper(alloc);
    newTypeCheckHelper(T);

    return __primitive("new with allocator", alloc, T, (...args));
  }

  /*
    Delete the ``objects`` by invoking the :proc:`~allocator.Self.deallocate`
    method of the given :interface:`allocator`. This is a drop-in replacement
    for ``delete``.
  */
  pragma "docs only"
  inline proc deleteWithAllocator(alloc: allocator, objects...) {
    compilerError("docs-only deleteWithAllocator should not be compiled");
  }

  @chpldoc.nodoc
  inline proc deleteWithAllocator(ref alloc: record, objects...?k) {
    checkInterfaceHelper(alloc);
    for param i in 0..#k {
      newTypeCheckHelper(objects(i).type);
      if compiledForSingleLocale() {
                         chpl__deleteWithAllocator(alloc, objects(i));
      } else {
        on objects(i) do chpl__deleteWithAllocator(alloc, objects(i));
      }
    }
  }
  @chpldoc.nodoc
  inline proc deleteWithAllocator(const ref alloc: class, objects...?k) {
    checkInterfaceHelper(alloc);
    for param i in 0..#k {
      newTypeCheckHelper(objects(i).type);
      if compiledForSingleLocale() {
                         chpl__deleteWithAllocator(alloc, objects(i));
      } else {
        on objects(i) do chpl__deleteWithAllocator(alloc, objects(i));
      }
    }
  }

  @chpldoc.nodoc
  inline proc chpl__deleteWithAllocator(const ref alloc: class, const obj) {
    var p = c_ptrTo(obj);
    if obj != nil then
      obj!.deinit();
    alloc.deallocate(p);
  }
  @chpldoc.nodoc
  inline proc chpl__deleteWithAllocator(ref alloc: record, const obj) {
    var p = c_ptrTo(obj);
    if obj != nil then
      obj!.deinit();
    alloc.deallocate(p);
  }

  @chpldoc.nodoc
  class _LockWrapper {
    type lockType = ChapelLocks.chpl_LocalSpinlock;
    var lockVar = new lockType();
    inline proc lock() do lockVar.lock();
    inline proc unlock() do lockVar.unlock();
  }

  /*
    A simple allocator that acts as a wrapper around :proc:`CTypes.allocate`
    and :proc:`CTypes.deallocate`.
  */
  class mallocWrapper: allocator {
    /*
      Allocate memory for ``n`` bytes.

      :arg n: The number of bytes to allocate.
      :returns: A ``c_ptr(void)`` to the allocated memory.
    */
    proc allocate(n: int): c_ptr(void) {
      return CTypes.allocate(int(8), n.safeCast(c_size_t));
    }
    /*
      Deallocate memory at ``p``.

      :arg p: The pointer to the allocated memory.
    */
    proc deallocate(p: c_ptr(void)) {
      CTypes.deallocate(p);
    }
  }


  /*
    The is a simple bump pointer allocator that is useful for allocating many
    small objects.

    This allocator makes a single memory allocation in a contiguous block,
    calls to :proc:`~bumpPtrMemPool.allocate` will return the next available
    address. This allocator is not thread-safe by default, but can be made
    thread-safe by setting the :proc:`~bumpPtrMemPool.parSafe` parameter to
    ``true``. By default, all memory allocated by :type:`bumpPtrMemPool` is aligned
    to 16 bytes. This can be changed by setting the
    :param:`~bumpPtrMemPool.alignment` parameter. An alignment of 0 will disable
    alignment.

    The :type:`bumpPtrMemPool` is initialized with a fixed size and allocates
    memory from that fixed size. Attempting to allocate more memory than the
    fixed size will result in an error. With checks enabled the program will
    halt, otherwise memory errors will occur.

    All memory allocated by :type:`bumpPtrMemPool` is deallocated when the
    :type:`bumpPtrMemPool` instance is destroyed. The
    :proc:`~bumpPtrMemPool.deallocate` method is a no-op.
  */
  class bumpPtrMemPool: allocator {
    /*
      Whether or not this allocator is thread-safe. if ``true``, this allocator
      will use a lock to ensure that only one thread can allocate memory at a
      time.
    */
    param parSafe: bool = false;
    /*
      The alignment of memory allocated by this allocator. If ``alignment`` is
      greater than 0, the pointers returned by :proc:`~bumpPtrMemPool.allocate`
      will be aligned to that alignment. ``alignment`` must be a power of 2.
    */
    param alignment: int = 16;
    /*
      The size of the memory block allocated by this allocator.
    */
    var size: int(64);
    @chpldoc.nodoc
    var basePtr: c_ptr(int(8));
    @chpldoc.nodoc
    var ptr: c_ptr(int(8));
    @chpldoc.nodoc
    var lock_ = if parSafe then new _LockWrapper() else none;


    /*
      Initializes the allocator with a fixed size. The allocator will allocate
      memory from this fixed size.

      Memory is allocated eagerly, so the entire size is allocated when the
      allocator is created.

      :arg size: The size of the memory block allocated by this allocator.
      :param parSafe: Whether or not this allocator is thread-safe.
      :param alignment: The alignment of memory allocated by this allocator.
    */
    proc init(size: int(64), param parSafe: bool = false, param alignment: int = 16) {
      this.parSafe = parSafe;

      this.alignment = alignment;
      if this.alignment < 0 then
        compilerError("alignment must be non-negative");
      if (this.alignment & (this.alignment - 1)) != 0 then
        compilerError("alignment must be a power of 2");

      this.size = size;
      if boundsChecking {
        if this.size <= 0 {
          halt("size must be positive");
        }
      }

      basePtr = CTypes.allocate(int(8), this.size.safeCast(c_size_t));
      ptr = basePtr;
      init this;
    }

    /*
      Deallocates all memory owned by this allocator. Attempting to use an
      object allocated by this allocator after calling this method will result
      in a use-after-free error.
    */
    proc deinit() {
      if basePtr then
        CTypes.deallocate(basePtr);
    }

    // TODO: not using `do if ...` due to chpldoc bug: https://github.com/chapel-lang/chapel/issues/25790
    @chpldoc.nodoc
    proc _lock() { if parSafe then lock_.lock(); }
    @chpldoc.nodoc
    inline proc _unlock() { if parSafe then lock_.unlock(); }

    /*
      Allocate memory for ``n`` bytes. If :param:`~bumpPtrMemPool.alignment` is
      greater than 0, the memory will be aligned to that alignment.

      If not enough memory is available and checks are enabled, this method
      will halt the program.

      :arg n: The number of bytes to allocate.
      :returns: A ``c_ptr(void)`` to the allocated memory.
    */
    proc allocate(n: int): c_ptr(void) {
      _lock();
      if boundsChecking {
        if n <= 0 {
          halt("n must be positive");
        }
      }

      if alignment > 0 then
        ptr = alignup(ptr, this.alignment): c_ptr(int(8));

      if boundsChecking {
        if (ptr + n):c_intptr > (basePtr + size):c_intptr {
          halt("out of memory");
        }
      }

      var p = ptr;
      ptr += n;

      _unlock();
      return p;
    }

    /*
      This is a stub method that does nothing. All memory owned by this
      allocator is deallocated when the allocator is destroyed.

      :arg p: The pointer to the allocated memory.
    */
    proc deallocate(p: c_ptr(void)) {
      // no-op
    }
  }
}
