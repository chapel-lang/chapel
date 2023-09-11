use GPU;

config const N=10;

on here.gpus[0] {
  var A : [0..<N] int;
  foreach i in 0..<N {
    assertOnGpu();
    A[i] = i;
  }
  writeln(A);
}
