// Same as simple1.chpl, but instead fast writer, slow reader.
// Outputs what was read.

param ITERATIONS = 10;
param WAIT = 10000000;
var s: sync int;

begin {
  var r: int;
  var j: int;
  j = 0;
  while (j < ITERATIONS) {
    for w in 1..WAIT do;
    r = s.readFE();
    writeln( "2: got ", r);
    j += 1;
  }
}

var k: int;
k = 0;
while (k < ITERATIONS) {
  s.writeEF(k);
  k += 1;
}
