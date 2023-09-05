record R {
    var x: uint;

    proc enterContext() ref: uint {
        return x;
    }

    proc exitContext(in error: owned Error?) {}
}

writeln(new R());
