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
proc makeSymm(ref A : [?Adom]) {
  for (i,j) in Adom {
    if i < j then A[i,j] = A[j,i];
  }
}

// Make a hermitian matrix
// This does this explicitly, by making the lower triangular portion
// equal to the upper triangular
proc makeHerm(ref A : [?Adom]) {
  for (i,j) in Adom {
    if i < j then A[i,j] = conj(A[j,i]);
    if i==j then A[i,i] = A[i,i].re;
  }
}


// Make band-diagonal matrix
proc makeBand(ref A:[?Adom] ?t, kl, ku)
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
proc makeBandTriangular(ref A:[?Adom], k,  uplo:Uplo=Uplo.Upper)
  where Adom.rank == 2
{
  compilerError('Not yet implemented');
}


// Make a lower or upper triangular matrix
proc makeTri(ref A:[?Adom], uplo:Uplo = Uplo.Upper)
  where Adom.rank == 2 {
  if uplo == Uplo.Upper {
    zeroTri(A, zeroLow=true);
  } else {
    zeroTri(A, zeroLow=false);
  }
}

proc zeroTri(ref A:[?Adom], uplo:Uplo) where Adom.rank == 2 {
  const zeroLow = if uplo==Uplo.Upper then true else false;
  zeroTri(A, zeroLow);
}

// Zero out upper or lower triangular piece
proc zeroTri(ref A:[?Adom], zeroLow:bool=true) where Adom.rank == 2 {
  type t = A.eltType;
  const zero = 0 : t;
  forall (i,j) in Adom with (ref A) {
    if (i > j) & zeroLow then A[i,j] = zero;
    if (i < j) & !zeroLow then A[i,j] = zero;
  }
}

// Make identity matrix
proc makeUnit(ref A : [?Adom], val:real = 1.0) {
  type t = A.eltType;
  const zero = 0 : t;
  const diag = (val*1) : t;
  forall (i,j) in Adom with (ref A) {
    if (i!=j) then A[i,j] = zero;
              else A[i,i] = diag;
  }
}


// Band Array -- experimental
// TODO: finish this for banded pr
proc bandArray(A: [?Dom] ?eltType, ku, kl, param order: Order = Order.Row)
  where A.rank == 2 {
  const m = Dom.dim(0).size,
        n = Dom.dim(1).size,
        lda = kl+ku+1;

  // adapted from: https://www.intel.com/content/www/us/en/docs/onemkl/developer-reference-c/2024-1/cblas-gbmv.html
  // would be nice to allow `return select order { ... }`
  select order {
    when Order.Row {
      var AB: [1..m, 1..lda] eltType;
      for i in 1..m {
        var k = kl + 1 - i;
        for j in max(1, i-kl)..min(n, i+ku) do AB(i, k+j) = A(i, j);
      }
      return AB;
    }
    when Order.Col {
      var AB: [1..n, 1..lda] eltType;
      for j in 1..n {
        var k = ku + 1 - j;
        for i in max(1, j-ku)..min(m, j+kl) do AB(j, k+i) = A(i, j);
      }
      return AB;
    }
    otherwise do compilerError("no such order ", order);
  };
}


// Triangular band array - experimental
// TODO: finish this for banded pr
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
// TODO: finish this for banded pr
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
  B = conj(A);
  return B;
}

/* Pseudo-random nonsingular matrix ((1/n**2)*A**2 + I) */
proc makeRandomInvertible (ref A: [?Adom] ?t) {
  fillRandom(A);

  var I: [Adom] t;
  makeUnit(I);

  var A2: [Adom] t;
  forall (i,j) in A.domain with (ref A2) do A2[i,j] = + reduce (A[i,..]*conj(A[..,j]));

  const n = Adom.shape(1);
  const scale = (1/n**2) : t;
  A = scale*A2 + I;
}
