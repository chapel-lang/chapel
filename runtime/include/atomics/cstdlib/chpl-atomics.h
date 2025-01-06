/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#ifndef _chpl_atomics_h_
#define _chpl_atomics_h_

#include "chpltypes.h"
#include "chpl-comp-detect-macros.h"
#include "chpl-tasks.h"

// g++ 4.8 and 4.9 appear to support standard atomics, but have errors
// in their <atomic> header.  Also, gcc 4.8 is missing <stdatomic.h>.
#if RT_COMP_CC == RT_COMP_GCC && RT_COMP_GCC_VERSION_MAJOR < 5
  #error "The cstdlib atomics need at least gcc 5.  Use intrinsics or locks."
#endif

#if defined(__cplusplus)
  #if __cplusplus >= 201103L
    #include <atomic>
    #define Atomic(T) std::atomic<T>
    using std::atomic_int_least8_t;
    using std::atomic_int_least16_t;
    using std::atomic_int_least32_t;
    using std::atomic_int_least64_t;
    using std::atomic_uint_least8_t;
    using std::atomic_uint_least16_t;
    using std::atomic_uint_least32_t;
    using std::atomic_uint_least64_t;
    using std::atomic_uintptr_t;
    using std::atomic_bool;
    using std::memory_order_relaxed;
    using std::memory_order_consume;
    using std::memory_order_acquire;
    using std::memory_order_release;
    using std::memory_order_acq_rel;
    using std::memory_order_seq_cst;
    using std::memory_order;
  #else
    #error "The cstdlib atomics need at least C++11.  Use intrinsics or locks."
  #endif
#elif __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_ATOMICS__)
  #include <stdatomic.h>
  #define Atomic(T) _Atomic T
#else
  #error "The cstdlib atomics need at least C11.  Use intrinsics or locks."
#endif

#ifdef __cplusplus
extern "C" {
#endif


typedef atomic_int_least8_t chpl_atomic_int_least8_t;
typedef atomic_int_least16_t chpl_atomic_int_least16_t;
typedef atomic_int_least32_t chpl_atomic_int_least32_t;
typedef atomic_int_least64_t chpl_atomic_int_least64_t;
typedef atomic_uint_least8_t chpl_atomic_uint_least8_t;
typedef atomic_uint_least16_t chpl_atomic_uint_least16_t;
typedef atomic_uint_least32_t chpl_atomic_uint_least32_t;
typedef atomic_uint_least64_t chpl_atomic_uint_least64_t;
typedef atomic_uintptr_t chpl_atomic_uintptr_t;
typedef atomic_bool chpl_atomic_bool;

typedef Atomic(_real64) chpl_atomic__real64;
typedef Atomic(_real32) chpl_atomic__real32;

typedef chpl_atomic_bool chpl_atomic_spinlock_t;

typedef memory_order chpl_memory_order;
#define chpl_memory_order_relaxed memory_order_relaxed
#define chpl_memory_order_consume memory_order_consume
#define chpl_memory_order_acquire memory_order_acquire
#define chpl_memory_order_release memory_order_release
#define chpl_memory_order_acq_rel memory_order_acq_rel
#define chpl_memory_order_seq_cst memory_order_seq_cst


static inline chpl_memory_order _defaultOfMemoryOrder(void) {
  return chpl_memory_order_seq_cst;
}

static inline void chpl_atomic_thread_fence(chpl_memory_order order)
{
  atomic_thread_fence(order);
}
static inline void chpl_atomic_signal_fence(chpl_memory_order order)
{
  atomic_signal_fence(order);
}


///////////////////////////////////////////////////////////////////////////////
////                      START OF ATOMICS BASE                           ////
//////////////////////////////////////////////////////////////////////////////
#define DECLARE_ATOMICS_BASE(type, basetype) \
static inline chpl_bool atomic_is_lock_free_ ## type(chpl_atomic_ ## type * obj) { \
  return atomic_is_lock_free(obj); \
} \
static inline void atomic_init_ ## type(chpl_atomic_ ## type * obj, basetype value) { \
  atomic_init(obj, value); \
} \
static inline void atomic_destroy_ ## type(chpl_atomic_ ## type * obj) { \
} \
static inline void atomic_store_explicit_ ## type(chpl_atomic_ ## type * obj, basetype value, chpl_memory_order order) { \
  atomic_store_explicit(obj, value, order); \
} \
static inline void atomic_store_ ## type(chpl_atomic_ ## type * obj, basetype value) { \
  atomic_store(obj, value); \
} \
static inline basetype atomic_load_explicit_ ## type(chpl_atomic_ ## type * obj, chpl_memory_order order) { \
  return atomic_load_explicit(obj, order); \
} \
static inline basetype atomic_load_ ## type(chpl_atomic_ ## type * obj) { \
  return atomic_load(obj); \
}


///////////////////////////////////////////////////////////////////////////////
////                 START OF ATOMIC EXCHANGE OPS                         ////
//////////////////////////////////////////////////////////////////////////////
#define DECLARE_ATOMICS_EXCHANGE_OPS(type, basetype) \
static inline basetype atomic_exchange_explicit_ ## type(chpl_atomic_ ## type * obj, basetype value, chpl_memory_order order) { \
  return atomic_exchange_explicit(obj, value, order); \
} \
static inline basetype atomic_exchange_ ## type(chpl_atomic_ ## type * obj, basetype value) { \
  return atomic_exchange(obj, value); \
} \
static inline chpl_bool atomic_compare_exchange_strong_explicit_ ## type(chpl_atomic_ ## type * obj, basetype * expected, basetype desired, chpl_memory_order succ, chpl_memory_order fail) { \
  return atomic_compare_exchange_strong_explicit(obj, expected, desired, succ, fail); \
} \
static inline chpl_bool atomic_compare_exchange_strong_ ## type(chpl_atomic_ ## type * obj, basetype * expected, basetype desired) { \
  return atomic_compare_exchange_strong(obj, expected, desired); \
} \
static inline chpl_bool atomic_compare_exchange_weak_explicit_ ## type(chpl_atomic_ ## type * obj, basetype * expected, basetype desired, chpl_memory_order succ, chpl_memory_order fail) { \
  return atomic_compare_exchange_weak_explicit(obj, expected, desired, succ, fail); \
} \
static inline chpl_bool atomic_compare_exchange_weak_ ## type(chpl_atomic_ ## type * obj, basetype * expected, basetype desired) { \
  return atomic_compare_exchange_weak(obj, expected, desired); \
}


///////////////////////////////////////////////////////////////////////////////
////              START OF INTEGER ATOMIC FETCH OPERATIONS                ////
//////////////////////////////////////////////////////////////////////////////
#define DECLARE_ATOMICS_FETCH_OPS(type) \
static inline type atomic_fetch_add_explicit_ ## type(chpl_atomic_ ## type * obj, type operand, chpl_memory_order order) { \
  return atomic_fetch_add_explicit(obj, operand, order); \
} \
static inline type atomic_fetch_add_ ## type(chpl_atomic_ ## type * obj, type operand) { \
  return atomic_fetch_add(obj, operand); \
} \
static inline type atomic_fetch_sub_explicit_ ## type(chpl_atomic_ ## type * obj, type operand, chpl_memory_order order) { \
  return atomic_fetch_sub_explicit(obj, operand, order); \
} \
static inline type atomic_fetch_sub_ ## type(chpl_atomic_ ## type * obj, type operand) { \
  return atomic_fetch_sub(obj, operand); \
} \
static inline type atomic_fetch_or_explicit_ ## type(chpl_atomic_ ## type * obj, type operand, chpl_memory_order order) { \
  return atomic_fetch_or_explicit(obj, operand, order); \
} \
static inline type atomic_fetch_or_ ## type(chpl_atomic_ ## type * obj, type operand) { \
  return atomic_fetch_or(obj, operand); \
} \
static inline type atomic_fetch_and_explicit_ ## type(chpl_atomic_ ## type * obj, type operand, chpl_memory_order order) { \
  return atomic_fetch_and_explicit(obj, operand, order); \
} \
static inline type atomic_fetch_and_ ## type(chpl_atomic_ ## type * obj, type operand) { \
  return atomic_fetch_and(obj, operand); \
} \
static inline type atomic_fetch_xor_explicit_ ## type(chpl_atomic_ ## type * obj, type operand, chpl_memory_order order) { \
  return atomic_fetch_xor_explicit(obj, operand, order); \
} \
static inline type atomic_fetch_xor_ ## type(chpl_atomic_ ## type * obj, type operand) { \
  return atomic_fetch_xor(obj, operand); \
}


//
// Given an input memory order, find a memory order that can be used for
// an atomic_load or for the compare_exchange failure case. This is used
// for implementing floating point fetch ops, which are only passed a
// single memory order.
//
// The C standard requires that an atomic_load or the compare_exchange
// failure case not have chpl_memory_order_release or chpl_memory_order_acq_rel.
// In addition, the compare_exchange failure case cannot have a stronger
// order than the success case. We choose the strongest order we can
// that satisfies those requirements.
//
#define GET_READABLE_ORDER(order) (order != chpl_memory_order_release && \
                                   order != chpl_memory_order_acq_rel ? \
                                   order : chpl_memory_order_acquire)

///////////////////////////////////////////////////////////////////////////////
////                   START OF REAL ATOMICS FETCH OPS                    ////
//////////////////////////////////////////////////////////////////////////////
#define DECLARE_REAL_ATOMICS_FETCH_OPS(type) \
static inline type atomic_fetch_add_explicit_ ## type(chpl_atomic_ ## type * obj, type operand, chpl_memory_order order) { \
  chpl_memory_order ld_exp_order = GET_READABLE_ORDER(order); \
  type old_val = atomic_load_explicit(obj, ld_exp_order); \
  type new_val; \
  do { \
    new_val = old_val + operand; \
  } while (!atomic_compare_exchange_weak_explicit(obj, &old_val, new_val, order, ld_exp_order)); \
  return old_val; \
} \
static inline type atomic_fetch_add_ ## type(chpl_atomic_ ## type * obj, type operand) { \
  return atomic_fetch_add_explicit_ ## type(obj, operand, chpl_memory_order_seq_cst); \
} \
static inline type atomic_fetch_sub_explicit_ ## type(chpl_atomic_ ## type * obj, type operand, chpl_memory_order order) { \
  chpl_memory_order ld_exp_order = GET_READABLE_ORDER(order); \
  type old_val = atomic_load_explicit(obj, ld_exp_order); \
  type new_val; \
  do { \
    new_val = old_val - operand; \
  } while (!atomic_compare_exchange_weak_explicit(obj, &old_val, new_val, order, ld_exp_order)); \
  return old_val; \
} \
static inline type atomic_fetch_sub_ ## type(chpl_atomic_ ## type * obj, type operand) { \
  return atomic_fetch_sub_explicit_ ## type(obj, operand, chpl_memory_order_seq_cst); \
}


// Actually declare the atomics for integer and real types using the above macros
DECLARE_ATOMICS_BASE(bool, chpl_bool);
DECLARE_ATOMICS_EXCHANGE_OPS(bool, chpl_bool);


#define DECLARE_ATOMICS(type) \
  DECLARE_ATOMICS_BASE(type, type) \
  DECLARE_ATOMICS_EXCHANGE_OPS(type, type); \
  DECLARE_ATOMICS_FETCH_OPS(type)

DECLARE_ATOMICS(int_least8_t);
DECLARE_ATOMICS(int_least16_t);
DECLARE_ATOMICS(int_least32_t);
DECLARE_ATOMICS(int_least64_t);
DECLARE_ATOMICS(uint_least8_t);
DECLARE_ATOMICS(uint_least16_t);
DECLARE_ATOMICS(uint_least32_t);
DECLARE_ATOMICS(uint_least64_t);

// On netbsd the DECLARE_ATOMICS macro doesn't work for uintptr_t.
DECLARE_ATOMICS_BASE(uintptr_t, uintptr_t);
DECLARE_ATOMICS_EXCHANGE_OPS(uintptr_t, uintptr_t);
DECLARE_ATOMICS_FETCH_OPS(uintptr_t);


#define DECLARE_REAL_ATOMICS(type) \
  DECLARE_ATOMICS_BASE(type, type) \
  DECLARE_ATOMICS_EXCHANGE_OPS(type, type) \
  DECLARE_REAL_ATOMICS_FETCH_OPS(type)

DECLARE_REAL_ATOMICS(_real32);
DECLARE_REAL_ATOMICS(_real64);


#undef DECLARE_ATOMICS_BASE
#undef DECLARE_ATOMICS_EXCHANGE_OPS
#undef DECLARE_ATOMICS_FETCH_OPS
#undef DECLARE_REAL_ATOMICS_FETCH_OPS
#undef DECLARE_ATOMICS
#undef DECLARE_REAL_ATOMICS

static inline void atomic_init_spinlock_t(chpl_atomic_spinlock_t* lock) {
  atomic_init(lock, false);
}

static inline void atomic_destroy_spinlock_t(chpl_atomic_spinlock_t* lock) { }

static inline chpl_bool atomic_try_lock_spinlock_t(chpl_atomic_spinlock_t* lock) {
  return !atomic_load(lock) && !atomic_exchange_explicit(lock, true, chpl_memory_order_acquire);
}

static inline void atomic_lock_spinlock_t(chpl_atomic_spinlock_t* lock) {
  while(!atomic_try_lock_spinlock_t(lock)) {
    chpl_task_yield();
  }
}

static inline void atomic_unlock_spinlock_t(chpl_atomic_spinlock_t* lock) {
  atomic_store_explicit(lock, false, chpl_memory_order_release);
}

#ifdef __cplusplus
}
#endif

#endif // _chpl_atomics_h_
