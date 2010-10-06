/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_extended_refbarrier.c,v $
 *     $Date: 2010/05/07 03:14:25 $
 * $Revision: 1.37.2.2 $
 * Description: Reference implemetation of GASNet Barrier, using Active Messages
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef GASNETI_GASNET_EXTENDED_REFBARRIER_C
  #error This file not meant to be compiled directly - included by gasnet_extended.c
#endif

#include <limits.h>
#include <gasnet_coll_internal.h>

/*  TODO: add more reference barrier implementation options (bug 264) */

/* ------------------------------------------------------------------------------------ */
/* state shared between barrier implementations */

#if GASNETI_STATS_OR_TRACE
  gasneti_tick_t gasnete_barrier_notifytime; /* for statistical purposes */ 
#endif

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
/* 
 * GASNETI_PSHM_BARRIER: do we build the shared-memory barrier
 * GASNETI_PSHM_BARRIER_HIER: for use alone (0) or in a heirarchical barrier (1)
 */
#if !GASNET_PSHM
  /* No PSHM support: GASNETI_PSHM_BARRIER == GASNETI_PSHM_BARRIER_HIER == 0 */
  #if GASNETI_PSHM_BARRIER_HIER
    #error "GASNETI_PSHM_BARRIER_HIER non-zero but not configured for PHSM support"
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

typedef struct gasnete_coll_pshmbarrier_s {
  struct {
    int volatile two_to_phase; /* Local var alternates between 2^0 and 2^1 */
    struct gasneti_pshm_barrier_node *mynode;
    int rank;
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
    gasneti_assert(PSHM_BSTATE_TO_RESULT(_state) == _tmp_result);              \
    gasneti_atomic_set(&(_bdata)->shared->state, _state, GASNETI_ATOMIC_REL);  \
  } while(0)

GASNETI_ALWAYS_INLINE(gasnete_pshmbarrier_notify_inner)
int gasnete_pshmbarrier_notify_inner(gasnete_pshmbarrier_data_t * const pshm_bdata, int id, int flags) {
  gasneti_pshm_barrier_t * const shared_data = pshm_bdata->shared;
  struct gasneti_pshm_barrier_node * const mynode = pshm_bdata->private.mynode;
  int last;

  /* Start a new phase */
  int two_to_phase = (pshm_bdata->private.two_to_phase ^= 3); /* alternates between 01 and 10 base-2 */

  /* Record the passed flag and value */
  mynode->flags = flags;
  mynode->value = id;
  
  /* Signal my arrival - includes WMB to commit the value/flags writes */
  last = gasneti_atomic_decrement_and_test(&shared_data->counter, GASNETI_ATOMIC_REL);
  if (last) {
    /* I am last arrival */
    const struct gasneti_pshm_barrier_node *node = shared_data->node;
    const int size = shared_data->size;
    int result = GASNET_OK; /* assume success */
    int i;
  
    /* Reset counter - includes the RMB needed to ensure up-to-date reads of value/flags */
    gasneti_atomic_set(&shared_data->counter, size, GASNETI_ATOMIC_ACQ);

    /* Determine and "publish" the result */
    flags = GASNET_BARRIERFLAG_ANONYMOUS;
    for (i = 0; i < size; ++i, ++node) {
      const int flag = node->flags;
      if_pt (flag & GASNET_BARRIERFLAG_ANONYMOUS) {
        continue;
      } else if_pf (flag & GASNET_BARRIERFLAG_MISMATCH) {
        result = GASNET_ERR_BARRIER_MISMATCH; 
        flags = GASNET_BARRIERFLAG_MISMATCH;
        break;
      } else {
        const int val = node->value;
        if (flags) {
          gasneti_assert(flags == GASNET_BARRIERFLAG_ANONYMOUS);
          id = val;
          flags = 0;
        } else if (val != id) {
          result = GASNET_ERR_BARRIER_MISMATCH; 
          flags = GASNET_BARRIERFLAG_MISMATCH;
          break;
        }
      }
    }

#if GASNETI_PSHM_BARRIER_HIER
    /* Publish the results for use in hierarhical barrier */
    pshm_bdata->shared->value = id;
    pshm_bdata->shared->flags = flags;
#endif

    /* Signal the barrier w/ phase and result */
    PSHM_BSTATE_SIGNAL(pshm_bdata, result, two_to_phase);
  }

  return last;
}

GASNETI_ALWAYS_INLINE(finish_pshm_barrier)
int finish_pshm_barrier(const gasnete_pshmbarrier_data_t * const pshm_bdata, int id, int flags, gasneti_atomic_sval_t state) {
  const struct gasneti_pshm_barrier_node * const mynode = pshm_bdata->private.mynode;
  int ret = PSHM_BSTATE_TO_RESULT(state); /* default unless args mismatch those from notify */

  /* Check args for mismatch */
  if_pf((flags != mynode->flags) ||
        (!(flags & GASNET_BARRIERFLAG_ANONYMOUS) && (id != mynode->value))) {
    ret = GASNET_ERR_BARRIER_MISMATCH; 
  }

  return ret;
}

/* Poll waiting for appropriate done bit in "state"
 * Returns GASNET_{OK,ERR_BARRIER_MISMATCH}
 */
GASNETI_ALWAYS_INLINE(gasnete_pshmbarrier_wait_inner)
int gasnete_pshmbarrier_wait_inner(const gasnete_pshmbarrier_data_t * const pshm_bdata, int id, int flags, int shift) {
  const gasneti_atomic_sval_t goal = pshm_bdata->private.two_to_phase << shift;
  gasneti_atomic_t * const state_p = &pshm_bdata->shared->state;
  gasneti_atomic_sval_t state;

  gasneti_polluntil(goal & (state = gasneti_atomic_read(state_p, 0)));

  return finish_pshm_barrier(pshm_bdata, id, flags, state);
}

/* Test for appropriate done bit in "state"
 * Returns zero or non-zero (the state in pure-SMP case)
 */
GASNETI_ALWAYS_INLINE(gasnete_pshmbarrier_try_inner)
gasneti_atomic_sval_t gasnete_pshmbarrier_try_inner(const gasnete_pshmbarrier_data_t * const pshm_bdata, int shift) { 
  const gasneti_atomic_sval_t goal = pshm_bdata->private.two_to_phase << shift;
  gasneti_atomic_t * const state_p = &pshm_bdata->shared->state;
  const gasneti_atomic_sval_t state = gasneti_atomic_read(state_p, GASNETI_ATOMIC_ACQ);

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
  gasnete_pshmbarrier_data_t *pshm_bdata;
  gasneti_pshm_barrier_t *shared_data = NULL;

  if (team == GASNET_TEAM_ALL) {
    shared_data = gasneti_pshm_barrier;
  } else {
    /* TODO: non-TEAM_ALL will need to allocate storage from shared space */
    return NULL;
  }

  if (shared_data) {
    int size, rank;

    /* Find size/rank w/i my supernode, but limited to team members */
#if !GASNET_DEBUG
    if (team == GASNET_TEAM_ALL) {
      size = gasneti_nodemap_local_count;
      rank = gasneti_nodemap_local_rank;
    } else
#endif
    {
      int i;

      size = 0; rank = -1;
      for (i=0; i < team->total_ranks; i++) {
        gasnet_node_t n = GASNETE_COLL_REL2ACT(team, i);
        if (gasneti_pshm_in_supernode(n)) {
          if (n == gasneti_mynode) rank = size;
          ++size;
	  if (size == gasneti_nodemap_local_count) break;
        }
      }
      gasneti_assert((size >  0) && (size <= gasneti_nodemap_local_count));
      gasneti_assert((rank >= 0) && (rank <  gasneti_nodemap_local_count));
    }

    pshm_bdata = gasneti_malloc(sizeof(gasnete_pshmbarrier_data_t));
    pshm_bdata->private.two_to_phase = 1; /* 2^0 */
    pshm_bdata->private.rank = rank;
    pshm_bdata->private.mynode = &shared_data->node[rank];

    pshm_bdata->shared = shared_data;

    /* One node initializes shared data */
    if (!rank) {
      /* Flags word to poll or spin on until barrier is done */
      gasneti_atomic_set(&shared_data->state, 0, 0);

      /* Counter used to detect that all nodes have reached the barrier */
      shared_data->size = size;
      gasneti_atomic_set(&shared_data->counter, shared_data->size, 0);
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

static int gasnete_node_pair_sort_fn(const void *a_p, const void *b_p) {
  const int a0 = ((const gasnet_node_t *)a_p)[0];
  const int b0 = ((const gasnet_node_t *)b_p)[0];
  const int d0 = (a0 - b0); /* sort first by supernode */
  if (d0) return d0;
  else {
    const int a1 = ((const gasnet_node_t *)a_p)[1];
    const int b1 = ((const gasnet_node_t *)b_p)[1];
    /* break ties by node - must be increasing order because
     * we use local rank to determine the active node
     */
    return (a1 - b1);
  }
}

static gasnete_pshmbarrier_data_t *
gasnete_pshmbarrier_init_hier(gasnete_coll_team_t team, int *size_p, int *rank_p, gasnet_node_t **reps_p) {
  gasnete_pshmbarrier_data_t * const pshm_bdata = gasnete_pshmbarrier_init_inner(team);

  if (pshm_bdata) {
    const int total_ranks = team->total_ranks;
    int size, rank;
    gasnet_node_t *reps;

#if !GASNET_DEBUG
    if (team == GASNET_TEAM_ALL) {
      size = gasneti_nodemap_global_count;
      rank = gasneti_nodemap_global_rank;
      reps = NULL;
    } else
#endif
    {
      gasnet_node_t *node_vector = gasneti_malloc(2 * total_ranks * sizeof(gasnet_node_t));
      int i;

      /* Created a sorted vector of (supernode,node) for members of this team */
      for (i = 0; i < total_ranks; ++i) {
        gasnet_node_t n = GASNETE_COLL_REL2ACT(team, i);
        node_vector[2*i+0] = gasneti_pshm_node2supernode(n);
        node_vector[2*i+1] = n;
      }
      qsort(node_vector, total_ranks, 2*sizeof(gasnet_node_t), &gasnete_node_pair_sort_fn);

      /* Count unique entries and find my supernode's rank */
      size = 1; rank = 0;
      for (i = 1; i < total_ranks; ++i) {
        if (node_vector[2*i] != node_vector[2*(i-1)]) {
          if (node_vector[2*i] == gasneti_pshm_mysupernode) rank = size;
          ++size;
          /* dirty (clever?) hack warning:
           * To avoid a second pass after counting, we overwrite node_vector
           * with the node numbers of representatives.  Note that initially
           * node_vector[1] already contains the first representative.
           */
          gasneti_assert(size <= 2*i);
          node_vector[size] = node_vector[2*i+1];
        }
      }
      gasneti_assert((size >  0) && (size <= gasneti_nodemap_global_count));
      gasneti_assert((rank >= 0) && (rank <  gasneti_nodemap_global_count));

      /* Extract from node_vector a representative for each supernode */
      {
        size_t alloc_size = size * sizeof(gasnet_node_t); 
        reps = gasneti_malloc(alloc_size);
        memcpy(reps, node_vector+1, alloc_size);
      }

      gasneti_free(node_vector);
    }

    *size_p = size;
    *rank_p = rank;
    *reps_p = reps;
  }

  return pshm_bdata;
}
#endif /* GASNETI_PSHM_BARRIER_HIER */

#if !GASNETI_PSHM_BARRIER_HIER
/* Entry points for SMP-conduit */

static void gasnete_pshmbarrier_notify(gasnete_coll_team_t team, int id, int flags) {
  gasneti_sync_reads();
  if_pf (team->barrier_splitstate == INSIDE_BARRIER) {
    gasneti_fatalerror("gasnet_barrier_notify() called twice in a row");
  } 

  (void)gasnete_pshmbarrier_notify_inner(team->barrier_data, id, flags);
  
  /* No sync_writes() needed due to REL in dec-and-test inside notify_inner */
  team->barrier_splitstate = INSIDE_BARRIER; 
}

static int gasnete_pshmbarrier_wait(gasnete_coll_team_t team, int id, int flags) {
  gasneti_sync_reads();
  if_pf (team->barrier_splitstate == OUTSIDE_BARRIER) {
    gasneti_fatalerror("gasnet_barrier_wait() called without a matching notify");
  }

  {
    const int result = gasnete_pshmbarrier_wait_inner(team->barrier_data, id, flags, 0);
    gasneti_assert(result != GASNET_ERR_NOT_READY);

    team->barrier_splitstate = OUTSIDE_BARRIER;
    gasneti_sync_writes();
    return result;
  }
}

static int gasnete_pshmbarrier_try(gasnete_coll_team_t team, int id, int flags) { 
  gasneti_sync_reads();
  if_pf (team->barrier_splitstate == OUTSIDE_BARRIER) {
    gasneti_fatalerror("gasnet_barrier_try() called without a matching notify");
  }

  {
    const gasneti_atomic_sval_t state = gasnete_pshmbarrier_try_inner(team->barrier_data, 0);
    int result;

    if (state) {
      result = finish_pshm_barrier(team->barrier_data, id, flags, state);

      team->barrier_splitstate = OUTSIDE_BARRIER;
      gasneti_sync_writes();
    } else {
      result = GASNET_ERR_NOT_READY;
    }
    return result;
  }
}

static void gasnete_pshmbarrier_init(gasnete_coll_team_t team) {
  team->barrier_data = (void *)gasnete_pshmbarrier_init_inner(team);

  team->barrier_splitstate = OUTSIDE_BARRIER;

  team->barrier_notify = &gasnete_pshmbarrier_notify;
  team->barrier_wait =   &gasnete_pshmbarrier_wait;
  team->barrier_try =    &gasnete_pshmbarrier_try;
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
  gasnet_hsl_t amdbarrier_lock;
  gasnet_node_t *amdbarrier_peers; /* precomputed list of peers to communicate with */
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
  int volatile amdbarrier_mismatch[2];   /*  non-zero if we detected a mismatch */
  int volatile amdbarrier_recv_value[2]; /*  consensus ambarrier value */
  int volatile amdbarrier_recv_value_present[2]; /*  consensus ambarrier value is present */
} gasnete_coll_amdbarrier_t;
  
static void gasnete_amdbarrier_notify_reqh(gasnet_token_t token, 
                                           gasnet_handlerarg_t teamid, gasnet_handlerarg_t phase, gasnet_handlerarg_t step, gasnet_handlerarg_t value, gasnet_handlerarg_t flags) {
  gasnete_coll_team_t team = gasnete_coll_team_lookup((uint32_t)teamid);
  gasnete_coll_amdbarrier_t *barrier_data = team->barrier_data;

  gasnet_hsl_lock(&barrier_data->amdbarrier_lock);
  { 
    /* Note we might not receive the steps in the numbered order.
     * We record the value received on the first one to actually arrive.
     * In subsequent steps we check for mismatch of received values.
     * The local value is compared in the kick function.
     */
    if (!(flags & (GASNET_BARRIERFLAG_ANONYMOUS|GASNET_BARRIERFLAG_MISMATCH)) && 
        !barrier_data->amdbarrier_recv_value_present[phase]) {  /* first named value we've seen */
      barrier_data->amdbarrier_recv_value[phase] = (int)value;
      gasneti_sync_writes();
      barrier_data->amdbarrier_recv_value_present[phase] = 1;
    } else if ((flags & GASNET_BARRIERFLAG_MISMATCH) || /* explicit mismatch */
               (!(flags & GASNET_BARRIERFLAG_ANONYMOUS) && /* 2nd+ named value and mismatch */
                 barrier_data->amdbarrier_recv_value[phase] != (int)value)) {
      barrier_data->amdbarrier_mismatch[phase] = 1;
    }
    
    gasneti_assert(barrier_data->amdbarrier_step_done[phase][step] == 0);
    
    gasneti_sync_writes();
    barrier_data->amdbarrier_step_done[phase][step] = 1;
  }
  gasnet_hsl_unlock(&barrier_data->amdbarrier_lock);
}

/* For a rmb() between unlocked reads of _recv_value_present and _recv_value
 * Equivalent to ``(gasneti_sync_reads(), ambarrier_recv_value[phase])'',
 * except w/o assuming gasneti_sync_reads() to be valid in expression context.
 */
GASNETI_INLINE(amdbarrier_recv_value_synced)
int amdbarrier_recv_value_synced(gasnete_coll_team_t team, int phase) {
  gasnete_coll_amdbarrier_t *barrier_data = team->barrier_data;
  gasneti_sync_reads();
  return barrier_data->amdbarrier_recv_value[phase];
}

void gasnete_amdbarrier_kick(gasnete_coll_team_t team) {
  gasnete_coll_amdbarrier_t *barrier_data = team->barrier_data;
  int phase = barrier_data->amdbarrier_phase;
  int step = barrier_data->amdbarrier_step;
  int numsteps = 0;
  gasnet_handlerarg_t flags, value;

  if (step == barrier_data->amdbarrier_size || !barrier_data->amdbarrier_step_done[phase][step]) 
    return; /* nothing to do */

  gasneti_assert(team->total_ranks > 1);

  gasnet_hsl_lock(&barrier_data->amdbarrier_lock);
    phase = barrier_data->amdbarrier_phase;
    step = barrier_data->amdbarrier_step;
    /* count steps we can take while holding the lock - must release before send,
       so coalesce as many as possible in one acquisition */
    while (step+numsteps < barrier_data->amdbarrier_size && barrier_data->amdbarrier_step_done[phase][step+numsteps]) {
      numsteps++;
    }

#if GASNETI_PSHM_BARRIER_HIER
    if (barrier_data->amdbarrier_pshm) {
      const PSHM_BDATA_DECL(pshm_bdata, barrier_data->amdbarrier_pshm);
      if (!step) {
        /* Must use supernode's consensus for value and flags */
        const int passive_shift = barrier_data->amdbarrier_passive;
        if (gasnete_pshmbarrier_try_inner(pshm_bdata, passive_shift)) {
          barrier_data->amdbarrier_value = pshm_bdata->shared->value;
          barrier_data->amdbarrier_flags = pshm_bdata->shared->flags;
        } else {
          /* not yet safe to make progress */
          numsteps = 0;
        }
      }
    }
#endif

    if (numsteps) { /* completed one or more steps */
      gasneti_sync_reads(); /* between unlocked reads of _step_done and _mismatch */
      if_pf (barrier_data->amdbarrier_mismatch[phase] ||
	     ((barrier_data->amdbarrier_flags == 0) && 
	      barrier_data->amdbarrier_recv_value_present[phase] &&
	      (amdbarrier_recv_value_synced(team, phase) != barrier_data->amdbarrier_value))) {
        barrier_data->amdbarrier_flags = GASNET_BARRIERFLAG_MISMATCH;
        barrier_data->amdbarrier_mismatch[phase] = 1;
      }
      if (step+numsteps == barrier_data->amdbarrier_size) { /* We got the last recv - barrier locally complete */
        gasnete_barrier_pf_disable(team);
        gasneti_sync_writes(); /* flush state before the write to ambarrier_step below */
      } 
      if (step + 1 < barrier_data->amdbarrier_size) {
        /* we will send at least one message - so calculate args */
        if ((barrier_data->amdbarrier_flags & GASNET_BARRIERFLAG_ANONYMOUS) &&
	    barrier_data->amdbarrier_recv_value_present[phase]) {
	  /* If we are on an node with an anonymous barrier invocation we
	   * may have received a barrier name from another node.  If so we
	   * must forward it to allow for matching tests.
	   */
	  gasneti_sync_reads(); /* Between unlocked reads of _recv_value_present and _recv_value */
	  flags = 0;
	  value = barrier_data->amdbarrier_recv_value[phase];
        } else {
	  value = barrier_data->amdbarrier_value;
	  flags = barrier_data->amdbarrier_flags;
        }
      }
      /* notify all threads of the step increase - 
         this may allow other local threads to proceed on the barrier and even indicate
         barrier completion while we overlap outgoing notifications to other nodes
      */
      barrier_data->amdbarrier_step = step+numsteps;
    } 
  gasnet_hsl_unlock(&barrier_data->amdbarrier_lock);

  for ( ; numsteps; numsteps--) {
    step++;
    if (step == barrier_data->amdbarrier_size) { /* no send upon reaching last step */
      gasneti_assert(numsteps == 1);
      break;
    }

    GASNETI_SAFE(
      gasnet_AMRequestShort5(barrier_data->amdbarrier_peers[step],
                             gasneti_handleridx(gasnete_amdbarrier_notify_reqh), 
                             team->team_id, phase, step, value, flags));
  }
}

static void gasnete_amdbarrier_notify(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_amdbarrier_t *barrier_data = team->barrier_data;
  int do_send = 1;
  int phase;
  
  gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
  if_pf(team->barrier_splitstate == INSIDE_BARRIER) 
    gasneti_fatalerror("gasnet_barrier_notify() called twice in a row");

#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->amdbarrier_pshm) {
    PSHM_BDATA_DECL(pshm_bdata, barrier_data->amdbarrier_pshm);
    if (gasnete_pshmbarrier_notify_inner(pshm_bdata, id, flags)) {
      /* last arrival - send AM w/ supernode consensus value/flags */
      id = pshm_bdata->shared->value;
      flags = pshm_bdata->shared->flags;
    } else {
      /* Not the last arrival - don't send an AM */
      do_send = 0;
    }
  }
#endif

  /* If we are on an ILP64 platform, this cast will ensure we truncate the same
   * bits locally as we do when passing over the network.
   */
  barrier_data->amdbarrier_value = (gasnet_handlerarg_t)id;

  phase = !barrier_data->amdbarrier_phase; /*  enter new phase */
  if_pf (flags & GASNET_BARRIERFLAG_MISMATCH) {
    barrier_data->amdbarrier_mismatch[phase] = 1;
    flags = GASNET_BARRIERFLAG_MISMATCH;
  }
  barrier_data->amdbarrier_flags = flags;
  barrier_data->amdbarrier_step = 0;
  gasneti_sync_writes(); 
  barrier_data->amdbarrier_phase = phase;

  if (barrier_data->amdbarrier_size) {
    /*  (possibly) send notify msg to peer */
    if (do_send) GASNETI_SAFE(
      gasnet_AMRequestShort5(barrier_data->amdbarrier_peers[0],
                             gasneti_handleridx(gasnete_amdbarrier_notify_reqh), 
                             team->team_id, phase, 0, id, flags));
#if GASNETI_PSHM_BARRIER_HIER
    if (!barrier_data->amdbarrier_passive)
#endif
      gasnete_barrier_pf_enable(team);
  } else {
    barrier_data->amdbarrier_recv_value[phase] = id;	/* to simplify checking in _wait */
  }

  /*  update state */
  team->barrier_splitstate = INSIDE_BARRIER;
  gasneti_sync_writes(); /* ensure all state changes committed before return */
}


static int gasnete_amdbarrier_wait(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_amdbarrier_t *barrier_data = team->barrier_data;
  int retval = GASNET_OK;
  int i;

  int phase;
  gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
  phase = barrier_data->amdbarrier_phase;
  if_pf(team->barrier_splitstate == OUTSIDE_BARRIER) 
    gasneti_fatalerror("gasnet_barrier_wait() called without a matching notify");

#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->amdbarrier_pshm) {
    const int passive_shift = barrier_data->amdbarrier_passive;
    retval = gasnete_pshmbarrier_wait_inner(barrier_data->amdbarrier_pshm, id, flags, passive_shift);
    if (passive_shift) {
      /* Once the active peer signals done, we can return */
      team->barrier_splitstate = OUTSIDE_BARRIER;
      gasneti_sync_writes(); /* ensure all state changes committed before return */
      return retval;
    }
  }
#endif

  if (barrier_data->amdbarrier_step == barrier_data->amdbarrier_size) { /* completed asynchronously before wait (via progressfns or try) */
    GASNETI_TRACE_EVENT_TIME(B,BARRIER_ASYNC_COMPLETION,GASNETI_TICKS_NOW_IFENABLED(B)-gasnete_barrier_notifytime);
  } else { /*  wait for response */
    GASNET_BLOCKUNTIL((gasnete_amdbarrier_kick(team), barrier_data->amdbarrier_step == barrier_data->amdbarrier_size));
  }

  /* determine return value */
  if_pf(barrier_data->amdbarrier_mismatch[phase]) {
    barrier_data->amdbarrier_mismatch[phase] = 0;
    retval = GASNET_ERR_BARRIER_MISMATCH;
  } else
#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->amdbarrier_pshm) {
    /* amdbarrier_{value,flags} may not contain this node's values
     * finish_pshm_barrier() checks local notify-vs-wait mismatch instead.
     */
  } else
#endif
  if_pf((!(flags & GASNET_BARRIERFLAG_ANONYMOUS) &&
         ((gasnet_handlerarg_t)id != barrier_data->amdbarrier_value)) || 
        flags != barrier_data->amdbarrier_flags) {
	retval = GASNET_ERR_BARRIER_MISMATCH;
  }

  /*  update state */
  team->barrier_splitstate = OUTSIDE_BARRIER;
  for (i=0; i < barrier_data->amdbarrier_size; ++i) {
    barrier_data->amdbarrier_step_done[phase][i] = 0;
  }
  barrier_data->amdbarrier_recv_value_present[phase] = 0;
#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->amdbarrier_pshm) {
    /* Signal any passive peers w/ the final result */
    const PSHM_BDATA_DECL(pshm_bdata, barrier_data->amdbarrier_pshm);
    PSHM_BSTATE_SIGNAL(pshm_bdata, retval, pshm_bdata->private.two_to_phase << 2); /* includes a WMB */
    gasneti_assert(!barrier_data->amdbarrier_passive);
  } else
#endif
  gasneti_sync_writes(); /* ensure all state changes committed before return */

  return retval;
}

static int gasnete_amdbarrier_try(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_amdbarrier_t *barrier_data = team->barrier_data;
  gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */

  if_pf(team->barrier_splitstate == OUTSIDE_BARRIER) 
    gasneti_fatalerror("gasnet_barrier_try() called without a matching notify");

  GASNETI_SAFE(gasneti_AMPoll());
  gasnete_amdbarrier_kick(team);

#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->amdbarrier_pshm) {
    const int passive_shift = barrier_data->amdbarrier_passive;
    if (!gasnete_pshmbarrier_try_inner(barrier_data->amdbarrier_pshm, passive_shift))
      return GASNET_ERR_NOT_READY;
    if (passive_shift)
      return gasnete_amdbarrier_wait(team, id, flags);
  }
#endif

  if (barrier_data->amdbarrier_step == barrier_data->amdbarrier_size) return gasnete_amdbarrier_wait(team, id, flags);
  else return GASNET_ERR_NOT_READY;
}

void gasnete_amdbarrier_kick_team_all(void) {
  gasnete_amdbarrier_kick(GASNET_TEAM_ALL);
}

static void gasnete_amdbarrier_init(gasnete_coll_team_t team) {
  gasnete_coll_amdbarrier_t *barrier_data = gasneti_calloc(1,sizeof(gasnete_coll_amdbarrier_t));
  int steps;
  int total_ranks = team->total_ranks;
  int myrank = team->myrank;
  int64_t j;

#if GASNETI_PSHM_BARRIER_HIER
  gasnet_node_t *supernode_reps = NULL;
  PSHM_BDATA_DECL(pshm_bdata, gasnete_pshmbarrier_init_hier(team, &total_ranks, &myrank, &supernode_reps));

  if (pshm_bdata) {
    barrier_data->amdbarrier_passive = (pshm_bdata->private.rank != 0) ? 2 : 0; /* precompute shift */
    barrier_data->amdbarrier_pshm = pshm_bdata;
  }
#endif

  team->barrier_data = barrier_data;
  gasnet_hsl_init(&barrier_data->amdbarrier_lock);
  team->barrier_splitstate = OUTSIDE_BARRIER;

  /* determine barrier size (number of steps) */
  for (steps=0, j=1; j < total_ranks; ++steps, j*=2) ;

  barrier_data->amdbarrier_size = steps;
  gasneti_assert(barrier_data->amdbarrier_size <= GASNETE_AMDBARRIER_MAXSTEP);

  if (steps) {
#if GASNETI_PSHM_BARRIER_HIER
    gasnet_node_t *nodes = supernode_reps ? supernode_reps : gasneti_pshm_firsts;
#endif
    int step;

    barrier_data->amdbarrier_peers = gasneti_calloc(steps, sizeof(gasnet_node_t));
  
    for (step = 0; step < steps; ++step) {
      /* No need for a full mod because worst case is < 2*team->total_ranks.
       * However, we must take care for overflow if we try to do the
       * arithmetic in gasnet_node_t.  An example is gasnet_node_t
       * of uint8_t and team->total_ranks=250 nodes.  The largest value of
       * gasnet_mynode is 249 and the largest value of 2^step is 128.
       * We can't compute (249 + 128) mod 250 in 8-bit arithmetic.
       * If we are using GASNET_MAXNODES <= INT_MAX then we can
       * fit the arithmetic into unsigned integers (32-bit example is
       * 0x7ffffffe + 0x40000000 = 0xbffffffe).  Otherwise we are
       * confident that 64-bit integers are ALWAYS large enough.
       */
    #if (GASNET_MAXNODES <= INT_MAX)
      unsigned int tmp;
    #else
      uint64_t tmp;
    #endif
      gasnet_node_t peer;

      tmp = (1 << step) + myrank;
      peer = (tmp >= total_ranks) ? (tmp - total_ranks) : tmp;
      gasneti_assert(peer < total_ranks);

#if GASNETI_PSHM_BARRIER_HIER
      if (pshm_bdata) {
        barrier_data->amdbarrier_peers[step] = nodes[peer];
      } else
#endif
      barrier_data->amdbarrier_peers[step] = GASNETE_COLL_REL2ACT(team, peer);
    }
  }

#if GASNETI_PSHM_BARRIER_HIER
  gasneti_free(supernode_reps);

  if (pshm_bdata && (pshm_bdata->shared->size == 1)) {
    /* With singlton proc on local supernode we can short-cut the PHSM code.
     * This does not require alteration of the amdbarrier_peers[] contructed above
     */
    gasnete_pshmbarrier_fini_inner(pshm_bdata);
    barrier_data->amdbarrier_pshm = NULL;
  }
#endif

  team->barrier_notify = &gasnete_amdbarrier_notify;
  team->barrier_wait =   &gasnete_amdbarrier_wait;
  team->barrier_try =    &gasnete_amdbarrier_try;
  team->barrier_pf =     (team == GASNET_TEAM_ALL) ? &gasnete_amdbarrier_kick_team_all : NULL;
}

#define GASNETE_AMDBARRIER_HANDLERS()                                 \
  gasneti_handler_tableentry_no_bits(gasnete_amdbarrier_notify_reqh)

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
  int volatile amcbarrier_value; /*  local ambarrier value */
  int volatile amcbarrier_flags; /*  local ambarrier flags */
  int volatile amcbarrier_phase; /*  2-phase operation to improve pipelining */
  int volatile amcbarrier_response_done[2];     /*  non-zero when ambarrier is complete */
  int volatile amcbarrier_response_mismatch[2]; /*  non-zero if we detected a mismatch */
  
  int           amcbarrier_max;
  gasnet_node_t amcbarrier_master; /* ACT, not REL */

#if GASNETI_PSHM_BARRIER_HIER
  gasnete_pshmbarrier_data_t *amcbarrier_pshm; /* non-NULL if using hierarchical code */
  gasnet_node_t *amcbarrier_active;/* nodes (ACT) that need to recv broadcast */
  int amcbarrier_passive;          /* 2 if some other node makes progress for me, 0 otherwise */
#endif
  /*  global state on master */
  gasnet_hsl_t amcbarrier_lock;
  int volatile amcbarrier_consensus_value[2]; /*  consensus ambarrier value */
  int volatile amcbarrier_consensus_value_present[2]; /*  consensus ambarrier value found */
  int volatile amcbarrier_consensus_mismatch[2]; /*  non-zero if we detected a mismatch */
  int volatile amcbarrier_count[2];/*  count of how many remotes have notified (on master) */
} gasnete_coll_amcbarrier_t;


static void gasnete_amcbarrier_notify_reqh(gasnet_token_t token, 
                                           gasnet_handlerarg_t teamid, gasnet_handlerarg_t phase, gasnet_handlerarg_t value, gasnet_handlerarg_t flags) {
  gasnete_coll_team_t team = gasnete_coll_team_lookup((uint32_t)teamid);
  gasnete_coll_amcbarrier_t *barrier_data = team->barrier_data;

  gasneti_assert(gasneti_mynode == barrier_data->amcbarrier_master);
  
  gasnet_hsl_lock(&barrier_data->amcbarrier_lock);
  { int count = barrier_data->amcbarrier_count[phase];
    if (!(flags & (GASNET_BARRIERFLAG_ANONYMOUS|GASNET_BARRIERFLAG_MISMATCH)) && 
        !barrier_data->amcbarrier_consensus_value_present[phase]) {
      barrier_data->amcbarrier_consensus_value[phase] = (int)value;
      barrier_data->amcbarrier_consensus_value_present[phase] = 1;
    } else if ((flags & GASNET_BARRIERFLAG_MISMATCH) ||
               (!(flags & GASNET_BARRIERFLAG_ANONYMOUS) && 
                barrier_data->amcbarrier_consensus_value[phase] != (int)value)) {
      barrier_data->amcbarrier_consensus_mismatch[phase] = 1;
    }
    count++;
    if (count == barrier_data->amcbarrier_max) gasneti_sync_writes(); /* about to signal, ensure we flush state */
    barrier_data->amcbarrier_count[phase] = count;
  }
  gasnet_hsl_unlock(&barrier_data->amcbarrier_lock);
}

static void gasnete_amcbarrier_done_reqh(gasnet_token_t token, 
  gasnet_handlerarg_t teamid, gasnet_handlerarg_t phase,  gasnet_handlerarg_t mismatch) {
  gasnete_coll_team_t team = gasnete_coll_team_lookup((uint32_t)teamid);
  gasnete_coll_amcbarrier_t *barrier_data = team->barrier_data;

#if GASNETI_PSHM_BARRIER_HIER
  gasneti_assert(!barrier_data->amcbarrier_passive);
#endif
  gasneti_assert(phase == barrier_data->amcbarrier_phase);

  barrier_data->amcbarrier_response_mismatch[phase] = mismatch;
  gasneti_sync_writes();
  barrier_data->amcbarrier_response_done[phase] = 1;
}

/*  make some progress on the ambarrier */
void gasnete_amcbarrier_kick(gasnete_coll_team_t team) {
  gasnete_coll_amcbarrier_t *barrier_data = team->barrier_data;
  int phase = barrier_data->amcbarrier_phase;

  if (gasneti_mynode != barrier_data->amcbarrier_master) return;

  /*  master does all the work */
  if (barrier_data->amcbarrier_count[phase] == barrier_data->amcbarrier_max) {
    int gotit = 0;
    gasnet_hsl_lock(&barrier_data->amcbarrier_lock);
      if (barrier_data->amcbarrier_count[phase] == barrier_data->amcbarrier_max) {
        barrier_data->amcbarrier_count[phase] = 0;
        gotit = 1;
      }
    gasnet_hsl_unlock(&barrier_data->amcbarrier_lock);

    if (gotit) { /*  ambarrier is complete */
      int i;
      int mismatch = barrier_data->amcbarrier_consensus_mismatch[phase];

      gasnete_barrier_pf_disable(team);

      /*  inform the nodes */
#if GASNETI_PSHM_BARRIER_HIER
      if (barrier_data->amcbarrier_active) {
        for (i=0; i < barrier_data->amcbarrier_max; i++) {
          GASNETI_SAFE(
            gasnet_AMRequestShort3(barrier_data->amcbarrier_active[i],
                                   gasneti_handleridx(gasnete_amcbarrier_done_reqh), 
                                   team->team_id, phase, mismatch));
        }
      } else
#endif
      for (i=0; i < team->total_ranks; i++) {
        GASNETI_SAFE(
          gasnet_AMRequestShort3(GASNETE_COLL_REL2ACT(team, i), gasneti_handleridx(gasnete_amcbarrier_done_reqh), 
                                 team->team_id, phase, mismatch));
      }

      /*  reset state */
      barrier_data->amcbarrier_consensus_mismatch[phase] = 0;
      barrier_data->amcbarrier_consensus_value_present[phase] = 0;
    }
  }
}

static void gasnete_amcbarrier_notify(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_amcbarrier_t *barrier_data = team->barrier_data;
  int do_send = 1;
  int phase;

  gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
  if_pf(team->barrier_splitstate == INSIDE_BARRIER) 
    gasneti_fatalerror("gasnet_barrier_notify() called twice in a row");

#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->amcbarrier_pshm) {
    PSHM_BDATA_DECL(pshm_bdata, barrier_data->amcbarrier_pshm);
    if (gasnete_pshmbarrier_notify_inner(pshm_bdata, id, flags)) {
      /* last arrival - send AM w/ supernode consensus value/flags */
      id = pshm_bdata->shared->value;
      flags = pshm_bdata->shared->flags;
    } else {
      /* Not the last arrival - don't send an AM */
      do_send = 0;
    }
  }
#endif

  /* If we are on an ILP64 platform, this cast will ensure we truncate the same
   * bits locally as we do when passing over the network.
   */
  barrier_data->amcbarrier_value = (gasnet_handlerarg_t)id;

  barrier_data->amcbarrier_flags = flags;
  phase = !barrier_data->amcbarrier_phase; /*  enter new phase */
  barrier_data->amcbarrier_phase = phase;

  if (barrier_data->amcbarrier_max > 1) {
    /*  send notify msg to master */
    if (do_send) GASNETI_SAFE(
      gasnet_AMRequestShort4(barrier_data->amcbarrier_master,
                             gasneti_handleridx(gasnete_amcbarrier_notify_reqh), 
                             team->team_id, phase, barrier_data->amcbarrier_value, flags));
    if (gasneti_mynode == barrier_data->amcbarrier_master) gasnete_barrier_pf_enable(team);
  } else {
    barrier_data->amcbarrier_response_mismatch[phase] = (flags & GASNET_BARRIERFLAG_MISMATCH);
    barrier_data->amcbarrier_response_done[phase] = 1;
  }

  /*  update state */
  team->barrier_splitstate = INSIDE_BARRIER;
  gasneti_sync_writes(); /* ensure all state changes committed before return */
}

static int gasnete_amcbarrier_wait(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_amcbarrier_t *barrier_data = team->barrier_data;
  int retval = GASNET_OK;
  int phase;

  gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
  phase = barrier_data->amcbarrier_phase;
  if_pf(team->barrier_splitstate == OUTSIDE_BARRIER) 
    gasneti_fatalerror("gasnet_barrier_wait() called without a matching notify");

#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->amcbarrier_pshm) {
    const int passive_shift = barrier_data->amcbarrier_passive;
    retval = gasnete_pshmbarrier_wait_inner(barrier_data->amcbarrier_pshm, id, flags, passive_shift);
    if (passive_shift) {
      /* Once the active peer signals done, we can return */
      team->barrier_splitstate = OUTSIDE_BARRIER;
      gasneti_sync_writes(); /* ensure all state changes committed before return */
      return retval;
    }
  }
#endif

  if (barrier_data->amcbarrier_response_done[phase]) { /* completed asynchronously before wait (via progressfns or try) */
    GASNETI_TRACE_EVENT_TIME(B,BARRIER_ASYNC_COMPLETION,GASNETI_TICKS_NOW_IFENABLED(B)-gasnete_barrier_notifytime);
  } else { /*  wait for response */
    GASNET_BLOCKUNTIL((gasnete_amcbarrier_kick(team), barrier_data->amcbarrier_response_done[phase]));
  }

  /* determine result */
  if_pf(barrier_data->amcbarrier_response_mismatch[phase]) {
    barrier_data->amcbarrier_response_mismatch[phase] = 0;
    retval = GASNET_ERR_BARRIER_MISMATCH;
  } else
#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->amcbarrier_pshm) {
    /* amcbarrier_{value,flags} may not contain this node's values
     * finish_pshm_barrier() checks local notify-vs-wait mismatch instead.
     */
  } else
#endif
  if_pf((!(flags & GASNET_BARRIERFLAG_ANONYMOUS) && (gasnet_handlerarg_t)id != barrier_data->amcbarrier_value) || 
        flags != barrier_data->amcbarrier_flags) {
        retval = GASNET_ERR_BARRIER_MISMATCH;
  }

  /*  update state */
  team->barrier_splitstate = OUTSIDE_BARRIER;
  barrier_data->amcbarrier_response_done[phase] = 0;
#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->amcbarrier_pshm) {
    /* Signal any passive peers w/ the final result */
    const PSHM_BDATA_DECL(pshm_bdata, barrier_data->amcbarrier_pshm);
    PSHM_BSTATE_SIGNAL(pshm_bdata, retval, pshm_bdata->private.two_to_phase << 2); /* includes a WMB */
    gasneti_assert(!barrier_data->amcbarrier_passive);
  } else
#endif
  gasneti_sync_writes(); /* ensure all state changes committed before return */
  
  return retval;
}

static int gasnete_amcbarrier_try(gasnete_coll_team_t team, int id, int flags) {
  gasnete_coll_amcbarrier_t *barrier_data = team->barrier_data;

  gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
  if_pf(team->barrier_splitstate == OUTSIDE_BARRIER) 
    gasneti_fatalerror("gasnet_barrier_try() called without a matching notify");

  GASNETI_SAFE(gasneti_AMPoll());
  gasnete_amcbarrier_kick(team);

#if GASNETI_PSHM_BARRIER_HIER
  if (barrier_data->amcbarrier_pshm) {
    const int passive_shift = barrier_data->amcbarrier_passive;
    if (!gasnete_pshmbarrier_try_inner(barrier_data->amcbarrier_pshm, passive_shift))
      return GASNET_ERR_NOT_READY;
    if (passive_shift)
      return gasnete_amcbarrier_wait(team, id, flags);
  }
#endif

  if (barrier_data->amcbarrier_response_done[barrier_data->amcbarrier_phase]) return gasnete_amcbarrier_wait(team, id, flags);
  else return GASNET_ERR_NOT_READY;
}

void gasnete_amcbarrier_kick_team_all(void) {
  gasnete_amcbarrier_kick(GASNET_TEAM_ALL);
}

static void gasnete_amcbarrier_init(gasnete_coll_team_t team) {
  gasnete_coll_amcbarrier_t *barrier_data = gasneti_calloc(1,sizeof(gasnete_coll_amcbarrier_t));
  int total_ranks = team->total_ranks;
  int myrank = team->myrank;

#if GASNETI_PSHM_BARRIER_HIER
  gasnet_node_t *supernode_reps = NULL;
  PSHM_BDATA_DECL(pshm_bdata, gasnete_pshmbarrier_init_hier(team, &total_ranks, &myrank, &supernode_reps));

  if (pshm_bdata) {
    barrier_data->amcbarrier_passive = (pshm_bdata->private.rank != 0) ? 2 : 0; /* precompute shift */
    barrier_data->amcbarrier_pshm = pshm_bdata;
  }
#endif

  gasnet_hsl_init(&barrier_data->amcbarrier_lock);

  barrier_data->amcbarrier_max = total_ranks;
#if GASNETI_PSHM_BARRIER_HIER
  if (pshm_bdata) {
    gasnet_node_t *nodes = supernode_reps ? supernode_reps : gasneti_pshm_firsts;
    barrier_data->amcbarrier_master = nodes[total_ranks-1];
    barrier_data->amcbarrier_active = nodes;
  } else
#endif
  barrier_data->amcbarrier_master = GASNETE_COLL_REL2ACT(team, (total_ranks - 1));

#if GASNETI_PSHM_BARRIER_HIER
  if (pshm_bdata && (pshm_bdata->shared->size == 1)) {
    /* With singleton proc on local supernode we can short-cut the PHSM code.
     * This does not require changing the amcbarrier_master selected above.
     */
    gasnete_pshmbarrier_fini_inner(pshm_bdata);
    barrier_data->amcbarrier_pshm = NULL;
  }
#endif

  team->barrier_splitstate = OUTSIDE_BARRIER;
  team->barrier_data =   barrier_data;
  team->barrier_notify = &gasnete_amcbarrier_notify;
  team->barrier_wait =   &gasnete_amcbarrier_wait;
  team->barrier_try =    &gasnete_amcbarrier_try;
  team->barrier_pf =     (team == GASNET_TEAM_ALL) ? &gasnete_amcbarrier_kick_team_all : NULL;
}

#define GASNETE_AMCBARRIER_HANDLERS()                                 \
  gasneti_handler_tableentry_no_bits(gasnete_amcbarrier_notify_reqh), \
  gasneti_handler_tableentry_no_bits(gasnete_amcbarrier_done_reqh)  

/* ------------------------------------------------------------------------------------ */
/* Initialization and barrier mechanism selection */

static gasnete_coll_barrier_type_t gasnete_coll_default_barrier_type=GASNETE_COLL_BARRIER_ENVDEFAULT;

GASNETI_INLINE(gasnete_coll_barrier_notify_internal)
void gasnete_coll_barrier_notify_internal(gasnete_coll_team_t team, int id, int flags GASNETE_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  gasneti_assert(team->barrier_notify);
#if GASNET_PAR
  if(flags & GASNET_BARRIERFLAG_IMAGES) {
    if(team->total_ranks >1) smp_coll_barrier(td->smp_coll_handle, 0);
    if(td->my_local_image == 0) (*team->barrier_notify)(team, id, flags);
  }  else 
#endif
    (*team->barrier_notify)(team, id, flags);  
}

GASNETI_INLINE(gasnete_coll_barrier_try_internal)
int gasnete_coll_barrier_try_internal(gasnete_coll_team_t team, int id, int flags GASNETE_THREAD_FARG) {
  int ret;
  gasneti_assert(team->barrier_try);
  

  /* currently there's no try version of the smp_coll_barriers*/
  /* so the try is not yet supported over the images*/
  gasneti_assert(!(flags & GASNET_BARRIERFLAG_IMAGES));
#if GASNET_PAR && 0
  {
    gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
    if(td->my_local_image == 0) ret =  (*team->barrier_try)(team, id, flags);
    /*if the barrier has succeeded then call the local smp barrier on the way out*/
    /*if there is exactly one gasnet_node then the barrier on the notify is sufficient*/
    if(flags & GASNET_BARRIERFLAG_IMAGES && team->total_ranks > 1 && ret == GASNET_OK) {
      smp_coll_barrier(td->smp_coll_handle, 0);
    } 
    return ret;
  }
#else
  return (*team->barrier_try)(team, id, flags);
#endif

  
}

GASNETI_INLINE(gasnete_coll_barrier_wait_internal)
int gasnete_coll_barrier_wait_internal(gasnete_coll_team_t team, int id, int flags GASNETE_THREAD_FARG) {
  int ret;
  gasneti_assert(team->barrier_wait);
  
#if GASNET_PAR 
  if(flags & GASNET_BARRIERFLAG_IMAGES){
    gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
    if(td->my_local_image == 0) ret = (*team->barrier_wait)(team, id, flags);
    else ret = GASNET_OK;
    /*if the barrier has succeeded then call the local smp barrier on the way out*/
    /*if there is exactly one gasnet_node then the barrier on the notify is sufficient*/
    if(ret == GASNET_OK) smp_coll_barrier(td->smp_coll_handle, 0);
    return ret;
  } else
#endif
    return (*team->barrier_wait)(team, id, flags);
  
}

void gasnete_coll_barrier_notify(gasnete_coll_team_t team, int id, int flags GASNETE_THREAD_FARG) {
  gasnete_coll_barrier_notify_internal(team, id, flags GASNETE_THREAD_PASS);
}

int gasnete_coll_barrier_try(gasnete_coll_team_t team, int id, int flags GASNETE_THREAD_FARG) {
  return gasnete_coll_barrier_try_internal(team, id, flags GASNETE_THREAD_PASS);
}

int gasnete_coll_barrier_wait(gasnete_coll_team_t team, int id, int flags GASNETE_THREAD_FARG) {
  return gasnete_coll_barrier_wait_internal(team, id, flags GASNETE_THREAD_PASS);
}

/*the default gasnet_barrier_* as defined by the spec must only be called amongst the nodes by ONE representative image
   client is responsible for synchronizing images
 */
void gasnet_barrier_notify(int id, int flags) {
  GASNETI_TRACE_PRINTF(B, ("BARRIER_NOTIFY(team=GASNET_TEAM_ALL,id=%i,flags=%i)", id, flags));
  #if GASNETI_STATS_OR_TRACE
    gasnete_barrier_notifytime = GASNETI_TICKS_NOW_IFENABLED(B);
  #endif

  gasneti_assert(GASNET_TEAM_ALL->barrier_notify);
  gasneti_assert(!(flags & GASNET_BARRIERFLAG_IMAGES));
  gasnete_coll_barrier_notify_internal(GASNET_TEAM_ALL, id, flags GASNETE_THREAD_GET);
}


int gasnet_barrier_wait(int id, int flags) {
  #if GASNETI_STATS_OR_TRACE
    gasneti_tick_t wait_start = GASNETI_TICKS_NOW_IFENABLED(B);
  #endif
  int retval;
  GASNETI_TRACE_EVENT_TIME(B,BARRIER_NOTIFYWAIT,GASNETI_TICKS_NOW_IFENABLED(B)-gasnete_barrier_notifytime);
  
  gasneti_assert(GASNET_TEAM_ALL->barrier_wait);
  gasneti_assert(!(flags & GASNET_BARRIERFLAG_IMAGES));
  retval = gasnete_coll_barrier_wait_internal(GASNET_TEAM_ALL, id, flags GASNETE_THREAD_GET);
 
  GASNETI_TRACE_EVENT_TIME(B,BARRIER_WAIT,GASNETI_TICKS_NOW_IFENABLED(B)-wait_start);
  return retval;
}

int gasnet_barrier_try(int id, int flags) {
  int retval;

  gasneti_assert(GASNET_TEAM_ALL->barrier_try);
  gasneti_assert(!(flags & GASNET_BARRIERFLAG_IMAGES));
  
  retval = gasnete_coll_barrier_try_internal(GASNET_TEAM_ALL, id, flags GASNETE_THREAD_GET);

  GASNETI_TRACE_EVENT_VAL(B,BARRIER_TRY,(retval != GASNET_ERR_NOT_READY));
  return retval;
}

extern void gasnete_coll_barrier_init(gasnete_coll_team_t team,  int barrier_type_in) {
#ifndef GASNETE_BARRIER_DEFAULT
  /* conduit plugin for default barrier mechanism */
#define GASNETE_BARRIER_DEFAULT "AMDISSEM"
#endif
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
    
    if(GASNETE_ISBARRIER("AMDISSEM")) gasnete_coll_default_barrier_type = GASNETE_COLL_BARRIER_AMDISSEM;
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
  if(team==NULL) { /*global barrier hasn't been initialized yet so take care of it*/
    team = GASNET_TEAM_ALL = (gasnete_coll_team_t) gasneti_calloc(1,sizeof(struct gasnete_coll_team_t_));
    team->team_id=0;
    team->myrank = gasneti_mynode;
    team->total_ranks = gasneti_nodes;
    team->team_id=0; 
  }
  
  
  if(barrier_type == 0) barrier_type = gasnete_coll_default_barrier_type;
  
  #ifndef GASNETE_BARRIER_INIT
  /* conduit plugin to select a barrier - 
     should use GASNETE_ISBARRIER("whatever") to check if enabled, and then set the
     barrier function pointers */
  #define GASNETE_BARRIER_INIT(team, barrier_type)
  #endif
  /*reset the barrier types*/
  team->barrier_data = NULL;
  team->barrier_notify = NULL;
  team->barrier_wait = NULL;
  team->barrier_try = NULL;
  GASNETE_BARRIER_INIT(team, barrier_type);
  if (team->barrier_notify) { /* conduit has identified a barrier mechanism */
    /*make sure that wait and try were also defined*/
    gasneti_assert(team->barrier_wait && team->barrier_try);
    return;
  } else if (barrier_type == GASNETE_COLL_BARRIER_AMCENTRAL) {
    /*we explicitly specify that we want an AM CENTRAL Barrier*/
    gasnete_amcbarrier_init(team);
  } else if (barrier_type == GASNETE_COLL_BARRIER_AMDISSEM) {
    /*we explicitly specify that we want an AM DISSEM Barrier*/
    gasnete_amdbarrier_init(team);
  } else {
    /* fallback to AM DISSEM */
    gasnete_amdbarrier_init(team);
  }
}
/* ------------------------------------------------------------------------------------ */
#define GASNETE_REFBARRIER_HANDLERS() \
        GASNETE_AMDBARRIER_HANDLERS(), \
        GASNETE_AMCBARRIER_HANDLERS()
