// Test class with a sync var field.

// Part 1. Main thread slowly writes to s.  Child thread reads from s and
// outputs what was read.

param ITERATIONS = 10;
param WAIT = 10000000;

class C {
  var s: sync int;
}
var c: C = C();

writeln( "\ntest 1");
begin {
  var r: int;
  var j: int;
  j = 0;
  while (j < ITERATIONS) {
    r = c.s;
    writeln( "2: got ", r);
    j += 1;
  }
}

var k: int;
k = 0;
while (k < ITERATIONS) {
  for w in 1..WAIT;
  c.s = k;
  k += 1;
}

// Part 2. Same as above, but instead fast writer, slow reader.
// Outputs what was read.

writeln( "\ntest 2");
begin {
  var r: int;
  var j: int;
  j = 0;
  while (j < ITERATIONS) {
    for w in 1..WAIT;
    r = c.s;
    writeln( "2: got ", r);
    j += 1;
  }
}

k = 0;
while (k < ITERATIONS) {
  c.s = k;
  k += 1;
}
