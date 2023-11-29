use GpuDiagnostics;

startVerboseGpu();

on here.gpus[0] {
    var A = 2 * [1,2,3,4,5];
    writeln(A);
}
