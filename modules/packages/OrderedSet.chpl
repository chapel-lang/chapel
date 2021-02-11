/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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
  This module contains the implementation of the ``orderedSet`` type.

  An ``orderedSet`` is a collection of unique and ordered elements. The
  ``orderedSet`` accepts a :ref:`comparator <comparators>` to determine how
  elements are compared.  The default comparator is `defaultComparator`. In this
  case, elements are stored and considered in ascending order. For example,
  ``these`` will yield elements in ascending order.

  All references to ``orderedSet`` elements are invalidated when the ``orderedSet`` is 
  cleared or deinitialized.

  ``orderedSet`` is not parallel safe by default, but can be made parallel safe
  by setting the param formal `parSafe` to true in any ``orderedSet``
  constructor. When constructed from another ``orderedSet``, the new
  ``orderedSet`` will inherit the parallel safety mode of its originating
  ``orderedSet``.

*/
module OrderedSet {
  include module Treap;
  private use Treap;
  private use Reflection;
  private use IO;
  public use Sort only defaultComparator;

  record orderedSet {
    /* The type of the elements contained in this orderedSet. */
    type eltType;

    /* If `true`, this orderedSet will perform parallel safe operations. */
    param parSafe = false;

    /* The underlying implementation */
    pragma "no doc"
    var instance: treap(eltType, parSafe);

    /*
      Initializes an empty orderedSet containing elements of the given type.

      :arg eltType: The type of the elements of this orderedSet.
      :arg parSafe: If `true`, this orderedSet will use parallel safe operations.
      :arg comparator: The comparator used to compare elements.
    */
    proc init(type eltType, param parSafe = false,
              comparator: record = defaultComparator) {
      this.eltType = eltType;
      this.parSafe = parSafe;

      this.instance = new treap(eltType, parSafe, comparator);
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
    proc init(type eltType, iterable, param parSafe=false,
              comparator: record = defaultComparator)
    where canResolveMethod(iterable, "these") lifetime this < iterable {
      this.eltType = eltType;
      this.parSafe = parSafe;

      this.instance = new treap(eltType, iterable, parSafe, comparator);
    }

    /*
      Initialize this orderedSet with a copy of each of the elements contained in
      the orderedSet `other`. This orderedSet will inherit the `parSafe` value of 
      the orderedSet `other`.

      :arg other: An orderedSet to initialize this orderedSet with.
    */
    proc init=(const ref other: orderedSet(?t)) lifetime this < other {
      this.eltType = t;
      this.parSafe = other.parSafe;
      this.instance = new treap(this.eltType, this.parSafe,
                                            other.instance.comparator); 

      this.complete();


      if !isCopyableType(eltType) then
        compilerError('cannot initialize ' + this.type:string + ' from ' +
                      other.type:string + ' because element type ' +
                      eltType:string + ' is not copyable');

      for elem in other do instance._add(elem);
    }

    /*
      Write the contents of this orderedSet to a channel.

      :arg ch: A channel to write to.
    */
    inline proc const writeThis(ch: channel) throws {
      instance.writeThis(ch);
    }

    /*
      The current number of elements contained in this orderedSet.
    */
    inline proc const size {
      return instance.size;
    }

    /*
      Add a copy of the element `x` to this orderedSet. Does nothing if this orderedSet
      already contains an element equal to the value of `x`.

      :arg x: The element to add to this orderedSet.
    */
    inline proc ref add(in x: eltType) lifetime this < x {
      instance.add(x);
    }

    /*
      Returns `true` if the given element is a member of this orderedSet, and `false`
      otherwise.

      :arg x: The element to test for membership.
      :return: Whether or not the given element is a member of this orderedSet.
      :rtype: `bool`
    */
    inline proc const contains(const ref x: eltType): bool {
      return instance.contains(x);
    }

    /*
      Attempt to remove the item from this orderedSet with a value equal to `x`. If
      an element equal to `x` was removed from this orderedSet, return `true`, else
      return `false` if no such value was found.

      :arg x: The element to remove.
      :return: Whether or not an element equal to `x` was removed.
      :rtype: `bool`
    */
    inline proc ref remove(const ref x: eltType): bool {
      return instance.remove(x);
    }

    /*
      Clear the contents of this orderedSet.

      .. warning::

        Clearing the contents of this orderedSet will invalidate all existing
        references to the elements contained in this orderedSet.
    */
    inline proc ref clear() {
      instance.clear();
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
    inline proc const lowerBound(e: eltType): (bool, eltType) {
      return instance.lowerBound(e);
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
    inline proc const upperBound(e: eltType): (bool, eltType) {
      return instance.upperBound(e);
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
    inline proc const predecessor(e: eltType): (bool, eltType) {
      return instance.predecessor(e);
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
    inline proc const successor(e: eltType): (bool, eltType) {
      return instance.successor(e);
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
    inline proc const kth(k: int): (bool, eltType) {
      return instance.kth(k);
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
    iter const these() {
      for x in instance do
        yield x;
    }

    /*
      Returns `true` if this orderedSet shares no elements in common with the orderedSet
      `other`, and `false` otherwise.

      :arg other: The orderedSet to compare against.
      :return: Whether or not this orderedSet and `other` are disjoint.
      :rtype: `bool`
    */
    inline proc const isDisjoint(const ref other: orderedSet(eltType, ?)): bool {
      return instance.isDisjoint(other);
    }

    /*
      Returns `true` if this orderedSet and `other` have at least one element in
      common, and `false` otherwise.

      :arg other: The orderedSet to compare against.
      :return: Whether or not this orderedSet and `other` intersect.
      :rtype: `bool`
    */
    inline proc const isIntersecting(const ref other: orderedSet(eltType, ?)): bool {
      return instance.isIntersecting(other);
    }

    /*
      Returns `true` if this orderedSet is empty (size == 0).

      :rtype: `bool`
    */
    inline proc const isEmpty(): bool {
      return instance.isEmpty();
    }

    /*
      Returns a new array containing a copy of each of the
      elements contained in this orderedSet. The array will be in order.

      :return: An array containing a copy of each of the elements in this orderedSet.
      :rtype: `[] eltType`
    */
    inline proc const toArray(): [] eltType {
      return instance.toArray();
    }
  }

  /*
    NOTE: Operators are borrowed from Set.chpl
  */

  /*
    Clear the contents of this orderedSet, then extend this now empty orderedSet 
    with the elements contained in another orderedSet.

    .. warning::

      This will invalidate any references to elements previously contained in
      `lhs`.

    :arg lhs: The orderedSet to assign to.
    :arg rhs: The orderedSet to assign from. 
  */
  proc =(ref lhs: orderedSet(?t), rhs: orderedSet(t)) {
    lhs.clear();
    for x in rhs {
      lhs.add(x);
    }
  }

  /*
    Return a new orderedSet that contains the union of two sets.

    :arg a: An orderedSet to take the union of.
    :arg b: An orderedSet to take the union of.

    :return: A new orderedSet containing the union between `a` and `b`.
    :rtype: `orderedSet(?t)`
  */
  proc |(const ref a: orderedSet(?t), const ref b: orderedSet(t)): orderedSet(t) {
    var result: orderedSet(t, (a.parSafe || b.parSafe));

    result = a;
    result |= b;

    return result;
  }

  /*
    Add to the orderedSet `lhs` all the elements of `rhs`.

    :arg lhs: An orderedSet to take the union of and then assign to.
    :arg rhs: An orderedSet to take the union of.
  */
  proc |=(ref lhs: orderedSet(?t), const ref rhs: orderedSet(t)) {
    for x in rhs do
      lhs.add(x);
  }

  /*
    Return a new orderedSet that contains the union of two sets. Alias for the `|`
    operator.

    :arg a: An orderedSet to take the union of.
    :arg b: An orderedSet to take the union of.

    :return: A new orderedSet containing the union between `a` and `b`.
    :rtype: `orderedSet(?t)`
  */
  proc +(const ref a: orderedSet(?t), const ref b: orderedSet(t)): orderedSet(t) {
    return a | b;
  }

  /*
    Add to the orderedSet `lhs` all the elements of `rhs`.

    :arg lhs: An orderedSet to take the union of and then assign to.
    :arg rhs: An orderedSet to take the union of.
  */
  proc +=(ref lhs: orderedSet(?t), const ref rhs: orderedSet(t)) {
    lhs |= rhs;
  }

  /*
    Return a new orderedSet that contains the difference of two sets.

    :arg a: An orderedSet to take the difference of.
    :arg b: An orderedSet to take the difference of.

    :return: A new orderedSet containing the difference between `a` and `b`.
    :rtype: `orderedSet(t)`
  */
  proc -(const ref a: orderedSet(?t), const ref b: orderedSet(t)): orderedSet(t) {
    var result = new orderedSet(t, (a.parSafe || b.parSafe));

    for x in a do
      if !b.contains(x) then
        result.add(x);

    return result;
  }

  /*
    Remove from the orderedSet `lhs` the elements of `rhs`.

    .. warning::

      This will invalidate any references to elements previously contained in
      the orderedSet `lhs`.

    :arg lhs: An orderedSet to take the difference of and then assign to.
    :arg rhs: An orderedSet to take the difference of.
  */
  proc -=(ref lhs: orderedSet(?t), const ref rhs: orderedSet(t)) {
    for x in rhs do
      lhs.remove(x);
  }

  /*
    Return a new orderedSet that contains the intersection of two sets.

    :arg a: An orderedSet to take the intersection of.
    :arg b: An orderedSet to take the intersection of.

    :return: A new orderedSet containing the intersection of `a` and `b`.
    :rtype: `orderedSet(t)`
  */
  proc &(const ref a: orderedSet(?t), const ref b: orderedSet(t)): orderedSet(t) {
    var result: orderedSet(t, (a.parSafe || b.parSafe));

    /* Iterate over the smaller orderedSet */
    if a.size <= b.size {
      for x in a do
        if b.contains(x) then
          result.add(x);
    } else {
      for x in b do
        if a.contains(x) then
          result.add(x);
    }

    return result;
  }

  /*
    Assign to the orderedSet `lhs` the orderedSet that is the intersection of `lhs` 
    and `rhs`.

    .. warning::

      This will invalidate any references to elements previously contained in
      the orderedSet `lhs`.

    :arg lhs: An orderedSet to take the intersection of and then assign to.
    :arg rhs: An orderedSet to take the intersection of.
  */
  proc &=(ref lhs: orderedSet(?t, ?), const ref rhs: orderedSet(t, ?)) {
    /* We can't remove things from lhs while iterating over it, so
     * use a temporary. */
    var result: orderedSet(t, (lhs.parSafe || rhs.parSafe));

    for x in lhs do
      if rhs.contains(x) then
        result.add(x);

    lhs = result;
  }

  /*
    Return the symmetric difference of two sets.

    :arg a: An orderedSet to take the symmetric difference of.
    :arg b: An orderedSet to take the symmetric difference of.

    :return: A new orderedSet containing the symmetric difference of `a` and `b`.
    :rtype: `orderedSet(?t)`
  */
  proc ^(const ref a: orderedSet(?t), const ref b: orderedSet(t)): orderedSet(t) {
    var result: orderedSet(t, (a.parSafe || b.parSafe));

    /* Expect the loop in ^= to be more expensive than the loop in =,
       so arrange for the rhs of the ^= to be the smaller orderedSet. */
    if a.size <= b.size {
      result = b;
      result ^= a;
    } else {
      result = a;
      result ^= b;
    }

    return result;
  }

  /*
    Assign to the orderedSet `lhs` the orderedSet that is the symmetric difference
    of `lhs` and `rhs`.

    .. warning::

      This will invalidate any references to elements previously contained in
      the orderedSet `lhs`.

    :arg lhs: An orderedSet to take the symmetric difference of and then assign to.
    :arg rhs: An orderedSet to take the symmetric difference of.
  */
  proc ^=(ref lhs: orderedSet(?t), const ref rhs: orderedSet(t)) {
    for x in rhs {
      if lhs.contains(x) {
        lhs.remove(x);
      } else {
        lhs.add(x);
      }
    }
  }

  /*
    Return `true` if the sets `a` and `b` are equal. That is, they are the
    same size and contain the same elements.

    :arg a: An orderedSet to compare.
    :arg b: An orderedSet to compare.

    :return: `true` if two sets are equal.
    :rtype: `bool`
  */
  proc ==(const ref a: orderedSet(?t), const ref b: orderedSet(t)): bool {
    if a.size != b.size then
      return false;

    for x in a do
      if !b.contains(x) then
        return false;

    return true;
  }

  /*
    Return `true` if the sets `a` and `b` are not equal.

    :arg a: An orderedSet to compare.
    :arg b: An orderedSet to compare.

    :return: `true` if two sets are not equal.
    :rtype: `bool`
  */
  proc !=(const ref a: orderedSet(?t), const ref b: orderedSet(t)): bool {
    return !(a == b);
  }

  /*
    Return `true` if `a` is a proper subset of `b`.

    :arg a: An orderedSet to compare.
    :arg b: An orderedSet to compare.

    :return: `true` if `a` is a proper subset of `b`.
    :rtype: `bool`
  */
  proc <(const ref a: orderedSet(?t), const ref b: orderedSet(t)): bool {
    if a.size >= b.size then
      return false;
    return a <= b;
  }

  /*
    Return `true` if `a` is a subset of `b`.

    :arg a: An orderedSet to compare.
    :arg b: An orderedSet to compare.

    :return: `true` if `a` is a subset of `b`.
    :rtype: `bool`
  */
  proc <=(const ref a: orderedSet(?t), const ref b: orderedSet(t)): bool {
    if a.size > b.size then
      return false;

    for x in a do
      if !b.contains(x) then
        return false;

    return true;
  }

  /*
    Return `true` if `a` is a proper superset of `b`.

    :arg a: An orderedSet to compare.
    :arg b: An orderedSet to compare.

    :return: `true` if `a` is a proper superset of `b`.
    :rtype: `bool`
  */
  proc >(const ref a: orderedSet(?t), const ref b: orderedSet(t)): bool {
    if a.size <= b.size then
      return false;
    return a >= b;
  }

  /*
    Return `true` if `a` is a superset of `b`.

    :arg a: An orderedSet to compare.
    :arg b: An orderedSet to compare.

    :return: `true` if `a` is a superset of `b`.
    :rtype: `bool`
  */
  proc >=(const ref a: orderedSet(?t), const ref b: orderedSet(t)): bool {
    if a.size < b.size then
      return false;

    for x in b do
      if !a.contains(x) then
        return false;

    return true;
  }
}
