// This test is intended to ensure that a warning message is emitted when the
// system is unable to create as many threads as requested.  (On most platforms,
// the default value of maxThreads is unbounded, but usually the system runs out
// of resources after two or three thousand threads have been created.)

// This test also makes sure that regardless of whether the system runs out of
// resources, tasks can be placed in the task pool, and these tasks are
// eventually performed to completion.

use Time;

config const numThreads = 6000;
var total: int,
    count: int = numThreads,
     done: sync bool,
    ready: sync bool;

def foo (x) {

  if ready {
    total += x;
    count -= 1;
    if count == 0 then done = true;
    ready = true;
  }
    
}

writeln ("need a short nap ...");
begin
  coforall i in 1..numThreads do
    foo (i);
sleep (10);
ready = true;
if done then writeln ("total is ", total);
