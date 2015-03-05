/*   $Source: bitbucket.org:berkeleylab/gasnet.git/shmem-conduit/gasnet_extended.c $
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

/* ------------------------------------------------------------------------------------ */
/*
  Extended API Common Code
  ========================
  Factored bits of extended API code common to most conduits, overridable when necessary
*/

#define GASNETE_THREADING_CUSTOM 1
#define GASNETE_VALGET_CUSTOM 1

#include "gasnet_extended_common.c"

extern void gasnete_register_threadcleanup(void (*cleanupfn)(void *), void *context) { }
extern uint64_t gasneti_max_threads(void) { return 1; }

/* ------------------------------------------------------------------------------------ */


extern void gasnete_init(void) {
  GASNETI_UNUSED_UNLESS_DEBUG
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
#ifdef GASNETE_GLOBAL_ADDRESS
    int	 *ptr = GASNETE_SHMPTR_AM(dest,node);
    int	 isdone = 0;
    void *pdone = (void*)&isdone;

    GASNETI_SAFE(
	SHORT_REQ(4,7,(node, gasneti_handleridx(gasnete_memset_reqh),
		      (gasnet_handlerarg_t)val, PACK(nbytes), 
		      PACK(ptr), PACK(pdone))));

    /* Always blocking, even if an AM */
    GASNET_BLOCKUNTIL(isdone != 0);
#else
    /* TODO: keep this dynamic array */

    char *tmp = gasneti_malloc(nbytes);
    memset(tmp, val, nbytes);

    shmem_putmem(dest, tmp, nbytes, node);

    gasneti_free(tmp);
#endif

    return GASNETE_SYNC_NONE;
}

GASNETI_INLINE(gasnete_memset_reqh_inner)
void 
gasnete_memset_reqh_inner(gasnet_token_t token, gasnet_handlerarg_t val, 
			  void *nbytes_arg, void *dest, void *op) 
{
    size_t nbytes = (uintptr_t)nbytes_arg;
    memset(dest, (int)(uint32_t)val, nbytes);
    gasneti_sync_writes();

    GASNETI_SAFE(
	SHORT_REP(1,2,(token, gasneti_handleridx(gasnete_markdone_reph),
                  PACK(op))));
}
SHORT_HANDLER(gasnete_memset_reqh,4,7,
              (token, a0, UNPACK(a1),      UNPACK(a2),      UNPACK(a3)     ),
              (token, a0, UNPACK2(a1, a2), UNPACK2(a3, a4), UNPACK2(a5, a6)));

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

/* RDMADISSEM is faster and more scalable than our current shmem-specific barrier */
#define GASNETE_BARRIER_DEFAULT "RDMADISSEM"

/* Our SHMEM barrier uses a 64-bit atomic operation to allow the user's
 * 32-bit barrier value to be distinguished from a "uninitialized" marker.
 * This code requires shmem_long_finc() and an 8-byte shmem_*_cswap().
 */
#if defined(GASNETE_USE_SHMEM_BARRIER)
  /* Keep the current value */
#elif PLATFORM_ARCH_CRAYX1 || GASNETI_ARCH_ALTIX || \
     (HAVE_SHMEM_LONG_FINC && \
      (((SIZEOF_LONG == 8) && HAVE_SHMEM_LONG_CSWAP) || \
       ((SIZEOF_LONG_LONG == 8) && HAVE_SHMEM_LONGLONG_CSWAP)))
  #define GASNETE_USE_SHMEM_BARRIER 1
#endif

#if GASNETE_USE_SHMEM_BARRIER
static void gasnete_shmembarrier_init(gasnete_coll_team_t team);

#define GASNETE_BARRIER_READENV() do { \
  if(GASNETE_ISBARRIER("SHMEM")) gasnete_coll_default_barrier_type = GASNETE_COLL_BARRIER_SHMEM; \
} while (0)

#define GASNETE_BARRIER_INIT(TEAM, BARRIER_TYPE) do {        \
    if ((BARRIER_TYPE) == GASNETE_COLL_BARRIER_SHMEM && (TEAM)==GASNET_TEAM_ALL) { \
      gasnete_shmembarrier_init(TEAM);                       \
    }                                                        \
  } while (0)
#endif /* GASNETE_USE_SHMEM_BARRIER */

/* allow reference implementation of barrier */
#define GASNETI_GASNET_EXTENDED_REFBARRIER_C 1
#include "gasnet_extended_refbarrier.c"
#undef GASNETI_GASNET_EXTENDED_REFBARRIER_C

#if GASNETE_USE_SHMEM_BARRIER
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
 * translation function with GASNet's inlined GASNETE_TRANSLATE_X1 macro.
 *
 * The SGI Origin (IRIX) support is so far using the same code as Altix.
 * However, it has not been tuned and is not ported to ILP32.
 */

#define BARRIER_PAD_CACHELINE_SIZE 128

#ifdef SGI_SHMEM
#define BARRIER_READ_NOTIFYCTR	1
#define _BARRIER_PAD(name)  \
	GASNETI_UNUSED static char __barrier_pad ## name[BARRIER_PAD_CACHELINE_SIZE] = { 0 }
#else
#define BARRIER_READ_NOTIFYCTR	0
#define _BARRIER_PAD(name)  \
	GASNETI_UNUSED static char __barrier_pad ## name[BARRIER_PAD_CACHELINE_SIZE] = { 0 }
#endif

typedef struct {
    int barrier_value;
    int barrier_flags;
} gasnete_barrier_state_t;

#define BARRIER_INITVAL 0x1234567800000000LL
#define BARRIER_ANONVAL 0x9abcdef000000000LL

static uint64_t barrier_value[2] = { BARRIER_INITVAL, BARRIER_INITVAL };
static int  barrier_mismatch[2] = { 0, 0 };
static int  barrier_phase = 0;
static uint64_t barrier_result = BARRIER_INITVAL;

/* On Altix, ensure value, done and notify_ctr live on different cache lines */
_BARRIER_PAD(n0);
static uint64_t volatile            barrier_final[2] = { BARRIER_INITVAL, BARRIER_INITVAL };
_BARRIER_PAD(n1);
static long volatile		    barrier_notify_ctr[2] = { 0, 0 };
static gasnete_barrier_state_t	    barrier_state[2];

GASNETI_NEVER_INLINE(gasnete_barrier_broadcastmismatch,
static void gasnete_barrier_broadcastmismatch(void)) {
  int i;
  for (i=0; i < gasneti_nodes; i++) 
#ifdef GASNETE_GLOBAL_ADDRESS
    *((int *)shmem_ptr(&barrier_mismatch[barrier_phase], i)) = 1;
#else
    shmem_int_p(&barrier_mismatch[barrier_phase], 1, i);
#endif
  shmem_quiet();
  gasneti_local_wmb();
}

#if !PLATFORM_ARCH_CRAYX1
  /* These are sometimes missing from shmem.h */
  #if HAVE_SHMEM_LONG_FINC
    extern long shmem_long_finc(long *addr, int pe);
  #endif
  #if HAVE_SHMEM_LONG_CSWAP
    extern long shmem_long_cswap(long *target, long cond, long value, int pe);
  #endif
  #if HAVE_SHMEM_LONGLONG_CSWAP
    extern long long shmem_longlong_cswap(long long *target, long long cond, long long value, int pe);
  #endif
#endif

static void gasnete_shmembarrier_notify(gasnete_coll_team_t team, int id, int flags) {
    uint64_t curval;
    GASNETE_SPLITSTATE_NOTIFY_ENTER(team);

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
	#elif (SIZEOF_LONG == 8)
	    curval = shmem_long_cswap((long *)&barrier_value[barrier_phase], 
				      BARRIER_INITVAL, (long) id, 0);
	#elif (SIZEOF_LONG_LONG == 8)
	    curval = shmem_longlong_cswap((long long *)&barrier_value[barrier_phase], 
					  BARRIER_INITVAL, (long long) id, 0);
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

    gasneti_sync_writes();
}

static int gasnete_shmembarrier_wait(gasnete_coll_team_t team, int id, int flags) {
    #if !BARRIER_READ_NOTIFYCTR
    int i;
    #endif
    uint64_t volatile *final_val = &barrier_final[barrier_phase];
    uint64_t consensus;

    gasneti_sync_reads();

    GASNETE_SPLITSTATE_WAIT_LEAVE(team);

    gasneti_sync_writes();

    if (gasneti_mynode == 0) {
	long volatile *not_ctr = &barrier_notify_ctr[barrier_phase];

	/* Wait until all nodes have updated value */
	GASNET_BLOCKUNTIL(*not_ctr == gasneti_nodes);
	*not_ctr = 0;

        /* Determine what to publish as final value */
        consensus = barrier_value[barrier_phase];
        if_pf (consensus == BARRIER_INITVAL) consensus = BARRIER_ANONVAL;

	/*
	 * Broadcast the return value for global mismatches
	 */
	#if BARRIER_READ_NOTIFYCTR
	    /* This is the only safe point to reset the barrier done flag of
	     * the barrier on "the other" phase. */
	    barrier_final[!barrier_phase] = BARRIER_INITVAL;
	    *final_val = consensus;
	#else
	    /*GASNETC_VECTORIZE*/
	    for (i=0; i < gasneti_nodes; i++) 
		#if PLATFORM_ARCH_CRAYX1
		    *((uint64_t *) GASNETE_TRANSLATE_X1(final_val, i)) = consensus;
		#elif (SIZEOF_LONG == 8)
		    shmem_long_p((long *)final_val, consensus, i);
		#elif (SIZEOF_LONG_LONG == 8)
		    shmem_longlong_p((long *)final_val, consensus, i);
		#endif
	#endif

	barrier_value[barrier_phase] = BARRIER_INITVAL;
    }
    else {
	#if BARRIER_READ_NOTIFYCTR
	    #if PLATFORM_ARCH_CRAYX1
		final_val = GASNETE_TRANSLATE_X1((void *)final_val, 0);
	    #else
		final_val = shmem_ptr((void *)final_val, 0);
	    #endif
	    /* Wait for notification from node 0 */
	    GASNET_BLOCKUNTIL(BARRIER_INITVAL != (consensus = *final_val));
	    /* Don't reset counter, it lives on zero! */
	#else
	    /* Spin on a local counter and reset it is set by node 0 */
	    GASNET_BLOCKUNTIL(BARRIER_INITVAL != (consensus = *final_val));
	    *final_val = BARRIER_INITVAL;
	#endif

    }

    barrier_result = consensus;
    gasneti_sync_writes();

    if (barrier_mismatch[barrier_phase]) {
	barrier_mismatch[barrier_phase] = 0;
	return GASNET_ERR_BARRIER_MISMATCH;
    }
    else if_pf(!(flags & GASNET_BARRIERFLAG_ANONYMOUS) &&
               (consensus != BARRIER_ANONVAL) &&
               (id != GASNETI_LOWORD(consensus))) {
	return GASNET_ERR_BARRIER_MISMATCH;
    }
    else
	return GASNET_OK;
}

static int gasnete_shmembarrier_try(gasnete_coll_team_t team, int id, int flags) {
    GASNETE_SPLITSTATE_TRY(team);
    return gasnete_shmembarrier_wait(team, id, flags);
}

static int gasnete_shmembarrier_result(gasnete_coll_team_t team, int *id) {
   GASNETE_SPLITSTATE_RESULT(team);

   *id = GASNETI_LOWORD(barrier_result);
   return (barrier_result == BARRIER_ANONVAL);
}

static void gasnete_shmembarrier_init(gasnete_coll_team_t team) {
  team->barrier_notify = &gasnete_shmembarrier_notify;
  team->barrier_wait =   &gasnete_shmembarrier_wait;
  team->barrier_try =    &gasnete_shmembarrier_try;
  team->barrier_result = &gasnete_shmembarrier_result;
  team->barrier_pf =     NULL;
  /* nothing else to do.. */
}

#endif /* GASNETE_USE_SHMEM_BARRIER */

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
gasnete_get_handlertable(void) {
  return gasnete_handlers;
}

/* ------------------------------------------------------------------------ */
