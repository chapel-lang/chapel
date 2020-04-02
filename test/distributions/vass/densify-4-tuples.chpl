// Testing densifyBounded on tuples,
// assuming its correctness on ranges.

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


const samples = (
  2..7 by 1,  1..0 by 1,
  2..7 by 1,  3..4 by 1,
  2..7 by 1,  3..4 by -1,
  2..7 by -1, 3..4 by 1,
  2..7 by -1, 3..4 by -1,
  2..7 by 2,  1..7 by 4 align 2,
  2..7 by 2,  1..7 by -4 align 2,
  1..0 by 1 /* dummy to mark the end of the list */
);

// If we make things below non-param (to reduce code size),
// 'samples' above must be homogenous.
// Otherwise it is OK to vary stridability among samples' members.

/*param*/ const nt = samples.size / 2;
proc w(/*param*/ i) return samples(2 * i);
proc r(/*param*/ i) return samples(2 * i + 1);

for /*param*/ i1 in 0..#nt {

  // 1-tuples
  const w1 = w(i1), r1 = r(i1);
  const a1 = densifyBounded(w1, r1);
  const b1 = densifyBounded((w1,), (r1,));
  check(a1, (...b1), (w1, r1));

  for /*param*/ i2 in 0..#nt {

    // 2-tuples
    const w2 = w(i2), r2 = r(i2);
    const a2 = densifyBounded(w2, r2);
    const b2 = densifyBounded((w1, w2), (r1, r2));
    check((a1,a2), b2, ((w1, w2), (r1, r2)));

    for /*param*/ i3 in 0..#nt {

      // 3-tuples
      const w3 = w(i3), r3 = r(i3);
      const a3 = densifyBounded(w3, r3);
      const b3 = densifyBounded((w1,w2,w3), (r1,r2,r3));
      check((a1,a2,a3), b3, ((w1,w2,w3), (r1,r2,r3)));

    }
  }
}

proc check(a, b, msg) where isRange(a) && isRange(b) {
  if a != b then
    writeln("error: ", a, " vs ", b, "  context: ", msg);
}

proc check(a, b, msg) where isTuple(a) && isTuple(b) {
  if a.size != b.size then
    compilerError("check: size mismatch", (a.size, b.size),
                  "  context:", msg.type);
  for /*param*/ i in 0..#a.size do
    check(a(i), b(i), ((...msg), "  component ", i));
}
