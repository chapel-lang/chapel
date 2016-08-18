/*
 * Copyright 2004-2016 Cray Inc.
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

enum RemElems {
  Dist,
  Tail
}
use RemElems;

// TODO: 1..numChunks optimization?
// iter chunks(numElems: integral, numChunks integral, remPol: RemElems = Dist)

iter chunks(
  r: range,
  numChunks: integral,
  remPol: RemElems = Dist): range
{
  for (startOrder, endOrder) in chunksOrder(r, numChunks, remPol) {
    var start = r.orderToIndex(startOrder);
    var end = r.orderToIndex(endOrder);
    yield start..end by r.stride;
  }
}

iter chunksOrder(
  r: range,
  numChunks: integral,
  remPol: RemElems = Dist): 2*r.idxType
{
  var numElems = r.length;
  var div = numElems / numChunks;
  var rem = numElems % numChunks;
  for i in 1..numChunks {
    var order = if remPol == Dist
      then chunkOrderDist(div, rem, numElems, numChunks, i)
      else chunkOrderTail(div, numElems, numChunks, i);
    yield order; 
  }
}

proc chunk(
  r: range,
  numChunks: integral,
  i: integral,
  remPol: RemElems = Dist): range
{
  var (startOrder, endOrder) = chunkOrder(r, numChunks, i, remPol);
  var start = r.orderToIndex(startOrder);
  var end = r.orderToIndex(endOrder);
  return start..end by r.stride;
}

// Divide r into (almost) equal numChunks pieces, return the i-th piece.
proc chunkOrder(
  r: range,
  numChunks: integral,
  i: integral,
  remPol: RemElems = Dist): 2*r.idxType
{
  var numElems = r.length;
  var div = numElems / numChunks;
  var rem = numElems % numChunks;
  var order = if remPol == Dist
    then chunkOrderDist(div, rem, numElems, numChunks, i)
    else chunkOrderTail(div, numElems, numChunks, i);
  return order;
}

// remainder elements distributed over chunks
// zero-based indexing
private proc chunkOrderDist(
  div: uint,
  rem: uint,
  numElems: uint,
  numChunks: uint,
  i: uint): (uint, uint)
{
  var start, end: uint;
  if i <= rem {
    // (div+1) elements per chunk
    var end = i * (div + 1) - 1;
    start = end - div;
  } else {
    // (div) elements per chunk
    start = numElems - (numChunks - i + 1) * div;
    end = start + div;
  }
  return (start, end);
}

// remainder elements included in last chunk
// zero-based indexing
private proc chunkOrderTail(
  div: uint,
  numElems: uint,
  numChunks: uint,
  i: uint): (uint, uint)
{
  var start = div * (i - 1);
  var end = if i == numChunks
    then numElems
    else start + div;
  return (start, end);
}
