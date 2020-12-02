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
             seed = if useRandomSeed then SeedGenerator.oddCurrentTime else 31415;

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
config var realInitAB = true;
config var blast = max(n.type);
config var maxKinPS = max(int);
config var onlyBsub = false;

// do additional checking in backwardSub()?
config param checkBsub = boundsChecking;
// for debugging; need to disable 'local' in bsComputeMyXs()
config param bsFinalizeXiVerbose = false;
 
// hooks for debugging
proc vmsg(args...) {}
proc vmsgmore(args...) {}
record VTimer { proc start(){} proc stop(){} proc printTotal(){} }
var tInit, tPS1iter, tUBR1iter, tSC1call, tLF1iter, tBScall, tVer: VTimer;

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
  const tl1 = targetIds.dim(0).size,
        tl2 = targetIds.dim(1).size;

  if onlyBsub && tl1 != tl2 then
    halt("backwardSub() is implemented only for a square locale grid");
/*
  if printParams && printStats then
    writeln("target locales ", tl1, " x ", tl2);
*/

  writeln("HPL  n ", n, "  blkSize ", blkSize, "  blk/node ",
          divceilpos(n, blkSize*tl1), "*", divceilpos(n+1, blkSize*tl2),
          "  locs ", tl1, "*", tl2, "  dPTPL ", dataParTasksPerLocale);

  const distReplicated = new Replicated();

  // Replicate 'targetLocales' to reduce comm.
  // 'targetLocales' itself could be replicated instead,
  // but this way the changes are smaller.
  const targetIdsRepl = targetIds dmapped new dmap(distReplicated);
  var targetLocalesRepl: [targetIdsRepl] locale;
  coforall (l, id) in zip(Locales, LocaleSpace) do on l {
    targetLocalesRepl._value.localArrs[id].arrLocalRep = targetLocales;
  }
  vmsg("replicated targetLocales");

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

  const
    dist1b2b = new unmanaged DimensionalDist2D(targetLocales, bdim1, bdim2, "dist1b2b"),
    dist1b2r = new unmanaged DimensionalDist2D(targetLocales, bdim1, rdim2, "dist1b2r"),
    dist1r2b = new unmanaged DimensionalDist2D(targetLocales, rdim1, bdim2, "dist1r2b");

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

  //
  // additional data structures for backwardSub()
  //
  type cpuAtomicElemType = // presently unused
    if elemType == real(64) then chpl__processorAtomicType(real)
    else compilerError("need to define cpuAtomicElemType");
  type cpuAtomicCountType = chpl__processorAtomicType(int);
  const replPD = {0..#blkSize} dmapped new dmap(distReplicated);

  // the working 'x' vector - we reuse replK
  //
  // Implementation consequences: (A) Need to zero it out in backwardSub().
  // (B) If we allocated replX separately from replK, we need to
  // replicate X where backwardSub currently invokes replicateK().
  // 
  ref replX = replK;

  // Allocate these arrays just once.
  // Partial sums local to each node.
  var bsLocalPartSums: [replPD] elemType;

  // Partial sums from another node in the row
  // brought to the current diagonal node, with an "available" flag.
  param spsNA=0, spsAV=1, spsNBL=2;
  record bsSetofPartSums {
    var values: [0..#blkSize] elemType;
    // spsNA - not available yet or has already been read
    // spsAV - available
    // spsNBL - no applicable blocks on that node
    var avail: cpuAtomicCountType; // initially 0
  }

  // (Only needed on diagonal nodes, for simplicity create everywhere.)
  const bsOthersPartSumsD = {0..#tl2} dmapped new dmap(distReplicated);
  var bsOthersPartSums: [bsOthersPartSumsD] bsSetofPartSums;

  initAB();

  const startTime = getCurrentTime();     // capture the start time

 if !onlyBsub then
  LUFactorize(n, piv);                 // compute the LU factorization

  ref x = backwardSub(n);  // perform the back substitution

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
  if Rest.size == 0 then return;

  tSC1call.start();

  //
  // Copy data into replicated arrays so every processor has a local copy
  // of the data it will need to perform a local matrix-multiply.
  //

  const ref AbSlice1 = Ab[1..n, AD.dim(1)],
            AbSlice2 = Ab[BD.dim(0), 1..n+1];
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
    const outterRange = Rest.dim(0)(row..#blkSize),
          innerRange  = Rest.dim(1)(col..#blkSize),
          blkRange = 1..blkSize;

   local {

    ref h1 = Ab._value.dsiLocalSlice1((outterRange, innerRange)),
        h2 = replA._value.dsiLocalSlice1((outterRange, blkRange)),
        h3 = replB._value.dsiLocalSlice1((blkRange, innerRange));

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
    return if isIntegralType(slice.type) then slice else slice.low;
  proc toRange(slice)
    return if isIntegralType(slice.type) then slice..slice else slice;
  proc origScalar(param dim) param
    return isIntegralType(if dim==1 then sliceDim1.type else sliceDim2.type);
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
             then if origScalar(1) then sliceDim1 else dom.whole.dim(0)(sliceDim1)
             else toOrig(1, locAdesc.locDom.doml1.dsiLocalSliceStorageIndices1d(dom.dom1, toRange(sliceDim1))),
        r2 = if dom.dom2.dsiStorageUsesUserIndices()
             then if origScalar(2) then sliceDim2 else dom.whole.dim(1)(sliceDim2)
             else toOrig(2, locAdesc.locDom.doml2.dsiLocalSliceStorageIndices1d(dom.dom2, toRange(sliceDim2)));

  const reindexExpr =
    if origScalar(1) then
      if origScalar(2) then compilerError("dsiLocalSlice1 is not supported for two scalars")
      else (sliceDim2,)
    else
      if origScalar(2) then (sliceDim1,)
      else (sliceDim1, sliceDim2);
  ref result = locAdesc.myStorageArr[r1, r2].reindex(reindexExpr);
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

  const blk = panel.dim(0).low;
  const cornerLocale = targetLocaleCorner(blk);
  var kCount = maxKinPS;

  for k in panel.dim(1) {             // iterate through the columns

    kCount -= 1;
    if kCount <= 0 {
      vmsg("panelSolve() skipping from k ", k);
      return;
    }

    tPS1iter.start();
    const col = panel[k.., k..k];
    vmsgmore("  col");
    
    // If there are no rows below the current column return
    if col.size == 0 then { vmsg("panelSolve()"); return; }
    
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
        for j in panel.dim(1)[k+1..] do
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
  const blk = tl.dim(0).low;
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
  for row in tr.dim(0) {
    tUBR1iter.start();
    const activeRow = tr[row..row, ..],
          prevRows = tr.dim(0).low..row-1;

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
proc backwardSubRef(n: indexType) {
  // make the rest of the code operate locally without changing it
  ref Ab = makeLocalCopyOfAb();

  const bd = Ab.domain.dim(0);  // or simply 1..n
  var x: [bd] elemType;

  for i in bd by -1 do
    x[i] = (Ab[i,n+1] - (+ reduce [j in i+1..bd.high] (Ab[i,j] * x[j]))) 
            / Ab[i,i];

  return x;
}

// Illustrates bsFinalizeXi().
proc backwardSubAlt(n: indexType) {
  ref Ab = makeLocalCopyOfAb();

  const bd = Ab.domain.dim(0);  // or simply 1..n
  var x: [bd] elemType;

  for i in bd by -1 do
  {
    const redu = (+ reduce [j in i+1..bd.high] (Ab[i,j] * x[j]));
    bsFinalizeXi(i, x[i], redu, Ab[i,i], Ab[i,n+1]);
  }

  return x;
}

inline proc bsFinalizeXi(i, ref x_i, partSum, Ab_i_i, Ab_i_n1) {
  if bsFinalizeXiVerbose then // beware of 'local' in bsComputeMyXs()
    writeln("bsFinalizeXi  ", i, "  ", x_i, "  ", partSum, "  ",
            "  Ab[i,i] ", Ab_i_i, "  Ab[i,n+1] ", Ab_i_n1);
  x_i = (Ab_i_n1 - (x_i + partSum)) / Ab_i_i;
}

//
// compute the backwards substitution, the new way
//
proc backwardSub(nArg: indexType) {
  if tl1 != tl2 then
    halt("backwardSub() is implemented only for a square locale grid");
  assert(n == nArg, "bs-n"); // do not rely on value forwarding for 'nArg',
                             // instead rely on constant replication for 'n'

  tBScall.start();
  const diaFrom_N = (divceilpos(n, blkSize)-1) * blkSize + 1,
        diaTo_N   = n,
        locId1_N  = targetLocalesIndexForAbIndex(1, diaFrom_N),
        locId2_N  = targetLocalesIndexForAbIndex(2, diaFrom_N);

  vmsg("BS diagonal ", diaFrom_N, "..", diaTo_N,
       "  on [", locId1_N, ",", locId2_N, "]");
  if checkBsub {
    assert(diaTo_N < diaFrom_N + blkSize, "bs-7-1");
    assert(diaFrom_N % blkSize == 1, "bs-7-2");
  }

  on targetLocales[locId1_N, locId2_N] {

    // the key computation for the last (lower-right) diagonal block
    // no data dependencies - just compute X
    // zeros out its block of replX at start
    bsComputeMyXs(diaFrom_N, diaTo_N, locId1_N, locId2_N, true);
    vmsgmore("  bsComputeMyXs");

    // TODO: all except the block right above can be done in background
    replicateK(diaFrom_N);  // replicates replX
    vmsgmore("  replicateK");
  }

  var prevLocId1 = locId1_N,
      prevLocId2 = locId2_N,
      prevDiaFrom = diaFrom_N;

  while prevDiaFrom > 1 {
      
    // the immediately preceeding block along the diagonal
    const locId1 = (prevLocId1 + (tl1-1)) % tl1,
          locId2 = (prevLocId2 + (tl2-1)) % tl2,
          // const-ify for value forwarding
          diaFrom = prevDiaFrom - blkSize,
          diaTo   = prevDiaFrom - 1;

    vmsg("BS diagonal ", diaFrom, "..", diaTo,
         "  on [", locId1, ",", locId2, "]");
    if checkBsub {
      assert(diaTo == diaFrom + blkSize - 1, "bs-8-1");
      assert(diaFrom_N % blkSize == 1, "bs-8-2");
    }

    // update "previous" vars
    prevLocId1 = locId1;
    prevLocId2 = locId2;
    prevDiaFrom = diaFrom;

    // the key computation for the current diagonal block
    // (if serialization is desired, do bsComputeRow for locCol==locId2 LAST)
    coforall locCol in 0..#tl2 do
      on targetLocales[locId1, locCol] do
        bsComputeRow(diaFrom, diaTo, locId1, locCol, locId2);

  }  // while prevDiaFrom

  vmsg("BS computations");

  // gather into a DR array
  // What's copied has been updated appropriately
  // because we do replication synchronously.
  var xTemp: [(...replX.domain.dims())] elemType = replX;

  // The 'forall' below, as an alternative to 'x = replX', would
  // iterate over *all* locales, which is too much and also causes
  // the error 'zippered iterations have non-equal lengths'.
  //forall (repl,locl) in zip(replX,x) do locl = repl;

  vmsg("BS copy");
  // end the new algorithm

  tBScall.stop();
  return xTemp[0, 1..n];
}

proc bsComputeRow(diaFrom, diaTo, locId1, locId2, diaLocId2) {
  if checkBsub {
    assert(here == targetLocalesRepl[locId1, locId2], "bsR-1-1");
    // we are doing a full block - this is an FYI
    assert(diaTo == diaFrom + blkSize - 1, "bsR-1-3");
  }

  const onDiag = locId2 == diaLocId2;
  if onDiag {
    // NB the following picks locId2-th element from the current
    // replicand of bsOthersPartSums.
    ref myPartSums = bsOthersPartSums[locId2].values;
    ensureDR(myPartSums._value, "bsR myPartSums");

    var gotBlocks: bool; // ignored
    bsComputePartSums(diaFrom, diaTo, locId1, locId2, diaLocId2,
                      myPartSums, gotBlocks);
    bsOthersPartSums[locId2].avail.write(if gotBlocks then spsAV else spsNBL);
    vmsgmore("  bsR-diag bsComputePartSums  gotBlocks=", gotBlocks);

    // zeros out its block of replX at start
    bsIncorporateOthersPartSums(diaFrom, diaTo, locId1, diaLocId2);
    vmsgmore("  bsR-diag bsIncorporateOthersPartSums");

    bsComputeMyXs(diaFrom, diaTo, locId1, locId2, false);
    vmsgmore("  bsR-diag bsComputeMyXs");

    // TODO: all except the block right above can be done in background
    replicateK(diaFrom);  // replicates replX
    vmsgmore("  bsR-diag replicateK");

    // Reset partial values for future use.
    local do for ps in myPartSums do ps = 0;
    vmsgmore("  bsR-diag reset myPartSums");

  } else {
    // off the diagonal

    ref myPartSums = bsLocalPartSums._value.localArrs[here.id].arrLocalRep;
    ensureDR(myPartSums._value, "bsR myPartSums");

    if checkBsub {
      assert(myPartSums.domain.dims() == (0..#blkSize,), "bsR-2");
      // Verify initialization.
      for ps in myPartSums do assert(ps == 0, "bsR-3");
    }

    var gotBlocks: bool;
    bsComputePartSums(diaFrom, diaTo, locId1, locId2, diaLocId2,
                      myPartSums, gotBlocks);

    bsSendPartSums(diaFrom, diaTo, locId1, locId2, diaLocId2,
                   myPartSums, gotBlocks);

    // Reset partial values for future use.
    local do for ps in myPartSums do ps = 0;
  }
}

proc bsComputeMyXs(diaFrom, diaTo, locId1, locId2, zeroOutX) {
  const diaSlice = diaFrom..diaTo;
  ref locAB = Ab._value.dsiLocalSlice1((diaSlice, diaSlice));
  ref locX = replX._value.dsiLocalSlice1((0, diaSlice));

  if checkBsub {
    assert(here == targetLocalesRepl[locId1, locId2], "bsX-1");
    assert(locAB.domain.dims() == (diaSlice, diaSlice), "bsX-2");
    assert(locX.domain.dims() == (diaSlice,), "bsX-3");
  }

  // because we are reusing replK for replX
  if zeroOutX then
    for x in locX do x = 0;

  if targetLocaleReplForAbIndex(diaFrom, n+1) == here {
    // Ab[diaSlice, n+1] is local
    local {
      ref locB = Ab._value.dsiLocalSlice1((diaSlice, n+1));
      bsComputeMyXsWithB(diaFrom, diaTo, locAB, locX, locB);
    }
  } else {
    // 'locB' is the local replicand, we use it as a temp array
    ref locB = replB._value.dsiLocalSlice1((1, diaSlice));
    // get the remote Ab[diaSlice, n+1] into locB
    on targetLocaleReplForAbIndex(diaFrom, n+1) {
      if checkBsub {
        // verify that that is really a local slice
        local do ref testDummy = Ab._value.dsiLocalSlice1((diaSlice, n+1));
      }
      // TODO: bulkify, unless it is already
      locB = Ab._value.dsiLocalSlice1((diaSlice, n+1));
    }
    local
      bsComputeMyXsWithB(diaFrom, diaTo, locAB, locX, locB);
  }
}

proc bsComputeMyXsWithB(diaFrom, diaTo, locAB, locX, locB) {
  // the i1 loop, as written, must be serial
  for i in diaFrom..diaTo by -1 {
    var sum: elemType = 0;
    for j in i+1..diaTo do
      sum +=locAB[i,j] * locX[j];
    bsFinalizeXi(i, locX[i], sum, locAB[i,i], locB[i]);
  }
}

proc bsIncorporateOthersPartSums(diaFrom, diaTo, locId1, locId2) {
  // Apart from asserts and writeln(), everything here should be local.
  ref partSums = bsOthersPartSums._value.localArrs[here.id].arrLocalRep;
  ref locX     = replX._value.dsiLocalSlice1((0, diaFrom..diaTo));
  ensureDR(partSums._value, "bsI partSums");
  ensureDR(locX._value, "bsI locX");

  if checkBsub {
    // Our own partial sums are already available.
    assert(partSums[locId2].avail.read() != spsNA, "bsI-1");
    assert(locX.size == blkSize, "bsI-3");
  }

  // because we are reusing replK for replX
  for x in locX do x = 0;

  var toIncorporate = tl2;
  var seenOther = false;

  inline proc ihelper(ref ps: bsSetofPartSums, locId2: int) {
    const avail = ps.avail.read();
    if avail == spsNA then return;
    if checkBsub then assert(avail == spsAV || avail == spsNBL, "bsI-4");
    // Indicate that we got it.
    ps.avail.write(spsNA);
    toIncorporate -= 1;
    seenOther = true;
    //writeln("incorporating avail=", avail, "  [", locId1, ",", locId2, "]");

    if avail == spsAV {
      // yes incorporate
      for i in 0..#blkSize do locX[diaFrom+i] += ps.values[i];
    } else {
      // nothing to incorporate
    }
  } // proc ihelper

  local
    while toIncorporate > 0 {
      seenOther = false;
      // since incorporation in ihelper
      for l2 in 0..#tl2 do ihelper(partSums[l2], l2);
      if !seenOther then chpl_task_yield();
    }
  // We must have seen *something* the last time around.
  if checkBsub then assert(seenOther, "bsI-2");
  vmsg("  bsIncorporateOthersPartSums");

}  // bsIncorporateOthersPartSums

proc bsSendPartSums(diaFrom, diaTo, locId1, locId2, diaLocId2,
                    myPartSums, gotBlocks) {
  on targetLocalesRepl[locId1, diaLocId2] {
    sendhelper(bsOthersPartSums[locId2]);

    proc sendhelper(ref ps: bsSetofPartSums) {
      if checkBsub then assert(ps.avail.read() == spsNA, "bsSPS-1");
      const avail = if gotBlocks then spsAV else spsNBL;
      if gotBlocks then
        // TODO: bulkify, unless it is already
        ps.values = myPartSums;
      ps.avail.write(avail);
    }
  }
}  // bsSendPartSums

proc bsComputePartSums(diaFrom, diaTo, locId1, locId2, diaLocId2,
                       myPartSums, out gotBlocksArg) {
  //writeln("bsComputePartSums  diaSlice=", diaFrom..diaTo,
  //        "  loc[", locId1, ",", locId2, "]  diaLoc [", locId1, ",",
  //        diaLocId2, "]  myPartSums.domain ", myPartSums.domain);
  assert(diaFrom % blkSize == 1, "bsCPS-1");

  // Workaround for asserts not working within 'local'.
  var errs = "";

  // TODO: move 'local' to the caller, once error checks are out
  local {
    // Starts of all the blocks local to this node.
    const startsAllMine = 1..n by blkSize*tl2 align 1+blkSize*locId2;
    // Starts for those to be computed, i.e. to the right from the diagonal.
    const startsToProcess = startsAllMine[diaFrom+blkSize..];

    const diaSlice = diaFrom..diaTo;  // iterate in dim 1
    var gotBlocks = false;

    if checkBsub {
      for (mps, ix) in zip(myPartSums, myPartSums.domain) {
        if mps != 0 then
          errs += (" bsCPS-7 myPartSums[" + ix + "] = " + mps);
      }
    }

    // TODO: some parallelism would be nice here
    // for that, would need myPartSums to have atomic elements
    for pstart in startsToProcess {
      gotBlocks = true;
      const psSlice = pstart..min(pstart+blkSize-1,n);  // iterate in dim 2
      ref pX  = replX._value.dsiLocalSlice1((0, psSlice));
      ref pAB = Ab._value.dsiLocalSlice1((diaSlice, psSlice));
      ensureDR(pX._value, "bsCPS pX");
      ensureDR(pAB._value, "bsCPS pAB");

      if checkBsub {
        if (pstart-1) % (blkSize*tl2) != blkSize * locId2 then
          errs += (" bsCPS-2  pstart " + pstart + "  locId2 " + locId2);
        if pX.domain.dims() != (psSlice,) then
          errs += (" bsCPS-3 "+pstart+": " + (pX.domain.dims():string));
        if pAB.domain.dims() != (diaSlice, psSlice) then
          errs += (" bsCPS-4 "+pstart+": " + (pAB.domain.dims():string));
      }

      // TODO: some parallelism would be nice here
      for i in diaSlice {
        var sum: elemType = 0;
        for j in psSlice do
          sum += pAB[i,j] * pX[j];

        ensureDR(myPartSums._value, "bsCPS myPartSums");
        myPartSums[i-diaFrom] += sum;
      }
    }

    gotBlocksArg = gotBlocks;
  }  // local

  if errs.size != 0 then
    writeln("bsComputePartSums on [", locId1, ",", locId2, "] ERRORS:", errs);

  //writeln("bsComputePartSums  result ", myPartSums,
  //        "  gotBlocks ", gotBlocksArg);
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
  if printStats {
    writeln();
    writeln("The totals are:");
    tPS1iter.printTotal();
    tUBR1iter.printTotal();
    tSC1call.printTotal();
    tLF1iter.printTotal();
    tBScall.printTotal();
    writeln();
  }
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
  forall (ab, init) in zip(Ab, initABalt()) do ab = init;
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

  if onlyBsub {
    ref xRef = backwardSubRef(n);
    const (err, eloc) = maxloc reduce zip(abs(x - xRef), 1..n);
    const mAct = x[eloc], mRef = xRef[eloc];
    const checkOK = (err == 0) ||
                    // the 1e-10 bound is chosen empirically
                    ((err / max(abs(mAct), abs(mRef))) < 1e-10);
    if !checkOK then
      writeln("verifyResults on backwardSub(): error ", err, "  at ", eloc,
            "  got ", mAct, "  expected ", mRef);
    tVer.stop();
    return checkOK;
  }

  // make the rest of the code operate locally without changing it
  ref Ab = makeLocalCopyOfAb();
  
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

  const checkOK = max(resid1, resid2, resid3) < 16.0;
  if (printStats && !checkOK) {
    writeln("resid1: ", resid1);
    writeln("resid2: ", resid2);
    writeln("resid3: ", resid3);
  }

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
        // todo: halt or writeln?
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
    ref myLocalArr = replU._value.localArrs[here.id].arrLocalRep;
    coforall targloc in targetLocales[fromLocId1, ..] do
      if targloc != here then
        replU._value.localArrs[targloc.id].arrLocalRep = myLocalArr;
}

proc targetLocalesIndexForAbIndex(param dim, abIx)
  return (divceilpos(abIx, blkSize) - 1) % (if dim == 1 then tl1 else tl2);

proc targetLocaleForAbIndex(i,j)
  return targetLocales[targetLocalesIndexForAbIndex(1, i),
                       targetLocalesIndexForAbIndex(2, j)];

proc targetLocaleReplForAbIndex(i,j)
  return targetLocalesRepl[targetLocalesIndexForAbIndex(1, i),
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
    const GflopPerSec = ((2.0/3.0) * n**3 + (3.0/2.0) * n**2) / execTime * 1e-9;
    writeln("Performance (Gflop/s) = ", GflopPerSec);
  }
}
proc printTime(execTime) {
  if (printStats) {
    const GflopPerSec = ((2.0/3.0) * n**3 + (3.0/2.0) * n**2) / execTime * 1e-9;
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
// TODO: 'serial' makes the following faster?
    serial { res[i] = (+ reduce [j in xD] (A[i,j] * x[j])) - y[i,n+1]; }

  return res;
}

// This confirms that our intentions, for efficiency, are fulfilled.
proc ensureDR(value, param msg) {
  proc etest(type t) param where t : DefaultRectangularArr return true;
  proc etest(type t) param return false;
  compilerAssert(etest(value.type), "ensureDR ", msg, 2);
}

proc makeLocalCopyOfAb() {
  var localAb: [1..n, 1..n+1] elemType;
  ensureDR(localAb._value, "makeLocalCopyOfAb");
  if numLocales <= 4 {  // heuristic
    localAb = Ab;
    vmsgmore("localized Ab - 1");
  } else {
    forall (g,l) in zip(Ab,localAb) do l = g;
    vmsgmore("localized Ab - 2");
  }
  return localAb;
}
