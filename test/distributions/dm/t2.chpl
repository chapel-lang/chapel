// This tests primarily dsiAccess1d for a block distribution,
// on various domains (positive/negative unit/non-unit strides).

use DimensionalDist2D, ReplicatedDim, BlockDim;
use u;

config const verbose = false;
config const nopfx = false;
no_pfx = nopfx;

config const s1 = 1;
config const s2 = 3;
setupLocales(s1, s2);

var phase = 0;
proc leapphase() { phase += 10; fphase(phase); }

proc test(d) {
  leapphase();

  hd("testing ", d);
  tl();

  const vd = d._value.dom2;
  if verbose {
    hd("vd = ", vd, "  : ", vd.type:string);
    tl();
  } else {
    nextphase();
  }

  hd("dsiAccess1d");
  for i in d.dim(1) do
    msg(i, " at ", vd.dsiAccess1d(i));
  tl();

  hd("for");
  msgserial(d);
  tl();

  hd("forall");
  forall i in d do msg(i);
  tl();
}

proc testsuite(type T, initphase) {
  phase = initphase; leapphase();
  hd("testsuite(", T:string, ")");
  tl();

  const vdf = new ReplicatedDim(1);
  const sdf = new BlockDim(3, 1:T, 8:T);
  const dm = new dmap(new unmanaged DimensionalDist2D(mylocs, vdf, sdf, "dm", idxType=T));

  test({1:T..1:T, 0:T..9:T       } dmapped dm);
  test({1:T..1:T, 1:T..9:T by -1 } dmapped dm);
  test({1:T..1:T, 2:T..9:T by -2 } dmapped dm);
  test({1:T..1:T, 0:T..9:T by  3 } dmapped dm);
  test({1:T..1:T, 0:T..9:T by  2 } dmapped dm);
  test({1:T..1:T, 3:T..9:T by -3 } dmapped dm);
}

testsuite(int,        0);
testsuite(uint,     200);
testsuite(int(64),  400);
testsuite(uint(64), 600);
