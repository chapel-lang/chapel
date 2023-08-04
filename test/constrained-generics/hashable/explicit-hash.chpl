use Set;

record R {
    var x: uint;

    proc hash(): uint {
        writeln("in hash");
        return x.hash();
    }
}
R implements Hashable;

// Make sure R is usable for hashing.
chpl__defaultHashWrapper(new R());
