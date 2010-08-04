/*   $Source: /var/local/cvs/gasnet/gm-conduit/gasnet_extended.c,v $
 *     $Date: 2010/04/17 03:14:25 $
 * $Revision: 1.45.4.2 $
 * Description: GASNet Extended API GM Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_handler.h>

const gasnete_eopaddr_t	EOPADDR_NIL = { { 0xFF, 0xFF } };

/* ------------------------------------------------------------------------------------ */
/*
  Tuning Parameters
  =================
  Conduits may choose to override the default tuning parameters below by
  defining them in their gasnet_core_fwd.h
*/

/* the size threshold where gets/puts stop using medium messages and start
 * using longs */
#ifndef GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD
#define GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD   gasnet_AMMaxMedium()
#endif

/* true if we should try to use Long replies in gets (only possible if dest
 * falls in segment) */
#ifndef GASNETE_USE_LONG_GETS
#define GASNETE_USE_LONG_GETS 1
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Extended API Common Code
  ========================
  Factored bits of extended API code common to most conduits, overridable when necessary
*/

#include "gasnet_extended_common.c"

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* called at startup to check configuration sanity */
static void gasnete_check_config(void) {
  gasneti_check_config_postattach();

  gasneti_assert_always(GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD <= gasnet_AMMaxMedium());
  gasneti_assert_always(gasnete_eopaddr_isnil(EOPADDR_NIL));

#if 0 /* No AM-based Gets in gm-conduit */
  /* The next two ensure nbytes in AM-based Gets will fit in handler_arg_t (bug 2770) */
  gasneti_assert_always(gasnet_AMMaxMedium() <= (size_t)0xffffffff);
  gasneti_assert_always(gasnet_AMMaxLongReply() <= (size_t)0xffffffff);
#endif
}

extern void gasnete_init(void) {
    static int firstcall = 1;
    GASNETI_TRACE_PRINTF(C,("gasnete_init()"));
    gasneti_assert(firstcall); /*  make sure we haven't been called before */
    firstcall = 0;

	gasnete_check_config(); /* check for sanity */

	gasneti_assert(gasneti_nodes >= 1 && gasneti_mynode < gasneti_nodes);

	{ 
		gasnete_threaddata_t *threaddata = NULL;
		gasnete_eop_t *eop = NULL;
		#if GASNETI_CLIENT_THREADS
			/* register first thread (optimization) */
			threaddata = gasnete_mythread(); 
		#else
			/* register only thread (required) */
			threaddata = gasnete_new_threaddata();
		#endif

		/* cause the first pool of eops to be allocated optimization */
		eop = gasnete_eop_new(threaddata);
		gasnete_op_markdone((gasnete_op_t *)eop, 0);
		gasnete_op_free((gasnete_op_t *)eop);
	}
 
  /* Initialize barrier resources */
  gasnete_barrier_init();

  /* Initialize VIS subsystem */
  gasnete_vis_init();
}
/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for explicit-handle non-blocking operations:
  ===========================================================
*/

extern int  gasnete_try_syncnb(gasnet_handle_t handle) {
	GASNETI_SAFE(gasneti_AMPoll());

	if (gasnete_op_isdone(handle)) {
		gasneti_sync_reads();
		gasnete_op_free(handle);
		return GASNET_OK;
	}
	else return GASNET_ERR_NOT_READY;
}

extern int
gasnete_try_syncnb_some (gasnet_handle_t *phandle, size_t numhandles)
{
	int success = 0;
	int empty = 1;

	GASNETI_SAFE(gasneti_AMPoll());
	gasneti_assert(phandle);

	{ 
		int i;
		for (i = 0; i < numhandles; i++) {
			gasnete_op_t *op = phandle[i];
			if (op != GASNET_INVALID_HANDLE) {
				empty = 0;
				if (gasnete_op_isdone(op)) {
					gasneti_sync_reads();
					gasnete_op_free(op);
					phandle[i] = GASNET_INVALID_HANDLE;
					success = 1;
				}
			}
		}
	}
	if (success || empty) 
		return GASNET_OK;
	else 
		return GASNET_ERR_NOT_READY;
}

extern int  
gasnete_try_syncnb_all (gasnet_handle_t *phandle, size_t numhandles)
{
	int success = 1;
	GASNETI_SAFE(gasneti_AMPoll());

	gasneti_assert(phandle);

	{ 
		int i;
		for (i = 0; i < numhandles; i++) {
			gasnete_op_t *op = phandle[i];
			if (op != GASNET_INVALID_HANDLE) {
				if (gasnete_op_isdone(op)) {
					gasneti_sync_reads();
					gasnete_op_free(op);
					phandle[i] = GASNET_INVALID_HANDLE;
				} 
				else 
					success = 0;
			}
		}
	}
	if (success) 
		return GASNET_OK;
	else 
		return GASNET_ERR_NOT_READY;
}

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for implicit-handle non-blocking operations:
  ===========================================================
*/

extern int  gasnete_try_syncnbi_gets(GASNETE_THREAD_FARG_ALONE) {
  #if 0
    /* polling for syncnbi now happens in header file to avoid duplication */
    GASNETI_SAFE(gasneti_AMPoll());
  #endif
  {
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t *iop = mythread->current_iop;
    gasneti_assert(iop->threadidx == mythread->threadidx);
    gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);
    #if GASNET_DEBUG
      if (iop->next != NULL)
        gasneti_fatalerror("VIOLATION: attempted to call gasnete_try_syncnbi_gets() inside an NBI access region");
    #endif

    if (gasneti_weakatomic_read(&(iop->completed_get_cnt),0) == iop->initiated_get_cnt) {
      if_pf (iop->initiated_get_cnt > 65000) { /* make sure we don't overflow the counters */
        gasneti_weakatomic_set(&(iop->completed_get_cnt), 0, 0);
        iop->initiated_get_cnt = 0;
      }
      gasneti_sync_reads();
      return GASNET_OK;
    } else return GASNET_ERR_NOT_READY;
  }
}

extern int  gasnete_try_syncnbi_puts(GASNETE_THREAD_FARG_ALONE) {
  #if 0
    /* polling for syncnbi now happens in header file to avoid duplication */
    GASNETI_SAFE(gasneti_AMPoll());
  #endif
  {
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t *iop = mythread->current_iop;
    gasneti_assert(iop->threadidx == mythread->threadidx);
    gasneti_assert(iop->next == NULL);
    gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);
    #if GASNET_DEBUG
      if (iop->next != NULL)
        gasneti_fatalerror("VIOLATION: attempted to call gasnete_try_syncnbi_puts() inside an NBI access region");
    #endif


    if (gasneti_weakatomic_read(&(iop->completed_put_cnt),0) == iop->initiated_put_cnt) {
      if_pf (iop->initiated_put_cnt > 65000) { /* make sure we don't overflow the counters */
        gasneti_weakatomic_set(&(iop->completed_put_cnt), 0, 0);
        iop->initiated_put_cnt = 0;
      }
      gasneti_sync_reads();
      return GASNET_OK;
    } else return GASNET_ERR_NOT_READY;
  }
}

/* ------------------------------------------------------------------------------------ */
/*
  Implicit access region synchronization
  ======================================
*/
/*  This implementation allows recursive access regions, although the spec does not require that */
/*  operations are associated with the most immediately enclosing access region */
extern void            gasnete_begin_nbi_accessregion(int allowrecursion GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t *iop = gasnete_iop_new(mythread); /*  push an iop  */
  GASNETI_TRACE_PRINTF(S,("BEGIN_NBI_ACCESSREGION"));
  #if GASNET_DEBUG
    if (!allowrecursion && mythread->current_iop->next != NULL)
      gasneti_fatalerror("VIOLATION: tried to initiate a recursive NBI access region");
  #endif
  iop->next = mythread->current_iop;
  mythread->current_iop = iop;
}

extern gasnet_handle_t gasnete_end_nbi_accessregion(GASNETE_THREAD_FARG_ALONE) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t *iop = mythread->current_iop; /*  pop an iop */
  GASNETI_TRACE_EVENT_VAL(S,END_NBI_ACCESSREGION,iop->initiated_get_cnt + iop->initiated_put_cnt);
  #if GASNET_DEBUG
    if (iop->next == NULL)
      gasneti_fatalerror("VIOLATION: call to gasnete_end_nbi_accessregion() outside access region");
  #endif
  mythread->current_iop = iop->next;
  iop->next = NULL;
  return (gasnet_handle_t)iop;
}

/* ------------------------------------------------------------------------------------ */

extern gasnet_handle_t 
gasnete_memset_nb(gasnet_node_t node, void *dest, int val, 
		  size_t nbytes   GASNETE_THREAD_FARG) {
	GASNETI_CHECKPSHM_MEMSET(H);
	return gasnete_extref_memset_nb(node, dest, val, 
	    nbytes GASNETE_THREAD_PASS);
}

extern void
gasnete_memset_nbi(gasnet_node_t node, void *dest, int val, 
		  size_t nbytes   GASNETE_THREAD_FARG) {
	GASNETI_CHECKPSHM_MEMSET(V);
	gasnete_extref_memset_nbi(node, dest, val, 
	    nbytes GASNETE_THREAD_PASS);
	return;
}
