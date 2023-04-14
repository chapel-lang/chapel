use GPU;
use Memory.Diagnostics;

startVerboseMem();

on here.gpus[0] {
  var A: [1..10] int;
  foreach i in A.domain {
    assertOnGpu();
    A[i] = i;
  }
  writeln(A);
}
stopVerboseMem();
