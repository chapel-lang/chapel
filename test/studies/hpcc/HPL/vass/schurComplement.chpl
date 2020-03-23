use DimensionalDist2D;
use ReplicatedDim;
use BlockCycDim;
use Memory, Time, Random;


/////////// configuration ///////////

// debugging/reporting flags
config const reproducible = boundsChecking, // auto off upon --fast
             verbose = false, // currently unused
             show_dgemms = false;
config param dgemm_asserts = false,
             do_dgemms = true,
             dummycomps_insteadof_dgemms = false;

type indexType = int,
     elemType = real(64);

// grid dimensions
config const tl1:indexType = tlFromNLocs(1),
             tl2:indexType = tlFromNLocs(2);
var tld: bool;  // whether our targetLocales are all distinct
var tla: [0..#tl1, 0..#tl2] locale = setupTargetLocales();

// automatically compute an acceptable (reqN) or desirable (factorN) size
config const reqN = false,
             factorN = 20;

// problem and block sizes
config const blkSize = if reqN || factorN > 0 then 64 else 8,
             blk = 1, // can add any multiple of blkSize
             n = if reqN then computeRequiredN()
                 else if factorN then computeFactorN()
                 else 63;

// report it
writeln("n ", n, "  blkSize ", blkSize, "  blk ", blk,
        "  locales ", tl1, "*", tl2,
        if dgemm_asserts then "  dgemm_asserts are ON" else "");


/////////// domains and arrays ///////////

// non-distributed index space for Ab
const MatVectSpace = {1..n, 1..n+1};

// block-cyclic starting indices
const st1=1, st2=1;

// 1-d descriptors for Dimensional
const
  bdim1 = new BlockCyclicDim(lowIdx=st1, blockSize=blkSize, numLocales=tl1),
  rdim1 = new ReplicatedDim(tl1),
  bdim2 = new BlockCyclicDim(lowIdx=st2, blockSize=blkSize, numLocales=tl2),
  rdim2 = new ReplicatedDim(tl2);

const dimdist = new dmap(new unmanaged DimensionalDist2D(tla, bdim1, bdim2, "dim"));

// the distributed domain for Ab
const AbD: domain(2, indexType) dmapped dimdist = MatVectSpace;

// temporaries
var Rest: domain(2, indexType) dmapped dimdist; //AbD.dist;
var RestByBlkSize: domain(2, indexType, true) dmapped dimdist; //AbD.dist;

// Ab: the matrix A and vector b
var Ab: [if do_dgemms then AbD else 1..1] elemType; // small if !do_dgemms

// the domains for replication
const
  replAD = {1..n, 1..blkSize} dmapped
    DimensionalDist2D(tla, bdim1, rdim2, "distBR"),
  replBD = {1..blkSize, 1..n+1} dmapped
    DimensionalDist2D(tla, rdim1, bdim2, "distRB");

// the arrays for replication
var replA: [replAD] elemType,
    replB: [replBD] elemType;


/////////// for the reference implementation ///////////

config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.oddCurrentTime else 31415;
config const verify = false;
// can be too much memory
//var Abref: [MatVectSpace] elemType;
var refsuccess = true;


/////////// run it ///////////

const startTime = getCurrentTime();

schurComplement(blk);

const execTime = getCurrentTime() - startTime;  // store the elapsed time
write("DONE");
if reproducible then writeln(); else writeln("  time = ", execTime);


/////////////////////////////////////////////////////////////////////////////

proc schurComplement(blk) {

  //showCurrTime("schurComplement  blk ", blk);


//  const AD   = AbD[blk+blkSize.., blk..#blkSize];
//  const BD   = AbD[blk..#blkSize, blk+blkSize..];

  const AD_dim2 = MatVectSpace.dim(2)(blk..#blkSize);
  const BD_dim1 = MatVectSpace.dim(1)(blk..#blkSize);

  const AbSlice1 = Ab[1..n, AD_dim2],
        AbSlice2 = Ab[BD_dim1, 1..n+1];
  //showCurrTime("AbSlices");

  forall (ab, ra) in zip(AbSlice1, replA) do
    local do
      ra = ab;

  replicateA(blk);

  forall (ab, rb) in zip(AbSlice2, replB) do
    local do
      rb = ab;

  replicateB(blk);

  const RestLocal = MatVectSpace[blk+blkSize.., blk+blkSize..];
  Rest = RestLocal;
  RestByBlkSize = RestLocal by (blkSize, blkSize);
  //showCurrTime("replication; Rest");

  if Rest.size == 0 {
    writeln("schurComplement: nothing to do   Rest = ", Rest);
    return;
  }

// Referring to 'Rest' in dgdriver() seems faster than passing these constants.
//  const RestD1cp = Rest.dim(1),
//        RestD2cp = Rest.dim(2),
//        Rest1end = RestD1cp.last,
//        Rest2end = RestD2cp.last,
//        lastFullRow = Rest1end - blkSize + 1,
//        lastFullCol = Rest2end - blkSize + 1;

  reportBlocks((Rest.dim(1) by blkSize).size, (Rest.dim(2) by blkSize).size);

  forall (row,col) in RestByBlkSize {
    // we have some additional stuff for debugging/tuning

    if show_dgemms then
      writeln((row, col), " on ", here.id);

    if dummycomps_insteadof_dgemms then local {
      const r1 = Rest.dim(1)(row..#blkSize), r2 = Rest.dim(2)(col..#blkSize);
    }

    if do_dgemms then
      dgdriver(row, col);

  } // forall row,col

  //showCurrTime("dgemms");
}

proc dgdriver(row, col) {
  local {

    const
          rng1 = Rest.dim(1)(row..#blkSize),
          rng2 = Rest.dim(2)(col..#blkSize);
//          rng1 = if row <= lastFullRow then row..#blkSize else row..Rest1end,
//          rng2 = if col <= lastFullCol then col..#blkSize else col..Rest2end;

    dgemm(replA.localSlice(rng1, ..),
          replB.localSlice(.., rng2),
          Ab.localSlice(rng1, rng2));
  }

// The original loop:
//      for a in rng1 do
//        for w in 1..blkSize do
//          for b in rng2 do
//            Ab[a,b] -= replA[a,w] * replB[w,b];

}

proc dgemm(LreplA, LreplB, LAb) {
  if dgemm_asserts {
    // This is because these dimensions have the same user indices
    // and are handled by the same distribution 1-d descriptors.
    // So they generate the same (pair-wise) storage indices.
    assert(LAb.domain.dim(1) == LreplA.domain.dim(1));
    assert(LAb.domain.dim(2) == LreplB.domain.dim(2));
    // These dimensions are replicated and their user indices are 1..blkSize.
    assert(LreplA.domain.dim(2) == (1..blkSize));
    assert(LreplB.domain.dim(1) == (1..blkSize));
  }
//  write("replA ", LreplA.domain, "  ");
//  write("replB ", LreplB.domain, "  ");
//  write("Ab    ", LAb.domain, "  ");
//  writeln();

  const r1 = LAb.domain.dim(1),
        r2 = LAb.domain.dim(2),
        rB = 1..blkSize;

  // interchange the j and k loops
  // plus hoist LAb[i,k]
  for i in r1 do
    for k in r2 do {
      var acc: elemType = 0;
      for j in rB do
        acc -= LreplA[i,j] * LreplB[j,k];
      LAb[i,k] += acc;
    }
}


/////////////////////////////////////////////////////////////////////////////

proc replicateA(abIx) {
    const fromLocId2 = targetLocalesIndexForAbIndex(2, abIx);
    coforall lid1 in 0..#tl1 do
      on tla(lid1, fromLocId2) do
        coforall lid2 in 0..#tl2 do
          if lid2 != fromLocId2 then
            replA._value.localAdescs[lid1,lid2].myStorageArr =
              replA._value.localAdescs[lid1,fromLocId2].myStorageArr;
}

proc replicateB(abIx) {
    const fromLocId1 = targetLocalesIndexForAbIndex(1, abIx);
    coforall lid2 in 0..#tl2 do
      on tla(fromLocId1, lid2) do
        coforall lid1 in 0..#tl1 do
          if lid1 != fromLocId1 then
            replB._value.localAdescs[lid1,lid2].myStorageArr =
              replB._value.localAdescs[fromLocId1,lid2].myStorageArr;
}

proc targetLocalesIndexForAbIndex(param dim, abIx)
  return (divceilpos(abIx, blkSize) - 1) % (if dim == 1 then tl1 else tl2);


/////////////////////////////////////////////////////////////////////////////

proc setupTargetLocales() {
  var tla: [0..#tl1, 0..#tl2] locale;
//  writeln("setting up for ", tl1, "*", tl2, " locales");
  tld = numLocales >= tla.size;
  if tld {
    if numLocales > tla.size then
      writeln("UNUSED LOCALES ", numLocales - tla.size);
    for (l,i) in zip(tla,0..) do l = Locales[i];
  } else {
writeln("insufficient locales");
halt();
    writeln("oversubscribing Locales(0)");
    tla = Locales(0);
  }
  return tla;
}

// interpreting numLocales
proc tlFromNLocs(dim) {
  var tl1, tl2: indexType;
  select numLocales {
    when  1 do (tl1,tl2) = (1,1);
    when  2 do (tl1,tl2) = (1,2);
    when  3 do (tl1,tl2) = (1,3);
    when  4 do (tl1,tl2) = (2,2);
    when  8 do (tl1,tl2) = (2,4);
    when  9 do (tl1,tl2) = (3,3);
    when 16 do (tl1,tl2) = (4,4);
    when 18 do (tl1,tl2) = (3,6);
    when 25 do (tl1,tl2) = (5,5);
    when 32 do (tl1,tl2) = (4,8);
    when 50 do (tl1,tl2) = (5,10);
    when 64 do (tl1,tl2) = (8,8);
    when 72 do (tl1,tl2) = (6,12);
//    when  do (tl1,tl2) = ();
    otherwise { tl1 = sqrt(numLocales):indexType; tl2 = tl1; }
  }
  select dim {
    when 1 do return tl1;
    when 2 do return tl2;
    otherwise halt("invalid dim ", dim);
  }
}

proc computeRequiredN() {
  // HPL requirement: 8*n^2 > (total memory on the system) / 2.
  // We compute total memory only over tla's locales, and seek
  //    n > sqrt(totalMem / 16)
  // We also want 'Rest' to fit perfectly in a multiple of tl1*tl2 blocks.
  // So we make nRest a multiple of (tl1*blkSize) and (tl2*blkSize),
  // where Ab's longer dimension (n+1) == (nRest + blkSize).

  const totalMem = + reduce tla.physicalMemory(unit = MemUnits.Bytes);
  const nUnsquaredReqReal = sqrt(totalMem / 16);
  const nUnsquaredReq = floor(nUnsquaredReqReal) : int + 1; // n must be at least this
  const nRestRaw = floor(nUnsquaredReq):int + 1 - blkSize;
  const (nResult, multiple) = computeNfromNRestRaw(nRestRaw, true);

//writeln("n ", nResult, "  nPrev ", nResult - multiple,
//        "  nUnsquaredReqReal ", nUnsquaredReqReal, "  blkSize ", blkSize,
//        "  nlocs ", tla.size);

  // verify we computed it well
  assert(nResult > nUnsquaredReq);
  assert(nResult - multiple <= nUnsquaredReq);

  return nResult;
}

proc computeNfromNRest(nRest) {
  return nRest - 1 + blkSize;
}
proc computeNfromNRestRaw(nRestRaw, param needMultiple = false) {
  const multiple = blkSize * (if tl2 == tl1 || tl2 == 2*tl1 then tl2 else tl1*tl2);
  const nRest = divceil(nRestRaw, multiple) * multiple;
  const nResult = computeNfromNRest(nRest);

//writeln("nRestRaw ", nRestRaw, "  multiple ", multiple, "  nRest ", nRest);
  return if needMultiple then (nResult, multiple) else nResult;
}

proc computeFactorN() {
  // nRest = factor * (tlEach * blkSize),
  // if tlEach==tl1==tl2. More generally,
  // nRest ~= factor * sqrt(tl1*tl2) * blkSize,
  // making nRest a multiple of tl1*tl2*blkSize.
  // nRest = (n+1) - blkSize.

  const nRestRaw = ceil(factorN * sqrt(tl1*tl2) * blkSize) : int;
  return computeNfromNRestRaw(nRestRaw);
}

proc reportBlocks(len1, len2) {
  const pfx  = "",
        rem1 = len1 % tl1,
        rem2 = len2 % tl2,
        remnote = if rem1 == 0 && rem2 == 0 then "" else
          "  rem " + rem1:string + "*" + rem2:string;

  writeln(pfx, len1, "*", len2, " blocks",
          "  factor ", (len1 / tl1) * (len2 / tl2),
          " = ", len1 / tl1, "*", len2 / tl2, remnote);
}
