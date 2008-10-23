//
// Use standard modules for Random numbers and Timing routines
//
use Random, Time;

//
// Use common user module for computing vector and matrix norms
//
// TODO: Make this standard?
//
use Norm;

//
// Use the user module for computing HPCC problem sizes
//
use HPCCProblemSize;

//
// The number of matrices and the element type of those matrices
//
const numMatrices = 1;
type indexType = int,   // TODO: should be int(64)
     elemType = real;

//
// Configuration constants indicating the problem size (n) and the
// block size (blkSize)
//
config const n = computeProblemSize(numMatrices, elemType, rank=2, 
                                    memFraction=2): indexType, // TODO: remove cast when indexType == int(64)
             blkSize = 5;


//
// Configuration constant used for verification thresholds
//
config const epsilon = 2.0e-15;

//
// Configuration constants to indicate whether or not to use a
// pseudo-random seed (based on the clock) or a fixed seed; and to
// specify the fixed seed explicitly
//
config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.clockMS else 31415;

//
// Configuration constants to control what's printed -- benchmark
// parameters, input and output arrays, and/or statistics
//
config const printParams = true,
             printArrays = false,
             printStats = true;

//
// The program entry point
//
def main() {
  printConfiguration();

  const MatVectSpace: domain(2, indexType)   = [1..n, 1..n+1],
        MatrixSpace = MatVectSpace[.., ..n];
  
  var A   : [MatVectSpace] elemType,
      piv : [1..n] indexType,
      x   : [1..n] elemType;
   
  // construct an n by n+1 matrix filled with random values
  fillRandom(A, seed);
  A = A * 2.0 - 1.0;

  // start timed region
  const startTime = getCurrentTime();

  // perform LU decomposition
  LUFactorize(n, A, piv);

  // do back-substitution
  var b => A[.., n+1];
  x = backwardSub(n, A[MatrixSpace], b);

  // end timed region
  const execTime = getCurrentTime() - startTime;

  const  validAnswer = verifyResults(A, MatrixSpace, x, b);
  printResults(validAnswer, execTime);
}

// blocked LU factorization with pivoting for matrix augmented with vector of
// RHS values.
def LUFactorize(n : indexType, A : [1..n, 1..n+1] elemType, piv : [1..n] indexType) {
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
def schurComplement(A         : [1..n, 1..n+1] elemType,
                    ptOp       : indexType) {
  const AD = A.domain;

  // Calculate location of ptSol (see diagram above)
  const ptSol = ptOp+blkSize;

  // Copy data into replicated array so every processor has a local copy
  // of the data it will need to perform a local matrix-multiply.

  //var replAD : domain(2) distributed(
  //    Dimensional(BlkCyc(k), Replicated)) = AD[ptSol.., 1..#blkSize];
  const replAD : domain(2) = AD[ptSol.., ptOp..#blkSize];
  const replBD : domain(2) = AD[ptOp..#blkSize, ptSol..];
    
  const replA : [replAD] elemType = A[ptSol.., ptOp..#blkSize];
  const replB : [replBD] elemType = A[ptOp..#blkSize, ptSol..];

  // do local matrix-multiply on a block-by-block basis
  forall (row,col) in AD[ptSol.., ptSol..] by (blkSize, blkSize) {
    const aBlkD = replAD[row..#blkSize, ptOp..#blkSize];
    const bBlkD = replBD[ptOp..#blkSize, col..#blkSize];
    const cBlkD = AD[row..#blkSize, col..#blkSize];
    
    local {
      dgemm(
            aBlkD.dim(1).length,
            aBlkD.dim(2).length,
            bBlkD.dim(2).length,
            replA(aBlkD),
            replB(bBlkD),
            A(cBlkD));
    }
  }
}

// calculate C = C - A * B.
def dgemm(p : indexType,    // number of rows in A
          q : indexType,    // number of cols in A, number of rows in B
          r : indexType,    // number of cols in B
          A : [1..p, 1..q] ?t,
          B : [1..q, 1..r] t,
          C : [1..p, 1..r] t) {
  // Calculate (i,j) using a dot product of a row of A and a column of B.
  for i in 1..p {
    for j in 1..r {
      for k in 1..q {
        C[i,j] -= A[i, k] * B[k, j];
      }
    }
  }
}


// do unblocked-LU decomposition within the specified panel, update the
// pivot vector accordingly
def panelSolve(A : [] ?t,
               panel : domain(2, indexType),
               piv : [] indexType) {
  const pnlRows = panel.dim(1);
  const pnlCols = panel.dim(2);

  // Ideally some type of assertion to ensure panel is embedded in A's
  // domain
  assert(piv.domain.dim(1) == A.domain.dim(1));
  
  if(pnlCols.length == 0) then return;
  
  // iterate through the columns
  for k in pnlCols {
    var col = panel[k.., k..k];
    
    // If there are no rows below the current column return
    if col.dim(1).length == 0 then return;
    
    // The pivot is the element with the largest absolute value.  I need to
    // do this in two steps since if I assign it like (pivot, pivotRow) =
    // maxloc(...) pivot will be assigned to the absolute value, not the
    // actual value.
    const (_, pivotRow) =
      maxloc reduce(abs(A(col)), col.dim(1));
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
def updateBlockRow(A : [] ?t, tl : domain(2), tr : domain(2))
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

def backwardSub(n : int,
                A : [1..n, 1..n] elemType,
                b : [1..n] elemType) {
  var x : [b.domain] elemType;

  for i in [b.domain by -1] do {
    x[i] = b[i];
    
    for j in [i+1..b.domain.high] do {
      x[i] -= A[i,j] * x[j];
    }

    x[i] /= A[i,i];
  }

  return x;
}


def printConfiguration() {
  if (printParams) {
    printProblemSize(elemType, numMatrices, n, rank=2);
    writeln("block size = ", blkSize, "\n");
  }
}


// TODO: Should be able to replace A + MatrixSpace with passing in
// A[MatrixSpace], no?
//
def verifyResults(A, MatrixSpace, x, b) {
  // calculate and report verification residuals
  fillRandom(A, seed);
  A = A * 2.0 - 1.0;
  
  const axmbNorm = norm(gaxpyMinus(n, n, A[MatrixSpace], x, b), normType.normInf),

        a1norm   = norm(A[MatrixSpace], normType.norm1),
        aInfNorm = norm(A[MatrixSpace], normType.normInf),
        x1Norm   = norm(x, normType.norm1),
        xInfNorm = norm(x, normType.normInf);

  const resid1 = axmbNorm / (epsilon * a1norm * n),
        resid2 = axmbNorm / (epsilon * a1norm * x1Norm),
        resid3 = axmbNorm / (epsilon * aInfNorm * xInfNorm);

  if (printStats) {
    writeln("resid1: ", resid1);
    writeln("resid2: ", resid2);
    writeln("resid3: ", resid3);
  }

  return max(resid1, resid2, resid3) < 16.0;
}


def printResults(successful, execTime) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    writeln("Execution time = ", execTime);
    const GflopPerSec = ((2.0/3.0) * n**3 + (3.0/2.0) * n**2) / execTime * 10e-9;
    writeln("Performance (Gflop/s) = ", GflopPerSec);
  }
}

// matrix-vector multiplication, solve equation A*x-y
def gaxpyMinus(n : indexType,
               m : indexType,
               A : [1..n, 1..m],
               x : [1..m],
               y : [1..n]) {
  var res : [1..n] elemType;

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

