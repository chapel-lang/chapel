config const n = 10;

proc foo(i): int {
  if i > 0 then {
    return 1 + foo(i/2);
  }
  return 0;
}

on here.getChild(1) {
  var A: [0..#n] int;
  forall i in 0..#n {
    A[i] = foo(i);
  }

  writeln(A);
}
