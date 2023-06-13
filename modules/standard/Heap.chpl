/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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
  This module contains the implementation of a 'heap' type.

  A heap is a specialized tree-based data structure
  that supports extracting the maximal or the minimal element quickly,
  which can serve as a priority queue.

  * Both `push` and `pop` operations are O(lgN).
  * Querying the top element is O(1).
  * Initialization from an array is O(N).

  The heap accepts a :ref:`comparator <comparators>` to determine how
  elements are compared. The default comparator is `defaultComparator` and makes
  a max-heap. In this case, ``top`` will return the greatest element in the
  heap.

  If a ``reverseComparator`` is passed to ``init``,
  ``top`` will return the minimal element.

*/
@unstable("The 'Heap' module is unstable")
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
  // contention (IE, lots of tasks trying to call toArray on the heap
  // or any operation that is O(n)).
  //
  @chpldoc.nodoc
  type _lockType = ChapelLocks.chpl_LocalSpinlock;

  //
  // Use a wrapper class to let heap methods have a const ref receiver even
  // when `parSafe` is `true` and the heap lock is used.
  //
  @chpldoc.nodoc
  class _LockWrapper {
    var lock$ = new _lockType();

    inline proc lock() {
      lock$.lock();
    }

    inline proc unlock() {
      lock$.unlock();
    }
  }

  @chpldoc.nodoc
  proc _checkType(type eltType) {
    //NOTE: This is borrowed from List.chpl
    if isGenericType(eltType) {
      compilerWarning("creating a heap with element type " +
                      eltType:string);
      if isClassType(eltType) && !isGenericType(eltType:borrowed) {
        compilerWarning("which is a class type with generic management");
      }
      compilerError("heap element type cannot currently be generic");
      // In the future we might support it if the list is not default-inited
    }
  }

  record heap {

    /* The type of the elements contained in this heap. */
    type eltType;

    /* If `true`, this heap will perform parallel safe operations. */
    param parSafe = false;

    /*
      Comparator record that defines how the
      data is compared. The greatest element will be on the top.
    */
    var comparator: record;

    @chpldoc.nodoc
    var _lock$ = if parSafe then new _LockWrapper() else none;

    /*
      Use a list to store elements.
    */
    @chpldoc.nodoc
    var _data: list(eltType);

    /*
      Build the heap from elements that have been stored, from bottom to top
      in O(N)
    */
    @chpldoc.nodoc
    proc _commonInitFromIterable(iterable)
    lifetime this < iterable {
      _data = new list(eltType);
      for x in iterable do
        _data.pushBack(x);
      for i in 0 .. _data.size-1 by -1 {
        _heapify_down(i);
      }
    }

    /*
      Initializes an empty heap.

      :arg eltType: The type of the elements

      :arg comparator: The comparator to use

      :arg parSafe: If `true`, this heap will use parallel safe operations.
      :type parSafe: `param bool`
    */
    proc init(type eltType, param parSafe = false, comparator: record = defaultComparator) {
      _checkType(eltType);
      this.eltType = eltType;
      this.parSafe = parSafe;
      this.comparator = comparator;
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
      this.parSafe = this.type.parSafe;
      this.comparator = other.comparator;
      this.complete();
      _commonInitFromIterable(other._data);
    }

    /*
      Locks operations
    */
    @chpldoc.nodoc
    inline proc _enter() {
      if parSafe then
        _lock$.lock();
    }

    @chpldoc.nodoc
    inline proc _leave() {
      if parSafe then
        _lock$.unlock();
    }

    /*
      Return the size of the heap.

      :return: The size of the heap
      :rtype: int
    */
    proc size: int {
      _enter();
      var result = _data.size;
      _leave();
      return result;
    }

    /*
      Returns `true` if the heap is empty (has size == 0), `false` otherwise

      :return: `true` if this heap is empty.
      :rtype: `bool`
    */
    proc isEmpty(): bool {
      _enter();
      var result = _data.isEmpty();
      _leave();
      return result;
    }

    /*
      Return the top element in the heap.

      :return: The top element in the heap
      :rtype: `eltType`

    */
    proc top() {
      if (isOwnedClass(eltType)) {
        compilerError("top() method is not available on a 'heap'",
                      " with elements of an owned type, here: ",
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
    @chpldoc.nodoc
    proc _greater(x: eltType, y: eltType) {
      return chpl_compare(x, y, comparator) > 0;
    }

    /*
      Helper procedures to maintain the heap
    */
    @chpldoc.nodoc
    proc _heapify_up(in pos: int) {
      while (pos) {
        var parent = pos / 2;
        if (_greater(_data[pos],_data[parent])) {
          _data[parent] <=> _data[pos];
          pos = parent;
        }
        else break;
      }
    }

    @chpldoc.nodoc
    proc _heapify_down(in pos: int) {
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

    @chpldoc.nodoc
    proc _push(in element: eltType)
    lifetime this < element {
      _data.pushBack(element);
      _heapify_up(_data.size-1);
    }
    /*
      Push an element into the heap.

      :arg element: The element to push
      :type element: `eltType`
    */
    proc push(in element: eltType)
    lifetime this < element {
      _enter();
      _push(element);
      _leave();
    }

    /*
      Push elements of a list into the heap.

      :arg x: The list of elements to push
      :type x: `list(eltType)`
    */
    proc push(const ref x: list(eltType)) {
      _enter();
      for e in x do
        _push(x);
      _leave();
    }

    /*
      Push elements in an array into a heap.

      :arg x: The array of elements to push
      :type x: `[?d] eltType`
    */
    proc push(const ref x:[?d] eltType) {
      _enter();
      for e in x do
        _push(e);
      _leave();
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

      if _data.size != 1 then
        _data(0) <=> _data(_data.size-1);

      var ret = _data.popBack();
      _heapify_down(0);
      _leave();
      return ret;
    }

    /*
      Iterate over the elements of this heap in an arbitrary order.
    */
    iter these() ref {
      for e in _data {
        yield e;
      }
    }

    /*
      Iterate over the elements of this heap in order,
      while removing the yielded elements.
    */
    iter consume() {
      var h = this;
      while !h.isEmpty() {
        yield h.pop();
      }
    }

    /*
      Returns a new array containing a copy of each of the
      elements contained in this heap in arbitrary order.

      :return: A new array.
    */
    proc const toArray(): [] eltType {
      if !isCopyableType(eltType) then
        compilerError("toArray() method is not available on a 'heap'",
                      " with elements of a type that can't be copied, here: ",
                      eltType: string);
      _enter();
      var l = _data.toArray();
      _leave();
      return l;
    }

    /*
      Write the contents of this heap to a channel in arbitrary order.

      :arg ch: A channel to write to.
    */
    proc writeThis(ch: fileWriter) throws {
      _enter();
      ch.write(this._data);
      _leave();
    }
  }

  /*
    Copy elements to this heap from another heap.

    :arg lhs: The heap to assign to.
    :arg rhs: The heap to assign from.
  */
  operator heap.=(ref lhs: heap(?t, ?), ref rhs: heap(t, ?)) {
    lhs.comparator = rhs.comparator;
    lhs._data = rhs._data;
  }

  /*
    Create a heap from a list.

    :arg x: The list to initialize the heap from.
    :type x: `list(?t)`

    :arg comparator: The comparator to use

    :rtype: heap(t, comparator)
  */
  proc createHeap(const ref x: list(?t), param parSafe: bool = false, comparator = defaultComparator) {
    var h = new heap(t, parSafe, comparator);
    h._commonInitFromIterable(x);
    return h;
  }

  /*
    Create a heap from an array.

    :arg x: The array to initialize the heap from.
    :type x: `[?d] ?t`

    :arg comparator: The comparator to use

    :rtype: heap(t, comparator)
  */
  proc createHeap(const ref x: [?d] ?t, param parSafe: bool = false, comparator = defaultComparator) {
    var h = new heap(t, parSafe, comparator);
    h._commonInitFromIterable(x);
    return h;
  }
}
