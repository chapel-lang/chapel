/*
 * Copyright 2004-2017 Cray Inc.
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

    The documentation for the Collection modules are being incrementally revised and improved.
*/

module Collection {
  class CollectionImpl {
    /*
      The type of element that this Collection holds.
    */
    type eltType;

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

      **Warning:** While this method will add as many items as possible, it will `break`
      when it is unable to consume more elements. Due to the fact that using `break` in
      an iterator will cause a memory leak and potentially leave this data structure in
      a undefined state, this should not be used to append another data structure to a
      bounded structure unless it is known it will succeed.
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

      **BUG:** Compiler will segfault if the returned value is not captured at callsite.
      Issue: #6542

      **FIX:** Ensure that you always capture the return value...

      ::

        var capturedRetval = c.remove()

      **BUG:** Loop Invariant Code Motion causes undefined behavior if assigned to a
      variable declared outside of loop. Issue: #7003

      **FIX:** Use the `--no-loop-invariant-code-motion` to disable LICM.
      Otherwise, just make sure you always capture the return value inside of a loop
      in a variable not declared outside of loop...

      .. code-block:: chapel

        for i in 1 .. N {
          var retval = c.remove();
        }
      
      **BUG:** Sometimes the compiler will produce an internal error related to 'visibility blocks',
      this can be resolved by declaring the return type rather than letting the compiler infer it.
      The exact cause has yet to be found.

      .. code-block:: chapel
  
        var retval : (bool, eltType) = c.remove();

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

      for 1 .. nElts {
        var (hasElt, elt) = remove();
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
      while true {
        var (hasElt, elt) = remove();
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

    /*
      Syntactic sugar for `getSize`.
    */
    proc length : int {
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

      **BUG:** Compiler does not currently allow overriding standalone or leader/follower
      iterators, and as such only serial iterators may be used with the base type. See
      issue #6998

      **BUG:** Resources are not properly cleaned up when the user breaks or returns
      from a serial iterator, and so this *must* be avoided at all cost. See issue #6912
    */
    iter these() : eltType {
      halt("'iter these() : eltType' is not supported...");
      yield _defaultOf(eltType);
    }
  }

  /*
    Syntactic sugar for 'add'.

    **BUG:** Compiler produces a warning that `c` should be a `ref`, when it is not needed
    because it is a class and can be used by value. Need a pragma to disable this?
    As such, this cannot be used where `ref` intents cannot be used, such as ``forall``
    and ``coforall`` loops.
  */
  inline proc +=(ref c : CollectionImpl(?eltType), elt : eltType) {
    c.add(elt);
  }
}