// test of localSlice for Dimensional with block-cyclic specifiers

use DimensionalDist2D, ReplicatedDim;
use BlockCycDim; //MBC //BC
use Time, Random;

config const verb = false;

type indexType = int, elemType = int(64);

// locales
config const tl1:indexType = sqrt(numLocales):int,
             tl2:indexType = sqrt(numLocales):int;
var tla: [0..#tl1, 0..#tl2] locale;
var tld: bool;  // whether our targetLocales are all distinct

setupTargetLocales();

config const n = 8,
             blkSize = 2;

const st1=1, st2=1;

// non-distributed version
const MatVectSpace = {1..n, 1..n+1};

const
  rdim1 = new ReplicatedDim(tl1),
  bdim1 = new BlockCyclicDim(lowIdx=st1, blockSize=blkSize, numLocales=tl1),
  rdim2 = new ReplicatedDim(tl2),
  bdim2 = new BlockCyclicDim(lowIdx=st2, blockSize=blkSize, numLocales=tl2);

const AbD: domain(2, indexType)
   dmapped DimensionalDist2D(tla, bdim1, bdim2, "dim") //BC
  = MatVectSpace;

var Ab: [AbD] elemType;  // the matrix A and vector b

// for the reference implementation
config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.oddCurrentTime else 31415;
config const verify = false;
var Abref: [MatVectSpace] elemType;
var refsuccess = true;

writeln("n ", n, "  blkSize ", blkSize, "  locales ", tl1, "*", tl2);

// the domains for the arrays used for replication
const
  replAD = {1..n, 1..blkSize} dmapped
    DimensionalDist2D(tla, bdim1, rdim2, "distBR"),
  replBD = {1..blkSize, 1..n+1} dmapped
    DimensionalDist2D(tla, rdim1, bdim2, "distRB");

var replA: [replAD] elemType,
    replB: [replBD] elemType;

// initialize some arrays
for (iloc,l) in zip(tla.domain.dim(2), tla[0,..]) do
  on l do
    forall (iarr, a) in zip(replAD, replA) do
      a = (iloc*1000 + iarr(1) * 10 + iarr(2)) * 1000000;
//
for (iloc,l) in zip(tla.domain.dim(1), tla[..,0]) do
  on l do
    forall (iarr, a) in zip(replBD, replB) do
      a = iloc*1000 + iarr(1)*100 + iarr(2);

if verb then
  for (iloc, l) in zip(tla.domain, tla) do
    on l do
      writeln("on ", l, " ", iloc,
              "\nreplA\n", copyToDF(replA),
              "\nreplB\n", copyToDF(replB), "\n");

var a$: sync int;

forall (row,col) in AbD by blkSize do {
  if row + blkSize - 1 <= n && col + blkSize - 1 <= n + 1 {
    const cur1 = (1..n)(row..#blkSize),
          cur2 = (1..n+1)(col..#blkSize);
    a$ = 1;
    if verb then writeln(cur1, ", ", cur2, "  on ", here.id);
    local {
      test(Ab.localSlice(cur1, cur2),
           replA.localSlice(cur1, 1..blkSize),
           replB.localSlice(1..blkSize, cur2));
    }
    a$;
  } else {
    if verb then writeln((row, col), "  on ", here.id, "  skipped");
  }
}

writeln();
writeln(Ab);
writeln("DONE");

proc test(X, Y, Z) {
  for (x, y, z) in zip(X, Y, Z) do x = y + z;
}

/////////////////////////////////////////////////////////////////////////////

// copy to a default-rectangular array, for writeln()
proc copyToDF(A:[]) {
  var D: domain(A.rank, A.domain.idxType, A.domain.stridable);
  D; D = A.domain; // see issue #14746
  var Res: [D] A.eltType = A;
  return Res;
}

proc setupTargetLocales() {
//  writeln("setting up for ", tl1, "*", tl2, " locales");
  tld = numLocales >= tla.numElements;
  if tld {
    if numLocales > tla.numElements then
      writeln("UNUSED LOCALES ", numLocales - tla.numElements);
    for (l,i) in zip(tla,0..) do l = Locales[i];
  } else {
writeln("insufficient locales");
halt();
    writeln("oversubscribing Locales(0)");
    tla = Locales(0);
  }
}
