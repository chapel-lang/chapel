proc dbg_proc() {
    // CHECK: call void @llvm.dbg.declare{{.*}}
    const x = 42.42;
    var b = 42;
    writeln(x);
    writeln(b);
    return x + b;
}

dbg_proc();
