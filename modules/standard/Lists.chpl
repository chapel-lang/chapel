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

  This module contains the implementation of a list datatype, intended to
  replace the use of arrays to perform "vector-like" operations.

  A List is a lightweight container intended to mimic Python's list datatype,
  with fast O(log n)/O(1) (whatever we end up on) indexing  and an API close
  in spirit to its Python counterpart.

  The highly parallel nature of Chapel means that great care should be taken
  when performing operations which may trigger a move of List elements.
  Inserts and removals into the middle of a List are example operations which
  may trigger a move, thus invalidating all references to elements of the
  List held by tasks before the move. Appending an element to the end of a
  List will never cause a move.

  The following operations may trigger a move:
    - insert
    - remove
    - reverse
    - sort
    - pop

  ---

  (End word smithing module text).

  I know that @mppf in particular is very interested in limiting the number of
  operations that move/displace List elements.

  This pertains to the general issue of invalidating references to (and also
  iterators over) List elements.

  There is also the separate, more general issue of parallel safety. I'd
  happily appreciate any/all recommendations for the "Chapeltastic" way of
  making this List type parallel safe.

  ---

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
    A container intended to replace the use of arrays to perform "vector-like"
    operations.

    Is not currently parallel safe.

    The following operations may trigger a move:
      - insert
      - remove
      - reverse
      - sort
      - pop

    A move will invalidate any references to List elements held by tasks, as
    well as iterators to List elements produced by tasks before the move
    occurred.
  */
  class List {

    pragma "no doc"
    var _size = 0;
    pragma "no doc"
    var _etype;

    //
    // Michael suggested that we move from a LL of blocks to an array, which
    // would give us constant speed indexing at the cost of more complicated
    // indexing logic.
    //
    // For now, I can start with Vass's logic and then switch over to a O(1)
    // indexing scheme when everything else is working.
    //
    pragma "no doc"
    var _head, _tail: unmanaged ListBlock(_etype) = nil;

    /*
      Initializes a new List containing elements of the given type.

      :arg etype: The type of the elements of this List.
    */
    proc init(type etype) {
      _etype = etype;
    }

    /*
      Initializes a new List as a copy of another List object.

      :arg other: The List object to initialize from.
    */
    proc init=(other: List) {
      this._etype = other._etype;
    }
  
    proc deinit() {}

    /*
      Add an element to the end of this List.

      .. note::

        Appending an element to a List will never invalidate references to
        elements in the List.

      :arg x: An element to append.
    */
    proc append(x: this._etype) {}

    /*
      Extend this List by appending all the elements from another List.

      :arg other: A List, the elements of which are appended to this List.
    */
    proc extend(other: List(this._etype)) {}

    /*
      Extend this List by appending all the elements from an array.

      :arg other: An array, the elements of which are appended to this List.
    */
    proc extend(other: this._etype[?d]) {}

    /*
      Insert an element at a given position in this List, shifting all elements
      following that index one to the right. Note that `a.insert(a.size, x)`
      is equivalent to `a.append(x)`.

      :arg i: The index of the element at which to insert.
      :arg x: The element to insert.

      :throws IllegalArgumentError: If the given index is out of bounds.
    */
    proc insert(i: int, x: this._etype) throws {}

    /*
      Remove the first item from this List whose value is equal to _x_.

      :arg x: The element to remove.

      :throws IllegalArgumentError: If there is no such item.
    */
    proc remove(x: this._etype) throws {}

    /*
      Remove the item at the given position in this List, and return it. If no
      index is specified, remove and return the last item in this List.

      :arg i: The index of the element to remove. Defaults to the last item
        in this List, or `a.remove(a.size - 1)`.
      
      :return: The item removed.

      :throws IllegalArgumentError: If the given index is out of bounds.
    */
    proc pop(i: int=this._size - 1): this._etype throws {
      return nil;
    }

    /*
      Clear the contents of this List.
    */
    proc clear() {}

    /*
      Return a zero-based index into this List of the first item whose value
      is equal to _x_.

      :arg x: An element to search for.

      :return: The index of the element to search for.

      :throws IllegalArgumentError: If the given element cannot be found.
    */
    proc indexOf(x: _etype, start: int=0, end: int=_size): int throws {
      return 0; 
    }

    /*
      Return the number of times _x_ appears in this List.

      :arg x: An element to count.

      :return: The number of times a given element is found in this List.
      :rtype: `int`
    */
    proc count(x: this._etype): int {
      return 0;
    }

    /*
      Sort the items of this List in place. The parameters of this method are
      holdovers from Python. This method could more closely integrate with
      Chapel sorting APIs.

      :arg key: Unused in current API.
      :arg reverse: True if this List should be sorted in reverse order.

    */
    proc sort(key: this._etype=nil, reverse: bool=false) {}

    /*
      Reverse the elements of this List in place.
    */
    proc reverse() {}

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
      :y
    */
    iter these() ref {
      yield _size;
    }

    /*
      Write the contents of this List to a given channel.

      :arg ch: A channel to write to.
    */
    proc writeThis(ch: channel) {}

    /*
      Convert the contents of this List to string form. Could be replaced by
      an overload of the cast operation?

      :return: The contents of this List, in a form suitable for printing.
      :rtype: `string`
    */
    proc toString(): string {
      return "";
    }

    /*
      Return the number of elements in this List.

      :return: The number of elements in this List.
      :rtype: `int`
    */
    proc size {
      return _size;
    }

    /*
      Returns `true` if this List contains zero elements.

      :return: `true` if this List is empty.
      :rtype: `bool`
    */
    proc isEmpty(): bool {
      return true;
    }

    /*
      Returns a new DefaultRectangular array containing the elements of this
      List.

      :return: A new DefaultRectangular array.
    */
    proc toArray(): this._etype[] {
      return nil;
    }
  }

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

  /*
    An important question here: do we want the semantics of addition to mirror
    Chapel arrays, or Python lists?

    Given Chapel semantics, `+` would be a "vectorlike" operation, performing
    scalar addition against all elements of this List, returning a new List
    as a result.

    Given Python semantics, `+` would be a shorthand for concatenating two
    Lists together, returning a new List as a result.

    :arg a: A List to add.
    :arg b: A List to add.

    :return: A new List whose elements are the concatenation of two Lists.
  */
  proc +(a: List(?t), b: List(t)): List(t) { return nil; }


} // End module Lists!

