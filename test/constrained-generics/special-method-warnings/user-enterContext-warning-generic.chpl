record R {
    var x;

    proc enterContext() ref: uint {
        return x;
    }
}

type Rint = R(int);
writeln(Rint : string);
