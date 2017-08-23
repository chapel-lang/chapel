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
  A 'Collection' is a data structure, a container for elements that provide support
  for insert, lookup, remove, and iteration operations.
*/
class Collection {
  /*
    The type of element that this Collection holds.
  */
  type eltType;

  /*
    Adds an element to this data structure.
  */
  inline proc add(elt : eltType) : bool {
    halt("'proc add(elt : eltType) : bool' is not supported...");
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

    ::

      for i in 1 .. N {
        var retval = c.remove();
      }

  */
  inline proc remove() : (bool, eltType) {
    halt("'proc remove() : (bool, eltType)' is not supported...");
  }

  /*
    Determine whether an element exists in this collection.
  */
  inline proc contains(elt : eltType) : bool {
    halt("'proc contains(elt : eltType) : bool' is not supported...");
  }

  /*
    Clears all elements in this collection.
  */
  inline proc clear() {
    while !remove()[1] do ;
  }

  /*
    Check if this data structure is empty.
  */
  inline proc isEmpty() : bool {
    return getSize() == 0;
  }

  /*
    Syntactic sugar for `getSize`.
  */
  inline proc length : int {
    return getSize();
  }

  /*
    Syntactic sugar for `getSize`.
  */
  inline proc size : int {
    return getSize();
  }

  /*
    Obtain the number of elements contained in this collection.
  */
  inline proc getSize() : int {
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
proc +=(ref c : Collection(?eltType), elt : eltType) {
  c.add(elt);
}
