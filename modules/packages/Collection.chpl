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

  Summary
  _______

  A 'Collection' is an interface for a data structure that is a 'contract' between
  the user and implementer that it fulfills the following:

  1. Is safe parallel-safe, hence is safe to use across multiple tasks across multiple locales.
  2. Supports the basic operations that any data structure needs to be truly useful, that is:

    a. Insertion of an arbitrary element. From this, we can insert bulk arbitrary elements.
    b. Removal of an arbitrary element. From this, we can remove bulk arbitrary elements.
    c. Iteration over all elements. From this, we can perform lookups over all elements.

  From the standpoint of the user, who directly benefits, they obtain a very nice but minimal
  guarantee on the object they are using, and from the implementor's view, they get to the
  benefit from both implementing a well-design interface, and from getting some utility methods
  'for-free'.

  .. note::

    This module is a work in progress and may change in future releases.

  Bugs and Known Issues
  _____________________

  1.  Parallel iteration is not apart of the core Collections module as currently the Chapel compiler
      will produce an internal error. As such, the utility methods provided 'for-free' use serial iteration.
      This issue has been documented under issue `#6998 <https://github.com/chapel-lang/chapel/issues/6998>`_ . Couple this
      fact with the fact that ``break`` ing out of a serial iterator will result in resource leakage, where destructors
      are not called, hence Collections using some RAII-based resource cleanup will end up leaking and potentially leaving
      the Collection in an undefined state. This has been documented under issue `#6912 <https://github.com/chapel-lang/chapel/issues/6912>`_ .

  Methods
  _______
*/

module Collection {
  class CollectionImpl {
    /*
      The type of element that this Collection holds.
    */
    type eltType;

    proc init(type eltType) {
      this.eltType = eltType;

      complete();
    }

    /*
      Adds an element to this data structure.
    */
    proc add(elt : eltType) : bool {
      halt("'proc add(elt : eltType) : bool' is not supported...");
    }

    /*
      Add all elements in bulk to this data structure. If the data structure
      rejects an element, we cease to offer more. We return the number of elements
      successfully added to this data structure.
    */
    proc addBulk(elts) : int {
      var successful : int;
      for elt in elts {
        if !add(elt) {
          break;
        }

        successful += 1;
      }

      return successful;
    }

    /*
      Removes an arbitrary element from this data structure.
    */
    proc remove() : (bool, eltType) {
      halt("'proc remove() : (bool, eltType)' is not supported...");
    }

    /*
      Removes `nElts` elements from this data structure, returning them as an array.
      If the data structure fails to produce a new element, we cease and shrink the array
      to its appropriate size and return that.
    */
    proc removeBulk(nElts : int) {
      var dom = {0..#nElts};
      var arr : [dom] eltType;
      var idx = 0;

      var (hasElt, elt) : (bool, eltType);
      for 1 .. nElts {
        (hasElt, elt) = remove();
        if !hasElt {
          dom = {0..#idx};
          break;
        }

        arr[idx] = elt;
        idx += 1;
      }

      return arr;
    }

    /*
      Determine whether an element exists in this collection.
    */
    proc contains(elt : eltType) : bool {
      var found = false;

      // Iterate to find it... note that we cannot break early due to
      // issues with leaking during serial iteration.
      for e in these() {
        if e == elt {
          found = true;
        }
      }

      return found;
    }

    /*
      Clears all elements in this collection.
    */
    proc clear() {
      var (hasElt, elt) : (bool, eltType);
      while true {
        (hasElt, elt) = remove();
        if !hasElt {
          break;
        }
      }
    }

    /*
      Check if this data structure is empty.
    */
    proc isEmpty() : bool {
      return getSize() == 0;
    }

    /* Deprecated - please use :proc:`CollectionImpl.size`. */
    proc length : int {
      compilerWarning("'CollectionImpl.length' is deprecated - " +
                      "please use 'CollectionImpl.size' instead");
      return getSize();
    }

    /*
      Syntactic sugar for `getSize`.
    */
    proc size : int {
      return getSize();
    }

    /*
      Obtain the number of elements contained in this collection.
    */
    proc getSize() : int {
      halt("'proc size() : int' is not supported...");
    }

    /*
      Iterate over all elements in the data structure.
    */
    iter these() : eltType {
      halt("'iter these() : eltType' is not supported...");
      var default: eltType;
      yield default;
    }
  }

  /*
    Syntactic sugar for :proc:`CollectionImpl.add`.
  */
  inline proc +=(ref c : CollectionImpl(?eltType), elt : eltType) {
    c.add(elt);
  }
}
