use Set;

record R {
    var x: uint;
}
// Compiler should be able to auto-generate Hashable for compatible interfaces.

// Make sure R is usable for hashing.
chpl__defaultHashWrapper(new R());
