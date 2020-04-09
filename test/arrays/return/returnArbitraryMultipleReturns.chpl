proc inc(X: [] real, two: bool): [] real {
  if (two) {
    var ret = X + 2;
    return ret;
  } else {
    var ret = X + 1;
    return ret;
  }
}

var A = [1.0, 2.0, 3.0];

writeln(inc(A, false));
writeln(A);
writeln(inc(A, true));
writeln(A);
