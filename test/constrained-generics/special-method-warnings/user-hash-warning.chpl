use Set;

record R {
    var x: uint;

    proc hash(): uint {
        writeln("in hash");
        return x.hash();
    }
}

writeln(new R());
