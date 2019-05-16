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
  (Word smithing module text below).

  ---

  This module contains the implementation of the List type.

  A List is a lightweight container similar to an array that is suitable for
  building up and iterating over a collection of elements in a structured
  manner.

  The highly parallel nature of Chapel means that great care should be taken
  when performing operations that may invalidate references to List elements.
  Inserts and removals into the middle of a List are example operations that
  may invalidate references. Appending an element to the end of a List will
  never invalidate references to elements contained in the List.

  The following operations may invalidate references to elements contained in
  a List:

      - insert
      - remove
      - reverse
      - sort
      - pop

  Additionally, all references to List elements are invalidated when the List
  is deinitialized.

  (Paragraph about parallel safety).

  Inserts and removals into a List are O(n) worst case and should be performed
  with care. For fast O(1) appending to either end consider the use of the
  Deque type instead. For O(1) inserts and removals during iteration, consider
  the use of the LinkedList type.

  ---

  (This text will NOT appear in final module description).

  I believe it is the intention of this new List datatype to replace all
  "array-as-vec" operations on arrays. That means the following procedures
  would no longer be supported (deprecated?, or removed?) on arrays:

    - proc push_back(in val: this.eltType)
    - proc push_back(vals: [])
    - proc pop_back()
    - proc push_front(in val: this.eltType)
    - proc push_front(vals: [])
    - proc pop_front()
    - proc insert(pos: this.idxType, in val: this.eltType)
    - proc insert(pos: this.idxType, vals: [])
    - proc remove(pos: this.idxType)
    - proc remove(pos: this.idxType, count: this.idxType)
    - proc remove(pos: range(this.idxType, stridable = false))
    - proc clear()

  All of the methods given above change an array's domain (exactly what we
  are trying to avoid, and the impetus for the List type).

*/
module Lists {

  // Building blocks used to store "chunks" of List elements.
  pragma "no doc"
  class ListBlock {
    type _etype;
    var data: _ddata(_etype) = nil;
  }

  /*
    A List is a lightweight container suitable for building up and iterating
    over a collection of elements in a structured manner. It is intended to
    replace the use of arrays to perform "vector-like" operations. Unlike a
    stack, the List type also supports inserts or removals into the middle of
    the List. The List type is close in spirit to its Python counterpart, with
    fast O(log n) (and O(1) eventually) indexing.

    The List type is parallel safe by default. For situations in which such
    overhead is undesirable, parallel safety can be disabled by setting
    `parSafe = false` in the List constructor. A List object constructed
    from another List object inherits the parallel safety mode of that list
    unless otherwise specified.
  */
  class List {

    /* The number of elements contained in this List. */
    var size = 0;

    /* The type of the elements contained in this List. */
    type eltType;

    /* If `true`, this List will use parallel safe operations. */
    param parSafe = true;

    //
    // Michael suggested that we move from a LL of blocks to an array, which
    // would give us constant speed indexing at the cost of more complicated
    // indexing logic.
    //
    // For now, I can start with Vass's logic and then switch over to a O(1)
    // indexing scheme when everything else is working.
    //
    pragma "no doc"
    var _head, _tail: unmanaged ListBlock(eltType) = nil;

    /*
      Initializes an empty List containing elements of the given type.

      :arg eltType: The type of the elements of this List.
      :arg parSafe: If `true`, this List will use parallel safe operations.
        Is `true` by default.
    */
    proc init(type eltType, param parSafe=true) {
      this.eltType = eltType;
    }

    /*
      Initializes a List containing elements that are copy initialized from
      the elements in the old List.

      :arg other: The List object to initialize from.
      :arg parSafe: If `true`, this List will use parallel safe operations.
        Inherits the value from the other List by default.
    */
    proc init=(other: List, param parSafe=other.parSafe) {
      this.eltType = other.eltType;
    }

    pragma "no doc"
    proc deinit() {}

    // This may not be necessary, so don't fixate too much on it.
    pragma "no doc"
    proc _cast(type t: string, x: List): string {
      return "";
    }

    /*
      Add an element to the end of this List.

      :arg x: An element to append.
    */
    proc append(in x: eltType) {}

    /*
      Extend this List by appending a copy of each element contained in
      another List.

      :arg other: A List containing elements of the same type as those
        contained in this List.
    */
    proc extend(other: List(eltType)) {}

    /*
      Extend this List by appending a copy of each element contained in an
      array.

      :arg other: An array containing elements of the same type as those
        contained in this List.
    */
    proc extend(other: [?d] eltType) {}

    /*
      Insert an element at a given position in this List, shifting all elements
      following that index one to the right.

      .. warn::
      
        Inserting an element into this List may invalidate existing references
        to the elements contained in this List.

      :arg i: The index of the element at which to insert.
      :arg x: The element to insert.

      :throws IllegalArgumentError: If the given index is out of bounds.
    */
    proc insert(i: int, in x: eltType) throws {}

    /*
      Remove the first item from this List whose value is equal to x, shifting
      all elements following the removed item one to the left.

      .. warn::

        Removing an element from this List may invalidate existing references
        to the elements contained in this List.

      :arg x: The element to remove.

      :throws IllegalArgumentError: If there is no such item.
    */
    proc remove(x: eltType) throws {}

    /*
      Remove the item at the given position in this List, and return it. If no
      index is specified, remove and return the last item in this List.

      .. warn::

        Popping an element from this List will invalidate any reference to
        the element taken while it was contained in this List.

      :arg i: The index of the element to remove. Defaults to the last item
        in this List.

      :return: The item removed.

      :throws IllegalArgumentError: If the given index is out of bounds.
    */
    proc pop(i: int=size - 1): eltType throws {
      return nil;
    }

    /*
      Clear the contents of this List.

      .. warn::

        Clearing the contents of this List will invalidate all existing
        references to the elements contained in this List.
    */
    proc clear() {}

    /*
      Return a zero-based index into this List of the first item whose value
      is equal to x.

      :arg x: An element to search for.

      :return: The index of the element to search for.

      :throws IllegalArgumentError: If the given element cannot be found.
    */
    proc indexOf(x: eltType, start: int=0, end: int=size): int throws {
      return 0; 
    }

    /*
      Return the number of times a given element is found in this List.

      :arg x: An element to count.

      :return: The number of times a given element is found in this List.
      :rtype: `int`
    */
    proc count(x: eltType): int {
      return 0;
    }

    /*
       Sort the elements of this List in place using their default sort order.

       .. warn::

        Sorting the contents of this List may invalidate existing references
        to the elements contained in this List.
    */
    proc sort() {}

    /*
      Sort the items of this List in place using a comparator.

      .. warn::

        Sorting the elements of this List may invalidate existing references
        to the elements contained in this List.

      :arg comparator: A comparator object used to sort this List.

    */
    proc sort(comparator) {}

    /*
      Index this List via subscript. Returns a reference to the element at a
      given index in this List.

      :arg i: The index of the element to access.

      :return: An element from this List.

      :throws IllegalArgumentError: If the given index is out of bounds.
    */
    proc this(i: int) ref throws {
      return nil;
    }

    /*
      Produce a serial iterator over the elements of this List.

      :yields: A reference to one of the elements contained in this List.
    */
    iter these() ref {
      yield size;
    }

    /*
      Write the contents of this List to a channel.

      :arg ch: A channel to write to.
    */
    proc writeThis(ch: channel) {}

    /*
      Returns `true` if this List contains zero elements.

      :return: `true` if this List is empty.
      :rtype: `bool`
    */
    proc isEmpty(): bool {
      return true;
    }

    /*
      Returns a new DefaultRectangular array containing a copy of each of the
      elements contained in this List.

      :return: A new DefaultRectangular array.
    */
    proc toArray(): [] eltType {
      return nil;
    }

  } // End class List!

  /*
    (This is a question for debate, not an actual docstring).

    Should the List type support a deep copy on assignment (IE, an overload of
    the `=` operator along with making it a record)?

    IE, when I assign L2 to L1, the contents of L1 are now a copy of the
    elements contained in L2.
  */
  proc =(a: List(?t), b: List(t)) {}

  /*
    Returns `true` if the contents of two Lists are the same.

    :arg a: A List to compare.
    :arg b: A List to compare.

    :return: `true` if the contents of two Lists are the same.
    :rtype: `bool`
  */
  proc ==(a: List(?t), b: List(t)): bool {
    return true;
  }

} // End module Lists!

