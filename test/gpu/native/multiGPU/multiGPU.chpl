use GPUDiagnostics;

config const n = 10;

config const alpha = 10;

config const writeArrays = true;


startGPUDiagnostics();
for subloc in 0..<here.getChildCount() do on here.gpus[subloc] {
  var A: [1..n] int;
  var B: [1..n] int;
  var C: [1..n] int;

  // each of these are one launch
  A = 1;                            writeArr(A);
  B = 2;                            writeArr(B);
  C = 3;                            writeArr(C);
  A = B + alpha * C;                writeArr(A);
}
stopGPUDiagnostics();

const nLaunch = getGPUDiagnostics().kernel_launch;

assert(nLaunch == here.getChildCount()*4);

proc writeArr(A) {
  if writeArrays {
    // normally we can just do writeln(A), but we don't have a good way of
    // having verbose GPU execution. So, if we do that the output is just too
    // messy with verbose. If we had verbosity for only launches, things would
    // have been much easier.
    write("Array: ");
    for a in A do
      write(a, " ");
    writeln();
  }
}
