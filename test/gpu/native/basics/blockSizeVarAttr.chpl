use GpuDiagnostics;

startVerboseGpu();

proc f(x: int) {
    return x + 4;
}

proc g(a: int = 10, b: int, c: int = 20) {
    return a + b + c;
}

record myRec {
    var x : int;

    proc foo() {
        return x + 5;
    }
}

on here.gpus[0] {
    var Records = [i in 0..0] new myRec(i);

    for bs in 32..128 by 32 {
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

        @gpu.blockSize(bs)
        var G = g(1, A, 2);

        @gpu.blockSize(bs)
        var H = g(1, A);
    }
}
