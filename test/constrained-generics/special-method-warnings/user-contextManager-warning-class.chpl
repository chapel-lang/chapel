class C {
    var x: uint;

    proc enterContext() ref: uint {
        return x;
    }
    proc exitContext(in err: owned Error?) {}
}

writeln(new C());
