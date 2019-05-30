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
      - sort
      - pop
      - clear

  Additionally, all references to List elements are invalidated when the List
  is deinitialized.

  List objects are not parallel safe by default, but can be made parallel safe
  by setting the param formal `parSafe` to true in any List constructor. When
  constructed from another List, a List object will inherit the parallel
  safety mode of its originating List.

  Inserts and removals into a List are O(n) worst case and should be performed
  with care. Access into a List object is currently O(log n), but is expected
  to reduce to O(1) in the future.
*/
module Lists {

  pragma "no doc"
  config const _initialCapacity = 8;

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
    var _data: _ddata(_etype);
    const capacity: int;
    var next: unmanaged ListBlock(_etype) = nil;

    proc init(type _etype, capacity: int) {
      assert(capacity > 0);
      this._etype = _etype;
      //
      // Use `c_calloc` to avoid calling initializers. TODO: Is use of calloc
      // safe in a multi-locale context? Michael said that the cast to
      // _ddata will convert the c_ptr to a wide pointer.
      //
      this._data = c_calloc(_etype, capacity):_ddata(_etype);
      this.capacity = capacity;
      this.next = nil;
    }

    proc deinit() {
      c_free(_data:c_void_ptr);
    }

    inline proc this(i: int) ref {
      return _data[i];
    }
  }

  //
  // We can change the lock type later. Use a spinlock for now, even if it
  // is suboptimal in cases where long critical sections have high
  // contention (IE, lots of tasks trying to insert into the middle of this
  // List, or any operation that is O(n)).
  //
  pragma "no doc"
  type lockType = ChapelLocks.chpl_LocalSpinlock;

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

    pragma "no doc"
    var _size = 0;

    /* The type of the elements contained in this List. */
    type eltType;

    /* If `true`, this List will perform parallel safe operations. */
    param parSafe;

    //
    // We can change the lock type later. Use a spinlock for now, even if it
    // is suboptimal in cases where long critical sections have high
    // contention (IE, lots of tasks trying to insert into the middle of this
    // List, or any operation that is O(n).
    //
    // pragma "no doc"
    // type lockType = ChapelLocks.chpl_LocalSpinlock;

    //
    // I wanted to do the little piece of wizardry @ronawho used to make the
    // _lock$ variable reduce to "nothing" when parSafe is false, but I was
    // getting strange compiler errors. See #13104 for the idea.
    //
    pragma "no doc"
    var _lock$ = if parSafe then new lockType() else none;

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
    proc init(type eltType, param parSafe=false) {
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
      _enter();
      assert(_head == nil && _tail == nil);
      assert(_size == 0);
      _leave();
    }

    //
    // This may not be necessary, so don't fixate too much on it.
    //
    //pragma "no doc"
    //proc _cast(type t: string, x: List): string {
     // assert("List cast not implemented yet!" == "");
    //}

    pragma "no doc"
    inline proc _destroy(ref item: eltType) {
      chpl__autoDestroy(item);
    }

    pragma "no doc"
    proc _move(ref src: ?t, ref dst: t) {
      __primitive("=", dst, src);
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
      var start = _head;

      while start != nil && idx > start.capacity do {
        idx -= start.capacity;
        start = start.next;
      }

      // If this assertion fires, then we've abused _getRef() in some way.
      assert(start != nil && idx <= start.capacity && idx > 0);

      return start[idx - 1];
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
    inline proc _withinBounds(i: int): bool {
      return (i >= 1 && i <= _size);
    }

    //
    // This call assumes that a lock (acquired by `_enter`) is already
    // held (if parSafe==true), and releases it before throwing an error if
    // a bounds check fails.
    //
    pragma "no doc"
    inline proc _boundsCheckLeaveOnThrow(i: int) throws {
      if !_withinBounds(i) then {
        _leave();
        const msg = "List index out of bounds: " + i:string;
        throw new owned
          IllegalArgumentError(msg);
      }
    }

    //
    // Shift elements including and after index one to the right in memory,
    // possibly resizing.
    //
    pragma "no doc"
    proc _expand(idx: int) {
      assert(_withinBounds(idx));
      
      _maybeAcquireMem(1);

      for i in idx.._size by -1 do {
        ref src = _getRef(i);
        ref dst = _getRef(i + 1);
        _move(src, dst);
      }
    }

    //
    // Shift all elements after index "idx" one to the left in memory,
    // possibly resizing.
    //
    pragma "no doc"
    proc _collapse(idx: int) {
      assert(_withinBounds(idx));

      if idx == _size then
        return;

      for i in idx.._size do {
        ref src = _getRef(i + 1);
        ref dst = _getRef(i);
        _move(src, dst);
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
      while (_size + amount) >= _maxCapacity do {
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
        return;
      }
      
      assert(amount >= 0 && amount <= _size);
      const nsize = _size - amount;

      const threshold = _maxCapacity - _tail.capacity;
      if nsize >= threshold then
        return;

      _maxCapacity = threshold;

      var node = _head;
      while node.next != _tail do
        node = node.next;

      delete node.next;
      node.next = nil;
      _tail = node;
    }

    //
    // Assumes that a copy of the input element has already been made at some
    // previous boundary, IE giving append's parameter the "in" intent.
    //
    pragma "no doc"
    proc _append(ref x: eltType) {
      _maybeAcquireMem(1);
      ref src = x;
      ref dst = _getRef(_size + 1);
      _move(src, dst);
      _size += 1;
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
      const oldSize = _size;
      
      _maybeAcquireMem(collection.size);
      
      for i in 1..collection.size do {
        pragma "no auto destroy"
        var cpy = collection[i];
        ref src = cpy;
        ref dst = _getRef(oldSize + i);
        _move(src, dst);
      }

      _size += collection.size;
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
      try _boundsCheckLeaveOnThrow(i);
      _expand(i);
      ref src = x;
      ref dst = _getRef(i);
      _move(src, dst);
      _size += 1;
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

      for i in 1.._size do {
          ref item = _getRef(i);
          if x == item then {
            _destroy(item);
            _collapse(i);
            _size -= 1;
            _leave();
            return;
          }
      }

      _leave();

      const msg = "No such element in List: " + x:string;
      throw new owned
        IllegalArgumentError(msg);
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
      try _boundsCheckLeaveOnThrow(i);
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
      for i in 1.._size do {
        ref item = _getRef(i);
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
      _size = 0;

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

      try _boundsCheckLeaveOnThrow(start);
      try _boundsCheckLeaveOnThrow(end);

      for i in start..end do
        if x == _getRef(i) then {
          _leave();
          return i;
        }

      _leave();

      const msg = "No such element in List: " + x:string;
      throw new owned
        IllegalArgumentError(msg);

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

      for i in 1.._size do {
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
      try _boundsCheckLeaveOnThrow(i);
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
      for i in 1.._size do {
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
      //
      // Presently, if I try to let these calls throw, then they will trigger
      // an error in modules/standard/IO.chpl along the lines of "error: call
      // to throwing function writeThis without throws, try, or try!
      //
      try {
        ch.write("[");

        for i in 1..(_size - 1) do
          ch.write(_getRef(i), ", ");

        if _size > 0 then
          ch.write(_getRef(_size));

        ch.write("]");
      } catch e {
        _leave();
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
      var result = (_size == 0);
      _leave();
      return result;
    }

    /*
      The current number of elements contained in this List.
    */
    inline proc size {
      _enter();
      var result = _size;
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
      var result: [1.._size] eltType;

      for i in 1.._size do
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

