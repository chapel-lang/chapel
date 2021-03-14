//
// This is a copy of the version in release/examples as of the 1.0.2 release
//  with the arrays domain mapped by Dist2D and the local block removed in
//  schurComplement.
//
use driver;

//
// Use standard modules for vector and matrix Norms, Random numbers
// and Timing routines
//
use LinearAlgebra, Random, Time;

//
// Use the user module for computing HPCC problem sizes
//
use HPCCProblemSize;

//
// The number of matrices and the element type of those matrices
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
  // domain mapped using a BlockCyclic(blkSize) distribution.
  //
  const MatVectSpace: domain(2, indexType) dmapped Dist2D = {1..n, 1..n+1},
        MatrixSpace = MatVectSpace[.., ..n];

  var Ab : [MatVectSpace] elemType,  // the matrix A and vector b
      piv: [1..n] indexType,         // a vector of pivot values
      x  : [1..n] elemType;          // the solution vector, x

  ref A = Ab[MatrixSpace],           // an alias for the Matrix part of Ab
      b = Ab[.., n+1];               // an alias for the last column of Ab

  initAB(Ab);

  const startTime = getCurrentTime();     // capture the start time

  LUFactorize(n, Ab, piv);                 // compute the LU factorization

  x = backwardSub(n, A, b);  // perform the back substitution

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
proc LUFactorize(n: indexType, Ab: [1..n, 1..n+1] elemType,
                piv: [1..n] indexType) {
  const AbD = Ab.domain;    // alias Ab.domain to save typing
  
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
    if (tr.size > 0) then
      updateBlockRow(Ab, tl, tr);
    
    //
    // update trailing submatrix (if any)
    //
    if (br.size > 0) then
      schurComplement(Ab, blk);
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
//     |aaaaa|.....|.....|.....|  The vertex labeled [1] is location
//     |aaaaa|.....|.....|.....|  (ptOp, ptOp) in the code below.
//     +-----+-----+-----+-----+
//     |aaaaa|.....|.....|.....|  The vertex labeled [2] is location
//     |aaaaa|.....|.....|.....|  (ptSol, ptSol)
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
proc schurComplement(Ab: [1..n, 1..n+1] elemType, ptOp: indexType) {
  const AbD = Ab.domain;

  //
  // Calculate location of ptSol (see diagram above)
  //
  const ptSol = ptOp+blkSize;

  //
  // Copy data into replicated array so every processor has a local copy
  // of the data it will need to perform a local matrix-multiply.  These
  // replicated distributions aren't implemented yet, but imagine that
  // they look something like the following:
  //
  //var replAbD: domain(2) 
  //            dmapped new Dimensional(BlkCyc(blkSize), Replicated)) 
  //          = AbD[ptSol.., 1..#blkSize];
  //
  const replAD: domain(2) dmapped Dist2D = AbD[ptSol.., ptOp..#blkSize],
        replBD: domain(2) dmapped Dist2D = AbD[ptOp..#blkSize, ptSol..];
    
  const replA : [replAD] elemType = Ab[ptSol.., ptOp..#blkSize],
        replB : [replBD] elemType = Ab[ptOp..#blkSize, ptSol..];

  // do local matrix-multiply on a block-by-block basis
  forall (row,col) in AbD[ptSol.., ptSol..] by (blkSize, blkSize) {
    //
    // At this point, the dgemms should all be local, so assert that
    // fact
    //
    // local { // Removed for distribution testing
      const aBlkD = replAD[row..#blkSize, ptOp..#blkSize],
            bBlkD = replBD[ptOp..#blkSize, col..#blkSize],
            cBlkD = AbD[row..#blkSize, col..#blkSize];

      dgemm(replA(aBlkD),
            replB(bBlkD),
            Ab(cBlkD));
      // }
  }
}

//
// calculate C = C - A * B.
//
proc dgemm(A: [?AD] ?t,
           B: [?BD] t,
           C: [?CD] t) {
  // Calculate (i,j) using a dot product of a row of A and a column of B.
  for i in AD.dim(0) do
    for j in CD.dim(1) do
      for k in AD.dim(1) do
        C[i,j] -= A[i, k] * B[k, j];
}

//
// do unblocked-LU decomposition within the specified panel, update the
// pivot vector accordingly
//
proc panelSolve(Ab: [] ?t,
               panel: domain(2, indexType) dmapped Dist2D,
               piv: [] indexType) {
  const pnlRows = panel.dim(0),
        pnlCols = panel.dim(1);

  //
  // Ideally some type of assertion to ensure panel is embedded in Ab's
  // domain
  //
  assert(piv.domain.dim(0) == Ab.domain.dim(0));
  
  if (pnlCols.size == 0) then return;
  
  for k in pnlCols {             // iterate through the columns
    var col = panel[k.., k..k];
    
    // If there are no rows below the current column return
    if col.dim(0).size == 0 then return;
    
    // Find the pivot, the element with the largest absolute value.
    const (_, (pivotRow, _)) = maxloc reduce zip(abs(Ab(col)), col),
          pivot = Ab[pivotRow, k];
    
    // Swap the current row with the pivot row
    piv[k] <=> piv[pivotRow];

    Ab[k, ..] <=> Ab[pivotRow, ..];
    
    if (pivot == 0) then
      halt("Matrix can not be factorized");
    
    // divide all values below and in the same col as the pivot by
    // the pivot
    if k+1 <= pnlRows.high then
      Ab(col)[k+1.., k..k] /= pivot;
    
    // update all other values below the pivot
    if k+1 <= pnlRows.high && k+1 <= pnlCols.high then
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
proc updateBlockRow(Ab: [] ?t, tl: domain(2) dmapped Dist2D, tr: domain(2) dmapped Dist2D) {
  const tlRows = tl.dim(0),
        tlCols = tl.dim(1),
        trRows = tr.dim(0),
        trCols = tr.dim(1);
  
  assert(tlCols == trRows);

  //
  // Ultimately, we will probably want to do some replication of the
  // tl block in order to make this operation completely localized as
  // in the dgemm.  We have not yet undertaken that optimization.
  //
  for i in trRows do
    forall j in trCols do
      for k in tlRows.low..i-1 do
        Ab[i, j] -= Ab[i, k] * Ab[k,j];
}

//
// compute the backwards substitution
//
proc backwardSub(n: int,
                A: [1..n, 1..n] elemType,
                b: [1..n] elemType) {
  var x: [b.domain] elemType;

  for i in b.domain by -1 {
    x[i] = b[i];
    
    for j in i+1..b.domain.high do
      x[i] -= A[i,j] * x[j];

    x[i] /= A[i,i];
  }

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
  ref A = Ab[MatrixSpace],
      b = Ab[.., n+1];

  initAB(Ab);
  
  const axmbNorm = norm(gaxpyMinus(n, n, A, x, b), normType.normInf);

  const a1norm   = norm(A, normType.norm1),
        aInfNorm = norm(A, normType.normInf),
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

//
// print success/failure, the execution time and the Gflop/s value
//
proc printResults(successful, execTime) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    writeln("Execution time = ", execTime);
    const GflopPerSec = ((2.0/3.0) * n**3 + (3.0/2.0) * n**2) / execTime * 10e-9;
    writeln("Performance (Gflop/s) = ", GflopPerSec);
  }
}

//
// simple matrix-vector multiplication, solve equation A*x-y
//
proc gaxpyMinus(n: indexType,
               m: indexType,
               A: [1..n, 1..m],
               x: [1..m],
               y: [1..n]) {
  var res: [1..n] elemType;

  for i in 1..n do
    for j in 1..m do
      res[i] += A[i,j]*x[j];

  for i in 1..n do
    res[i] -= y[i];

  return res;
}
