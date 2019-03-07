proc inc(X: [] int): [] real {
  return X + 1;
}

var A = [1, 2, 3];

writeln(inc(A));
writeln(A);
