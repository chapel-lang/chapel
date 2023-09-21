use Set;

record R {
    var x;

    proc hash(): uint {
        writeln("in hash");
        return x.hash();
    }
}

type Rint = R(int);
writeln(Rint : string);
