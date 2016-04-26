config const n = 5;
config const m = 5;
config const o = 5;
config const initialize = true;
config const printOutput = true;
config const printTiming = false;
type indexType = int;

use Random, Time;

proc dgemm(p: indexType,       // number of rows in A
          q: indexType,       // number of cols in A, number of rows in B
          r: indexType,       // number of cols in B
          A: [1..p, 1..q] ?t,
          B: [1..q, 1..r] t,
          C: [1..p, 1..r] t) {
  // Calculate (i,j) using a dot product of a row of A and a column of B.
  const st = getCurrentTime();
  for i in 1..p do
    for j in 1..r do
      for k in 1..q do
        C[i,j] -= A[i, k] * B[k, j];
  const dt = getCurrentTime() - st;
  if printOutput then
    writeln("dgemm:\n", C);
  if printTiming then
    writeln("dgemm: ", dt);
}

proc dgemm_local(p: indexType,       // number of rows in A
                q: indexType,       // number of cols in A, number of rows in B
                r: indexType) {     // number of cols in B
  var A: [1..p,1..q] real;
  var B: [1..q,1..r] real = 1.0;
  var C: [1..p,1..r] real = 1.0;
  if initialize then
    fillRandom(A, 31415, algorithm=RNG.NPB);

  A = A * 2.0 - 1.0;
  // Calculate (i,j) using a dot product of a row of A and a column of B.
  const st = getCurrentTime();
  for i in 1..p do
    for j in 1..r do
      for k in 1..q do
        C[i,j] -= A[i, k] * B[k, j];
  const dt = getCurrentTime() - st;
  if printOutput then
    writeln("dgemm_local:\n", C);
  if printTiming then
    writeln("dgemm_local: ", dt);
}

var A: [1..n,1..m] real;
var B: [1..m,1..o] real = 1.0;
var C: [1..n,1..o] real = 1.0;
if initialize then
  fillRandom(A, 31415, algorithm=RNG.NPB);
A = A * 2.0 - 1.0;

writeln("n=", n, " m=", m, " o=",o);
dgemm(n,m,o,A,B,C);
dgemm_local(n,m,o);
