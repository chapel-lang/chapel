// Like hplExample3, except cleaned up a little, specifically:
//
// - The matrix in LUFactorize is no longer patitioned in a two step process of
//   defining ranges and explicitly defining a set of domains over these ranges.
//   Rather its partioned by slicing into the overall domain.
// - luLikeMultiply now defines two domains (one that's the same as the input
//   matrix's domain except rounded up to the next multiple of blocksize along
//   the horizontal axis, and one that does the same for the vertical.  With
//   these domains the function has been rewritten to cleverly use slicing.
//   Overall this should improve clarity and removes several large clumps of
//   ugly code.

use Random;
use LinearAlgebra;

config const n = 100;
config const seed = 3145;
config const trials = 3;
config const blkSize = 5;
config const blksHoriz : int = ceil((n+1.0)/blkSize:real):int;
config const blksVert  : int = ceil(n:real/blkSize:real):int;


// calculate C = C - A * B.
proc dgemm(
    A : [?AD] ?t,
    B : [?BD] t,
    C : [?CD] t)
{
    // Calculate (i,j) using a dot product of a row of A and a column of B.
    for (ai,ci) in zip(AD.dim(0),CD.dim(0)) {
        for (bj,cj) in zip(BD.dim(1),CD.dim(1)) {
            for (ak,bk) in zip(AD.dim(1),BD.dim(0)) {
                C[ci,cj] -= A[ai, ak] * B[bk, bj];
            }
        }
    }
}


// Distributed matrix-multiply for HPL. The idea behind this algorithm is that
// some point the matrix will be partioned as shown in the following diagram:
//
//    [1]----+-----+-----+-----+
//     |     |bbbbb|bbbbb|bbbbb|  Solve for the dotted region by
//     |     |bbbbb|bbbbb|bbbbb|  multiplying the 'a' and 'b' region.
//     |     |bbbbb|bbbbb|bbbbb|  The 'a' region is a block column, the
//     +----[2]----+-----+-----+  'b' region is a block row.
//     |aaaaa|.....|.....|.....|
//     |aaaaa|.....|.....|.....|  The vertex labeled [1] is location
//     |aaaaa|.....|.....|.....|  (ptOp, ptOp) in the code below.
//     +-----+-----+-----+-----+
//     |aaaaa|.....|.....|.....|  The vertex labeled [2] is location
//     |aaaaa|.....|.....|.....|  (ptSol, ptSol)
//     |aaaaa|.....|.....|.....|
//     +-----+-----+-----+-----+
//
// Assume there's a 1 to 1 mapping of blocks to locales. In this case for a
// locale in the dotted region to perform the necessary matrix multiply it will
// need a copy of the block in the 'a' region in the same block-row and a copy
// of the block in the 'b' region in the same block-column.
proc luLikeMultiply(
    A         : [1..n, 1..n+1] real,
    blk       : int)
{
    // HD is a superdomain of A's domain except the horizontal range is rounded
    // up to be divisible with blkSize.  VD is similiar except it expands along
    // the vertical axis.
    const AD = A.domain;
    const HD = {A.domain.dim(0), 1..blksHoriz*blkSize};
    const VD = {1..blksVert*blkSize, A.domain.dim(1)};

    // Define the operand and solution points (see diagram above)
    const ptOp  = (blk-1)*blkSize+1;
    const ptSol = blk*blkSize+1;

    var ACopies : [HD[ptSol.., ptSol..]] real;
    var BCopies : [VD[ptSol.., ptSol..]] real;

    // aBlkD is the domain for the 'a' region in te diagram, aBldD is for the
    // 'b'.
    const aBlkD = AD[ptSol.., ptOp..#blkSize];
    const bBlkD = AD[ptOp..#blkSize, ptSol..];

    // stamp A across into ACopies. col is an index
    forall col in AD.dim(1)(ptSol..) by blkSize {
        const cBlkD = HD[ptSol.., col..#blkSize];

        ACopies[cBlkD] = A[aBlkD];
    }

    // stamp B down into BCopies,
    forall row in AD.dim(0)(ptSol..) by blkSize {
        const cBlkD = VD[row..#blkSize, ptSol..];

        BCopies[cBlkD] = A[bBlkD];
    }

    // do local matrix-multiply on a block-by-block basis
    forall (row,col) in AD[ptSol.., ptSol..] by (blkSize, blkSize)
    {
        const aBlkD = HD[row..#blkSize, col..#blkSize];
        const bBlkD = VD[row..#blkSize, col..#blkSize];
        const cBlkD = AD[row..#blkSize, col..#blkSize];

        local {
            dgemm(
                ACopies(aBlkD),
                BCopies(bBlkD),
                A(cBlkD));
        }
    }
}



// do unblocked-LU decomposition within the specified panel, update the
// pivot vector accordingly
proc panelSolve(
    A : [] ?t,
    panel : domain(2),
    piv : [] int)
{
    var pnlRows = panel.dim(0);
    var pnlCols = panel.dim(1);

    // Ideally some type of assertion to ensure panel is embedded in A's
    // domain
    assert(piv.domain.dim(0) == A.domain.dim(0));

    // iterate through the columns
    for k in pnlCols {
        var col = panel[k.., k..k];

        // The pivot is the element with the largest absolute value.
        var (_, loc) =
            maxloc reduce zip(abs(A(col)), col);
        var pivotRow = loc(1);
        var pivot = A[pivotRow, k];

        // Swap the current row with the pivot row
        piv[k] <=> piv[pivotRow];
        A[k, ..] <=> A[pivotRow, ..];
        
        if(pivot == 0) then {
            halt("Matrix can not be factorized");
        }
        
        // divide all values below and in the same col as the pivot by
        // the pivot
        if k+1 <= pnlRows.high {
            A(col)[k+1.., k..k] /= pivot;
        }

        // update all other values below the pivot
        if k+1 <= pnlRows.high && k+1 <= pnlCols.high {
            forall (i,j) in panel[k+1.., k+1..] {
                A[i,j] -= A[i,k] * A[k,j];
            }
        }
    }
}

// Update the block row (tr for top-right) portion of the matrix in a blocked
// LU decomposition.  Each step of the LU decomposition will solve a block
// (tl for top-left) portion of a matrix. This function solves the rows to the
// right of the block.
proc updateBlockRow(A : [] ?t, tl : domain(2), tr : domain(2))
{
    var tlRows = tl.dim(0);
    var tlCols = tl.dim(1);
    var trRows = tr.dim(0);
    var trCols = tr.dim(1);

    assert(tlCols == trRows);

    for i in trRows {
        forall j in trCols {
            for k in tlRows.low..i-1 {
                A[i, j] -= A[i, k] * A[k,j];
            }
        }
    }
}

// blocked LU factorization with pivoting for matrix augmented with vector of
// RHS values.
proc LUFactorize(n : int, A : [1..n, 1..n+1] real, piv : [1..n] int) {
    const AD = A.domain;    // alias A.domain to save typing

    // Initialize the pivot vector to represent the initially unpivoted matrix.
    piv = 1..n;

    /*
       The following diagram illustrates how we partition the matrix.  Each
       iteration the loop increments a variable blk, point (blk, blk) is the
       upper-left location of the currently unfactored matrix (the dotted
       region represents the areas factored in prior iterations).  The
       unfactored matrix is partioned into four domains: tl, bl, tr, and br,
       and an additional domain (not shown) l that is the union of tl and bl.

                (point blk, blk)
       +-------//------------------+
       |......//...................|
       |.....//....................|
       |....+-----+----------------|
       |....|     |                |
       |....| tl  |      tr        |
       |....|     |                |
       |....+-----+----------------|
       |....|     |                |
       |....|     |                |
       |....| bl  |      br        |
       |....|     |                |
       |....|     |                |
       +----+-----+----------------+
    */
    for blk in 1..n by blkSize {
        var tl = AD[blk..#blkSize, blk..#blkSize];
        var tr = AD[blk..#blkSize, blk+blkSize..];
        var bl = AD[blk+blkSize.., blk..#blkSize];
        var br = AD[blk+blkSize.., blk+blkSize..];
        var l  = AD[blk.., blk..#blkSize];

        // Now that we've sliced and diced A properly do the blocked-LU
        // computation:
        panelSolve(A, l, piv);
        updateBlockRow(A, tl, tr);

        // update trailing submatrix (if there)
        if(br.size > 0) {
            var blkID : int = (blk/blkSize):int + 1;
            luLikeMultiply(A, blkID);
         }
    }
}

// -------------------------------------------------------------------------- 
//   TESTING SYSTEM:
// -------------------------------------------------------------------------- 
// matrix-vector multiplication, solve equation A*x-y
proc gaxpyMinus(
    n : int,
    m : int,
    A : [1..n, 1..m],
    x : [1..m],
    y : [1..n])
{
    var res : [1..n] real;

    for i in 1..n {
        for j in 1..m {
            res[i] += A[i,j]*x[j];
        }
    }

    for i in 1..n {
        res[i] -= y[i];
    }

    return res;
}

proc backwardSub(
    n : int,
    A : [1..n, 1..n] real,
    b : [1..n] real)
{
    var x : [b.domain] real;

    for i in b.domain by -1 do {
        x[i] = b[i];

        for j in i+1..b.domain.high do {
            x[i] -= A[i,j] * x[j];
        }

        x[i] /= A[i,i];
    }

    return x;
}

proc test_LUFactorizeNorms(
    n    : int,
    A    : [1..n, 1..n] real,
    b    : [1..n] real,
    AHat : [1..n, 1..n] real,
    bHat : [1..n] real,
    piv  : [1..n] int) : bool
{
    var x : [1..n] real;

    // use backward substitution to solve the system of equations.
    x = backwardSub(n, AHat, bHat);

    // calculate residuals (equations at bottom of this page
    //     http://www.netlib.org/benchmark/hpl/algorithm.html):

    var axmbNorm =
        norm(gaxpyMinus(n, n, A({1..n, 1..n}), x, b), normType.normInf);

    var a1norm   = norm(A({1..n, 1..n}), normType.norm1);
    var aInfNorm = norm(A({1..n, 1..n}), normType.normInf);
    var x1Norm   = norm(x, normType.norm1);
    var xInfNorm = norm(x, normType.normInf);

    const eps = 2.0e-15;

    var resid1 = axmbNorm / (eps * a1norm * n);
    var resid2 = axmbNorm / (eps * a1norm * x1Norm);
    var resid3 = axmbNorm / (eps * aInfNorm * xInfNorm);

    return max(resid1, resid2, resid3) < 16.0;
}

proc test_LUFactorize(rprt = true) : bool {
    // construct a 100x100 matrix filled with random values
    var rand = new owned RandomStream(real, seed);
    var A : [1..n, 1..n+1] real;
    for idx in A.domain do A[idx] = rand.getNext();

    // save a copy
    var origA = A;

    var piv : [1..n] int;

    LUFactorize(n, A, piv);
    
    // test using norms

    var arg_A    : [1..n, 1..n] real;
    var arg_b    : [1..n] real;
    var arg_AHat : [1..n, 1..n] real;
    var arg_bHat : [1..n] real;
    var arg_piv  : [1..n] int;

    arg_A = origA[1..n, 1..n];
    arg_b = origA[1..n, n+1];
    arg_AHat = A[1..n, 1..n];
    arg_bHat = A[1..n, n+1];
    arg_piv = piv;

    if test_LUFactorizeNorms(
        n, arg_A, arg_b, arg_AHat, arg_bHat, arg_piv)
    {
        if rprt then writeln("PASSED");
        return true;
    } else {
        if rprt then writeln("FAILED");
        return false;
    }
}

proc main() {
    var numPassed = 0;
    write("test_LUFactorize: ");
    for i in 1..trials do
        numPassed += test_LUFactorize();
    writeln(numPassed, " PASSED, ", trials-numPassed, " FAILED");
}

