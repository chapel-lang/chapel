// Array copy routine
def tensorDup(A) {
    var B = A;
    return B;
}

// In place transpose for rank 2 matrices
def transpose(A: [] real) where A.rank == 2 {
    for (i, j) in A.domain do
        if i < j then A[i, j] <=> A[j, i];
    return A;
}

// Transform a vector by a matrix
def transform(in V: [] real, in M: [] real) where V.rank == 1 && M.rank == 2 {
    var R: [V.domain] real = 0.0;

    if V.domain.dim(1) != M.domain.dim(2) then
        halt("transform: Vector and matrix dims must match");

    for (i, j) in M.domain do
        R[i] += V[j] * M[j, i];

    return R;
}

// Vector-Matrix multiplication
def *(in V: [] real, in M: [] real) where V.rank == 1 && M.rank == 2 {
    var R: [M.domain.dim(2)] real = 0.0;

    if V.domain.dim(1) != M.domain.dim(1) then
        halt("*: Vector and matrix dims must match");

    for (i, j) in M.domain do
        R[i] += V[j] * M[i, j];

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
