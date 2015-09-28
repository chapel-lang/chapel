proc foo(X: [2..4, 0..2] int) {
  for i in X.domain do
    writeln(i, ": ", X(i));
}

var A: [1..3, 1..3] int;

for i in 1..3 do
  A(i,i) = i;

foo(=>A);
