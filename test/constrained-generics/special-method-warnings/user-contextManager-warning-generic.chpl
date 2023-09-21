record R {
    var x;

    proc enterContext() ref: uint {
        return x;
    }
    proc exitContext(in err: owned Error?) {}
}

type Rint = R(int);
writeln(Rint : string);
