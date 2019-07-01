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
  This modules contains the implementation of the set type.
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
    A set is a...
  */
  record set {

    /* The type of the elements contained in this set. */
    type eltType;

    /* If `true`, this set will perform parallel safe operations. */
    param parSafe = false;

    pragma "no doc"
    var _size = 0;

    pragma "no doc"
    var _lock$ = if parSafe then new _LockWrapper() else none;

    /*
      Initializes an empty set containing elements of the given type.

      :arg eltType: The type of the elements of this set.
      :arg parSafe: If `true`, this set will use parallel safe operations.
    */
    proc init(type eltType, param parSafe=false) {
    }

    /*
      Initializes a set containing elements that are copy initialized from
      the elements contained in another set.

      :arg other: The set to initialize from.
      :arg parSafe: If `true`, this set will use parallel safe operations.
    */
    proc init=(const ref other: set(?t), param parSafe=other.parSafe) {
    }

    pragma "no doc"
    inline proc deinit() {
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
    proc add(const ref x: eltType) {
      return;
    }

    /*
      Returns `true` if the given element is a member of this set, and `false`
      otherwise.

      :arg x: The element to test for membership.
      :return: Whether or not the given element is a member of this set.
    */
    proc const contains(const ref x: eltType): bool {
      return false;
    }

    /*
      Returns `true` if this set shares no elements in common with the set
      `other`, and `false` otherwise.

      :arg other: The set to compare against.
      :return: Whether or not this set and `other` are disjoint.
    */
    proc const disjoint(const ref other: set(eltType, ?)): bool {
      return false;
    }

    /*
      Returns `true` if this set and `other` have at least one element in
      common.

      :arg other: The set to compare against.
      :return: Whether or not this set and `other` intersect.
    */
    proc const intersect(const ref other: set(eltType, ?)): bool {
      return false;
    }

    /*
      Returns `true` if the value of every element in this set is contained
      in the set `other`.

      :arg other: The set to compare against.
      :return: Whether or not this set is a subset of `other`.
    */
    proc const subset(const ref other: set(eltType, ?)): bool {
      return false;
    }

    /*
      Returns `true` if this set is a proper subset of `other`. That is,
      the value of every element in this set is contained in `other`, and
      this set does not equal `other`.

      :arg other: The set to compare against.
      :return: Whether or not this set is a proper subset of `other`.
    */
    proc const properSubset(const ref other: set(eltType, ?)): bool {
      return false;
    }

    /*
    	Returns `true` if the value of every element in the set `other` is
      contained in this set.

      :arg other: The set to compare against.
      :return: Whether or not this set is a superset of `other`.
    */
    proc const superset(const ref other: set(eltType, ?)): bool {
      return false;
    }

    /*
    	Returns `true` if this set is a proper superset of `other`. That is,
      the value of every element in `other` is contained in this set, and
      this set does not equal `other`.

      :arg other: The set to compare against.
      :return: Whether or not this set is a proper superset of `other`.
    */
    proc const properSuperset(const ref other: set(eltType, ?)): bool {
      return false;
    }

    /*
    	Remove the item from this set with a value equal to `x`.

      .. warning::

        Removing an element from this set may invalidate existing references
        to the elements contained in this set.
    
      :arg x: The element to remove.

      :throws IllegalArgumentError: If the list contains no such element.
    */
    proc remove(const ref x: eltType) throws {
      return;
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
      return;
    }

    /*
    	Clear the contents of this set.

      .. warning::

        Clearing the contents of this set will invalidate all existing
        references to the elements contained in this set.
    */
    proc clear() {
      return;
    }

    /*
    	Iterate over the elements of this set.

      :yields: A reference to one of the elements contained in this set.
    */
    iter these() ref {
      return nil;
    }

    /*
    	Write the contents of this set to a channel.

      :arg ch: A channel to write to.
    */
    proc const writeThis(ch: channel) {
      return;
    }

    /*
    	Returns `true` if this set contains zero elements.

      :return: `true` if this set is empty.
      :rtype: `bool`
    */
    inline proc const isEmpty(): bool {
      return false;
    }

    /*
    	The current number of elements contained in this set.
    */
    inline proc const size {
      return 0;
    }

    /*
    	Returns a new DefaultRectangular array containing a copy of each of the
      elements contained in this set. The returned array is not guaranteed
      to follow any particular ordering.
    */
    proc const toArray(): [] eltType {
      return nil;
    }

  } // End record "set".

  /*
    Clear the contents of this set, then iterate through the contents of `rhs`
    and add a copy of each element to this set.

    .. warning::

      This will invalidate any references to elements previously contained in
      `lhs`.

    :arg lhs: The set to assign to.
    :arg rhs: The set to assign from. 
  */
  proc =(ref lhs: set(?t, ?), const ref rhs: set(t, ?)) {
    return;
  }

  /*
    Return a new set that contains the union of two sets.

    :arg a: A set to take the difference of.
    :arg b: A set to take the difference of.

    :return: A new set containing the difference between `a` and `b`.
    :rtype: `set(?t, ?)`
  */
  proc setUnion(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t, ?) {
    return new set(t, false);
  }

  /*
    Return a new set that contains the union of two sets.

    :arg a: A set to take the difference of.
    :arg b: A set to take the difference of.

    :return: A new set containing the difference between `a` and `b`.
    :rtype: `set(?t, ?)`
  */
  proc |(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t, ?) {
    return setUnion(a, b);
  }

  /*
    Return a new set that contains the difference of two sets.

    :arg a: A set to take the difference of.
    :arg b: A set to take the difference of.

    :return: A new set containing the difference between `a` and `b`.
    :rtype: `set(?t, ?)`
  */
  proc setDifference(const ref a: set(?t, ?),
                     const ref b: set(t, ?)): set(t, ?) {
    return new set(t, false);
  }

  /*
    Return a new set that contains the difference of two sets.

    :arg a: A set to take the difference of.
    :arg b: A set to take the difference of.

    :return: A new set containing the difference between `a` and `b`.
    :rtype: `set(?t, ?)`
  */
  proc -(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t, ?) {
    return setDifference(a, b);
  }

  /*
  	Return a new set that contains the intersection of two sets.

    :arg a: A set to take the intersection of.
    :arg b: A set to take the intersection of.

    :return: A new set containing the intersection of `a` and `b`.
    :rtype: `set(?t, ?)`
  */
  proc setIntersection(const ref a: set(?t, ?),
                     const ref b: set(t, ?)): set(t, ?) {
    return new set(t, false);
  }

  /*
    Return a new set that contains the intersection of two sets.

    :arg a: A set to take the intersection of.
    :arg b: A set to take the intersection of.

    :return: A new set containing the intersection of `a` and `b`.
    :rtype: `set(?t, ?)`
  */
  proc &(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t, ?) {
    return setIntersection(a, b);
  }

  /*
    Return a new set that contains the symmetric difference of two sets.

    :arg a: A set to take the symmetric difference of.
    :arg b: A set to take the symmetric difference of.

    :return: A new set containing the symmetric difference of `a` and `b`.
    :rtype: `set(?t, ?)`
  */
  proc setSymmetricDiff(const ref a: set(?t, ?),
                     const ref b: set(t, ?)): set(t, ?) {
    return new set(t, false);
  }

  /*
  	Return the symmetric difference of two sets.

    :arg a: A set to take the symmetric difference of.
    :arg b: A set to take the symmetric difference of.

    :return: A new set containing the symmetric difference of `a` and `b`.
    :rtype: `set(?t, ?)`
  */
  proc ^(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t, ?) {
    return setSymmetricDiff(a, b);
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
    return false;
  }

  /*
    Return `true` if the sets `a` and `b` are not equal.

    :arg a: A set to compare.
    :arg b: A set to compare.

    :return: `true` if two sets are not equal.
    :rtype: `bool`
  */
  proc !=(const ref a: set(?t, ?), const ref b: set(t, ?)): bool {
    return false;
  }

  /*
    Return `true` if `a` is a proper subset of `b`.

    :arg a: A set to compare.
    :arg b: A set to compare.

    :return: `true` if `a` is a proper subset of `b`.
    :rtype: `bool`
  */
  proc <(const ref a: set(?t, ?), const ref b: set(t, ?)): bool {
    return false;
  }

  /*
    Return `true` if `a` is a subset of `b`.

    :arg a: A set to compare.
    :arg b: A set to compare.

    :return: `true` if `a` is a subset of `b`.
    :rtype: `bool`
  */
  proc <=(const ref a: set(?t, ?), const ref b: set(t, ?)): bool {
    return false;
  }

  /*
    Return `true` if `a` is a proper superset of `b`.

    :arg a: A set to compare.
    :arg b: A set to compare.

    :return: `true` if `a` is a proper superset of `b`.
    :rtype: `bool`
  */
  proc >(const ref a: set(?t, ?), const ref b: set(t, ?)): bool {
    return false;
  }

  /*
    Return `true` if `a` is a superset of `b`.

    :arg a: A set to compare.
    :arg b: A set to compare.

    :return `true` if `a` is a superset of `b`.
    :rtype: `bool`
  */
  proc >=(const ref a: set(?t, ?), const ref b: set(t, ?)): bool {
    return false;
  }

} // End module "Sets".

