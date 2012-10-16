//
// Use standard modules for vector and matrix Norms, Random numbers
// and Timing routines
//
use Norm, Random, Time;
use utils, UtilMath;

//
// Use the user module for computing HPCC problem sizes
//
use HPCCProblemSize;

//
// Use the distributions we need for this computation
//
use DimensionalDist2D, ReplicatedDim, BlockCycDim;
use ReplicatedDist;

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
config const n = 62,
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
config const useRandomSeed = false,
             seed = if useRandomSeed then SeedGenerator.currentTime else 31415;

//
// Configuration constants to control what's printed -- benchmark
// parameters, input and output arrays, and/or statistics
//
config const printParams = false,
             printArrays = false,
             printStats = true;

// These are solely to make the testing system happy given the COMPOPTS file.
// To be removed once COMPOPTS becomes a non-issue.
config var reproducible = false, verbose = false;

// skip some things
config var verify = true;
config var blast = max(n.type);
config var onlyBsub = false;
config var maxKinPS = max(int);
config var realInitAB = true;

// hooks for debugging
proc vmsg(args...) {}
proc vmsgmore(args...) {}
record VTimer { proc start(){} proc stop(){} proc printTotal(){} }
var tPS1iter, tUBR1iter, tSC1call, tLF1iter, tBScall, tVer: VTimer;  

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
/*
  if printParams && printStats then
    writeln("target locales ", tl1, " x ", tl2);
*/

  writeln("HPL  n ", n, "  blkSize ", blkSize, "  blk/node ",
          divceilpos(n, blkSize*tl1), "*", divceilpos(n+1, blkSize*tl2),
          "  locs ", tl1, "*", tl2, "  dPTPL ", dataParTasksPerLocale);

  const distReplicated = new ReplicatedDist();

  const
    // block-cyclic for 1st dimension
    bdim1 = new BlockCyclicDim(lowIdx=1, blockSize=blkSize, numLocales=tl1),
    // replicated for 1st dimension
    rdim1 = new ReplicatedDim(tl1),

    // block-cyclic for 2nd dimension
    bdim2 = new BlockCyclicDim(lowIdx=1, blockSize=blkSize, numLocales=tl2),
    // replicated for 2nd dimension
    rdim2 = new ReplicatedDim(tl2);

  const
    dist1b2b = new DimensionalDist2D(targetLocales, bdim1, bdim2, "dist1b2b"),
    dist1b2r = new DimensionalDist2D(targetLocales, bdim1, rdim2, "dist1b2r"),
    dist1r2b = new DimensionalDist2D(targetLocales, rdim1, bdim2, "dist1r2b");

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
  const AbD = {1..n, 1..n+1} dmapped new dmap(dist1b2b);

  var Ab : [AbD] elemType,           // the matrix A and vector b
      piv: [1..n] indexType;         // a vector of pivot values

  vmsg("allocated Ab");

  //
  // Create the 1-d replicated arrays for schurComplement().
  //
  const replAD = {1..n, 1..blkSize}   dmapped new dmap(dist1b2r),
        replBD = {1..blkSize, 1..n+1} dmapped new dmap(dist1r2b);

  var replA: [replAD] elemType,
      replB: [replBD] elemType;

  vmsg("allocated replA,B");

  const replKD = {0..0, 1..n+1} dmapped new dmap(dist1r2b);
  var   replK: [replKD] elemType;

  const replUD = {0..#blkSize, 0..#blkSize} dmapped new dmap(distReplicated);
        // can't use targetLocales because of "consistency" req. - that's OK
  var   replU: [replUD] elemType;

  vmsg("allocated replK,U");

  initAB();

  const startTime = getCurrentTime();     // capture the start time

 if !onlyBsub then
  LUFactorize(n, piv);                 // compute the LU factorization

  var x => backwardSub(n);  // perform the back substitution

  const execTime = getCurrentTime() - startTime;  // store the elapsed time
  printTime(execTime);
  quit(doExit = false);

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
    if blk > blast {
      writeln("terminating early before blk ", blk);
      quit();
    }

    tLF1iter.start();
    if printStats then writeln("processing block ", blk);
    else vmsg("BLOCK  blk ", blk);

    const tl = AbD[blk..#blkSize, blk..#blkSize],
          tr = AbD[blk..#blkSize, blk+blkSize..],
          bl = AbD[blk+blkSize.., blk..#blkSize],
          br = AbD[blk+blkSize.., blk+blkSize..],
          l  = AbD[blk.., blk..#blkSize];
    vmsg("slices");

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
    tLF1iter.stop();
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
proc schurComplement(blk, AD, BD, Rest) {

  // Prevent replication of unequal-sized slices
  if Rest.numIndices == 0 then return;

  tSC1call.start();

  //
  // Copy data into replicated arrays so every processor has a local copy
  // of the data it will need to perform a local matrix-multiply.
  //

  const AbSlice1 => Ab[1..n, AD.dim(2)],
        AbSlice2 => Ab[BD.dim(1), 1..n+1];
  vmsgmore("  AbSlices");


  forall (ab, ra) in zip(AbSlice1, replA) do
    local
      ra = ab;
  replicateA(blk);
  vmsgmore("  replA");

  forall (ab, rb) in zip(AbSlice2, replB) do
    local
      rb = ab;
  replicateB(blk);
  vmsgmore("  replB");

  // do local matrix-multiply on a block-by-block basis
  forall (row,col) in Rest by (blkSize, blkSize) {
    // localize Rest explicitly as a workaround;
    // also hoist the innerRange computation
    const outterRange = Rest.dim(1)(row..#blkSize),
          innerRange  = Rest.dim(2)(col..#blkSize),
          blkRange = 1..blkSize;

   local {

    var h1 => Ab._value.dsiLocalSlice1((outterRange, innerRange)),
        h2 => replA._value.dsiLocalSlice1((outterRange, blkRange)),
        h3 => replB._value.dsiLocalSlice1((blkRange, innerRange));

      for a in outterRange do
        for w in blkRange do
          for b in innerRange do
            //Ab[a,b] -= replA[a,w] * replB[w,b];
            h1[a,b] -= h2[a,w] * h3[w,b];
   } // local
  } // forall
  vmsg("schurComplement()");
  tSC1call.stop();
}

/////////////////////////////////

// NB works only when storage indices for sliceDim1,2 are contiguous
// Each sliceDim1,2 can be either a range or an int

proc DimensionalArr.dsiLocalSlice1((sliceDim1, sliceDim2)) {
  // might be more elegant to replace the explicit arg tuple with 'slice'
  proc toScalar(slice)
    return if _isIntegralType(slice.type) then slice else slice.low;
  proc toRange(slice)
    return if _isIntegralType(slice.type) then slice..slice else slice;
  proc origScalar(param dim) param
    return _isIntegralType(if dim==1 then sliceDim1.type else sliceDim2.type);
  proc toOrig(param dim, arg)
    return if origScalar(dim) then toScalar(arg) else toRange(arg);

  const dom = this.dom;
  const dist = dom.dist;
  // todo: cache (l1, l2) in privatized copies when possible
  // (i.e. if privatization is supported and there is no oversubscription)
  // Assuming dsiLocalSlice is guaranteed to be local to 'here'.
  const l1 = dist.di1.dsiIndexToLocale1d(toScalar(sliceDim1)),
        l2 = dist.di2.dsiIndexToLocale1d(toScalar(sliceDim2)),
        locAdesc = this.localAdescs[l1, l2],
        r1 = if dom.dom1.dsiStorageUsesUserIndices()
             then if origScalar(1) then sliceDim1 else dom.whole.dim(1)(sliceDim1)
             else toOrig(1, locAdesc.locDom.doml1.dsiLocalSliceStorageIndices1d(dom.dom1, toRange(sliceDim1))),
        r2 = if dom.dom2.dsiStorageUsesUserIndices()
             then if origScalar(2) then sliceDim2 else dom.whole.dim(2)(sliceDim2)
             else toOrig(2, locAdesc.locDom.doml2.dsiLocalSliceStorageIndices1d(dom.dom2, toRange(sliceDim2)));

  const reindexExpr =
    if origScalar(1) then
      if origScalar(2) then compilerError("dsiLocalSlice1 is not supported for two scalars")
      else tuple(sliceDim2)
    else
      if origScalar(2) then tuple(sliceDim1)
      else tuple(sliceDim1, sliceDim2);
  var result: [(...reindexExpr)] => locAdesc.myStorageArr[r1, r2];
  return result;
}

/////////////////////////////////

//
// do unblocked-LU decomposition within the specified panel, update the
// pivot vector accordingly
//
proc panelSolve(
               panel: domain,
               piv: [] indexType) {

  const blk = panel.dim(1).low;
  const cornerLocale = targetLocaleCorner(blk);
  var kCount = maxKinPS;

  for k in panel.dim(2) {             // iterate through the columns

    kCount -= 1;
    if kCount <= 0 {
      vmsg("panelSolve() skipping from k ", k);
      return;
    }

    tPS1iter.start();
    const col = panel[k.., k..k];
    vmsgmore("  col");
    
    // If there are no rows below the current column return
    if col.numIndices == 0 then { vmsg("panelSolve()"); return; }
    
    // Find the pivot, the element with the largest absolute value.
    const (_, (pivotRow, _)) = maxloc reduce zip(abs(Ab(col)), col);
    vmsgmore("  reduce");

    // Capture the pivot value explicitly (note that result of maxloc
    // is absolute value, so it can't be used directly).
    //
    const pivotVal = Ab[pivotRow, k];
    
    // Swap the current row with the pivot row and update the pivot vector
    // to reflect that
    Ab[k..k, ..] <=> Ab[pivotRow..pivotRow, ..];
    piv[k] <=> piv[pivotRow];
    vmsgmore("  swaps");

    if (pivotVal == 0) then
      halt("Matrix cannot be factorized");
    
    // divide all values below and in the same col as the pivot by
    // the pivot value
    Ab[k+1.., k..k] /= pivotVal;
    vmsgmore("  pivotVal");
    
    // update all other values below the pivot
/*
    forall (i,j) in panel[k+1.., k+1..] do
      Ab[i,j] -= Ab[i,k] * Ab[k,j];
*/
    // replicate
    on cornerLocale {
      local
        for j in panel.dim(2)[k+1..] do
          replK[0,j] = Ab[k,j];
      vmsgmore("  seeding replication");
      replicateK(blk);
      vmsgmore("  replicating K");
    }
    // compute
    forall (i,j) in panel[k+1.., k+1..] do
      local
        Ab[i,j] -= Ab[i,k] * replK[0,j];

    vmsgmore("panelSolve ", k);
    tPS1iter.stop();
  }
  vmsg("panelSolve()");
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
  const blk = tl.dim(1).low;
  const cornerLocale = targetLocaleCorner(blk);
  // 'tl' is a Dimensional-mapped domain, so has stuff
  // on all locales, even where the index set is empty
  const tlDims = tl.dims();
  on cornerLocale {
    const tlDimsLocal = tlDims; // where is value forwarding???
    local
      // we do not need the last row of tl, but pruning it off seems expensive
      forall (i,j) in {(...tlDimsLocal)} do
        replU[i-blk, j-blk] = Ab[i,j];
    vmsgmore("  seeding replication");
    replicateU(blk);
    vmsgmore("  replicating U");
  }
  for row in tr.dim(1) {
    tUBR1iter.start();
    const activeRow = tr[row..row, ..],
          prevRows = tr.dim(1).low..row-1;

    forall (i,j) in activeRow do
      for k in prevRows {
        if boundsChecking {
          const v1 = Ab[i,k];
          const v2 = replU[i-blk, k-blk];
          if v1 != v2 then halt("replU mismatch ", v1, " vs ", v2,
                                "  at ", (i,k,j));
        }
        local
          Ab[i, j] -= replU[i-blk, k-blk] * Ab[k,j];
      }

    vmsgmore("  row ", row);
    tUBR1iter.stop();
  }
  vmsg("updateBlockRow()");
}

/////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////

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

proc quit(doExit = true) {
  writeln();
  writeln("Timer totals are:");
  tPS1iter.printTotal();
  tUBR1iter.printTotal();
  tSC1call.printTotal();
  tLF1iter.printTotal();
  tBScall.printTotal();
  writeln();
  if doExit { writeln("quit(): exiting");  exit(0); }
}
  

//   
// construct an n by n+1 matrix filled with random values and scale
// it to be in the range -1.0..1.0
//
proc initAB() {
  tInit.start();
 if realInitAB {
  fillRandom(Ab, seed);
  vmsg("fillRandom");
  forall ab in Ab do local { ab = ab * 2.0 - 1.0; }  //Ab = Ab * 2.0 - 1.0;
  vmsg("Ab computation in initAB");
 } else {
   forall (ab, init) in zip(Ab, initABalt()) {
     ab = init;
   }
   vmsg("initABalt()");
 }
  tInit.stop();
}

iter initABalt(param tag: iterKind, followThis): real
  where tag == iterKind.follower
{
  const iaStart = -0.912534, iaStep = 0.983, iaBound = 0.99,
        iaReset = -1.98234;

  //writeln("follower ", followThis, "  on ", here.id);
  // we know that followThis.low will be >= 0
  const f1 = followThis(1).low + 1,
        f2 = followThis(2).low + 1;
  var cur = iaStart + 0.77 / f1 + 0.83 / f2;
  const step = iaStep / f1 / f2;

  inline proc advance() {
    if boundsChecking then
      if cur <= -1 || cur >= 1 then halt("initABalt: invalid cur = ", cur);
    cur += step;
    if cur > iaBound then cur += iaReset;
    return cur;
  }

  if boundsChecking {
    for f1 in followThis(1) do
      for f2 in followThis(2) do
        yield advance();
  } else {
    // we don't care how many times to do this
    while true do
      yield advance();
  }
}

iter initABalt(): real
{
  writeln("dummy initABalt");
  yield 0;
}

//
// calculate norms and residuals to verify the results
//
proc verifyResults(x) {
  if !verify then return true;
  writeln("verifying results locally...");
  tVer.start();
  initAB();
  var ABglobal => Ab;
  var checkOK: bool;
 { // hide global Ab
  var Ab: [1..n, 1..n+1] elemType;
  if numLocales <= 4 {  // heuristic
    Ab = ABglobal;
    vmsg("localized Ab - 1");
  } else {
    forall (g,l) in zip(ABglobal,Ab) do l = g;
    vmsg("localized Ab - 2");
  }

  const axmbNorm = norm(gaxpyMinus(Ab[.., 1..n], x, Ab[.., n+1..n+1]), normType.normInf);
  vmsgmore("axmbNorm");

  const a1norm   = norm(Ab[.., 1..n], normType.norm1),
        aInfNorm = norm(Ab[.., 1..n], normType.normInf),
        x1Norm   = norm(Ab[.., n+1..n+1], normType.norm1),
        xInfNorm = norm(Ab[.., n+1..n+1], normType.normInf);
  vmsgmore("four norms");

  const resid1 = axmbNorm / (epsilon * a1norm * n),
        resid2 = axmbNorm / (epsilon * a1norm * x1Norm),
        resid3 = axmbNorm / (epsilon * aInfNorm * xInfNorm);
  vmsgmore("three resids");

  checkOK = max(resid1, resid2, resid3) < 16.0;
  if (printStats && !checkOK) {
    writeln("resid1: ", resid1);
    writeln("resid2: ", resid2);
    writeln("resid3: ", resid3);
  }
 } // hide global Ab
  tVer.stop();
  return checkOK;
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

proc replicateU(abIx) {
    const fromLocId1 = targetLocalesIndexForAbIndex(1, abIx);
    const fromLocId2 = targetLocalesIndexForAbIndex(2, abIx);
    // verify that we are on a good locale, optionally
    if boundsChecking {
      const fromLocale = targetLocales(fromLocId1, fromLocId2);
      if here != fromLocale then
        // todo: halt or writeln?
        halt("replicateU(abIx=", abIx, ")  executing on ",
                here.id, "  expecting ", fromLocale.id, " ",
                (fromLocId1, fromLocId2));
    }
    var myLocalArr => replU._value.localArrs[here.id].arrLocalRep;
    coforall targloc in targetLocales[fromLocId1, ..] do
      if targloc != here then
        replU._value.localArrs[targloc.id].arrLocalRep = myLocalArr;
}

proc targetLocalesIndexForAbIndex(param dim, abIx)
  return (abIx / blkSize) % (if dim == 1 then tl1 else tl2);

proc targetLocaleForAbIndex(i,j)
  return targetLocales[targetLocalesIndexForAbIndex(1, i),
                       targetLocalesIndexForAbIndex(2, j)];

proc targetLocaleCorner(blk) {
  const cornerLocale = targetLocaleForAbIndex(blk, blk);
  if boundsChecking {
    const altLocale = Ab[blk,blk].locale;
    if cornerLocale != altLocale then
      halt("cornerLocale(blk=", blk, ")  mismatch:",
           "  comp1 -> ", cornerLocale.id, " locale idx ",
           (targetLocalesIndexForAbIndex(1, blk),
            targetLocalesIndexForAbIndex(2, blk)),
           "  comp2 -> ", altLocale.id);
  }
  return cornerLocale;
}

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
proc printTime(execTime) {
  if (printStats) {
    const GflopPerSec = ((2.0/3.0) * n**3 + (3.0/2.0) * n**2) / execTime * 10e-9;
    writeln(execTime, " sec   ", GflopPerSec, " GFLOPS");
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
