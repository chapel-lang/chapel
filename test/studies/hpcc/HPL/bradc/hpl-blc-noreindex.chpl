//
// Use standard modules for vector and matrix Norms, Random numbers
// and Timing routines
//
use Norm, Random, Time;

//
// Use the user module for computing HPCC problem sizes
//
use HPCCProblemSize;

//
// Use the distributions we need for this computation
//
use BlockCycDist;

//
// The number of matrices and the element type of those matrices
// indexType can be int or int(64), elemType can be real or complex
//
const numMatrices = 1;
type indexType = int,
     elemType = real;

//
// Configuration constants indicating the problem size (n) and the
// block size (blkSize)
//
config const n = computeProblemSize(numMatrices, elemType, rank=2, 
                                    memFraction=2, retType=indexType),
             blkSize = 8;

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
             seed = if useRandomSeed then SeedGenerator.oddCurrentTime else 31415;

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
proc main() {
  printConfiguration();

  //
  // MatVectSpace is a 2D domain of type indexType that represents the
  // n x n matrix adjacent to the column vector b.  MatrixSpace is a
  // subdomain that is created by slicing into MatVectSpace,
  // inheriting all of its rows and its low column bound.  As our
  // standard distribution library is filled out, MatVectSpace will be
  // distributed using a BlockCyclic(blkSize) distribution.
  //
  const MatVectSpace: domain(2, indexType) 
                      dmapped BlockCyclic(startIdx=(1,1), (blkSize,blkSize)) 
                    = {1..n, 1..n+1},
        MatrixSpace = MatVectSpace[.., ..n];

  var Ab : [MatVectSpace] elemType,  // the matrix A and vector b
      piv: [1..n] indexType;         // a vector of pivot values

  initAB(Ab);

  const startTime = getCurrentTime();     // capture the start time

  LUFactorize(n, Ab, piv);                 // compute the LU factorization

  var x = backwardSub(n, Ab);  // perform the back substitution

  const execTime = getCurrentTime() - startTime;  // store the elapsed time

  //
  // Validate the answer and print the results
  const validAnswer = verifyResults(Ab, MatrixSpace, x);
  printResults(validAnswer, execTime);
}

//
// blocked LU factorization with pivoting for matrix augmented with
// vector of RHS values.
//
proc LUFactorize(n: indexType, Ab: [?AbD] elemType,
                piv: [1..n] indexType) {
  
  // Initialize the pivot vector to represent the initially unpivoted matrix.
  piv = 1..n;

  /* The following diagram illustrates how we partition the matrix.
     Each iteration of the loop increments a variable blk by blkSize;
     point (blk, blk) is the upper-left location of the currently
     unfactored matrix (the dotted region represents the areas
     factored in prior iterations).  The unfactored matrix is
     partioned into four subdomains: tl, tr, bl, and br, and an
     additional domain (not shown), l, that is the union of tl and bl.

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
    const tl = AbD[blk..#blkSize, blk..#blkSize],
          tr = AbD[blk..#blkSize, blk+blkSize..],
          bl = AbD[blk+blkSize.., blk..#blkSize],
          br = AbD[blk+blkSize.., blk+blkSize..],
          l  = AbD[blk.., blk..#blkSize];

    //
    // Now that we've sliced and diced Ab properly, do the blocked-LU
    // computation:
    //
    panelSolve(Ab, l, piv);
    updateBlockRow(Ab, tl, tr);
    
    //
    // update trailing submatrix (if any)
    //
    schurComplement(Ab, bl, tr, br);
  }
}

//
// Distributed matrix-multiply for HPL. The idea behind this algorithm is that
// some point the matrix will be partioned as shown in the following diagram:
//
//    [1]----+-----+-----+-----+
//     |     |bbbbb|bbbbb|bbbbb|  Solve for the dotted region by
//     |     |bbbbb|bbbbb|bbbbb|  multiplying the 'a' and 'b' region.
//     |     |bbbbb|bbbbb|bbbbb|  The 'a' region is a block column, the
//     +----[2]----+-----+-----+  'b' region is a block row.
//     |aaaaa|.....|.....|.....|
//     |aaaaa|.....|.....|.....|  The 'a' region was 'bl' in the calling
//     |aaaaa|.....|.....|.....|  function but called AD here.  Similarly,
//     +-----+-----+-----+-----+  'b' was 'tr' in the calling code, but BD
//     |aaaaa|.....|.....|.....|  here.
//     |aaaaa|.....|.....|.....|  
//     |aaaaa|.....|.....|.....|
//     +-----+-----+-----+-----+
//
// Every locale with a block of data in the dotted region updates
// itself by multiplying the neighboring a-region block to its left
// with the neighboring b-region block above it and subtracting its
// current data from the result of this multiplication. To ensure that
// all locales have local copies of the data needed to perform this
// multiplication we copy the data A and B data into the replA and
// replB arrays, which will use a dimensional (block-cyclic,
// replicated-block) distribution (or vice-versa) to ensure that every
// locale only stores one copy of each block it requires for all of
// its rows/columns.
//
proc schurComplement(Ab: [?AbD] elemType, AD: domain, BD: domain, Rest: domain) {
  //
  // Copy data into replicated array so every processor has a local copy
  // of the data it will need to perform a local matrix-multiply.  These
  // replicated distributions aren't implemented yet, but imagine that
  // they look something like the following:
  //
  //var replAbD: domain(2) 
  //            dmapped new Dimensional(BlkCyc(blkSize), Replicated)) = AbD[AD];
  //
  var replAD: domain(2, indexType),
      replBD: domain(2, indexType);
  replAD = AD;
  replBD = BD;
    
  const replA : [replAD] elemType = Ab[replAD],
        replB : [replBD] elemType = Ab[replBD];

  //  writeln("Rest = ", Rest);
  //  writeln("Rest by blkSize = ", Rest by (blkSize, blkSize));
  // do local matrix-multiply on a block-by-block basis
  // TODO: re-enable parallel loop
  forall (row,col) in Rest by (blkSize, blkSize) {
    //for (row,col) in Rest by (blkSize, blkSize) {
    //
    // At this point, the dgemms should all be local, so assert that
    // fact
    //
    //    writeln("On ", here.id, ", trying to access ", 
    //            (row..#blkSize, col..#blkSize));

    // TODO: enable this on distributed memory
    //    local {
      const aBlkD = replAD[row..#blkSize, ..],
            bBlkD = replBD[.., col..#blkSize],
            cBlkD = AbD[row..#blkSize, col..#blkSize];

      dgemmNativeInds(replA[aBlkD], replB[bBlkD], Ab[cBlkD]);
      /*
      dgemmReindexed(cBlkD.dim(0).size, aBlkD.dim(1).size, cBlkD.dim(1).size,
                     replA[aBlkD], replB[bBlkD], Ab[cBlkD]);
      */
      /*
      dgemmIdeal(replA[aBlkD], replB[bBlkD], Ab[cBlkD]);
      */
      //    }
  }
}

//
// calculate C = C - A * B.
//
proc dgemmNativeInds(A: [] elemType,
                     B: [] elemType,
                     C: [] elemType) {
  for (iA, iC) in zip(A.domain.dim(0), C.domain.dim(0)) do
    for (jA, iB) in zip(A.domain.dim(1), B.domain.dim(0)) do
      for (jB, jC) in zip(B.domain.dim(1), C.domain.dim(1)) do
        C[iC,jC] -= A[iA, jA] * B[iB, jB];
}

proc dgemmReindexed(p: indexType,    // number of rows in A
                   q: indexType,    // number of cols in A, number of rows in B
                   r: indexType,    // number of cols in B
                   A: [1..p, 1..q] elemType,
                   B: [1..q, 1..r] elemType,
                   C: [1..p, 1..r] elemType) {
  // Calculate (i,j) using a dot product of a row of A and a column of B.
  for i in 1..p do
    for j in 1..r do
      for k in 1..q do
        C[i,j] -= A[i, k] * B[k, j];
}

proc dgemmIdeal(A: [1.., 1..] elemType,
               B: [1.., 1..] elemType,
               C: [1.., 1..] elemType) {
  for i in C.domain.dim(0) do
    for j in C.domain.dim(1) do
      for k in A.domain.dim(1) do
        C[i,j] -= A[i, k] * B[k, j];
}


//
// do unblocked-LU decomposition within the specified panel, update the
// pivot vector accordingly
//
proc panelSolve(Ab: [] elemType,
               panel: domain,
               piv: [] indexType) {

  //
  // TODO: Use on clause here (or avoid using a range?
  //
  for k in panel.dim(1) {             // iterate through the columns
    var col = panel[k.., k..k];
    
    // If there are no rows below the current column return
    if col.size == 0 then return;
    
    // Find the pivot, the element with the largest absolute value.
    const (_, (pivotRow, _)) = maxloc reduce zip(abs(Ab(col)), col);

    // Capture the pivot value explicitly (note that result of maxloc
    // is absolute value, so it can't be used directly).
    //
    // TODO: could introduce a maxabsloc reduction that returns the
    //       raw value with the highest absolute value
    const pivotVal = Ab[pivotRow, k];
    
    // Swap the current row with the pivot row and update the pivot vector
    // to reflect that
    Ab[k..k, ..] <=> Ab[pivotRow..pivotRow, ..];
    piv[k] <=> piv[pivotRow];

    if (pivotVal == 0) then
      halt("Matrix cannot be factorized");
    
    // divide all values below and in the same col as the pivot by
    // the pivot value
    Ab[k+1.., k..k] /= pivotVal;
    
    // update all other values below the pivot
    forall (i,j) in panel[k+1.., k+1..] do
      Ab[i,j] -= Ab[i,k] * Ab[k,j];
  }
}

//
// Update the block row (tr for top-right) portion of the matrix in a
// blocked LU decomposition.  Each step of the LU decomposition will
// solve a block (tl for top-left) portion of a matrix. This function
// solves the rows to the right of the block.
//
proc updateBlockRow(Ab: [] elemType,
                   tl: domain,
                   tr: domain) {

  for row in tr.dim(0) {
    const activeRow = tr[row..row, ..],
          prevRows = tr.dim(0).low..row-1;

    forall (i,j) in activeRow do
      for k in prevRows do
        Ab[i, j] -= Ab[i, k] * Ab[k,j];
  }
}


//
// compute the backwards substitution
//
proc backwardSub(n: indexType,
                 Ab: [] elemType) {
  const bd = Ab.domain.dim(0);
  var x: [bd] elemType;

  // TODO: Really want a partial reduction here
  for i in bd by -1 do
    x[i] = (Ab[i,n+1] - (+ reduce [j in i+1..bd.high] (Ab[i,j] * x[j]))) 
            / Ab[i,i];

  return x;
}

//
// print out the problem size and block size if requested
//
proc printConfiguration() {
  if (printParams) {
    if (printStats) then printLocalesTasks();
    printProblemSize(elemType, numMatrices, n, rank=2);
    writeln("block size = ", blkSize, "\n");
  }
}

//   
// construct an n by n+1 matrix filled with random values and scale
// it to be in the range -1.0..1.0
//
proc initAB(Ab: [] elemType) {
  fillRandom(Ab, seed);
  Ab = Ab * 2.0 - 1.0;
}

//
// calculate norms and residuals to verify the results
//
proc verifyResults(Ab, MatrixSpace, x) {
  initAB(Ab);

  const axmbNorm = norm(gaxpyMinus(Ab[.., 1..n], x, Ab[.., n+1..n+1]), normType.normInf);

  const a1norm   = norm(Ab[.., 1..n], normType.norm1),
        aInfNorm = norm(Ab[.., 1..n], normType.normInf),
        x1Norm   = norm(Ab[.., n+1..n+1], normType.norm1),
        xInfNorm = norm(Ab[.., n+1..n+1], normType.normInf);

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

//
// print success/failure, the execution time and the Gflop/s value
//
proc printResults(successful, execTime) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    writeln("Execution time = ", execTime);
    const GflopPerSec = ((2.0/3.0) * n**3 + (3.0/2.0) * n**2) / execTime * 1e-9;
    writeln("Performance (Gflop/s) = ", GflopPerSec);
  }
}

//
// simple matrix-vector multiplication, solve equation A*x-y
//
proc gaxpyMinus(A: [],
                x: [?xD],
                y: [?yD]) {
  var res: [1..n] elemType;

  // TODO: really want a partial reduction here
  forall i in 1..n do
    res[i] = (+ reduce [j in xD] (A[i,j] * x[j])) - y[i,n+1];

  return res;
}
