// without ref gets infered to be const ref
proc bar(A) { // should fail without ref
  A = 1;
}

proc main() {
  var A: [1..10] int;
  A = 1..10;
  writeln("A, ", A);

  bar(A);
  writeln("A, ", A);
}
