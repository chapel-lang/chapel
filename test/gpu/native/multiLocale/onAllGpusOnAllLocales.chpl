// Do a kernel launch on all GPUS across all locales

use GPUDiagnostics;

config param debug = false;

startGPUDiagnostics();
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
stopGPUDiagnostics();

// Some test conditions to verify that we had the number of kernel launches we
// expected:

// in order for the test to be meaningful we need to run it on a system that has
// more than 1 GPU per locale:
writeln("here.gpus.size > 1:  ", here.gpus.size > 1);

// Should match the number of locales specified by NUMLOCALES file
var numLaunches = getGPUDiagnostics().kernel_launch;
writeln("numLaunches.size:   ", numLaunches.size);

// We expect the first locale to have 1 more launch than all subsequent locales
writeln("numLaunches[0] == numLaunches[1] + 1:  ",
  numLaunches[0] == numLaunches[1] + 1);

// We expect the second locale to have launches equal to the number of GPUs per
// node (we're assuming all nodes have an equal number of GPUs)
writeln("numLaunches[1] == here.gpus.size:  ", numLaunches[1] == here.gpus.size);

// Ensure that for all but the first locale the number of kernel launches
// on each locale matches
var foundMismatch = false;
for i in 1..<numLaunches.size-1 do
  if numLaunches[i] != numLaunches[i+1] then foundMismatch = true;
writeln("numLaunches[i] == numLaunches[i+1] for all i > 0?:  ", !foundMismatch);
