module RadixSort {
  import BitOps;
  import Math;
  import Time;
  use GPU;

  config param useGpuId = 0;
  config const noisy = false;
  config const bitsAtATime = 16; // 1-8 are all reasonable values
                                // but for some reason 16 performs best.
                                // My guess is because we're dealing with
                                // uint(64) it makes sense for 16 to do well
                                // since its a nice even factor and causes
                                // only 4 iterations of a single radix sort step

  private proc arrCountAndScan(ref count : [?D] uint, ref inputArr : [] uint, exp : int, bitMask : int) {
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

      // Scan the count array to get the prefix sum
      gpuScan(gpuCount);
      count = gpuCount;
    }
  }

  proc atomicRadixSort(ref inputArr : [] uint) {
    const buckets = 1 << bitsAtATime; // 2^bitsAtATime, ex: 2^4 = 16 = 0b10000
    const bitMask = buckets - 1; // 2^bitsAtATime - 1, ex: 2^4 - 1 = 15 = 0b1111

    if noisy {
        writeln("Bits at a time: ", bitsAtATime);
        writeln("Buckets: ", buckets);
        writeln("Bit mask: ", bitMask);
    }

    // Create the output array and copy input
    var outputArr : [0..<inputArr.size] uint;

    // Create the count and prefix sum arrays once
    // This was we can reuse it for each iteration of radix Sort
    var prefixSum : [0..<buckets] uint;

    // Ceiling on number of iterations based on max element in array
    var maxVal = max reduce inputArr;

    // Number of iterations is the number of bits in the max element divided by number of bits we sort at a time
    var exp = 0;
    while maxVal > 0 {
      if noisy then writeln("       Exp: ", exp);

      var timer : Time.stopwatch;
      timer.start();
      arrCountAndScan(prefixSum, inputArr, exp, bitMask);
      timer.stop();
      if noisy then writeln("Count and Scan Time: ", timer.elapsed());
      if noisy then writeln("       Prefix sum: ", prefixSum);

      // Use the prefix sum to reorder the array at the exp position
      // This is done by iterating through the array backwards
      // and placing each element in the correct position in the output array
      timer.clear();
      timer.start();
      for i in inputArr.dim(0) {
        const idx = ((inputArr[i]>>exp) & bitMask):int;
        outputArr[prefixSum[idx]:int] = inputArr[i];
        prefixSum[idx] += 1;
      }
      timer.stop();
      if noisy then writeln("Scatter Time: ", timer.elapsed());
      if noisy then writeln("       Output: ", outputArr);

      // Increment the exp, decrement max by shifting, and copy arrays
      exp += bitsAtATime;
      maxVal = maxVal >> bitsAtATime;
      inputArr = outputArr;
    }
    if noisy then writeln("       Output: ", outputArr);
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
  config const atomicSort = false;
  config const verboseGpu = false;

  proc checkSorted(arr : [] uint) {
    for i in 0..<arr.size-1 {
      if arr[i] > arr[i+1] {
        writeln("Not sorted");
        return;
      }
      if arr[i] == 0 {
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
    import GPU;

    if gpuDiags then startGpuDiagnostics();
    if verboseGpu then startVerboseGpu();

    var timer: Time.stopwatch;
    var arr: [0..<arrSize] uint;
    var seed = 17; // Make it deterministic
    Random.fillRandom(arr, seed);

    if printArr then writeln(arr);

    if atomicSort then {
      timer.start();
      RadixSort.atomicRadixSort(arr);
      timer.stop();
    } else on here.gpus[useGpuId] {
      var gpuArr = arr; // Copy to GPU
      timer.start();
      GPU.gpuSort(gpuArr);
      timer.stop();
      arr = gpuArr; // Copy back to CPU
    }

    if printArr then writeln(arr);
    if perftest then writeln("Time: ", timer.elapsed());
    if validate then checkSorted(arr);
    if verboseGpu then stopVerboseGpu();
    if gpuDiags then {
      stopGpuDiagnostics();
      if atomicSort then
        assertGpuDiags(kernel_launch_aod=52, kernel_launch_um=44, host_to_device=8,
                       device_to_host=4, device_to_device=32);
      // else
        // With the addition of the CUB sort, this can be hard to pin down,
        // because we would have to do some effort to figure out if we used the
        // CUB sort or not.
        // assertGpuDiags(kernel_launch_aod=225, kernel_launch_um=201, host_to_device=9,
        //                device_to_host=1, device_to_device=184);
    }
  }
}
