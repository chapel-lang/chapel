/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ibv-conduit/gasnet_extended.c $
 * Description: GASNet Extended API over IB Verbs Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_coll_internal.h> // for refbarrier.c
#include <gasnet_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_ibv.h>

/* ------------------------------------------------------------------------------------ */
/*
  Extended API Common Code
  ========================
  Factored bits of extended API code common to most conduits, overridable when necessary
*/

extern void gasnetc_new_threaddata_callback(void **core_threadinfo);
#define GASNETC_NEW_THREADDATA_CALLBACK(td) gasnetc_new_threaddata_callback(&((td)->gasnetc_threaddata))

#include "gasnet_extended_common.c"

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* called at startup to check configuration sanity */
static void gasnete_check_config(void) {
  gasneti_check_config_postattach();
  gasnete_check_config_amref();

  gasneti_assert(sizeof(gasnete_eop_t) >= sizeof(void*));
}

extern void gasnete_init(void) {
  static int firstcall = 1;
  GASNETI_TRACE_PRINTF(C,("gasnete_init()"));
  gasneti_assert(firstcall); /*  make sure we haven't been called before */
  firstcall = 0;

  gasnete_check_config(); /*  check for sanity */

  gasneti_assert(gasneti_nodes >= 1 && gasneti_mynode < gasneti_nodes);

  { gasneti_threaddata_t *threaddata = NULL;
  #if GASNETI_MAX_THREADS > 1
    /* register first thread (optimization) */
    threaddata = _gasneti_mythread_slow();
  #else
    /* register only thread (required) */
    threaddata = gasnete_new_threaddata();
  #endif
  #if !GASNETI_DISABLE_REFERENCE_EOP
    /* cause the first pool of eops to be allocated (optimization) */
    GASNET_POST_THREADINFO(threaddata);
    gasnete_eop_t *eop = gasnete_eop_new(threaddata);
    GASNETE_EOP_MARKDONE(eop);
    gasnete_eop_free(eop GASNETI_THREAD_PASS);
  #endif
  }

  /* Initialize barrier resources */
  gasnete_barrier_init();

  /* Initialize team/collectives */
  gasnete_coll_init_subsystem();

  /* Initialize VIS subsystem */
  gasnete_vis_init();
}

/* ------------------------------------------------------------------------------------ */
/*
  Get/Put:
  ========
*/

/* Use some or all of the reference implementation of get/put in terms of AMs
 * Configuration appears in gasnet_extended_fwd.h
 */
#include "gasnet_extended_amref.c"

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (explicit event)
  ==========================================================
*/
/* ------------------------------------------------------------------------------------ */

/* Conduits not using the gasnete_amref_ versions should implement at least the following:
     gasnete_get_nb
     gasnete_put_nb
*/

extern
gex_Event_t gasnete_get_nb(
                     gex_TM_t tm,
                     void *dest,
                     gex_Rank_t rank, void *src,
                     size_t nbytes,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_GET(tm,dest,rank,src,nbytes);
 {
  gasnete_eop_t *op = gasnete_eop_new(GASNETI_MYTHREAD);

  /* XXX check error returns */
  gasnetc_rdma_get(tm, rank, src, dest, nbytes, flags,
                   &op->initiated_cnt, gasnetc_cb_eop_get
                   GASNETI_THREAD_PASS);
  return (gex_Event_t)op;
 }
}

extern
gex_Event_t gasnete_put_nb(
                     gex_TM_t tm,
                     gex_Rank_t rank, void *dest,
                     void *src,
                     size_t nbytes, gex_Event_t *lc_opt,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUT(tm,rank,dest,src,nbytes);

  gasnete_eop_t *op = gasnete_eop_new(GASNETI_MYTHREAD);

  /* XXX check error returns */
  #define GASNETC_RDMA_PUT(local_cnt, local_cb)              \
    gasnetc_rdma_put(tm, rank, src, dest, nbytes, flags,     \
                     local_cnt, local_cb,                    \
                     &op->initiated_cnt, gasnetc_cb_eop_put  \
                     GASNETI_THREAD_PASS)

  if (lc_opt == GEX_EVENT_NOW) {
    gasnetc_counter_t counter = GASNETC_COUNTER_INITIALIZER;
    GASNETC_RDMA_PUT(&counter.initiated, gasnetc_cb_counter);
    gasnetc_counter_wait(&counter, 0 GASNETI_THREAD_PASS);
  } else if (lc_opt == GEX_EVENT_DEFER) {
    GASNETC_RDMA_PUT(NULL, NULL);
  } else {
  // Intel C prior to 2019.0.117 (builddate 20180804) issues a buggy warning here
  #if !(PLATFORM_COMPILER_INTEL && PLATFORM_COMPILER_VERSION_LT(19,0,20180800))
    gasneti_assume(gasneti_leaf_is_pointer(lc_opt));
  #endif
    GASNETE_EOP_LC_START(op);
    gasnetc_atomic_val_t start_cnt = op->initiated_alc;
    GASNETC_RDMA_PUT(&op->initiated_alc, gasnetc_cb_eop_alc);
    if (start_cnt == op->initiated_alc) {
      // Synchronous LC - reset the eop's LC state
      GASNETE_EOP_LC_FINISH(op);
      *lc_opt = GEX_EVENT_INVALID;
    } else {
      *lc_opt = gasneti_op_event(op, gasnete_eop_event_alc);
    }
  }

  return (gex_Event_t)op;
}
#undef GASNETC_RDMA_PUT

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (implicit event)
  ==========================================================
*/
/* ------------------------------------------------------------------------------------ */

/* Conduits not using the gasnete_amref_ versions should implement at least the following:
     gasnete_get_nbi
     gasnete_put_nbi
*/

extern
int gasnete_get_nbi (gex_TM_t tm,
                     void *dest,
                     gex_Rank_t rank, void *src,
                     size_t nbytes,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_GET(tm,dest,rank,src,nbytes);
 {
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t *op = mythread->current_iop;

  /* XXX check error returns */ 
  gasnetc_rdma_get(tm, rank, src, dest, nbytes, flags,
                   &op->initiated_get_cnt,
                   op->next ? gasnetc_cb_nar_get : gasnetc_cb_iop_get
                   GASNETI_THREAD_PASS);
  return 0;
 }
}

extern
int gasnete_put_nbi (gex_TM_t tm,
                     gex_Rank_t rank, void *dest,
                     void *src,
                     size_t nbytes, gex_Event_t *lc_opt,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUT(tm,rank,dest,src,nbytes);
 {
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t *op = mythread->current_iop;
  gasnetc_counter_t    counter = GASNETC_COUNTER_INITIALIZER;
  gasnetc_atomic_val_t *local_cnt;
  gasnetc_cb_t         local_cb;

  if (lc_opt == GEX_EVENT_GROUP) {
    local_cnt = &op->initiated_alc_cnt;
    local_cb = op->next ? gasnetc_cb_nar_alc : gasnetc_cb_iop_alc;
  } else if (lc_opt == GEX_EVENT_NOW) {
    local_cnt = &counter.initiated;
    local_cb = gasnetc_cb_counter;
  } else if (lc_opt == GEX_EVENT_DEFER) {
    local_cnt = NULL;
    local_cb = NULL;
  } else {
    gasneti_fatalerror("Invalid lc_opt argument to Put_nbi");
  }

  /* XXX check error returns */ 
  gasnetc_rdma_put(tm, rank, src, dest, nbytes, flags,
                   local_cnt, local_cb,
                   &op->initiated_put_cnt,
                   op->next ? gasnetc_cb_nar_put : gasnetc_cb_iop_put
                   GASNETI_THREAD_PASS);
  if (lc_opt == GEX_EVENT_NOW) gasnetc_counter_wait(&counter, 0 GASNETI_THREAD_PASS);
  return 0;
 }
}

/* ------------------------------------------------------------------------------------ */
/*
  Blocking memory-to-memory transfers
  ===================================
*/

extern int gasnete_get  (gex_TM_t tm,
                         void* dest,
                         gex_Rank_t rank, void *src,
                         size_t nbytes, gex_Flags_t flags
                         GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_GET(tm,dest,rank,src,nbytes);
 {
  gasnetc_counter_t req_oust = GASNETC_COUNTER_INITIALIZER;

  /* XXX check error returns */ 
  gasnetc_rdma_get(tm, rank, src, dest, nbytes, flags,
                   &req_oust.initiated, gasnetc_cb_counter_rel
                   GASNETI_THREAD_PASS);
  gasnetc_counter_wait(&req_oust, 0 GASNETI_THREAD_PASS);
  return 0;
 }
}

extern int gasnete_put  (gex_TM_t tm,
                         gex_Rank_t rank, void* dest,
                         /*const*/ void *src,
                         size_t nbytes, gex_Flags_t flags
                         GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUT_NOLC(tm,rank,dest,src,nbytes);
 {
  gasnetc_counter_t req_oust = GASNETC_COUNTER_INITIALIZER;

  /* XXX check error returns */ 
  gasnetc_rdma_put(tm, rank, src, dest, nbytes, flags,
                   NULL, NULL,
                   &req_oust.initiated, gasnetc_cb_counter
                   GASNETI_THREAD_PASS);
  gasnetc_counter_wait(&req_oust, 0 GASNETI_THREAD_PASS);
  return 0;
 }
}   

/* ------------------------------------------------------------------------------------ */
/*
  Barriers:
  =========
  "ibd" = IB Dissemination
*/

/* Safe only for 64-bit PCI bus, which we can assume only for 64-bit CPU */
#if PLATFORM_ARCH_64
  #define GASNETE_BARRIER_DEFAULT "IBDISSEM"
#endif

/* Forward decls for init function(s): */
static void gasnete_ibdbarrier_init(gasnete_coll_team_t team);

#define GASNETE_BARRIER_READENV() do {                                  \
  if (GASNETE_ISBARRIER("IBDISSEM"))                                    \
    gasnete_coll_default_barrier_type = GASNETE_COLL_BARRIER_IBDISSEM;  \
} while (0)

#define GASNETE_BARRIER_INIT(TEAM, TYPE, NODES, SUPERNODES) do { \
    if ((TYPE) == GASNETE_COLL_BARRIER_IBDISSEM &&               \
        (TEAM) == GASNET_TEAM_ALL) {                             \
      gasnete_ibdbarrier_init(TEAM);                             \
    }                                                            \
  } while (0)

/* Can use the auxseg allocation from the generic implementation: */
static int gasnete_conduit_rdmabarrier(const char *barrier, gasneti_auxseg_request_t *result);
#define GASNETE_CONDUIT_RDMABARRIER gasnete_conduit_rdmabarrier

/* use reference implementation of barrier */
#define GASNETI_GASNET_EXTENDED_REFBARRIER_C 1
#include "gasnet_extended_refbarrier.c"
#undef GASNETI_GASNET_EXTENDED_REFBARRIER_C

static int gasnete_conduit_rdmabarrier(const char *barrier, gasneti_auxseg_request_t *result) {
  if (0 == strcmp(barrier, "IBDISSEM")) {
    /* TODO: could keep the full space and allocate some to additional teams */
    size_t request;
#if GASNETI_PSHM_BARRIER_HIER
    const int is_hier = gasneti_getenv_yesno_withdefault("GASNET_PSHM_BARRIER_HIER", 1);
    const int size = is_hier ? gasneti_nodemap_global_count : gasneti_nodes;
#else
    const int size = gasneti_nodes;
#endif
    int steps, i;

    for (steps=0, i=1; i<size; ++steps, i*=2) /* empty */ ;

    request = 2 * steps * GASNETE_RDMABARRIER_INBOX_SZ;
    gasneti_assert_always(request <= result->optimalsz);
    result->minsz = request;
    result->optimalsz = request;
    return (steps != 0);
  }

  return 0;
}

/* ------------------------------------------------------------------------------------ */
/* IB-specific RDMA-based Dissemination implementation of barrier
 * This is a minor variation on the "rmd" barrier in extended-ref.
 * Key differences:
 *  + no complications due to thread-specific events
 *  + no eop completion latency
 * TODO: factor the common elements
 */

/* Reusing gasnete_coll_rmdbarrier_inbox_t from the reference implementation */

typedef struct {
  GASNETE_RMDBARRIER_LOCK(barrier_lock) /* no semicolon */
  struct {
    gex_Rank_t    jobrank;
    uintptr_t     addr;
  } *barrier_peers;           /*  precomputed list of peers to communicate with */
#if GASNETI_PSHM_BARRIER_HIER
  gasnete_pshmbarrier_data_t *barrier_pshm; /* non-NULL if using hierarchical code */
  int barrier_passive;        /*  2 if some other node makes progress for me, 0 otherwise */
#endif
  int barrier_size;           /*  ceil(lg(nodes)) */
  int barrier_goal;           /*  (1+ceil(lg(nodes)) << 1) == final barrier_state for phase=0 */
  int volatile barrier_state; /*  (step << 1) | phase, where step is 1-based (0 is pshm notify) */
  int volatile barrier_value; /*  barrier value (evolves from local value) */
  int volatile barrier_flags; /*  barrier flags (evolves from local value) */
  void *barrier_inbox;        /*  in-segment memory to recv notifications */
} gasnete_coll_ibdbarrier_t;


GASNETI_INLINE(gasnete_ibdbarrier_send)
void gasnete_ibdbarrier_send(gasnete_coll_ibdbarrier_t *barrier_data,
                             int numsteps, unsigned int state,
                             gex_AM_Arg_t value, gex_AM_Arg_t flags) {
  GASNET_BEGIN_FUNCTION(); // TODO-EX: eliminate this?
  unsigned int step = state >> 1;
  gasnete_coll_rmdbarrier_inbox_t *payload;
  int i;

  /* Use the upper half (padding) an "other phase" inbox as an in-segment temporary.
   * This has sufficient lifetime for bulk and sufficient alignment for non-bulk.
   * Use of opposite phase prevents cacheline contention with arrivals.
   */
  const unsigned int stride = GASNETE_RDMABARRIER_INBOX_SZ / sizeof(gasnete_coll_rmdbarrier_inbox_t);
  payload = (stride/2) + GASNETE_RDMABARRIER_INBOX(barrier_data, (state^1));
  payload->value  = value;
  payload->flags  = flags;
  payload->flags2 = ~flags;
  payload->value2 = ~value;

  /* TODO:
   *   Reduce latency by pre-computing sr_desc and rkey_index at init time
   *   AND/OR providing a specialized alternative to gasnetc_rdma_put()
   */
  gasneti_assert(state < barrier_data->barrier_goal);

  for (i = 0; i < numsteps; ++i, state += 2, step += 1) {
    const gex_Rank_t jobrank = barrier_data->barrier_peers[step].jobrank;
    void * const addr = GASNETE_RDMABARRIER_INBOX_REMOTE(barrier_data, step, state);
#if GASNET_PSHM
    if (gasneti_pshm_jobrank_in_supernode(jobrank)) {
      *(volatile gasnete_coll_rmdbarrier_inbox_t *)addr = *payload;
    } else
#endif
    (void) gasnetc_rdma_put(gasneti_THUNK_TM, jobrank, (void*)payload, addr, sizeof(*payload), 0,
                            NULL, NULL, NULL, NULL GASNETI_THREAD_PASS);
  }
}

#if GASNETI_PSHM_BARRIER_HIER
static int gasnete_ibdbarrier_kick_pshm(gasnete_coll_team_t team) {
  gasnete_coll_ibdbarrier_t *barrier_data = team->barrier_data;
  int done = (barrier_data->barrier_state > 1);

  if (!done && !gasnete_rmdbarrier_trylock(&barrier_data->barrier_lock)) {
    const int state = barrier_data->barrier_state;
    done = (state > 1);
    if (!done) {
      PSHM_BDATA_DECL(pshm_bdata, barrier_data->barrier_pshm);
      if (gasnete_pshmbarrier_kick(pshm_bdata)) {
        const int value = pshm_bdata->shared->value;
        const int flags = pshm_bdata->shared->flags;
        barrier_data->barrier_value = value;
        barrier_data->barrier_flags = flags;
        gasneti_sync_writes();
        barrier_data->barrier_state = state + 2;
        gasnete_rmdbarrier_unlock(&barrier_data->barrier_lock); /* Cannot send while holding HSL */
        if (barrier_data->barrier_size && !barrier_data->barrier_passive) {
          gasnete_ibdbarrier_send(barrier_data, 1, state+2, value, flags);
        } else {
          gasnete_barrier_pf_disable(team);
        }
        return 1;
      }
    }
    gasnete_rmdbarrier_unlock(&barrier_data->barrier_lock);
  }

  return done;
}
#endif

void gasnete_ibdbarrier_kick(gasnete_coll_team_t team) {
  gasnete_coll_ibdbarrier_t *barrier_data = team->barrier_data;
  gasnete_coll_rmdbarrier_inbox_t *inbox;
  int numsteps = 0;
  int state, new_state;
  int flags, value;

  /* early unlocked read: */
  state = barrier_data->barrier_state;
  if (state >= barrier_data->barrier_goal)
    return; /* nothing to do */

  gasneti_assert(team->total_ranks > 1); /* singleton should have matched (state >= goal), above */

#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->barrier_pshm) {
    /* Cannot begin to probe until local notify is complete */
    if (!gasnete_ibdbarrier_kick_pshm(team)) return;
  }
#endif

  if (gasnete_rmdbarrier_trylock(&barrier_data->barrier_lock))
    return; /* another thread is currently in kick */

  /* reread w/ lock held and/or because kick_pshm may have advanced it */
  state = barrier_data->barrier_state;

#if GASNETI_PSHM_BARRIER_HIER
  if_pf (state < 2) { /* local notify has not completed */
    gasnete_rmdbarrier_unlock(&barrier_data->barrier_lock);
    return;
  } else if (barrier_data->barrier_passive) {
    gasnete_barrier_pf_disable(team);
    gasnete_rmdbarrier_unlock(&barrier_data->barrier_lock);
    return;
  }
  gasneti_assert(!barrier_data->barrier_passive);
#endif

#if GASNETI_THREADS
  if_pf (state < 4) {/* need to pick up value/flags from notify */
    gasneti_sync_reads(); /* value/flags were written by the non-locked notify */
  }
#endif

  value = barrier_data->barrier_value;
  flags = barrier_data->barrier_flags;

  /* process all consecutive steps which have arrived since we last ran */
  inbox = GASNETE_RDMABARRIER_INBOX(barrier_data, state);
  for (new_state = state; new_state < barrier_data->barrier_goal && gasnete_rmdbarrier_poll(inbox); new_state+=2) {
    const int step_value = inbox->value;
    const int step_flags = inbox->flags;

    gasneti_assert(inbox->value2 == ~step_value);
    gasneti_assert(inbox->flags2 == ~step_flags);

    /* "reset" the inbox
     *
     * We need to protect against "late arrivals", which are bytes written by the NIC
     * after we reset.  These late arrivals are possible if we've passed the _poll
     * check "too early" because one or more bytes already contained the proper value
     * by chance.  The risk is that the late arrivial byte(s) might match their peer
     * (e.g. value vs value2) BEFORE the next barrier leads to an actual arrival.
     * So, we must ensure that every (non-padding) byte is changed by the reset.
     */
    {
  #if SIZEOF_INT == 8
      unsigned int mask = 0x0101010101010101;
  #else
      unsigned int mask = 0x01010101;
  #endif
      inbox->value = inbox->value2 = step_value ^ mask;
      inbox->flags = inbox->flags2 = step_flags ^ mask;
    }

    if ((flags | step_flags) & GASNET_BARRIERFLAG_MISMATCH) {
      flags = GASNET_BARRIERFLAG_MISMATCH; 
    } else if (flags & GASNET_BARRIERFLAG_ANONYMOUS) {
      flags = step_flags; 
      value = step_value; 
    } else if (!(step_flags & GASNET_BARRIERFLAG_ANONYMOUS) && (step_value != value)) {
      flags = GASNET_BARRIERFLAG_MISMATCH; 
    }

    ++numsteps;
    inbox = GASNETE_RDMABARRIER_INBOX_NEXT(inbox);
  }

  if (numsteps) { /* completed one or more steps */
    barrier_data->barrier_flags = flags; 
    barrier_data->barrier_value = value; 

    if (new_state >= barrier_data->barrier_goal) { /* We got the last recv - barrier locally complete */
      gasnete_barrier_pf_disable(team);
      gasneti_sync_writes(); /* flush state before the write to barrier_state below */
      numsteps -= 1; /* no send at last step */
    } 
    /* notify all threads of the step increase - 
       this may allow other local threads to proceed on the barrier and even indicate
       barrier completion while we overlap outgoing notifications to other nodes
    */
    barrier_data->barrier_state = new_state;
  } 

  gasnete_rmdbarrier_unlock(&barrier_data->barrier_lock);

  if (numsteps) { /* need to issue one or more Puts */
    gasnete_ibdbarrier_send(barrier_data, numsteps, state+2, value, flags);
  }
}

static void gasnete_ibdbarrier_notify(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_ibdbarrier_t *barrier_data = team->barrier_data;
  int state = 2 + ((barrier_data->barrier_state & 1) ^ 1); /* enter new phase */
  int do_send = 1;
  int want_pf = 1;

  GASNETE_SPLITSTATE_NOTIFY_ENTER(team);

#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->barrier_pshm) {
    PSHM_BDATA_DECL(pshm_bdata, barrier_data->barrier_pshm);
    if (gasnete_pshmbarrier_notify_inner(pshm_bdata, id, flags)) {
      id = pshm_bdata->shared->value;
      flags = pshm_bdata->shared->flags;
      want_pf = do_send = !barrier_data->barrier_passive;
    } else {
      do_send = 0;
      state -= 2;
    }
  }
#endif

  barrier_data->barrier_value = id;
  barrier_data->barrier_flags = flags;

  gasneti_sync_writes();
  barrier_data->barrier_state = state;

  if (do_send) gasnete_ibdbarrier_send(barrier_data, 1, state, id, flags);
  if (want_pf) gasnete_barrier_pf_enable(team);

  /*  update state */
  gasneti_sync_writes(); /* ensure all state changes committed before return */
}

/* Notify specialized to one (super)node case (reduced branches in BOTH variants) */
static void gasnete_ibdbarrier_notify_singleton(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_ibdbarrier_t *barrier_data = team->barrier_data;
#if GASNETI_PSHM_BARRIER_HIER
  int state = 2;
#endif

  GASNETE_SPLITSTATE_NOTIFY_ENTER(team);

#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->barrier_pshm) {
    PSHM_BDATA_DECL(pshm_bdata, barrier_data->barrier_pshm);
    if (gasnete_pshmbarrier_notify_inner(pshm_bdata, id, flags)) {
      id = pshm_bdata->shared->value;
      flags = pshm_bdata->shared->flags;
    } else {
      state = 0;
    }
  }
#endif

  barrier_data->barrier_value = id;
  barrier_data->barrier_flags = flags;

#if GASNETI_PSHM_BARRIER_HIER
  gasneti_sync_writes();
  barrier_data->barrier_state = state;
  if (!state) gasnete_barrier_pf_enable(team);
#endif

  /*  update state */
  gasneti_sync_writes(); /* ensure all state changes committed before return */
}

static int gasnete_ibdbarrier_wait(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_ibdbarrier_t *barrier_data = team->barrier_data;
#if GASNETI_PSHM_BARRIER_HIER
  PSHM_BDATA_DECL(pshm_bdata, barrier_data->barrier_pshm);
#endif
  int retval = GASNET_OK;

  gasneti_sync_reads(); /* ensure we read correct state */
  GASNETE_SPLITSTATE_WAIT_LEAVE(team);

#if GASNETI_PSHM_BARRIER_HIER
  if (pshm_bdata) {
    const int passive_shift = barrier_data->barrier_passive;
    gasneti_polluntil(gasnete_ibdbarrier_kick_pshm(team));
    retval = gasnete_pshmbarrier_wait_inner(pshm_bdata, id, flags, passive_shift);
    if (passive_shift) {
      /* Once the active peer signals done, we can return */
      barrier_data->barrier_value = pshm_bdata->shared->value;
      barrier_data->barrier_flags = pshm_bdata->shared->flags;
      gasneti_sync_writes(); /* ensure all state changes committed before return */
      return retval;
    }
  }
#endif

  if (barrier_data->barrier_state >= barrier_data->barrier_goal) {
    /* completed asynchronously before wait (via progressfns or try) */
    GASNETI_TRACE_EVENT_TIME(B,BARRIER_ASYNC_COMPLETION,GASNETI_TICKS_NOW_IFENABLED(B)-gasnete_barrier_notifytime);
  } else {
    /* kick once, and if still necessary, wait for a response */
    gasnete_ibdbarrier_kick(team);
    /* cannot BLOCKUNTIL since progess may occur on non-AM events */
    while (barrier_data->barrier_state < barrier_data->barrier_goal) {
      GASNETI_WAITHOOK();
      GASNETI_SAFE(gasneti_AMPoll());
      gasnete_ibdbarrier_kick(team);
    }
  }
  gasneti_sync_reads(); /* ensure correct barrier_flags will be read */

  /* determine return value */
  if_pf (barrier_data->barrier_flags & GASNET_BARRIERFLAG_MISMATCH) {
    retval = GASNET_ERR_BARRIER_MISMATCH;
  } else
  if_pf(/* try/wait value must match consensus value, if both are present */
        !((flags|barrier_data->barrier_flags) & GASNET_BARRIERFLAG_ANONYMOUS) &&
	 ((gex_AM_Arg_t)id != barrier_data->barrier_value)) {
    retval = GASNET_ERR_BARRIER_MISMATCH;
  }

  /*  update state */
#if GASNETI_PSHM_BARRIER_HIER
  if (pshm_bdata) {
    /* Signal any passive peers w/ the final result */
    pshm_bdata->shared->value = barrier_data->barrier_value;
    pshm_bdata->shared->flags = barrier_data->barrier_flags;
    PSHM_BSTATE_SIGNAL(pshm_bdata, retval, pshm_bdata->private.two_to_phase << 2); /* includes a WMB */
    gasneti_assert(!barrier_data->barrier_passive);
  } else
#endif
  gasneti_sync_writes(); /* ensure all state changes committed before return */

  return retval;
}

static int gasnete_ibdbarrier_try(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_ibdbarrier_t *barrier_data = team->barrier_data;
  gasneti_sync_reads(); /* ensure we read correct state */

  GASNETE_SPLITSTATE_TRY(team);

  GASNETI_SAFE(gasneti_AMPoll());

#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->barrier_pshm) {
    const int passive_shift = barrier_data->barrier_passive;
    if (!gasnete_ibdbarrier_kick_pshm(team) ||
        !gasnete_pshmbarrier_try_inner(barrier_data->barrier_pshm, passive_shift))
      return GASNET_ERR_NOT_READY;
    if (passive_shift)
      return gasnete_ibdbarrier_wait(team, id, flags);
  }
  if (!barrier_data->barrier_passive)
#endif
    gasnete_ibdbarrier_kick(team);

  if (barrier_data->barrier_state >= barrier_data->barrier_goal)
    return gasnete_ibdbarrier_wait(team, id, flags);
  else return GASNET_ERR_NOT_READY;
}

static int gasnete_ibdbarrier_result(gasnete_coll_team_t team, int *id) {
  gasneti_sync_reads();
  GASNETE_SPLITSTATE_RESULT(team);

  { const gasnete_coll_ibdbarrier_t * const barrier_data = team->barrier_data;
    *id = barrier_data->barrier_value;
    return (GASNET_BARRIERFLAG_ANONYMOUS & barrier_data->barrier_flags);
  }
}

void gasnete_ibdbarrier_kick_team_all(void) {
  gasnete_ibdbarrier_kick(GASNET_TEAM_ALL);
}

static void gasnete_ibdbarrier_init(gasnete_coll_team_t team) {
  gasnete_coll_ibdbarrier_t *barrier_data;
  int steps;
  int total_ranks = team->total_ranks;
  int myrank = team->myrank;
  gasnete_coll_peer_list_t *peers = &team->peers;

#if GASNETI_PSHM_BARRIER_HIER
  PSHM_BDATA_DECL(pshm_bdata, gasnete_pshmbarrier_init_hier(team, &total_ranks, &myrank, &peers));
#endif

  barrier_data = gasneti_malloc_aligned(GASNETI_CACHE_LINE_BYTES, sizeof(gasnete_coll_ibdbarrier_t));
  gasneti_leak_aligned(barrier_data);
  memset(barrier_data, 0, sizeof(gasnete_coll_ibdbarrier_t));
  team->barrier_data = barrier_data;

#if GASNETI_PSHM_BARRIER_HIER
  if (pshm_bdata) {
    barrier_data->barrier_passive = (pshm_bdata->private.rank != 0) ? 2 : 0; /* precompute shift */
    barrier_data->barrier_pshm = pshm_bdata;
  }
#endif

  gasneti_assert(team == GASNET_TEAM_ALL); /* TODO: deal w/ in-segment allocation */

  gasnete_rmdbarrier_lock_init(&barrier_data->barrier_lock);

  /* determine barrier size (number of steps) */
  steps = peers->num;
  barrier_data->barrier_size = steps;
  barrier_data->barrier_goal = (1+steps) << 1;

  if (steps) {
    int step;

    gasneti_assert(gasnete_rdmabarrier_auxseg);
    gasneti_assert_always(2 * sizeof(gasnete_coll_rmdbarrier_inbox_t) <= GASNETE_RDMABARRIER_INBOX_SZ);
    barrier_data->barrier_inbox = gasnete_rdmabarrier_auxseg[gasneti_mynode].addr;

    barrier_data->barrier_peers = gasneti_malloc((1+steps) * sizeof(* barrier_data->barrier_peers));
    gasneti_leak(barrier_data->barrier_peers);
  
    for (step = 0; step < steps; ++step) {
      gex_Rank_t jobrank = peers->fwd[step]; // is always a jobrank
      void *addr = gasnete_rdmabarrier_auxseg[jobrank].addr;
      barrier_data->barrier_peers[1+step].jobrank = jobrank;
    #if GASNET_PSHM
      if (gasneti_pshm_jobrank_in_supernode(jobrank)) {
        barrier_data->barrier_peers[1+step].addr = (uintptr_t)gasneti_pshm_jobrank_addr2local(jobrank, addr);
      } else
    #endif
      barrier_data->barrier_peers[1+step].addr = (uintptr_t)addr;
    }
  } else {
    barrier_data->barrier_state = barrier_data->barrier_goal;
  }

  gasneti_free(gasnete_rdmabarrier_auxseg);

#if GASNETI_PSHM_BARRIER_HIER
  if (pshm_bdata && (pshm_bdata->shared->size == 1)) {
    /* With singleton proc on local supernode we can short-cut the PSHM code.
     * This does not require alteration of the barrier_peers[] contructed above
     */
    gasnete_pshmbarrier_fini_inner(pshm_bdata);
    barrier_data->barrier_pshm = NULL;
  }
#endif

  team->barrier_notify = steps ? &gasnete_ibdbarrier_notify : &gasnete_ibdbarrier_notify_singleton;
  team->barrier_wait =   &gasnete_ibdbarrier_wait;
  team->barrier_try =    &gasnete_ibdbarrier_try;
  team->barrier_result = &gasnete_ibdbarrier_result;
  team->barrier_pf =     (team == GASNET_TEAM_ALL) ? &gasnete_ibdbarrier_kick_team_all : NULL;
}

/* ------------------------------------------------------------------------------------ */
/*
  Vector, Indexed & Strided:
  =========================
*/

/* use reference implementation of scatter/gather and strided */
#include "gasnet_refvis.h"

/* ------------------------------------------------------------------------------------ */
/*
  Collectives:
  ============
*/

/* use reference implementation of collectives */
#include "gasnet_refcoll.h"

/* ------------------------------------------------------------------------------------ */
/*
  Remote Atomics:
  ==============
*/

/* use reference implementation of remote atomics */
#include "gasnet_refratomic.h"

/* ------------------------------------------------------------------------------------ */
/*
  Handlers:
  =========
*/
static gex_AM_Entry_t const gasnete_handlers[] = {
  #ifdef GASNETE_REFBARRIER_HANDLERS
    GASNETE_REFBARRIER_HANDLERS(),
  #endif
  #ifdef GASNETE_REFVIS_HANDLERS
    GASNETE_REFVIS_HANDLERS()
  #endif
  #ifdef GASNETE_REFCOLL_HANDLERS
    GASNETE_REFCOLL_HANDLERS()
  #endif
  #ifdef GASNETE_AMREF_HANDLERS
    GASNETE_AMREF_HANDLERS()
  #endif
  #ifdef GASNETE_AMRATOMIC_HANDLERS
    GASNETE_AMRATOMIC_HANDLERS()
  #endif

  /* ptr-width independent handlers */

  /* ptr-width dependent handlers */

  GASNETI_HANDLER_EOT
};

extern gex_AM_Entry_t const *gasnete_get_handlertable(void) {
  return gasnete_handlers;
}
/* ------------------------------------------------------------------------------------ */

