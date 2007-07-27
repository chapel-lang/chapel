/*   $Source$
 *     $Date$
 * $Revision$
 * Description: Reference implemetation of GASNet Collectives
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_extended_refcoll.h>
#include <gasnet_coll.h>
#include <gasnet_vis.h>

/*---------------------------------------------------------------------------------*/
/* Forward decls and macros */

#if GASNET_PAR
  #define GASNETE_COLL_FORWARD_FLAGS(flags) \
	(((flags) & ~(GASNET_COLL_IN_ALLSYNC|GASNET_COLL_IN_MYSYNC|\
		      GASNET_COLL_OUT_ALLSYNC|GASNET_COLL_OUT_MYSYNC|\
		      GASNET_COLL_AGGREGATE|GASNETE_COLL_THREAD_LOCAL)) \
	  | (GASNET_COLL_IN_NOSYNC|GASNET_COLL_OUT_NOSYNC|GASNETE_COLL_SUBORDINATE))
#else
  #define GASNETE_COLL_FORWARD_FLAGS(flags) \
	(((flags) & ~(GASNET_COLL_IN_ALLSYNC|GASNET_COLL_IN_MYSYNC|\
		      GASNET_COLL_OUT_ALLSYNC|GASNET_COLL_OUT_MYSYNC|\
		      GASNET_COLL_AGGREGATE)) \
	  | (GASNET_COLL_IN_NOSYNC|GASNET_COLL_OUT_NOSYNC|GASNETE_COLL_SUBORDINATE))
#endif

/* XXX: Until we have gasnete_poll, a thread will only poll the collectives when
   it does a try/wait on a collective handle.  So we must ensure a polling thread
   does not initiate any RDMA or subordinate collective operation unless we can be
   certain it will continue to poll until the RDMA or subordinate is done.
   The cases we must protect against include threads that get an INVALID_HANDLE
   for one or more ops and therefore may cease to poll before all collective ops
   are completed.
   The solution used here is to only allow a poller to initiate RDMA and/or
   subordinate operations if it has a handle for the current operation.  While
   we can't always know for sure, it is sufficient to only initiate when we
   can *prove* the current thread has a handle for the current op:
    + The thread that initiated to op is the "owner" and must have a handle.
      Assigning ownership in this manner ensures that there is always at least
      one thread that may make progress on each outstanding op.  In some cases
      this is the only thread which is certain to poll again, so tracking of
      the "owner" is required.
    + (flags & (OUT_MYSYNC|OUT_ALLSYNC)) implies that ALL threads must have a
      handle for this op (note that subordinate ops are initiated by a single
      thread, but this is OK since they always have NO/NO sync flags).  This
      is the only current improvement over the older CHECK_OWNER tests.
      The MY case won't work when we begin to signal threads individially as
      their data is delivered/consumed, but gasnete_poll() should be done
      before that.
   None of this is needed once gasnete_poll() will ensure that gasneti_AMPoll()
   will poll collectives as long as any remain unfinished.
*/
#if GASNETI_USE_TRUE_MUTEXES
  #define GASNETE_COLL_MAY_INIT_FOR(op)	((GASNETE_COLL_GENERIC_DATA(op)->owner == GASNETE_MYTHREAD) || \
					 ((op)->flags & (GASNET_COLL_OUT_MYSYNC | GASNET_COLL_OUT_ALLSYNC)))
  #define GASNETE_COLL_SET_OWNER(data)	(data)->owner = GASNETE_MYTHREAD
#else
  #define GASNETE_COLL_MAY_INIT_FOR(op)	1
  #if GASNET_DEBUG
    #define GASNETE_COLL_SET_OWNER(data)	(data)->owner = GASNETE_MYTHREAD
  #else
    #define GASNETE_COLL_SET_OWNER(data)	do {} while (0)
  #endif
#endif


/*---------------------------------------------------------------------------------*/
/* XXX: sequence and other stuff that will need to be per-team scoped: */

gasnet_coll_fn_entry_t *gasnete_coll_fn_tbl;
size_t gasnete_coll_fn_count;
uint32_t gasnete_coll_sequence = 12345;	/* arbitrary non-zero starting value */
gasnet_image_t *gasnete_coll_all_images;
gasnet_image_t *gasnete_coll_all_offset;
gasnet_image_t gasnete_coll_total_images;
gasnet_image_t gasnete_coll_max_images;
gasnet_image_t gasnete_coll_my_images;	/* count of local images */
gasnet_image_t gasnete_coll_my_offset;	/* count of images before my first image */
#if !GASNET_SEQ
  gasnet_node_t *gasnete_coll_image_to_node = NULL;
#endif
#if GASNET_PAR
  int gasnete_coll_multi_images;	/* count of local images > 1 */
  int gasnete_coll_multi_images_any;	/* count of any node's images > 1 */
#endif

#define GASNETE_COLL_1ST_IMAGE(LIST,NODE) \
	(((void * const *)(LIST))[gasnete_coll_all_offset[(NODE)]])
#define GASNETE_COLL_MY_1ST_IMAGE(LIST,FLAGS) \
	(((void * const *)(LIST))[((FLAGS) & GASNET_COLL_LOCAL) ? 0 : gasnete_coll_my_offset])

/*---------------------------------------------------------------------------------*/

int gasnete_coll_init_done = 0;

void gasnete_coll_validate(gasnet_team_handle_t team,
			   gasnet_image_t dstimage, const void *dst, size_t dstlen, int dstisv,
			   gasnet_image_t srcimage, const void *src, size_t srclen, int srcisv,
			   int flags GASNETE_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
  int i;

  if_pf (!gasnete_coll_init_done) {
    gasneti_fatalerror("Illegal call to GASNet collectives before gasnet_coll_init()\n");
  }

  if (dstimage == (gasnet_image_t)(-1)) {
    dstimage = td->my_image;
  }
  if (srcimage == (gasnet_image_t)(-1)) {
    srcimage = td->my_image;
  }

  /* Some flags should never come the user */
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  #if GASNET_PAR
    gasneti_assert(!(flags & GASNETE_COLL_THREAD_LOCAL));
  #endif

  /* XXX: temporary limitation: */
  gasneti_assert(team == GASNET_TEAM_ALL);


  #if GASNET_DEBUG
    #if GASNET_SEQ
      if ((flags & GASNET_COLL_LOCAL) && (dstisv || srcisv) && !gasneti_mynode) {
        static int once = 1;
        if_pf (once) {
          fprintf(stderr, "NOTICE: Use of GASNET_COLL_LOCAL is discouraged with multi-address collectives in a GASNET_SEQ build - use single-address collectives instead\n");
	  once = 0;
        }
      }
    #endif
    #if GASNET_PAR
      if ((flags & GASNET_COLL_SINGLE) && !(dstisv || srcisv) && gasnete_coll_multi_images_any) {
        gasneti_fatalerror("illegal use of GASNET_COLL_SINGLE with single-address collectives and multiple threads");
      }
    #endif

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

  gasneti_assert(((flags & GASNET_COLL_SINGLE)?1:0) ^ ((flags & GASNET_COLL_LOCAL)?1:0));

  /* Bounds check any local portion of dst/dstlist which user claims is in-segment */
  gasneti_assert(dstlen > 0);
  if ((dstimage == td->my_image) && (flags & GASNET_COLL_DST_IN_SEGMENT)) {
    if (!dstisv) {
      gasneti_boundscheck(gasneti_mynode, dst, dstlen);
    } else {
      void * const *p = &GASNETE_COLL_MY_1ST_IMAGE(dst, flags);
      size_t limit = gasnete_coll_my_images;
      for (i = 0; i < limit; ++i, ++p) {
	gasneti_boundscheck(gasneti_mynode, *p, dstlen);
      }
    }
  }

  /* Bounds check any local portion of src/srclist which user claims is in-segment */
  gasneti_assert(srclen > 0);
  if ((srcimage == td->my_image) && (flags & GASNET_COLL_SRC_IN_SEGMENT)) {
    if (!srcisv) {
      gasneti_boundscheck(gasneti_mynode, src, srclen);
    } else {
      void * const *p = &GASNETE_COLL_MY_1ST_IMAGE(src, flags);
      size_t limit = gasnete_coll_my_images;
      for (i = 0; i < limit; ++i, ++p) {
	gasneti_boundscheck(gasneti_mynode, *p, srclen);
      }
    }
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
    #define GASNETE_COLL_HANDLE_DONE(h) ((h)->done)
    #define GASNETE_COLL_HANDLE_NEXT(h) ((h)->next)
  #else
    #define GASNETE_COLL_HANDLE_DONE(h)	(*h)
    #define GASNETE_COLL_HANDLE_NEXT(h) (*((gasnet_coll_handle_t *)(h)))
  #endif 

  extern gasnet_coll_handle_t gasnete_coll_handle_create(GASNETE_THREAD_FARG_ALONE) {
    gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
    gasnet_coll_handle_t result;

    result = td->handle_freelist;
    if_pt (result) {
      td->handle_freelist = GASNETE_COLL_HANDLE_NEXT(result);
    } else {
      /* XXX: allocate in large chunks and scatter across cache lines */
      /* XXX: destroy freelist at exit */
      result = (gasnet_coll_handle_t)gasneti_malloc(sizeof(*result));
    }

    GASNETE_COLL_HANDLE_DONE(result) = 0;
    #if GASNET_PAR
      GASNETE_COLL_HANDLE_NEXT(result) = GASNET_COLL_INVALID_HANDLE;
    #endif
    return result;
  }

  extern void gasnete_coll_handle_signal(gasnet_coll_handle_t handle GASNETE_THREAD_FARG) {
    gasneti_assert(handle != GASNET_COLL_INVALID_HANDLE);
    gasneti_mutex_assertlocked(&gasnete_coll_active_lock);
    #if GASNET_PAR
      do {
	gasnet_coll_handle_t next = GASNETE_COLL_HANDLE_NEXT(handle);
        gasneti_sync_writes(); /* XXX? */
	GASNETE_COLL_HANDLE_DONE(handle) = 1;
	handle = next;
      } while (handle != GASNET_COLL_INVALID_HANDLE);
    #else
      GASNETE_COLL_HANDLE_DONE(handle) = 1;
    #endif
  }

  /* NOTE: caller is responsible for a gasneti_flush_reads() on success */
  extern int gasnete_coll_handle_done(gasnet_coll_handle_t handle GASNETE_THREAD_FARG) {
    int result = 0;
    gasneti_assert(handle != GASNET_COLL_INVALID_HANDLE);

    if_pf (GASNETE_COLL_HANDLE_DONE(handle)) {
      gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
      GASNETE_COLL_HANDLE_NEXT(handle) = td->handle_freelist;
      result = 1;
    }

    return result;
  }
#endif

#ifndef gasnete_coll_try_sync
  /* NOTE: caller is responsible for a gasneti_flush_reads() on success */
  extern int
  gasnete_coll_try_sync(gasnet_coll_handle_t handle GASNETE_THREAD_FARG) {
    gasneti_assert(handle != GASNET_COLL_INVALID_HANDLE); /* caller must check */

    gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);

    return gasnete_coll_handle_done(handle GASNETE_THREAD_PASS) ? GASNET_OK : GASNET_ERR_NOT_READY;
  }
#endif

#ifndef gasnete_coll_try_sync_some
  /* Note caller is responsible for a gasneti_flush_reads() on success */
  extern int
  gasnete_coll_try_sync_some(gasnet_coll_handle_t *phandle, size_t numhandles GASNETE_THREAD_FARG) {
    int empty = 1;
    int result = GASNET_ERR_NOT_READY;
    int i;

    gasneti_assert(phandle != NULL);

    gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);

    for (i = 0; i < numhandles; ++i, ++phandle) {
      if (*phandle != GASNET_COLL_INVALID_HANDLE) {
	empty = 0;
	if (gasnete_coll_handle_done(*phandle GASNETE_THREAD_PASS)) {
	  *phandle = GASNET_COLL_INVALID_HANDLE;
	  result = GASNET_OK;
	}
      }
    }

    return empty ? GASNET_OK : result;
  }
#endif

#ifndef gasnete_coll_try_sync_all
  /* NOTE: caller is responsible for a gasneti_flush_reads() on success */
  extern int
  gasnete_coll_try_sync_all(gasnet_coll_handle_t *phandle, size_t numhandles GASNETE_THREAD_FARG) {
    int result = GASNET_OK;
    int i;

    gasneti_assert(phandle != NULL);

    gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);

    for (i = 0; i < numhandles; ++i, ++phandle) {
      if (*phandle != GASNET_COLL_INVALID_HANDLE) {
	if (gasnete_coll_handle_done(*phandle GASNETE_THREAD_PASS)) {
	  *phandle = GASNET_COLL_INVALID_HANDLE;
	} else {
	  result = GASNET_ERR_NOT_READY;
	}
      }
    }

    return result;
  }
#endif

/*---------------------------------------------------------------------------------*/
/* Code to handle thread-specific list of handles to sync */

typedef struct {
    uintptr_t		addr;	/* least significant bit: 0 = handle, 1 = coll_handle */
    union {
	gasnet_handle_t		handle;
	gasnet_coll_handle_t	coll_handle;
    }		u;
} gasnete_coll_local_handle_t;

GASNETI_INLINE(gasnete_coll_local_handles)
gasnete_coll_local_handle_t *
gasnete_coll_local_handles(gasnete_coll_threaddata_t *td, int grow) {
    gasnete_coll_local_handle_t *result = (gasnete_coll_local_handle_t *)td->handles.array;

    if (grow) {
	int allocated = td->handles.allocated;
	if_pf (allocated == td->handles.used) {
	    allocated += 8;
	    td->handles.allocated = allocated;
	    result = gasneti_realloc(result, allocated*sizeof(gasnete_coll_local_handle_t));
	    td->handles.array = result;
	}
    }

    return result;
}

void gasnete_coll_save_handle(gasnet_handle_t *handle_p GASNETE_THREAD_FARG) {
    if (*handle_p != GASNET_INVALID_HANDLE) {
	gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
	gasnete_coll_local_handle_t *p = gasnete_coll_local_handles(td, 1);
	p[td->handles.used].addr = (uintptr_t)handle_p;
	p[td->handles.used].u.handle = *handle_p;
	td->handles.used += 1;
    }
}

void gasnete_coll_save_coll_handle(gasnet_coll_handle_t *handle_p GASNETE_THREAD_FARG) {
    if (*handle_p != GASNET_COLL_INVALID_HANDLE) {
	gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
	gasnete_coll_local_handle_t *p = gasnete_coll_local_handles(td, 1);
	p[td->handles.used].addr = 1 | (uintptr_t)handle_p;
	p[td->handles.used].u.coll_handle = *handle_p;
	td->handles.used += 1;
    }
}

void gasnete_coll_sync_saved_handles(GASNETE_THREAD_FARG_ALONE) {
    gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
    int used = td->handles.used;

    if (used) {
	gasnete_coll_local_handle_t *curr = gasnete_coll_local_handles(td, 0);
	gasnete_coll_local_handle_t *last = curr + used - 1;
	int i;

	for (i = 0; i < used; ++i) {
	    uintptr_t addr = curr->addr;
	    int synced = 0;
	    if (addr & 1) {
		/* Coll handle - take care not to re-enter coll_poll()!! */
		addr &= ~1;
		synced = gasnete_coll_handle_done(curr->u.coll_handle GASNETE_THREAD_PASS);
		if (synced) {
		    gasneti_sync_writes();
		    *((gasnet_coll_handle_t *)addr) = GASNET_COLL_INVALID_HANDLE;
		}
	    } else {
		synced = (gasnete_try_syncnb(curr->u.handle) == GASNET_OK);
		if (synced) {
		    gasneti_sync_writes();
		    *((gasnet_handle_t *)addr) = GASNET_INVALID_HANDLE;
		}
	    }
	    if (synced) {
		*curr = *(last--);
		--td->handles.used;
	    } else {
		++curr;
	    }
	}
    }
}

/*---------------------------------------------------------------------------------*/
/* Collective teams */

/* XXX: Teams are not yet fully designed
 *
 * Likely interface:
 *
 *  void gasnete_coll_team_ins(op)
 *	Add a team to the table
 *  void gasnete_coll_team_del(op)
 *	Remove a team from the table
 *  gasnete_coll_team_t gasnete_coll_team_find(team_id)
 *	Lookup a team by its 32-bit id, returning NULL if not found.
 *
 * Serialization done inside the implementation
 */

#ifndef GASNETE_COLL_TEAMS_OVERRIDE
    /* Called by by AM handlers to lookup the team by id */
    gasnete_coll_team_t gasnete_coll_team_lookup(uint32_t team_id) {
	/* XXX: no implementation of teams yet */
	if (team_id != 0) {
	    gasneti_fatalerror("Non-zero team id passed, but teams are not yet implemented.");
	}
	return GASNET_TEAM_ALL;
    }

    gasnet_node_t gasnete_coll_team_rank2node(gasnete_coll_team_t team, int rank) {
	gasneti_assert(team == NULL);
	return (gasnet_node_t)rank;
    }

    int gasnete_coll_team_node2rank(gasnete_coll_team_t team, gasnet_node_t node) {
	gasneti_assert(team == NULL);
	return (int)node;
    }

    uint32_t gasnete_coll_team_id(gasnete_coll_team_t team) {
	gasneti_assert(team == NULL);
	return 0;
    }
#endif

/*---------------------------------------------------------------------------------*/
/* Synchronization for threads in PAR builds */

/* Current state:
 * In a SEQ or PARSYNC build this code compiles away.
 *
 * In a PAR build we have the following properties:
 * + First arrival takes lock and holds it until operations is *queued*.
 *   This is the shortest we can get away with if the later arrivals are
 *   to reliably locate the queued op.
 * + Late arrivals acquire the same lock to lookup the operation.  If
 *   found they call either an op-specific or default arrival function.
 *   The default decrements 'threads.remaining' (unless IN_NOSYNC) and
 *   creates a thread-specific handle (unless OUT_NOSYNC), adding it
 *   to the handle list (linked off the op).
 * + Just before signalling the handle(s) for a completed op, the lock is
 *   obtained to atomically remove the op from the list used by "late"
 *   arrivals to find in-flight ops.  The ensures that late arrivals are
 *   either signalled or see GASNET_COLL_INVALID_HANDLE (when not on list).
 * + Currenly IN_ALLSYNC and IN_MYSYNC involve pthread-level "barrier" before
 *   operation can "enter the network".
 * XXX Some per-op hook might be able to improve IN_MYSYNC slighty by allowing
 *     PARTS of the data to begin moving before all threads have arrived, but
 *     can never eliminate the need for all arrivals before op is internally
 *     completed.
 * + OUT_*SYNC handles:
 *   - OUT_NOSYNC will return INVALID_HANDLE to all but first arrival.
 *   - OUT_{MY,ALL}SYNC returns unique handles to any thread arriving before
 *     actual completion, but returns INVALID_HANDLE to any arriving later
 *     (which is only possible with IN_NOSYNC at the moment).
 * XXX Some per-op hook could relax things for OUT_MYSYNC.  An example would
 *     be a rooted operation - on the root node the non-root threads could
 *     be synced as soon as the local data movement is done.
 *
 * XXX: For "M_Eager" versions of Bcast and Scatter, the OUT_MYSYNC could be
 *	implemented to provide per-thread completion indications.  However,
 *	this requires short-cutting the current mechanism for signalling
 *	handles (by never returning GASNETE_COLL_OP_COMPLETE from the poll
 *	function) and tracking (in op->data?) of the handles assigned to
 *	each thread.
 * XXX: For "gathM_Eager" the IN_MYSYNC case can be implemented w/o a
 *	pthread-level "input" barrier by having each thread perform its
 *	own put.
 */

/* XXX - a work in progress */
#if GASNET_PAR
    gasneti_mutex_t gasnete_coll_threads_mutex = GASNETI_MUTEX_INITIALIZER;
    uint32_t gasnete_coll_threads_sequence = 0;	/* independent of collective sequence space */ /* XXX: TEAMS */
    gasnete_coll_op_t *gasnete_coll_threads_head = NULL;
    gasnete_coll_op_t **gasnete_coll_threads_tail_p = &(gasnete_coll_threads_head);

    void gasnete_coll_threads_lock(int flags GASNETE_THREAD_FARG) {
      gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
      if_pt (!gasnete_coll_multi_images || (flags & GASNETE_COLL_SUBORDINATE)) {
	/* I am only thread (and thus trivally the first) and therefore don't need the lock */
	gasneti_assert(td->threads.hold_lock == 0);
      } else {
	gasneti_mutex_lock(&gasnete_coll_threads_mutex);
	td->threads.hold_lock = 1;
      }
    }

    void gasnete_coll_threads_unlock(GASNETE_THREAD_FARG_ALONE) {
      gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
      if_pf (td->threads.hold_lock) {
        gasneti_mutex_assertlocked(&gasnete_coll_threads_mutex);
        gasneti_mutex_unlock(&gasnete_coll_threads_mutex);
	td->threads.hold_lock = 0;
      } else {
        gasneti_mutex_assertunlocked(&gasnete_coll_threads_mutex);
      }
    }

    /* Each thread calls this upon arrival.  First arrival gets non-zero */
    int gasnete_coll_threads_first(GASNETE_THREAD_FARG_ALONE) {
      gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
      if_pt (td->threads.hold_lock == 0) {
	/* single-threaded */
	return 1;
      } else {
	/* multi-threaded */
        const uint32_t sequence = td->threads.sequence;

        ++td->threads.sequence;
        if (sequence == gasnete_coll_threads_sequence) {
	  ++gasnete_coll_threads_sequence;
	  return 1;
	} else {
	  return 0;
	}
      }
    }

    gasnete_coll_op_t *gasnete_coll_threads_get_op(GASNETE_THREAD_FARG_ALONE) {
      gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
      const uint32_t sequence = td->threads.sequence - 1;
      gasnete_coll_op_t *op;

      gasneti_mutex_assertlocked(&gasnete_coll_active_lock);
      op = gasnete_coll_active_first();
      while (op && ((op->flags & GASNETE_COLL_SUBORDINATE) || (op->threads.sequence != sequence))) {
	op = gasnete_coll_active_next(op);
      }
      return op;
    }

    gasnet_coll_handle_t gasnete_coll_threads_add_handle(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
      gasnet_coll_handle_t handle = GASNET_COLL_INVALID_HANDLE;

      gasneti_mutex_assertlocked(&gasnete_coll_active_lock);
      if (op->handle != GASNET_COLL_INVALID_HANDLE) {
        handle = gasnete_coll_handle_create(GASNETE_THREAD_PASS_ALONE);
        GASNETE_COLL_HANDLE_NEXT(handle) = op->handle;
        op->handle = handle;
      }

      return handle;
    }

    /* Default thread-arrival function */
    gasnet_coll_handle_t gasnete_coll_threads_get_handle(GASNETE_THREAD_FARG_ALONE) {
      gasnete_coll_op_t *op;
      gasnet_coll_handle_t result = GASNET_COLL_INVALID_HANDLE;

      gasneti_mutex_lock(&gasnete_coll_active_lock);
      op = gasnete_coll_threads_get_op(GASNETE_THREAD_PASS_ALONE);
      if_pt (op != NULL) {
        /* Deal with IN_*SYNC */
        if (op->flags & (GASNET_COLL_IN_ALLSYNC | GASNET_COLL_IN_MYSYNC)) {
	  /* signal thread barrier */
          gasneti_assert(op->data != NULL);
          gasneti_atomic_decrement(&GASNETE_COLL_GENERIC_DATA(op)->threads.remaining, 0);
        }

        /* Deal with OUT_*SYNC */
        if (op->flags & (GASNET_COLL_OUT_ALLSYNC | GASNET_COLL_OUT_MYSYNC)) {
	  result = gasnete_coll_threads_add_handle(op GASNETE_THREAD_PASS);
        }
      }
      gasneti_mutex_unlock(&gasnete_coll_active_lock);

      return result;
    }

    /* Thread-arrival function when collecting thread-local addrs */
    gasnet_coll_handle_t
    gasnete_coll_threads_get_handle_and_data(gasnete_coll_generic_data_t **data_p GASNETE_THREAD_FARG) {
      gasnete_coll_op_t *op;
      gasnet_coll_handle_t result;

      gasneti_mutex_lock(&gasnete_coll_active_lock);
      op = gasnete_coll_threads_get_op(GASNETE_THREAD_PASS_ALONE);
      gasneti_assert(op != NULL);
      if (op->flags & (GASNET_COLL_OUT_ALLSYNC | GASNET_COLL_OUT_MYSYNC)) {
        result = gasnete_coll_threads_add_handle(op GASNETE_THREAD_PASS);
      } else {
        result = GASNET_COLL_INVALID_HANDLE;
      }
      gasneti_mutex_unlock(&gasnete_coll_active_lock);

      *data_p = op->data;

      return result;
    }

    int gasnete_coll_threads_addrs_ready(void * volatile *list GASNETE_THREAD_FARG) {
      /* The idea is to scan a list of thread-local addresses to see that they have all
       * been set to non-NULL values.  This function tries to lessen the amount of ping-
       * ponging of cache lines by eliminating the potential "hot spot" at the beginning
       * of the list.  This only ends up making a difference if the list spans cachelines.
       *
       * Note that non-application threads are OK, and will have my_local_image == 0.
       */
      const gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
      int i;

      for (i = td->my_local_image; i < gasnete_coll_my_images; ++i) { /* >= self */
	if (list[i] == NULL) {
	  return 0;
	}
      }
      for (i = 0; i < td->my_local_image; ++i) { /* < self */
	if (list[i] == NULL) {
	  return 0;
	}
      }
      return 1;
    }

    int gasnete_coll_threads_ready1(gasnete_coll_op_t *op, void **list GASNETE_THREAD_FARG) {
      if (op->flags & GASNETE_COLL_THREAD_LOCAL) {
	return gasnete_coll_threads_addrs_ready(list GASNETE_THREAD_PASS);
      } else {
	return gasnete_coll_generic_all_threads(op->data);
      }
    }

    int gasnete_coll_threads_ready2(gasnete_coll_op_t *op, void **list1, void **list2 GASNETE_THREAD_FARG) {
      if (op->flags & GASNETE_COLL_THREAD_LOCAL) {
	return gasnete_coll_threads_addrs_ready(list1 GASNETE_THREAD_PASS) &&
	       gasnete_coll_threads_addrs_ready(list2 GASNETE_THREAD_PASS);
      } else {
	return gasnete_coll_generic_all_threads(op->data);
      }
    }
#else
    #define gasnete_coll_threads_ready1(op,list1)	1
    #define gasnete_coll_threads_ready2(op,list1,list2)	1
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

gasneti_mutex_t gasnete_coll_active_lock = GASNETI_MUTEX_INITIALIZER;

#ifndef GASNETE_COLL_LIST_OVERRIDE
    /* Default implementation of coll_ops active list:
     *
     * Iteration over the active list is based on a linked list (queue).
     * Iteration starts from the head and new ops are added at the tail.
     *
     * XXX: use list macros?
     */
    static gasnete_coll_op_t	*gasnete_coll_active_head;
    static gasnete_coll_op_t	**gasnete_coll_active_tail_p;

    /* Caller must obtain lock */
    gasnete_coll_op_t *gasnete_coll_active_first(void) {
      return gasnete_coll_active_head;
    }

    /* Caller must obtain lock */
    gasnete_coll_op_t *gasnete_coll_active_next(gasnete_coll_op_t *op) {
      return op->active_next;
    }

    /* No lock needed */
    void gasnete_coll_active_new(gasnete_coll_op_t *op) {
      op->active_next = NULL;
      op->active_prev_p = &(op->active_next);
    }

    /* Caller must obtain lock */
    void gasnete_coll_active_ins(gasnete_coll_op_t *op) {
      *(gasnete_coll_active_tail_p) = op;
      op->active_prev_p = gasnete_coll_active_tail_p;
      gasnete_coll_active_tail_p = &(op->active_next);
    }

    /* Caller must obtain lock */
    void gasnete_coll_active_del(gasnete_coll_op_t *op) {
      gasnete_coll_op_t *next = op->active_next;
      *(op->active_prev_p) = next;
      if (next) {
	next->active_prev_p = op->active_prev_p;
      } else {
	gasnete_coll_active_tail_p = op->active_prev_p;
      }
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

extern gasnete_coll_threaddata_t *gasnete_coll_new_threaddata(void) {
    gasnete_coll_threaddata_t *result = gasneti_calloc(1,sizeof(*result));
    return result;
}

/*---------------------------------------------------------------------------------*/
/* Aggregation/filtering */

/* interface:
 *   gasnet_coll_handle_t gasnete_coll_op_submit(op, handle, th)
 *	Place coll_op in active list or not, as desired/required.
 *   void gasnete_coll_op_complete(op, poll_result);
 *	Completion hook
 *
 */

#ifndef GASNETE_COLL_AGG_OVERRIDE
    /* Default implementation of aggregation/filtering */

    /* XXX: how will teams interact w/ aggregation? */

    static gasnete_coll_op_t *gasnete_coll_agg = NULL;

    gasnet_coll_handle_t
    gasnete_coll_op_submit(gasnete_coll_op_t *op, gasnet_coll_handle_t handle GASNETE_THREAD_FARG) {
      op->agg_head = NULL;
      op->handle = handle;

      if_pf (op->flags & GASNET_COLL_AGGREGATE) {
	gasnete_coll_op_t *head = gasnete_coll_agg;

	gasneti_assert(handle == GASNET_COLL_INVALID_HANDLE);	/* check for handle leak */

	if (head == NULL) {
	  /* Build a container to hold the aggregate.
	   * The team, sequence and flags don't matter.
	   */
	  head = gasnete_coll_agg = gasnete_coll_op_create(op->team, 0, 0 GASNETE_THREAD_PASS);
	  head->agg_next = head->agg_prev = head;
	}

	/* Aggregate members go in a circular list */
	op->agg_next = head;
	op->agg_prev = head->agg_prev;
	head->agg_prev->agg_next = op;
	head->agg_prev = op;

	/* We don't set the agg_head yet.
	 * If the aggregation list becomes empty now it is
	 * only temporary and should not signal 'done'.
	 */
      } else if_pf (gasnete_coll_agg) {
	gasnete_coll_op_t *tmp;

	/* End of aggregate, place final op in the list */
	tmp = gasnete_coll_agg;
	op->agg_next = tmp;
	op->agg_prev = tmp->agg_prev;
	tmp->agg_prev->agg_next = op;
	tmp->agg_prev = op;

	/* Set all of the agg_head fields so we can signal
	 * the container op when the list becomes empty.
	 */
	gasneti_assert(tmp == gasnete_coll_agg);
	tmp = tmp->agg_next;
	do {
	   tmp->agg_head = gasnete_coll_agg;
	   tmp = tmp->agg_next;
	} while (tmp != gasnete_coll_agg);

	/* Return the container in place of the ops */
	gasneti_assert(tmp == gasnete_coll_agg);
	gasnete_coll_agg = NULL;
	tmp->handle = op->handle;
	op->handle = GASNET_COLL_INVALID_HANDLE;
      } else {
	/* An isolated coll_op (the normal case) */
	op->agg_next = NULL;
      }

      /* All ops go onto the active list */
      gasneti_mutex_lock(&gasnete_coll_active_lock);
      gasnete_coll_active_ins(op);
      gasneti_mutex_unlock(&gasnete_coll_active_lock);

      return handle;
    }

    void gasnete_coll_op_complete(gasnete_coll_op_t *op, int poll_result GASNETE_THREAD_FARG) {
      if (poll_result & GASNETE_COLL_OP_COMPLETE) {
	if_pt (op->handle != GASNET_COLL_INVALID_HANDLE) {
	    /* Normal case, just signal the handle */
	    gasnete_coll_handle_signal(op->handle GASNETE_THREAD_PASS);
	    op->handle = GASNET_COLL_INVALID_HANDLE;
	    gasneti_assert(op->agg_head == NULL);
	} else if (op->agg_next) {
	  gasnete_coll_op_t *head;

	  /* Remove this member from the aggregate */
	  op->agg_next->agg_prev = op->agg_prev;
	  op->agg_prev->agg_next = op->agg_next;

	  /* If the container op exists and is now empty, mark it's handle as done. */
	  head = op->agg_head;
	  if (head && (head->agg_next == head)) {
	    gasnete_coll_handle_signal(head->handle GASNETE_THREAD_PASS);
	    head->handle = GASNET_COLL_INVALID_HANDLE;
	    gasnete_coll_op_destroy(head GASNETE_THREAD_PASS);
	  }
	}
      }

      if (poll_result & GASNETE_COLL_OP_INACTIVE) {
	/* delete from the active list and destoy */
	gasnete_coll_active_del(op);
	gasnete_coll_op_destroy(op GASNETE_THREAD_PASS);
      }
    }
#endif

/*---------------------------------------------------------------------------------*/
gasnete_coll_op_t *
gasnete_coll_op_create(gasnete_coll_team_t team, uint32_t sequence, int flags GASNETE_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  gasnete_coll_op_t *op;

  op = td->op_freelist;
  if_pt (op != NULL) {
    td->op_freelist = *((gasnete_coll_op_t **)op);
  } else {
    /* XXX: allocate in chunks and scatter across cache lines */
    /* XXX: destroy freelist at exit */
    op = (gasnete_coll_op_t *)gasneti_malloc(sizeof(gasnete_coll_op_t));
  }

  gasnete_coll_active_new(op);
  op->team     = team;
  op->sequence = sequence;
  op->flags    = flags;
  op->handle   = GASNET_COLL_INVALID_HANDLE;
  op->poll_fn  = (gasnete_coll_poll_fn)NULL;

  /* The aggregation and 'data' fields are setup elsewhere */

  return op;
}

void
gasnete_coll_op_destroy(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
  *((gasnete_coll_op_t **)op) =  td->op_freelist;
  td->op_freelist = op;
}

void gasnete_coll_poll(GASNETE_THREAD_FARG_ALONE) {
  static gasneti_mutex_t poll_lock = GASNETI_MUTEX_INITIALIZER;

  gasneti_AMPoll();

  /* First try to make progress on any handles this thread has initiated */
  gasnete_coll_sync_saved_handles(GASNETE_THREAD_PASS_ALONE);

  /* XXX: We'd also like to have multiple pollers walk the list polling
   * distinct entries.
   */
  if (gasneti_mutex_trylock(&poll_lock) == 0) {
    gasnete_coll_op_t *op;

    gasneti_mutex_lock(&gasnete_coll_active_lock);
    op = gasnete_coll_active_first();
    gasneti_mutex_unlock(&gasnete_coll_active_lock);

    while (op != NULL) {
      gasnete_coll_op_t *next;
      int poll_result = 0;

      /* Poll/kick the op */
      gasneti_assert(op->poll_fn != (gasnete_coll_poll_fn)NULL);
      poll_result = (*op->poll_fn)(op GASNETE_THREAD_PASS);

      /* Advance down the list, possibly deleting this current element */
      gasneti_mutex_lock(&gasnete_coll_active_lock);
      next = gasnete_coll_active_next(op);
      if (poll_result != 0) {
        gasnete_coll_op_complete(op, poll_result GASNETE_THREAD_PASS);
      }
      gasneti_mutex_unlock(&gasnete_coll_active_lock);

      /* Next... */
      op = next;
    }

    gasneti_mutex_unlock(&poll_lock);
  }
}

extern void gasnete_coll_init(const gasnet_image_t images[], gasnet_image_t my_image,
			      gasnet_coll_fn_entry_t fn_tbl[], size_t fn_count,
			      int init_flags GASNETE_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  static gasneti_cond_t init_cond = GASNETI_COND_INITIALIZER;
  static gasneti_mutex_t init_lock = GASNETI_MUTEX_INITIALIZER;
  static gasnet_image_t remain = 0;
  size_t image_size = gasneti_nodes * sizeof(gasnet_image_t);
  int first;
  int i;

  GASNETI_CHECKATTACH();

  /* Sanity checks - performed only for debug builds */
  #if GASNET_DEBUG
    if (gasnete_coll_init_done) {
      gasneti_fatalerror("Multiple calls to gasnet_coll_init()\n");
    }
    if (init_flags) {
      gasneti_fatalerror("Invalid call to gasnet_coll_init() with non-zero flags\n");
    }
    #if GASNET_SEQ
      gasneti_assert(images == NULL);
    #endif
  #endif

  if (images) {
    td->my_image = my_image;
    gasneti_mutex_lock(&init_lock);
    if (!remain) {
      /* First thread to arrive */
      remain = images[gasneti_mynode];
      first = 1;
    } else {
      first = 0;
    }
    gasneti_mutex_unlock(&init_lock);
  } else {
    td->my_image = gasneti_mynode;
    first = 1; /* only thread, so always first */
  }

  if (first) {
    gasnete_coll_active_init();
    gasnete_coll_p2p_init();

    gasnete_coll_all_images = gasneti_malloc(image_size);
    gasnete_coll_all_offset = gasneti_malloc(image_size);
    if (images != NULL) {
      memcpy(gasnete_coll_all_images, images, image_size);
    } else  {
      for (i = 0; i < gasneti_nodes; ++i) {
        gasnete_coll_all_images[i] = 1;
      }
    }
    gasnete_coll_total_images = 0;
    gasnete_coll_max_images = 0;
    for (i = 0; i < gasneti_nodes; ++i) {
      gasnete_coll_all_offset[i] = gasnete_coll_total_images;
      gasnete_coll_total_images += gasnete_coll_all_images[i];
      gasnete_coll_max_images = MAX(gasnete_coll_max_images,gasnete_coll_all_images[i]);
    }
    gasnete_coll_my_images = gasnete_coll_all_images[gasneti_mynode];
    gasnete_coll_my_offset = gasnete_coll_all_offset[gasneti_mynode];
    #if GASNET_PAR
      if (!images) {
        gasnete_coll_multi_images = 0;
        gasnete_coll_multi_images_any = 0;
      } else if (gasnete_coll_my_images != 1) {
        gasnete_coll_multi_images = 1;
        gasnete_coll_multi_images_any = 1;
      } else {
        gasnete_coll_multi_images = 0;
        gasnete_coll_multi_images_any = 0;
        for (i = 0; i < gasneti_nodes; ++i) {
          if (gasnete_coll_all_images[i] > 1) {
	    gasnete_coll_multi_images_any = 1;
	    break;
          }
        }
      }
    #endif

    #if !GASNET_SEQ
    {
      gasnet_image_t j;
      gasnete_coll_image_to_node = gasneti_malloc(gasnete_coll_total_images * sizeof(gasnet_node_t));
      for (j = 0, i = 0; j < gasnete_coll_total_images; ++j) {
	if (j >= (gasnete_coll_all_offset[i] + gasnete_coll_all_images[i])) {
	  i += 1;
	}
        gasnete_coll_image_to_node[j] = i;
      }
    }
    #endif

    gasnete_coll_fn_count = fn_count;
    if (fn_count != 0) {
      size_t tbl_size = sizeof(gasnet_coll_fn_entry_t) * fn_count;
      gasnete_coll_fn_tbl = gasneti_malloc(tbl_size);
      memcpy(gasnete_coll_fn_tbl, fn_tbl, tbl_size);
      #if GASNET_DEBUG
        for (i = 0; i < fn_count; ++i) {
	  if (fn_tbl[i].flags & ~(GASNET_COLL_AMSAFE | GASNET_COLL_NONCOMM)) {
	    gasneti_fatalerror("gasnet_coll_init: function table entry %i has unknown flag(s)", i);
          }
        }
      #endif
    }

    gasnet_barrier_notify((int)gasnete_coll_sequence,0);
    gasnet_barrier_wait((int)gasnete_coll_sequence,0);
  }
  if (images) {
    /* Simple barrier */
    gasneti_mutex_lock(&init_lock);
    remain -= 1;
    if (remain == 0) {
      gasneti_cond_broadcast(&init_cond);
    } else {
      do {
        gasneti_cond_wait(&init_cond, &init_lock);
      } while (remain);
    }
    gasneti_mutex_unlock(&init_lock);
  }
  #if GASNET_DEBUG
    /* Ensure agreement across threads */
    gasneti_assert(fn_count == gasnete_coll_fn_count);
    for (i = 0; i < fn_count; ++i) {
      gasneti_assert(fn_tbl[i].flags == gasnete_coll_fn_tbl[i].flags);
      gasneti_assert(fn_tbl[i].fnptr == gasnete_coll_fn_tbl[i].fnptr);
    }
  #endif
  if (images) {
    td->my_local_image = my_image - gasnete_coll_my_offset;
    gasneti_assert(td->my_local_image < gasnete_coll_my_images);
  } else {
    td->my_local_image = 0;
  }
  gasnete_coll_init_done = 1;
}

/*---------------------------------------------------------------------------------*/
/* Synchronization primitives */

#ifndef GASNETE_COLL_CONSENSUS_OVERRIDE
    /* gasnete_coll_issued_id counts barrier sequence numbers as they are allocated
     * to collective operations. */
    static uint32_t gasnete_coll_issued_id = 0;

    /* gasnete_coll_consensus_id holds the current barrier state and sequence.
     * The upper 31 bits of gasnete_coll_issued_id holds the lower 31 bits of
     * the barrier sequence number of the current barrier.  This imposes a
     * limit of around 1 billion simultaneous outstanding collective ops before
     * counter overflow could introduce ambiguity.  Otherwise, careful use of
     * unsigned arithmetic eliminates problems due to wrap.
     * The least significant bit of gasnete_coll_consensus_id is 0 if the next
     * operation is to be a notify, or a 1 if the next is a try.
     * Any caller may issue a try (when the phase indicates a try) and must
     * advance gasnete_coll_issued_id by 1 if the try returns success.
     * Only the matching caller may issue a notify and must unconditionally
     * advance gasnete_coll_issued_id by 1.
     * In a debug build the gasnete_coll_issued_id is also used as the barrier
     * name to help detect bugs, but anonymous barriers are used in non-debug
     * builds for speed.
     */
    static uint32_t gasnete_coll_consensus_id = 0;

    extern gasnete_coll_consensus_t gasnete_coll_consensus_create(void) {
      return gasnete_coll_issued_id++;
    }

    GASNETI_INLINE(gasnete_coll_consensus_do_try)
    int gasnete_coll_consensus_do_try(void) {
#if GASNET_DEBUG
      int rc = gasnet_barrier_try(gasnete_coll_consensus_id, 0);
      if_pt (rc == GASNET_OK) {
	/* A barrier is complete, advance */
	++gasnete_coll_consensus_id;
	return 1;
      } else if (rc == GASNET_ERR_BARRIER_MISMATCH) {
	gasneti_fatalerror("Named barrier mismatch detected in collectives");
      } else {
	gasneti_assert(rc == GASNET_ERR_NOT_READY);
      }
      return 0;
#else
      int rc = gasnet_barrier_try(0, GASNET_BARRIERFLAG_ANONYMOUS);
      if_pt (rc == GASNET_OK) {
	/* A barrier is complete, advance */
	++gasnete_coll_consensus_id;
	return 1;
      }
      return 0;
#endif
    }

    GASNETI_INLINE(gasnete_coll_consensus_do_notify)
    void gasnete_coll_consensus_do_notify(void) {
	  ++gasnete_coll_consensus_id;
#if GASNET_DEBUG
	  gasnet_barrier_notify(gasnete_coll_consensus_id, 0);
#else
	  gasnet_barrier_notify(0, GASNET_BARRIERFLAG_ANONYMOUS);
#endif
    }

    extern int gasnete_coll_consensus_try(gasnete_coll_consensus_t id) {
      uint32_t tmp = id << 1;	/* low bit is used for barrier phase (notify vs wait) */

      /* We can only notify when our own turn comes up.
       * Thus, the most progress we could make in one call
       * would be to sucessfully 'try' for our predecessor,
       * 'notify' our our barrier, and then 'try' our own.
       */
      switch (tmp - gasnete_coll_consensus_id) {
        case 1:
	  /* Try for our predecessor, hoping we can then notify */
	  if (!gasnete_coll_consensus_do_try()) {
	    gasneti_assert((tmp - gasnete_coll_consensus_id) == 1);
	    /* Sucessor is not yet done */
	    break;
	  }
	  gasneti_assert(tmp == gasnete_coll_consensus_id);
	  /* ready to advance, so fall through... */
        case 0:
	  /* Our own turn has come - notify and try */
	  gasnete_coll_consensus_do_notify();
	  gasneti_assert((gasnete_coll_consensus_id - tmp) == 1);
	  gasnete_coll_consensus_do_try();
	  gasneti_assert(((gasnete_coll_consensus_id - tmp) == 1) ||
			 ((gasnete_coll_consensus_id - tmp) == 2));
	  break;

        default:
	  /* not our turn, but we can 'try' if the phase is right */
	  if (gasnete_coll_consensus_id & 1) {
	    gasnete_coll_consensus_do_try();
	  }
      }

      /* Note that we need to be careful of wrapping, thus the (int32_t)(a-b) construct
       * must be used in place of simply (a-b).
       */
      return ((int32_t)(gasnete_coll_consensus_id - tmp) > 1) ? GASNET_OK
							      : GASNET_ERR_NOT_READY;
    }
#endif

#ifndef GASNETE_COLL_P2P_OVERRIDE
    #ifndef GASNETE_COLL_P2P_TABLE_SIZE
      #define GASNETE_COLL_P2P_TABLE_SIZE 16
    #endif
    #if 0
      /* This is one possible implementation when we have teams */
      #define GASNETE_COLL_P2P_TABLE_SLOT(T,S) \
	 (((uint32_t)(uintptr_t)(T) ^ (uint32_t)(S)) % GASNETE_COLL_P2P_TABLE_SIZE)
    #else
      /* Use this mapping until teams are implemented */
      #define GASNETE_COLL_P2P_TABLE_SLOT(T,S) \
	 (gasneti_assert(gasnete_coll_team_lookup(T)==NULL), ((uint32_t)(S) % GASNETE_COLL_P2P_TABLE_SIZE))
    #endif

    /* XXX free list could/should be per team: */
    static gasnete_coll_p2p_t *gasnete_coll_p2p_freelist = NULL;

    static gasnete_coll_p2p_t gasnete_coll_p2p_table[GASNETE_COLL_P2P_TABLE_SIZE];
    static gasnet_hsl_t gasnete_coll_p2p_table_lock = GASNET_HSL_INITIALIZER;

    void gasnete_coll_p2p_init() {
      int i;

      for (i = 0; i < GASNETE_COLL_P2P_TABLE_SIZE; ++i) {
	gasnete_coll_p2p_t *tmp = &(gasnete_coll_p2p_table[i]);
	tmp->p2p_next = tmp->p2p_prev = tmp;
      }
    }

    void gasnete_coll_p2p_fini() {
      int i;

      for (i = 0; i < GASNETE_COLL_P2P_TABLE_SIZE; ++i) {
	gasnete_coll_p2p_t *tmp = &(gasnete_coll_p2p_table[i]);
	/* Check that table is actually empty */
	gasneti_assert(tmp->p2p_next == tmp);
	gasneti_assert(tmp->p2p_prev == tmp);
      }
    }

    gasnete_coll_p2p_t *gasnete_coll_p2p_get(uint32_t team_id, uint32_t sequence) {
      unsigned int slot_nr = GASNETE_COLL_P2P_TABLE_SLOT(team_id, sequence);
      gasnete_coll_p2p_t *head = &(gasnete_coll_p2p_table[slot_nr]);
      gasnete_coll_p2p_t *p2p;

      gasneti_assert(gasnete_coll_team_lookup(team_id) == GASNET_TEAM_ALL);

      gasnet_hsl_lock(&gasnete_coll_p2p_table_lock);

      /* Search table */
      p2p = head->p2p_next;
      while ((p2p != head) && ((p2p->team_id != team_id) || (p2p->sequence != sequence))) {
	p2p = p2p->p2p_next;
      }

      /* If not found, create it with all zeros */
      if_pf (p2p == head) {
	size_t buffersz = MAX(GASNETE_COLL_P2P_EAGER_MIN,
			      gasnete_coll_total_images * GASNETE_COLL_P2P_EAGER_SCALE);
	size_t statesz = GASNETI_ALIGNUP(gasnete_coll_total_images * sizeof(uint32_t), 8);

	p2p = gasnete_coll_p2p_freelist;	/* XXX: per-team */

	if_pf (p2p == NULL) {
	  /* Round to 8-byte alignment of entry array */
	  size_t alloc_size = GASNETI_ALIGNUP(sizeof(gasnete_coll_p2p_t) + statesz,8) + buffersz;
	  uintptr_t p = (uintptr_t)gasneti_malloc(alloc_size);

	  p2p = (gasnete_coll_p2p_t *)p;
	  p += sizeof(gasnete_coll_p2p_t);

	  p2p->state = (uint32_t *)p;
	  p += statesz;

	  p = GASNETI_ALIGNUP(p,8);
	  p2p->data = (uint8_t *)p;

	  p2p->p2p_next = NULL;
	}

	memset((void *)p2p->state, 0, statesz);
	memset(p2p->data, 0, buffersz);

	p2p->team_id = team_id;
	p2p->sequence = sequence;
	gasnet_hsl_init(&p2p->lock);

	gasnete_coll_p2p_freelist = p2p->p2p_next;

	/* XXX: searches out-number insertions.  So, we should do the work here to
	 * keep the list sorted, and take advantage in the search, above. */
	p2p->p2p_prev = head;
	p2p->p2p_next = head->p2p_next;
	head->p2p_next->p2p_prev = p2p;
	head->p2p_next = p2p;
	#ifdef GASNETE_P2P_EXTRA_INIT
	  GASNETE_P2P_EXTRA_INIT(p2p)
	#endif
      }

      gasnet_hsl_unlock(&gasnete_coll_p2p_table_lock);

      gasneti_assert(p2p != NULL);
      gasneti_assert(p2p->state != NULL);
      gasneti_assert(p2p->data != NULL);

      return p2p;
    }

    void gasnete_coll_p2p_free(gasnete_coll_p2p_t *p2p) {
      gasneti_assert(p2p != NULL);

      gasnet_hsl_lock(&gasnete_coll_p2p_table_lock);

      p2p->p2p_prev->p2p_next = p2p->p2p_next;
      p2p->p2p_next->p2p_prev = p2p->p2p_prev;
      #ifdef GASNETE_P2P_EXTRA_FREE
        GASNETE_P2P_EXTRA_FREE(p2p)
      #endif

      p2p->p2p_next = gasnete_coll_p2p_freelist;	/* XXX: per-team */
      gasnete_coll_p2p_freelist = p2p;

      gasnet_hsl_unlock(&gasnete_coll_p2p_table_lock);
    }

    /* Delivers a long payload and updates 1 or more states
       count: number of states to update
       offset: index of first state to update
       state: value to assign to states [offset, offset+count)
     */
    extern void gasnete_coll_p2p_long_reqh(gasnet_token_t token, void *buf, size_t nbytes,
					   gasnet_handlerarg_t team_id,
					   gasnet_handlerarg_t sequence,
					   gasnet_handlerarg_t count,
					   gasnet_handlerarg_t offset,
					   gasnet_handlerarg_t state) {
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
    extern void gasnete_coll_p2p_med_reqh(gasnet_token_t token, void *buf, size_t nbytes,
					  gasnet_handlerarg_t team_id,
					  gasnet_handlerarg_t sequence,
					  gasnet_handlerarg_t count,
					  gasnet_handlerarg_t offset,
					  gasnet_handlerarg_t state,
					  gasnet_handlerarg_t size) {
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

    /* No payload to deliver, just updates 1 or more states
       count: number of states to update
       offset: index of first state to update
       state: value to assign to states [offset, offset+count)
     */
    extern void gasnete_coll_p2p_short_reqh(gasnet_token_t token,
						  gasnet_handlerarg_t team_id,
						  gasnet_handlerarg_t sequence,
						  gasnet_handlerarg_t count,
						  gasnet_handlerarg_t offset,
						  gasnet_handlerarg_t state) {
      gasnete_coll_p2p_t *p2p = gasnete_coll_p2p_get(team_id, sequence);
      int i;

      for (i = 0; i < count; ++i, ++offset) {
        p2p->state[offset] = state;
      }
    }

    /* Memcopy payload and then decrement atomic counter if requested */
    GASNETI_INLINE(gasnete_coll_p2p_memcpy_reqh_inner)
    void gasnete_coll_p2p_memcpy_reqh_inner(gasnet_token_t token, void *buf, size_t nbytes,
						   void *dest,
						   gasnet_handlerarg_t team_id,
						   gasnet_handlerarg_t sequence,
						   gasnet_handlerarg_t decrement) {
      gasnete_coll_p2p_t *p2p = gasnete_coll_p2p_get(team_id, sequence);

      GASNETE_FAST_UNALIGNED_MEMCPY(dest, buf, nbytes);
      if (decrement) {
        gasneti_sync_writes();
	gasnet_hsl_lock(&p2p->lock);
        --(p2p->state[0]);
	gasnet_hsl_unlock(&p2p->lock);
      }
    }
    MEDIUM_HANDLER(gasnete_coll_p2p_memcpy_reqh,4,5,
                  (token,addr,nbytes, UNPACK(a0),      a1, a2, a3),
                  (token,addr,nbytes, UNPACK2(a0, a1), a2, a3, a4));

    /* Put up to gasnet_AMMaxLongRequest() bytes, signalling the recipient */
    /* Returns as soon as local buffer is reusable */
    void gasnete_coll_p2p_signalling_put(gasnete_coll_op_t *op, gasnet_node_t dstnode, void *dst,
					 void *src, size_t nbytes, uint32_t offset, uint32_t state) {
      uint32_t team_id = gasnete_coll_team_id(op->team);

      gasneti_assert(nbytes <= gasnet_AMMaxLongRequest());

      GASNETI_SAFE(
	LONG_REQ(5,5,(dstnode, gasneti_handleridx(gasnete_coll_p2p_long_reqh),
		      src, nbytes, dst, team_id, op->sequence, 1, offset, state)));
    }

    /* Put up to gasnet_AMMaxLongRequest() bytes, signalling the recipient */
    /* Returns immediately even if the local buffer is not yet reusable */
    void gasnete_coll_p2p_signalling_putAsync(gasnete_coll_op_t *op, gasnet_node_t dstnode, void *dst,
					      void *src, size_t nbytes, uint32_t offset, uint32_t state) {
      uint32_t team_id = gasnete_coll_team_id(op->team);

      gasneti_assert(nbytes <= gasnet_AMMaxLongRequest());

      GASNETI_SAFE(
	LONGASYNC_REQ(5,5,(dstnode, gasneti_handleridx(gasnete_coll_p2p_long_reqh),
			   src, nbytes, dst, team_id, op->sequence, 1, offset, state)));
    }

    /* Send data to be buffered by the recipient */
    void gasnete_coll_p2p_eager_putM(gasnete_coll_op_t *op, gasnet_node_t dstnode,
				     void *src, uint32_t count, size_t size,
				     uint32_t offset, uint32_t state) {
      uint32_t team_id = gasnete_coll_team_id(op->team);
      size_t limit;

      limit = gasnet_AMMaxMedium() / size;
      if_pf (count > limit) {
	size_t nbytes = limit * size;

	do {
          GASNETI_SAFE(
	    MEDIUM_REQ(6,6,(dstnode, gasneti_handleridx(gasnete_coll_p2p_med_reqh),
			    src, nbytes, team_id, op->sequence, limit, offset, state, size)));
	  offset += limit;
	  src = (void *)((uintptr_t)src + nbytes);
	  count -= limit;
	} while (count > limit);
      }

      GASNETI_SAFE(
	MEDIUM_REQ(6,6,(dstnode, gasneti_handleridx(gasnete_coll_p2p_med_reqh),
			src, count * size, team_id, op->sequence, count, offset, state, size)));
    }

    /* Update one or more states w/o delivering any data */
    void gasnete_coll_p2p_change_states(gasnete_coll_op_t *op, gasnet_node_t dstnode,
				        uint32_t count, uint32_t offset, uint32_t state) {
      uint32_t team_id = gasnete_coll_team_id(op->team);

      GASNETI_SAFE(
        SHORT_REQ(5,5,(dstnode, gasneti_handleridx(gasnete_coll_p2p_short_reqh),
                       team_id, op->sequence, count, offset, state)));
    }

    /* Memcpy up to gasnet_AMMaxMedium() bytes, signalling the recipient */
    /* Returns as soon as local buffer is reusable */
    void gasnete_coll_p2p_memcpy(gasnete_coll_op_t *op, gasnet_node_t dstnode, void *dst,
				 void *src, size_t nbytes) {
      uint32_t team_id = gasnete_coll_team_id(op->team);

      gasneti_assert(nbytes <= gasnet_AMMaxMedium());

      GASNETI_SAFE(
	MEDIUM_REQ(4,5,(dstnode, gasneti_handleridx(gasnete_coll_p2p_memcpy_reqh),
			src, nbytes, PACK(dst), team_id, op->sequence, 1)));
    }

    struct gasnete_coll_p2p_send_struct { void *addr; size_t sent; };

    /* Indicate ready for a gasnete_coll_p2p_memcpy, placing request in slots "offset+" */
    /* XXX: we send addr+"0", when only the addr is needed (want "custom" AM, not eager_put) . */
    void gasnete_coll_p2p_send_rtrM(gasnete_coll_op_t *op, gasnete_coll_p2p_t *p2p,
				    uint32_t offset, void * const *dstlist,
				    gasnet_node_t node, size_t nbytes, uint32_t count) {
      struct gasnete_coll_p2p_send_struct *tmp =
		gasneti_malloc(count * sizeof(struct gasnete_coll_p2p_send_struct));
      int i;
      for (i = 0; i < count; ++i) {
        tmp[i].addr = dstlist[i];
        tmp[i].sent = 0;
      }
      gasnet_hsl_lock(&p2p->lock);
      /* Record the number of Mediums we know we'll receive. */
      p2p->state[0] += count * ((nbytes + gasnet_AMMaxMedium() - 1) / gasnet_AMMaxMedium());
      gasnet_hsl_unlock(&p2p->lock);
      gasnete_coll_p2p_eager_putM(op, node, tmp, count, sizeof(*tmp), offset, 1);
    }

    /* Indicate ready for a gasnete_coll_p2p_memcpy, placing request in slot "offset" */
    /* XXX: we send addr+"0", when only the addr is needed. */
    void gasnete_coll_p2p_send_rtr(gasnete_coll_op_t *op, gasnete_coll_p2p_t *p2p,
				   uint32_t offset, void *dst,
				   gasnet_node_t node, size_t nbytes) {
      gasnete_coll_p2p_send_rtrM(op, p2p, offset, &dst, node, nbytes, 1);
    }

    /* Check completion of a gasnete_coll_p2p_memcpy (on rcvr) */
    int gasnete_coll_p2p_send_done(gasnete_coll_p2p_t *p2p) {
      int result;
      gasnet_hsl_lock(&p2p->lock);
      result = !p2p->state[0];
      gasnet_hsl_unlock(&p2p->lock);
      return result;
    }

    /* Respond to a gasnete_coll_p2p_send_rtr */
    int gasnete_coll_p2p_send_data(gasnete_coll_op_t *op, gasnete_coll_p2p_t *p2p,
				   gasnet_node_t node, uint32_t offset,
				   const void *src, size_t nbytes) {
      struct gasnete_coll_p2p_send_struct *status = (struct gasnete_coll_p2p_send_struct *)p2p->data;
      if (p2p->state[offset] == 1) {
	size_t sent = status[offset].sent;
	size_t count = nbytes - sent;
	if_pt (count) {
	  void *tmp = (void *)((uintptr_t)src + sent);
	  void *addr = status[offset].addr;
	  count = MIN(count, gasnet_AMMaxMedium());
	  gasnete_coll_p2p_memcpy(op, node, addr, tmp, count);
	  status[offset].addr = (void *)((uintptr_t)addr + count);
	  status[offset].sent += count;
	} else {
	  p2p->state[offset] = 2;
	}
      }
      return (p2p->state[offset] == 2);
    }
#endif

/*---------------------------------------------------------------------------------*/
/* functions for generic ops */

#if PLATFORM_COMPILER_COMPAQ_C /* bug525 workaround - prevent inliner resource exhaustion with -inline all */
  #pragma noinline (gasnete_coll_generic_alloc,gasnete_coll_generic_free,gasnete_coll_op_generic_init)
#endif

extern gasnete_coll_generic_data_t *gasnete_coll_generic_alloc(GASNETE_THREAD_FARG_ALONE) {
    gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
    gasnete_coll_generic_data_t *result;

    gasneti_assert(td != NULL);

    result = td->generic_data_freelist;
    if_pt (result != NULL) {
	td->generic_data_freelist = *((gasnete_coll_generic_data_t **)result);
    } else {
	/* XXX: allocate in chunks and scatter across cache lines */
	/* XXX: destroy freelist at exit */
	result = (gasnete_coll_generic_data_t *)gasneti_malloc(sizeof(gasnete_coll_generic_data_t));
    }

    memset(result, 0, sizeof(*result));

    return result;
}

extern void gasnete_coll_generic_free(gasnete_coll_generic_data_t *data GASNETE_THREAD_FARG) {
    gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;

    gasneti_assert(data != NULL);

    if (data->options & GASNETE_COLL_GENERIC_OPT_P2P) {
      gasnete_coll_p2p_free(data->p2p);
    }

    #if GASNET_PAR
    if (data->threads.data) {
      gasneti_free(data->threads.data);
    }
    #endif

    *((gasnete_coll_generic_data_t **)data) =  td->generic_data_freelist;
    td->generic_data_freelist = data;
}

/* Generic routine to create an op and enter it in the active list, etc..
 * Caller provides 'data' and 'poll_fn' specific to the operation.
 * Handle is allocated automatically if flags don't indicate aggregation.
 *
 * 'sequence' can have two meanings:
 *  w/ GASNETE_COLL_SUBORDINATE it is the pre-allocated sequence number to assign
 *  w/o GASNETE_COLL_SUBORDINATE is is the number of slots to pre-allocate
 *
 * Just returns the handle.
 */
extern gasnet_coll_handle_t
gasnete_coll_op_generic_init(gasnete_coll_team_t team, int flags,
			     gasnete_coll_generic_data_t *data, gasnete_coll_poll_fn poll_fn,
			     uint32_t sequence GASNETE_THREAD_FARG) {
      gasnet_coll_handle_t handle = GASNET_COLL_INVALID_HANDLE;
      gasnete_coll_op_t *op;

      gasneti_assert(team == GASNET_TEAM_ALL);
      gasneti_assert(data != NULL);

      GASNETE_COLL_SET_OWNER(data);

      if_pf (flags & GASNETE_COLL_SUBORDINATE) {
        /* Subordinates can't allocate sequence numbers or barriers, due to non-collective calling */
	gasneti_assert(!(data->options & (GASNETE_COLL_GENERIC_OPT_INSYNC |
					  GASNETE_COLL_GENERIC_OPT_OUTSYNC)));
	/* Subordinates can't AGGREGATE (but maybe they should?) */
        gasneti_assert(!(flags & GASNET_COLL_AGGREGATE));
      } else {
	/* XXX: need team scope for sequence numbers */
	uint32_t tmp = gasnete_coll_sequence;
	gasnete_coll_sequence += (1 + sequence);
        sequence = tmp;

	/* Conditionally allocate barriers */
	/* XXX: this is where we could do some aggregation of syncs */
	if (data->options & GASNETE_COLL_GENERIC_OPT_INSYNC) {
	  data->in_barrier = gasnete_coll_consensus_create();
	}
	if (data->options & GASNETE_COLL_GENERIC_OPT_OUTSYNC) {
	  data->out_barrier = gasnete_coll_consensus_create();
	}
      }

      /* Conditionally allocate data for point-to-point syncs */
      if (data->options & GASNETE_COLL_GENERIC_OPT_P2P) {
	data->p2p = gasnete_coll_p2p_get(gasnete_coll_team_id(team), sequence);
      }

      /* Conditionally allocate a handle */
      if_pt (!(flags & GASNET_COLL_AGGREGATE)) {
	handle = gasnete_coll_handle_create(GASNETE_THREAD_PASS_ALONE);
      }

      /* Create the op */
      op = gasnete_coll_op_create(team, sequence, flags GASNETE_THREAD_PASS);
      op->data = data;
      op->poll_fn = poll_fn;

      #if GASNET_PAR
      if (gasnete_coll_multi_images && !(flags & GASNETE_COLL_SUBORDINATE)) {
        op->threads.sequence = gasnete_coll_threads_sequence - 1;
        gasneti_atomic_set(&data->threads.remaining, (flags & GASNET_COLL_IN_NOSYNC) ? 0 : (gasnete_coll_my_images - 1), 0);
      } else {
        gasneti_atomic_set(&data->threads.remaining, 0, 0);
      }
      #endif

      /* Submit the op via aggregation filter */
      handle = gasnete_coll_op_submit(op, handle GASNETE_THREAD_PASS);

      return handle;
}

/* NOTE: caller is responsible for a gasneti_sync_reads() if they read any transferred data.  */
extern int gasnete_coll_generic_coll_sync(gasnet_coll_handle_t *p, size_t count GASNETE_THREAD_FARG) {
  int result = 1;
  int i;

  for (i = 0; i < count; ++i) {
    if (p[i] != GASNET_COLL_INVALID_HANDLE) {
      result = 0;
      break;
    }
  }

  return result;
}

/*---------------------------------------------------------------------------------*/
/* functions for generic tree-based ops */

uint32_t gasnete_coll_pipe_seg_size = 1024;

/* XXX: should per-team */
static gasnete_coll_tree_geom_t *gasnete_coll_tree_geom_init(gasnete_coll_tree_kind_t kind, gasnet_node_t root) {
  #define START(lev) ((1 << (lev))-1)
  #define ACT2REL(actrank, root) ( (actrank >= root) ? actrank - root : actrank - root + gasneti_nodes )
  #define REL2ACT(relrank, root) (((relrank < (gasneti_nodes-root)) ? relrank + root : relrank + root - gasneti_nodes))
  gasnete_coll_tree_geom_t *geom = NULL;
  int relrank = ACT2REL(gasneti_mynode, root);

  geom = gasneti_malloc(sizeof(gasnete_coll_tree_geom_t));
  geom->kind = kind;
  geom->root = root;
  gasneti_weakatomic_set(&(geom->ref_count), 1, 0);

  geom->parent = (gasnet_node_t)(-1);
  geom->child_id = -1;

  switch(kind) {
    case GASNETE_COLL_TREE_KIND_CHAIN:
      if (relrank!=(gasneti_nodes-1)) {
	geom->child_count = 1;
	geom->child_list = (gasnet_node_t *)gasneti_malloc(sizeof(gasnet_node_t));
	geom->child_list[0] = REL2ACT(relrank+1,root);
      } else {
	geom->child_count = 0;
	geom->child_list = NULL;
      }
      if (relrank==0) {
	geom->parent = (gasnet_node_t)-1;
      } else {
	geom->parent = REL2ACT(relrank-1,root);
      }
      geom->child_id = 0; /*only one child by def*/
      break;

    case GASNETE_COLL_TREE_KIND_BINARY:
    {
      int level;
      int tchild0;
      int tchild1;

      level = 0;
      while(1) { /* has to terminate because of the semantics of the loop  */
	if (relrank >= START(level) && relrank < START(level+1)) {
	  break;
	} else {
	  level++;
	}
      }
      if (relrank!=0) {
	/* we expect to recieve from some one */
	int relparent = (relrank-START(level))/2 + START(level-1);
	geom->parent = REL2ACT(relparent,root);
	geom->child_id = (relrank+1)%2; /*odd nodes are left child even are right*/
      } else {
	/* geom->parent = -1;  by default */
	/* geom->child_id = -1;  by default */
      }
      /* so now the level of the current node is set. */
      /* now we figure out where to expect the message from */
      /* special case is the root */
      /* now we need to set the 2 destinations */
      tchild0= (relrank - START(level))*2 + START(level+1);
      tchild1= tchild0+1;
      if (tchild0<gasneti_nodes && tchild1<gasneti_nodes) {
	geom->child_count = 2;
	geom->child_list = (gasnet_node_t *) gasneti_malloc(sizeof(gasnet_node_t)*2);
	geom->child_list[0] = REL2ACT(tchild0,root);
	geom->child_list[1] = REL2ACT(tchild1,root);
      } else if (tchild0<gasneti_nodes && tchild1>=gasneti_nodes) {
	geom->child_list = (gasnet_node_t *) gasneti_malloc(sizeof(gasnet_node_t));
	geom->child_count = 1;
	geom->child_list[0] = REL2ACT(tchild0,root);
      } else if (tchild0>=gasneti_nodes && tchild1<gasneti_nodes) {
	geom->child_list = (gasnet_node_t *) gasneti_malloc(sizeof(gasnet_node_t));
	geom->child_count = 1;
	geom->child_list[0] = REL2ACT(tchild1,root);
      } else {
	geom->child_count = 0;
	geom->child_list = NULL;
      }
      break;
    }

    case GASNETE_COLL_TREE_KIND_BINOMIAL:
    {
      gasnet_node_t child, src;
      gasnet_node_t temp_dest_list[8*sizeof(gasnet_node_t)];
      int mask = 1;
      gasnet_node_t num_child=0;

      mask = 0x1;
      while (mask < gasneti_nodes) {
	if (relrank & mask) {
	  src = (gasneti_mynode >= mask) ? (gasneti_mynode - mask)
					 : (gasneti_mynode + (gasneti_nodes - mask));
	  geom->parent = src;
	  break;
	}
	mask <<= 1;
      }

      mask >>= 1;
      while (mask > 0) {
	if (relrank + mask < gasneti_nodes) {
	  child = gasneti_mynode + mask;
	  if (child >= gasneti_nodes) child -= gasneti_nodes;
	  temp_dest_list[num_child]=child;
	  num_child++;
	}
	mask >>= 1;
      }
      if (num_child > 0) {
	geom->child_list = (gasnet_node_t *)gasneti_malloc(sizeof(gasnet_node_t )*num_child);
	for (child = 0; child<(num_child); child++) {
	  geom->child_list[child] = temp_dest_list[child];
	}
      } else {
	geom->child_list = NULL;
      }

      if (relrank != 0) {
	int id, i, j;
	i = relrank - ACT2REL(src, root);
	/* compute floor(log_base_2(i)): */
	for (j=1, id=0; (i-j) >= j; ++id, j = j<<1) {/*nothing*/}
	geom->child_id = id;
      } else {
	/* geom->child_id = -1; by default */
      }
      geom->child_count = num_child;
    }
    break;

    case GASNETE_COLL_TREE_KIND_SEQUENTIAL:
    {
      int i=0;
      if (gasneti_mynode ==  root) {
	geom->parent = (gasnet_node_t)-1;
	geom->child_count = gasneti_nodes-1;
	if (gasneti_nodes > 1) {
	  geom->child_list = (gasnet_node_t *)gasneti_malloc(sizeof(gasnet_node_t)*(gasneti_nodes-1));
	}
	for (i=0; i<gasneti_nodes-1; i++) {
	  geom->child_list[i] = REL2ACT(i+1,root);
	}
      } else {
	geom->parent = root;
	geom->child_count = 0;
	geom->child_list = NULL;
      }
      geom->child_id = relrank-1;
    }
    break;

#if 0
    case GASNETE_COLL_TREE_KIND_CHAIN_SMP:
    {
      int i;
      gasnet_node_t num_child = 0;

      if (relrank % procs_per_node == 0) {
	int start;
	if (relrank!=0) {
	  geom->parent = relrank - procs_per_node;
	} else {
	  geom->parent = (gasnet_node_t)-1;
	}
	if (relrank + procs_per_node < gasneti_nodes) {
	  num_child++;
	}
	for (i=1; i<procs_per_node; i++) {
	  if (relrank+i < gasneti_nodes) {
	    num_child++;
	  }
	}
	if (num_child > 0) {
	  geom->child_list = (gasnet_node_t *)gasneti_malloc(sizeof(gasnet_node_t)*num_child);
	  if (relrank+procs_per_node < gasneti_nodes) {
	    geom->child_list[0] = REL2ACT(relrank+procs_per_node, root);
	    start = 1;
	  } else {
	    start = 0;
	  }
	  for (i=start; i<num_child; i++) {
	    if (start == 0) {
	      geom->child_list[i] = REL2ACT(relrank+i+1, root);
	    } else {
	      geom->child_list[i] = REL2ACT(relrank+i, root);
	    }
	  }
	}
      } else {
	geom->parent = (relrank / procs_per_node)*procs_per_node;
	num_child = 0;
	geom->child_list = NULL;
      }
      geom->child_count = num_child;
      break;
    }

    case GASNETE_COLL_TREE_KIND_BINARY_SMP:
    {
      gasnet_node_t num_child = 0;
      if (relrank%procs_per_node==0) {
	int level;
	int tchild0;
	int tchild1;
	int smprelrank = relrank/procs_per_node;
	level = 0;
	while(1) { /* has to terminate because of the semantics of the loop */
 	  if (smprelrank >= START(level) && smprelrank < START(level+1)) {
 	    break;
 	  } else {
 	    level++;
	  }
	}
	if (relrank!=0) {
 	  /* we expect to recieve from some one */
 	  geom->parent = ((smprelrank-START(level))/2 + START(level-1))*procs_per_node;
 	  geom->parent = REL2ACT(geom->parent,root);
	} else {
 	  geom->parent = (gasnet_node_t)-1;
	}
	/* so now the level of the current node is set. */
	/* now we figure out where to expect the message from */
	/* special case is the root */
	/* now we need to set the 2 destinations */

	tchild0= (smprelrank - START(level))*2 + START(level+1);
	tchild1= tchild0+1;
	tchild0 *= procs_per_node;
	tchild1 *= procs_per_node;
	if (tchild0<gasneti_nodes && tchild1<gasneti_nodes) {
 	  num_child = 2;
 	  for (i=1; i<procs_per_node; i++) {
 	    if (relrank + i < gasneti_nodes) {
 	      num_child++;
 	    }
 	  }
 	  geom->child_list = (gasnet_node_t *) gasneti_malloc(sizeof(gasnet_node_t)*num_child);

 	  geom->child_list[0] = REL2ACT(tchild0,root);
 	  geom->child_list[1] = REL2ACT(tchild1,root);

 	  for (i=2; i<num_child; i++) {
 	    geom->child_list[i] = REL2ACT(relrank+i, root);
 	  }
	} else if (tchild0<gasneti_nodes && tchild1>=gasneti_nodes) {
 	  num_child = 1;
 	  for (i=1; i<procs_per_node; i++) {
 	    if (relrank + i < gasneti_nodes) {
 	      num_child++;
 	    }
 	  }
 	  geom->child_list = (gasnet_node_t *) gasneti_malloc(sizeof(gasnet_node_t)*num_child);
 	  geom->child_list[0] = REL2ACT(tchild0,root);
 	  for (i=1; i<num_child; i++) {
 	    geom->child_list[i] = REL2ACT(relrank+i, root);
 	  }
	} else if (tchild0>=gasneti_nodes && tchild1<gasneti_nodes) {
 	  num_child = 1;
 	  for (i=1; i<procs_per_node; i++) {
 	    if (relrank + i < gasneti_nodes) {
 	      num_child++;
 	    }
 	  }
 	  geom->child_list = (gasnet_node_t *) gasneti_malloc(sizeof(gasnet_node_t)*num_child);
 	  geom->child_list[0] = REL2ACT(tchild1,root);
 	  for (i=1; i<num_child; i++) {
 	    geom->child_list[i] = REL2ACT(relrank+i, root);
 	  }
	} else {
 	  num_child = 0;
 	  for (i=1; i<procs_per_node; i++) {
 	    if (relrank + i < gasneti_nodes) {
 	      num_child++;
 	    }
 	  }
 	  geom->child_list = (gasnet_node_t *) gasneti_malloc(sizeof(gasnet_node_t)*num_child);
 	  geom->child_list[0] = REL2ACT(tchild1,root);
 	  for (i=0; i<num_child; i++) {
 	    geom->child_list[i] = REL2ACT(relrank+i, root);
 	  }
	}
      } else {
	geom->parent = (relrank / procs_per_node)*procs_per_node;
	num_child = 0;
	geom->child_list = NULL;
      }
      geom->child_id = /*???*/;
      geom->child_count = num_child;
      break;
    }
#endif

    default:
#ifdef GASNETE_COLL_TREE_GEOM_INIT_EXTRA
      /* Hook to add additional cases.  Return 0 if kind was unrecongnized. */
      if (!GASNETE_COLL_TREE_GEOM_INIT_EXTRA(geom, kind, root))
#endif
      {
        gasneti_fatalerror("unknown, invalid or unimplemented tree type");
      }
  }


  return geom;
  #undef START
  #undef ACT2REL
  #undef REL2ACT
}

/* No locks needed.
 * If ref_count reaches zero then it must not appear in the cache and
 * therefore cannot receive additional references.
 */
static void gasnete_coll_tree_geom_put(gasnete_coll_tree_geom_t *geom) {
  if (gasneti_weakatomic_decrement_and_test(&(geom->ref_count), 0)) {
    if (geom->child_list) {
      gasneti_free(geom->child_list);
    }
    gasneti_free(geom);
  }
}

/* XXX: should per-team */
static gasnete_coll_tree_geom_t *gasnete_coll_tree_geom_get(gasnete_coll_tree_kind_t kind, gasnet_node_t root) {
  /* Simple 1-element (trivially LRU) cache */
  /* XXX: larger and more complex cache is desired */
  static gasneti_mutex_t gasnete_coll_geom_lock = GASNETI_MUTEX_INITIALIZER;
  static gasnete_coll_tree_geom_t *gasnete_coll_tree_geom_cache = NULL;

  gasnete_coll_tree_geom_t *geom;

  gasneti_mutex_lock(&gasnete_coll_geom_lock);
    geom = gasnete_coll_tree_geom_cache;

    if_pf (geom == NULL) {
      /* only happens on first call */
      geom = gasnete_coll_tree_geom_cache = gasnete_coll_tree_geom_init(kind, root);
    } else if_pf ((geom->kind != kind) || (geom->root != root)) {
      gasnete_coll_tree_geom_put(geom);
      geom = gasnete_coll_tree_geom_cache = gasnete_coll_tree_geom_init(kind, root);
    }

    gasneti_weakatomic_increment(&(geom->ref_count), 0);
  gasneti_mutex_unlock(&gasnete_coll_geom_lock);

  return geom;
}

/* XXX: should per-team */
extern gasnete_coll_tree_data_t *gasnete_coll_tree_init(gasnete_coll_tree_kind_t kind, gasnet_node_t root GASNETE_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  gasnete_coll_tree_data_t *data = NULL;

  if_pf (td->tree_data_freelist == NULL) {
    data = gasneti_malloc(sizeof(gasnete_coll_tree_data_t));
  } else {
    data = td->tree_data_freelist;
    td->tree_data_freelist = *(gasnete_coll_tree_data_t **)data;
  }

  data->pipe_seg_size = gasnete_coll_pipe_seg_size ? gasnete_coll_pipe_seg_size : 1024;
  data->sent_bytes = 0;
  data->geom = gasnete_coll_tree_geom_get(kind, root);

  return data;
}

extern void gasnete_coll_tree_free(gasnete_coll_tree_data_t *tree GASNETE_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  gasnete_coll_tree_geom_put(tree->geom);
  *(gasnete_coll_tree_data_t **)tree = td->tree_data_freelist;
  td->tree_data_freelist = tree;
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_broadcast_nb() */

/* bcast Get: all nodes perform uncoordinated gets */
static int gasnete_coll_pf_bcast_Get(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->srcnode) {
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
      } else {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	data->handle = gasnete_get_nb_bulk(args->dst, args->srcnode, args->src,
					   args->nbytes GASNETE_THREAD_PASS);
	gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_Get(gasnet_team_handle_t team,
		       void *dst,
		       gasnet_image_t srcimage, void *src,
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  gasneti_assert(flags & GASNET_COLL_SINGLE);
  gasneti_assert(flags & GASNET_COLL_SRC_IN_SEGMENT);

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_Get, options,
					   NULL, sequence GASNETE_THREAD_PASS);
}

/* bcast Put: root node performs carefully ordered puts */
static int gasnete_coll_pf_bcast_Put(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode != args->srcnode) {
	/* Nothing to do */
      } else {
	void   *src   = args->src;
	void   *dst   = args->dst;
	size_t nbytes = args->nbytes;
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;

	/* Queue PUTS in an NBI access region */
	gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
	{
	  int i;

	  /* Put to nodes to the "right" of ourself */
	  for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i) {
	    gasnete_put_nbi_bulk(i, dst, src, nbytes GASNETE_THREAD_PASS);
	  }
	  /* Put to nodes to the "left" of ourself */
	  for (i = 0; i < gasneti_mynode; ++i) {
	    gasnete_put_nbi_bulk(i, dst, src, nbytes GASNETE_THREAD_PASS);
	  }
	}
	data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
	gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);

	/* Do local copy LAST, perhaps overlapping with communication */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(dst, src, nbytes);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_Put(gasnet_team_handle_t team,
		       void *dst,
		       gasnet_image_t srcimage, void *src,
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  gasneti_assert(flags & GASNET_COLL_SINGLE);
  gasneti_assert(flags & GASNET_COLL_DST_IN_SEGMENT);

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_Put, options,
					   NULL, sequence GASNETE_THREAD_PASS);
}

/* bcast Eager: root node performs carefully ordered eager puts */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_bcast_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_p2p_eager_put_all(op, args->src, args->nbytes, 0, 0, 1);	/* broadcast data */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
	gasneti_sync_reads();
	GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, data->p2p->data, args->nbytes);
      } else {
	break;	/* Stalled until data arrives */
      }
      data->state = 2;

    case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_Eager(gasnet_team_handle_t team,
			 void *dst,
			 gasnet_image_t srcimage, void *src,
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC)  |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  gasneti_assert(nbytes <= GASNETE_COLL_P2P_EAGER_MIN);

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_Eager, options,
					   NULL, sequence GASNETE_THREAD_PASS);
}

/* bcast RVGet: root node broadcasts address, others get from that address */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_bcast_RVGet(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier and rendezvous */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->src, 0, 1);	/* broadcast src address */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	gasneti_sync_reads();
	data->handle = gasnete_get_nb_bulk(args->dst, args->srcnode,
					   *(void **)data->p2p->data,
					   args->nbytes GASNETE_THREAD_PASS);
	gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      } else {
	break;
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_RVGet(gasnet_team_handle_t team,
			 void *dst,
			 gasnet_image_t srcimage, void *src,
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  gasneti_assert(flags & GASNET_COLL_SRC_IN_SEGMENT);

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_RVGet, options,
					   NULL, sequence GASNETE_THREAD_PASS);
}

/* bcast RVous: root node uses AM Mediums to send to addrs provided by each node */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
static int gasnete_coll_pf_bcast_RVous(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Rendevous w/ root to pass addr */
      if (gasneti_mynode == args->srcnode) {
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
      } else {
	/* Send our addr to root */
	gasnete_coll_p2p_send_rtr(op, data->p2p, gasneti_mynode, args->dst, args->srcnode, args->nbytes);
      }
      data->state = 2;

    case 2:
      if (gasneti_mynode == args->srcnode) {
	/* Root sends at most one AM per peer for each poll */
	gasnet_node_t i;
	int done = 1;
	for (i=0; i<gasneti_nodes; ++i) {
	  if (i == gasneti_mynode) continue;
	  done &= gasnete_coll_p2p_send_data(op, data->p2p, i, i, args->src, args->nbytes);
	}
	if (!done) {break;}
      } else if (!gasnete_coll_p2p_send_done(data->p2p)) {
	/* Not all data has arrived yet */
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_RVous(gasnet_team_handle_t team,
			 void *dst,
			 gasnet_image_t srcimage, void *src,
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_RVous, options,
					   NULL, sequence GASNETE_THREAD_PASS);
}

/* bcast TreePut */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
/* Naturally IN_NOSYNC, OUT_MYSYNC */
/* max size is MaxLongRequest */
static int gasnete_coll_pf_bcast_TreePut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->private_data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;
  int child;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;


    case 1:
      if (gasneti_mynode == args->srcnode) {
	for (child = 0; child < tree->geom->child_count; child++) {
	  gasnete_coll_p2p_signalling_put(op, tree->geom->child_list[child], args->dst, args->src, args->nbytes, 0, 1);
	}
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
	gasneti_sync_reads();
	for (child = 0; child < tree->geom->child_count; child++) {
	  gasnete_coll_p2p_signalling_put(op, tree->geom->child_list[child], args->dst, args->src, args->nbytes, 0, 1);
	}
      } else {
	break;	/* Waiting for parent to push data and signal */
      }
      data->state = 2;


    case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_tree_free(tree GASNETE_THREAD_PASS);
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_TreePut(gasnet_team_handle_t team,
		           void *dst,
			   gasnet_image_t srcimage, void *src,
			   size_t nbytes, int flags,
			   gasnete_coll_tree_kind_t kind,
			   uint32_t sequence
			   GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF(!(flags & GASNET_COLL_IN_NOSYNC))  |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF (flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  gasneti_assert(nbytes <= gasnet_AMMaxLongRequest());

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_TreePut, options,
					   gasnete_coll_tree_init(kind,
								  gasnete_coll_image_node(srcimage)
								  GASNETE_THREAD_PASS),
					   sequence
					   GASNETE_THREAD_PASS);
}

/* bcast TreeGet */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
/* Naturally IN_MYSYNC, OUT_MYSYNC */
/* size is unbounded */
static int gasnete_coll_pf_bcast_TreeGet(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->private_data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;
  int i;
  int child;


  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;


    case 1:
      if (gasneti_mynode == args->srcnode) {
        /* Sent my address to my children so they can issue their gets */
        for (child=0; child < tree->geom->child_count; child++) {
	  gasnete_coll_p2p_eager_addr(op, tree->geom->child_list[child], args->dst, 0, 1);
        }
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
        data->state = 3;
	break;	/* skip state 2 */
      } else if (data->p2p->state[0]){
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	/* I have address from my parent, so perform a get */
	gasneti_sync_reads();
	data->handle = gasnete_get_nb_bulk(args->dst, tree->geom->parent,
					   *(void **)data->p2p->data,
					   args->nbytes GASNETE_THREAD_PASS);
	gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      } else {
	break;
      }
      data->state = 2;


    case 2:
      gasneti_assert(gasneti_mynode != args->srcnode);
      if (data->handle != GASNET_INVALID_HANDLE) {
	 break;
      }

      /* Send ack to my parent */
      gasnete_coll_p2p_change_state(op, tree->geom->parent, tree->geom->child_id+1, 1);
      /* Sent my address to my children so they can issue their gets */
      for (child=0; child < tree->geom->child_count; child++) {
	gasnete_coll_p2p_eager_addr(op, tree->geom->child_list[child], args->dst, 0, 1);
      }
      data->state = 3;


    case 3:	/* Wait for all children to ack */
    {
      int done = 1;
      for (i=1; i <= tree->geom->child_count; i++) {
	if (data->p2p->state[i] == 0) {
	  done = 0;
	  break;
	}
      }

      if (done) {
	data->state = 4;
      } else {
	break;
      }
    }

    case 4:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_tree_free(tree GASNETE_THREAD_PASS);
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_TreeGet(gasnet_team_handle_t team,
		           void *dst,
			   gasnet_image_t srcimage, void *src,
			   size_t nbytes, int flags,
			   gasnete_coll_tree_kind_t kind,
			   uint32_t sequence
			   GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC)  |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_TreeGet, options,
					   gasnete_coll_tree_init(kind,
								  gasnete_coll_image_node(srcimage)
								  GASNETE_THREAD_PASS),
					   sequence
					   GASNETE_THREAD_PASS);
}

/* bcast TreeEager */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
/* Naturally IN_MYSYNC, OUT_MYSYNC */
/* Max size is the eager limit */
static int gasnete_coll_pf_bcast_TreeEager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->private_data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;
  int child;


  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;


    case 1:	/* Data movement */
      if (gasneti_mynode == args->srcnode) {
	for (child=0;child<tree->geom->child_count; child++){
	  gasnete_coll_p2p_eager_put(op, tree->geom->child_list[child], args->src, args->nbytes, 0, 1);
	}
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
	gasneti_sync_reads();
	for (child=0;child<tree->geom->child_count;child++) {
	  gasnete_coll_p2p_eager_put(op, tree->geom->child_list[child], data->p2p->data, args->nbytes, 0, 1);
	}
	GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, data->p2p->data, args->nbytes);
      } else {
	 break;	/* Stalled until data arrives */
      }
      data->state = 2;


    case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_tree_free(tree GASNETE_THREAD_PASS);
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_TreeEager(gasnet_team_handle_t team,
			     void *dst,
			     gasnet_image_t srcimage, void *src,
			     size_t nbytes, int flags,
			     gasnete_coll_tree_kind_t kind,
			     uint32_t sequence
			     GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC)  |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  gasneti_assert(nbytes <= GASNETE_COLL_P2P_EAGER_MIN);

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_TreeEager, options,
					   gasnete_coll_tree_init(kind,
								  gasnete_coll_image_node(srcimage)
								  GASNETE_THREAD_PASS),
					   sequence
					   GASNETE_THREAD_PASS);
}

/* XXX: broken for the following reasons.
   1) If nbytes is too big to fit in uint32_t, p2p->state will wrap
      Possible fix is to count segments, but would still have a potential
      problem if 4Billion * gasnet_AMMaxLongRequest :-)
   2) Use of signalling put currently is assuming in-order delivery!!
*/
static int gasnete_coll_pf_bcast_sig_TreePutPipe(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->private_data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;
  int i;
  int child;

  /* XXX: Current size limitation due to using p2p->state to count bytes */
  gasneti_assert(args->nbytes < (size_t)(~((uint32_t)0)));

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;


    case 1:
      if (gasneti_mynode == args->srcnode) {
	for (i=0; i<args->nbytes; i+=tree->pipe_seg_size) {
	  int msgsize = MIN(tree->pipe_seg_size, args->nbytes-tree->sent_bytes);
	  for (child=0; child<tree->geom->child_count; child++) {
	    /*  printf(stderr, "%d sending to %d\n", gasneti_mynode, tree->geom->child_list[child]); */
	    gasnete_coll_p2p_signalling_put(op, tree->geom->child_list[child], (char*)args->dst+i, (char*)args->src+i, msgsize, 0, i+msgsize);
	  }
	  GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
	  tree->sent_bytes += msgsize;
	}
	data->state = 2;
      } else if (data->p2p->state[0] > tree->sent_bytes) {
	int msgsize = MIN(tree->pipe_seg_size, args->nbytes-tree->sent_bytes);

	gasneti_sync_reads();
	for (child = 0; child<tree->geom->child_count; child++) {
	  gasnete_coll_p2p_signalling_put(op, tree->geom->child_list[child], (char*)args->dst+tree->sent_bytes,
					  (char*)args->dst+tree->sent_bytes, msgsize, 0, tree->sent_bytes+msgsize);
	}
	tree->sent_bytes += msgsize;

	if (tree->sent_bytes == args->nbytes) {
	  data->state = 2;
	} else {
	  break;
	}
      } else {
	break;
      }


    case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_tree_free(tree GASNETE_THREAD_PASS);
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}

/* XXX: broken for the following reasons.
   1) If nbytes is too big to fit in uint32_t, p2p->state will wrap
      Possible fix is to count segments, but would still have a potential
      problem if 4Billion * gasnet_AMMaxLongRequest :-)
   2) Use of AMs currently is assuming in-order delivery!!
*/
static int gasnete_coll_pf_bcast_TreeGetPipe(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->private_data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;
  int i;
  int child;

  /* XXX: Current size limitation due to using p2p->state to count bytes */
  gasneti_assert(args->nbytes < (size_t)(~((uint32_t)0)));

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;


    case 1:
      if (gasneti_mynode == args->srcnode) {
	for (child=0; child<tree->geom->child_count; child++) {
	  gasnete_coll_p2p_eager_addr(op, tree->geom->child_list[child], args->src, 0, args->nbytes);
	}

	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);

	data->state = 3;
	break;
      } else if (data->p2p->state[0] > tree->sent_bytes){
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	/* I have address from my parent, so perform a get of 1 segment */
	gasneti_sync_reads();

	data->handle =
	  gasnete_get_nb_bulk((char*)(args->dst)+tree->sent_bytes, tree->geom->parent,
			      ((char*)(*(void **)data->p2p->data))+tree->sent_bytes,
			      MIN(args->nbytes-tree->sent_bytes, gasnete_coll_pipe_seg_size)
			      GASNETE_THREAD_PASS);
	gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
	data->state = 2;
      } else {
	break;
      }


    case 2:
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }

      if (tree->sent_bytes == 0) { /*first message*/
	for (child=0; child<tree->geom->child_count; child++) {
	  gasnete_coll_p2p_eager_addr(op, tree->geom->child_list[child], args->dst, 0, MIN(args->nbytes-tree->sent_bytes, gasnete_coll_pipe_seg_size));
	}
      }

      tree->sent_bytes+=MIN(args->nbytes-tree->sent_bytes, gasnete_coll_pipe_seg_size);

      for (child=0; child<tree->geom->child_count; child++) {
	gasnete_coll_p2p_change_state(op, tree->geom->child_list[child], 0, tree->sent_bytes);
      }

      if (tree->sent_bytes<args->nbytes) {
	data->state = 1;	/* still more data to recv */
	break;
      } else {
	gasnete_coll_p2p_change_state(op, tree->geom->parent, tree->geom->child_id+1, args->nbytes);
	data->state = 3;
      }


    case 3: /* wait for all child nodes to acknowledge recpt */
    {
      int done = 1;
      for (i=1; i<=tree->geom->child_count; i++) {
	if (data->p2p->state[i]!=args->nbytes) {
	  done = 0;
	  break;
	}
      }

      if (done) {
	data->state=4;
      } else {
	break;
      }
    }


    case 4:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_tree_free(tree GASNETE_THREAD_PASS);
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_generic_broadcast_nb(gasnet_team_handle_t team,
				  void *dst,
				  gasnet_image_t srcimage, void *src,
				  size_t nbytes, int flags,
				  gasnete_coll_poll_fn poll_fn, int options,
				  void *private_data, uint32_t sequence
                                  GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t result;

  gasnete_coll_threads_lock(flags GASNETE_THREAD_PASS);
  if_pt (gasnete_coll_threads_first(GASNETE_THREAD_PASS_ALONE)) {
    gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
    GASNETE_COLL_GENERIC_SET_TAG(data, broadcast);
    data->args.broadcast.dst        = dst;
    #if !GASNET_SEQ
      data->args.broadcast.srcimage = srcimage;
    #endif
    data->args.broadcast.srcnode    = gasnete_coll_image_node(srcimage);
    data->args.broadcast.src        = src;
    data->args.broadcast.nbytes     = nbytes;
    data->options = options;
    data->private_data = private_data;
    result = gasnete_coll_op_generic_init(team, flags, data, poll_fn, sequence GASNETE_THREAD_PASS);
  } else {
    result = gasnete_coll_threads_get_handle(GASNETE_THREAD_PASS_ALONE);
  }
  gasnete_coll_threads_unlock(GASNETE_THREAD_PASS_ALONE);
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_broadcast_nb_default(gasnet_team_handle_t team,
				  void *dst,
				  gasnet_image_t srcimage, void *src,
				  size_t nbytes, int flags, uint32_t sequence
                                  GASNETE_THREAD_FARG)
{
  const size_t eager_limit = GASNETE_COLL_P2P_EAGER_MIN;

  #if GASNET_PAR
  /* Thread-local addr(s) - forward to bcastM_nb() */
  if (flags & GASNET_COLL_LOCAL) {
    return gasnete_coll_broadcastM_nb(team, &dst, srcimage, src, nbytes,
				      flags | GASNETE_COLL_THREAD_LOCAL, sequence
                                      GASNETE_THREAD_PASS);
  }
  #endif

  /* "Discover" in-segment flags if needed/possible */
  flags = gasnete_coll_segment_check(flags, 0, 0, dst, nbytes, 1, srcimage, src, nbytes);

  /* Choose algorithm based on arguments */
  if ((nbytes <= eager_limit) &&
      (flags & (GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_LOCAL))) {
    /* Small enough for Eager, which will eliminate any barriers for *_MYSYNC and
     * the need for passing addresses for _LOCAL
     * Eager is totally AM-based and thus safe regardless of *_IN_SEGMENT
     */
    return gasnete_coll_bcast_Eager(team, dst, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
  } else if (flags & GASNET_COLL_SRC_IN_SEGMENT) {
    if (flags & (GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_LOCAL)) {
      /* We can use Rendezvous+Get to eliminate any barriers for *_MYSYNC.
       * The Rendezvous is needed for _LOCAL.
       */
      return gasnete_coll_bcast_RVGet(team, dst, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    } else {
      return gasnete_coll_bcast_Get(team, dst, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    }
  } else if (flags & GASNET_COLL_DST_IN_SEGMENT) {
    if (flags & GASNET_COLL_SINGLE) {
      /* We use a Put-based algorithm w/ full barriers for *_{MY,ALL}SYNC */
      return gasnete_coll_bcast_Put(team, dst, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    } else {
      /* XXX: could do better w/ RVPut since dst is writtable */
      return gasnete_coll_bcast_RVous(team, dst, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    }
  } else {
    /* If we reach here then neither src nor dst is in-segment */
    return gasnete_coll_bcast_RVous(team, dst, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
  }
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_broadcastM_nb() */

/* bcastM Get: all nodes perform uncoordinated gets */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_bcastM_Get(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  int result = 0;

  gasneti_assert(op->flags & GASNET_COLL_SINGLE);

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_local_broadcast(gasnete_coll_my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, 0),
				     args->src, args->nbytes);
      } else {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
        /* Get only the 1st local image */
	data->handle = gasnete_get_nb_bulk(GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, 0),
					   args->srcnode, args->src, args->nbytes GASNETE_THREAD_PASS);
	gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      }
      data->state = 2;

    case 2:	/* Sync data movement and perform local copies */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      } else if (gasneti_mynode != args->srcnode) {
	void * const *p = &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, 0);
	gasneti_sync_reads();
	gasnete_coll_local_broadcast(gasnete_coll_my_images - 1, p + 1, *p, args->nbytes);
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcastM_Get(gasnet_team_handle_t team,
			void * const dstlist[],
			gasnet_image_t srcimage, void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcastM_Get, options,
					    NULL, sequence GASNETE_THREAD_PASS);
}

/* bcastM Put: root node performs carefully ordered puts */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_bcastM_Put(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  int result = 0;

  gasneti_assert(op->flags & GASNET_COLL_SINGLE);

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1: 	/* Initiate data movement */
      if (gasneti_mynode != args->srcnode) {
	/* Nothing to do */
      } else {
	void   *src   = args->src;
	size_t nbytes = args->nbytes;
	int i, j, limit;
	void * const *p;
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;

	/* Queue PUTS in an NBI access region */
	/* We don't use VIS here, since that would send the same data multiple times */
	gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
	{
	  /* Put to nodes to the "right" of ourself */
	  if (gasneti_mynode < gasneti_nodes - 1) {
	    p = &GASNETE_COLL_1ST_IMAGE(args->dstlist, gasneti_mynode + 1);
	    for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i) {
	      limit = gasnete_coll_all_images[i];
	      for (j = 0; j < limit; ++j) {
		gasnete_put_nbi_bulk(i, *p, src, nbytes GASNETE_THREAD_PASS);
		++p;
	      }
	    }
	  }
	  /* Put to nodes to the "left" of ourself */
	  if (gasneti_mynode != 0) {
	    p = &GASNETE_COLL_1ST_IMAGE(args->dstlist, 0);
	    for (i = 0; i < gasneti_mynode; ++i) {
	      limit = gasnete_coll_all_images[i];
	      for (j = 0; j < limit; ++j) {
		gasnete_put_nbi_bulk(i, *p, src, nbytes GASNETE_THREAD_PASS);
		++p;
	      }
	    }
	  }
	}
	data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
	gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);

	/* Do local copy LAST, perhaps overlapping with communication */
	gasnete_coll_local_broadcast(gasnete_coll_my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, 0),
				     src, nbytes);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcastM_Put(gasnet_team_handle_t team,
			void * const dstlist[],
			gasnet_image_t srcimage, void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcastM_Put, options,
					    NULL, sequence GASNETE_THREAD_PASS);
}

/* bcastM Eager: root node performs carefully ordered eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_bcastM_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_p2p_eager_put_all(op, args->src, args->nbytes, 0, 0, 1);	/* broadcast data */
	gasnete_coll_local_broadcast(gasnete_coll_my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				     args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
	gasneti_sync_reads();
	gasnete_coll_local_broadcast(gasnete_coll_my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				     data->p2p->data, args->nbytes);
      } else {
        break;  /* Stalled until data arrives */
      }
      data->state = 2;

    case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcastM_Eager(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcastM_Eager, options,
					    NULL, sequence GASNETE_THREAD_PASS);
}

/* bcastM RVGet: root node broadcasts address, others get from that address */
/* Valid for SINGLE and LOCAL, any size */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_bcastM_RVGet(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->src, 0, 1);	/* broadcast src address */
	/* Do local copy LAST, perhaps overlapping with communication */
	gasnete_coll_local_broadcast(gasnete_coll_my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				     args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	/* Get 1st image only */
	gasneti_sync_reads();
	data->handle = gasnete_get_nb_bulk(GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
					   args->srcnode, *(void **)data->p2p->data,
					   args->nbytes GASNETE_THREAD_PASS);
	gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      } else {
        break;
      }
      data->state = 2;

    case 2:	/* Complete data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	  break;
      } else if (gasneti_mynode != args->srcnode) {
	void * const *p = &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags);
	gasneti_sync_reads();
	gasnete_coll_local_broadcast(gasnete_coll_my_images - 1, p + 1, *p, args->nbytes);
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcastM_RVGet(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC)   |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC))|
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcastM_RVGet, options,
					    NULL, sequence GASNETE_THREAD_PASS);
}

/* bcastM RVous: root node uses AM Mediums to send to addrs provided by each node */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
static int gasnete_coll_pf_bcastM_RVous(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_p2p_t *p2p = data->p2p;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Rendevous w/ root to pass addr */
      if (gasneti_mynode == args->srcnode) {
        gasnete_coll_local_broadcast(gasnete_coll_my_images,
                                     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
                                     args->src, args->nbytes);
      } else {
	/* Send our addr to root */
	gasnete_coll_p2p_send_rtr(op, data->p2p, gasneti_mynode,
				  GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				  args->srcnode, args->nbytes);
      }
      data->state = 2;

    case 2:
      if (gasneti_mynode == args->srcnode) {
	/* Root sends at most one AM per peer for each poll */
	gasnet_node_t i;
	int done = 1;
	for (i=0; i<gasneti_nodes; ++i) {
	  if (i == gasneti_mynode) continue;
	  done &= gasnete_coll_p2p_send_data(op, data->p2p, i, i, args->src, args->nbytes);
	}
	if (!done) {break;}
      } else if (!gasnete_coll_p2p_send_done(data->p2p)) {
	/* Not all data has arrived yet */
	break;
      } else {
	/* All data has arrived - perform local copies */
	void * const *p = &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags);
	gasneti_sync_reads();
	gasnete_coll_local_broadcast(gasnete_coll_my_images - 1, p + 1, *p, args->nbytes);
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcastM_RVous(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcastM_RVous, options,
					    NULL, sequence GASNETE_THREAD_PASS);
}


extern gasnet_coll_handle_t
gasnete_coll_generic_broadcastM_nb(gasnet_team_handle_t team,
				   void * const dstlist[],
				   gasnet_image_t srcimage, void *src,
				   size_t nbytes, int flags,
				   gasnete_coll_poll_fn poll_fn, int options,
				   void *private_data, uint32_t sequence
                                   GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t result;

  gasnete_coll_threads_lock(flags GASNETE_THREAD_PASS);

  #if GASNET_PAR
  if (flags & GASNETE_COLL_THREAD_LOCAL) {
    gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
    gasnete_coll_generic_data_t *data;
    if_pt (gasnete_coll_threads_first(GASNETE_THREAD_PASS_ALONE)) {
      data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
      GASNETE_COLL_GENERIC_SET_TAG(data, broadcastM);
      data->threads.data = gasneti_calloc(gasnete_coll_my_images, sizeof(void *));
      data->args.broadcastM.dstlist    = (void**)data->threads.data;
      data->args.broadcastM.srcimage   = srcimage;
      data->args.broadcastM.srcnode    = gasnete_coll_image_node(srcimage);
      data->args.broadcastM.src        = src; /* may get overwritten, but not worth a branch */
      data->args.broadcastM.nbytes     = nbytes;
      data->options = options;
      data->private_data = private_data;
      result = gasnete_coll_op_generic_init(team, flags, data, poll_fn, sequence GASNETE_THREAD_PASS);
    } else {
      result = gasnete_coll_threads_get_handle_and_data(&data GASNETE_THREAD_PASS);
      if (td->my_image == srcimage) {
	gasneti_assert(src != NULL);
        data->args.broadcastM.src = src;
        gasneti_sync_writes();
      }
    }
    gasneti_assert(*dstlist != NULL);
    data->args.broadcastM.dstlist[td->my_local_image] = *dstlist; /* signalling write */
  } else
  #endif

  if_pt (gasnete_coll_threads_first(GASNETE_THREAD_PASS_ALONE)) {
    gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
    GASNETE_COLL_GENERIC_SET_TAG(data, broadcastM);
    data->args.broadcastM.dstlist    = (void**)dstlist;
    #if !GASNET_SEQ
      data->args.broadcastM.srcimage = srcimage;
    #endif
    data->args.broadcastM.srcnode    = gasnete_coll_image_node(srcimage);
    data->args.broadcastM.src        = src;
    data->args.broadcastM.nbytes     = nbytes;
    data->options = options;
    data->private_data = private_data;
    result = gasnete_coll_op_generic_init(team, flags, data, poll_fn, sequence GASNETE_THREAD_PASS);
  } else {
    result = gasnete_coll_threads_get_handle(GASNETE_THREAD_PASS_ALONE);
  }

  gasnete_coll_threads_unlock(GASNETE_THREAD_PASS_ALONE);
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_broadcastM_nb_default(gasnet_team_handle_t team,
				   void * const dstlist[],
				   gasnet_image_t srcimage, void *src,
				   size_t nbytes, int flags, uint32_t sequence
                                   GASNETE_THREAD_FARG)
{
  const size_t eager_limit = GASNETE_COLL_P2P_EAGER_MIN;

  #if GASNET_SEQ
  /* Exactly one thread-local addr - forward to bcast_nb() */
  if (flags & GASNET_COLL_LOCAL) {
    return gasnete_coll_broadcast_nb(team, dstlist[0], srcimage, src, nbytes,
				     flags, sequence GASNETE_THREAD_PASS);
  }
  #endif


  /* "Discover" in-segment flags if needed/possible */
  flags = gasnete_coll_segment_checkM(flags, 0, 0, dstlist, nbytes, 1, srcimage, src, nbytes);

  /* Choose algorithm based on arguments */
  if ((nbytes <= eager_limit) &&
      (flags & (GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_LOCAL))) {
    /* Small enough for Eager, which will eliminate any barriers for *_MYSYNC and
     * the need for passing addresses for _LOCAL
     * Eager is totally AM-based and thus safe regardless of *_IN_SEGMENT
     */
    return gasnete_coll_bcastM_Eager(team, dstlist, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
  } else if (flags & GASNET_COLL_SRC_IN_SEGMENT) {
    if (flags & (GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_LOCAL)) {
      /* We can use Rendezvous+Get to eliminate any barriers for *_MYSYNC.
       * The Rendezvous is needed for _LOCAL.
       */
      return gasnete_coll_bcastM_RVGet(team, dstlist, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    } else {
      return gasnete_coll_bcastM_Get(team, dstlist, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    }
  } else if (flags & GASNET_COLL_DST_IN_SEGMENT) {
    if (flags & GASNET_COLL_SINGLE) {
      /* We use a Put-based algorithm w/ full barriers for *_{MY,ALL}SYNC */
      return gasnete_coll_bcastM_Put(team, dstlist, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    } else {
      /* XXX: could do better w/ RVPut since dst is writtable */
      return gasnete_coll_bcastM_RVous(team, dstlist, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    }
  } else {
    /* If we reach here then neither src nor dst is in-segment */
    return gasnete_coll_bcastM_RVous(team, dstlist, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
  }
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_scatter_nb() */

/* scat Get: all nodes perform uncoordinated gets */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_scat_Get(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  int result = 0;

  gasneti_assert(op->flags & GASNET_COLL_SINGLE);

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->srcnode) {
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst,
				      gasnete_coll_scale_ptr(args->src, gasneti_mynode, args->nbytes),
				      args->nbytes);
      } else {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	data->handle = gasnete_get_nb_bulk(args->dst, args->srcnode,
					   gasnete_coll_scale_ptr(args->src, gasneti_mynode, args->nbytes),
					   args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scat_Get(gasnet_team_handle_t team,
		      void *dst,
		      gasnet_image_t srcimage, void *src,
		      size_t nbytes, int flags, uint32_t sequence
                      GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, flags,
					 &gasnete_coll_pf_scat_Get, options,
					 NULL, sequence GASNETE_THREAD_PASS);
}

/* scat Put: root node performs carefully ordered puts */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_scat_Put(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  int result = 0;

  gasneti_assert(op->flags & GASNET_COLL_SINGLE);

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:
      if (gasneti_mynode != args->srcnode) {
	/* Nothing to do */
      } else {
	void   *dst   = args->dst;
	size_t nbytes = args->nbytes;
	uintptr_t p;
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;

	/* Queue PUTS in an NBI access region */
	gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
	{
	  int i;

	  /* Put to nodes to the "right" of ourself */
	  p = (uintptr_t)gasnete_coll_scale_ptr(args->src, (gasneti_mynode + 1), nbytes);
	  for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i, p += nbytes) {
	    gasnete_put_nbi_bulk(i, dst, (void *)p, nbytes GASNETE_THREAD_PASS);
	  }
	  /* Put to nodes to the "left" of ourself */
	  p = (uintptr_t)gasnete_coll_scale_ptr(args->src, 0, nbytes);
	  for (i = 0; i < gasneti_mynode; ++i, p += nbytes) {
	    gasnete_put_nbi_bulk(i, dst, (void *)p, nbytes GASNETE_THREAD_PASS);
	  }
	}
	data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);

	/* Do local copy LAST, perhaps overlapping with communication */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(dst,
				      gasnete_coll_scale_ptr(args->src, gasneti_mynode, nbytes),
				      nbytes);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scat_Put(gasnet_team_handle_t team,
		      void *dst,
		      gasnet_image_t srcimage, void *src,
		      size_t nbytes, int flags, uint32_t sequence
                      GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, flags,
					 &gasnete_coll_pf_scat_Put, options,
					 NULL, sequence GASNETE_THREAD_PASS);
}

/* scat Eager: root node performs carefully ordered eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_scat_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_p2p_eager_put_all(op, args->src, args->nbytes, 1, 0, 1);	/* scatter data */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst,
				      gasnete_coll_scale_ptr(args->src, gasneti_mynode, args->nbytes),
				      args->nbytes);
      } else if (data->p2p->state[0]) {
	gasneti_sync_reads();
	GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, data->p2p->data, args->nbytes);
      } else {
	break;	/* Stalled until data arrives */
      }
      data->state = 2;

    case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scat_Eager(gasnet_team_handle_t team,
			void *dst,
			gasnet_image_t srcimage, void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, flags,
					 &gasnete_coll_pf_scat_Eager, options,
					 NULL, sequence GASNETE_THREAD_PASS);
}

/* scat RVGet: root node broadcasts address, others get from offsets from that address */
/* Valid for SINGLE and LOCAL, any size */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_scat_RVGet(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->src, 0, 1);	/* broadcast src address */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, 
				      gasnete_coll_scale_ptr(args->src, gasneti_mynode, args->nbytes),
				      args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	gasneti_sync_reads();
	data->handle = gasnete_get_nb_bulk(args->dst, args->srcnode,
					   gasnete_coll_scale_ptr(*(void **)data->p2p->data,
								  gasneti_mynode, args->nbytes),
					   args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      } else {
	break;
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scat_RVGet(gasnet_team_handle_t team,
			void *dst,
			gasnet_image_t srcimage, void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, flags,
					 &gasnete_coll_pf_scat_RVGet, options,
					 NULL, sequence GASNETE_THREAD_PASS);
}

/* scat RVous: root node uses AM Mediums to send to addrs provided by each node */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
static int gasnete_coll_pf_scat_RVous(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Rendevous w/ root to pass addr */
      if (gasneti_mynode == args->srcnode) {
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, 
				      gasnete_coll_scale_ptr(args->src, gasneti_mynode, args->nbytes),
				      args->nbytes);
      } else {
	/* Send our addr to root */
	gasnete_coll_p2p_send_rtr(op, data->p2p, gasneti_mynode, args->dst, args->srcnode, args->nbytes);
      }
      data->state = 2;

    case 2:
      if (gasneti_mynode == args->srcnode) {
	/* Root sends at most one AM per peer for each poll */
	gasnet_node_t i;
	int done = 1;
	for (i=0; i<gasneti_nodes; ++i) {
	  if (i == gasneti_mynode) continue;
	  done &= gasnete_coll_p2p_send_data(op, data->p2p, i, i,
					     gasnete_coll_scale_ptr(args->src, i, args->nbytes),
					     args->nbytes);
	}
	if (!done) {break;}
      } else if (!gasnete_coll_p2p_send_done(data->p2p)) {
	/* Not all data has arrived yet */
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scat_RVous(gasnet_team_handle_t team,
			 void *dst,
			 gasnet_image_t srcimage, void *src,
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, flags,
					 &gasnete_coll_pf_scat_RVous, options,
					 NULL, sequence GASNETE_THREAD_PASS);
}

extern gasnet_coll_handle_t
gasnete_coll_generic_scatter_nb(gasnet_team_handle_t team,
				void *dst,
				gasnet_image_t srcimage, void *src,
				size_t nbytes, int flags,
				gasnete_coll_poll_fn poll_fn, int options,
				void *private_data, uint32_t sequence
                                GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t result;

  gasnete_coll_threads_lock(flags GASNETE_THREAD_PASS);
  if_pt (gasnete_coll_threads_first(GASNETE_THREAD_PASS_ALONE)) {
    gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
    GASNETE_COLL_GENERIC_SET_TAG(data, scatter);
    data->args.scatter.dst        = dst;
    #if !GASNET_SEQ
      data->args.scatter.srcimage = srcimage;
    #endif
    data->args.scatter.srcnode    = gasnete_coll_image_node(srcimage);
    data->args.scatter.src        = src;
    data->args.scatter.nbytes     = nbytes;
    data->options = options;
    data->private_data = private_data;
    result = gasnete_coll_op_generic_init(team, flags, data, poll_fn, sequence GASNETE_THREAD_PASS);
  } else {
    result = gasnete_coll_threads_get_handle(GASNETE_THREAD_PASS_ALONE);
  }
  gasnete_coll_threads_unlock(GASNETE_THREAD_PASS_ALONE);
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_scatter_nb_default(gasnet_team_handle_t team,
				void *dst,
				gasnet_image_t srcimage, void *src,
				size_t nbytes, int flags, uint32_t sequence
                                GASNETE_THREAD_FARG)
{
  const size_t eager_limit = GASNETE_COLL_P2P_EAGER_MIN;

  #if GASNET_PAR
  /* Thread-local addr(s) - forward to scatM_nb() */
  if (flags & GASNET_COLL_LOCAL) {
    return gasnete_coll_scatterM_nb(team, &dst, srcimage, src, nbytes,
				    flags | GASNETE_COLL_THREAD_LOCAL, sequence
                                    GASNETE_THREAD_PASS);
  }
  #endif

  /* "Discover" in-segment flags if needed/possible */
  flags = gasnete_coll_segment_check(flags, 0, 0, dst, nbytes,
				     1, srcimage, src, nbytes*gasneti_nodes);

  /* Choose algorithm based on arguments */
  if ((flags & GASNET_COLL_DST_IN_SEGMENT) && (flags & GASNET_COLL_SRC_IN_SEGMENT)) {
    /* Both ends are in-segment */
    if ((flags & GASNET_COLL_IN_MYSYNC) || (flags & GASNET_COLL_LOCAL)) {
      if (nbytes <= eager_limit) {
        return gasnete_coll_scat_Eager(team, dst, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
      } else {
        return gasnete_coll_scat_RVGet(team, dst, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
      }
    } else if ((flags & GASNET_COLL_OUT_MYSYNC) && (nbytes <= eager_limit)) {
      return gasnete_coll_scat_Eager(team, dst, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    } else {
      return gasnete_coll_scat_Put(team, dst, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    }
  } else if (nbytes <= eager_limit) {
    /* Small enough for Eager, which works for out-of-segment src and/or dst */
    return gasnete_coll_scat_Eager(team, dst, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
  } else if (flags & GASNET_COLL_SRC_IN_SEGMENT) {
    /* Only the source is in-segment (and too big for Eager) */
    if ((flags & GASNET_COLL_IN_NOSYNC) && (flags & GASNET_COLL_SINGLE)) {
      return gasnete_coll_scat_Get(team, dst, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    } else {
      return gasnete_coll_scat_RVGet(team, dst, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    }
  } else if (flags & GASNET_COLL_DST_IN_SEGMENT) {
    /* Only the destination is in-segment (and too big for Eager) */
    if (flags & GASNET_COLL_SINGLE) {
      return gasnete_coll_scat_Put(team, dst, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    } else {
      /* XXX: could do better since DST is in-segment */
      return gasnete_coll_scat_RVous(team, dst, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    }
  } else {
    return gasnete_coll_scat_RVous(team, dst, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
  }
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_scatterM_nb() */

/* scatM Get: all nodes perform uncoordinated gets */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_scatM_Get(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatterM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatterM);
  int result = 0;

  gasneti_assert(op->flags & GASNET_COLL_SINGLE);

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_local_scatter(gasnete_coll_my_images,
				   &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, 0),
				   gasnete_coll_scale_ptr(args->src, gasnete_coll_my_offset, args->nbytes),
				   args->nbytes);
      } else {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	data->private_data = gasnete_coll_scale_ptr(args->src, gasnete_coll_my_offset, args->nbytes),
	data->handle = gasnete_geti(gasnete_synctype_nb, gasnete_coll_my_images,
				    &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, 0), args->nbytes,
			  	    args->srcnode, 1, &(data->private_data),
				    gasnete_coll_my_images * args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scatM_Get(gasnet_team_handle_t team,
		       void * const dstlist[],
		       gasnet_image_t srcimage, void *src,
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, flags,
					  &gasnete_coll_pf_scatM_Get, options,
					  NULL, sequence GASNETE_THREAD_PASS);
}

/* scatM Put: root node performs carefully ordered puts */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_scatM_Put(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatterM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatterM);
  int result = 0;

  gasneti_assert(op->flags & GASNET_COLL_SINGLE);

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:
      if (gasneti_mynode != args->srcnode) {
	/* Nothing to do */
      } else {
	size_t nbytes = args->nbytes;
	uintptr_t src_addr;
	int i;
	void ** srclist;
	void * const *p;
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;

	/* Allocate a source vector for puti */
	/* XXX: Use freelist? */
	srclist = gasneti_malloc(gasneti_nodes * sizeof(void *));
	data->private_data = srclist;

	/* Queue PUTIs in an NBI access region */
	/* XXX: is gasnete_puti(gasnete_synctype_nbi,...) correct non-tracing variant of puti ? */
	gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
	{
	  void **q;

	  /* Put to nodes to the "right" of ourself */
	  src_addr = (uintptr_t)gasnete_coll_scale_ptr(args->src,
			  			       gasnete_coll_all_offset[gasneti_mynode + 1],
						       nbytes);
	  p = &GASNETE_COLL_1ST_IMAGE(args->dstlist, gasneti_mynode + 1);
	  q = &srclist[gasneti_mynode + 1];
	  for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i) {
	    size_t count = gasnete_coll_all_images[i];
	    size_t len = count * nbytes;
	    *q = (void *)src_addr;
	    gasnete_puti(gasnete_synctype_nbi, i, count, p, nbytes, 1, q, len GASNETE_THREAD_PASS);
	    src_addr += len;
	    p += count;
	    ++q;
	  }
	  /* Put to nodes to the "left" of ourself */
	  src_addr = (uintptr_t)gasnete_coll_scale_ptr(args->src, 0, nbytes);
	  p = &GASNETE_COLL_1ST_IMAGE(args->dstlist, 0);
	  q = &srclist[0];
	  for (i = 0; i < gasneti_mynode; ++i) {
	    size_t count = gasnete_coll_all_images[i];
	    size_t len = count * nbytes;
	    *q = (void *)src_addr;
	    gasnete_puti(gasnete_synctype_nbi, i, count, p, nbytes, 1, q, len GASNETE_THREAD_PASS);
	    src_addr += len;
	    p += count;
	    ++q;
	  }
	}
	data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);

	/* Do local copy LAST, perhaps overlapping with communication */
	gasnete_coll_local_scatter(gasnete_coll_my_images,
				   &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, 0),
				   gasnete_coll_scale_ptr(args->src, gasnete_coll_my_offset, nbytes),
				   nbytes);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (gasneti_mynode == args->srcnode) {
        if (data->handle != GASNET_INVALID_HANDLE) {
	  break;
        }
        gasneti_free(data->private_data);	/* the temporary srclist */
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scatM_Put(gasnet_team_handle_t team,
		       void * const dstlist[],
		       gasnet_image_t srcimage, void *src,
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, flags,
					  &gasnete_coll_pf_scatM_Put, options,
					  NULL, sequence GASNETE_THREAD_PASS);
}

/* scatM Eager: root node performs carefully ordered eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_scatM_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatterM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatterM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Data movement */
      if (gasneti_mynode == args->srcnode) {
	const void * const src   = args->src;
	size_t nbytes = args->nbytes;
	uintptr_t src_addr;
	int i;

	/* Send to nodes to the "right" of ourself */
	if (gasneti_mynode < gasneti_nodes - 1) {
	  src_addr = (uintptr_t)gasnete_coll_scale_ptr(src, gasnete_coll_all_offset[gasneti_mynode + 1], nbytes);
	  for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i) {
	    const size_t count = gasnete_coll_all_images[i];

	    gasnete_coll_p2p_eager_putM(op, i, (void *)src_addr, count, nbytes, 0, 1);
	    src_addr += count * nbytes;
	  }
	}
	/* Send to nodes to the "left" of ourself */
	if (gasneti_mynode > 0) {
	  src_addr = (uintptr_t)gasnete_coll_scale_ptr(src, 0, nbytes);
	  for (i = 0; i < gasneti_mynode; ++i) {
	    const size_t count = gasnete_coll_all_images[i];

	    gasnete_coll_p2p_eager_putM(op, i, (void *)src_addr, count, nbytes, 0, 1);
	    src_addr += count * nbytes;
	  }
	}

	/* Local data movement */
	gasnete_coll_local_scatter(gasnete_coll_my_images,
				   &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				   gasnete_coll_scale_ptr(src, gasnete_coll_my_offset, nbytes), nbytes);
      } else {
	gasnete_coll_p2p_t *p2p = data->p2p;
	volatile uint32_t *state;
	size_t nbytes = args->nbytes;
	void * const *p;
	uintptr_t src_addr;
	int i, done;

	gasneti_assert(p2p != NULL);
	gasneti_assert(p2p->state != NULL);
	state = data->p2p->state;
	gasneti_assert(p2p->data != NULL);

	done = 1;
	p = &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags);
	src_addr = (uintptr_t)(p2p->data);
	for (i = 0; i < gasnete_coll_my_images; ++i, ++p, src_addr += nbytes) {
	  uint32_t s = state[i];

	  if (s == 0) {
	    /* Nothing received yet */
	    done = 0;
	  } else {
	    /* Received but not yet copied into place */
	    gasneti_sync_reads();
	    GASNETE_FAST_UNALIGNED_MEMCPY(*p, (void *)src_addr, nbytes);
	    state[i] = 2;
	  }
	}

	if (!done) { break; }
      }
      data->state = 2;

    case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scatM_Eager(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC)|
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, flags,
					  &gasnete_coll_pf_scatM_Eager, options,
					  NULL, sequence GASNETE_THREAD_PASS);
}

/* scatM RVGet: root node scatters address, others get from that address */
/* Valid for SINGLE and LOCAL, any size */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_scatM_RVGet(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatterM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatterM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->src, 0, 1);	/* broadcast src address */
	gasnete_coll_local_scatter(gasnete_coll_my_images,
				   &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				   gasnete_coll_scale_ptr(args->src, gasnete_coll_my_offset, args->nbytes),
				   args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	gasneti_sync_reads();
	data->private_data = gasnete_coll_scale_ptr(*(void **)data->p2p->data,
					       gasnete_coll_my_offset,
					       args->nbytes);
	data->handle = gasnete_geti(gasnete_synctype_nb,
				    gasnete_coll_my_images,
				    &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags), args->nbytes,
				    args->srcnode, 1, &(data->private_data),
				    args->nbytes * gasnete_coll_my_images GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      } else {
	break;
      }
      data->state = 2;

    case 2:
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scatM_RVGet(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, flags,
					  &gasnete_coll_pf_scatM_RVGet, options,
					  NULL, sequence GASNETE_THREAD_PASS);
}

/* scatM RVous: root node uses AM Mediums to send to addrs provided by each node */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
static int gasnete_coll_pf_scatM_RVous(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_p2p_t *p2p = data->p2p;
  const gasnete_coll_scatterM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatterM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Rendevous w/ root to pass addr */
      if (gasneti_mynode == args->srcnode) {
        gasnete_coll_local_scatter(gasnete_coll_my_images,
                                   &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
                                   gasnete_coll_scale_ptr(args->src, gasnete_coll_my_offset, args->nbytes),
                                   args->nbytes);
      } else {
	/* Send our addrs to root */
	gasneti_assert(GASNETE_COLL_P2P_EAGER_SCALE >= sizeof(struct gasnete_coll_p2p_send_struct));
	gasnete_coll_p2p_send_rtrM(op, data->p2p, gasnete_coll_my_offset,
				   &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				   args->srcnode, args->nbytes, gasnete_coll_my_images);
      }
      data->state = 2;

    case 2:
      if (gasneti_mynode == args->srcnode) {
	/* Root sends at most one AM per peer for each poll */
	gasnet_node_t i;
	int done = 1;
	for (i=0; i<gasneti_nodes; ++i) {
	  gasnet_image_t j,k;
	  if (i == gasneti_mynode) continue;
	  k = gasnete_coll_all_offset[i] + gasnete_coll_all_images[i];
	  for (j = gasnete_coll_all_offset[i]; j < k; ++j) {
	    /* TODO: some pack/unpack of data (makes completion counter harder) */
	    done &= gasnete_coll_p2p_send_data(op, data->p2p, i, j,
					       gasnete_coll_scale_ptr(args->src, j, args->nbytes),
					       args->nbytes);
	  }
	}
	if (!done) {break;}
      } else if (!gasnete_coll_p2p_send_done(data->p2p)) {
	/* Not all data has arrived yet */
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scatM_RVous(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, flags,
					  &gasnete_coll_pf_scatM_RVous, options,
					  NULL, sequence GASNETE_THREAD_PASS);
}

extern gasnet_coll_handle_t
gasnete_coll_generic_scatterM_nb(gasnet_team_handle_t team,
				 void * const dstlist[],
				 gasnet_image_t srcimage, void *src,
				 size_t nbytes, int flags,
				 gasnete_coll_poll_fn poll_fn, int options,
				 void *private_data, uint32_t sequence
                                 GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t result;

  gasnete_coll_threads_lock(flags GASNETE_THREAD_PASS);

  #if GASNET_PAR
  if (flags & GASNETE_COLL_THREAD_LOCAL) {
    gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
    gasnete_coll_generic_data_t *data;
    if_pt (gasnete_coll_threads_first(GASNETE_THREAD_PASS_ALONE)) {
      data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
      GASNETE_COLL_GENERIC_SET_TAG(data, scatterM);
      data->threads.data = gasneti_calloc(gasnete_coll_my_images, sizeof(void *));
      data->args.scatterM.dstlist    = (void**)data->threads.data;
      data->args.scatterM.srcimage   = srcimage;
      data->args.scatterM.srcnode    = gasnete_coll_image_node(srcimage);
      data->args.scatterM.src        = src; /* may get overwritten, but not worth a branch */
      data->args.scatterM.nbytes     = nbytes;
      data->options = options;
      data->private_data = private_data;
      result = gasnete_coll_op_generic_init(team, flags, data, poll_fn, sequence GASNETE_THREAD_PASS);
    } else {
      result = gasnete_coll_threads_get_handle_and_data(&data GASNETE_THREAD_PASS);
      if (td->my_image == srcimage) {
	gasneti_assert(src != NULL);
        data->args.scatterM.src = src;
        gasneti_sync_writes();
      }
    }
    gasneti_assert(*dstlist != NULL);
    data->args.scatterM.dstlist[td->my_local_image] = *dstlist; /* signalling write */
  } else
  #endif

  if_pt (gasnete_coll_threads_first(GASNETE_THREAD_PASS_ALONE)) {
    gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
    GASNETE_COLL_GENERIC_SET_TAG(data, scatterM);
    data->args.scatterM.dstlist    = (void**)dstlist;
    #if !GASNET_SEQ
      data->args.scatterM.srcimage = srcimage;
    #endif
    data->args.scatterM.srcnode    = gasnete_coll_image_node(srcimage);
    data->args.scatterM.src        = src;
    data->args.scatterM.nbytes     = nbytes;
    data->options = options;
    data->private_data = private_data;
    result = gasnete_coll_op_generic_init(team, flags, data, poll_fn, sequence GASNETE_THREAD_PASS);
  } else {
    result = gasnete_coll_threads_get_handle(GASNETE_THREAD_PASS_ALONE);
  }

  gasnete_coll_threads_unlock(GASNETE_THREAD_PASS_ALONE);
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_scatterM_nb_default(gasnet_team_handle_t team,
				 void * const dstlist[],
				 gasnet_image_t srcimage, void *src,
				 size_t nbytes, int flags, uint32_t sequence
                                 GASNETE_THREAD_FARG)
{
  const size_t eager_limit = GASNETE_COLL_P2P_EAGER_MIN;

  #if GASNET_SEQ
  /* Exactly one thread-local addr - forward to scat_nb() */
  if (flags & GASNET_COLL_LOCAL) {
    return gasnete_coll_scatter_nb(team, dstlist[0], srcimage, src, nbytes,
				   flags, sequence GASNETE_THREAD_PASS);
  }
  #endif

  /* "Discover" in-segment flags if needed/possible */
  flags = gasnete_coll_segment_checkM(flags, 0, 0, dstlist, nbytes,
				      1, srcimage, src, nbytes*gasneti_nodes);

  /* Choose algorithm based on arguments */
  if ((flags & GASNET_COLL_DST_IN_SEGMENT) && (flags & GASNET_COLL_SRC_IN_SEGMENT)) {
    /* Both ends are in-segment */
    if ((flags & GASNET_COLL_IN_MYSYNC) || (flags & GASNET_COLL_LOCAL)) {
      if (nbytes <= eager_limit) {
        return gasnete_coll_scatM_Eager(team, dstlist, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
      } else {
        return gasnete_coll_scatM_RVGet(team, dstlist, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
      }
    } else if ((flags & GASNET_COLL_OUT_MYSYNC) && (nbytes <= eager_limit)) {
      return gasnete_coll_scatM_Eager(team, dstlist, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    } else {
      return gasnete_coll_scatM_Get(team, dstlist, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    }
  } else if (nbytes <= eager_limit) {
    /* Small enough for Eager, which works for out-of-segment src and/or dst */
    return gasnete_coll_scatM_Eager(team, dstlist, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
  } else if (flags & GASNET_COLL_SRC_IN_SEGMENT) {
    /* Only the source is in-segment (and too big for Eager) */
    if ((flags & GASNET_COLL_IN_NOSYNC) && (flags & GASNET_COLL_SINGLE)) {
      return gasnete_coll_scatM_Get(team, dstlist, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    } else {
      return gasnete_coll_scatM_RVGet(team, dstlist, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    }
  } else if (flags & GASNET_COLL_DST_IN_SEGMENT) {
    /* Only the destination is in-segment (and too big for Eager) */
    if (flags & GASNET_COLL_SINGLE) {
      return gasnete_coll_scatM_Put(team, dstlist, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    } else {
      /* Could do better since dst is in-segment */
      return gasnete_coll_scatM_RVous(team, dstlist, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    }
  } else {
    return gasnete_coll_scatM_RVous(team, dstlist, srcimage, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
  }
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_nb() */

/* gath Get: root node performs carefully ordered gets */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_gath_Get(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result = 0;

  gasneti_assert(op->flags & GASNET_COLL_SINGLE);

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode != args->dstnode) {
	/* Nothing to do */
      } else {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	/* Queue GETs in an NBI access region */
	gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
	{
	  int i;
	  uintptr_t p;

	  /* Get from nodes to the "right" of ourself */
	  p = (uintptr_t)gasnete_coll_scale_ptr(args->dst, (gasneti_mynode + 1), args->nbytes);
	  for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i, p += args->nbytes) {
	    gasnete_get_nbi_bulk((void *)p, i, args->src, args->nbytes GASNETE_THREAD_PASS);
	  }
	  /* Get from nodes to the "left" of ourself */
	  p = (uintptr_t)gasnete_coll_scale_ptr(args->dst, 0, args->nbytes);
	  for (i = 0; i < gasneti_mynode; ++i, p += args->nbytes) {
	    gasnete_get_nbi_bulk((void *)p, i, args->src, args->nbytes GASNETE_THREAD_PASS);
	  }
	}
	data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);

	/* Do local copy LAST, perhaps overlapping with communication */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(args->dst, gasneti_mynode, args->nbytes),
				      args->src, args->nbytes);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gath_Get(gasnet_team_handle_t team,
		      gasnet_image_t dstimage, void *dst,
		      void *src,
		      size_t nbytes, int flags, uint32_t sequence
                      GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, flags,
					&gasnete_coll_pf_gath_Get, options,
					NULL, sequence GASNETE_THREAD_PASS);
}

/* gath Put: all nodes perform uncoordinated puts */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_gath_Put(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result = 0;

  gasneti_assert(op->flags & GASNET_COLL_SINGLE);

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->dstnode) {
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(args->dst, gasneti_mynode, args->nbytes),
				      args->src, args->nbytes);
      } else {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	data->handle = gasnete_put_nb_bulk(args->dstnode, 
					   gasnete_coll_scale_ptr(args->dst, gasneti_mynode, args->nbytes),
					   args->src, args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gath_Put(gasnet_team_handle_t team,
		      gasnet_image_t dstimage, void *dst,
		      void *src,
		      size_t nbytes, int flags, uint32_t sequence
                      GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, flags,
					&gasnete_coll_pf_gath_Put, options,
					NULL, sequence GASNETE_THREAD_PASS);
}

/* gath Eager: all nodes perform uncoordinated eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on root node */
static int gasnete_coll_pf_gath_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

      /* Initiate data movement */
      if (gasneti_mynode != args->dstnode) {
	gasnete_coll_p2p_eager_put(op, args->dstnode, args->src, args->nbytes, gasneti_mynode, 1);
      } else {
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(args->dst, gasneti_mynode, args->nbytes),
				      args->src, args->nbytes);
	data->p2p->state[gasneti_mynode] = 2;
      }

    case 1:	/* Complete data movement */
      if (gasneti_mynode == args->dstnode) {
	gasnete_coll_p2p_t *p2p = data->p2p;
	volatile uint32_t *state;
	uintptr_t dst_addr, src_addr;
	size_t nbytes = args->nbytes;
	int i, done;

	gasneti_assert(p2p != NULL);
	gasneti_assert(p2p->state != NULL);
	state = data->p2p->state;
	gasneti_assert(p2p->data != NULL);

	done = 1;
	dst_addr = (uintptr_t)(args->dst);
	src_addr = (uintptr_t)(p2p->data);
	for (i = 0; i < gasneti_nodes; ++i, dst_addr += nbytes, src_addr += nbytes) {
	  uint32_t s = state[i];

	  if (s == 0) {
	    /* Nothing received yet */
	    done = 0;
	  } else if (s == 1) {
	    /* Received but not yet copied into place */
	    gasneti_sync_reads();
	    GASNETE_FAST_UNALIGNED_MEMCPY((void *)dst_addr, (void *)src_addr, nbytes);
	    state[i] = 2;
	  }
	}

	if (!done) { break; }
      }
      data->state = 2;

    case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gath_Eager(gasnet_team_handle_t team,
			gasnet_image_t dstimage, void *dst,
			void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC)|
		GASNETE_COLL_GENERIC_OPT_P2P_IF(gasnete_coll_image_is_local(dstimage));

  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, flags,
					&gasnete_coll_pf_gath_Eager, options,
					NULL, sequence GASNETE_THREAD_PASS);
}

/* gath RVPut: root node broadcasts addresses, others put to that address (plus offset) */
/* Valid for SINGLE and LOCAL, any size */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_gath_RVPut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->dstnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->dst, 0, 1);	/* broadcast dst address */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(args->dst, gasneti_mynode, args->nbytes),
				      args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	gasneti_sync_reads();
	data->handle = gasnete_put_nb_bulk(args->dstnode,
					   gasnete_coll_scale_ptr(*(void **)data->p2p->data,
								  gasneti_mynode, args->nbytes),
					   args->src, args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      } else {
	  break;
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gath_RVPut(gasnet_team_handle_t team,
			 gasnet_image_t dstimage, void *dst,
			 void *src,
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(dstimage));

  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, flags,
					&gasnete_coll_pf_gath_RVPut, options,
					NULL, sequence GASNETE_THREAD_PASS);
}

/* gath RVous: non-root nodes use AM Mediums to send to addrs provided by root */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
static int gasnete_coll_pf_gath_RVous(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Root send addrs */
      if (gasneti_mynode == args->dstnode) {
	gasnet_node_t i;
	for (i = 0; i < gasneti_nodes; ++i) {
	  if (i == gasneti_mynode) continue;
	  gasnete_coll_p2p_send_rtr(op, data->p2p, 0,
				    gasnete_coll_scale_ptr(args->dst, i, args->nbytes),
				    i, args->nbytes);
	}
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(args->dst, gasneti_mynode, args->nbytes),
				      args->src, args->nbytes);
      }
      data->state = 2;

    case 2:
      if (gasneti_mynode != args->dstnode) {
	/* non-root nodes send at most one AM per poll */
	int done = gasnete_coll_p2p_send_data(op, data->p2p, args->dstnode, 0, args->src, args->nbytes);
	if (!done) {break;}
      } else if (!gasnete_coll_p2p_send_done(data->p2p)) {
	/* Not all data has arrived yet */
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gath_RVous(gasnet_team_handle_t team,
			gasnet_image_t dstimage, void *dst,
			void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, flags,
					&gasnete_coll_pf_gath_RVous, options,
					NULL, sequence GASNETE_THREAD_PASS);
}

extern gasnet_coll_handle_t
gasnete_coll_generic_gather_nb(gasnet_team_handle_t team,
			       gasnet_image_t dstimage, void *dst,
			       void *src,
			       size_t nbytes, int flags,
			       gasnete_coll_poll_fn poll_fn, int options,
			       void *private_data, uint32_t sequence
                               GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t result;

  gasnete_coll_threads_lock(flags GASNETE_THREAD_PASS);
  if_pt (gasnete_coll_threads_first(GASNETE_THREAD_PASS_ALONE)) {
    gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
    GASNETE_COLL_GENERIC_SET_TAG(data, gather);
    #if !GASNET_SEQ
      data->args.gather.dstimage = dstimage;
    #endif
    data->args.gather.dstnode    = gasnete_coll_image_node(dstimage);
    data->args.gather.dst        = dst;
    data->args.gather.src        = src;
    data->args.gather.nbytes     = nbytes;
    data->options = options;
    data->private_data = private_data;
    result = gasnete_coll_op_generic_init(team, flags, data, poll_fn, sequence GASNETE_THREAD_PASS);
  } else {
    result = gasnete_coll_threads_get_handle(GASNETE_THREAD_PASS_ALONE);
  }
  gasnete_coll_threads_unlock(GASNETE_THREAD_PASS_ALONE);
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_gather_nb_default(gasnet_team_handle_t team,
			       gasnet_image_t dstimage, void *dst,
			       void *src,
			       size_t nbytes, int flags, uint32_t sequence
                               GASNETE_THREAD_FARG)
{
  const size_t eager_limit = GASNETE_COLL_P2P_EAGER_MIN;

  #if GASNET_PAR
  /* Thread-local addr(s) - forward to gathM_nb() */
  if (flags & GASNET_COLL_LOCAL) {
    return gasnete_coll_gatherM_nb(team, dstimage, dst, &src, nbytes,
				   flags | GASNETE_COLL_THREAD_LOCAL, sequence
                                   GASNETE_THREAD_PASS);
  }
  #endif

  /* "Discover" in-segment flags if needed/possible */
  flags = gasnete_coll_segment_check(flags, 1, dstimage, dst, nbytes*gasneti_nodes,
				     0, 0, src, nbytes);

  /* Choose algorithm based on arguments */
  if ((flags & GASNET_COLL_DST_IN_SEGMENT) && (flags & GASNET_COLL_SRC_IN_SEGMENT)) {
    /* Both ends are in-segment */
    if ((flags & GASNET_COLL_IN_MYSYNC) || (flags & GASNET_COLL_LOCAL)) {
      if (nbytes <= eager_limit) {
        return gasnete_coll_gath_Eager(team, dstimage, dst, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
      } else {
        return gasnete_coll_gath_RVPut(team, dstimage, dst, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
      }
    } else if ((flags & GASNET_COLL_OUT_MYSYNC) && (nbytes <= eager_limit)) {
      return gasnete_coll_gath_Eager(team, dstimage, dst, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    } else {
      return gasnete_coll_gath_Put(team, dstimage, dst, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    }
  } else if (nbytes <= eager_limit) {
    /* Small enough for Eager, which works for out-of-segment src and/or dst */
    return gasnete_coll_gath_Eager(team, dstimage, dst, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
  } else if (flags & GASNET_COLL_DST_IN_SEGMENT) {
    /* Only the destination is in-segment (and too big for Eager) */
    if ((flags & GASNET_COLL_IN_NOSYNC) && (flags & GASNET_COLL_SINGLE)) {
      return gasnete_coll_gath_Put(team, dstimage, dst, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    } else {
      return gasnete_coll_gath_RVPut(team, dstimage, dst, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    }
  } else if (flags & GASNET_COLL_SRC_IN_SEGMENT) {
    /* Only the source is in-segment (and too big for Eager) */
    if (flags & GASNET_COLL_SINGLE) {
      return gasnete_coll_gath_Get(team, dstimage, dst, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    } else {
      /* XXX: could do better since src is in-segment */
      return gasnete_coll_gath_RVous(team, dstimage, dst, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
    }
  } else {
    return gasnete_coll_gath_RVous(team, dstimage, dst, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
  }
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gatherM_nb() */

/* gathM Get: root node performs carefully ordered gets */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_gathM_Get(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gatherM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gatherM);
  int result = 0;

  gasneti_assert(op->flags & GASNET_COLL_SINGLE);

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode != args->dstnode) {
	/* Nothing to do */
      } else {
	size_t nbytes = args->nbytes;
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;

	/* Queue GETIs in an NBI access region */
	gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS);
	{
	  void **q;
	  uintptr_t dst_addr;
	  int i;
	  void * const *p;
	  void ** dstlist = gasneti_malloc(gasneti_nodes * sizeof(void *));
	  data->private_data = dstlist;

	  /* Get from the "right" of ourself */
	  dst_addr = (uintptr_t)gasnete_coll_scale_ptr(args->dst,
			  			       gasnete_coll_all_offset[gasneti_mynode + 1],
						       nbytes);
	  p = &GASNETE_COLL_1ST_IMAGE(args->srclist, gasneti_mynode + 1);
	  q = &dstlist[gasneti_mynode + 1];
	  for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i) {
	    size_t count = gasnete_coll_all_images[i];
	    size_t len = count * nbytes;
	    *q = (void *)dst_addr;
	    gasnete_geti(gasnete_synctype_nbi, 1, q, len, i, count, p, nbytes GASNETE_THREAD_PASS);
	    dst_addr += len;
	    p += count;
	    ++q;
	  }
	  /* Get from nodes to the "left" of ourself */
	  dst_addr = (uintptr_t)args->dst;
	  dst_addr = (uintptr_t)gasnete_coll_scale_ptr(args->dst, 0, nbytes);
	  p = &GASNETE_COLL_1ST_IMAGE(args->srclist, 0);
	  q = &dstlist[0];
	  for (i = 0; i < gasneti_mynode; ++i) {
	    size_t count = gasnete_coll_all_images[i];
	    size_t len = count * nbytes;
	    *q = (void *)dst_addr;
	    gasnete_geti(gasnete_synctype_nbi, 1, q, len, i, count, p, nbytes GASNETE_THREAD_PASS);
	    dst_addr += len;
	    p += count;
	    ++q;
	  }
	}
	data->handle = gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);

	/* Do local copy LAST, perhaps overlapping with communication */
	gasnete_coll_local_gather(gasnete_coll_my_images,
				  gasnete_coll_scale_ptr(args->dst, gasnete_coll_my_offset, nbytes),
				  &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, 0), nbytes);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (gasneti_mynode == args->dstnode) {
        if (data->handle != GASNET_INVALID_HANDLE) {
	  break;
        }
        gasneti_free(data->private_data);	/* the temporary dstlist */
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gathM_Get(gasnet_team_handle_t team,
		       gasnet_image_t dstimage, void *dst,
		       void * const srclist[],
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, flags,
					 &gasnete_coll_pf_gathM_Get, options,
					 NULL, sequence GASNETE_THREAD_PASS);
}

/* gathM Put: all nodes perform uncoordinated puts */
/* Valid for SINGLE only, any size */
static int gasnete_coll_pf_gathM_Put(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gatherM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gatherM);
  int result = 0;

  gasneti_assert(op->flags & GASNET_COLL_SINGLE);

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (gasneti_mynode == args->dstnode) {
	gasnete_coll_local_gather(gasnete_coll_my_images,
				  gasnete_coll_scale_ptr(args->dst, gasnete_coll_my_offset, args->nbytes),
				  &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, 0), args->nbytes);
      } else {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	data->private_data = gasnete_coll_scale_ptr(args->dst, gasnete_coll_my_offset, args->nbytes);
	data->handle = gasnete_puti(gasnete_synctype_nb, args->dstnode,
				    1, &(data->private_data), gasnete_coll_my_images * args->nbytes,
				    gasnete_coll_my_images, &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, 0),
				    args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }

      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gathM_Put(gasnet_team_handle_t team,
		       gasnet_image_t dstimage, void *dst,
		       void * const srclist[],
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));

  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, flags,
					 &gasnete_coll_pf_gathM_Put, options,
					 NULL, sequence GASNETE_THREAD_PASS);
}

/* gathM Eager: all nodes perform uncoordinated eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on root node */
static int gasnete_coll_pf_gathM_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gatherM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gatherM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

      /* Initiate data movement */
      if (gasneti_mynode != args->dstnode) {
	size_t nbytes = args->nbytes;
	void * tmp = gasneti_malloc(gasnete_coll_my_images * nbytes);
	gasnete_coll_local_gather(gasnete_coll_my_images, tmp,
				  &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, op->flags), nbytes);
        gasnete_coll_p2p_eager_putM(op, args->dstnode, tmp, gasnete_coll_my_images,
				    nbytes, gasnete_coll_my_offset, 1);
	gasneti_free(tmp);
      } else {
	volatile uint32_t *s;
	int i;

	gasnete_coll_local_gather(gasnete_coll_my_images,
				  gasnete_coll_scale_ptr(args->dst, gasnete_coll_my_offset, args->nbytes),
				  &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, op->flags), args->nbytes);
	s = &(data->p2p->state[gasnete_coll_my_offset]);
	for (i = 0; i < gasnete_coll_my_images; ++i) {
	  *(s++) = 2;
        }
      }

    case 1:	/* Complete data movement */
      if (gasneti_mynode == args->dstnode) {
	gasnete_coll_p2p_t *p2p = data->p2p;
	volatile uint32_t *state;
	uintptr_t dst_addr, src_addr;
	size_t nbytes = args->nbytes;
	int i, done;

	gasneti_assert(p2p != NULL);
	gasneti_assert(p2p->state != NULL);
	state = data->p2p->state;
	gasneti_assert(p2p->data != NULL);

	done = 1;
	dst_addr = (uintptr_t)(args->dst);
	src_addr = (uintptr_t)(p2p->data);
	for (i = 0; i < gasnete_coll_total_images; ++i, dst_addr += nbytes, src_addr += nbytes) {
	  uint32_t s = state[i];

	  if (s == 0) {
	    /* Nothing received yet */
	    done = 0;
	  } else if (s == 1) {
	    /* Received but not yet copied into place */
	    gasneti_sync_reads();
	    GASNETE_FAST_UNALIGNED_MEMCPY((void *)dst_addr, (void *)src_addr, nbytes);
	    state[i] = 2;
	  }
	}

	if (!done) { break; }
      }
      data->state = 2;

    case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gathM_Eager(gasnet_team_handle_t team,
			 gasnet_image_t dstimage, void *dst,
			 void * const srclist[],
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(gasnete_coll_image_is_local(dstimage));

  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, flags,
					 &gasnete_coll_pf_gathM_Eager, options,
					 NULL, sequence GASNETE_THREAD_PASS);
}

/* gathM RVPut: root node broadcasts addresses, others put to that address (plus offset) */
/* Valid for SINGLE and LOCAL, any size */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_gathM_RVPut(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gatherM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gatherM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:
      if (gasneti_mynode == args->dstnode) {
	gasnete_coll_p2p_eager_addr_all(op, args->dst, 0, 1);	/* broadcast dst address */
	gasnete_coll_local_gather(gasnete_coll_my_images,
				  gasnete_coll_scale_ptr(args->dst, gasnete_coll_my_offset, args->nbytes),
				  &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, op->flags), args->nbytes);
      } else if (data->p2p->state[0]) {
	if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
	gasneti_sync_reads();
	data->private_data = gasnete_coll_scale_ptr(*(void **)data->p2p->data, gasnete_coll_my_offset, args->nbytes);
	data->handle = gasnete_puti(gasnete_synctype_nb, args->dstnode,
				    1, &(data->private_data), args->nbytes * gasnete_coll_my_images,
				    gasnete_coll_my_images,
				    &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, op->flags),
				    args->nbytes GASNETE_THREAD_PASS);
        gasnete_coll_save_handle(&data->handle GASNETE_THREAD_PASS);
      } else {
	break;
      }
      data->state = 2;

    case 2:
      if (data->handle != GASNET_INVALID_HANDLE) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gathM_RVPut(gasnet_team_handle_t team,
			 gasnet_image_t dstimage, void *dst,
			 void * const srclist[],
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(dstimage));

  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, flags,
					 &gasnete_coll_pf_gathM_RVPut, options,
					 NULL, sequence GASNETE_THREAD_PASS);
}

/* gathM RVous: non-root nodes use AM Mediums to send to addrs provided by root */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on all nodes */
static int gasnete_coll_pf_gathM_RVous(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gatherM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gatherM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Root send addrs */
      if (gasneti_mynode == args->dstnode) {
	void **tmp = gasneti_malloc(sizeof(void *) * gasnete_coll_total_images);
	gasnet_image_t j;
	gasnet_node_t i;
	for (j = 0; j < gasnete_coll_total_images; ++j) {
	  tmp[j] = gasnete_coll_scale_ptr(args->dst, j, args->nbytes);
	}
	for (i = 0; i < gasneti_nodes; ++i) {
	  if (i == gasneti_mynode) continue;
	  gasnete_coll_p2p_send_rtrM(op, data->p2p, 0, &GASNETE_COLL_1ST_IMAGE(tmp, i),
				     i, args->nbytes, gasnete_coll_all_images[i]);
	}
	gasneti_free(tmp);
	gasnete_coll_local_gather(gasnete_coll_my_images,
				  gasnete_coll_scale_ptr(args->dst, gasnete_coll_my_offset, args->nbytes),
				  &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, op->flags), args->nbytes);
      }
      data->state = 2;

    case 2:
      if (gasneti_mynode != args->dstnode) {
	/* non-root nodes send at most one AM per image each poll */
	void * const *p = &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, op->flags);
	int done = 1;
	gasnet_image_t i;
	for (i = 0; i < gasnete_coll_my_images; ++i) {
	  done &= gasnete_coll_p2p_send_data(op, data->p2p, args->dstnode, i, p[i], args->nbytes);
	}
	if (!done) {break;}
      } else if (!gasnete_coll_p2p_send_done(data->p2p)) {
	/* Not all data has arrived yet */
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gathM_RVous(gasnet_team_handle_t team,
			 gasnet_image_t dstimage, void *dst,
			 void * const srclist[],
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, flags,
					 &gasnete_coll_pf_gathM_RVous, options,
					 NULL, sequence GASNETE_THREAD_PASS);
}


extern gasnet_coll_handle_t
gasnete_coll_generic_gatherM_nb(gasnet_team_handle_t team,
				gasnet_image_t dstimage, void *dst,
				void * const srclist[],
				size_t nbytes, int flags,
				gasnete_coll_poll_fn poll_fn, int options,
				void *private_data, uint32_t sequence
                                GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t result;

  gasnete_coll_threads_lock(flags GASNETE_THREAD_PASS);

  #if GASNET_PAR
  if (flags & GASNETE_COLL_THREAD_LOCAL) {
    gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
    gasnete_coll_generic_data_t *data;
    if_pt (gasnete_coll_threads_first(GASNETE_THREAD_PASS_ALONE)) {
      data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
      GASNETE_COLL_GENERIC_SET_TAG(data, gatherM);
      data->threads.data = gasneti_calloc(gasnete_coll_my_images, sizeof(void *));
      data->args.gatherM.srclist    = (void**)data->threads.data;
      data->args.gatherM.dstimage   = dstimage;
      data->args.gatherM.dstnode    = gasnete_coll_image_node(dstimage);
      data->args.gatherM.dst        = dst; /* may get overwritten, but not worth a branch */
      data->args.gatherM.nbytes     = nbytes;
      data->options = options;
      data->private_data = private_data;
      result = gasnete_coll_op_generic_init(team, flags, data, poll_fn, sequence GASNETE_THREAD_PASS);
    } else {
      result = gasnete_coll_threads_get_handle_and_data(&data GASNETE_THREAD_PASS);
      if (td->my_image == dstimage) {
	gasneti_assert(dst != NULL);
        data->args.gatherM.dst = dst;
        gasneti_sync_writes();
      }
    }
    gasneti_assert(*srclist != NULL);
    data->args.gatherM.srclist[td->my_local_image] = *srclist; /* signalling write */
  } else
  #endif

  if_pt (gasnete_coll_threads_first(GASNETE_THREAD_PASS_ALONE)) {
    gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
    GASNETE_COLL_GENERIC_SET_TAG(data, gatherM);
    #if !GASNET_SEQ
      data->args.gatherM.dstimage = dstimage;
    #endif
    data->args.gatherM.dstnode    = gasnete_coll_image_node(dstimage);
    data->args.gatherM.dst        = dst;
    data->args.gatherM.srclist    = (void**)srclist;
    data->args.gatherM.nbytes     = nbytes;
    data->options = options;
    data->private_data = private_data;
    result = gasnete_coll_op_generic_init(team, flags, data, poll_fn, sequence GASNETE_THREAD_PASS);
  } else {
    result = gasnete_coll_threads_get_handle(GASNETE_THREAD_PASS_ALONE);
  }

  gasnete_coll_threads_unlock(GASNETE_THREAD_PASS_ALONE);
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_gatherM_nb_default(gasnet_team_handle_t team,
				gasnet_image_t dstimage, void *dst,
				void * const srclist[],
				size_t nbytes, int flags, uint32_t sequence
                                GASNETE_THREAD_FARG)
{
  const size_t eager_limit = GASNETE_COLL_P2P_EAGER_MIN;

  #if GASNET_SEQ
  /* Exactly one thread-local addr - forward to gath_nb() */
  if (flags & GASNET_COLL_LOCAL) {
    return gasnete_coll_gather_nb(team, dstimage, dst, srclist[0], nbytes,
				  flags, sequence GASNETE_THREAD_PASS);
  }
  #endif

  /* "Discover" in-segment flags if needed/possible */
  flags = gasnete_coll_segment_checkM(flags, 1, dstimage, dst, nbytes*gasneti_nodes,
				      0, 0, srclist, nbytes);

  /* Choose algorithm based on arguments */
  if ((flags & GASNET_COLL_DST_IN_SEGMENT) && (flags & GASNET_COLL_SRC_IN_SEGMENT)) {
    /* Both ends are in-segment */
    if ((flags & GASNET_COLL_IN_MYSYNC) || (flags & GASNET_COLL_LOCAL)) {
      if (nbytes <= eager_limit) {
        return gasnete_coll_gathM_Eager(team, dstimage, dst, srclist, nbytes, flags, sequence GASNETE_THREAD_PASS);
      } else {
        return gasnete_coll_gathM_RVPut(team, dstimage, dst, srclist, nbytes, flags, sequence GASNETE_THREAD_PASS);
      }
    } else if ((flags & GASNET_COLL_OUT_MYSYNC) && (nbytes <= eager_limit)) {
      return gasnete_coll_gathM_Eager(team, dstimage, dst, srclist, nbytes, flags, sequence GASNETE_THREAD_PASS);
    } else {
      return gasnete_coll_gathM_Put(team, dstimage, dst, srclist, nbytes, flags, sequence GASNETE_THREAD_PASS);
    }
  } else if (nbytes <= eager_limit) {
    /* Small enough for Eager, which works for out-of-segment src and/or dst */
    return gasnete_coll_gathM_Eager(team, dstimage, dst, srclist, nbytes, flags, sequence GASNETE_THREAD_PASS);
  } else if (flags & GASNET_COLL_DST_IN_SEGMENT) {
    /* Only the destination is in-segment (and too big for Eager) */
    if ((flags & GASNET_COLL_IN_NOSYNC) && (flags & GASNET_COLL_SINGLE)) {
      return gasnete_coll_gathM_Put(team, dstimage, dst, srclist, nbytes, flags, sequence GASNETE_THREAD_PASS);
    } else {
      return gasnete_coll_gathM_RVPut(team, dstimage, dst, srclist, nbytes, flags, sequence GASNETE_THREAD_PASS);
    }
  } else if (flags & GASNET_COLL_SRC_IN_SEGMENT) {
    /* Only the source is in-segment (and too big for Eager) */
    if (flags & GASNET_COLL_SINGLE) {
      return gasnete_coll_gathM_Get(team, dstimage, dst, srclist, nbytes, flags, sequence GASNETE_THREAD_PASS);
    } else {
      /* XXX: could do better since src is in-segment */
      return gasnete_coll_gathM_RVous(team, dstimage, dst, srclist, nbytes, flags, sequence GASNETE_THREAD_PASS);
    }
  } else {
    return gasnete_coll_gathM_RVous(team, dstimage, dst, srclist, nbytes, flags, sequence GASNETE_THREAD_PASS);
  }
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_all_nb() */

/* gall Gath: Implement gather_all in terms of simultaneous gathers */
/* This is meant mostly as an example and a short-term solution */
/* Valid wherever the underlying gather is valid */
static int gasnete_coll_pf_gall_Gath(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_all_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_all);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
      {
	gasnet_coll_handle_t *h;
        int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
	gasnet_team_handle_t team = op->team;
	void *dst = args->dst;
	void *src = args->src;
	size_t nbytes = args->nbytes;
        gasnet_image_t i;

	/* XXX: freelist ? */
	h = gasneti_malloc(gasnete_coll_total_images * sizeof(gasnet_coll_handle_t));
	data->private_data = h;

        for (i = 0; i < gasnete_coll_total_images; ++i, ++h) {
          *h = gasnete_coll_gather_nb(team, i, dst, src, nbytes,
				      flags, op->sequence+i+1 GASNETE_THREAD_PASS);
          gasnete_coll_save_coll_handle(h GASNETE_THREAD_PASS);
        }
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (!gasnete_coll_generic_coll_sync(data->private_data, gasnete_coll_total_images GASNETE_THREAD_PASS)) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasneti_free(data->private_data);
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gall_Gath(gasnet_team_handle_t team,
		       void *dst, void *src,
		       size_t nbytes, int flags, uint32_t sequence
		       GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));

  return gasnete_coll_generic_gather_all_nb(team, dst, src, nbytes, flags,
					    &gasnete_coll_pf_gall_Gath, options,
					    NULL, gasnete_coll_total_images GASNETE_THREAD_PASS);
}

extern gasnet_coll_handle_t
gasnete_coll_generic_gather_all_nb(gasnet_team_handle_t team,
				   void *dst, void *src,
				   size_t nbytes, int flags,
				   gasnete_coll_poll_fn poll_fn, int options,
				   void *private_data, uint32_t sequence
				   GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t result;

  gasnete_coll_threads_lock(flags GASNETE_THREAD_PASS);
  if_pt (gasnete_coll_threads_first(GASNETE_THREAD_PASS_ALONE)) {
    gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
    GASNETE_COLL_GENERIC_SET_TAG(data, gather_all);
    data->args.gather_all.dst     = dst;
    data->args.gather_all.src     = src;
    data->args.gather_all.nbytes  = nbytes;
    data->options = options;
    data->private_data = private_data;
    result = gasnete_coll_op_generic_init(team, flags, data, poll_fn, sequence GASNETE_THREAD_PASS);
  } else {
    result = gasnete_coll_threads_get_handle(GASNETE_THREAD_PASS_ALONE);
  }
  gasnete_coll_threads_unlock(GASNETE_THREAD_PASS_ALONE);
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_gather_all_nb_default(gasnet_team_handle_t team,
				   void *dst, void *src,
				   size_t nbytes, int flags, uint32_t sequence
				   GASNETE_THREAD_FARG) {
  #if GASNET_PAR
  /* Thread-local addr(s) - forward to gallM_nb() */
  if (flags & GASNET_COLL_LOCAL) {
    return gasnete_coll_gather_allM_nb(team, &dst, &src, nbytes,
				       flags | GASNETE_COLL_THREAD_LOCAL, sequence
				       GASNETE_THREAD_PASS);
  }
  #endif

  /* "Discover" in-segment flags if needed/possible */
  flags = gasnete_coll_segment_check(flags, 0, 0, dst, nbytes*gasneti_nodes,
				     0, 0, src, nbytes);

  /* XXX: need more implementations to choose from here */
  return gasnete_coll_gall_Gath(team, dst, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_allM_nb() */

/* gallM Gath: Implement gather_allM in terms of simultaneous gathers */
/* This is meant mostly as an example and a short-term solution */
/* Valid wherever the underlying gather is valid */
static int gasnete_coll_pf_gallM_Gath(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_allM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_allM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready2(op, args->dstlist, args->srclist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
      {
	gasnet_coll_handle_t *h;
        int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
	gasnet_team_handle_t team = op->team;
	void * const *srclist = args->srclist;
	size_t nbytes = args->nbytes;
	gasnet_image_t i;

	/* XXX: freelist ? */
	h = gasneti_malloc(gasnete_coll_total_images * sizeof(gasnet_coll_handle_t));
	data->private_data = h;

	if (op->flags & GASNET_COLL_SINGLE) {
	  void * const *p = args->dstlist;
          for (i = 0; i < gasnete_coll_total_images; ++i, ++h, ++p) {
            *h = gasnete_coll_gatherM_nb(team, i, *p, srclist, nbytes,
					 flags, op->sequence+i+1 GASNETE_THREAD_PASS);
            gasnete_coll_save_coll_handle(h GASNETE_THREAD_PASS);
          }
        } else {
	  void * const *p = &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist,GASNET_COLL_LOCAL);
          for (i = 0; i < gasnete_coll_total_images; ++i, ++h) {
            *h = gasnete_coll_gatherM_nb(team, i, *p, srclist, nbytes,
					 flags, op->sequence+i+1 GASNETE_THREAD_PASS);
            gasnete_coll_save_coll_handle(h GASNETE_THREAD_PASS);
	    if (gasnete_coll_image_is_local(i)) ++p;
          }
	}
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (!gasnete_coll_generic_coll_sync(data->private_data, gasnete_coll_total_images GASNETE_THREAD_PASS)) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasneti_free(data->private_data);
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gallM_Gath(gasnet_team_handle_t team,
			void * const dstlist[], void * const srclist[],
			size_t nbytes, int flags, uint32_t sequence
			GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));

  return gasnete_coll_generic_gather_allM_nb(team, dstlist, srclist, nbytes, flags,
					     &gasnete_coll_pf_gallM_Gath, options,
					     NULL, gasnete_coll_total_images GASNETE_THREAD_PASS);
}

extern gasnet_coll_handle_t
gasnete_coll_generic_gather_allM_nb(gasnet_team_handle_t team,
				    void * const dstlist[], void * const srclist[],
				    size_t nbytes, int flags,
				    gasnete_coll_poll_fn poll_fn, int options,
				    void *private_data, uint32_t sequence
				    GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t result;

  gasnete_coll_threads_lock(flags GASNETE_THREAD_PASS);

  #if GASNET_PAR
  if (flags & GASNETE_COLL_THREAD_LOCAL) {
    gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
    gasnete_coll_generic_data_t *data;
    if_pt (gasnete_coll_threads_first(GASNETE_THREAD_PASS_ALONE)) {
      data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
      GASNETE_COLL_GENERIC_SET_TAG(data, gather_allM);
      data->threads.data = gasneti_calloc(2 * gasnete_coll_my_images, sizeof(void *));
      data->args.gather_allM.srclist    = (void**)data->threads.data;
      data->args.gather_allM.dstlist    = (void**)data->threads.data + gasnete_coll_my_images;
      data->args.gather_allM.nbytes     = nbytes;
      data->options = options;
      data->private_data = private_data;
      result = gasnete_coll_op_generic_init(team, flags, data, poll_fn, sequence GASNETE_THREAD_PASS);
    } else {
      result = gasnete_coll_threads_get_handle_and_data(&data GASNETE_THREAD_PASS);
    }
    gasneti_assert(*srclist != NULL);
    data->args.gather_allM.srclist[td->my_local_image] = *srclist;
    gasneti_assert(*dstlist != NULL);
    data->args.gather_allM.dstlist[td->my_local_image] = *dstlist;
  } else
  #endif

  if_pt (gasnete_coll_threads_first(GASNETE_THREAD_PASS_ALONE)) {
    gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
    GASNETE_COLL_GENERIC_SET_TAG(data, gather_allM);
    data->args.gather_allM.dstlist = (void**)dstlist;
    data->args.gather_allM.srclist = (void**)srclist;
    data->args.gather_allM.nbytes  = nbytes;
    data->options = options;
    data->private_data = private_data;
    result = gasnete_coll_op_generic_init(team, flags, data, poll_fn, sequence GASNETE_THREAD_PASS);
  } else {
    result = gasnete_coll_threads_get_handle(GASNETE_THREAD_PASS_ALONE);
  }

  gasnete_coll_threads_unlock(GASNETE_THREAD_PASS_ALONE);
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_gather_allM_nb_default(gasnet_team_handle_t team,
				    void * const dstlist[], void * const srclist[],
				    size_t nbytes, int flags, uint32_t sequence
				    GASNETE_THREAD_FARG)
{
  #if GASNET_SEQ
  /* Exactly one thread-local addr per list - forward to gall_nb() */
  if (flags & GASNET_COLL_LOCAL) {
    return gasnete_coll_gather_all_nb(team, dstlist[0], srclist[0], nbytes,
				      flags, sequence GASNETE_THREAD_PASS);
  }
  #endif

  /* "Discover" in-segment flags if needed/possible */
  flags = gasnete_coll_segment_checkM(flags, 0, 0, dstlist, nbytes*gasneti_nodes,
    					 0, 0, srclist, nbytes);

  /* XXX: need more implementations to choose from here */
  return gasnete_coll_gallM_Gath(team, dstlist, srclist, nbytes, flags, sequence GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_exchange_nb() */

/* exchg Gath: Implement exchange in terms of simultaneous gathers */
/* This is meant mostly as an example and a short-term solution */
/* Valid wherever the underlying gather is valid */
static int gasnete_coll_pf_exchg_Gath(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_exchange_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, exchange);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
      {
	gasnet_coll_handle_t *h;
        int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
	gasnet_team_handle_t team = op->team;
	void *dst = args->dst;
	uintptr_t src_addr = (uintptr_t)args->src;
	size_t nbytes = args->nbytes;
        gasnet_image_t i;

	/* XXX: freelist ? */
	h = gasneti_malloc(gasnete_coll_total_images * sizeof(gasnet_coll_handle_t));
	data->private_data = h;

        for (i = 0; i < gasnete_coll_total_images; ++i, ++h, src_addr += nbytes) {
          *h = gasnete_coll_gather_nb(team, i, dst, (void *)src_addr, nbytes,
				      flags, op->sequence+i+1 GASNETE_THREAD_PASS);
          gasnete_coll_save_coll_handle(h GASNETE_THREAD_PASS);
        }
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (!gasnete_coll_generic_coll_sync(data->private_data, gasnete_coll_total_images GASNETE_THREAD_PASS)) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasneti_free(data->private_data);
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_exchg_Gath(gasnet_team_handle_t team,
			void *dst, void *src,
			size_t nbytes, int flags, uint32_t sequence
			GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));

  return gasnete_coll_generic_exchange_nb(team, dst, src, nbytes, flags,
					  &gasnete_coll_pf_exchg_Gath, options,
					  NULL, gasnete_coll_total_images GASNETE_THREAD_PASS);
}

extern gasnet_coll_handle_t
gasnete_coll_generic_exchange_nb(gasnet_team_handle_t team,
				 void *dst, void *src,
				 size_t nbytes, int flags,
				 gasnete_coll_poll_fn poll_fn, int options,
				 void *private_data, uint32_t sequence
				 GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t result;

  gasnete_coll_threads_lock(flags GASNETE_THREAD_PASS);
  if_pt (gasnete_coll_threads_first(GASNETE_THREAD_PASS_ALONE)) {
    gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
    GASNETE_COLL_GENERIC_SET_TAG(data, exchange);
    data->args.exchange.dst     = dst;
    data->args.exchange.src     = src;
    data->args.exchange.nbytes  = nbytes;
    data->options = options;
    data->private_data = private_data;
    result = gasnete_coll_op_generic_init(team, flags, data, poll_fn, sequence GASNETE_THREAD_PASS);
  } else {
    result = gasnete_coll_threads_get_handle(GASNETE_THREAD_PASS_ALONE);
  }
  gasnete_coll_threads_unlock(GASNETE_THREAD_PASS_ALONE);
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_exchange_nb_default(gasnet_team_handle_t team,
				 void *dst, void *src,
				 size_t nbytes, int flags, uint32_t sequence
				 GASNETE_THREAD_FARG)
{
  #if GASNET_PAR
  /* Thread-local addr(s) - forward to exchgM_nb() */
  if (flags & GASNET_COLL_LOCAL) {
    return gasnete_coll_exchangeM_nb(team, &dst, &src, nbytes,
				     flags | GASNETE_COLL_THREAD_LOCAL, sequence
				     GASNETE_THREAD_PASS);
  }
  #endif

  /* "Discover" in-segment flags if needed/possible */
  flags = gasnete_coll_segment_check(flags, 0, 0, dst, nbytes*gasneti_nodes,
				     0, 0, src, nbytes*gasneti_nodes);

  /* XXX: need more implementations to choose from here */
  return gasnete_coll_exchg_Gath(team, dst, src, nbytes, flags, sequence GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_exchangeM_nb() */

/* exchgM Gath: Implement exchangeM in terms of simultaneous gathers */
/* This is meant mostly as an example and a short-term solution */
/* Valid wherever the underlying gather is valid */
static int gasnete_coll_pf_exchgM_Gath(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_exchangeM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, exchangeM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready2(op, args->dstlist, args->srclist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Initiate data movement */
      if (!GASNETE_COLL_MAY_INIT_FOR(op)) break;
      {
	gasnet_coll_handle_t *h;
        int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
	gasnet_team_handle_t team = op->team;
	void **srclist;
	void **p;
	void * const *q;
	size_t nbytes = args->nbytes;
        gasnet_image_t i, j;

	if (op->flags & GASNET_COLL_SINGLE) {
	  data->private_data = gasneti_malloc(gasnete_coll_total_images * sizeof(gasnet_coll_handle_t) +
				       gasnete_coll_total_images * gasnete_coll_total_images * sizeof(void *));
	  h = (gasnet_coll_handle_t *)data->private_data;
	  srclist = gasnete_coll_scale_ptr(data->private_data, sizeof(gasnet_coll_handle_t), gasnete_coll_total_images);

	  /* XXX: A better design would not need N^2 temporary space */
	  p = srclist;
	  for (i = 0; i < gasnete_coll_total_images; ++i) {
	    q = args->srclist;
	    for (j = 0; j < gasnete_coll_total_images; ++j, ++p, ++q) {
	      *p = gasnete_coll_scale_ptr(*q, i, nbytes);
	    }
	  }

	  p = srclist;
	  q = args->dstlist;
          for (i = 0; i < gasnete_coll_total_images; ++i, ++h, ++q, p += gasnete_coll_total_images) {
            *h = gasnete_coll_gatherM_nb(team, i, *q, p, nbytes,
					 flags, op->sequence+i+1 GASNETE_THREAD_PASS);
	    gasnete_coll_save_coll_handle(h GASNETE_THREAD_PASS);
          }
	} else {
	  data->private_data = gasneti_malloc(gasnete_coll_total_images * sizeof(gasnet_coll_handle_t) +
				       gasnete_coll_total_images * gasnete_coll_my_images * sizeof(void *));
	  h = (gasnet_coll_handle_t *)data->private_data;
	  srclist = gasnete_coll_scale_ptr(data->private_data, sizeof(gasnet_coll_handle_t), gasnete_coll_total_images);

	  p = srclist;
	  for (i = 0; i < gasnete_coll_total_images; ++i) {
	    q = args->srclist;
	    for (j = 0; j < gasnete_coll_my_images; ++j, ++p, ++q) {
	      *p = gasnete_coll_scale_ptr(*q, i, nbytes);
	    }
	  }

	  p = srclist;
	  q = args->dstlist;
          for (i = 0; i < gasnete_coll_total_images; ++i, ++h, p += gasnete_coll_my_images) {
            *h = gasnete_coll_gatherM_nb(team, i, *q, p, nbytes,
					 flags, op->sequence+i+1 GASNETE_THREAD_PASS);
	    gasnete_coll_save_coll_handle(h GASNETE_THREAD_PASS);
	    if (gasnete_coll_image_is_local(i)) ++q;
          }
        }
      }
      data->state = 2;

    case 2:	/* Sync data movement */
      if (!gasnete_coll_generic_coll_sync(data->private_data, gasnete_coll_total_images GASNETE_THREAD_PASS)) {
	break;
      }
      data->state = 3;

    case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasneti_free(data->private_data);
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_exchgM_Gath(gasnet_team_handle_t team,
			 void * const dstlist[], void * const srclist[],
			 size_t nbytes, int flags, uint32_t sequence
			 GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));

  return gasnete_coll_generic_exchangeM_nb(team, dstlist, srclist, nbytes, flags,
					   &gasnete_coll_pf_exchgM_Gath, options,
					   NULL, gasnete_coll_total_images GASNETE_THREAD_PASS);
}

extern gasnet_coll_handle_t
gasnete_coll_generic_exchangeM_nb(gasnet_team_handle_t team,
				  void * const dstlist[], void * const srclist[],
				  size_t nbytes, int flags,
				  gasnete_coll_poll_fn poll_fn, int options,
				  void *private_data, uint32_t sequence
				  GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t result;

  gasnete_coll_threads_lock(flags GASNETE_THREAD_PASS);

  #if GASNET_PAR
  if (flags & GASNETE_COLL_THREAD_LOCAL) {
    gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
    gasnete_coll_generic_data_t *data;
    if_pt (gasnete_coll_threads_first(GASNETE_THREAD_PASS_ALONE)) {
      data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
      GASNETE_COLL_GENERIC_SET_TAG(data, exchangeM);
      data->threads.data = gasneti_calloc(2 * gasnete_coll_my_images, sizeof(void *));
      data->args.exchangeM.srclist    = (void**)data->threads.data;
      data->args.exchangeM.dstlist    = (void**)data->threads.data + gasnete_coll_my_images;
      data->args.exchangeM.nbytes     = nbytes;
      data->options = options;
      data->private_data = private_data;
      result = gasnete_coll_op_generic_init(team, flags, data, poll_fn, sequence GASNETE_THREAD_PASS);
    } else {
      result = gasnete_coll_threads_get_handle_and_data(&data GASNETE_THREAD_PASS);
    }
    gasneti_assert(*srclist != NULL);
    data->args.exchangeM.srclist[td->my_local_image] = *srclist;
    gasneti_assert(*dstlist != NULL);
    data->args.exchangeM.dstlist[td->my_local_image] = *dstlist;
  } else
  #endif

  if_pt (gasnete_coll_threads_first(GASNETE_THREAD_PASS_ALONE)) {
    gasnete_coll_generic_data_t *data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
    GASNETE_COLL_GENERIC_SET_TAG(data, exchangeM);
    data->args.exchangeM.dstlist = (void**)dstlist;
    data->args.exchangeM.srclist = (void**)srclist;
    data->args.exchangeM.nbytes  = nbytes;
    data->options = options;
    data->private_data = private_data;
    result = gasnete_coll_op_generic_init(team, flags, data, poll_fn, sequence GASNETE_THREAD_PASS);
  } else {
    result = gasnete_coll_threads_get_handle(GASNETE_THREAD_PASS_ALONE);
  }

  gasnete_coll_threads_unlock(GASNETE_THREAD_PASS_ALONE);
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_exchangeM_nb_default(gasnet_team_handle_t team,
				  void * const dstlist[], void * const srclist[],
				  size_t nbytes, int flags, uint32_t sequence
				  GASNETE_THREAD_FARG)
{
  #if GASNET_SEQ
  /* Exactly one thread-local addr per list - forward to exchg_nb() */
  if (flags & GASNET_COLL_LOCAL) {
    return gasnete_coll_exchange_nb(team, dstlist[0], srclist[0], nbytes,
				    flags, sequence GASNETE_THREAD_PASS);
  }
  #endif

  /* "Discover" in-segment flags if needed/possible */
  flags = gasnete_coll_segment_checkM(flags, 0, 0, dstlist, nbytes*gasneti_nodes,
				      0, 0, srclist, nbytes*gasneti_nodes);

  /* XXX: need more implementations to choose from here */
  return gasnete_coll_exchgM_Gath(team, dstlist, srclist, nbytes, flags, sequence GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_reduce_nb_default(gasnet_team_handle_t team,
			       gasnet_image_t dstimage, void *dst,
			       void *src, size_t src_blksz, size_t src_offset,
			       size_t elem_size, size_t elem_count,
			       gasnet_coll_fn_handle_t func, int func_arg,
			       int flags GASNETE_THREAD_FARG)
{
  gasneti_fatalerror("%s UNIMPLEMENTED", GASNETI_CURRENT_FUNCTION);
  return GASNET_COLL_INVALID_HANDLE;
}

/*---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_reduceM_nb_default(gasnet_team_handle_t team,
				gasnet_image_t dstimage, void *dst,
				void * const srclist[], size_t src_blksz, size_t src_offset,
				size_t elem_size, size_t elem_count,
				gasnet_coll_fn_handle_t func, int func_arg,
				int flags GASNETE_THREAD_FARG)
{
  gasneti_fatalerror("%s UNIMPLEMENTED", GASNETI_CURRENT_FUNCTION);
  return GASNET_COLL_INVALID_HANDLE;
}

/*---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_scan_nb_default(gasnet_team_handle_t team,
			     void *dst, size_t dst_blksz, size_t dst_offset,
			     void *src, size_t src_blksz, size_t src_offset,
			     size_t elem_size, size_t elem_count,
			     gasnet_coll_fn_handle_t func, int func_arg,
			     int flags GASNETE_THREAD_FARG)
{
  gasneti_fatalerror("%s UNIMPLEMENTED", GASNETI_CURRENT_FUNCTION);
  return GASNET_COLL_INVALID_HANDLE;
}

/*---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_scanM_nb_default(gasnet_team_handle_t team,
			      void * const dstlist[], size_t dst_blksz, size_t dst_offset,
			      void * const srclist[], size_t src_blksz, size_t src_offset,
			      size_t elem_size, size_t elem_count,
			      gasnet_coll_fn_handle_t func, int func_arg,
			      int flags GASNETE_THREAD_FARG)
{
  gasneti_fatalerror("%s UNIMPLEMENTED", GASNETI_CURRENT_FUNCTION);
  return GASNET_COLL_INVALID_HANDLE;
}

/*---------------------------------------------------------------------------------*/

#if GASNET_DEBUG
void gasnete_coll_stat_(GASNETE_THREAD_FARG_ALONE) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD_NOALLOC;
  int used = td->handles.used;
  gasnete_coll_op_t *op;
  fprintf(stderr, "%d:%d> %d handles used\n", (int)gasneti_mynode, (int)td->my_local_image, used);

  if (used) {
    gasnete_coll_local_handle_t *curr = gasnete_coll_local_handles(td, 0);
    int i;

    for (i = 0; i < used; ++i, ++curr) {
      uintptr_t addr = curr->addr;
      fprintf(stderr, "%sHANDLE %p\n", (addr&1)?"COLL_":"", (void*)(curr->u.handle));
    }
  }

  /* gasneti_mutex_lock(&gasnete_coll_active_lock); */
  op = gasnete_coll_active_first();
  while (op) {
    gasnete_coll_generic_data_t *data = op->data;
    fprintf(stderr, "OP: %p in state %d\n", op, data->state);
    op = gasnete_coll_active_next(op);
  }
  /* gasneti_mutex_unlock(&gasnete_coll_active_lock); */
}

extern void gasnete_coll_stat(void) { gasnete_coll_stat_(GASNETE_THREAD_GET_ALONE); }
#endif

