// Assignment and initialization into `range(u/int(?))` from `range(bool)`
// should be allowed.

// When this is fixed, uncomment these cases in illegal-assigns.compopts
// and illegal-inits.compopts and remove this test.

var b: range(bool);
var r1: range(int(8)) = b;
var r2: range(int(64)) = b;
var r3: range(uint(8)) = b;
var r4: range(uint(64)) = b;

r1 = b;
r2 = b;
r3 = b;
r4 = b;

writeln("OK");
