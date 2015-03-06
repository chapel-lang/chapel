// Test writing of sync/single vars.
// In the past, it assumed write() did not interfer with full/empty
// sync/single args were passed by-reference argument.

// Global vars.
var g1: sync real;
var g2: single real;

writeln("global - empty: ", g1, " ", g2);
g1 = 1;
g2 = 2;
writeln("global - full: ", g1, " ", g2);
g1;
writeln("global - e/f: ", g1, " ", g2);

testLocals();

// Test local variables and generic by-ref passing.
proc testLocals() {
  var l1: sync int;
  var l2: single int;
  report("empty", l1, l2);
  l1 = 111;
  l2 = 222;
  report("full", l1, l2);
  l1;
  report("e/f", l1, l2);
}

proc report(msg, var1, var2) {
  writeln("local - ", msg, ": ", var1, " ", var2);
}
