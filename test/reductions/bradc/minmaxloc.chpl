config var n = 9;

var D = {1..n, 1..n};
var A: [D] real;

forall (i,j) in D {
  A(i,j) = i + j/10.0;
}

var (minA, minALoc) = minloc reduce zip(A, A.domain);
writeln("minA = ", minA, ", minALoc = ", minALoc);
var (maxVal, maxValNum) = maxloc reduce zip([i in 1..n] foo(i), 1..n);
writeln("maxVal = ", maxVal, ", maxValNum = ", maxValNum);

proc foo(x) {
  return abs(n/2 - x);
}
