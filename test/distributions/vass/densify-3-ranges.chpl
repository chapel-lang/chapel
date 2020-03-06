// densifyBounded() was the code name for densify()
proc densifyBounded(w,r) {
  const result = densify(r,w);
  testUndensify(w,r,result);
  return result;
}

proc testUndensify(whole, r, dense) {
  const undense = unDensify(dense, whole);
  if undense != r then
    writeln("testUndensify() error: ", (whole, r, dense, undense));
}

config var verbose = false;
var numVerifies = 0;

for /*param*/ len in 1..5 {
  for /*param*/ sa in 1..4 {
    for /*param*/ ssign in -1..1 by -2 {

      const low = 10;
      const high = low + (len - 1) * sa;
      const stride = sa * ssign;
      if stride == 1 || len <= 1 {
        if ssign == -1 then
          runSuite(len, sa, ssign, low .. high);  // without 'by 1'
        runSuite(len, sa, ssign, low .. high by stride);
      } else for /*param*/ al in 0..(sa-1) {
          if al == 0 then
            runSuite(len, sa, ssign, low .. high by stride);
          else
            runSuite(len, sa, ssign, low .. (high + sa -1) by stride align al);
        } // if stride for al
    } // for ssign
  } // for sa
} // for len

proc runSuite(/*param*/ len:int, /*param*/ sa:int, /*param*/ ssign:int, w: range(?)) {
  proc verify(r) { verify(w, r); }

  //writeln("runSuite ", (len,sa,ssign, w));

  // test self
  verify(w);
  verify(w by -1);
  verify(1..0);
  verify(1..0 by -1);

  assert(len == w.size, "runSuite", (len, sa, ssign, w), ": w.size=", w.size, " (does not match 'len')");
  const a: [1..len] w.idxType;

  // workaround for bug in implementing 'a = w;'
  for (aa,ww) in zip(a,w) do aa = ww;

  // test with r.size == 1
  if len <= 1 then return;

  for aa in a {
    verify(aa..aa);
    verify(aa..aa by -1);
  }

  // test with r.size == 2
  if len <= 2 then return;

  for i1 in 1..len-1 {
    for i2 in i1+1..len {
      var low = a(i1), high = a(i2);
      if low > high then low <=> high;
      verify(low .. high by (high - low));
      verify(low .. high by -(high - low));
    }
  }

  // test with sub-sequences

  for substride in 2 .. len {
    verify(w by substride);
    verify(w by -substride);
  }

  // test with r.size == 3
  if len <= 3 then return;

  verify(w.alignedLow .. w.alignedLow + 2 * sa by sa);
  verify(w.alignedLow .. w.alignedLow + 2 * sa by -sa);
  verify(w.alignedHigh - 2 * sa .. w.alignedHigh by sa);
  verify(w.alignedHigh - 2 * sa .. w.alignedHigh by -sa);

} // proc runSuite

/////////////////////////////////

use DSIUtil;

proc verify(w, r) {
  numVerifies += 1;
  proc report1() { write(w, " // ", r); }
  if verbose then report1();

  const res = densifyBounded(w, r);
  proc report2() { write(" = ", res); }
  proc report()  { report1(); report2(); }
  if verbose then report2();

  if res.size != r.size {
    if !verbose then report();
    writeln("  UNEXPECTED: different length");
    return;
  }
  for (inp,outp) in zip(r,res) {
    if outp != w.indexOrder(inp) {
      if !verbose then report();
      writeln("  UNEXPECTED: ", outp, " is not indexOrder of ", inp);
    }
    if inp != w.orderToIndex(outp) {
      if !verbose then report();
      writeln("  UNEXPECTED: ", inp, " is not orderToIndex of ", outp);
    }
  }
  if verbose then writeln(); // OK
}

/////////////////////////////////

proc test(w, r, e) {
  verify(w, r);
  const res = densifyBounded(w, r);
  if res != e then
    writeln(" UNEXPECTED, should be ", e);
}

///////////

test(2..7, 1..0, 1..0);
test(2..7, 3..3, 1..1);
test(2..7, 3..4, 1..2);
test(2..7, 3..7, 1..5);

///////////

test(2..7 by -1, 1..0, 1..0);
test(2..7 by -1, 3..3 by -1, 4..4);
test(2..7 by -1, 3..3,       4..4);
test(2..7 by -1, 3..4 by -1, 3..4);
test(2..7 by -1, 3..4 by 2,  4..4);
test(2..7 by -1, 2..5 by -1, 2..5);

///////////

test(2..11 by 3, 1..0, 1..0);
test(2..11 by 3, 5..5,      1..1);
test(2..11 by 3, 5..5 by 3, 1..1);
test(2..11 by 3, 5..6 by 3, 1..1);
test(2..11 by 3, 5..9 by 3, 1..2);
test(2..11 by 3, 1..9 by 3 align 2, 0..2);
test(1..12 by 3 align 2, 2..8 by 3, 0..2);

///////////

test(2..11 by -3, 1..0, 1..0);
test(2..11 by -3, 5..5 by -3, 2..2);
test(2..11 by -3, 4..5 by -3, 2..2);
test(2..11 by -3, 5..11 by -3, 0..2);
test(2..11 by -3, 1..9 by -3 align 2, 1..3);
test(1..12 by -3 align 2, 2..8 by -3, 1..3);

///////////

writeln("DONE  ", numVerifies, " verifies");
