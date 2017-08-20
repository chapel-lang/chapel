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
    halt("'proc add(elts : eltType ... ?nElts) : bool' is not supported...");
  }

  /*
    Removes an arbitrary element from this data structure.

    **BUG:** Compiler will segfault if the returned value is not captured at callsite.

    **FIX:** Ensure that you always capture the return value...

    ::
    
      var capturedRetval = c.remove()

    **BUG:** Loop Invariant Code Motion causes undefined behavior if assigned to a
    variable declared outside of loop

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
    return size() == 0;
  }

  /*
    Obtain the number of elements contained in this collection.
  */
  inline proc size() : int {
    halt("'proc size() : int' is not supported...");
  }

  /*
    Iterate over all elements in the data structure.

    **BUG:** Compiler does not currently allow overloading standalone or leader/follower
    iterators, and as such only serial iterators may be used with the base type.
  */
  iter these() : eltType {
    halt("'iter these() : eltType' is not supported...");
    yield _defaultOf(eltType);
  }

  /*
    Declares this collection immutable.

    **NOTE:** This method may be removed from the core interface.
  */
  inline proc freeze() : bool {
    halt("'proc freeze() : bool' is not supported...");
  }

  /*
    Declares this collection mutable.

    **NOTE:** This method may be removed from the core interface.
  */
  inline proc unfreeze() : bool {
    halt("'proc freeze() : bool' is not supported...");
  }

  /*
    Whether this collection supports freezing.

    **NOTE:** This method may be removed from the core interface.
  */
  inline proc canFreeze() : bool {
    return false;
  }

  /*
    Determines if this collection is currently frozen.

    **NOTE:** This method may be removed from the core interface.
  */
  inline proc isFrozen() : bool {
    halt("'proc isFrozen() : bool' is not supported...");
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
