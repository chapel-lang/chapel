/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

/* Support for barriers between tasks.

   The Barrier type provided in this module can be used to prevent tasks
   from proceeding until all other related tasks have also reached the
   barrier.

   In the following example, all of the tasks created by the coforall loop
   will print their "entering the barrier" messages in an unspecified order,
   then all of the tasks will print their "past the barrier" messages, also
   in an unspecified order.  Because of the barrier, all of the
   "entering the barrier" messages will be printed before any of the
   "past the barrier" messages.

   .. code-block:: chapel

     use Barriers;

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
deprecated "The 'Barriers' module is deprecated, please use 'Collectives' instead"
module Barriers {
  import HaltWrappers;
  import ChplConfig;
  import Collectives.{BarrierBaseType, aBarrier, sBarrier};

  /* An enumeration of the different barrier implementations.  Used to choose
     the implementation to use when constructing a new barrier object.

     * `BarrierType.Atomic` uses Chapel atomic variables to control the barrier.
     * `BarrierType.Sync` uses Chapel sync variables to control the barrier.
  */
  deprecated "BarrierType is deprecated, please use the default barrier implementation"
  enum BarrierType {Atomic, Sync}

  /* A barrier that will cause `numTasks` to wait before proceeding. */
  deprecated "The 'Barrier' type is deprecated, please use 'Collectives.barrier' instead"
  record Barrier {
    pragma "no doc"
    var bar: unmanaged BarrierBaseType;
    pragma "no doc"
    var isowned: bool = false;

    /* Construct a new barrier object.

       :arg numTasks: The number of tasks that will use this barrier
       :arg barrierType: The barrier implementation to use
       :arg reusable: Incur some extra overhead to allow reuse of this barrier?

    */
    proc init(numTasks: int, reusable: bool = true) {
      if reusable {
        bar = new unmanaged aBarrier(numTasks, reusable=true);
      } else {
        bar = new unmanaged aBarrier(numTasks, reusable=false);
      }
      isowned = true;
    }

    /* Construct a new barrier object.

       :arg numTasks: The number of tasks that will use this barrier
       :arg barrierType: The barrier implementation to use
       :arg reusable: Incur some extra overhead to allow reuse of this barrier?

    */
    deprecated "choosing a barrier type is deprecated, please remove the 'barrierType' argument"
    proc init(numTasks: int,
              barrierType: BarrierType,
              reusable: bool = true) {
      select barrierType {
        when BarrierType.Atomic {
          if reusable {
            bar = new unmanaged aBarrier(numTasks, reusable=true);
          } else {
            bar = new unmanaged aBarrier(numTasks, reusable=false);
          }
        }
        when BarrierType.Sync {
          if reusable {
            bar = new unmanaged sBarrier(numTasks, reusable=true);
          } else {
            bar = new unmanaged sBarrier(numTasks, reusable=false);
          }
        }
        otherwise {
          HaltWrappers.exhaustiveSelectHalt("unknown barrier type");
          bar = new unmanaged BarrierBaseType(); // dummy
        }
      }
      isowned = true;
    }

    pragma "no doc"
    proc init() {
      this.init(0);
    }

    /* copy initializer */
    pragma "no doc"
    proc init=(b: Barrier) {
      this.bar = b.bar;
      this.isowned = false;
    }

    pragma "no doc"
    proc deinit() {
      if isowned && bar != nil {
        delete bar;
      }
    }

    /* Block until all related tasks have called this method.  If `reusable`
       is true, reset the barrier to be used again.
     */
    inline proc barrier() {
      bar.barrier();
    }

    /* Notify the barrier that this task has reached this point. */
    inline proc notify() {
      bar.notify();
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
      bar.wait();
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
      bar.reset(nTasks);
    }
  }

  pragma "no doc"
  operator Barrier.=(ref lhs: Barrier, rhs: Barrier) {
    if lhs.isowned {
      delete lhs.bar;
    }
    lhs.bar = rhs.bar;
    lhs.isowned = false;
  }

}
