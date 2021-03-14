//
// A test for multiple mains with different signatures.
// Used to ensure that the zero argument main is chosen as the
// program entry point and that main able to be overloaded,
// but now checks that we get the right error.
//
proc main(i: int) {
  writeln("main with int argument ", i);
}

proc main() {
  writeln("zero argument main");
  main(3);
}

