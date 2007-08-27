// Constant empty vector
const None: [0..-1] real;

// Check if we have None
def isNone(x) {
    return x.numElements == 0;
}

// Copy matrix B's transpose into matrix A
def transposeCopy(A: [] real, B: [] real) where A.rank == 2 && B.rank == 2 {
    forall (i, j) in A.domain do
        A[i, j] = B[j, i];
}

// Vector-Matrix transformation
def *(in V: [] real, in M: [] real) where V.rank == 1 && M.rank == 2 {
    var R: [M.domain.dim(2)] real = 0.0;

    if V.domain.dim(1) != M.domain.dim(1) then
        halt("*: Vector and matrix dims must match");

    for (i, j) in M.domain do
        R[i] += V[j] * M[j, i];

    return R;
}

// Frobenius Norm -- 2-norm for Vectors
def normf(in A) where A.rank == 1 {
    return sqrt(+ reduce A**2);
}

// Inner product of A and B
def inner(in A, in B) where A.rank == B.rank {
    return + reduce A*B;
}
