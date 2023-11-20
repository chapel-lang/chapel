module RadixSort {
  import BitOps;
  import Math;
  import Time;
  use GPU;

  config const useGpuId = 0;
  config const noisy = false;
  config const scanTest = false;
  config const scanType = "hillisSteele"; // "serial", "hillisSteele", "blelloch"
  config const bitsAtATime = 4; // 1-8 are all reasonable values
                                // but for some reason 16 performs best.
                                // My guess is because we're dealing with
                                // uint(64) it makes sense for 16 to do well
                                // since its a nice even factor and causes
                                // only 4 iterations of a single radix sort step
  // How many bits we sort at once based on the bitsAtATime
  const buckets = 1 << bitsAtATime; // 2^bitsAtATime, ex: 2^4 = 16 = 0b10000
  const bitMask = buckets - 1; // 2^bitsAtATime - 1, ex: 2^4 - 1 = 15 = 0b1111
  // The size of the chunks the input array is divided into for parallelism
  // Each chunk has it's own count table (hence it's own prefix sum)
  // Which we then intelligently combine together to produce a sorted output
  config const chunkSize = 128;
  config const scanChunkSize = 1024;
  // TODO Choose better defaults for the above two things
  config const parallelScan = false; // Need to improve this more
  config const distributed = false; // Distributed count across GPUs


  // This function requires that startIdx and endIdx are within the bounds of the array
  // it checks that only if boundsChecking is true (i.e. NOT with --fast or --no-checks)
  proc serialScan(ref arr: [] uint, startIdx = arr.domain.low, endIdx = arr.domain.high){
    // Convert this count array into a prefix sum
    // This is the same as the count array, but each element is the sum of all previous elements
    // This is an exclusive scan
    // Serial implementation
    if boundsChecking then
      assert(startIdx >= arr.domain.low && endIdx <= arr.domain.high);
    // Calculate the prefix sum
    var sum : uint = 0;
    for i in startIdx..endIdx {
      var t : uint = arr[i];
      arr[i] = sum;
      sum += t;
    }
  }

  proc roundToPowerof2(const x: uint){
    // Powers of two only have the highest bit set.
    // Power of two minus one will have all bits set except the highest.
    // & those two together should give us 0;
    // Ex 1000 & 0111 = 0000
    if (x & (x - 1)) == 0 then
      return x;
    // Not a power of two, so we pad it out
    // To the next nearest power of two
    const log_2_x = numBytes(uint)*8 - BitOps.clz(x); // Calculates the log quickly
    // Next highest nerest power of two is
    const rounded = 1 << log_2_x;
    return rounded;
  }

  proc hillisSteeleScan(ref hostArr: [] uint){
    // Hillis Steele Scan
    // This is the same as the count array, but each element is the sum of all previous elements
    // Uses a naive algorithm that does O(nlogn) work
    // Hillis and Steele (1986)
    if hostArr.size==0 then return;

    on here.gpus[useGpuId]{
      // The algorithm only works for arrays that are size of a power of two.
      // In case it's not a power of two we pad it out with 0s
      const size = roundToPowerof2(hostArr.size);
      var arr : [0..<size] uint;
      arr[hostArr.domain] = hostArr;

      var offset = 1;
      while offset < arr.size {
          var arrBuffer = arr;
          @assertOnGpu
          foreach i in offset..<arr.size {
            arr[i] = arrBuffer[i] + arrBuffer[i-offset];
          }
          offset = offset << 1;
      }

      // This code is a more reader friendly way to do the same thing as above
      // const log_2_size = numBytes(uint)*8 - BitOps.clz(arr.size); // Calculates the log quickly
      // for d in 1..<log_2_size {
      //   @assertOnGpu
      //   foreach i in 0..<arr.size {
      //     if i >= 1 << d { // 2^d
      //       arr[i] = arr[i-(1 << (d-1))] + arr[i];
      //     }
      //   }
      // }

      // Change inclusive scan to exclusive
      var arrBuffer = arr;
      foreach i in 1..<arr.size {
        arr[i] = arrBuffer[i-1];
      }
      arr[0] = 0;

      // Copy back
      hostArr=arr[hostArr.domain];
    }
  }

  proc blellochScan(ref hostArr: [] uint){
    // Blelloch Scan
    // This is the same as the count array, but each element is the sum of all previous elements
    // Uses a more efficient algorithm that does O(n) work
    // Blelloch (1990)
    // Up-sweep
    on here.gpus[useGpuId]{
      // The algorithm only works for arrays that are size of a power of two.
      // In case it's not a power of two we pad it out with 0s
      const size = roundToPowerof2(hostArr.size);
      var arr : [0..<size] uint;
      arr[hostArr.domain] = hostArr;
      var offset = 1;
      while offset < arr.size {
        var arrBuffer = arr;
        @assertOnGpu
        foreach idx in 0..<arr.size/(2*offset) {
          const doubleOff = offset << 1;
          const i = idx*doubleOff;
          arr[i+doubleOff-1] = arrBuffer[i+offset-1] + arrBuffer[i+doubleOff-1];
        }
        offset = offset << 1;
      }

      // Down-sweep
      arr[arr.size-1] = 0;
      offset = arr.size >> 1;
      while offset > 0 {
        var arrBuffer = arr;
        @assertOnGpu
        foreach idx in 0..<arr.size/(2*offset) {
          const i = idx*2*offset;
          const t = arrBuffer[i+offset-1];
          arr[i+offset-1] = arrBuffer[i+2*offset-1];
          arr[i+2*offset-1] = arr[i+2*offset-1] + t;
        }
        offset = offset >> 1;
      }
      hostArr=arr[hostArr.domain];
    }
  }

  proc arrScan(ref arr: [] uint){
    if scanType == "hillisSteele" then
      hillisSteeleScan(arr);
    else if scanType == "blelloch" then
      blellochScan(arr);
    else if scanType == "serial" then
      serialScan(arr);
  }

  proc parallelCount(ref counts: [], ref inputArr: [] uint, const exp: int, const bitMask: int,
                    const numChunks: int,  const numChunksThisGpu : int = numChunks, const startChunk: int = 0,
                    const gpuId: int = 0, const resetCountsArray = false){
    // Instead of using a nested array of arrays, use a simple 1D array of
    // size numChunks*buckets which is a column major representation
    // of the 2D array where each chunk has it's own array of buckets.
    // This way calculating the offset is just a prefix sum
    // And we can use strided iteration to work on them as if they were
    // 2D arrays.
    // writeln("Counting on gpu: ", gpuId);
    on here.gpus[gpuId]{
      var gpuCounts : counts.type; // All 0s or ...
      if !resetCountsArray then
        gpuCounts = counts; // ...initialized to counts
      const gpuInputArr = inputArr;
      // writeln("Bounds for chunk: ", startChunk, " to ", startChunk+numChunksThisGpu-1);
      @assertOnGpu
      foreach chunk in startChunk..#numChunksThisGpu {
        // Count for each chunk in parallel.
        const startIdx : int = (chunk:int)*chunkSize;
        const endIdx : int = startIdx+chunkSize-1;
        for i in startIdx..endIdx {
          const tmp = ((gpuInputArr[i]>>exp) & bitMask):int;
          const tmp2 = chunk+(numChunks*tmp);
          gpuCounts[tmp2] += 1;
        }
      }
      counts = gpuCounts;
}
  }

  proc distributedCount(ref counts: [], ref inputArr: [] uint, const exp: int, const bitMask: int,
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
      parallelCount(counts, inputArr, exp, bitMask, numChunks, numChunksThisGpu, startChunk, gpu, firstRunReset);
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

  proc parallelArrScan(ref arr: [] uint){
    // Divide up the array into chunks of a reasonable size
    // Config this with a scanChunkSize
    const numScanChunks = Math.divCeil(arr.size,scanChunkSize);

    if numScanChunks == 1 {
      arrScan(arr);
      return;
    }

    // Allocate an accumulate array
    var scanArr : [0..<numScanChunks] uint;

    var host = here;

    on here.gpus[useGpuId]{
      var gpuScanArr = scanArr;
      var gpuArr = arr;
      const ceil = gpuArr.domain.high;
      // In parallel: For each chunk we do an in lane serial scan
      @assertOnGpu
      foreach chunk in 0..<numScanChunks {
        const start = chunk*scanChunkSize;
        var end = start+scanChunkSize-1;
        if end > ceil then end = ceil;
        gpuScanArr[chunk] = gpuArr[end]; // Save the last element before the scan overwrites it
        serialScan(gpuArr, start, end); // Exclusive scan in serial
        gpuScanArr[chunk] += gpuArr[end]; // Save inclusive scan in the scan Arr

      }

      on host {
        // Scan the scanArr
        // This is risky because gpuScanArr lives on the GPU
        // 2 out of 3 scan algorithms also work on GPU
        // So it's fine for those
        // But the serialScan is on the CPU
        // therefor we make sure we don't use serialScan here
        assert(scanType!="serialScan");
        arrScan(gpuScanArr); // funky words
      }

      @assertOnGpu
      foreach i in 0..<gpuArr.size {
        // In propagate the right values from scanArr
        // to complete the global scan
        const offset : int = i / scanChunkSize;
        gpuArr[i] += gpuScanArr[offset];
      }
      arr = gpuArr;
    }
  }

  proc arrCountAndScan(ref count: [?D] uint, ref inputArr: [] uint, exp: int, bitMask: int){
    // Count the number of occurrences of each bitsAtATime-bit chunk in the array
    // at the current exp position
    on here.gpus[useGpuId]{
      var gpuCount : [D] uint; // Initialized all 0s
      var gpuInputArr = inputArr;
      @assertOnGpu
      foreach i in gpuInputArr {
        gpuAtomicAdd(gpuCount[((i>>exp) & bitMask):int], 1);
        // count[((i>>exp) & bitMask):int] += 1;
      }
      count = gpuCount;
    }
    if scanTest then
      writeln(" Count: ", count);

    if parallelScan then parallelArrScan(count); else arrScan(count);

    if scanTest then
      writeln("Pf sum: ",count);
  }

  proc parallelScatter(ref offsets: [], ref inputArr: [] uint, const exp: int, const bitMask: int, const numChunks: int){
    on here.gpus[useGpuId]{
      var gpuOffsets = offsets;
      const gpuInputArr = inputArr;
      var gpuOutputArr : inputArr.type;
      const arrSize = gpuInputArr.size;
      const low = gpuInputArr.domain.low; // https://github.com/chapel-lang/chapel/issues/22433

      @assertOnGpu
      foreach chunk in 0..<numChunks {
        // Count for each chunk in parallel.
        const startIdx : int = (chunk:int)*chunkSize;
        const endIdx : int = if startIdx+chunkSize>arrSize then arrSize else startIdx+chunkSize;
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
      inputArr = gpuOutputArr;
    }
  }

  proc parallelRadixSort(ref inputArr: [] uint){
    const numChunks = Math.divCeil(inputArr.size,chunkSize);

    if (noisy){
        writeln("Bits at a time: ", bitsAtATime);
        writeln("Buckets: ", buckets);
        writeln("Bit mask: ", bitMask);
        // writeln("Input: ", inputArr);
        writeln("Chunk Size: ", chunkSize);
        writeln("Num Chunks: ", numChunks);
    }

    // Based on the number of chunks we're gonna divide this input into,
    // we create the prefixSum arrays, one for each chunk
    // And we only create it once
    // This was we can reuse it for each iteration of radix Sort
    var prefixSums: [0..<numChunks*buckets] uint;

    // Ceiling on number of iterations based on max element in array
    // Reduce can be used to do this faster
    // But I'm leaving this as is due to https://github.com/chapel-lang/chapel/issues/22736
    var max = inputArr[0];
    for i in 1..<inputArr.size {
      if inputArr[i] > max {
        max = inputArr[i];
      }
    }

    // Number of iterations is the number of bits in the max element divided by number of bits we sort at a time
    var exp = 0;
    while (max> 0){
      if(noisy) then
        writeln("       Exp: ", exp);

      // Each chunk maintains it's own count array (here it's called prefixSum)
      // First we do the counts in parallel
      // After this we need to use the prefix sum to get a global offset
      // And finally we will use that offset to do the reorder/scatter step
      // in parallel

      // Count
      var timer: Time.stopwatch;
      timer.start();
      if !distributed then
        parallelCount(prefixSums, inputArr, exp, bitMask, numChunks);
      else {
        const numChunksPerGpu = Math.divCeil(numChunks, 2);
        distributedCount(prefixSums, inputArr, exp, bitMask, numChunks, here.gpus.size);
      }
      timer.stop();
      if scanTest then
        writeln(" Count: ",prefixSums);

      // writeln("Parallel Count Time: ", timer.elapsed());

      // writeln("Here!");
      // Calculate global offsets for each chunk
      // I can calculate the global offset by simply doing a prefix sum over the array
      // See https://gpuopen.com/download/publications/Introduction_to_GPU_Radix_Sort.pdf
      // Figure 1 and equation 1 for explanation
      timer.clear();
      timer.start();
      if parallelScan then parallelArrScan(prefixSums); else arrScan(prefixSums);
      timer.stop();
      // writeln("Parallel Scan Time: ", timer.elapsed());
      if scanTest then
        writeln("Offset: ", prefixSums, "\n");

      // Scatter
      // Now we have the offsets for each individual chunk
      // We can use those to scatter back into the output Array

      timer.clear();
      timer.start();
      parallelScatter(prefixSums, inputArr, exp, bitMask, numChunks);
      timer.stop();
      // writeln("Parallel Scatter Time: ", timer.elapsed());

      // if noisy then
      //   writeln("       Output: ", inputArr);

      // Increment the exp, decrement max by shifting, and copy arrays
      exp += bitsAtATime;
      max = max >> bitsAtATime;
    }
  }

  proc radixSort(ref inputArr: [] uint){
    if (noisy){
        writeln("Bits at a time: ", bitsAtATime);
        writeln("Buckets: ", buckets);
        writeln("Bit mask: ", bitMask);
        writeln("Input: ", inputArr);
    }

    // Create the output array and copy input
    var outputArr: [0..<inputArr.size] uint;

    // Create the count and prefix sum arrays once
    // This was we can reuse it for each iteration of radix Sort
    var prefixSum: [0..<buckets] uint;

    // Ceiling on number of iterations based on max element in array
    // Reduce can be used to do this faster
    // But I'm leaving this as is due to https://github.com/chapel-lang/chapel/issues/22736
    var max = inputArr[0];
    for i in 1..<inputArr.size {
      if inputArr[i] > max {
        max = inputArr[i];
      }
    }

    // Number of iterations is the number of bits in the max element divided by number of bits we sort at a time
    var exp = 0;
    while (max> 0){
      if(noisy) then
        writeln("       Exp: ", exp);

      var timer: Time.stopwatch;
      timer.start();
      arrCountAndScan(prefixSum, inputArr, exp, bitMask);
      timer.stop();
      // writeln("Count and Scan Time: ", timer.elapsed());

      if noisy then
        writeln("       Prefix sum: ", prefixSum);
      // Use the prefix sum to reorder the array at the exp position
      // This is done by iterating through the array backwards
      // and placing each element in the correct position in the output array

      timer.clear();
      timer.start();
      for i in inputArr.dim(0){
        const idx = ((inputArr[i]>>exp) & bitMask):int;
        outputArr[prefixSum[idx]:int] = inputArr[i];
        prefixSum[idx] += 1;
      }
      timer.stop();
      // writeln("Scatter Time: ", timer.elapsed());

      if noisy {
        writeln("       Output: ", outputArr);
      }
      // Increment the exp, decrement max by shifting, and copy arrays
      exp += bitsAtATime;
      max = max >> bitsAtATime;
      inputArr = outputArr;
    }
    if noisy then
      writeln("       Output: ", outputArr);

  }

}



module SortTest {
  use GpuDiagnostics;
  use RadixSort;
  config const arrSize = 100;
  config const validate = true;
  config const printArr = false;
  config const perftest = false;
  config const gpuDiags = false;
  config const verboseGpu = false;
  config const parallel = false;

  proc checkSorted(arr: [] uint) {
    for i in 0..<arr.size-1 {
      if arr[i] > arr[i+1] {
        writeln("Not sorted");
        return;
      }
      if arr[i]==0 {
        writeln("Zero");  // sanity check. This should *not* happen
                          //based on our random seed
        return;
      }
    }
    writeln("Sorted");
  }

  proc main() {
    import Random;
    import Time;
    if gpuDiags then
      startGpuDiagnostics();
    if verboseGpu then
      startVerboseGpu();

    var timer: Time.stopwatch;
    // var arr = [1,4, 2, 6, 3, 7, 5, 8] : uint;
    var arr: [0..<arrSize] uint;
    var seed = 17; // Make it deterministic
    Random.fillRandom(arr, seed);
    if printArr then
      writeln(arr);

    timer.start();
    if parallel then
      RadixSort.parallelRadixSort(arr);
    else
      RadixSort.radixSort(arr);
    timer.stop();

    if printArr then
      writeln(arr);

    if perftest then
      writeln("Time: ", timer.elapsed());
    if validate then
      checkSorted(arr);

    if verboseGpu then
        stopVerboseGpu();
    if gpuDiags then {
      stopGpuDiagnostics();
      if parallel == true then
        assertGpuDiags(kernel_launch_aod=344, kernel_launch_um=344, host_to_device=40,
                       device_to_host=24, device_to_device=304);
      else if scanType == "hillisSteele" then
        assertGpuDiags(kernel_launch_aod=80, kernel_launch_um=72, host_to_device=12,
                       device_to_host=8, device_to_device=68);

      else if scanType == "blelloch" then
        assertGpuDiags(kernel_launch_aod=140, kernel_launch_um=132, host_to_device=12,
                       device_to_host=8, device_to_device=128);

      else if scanType == "serial" then
        assertGpuDiags(kernel_launch_aod=8, kernel_launch_um=4, host_to_device=4,
                       device_to_host=4, device_to_device=0);
    }
  }
}
