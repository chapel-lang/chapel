var x = [1, 2, 3, 4];

foo(x.reshape({1..2, 1..2}));

writeln(x);

bar(x.reshape({1..2, 1..2}));

writeln(x);

proc foo(in X: [] ) {
  writeln(X);
  X[1, 1] = 5;
}

proc bar(ref X: []) {
  writeln(X);
  X[1, 2] = 6;
}
