// Test calling isHomogeneousTupleType on a non-tuple type.
// We had a bug where this would cause an internal error when it should return
// false.
writeln(isHomogeneousTupleType(int));
