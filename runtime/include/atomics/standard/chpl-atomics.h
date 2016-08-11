/*
 * Copyright 2004-2016 Cray Inc.
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
    #error "Standard atomics need at least C++11.  Use intrinsics or locks."
  #endif
#elif __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_ATOMICS__)
  #include <stdatomic.h>
  #define Atomic(T) _Atomic T
#else
  #error "Standard atomics need at least C11.  Use intrinsics or locks."
#endif

typedef Atomic(_real64) atomic__real64;
typedef Atomic(_real32) atomic__real32;

static inline memory_order _defaultOfMemoryOrder(void) {
  return memory_order_seq_cst;
}


///////////////////////////////////////////////////////////////////////////////
////                      START OF INTEGER ATOMICS BASE                   ////
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
////                 START OF INTEGER ATOMIC EXCHANGE OPS                 ////
//////////////////////////////////////////////////////////////////////////////
#define DECLARE_ATOMICS_EXCHANGE_OPS(type, basetype) \
static inline basetype atomic_exchange_explicit_ ## type(atomic_ ## type * obj, basetype value, memory_order order) { \
  return atomic_exchange_explicit(obj, value, order); \
} \
static inline basetype atomic_exchange_ ## type(atomic_ ## type * obj, basetype value) { \
  return atomic_exchange(obj, value); \
} \
static inline chpl_bool atomic_compare_exchange_strong_explicit_ ## type(atomic_ ## type * obj, basetype expected, basetype desired, memory_order order) { \
  return atomic_compare_exchange_strong_explicit(obj, &expected, desired, order, order); \
} \
static inline chpl_bool atomic_compare_exchange_strong_ ## type(atomic_ ## type * obj, basetype expected, basetype desired) { \
  return atomic_compare_exchange_strong(obj, &expected, desired); \
} \
static inline chpl_bool atomic_compare_exchange_weak_explicit_ ## type(atomic_ ## type * obj, basetype expected, basetype desired, memory_order order) { \
  return atomic_compare_exchange_weak_explicit(obj, &expected, desired, order, order); \
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
////                       START OF REAL ATOMICS BASE                     ////
//////////////////////////////////////////////////////////////////////////////
#define DECLARE_REAL_ATOMICS_BASE(type) \
static inline chpl_bool atomic_is_lock_free_ ## type(atomic_ ## type * obj) { \
  return atomic_is_lock_free(obj); \
} \
static inline void atomic_init_ ## type(atomic_ ## type * obj, type value) { \
  atomic_init(obj, value); \
} \
static inline void atomic_destroy_ ## type(atomic_ ## type * obj) { \
} \
static inline void atomic_store_explicit_ ## type(atomic_ ## type * obj, type value, memory_order order) { \
  atomic_store_explicit(obj, value, order); \
} \
static inline void atomic_store_ ## type(atomic_ ## type * obj, type value) { \
  atomic_store(obj, value); \
} \
static inline type atomic_load_explicit_ ## type(atomic_ ## type * obj, memory_order order) { \
  return atomic_load_explicit(obj, order); \
} \
static inline type atomic_load_ ## type(atomic_ ## type * obj) { \
  return atomic_load(obj); \
}


///////////////////////////////////////////////////////////////////////////////
////                START OF REAL ATOMICS EXCHANGE OPS                    ////
//////////////////////////////////////////////////////////////////////////////
#define DECLARE_REAL_ATOMICS_EXCHANGE_OPS(type) \
static inline type atomic_exchange_explicit_ ## type(atomic_ ## type * obj, type _value, memory_order order) { \
  return atomic_exchange_explicit(obj, _value, order); \
} \
static inline type atomic_exchange_ ## type(atomic_ ## type * obj, type value) { \
  return atomic_exchange(obj, value); \
} \
static inline chpl_bool atomic_compare_exchange_strong_explicit_ ## type(atomic_ ## type * obj, type expected, type desired, memory_order order) { \
  return atomic_compare_exchange_strong_explicit(obj, &expected, desired, order, order); \
} \
static inline chpl_bool atomic_compare_exchange_strong_ ## type(atomic_ ## type * obj, type expected, type desired) { \
  return atomic_compare_exchange_strong(obj, &expected, desired); \
} \
static inline chpl_bool atomic_compare_exchange_weak_explicit_ ## type(atomic_ ## type * obj, type expected, type desired, memory_order order) { \
  return atomic_compare_exchange_weak_explicit(obj, &expected, desired, order, order); \
} \
static inline chpl_bool atomic_compare_exchange_weak_ ## type(atomic_ ## type * obj, type expected, type desired) { \
  return atomic_compare_exchange_weak(obj, &expected, desired); \
}


///////////////////////////////////////////////////////////////////////////////
////                   START OF REAL ATOMICS FETCH OPS                    ////
//////////////////////////////////////////////////////////////////////////////
#define DECLARE_REAL_ATOMICS_FETCH_OPS(type) \
static inline type atomic_fetch_add_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type old_val = *obj; \
  type new_val; \
  do { \
    new_val = old_val + operand; \
  } while (!atomic_compare_exchange_weak_explicit(obj, &old_val, new_val, order, order)); \
  return old_val; \
} \
static inline type atomic_fetch_add_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_add_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \
static inline type atomic_fetch_sub_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type old_val = *obj; \
  type new_val; \
  do { \
    new_val = old_val - operand; \
  } while (!atomic_compare_exchange_weak_explicit(obj, &old_val, new_val, order, order)); \
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

// On netbsd the DECLARE_ATOMICS macro doesn't work for uintptr_t. From gbt:
// The root of the problem is the fact the on netbsd <stdint.h> (indirectly via
// <sys.stdint.h>) #defines uintptr_t as __uintptr_t.  (__uintptr_t is in turn
// typedef'd as unsigned long, but that doesn't matter to us.)  The C standard
// (6.3.10.1(1) for C99) says that the actual arguments in a macro invocation
// are themselves macro-expanded before being substituted into the replacement
// text, unless they are preceded by a # or ## token.  In our case, the "type"
// formal argument of DECLARE_ATOMICS_BASE has a ## before it in the
// replacement text, so uintptr_t is not macro-expanded and the concatenation
// produces the expected atomic_uintptr_t typedef name.  But in the case of
// DECLARE_ATOMICS there is neither a # or ## before "type" in the replacement
// text, so the uintptr_t actual becomes __uintptr_t via macro expansion before
// DECLARE_ATOMICS_BASE is invoked, and we get a syntax error on the resulting
// atomic___uintptr_t because it's not a typedef type.
DECLARE_ATOMICS_BASE(uintptr_t, uintptr_t);
DECLARE_ATOMICS_EXCHANGE_OPS(uintptr_t, uintptr_t);
DECLARE_ATOMICS_FETCH_OPS(uintptr_t);


#define DECLARE_REAL_ATOMICS(type) \
  DECLARE_REAL_ATOMICS_BASE(type) \
  DECLARE_REAL_ATOMICS_EXCHANGE_OPS(type) \
  DECLARE_REAL_ATOMICS_FETCH_OPS(type)

DECLARE_REAL_ATOMICS(_real32);
DECLARE_REAL_ATOMICS(_real64);


#undef DECLARE_ATOMICS_BASE
#undef DECLARE_ATOMICS_EXCHANGE_OPS
#undef DECLARE_ATOMICS_FETCH_OPS
#undef DECLARE_REAL_ATOMICS_BASE
#undef DECLARE_REAL_ATOMICS_EXCHANGE_OPS
#undef DECLARE_REAL_ATOMICS_FETCH_OPS
#undef DECLARE_ATOMICS
#undef DECLARE_REAL_ATOMICS

#endif // _chpl_atomics_h_

/*
 * Some misc notes:
 *
 *  - Our interface for the atomic_compare_exchange_* functions is slightly
 *  different than the C11 interface. The C11 interface is supposed to be
 *  atomic_compare_exchange_*(volatile A* obj, C* expected, C desired), but we
 *  do not pass expected in by reference. If the compare was unsuccessful
 *  expected is supposed to be set to value of obj, but since we don't pass it
 *  by reference, we can't make that change.
 */
