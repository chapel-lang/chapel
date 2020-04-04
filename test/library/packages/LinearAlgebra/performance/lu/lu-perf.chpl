use Time;
use Math;
use LinearAlgebra;
use LinearAlgebra.Sparse;

config const m=4,
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

  setDiag(A, 0, 2);
  setDiag(A, 1, 0.5);
  setDiag(A, -2, -0.5);

  var LU: [A.domain] eltType;
  var ipiv: [A.domain.dim(0)] eltType;

  for 1..iters {
    t.start();
    (LU, ipiv) = lu(A);
    t.stop();
  }

  if !correctness then
    writeln('LinearAlgebra.lu: ', t.elapsed() / iters);
  else
    writeln(LU);

  

  t.clear();
}


