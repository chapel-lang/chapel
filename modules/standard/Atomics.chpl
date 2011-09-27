module Atomics {

  extern type memory_order;

  extern const memory_order_relaxed:memory_order;
  extern const memory_order_consume:memory_order;
  extern const memory_order_acquire:memory_order;
  extern const memory_order_release:memory_order;
  extern const memory_order_acq_rel:memory_order;
  extern const memory_order_seq_cst:memory_order;

  extern type atomic_uint_least8_t;
  extern type atomic_uint_least16_t;
  extern type atomic_uint_least32_t;
  extern type atomic_uint_least64_t;
  extern type atomic_uintptr_t;

  extern type atomic_flag;

  extern proc atomic_thread_fence(order:memory_order);
  extern proc atomic_signal_thread_fence(order:memory_order);

  extern proc atomic_flag_test_and_set_explicit(inout obj:atomic_flag, order:memory_order);
  extern proc atomic_flag_test_and_set(inout obj:atomic_flag);

  extern proc atomic_flag_clear_explicit(inout obj:atomic_flag, order:memory_order);
  extern proc atomic_flag_clear(inout obj:atomic_flag);

  extern proc atomic_is_lock_free_uint_least8_t(inout obj:atomic_uint_least8_t):bool;
  extern proc atomic_init_uint_least8_t(inout obj:atomic_uint_least8_t, value:uint(8));
  extern proc atomic_destroy_uint_least8_t(inout obj:atomic_uint_least8_t);
  extern proc atomic_store_explicit_uint_least8_t(inout obj:atomic_uint_least8_t, value:uint(8), order:memory_order);
  extern proc atomic_load_explicit_uint_least8_t(inout obj:atomic_uint_least8_t, order:memory_order):uint(8);
  extern proc atomic_exchange_explicit_uint_least8_t(inout obj:atomic_uint_least8_t, value:uint(8), order:memory_order):uint(8);
  extern proc atomic_compare_exchange_strong_explicit_uint_least8_t(inout obj:atomic_uint_least8_t, expected:uint(8), desired:uint(8), order:memory_order):bool;
  extern proc atomic_compare_exchange_weak_explicit_uint_least8_t(inout obj:atomic_uint_least8_t, expected:uint(8), desired:uint(8), order:memory_order):bool;
  extern proc atomic_fetch_add_explicit_uint_least8_t(inout obj:atomic_uint_least8_t, operand:uint(8), order:memory_order):uint(8);
  extern proc atomic_fetch_sub_explicit_uint_least8_t(inout obj:atomic_uint_least8_t, operand:uint(8), order:memory_order):uint(8);
  extern proc atomic_fetch_or_explicit_uint_least8_t(inout obj:atomic_uint_least8_t, operand:uint(8), order:memory_order):uint(8);
  extern proc atomic_fetch_xor_explicit_uint_least8_t(inout obj:atomic_uint_least8_t, operand:uint(8), order:memory_order):uint(8);
  extern proc atomic_fetch_and_explicit_uint_least8_t(inout obj:atomic_uint_least8_t, operand:uint(8), order:memory_order):uint(8);

  extern proc atomic_is_lock_free_uint_least16_t(inout obj:atomic_uint_least16_t):bool;
  extern proc atomic_init_uint_least16_t(inout obj:atomic_uint_least16_t, value:uint(16));
  extern proc atomic_destroy_uint_least16_t(inout obj:atomic_uint_least16_t);
  extern proc atomic_store_explicit_uint_least16_t(inout obj:atomic_uint_least16_t, value:uint(16), order:memory_order);
  extern proc atomic_load_explicit_uint_least16_t(inout obj:atomic_uint_least16_t, order:memory_order):uint(16);
  extern proc atomic_exchange_explicit_uint_least16_t(inout obj:atomic_uint_least16_t, value:uint(16), order:memory_order):uint(16);
  extern proc atomic_compare_exchange_strong_explicit_uint_least16_t(inout obj:atomic_uint_least16_t, expected:uint(16), desired:uint(16), order:memory_order):bool;
  extern proc atomic_compare_exchange_weak_explicit_uint_least16_t(inout obj:atomic_uint_least16_t, expected:uint(16), desired:uint(16), order:memory_order):bool;
  extern proc atomic_fetch_add_explicit_uint_least16_t(inout obj:atomic_uint_least16_t, operand:uint(16), order:memory_order):uint(16);
  extern proc atomic_fetch_sub_explicit_uint_least16_t(inout obj:atomic_uint_least16_t, operand:uint(16), order:memory_order):uint(16);
  extern proc atomic_fetch_or_explicit_uint_least16_t(inout obj:atomic_uint_least16_t, operand:uint(16), order:memory_order):uint(16);
  extern proc atomic_fetch_xor_explicit_uint_least16_t(inout obj:atomic_uint_least16_t, operand:uint(16), order:memory_order):uint(16);
  extern proc atomic_fetch_and_explicit_uint_least16_t(inout obj:atomic_uint_least16_t, operand:uint(16), order:memory_order):uint(16);


  extern proc atomic_is_lock_free_uint_least32_t(inout obj:atomic_uint_least32_t):bool;
  extern proc atomic_init_uint_least32_t(inout obj:atomic_uint_least32_t, value:uint(32));
  extern proc atomic_destroy_uint_least32_t(inout obj:atomic_uint_least32_t);
  extern proc atomic_store_explicit_uint_least32_t(inout obj:atomic_uint_least32_t, value:uint(32), order:memory_order);
  extern proc atomic_load_explicit_uint_least32_t(inout obj:atomic_uint_least32_t, order:memory_order):uint(32);
  extern proc atomic_exchange_explicit_uint_least32_t(inout obj:atomic_uint_least32_t, value:uint(32), order:memory_order):uint(32);
  extern proc atomic_compare_exchange_strong_explicit_uint_least32_t(inout obj:atomic_uint_least32_t, expected:uint(32), desired:uint(32), order:memory_order):bool;
  extern proc atomic_compare_exchange_weak_explicit_uint_least32_t(inout obj:atomic_uint_least32_t, expected:uint(32), desired:uint(32), order:memory_order):bool;
  extern proc atomic_fetch_add_explicit_uint_least32_t(inout obj:atomic_uint_least32_t, operand:uint(32), order:memory_order):uint(32);
  extern proc atomic_fetch_sub_explicit_uint_least32_t(inout obj:atomic_uint_least32_t, operand:uint(32), order:memory_order):uint(32);
  extern proc atomic_fetch_or_explicit_uint_least32_t(inout obj:atomic_uint_least32_t, operand:uint(32), order:memory_order):uint(32);
  extern proc atomic_fetch_xor_explicit_uint_least32_t(inout obj:atomic_uint_least32_t, operand:uint(32), order:memory_order):uint(32);
  extern proc atomic_fetch_and_explicit_uint_least32_t(inout obj:atomic_uint_least32_t, operand:uint(32), order:memory_order):uint(32);

  extern proc atomic_is_lock_free_uint_least64_t(inout obj:atomic_uint_least64_t):bool;
  extern proc atomic_init_uint_least64_t(inout obj:atomic_uint_least64_t, value:uint(64));
  extern proc atomic_destroy_uint_least64_t(inout obj:atomic_uint_least64_t);
  extern proc atomic_store_explicit_uint_least64_t(inout obj:atomic_uint_least64_t, value:uint(64), order:memory_order);
  extern proc atomic_load_explicit_uint_least64_t(inout obj:atomic_uint_least64_t, order:memory_order):uint(64);
  extern proc atomic_exchange_explicit_uint_least64_t(inout obj:atomic_uint_least64_t, value:uint(64), order:memory_order):uint(64);
  extern proc atomic_compare_exchange_strong_explicit_uint_least64_t(inout obj:atomic_uint_least64_t, expected:uint(64), desired:uint(64), order:memory_order):bool;
  extern proc atomic_compare_exchange_weak_explicit_uint_least64_t(inout obj:atomic_uint_least64_t, expected:uint(64), desired:uint(64), order:memory_order):bool;
  extern proc atomic_fetch_add_explicit_uint_least64_t(inout obj:atomic_uint_least64_t, operand:uint(64), order:memory_order):uint(64);
  extern proc atomic_fetch_sub_explicit_uint_least64_t(inout obj:atomic_uint_least64_t, operand:uint(64), order:memory_order):uint(64);
  extern proc atomic_fetch_or_explicit_uint_least64_t(inout obj:atomic_uint_least64_t, operand:uint(64), order:memory_order):uint(64);
  extern proc atomic_fetch_xor_explicit_uint_least64_t(inout obj:atomic_uint_least64_t, operand:uint(64), order:memory_order):uint(64);
  extern proc atomic_fetch_and_explicit_uint_least64_t(inout obj:atomic_uint_least64_t, operand:uint(64), order:memory_order):uint(64);

  extern proc atomic_is_lock_free_uintptr_t(inout obj:atomic_uintptr_t):bool;
  extern proc atomic_init_uintptr_t(inout obj:atomic_uintptr_t, value:c_ptr);
  extern proc atomic_destroy_uintptr_t(inout obj:atomic_uintptr_t);
  extern proc atomic_store_explicit_uintptr_t(inout obj:atomic_uintptr_t, value:c_ptr, order:memory_order);
  extern proc atomic_load_explicit_uintptr_t(inout obj:atomic_uintptr_t, order:memory_order):c_ptr;
  extern proc atomic_exchange_explicit_uintptr_t(inout obj:atomic_uintptr_t, value:c_ptr, order:memory_order):c_ptr;
  extern proc atomic_compare_exchange_strong_explicit_uintptr_t(inout obj:atomic_uintptr_t, expected:c_ptr, desired:c_ptr, order:memory_order):bool;
  extern proc atomic_compare_exchange_weak_explicit_uintptr_t(inout obj:atomic_uintptr_t, expected:c_ptr, desired:c_ptr, order:memory_order):bool;
  extern proc atomic_fetch_add_explicit_uintptr_t(inout obj:atomic_uintptr_t, operand:c_ptr, order:memory_order):c_ptr;
  extern proc atomic_fetch_sub_explicit_uintptr_t(inout obj:atomic_uintptr_t, operand:c_ptr, order:memory_order):c_ptr;
  extern proc atomic_fetch_or_explicit_uintptr_t(inout obj:atomic_uintptr_t, operand:c_ptr, order:memory_order):c_ptr;
  extern proc atomic_fetch_xor_explicit_uintptr_t(inout obj:atomic_uintptr_t, operand:c_ptr, order:memory_order):c_ptr;
  extern proc atomic_fetch_and_explicit_uintptr_t(inout obj:atomic_uintptr_t, operand:c_ptr, order:memory_order):c_ptr;


  // Begin Chapel interface for atomic integers.
  // it would be nice to create records wrapping these data types
  // or create methods for these data types, but right now I'm
  // not up to it.

  record atomic_uint32 {
    var _v:atomic_uint_least32_t;
    proc init(value:uint(32)) {
      atomic_init_uint_least32_t(_v, value);
    }
    proc destroy() {
      atomic_destroy_uint_least32_t(_v);
    }
    proc load(order = memory_order_seq_cst):uint(32) {
      return atomic_load_explicit_uint_least32_t(_v, order);
    }
    proc store(value:uint(32), order = memory_order_seq_cst) {
      atomic_store_explicit_uint_least32_t(_v, value, order);
    }
    proc exchange(value:uint(32), order = memory_order_seq_cst):uint(32) {
      return atomic_exchange_explicit_uint_least32_t(_v, value, order);
    }
    proc compareExchangeWeak(expected:uint(32), desired:uint(32), order = memory_order_seq_cst):bool {
      return atomic_compare_exchange_weak_explicit_uint_least32_t(_v, expected, desired, order);
    }
    proc compareExchangeStrong(expected:uint(32), desired:uint(32), order = memory_order_seq_cst):bool {
      return atomic_compare_exchange_strong_explicit_uint_least32_t(_v, expected, desired, order);
    }
    proc fetchAdd(value:uint(32), order = memory_order_seq_cst):uint(32) {
      return atomic_fetch_add_explicit_uint_least32_t(_v, value, order);
    }
    proc fetchSub(value:uint(32), order = memory_order_seq_cst):uint(32) {
      return atomic_fetch_sub_explicit_uint_least32_t(_v, value, order);
    }
    proc fetchOr(value:uint(32), order = memory_order_seq_cst):uint(32) {
      return atomic_fetch_or_explicit_uint_least32_t(_v, value, order);
    }
    proc fetchXor(value:uint(32), order = memory_order_seq_cst):uint(32) {
      return atomic_fetch_xor_explicit_uint_least32_t(_v, value, order);
    }
    proc fetchAnd(value:uint(32), order = memory_order_seq_cst):uint(32) {
      return atomic_fetch_and_explicit_uint_least32_t(_v, value, order);
    }
  }

  record atomic_uint64 {
    var _v:atomic_uint_least64_t;
    proc init(value:uint(64)) {
      atomic_init_uint_least64_t(_v, value);
    }
    proc destroy() {
      atomic_destroy_uint_least64_t(_v);
    }
    proc load(order = memory_order_seq_cst):uint(64) {
      return atomic_load_explicit_uint_least64_t(_v, order);
    }
    proc store(value:uint(64), order = memory_order_seq_cst) {
      atomic_store_explicit_uint_least64_t(_v, value, order);
    }
    proc exchange(value:uint(64), order = memory_order_seq_cst):uint(64) {
      return atomic_exchange_explicit_uint_least64_t(_v, value, order);
    }
    proc compareExchangeWeak(expected:uint(64), desired:uint(64), order = memory_order_seq_cst):bool {
      return atomic_compare_exchange_weak_explicit_uint_least64_t(_v, expected, desired, order);
    }
    proc compareExchangeStrong(expected:uint(64), desired:uint(64), order = memory_order_seq_cst):bool {
      return atomic_compare_exchange_strong_explicit_uint_least64_t(_v, expected, desired, order);
    }
    proc fetchAdd(value:uint(64), order = memory_order_seq_cst):uint(64) {
      return atomic_fetch_add_explicit_uint_least64_t(_v, value, order);
    }
    proc fetchSub(value:uint(64), order = memory_order_seq_cst):uint(64) {
      return atomic_fetch_sub_explicit_uint_least64_t(_v, value, order);
    }
    proc fetchOr(value:uint(64), order = memory_order_seq_cst):uint(64) {
      return atomic_fetch_or_explicit_uint_least64_t(_v, value, order);
    }
    proc fetchXor(value:uint(64), order = memory_order_seq_cst):uint(64) {
      return atomic_fetch_xor_explicit_uint_least64_t(_v, value, order);
    }
    proc fetchAnd(value:uint(64), order = memory_order_seq_cst):uint(64) {
      return atomic_fetch_and_explicit_uint_least64_t(_v, value, order);
    }
  }
}

