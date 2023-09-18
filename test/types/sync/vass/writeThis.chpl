// Test writing of sync vars.
// In the past, it assumed write() did not interfer with full/empty
// sync args were passed by-reference argument.

// Global vars.
var g1: sync real;

writeln("global - empty: ", g1);
g1 = 1;
writeln("global - full: ", g1);
g1;
writeln("global - e/f: ", g1);

testLocals();

// Test local variables and generic by-ref passing.
proc testLocals() {
  var l1: sync int;
  report("empty", l1);
  l1 = 111;
  report("full", l1);
  l1;
  report("e/f", l1);
}

proc report(msg, var1) {
  writeln("local - ", msg, ": ", var1);
}
