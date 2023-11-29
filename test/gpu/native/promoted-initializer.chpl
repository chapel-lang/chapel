use GpuDiagnostics;

startGpuDiagnostics();

on here.gpus[0] {
    var A = 2 * [1,2,3,4,5];
    writeln(A);
}

stopGpuDiagnostics();

writeln(+ reduce getGpuDiagnostics().kernel_launch);
