var x = [1, 2, 3, 4];

ref xref = reshape(x, {1..2, 1..2});
foo(xref);

writeln(x);

bar(xref);

writeln(x);

proc foo(in X: [] ) {
  writeln(X);
  X[1, 1] = 5;
}

proc bar(ref X: []) {
  writeln(X);
  X[1, 2] = 6;
}
