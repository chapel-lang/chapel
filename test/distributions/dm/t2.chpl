// This tests primarily dsiAccess1d for a block distribution,
// on various domains (positive/negative unit/non-unit strides).

use d, r, u;

config const verbose = false;

config const s1 = 1;
config const s2 = 3;
setupLocales(s1, s2); //, true);

var vdf = new vdist(1);
var sdf = new sdist(3, 1, 8);
var dm = new dmap(new DimensionalDist(mylocs, vdf, sdf, "dm"));

proc test(d) {
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

  hd("forall");
  forall i in d do msg(i);
  tl();
}

test([1..1, 0..9] dmapped dm);
test([1..1, 0..9 by -1] dmapped dm);
test([1..1, 0..9 by -2] dmapped dm);
test([1..1, 0..9 by 3] dmapped dm);
