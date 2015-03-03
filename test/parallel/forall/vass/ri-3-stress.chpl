// Stress test for parallel correctness.

config var
  r = 100000, // how many times to repeat
  t = 16,     // how many tasks to issue concurrently
  y = 6,      // how many times to yield from the follower
  f = 0;      // frequency of reports, 0 if none

var nErr = 0;

proc main {
  writeConfig("starting"); writeln();
  for ri in 1..r {
    onetestSA(ri);
    onetestLF(ri);
    if f != 0 && ri % f == 0 then writeln(ri, " tests");
  }
  writeConfig("done");
  if nErr then writeln("   ", nErr, " errors");
  else writeln("   all good");
}

proc writeConfig(msg) {
  write(msg, "  ", r, " tests  ", t, " tasks  ", y, "  flwr yields");
}

/////////// testing ///////////

proc onetestSA(ri: int) {
  var red = 5;
  forall idx in myStandalone() with (+ reduce red) {
    red += idx;
  }
  const resultSA = red;
  check(resultSA, sum(1..t), ri, "standalone");
}

proc onetestLF(ri: int) {
  var red = 5;
  forall idx in myLeaderfollower() with (+ reduce red) {
    red += idx;
  }
  const resultLF = red;
  check(resultLF, sum(1..t) * y + sum(1..y) * t, ri, "leader-follower");
}

proc sum(r) { const l = r.length; return l * (l+1) / 2; }

proc check(actual, expected, ri, name) {
  if actual == expected then return; // OK!
  nErr += 1;
  writeln("ERROR: onetest(", ri, ", ", name, ")  expected ", expected,
          "  actual ", actual);
}

/////////// iterators ///////////

iter myStandalone() {
  writeln("Serial SA");  // should not happen
  yield 111;
}

iter myStandalone(param tag) where tag == iterKind.standalone {
  coforall ti in 1..t do
    yield ti;
}

iter myLeaderfollower() {
  writeln("Serial LF");  // should not happen
  yield 111;
}

// leader
iter myLeaderfollower(param tag) where tag == iterKind.leader {
  coforall ti in 1..t do
    yield ti;
}

// follower
iter myLeaderfollower(followThis, param tag) where tag == iterKind.follower {
  for yi in 1..y do
    yield followThis + yi;
}
