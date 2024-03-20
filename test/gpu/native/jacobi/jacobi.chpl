use GpuDiagnostics;
use CTypes;
use GPU; // just to check the unstability warning


config const nSteps = 10;
config const n = 10;

startGpuDiagnostics();

writeln("on GPU:");
jacobi(here.gpus[0]);
writeln("on CPU:");
jacobi(here);

stopGpuDiagnostics();

/* We expect 21 kernel launches from the GPU jacobi call here. 1 for the
   initialization forall loop + (2 for the computation forall loops *
                                 10 for nSteps)
   With array_on_device, you'll get 2 more for array inits.
*/
assertGpuDiags(kernel_launch_um=21, kernel_launch_aod=23);

proc jacobi(loc) {
  on loc {
    var A, B: [0..n+1] real;

    A[0] = 1; A[n+1] = 1;
    forall i in 1..n with (ref A) { A[i] = i:real; }

    for step in 1..nSteps {
      forall i in 1..n with (ref B) { B[i] = 0.33333 * (A[i-1] + A[i] + A[i+1]); }
      forall i in 1..n with (ref A) { A[i] = 0.33333 * (B[i-1] + B[i] + B[i+1]); }
    }
    writeln(A);
  }
}
