use GpuDiagnostics;

config const n = 10;

config const alpha = 10;

startGpuDiagnostics();
on here.gpus[0] {
  var A: [1..n] int;
  var B: [1..n] int;
  var C: [1..n] int;

  // Each of the following should cause a kernel launch
  A = 1;                            writeln(A);
  B = 2;                            writeln(B);
  C = 3;                            writeln(C);
  A = B + alpha * C;                writeln(A);
  A = A + A + A + B + B + C;        writeln(A);
  A = B * C + A;                    writeln(A);
  A = foo(A);                       writeln(A);

}
stopGpuDiagnostics();
assert(getGpuDiagnostics()[0].kernel_launch == 7);

proc foo(a: int) {
  return a+1;
}
