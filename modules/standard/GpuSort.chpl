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

/* Supports sorting uints on GPUs using radix sort.

    These functions are supposed to be called from the GPU locale.

  .. warning::

    This module is unstable and its interface is subject to change in the
    future.
*/
@unstable("The GpuSort module is unstable and its interface is subject to change in the future.")
module GpuSort
{
  use GPU;
  import Math;
  import Time;

  /*
    Sort an array on the GPU.
    The array must be in GPU-accessible memory and the function must
    be called from outside a GPU-eligible loop.
    Only arrays with uint eltType are supported.
    A simple example is the following:

     .. code-block:: chapel

       on here.gpus[0] {
         var Arr = [3, 2, 1, 5, 4] : uint; // will be GPU-accessible
         GpuSort.sort(Arr);
         writeln(Arr); // [1, 2, 3, 4, 5]
       }
  */
  proc sort(ref gpuInputArr: [] uint) {
    if(!here.isGpu()) then halt("GpuSort.sort must be run on a gpu locale");

    if gpuInputArr.size == 0 then return;
    // Based on the inputArr size, get a chunkSize such that numChunks is on the order of thousands
    // TODO better heuristic here?
    var chunkSize = Math.divCeil(gpuInputArr.size, 2000);
    parallelRadixSort(gpuInputArr, bitsAtATime=8, chunkSize, noisy=false, distributed=false);
  }

  // We no doc it so we can test this independently to simulate all cases that can happen with sort
  @chpldoc.nodoc
  proc parallelRadixSort(ref gpuInputArr: [] uint, const bitsAtATime : int = 8, const chunkSize : int = 512, const noisy : bool = false,
                         const distributed : bool = false) { // The last argument is for multi GPU sort that is pending a patch before it can work

    // How many bits we sort at once based on the bitsAtATime
    const buckets = 1 << bitsAtATime; // 2^bitsAtATime, ex: 2^4 = 16 = 0b10000
    const bitMask = buckets - 1; // 2^bitsAtATime - 1, ex: 2^4 - 1 = 15 = 0b1111

    // The size of the chunks the input array is divided into for parallelism
    // Each chunk has it's own count table (hence it's own prefix sum)
    // Which we then intelligently combine together to produce a sorted output
    const numChunks = Math.divCeil(gpuInputArr.size,chunkSize);
    // TODO: make up some heuristic to chose a good chunkSize and numChunks?


    if (noisy){
        writeln("Bits at a time: ", bitsAtATime);
        writeln("Buckets: ", buckets);
        writeln("Bit mask: ", bitMask);
        // writeln("Input: ", gpuInputArr);
        writeln("Chunk Size: ", chunkSize);
        writeln("Num Chunks: ", numChunks);
    }

    // Based on the number of chunks we're gonna divide this input into,
    // we create the prefixSum arrays, one for each chunk
    // And we only create it once
    // This was we can reuse it for each iteration of radix Sort
    var prefixSums: [0..<numChunks*buckets] uint;

    var timer: Time.stopwatch;
    // Ceiling on number of iterations based on max element in array
    timer.start();
    var maxVal = max(uint);
    // Reduce can be used to do this faster
    // We do this because the preferred ways below either don't work
    // or take too long
    // But I'm leaving this as is due to https://github.com/chapel-lang/chapel/issues/22736
    // var maxVal = max reduce gpuInputArr; // Doesn't work actually
    // var maxVal = gpuInputArr[0]; // Takes toooo long
    // for i in 1..<gpuInputArr.size {
    //   if gpuInputArr[i] > maxVal {
    //     maxVal = gpuInputArr[i];
    //   }
    // }
    timer.stop();
    if noisy then writeln("maxVal: ", maxVal);
    if noisy then writeln("Finding Max time: ", timer.elapsed());

    // Number of iterations is the number of bits in the max element divided by number of bits we sort at a time
    var exp = 0;
    while (maxVal> 0){
      if(noisy) then writeln("       Exp: ", exp);

      // Each chunk maintains it's own count array (here it's called prefixSum)
      // First we do the counts in parallel
      // After this we need to use the prefix sum to get a global offset
      // And finally we will use that offset to do the reorder/scatter step
      // in parallel

      // Count
      timer.clear();
      timer.start();
      if !distributed then
        parallelCount(prefixSums, gpuInputArr, exp, bitMask, chunkSize, numChunks);
      else {
        const numChunksPerGpu = Math.divCeil(numChunks, 2);
        distributedCount(prefixSums, gpuInputArr, exp, bitMask, numChunks, here.gpus.size);
      }
      timer.stop();

      if noisy then writeln("Parallel Count Time: ", timer.elapsed());
      if noisy then writeln("Prefix Sums after count: ", prefixSums);

      // Calculate global offsets for each chunk
      // I can calculate the global offset by simply doing a prefix sum over the array
      // See https://gpuopen.com/download/publications/Introduction_to_GPU_Radix_Sort.pdf
      // Figure 1 and equation 1 for explanation
      timer.clear();
      timer.start();
      gpuScan(prefixSums);
      timer.stop();
      if noisy then writeln("Parallel Scan Time: ", timer.elapsed());
      if noisy then writeln("Prefix Sums after scan: ", prefixSums);

      // Scatter
      // Now we have the offsets for each individual chunk
      // We can use those to scatter back into the output Array
      timer.clear();
      timer.start();
      parallelScatter(prefixSums, gpuInputArr, exp, bitMask, chunkSize, numChunks);
      timer.stop();
      if noisy then writeln("Parallel Scatter Time: ", timer.elapsed());
      if noisy then writeln("Array after scatter: ", gpuInputArr);

      // Increment the exp, decrement maxVal by shifting, and copy arrays
      exp += bitsAtATime;
      maxVal = maxVal >> bitsAtATime;
    }
  }

  private proc parallelCount(ref gpuCounts: [], ref gpuInputArr: [] uint, const exp: int,
                    const bitMask: int, const chunkSize: int,
                    const numChunks: int,  const numChunksThisGpu : int = numChunks,
                    const startChunk: int = 0,
                    const gpuId: int = 0, const resetCountsArray = true){

    // Instead of using a nested array of arrays, use a simple 1D array of
    // size numChunks*buckets which is a column major representation
    // of the 2D array where each chunk has it's own array of buckets.
    // This way calculating the offset is just a prefix sum
    // And we can use strided iteration to work on them as if they were
    // 2D arrays.
    // writeln("Counting on gpu: ", gpuId);
    if resetCountsArray then
      gpuCounts = 0; // All 0s
    // writeln("Bounds for chunk: ", startChunk, " to ", startChunk+numChunksThisGpu-1);
    const arrSize = gpuInputArr.size;
    const low = gpuInputArr.domain.low; // https://github.com/chapel-lang/chapel/issues/22433

    @assertOnGpu
    foreach chunk in startChunk..#numChunksThisGpu {
      // Count for each chunk in parallel.
      const startIdx : int = (chunk:int)*chunkSize;
      const endIdx : int = min(arrSize,startIdx+chunkSize);
      for i in startIdx..<endIdx {
        const arrIdx = i+low;
        const tmp = ((gpuInputArr[arrIdx]>>exp) & bitMask):int;
        const tmp2 = chunk+(numChunks*tmp);
        gpuCounts[tmp2] += 1;
      }
    }
  }

  // Multi GPU Experimental function
  // This won't work for now since it wasn't updated with the modularization of radix sort
  private proc distributedCount(ref counts: [], ref gpuInputArr: [] uint, const exp: int, const bitMask: int,
                        const numChunks: int, const numGpus : int ) {
    // Counts should be all 0s
    // counts = 0;

    const numChunksPerGpu = Math.divCeil(numChunks, numGpus);
    const numChunksPerGpuLast = numChunks - (numChunksPerGpu * (numGpus-1));
    var firstRunReset = true;

    // Distribute the counts across the GPUs
    for gpu in 0..<numGpus {
      const numChunksThisGpu = if (gpu==numGpus-1) then numChunksPerGpuLast else numChunksPerGpu;
      const startChunk = gpu*numChunksPerGpu;

      // Distributing the counts across the GPUs
      // Each GPU works on a subset of chunks from the array
      // Since each chunk already gets a slot in the counts array
      // We will pass the entire counts array to each GPU
      // But each GPU will only work on a subset of the counts array and not care about the rest
      parallelCount(counts, gpuInputArr, exp, bitMask, numChunks, numChunksThisGpu, startChunk, gpu, firstRunReset);
      // writeln(" Count: ", thisCounts);
      // This works because counts array is all 0s and
      // we're only adding to it. The parallelCount function leaves the
      // rest of the array untouched (i.e. 0s) so for 2 GPUs the add might be:
      // counts1 : 1 0 | 3 0 | 2 0 | 32 0
      // counts2 : 0 3 | 0 2 | 0 12 | 0 2
      // counts      : 1 3 | 3 2 | 2 12 | 32 2
      // I think this is not a race condition for the same reason
      firstRunReset = false; // This is a hacky solution


      // In an ideal world I would like to use a + reduce intent here such that each iteration gets it's own
      // copy of the counts array and it is accumulated into the main counts array
      // And the loop should be a coforall instead of a foreach
      // But I could not get that to work/ don't know how that works
    }
  }

  private proc parallelScatter(ref gpuOffsets: [], ref gpuInputArr: [] uint,
                       const exp: int, const bitMask: int,
                       const chunkSize: int, const numChunks: int){

    var gpuOutputArr : gpuInputArr.type;
    const arrSize = gpuInputArr.size;
    const low = gpuInputArr.domain.low; // https://github.com/chapel-lang/chapel/issues/22433

    @assertOnGpu
    foreach chunk in 0..<numChunks {
      // Count for each chunk in parallel.
      const startIdx : int = (chunk:int)*chunkSize;
      const endIdx : int = min(arrSize,startIdx+chunkSize);
      for i in startIdx..<endIdx {
        const arrIdx = i+low;
        const tmp = ((gpuInputArr[arrIdx]>>exp) & bitMask):int; // Where in the counts array to look
        const tmp2 = chunk+(numChunks*tmp); // Index into the offsets array
        const tmp3 = gpuOffsets[tmp2]:int; // Index into the output array
        // This may happen when gpuInputArr.size%chunkSize!=0
        // if tmp3>=gpuInputArr.size then continue;
        gpuOutputArr[low+tmp3] = gpuInputArr[arrIdx];
        gpuOffsets[tmp2] += 1;
      }
    }
    gpuInputArr = gpuOutputArr;
  }

}
