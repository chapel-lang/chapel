use GPUDiagnostics;

config const n = 10;

config const alpha = 10;

config const writeArrays = false;


startGPUDiagnostics();
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
stopGPUDiagnostics();

const nLaunch = getGPUDiagnostics().kernel_launch;

assert(nLaunch == here.getChildCount()*4);

proc assertElemVal(X, val) {
  if writeArrays {
    // normally we can just do writeln(A), but we don't have a good way of
    // having verbose GPU execution. So, if we do that the output is just too
    // messy with verbose. If we had verbosity for only launches, things would
    // have been much easier.
    write("Array: ");
    for x in X do
      write(x, " ");
    writeln();
  }

  for x in X do assert(x == val, "Expected: ", val, " Actual: ", x);
}
