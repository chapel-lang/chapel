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

class listNode {
  type eltType;
  var data: eltType;
  var next: listNode(eltType);
}

record list {
  type eltType;
  var first: listNode(eltType);
  var last: listNode(eltType);
  var length: int;

  proc destroy() {
    var current = first;
    while (current != nil) {
      var next = current.next;
      delete current;
      current = next;
    }
  }

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

  iter these() {
    var tmp = first;
    while tmp != nil {
      yield tmp.data;
      tmp = tmp.next;
    }
  }

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

  proc append(e: eltType, es: eltType ...?k) {
    append(e);
    for param i in 1..k do
      append(es(i));
  }

  proc prepend(e : eltType) {
    first = new listNode(eltType, e, first);
    if last == nil then
      last = first;
    length += 1;
  }

  proc concat(l: list(eltType)) {
    for e in l do
      append(e);
  }

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

proc makeList(x ...?k) {
  var s: list(x(1).type);
  for param i in 1..k do
    s.append(x(i));
  return s;
}
