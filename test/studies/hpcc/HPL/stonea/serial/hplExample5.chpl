// Like hplExample4, except cleaned up so it looks close to what the final
// submission might look like.
//
// - No wasted data (except that the replicated data buffer gets recreated in
//   the schur complement function every time its called)
// - Calculates flops/sec

use Random;
use Norm;
use Time;

config const n = 100;
config const blkSize = 5;
config const seed = 31415;
config const blksHoriz : int = ceil((n+1.0)/blkSize:real):int;
config const blksVert  : int = ceil(n:real/blkSize:real):int;
config const verbose: bool = true;
var timer = new Timer();

// calculate C = C - A * B.
proc dgemm(
    A : [?AD] ?t,
    B : [?BD] t,
    C : [?CD] t)
{
    // Calculate (i,j) using a dot product of a row of A and a column of B.
    for i in AD.dim(1) {
        for j in CD.dim(2) {
            for k in AD.dim(2) {
                C[i,j] -= A[i, k] * B[k, j];
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
// Every locale with a block of data in the dotted region updates that its by
// multiplying the neighboring a-region block to its left with the neighboring
// b-region block above it and subtracting its current data from the result of
// this multiplication. To ensure that all locales have local copies of the
// data needed to perform this multiplication we copy the data A and B data
// into the replA and replB arrays, which use a block-cyclic by block-cyclic
// replicated distribution to ensure that every locale only gets one copy of
// each block it requires.
proc schurComplement(
    A         : [1..n, 1..n+1] real,
    ptOp       : int)
{
    const AD = A.domain;

    // Calculate location of ptSol (see diagram above)
    const ptSol = ptOp+blkSize;

    // Copy data into replicated array so every processor has a local copy
    // of the data it will need to perform a local matrix-multiply.

    //var replAD : domain(2) dmapped(
    //    Dimensional(BlkCyc(k), Replicated)) = AD[ptSol.., 1..#blkSize];
    const replAD : domain(2) = AD[ptSol.., ptOp..#blkSize];
    const replBD : domain(2) = AD[ptOp..#blkSize, ptSol..];
    
    const replA : [replAD] real = A[ptSol.., ptOp..#blkSize];
    const replB : [replBD] real = A[ptOp..#blkSize, ptSol..];

    // do local matrix-multiply on a block-by-block basis
    forall (row,col) in AD[ptSol.., ptSol..] by (blkSize, blkSize)
    {
        const aBlkD = replAD[row..#blkSize, ptOp..#blkSize];
        const bBlkD = replBD[ptOp..#blkSize, col..#blkSize];
        const cBlkD = AD[row..#blkSize, col..#blkSize];

        local {
            dgemm(
                replA(aBlkD),
                replB(bBlkD),
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
    const pnlRows = panel.dim(1);
    const pnlCols = panel.dim(2);

    // Ideally some type of assertion to ensure panel is embedded in A's
    // domain
    assert(piv.domain.dim(1) == A.domain.dim(1));

    if(pnlCols.size == 0) then return;

    // iterate through the columns
    for k in pnlCols {
        var col = panel[k.., k..k];

        // If there are no rows below the current column return
        if col.dim(1).size == 0 then return;

        // The pivot is the element with the largest absolute value.
        const (_, loc) = maxloc reduce zip(abs(A(col)), col);
        const pivotRow = loc(1);
        const pivot = A[pivotRow, k];

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
    const tlRows = tl.dim(1);
    const tlCols = tl.dim(2);
    const trRows = tr.dim(1);
    const trCols = tr.dim(2);

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
        const tl = AD[blk..#blkSize, blk..#blkSize];
        const tr = AD[blk..#blkSize, blk+blkSize..];
        const bl = AD[blk+blkSize.., blk..#blkSize];
        const br = AD[blk+blkSize.., blk+blkSize..];
        const l  = AD[blk.., blk..#blkSize];

        // Now that we've sliced and diced A properly do the blocked-LU
        // computation:
        panelSolve(A, l, piv);
        if(tr.numIndices > 0) then
            updateBlockRow(A, tl, tr);

        // update trailing submatrix (if there)
        if(br.numIndices > 0) {
            schurComplement(A, blk);
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

proc main() {
    var A   : [1..n, 1..n+1] real;
    var piv : [1..n] int;
    var x   : [1..n] real;
   
    // construct an n by n+1 matrix filled with random values
    fillRandom(A, seed);
    A = A * 2.0 - 1.0;

    // start timed region
    timer.start();

    // perform LU decomposition
    LUFactorize(n, A, piv);

    // do back-substitution
    ref b = A[1..n, n+1];
    x = backwardSub(n, A[1..n, 1..n], b);

    // end timed region
    timer.stop();

    // calculate and report verification residuals
    fillRandom(A, seed);
    A = A * 2.0 - 1.0;

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

    if (verbose) {
      writeln("resid1: ", resid1);
      writeln("resid2: ", resid2);
      writeln("resid3: ", resid3);
    }
    writeln("verification result: ",
        if max(resid1, resid2, resid3) < 16.0 then "PASSED"
        else "FAILED");

    // calculate and report performance
    var performance = 
        ((2.0/3.0) * n**3 + (3.0/2.0) * n**2) / timer.elapsed() * 1e-9;
    writeln("computation time: ", timer.elapsed(), " seconds.");
    writeln("performance: ", performance, " Gflops/s.");
}

