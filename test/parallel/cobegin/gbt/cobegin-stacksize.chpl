//
// This test is intended to show that CHPL_RT_CALL_STACK_SIZE properly sets the
// call stack size. Other tests check that the runtime reports the correct
// value, but this will show if the value is actually changed based on whether
// or not program execution was successful or not. To do this we run the
// program twice, once with a really small stack size, and once with a really
// large stack size to avoid having to think about what the stack size is for
// different configurations and worrying about it changing in the future.
//

//
// First, run some tasks whose only purpose is to encourage the tasking
// layer to start threads to run them.  What we're trying to do here is
// to increase the likelihood that the two tasks below which call
// recursive_sum() do so on threads other than the one hosting the
// parent (main) task, since that thread may or may not have had its
// stack size set.  For this, it's necessary though not sufficient that
// we get those tasks started now.
//
// (Note that this cobegin and the one below were both constructed with
// the implementation of tasks=fifo (as of 2016/01/28) in mind, in
// particular, knowing that the parent's thread tries to run child
// tasks from last to first.)
//
var started_1, started_2: sync bool;
cobegin {
  {
    started_1.writeEF(true);
  }
  {
    started_1.readFE();
    started_2.writeEF(true);
  }
  {
    started_2.readFE();
  }
}

config var depth: uint(64) = 1024;

var sum1$, sum2$: single uint(64);

proc recursive_sum(n: uint(64)): uint(64) {
  if n <= 1 then return n; else return n + recursive_sum(n - 1);
}

//
// The real pass/fail test here is based on the recursive_sum() calls
// in the first two tasks, which should be run on threads subject to
// the stack size setting.  The last task is just here to consume
// the attention of the thread running the parent task if that one
// is available to run children while waiting for the cobegin to
// finish.  (The thread running the parent might be the process for
// the program, and might not be subject to the stack size limit.)
//
cobegin {
  {
    started_1.writeEF(true);
    sum2$.writeEF(recursive_sum(depth));
  }
  {
    started_1.readFE();
    started_2.writeEF(true);
    sum1$.writeEF(recursive_sum(depth));
  }
  {
    started_2.readFE();
  }
}

writeln("sums are ", sum1$.readFF(), ", ", sum2$.readFF());
