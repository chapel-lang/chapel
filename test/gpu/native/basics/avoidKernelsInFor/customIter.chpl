use GpuDiagnostics;

var counter = 0;

config const runOnGpu = true;

on if runOnGpu then here.gpus[0] else here {
  var A: [0..100] int = 1;

  startGpuDiagnostics();
  for i in myIter() {  // Loop A
    A[i] += A[i+1];
  }
  stopGpuDiagnostics();
  writeln(A);
}

iter myIter() {
  foreach myI in 0..9 with (ref counter) {  // Loop B
    for myJ in 0..9 {    // Loop C
      counter += 1;  // to make sure that this is not a trivial `for`
      yield myI*10+myJ;
    }
  }
}

assertGpuDiags(kernel_launch=0);
