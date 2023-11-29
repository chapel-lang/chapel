on here.gpus[0] {
    [i in 1..100];
    f();
    g();
    h();
    i();
}

proc f() {
    [i in 1..100];
    [i in 1..100];
}

proc g() {
    [i in 1..100];
    var A: [0..#100] int;
    foreach a in A {
        a += 1;
        // __primitive("gpu kernel name", "my_gpu_kernel");
    }
}

inline proc h() {
    [i in 1..100];
    [i in 1..100];
}

proc i() {
    var A: [0..#100] int;
    A = 1;
}
