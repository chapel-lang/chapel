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
    inline proc _destroy(ref item: eltType) {
      chpl__autoDestroy(item);
    }

    //
    // Getting weird lifetime errors when using this function over classes,
    // and I'm not sure quite how to solve them yet. Since this is used in a
    // managed way internally, try marking "unsafe" for now to circumvent
    // the errors, and see if we can deal with them later.
    //
    pragma "no doc"
    pragma "unsafe"
    inline proc _move(ref src: ?t, ref dst: t) {
      __primitive("=", dst, src);
    }

    pragma "no doc"
    proc _getRef(idx: int) ref {
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
    */
    proc add(in x: eltType) {
      return;
    }

    /*
      Returns `true` if the given element is a member of this set, and `false`
      otherwise.

      :arg x: The element to test for membership.
      :return: Whether or not the given element is a member of this set.
    */
    proc contains(const ref x: eltType): bool {
      return false;
    }

    /*
    */
    proc disjoint(other: set(eltType, ?)): bool {
      return false;
    }

    /*
    */
    proc intersect(other: set(eltType, ?)): bool {
      return false;
    }

    /*
    */
    proc subset(other: set(eltType, ?)): bool {
      return false;
    }

    /*
    */
    proc properSubset(other: set(eltType, ?)): bool {
      return false;
    }

    /*
    */
    proc superset(other: set(eltType, ?)): bool {
      return false;
    }

    /*
    */
    proc properSuperset(other: set(eltType, ?)): bool {
      return false;
    }

    /*
    */
    proc remove(const ref x: eltType) throws {
      return;
    }

    /*
    */
    proc discard(const ref x: eltType) {
      return;
    }

    /*
    */
    proc clear() {
      return;
    }

    /*
    */
    iter these() ref {
      return nil;
    }

    /*
    */
    proc writeThis(ch: channel) {
      return;
    }

    /*
    */
    inline proc const isEmpty(): bool {
      return false;
    }

    /*
    */
    inline proc const size {
      return 0;
    }

    /*
    */
    proc toArray(): [] eltType {
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
  */
  proc setUnion(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t, ?) {
    return new set(t, false);
  }

  /*
  */
  proc |(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t, ?) {
    return new set(t, false);
  }

  /*
  */
  proc setDifference(const ref a: set(?t, ?),
                     const ref b: set(t, ?)): set(t, ?) {
    return new set(t, false);
  }

  /*
  */
  proc -(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t, ?) {
    return new set(t, false);
  }

  /*
  */
  proc setIntersection(const ref a: set(?t, ?),
                     const ref b: set(t, ?)): set(t, ?) {
    return new set(t, false);
  }

  /*
  */
  proc &(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t, ?) {
    return new set(t, false);
  }

  /*
  */
  proc setSymmetricDiff(const ref a: set(?t, ?),
                     const ref b: set(t, ?)): set(t, ?) {
    return new set(t, false);
  }

  /*
  */
  proc ^(const ref a: set(?t, ?), const ref b: set(t, ?)): set(t, ?) {
    return new set(t, false);
  }

  /*
    Return `true` if `a` contains the same elements as `b`.
  */
  proc ==(const ref a: set(?t, ?), const ref b: set(t, ?)): bool {
    return false;
  }

  /*
    Return `true` if `a` is a proper subset of `b`.
  */
  proc <(const ref a: set(?t, ?), const ref b: set(t, ?)): bool {
    return false;
  }

  /*
    Return `true` if `a` is a subset of `b`.
  */
  proc <=(const ref a: set(?t, ?), const ref b: set(t, ?)): bool {
    return false;
  }

  /*
    Return `true` if `a` is a proper superset of `b`.
  */
  proc >(const ref a: set(?t, ?), const ref b: set(t, ?)): bool {
    return false;
  }

  /*
    Return `true` if `a` is a superset of `b`.
  */
  proc >=(const ref a: set(?t, ?), const ref b: set(t, ?)): bool {
    return false;
  }

} // End module "Sets".

