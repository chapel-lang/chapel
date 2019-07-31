use Time;
use Math;
use LinearAlgebra;
use LinearAlgebra.Sparse;

config const m=1000,
             iters=10,
             /* Omit timing output */
             correctness=false;
             
config type eltType = real;

const bytes = numBytes(eltType);

// Identical to #13515, should be removed after #13515 is merged
proc addDiag (ref X: [?D] ?eltType, in p: int = 0, val: eltType = 0)
              where isSparseArr(X) { 
    if D.rank != 2 then
      halt("Wrong rank for addDiag");

    if D.shape(1) != D.shape(2) then
      halt("addDiag only supports square matrices");

    var start, end = 0;
    if (p >= 0) { // upper or main diagonal
      start = 1;
      end = D.shape(1) - p;
    }
    else { // lower diagonal
      start = 1 - p;
      end = D.shape(1);
    }
    var indices : [start..end] (D.idxType, D.idxType);
    forall ind in {start..end} {
      indices[ind] = (ind, ind+p);
    }
    D.bulkAdd(indices, dataSorted=true, isUnique=true, preserveInds=false);
    forall ind in indices {
      X(ind) = val;
    }
}

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
    writeln('MB    : ', (bytes*m*m) / 10**6);
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


