/*
 * Copyright 2004-2015 Cray Inc.
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
module List {

pragma "no doc"
class listNode {
  type eltType;
  var data: eltType;
  var next: listNode(eltType);
}


/*
  A singly linked list.

  .. note::

      :proc:`~list.destroy` must be called to reclaim any memory used by the list.

 */
record list {
  /*
    The type of the data stored in every node.
   */
  type eltType;
  pragma "no doc"
  var first: listNode(eltType);
  pragma "no doc"
  var last: listNode(eltType);
  /*
    The number of nodes in the list.
   */
  var length: int;

  /*
     Synonym for length.
   */
  proc size {
    return length;
  }

  /*
    Iterate over the list, yielding each element.

    :ytype: eltType
   */
  iter these() {
    var tmp = first;
    while tmp != nil {
      yield tmp.data;
      tmp = tmp.next;
    }
  }

  /*
    Append `e` to the list.
   */
  proc append(e : eltType) {
    if last {
      last.next = new listNode(eltType, e);
      last = last.next;
    } else {
      first = new listNode(eltType, e);
      last = first;
    }
    length += 1;
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
  //TODO: merge the append overloads
  proc append(e: eltType, es: eltType ...?k) {
    append(e);
    for param i in 1..k do
      append(es(i));
  }

  /*
    Prepend `e` to the list.
   */
  proc prepend(e : eltType) {
    first = new listNode(eltType, e, first);
    if last == nil then
      last = first;
    length += 1;
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
  proc concat(l: list(eltType)) {
    for e in l do
      append(e);
  }

  /*
    Remove the first encountered instance of `x` from the list.
   */
  proc remove(x: eltType) {
    var tmp = first,
        prev: first.type = nil;
    while tmp != nil && tmp.data != x {
      prev = tmp;
      tmp = tmp.next;
    }
    if tmp != nil {
      if prev != nil then
        prev.next = tmp.next;
      if first == tmp then
        first = tmp.next;
      if last == tmp then
        last = prev;
      delete tmp;
      length -= 1;
    }
  }

  /*
     Remove the first element from the list and return it.
     It is an error to call this function on an empty list.
   */
 proc pop_front():eltType {
   if length < 1 then halt("pop_front on empty list");
   var oldfirst = first;
   var newfirst = first.next;
   var ret = oldfirst.data;
   first = newfirst;
   if last == oldfirst then last = newfirst;
   length -= 1;
   return ret;
 }

  /*
    Delete every node in the list.
   */
  // TODO: call from a destructor?
  proc destroy() {
    var current = first;
    while (current != nil) {
      var next = current.next;
      delete current;
      current = next;
    }
    first = nil;
    last = nil;
    length = 0;
  }

  pragma "no doc"
  proc writeThis(f) {
    var binary = f.binary();
    var arrayStyle = f.styleElement(QIO_STYLE_ELEMENT_ARRAY);
    var isspace = arrayStyle == QIO_ARRAY_FORMAT_SPACE && !binary;
    var isjson = arrayStyle == QIO_ARRAY_FORMAT_JSON && !binary;
    var ischpl = arrayStyle == QIO_ARRAY_FORMAT_CHPL && !binary;
 
    if binary {
      // Write the number of elements.
      f <~> length;
    }
    if isjson || ischpl {
      f <~> new ioLiteral("[");
    }

    var first = true;
    for e in this {
      if first then first = false;
      else {
        if isspace then f <~> new ioLiteral(" ");
        else if isjson || ischpl then f <~> new ioLiteral(",");
      }

      f <~> e;
    }

    if isjson || ischpl {
      f <~> new ioLiteral("]");
    }

  }
  
  pragma "no doc"
  proc readThis(f) {
    // Special handling for reading in order to handle
    // reading an arbitrary length.
    var binary = f.binary();
    var arrayStyle = f.styleElement(QIO_STYLE_ELEMENT_ARRAY);
    var isspace = arrayStyle == QIO_ARRAY_FORMAT_SPACE && !binary;
    var isjson = arrayStyle == QIO_ARRAY_FORMAT_JSON && !binary;
    var ischpl = arrayStyle == QIO_ARRAY_FORMAT_CHPL && !binary;

    // How many elements should we read (for binary mode)?
    var num = 0;

    if binary {
      // Read the number of elements.
      f <~> num;
    }
    if isjson || ischpl {
      f <~> new ioLiteral("[");
    }

    if ! f.error() {
      // Clear out existing elements in the list
      destroy();
    }

    var first = true;
    var i = 0;
    var read_end = false;

    while ! f.error() {
      if binary {
        // Read only num elements.
        if i >= num then break;
      } else {
        if first {
          first = false;
          // but check for a ]
          if isjson || ischpl {
            f <~> new ioLiteral("]");
          } else if isspace {
            f <~> new ioNewline(skipWhitespaceOnly=true);
          }
          if f.error() == EFORMAT {
            f.clearError();
          } else {
            read_end = true;
            break;
          }
        } else {
          // read a comma or a space.
          if isspace then f <~> new ioLiteral(" ");
          else if isjson || ischpl then f <~> new ioLiteral(",");

          if f.error() == EFORMAT {
            f.clearError();
            // No comma.
            break;
          }
        }
      }

      var elt:eltType;

      // read the element
      f <~> elt;
      
      // add it to the list
      append(elt);

      i += 1;
    }

    if ! read_end {
      if isjson || ischpl {
        f <~> new ioLiteral("]");
      }
    }
  }
}

/*
  Construct a new :record:`list` containing all of the supplied arguments.

  :arg x: Every argument must be of type `T`.
  :type x: T
  :rtype: list(T)
 */
// TODO: could just be a constructor?
proc makeList(x ...?k) {
  var s: list(x(1).type);
  for param i in 1..k do
    s.append(x(i));
  return s;
}

} // end module List
