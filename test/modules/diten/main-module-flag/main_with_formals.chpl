//
// A test to ensure that the zero argument main is chosen as the
// program entry point and that main able to be overloaded
//
proc main(i: int) {
  writeln("main with int argument ", i);
}

proc main() {
  writeln("zero argument main");
  main(3);
}

