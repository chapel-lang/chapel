proc foo(A: [?D] int) {
  for i in D do
    writeln(A(i));
}

var A: [1..5] int = (3, 1, 4, 1, 6);

foo(A);
