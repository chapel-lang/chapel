/* Matrix multiplication */

use LayoutCS;
use Map;
use Time;

var subTimers = new map(string, Timer);
subTimers['multiply'] = new Timer();
subTimers['add indices'] = new Timer();
subTimers['find indices'] = new Timer();
subTimers[' overlap'] = new Timer();
subTimers['  push_back'] = new Timer();
subTimers['setup'] = new Timer();

config param subtimers = false;

config const correctness = false,
             m = 6,
             n = 7;


proc main() {
  const ADom = {1..m, 1..n},
        BDom = {1..n, 1..m};

  var csrDom: sparse subdomain(ADom) dmapped CS(),
      cscDom: sparse subdomain(BDom) dmapped CS(compressRows=false);

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


proc multiply(A, B) {
  return denseMultiply(A, B);
}

/* Dense matrix-matrix multiplication */
proc denseMultiply(A: [?ADom] ?eltType, B: [?BDom] eltType) {
  const CDom = {ADom.dim(0), BDom.dim(1)};
  var C: [CDom] eltType;
  forall (i, j) in CDom {
    for k in BDom.dim(0) {
      C[i,j] += A[i, k] * B[k, j];
    }
  }
  return C;
}

// TODO: Optimize & Parallelize
/* Sparse CSR-CSC multiplication */
proc multiply(A: [?ADom] ?eltType, B: [?BDom] eltType) where isSparseArr(A) && isSparseArr(B) {
  use List;

  if !(ADom._value.compressRows && !BDom._value.compressRows) then
    compilerError('Only CSR-CSC multiplication is currently supported');

  if subtimers then subTimers['setup'].start();
  var CDom: sparse subdomain({ADom._value.parentDom.dim(0), BDom._value.parentDom.dim(1)}) dmapped CS();
  var C: [CDom] eltType;

  ref idxA = A.domain._value.idx,
      idxB = B.domain._value.idx;

  /* Compute overlap between idxA & idxB over r1 and r2, respectively */
  iter overlap(const ref r1, const ref r2) {
    var ptr2 = r2.first;
    for ptr1 in r1 {
      if idxA(ptr1) == idxB(ptr2) then yield (ptr1, ptr2);
      while (idxB(ptr2) < idxA(ptr1) && ptr2 < r2.last) do ptr2 += 1;
    }
  }

  var indices: list(2*int);

  if subtimers then subTimers['setup'].stop();

  if subtimers then subTimers['find indices'].start();

  const ref Astart = ADom._value.startIdx,
            Bstart = BDom._value.startIdx;

  for r in ADom._value.rowRange {
    const ref a1 = Astart[r],
              a2 = Astart[r+1]-1;
    if a1 > a2 then continue;
    const aRange = a1..a2;
    for c in BDom._value.colRange {
      const ref b1 = Bstart[c],
                b2 = Bstart[c+1]-1;
      if b1 > b2 then continue;
      const bRange = b1..b2;

      if subtimers then subTimers[' overlap'].start();
      for (i, j) in overlap(aRange, bRange) {
        // Add to index!
        if subtimers then subTimers['  push_back'].start();
        indices.append((r, c));
        if subtimers then subTimers['  push_back'].stop();
        break;
      }
      if subtimers then subTimers[' overlap'].stop();
    }
  }
  if subtimers then subTimers['find indices'].stop();

  if subtimers then subTimers['add indices'].start();
  CDom += indices.toArray();
  if subtimers then subTimers['add indices'].stop();

  if subtimers then subTimers['multiply'].start();
  ref idxC = C.domain._value.idx;
  for r in CDom._value.rowRange {
    const cRange = idxRange(A, r);
    for c in idxRange(C, r) {
      const rRange = idxRange(B, idxC(c));
      var tmp: eltType;
      for (i, j) in overlap(cRange, rRange) {
        tmp += A._value.data(i) * B._value.data(j);
      }
      C._value.data(c) = tmp;
    }
  }
  if subtimers then subTimers['multiply'].stop();

  if subtimers {
    var s: real;
    for key in subTimers {
        write(key, ': ');
        var t = subTimers[key].elapsed();
        writeln(t);
      if !key[1].startsWith(' ') then s += t;
    }
    writeln('total: ', s);
  }

  return C;
}


//
// Some helpers for accessing CSDom internals
//

inline proc idxRange(Arr: [?Dom], i) {
  return Dom._value.startIdx(i)..Dom._value.stopIdx(i);
}


proc writeDense(A: [?D]) {
  for i in D.dim(0) {
    for j in D.dim(1) {
      writef('%3n ', A[i, j]);
    }
    writeln();
  }
}
