proc foo(const ref x) { }
proc dbg_proc() {
    // CHECK: #dbg_value{{.*}}
    const x = 42.42;
    foo(x);
    var b = 42;
    writeln(x);
    writeln(b);
    return x + b;
}

dbg_proc();
