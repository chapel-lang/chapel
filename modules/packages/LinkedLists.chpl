/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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
  This module provides a simple singly linked list.

  .. note::

      This module is expected to change in the future.
 */
module LinkedLists {


@chpldoc.nodoc
class listNode {
  type eltType;
  var data: eltType;
  var next: unmanaged listNode(eltType)?;
}


  @chpldoc.nodoc
  operator LinkedList.=(ref l1: LinkedList(?t), const ref l2: LinkedList(?t2)) {
    l1.destroy();
    for i in l2 do
      l1.append(i);
  }

  use IO;

/*
  A singly linked list.

  .. note::

      :proc:`~LinkedList.destroy` must be called to reclaim any memory used by the list.

 */
record LinkedList {
  /*
    The type of the data stored in every node.
   */
  type eltType;
  pragma "owned"
  @chpldoc.nodoc
  var _first: unmanaged listNode(eltType)?;
  pragma "owned"
  @chpldoc.nodoc
  var _last: unmanaged listNode(eltType)?;
  /*
    The number of nodes in the list.
   */
  var size: int;

  @chpldoc.nodoc
  proc init(type eltType, first : unmanaged listNode(eltType)? = nil, last : unmanaged listNode(eltType)? = nil) {
    this.eltType = eltType;
    this._first = first;
    this._last = last;
  }

  @chpldoc.nodoc
  proc init=(l : this.type) {
    this.eltType = l.eltType;
    this.complete();
    for i in l do
      this.append(i);
  }

  /*
    Iterate over the list, yielding each element.

    :ytype: eltType
   */
  iter these() {
    var tmp = _first;
    while tmp != nil {
      yield tmp!.data;
      tmp = tmp!.next;
    }
  }

  /*
    Append `e` to the list.
   */
  proc append(e : eltType) {
    if _last {
      _last!.next = new unmanaged listNode(eltType, e);
      _last = _last!.next;
    } else {
      _first = new unmanaged listNode(eltType, e);
      _last = _first;
    }
    size += 1;
  }
  /*
     Synonym for append.
   */
  inline proc push_back(e : eltType) {
    append(e);
  }

  /*
    Append all of the supplied arguments to the list.
   */
  proc append(e: eltType, es: eltType ...?k) {
    //TODO: merge the append overloads
    append(e);
    for param i in 0..k-1 do
      append(es(i));
  }

  /*
    Prepend `e` to the list.
   */
  proc prepend(e : eltType) {
    _first = new unmanaged listNode(eltType, e, _first);
    if _last == nil then
      _last = _first;
    size += 1;
  }

  /*
     Synonym for prepend.
   */
  inline proc push_front(e : eltType) {
    prepend(e);
  }


  /*
    Append all the elements in `l` to the end of the list.
   */
  proc concat(l: LinkedList(eltType)) {
    for e in l do
      append(e);
  }

  /*
    Remove the first encountered instance of `x` from the list.
    Does nothing if `x` is not present in the list.
   */
  proc remove(x: eltType) {
    var tmp = _first,
        prev: _first.type = nil;
    while tmp != nil && tmp!.data != x {
      prev = tmp;
      tmp = tmp!.next;
    }
    if tmp != nil {
      if prev != nil then
        prev!.next = tmp!.next;
      if _first == tmp then
        _first = tmp!.next;
      if _last == tmp then
        _last = prev;
      delete tmp;
      size -= 1;
    }
  }

  /*
     Remove the first element from the list and return it.
     It is an error to call this function on an empty list.
   */
   proc pop_front():eltType {
     import HaltWrappers;
     if boundsChecking && size < 1 {
       HaltWrappers.boundsCheckHalt("pop_front on empty list");
     }
     var oldfirst = _first!;
     var newfirst = _first!.next;
     var ret = oldfirst.data;
     _first = newfirst;
     if _last == oldfirst then _last = newfirst;
     size -= 1;
     delete oldfirst;
     return ret;
   }

  /*
    Returns true if this list contains an element equal to the value of
    e. Returns false otherwise.

    :arg e: The element search for
    :return: `true` if the `e` was found
    :rtype: `bool`
   */
  proc contains(const e: eltType): bool {
    for item in this {
      if (e == item) {
        return true;
      }
    }
    return false;
  }

  /*
    Returns a reference to the first item in the list

    .. warning::
      Calling this method on an empty list will cause the currently running
      program to halt. If the --fast flag is used, no safety checks will be
      performed

    :return: a reference to the first item in the list
    :rtype: `ref eltType`
   */
  proc ref first() ref: eltType {
    import HaltWrappers;
     if boundsChecking && size < 1 {
       HaltWrappers.boundsCheckHalt("called LinkedList.front on empty list");
     }
     return _first!.data;
  }

  /*
    Returns a reference to the last item in the list

    .. warning::
      Calling this method on an empty list will cause the currently running
      program to halt. If the --fast flag is used, no safety checks will be
      performed

    :return: a reference to the last item in the list
    :rtype: `ref eltType`
   */
  proc ref last() ref: eltType {
    import HaltWrappers;
     if boundsChecking && size < 1 {
       HaltWrappers.boundsCheckHalt("called LinkedList.last on empty list");
     }
    return _last!.data;
  }

  /*
    Delete every node in the list.
   */
  proc destroy() {
    var current = _first;
    while (current != nil) {
      var next = current!.next;
      delete current;
      current = next;
    }
    _first = nil;
    _last = nil;
    size = 0;
  }

  /*
    Destructor
   */
  @chpldoc.nodoc
  proc deinit(){
    destroy();
  }

  @chpldoc.nodoc
  proc writeThis(f) throws {
    var binary = f.binary();
    var arrayStyle = f.styleElement(QIO_STYLE_ELEMENT_ARRAY);
    var isspace = arrayStyle == QIO_ARRAY_FORMAT_SPACE && !binary;
    var isjson = arrayStyle == QIO_ARRAY_FORMAT_JSON && !binary;
    var ischpl = arrayStyle == QIO_ARRAY_FORMAT_CHPL && !binary;

    if binary {
      // Write the number of elements.
      f.write(size);
    }
    if isjson || ischpl {
      f._writeLiteral("[");
    }

    var first = true;
    for e in this {
      if first then first = false;
      else {
        if isspace then f._writeLiteral(" ");
        else if isjson || ischpl then f._writeLiteral(", ");
      }

      f.write(e);
    }

    if isjson || ischpl {
      f._writeLiteral("]");
    }

  }

  @chpldoc.nodoc
  proc readThis(f) throws {
    use OS;

    //
    // Special handling for reading in order to handle reading an arbitrary
    // size.
    //
    const isBinary = f.binary();
    const arrayStyle = f.styleElement(QIO_STYLE_ELEMENT_ARRAY);
    const isSpace = arrayStyle == QIO_ARRAY_FORMAT_SPACE && !isBinary;
    const isJson = arrayStyle == QIO_ARRAY_FORMAT_JSON && !isBinary;
    const isChpl = arrayStyle == QIO_ARRAY_FORMAT_CHPL && !isBinary;

    // How many elements should we read (for binary mode)?
    const num : int = if isBinary then f.read(int) else 0;

    if isJson || isChpl then f._readLiteral("[");

    // Clear out existing elements in the list.
    destroy();

    var isFirst = true;
    var hasReadEnd = false;
    var i = 0;

    while !hasReadEnd {
      if isBinary {
        if i >= num then break;
        continue;
      }

      if isFirst {
        isFirst = false;

        // Try reading an end bracket. If we don't, then continue on.
        try {
          if isJson || isChpl {
            f._readLiteral("]");
          } else if isSpace {
            f._readNewline();
          }

          hasReadEnd = true;
          break;
        } catch err: BadFormatError {
          // Continue on if we didn't read an end bracket.
        }
      } else {

        // Try to read a space or a comma. Break if we don't.
        try {
          if isSpace {
            f._readLiteral(" ");
          } else if isJson || isChpl {
            f._readLiteral(",");
          }
        } catch err: BadFormatError {
          break;
        }
      }

      append(f.read(eltType));
      i += 1;
    }

    if !hasReadEnd then
      if isJson || isChpl then f._readLiteral("]");
  }

  // TODO: temporary implementation to get some tests passing, but needs to
  // go through the formatter eventually.
  proc init(type eltType, r: fileReader) throws {
    this.init(eltType);
    readThis(r);
  }
}

/*
  Initialize a new :record:`LinkedList` containing all of the supplied arguments.

  :arg x: Every argument must be of type `T`.
  :type x: `T`
  :rtype: LinkedList(T)
 */
proc makeList(x ...?k) {
  // TODO: could just be an initializer?
  var s: LinkedList(x(0).type);
  for param i in 0..k-1 do
    s.append(x(i));
  return s;
}

} // end module LinkedLists
