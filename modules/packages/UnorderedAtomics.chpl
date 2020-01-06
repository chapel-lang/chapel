/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

/*
   .. warning::
     This module represents work in progress. The API is unstable and likely to
     change over time.

   This module provides unordered versions of non-fetching atomic operations for
   all ``int``, ``uint``, and ``real`` types.  Unordered versions of
   :proc:`~Atomics.add()`, :proc:`~Atomics.sub()`, :proc:`~Atomics.or()`,
   :proc:`~Atomics.and()`, and :proc:`~Atomics.xor()` are provided. The results
   of these functions are not visible until task or forall termination or an
   explicit :proc:`unorderedAtomicTaskFence()`, but they can provide a
   significant speedup for bulk atomic operations that do not require ordering:

   .. code-block:: chapel

     use UnorderedAtomics;

     const numTasksPerLocale = here.maxTaskPar,
           iters = 10000;

     var a: atomic int;

     coforall loc in Locales do on loc do
       coforall 1..numTasksPerLocale do
         for i in 1..iters do
           a.unorderedAdd(i); // unordered atomic add

     // no fence required, fenced at task termination

     const itersSum = iters*(iters+1)/2, // sum from 1..iters
           numTasks = numLocales * numTasksPerLocale;
     assert(a.read() == numTasks * itersSum);

   It's important to be aware that unordered atomic operations are not
   consistent with regular atomic operations and updates may not be visible
   until the task or forall that issued them terminates or they are explicitly
   fenced with :proc:`unorderedAtomicTaskFence()`.

   .. code-block:: chapel

     var a: atomic int;
     a.unorderedAdd(1);
     writeln(a);        // can print 0 or 1
     unorderedAtomicTaskFence();
     writeln(a);        // prints 1

   Generally speaking they are useful for when you have a large batch of atomic
   updates to perform and the order of those operations doesn't matter.

   .. note::
     Currently, these are only optimized for ``CHPL_NETWORK_ATOMICS=ugni``.
     Processor atomics or any other implementation falls back to ordered
     operations. Under ugni these operations are internally buffered. When the
     buffers are flushed, the operations are performed all at once. Cray Linux
     Environment (CLE) 5.2.UP04 or newer is required for best performance. In
     our experience, unordered atomics can achieve up to a 5X performance
     improvement over ordered atomics for CLE 5.2UP04 or newer.
 */
module UnorderedAtomics {

  private proc externFunc(param s: string, type T) param {
    if isInt(T)  then return "chpl_comm_atomic_" + s + "_int"  + numBits(T):string;
    if isUint(T) then return "chpl_comm_atomic_" + s + "_uint" + numBits(T):string;
    if isReal(T) then return "chpl_comm_atomic_" + s + "_real" + numBits(T):string;
  }

  /* Unordered atomic add. */
  inline proc AtomicT.unorderedAdd(value:T): void {
    this.add(value);
  }
  pragma "no doc"
  inline proc RAtomicT.unorderedAdd(value:T): void {
    pragma "insert line file info" extern externFunc("add_unordered", T)
      proc atomic_add_unordered(ref op:T, l:int(32), obj:c_void_ptr): void;

    var v = value;
    atomic_add_unordered(v, _localeid(), _addr());
  }

  /* Unordered atomic sub. */
  inline proc AtomicT.unorderedSub(value:T): void {
    this.sub(value);
  }
  pragma "no doc"
  inline proc RAtomicT.unorderedSub(value:T): void {
    pragma "insert line file info" extern externFunc("sub_unordered", T)
      proc atomic_sub_unordered(ref op:T, l:int(32), obj:c_void_ptr): void;

    var v = value;
    atomic_sub_unordered(v, _localeid(), _addr());
  }

  /* Unordered atomic or. */
  inline proc AtomicT.unorderedOr(value:T): void {
    this.or(value);
  }
  pragma "no doc"
  inline proc RAtomicT.unorderedOr(value:T): void {
    if !isIntegral(T) then compilerError("or is only defined for integer atomic types");
    pragma "insert line file info" extern externFunc("or_unordered", T)
      proc atomic_or_unordered(ref op:T, l:int(32), obj:c_void_ptr): void;

    var v = value;
    atomic_or_unordered(v, _localeid(), _addr());
  }

  /* Unordered atomic and. */
  inline proc AtomicT.unorderedAnd(value:T): void {
    this.and(value);
  }
  pragma "no doc"
  inline proc RAtomicT.unorderedAnd(value:T): void {
    if !isIntegral(T) then compilerError("and is only defined for integer atomic types");
    pragma "insert line file info" extern externFunc("and_unordered", T)
      proc atomic_and_unordered(ref op:T, l:int(32), obj:c_void_ptr): void;

    var v = value;
    atomic_and_unordered(v, _localeid(), _addr());
  }

  /* Unordered atomic xor. */
  inline proc AtomicT.unorderedXor(value:T): void {
    this.xor(value);
  }
  pragma "no doc"
  inline proc RAtomicT.unorderedXor(value:T): void {
    if !isIntegral(T) then compilerError("xor is only defined for integer atomic types");
    pragma "insert line file info" extern externFunc("xor_unordered", T)
      proc atomic_xor_unordered(ref op:T, l:int(32), obj:c_void_ptr): void;

    var v = value;
    atomic_xor_unordered(v, _localeid(), _addr());
  }

  /*
     Fence any pending unordered atomics issued by the current task.
   */
  inline proc unorderedAtomicTaskFence(): void {
    if CHPL_NETWORK_ATOMICS != "none" {
      extern proc chpl_comm_atomic_unordered_task_fence();
      chpl_comm_atomic_unordered_task_fence();
    }
  }
}
