use GPU;

config const N=10;

on here.gpus[0] {
  var A : [0..<N] int;
  @assertOnGpu
  foreach i in 0..<N {
    A[i] = i;
  }
  writeln(A);
}
