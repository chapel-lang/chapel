module Barrier {

  record BarrierBaseType {
    proc barrier() {
      compilerError("barrier() not implemented");
    }
    proc notify() {
      compilerWarning("notify() not implemented");
    }
    proc wait() {
      compilerWarning("wait() not implemented");
    }
    proc try() {
      compilerWarning("try() not implemented");
    }
  }

  extern proc chpl_task_yield();
  record Barrier: BarrierBaseType {
    var count: atomic int;
    var done: atomic bool;

    proc Barrier(n: int) {
      reset(n);
    }

    inline proc reset(n: int) {
      on this {
        count.write(n);
        done.write(false);
      }
    }

    inline proc barrier() {
      on this {
        const myc = count.fetchSub(1);
        if myc<=1 {
          if done.testAndSet() then
            halt("Too many callers to barrier()");
        } else {
          wait();
        }
      }
    }

    inline proc notify() {
      on this {
        const myc = count.fetchSub(1);
        if myc<=1 {
          if done.testAndSet() then
            halt("Too many callers to notify()");
        }
      }
    }

    inline proc wait() {
      on this do
        while !done.read() do chpl_task_yield();
    }

    inline proc try() {
      return done.read();
    }
  }

  //
  // sync/single version
  //
  record sBarrier: BarrierBaseType {
    var count: sync int;
    var done: single bool;

    proc sBarrier(n: int) {
      count = n;
    }

    inline proc barrier() {
      on this {
        const myc = count;
        if myc==1 {
          done = true;
        } else {
          if myc < 1 then
            halt("Too many callers to barrier()");
          count = myc-1;
          wait();
        }
      }
    }

    inline proc notify() {
      on this {
        const myc = count;
        if myc==1 {
          done = true;
        } else {
          if myc < 1 then
            halt("Too many callers to notify()");
          count = myc-1;
        }
      }
    }

    inline proc wait() {
      done;
    }

    inline proc try() {
      return done.readXX();
    }
  }

}

