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
   See runtime/include/atomics/README for more information about atomics.

   Note that when compiling with --cache-remote, the compiler
   will add fences to methods in atomic types with order arguments e.g.::

     proc sub (... param order:memoryOrder = memoryOrder.seqCst):void {
       on this do atomic_fetch_sub_explicit_...(_v, value, c_memory_order(order));
     }

   becomes::

     proc sub (... param order:memoryOrder = memoryOrder.seqCst):void {
       chpl_rmem_consist_maybe_release(order);
       on this do atomic_fetch_sub_explicit_...(_v, value, c_memory_order(order));
       chpl_rmem_consist_maybe_acquire(order);
     }

   In addition, when --cache-remote is activated, the normally required
   memory fence for an 'on' statement is omitted for these functions
   (since the maybe_release/maybe_acquire fence takes care of it).

   These methods are detected based on the type of the order argument,
   but to avoid errors, the compiler will give an error if the
   order argument is not explicitly specified (because the relevant
   pass runs before function resolution).

   waitFor methods also do not need the fences written here -
   because they will be added if the cache is enabled - but they
   do need a thread fence after the loop of memoryOrder.relaxed
   transactions in order to correctly run in a comm=none compilation
   where the 'on' statement is omitted.
*/

/*
   A note about const-ness: these atomic types are records but
   in order for record assignment to work, the read() functions
   need to be able to work with a const RHS.

   To enable that, the read/waitFor/writeThis functions take in `this`
   with const intent. That is reasonable even if the atomic is
   implemented with a lock because the programmer can view it
   as constant, and on good hardware it really will be. If we change
   our mind about this, we will need to revisit the default `=`
   generated for records containing atomic fields.
*/

/*
   Atomic variables are variables that support atomic operations. Chapel
   currently supports atomic operations for bools, all supported sizes of
   signed and unsigned integers, as well as all supported sizes of reals.

   Most atomic methods accept an optional argument named ``order`` of type
   ``memoryOrder``. The ``order`` argument is used to specify the ordering
   constraints of atomic operations. The supported values are:

     * memoryOrder.relaxed
     * memoryOrder.acquire
     * memoryOrder.release
     * memoryOrder.acqRel
     * memoryOrder.seqCst
*/
pragma "atomic module"
module Atomics {

  private use ChapelBase;
  use MemConsistency;
  use ChapelEnv;

  pragma "no doc"
  extern proc chpl_atomic_thread_fence(order:memory_order);

  // non user-facing fence that is called by the compiler
  pragma "no doc"
  proc atomic_fence(order:memory_order = memory_order_seq_cst) {
    chpl_atomic_thread_fence(order);
    if CHPL_CACHE_REMOTE then
      chpl_rmem_consist_fence(order);
  }

  /* An atomic fence that establishes an ordering of non-atomic and relaxed
     atomic operations. */
  inline proc atomicFence(param order: memoryOrder = memoryOrder.seqCst) {
    atomic_fence(c_memory_order(order));
  }

  private proc isSupported(type T) param {
    return T == bool || isInt(T) || isUint(T) || isReal(T);
  }

  // Compute the C/Runtime type from the Chapel type
  // TODO support extern type renaming?
  private proc externT(type T) type {
    extern type atomic_bool;

    extern type atomic_int_least8_t;
    extern type atomic_int_least16_t;
    extern type atomic_int_least32_t;
    extern type atomic_int_least64_t;

    extern type atomic_uint_least8_t;
    extern type atomic_uint_least16_t;
    extern type atomic_uint_least32_t;
    extern type atomic_uint_least64_t;

    extern type atomic__real64;
    extern type atomic__real32;

    select T {
      when bool     do return atomic_bool;

      when int(8)   do return atomic_int_least8_t;
      when int(16)  do return atomic_int_least16_t;
      when int(32)  do return atomic_int_least32_t;
      when int(64)  do return atomic_int_least64_t;

      when uint(8)  do return atomic_uint_least8_t;
      when uint(16) do return atomic_uint_least16_t;
      when uint(32) do return atomic_uint_least32_t;
      when uint(64) do return atomic_uint_least64_t;

      when real(32) do return atomic__real32;
      when real(64) do return atomic__real64;
    }
  }

  private proc externTString(type T) param {
    if T == bool then return "bool";
    if isInt(T)  then return "int_least"  + numBits(T):string + "_t";
    if isUint(T) then return "uint_least" + numBits(T):string + "_t";
    if isReal(T) then return "_real"      + numBits(T):string;
  }

  private proc externFunc(param s: string, type T, param explicit=true) param {
    if explicit then return "atomic_" + s + "_explicit_" + externTString(T);
                else return "atomic_" + s + "_"          + externTString(T);
  }

  pragma "no doc"
  proc chpl__processorAtomicType(type T) type {
    if T == bool           then return AtomicBool;
    else if isSupported(T) then return AtomicT(T);
    else compilerError("Unsupported atomic type: " + T:string);
  }

  // Parser hook
  pragma "no doc"
  proc chpl__atomicType(type T) type {
    if CHPL_NETWORK_ATOMICS == "none" {
      return chpl__processorAtomicType(T);
    } else {
      return chpl__networkAtomicType(T);
    }
  }

  pragma "atomic type"
  pragma "ignore noinit"
  record AtomicBool {
    pragma "no doc"
    var _v:externT(bool);

    pragma "no doc"
    proc init_helper(value:bool) {
      pragma "fn synchronization free"
      extern externFunc("init", bool, explicit=false)
        proc atomic_init(ref obj:externT(bool), value:bool): void;

      atomic_init(_v, value);
    }

    pragma "no doc"
    proc init() {
      this.complete();
      const default: bool;
      init_helper(default);
    }

    pragma "no doc"
    proc init=(other:AtomicBool) {
      this.complete();
      init_helper(other.read());
    }

    pragma "no doc"
    proc init=(other:bool) {
      this.complete();
      init_helper(other);
    }

    pragma "no doc"
    proc deinit() {
      pragma "fn synchronization free"
      extern externFunc("destroy", bool, explicit=false)
        proc atomic_destroy(ref obj:externT(bool)): void;

      on this do atomic_destroy(_v);
    }

    /*
       :returns: The stored value.
    */
    inline proc const read(param order: memoryOrder = memoryOrder.seqCst): bool {
      extern externFunc("load", bool)
        proc atomic_load(const ref obj:externT(bool), order:memory_order): bool;

      var ret:bool;
      on this do ret = atomic_load(_v, c_memory_order(order));
      return ret;
    }

    /*
       Stores `value` as the new value.
    */
    inline proc write(value:bool, param order: memoryOrder = memoryOrder.seqCst): void {
      extern externFunc("store", bool)
        proc atomic_store(ref obj:externT(bool), value:bool, order:memory_order): void;

      on this do atomic_store(_v, value, c_memory_order(order));
    }

    /*
       Stores `value` as the new value and returns the original value.
    */
    inline proc exchange(value:bool, param order: memoryOrder = memoryOrder.seqCst): bool {
      extern externFunc("exchange", bool)
        proc atomic_exchange(ref obj:externT(bool), value:bool, order:memory_order): bool;

      var ret:bool;
      on this do ret = atomic_exchange(_v, value, c_memory_order(order));
      return ret;
    }

    /*
       Stores `desired` as the new value, if and only if the original value is
       equal to `expected`. Returns `true` if `desired` was stored.
    */
    inline proc compareAndSwap(expected:bool, desired:bool, param order: memoryOrder = memoryOrder.seqCst): bool {
      extern externFunc("compare_exchange_strong", bool)
        proc atomic_compare_exchange_strong(ref obj:externT(bool), expected:bool, desired:bool, order:memory_order): bool;

      var ret:bool;
      on this do ret = atomic_compare_exchange_strong(_v, expected, desired, c_memory_order(order));
      return ret;
    }

    /*
       Stores `true` as the new value and returns the old value.
    */
    inline proc testAndSet(param order: memoryOrder = memoryOrder.seqCst): bool {
      return this.exchange(true, order);
    }

    /*
       Stores `false` as the new value.
    */
    inline proc clear(param order: memoryOrder = memoryOrder.seqCst): void {
      this.write(false, order);
    }

    /*
       :arg value: Value to compare against.

       Waits until the stored value is equal to `value`. The implementation may
       yield the running task while waiting.
    */
    inline proc const waitFor(value:bool, param order: memoryOrder = memoryOrder.seqCst): void {
      on this {
        while (this.read(order=memoryOrder.relaxed) != value) {
          chpl_task_yield();
        }
        chpl_atomic_thread_fence(c_memory_order(order));
      }
    }

    pragma "no doc"
    proc const writeThis(x) throws {
      x <~> read();
    }

  }

  pragma "atomic type"
  pragma "ignore noinit"
  record AtomicT {
    pragma "no doc"
    type T;

    pragma "no doc"
    var _v:externT(T);

    pragma "no doc"
    proc init_helper(value:T) {
      pragma "fn synchronization free"
      extern externFunc("init", T, explicit=false)
        proc atomic_init(ref obj:externT(T), value:T): void;

      atomic_init(_v, value);
    }

    pragma "no doc"
    proc init(type T) {
      this.T = T;
      this.complete();
      const default: T;
      init_helper(default);
    }

    pragma "no doc"
    proc init=(other:this.type) {
      this.T = other.T;
      this.complete();
      init_helper(other.read());
    }

    pragma "no doc"
    proc init=(other:this.type.T) {
      this.T = other.type;
      this.complete();
      init_helper(other);
    }

    pragma "no doc"
    proc deinit() {
      pragma "fn synchronization free"
      extern externFunc("destroy", T, explicit=false)
        proc atomic_destroy(ref obj:externT(T)): void;

      on this do atomic_destroy(_v);
    }

    /*
       :returns: The stored value.
    */
    inline proc const read(param order: memoryOrder = memoryOrder.seqCst): T {
      extern externFunc("load", T)
        proc atomic_load(const ref obj:externT(T), order:memory_order): T;

      var ret:T;
      on this do ret = atomic_load(_v, c_memory_order(order));
      return ret;
    }

    /*
       Stores `value` as the new value.
    */
    inline proc write(value:T, param order: memoryOrder = memoryOrder.seqCst): void {
      extern externFunc("store", T)
        proc atomic_store(ref obj:externT(T), value:T, order:memory_order): void;

      on this do atomic_store(_v, value, c_memory_order(order));
    }

    /*
       Stores `value` as the new value and returns the original value.
    */
    inline proc exchange(value:T, param order: memoryOrder = memoryOrder.seqCst): T {
      extern externFunc("exchange", T)
        proc atomic_exchange(ref obj:externT(T), value:T, order:memory_order): T;

      var ret:T;
      on this do ret = atomic_exchange(_v, value, c_memory_order(order));
      return ret;
    }

    /*
       Stores `desired` as the new value, if and only if the original value is
       equal to `expected`. Returns `true` if `desired` was stored.
    */
    inline proc compareAndSwap(expected:T, desired:T, param order: memoryOrder = memoryOrder.seqCst): bool {
      extern externFunc("compare_exchange_strong", T)
        proc atomic_compare_exchange_strong(ref obj:externT(T), expected:T, desired:T, order:memory_order): bool;

      var ret:bool;
      on this do ret = atomic_compare_exchange_strong(_v, expected, desired, c_memory_order(order));
      return ret;
    }

    /*
       :returns: The original value.

       Adds `value` to the original value and stores the result. Defined for
       integer and real atomic types.
    */
    inline proc fetchAdd(value:T, param order: memoryOrder = memoryOrder.seqCst): T {
      extern externFunc("fetch_add", T)
        proc atomic_fetch_add(ref obj:externT(T), operand:T, order:memory_order): T;

      var ret:T;
      on this do ret = atomic_fetch_add(_v, value, c_memory_order(order));
      return ret;
    }

    /*
       Adds `value` to the original value and stores the result. Defined for
       integer and real atomic types.
    */
    inline proc add(value:T, param order: memoryOrder = memoryOrder.seqCst): void {
      extern externFunc("fetch_add", T)
        proc atomic_fetch_add(ref obj:externT(T), operand:T, order:memory_order): T;

      on this do atomic_fetch_add(_v, value, c_memory_order(order));
    }

    /*
       :returns: The original value.

       Subtracts `value` from the original value and stores the result. Defined
       for integer and real atomic types.
    */
    inline proc fetchSub(value:T, param order: memoryOrder = memoryOrder.seqCst): T {
      extern externFunc("fetch_sub", T)
        proc atomic_fetch_sub(ref obj:externT(T), operand:T, order:memory_order): T;

      var ret:T;
      on this do ret = atomic_fetch_sub(_v, value, c_memory_order(order));
      return ret;
    }

    /*
       Subtracts `value` from the original value and stores the result. Defined
       for integer and real atomic types.
    */
    inline proc sub(value:T, param order: memoryOrder = memoryOrder.seqCst): void {
      extern externFunc("fetch_sub", T)
        proc atomic_fetch_sub(ref obj:externT(T), operand:T, order:memory_order): T;

      on this do atomic_fetch_sub(_v, value, c_memory_order(order));
    }

    /*
       :returns: The original value.

       Applies the ``|`` operator to `value` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc fetchOr(value:T, param order: memoryOrder = memoryOrder.seqCst): T {
      if !isIntegral(T) then compilerError("fetchOr is only defined for integer atomic types");
      extern externFunc("fetch_or", T)
        proc atomic_fetch_or(ref obj:externT(T), operand:T, order:memory_order): T;

      var ret:T;
      on this do ret = atomic_fetch_or(_v, value, c_memory_order(order));
      return ret;
    }

    /*
       Applies the ``|`` operator to `value` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc or(value:T, param order: memoryOrder = memoryOrder.seqCst): void {
      if !isIntegral(T) then compilerError("or is only defined for integer atomic types");
      extern externFunc("fetch_or", T)
        proc atomic_fetch_or(ref obj:externT(T), operand:T, order:memory_order): T;

      on this do atomic_fetch_or(_v, value, c_memory_order(order));
    }

    /*
       :returns: The original value.

       Applies the ``&`` operator to `value` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc fetchAnd(value:T, param order: memoryOrder = memoryOrder.seqCst): T {
      if !isIntegral(T) then compilerError("fetchAnd is only defined for integer atomic types");
      extern externFunc("fetch_and", T)
        proc atomic_fetch_and(ref obj:externT(T), operand:T, order:memory_order): T;

      var ret:T;
      on this do ret = atomic_fetch_and(_v, value, c_memory_order(order));
      return ret;
    }

    /*
       Applies the ``&`` operator to `value` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc and(value:T, param order: memoryOrder = memoryOrder.seqCst): void {
      if !isIntegral(T) then compilerError("and is only defined for integer atomic types");
      extern externFunc("fetch_and", T)
        proc atomic_fetch_and(ref obj:externT(T), operand:T, order:memory_order): T;

      on this do atomic_fetch_and(_v, value, c_memory_order(order));
    }

    /*
       :returns: The original value.

       Applies the ``^`` operator to `value` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc fetchXor(value:T, param order: memoryOrder = memoryOrder.seqCst): T {
      if !isIntegral(T) then compilerError("fetchXor is only defined for integer atomic types");
      extern externFunc("fetch_xor", T)
        proc atomic_fetch_xor(ref obj:externT(T), operand:T, order:memory_order): T;

      var ret:T;
      on this do ret = atomic_fetch_xor(_v, value, c_memory_order(order));
      return ret;
    }

    /*
       Applies the ``^`` operator to `value` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc xor(value:T, param order: memoryOrder = memoryOrder.seqCst): void {
      if !isIntegral(T) then compilerError("xor is only defined for integer atomic types");
      extern externFunc("fetch_xor", T)
        proc atomic_fetch_xor(ref obj:externT(T), operand:T, order:memory_order): T;

      on this do atomic_fetch_xor(_v, value, c_memory_order(order));
    }

    /*
       Waits until the stored value is equal to `value`. The implementation may
       yield the running task while waiting.
    */
    inline proc const waitFor(value:T, param order: memoryOrder = memoryOrder.seqCst): void {
      on this {
        while (this.read(order=memoryOrder.relaxed) != value) {
          chpl_task_yield();
        }
        chpl_atomic_thread_fence(c_memory_order(order));
      }
    }

    pragma "no doc"
    proc const writeThis(x) throws {
      x <~> read();
    }

  }

  //
  // For the first cut, we will be making assignment and other normal
  // operations illegal.  In addition, we are also punting on
  // the default argument intent since it's not obvious what it
  // should be.
  //

  // We need to explicitly define these for all types because the atomic
  // types are records and unless explicitly defined, it will resolve
  // to the normal record version of the function.  Sigh.
  inline proc =(ref a:AtomicBool, const ref b:AtomicBool) {
    a.write(b.read());
  }
  inline proc =(ref a:AtomicBool, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  inline proc =(ref a:AtomicT, const ref b:AtomicT) {
    a.write(b.read());
  }
  inline proc =(ref a:AtomicT, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  inline proc +(a:AtomicT, b) {
    compilerError("Cannot directly add atomic variables");
    return a;
  }
  inline proc -(a:AtomicT, b) {
    compilerError("Cannot directly subtract atomic variables");
    return a;
  }
  inline proc *(a:AtomicT, b) {
    compilerError("Cannot directly multiply atomic variables");
    return a;
  }
  inline proc /(a:AtomicT, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc %(a:AtomicT, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
}
