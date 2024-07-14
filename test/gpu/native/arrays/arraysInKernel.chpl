use GpuDiagnostics;

on here.gpus[0] {
    startGpuDiagnostics();
    @assertOnGpu
    foreach 0..<20 {
        var a : [1..10] int;
    }
    stopGpuDiagnostics();
    assertGpuDiags(kernel_launch=1, host_to_device=0,
                   device_to_host=0, device_to_device=0);
}
