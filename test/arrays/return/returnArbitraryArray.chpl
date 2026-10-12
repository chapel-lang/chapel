proc inc(X: [] real): [] real {
  return X + 1;
}
proc incByTuple(X: [] real): (real, [] real) {
  return (X[1], X + 1);
}
proc incByTuple2(X: [] real): ([] real, ([] real, [] real)) {
  return (X + 1, (X + 2, X + 3));
}
proc incByTuple3(X: [] real): (real, ([] real, [] real), [] real) {
  return (X[1], (X + 2, X + 3), X + 4);
}

var A = [1.0, 2.0, 3.0];

writeln(inc(A));
writeln(incByTuple(A));
writeln(incByTuple2(A));
writeln(incByTuple3(A));
writeln(A);
