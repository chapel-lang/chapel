/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_refcoll.c $
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>, Paul H. Hargrove <PHHargrove@lbl.gov>, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */


#define GASNET_COLL_TREE_DEBUG 0

#include <coll/gasnet_coll_internal.h>
#include <coll/gasnet_team.h>
#include <coll/gasnet_autotune_internal.h>
#include <coll/gasnet_scratch.h>
#include <coll/gasnet_trees.h>
#include <coll/gasnet_refcoll.h>

/*TEMPORARY (Need to eventually change it such that 
  the files are compiled under their own .o files)*/
#include <coll/gasnet_trees.c>
/* gasnet_coll_autotune.c and gasnet_coll_scratch.c have 
   to be included after gasnet_coll_trees.c
*/
#include <coll/gasnet_autotune.c>
#include <coll/gasnet_scratch.c>

size_t gasnete_coll_p2p_eager_min = 0;
size_t gasnete_coll_p2p_eager_scale = 0;
/*set a std segment size of 1024 bytes*/

/*---------------------------------------------------------------------------------*/

/*declarations for gasnet team all*/
gasnet_team_handle_t gasnete_coll_team_all;
/*---------------------------------------------------------------------------------*/

static gasneti_mutex_t gasnete_coll_active_lock = GASNETI_MUTEX_INITIALIZER;
static gasnete_coll_op_t *gasnete_coll_active_first(void);
static gasnete_coll_op_t *gasnete_coll_active_next(gasnete_coll_op_t *op);
static void gasnete_coll_active_new(gasnete_coll_op_t *op);
static void gasnete_coll_active_ins(gasnete_coll_op_t *op);
static void gasnete_coll_active_del(gasnete_coll_op_t *op);

/*---------------------------------------------------------------------------------*/

void gasnete_coll_validate(gasnet_team_handle_t team,
                           gex_Rank_t dstrank, const void *dst, size_t dstlen,
                           gex_Rank_t srcrank, const void *src, size_t srclen,
                           int flags GASNETI_THREAD_FARG) {
  if (dstrank == GEX_RANK_INVALID) {
    dstrank = team->myrank;
  }
  if (srcrank == GEX_RANK_INVALID) {
    srcrank = team->myrank;
  }

  /* Some flags should never come the user */
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));

#if GASNET_DEBUG
  /* Validate IN sync mode */
  switch (GASNETE_COLL_IN_MODE(flags)) {
  case 0:
    gasneti_fatalerror("No GASNET_COLL_IN_*SYNC flag given");
    break;
  case GASNET_COLL_IN_NOSYNC:
  case GASNET_COLL_IN_MYSYNC:
  case GASNET_COLL_IN_ALLSYNC:
    break; /* OK */
  default:
    gasneti_fatalerror("Multiple GASNET_COLL_IN_*SYNC flags given");
    break;
  }

  /* Validate OUT sync mode */
  switch (GASNETE_COLL_OUT_MODE(flags)) {
  case 0:
    gasneti_fatalerror("No GASNET_COLL_OUT_*SYNC flag given");
    break;
  case GASNET_COLL_OUT_NOSYNC:
  case GASNET_COLL_OUT_MYSYNC:
  case GASNET_COLL_OUT_ALLSYNC:
    break; /* OK */
  default:
    gasneti_fatalerror("Multiple GASNET_COLL_OUT_*SYNC flags given");
    break;
  }
#endif

  // Only COLL_LOCAL supported
  gasneti_assert(flags & GASNET_COLL_LOCAL);

  /* Bounds check any local portion of dst which user claims is in-segment */
  gasneti_assert(dstlen > 0);
  if ((dstrank == team->myrank) && (flags & GASNET_COLL_DST_IN_SEGMENT)) {
      gasneti_boundscheck(team->e_tm, dstrank, dst, dstlen);
  }

  /* Bounds check any local portion of src which user claims is in-segment */
  gasneti_assert(srclen > 0);
  if ((srcrank == team->myrank) && (flags & GASNET_COLL_SRC_IN_SEGMENT)) {
      gasneti_boundscheck(team->e_tm, srcrank, src, srclen);
  }

  /* XXX: TO DO
   * + check that team handle is valid (requires a teams interface)
   * + check that mynode is a member of the team (requires a teams interface)
   */
}

/*---------------------------------------------------------------------------------*/
/* Handles */
#ifndef GASNETE_COLL_HANDLE_OVERRIDE

#if GASNET_PAR
#define GASNETE_COLL_REAL_EOP(e) ((e)->eop)
#define GASNETE_COLL_EOP_TO_EVENT(e) ((e) ? gasneti_eop_to_event((e)->eop) : GEX_EVENT_INVALID)
#else
#define GASNETE_COLL_REAL_EOP(e) (e)
#define GASNETE_COLL_EOP_TO_EVENT(e) gasneti_eop_to_event(e)
#endif

extern gasnete_coll_eop_t gasnete_coll_eop_create(GASNETI_THREAD_FARG_ALONE) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  gasnete_coll_eop_t result;

#if GASNET_PAR
  result = td->eop_freelist;
  if_pt (result) {
    td->eop_freelist = result->next;
  } else {
    /* XXX: allocate in large chunks and scatter across cache lines (and update gasnete_coll_cleanup_threaddata) */
    result = (gasnete_coll_eop_t)gasneti_malloc(sizeof(*result));
    GASNETI_STAT_EVENT_VAL(W, COLL_EOP_ALLOC, sizeof(*result));
  }

    result->next = NULL;
#endif

    GASNETE_COLL_REAL_EOP(result) = gasneti_eop_create(GASNETI_THREAD_PASS_ALONE);

    return result;
}

extern void gasnete_coll_eop_signal(gasnete_coll_eop_t eop GASNETI_THREAD_FARG) {
  gasneti_assert(eop != NULL);
  gasneti_mutex_assertlocked(&gasnete_coll_active_lock);
#if GASNET_PAR
  gasneti_sync_writes();
  for (gasnete_coll_eop_t curr = eop; ; curr = curr->next) {
    gasneti_eop_markdone(GASNETE_COLL_REAL_EOP(curr));
    if (! curr->next) {
      gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
      curr->next = td->eop_freelist;
      td->eop_freelist = eop;
      break;
    }
  }
#else
  gasneti_eop_markdone(eop);
#endif
}

#endif

/*---------------------------------------------------------------------------------*/
/* Code for list of events to test in progress */

gasneti_mutex_t gasnete_coll_poll_lock = GASNETI_MUTEX_INITIALIZER;

static struct {
  gex_Event_t  **addrs;
  gex_Event_t  *events;
  size_t       allocated, used;
} gasnete_coll_event_list = {NULL, NULL, 0, 0};

void gasnete_coll_save_event(gex_Event_t *event_p) {
  gasneti_mutex_assertlocked(&gasnete_coll_poll_lock);
  if (*event_p != GEX_EVENT_INVALID) {
    int allocated = gasnete_coll_event_list.allocated;
    int used = gasnete_coll_event_list.used;
    if_pf (allocated == used) {
        gasnete_coll_event_list.allocated = (allocated += 8);
        gasnete_coll_event_list.addrs  = gasneti_realloc(gasnete_coll_event_list.addrs,
                                                         allocated * sizeof(gex_Event_t*));
        gasnete_coll_event_list.events = gasneti_realloc(gasnete_coll_event_list.events,
                                                         allocated * sizeof(gex_Event_t));
    }
    gasnete_coll_event_list.used += 1;

    gasnete_coll_event_list.addrs[used]  = event_p;
    gasnete_coll_event_list.events[used] = *event_p;
  }
}

/* Assumes caller calls AMPoll */
void gasnete_coll_sync_saved_events(GASNETI_THREAD_FARG_ALONE) {
  int used = gasnete_coll_event_list.used;

  gasneti_mutex_assertlocked(&gasnete_coll_poll_lock);
  if (used) {
    gex_Event_t *events = gasnete_coll_event_list.events;
    if (! gasnete_test_some(events, used GASNETI_THREAD_PASS)) {
      gasneti_sync_writes(); // TODO-EX: is this really needed given the use of mutexes
      gex_Event_t **addrs = gasnete_coll_event_list.addrs;
      for (int i = 0; i < used; ) {
        if (events[i] == GEX_EVENT_INVALID) {
          *(addrs[i]) = GEX_EVENT_INVALID;
          used -= 1;
          addrs[i] = addrs[used];
          events[i] = events[used];
        } else {
          ++i;
        }
      }
      gasneti_assert(gasnete_coll_event_list.used != used);
      gasnete_coll_event_list.used = used;
    }
  }
}

/*---------------------------------------------------------------------------------*/
/* Synchronization for threads in PAR builds */

/* Current state:
 * In a SEQ or PARSYNC build this code compiles away.
 * In a PAR build we are in the process of removal!
 *
 * TODO-EX:
 * The following are currently no-ops, but will need multi-EP equivalents.
 * So, we retain a mutex in (only) DEBUG buils to avoid bit-rot in callers.
 */

#if GASNET_PAR && GASNET_DEBUG
  void gasnete_coll_threads_lock(gasnete_coll_team_t team, int flags GASNETI_THREAD_FARG) {
    gasneti_mutex_lock(&team->threads_mutex);
  }
  void gasnete_coll_threads_unlock(gasnete_coll_team_t team GASNETI_THREAD_FARG) {
    gasneti_mutex_unlock(&team->threads_mutex);
  }
#endif

/*---------------------------------------------------------------------------------*/
/* The list of active collective ops (coll ops) */

/* There exists a "active list".
 * Ops in the active table will be polled to make progress.
 *
 * Operations of the active list
 *   void gasnete_coll_active_init()
 *   void gasnete_coll_active_fini()
 *   gasnete_coll_op_t *gasnete_coll_active_first()
 *	Return the first coll op in the active list.
 *   gasnete_coll_op_t *gasnete_coll_active_last()
 *	Return the last coll op in the active list.
 *   gasnete_coll_op_t *gasnete_coll_active_next(op)
 *	Iterate over the coll ops in the active list.
 *   void gasnete_coll_active_new(op)
 *	Init active list fields of a coll op.
 *   void gasnete_coll_active_ins(op)
 *	Add a coll op to the active list.
 *   void gasnete_coll_active_del(op)
 *	Delete a coll op from the active list.
 *
 */

#ifndef GASNETE_COLL_LIST_OVERRIDE
/* Default implementation of coll_ops active list:
 *
 * Iteration over the active list is based on a linked list (queue).
 * Iteration starts from the head and new ops are added at the tail.
 *
 * Callers to ins and del must always hold the active lock.
 * Traversal and ins may run concurrently.
 * See comment in gasneti_coll_progressfn() for more info.
 *
 * XXX: use list macros?
 */
static gasnete_coll_op_t	*gasnete_coll_active_head;
static gasnete_coll_op_t	**gasnete_coll_active_tail_p;

static
gasnete_coll_op_t *gasnete_coll_active_first(void) {
  return gasnete_coll_active_head;
}

static
gasnete_coll_op_t *gasnete_coll_active_last(void) {
  return gasneti_container_of(gasnete_coll_active_tail_p, gasnete_coll_op_t, active_next);
}

static
gasnete_coll_op_t *gasnete_coll_active_next(gasnete_coll_op_t *op) {
  return op->active_next;
}

/* No lock needed */
static
void gasnete_coll_active_new(gasnete_coll_op_t *op) {
  op->active_next = NULL;
  op->active_prev_p = &(op->active_next);
}

static
void gasnete_coll_active_ins(gasnete_coll_op_t *op) {
  gasneti_mutex_assertlocked(&gasnete_coll_active_lock);
  if (! gasnete_coll_active_head) GASNETI_PROGRESSFNS_ENABLE(gasneti_pf_coll,BOOLEAN);
  gasneti_assert(op->active_next == NULL);
  *(gasnete_coll_active_tail_p) = op;
  op->active_prev_p = gasnete_coll_active_tail_p;
  gasnete_coll_active_tail_p = &(op->active_next);
}

static
void gasnete_coll_active_del(gasnete_coll_op_t *op) {
  gasneti_mutex_assertlocked(&gasnete_coll_active_lock);
  gasnete_coll_op_t *next = op->active_next;
  *(op->active_prev_p) = next;
  if (next) {
    next->active_prev_p = op->active_prev_p;
  } else {
    gasnete_coll_active_tail_p = op->active_prev_p;
  }
  if (! gasnete_coll_active_head) GASNETI_PROGRESSFNS_DISABLE(gasneti_pf_coll,BOOLEAN);
}

void
gasnete_coll_active_init(void) {
  gasnete_coll_active_head = NULL;
  gasnete_coll_active_tail_p = &(gasnete_coll_active_head);
}

void
gasnete_coll_active_fini(void) {
  gasneti_assert(gasnete_coll_active_head == NULL);
}
#endif

/*---------------------------------------------------------------------------------*/
static int gasnete_coll_cleanup_freelist(void **head) {
  void **next;
  int count = 0;
  while ((next = (void **)*head) != NULL) {
    *head = *next;
    gasneti_free(next);
    count++;
  }
  return count;
}
static void gasnete_coll_cleanup_threaddata(void *_td) {
  int count;

  gasnete_coll_threaddata_t *td = (gasnete_coll_threaddata_t *)_td;

  /* these free lists are all linked by initial pointer */
  count = gasnete_coll_cleanup_freelist((void **)&(td->op_freelist));
  GASNETI_STAT_EVENT_VAL(W, COLL_OP_FREE, count * sizeof(gasnete_coll_op_t));
  count = gasnete_coll_cleanup_freelist((void **)&(td->generic_data_freelist));
  GASNETI_STAT_EVENT_VAL(W, COLL_GDATA_FREE, count * sizeof(gasnete_coll_generic_data_t));

#ifndef GASNETE_COLL_HANDLE_OVERRIDE
#if GASNET_PAR
  count = 0;
  while (td->eop_freelist) {
    gasnete_coll_eop_t next = td->eop_freelist->next;
    gasneti_free((void *)td->eop_freelist);
    td->eop_freelist = next;
    count++;
  }
  GASNETI_STAT_EVENT_VAL(W, COLL_EOP_FREE, count * sizeof(gasnete_coll_eop_t));
#endif
#endif

#ifdef GASNETE_COLL_THREADDATA_EXTRA_CLEANUP
  GASNETE_COLL_THREADDATA_EXTRA_CLEANUP(td);
#endif
  gasneti_free(td);
}

GASNETI_NEVER_INLINE(gasnete_coll_new_threaddata,
extern gasnete_coll_threaddata_t *gasnete_coll_new_threaddata(void)) {
  gasnete_coll_threaddata_t *result = gasneti_calloc(1,sizeof(*result));

  gasnete_register_threadcleanup(gasnete_coll_cleanup_threaddata, result);
  return result;
}

/*---------------------------------------------------------------------------------*/
/* Active list management */

GASNETI_INLINE(gasnete_coll_op_submit)
void gasnete_coll_op_submit(gasnete_coll_op_t *op, gasnete_coll_eop_t eop GASNETI_THREAD_FARG) {  
    op->eop = eop;
    gasneti_mutex_lock(&gasnete_coll_active_lock);
    gasnete_coll_active_ins(op);
    gasneti_mutex_unlock(&gasnete_coll_active_lock);
}

GASNETI_INLINE(gasnete_coll_op_complete)
void gasnete_coll_op_complete(gasnete_coll_op_t *op, int poll_result GASNETI_THREAD_FARG) {
  gasneti_mutex_assertlocked(&gasnete_coll_active_lock);

  if (poll_result & GASNETE_COLL_OP_COMPLETE) {
    if_pt (op->eop != NULL) {
	    /* Normal case, just signal the eop */
	    gasnete_coll_eop_signal(op->eop GASNETI_THREAD_PASS);
	    op->eop = NULL;
    }
  }

  if (poll_result & GASNETE_COLL_OP_INACTIVE) {
    /* delete from the active list and destoy */
    gasnete_coll_active_del(op);
    gasnete_coll_op_destroy(op GASNETI_THREAD_PASS);
  }
}

/*---------------------------------------------------------------------------------*/
gasnete_coll_op_t *
gasnete_coll_op_create(gasnete_coll_team_t team, uint32_t sequence, int flags GASNETI_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  gasnete_coll_op_t *op;

  op = td->op_freelist;
  if_pt (op != NULL) {
    td->op_freelist = *((gasnete_coll_op_t **)op);
  } else {
    /* XXX: allocate in chunks and scatter across cache lines */
    /* XXX: destroy freelist at exit */
    op = (gasnete_coll_op_t *)gasneti_malloc(sizeof(gasnete_coll_op_t));
    GASNETI_STAT_EVENT_VAL(W, COLL_OP_ALLOC, sizeof(gasnete_coll_op_t));
  }

    gasnete_coll_active_new(op);
    op->team     = team;
    op->e_tm     = team->e_tm;
    op->sequence = sequence;
    op->flags    = flags;
    op->eop      = NULL;
    op->poll_fn  = (gasnete_coll_poll_fn)NULL;
    op->scratchpos = NULL;

    /* The 'data' field is setup elsewhere */

    return op;
}

void
gasnete_coll_op_destroy(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
  *((gasnete_coll_op_t **)op) =  td->op_freelist;
  td->op_freelist = op;
}

#ifndef gasneti_coll_progressfn
extern void gasneti_coll_progressfn(void) {
  GASNET_BEGIN_FUNCTION(); /* TODO: remove this lookup */
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;

  if (td->in_poll) return; /* prevent recursion */
  td->in_poll = 1;

  if (gasneti_mutex_trylock(&gasnete_coll_poll_lock) == 0)
  {
    /* First try to make progress on any pending events */
    gasnete_coll_sync_saved_events(GASNETI_THREAD_PASS_ALONE);

    /* NOTE regarding thread safety of active list:
     * We traverse the active list here, possibly deleting elements.
     * However, this basic block is the *only* place that deletions can
     * occur AND execution of this block is serialized by 'poll_lock'.
     * Meanwhile, the only other modification that can be made is the
     * insertion (at the list tail) of new entries.  Therefore, we can
     * safely call gasnete_coll_active_next() without the lock held.
     * However, we must hold it for deletions.
     */
    gasneti_mutex_lock(&gasnete_coll_active_lock);
    gasnete_coll_op_t *next = gasnete_coll_active_first();
    gasnete_coll_op_t *last = gasnete_coll_active_last();
    gasneti_mutex_unlock(&gasnete_coll_active_lock);

    gasnete_coll_op_t *op;
    if (next) {
      do {
        gasneti_assert(next);
        op = next;
        next = gasnete_coll_active_next(op);

        // Poll/kick the op
        gasneti_assert(op->poll_fn);
        int poll_result = (*op->poll_fn)(op GASNETI_THREAD_PASS);

        if (poll_result != 0) {
          // signal and/or destroy the op
          gasneti_mutex_lock(&gasnete_coll_active_lock);
          gasnete_coll_op_complete(op, poll_result GASNETI_THREAD_PASS);
          gasneti_mutex_unlock(&gasnete_coll_active_lock);
        }
      } while (op != last); // Stop at original end, not at NULL, due to lack of fences
    }

    gasneti_mutex_unlock(&gasnete_coll_poll_lock);
  }
  td->in_poll = 0;
}
#endif

size_t gasnete_coll_auxseg_size = 0;
size_t gasnete_coll_auxseg_offset = 0;

/* AuxSeg setup for distributed scratch space*/
gasneti_auxseg_request_t gasnete_coll_auxseg_alloc(gasnet_seginfo_t *auxseg_info) {
  gasneti_auxseg_request_t retval;
  
  retval.minsz = gasneti_getenv_int_withdefault("GASNET_COLL_MIN_SCRATCH_SIZE",
                                                GASNETE_COLL_MIN_SCRATCH_SIZE_DEFAULT,1);
  retval.optimalsz = gasneti_getenv_int_withdefault("GASNET_COLL_SCRATCH_SIZE",
                                                    GASNETE_COLL_SCRATCH_SIZE_DEFAULT,1);
  if (auxseg_info == NULL){
    return retval; /* initial query */
  }	
  else { /* auxseg granted */
    gasneti_assert(!gasnete_coll_auxseg_size);
    gasnete_coll_auxseg_size   = auxseg_info[0].size;
    gasnete_coll_auxseg_offset = (uintptr_t)auxseg_info[0].addr -
                                 (uintptr_t)gasneti_seginfo_aux[0].addr;
  }

  return retval;
}
  
// Initialize legacy coll_team subsystem for use by gex_TM/gex_Coll
// TODO-EX: remove any portions displaced by gex-ification
extern void gasnete_coll_init_subsystem(void)
{
    GASNET_BEGIN_FUNCTION();

    gasnete_coll_p2p_eager_min = gasneti_getenv_int_withdefault("GASNET_COLL_P2P_EAGER_MIN",
                                                                GASNETE_COLL_P2P_EAGER_MIN_DEFAULT, 0);
    gasnete_coll_p2p_eager_scale = gasneti_getenv_int_withdefault("GASNET_COLL_P2P_EAGER_SCALE",
                                                                  GASNETE_COLL_P2P_EAGER_SCALE_DEFAULT, 0);

    gasnete_coll_active_init();

#ifdef gasnete_coll_init_conduit
    /* initialization of conduit specific collectives */
    gasnete_coll_init_conduit();
#endif

    /* setup information for TM0 */
    gasnete_coll_team_init(GASNET_TEAM_ALL, 0, NULL, 0 GASNETI_THREAD_PASS);
    gasneti_import_tm(gasneti_THUNK_TM)->_coll_team = GASNET_TEAM_ALL;
    GASNET_TEAM_ALL->e_tm = gasneti_THUNK_TM;

    // TODO-EX:  Move other per-OPs default tree types out of autotune infrastructure?

    const char *default_tree_type = gasneti_getenv_withdefault("GASNET_COLL_ROOTED_GEOM",
                                                               GASNETE_COLL_DEFAULT_TREE_TYPE_STR);
    const char *reduce_tree_type = gasneti_getenv_withdefault("GASNET_COLL_REDUCE_GEOM", default_tree_type);
    gasnetc_tm_reduce_tree_type = gasnete_coll_make_tree_type_str(reduce_tree_type);

    gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD; // Force allocation
}

/*---------------------------------------------------------------------------------*/
/* Synchronization primitives */

#ifndef GASNETE_COLL_CONSENSUS_OVERRIDE
/* team->consensus_issued_id counts barrier sequence numbers as they are allocated
 * to collective operations.  They are always even (see below).
 */


/* team->consensus_id holds the current barrier state and sequence.
 * The upper 31 bits are the sequence, and the least significant bit
 * is the notify-vs-wait phase of the current barrier.  This imposes a
 * limit of around 1 billion simultaneous outstanding barriers before
 * counter overflow could introduce ambiguity.  Otherwise, careful use
 * of unsigned arithmetic eliminates problems due to wrap.
 * The least significant bit of team->consensus_id is 0 if the next
 * operation is to be a notify, or a 1 if the next is a try.
 * Any caller may issue a try (when the phase indicates a try) and must
 * advance gasnete_coll_issued_id by 1 if the try returns success.
 * Only the matching caller may issue a notify and must unconditionally
 * advance gasnete_coll_issued_id by 1.
 * In a debug build the gasnete_coll_issued_id is also used as the barrier
 * name to help detect bugs, but anonymous barriers are used in non-debug
 * builds for speed.
 */


// Means for partial replacement by conduits:
#if defined(GASNETE_COLL_CONSENSUS_TRY) && \
    defined(GASNETE_COLL_CONSENSUS_NOTIFY)
  // Use conduit-provided versions
  #ifdef GASNETE_COLL_CONSENSUS_DEFNS
    // Expand any conduit-provided deferred definitions
    // This hook is provided because the 'team' argument to the macros
    // GASNETE_COLL_CONSENSUS_{NOTIFY,TRY}() has a private type.
    GASNETE_COLL_CONSENSUS_DEFNS
  #endif
#elif defined(GASNETE_COLL_CONSENSUS_TRY) || \
      defined(GASNETE_COLL_CONSENSUS_NOTIFY)
  #error Conduit must define both or neither of GASNETE_COLL_CONSENSUS_{NOTIFY,TRY}
#else
  // Use defaults, below
  #define GASNETE_COLL_CONSENSUS_NOTIFY(team) \
          GASNETE_COLL_CONSENSUS_DEFAULT_NOTIFY(team)
  #define GASNETE_COLL_CONSENSUS_TRY(team) \
          GASNETE_COLL_CONSENSUS_DEFAULT_TRY(team)
#endif

// Default versions (also available for use by conduit-provided ones)
#if GASNET_DEBUG
  // Use full name-matching facility for error checking
  #define GASNETE_COLL_CONSENSUS_DEFAULT_NOTIFY(team) \
          gasnet_coll_barrier_notify(team, team->consensus_id, 0)
  #define GASNETE_COLL_CONSENSUS_DEFAULT_TRY(team) \
          gasnet_coll_barrier_try(team, team->consensus_id, 0)
#else
  // Use "unnamed" barrier for performance
  #define GASNETE_COLL_CONSENSUS_DEFAULT_NOTIFY(team) \
          gasnet_coll_barrier_notify(team, 0, GASNET_BARRIERFLAG_UNNAMED)
  #define GASNETE_COLL_CONSENSUS_DEFAULT_TRY(team) \
          gasnet_coll_barrier_try(team, 0, GASNET_BARRIERFLAG_UNNAMED)
#endif


extern gasnete_coll_consensus_t gasnete_coll_consensus_create(gasnete_coll_team_t team) {
  gasnete_coll_consensus_t result = team->consensus_issued_id;
  team->consensus_issued_id = result + 2;
  GASNETE_COLL_SEQ32_SAFE(result, team->consensus_id);
  return result;
}

void gasnete_coll_consensus_free(gasnete_coll_team_t team, gasnete_coll_consensus_t consensus) {
  // Nothing to do
}

GASNETI_INLINE(gasnete_coll_consensus_do_try)
int gasnete_coll_consensus_do_try(gasnete_coll_team_t team) {
  int rc = GASNETE_COLL_CONSENSUS_TRY(team);
  if_pt (rc == GASNET_OK) {
    /* A barrier is complete, advance */
    ++team->consensus_id;
    return 1;
  }
#if GASNET_DEBUG
  else if (rc == GASNET_ERR_BARRIER_MISMATCH) {
    gasneti_fatalerror("Named barrier mismatch detected in collectives");
  } else {
    gasneti_assert(rc == GASNET_ERR_NOT_READY);
  }
#endif
  return 0;
}

GASNETI_INLINE(gasnete_coll_consensus_do_notify)
void gasnete_coll_consensus_do_notify(gasnete_coll_team_t team) {
  ++team->consensus_id;
  GASNETE_COLL_CONSENSUS_NOTIFY(team);
}


extern int gasnete_coll_consensus_try(gasnete_coll_team_t team, gasnete_coll_consensus_t id) {
#if GASNET_DEBUG
  // With respect to any given team, this function is neither thread-safe nor recursion-safe
  gasneti_assert_always_int(gasneti_mutex_trylock(&team->barrier_lock) ,==, GASNET_OK);
#endif

  gasneti_assert(! (id & 1)); // always even
  /* We can only notify when our own turn comes up.
   * Thus, the most progress we could make in one call
   * would be to sucessfully 'try' for our predecessor,
   * 'notify' our our barrier, and then 'try' our own.
   */
  switch (id - team->consensus_id) {
  case 1:
	  /* Try for our predecessor, hoping we can then notify */
	  if (!gasnete_coll_consensus_do_try(team)) {
	    gasneti_assert_uint((id - team->consensus_id) ,==, 1);
	    /* Sucessor is not yet done */
	    break;
	  }
	  gasneti_assert_uint(id ,==, team->consensus_id);
	  /* ready to advance, so fall through... */ GASNETI_FALLTHROUGH
  case 0:
	  /* Our own turn has come - notify and try */
	  gasnete_coll_consensus_do_notify(team);
	  gasneti_assert_uint((team->consensus_id - id) ,==,1);
	  gasnete_coll_consensus_do_try(team);
	  gasneti_assert(((team->consensus_id - id) == 1) ||
                         ((team->consensus_id - id) == 2));
	  break;

  default:
	  /* not our turn, but we can 'try' if the phase is right */
	  if (team->consensus_id & 1) {
	    gasnete_coll_consensus_do_try(team);
	  }
  }

  // Use of macro takes care with respect to wrap-around
  int done = GASNETE_COLL_SEQ32_GE(team->consensus_id, id + 2);

#if GASNET_DEBUG
  gasneti_mutex_unlock(&team->barrier_lock);
#endif

  return done ? GASNET_OK : GASNET_ERR_NOT_READY;
}

// Helper for gasnete_coll_consensus_barrier()
// Bug 3854 identified an undesired recursion when calling
// gasnete_coll_consensus_try() from gasnete_coll_consensus_barrier() since the
// "try" operation could lead to an AMPoll which runs the collectives progress
// function (and thus a nested gasnete_coll_consensus_try).
// So, here we masquerade as an instance of the progress function to prevent
// that recursion.
GASNETI_INLINE(gasnete_coll_consensus_try_as_poller)
int gasnete_coll_consensus_try_as_poller(gasnete_coll_threaddata_t *td, gasnete_coll_team_t team, gasnete_coll_consensus_t id)
{
  gasneti_assert(! td->in_poll);
  td->in_poll = 1;
  int rc = gasnete_coll_consensus_try(team, id);
  td->in_poll = 0;
  return rc;
}

// gasnete_coll_consensus_barrier():
// Allocate a new barrier and wait for all earlier barriers to finish.
// This omits the overheads of allocating a collective op, but the cost is
// that we must interlock with the collectives progress function.
// NOTE: therefore illegal to call from a collective poll fn
extern int gasnete_coll_consensus_barrier(gasnete_coll_team_t team GASNETI_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  gasnete_coll_consensus_t mybarr = gasnete_coll_consensus_create(team);

  while (gasnete_coll_consensus_try_as_poller(td, team, mybarr) == GASNET_ERR_NOT_READY) {
    gasneti_AMPoll();
  }

  return GASNET_OK;
}
#endif

#ifndef GASNETE_COLL_P2P_OVERRIDE
	
#define GASNETE_COLL_P2P_TABLE_SLOT(S) \
	 (gasneti_assert(GASNETI_POWEROFTWO(GASNETE_COLL_P2P_TABLE_SIZE)), \
          ((uint32_t)(S) & (GASNETE_COLL_P2P_TABLE_SIZE-1)))

gasnete_coll_p2p_t *gasnete_coll_p2p_get(uint32_t team_id, uint32_t sequence) {
  gasnete_coll_team_t team = gasnete_coll_team_lookup(team_id);
  unsigned int slot_nr = GASNETE_COLL_P2P_TABLE_SLOT(sequence);
  gasnete_coll_p2p_t *p2p, **prev_p;
  int i;
  
  gex_HSL_Lock(&team->p2p_lock);

  /* Search table, which is sorted by sequence */
  prev_p = &(team->p2p_table[slot_nr]);
  p2p = team->p2p_table[slot_nr];
  while (p2p && GASNETE_COLL_SEQ32_LT(p2p->sequence, sequence)) {
    prev_p = &p2p->p2p_next;
    p2p = p2p->p2p_next;
  }

  /* If not found, create it with all zeros */
  if_pf ((p2p == NULL) || (p2p->sequence != sequence)) {
    size_t statesz = GASNETI_ALIGNUP(2*team->total_ranks * sizeof(uint32_t), 8);
    size_t countersz = GASNETI_ALIGNUP(2*team->total_ranks * sizeof(gasneti_weakatomic_t), 8);
    gasnete_coll_p2p_t *next = p2p;
        
    p2p = team->p2p_freelist;
        
    if_pf (p2p == NULL) {
      /* Round to 8-byte alignment of entry array */
      size_t alloc_size = GASNETI_ALIGNUP(sizeof(gasnete_coll_p2p_t) + statesz + countersz,8)
        + team->p2p_eager_buffersz;
      void *alloc_ptr = gasneti_malloc(alloc_size);
      gasneti_leak(alloc_ptr);
      uintptr_t p = (uintptr_t)alloc_ptr;
      GASNETI_STAT_EVENT_VAL(W, COLL_P2P_ALLOC, alloc_size);
          
      p2p = (gasnete_coll_p2p_t *)p;
      p += sizeof(gasnete_coll_p2p_t);
          
      p2p->state = (uint32_t *)p;
      p += statesz;
          
      p2p->counter = (gasneti_weakatomic_t *)p;
      p += countersz;
          
      p = GASNETI_ALIGNUP(p,8);
      p2p->data = (uint8_t *)p;
          
      p2p->p2p_next = NULL;
    }
        
    memset((void *)p2p->state, 0, statesz);
    memset(p2p->data, 0, team->p2p_eager_buffersz);
    for(i=0; i<2*team->total_ranks; i++) {
      gasneti_weakatomic_set(&p2p->counter[i], 0, 0);
    }
    gasneti_sync_writes();
        
#if GASNET_DEBUG
    p2p->team_id = team_id;
#endif
    p2p->sequence = sequence;
    gex_HSL_Init(&p2p->lock);
        
    team->p2p_freelist = p2p->p2p_next;
        
    /* Insert in order before the last location searched */
    gasneti_assert(prev_p != NULL);
    gasneti_assert(!next || (next->p2p_prev_p == prev_p));
    *prev_p = p2p;
    p2p->p2p_prev_p = prev_p;
    p2p->p2p_next = next;
    if (next) {
      next->p2p_prev_p = &p2p->p2p_next;
    }
#ifdef GASNETE_P2P_EXTRA_INIT
    GASNETE_P2P_EXTRA_INIT(p2p)
#endif
  }
      
  gex_HSL_Unlock(&team->p2p_lock);
      
  gasneti_assert(p2p != NULL);
  gasneti_assert(p2p->state != NULL);
  gasneti_assert(p2p->data != NULL);
  gasneti_assert(p2p->team_id == team->team_id);
      
  return p2p;
}

void gasnete_coll_p2p_free(gasnete_coll_team_t team, gasnete_coll_p2p_t *p2p) {
  gasneti_assert(p2p != NULL);
  gasneti_assert(p2p->team_id == team->team_id);

  gex_HSL_Lock(&team->p2p_lock);

  *(p2p->p2p_prev_p) = p2p->p2p_next;
  if (p2p->p2p_next) {
    p2p->p2p_next->p2p_prev_p = p2p->p2p_prev_p;
  }
#ifdef GASNETE_P2P_EXTRA_FREE
  GASNETE_P2P_EXTRA_FREE(p2p)
#endif

  p2p->p2p_next = team->p2p_freelist;
  team->p2p_freelist = p2p;

#if GASNET_DEBUG
  /* Detect double free using otherwise unused prev pointer */
  gasneti_assert(p2p->p2p_prev_p != &p2p->p2p_next);
  p2p->p2p_prev_p = &p2p->p2p_next;
#endif

  gex_HSL_Unlock(&team->p2p_lock);
}

void gasnete_coll_p2p_purge(gasnete_coll_team_t team) {
  gex_HSL_Lock(&team->p2p_lock);

  gasnete_coll_p2p_t *p2p = team->p2p_freelist;
  team->p2p_freelist = NULL;

  while (p2p) {
    gasnete_coll_p2p_t *next = p2p->p2p_next;
    gasneti_free(p2p);
    p2p = next;
  }

  gex_HSL_Unlock(&team->p2p_lock);
}

    
/* Delivers a long payload and updates 1 or more states
   count: number of states to update
   offset: index of first state to update
   state: value to assign to states [offset, offset+count)
*/
extern void gasnete_coll_p2p_long_reqh(gex_Token_t token, void *buf, size_t nbytes,
                                       gex_AM_Arg_t team_id,
                                       gex_AM_Arg_t sequence,
                                       gex_AM_Arg_t count,
                                       gex_AM_Arg_t offset,
                                       gex_AM_Arg_t state) {
  gasnete_coll_p2p_t *p2p = gasnete_coll_p2p_get(team_id, sequence);
  int i;

  if (nbytes) {
    gasneti_sync_writes();
  }

  for (i = 0; i < count; ++i, ++offset) {
    p2p->state[offset] = state;
  }
}

/* Delivers a medium payload to the eager buffer space and updates 1 or more states
   count: number of states to update
   offset: index of first state to update
   state: value to assign to states [offset, offset+count)
   size: eager element size; payload is copied to (p2p->data + offset*size)
*/
extern void gasnete_coll_p2p_med_reqh(gex_Token_t token, void *buf, size_t nbytes,
                                      gex_AM_Arg_t team_id,
                                      gex_AM_Arg_t sequence,
                                      gex_AM_Arg_t count,
                                      gex_AM_Arg_t offset,
                                      gex_AM_Arg_t state,
                                      gex_AM_Arg_t size) {
  gasnete_coll_p2p_t *p2p = gasnete_coll_p2p_get(team_id, sequence);
  int i;

  if (size) {
    GASNETE_FAST_UNALIGNED_MEMCPY(p2p->data + offset*size, buf, nbytes);
    gasneti_sync_writes();
  }

  for (i = 0; i < count; ++i, ++offset) {
    p2p->state[offset] = state;
  }
}

extern void gasnete_coll_p2p_med_counting_reqh(gex_Token_t token, void *buf, size_t nbytes,
                                               gex_AM_Arg_t team_id,
                                               gex_AM_Arg_t sequence,
                                               gex_AM_Arg_t offset,
                                               gex_AM_Arg_t idx,
                                               gex_AM_Arg_t size) {
  gasnete_coll_p2p_t *p2p = gasnete_coll_p2p_get(team_id, sequence);
  
  if (size) {
    GASNETE_FAST_UNALIGNED_MEMCPY(p2p->data + offset*size, buf, nbytes);
    gasneti_sync_writes();
  }
  
  gasneti_weakatomic_increment(&p2p->counter[idx], 0);
}

/* Delivers a medium payload to the eager buffer space and updates 1 state
   size: eager element size; payload is copied to (p2p->data)
*/
extern void gasnete_coll_p2p_med_tree_reqh(gex_Token_t token, void *buf, size_t nbytes,
                                           gex_AM_Arg_t team_id,
                                           gex_AM_Arg_t sequence) {
  gasnete_coll_p2p_t *p2p = gasnete_coll_p2p_get(team_id, sequence);
      
  GASNETE_FAST_UNALIGNED_MEMCPY(p2p->data, buf, nbytes);
  gasneti_sync_writes();
      
  p2p->state[0] = 1;
      
}

/* No payload to deliver, just updates 1 or more states
   count: number of states to update
   offset: index of first state to update
   state: value to assign to states [offset, offset+count)
*/
extern void gasnete_coll_p2p_short_reqh(gex_Token_t token,
                                        gex_AM_Arg_t team_id,
                                        gex_AM_Arg_t sequence,
                                        gex_AM_Arg_t count,
                                        gex_AM_Arg_t offset,
                                        gex_AM_Arg_t state) {
  gasnete_coll_p2p_t *p2p = gasnete_coll_p2p_get(team_id, sequence);
  int i;

  for (i = 0; i < count; ++i, ++offset) {
    p2p->state[offset] = state;
  }
}

/* Increment atomic counter */
extern void gasnete_coll_p2p_advance_reqh(gex_Token_t token,
                                          gex_AM_Arg_t team_id,
                                          gex_AM_Arg_t sequence,
                                          gex_AM_Arg_t idx) {

  gasnete_coll_p2p_t *p2p = gasnete_coll_p2p_get(team_id, sequence);
  gasneti_weakatomic_increment(&p2p->counter[idx], 0);
}

/* Send the data and increment atomic counter */
extern void gasnete_coll_p2p_put_and_advance_reqh(gex_Token_t token, void *buf, size_t nbytes,
                                                  gex_AM_Arg_t team_id,
                                                  gex_AM_Arg_t sequence,
                                                  gex_AM_Arg_t idx) {

  gasnete_coll_p2p_t *p2p;

  if (nbytes) {
    gasneti_sync_writes();
  }
      
  p2p = gasnete_coll_p2p_get(team_id, sequence);
  gasneti_weakatomic_increment(&p2p->counter[idx], 0);
}

/* Memcopy payload and then decrement atomic counter if requested */
GASNETI_INLINE(gasnete_coll_p2p_memcpy_reqh_inner)
     void gasnete_coll_p2p_memcpy_reqh_inner(gex_Token_t token, void *buf, size_t nbytes,
                                             void *dest,
                                             gex_AM_Arg_t team_id,
                                             gex_AM_Arg_t sequence,
                                             gex_AM_Arg_t decrement) {
  gasnete_coll_p2p_t *p2p = gasnete_coll_p2p_get(team_id, sequence);

  GASNETE_FAST_UNALIGNED_MEMCPY(dest, buf, nbytes);
  if (decrement) {
    gasneti_weakatomic_decrement(&p2p->counter[0], GASNETI_ATOMIC_REL);
  }
}
MEDIUM_HANDLER(gasnete_coll_p2p_memcpy_reqh,4,5,
               (token,addr,nbytes, UNPACK(a0),      a1, a2, a3),
               (token,addr,nbytes, UNPACK2(a0, a1), a2, a3, a4));

/* Put up to gex_AM_LUBRequestLong() bytes, signalling the recipient */
/* Returns as soon as local buffer is reusable */
void gasnete_tm_p2p_counting_put(gasnete_coll_op_t *op, gex_Rank_t dstrank, void *dst,
                                 void *src, size_t nbytes, uint32_t idx
                                 GASNETI_THREAD_FARG)
{
  uint32_t seq_num = op->sequence;
  const uint32_t team_id = op->team->team_id;

  gasneti_assert(nbytes <= gex_AM_LUBRequestLong());
  
  gex_AM_RequestLong(op->e_tm, dstrank, gasneti_handleridx(gasnete_coll_p2p_put_and_advance_reqh),
                         src, nbytes, dst, GEX_EVENT_NOW, 0, team_id, seq_num, idx);
}
/* Put up to gex_AM_LUBRequestLong() bytes, signalling the recipient */
/* Returns immediately even if the local buffer is not yet reusable */
void gasnete_tm_p2p_counting_putAsync(gasnete_coll_op_t *op, gex_Rank_t dstrank, void *dst,
                                      void *src, size_t nbytes, uint32_t idx
                                      GASNETI_THREAD_FARG)
{
  uint32_t seq_num = op->sequence;
  const uint32_t team_id = op->team->team_id;
  
  gasneti_assert(nbytes <= gex_AM_LUBRequestLong());
  
  // TODO-EX: restore "Async"
  gex_AM_RequestLong(op->e_tm, dstrank, gasneti_handleridx(gasnete_coll_p2p_put_and_advance_reqh),
                         src, nbytes, dst, GEX_EVENT_NOW, 0, team_id, seq_num, idx);
}
    
/* Send data to be buffered by the recipient */
int gasnete_tm_p2p_eager_putM(
                        gasnete_coll_op_t *op,
                        gex_Rank_t rank,
                        const void *src, uint32_t count, size_t size,
                        gex_Event_t *lc_opt, gex_Flags_t flags,
                        uint32_t offset, uint32_t state
                        GASNETI_THREAD_FARG)
{
  // TODO-EX: flags |= INTERNAL to prevent tracing
  const uint32_t team_id = op->team->team_id;
  const uint32_t sequence = op->sequence;
  if_pf (count * size > gex_AM_LUBRequestMedium()) {
    const size_t chunk = gex_AM_LUBRequestMedium() / size;
    size_t nbytes = chunk * size;
    flags &= ~GEX_FLAG_IMMEDIATE; // TODO-EX: support maybe IMMEDIATE for multi-AM case?
    do {
      gasneti_assert_zeroret(
         gex_AM_RequestMedium6(op->e_tm, rank, gasneti_handleridx(gasnete_coll_p2p_med_reqh),
                               (void*)src, nbytes, GEX_EVENT_NOW, flags,
                               team_id, sequence, chunk, offset, state, size));
      offset += chunk;
      src = (void *)((uintptr_t)src + nbytes);
      count -= chunk;
    } while (count > chunk);
  }
  return gex_AM_RequestMedium6(op->e_tm, rank, gasneti_handleridx(gasnete_coll_p2p_med_reqh),
                               (void*)src, count * size, lc_opt, flags,
                               team_id, sequence, count, offset, state, size);
}

/* a simplification for eager putM so that we send less bits on the wire*/ 
/* we hardcode the assumption that we want to send to state 0 and set a value of 1*/
/* for cases in which we are just sending down the tree (such as a broadcast) this is sufficient*/
void gasnete_tm_p2p_eager_put_tree(gasnete_coll_op_t *op, gex_Rank_t dstrank,
                                   void *src, size_t size GASNETI_THREAD_FARG)
{
  uint32_t seq_num = op->sequence;
  const uint32_t team_id = op->team->team_id;

  gasneti_assert(size <= gex_AM_LUBRequestMedium());
  gex_AM_RequestMedium(op->e_tm, dstrank, gasneti_handleridx(gasnete_coll_p2p_med_tree_reqh),
                           src, size, GEX_EVENT_NOW, 0, team_id, seq_num);
}

/* Memcpy up to gex_AM_LUBRequestMedium() bytes, signalling the recipient */
int gasnete_tm_p2p_memcpy(gasnete_coll_op_t *op, gex_Rank_t rank, void *dst,
                          void *src, size_t nbytes, gex_Flags_t flags GASNETI_THREAD_FARG)
{
  const uint32_t seq_num = op->sequence;
  const uint32_t team_id = op->team->team_id;

  gasneti_assert(nbytes <= gex_AM_LUBRequestMedium());

  return gex_AM_RequestMedium(op->e_tm, rank, gasneti_handleridx(gasnete_coll_p2p_memcpy_reqh),
                              src, nbytes, GEX_EVENT_NOW, flags, PACK(dst), team_id, seq_num, 1);
}


extern void gasnete_tm_p2p_counting_eager_put(gasnete_coll_op_t *op, gex_Rank_t dstrank,
                                              void *src, size_t nbytes, size_t offset_size,
                                              uint32_t offset, uint32_t idx GASNETI_THREAD_FARG)
{
  const uint32_t team_id = op->team->team_id;
  
  gex_AM_RequestMedium(op->e_tm, dstrank, gasneti_handleridx(gasnete_coll_p2p_med_counting_reqh),
                           src, nbytes, GEX_EVENT_NOW, 0, team_id, op->sequence, offset, idx, offset_size);
}


/* Indicate ready for a gasnete_tm_p2p_memcpy, placing request in slot "offset" */
int gasnete_tm_p2p_send_rtr(
                        gasnete_coll_op_t *op, gasnete_coll_p2p_t *p2p,
                        gex_Rank_t rank, uint32_t offset,
                        void *dst, size_t nbytes,
                        gex_Flags_t flags GASNETI_THREAD_FARG)
{
  struct gasnete_tm_p2p_send_struct tmp;
  tmp.addr = dst;
  tmp.sent = 0;
  /* TODO: we send addr+"0", when only the addr is needed (need custom AM instead of eager_put). */
  int retval =
    gasnete_tm_p2p_eager_put(op, rank, &tmp, sizeof(tmp),
                             GEX_EVENT_NOW, flags, offset, 1 GASNETI_THREAD_PASS);
  if (retval) {
    // back pressure
    gasneti_assert(flags & GEX_FLAG_IMMEDIATE);
  } else {
    /* Compute the number of Mediums we know we'll receive. */
    const gasneti_weakatomic_val_t msg_count = ((nbytes + gex_AM_LUBRequestMedium() - 1) / gex_AM_LUBRequestMedium());
    // check for overflow of msg_count:
    gasneti_assert_uint(nbytes ,<=, (msg_count * gex_AM_LUBRequestMedium()));
    gasneti_assert_uint(msg_count ,<=, GASNETI_ATOMIC_MAX);
    gasneti_weakatomic_add(&p2p->counter[0], msg_count, GASNETI_ATOMIC_NONE);
  }
  return retval;
}

/* Respond to a gasnete_tm_p2p_send_rtr */
// Return:
//   0: no data remains to be sent (done)
//   1: unsent data remains OR xfer has not even started
//   2: unsent data remains, but was not sent due to IMMEDIATE back-pressure
int gasnete_tm_p2p_send_data(gasnete_coll_op_t *op, gasnete_coll_p2p_t *p2p,
                             gex_Rank_t rank, uint32_t offset,
                             const void *src, size_t nbytes,
                             gex_Flags_t flags GASNETI_THREAD_FARG) {
  struct gasnete_tm_p2p_send_struct *status = (struct gasnete_tm_p2p_send_struct *)p2p->data;
  if (p2p->state[offset] == 1) {
    size_t sent = status[offset].sent;
    gasneti_assert_uint(nbytes ,>=, sent);
    size_t count = nbytes - sent;
    if_pt (count) {
      void *tmp = (void *)((uintptr_t)src + sent);
      void *addr = status[offset].addr;
      const size_t limit = gex_AM_LUBRequestMedium();
      const int more = (count > limit);
      if (more) count = limit;
      if (gasnete_tm_p2p_memcpy(op, rank, addr, tmp, count, flags GASNETI_THREAD_PASS)) {
        return 2; // back pressure
      }
      status[offset].addr = (void *)((uintptr_t)addr + count);
      status[offset].sent += count;
      return more;
    } else {
      p2p->state[offset] = 2;
      return 0;
    }
  }
  return (p2p->state[offset] != 2);
}
#endif

/*---------------------------------------------------------------------------------*/
/* functions for generic ops */

extern gasnete_coll_generic_data_t *gasnete_coll_generic_alloc(GASNETI_THREAD_FARG_ALONE) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  gasnete_coll_generic_data_t *result;

  gasneti_assert(td != NULL);

  result = td->generic_data_freelist;
  if_pt (result != NULL) {
    td->generic_data_freelist = *((gasnete_coll_generic_data_t **)result);
  } else {
    /* XXX: allocate in chunks and scatter across cache lines */
    /* XXX: destroy freelist at exit */
    result = (gasnete_coll_generic_data_t *)gasneti_calloc(1, sizeof(gasnete_coll_generic_data_t));
    GASNETI_STAT_EVENT_VAL(W, COLL_GDATA_ALLOC, sizeof(gasnete_coll_generic_data_t));
  }

  memset(result, 0, sizeof(*result));
  gasneti_sync_writes();
  return result;
}

extern void gasnete_coll_generic_free(gasnete_coll_team_t team, gasnete_coll_generic_data_t *data GASNETI_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
  gasneti_assert(data != NULL);
	
  if (data->options & GASNETE_COLL_GENERIC_OPT_P2P) {
    gasnete_coll_p2p_free(team, data->p2p);
  }
  if(data->options & GASNETE_COLL_GENERIC_OPT_INSYNC) {
    gasnete_coll_consensus_free(team, data->in_barrier);
  }
  if(data->options & GASNETE_COLL_GENERIC_OPT_OUTSYNC) {
    gasnete_coll_consensus_free(team, data->out_barrier);
  }

  *((gasnete_coll_generic_data_t **)data) =  td->generic_data_freelist;
  td->generic_data_freelist = data;
}

/* Generic routine to create an op and enter it in the active list, etc..
 * Caller provides 'data' and 'poll_fn' specific to the operation.
 * Event is allocated automatically.
 *
 * 'sequence' can have two meanings:
 *  w/ GASNETE_COLL_SUBORDINATE it is the pre-allocated sequence number to assign
 *  w/o GASNETE_COLL_SUBORDINATE is is the number of slots to pre-allocate
 *
 * Just returns the handle.
 */
 



extern gex_Event_t
gasnete_coll_op_generic_init_with_scratch(gasnete_coll_team_t team, int flags,
                                          gasnete_coll_generic_data_t *data, gasnete_coll_poll_fn poll_fn,
                                          uint32_t sequence, gasnete_coll_scratch_req_t *scratch_req,
                                          int num_params, uint32_t *param_list,
                                          gasnete_coll_local_tree_geom_t *geom_info
                                          GASNETI_THREAD_FARG) {
  gasnete_coll_eop_t result = NULL;
  gasnete_coll_op_t *op;

  gasneti_assert(data != NULL);
      
  if_pf (!(flags & GASNETE_COLL_SUBORDINATE)) {
    uint32_t tmp = team->sequence;
    team->sequence += (1 + sequence);
    sequence = tmp;
#if GASNET_DEBUG
    // Check largest allocated sequence number lies is within safe range of oldest "live"
    // Depends on order of the active list (oldest first)
    uint32_t last = team->sequence - 1;
    gasneti_mutex_lock(&gasnete_coll_active_lock);
      gasnete_coll_op_t *op = gasnete_coll_active_first();
      while (op && op->team != team) {
        op = gasnete_coll_active_next(op);
      }
    gasneti_mutex_unlock(&gasnete_coll_active_lock);
    if (op) {
      GASNETE_COLL_SEQ32_SAFE(last, op->sequence);
    }
#endif
  }

    /* Conditionally allocate data for point-to-point syncs */
    if (data->options & GASNETE_COLL_GENERIC_OPT_P2P) {
      data->p2p = gasnete_coll_p2p_get(team->team_id, sequence);
    }

    /* Unconditionally allocate an eop */
    result = gasnete_coll_eop_create(GASNETI_THREAD_PASS_ALONE);

    /* Create the op */
    op = gasnete_coll_op_create(team, sequence, flags GASNETI_THREAD_PASS);
    op->data = data;
    op->poll_fn = poll_fn;
    op->flags = flags;

		/*set up scratch space here as needed modify coll op to take an extra struct argument
		  if it is NULL it indicates that no scratch is required (default case)
		  if it isn't NULL then it means that we want to call it with scratch
		  MAKE SURE TO SETUP SCRATCH BEFORE THE OP IS SET TO BE ACTIVE
    */

    op->scratch_req = scratch_req;
    if (scratch_req) scratch_req->op = op;

    /* Allocate the barriers AFTER SCRATCH SPACE*/
    /* This will allow the scratch space to use its own consensus barriers*/
    if_pf (flags & GASNETE_COLL_SUBORDINATE) {
      /* Subordinates can't allocate sequence numbers or barriers, due to non-collective calling */
      gasneti_assert(!(data->options & (GASNETE_COLL_GENERIC_OPT_INSYNC |
                                        GASNETE_COLL_GENERIC_OPT_OUTSYNC)));
    } else {
      /* Conditionally allocate barriers */
      if (data->options & GASNETE_COLL_GENERIC_OPT_INSYNC) {
        data->in_barrier = gasnete_coll_consensus_create(team);
      }
      if (data->options & GASNETE_COLL_GENERIC_OPT_OUTSYNC) {
        data->out_barrier = gasnete_coll_consensus_create(team);
      }
    }

    op->waiting_scratch_op = 0;
    op->active_scratch_op = 0;
    op->waiting_for_reconfig_clear=0;
#if GASNET_DEBUG
    op->scratch_op_freed = 0;
#endif
    op->num_coll_params = num_params;
    GASNETI_MEMCPY_SAFE_EMPTY(op->param_list, param_list, sizeof(uint32_t)*num_params);

    gasnete_coll_op_submit(op, result GASNETI_THREAD_PASS);
    return GASNETE_COLL_EOP_TO_EVENT(result);
}

/* NOTE: caller is responsible for a gasneti_sync_reads() if they read any transferred data.  */
extern int gasnete_coll_generic_coll_sync(gex_Event_t *p, size_t count GASNETI_THREAD_FARG) {
  int result = 1;
  int i;

  for (i = 0; i < count; ++i) {
    if (p[i] != GEX_EVENT_INVALID) {
      result = 0;
      break;
    }
  }

  return result;
}

/*********** BROADCAST *******/
#ifndef gasnete_coll_broadcast_nb
#define gasnete_coll_broadcast_nb gasnete_coll_broadcast_nb_default
#else
  extern gex_Event_t
  gasnete_coll_broadcast_nb_default(gasnet_team_handle_t team,
                                    void *dst,
                                    gasnet_image_t srcimage, void *src,
                                    size_t nbytes, int flags, uint32_t sequence
                                    GASNETI_THREAD_FARG);
#endif
extern gex_Event_t
gasnete_coll_broadcast_nb(gasnet_team_handle_t team,
                          void *dst,
                          gasnet_image_t srcimage, void *src,
                          size_t nbytes, int flags, uint32_t sequence
                          GASNETI_THREAD_FARG);
GASNETI_COLL_FN_HEADER(_gasnet_coll_broadcast_nb) GASNETI_WARN_UNUSED_RESULT
gex_Event_t
_gasnet_coll_broadcast_nb(gasnet_team_handle_t team,
                          void *dst,
                          gasnet_image_t srcimage, void *src,
                          size_t nbytes, int flags GASNETI_THREAD_FARG) {
  gex_Event_t handle;

  GASNETI_TRACE_COLL_BROADCAST(COLL_BROADCAST_NB,team,dst,srcimage,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_BROADCAST(team,dst,srcimage,src,nbytes,flags);
  handle = gasnete_coll_broadcast_nb(team,dst,srcimage,src,nbytes,flags,0 GASNETI_THREAD_PASS);
  gasneti_AMPoll(); // No progress made until now
  return handle;
}

#ifdef gasnete_coll_broadcast

extern void 
gasnete_coll_broadcast(gasnet_team_handle_t team,
                       void *dst,
                       gasnet_image_t srcimage, void *src,
                       size_t nbytes, int flags GASNETI_THREAD_FARG);
#else
GASNETI_COLL_FN_HEADER(gasnete_coll_broadcast)
     void gasnete_coll_broadcast(gasnet_team_handle_t team,
                                 void *dst,
                                 gasnet_image_t srcimage, void *src,
                                 size_t nbytes, int flags GASNETI_THREAD_FARG) {
  gex_Event_t handle;
  handle = gasnete_coll_broadcast_nb(team,dst,srcimage,src,nbytes,flags,0 GASNETI_THREAD_PASS);
  gasnete_wait(handle GASNETI_THREAD_PASS);
}
#endif
GASNETI_COLL_FN_HEADER(_gasnet_coll_broadcast)
     void _gasnet_coll_broadcast(gasnet_team_handle_t team,
                                 void *dst,
                                 gasnet_image_t srcimage, void *src,
                                 size_t nbytes, int flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_COLL_BROADCAST(COLL_BROADCAST,team,dst,srcimage,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_BROADCAST(team,dst,srcimage,src,nbytes,flags);
  gasnete_coll_broadcast(team,dst,srcimage,src,nbytes,flags GASNETI_THREAD_PASS);
}

/********* SCATTER *****************/
#ifndef gasnete_coll_scatter_nb
#define gasnete_coll_scatter_nb gasnete_coll_scatter_nb_default
#else
extern gex_Event_t
gasnete_coll_scatter_nb_default(gasnet_team_handle_t team,
                                void *dst,
                                gasnet_image_t srcimage, void *src,
                                size_t nbytes, int flags, uint32_t sequence
                                GASNETI_THREAD_FARG);
#endif
extern gex_Event_t
gasnete_coll_scatter_nb(gasnet_team_handle_t team,
                        void *dst,
                        gasnet_image_t srcimage, void *src,
                        size_t nbytes, int flags, uint32_t sequence
                        GASNETI_THREAD_FARG);
GASNETI_COLL_FN_HEADER(_gasnet_coll_scatter_nb) GASNETI_WARN_UNUSED_RESULT
gex_Event_t
_gasnet_coll_scatter_nb(gasnet_team_handle_t team,
                        void *dst,
                        gasnet_image_t srcimage, void *src,
                        size_t nbytes, int flags GASNETI_THREAD_FARG) {
  gex_Event_t handle;
  GASNETI_TRACE_COLL_SCATTER(COLL_SCATTER_NB,team,dst,srcimage,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_SCATTER(team,dst,srcimage,src,nbytes,flags);
  handle = gasnete_coll_scatter_nb(team,dst,srcimage,src,nbytes,flags,0 GASNETI_THREAD_PASS);
  gasneti_AMPoll(); // No progress made until now
  return handle;
}


#ifdef gasnete_coll_scatter
extern void
gasnete_coll_scatter(gasnet_team_handle_t team,
                     void *dst,
                     gasnet_image_t srcimage, void *src,
                     size_t nbytes, int flags GASNETI_THREAD_FARG);
#else
GASNETI_COLL_FN_HEADER(gasnete_coll_scatter)
     void gasnete_coll_scatter(gasnet_team_handle_t team,
                               void *dst,
                               gasnet_image_t srcimage, void *src,
                               size_t nbytes, int flags GASNETI_THREAD_FARG) {
  gex_Event_t handle;
  handle = gasnete_coll_scatter_nb(team,dst,srcimage,src,nbytes,flags,0 GASNETI_THREAD_PASS);
  gasnete_wait(handle GASNETI_THREAD_PASS);
}
#endif
GASNETI_COLL_FN_HEADER(_gasnet_coll_scatter)
     void _gasnet_coll_scatter(gasnet_team_handle_t team,
                               void *dst,
                               gasnet_image_t srcimage, void *src,
                               size_t nbytes, int flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_COLL_SCATTER(COLL_SCATTER,team,dst,srcimage,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_SCATTER(team,dst,srcimage,src,nbytes,flags);
  gasnete_coll_scatter(team,dst,srcimage,src,nbytes,flags GASNETI_THREAD_PASS);
}

/*********GATHER *************/

#ifndef gasnete_coll_gather_nb
#define gasnete_coll_gather_nb gasnete_coll_gather_nb_default
#else
extern gex_Event_t
gasnete_coll_gather_nb_default(gasnet_team_handle_t team,
                               gasnet_image_t dstimage, void *dst,
                               void *src,
                               size_t nbytes, int flags, uint32_t sequence
                               GASNETI_THREAD_FARG);
#endif
extern gex_Event_t
gasnete_coll_gather_nb(gasnet_team_handle_t team,
                       gasnet_image_t dstimage, void *dst,
                       void *src,
                       size_t nbytes, int flags, uint32_t sequence
                       GASNETI_THREAD_FARG);
GASNETI_COLL_FN_HEADER(_gasnet_coll_gather_nb) GASNETI_WARN_UNUSED_RESULT
gex_Event_t
_gasnet_coll_gather_nb(gasnet_team_handle_t team,
                       gasnet_image_t dstimage, void *dst,
                       void *src,
                       size_t nbytes, int flags GASNETI_THREAD_FARG) {
  gex_Event_t handle;
  GASNETI_TRACE_COLL_GATHER(COLL_GATHER_NB,team,dstimage,dst,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_GATHER(team,dstimage,dst,src,nbytes,flags);
  handle = gasnete_coll_gather_nb(team,dstimage,dst,src,nbytes,flags,0 GASNETI_THREAD_PASS);
  gasneti_AMPoll(); // No progress made until now
  return handle;
}

#ifdef gasnete_coll_gather
extern void
gasnete_coll_gather(gasnet_team_handle_t team,
                    gasnet_image_t dstimage, void *dst,
                    void *src,
                    size_t nbytes, int flags GASNETI_THREAD_FARG);
#else
GASNETI_COLL_FN_HEADER(gasnete_coll_gather)
     void gasnete_coll_gather(gasnet_team_handle_t team,
                              gasnet_image_t dstimage, void *dst,
                              void *src,
                              size_t nbytes, int flags GASNETI_THREAD_FARG) {
  gex_Event_t handle;
  handle = gasnete_coll_gather_nb(team,dstimage,dst,src,nbytes,flags,0 GASNETI_THREAD_PASS);
  gasnete_wait(handle GASNETI_THREAD_PASS);
}
#endif
GASNETI_COLL_FN_HEADER(_gasnet_coll_gather)
     void _gasnet_coll_gather(gasnet_team_handle_t team,
                              gasnet_image_t dstimage, void *dst,
                              void *src,
                              size_t nbytes, int flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_COLL_GATHER(COLL_GATHER,team,dstimage,dst,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_GATHER(team,dstimage,dst,src,nbytes,flags);
  gasnete_coll_gather(team,dstimage,dst,src,nbytes,flags GASNETI_THREAD_PASS);
}

/**** Gather ALL ***/
#ifndef gasnete_coll_gather_all_nb
#define gasnete_coll_gather_all_nb gasnete_coll_gather_all_nb_default
#else
extern gex_Event_t
gasnete_coll_gather_all_nb_default(gasnet_team_handle_t team,
                                   void *dst, void *src,
                                   size_t nbytes, int flags, uint32_t sequence
                                   GASNETI_THREAD_FARG);
#endif
extern gex_Event_t
gasnete_coll_gather_all_nb(gasnet_team_handle_t team,
                           void *dst, void *src,
                           size_t nbytes, int flags, uint32_t sequence
                           GASNETI_THREAD_FARG);
GASNETI_COLL_FN_HEADER(_gasnet_coll_gather_all_nb) GASNETI_WARN_UNUSED_RESULT
gex_Event_t
_gasnet_coll_gather_all_nb(gasnet_team_handle_t team,
                           void *dst, void *src,
                           size_t nbytes, int flags GASNETI_THREAD_FARG) {
  gex_Event_t handle;

  GASNETI_TRACE_COLL_GATHER_ALL(COLL_GATHER_ALL_NB,team,dst,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_GATHER_ALL(team,dst,src,nbytes,flags);
  handle = gasnete_coll_gather_all_nb(team,dst,src,nbytes,flags,0 GASNETI_THREAD_PASS);
  gasneti_AMPoll(); // No progress made until now
  return handle;
}

#ifdef gasnete_coll_gather_all
extern void
gasnete_coll_gather_all(gasnet_team_handle_t team,
                        void *dst, void *src,
                        size_t nbytes, int flags GASNETI_THREAD_FARG);
#else
GASNETI_COLL_FN_HEADER(gasnete_coll_gather_all)
     void gasnete_coll_gather_all(gasnet_team_handle_t team,
                                  void *dst, void *src,
                                  size_t nbytes, int flags GASNETI_THREAD_FARG) {
  gex_Event_t handle;
  handle = gasnete_coll_gather_all_nb(team,dst,src,nbytes,flags,0 GASNETI_THREAD_PASS);
  gasnete_wait(handle GASNETI_THREAD_PASS);
}
#endif
GASNETI_COLL_FN_HEADER(_gasnet_coll_gather_all)
     void _gasnet_coll_gather_all(gasnet_team_handle_t team,
                                  void *dst, void *src,
                                  size_t nbytes, int flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_COLL_GATHER_ALL(COLL_GATHER_ALL,team,dst,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_GATHER_ALL(team,dst,src,nbytes,flags);
  gasnete_coll_gather_all(team,dst,src,nbytes,flags GASNETI_THREAD_PASS);
}

/****** Exchange *********/

#ifndef gasnete_coll_exchange_nb
#define gasnete_coll_exchange_nb gasnete_coll_exchange_nb_default
#else
extern gex_Event_t
gasnete_coll_exchange_nb_default(gasnet_team_handle_t team,
                                 void *dst, void *src,
                                 size_t nbytes, int flags, uint32_t sequence
                                 GASNETI_THREAD_FARG);
#endif
extern gex_Event_t
gasnete_coll_exchange_nb(gasnet_team_handle_t team,
                         void *dst, void *src,
                         size_t nbytes, int flags, uint32_t sequence
                         GASNETI_THREAD_FARG);
GASNETI_COLL_FN_HEADER(_gasnet_coll_exchange_nb) GASNETI_WARN_UNUSED_RESULT
gex_Event_t
_gasnet_coll_exchange_nb(gasnet_team_handle_t team,
                         void *dst, void *src,
                         size_t nbytes, int flags GASNETI_THREAD_FARG) {
  gex_Event_t handle;
  GASNETI_TRACE_COLL_EXCHANGE(COLL_EXCHANGE_NB,team,dst,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_EXCHANGE(team,dst,src,nbytes,flags);
  handle = gasnete_coll_exchange_nb(team,dst,src,nbytes,flags,0 GASNETI_THREAD_PASS);
  gasneti_AMPoll(); // No progress made until now
  return handle;
}

#ifdef gasnete_coll_exchange
extern void
gasnete_coll_exchange(gasnet_team_handle_t team,
                      void *dst, void *src,
                      size_t nbytes, int flags GASNETI_THREAD_FARG);
#else
GASNETI_COLL_FN_HEADER(gasnete_coll_exchange)
     void gasnete_coll_exchange(gasnet_team_handle_t team,
                                void *dst, void *src,
                                size_t nbytes, int flags GASNETI_THREAD_FARG) {
  gex_Event_t handle;
  handle = gasnete_coll_exchange_nb(team,dst,src,nbytes,flags,0 GASNETI_THREAD_PASS);
  gasnete_wait(handle GASNETI_THREAD_PASS);
}
#endif
GASNETI_COLL_FN_HEADER(_gasnet_coll_exchange)
     void _gasnet_coll_exchange(gasnet_team_handle_t team,
                                void *dst, void *src,
                                size_t nbytes, int flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_COLL_EXCHANGE(COLL_EXCHANGE,team,dst,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_EXCHANGE(team,dst,src,nbytes,flags);
  gasnete_coll_exchange(team,dst,src,nbytes,flags GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_broadcast_nb() */

extern gex_Event_t
gasnete_coll_generic_broadcast_nb(gasnet_team_handle_t team,
                                  void *dst,
                                  gasnet_image_t srcimage, void *src,
                                  size_t nbytes, int flags,
                                  gasnete_coll_poll_fn poll_fn, int options,
                                  gasnete_coll_local_tree_geom_t *geom_info, uint32_t sequence,
                                  int num_params, uint32_t *param_list
                                  GASNETI_THREAD_FARG) {
  gex_Event_t result;
  gasnete_coll_scratch_req_t *scratch_req=NULL;
  
  /*fill out a scratch request "form" if you need scratch space with this operation*/
  if(options & (GASNETE_COLL_USE_SCRATCH)) {
    scratch_req = gasnete_coll_scratch_alloc_req(team);
    /*fill out the tree information*/
    scratch_req->tree_type = geom_info->tree_type;
    scratch_req->root = geom_info->root;
    scratch_req->tree_dir = GASNETE_COLL_DOWN_TREE;
    scratch_req->op_type = GASNETE_COLL_TREE_OP;
    /*fill out the peer information*/
	
    scratch_req->incoming_size = nbytes;
    if(team->myrank == geom_info->root) {
      scratch_req->num_in_peers = 0;
      scratch_req->in_peers = NULL;
    } else {
      scratch_req->num_in_peers = 1;
      scratch_req->in_peers = &(GASNETE_COLL_TREE_GEOM_PARENT(geom_info));
    }

    scratch_req->num_out_peers = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom_info);
    gasnete_coll_scratch_alloc_out_sizes(scratch_req, scratch_req->num_out_peers);
    for (int i = 0; i < scratch_req->num_out_peers; i++) {
      scratch_req->out_sizes[i] = nbytes;
    }
    scratch_req->out_peers = GASNETE_COLL_TREE_GEOM_CHILDREN(geom_info);
  }

  gasnete_coll_threads_lock(team, flags GASNETI_THREAD_PASS);
  const int first_thread = 1; // TODO-EX: multi-EP may need "first arrival" logic here
  
  if_pt (first_thread) {
    gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETI_THREAD_PASS_ALONE);
    GASNETE_COLL_GENERIC_SET_TAG(data, broadcast);
    data->args.broadcast.dst        = dst;
    data->args.broadcast.srcrank    = srcimage;
    data->args.broadcast.src        = src;
    data->args.broadcast.nbytes     = nbytes;
    data->options = options;
    data->tree_geom = geom_info;
    result = gasnete_coll_op_generic_init_with_scratch(team, flags, data, poll_fn, sequence, scratch_req, num_params, param_list, geom_info GASNETI_THREAD_PASS);
  } else {
    result = gasnete_coll_threads_get_handle(team GASNETI_THREAD_PASS);
  }
  gasnete_coll_threads_unlock(team GASNETI_THREAD_PASS);

  return result;
}



extern gex_Event_t
gasnete_coll_broadcast_nb_default(gasnet_team_handle_t team,
                                  void *dst,
                                  gasnet_image_t srcimage, void *src,
                                  size_t nbytes, int flags, uint32_t sequence
                                  GASNETI_THREAD_FARG)
{
  gasnete_coll_implementation_t impl;
  gex_Event_t ret;

  /* "Discover" in-segment flags if needed/possible */
  flags = gasnete_coll_segment_check(team, flags, 0, 0, dst, nbytes, 1, srcimage, src, nbytes);
  impl = gasnete_coll_autotune_get_bcast_algorithm(team, dst, srcimage, src, nbytes, flags GASNETI_THREAD_PASS);
  ret = (*((gasnete_coll_bcast_fn_ptr_t) (impl->fn_ptr)))(team, dst, srcimage, src, nbytes, flags, impl, sequence GASNETI_THREAD_PASS);
  if(impl->need_to_free) gasnete_coll_free_implementation(impl);
  return ret;
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_scatter_nb() */


extern gex_Event_t
gasnete_coll_generic_scatter_nb(gasnet_team_handle_t team,
                                void *dst,
                                gasnet_image_t srcimage, void *src,
                                size_t nbytes, size_t dist, int flags,
                                gasnete_coll_poll_fn poll_fn, int options,
                                gasnete_coll_local_tree_geom_t *geom_info, uint32_t sequence,
                                int num_params, uint32_t *param_list
                                GASNETI_THREAD_FARG) {
  gex_Event_t result;
  gasnete_coll_scratch_req_t *scratch_req=NULL;

  if(options & (GASNETE_COLL_USE_SCRATCH)) {
    scratch_req = gasnete_coll_scratch_alloc_req(team);
    /*fill out the tree information*/
    scratch_req->tree_type = geom_info->tree_type;
    scratch_req->root = geom_info->root;
    scratch_req->tree_dir = GASNETE_COLL_DOWN_TREE;
    scratch_req->op_type = GASNETE_COLL_TREE_OP;
    /*fill out the peer information*/
    scratch_req->incoming_size = nbytes*geom_info->mysubtree_size;

    if(team->myrank == geom_info->root) {
      scratch_req->num_in_peers = 0;
      scratch_req->in_peers = NULL;      
    } else {
      scratch_req->num_in_peers = 1;
      scratch_req->in_peers = &(GASNETE_COLL_TREE_GEOM_PARENT(geom_info));
    }
    scratch_req->num_out_peers = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom_info);
    gasnete_coll_scratch_alloc_out_sizes(scratch_req, scratch_req->num_out_peers);
    for (int i =0 ; i< scratch_req->num_out_peers; i++) {
      scratch_req->out_sizes[i] = nbytes*geom_info->subtree_sizes[i];
    }
    scratch_req->out_peers = GASNETE_COLL_TREE_GEOM_CHILDREN(geom_info);
  }
  
  gasnete_coll_threads_lock(team, flags GASNETI_THREAD_PASS);
  const int first_thread = 1; // TODO-EX: multi-EP may need "first arrival" logic here

  if_pt (first_thread) {
    gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETI_THREAD_PASS_ALONE);
    GASNETE_COLL_GENERIC_SET_TAG(data, scatter);
    data->args.scatter.dst        = dst;
    data->args.scatter.srcrank    = srcimage;
    data->args.scatter.src        = src;
    data->args.scatter.nbytes     = nbytes;
    data->args.scatter.dist     = dist;
    data->options = options;
    data->tree_geom = geom_info;
    result = gasnete_coll_op_generic_init_with_scratch(team, flags, data, poll_fn, sequence, scratch_req, num_params, param_list, geom_info GASNETI_THREAD_PASS);
  } else {
    result = gasnete_coll_threads_get_handle(team GASNETI_THREAD_PASS);
  }
  gasnete_coll_threads_unlock(team GASNETI_THREAD_PASS);
  return result;
}

extern gex_Event_t
gasnete_coll_scatter_nb_default(gasnet_team_handle_t team,
                                void *dst,
                                gasnet_image_t srcimage, void *src,
                                size_t nbytes, int flags, uint32_t sequence
                                GASNETI_THREAD_FARG)
{
  gasnete_coll_implementation_t impl;
  gex_Event_t ret;

  flags = gasnete_coll_segment_check(team, flags, 0, 0, dst, nbytes, 1, srcimage, src, nbytes*team->total_ranks);
  impl = gasnete_coll_autotune_get_scatter_algorithm(team, dst, srcimage, src, nbytes, nbytes, flags GASNETI_THREAD_PASS);
  ret = (*((gasnete_coll_scatter_fn_ptr_t) (impl->fn_ptr)))(team, dst, srcimage, src, nbytes, nbytes, flags, impl, sequence GASNETI_THREAD_PASS);
  if(impl->need_to_free) gasnete_coll_free_implementation(impl);
  return ret;
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_nb() */

extern gex_Event_t
gasnete_coll_generic_gather_nb(gasnet_team_handle_t team,
                               gasnet_image_t dstimage, void *dst,
                               void *src,
                               size_t nbytes, size_t dist, int flags,
                               gasnete_coll_poll_fn poll_fn, int options,
                               gasnete_coll_local_tree_geom_t *geom_info, uint32_t sequence,
                               int num_params, uint32_t *param_list
                               GASNETI_THREAD_FARG) {
  gex_Event_t result;
  gasnete_coll_scratch_req_t *scratch_req=NULL;
  
  if(options & (GASNETE_COLL_USE_SCRATCH)) {
    scratch_req = gasnete_coll_scratch_alloc_req(team);
    /*fill out the tree information*/
    scratch_req->tree_type = geom_info->tree_type;
    scratch_req->tree_dir = GASNETE_COLL_UP_TREE;
    scratch_req->root = geom_info->root;

    scratch_req->op_type = GASNETE_COLL_TREE_OP;
    /*fill out the peer information*/
    scratch_req->incoming_size = nbytes*geom_info->mysubtree_size;
    scratch_req->num_in_peers = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom_info);
    if(scratch_req->num_in_peers > 0) {
      scratch_req->in_peers = GASNETE_COLL_TREE_GEOM_CHILDREN(geom_info);      
    } else {
      scratch_req->in_peers = NULL;
    }
    if(team->myrank == dstimage) {
      scratch_req->num_out_peers = 0;
      scratch_req->out_peers = NULL;      
      scratch_req->out_sizes = NULL;
    }
    else {
      scratch_req->num_out_peers = 1;
      gasnete_coll_scratch_alloc_out_sizes(scratch_req, 1);
      scratch_req->out_sizes[0] = nbytes*geom_info->parent_subtree_size;
      scratch_req->out_peers = &(GASNETE_COLL_TREE_GEOM_PARENT(geom_info));
    }
  }
  
  gasnete_coll_threads_lock(team, flags GASNETI_THREAD_PASS);
  const int first_thread = 1; // TODO-EX: multi-EP may need "first arrival" logic here
  
  if_pt (first_thread) {
    gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETI_THREAD_PASS_ALONE);
    GASNETE_COLL_GENERIC_SET_TAG(data, gather);
    data->args.gather.dstrank    = dstimage;
    data->args.gather.dst        = dst;
    data->args.gather.src        = src;
    data->args.gather.nbytes     = nbytes;
    data->args.gather.dist     = dist;
    data->options = options;
    data->private_data = NULL;
    data->tree_geom = geom_info;
    result = gasnete_coll_op_generic_init_with_scratch(team, flags, data, poll_fn, sequence, scratch_req, num_params, param_list, geom_info GASNETI_THREAD_PASS);
  } else {
    result = gasnete_coll_threads_get_handle(team GASNETI_THREAD_PASS);
  }
  gasnete_coll_threads_unlock(team GASNETI_THREAD_PASS);
  return result;
}
extern gex_Event_t
gasnete_coll_gather_nb_default(gasnet_team_handle_t team,
                               gasnet_image_t dstimage, void *dst,
                               void *src,
                               size_t nbytes, int flags, uint32_t sequence
                               GASNETI_THREAD_FARG)
{
  gasnete_coll_implementation_t impl;
  gex_Event_t ret;

  /* "Discover" in-segment flags if needed/possible */
  flags = gasnete_coll_segment_check(team, flags, 1, dstimage, dst, nbytes*team->total_ranks,
                                     0, 0, src, nbytes);
  
  impl = gasnete_coll_autotune_get_gather_algorithm(team,dstimage, dst, src, 
                                                    nbytes, nbytes, flags  GASNETI_THREAD_PASS);
  
  ret = (*((gasnete_coll_gather_fn_ptr_t) (impl->fn_ptr)))(team, dstimage, dst, src, nbytes, nbytes, flags, impl, sequence GASNETI_THREAD_PASS);
  if(impl->need_to_free) gasnete_coll_free_implementation(impl);
  return ret;
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_all_nb() */

/* gall Gath: Implement gather_all in terms of simultaneous gathers */
/* This is meant mostly as an example and a short-term solution */
/* Valid wherever the underlying gather is valid */
static int gasnete_coll_pf_gall_Gath(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_all_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_all);
  int result = 0;

  switch (data->state) {
  case 0:	/* Optional IN barrier */
    if (!gasnete_coll_generic_all_threads(data) ||
        !gasnete_coll_generic_insync(op->team, data)) {
      break;
    }
    data->state = 1; GASNETI_FALLTHROUGH

  case 1:	/* Initiate data movement */
    {
      gex_Event_t *h;
      int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
      gasnet_team_handle_t team = op->team;
      void *dst = args->dst;
      void *src = args->src;
      size_t nbytes = args->nbytes;
      gasnet_image_t i;

      /* XXX: freelist ? */
      h = gasneti_malloc(op->team->total_ranks * sizeof(gex_Event_t));
      data->private_data = h;

      for (i = 0; i < op->team->total_ranks; ++i, ++h) {
        *h = gasnete_coll_gather_nb(team, i, dst, src, nbytes,
                                    flags|GASNETE_COLL_NONROOT_SUBORDINATE|GASNET_COLL_DISABLE_AUTOTUNE, op->sequence+i+1 GASNETI_THREAD_PASS);
        gasnete_coll_save_event(h);
      }
    }
    data->state = 2; GASNETI_FALLTHROUGH

  case 2:	/* Sync data movement */
    if (!gasnete_coll_generic_coll_sync(data->private_data, op->team->total_ranks GASNETI_THREAD_PASS)) {
      break;
    }
    data->state = 3; GASNETI_FALLTHROUGH

  case 3:	/* Optional OUT barrier */
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      break;
    }

    gasneti_free(data->private_data);
    gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gex_Event_t
gasnete_coll_gall_Gath(gasnet_team_handle_t team,
                       void *dst, void *src,
                       size_t nbytes, int flags, 
                       gasnete_coll_implementation_t coll_params,
                       uint32_t sequence
                       GASNETI_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  if(flags & GASNETE_COLL_SUBORDINATE) 
    return gasnete_coll_generic_gather_all_nb(team, dst, src, nbytes, flags,
                                              &gasnete_coll_pf_gall_Gath, options,
                                              NULL, sequence,
                                              coll_params->num_params, coll_params->param_list 
                                              GASNETI_THREAD_PASS); 
  else {
    return gasnete_coll_generic_gather_all_nb(team, dst, src, nbytes, flags,
                                              &gasnete_coll_pf_gall_Gath, options,
                                              NULL, team->total_ranks,
                                              coll_params->num_params, coll_params->param_list
                                              GASNETI_THREAD_PASS); 
  }
}

extern gex_Event_t
gasnete_coll_generic_gather_all_nb(gasnet_team_handle_t team,
                                   void *dst, void *src,
                                   size_t nbytes, int flags,
                                   gasnete_coll_poll_fn poll_fn, int options,
                                   void *private_data,uint32_t sequence,
                                   int num_params, uint32_t *param_list
                                   GASNETI_THREAD_FARG) {
  gex_Event_t result;
  gasnete_coll_scratch_req_t *scratch_req=NULL;
  gasnete_coll_dissem_info_t *dissem = gasnete_coll_fetch_dissemination(2,team);
  
  if(options & (GASNETE_COLL_USE_SCRATCH)) {
    /*fill out a scratch request form*/	
    scratch_req = gasnete_coll_scratch_alloc_req(team);
    scratch_req->op_type = GASNETE_COLL_DISSEM_OP;
    scratch_req->tree_dir = GASNETE_COLL_UP_TREE;
    scratch_req->tree_type = NULL;
    scratch_req->incoming_size = 
      nbytes*team->total_ranks;
    scratch_req->num_out_peers = scratch_req->num_in_peers = GASNETE_COLL_DISSEM_GET_PEER_COUNT(dissem);
    gasnete_coll_scratch_alloc_out_sizes(scratch_req, 1);
    scratch_req->out_sizes[0] = scratch_req->incoming_size;
    scratch_req->out_peers = GASNETE_COLL_DISSEM_GET_BEHIND_PEERS(dissem);
    scratch_req->in_peers = GASNETE_COLL_DISSEM_GET_FRONT_PEERS(dissem);
  }  

  gasnete_coll_threads_lock(team, flags GASNETI_THREAD_PASS);
  const int first_thread = 1; // TODO-EX: multi-EP may need "first arrival" logic here
  
  if_pt (first_thread) {
    gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETI_THREAD_PASS_ALONE);
    GASNETE_COLL_GENERIC_SET_TAG(data, gather_all);
    data->args.gather_all.dst     = dst;
    data->args.gather_all.src     = src;
    data->args.gather_all.nbytes  = nbytes;
    data->options = options;
    data->private_data = private_data;
    data->tree_geom = NULL;
    data->dissem_info = dissem;
    result = gasnete_coll_op_generic_init_with_scratch(team, flags, data, poll_fn, sequence, scratch_req, num_params, param_list, NULL GASNETI_THREAD_PASS);
  } else {
    result = gasnete_coll_threads_get_handle(team GASNETI_THREAD_PASS);
  }
  gasnete_coll_threads_unlock(team GASNETI_THREAD_PASS);
  return result;
}

extern gex_Event_t
gasnete_coll_gather_all_nb_default(gasnet_team_handle_t team,
                                   void *dst, void *src,
                                   size_t nbytes, int flags, uint32_t sequence
                                   GASNETI_THREAD_FARG) {
  gasnete_coll_implementation_t impl;
  gex_Event_t ret;

  flags = gasnete_coll_segment_check(team, flags, 0, 0, dst, nbytes*team->total_ranks,
                                     0, 0, src, nbytes);
  
  impl = gasnete_coll_autotune_get_gather_all_algorithm(team, dst, src, 
                                                        nbytes, flags  GASNETI_THREAD_PASS);
  
  ret =  (*((gasnete_coll_gather_all_fn_ptr_t) (impl->fn_ptr)))(team, dst, src, nbytes, flags, impl, sequence GASNETI_THREAD_PASS);
  if(impl->need_to_free) gasnete_coll_free_implementation(impl);
  
  return ret;
  
}


/*---------------------------------------------------------------------------------*/
/* gasnete_coll_exchange_nb() */

/* exchg Gath: Implement exchange in terms of simultaneous gathers */
/* This is meant mostly as an example and a short-term solution */
/* Valid wherever the underlying gather is valid */
static int gasnete_coll_pf_exchg_Gath(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_exchange_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, exchange);
  int result = 0;

  switch (data->state) {
  case 0:	/* Optional IN barrier */
    if (!gasnete_coll_generic_all_threads(data) ||
        !gasnete_coll_generic_insync(op->team, data)) {
      break;
    }
    data->state = 1; GASNETI_FALLTHROUGH

  case 1:	/* Initiate data movement */
    {
      gex_Event_t *h;
      int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
      gasnet_team_handle_t team = op->team;
      void *dst = args->dst;
      uintptr_t src_addr = (uintptr_t)args->src;
      size_t nbytes = args->nbytes;
      gasnet_image_t i;

      /* XXX: freelist ? */
      h = gasneti_malloc(team->total_ranks * sizeof(gex_Event_t));
      data->private_data = h;

      for (i = 0; i < team->total_ranks; ++i, ++h, src_addr += nbytes) {
        *h = gasnete_coll_gather_nb(team, i, dst, (void *)src_addr, nbytes,
                                    flags|GASNETE_COLL_NONROOT_SUBORDINATE|GASNET_COLL_DISABLE_AUTOTUNE, op->sequence+i+1 GASNETI_THREAD_PASS);
        gasnete_coll_save_event(h);
      }
    }
    data->state = 2; GASNETI_FALLTHROUGH

  case 2:	/* Sync data movement */
    if (!gasnete_coll_generic_coll_sync(data->private_data, op->team->total_ranks GASNETI_THREAD_PASS)) {
      break;
    }
    data->state = 3; GASNETI_FALLTHROUGH

  case 3:	/* Optional OUT barrier */
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      break;
    }

    gasneti_free(data->private_data);
    gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gex_Event_t
gasnete_coll_exchg_Gath(gasnet_team_handle_t team,
                        void *dst, void *src,
                        size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                        GASNETI_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));

  return gasnete_coll_generic_exchange_nb(team, dst, src, nbytes, flags,
                                          &gasnete_coll_pf_exchg_Gath, options,
                                          NULL, NULL, team->total_ranks, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}

extern gex_Event_t
gasnete_coll_generic_exchange_nb(gasnet_team_handle_t team,
                                 void *dst, void *src,
                                 size_t nbytes, int flags,
                                 gasnete_coll_poll_fn poll_fn, int options,
                                 void *private_data, gasnete_coll_dissem_info_t* dissem, uint32_t sequence,
                                 int num_params, uint32_t *param_list
                                 GASNETI_THREAD_FARG) {
  gex_Event_t result;
  gasnete_coll_scratch_req_t *scratch_req=NULL;
  if(options & GASNETE_COLL_USE_SCRATCH) {
    /*fill out a scratch request form*/	
    scratch_req = gasnete_coll_scratch_alloc_req(team);
    scratch_req->op_type = GASNETE_COLL_DISSEM_OP;
    scratch_req->tree_dir = GASNETE_COLL_DOWN_TREE;
    scratch_req->tree_type = NULL;
    scratch_req->incoming_size = 
      nbytes*team->total_ranks+
      (nbytes*dissem->max_dissem_blocks*2*(dissem->dissemination_radix-1));   
    scratch_req->num_out_peers = scratch_req->num_in_peers = GASNETE_COLL_DISSEM_GET_PEER_COUNT(dissem);
    gasnete_coll_scratch_alloc_out_sizes(scratch_req, 1);
    scratch_req->out_sizes[0] = scratch_req->incoming_size;
    scratch_req->out_peers = GASNETE_COLL_DISSEM_GET_FRONT_PEERS(dissem);
    scratch_req->in_peers = GASNETE_COLL_DISSEM_GET_BEHIND_PEERS(dissem);
  }
  
  gasnete_coll_threads_lock(team, flags GASNETI_THREAD_PASS);
  const int first_thread = 1; // TODO-EX: multi-EP may need "first arrival" logic here
  
  if_pt (first_thread) {
    gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETI_THREAD_PASS_ALONE);
    GASNETE_COLL_GENERIC_SET_TAG(data, exchange);
    data->args.exchange.dst     = dst;
    data->args.exchange.src     = src;
    data->args.exchange.nbytes  = nbytes;
    data->options = options;
    data->private_data = private_data;
    data->tree_geom = NULL;
    data->dissem_info = dissem;
    result = gasnete_coll_op_generic_init_with_scratch(team, flags, data, poll_fn, sequence, scratch_req, num_params, param_list, NULL GASNETI_THREAD_PASS);
  } else {
    result = gasnete_coll_threads_get_handle(team GASNETI_THREAD_PASS);
  }
  gasnete_coll_threads_unlock(team GASNETI_THREAD_PASS);


  return result;
}

extern gex_Event_t
gasnete_coll_exchange_nb_default(gasnet_team_handle_t team,
                                 void *dst, void *src,
                                 size_t nbytes, int flags, uint32_t sequence
                                 GASNETI_THREAD_FARG)
{
  gasnete_coll_implementation_t impl;
  gex_Event_t ret;

  /* "Discover" in-segment flags if needed/possible */
  flags = gasnete_coll_segment_check(team, flags, 0, 0, dst, nbytes*team->total_ranks,
                                     0, 0, src, nbytes*team->total_ranks);
  
  impl = gasnete_coll_autotune_get_exchange_algorithm(team, dst, src, nbytes, flags GASNETI_THREAD_PASS);
  ret =  (*((gasnete_coll_exchange_fn_ptr_t) (impl->fn_ptr)))(team, dst, src, nbytes, flags, impl, sequence GASNETI_THREAD_PASS);
  if(impl->need_to_free) gasnete_coll_free_implementation(impl);
  return ret;
}

/*---------------------------------------------------------------------------------*/
// Barrier

#ifndef gasnete_tm_barrier_nb
  // In absence of conduit override we drop the _default suffix
  #define gasnete_tm_barrier_nb_default gasnete_tm_barrier_nb
#endif
static int gasnete_coll_pf_barrier(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnet_team_handle_t team = op->team;
  gasnete_coll_consensus_t id = (gasnete_coll_consensus_t)(uintptr_t)op->data;
  int done = gasnete_coll_consensus_try(team, id) == GASNET_OK;
  return done ? (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE) : 0;
}
extern gex_Event_t
gasnete_tm_barrier_nb_default(gex_TM_t e_tm, gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasnet_team_handle_t team = gasneti_import_tm(e_tm)->_coll_team;
  const int coll_flags = 0;
  gex_Event_t result;

  gasnete_coll_threads_lock(team, coll_flags GASNETI_THREAD_PASS);

  gasnete_coll_op_t *op = gasnete_coll_op_create(team, 0, coll_flags GASNETI_THREAD_PASS);
  op->poll_fn = &gasnete_coll_pf_barrier;
  op->flags = coll_flags;

  gasnete_coll_consensus_t id = gasnete_coll_consensus_create(team);
  gasneti_assert(sizeof(id) <= sizeof(op->data));
  op->data = (void *)(uintptr_t)id;

  gasnete_coll_eop_t eop = gasnete_coll_eop_create(GASNETI_THREAD_PASS_ALONE);
  gasnete_coll_op_submit(op, eop GASNETI_THREAD_PASS);
  result = GASNETE_COLL_EOP_TO_EVENT(eop);

  gasnete_coll_threads_unlock(team GASNETI_THREAD_PASS);

  gasneti_AMPoll(); // No progress made until now
  return result;
}

/*---------------------------------------------------------------------------------*/
// Barrier (undocumented Blocking variant)

#ifndef gasnete_tm_barrier
  // In absence of conduit override we drop the _default suffix
  #define gasnete_tm_barrier_default gasnete_tm_barrier
#endif
extern void
gasnete_tm_barrier_default(gex_TM_t e_tm, gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasnet_team_handle_t team = gasneti_import_tm(e_tm)->_coll_team;
  gasnete_coll_consensus_barrier(team GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
// GEX Broadcast
//
// TODO-EX: replace this THUNK w/ something implementing only the GEX
// semantics (e.g. no sync flags or single-valued addr) and using GEX-ified
// poll_fn (e.g. use of IMMEDIATE and NP-AM).

#ifndef gasnete_tm_broadcast_nb
  // In absence of conduit override we drop the _default suffix
  #define gasnete_tm_broadcast_nb_default gasnete_tm_broadcast_nb
#endif
gex_Event_t
gasnete_tm_broadcast_nb_default(gex_TM_t e_tm, gex_Rank_t root,
                                void *dst, const void *src,
                                size_t nbytes, gex_Flags_t flags,
                                uint32_t sequence GASNETI_THREAD_FARG)
{
  gasnet_team_handle_t team = gasneti_import_tm(e_tm)->_coll_team;
  int coll_flags = GASNET_COLL_LOCAL | GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC;
  coll_flags |= (flags & GASNETI_FLAG_COLL_SUBORDINATE) ? GASNETE_COLL_SUBORDINATE : 0;
  return gasnete_coll_broadcast_nb(team, dst, root, (/*non-const*/ void*)src,
                                   nbytes, coll_flags, sequence GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
// GEX Reduce

gex_Event_t
gasnete_tm_generic_reduce_nb(gex_TM_t tm, gex_Rank_t root, void *dst, const void *src,
                             gex_DT_t dt, size_t dt_sz, size_t dt_cnt,
                             gex_OP_t opcode, gex_Coll_ReduceFn_t fnptr, void *cdata,
                             int coll_flags, gasnete_coll_poll_fn poll_fn, int options,
                             gasnete_coll_local_tree_geom_t *geom_info, uint32_t sequence,
                             int num_params, uint32_t *param_list,
                             gasnete_coll_scratch_req_t *scratch_req
                             GASNETI_THREAD_FARG)
{
  gasnet_team_handle_t team = gasneti_import_tm(tm)->_coll_team;
  gex_Event_t result;

  gasnete_coll_threads_lock(team, coll_flags GASNETI_THREAD_PASS);

  gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETI_THREAD_PASS_ALONE);
  GASNETE_COLL_GENERIC_SET_TAG(data, tm_reduce);

  data->args.tm_reduce.root    = root;
  data->args.tm_reduce.dst     = dst;
  data->args.tm_reduce.src     = src;

  data->args.tm_reduce.dt      = dt;
  data->args.tm_reduce.dt_sz   = dt_sz;
  data->args.tm_reduce.dt_cnt  = dt_cnt;

  switch (opcode) {
    case GEX_OP_USER_NC:
      gasneti_fatalerror("Support for GEX_OP_USER_NC reductions is UNIMPLEMENTED");
      break;

    case GEX_OP_USER:
      data->args.tm_reduce.op_fnptr  = fnptr;
      data->args.tm_reduce.op_cdata  = cdata;
      break;

    // Otherwise convert DT/OP pair to an *internal* fnptr and cdata
    // TODO-EX: this just selects on DT and smuggles the opcode in the
    // cdata, which then leaves a switch(opcode) in the critical path.
    default:
      data->args.tm_reduce.op_cdata = (void*)(uintptr_t)opcode;
      switch (dt) {
        #define REDUCE_OP_CASE(DT) \
          case GEX_DT_##DT:                                                  \
             data->args.tm_reduce.op_fnptr =  gasnete_shrinkray_gex_dt_##DT; \
             break;
        GASNETE_TM_REDUCE_FOREACH_DT(REDUCE_OP_CASE)
        #undef REDUCE_OP_CASE

        default: gasneti_unreachable();
      }
      break;
  }

  data->options      = options;
  data->private_data = NULL;
  data->tree_geom    = geom_info;

  result = gasnete_coll_op_generic_init_with_scratch(team, coll_flags, data, poll_fn, sequence, scratch_req, num_params, param_list, geom_info GASNETI_THREAD_PASS);

  gasnete_coll_threads_unlock(team GASNETI_THREAD_PASS);

  return result;
}


#ifndef gasnete_tm_reduce_nb
  // In absence of conduit override we drop the _default suffix
  #define gasnete_tm_reduce_nb_default gasnete_tm_reduce_nb
#endif
gex_Event_t
gasnete_tm_reduce_nb_default(
                gex_TM_t e_tm, gex_Rank_t root,
                void *dst, const void *src,
                gex_DT_t dt, size_t dt_sz, size_t dt_cnt,
                gex_OP_t opcode, gex_Coll_ReduceFn_t user_fnptr, void *user_cdata,
                gex_Flags_t flags, uint32_t sequence GASNETI_THREAD_FARG)
{
  gasneti_TM_t i_tm = gasneti_import_tm(e_tm);

  GASNETI_TRACE_TM_REDUCE(COLL_REDUCE_NB,e_tm,root,dst,src,dt,dt_sz,dt_cnt,opcode,user_fnptr,user_cdata,flags);

  // Argument validation
  // TODO-EX: factor to avoid cloning this logic to conduit collectives
  // TODO-EX: informative fatalerror() in place of assertion failure
  gasneti_assert(root < i_tm->_size);
  gasneti_assert((root != i_tm->_rank) || dst);
  gasneti_assert(src);
  gasneti_assert(dt_sz != 0);
  gasneti_assert(dt_cnt != 0);
  gasneti_assert((dt == GEX_DT_USER) || (dt_sz == gasneti_dt_size(dt)));
  gasneti_assert(gasneti_dt_valid_reduce(dt));
  gasneti_assert(gasneti_op_valid_reduce(opcode));
  gasneti_assert((dt == GEX_DT_USER) ||
                 (gasneti_dt_int(dt) && gasneti_op_int(opcode)) ||
                 (gasneti_dt_fp(dt)  && gasneti_op_fp(opcode)));

  // Short-circuit singleton
  // TODO-EX:  hoist to gasnet_coll.h?
  if (i_tm->_size == 1) {
    GASNETI_MEMCPY_SAFE_IDENTICAL(dst, src, dt_sz * dt_cnt);
    return GEX_EVENT_INVALID;
  }

  // TODO-EX: LUB can be relaxed (potentially significantly) for pshm-only teams
  gasnete_coll_team_t team = i_tm->_coll_team;
  const size_t nbytes = dt_sz * dt_cnt;
  gasnete_coll_local_tree_geom_t *geom = NULL;
  gasnete_tm_reduce_fn_ptr_t alg;
  const int binomial_root_radix = 1 + gasnete_coll_log2_rank(i_tm->_size - 1);
  if ((nbytes * binomial_root_radix <= team->p2p_eager_buffersz) &&
      (nbytes <= gex_AM_LUBRequestMedium())) {
    alg = &gasnete_tm_reduce_BinomialEager;
  } else {
    gasnete_coll_team_t team = i_tm->_coll_team;
    const size_t smallest_scratch = team->scratch_size;
    geom = gasnete_coll_local_tree_geom_fetch(gasnetc_tm_reduce_tree_type, root, team);
    const gex_Rank_t max_radix = geom->max_radix;
    if ((nbytes * max_radix <= smallest_scratch) && (nbytes <= gex_AM_LUBRequestLong())) {
      alg = &gasnete_tm_reduce_TreePut;
    } else if ((dt_sz * (max_radix + 1) <= smallest_scratch) && (dt_sz <= gex_AM_LUBRequestLong())) {
      alg = &gasnete_tm_reduce_TreePutSeg;
    } else if ((dt_sz * binomial_root_radix <= team->p2p_eager_buffersz) &&
               (dt_sz <= gex_AM_LUBRequestMedium())) {
      alg = &gasnete_tm_reduce_BinomialEagerSeg;
    } else {
      gasneti_assert(dt == GEX_DT_USER);
      gasneti_fatalerror("gex_Coll_ReduceToOneNB: (dt_sz == %"PRIuSZ") is TOO LARGE for this implementation",
                         dt_sz);
    }
  }
  
  // TODO-EX: stop abusing implementation_t argument to pass the geom
  int coll_flags = (flags & GASNETI_FLAG_COLL_SUBORDINATE) ? GASNETE_COLL_SUBORDINATE : 0;
  gex_Event_t result =
         (*alg)(e_tm, root, dst, src,
                dt, dt_sz, dt_cnt,
                opcode, user_fnptr, user_cdata,
                coll_flags, (void*)geom, sequence GASNETI_THREAD_PASS);

  gasneti_AMPoll(); // No progress made until now
  return result;
}

/*---------------------------------------------------------------------------------*/
// GEX Reduce to All

gex_Event_t
gasnete_tm_generic_reduce_all_nb(
                        gex_TM_t tm, void *dst, const void *src,
                        gex_DT_t dt, size_t dt_sz, size_t dt_cnt,
                        gex_OP_t opcode, gex_Coll_ReduceFn_t fnptr, void *cdata,
                        int coll_flags, gasnete_coll_poll_fn poll_fn, int options,
                        gasnete_coll_local_tree_geom_t *geom_info, uint32_t sequence,
                        int num_params, uint32_t *param_list,
                        gasnete_coll_scratch_req_t *scratch_req
                        GASNETI_THREAD_FARG)
{
  gasnet_team_handle_t team = gasneti_import_tm(tm)->_coll_team;
  gex_Event_t result;

  gasnete_coll_threads_lock(team, coll_flags GASNETI_THREAD_PASS);

  gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETI_THREAD_PASS_ALONE);
  GASNETE_COLL_GENERIC_SET_TAG(data, tm_reduce_all);

  data->args.tm_reduce_all.dst     = dst;
  data->args.tm_reduce_all.src     = src;

  data->args.tm_reduce_all.dt      = dt;
  data->args.tm_reduce_all.dt_sz   = dt_sz;
  data->args.tm_reduce_all.dt_cnt  = dt_cnt;

  data->args.tm_reduce_all.opcode  = opcode;

  switch (opcode) {
    case GEX_OP_USER_NC:
      gasneti_fatalerror("Support for GEX_OP_USER_NC reductions is UNIMPLEMENTED");
      break;

    case GEX_OP_USER:
      data->args.tm_reduce_all.op_fnptr  = fnptr;
      data->args.tm_reduce_all.op_cdata  = cdata;
      break;

    // Otherwise convert DT/OP pair to an *internal* fnptr and cdata
    // TODO-EX: this just selects on DT and smuggles the opcode in the
    // cdata, which then leaves a switch(opcode) in the critical path.
    default:
      data->args.tm_reduce_all.op_cdata = (void*)(uintptr_t)opcode;
      switch (dt) {
        #define REDUCE_OP_CASE(DT) \
          case GEX_DT_##DT:                                                  \
             data->args.tm_reduce_all.op_fnptr =  gasnete_shrinkray_gex_dt_##DT; \
             break;
        GASNETE_TM_REDUCE_FOREACH_DT(REDUCE_OP_CASE)
        #undef REDUCE_OP_CASE

        default: gasneti_unreachable();
      }
      break;
  }

  data->options      = options;
  data->private_data = NULL;
  data->tree_geom    = geom_info;

  result = gasnete_coll_op_generic_init_with_scratch(team, coll_flags, data, poll_fn, sequence, scratch_req, num_params, param_list, geom_info GASNETI_THREAD_PASS);

  gasnete_coll_threads_unlock(team GASNETI_THREAD_PASS);

  return result;
}

#ifndef gasnete_tm_reduce_all_nb
  // In absence of conduit override we drop the _default suffix
  #define gasnete_tm_reduce_all_nb_default gasnete_tm_reduce_all_nb
#endif
gex_Event_t
gasnete_tm_reduce_all_nb_default(
                gex_TM_t e_tm,
                void *dst, const void *src,
                gex_DT_t dt, size_t dt_sz, size_t dt_cnt,
                gex_OP_t opcode, gex_Coll_ReduceFn_t user_fnptr, void *user_cdata,
                gex_Flags_t flags, uint32_t sequence GASNETI_THREAD_FARG)
{
  gasneti_TM_t i_tm = gasneti_import_tm(e_tm);

  GASNETI_TRACE_TM_REDUCE_ALL(COLL_REDUCE_ALL_NB,e_tm,dst,src,dt,dt_sz,dt_cnt,opcode,user_fnptr,user_cdata,flags);

  // Argument validation
  // TODO-EX: factor to avoid cloning this logic to conduit collectives
  // TODO-EX: informative fatalerror() in place of assertion failure
  gasneti_assert(src);
  gasneti_assert(dst);
  // Note GASNETI_MEMCPY_SAFE_IDENTICAL will check src/dst overlap
  gasneti_assert(dt_sz != 0);
  gasneti_assert(dt_cnt != 0);
  gasneti_assert((dt == GEX_DT_USER) || (dt_sz == gasneti_dt_size(dt)));
  gasneti_assert(gasneti_dt_valid_reduce(dt));
  gasneti_assert(gasneti_op_valid_reduce(opcode));
  gasneti_assert((dt == GEX_DT_USER) ||
                 (gasneti_dt_int(dt) && gasneti_op_int(opcode)) ||
                 (gasneti_dt_fp(dt)  && gasneti_op_fp(opcode)));

  // Short-circuit singleton
  // TODO-EX:  hoist to gasnet_coll.h?
  if (i_tm->_size == 1) {
    GASNETI_MEMCPY_SAFE_IDENTICAL(dst, src, dt_sz * dt_cnt);
    return GEX_EVENT_INVALID;
  }

  // TODO-EX: replace this correct-but-horrible implementation:
  gex_Event_t result =
         gasnete_tm_reduce_all_Bcast(e_tm, dst, src, dt, dt_sz, dt_cnt,
                                     opcode, user_fnptr, user_cdata,
                                     0, NULL, 0 GASNETI_THREAD_PASS);

  gasneti_AMPoll(); // No progress made until now
  return result;
}

/*---------------------------------------------------------------------------------*/

#if GASNET_DEBUG
extern void gasnete_coll_stat(void) {
  GASNET_BEGIN_FUNCTION();
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
  int used = gasnete_coll_event_list.used;
  gasnete_coll_op_t *op;
  fprintf(stderr, "%d> %d events used\n", (int)gasneti_mynode, used);

  if (used) {
    for (int i = 0; i < used; ++i) {
      fprintf(stderr, "EVENT %p\n", (void *)gasnete_coll_event_list.events[i]);
    }
  }

  /* gasneti_mutex_lock(&gasnete_coll_active_lock); */
  op = gasnete_coll_active_first();
  while (op) {
    gasnete_coll_generic_data_t *data = op->data;
    fprintf(stderr, "OP: %p in state %d\n", (void *)op, data->state);
    op = gasnete_coll_active_next(op);
  }
  /* gasneti_mutex_unlock(&gasnete_coll_active_lock); */
}
#endif

/*---------------------------------------------------------------------------------*/

