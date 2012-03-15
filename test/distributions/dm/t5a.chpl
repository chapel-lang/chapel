use d, r, f, u;

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
      const dm = new dmap(new DimensionalDist(mylocs, dd1, dd2, "dm"));

      const Dbase = [1..3, 1..5];
      const Ddm = Dbase dmapped dm;

      var A: [Ddm] int = [(i,j) in Dbase] i*10+j;
      hd("A: base"); msgserial(A); tl();

      var B: [Ddm by (2,2)] int = [(i,j) in Dbase by (2,2)] i*100+j;
      hd("B: by (2,2)"); msgserial(B); tl();

// NOTE: .good for this reflect the current bug in domain leader iterator.
// C should have 2002, 2003, 2004.
// See test/domains/vass/aligned-domain-forall.chpl
      const rc1 = 0..5 by 2, rc2 = 2..4;
      var C: [Ddm(rc1,rc2)] int = [(i,j) in Dbase(rc1,rc2)] i*1000+j;
      hd("C: slice", (rc1,rc2)); msgserial(C); tl();

// NOTE: likewise, D should have 32, 35.
      const rd1 = 0..6 by 3, rd2 = 0..6 by 3 align 2;
      var D: [Ddm(rd1,rd2)] int = [(i,j) in Dbase(rd1,rd2)] i*10+j;
      hd("D: aligned ", (rd1,rd2)); msgserial(D); tl();
//    hd("testing"); msg(Dbase(rd1,rd2)); msgserial(Dbase(rd1,rd2)); tl();

  } else {
    hd("not selected"); tl();
  }
}

test("vdist,vdist", 1, new vdist(s1), new vdist(s2));
test("sdist,sdist", 2, new sdist(s1, 1, 4), new sdist(s2, 1, 4));
test("idist,idist", 3, new idist(s1, 0, 2), new idist(s2, 2, 3));

test("vdist,sdist", 4, new vdist(s1),       new sdist(s2, 1, 4));
test("vdist,idist", 5, new vdist(s1),       new idist(s2, 2, 3));
test("sdist,vdist", 6, new sdist(s1, 1, 4), new vdist(s2));
test("sdist,idist", 7, new sdist(s1, 1, 4), new idist(s2, 2, 3));
test("idist,vdist", 8, new idist(s1, 0, 2), new vdist(s2));
test("idist,sdist", 9, new idist(s1, 0, 2), new sdist(s2, 1, 4));
