config param eps            = 1e-16;
config param truncateAtEps  = false;

// Truncate values to 0 if they are below machine precision or truncate
// values to the nearest 10 if they are large enough
proc truncate(x) {
    if truncateAtEps {
      if abs(x) <= eps then return 0.0;
      if abs(x) >= 1.0/eps then return trunc(x/10) * 10;
    }
    return x;
}

// Return matrix A's transpose
proc transpose(A: [] real) where A.rank == 2 {
    var B : [A.domain] real;

    forall (i, j) in A.domain do
        B[i, j] = A[j, i];

    return B;
}

// Copy matrix B's transpose into matrix A
proc transposeCopy(A: [] real, B: [] real) where A.rank == 2 && B.rank == 2 {
    forall (i, j) in A.domain do
        A[i, j] = B[j, i];
}

// Vector-Matrix transformation
proc *(V: [] real, M: [] real) where V.rank == 1 && M.rank == 2 {
    var R: [M.domain.dim(1)] real = 0.0;

    if V.domain.dim(0) != M.domain.dim(0) then
        halt("*: Vector and matrix dims must match");

    for (i, j) in M.domain do
        R[i] += V[j] * M[j, i];

    return R;
}

// Matrix-Vector multiplication 
proc *(M: [] real, V: [] real) where V.rank == 1 && M.rank == 2 {
    var R: [M.domain.dim(0)] real = 0.0;

    if V.domain.dim(0) != M.domain.dim(1) then
        halt("*: Vector and matrix dims must match");

    for (i, j) in M.domain do
        R[i] += V[j] * M[i, j];

    return R;
}

// Frobenius Norm -- 2-norm for Vectors
proc normf(A) where A.rank == 1 {
    return sqrt(+ reduce A**2);
}

// Inner product of A and B
proc inner(A, B) where A.rank == 1 && B.rank == 1 {
    return + reduce (A*B);
}
