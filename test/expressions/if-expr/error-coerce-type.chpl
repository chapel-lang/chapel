
// This test exists to ensure the compiler still emits a helpful error message
// even in the case that the type variables could coerce to one another.

config const cond : bool;

var foo = if cond then int else real;
