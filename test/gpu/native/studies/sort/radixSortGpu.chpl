module RadixSort {
  import BitOps;
  use GPU;

  config const useGpuId = 0;
  config const noisy = false;
  config const scanTest = false;
  config const scanType = "blelloch"; // "serial", "hillisSteele", "blelloch"
  config const bitsAtATime = 4; // 1-8 are all reasonable values
                                // but for some reason 16 performs best.
                                // My guess is because we're dealing with
                                // uint(64) it makes sense for 16 to do well
                                // since its a nice even factor and causes
                                // only 4 iterations of a single radix sort step
  // How many bits we sort at once based on the bitsAtATime
  const buckets = 1 << bitsAtATime; // 2^bitsAtATime, ex: 2^4 = 16 = 0b10000
  const bitMask = buckets - 1; // 2^bitsAtATime - 1, ex: 2^4 - 1 = 15 = 0b1111

  proc serialScan(ref prefixSum: [] uint, buckets: int){
    // Convert this count array into a prefix sum
    // This is the same as the count array, but each element is the sum of all previous elements
    // This is an exclusive scan
    // Serial implementation

    // Calculate the prefix sum
    var sum : uint = 0;
    for i in 0..<buckets {
      var t : uint = prefixSum[i];
      prefixSum[i] = sum;
      sum += t;
    }
  }

  proc hillisSteeleScan(ref hostPrefixSum: [] uint, hostBuckets: int){
    // Hillis Steele Scan
    // This is the same as the count array, but each element is the sum of all previous elements
    // Uses a naive algorithm that does O(nlogn) work
    // Hillis and Steele (1986)
    on here.gpus[useGpuId]{
      var buckets = hostBuckets;
      var prefixSum = hostPrefixSum;
      var offset = 1;
      while offset <= buckets {
          var prefixSumBuffer = prefixSum;
          @assertOnGpu
          forall i in offset..<buckets { // foreach causes error? Investigate later
            prefixSum[i] = prefixSumBuffer[i] + prefixSumBuffer[i-offset];
          }
          offset = offset << 1;
      }

      // Change inclusive scan to exclusive
      var prefixSumBuffer = prefixSum;
      foreach i in 1..<buckets {
        prefixSum[i] = prefixSumBuffer[i-1];
      }
      prefixSum[0] = 0;

      // This code is a more reader friendly way to do the same thing as above
      // const log_2_buckets = numBytes(uint)*8 - BitOps.clz(buckets); // Calculates the log quickly
      // for d in 1..log_2_buckets {
      //   @assertOnGpu
      //   foreach i in 0..<buckets {
      //     if i >= 1 << d { // 2^d
      //       prefixSum[i] = prefixSum[i-(1 << (d-1))] + prefixSum[i];
      //     }
      //   }
      // }
      hostPrefixSum=prefixSum;
    }
  }

  proc blellochScan(ref hostprefixSum: [] uint, hostbuckets: int){
      // Blelloch Scan
      // This is the same as the count array, but each element is the sum of all previous elements
      // Uses a more efficient algorithm that does O(n) work
      // Blelloch (1990)
      // Up-sweep
      on here.gpus[useGpuId]{
        var buckets = hostbuckets;
        var prefixSum = hostprefixSum;
        var offset = 1;
        while offset < buckets {
          var prefixSumBuffer = prefixSum;
          @assertOnGpu
          foreach idx in 0..<buckets/(2*offset) {
            const doubleOff = offset << 1;
            const i = idx*doubleOff;
            prefixSum[i+doubleOff-1] = prefixSumBuffer[i+offset-1] + prefixSumBuffer[i+doubleOff-1];
          }
          offset = offset << 1;
        }

        // Down-sweep
        prefixSum[buckets-1] = 0;
        offset = buckets >> 1;
        while offset > 0 {
          var prefixSumBuffer = prefixSum;
          @assertOnGpu
          foreach idx in 0..<buckets/(2*offset) {
            const i = idx*2*offset;
            const t = prefixSumBuffer[i+offset-1];
            prefixSum[i+offset-1] = prefixSumBuffer[i+2*offset-1];
            prefixSum[i+2*offset-1] = prefixSum[i+2*offset-1] + t;
          }
          offset = offset >> 1;
        }
        hostprefixSum = prefixSum;
      }
  }

  proc arrScan(ref count: [?D] uint, ref inputArr: [] uint, exp: int, bitMask: int, buckets: int){
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


    if scanType == "hillisSteele" then
      hillisSteeleScan(count, buckets);
    else if scanType == "blelloch" then
      blellochScan(count, buckets);
    else if scanType == "serial" then
      serialScan(count, buckets);

    if scanTest then
      writeln("Pf sum: ",count);
  }

  proc radixSort(inputArr: [] uint){
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

      arrScan(prefixSum, inputArr, exp, bitMask, buckets);

      if noisy then
        writeln("       Prefix sum: ", prefixSum);
      // Use the prefix sum to reorder the array at the exp position
      // This is done by iterating through the array backwards
      // and placing each element in the correct position in the output array


      // on here.gpus[useGpuId]{
      //   var gpuPrefixSum = prefixSum;
      //   var gpuInputArr = inputArr;
      //   var gpuOutputArr : outputArr.type;
      //   var gpuExp = exp;
      //   @assertOnGpu
      //   foreach i in gpuInputArr{
      //     const idx = ((i>>gpuExp) & bitMask):int;
      //     const arrIdx = gpuAtomicAdd(gpuPrefixSum[idx], 1);
      //     gpuOutputArr[arrIdx:int] = i;
      //   }
      //   outputArr = gpuOutputArr;
      // }

      for i in inputArr{
        const idx = ((i>>exp) & bitMask):int;
        outputArr[prefixSum[idx]:int] = i;
        prefixSum[idx] += 1;
      }



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
  config const verboseGpu = false;

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
    if !perftest then
      writeln("Sorted");
  }

  proc main() {
    import Random;
    import Time;
    if !perftest then
      startGpuDiagnostics();
    if verboseGpu then
      startVerboseGpu();

    var timer: Time.stopwatch;
    var arr: [0..<arrSize] uint;
    var seed = 17; // Make it deterministic
    Random.fillRandom(arr, seed);
    if printArr then
      writeln(arr);

    timer.start();
    RadixSort.radixSort(arr);
    timer.stop();

    if printArr then
      writeln(arr);

    if perftest then
      writeln("Time: ", timer.elapsed());
    if validate then
      checkSorted(arr);


    if !perftest then {
      stopGpuDiagnostics();
      if verboseGpu then
        stopVerboseGpu();
      if scanType == "hillisSteele" then
        assertGpuDiags(kernel_launch=76, host_to_device=12,device_to_host=8,
                                                      device_to_device=72);
      else if scanType == "blelloch" then
        assertGpuDiags(kernel_launch=136, host_to_device=12,device_to_host=8,
                                                      device_to_device=128);
      else if scanType == "serial" then
        assertGpuDiags(kernel_launch=8, host_to_device=4,device_to_host=4,
                                                      device_to_device=0);

    }
  }
}
