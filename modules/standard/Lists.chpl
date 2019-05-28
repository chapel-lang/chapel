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
  This module contains the implementation of the List type.

  A List is a lightweight container similar to an array that is suitable for
  building up and iterating over a collection of elements in a structured
  manner.

  The highly parallel nature of Chapel means that great care should be taken
  when performing operations that may invalidate references to List elements.
  Inserts and removals into the middle of a List are example operations that
  may invalidate references. Appending an element to the end of a List will
  never invalidate references to elements contained in the List.

  The following operations may invalidate references to elements contained in
  a List:

      - insert
      - remove
      - reverse
      - sort
      - pop

  Additionally, all references to List elements are invalidated when the List
  is deinitialized.

  (Paragraph about parallel safety).

  Inserts and removals into a List are O(n) worst case and should be performed
  with care. For fast O(1) appending to either end consider the use of the
  Deque type instead.
*/
module Lists {

  pragma "no doc"
  config const _initialCapacity = 32;

  pragma "no doc"
  config const _growthFactor = 2;

  //
  // Building blocks used to store "chunks" of List elements. Inspired by
  // @vasslitvinov's VectorList implementation. Use this until we adopt a
  // O(1) indexing scheme recommended by @mppf.
  //
  pragma "no doc"
  class ListBlock {

    type _etype;
    const capacity: int;
    var size = 0;
    var data: _ddata(_etype);
    var next: unmanaged ListBlock(_etype) = nil;

    proc init(type _etype, capacity: int) {
      assert(capacity > 0);
      this._etype = _etype;
      this.capacity = capacity;
      this.size = 0;
      //
      // Use `c_calloc` to avoid calling initializers. TODO: Is use of calloc
      // safe in a multi-locale context? Michael said that the cast to
      // _ddata will convert the c_ptr to a wide pointer.
      //
      this.data = c_calloc(_etype, capacity):_ddata(_etype);
      this.next = nil;
    }

    proc deinit() {
      c_free(data:c_void_ptr);
    }

    inline proc this(i: int) ref {
      return data[i];
    }
  }

  /*
    A List is a lightweight container suitable for building up and iterating
    over a collection of elements in a structured manner. It is intended to
    replace the use of arrays to perform "vector-like" operations. Unlike a
    stack, the List type also supports inserts or removals into the middle of
    the List. The List type is close in spirit to its Python counterpart, with
    fast O(log n) (and O(1) eventually) indexing.

    The List type is parallel safe by default. For situations in which such
    overhead is undesirable, parallel safety can be disabled by setting
    `parSafe = false` in the List constructor. A List object constructed
    from another List object inherits the parallel safety mode of that List
    unless otherwise specified.
  */
  record List {

    /* The number of elements contained in this List. */
    var size = 0;

    /* The type of the elements contained in this List. */
    type eltType;

    /* If `true`, this List will use parallel safe operations. */
    param parSafe;

    pragma "no doc"
    const _lock$: nothing = none;

    //
    // Michael suggested that we move from a LL of blocks to an array, which
    // would give us constant speed indexing at the cost of more complicated
    // indexing logic.
    //
    // For now, I can start with Vass's logic and then switch over to a O(1)
    // indexing scheme when everything else is working.
    //
    pragma "no doc"
    var _head, _tail: unmanaged ListBlock(eltType) = nil;

    //
    // This is the sum of the capacities of all currently allocated ListBlock.
    //
    pragma "no doc"
    var _maxCapacity: int = 0;

    /*
      Initializes an empty List containing elements of the given type.

      :arg eltType: The type of the elements of this List.
      :arg parSafe: If `true`, this List will use parallel safe operations.
        Is `true` by default.
    */
    proc init(type eltType, param parSafe=true) {
      this.eltType = eltType;
      this.parSafe = parSafe;
    }

    /*
      Initializes a List containing elements that are copy initialized from
      the elements in the old List.

      :arg other: The List object to initialize from.
      :arg parSafe: If `true`, this List will use parallel safe operations.
        Inherits the value from the other List by default.
    */
    proc init=(other: List(?t), param parSafe=other.parSafe) {
      this.eltType = t;
      this.parSafe = other.parSafe;
      this.complete();
      this.extend(other);
    }

    pragma "no doc"
    inline proc deinit() {
      clear();
      assert(_head == nil && _tail == nil);
      assert(size == 0);
    }

    // This may not be necessary, so don't fixate too much on it.
    pragma "no doc"
    proc _cast(type t: string, x: List): string {
      return "List cast not implemented yet!";
    }

    pragma "no doc"
    inline proc _destroy(ref item: eltType) {
      chpl__autoDestroy(item);
    }

    pragma "no doc"
    proc _move(ref lhs: ?t, ref rhs: t) {
      __primitive("=", lhs, rhs);
    }

    //
    // Right now, Chapel ranges typically start from one and are inclusive on
    // either side of the range (as opposed to ranges in Python, which are
    // only open on the left).
    // So make sure to keep this in mind when working with _ddata (which uses
    // zero based indexing).
    //
    pragma "no doc"
    proc _getRef(in idx: int) ref {
      assert(_withinBounds(idx));

      var start = _head;

      while idx > start.capacity do {
        idx -= start.capacity;
        start = start.next;
      }

      return start.data[idx - 1];
    }

    pragma "no doc"
    inline proc _enter() {
      // TODO: Lock call goes here.
      if parSafe then
        return;
    }

    pragma "no doc"
    inline proc _leave() {
      // TODO: Unlock call goes here.
      if parSafe then
        return;
    }

    pragma "no doc"
    inline proc _withinBounds(i: int): bool {
      return (i >= 1 && i <= size);
    }

    //
    // This call assumes that a lock (acquired by `_cs_enter`) is already
    // held (if parSafe==true), and releases it before throwing an error if
    // a bounds check fails.
    //
    pragma "no doc"
    inline proc _boundsCheckReleaseOnThrow(i: int) throws {
      if !_withinBounds(i) then {
        _leave();
        throw new owned
          IllegalArgumentError("List index out of bounds: ", i);
      }
    }

    //
    // Shift elements after index one to the right, inclusive.
    //
    pragma "no doc"
    proc _expand(idx: int) {
      assert(_withinBounds(idx));
      
      _maybeAcquireMem(1);

      for i in size..idx do {
        ref rhs = _getRef(i);
        ref lhs = _getRef(i + 1);
        _move(lhs, rhs);
      }
    }

    //
    // Shift elements after index one to the left, exclusive.
    //
    pragma "no doc"
    proc _collapse(idx: int) {
      assert(_withinBounds(idx));

      for i in 2..idx do {
        ref rhs = _getRef(i);
        ref lhs = _getRef(i - 1);
        _move(lhs, rhs);
      }

      _maybeReleaseMem(1);
    }

    //
    // Potentially reserve memory at the end of this array.
    //
    pragma "no doc"
    proc _maybeAcquireMem(in amount: int) {
      // Edge case for when this List is empty.
      if _head == nil then {
        assert(_tail == nil);
        _head = new unmanaged ListBlock(eltType, _initialCapacity);
        _maxCapacity = _initialCapacity;
        _tail = _head;
      }

      // Create a new chunk if we need to.
      while (size + amount) >= _maxCapacity do {
        assert(_tail.next == nil);
        const blocksize = _tail.capacity * _growthFactor;
        _tail.next = new unmanaged ListBlock(eltType, blocksize);
        _tail = _tail.next;
        amount -= blocksize;
        _maxCapacity += blocksize;
      }
    }

    //
    // Potentially release memory at the end of this array.
    //
    pragma "no doc"
    proc _maybeReleaseMem(in amount: int=1) {
      if _head == nil || _head == _tail {
        assert(_tail == nil);
        return;
      }

      const threshold = _maxCapacity - _tail.capacity;
      if size >= threshold then
        return;

      var node = _head;
      while node.next != _tail do
        node = node.next;

      _maxCapacity = threshold;
      delete node.next;
      node.next = nil;
      _tail = node;
    }

    //
    // Assumes that a copy of the input element has already been made at some
    // previous boundary, IE giving append's parameter the "in" intent.
    // We also make sure to increment the element size before performing the
    // move so that the assert in _getRef() doesn't fire (even though the
    // memory isn't properly initialized until after the move).
    //
    pragma "no doc"
    proc _append(ref x: eltType) {
      _maybeAcquireMem(1);
      size += 1;
      ref rhs = x;
      ref lhs = _getRef(size);
      _move(lhs, rhs);
    }

    /*
      Add an element to the end of this List.

      :arg x: An element to append.
    */
    proc append(pragma "no auto destroy" in x: eltType) {
      _enter();
      _append(x);
      _leave();
    }

    pragma "no doc"
    inline proc _extendGeneric(collection) {

      //
      // TODO: This could be faster if we resized once and then performed
      // repeated moves, rather than calling _append().
      //
      for x in collection do
        _append(x);
    }

    pragma "no doc"
    inline proc _extendGenericAlter(collection) {
      const csize = collection.size;
      const oldsize = size;
      
      _maybeAcquireMem(csize);
      size += csize;
      
      for i in 1..csize do {
        pragma "no auto destroy"
        var cpy = collection[i];
        ref rhs = cpy;
        ref lhs = _getRef(oldsize + i);
        _move(lhs, rhs);
      }
    }

    /*
      Extend this List by appending a copy of each element contained in
      another List.

      :arg other: A List containing elements of the same type as those
        contained in this List.
    */
    proc extend(other: List(?t)) where t == eltType {
      _enter();
      _extendGeneric(other);
      _leave();
    }

    /*
      Extend this List by appending a copy of each element contained in an
      array.

      :arg other: An array containing elements of the same type as those
        contained in this List.
    */
    proc extend(other: [?d] ?t) where t == eltType {
      _enter();
      _extendGeneric(other);
      _leave();
    }

    /*
      Insert an element at a given position in this List, shifting all elements
      following that index one to the right.

      .. warning::
      
        Inserting an element into this List may invalidate existing references
        to the elements contained in this List.

      :arg i: The index of the element at which to insert.
      :arg x: The element to insert.

      :throws IllegalArgumentError: If the given index is out of bounds.
    */
    proc insert(i: int, pragma "no auto destroy" in x: eltType) throws {
      _enter();
      try _boundsCheckReleaseOnThrow(i);
      _expand(i);
      ref lhs = _getRef(i);
      ref rhs = x;
      _move(lhs, rhs);
      size += 1;
      _leave();
    }

    /*
      Remove the first item from this List whose value is equal to x, shifting
      all elements following the removed item one to the left.

      .. warning::

        Removing an element from this List may invalidate existing references
        to the elements contained in this List.

      :arg x: The element to remove.

      :throws IllegalArgumentError: If there is no such item.
    */
    proc remove(x: eltType) throws {
      _enter();

      //
      // TODO: Using this operator over entire list gives us O(n log n) perf,
      // which is suboptimal, but will reduce to O(n) when this becomes O(1).
      //
      for i in 1..size do {
          ref item = _getRef(i);
          if x == item then {
            _destroy(item);
            _collapse(i);
            _leave();
            return;
          }
      }

      _leave();

      throw new owned
        IllegalArgumentError("No such element in List: ", x);
    }

    /*
      Remove the item at the given position in this List, and return it. If no
      index is specified, remove and return the last item in this List.

      .. warning::

        Popping an element from this List will invalidate any reference to
        the element taken while it was contained in this List.

      :arg i: The index of the element to remove. Defaults to the last item
        in this List.

      :return: The item removed.

      :throws IllegalArgumentError: If the given index is out of bounds.
    */
    proc pop(i: int=size): eltType throws {
      _enter();
      try _boundsCheckReleaseOnThrow(i);
      // TODO: Is this fine, or do I need to disable initializers here?
      // TODO: Do I need to invoke a destructor here?
      var result = _getRef(i);
      _collapse(i);
      _leave();
      return result;
    }

    /*
      Clear the contents of this List.

      .. warning::

        Clearing the contents of this List will invalidate all existing
        references to the elements contained in this List.
    */
    proc clear() {
      _enter();

      // Manually call destructors on each currently allocated element.
      for i in 1..size do {
        ref item = this[i];
        chpl__autoDestroy(item);
      }

      // Free all currently allocated blocks.
      while _head != nil do {
        var block = _head;
        _head = _head.next;
        delete block;
      }

      _head = nil;
      _tail = nil;
      size = 0;

      _leave();
    }

    /*
      Return a zero-based index into this List of the first item whose value
      is equal to x.

      :arg x: An element to search for.
      :arg start: The start index to start searching from.
      :arg end: The end index to stop searching at.

      :return: The index of the element to search for.

      :throws IllegalArgumentError: If the given element cannot be found.
    */
    proc indexOf(x: eltType, start: int=0, end: int=size): int throws {
      _enter();

      try _boundsCheckReleaseOnThrow(start);
      try _boundsCheckReleaseOnThrow(end);

      for i in start..end do
        if x == _getRef(i) then {
          _leave();
          return i;
        }

      _leave();

      throw new owned
        IllegalArgumentError("No such element in List: ", x);

      // Should never reach here.
      return -1;
    }

    /*
      Return the number of times a given element is found in this List.

      :arg x: An element to count.

      :return: The number of times a given element is found in this List.
      :rtype: `int`
    */
    proc count(x: eltType): int {
      _enter();

      var result = 0;

      for i in 1..size do {
        if x == _getRef(i) then
          result += 1;
      }

      _leave();
      return result;
    }

    /*
       Sort the elements of this List in place using their default sort order.

       .. warning::

        Sorting the contents of this List may invalidate existing references
        to the elements contained in this List.
    */
    proc sort() {
      // TODO: Need to look in the Sort module to see the API.
    }

    /*
      Sort the items of this List in place using a comparator.

      .. warning::

        Sorting the elements of this List may invalidate existing references
        to the elements contained in this List.

      :arg comparator: A comparator object used to sort this List.
    */
    proc sort(comparator) {
      // TODO: Need to look in the Sort module to see the API. 
    }

    /*
      Index this List via subscript. Returns a reference to the element at a
      given index in this List.

      :arg i: The index of the element to access.

      :return: An element from this List.

      :throws IllegalArgumentError: If the given index is out of bounds.
    */
    proc this(i: int) ref throws {
      _enter();
      try _boundsCheckReleaseOnThrow(i);
      ref result = _getRef(i);
      _leave();
      return result;
    }

    /*
      Produce a serial iterator over the elements of this List.

      :yields: A reference to one of the elements contained in this List.
    */
    iter these() ref {

      //
      // TODO: I'm not even sure of what the best way to WRITE a threadsafe
      // iterator is, _let alone_ whether it should even be threadsafe
      // (I mean, is there even a point when reference/iterator invalidation
      // is still a thing?
      //
      for i in 1..size do {
        _enter();
        ref result = _getRef(i);
        _leave();
        yield result;
      }
    }

    /*
      Write the contents of this List to a channel.

      :arg ch: A channel to write to.

      :throws SystemError: When a write to the channel fails.
    */
    proc writeThis(ch: channel) {
      _enter();

      // 
      // TODO TODO TODO TODO
      // I must be missing something about the writeThis operator, because how
      // are we supposed to _properly_ implement this now that "write" can
      // throw?
      //
      try! {
        ch.write("[");

        for i in 1..(size - 1) do
          ch.write(_getRef(i), ", ");

        if size > 0 then
          ch.write(_getRef(size));

        ch.write("]");
      }

      _leave();
    }

    /*
      Returns `true` if this List contains zero elements.

      :return: `true` if this List is empty.
      :rtype: `bool`
    */
    inline proc isEmpty(): bool {
      _enter();
      var result = (size == 0);
      _leave();
      return result;
    }

    /*
      Returns a new DefaultRectangular array containing a copy of each of the
      elements contained in this List.

      :return: A new DefaultRectangular array.
    */
    proc toArray(): [] eltType {
      _enter();

      //
      // TODO: How to allocate memory for array when we may not be able to
      // default initialize?
      //
      var result: [1..size] eltType;

      for i in 1..size do
        result[i] = this[i];

      _leave();

      return result;
    }

  } // End class List!

  /*
    Clear the contents of this List, then extend this now empty List with the
    elements contained in another List.

    .. warning::

      Assigning another List to this List will invalidate any references to
      elements previously contained in this List.

    :arg lhs: The List object to assign to.
    :arg rhs: The List object to assign from. 
  */
  proc =(lhs: List(?t), rhs: List(t)) {
    lhs.clear();
    lhs.extend(rhs);
  }

  /*
    Returns `true` if the contents of two Lists are the same.

    :arg a: A List to compare.
    :arg b: A List to compare.

    :return: `true` if the contents of two Lists are the same.
    :rtype: `bool`
  */
  proc ==(a: List(?t), b: List(t)): bool {
    if a.size != b.size then
      return false;

    for i in 1..(a.size) do
      if a[i] != b[i] then
        return false;

    return true;
  }

} // End module Lists!

