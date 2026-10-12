proc inc(X: [] int): (int, (int, [] int)) {
  return (X[0], (X[0], X + 3));
}

var A = [1, 2, 3, 4, 5];

writeln(inc(A));
writeln(A);
