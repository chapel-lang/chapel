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

/*
   .. warning::
     This module represents work in progress. The API is unstable and likely to
     change over time.

   This module provides buffered versions of non-fetching atomic operations for
   all ``int``, ``uint``, and ``real`` types.  Buffered versions of
   :proc:`~Atomics.add()`, :proc:`~Atomics.sub()`, :proc:`~Atomics.or()`,
   :proc:`~Atomics.and()`, and :proc:`~Atomics.xor()` are provided. These
   variants are internally buffered and the buffers are flushed implicitly when
   full or explicitly with :proc:`flushAtomicBuff()`. These buffered operations
   can provide a significant speedup for bulk atomic operations that do not
   require strict ordering of operations:

   .. code-block:: chapel

     use BufferedAtomics;

     const numTasksPerLocale = here.maxTaskPar,
           iters = 10000;


     var a: atomic int;

     coforall loc in Locales do on loc do
       coforall 1..numTasksPerLocale do
         for i in 1..iters do
           a.addBuff(i);                   // buffered atomic add

     flushAtomicBuff();                    // flush any pending operations (required)


     const itersSum = iters*(iters+1)/2,   // sum from 1..iters
           numTasks = numLocales * numTasksPerLocale;
     assert(a.read() == numTasks * itersSum);

   It's important to be aware that buffered atomic operations are not
   consistent with regular atomic operations and updates may not be visible
   until the buffers are explicitly flushed with :proc:`flushAtomicBuff()`.

   .. code-block:: chapel

     var a: atomic int;
     a.addBuff(1);
     writeln(a);        // can print 0 or 1
     flushAtomicBuff();
     writeln(a);        // prints 1

   Generally speaking they are useful for when you have a large batch of atomic
   updates to perform and the order of those operations doesn't matter.

   .. note::
     Currently, these are only optimized for ``CHPL_NETWORK_ATOMICS=ugni``.
     Processor atomics or any other implementation falls back to non-buffered
     operations. Under ugni these operations are internally buffered. When the
     buffers are flushed, the operations are performed all at once. Cray Linux
     Environment (CLE) 5.2.UP04 or newer is required for best performance. In
     our experience, buffered atomics can achieve up to a 5X performance
     improvement over non-buffered atomics for CLE 5.2UP04 or newer and up to a
     2.5X improvement for older versions of CLE.
 */
module BufferedAtomics {

  private proc externFunc(param s: string, type T) param {
    if isInt(T)  then return "chpl_comm_atomic_" + s + "_int"  + numBits(T):string;
    if isUint(T) then return "chpl_comm_atomic_" + s + "_uint" + numBits(T):string;
    if isReal(T) then return "chpl_comm_atomic_" + s + "_real" + numBits(T):string;
  }

  /* Buffered atomic add. */
  inline proc AtomicT.addBuff(value:T): void {
    this.add(value);
  }
  pragma "no doc"
  inline proc RAtomicT.addBuff(value:T): void {
    pragma "insert line file info" extern externFunc("add_buff", T)
      proc atomic_add_buff(ref op:T, l:int(32), obj:c_void_ptr): void;

    var v = value;
    atomic_add_buff(v, _localeid(), _addr());
  }

  /* Buffered atomic sub. */
  inline proc AtomicT.subBuff(value:T): void {
    this.sub(value);
  }
  pragma "no doc"
  inline proc RAtomicT.subBuff(value:T): void {
    pragma "insert line file info" extern externFunc("sub_buff", T)
      proc atomic_sub_buff(ref op:T, l:int(32), obj:c_void_ptr): void;

    var v = value;
    atomic_sub_buff(v, _localeid(), _addr());
  }

  /* Buffered atomic or. */
  inline proc AtomicT.orBuff(value:T): void {
    this.or(value);
  }
  pragma "no doc"
  inline proc RAtomicT.orBuff(value:T): void {
    if !isIntegral(T) then compilerError("or is only defined for integer atomic types");
    pragma "insert line file info" extern externFunc("or_buff", T)
      proc atomic_or_buff(ref op:T, l:int(32), obj:c_void_ptr): void;

    var v = value;
    atomic_or_buff(v, _localeid(), _addr());
  }

  /* Buffered atomic and. */
  inline proc AtomicT.andBuff(value:T): void {
    this.and(value);
  }
  pragma "no doc"
  inline proc RAtomicT.andBuff(value:T): void {
    if !isIntegral(T) then compilerError("and is only defined for integer atomic types");
    pragma "insert line file info" extern externFunc("and_buff", T)
      proc atomic_and_buff(ref op:T, l:int(32), obj:c_void_ptr): void;

    var v = value;
    atomic_and_buff(v, _localeid(), _addr());
  }

  /* Buffered atomic xor. */
  inline proc AtomicT.xorBuff(value:T): void {
    this.xor(value);
  }
  pragma "no doc"
  inline proc RAtomicT.xorBuff(value:T): void {
    if !isIntegral(T) then compilerError("xor is only defined for integer atomic types");
    pragma "insert line file info" extern externFunc("xor_buff", T)
      proc atomic_xor_buff(ref op:T, l:int(32), obj:c_void_ptr): void;

    var v = value;
    atomic_xor_buff(v, _localeid(), _addr());
  }

  /*
     Flush any atomic operations that are still buffered. Note that this
     flushes any pending operations on all locales, not just the current
     locale.
   */
  inline proc flushAtomicBuff(): void {
    if CHPL_NETWORK_ATOMICS != "none" {
      extern proc chpl_comm_atomic_buff_flush();
      coforall loc in Locales do on loc {
        chpl_comm_atomic_buff_flush();
      }
    }
  }
}
