// test of slicing for block specifier


use DimensionalDist2D, BlockDim;
use u;

config const
  // num locales
  tl1 = 2,
  tl2 = 2,
  // matrix size
  n = 7,
  // block params
  start1 = 0,
  end1   = n+1,
  start2 = 1,
  end2   = n;

setupLocales(tl1, tl2);

var phase = 0;
proc leapphase() { phase += 20; fphase(phase); }

const dd1 = new BlockDim(numLocales=tl1,
                         boundingBoxLow=start1, boundingBoxHigh=end1);
const dd2 = new BlockDim(numLocales=tl2,
                         boundingBoxLow=start2, boundingBoxHigh=end2);

const space = {1..n, 1..n+1};
const AbD
  : domain(2, int) dmapped DimensionalDist2D(mylocs, dd1, dd2, "dim")
  = space;

var Ab: [AbD] int;

for ((i1,i2),a) in zip(AbD, Ab) do a = i1*10 + i2;

hd("writeln(Ab)");
writeln(Ab);
tl();

proc test(A, ix1, ix2) {
  leapphase();

  hd("testing a slice with ", A.domain);
  tl();

  hd("indexing with ", ix1);
  msg(A[ix1]);
  tl();

  hd("indexing with ", ix2);
  msg(A[ix2]);
  tl();

  hd("serial iterator");
  msgserial(A);
  tl();

  hd("parallel iterator");
  forall a in A do msg(a);
  tl();

  var D: domain(A.rank, A.domain.idxType, A.domain.stridable);
  D = A.domain;

  hd("zippered iterator (A,D)");
  forall (a,i) in zip(A,D) do msg(i, "  ", a);
  tl();

  hd("zippered iterator (D,A)");
  forall (i,a) in zip(D,A) do msg(i, "  ", a);
  tl();
}

test(Ab[1..n, 1..3],           (1,1), (2,2));
test(Ab[1..n by 3, 5..n by 2], (4,5), (7,7));
