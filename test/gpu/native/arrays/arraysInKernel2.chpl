use GpuDiagnostics;

on here.gpus[0] {
    startGpuDiagnostics();
    var Dom = {1..10};
    var dummy : [Dom] int;
    type t = dummy.type;
    @assertOnGpu
    foreach 0..<20 {
        var a : t;
    }
    stopGpuDiagnostics();
    assertGpuDiags(kernel_launch=1, host_to_device=0,
                   device_to_host=0, device_to_device=0);
}
