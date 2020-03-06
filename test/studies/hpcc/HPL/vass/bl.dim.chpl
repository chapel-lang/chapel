// Key:
//MBD   multi-dimensional BlockDist; Dimensional(replicated, block)
//BD    Dimensional(block, block);   Dimensional(replicated, block)
//MBC   multi-dimensional BlockCycDist; Dimensional(replicated, block-cyclic)
//BC    Dimensional(block-cyclic, b-c); Dimensional(replicated, block-cyclic)

// This version: //BD.

//use BlockDist;    //MBD
//use BlockCycDist; //MBC
use DimensionalDist2D;
use ReplicatedDim;
use BlockDim;
//use BlockCycDim; //MBC //BC
use Time, Random;

config param reproducible = false;
config var verbose = true;

proc vwln(args...) { if verbose then writeln((...args)); }
vwln("reproducible=", reproducible, "  verbose=", verbose, "\n");

// indexType can be int or int(64), elemType can be real or complex
// for schurComplement, elemType can be any numeric type, really
type indexType = int,
     elemType = int(64);

// tla - targetLocales array, tl1 * tl2 locales
//
config const tl1 = 2,
             tl2 = 2;
var tld: bool;  // whether our targetLocales are all distinct
var tla: [0..#tl1, 0..#tl2] locale = setupTargetLocales();

config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.oddCurrentTime else 31415;

//
// Configuration constants indicating the problem size (n) and the
// block size (blkSize)
//
config const n = 62,
             blkSize = 8;

// The bounding box for the Block distributions.
// We arbitrarily choose to round up, rather than down.
const nbb1 = divceilpos(n, blkSize * tl1) * blkSize * tl1, //MBD //BD
      nbb2 = divceilpos(n, blkSize * tl2) * blkSize * tl2; //MBD //BD

// The starting indices for the Block-Cyclic distributions.
//const st1=1, st2=1; //MBC //BC

// non-distributed version
const MatVectSpace = {1..n, 1..n+1};

const
  bdim1 =
    new BlockDim(tl1, 1, nbb1), //MBD //BD
//  new BlockCyclicDim(lowIdx=st1, blockSize=blkSize, numLocales=tl1), //MBC //BC
  rdim1 = new ReplicatedDim(tl1),

  bdim2 =
    new BlockDim(tl2, 1, nbb2), //MBD //BD
//  new BlockCyclicDim(lowIdx=st2, blockSize=blkSize, numLocales=tl2), //MBC //BC
  rdim2 = new ReplicatedDim(tl2);

const AbD: domain(2, indexType)
// dmapped Block(boundingBox={1..nbb1, 1..nbb2}, targetLocales=tla) //MBD
// dmapped BlockCyclic(startIdx=(st1,st2), blocksize=(blkSize,blkSize), targetLocales=tla) //MBC
   dmapped DimensionalDist2D(tla, bdim1, bdim2, "dim") //BD //BC
  = MatVectSpace;

var Ab: [AbD] elemType;  // the matrix A and vector b

// for the reference implementation
config const verify = true;
var Abref: [MatVectSpace] elemType;
var refsuccess = true;

// the domains for the arrays used for replication
const
  replAD = {1..n, 1..blkSize} dmapped
    DimensionalDist2D(tla, bdim1, rdim2, "distBR"), //DIM
  replBD = {1..blkSize, 1..n+1} dmapped
    DimensionalDist2D(tla, rdim1, bdim2, "distRB"); //DIM

var replA: [replAD] elemType,
    replB: [replBD] elemType;

writeln("n = ", n, "\n", "blkSize = ", blkSize, "\n", "AbD = ", AbD, "\n",
        "bounding box = ", {1..nbb1, 1..nbb2}, //MBD //BD
      //"starting offsets = ", st1, ", ", st2, //MBC //BC
        "\n");

const startTime = getCurrentTime();     // capture the start time

LUFactorize();

const execTime = getCurrentTime() - startTime;  // store the elapsed time

write("DONE ");
if reproducible {
  writeln();
} else {
  writeln("  time = ", execTime);
}
writeln(
  if verify then
    if refsuccess then "verification OK"
    else               "verification FAILED"
  else "not verified"
);


/////////////////////////////////////////////////////////////////////////////

proc LUFactorize() {

  initABref();
  Ab = Abref;
  doverify(0);

  for blk in 1..n by blkSize {

    // AD is bottom-left; BD is top-right; Rest is bottom-right.
    //
    schurComplement(AD   = AbD[blk+blkSize.., blk..#blkSize],
                    BD   = AbD[blk..#blkSize, blk+blkSize..],
                    Rest = AbD[blk+blkSize.., blk+blkSize..]);

    doverify(blk);
  }
}

proc schurComplement(AD, BD, Rest) {
  vwln("schurComplement(", BD.dim(1).low, ",", AD.dim(2).low, ")",
       //"  [2] ", Rest.low, "\n", "  AD ", AD, "  BD ", BD,
       if BD.dim(1).low < 10 then "  " else "",
       "  Rest ", Rest);

// If Rest is empty, panelSolve and updateBlockRow are still meaningful?
// Otherwise don't invoke schurComplement at all.
  if Rest.numIndices == 0 {
    vwln("  Rest is empty");
    return;
  }

  vwln("  replA", replA.domain, " = Ab", AD, "  ", {1..n, AD.dim(2)});
  vwln("  replB", replB.domain, " = Ab", BD, "  ", {BD.dim(1), 1..n+1});

  // TODO later: only assign from Ab[AD] and Ab[BD], resp.
  // Note: AD.dim(2)  and BD.dim(1) are always blkSize wide;
  // AD.dim(1)==Rest.dim(1) and BD.dim(2)==Rest.dim(2) are not necessarily
  // a multiple of blkSize (but are always non-empty if Rest is non-empty).

  // replicating into replA, replB
  coforall dest in tla[tla.domain.dim(1).high, tla.domain.dim(2)] do
    on dest do
      { vwln("copying to replA on ", here.id);
      replA = Ab[1..n, AD.dim(2)];
      }
  coforall dest in tla[tla.domain.dim(1), tla.domain.dim(2).high] do
    on dest do
      { vwln("copying to replB on ", here.id);
      replB = Ab[BD.dim(1), 1..n+1];
      }

  forall (row,col) in Rest by (blkSize, blkSize) {

    vwln("  dgemm(", (Rest.dim(1))(row..#blkSize), ",",
                     (Rest.dim(2))(col..#blkSize), ")  on ", here.id);

    // This might be an implementation bug, as 'Rest' supports privatization.
    const RestLcl = Rest;

    local {
      for a in (RestLcl.dim(1))(row..#blkSize) do
        for w in 1..blkSize do
          for b in (RestLcl.dim(2))(col..#blkSize) do
            Ab[a,b] -= replA[a,w] * replB[w,b];
    }
  }
}

proc setupTargetLocales() {
  var tla: [0..#tl1, 0..#tl2] locale?;
  writeln("setting up for ", tl1, "*", tl2, " locales");
  tld = numLocales >= tla.numElements;
  if tld {
    var i = 0;
    for l in tla { l = Locales[i]; i += 1; }
  } else {
    vwln("oversubscribing Locales(0)");
    tla = Locales(0);
  }
  vwln("target locales =\n", tla, "\n");
  return tla!;
}

// random initialization
proc initAB(Ab: [] elemType) {
  fillRandom(Ab, seed);
  Ab = Ab * 2.0 - 1.0;
}

proc initABref() {
  if !useRandomSeed || !reproducible then
    writeln("initABref seed = ", seed);
  else if useRandomSeed then
    writeln("initABref is using a random seed");

  if isIntegralType(elemType) {

    var Abtemp: [MatVectSpace] real;
    fillRandom(Abtemp, seed);
    for (r,t) in zip(Abref,Abtemp) do
      r = (t * 2000 - 1000):elemType;

  } else {
    initAB(Abref);
  }
}

proc doverify(blk) {
  if !verify then return;

  if blk != 0 then
    schurComplementRefWrapper(blk);

  const OK = && reduce (Ab == Abref);
  if !OK then refsuccess = false;
  vwln("verification for blk=", blk,
       if OK then " succeeded" else " FAILED", "\n");
}

proc schurComplementRefWrapper(blk:int):void {
    const AbD = MatVectSpace;
    const tl = AbD[blk..#blkSize, blk..#blkSize],
          tr = AbD[blk..#blkSize, blk+blkSize..],
          bl = AbD[blk+blkSize.., blk..#blkSize],
          br = AbD[blk+blkSize.., blk+blkSize..],
          l  = AbD[blk.., blk..#blkSize];

    schurComplementRef(Abref, bl, tr, br);
    // throw it off: Abref[Abref.domain.low] = 0;
}

proc schurComplementRef(Ab: [?AbD] elemType, AD: domain, BD: domain, Rest: domain) {
  const replAD: domain(2, indexType) = AD,
        replBD: domain(2, indexType) = BD;
    
  const replA : [replAD] elemType = Ab[replAD],
        replB : [replBD] elemType = Ab[replBD];

  //  writeln("Rest = ", Rest);
  //  writeln("Rest by blkSize = ", Rest by (blkSize, blkSize));
  // do local matrix-multiply on a block-by-block basis
  forall (row,col) in Rest by (blkSize, blkSize) {
    //
    // At this point, the dgemms should all be local once we have
    // replication correct, so we'll want to assert that fact
    //
    //    local {
      const aBlkD = replAD[row..#blkSize, ..],
            bBlkD = replBD[.., col..#blkSize],
            cBlkD = AbD[row..#blkSize, col..#blkSize];

      dgemmNativeInds(replA[aBlkD], replB[bBlkD], Ab[cBlkD]);
      //    }
  }
}

//
// calculate C = C - A * B.
//
proc dgemmNativeInds(A: [] elemType,
                    B: [] elemType,
                    C: [] elemType) {
  for (iA, iC) in zip(A.domain.dim(1), C.domain.dim(1)) do
    for (jA, iB) in zip(A.domain.dim(2), B.domain.dim(1)) do
      for (jB, jC) in zip(B.domain.dim(2), C.domain.dim(2)) do
        C[iC,jC] -= A[iA, jA] * B[iB, jB];
}
