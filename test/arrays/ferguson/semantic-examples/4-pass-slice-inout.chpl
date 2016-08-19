proc run() {
  var A:[1..10] int;

  foo(A[1..3]);
  writeln(A);
}

proc foo(inout A) {
  A[1] = 1;
}

run();
