/*
Sparse diagonal creation performance testing
*/

use LinearAlgebra;
use Time;
use BlockDist;
use Math;

config const m=1000000,
             iters=10,
             /* Omit timing output */
             correctness=false;

config type eltType = real;

const bytes = numBytes(eltType);

proc main() {
  const Space = {1..m, 1..m};
  var SparseDom: sparse subdomain(Space);

  var t: Timer;

  if !correctness {
    writeln('=====================================');
    writeln('Sparse Diag Creation Performance Test');
    writeln('=====================================');
    writeln('iters : ', iters);
    writeln('m     : ', m);
    writeln('numLocales     : ', numLocales);
    writeln('MB    : ', (bytes*m*m) / 10**6);
    writeln();
  }

  for 1..iters {
    var A: [SparseDom] real;
    t.start();
    addDiag(A, 0, 2);
    addDiag(A, 1, -1);
    addDiag(A, -1, -1);
    t.stop();
  }

  if !correctness then
    writeln('LinearAlgebra.addDiag: ', t.elapsed() / iters);
  t.clear();
}

