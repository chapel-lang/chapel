proc inc(X: [0..4] int): (int, (int, [1..6] int)) {
  return (X[0], (X[0], X + 3));
}

var A = [1, 2, 3, 4, 5];

writeln(inc(A));
writeln(A);
