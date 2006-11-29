// Producer-consumer example. Producer thread slowly writes to sync variable
// s.  Child thread reads from s and outputs what was read.

use Time;

param iterations = 5;
param sleepTime  = 2;
var s: sync int;

begin {  // fork consumer computation
  for c in 1..iterations do
    writeln("consumer got ", s);
}

// producer computation
for p in 1..iterations {
  sleep(sleepTime);
  s = p;
}
