use GPU;

config const n = 4096;

on here.gpus[0] {
  var A: [1..n] int;

  foreach i in 1..n*10 {
    assertOnGpu();
    A[i] = i;
  }

  writeln(A[n]);
}
