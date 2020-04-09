proc foo(X: [1..5] int) {
  for i in X.domain do
    writeln(i, ": ", X(i));
}

proc bar(X: [1..20 by 4] int) {
  for i in X.domain do
    writeln(i, ": ", X(i));
}

var A: [1..10 by 2] int;

for i in A.domain do
  A(i) = i;

foo(=>A);

bar(=>A);
