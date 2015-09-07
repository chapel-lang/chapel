var D = {1..8};
var A: [D] int = D;

writeln(A);

writeln(A(1..8 by 6));

proc foo(A: [3..6 by 2] int) {
  writeln(A);
}

foo(A(1..8 by 6).reindex({3..6 by 2}));

