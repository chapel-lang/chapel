use Set;

record R {
    var x: uint;

    proc hash(): uint {
        return x.hash();
    }
}
R implements Hashable;

// Make sure R can be used in a set, which requires hashing.
var setOfR: set(R);
