proc inc(X: [] real, two: bool): [] real {
  if (two) {
    var ret = X + 2;
    return ret;
  } else {
    var ret = X + 1;
    return ret;
  }
}

proc incByTuple(X: [] real, two: bool): ([] real, ([] real, [] real)) {
  if (two) {
    var ret = X + 2;
    return (ret, (ret + 3, X + 4));
  } else {
    var ret = X + 1;
    return (ret, (X + 2, ret + 3));
  }
}

var A = [1.0, 2.0, 3.0];

writeln(inc(A, false));
writeln(incByTuple(A, false));
writeln(A);
writeln(inc(A, true));
writeln(incByTuple(A, true));
writeln(A);
