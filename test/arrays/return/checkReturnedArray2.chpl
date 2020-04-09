proc inc(X: [0..2] real): [1..2] real {
  return X;
}

var A = [1.0, 2.0, 3.0];

writeln(inc(A));
writeln(A);
