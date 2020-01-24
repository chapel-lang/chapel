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

#ifndef _chpl_atomics_h_
#define _chpl_atomics_h_

#include "chpltypes.h"
#include "chpl-comp-detect-macros.h"
#include <assert.h>

//
// SIZE_ALIGN_TYPE:  Declare a version of a type aligned to at least its size.
//
// All the compilers we support with the intrinsics version of atomics
// also support the gcc alignment attribute syntax used here.
//
// This is needed for 64-bit atomic types on 32-bit machines,
// to guarantee that the atomic objects will never straddle a
// cache line boundary, which has a severe performance penalty.
//
#define SIZE_ALIGN_TYPE(t) __attribute__ ((aligned (sizeof(t)))) t

typedef int_least8_t atomic_int_least8_t;
typedef int_least16_t atomic_int_least16_t;
typedef int_least32_t atomic_int_least32_t;
typedef SIZE_ALIGN_TYPE(int_least64_t) atomic_int_least64_t;
typedef uint_least8_t atomic_uint_least8_t;
typedef uint_least16_t atomic_uint_least16_t;
typedef uint_least32_t atomic_uint_least32_t;
typedef SIZE_ALIGN_TYPE(uint_least64_t) atomic_uint_least64_t;
typedef uintptr_t atomic_uintptr_t;
typedef chpl_bool atomic_bool;
typedef SIZE_ALIGN_TYPE(uint64_t) atomic__real64;
typedef uint32_t atomic__real32;

#undef SIZE_ALIGN_TYPE

typedef enum {
 memory_order_relaxed,
 memory_order_consume,
 memory_order_acquire,
 memory_order_release,
 memory_order_acq_rel,
 memory_order_seq_cst
} memory_order;

static inline memory_order _defaultOfMemoryOrder(void) {
  return memory_order_seq_cst;
}

// __sync_synchronize/__sync_bool_compare_and_swap are missing for cce < 8.4
// and __sync_bool_compare_and_swap is broken in newer versions. Use
// __builtin_ia32_mfence and __sync_val_compare_and_swap instead.
#if RT_COMP_CC == RT_COMP_CRAY
  #include <intrinsics.h>
  #define full_memory_barrier __builtin_ia32_mfence
  
  # define my__sync_bool_compare_and_swap(obj, expected, desired) \
  (__sync_val_compare_and_swap(obj, expected, desired) == expected) 
#else
  #define full_memory_barrier __sync_synchronize
 
  # define my__sync_bool_compare_and_swap(obj, expected, desired) \
  __sync_bool_compare_and_swap(obj, expected, desired) 
#endif


static inline void chpl_atomic_thread_fence(memory_order order)
{
  full_memory_barrier();
}
static inline void chpl_atomic_signal_fence(memory_order order)
{
  full_memory_barrier();
}


///////////////////////////////////////////////////////////////////////////////
////                      START OF INTEGER ATOMICS BASE                   ////
//////////////////////////////////////////////////////////////////////////////

// The obvious implementation of atomic loads and stores is insufficient.
// They must be implemented with atomic primitives to ensure consistent
// visibility of the results.
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
  basetype ret = *obj; \
  basetype old_val; \
  do { \
    old_val = ret; \
    ret = __sync_val_compare_and_swap(obj, old_val, value); \
  } while (ret != old_val); \
} \
static inline void atomic_store_ ## type(atomic_ ## type * obj, basetype value) { \
  atomic_store_explicit_ ## type(obj, value, memory_order_seq_cst); \
} \
static inline basetype atomic_load_explicit_ ## type(atomic_ ## type * obj, memory_order order) { \
  return __sync_val_compare_and_swap(obj, (basetype)0, (basetype)0); \
} \
static inline basetype atomic_load_ ## type(atomic_ ## type * obj) { \
  return atomic_load_explicit_ ## type(obj, memory_order_seq_cst); \
}


///////////////////////////////////////////////////////////////////////////////
////                 START OF INTEGER ATOMIC EXCHANGE OPS                 ////
//////////////////////////////////////////////////////////////////////////////
#define DECLARE_ATOMICS_EXCHANGE_OPS(type, basetype) \
static inline basetype atomic_exchange_explicit_ ## type(atomic_ ## type * obj, basetype value, memory_order order) { \
  basetype ret = *obj; \
  basetype old_val; \
  do { \
    old_val = ret; \
    ret = __sync_val_compare_and_swap(obj, old_val, value); \
  } while(ret != old_val); \
  return ret; \
} \
static inline basetype atomic_exchange_ ## type(atomic_ ## type * obj, basetype value) { \
  return atomic_exchange_explicit_ ## type(obj, value, memory_order_seq_cst); \
} \
static inline chpl_bool atomic_compare_exchange_strong_explicit_ ## type(atomic_ ## type * obj, basetype expected, basetype desired, memory_order order) { \
  return my__sync_bool_compare_and_swap(obj, expected, desired); \
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


///////////////////////////////////////////////////////////////////////////////
////              START OF INTEGER ATOMIC FETCH OPERATIONS                ////
//////////////////////////////////////////////////////////////////////////////
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
} \
static inline type atomic_fetch_xor_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  return __sync_fetch_and_xor(obj, operand); \
} \
static inline type atomic_fetch_xor_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_xor_explicit_ ## type(obj, operand, memory_order_seq_cst); \
}


///////////////////////////////////////////////////////////////////////////////
////                       START OF REAL ATOMICS BASE                     ////
//////////////////////////////////////////////////////////////////////////////

// The obvious implementation of atomic loads and stores is insufficient.
// They must be implemented with atomic primitives to ensure consistent
// visibility of the results.
#define DECLARE_REAL_ATOMICS_BASE(type, uinttype) \
static inline chpl_bool atomic_is_lock_free_ ## type(atomic_ ## type * obj) { \
  return true; \
} \
static inline void atomic_init_ ## type(atomic_ ## type * obj, type value) { \
  assert(sizeof(type) == sizeof(uinttype)); \
  memcpy(obj, &value, sizeof(*obj)); \
} \
static inline void atomic_destroy_ ## type(atomic_ ## type * obj) { \
} \
static inline void atomic_store_explicit_ ## type(atomic_ ## type * obj, type value, memory_order order) { \
  uinttype ret_uint = *obj; \
  uinttype val_uint; \
  uinttype old_uint; \
  memcpy(&val_uint, &value, sizeof(val_uint)); \
  do { \
    old_uint = ret_uint; \
    ret_uint = __sync_val_compare_and_swap(obj, old_uint, val_uint); \
  } while (ret_uint != old_uint); \
} \
static inline void atomic_store_ ## type(atomic_ ## type * obj, type value) { \
  atomic_store_explicit_ ## type(obj, value, memory_order_seq_cst); \
} \
static inline type atomic_load_explicit_ ## type(atomic_ ## type * obj, memory_order order) { \
  type ret; \
  uinttype ret_uint = __sync_val_compare_and_swap(obj, (uinttype)0, (uinttype)0); \
  memcpy(&ret, &ret_uint, sizeof(ret)); \
  return ret; \
} \
static inline type atomic_load_ ## type(atomic_ ## type * obj) { \
  return atomic_load_explicit_ ## type(obj, memory_order_seq_cst); \
} 


///////////////////////////////////////////////////////////////////////////////
////                START OF REAL ATOMICS EXCHANGE OPS                    ////
//////////////////////////////////////////////////////////////////////////////
#define DECLARE_REAL_ATOMICS_EXCHANGE_OPS(type, uinttype) \
static inline type atomic_exchange_explicit_ ## type(atomic_ ## type * obj, type value, memory_order order) { \
  type ret; \
  uinttype value_as_uint; \
  uinttype ret_as_uint = *obj; \
  uinttype old_val_as_uint; \
  memcpy(&value_as_uint, &value, sizeof(value_as_uint)); \
  do { \
    old_val_as_uint = ret_as_uint; \
    ret_as_uint = __sync_val_compare_and_swap(obj, old_val_as_uint, value_as_uint); \
  } while(ret_as_uint != old_val_as_uint); \
  memcpy(&ret, &ret_as_uint, sizeof(ret)); \
  return ret; \
} \
static inline type atomic_exchange_ ## type(atomic_ ## type * obj, type value) { \
  return atomic_exchange_explicit_ ## type(obj, value, memory_order_seq_cst); \
} \
static inline chpl_bool atomic_compare_exchange_strong_explicit_ ## type(atomic_ ## type * obj, type expected, type desired, memory_order order) { \
  uinttype expected_as_uint; \
  uinttype desired_as_uint; \
  memcpy(&expected_as_uint, &expected, sizeof(expected_as_uint)); \
  memcpy(&desired_as_uint, &desired, sizeof(desired_as_uint)); \
  return my__sync_bool_compare_and_swap(obj, expected_as_uint, desired_as_uint); \
} \
static inline chpl_bool atomic_compare_exchange_strong_ ## type(atomic_ ## type * obj, type expected, type desired) { \
  return atomic_compare_exchange_strong_explicit_ ## type(obj, expected, desired, memory_order_seq_cst); \
} \
static inline chpl_bool atomic_compare_exchange_weak_explicit_ ## type(atomic_ ## type * obj, type expected, type desired, memory_order order) { \
  return atomic_compare_exchange_strong_explicit_ ## type(obj, expected, desired, order); \
} \
static inline chpl_bool atomic_compare_exchange_weak_ ## type(atomic_ ## type * obj, type expected, type desired) { \
  return atomic_compare_exchange_weak_explicit_ ## type(obj, expected, desired, memory_order_seq_cst); \
} 


///////////////////////////////////////////////////////////////////////////////
////                   START OF REAL ATOMICS FETCH OPS                    ////
//////////////////////////////////////////////////////////////////////////////
#define DECLARE_REAL_ATOMICS_FETCH_OPS(type, uinttype) \
static inline type atomic_fetch_add_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type ret; \
  type old; \
  type desired; \
  uinttype ret_as_uint = *obj; \
  uinttype old_as_uint; \
  uinttype desired_as_uint; \
  do { \
    old_as_uint = ret_as_uint; \
    memcpy(&old, &old_as_uint, sizeof(old)); \
    desired = old + operand; \
    memcpy(&desired_as_uint, &desired, sizeof(desired_as_uint)); \
    ret_as_uint = __sync_val_compare_and_swap(obj, old_as_uint, desired_as_uint); \
  } while (ret_as_uint != old_as_uint); \
  memcpy(&ret, &ret_as_uint, sizeof(ret)); \
  return ret; \
} \
static inline type atomic_fetch_add_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_add_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \
static inline type atomic_fetch_sub_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type ret; \
  type old; \
  type desired; \
  uinttype ret_as_uint = *obj; \
  uinttype old_as_uint; \
  uinttype desired_as_uint; \
  do { \
    old_as_uint = ret_as_uint; \
    memcpy(&old, &old_as_uint, sizeof(old)); \
    desired = old - operand; \
    memcpy(&desired_as_uint, &desired, sizeof(desired_as_uint)); \
    ret_as_uint = __sync_val_compare_and_swap(obj, old_as_uint, desired_as_uint); \
  } while (ret_as_uint != old_as_uint); \
  memcpy(&ret, &ret_as_uint, sizeof(ret)); \
  return ret; \
} \
static inline type atomic_fetch_sub_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_sub_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \


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


#define DECLARE_REAL_ATOMICS(type, uinttype) \
  DECLARE_REAL_ATOMICS_BASE(type, uinttype) \
  DECLARE_REAL_ATOMICS_EXCHANGE_OPS(type, uinttype) \
  DECLARE_REAL_ATOMICS_FETCH_OPS(type, uinttype)

DECLARE_REAL_ATOMICS(_real32, uint32_t);
DECLARE_REAL_ATOMICS(_real64, uint64_t);


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
 *  - According to the spec, the atomic intrinsics are technically only
 *  supported for ints, longs, long longs, and their unsigned counterparts but
 *  all the current compilers that we support have intrinsics for  1, 2, 4, and
 *  8 byte variables. 
 * 
 *  - Our interface for the atomic_compare_exchange_* functions is slightly
 *  different than the C11 interface. The C11 interface is supposed to be
 *  atomic_compare_exchange_*(volatile A* obj, C* expected, C desired), but we
 *  do not pass expected in by reference. If the compare was unsuccessful
 *  expected is supposed to be set to value of obj, but since we don't pass it
 *  by reference, we can't make that change. 
 */
