// Tests the Quiescent State-Based Reclamation algorithm in cases where deadlock occurs
// when a thread that cannot pass a checkpoint is waiting on a write that is waiting on them.
// The desired result is that the older privatized array should be deferred for deletion
// to this spinning thread.

use PrivatizationWrappers;
use Memory;

config const numIters = 100000;

var m1 = memoryUsed();
var done : atomic bool;
done.write(false);

// Spin on atomic counter without calling checkpoint
begin {
	while !done.read() do ;
	writeln("Spinning Thread Finished!");
}

for i in 1 .. numIters do insertPrivatized(new object(), i);
done.write(true);

// no leaks
for i in 1 .. numIters {
  var c = getPrivatized(i);
  delete c;
  clearPrivatized(i);
}

var m2 = memoryUsed();

writeln("Memory Leaked: ", (m2 - m1) > 0);