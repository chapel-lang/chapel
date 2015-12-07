// Test calling isHomogeneousTupleType on a non-tuple type.
// If isTupleType was called first, because of short-circuiting this would work
// when dropping the isTupleType call would cause the test to fail.
writeln(isTupleType(int) && isHomogeneousTupleType(int));
