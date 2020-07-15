/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

  A set is a collection of unique elements. Sets are unordered and unindexed.

  The highly parallel nature of Chapel means that great care should be taken
  when performing operations that may invalidate references to set elements.
  Adding or removing an element from a set may invalidate references to
  elements contained in the set.

  All references to set elements are invalidated when the set is cleared or
  deinitialized.

  Sets are not parallel safe by default, but can be made parallel safe by
  setting the param formal 'parSafe` to true in any set constructor. When
  constructed from another set, the new set will inherit the parallel safety
  mode of its originating set.
*/
module Set {

  //
  // Use this to restrict our secondary initializer to only resolve when the
  // "iterable" argument has a method named "these".
  //
  import ChapelLocks;
  private use IO;
  private use Reflection;
  private use ChapelHashtable;

  pragma "no doc"
  private param _sanityChecks = true;

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
  // contention.
  //
  pragma "no doc"
  type _lockType = ChapelLocks.chpl_LocalSpinlock;

  //
  // Use a wrapper class to let set methods have a const ref receiver even
  // when `parSafe` is `true` and the set lock is used.
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
  proc _checkElementType(type t) {
    // In the future we might support it if the set is not default-inited.
    if isGenericType(t) {
      compilerWarning('creating a set with element type ' + t:string, 2);
      compilerError('set element type cannot currently be generic', 2);
    }
  }

  /*
    A set is a collection of unique elements. Attempting to add a duplicate
    element to a set has no effect.

    The set type supports a test for membership via the :proc:`contains`
    method, along with free functions for calculating the union, difference,
    intersection, and symmetric difference of two sets. The set type also
    defines the (proper) subset and (proper) superset operations by
    overloading common comparison operators.

    Sets can be iterated over. The set type makes no guarantee of a consistent
    iteration order.

    A set can be default initialized (containing no elements), or it may be
    initialized with elements that are copies of those contained by any
    type that supports an iterator.

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
    var _htb: chpl__hashtable(eltType, nothing);

    /*
      Initializes an empty set containing elements of the given type.

      :arg eltType: The type of the elements of this set.
      :arg parSafe: If `true`, this set will use parallel safe operations.
    */
    proc init(type eltType, param parSafe=false) {
      _checkElementType(eltType);
      this.eltType = eltType;
      this.parSafe = parSafe;
    }

    // Returns true if the key was added to the hashtable.
    pragma "no doc"
    proc _addElem(in elem: eltType): bool {
      var (isFullSlot, idx) = _htb.findAvailableSlot(elem);

      if isFullSlot then return false;
      _htb.fillSlot(idx, elem, none);
      return true;
    }

    /*
      Initialize this set with a unique copy of each element contained in
      `iterable`. If an element from `iterable` is already contained in this
      set, it will not be added again. The formal `iterable` must be a type
      with an iterator named "these" defined for it.

      :arg iterable: A collection of elements to add to this set.
      :arg parSafe: If `true`, this set will use parallel safe operations.
    */
    proc init(type eltType, iterable, param parSafe=false)
    where canResolveMethod(iterable, "these") lifetime this < iterable {
      _checkElementType(eltType); 

      this.eltType = eltType;
      this.parSafe = parSafe;
      this.complete();

      for elem in iterable do _addElem(elem);
    }

    /*
      Initialize this set with a copy of each of the elements contained in
      the set `other`. This set will inherit the `parSafe` value of the
      set `other`.

      :arg other: A set to initialize this set with.
    */
    proc init=(const ref other: set(?t, ?)) lifetime this < other {
      this.eltType = t;
      this.parSafe = other.parSafe;
      this.complete();

      if !isCopyableType(eltType) then
        compilerError('Cannot initialize ' + this.type:string + ' from ' +
                      other.type:string + ' because element type ' +
                      eltType:string + ' is not copyable');

      for elem in other do _addElem(elem);
    }

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

    /*
      Add a copy of the element `x` to this set. Does nothing if this set
      already contains an element equal to the value of `x`.

      :arg x: The element to add to this set.
    */
    proc ref add(in x: eltType) lifetime this < x {

      // Remove `on this` block because it prevents copy elision of `x` when
      // passed to `_addElem`. See #15808.
      _enter(); defer _leave();
      _addElem(x);
    }

    /*
      Returns `true` if the given element is a member of this set, and `false`
      otherwise.

      :arg x: The element to test for membership.
      :return: Whether or not the given element is a member of this set.
      :rtype: `bool`
    */
    proc const contains(const ref x: eltType): bool {
      var result = false;

      on this {
        _enter(); defer _leave();
        var (hasFoundSlot, _) = _htb.findFullSlot(x);
        result = hasFoundSlot;
      }

      return result;
    }

    /*
      Returns `true` if this set shares no elements in common with the set
      `other`, and `false` otherwise.

      :arg other: The set to compare against.
      :return: Whether or not this set and `other` are disjoint.
      :rtype: `bool`
    */
    proc const isDisjoint(const ref other: set(eltType, ?)): bool {
      var result = true;

      on this {
        _enter(); defer _leave();

        if !(size == 0 || other.size == 0) {

          // TODO: Take locks on other?
          for x in other do
            if this.contains(x) {
              result = false;
              break;
            }
        }
      }

      return result;
    }

    /*
      Returns `true` if this set and `other` have at least one element in
      common, and `false` otherwise.

      :arg other: The set to compare against.
      :return: Whether or not this set and `other` intersect.
      :rtype: `bool`
    */
    proc const isIntersecting(const ref other: set(eltType, ?)): bool {
      return !isDisjoint(other);
    }

    /*
      Attempt to remove the item from this set with a value equal to `x`. If
      an element equal to `x` was removed from this set, return `true`, else
      return `false` if no such value was found.

      .. warning::

        Removing an element from this set may invalidate existing references
        to the elements contained in this set.

      :arg x: The element to remove.
      :return: Whether or not an element equal to `x` was removed.
      :rtype: `bool`
    */
    proc ref remove(const ref x: eltType): bool {
      var result = false;

      on this {
        _enter(); defer _leave();

        var (hasFoundSlot, idx) = _htb.findFullSlot(x);

        if hasFoundSlot {
          // TODO: Return the removed element? #15819
          var key: eltType;
          var val: nothing;

          _htb.clearSlot(idx, key, val);
          _htb.maybeShrinkAfterRemove();
          result = true;
        }
      }

      return result;
    }

    /*
      Clear the contents of this set.

      .. warning::

        Clearing the contents of this set will invalidate all existing
        references to the elements contained in this set.
    */
    proc ref clear() {
      on this {
        _enter(); defer _leave();

        for idx in 0..#_htb.tableSize {
          if _htb.isSlotFull(idx) {
            var key: eltType;
            var val: nothing;
            _htb.clearSlot(idx, key, val);
          }
        }

        _htb.maybeShrinkAfterRemove();
      }
    }

    /*
      Iterate over the elements of this set. Yields constant references
      that cannot be modified.

      .. warning::

        Modifying this set while iterating over it may invalidate the
        references returned by an iterator and is considered undefined
        behavior.
      
      :yields: A constant reference to an element in this set.
    */
    iter const these() {
      for idx in 0..#_htb.tableSize do
        if _htb.isSlotFull(idx) then yield _htb.table[idx].key;
    }

    pragma "no doc"
    iter const these(param tag) where tag == iterKind.standalone {
      var space = 0..#_htb.tableSize;
      for idx in space.these(tag) do
        if _htb.isSlotFull(idx) then yield _htb.table[idx].key;
    }

    pragma "no doc"
    iter const these(param tag) where tag == iterKind.leader {
      var space = 0..#_htb.tableSize;
      for followThis in space.these(tag) {
        yield followThis;
      }
    }

    pragma "no doc"
    iter const these(param tag, followThis)
    where tag == iterKind.follower {
      for idx in followThis(0) do
        if _htb.isSlotFull(idx) then yield _htb.table[idx].key;
    }

    /*
      Write the contents of this set to a channel.

      :arg ch: A channel to write to.
    */
    proc const writeThis(ch: channel) throws {
      on this {
        _enter(); defer _leave();

        var count = 1;
        ch <~> "{";

        for x in this {
          if count <= (_htb.tableNumFullSlots - 1) {
            count += 1;
            ch <~> x <~> ", ";
          } else {
            ch <~> x;
          }
        }

        ch <~> "}";
      }
    }

    /*
      Returns `true` if this set contains zero elements.

      :return: `true` if this set is empty.
      :rtype: `bool`
    */
    inline proc const isEmpty(): bool {
      var result = false;

      on this {
        _enter(); defer _leave();
        result = _htb.tableNumFullSlots == 0;
      }

      return result;
    }

    /*
      The current number of elements contained in this set.
    */
    inline proc const size {
      var result = 0;

      on this {
        _enter(); defer _leave();
        result = _htb.tableNumFullSlots;
      }

      return result;
    }

    /*
      Returns a new DefaultRectangular array containing a copy of each of the
      elements contained in this set. The elements of the returned array are
      not guaranteed to follow any particular ordering.

      :return: An array containing a copy of each of the elements in this set.
      :rtype: `[] eltType`
    */
    proc const toArray(): [] eltType {
      // May take locks non-locally...
      _enter(); defer _leave();

      var result: [0..#_htb.tableNumFullSlots] eltType;

      if !isCopyableType(eltType) then
        compilerError('Cannot create array because set element type ' +
                      eltType:string + ' is not copyable');

      on this {
        if _htb.tableNumFullSlots != 0 {
          var count = 0;
          var array: [0..#_htb.tableNumFullSlots] eltType;

          for x in this {
            array[count] = x;
            count += 1;
          }

          result = array;
        }
      }

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
  proc |(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t) {
    var result: set(t, (a.parSafe || b.parSafe));

    result = a;
    result |= b;

    return result;
  }

  /*
    Add to the set `lhs` all the elements of `rhs`.

    :arg lhs: A set to take the union of and then assign to.
    :arg rhs: A set to take the union of.
  */
  proc |=(ref lhs: set(?t, ?), const ref rhs: set(t, ?)) {
    for x in rhs do
      lhs.add(x);
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
    Add to the set `lhs` all the elements of `rhs`.

    :arg lhs: A set to take the union of and then assign to.
    :arg rhs: A set to take the union of.
  */
  proc +=(ref lhs: set(?t, ?), const ref rhs: set(t, ?)) {
    lhs |= rhs;
  }

  /*
    Return a new set that contains the difference of two sets.

    :arg a: A set to take the difference of.
    :arg b: A set to take the difference of.

    :return: A new set containing the difference between `a` and `b`.
    :rtype: `set(t)`
  */
  proc -(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t) {
    var result = new set(t, (a.parSafe || b.parSafe));

    if a.parSafe && b.parSafe {
      forall x in a do
        if !b.contains(x) then
          result.add(x);
    } else {
      for x in a do
        if !b.contains(x) then
          result.add(x);
    }

    return result;
  }

  /*
    Remove from the set `lhs` the elements of `rhs`.

    .. warning::

      This will invalidate any references to elements previously contained in
      the set `lhs`.

    :arg lhs: A set to take the difference of and then assign to.
    :arg rhs: A set to take the difference of.
  */
  proc -=(ref lhs: set(?t, ?), const ref rhs: set(t, ?)) {
    if lhs.parSafe && rhs.parSafe {
      forall x in rhs do
        lhs.remove(x);
    } else {
      for x in rhs do
        lhs.remove(x);
    }
  }

  /*
    Return a new set that contains the intersection of two sets.

    :arg a: A set to take the intersection of.
    :arg b: A set to take the intersection of.

    :return: A new set containing the intersection of `a` and `b`.
    :rtype: `set(t)`
  */
  proc &(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t) {
    var result: set(t, (a.parSafe || b.parSafe));

    /* Iterate over the smaller set */
    if a.size <= b.size {
      if a.parSafe && b.parSafe {
        forall x in a do
          if b.contains(x) then
            result.add(x);
      } else {
        for x in a do
          if b.contains(x) then
            result.add(x);
      }
    } else {
      if a.parSafe && b.parSafe {
        forall x in b do
          if a.contains(x) then
            result.add(x);
      } else {
        for x in b do
          if a.contains(x) then
            result.add(x);
      }
    }

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
    /* We can't remove things from lhs while iterating over it, so
     * use a temporary. */
    var result: set(t, (lhs.parSafe || rhs.parSafe));

    if lhs.parSafe && rhs.parSafe {
      forall x in lhs do
        if rhs.contains(x) then
          result.add(x);
    } else {
      for x in lhs do
        if rhs.contains(x) then
          result.add(x);
    }
    lhs = result;
  }

  /*
    Return the symmetric difference of two sets.

    :arg a: A set to take the symmetric difference of.
    :arg b: A set to take the symmetric difference of.

    :return: A new set containing the symmetric difference of `a` and `b`.
    :rtype: `set(?t, ?)`
  */
  proc ^(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t) {
    var result: set(t, (a.parSafe || b.parSafe));

    /* Expect the loop in ^= to be more expensive than the loop in =,
       so arrange for the rhs of the ^= to be the smaller set. */
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
    Assign to the set `lhs` the set that is the symmetric difference of `lhs`
    and `rhs`.

    .. warning::

      This will invalidate any references to elements previously contained in
      the set `lhs`.

    :arg lhs: A set to take the symmetric difference of and then assign to.
    :arg rhs: A set to take the symmetric difference of.
  */
  proc ^=(ref lhs: set(?t, ?), const ref rhs: set(t, ?)) {
    if lhs.parSafe && rhs.parSafe {
      forall x in rhs {
        if lhs.contains(x) {
          lhs.remove(x);
        } else {
          lhs.add(x);
        }
      }
    } else {
      for x in rhs {
        if lhs.contains(x) {
          lhs.remove(x);
        } else {
          lhs.add(x);
        }
      }
    }
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

    if a.parSafe && b.parSafe {
      forall x in a do
        if !b.contains(x) then
          result = false;
    } else {
      for x in a do
        if !b.contains(x) then
          return false;
    }

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
    if a.parSafe && b.parSafe {
      forall x in a do
        if !b.contains(x) then
          result = false;
    } else {
      for x in a do
        if !b.contains(x) then
          return false;
    }

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

    if a.parSafe && b.parSafe {
      forall x in b do
        if !a.contains(x) then
          result = false;
    } else {
      for x in b do
        if !a.contains(x) then
          return false;
    }

    return result;
  }

} // End module "Set".
