use GpuDiagnostics;

config param useForall = false;

proc yepGPU(A) {
  if useForall then
    forall a in A do a += 1;
  else
    foreach a in A do a += 1;
}

// NOTE: the pragma is implemented with mostly coforall wrappers in mind, which
// have single invocation. So, if you rewrite this test with the function bodies
// placed in a single function `foo`, and call `foo` from `yepGPU` and
// `nopeGPU`, the pragma will not function properly. (And that's OK for now)
// Another way to put it is that, this pragma only applies to order-independent
// loops within this function's body.
pragma "no gpu codegen"
proc nopeGPU(A) {
  if useForall then
    forall a in A do a += 1;
  else
    foreach a in A do a += 1;
}

on here.gpus[0] {
  var A: [1..10] int;

  startGpuDiagnostics();
  yepGPU(A);
  stopGpuDiagnostics();
  assert(getGpuDiagnostics()[0].kernel_launch == 1);
  writeln(A);

  resetGpuDiagnostics();
  startGpuDiagnostics();
  nopeGPU(A);
  stopGpuDiagnostics();
  assert(getGpuDiagnostics()[0].kernel_launch == 0);
  writeln(A);
}
