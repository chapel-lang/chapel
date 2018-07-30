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

  extern type atomic_int_least8_t;
  extern type atomic_int_least16_t;
  extern type atomic_int_least32_t;
  extern type atomic_uint_least8_t;
  extern type atomic_uint_least16_t;
  extern type atomic_uint_least32_t;
  extern type atomic_uint_least64_t;
  extern type atomic_uintptr_t;

  extern type atomic__real64;
  extern type atomic__real32;

  extern proc atomic_thread_fence(order:memory_order);
  extern proc atomic_signal_fence(order:memory_order);


  extern proc atomic_is_lock_free_uint_least8_t(ref obj:atomic_uint_least8_t):bool;
  extern proc atomic_init_uint_least8_t(ref obj:atomic_uint_least8_t, value:uint(8));
  extern proc atomic_destroy_uint_least8_t(ref obj:atomic_uint_least8_t);
  extern proc atomic_store_explicit_uint_least8_t(ref obj:atomic_uint_least8_t, value:uint(8), order:memory_order);
  extern proc atomic_load_explicit_uint_least8_t(const ref obj:atomic_uint_least8_t, order:memory_order):uint(8);
  extern proc atomic_exchange_explicit_uint_least8_t(ref obj:atomic_uint_least8_t, value:uint(8), order:memory_order):uint(8);
  extern proc atomic_compare_exchange_strong_explicit_uint_least8_t(ref obj:atomic_uint_least8_t, expected:uint(8), desired:uint(8), order:memory_order):bool;
  extern proc atomic_compare_exchange_weak_explicit_uint_least8_t(ref obj:atomic_uint_least8_t, expected:uint(8), desired:uint(8), order:memory_order):bool;
  extern proc atomic_fetch_add_explicit_uint_least8_t(ref obj:atomic_uint_least8_t, operand:uint(8), order:memory_order):uint(8);
  extern proc atomic_fetch_sub_explicit_uint_least8_t(ref obj:atomic_uint_least8_t, operand:uint(8), order:memory_order):uint(8);
  extern proc atomic_fetch_or_explicit_uint_least8_t(ref obj:atomic_uint_least8_t, operand:uint(8), order:memory_order):uint(8);
  extern proc atomic_fetch_and_explicit_uint_least8_t(ref obj:atomic_uint_least8_t, operand:uint(8), order:memory_order):uint(8);
  extern proc atomic_fetch_xor_explicit_uint_least8_t(ref obj:atomic_uint_least8_t, operand:uint(8), order:memory_order):uint(8);

  extern proc atomic_is_lock_free_uint_least16_t(ref obj:atomic_uint_least16_t):bool;
  extern proc atomic_init_uint_least16_t(ref obj:atomic_uint_least16_t, value:uint(16));
  extern proc atomic_destroy_uint_least16_t(ref obj:atomic_uint_least16_t);
  extern proc atomic_store_explicit_uint_least16_t(ref obj:atomic_uint_least16_t, value:uint(16), order:memory_order);
  extern proc atomic_load_explicit_uint_least16_t(const ref obj:atomic_uint_least16_t, order:memory_order):uint(16);
  extern proc atomic_exchange_explicit_uint_least16_t(ref obj:atomic_uint_least16_t, value:uint(16), order:memory_order):uint(16);
  extern proc atomic_compare_exchange_strong_explicit_uint_least16_t(ref obj:atomic_uint_least16_t, expected:uint(16), desired:uint(16), order:memory_order):bool;
  extern proc atomic_compare_exchange_weak_explicit_uint_least16_t(ref obj:atomic_uint_least16_t, expected:uint(16), desired:uint(16), order:memory_order):bool;
  extern proc atomic_fetch_add_explicit_uint_least16_t(ref obj:atomic_uint_least16_t, operand:uint(16), order:memory_order):uint(16);
  extern proc atomic_fetch_sub_explicit_uint_least16_t(ref obj:atomic_uint_least16_t, operand:uint(16), order:memory_order):uint(16);
  extern proc atomic_fetch_or_explicit_uint_least16_t(ref obj:atomic_uint_least16_t, operand:uint(16), order:memory_order):uint(16);
  extern proc atomic_fetch_and_explicit_uint_least16_t(ref obj:atomic_uint_least16_t, operand:uint(16), order:memory_order):uint(16);
  extern proc atomic_fetch_xor_explicit_uint_least16_t(ref obj:atomic_uint_least16_t, operand:uint(16), order:memory_order):uint(16);


  extern proc atomic_is_lock_free_uint_least32_t(ref obj:atomic_uint_least32_t):bool;
  extern proc atomic_init_uint_least32_t(ref obj:atomic_uint_least32_t, value:uint(32));
  extern proc atomic_destroy_uint_least32_t(ref obj:atomic_uint_least32_t);
  extern proc atomic_store_explicit_uint_least32_t(ref obj:atomic_uint_least32_t, value:uint(32), order:memory_order);
  extern proc atomic_load_explicit_uint_least32_t(const ref obj:atomic_uint_least32_t, order:memory_order):uint(32);
  extern proc atomic_exchange_explicit_uint_least32_t(ref obj:atomic_uint_least32_t, value:uint(32), order:memory_order):uint(32);
  extern proc atomic_compare_exchange_strong_explicit_uint_least32_t(ref obj:atomic_uint_least32_t, expected:uint(32), desired:uint(32), order:memory_order):bool;
  extern proc atomic_compare_exchange_weak_explicit_uint_least32_t(ref obj:atomic_uint_least32_t, expected:uint(32), desired:uint(32), order:memory_order):bool;
  extern proc atomic_fetch_add_explicit_uint_least32_t(ref obj:atomic_uint_least32_t, operand:uint(32), order:memory_order):uint(32);
  extern proc atomic_fetch_sub_explicit_uint_least32_t(ref obj:atomic_uint_least32_t, operand:uint(32), order:memory_order):uint(32);
  extern proc atomic_fetch_or_explicit_uint_least32_t(ref obj:atomic_uint_least32_t, operand:uint(32), order:memory_order):uint(32);
  extern proc atomic_fetch_and_explicit_uint_least32_t(ref obj:atomic_uint_least32_t, operand:uint(32), order:memory_order):uint(32);
  extern proc atomic_fetch_xor_explicit_uint_least32_t(ref obj:atomic_uint_least32_t, operand:uint(32), order:memory_order):uint(32);

  extern proc atomic_is_lock_free_uint_least64_t(ref obj:atomic_uint_least64_t):bool;
  extern proc atomic_init_uint_least64_t(ref obj:atomic_uint_least64_t, value:uint(64));
  extern proc atomic_destroy_uint_least64_t(ref obj:atomic_uint_least64_t);
  extern proc atomic_store_explicit_uint_least64_t(ref obj:atomic_uint_least64_t, value:uint(64), order:memory_order);
  extern proc atomic_load_explicit_uint_least64_t(const ref obj:atomic_uint_least64_t, order:memory_order):uint(64);
  extern proc atomic_exchange_explicit_uint_least64_t(ref obj:atomic_uint_least64_t, value:uint(64), order:memory_order):uint(64);
  extern proc atomic_compare_exchange_strong_explicit_uint_least64_t(ref obj:atomic_uint_least64_t, expected:uint(64), desired:uint(64), order:memory_order):bool;
  extern proc atomic_compare_exchange_weak_explicit_uint_least64_t(ref obj:atomic_uint_least64_t, expected:uint(64), desired:uint(64), order:memory_order):bool;
  extern proc atomic_fetch_add_explicit_uint_least64_t(ref obj:atomic_uint_least64_t, operand:uint(64), order:memory_order):uint(64);
  extern proc atomic_fetch_sub_explicit_uint_least64_t(ref obj:atomic_uint_least64_t, operand:uint(64), order:memory_order):uint(64);
  extern proc atomic_fetch_or_explicit_uint_least64_t(ref obj:atomic_uint_least64_t, operand:uint(64), order:memory_order):uint(64);
  extern proc atomic_fetch_and_explicit_uint_least64_t(ref obj:atomic_uint_least64_t, operand:uint(64), order:memory_order):uint(64);
  extern proc atomic_fetch_xor_explicit_uint_least64_t(ref obj:atomic_uint_least64_t, operand:uint(64), order:memory_order):uint(64);

  extern proc atomic_is_lock_free_uintptr_t(ref obj:atomic_uintptr_t):bool;
  extern proc atomic_init_uintptr_t(ref obj:atomic_uintptr_t, value:c_void_ptr);
  extern proc atomic_destroy_uintptr_t(ref obj:atomic_uintptr_t);
  extern proc atomic_store_explicit_uintptr_t(ref obj:atomic_uintptr_t, value:c_void_ptr, order:memory_order);
  extern proc atomic_load_explicit_uintptr_t(const ref obj:atomic_uintptr_t, order:memory_order):c_void_ptr;
  extern proc atomic_exchange_explicit_uintptr_t(ref obj:atomic_uintptr_t, value:c_void_ptr, order:memory_order):c_void_ptr;
  extern proc atomic_compare_exchange_strong_explicit_uintptr_t(ref obj:atomic_uintptr_t, expected:c_void_ptr, desired:c_void_ptr, order:memory_order):bool;
  extern proc atomic_compare_exchange_weak_explicit_uintptr_t(ref obj:atomic_uintptr_t, expected:c_void_ptr, desired:c_void_ptr, order:memory_order):bool;
  extern proc atomic_fetch_add_explicit_uintptr_t(ref obj:atomic_uintptr_t, operand:c_void_ptr, order:memory_order):c_void_ptr;
  extern proc atomic_fetch_sub_explicit_uintptr_t(ref obj:atomic_uintptr_t, operand:c_void_ptr, order:memory_order):c_void_ptr;
  extern proc atomic_fetch_or_explicit_uintptr_t(ref obj:atomic_uintptr_t, operand:c_void_ptr, order:memory_order):c_void_ptr;
  extern proc atomic_fetch_and_explicit_uintptr_t(ref obj:atomic_uintptr_t, operand:c_void_ptr, order:memory_order):c_void_ptr;
  extern proc atomic_fetch_xor_explicit_uintptr_t(ref obj:atomic_uintptr_t, operand:c_void_ptr, order:memory_order):c_void_ptr;

  extern proc atomic_is_lock_free_int_least8_t(ref obj:atomic_int_least8_t):bool;
  extern proc atomic_init_int_least8_t(ref obj:atomic_int_least8_t, value:int(8));
  extern proc atomic_destroy_int_least8_t(ref obj:atomic_int_least8_t);
  extern proc atomic_store_explicit_int_least8_t(ref obj:atomic_int_least8_t, value:int(8), order:memory_order);
  extern proc atomic_load_explicit_int_least8_t(const ref obj:atomic_int_least8_t, order:memory_order):int(8);
  extern proc atomic_exchange_explicit_int_least8_t(ref obj:atomic_int_least8_t, value:int(8), order:memory_order):int(8);
  extern proc atomic_compare_exchange_strong_explicit_int_least8_t(ref obj:atomic_int_least8_t, expected:int(8), desired:int(8), order:memory_order):bool;
  extern proc atomic_compare_exchange_weak_explicit_int_least8_t(ref obj:atomic_int_least8_t, expected:int(8), desired:int(8), order:memory_order):bool;
  extern proc atomic_fetch_add_explicit_int_least8_t(ref obj:atomic_int_least8_t, operand:int(8), order:memory_order):int(8);
  extern proc atomic_fetch_sub_explicit_int_least8_t(ref obj:atomic_int_least8_t, operand:int(8), order:memory_order):int(8);
  extern proc atomic_fetch_or_explicit_int_least8_t(ref obj:atomic_int_least8_t, operand:int(8), order:memory_order):int(8);
  extern proc atomic_fetch_and_explicit_int_least8_t(ref obj:atomic_int_least8_t, operand:int(8), order:memory_order):int(8);
  extern proc atomic_fetch_xor_explicit_int_least8_t(ref obj:atomic_int_least8_t, operand:int(8), order:memory_order):int(8);

  extern proc atomic_is_lock_free_int_least16_t(ref obj:atomic_int_least16_t):bool;
  extern proc atomic_init_int_least16_t(ref obj:atomic_int_least16_t, value:int(16));
  extern proc atomic_destroy_int_least16_t(ref obj:atomic_int_least16_t);
  extern proc atomic_store_explicit_int_least16_t(ref obj:atomic_int_least16_t, value:int(16), order:memory_order);
  extern proc atomic_load_explicit_int_least16_t(const ref obj:atomic_int_least16_t, order:memory_order):int(16);
  extern proc atomic_exchange_explicit_int_least16_t(ref obj:atomic_int_least16_t, value:int(16), order:memory_order):int(16);
  extern proc atomic_compare_exchange_strong_explicit_int_least16_t(ref obj:atomic_int_least16_t, expected:int(16), desired:int(16), order:memory_order):bool;
  extern proc atomic_compare_exchange_weak_explicit_int_least16_t(ref obj:atomic_int_least16_t, expected:int(16), desired:int(16), order:memory_order):bool;
  extern proc atomic_fetch_add_explicit_int_least16_t(ref obj:atomic_int_least16_t, operand:int(16), order:memory_order):int(16);
  extern proc atomic_fetch_sub_explicit_int_least16_t(ref obj:atomic_int_least16_t, operand:int(16), order:memory_order):int(16);
  extern proc atomic_fetch_or_explicit_int_least16_t(ref obj:atomic_int_least16_t, operand:int(16), order:memory_order):int(16);
  extern proc atomic_fetch_and_explicit_int_least16_t(ref obj:atomic_int_least16_t, operand:int(16), order:memory_order):int(16);
  extern proc atomic_fetch_xor_explicit_int_least16_t(ref obj:atomic_int_least16_t, operand:int(16), order:memory_order):int(16);

  extern proc atomic_is_lock_free_int_least32_t(ref obj:atomic_int_least32_t):bool;
  extern proc atomic_init_int_least32_t(ref obj:atomic_int_least32_t, value:int(32));
  extern proc atomic_destroy_int_least32_t(ref obj:atomic_int_least32_t);
  extern proc atomic_store_explicit_int_least32_t(ref obj:atomic_int_least32_t, value:int(32), order:memory_order);
  extern proc atomic_load_explicit_int_least32_t(const ref obj:atomic_int_least32_t, order:memory_order):int(32);
  extern proc atomic_exchange_explicit_int_least32_t(ref obj:atomic_int_least32_t, value:int(32), order:memory_order):int(32);
  extern proc atomic_compare_exchange_strong_explicit_int_least32_t(ref obj:atomic_int_least32_t, expected:int(32), desired:int(32), order:memory_order):bool;
  extern proc atomic_compare_exchange_weak_explicit_int_least32_t(ref obj:atomic_int_least32_t, expected:int(32), desired:int(32), order:memory_order):bool;
  extern proc atomic_fetch_add_explicit_int_least32_t(ref obj:atomic_int_least32_t, operand:int(32), order:memory_order):int(32);
  extern proc atomic_fetch_sub_explicit_int_least32_t(ref obj:atomic_int_least32_t, operand:int(32), order:memory_order):int(32);
  extern proc atomic_fetch_or_explicit_int_least32_t(ref obj:atomic_int_least32_t, operand:int(32), order:memory_order):int(32);
  extern proc atomic_fetch_and_explicit_int_least32_t(ref obj:atomic_int_least32_t, operand:int(32), order:memory_order):int(32);
  extern proc atomic_fetch_xor_explicit_int_least32_t(ref obj:atomic_int_least32_t, operand:int(32), order:memory_order):int(32);


  extern proc atomic_init__real64(ref obj:atomic__real64, value:real(64));
  extern proc atomic_destroy__real64(ref obj:atomic__real64);
  extern proc atomic_store_explicit__real64(ref obj:atomic__real64, value:real(64), order:memory_order);
  extern proc atomic_is_lock_free__real64(ref obj:atomic__real64):bool;
  extern proc atomic_load_explicit__real64(const ref obj:atomic__real64, order:memory_order):real(64);
  extern proc atomic_exchange_explicit__real64(ref obj:atomic__real64, value:real(64), order:memory_order):real(64);
  extern proc atomic_compare_exchange_strong_explicit__real64(ref obj:atomic__real64, expected:real(64), desired:real(64), order:memory_order):bool;

  extern proc atomic_compare_exchange_weak_explicit__real64(ref obj:atomic__real64, expected:real(64), desired:real(64), order:memory_order):bool;
  extern proc atomic_fetch_add_explicit__real64(ref obj:atomic__real64, operand:real(64), order:memory_order):real(64);
  extern proc atomic_fetch_sub_explicit__real64(ref obj:atomic__real64, operand:real(64), order:memory_order):real(64);

  extern proc atomic_init__real32(ref obj:atomic__real32, value:real(32));
  extern proc atomic_destroy__real32(ref obj:atomic__real32);
  extern proc atomic_store_explicit__real32(ref obj:atomic__real32, value:real(32), order:memory_order);
  extern proc atomic_is_lock_free__real32(ref obj:atomic__real32):bool;
  extern proc atomic_load_explicit__real32(const ref obj:atomic__real32, order:memory_order):real(32);
  extern proc atomic_exchange_explicit__real32(ref obj:atomic__real32, value:real(32), order:memory_order):real(32);
  extern proc atomic_compare_exchange_strong_explicit__real32(ref obj:atomic__real32, expected:real(32), desired:real(32), order:memory_order):bool;

  extern proc atomic_compare_exchange_weak_explicit__real32(ref obj:atomic__real32, expected:real(32), desired:real(32), order:memory_order):bool;
  extern proc atomic_fetch_add_explicit__real32(ref obj:atomic__real32, operand:real(32), order:memory_order):real(32);
  extern proc atomic_fetch_sub_explicit__real32(ref obj:atomic__real32, operand:real(32), order:memory_order):real(32);

  // Begin Chapel interface for atomic integers.

  // See runtime/include/atomics/README for more info about these functions

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
  proc chpl__processorAtomicType(type base_type) type {
    if base_type==bool then return AtomicBool;
    else if base_type==uint(8) then return atomic_uint8;
    else if base_type==uint(16) then return atomic_uint16;
    else if base_type==uint(32) then return atomic_uint32;
    else if base_type==uint(64) then return atomic_uint64;
    else if base_type==int(8) then return atomic_int8;
    else if base_type==int(16) then return atomic_int16;
    else if base_type==int(32) then return atomic_int32;
    else if base_type==int(64) then return AtomicT(base_type);
    else if base_type==real(64) then return atomic_real64;
    else if base_type==real(32) then return atomic_real32;
    else compilerError("Unsupported atomic type");
  }

  pragma "no doc"
  proc chpl__atomicType(type base_type) type {
    if CHPL_NETWORK_ATOMICS == "none" {
      return chpl__processorAtomicType(base_type);
    } else {
      return chpl__networkAtomicType(base_type);
    }
  };


  pragma "atomic type"
  pragma "ignore noinit"
  /*
     The boolean atomic type.
  */
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

      atomic_destroy(_v);
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
  pragma "no doc"
  record atomic_uint8 {
    var _v:atomic_uint_least8_t;
    proc init() {
      this.complete();
      atomic_init_uint_least8_t(_v, _defaultOf(uint(8)));
    }
    proc deinit() {
      atomic_destroy_uint_least8_t(_v);
    }
    inline proc const read(order:memory_order = memory_order_seq_cst):uint(8) {
      var ret:uint(8);
      on this do ret = atomic_load_explicit_uint_least8_t(_v, order);
      return ret;
    }
    inline proc write(value:uint(8), order:memory_order = memory_order_seq_cst) {
      on this do atomic_store_explicit_uint_least8_t(_v, value, order);
    }
    inline proc exchange(value:uint(8), order:memory_order = memory_order_seq_cst):uint(8) {
      var ret:uint(8);
      on this do ret = atomic_exchange_explicit_uint_least8_t(_v, value, order);
      return ret;
    }
    inline proc compareExchange(expected:uint(8), desired:uint(8), order:memory_order = memory_order_seq_cst):bool {
      return compareExchangeStrong(expected, desired, order);
    }
    inline proc compareExchangeWeak(expected:uint(8), desired:uint(8), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_weak_explicit_uint_least8_t(_v, expected, desired, order);
      return ret;
    }
    inline proc compareExchangeStrong(expected:uint(8), desired:uint(8), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_strong_explicit_uint_least8_t(_v, expected, desired, order);
      return ret;
    }
    inline proc fetchAdd(value:uint(8), order:memory_order = memory_order_seq_cst):uint(8) {
      var ret:uint(8);
      on this do ret = atomic_fetch_add_explicit_uint_least8_t(_v, value, order);
      return ret;
    }
    inline proc add(value:uint(8), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_add_explicit_uint_least8_t(_v, value, order);
    }
    inline proc fetchSub(value:uint(8), order:memory_order = memory_order_seq_cst):uint(8) {
      var ret:uint(8);
      on this do ret = atomic_fetch_sub_explicit_uint_least8_t(_v, value, order);
      return ret;
    }
    inline proc sub(value:uint(8), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_sub_explicit_uint_least8_t(_v, value, order);
    }
    inline proc fetchOr(value:uint(8), order:memory_order = memory_order_seq_cst):uint(8) {
      var ret:uint(8);
      on this do ret = atomic_fetch_or_explicit_uint_least8_t(_v, value, order);
      return ret;
    }
    inline proc or(value:uint(8), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_or_explicit_uint_least8_t(_v, value, order);
    }
    inline proc fetchAnd(value:uint(8), order:memory_order = memory_order_seq_cst):uint(8) {
      var ret:uint(8);
      on this do ret = atomic_fetch_and_explicit_uint_least8_t(_v, value, order);
      return ret;
    }
    inline proc and(value:uint(8), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_and_explicit_uint_least8_t(_v, value, order);
    }
    inline proc fetchXor(value:uint(8), order:memory_order = memory_order_seq_cst):uint(8) {
      var ret:uint(8);
      on this do ret = atomic_fetch_xor_explicit_uint_least8_t(_v, value, order);
      return ret;
    }
    inline proc xor(value:uint(8), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_xor_explicit_uint_least8_t(_v, value, order);
    }

    inline proc const waitFor(val:uint(8), order:memory_order = memory_order_seq_cst) {
      on this {
        while (atomic_load_explicit_uint_least8_t(_v, memory_order_relaxed)
                != val) {
          chpl_task_yield();
        }
        atomic_thread_fence(order);
      }
    }

    inline proc const peek() {
      return this.read(order=memory_order_relaxed);
    }
    inline proc poke(value:uint(8)) {
      this.write(value, order=memory_order_relaxed);
    }

    proc const writeThis(x) {
      x <~> read();
    }
  }

  pragma "atomic type"
  pragma "ignore noinit"
  pragma "no doc"
  record atomic_uint16 {
    var _v:atomic_uint_least16_t;
    proc init() {
      this.complete();
      atomic_init_uint_least16_t(_v, _defaultOf(uint(16)));
    }
    proc deinit() {
      atomic_destroy_uint_least16_t(_v);
    }
    inline proc const read(order:memory_order = memory_order_seq_cst):uint(16) {
      var ret:uint(16);
      on this do ret = atomic_load_explicit_uint_least16_t(_v, order);
      return ret;
    }
    inline proc write(value:uint(16), order:memory_order = memory_order_seq_cst) {
      on this do atomic_store_explicit_uint_least16_t(_v, value, order);
    }
    inline proc exchange(value:uint(16), order:memory_order = memory_order_seq_cst):uint(16) {
      var ret:uint(16);
      on this do ret = atomic_exchange_explicit_uint_least16_t(_v, value, order);
      return ret;
    }
    inline proc compareExchange(expected:uint(16), desired:uint(16), order:memory_order = memory_order_seq_cst):bool {
      return compareExchangeStrong(expected, desired, order);
    }
    inline proc compareExchangeWeak(expected:uint(16), desired:uint(16), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_weak_explicit_uint_least16_t(_v, expected, desired, order);
      return ret;
    }
    inline proc compareExchangeStrong(expected:uint(16), desired:uint(16), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_strong_explicit_uint_least16_t(_v, expected, desired, order);
      return ret;
    }
    inline proc fetchAdd(value:uint(16), order:memory_order = memory_order_seq_cst):uint(16) {
      var ret:uint(16);
      on this do ret = atomic_fetch_add_explicit_uint_least16_t(_v, value, order);
      return ret;
    }
    inline proc add(value:uint(16), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_add_explicit_uint_least16_t(_v, value, order);
    }
    inline proc fetchSub(value:uint(16), order:memory_order = memory_order_seq_cst):uint(16) {
      var ret:uint(16);
      on this do ret = atomic_fetch_sub_explicit_uint_least16_t(_v, value, order);
      return ret;
    }
    inline proc sub(value:uint(16), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_sub_explicit_uint_least16_t(_v, value, order);
    }
    inline proc fetchOr(value:uint(16), order:memory_order = memory_order_seq_cst):uint(16) {
      var ret:uint(16);
      on this do ret = atomic_fetch_or_explicit_uint_least16_t(_v, value, order);
      return ret;
    }
    inline proc or(value:uint(16), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_or_explicit_uint_least16_t(_v, value, order);
    }
    inline proc fetchAnd(value:uint(16), order:memory_order = memory_order_seq_cst):uint(16) {
      var ret:uint(16);
      on this do ret = atomic_fetch_and_explicit_uint_least16_t(_v, value, order);
      return ret;
    }
    inline proc and(value:uint(16), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_and_explicit_uint_least16_t(_v, value, order);
    }
    inline proc fetchXor(value:uint(16), order:memory_order = memory_order_seq_cst):uint(16) {
      var ret:uint(16);
      on this do ret = atomic_fetch_xor_explicit_uint_least16_t(_v, value, order);
      return ret;
    }
    inline proc xor(value:uint(16), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_xor_explicit_uint_least16_t(_v, value, order);
    }

    inline proc const waitFor(val:uint(16), order:memory_order = memory_order_seq_cst) {
      on this {
        while (atomic_load_explicit_uint_least16_t(_v, memory_order_relaxed)
                != val) {
          chpl_task_yield();
        }
        atomic_thread_fence(order);
      }
    }

    inline proc const peek() {
      return this.read(order=memory_order_relaxed);
    }
    inline proc poke(value:uint(16)) {
      this.write(value, order=memory_order_relaxed);
    }

    proc const writeThis(x) {
      x <~> read();
    }
  }

  pragma "atomic type"
  pragma "ignore noinit"
  pragma "no doc"
  record atomic_uint32 {
    var _v:atomic_uint_least32_t;
    proc init() {
      this.complete();
      atomic_init_uint_least32_t(_v, _defaultOf(uint(32)));
    }
    proc deinit() {
      atomic_destroy_uint_least32_t(_v);
    }
    inline proc const read(order:memory_order = memory_order_seq_cst):uint(32) {
      var ret:uint(32);
      on this do ret = atomic_load_explicit_uint_least32_t(_v, order);
      return ret;
    }
    inline proc write(value:uint(32), order:memory_order = memory_order_seq_cst) {
      on this do atomic_store_explicit_uint_least32_t(_v, value, order);
    }
    inline proc exchange(value:uint(32), order:memory_order = memory_order_seq_cst):uint(32) {
      var ret:uint(32);
      on this do ret = atomic_exchange_explicit_uint_least32_t(_v, value, order);
      return ret;
    }
    inline proc compareExchange(expected:uint(32), desired:uint(32), order:memory_order = memory_order_seq_cst):bool {
      return compareExchangeStrong(expected, desired, order);
    }
    inline proc compareExchangeWeak(expected:uint(32), desired:uint(32), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_weak_explicit_uint_least32_t(_v, expected, desired, order);
      return ret;
    }
    inline proc compareExchangeStrong(expected:uint(32), desired:uint(32), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_strong_explicit_uint_least32_t(_v, expected, desired, order);
      return ret;
    }
    inline proc fetchAdd(value:uint(32), order:memory_order = memory_order_seq_cst):uint(32) {
      var ret:uint(32);
      on this do ret = atomic_fetch_add_explicit_uint_least32_t(_v, value, order);
      return ret;
    }
    inline proc add(value:uint(32), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_add_explicit_uint_least32_t(_v, value, order);
    }
    inline proc fetchSub(value:uint(32), order:memory_order = memory_order_seq_cst):uint(32) {
      var ret:uint(32);
      on this do ret = atomic_fetch_sub_explicit_uint_least32_t(_v, value, order);
      return ret;
    }
    inline proc sub(value:uint(32), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_sub_explicit_uint_least32_t(_v, value, order);
    }
    inline proc fetchOr(value:uint(32), order:memory_order = memory_order_seq_cst):uint(32) {
      var ret:uint(32);
      on this do ret = atomic_fetch_or_explicit_uint_least32_t(_v, value, order);
      return ret;
    }
    inline proc or(value:uint(32), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_or_explicit_uint_least32_t(_v, value, order);
    }
    inline proc fetchAnd(value:uint(32), order:memory_order = memory_order_seq_cst):uint(32) {
      var ret:uint(32);
      on this do ret = atomic_fetch_and_explicit_uint_least32_t(_v, value, order);
      return ret;
    }
    inline proc and(value:uint(32), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_and_explicit_uint_least32_t(_v, value, order);
    }
    inline proc fetchXor(value:uint(32), order:memory_order = memory_order_seq_cst):uint(32) {
      var ret:uint(32);
      on this do ret = atomic_fetch_xor_explicit_uint_least32_t(_v, value, order);
      return ret;
    }
    inline proc xor(value:uint(32), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_xor_explicit_uint_least32_t(_v, value, order);
    }

    inline proc const waitFor(val:uint(32), order:memory_order = memory_order_seq_cst) {
      on this {
        while (atomic_load_explicit_uint_least32_t(_v,memory_order_relaxed)
                != val) {
          chpl_task_yield();
        }
        atomic_thread_fence(order);
      }
    }

    inline proc const peek() {
      return this.read(order=memory_order_relaxed);
    }
    inline proc poke(value:uint(32)) {
      this.write(value, order=memory_order_relaxed);
    }

    proc const writeThis(x) {
      x <~> read();
    }
  }

  pragma "atomic type"
  pragma "ignore noinit"
  pragma "no doc"
  record atomic_uint64 {
    var _v:atomic_uint_least64_t;
    proc init() {
      this.complete();
      atomic_init_uint_least64_t(_v, _defaultOf(uint(64)));
    }
    proc deinit() {
      atomic_destroy_uint_least64_t(_v);
    }
    inline proc const read(order:memory_order = memory_order_seq_cst):uint(64) {
      var ret:uint(64);
      on this do ret = atomic_load_explicit_uint_least64_t(_v, order);
      return ret;
    }
    inline proc write(value:uint(64), order:memory_order = memory_order_seq_cst) {
      on this do atomic_store_explicit_uint_least64_t(_v, value, order);
    }
    inline proc exchange(value:uint(64), order:memory_order = memory_order_seq_cst):uint(64) {
      var ret:uint(64);
      on this do ret = atomic_exchange_explicit_uint_least64_t(_v, value, order);
      return ret;
    }
    inline proc compareExchange(expected:uint(64), desired:uint(64), order:memory_order = memory_order_seq_cst):bool {
      return compareExchangeStrong(expected, desired, order);
    }
    inline proc compareExchangeWeak(expected:uint(64), desired:uint(64), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_weak_explicit_uint_least64_t(_v, expected, desired, order);
      return ret;
    }
    inline proc compareExchangeStrong(expected:uint(64), desired:uint(64), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_strong_explicit_uint_least64_t(_v, expected, desired, order);
      return ret;
    }
    inline proc fetchAdd(value:uint(64), order:memory_order = memory_order_seq_cst):uint(64) {
      var ret:uint(64);
      on this do ret = atomic_fetch_add_explicit_uint_least64_t(_v, value, order);
      return ret;
    }
    inline proc add(value:uint(64), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_add_explicit_uint_least64_t(_v, value, order);
    }
    inline proc fetchSub(value:uint(64), order:memory_order = memory_order_seq_cst):uint(64) {
      var ret:uint(64);
      on this do ret = atomic_fetch_sub_explicit_uint_least64_t(_v, value, order);
      return ret;
    }
    inline proc sub(value:uint(64), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_sub_explicit_uint_least64_t(_v, value, order);
    }
    inline proc fetchOr(value:uint(64), order:memory_order = memory_order_seq_cst):uint(64) {
      var ret:uint(64);
      on this do ret = atomic_fetch_or_explicit_uint_least64_t(_v, value, order);
      return ret;
    }
    inline proc or(value:uint(64), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_or_explicit_uint_least64_t(_v, value, order);
    }
    inline proc fetchAnd(value:uint(64), order:memory_order = memory_order_seq_cst):uint(64) {
      var ret:uint(64);
      on this do ret = atomic_fetch_and_explicit_uint_least64_t(_v, value, order);
      return ret;
    }
    inline proc and(value:uint(64), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_and_explicit_uint_least64_t(_v, value, order);
    }
    inline proc fetchXor(value:uint(64), order:memory_order = memory_order_seq_cst):uint(64) {
      var ret:uint(64);
      on this do ret = atomic_fetch_xor_explicit_uint_least64_t(_v, value, order);
      return ret;
    }
    inline proc xor(value:uint(64), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_xor_explicit_uint_least64_t(_v, value, order);
    }

    inline proc const waitFor(val:uint(64), order:memory_order = memory_order_seq_cst) {
      on this {
        while (atomic_load_explicit_uint_least64_t(_v, memory_order_relaxed)
                 != val) {
          chpl_task_yield();
        }
        atomic_thread_fence(order);
      }
    }

    inline proc const peek() {
      return this.read(order=memory_order_relaxed);
    }
    inline proc poke(value:uint(64)) {
      this.write(value, order=memory_order_relaxed);
    }

    proc const writeThis(x) {
      x <~> read();
    }
  }

  pragma "atomic type"
  pragma "ignore noinit"
  pragma "no doc"
  record atomic_int8 {
    var _v:atomic_int_least8_t;
    proc init() {
      this.complete();
      atomic_init_int_least8_t(_v, _defaultOf(int(8)));
    }
    proc deinit() {
      atomic_destroy_int_least8_t(_v);
    }
    inline proc const read(order:memory_order = memory_order_seq_cst):int(8) {
      var ret:int(8);
      on this do ret = atomic_load_explicit_int_least8_t(_v, order);
      return ret;
    }
    inline proc write(value:int(8), order:memory_order = memory_order_seq_cst) {
      on this do atomic_store_explicit_int_least8_t(_v, value, order);
    }
    inline proc exchange(value:int(8), order:memory_order = memory_order_seq_cst):int(8) {
      var ret:int(8);
      on this do ret = atomic_exchange_explicit_int_least8_t(_v, value, order);
      return ret;
    }
    inline proc compareExchange(expected:int(8), desired:int(8), order:memory_order = memory_order_seq_cst):bool {
      return compareExchangeStrong(expected, desired, order);
    }
    inline proc compareExchangeWeak(expected:int(8), desired:int(8), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_weak_explicit_int_least8_t(_v, expected, desired, order);
      return ret;
    }
    inline proc compareExchangeStrong(expected:int(8), desired:int(8), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_strong_explicit_int_least8_t(_v, expected, desired, order);
      return ret;
    }
    inline proc fetchAdd(value:int(8), order:memory_order = memory_order_seq_cst):int(8) {
      var ret:int(8);
      on this do ret = atomic_fetch_add_explicit_int_least8_t(_v, value, order);
      return ret;
    }
    inline proc add(value:int(8), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_add_explicit_int_least8_t(_v, value, order);
    }
    inline proc fetchSub(value:int(8), order:memory_order = memory_order_seq_cst):int(8) {
      var ret:int(8);
      on this do ret = atomic_fetch_sub_explicit_int_least8_t(_v, value, order);
      return ret;
    }
    inline proc sub(value:int(8), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_sub_explicit_int_least8_t(_v, value, order);
    }
    inline proc fetchOr(value:int(8), order:memory_order = memory_order_seq_cst):int(8) {
      var ret:int(8);
      on this do ret = atomic_fetch_or_explicit_int_least8_t(_v, value, order);
      return ret;
    }
    inline proc or(value:int(8), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_or_explicit_int_least8_t(_v, value, order);
    }
    inline proc fetchAnd(value:int(8), order:memory_order = memory_order_seq_cst):int(8) {
      var ret:int(8);
      on this do ret = atomic_fetch_and_explicit_int_least8_t(_v, value, order);
      return ret;
    }
    inline proc and(value:int(8), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_and_explicit_int_least8_t(_v, value, order);
    }
    inline proc fetchXor(value:int(8), order:memory_order = memory_order_seq_cst):int(8) {
      var ret:int(8);
      on this do ret = atomic_fetch_xor_explicit_int_least8_t(_v, value, order);
      return ret;
    }
    inline proc xor(value:int(8), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_xor_explicit_int_least8_t(_v, value, order);
    }

    inline proc const waitFor(val:int(8), order:memory_order = memory_order_seq_cst) {
      on this {
        while (atomic_load_explicit_int_least8_t(_v, memory_order_relaxed)
                != val) {
          chpl_task_yield();
        }
        atomic_thread_fence(order);
      }
    }

    inline proc const peek() {
      return this.read(order=memory_order_relaxed);
    }
    inline proc poke(value:int(8)) {
      this.write(value, order=memory_order_relaxed);
    }

    proc const writeThis(x) {
      x <~> read();
    }
  }

  pragma "atomic type"
  pragma "ignore noinit"
  pragma "no doc"
  record atomic_int16 {
    var _v:atomic_int_least16_t;
    proc init() {
      this.complete();
      atomic_init_int_least16_t(_v, _defaultOf(int(16)));
    }
    proc deinit() {
      atomic_destroy_int_least16_t(_v);
    }
    inline proc const read(order:memory_order = memory_order_seq_cst):int(16) {
      var ret:int(16);
      on this do ret = atomic_load_explicit_int_least16_t(_v, order);
      return ret;
    }
    inline proc write(value:int(16), order:memory_order = memory_order_seq_cst) {
      on this do atomic_store_explicit_int_least16_t(_v, value, order);
    }
    inline proc exchange(value:int(16), order:memory_order = memory_order_seq_cst):int(16) {
      var ret:int(16);
      on this do ret = atomic_exchange_explicit_int_least16_t(_v, value, order);
      return ret;
    }
    inline proc compareExchange(expected:int(16), desired:int(16), order:memory_order = memory_order_seq_cst):bool {
      return compareExchangeStrong(expected, desired, order);
    }
    inline proc compareExchangeWeak(expected:int(16), desired:int(16), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_weak_explicit_int_least16_t(_v, expected, desired, order);
      return ret;
    }
    inline proc compareExchangeStrong(expected:int(16), desired:int(16), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_strong_explicit_int_least16_t(_v, expected, desired, order);
      return ret;
    }
    inline proc fetchAdd(value:int(16), order:memory_order = memory_order_seq_cst):int(16) {
      var ret:int(16);
      on this do ret = atomic_fetch_add_explicit_int_least16_t(_v, value, order);
      return ret;
    }
    inline proc add(value:int(16), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_add_explicit_int_least16_t(_v, value, order);
    }
    inline proc fetchSub(value:int(16), order:memory_order = memory_order_seq_cst):int(16) {
      var ret:int(16);
      on this do ret = atomic_fetch_sub_explicit_int_least16_t(_v, value, order);
      return ret;
    }
    inline proc sub(value:int(16), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_sub_explicit_int_least16_t(_v, value, order);
    }
    inline proc fetchOr(value:int(16), order:memory_order = memory_order_seq_cst):int(16) {
      var ret:int(16);
      on this do ret = atomic_fetch_or_explicit_int_least16_t(_v, value, order);
      return ret;
    }
    inline proc or(value:int(16), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_or_explicit_int_least16_t(_v, value, order);
    }
    inline proc fetchAnd(value:int(16), order:memory_order = memory_order_seq_cst):int(16) {
      var ret:int(16);
      on this do ret = atomic_fetch_and_explicit_int_least16_t(_v, value, order);
      return ret;
    }
    inline proc and(value:int(16), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_and_explicit_int_least16_t(_v, value, order);
    }
    inline proc fetchXor(value:int(16), order:memory_order = memory_order_seq_cst):int(16) {
      var ret:int(16);
      on this do ret = atomic_fetch_xor_explicit_int_least16_t(_v, value, order);
      return ret;
    }
    inline proc xor(value:int(16), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_xor_explicit_int_least16_t(_v, value, order);
    }

    inline proc const waitFor(val:int(16), order:memory_order = memory_order_seq_cst) {
      on this {
        while (atomic_load_explicit_int_least16_t(_v,memory_order_relaxed)
                != val) {
          chpl_task_yield();
        }
        atomic_thread_fence(order);
      }
    }

    inline proc const peek() {
      return this.read(order=memory_order_relaxed);
    }
    inline proc poke(value:int(16)) {
      this.write(value, order=memory_order_relaxed);
    }

    proc const writeThis(x) {
      x <~> read();
    }
  }

  pragma "atomic type"
  pragma "ignore noinit"
  pragma "no doc"
  record atomic_int32 {
    var _v:atomic_int_least32_t;
    proc init() {
      this.complete();
      atomic_init_int_least32_t(_v, _defaultOf(int(32)));
    }
    proc deinit() {
      atomic_destroy_int_least32_t(_v);
    }
    inline proc const read(order:memory_order = memory_order_seq_cst):int(32) {
      var ret:int(32);
      on this do ret = atomic_load_explicit_int_least32_t(_v, order);
      return ret;
    }
    inline proc write(value:int(32), order:memory_order = memory_order_seq_cst) {
      on this do atomic_store_explicit_int_least32_t(_v, value, order);
    }
    inline proc exchange(value:int(32), order:memory_order = memory_order_seq_cst):int(32) {
      var ret:int(32);
      on this do ret = atomic_exchange_explicit_int_least32_t(_v, value, order);
      return ret;
    }
    inline proc compareExchange(expected:int(32), desired:int(32), order:memory_order = memory_order_seq_cst):bool {
      return compareExchangeStrong(expected, desired, order);
    }
    inline proc compareExchangeWeak(expected:int(32), desired:int(32), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_weak_explicit_int_least32_t(_v, expected, desired, order);
      return ret;
    }
    inline proc compareExchangeStrong(expected:int(32), desired:int(32), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_strong_explicit_int_least32_t(_v, expected, desired, order);
      return ret;
    }
    inline proc fetchAdd(value:int(32), order:memory_order = memory_order_seq_cst):int(32) {
      var ret:int(32);
      on this do ret = atomic_fetch_add_explicit_int_least32_t(_v, value, order);
      return ret;
    }
    inline proc add(value:int(32), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_add_explicit_int_least32_t(_v, value, order);
    }
    inline proc fetchSub(value:int(32), order:memory_order = memory_order_seq_cst):int(32) {
      var ret:int(32);
      on this do ret = atomic_fetch_sub_explicit_int_least32_t(_v, value, order);
      return ret;
    }
    inline proc sub(value:int(32), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_sub_explicit_int_least32_t(_v, value, order);
    }
    inline proc fetchOr(value:int(32), order:memory_order = memory_order_seq_cst):int(32) {
      var ret:int(32);
      on this do ret = atomic_fetch_or_explicit_int_least32_t(_v, value, order);
      return ret;
    }
    inline proc or(value:int(32), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_or_explicit_int_least32_t(_v, value, order);
    }
    inline proc fetchAnd(value:int(32), order:memory_order = memory_order_seq_cst):int(32) {
      var ret:int(32);
      on this do ret = atomic_fetch_and_explicit_int_least32_t(_v, value, order);
      return ret;
    }
    inline proc and(value:int(32), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_and_explicit_int_least32_t(_v, value, order);
    }
    inline proc fetchXor(value:int(32), order:memory_order = memory_order_seq_cst):int(32) {
      var ret:int(32);
      on this do ret = atomic_fetch_xor_explicit_int_least32_t(_v, value, order);
      return ret;
    }
    inline proc xor(value:int(32), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_xor_explicit_int_least32_t(_v, value, order);
    }

    inline proc const waitFor(val:int(32), order:memory_order = memory_order_seq_cst) {
      on this {
        while (atomic_load_explicit_int_least32_t(_v, memory_order_relaxed)
                != val) { chpl_task_yield();
        }
        atomic_thread_fence(order);
      }
    }

    inline proc const peek() {
      return this.read(order=memory_order_relaxed);
    }
    inline proc poke(value:int(32)) {
      this.write(value, order=memory_order_relaxed);
    }

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

      atomic_destroy(_v);
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

  pragma "atomic type"
  pragma "ignore noinit"
  pragma "no doc"
  record atomic_real64 {
    var _v:atomic__real64;
    proc init() {
      this.complete();
      atomic_init__real64(_v, _defaultOf(real(64)));
    }
    proc deinit() {
      atomic_destroy__real64(_v);
    }
    inline proc const read(order:memory_order = memory_order_seq_cst):real(64) {
      var ret:real(64);
      on this do ret = atomic_load_explicit__real64(_v, order);
      return ret;
    }
    inline proc write(value:real(64), order:memory_order = memory_order_seq_cst) {
      on this do atomic_store_explicit__real64(_v, value, order);
    }

    inline proc exchange(value:real(64), order:memory_order = memory_order_seq_cst):real(64) {
      var ret:real(64);
      on this do ret = atomic_exchange_explicit__real64(_v, value, order);
      return ret;
    }
    inline proc compareExchange(expected:real(64), desired:real(64), order:memory_order = memory_order_seq_cst):bool {
      return compareExchangeStrong(expected, desired, order);
    }
    inline proc compareExchangeWeak(expected:real(64), desired:real(64), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_weak_explicit__real64(_v, expected, desired, order);
      return ret;
    }
    inline proc compareExchangeStrong(expected:real(64), desired:real(64), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_strong_explicit__real64(_v, expected, desired, order);
      return ret;
    }
    inline proc fetchAdd(value:real(64), order:memory_order = memory_order_seq_cst):real(64) {
      var ret:real(64);
      on this do ret = atomic_fetch_add_explicit__real64(_v, value, order);
      return ret;
    }
    inline proc add(value:real(64), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_add_explicit__real64(_v, value, order);
    }
    inline proc fetchSub(value:real(64), order:memory_order = memory_order_seq_cst):real(64) {
      var ret:real(64);
      on this do ret = atomic_fetch_sub_explicit__real64(_v, value, order);
      return ret;
    }
    inline proc sub(value:real(64), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_sub_explicit__real64(_v, value, order);
    }
    inline proc const waitFor(val:real(64), order:memory_order = memory_order_seq_cst) {
      on this {
        while (atomic_load_explicit__real64(_v, memory_order_relaxed)
                != val) {
          chpl_task_yield();
        }
        atomic_thread_fence(order);
      }
    }

    inline proc const peek() {
      return this.read(order=memory_order_relaxed);
    }
    inline proc poke(value:real(64)) {
      this.write(value, order=memory_order_relaxed);
    }

    proc const writeThis(x) {
      x <~> read();
    }
  }

  pragma "atomic type"
  pragma "ignore noinit"
  pragma "no doc"
  record atomic_real32 {
    var _v:atomic__real32;
    proc init() {
      this.complete();
      atomic_init__real32(_v, _defaultOf(real(32)));
    }
    proc deinit() {
      atomic_destroy__real32(_v);
    }
    inline proc const read(order:memory_order = memory_order_seq_cst):real(32) {
      var ret:real(32);
      on this do ret = atomic_load_explicit__real32(_v, order);
      return ret;
    }
    inline proc write(value:real(32), order:memory_order = memory_order_seq_cst) {
      on this do atomic_store_explicit__real32(_v, value, order);
    }
    inline proc exchange(value:real(32), order:memory_order = memory_order_seq_cst):real(32) {
      var ret:real(32);
      on this do ret = atomic_exchange_explicit__real32(_v, value, order);
      return ret;
    }
    inline proc compareExchange(expected:real(32), desired:real(32), order:memory_order = memory_order_seq_cst):bool {
      return compareExchangeStrong(expected, desired, order);
    }
    inline proc compareExchangeWeak(expected:real(32), desired:real(32), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_weak_explicit__real32(_v, expected, desired, order);
      return ret;
    }
    inline proc compareExchangeStrong(expected:real(32), desired:real(32), order:memory_order = memory_order_seq_cst):bool {
      var ret:bool;
      on this do ret = atomic_compare_exchange_strong_explicit__real32(_v, expected, desired, order);
      return ret;
    }
    inline proc fetchAdd(value:real(32), order:memory_order = memory_order_seq_cst):real(32) {
      var ret:real(32);
      on this do ret = atomic_fetch_add_explicit__real32(_v, value, order);
      return ret;
    }
    inline proc add(value:real(32), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_add_explicit__real32(_v, value, order);
    }
    inline proc fetchSub(value:real(32), order:memory_order = memory_order_seq_cst):real(32) {
      var ret:real(32);
      on this do ret = atomic_fetch_sub_explicit__real32(_v, value, order);
      return ret;
    }
    inline proc sub(value:real(32), order:memory_order = memory_order_seq_cst):void {
      on this do atomic_fetch_sub_explicit__real32(_v, value, order);
    }
    inline proc const waitFor(val:real(32), order:memory_order = memory_order_seq_cst) {
      on this {
        while (atomic_load_explicit__real32(_v, memory_order_relaxed) != val) {
          chpl_task_yield();
        }
        atomic_thread_fence(order);
      }
    }

    inline proc const peek() {
      return this.read(order=memory_order_relaxed);
    }
    inline proc poke(value:real(32)) {
      this.write(value, order=memory_order_relaxed);
    }

    proc const writeThis(x) {
      x <~> read();
    }
  }

  //
  // For the first cut, we will be making assignment and other normal
  //  operations illegal.  In addition, we are also punting on
  //  the default argument intent since it's not obvious what it
  //  should be.
  //

  // We need to explicitly define these for all types because the atomic
  //  types are records and unless explicitly defined, it will resolve
  //  to the normal record version of the function.  Sigh.
  inline proc =(ref a:AtomicBool, const ref b:AtomicBool) {
    a.write(b.read());
  }
  inline proc =(ref a:AtomicBool, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  inline proc =(ref a:atomic_uint8, const ref b:atomic_uint8) {
    a.write(b.read());
  }
  inline proc =(ref a:atomic_uint8, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  inline proc =(ref a:atomic_uint16, const ref b:atomic_uint16) {
    a.write(b.read());
  }
  inline proc =(ref a:atomic_uint16, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  inline proc =(ref a:atomic_uint32, const ref b:atomic_uint32) {
    a.write(b.read());
  }
  inline proc =(ref a:atomic_uint32, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  inline proc =(ref a:atomic_uint64, const ref b:atomic_uint64) {
    a.write(b.read());
  }
  inline proc =(ref a:atomic_uint64, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  inline proc =(ref a:atomic_int8, const ref b:atomic_int8) {
    a.write(b.read());
  }
  inline proc =(ref a:atomic_int8, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  inline proc =(ref a:atomic_int16, const ref b:atomic_int16) {
    a.write(b.read());
  }
  inline proc =(ref a:atomic_int16, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  inline proc =(ref a:atomic_int32, const ref b:atomic_int32) {
    a.write(b.read());
  }
  inline proc =(ref a:atomic_int32, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  inline proc =(ref a:AtomicT, const ref b:AtomicT) {
    a.write(b.read());
  }
  inline proc =(ref a:AtomicT, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  inline proc =(ref a:atomic_real32, const ref b:atomic_real32) {
    a.write(b.read());
  }
  inline proc =(ref a:atomic_real32, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  inline proc =(ref a:atomic_real64, const ref b:atomic_real64) {
    a.write(b.read());
  }
  inline proc =(ref a:atomic_real64, b) {
    compilerError("Cannot directly assign atomic variables");
  }

  inline proc +(a:atomic_uint8, b) {
    compilerError("Cannot directly add atomic variables");
    return a;
  }
  inline proc +(a:atomic_uint16, b) {
    compilerError("Cannot directly add atomic variables");
    return a;
  }
  inline proc +(a:atomic_uint32, b) {
    compilerError("Cannot directly add atomic variables");
    return a;
  }
  inline proc +(a:atomic_uint64, b) {
    compilerError("Cannot directly add atomic variables");
    return a;
  }
  inline proc +(a:atomic_int8, b) {
    compilerError("Cannot directly add atomic variables");
    return a;
  }
  inline proc +(a:atomic_int16, b) {
    compilerError("Cannot directly add atomic variables");
    return a;
  }
  inline proc +(a:atomic_int32, b) {
    compilerError("Cannot directly add atomic variables");
    return a;
  }
  inline proc +(a:AtomicT, b) {
    compilerError("Cannot directly add atomic variables");
    return a;
  }
  inline proc +(a:atomic_real32, b) {
    compilerError("Cannot directly add atomic variables");
    return a;
  }
  inline proc +(a:atomic_real64, b) {
    compilerError("Cannot directly add atomic variables");
    return a;
  }

  inline proc -(a:atomic_uint8, b) {
    compilerError("Cannot directly subtract atomic variables");
    return a;
  }
  inline proc -(a:atomic_uint16, b) {
    compilerError("Cannot directly subtract atomic variables");
    return a;
  }
  inline proc -(a:atomic_uint32, b) {
    compilerError("Cannot directly subtract atomic variables");
    return a;
  }
  inline proc -(a:atomic_uint64, b) {
    compilerError("Cannot directly subtract atomic variables");
    return a;
  }
  inline proc -(a:atomic_int8, b) {
    compilerError("Cannot directly subtract atomic variables");
    return a;
  }
  inline proc -(a:atomic_int16, b) {
    compilerError("Cannot directly subtract atomic variables");
    return a;
  }
  inline proc -(a:atomic_int32, b) {
    compilerError("Cannot directly subtract atomic variables");
    return a;
  }
  inline proc -(a:AtomicT, b) {
    compilerError("Cannot directly subtract atomic variables");
    return a;
  }
  inline proc -(a:atomic_real32, b) {
    compilerError("Cannot directly subtract atomic variables");
    return a;
  }
  inline proc -(a:atomic_real64, b) {
    compilerError("Cannot directly subtract atomic variables");
    return a;
  }

  inline proc *(a:atomic_uint8, b) {
    compilerError("Cannot directly multiply atomic variables");
    return a;
  }
  inline proc *(a:atomic_uint16, b) {
    compilerError("Cannot directly multiply atomic variables");
    return a;
  }
  inline proc *(a:atomic_uint32, b) {
    compilerError("Cannot directly multiply atomic variables");
    return a;
  }
  inline proc *(a:atomic_uint64, b) {
    compilerError("Cannot directly multiply atomic variables");
    return a;
  }
  inline proc *(a:atomic_int8, b) {
    compilerError("Cannot directly multiply atomic variables");
    return a;
  }
  inline proc *(a:atomic_int16, b) {
    compilerError("Cannot directly multiply atomic variables");
    return a;
  }
  inline proc *(a:atomic_int32, b) {
    compilerError("Cannot directly multiply atomic variables");
    return a;
  }
  inline proc *(a:AtomicT, b) {
    compilerError("Cannot directly multiply atomic variables");
    return a;
  }
  inline proc *(a:atomic_real32, b) {
    compilerError("Cannot directly multiply atomic variables");
    return a;
  }
  inline proc *(a:atomic_real64, b) {
    compilerError("Cannot directly multiply atomic variables");
    return a;
  }

  inline proc /(a:atomic_uint8, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc /(a:atomic_uint16, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc /(a:atomic_uint32, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc /(a:atomic_uint64, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc /(a:atomic_int8, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc /(a:atomic_int16, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc /(a:atomic_int32, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc /(a:AtomicT, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc /(a:atomic_real32, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc /(a:atomic_real64, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }

  inline proc %(a:atomic_uint8, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc %(a:atomic_uint16, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc %(a:atomic_uint32, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc %(a:atomic_uint64, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc %(a:atomic_int8, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc %(a:atomic_int16, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc %(a:atomic_int32, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc %(a:AtomicT, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc %(a:atomic_real32, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc %(a:atomic_real64, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }


}
