/* This is a complicated test. See comments next to foreaches */
use GpuDiagnostics;
config const n = 10;

proc foo(n) {
  stopVerboseGpu(); // we're not focusing on this array's kernels
  var A: [0..n] real;
  startVerboseGpu();

  foreach i in 0..n { // this foreach prevents `foo` to be called from inside
                      // GPU kernels, but if `foo` is called directly from a
                      // proper `on` statement, this will turn into a kernel
    A[i] = i * 1.5;
  }

  return A[n];
}


const host = here;

startVerboseGpu();
on here.gpus[0] {
  stopVerboseGpu(); // we're not focusing on this array's kernels
  var A: [0..#n] real;
  startVerboseGpu();

  foreach i in 0..#n {  // This is not a kernel, `foo` is too complicated
    A[i] = foo(i);      // This will cause n kernel launches from `foo`
  }

  on host { // copy data back to avoid interleaving output with verbose GPU
            // chatter
    var locA = A;
    writeln(locA);
  }

  var x = foo(n); // This should cause 1 more launch
  writeln(x);
}

var x2 = foo(n);  // This should not cause any kernel launch
stopVerboseGpu();

writeln(x2);
