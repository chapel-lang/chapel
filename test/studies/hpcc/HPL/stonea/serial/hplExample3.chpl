// Like hplExample2, except:
//  - Tested matrices are sized by configuration param n, and only the overall
//    LU decomposition test is included.
//  - Distributed style matrix-mult (from luLike2 testcase).
//  - The mannar in which A is partioned is different (I no longer do a two
//    step process of extracting ranges and defining regions with these).

use Random;
use Norm;

config const n = 100;
config const trials = 3;
config const blkSize = 5;
config const seed = 31415;
config const blksHoriz : int = ceil((n+1.0)/blkSize:real):int;
config const blksVert  : int = ceil(n:real/blkSize:real):int;


// calculate C = C - A * B.
proc dgemm(
    A : [?AD] ?t,
    B : [?BD] t,
    C : [?CD] t)
{
    // Calculate (i,j) using a dot product of a row of A and a column of B.
    for (ai,ci) in zip(AD.dim(1),CD.dim(1)) {
        for (bj,cj) in zip(BD.dim(2),CD.dim(2)) {
            for (ak,bk) in zip(AD.dim(2),BD.dim(1)) {
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
    // ABlkDmn is a superdomain of ADmn with dimensions such that they are
    // divisible by blocksize.
    const ADmn    = A.domain;
    const ABlkDmn = {1..blksVert*blkSize, 1..blksHoriz*blkSize};

    const ptOp  = (blk-1)*blkSize+1;
    const ptSol = blk*blkSize+1;

    var ACopies : [ABlkDmn[ptSol.., ptSol..]] real;
    var BCopies : [ABlkDmn[ptSol.., ptSol..]] real;

    // stamp A across into ACopies,
    forall blkCol in blk+1..blksHoriz {
        const cBlkD = {(ADmn.dim(1))(ptSol..),
                       (ABlkDmn.dim(2))((blkCol-1)*blkSize+1..#blkSize)};
        const aBlkD = {ptSol..#cBlkD.dim(1).size,
                       ptOp..#cBlkD.dim(2).size};

        ACopies[cBlkD] = A[aBlkD];
    }

    // stamp B down into BCopies,
    forall blkRow in blk+1..blksVert {
        const cBlkD = {(ABlkDmn.dim(1))((blkRow-1)*blkSize+1..#blkSize),
                       (ADmn.dim(2))(ptSol..)};
        const bBlkD = {ptOp..#cBlkD.dim(1).size,
                       ptSol..#cBlkD.dim(2).size};

        BCopies[cBlkD] = A[bBlkD];
    }

    // do local matrix-multiply on a block-by-block basis
    forall (blkRow,blkCol) in {blk+1..blksVert, blk+1..blksHoriz} {
        const aBlkD = {(ADmn.dim(1))((blkRow-1)*blkSize+1..#blkSize),
                       (ABlkDmn.dim(2))((blkCol-1)*blkSize+1..#blkSize)};
        const bBlkD = {(ABlkDmn.dim(1))((blkRow-1)*blkSize+1..#blkSize),
                       (ADmn.dim(2))((blkCol-1)*blkSize+1..#blkSize)};
        const cBlkD = ADmn[(blkRow-1)*blkSize+1..#aBlkD.dim(1).size,
                           (blkCol-1)*blkSize+1..#bBlkD.dim(2).size];

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
    var pnlRows = panel.dim(1);
    var pnlCols = panel.dim(2);

    // Ideally some type of assertion to ensure panel is embedded in A's
    // domain
    assert(piv.domain.dim(1) == A.domain.dim(1));

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
    var tlRows = tl.dim(1);
    var tlCols = tl.dim(2);
    var trRows = tr.dim(1);
    var trCols = tr.dim(2);

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
    const ARows = A.domain.dim(1);
    const ACols = A.domain.dim(2);

    piv = 1..n;

    /*
       The following diagram illustrates how we partition the matrix.  The
       program defines a number of ranges (specified in the legend) to span
       various portions of the array.

       We use these ranges to find subdomains of the array (labeled in the
       picture as tl, tr, bl, and br) there's an additional domain (not shown)
       l that is the union of tl and bl.

                (point blk, blk)
       +-------//------------------+ -----------   LEGEND:
       |......//...................|          |      1: blockRange
       |.....//....................|          |      2: trailingRows
       |....+-----+----------------| ------.  |      3: unfactoredRows
       |....|     |                |  |    |  |      4: trailingCols
       |....| tl  |      tr        |  1    |         5: unfactoredCols
       |....|     |                |  |    |  7      6: ARows
       |....+-----+----------------| ---             7: ACols
       |....|     |                |  |    3  |      
       |....|     |                |          |      
       |....| bl  |      br        |  2    |  |
       |....|     |                |       |  |
       |....|     |                |  |    |  |
       +----+-----+----------------+ ------------

       |    |    |                 |    
       |    |-1 -|------ 4 --------|
       |    |                      |
       |    `--------- 5 ----------|
       |                           |
       |------------ 6 ------------|
       |                           |
    */

    for blk in 1..n by blkSize {
        const crntBlkSize = min(blkSize, n-blk+1);

        const blockRange     = blk..blk+crntBlkSize-1;
        const trailingRows   = blk+crntBlkSize..ARows.high;
        const unfactoredRows = blk..ARows.high;
        const trailingCols   = blk+crntBlkSize..ACols.high;
        const unfactoredCols = blk..ACols.high;

        var tl = A.domain[blockRange, blockRange];
        var tr = A.domain[blockRange, trailingCols];
        var bl = A.domain[trailingRows, blockRange];
        var br = A.domain[trailingRows, trailingCols];
        var l  = A.domain[unfactoredRows, blockRange];
        
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
    var rand = new borrowed RandomStream(real, seed);
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

