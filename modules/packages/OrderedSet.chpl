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
  This module contains the implementation of the orderedSet type.

  An orderedSet is a collection of unique elements. orderSets are ordered.

  All references to orderedSet elements are invalidated when the orderedSet is 
  cleared or deinitialized.

  orderedSet are not parallel safe by default, but can be made parallel safe by 
  setting the param formal `parSafe` to true in any orderedSet constructor.

  When constructed from another orderedSet, 
  the new orderedSet will inherit the parallel safety mode 
  of its originating orderedSet.
*/
module OrderedSet {
  include module Treap;
  private use Treap;
  private use Reflection;
  private use IO;
  public use Sort only defaultComparator;


  //TODO: Maybe add skipList in the future
  /* Implementations supported */
  enum setImpl {treap};

  pragma "no doc"
  proc getTypeFromEnumVal(param val, type eltType, param parSafe) type {
    if val == setImpl.treap then return treap(eltType, parSafe);
  }

  pragma "no doc"
  proc getInstanceFromEnumVal(param val, type eltType, param parSafe,
                              comparator: record = defaultComparator) {
    if val == setImpl.treap then return new treap(eltType, parSafe, comparator);
  }

  pragma "no doc"
  proc getInstanceFromEnumVal(param val, type eltType, iterable, param parSafe,
                              comparator: record = defaultComparator)
                              where canResolveMethod(iterable, "these") {
    if val == setImpl.treap {
       return new treap(eltType, iterable, parSafe, comparator);
    }
  }

  /* The default implementation to use */
  param defaultImpl = setImpl.treap;

  record orderedSet {
    /* The type of the elements contained in this orderedSet. */
    type eltType;

    /* If `true`, this orderedSet will perform parallel safe operations. */
    param parSafe = false;

    /* The implementation to use */
    param implType = defaultImpl;

    //TODO: Should this documented?
    /* The underlying implementation */
    forwarding var instance: getTypeFromEnumVal(implType, eltType, parSafe);

    /*
      Initializes an empty orderedSet containing elements of the given type.

      :arg eltType: The type of the elements of this orderedSet.
      :arg parSafe: If `true`, this orderedSet will use parallel safe operations.
      :type parSafe: bool
      :arg comparator: The comparator used to compare elements.
    */
    proc init(type eltType, param parSafe = false,
              comparator: record = defaultComparator,
              param implType: setImpl = defaultImpl) {
      this.eltType = eltType;
      this.parSafe = parSafe;
      this.implType = implType;

      this.instance = getInstanceFromEnumVal(implType, eltType, parSafe, comparator); 
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
              comparator: record = defaultComparator,
              param implType: setImpl = defaultImpl)
    where canResolveMethod(iterable, "these") lifetime this < iterable {
      this.eltType = eltType;
      this.parSafe = parSafe;
      this.implType = implType;

      this.instance = getInstanceFromEnumVal(implType, eltType, iterable,
                                            parSafe, comparator); 
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
      this.instance = getInstanceFromEnumVal(this.implType, this.eltType,
                                            this.parSafe,
                                            other.instance.comparator); 

      this.complete();


      if !isCopyableType(eltType) then
        compilerError('Cannot initialize ' + this.type:string + ' from ' +
                      other.type:string + ' because element type ' +
                      eltType:string + ' is not copyable');

      for elem in other do instance._add(elem);
    }

    /*
      Write the contents of this orderedSet to a channel.

      :arg ch: A channel to write to.
    */
    proc const writeThis(ch: channel) throws {
      instance.writeThis(ch);
    }

    //FIXME: Workaround for https://github.com/chapel-lang/chapel/issues/16045
    pragma "no doc"
    proc const kth(k: int, ref result: eltType): bool {
      return instance.kth(k, result);
    }

    pragma "no doc"
    proc const lowerBound(e: eltType, ref result: eltType): bool {
      return instance.lowerBound(e, result);
    }

    pragma "no doc"
    proc const upperBound(e: eltType, ref result: eltType): bool {
      return instance.upperBound(e, result);
    }

    pragma "no doc"
    proc const predecessor(e: eltType, ref result: eltType) {
      return instance.predecessor(e, result);
    }

    pragma "no doc"
    proc const successor(e: eltType, ref result: eltType) {
      return instance.successor(e, result);
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
  proc =(ref lhs: orderedSet(?t), rhs: orderedSet(?r)) {
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

    var result = true;

    for x in a do
      if !b.contains(x) then
        return false;

    return result;
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

    var result = true;

    for x in a do
      if !b.contains(x) then
        return false;

    return result;
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

    var result = true;

    for x in b do
      if !a.contains(x) then
        return false;

    return result;
  }
}
