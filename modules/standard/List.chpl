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
  This module contains the implementation of the list type.

  A list is a lightweight container similar to an array that is suitable for
  building up and iterating over a collection of elements in a structured
  manner.

  The highly parallel nature of Chapel means that great care should be taken
  when performing operations that may invalidate references to list elements.
  Inserts and removals into the middle of a list are example operations that
  may invalidate references. Appending an element to the end of a list will
  never invalidate references to elements contained in the list.

  The following operations may invalidate references to elements contained in
  a list:

      - insert
      - remove
      - sort
      - pop
      - clear

  Additionally, all references to list elements are invalidated when the list
  is deinitialized.

  Lists are not parallel safe by default, but can be made parallel safe by
  setting the param formal `parSafe` to true in any list constructor. When
  constructed from another list, the new list will inherit the parallel safety
  mode of its originating list.

  Inserts and removals into a list are O(n) worst case and should be performed
  with care. Appends into a list have an amortized speed of O(1). Indexing
  into a list is O(1).
*/
module List {
  include module Vector;
  include module MultiBuffer;
  private use Vector only vector;
  private use MultiBuffer only multiBuffer;
  private use Reflection;
  private use IO;

  enum listImpl {buffer, multibuffer};

  proc getTypeFromEnumVal(param val: listImpl, type eltType, param parSafe) type {
    if val == listImpl.buffer then return vector(eltType, parSafe);
    if val == listImpl.multibuffer then return multiBuffer(eltType, parSafe);
  }
  proc getInstanceFromEnumVal(param val: listImpl, type eltType, param parSafe, other) {
    if val == listImpl.buffer then return new vector(other, parSafe);
    if val == listImpl.multibuffer then return new multiBuffer(other, parSafe);
  }

  /* The default implementation to use */
  param defaultImpl = listImpl.multibuffer;

  record list {
    type eltType;
    param parSafe = false;
    param implType = defaultImpl;
    forwarding var instance: getTypeFromEnumVal(implType, eltType, parSafe);

    proc init(type eltType, param parSafe = false, param implType: listImpl = defaultImpl) {
      this.eltType = eltType;
      this.parSafe = parSafe;
      this.implType = implType;
    }

    proc init(other: [?d] ?t, param parSafe = false, param implType: listImpl = defaultImpl) {
      this.eltType = t;
      this.parSafe = parSafe;
      this.implType = implType;
      this.instance = getInstanceFromEnumVal(implType, eltType, parSafe, other);
    }

    proc init(other: range(?t), param parSafe = false, param implType: listImpl = defaultImpl) {
      this.eltType = t;
      this.parSafe = parSafe;
      this.implType = implType;
      this.instance = getInstanceFromEnumVal(implType, eltType, parSafe, other);
    }

    proc init=(other: list(this.type.eltType, ?p)) {
      this.eltType = this.type.eltType;
      this.parSafe = this.type.parSafe;
      this.implType = this.type.implType;

      this.instance = getInstanceFromEnumVal(implType, eltType, parSafe, other);
    }

    proc init=(other: [?d] this.type.eltType) {
      this.eltType = this.type.eltType;
      this.parSafe = this.type.parSafe;
      this.implType = this.type.implType;

      this.instance = getInstanceFromEnumVal(implType, eltType, parSafe, other);
    }

    proc init=(other: range(?t)) {
      this.eltType = this.type.eltType;
      this.parSafe = this.type.parSafe;
      this.implType = this.type.implType;

      this.instance = getInstanceFromEnumVal(implType, eltType, parSafe, other);
    }
    
    /* FIXME: Workaround for # */
    proc ref extend(const ref other: list(eltType)) {
      instance.extend(other);
    }

    proc readWriteThis(ch: channel) throws {
      ch <~> instance;
    }

    proc requestCapacity(size: int) where implType == listImpl.list {
      compilerError("List doesn't support requestCapacity");
    }
  };

  /*
    Clear the contents of this list, then extend this now empty list with the
    elements contained in another list.

    .. warning::

      This will invalidate any references to elements previously contained in
      `lhs`.

    :arg lhs: The list to assign to.
    :arg rhs: The list to assign from. 
  */
  proc =(ref lhs: list(?t, ?), rhs: list(t, ?)) {
    lhs.clear();
    lhs.extend(rhs);
  }

  /*
    Returns `true` if the contents of two lists are the same.

    :arg a: A list to compare.
    :arg b: A list to compare.

    :return: `true` if the contents of two lists are equal.
    :rtype: `bool`
  */
  proc ==(a: list(?t, ?), b: list(t, ?)): bool {
    if a.size != b.size then
      return false;

    //
    // TODO: Make this a forall loop eventually.
    //
    for i in 0..#(a.size) do
      if a[i] != b[i] then
        return false;

    return true;
  }

  /*
    Return `true` if the contents of two lists are not the same.

    :arg a: A list to compare.
    :arg b: A list to compare.

    :return: `true` if the contents of two lists are not equal.
    :rtype: `bool`
  */
  proc !=(a: list(?t, ?), b: list(t, ?)): bool {
    return !(a == b);
  }

} // End module "Lists".

