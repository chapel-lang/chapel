/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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
   The ``RangeChunk`` module assists with dividing a bounded ``range`` of any ``idxType``
   and stride into ``numChunks``. Chunks are 0-based, with the ``0`` index chunk including
   ``range.low`` and the ``numChunks - 1`` index chunk including ``range.high``.
   
   Chunks are accessible in several ways:

     * as a range, through an iterator
     * as a range, through a query
     * as a tuple of 0-based orders into the range, through an iterator
     * as a tuple of 0-based orders into the range, through a query
   
   Given that it will be uncommon for the length of a given ``range`` to be divisible by
   ``numChunks``, there are three different remainder policies available, expressed
   by the enum ``RemElems``.
*/  
module RangeChunk {

  /*
     ``RemElems`` specifies the distribution of remainder elements:
       
       - **Thru**: default policy; remainder elements will be distributed throughout
         ``numChunks`` chunks
       - **Pack**: chunks at the front will receive ``ceil(range.size / numChunks)``
         elements, then one chunk will receive what is left over; the actual number of chunks
         may be less than ``numChunks``
       - **Mod**: in ``numChunks`` chunks, every chunk that has an index less than
         ``range.size % numChunks`` will receive a remainder element
  */
  enum RemElems {
    Thru,
    Pack,
    Mod
  }
  private use RemElems;
  private use BoundedRangeType;

  
  /*
     Iterates through chunks ``0`` to ``numChunks - 1`` of range ``r``, emitting each
     as a range. The remainders will be distributed according to ``remPol``.
  */
  pragma "order independent yielding loops"
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

  /*
     Returns the ``idx`` chunk of range ``r`` as a range. The remainders will be
     distributed according to ``remPol``.
  */
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
    Iterates through a range ``r``, which is blocked up in repeating ```nTasks```
    blocks of size ```blockSize```. Blocks are indexed from 0..nTasks-1 and the  
    iterator emits all blocks with index ```tid``` in a cyclic manner.
  */
  //  Eg : 
  //  1. For a range 1..15 and 4 blocks of size 2
  //  2. The block indexes range : 0-2
  //  3. The range is blocked up as following block indexes :
  //      1,2, 3,4, 5,6, 7,8, 9,10, 11,12, 13,14, 15
  //       0    1    2    3    0      1      2     3 
  //  4. For a desired tid 2, the following chunks are emitted
  //      (5,6) (13,14)
  iter blockCyclicChunks(r: range(?t, boundedType=BoundedRangeType.bounded,
                         ?strided), blockSize: integral, tid: integral, 
                         nTasks: integral) {
    if (tid >= nTasks) then
      halt("Parameter tid must be < nTasks " +
           "because blocks are indexed from 0..nTasks-1");

    if (blockSize <= 0) then
      halt("blockSize must a positive number");

    if (nTasks <= 0) then
      halt("nTasks must be a positive number");

    var rangeStride = r.stride;
    var blockStride = blockSize * rangeStride;
    var low = r.low;
    var high = r.high;
    var firstBlockStart = (if rangeStride > 0 then r.low  else r.high) +
                            blockStride * tid;
    if firstBlockStart > r.high || firstBlockStart < r.low then return;

    var strideToNextBlock = blockStride * nTasks;
  
    if rangeStride > 0 {
      for blockStart in firstBlockStart..high by strideToNextBlock {
        var blockEnd = min(high, blockStart + blockStride - 1);
        yield blockStart..blockEnd by rangeStride;
      }
    } else {
      for blockEnd in low..firstBlockStart by strideToNextBlock {
        var blockStart = max(low, blockEnd + blockStride + 1);
        yield blockStart..blockEnd by rangeStride;
      }
    }
  }

  /*
     Iterates through chunks ``0`` to ``numChunks - 1`` of range ``r``, emitting each
     as a 0-based order tuple. The remainders will be distributed according to ``remPol``.
  */
  pragma "order independent yielding loops"
  iter chunksOrder(r: range(?RT, bounded, ?), numChunks: integral,
                   remPol: RemElems = Thru): 2*RT {
    if r.size == 0 || numChunks <= 0 then
      return;
    const nElems = r.size;
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

  /*
     Returns the ``idx`` chunk of range ``r`` as a 0-based order tuple. The remainders
     will be distributed according to ``remPol``.
  */
  proc chunkOrder(r: range(?RT, bounded, ?), numChunks: integral, idx: integral,
                  remPol: RemElems = Thru): 2*RT {
    if r.size == 0 || numChunks <= 0 || idx < 0 || idx >= numChunks then
      return (1: RT, 0: RT);

    const nElems = r.size;
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


  //
  // Private helpers for order pairs and thereby ranges.
  // Each corresponds with a remainder policy.
  //
  pragma "no doc"
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

  pragma "no doc"
  private proc chunkOrderPack(chunkSize: ?I, nElems: I, i: I): (I, I) {
    const start = chunkSize * i;
    if start >= nElems then
      return (1: I, 0: I);

    var end = start + chunkSize - 1;
    if end >= nElems then
      end = nElems - 1;
    return (start, end); 
  }

  pragma "no doc"
  private proc chunkOrderMod(chunkSize: ?I, rem: I, nElems: I, nChunks: I,
                             i: I): (I, I) {
    var start, end: I;
    if i < rem {
      start = i * (chunkSize + 1);
      end = start + chunkSize;
    } else {
      start = nElems - (nChunks - i) * chunkSize;
      end = start + chunkSize - 1;
    }
    return (start, end);
  }
}
