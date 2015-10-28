proc foo(A: [1..10] int) {
  writeln(A);
}

var A: [1..2, 1..5] int;
foo(A.reindex({1..10}));
