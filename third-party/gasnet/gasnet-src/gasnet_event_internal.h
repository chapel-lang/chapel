/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_event_internal.h $
 * Description: GASNet header for internal definitions for event management
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_EVENT_INTERNAL_H
#define _GASNET_EVENT_INTERNAL_H

#include <gasnet_core_internal.h>

/* ------------------------------------------------------------------------------------ */

#ifdef GASNETE_HAVE_LC
#  undef GASNETE_HAVE_LC
#  define GASNETE_HAVE_LC 1
#else // Default is NO local completion
#  define GASNETE_IOP_LC_DONE(op) 1
#  define GASNETE_IOP_LC_CNTDONE(op) 1
#  define GASNETE_EOP_LC_DONE(op) 1
#  define GASNETE_IOP_LC_START(op)  ERROR
#  define GASNETE_IOP_LC_FINISH(op) ERROR
#  define GASNETE_EOP_LC_START(op)  ERROR
#  define GASNETE_EOP_LC_FINISH(op) ERROR
#  define GASNETE_HAVE_LC 0
#endif

/* ------------------------------------------------------------------------------------ */

/* Conduit may optionally choose the atomic type for counters */
#ifndef gasnete_op_atomic_
  #define gasnete_op_atomic_(_id)       gasneti_weakatomic_##_id
#endif
#define gasnete_op_atomic_t             gasnete_op_atomic_(t)
#define gasnete_op_atomic_val_t         gasnete_op_atomic_(val_t)
#define gasnete_op_atomic_read          gasnete_op_atomic_(read)
#define gasnete_op_atomic_set           gasnete_op_atomic_(set)
#define gasnete_op_atomic_increment     gasnete_op_atomic_(increment)
#define gasnete_op_atomic_add           gasnete_op_atomic_(add)

/* ------------------------------------------------------------------------------------ */

/* gex_Event_t is a void* pointer to a gasnete_op_t, 
   which is either a gasnete_eop_t or an gasnete_iop_t
 */
#define GASNETE_OP_EVENTS 6
typedef struct _gasnete_op_t {
  uint8_t event[GASNETE_OP_EVENTS];
  gasnete_threadidx_t threadidx;  /*  thread that owns me (16-bit by default) */
} gasnete_op_t;

struct _gasnete_eop_t {
  uint8_t event[GASNETE_OP_EVENTS];
  gasnete_threadidx_t threadidx;  /*  thread that owns me */
  //----------------------------------
  struct _gasnete_eop_t *next;
  #ifdef GASNETE_CONDUIT_EOP_FIELDS
  GASNETE_CONDUIT_EOP_FIELDS
  #endif
};

struct _gasnete_iop_t {
  uint8_t event[GASNETE_OP_EVENTS];
  gasnete_threadidx_t threadidx;  /*  thread that owns me */
  //----------------------------------
#if GASNETE_HAVE_LC
  gasnete_op_atomic_val_t initiated_alc_cnt;     /*  count of ops initiated with async local completion */
#endif
  gasnete_op_atomic_val_t initiated_rmw_cnt;     /*  count of ratomic rmw ops initiated */
  gasnete_op_atomic_val_t initiated_get_cnt;     /*  count of get ops initiated */
  gasnete_op_atomic_val_t initiated_put_cnt;     /*  count of put ops initiated */

  struct _gasnete_iop_t *next;    /*  next cell while in free list, deferred iop while being filled */

  /*  make sure the corresponding initiated/completed counters live on different cache lines for SMP's */
#if GASNETE_HAVE_LC
  uint8_t pad[GASNETI_CACHE_PAD(sizeof(void*) + 4*sizeof(gasnete_op_atomic_val_t))];
#else
  uint8_t pad[GASNETI_CACHE_PAD(sizeof(void*) + 3*sizeof(gasnete_op_atomic_val_t))];
#endif

#if GASNETE_HAVE_LC
  gasnete_op_atomic_t completed_alc_cnt;     /*  count of async-lc ops completed */
#endif
  gasnete_op_atomic_t completed_rmw_cnt;     /*  count of ratomic rmw ops completed */
  gasnete_op_atomic_t completed_get_cnt;     /*  count of get ops completed */
  gasnete_op_atomic_t completed_put_cnt;     /*  count of put ops completed */

  #ifdef GASNETE_CONDUIT_IOP_FIELDS
  GASNETE_CONDUIT_IOP_FIELDS
  #endif
};

/* ------------------------------------------------------------------------------------ */

// Each 1-byte event field in an OP has 4 bits for "type"
// The most-significant bit is used to indicate liveness
#define EVENT_TYPE_MASK    0x0F
#define EVENT_LIVE_MASK    0x80

#if PLATFORM_ARCH_BIG_ENDIAN
  #define EVENT_ANY_LIVE_MASK (((uint64_t)0x8080808080808080ULL) << (8*(8-GASNETE_OP_EVENTS)))
#else
  #define EVENT_ANY_LIVE_MASK (((uint64_t)0x8080808080808080ULL) >> (8*(8-GASNETE_OP_EVENTS)))
#endif

// gasnete_event_type_* values
enum {
#if GASNET_DEBUG // used only for correctness checking of recycle paths
  gasnete_event_type_free_eop = 0,
  gasnete_event_type_free_iop,
  gasnete_event_type_pendingfree_eop,
  gasnete_event_type_pendingfree_iop,
#endif
  gasnete_event_type_eop,    // root event only (event[0])
  gasnete_event_type_iop,    // root event only (event[0])
  gasnete_event_type_lc,     // local completion
  gasnete_event_type_lc_now, // local completion w/ EVENT_NOW
#ifdef GASNETE_CONDUIT_EVENT_TYPES
  GASNETE_CONDUIT_EVENT_TYPES // A conduit may add additional types as needed to distinguish its unique cases
#endif
};

/* ------------------------------------------------------------------------------------ */

// Legacy:
#define OPTYPE_EXPLICIT               gasnete_event_type_eop
#define OPTYPE_IMPLICIT               gasnete_event_type_iop
#define OPTYPE(op)                    EVENT_TYPE(op,0)

// SET_EVENT_TYPE: given a gasnete_op_t and event idx, return the gasneti_event_type_*
#define EVENT_TYPE(op,idx) ((op)->event[idx] & EVENT_TYPE_MASK)

// SET_EVENT_TYPE: given a gasnete_op_t, event idx and gasneti_event_type_*, 
// set that event to the provided type and mark it live
#define SET_EVENT_TYPE(op,idx,type) _SET_EVENT_TYPE((gasnete_op_t*)(op),idx,type)
GASNETI_INLINE(_SET_EVENT_TYPE)
void _SET_EVENT_TYPE(gasnete_op_t *op, unsigned int idx, uint8_t type) {
  gasneti_assert_uint(idx ,<, GASNETE_OP_EVENTS);
  gasneti_assert((EVENT_TYPE(op, idx) == 0) || (EVENT_TYPE(op, idx) == type));
  gasneti_assert_uint(type ,==, (type & EVENT_TYPE_MASK));
  op->event[idx] = EVENT_LIVE_MASK | type;
}

// EVENT_DONE: given a gasnete_op_t and child idx, return whether that event is live
#define EVENT_DONE(op,idx) (!((op)->event[idx] & EVENT_LIVE_MASK))

// SET_EVENT_DONE: given a gasnete_op_t and live child idx, mark that event done
// note the event type is not cleared
#define SET_EVENT_DONE(op,idx) _SET_EVENT_DONE((gasnete_op_t*)(op),idx)
GASNETI_INLINE(_SET_EVENT_DONE)
void _SET_EVENT_DONE(gasnete_op_t *op, unsigned int idx) {
  gasneti_assert_uint(idx ,<, GASNETE_OP_EVENTS);
  gasneti_assert(! EVENT_DONE(op, idx));
  // TODO-EX: OPT build could potentially replace "&= ..." with just "= 0".
  // This would be only for idx!=0, and may not work if there is a need
  // to preserve any other bits in the event bytes (still TBD).
  op->event[idx] &= ~EVENT_LIVE_MASK;
}

// Test all event bits in an eop or iop
// EVENT_ANY_LIVE: true iff ANY event in the gasnete_op_t is live
// EVENT_ALL_DONE: true iff NO  event in the gasnete_op_t is live
#define EVENT_ANY_LIVE(op) \
  (gasneti_assert(!gasneti_event_idx(op)),\
   (*(volatile uint64_t *)(op) & EVENT_ANY_LIVE_MASK))
#define EVENT_ALL_DONE(op) (!EVENT_ANY_LIVE(op))


#if GASNET_DEBUG
  /* check an in-flight/complete eop */
  #define gasnete_eop_check(eop) do {                                \
    gasnete_eop_t *_eop = (eop);                                     \
    gasneti_assert_uint(OPTYPE(_eop) ,==, OPTYPE_EXPLICIT);          \
    gasnete_assert_valid_threadid(_eop->threadidx);                  \
  } while (0)
  #define gasnete_iop_check_cnt_(iop,counter) do { \
    gasnete_op_atomic_val_t _temp = gasnete_op_atomic_read(&((iop)->completed_##counter##_cnt), GASNETI_ATOMIC_RMB_POST); \
    gasneti_assert_uint((((iop)->initiated_##counter##_cnt - _temp) & GASNETI_ATOMIC_MAX) ,<, (GASNETI_ATOMIC_MAX/2)); \
  } while (0)
#if GASNETE_HAVE_LC
  #define gasnete_iop_check_cnt_alc_(iop) gasnete_iop_check_cnt_(iop,alc)
#else
  #define gasnete_iop_check_cnt_alc_(iop) ((void)0)
#endif
  // TODO-EX: type_free_iop occurs only when called via gasnete_free_threaddata()
  #define gasnete_iop_check(iop) do {                         \
    gasnete_iop_t *_iop = (iop);                              \
    gasnete_iop_t *_tmp_next;                                 \
    gasneti_memcheck(_iop);                                   \
    _tmp_next = (_iop)->next;                                 \
    if (_tmp_next != NULL && _tmp_next != _iop)               \
       _gasnete_iop_check(_tmp_next);                         \
    gasneti_assert(OPTYPE(_iop) == gasnete_event_type_iop ||  \
                   OPTYPE(_iop) == gasnete_event_type_free_iop);\
    gasnete_assert_valid_threadid(_iop->threadidx);           \
    gasnete_iop_check_cnt_(_iop,put);                         \
    gasnete_iop_check_cnt_(_iop,get);                         \
    gasnete_iop_check_cnt_(_iop,rmw);                         \
    gasnete_iop_check_cnt_alc_(_iop);                         \
  } while (0)
  extern void _gasnete_iop_check(gasnete_iop_t *iop);
  #define gasnete_event_check(_h) do { \
    gasneti_assert(_h != GEX_EVENT_INVALID);              \
    gasneti_assert(_h != GEX_EVENT_NO_OP);                \
    gasneti_assert_uint(gasneti_event_idx(_h) ,<, GASNETE_OP_EVENTS); \
    gasnete_op_t *_op = gasneti_event_op(_h);                  \
    switch (EVENT_TYPE(_op,0)) {                               \
      case gasnete_event_type_eop:                             \
        gasnete_eop_check((gasnete_eop_t*)_op);                \
        break;                                                 \
      case gasnete_event_type_iop:                             \
        gasnete_iop_check((gasnete_iop_t*)_op);                \
        break;                                                 \
      case gasnete_event_type_free_eop:                        \
      case gasnete_event_type_pendingfree_eop:                 \
        gasneti_fatalerror("Invalid use of free eop");         \
        break;                                                 \
      case gasnete_event_type_free_iop:                        \
      case gasnete_event_type_pendingfree_iop:                 \
        gasneti_fatalerror("Invalid use of free iop");         \
        break;                                                 \
      default:                                                 \
        gasneti_fatalerror("Event has invalid type %d",        \
                           EVENT_TYPE(_op,0));                 \
    }                                                          \
  } while (0)
#else
  #define gasnete_eop_check(eop)   ((void)0)
  #define gasnete_iop_check(iop)   ((void)0)
  #define gasnete_event_check(h)  ((void)0)
#endif

// TODO-EX: EOP_INTERFACE
//    To efficiently generalize beyond put/get, this enum will
//    probably need to reflect the order of the counters in the iop
//    (to compute addresses instead of dispatch via 'if' or 'switch').
#if 1 // TODO-EX: do we want/need a mechanism for overriding these assignments?
  // gasnete_iop_event_* the event indexes for the named event in an iop
  enum {
    gasnete_iop_event_put = 0,
    gasnete_iop_event_alc = 1,
    gasnete_iop_event_get = 2,
    gasnete_iop_event_rmw = 3,
  };
#endif
// gasnete_eop_event_* the child event indexes for the named event in an eop
#define gasnete_eop_event_alc gasnete_iop_event_alc

// Start == advance an initiated counter for named event in ANY iop by nop
// note this relies upon gasnete_begin_nbi_accessregion setting live bits for aop
#ifndef GASNETE_IOP_CNT_START
  #define GASNETE_IOP_CNT_START(_iop, _name, _nop) do {  \
    (_iop)->initiated_##_name##_cnt += (_nop);           \
  } while (0)
#endif

//
// "Finish" an iop in an nbi access region
// NOTE: caller must pass REL flag when required for Gets
//
// TODO-EX: this or the callers will probably need to call a
// non-inline function in case of dependent ops.
//
#ifndef GASNETE_IOP_CNT_FINISH_REG
  #define GASNETE_IOP_CNT_FINISH_REG(_iop, _name, _nop, _flags) do {      \
    gasneti_assert((_iop)->next);                                         \
    gasnete_op_atomic_val_t _completed =                                  \
            gasnete_op_atomic_add(&(_iop)->completed_##_name##_cnt,       \
                                  (_nop), (_flags)|GASNETI_ATOMIC_ACQ);   \
    gasnete_op_atomic_val_t _initiated = (_iop)->initiated_##_name##_cnt; \
    if (_completed == (_initiated & GASNETI_ATOMIC_MAX)) {                \
      SET_EVENT_DONE((_iop),gasnete_iop_event_##_name);                   \
    }                                                                     \
  } while (0)
#endif

//
// "Finish" the internal iop, NOT in an nbi access region
// NOTE: caller must pass REL flag when required for Gets
//
#ifndef GASNETE_IOP_CNT_FINISH_INT
  #define GASNETE_IOP_CNT_FINISH_INT(_iop, _name, _nop, _flags) do {         \
    gasneti_assert(! (_iop)->next);                                          \
    if (gasneti_constant_p(_nop) && ((_nop) == 1))                           \
      gasnete_op_atomic_increment(&(_iop)->completed_##_name##_cnt, _flags); \
    else                                                                     \
      gasnete_op_atomic_add(&(_iop)->completed_##_name##_cnt, _nop, _flags); \
  } while (0)
#endif

// Finish == advance a completed counter for named event in ANY iop by nop, with flags fencing
#ifndef GASNETE_IOP_CNT_FINISH
  #define GASNETE_IOP_CNT_FINISH(_iop, _name, _nop, _flags) do {    \
    if ((_iop)->next) /* access region iop */                       \
      GASNETE_IOP_CNT_FINISH_REG((_iop), _name, (_nop), (_flags));  \
    else              /* implicit iop */                            \
      GASNETE_IOP_CNT_FINISH_INT((_iop), _name, (_nop), (_flags));  \
  } while (0)
#endif

#ifndef GASNETE_IOP_CNTDONE // Test iop for counter balance of named event
  #define GASNETE_IOP_CNTDONE(_iop, _name) \
    (gasnete_op_atomic_read(&(_iop)->completed_##_name##_cnt, 0) \
          == ((_iop)->initiated_##_name##_cnt & GASNETI_ATOMIC_MAX))
#endif

#ifndef GASNETE_IOP_DONE // Query named event bit from accessregion iop for liveness
  #define GASNETE_IOP_DONE(_iop, _name) \
    (gasneti_assert((_iop)->next), EVENT_DONE((_iop),gasnete_iop_event_##_name))
#endif

#ifndef GASNETE_EOP_DONE // Query Root event of eop for completion
  #define GASNETE_EOP_DONE(_eop) EVENT_DONE(_eop,0)
#endif

#ifndef GASNETE_EOP_MARKDONE // Set Root event of eop to done
  #define GASNETE_EOP_MARKDONE(_eop) SET_EVENT_DONE(_eop,0)
#endif

// local completion management for aop/iop
#ifndef GASNETE_IOP_LC_START
  #define GASNETE_IOP_LC_START(iop)  GASNETE_IOP_CNT_START(iop, alc, 1)
#endif
#ifndef GASNETE_IOP_LC_FINISH
  #define GASNETE_IOP_LC_FINISH(iop) GASNETE_IOP_CNT_FINISH(iop, alc, 1, 0)
#endif

// event:lc - for local-completion of Put/Med/Long with lc_opt = pointer
#ifndef GASNETE_EOP_LC_START
  #define GASNETE_EOP_LC_START(op)   SET_EVENT_TYPE(op, gasnete_eop_event_alc, gasnete_event_type_lc)
#endif
#ifndef GASNETE_EOP_LC_FINISH
  #define GASNETE_EOP_LC_FINISH(_eop) SET_EVENT_DONE((_eop),gasnete_eop_event_alc)
#endif

// event:lc - DONE queries on IOP and EOP
#ifndef GASNETE_IOP_LC_DONE // access region iops only
  #define GASNETE_IOP_LC_DONE(_iop) GASNETE_IOP_DONE((_iop),alc)
#endif
#ifndef GASNETE_IOP_LC_CNTDONE
  #define GASNETE_IOP_LC_CNTDONE(_iop) GASNETE_IOP_CNTDONE((_iop),alc)
#endif
#ifndef GASNETE_EOP_LC_DONE
  #define GASNETE_EOP_LC_DONE(_eop) EVENT_DONE((_eop),gasnete_eop_event_alc)
#endif


// event:lc_now - for local-completion of Put/Med/Long with lc_opt = EVENT_NOW.
// This can be applied to *either* EOP or IOP, since the call with EVENT_NOW
// must block until the event has been signaled.  Additionally, this can co-exist
// with event:lc (both using same event[]) since they are per-operation mutually
// exclusive.
#define GASNETE_LC_NOW_START(op)   SET_EVENT_TYPE(op, gasnete_eop_event_alc, gasnete_event_type_lc_now)
#define GASNETE_LC_NOW_FINISH(op)  SET_EVENT_DONE(op, gasnete_eop_event_alc)
#define GASNETE_LC_NOW_DONE(op)    EVENT_DONE(op, gasnete_eop_event_alc)


// Extract root (op) and index from any event
#define gasneti_event_op(_h)  ((gasnete_op_t*)((uintptr_t)(_h) & ~7))
#define gasneti_event_idx(_h) ((uintptr_t)(_h) & 7)

// Convert root (op) to any leaf (event)
#define gasneti_op_event(_op,_idx) ((gex_Event_t)&((_op)->event[_idx]))

/* ------------------------------------------------------------------------------------ */
/* Reference implementation of eop and iop */
#if !GASNETI_DISABLE_REFERENCE_EOP

#ifndef GASNETE_EOP_CHUNKCNT
#define GASNETE_EOP_CHUNKCNT 256  // the number of eops to allocate together as a block
#endif

extern void gasnete_eop_alloc(gasneti_threaddata_t * const thread);
extern gasnete_iop_t *gasnete_iop_new(gasneti_threaddata_t * const thread);

/*  get a new op */
GASNETI_INLINE(_gasnete_eop_new)
gasnete_eop_t *_gasnete_eop_new(gasneti_threaddata_t * const thread) {
  gasnete_eop_t *eop = thread->eop_free;
  if_pf (!eop) {
    gasneti_mutex_lock(&thread->foreign_lock);
    { // no branch needed - an empty list remains empty
      eop = thread->foreign_eops;
      thread->foreign_eops = NULL;
    }
    gasneti_mutex_unlock(&thread->foreign_lock);
    if (!eop) {
      gasnete_eop_alloc(thread);
      eop = thread->eop_free;
    }
  }
  {
    thread->eop_free = eop->next;
  #if GASNET_DEBUG
    gasneti_assert_uint(eop->threadidx ,==, thread->threadidx);
    gasneti_assert_uint(eop->event[0] ,==, gasnete_event_type_free_eop);
    eop->event[0] = gasnete_event_type_eop;
  #endif
  #ifdef _GASNETE_EOP_NEW_EXTRA
    // Hook for conduit-specific initializations and assertions
    _GASNETE_EOP_NEW_EXTRA(eop);
  #endif
    gasneti_assert(GASNETE_EOP_DONE(eop));
    gasneti_assert(GASNETE_EOP_LC_DONE(eop));
    return eop;
  }
}

/*  get a new op AND mark it in flight */
GASNETI_INLINE(gasnete_eop_new)
gasnete_eop_t *gasnete_eop_new(gasneti_threaddata_t * const thread) {
  gasnete_eop_t *eop = _gasnete_eop_new(thread);
  SET_EVENT_TYPE(eop, 0, gasnete_event_type_eop);
#ifdef GASNETE_EOP_NEW_EXTRA
  // Hook for conduit-specific initializations and assertions
  GASNETE_EOP_NEW_EXTRA(eop);
#endif
  gasneti_assert(! GASNETE_EOP_DONE(eop));
  return eop;
}

#if GASNET_DEBUG
/*  query an iop for completeness -
 *  this means all catagories (puts, gets, LC, etc.)
 *  TODO-EX: DEPRECATE/REMOVE?
 *   Only used (via GASNETE_IOP_ISDONE) to assert in gasnete_free_threaddata().
 */
static
int gasnete_iop_isdone(gasnete_iop_t *iop) {
  int result;
  gasnete_iop_check(iop);
  if (iop->next) { // access region, uses op bits
    result = EVENT_ALL_DONE(iop);
  } else { // implicit iop, uses counters
    result = (GASNETE_IOP_CNTDONE(iop,get) &&
              GASNETE_IOP_CNTDONE(iop,put) &&
              GASNETE_IOP_CNTDONE(iop,rmw) &&
              GASNETE_IOP_LC_CNTDONE(iop));
  }
  return result;
}
#endif // GASNET_DEBUG

/*  mark an op done - isget ignored for explicit ops */
// TODO-EX: DEPRECATED
//   Must replace the only remaining call (in gasnet_extended_amref.c)
//   with something supporting all event categories.
static
void gasnete_op_markdone(gasnete_op_t *op, int isget) {
  if (OPTYPE(op) == OPTYPE_EXPLICIT) {
    gasnete_eop_t *eop = (gasnete_eop_t *)op;
    gasnete_eop_check(eop);
    GASNETE_EOP_MARKDONE(eop);
  } else {
    gasnete_iop_t *iop = (gasnete_iop_t *)op;
    gasnete_iop_check(iop);
    if (isget) GASNETE_IOP_CNT_FINISH(iop, get, 1, 0);
    else       GASNETE_IOP_CNT_FINISH(iop, put, 1, 0);
  }
}

/*  prepare to free an eop, but do not destroy anything that would
 *  be necessary to test/wait on the eop.
 */
GASNETI_INLINE(gasnete_eop_prep_free)
void gasnete_eop_prep_free(gasnete_eop_t *eop) {
  gasnete_eop_check(eop);
  gasneti_assert(EVENT_ALL_DONE(eop));
  gasneti_assert(GASNETE_EOP_DONE(eop));
  gasneti_assert(GASNETE_EOP_LC_DONE(eop));
#ifdef GASNETE_EOP_PREP_FREE_EXTRA
  // Hook for conduit-specific cleanups and assertions
  GASNETE_EOP_PREP_FREE_EXTRA(eop);
#endif
#if GASNET_DEBUG
  gasneti_assert_uint(eop->event[0] ,==, gasnete_event_type_eop);
  eop->event[0] = gasnete_event_type_pendingfree_eop;
#endif
}

/*  free an eop */
static
void gasnete_eop_free(gasnete_eop_t *eop GASNETI_THREAD_FARG) {
  gasnete_eop_prep_free(eop);
#ifdef GASNETE_EOP_FREE_EXTRA
  // Hook for conduit-specific cleanups
  // NOTE: Defining this adds an extra pass in {test,wait}_{some,all} and
  // therefore should only be used if there are steps that cannot safely be
  // performed in GASNETE_EOP_PREP_FREE_EXTRA.
  GASNETE_EOP_FREE_EXTRA(eop);
#endif
#if GASNET_DEBUG
  gasneti_assert_uint(eop->event[0] ,==, gasnete_event_type_pendingfree_eop);
  eop->event[0] = gasnete_event_type_free_eop;
#endif
  gasneti_threaddata_t * const thread = gasnete_threadtable[eop->threadidx];
  if_pt (thread == GASNETI_MYTHREAD) {
    eop->next = thread->eop_free;
    thread->eop_free = eop;
  } else {
    gasneti_mutex_lock(&thread->foreign_lock);
    eop->next = thread->foreign_eops;
    thread->foreign_eops = eop;
    gasneti_mutex_unlock(&thread->foreign_lock);
  }
}

#endif // GASNETI_DISABLE_REFERENCE_EOP
/* ------------------------------------------------------------------------------------ */

#endif
