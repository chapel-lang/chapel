/* Matrix multiplication */

use LayoutCS;

config const correctness = false,
             m = 6,
             n = 7;

proc main() {
  const ADom = {1..m, 1..n},
        BDom = {1..n, 1..m};

  var csrDom: sparse subdomain(ADom) dmapped CS(),
      cscDom: sparse subdomain(BDom) dmapped CS(row=false);

  var csrArr: [csrDom] real,
      cscArr: [cscDom] real;

  var A: [ADom] real,
      B: [BDom] real;


  csrDom += ADom by 2;
  for (i,j) in ADom by 2 {
    A[i, j] = i;
    csrArr[i, j] = i;
  }

  cscDom += BDom by 3;
  for (i,j) in BDom by 3 {
    B[i, j] = j;
    cscArr[i, j] = j;
  }

  //
  // Dense Multiplication
  //

  if !correctness {
    writeln('A:');
    writeDense(A);
    writeln('B:');
    writeDense(B);
  }

  var AB = multiply(A, B);
  if !correctness {
    writeln('A x B:');
    writeDense(AB);
  }

  //
  // Sparse Multiplication
  //

  if !correctness {
    writeln('csrArr:');
    writeDense(csrArr);
    writeln();
    writeln('cscArr:');
    writeDense(cscArr);
    writeln();
  }

  var csrcscArr = multiply(csrArr, cscArr);
  if !correctness {
    writeln('csrArr x cscArr:');
    writeDense(csrcscArr);
  }

  // Validate csrcscArr nonzeros against AB
  for (i, j) in csrcscArr.domain {
    assert(AB[i,j] == csrcscArr[i, j]);
  }
}

/* Dense matrix-matrix multiplication */
proc multiply(A: [?ADom] ?eltType, B: [?BDom] eltType) {
  const CDom = {ADom.dim(1), BDom.dim(2)};
  var C: [CDom] eltType;
  [(i,j) in CDom] C[i,j] = + reduce(A[i,..] * B[.., j]);
  return C;
}

/* Sparse CSR-CSC multiplication */
proc multiply(A: [?ADom] ?eltType, B: [?BDom] eltType) where isSparseArr(A) && isSparseArr(B) {
  // TODO: Checks for ADom._value.row && BDom._value.row
  // TODO: Cleaner Implementation
  // TODO: Parallel Implementation

  var CDom: sparse subdomain({ADom._value.parentDom.dim(1), BDom._value.parentDom.dim(2)}) dmapped CS();
  var C: [CDom] eltType;

  // C index
  var i = 0;
  // Loop over rows in A
  for r in ADom._value.rowRange {
    for c in BDom._value.colRange {

      const cRange = idxRange(A, r);
      const rRange = idxRange(B, c);

      // Skip if either row/column is empty
      if cRange.isEmpty() || rRange.isEmpty() then continue;

      // Non-zero row pointer for B
      var nzr = rRange.first;

      var indexAdded = false;
      // Loop overs non-zeros within an A row
      for nzc in cRange {
        const idxA = idx(A, nzc),
              idxB = idx(B, nzr);
        if idxA == idxB {
          // Multiply!
          if !indexAdded {
            CDom += (r, c);
            indexAdded = true;
            i += 1;
          }
          C._value.data(i) += A._value.data(nzc) * B._value.data(nzr);
        }
        if nzr == rRange.last then break;
        while idx(B, nzr) < idxA && nzr < rRange.last do nzr += 1;
      }
    }
  }
  return C;
}

//
// Some helpers for accessing CSDom internals
//

inline proc idxRange(Arr: [?Dom], i) {
  return Dom._value.startIdx(i)..Dom._value.stopIdx(i);
}

inline proc idx(Arr: [?Dom], i) {
  return Dom._value.idx(i);
}


proc writeDense(A: [?D]) {
  for i in D.dim(1) {
    for j in D.dim(2) {
      writef('%3n ', A[i, j]);
    }
    writeln();
  }
}
