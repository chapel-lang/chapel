proc inc(X: [] real, two: bool): [] real {
  if (two) {
    return X + 2;
  } else {
    return X + 1;
  }
}

var A = [1.0, 2.0, 3.0];

writeln(inc(A, false));
writeln(A);
writeln(inc(A, true));
writeln(A);
