use GPUDiagnostics;

writeln("Start");

startVerboseGPU();
startGPUDiagnostics();

on here.getChild(1) {
  var A: [1..10] int;
  foreach a in A {
    a += 1;
  }

  writeln(A);
}

stopGPUDiagnostics();
stopVerboseGPU();

writeln("End");

writeln("GPU diagnostics:");
writeln(getGPUDiagnostics());
