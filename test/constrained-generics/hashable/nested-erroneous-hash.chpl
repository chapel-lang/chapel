// What if Outer has a proper (generated) interface implementation + hash function,
// but Inner does not? We still need to warn.

record Inner {
    proc hash(): uint {
        return 0;
    }
}
// Inner doesn't implement hashable explicitly and thus this should warn.

record Outer {
    var i: Inner;
}
// Outer should have a purely compiler-generated hash, and itself needs no warning.

chpl__defaultHashWrapper(new Outer());
