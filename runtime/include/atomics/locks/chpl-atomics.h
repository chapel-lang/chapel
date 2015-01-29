// chpl-atomics.h
//
// Since some of these non-intrinsic atomic routines are somewhat verbose, the GNU
// compiler issues a warning if inlining is requested and denied.
// If inlining is desired anyway, supply -DINLINE_LOCK_BASED_ATOMICS
//

#ifndef _chpl_atomics_h_
#define _chpl_atomics_h_

#include "chpltypes.h" // chpl_bool
#include "chpl-tasks.h" // chpl_sync_aux_t

#ifdef MAYBE_INLINE
 #error No fair.  I want to use this name.
#endif
#ifdef INLINE_LOCK_BASED_ATOMICS
 #define MAYBE_INLINE inline
#else
 #define MAYBE_INLINE
#endif

typedef struct atomic_int_least8_s {
  chpl_sync_aux_t sv;
  int_least8_t v;
} atomic_int_least8_t;
typedef struct atomic_int_least16_s {
  chpl_sync_aux_t sv;
  int_least16_t v;
} atomic_int_least16_t;
typedef struct atomic_int_least32_s {
  chpl_sync_aux_t sv;
  int_least32_t v;
} atomic_int_least32_t;
typedef struct atomic_int_least64_s {
  chpl_sync_aux_t sv;
  int_least64_t v;
} atomic_int_least64_t;
typedef struct atomic_uint_least8_s {
  chpl_sync_aux_t sv;
  uint_least8_t v;
} atomic_uint_least8_t;
typedef struct atomic_uint_least16_s {
  chpl_sync_aux_t sv;
  uint_least16_t v;
} atomic_uint_least16_t;
typedef struct atomic_uint_least32_s {
  chpl_sync_aux_t sv;
  uint_least32_t v;
} atomic_uint_least32_t;
typedef struct atomic_uint_least64_s {
  chpl_sync_aux_t sv;
  uint_least64_t v;
} atomic_uint_least64_t;
typedef struct atomic_uintptr_s {
  chpl_sync_aux_t sv;
  uintptr_t v;
} atomic_uintptr_t;

typedef struct atomic_flag_s {
  chpl_sync_aux_t sv;
  chpl_bool v;
} atomic_flag;

typedef struct atomic__real32_s {
  chpl_sync_aux_t sv;
  _real32 v;
} atomic__real32;

typedef struct atomic__real64_s {
  chpl_sync_aux_t sv;
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

static inline
void atomic_thread_fence(memory_order order)
{
  // No idea!
}
static inline
void atomic_signal_thread_fence(memory_order order)
{
  // No idea!
}

static MAYBE_INLINE chpl_bool
atomic_flag_test_and_set_explicit(atomic_flag *obj, memory_order order) {
  chpl_bool ret;
  chpl_sync_lock(&obj->sv);
  ret = obj->v;
  obj->v = true;
  chpl_sync_unlock(&obj->sv);
  return ret;
}
static inline chpl_bool atomic_flag_test_and_set(atomic_flag *obj) {
  return atomic_flag_test_and_set_explicit(obj, memory_order_seq_cst);
}
static MAYBE_INLINE void
atomic_flag_clear_explicit(atomic_flag *obj, memory_order order) {
  chpl_sync_lock(&obj->sv);
  obj->v = false;
  chpl_sync_unlock(&obj->sv);
}
static inline void atomic_flag_clear(atomic_flag *obj) {
  atomic_flag_clear_explicit(obj, memory_order_seq_cst);
}

#define DECLARE_ATOMICS_BASE(type, basetype) \
static inline chpl_bool atomic_is_lock_free_ ## type(atomic_ ## type * obj) { \
  return false; \
} \
static inline void atomic_init_ ## type(atomic_ ## type * obj, basetype value) { \
  obj->v = value; \
  chpl_sync_initAux(&obj->sv); \
} \
static inline void atomic_destroy_ ## type(atomic_ ## type * obj) { \
  chpl_sync_destroyAux(&obj->sv); \
} \
static MAYBE_INLINE void \
atomic_store_explicit_ ## type(atomic_ ## type * obj, basetype value, memory_order order) { \
  chpl_sync_lock(&obj->sv); \
  obj->v = value; \
  chpl_sync_unlock(&obj->sv); \
} \
static inline void atomic_store_ ## type(atomic_ ## type * obj, basetype value) { \
  atomic_store_explicit_ ## type(obj, value, memory_order_seq_cst); \
} \
static MAYBE_INLINE basetype \
atomic_load_explicit_ ## type(atomic_ ## type * obj, memory_order order) { \
  basetype ret; \
  chpl_sync_lock(&obj->sv); \
  ret = obj->v; \
  chpl_sync_unlock(&obj->sv); \
  return ret; \
} \
static inline basetype atomic_load_ ## type(atomic_ ## type * obj) { \
  return atomic_load_explicit_ ## type(obj, memory_order_seq_cst); \
} \
static MAYBE_INLINE basetype \
atomic_exchange_explicit_ ## type(atomic_ ## type * obj, basetype value, memory_order order) { \
  basetype ret; \
  chpl_sync_lock(&obj->sv); \
  ret = obj->v; \
  obj->v = value; \
  chpl_sync_unlock(&obj->sv); \
  return ret; \
} \
static inline basetype atomic_exchange_ ## type(atomic_ ## type * obj, basetype value) { \
  return atomic_exchange_explicit_ ## type(obj, value, memory_order_seq_cst); \
} \
static MAYBE_INLINE chpl_bool \
atomic_compare_exchange_strong_explicit_ ## type(atomic_ ## type * obj, basetype expected, basetype desired, memory_order order) { \
  basetype ret; \
  chpl_sync_lock(&obj->sv); \
  if( obj->v == expected ) { \
    obj->v = desired; \
    ret = true; \
  } else { \
    ret = false; \
  } \
  chpl_sync_unlock(&obj->sv); \
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
  chpl_sync_lock(&obj->sv); \
  ret = obj->v; \
  obj->v += operand; \
  chpl_sync_unlock(&obj->sv); \
  return ret; \
} \
static inline type atomic_fetch_add_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_add_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \
static MAYBE_INLINE type \
atomic_fetch_sub_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type ret; \
  chpl_sync_lock(&obj->sv); \
  ret = obj->v; \
  obj->v -= operand; \
  chpl_sync_unlock(&obj->sv); \
  return ret; \
} \
static inline type atomic_fetch_sub_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_sub_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \
static MAYBE_INLINE type \
atomic_fetch_or_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type ret; \
  chpl_sync_lock(&obj->sv); \
  ret = obj->v; \
  obj->v |= operand; \
  chpl_sync_unlock(&obj->sv); \
  return ret; \
} \
static inline type atomic_fetch_or_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_or_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \
static MAYBE_INLINE type \
atomic_fetch_and_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type ret; \
  chpl_sync_lock(&obj->sv); \
  ret = obj->v; \
  obj->v &= operand; \
  chpl_sync_unlock(&obj->sv); \
  return ret; \
} \
static inline type atomic_fetch_and_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_and_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \
static MAYBE_INLINE type \
atomic_fetch_xor_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type ret; \
  chpl_sync_lock(&obj->sv); \
  ret = obj->v; \
  obj->v ^= operand; \
  chpl_sync_unlock(&obj->sv); \
  return ret; \
} \
static inline type atomic_fetch_xor_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_xor_explicit_ ## type(obj, operand, memory_order_seq_cst); \
}

#define DECLARE_REAL_ATOMICS_FETCH_OPS(type) \
static MAYBE_INLINE type \
atomic_fetch_add_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type ret; \
  chpl_sync_lock(&obj->sv); \
  ret = obj->v; \
  obj->v += operand; \
  chpl_sync_unlock(&obj->sv); \
  return ret; \
} \
static inline type atomic_fetch_add_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_add_explicit_ ## type(obj, operand, memory_order_seq_cst); \
} \
static MAYBE_INLINE type \
atomic_fetch_sub_explicit_ ## type(atomic_ ## type * obj, type operand, memory_order order) { \
  type ret; \
  chpl_sync_lock(&obj->sv); \
  ret = obj->v; \
  obj->v -= operand; \
  chpl_sync_unlock(&obj->sv); \
  return ret; \
} \
static inline type atomic_fetch_sub_ ## type(atomic_ ## type * obj, type operand) { \
  return atomic_fetch_sub_explicit_ ## type(obj, operand, memory_order_seq_cst); \
}

DECLARE_ATOMICS_BASE(flag, chpl_bool);

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
DECLARE_ATOMICS(uintptr_t);

DECLARE_REAL_ATOMICS(_real32);
DECLARE_REAL_ATOMICS(_real64);

#undef DECLARE_ATOMICS_BASE
#undef DECLARE_ATOMICS_FETCH_OPS
#undef DECLARE_ATOMICS
#undef MAYBE_INLINE

#endif // _chpl_atomics_h_
