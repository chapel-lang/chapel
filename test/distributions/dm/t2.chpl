// This tests primarily dsiAccess1d for a block distribution,
// on various domains (positive/negative unit/non-unit strides).

use d, r, u;

config const verbose = false;

config const s1 = 1;
config const s2 = 3;
setupLocales(s1, s2); //, true);

var phase = 1;
proc leapphase() { phase += 10; fphase(phase); }

proc test(d) {
  leapphase();

  hd("testing ", d);
  tl();

  const vd = d._value.dom2;
  if verbose {
    hd("vd = ", vd, "  : ", typeToString(vd.type));
    tl();
  } else {
    nextphase();
  }

  hd("dsiAccess1d");
  for i in d.dim(2) do
    msg(i, " at ", vd.dsiAccess1d(i));
  tl();

  hd("for");
  msgserial(d);
  tl();

  hd("forall");
  forall i in d do msg(i);
  tl();
}

proc testsuite(type T) {
  leapphase();
  hd("testsuite(", typeToString(T), ")");
  tl();

  const vdf = new vdist(1);
  const sdf = new sdist(3, 1:T, 8:T);
  const dm = new dmap(new DimensionalDist(mylocs, vdf, sdf, "dm", idxType=T));

  test([1:T..1:T, 0:T..9:T       ] dmapped dm);
  test([1:T..1:T, 0:T..9:T by -1 ] dmapped dm);
  test([1:T..1:T, 0:T..9:T by -2 ] dmapped dm);
  test([1:T..1:T, 0:T..9:T by  3 ] dmapped dm);
}

testsuite(int);
testsuite(uint);
testsuite(int(64));
testsuite(uint(64));
