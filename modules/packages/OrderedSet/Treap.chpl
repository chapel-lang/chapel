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
  This module contains an implementation of Treap,
  which provides the functionality of OrderedSet.
  Treap supports insertion, deletion, query in O(lgN).

  .. note::
    Generally, users don't have to directly use this module. The methods of a treap 
    are available for an orderedSet. This page is for reference.
*/
pragma "no doc"
module Treap {
  import ChapelLocks;
  private use HaltWrappers;
  private use Sort;
  private use Random;
  private use IO;
  private use Reflection;
  private use OrderedSet only orderedSet;

  pragma "no doc"
  private param _sanityChecks = false;

  // The locker is borrowed from List.chpl
  // 
  // We can change the lock type later. Use a spinlock for now.
  //
  pragma "no doc"
  type _lockType = ChapelLocks.chpl_LocalSpinlock;

  //
  // Use a wrapper class to let heap methods have a const ref receiver even
  // when `parSafe` is `true` and the orderedSet lock is used.
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

  pragma "no doc"
  var _treapRandomStream = new RandomStream(int);

  /*
    Helper procedure to get one random int
  */
  pragma "no doc"
  proc _random(): int {
    return _treapRandomStream.getNext();
  }

  pragma "no doc"
  proc _checkType(type t) {
    if isGenericType(t) {
      compilerError('creating an treap with element type ' + t:string
                      + '. treap element type cannot currently be generic', 2);
    }
    if isOwnedClass(t) then
        compilerError('treap does not support owned class type: ' + t:string, 2);
  }

  pragma "no doc"
  class _treapNode {
    type eltType;
    var element: eltType;
    /*
      priority is a randomly generated number.

      A node should have greater priority than its children.
      In this way, a treap can take advantage of randomness and avoid bad cases
      like regressing to a linked list when all nodes have at most one child.

      size is the size of the subtree rooted on the node.
    */
    var priority, size: int;
    var parent: unmanaged _treapNode(eltType)?;
    var children: (unmanaged _treapNode(eltType)?, unmanaged _treapNode(eltType)?);

    proc update() {
      size = 1;
      for child in children {
        if child != nil {
          size += child!.size;
        }
      }
    }

    proc deinit() {
      for child in children {
        if child != nil {
          // Delete the tree recursively by nodes' deinit
          delete child;
        }
      }
    }

    proc sanityChecks() {
      if !_sanityChecks then return;
      for child in children {
        if child != nil {
          assert(child!.parent == this);
        }
      }
    }
  }

  record treap {
    /* The type of the elements contained in this orderedSet.*/
    type eltType;

    /* If `true`, this orderedSet will perform parallel safe operations. */
    param parSafe = false;

    /* The comparator to use for comparing elements */
    var comparator: record = defaultComparator;

    pragma "no doc"
    type nodeType = unmanaged _treapNode(eltType)?;
    pragma "no doc"
    var _root: nodeType = nil;

    pragma "no doc"
    var _lock$ = if parSafe then new _LockWrapper() else none;

    pragma "no doc"
    inline proc _enter() {
      if parSafe then
        on this {
          _lock$.lock();
        }
    }

    pragma "no doc"
    inline proc _leave() {
      if parSafe then
        on this {
          _lock$.unlock();
        }
    }

    pragma "no doc"
    proc deinit() {
      if _root != nil {
        delete _root;
      }
    }

    /*
      The current number of elements contained in this orderedSet.
    */
    inline proc const size {
      var result = 0;

      on this {
        _enter();
        if _root != nil then
          result = _root!.size;
        _leave();
      }

      return result;
    }

    /*
      Initializes an empty orderedSet containing elements of the given type.

      :arg eltType: The type of the elements of this orderedSet.
      :arg parSafe: If `true`, this orderedSet will use parallel safe operations.
      :arg comparator: The comparator used to compare elements.
    */
    proc init(type eltType, param parSafe = false, comparator: record = defaultComparator) {
      _checkType(eltType);
      this.eltType = eltType;
      this.parSafe = parSafe;
      this.comparator = comparator;
    }

    /*
      Initialize this orderedSet with a unique copy of each element contained in
      `iterable`. If an element from `iterable` is already contained in this
      orderedSet, it will not be added again. The formal `iterable` must be a type
      with an iterator named "these" defined for it.

      :arg iterable: A collection of elements to add to this orderedSet.
      :arg parSafe: If `true`, this orderedSet will use parallel safe operations.
      :arg comparator: The comparator used to compare elements.
    */
    proc init(type eltType, iterable, param parSafe = false, comparator: record = defaultComparator)
    where canResolveMethod(iterable, "these") lifetime this < iterable {
      _checkType(eltType); 

      this.eltType = eltType;
      this.parSafe = parSafe;
      this.comparator = comparator;
      this.complete();

      for elem in iterable do _add(elem);
    }

    /*
      Inorder traversal for output
    */
    pragma "no doc"
    proc const _inorderVisit(node: nodeType, ch: channel) throws {
      if node == nil {
        return;
      }
      else {
        node!.sanityChecks();
        _inorderVisit(node!.children[0], ch);
        ch.write(node!.element, ' ');
        _inorderVisit(node!.children[1], ch);
      }
    }

    /*
      Visit and output elements in order
    */
    pragma "no doc"
    proc const _visit(ch: channel) throws {
      ch.write('[ ');
      _inorderVisit(_root, ch);
      ch.write(']');
    }

    pragma "no doc"
    proc ref _add(in x: eltType) lifetime this < x {
      _insert(_root, x, nil);
    }

    /*
      Add a copy of the element `x` to this orderedSet. Does nothing if this orderedSet
      already contains an element equal to the value of `x`.

      :arg x: The element to add to this orderedSet.
    */
    proc ref add(in x: eltType) lifetime this < x {

      // Remove `on this` block because it prevents copy elision of `x` when
      // passed to `_add`. See #15808.
      _enter();
      _add(x);
      _leave();
    }

    pragma "no doc"
    proc const _contains(const ref x: eltType): bool {
      return _find(_root, x) != nil;
    }

    /*
      Returns `true` if the given element is a member of this orderedSet, and `false`
      otherwise.

      :arg x: The element to test for membership.
      :return: Whether or not the given element is a member of this orderedSet.
      :rtype: `bool`
    */
    proc const contains(const ref x: eltType): bool {
      var result = false;

      on this {
        _enter(); 
        result = _contains(x);
        _leave();
      }

      return result;
    }

    /*
      Helper procedure to make x become a child of y, in position pos
      Note that it's not robust and should be called with care to avoid
      invalidating the tree. All it does is simply linking.
    */
    pragma "no doc"
    proc _link(x: nodeType, y: nodeType, pos: int) {
      if x != nil then
        x!.parent = y;
      if y != nil then
        y!.children[pos] = x;
    }

    /*
      Helper procedure to rotate
      The rotation will make the node.children[pos] become the new root of
      the subtree formerly rooted at node.

      Note that the rotation will change the value of node passed in.
    */
    pragma "no doc"
    proc _rotate(ref node: nodeType, pos: int) {
      var child = node!.children[pos];
      var parent = node!.parent;

      _link(child!.children[pos^1], node, pos);
      _link(node, child, pos^1);
      

      // Update the size field.
      node!.update();
      child!.update();

      child!.parent = parent;
      node = child; // This is for change the node in its parent's children array.
    }

    /*
      Helper procedure to locate a certain node
    */
    pragma "no doc"
    proc const _find(const node: nodeType, element: eltType): nodeType
    lifetime return node {
      if node == nil then return node;
      var cmp = chpl_compare(element, node!.element, comparator);
      if cmp == 0 then return node;
      else if cmp < 0 then return _find(node!.children[0], element);
      else return _find(node!.children[1], element);
    }

    /*
      Helper procedure to locate a certain node
      Returns a ref to the node
    */
    pragma "no doc"
    proc _findRef(ref node: nodeType, element: eltType) ref: nodeType
    lifetime return node {
      if node == nil then return node;
      var cmp = chpl_compare(element, node!.element, comparator);
      if cmp == 0 then return node;
      else if cmp < 0 then return _findRef(node!.children[0], element);
      else return _findRef(node!.children[1], element);
    }

    /* Given one element, return the reference to the element in the orderedSet, 
       which equals to the former in the perspective of the comparator.

       This procedure could halt when there is no hit.

       Used by orderedMap
     */
    pragma "no doc"
    proc _getReference(element: eltType) ref {
      var node = _findRef(_root, element);
      if node == nil then
        boundsCheckHalt("index " + element:string + " out of bounds");
      ref result = node!.element;
      return result;
    }

    /* Given one element, return the element in the orderedSet, which equals to the 
       former in the perspective of the comparator.

       This procedure could halt when there is no hit.

       Used by orderedMap
     */
    pragma "no doc"
    proc const _getValue(element: eltType) const {
      var node = _find(_root, element);
      if node == nil then
        boundsCheckHalt("index " + element:string + " out of bounds");
      var result = node!.element;
      return result;
    }

    /*
      Compare wrapper
    */
    pragma "no doc"
    proc const _compare(x: eltType, y: eltType) {
      return chpl_compare(x, y, comparator);
    }

    pragma "no doc"
    proc ref _insert(ref node: nodeType, element: eltType, parent: nodeType): bool {
      if node == nil {
        node = new nodeType(element, _random(), 1, parent);
        return true;
      }
      var cmp = _compare(element, node!.element);
      if cmp == 0 then return false;
      else {
        var result = false;
        var pos: int = cmp > 0;
        result = _insert(node!.children[pos], element, node);
        if node!.children[pos]!.priority > node!.priority {
          _rotate(node, pos);
        }
        node!.update();
        return result;
      }
    }

    pragma "no doc"
    proc ref _remove(ref node: nodeType, const ref x: eltType): bool {
      if node == nil then return false;
      var cmp = _compare(x, node!.element);
      if cmp == 0 {
        var children = node!.children;
        if children[0] == nil && children[1] == nil {
          // Leaf node, safely removed
          delete node;
          node = nil;
          return true;
        }

        // Choose the child with the greater priority
        var childPos = 0;
        if children[0] == nil {
          childPos = 1;
        }
        else {
          if children[1] != nil {
            if children[1]!.priority > children[0]!.priority {
              childPos = 1;
            }
          }
        }


        // Rotate the root down
        // Note that _rotate will change the value of node
        _rotate(node, childPos);

        // Remove the old root recursively
        var result = _remove(node!.children[childPos^1], x);
        node!.update();
        return result;
      }
      else {
        var pos: int = cmp > 0;
        var result = _remove(node!.children[pos], x);
        node!.update();
        return result;
      }
    }

    /*
      Attempt to remove the item from this orderedSet with a value equal to `x`. If
      an element equal to `x` was removed from this orderedSet, return `true`, else
      return `false` if no such value was found.

      :arg x: The element to remove.
      :return: Whether or not an element equal to `x` was removed.
      :rtype: `bool`
    */
    proc ref remove(const ref x: eltType): bool {
      var result = false;

      on this {
        _enter();
        result = _remove(_root, x);
        _leave();
      }

      return result;
    }

    /*
      Clear the contents of this orderedSet.

      .. warning::

        Clearing the contents of this orderedSet will invalidate all existing
        references to the elements contained in this orderedSet.
    */
    proc ref clear() {
      on this {
        _enter();
        if _root != nil {
          // Delete the tree recursively by nodes' deinit
          delete _root;
          _root = nil;
        }
        _leave();
      }
    }

    /*
      Helper procedure to return predecessor/successor of one node
      if direction is 0, return predecessor
      else if 1, return successor
    */
    pragma "no doc"
    proc _neighbor(in node: nodeType, in direction: int) {
      // Assuming direction is 1, we're finding the successor
      if node == nil then return nil;
      if node!.children[direction] {
        // node has right child,
        // find the leftmost node in its right child tree 
        node = node!.children[direction];
        direction ^= 1;
        while node != nil && node!.children[direction] != nil {
          node = node!.children[direction];
        }
        return node;
      }
      else {
        // node doesn't have right child,
        // find its first ancestor whose left child tree it belongs to
        direction ^= 1;
        while node!.parent != nil && node!.parent!.children[direction] != node {
          node = node!.parent;
        }
        return node!.parent;
      }
    }

    pragma "no doc"
    proc const _lower_bound(node: nodeType, e: eltType): nodeType {
      if node == nil then return nil;
      var cmp = _compare(e, node!.element);
      if cmp == 0 then return node;
      else if cmp < 0 {
        var result = _lower_bound(node!.children[0], e);
        if result != nil then return result;
        else return node;
      }
      else return _lower_bound(node!.children[1], e);
    }

    pragma "no doc"
    proc const _upper_bound(node: nodeType, e: eltType): nodeType {
      if node == nil then return nil;
      var cmp = _compare(e, node!.element);
      if cmp >= 0 then return _upper_bound(node!.children[1], e);
      else {
        var result = _upper_bound(node!.children[0], e);
        if result != nil then return result;
        else return node;
      }
    }

    /*
      Find the first element in the orderedSet
      which is not less than e.

      Returns a tuple containing two elements:
      The first element is a `bool` that indicates whether there is such an element.
      The second element is the occurrence in the orderedSet, if there's any.

      :returns: a tuple containing result
      :rtype: `(bool, eltType)`
    */
    proc const lowerBound(e: eltType): (bool, eltType) {
      if !isDefaultInitializable(e) {
        compilerError("lowerBound is not available on types that can't be \
                      default-initialized, here: " + eltType: string);
      }
      
      _enter(); defer _leave();

      var node = _lower_bound(_root, e);
      var result: (bool, eltType);
      if node == nil then  result[0] = false;
      else result = (true, node!.element);
      return result;
    }

    /*
      Find the first element in the orderedSet
      which is greater than e.

      Returns a tuple containing two elements:
      The first element is a `bool` that indicates whether there is such an element.
      The second element is the occurrence in the orderedSet, if there's any.

      :returns: a tuple containing result
      :rtype: `(bool, eltType)`
    */
    proc const upperBound(e: eltType): (bool, eltType) {
      if !isDefaultInitializable(e) {
        compilerError("upperBound is not available on types that can't be \
                      default-initialized, here: " + eltType: string);
      }

      _enter(); defer _leave();

      var node = _upper_bound(_root, e);
      var result: (bool, eltType);
      if node == nil then  result[0] = false;
      else result = (true, node!.element);
      return result;
    }

    /*
      Find the predecessor of one element in the orderedSet.

      Returns a tuple containing two elements:
      The first element is a `bool` that indicates whether there is such an element.
      The second element is the occurrence in the orderedSet, if there's any.

      :arg e: The element to base
      :type e: `eltType`

      :returns: a tuple containing result
      :rtype: `(bool, eltType)`
    */
    proc const predecessor(e: eltType): (bool, eltType) {
      if !isDefaultInitializable(e) {
        compilerError("predecessor is not available on types that can't be \
                      default-initialized, here: " + eltType: string);
      }

      _enter(); defer _leave();

      var result: (bool, eltType);
      result[0] = false;
      var baseNode = _find(_root, e);
      if baseNode == nil {
        return result;
      }

      var resultNode = _neighbor(baseNode, 0);
      if resultNode != nil {
        result = (true, resultNode!.element);
      }
      return result;
    }
    
    /*
      Find the successor of one element in the orderedSet.

      Returns a tuple containing two elements:
      The first element is a `bool` that indicates whether there is such an element.
      The second element is the occurrence in the orderedSet, if there's any.

      :arg e: The element to base
      :type e: `eltType`

      :returns: a tuple containing result
      :rtype: `(bool, eltType)`
    */
    proc const successor(e: eltType): (bool, eltType) {
      if !isDefaultInitializable(e) {
        compilerError("successor is not available on types that can't be \
                      default-initialized, here: " + eltType: string);
      }

      _enter(); defer _leave();

      var result: (bool, eltType);
      result[0] = false;
      var baseNode = _find(_root, e);
      if baseNode == nil {
        return result;
      }

      var resultNode = _neighbor(baseNode, 1);
      if resultNode != nil {
        result = (true, resultNode!.element);
      }
      return result;
    }

    /* Implementation for the `kth` procedure, without acquiring the lock */
    pragma "no doc"
    proc _kth(node: nodeType, in k: int): nodeType {
      if node == nil then return nil;

      /* The size of children
         If nil then 0 ( which is the default value for int )
      */
      var childSize: (int, int);

      if node!.children[0] != nil {
        childSize[0] = node!.children[0]!.size;
      }
      if node!.children[1] != nil {
        childSize[1] = node!.children[1]!.size;
      }

      // k-th in left subtree
      if k <= childSize[0] then return _kth(node!.children[0], k);
      k -= childSize[0];

      // k-th is the current node
      if k == 1 then return node;
      k -= 1;

      // k-th is in the right subtree
      if k <= childSize[1] then return _kth(node!.children[1], k);

      // else k-th is out of range, return nil
      return nil;
    }

    /*
      Find the k-th element in the orderedSet. k starts from 1.

      Returns a tuple containing two elements:
      The first element is a `bool` that indicates whether there is such an element.
      The second element is the occurrence in the orderedSet, if there's any.

      :arg k: To find k-th element
      :type k: `int`

      :returns: a tuple containing result
      :rtype: `(bool, eltType)`
    */
    proc const kth(k: int): (bool, eltType) {
      if !isDefaultInitializable(e) {
        compilerError("kth is not available on types that can't be \
                      default-initialized, here: " + eltType: string);
      }

      _enter(); defer _leave();

      var resultNode = _kth(_root, k);
      if resultNode == nil {
        var defaultValue: eltType;
        return (false, defaultValue);
      }
      return (true, resultNode!.element);
    }

    /*
      Returns the minimal element in the tree
    */
    pragma "no doc"
    proc _first() {
      var node = _root;
      while node != nil && node!.children[0] != nil {
        node = node!.children[0];
      }
      return node;
    }

    /*
      Iterate over the elements of this orderedSet. Yields constant references
      that cannot be modified.

      .. warning::

        Modifying this orderedSet while iterating over it may invalidate the
        references returned by an iterator and is considered undefined
        behavior.
      
      :yields: A constant reference to an element in this orderedSet.
    */
    pragma "not order independent yielding loops"
    iter const these() {
      var node = _first();
      while node != nil {
        yield node!.element;
        node = _neighbor(node, 1);
      }
    }

    /*
      Returns `true` if this orderedSet shares no elements in common with the orderedSet
      `other`, and `false` otherwise.

      :arg other: The orderedSet to compare against.
      :return: Whether or not this orderedSet and `other` are disjoint.
      :rtype: `bool`
    */
    proc const isDisjoint(const ref other: orderedSet(eltType, ?)): bool {
      var result = true;

      on this {
        _enter(); defer _leave();

        if !(size == 0 || other.size == 0) {

          // TODO: Take locks on other?
          for x in other do
            if this._contains(x) {
              result = false;
              break;
            }
        }
      }

      return result;
    }
    
    /*
      Returns `true` if this orderedSet and `other` have at least one element in
      common, and `false` otherwise.

      :arg other: The orderedSet to compare against.
      :return: Whether or not this orderedSet and `other` intersect.
      :rtype: `bool`
    */
    proc const isIntersecting(const ref other: orderedSet(eltType, ?)): bool {
      return !isDisjoint(other);
    }

    /*
      Write the contents of this orderedSet to a channel.

      :arg ch: A channel to write to.
    */
    proc const writeThis(ch: channel) throws {
      _enter();
      _visit(ch);
      _leave();
    }

    /*
      Returns `true` if this orderedSet is empty (size == 0).

      :rtype: `bool`
    */
    inline proc const isEmpty(): bool {
      var result = false;

      on this {
        _enter();
        result = _root == nil;
        _leave();
      }

      return result;
    }

    /*
      Returns a new array containing a copy of each of the
      elements contained in this orderedSet. The array will be in order.

      :return: An array containing a copy of each of the elements in this orderedSet.
      :rtype: `[] eltType`
    */
    proc const toArray(): [] eltType {
      // May take locks non-locally...
      _enter(); defer _leave();

      var treapSize = 0;
      if _root != nil then treapSize = _root!.size;

      var result: [0..#treapSize] eltType;

      if !isCopyableType(eltType) then
        compilerError('Cannot create array because orderedSet element type ' +
                      eltType:string + ' is not copyable');

      on this {
        if treapSize != 0 {
          var count = 0;
          var array: [0..#treapSize] eltType;

          for x in this {
            array[count] = x;
            count += 1;
          }

          result = array;
        }
      }

      return result;
    }
  }
}
