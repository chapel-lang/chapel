// Before, this function was only defined for an argument of tuple type.
// This assumed that a non-tuple type would never be sent to the function, but
// that seems like an unnecessary assumption.
writeln(isHomogeneousTupleValue(4));
