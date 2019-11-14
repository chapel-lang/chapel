/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_event.c $
 * Description: GASNet event/eop/iop common code
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_event_internal.h>

extern void _gasnete_iop_check(gasnete_iop_t *iop) { gasnete_iop_check(iop); }

/* ------------------------------------------------------------------------------------ */
/*
  Op management
  =============
*/

#if !GASNETI_DISABLE_REFERENCE_EOP

// eop chunk format:
// void *next_chunk;
// *cache pad*
// eop[0]
// *cache pad*
// eop[1]
// ...

//  allocate more eops: only valid when the free pool is empty
GASNETI_NEVER_INLINE(gasnete_eop_alloc,
extern void gasnete_eop_alloc(gasneti_threaddata_t * const thread)) {
    gasnete_threadidx_t const threadidx = thread->threadidx;
    size_t const eopsz = GASNETI_ALIGNUP(sizeof(gasnete_eop_t),GASNETI_CACHE_LINE_BYTES); // eop size to ensure cache isolation
    thread->eop_num_bufs++;
    gasneti_assert(thread->eop_num_bufs); // check for overflow
    GASNETI_TRACE_PRINTF(I,("Growing thread eop pool to %"PRIuPTR, (uintptr_t)(GASNETE_EOP_CHUNKCNT*thread->eop_num_bufs)));
    // ensure we cache align the eops, even for an allocator that returns worst-case alignment
    void * const buf = gasneti_calloc(1,(sizeof(void *)-1) + (GASNETI_CACHE_LINE_BYTES-1) + eopsz * GASNETE_EOP_CHUNKCNT);
    gasneti_leak(buf);
    gasnete_eop_t * const first_eop = (gasnete_eop_t *)GASNETI_ALIGNUP((uintptr_t)buf + sizeof(void*),GASNETI_CACHE_LINE_BYTES);
    gasnete_eop_t *eop = first_eop;
    for (int i=0; i < GASNETE_EOP_CHUNKCNT; i++) {
      eop->threadidx = threadidx;
      #if GASNET_DEBUG
        // Returns to type==free_eop when on free list
        eop->event[0] = gasnete_event_type_free_eop;
      #else
        // Type==eop at all times
        eop->event[0] = gasnete_event_type_eop;
      #endif
      #ifdef GASNETE_EOP_ALLOC_EXTRA // Hook for conduit-specific initializations and assertions
        GASNETE_EOP_ALLOC_EXTRA(eop);
      #endif
      eop->next = (gasnete_eop_t *)((uintptr_t)eop + eopsz);
      eop = eop->next;
    }
    eop = (gasnete_eop_t *)((uintptr_t)eop - eopsz);   // backup to last
    eop->next = NULL;                                  // null terminate

    *(void **)buf = thread->eop_bufs; // link the chunk
    thread->eop_bufs = buf;
    gasneti_assert(!thread->eop_free);
    thread->eop_free = first_eop;

    #if GASNET_DEBUG
    { /* verify new free list got built correctly */
      gasneti_memcheck(thread->eop_bufs);
      eop = first_eop;
      for (size_t i=0; i < GASNETE_EOP_CHUNKCNT; i++) {
        size_t eopidx = (((uintptr_t)eop) - ((uintptr_t)first_eop)) / eopsz;
        gasneti_assert_uint(eopidx ,==, i); // verify linkage
        gasneti_assert_ptr(eop ,==, (gasnete_eop_t *)GASNETI_ALIGNUP(eop,GASNETI_CACHE_LINE_BYTES)); // verify cache alignment
        eop = eop->next;
      }
      gasneti_assert(eop == NULL);
    }
    #endif
}

/*  allocate a new iop */
GASNETI_NEVER_INLINE(gasnete_iop_alloc,
static gasnete_iop_t *gasnete_iop_alloc(gasneti_threaddata_t * const thread)) {
    gasnete_iop_t *iop = (gasnete_iop_t *)gasneti_malloc(sizeof(gasnete_iop_t));
    gasneti_leak(iop);
    thread->iop_num++;
    #if GASNET_DEBUG
      memset(iop, 0, sizeof(gasnete_iop_t)); /* set event[] and pad to known value */
    #else
      memset(&iop->event, 0, sizeof(iop->event)); /* set event[] to known value */
    #endif
    iop->event[0] = OPTYPE_IMPLICIT;
    iop->threadidx = thread->threadidx;
    iop->initiated_get_cnt = 0;
    iop->initiated_put_cnt = 0;
    iop->initiated_rmw_cnt = 0;
    gasnete_op_atomic_set(&(iop->completed_get_cnt), 0, 0);
    gasnete_op_atomic_set(&(iop->completed_put_cnt), 0, 0);
    gasnete_op_atomic_set(&(iop->completed_rmw_cnt), 0, 0);
  #if GASNETE_HAVE_LC
    iop->initiated_alc_cnt = 0;
    gasnete_op_atomic_set(&(iop->completed_alc_cnt), 0, 0);
  #endif
  #ifdef GASNETE_IOP_ALLOC_EXTRA
    // Hook for conduit-specific initializations and assertions
    GASNETE_IOP_ALLOC_EXTRA(iop);
  #endif
    return iop;
}

GASNETI_INLINE(_gasnete_iop_new)
gasnete_iop_t *_gasnete_iop_new(gasneti_threaddata_t * const thread) {
  gasnete_iop_t *iop = thread->iop_free;
  if_pf (!iop) {
    gasneti_mutex_lock(&thread->foreign_lock);
    { // no branch needed - an empty list remains empty
      iop = thread->foreign_iops;
      thread->foreign_iops = NULL;
    }
    gasneti_mutex_unlock(&thread->foreign_lock);
  }
  return iop;
}

/*  get a new iop */
extern
gasnete_iop_t *gasnete_iop_new(gasneti_threaddata_t * const thread) {
  gasnete_iop_t *iop = _gasnete_iop_new(thread);
  if_pt (iop) {
    thread->iop_free = iop->next;
    gasneti_memcheck(iop);
    gasneti_assert_uint(iop->threadidx ,==, thread->threadidx);
    #if GASNET_DEBUG
      gasneti_assert_uint(OPTYPE(iop) ,==, gasnete_event_type_free_iop);
      iop->event[0] = gasnete_event_type_iop;
    #endif
    /* If using trace or stats, want meaningful counts when tracing NBI access regions */
    #if GASNETI_STATS_OR_TRACE
      iop->initiated_get_cnt = 0;
      iop->initiated_put_cnt = 0;
      gasnete_op_atomic_set(&(iop->completed_get_cnt), 0, 0);
      gasnete_op_atomic_set(&(iop->completed_put_cnt), 0, 0);
    #endif
  } else {
    iop = gasnete_iop_alloc(thread);
  }
  iop->next = NULL;
#ifdef GASNETE_IOP_NEW_EXTRA
  // Hook for conduit-specific initializations and assertions
  GASNETE_IOP_NEW_EXTRA(iop);
#endif
  gasnete_iop_check(iop);
  return iop;
}


/* prepare to free an iop, but do not destroy anything that would
 * be necessary to test/wait on the iop.
 */
GASNETI_INLINE(gasnete_iop_prep_free)
void gasnete_iop_prep_free(gasnete_iop_t *iop) {
  gasnete_iop_check(iop);
  gasneti_assert(EVENT_ALL_DONE(iop));
  gasneti_assert(GASNETE_IOP_CNTDONE(iop,get));
  gasneti_assert(GASNETE_IOP_CNTDONE(iop,put));
  gasneti_assert(GASNETE_IOP_CNTDONE(iop,rmw));
  gasneti_assert(GASNETE_IOP_LC_CNTDONE(iop));
  gasneti_assert(iop->next == iop);
#ifdef GASNETE_IOP_PREP_FREE_EXTRA
  // Hook for conduit-specific cleanups and assertions
  GASNETE_IOP_PREP_FREE_EXTRA(iop);
#endif
#if GASNET_DEBUG
  gasneti_assert_uint(iop->event[0] ,==, gasnete_event_type_iop);
  iop->event[0] = gasnete_event_type_pendingfree_iop;
#endif
}

/*  free an iop */
static
void gasnete_iop_free(gasnete_iop_t *iop GASNETI_THREAD_FARG) {
  gasnete_iop_prep_free(iop);
#ifdef GASNETE_IOP_FREE_EXTRA
  // Hook for conduit-specific cleanups
  // NOTE: Defining this adds an extra pass in {test,wait}_{some,all} and
  // therefore should only be used if there are steps that cannot safely be
  // performed in GASNETE_IOP_PREP_FREE_EXTRA.
  GASNETE_IOP_FREE_EXTRA(iop);
#endif
#if GASNET_DEBUG
  gasneti_assert_uint(iop->event[0] ,==, gasnete_event_type_pendingfree_iop);
  iop->event[0] = gasnete_event_type_free_iop;
#endif
  gasneti_threaddata_t * const thread = gasnete_threadtable[iop->threadidx];
  if (thread == GASNETI_MYTHREAD) {
    iop->next = thread->iop_free;
    thread->iop_free = iop;
  } else {
    gasneti_mutex_lock(&thread->foreign_lock);
    iop->next = thread->foreign_iops;
    thread->foreign_iops = iop;
    gasneti_mutex_unlock(&thread->foreign_lock);
  }
}

#endif // GASNETI_DISABLE_REFERENCE_EOP

/* ------------------------------------------------------------------------------------ */
/* GASNET-Internal OP Interface */

// TODO-EX: EOP_INTERFACE
//   Must generalize IOP to encompass LC, RMW and future Event Categories
//     Anticipated implementation is to replace 'isget' with a member of the
//     gasnete_iop_event_* enum, and to use it to compute the address of the
//     initiated and completed counters.
//   Must generalize EOP to encompass LC
//     Initially we only have PUT and LC subevents, but the generalization
//     should allow for more, and thus likely resembles the plan for IOP.

gasneti_eop_t *gasneti_eop_create(GASNETI_THREAD_FARG_ALONE) {
  gasnete_eop_t *op = gasnete_eop_new(GASNETI_MYTHREAD);
  return (gasneti_eop_t *)op;
}
gasneti_iop_t *gasneti_iop_register(unsigned int noperations, int isget GASNETI_THREAD_FARG) {
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t * const op = mythread->current_iop;
  gasnete_iop_check(op);
  if (isget) op->initiated_get_cnt += noperations;
  else       op->initiated_put_cnt += noperations;
  gasnete_iop_check(op);
  return (gasneti_iop_t *)op;
}
int gasneti_op_is_eop(void *_op) {
  gasnete_op_t * op = _op;
  return OPTYPE(op) == OPTYPE_EXPLICIT;
}

void gasneti_eop_markdone(gasneti_eop_t *eop) {
  gasnete_eop_t *op = (gasnete_eop_t *)eop;
  gasnete_eop_check(op);
  GASNETE_EOP_MARKDONE(op);
}
void gasneti_iop_markdone(gasneti_iop_t *iop, unsigned int noperations, int isget) {
  gasnete_iop_t *op = (gasnete_iop_t *)iop;
  if (isget) GASNETE_IOP_CNT_FINISH(op, get, noperations, 0);
  else       GASNETE_IOP_CNT_FINISH(op, put, noperations, 0);
  gasnete_iop_check(op);
}

// TODO-EX: EOP_INTERFACE
//   These next two are a stop-gap pending proper generalization.

gasneti_iop_t *gasneti_iop_register_rmw(unsigned int noperations GASNETI_THREAD_FARG) {
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t * const op = mythread->current_iop;
  gasnete_iop_check(op);
  op->initiated_rmw_cnt += noperations;
  gasnete_iop_check(op);
  return (gasneti_iop_t *)op;
}
void gasneti_iop_markdone_rmw(gasneti_iop_t *iop, unsigned int noperations) {
  gasnete_iop_t *op = (gasnete_iop_t *)iop;
  GASNETE_IOP_CNT_FINISH(op, rmw, noperations, 0);
  gasnete_iop_check(op);
}

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for explicit-event non-blocking operations:
  ===========================================================
*/

#ifndef gasnete_test
/*  query an op for completeness 
 *  free it if complete
 *  returns 0 or 1 */
GASNETI_INLINE(gasnete_op_try_free)
int gasnete_op_try_free(gex_Event_t event GASNETI_THREAD_FARG) {
#ifdef GASNETE_OP_TRY_FREE_EXTRA
  // Hook to operate on conduit-specific events
  GASNETE_OP_TRY_FREE_EXTRA(event);
  #error "GASNETE_OP_TRY_FREE_EXTRA not implemented in array test/wait ops"
#endif

  gasnete_event_check(event);

  // "Fast-path" detects outstanding event w/o any branches
  if (EVENT_LIVE_MASK & *(volatile uint8_t *)event) return 0;

  // "Slow-path" must distinguish root from leaf
  const unsigned int idx = gasneti_event_idx(event);
  if_pt (! idx) { // It's a root event event
    if (EVENT_ANY_LIVE(event)) return 0;

    // TODO-EX:
    // Could potentially weaken "sync_reads" for some cases?
    // However, that might not be worth the branching it would require.
    gasneti_sync_reads();

    // TODO-EX: the mask operation in OPTYPE() unnecessary?
    if_pt (OPTYPE((gasnete_op_t*)event) == OPTYPE_EXPLICIT) {
      gasnete_eop_free((gasnete_eop_t*)event GASNETI_THREAD_PASS);
    } else {
      gasnete_iop_free((gasnete_iop_t*)event GASNETI_THREAD_PASS);
    }
  } else { // It's a leaf event
    gasneti_assert(EVENT_DONE(gasneti_event_op(event), idx)); // confirm the EVENT_LIVE_MASK result
    gasneti_compiler_fence(); // TODO-EX: revisit this
  }

  return 1;
}

extern int  gasnete_test(gex_Event_t event GASNETI_THREAD_FARG) {
  gasneti_assert(event != GEX_EVENT_INVALID); // invalid handled inline in header
  return gasnete_op_try_free(event GASNETI_THREAD_PASS) ? GASNET_OK : GASNET_ERR_NOT_READY;
}
#endif

#if !defined(gasnete_test_all) || \
    !defined(gasnete_test_some)
GASNETI_INLINE(gasnete_bulk_free_eops)
void gasnete_bulk_free_eops(gasnete_eop_t *head, gasnete_eop_t **tail_p,
                       gasneti_threaddata_t * const thread GASNETI_THREAD_FARG) {
  gasneti_assert_uint(head->threadidx ,==, thread->threadidx); // TODO: validate entire list?
#if GASNETI_MAX_THREADS > 1
  if_pf (thread != GASNETI_MYTHREAD) {
    gasneti_mutex_lock(&thread->foreign_lock);
    *tail_p = thread->foreign_eops;
    thread->foreign_eops = head;
    gasneti_mutex_unlock(&thread->foreign_lock);
  } else
#endif
  {
    *tail_p = thread->eop_free;
    thread->eop_free = head;
  }
}
GASNETI_INLINE(gasnete_bulk_free_iops)
void gasnete_bulk_free_iops(gasnete_iop_t *head, gasnete_iop_t **tail_p,
                       gasneti_threaddata_t * const thread GASNETI_THREAD_FARG) {
  gasneti_assert_uint(head->threadidx ,==, thread->threadidx); // TODO: validate entire list?
#if GASNETI_MAX_THREADS > 1
  if_pf (thread != GASNETI_MYTHREAD) {
    gasneti_mutex_lock(&thread->foreign_lock);
    *tail_p = thread->foreign_iops;
    thread->foreign_iops = head;
    gasneti_mutex_unlock(&thread->foreign_lock);
  } else
#endif
  {
    *tail_p = thread->iop_free;
    thread->iop_free = head;
  }
}
GASNETI_INLINE(gasnete_test_array)
int gasnete_test_array(const int is_all, gex_Event_t *pevent, size_t numevents GASNETI_THREAD_FARG) {
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
#if GASNETI_MAX_THREADS > 1
  gasnete_threadidx_t eop_threadidx = GASNETE_INVALID_THREADIDX;
  gasnete_threadidx_t iop_threadidx = GASNETE_INVALID_THREADIDX;
  gasnete_eop_t *eop_head[2] = {NULL, NULL};
  gasnete_eop_t **eop_tail_p[2] = {&eop_head[0], &eop_head[1]};
  gasnete_iop_t *iop_head[2] = {NULL, NULL};
  gasnete_iop_t **iop_tail_p[2] = {&iop_head[0], &iop_head[1]};
#else
  const gasnete_threadidx_t eop_threadidx = 0;
  const gasnete_threadidx_t iop_threadidx = 0;
  gasnete_eop_t *eop_head[1] = {NULL};
  gasnete_eop_t **eop_tail_p[1] = {&eop_head[0]};
  gasnete_iop_t *iop_head[1] = {NULL};
  gasnete_iop_t **iop_tail_p[1] = {&iop_head[0]};
#endif
  int all_synced = 1;
  int some_synced = 0;
  int roots_synced = 0;
  int empty = 1;
  size_t to_retest = 0;

  gasneti_assert(pevent);

  // NOTE: We must allow leaves and their corresponding roots to appear in the
  // array in any order while ensuring that we don't sync a root but not some
  // corresponding leaf.  The current approach is to make two passes, the first
  // testing all events.  If and only if the first pass synced at least one
  // root and failed to sync at least one leaf, a second pass is made to retest
  // all of the remaining leaves.
  //
  // Between the two passes, the roots synced in the first pass are kept on
  // temporary linked-lists to avoid them being recycled from the free list
  // while their leaves are still being tested.  Even in the absence of a
  // second pass, this will yield an efficient "bulk free".

  // Pass 1: test all events, evaluate 'empty', and count 'to_retest'
  for (size_t i = 0; i < numevents; i++) {
    const gex_Event_t event = pevent[i];
    if (GEX_EVENT_INVALID != event) {
      gasnete_event_check(event);
      empty = 0;
      if (gasneti_event_idx(event)) { // It's a leaf
        if (EVENT_LIVE_MASK & *(volatile uint8_t *)pevent[i]) {
          // Do NOT "all_synced = 0" since we'll retry this leaf in second pass
          to_retest += 1;
          continue;
        }
      } else { // It's a root
        if (EVENT_ANY_LIVE(event)) {
          all_synced = 0;
          continue;
        }

        // TODO-EX: we need to revist this sync_reads() calll.
        // Can we weaken it for some cases (if the branches don't eliminate the benfits)?
        // Can we ever safely move it out of this loop?
        // Should it move inside the ..._prep_free() calls and become DEBUG only?
        // Of course, if any Gets were syned we still need at least one RMB before return.
        gasneti_sync_reads();

        gasnete_op_t *op = (gasnete_op_t*)event;
      #if GASNETI_MAX_THREADS > 1
        int other;
      #else
        const int other = 0;
      #endif
        if (OPTYPE(op) == OPTYPE_EXPLICIT) { // TODO-EX: the mask operation in OPTYPE() unnecessary?
        #if GASNETI_MAX_THREADS > 1
          if (eop_threadidx == GASNETE_INVALID_THREADIDX) {
            eop_threadidx = op->threadidx;
            other = 0;
          } else {
            other = (eop_threadidx != op->threadidx);
          }
        #endif
          gasnete_eop_t *eop = (gasnete_eop_t*)op;
          gasnete_eop_prep_free(eop);
          *eop_tail_p[other] = eop;
          eop_tail_p[other] = &eop->next;
          eop->next = NULL;
        } else {
        #if GASNETI_MAX_THREADS > 1
          if (iop_threadidx == GASNETE_INVALID_THREADIDX) {
            iop_threadidx = op->threadidx;
            other = 0;
          } else {
            other = (iop_threadidx != op->threadidx);
          }
        #endif
          gasnete_iop_t *iop = (gasnete_iop_t*)op;
          gasnete_iop_prep_free(iop);
          *iop_tail_p[other] = iop;
          iop_tail_p[other] = &iop->next;
          iop->next = NULL;
        }

        roots_synced = 1;
      }

      pevent[i] = GEX_EVENT_INVALID;
      some_synced = 1;
    }
  }

  // Pass 2: retest all still-live leaf events (if any)
  gasneti_assert(! gasneti_event_idx(GEX_EVENT_INVALID));
  if (to_retest) {
    if (roots_synced) {
      size_t to_test = to_retest;
      for (size_t i = 0; to_test; i++) {
        gasneti_assert_uint(i ,<, numevents);
        const gex_Event_t event = pevent[i];
        if (gasneti_event_idx(event)) {
          to_test -= 1;
          if (EVENT_LIVE_MASK & *(volatile uint8_t *)event) {
            all_synced = 0;
          } else {
            pevent[i] = GEX_EVENT_INVALID;
	    some_synced = 1;
          }
        }
      }
    } else {
      all_synced = 0;
    }
  }

  // Bulk free "main" (first seen) thread's eops and iops
  if (eop_head[0]) {
  #if defined(GASNET_DEBUG) || defined(GASNETE_EOP_FREE_EXTRA)
    gasnete_eop_t *eop = eop_head[0];
    do {
    #ifdef GASNETE_EOP_FREE_EXTRA
      GASNETE_EOP_FREE_EXTRA(eop);
    #endif
    #if GASNET_DEBUG
      eop->event[0] = gasnete_event_type_free_eop;
    #endif
      eop = eop->next;
    } while(eop);
  #endif
    gasnete_bulk_free_eops(eop_head[0], eop_tail_p[0], gasnete_threadtable[eop_threadidx] GASNETI_THREAD_PASS);
  }
  if (iop_head[0]) {
  #if defined(GASNET_DEBUG) || defined(GASNETE_IOP_FREE_EXTRA)
    gasnete_iop_t *iop = iop_head[0];
    do {
    #ifdef GASNETE_IOP_FREE_EXTRA
      GASNETE_EOP_FREE_EXTRA(iop);
    #endif
    #if GASNET_DEBUG
      iop->event[0] = gasnete_event_type_free_iop;
    #endif
      iop = iop->next;
    } while(iop);
  #endif
    gasnete_bulk_free_iops(iop_head[0], iop_tail_p[0], gasnete_threadtable[iop_threadidx] GASNETI_THREAD_PASS);
  }

#if GASNETI_MAX_THREADS > 1
  // Individually (non-bulk) free remaining eops and iops (not beloning to "main" thread)
  // TODO-EX: sort/bin to amortize lock accesses?
  {
    gasnete_eop_t *eop = eop_head[1];
    while (eop) {
    #ifdef GASNETE_EOP_FREE_EXTRA
      GASNETE_EOP_FREE_EXTRA(eop);
    #endif
    #if GASNET_DEBUG
      eop->event[0] = gasnete_event_type_free_eop;
    #endif
      gasnete_eop_t * const next = eop->next;
      gasnete_bulk_free_eops(eop, &eop->next, gasnete_threadtable[eop->threadidx] GASNETI_THREAD_PASS);
      eop = next;
    }

    gasnete_iop_t *iop = iop_head[1];
    while (iop) {
    #ifdef GASNETE_IOP_FREE_EXTRA
      GASNETE_EOP_FREE_EXTRA(iop);
    #endif
    #if GASNET_DEBUG
      iop->event[0] = gasnete_event_type_free_iop;
    #endif
      gasnete_iop_t * const next = iop->next;
      gasnete_bulk_free_iops(iop, &iop->next, gasnete_threadtable[iop->threadidx] GASNETI_THREAD_PASS);
      iop = next;
    }
  }
#endif

  return is_all ? all_synced : (some_synced || empty);
}
#endif

#ifndef gasnete_test_some
extern int  gasnete_test_some (gex_Event_t *pevent, size_t numevents GASNETI_THREAD_FARG) {
  return gasnete_test_array(0, pevent, numevents GASNETI_THREAD_PASS) ? GASNET_OK : GASNET_ERR_NOT_READY;
}
#endif

#ifndef gasnete_test_all
extern int  gasnete_test_all (gex_Event_t *pevent, size_t numevents GASNETI_THREAD_FARG) {
  return gasnete_test_array(1, pevent, numevents GASNETI_THREAD_PASS) ? GASNET_OK : GASNET_ERR_NOT_READY;
}
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for implicit-event non-blocking operations:
  ===========================================================
*/

#ifndef gasnete_test_syncnbi_mask
// TODO-EX: public header should dispatch to specialized functions on constant mask
extern int gasnete_test_syncnbi_mask(gex_EC_t mask, gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t *iop = mythread->current_iop;
  gasneti_assert_uint(iop->threadidx ,==, mythread->threadidx);
  gasneti_assert_uint(OPTYPE(iop) ,==, OPTYPE_IMPLICIT);
  #if GASNET_DEBUG
    if (iop->next != NULL)
      gasneti_fatalerror("VIOLATION: attempted to call gex_NBI_Test() inside an NBI access region");
  #endif

#if 0
  // Version suitable for inlining when mask is constant
  if (mask & GASNETI_EC_ALC) {
    if (! GASNETE_IOP_LC_CNTDONE(iop)) return GASNET_ERR_NOT_READY;
  }
  if (mask & GASNETI_EC_PUT) {
    if (! GASNETE_IOP_CNTDONE(iop,put)) return GASNET_ERR_NOT_READY;
  }
  if (mask & GASNETI_EC_GET) {
    if (! GASNETE_IOP_CNTDONE(iop,get)) return GASNET_ERR_NOT_READY;
  }
  if (mask & GASNETI_EC_RMW) {
    if (! GASNETE_IOP_CNTDONE(iop,rmw)) return GASNET_ERR_NOT_READY;
  }
#else
  // Version to reduce branches when mask is unknown
  gex_EC_t live_mask = 0;
  if (! GASNETE_IOP_LC_CNTDONE(iop))  live_mask |= GASNETI_EC_ALC;
  if (! GASNETE_IOP_CNTDONE(iop,put)) live_mask |= GASNETI_EC_PUT;
  if (! GASNETE_IOP_CNTDONE(iop,get)) live_mask |= GASNETI_EC_GET;
  if (! GASNETE_IOP_CNTDONE(iop,rmw)) live_mask |= GASNETI_EC_RMW;
  if (mask & live_mask) return GASNET_ERR_NOT_READY;
#endif

  // TODO-EX: revisit this logic
  if (mask & (GASNETI_EC_GET|GASNETI_EC_RMW)) {
    gasneti_sync_reads();
  } else {
    gasneti_compiler_fence();
  }

  return GASNET_OK;
}
#endif


/* ------------------------------------------------------------------------------------ */
/*
  Implicit access region synchronization
  ======================================
*/
/*  This implementation allows recursive access regions, although the spec does not require that */
/*  operations are associated with the most immediately enclosing access region */
#ifndef gasnete_begin_nbi_accessregion
extern void gasnete_begin_nbi_accessregion(gex_Flags_t flags, int allowrecursion GASNETI_THREAD_FARG) {
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t *iop = gasnete_iop_new(mythread); /*  push an iop */
  GASNETI_TRACE_PRINTF(S,("BEGIN_NBI_ACCESSREGION"));
  #if GASNET_DEBUG
    if (!allowrecursion && mythread->current_iop->next != NULL)
      gasneti_fatalerror("VIOLATION: tried to initiate a recursive NBI access region");
  #endif

  // "Arm" all events and offset the counters to prevent them "firing" prematurely
  // TODO: should merge SET_EVENT_TYPE() calls into one write.
  iop->initiated_put_cnt++;
  iop->initiated_get_cnt++;
  iop->initiated_rmw_cnt++;
  SET_EVENT_TYPE(iop, gasnete_iop_event_put, gasnete_event_type_iop);
  SET_EVENT_TYPE(iop, gasnete_iop_event_get, gasnete_event_type_iop);
  SET_EVENT_TYPE(iop, gasnete_iop_event_rmw, gasnete_event_type_iop);
#if GASNETE_HAVE_LC
  iop->initiated_alc_cnt++;
  SET_EVENT_TYPE(iop, gasnete_iop_event_alc, gasnete_event_type_lc);
#endif

  iop->next = mythread->current_iop;
  mythread->current_iop = iop;
}
#endif

#ifndef gasnete_end_nbi_accessregion
extern gex_Event_t gasnete_end_nbi_accessregion(gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t *iop = mythread->current_iop; /*  pop an iop */
  GASNETI_TRACE_EVENT_VAL(S,END_NBI_ACCESSREGION,iop->initiated_get_cnt + iop->initiated_put_cnt);

  // Balance the offsets applied to each counter by begin_nbi_accessregion
  GASNETE_IOP_CNT_FINISH_REG(iop, put, 1, 0);
  GASNETE_IOP_CNT_FINISH_REG(iop, get, 1, 0);
  GASNETE_IOP_CNT_FINISH_REG(iop, rmw, 1, 0);
#if GASNETE_HAVE_LC
  GASNETE_IOP_CNT_FINISH_REG(iop, alc, 1, 0);
#endif

  #if GASNET_DEBUG
    if (iop->next == NULL)
      gasneti_fatalerror("VIOLATION: call to gasnete_end_nbi_accessregion() outside access region");
  #endif
  mythread->current_iop = iop->next;
  iop->next = iop; /* Identifies an iop returned from access region */
  return (gex_Event_t)iop;
}
#endif

#ifndef gasnete_Event_QueryLeaf
#if GASNET_DEBUG
static void _gasnete_get_leaf_check(gasnete_op_t *op, gex_EC_t event_id) {
  gasneti_assert(! gasneti_event_idx(op));
  switch (OPTYPE(op)) {
    case OPTYPE_IMPLICIT: {
      gasnete_iop_t *iop = (gasnete_iop_t*)op;
      gasnete_iop_check(iop);
      gasneti_assert(iop->next); // was returned from access region
      switch (event_id) {
        case GEX_EC_PUT:  // fall-through...
        case GEX_EC_GET:  // fall-through...
        case GEX_EC_RMW:  // fall-through...
        case GEX_EC_LC:    return;
      }
      break;
    }
    case OPTYPE_EXPLICIT: {
      gasnete_eop_t *eop = (gasnete_eop_t*)op;
      gasnete_eop_check(eop);
      switch (event_id) {
        case GEX_EC_LC: return;
      }
      break;
    }
  }
  gasneti_fatalerror("Invalid arguments to gex_Event_QueryLeaf()");
}
#else
  #define _gasnete_get_leaf_check(op, event_id) ((void)0)
#endif

extern gex_Event_t gasnete_Event_QueryLeaf(gex_Event_t root, gex_EC_t event_id) {
  if (root == GEX_EVENT_INVALID) return GEX_EVENT_INVALID;
  gasnete_op_t *op = (gasnete_op_t*)root;
  _gasnete_get_leaf_check(op, event_id);

  gasneti_static_assert(gasnete_eop_event_alc == gasnete_iop_event_alc);

  switch (event_id) {
    // TODO_EX: did we really want to allow extraction of PUT and GET (root) events?
    case GEX_EC_PUT: return gasneti_op_event(op, gasnete_iop_event_put);
    case GEX_EC_GET: return gasneti_op_event(op, gasnete_iop_event_get);
    case GEX_EC_RMW: return gasneti_op_event(op, gasnete_iop_event_rmw);
    case GEX_EC_LC:  return gasneti_op_event(op, gasnete_iop_event_alc);
  }

  gasneti_fatalerror("Invalid arguments to gex_Event_QueryLeaf()");
  return GEX_EVENT_INVALID; // NOT REACHED
}
#endif

/* ------------------------------------------------------------------------------------ */

