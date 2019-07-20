/*
Dense matrix inverse performance testing
*/

use LinearAlgebra;
use Time;
use Math;

config const m=1000,
             iters=10,
             correctness=false;

config type eltType = real;

const bytes = numBytes(eltType);

proc addDiag (ref X: [?D] ?eltype, in p: int = 0, val: eltype = 0) {
  var start, end = 0;
  if (p >= 0) { // upper or main diagonal
    start = 1;
    end = D.shape(1) - p;
  }
  else { // lower diagonal
    start = 1 - p;
    end = D.shape(1);
  }
  forall row in {start..end} {
    X(row, row+p) = val;
  }
}

proc main() {
  const Space = {1..m, 1..m};

  var t: Timer;

  if !correctness {
    writeln('=========================================');
    writeln('  Dense Matrix Inverse Performance Test');
    writeln('=========================================');
    writeln('iters : ', iters);
    writeln('m     : ', m);
    writeln('numLocales     : ', numLocales);
    writeln('MB    : ', (bytes*m*m) / 10**6);
    writeln();
  }
  
  var I, A: [Space] eltType;
  
  addDiag(A, 0, 2);
  addDiag(A, 1, -1);
  addDiag(A, -1, -1);
  addDiag(A, 10, 0.5);
  addDiag(A, -10, 0.5);

  for 1..iters {
    t.start();
    I = inv(A)(1);
    t.stop();
  }

  if !correctness then
    writeln('LinearAlgebra.inv: ', t.elapsed() / iters);
  t.clear();
}
