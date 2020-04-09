proc inc(X: [] real): [:domain(1)] real {
  return X + 1;
}

var A = [1.0, 2.0, 3.0];

writeln(inc(A));
writeln(A);
