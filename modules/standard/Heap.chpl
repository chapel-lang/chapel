/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
  This module contains the implementation of the heap type.

  A heap is a specialized tree-based data structure
  that supports extracting the maximal or the minimal element quickly,
  which can serve as a priority queue.

  * Both `push` and `pop` operations are O(lgN).
  * Querying the top element is O(1).
  * Initialization from an array is O(N).
*/
module Heap {
  import ChapelLocks;
  private use HaltWrappers;
  private use List;
  private use IO;

  public use Sort only defaultComparator, DefaultComparator,
                       reverseComparator, ReverseComparator;
  private use Sort;

  // The locker is borrowed from List.chpl
  // 
  // We can change the lock type later. Use a spinlock for now, even if it
  // is suboptimal in cases where long critical sections have high
  // contention (IE, lots of tasks trying to insert into the middle of this
  // list, or any operation that is O(n)).
  //
  pragma "no doc"
  type _lockType = ChapelLocks.chpl_LocalSpinlock;

  //
  // Use a wrapper class to let heap methods have a const ref receiver even
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

  proc _checkType(type eltType) {
    /*
    if isNilableClass(eltType) then
      compilerError("Cannot create heap with nilable type");
      */
  }
    
  record heap {

    /* The type of the elements contained in this heap. */
    type eltType;

    /*
      Comparator record that defines how the
      data is compared. The greatest element will be on the top.
    */
    var comparator: record;

    /* If `true`, this heap will perform parallel safe operations. */
    param parSafe = false;

    pragma "no doc"
    var _lock$ = if parSafe then new _LockWrapper() else none;

    /*
      Use a list to store elements.
    */
    pragma "no doc"
    var _data: list(eltType);

    /*
      Build the heap from elements that have been stored, from bottom to top
      in O(N)
    */
    pragma "no doc"
    proc _commonInitFromIterable(iterable)
    lifetime this < iterable {
      _data = new list(eltType);
      for x in iterable do
        _data.append(x);
      for i in 0 .. _data.size-1 by -1 {
        _heapify_down(i);
      }
    }

    /*
      Initializes an empty heap.

      :arg eltType: The type of the elements

      :arg comparator: `DefaultComparator` makes max-heap and `ReverseCompartor` makes a min-heap

      :arg parSafe: If `true`, this heap will use parallel safe operations.
      :type parSafe: `param bool`
    */
    proc init(type eltType, comparator: record = defaultComparator, param parSafe=false) {
      _checkType(eltType);
      this.eltType = eltType;
      this.comparator = comparator;
      this.parSafe = parSafe;
      this._data = new list(eltType);
    }

    /*
      Initializes a heap containing elements that are copy initialized from
      the elements contained in another heap.

      :arg other: The heap to initialize from.
    */
    proc init=(other: heap(this.type.eltType)) {
      _checkType(this.type.eltType);
      if !isCopyableType(this.type.eltType) then
        compilerError("Cannot copy heap with element type that cannot be copied");

      this.eltType = this.type.eltType;
      this.comparator = other.comparator;
      this.parSafe = this.type.parSafe;
      this.complete();
      _commonInitFromIterable(other._data);
    }

    /*
      Initializes a heap containing elements that are copy initialized from
      the elements contained in another list.

      :arg other: The list to initialize from.
    */
    proc init=(other: list(this.type.eltType, ?p)) {
      _checkType(this.type.eltType);
      if !isCopyableType(this.type.eltType) then
        compilerError("Cannot copy list with element type that cannot be copied");

      this.eltType = this.type.eltType;
      this.comparator = new this.type.comparator();
      this.parSafe = this.type.parSafe;
      this.complete();
      _commonInitFromIterable(other);
    }

    /*
      Initializes a heap containing elements that are copy initialized from
      the elements contained in an array.

      :arg other: The array to initialize from.
    */
    proc init=(other: [?d] this.type.eltType) {
      _checkType(this.type.eltType);
      if !isCopyableType(this.type.eltType) then
        compilerError("Cannot copy heap from array with element type that cannot be copied");

      this.eltType = this.type.eltType;
      this.comparator = new this.type.comparator();
      this.parSafe = this.type.parSafe;
      this.complete();
      _commonInitFromIterable(other);
    }

    /*
      Locks operations
    */
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

    /*
      Return the size of the heap.

      :return: The size of the heap
      :rtype: int
    */
    proc size:int {
      _enter();
      var result = _data.size;
      _leave();
      return result;
    }

    /*
      Returns `true` if this heap contains zero elements.

      :return: `true` if this heap is empty.
      :rtype: `bool`
    */
    proc isEmpty():bool {
      _enter();
      var result = _data.isEmpty();
      _leave();
      return result;
    }

    /*
      Return the maximal element in the heap.

      :return: The maximal element in the heap
      :rtype: `eltType`

      .. note::
        *Maximal* is defined by ``comparator``. If a ``reverseComparator`` is passed to ``init``,
        the heap will return the minimal element.

    */
    proc top() {
      if (isOwnedClass(eltType)) {
        compilerError("top() method is not avaliable on a 'heap'",
                      " with elements of a owned type, here: ",
                      eltType: string);
      }
      _enter();
      if (boundsChecking && isEmpty()) {
        boundsCheckHalt("Called \"heap.top\" on an empty heap.");
      }
      var result = _data[0];
      _leave();
      return result;
    }

    /*
      Wrapper of comparing elements
    */
    pragma "no doc"
    proc _greater(x:eltType, y:eltType) {
      return chpl_compare(x, y, comparator) > 0;
    }

    /*
      helper procs to maintain the heap
    */
    pragma "no doc"
    proc _heapify_up(in pos:int) {
      while (pos) {
        var parent = pos / 2;
        if (_greater(_data[pos],_data[parent])) {
          _data[parent] <=> _data[pos];
          pos = parent;
        }
        else break;
      }
    }

    pragma "no doc"
    proc _heapify_down(in pos:int) {
      while (pos < _data.size) {
        // find the child node with greater value
        var greaterChild = pos*2;
        if (greaterChild >= _data.size) then break; // reach leaf node, break
        if (greaterChild + 1 < _data.size) {
          // if the right child node exists
          if (_greater(_data[greaterChild+1],_data[greaterChild])) {
            // if the right child is greater, then update the greaterChild
            greaterChild += 1;
          }
        }
        // if the greaterChild's value is greater than current node, then swap and continue
        if (_greater(_data[greaterChild],_data[pos])) {
          _data[greaterChild] <=> _data[pos];
          pos = greaterChild;
        }
        else break;
      }
    }

    /*
      Push an element into the heap

      :arg element: The element that will be pushed
      :type element: `eltType`
    */
    proc push(in element:eltType)
    lifetime this < element {
      _enter();
      _data.append(element);
      _heapify_up(_data.size-1);
      _leave();
    }

    /*
      Push elements of a list into the heap

      :arg x: The list of which elements is to push.
      :type x: `list(eltType)`
    */
    proc push(ref x: list(eltType)) {
      for e in x do
        push(e);
    }

    /*
      Push elements in an array into a heap.

      :arg x: The array of which elements is to push.
      :type x: `[?d] eltType`
    */
    proc push(ref x:[?d] eltType) {
      for e in x do
        push(e);
    }

    /*
      Pop an element and return it.

      :return: the top element
      :rtype: eltType
    */
    proc pop(): eltType {
      _enter();
      if (boundsChecking && isEmpty()) {
        boundsCheckHalt("Called \"heap.pop\" on an empty heap.");
      }
      _data(0) <=> _data(_data.size-1);
      var ret = _data.pop();
      _heapify_down(0);
      _leave();
      return ret;
    }

    /*
      Iterate over the elements of this heap

        .. note::
          Elements in the heap is not sorted

    */
    iter these() ref {
      for e in _data {
        yield e;
      }
    }

    proc const toArray(): [] eltType {
      if !isCopyableType(eltType) then
        compilerError("toArray() method is not avaliable on a 'heap'",
                      " with elements of a type that can't be copied, here: ",
                      eltType: string);
      var l: [0..#size] eltType;
      for i in 0..#size {
        l[i] = _data[i];
      }
      return l;
    }

    /*
      Returns a new DefaultRectangular array containing the
      elements contained in this heap. The heap will be empty.

      :return: A new DefaultRectangular array.
    */
    proc consume(): [] eltType {
      var l: [0..#size] eltType;
      for i in 0..#size {
        l[i] = pop();
      }
      return l;
    }

    /*
      Write the contents of this list to a channel.

      :arg ch: A channel to write to.
    */
    proc readWriteThis(ch: channel) throws {
      _enter();
      ch <~> this._data;
      _leave();
    }
  }
  /*
    Make a heap from a list.

    :arg x: The list to initialize the heap from.
    :type x: `list(?t)`

    :arg comparator: The comparator type

    :rtype: heap(t, comparator)
  */
  proc createHeap(x:list(?t), comparator = defaultComparator) {
    var h = new heap(t, comparator);
    h._commonInitFromIterable(x);
    return h;
  }
  /*
    Make a heap from a array.

    :arg x: The array to initialize the heap from.
    :type x: `[?d] ?t`

    :arg comparator: The comparator type

    :rtype: heap(t, comparator)
  */
  proc createHeap(x:[?d] ?t, comparator = defaultComparator) {
    var h = new heap(t, comparator);
    h._commonInitFromIterable(x);
    return h;
  }

  /*
    Pop elements into a list.

    :arg h: The heap to pop
    :type h: `ref heap(t)`

    :return: A list containing all elements in the heap
    :rtype: `list(t)`
  */
  //FIXME: No need for this as we have toArray and consume
  /*
  proc popHeap(ref h:heap(?t)) {
    var l = new list(t);
    while (!h.isEmpty()) {
      l.append(h.pop());
    }
    return l;
  }
  */
}
