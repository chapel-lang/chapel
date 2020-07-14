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
  This module contains the implementation of the vector type.

  Vector is a kind of list storing elements in a contiguous area.
*/
module Vector {

  import ChapelLocks;
  private use HaltWrappers;
  private use Sort;
  private use List only list;

  pragma "no doc"
  private const _initialCapacity = 8;
  //
  // We can change the lock type later. Use a spinlock for now, even if it
  // is suboptimal in cases where long critical sections have high
  // contention (IE, lots of tasks trying to insert into the middle of this
  // vector, or any operation that is O(n)).
  //
  pragma "no doc"
  type _lockType = ChapelLocks.chpl_LocalSpinlock;

  //
  // Use a wrapper class to let vector methods have a const ref receiver even
  // when `parSafe` is `true` and the vector lock is used.
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

  private use IO;

  proc _checkType(type t) {
    if (!isDefaultInitializable(t)) {
      compilerError("Vector does not support an eltType that can't be default initialized, ", 
                    "here: ", t:string);
    }
  }
  record vector {
    /* The type of the elements contained in this vector. */
    type eltType;
    /* If `true`, this vector will perform parallel safe operations. */
    param parSafe = false;


    /*
      The number of valid elements in this vector.
    */
    pragma "no doc"
    var _size = 0;

    /*
      The capacity of this vector, which represents how much space is allocated.
    */
    pragma "no doc"
    var _capacity = 0;

    pragma "no doc"
    var _domain = {0..#_initialCapacity};

    pragma "no doc"
    var _data: [_domain] eltType;

    pragma "no doc"
    var _lock$ = if parSafe then new _LockWrapper() else none;

    /*
      Initializes an empty list.

      :arg eltType: The type of the elements of this vector.

      :arg parSafe: If `true`, this vector will use parallel safe operations.
      :type parSafe: `param bool`
    */
    proc init(type eltType, param parSafe=false) {
      _checkType(eltType);
      this.eltType = eltType;
      this.parSafe = parSafe;
      this.complete();
    }

    /*
      Initializes a vector containing elements that are copied from
      the elements contained in a list.

      Used in new expressions.

      :arg other: The list to initialize from.

      :arg parSafe: If `true`, this vector will use parallel safe operations.
      :type parSafe: `param bool`
    */
    proc init(other: list(?t), param parSafe=false) {
      _checkType(this.type.eltType);
      if !isCopyableType(this.type.eltType) then
        compilerError("Cannot copy vector with element type that " +
                      "cannot be copied");
      this.eltType = t;
      this.parSafe = parSafe;
      this.complete();
      _requestCapacity(other.size);
      _commonInitFromIterable(other);
    }

    /*
      Initializes a vector containing elements that are copied from
      the elements contained in another list.

      Used in new expressions.

      :arg other: The vector to initialize from.

      :arg parSafe: If `true`, this vector will use parallel safe operations.
      :type parSafe: `param bool`
    */
    proc init(other: vector(?t), param parSafe=false) {
      _checkType(t);
      if !isCopyableType(this.type.eltType) then
        compilerError("Cannot copy vector with element type that " +
                      "cannot be copied");
      this.eltType = t;
      this.parSafe = parSafe;
      this.complete();
      _requestCapacity(other.size);
      _commonInitFromIterable(other);
    }

    /*
      Initializes a vector containing elements that are copied from
      the elements contained in another vector.

      :arg other: The vector to initialize from.
    */
    proc init=(other: vector(this.type.eltType)) {
      _checkType(this.type.eltType);
      if !isCopyableType(this.type.eltType) then
        compilerError("Cannot copy vector with element type that " +
                      "cannot be copied");
      this.eltType = this.type.eltType;
      this.parSafe = this.type.parSafe;
      this.complete();

      _requestCapacity(other.size);
      _commonInitFromIterable(other);
    }

    /*
      Initializes a vector containing elements that are copied from
      the elements contained in an array.

      Used in new expressions.

      :arg other: The array to initialize from.

      :arg parSafe: If `true`, this vector will use parallel safe operations.
      :type parSafe: `param bool`
    */
    proc init(other: [?d] ?t, param parSafe=false) {
      _checkType(t);
      if !isCopyableType(t) then
        compilerError("Cannot construct vector from array with element " +
                      "type that cannot be copied");

      this.eltType = t;
      this.parSafe = parSafe;
      this.complete();
      _requestCapacity(other.size);
      _commonInitFromIterable(other);
    }

    /*
      Initializes a vector containing elements that are copied from
      the elements yielded by a range.

      Used in new expressions.

      .. note::

        Attempting to initialize a vector from an unbounded range will trigger
        a compiler error.

      :arg other: The range to initialize from.

      :arg parSafe: If `true`, this vector will use parallel safe operations.
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
      _requestCapacity(other.size);
      _commonInitFromIterable(other);
    }

    /*
      Initializes a vector containing elements that are copied from
      the elements contained in the list.

      :arg other: The list to initialize from.
    */
    proc init=(other: list(this.type.eltType)) {
      _checkType(this.type.eltType);
      if !isCopyableType(this.type.eltType) then
        compilerError("Cannot copy vector with element type that " +
                      "cannot be copied");

      this.eltType = this.type.eltType;
      this.parSafe = this.type.parSafe;
      this.complete();

      _requestCapacity(other.size);
      _commonInitFromIterable(other);
    }

    /*
      Initializes a vector containing elements that are copied from
      the elements contained in an array.

      :arg other: The array to initialize from.
    */
    proc init=(other: [?d] this.type.eltType) {
      _checkType(this.type.eltType);
      if !isCopyableType(this.type.eltType) then
        compilerError("Cannot copy vector with element type that " +
                      "cannot be copied");
      this.eltType = this.type.eltType;
      this.parSafe = this.type.parSafe;
      this.complete();

      _requestCapacity(d.size);
      _commonInitFromIterable(other);
    }

    /*
      Initializes a vector containing elements that are copied from
      the elements yielded by a range.

      .. note::

        Attempting to initialize a vector from an unbounded range will trigger
        a compiler error.

      :arg other: The range to initialize from.
      :type other: `range(this.type.eltType)`
    */
    proc init=(other: range(this.type.eltType, ?b, ?d)) {
      _checkType(this.type.eltType);
      this.eltType = this.type.eltType;
      this.parSafe = this.type.parSafe;

      if !isBoundedRange(other) {
        param e = this.type:string;
        param f = other.type:string;
        param msg = "Cannot init " + e + " from unbounded " + f;
        compilerError(msg);
      }

      this.complete();

      _requestCapacity(other.size);
      _commonInitFromIterable(other);
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
    proc _commonInitFromIterable(iterable) {
      for x in iterable do {
        _append(x);
      }
    }

    pragma "no doc"
    proc ref _append(x: eltType) lifetime x > this {
      _requestCapacity(_size+1);
      _data[_size] = x;
      _size += 1;
    }

    pragma "no doc"
    proc ref _append(ref x: eltType) where isOwnedClass(x)
    lifetime x > this {
      _requestCapacity(_size+1);
      _data[_size] = x;
      _size += 1;
    }

    /*
      The current number of elements contained in this vector.
    */
    inline proc const size {
      var result = 0;

      on this {
        _enter();
        result = _size;
        _leave();
      }

      return result;
    }

    /*
      The capacity of this vector, which represents how much space is allocated.
    */
    inline proc const capacity {
      var result = 0;

      _enter();
      result = _capacity;
      _leave();

      return result;
    }

    /*
      Add an element to the end of this vector.

      :arg x: An element to append.
      :type x: `eltType`
    */
    proc ref append(x: eltType) lifetime x > this {
      _enter();

      _append(x);

      _leave();
    }

    pragma "no doc"
    proc ref append(ref x: eltType) where isOwnedClass(x)
    lifetime x > this {
      _enter();

      _append(x);

      _leave();
    }

    /*
      Returns `true` if this vector contains an element equal to the value of
      `x`, and `false` otherwise.

      :arg x: An element to search for.
      :type x: `eltType`

      :return: `true` if this vector contains `x`.
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
      Returns a reference to the first item in this vector.

      .. warning::

        Calling this method on an empty vector will cause the currently running
        program to halt. If the `--fast` flag is used, no safety checks will
        be performed.

      :return: A reference to the first item in this vector.
      :rtype: `ref eltType`
    */
    proc ref first() ref {
      _enter();

      if boundsChecking && _size == 0 {
        _leave();
        boundsCheckHalt("Called \"vector.first\" on an empty vector.");
      }

      ref result = _data[0];
      _leave();

      return result;
    }

    /*
      Returns a reference to the last item in this vector.

      .. warning::

        Calling this method on an empty vector will cause the currently running
        program to halt. If the `--fast` flag is used, no safety checks will
        be performed.

      :return: A reference to the last item in this vector.
      :rtype: `ref eltType`
    */
    proc ref last() ref {
      _enter();

      if boundsChecking && _size == 0 {
        _leave();
        boundsCheckHalt("Called \"vector.last\" on an empty vector.");
      }
     
      ref result = _data[_size-1];
      _leave();

      return result;  
    }

    pragma "no doc"
    inline proc const _withinBounds(idx: int): bool {
      return (idx >= 0 && idx < _size);
    }

    /*
      Insert an element at a given position in this vector, shifting all elements
      currently at and following that index one to the right. The call
      ``a.insert(0, x)`` inserts an element at the front of the vector `a`, and
      ``a.insert((a.size), x)`` is equivalent to ``a.append(x)``.

      If the insertion is successful, this method returns `true`. If the given
      index is out of bounds, this method does nothing and returns `false`.

      .. warning::
      
        Inserting an element into this vector may invalidate existing references
        to the elements contained in this vector.

      :arg idx: The index into this vector at which to insert.
      :type idx: `int`

      :arg x: The element to insert.
      :type x: `eltType`

      :return: `true` if `x` was inserted, `false` otherwise.
      :rtype: `bool`
    */
    proc ref insert(idx: int, x: eltType): bool {
      var result = false;
      on this {
        _enter();
        if (idx == _size) {
          _append(x);
          result = true;
        }
        else if _withinBounds(idx) {
          _expand(idx);
          _data[idx] = x;
          _size += 1;
          result = true;
        }
        _leave();
      }
      return result;
    }

    //
    // Shift elements including and after index `idx` so that they are moved
    // `shift` positions to the right in memory, possibly resizing. May
    // expand memory if necessary.
    //
    pragma "no doc"
    proc ref _expand(idx: int, shift: int=1) {

      if shift <= 0 then
        return;

      on this {
        _requestCapacity(_size + shift);

        for i in idx.._size-1 by -1 {
          _data[i+shift] = _data[i];
        }
      }
      return;
    }

    pragma "no doc"
    proc ref _insertGenericKnownSize(idx: int, items, size: int): bool {
      var result = false;

      if size == 0 then
        return true;
      on this {
        if idx == _size {
          _requestCapacity(_size + size);
          _extendGeneric(items);
          result = true;
        } else if _withinBounds(idx) {
          _requestCapacity(_size + size);
          _expand(idx, size);

          var i = idx;
          for x in items {
            _data[i] = x;
            _size += 1;
            i += 1;
          }

          result = true;
        }
      }

      return result;
    }

    /*
      Insert an array of elements `arr` into this vector at index `idx`,
      shifting all elements at and following the index `arr.size` positions
      to the right. 

      If the insertion is successful, this method returns `true`. If the given
      index is out of bounds, this method does nothing and returns `false`.

      .. warning::

        Inserting elements into this vector may invalidate existing references
        to the elements contained in this vector.

      :arg idx: The index into this vector at which to insert.
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
      Insert a list of elements `lst` into this vector at index `idx`, shifting
      all elements at and following the index `lst.size` positions to the
      right.

      If the insertion is successful, this method returns `true`. If the given
      index is out of bounds, this method does nothing and returns `false`.

      .. warning::

        Inserting elements into this vector may invalidate existing references
        to the elements contained in this vector.

      :arg idx: The index into this vector at which to insert.
      :type idx: `int`

      :arg lst: A list of elements to insert.
      :type lst: `list(eltType)`

      :return: `true` if `lst` was inserted, `false` otherwise.
      :rtype: `bool`
    */
    proc ref insert(idx: int, lst: list(eltType)): bool lifetime this < lst {
      
      var result = false;
      
      // Prevent deadlock if we are trying to insert this into itself.
      // Who want to do that?
      const size = lst.size;

      on this {
        _enter();
        result = _insertGenericKnownSize(idx, lst, size);
        _leave();
      }

      return result;
    }

    /*
      Insert a vector of elements `vec` into this vector at index `idx`, shifting
      all elements at and following the index `vec.size` positions to the
      right.

      If the insertion is successful, this method returns `true`. If the given
      index is out of bounds, this method does nothing and returns `false`.

      .. warning::

        Inserting elements into this vector may invalidate existing references
        to the elements contained in this vector.

      :arg idx: The index into this vector at which to insert.
      :type idx: `int`

      :arg lst: A vector of elements to insert.
      :type lst: `vector(eltType)`

      :return: `true` if `vec` was inserted, `false` otherwise.
      :rtype: `bool`
    */
    proc ref insert(idx: int, vec: vector(eltType)): bool lifetime this < vec {
      
      var result = false;
      
      // Prevent deadlock if we are trying to insert this into itself.
      const size = vec.size;

      on this {
        _enter();
        result = _insertGenericKnownSize(idx, vec, size);
        _leave();
      }

      return result;
    }

    /*
      Remove the first `count` elements from this vector with values equal to
      `x`, shifting all elements following the removed item left.

      If the count of elements to remove is less than or equal to zero, then
      all elements from this vector equal to the value of `x` will be removed.

      .. warning::

        Removing elements from this vector may invalidate existing references
        to the elements contained in this vector.

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
          if _data[dst_i] == x {
            removed += 1;
            break;
          }
          dst_i += 1;
        }

        // Once we've found an x, everything after has to be _move()d.
        // Run src_i ahead, moving everything that isn't x back to dst_i.
        src_i = dst_i + 1;
        while src_i < _size {
          if (count == 0 || removed < count) && _data[src_i] == x {
            src_i += 1;
            removed += 1;
            continue;
          }
          _data[dst_i] = _data[src_i];
          src_i += 1;
          dst_i += 1;
        }

        if (removed) {
          _size = _size - removed;
          _maybeDecreaseCapacity();
        }

        _leave();

        result = removed;
      }

      return result;
    }

    pragma "no doc"
    proc ref _popAtIndex(idx: int, unlockBeforeHalt=true): eltType {

      if boundsChecking && _size <= 0 {
        if unlockBeforeHalt then
          _leave();
        boundsCheckHalt("Called \"vector.pop\" on an empty vector.");
      }

      if boundsChecking && !_withinBounds(idx) {
        if unlockBeforeHalt then
          _leave();
        const msg = "Index for \"vector.pop\" out of bounds: " + idx:string;
        boundsCheckHalt(msg);
      }

      var result = _data[idx];
      _size -= 1;
      for i in idx .. _size-1 {
        _data[i] = _data[i+1];
      }
      _maybeDecreaseCapacity();

      return result;
    }

    /*
      Remove the element at the end of this vector and return it.

      .. warning::

        Popping an element from this vector will invalidate any reference to
        the element taken while it was contained in this vector.

      .. warning::

        Calling this method on an empty vector will cause the currently running
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
      Remove the element at the index `idx` from this vector and return it. The
      elements at indices after `idx` are shifted one to the left in memory,
      making this operation O(n).

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

    /*
      Clear the contents of this vector.

      .. warning::

        Clearing the contents of this vector will invalidate all existing
        references to the elements contained in this vector.
    */
    proc ref clear() {
      on this {
        _enter();

        _size = 0;
        _capacity = _initialCapacity;
        _maybeDecreaseCapacity();

        _leave();
      }
    }

    /*
      Return a zero-based index into this vector of the first item whose value
      is equal to `x`. If no such element can be found this method returns
      the value `-1`.

      .. warning::

        Calling this method on an empty vector or with values of `start` or 
        `end` that are out of bounds will cause the currently running program
        to halt. If the `--fast` flag is used, no safety checks will be
        performed.

      :arg x: An element to search for.
      :type x: `eltType`

      :arg start: The start index to start searching from.
      :type start: `int`

      :arg end: The end index to stop searching at. A value less than
                `0` will search the entire vector.
      :type end: `int`

      :return: The index of the element to search for, or `-1` on error.
      :rtype: `int`
    */
    proc const indexOf(x: eltType, start: int=0, end: int=-1): int {
      if boundsChecking {
        const msg = " index for \"vector.indexOf\" out of bounds: ";

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
          if x == _data[i] {
            result = i;
            break;
          }

        _leave();
      }

      return result;
    }

    /*
      Return the number of times a given element is found in this vector.

      :arg x: An element to count.
      :type x: `eltType`

      :return: The number of times a given element is found in this vector.
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
      Sort the items of this vector in place using a comparator. If no comparator
      is provided, sort this vector using the default sort order of its elements.

      .. warning::

        Sorting the elements of this vector may invalidate existing references
        to the elements contained in this vector.

      :arg comparator: A comparator used to sort this vector.
    */
    proc ref sort(comparator: ?rec=Sort.defaultComparator) {
      on this {
        _enter();

        if _size > 1 {
          Sort.sort(_data[0..#_size], comparator);
        }
        
        _leave();
      }
      return;
    }

    pragma "no doc"
    proc _requestCapacity(newCap: int) {
      if (_capacity >= newCap) then return;
      if (_capacity == 0) {
        _capacity = _initialCapacity;
      }
      while (_capacity < newCap) {
        _capacity *= 2;
      }

      _domain = {0..#_capacity};
    }

    pragma "no doc"
    proc _maybeDecreaseCapacity() {

      const threshold = _capacity/2;

      if threshold <= _initialCapacity then
        return;

      if _size > threshold then
        return;

      _capacity /= 2; 
      _domain = {0..#_capacity};
    }

    /*
      Request a change in capacity
    */
    proc requestCapacity(newCapacaity: int) {
      _enter();
      _requestCapacity(newCapacaity);
      _leave();
    }

    pragma "no doc"
    inline proc ref _extendGeneric(collection) {
      on this {
        for item in collection {
          _append(item);
        }
      }
    }

    /*
      Extend this vector by appending a copy of each element contained in
      the list.

      :arg other: A list containing elements of the same type as those
        contained in this vector.
      :type other: `list(eltType)`
    */
    proc ref extend(other: vector(eltType, ?p)) lifetime this < other {
      on this {
        _enter();
        _requestCapacity(_size + other.size);
        _extendGeneric(other);
        _leave();
      }
    }

    /*
      Extend this vector by appending a copy of each element contained in an
      array.

      :arg other: An array containing elements of the same type as those
        contained in this vector.
      :type other: `[?d] eltType`
    */
    proc ref extend(other: [?d] eltType) lifetime this < other {
      on this {
        _enter();
        _requestCapacity(_size + d.size);
        _extendGeneric(other);
        _leave();
      }
    }

    /*
      Extends this vector by appending a copy of each element yielded by a
      range.

      .. note::

        Attempting to initialize a vector from an unbounded range will trigger
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
        _requestCapacity(_size + other.size);
        _extendGeneric(other);
        _leave();
      }
    }

    /*
      Iterate over the elements of this vector.

      :yields: A reference to one of the elements contained in this vector.
    */
    iter these() ref {
      for i in 0..#_size {
        yield _data[i];
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
      for i in followThis(0) do
        yield this[i];
    }

    /*
      Index this vector via subscript. Returns a reference to the element at a
      given index in this vector.

      :arg i: The index of the element to access.

      .. warning::

        Use of the `this` method with an out of bounds index (while bounds
        checking is on) will cause the currently running program to halt.

      :return: An element from this vector.
    */
    proc ref this(i: int) ref {
      if boundsChecking && !_withinBounds(i) {
        const msg = "Invalid vector index: " + i:string;
        boundsCheckHalt(msg);
      }

      return _data[i];
    }

    proc const ref this(i: int) const ref {
      if boundsChecking && !_withinBounds(i) {
        const msg = "Invalid vector index: " + i:string;
        boundsCheckHalt(msg);
      }

      return _data[i];
    }

    /*
      Write the contents of this vector to a channel.

      :arg ch: A channel to write to.
    */
    proc readWriteThis(ch: channel) throws {
      _enter();
      
      ch <~> "[";

      for i in 0..(_size - 2) do
        ch <~> _data[i] <~> ", ";

      if _size > 0 then
        ch <~> _data[_size-1];

      ch <~> "]";

      _leave();
    }

    /*
      Returns `true` if this vector contains zero elements.

      :return: `true` if this vector is empty.
      :rtype: `bool`
    */
    proc const isEmpty(): bool {

      _enter();
      var result = (_size == 0);
      _leave();
      return result;
    }

    /*
      Returns the vector's legal indices as the range ``0..<this.size``.

      :return: ``0..<this.size``
      :rtype: `range`
    */
    proc const indices {
      return 0..<this.size;
    }

    /*
      Returns a new DefaultRectangular array containing a copy of each of the
      elements contained in this vector.

      :return: A new DefaultRectangular array.
    */
    proc const toArray(): [] eltType {
      if isNonNilableClass(eltType) && isOwnedClass(eltType) then
        compilerError("toArray() method is not available on a 'vector'",
                      " with elements of a non-nilable owned type, here: ",
                      eltType:string);

      var result: [0..#_size] eltType;
      on this {
        _enter();

         result = forall i in 0..#_size do _data[i];

        _leave();
      }

      return result;
    }
  }

  /*
    Clear the contents of this vector, then extend this now empty vector with the
    elements contained in another vector.

    .. warning::

      This will invalidate any references to elements previously contained in
      `lhs`.

    :arg lhs: The vector to assign to.
    :arg rhs: The vector to assign from. 
  */
  proc =(ref lhs: vector(?t, ?), rhs: vector(t, ?)) {
    lhs.clear();
    lhs.extend(rhs);
  }

  /*
    Returns `true` if the contents of two vectors are the same.

    :arg a: A vector to compare.
    :arg b: A vector to compare.

    :return: `true` if the contents of two vectors are equal.
    :rtype: `bool`
  */
  proc ==(a: vector(?t, ?), b: vector(t, ?)): bool {
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
    Return `true` if the contents of two vectors are not the same.

    :arg a: A vector to compare.
    :arg b: A vector to compare.

    :return: `true` if the contents of two vectors are not equal.
    :rtype: `bool`
  */
  proc !=(a: vector(?t, ?), b: vector(t, ?)): bool {
    return !(a == b);
  }
}
