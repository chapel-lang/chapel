/* Helper functions used in BLAS tests */

config const errorThresholdDouble = 1.e-10;
config const errorThresholdSingle = 1.e-5;

proc printErrors(name: string, type t, passed, failed, tests) {
  writef("%s%s : %i PASSED, %i FAILED \n".format(blasPrefix(t), name, passed, failed));
}

proc trackErrors(name, err, errorThreshold, ref passed, ref failed, ref tests) {
  if err > errorThreshold {
    failed += 1;
    writef("%s : Failure on test %i : %r\n",name, tests, err);
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
