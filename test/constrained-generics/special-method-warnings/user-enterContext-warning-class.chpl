class C {
    var x: uint;

    proc enterContext() ref: uint {
        return x;
    }
}

writeln(new C());
