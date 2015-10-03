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

  /* list destructor

     It simply calls the destroy() method. */
  proc ~list() { destroy(); }

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
    Delete every node in the list.
   */
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
  proc writeThis(f: Writer) {
    var first: bool = true;
    for e in this {
      if !first then
        f.write(" ");
      else
        first = false;
      f.write(e);
    }
  }
}

pragma "auto copy fn"
proc chpl__autoCopy(rhs:list) {
  var result: rhs.type;

  // Can't do this because _getIterator() calls autoCopy on the list to put a
  // copy of it into the iterator class.  Heh.
  //  for e in rhs do
  //    result.append(e);
  // So ... the body of these() is inlined here:
  var current = rhs.first;
  while (current != nil) {
    result.append(current.data);
    current = current.next;
  }

  return result;
}

// TODO: Remove the dependence of tuple construction on the initcopy function,
// and then remove this override.
pragma "init copy fn"
proc chpl__initCopy(rhs:list) {
  return chpl__autoCopy(rhs);
}

proc =(ref a:list, b:list) {
  a.destroy();
  a.concat(b);
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
