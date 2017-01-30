/* Helper functions used in BLAS tests */

config const errorThresholdDouble = 1.e-10;
config const errorThresholdSingle = 1.e-5;

proc printErrors(name: string, passed, failed, tests) {
  writef("%s : %i PASSED, %i FAILED\n".format(name, passed, failed));
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

// Zero out upper or lower triangular piece
proc zeroTri(A:[?Adom], zeroLow:bool=true) {
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

// No-ops when conjugating real numbers...
inline proc conjg(x : real(32)) {
  return x;
}

inline proc conjg(x : real(64)) {
  return x;
}


/* Convert array to band-storage, assumes 0-based index */
proc bandArray(A: [?Dom] ?eltType, l, u) where A.rank == 2 {

  const m = Dom.dim(1).size,
        n = Dom.dim(2).size;

  const k = min(m, n),
        rows = 1+l+u;

  var a: [{0..#rows, 0..#k}] eltType;

  for j in 1..k {
    for i in max(1, j-u)..min(n, j+l) {
      const idx = u+i-j;
      a[idx, j-1] = A[i-1, j-1];
    }
  }

  return a;
}


/* Naive transpose */
proc transpose(A: [?D] ?t) where A.rank == 2 {

  const m = D.dim(1).size,
        n = D.dim(2).size;

  var B: [{0..#n, 0..#m}] t;
  for (i, j) in D {
    B[j, i] = A[i, j];
  }
  return B;
}
