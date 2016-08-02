// When it was added, this test deadlocked with any of the execopts that turned
// on memory tracking (--memTrack, --memLeaks, etc.) In order for the atomic
// "lock"/"unlock" (testAndSet()/clear() respectively) to not deadlock, the
// tasks can't yield, which means that memory tracking routines can't ever
// cause tasks to yield.

var x: int;
var iters = here.maxTaskPar * 100;
var l: atomic bool;

proc xInc() {
  chpl_here_free(chpl_here_alloc(1, 0)); // need an allocation for mem tracking
  x += 1;
}

coforall i in 0..#iters {
  while l.testAndSet() {
    // yielding would eventually allow the task we're waiting on to be
    // rescheduled, masking any potential task yields in mem tracking
    //
    // chpl_task_yield();
  }
  xInc();
  l.clear();
}

writeln(x/here.maxTaskPar);
