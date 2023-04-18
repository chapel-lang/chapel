/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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
  This module contains the implementation of the 'unrolledLinkedList' type.

  An unrolled linked list is a linked list of small arrays, all of the same size
  where each is so small that the insertion or deletion is fast and quick, but
  large enough to fill the cache line. The list tends to keep each node half full.

  Given a list with size N and nodeCapacity M, indexing is O(N/M). And insertion or
  deletion at a given place is O(N/M + M), which contains a indexing operation.
  Appending operation, which doesn't need to index, is O(M).
*/
module UnrolledLinkedList {
  import ChapelLocks;
  private use HaltWrappers;
  private use Sort;
  private use IO;
  private use List;

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

  pragma "no doc"
  type _lockType = ChapelLocks.chpl_LocalSpinlock;

  //
  // Use a wrapper class to let unrolledLinkedList methods have a const ref receiver even
  // when `parSafe` is `true` and the unrolledLinkedList lock is used.
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

  /* Check that element type is supported by unrolledLinkedList */
  pragma "no doc"
  proc _checkType(type eltType) {
    if !isDefaultInitializable(eltType) {
      compilerError("unrolledLinkedList element type must be default-initializable");
    }
  }

  pragma "no doc"
  proc _checkNodeCapacity(cap: int) {
    if cap < 2 {
      halt("unrolledLinkedList nodeCapacity can't be less than 2");
    }
    if cap % 2 == 1 {
      halt("unrolledLinkedList nodeCapacity can't be odd");
    }
  }

  pragma "no doc"
  class _linkedNode {
    type eltType;
    const capacity: int = 32;
    var size: int = 0;
    var data: [0..#capacity] eltType;
    var next: unmanaged _linkedNode(eltType)? = nil;

    proc append(in x: eltType) lifetime this < x {
      if _sanityChecks then
        assert(size != capacity);
      data[size] = x;
      size += 1;
    }

    proc append(ref x: eltType) where isOwnedClass(x)
    lifetime this < x {
      if _sanityChecks then
        assert(size != capacity);
      data[size] = x;
      size += 1;
    }
  };

  record unrolledLinkedList {

    /* The type of the elements contained in this unrolledLinkedList. */
    type eltType;

    /* If `true`, this unrolledLinkedList will perform parallel safe operations. */
    param parSafe = false;

    /* The capacity of one linked node in the unrolledLinkedList */
    var nodeCapacity: int = 32;

    /*
      The number of nodes in the unrolledLinkedList
    */
    pragma "no doc"
    var _size = 0;

    pragma "no doc"
    var _lock$ = if parSafe then new _LockWrapper() else none;


    pragma "no doc"
    var _head: unmanaged _linkedNode(eltType)? = nil;

    pragma "no doc"
    var _tail: unmanaged _linkedNode(eltType)? = nil;

    /*
      Delete all nodes in the list
    */
    pragma "no doc"
    proc _destroy() {
      var cur = _head;
      while cur != nil {
        var next = cur!.next;
        delete cur;
        cur = next;
      }
    }

    pragma "no doc"
    proc deinit() {
      _destroy();
    }

    /*
      Initializes an empty unrolledLinkedList.

      :arg eltType: The type of the elements of this unrolledLinkedList.

      :arg parSafe: If `true`, this unrolledLinkedList will use parallel safe operations.
      :type parSafe: `param bool`
    */
    proc init(type eltType, param parSafe=false, nodeCapacity: int = 32) {
      _checkType(eltType);
      _checkNodeCapacity(nodeCapacity);

      this.eltType = eltType;
      this.parSafe = parSafe;
      this.nodeCapacity = nodeCapacity;
      this.complete();
    }

    /*
      Initializes an unrolledLinkedList containing elements that are copy initialized from
      the elements contained in a list.

      Used in new expressions.

      :arg other: The list to initialize from.

      :arg parSafe: If `true`, this unrolledLinkedList will use parallel safe operations.
      :type parSafe: `param bool`
    */
    proc init(other: list(?t), param parSafe=false, nodeCapacity: int = 32) {
      _checkType(t);
      _checkNodeCapacity(nodeCapacity);
      if !isCopyableType(this.type.eltType) then
        compilerError("Cannot construct unrolledLinkedList from list with "
        + "element type that cannot be copied");
      this.eltType = t;
      this.parSafe = parSafe;
      this.nodeCapacity = nodeCapacity;
      this.complete();
      _commonInitFromIterable(other);
    }

    /*
      Initializes an unrolledLinkedList containing elements that are copy
      initialized from the elements contained in an array.

      Used in new expressions.

      :arg other: The array to initialize from.

      :arg parSafe: If `true`, this unrolledLinkedList will use parallel safe operations.
      :type parSafe: `param bool`
    */
    proc init(other: [?d] ?t, param parSafe=false, nodeCapacity: int = 32) {
      _checkType(t);
      _checkNodeCapacity(nodeCapacity);
      if !isCopyableType(t) then
        compilerError("Cannot construct unrolledLinkedList from array with element " +
                      "type that cannot be copied");

      this.eltType = t;
      this.parSafe = parSafe;
      this.nodeCapacity = nodeCapacity;
      this.complete();
      _commonInitFromIterable(other);
    }

    /*
      Initializes an unrolledLinkedList containing elements that are copy
      initialized from the elements contained in another unrolledLinkedList.

      :arg other: The list to initialize from.
    */
    proc init=(other: unrolledLinkedList(this.type.eltType, ?p)) {
      _checkType(this.type.eltType);
      _checkNodeCapacity(other.nodeCapacity);
      if !isCopyableType(this.type.eltType) then
        compilerError("Cannot construct unrolledLinkedList from list with " +
        "element type that cannot be copied");

      this.eltType = this.type.eltType;
      this.parSafe = this.type.parSafe;
      this.nodeCapacity = other.nodeCapacity;
      this.complete();
      _commonInitFromIterable(other);
    }

    pragma "no doc"
    proc _commonInitFromIterable(iterable) {
      for x in iterable do
        append(x);
    }

    // A helper function for getting a reference to an unrolledLinkedList element.
    //
    pragma "no doc"
    inline proc const ref _getRef(idx: int) ref {
      if _sanityChecks {
        assert(idx >= 0 && idx < _size);
        assert(_head != nil);
      }
      var cur = _head, pos = 0;
      while cur != nil {
        if idx < pos + cur!.size {
          ref result = cur!.data[idx - pos];
          return result;
        }
        pos += cur!.size;
        cur = cur!.next;
      }
      halt("unrolledLinkedList out of range");
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

    /*
     Split a node from the given node p
     by moving half of the content of p into the new one
    */
    pragma "no doc"
    proc _split(p: unmanaged _linkedNode(eltType)) {
      if _sanityChecks then
        assert(p.size == nodeCapacity);
      var node = new unmanaged _linkedNode(eltType, nodeCapacity);
      var start = p.size/2;
      for i in start..p.size-1 {
        node.append(p.data[i]);
        if isSharedClass(this.eltType) then
          p.data[i] = nil;
      }
      p.size -= node.size;
      if _sanityChecks then
        assert(p.size > 0);

      // link the new node to the old node
      node.next = p.next;
      p.next = node;

      if p == _tail then _tail = p.next;

      return node;
    }

    /*
      If it's possible to merge this node and the next node, then do it
      If it's not possible to merge nodes, then fill the previous node to half
      Return whether it's merged
    */
    pragma "no doc"
    proc _merge(p: unmanaged _linkedNode(eltType)): bool {
      var result = false;

      // Nothing to do
      if p.next == nil then return result;

      var next = p.next!;

      if p.size < nodeCapacity/2 {
        // Fill p to half
        var toFill = nodeCapacity/2 - p.size;
        if next.size - toFill < nodeCapacity/2 {
          toFill = next.size;
          result = true;
        }

        for i in 0..#toFill {
          p.data[p.size] = next.data[i];
          p.size += 1;
        }

        // Shift or delete the next node
        next.size -= toFill;
        if next.size == 0 {
          p.next = next.next;
          if _tail == next {
            _tail = p;
          }
          delete next;
        }
        else {
          for i in 0..#next.size {
            next.data[i] = next.data[i + toFill];
          }
        }
      }

      return result;
    }

    /*
      Make sure there's enough space in _tail for one element
    */
    pragma "no doc"
    proc _spareSpaceInTail() {
      if _tail == nil {
        _tail = new unmanaged _linkedNode(eltType, nodeCapacity);
        _head = _tail;
      }

      if _sanityChecks then
        assert(_tail != nil);

      if _tail!.size == nodeCapacity {
        // the node is full, create new node
        // This could change the value of _tail
        _split(_tail!);
      }
    }

    pragma "no doc"
    proc _append(x: eltType)
    lifetime this < x {
      _size += 1;
      _spareSpaceInTail();
      _tail!.append(x);
    }

    pragma "no doc"
    proc _append(ref x: eltType) where isOwnedClass(x)
    lifetime this < x {
      _size += 1;
      _spareSpaceInTail();
      _tail!.append(x);
    }

    /*
      Add an element to the end of this unrolledLinkedList.

      :arg x: An element to append.
      :type x: `eltType`

      :return: List index where element was inserted.
      :rtype: `int`
    */
    proc ref append(x: eltType)
    lifetime this < x {
      _enter();
      _append(x);
      _leave();
    }

    pragma "no doc"
    proc append(ref x: eltType) where isOwnedClass(x)
    lifetime this < x {
      _enter();
      _append(x);
      _leave();
    }

    pragma "no doc"
    inline proc ref _appendGeneric(collection) {
      var startSize: int;
      var endSize: int;
      on this {
        startSize = _size;
        // TODO: Maybe we can allocate space at one time and append.
        for item in collection {
          _append(item);
        }
        endSize = _size;
      }

      return startSize..(endSize-1);
    }

    /*
      Append a copy of each element contained in a list to the end of this
      unrolledLinkedList.

      :arg other: A list containing elements of the same type as those
        contained in this list.
      :type other: `list(eltType)`

      :return: List indices where elements were inserted.
      :rtype: `range`
    */
    proc ref append(other: list(eltType, ?p)) lifetime this < other {
      var ret: range;
      on this {
        _enter();
        ret = _appendGeneric(other);
        _leave();
      }
      return ret;
    }

    /*
      Append a copy of each element contained in another unrolledLinkedList to
      the end of this unrolledLinkedList.

      :arg other: an unrolledLinkedList containing elements of the same type as
        those contained in this unrolledLinkedList.
      :type other: `unrolledLinkedList(eltType)`

      :return: List indices where elements were inserted.
      :rtype: `range`
    */
    proc ref append(other: unrolledLinkedList(eltType, ?p)) lifetime this < other {
      var ret: range;
      on this {
        _enter();
        ret = _appendGeneric(other);
        _leave();
      }
      return ret;
    }

    /*
      Append a copy of each element contained in an array to the end of this
      list.

      :arg other: An array containing elements of the same type as those
        contained in this unrolledLinkedList.
      :type other: `[?d] eltType`

      :return: List indices where elements were inserted.
      :rtype: `range`
    */
    proc ref append(other: [?d] eltType) lifetime this < other {
      var ret: range;
      on this {
        _enter();
        ret = _appendGeneric(other);
        _leave();
      }
      return ret;
    }

    /*
      Append a copy of each element yielded by a range to the end of this
      unrolledLinkedList.

      .. note::

        Attempting to initialize an unrolledLinkedList from an unbounded range
        will trigger a compiler error.

      :arg other: The range to initialize from.
      :type other: `range(eltType)`

      :return: List indices where elements were inserted.
      :rtype: `range`
    */
    proc ref append(other: range(eltType, ?b, ?d)) lifetime this < other {
      if !isBoundedRange(other) {
        param e = this.type:string;
        param f = other.type:string;
        param msg = "Cannot extend " + e + " with unbounded " + f;
        compilerError(msg);
      }

      var ret: range;
      on this {
        _enter();
        ret = _appendGeneric(other);
        _leave();
      }
      return ret;
    }

    /*
      Returns `true` if this unrolledLinkedList contains an element equal to the
      value of `x`, and `false` otherwise.

      :arg x: An element to search for.
      :type x: `eltType`

      :return: `true` if this unrolledLinkedList contains `x`.
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
      Returns a reference to the first item in this unrolledLinkedList.

      .. warning::

        Calling this method on an empty unrolledLinkedList will cause the
        currently running program to halt. If the `--fast` flag is used, no
        safety checks will be performed.

      :return: A reference to the first item in this unrolledLinkedList.
      :rtype: `ref eltType`
    */
    proc ref first() ref {
      _enter();

      if boundsChecking && _size == 0 {
        _leave();
        boundsCheckHalt("Called \"unrolledLinkedList.first\" on an empty unrolledLinkedList.");
      }

      ref result = _getRef(0);
      _leave();

      return result;
    }

    /*
      Returns a reference to the last item in this unrolledLinkedList.

      .. warning::

        Calling this method on an empty unrolledLinkedList will cause the
        currently running program to halt. If the `--fast` flag is used, no
        safety checks will be performed.

      :return: A reference to the last item in this unrolledLinkedList.
      :rtype: `ref eltType`
    */
    proc ref last() ref {
      _enter();

      if boundsChecking && _size == 0 {
        _leave();
        boundsCheckHalt("Called \"unrolledLinkedList.last\" on an empty unrolledLinkedList.");
      }

      ref result = _tail!.data[_tail!.size-1];
      _leave();

      return result;
    }

    @deprecated(notes="unrolledLinkedList.extend is deprecated, please use unrolledLinkedList.append")
    proc ref extend(other: list(eltType, ?p)) lifetime this < other {
      append(other);
    }

    @deprecated(notes="unrolledLinkedList.extend is deprecated, please use unrolledLinkedList.append")
    proc ref extend(other: unrolledLinkedList(eltType, ?p)) lifetime this < other {
      append(other);
    }

    @deprecated(notes="unrolledLinkedList.extend is deprecated, please use unrolledLinkedList.append")
    proc ref extend(other: [?d] eltType) lifetime this < other {
      append(other);
    }

    @deprecated(notes="unrolledLinkedList.extend is deprecated, please use unrolledLinkedList.append")
    proc ref extend(other: range(eltType, ?b, ?d)) lifetime this < other {
      append(other);
    }

    pragma "no doc"
    proc ref _insert(idx: int, in x: eltType)
         lifetime this < x {

      _size += 1;

      var pos = 0;
      var cur = _head;
      while cur != nil {
        if idx >= pos + cur!.size {
          pos += cur!.size;
          cur = cur!.next;
        }
        else {
          // The position in this node
          var i = idx - pos;

          // The node to insert into
          var dest = cur;

          if cur!.size == nodeCapacity {
            // The current node is full
            var newNode = _split(cur!);
            if i >= cur!.size {
              dest = newNode;
              i -= cur!.size;
            }
          }

          // Shift elements by one step
          dest!.size += 1;
          for j in i+1..dest!.size-1 by -1 {
            dest!.data[j] = dest!.data[j-1];
          }

          dest!.data[i] = x;

          break;
        }
      }
    }

    /*
      Insert an element at a given position in this unrolledLinkedList, shifting
      all elements currently at and following that index one to the right. The
      call ``a.insert(0, x)`` inserts an element at the front of the
      unrolledLinkedList `a`, and ``a.insert((a.size), x)`` is equivalent to
      ``a.append(x)``.

      If the insertion is successful, this method returns `true`. If the given
      index is out of bounds, this method does nothing and returns `false`.

      .. warning::

        Inserting an element into this unrolledLinkedList may invalidate
        existing references to the elements contained in this unrolledLinkedList.

      :arg idx: The index into this unrolledLinkedList at which to insert.
      :type idx: `int`

      :arg x: The element to insert.
      :type x: `eltType`

      :return: `true` if `x` was inserted, `false` otherwise.
      :rtype: `bool`
    */
    proc ref insert(idx: int, x: eltType): bool
         lifetime this < x {
      var result = false;

      on this {
        _enter();

        // Handle special case of `a.insert((a.size), x)` here.
        if idx == _size {
          _append(x);
          result = true;
        } else if _withinBounds(idx) {
          _insert(idx, x);
          result = true;
        }

        _leave();
      }

      return result;
    }

    pragma "no doc"
    proc ref _insertGenericKnownSize(idx: int, items, size: int): bool {
      //TODO: Maybe some optimization here. This O(N^2)
      var result = false;

      if _sanityChecks then
        assert(size >= 0);

      if size == 0 then
        return true;


      on this {
        if idx == _size {
          for x in items {
            _append(x);
          }
          result = true;
        }
        else if _withinBounds(idx) {
          var count = 0;
          for x in items {
            _insert(idx + count, x);
            count += 1;
          }
          result = true;
        }
      }

      return result;
    }

    /*
      Insert elements of an array `arr` into this unrolledLinkedList at index
      `idx`, shifting all elements at and following the index `arr.size`
      positions to the right.

      If the insertion is successful, this method returns `true`. If the given
      index is out of bounds, this method does nothing and returns `false`.

      .. warning::

        Inserting elements into this unrolledLinkedList may invalidate existing
        references to the elements contained in this unrolledLinkedList.

      :arg idx: The index into this unrolledLinkedList at which to insert.
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
      Insert elements of a list `lst` into this unrolledLinkedList at index
      `idx`, shifting all elements at and following the index `lst.size`
      positions to the right.

      If the insertion is successful, this method returns `true`. If the given
      index is out of bounds, this method does nothing and returns `false`.

      .. warning::

        Inserting elements into this unrolledLinkedList may invalidate existing
        references to the elements contained in this unrolledLinkedList.

      :arg idx: The index into this unrolledLinkedList at which to insert.
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
      Remove the first `count` elements from this unrolledLinkedList with values
      equal to `x`, shifting all elements following the removed item left.

      If the count of elements to remove is less than or equal to zero, then
      all elements from this unrolledLinkedList equal to the value of `x` will
      be removed.

      .. warning::

        Removing elements from this unrolledLinkedList may invalidate existing
        references to the elements contained in this unrolledLinkedList.

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
        _enter(); defer _leave();

        var removed = 0;

        // For each node
        var cur = _head;
        while cur != nil && (removed < count || count == 0) {
          // The following is similar to the original List's remove

          // Run through the leading non-x prefix
          var dst_i = 0;
          while dst_i < cur!.size {
            if cur!.data[dst_i] == x {
              removed += 1;
              break;
            }
            dst_i += 1;
          }

          // No hit in this node, continue
          if dst_i == cur!.size {
            cur = cur!.next;
            continue;
          }

          // Now dst_i should be the index of the first `x`
          var src_i = dst_i + 1;
          while src_i < cur!.size {
            ref src = cur!.data[src_i];
            if (count == 0 || removed < count) && src == x {
              src_i += 1;
              removed += 1;
              continue;
            }
            ref dst = cur!.data[dst_i];
            if src_i != dst_i then dst = src;
            src_i += 1;
            dst_i += 1;
          }

          cur!.size = dst_i;

          cur = cur!.next;
        }

        // Maybe merge nodes
        cur = _head;
        while cur != nil {
          var result = _merge(cur!);
          if result == false then cur = cur!.next;
        }

        result = removed;
        _size -= removed;
      }

      return result;
    }

    pragma "no doc"
    proc ref _popAtIndex(idx: int): eltType {
      _size -= 1;

      var cur = _head;
      var pos = 0;
      while cur != nil {
        if idx < pos + cur!.size {
          var result = cur!.data[idx-pos];

          // Shift elements
          cur!.size -= 1;
          for i in idx-pos..<cur!.size {
            cur!.data[i] = cur!.data[i+1];
          }

          _merge(cur!);

          return result;
        }
        else {
          pos += cur!.size;
          cur = cur!.next;
        }
      }

      // shouldn't reach here
      boundsCheckHalt("out of bounds on unrolledLinkedList");
    }

    /*
      Remove the element at the end of this unrolledLinkedList and return it.

      .. warning::

        Popping an element from this unrolledLinkedList will invalidate any
        reference to the element taken while it was contained in this
        unrolledLinkedList.

      .. warning::

        Calling this method on an empty unrolledLinkedList will cause the
        currently running program to halt. If the `--fast` flag is used, no
        safety checks will be performed.

      :return: The element popped.
      :rtype: `eltType`
    */
    proc ref pop(): eltType {
      _enter(); defer _leave();

      if boundsChecking && _size <= 0 {
        boundsCheckHalt("Called \"unrolledLinkedList.pop\" on an empty unrolledLinkedList.");
      }

      var result = _popAtIndex(_size-1);
      return result;
    }

    /*
      Remove the element at the index `idx` from this unrolledLinkedList and
      return it.

      .. warning::

        Popping an element from this unrolledLinkedList will invalidate any
        reference to the element taken while it was contained in this
        unrolledLinkedList.

      .. warning::

        Calling this method on an empty unrolledLinkedList or with values of
        `idx` that are out of bounds will cause the currently running program
        to halt. If the `--fast` flag is used, no safety checks will be
        performed.

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
      Clear the contents of this unrolledLinkedList.

      .. warning::

        Clearing the contents of this unrolledLinkedList will invalidate all
        existing references to the elements contained in this unrolledLinkedList.
    */
    proc ref clear() {
      on this {
        _enter();

        _destroy();
        _head = nil;
        _tail = nil;
        _size = 0;

        if _sanityChecks {
          assert(_size == 0);
          assert(_head == nil);
        }

        _leave();
      }
    }

    /*
      Return a zero-based index into this unrolledLinkedList of the first item
      whose value is equal to `x`. If no such element can be found this method
      returns the value `-1`.

      .. warning::

        Calling this method on an empty unrolledLinkedList or with values of
        `start` or `end` that are out of bounds will cause the currently running
        program to halt. If the `--fast` flag is used, no safety checks will be
        performed.

      :arg x: An element to search for.
      :type x: `eltType`

      :arg start: The start index to start searching from.
      :type start: `int`

      :arg end: The end index to stop searching at. A value less than
                `0` will search the entire unrolledLinkedList.
      :type end: `int`

      :return: The index of the element to search for, or `-1` on error.
      :rtype: `int`
    */
    proc const indexOf(x: eltType, start: int=0, end: int=-1): int {
      if boundsChecking {
        const msg = " index for \"unrolledLinkedList.indexOf\" out of bounds: ";

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

        var i = 0;
        for item in this {
          if i > stop then break;
          if i >= start {
            if x == item {
              result = i;
              break;
            }
          }
          i += 1;
        }

        _leave();
      }

      return result;
    }

    /*
      Return the number of times a given element is found in this unrolledLinkedList.

      :arg x: An element to count.
      :type x: `eltType`

      :return: The number of times a given element is found in this unrolledLinkedList.
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
      Index this unrolledLinkedList via subscript. Returns a reference to the
      element at a given index in this unrolledLinkedList.

      :arg i: The index of the element to access.

      .. warning::

        Use of the `this` method with an out of bounds index (while bounds
        checking is on) will cause the currently running program to halt.

      :return: An element from this unrolledLinkedList.
    */
    proc ref this(i: int) ref {
      if boundsChecking && !_withinBounds(i) {
        const msg = "Invalid unrolledLinkedList index: " + i:string;
        boundsCheckHalt(msg);
      }

      ref result = _getRef(i);
      return result;
    }

    pragma "no doc"
    proc const ref this(i: int) const ref {
      if boundsChecking && !_withinBounds(i) {
        const msg = "Invalid unrolledLinkedList index: " + i:string;
        halt(msg);
      }

      const ref result = _getRef(i);
      return result;
    }

    /*
      Iterate over the elements of this unrolledLinkedList.

      :yields: A reference to one of the elements contained in this unrolledLinkedList.
    */
    iter these() ref {
      var cur = _head;
      while cur != nil {
        foreach i in 0..#cur!.size {
          yield cur!.data[i];
        }
        cur = cur!.next;
      }
    }

    /*
      Write the contents of this unrolledLinkedList to a channel.

      :arg ch: A channel to write to.
    */
    proc writeThis(ch: fileWriter) throws {
      _enter();

      ch.write("[");

      var first = true;

      for x in this {
        if !first then ch.write(", ");
        else first = false;
        ch.write(x);
      }

      ch.write("]");

      _leave();
    }

    /*
      Returns `true` if this unrolledLinkedList contains zero elements.

      :return: `true` if this unrolledLinkedList is empty.
      :rtype: `bool`
    */
    proc const isEmpty(): bool {
      return size == 0;
    }

    /*
      The current number of elements contained in this unrolledLinkedList. Returns
      in O(1).
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
      Returns the unrolledLinkedList's legal indices as the range ``0..<this.size``.

      :return: ``0..<this.size``
      :rtype: `range`
    */
    proc indices {
      return 0..<this.size;
    }

    /*
      Returns a new DefaultRectangular array containing a copy of
      each of the elements contained in this unrolledLinkedList.

      :return: A new DefaultRectangular array.
    */
    proc const toArray(): [] eltType {
      if isNonNilableClass(eltType) && isOwnedClass(eltType) then
        compilerError("toArray() method is not available on a 'unrolledLinkedList'",
                      " with elements of a non-nilable owned type, here: ",
                      eltType:string);


      var result: [0..#_size] eltType;

      on this {
        _enter();

        for (a,x) in zip(result, this) do
          a = x;

        _leave();
      }

      return result;
    }

  } // End record "unrolledLinkedList".

  /*
    Clear the contents of this unrolledLinkedList, then extend this now-empty
    unrolledLinkedList with the elements contained in another unrolledLinkedList.

    .. warning::

      This will invalidate any references to elements previously contained in
      `lhs`.

  */
  operator unrolledLinkedList.=(ref lhs: unrolledLinkedList(?t, ?),
                                rhs: unrolledLinkedList(t, ?)) {
    lhs.clear();
    lhs.append(rhs);
  }

  /*
    Returns `true` if the contents of two unrolledLinkedLists are the same.

    :return: `true` if the contents of two unrolledLinkedLists are equal.
    :rtype: `bool`
  */
  operator unrolledLinkedList.==(a: unrolledLinkedList(?t, ?),
                                 b: unrolledLinkedList(t, ?)): bool {
    if a.size != b.size then
      return false;

    for (x, y) in zip(a, b) do
      if x != y then return false;

    return true;
  }

  /*
    Return `true` if the contents of two unrolledLinkedLists are not the same.

    :return: `true` if the contents of two unrolledLinkedLists are not equal.
    :rtype: `bool`
  */
  operator unrolledLinkedList.!=(a: unrolledLinkedList(?t, ?),
                                 b: unrolledLinkedList(t, ?)): bool {
    return !(a == b);
  }

} // End module "UnrolledLinkedList".
