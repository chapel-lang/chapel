// Test class with a sync var field.

// Part 1. Main thread slowly writes to s.  Child thread reads from s and
// outputs what was read.

param ITERATIONS = 10;
param WAIT = 10000000;

class C {
  var s: sync int;
}
var c: unmanaged C = new unmanaged C();

writeln( "\ntest 1");
begin {
  var r: int;
  var j: int;
  j = 0;
  while (j < ITERATIONS) {
    r = c.s.readFE();
    writeln( "2: got ", r);
    j += 1;
  }
  delete c;
}

var k: int;
k = 0;
while (k < ITERATIONS) {
  for w in 1..WAIT do;
  c.s.writeEF(k);
  k += 1;
}
