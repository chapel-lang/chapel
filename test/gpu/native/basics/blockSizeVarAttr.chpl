use GpuDiagnostics;

startVerboseGpu();

on here.gpus[0] {
    for bs in 1..128 {
        @gpu.blockSize(bs)
        var A = foreach i in 0..0 do i;
    }
}
