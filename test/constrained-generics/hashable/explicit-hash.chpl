use Set;

record R : Hashable {
    var x: uint;

    proc hash(): uint {
        writeln("in hash");
        return x.hash();
    }
}

// Make sure R is usable for hashing.
chpl__defaultHashWrapper(new R());
