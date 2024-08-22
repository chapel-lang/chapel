use GPU;
config const n=1000;
on here.gpus[0] {
  var Arr: [0..<n] int;
  var Dummy: [0..<n] int;
  var CorrectResult: [0..<n] bool;
  @assertOnGpu
  foreach i in Arr.domain {
    const warpIndex = i/32;

    if i%32==0 {
      for j in 0..<32 {
        Arr[i+j] = warpIndex;
      }
      for i in 0..<n { // We do this to try and make the threads out of sync
        Dummy[i] = i;
      }
    }
    syncWarp();

    CorrectResult[i] = Arr[i] == warpIndex;
  }
  // expect all `true`s in `CorrectResult` only if there's a syncWarp call
  // It is out of our control whether a divergence in a warp really occurs
  // on not; that is decided by the thread scheduler.
  // So it is possible that a syncWarp will not be needed, so a passing test
  // doesn't mean that the test is correct unfortunately
  for i in CorrectResult.domain {
    if(CorrectResult[i] == false) {
      writeln("Incorrect result at index ", i);
      break;
    }
  }
}