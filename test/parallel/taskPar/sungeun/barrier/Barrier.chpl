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

  record Barrier: BarrierBaseType {
    param reusable = true;
    var n: int;
    var count: atomic int;
    var done: atomic bool;

    proc Barrier(_n: int) {
      reset(_n);
    }

    inline proc reset(_n: int) {
      on this {
        n = _n;
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
          if reusable {
            count.waitFor(n-1);
            count.add(1);
            done.clear();
          }
        } else {
          done.waitFor(true);
          if reusable {
            count.add(1);
            done.waitFor(false);
          }
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
      on this {
        done.waitFor(true);
        if reusable {
          const myc = count.fetchAdd(1);
          if myc == n-1 then
            done.clear();
          done.waitFor(false);
        }
      }
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

