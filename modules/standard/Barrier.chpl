/*
 * Copyright 2004-2015 Cray Inc.
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

   The Barrier records provided in this module can be used to prevent tasks
   from proceeding until all other related tasks have also reached the
   barrier.

   In the following example, all of the tasks created by the coforall loop
   will print their "entering the barrier" messages in an unspecified order,
   then all of the tasks will print their "past the barrier" messages, also
   in an unspecified order.  Because of the barrier, all of the
   "entering the barrier" messages will be printed before any of the
   "past the barrier" messages. ::

     config const numTasks = here.maxTaskPar;
     var b = new Barrier(numTasks);

     coforall tid in 1..numTasks with (ref b) {
       writeln("Task ", tid, " is entering the barrier");
       b.barrier();
       writeln("Task ", tid, " is past the barrier");
     }

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
module Barrier {
  /* The BarrierBaseType record provides an abstract base type for barriers
   */
  record BarrierBaseType {
    pragma "no doc"
    proc barrier() {
      compilerError("barrier() not implemented");
    }

    pragma "no doc"
    proc notify() {
      compilerWarning("notify() not implemented");
    }

    pragma "no doc"
    proc wait() {
      compilerWarning("wait() not implemented");
    }

    pragma "no doc"
    proc try(): int {
      compilerWarning("try() not implemented");
    }
  }


/* A task barrier implemented using atomics. Can be used as a simple barrier
   or as a split-phase barrier.
 */
  record Barrier: BarrierBaseType {
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
    proc Barrier(n: int) {
      reset(n);
    }

    pragma "no doc"
    inline proc reset(_n: int) {
      on this {
        n = _n;
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

    /* return `true` if `n` tasks have called :proc:`notify`
     */
    inline proc try(): bool {
      return done.read();
    }
  }

  /* A task barrier implemented using sync and single variables. Can be used
     as a simple barrier or as a split-phase barrier.
   */
  record sBarrier: BarrierBaseType {
    pragma "no doc"
    var count: sync int;
    pragma "no doc"
    var done: single bool;

    /* Construct a new `n` task Barrier.

       :arg n: The number of tasks that will be involved in the barrier.
     */
    proc sBarrier(n: int) {
      count = n;
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

    /* return `true` if `n` tasks have called :proc:`notify`
     */
    inline proc try(): bool {
      return done.readXX();
    }
  }
}

