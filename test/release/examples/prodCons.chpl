/* 
 *   Producer-Consumer Example. 
 *
 *   Producer thread slowly writes to sync variable
 *   s.  Child thread reads from s and outputs what was read.
 */

use Time;

config var numIterations: int = 5,
           sleepTime: uint = 2;

var s: sync int;

begin {  // fork consumer computation
  for c in 1..numIterations do
    writeln("consumer got ", s);
}

// producer computation
for p in 1..numIterations {
  sleep(sleepTime);
  s = p;
}
