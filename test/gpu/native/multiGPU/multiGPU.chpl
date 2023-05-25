use GpuDiagnostics;

config const n = 10;

config const alpha = 10;

config const writeArrays = false;


startGpuDiagnostics();
for gpu in here.gpus do on gpu {
  var A: [1..n] int;
  var B: [1..n] int;
  var C: [1..n] int;

  // each of these are one launch
  A = 1;                            assertElemVal(A, 1);
  B = 2;                            assertElemVal(B, 2);
  C = 3;                            assertElemVal(C, 3);
  A = B + alpha * C;                assertElemVal(A, 2+alpha*3);
}
stopGpuDiagnostics();

const nLaunch = getGpuDiagnostics().kernel_launch;

assert(nLaunch == here.gpus.size*4);

proc assertElemVal(X, val) {
  if writeArrays {
    writeln(X);
  }

  for x in X do assert(x == val, "Expected: ", val, " Actual: ", x);
}
