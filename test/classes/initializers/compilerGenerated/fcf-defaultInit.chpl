// Exercises the behavior of first class functions in a --force-initializers
// world, since they are represented in the compiler (or were when I checked in
// Chapel 1.16) via class types.
proc foo() {
  writeln("Hello!");
}

proc baz(f) {
  f();
}

baz(foo);
