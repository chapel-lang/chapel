proc foo(A: []) {
  writeln(A);
}

proc bar(A: [2..4]) {
  for i in 2..4 do
    writeln((i, A(i)));
}

proc bar2(A: [2..4] real) {
  for i in 2..4 do
    writeln((i, A(i)));
}

var A: [1..3] real = (1.0, 2.0, 3.0);
var B: [1..3] int = (1, 2, 3);

foo(A);
foo(B);
bar2(A.reindex({2..4}));
bar(A.reindex({2..4}));
bar(B.reindex({2..4}));

