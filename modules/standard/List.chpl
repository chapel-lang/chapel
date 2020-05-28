/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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
  This module contains the implementation of the list type.

  A list is a lightweight container similar to an array that is suitable for
  building up and iterating over a collection of elements in a structured
  manner.

  The highly parallel nature of Chapel means that great care should be taken
  when performing operations that may invalidate references to list elements.
  Inserts and removals into the middle of a list are example operations that
  may invalidate references. Appending an element to the end of a list will
  never invalidate references to elements contained in the list.

  The following operations may invalidate references to elements contained in
  a list:

      - insert
      - remove
      - sort
      - pop
      - clear

  Additionally, all references to list elements are invalidated when the list
  is deinitialized.

  Lists are not parallel safe by default, but can be made parallel safe by
  setting the param formal `parSafe` to true in any list constructor. When
  constructed from another list, the new list will inherit the parallel safety
  mode of its originating list.

  Inserts and removals into a list are O(n) worst case and should be performed
  with care. Appends into a list have an amortized speed of O(1). Indexing
  into a list is O(1).
*/
module List {
  import ChapelLocks;
  private use HaltWrappers;
  private use Sort;

  pragma "no doc"
  private const _initialCapacity = 8;

  pragma "no doc"
  private const _initialArrayCapacity = 16;

  pragma "no doc"
  private param _sanityChecks = false;

  //
  // Some asserts are useful while developing, but can be turned off when the
  // implementation is correct.
  //
  private inline proc _sanity(expr: bool) {
    if _sanityChecks then
      assert(expr);
  }

  //
  // We can change the lock type later. Use a spinlock for now, even if it
  // is suboptimal in cases where long critical sections have high
  // contention (IE, lots of tasks trying to insert into the middle of this
  // list, or any operation that is O(n)).
  //
  pragma "no doc"
  type _lockType = ChapelLocks.chpl_LocalSpinlock;

  //
  // Use a wrapper class to let list methods have a const ref receiver even
  // when `parSafe` is `true` and the list lock is used.
  //
  pragma "no doc"
  class _LockWrapper {
    var lock$ = new _lockType();

    inline proc lock() {
      lock$.lock();
    }

    inline proc unlock() {
      lock$.unlock();
    }
  }

  /* Check that element type is supported by list */
  proc _checkType(type eltType) {
    if isGenericType(eltType) {
      compilerWarning("creating a list with element type " +
                      eltType:string);
      if isClassType(eltType) && !isGenericType(borrowed eltType) {
        compilerWarning("which now means class type with generic management");
      }
      compilerError("list element type cannot currently be generic");
      // In the future we might support it if the list is not default-inited
    }
  }

  pragma "no doc"
  proc _dummyFieldType(type t) type {
    if isBorrowedClass(t) {
      return t?;
    } else {
      return nothing;
    }
  }

  private use IO;

  /*
    A list is a lightweight container suitable for building up and iterating
    over a collection of elements in a structured manner. Unlike a stack, the
    list type also supports inserts or removals into the middle of the list.
    The list type is close in spirit to its Python counterpart, with fast O(1)
    random access and append operations.

    The list type is not parallel safe by default. For situations in which
    such protections are desirable, parallel safety can be enabled by setting
    `parSafe = true` in any list constructor.

    Unlike an array, the set of indices of a list is always `0..<size`.
  */
  record list {

    /* The type of the elements contained in this list. */
    type eltType;

    /* If `true`, this list will perform parallel safe operations. */
    param parSafe = false;

    pragma "no doc"
    var _size = 0;

    pragma "no doc"
    var _lock$ = if parSafe then new _LockWrapper() else none;

    pragma "no doc"
    var _arrays: _ddata(_ddata(eltType)) = nil;

    pragma "no doc"
    var _arrayCapacity = 0;

    pragma "no doc"
    var _totalCapacity = 0;

    //
    // If the list element type is a borrowed class, instantiate this dummy
    // field as a `borrowed c?` and let it default initialize to `nil`.
    // Otherwise, instantiate it as a `nothing` and let the compiler fold
    // it away. See #15575.
    //
    pragma "no doc"
    var _dummyFieldToForceBorrowChecking: _dummyFieldType(eltType);

    /*
      Initializes an empty list.

      :arg eltType: The type of the elements of this list.

      :arg parSafe: If `true`, this list will use parallel safe operations.
      :type parSafe: `param bool`
    */
    proc init(type eltType, param parSafe=false) {
      _checkType(eltType);
      this.eltType = eltType;
      this.parSafe = parSafe;
      this.complete();
      this._firstTimeInitializeArrays();
    }

    /*
      Initializes a list containing elements that are copy initialized from
      the elements contained in another list.

      Used in new expressions.

      :arg other: The list to initialize from.

      :arg parSafe: If `true`, this list will use parallel safe operations.
      :type parSafe: `param bool`
    */
    proc init(other: list(?t), param parSafe=false) {
      if !isCopyableType(this.type.eltType) then
        compilerError("Cannot copy list with element type that " +
                      "cannot be copied");
      this.eltType = t;
      this.parSafe = parSafe;
      this.complete();
      _commonInitFromIterable(other);
    }

    /*
      Initializes a list containing elements that are copy initialized from
      the elements contained in an array.

      Used in new expressions.

      :arg other: The array to initialize from.

      :arg parSafe: If `true`, this list will use parallel safe operations.
      :type parSafe: `param bool`
    */
    proc init(other: [?d] ?t, param parSafe=false) {
      _checkType(t);
      if !isCopyableType(t) then
        compilerError("Cannot construct list from array with element " +
                      "type that cannot be copied");

      this.eltType = t;
      this.parSafe = parSafe;
      this.complete();
      _commonInitFromIterable(other);
    }

    /*
      Initializes a list containing elements that are copy initialized from
      the elements yielded by a range.

      Used in new expressions.

      .. note::

        Attempting to initialize a list from an unbounded range will trigger
        a compiler error.

      :arg other: The range to initialize from.

      :arg parSafe: If `true`, this list will use parallel safe operations.
      :type parSafe: `param bool`
    */
    proc init(other: range(?t), param parSafe=false) {
      _checkType(t);
      this.eltType = t;
      this.parSafe = parSafe;

      if !isBoundedRange(other) {
        param e = this.type:string;
        param f = other.type:string;
        param msg = "Cannot init " + e + " from unbounded " + f;
        compilerError(msg);
      }

      this.complete();
      _commonInitFromIterable(other);
    }

    /*
      Initializes a list containing elements that are copy initialized from
      the elements contained in another list.

      :arg other: The list to initialize from.
    */
    proc init=(other: list(this.type.eltType, ?p)) {
      if !isCopyableType(this.type.eltType) then
        compilerError("Cannot copy list with element type that " +
                      "cannot be copied");

      this.eltType = this.type.eltType;
      this.parSafe = this.type.parSafe;
      this.complete();
      _commonInitFromIterable(other);
    }

    /*
      Initializes a list containing elements that are copy initialized from
      the elements contained in an array.

      :arg other: The array to initialize from.
    */
    proc init=(other: [?d] this.type.eltType) {
      if !isCopyableType(this.type.eltType) then
        compilerError("Cannot copy list from array with element type " +
                      "that cannot be copied");

      this.eltType = this.type.eltType;
      this.parSafe = this.type.parSafe;
      this.complete();
      _commonInitFromIterable(other);
    }

    /*
      Initializes a list containing elements that are copy initialized from
      the elements yielded by a range.

      .. note::

        Attempting to initialize a list from an unbounded range will trigger
        a compiler error.

      :arg other: The range to initialize from.
      :type other: `range(this.type.eltType)`
    */
    proc init=(other: range(this.type.eltType, ?b, ?d)) {
      this.eltType = this.type.eltType;
      this.parSafe = this.type.parSafe;

      if !isBoundedRange(other) {
        param e = this.type:string;
        param f = other.type:string;
        param msg = "Cannot init " + e + " from unbounded " + f;
        compilerError(msg);
      }

      this.complete();
      _commonInitFromIterable(other);
    }

    pragma "no doc"
    proc _commonInitFromIterable(iterable) {
      this._firstTimeInitializeArrays();

      for x in iterable do {
        pragma "no auto destroy"
        var cpy = x;
        _appendByRef(cpy);
      }
    }

    pragma "no doc"
    proc _firstTimeInitializeArrays() {
      _sanity(_arrays == nil);
      _sanity(_totalCapacity == 0);
      _sanity(_size == 0);
      _arrays = _makeBlockArray(_initialArrayCapacity);
      _arrayCapacity = _initialArrayCapacity;
      _arrays[0] = _makeArray(_initialCapacity);
      _totalCapacity = _initialCapacity;
    }

    pragma "no doc"
    inline proc deinit() {
      _fireAllDestructors();
      _freeAllArrays();
      _sanity(_totalCapacity == 0);
      _sanity(_size == 0);
      _sanity(_arrays == nil);
    }

    pragma "no doc"
    inline proc _destroy(ref item: eltType) {
      chpl__autoDestroy(item);
    }

    pragma "no doc"
    pragma "unsafe"
    inline proc _move(ref src: ?t, ref dst: t) lifetime src == dst {
      __primitive("=", dst, src);
    }

    pragma "no doc"
    inline proc _getArrayCapacity(array: int): int {
      const exp = array + log2(_initialCapacity);
      const result = 2 ** exp;
      return result;
    }

    pragma "no doc"
    inline proc _getArrayIdx(zpos: int): int {
      const adj = zpos + _initialCapacity;
      const result = log2(adj) - log2(_initialCapacity);
      return result;
    }

    pragma "no doc"
    inline proc _getLastArrayIdx(): int {
      const result = _getArrayIdx(_size - 1);
      _sanity(result >= 0);
      return result;
    }

    pragma "no doc"
    inline proc _getItemIdx(zpos: int): int {
      const adj = zpos + _initialCapacity;
      const result = adj ^ (1 << log2(adj));
      return result;
    }

    //
    // A helper function for getting a reference to a list element.
    // May be less important now that lists use 0-based indexing(?).
    //
    pragma "no doc"
    inline proc const ref _getRef(idx: int) ref {
      _sanity(idx >= 0 && idx < _totalCapacity);
      const arrayIdx = _getArrayIdx(idx);
      const itemIdx = _getItemIdx(idx);
      const array = _arrays[arrayIdx];
      _sanity(array != nil);
      ref result = array[itemIdx];
      return result;
    }

    pragma "no doc"
    inline proc _enter() {
      if parSafe then
        _lock$.lock();
    }

    pragma "no doc"
    inline proc _leave() {
      if parSafe then
        _lock$.unlock();
    }

    pragma "no doc"
    inline proc const _withinBounds(idx: int): bool {
      return (idx >= 0 && idx < _size);
    }

    //
    // This call assumes that a lock (acquired by `_enter`) is already
    // held (if parSafe==true), and releases it before throwing an error if
    // a bounds check fails.
    //
    pragma "no doc"
    inline proc const _boundsCheckLeaveOnThrow(i: int, umsg: string="") throws {
      if !_withinBounds(i) {
        _leave();
        const msg = if umsg != "" then umsg else
          "Index out of bounds: " + i:string;
        throw new owned
          IllegalArgumentError(msg);
      }
    }

    pragma "no doc"
    proc _makeBlockArray(size: int) {
      return _ddata_allocate(_ddata(eltType), size);
    }

    pragma "no doc"
    proc _freeBlockArray(data: _ddata(_ddata(eltType)), size: int) {
      _ddata_free(data, size);
    }

    pragma "no doc"
    proc _makeArray(size: int) {
      return _ddata_allocate(eltType, size, initElts=false);
    }

    pragma "no doc"
    proc _freeArray(data: _ddata(eltType), size: int) {
      _ddata_free(data, size);
    }

    pragma "no doc"
    proc _maybeAcquireMem(amount: int) {

      const remaining = _totalCapacity - _size;
      _sanity(remaining >= 0);

      if remaining >= amount then
        return;

      var lastArrayIdx = if _size == 0 then 0 else _getLastArrayIdx();
      var req = amount - remaining;

      while req > 0 {

        //
        // Double the block array if we've run out of space.
        //
        if lastArrayIdx >= (_arrayCapacity - 1) then
          on this {
            var _narrays = _makeBlockArray(_arrayCapacity * 2);

            for i in 0..#_arrayCapacity do
              _narrays[i] = _arrays[i];

            _freeBlockArray(_arrays, _arrayCapacity);
            _arrays = _narrays;
            _arrayCapacity *= 2;
          }

        //
        // Add a new block to the block array that is twice the size of the
        // previous block.
        //
        on this {
          const oldLast = _arrays[lastArrayIdx];
          const oldLastCapacity = _getArrayCapacity(lastArrayIdx);
          lastArrayIdx += 1;

          ref newLast = _arrays[lastArrayIdx];
          const newLastCapacity = oldLastCapacity * 2;

          _sanity(oldLast != nil);
          _sanity(newLast == nil);

          newLast = _makeArray(newLastCapacity);

          _totalCapacity += newLastCapacity;
          req -= newLastCapacity;
        }
      }
      return;
    }

    //
    // If the current size (occupied slots) minus a given amount is small
    // enough such that no slots in the last "sub-block" are occupied, then
    // preemptively free that block.
    //
    // This method _does not_ fire destructors!
    //
    pragma "no doc"
    proc _maybeReleaseMem(amount: int) {

      //
      // If we're down to one single "sub array", then there's no sense in
      // doing anymore work (upon repeated appends/pops, we'd just have to
      // reinitialize/deinitialize the whole segment array repeatedly).
      //
      if _totalCapacity <= _initialCapacity then
        return;

      const lastArrayIdx = _getLastArrayIdx();
      _sanity(lastArrayIdx != 0);

      const lastArrayCapacity = _getArrayCapacity(lastArrayIdx);
      const threshold = _totalCapacity - lastArrayCapacity;
      const nsize = _size - amount;

      if nsize > threshold then
        return;

      ref array = _arrays[lastArrayIdx];
      _sanity(array != nil);

      _freeArray(array, lastArrayCapacity);
      _totalCapacity -= lastArrayCapacity;
      array = nil;
    }

    //
    // Shift elements including and after index `idx` so that they are moved
    // `shift` positions to the right in memory, possibly resizing. May
    // expand memory if necessary.
    //
    pragma "no doc"
    proc ref _expand(idx: int, shift: int=1) {
      _sanity(_withinBounds(idx));

      if shift <= 0 then
        return;

      on this {
        _maybeAcquireMem(shift);

        for i in idx.._size-1 by -1 {
          ref src = _getRef(i);
          ref dst = _getRef(i + shift);
          _move(src, dst);
        }
      }
      return;
    }

    //
    // Move all elements at and following the index `shift` left in memory
    // so that they begin at index `idx`, possibly resizing. May release
    // memory if possible.
    //
    // This method does not fire destructors, so do so before calling it.
    //
    pragma "no doc"
    proc ref _collapse(idx: int, shift: int=1) {
      _sanity(_withinBounds(idx));

      if idx == _size-1 then
        return;
      
      on this {
        for i in idx..(_size - 2) {
          ref src = _getRef(i + 1);
          ref dst = _getRef(i);
          _move(src, dst);
        }

        _maybeReleaseMem(1);
      }
      return;
    }

    //
    // Assumes that a copy of the input element has already been made at some
    // previous boundary, IE giving a parameter the "in" intent. Whatever 
    // copy you've made, make sure that the "no auto destroy" pragma is
    // attached so that you avoid firing a destructor early (and in the worst
    // case, fire it twice).
    //
    pragma "no doc"
    proc ref _appendByRef(ref x: eltType) {
      _maybeAcquireMem(1);
      ref src = x;
      ref dst = _getRef(_size);
      _move(src, dst);
      _size += 1;
    }

    /*
      Add an element to the end of this list.

      :arg x: An element to append.
      :type x: `eltType`
    */
    proc ref append(pragma "no auto destroy" in x: this.eltType)
    lifetime this < x {
      _enter();

      //
      // TODO: Can't use on statement here without getting a memory leak on
      // gasnet/multilocale configurations.
      //
      _appendByRef(x);
      _leave();
    }

    /*
      Returns `true` if this list contains an element equal to the value of
      `x`, and `false` otherwise.

      :arg x: An element to search for.
      :type x: `eltType`

      :return: `true` if this list contains `x`.
      :rtype: `bool`
    */
    proc const contains(x: eltType): bool {
      var result = false;

      on this {
        _enter();

        for item in this do
          if item == x {
            result = true;
            break;
          }

        _leave();
      }

      return result;
    }

    /*
      Returns a reference to the first item in this list.

      .. warning::

        Calling this method on an empty list will cause the currently running
        program to halt. If the `--fast` flag is used, no safety checks will
        be performed.

      :return: A reference to the first item in this list.
      :rtype: `ref eltType`
    */
    proc ref first() ref {
      _enter();

      if boundsChecking && _size == 0 {
        _leave();
        boundsCheckHalt("Called \"list.first\" on an empty list.");
      }

      // TODO: How to make this work with on clauses?
      ref result = _getRef(0);
      _leave();

      return result;
    }

    /*
      Returns a reference to the last item in this list.

      .. warning::

        Calling this method on an empty list will cause the currently running
        program to halt. If the `--fast` flag is used, no safety checks will
        be performed.

      :return: A reference to the last item in this list.
      :rtype: `ref eltType`
    */
    proc ref last() ref {
      _enter();

      if boundsChecking && _size == 0 {
        _leave();
        boundsCheckHalt("Called \"list.last\" on an empty list.");
      }
     
      // TODO: How to make this work with on clauses?
      ref result = _getRef(_size-1);
      _leave();

      return result;  
    }

    pragma "no doc"
    inline proc ref _extendGeneric(collection) {

      //
      // TODO: This could avoid repeated resizes at smaller total capacities
      // if we resized once and then performed repeated moves, rather than
      // calling _append().
      //
      on this {
        for item in collection {
          pragma "no auto destroy"
          var cpy = item;
          _appendByRef(cpy);
        }
      }
    }

    /*
      Extend this list by appending a copy of each element contained in
      another list.

      :arg other: A list containing elements of the same type as those
        contained in this list.
      :type other: `list(eltType)`
    */
    proc ref extend(other: list(eltType, ?p)) lifetime this < other {
      on this {
        _enter();
        _extendGeneric(other);
        _leave();
      }
    }

    /*
      Extend this list by appending a copy of each element contained in an
      array.

      :arg other: An array containing elements of the same type as those
        contained in this list.
      :type other: `[?d] eltType`
    */
    proc ref extend(other: [?d] eltType) lifetime this < other {
      on this {
        _enter();
        _extendGeneric(other);
        _leave();
      }
    }

    /*
      Extends this list by appending a copy of each element yielded by a
      range.

      .. note::

        Attempting to initialize a list from an unbounded range will trigger
        a compiler error.

      :arg other: The range to initialize from.
      :type other: `range(eltType)`
    */
    proc ref extend(other: range(eltType, ?b, ?d)) lifetime this < other {
      if !isBoundedRange(other) {
        param e = this.type:string;
        param f = other.type:string;
        param msg = "Cannot extend " + e + " with unbounded " + f;
        compilerError(msg);
      }

      on this {
        _enter();
        _extendGeneric(other);
        _leave();
      }
    }

    /*
      Insert an element at a given position in this list, shifting all elements
      currently at and following that index one to the right. The call
      ``a.insert(0, x)`` inserts an element at the front of the list `a`, and
      ``a.insert((a.size), x)`` is equivalent to ``a.append(x)``.

      If the insertion is successful, this method returns `true`. If the given
      index is out of bounds, this method does nothing and returns `false`.

      .. warning::
      
        Inserting an element into this list may invalidate existing references
        to the elements contained in this list.

      :arg idx: The index into this list at which to insert.
      :type idx: `int`

      :arg x: The element to insert.
      :type x: `eltType`

      :return: `true` if `x` was inserted, `false` otherwise.
      :rtype: `bool`
    */
    proc ref insert(idx: int, pragma "no auto destroy" in x: eltType): bool
         lifetime this < x {
      var result = false;

      on this {
        _enter();

      // Handle special case of `a.insert((a.size), x)` here.
      if idx == _size {
        _appendByRef(x);
        result = true;
      } else if _withinBounds(idx) {
        _expand(idx);
        ref src = x;
        ref dst = _getRef(idx);
        _move(src, dst);
        _size += 1;
        result = true;
      }

        _leave();
      }

      // Destroy our copy if it was never used.
      if !result then
        _destroy(x);

      return result;  
    }

    pragma "no doc"
    proc ref _insertGenericKnownSize(idx: int, items, size: int): bool {
      var result = false;

      _sanity(size >= 0);

      if size == 0 then
        return true;

      on this {
        if idx == _size {
          // TODO: In an ideal world, we'd resize only once.
          _extendGeneric(items);
          result = true;
        } else if _withinBounds(idx) {
          _expand(idx, size);

          var i = idx;
          for x in items {
            pragma "no auto destroy"
            var cpy = x;
            ref src = cpy;
            ref dst = _getRef(i);
            _move(src, dst);
            _size += 1;
            i += 1;
          }

          result = true;
        }
      }

      return result;
    }

    /*
      Insert an array of elements `arr` into this list at index `idx`,
      shifting all elements at and following the index `arr.size` positions
      to the right. 

      If the insertion is successful, this method returns `true`. If the given
      index is out of bounds, this method does nothing and returns `false`.

      .. warning::

        Inserting elements into this list may invalidate existing references
        to the elements contained in this list.

      :arg idx: The index into this list at which to insert.
      :type idx: `int`

      :arg arr: An array of elements to insert.
      :type x: `[] eltType`

      :return: `true` if `arr` was inserted, `false` otherwise.
      :rtype: `bool`
    */
    proc ref insert(idx: int, arr: [?d] eltType): bool lifetime this < arr {

      var result = false;

      on this {
        _enter();
        result = _insertGenericKnownSize(idx, arr, arr.size);
        _leave();
      }

      return result;
    }

    /*
      Insert a list of elements `lst` into this list at index `idx`, shifting
      all elements at and following the index `lst.size` positions to the
      right.

      If the insertion is successful, this method returns `true`. If the given
      index is out of bounds, this method does nothing and returns `false`.

      .. warning::

        Inserting elements into this list may invalidate existing references
        to the elements contained in this list.

      :arg idx: The index into this list at which to insert.
      :type idx: `int`

      :arg lst: A list of elements to insert.
      :type lst: `list(eltType)`

      :return: `true` if `lst` was inserted, `false` otherwise.
      :rtype: `bool`
    */
    proc ref insert(idx: int, lst: list(eltType)): bool lifetime this < lst {
      
      var result = false;
      
      // Prevent deadlock if we are trying to insert this into itself.
      const size = lst.size;

      on this {
        _enter();
        result = _insertGenericKnownSize(idx, lst, size);
        _leave();
      }

      return result;
    }

    /*
      Remove the first `count` elements from this list with values equal to
      `x`, shifting all elements following the removed item left.

      If the count of elements to remove is less than or equal to zero, then
      all elements from this list equal to the value of `x` will be removed.

      .. warning::

        Removing elements from this list may invalidate existing references
        to the elements contained in this list.

      :arg x: The value of the element to remove.
      :type x: `eltType`

      :arg count: The number of elements to remove.
      :type count: `int`

      :return: The number of elements removed.
      :rtype: `int`
    */
    proc ref remove(x: eltType, count:int=1): int {
      var result = 0;

      on this {
        var src_i, dst_i: int;
        var removed = 0;
        src_i = 0;

        _enter();

        // Run through the leading non-x prefix
        while dst_i < _size {
          ref dst = _getRef(dst_i);
          if dst == x {
            _destroy(dst);
            removed += 1;
            break;
          }
          dst_i += 1;
        }

        // Once we've found an x, everything after has to be _move()d.
        // Run src_i ahead, moving everything that isn't x back to dst_i.
        src_i = dst_i + 1;
        while src_i < _size {
          ref src = _getRef(src_i);
          if (count == 0 || removed < count) && src == x {
            _destroy(src);
            src_i += 1;
            removed += 1;
            continue;
          }
          ref dst = _getRef(dst_i);
          _move(src, dst);
          src_i += 1;
          dst_i += 1;
        }

        if (removed) {
          _maybeReleaseMem(removed);
          _size = _size - removed;
        }

        _leave();

        result = removed;
      }

      return result;
    }

    //
    // Not sure if strictly necessary, since we're probably only going to
    // call this from `pop`, but I added `unlockBeforeHalt` all the same.
    //
    pragma "no doc"
    proc ref _popAtIndex(idx: int, unlockBeforeHalt=true): eltType {

      //
      // TODO: We would like to put this in an on statement, but we can't yet
      // because there is no way to "default initialize a non-nilable class",
      // even if the variable is pragma "no init". Either we need to support
      // returning from on statements, or make the "no init" pragma work with
      // non-nilable classes.
      //

      if boundsChecking && _size <= 0 {
        if unlockBeforeHalt then
          _leave();
        boundsCheckHalt("Called \"list.pop\" on an empty list.");
      }

      if boundsChecking && !_withinBounds(idx) {
        if unlockBeforeHalt then
          _leave();
        const msg = "Index for \"list.pop\" out of bounds: " + idx:string;
        boundsCheckHalt(msg);
      }

      ref item = _getRef(idx);

      pragma "no init"
      var result:eltType;
      _move(item, result);

      // May release memory based on size before pop.
      _collapse(idx);
      _size -= 1;

      return result;
    }

    /*
      Remove the element at the end of this list and return it.

      .. warning::

        Popping an element from this list will invalidate any reference to
        the element taken while it was contained in this list.

      .. warning::

        Calling this method on an empty list will cause the currently running
        program to halt. If the `--fast` flag is used, no safety checks will
        be performed.

      :return: The element popped.
      :rtype: `eltType`
    */
    proc ref pop(): eltType {
      _enter();
      var result = _popAtIndex(_size-1);
      _leave();
      return result;
    }

    /*
      Remove the element at the index `idx` from this list and return it. The
      elements at indices after `idx` are shifted one to the left in memory,
      making this operation O(n).

      .. warning::

        Popping an element from this list will invalidate any reference to
        the element taken while it was contained in this list.

      .. warning::

        Calling this method on an empty list or with values of `idx` that
        are out of bounds will cause the currently running program to halt.
        If the `--fast` flag is used, no safety checks will be performed.

      :arg idx: The index of the element to remove.
      :type idx: `int`

      :return: The element popped.
      :rtype: `eltType`
    */
    proc ref pop(idx: int): eltType {
      _enter();
      var result = _popAtIndex(idx);
      _leave();
      return result;
    }

    //
    // Manually call destructors on each currently allocated element. For
    // logical consistency, set size to zero once all destructors have been
    // fired.
    //
    pragma "no doc"
    proc _fireAllDestructors() {
      on this {
        for i in 0..#_size {
          ref item = _getRef(i);
          _destroy(item);
        }
        _size = 0;
      }
      return;
    }

    pragma "no doc"
    proc _freeAllArrays() {

      if _arrays == nil then
        return;

      _sanity(_totalCapacity != 0);
      _sanity(_arrayCapacity != 0);
    
      on this {
        // Remember to use zero-based indexing with `_ddata`!
        for i in 0..#_arrayCapacity {
          ref array = _arrays[i];
          if array == nil then
            continue;
          const capacity = _getArrayCapacity(i);
          _totalCapacity -= capacity;
          _freeArray(array, capacity);
          array = nil;
        }

        _sanity(_totalCapacity == 0);

        _freeBlockArray(_arrays, _arrayCapacity);
        _arrays = nil;
        _size = 0;
      }
      return;
    }

    /*
      Clear the contents of this list.

      .. warning::

        Clearing the contents of this list will invalidate all existing
        references to the elements contained in this list.
    */
    proc ref clear() {
      on this {
        _enter();

        _fireAllDestructors();
        _freeAllArrays();
        _sanity(_totalCapacity == 0);
        _sanity(_size == 0);
        _sanity(_arrays == nil);

        // All array operations assume a consistent initial state.
        _firstTimeInitializeArrays();

        _leave();
      }
    }

    /*
      Return a zero-based index into this list of the first item whose value
      is equal to `x`. If no such element can be found this method returns
      the value `-1`.

      .. warning::

        Calling this method on an empty list or with values of `start` or 
        `end` that are out of bounds will cause the currently running program
        to halt. If the `--fast` flag is used, no safety checks will be
        performed.

      :arg x: An element to search for.
      :type x: `eltType`

      :arg start: The start index to start searching from.
      :type start: `int`

      :arg end: The end index to stop searching at. A value less than
                `0` will search the entire list.
      :type end: `int`

      :return: The index of the element to search for, or `-1` on error.
      :rtype: `int`
    */
    proc const indexOf(x: eltType, start: int=0, end: int=-1): int {
      if boundsChecking {
        const msg = " index for \"list.indexOf\" out of bounds: ";

        if end >= 0 && !_withinBounds(end) then
          boundsCheckHalt("End" + msg + end:string);

        if !_withinBounds(start) then
          boundsCheckHalt("Start" + msg + start:string);
      }

      param error = -1;

      if end >= 0 && end < start then
        return error;

      var result = error;

      on this {
        _enter();

        const stop = if end < 0 then _size-1 else end;

        for i in start..stop do
          if x == _getRef(i) {
            result = i;
            break;
          }

        _leave();
      }

      return result;
    }

    /*
      Return the number of times a given element is found in this list.

      :arg x: An element to count.
      :type x: `eltType`

      :return: The number of times a given element is found in this list.
      :rtype: `int`
    */
    proc const count(x: eltType): int {
      var result = 0;

      on this {
        _enter();

        var count = 0;

        for item in this do
          if x == item then
            count += 1;

        result = count;

        _leave();
      }

      return result;
    }

    /*
      Sort the items of this list in place using a comparator. If no comparator
      is provided, sort this list using the default sort order of its elements.

      .. warning::

        Sorting the elements of this list may invalidate existing references
        to the elements contained in this list.

      :arg comparator: A comparator used to sort this list.
    */
    proc ref sort(comparator: ?rec=Sort.defaultComparator) {
      on this {
        _enter();

        //
        // TODO: This is not ideal, but the Sort API needs to be adjusted
        // before we can sort over lists directly.
        //
        if _size > 1 {

          // Copy current list contents into an array.
          var arr: [0..#_size] eltType;
          for i in 0..#_size do
            arr[i] = this[i];

          Sort.sort(arr, comparator);

          // This is equivalent to the clear routine.
          _fireAllDestructors();
          _freeAllArrays();
          _firstTimeInitializeArrays();
          _extendGeneric(arr);
        }
        
        _leave();
      }
      return;
    }

    /*
      Index this list via subscript. Returns a reference to the element at a
      given index in this list.

      :arg i: The index of the element to access.

      .. warning::

        Use of the `this` method with an out of bounds index (while bounds
        checking is on) will cause the currently running program to halt.

      :return: An element from this list.
    */
    proc ref this(i: int) ref {
      if boundsChecking && !_withinBounds(i) {
        const msg = "Invalid list index: " + i:string;
        boundsCheckHalt(msg);
      }

      ref result = _getRef(i);
      return result;
    }
    proc const ref this(i: int) const ref {
      if boundsChecking && !_withinBounds(i) {
        const msg = "Invalid list index: " + i:string;
        halt(msg);
      }

      const ref result = _getRef(i);
      return result;
    }

    // TODO - make const ref return intent overloads for `these`
    // and make the ref-return overload accept this by `ref`
    // once #12944 is fixed.

    /*
      Iterate over the elements of this list.

      :yields: A reference to one of the elements contained in this list.
    */
    iter these() ref {
      // TODO: We can just iterate through the _ddata directly here.
      for i in 0..#_size {
        ref result = _getRef(i);
        yield result;
      }
    }

    pragma "no doc"
    iter these(param tag: iterKind) ref where tag == iterKind.standalone {
      const osz = _size;
      const minChunkSize = 64;
      const hasOneChunk = osz <= minChunkSize;
      const numTasks = if hasOneChunk then 1 else here.maxTaskPar;
      const chunkSize = floor(osz / numTasks):int;
      const trailing = osz - chunkSize * numTasks;

      coforall tid in 0..#numTasks {
        var chunk = _computeChunk(tid, chunkSize, trailing);
        for i in chunk(0) do
          yield this[i];
      }
    }

    pragma "no doc"
    proc _computeChunk(tid, chunkSize, trailing) {
      var lo, hi = 0;

      if tid <= 0 {
        lo = 0;
        hi = chunkSize + trailing - 1;
      } else {
        lo = chunkSize * tid + trailing;
        hi = lo + chunkSize - 1;
      }

      return (lo..hi,);
    }

    pragma "no doc"
    iter these(param tag) ref where tag == iterKind.leader {
      const osz = _size;
      const minChunkSize = 32;
      const hasOneChunk = osz <= minChunkSize;
      const numTasks = if hasOneChunk then 1 else dataParTasksPerLocale;
      const chunkSize = floor(osz / numTasks):int;
      const trailing = osz - chunkSize * numTasks;

      // TODO: We can just use the range iterator like above.
      coforall tid in 0..#numTasks {
        var chunk = _computeChunk(tid, chunkSize, trailing);
        yield chunk;
      }
    }

    pragma "no doc"
    iter these(param tag, followThis) ref where tag == iterKind.follower {

      //
      // TODO: A faster scheme would access the _ddata directly to avoid
      // the penalty of logarithmic indexing over and over again.
      //
      for i in followThis(0) do
        yield this[i];
    }

    /*
      Write the contents of this list to a channel.

      :arg ch: A channel to write to.
    */
    proc readWriteThis(ch: channel) throws {
      _enter();
      
      ch <~> "[";

      for i in 0..(_size - 2) do
        ch <~> _getRef(i) <~> ", ";

      if _size > 0 then
        ch <~> _getRef(_size-1);

      ch <~> "]";

      _leave();
    }

    /*
      Returns `true` if this list contains zero elements.

      :return: `true` if this list is empty.
      :rtype: `bool`
    */
    proc const isEmpty(): bool {

      //
      // TODO: We can make _size atomic to avoid having to worry about guard-
      // ing access to it like this. Haven't done yet because I was getting
      // some compiler errors related to atomics.
      //
      _enter();
      var result = (_size == 0);
      _leave();
      return result;
    }

    /*
      The current number of elements contained in this list.
    */
    inline proc const size {
      var result = 0;

      //
      // TODO: Ditto the above code comment.
      //
      on this {
        _enter();
        result = _size;
        _leave();
      }

      return result;
    }

    /*
      Returns the list's legal indices as the range ``0..<this.size``.

      :return: ``0..<this.size``
      :rtype: `range`
    */
    proc indices {
      return 0..<this.size;
    }

    /*
      Returns a new DefaultRectangular array containing a copy of each of the
      elements contained in this list.

      :return: A new DefaultRectangular array.
    */
    proc const toArray(): [] eltType {
      if isNonNilableClass(eltType) && isOwnedClass(eltType) then
        compilerError("toArray() method is not available on a 'list'",
                      " with elements of a non-nilable owned type, here: ",
                      eltType:string);

      // Once GitHub Issue #7704 is resolved, replace pragma "unsafe"
      // with a remote var declaration.
      pragma "unsafe" var result: [0..#_size] eltType;

      on this {
        _enter();

        var tmp: [0..#_size] eltType =
          forall i in 0..#_size do _getRef(i);

        result = tmp;

        _leave();
      }

      return result;
    }

  } // End record "list".

  /*
    Clear the contents of this list, then extend this now empty list with the
    elements contained in another list.

    .. warning::

      This will invalidate any references to elements previously contained in
      `lhs`.

    :arg lhs: The list to assign to.
    :arg rhs: The list to assign from. 
  */
  proc =(ref lhs: list(?t, ?), rhs: list(t, ?)) {
    lhs.clear();
    lhs.extend(rhs);
  }

  /*
    Returns `true` if the contents of two lists are the same.

    :arg a: A list to compare.
    :arg b: A list to compare.

    :return: `true` if the contents of two lists are equal.
    :rtype: `bool`
  */
  proc ==(a: list(?t, ?), b: list(t, ?)): bool {
    if a.size != b.size then
      return false;

    //
    // TODO: Make this a forall loop eventually.
    //
    for i in 0..#(a.size) do
      if a[i] != b[i] then
        return false;

    return true;
  }

  /*
    Return `true` if the contents of two lists are not the same.

    :arg a: A list to compare.
    :arg b: A list to compare.

    :return: `true` if the contents of two lists are not equal.
    :rtype: `bool`
  */
  proc !=(a: list(?t, ?), b: list(t, ?)): bool {
    return !(a == b);
  }

} // End module "Lists".

