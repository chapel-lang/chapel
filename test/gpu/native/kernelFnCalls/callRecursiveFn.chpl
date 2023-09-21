config const n = 10;

proc foo(i): int {
  if i > 0 then {
    return 1 + foo(i/2);
  }
  return 0;
}

on here.gpus[0] {
  var A: [0..#n] int;
  forall i in 0..#n with (ref A) {
    A[i] = foo(i);
  }

  writeln(A);
}
