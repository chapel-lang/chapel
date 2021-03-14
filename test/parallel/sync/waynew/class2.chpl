// Test class with a sync var field.

// Similar to class1.chpl, but instead fast writer, slow reader.
// Outputs what was read.

param ITERATIONS = 10;
param WAIT = 10000000;

class C {
  var s: sync int;
}
var c: unmanaged C = new unmanaged C();

writeln( "\ntest 2");
begin {
  var r: int;
  var j: int;
  j = 0;
  while (j < ITERATIONS) {
    for w in 1..WAIT do;
    r = c.s.readFE();
    writeln( "2: got ", r);
    j += 1;
  }
  delete c;
}

var k: int;
k = 0;
while (k < ITERATIONS) {
  c.s.writeEF(k);
  k += 1;
}
