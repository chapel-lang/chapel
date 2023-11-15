use LinearAlgebra;

const eps = 1e-7;

proc checkResults(A, x, b) {
  var v = dot(A, x);
  for (vv, bb) in zip(v, b) {
    if (abs(vv - bb) > eps) then return false;
  }
  return true;
}

/*
  Check correctness of `solve` (calling `lu`).

  Expected results (manually computed): 5.0 -2.0 3.0
*/

var res = true;

// 0-based domain (default)
var A: [0..#3, 0..#3] real;
var b: [0..#3] real;

A[0, ..] = [ 2.0,  1.0, 3.0];
A[1, ..] = [ 4.0, -1.0, 3.0];
A[2, ..] = [-2.0,  5.0, 5.0];

b = [17.0, 31.0, -5.0];

const x = solve(A, b);
b = [17.0, 31.0, -5.0]; // `solve` overrites b
if !checkResults(A, x, b) {
  res = false;
  writeln("test 1 failed");
}

// 11-based domain
var A2 = A.reindex(11..#3, 11..#3);
var b2 = b.reindex(11..#3);

const x2 = solve(A2, b2); // `solve` overrites b2
b2 = [17.0, 31.0, -5.0];
if !checkResults(A2, x2, b2) {
  res = false;
  writeln("test 2 failed");
}

// -7-based domain
var A3 = A.reindex(-7..#3, -7..#3);
var b3 = b.reindex(-7..#3);

const x3 = solve(A3, b3); // `solve` overrites b3
b3 = [17.0, 31.0, -5.0];
if !checkResults(A3, x3, b3) {
  res = false;
  writeln("test 3 failed");
}

if res then writeln("All tests passed");
