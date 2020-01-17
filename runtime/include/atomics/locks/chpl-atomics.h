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
#include <pthread.h>

// Locks based atomic implementation. Note that we use pthread mutexes instead
// of chpl sync variables because we need to use atomics during runtime init
// and teardown (when the tasking layer is not active.) It's safe to use
// pthread mutexes because the critical section of these routines never yield.
// Since there are no yields, there's no possibility of recursive locking on
// the same thread from different tasks or unlocking from a different thread,
// both of which violate pthread semantics.


// Since some of these non-intrinsic atomic routines are somewhat verbose, the GNU
// compiler issues a warning if inlining is requested and denied.
// If inlining is desired anyway, supply -DINLINE_LOCK_BASED_ATOMICS
#ifdef MAYBE_INLINE
 #error No fair.  I want to use this name.
#endif
#ifdef INLINE_LOCK_BASED_ATOMICS
 #define MAYBE_INLINE inline
#else
 #define MAYBE_INLINE
#endif

typedef struct atomic_int_least8_s {
  pthread_mutex_t lock;
  int_least8_t v;
} atomic_int_least8_t;
typedef struct atomic_int_least16_s {
  pthread_mutex_t lock;
  int_least16_t v;
} atomic_int_least16_t;
typedef struct atomic_int_least32_s {
  pthread_mutex_t lock;
  int_least32_t v;
} atomic_int_least32_t;
typedef struct atomic_int_least64_s {
  pthread_mutex_t lock;
  int_least64_t v;
} atomic_int_least64_t;
typedef struct atomic_uint_least8_s {
  pthread_mutex_t lock;
  uint_least8_t v;
} atomic_uint_least8_t;
typedef struct atomic_uint_least16_s {
  pthread_mutex_t lock;
  uint_least16_t v;
} atomic_uint_least16_t;
typedef struct atomic_uint_least32_s {
  pthread_mutex_t lock;
  uint_least32_t v;
} atomic_uint_least32_t;
typedef struct atomic_uint_least64_s {
  pthread_mutex_t lock;
  uint_least64_t v;
} atomic_uint_least64_t;
typedef struct atomic_uintptr_s {
  pthread_mutex_t lock;
  uintptr_t v;
} atomic_uintptr_t;

typedef struct atomic_bool_s {
  pthread_mutex_t lock;
  chpl_bool v;
} atomic_bool;

typedef struct atomic__real32_s {
  pthread_mutex_t lock;
  _real32 v;
} atomic__real32;

typedef struct atomic__real64_s {
  pthread_mutex_t lock;
  _real64 v;
} atomic__real64;

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

static inline
void chpl_atomic_thread_fence(memory_order order)
{
  // No idea!
}
static inline
void chpl_atomic_signal_fence(memory_order order)
{
  // No idea!
}

#define DECLARE_ATOMICS_BASE(type, basetype) \
static inline chpl_bool atomic_is_lock_free_ ## type(atomic_ ## type * obj) { \
  return false; \
} \
static inline void atomic_init_ ## type(atomic_ ## type * obj, basetype value) { \
  obj->v = value; \
  (void) pthread_mutex_init(&obj->lock, NULL); \
} \
static inline void atomic_destroy_ ## type(atomic_ ## type * obj) { \
  (void) pthread_mutex_destroy(&obj->lock); \
} \
static MAYBE_INLINE void \
atomic_store_explicit_ ## type(atomic_ ## type * obj, basetype value, memory_order order) { \
  (void) pthread_mutex_lock(&obj->lock); \
  obj->v = value; \
  (void) pthread_mutex_unlock(&obj->lock); \
} \
static inline void atomic_store_ ## type(atomic_ ## type * obj, basetype value) { \
  atomic_store_explicit_ ## type(obj, value, memory_order_seq_cst); \
} \
static MAYBE_INLINE basetype \
atomic_load_explicit_ ## type(atomic_ ## type * obj, memory_order order) { \
  basetype ret; \
  (void) pthread_mutex_lock(&obj->lock); \
  ret = obj->v; \
  (void) pthread_mutex_unlock(&obj->lock); \
  return ret; \
} \
static inline basetype atomic_load_ ## type(atomic_ ## type * obj) { \
  return atomic_load_explicit_ ## type(obj, memory_order_seq_cst); \
} \
static MAYBE_INLINE basetype \
atomic_exchange_explicit_ ## type(atomic_ ## type * obj, basetype value, memory_order order) { \
  basetype ret; \
  (void) pthread_mutex_lock(&obj->lock); \
  ret = obj->v; \
  obj->v = value; \
  (void) pthread_mutex_unlock(&obj->lock); \
  return ret; \
} \
static inline basetype atomic_exchange_ ## type(atomic_ ## type * obj, basetype value) { \
  return atomic_exchange_explicit_ ## type(obj, value, memory_order_seq_cst); \
} \
static MAYBE_INLINE chpl_bool \
atomic_compare_exchange_strong_explicit_ ## type(atomic_ ## type * obj, basetype expected, basetype desired, memory_order order) { \
  basetype ret; \
  (void) pthread_mutex_lock(&obj->lock); \
  if( obj->v == expected ) { \
    obj->v = desired; \
    ret = true; \
  } else { \
    ret = false; \
  } \
  (void) pthread_mutex_unlock(&obj->lock); \
  return ret; \
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
static MAYBE_INLINE type \
atomic_fetch_add_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type ret; \
  (void) pthread_mutex_lock(&obj->lock); \
  ret = obj->v; \
  obj->v += operand; \
  (void) pthread_mutex_unlock(&obj->lock); \
  return ret; \
} \
static inline type atomic_fetch_add_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_add_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \
static MAYBE_INLINE type \
atomic_fetch_sub_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type ret; \
  (void) pthread_mutex_lock(&obj->lock); \
  ret = obj->v; \
  obj->v -= operand; \
  (void) pthread_mutex_unlock(&obj->lock); \
  return ret; \
} \
static inline type atomic_fetch_sub_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_sub_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \
static MAYBE_INLINE type \
atomic_fetch_or_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type ret; \
  (void) pthread_mutex_lock(&obj->lock); \
  ret = obj->v; \
  obj->v |= operand; \
  (void) pthread_mutex_unlock(&obj->lock); \
  return ret; \
} \
static inline type atomic_fetch_or_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_or_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \
static MAYBE_INLINE type \
atomic_fetch_and_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type ret; \
  (void) pthread_mutex_lock(&obj->lock); \
  ret = obj->v; \
  obj->v &= operand; \
  (void) pthread_mutex_unlock(&obj->lock); \
  return ret; \
} \
static inline type atomic_fetch_and_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_and_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \
static MAYBE_INLINE type \
atomic_fetch_xor_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type ret; \
  (void) pthread_mutex_lock(&obj->lock); \
  ret = obj->v; \
  obj->v ^= operand; \
  (void) pthread_mutex_unlock(&obj->lock); \
  return ret; \
} \
static inline type atomic_fetch_xor_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_xor_explicit_ ## type(obj, operand, memory_order_seq_cst); \
}

#define DECLARE_REAL_ATOMICS_FETCH_OPS(type) \
static MAYBE_INLINE type \
atomic_fetch_add_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type ret; \
  (void) pthread_mutex_lock(&obj->lock); \
  ret = obj->v; \
  obj->v += operand; \
  (void) pthread_mutex_unlock(&obj->lock); \
  return ret; \
} \
static inline type atomic_fetch_add_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_add_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \
static MAYBE_INLINE type \
atomic_fetch_sub_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type ret; \
  (void) pthread_mutex_lock(&obj->lock); \
  ret = obj->v; \
  obj->v -= operand; \
  (void) pthread_mutex_unlock(&obj->lock); \
  return ret; \
} \
static inline type atomic_fetch_sub_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_sub_explicit_ ## type(obj, operand, memory_order_seq_cst); \
}

DECLARE_ATOMICS_BASE(bool, chpl_bool);

#define DECLARE_ATOMICS(type) \
  DECLARE_ATOMICS_BASE(type, type) \
  DECLARE_ATOMICS_FETCH_OPS(type)

#define DECLARE_REAL_ATOMICS(type) \
  DECLARE_ATOMICS_BASE(type, type) \
  DECLARE_REAL_ATOMICS_FETCH_OPS(type)

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
DECLARE_ATOMICS_FETCH_OPS(uintptr_t);

DECLARE_REAL_ATOMICS(_real32);
DECLARE_REAL_ATOMICS(_real64);

#undef DECLARE_ATOMICS_BASE
#undef DECLARE_ATOMICS_FETCH_OPS
#undef DECLARE_ATOMICS
#undef MAYBE_INLINE

#endif // _chpl_atomics_h_
