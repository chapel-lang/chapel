//
// A simple barrier implementation.
//
// * Uses an atomic for counting and waitFor() for waiting. Hence "WF".
//
// * A BarrierWF object can be used for multiple barriers. I.e.
//   after everyone reaches the barrier, the object is ready for the next one.
//
// * The number of tasks expected at the barrier is a constructor argument.
//   Updates to that number are not supported.
//
// * To use: (a) create it: const b = new BarrierWF(numTasks);
//   (b) to execute a barrier, each of the numTasks tasks calls b.barrier().
//
// * It is not specifically optimized for multi-locale execution.
//
// Most of this code can be reused for a barrier where the number
// of barriering tasks is given as an argument to the barrier() method,
// assuming that number will be consistent among the tasks.
//
class BarrierWF {
  // how many tasks are barriering
  const tasks: int;

  // which of the counts below is active: 0 or 1
  var active: int;

  // the alternating counts
  var counts: 2 * atomic int;

  // constructor
  proc init(numTasks: int) {
    // otherwise the barrier conditions are hosed
    if numTasks <= 0 then halt("BarrierWF constructor expects numTasks>0",
                               " but received ", numTasks);
    tasks = numTasks;
    this.complete();
    setup(0);
  }

  // set up for the next barrier
  proc setup(newActive: int) {
    active = newActive;
    counts(newActive).write(tasks);
  }

  // what would be the other "active"
  proc nextActive(act) return 1-act;

  // To be invoked by each task to barrier.
  // After everyone reaches the barrier, it can be used again.
  proc barrier() {
    const curActive = active;
    const myc = counts(curActive).fetchSub(1) - 1;
    if myc > 0 {
      counts(curActive).waitFor(-1);
    } else {
      assert(myc == 0);
      // the last arriving task sets up for the next barrier
      setup(nextActive(curActive));
      // ... and allows the others to proceed
      counts(curActive).write(-1);
    }
  }
}  // class BarrierWF
