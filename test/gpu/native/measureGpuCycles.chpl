use GPU;

config param howManyResults = 3;
config param showTimingResults = false;

on here.gpus[0] {
  var A : [0..1024] int;
  var clockDiff : [0..<howManyResults] uint;

  foreach i in 0..0 {
    var dblFactor = 1;
    var start, stop : uint;

    // Warm up loop to pull "A" onto the GPU if using
    // unified memory
    for k in 0..1024 * dblFactor {
      A[k % 1024] = A[k % 1024] * k;
    }

    for j in 0..<howManyResults {
      start = gpuClock();
      for k in 0..1024 * dblFactor {
        A[k % 1024] = A[k % 1024] + j * k;
      }
      stop = gpuClock();
      clockDiff[j] = stop - start;
      dblFactor *= 10;
    }
  }

  var isMonotonicallyIncreasing = true;
  for i in 0..<howManyResults {
    if showTimingResults then
      writeln("Diff is: ",
        (clockDiff[i] : real) / (gpuClocksPerSec(0) : real));
    if i > 0 && clockDiff[i] < clockDiff[i-1] then
      isMonotonicallyIncreasing = false;
  }

  // Used as a crude correctness check. If this isn't true then we're likely
  // doing something wrong.
  writeln("Largers loops always took longer: ", isMonotonicallyIncreasing);
}
