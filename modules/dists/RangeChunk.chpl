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

iter chunks(
  r: range,
  numChunks: integral,
  remPol: RemElems = Dist): range(r.idxType, r.boundedType, true)
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
  type RT = r.idxType;
  var nElems = r.length: RT;
  var nChunks = numChunks: RT;

  var div = nElems / nChunks;
  var rem = nElems % nChunks;
  for i in 1..nChunks {
    yield if remPol == Dist
      then chunkOrderDist(div, rem, nElems, nChunks, i)
      else chunkOrderTail(div, nElems, nChunks, i);
  }
}

// Divide r into (almost) equal numChunks pieces, return the i-th piece.
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

proc chunkOrder(
  r: range,
  numChunks: integral,
  i: integral,
  remPol: RemElems = Dist): 2*r.idxType
{
  type RT = r.idxType;
  var nElems = r.length: RT;
  var nChunks = numChunks: RT;
  var idx = i: RT;

  var div = nElems / nChunks;
  if remPol == Dist {
    var rem = nElems % nChunks;
    return chunkOrderDist(div, rem, nElems, nChunks, idx);
  } else {
    return chunkOrderTail(div, nElems, nChunks, idx);
  }
}

// remainder elements distributed over chunks
// zero-based indexing
private proc chunkOrderDist(
  div: ?I,
  rem: I,
  nElems: I,
  nChunks: I,
  i: I): (I, I)
{
  var start, end: I;
  if i <= rem {
    // (div+1) elements per chunk
    var end = i * (div + 1) - 1;
    start = end - div;
  } else {
    // (div) elements per chunk
    start = nElems - (nChunks - i + 1) * div;
    end = start + div;
  }
  return (start, end);
}

// remainder elements included in last chunk
// zero-based indexing
private proc chunkOrderTail(
  div: ?I,
  nElems: I,
  nChunks: I,
  i: I): (I, I)
{
  var start = div * (i - 1);
  var end = if i == nChunks
    then nElems
    else start + div;
  return (start, end);
}
