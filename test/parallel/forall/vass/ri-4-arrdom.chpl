// Stress test for parallel correctness.

config var
  r = 30000,  // how many times to repeat
  d = 250,    // each dimension of the domain and array
  f = 5000;   // frequency of reports, 0 if none

var nErr = 0;

var
  D1 = {1..d, 1..d},
  D2 = {0..d-1, 0..d-1},
  A1: [D1] int,
  A2: [D2] real;

proc main {
  writeConfig("starting"); writeln();
  initArrays();
  for ri in 1..r {
    domain11test(ri);
    domain12test(ri);
    domain21test(ri);
    domain22test(ri);
/* coming up:
    array1test(ri);
    array21test(ri);
    array22test(ri);
*/
    if f != 0 && ri % f == 0 then writeln(ri, " tests");
  }
  writeConfig("done");
  if nErr then writeln("   ", nErr, " errors");
  else writeln("   all good");
}

proc writeConfig(msg) {
  write(msg, "  ", r, " tests  ", d, " size");
}

/////////// testing - domains ///////////

proc domain11test(ri: int) {
  var red = 5;
  forall (i,j) in D1 with (+ reduce red) {
    red += i + 2*j;
  }
  const result1d = red;
  check(result1d, 3 * d * sum1(d), ri, "single domain");
}

proc domain12test(ri: int) {
  var red1 = 5, red2 = 6;
  forall (i,j) in D1 with (+ reduce red1, + reduce red2) {
    red1 += i;
    red2 += j;
  }
  check(red1, d * sum1(d), ri, "single domain - red1");
  check(red2, d * sum1(d), ri, "single domain - red2");
}

proc domain21test(ri: int) {
  var red = 5;
  forall ((i1,j1),(i2,j2)) in zip(D1,D2) with (+ reduce red) {
    red += i1*2 + j1*3 + i2*4 + j2*5;
  }
  const result1d = red;
  check(result1d, 5 * d * sum1(d) + 9 * d * sum0(d),
        ri, "zippered domains");
}

proc domain22test(ri: int) {
  var red1 = 5, red2 = 6;
  forall ((i1,j1),(i2,j2)) in zip(D1,D2) with (+ reduce red1, + reduce red2) {
    red1 += i1*2 + j1*3;
    red2 += i2*4 + j2*5;
  }
  check(red1, 5 * d * sum1(d), ri, "zippered domains - red1");
  check(red2, 9 * d * sum0(d), ri, "zippered domains - red2");
}

/////////// testing - arrays ///////////

proc initArrays() {
  for (a1, a2, (i,j)) in zip(A1,A2,D1) {
    a1 = i + 2*j;
    a2 = 3*i + j;
  }
}

proc array1test(ri: int) {
  var red = 5;
  forall a1 in A1 with (+ reduce red) {
    red += a1;
  }
  const result1d = red;
  check(result1d, 3 * d * sum1(d), ri, "single array");
}

proc array21test(ri: int) {
  var red = 5.5;
  forall (a1,a2) in zip(A1,A2) with (+ reduce red) {
    red += 2*a1 + 3*a2;
  }
  const result1d = red;
  check(result1d, 18 * d * sum1(d), ri, "zippered arrays");
}

proc array22test(ri: int) {
  var red1 = 5, red2 = 6.6;
  forall (a1,a2) in zip(A1,A2) with (+ reduce red1, + reduce red2) {
    red1 += a1;
    red2 += a2;
  }
  check(red1, 3 * d * sum1(d), ri, "zippered arrays - red1");
  check(red2, 4 * d * sum1(d), ri, "zippered arrays - red2");
}

/////////// checking ///////////

proc sum0(l) return l * (l-1) / 2;  // sum(0..l-1)
proc sum1(l) return l * (l+1) / 2;  // sum(1..l)

proc check(actual, expected, ri, name) {
  if actual == expected then return; // OK!
  nErr += 1;
  writeln("ERROR: onetest(", ri, ", ", name, ")  expected ", expected,
          "  actual ", actual);
}
