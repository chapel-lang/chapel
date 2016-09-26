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

  iter chunks(r: range(?RT, bounded, ?S), numChunks: integral,
              remPol: RemElems = Thru): range(RT, bounded, S) {
    for (startOrder, endOrder) in chunksOrder(r, numChunks, remPol) {
      const start = r.orderToIndex(startOrder);
      const end = r.orderToIndex(endOrder);
      yield if S
        then start..end by r.stride
        else start..end;
    }
  }

  proc chunk(r: range(?RT, bounded, ?S), numChunks: integral, idx: integral,
             remPol: RemElems = Thru): range(RT, bounded, S) {
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

  iter chunksOrder(r: range(?RT, bounded, ?), numChunks: integral,
                   remPol: RemElems = Thru): 2*RT {
    if r.length == 0 || numChunks <= 0 then
      return;
    const nElems = r.length;
    var nChunks = min(numChunks, nElems): RT;

    var chunkSize, rem: RT;
    select (remPol) {
      when Pack {
        chunkSize = nElems / nChunks;
        if chunkSize * nChunks != nElems {
          chunkSize += 1;
          nChunks = divceil(nElems, chunkSize);
        }
      }
      when Mod {
        chunkSize = nElems / nChunks;
        rem = nElems - chunkSize * nChunks;
      }
    }

    for i in 0..#nChunks {
      var chunk: 2*RT;
      select (remPol) {
        when Thru do chunk = chunkOrderThru(nElems, nChunks, i);
        when Pack do chunk = chunkOrderPack(chunkSize, nElems, i);
        when Mod  do chunk = chunkOrderMod(chunkSize, rem, nElems, nChunks, i);
        otherwise halt("RangeChunk: unknown RemElems in chunksOrder");
      }
      yield chunk;
    }
  }

  proc chunkOrder(r: range(?RT, bounded, ?), numChunks: integral, idx: integral,
                  remPol: RemElems = Thru): 2*RT {
    if r.length == 0 || numChunks <= 0 || idx < 0 || idx >= numChunks then
      return (1: RT, 0: RT);

    const nElems = r.length;
    const nChunks = min(numChunks, nElems): RT;
    const i = idx: RT;

    select (remPol) {
      when Thru {
        return chunkOrderThru(nElems, nChunks, i);
      }
      when Pack { 
        var chunkSize = nElems / nChunks;
        if chunkSize * nChunks != nElems then
          chunkSize += 1;
        return chunkOrderPack(chunkSize, nElems, i);
      }
      when Mod {
        const chunkSize = nElems / nChunks;
        const rem = nElems - chunkSize * nChunks;
        return chunkOrderMod(chunkSize, rem, nElems, nChunks, i);
      }
      otherwise {
        halt("RangeChunk: unknown RemElems in chunkOrder");
      }
    }
  }


  /*
   * Private helpers for order pairs and thereby ranges.
   * Each corresponds with a remainder policy.
   * i is a zero-based index.
   */

  // remainder elems distributed throughout 
  private proc chunkOrderThru(nElems: ?I, nChunks: I, i: I): (I, I) {
    const m = nElems * i;
    const start = if i == 0
      then 0: I
      else divceil(m, nChunks);
    const end = if i == nChunks - 1
      then nElems - 1
      else divceil(m + nElems, nChunks) - 1;
    return (start, end);
  }

  // remainder elems packed into all chunks with small tail
  private proc chunkOrderPack(chunkSize: ?I, nElems: I, i: I): (I, I) {
    const start = chunkSize * i;
    if start >= nElems then
      return (1: I, 0: I);

    var end = start + chunkSize - 1;
    if end >= nElems then
      end = nElems - 1;
    return (start, end); 
  }

  // remainder elems distributed over chunks before rem
  private proc chunkOrderMod(chunkSize: ?I, rem: I, nElems: I, nChunks: I,
                             i: I): (I, I) {
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
