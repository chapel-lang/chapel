// This test is based on a set of functions
// in the standard library for '**' that were
// causing a surprising ambiguity
// (where the "better than" relation was not transitive).

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
