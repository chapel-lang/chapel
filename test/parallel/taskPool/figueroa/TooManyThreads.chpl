// This test is intended to ensure that a warning message is emitted when the
// system is unable to create as many threads as requested.  (On most platforms,
// the default value of CHPL_RT_NUM_THREADS_PER_LOCALE is unbounded, but
// usually the system runs out of resources after two or three thousand
// threads have been created.)

// If CHPL_RT_NUM_THREADS_PER_LOCALE has been set then this test will fail
// because it won't print out the expected warning.  One could alter the prediff
// to use an alternate .good file in that case, but this seems like more trouble
// than it is worth at the moment.

// This test also makes sure that regardless of whether the system runs out of
// resources, tasks can be placed in the task pool, and these tasks are
// eventually performed to completion.

use Time;

extern proc setThreadLimit(l: int(64));
extern proc setStackLimit(l: int(64));

config const numThreads = 6000;
config const threadLimit = 500;
config const stackLimit = 8192;
var total: int,
    count: int = numThreads,
     done: sync bool,
    ready: sync bool;
ensureDefaultInit(ready);
proc ensureDefaultInit(arg) { }

proc foo (x) {

  if ready {
    total += x;
    count -= 1;
    if count == 0 then done = true;
    ready = true;
  }
    
}

// Set the system limit on number processes artificially low
setThreadLimit(threadLimit);
// Set the system limit on stack size artificially low
setStackLimit(stackLimit);

writeln ("need a short nap ...");
begin
  coforall i in 1..numThreads do
    foo (i);
sleep (10);
ready = true;
if done then writeln ("total is ", total);
