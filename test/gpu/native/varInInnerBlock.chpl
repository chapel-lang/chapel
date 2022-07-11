use GPUDiagnostics;

config const n = 10;

startGPUDiagnostics();
on here.getChild(1) {
  var A: [1..n] int;

  foreach a in A {
    for j in 1..10 {
      var x = j;  // this threw off the outliner as the enclosing loop is not
                  // the loop that we are outlining.
      x *= 2;

      a += x;
    }
  }

  writeln(A);
}
stopGPUDiagnostics();

assert(getGPUDiagnostics()[0].kernel_launch == 1);
