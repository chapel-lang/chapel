use GPU;
use MemDiagnostics;

config param triggerAssert = false;


for gpu in here.gpus do on gpu {
  var A: [1..10] int;
  @assertOnGpu
  foreach i in A.domain {
    A[i] = i;

    if triggerAssert then
      writeln(A[i]);
  }
  writeln(here, " ", A);
}
