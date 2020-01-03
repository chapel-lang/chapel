/*
 * Copyright 2004-2020 Cray Inc.
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

typedef Atomic(_real64) atomic__real64;
typedef Atomic(_real32) atomic__real32;

static inline memory_order _defaultOfMemoryOrder(void) {
  return memory_order_seq_cst;
}

static inline void chpl_atomic_thread_fence(memory_order order)
{
  atomic_thread_fence(order);
}
static inline void chpl_atomic_signal_fence(memory_order order)
{
  atomic_signal_fence(order);
}


//
// Given an input memory order, find a memory order that can be used for
// an atomic_load or for the compare_exchange failure case.  This is an
// issue because the Chapel API does not currently make the compare_exchange
// "expected" parameter a pointer.  As a result, the user does not pass in a
// memory order for the failure case, and it must be inferred from the order
// they provided for the success case.  In the long run, we expect to migrate
// to an API that matches the C standard API.
//
// The C standard requires that an atomic_load or the compare_exchange
// failure case not have memory_order_release or memory_order_acq_rel.
// In addition, the compare_exchange failure case cannot have a stronger
// order than the success case.  We choose the strongest order we can
// that satisfies those requirements.
//
#define GET_READABLE_ORDER(order) (order != memory_order_release && \
                                   order != memory_order_acq_rel ? \
                                   order : memory_order_acquire)


///////////////////////////////////////////////////////////////////////////////
////                      START OF ATOMICS BASE                           ////
//////////////////////////////////////////////////////////////////////////////
#define DECLARE_ATOMICS_BASE(type, basetype) \
static inline chpl_bool atomic_is_lock_free_ ## type(atomic_ ## type * obj) { \
  return atomic_is_lock_free(obj); \
} \
static inline void atomic_init_ ## type(atomic_ ## type * obj, basetype value) { \
  atomic_init(obj, value); \
} \
static inline void atomic_destroy_ ## type(atomic_ ## type * obj) { \
} \
static inline void atomic_store_explicit_ ## type(atomic_ ## type * obj, basetype value, memory_order order) { \
  atomic_store_explicit(obj, value, order); \
} \
static inline void atomic_store_ ## type(atomic_ ## type * obj, basetype value) { \
  atomic_store(obj, value); \
} \
static inline basetype atomic_load_explicit_ ## type(atomic_ ## type * obj, memory_order order) { \
  return atomic_load_explicit(obj, order); \
} \
static inline basetype atomic_load_ ## type(atomic_ ## type * obj) { \
  return atomic_load(obj); \
}


///////////////////////////////////////////////////////////////////////////////
////                 START OF ATOMIC EXCHANGE OPS                         ////
//////////////////////////////////////////////////////////////////////////////
#define DECLARE_ATOMICS_EXCHANGE_OPS(type, basetype) \
static inline basetype atomic_exchange_explicit_ ## type(atomic_ ## type * obj, basetype value, memory_order order) { \
  return atomic_exchange_explicit(obj, value, order); \
} \
static inline basetype atomic_exchange_ ## type(atomic_ ## type * obj, basetype value) { \
  return atomic_exchange(obj, value); \
} \
static inline chpl_bool atomic_compare_exchange_strong_explicit_ ## type(atomic_ ## type * obj, basetype expected, basetype desired, memory_order order) { \
  memory_order exp_order = GET_READABLE_ORDER(order); \
  return atomic_compare_exchange_strong_explicit(obj, &expected, desired, order, exp_order); \
} \
static inline chpl_bool atomic_compare_exchange_strong_ ## type(atomic_ ## type * obj, basetype expected, basetype desired) { \
  return atomic_compare_exchange_strong(obj, &expected, desired); \
} \
static inline chpl_bool atomic_compare_exchange_weak_explicit_ ## type(atomic_ ## type * obj, basetype expected, basetype desired, memory_order order) { \
  memory_order exp_order = GET_READABLE_ORDER(order); \
  return atomic_compare_exchange_weak_explicit(obj, &expected, desired, order, exp_order); \
} \
static inline chpl_bool atomic_compare_exchange_weak_ ## type(atomic_ ## type * obj, basetype expected, basetype desired) { \
  return atomic_compare_exchange_weak(obj, &expected, desired); \
}


///////////////////////////////////////////////////////////////////////////////
////              START OF INTEGER ATOMIC FETCH OPERATIONS                ////
//////////////////////////////////////////////////////////////////////////////
#define DECLARE_ATOMICS_FETCH_OPS(type) \
static inline type atomic_fetch_add_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  return atomic_fetch_add_explicit(obj, operand, order); \
} \
static inline type atomic_fetch_add_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_add(obj, operand); \
} \
static inline type atomic_fetch_sub_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  return atomic_fetch_sub_explicit(obj, operand, order); \
} \
static inline type atomic_fetch_sub_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_sub(obj, operand); \
} \
static inline type atomic_fetch_or_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  return atomic_fetch_or_explicit(obj, operand, order); \
} \
static inline type atomic_fetch_or_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_or(obj, operand); \
} \
static inline type atomic_fetch_and_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  return atomic_fetch_and_explicit(obj, operand, order); \
} \
static inline type atomic_fetch_and_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_and(obj, operand); \
} \
static inline type atomic_fetch_xor_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  return atomic_fetch_xor_explicit(obj, operand, order); \
} \
static inline type atomic_fetch_xor_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_xor(obj, operand); \
}


///////////////////////////////////////////////////////////////////////////////
////                   START OF REAL ATOMICS FETCH OPS                    ////
//////////////////////////////////////////////////////////////////////////////
#define DECLARE_REAL_ATOMICS_FETCH_OPS(type) \
static inline type atomic_fetch_add_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  memory_order ld_exp_order = GET_READABLE_ORDER(order); \
  type old_val = atomic_load_explicit(obj, ld_exp_order); \
  type new_val; \
  do { \
    new_val = old_val + operand; \
  } while (!atomic_compare_exchange_weak_explicit(obj, &old_val, new_val, order, ld_exp_order)); \
  return old_val; \
} \
static inline type atomic_fetch_add_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_add_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \
static inline type atomic_fetch_sub_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  memory_order ld_exp_order = GET_READABLE_ORDER(order); \
  type old_val = atomic_load_explicit(obj, ld_exp_order); \
  type new_val; \
  do { \
    new_val = old_val - operand; \
  } while (!atomic_compare_exchange_weak_explicit(obj, &old_val, new_val, order, ld_exp_order)); \
  return old_val; \
} \
static inline type atomic_fetch_sub_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_sub_explicit_ ## type(obj, operand, memory_order_seq_cst); \
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

// On netbsd the DECLARE_ATOMICS macro doesn't work for uintptr_t, so we
// call out the individual parts explicitly.  For more background, see the
// comment in the intrinsics implementation of atomics.
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

#endif // _chpl_atomics_h_
