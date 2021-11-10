config const n = 10;

proc foo(i) {
  return 1 + bar(i);
}

proc bar(i) {
  return i / 2;
}

on here.getChild(1) {
  var A: [0..#n] int;

  forall i in 0..#n {
    A[i] = foo(i);
  }

  writeln(A);
}
