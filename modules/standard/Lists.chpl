/*
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
module Lists {

  pragma "no doc"
  config const _initialCapacity = 8;

  pragma "no doc"
  config const _initialArrayCapacity = 16;

  pragma "no doc"
  config param _sanityChecks = true;

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

  /*
    A list is a lightweight container suitable for building up and iterating
    over a collection of elements in a structured manner. Unlike a stack, the
    list type also supports inserts or removals into the middle of the list.
    The list type is close in spirit to its Python counterpart, with fast O(1)
    random access and append operations.

    The list type is not parallel safe by default. For situations in which
    such protections are desirable, parallel safety can be enabled by setting
    `parSafe = true` in any list constructor. A list constructed from another
    list inherits the parallel safety mode of that list by default.
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

    /*
      Initializes an empty list containing elements of the given type.

      :arg eltType: The type of the elements of this list.
      :arg parSafe: If `true`, this list will use parallel safe operations.
    */
    proc init(type eltType, param parSafe=false) {
      this.eltType = eltType;
      this.parSafe = parSafe;
    }

    /*
      Initializes a list containing elements that are copy initialized from
      the elements contained in another list.

      :arg other: The list to initialize from.
      :arg parSafe: If `true`, this list will use parallel safe operations.
    */
    proc init=(const ref other: list(?t), param parSafe=other.parSafe) {
      this.eltType = t;
      this.parSafe = other.parSafe;
      this._arrays = nil;
      this.complete();
      this.extend(other);
    }

    pragma "no doc"
    inline proc deinit() {
      clear();
    }

    pragma "no doc"
    inline proc _destroy(ref item: eltType) {
      chpl__autoDestroy(item);
    }

    //
    // Getting weird lifetime errors when using this function over classes,
    // and I'm not sure quite how to solve them yet. Since this is used in a
    // managed way internally, try marking "unsafe" for now to circumvent
    // the errors, and see if we can deal with them later.
    //
    pragma "no doc"
    pragma "unsafe"
    inline proc _move(ref src: ?t, ref dst: t) {
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
    // Performs conversion from one-based to zero-based indexing, all one-based
    // accesses of list elements should go through this function.
    //
    pragma "no doc"
    proc _getRef(idx: int) ref {
      _sanity(idx >= 1 && idx <= _totalCapacity);
      const zpos = idx - 1;
      const arrayIdx = _getArrayIdx(zpos);
      const itemIdx = _getItemIdx(zpos);
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
    inline proc _withinBounds(idx: int): bool {
      return (idx >= 1 && idx <= _size);
    }

    //
    // This call assumes that a lock (acquired by `_enter`) is already
    // held (if parSafe==true), and releases it before throwing an error if
    // a bounds check fails.
    //
    pragma "no doc"
    inline proc _boundsCheckLeaveOnThrow(i: int, umsg: string="") throws {
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
      //
      // TODO
      // Adopt a new implementation where the first array is always allocated.
      //
      if _arrays == nil {
        _sanity(_totalCapacity == 0);
        _sanity(_size == 0);
        _arrays = _makeBlockArray(_initialArrayCapacity);
        _arrayCapacity = _initialArrayCapacity;
        _arrays[0] = _makeArray(_initialCapacity);
        _totalCapacity = _initialCapacity;
      }

      const remaining = _totalCapacity - _size;
      if remaining > amount then
        return;

      var lastArrayIdx = _getLastArrayIdx();
      var req = amount - remaining;

      while req > 0 {

        //
        // Double the block array if we've run out of space.
        //
        if lastArrayIdx >= (_arrayCapacity - 1) {
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

    //
    // If the current size (occupied slots) minus a given amount is small
    // enough such that no slots in the last "sub-block" are occupied, then
    // preemptively free that block. This method _does not_ fire destructors!
    //
    pragma "no doc"
    proc _maybeReleaseMem(amount: int) {
      if _arrays == nil || _totalCapacity == 0 then
        return;

      const lastArrayIdx = _getLastArrayIdx();
      const lastArrayCapacity = _getArrayCapacity(lastArrayIdx);
      const threshold = _totalCapacity - lastArrayCapacity;
      const nsize = _size - amount;

      if nsize >= threshold then
        return;

      ref array = _arrays[lastArrayIdx];
      _freeArray(array, lastArrayCapacity);
      _totalCapacity -= lastArrayCapacity;
      array = nil; 
    }

    //
    // Shift elements including and after index one to the right in memory,
    // possibly resizing. May expand memory if necessary.
    //
    pragma "no doc"
    proc _expand(idx: int) {
      _sanity(_withinBounds(idx));

      _maybeAcquireMem(1);

      for i in idx.._size by -1 {
        ref src = _getRef(i);
        ref dst = _getRef(i + 1);
        _move(src, dst);
      }
    }

    //
    // Shift all elements after index one to the left in memory, possibly
    // resizing. This assumes the element at `idx` has already been
    // deinitialized if that is necessary. May release memory if possible.
    //
    pragma "no doc"
    proc _collapse(idx: int) {
      _sanity(_withinBounds(idx));

      if idx == _size then
        return;

      for i in idx.._size {
        ref src = _getRef(i + 1);
        ref dst = _getRef(i);
        _move(src, dst);
      }

      _maybeReleaseMem(1);
    }

    //
    // Assumes that a copy of the input element has already been made at some
    // previous boundary, IE giving a parameter the "in" intent. Whatever 
    // copy you've made, make sure that the "no auto destroy" pragma is
    // attached so that you avoid firing a destructor early (and in the worst
    // case, fire it twice).
    //
    pragma "no doc"
    proc _append_by_ref(ref x: eltType) {
      _maybeAcquireMem(1);
      ref src = x;
      ref dst = _getRef(_size + 1);
      _move(src, dst);
      _size += 1;
    }

    /*
      Add an element to the end of this list.

      :arg x: An element to append.
    */
    proc append(x: eltType) {
      //
      // TODO: Use a local copy until this pragma works with formals.
      // See: https://github.com/chapel-lang/chapel/issues/13225
      //
      pragma "no auto destroy"
      var cpy = x;
      _enter();
      _append_by_ref(cpy);
      _leave();
    }

    pragma "no doc"
    inline proc _extendGeneric(collection) {

      //
      // TODO: This could avoid repeated resizes at smaller total capacities
      // if we resized once and then performed repeated moves, rather than
      // calling _append().
      //
      for item in collection {
        // See: https://github.com/chapel-lang/chapel/issues/13225
        pragma "no auto destroy"
        var cpy = item;
        _append_by_ref(cpy);
      }
    }

    /*
      Extend this list by appending a copy of each element contained in
      another list.

      :arg other: A list containing elements of the same type as those
        contained in this list.
    */
    proc extend(other: list(eltType, ?)) {
      _enter();
      _extendGeneric(other);
      _leave();
    }

    /*
      Extend this list by appending a copy of each element contained in an
      array.

      :arg other: An array containing elements of the same type as those
        contained in this list.
    */
    proc extend(other: [?d] eltType) {
      _enter();
      _extendGeneric(other);
      _leave();
    }

    /*
      Insert an element at a given position in this list, shifting all elements
      currently at and following that index one to the right. The call
      ``a.insert(1, x)`` inserts an element at the front of the list `a`, and
      ``a.insert((a.size + 1), x)`` is equivalent to ``a.append(x)``.

      .. warning::
      
        Inserting an element into this list may invalidate existing references
        to the elements contained in this list.

      :arg i: The index of the element at which to insert.
      :arg x: The element to insert.

      :throws IllegalArgumentError: If the given index is out of bounds.
    */
    proc insert(i: int, x: eltType) throws {
      _enter();

      // TODO: Use a local copy until this pragma works with formals.
      pragma "no auto destroy"
      var cpy = x;

      // Handle special case of `a.insert((a.size + 1), x)` here.
      if i == _size + 1 {
        _append_by_ref(cpy);
        _leave();
        return;
      }

      try _boundsCheckLeaveOnThrow(i);
      // May acquire memory based on size before insert.
      _expand(i);
      ref src = cpy;
      ref dst = _getRef(i);
      _move(src, dst);
      _size += 1;
      _leave();
    }

    /*
      Remove the first item from this list whose value is equal to x, shifting
      all elements following the removed item one to the left.

      .. warning::

        Removing an element from this list may invalidate existing references
        to the elements contained in this list.

      :arg x: The element to remove.

      :throws IllegalArgumentError: If the list contains no such element.
    */
    proc remove(x: eltType) throws {
      _enter();

      for i in 1.._size {
        ref item = _getRef(i);
        if x == item {
          _destroy(item);
          // May release memory based on size before remove.
          _collapse(i);
          _maybeReleaseMem(1);
          _size -= 1;
          _leave();
          return;
        }
      }

      _leave();

      const msg = "No such element in list: " + x:string;
      throw new owned
        IllegalArgumentError(msg);
    }

    /*
      Remove the item at the given position in this list, and return it. If no
      index is specified, remove and return the last item in this list.

      .. warning::

        Popping an element from this list will invalidate any reference to
        the element taken while it was contained in this list.

      :arg i: The index of the element to remove. Defaults to the last item
        in this list.

      :return: The item removed.

      :throws IllegalArgumentError: If the given index is out of bounds.
      :throws IllegalArgumentError: If the list is empty.
    */
    proc pop(i: int=size): eltType throws {
      _enter();

      if _size <= 0 {
        _leave();
        const msg = "Pop called on empty list.";
        throw new owned
          IllegalArgumentError(msg);
      }
      
      try _boundsCheckLeaveOnThrow(i);

      //
      // What about situations where "result" does not have a default init?
      // In such situations we would have to initialize result from a
      // reference to item, as below, rather than a default init followed by
      // a move.
      //
      ref item = _getRef(i);
      var result = item;
      _destroy(item);
      // May release memory based on size before pop.
      _collapse(i);
      _size -= 1;

      _leave();
      return result;
    }

    /*
      Clear the contents of this list.

      .. warning::

        Clearing the contents of this list will invalidate all existing
        references to the elements contained in this list.
    */
    proc clear() {
      _enter();

      //
      // Manually call destructors on each currently allocated element. Use
      // one-based indexing here since we're going through _getRef().
      //
      for i in 1.._size {
        ref item = _getRef(i);
        _destroy(item);
      }

      //
      // Loop through all arrays and clean them up, then clean up the "block
      // array" itself.
      //
      if _arrays != nil {
        _sanity(_totalCapacity != 0);
        _sanity(_arrayCapacity != 0);
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

        _freeBlockArray(_arrays, _arrayCapacity);
        _arrays = nil;
      }

      _sanity(_totalCapacity == 0);
      _size = 0;

      _leave();
    }

    /*
      Return a one-based index into this list of the first item whose value
      is equal to x.

      :arg x: An element to search for.
      :arg start: The start index to start searching from.
      :arg end: The end index to stop searching at.

      :return: The index of the element to search for.

      :throws IllegalArgumentError: If the given element cannot be found.
    */
    proc indexOf(x: eltType, start: int=1, end: int=size): int throws {
      _enter();

      try _boundsCheckLeaveOnThrow(start,
          "Start index out of bounds: " + start:string);
      try _boundsCheckLeaveOnThrow(end,
          "End index out of bounds: " + end:string);

      for i in start..end do
        if x == _getRef(i) {
          _leave();
          return i;
        }

      _leave();

      // TODO: Introduce ValueError and use that here instead.
      const msg = "No such element: " + x:string;
      throw new owned
        IllegalArgumentError(msg);

      // Should never reach here.
      return -1;
    }

    /*
      Return the number of times a given element is found in this list.

      :arg x: An element to count.

      :return: The number of times a given element is found in this list.
      :rtype: `int`
    */
    proc count(x: eltType): int {
      _enter();

      var result = 0;

      for i in 1.._size do
        if x == _getRef(i) then
          result += 1;

      _leave();
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
    proc sort(comparator=Sort.defaultComparator) {
      //
      // TODO: This is not ideal, but the Sort API needs to be adjusted before
      // we can sort over lists directly.
      //
      var array = toArray();
      clear();
      Sort.sort(array, comparator);
      extend(array);
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
    proc const this(i: int) ref {
      _enter();

      if boundsChecking && !_withinBounds(i) {
        _leave();
        const msg = "Invalid list index: " + i:string;
        halt(msg);
      }

      ref result = _getRef(i);
      _leave();
      return result;
    }

    /*
      Iterate over the elements of this list.

      :yields: A reference to one of the elements contained in this list.
    */
    iter these() ref {

      //
      // TODO: I'm not even sure of what the best way to WRITE a threadsafe
      // iterator is, _let alone_ whether it should even be threadsafe
      // (I mean, is there even a point when reference/iterator invalidation
      // is still a thing?).
      //
      for i in 1.._size {
        _enter();
        ref result = _getRef(i);
        _leave();
        yield result;
      }
    }

    /*
      Write the contents of this list to a channel.

      :arg ch: A channel to write to.
    */
    proc writeThis(ch: channel) {
      _enter();
      
      //
      // TODO: Should this method throw? The current implementation uses <~>
      // so that it does not have to throw, but it also will not report
      // any IO errors.
      //

      ch <~> "[";

      for i in 1..(_size - 1) do
        ch <~> _getRef(i) <~> ", ";

      if _size > 0 then
        ch <~> _getRef(_size);

      ch <~> "]";

      _leave();
    }

    /*
      Returns `true` if this list contains zero elements.

      :return: `true` if this list is empty.
      :rtype: `bool`
    */
    inline proc const isEmpty(): bool {

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

      //
      // TODO: Ditto the above code comment.
      //
      _enter();
      var result = _size;
      _leave();
      return result;
    }

    /*
      Returns a new DefaultRectangular array containing a copy of each of the
      elements contained in this list.

      :return: A new DefaultRectangular array.
    */
    proc toArray(): [] eltType {
      _enter();

      var result: [1.._size] eltType;

      for i in 1.._size do
        result[i] = _getRef(i);

      _leave();

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
  proc =(ref lhs: list(?t, ?), const ref rhs: list(t, ?)) {
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
  proc ==(const ref a: list(?t, ?), const ref b: list(t, ?)): bool {
    if a.size != b.size then
      return false;

    //
    // TODO: Make this a forall loop eventually.
    //
    for i in 1..(a.size) do
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
  proc !=(const ref a: list(?t, ?), const ref b: list(t, ?)): bool {
    return !(a == b);
  }

} // End module "Lists".

