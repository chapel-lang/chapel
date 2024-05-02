use GpuDiagnostics;

startVerboseGpu();

proc f(x: int) {
    return x + 4;
}

record myRec {
    var x : int;

    proc foo() {
        return x + 5;
    }
}

on here.gpus[0] {
    var Records = [i in 0..0] new myRec(i);

    for bs in 1..128 {
        @gpu.blockSize(bs)
        var A = foreach i in 0..0 do i;

        @gpu.blockSize(bs)
        var B = foreach p in zip(foreach i in 0..0 do i, foreach i in 0..0 do i) do p;

        @gpu.blockSize(bs)
        var C = A + 1;

        @gpu.blockSize(bs)
        var D = A + 1 + 1;

        @gpu.blockSize(bs)
        var E = f(A);

        @gpu.blockSize(bs)
        var F = Records.foo();
    }
}
