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
use DimensionalDist2D, ReplicatedDim, BlockCycDim;

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
             seed = if useRandomSeed then SeedGenerator.currentTime else 31415;

//
// Configuration constants to control what's printed -- benchmark
// parameters, input and output arrays, and/or statistics
//
config const printParams = true,
             printArrays = false,
             printStats = true;

// These are solely to make the testing system happy given the COMPOPTS file.
// To be removed once COMPOPTS becomes a non-issue.
config var reproducible = false, verbose = false;

//
// The program entry point, currently module initialization
//
  printConfiguration();

  //
  // Compute targetLocales - required for Dimensional.
  // We hard-code 2 dimensions.
  //
  var targetIds: domain(2);
  var targetLocales: [targetIds] locale;
  setupTargetLocalesArray(targetIds, targetLocales, Locales);

  // Here are the dimensions of our grid of locales.
  const tl1 = targetIds.dim(1).length,
        tl2 = targetIds.dim(2).length;
  if printParams && printStats then
    writeln("target locales ", tl1, " x ", tl2);

  // Create individual dimension descriptors
  const
    // block-cyclic for 1st dimension
    bdim1 = new BlockCyclicDim(lowIdx=1, blockSize=blkSize, numLocales=tl1),
    // replicated for 1st dimension
    rdim1 = new ReplicatedDim(tl1),

    // block-cyclic for 2nd dimension
    bdim2 = new BlockCyclicDim(lowIdx=1, blockSize=blkSize, numLocales=tl2),
    // replicated for 2nd dimension
    rdim2 = new ReplicatedDim(tl2);

  //
  // MatVectSpace is a 2D domain of type indexType that represents the
  // n x n matrix adjacent to the column vector b.  MatrixSpace is a
  // subdomain that is created by slicing into MatVectSpace,
  // inheriting all of its rows and its low column bound.  As our
  // standard distribution library is filled out, MatVectSpace will be
  // distributed using a BlockCyclic(blkSize) distribution.
  //
  // We use 'AbD' instead of 'MatVectSpace' throughout.
  //
  const AbD: domain(2, indexType)
          dmapped DimensionalDist2D(targetLocales, bdim1, bdim2, "dim")
          = {1..n, 1..n+1},
        MatrixSpace = AbD[.., ..n];

  var Ab : [AbD] elemType,           // the matrix A and vector b
      piv: [1..n] indexType;         // a vector of pivot values

  //
  // Create the 1-d replicated arrays for schurComplement().
  //
  const
    replAD = {1..n, 1..blkSize}
      dmapped DimensionalDist2D(targetLocales, bdim1, rdim2, "distBR"),
    replBD = {1..blkSize, 1..n+1}
      dmapped DimensionalDist2D(targetLocales, rdim1, bdim2, "distRB");

  var replA: [replAD] elemType,
      replB: [replBD] elemType;

  const
    replKD = {0..0, 1..n+1}
      dmapped new dmap(replBD._value.dist);
//    dmapped DimensionalDist2D(targetLocales, rdim1, bdim2, "distRB-replK");

  var replK: [replKD] elemType;

  initAB();

  const startTime = getCurrentTime();     // capture the start time

  LUFactorize(n, piv);                 // compute the LU factorization

  var x = backwardSub(n);  // perform the back substitution

  const execTime = getCurrentTime() - startTime;  // store the elapsed time

  //
  // Validate the answer and print the results
  const validAnswer = verifyResults(x);
  printResults(validAnswer, execTime);

//
// blocked LU factorization with pivoting for matrix augmented with
// vector of RHS values.
//
proc LUFactorize(n: indexType,
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
    if printStats then writeln("processing block ", blk);

    const tl = AbD[blk..#blkSize, blk..#blkSize],
          tr = AbD[blk..#blkSize, blk+blkSize..],
          bl = AbD[blk+blkSize.., blk..#blkSize],
          br = AbD[blk+blkSize.., blk+blkSize..],
          l  = AbD[blk.., blk..#blkSize];

    //
    // Now that we've sliced and diced Ab properly, do the blocked-LU
    // computation:
    //
    panelSolve(l, piv);
    updateBlockRow(tl, tr);
    
    //
    // update trailing submatrix (if any)
    //
    schurComplement(blk, bl, tr, br);
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
proc schurComplement(blk, AD: domain, BD: domain, Rest: domain) {

  // Prevent replication of unequal-sized slices
  if Rest.numIndices == 0 then return;

  //
  // Copy data into replicated arrays so every processor has a local copy
  // of the data it will need to perform a local matrix-multiply.
  //

  const AbSlice1 => Ab[1..n, AD.dim(2)],
        AbSlice2 => Ab[BD.dim(1), 1..n+1];

  forall (ab, ra) in (AbSlice1, replA) do
    local
      ra = ab;
  replicateA(blk);

  forall (ab, rb) in (AbSlice2, replB) do
    local
      rb = ab;
  replicateB(blk);

  // do local matrix-multiply on a block-by-block basis
  forall (row,col) in Rest by (blkSize, blkSize) {
    // localize Rest explicitly as a workaround;
    // also hoist the innerRange computation
    const outterRange = Rest.dim(1)(row..#blkSize),
          innerRange  = Rest.dim(2)(col..#blkSize);

    local {
      for a in outterRange do
        for w in 1..blkSize do
          for b in innerRange do
            Ab[a,b] -= replA[a,w] * replB[w,b];
    }
  }
}

//
// do unblocked-LU decomposition within the specified panel, update the
// pivot vector accordingly
//
proc panelSolve(
               panel: domain,
               piv: [] indexType) {

  const blk = panel.dim(1).low;
  const cornerLocale = Ab[blk,blk].locale;

  for k in panel.dim(2) {             // iterate through the columns
    const col = panel[k.., k..k];
    
    // If there are no rows below the current column return
    if col.numIndices == 0 then return;
    
    // Find the pivot, the element with the largest absolute value.
    const (_, (pivotRow, _)) = maxloc reduce(abs(Ab(col)), col);

    // Capture the pivot value explicitly (note that result of maxloc
    // is absolute value, so it can't be used directly).
    //
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
    // (a) replicate
    on cornerLocale {
      local
        for j in panel.dim(2)[k+1..] do
          replK[0,j] = Ab[k,j];
      replicateK(blk);
    }
    // (b) compute
    forall (i,j) in panel[k+1.., k+1..] do
      local
        Ab[i,j] -= Ab[i,k] * replK[0,j];
  }
}

//
// Update the block row (tr for top-right) portion of the matrix in a
// blocked LU decomposition.  Each step of the LU decomposition will
// solve a block (tl for top-left) portion of a matrix. This function
// solves the rows to the right of the block.
//
proc updateBlockRow(
                   tl: domain,
                   tr: domain) {

  for row in tr.dim(1) {
    const activeRow = tr[row..row, ..],
          prevRows = tr.dim(1).low..row-1;

    forall (i,j) in activeRow do
      for k in prevRows do
        Ab[i, j] -= Ab[i, k] * Ab[k,j];
  }
}


//
// compute the backwards substitution
//
proc backwardSub(n: indexType) {
  const bd = Ab.domain.dim(1);  // or simply 1..n
  var x: [bd] elemType;

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
proc initAB() {
  fillRandom(Ab, seed);
  Ab = Ab * 2.0 - 1.0;
}

//
// calculate norms and residuals to verify the results
//
proc verifyResults(x) {
  initAB();
  
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

/////////////////////////////////////////////////////////////////////////////

proc replicateA(abIx) {
    const fromLocId2 = targetLocalesIndexForAbIndex(2, abIx);
    coforall lid1 in 0..#tl1 do
      on targetLocales(lid1, fromLocId2) do
        coforall lid2 in 0..#tl2 do
          if lid2 != fromLocId2 then
            replA._value.localAdescs[lid1,lid2].myStorageArr =
              replA._value.localAdescs[lid1,fromLocId2].myStorageArr;
}

proc replicateB(abIx) {
    const fromLocId1 = targetLocalesIndexForAbIndex(1, abIx);
    coforall lid2 in 0..#tl2 do
      on targetLocales(fromLocId1, lid2) do
        coforall lid1 in 0..#tl1 do
          if lid1 != fromLocId1 then
            replB._value.localAdescs[lid1,lid2].myStorageArr =
              replB._value.localAdescs[fromLocId1,lid2].myStorageArr;
}

// replicate only along one column of processors,
// starting on a good locale
proc replicateK(abIx) {
    const fromLocId1 = targetLocalesIndexForAbIndex(1, abIx);
    const fromLocId2 = targetLocalesIndexForAbIndex(2, abIx);
    // verify that we are on a good locale, optionally
    if boundsChecking {
      const fromLocale = targetLocales(fromLocId1, fromLocId2);
      if here != fromLocale then
        halt("replicateK(abIx=", abIx, ")  executing on ",
             here.id, "  expecting ", fromLocale.id, " ",
             (fromLocId1, fromLocId2));
    }
    const lid2 = fromLocId2;
        coforall lid1 in 0..#tl1 do
          if lid1 != fromLocId1 then
            replK._value.localAdescs[lid1,lid2].myStorageArr =
              replK._value.localAdescs[fromLocId1,lid2].myStorageArr;
}

proc targetLocalesIndexForAbIndex(param dim, abIx)
  return (abIx / blkSize) % (if dim == 1 then tl1 else tl2);


/////////////////////////////////////////////////////////////////////////////

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
proc gaxpyMinus(A: [],
                x: [?xD],
                y: [?yD]) {
  var res: [1..n] elemType;

  forall i in 1..n do
    // 'serial' to avoid spawning more tasks
    serial(true) { res[i] = (+ reduce [j in xD] (A[i,j] * x[j])) - y[i,n+1]; }

  return res;
}
