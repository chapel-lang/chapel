var x = 1;

proc f1() {
    x = 2;
}

proc f2() {
    f1();
}

proc f3() {
    f2();
}

on here.gpus[0] {
    @assertOnGpu
    foreach i in 1..100 {
        f3();
    }
}
