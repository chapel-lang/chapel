var A: [1..12] int = 1..12;

foo(A[2..12 by 2].reindex({1..6}));

proc foo(A: [1..6] int) {
  writeln(A);
  bar(A[2..6 by 2].reindex({1..3}));
}

proc bar(A: [1..3] int) {
  writeln(A);
}
