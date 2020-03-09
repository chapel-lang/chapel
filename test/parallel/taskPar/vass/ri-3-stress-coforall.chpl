// Stress test for parallel correctness.

config var
  r = 10000,  // how many times to repeat
  t = 16,     // how many tasks to issue concurrently
  f = 2000;   // frequency of reports, 0 if none

var nErr = 0;

proc main {
  writeConfig("starting"); writeln();
  for ri in 1..r {
    onetest(ri);
    if f != 0 && ri % f == 0 then writeln(ri, " tests");
  }
  writeConfig("done");
  if nErr then writeln("   ", nErr, " errors");
  else writeln("   all good");
}

proc writeConfig(msg) {
  write(msg, "  ", r, " tests  ", t, " tasks");
}

/////////// testing ///////////

proc onetest(ri: int) {
  var red = 5;
  coforall idx in 1..t with (+ reduce red) {
    red += idx;
  }
  const result = red;
  check(result, 5 + sum(1..t), ri);
}

proc sum(r) { const l = r.size; return l * (l+1) / 2; }

proc check(actual, expected, ri) {
  if actual == expected then return; // OK!
  nErr += 1;
  writeln("ERROR: onetest(", ri, ")  expected ", expected,
          "  actual ", actual);
}
