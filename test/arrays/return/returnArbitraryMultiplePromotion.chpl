proc inc(X: [] real, two: bool): [] real {
  if (two) {
    return X + 2;
  } else {
    return X + 1;
  }
}
proc inc2(X: [] real, two: bool): ([] real, ([] real, [] real)) {
  if (two) {
    return (X + 2, (X + 3, X + 4));
  } else {
    return (X + 1, (X + 2, X + 3));
  }
}

var A = [1.0, 2.0, 3.0];

writeln(inc(A, false));
writeln(A);
writeln(inc(A, true));
writeln(A);
writeln(inc2(A, false));
writeln(A);
writeln(inc2(A, true));
writeln(A);
