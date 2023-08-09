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

   The barrier type provided in this module can be used to prevent tasks
   from proceeding until all other related tasks have also reached the
   barrier.

   In the following example, all of the tasks created by the coforall loop
   will print their "entering the barrier" messages in an unspecified order,
   then all of the tasks will print their "past the barrier" messages, also
   in an unspecified order.  Because of the barrier, all of the
   "entering the barrier" messages will be printed before any of the
   "past the barrier" messages.

   .. code-block:: chapel

     use Collectives;

     config const numTasks = here.maxTaskPar;
     var b = new barrier(numTasks);

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
module Collectives {
  import HaltWrappers;
  import ChplConfig;
  use CTypes;

  /* An enumeration of the different barrier implementations.  Used to choose
     the implementation to use when constructing a new barrier object.

     * `BarrierType.Atomic` uses Chapel atomic variables to control the barrier.
     * `BarrierType.Sync` uses Chapel sync variables to control the barrier.
  */
  @deprecated(notes="BarrierType is deprecated, please use the default barrier implementation")
  enum BarrierType {Atomic, Sync}

  /* A barrier that will cause `numTasks` to wait before proceeding. */
  record barrier {
    @chpldoc.nodoc
    var bar: unmanaged BarrierBaseType;
    @chpldoc.nodoc
    var isowned: bool = false;

    /* Construct a new barrier object.

       :arg numTasks: The number of tasks that will use this barrier

    */
    proc init(numTasks: int) {
      bar = new unmanaged aBarrier(numTasks, reusable=true);
      isowned = true;
    }


    /* Construct a new barrier object.

       :arg numTasks: The number of tasks that will use this barrier
       :arg reusable: Incur some extra overhead to allow reuse of this barrier?

    */
    @deprecated(notes="non-reusable barriers are deprecated, please remove the 'reusable' argument from this initializer call")
    proc init(numTasks: int, reusable: bool) {
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
    @deprecated(notes="choosing a barrier type is deprecated, please remove the 'barrierType' argument")
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

    @chpldoc.nodoc
    proc init() {
      this.init(0);
    }

    /* copy initializer */
    @chpldoc.nodoc
    proc init=(b: barrier) {
      this.bar = b.bar;
      this.isowned = false;
    }

    @chpldoc.nodoc
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

    /* Return `true` if fewer than `n` tasks have called :proc:`notify`
     */
    inline proc pending(): bool {
      return !bar.check();
    }

    @deprecated(notes="'barrier.check()' is deprecated, please use '!barrier.pending()' instead")
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

  /* The BarrierBaseType class provides an abstract base type for barriers
   */
  @chpldoc.nodoc
  class BarrierBaseType {
    @chpldoc.nodoc
    proc barrier() {
      HaltWrappers.pureVirtualMethodHalt();
    }

    @chpldoc.nodoc
    proc notify() {
      HaltWrappers.pureVirtualMethodHalt();
    }

    @chpldoc.nodoc
    proc wait() {
      HaltWrappers.pureVirtualMethodHalt();
    }

    @chpldoc.nodoc
    proc check(): bool {
      HaltWrappers.pureVirtualMethodHalt();
    }

    @chpldoc.nodoc
    proc reset(nTasks: int) {
      HaltWrappers.pureVirtualMethodHalt();
    }
  }

/* A task barrier implemented using atomics. Can be used as a simple barrier
   or as a split-phase barrier.
 */
  @chpldoc.nodoc class aBarrier: BarrierBaseType {
    /* If true the barrier can be used multiple times.  When using this as a
       split-phase barrier this causes :proc:`wait` to block until all tasks
       have reached the wait */
    param reusable = true;

    @chpldoc.nodoc
    var n: int;
    @chpldoc.nodoc
    param procAtomics = if ChplConfig.CHPL_NETWORK_ATOMICS == "none" then true else false;
    @chpldoc.nodoc
    var count: if procAtomics then chpl__processorAtomicType(int) else atomic int;
    @chpldoc.nodoc
    var done: if procAtomics then chpl__processorAtomicType(bool) else atomic bool;

    // Hack for AllLocalesBarrier
    @chpldoc.nodoc
    param hackIntoCommBarrier = false;

    /* Construct a new Barrier object.

       :arg n: The number of tasks involved in this barrier
     */
    proc init(n: int, param reusable: bool) {
      this.reusable = reusable;
      this.complete();
      reset(n);
    }

    // Hack for AllLocalesBarrier
    @chpldoc.nodoc
    proc init(n: int, param reusable: bool, param procAtomics: bool, param hackIntoCommBarrier: bool) {
      this.reusable = reusable;
      this.procAtomics = procAtomics;
      this.hackIntoCommBarrier = hackIntoCommBarrier;
      this.complete();
      reset(n);
    }

    @chpldoc.nodoc
    /* inline */ override proc reset(nTasks: int) {
      inline proc innerReset() {
        n = nTasks;
        count.write(n);
        done.write(false);
      }
      if procAtomics then on this do innerReset(); else innerReset();
    }

    /* Block until n tasks have called this method.  If `reusable` is true,
       reset the barrier to be used again.
     */
    /* inline */ override proc barrier() {
      inline proc innerBarrier() {
        const myc = count.fetchSub(1);
        if myc<=1 {
          if hackIntoCommBarrier {
            extern proc chpl_comm_barrier(msg: c_ptrConst(c_char));
            chpl_comm_barrier("local barrier call");
          }
          const alreadySet = done.testAndSet();
          if boundsChecking && alreadySet {
            HaltWrappers.boundsCheckHalt("Too many callers to barrier()");
          }
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
      if procAtomics then on this do innerBarrier(); else innerBarrier();
    }

    /* Notify the barrier that this task has reached this point. */
    /* inline */ override proc notify() {
      inline proc innerNotify() {
        const myc = count.fetchSub(1);
        if myc<=1 {
          const alreadySet = done.testAndSet();
          if boundsChecking && alreadySet {
            HaltWrappers.boundsCheckHalt("Too many callers to notify()");
          }
        }
      }
      if procAtomics then on this do innerNotify(); else innerNotify();
    }

    /* Wait until `n` tasks have called :proc:`notify`.  If `reusable` is true,
       reset the barrier to be used again.  Note: if `reusable` is true
       :proc:`wait` will block until `n` tasks have called it after calling
       :proc:`notify`.
     */
    /* inline */ override proc wait() {
      inline proc innerWait() {
        done.waitFor(true);
        if reusable {
          const myc = count.fetchAdd(1);
          if myc == n-1 then
            done.clear();
          done.waitFor(false);
        }
      }
      if procAtomics then on this do innerWait(); else innerWait();
    }

    /* Return `true` if `n` tasks have called :proc:`notify`
     */
    /* inline */ override proc check(): bool {
      return done.read();
    }
  }

  /* A task barrier implemented using sync and single variables. Can be used
     as a simple barrier or as a split-phase barrier.
   */
 @chpldoc.nodoc class sBarrier: BarrierBaseType {
    /* If true the barrier can be used multiple times.  When using this as a
       split-phase barrier this causes :proc:`wait` to block until all tasks
       have reached the wait */
    param reusable = true;

    @chpldoc.nodoc
    var inGate: sync int;
    @chpldoc.nodoc
    var outGate: sync int;
    @chpldoc.nodoc
    var blockers: chpl__processorAtomicType(int);
    @chpldoc.nodoc
    var maxBlockers: int;

    /* Construct a new `n` task Barrier.
       :arg n: The number of tasks that will be involved in the barrier.
     */
    proc init(n: int, param reusable: bool) {
      this.reusable = reusable;
      this.complete();
      reset(n);
    }

    /* inline */ override proc reset(nTasks: int) {
      maxBlockers = nTasks;
      blockers.write(0);
      outGate.reset();
      inGate.writeXF(0);
    }

    /* Block until `n` tasks have called this method.
     */
    /* inline */ override proc barrier() {
      on this {
        if boundsChecking && blockers.read() >= maxBlockers {
          HaltWrappers.boundsCheckHalt("Too many callers to barrier()");
        }
        inGate.readFF();
        var waiters = blockers.fetchAdd(1) + 1;

        if waiters == maxBlockers {
          inGate.reset();
          outGate.writeXF(0);
        } else {
          outGate.readFF();
        }

        if reusable {
          waiters = blockers.fetchSub(1) - 1;
          if waiters == 0 {
            outGate.reset();
            inGate.writeXF(0);
          }
        }
      }
    }

    /* Notify the barrier that this task has reached this point. */
    /* inline */ override proc notify() {
      on this {
        if boundsChecking && blockers.read() >= maxBlockers {
          HaltWrappers.boundsCheckHalt("Too many callers to notify()");
        }

        inGate.readFF();
        var waiters = blockers.fetchAdd(1) + 1;
        if waiters == maxBlockers {
          inGate.reset();
          outGate.writeXF(0);
        }
      }
    }

    /* Wait until `n` tasks have called :proc:`notify`. */
    /* inline */ override proc wait() {
      on this {
        outGate.readFF();
        if reusable {
          var waiters = blockers.fetchSub(1) - 1;
          if waiters == 0 {
            outGate.reset();
            inGate.writeXF(0);
          }
        }
      }
    }

    /* Return `true` if `n` tasks have called :proc:`notify`
     */
    /* inline */ override proc check(): bool {
      return outGate.isFull;
    }
  }

  @chpldoc.nodoc
  operator barrier.=(ref lhs: barrier, rhs: barrier) {
    if lhs.isowned {
      delete lhs.bar;
    }
    lhs.bar = rhs.bar;
    lhs.isowned = false;
  }

}

