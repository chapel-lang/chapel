/*
Transpose performance testing

--m=10      --iters=1000
--m=100     --iters=100
--m=1000    --iters=10
--m=10000   --iters=1
*/

use LinearAlgebra;
use BLAS;
use Time;

config const m=1000,
             iters=10,
             reference=false,
             correctness=false;


config type eltType = real;

const nBytes = numBytes(eltType);

proc main() {
  var D = {0..#m, 0..#m*2};
  var A = Matrix(D, eltType=eltType);

  [(i, j) in D] A[i,j] = i - j;

  var t: Timer;

  if !correctness {
    writeln('==========================');
    writeln('Transpose Performance Test');
    writeln('==========================');
    writeln('iters : ', iters);
    writeln('m     : ', m);
    writeln('MB    : ', (nBytes*m*m) / 10**6);
    writeln();
  }

  for 1..iters {
    t.start();
    transpose(A);
    t.stop();
  }

  if !correctness then
    writeln('LinearAlgebra.transpose: ', t.elapsed() / iters);
  t.clear();

  if reference {
    for 1..iters {
      t.start();
      blasTranspose(A);
      t.stop();
    }

    if !correctness then
      writeln('BLAS.transpose: ', t.elapsed() / iters);
    t.clear();
  } else {
    if !correctness then
      writeln('BLAS.transpose: -1');
  }
}


/* BLAS Transpose */
proc blasTranspose(A: [?Dom] ?eltType) {
  var B: [Dom] eltType = eye(Dom, eltType=eltType);
  var C: [transpose(Dom)] eltType;
  gemm(A, B, C, 1:eltType, 0:eltType, opA=Op.T);
  return C;
}
