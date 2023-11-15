use LinearAlgebra;

proc checkResults(A, x, b) {
  var v = dot(A, x);
  for (vv, bb) in zip(v, b) {
    if (vv != bb) then return false;
  }
  return true;
}

/*
  Check correctness of `solve_tril`.
  Expected result: 0.5 5.5 5.5 (manually verified)
*/

var res = true;

// 0-based domain (default)
var A: [0..#3, 0..#3] real;
var b: [0..#3] real;

A[0, ..] = [ 2.0,  0.0, 0.0];
A[1, ..] = [-7.0,  1.0, 0.0];
A[2, ..] = [ 0.5, -2.0, 2.5];

b = [1.0, 2.0, 3.0];

const x = solve_tril(A, b, false);
if !checkResults(A, x, b) {
  res = false;
  writeln("test 1 failed");
}

// 11-based domain
var A2 = A.reindex(11..#3, 11..#3);
var b2 = b.reindex(11..#3);

const x2 = solve_tril(A2, b2, false);
if !checkResults(A2, x2, b2) {
  res = false;
  writeln("test 2 failed");
}

// -7-based domain
var A3 = A.reindex(-7..#3, -7..#3);
var b3 = b.reindex(-7..#3);

const x3 = solve_tril(A3, b3, false);
if !checkResults(A3, x3, b3) {
  res = false;
  writeln("test 3 failed");
}

if res then writeln("All tests passed");
