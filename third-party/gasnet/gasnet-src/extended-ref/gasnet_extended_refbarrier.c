/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/gasnet_extended_refbarrier.c $
 * Description: Reference implemetation of GASNet Barrier, using Active Messages
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef GASNETI_GASNET_EXTENDED_REFBARRIER_C
  #error This file not meant to be compiled directly - included by gasnet_extended.c
#endif

#include <limits.h>

/* ------------------------------------------------------------------------------------ */
/* state shared between barrier implementations */

/* Flags bits that are valid for use by the client */
#define GASNETE_BARRIERFLAGS_CLIENT_ALL \
    (GASNET_BARRIERFLAG_MISMATCH | GASNET_BARRIERFLAG_ANONYMOUS | GASNET_BARRIERFLAG_UNNAMED)
#define GASNETE_BARRIERFLAGS_CLIENT_COLL GASNETE_BARRIERFLAGS_CLIENT_ALL

#ifndef GASNETE_BARRIER_DEFAULT
/* conduit plugin for default barrier mechanism */
#define GASNETE_BARRIER_DEFAULT "DISSEM"
#endif

#if GASNETI_STATS_OR_TRACE
  gasneti_tick_t gasnete_barrier_notifytime; /* for statistical purposes */ 
#endif

static gasnet_seginfo_t *gasnete_rdmabarrier_auxseg = NULL;

/*eventually this has to be changed so that all outstanding barriers are polled*/
/*keep a list of active barriers across all the teams. The poller walks the list and then kicks
 each one of them*/
/*XXX: for now only team all registers their pollers*/
gasneti_progressfn_t gasnete_barrier_pf= NULL;

GASNETI_INLINE(gasnete_barrier_pf_enable)
void gasnete_barrier_pf_enable(gasnete_coll_team_t team) {
  if (team->barrier_pf) {
    gasneti_assert(team == GASNET_TEAM_ALL);
    gasnete_barrier_pf = team->barrier_pf; /* Will need to QUEUE, not assign */
    GASNETI_PROGRESSFNS_ENABLE(gasneti_pf_barrier,BOOLEAN);
  }
}

GASNETI_INLINE(gasnete_barrier_pf_disable)
void gasnete_barrier_pf_disable(gasnete_coll_team_t team) {
  if (team->barrier_pf) {
    gasneti_assert(team == GASNET_TEAM_ALL);
    GASNETI_PROGRESSFNS_DISABLE(gasneti_pf_barrier,BOOLEAN);
  }
}

/* ------------------------------------------------------------------------------------ */
/* Code to check for proper nesting of barrier calls - compiles away in NDBUG builds */

#if GASNET_DEBUG
  #define GASNETE_SPLITSTATE_SET(_team,_state) do { \
    (_team)->barrier_splitstate = (_state);         \
    gasneti_sync_writes(); /* OK if redundant */    \
  } while(0)
  #define GASNETE_SPLITSTATE_CHECK(_team,_state,_msg) do {  \
    gasneti_sync_reads(); /* OK if redundant */             \
    if_pf ((_team)->barrier_splitstate == (_state)) {       \
      gasneti_fatalerror(_msg);                             \
    }                                                       \
  } while(0)
#else
  #define GASNETE_SPLITSTATE_SET(_team,_state) ((void)0)
  #define GASNETE_SPLITSTATE_CHECK(_team,_state,_msg) ((void)0)
#endif
#define GASNETE_SPLITSTATE_ENTER(_team) \
    GASNETE_SPLITSTATE_SET(_team,INSIDE_BARRIER)
#define GASNETE_SPLITSTATE_LEAVE(_team) \
    GASNETE_SPLITSTATE_SET(_team,OUTSIDE_BARRIER)
#define GASNETE_SPLITSTATE_NOTIFY(_team) \
    GASNETE_SPLITSTATE_CHECK(team,INSIDE_BARRIER,"gasnet_barrier_notify() called twice in a row")
#define GASNETE_SPLITSTATE_WAIT(_team) \
    GASNETE_SPLITSTATE_CHECK(team,OUTSIDE_BARRIER,"gasnet_barrier_wait() called without a matching notify")
#define GASNETE_SPLITSTATE_TRY(_team) \
    GASNETE_SPLITSTATE_CHECK(team,OUTSIDE_BARRIER,"gasnet_barrier_try() called without a matching notify")
#define GASNETE_SPLITSTATE_BARRIER(_team) \
    GASNETE_SPLITSTATE_CHECK(team,INSIDE_BARRIER,"gasnet_barrier() called between notify and wait/try")
#define GASNETE_SPLITSTATE_RESULT(_team) \
    GASNETE_SPLITSTATE_CHECK(team,INSIDE_BARRIER,"gasnet_barrier_result() called between notify and wait/try")

#define GASNETE_SPLITSTATE_NOTIFY_ENTER(_team) do { \
    GASNETE_SPLITSTATE_NOTIFY(_team);               \
    GASNETE_SPLITSTATE_ENTER(_team);                \
  } while(0)
#define GASNETE_SPLITSTATE_WAIT_LEAVE(_team) do { \
    GASNETE_SPLITSTATE_WAIT(_team);               \
    GASNETE_SPLITSTATE_LEAVE(_team);              \
  } while(0)

/* ------------------------------------------------------------------------------------ */
/* 
 * GASNETI_PSHM_BARRIER: do we build the shared-memory barrier
 * GASNETI_PSHM_BARRIER_HIER: for use alone (0) or in a heirarchical barrier (1)
 */
#if !GASNET_PSHM
  /* No PSHM support: GASNETI_PSHM_BARRIER == GASNETI_PSHM_BARRIER_HIER == 0 */
  #if GASNETI_PSHM_BARRIER_HIER
    #error "GASNETI_PSHM_BARRIER_HIER non-zero but not configured for PSHM support"
  #endif
  #undef GASNETI_PSHM_BARRIER_HIER
  #define GASNETI_PSHM_BARRIER_HIER 0
  #define GASNETI_PSHM_BARRIER 0
#elif defined(GASNET_CONDUIT_SMP)
  /* PSHM+SMP: GASNETI_PSHM_BARRIER == 1, GASNETI_PSHM_BARRIER_HIER == 0
   * even if user set GASNETI_PSHM_BARRIER_HIER explicitly */
  #undef GASNETI_PSHM_BARRIER_HIER
  #define GASNETI_PSHM_BARRIER_HIER 0
  #define GASNETI_PSHM_BARRIER 1
#else
  /* PSHM+NET: GASNETI_PSHM_BARRIER_HIER == 1 unless set by user
   * GASNETI_PSHM_BARRIER always follows GASNETI_PSHM_BARRIER_HIER
   */
  #ifndef GASNETI_PSHM_BARRIER_HIER /* Preserve user's setting, if any */
    #define GASNETI_PSHM_BARRIER_HIER 1
  #endif
  #define GASNETI_PSHM_BARRIER GASNETI_PSHM_BARRIER_HIER
#endif


#if GASNETI_PSHM_BARRIER
/* ------------------------------------------------------------------------------------ */
/* the shared memory intra-supernode implementation of barrier */

/* This is a shared-memory barrier.  As such the gasneti_pshm_barrier_t must exist
 * within either the GASNet segments (Aux or Client portions are both possible) or
 * within the N+1st shared mmap() which contains the AMPSHM data structures.  In the
 * case of TEAM_ALL this memory comes from that N+1st mmap.  To get full team support
 * in this barrier implementation, we'll probably need to carve the memory out of the
 * team's scratch space.  I am not sure if we can hold on to a piece of the scratch
 * space indefinately (I doubt it) or whether is will need to be recycled back into
 * to the pool and associate a collective op with each barrier.  Another option
 * would be to allocate some of the space left over at the end of that N+1st mmap.
 * This question of shared-space allocation is the only thing still blocking team
 * support (well, other than testing) for the PSHM barrier code.     -PHH 2010.03.16
 */

#ifdef GASNETE_PSHM_BARR_U64
  /* Keep the existing defn */
#elif PLATFORM_ARCH_64 && !GASNETI_ATOMIC64_NOT_SIGNALSAFE && (SIZEOF_INT == 4) && \
    (((GASNET_BARRIERFLAG_MISMATCH|GASNET_BARRIERFLAG_ANONYMOUS) & 0xffff) == \
                   (GASNET_BARRIERFLAG_MISMATCH|GASNET_BARRIERFLAG_ANONYMOUS))
  /* We can fit everything in a 64-bit read/write w/o fear of word-tearing. */
  #define GASNETE_PSHM_BARR_U64 1
  #define GASNETE_PSHM_BARR_PHASE_SHIFT 16
  #if PLATFORM_ARCH_LITTLE_ENDIAN
    #define GASNETE_PSHM_BARR_PACK(_value, _flags, _phase) \
                GASNETI_MAKEWORD((_flags | (_phase << GASNETE_PSHM_BARR_PHASE_SHIFT)), _value)
    #define GASNETE_PSHM_BARR_FLAGS(_u64)    GASNETI_HIWORD(_u64) /* and phase too */
    #define GASNETE_PSHM_BARR_VALUE(_u64)    GASNETI_LOWORD(_u64)
  #else
    #define GASNETE_PSHM_BARR_PACK(_value, _flags, _phase) \
                GASNETI_MAKEWORD(_value, (_flags | (_phase << GASNETE_PSHM_BARR_PHASE_SHIFT)))
    #define GASNETE_PSHM_BARR_FLAGS(_u64)    GASNETI_LOWORD(_u64) /* and phase too */
    #define GASNETE_PSHM_BARR_VALUE(_u64)    GASNETI_HIWORD(_u64)
  #endif
#else
  #define GASNETE_PSHM_BARR_U64 0
#endif

typedef struct gasnete_coll_pshmbarrier_s {
  struct {
    struct gasneti_pshm_barrier_node *mynode;
    struct gasnete_pshmbarrier_children {
        struct gasneti_pshm_barrier_node *node;
      #if GASNETE_PSHM_BARR_U64
        uint64_t u64;
      #else
        int phase;
      #endif
    } *children;
    int rank, num_children;
    int remain, value, flags; /* Partial state between notify and completion */
    int volatile two_to_phase; /* Local var alternates between 2^0 and 2^1 */
  } private;
  gasneti_pshm_barrier_t *shared;
} gasnete_pshmbarrier_data_t;

#define PSHM_BDATA_DECL(_name, _value) \
      gasnete_pshmbarrier_data_t * const _name = (_value) /* no semicolon */

/* We encode the done bits and the result into a single word
 * The hierarhical case needs space for 4 done bits; pure-SMP needs only 2.
 */
#if GASNETI_PSHM_BARRIER_HIER
  #define PSHM_BSTATE_DONE_BITS 4
#else
  #define PSHM_BSTATE_DONE_BITS 2
#endif
#define PSHM_BSTATE_TO_RESULT(_state) ((_state) >> PSHM_BSTATE_DONE_BITS)
#define PSHM_BSTATE_SIGNAL(_bdata, _result, _two_to_phase) do {                \
    const int _tmp_result = (_result);                                         \
    const gasneti_atomic_sval_t _state = (_tmp_result << PSHM_BSTATE_DONE_BITS) | (_two_to_phase);\
    gasneti_assert_int(PSHM_BSTATE_TO_RESULT(_state) ,==, _tmp_result);        \
    gasneti_atomic_set(&(_bdata)->shared->state, _state, GASNETI_ATOMIC_REL);  \
  } while(0)


GASNETI_INLINE(gasnete_pshmbarrier_arrive)
void gasnete_pshmbarrier_arrive(gasnete_pshmbarrier_data_t * const pshm_bdata, int value, int flags, int two_to_phase) {
  /* Signal my own arrival */
#if GASNETE_PSHM_BARR_U64
  gasneti_local_wmb();
  pshm_bdata->private.mynode->u.u64 = GASNETE_PSHM_BARR_PACK(value, flags, two_to_phase);
#else
  pshm_bdata->private.mynode->u.wmb.value = value;
  pshm_bdata->private.mynode->u.wmb.flags = flags;
  gasneti_local_wmb();
  pshm_bdata->private.mynode->u.wmb.phase = two_to_phase;
#endif

  /* Root (rank == 0) must publish the results and signal the barrier w/ phase and result */
  if (! pshm_bdata->private.rank) {
    gasneti_pshm_barrier_t * const shared_data = pshm_bdata->shared;
    int result;
    shared_data->value = value;
    shared_data->flags = flags;
    result = (flags & GASNET_BARRIERFLAG_MISMATCH) ? GASNET_ERR_BARRIER_MISMATCH : GASNET_OK;
    PSHM_BSTATE_SIGNAL(pshm_bdata, result, two_to_phase);
  }
}

/* TODO: to inline or not? */
static
int gasnete_pshmbarrier_kick(gasnete_pshmbarrier_data_t * const pshm_bdata) {
    /* The algorithm:
     * 
     * This is basically a tree-based reduction, except that by default we will
     * devolve to a "flat" tree with node==0 as the parent of all others.
     *
     * 1) Each node (carefully) polls for the phase fields of children, if any,
     *    applying the name-matching logic to values as they becomes available.
     * 2) Each node stores the value/flags resulting from the application of the
     *    matching logic and its own value/flags in its corresponding tree node.
     * 
     * The "careful" polling is derived from the barrier Dan Bonachea developed
     * for the Titanium runtime (in which the barrier was neither split-phase
     * nor named).
     * 
     * TODO: For CPU overcommit we may want a different algorithm rather than
     *       just use of GASNETI_WAITHOOK() which is dependent on the setting
     *       of the waitmode.  Even if we stick with this same algortihm, we
     *       could consider yielding in the overcommit case independent of
     *       the waitmode setting.
     *
     * TODO: For large enough core count our prefetch of all the data at once
     *       could potentially lead to conflict misses in cache.  Some sort of
     *       segmenting of our fetches could help if we reach that point.
     *
     * TODO: Current data layout places the nodes together on a page which is
     *       allocated and first touched by the master (reader).  The results
     *       of trials with the data distributed to have affinity of each node
     *       correspond to its writer were mixed.  On PPC and SPARC platforms
     *       one sees a 50% (POWER7) to 400% (SPARC T4) slow-down when the
     *       data has affinity to the writter.  That is enough to avoid making
     *       any change to writer-affinity at the present time.
     *       However, on AMD and Intel CPUs the results need more study:
     *       + On an SGI UV 1000 platform the use of writer-affinity eliminated
     *         an anomalous performance characteristic seen on runs which use
     *         3 or 4 blades, but otherwise slows the performance slightly.
     *       + On a dual-socket Intel Sandy Bridge node, writer-affinity gave a
     *         40% to 50% speed-up when using both sockets - no change otherwise.
     *       + On a dual-socket AMD Magny-Cours node, writer-affinity gave a 10%
     *         to 20% slow-down.
     *       NOTE: These results were taken ONLY with the linear case.
     *
     * TODO: Measurements above suggest that for some systems (PPC and SPARC
     *       in particular, but also some x86-64) that there is a benefit to
     *       spinning on memory local to the reader (as opposed to the writer).
     *       This is automatically the case when radix==0, but for the case of
     *       a tree with more than the trivial 2 levels the inititialization
     *       code could (and therefore perhaps should) layout the nodes such
     *       that every parent reads only from memory with local affinity.
     */

  int n = pshm_bdata->private.remain;
  if (n) {
      gasneti_pshm_barrier_t * const shared_data = pshm_bdata->shared;
      struct gasneti_pshm_barrier_node * const nodes = shared_data->node;
      const int two_to_phase = pshm_bdata->private.two_to_phase;
      int value = pshm_bdata->private.value;
      int flags = pshm_bdata->private.flags;

    #if GASNETE_PSHM_BARR_U64
      const uint64_t goal = GASNETE_PSHM_BARR_PACK(0, 0, two_to_phase);
    #endif
      struct gasnete_pshmbarrier_children * const children = pshm_bdata->private.children;

      /* Poll for children's phase fields to indicate arrival, processing in batches */
      {
        int arrivals = 0;
        int i;

        /* 1. Fetch any nodes which are still outstanding */
        for (i = 0; i < n; ++i) {
        #if GASNETE_PSHM_BARR_U64
          children[i].u64 = children[i].node->u.u64;
        #else
          children[i].phase = children[i].node->u.wmb.phase;
        #endif
        }

        /* 2. Scan the phases, collecting completed entries at the end of the list */
        for (i = 0; i < n; /*empty*/) {
        #if GASNETE_PSHM_BARR_U64
          const int ready = (goal & children[i].u64) != 0; /* goal is a single bit */
        #else
          const int ready = two_to_phase == children[i].phase;
        #endif
          if (ready) {
            struct gasnete_pshmbarrier_children tmp = children[--n];
            children[n] = children[i];
            children[i] = tmp;
            ++arrivals;
          } else {
            /* We don't reread until the next kick */
            ++i;
          }
        }

        /* 3. Apply name-matching logic to recent arrivals, if any */
        if (arrivals) {
        #if !GASNETE_PSHM_BARR_U64
          gasneti_local_rmb();
        #endif
          for (i = 0; i < arrivals; ++i) {
          #if GASNETE_PSHM_BARR_U64
            const uint64_t u64 = children[n+i].u64;
            const int other_value = GASNETE_PSHM_BARR_VALUE(u64);
            const int other_flags = GASNETE_PSHM_BARR_FLAGS(u64); /* No need to mask */
          #else
            const struct gasneti_pshm_barrier_node * node = children[n+i].node;
            const int other_value = node->u.wmb.value;
            const int other_flags = node->u.wmb.flags;
            gasneti_assert_int(node->u.wmb.phase ,==, two_to_phase);
          #endif

            if ((flags | other_flags) & GASNET_BARRIERFLAG_MISMATCH) {
              flags = GASNET_BARRIERFLAG_MISMATCH;
            } else if (flags & GASNET_BARRIERFLAG_ANONYMOUS) {
              flags = other_flags;
              value = other_value;
            } else if (!(other_flags & GASNET_BARRIERFLAG_ANONYMOUS) && (other_value != value)) {
              flags = GASNET_BARRIERFLAG_MISMATCH;
            }
          }
        }
      }

      pshm_bdata->private.remain = n;
      if (n) { /* Not done - save state and return */
        pshm_bdata->private.value = value;
        pshm_bdata->private.flags = flags;
        return 0;
      }

      /* Can finally signal my own arrival */
      gasnete_pshmbarrier_arrive(pshm_bdata, value, flags, two_to_phase);
  }

  return 1;
}

/* Returns non-zero IFF barrier is "locally complete" == does NOT require further kicks to progress */
GASNETI_INLINE(gasnete_pshmbarrier_notify_inner)
int gasnete_pshmbarrier_notify_inner(gasnete_pshmbarrier_data_t * const pshm_bdata, int value, int flags) {
  /* Start a new phase */
  int two_to_phase = (pshm_bdata->private.two_to_phase ^= 3); /* alternates between 01 and 10 base-2 */

  int num_children = pshm_bdata->private.num_children;
  if (num_children) {
    pshm_bdata->private.remain = num_children;
    pshm_bdata->private.value = value;
    pshm_bdata->private.flags = flags;
    return gasnete_pshmbarrier_kick(pshm_bdata);
  } else {
    gasnete_pshmbarrier_arrive(pshm_bdata, value, flags, two_to_phase);
    return 1;
  }
}

GASNETI_INLINE(finish_pshm_barrier)
int finish_pshm_barrier(const gasnete_pshmbarrier_data_t * const pshm_bdata, int id, int flags, gasneti_atomic_sval_t state) {
  const struct gasneti_pshm_barrier_node * const mynode = pshm_bdata->private.mynode;
  const gasneti_pshm_barrier_t * const shared_data = pshm_bdata->shared;
  int ret = PSHM_BSTATE_TO_RESULT(state); /* default unless args mismatch those from notify */

  /* Check args for mismatch */
  if_pf(/* try/wait value must match consensus value, if both are present */
        !((flags|shared_data->flags) & GASNET_BARRIERFLAG_ANONYMOUS) &&
         (id != shared_data->value)) {
    ret = GASNET_ERR_BARRIER_MISMATCH; 
  }

  return ret;
}

/* Poll waiting for appropriate done bit in "state"
 * Returns GASNET_{OK,ERR_BARRIER_MISMATCH}
 */
GASNETI_INLINE(gasnete_pshmbarrier_wait_inner)
int gasnete_pshmbarrier_wait_inner(gasnete_pshmbarrier_data_t * const pshm_bdata, int id, int flags, int shift) {
  const gasneti_atomic_sval_t goal = pshm_bdata->private.two_to_phase << shift;
  gasneti_atomic_t * const state_p = &pshm_bdata->shared->state;
  gasneti_atomic_sval_t state;

  gasneti_polluntil((gasnete_pshmbarrier_kick(pshm_bdata),
                     (goal & (state = gasneti_atomic_read(state_p, 0)))));

  return finish_pshm_barrier(pshm_bdata, id, flags, state);
}

/* Test for appropriate done bit in "state"
 * Returns zero or non-zero (the state in pure-SMP case)
 */
GASNETI_INLINE(gasnete_pshmbarrier_try_inner)
gasneti_atomic_sval_t gasnete_pshmbarrier_try_inner(gasnete_pshmbarrier_data_t * const pshm_bdata, int shift) {
  const gasneti_atomic_sval_t goal = pshm_bdata->private.two_to_phase << shift;
  gasneti_atomic_t * const state_p = &pshm_bdata->shared->state;
  gasneti_atomic_sval_t state;

  gasnete_pshmbarrier_kick(pshm_bdata);
  state = gasneti_atomic_read(state_p, GASNETI_ATOMIC_ACQ);

#if !GASNETI_PSHM_BARRIER_HIER
  return (goal & state) ? state : 0;
#else
  return (goal & state);
#endif
}

/* Returns non-NULL on success
 * NULL return on failure might eventually come from a failed shared memory allocation.
 */
static gasnete_pshmbarrier_data_t *
gasnete_pshmbarrier_init_inner(gasnete_coll_team_t team) {
  gasnete_pshmbarrier_data_t *pshm_bdata = NULL;
  gasneti_pshm_barrier_t *shared_data = NULL;
  const int two_to_phase = 1; /* 2^0 */
  int i, radix;

  if (team == GASNET_TEAM_ALL) {
    shared_data = gasneti_pshm_barrier;
  } else {
    /* TODO: non-TEAM_ALL will need to allocate storage from shared space */
    return NULL;
  }

  if (shared_data) {
    const int size = team->supernode.node_count;
    const int rank = team->supernode.node_rank;

    pshm_bdata = gasneti_malloc(sizeof(gasnete_pshmbarrier_data_t));
    gasneti_leak(pshm_bdata);
    pshm_bdata->private.two_to_phase = two_to_phase;
    pshm_bdata->private.rank = rank;
    pshm_bdata->private.mynode = &shared_data->node[rank];

    /* GASNET_PSHM_BARRIER_RADIX
     *  If positive then the given value is the out-degree of the tree.
     *  If zero (default) then radix = size - 1, resulting in a "flat tree" (linear time)
     *  If negative then a tree is build with the processes in groups of size = -radix,
     *    and the first process per group is the parent of the others.  The rank==0
     *    process is the parent of the other group-representatives (in addition to
     *    being the parent of the others in its own group).
     */
    radix = gasneti_getenv_int_withdefault("GASNET_PSHM_BARRIER_RADIX", 0, 0);
    if (radix == 0) radix = size - 1;

    pshm_bdata->private.children = NULL;
    pshm_bdata->private.num_children = 0;
    pshm_bdata->private.remain = 0;

    if (size == 1) {
      /* Nothing to do */
    } else if (radix < 0) { /* Break into "cells" of size = -radix (e.g. cores/socket) */
      radix = -radix;
      if (rank == 0) {
        int last  = MIN(size, radix) - 1;
        int count = last + (size - 1) / radix;
        int j = 0;

        pshm_bdata->private.num_children = count;
        pshm_bdata->private.children = gasneti_malloc(count * sizeof(struct gasnete_pshmbarrier_children));
        for (i = 1; i <= last; ++i) {
          pshm_bdata->private.children[j++].node = &shared_data->node[i];
        }
        for (i = radix; i < size; i += radix) {
          pshm_bdata->private.children[j++].node = &shared_data->node[i];
        }
        gasneti_assert_int(j ,==, count);
      } else if ((rank % radix) == 0) {
        int last  = MIN(size, rank + radix) - 1;
        int count = MAX(0, last - rank);

        if (count) {
          pshm_bdata->private.num_children = count;
          pshm_bdata->private.children = gasneti_malloc(count * sizeof(struct gasnete_pshmbarrier_children));
          gasneti_leak(pshm_bdata->private.children);
          for (i = 0; i < count; ++i) {
            pshm_bdata->private.children[i].node = &shared_data->node[rank+1+i];
          }
        }
      }
    } else { /* Build an N-ary tree */
      int first = radix * rank + 1;
      int last  = MIN(size, first + radix) - 1;
      int count = MAX(0, 1 + last - first);

      if (count) {
        pshm_bdata->private.num_children = count;
        pshm_bdata->private.children = gasneti_malloc(count * sizeof(struct gasnete_pshmbarrier_children));
        gasneti_leak(pshm_bdata->private.children);
        for (i = 0; i < count; ++i) {
          pshm_bdata->private.children[i].node = &shared_data->node[first + i];
        }
      }
    }

    pshm_bdata->shared = shared_data;

    /* One node initializes shared data, while others wait */
    if (!rank) {
      /* Values used to detect arrivals at Notify */
      for (i=0; i < size; i++) {
      #if GASNETE_PSHM_BARR_U64
        shared_data->node[i].u.u64 = GASNETE_PSHM_BARR_PACK(0, 0, two_to_phase);
      #else
        shared_data->node[i].u.wmb.phase = two_to_phase;
      #endif
      }

      /* Flags word to poll or spin on until barrier is done */
      gasneti_atomic_set(&shared_data->state, 0, 0);

      shared_data->size = size;

      /* Indicate completion of this initialization */
      gasneti_atomic_set(&shared_data->ready, size, GASNETI_ATOMIC_REL);
    }
    if (team == GASNET_TEAM_ALL) {
       gasneti_pshmnet_bootstrapBarrier();
    } else if (rank) {
      /* XXX: What if this value is present by chance? */
      gasneti_waituntil(gasneti_atomic_read(&shared_data->ready, 0) == size);
    }
  }

  return pshm_bdata;
}

#if GASNETI_PSHM_BARRIER_HIER

/* Not yet used for SMP-conduit code */
static void gasnete_pshmbarrier_fini_inner(gasnete_pshmbarrier_data_t *pshm_bdata) {
  gasneti_assert(pshm_bdata);
  gasneti_assert(pshm_bdata->shared);

  if (pshm_bdata->shared == gasneti_pshm_barrier) {
    /* TEAM_ALL - shared allocation is "static" */
  } else {
    /* TODO: once we to shared memory allocation in _init, can we also free it? */
  }

  gasneti_free(pshm_bdata);
}

static gasnete_pshmbarrier_data_t *
gasnete_pshmbarrier_init_hier(gasnete_coll_team_t team, int *size_p, int *rank_p, gasnete_coll_peer_list_t **peers_p) {
  gasnete_pshmbarrier_data_t * pshm_bdata;
  
  if (! gasneti_getenv_yesno_withdefault("GASNET_PSHM_BARRIER_HIER", 1)) {
    /* User has disabled at runtime */
    return NULL;
  }

  pshm_bdata = gasnete_pshmbarrier_init_inner(team);
  if (pshm_bdata) {
    *size_p = team->supernode.grp_count;
    *rank_p = team->supernode.grp_rank;
    if (peers_p) *peers_p = &team->supernode_peers;
  }

  return pshm_bdata;
}
#endif /* GASNETI_PSHM_BARRIER_HIER */

#if !GASNETI_PSHM_BARRIER_HIER
/* Entry points for SMP-conduit */

static void gasnete_pshmbarrier_notify(gasnete_coll_team_t team, int id, int flags) {
  gasneti_sync_reads(); /* ensure we read up-to-date phase, etc */
  GASNETE_SPLITSTATE_NOTIFY_ENTER(team);

  (void)gasnete_pshmbarrier_notify_inner(team->barrier_data, id, flags);
  
#if GASNETE_PSHM_BARR_U64
  gasneti_sync_writes();
#else
  /* No sync_writes() needed due to WMB inside notify_inner */
#endif
}

static int gasnete_pshmbarrier_wait(gasnete_coll_team_t team, int id, int flags) {
  gasneti_sync_reads(); /* ensure we read up-to-date phase, etc */
  GASNETE_SPLITSTATE_WAIT_LEAVE(team);

  {
    const int result = gasnete_pshmbarrier_wait_inner(team->barrier_data, id, flags, 0);
    gasneti_assert(result != GASNET_ERR_NOT_READY);

    gasneti_sync_writes();
    return result;
  }
}

static int gasnete_pshmbarrier_try(gasnete_coll_team_t team, int id, int flags) { 
  gasneti_sync_reads(); /* ensure we read up-to-date phase, etc */
  GASNETE_SPLITSTATE_TRY(team);

  GASNETI_SAFE(gasneti_AMPoll());

  {
    const gasneti_atomic_sval_t state = gasnete_pshmbarrier_try_inner(team->barrier_data, 0);
    int result;

    if (state) {
      result = finish_pshm_barrier(team->barrier_data, id, flags, state);

      GASNETE_SPLITSTATE_LEAVE(team);
      gasneti_sync_writes();
    } else {
      GASNETI_WAITHOOK();
      result = GASNET_ERR_NOT_READY;
    }
    return result;
  }
}

static int gasnete_pshmbarrier_result(gasnete_coll_team_t team, int *id) {
  gasneti_sync_reads(); /* ensure we read up-to-date phase, etc */
  GASNETE_SPLITSTATE_RESULT(team);

  { const gasnete_pshmbarrier_data_t * const pshm_bdata = team->barrier_data;
    const gasneti_pshm_barrier_t * const shared_data = pshm_bdata->shared;
    *id = shared_data->value;
    return (GASNET_BARRIERFLAG_ANONYMOUS & shared_data->flags);
  }
}

static void gasnete_pshmbarrier_init(gasnete_coll_team_t team) {
  team->barrier_data = (void *)gasnete_pshmbarrier_init_inner(team);

  team->barrier_notify = &gasnete_pshmbarrier_notify;
  team->barrier_wait =   &gasnete_pshmbarrier_wait;
  team->barrier_try =    &gasnete_pshmbarrier_try;
  team->barrier_result = &gasnete_pshmbarrier_result;

  /* TODO: progress via kick (once thread safety is ensured) */
}
#endif /* !GASNETI_PSHM_BARRIER_HIER */

#endif /* GASNETI_PSHM_BARRIER */

/* ------------------------------------------------------------------------------------ */
/* the AM-based Dissemination implementation of barrier */

/*  an AM-based Dissemination barrier implementation:
     With N nodes, the barrier takes ceil(lg(N)) steps (lg = log-base-2).
     At step i (i=0..):
	node n first sends to node ((n + 2^i) mod N)
	then node n waits to receive (from node ((n + N - 2^i) mod N))
	once we receive for step i, we can move the step i+1 (or finish)
    The distributed nature makes this barrier more scalable than a centralized
     barrier, but also more sensitive to any lack of attentiveness to the
     network.
    We use a static allocation, limiting us to 2^GASNETE_AMBARRIER_MAXSTEP nodes.

    Algorithm is described in section 3.3 of
    John M. Mellor-Crummey and Michael L. Scott. "Algorithms for scalable synchronization
    on shared-memory multiprocessors." ACM ToCS, 9(1):21 65, 1991.
 */

typedef struct {
  gex_HSL_t amdbarrier_lock;
  gex_Rank_t *amdbarrier_peers; /* precomputed list of peers to communicate with */
#if GASNETI_PSHM_BARRIER_HIER
  gasnete_pshmbarrier_data_t *amdbarrier_pshm; /* non-NULL if using hierarchical code */
  int amdbarrier_passive;          /* 2 if some other node makes progress for me, 0 otherwise */
#endif
  int volatile amdbarrier_value;   /* (supernode-)local ambarrier value */
  int volatile amdbarrier_flags;   /* (supernode-)local ambarrier flags */
  int volatile amdbarrier_step;  /*  local ambarrier step */
  int volatile amdbarrier_size;  /*  ceil(lg(nodes)) */
  int volatile amdbarrier_phase; /*  2-phase operation to improve pipelining */
  int volatile amdbarrier_step_done[2][GASNETE_AMDBARRIER_MAXSTEP]; /* non-zero when a step is complete */
  int volatile amdbarrier_recv_value[2]; /*  consensus ambarrier value */
  int volatile amdbarrier_recv_flags[2]; /*  consensus ambarrier flags */
} gasnete_coll_amdbarrier_t;
  
static void gasnete_amdbarrier_notify_reqh(gex_Token_t token,
                                           gex_AM_Arg_t teamid, gex_AM_Arg_t phase, gex_AM_Arg_t step, gex_AM_Arg_t value, gex_AM_Arg_t flags) {
  gasnete_coll_team_t team = gasnete_coll_team_lookup((uint32_t)teamid);
  gasnete_coll_amdbarrier_t *barrier_data = team->barrier_data;

  gex_HSL_Lock(&barrier_data->amdbarrier_lock);
  { 
    /* Note we might not receive the steps in the numbered order.
     * We record the value received on the first one to actually arrive.
     * In subsequent steps we check for mismatch of received values.
     * The local value is compared in the kick function.
     */
    register int recv_flags = barrier_data->amdbarrier_recv_flags[phase];
    register int recv_value = barrier_data->amdbarrier_recv_value[phase];

    if ((recv_flags | flags) & GASNET_BARRIERFLAG_MISMATCH) {
      recv_flags = GASNET_BARRIERFLAG_MISMATCH; 
    } else if (recv_flags & GASNET_BARRIERFLAG_ANONYMOUS) {
      recv_flags = flags; 
      recv_value = value; 
    } else if (!(flags & GASNET_BARRIERFLAG_ANONYMOUS) && (value != recv_value)) {
      recv_flags = GASNET_BARRIERFLAG_MISMATCH; 
    }

    barrier_data->amdbarrier_recv_flags[phase] = recv_flags;
    barrier_data->amdbarrier_recv_value[phase] = recv_value;
  }
  gex_HSL_Unlock(&barrier_data->amdbarrier_lock);

  gasneti_assert(barrier_data->amdbarrier_step_done[phase][step] == 0);
  barrier_data->amdbarrier_step_done[phase][step] = 1;
}

GASNETI_INLINE(gasnete_amdbarrier_send)
void gasnete_amdbarrier_send(gasnete_coll_team_t team, int phase, int step, int value, int flags) {
  gasnete_coll_amdbarrier_t *barrier_data = team->barrier_data;
  gex_AM_RequestShort5(gasneti_THUNK_TM, barrier_data->amdbarrier_peers[step],
                           gasneti_handleridx(gasnete_amdbarrier_notify_reqh), 0,
                           team->team_id, phase, step, value, flags);
}

#if GASNETI_PSHM_BARRIER_HIER
static int gasnete_amdbarrier_kick_pshm(gasnete_coll_team_t team) {
  gasnete_coll_amdbarrier_t *barrier_data = team->barrier_data;
  int done = (barrier_data->amdbarrier_step >= 0);

  if (!done && !gex_HSL_Trylock(&barrier_data->amdbarrier_lock)) {
    done = (barrier_data->amdbarrier_step >= 0);
    if (!done) {
      PSHM_BDATA_DECL(pshm_bdata, barrier_data->amdbarrier_pshm);
      if (gasnete_pshmbarrier_kick(pshm_bdata)) {
        const int phase = barrier_data->amdbarrier_phase;
        const int value = pshm_bdata->shared->value;
        const int flags = pshm_bdata->shared->flags;
        barrier_data->amdbarrier_value = value;
        barrier_data->amdbarrier_flags = flags;
        if (!barrier_data->amdbarrier_size) { /* singelton */
          barrier_data->amdbarrier_recv_value[phase] = value;
          barrier_data->amdbarrier_recv_flags[phase] = flags;
        }
        gasneti_sync_writes();
        barrier_data->amdbarrier_step = 0;
        gex_HSL_Unlock(&barrier_data->amdbarrier_lock); /* Cannot send while holding HSL */
        if (barrier_data->amdbarrier_size && !barrier_data->amdbarrier_passive) {
          gasnete_amdbarrier_send(team, phase, 0, value, flags);
        } else {
          gasnete_barrier_pf_disable(team);
        }
        return 1;
      }
    }
    gex_HSL_Unlock(&barrier_data->amdbarrier_lock);
  }

  return done;
}
#endif

void gasnete_amdbarrier_kick(gasnete_coll_team_t team) {
  gasnete_coll_amdbarrier_t *barrier_data = team->barrier_data;
  int phase, step;
  int cursor, numsteps = 0;
  gex_AM_Arg_t flags, value;

  /* early unlocked read: */
  step = barrier_data->amdbarrier_step;
  if (step == barrier_data->amdbarrier_size)
    return; /* nothing to do */

  gasneti_assert(team->total_ranks > 1); /* singleton should have matched step==size, above */

#if GASNETI_PSHM_BARRIER_HIER
  if (step < 0) {
    /* Cannot begin to probe until local notify is complete */
    gasneti_assert(barrier_data->amdbarrier_pshm);
    if (!gasnete_amdbarrier_kick_pshm(team)) return;
  }
#endif

  if (gex_HSL_Trylock(&barrier_data->amdbarrier_lock))
    return; /* another thread is currently in kick */

  {
    step = barrier_data->amdbarrier_step;
    phase = barrier_data->amdbarrier_phase;

#if GASNETI_PSHM_BARRIER_HIER
    if_pf (step < 0) { /* local notify has not completed */
      gex_HSL_Unlock(&barrier_data->amdbarrier_lock);
      return;
    } else if (barrier_data->amdbarrier_passive) {
      gasnete_barrier_pf_disable(team);
      gex_HSL_Unlock(&barrier_data->amdbarrier_lock);
      return;
    }
    gasneti_assert(!barrier_data->amdbarrier_passive);
#endif

    /* count steps we can take while holding the lock - must release before send,
       so coalesce as many as possible in one acquisition */
    for (cursor = step; cursor < barrier_data->amdbarrier_size &&
                        barrier_data->amdbarrier_step_done[phase][cursor]; ++cursor) {
      barrier_data->amdbarrier_step_done[phase][cursor] = 0;
      numsteps++;
    }

    if (numsteps) { /* completed one or more steps */
      /* we might send at least one message - so fetch args while lock is held */
      flags = barrier_data->amdbarrier_recv_flags[phase];
      value = barrier_data->amdbarrier_recv_value[phase];

      if (!step) { /* merge local notify */
        const int local_flags = barrier_data->amdbarrier_flags;
        const int local_value = barrier_data->amdbarrier_value;

        if ((flags | local_flags) & GASNET_BARRIERFLAG_MISMATCH) {
          flags = GASNET_BARRIERFLAG_MISMATCH; 
        } else if (flags & GASNET_BARRIERFLAG_ANONYMOUS) {
          flags = local_flags; 
          value = local_value; 
        } else if (!(local_flags & GASNET_BARRIERFLAG_ANONYMOUS) && (local_value != value)) {
          flags = GASNET_BARRIERFLAG_MISMATCH; 
        }

        barrier_data->amdbarrier_recv_flags[phase] = flags;
        barrier_data->amdbarrier_recv_value[phase] = value;
      }

      if (cursor == barrier_data->amdbarrier_size) { /* We got the last recv - barrier locally complete */
        gasnete_barrier_pf_disable(team);
        gasneti_sync_writes(); /* flush state before the write to ambarrier_step below */
        numsteps -= 1; /* no send at last step */
      } 
      /* notify all threads of the step increase - 
         this may allow other local threads to proceed on the barrier and even indicate
         barrier completion while we overlap outgoing notifications to other nodes
      */
      barrier_data->amdbarrier_step = cursor;
    } 
  }

  gex_HSL_Unlock(&barrier_data->amdbarrier_lock);

  for ( ; numsteps; numsteps--) {
    gasnete_amdbarrier_send(team, phase, ++step, value, flags);
  }
}

static void gasnete_amdbarrier_notify(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_amdbarrier_t *barrier_data = team->barrier_data;
  int phase;
  int do_send = 1;
  int want_pf = 1;
  int step = 0;
  
  gasneti_sync_reads(); /* ensure we read up-to-date phase, etc */
  GASNETE_SPLITSTATE_NOTIFY_ENTER(team);

#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->amdbarrier_pshm) {
    PSHM_BDATA_DECL(pshm_bdata, barrier_data->amdbarrier_pshm);
    if (gasnete_pshmbarrier_notify_inner(pshm_bdata, id, flags)) {
      id = pshm_bdata->shared->value;
      flags = pshm_bdata->shared->flags;
      want_pf = do_send = !barrier_data->amdbarrier_passive;
    } else {
      do_send = 0;
      step = -1;
    }
  }
#endif

  /* If we are on an ILP64 platform, this cast will ensure we truncate the same
   * bits locally as we do when passing over the network.
   */
  barrier_data->amdbarrier_value = (gex_AM_Arg_t)id;
  barrier_data->amdbarrier_flags = flags;
  barrier_data->amdbarrier_step = step;
  gasneti_sync_writes(); 
  phase = !barrier_data->amdbarrier_phase; /*  enter new phase */
  barrier_data->amdbarrier_phase = phase;

  /*  (possibly) send notify msg to peer */
  if (do_send) gasnete_amdbarrier_send(team, phase, 0, id, flags);
  if (want_pf) gasnete_barrier_pf_enable(team);

  /*  update state */
  gasneti_sync_writes(); /* ensure all state changes committed before return */
}

static void gasnete_amdbarrier_notify_singleton(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_amdbarrier_t *barrier_data = team->barrier_data;
  int phase;
#if GASNETI_PSHM_BARRIER_HIER
  int step = 0;
#endif
  
  gasneti_sync_reads(); /* ensure we read up-to-date phase, etc */
  GASNETE_SPLITSTATE_NOTIFY_ENTER(team);

#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->amdbarrier_pshm) {
    PSHM_BDATA_DECL(pshm_bdata, barrier_data->amdbarrier_pshm);
    if (gasnete_pshmbarrier_notify_inner(pshm_bdata, id, flags)) {
      id = pshm_bdata->shared->value;
      flags = pshm_bdata->shared->flags;
    } else {
      step = -1;
    }
  }
#endif

  /* If we are on an ILP64 platform, this cast will ensure we truncate the same
   * bits locally as we do when passing over the network.
   */
  barrier_data->amdbarrier_value = (gex_AM_Arg_t)id;
  barrier_data->amdbarrier_flags = flags;
#if GASNETI_PSHM_BARRIER_HIER
  barrier_data->amdbarrier_step = step;
#endif
  gasneti_sync_writes(); 
  phase = !barrier_data->amdbarrier_phase; /*  enter new phase */
  barrier_data->amdbarrier_phase = phase;

  barrier_data->amdbarrier_recv_value[phase] = id;	/* to simplify checking in _wait */
  barrier_data->amdbarrier_recv_flags[phase] = flags;

#if GASNETI_PSHM_BARRIER_HIER
  if (0 != step) gasnete_barrier_pf_enable(team);
#endif

  /*  update state */
  gasneti_sync_writes(); /* ensure all state changes committed before return */
}


static int gasnete_amdbarrier_wait(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_amdbarrier_t *barrier_data = team->barrier_data;
#if GASNETI_PSHM_BARRIER_HIER
  PSHM_BDATA_DECL(pshm_bdata, barrier_data->amdbarrier_pshm);
#endif
  int retval = GASNET_OK;
  int i;

  int phase;
  gasneti_sync_reads(); /* ensure we read up-to-date phase, etc */
  phase = barrier_data->amdbarrier_phase;
  GASNETE_SPLITSTATE_WAIT_LEAVE(team);

#if GASNETI_PSHM_BARRIER_HIER
  if (pshm_bdata) {
    const int passive_shift = barrier_data->amdbarrier_passive;
    gasneti_polluntil(gasnete_amdbarrier_kick_pshm(team));
    retval = gasnete_pshmbarrier_wait_inner(pshm_bdata, id, flags, passive_shift);
    if (passive_shift) {
      /* Once the active peer signals done, we can return */
      barrier_data->amdbarrier_value = pshm_bdata->shared->value;
      barrier_data->amdbarrier_flags = pshm_bdata->shared->flags;
      gasneti_sync_writes(); /* ensure all state changes committed before return */
      return retval;
    }
  }
#endif

  if (barrier_data->amdbarrier_step == barrier_data->amdbarrier_size) { /* completed asynchronously before wait (via progressfns or try) */
    GASNETI_TRACE_EVENT_TIME(B,BARRIER_ASYNC_COMPLETION,GASNETI_TICKS_NOW_IFENABLED(B)-gasnete_barrier_notifytime);
    gasneti_sync_reads(); /* ensure correct amdbarrier_recv_* will be read */
  } else { /*  wait for response */
    gasnete_barrier_pf_disable(team);
    GASNET_BLOCKUNTIL((gasnete_amdbarrier_kick(team), barrier_data->amdbarrier_step == barrier_data->amdbarrier_size));
  }

  /* determine return value */
  if_pf (barrier_data->amdbarrier_recv_flags[phase] & GASNET_BARRIERFLAG_MISMATCH) {
    retval = GASNET_ERR_BARRIER_MISMATCH;
  } else
  if_pf(/* try/wait value must match consensus value, if both are present */
        !((flags|barrier_data->amdbarrier_recv_flags[phase]) & GASNET_BARRIERFLAG_ANONYMOUS) &&
	 ((gex_AM_Arg_t)id != barrier_data->amdbarrier_recv_value[phase])) {
    retval = GASNET_ERR_BARRIER_MISMATCH;
  }

  /*  preserve state for possible _result call */
  barrier_data->amdbarrier_value = barrier_data->amdbarrier_recv_value[phase];
  barrier_data->amdbarrier_flags = barrier_data->amdbarrier_recv_flags[phase];

  /*  update state */
  barrier_data->amdbarrier_recv_flags[phase] = GASNET_BARRIERFLAG_ANONYMOUS;
#if GASNETI_PSHM_BARRIER_HIER
  if (pshm_bdata) {
    /* Signal any passive peers w/ the final result */
    pshm_bdata->shared->value = barrier_data->amdbarrier_value;
    pshm_bdata->shared->flags = barrier_data->amdbarrier_flags;
    PSHM_BSTATE_SIGNAL(pshm_bdata, retval, pshm_bdata->private.two_to_phase << 2); /* includes a WMB */
    gasneti_assert(!barrier_data->amdbarrier_passive);
  } else
#endif
  gasneti_sync_writes(); /* ensure all state changes committed before return */

  return retval;
}

static int gasnete_amdbarrier_try(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_amdbarrier_t *barrier_data = team->barrier_data;
  gasneti_sync_reads(); /* ensure we read up-to-date phase, etc */

  GASNETE_SPLITSTATE_TRY(team);

  GASNETI_SAFE(gasneti_AMPoll());

#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->amdbarrier_pshm) {
    const int passive_shift = barrier_data->amdbarrier_passive;
    if (!gasnete_amdbarrier_kick_pshm(team) ||
        !gasnete_pshmbarrier_try_inner(barrier_data->amdbarrier_pshm, passive_shift))
      return GASNET_ERR_NOT_READY;
    if (passive_shift)
      return gasnete_amdbarrier_wait(team, id, flags);
  }
  if (!barrier_data->amdbarrier_passive)
#endif
    gasnete_amdbarrier_kick(team);

  if (barrier_data->amdbarrier_step == barrier_data->amdbarrier_size) return gasnete_amdbarrier_wait(team, id, flags);
  else return GASNET_ERR_NOT_READY;
}

static int gasnete_amdbarrier_result(gasnete_coll_team_t team, int *id) {
  gasneti_sync_reads(); /* ensure we read only up-to-date values */
  GASNETE_SPLITSTATE_RESULT(team);

  { const gasnete_coll_amdbarrier_t * const barrier_data = team->barrier_data;
    *id = barrier_data->amdbarrier_value;
    return (GASNET_BARRIERFLAG_ANONYMOUS & barrier_data->amdbarrier_flags);
  }
}

void gasnete_amdbarrier_kick_team_all(void) {
  gasnete_amdbarrier_kick(GASNET_TEAM_ALL);
}

static void gasnete_amdbarrier_init(gasnete_coll_team_t team) {
  gasnete_coll_amdbarrier_t *barrier_data = gasneti_calloc(1,sizeof(gasnete_coll_amdbarrier_t));
  int steps;
  int total_ranks = team->total_ranks;
  int myrank = team->myrank;
  gasnete_coll_peer_list_t *peers = &team->peers;

#if GASNETI_PSHM_BARRIER_HIER
  PSHM_BDATA_DECL(pshm_bdata, gasnete_pshmbarrier_init_hier(team, &total_ranks, &myrank, &peers));

  if (pshm_bdata) {
    barrier_data->amdbarrier_passive = (pshm_bdata->private.rank != 0) ? 2 : 0; /* precompute shift */
    barrier_data->amdbarrier_pshm = pshm_bdata;
  }
#endif

  gasneti_leak(barrier_data);
  team->barrier_data = barrier_data;
  gex_HSL_Init(&barrier_data->amdbarrier_lock);

  barrier_data->amdbarrier_recv_flags[0] = GASNET_BARRIERFLAG_ANONYMOUS;
  barrier_data->amdbarrier_recv_flags[1] = GASNET_BARRIERFLAG_ANONYMOUS;

  steps = peers->num;
  barrier_data->amdbarrier_size = steps;
  gasneti_assert_uint(barrier_data->amdbarrier_size ,<=, GASNETE_AMDBARRIER_MAXSTEP);

  /* list of log(P) peers we will communicate with */
  barrier_data->amdbarrier_peers = peers->fwd;

#if GASNETI_PSHM_BARRIER_HIER
  if (pshm_bdata && (pshm_bdata->shared->size == 1)) {
    /* With singleton proc on local supernode we can short-cut the PSHM code.  */
    gasnete_pshmbarrier_fini_inner(pshm_bdata);
    barrier_data->amdbarrier_pshm = NULL;
  }
#endif

  team->barrier_notify = steps ? &gasnete_amdbarrier_notify : &gasnete_amdbarrier_notify_singleton;
  team->barrier_wait =   &gasnete_amdbarrier_wait;
  team->barrier_try =    &gasnete_amdbarrier_try;
  team->barrier_result = &gasnete_amdbarrier_result;
  team->barrier_pf =     (team == GASNET_TEAM_ALL) ? &gasnete_amdbarrier_kick_team_all : NULL;
}

#define GASNETE_AMDBARRIER_HANDLERS()                                 \
  gasneti_handler_tableentry_no_bits(gasnete_amdbarrier_notify_reqh,5,REQUEST,SHORT,0)

/* ------------------------------------------------------------------------------------ */
/* the RDMA-based Dissemination implementation of barrier */

/*  An RDMA-based Dissemination barrier implementation. "rmd"
     Principle is identical to the AM-based variant, above.
     The main difference is that RDMA-Puts are used to signal the peers
     instead of using AMs.  This results in polling of "inboxes" in
     memory instead of simply checking for the done bits set by AMs.
     Consequently work from the AM handler moves to the "kick", but is
     silghtly simpler since we'll not run the "kick" before the local
     notify has run.
 */

#if !GASNETI_THREADS
  #define GASNETE_RMDBARRIER_LOCK(_var)		/* empty */
  #define gasnete_rmdbarrier_lock_init(_var)	((void)0)
  #define gasnete_rmdbarrier_trylock(_var)	(0/*success*/)
  #define gasnete_rmdbarrier_unlock(_var)	((void)0)
#elif GASNETI_HAVE_SPINLOCK
  #define GASNETE_RMDBARRIER_LOCK(_var)		gasneti_atomic_t _var;
  #define gasnete_rmdbarrier_lock_init(_var)	gasneti_spinlock_init(_var)
  #define gasnete_rmdbarrier_trylock(_var)	gasneti_spinlock_trylock(_var)
  #define gasnete_rmdbarrier_unlock(_var)	gasneti_spinlock_unlock(_var)
#else
  #define GASNETE_RMDBARRIER_LOCK(_var)		gasneti_mutex_t _var;
  #define gasnete_rmdbarrier_lock_init(_var)	gasneti_mutex_init(_var)
  #define gasnete_rmdbarrier_trylock(_var)	gasneti_mutex_trylock(_var)
  #define gasnete_rmdbarrier_unlock(_var)	gasneti_mutex_unlock(_var)
#endif

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
#if !GASNETI_THREADS
  gex_Event_t *barrier_events; /* array of events for non-blocking puts */
#endif
} gasnete_coll_rmdbarrier_t;

/* So, what's this inbox structure all about?
 * We cannot assume RDMA will deliver the payload entirely in order.
 * So, we need some sort of checksum to tell when it has all arrived.
 * The "best" checksum is to simply send the data twice, but we go one
 * step further and invert the second copy to protect against an
 * imagined implementation that zeros the payload area first (which,
 * for instance, some memcpy() implementations are known to do).
 * Additionally, this ordering of fields ensures that for the "normal"
 * case of in-order delivery the _poll will detect incomplete payloads
 * in just 2 reads instead of 4.
 */
typedef struct gasnete_coll_rmdbarrier_inbox_s {
  int volatile flags;
  int volatile value;
  int volatile value2;
  int volatile flags2;
} gasnete_coll_rmdbarrier_inbox_t;
  
/* Pad struct to a specfic size and interleave */
#define GASNETE_RDMABARRIER_INBOX_SZ 64 /* requires max 4K total space */
#define GASNETE_RDMABARRIER_INBOX(_bd,_state)     \
            ((gasnete_coll_rmdbarrier_inbox_t *)        \
             ((uintptr_t)((_bd)->barrier_inbox)         \
                       + (unsigned)((_state)-2) * GASNETE_RDMABARRIER_INBOX_SZ))
#define GASNETE_RDMABARRIER_INBOX_REMOTE(_bd,_step,_state)  \
            ((gasnete_coll_rmdbarrier_inbox_t *)            \
             ((_bd)->barrier_peers[(unsigned)(_step)].addr    \
                       + (unsigned)((_state)-2) * GASNETE_RDMABARRIER_INBOX_SZ))
#define GASNETE_RDMABARRIER_INBOX_NEXT(_addr)    \
            ((gasnete_coll_rmdbarrier_inbox_t *) \
             ((uintptr_t)(_addr) + 2U * GASNETE_RDMABARRIER_INBOX_SZ))

GASNETI_INLINE(gasnete_rmdbarrier_send)
void gasnete_rmdbarrier_send(gasnete_coll_rmdbarrier_t *barrier_data,
                             int numsteps, unsigned int state,
                             gex_AM_Arg_t value, gex_AM_Arg_t flags) {
  GASNET_BEGIN_FUNCTION(); /* XXX: can we remove/avoid this lookup? */
  unsigned int step = state >> 1;
  gex_Event_t event;
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

  /* Here we use NBI bulk puts in a recursive NBI access region, which avoids
   * consuming any of the 65535 explicit events promised to the client.
   */

  gasnete_begin_nbi_accessregion(0,1 GASNETI_THREAD_PASS);
  for (i = 0; i < numsteps; ++i, state += 2, step += 1) {
    const gex_Rank_t jobrank = barrier_data->barrier_peers[step].jobrank;
    void * const addr = GASNETE_RDMABARRIER_INBOX_REMOTE(barrier_data, step, state);
    gasnete_put_nbi(gasneti_THUNK_TM, jobrank, addr, payload, sizeof(*payload),
                    GEX_EVENT_DEFER, 0 GASNETI_THREAD_PASS);
  }
  event = gasnete_end_nbi_accessregion(0 GASNETI_THREAD_PASS);

#if GASNETI_THREADS
  /* sync the new ops, since we can't know this thread will re-enter the barrier code */
  gasnete_wait(event GASNETI_THREAD_PASS);
#else
  /* save the new ops to sync after the barrier is complete */
  step -= (numsteps + 1);
  gasneti_assert(barrier_data->barrier_events[step] == GEX_EVENT_INVALID);
  barrier_data->barrier_events[step] = event;
#endif
}

#if GASNETI_PSHM_BARRIER_HIER
static int gasnete_rmdbarrier_kick_pshm(gasnete_coll_team_t team) {
  gasnete_coll_rmdbarrier_t *barrier_data = team->barrier_data;
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
          gasnete_rmdbarrier_send(barrier_data, 1, state+2, value, flags);
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

GASNETI_INLINE(gasnete_rmdbarrier_poll)
int gasnete_rmdbarrier_poll(gasnete_coll_rmdbarrier_inbox_t *inbox) {
  return ((inbox->value == ~inbox->value2) && (inbox->flags == ~inbox->flags2));
}

void gasnete_rmdbarrier_kick(gasnete_coll_team_t team) {
  gasnete_coll_rmdbarrier_t *barrier_data = team->barrier_data;
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
    if (!gasnete_rmdbarrier_kick_pshm(team)) return;
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

    gasneti_assert_int(inbox->value2 ,==, ~step_value);
    gasneti_assert_int(inbox->flags2 ,==, ~step_flags);

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
    gasnete_rmdbarrier_send(barrier_data, numsteps, state+2, value, flags);
  }
}

static void gasnete_rmdbarrier_notify(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_rmdbarrier_t *barrier_data = team->barrier_data;
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

  if (do_send) gasnete_rmdbarrier_send(barrier_data, 1, state, id, flags);
  if (want_pf) gasnete_barrier_pf_enable(team);

  /*  update state */
  gasneti_sync_writes(); /* ensure all state changes committed before return */
}

/* Notify specialized to one (super)node case (reduced branches in BOTH variants) */
static void gasnete_rmdbarrier_notify_singleton(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_rmdbarrier_t *barrier_data = team->barrier_data;
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

static int gasnete_rmdbarrier_wait(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_rmdbarrier_t *barrier_data = team->barrier_data;
#if GASNETI_PSHM_BARRIER_HIER
  PSHM_BDATA_DECL(pshm_bdata, barrier_data->barrier_pshm);
#endif
  int retval = GASNET_OK;

  gasneti_sync_reads(); /* ensure we read only up-to-date values */
  GASNETE_SPLITSTATE_WAIT_LEAVE(team);

#if GASNETI_PSHM_BARRIER_HIER
  if (pshm_bdata) {
    const int passive_shift = barrier_data->barrier_passive;
    gasneti_polluntil(gasnete_rmdbarrier_kick_pshm(team));
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
    gasnete_rmdbarrier_kick(team);
    /* cannot BLOCKUNTIL since progess may occur on non-AM events */
    while (barrier_data->barrier_state < barrier_data->barrier_goal) {
      GASNETI_WAITHOOK();
      GASNETI_SAFE(gasneti_AMPoll());
      gasnete_rmdbarrier_kick(team);
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

#if !GASNETI_THREADS
  /*  "drain" the put_nb events, if any */
 #if GASNETI_PSHM_BARRIER_HIER
  if (!barrier_data->barrier_passive)
 #endif
  gasnete_wait_all(barrier_data->barrier_events, barrier_data->barrier_size GASNETI_THREAD_GET);
#endif

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

static int gasnete_rmdbarrier_try(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_rmdbarrier_t *barrier_data = team->barrier_data;
  gasneti_sync_reads(); /* ensure we read up-to-date phase, etc */

  GASNETE_SPLITSTATE_TRY(team);

  GASNETI_SAFE(gasneti_AMPoll());

#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->barrier_pshm) {
    const int passive_shift = barrier_data->barrier_passive;
    if (!gasnete_rmdbarrier_kick_pshm(team) ||
        !gasnete_pshmbarrier_try_inner(barrier_data->barrier_pshm, passive_shift))
      return GASNET_ERR_NOT_READY;
    if (passive_shift)
      return gasnete_rmdbarrier_wait(team, id, flags);
  }
  if (!barrier_data->barrier_passive)
#endif
    gasnete_rmdbarrier_kick(team);

  if (barrier_data->barrier_state >= barrier_data->barrier_goal)
    return gasnete_rmdbarrier_wait(team, id, flags);
  else return GASNET_ERR_NOT_READY;
}

static int gasnete_rmdbarrier_result(gasnete_coll_team_t team, int *id) {
  gasneti_sync_reads(); /* ensure we read up-to-date values */
  GASNETE_SPLITSTATE_RESULT(team);

  { const gasnete_coll_rmdbarrier_t * const barrier_data = team->barrier_data;
    *id = barrier_data->barrier_value;
    return (GASNET_BARRIERFLAG_ANONYMOUS & barrier_data->barrier_flags);
  }
}

void gasnete_rmdbarrier_kick_team_all(void) {
  gasnete_rmdbarrier_kick(GASNET_TEAM_ALL);
}

static void gasnete_rmdbarrier_init(gasnete_coll_team_t team) {
  gasnete_coll_rmdbarrier_t *barrier_data;
  int steps;
  int total_ranks = team->total_ranks;
  int myrank = team->myrank;
  gasnete_coll_peer_list_t *peers = &team->peers;

#if GASNETI_PSHM_BARRIER_HIER
  PSHM_BDATA_DECL(pshm_bdata, gasnete_pshmbarrier_init_hier(team, &total_ranks, &myrank, &peers));
#endif

  barrier_data = gasneti_malloc_aligned(GASNETI_CACHE_LINE_BYTES, sizeof(gasnete_coll_rmdbarrier_t));
  gasneti_leak_aligned(barrier_data);
  memset(barrier_data, 0, sizeof(gasnete_coll_rmdbarrier_t));
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

#if !GASNETI_THREADS
    barrier_data->barrier_events = gasneti_calloc(steps, sizeof(gex_Event_t));
#endif

    gasneti_assert(gasnete_rdmabarrier_auxseg);
    gasneti_static_assert(2 * sizeof(gasnete_coll_rmdbarrier_inbox_t) <= GASNETE_RDMABARRIER_INBOX_SZ);
    barrier_data->barrier_inbox = gasnete_rdmabarrier_auxseg[gasneti_mynode].addr;

    barrier_data->barrier_peers = gasneti_malloc((1+steps) * sizeof(* barrier_data->barrier_peers));
    gasneti_leak(barrier_data->barrier_peers);
  
    for (step = 0; step < steps; ++step) {
      gex_Rank_t jobrank = peers->fwd[step]; // is always a job rank
      barrier_data->barrier_peers[1+step].jobrank = jobrank;
      barrier_data->barrier_peers[1+step].addr = (uintptr_t)gasnete_rdmabarrier_auxseg[jobrank].addr;
    }
  } else {
    barrier_data->barrier_state = barrier_data->barrier_goal;
#if !GASNETI_THREADS
    /* simplifies the sync path(s) */
    barrier_data->barrier_events = gasneti_calloc(1, sizeof(gex_Event_t));
#endif
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

  team->barrier_notify = steps ? &gasnete_rmdbarrier_notify : &gasnete_rmdbarrier_notify_singleton;
  team->barrier_wait =   &gasnete_rmdbarrier_wait;
  team->barrier_try =    &gasnete_rmdbarrier_try;
  team->barrier_result = &gasnete_rmdbarrier_result;
  team->barrier_pf =     (team == GASNET_TEAM_ALL) ? &gasnete_rmdbarrier_kick_team_all : NULL;
}

/* ------------------------------------------------------------------------------------ */
/* AM-based centralized implementation of barrier */

/*  a silly, centralized barrier implementation:
     everybody sends notifies to a single node, where we count them up
     central node eventually notices the barrier is complete (probably
     when it calls wait) and then it broadcasts the completion to all the nodes
    The main problem is the need for the master to call wait before the barrier can
     make progress - we really need a way for the "last thread" to notify all 
     the threads when completion is detected, but AM semantics don't provide a 
     simple way to do this.
    The centralized nature also makes it non-scalable - we really want to use 
     a tree-based barrier or pairwise exchange algorithm for scalability
     (but these impose even greater potential delays due to the lack of attentiveness to
     barrier progress)
 */

typedef struct {
  int volatile amcbarrier_phase; /*  2-phase operation to improve pipelining */
  int volatile amcbarrier_response_done[2];     /*  non-zero when ambarrier is complete */
  int volatile amcbarrier_response_flags[2];    /*  consensus ambarrier flags */
  int volatile amcbarrier_response_value[2];    /*  consensus ambarrier value */
  
  int           amcbarrier_max;    /* length of amcbarrier_active */
  gex_Rank_t    amcbarrier_master; // jobrank of master
  gex_Rank_t   *amcbarrier_active; // jobranks that need to recv broadcast

#if GASNETI_PSHM_BARRIER_HIER
  gasnete_pshmbarrier_data_t *amcbarrier_pshm; /* non-NULL if using hierarchical code */
  int amcbarrier_passive;          /* 2 if some other node makes progress for me, 0 otherwise */
  int amcbarrier_pshm_notify_done; /* must kick the pshm barrier while zero */
#endif
  /*  global state on master */
  gex_HSL_t amcbarrier_lock;
  int volatile amcbarrier_consensus_value[2]; /*  consensus ambarrier value */
  int volatile amcbarrier_consensus_flags[2]; /*  consensus ambarrier flags */
  int volatile amcbarrier_count[2];/*  count of how many remotes have notified (on master) */
} gasnete_coll_amcbarrier_t;


static void gasnete_amcbarrier_notify_reqh(gex_Token_t token,
                                           gex_AM_Arg_t teamid, gex_AM_Arg_t phase, gex_AM_Arg_t value, gex_AM_Arg_t flags) {
  gasnete_coll_team_t team = gasnete_coll_team_lookup((uint32_t)teamid);
  gasnete_coll_amcbarrier_t *barrier_data = team->barrier_data;

  gasneti_assert_uint(gasneti_mynode ,==, barrier_data->amcbarrier_master);
  
  gex_HSL_Lock(&barrier_data->amcbarrier_lock);
  { int count = barrier_data->amcbarrier_count[phase];
    const int consensus_flags = barrier_data->amcbarrier_consensus_flags[phase];
    const int consensus_value = barrier_data->amcbarrier_consensus_value[phase];

    if ((flags|consensus_flags) & GASNET_BARRIERFLAG_MISMATCH) {
      barrier_data->amcbarrier_consensus_flags[phase] = GASNET_BARRIERFLAG_MISMATCH;
    } else if (consensus_flags & GASNET_BARRIERFLAG_ANONYMOUS) {
      barrier_data->amcbarrier_consensus_flags[phase] = flags;
      barrier_data->amcbarrier_consensus_value[phase] = value;
    } else if (!(flags & GASNET_BARRIERFLAG_ANONYMOUS) && (value != consensus_value)) {
      barrier_data->amcbarrier_consensus_flags[phase] = GASNET_BARRIERFLAG_MISMATCH;
    }

    count++;
    if (count == barrier_data->amcbarrier_max) gasneti_sync_writes(); /* about to signal, ensure we flush state */
    barrier_data->amcbarrier_count[phase] = count;
  }
  gex_HSL_Unlock(&barrier_data->amcbarrier_lock);
}

static void gasnete_amcbarrier_done_reqh(gex_Token_t token,
  gex_AM_Arg_t teamid, gex_AM_Arg_t phase,
  gex_AM_Arg_t flags, gex_AM_Arg_t value) {
  gasnete_coll_team_t team = gasnete_coll_team_lookup((uint32_t)teamid);
  gasnete_coll_amcbarrier_t *barrier_data = team->barrier_data;

#if GASNETI_PSHM_BARRIER_HIER
  gasneti_assert(!barrier_data->amcbarrier_passive);
#endif
  gasneti_assert(phase == barrier_data->amcbarrier_phase);

  barrier_data->amcbarrier_response_flags[phase] = flags;
  barrier_data->amcbarrier_response_value[phase] = value;
  gasneti_sync_writes();
  barrier_data->amcbarrier_response_done[phase] = 1;
}

GASNETI_INLINE(gasnete_amcbarrier_send)
void gasnete_amcbarrier_send(gasnete_coll_team_t team, int phase, int value, int flags) {
  gasnete_coll_amcbarrier_t *barrier_data = team->barrier_data;

  if (barrier_data->amcbarrier_max == 1) {
    barrier_data->amcbarrier_response_value[phase] = (gex_AM_Arg_t)value;
    barrier_data->amcbarrier_response_flags[phase] = flags;
    barrier_data->amcbarrier_response_done[phase] = 1;
  } else {
    /*  send notify msg to master */
#if GASNETI_PSHM_BARRIER_HIER
    if (!barrier_data->amcbarrier_passive)
#endif
    gex_AM_RequestShort4(gasneti_THUNK_TM, barrier_data->amcbarrier_master,
                             gasneti_handleridx(gasnete_amcbarrier_notify_reqh), 0,
                             team->team_id, phase, value, flags);
  }

  if (gasneti_mynode == barrier_data->amcbarrier_master) gasnete_barrier_pf_enable(team);
}

#if GASNETI_PSHM_BARRIER_HIER
static int gasnete_amcbarrier_kick_pshm(gasnete_coll_team_t team) {
  gasnete_coll_amcbarrier_t *barrier_data = team->barrier_data;
  int done = barrier_data->amcbarrier_pshm_notify_done;

  if (!done && !gex_HSL_Trylock(&barrier_data->amcbarrier_lock)) {
    done = barrier_data->amcbarrier_pshm_notify_done;
    if (!done) {
      PSHM_BDATA_DECL(pshm_bdata, barrier_data->amcbarrier_pshm);
      if (gasnete_pshmbarrier_kick(pshm_bdata)) {
        barrier_data->amcbarrier_pshm_notify_done = 1;
        gex_HSL_Unlock(&barrier_data->amcbarrier_lock); /* Cannot send while holding HSL */
        gasnete_amcbarrier_send(team,
                                barrier_data->amcbarrier_phase,
                                pshm_bdata->shared->value,
                                pshm_bdata->shared->flags);
        return 1;
      }
    }
    gex_HSL_Unlock(&barrier_data->amcbarrier_lock);
  }

  return done;
}
#endif

/*  make some progress on the ambarrier */
void gasnete_amcbarrier_kick(gasnete_coll_team_t team) {
  gasnete_coll_amcbarrier_t *barrier_data = team->barrier_data;
  int phase = barrier_data->amcbarrier_phase;

#if GASNETI_PSHM_BARRIER_HIER
  PSHM_BDATA_DECL(pshm_bdata, barrier_data->amcbarrier_pshm);
  if (pshm_bdata) {
    gasnete_amcbarrier_kick_pshm(team);
  }
#endif

  /*  master does all the remaining work */
  if (gasneti_mynode != barrier_data->amcbarrier_master) return;

  if (barrier_data->amcbarrier_count[phase] == barrier_data->amcbarrier_max) {
    int gotit = 0;
    int flags = 0;
    int value = 0;
    gex_HSL_Lock(&barrier_data->amcbarrier_lock);
      if (barrier_data->amcbarrier_count[phase] == barrier_data->amcbarrier_max) {
        flags = barrier_data->amcbarrier_consensus_flags[phase];
        value = barrier_data->amcbarrier_consensus_value[phase];
        gotit = 1;
        /*  reset state before sending AMs - unlock is the WMB */
        barrier_data->amcbarrier_count[phase] = 0;
        barrier_data->amcbarrier_consensus_flags[phase] = GASNET_BARRIERFLAG_ANONYMOUS;
      }
    gex_HSL_Unlock(&barrier_data->amcbarrier_lock);

    if (gotit) { /*  ambarrier is complete */
      int i;

      gasnete_barrier_pf_disable(team);

      /*  inform the active nodes */
      for (i=0; i < barrier_data->amcbarrier_max; i++) {
        gex_AM_RequestShort4(gasneti_THUNK_TM, barrier_data->amcbarrier_active[i],
                                 gasneti_handleridx(gasnete_amcbarrier_done_reqh), 0,
                                 team->team_id, phase, flags, value);
      }
    }
  }
}

static void gasnete_amcbarrier_notify(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_amcbarrier_t *barrier_data = team->barrier_data;
  int do_send = 1;
  int phase;

  gasneti_sync_reads(); /* ensure we read up-to-date phase, etc */
  GASNETE_SPLITSTATE_NOTIFY_ENTER(team);

  phase = !barrier_data->amcbarrier_phase; /*  enter new phase */
  barrier_data->amcbarrier_phase = phase;

#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->amcbarrier_pshm) {
    PSHM_BDATA_DECL(pshm_bdata, barrier_data->amcbarrier_pshm);
    do_send = barrier_data->amcbarrier_pshm_notify_done =
            gasnete_pshmbarrier_notify_inner(pshm_bdata, id, flags);
    id = pshm_bdata->shared->value;
    flags = pshm_bdata->shared->flags;
  }
#endif

  if (do_send) gasnete_amcbarrier_send(team, phase, id, flags);

  /*  update state */
  gasneti_sync_writes(); /* ensure all state changes committed before return */
}

static int gasnete_amcbarrier_wait(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_amcbarrier_t *barrier_data = team->barrier_data;
#if GASNETI_PSHM_BARRIER_HIER
 PSHM_BDATA_DECL(pshm_bdata, barrier_data->amcbarrier_pshm);
#endif
  int retval = GASNET_OK;
  int phase;

  gasneti_sync_reads(); /* ensure we read up-to-date phase, etc */
  phase = barrier_data->amcbarrier_phase;
  GASNETE_SPLITSTATE_WAIT_LEAVE(team);

#if GASNETI_PSHM_BARRIER_HIER
  if (pshm_bdata) {
    const int passive_shift = barrier_data->amcbarrier_passive;
    gasneti_polluntil(gasnete_amcbarrier_kick_pshm(team));
    retval = gasnete_pshmbarrier_wait_inner(pshm_bdata, id, flags, passive_shift);
    if (passive_shift) {
      /* Once the active peer signals done, we can return */
      barrier_data->amcbarrier_response_value[phase] = pshm_bdata->shared->value;
      barrier_data->amcbarrier_response_flags[phase] = pshm_bdata->shared->flags;
      gasneti_sync_writes(); /* ensure all state changes committed before return */
      return retval;
    }
  }
#endif

  if (barrier_data->amcbarrier_response_done[phase]) { /* completed asynchronously before wait (via progressfns or try) */
    GASNETI_TRACE_EVENT_TIME(B,BARRIER_ASYNC_COMPLETION,GASNETI_TICKS_NOW_IFENABLED(B)-gasnete_barrier_notifytime);
    gasneti_sync_reads(); /* ensure we read correct amcbarrier_response_*[] */
  } else { /*  wait for response */
    gasnete_barrier_pf_disable(team);
    GASNET_BLOCKUNTIL((gasnete_amcbarrier_kick(team), barrier_data->amcbarrier_response_done[phase]));
    /* GASNET_BLOCKUNTIL contains RMB needed for read of amcbarrier_response_*[] */
  }

  /* determine result */
  if_pf(barrier_data->amcbarrier_response_flags[phase] & GASNET_BARRIERFLAG_MISMATCH) {
    retval = GASNET_ERR_BARRIER_MISMATCH;
  } else
  if_pf(/* try/wait value must match consensus value, if both are present */
        !((flags|barrier_data->amcbarrier_response_flags[phase]) & GASNET_BARRIERFLAG_ANONYMOUS) &&
         (id != barrier_data->amcbarrier_response_value[phase])) {
    retval = GASNET_ERR_BARRIER_MISMATCH;
  }

  /*  update state */
  barrier_data->amcbarrier_response_done[phase] = 0;
#if GASNETI_PSHM_BARRIER_HIER
  if (pshm_bdata) {
    /* Signal any passive peers w/ the final result */
    pshm_bdata->shared->value = barrier_data->amcbarrier_response_value[phase];
    pshm_bdata->shared->flags = barrier_data->amcbarrier_response_flags[phase];
    PSHM_BSTATE_SIGNAL(pshm_bdata, retval, pshm_bdata->private.two_to_phase << 2); /* includes a WMB */
    gasneti_assert(!barrier_data->amcbarrier_passive);
  } else
#endif
  gasneti_sync_writes(); /* ensure all state changes committed before return */
  
  return retval;
}

static int gasnete_amcbarrier_try(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_amcbarrier_t *barrier_data = team->barrier_data;

  gasneti_sync_reads(); /* ensure we read up-to-date phase, etc */
  GASNETE_SPLITSTATE_TRY(team);

  GASNETI_SAFE(gasneti_AMPoll());

#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->amcbarrier_pshm) {
    const int passive_shift = barrier_data->amcbarrier_passive;
    if (!gasnete_amcbarrier_kick_pshm(team) ||
        !gasnete_pshmbarrier_try_inner(barrier_data->amcbarrier_pshm, passive_shift))
      return GASNET_ERR_NOT_READY;
    if (passive_shift)
      return gasnete_amcbarrier_wait(team, id, flags);
  }
  if (!barrier_data->amcbarrier_passive)
#endif
    gasnete_amcbarrier_kick(team);

  if (barrier_data->amcbarrier_response_done[barrier_data->amcbarrier_phase]) return gasnete_amcbarrier_wait(team, id, flags);
  else return GASNET_ERR_NOT_READY;
}

static int gasnete_amcbarrier_result(gasnete_coll_team_t team, int *id) {
  gasneti_sync_reads(); /* ensure we read up-to-date values */
  GASNETE_SPLITSTATE_RESULT(team);

  { const gasnete_coll_amcbarrier_t * const barrier_data = team->barrier_data;
    const int phase = barrier_data->amcbarrier_phase;
    *id = barrier_data->amcbarrier_response_value[phase];
    return (GASNET_BARRIERFLAG_ANONYMOUS & barrier_data->amcbarrier_response_flags[phase]);
  }
}

void gasnete_amcbarrier_kick_team_all(void) {
  gasnete_amcbarrier_kick(GASNET_TEAM_ALL);
}

static void gasnete_amcbarrier_init(gasnete_coll_team_t team, gex_Rank_t *nodes, gex_Rank_t *supernodes) {
  gasnete_coll_amcbarrier_t *barrier_data = gasneti_calloc(1,sizeof(gasnete_coll_amcbarrier_t));
  int total_ranks = team->total_ranks;
  gex_Rank_t *active = nodes;

#if GASNETI_PSHM_BARRIER_HIER
  int myrank = team->myrank;
  PSHM_BDATA_DECL(pshm_bdata, gasnete_pshmbarrier_init_hier(team, &total_ranks, &myrank, NULL));

  if (pshm_bdata) {
    barrier_data->amcbarrier_passive = (pshm_bdata->private.rank != 0) ? 2 : 0; /* precompute shift */
    barrier_data->amcbarrier_pshm = pshm_bdata;
    active = supernodes;
  }
#endif

  gasneti_leak(barrier_data);
  gex_HSL_Init(&barrier_data->amcbarrier_lock);

  barrier_data->amcbarrier_consensus_flags[0] = GASNET_BARRIERFLAG_ANONYMOUS;
  barrier_data->amcbarrier_consensus_flags[1] = GASNET_BARRIERFLAG_ANONYMOUS;

  barrier_data->amcbarrier_max = total_ranks;

  /* Choice of last active node as "master" is arbitrary (as long as all agree) */
  barrier_data->amcbarrier_master = active[total_ranks-1];

  /* Only master needs the vector of active nodes */
  if (gasneti_mynode == barrier_data->amcbarrier_master) {
    size_t alloc_size = total_ranks * sizeof(gex_Rank_t);
    gasneti_leak(barrier_data->amcbarrier_active = gasneti_malloc(alloc_size));
    memcpy(barrier_data->amcbarrier_active, active, alloc_size);
  }

#if GASNETI_PSHM_BARRIER_HIER
  if (pshm_bdata && (pshm_bdata->shared->size == 1)) {
    /* With singleton proc on local supernode we can short-cut the PSHM code.
     * This does not require changing the amcbarrier_master selected above.
     */
    gasnete_pshmbarrier_fini_inner(pshm_bdata);
    barrier_data->amcbarrier_pshm = NULL;
  }
#endif

  team->barrier_data =   barrier_data;
  team->barrier_notify = &gasnete_amcbarrier_notify;
  team->barrier_wait =   &gasnete_amcbarrier_wait;
  team->barrier_try =    &gasnete_amcbarrier_try;
  team->barrier_result = &gasnete_amcbarrier_result;
  team->barrier_pf =     ((team == GASNET_TEAM_ALL) && (total_ranks > 1))
                             ? &gasnete_amcbarrier_kick_team_all : NULL;
}

#define GASNETE_AMCBARRIER_HANDLERS()                                 \
  gasneti_handler_tableentry_no_bits(gasnete_amcbarrier_notify_reqh,4,REQUEST,SHORT,0), \
  gasneti_handler_tableentry_no_bits(gasnete_amcbarrier_done_reqh,4,REQUEST,SHORT,0)

/* ------------------------------------------------------------------------------------ */
/* Generic layer called by both gasnet_coll_ and gasnet_ barrier APIs
 * These dispatch via function pointer(s) in the team structure
 */

GASNETI_INLINE(gasnete_barrier_notify_common)
void gasnete_barrier_notify_common(gasnete_coll_team_t team, int id, int flags) {
  gasneti_assert(team->barrier_notify);
  (*team->barrier_notify)(team, id, flags);  
}

GASNETI_INLINE(gasnete_barrier_try_common)
int gasnete_barrier_try_common(gasnete_coll_team_t team, int id, int flags) {
  gasneti_assert(team->barrier_try);
  return (*team->barrier_try)(team, id, flags);
}

GASNETI_INLINE(gasnete_barrier_wait_common)
int gasnete_barrier_wait_common(gasnete_coll_team_t team, int id, int flags) {
  gasneti_assert(team->barrier_wait);
  return (*team->barrier_wait)(team, id, flags);
}

GASNETI_INLINE(gasnete_barrier_common)
int gasnete_barrier_common(gasnete_coll_team_t team, int id, int flags) {
  gasneti_assert(team->barrier);
  return (*team->barrier)(team, id, flags);
}

GASNETI_INLINE(gasnete_barrier_result_common)
int gasnete_barrier_result_common(gasnete_coll_team_t team, int *id) {
  gasneti_assert(team->barrier_result);
  gasneti_assert(id);
  return (*team->barrier_result)(team, id);
}

/* ------------------------------------------------------------------------------------ */
/* gasnete_coll_barrier* layer which calls the generic layer, above.
 * These check flags before calling the generic layer.
 */

void gasnete_coll_barrier_notify(gasnete_coll_team_t team, int id, int flags GASNETI_THREAD_FARG) {
  gasneti_assert(flags == (flags & GASNETE_BARRIERFLAGS_CLIENT_COLL));
  gasnete_barrier_notify_common(team, id, flags);
}

int gasnete_coll_barrier_try(gasnete_coll_team_t team, int id, int flags GASNETI_THREAD_FARG) {
  gasneti_assert(flags == (flags & GASNETE_BARRIERFLAGS_CLIENT_COLL));
  return gasnete_barrier_try_common(team, id, flags);
}

int gasnete_coll_barrier_wait(gasnete_coll_team_t team, int id, int flags GASNETI_THREAD_FARG) {
  gasneti_assert(flags == (flags & GASNETE_BARRIERFLAGS_CLIENT_COLL));
  return gasnete_barrier_wait_common(team, id, flags);
}

int gasnete_coll_barrier(gasnete_coll_team_t team, int id, int flags GASNETI_THREAD_FARG) {
  gasneti_assert(flags == (flags & GASNETE_BARRIERFLAGS_CLIENT_COLL));
  return gasnete_barrier_common(team, id, flags);
}

int gasnete_coll_barrier_result(gasnete_coll_team_t team, int *id GASNETI_THREAD_FARG) {
  return gasnete_barrier_result_common(team, id);
}

/* ------------------------------------------------------------------------------------ */
/* gasnet_barrier* layer which calls the generic layer, above.
 * Handles passing TEAM_ALL to the generic layer.
 * Include tracing not (yet?) present in the teams-oriented gasnete_coll_barrier* API
 */

void gasnet_barrier_notify(int id, int flags) {
  GASNETI_TRACE_PRINTF(B, ("BARRIER_NOTIFY(team=GASNET_TEAM_ALL,id=%i,flags=%i)", id, flags));
  #if GASNETI_STATS_OR_TRACE
    gasnete_barrier_notifytime = GASNETI_TICKS_NOW_IFENABLED(B);
  #endif

  gasneti_assert(GASNET_TEAM_ALL->barrier_notify);
  gasneti_assert(flags == (flags & GASNETE_BARRIERFLAGS_CLIENT_ALL));
  gasnete_barrier_notify_common(GASNET_TEAM_ALL, id, flags);
}

int gasnet_barrier_wait(int id, int flags) {
  #if GASNETI_STATS_OR_TRACE
    gasneti_tick_t wait_start = GASNETI_TICKS_NOW_IFENABLED(B);
  #endif
  int retval;
  GASNETI_TRACE_EVENT_TIME(B,BARRIER_NOTIFYWAIT,GASNETI_TICKS_NOW_IFENABLED(B)-gasnete_barrier_notifytime);
  
  gasneti_assert(GASNET_TEAM_ALL->barrier_wait);
  gasneti_assert(flags == (flags & GASNETE_BARRIERFLAGS_CLIENT_ALL));
  retval = gasnete_barrier_wait_common(GASNET_TEAM_ALL, id, flags);
 
  GASNETI_TRACE_EVENT_TIME(B,BARRIER_WAIT,GASNETI_TICKS_NOW_IFENABLED(B)-wait_start);
  return retval;
}

int gasnet_barrier_try(int id, int flags) {
  int retval;

  gasneti_assert(GASNET_TEAM_ALL->barrier_try);
  gasneti_assert(flags == (flags & GASNETE_BARRIERFLAGS_CLIENT_ALL));
  retval = gasnete_barrier_try_common(GASNET_TEAM_ALL, id, flags);

  GASNETI_TRACE_EVENT_VAL(B,BARRIER_TRY,(retval != GASNET_ERR_NOT_READY));
  return retval;
}

int gasnet_barrier(int id, int flags) {
  GASNETI_TRACE_PRINTF(B, ("BARRIER(team=GASNET_TEAM_ALL,id=%i,flags=%i)", id, flags));

  gasneti_assert(GASNET_TEAM_ALL->barrier);
  gasneti_assert(flags == (flags & GASNETE_BARRIERFLAGS_CLIENT_ALL));
  return gasnete_barrier_common(GASNET_TEAM_ALL, id, flags);
}

int gasnet_barrier_result(int *id) {
  gasneti_assert(GASNET_TEAM_ALL->barrier_result);
  return gasnete_barrier_result_common(GASNET_TEAM_ALL, id);
}

/* ------------------------------------------------------------------------------------ */
/* Helpers for possible reuse as the team->barrier* function pointers */

/* This is for use by conduits that don't have a conforming version */
static int gasnete_barrier_result_default(gasnete_coll_team_t team, int *id) {
  /* Pretend all barriers are anonymous if no _result is implemented */
  return 1;
}

/* This is for use by conduits that don't have a specialized version */
int gasnete_barrier_default(gasnete_coll_team_t team, int id, int flags) {
  #if GASNETI_STATS_OR_TRACE
    gasneti_tick_t barrier_start = GASNETI_TICKS_NOW_IFENABLED(B);
  #endif
  int retval;

  GASNETE_SPLITSTATE_BARRIER(team);
  
  gasneti_assert(team->barrier_notify);
  (*team->barrier_notify)(team, id, flags);  

  gasneti_assert(team->barrier_wait);
  retval = (*team->barrier_wait)(team, id, flags);  
 
  GASNETI_TRACE_EVENT_TIME(B,BARRIER,GASNETI_TICKS_NOW_IFENABLED(B)-barrier_start);
  return retval;
}

/* ------------------------------------------------------------------------------------ */
/* Initialization and barrier mechanism selection */

static gasnete_coll_barrier_type_t gasnete_coll_default_barrier_type=GASNETE_COLL_BARRIER_ENVDEFAULT;

extern void gasnete_coll_barrier_init(gasnete_coll_team_t team, int barrier_type_in,
                                      gex_Rank_t *nodes, gex_Rank_t *supernodes) {
  gasnete_coll_barrier_type_t barrier_type= (gasnete_coll_barrier_type_t) barrier_type_in;
  static int envdefault_set = 0;
  
  
  if(!envdefault_set) {
    /* fetch user barrier selection */
    const char *selection = gasneti_getenv_withdefault("GASNET_BARRIER",GASNETE_BARRIER_DEFAULT);
    char tmp[255];
    char options[255];
    int i;
    for (i = 0; selection[i] && i < sizeof(tmp)-1; i++) {
      tmp[i] = toupper(selection[i]); /* normalize to uppercase */
    }
    tmp[i] = '\0';
    selection = tmp;
    options[0] = '\0';
#define GASNETE_ISBARRIER(namestr) \
((options[0]?strcat(options, ", "),(void)0:(void)0),strcat(options, namestr), \
!strcmp(selection, namestr))
    
    if(GASNETE_ISBARRIER("DISSEM")) gasnete_coll_default_barrier_type = GASNETE_COLL_BARRIER_DISSEM;
    else if(GASNETE_ISBARRIER("AMDISSEM")) gasnete_coll_default_barrier_type = GASNETE_COLL_BARRIER_AMDISSEM;
    else if(GASNETE_ISBARRIER("RDMADISSEM")) gasnete_coll_default_barrier_type = GASNETE_COLL_BARRIER_RDMADISSEM;
    else if(GASNETE_ISBARRIER("AMCENTRAL")) gasnete_coll_default_barrier_type = GASNETE_COLL_BARRIER_AMCENTRAL;
#ifdef GASNETE_BARRIER_READENV
    else {
      GASNETE_BARRIER_READENV();
    }
#endif
    if(gasnete_coll_default_barrier_type==0) {
      gasneti_fatalerror("GASNET_BARRIER=%s is not a recognized barrier mechanism. "
                         "Available mechanisms are: %s", selection, options);
    }
    
  }

  if(barrier_type == 0) barrier_type = gasnete_coll_default_barrier_type;
  
  #ifndef GASNETE_BARRIER_INIT
  /* conduit plugin to select a barrier - 
     should use GASNETE_ISBARRIER("whatever") to check if enabled, and then set the
     barrier function pointers */
  #define GASNETE_BARRIER_INIT(team, barrier_type, nodes, supernodes)
  #endif
  /*reset the barrier types*/
  team->barrier_data = NULL;
  team->barrier_notify = NULL;
  team->barrier_wait = NULL;
  team->barrier_try = NULL;
  team->barrier = &gasnete_barrier_default;
  team->barrier_result = NULL;
  GASNETE_SPLITSTATE_LEAVE(team);
  GASNETE_BARRIER_INIT(team, barrier_type, nodes, supernodes);
  if (team->barrier_notify) { /* conduit has identified a barrier mechanism */
    /*make sure that wait and try were also defined*/
    gasneti_assert(team->barrier_wait && team->barrier_try);
    return;
  } else if (barrier_type == GASNETE_COLL_BARRIER_AMCENTRAL) {
    /*we explicitly specify that we want an AM CENTRAL Barrier*/
    gasnete_amcbarrier_init(team, nodes, supernodes);
  } else if (barrier_type == GASNETE_COLL_BARRIER_AMDISSEM) {
    /*we explicitly specify that we want an AM DISSEM Barrier*/
    gasnete_amdbarrier_init(team);
  } else if ((barrier_type == GASNETE_COLL_BARRIER_RDMADISSEM) && (team == GASNET_TEAM_ALL)) {
    /* TODO: Currently only support TEAM_ALL due to static allocation of in-segment space.
             However SEGMENT_EVERYTHING and GASNETI_SUPPORTS_OUTOFSEGMENT_PUTGET both
             offer the option to use regular heap memory.
             Additionally, if TEAM_ALL doesn't require the entire block of space then
             one could imaging allocating space from it for some number of teams.
     */
    /*we explicitly specify that we want an RDMA DISSEM Barrier*/
    gasnete_rmdbarrier_init(team);
  } else if (barrier_type == GASNETE_COLL_BARRIER_DISSEM) {
    /*we specify that we want to auto-select either AMDISSEM or RDMADISSEM Barrier*/
  #if !GASNETE_USING_REF_EXTENDED_PUT
    if (team == GASNET_TEAM_ALL) {
      gasnete_rmdbarrier_init(team);
    } else
  #endif
    {
      gasnete_amdbarrier_init(team);
    }
  } else {
    /* fallback to AM DISSEM */
    gasnete_amdbarrier_init(team);
  }
}

void gasnete_barrier_init(void) {
  gex_Rank_t *supernodes = NULL;
  gasnete_coll_team_t team;
  int i;

  /* Partially initialize GASNET_TEAM_ALL to avoid circular dependency */
  team = (gasnete_coll_team_t) gasneti_calloc(1,sizeof(struct gasnete_coll_team_t_));
  gasneti_leak(team);
  team->team_id = 0;
  team->myrank = gasneti_mynode;
  team->total_ranks = gasneti_nodes;
  team->rel2act_map = (gex_Rank_t *)gasneti_malloc(sizeof(gex_Rank_t)*gasneti_nodes);
  gasneti_leak(team->rel2act_map);
  for (i=0; i<gasneti_nodes; i++)
    team->rel2act_map[i] = i;
  if (gasneti_nodes > 1) {
    unsigned int count = 0;
    for (i=1; i<gasneti_nodes; i*=2) ++count;
    team->peers.num = count;
    team->peers.fwd = gasneti_malloc(sizeof(gex_Rank_t) * count);
    gasneti_leak(team->peers.fwd);
    for (i=0; i<count; i++) {
      unsigned int dist = 1 << i;
      team->peers.fwd[i] = (gasneti_mynode + dist) % gasneti_nodes;
    }
  }

#if GASNET_PSHM
  if (gasneti_nodemap_global_count > 1) {
    unsigned int count = 0;
    for (i=1; i<gasneti_nodemap_global_count; i*=2) ++count;
    team->supernode_peers.num = count;
    team->supernode_peers.fwd = gasneti_malloc(sizeof(gex_Rank_t) * count);
    gasneti_leak(team->supernode_peers.fwd);
    for (i=0; i<count; i++) {
      unsigned int dist = 1 << i;
      unsigned int peer = (gasneti_nodemap_global_rank + dist) % gasneti_nodemap_global_count;
      team->supernode_peers.fwd[i] = gasneti_pshm_firsts[peer];
    }
  }

  team->supernode.node_count = gasneti_mysupernode.node_count;
  team->supernode.node_rank  = gasneti_mysupernode.node_rank;
  team->supernode.grp_count  = gasneti_mysupernode.grp_count;
  team->supernode.grp_rank   = gasneti_mysupernode.grp_rank;

  supernodes = gasneti_pshm_firsts;
#endif

  GASNET_TEAM_ALL = team;

  gasnete_coll_barrier_init(team, 0, team->rel2act_map, supernodes);
}

/* ------------------------------------------------------------------------------------ */
#define GASNETE_REFBARRIER_HANDLERS() \
        GASNETE_AMDBARRIER_HANDLERS(), \
        GASNETE_AMCBARRIER_HANDLERS()
/* ------------------------------------------------------------------------------------ */
/* AuxSeg setup for RDMA-dissem space */
/* Should modify to allocate for other barriers as required */

/* worst case assumption is lg(2^32 peers) * (2 phases) */

gasneti_auxseg_request_t gasnete_barr_auxseg_alloc(gasnet_seginfo_t *auxseg_info) {
  const char *barrier = gasneti_getenv_withdefault("GASNET_BARRIER",GASNETE_BARRIER_DEFAULT);
  size_t rmdbarrier_request = 64 * GASNETE_RDMABARRIER_INBOX_SZ;
  gasneti_auxseg_request_t retval;

  retval.minsz = rmdbarrier_request;
  retval.optimalsz = rmdbarrier_request;

  if (!strcmp(barrier, "RDMADISSEM")) {
    /* Nothing else to do */
  } else
#if !GASNETE_USING_REF_EXTENDED_PUT
  if (!strcmp(barrier, "DISSEM")) {
    /* Nothing else to do */
  } else
#endif
#ifdef GASNETE_CONDUIT_RDMABARRIER
  if (GASNETE_CONDUIT_RDMABARRIER(barrier, &retval)) {
    /* Nothing else to do */
  } else
#endif
  {
    retval.minsz = 0;
    retval.optimalsz = 0;
  }

  if (auxseg_info == NULL){
    return retval; /* initial query */
  }
  else if (auxseg_info[0].size) { /* auxseg granted */
    gasneti_assert(!gasnete_rdmabarrier_auxseg);
    gasnete_rdmabarrier_auxseg = gasneti_malloc(gasneti_nodes*sizeof(gasnet_seginfo_t));
    memcpy(gasnete_rdmabarrier_auxseg, auxseg_info, gasneti_nodes*sizeof(gasnet_seginfo_t));
  }

  return retval;
}
