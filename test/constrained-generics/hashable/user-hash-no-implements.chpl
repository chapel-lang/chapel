use Set;

record R {
    var x: uint;

    proc hash(): uint {
        return x.hash();
    }
}
// No implements statement; the user provided a hash proc but didn't make it
// special, so standard library routines that require hash should not work.

// Check that R is hashable by calling the hash function.
chpl__defaultHashWrapper(new R());
