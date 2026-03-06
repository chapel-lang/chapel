// without ref on the var args
proc foobar(args...) {
  args[0] = 2;
}

proc main() {
  var A: [1..10] int;
  A = 1..10;
  writeln("A, ", A);

  foobar(A);
  writeln("A, ", A);
}
