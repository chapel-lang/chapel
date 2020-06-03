/* Helper functions used in BLAS tests */

use Random;
use BLAS;

config const errorThresholdDouble = 1.e-10;
config const errorThresholdSingle = 1.e-5;

proc printErrors(name: string, passed, failed, tests) {
  writef("%5s : %i PASSED, %i FAILED\n".format(name, passed, failed));
}

proc trackErrors(name, err, errorThreshold, ref passed, ref failed, ref tests) {
  if err > errorThreshold {
    failed += 1;
    writef("%s : Failure on test %i : %r\n", name, tests, err);
  } else {
    passed += 1;
  }
  tests+=1;
}

proc blasError(type t) {
  select t {
    when real(32) do return errorThresholdSingle;
    when real(64) do return errorThresholdDouble;
    when complex(64) do return errorThresholdSingle;
    when complex(128) do return errorThresholdDouble;
  }
}

proc blasPrefix(type t) {
  select t {
    when real(32) do return "s";
    when real(64) do return "d";
    when complex(64) do return "c";
    when complex(128) do return "z";
  }
}

// Make a symmetric matrix
// This does this explicitly, by making the lower triangular portion
// equal to the upper triangular
proc makeSymm(A : [?Adom]) {
  for (i,j) in Adom {
    if i < j then A[i,j] = A[j,i];
  }
}

// Make a hermitian matrix
// This does this explicitly, by making the lower triangular portion
// equal to the upper triangular
proc makeHerm(A : [?Adom]) {
  for (i,j) in Adom {
    if i < j then A[i,j] = conjg(A[j,i]);
    if i==j then A[i,i] = A[i,i].re;
  }
}


// Make band-diagonal matrix
proc makeBand(A:[?Adom] ?t, kl, ku)
  where Adom.rank == 2 {
  for i in Adom.dim(0) {
    for j in Adom.dim(1) {
      if (j < i - kl) || (j > i + ku) {
        A[i,j] = (0):t;
      }
    }
  }
}

/* TODO -- Make band-diagonal triangular matrix */
proc makeBandTriangular(A:[?Adom], k,  uplo:Uplo=Uplo.Upper)
  where Adom.rank == 2
{
  compilerError('Not yet implemented');
}


// Make a lower or upper triangular matrix
proc makeTri(A:[?Adom], uplo:Uplo = Uplo.Upper)
  where Adom.rank == 2 {
  if uplo == Uplo.Upper {
    zeroTri(A, zeroLow=true);
  } else {
    zeroTri(A, zeroLow=false);
  }
}

proc zeroTri(A:[?Adom], uplo:Uplo) where Adom.rank == 2 {
  const zeroLow = if uplo==Uplo.Upper then true else false;
  zeroTri(A, zeroLow);
}

// Zero out upper or lower triangular piece
proc zeroTri(A:[?Adom], zeroLow:bool=true) where Adom.rank == 2 {
  type t = A.eltType;
  const zero = 0 : t;
  forall (i,j) in Adom {
    if (i > j) & zeroLow then A[i,j] = zero;
    if (i < j) & !zeroLow then A[i,j] = zero;
  }
}

// Make identity matrix
proc makeUnit(A : [?Adom], val:real = 1.0) {
  type t = A.eltType;
  const zero = 0 : t;
  const diag = (val*1) : t;
  forall (i,j) in Adom {
    if (i!=j) then A[i,j] = zero;
              else A[i,i] = diag;
  }
}


// Band Array -- experimental
proc bandArray(A: [?Dom] ?eltType, ku, kl, order=Order.Row) where A.rank == 2 {

  const m = Dom.dim(0).size,
        n = Dom.dim(1).size;

  const bandDim = if order==Order.Row then m else n;

  var AB: [{0..#(kl+ku+1), 0..#n}] eltType;

  if order == Order.Row {
    // RowMajor
    for i in 0..#n{
      const k = kl - i;
      const jlo = max(0, i-kl),
            jhi = min(n, i+ku+1)-1;
      for j in jlo..jhi {
        var tmp = A[j, i];
        //AB[k+j, i] = A[j, i];
        AB[k+j, i] = tmp;
      }
    }
  } else {
    // ColMajor
    for j in 0..#n{
      const k = ku - j;
      const ilo = max(0, j-ku),
            ihi = min(m, j+kl+1)-1;
      for i in ilo..ihi {
        AB[k+i, j] = A[i, j];
      }
    }
  }

  return AB;
}


// Triangular band array - experimental
proc bandArrayTriangular(A: [?Dom] ?eltType, k, uplo=Uplo.Upper, order=Order.Row)
  where A.rank == 2 {

  // Must be square matrix
  assert(Dom.dim(0).size == Dom.dim(1).size);

  // Matrix order
  const n = Dom.dim(0).size;

  // double check this...
  var a: [{0..#(k+1), 0..#n}] eltType;

  var m = 0;
  var irange: range;

  if order == order.Col {
    for j in 0..#n {
      if uplo == Uplo.Upper {
        m = k - j;
        irange = max(0, j-k)..j;
      } else {
        m = -j;
        irange = j..min(n, j+k+1)-1;
      }
      for i in irange {
        a[m+i, j] = A[i, j];
      }
    }
  } else {
    halt('order.Row not yet supported');
  }

  return a;
}


// Dense array from band array -- experimental
proc bandArrayDense(a: [?Dom] ?eltType, l, u, m, n) where a.rank == 2 {

  const k = min(m, n),
        rows = 1+l+u;

  var A: [{0..#m, 0..#n}] eltType;

  for j in 1..k {
    for i in max(1, j-u)..min(n, j+l) {
      const idx = u+i-j;
      A[i-1, j-1] = a[idx, j-1];
    }
  }

  return A;
}

/* TODO -- Create packed array from dense array, assume 0-based domain*/
proc packedArray(A: [?Dom] ?elType) where A.rank == 2 {
  compilerError('Not yet implemented');
}


/* Naive transpose */
proc transpose(A: [?D] ?t) where A.rank == 2 {

  const m = D.dim(0).size,
        n = D.dim(1).size;

  var B: [{0..#n, 0..#m}] t;
  for (i, j) in D {
    B[j, i] = A[i, j];
  }
  return B;
}

/* Adjoint = transpose, then complex conjugate (Hermitian)*/
proc adjoint(A: [?D] ?t) where A.rank == 2 {
  var B = transpose(A);
  B = conjg(A);
  return B;
}

/* Pseudo-random nonsingular matrix ((1/n**2)*A**2 + I) */
proc makeRandomInvertible (A: [?Adom] ?t) {
  var rng = createRandomStream(eltType=t,algorithm=RNG.PCG);
  rng.fillRandom(A);

  var I: [Adom] t;
  makeUnit(I);

  var A2: [Adom] t;
  forall (i,j) in A.domain do A2[i,j] = + reduce (A[i,..]*conjg(A[..,j]));

  const n = Adom.shape(1);
  const scale = (1/n**2) : t;
  A = scale*A2 + I;
}
