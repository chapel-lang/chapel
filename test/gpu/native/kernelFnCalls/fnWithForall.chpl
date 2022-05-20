/* This is a complicated test. See comments next to foreaches */
use GPUDiagnostics;
config const n = 10;

proc foo(n) {
  var A: [0..n] real;

  foreach i in 0..n { // this foreach prevents `foo` to be called from inside
                      // GPU kernels, but if `foo` is called directly from a
                      // proper `on` statement, this will turn into a kernel
    A[i] = i * 1.5;
  }

  return A[n];
}


startVerboseGPU();
on here.gpus[0] {
  var A: [0..#n] real;

  foreach i in 0..#n {  // This is not a kernel, `foo` is too complicated
    A[i] = foo(i);
  }

  writeln(A);

  var x = foo(n); // This should cause n+1 kernel launches
  writeln(x);
}

var x2 = foo(n);  // This should not cause any kernel launch
stopVerboseGPU();

writeln(x2);
