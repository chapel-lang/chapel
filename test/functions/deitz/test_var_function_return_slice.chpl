var A: [1..5] int;

proc foo() ref {
  return A(2..4);
}

foo() = 2;

writeln(A);
