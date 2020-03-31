/*
 Generate a random sparse matrix and square it.
*/

use MatrixUtils;

use LinearAlgebra;
use LinearAlgebra.Sparse;
private use List;
use Time;

config const n = 1000,
             nnz = 100,
             trials = 1,
             seed = 42,
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
  var AA = SPAdot(A, A);
  t.stop();

  for 2..trials {
    t.start();
    SPAdot(A, A);
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


/* CSR matrix-matrix multiplication

  Implementation derived from:

    Buluç, Aydın, J. R. Gilbert, and Viral B. Shah.
    "Implementing sparse matrices for graph algorithms."
    Graph Algorithms in the Language of Linear Algebra 22 (2011): 287.

 */

proc SPAdot(A: [?Adom], B: [?Bdom]) where isCSArr(A) && isCSArr(B) {

  const D = {Adom.dim(0), Bdom.dim(1)};
  var Cdom: sparse subdomain(D) dmapped CS(sortedIndices=false);
  var C: [Cdom] A.eltType;

  // pre-allocate nnz(A) + nnz(B) -- TODO: shrink later
  const nnzAB = Adom.size + Bdom.size;
  Cdom._value.nnzDom = {1..nnzAB};

  var spa = new _SPA(cols={D.dim(0)}, eltType=A.eltType);

  /*
   IR (row)     - nnz-rows  - A.domain._value.startIdx
   JC (column)  - nnz       - A.domain._value.idx
   VAL (values) - nnz       - A._value.data
  */

  for i in A.domain.dim(0) {
    const colRange = A.IR(i)..(A.IR(i+1)-1);
    for k in colRange {
      const jRange = B.IR(A.JC(k))..(B.IR(A.JC(k)+1)-1);
      for j in jRange {
        const value = A.NUM(k) * B.NUM(j),
              pos = B.JC(j);
        spa.scatter(value, pos);
      }
    }
    const nznew = spa.gather(C, i);
    C.IR[i+1] = C.IR[i] + nznew;
    spa.reset();
  }
  return C;

  /* Cleaner startIdx accessor */
  proc _array.IR ref return this._value.dom.startIdx;
  /* Cleaner idx accessor */
  proc _array.JC ref return this._value.dom.idx;
  /* Cleaner data accessor */
  proc _array.NUM ref return this._value.data;

}

pragma "no doc"
/* Sparse-accumulator */
record _SPA {
  var cols: domain(1);
  type eltType = int;
  var b: [cols] bool,      // occupation
      w: [cols] eltType,   // values
      ls: list(int);      // indices

  /* Reset w, b, and ls to empty */
  proc reset() {
    b = false;
    w = 0;
    ls.clear();
  }

  /* Accumulate nonzeros in SPA */
  proc scatter(const value, const pos) {
    if this.b[pos] == 0 {
      this.w[pos] = value;
      this.b[pos] = true;
      this.ls.append(pos);
    } else {
      this.w[pos] += value;
    }
  }

  proc gather(ref C: [?Cdom], i) {
    const nzcur = C.IR[i];
    var nzi = 0;
    this.ls.sort();

    for idx in this.ls {
      if nzcur + nzi  > C.JC.size then break;
      C.JC[nzcur+nzi] = idx;
      C.NUM[nzcur+nzi] = w[idx];
      Cdom._value._nnz += 1;
      nzi += 1;
    }
    return nzi;
  }
}
