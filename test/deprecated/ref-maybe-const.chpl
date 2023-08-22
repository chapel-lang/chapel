// infered to be const ref
proc foo(A) {
  return A;
}

// without ref gets infered to be const ref
proc bar(A) { // should fail without ref
  A = 1;
}

// without ref on the var args
proc foobar(args...) {
  args[0] = 2;
}

proc main() {
  var A: [1..10] int;
  A = 1..10;
  writeln("A, ", A);

  var B = foo(A);
  writeln("B, ", B);

  bar(A);
  writeln("A, ", A);

  foobar(A);
  writeln("A, ", A);
}
