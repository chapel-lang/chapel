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

  for i in 0..<howManyResults {
    if showTimingResults then
      writeln("Diff is: ",
        (clockDiff[i] : real) / (gpuClocksPerSec(0) : real));
    if clockDiff[i] <= 0 {
      writeln("Unexpected negative or zero result: i= ", i,  " clockDiff[i]=",
        clockDiff[i]);
    }
  }
}
