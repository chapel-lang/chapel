/*
 * Copyright 2004-2018 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Support for task barriers.

   The Barrier type provided in this module can be used to prevent tasks
   from proceeding until all other related tasks have also reached the
   barrier.

   In the following example, all of the tasks created by the coforall loop
   will print their "entering the barrier" messages in an unspecified order,
   then all of the tasks will print their "past the barrier" messages, also
   in an unspecified order.  Because of the barrier, all of the
   "entering the barrier" messages will be printed before any of the
   "past the barrier" messages. ::

     use Barrier;

     config const numTasks = here.maxTaskPar;
     var b = new Barrier(numTasks);

     coforall tid in 1..numTasks {
       writeln("Task ", tid, " is entering the barrier");
       b.barrier();
       writeln("Task ", tid, " is past the barrier");
     }

   Note: When a barrier instance goes out of scope it is automatically deleted.
   After it is deleted, any copies of the barrier that remain are invalid.

   Future direction
   ----------------
   In the future, we expect to add more language-level support for a
   "task-team" concept.  A task-team will more directly support collective
   operations such as barriers between the tasks within a team.

   The current implementation is designed for correctness, but is not expected
   to perform well at scale.  We expect performance at scale to improve as
   this barrier implementation is optimized and as the task-team concept is
   implemented and optimized.
*/
module Barriers {
  /* An enumeration of the different barrier implementations.  Used to choose
     the implementation to use when constructing a new barrier object.

     * `BarrierType.Atomic` uses Chapel atomic variables to control the barrier.
     * `BarrierType.Sync` uses Chapel sync variables to control the barrier.
  */
  enum BarrierType {Atomic, Sync}

  /* A barrier that will cause `numTasks` to wait before proceeding. */
  record Barrier {
    pragma "no doc"
    var bar: BarrierBaseType;
    pragma "no doc"
    var owned: bool = false;

    /* Construct a new barrier object.

       :arg numTasks: The number of tasks that will use this barrier
       :arg barrierType: The barrier implementation to use
       :arg reusable: Incur some extra overhead to allow reuse of this barrier?

    */
    proc init(numTasks: int,
              barrierType: BarrierType = BarrierType.Atomic,
              reusable: bool = (barrierType == BarrierType.Atomic)) {
      select barrierType {
        when BarrierType.Atomic {
          if reusable {
            bar = new aBarrier(numTasks, reusable=true);
          } else {
            bar = new aBarrier(numTasks, reusable=false);
          }
        }
        when BarrierType.Sync {
          if reusable {
            halt("reusable barriers not implemented for ", barrierType);
          } else {
            bar = new sBarrier(numTasks);
          }
        }
        otherwise {
          halt("unknown barrier type");
        }
      }
      owned = true;
    }

    /* copy initializer */
    pragma "no doc"
    proc init(b: Barrier) {
      this.bar = b.bar;
      this.owned = false;
      super.init();
    }

    pragma "no doc"
    proc deinit() {
      if owned && bar != nil {
        delete bar;
      }
    }

    /* Block until all related tasks have called this method.  If `reusable`
       is true, reset the barrier to be used again.
     */
    inline proc barrier() {
      on bar {
        bar.barrier();
      }
    }

    /* Notify the barrier that this task has reached this point. */
    inline proc notify() {
      on bar {
        bar.notify();
      }
    }

    /* Wait until `n` tasks have called :proc:`notify`.  If `reusable` is true,
       reset the barrier to be used again.

       Note: if `reusable` is true the tasks will wait until all `n` tasks
       have called both :proc:`notify` and :proc:`wait` at which point the
       barrier will automatically be reset for the next use.  If `reusable`
       is false, each task calling :proc:`wait` can return as soon as all
       `n` tasks have called :proc:`notify`.
     */
    inline proc wait() {
      on bar {
        bar.wait();
      }
    }

    /* Return `true` if `n` tasks have called :proc:`notify`
     */
    inline proc check(): bool {
      return bar.check();
    }

    /* Reset the barrier, setting it to work with `nTasks` tasks.  If some
       (but not all) tasks had already called :proc:`barrier` or :proc:`check`
       when :proc:`reset` is called, the behavior is undefined.
     */
    inline proc reset(nTasks: int) {
      on bar {
        bar.reset(nTasks);
      }
    }
  }

  /* The BarrierBaseType class provides an abstract base type for barriers
   */
  pragma "no doc" class BarrierBaseType {
    pragma "no doc"
    proc barrier() {
      halt("barrier() not implemented");
    }

    pragma "no doc"
    proc notify() {
      halt("notify() not implemented");
    }

    pragma "no doc"
    proc wait() {
      halt("wait() not implemented");
    }

    pragma "no doc"
    proc check(): bool {
      halt("check() not implemented");
    }

    pragma "no doc"
    proc reset(nTasks: int) {
      halt("reset() not implemented");
    }
  }


/* A task barrier implemented using atomics. Can be used as a simple barrier
   or as a split-phase barrier.
 */
  pragma "no doc" class aBarrier: BarrierBaseType {
    /* If true the barrier can be used multiple times.  When using this as a
       split-phase barrier this causes :proc:`wait` to block until all tasks
       have reached the wait */
    param reusable = true;

    pragma "no doc"
    var n: int;
    pragma "no doc"
    var count: atomic int;
    pragma "no doc"
    var done: atomic bool;

    /* Construct a new Barrier object.

       :arg n: The number of tasks involved in this barrier
     */
    proc init(n: int, param reusable: bool) {
      this.reusable = reusable;
      super.init();
      reset(n);
    }

    pragma "no doc"
    inline proc reset(nTasks: int) {
      on this {
        n = nTasks;
        count.write(n);
        done.write(false);
      }
    }

    /* Block until n tasks have called this method.  If `reusable` is true,
       reset the barrier to be used again.
     */
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

    /* Notify the barrier that this task has reached this point. */
    inline proc notify() {
      on this {
        const myc = count.fetchSub(1);
        if myc<=1 {
          if done.testAndSet() then
            halt("Too many callers to notify()");
        }
      }
    }

    /* Wait until `n` tasks have called :proc:`notify`.  If `reusable` is true,
       reset the barrier to be used again.  Note: if `reusable` is true
       :proc:`wait` will block until `n` tasks have called it after calling
       :proc:`notify`.
     */
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

    /* Return `true` if `n` tasks have called :proc:`notify`
     */
    inline proc check(): bool {
      return done.read();
    }
  }

  /* A task barrier implemented using sync and single variables. Can be used
     as a simple barrier or as a split-phase barrier.
   */
  pragma "no doc" class sBarrier: BarrierBaseType {
    pragma "no doc"
    var count: sync int;
    pragma "no doc"
    var done: single bool;

    /* Construct a new `n` task Barrier.

       :arg n: The number of tasks that will be involved in the barrier.
     */
    proc init(n: int) {
      count = n;
      super.init();
    }

    proc reset(nTasks: int) {
      halt("cannot reset sync based barrier");
    }

    /* Block until `n` tasks have called this method.
     */
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

    /* Notify the barrier that this task has reached this point. */
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

    /* Wait until `n` tasks have called :proc:`notify`. */
    inline proc wait() {
      done;
    }

    /* Return `true` if `n` tasks have called :proc:`notify`
     */
    inline proc check(): bool {
      return done.readXX();
    }
  }

  pragma "no doc"
  proc =(ref lhs: Barrier, rhs: Barrier) {
    if lhs.owned {
      delete lhs.bar;
    }
    lhs.bar = rhs.bar;
    lhs.owned = false;
  }

}

