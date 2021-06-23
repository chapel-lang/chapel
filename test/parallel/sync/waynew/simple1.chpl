// Main thread slowly writes to s.  Child thread reads from s and
// outputs what was read.

param ITERATIONS = 10;
param WAIT = 10000000;
var s: sync int;

begin {
  var r: int;
  var j: int;
  j = 0;
  while (j < ITERATIONS) {
    r = s.readFE();
    writeln( "2: got ", r);
    j += 1;
  }
}

var k: int;
k = 0;
while (k < ITERATIONS) {
  for w in 1..WAIT do;
  s.writeEF(k);
  k += 1;
}
