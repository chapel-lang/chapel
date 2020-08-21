use DSIUtil;

// I used to have a separate densifyToUnitStride implementation.
// This test was written for it. Now we fake it.
proc densifyToUnitStride(w,r) {
  const result = densify(r,w);
  testUndensify(w,r,result);
  return result;
}

proc testUndensify(whole, r, dense) {
  const undense = unDensify(dense, whole);
  if undense != r then
    writeln("testUndensify() error: ", (whole, r, dense, undense));
}

proc testerr(w, r) {
  write(w, " // ", r, "\n  error: ");
  const res: range(w.idxType) = densifyToUnitStride(w, r);
  const err = 1..0;
  if res != err then
    writeln("UNEXPECTED result: ", res);
}

proc test(w, r, exp) {
  write(w, " // ", r, " ");
  const res = densifyToUnitStride(w, r);
  write("= ", res);
  if res == exp then
    writeln();
  else
    writeln(" UNEXPECTED  should be ", exp);
}

///////////

test(2..7, 1..0, 1..0);
test(2..7, 3..3, 1..1);
test(2..7, 3..4, 1..2);
test(2..7, 3..7, 1..5);
// ok - diff strides: testerr(2..7, 3..5 by 2);
// ok - diff strides: testerr(2..7, 3..5 by -1);

// ok - crash: testerr(2..7, 1..1);
// ok - crash: testerr(2..7, 1..2);
// ok - crash: testerr(2..7, 7..8);
// ok - crash: testerr(2..7, 8..8);

///////////

test(2..7 by -1, 1..0, 1..0);
test(2..7 by -1, 3..3 by -1, 4..4);
test(2..7 by -1, 3..3,       4..4);
test(2..7 by -1, 3..4 by -1, 3..4);
test(2..7 by -1, 3..4 by 2,  4..4);
test(2..7 by -1, 2..5 by -1, 2..5);

// ok - diff strides: testerr(2..7 by -1, 3..4);
// ok - diff strides: testerr(2..7 by -1, 3..6 by -2);
// ok - diff strides: testerr(2..7 by -1, 3..5 by 2);
// ok - "unexpected": testerr(2..7 by -1, 3..5 by -1);

// ok - crash: testerr(2..7 by -1, 1..1 by -1);
// ok - crash: testerr(2..7 by -1, 1..2 by -1);
// ok - crash: testerr(2..7 by -1, 7..8 by -1);
// ok - crash: testerr(2..7 by -1, 8..8 by -1);

///////////

test(2..11 by 3, 1..0, 1..0);
test(2..11 by 3, 5..5,      1..1);
test(2..11 by 3, 5..5 by 3, 1..1);
test(2..11 by 3, 5..6 by 3, 1..1);
test(2..11 by 3, 5..9 by 3, 1..2);
test(2..11 by 3, 1..9 by 3 align 2, 0..2);
test(1..12 by 3 align 2, 2..8 by 3, 0..2);

// ok - diff strides: testerr(2..11 by 3, 2..3);
// ok - diff strides: testerr(2..11 by 3, 2..5 by -1);
// ok - diff strides: testerr(2..11 by 3, 2..5 by -3);

///////////

test(2..11 by -3, 1..0, 1..0);
test(2..11 by -3, 5..5 by -3, 2..2);
test(2..11 by -3, 4..5 by -3, 2..2);
test(2..11 by -3, 5..11 by -3, 0..2);
test(2..11 by -3, 1..9 by -3 align 2, 1..3);
test(1..12 by -3 align 2, 2..8 by -3, 1..3);

// ok - diff strides: testerr(2..11 by -3, 2..3);
// ok - diff strides: testerr(2..11 by -3, 2..5 by -1);
// ok - diff strides: testerr(2..11 by -3, 2..5 by 3);
