/* Sparse matrix-matrix multiplication performance benchmark

   Comparisons we'd like to make include:

   - Chapel:
     - [x] CSR * CSC - optimized
     - [x] CSR * cSC - naive
   - Python:
     - [ ] scipy.sparse.csr . scipy.sparse.csc (checked in)
   - MKL:
     - [ ] mkl_dcsrmultcsr()

*/
use LayoutCS;
use Random;
use Time;

// For configs and matrix multiplication routine
use correctness;

// TODO: Option to read in array
config const sparsity = 0.01,
             seed = 42;

proc main() {

  const D1 = {1..m, 1..n},
        D2 = {1..n, 1..m};

  // Domains
  var ADom: sparse subdomain(D1) dmapped new dmap(new CS(compressRows=true)),
      BDom: sparse subdomain(D2) dmapped new dmap(new CS(compressRows=false));

  // Arrays
  var A: [ADom] real,
      B: [BDom] real;

  // Populate
  populate(A, ADom, sparsity, seed);
  populate(B, BDom, sparsity, seed+1);

  var t: stopwatch;

  // CSR . CSC - optimized
  t.start();
  var result = multiply(A, B);
  t.stop();
  if !correctness {
    write('CSR . CSC - optimized: ');
    writeln(t.elapsed());
  }
}


/* Populate sparse matrix with random elements according to "sparsity" */
proc populate(ref A, ref ADom, sparsity: real, seed: int) where A.isSparse() {
  const nnz = (ADom._value.parentDom.size * sparsity): int;
  var indices: [1..nnz] 2*int;
  var randomIndices = new randomStream(eltType=int, seed=seed);
  for idx in indices {
    // Ensure no duplicates
    var newIdx = idx, loc: nnz.type;
    while indices.find(newIdx, loc) {
      newIdx = (randomIndices.next(ADom.dim(0).low, ADom.dim(0).high), randomIndices.next(ADom.dim(1).low, ADom.dim(1).high));
    }
    idx = newIdx;
  }

  ADom += indices;

  var randomReals = new randomStream(eltType=real, seed=seed);
  for idx in ADom {
    A[idx] = randomReals.next();
  }
}

/* Populate dense matrix with random elements according to "sparsity" */
proc populate(ref A: [?ADom], sparsity: real, seed: int) where !A.isSparse() {
  const nnz= (ADom.size * sparsity): int;
  var indices: [1..nnz] 2*int;
  var randomIndices = new randomStream(eltType=int, seed=seed);
  for idx in indices {
    // Ensure no duplicates
    var newIdx = idx, loc: nnz.type;
    while indices.find(newIdx, loc) {
      newIdx = (randomIndices.next(ADom.dim(0).low, ADom.dim(0).high), randomIndices.next(ADom.dim(1).low, ADom.dim(1).high));
    }
    idx = newIdx;
  }

  var randomReals = new randomStream(eltType=real, seed=seed);
  for idx in indices {
    A[idx] = randomReals.next();
  }
}
