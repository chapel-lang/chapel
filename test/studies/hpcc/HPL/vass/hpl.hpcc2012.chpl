//
// Use standard modules for vector and matrix Norms, Random numbers
// and Timing routines
//
use Norm, Random, Time;

//
// Use the user module for computing HPCC problem sizes
//
//use HPCCProblemSize;

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

// Perform verification?
config const verify = true;

// These are solely to make the testing system happy given the COMPOPTS file.
// To be removed once COMPOPTS becomes a non-issue.
config var reproducible = false, verbose = false;

config param maxBlkSize = 200;
if blkSize > maxBlkSize then
  halt("maxBlkSize ", maxBlkSize,
       " is exceeded by the actual blkSize ", blkSize);

// needed for performance
compilerAssert(CHPL_NETWORK_ATOMICS == "none",
               "export CHPL_NETWORK_ATOMICS=none");

//
// The program entry point, currently module initialization
//

  //
  // Compute targetLocales - required for Dimensional.
  // We hard-code 2 dimensions.
  //
  var targetIds: domain(2);
  var targetLocales: [targetIds] locale;
  setupTargetLocalesArray(targetIds, targetLocales, Locales);

  // Here are the dimensions of our grid of locales.
  const tl1 = targetIds.dim(1).size,
        tl2 = targetIds.dim(2).size;

  if tl1 != tl2 then
    halt("backwardSub() is implemented only for a square locale grid");

  printBriefConfiguration();

  const distReplicated = new Replicated();

  // Replicate 'targetLocales' to reduce comm.
  // 'targetLocales' itself could be replicated instead,
  // but this way the changes are smaller.
  const targetIdsRepl = targetIds dmapped new dmap(distReplicated);
  var targetLocalesRepl: [targetIdsRepl] locale;
  coforall (l, id) in zip(Locales, LocaleSpace) do on l {
    targetLocalesRepl._value.localArrs[id].arrLocalRep = targetLocales;
  }

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
  const MVS = {1..n, 1..n+1};
  const AbD = MVS dmapped new dmap(dist1b2b);

  var Ab : [AbD] elemType,           // the matrix A and vector b
      piv: [1..n] indexType;         // a vector of pivot values


  //
  // Create the 1-d replicated arrays for schurComplement().
  //
  const replAD = {1..n, 1..blkSize}   dmapped new dmap(dist1b2r),
        replBD = {1..blkSize, 1..n+1} dmapped new dmap(dist1r2b);

  var replA: [replAD] elemType,
      replB: [replBD] elemType;


  const replKD = {0..0, 1..n+1} dmapped new dmap(dist1r2b);
  var   replK: [replKD] elemType;

  const replUD = {0..#blkSize, 0..#blkSize} dmapped new dmap(distReplicated);
        // can't use targetLocales because of "consistency" req. - that's OK
  var   replU: [replUD] elemType;


  // For panelSolve().
  record psRedResultT {
    var row: indexType;
    var elmx, absmx: elemType;
    proc initResult() { absmx = min(absmx.type); }
    // updateE: // caller to ensure exclusive access
    proc updateE(r: indexType, e: elemType) { updateE(r, e, abs(e)); }
    proc updateE(alt: psRedResultT) { updateE(alt.row, alt.elmx, alt.absmx); }
    proc updateE(r: indexType, e: elemType, a: elemType) {
      if a > absmx { absmx = a; elmx = e; row = r; }
    }
  }
  var psRedLocalResults: [0..#tl1] psRedResultT;

  //
  // Additional data structures for backwardSub().
  //
  type cpuAtomicElemType = // presently unused
    if elemType == real(64) then chpl__processorAtomicType(real)
    else compilerError("need to define cpuAtomicElemType");
  type cpuAtomicCountType = chpl__processorAtomicType(int);
  const replPD = {0..#blkSize} dmapped new dmap(distReplicated);

  // for the working 'x' vector, replX, we'll reuse replK

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

  if printArrays then
    writeln("after initAB, Ab=\n", Ab);

  const startTime = getCurrentTime();     // capture the start time

  LUFactorize(n, piv);                 // compute the LU factorization

  if printArrays then
    writeln("after LUFactorize, Ab=\n", Ab);

  // Note: store result of backwardSub in xtemp instead of
  // returning a slice of a local variable (which is not supported).
  var xTemp = backwardSub(n); // perform the back substitution
  ref x = xTemp[0, 1..n];

  if printArrays then
    writeln("after backwardSub, Ab=\n", Ab, "\nx=\n", x);


  var execTime = getCurrentTime() - startTime;  // store the elapsed time
  if execTime < 0 then execTime += 24*3600;          // adjust for date change
  printTime(execTime);

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

    // MVS was AbD
    const tl = MVS[blk..#blkSize, blk..#blkSize],
          tr = MVS[blk..#blkSize, blk+blkSize..],
          bl = MVS[blk+blkSize.., blk..#blkSize],
          br = MVS[blk+blkSize.., blk+blkSize..],
          l  = MVS[blk.., blk..#blkSize];

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
proc schurComplement(blk, AD, BD, Rest) {

  // Prevent replication of unequal-sized slices
  if Rest.size == 0 then return;

  //
  // Copy data into replicated arrays so every processor has a local copy
  // of the data it will need to perform a local matrix-multiply.
  //
  replicateA(blk, AD.dim(2));
  //
  replicateB(blk, BD.dim(1));

    const low1 = Rest.dim(1).low,
          low2 = Rest.dim(2).low;
    coforall lid1 in 0..#tl1 do
      coforall lid2 in 0..#tl2 do
        on targetLocalesRepl[lid1, lid2] do
          local {
            const myStarts1 = low1..n by blkSize*tl1 align 1+blkSize*lid1;
            const myStarts2 = low2..n+1 by blkSize*tl2 align 1+blkSize*lid2;
            const blkRange = 1..blkSize;
            forall j1 in myStarts1 {
              const outerRange = j1..min(j1+blkSize-1, n);
              ref h2 = replA._value.dsiLocalSlice1((outerRange, blkRange));
              forall j2 in myStarts2 {
                const innerRange = j2..min(j2+blkSize-1, n+1);
                ref h1 = Ab._value.dsiLocalSlice1((outerRange, innerRange)),
                    h3 = replB._value.dsiLocalSlice1((blkRange, innerRange));
                for a in outerRange {
                  for w in blkRange  {
                    const h2aw = h2[a,w];
                    // The code below hoists loop-invariant computations in
                    // dsiAccess() out of the 'for innerRange' loop by hand.
                    // We are using tuples instead of objects because
                    // the generated code is more efficient, which is
                    // also a needed compiler optimization.
                    for b in innerRange do
                      h1[a,b] -= h2aw * h3[w,b];
                  } // for w
                } // for a
              } // forall j2
            } // forall j1
          } // local
}

/////////////////////////////////

// NB works only when storage indices for sliceDim1,2 are contiguous
// Each sliceDim1,2 can be either a range or an int

pragma "no copy return"
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
             then if origScalar(1) then sliceDim1 else dom.whole.dim(1)(sliceDim1)
             else toOrig(1, locAdesc.locDom.doml1.dsiLocalSliceStorageIndices1d(dom.dom1, toRange(sliceDim1))),
        r2 = if dom.dom2.dsiStorageUsesUserIndices()
             then if origScalar(2) then sliceDim2 else dom.whole.dim(2)(sliceDim2)
             else toOrig(2, locAdesc.locDom.doml2.dsiLocalSliceStorageIndices1d(dom.dom2, toRange(sliceDim2)));

  const reindexExpr =
    if origScalar(1) then
      if origScalar(2) then compilerError("dsiLocalSlice1 is not supported for two scalars")
      else (sliceDim2,)
    else
      if origScalar(2) then (sliceDim1,)
      else (sliceDim1, sliceDim2);

  return locAdesc.myStorageArr[r1, r2].reindex({(...reindexExpr)});
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
  const dim2 = panel.dim(2);
  const cornerLocale = targetLocaleCorner(blk);
  var tStart, tReduce, tSwap, tRepl, tComp: real;
  var tSwapMsg: string;

  for k in dim2[..n] {             // iterate through the columns

/* psReduce() does the following:

    const col = panel[k.., k..k];
    
    // Find the pivot, the element with the largest absolute value.
    const (_, (pivotRow, _)) = maxloc reduce zip(abs(Ab(col)), col);

    // Capture the pivot value explicitly (note that result of maxloc
    // is absolute value, so it can't be used directly).
    // k <= pivotRow <= n
    //
    const pivotVal = Ab[pivotRow, k];
*/
    const (pivotRow, pivotVal) = psReduce(blk, k);
    if (pivotVal == 0) then
      halt("Matrix cannot be factorized");

    // Swap the current row with the pivot row and update the pivot vector
    // to reflect that
   if k == pivotRow {
    // nothing
   } else {
    var wasLocal: bool; // fyi only
    //Ab[k..k, ..] <=> Ab[pivotRow..pivotRow, ..];
    psSwap(k, pivotRow, wasLocal);
    piv[k] <=> piv[pivotRow];
   }

/* replicate + psCompute() do the following:

    // divide all values below and in the same col as the pivot by
    // the pivot value
    Ab[k+1.., k..k] /= pivotVal;
    
    // update all other values below the pivot
    forall (i,j) in panel[k+1.., k+1..] do
      Ab[i,j] -= Ab[i,k] * Ab[k,j];
*/

    // replicate
    on cornerLocale {
      local do
        for j in dim2[k+1..] do
          replK[0,j] = Ab[k,j];
      replicateK(blk);
    }

    // compute
    psCompute(panel, blk, k, pivotVal);
  }
}

proc psReduce(blk, k) {
  const lid2 = targetLocalesIndexForAbIndex(2, k);
  const rlrLoc = psRedLocalResults.locale;

  coforall lid1 in 0..#tl1 {
    on targetLocalesRepl[lid1, lid2] {
      var locResult: psRedResultT;
      locResult.initResult();

      local {
        // guard updates to locResult and maxRes within the forall
        var upd$: sync bool;
        var maxRes: atomic real;
        maxRes.write(min(real));
        // Starts of all the blocks in the panel that are local to this node.
        const panelStarts = blk..n by blkSize*tl1 align 1+blkSize*lid1;
          forall iStart in panelStarts with (ref locResult) {
            const iRange = max(iStart, k)..min(iStart + blkSize - 1, n);
            var myResult: psRedResultT;
            myResult.initResult();
            ref locAB = Ab._value.dsiLocalSlice1((iRange, k));
            for i in iRange do myResult.updateE(i, locAB[i]);
            if myResult.absmx > maxRes.read() { // only lock if we might update
              upd$ = true;  // lock
              if myResult.absmx > maxRes.read() { // check again while locked
                locResult.updateE(myResult);
                maxRes.write(myResult.absmx);
              }
              upd$;  // unlock
            }
          } // forall
      } // local

      const locConst = locResult;  // for value forwarding
      on rlrLoc do psRedLocalResults[lid1] = locConst;

    } // on
  } // coforall

  // Merge the results from psRedLocalResults.
  var pivotAll: psRedResultT;
  pivotAll.initResult();
  for rlr in psRedLocalResults do pivotAll.updateE(rlr);

  return (pivotAll.row, pivotAll.elmx);
}

proc psCompute(panel, blk, k, pivotVal) {
  if k == n then return; // nothing to do
  const dim2end = panel.dim(2).alignedHigh;
  const lid2 = targetLocalesIndexForAbIndex(2, k);

  coforall lid1 in 0..#tl1 {
    on targetLocalesRepl[lid1, lid2] {

      // Starts of all the blocks local to this node.
      const allStarts = 1..n by blkSize*tl1 align 1+blkSize*lid1;
      // Starts of those in the panel. Exclude the first block
      // if there is nothing to do there. k==n check is done earlier.
      const emptyFirstBlock = (k == blk + blkSize - 1);
      const firstNonemptyBlock = blk + (if emptyFirstBlock then blkSize else 0);
      const panelStarts = allStarts[firstNonemptyBlock..];

      forall iStartRaw in panelStarts {
          // If this is the top block of the panel, start at Row k+1
          const iStart = max(iStartRaw, k+1),
                iEnd   = min(iStartRaw + blkSize - 1, n);

        // TODO: move this 'local' right after 'on'
        local {
          ref locAB = Ab._value.dsiLocalSlice1((iStart..iEnd, k..dim2end));
          ref locK = replK._value.dsiLocalSlice1((0, k..dim2end));
          for i in iStart..iEnd {
            // Ab[k+1.., k..k] /= pivotVal;
            locAB[i, k] /= pivotVal;

            //forall (i,j) in panel[k+1.., k+1..] do
            //  Ab[i,j] -= Ab[i,k] * Ab[k,j];
            //
            for j in k+1..dim2end do
              locAB[i,j] -= locAB[i,k] * locK[j];
          } // for i
        } // local
      } // forall iStartRaw
    } // on
  } // coforall lid1
} // psCompute()

proc psSwap(k, pr, out wasLocal) {
  const lidk1 = targetLocalesIndexForAbIndex(1, k),
        lidpr1 = targetLocalesIndexForAbIndex(1, pr);
  wasLocal = lidk1 == lidpr1;

  coforall lid2 in 0..#tl2 {
    on targetLocalesRepl[lidk1, lid2] {
      // Starts of all the blocks local to this node.
      const myStarts = 1..n+1 by blkSize*tl2 align 1+blkSize*lid2;

      forall js in myStarts {
        const mycol = js..min(js+blkSize-1,n+1);
        // Ab[k,mycol] <=> Ab[pr,mycol]

        if lidk1 == lidpr1 {
          // sweet, can swap locally
          local {
            ref locABk = Ab._value.dsiLocalSlice1((k, mycol));
            ref locABpr = Ab._value.dsiLocalSlice1((pr, mycol));
            for j in mycol do locABk[j] <=> locABpr[j];
          }
        } else {
          // need to copy the two sub-rows between two locales
          ref locABk = Ab._value.dsiLocalSlice1((k, mycol));

          // using a tuple for more efficient data transfer
          var tt: maxBlkSize*elemType;
          for j in mycol do tt[j-js+1] = locABk[j];

          // or:
          //var locKk  => K._value.dsiLocalSlice1((0, mycol));
          //for j in mycol do locKk[j] = locABk;

          on targetLocalesRepl[lidpr1, lid2] {
            var pp = tt;
            local {
              const myStart = mycol.low;
              ref locABpr = Ab._value.dsiLocalSlice1((pr, mycol));
              for j in mycol do pp[j-myStart+1] <=> locABpr[j];
            }
            tt = pp;

            // or: replK is available for temp storate
            //var locKpr  => replK._value.dsiLocalSlice1((0, mycol));
            //locKpr = locABk; // TODO: bulkify, unless it is already
            //for j in mycol do locABpr[j] <=> locKpr[j];
            //locABk = locKpr; // TODO: bulkify, unless it is already

          } // on lidpr1

          for j in mycol do locABk[j] = tt[j-js+1];

        } // if lidk1 == lidpr1
      } // forall js
    } // on
  } // coforall lid2
} // psSwap()

/////////////////////////////////////////////////////////////////////////////

//
// Update the block row (tr for top-right) portion of the matrix in a
// blocked LU decomposition.  Each step of the LU decomposition will
// solve a block (tl for top-left) portion of a matrix. This function
// solves the rows to the right of the block.
//
proc updateBlockRow(
                   tl: domain,
                   tr: domain) {
  // Note: the last call to updateBlockRow may have shorter dim1.
  if boundsChecking then assert(tl.dim(1) == tr.dim(1));
  const dim1 = tl.dim(1);
  const dim2 = tl.dim(2);
  const blk = dim1.low;
  const cornerLocale = targetLocaleCorner(blk);

  // 'tl' is a Dimensional-mapped domain, so has stuff
  // on all locales, even where the index set is empty
  on cornerLocale {
    local do
      // we do not need the last row of tl, but pruning it off seems expensive
      forall (i,j) in {dim1, dim2} do
        replU[i-blk, j-blk] = Ab[i,j];
    replicateU(blk);
  }

  const blkStarts = tr.dim(2)[.. by blkSize align 1];
  const lid1 = targetLocalesIndexForAbIndex(1, blk);
  const blkStartsStart = blkStarts.alignedLow;

  coforall lid2 in 0..#tl2 {
   on targetLocalesRepl[lid1, lid2] {
    const myStarts = blkStartsStart..n+1 by blkSize*tl2 align 1+blkSize*lid2;
    forall js in myStarts {

    const dim1local = dim1; // needed for 'local'?

    local {
      const dim2 = js..min(js+blkSize-1,n+1);
      ref locAB  = Ab._value.dsiLocalSlice1((dim1, dim2));
      ref locU   = replU._value.localArrs[here.id].arrLocalRep;

      for row in dim1 {
        const i = row, iRel = i-blk;
        for j in dim2 do
          for k in blk..row-1 do
            //Ab[i, j] -= Ab[i, k] * Ab[k,j];
            //Ab[i, j] -= replU[i-blk, k-blk] * Ab[k,j];
            locAB[i, j] -= locU[iRel, k-blk] * locAB[k, j];
      } // for row
    }  // local

    } // forall
   } // on
  } // coforall
}

/////////////////////////////////////////////////////////////////////////////

//
// compute the backwards substitution
//
inline proc bsFinalizeXi(i, ref x_i, partSum, Ab_i_i, Ab_i_n1) {
  x_i = (Ab_i_n1 - (x_i + partSum)) / Ab_i_i;
}

//
// compute the backwards substitution, the new way
//
proc backwardSub(n: indexType) {
  if tl1 != tl2 then
    halt("backwardSub() is implemented only for a square locale grid");

  const diaFrom_N = (divceilpos(n, blkSize)-1) * blkSize + 1,
        diaTo_N   = n,
        locId1_N  = targetLocalesIndexForAbIndex(1, diaFrom_N),
        locId2_N  = targetLocalesIndexForAbIndex(2, diaFrom_N);

  on targetLocales[locId1_N, locId2_N] {

    // the key computation for the last (lower-right) diagonal block
    // no data dependencies - just compute X
    // zeros out its block of replK at start
    bsComputeMyXs(diaFrom_N, diaTo_N, locId1_N, locId2_N, true);

    // TODO: all except the block right above can be done in background
    replicateK(diaFrom_N);  // replicates replK
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

  // gather into a DR array
  // What's copied has been updated appropriately
  // because we do replication synchronously.
  var xTemp: [(...replK.domain.dims())] elemType = replK;

  // The 'forall' below, as an alternative to 'x = replK', would
  // iterate over *all* locales, which is too much and also causes
  // the error 'zippered iterations have non-equal lengths'.
  //forall (repl,locl) in zip(replK,x) do locl = repl;

  return xTemp;
}

proc bsComputeRow(diaFrom, diaTo, locId1, locId2, diaLocId2) {
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

    // this zeros out its block of replK at start
    bsIncorporateOthersPartSums(diaFrom, diaTo, locId1, diaLocId2);

    bsComputeMyXs(diaFrom, diaTo, locId1, locId2, false);

    // TODO: all except the block right above can be done in background
    replicateK(diaFrom);  // replicates replK

    // Reset partial values for future use.
    local do for ps in myPartSums do ps = 0;

  } else {
    // off the diagonal

    ref myPartSums = bsLocalPartSums._value.localArrs[here.id].arrLocalRep;
    ensureDR(myPartSums._value, "bsR myPartSums");

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
  ref locX = replK._value.dsiLocalSlice1((0, diaSlice));

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
      // TODO: bulkify, unless it is already
      locB = Ab._value.dsiLocalSlice1((diaSlice, n+1));
    }
    local do
      bsComputeMyXsWithB(diaFrom, diaTo, locAB, locX, locB);
  }
}

proc bsComputeMyXsWithB(diaFrom, diaTo, locAB, locX, locB) {
  // the i1 loop, as written, must be serial
  for i in diaFrom..diaTo by -1 {
    var sum: elemType = 0;
    for j in i+1..diaTo do
      sum += locAB[i,j] * locX[j];
    bsFinalizeXi(i, locX[i], sum, locAB[i,i], locB[i]);
  }
}

proc bsIncorporateOthersPartSums(diaFrom, diaTo, locId1, locId2) {
  // Apart from asserts and writeln(), everything here should be local.
  ref partSums = bsOthersPartSums._value.localArrs[here.id].arrLocalRep;
  ref locX     = replK._value.dsiLocalSlice1((0, diaFrom..diaTo));
  ensureDR(partSums._value, "bsI partSums");
  ensureDR(locX._value, "bsI locX");

  // because we are reusing replK for replX
  for x in locX do x = 0;

  var toIncorporate = tl2;
  var seenOther = false;

  inline proc ihelper(ref ps: bsSetofPartSums, locId2: int) {
    const avail = ps.avail.read();
    if avail == spsNA then return;
    // Indicate that we got it.
    ps.avail.write(spsNA);
    toIncorporate -= 1;
    seenOther = true;

    if avail == spsAV {
      // yes incorporate
      for i in 0..#blkSize do locX[diaFrom+i] += ps.values[i];
    } else {
      // nothing to incorporate
    }
  } // proc ihelper

  local do
    while toIncorporate > 0 {
      seenOther = false;
      // since incorporation in ihelper
      for l2 in 0..#tl2 do ihelper(partSums[l2], l2);
      if !seenOther then chpl_task_yield();
    }

}  // bsIncorporateOthersPartSums

proc bsSendPartSums(diaFrom, diaTo, locId1, locId2, diaLocId2,
                    myPartSums, gotBlocks) {
  on targetLocalesRepl[locId1, diaLocId2] {
    sendhelper(bsOthersPartSums[locId2]);

    proc sendhelper(ref ps: bsSetofPartSums) {
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

  // TODO: move 'local' to the caller, once error checks are out
  local {
    // Starts of all the blocks local to this node.
    const startsAllMine = 1..n by blkSize*tl2 align 1+blkSize*locId2;
    // Starts for those to be computed, i.e. to the right from the diagonal.
    const startsToProcess = startsAllMine[diaFrom+blkSize..];

    const diaSlice = diaFrom..diaTo;  // iterate in dim 1
    var gotBlocks = false;

    // TODO: some parallelism would be nice here
    // for that, would need myPartSums to have atomic elements
    for pstart in startsToProcess {
      gotBlocks = true;
      const psSlice = pstart..min(pstart+blkSize-1,n);  // iterate in dim 2
      ref pX  = replK._value.dsiLocalSlice1((0, psSlice));
      ref pAB = Ab._value.dsiLocalSlice1((diaSlice, psSlice));
      ensureDR(pX._value, "bsCPS pX");
      ensureDR(pAB._value, "bsCPS pAB");

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

//   
// construct an n by n+1 matrix filled with random values and scale
// it to be in the range -1.0..1.0
//
proc initAB() {
  fillRandom(Ab, seed);
  forall ab in Ab do local { ab = ab * 2.0 - 1.0; }  //Ab = Ab * 2.0 - 1.0;
}

//
// calculate norms and residuals to verify the results
//
proc verifyResults(x) {
  if !verify then return true;
  if printStats then writeln("verifying results locally");
  initAB();

  // make the rest of the code operate locally without changing it
  ref Ab = makeLocalCopyOfAb();
  
  const axmbNorm = norm(gaxpyMinus(Ab[.., 1..n], x, Ab[.., n+1..n+1]), normType.normInf);

  const a1norm   = norm(Ab[.., 1..n], normType.norm1),
        aInfNorm = norm(Ab[.., 1..n], normType.normInf),
        x1Norm   = norm(Ab[.., n+1..n+1], normType.norm1),
        xInfNorm = norm(Ab[.., n+1..n+1], normType.normInf);

  const resid1 = axmbNorm / (epsilon * a1norm * n),
        resid2 = axmbNorm / (epsilon * a1norm * x1Norm),
        resid3 = axmbNorm / (epsilon * aInfNorm * xInfNorm);

  const checkOK = max(resid1, resid2, resid3) < 16.0;
  if (printStats && !checkOK) {
    writeln("resid1: ", resid1);
    writeln("resid2: ", resid2);
    writeln("resid3: ", resid3);
  }

  return checkOK;
}

/////////////////////////////////////////////////////////////////////////////

proc replicateA(abIx, dim2) {
    const fromLocId2 = targetLocalesIndexForAbIndex(2, abIx);
    coforall lid1 in 0..#tl1 do
      on targetLocales(lid1, fromLocId2) {

        ref locReplA =
          replA._value.localAdescs[lid1,fromLocId2].myStorageArr;

        // (A) copy from the local portion of A[1..n, dim2] into replA[..,..]
        /*local*/ {
          const myStarts = 1..n by blkSize*tl1 align 1+blkSize*lid1;

          forall iStart in myStarts {
            const iEnd = min(iStart + blkSize - 1, n),
                  iRange = iStart..iEnd;
            ref locAB = Ab._value.dsiLocalSlice1((iRange, dim2));

            // locReplA[iRange,..] = locAB[iRange, dim2];
            const jStart = dim2.alignedLow,
                  rStart = replA._value.dom.dom1._dsiStorageIdx(iStart);
            for i in iRange {
              for j in dim2 do
                locReplA[i - iStart + rStart, j - jStart + 1] = locAB[i,j];
            } // for i

          } // forall iStart
        } // local

        // (B) replicate that
        coforall lid2 in 0..#tl2 do
          if lid2 != fromLocId2 then
            replA._value.localAdescs[lid1,lid2].myStorageArr = locReplA;
      } // on
}

proc replicateB(abIx, dim1) {
    const fromLocId1 = targetLocalesIndexForAbIndex(1, abIx);
    coforall lid2 in 0..#tl2 do
      on targetLocales(fromLocId1, lid2) {

        ref locReplB =
          replB._value.localAdescs[fromLocId1,lid2].myStorageArr;

        // (A) copy from the local portion of A[dim1, 1..n+1] into replB[..,..]
        /*local*/ {
          const myStarts = 1..n+1 by blkSize*tl2 align 1+blkSize*lid2;

          forall jStart in myStarts {
            const jEnd = min(jStart + blkSize - 1, n+1),
                  jRange = jStart..jEnd;
            ref locAB = Ab._value.dsiLocalSlice1((dim1, jRange));

            // locReplB[..,jRange] = locAB[dim1, jRange];
            const iStart = dim1.alignedLow,
                  rStart = replB._value.dom.dom2._dsiStorageIdx(jStart);
            for i in dim1 {
              for j in jRange do
                locReplB[i - iStart + 1, j - jStart + rStart] = locAB[i,j];
            } // for i

          } // forall jStart
        } // local

        // (B) replicate that
        coforall lid1 in 0..#tl1 do
          if lid1 != fromLocId1 then
            replB._value.localAdescs[lid1,lid2].myStorageArr = locReplB;
      } // on
}

// replicate only along one column of processors,
// starting on a good locale
proc replicateK(abIx) {
    const fromLocId1 = targetLocalesIndexForAbIndex(1, abIx);
    const fromLocId2 = targetLocalesIndexForAbIndex(2, abIx);
    // verify that we are on a good locale, optionally
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
  return cornerLocale;
}

/////////////////////////////////////////////////////////////////////////////

//
// print success/failure, the execution time and the Gflop/s value
//
proc printResults(successful, execTime) {
  if !verify then writeln("Validation: skipped"); else
  if successful then writeln("Validation: success");
  else               writeln("Validation: FAILURE");
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
    printBriefConfiguration(); // for convenience
  }
}
proc printBriefConfiguration() {
  writeln("HPL  n ", n, "  blkSize ", blkSize, "  blk/node ",
          divceilpos(n, blkSize*tl1), "*", divceilpos(n+1, blkSize*tl2),
          "  locs ", tl1, "*", tl2, "  dPTPL ", dataParTasksPerLocale);
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
  /*
  proc etest(type t) param where t : DefaultRectangularArr return true;
  proc etest(type t) param return false;
  compilerAssert(etest(chpl__getActualArray(value).type), "ensureDR ", msg, 2);
  */
}

proc makeLocalCopyOfAb() {
  var localAb: [1..n, 1..n+1] elemType;
  ensureDR(localAb._value, "makeLocalCopyOfAb");
  if numLocales <= 4 {  // heuristic
    localAb = Ab;
  } else {
    forall (g,l) in zip(Ab,localAb) do l = g;
  }
  return localAb;
}
