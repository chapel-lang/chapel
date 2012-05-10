#ifndef _chpl_atomics_h_
#define _chpl_atomics_h_

#ifdef SIMPLE_TEST
#include "sys_basic.h"
#include <stdint.h>
#include <stdbool.h>
typedef bool chpl_bool;
#else
#include "chpltypes.h" // chpl_bool -- but that undefines printf, which
                       // causes problems with the C test cases.
#endif

typedef int_least8_t atomic_int_least8_t;
typedef int_least16_t atomic_int_least16_t;
typedef int_least32_t atomic_int_least32_t;
typedef int_least64_t atomic_int_least64_t;
typedef uint_least8_t atomic_uint_least8_t;
typedef uint_least16_t atomic_uint_least16_t;
typedef uint_least32_t atomic_uint_least32_t;
typedef uint_least64_t atomic_uint_least64_t;
typedef uintptr_t atomic_uintptr_t;
typedef chpl_bool atomic_flag;

typedef enum {
 memory_order_relaxed,
 memory_order_consume,
 memory_order_acquire,
 memory_order_release,
 memory_order_acq_rel,
 memory_order_seq_cst
} memory_order;

static inline
void atomic_thread_fence(memory_order order)
{
  __sync_synchronize();
}
static inline
void atomic_signal_thread_fence(memory_order order)
{
  __sync_synchronize();
}

static inline chpl_bool atomic_flag_test_and_set_explicit(atomic_flag *obj, memory_order order) {
  chpl_bool ret;
  ret = __sync_lock_test_and_set(obj, 1);
  if(order == memory_order_release ||
     order == memory_order_acq_rel ||
     order == memory_order_seq_cst )
    __sync_synchronize();
  
  return ret;
}
static inline chpl_bool atomic_flag_test_and_set(atomic_flag *obj) {
  return atomic_flag_test_and_set_explicit(obj, memory_order_seq_cst);
}

static inline void atomic_flag_clear_explicit(atomic_flag *obj, memory_order order) {
  *obj = false;
  __sync_synchronize();
}
static inline void atomic_flag_clear(atomic_flag *obj) {
  atomic_flag_clear_explicit(obj, memory_order_seq_cst);
}

#define DECLARE_ATOMICS_BASE(type, basetype) \
static inline chpl_bool atomic_is_lock_free_ ## type(atomic_ ## type * obj) { \
  return true; \
} \
static inline void atomic_init_ ## type(atomic_ ## type * obj, basetype value) { \
  *obj = value; \
} \
static inline void atomic_destroy_ ## type(atomic_ ## type * obj) { \
} \
static inline void atomic_store_explicit_ ## type(atomic_ ## type * obj, basetype value, memory_order order) { \
  *obj = value; \
  __sync_synchronize(); \
} \
static inline void atomic_store_ ## type(atomic_ ## type * obj, basetype value) { \
  atomic_store_explicit_ ## type(obj, value, memory_order_seq_cst); \
} \
static inline basetype atomic_load_explicit_ ## type(atomic_ ## type * obj, memory_order order) { \
  __sync_synchronize(); \
  return *obj; \
} \
static inline basetype atomic_load_ ## type(atomic_ ## type * obj) { \
  return atomic_load_explicit_ ## type(obj, memory_order_seq_cst); \
} \
static inline basetype atomic_exchange_explicit_ ## type(atomic_ ## type * obj, basetype value, memory_order order) { \
  return __sync_lock_test_and_set(obj, value); \
} \
static inline basetype atomic_exchange_ ## type(atomic_ ## type * obj, basetype value) { \
  return atomic_exchange_explicit_ ## type(obj, value, memory_order_seq_cst); \
} \
static inline chpl_bool atomic_compare_exchange_strong_explicit_ ## type(atomic_ ## type * obj, basetype expected, basetype desired, memory_order order) { \
  return __sync_bool_compare_and_swap(obj, expected, desired); \
} \
static inline chpl_bool atomic_compare_exchange_strong_ ## type(atomic_ ## type * obj, basetype expected, basetype desired) { \
  return atomic_compare_exchange_strong_explicit_ ## type(obj, expected, desired, memory_order_seq_cst); \
} \
static inline chpl_bool atomic_compare_exchange_weak_explicit_ ## type(atomic_ ## type * obj, basetype expected, basetype desired, memory_order order) { \
  return atomic_compare_exchange_strong_explicit_ ## type(obj, expected, desired, order); \
} \
static inline chpl_bool atomic_compare_exchange_weak_ ## type(atomic_ ## type * obj, basetype expected, basetype desired) { \
  return atomic_compare_exchange_weak_explicit_ ## type(obj, expected, desired, memory_order_seq_cst); \
}

#define DECLARE_ATOMICS_FETCH_OPS(type) \
static inline type atomic_fetch_add_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  return __sync_fetch_and_add(obj, operand); \
} \
static inline type atomic_fetch_add_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_add_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \
static inline type atomic_fetch_sub_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  return __sync_fetch_and_sub(obj, operand); \
} \
static inline type atomic_fetch_sub_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_sub_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \
static inline type atomic_fetch_or_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  return __sync_fetch_and_or(obj, operand); \
} \
static inline type atomic_fetch_or_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_or_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \
static inline type atomic_fetch_and_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  return __sync_fetch_and_and(obj, operand); \
} \
static inline type atomic_fetch_and_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_and_explicit_ ## type(obj, operand, memory_order_seq_cst); \
}

DECLARE_ATOMICS_BASE(flag, chpl_bool);

#define DECLARE_ATOMICS(type) \
  DECLARE_ATOMICS_BASE(type, type) \
  DECLARE_ATOMICS_FETCH_OPS(type)

DECLARE_ATOMICS(int_least8_t);
DECLARE_ATOMICS(int_least16_t);
DECLARE_ATOMICS(int_least32_t);
DECLARE_ATOMICS(int_least64_t);
DECLARE_ATOMICS(uint_least8_t);
DECLARE_ATOMICS(uint_least16_t);
DECLARE_ATOMICS(uint_least32_t);
DECLARE_ATOMICS(uint_least64_t);
DECLARE_ATOMICS(uintptr_t);

#undef DECLARE_ATOMICS_BASE
#undef DECLARE_ATOMICS_FETCH_OPS
#undef DECLARE_ATOMICS

/*
#define atomic_is_lock_free(obj) (1)
#define atomic_init(obj, value) (*obj = value)
#define atomic_store_explicit(obj, value, order) ({*obj = value; __sync_synchronize(); })
#define atomic_store(obj, value) (atomic_store_explicit(obj, value, memory_order_seq_cst))
#define atomic_load_explicit(obj, value, order) ({__sync_synchronize(); *obj; })
#define atomic_load(obj, value) (atomic_load_explicit(obj, value, memory_order_seq_cst))

// GCC 4.6 doesn't have a __sync_swap, but CLANG does.
// The GCC manual says __sync_lock_test_and_set might work there,
//  but some platforms only support 1 as the value..
#define atomic_exchange_explicit(obj, value, order) ({ __sync_lock_test_and_set(obj, value); __sync_synchronize(); })
#define atomic_exchange(obj, value) (atomic_exchange_explicit(obj, value, memory_order_seq_cst))

#define atomic_compare_exchange_strong_explicit(obj, expected, desired, order) (__sync_bool_compare_and_swap(obj, expected, desired))
#define atomic_compare_exchange_strong(obj, expected, desired) (atomic_compare_exchange_weak_explicit(obj, expected, desired, memory_order_seq_cst))
#define atomic_compare_exchange_weak_explicit(obj, expected, desired, order) (__sync_bool_compare_and_swap(obj, expected, desired))
#define atomic_compare_exchange_weak(obj, expected, desired) (atomic_compare_exchange_weak_explicit(obj, expected, desired, memory_order_seq_cst))

#define atomic_fetch_add_explicit(obj, operand, order) (__sync_fetch_and_add(obj, operand))
#define atomic_fetch_add(obj, operand) (__sync_fetch_and_add(obj, operand, memory_order_seq_cst))

#define atomic_fetch_sub_explicit(obj, operand, order) (__sync_fetch_and_sub(obj, operand))
#define atomic_fetch_sub(obj, operand) (__sync_fetch_and_sub(obj, operand, memory_order_seq_cst))

#define atomic_fetch_or_explicit(obj, operand, order) (__sync_fetch_and_or(obj, operand))
#define atomic_fetch_or(obj, operand) (__sync_fetch_and_or(obj, operand, memory_order_seq_cst))

#define atomic_fetch_xor_explicit(obj, operand, order) (__sync_fetch_and_xor(obj, operand))
#define atomic_fetch_xor(obj, operand) (__sync_fetch_and_xor(obj, operand, memory_order_seq_cst))

#define atomic_fetch_and_explicit(obj, operand, order) (__sync_fetch_and_and(obj, operand))
#define atomic_fetch_and(obj, operand) (__sync_fetch_and_and(obj, operand, memory_order_seq_cst))

#define atomic_flag_test_and_set_explicit(obj, order) {( \
  uint8_t ret; \
  ret = __sync_lock_test_and_set(obj, 1); \
  if(order == memory_order_release || \
     order == memory_order_acq_rel || \
     order == memory_order_seq_cst ) \
    __sync_synchronize(); \
  \
  ret; \
)}
 
#define atomic_flag_test_and_set(obj) (atomic_flag_test_and_set_explicit(obj, memory_order_seq_cst))

#define atomic_flag_test_and_set_explicit(obj, order) {( \
  *obj = 0; \
  if(order == memory_order_release || \
     order == memory_order_acq_rel || \
     order == memory_order_seq_cst ) \
    __sync_synchronize(); \
  \
  ret; \
)}
 
#define atomic_flag_clear(obj) (atomic_flag_clear_explicit(obj, memory_order_seq_cst))
*/

static inline int leadz8(uint8_t x) {
  return __builtin_clz((unsigned int) x) - 8*(sizeof(unsigned int)-sizeof(uint8_t));
}
static inline int leadz16(uint16_t x) {
  return __builtin_clz((unsigned int) x) - 8*(sizeof(unsigned int)-sizeof(uint16_t));
}
static inline int leadz32(uint32_t x) {
  return __builtin_clz((unsigned int) x);
}
static inline int leadz64(uint64_t x) {
  return __builtin_clzll((unsigned long long) x);
}

#endif // _chpl_atomics_h_
