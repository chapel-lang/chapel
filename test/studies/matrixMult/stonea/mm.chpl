// This file demonstrates various serial matrix-matrix multiply algorithms in
// Chapel, using various techniques.
//
// Algorithms that do matrix multiply by indexing the elements of the matrices
// can be tricky to write when the LHS and RHS matrices are not indexed in the
// same manner. Most of these functions specify a domain in the formals for the
// LHS and RHS, so the actual arguments get reindexed when the function is
// called. A couple counter examples to this are the
// matrixMult_tensored_no_indices_no_reindexing and
// matrixMult_tensored_no_indices functions. The first which does no
// reindexing, the second which does the reindexing in the function itself.
//
// A good paper that discusses various mm algorithms is: Implementing Linear
// Algebra Algorithms for Dense Matrices on a Vector Pipeline Machine (by:
// Dongarra, Gustavson, and Karp)

use Random;

proc dotProd(n : int, A : [] int, B : [] int) : int {
    return + reduce(A * B);
}

proc saxpy(
    n : int,
    alpha : int,
    x : [] int,
    y : [] int)
    :
    [1..n] int
{
    return alpha * x + y;
}

proc matrixMult_ijk(
    const m : int,
    const p : int,
    const n : int,
    const A : [?AD] int,
    const B : [?BD] int,
    C : [?CD] int)
{
    for (ai,ci) in zip(AD.dim(0), CD.dim(0)) {
        for (bj,cj) in zip(BD.dim(1), CD.dim(1)) {
            C[ci,cj] = 0;
            for (ak,bk) in zip(AD.dim(1),BD.dim(0)) {
                C[ci,cj] += A[ai,ak] * B[bk,bj];
            }
        }
    }
}

proc matrixMult_ijk_clever(
    const m : int,
    const p : int,
    const n : int,
    const A : [?AD] int,
    const B : [?BD] int,
    C : [?CD] int)
{
    for (ai,ci) in zip(AD.dim(0), CD.dim(0)) {
        for (bj,cj) in zip(BD.dim(1), CD.dim(1)) {
            C[ci,cj] = dotProd(p, A[ai,..], B[..,bj]);
        }
    }
}

proc matrixMult_kij_clever(
    const m : int,
    const p : int,
    const n : int,
    const A : [?AD] int,
    const B : [?BD] int,
    C : [?CD] int)
{
    C = 0;

    for (ak,bk) in zip(AD.dim(1), BD.dim(0)) {
        for (ai,ci) in zip(AD.dim(0), CD.dim(0)) {
            C[ci,..] = saxpy(n, A[ai,ak], B[bk,..], C[ci,..]);
        }
    }
}

proc matrixMult_tensored(
    const m : int,
    const p : int,
    const n : int,
    const A : [1..m, 1..p] int,
    const B : [1..p, 1..n] int,
    C : [1..m, 1..n] int)
{
    C = 0;

    for (i,j,k) in {C.domain.dim(0), C.domain.dim(1), 1..p} {
        C[i,j] += A[i,k] * B[k,j];
    }
}


// what are the implications of doing it like this for sparse matrices?
proc matrixMult_tensored_no_indices_no_reindexing(
    const A : [] int,
    const B : [] int,
    C : [] int)
    where
        A.domain.rank == 2 && B.domain.rank == 2 && C.domain.rank == 2
{
    // asserting correct A, B, and C sizes would be complicated

    // potential error if one of the domains is degenerate
    const rowRange  = 0..(C.domain.dim(0).high - C.domain.dim(0).low);
    const colRange  = 0..(C.domain.dim(1).high - C.domain.dim(1).low);
    const calcRange = 0..(B.domain.dim(0).high - B.domain.dim(0).low);

    const keyIdxA = (A.domain.dim(0).low, A.domain.dim(1).low);
    const keyIdxB = (B.domain.dim(0).low, B.domain.dim(1).low);
    const keyIdxC = (C.domain.dim(0).low, C.domain.dim(1).low);

    C = 0;
 
    for (i,j,k) in {rowRange, colRange, calcRange} {
        C[keyIdxC + (i,j)] += A[keyIdxA + (i,k)] * B[keyIdxB + (k,j)];
    }
}

// what are the implications of doing it like this for sparse matrices?
proc matrixMult_tensored_no_indices(
    const A : [] int,
    const B : [] int,
    C : [] int)
    where
        A.domain.rank == 2 && B.domain.rank == 2 && C.domain.rank == 2
{
    // asserting correct A, B, and C sizes would be complicated

    // potential error if one of the domains is degenerate
    const rowRange  = 1..(C.domain.dim(0).high - C.domain.dim(0).low)+1;
    const colRange  = 1..(C.domain.dim(1).high - C.domain.dim(1).low)+1;
    const calcRange = 1..(B.domain.dim(0).high - B.domain.dim(0).low)+1;

    // reindexed versions of A, B, and C
    ref Aprime = A.reindex(rowRange,  calcRange);
    ref Bprime = B.reindex(calcRange, colRange);
    ref Cprime = C.reindex(rowRange,  colRange);

    C = 0;

    for (i,j,k) in
      {Cprime.domain.dim(0), Cprime.domain.dim(1), Bprime.domain.dim(0)}
    {
        Cprime[i,j] += Aprime[i,k] * Bprime[k,j];
    }
}






proc checkMult(msg : string, C, D : [] int) {
    write(msg);
    if & reduce(C == D) then
        writeln("PASSED");
    else
        writeln("FAILED");
}

proc main() {
    var A : [3..6, 8..10] int = 
        ((8, 9, 0),
         (7, 6, 1),
         (3, 2, 4),
         (6, 2, 1));
    var B : [24..26, 30..34] int =
        (( 1, 8,  4,  6, 15),
         (11, 2, 10,  3, 16),
         ( 5, 9,  7, 12, 18));
    var C : [93..96, 101..105] int;
    var D : [1..4, 1..5] int =
        ((107,  82, 122, 75, 264),
         ( 78,  77,  95, 72, 219),
         ( 45,  64,  60, 72, 149),
         ( 33,  61,  51, 54, 140));

    const m = A.domain.dim(0).high - A.domain.dim(0).low + 1;
    const p = A.domain.dim(1).high - A.domain.dim(1).low + 1;
    const n = B.domain.dim(1).high - B.domain.dim(1).low + 1;

    matrixMult_ijk(m, p, n, A, B, C);
    checkMult("mult ijk: ", C, D);
    C = 2;

    matrixMult_ijk_clever(m, p, n, A, B, C);
    checkMult("mult ijk (with dot prod): ", C, D);
    C = 2;

    matrixMult_kij_clever(m, p, n, A, B, C);
    checkMult("mult kij (with saxpy): ", C, D);
    C = 2;

    matrixMult_tensored(m, p, n, A.reindex(1..m, 1..p), B.reindex(1..p, 1..n), C.reindex(1..m, 1..n));
    checkMult("mult tensored: ", C, D);
    C = 2;

    matrixMult_tensored_no_indices(A, B, C);
    checkMult("mult tensored (no indices): ", C, D);
    C = 2;

    matrixMult_tensored_no_indices_no_reindexing(A, B, C);
    checkMult("mult tensored (no indices, no reindexing): ", C, D);
    C = 2;
}

