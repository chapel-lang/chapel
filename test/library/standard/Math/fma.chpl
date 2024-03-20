import Math;

extern
proc c_sincosm1pi(a: real(64), ref res0: real(64), ref res1: real(64));

proc sincosm1pi(a: real(64), ref res0: real(64), ref res1: real(64)) {
  const t = a * a;
  const pi = 3.1415926535897931e+0;

  /*
   * Approximate cos(pi*x)-1 for x in [-0.25,0.25]
   * Approximate sin(pi*x) for x in [-0.25,0.25]
   */
  var c = (-1.0369917389758117e-4);
  var s = (+4.6151442520157035e-4);

  c = Math.fma(c, t,  1.9294935641298806e-3);
  s = Math.fma(s, t, -7.3700183130883555e-3);
  c = Math.fma(c, t, -2.5806887942825395e-2);
  s = Math.fma(s, t,  8.2145868949323936e-2);
  c = Math.fma(c, t,  2.3533063028328211e-1);
  s = Math.fma(s, t, -5.9926452893214921e-1);
  c = Math.fma(c, t, -1.3352627688538006e+0);
  s = Math.fma(s, t,  2.5501640398732688e+0);
  c = Math.fma(c, t,  4.0587121264167623e+0);
  s = Math.fma(s, t, -5.1677127800499516e+0);
  c = Math.fma(c, t, -4.9348022005446790e+0);
  s = Math.fma(a, pi, s * (t * a));

  res0 = c * t;
  res1 = s;
}

param seed1 = 123456789;
param seed2 = 314159265;
param seed3 = 123454321;

iter streamRandomNumeric(type T: numeric, hi: int) {
  import Random;
  const dom = {1..hi};
  var A, B, C: [dom] T;
  Random.fillRandom(A, seed1);
  Random.fillRandom(B, seed2);
  Random.fillRandom(C, seed3);
  for triple in zip(A, B, C) do yield triple;
}

iter streamRandomReal32(hi: int=4096) {
  for triple in streamRandomNumeric(real(32), hi) do yield triple;
}

iter streamRandomReal64(hi: int=4096) {
  for triple in streamRandomNumeric(real(64), hi) do yield triple;
}

proc test0() {
  extern proc fmaf(x: real(32), y: real(32), z:real(32)): real(32);
  for (x, y, z) in streamRandomReal32() {
    const n1 = Math.fma(x, y, z);
    const n2 = fmaf(x, y, z);
    const ok = isClose(n1, n2);
    assert(ok);
  }
}

proc test1() {
  extern proc fma(x: real(64), y: real(64), z:real(64)): real(64);
  for (x, y, z) in streamRandomReal64() {
    const n1 = Math.fma(x, y, z);
    const n2 = fma(x, y, z);
    const ok = isClose(n1, n2);
    assert(ok);
  }
}

proc test2() {
  for (x, _, _) in streamRandomReal64() {
    var ares0, ares1, bres0, bres1: real(64);
    c_sincosm1pi(x, ares0, ares1);
    sincosm1pi(x, bres0, bres1);
    const ok0 = isClose(ares0, bres0);
    const ok1 = isClose(ares1, bres1);
    assert(ok0 && ok1);
  }
}

proc main() {
  test0();
  test1();
  test2();
}
