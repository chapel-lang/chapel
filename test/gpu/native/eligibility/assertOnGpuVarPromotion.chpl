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
    var Source = [1,2,3,4,5];

    @assertOnGpu
    var A = Source + 1;
    writeln(A);

    @assertOnGpu
    var B = (foreach i in 1..5 do i) + 2;
    writeln(B);

    @gpu.blockSize(128)
    var C = (foreach i in 1..5 do i) + 3;
    writeln(C);

    @assertOnGpu
    var D = f(foreach i in 1..5 do i);
    writeln(D);

    var SourceRec = [i in Source] new myRec(i);

    // Disabled due to an unrelated bug:
    //     https://github.com/chapel-lang/chapel/issues/24989
    //
    // @assertOnGpu
    // @gpu.blockSize(64);
    // var E = SourceRec.foo();
    // writeln(E);
}
