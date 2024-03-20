// block distribution as special sauce with stream
use BlockDist;

config const m = 10,
             alpha = 3.0;

const ProblemSpace = blockDist.createDomain({1..m});

var A, B, C: [ProblemSpace] real;

B = 2.0;
C = 1.0;

A = B + alpha * C;



// data parallelism
config const n = 10;
{
  var D = {1..n, 1..n};
  var A: [D] real;
  forall (i,j) in D with (ref A) do
    A[i,j] = i + (j - 0.5)/n;
  writeln(A);
}

use CyclicDist;
{
  var D = cyclicDist.createDomain({1..n, 1..n});
  var A: [D] real;
  forall (i,j) in D with (ref A) do
    A[i,j] = i + (j - 0.5)/n;
  writeln(A);
}
