proc inc(X: [] real): ([] int, [] int) {
  return (X + 1, X + 2);
}

var A = [1.0, 2.0, 3.0];

writeln(inc(A));
writeln(A);
