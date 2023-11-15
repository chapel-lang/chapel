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
  Check correctness of `solve_[tril/triu]`.

  Expected results (manually computed):
      solve_tril(A,            b) =  0.5 5.5 5.5
      solve_triu(transpose(A), b) = 15.6 4.4 1.2
*/

var res = true;

// 0-based domain (default)
var A: [0..#3, 0..#3] real;
var At: [0..#3, 0..#3] real;
var b: [0..#3] real;

A[0, ..] = [ 2.0,  0.0, 0.0];
A[1, ..] = [-7.0,  1.0, 0.0];
A[2, ..] = [ 0.5, -2.0, 2.5];

At = transpose(A);

b = [1.0, 2.0, 3.0];

const xl = solve_tril(A, b, false);
const xu = solve_triu(At, b);
if !checkResults(A, xl, b) || !checkResults(At, xu, b) {
  res = false;
  writeln("test 1 failed");
}

// 11-based domain
var A2 = A.reindex(11..#3, 11..#3);
var At2 = At.reindex(11..#3, 11..#3);
var b2 = b.reindex(11..#3);

const xl2 = solve_tril(A2, b2, false);
const xu2 = solve_triu(At2, b2);
if !checkResults(A2, xl2, b2) || !checkResults(At2, xu2, b2) {
  res = false;
  writeln("test 2 failed");
}

// -7-based domain
var A3 = A.reindex(-7..#3, -7..#3);
var At3 = At.reindex(-7..#3, -7..#3);
var b3 = b.reindex(-7..#3);

const xl3 = solve_tril(A3, b3, false);
const xu3 = solve_triu(At3, b3);
if !checkResults(A3, xl3, b3) || !checkResults(At3, xu3, b3) {
  res = false;
  writeln("test 3 failed");
}

if res then writeln("All tests passed");
