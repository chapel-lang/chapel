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

    if V.domain.dim(1) != M.domain.dim(1) then
        halt("Tensor1d:transform: C and A dims must match");

    for i in M.domain.dim(1) {
        for ip in M.domain.dim(2) do
            R[i] += V[ip] * M[ip, i];
    }

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
