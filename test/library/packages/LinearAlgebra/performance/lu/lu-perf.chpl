use Time;
use Math;
use LinearAlgebra;
use LinearAlgebra.Sparse;

config const m=1000,
             iters=10,
             /* Omit timing output */
             correctness=false;
             
config type eltType = real;

const nbytes = numBytes(eltType);

proc main() {

  const Space = {1..m, 1..m};

  var t: Timer;

  if !correctness {
    writeln('=========================================');
    writeln('  LU Factorization Performance Test');
    writeln('=========================================');
    writeln('iters : ', iters);
    writeln('m     : ', m);
    writeln('numLocales     : ', numLocales);
    writeln('MB    : ', (nbytes*m*m) / 10**6);
    writeln();
  }

  var A: [Space] eltType;

  addDiag(A, 0, 2);
  addDiag(A, 1, -1);
  addDiag(A, -1, -1);
  addDiag(A, 10, 0.5);
  addDiag(A, -10, 0.5);

  for 1..iters {
    t.start();
    var (LU, ipiv) = lu(A);
    t.stop();
  }

  if !correctness then
    writeln('LinearAlgebra.lu: ', t.elapsed() / iters);
  t.clear();
}


