use DimensionalDist2D, ReplicatedDim, BlockDim, BlockCycDim;
use u;

config const s1 = 2;
config const s2 = 2;
setupLocales(s1, s2);

var phase = 0;
proc leapphase() { phase += 20; fphase(phase); }

config const selector = 0;

proc test(message, sel, dd1, dd2) {
  leapphase();

  // ignore 'message'
  hd(dd1.toString(), " + ", dd2.toString(), "  sel=", sel); tl();

  if selector == 0 || selector == sel {
      const dm = new dmap(new DimensionalDist2D(mylocs, dd1, dd2, "dm"));

      const Dbase = {1..3, 1..5};
      const Ddm = Dbase dmapped dm;

      var A: [Ddm] int = [(i,j) in Dbase] i*10+j;
      hd("A: base"); msgserial(A); tl();

      var B: [Ddm by (2,2)] int = [(i,j) in Dbase by (2,2)] i*100+j;
      hd("B: by (2,2)"); msgserial(B); tl();

      const rc1 = 0..5 by 2, rc2 = 2..4;
      var C: [Ddm(rc1,rc2)] int = [(i,j) in Dbase(rc1,rc2)] i*1000+j;
      hd("C: slice", (rc1,rc2)); msgserial(C); tl();

      const rd1 = 0..6 by 3, rd2 = 0..6 by 3 align 2;
      var D: [Ddm(rd1,rd2)] int = [(i,j) in Dbase(rd1,rd2)] i*10+j;
      hd("D: aligned ", (rd1,rd2)); msgserial(D); tl();
//    hd("testing"); msg(Dbase(rd1,rd2)); msgserial(Dbase(rd1,rd2)); tl();

  } else {
    hd("not selected"); tl();
  }
}

test("ReplicatedDim,ReplicatedDim", 1, new ReplicatedDim(s1), new ReplicatedDim(s2));
test("BlockDim,BlockDim", 2, new BlockDim(s1, 1, 4), new BlockDim(s2, 1, 4));
test("BlockCyclicDim,BlockCyclicDim", 3, new BlockCyclicDim(s1, 0, 2), new BlockCyclicDim(s2, 2, 3));

test("ReplicatedDim,BlockDim", 4, new ReplicatedDim(s1),       new BlockDim(s2, 1, 4));
test("ReplicatedDim,BlockCyclicDim", 5, new ReplicatedDim(s1), new BlockCyclicDim(s2, 2, 3));
test("BlockDim,ReplicatedDim", 6, new BlockDim(s1, 1, 4), new ReplicatedDim(s2));
test("BlockDim,BlockCyclicDim", 7, new BlockDim(s1, 1, 4), new BlockCyclicDim(s2, 2, 3));
test("BlockCyclicDim,ReplicatedDim", 8, new BlockCyclicDim(s1, 0, 2), new ReplicatedDim(s2));
test("BlockCyclicDim,BlockDim", 9, new BlockCyclicDim(s1, 0, 2), new BlockDim(s2, 1, 4));
