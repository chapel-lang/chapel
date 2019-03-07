/*
 Generate a random sparse matrix and square it.
*/

use MatrixUtils;

use LinearAlgebra;
use LinearAlgebra.Sparse;
use Time;


config const n = 1000,
             nnz = 100,
             seed = 42,
             trials = 1,
             /* Omit non-timing output */
             performance = false,
             /* Omit timing output */
             correctness = false,
             /* Write matrices to files */
             write = false;

proc main() {
  const sparsity = (nnz:real) / (n**2);

  if !performance {
    writeln('CSR Matrix');
    writeln('n        : ', n);
    writeln('nnz      : ', nnz);
    writeln('sparsity : ', sparsity);
  }

  if !performance {
    writeln('Creating A');
  }

  const R = 1..n;
  const D = {R, R};
  var csrD = CSRDomain(D);
  var A = CSRMatrix(csrD);

  var failsA = memberCheck(A);
  if failsA != 0 || !performance {
    writeln('Membership failures: ', failsA);
  }

  populate(A, csrD, nnz, seed);

  if write {
    if !performance {
      writeln('Writing matrix A to "A.txt"');
    }
    writeMatrix('A.txt', A);
  }

  if !performance {
    writeln('Multiplying A*A');
  }

  var t: Timer;

  t.start();
  var AA = A.dot(A);
  t.stop();

  for 2..trials {
    t.start();
    A.dot(A);
    t.stop();
  }

  if !correctness {
    writeln('sparsity : ', nnz, '/', n*n, ' = ', sparsity);
    writeln('time (s) : ', t.elapsed() / trials);
  }

  if write {
    if !performance {
      writeln('Writing matrix A*A to "AA.txt"');
    }
    writeMatrix('AA.txt', AA);
  }

  /* Ensures indices are still sorted */
  if !performance {
    writeln("Performing membership test");
  }

  var fails = memberCheck(AA);
  if fails != 0 || !performance {
    writeln('Membership failures: ', fails);
  }
}

proc memberCheck(A) {
  var fails = 0;
  forall (i,j) in A.domain with (+ reduce fails) {
    if A.domain.contains((i,j)) == false {
      fails += 1;
      writeln("NOT FOUND: ", (i,j));
    }
  }
  return fails;
}

