use LinearAlgebra;

config const m=10;

const Space = {42..#m, 23..#m};
const Space_0 = {0..#m, 0..#m};
const Space_1 = {1..#m, 1..#m};
const Space_neg = {-42..#m, -23..#m};

proc getMatWithDiags(d) {
  var A:[d] real;

  setDiag(A, 0, 2);
  setDiag(A, 1, -1);
  setDiag(A, -1, -1);
  setDiag(A, 6, 0.5);
  setDiag(A, -6, 0.5);
  setDiag(A, m - 1, 0.1);
  setDiag(A, (m - 1) * (-1), 0.1);

  return A;
}

var A = getMatWithDiags(Space);
var A_0 = getMatWithDiags(Space_0);
var A_1 = getMatWithDiags(Space_1);
var A_neg = getMatWithDiags(Space_neg);

writeln(A);
writeln(A_0.equals(A));
writeln(A_1.equals(A));
writeln(A_neg.equals(A));
