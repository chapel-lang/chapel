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
  This module contains the implementation of the set type.

  A set is a collection of unique elements.

  [Paragraph about parallel invalidating references.]
  [List of operations that may invalidate references.]
  [Paragraph about parallel safety.]
  [Speed of common operations.]
*/
module Sets {

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
    A set is a collection of unique elements. If an attempt is made to add an
    element to a set that already contains an element with an equal value, it
    will not be added again. The set type supports a test for membership via
    the `contains` operator, along with free functions for calculating the
    union, difference, intersection, and symmetric difference of two sets. The
    set type also defines the (proper) subset and (proper) superset operations
    by overloading common comparison operators.

    Sets can be iterated over, but they do not support random access. A set
    can be default initialized (containing no elements), or it may be
    initialized with elements that are a copy of those contained in any
    iterator.

    The set type is not parallel safe by default. For situations in which
    such protections are desirable, parallel safety can be enabled by setting
    `parSafe = true` in any set constructor. A set constructed from another
    set inherits the parallel safety mode of that set by default.
  */
  record set {

    /* The type of the elements contained in this set. */
    type eltType;

    /* If `true`, this set will perform parallel safe operations. */
    param parSafe = false;

    pragma "no doc"
    var _lock$ = if parSafe then new _LockWrapper() else none;

    pragma "no doc"
    var _dom: domain(eltType, parSafe=false);

    /*
      Initializes an empty set containing elements of the given type.

      :arg eltType: The type of the elements of this set.
      :arg parSafe: If `true`, this set will use parallel safe operations.
    */
    proc init(type eltType, param parSafe=false) {
      this.eltType = eltType;
      this.parSafe = parSafe;
    }

    /*
      Initialize this set with a unique copy of each element contained in
      `iterable`. If an element from `iterable` is already contained in this
      set, it will not be added again. The formal `iterable` must be a type
      with an iterator defined for it.

      :arg iterable: A collection of elements to add to this set.
      :arg parSafe: If `true`, this set will use parallel safe operations.
    */
    proc init(type eltType, param parSafe, iterable) {
      this.eltType = eltType;
      this.parSafe = parSafe;
      this.complete();
      //
      // TODO: Is there a reliable way to get iterable size, here? If so, we
      // could make a call to `requestCapacity`.
      //
      for x in iterable do
        _dom.add(x);
    }

    /*
      Initialize this set with a copy of each of the elements contained in
      the set `other`. This set will inherit the `parSafe` value of the
      set `other`.

      :arg other: A set to initialize this set with.
    */
    proc init=(const ref other: set(?t, ?)) {
      this.eltType = t;
      this.parSafe = other.parSafe;
      this.complete();
      for x in other {
        //
        // TODO: Not entirely sure if this inner copy is necessary, but we
        // want to avoid potentially locking over and over.
        //
        var cpy = x;
        _dom.add(cpy);
      }
    }

    pragma "no doc"
    inline proc deinit() {
      clear();
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
 
    /*
      Add a copy of the element `x` to this set. Does nothing if this set
      already contains an element equal to the value of `x`.

      :arg x: The element to add to this set.
    */
    proc add(in x: eltType) {
      _enter();
      const dbg = _dom.add(x);
      _leave();
    }

    /*
      Returns `true` if the given element is a member of this set, and `false`
      otherwise.

      :arg x: The element to test for membership.
      :return: Whether or not the given element is a member of this set.
    */
    proc const contains(const ref x: eltType): bool {
      _enter();
      var result = _dom.contains(x);
      _leave();
      return result;
    }

    /*
      Returns `true` if this set shares no elements in common with the set
      `other`, and `false` otherwise.

      :arg other: The set to compare against.
      :return: Whether or not this set and `other` are disjoint.
    */
    proc const isDisjoint(const ref other: set(eltType, ?)): bool {
      _enter();

      var result = true;
      for x in other do
        // TODO: Can we insert a break in a for statement?
        if !_dom.contains(x) then 
          result = false;

      _leave();

      return result;
    }

    /*
      Returns `true` if this set and `other` have at least one element in
      common.

      :arg other: The set to compare against.
      :return: Whether or not this set and `other` intersect.
    */
    proc const isIntersecting(const ref other: set(eltType, ?)): bool {
      return !isDisjoint(other);
    }

    /*
      Remove the item from this set with a value equal to `x`.

      .. warning::

        Removing an element from this set may invalidate existing references
        to the elements contained in this set.
    
      :arg x: The element to remove.

      :throws IllegalArgumentError: If the list contains no such element.
    */
    proc remove(const ref x: eltType): eltType throws {
      _enter();

      if !contains(x) {
        _leave();
        const msg = "No such element in set: " + x:string;
        // TODO: Replace with more appropriate error?
        throw new owned
          IllegalArgumentError(msg);
      }

      var result = _dom.remove(x);
      _leave();

      return result;
    }

    /*
      Remove the item `x` if it is present in this set, but do nothing if it
      is not.

      .. warning::

        Discarding an element from this set may invalidate existing references
        to the elements contained in this set.

      :arg x: The element to discard.
    */
    proc discard(const ref x: eltType) {
      _enter();

      if _dom.contains(x) then
        _dom.remove(x);

      _leave();
    }

    /*
      Clear the contents of this set.

      .. warning::

        Clearing the contents of this set will invalidate all existing
        references to the elements contained in this set.
    */
    proc clear() {
      _enter();
      _dom.clear();
      _leave();
    }

    /*
      Iterate over the elements of this set.

      :yields: A reference to one of the elements contained in this set.
    */
    iter these() ref {
      // TODO: How to make a parallel safe iterator?
      for x in _dom do
        yield x;
    }

    /*
      Write the contents of this set to a channel.

      :arg ch: A channel to write to.
    */
    proc const writeThis(ch: channel) {
      _enter();

      var count = 1;
      ch <~> "[";

      for x in _dom {
        if count <= (_dom.size - 1) {
          count += 1;
          ch <~> x <~> ", ";
        } else {
          ch <~> x;
        }
      }

      ch <~> "]";

      _leave();
    }

    /*
      Returns `true` if this set contains zero elements.

      :return: `true` if this set is empty.
      :rtype: `bool`
    */
    inline proc const isEmpty(): bool {
      _enter();
      const result = _dom.isEmpty();
      _leave();
      return result;
    }

    /*
      The current number of elements contained in this set.
    */
    inline proc const size {
      _enter();
      const result = _dom.size;
      _leave();
      return result;
    }

    /*
      Returns a new DefaultRectangular array containing a copy of each of the
      elements contained in this set. The elements of the returned array are
      not guaranteed to follow any particular ordering.
    */
    proc const toArray(): [] eltType {
      _enter();
      var result: [1.._dom.size] eltType;
      var count = 1;

      for x in _dom {
        result[count] = x;
        count += 1;
      }

      _leave();

      return result;
    }

  } // End record "set".

  /*
    Clear the contents of the set `lhs`, then iterate through the contents of
    `rhs` and add a copy of each element to `lhs`.

    .. warning::

      This will invalidate any references to elements previously contained in
      the set `lhs`.

    :arg lhs: The set to assign to.
    :arg rhs: The set to assign from. 
  */
  proc =(ref lhs: set(?t, ?), const ref rhs: set(t, ?)) {
    lhs.clear();
    for x in rhs do
      lhs.add(x);
  }

  /*
    Return a new set that contains the union of two sets.

    :arg a: A set to take the union of.
    :arg b: A set to take the union of.

    :return: A new set containing the union between `a` and `b`.
    :rtype: `set(?t, ?)`
  */
  proc |(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t, ?) {
    var result: set(t, (a.parSafe && b.parSafe));

    for x in a do
      result.add(x);

    for x in b do
      result.add(b);

    return result;
  }

  /*
    Assign to the set `lhs` the set that is the union of `lhs` and `rhs`.

    .. warning::

      This will invalidate any references to elements previously contained in
      the set `lhs`.

    :arg lhs: A set to take the union of and then assign to.
    :arg rhs: A set to take the union of.
  */
  proc |=(ref lhs: set(?t, ?), const ref rhs: set(t, ?)) {
    lhs = lhs | rhs;
  }

  /*
    Return a new set that contains the union of two sets. Alias for the `|`
    operator.

    :arg a: A set to take the union of.
    :arg b: A set to take the union of.

    :return: A new set containing the union between `a` and `b`.
    :rtype: `set(?t, ?)`
  */
  proc +(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t, ?) {
    return a | b;
  }

  /*
    Assign to the set `lhs` the set that is the union of `lhs` and `rhs`.

    .. warning::

      This will invalidate any references to elements previously contained in
      the set `lhs`.

    :arg lhs: A set to take the union of and then assign to.
    :arg rhs: A set to take the union of.
  */
  proc +=(ref lhs: set(?t, ?), const ref rhs: set(t, ?)) {
    lhs = lhs + rhs;
  }

  /*
    Return a new set that contains the difference of two sets.

    :arg a: A set to take the difference of.
    :arg b: A set to take the difference of.

    :return: A new set containing the difference between `a` and `b`.
    :rtype: `set(t)`
  */
  proc -(const ref a: set(?t, ?p1), const ref b: set(t, ?p2)): set(t) {
    var result = new set(t, (p1 || p2));

    for x in a do
      if !b.contains(x) then
        result.add(x);

    return result;
  }

  /*
    Assign to the set `lhs` the set that is the difference of `lhs` and `rhs`.

    .. warning::

      This will invalidate any references to elements previously contained in
      the set `lhs`.

    :arg lhs: A set to take the difference of and then assign to.
    :arg rhs: A set to take the difference of.
  */
  proc -=(ref lhs: set(?t, ?), const ref rhs: set(t, ?)) {
    lhs = lhs - rhs;
  }

  /*
    Return a new set that contains the intersection of two sets.

    :arg a: A set to take the intersection of.
    :arg b: A set to take the intersection of.

    :return: A new set containing the intersection of `a` and `b`.
    :rtype: `set(?t, ?)`
  */
  proc &(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t, ?) {
    var result: set(t, (a.parSafe && b.parSafe));

    for x in a do
      if b.contains(x) then
        result.add(x);

    return result;
  }

  /*
    Assign to the set `lhs` the set that is the intersection of `lhs` and
    `rhs`.

    .. warning::

      This will invalidate any references to elements previously contained in
      the set `lhs`.

    :arg lhs: A set to take the intersection of and then assign to.
    :arg rhs: A set to take the intersection of.
  */
  proc &=(ref lhs: set(?t, ?), const ref rhs: set(t, ?)) {
    lhs = lhs & rhs;
  }

  /*
    Return the symmetric difference of two sets.

    :arg a: A set to take the symmetric difference of.
    :arg b: A set to take the symmetric difference of.

    :return: A new set containing the symmetric difference of `a` and `b`.
    :rtype: `set(?t, ?)`
  */
  proc ^(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t, ?) {
    var result: set(t, (a.parSafe && b.parSaf));
    
    for x in a do
      if !b.contains(x) then
        result.add(x);

    for x in b do
      if !a.contains(x) then
        result.add(x);

    return result;
  }

  /*
    Assign to the set `lhs` the set that is the symmetric difference of `lhs`
    and `rhs`.

    .. warning::

      This will invalidate any references to elements previously contained in
      the set `lhs`.

    :arg lhs: A set to take the symmetric difference of and then assign to.
    :arg rhs: A set to take the symmetric difference of.
  */
  proc ^=(ref lhs: set(?t, ?), const ref rhs: set(t, ?)) {
    lhs = lhs ^ rhs;
  }

  /*
    Return `true` if the sets `a` and `b` are equal. That is, they are the
    same size and contain the same elements.

    :arg a: A set to compare.
    :arg b: A set to compare.

    :return: `true` if two sets are equal.
    :rtype: `bool`
  */
  proc ==(const ref a: set(?t, ?), const ref b: set(t, ?)): bool {
    if a.size != b.size then
      return false;

    var result = true;

    for x in a do
      if !b.contains(x) then
        result = false;

    return result;
  }

  /*
    Return `true` if the sets `a` and `b` are not equal.

    :arg a: A set to compare.
    :arg b: A set to compare.

    :return: `true` if two sets are not equal.
    :rtype: `bool`
  */
  proc !=(const ref a: set(?t, ?), const ref b: set(t, ?)): bool {
    return !(a == b);
  }

  /*
    Return `true` if `a` is a proper subset of `b`.

    :arg a: A set to compare.
    :arg b: A set to compare.

    :return: `true` if `a` is a proper subset of `b`.
    :rtype: `bool`
  */
  proc <(const ref a: set(?t, ?), const ref b: set(t, ?)): bool {
    if a.size >= b.size then
      return false;
    return a <= b;
  }

  /*
    Return `true` if `a` is a subset of `b`.

    :arg a: A set to compare.
    :arg b: A set to compare.

    :return: `true` if `a` is a subset of `b`.
    :rtype: `bool`
  */
  proc <=(const ref a: set(?t, ?), const ref b: set(t, ?)): bool {
    if a.size > b.size then
      return false;

    var result = true;

    // TODO: Do we need to guard/make result atomic here?
    for x in a do
      if !b.contains(x) then
        result = false;

    return result;
  }

  /*
    Return `true` if `a` is a proper superset of `b`.

    :arg a: A set to compare.
    :arg b: A set to compare.

    :return: `true` if `a` is a proper superset of `b`.
    :rtype: `bool`
  */
  proc >(const ref a: set(?t, ?), const ref b: set(t, ?)): bool {
    if a.size <= b.size then
      return false;
    return a >= b;
  }

  /*
    Return `true` if `a` is a superset of `b`.

    :arg a: A set to compare.
    :arg b: A set to compare.

    :return: `true` if `a` is a superset of `b`.
    :rtype: `bool`
  */
  proc >=(const ref a: set(?t, ?), const ref b: set(t, ?)): bool {
    if a.size < b.size then
      return false;

    var result = true;

    for x in a do
      if !b.contains(x) then
        result = false;

    return result;
  }

} // End module "Sets".

