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

module RangeChunk {
  enum RemElems {
    Thru,
    Pack,
    Mod
  }
  use RemElems;
  use BoundedRangeType;

  /*
   * Range-based interface, iterator and query.
   * idx in query is zero based.
   */

  iter chunks(
    r: range(?RT, bounded, ?S),
    numChunks: integral,
    remPol: RemElems = Thru): range(RT, bounded, S)
  {
    for (startOrder, endOrder) in chunksOrder(r, numChunks, remPol) {
      const start = r.orderToIndex(startOrder);
      const end = r.orderToIndex(endOrder);
      yield if S
        then start..end by r.stride
        else start..end;
    }
  }

  proc chunk(
    r: range(?RT, bounded, ?S),
    numChunks: integral,
    idx: integral,
    remPol: RemElems = Thru): range(RT, bounded, S)
  {
    const (startOrder, endOrder) = chunkOrder(r, numChunks, idx, remPol);
    const start = r.orderToIndex(startOrder);
    const end = r.orderToIndex(endOrder);
    return if S
      then start..end by r.stride
      else start..end;
  }

  /*
   * Order-based interface, iterator and query.
   * Orders are zero-based "indices" into a range. 
   * idx in query is zero-based.
   */

  iter chunksOrder(
    r: range(?RT, bounded, ?),
    numChunks: integral,
    remPol: RemElems = Thru): 2*RT
  {
    if r.length == 0 || numChunks <= 0 then
      return;
    const nElems = r.length;
    const nChunks = numChunks: RT;

    var chunkSize, rem: RT;
    if remPol == Pack {
      chunkSize = nElems / nChunks;
      if chunkSize * nChunks != nElems then
        chunkSize += 1;
    } else { // Mod
      chunkSize = nElems / nChunks;
      rem = nElems - chunkSize * nChunks;
    }

    for i in 0..#nChunks {
      var chunk: 2*RT;
      if remPol == Thru {
        chunk = chunkOrderThru(nElems, nChunks, i);
      } else if remPol == Pack {
         chunk = chunkOrderPack(chunkSize, nElems, i);
      } else { // Mod
        chunk = chunkOrderMod(chunkSize, rem, nElems, nChunks, i);
      }
      yield chunk;
    }
  }

  proc chunkOrder(
    r: range(?RT, bounded, ?),
    numChunks: integral,
    idx: integral,
    remPol: RemElems = Thru): 2*RT
  {
    if r.length == 0 || numChunks <= 0 || idx >= numChunks then
      return (1: RT, 0: RT);

    const nElems = r.length;
    const nChunks = numChunks: RT;
    const i = idx: RT;

    if remPol == Thru {
      return chunkOrderThru(nElems, nChunks, i);
    } else if remPol == Pack {
      var chunkSize = nElems / nChunks;
      if chunkSize * nChunks != nElems then
        chunkSize += 1;
      return chunkOrderPack(chunkSize, nElems, i);
    } else { // Mod
      const chunkSize = nElems / nChunks;
      const rem = nElems - chunkSize * nChunks;
      return chunkOrderMod(chunkSize, rem, nElems, nChunks, i);
    }
  }

  /*
   * Query the chunk index of a value in the range.
   * Returned value is zero-based.
   */

  proc whichChunk(
    r: range(?RT, bounded, ?),
    numChunks: integral,
    val: integral,
    remPol: RemElems = Thru): RT
  {
    assert(r.length != 0 && numChunks > 0);

    const nElems = r.length;
    const nChunks = numChunks: RT;
    const i = r.indexOrder(val);

    assert(i: int != -1);

    if remPol == Thru {
      return i * nChunks / nElems; 
    } else if remPol == Pack {
      var chunkSize = nElems / nChunks;
      if chunkSize * nChunks != nElems then
        chunkSize += 1;
      return i / chunkSize;
    } else { // Mod
      const chunkSize = nElems / nChunks;
      const chunkSizePlus = chunkSize + 1;
      const rem = nElems - chunkSize * nChunks;
      const splitPoint = rem * chunkSizePlus;
      return if i < splitPoint
        then i / chunkSizePlus 
        else rem + (i - splitPoint) / chunkSize; 
    }
  }


  /*
   * Private helpers for order pairs and thereby ranges.
   * Each corresponds with a remainder policy.
   * i is a zero-based index.
   */

  // remainder elems distributed throughout 
  private proc chunkOrderThru(
    nElems: ?I,
    nChunks: I,
    i: I): (I, I)
  {
    const m = nElems * i;
    const start = if i == 0
      then 0: I
      else ceilXDivByY(m, nChunks);
    const end = if i == nChunks - 1
      then nElems - 1
      else ceilXDivByY(m + nElems, nChunks) - 1;
    return (start, end);
  }

  private proc ceilXDivByY(x, y) return 1 + (x - 1)/y;

  // remainder elems packed into all chunks with small tail
  private proc chunkOrderPack(
    chunkSize: ?I,
    nElems: I,
    i: I): (I, I)
  {
    const start = chunkSize * i;
    var end = start + chunkSize - 1;
    if end > nElems then
      end = nElems;
    return (start, end); 
  }

  // remainder elems distributed over chunks before rem
  private proc chunkOrderMod(
    chunkSize: ?I,
    rem: I,
    nElems: I,
    nChunks: I,
    i: I): (I, I)
  {
    var start, end: I;
    if i < rem {
      // (chunkSize+1) elements per chunk
      start = i * (chunkSize + 1);
      end = start + chunkSize;
    } else {
      // (chunkSize) elements per chunk
      start = nElems - (nChunks - i) * chunkSize;
      end = start + chunkSize - 1;
    }
    return (start, end);
  }
}
