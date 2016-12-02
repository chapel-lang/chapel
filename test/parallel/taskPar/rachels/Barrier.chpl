// This Barrier is intended to store the finished status of each task locally
// on each locale (instead of on one locale that the other tasks have to keep
// an eye on).  It's currently limited to a single task per locale, but this
// could be fairly simply modified by making tasksFinished a [PrivateSpace] atomic
// int instead, counting down from an initial value of tasksPerLocale.
//
// However, in its present state, this Barrier performs very poorly by comparison
// to its ancestor in test/parallel/taskPar/sungeun/barrier.  We hypothesize that
// in a situation where the different tasks hit the barrier at very different times,
// this barrier might have better performance; however, this hypothesis hasn't
// been tested rigorously.
//
// Its performance is notably better if the flag disableBlockLazyRAD=true.

use BlockDist;

// use this instead of a PrivateDist for performance reasons (PrivateDists are expensive!)
const PrivateSpace = LocaleSpace dmapped Block(boundingBox=LocaleSpace);

record Barrier {
  param reusable = true;
  var n: int;
  var count: atomic int;
  var tasksFinished: [PrivateSpace] atomic bool; // to store finished? status locally
                                                 // on each locale

  proc Barrier() {
    reset(numLocales);
  }

  inline proc reset(_n: int) {
    on this {
      n = _n;
      count.write(n);
      for loc in tasksFinished {
        loc.write(false);
      }
    }
  }

  inline proc barrier() {
    const myc = count.fetchSub(1);
    if myc <= 1 {
      if tasksFinished[here.id].read() {
        halt("too many callers to barrier()");
      } 
      for loc in tasksFinished { // I'm the last one to finish, notify everyone!
        loc.write(true);
      }
      if reusable { // cleanup for reuse
        count.waitFor(n-1);
        count.add(1);
        for loc in tasksFinished {
          loc.clear();
        }
      }
    } else {
      tasksFinished[here.id].waitFor(true);
      if reusable { // cleanup for reuse
        count.add(1);
        tasksFinished[here.id].waitFor(false);
      }
    }
  }

  inline proc notify() {
    const myc = count.fetchSub(1);
    if myc <= 1 {
      if tasksFinished[here.id].read() {
        halt("too many callers to notify()");
      }
      for loc in tasksFinished {
        loc.write(true);
      }
    }
 }

  inline proc wait() {
    tasksFinished[here.id].waitFor(true);
    if reusable {
      var sum = count.fetchAdd(1);
      if sum == n-1 then tasksFinished.clear();
      else tasksFinished[here.id].waitFor(false);
    }
  }

  inline proc check() {
    return tasksFinished[here.id];
  }
}

