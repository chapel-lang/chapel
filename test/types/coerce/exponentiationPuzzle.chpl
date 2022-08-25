// This test is based on a set of functions
// in the standard library for '**' that were
// causing a surprising ambiguity
// (where the "better than" relation was not transitive).
//
// This originally came up with the expression
//   2:uint(32)**31
//
// This test is a simplified case both to make
// reasoning about problems in this area easier
// and to avoid new problems in this area if the **
// overloads change in the future.
//
// The point is not which candidate is selected here -
// it is that the "better than" relation is not
// cyclic leading to ambiguity.
proc f(param a: uint(32), param b: uint(32)) {
  writeln("f(param a: uint(32), param b: uint(32))");
}

proc f(a: int(64), param b: int(64)) {
  writeln("f(a: int(64), param b: int(64))");
}

proc f(param a: integral, b: int) {
  writeln("f(param a: integral, b: int)");
}

f(2:uint(32), 31);
