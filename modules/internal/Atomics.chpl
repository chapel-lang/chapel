/*
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

/*
   See runtime/include/atomics/README for more information about atomics.

   Note that when compiling with --cache-remote, the compiler
   will add fences to methods in atomic types with order arguments e.g.::

     proc sub (... order:memory_order = memory_order_seq_cst):void {
       on this do atomic_fetch_sub_explicit_...(_v, value, order);
     }

   becomes::

     proc sub (... order:memory_order = memory_order_seq_cst):void {
       chpl_rmem_consist_maybe_release(order);
       on this do atomic_fetch_sub_explicit_...(_v, value, order);
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
   do need a thread fence after the loop of memory_order_relaxed
   transactions in order to correctly run in a comm=none compilation
   where the 'on' statement is omitted.
*/

/*
   A note about const-ness: these atomic types are records but
   in order for record assignment to work, the read() functions
   need to be able to work with a const RHS.

   To enable that, the read/peek/waitFor/writeThis functions take in `this`
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
   ``memory_order``. The ``order`` argument is used to specify the ordering
   constraints of atomic operations. The supported values are:

     * memory_order_relaxed
     * memory_order_acquire
     * memory_order_release
     * memory_order_acq_rel
     * memory_order_seq_cst
*/
pragma "atomic module"
module Atomics {

  use MemConsistency;
  use ChapelEnv;

  pragma "no doc"
  extern proc atomic_thread_fence(order:memory_order);
  pragma "no doc"
  extern proc atomic_signal_fence(order:memory_order);

  // these can be called just the way they are:
  //extern proc atomic_thread_fence(order:memory_order);
  //extern proc atomic_signal_fence(order:memory_order);
  // but they only handle the local portion of a fence.
  // To include PUTs or GETs in the fence, use atomic_fence instead:
  pragma "no doc"
  proc atomic_fence(order:memory_order = memory_order_seq_cst) {
    atomic_thread_fence(order);
    chpl_rmem_consist_fence(order);
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
    proc init() {
      extern externFunc("init", bool, explicit=false)
        proc atomic_init(ref obj:externT(bool), value:bool): void;

      this.complete();
      atomic_init(_v, _defaultOf(bool));
    }

    pragma "no doc"
    proc deinit() {
      extern externFunc("destroy", bool, explicit=false)
        proc atomic_destroy(ref obj:externT(bool)): void;

      on this do atomic_destroy(_v);
    }

    /*
       :returns: The stored value.
    */
    inline proc const read(order:memory_order = memory_order_seq_cst): bool {
      extern externFunc("load", bool)
        proc atomic_load(const ref obj:externT(bool), order:memory_order): bool;

      var ret:bool;
      on this do ret = atomic_load(_v, order);
      return ret;
    }

    /*
       Stores `value` as the new value.
    */
    inline proc write(value:bool, order:memory_order = memory_order_seq_cst): void {
      extern externFunc("store", bool)
        proc atomic_store(ref obj:externT(bool), value:bool, order:memory_order): void;

      on this do atomic_store(_v, value, order);
    }

    /*
       Stores `value` as the new value and returns the original value.
    */
    inline proc exchange(value:bool, order:memory_order = memory_order_seq_cst): bool {
      extern externFunc("exchange", bool)
        proc atomic_exchange(ref obj:externT(bool), value:bool, order:memory_order): bool;

      var ret:bool;
      on this do ret = atomic_exchange(_v, value, order);
      return ret;
    }

    /* Equivalent to :proc:`compareExchangeStrong` */
    inline proc compareExchange(expected:bool, desired:bool, order:memory_order = memory_order_seq_cst): bool {
      return this.compareExchangeStrong(expected, desired, order);
    }

    /*
       Similar to :proc:`compareExchangeStrong`, except that this function may
       return `false` even if the original value was equal to `expected`. This
       may happen if the value could not be updated atomically.
    */
    inline proc compareExchangeWeak(expected:bool, desired:bool, order:memory_order = memory_order_seq_cst): bool {
      extern externFunc("compare_exchange_weak", bool)
        proc atomic_compare_exchange_weak(ref obj:externT(bool), expected:bool, desired:bool, order:memory_order): bool;

      var ret:bool;
      on this do ret = atomic_compare_exchange_weak(_v, expected, desired, order);
      return ret;
    }

    /*
       Stores `desired` as the new value, if and only if the original value is
       equal to `expected`. Returns `true` if `desired` was stored.
    */
    inline proc compareExchangeStrong(expected:bool, desired:bool, order:memory_order = memory_order_seq_cst): bool {
      extern externFunc("compare_exchange_strong", bool)
        proc atomic_compare_exchange_strong(ref obj:externT(bool), expected:bool, desired:bool, order:memory_order): bool;

      var ret:bool;
      on this do ret = atomic_compare_exchange_strong(_v, expected, desired, order);
      return ret;
    }

    /*
       Stores `true` as the new value and returns the old value.
    */
    inline proc testAndSet(order:memory_order = memory_order_seq_cst): bool {
      return this.exchange(true, order);
    }

    /*
       Stores `false` as the new value.
    */
    inline proc clear(order:memory_order = memory_order_seq_cst): void {
      this.write(false, order);
    }

    /*
       :arg value: Value to compare against.

       Waits until the stored value is equal to `value`. The implementation may
       yield the running task while waiting.
    */
    inline proc const waitFor(value:bool, order:memory_order = memory_order_seq_cst): void {
      on this {
        while (this.read(order=memory_order_relaxed) != value) {
          chpl_task_yield();
        }
        atomic_thread_fence(order);
      }
    }

    /*
       :returns: Stored value using memory_order_relaxed.
    */
    inline proc const peek(): bool {
      return this.read(order=memory_order_relaxed);
    }

    /*
       Stores `value` as the new value using memory_order_relaxed.
    */
    inline proc poke(value:bool): void {
      this.write(value, order=memory_order_relaxed);
    }

    pragma "no doc"
    proc const writeThis(x) {
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
    proc init(type T) {
      extern externFunc("init", T, explicit=false)
        proc atomic_init(ref obj:externT(T), value:T): void;

      this.T = T;
      this.complete();
      atomic_init(_v, _defaultOf(T));
    }

    pragma "no doc"
    proc deinit() {
      extern externFunc("destroy", T, explicit=false)
        proc atomic_destroy(ref obj:externT(T)): void;

      on this do atomic_destroy(_v);
    }

    /*
       :returns: The stored value.
    */
    inline proc const read(order:memory_order = memory_order_seq_cst): T {
      extern externFunc("load", T)
        proc atomic_load(const ref obj:externT(T), order:memory_order): T;

      var ret:T;
      on this do ret = atomic_load(_v, order);
      return ret;
    }

    /*
       Stores `value` as the new value.
    */
    inline proc write(value:T, order:memory_order = memory_order_seq_cst): void {
      extern externFunc("store", T)
        proc atomic_store(ref obj:externT(T), value:T, order:memory_order): void;

      on this do atomic_store(_v, value, order);
    }

    /*
       Stores `value` as the new value and returns the original value.
    */
    inline proc exchange(value:T, order:memory_order = memory_order_seq_cst): T {
      extern externFunc("exchange", T)
        proc atomic_exchange(ref obj:externT(T), value:T, order:memory_order): T;

      var ret:T;
      on this do ret = atomic_exchange(_v, value, order);
      return ret;
    }

    /* Equivalent to :proc:`compareExchangeStrong` */
    inline proc compareExchange(expected:T, desired:T, order:memory_order = memory_order_seq_cst): bool {
      return this.compareExchangeStrong(expected, desired, order);
    }

    /*
       Similar to :proc:`compareExchangeStrong`, except that this function may
       return `false` even if the original value was equal to `expected`. This
       may happen if the value could not be updated atomically.
    */
    inline proc compareExchangeWeak(expected:T, desired:T, order:memory_order = memory_order_seq_cst): bool {
      extern externFunc("compare_exchange_weak", T)
        proc atomic_compare_exchange_weak(ref obj:externT(T), expected:T, desired:T, order:memory_order): bool;

      var ret:bool;
      on this do ret = atomic_compare_exchange_weak(_v, expected, desired, order);
      return ret;
    }

    /*
       Stores `desired` as the new value, if and only if the original value is
       equal to `expected`. Returns `true` if `desired` was stored.
    */
    inline proc compareExchangeStrong(expected:T, desired:T, order:memory_order = memory_order_seq_cst): bool {
      extern externFunc("compare_exchange_strong", T)
        proc atomic_compare_exchange_strong(ref obj:externT(T), expected:T, desired:T, order:memory_order): bool;

      var ret:bool;
      on this do ret = atomic_compare_exchange_strong(_v, expected, desired, order);
      return ret;
    }

    /*
       :returns: The original value.

       Adds `value` to the original value and stores the result. Defined for
       integer and real atomic types.
    */
    inline proc fetchAdd(value:T, order:memory_order = memory_order_seq_cst): T {
      extern externFunc("fetch_add", T)
        proc atomic_fetch_add(ref obj:externT(T), operand:T, order:memory_order): T;

      var ret:T;
      on this do ret = atomic_fetch_add(_v, value, order);
      return ret;
    }

    /*
       Adds `value` to the original value and stores the result. Defined for
       integer and real atomic types.
    */
    inline proc add(value:T, order:memory_order = memory_order_seq_cst): void {
      extern externFunc("fetch_add", T)
        proc atomic_fetch_add(ref obj:externT(T), operand:T, order:memory_order): T;

      on this do atomic_fetch_add(_v, value, order);
    }

    /*
       :returns: The original value.

       Subtracts `value` from the original value and stores the result. Defined
       for integer and real atomic types.
    */
    inline proc fetchSub(value:T, order:memory_order = memory_order_seq_cst): T {
      extern externFunc("fetch_sub", T)
        proc atomic_fetch_sub(ref obj:externT(T), operand:T, order:memory_order): T;

      var ret:T;
      on this do ret = atomic_fetch_sub(_v, value, order);
      return ret;
    }

    /*
       Subtracts `value` from the original value and stores the result. Defined
       for integer and real atomic types.
    */
    inline proc sub(value:T, order:memory_order = memory_order_seq_cst): void {
      extern externFunc("fetch_sub", T)
        proc atomic_fetch_sub(ref obj:externT(T), operand:T, order:memory_order): T;

      on this do atomic_fetch_sub(_v, value, order);
    }

    /*
       :returns: The original value.

       Applies the ``|`` operator to `value` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc fetchOr(value:T, order:memory_order = memory_order_seq_cst): T {
      if !isIntegral(T) then compilerError("fetchOr is only defined for integer atomic types");
      extern externFunc("fetch_or", T)
        proc atomic_fetch_or(ref obj:externT(T), operand:T, order:memory_order): T;

      var ret:T;
      on this do ret = atomic_fetch_or(_v, value, order);
      return ret;
    }

    /*
       Applies the ``|`` operator to `value` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc or(value:T, order:memory_order = memory_order_seq_cst): void {
      if !isIntegral(T) then compilerError("or is only defined for integer atomic types");
      extern externFunc("fetch_or", T)
        proc atomic_fetch_or(ref obj:externT(T), operand:T, order:memory_order): T;

      on this do atomic_fetch_or(_v, value, order);
    }

    /*
       :returns: The original value.

       Applies the ``&`` operator to `value` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc fetchAnd(value:T, order:memory_order = memory_order_seq_cst): T {
      if !isIntegral(T) then compilerError("fetchAnd is only defined for integer atomic types");
      extern externFunc("fetch_and", T)
        proc atomic_fetch_and(ref obj:externT(T), operand:T, order:memory_order): T;

      var ret:T;
      on this do ret = atomic_fetch_and(_v, value, order);
      return ret;
    }

    /*
       Applies the ``&`` operator to `value` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc and(value:T, order:memory_order = memory_order_seq_cst): void {
      if !isIntegral(T) then compilerError("and is only defined for integer atomic types");
      extern externFunc("fetch_and", T)
        proc atomic_fetch_and(ref obj:externT(T), operand:T, order:memory_order): T;

      on this do atomic_fetch_and(_v, value, order);
    }

    /*
       :returns: The original value.

       Applies the ``^`` operator to `value` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc fetchXor(value:T, order:memory_order = memory_order_seq_cst): T {
      if !isIntegral(T) then compilerError("fetchXor is only defined for integer atomic types");
      extern externFunc("fetch_xor", T)
        proc atomic_fetch_xor(ref obj:externT(T), operand:T, order:memory_order): T;

      var ret:T;
      on this do ret = atomic_fetch_xor(_v, value, order);
      return ret;
    }

    /*
       Applies the ``^`` operator to `value` and the original value, then stores
       the result.

       Only defined for integer atomic types.
    */
    inline proc xor(value:T, order:memory_order = memory_order_seq_cst): void {
      if !isIntegral(T) then compilerError("xor is only defined for integer atomic types");
      extern externFunc("fetch_xor", T)
        proc atomic_fetch_xor(ref obj:externT(T), operand:T, order:memory_order): T;

      on this do atomic_fetch_xor(_v, value, order);
    }

    /*
       Waits until the stored value is equal to `value`. The implementation may
       yield the running task while waiting.
    */
    inline proc const waitFor(value:T, order:memory_order = memory_order_seq_cst): void {
      on this {
        while (this.read(order=memory_order_relaxed) != value) {
          chpl_task_yield();
        }
        atomic_thread_fence(order);
      }
    }

    /*
       :returns: Stored value using memory_order_relaxed.
    */
    inline proc const peek(): T {
      return this.read(order=memory_order_relaxed);
    }

    /*
       Stores `value` as the new value using memory_order_relaxed.
    */
    inline proc poke(value:T): void {
      this.write(value, order=memory_order_relaxed);
    }

    pragma "no doc"
    proc const writeThis(x) {
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
