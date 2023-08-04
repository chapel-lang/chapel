on here.gpus[0] {
    writeln(+ reduce (1..100));
    f();
    g();
    h();
    i();
}

proc f() {
    writeln(+ reduce (1..100));
    writeln(+ reduce (1..100));
}

proc g() {
    writeln(+ reduce (1..100));
    var A: [0..#100] int;
    foreach a in A {
        a += 1;
        // __primitive("gpu kernel name", "my_gpu_kernel");
    }
}

inline proc h() {
    writeln(+ reduce (1..100));
    writeln(+ reduce (1..100));
}

proc i() {
    var A: [0..#100] int;
    A = 1;
}
