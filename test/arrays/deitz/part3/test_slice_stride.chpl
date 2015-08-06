proc foo(A: [1..3] int) {
  writeln(A);
  for i in 1..3 do
    A(i) = -i;
}

var A: [1..24] int = 1..24;

writeln(A);

writeln(A[5..13 by 3]);

foo(A[5..13 by 3].reindex({1..3}));

writeln(A);
