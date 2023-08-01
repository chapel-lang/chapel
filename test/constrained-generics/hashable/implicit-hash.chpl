use Set;

record R {
    var x: uint;
}
// Compiler should be able to auto-generate Hashable for compatible interfaces.

// Make sure R can be used in a set, which requires hashing.
var setOfR: set(R);
