// Testing various aspects of BlockCyclic specifier.

use DimensionalDist2D, BlockCycDim;
use u;

config const verbose = false;
config const nopfx = false;
no_pfx = nopfx;

config const s1 = 1;
config const s2 = 3;
setupLocales(s1, s2);

var phase = 0;
proc leapphase() { phase += 20; fphase(phase); }

proc test(d) {
  leapphase();

  hd("domain = ", d);
  tl();

  const vd = d._value.dom2;
  if verbose {
    hd("vd = ", vd, "  : ", vd.type:string);
    tl();
  } else {
    nextphase();
  }

  hd("storage");
  for (ix, locDdesc) in zip(d._value.dist.targetIds, d._value.localDdescs) do
    msg(" ", ix, "   ", locDdesc.myStorageDom);
  tl();

  proc t1(param k) {
    hd("dsiAccess1d dim", k);
    const subdom = if k==1 then d._value.dom1 else d._value.dom2;
    for i in d.dim(k) do
      msg(" ", i, "   ", subdom.dsiAccess1d(i));
    tl();
  }

  t1(1);
  t1(2);

  hd("serial iterator over the domain");
  msgserial(d);
  tl();

  hd("parallel iterator over the domain");
  forall i in d do msg(i);
  tl();

  hd("creating an array");
  var a: [d] int;
  tl();

  hd("initializing the array with explicit indexing");
  for ix in d do
    a[ix] = ( ix(1)*1000 + ix(2) ): a.eltType;
  tl();

  hd("serial iterator over the array");
  msgserial(a);
  tl();

  hd("zippered iterator over (domain, array)");
  forall (ix,a) in zip(d,a) do msg(ix, "  ", a);
  tl();

  hd("zippered iterator over (array, domain)");
  forall (a,ix) in zip(a,d) do msg(ix, "  ", a);
  tl();

} // test()

proc testsuite(type T, initphase) {
//compilerWarning("testsuite -- ", T:string, 0);
  phase = initphase; leapphase();
  hd("testsuite(", T:string, ")");
  tl();

  const df8 = new BlockCyclicDim(lowIdx=100, blockSize=7, numLocales=s1);
  const df9 = new BlockCyclicDim(lowIdx=-10, blockSize=5, numLocales=s2);
  const dm = new dmap(new unmanaged DimensionalDist2D(mylocs, df8, df9, "dm", idxType=T));

  proc tw(a, b, c, d) { test({a:T..b:T, c:T..d:T} dmapped dm); }
  inline
  proc t2(a,b) { tw(5,5,a,b); }
  proc t22(a,b,st,al) { test({5:T..5:T, a:T..b:T by st:T align al:T} dmapped dm); }

  t2(7,7);   // 1,0
  t2(12,12); // 1,1
  t2(18,18); // 1,2

  t2(5,9);   // 1,0
  t2(10,14); // 1,1
  t2(15,19); // 1,2

  t2(3,7);   // 0,2 - 1,0

  t2(3,12);  // 0,2 - 1,1
  t2(3,15);  // 0,2 - 1,2
  t2(3,16);  // 0,2 - 1,2
  t2(3,17);  // 0,2 - 1,2
  t2(3,18);  // 0,2 - 1,2
  t2(3,19);  // 0,2 - 1,2

  t2(0,16);  // 0,2 - 1,2
  t2(1,16);  // 0,2 - 1,2
  t2(2,16);  // 0,2 - 1,2
  t2(3,16);  // 0,2 - 1,2
  t2(4,16);  // 0,2 - 1,2

  t22(0,19,3,0);
  t22(0,19,3,1);
  t22(0,19,3,2);

  t22(0,19,5,1);
  t22(0,19,7,1);
  t22(0,19,8,1);
  t22(0,19,9,1);
  t22(0,19,10,1);
  t22(0,19,15,1);
  t22(0,19,18,1);
  t22(0,19,19,1);
  t22(0,19,20,1);

  tw(1,1,  7,7);  // 14,0 | 1,0
  tw(1,1, 12,12); // 14,0 | 1,1
  tw(1,1, 18,18); // 14,0 | 1,2

  tw(5,11, 5,9);
  tw(5,11, 10,14);
  tw(5,11, 16,19);

  test({ 5:T..11:T, 12:T..12:T} dmapped dm);

  proc cmb1(param sgnOnly, r1, r2, m1, m2) {
    if !sgnOnly || isIntType(T) then
      test({r1, r2} dmapped dm);
    else {
      leapphase();
      hd("skipping for unsigned (", m1, ", ", m2, ")");
      tl();
    }
  }

  proc combo(param sgnOnly,
             r1a:range(T), r1b:range(T), r2:range(T),
             m1a:string,   m1b:string,   m2:string)
  {
    cmb1(sgnOnly, r1a, r2      , m1a, m2           );
    cmb1(sgnOnly, r1a, r2 by  1, m1a, m2 + " by  1");
    cmb1(sgnOnly, r1a, r2 by  2, m1a, m2 + " by  2");
    cmb1(sgnOnly, r1a, r2 by  3, m1a, m2 + " by  3");
    cmb1(sgnOnly, r1b, r2 by -1, m1b, m2 + " by -1");
    cmb1(sgnOnly, r1b, r2 by -2, m1b, m2 + " by -2");
    cmb1(sgnOnly, r1b, r2 by -3, m1b, m2 + " by -3");

    cmb1(sgnOnly, r2      , r1a, m2,            m1a);
    cmb1(sgnOnly, r2 by  1, r1a, m2 + " by  1", m1a);
    cmb1(sgnOnly, r2 by  2, r1a, m2 + " by  2", m1a);
    cmb1(sgnOnly, r2 by  3, r1a, m2 + " by  3", m1a);
    cmb1(sgnOnly, r2 by -1, r1b, m2 + " by -1", m1b);
    cmb1(sgnOnly, r2 by -2, r1b, m2 + " by -2", m1b);
    cmb1(sgnOnly, r2 by -3, r1b, m2 + " by -3", m1b);
  }

  const n5=-5, n9=-9; // so we can cast them to unsigned
  combo(false, 1:T..1:T, 1:T..1:T,  3:T..9:T, "1..1", "0..0", "3..9");
  combo(true,  1:T..1:T, 1:T..1:T, n5:T..5:T, "1..1", "0..0", "-5..5");
  combo(true,  1:T..1:T, 1:T..1:T, n9:T..0:T, "1..1", "0..0", "-9..0");

  tw(5,11, 47,42);
  tw(11,5, 42,47);
  tw(11,5, 47,42);
}

testsuite(int(64),  1000);
testsuite(int(32),  3000);
testsuite(uint(64), 5000);
testsuite(uint(32), 7000);
// currently the following won't create domains with idxType=u/int(8) or 16
//testsuite( int(8));
//testsuite(uint(8));
