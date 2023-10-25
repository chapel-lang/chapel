use GpuDiagnostics;

startGpuDiagnostics();

{
  var x = 10;

  on here.gpus[0] {
    var A = [1, 2, 3, 4, 5];
    foreach a in A do a += 1;
    writeln(A); // not in slide
  }

  writeln(x);
}

stopGpuDiagnostics();
assertGpuDiags(kernel_launch=1);
