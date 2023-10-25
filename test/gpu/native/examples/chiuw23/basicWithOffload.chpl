use GpuDiagnostics;

inline proc check(ref Arr) {
  writeln(Arr);
}

startGpuDiagnostics();

{
  var x = 10;
  var AHost = [1, 2, 3, 4, 5];

  on here.gpus[0] {
    var A = AHost;
    foreach a in A do a += 1;

    AHost = A;  // not in slide
  }

  writeln(x);
  writeln(AHost); // not in slide
}

stopGpuDiagnostics();
assertGpuDiags(kernel_launch=1);
