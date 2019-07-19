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

/* Support for VectorLists - collections with exponentially-growing capacity
   and without reallocating the existing elements upon push_back().
*/
module VectorLists {

  /* Default initial capacity of a new VectorList. */
  config const vectorListInitCapacity = 4;

  /* VectorList capacity growth factor. */
  config const vectorListGrowFactor = 2;

  /* VectorList capacity does not grow bigger than this. */
  config const vectorListMaxCapacity = 500000;

  /*
  VectorList is a collection that grows by push_back()-ing a single element
  at a time. It stores the elements in a linked list of contiguous regions
  of memory of exponentially-growing sizes. When the current capacity
  is exhausted, a new chunk of memory is appended to the list;
  already-accumulated elements are not moved.

  VectorList supports linear traversal of the entire collection
  with close-to-DR-array asymptotic speed.
  Supports log(size)-time random indexing.

  VectorList does not support push_front() and O(1)-time random indexing.
  These could be added in the future.
  */
  class VectorList {
    /* The type of the collection's elements.*/
    type eltType; 

    /* The number of elements presently in the collection. */
    var size = 0;

    // A singly-linked list of classes that store the elements.
    pragma "no doc"
    const head: unmanaged VectorListElement(eltType);

    // New elements are added here.
    pragma "no doc"
    var tail: head.type;

    /* Create an empty collection with the given element type. */
    inline proc init(type eltType, initCapacity = vectorListInitCapacity) {
      this.eltType = eltType;
      this.head = new unmanaged VectorListElement(eltType, initCapacity);
      this.tail = this.head;
    }

    proc deinit() {
      // We do not want "owned" for the VectorListElement links, i.e. this.head,
      // this.head.next, .next.next, etc. Because with "owned" these links would
      // be deinit-ed with recursive calls to VectorListElement.deinit(), one
      // call per list element. A long list would lead to a lot of stack frames.
      // Also, "owned" would not allow us to update this.tail in push_back().
      for LE in listElements() do
        delete LE;
    }      
  }

  /*
  Store 'size' elements in 'elements', with 'capacity'.
  Currently the backing store is allocated right away and does not change.
  */
  pragma "no doc"
  class VectorListElement {
    type eltType;
    var size: int;
    const capacity: int;
    const elements: _ddata(eltType);  // _ddata is not managed
    var next: unmanaged VectorListElement(eltType);

    inline proc init(type eltType, initCapacity: int) {
      this.eltType = eltType;
      this.size = 0;
      this.capacity = initCapacity;
      this.elements = _ddata_allocate(eltType, this.capacity);
      this.next = nil;
    }

    proc deinit() {
      // Deinit individual elements.
      for i in 0..#size do
        chpl__autoDestroy(elements[i]);

      _ddata_free(elements, capacity);
    }

    // 0-based indexing
    inline proc elemAt(idx: int) ref {
      return elements.this(idx);
    }
  }

  /* Add 'val' to the end of the collection */
  proc VectorList.push_back(in val: this.eltType) lifetime this < val {
    const last = this.tail;
    if boundsChecking then assert(last.next == nil);
    this.size += 1;

    if (last.size < last.capacity) {
      // Add to the currently-last VLE.
      last.elemAt(last.size) = val;
      last.size += 1;
      return;
    }

    // Otherwise append a new VLE.
    const nextCapacity =
      if last.capacity * vectorListGrowFactor > vectorListMaxCapacity
      then last.capacity else last.capacity * vectorListGrowFactor;

    var next = new unmanaged VectorListElement(eltType, nextCapacity);
    next.elemAt(0) = val;
    next.size = 1;

    // Update link pointers.
    last.next = next;
    this.tail = next;
  }

  /* Yields all VectorListElement. */
  inline iter VectorList.listElements() {
    var curr = head;
    do { const next = curr.next; yield curr; curr = next; }
    while curr != nil;
  }

  /* Yields pairs (starting index, VectorListElement pointer). */
  inline iter VectorList.links(): (int,unmanaged VectorListElement(eltType)) {
    if size == 0 then return; // no vector elements
    var currStart = 0;
    for LE in listElements() {
      yield (currStart, LE);
      currStart += LE.size;
    }
    if boundsChecking then assert(currStart == size);
  }

  /* Indexing is O(log(size)) time. */
  proc VectorList.this(idx: int) ref {
    if boundsChecking && (idx < 0 || idx >= size) then
      halt("VectorList index ", idx, " is out of bounds 0..", size-1);

    for (s,LE) in links() {
      if idx < s + LE.size then
        return LE.elemAt(idx - s);
    }

    halt("exhausted the list");
    return head.elemAt(0); // dummy
  }

  /* Serial iterator. */
  iter VectorList.these() ref {
    for (s,LE) in links() do
      for idx in 0..#LE.size do
        yield LE.elemAt(idx);
  }

  /* Leader iterator redirects to range leader. */
  iter VectorList.these(param tag) where tag == iterKind.leader {
    const rng = 0..#size;
    for flw in rng.these(tag) do
      yield flw;
  }

  /* Follower iterator takes O(log(size)) time to start up. */
  iter VectorList.these(param tag, followThis: _tuple) ref
    where tag == iterKind.follower
  {
    compilerAssert(followThis.size == 1,
                   "VectorList can follow only 1-dimensional leaders");
    ref followRange = followThis(1);
    compilerAssert(isRange(followRange));
    if followRange.stride < 0 then
      halt("VectorList cannot follow negative strides");
    const low = followRange.alignedLow;
    const high = followRange.alignedHigh;

    for (s,LE) in links() {
      if s > high then break; //done
      if s + LE.size <= low then continue;
      for idx in followRange[s..#LE.size] - s do
        yield LE.elemAt(idx);
    }
  }

  /* Copy all elements to a new DefaultRectangular array and return it. */
  proc VectorList.toArray() {
    var DR: [1..size] eltType;
    forall (dr,vl) in zip(DR,this) do  // should this be serial?
      dr = vl;
    return DR;
  }

}  // module VectorLists
