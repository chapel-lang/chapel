/*   $Source: /var/local/cvs/gasnet/shmem-conduit/gasnet_extended.c,v $
 *     $Date: 2007/01/03 17:12:32 $
 * $Revision: 1.23 $
 * Description: GASNet Extended API SHMEM Implementation
 * Copyright 2003, Christian Bell <csbell@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_handler.h>

int	    *gasnete_nbisync_cur = GASNETE_SYNC_NONE;
intptr_t     gasnete_segment_base = 0;

#ifdef CRAY_SHMEM
uintptr_t gasnete_pe_bits_shift = 0;
uintptr_t gasnete_addr_bits_mask = 0;
#endif

/* shmem-conduit cannot be used with threads. */
#ifdef GASNETI_CLIENT_THREADS
  #error shmem-conduit currently does not support threads
#endif

gasnete_threaddata_t	     gasnete_threaddata;
gasnete_threaddata_t  * const gasnete_threaddata_ptr = &gasnete_threaddata;
#undef gasnete_mythread
#define gasnete_mythread() (&gasnete_threaddata)

extern void gasnete_init() {
  int	    i;
  static int firstcall = 1;
  GASNETI_TRACE_PRINTF(C,("gasnete_init()"));
  gasneti_assert(firstcall); /*  make sure we haven't been called before */
  firstcall = 0;

    gasneti_assert(gasneti_nodes >= 1 && gasneti_mynode < gasneti_nodes);
    gasnete_segment_base = (intptr_t) gasneti_seginfo[gasneti_mynode].addr;

  /* Initialize barrier resources */
  gasnete_barrier_init();

  /* Initialize VIS subsystem */
  gasnete_vis_init();
}

#ifdef GASNETE_GLOBAL_ADDRESS
  /* tweaks required for bounds checking on clients who lie about node number*/
  extern int _gasneti_in_segment_bc(gasnet_node_t node, const void *ptr, size_t nbytes) {
    if (node != (gasnet_node_t)-1) return gasneti_in_segment(node,ptr,nbytes);
    else { /* check that it matches the segment of *some* node */
      gasnet_node_t i;
      for (i = 0; i < gasneti_nodes; i++) 
        if (gasneti_in_segment(i,ptr,nbytes)) return 1;
      return 0;
    }
  }
#endif

/* ------------------------------------------------------------------------ */
/*
  Non-blocking AM-based memory-to-memory transfers (implicit handle)
  ==========================================================
*/

/*
 * Non-blocking memsets are always completed as blocking operations, for
 * simplifying the code path in synchronizing messages
 * See comments in gasnet_extended_fwd.h
 */
extern gasnet_handle_t
gasnete_am_memset_nb(gasnet_node_t node, void *dest, int val, 
		     size_t nbytes GASNETE_THREAD_FARG) 
{
    int	 *ptr = GASNETE_SHMPTR_AM(dest,node);
    int	 isdone = 0;
    void *pdone = (void*)&isdone;

    GASNETI_SAFE(
	SHORT_REQ(4,6,(node, gasneti_handleridx(gasnete_memset_reqh),
		      (gasnet_handlerarg_t)val, (gasnet_handlerarg_t)nbytes, 
		      PACK(ptr), PACK(pdone))));

    /* Always blocking, even if an AM */
    GASNET_BLOCKUNTIL(isdone != 0);

    return GASNETE_SYNC_NONE;
}

GASNETI_INLINE(gasnete_memset_reqh_inner)
void 
gasnete_memset_reqh_inner(gasnet_token_t token, gasnet_handlerarg_t val, 
			  gasnet_handlerarg_t nbytes, void *dest, void *op) 
{
    memset(dest, (int)(uint32_t)val, nbytes);
    gasneti_sync_writes();

    GASNETI_SAFE(
	SHORT_REP(1,2,(token, gasneti_handleridx(gasnete_markdone_reph),
                  PACK(op))));
}
SHORT_HANDLER(gasnete_memset_reqh,4,6,
              (token, a0, a1, UNPACK(a2),      UNPACK(a3)     ),
              (token, a0, a1, UNPACK2(a2, a3), UNPACK2(a4, a5)));

GASNETI_INLINE(gasnete_markdone_reph_inner)
void 
gasnete_markdone_reph_inner(gasnet_token_t token, void *h) 
{
    int	*handle  = (int *) h;
    *handle = 1; /* Marks as done, requester spinning on handle != 0 */
    return;
}
SHORT_HANDLER(gasnete_markdone_reph,1,2,
              (token, UNPACK(a0)    ),
              (token, UNPACK2(a0, a1)));

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for implicit-handle non-blocking operations:
  ===========================================================
*/

/* ------------------------------------------------------------------------------------ */
/*
  Implicit access region synchronization
  ======================================
*/
/*  This implementation allows recursive access regions, although the spec does not require that */
/*  operations are associated with the most immediately enclosing access region */
extern void            
gasnete_begin_nbi_accessregion(int allowrecursion GASNETE_THREAD_FARG) 
{
    GASNETI_TRACE_PRINTF(S,("BEGIN_NBI_ACCESSREGION"));
    #if GASNET_DEBUG
    if (!allowrecursion && gasnete_nbisync_cur != GASNETE_SYNC_NONE)
	gasneti_fatalerror(
	    "VIOLATION: tried to initiate a recursive NBI access region");
    #endif
}

extern gasnet_handle_t 
gasnete_end_nbi_accessregion(GASNETE_THREAD_FARG_ALONE) 
{
    int	*hval;
    hval = gasnete_nbisync_cur;
    gasnete_nbisync_cur = GASNETE_SYNC_NONE;
    return hval;
}

/* ------------------------------------------------------------------------------------ */
/*
  Barriers:
  =========
*/

static void gasnete_shmembarrier_init();
static void gasnete_shmembarrier_notify(int id, int flags);
static int gasnete_shmembarrier_wait(int id, int flags);
static int gasnete_shmembarrier_try(int id, int flags);

#define GASNETE_BARRIER_DEFAULT "SHMEM"
#define GASNETE_BARRIER_INIT() do {                          \
    if (GASNETE_ISBARRIER("SHMEM")) {                        \
      gasnete_barrier_notify = &gasnete_shmembarrier_notify; \
      gasnete_barrier_wait =   &gasnete_shmembarrier_wait;   \
      gasnete_barrier_try =    &gasnete_shmembarrier_try;    \
      gasnete_shmembarrier_init();                           \
    }                                                        \
  } while (0)

/* allow reference implementation of barrier */
#define GASNETI_GASNET_EXTENDED_REFBARRIER_C 1
#include "gasnet_extended_refbarrier.c"
#undef GASNETI_GASNET_EXTENDED_REFBARRIER_C
/* ------------------------------------------------------------------------------------ */
/* SHMEM barrier */
/*
 * Atomic-inc/compare-and-swap based shmem barrier algorithm.
 *
 * Both Altix/X1 provide low-latecy atomic operations.  We use them here for
 * both named and anonymous barriers.  The algorithm works as follows.
 *
 * Notify (X1 and Altix):
 *   if value is client mismatche, broadcast a mismatch bit on each node
 *   if named barrier 
 *      cswap value located on node 0.
 *      if value mismatches, broadcast a mismatch bit on each node
 *  atomic inc counter on node 0.
 *
 * Wait (X1):
 *   check for local mismatches between local notify and local wait
 *   if node==0, spin-poll on atomic_inc counter for all node increments
 *               broadcast done bit to each node
 *   if node!=0, spin-poll on local counter until updated by 0.
 *      
 * Wait (Altix):
 *   check for local mismatches between local notify and local wait
 *   if node==0, spin-poll on atomic_inc counter for all node increments
 *               broadcast LOCAL done bit
 *   if node!=0, spin-poll on counter living at node 0.
 * 
 * On Altix, each node reads the counter living at node 0 (on an only cache
 * line) as cc-NUMA directory broadcasts outperform user-invoked serial
 * broadcast.
 *
 * On X1, performance-critical code paths replace the shmem_ptr shmem library
 * translation function with GASNet's inllined GASNETE_TRANSLATE_X1 macro.
 */

#define BARRIER_PAD_CACHELINE_SIZE 128

#ifdef SGI_SHMEM
#define BARRIER_READ_NOTIFYCTR	1
#define _BARRIER_PAD(name)  \
	static char __barrier_pad ## name[BARRIER_PAD_CACHELINE_SIZE] = { 0 }
#else
#define BARRIER_READ_NOTIFYCTR	0
#define _BARRIER_PAD(name)  \
	static char __barrier_pad ## name[BARRIER_PAD_CACHELINE_SIZE] = { 0 }
#endif

typedef struct {
    long volatile barrier_value;
    long volatile barrier_flags;
} gasnete_barrier_state_t;

#define BARRIER_INITVAL 0x1234567800000000

static long barrier_value[2] = { BARRIER_INITVAL, BARRIER_INITVAL };
static int  barrier_mismatch[2] = { 0, 0 };
static int  barrier_phase = 0;

/* On Altix, ensure value, done and notify_ctr live on different cache lines */
_BARRIER_PAD(n0);
static long volatile		    barrier_done[2] = { 0, 0 };
_BARRIER_PAD(n1);
static long volatile		    barrier_notify_ctr[2] = { 0, 0 };
static gasnete_barrier_state_t	    barrier_state[2];

static void gasnete_shmembarrier_init() {
  /* nothing to do.. */
}

static void gasnete_barrier_broadcastmismatch() {
  int i;
  for (i=0; i < gasneti_nodes; i++) 
    *((int *)shmem_ptr(&barrier_mismatch[barrier_phase], i)) = 1;
  shmem_quiet();
  gasneti_local_wmb();
}

static void gasnete_shmembarrier_notify(int id, int flags) {
    int i;
    long curval;
    if_pf (barrier_splitstate == INSIDE_BARRIER)
	gasneti_fatalerror("gasnet_barrier_notify() called twice in a row");

    barrier_phase = !barrier_phase;
    barrier_state[barrier_phase].barrier_value = id;
    barrier_state[barrier_phase].barrier_flags = flags;

    /*
     * Client-initiated mismatch -- broadcast to the mismatch flag.  Operation
     * is in a failure, non-optimized code path.
     */
    if (flags & GASNET_BARRIERFLAG_MISMATCH) gasnete_barrier_broadcastmismatch();
    else if (!(flags & GASNET_BARRIERFLAG_ANONYMOUS)) {
	#if PLATFORM_ARCH_CRAYX1
	    curval = _amo_acswap(
		    GASNETE_TRANSLATE_X1(&barrier_value[barrier_phase], 0), 
		    BARRIER_INITVAL, (long) id);
	#else
	    curval = shmem_long_cswap(&barrier_value[barrier_phase], 
				      BARRIER_INITVAL, (long) id, 0);
	#endif
	/*
	 * Value mismatch -- broadcast to the mismatch flag. Operation is in a
	 * failure, non-optimized path.
	 */
	if_pf (curval != BARRIER_INITVAL && curval != id) gasnete_barrier_broadcastmismatch();
    }
	
    /* Atomic increment at node 0 */
    #if PLATFORM_ARCH_CRAYX1
	_amo_aadd(GASNETE_TRANSLATE_X1(&barrier_notify_ctr[barrier_phase], 0), 
		  1);
    #else
	shmem_long_finc((long*)&barrier_notify_ctr[barrier_phase], 0);
    #endif

    barrier_splitstate = INSIDE_BARRIER;
    gasneti_sync_writes();
}

static int gasnete_shmembarrier_wait(int id, int flags) {
    int  i, local_mismatch = 0;
    long volatile *done_ctr = &barrier_done[barrier_phase];

    gasneti_sync_reads();

    if_pf(barrier_splitstate == OUTSIDE_BARRIER) 
	gasneti_fatalerror(
	    "gasnet_barrier_wait() called without a matching notify");

    barrier_splitstate = OUTSIDE_BARRIER;
    gasneti_sync_writes();

    /*
     * First check for local (non-collective) mismatches
     */
    if_pf (flags & GASNET_BARRIERFLAG_MISMATCH ||
	   flags != barrier_state[barrier_phase].barrier_flags ||
	   (!(flags & GASNET_BARRIERFLAG_ANONYMOUS) &&
	     id != barrier_state[barrier_phase].barrier_value)) {
	local_mismatch = 1;
    }

    if (gasneti_mynode == 0) {
	long volatile *not_ctr = &barrier_notify_ctr[barrier_phase];

	/* Wait until all nodes have updated value */
	GASNET_BLOCKUNTIL(*not_ctr == gasneti_nodes);
	*not_ctr = 0;

	/*
	 * Broadcast the return value for global mismatches
	 */
	#if BARRIER_READ_NOTIFYCTR
	    /* This is the only safe point to reset the barrier done flag of
	     * the barrier on "the other" phase. */
	    barrier_done[!barrier_phase] = 0;
	    *done_ctr = 1;
	#else
	    /*GASNETC_VECTORIZE*/
	    for (i=0; i < gasneti_nodes; i++) 
		#if PLATFORM_ARCH_CRAYX1
		    *((long *) GASNETE_TRANSLATE_X1(done_ctr, i)) = 1;
		#else
		    shmem_long_p((long *)done_ctr, 1, i);
		#endif
	#endif

	barrier_value[barrier_phase] = BARRIER_INITVAL;
    }
    else {
	#if BARRIER_READ_NOTIFYCTR
	    #if PLATFORM_ARCH_CRAYX1
		done_ctr = GASNETE_TRANSLATE_X1((void *)done_ctr, 0);
	    #else
		done_ctr = shmem_ptr((void *)done_ctr, 0);
	    #endif
	    /* Wait for notification from node 0 */
	    GASNET_BLOCKUNTIL(*done_ctr != 0);
	    /* Don't reset counter, it lives on zero! */
	#else
	    /* Spin on a local counter and reset it is set by node 0 */
	    GASNET_BLOCKUNTIL(*done_ctr != 0);
	    *done_ctr = 0;
	#endif

    }

    gasneti_sync_writes();

    if (local_mismatch || barrier_mismatch[barrier_phase]) {
	barrier_mismatch[barrier_phase] = 0;
	return GASNET_ERR_BARRIER_MISMATCH;
    }
    else
	return GASNET_OK;
}

static int gasnete_shmembarrier_try(int id, int flags) {
    if_pf(barrier_splitstate == OUTSIDE_BARRIER)
	gasneti_fatalerror("gasnet_barrier_try() called without a matching notify");
    return gasnete_shmembarrier_wait(id, flags);
}

/* ------------------------------------------------------------------------ */
/*
  Vector, Indexed & Strided:
  =========================
*/

/* use reference implementation of scatter/gather and strided */
#include "gasnet_extended_refvis.h"

/* ------------------------------------------------------------------------------------ */
/*
  Collectives:
  ============
*/

/* use reference implementation of collectives */
#include "gasnet_extended_refcoll.h"

/* ------------------------------------------------------------------------ */
/*
  Handlers:
  =========
*/
static gasnet_handlerentry_t const 
gasnete_handlers[] = {
    #ifdef GASNETE_REFBARRIER_HANDLERS
      GASNETE_REFBARRIER_HANDLERS(),
    #endif

    #ifdef GASNETE_REFVIS_HANDLERS
      GASNETE_REFVIS_HANDLERS()
    #endif

    #ifdef GASNETE_REFCOLL_HANDLERS
      GASNETE_REFCOLL_HANDLERS()
    #endif

    /* ptr-width independent handlers */

    /* ptr-width dependent handlers */
    gasneti_handler_tableentry_with_bits(gasnete_memset_reqh),
    gasneti_handler_tableentry_with_bits(gasnete_markdone_reph),
  { 0, NULL }
};

extern gasnet_handlerentry_t const *
gasnete_get_handlertable() {
  return gasnete_handlers;
}

/* ------------------------------------------------------------------------ */
