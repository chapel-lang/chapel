/*
Sparse diagonal creation performance testing intended for manual usage
*/

use LinearAlgebra;
use LinearAlgebra.Sparse;
use Time;
use BlockDist;
use Math;

config const m=1000000,
             iters=10,
             /* Omit timing output */
             correctness=false;

config type eltType = real;

const nbytes = numBytes(eltType);

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
    writeln('MB    : ', (nbytes*m*m) / 10**6);
    writeln();
  }

  for i in 1..iters {
    var A: [SparseDom] real;
    t.start();
    setDiag(A, 0, 2);
    setDiag(A, 1, -1);
    setDiag(A, -1, -1);
    t.stop();
    
    if correctness && i == 1 {
      for (i,j) in Space {
        if SparseDom.contains(i,j) then
          write(A(i,j), " "); 
        else
          write(". ");

        if (j == m) then writeln();
      }
      writeln();
    }  
  }

  if !correctness then
    writeln('LinearAlgebra.addDiag: ', t.elapsed() / iters);
  t.clear();
}
