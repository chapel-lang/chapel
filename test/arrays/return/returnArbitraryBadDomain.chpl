proc inc(X: [1..5] int): [1..6] {
  return X + 1;
}

var A = [1, 2, 3, 4, 5];

writeln(inc(A));
writeln(A);
