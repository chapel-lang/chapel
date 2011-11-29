var A: [1..5] int;

proc foo() var {
  return A(2..4);
}

foo() = 2;

writeln(A);
