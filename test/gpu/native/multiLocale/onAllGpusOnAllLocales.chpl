// Do a kernel launch on all GPUS across all locales

use GpuDiagnostics;

config param debug = false;

startGpuDiagnostics();
coforall loc in Locales do on loc {
  coforall subloc in here.gpus do on subloc {
    var A : [0..10] int;
    foreach i in 0..10  do A[i] = i * 2;
    if debug then writeln("on ", here, " A = ", A);
  }

  // Arbitrarily have locale 0 do an extra kernel launch
  // so we can see a difference in kernel launch count
  // bewteen Locale[0] and other locales.
  if(loc == Locales[0]) then on here.gpus[0] {
    var A : [0..10] int;
    foreach i in 0..10  do A[i] = i * 2;
    if debug then writeln("** on ", here, " A = ", A);
  }
}
stopGpuDiagnostics();

// Some test conditions to verify that we had the number of kernel launches we
// expected:

// TODO we do mix of things to confirm correct number of launches across
// multiple locales here. Unify how we do such checks.

// in order for the test to be meaningful we need to run it on a system that has
// more than 1 GPU per locale:
writeln("here.gpus.size > 1:  ", here.gpus.size > 1);

// Should match the number of locales
var numLaunches = getGpuDiagnostics().kernel_launch;
writeln("numLaunches.size == Locales.size: ", numLaunches.size == Locales.size);

use ChplConfig;

if CHPL_GPU_MEM_STRATEGY == "array_on_device" {
  // We expect the first locale to have 2 more launch than all subsequent
  // locales because of the explicit launch + array initializations
  assert(numLaunches[0] == numLaunches[1] + 2);

  // We expect the second locale to have launches equal to the number of GPUs
  // per node*2 (we're assuming all nodes have an equal number of GPUs), where
  // *2 is for array initialization.
  assert(numLaunches[1] == here.gpus.size*2);
}
else {
  // We expect the first locale to have 1 more launch than all subsequent
  // locales because of the explicit launch
  assert(numLaunches[0] == numLaunches[1] + 1);

  // We expect the second locale to have launches equal to the number of GPUs
  // per node (we're assuming all nodes have an equal number of GPUs)
  assert(numLaunches[1] == here.gpus.size);

}

// Ensure that for all but the first locale the number of kernel launches
// on each locale matches
var foundMismatch = false;
for i in 1..<numLaunches.size-1 do
  if numLaunches[i] != numLaunches[i+1] then foundMismatch = true;
writeln("numLaunches[i] == numLaunches[i+1] for all i > 0?:  ", !foundMismatch);
