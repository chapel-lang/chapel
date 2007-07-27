/*   $Source$
 *     $Date$
 * $Revision$
 * Description: GASNet Extended API Reference Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_handler.h>

GASNETI_IDENT(gasnete_IdentString_Version, "$GASNetExtendedLibraryVersion: " GASNET_EXTENDED_VERSION_STR " $");
GASNETI_IDENT(gasnete_IdentString_ExtendedName, "$GASNetExtendedLibraryName: " GASNET_EXTENDED_NAME_STR " $");

gasnete_threaddata_t *gasnete_threadtable[GASNETI_MAX_THREADS] = { 0 };
static int gasnete_numthreads = 0;
static gasnet_hsl_t threadtable_lock = GASNET_HSL_INITIALIZER;
#if GASNETI_CLIENT_THREADS
  /* pthread thread-specific ptr to our threaddata (or NULL for a thread never-seen before) */
  GASNETI_THREADKEY_DEFINE(gasnete_threaddata);
#endif
static const gasnete_eopaddr_t EOPADDR_NIL = { { 0xFF, 0xFF } };
extern void _gasnete_iop_check(gasnete_iop_t *iop) { gasnete_iop_check(iop); }

/* ====================================================================
 * LAPI Structures and constants
 * ====================================================================
 */
void** gasnete_remote_memset_hh;
void** gasnete_remote_barrier_hh;


/* ------------------------------------------------------------------------------------ */
/*
  Tuning Parameters
  =================
  Conduits may choose to override the default tuning parameters below by defining them
  in their gasnet_core_fwd.h
*/

/* ------------------------------------------------------------------------------------ */
/*
  Thread Management
  =================
*/
static gasnete_threaddata_t * gasnete_new_threaddata() {
    gasnete_threaddata_t *threaddata = NULL;
    int idx;
    gasnet_hsl_lock(&threadtable_lock);
    idx = gasnete_numthreads;
    gasnete_numthreads++;
    gasnet_hsl_unlock(&threadtable_lock);
    gasneti_assert(GASNETI_MAX_THREADS <= 256);
    #if GASNETI_CLIENT_THREADS
      if (idx >= GASNETI_MAX_THREADS) 
        gasneti_fatalerror("GASNet Extended API: Too many local client threads (limit=%i)",GASNETI_MAX_THREADS);
    #else
      gasneti_assert(idx == 0);
    #endif
    gasneti_assert(gasnete_threadtable[idx] == NULL);

    threaddata = (gasnete_threaddata_t *)gasneti_calloc(1,sizeof(gasnete_threaddata_t));

    threaddata->threadidx = idx;
    threaddata->eop_free = EOPADDR_NIL;

    gasnete_threadtable[idx] = threaddata;
    threaddata->current_iop = gasnete_iop_new(threaddata);

    return threaddata;
}
/* PURE function (returns same value for a given thread every time) 
 */
#if GASNETI_CLIENT_THREADS
  extern gasnete_threaddata_t *gasnete_mythread() {
    gasnete_threaddata_t *threaddata = gasneti_threadkey_get(gasnete_threaddata);
    GASNETI_TRACE_EVENT(C, DYNAMIC_THREADLOOKUP);
    if_pt (threaddata) {
      gasneti_memcheck(threaddata);
      return threaddata;
    }

    /* first time we've seen this thread - need to set it up */
    threaddata = gasnete_new_threaddata();
    gasneti_threadkey_set(gasnete_threaddata, threaddata);
    return threaddata;
  }
#endif
/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* called at startup to check configuration sanity */
static void gasnete_check_config() {
  gasneti_check_config_postattach();

  gasneti_assert_always(gasnete_eopaddr_isnil(EOPADDR_NIL));
  GASNETE_WIREFLAGS_SANITYCHECK();
}

extern void gasnete_init() {
    static int firstcall = 1;
    GASNETI_TRACE_PRINTF(C,("gasnete_init()"));
    gasneti_assert(firstcall); /*  make sure we haven't been called before */
    firstcall = 0;

    gasnete_check_config(); /*  check for sanity */

    gasneti_assert(gasneti_nodes >= 1 && gasneti_mynode < gasneti_nodes);

    /* Exchange LAPI addresses here */
    gasnete_remote_memset_hh = (void**)gasneti_malloc(gasneti_nodes*sizeof(void*));
    GASNETC_LCHECK(LAPI_Address_init(gasnetc_lapi_context,
				     (void*)&gasnete_lapi_memset_hh,
				     gasnete_remote_memset_hh));

    gasnete_remote_barrier_hh = (void**)gasneti_malloc(gasneti_nodes*sizeof(void*));
    GASNETC_LCHECK(LAPI_Address_init(gasnetc_lapi_context,
				     (void*)&gasnete_lapi_barrier_hh,
				     gasnete_remote_barrier_hh));



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

	/* cause the first pool of eops to be allocated (optimization) */
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
  Op management
  =============
*/
/*  get a new op and mark it in flight */
gasnete_eop_t *gasnete_eop_new(gasnete_threaddata_t * const thread) {
    gasnete_eopaddr_t head = thread->eop_free;
    if_pt (!gasnete_eopaddr_isnil(head)) {
	gasnete_eop_t *eop = GASNETE_EOPADDR_TO_PTR(thread, head);
	thread->eop_free = eop->addr;
	eop->addr = head;
	gasneti_assert(!gasnete_eopaddr_equal(thread->eop_free,head));
	gasneti_assert(eop->threadidx == thread->threadidx);
	gasneti_assert(OPTYPE(eop) == OPTYPE_EXPLICIT);
	gasneti_assert(OPSTATE(eop) == OPSTATE_FREE);
	SET_OPSTATE(eop, OPSTATE_INFLIGHT);
	GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&eop->cntr,0));
	eop->initiated_cnt = 0;
	return eop;
    } else { /*  free list empty - need more eops */
	int bufidx = thread->eop_num_bufs;
	gasnete_eop_t *buf;
	int i;
	gasnete_threadidx_t threadidx = thread->threadidx;
	if (bufidx == 256) gasneti_fatalerror("GASNet Extended API: Ran out of explicit handles (limit=65535)");
	thread->eop_num_bufs++;
	buf = (gasnete_eop_t *)gasneti_calloc(256,sizeof(gasnete_eop_t));
	for (i=0; i < 256; i++) {
	    gasnete_eopaddr_t addr;
	    addr.bufferidx = bufidx;
#if GASNETE_SCATTER_EOPS_ACROSS_CACHELINES
#ifdef GASNETE_EOP_MOD
	    addr.eopidx = (i+32) % 255;
#else
	    { int k = i+32;
            addr.eopidx = k > 255 ? k - 255 : k;
	    }
#endif
#else
	    addr.eopidx = i+1;
#endif
	    buf[i].threadidx = threadidx;
	    buf[i].addr = addr;
#if 0 /* these can safely be skipped when the values are zero */
	    SET_OPSTATE(&(buf[i]),OPSTATE_FREE); 
	    SET_OPTYPE(&(buf[i]),OPTYPE_EXPLICIT); 
#endif
	}
	/*  add a list terminator */
#if GASNETE_SCATTER_EOPS_ACROSS_CACHELINES
#ifdef GASNETE_EOP_MOD
        buf[223].addr.eopidx = 255; /* modular arithmetic messes up this one */
#endif
	buf[255].addr = EOPADDR_NIL;
#else
	buf[255].addr = EOPADDR_NIL;
#endif
	thread->eop_bufs[bufidx] = buf;
	head.bufferidx = bufidx;
	head.eopidx = 0;
	thread->eop_free = head;

#if GASNET_DEBUG
	{ /* verify new free list got built correctly */
	    int i;
	    int seen[256];
	    gasnete_eopaddr_t addr = thread->eop_free;

#if 0
	    if (gasneti_mynode == 0)
		for (i=0;i<256;i++) {                                   
		    fprintf(stderr,"%i:  %i: next=%i\n",gasneti_mynode,i,buf[i].addr.eopidx);
		    fflush(stderr);
		}
	    sleep(5);
#endif

            gasneti_memcheck(thread->eop_bufs[bufidx]);
	    memset(seen, 0, 256*sizeof(int));
	    for (i=0;i<(bufidx==255?255:256);i++) {                                   
		gasnete_eop_t *eop;                                   
		gasneti_assert(!gasnete_eopaddr_isnil(addr));                 
		eop = GASNETE_EOPADDR_TO_PTR(thread,addr);            
		gasneti_assert(OPTYPE(eop) == OPTYPE_EXPLICIT);               
		gasneti_assert(OPSTATE(eop) == OPSTATE_FREE);                 
		gasneti_assert(eop->threadidx == threadidx);                  
		gasneti_assert(addr.bufferidx == bufidx);
		gasneti_assert(!seen[addr.eopidx]);/* see if we hit a cycle */
		seen[addr.eopidx] = 1;
		addr = eop->addr;                                     
	    }                                                       
	    gasneti_assert(gasnete_eopaddr_isnil(addr)); 
	}
#endif

	return gasnete_eop_new(thread); /*  should succeed this time */
    }
}

gasnete_iop_t *gasnete_iop_new(gasnete_threaddata_t * const thread) {
    gasnete_iop_t *iop;
    if_pt (thread->iop_free) {
	iop = thread->iop_free;
	thread->iop_free = iop->next;
        gasneti_memcheck(iop);
	gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);
	gasneti_assert(iop->threadidx == thread->threadidx);
    } else {
	iop = (gasnete_iop_t *)gasneti_malloc(sizeof(gasnete_iop_t));
	SET_OPTYPE((gasnete_op_t *)iop, OPTYPE_IMPLICIT);
	iop->threadidx = thread->threadidx;
    }
	iop->next = NULL;
	iop->initiated_get_cnt = 0;
	iop->initiated_put_cnt = 0;
	GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&iop->get_cntr,0));
	GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&iop->put_cntr,0));
        gasneti_weakatomic_set(&iop->put_aux_cntr, 0, 0);
        gasneti_weakatomic_set(&iop->get_aux_cntr, 0, 0);
        gasnete_iop_check(iop);
	return iop;
}

/*  query an op for completeness - for iop this means both puts and gets */
int gasnete_op_isdone(gasnete_op_t *op) {
    int cnt = 0;
    gasneti_assert(op->threadidx == gasnete_mythread()->threadidx);
    if_pt (OPTYPE(op) == OPTYPE_EXPLICIT) {
	gasnete_eop_t *eop = (gasnete_eop_t*)op;
	gasneti_assert(OPSTATE(op) != OPSTATE_FREE);
        gasnete_eop_check(eop);
	if (eop->initiated_cnt > 0) {
	    GASNETC_LCHECK(LAPI_Getcntr(gasnetc_lapi_context,&eop->cntr,&cnt));
	    gasneti_assert(cnt <= eop->initiated_cnt);
	}
	return (eop->initiated_cnt == cnt);
    } else {
	/* only call getcntr if we need to */
	gasnete_iop_t *iop = (gasnete_iop_t*)op;
        gasnete_iop_check(iop);
        if (gasneti_weakatomic_read(&iop->get_aux_cntr, 0) > 0 || 
            gasneti_weakatomic_read(&iop->put_aux_cntr, 0) > 0) return 0;
	if (iop->initiated_get_cnt > 0) {
	    GASNETC_LCHECK(LAPI_Getcntr(gasnetc_lapi_context,&iop->get_cntr,&cnt));
	    gasneti_assert(cnt <= iop->initiated_get_cnt);
	}
	if (iop->initiated_get_cnt != cnt)
	    return 0;
	cnt = 0;
	if (iop->initiated_put_cnt > 0) {
	    GASNETC_LCHECK(LAPI_Getcntr(gasnetc_lapi_context,&iop->put_cntr,&cnt));
	    gasneti_assert(cnt <= iop->initiated_put_cnt);
	}
	return (iop->initiated_put_cnt == cnt);
    }
}

/* mark an op done
 */
void gasnete_op_markdone(gasnete_op_t *op, int isget) {
    if (OPTYPE(op) == OPTYPE_EXPLICIT) {
	gasnete_eop_t *eop = (gasnete_eop_t *)op;
	gasneti_assert(OPSTATE(eop) == OPSTATE_INFLIGHT);
        gasnete_eop_check(eop);
	GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&eop->cntr,eop->initiated_cnt));
	/* SET_OPSTATE(eop, OPSTATE_COMPLETE); -- not used */
        gasnete_eop_check(eop);
    } else {
	/* gasnete_iop_t *iop = (gasnete_iop_t *)op; */
        gasneti_fatalerror("this should not happen");
    }
}

/*  free an op */
void gasnete_op_free(gasnete_op_t *op) {
    gasnete_threaddata_t * const thread = gasnete_threadtable[op->threadidx];
    gasneti_assert(thread == gasnete_mythread());
    if (OPTYPE(op) == OPTYPE_EXPLICIT) {
	gasnete_eop_t *eop = (gasnete_eop_t *)op;
	gasnete_eopaddr_t addr = eop->addr;
        /* DOB: OPSTATE_COMPLETE not currently used by lapi-conduit
          gasneti_assert(OPSTATE(eop) == OPSTATE_COMPLETE);*/
        gasnete_eop_check(eop);
	SET_OPSTATE(eop, OPSTATE_FREE);
	eop->addr = thread->eop_free;
	thread->eop_free = addr;
    } else {
	gasnete_iop_t *iop = (gasnete_iop_t *)op;
        gasnete_iop_check(iop);
        gasneti_assert(iop->next == NULL);
	iop->next = thread->iop_free;
	thread->iop_free = iop;
    }
}

#if GASNETC_LAPI_FED_POLLBUG_WORKAROUND
/*
 * MLW: HACK to get around LAPI FEDERATION bug.
 * We allow syncing some of the outstanding put operations to
 * throttle the number of outstanding in-flight messages.  
 */
static void gasnete_wait_syncnbi_myputs(int numputs GASNETE_THREAD_FARG)
{
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t *iop = mythread->current_iop;
    int cnt = 0;
    gasneti_assert(iop->threadidx == mythread->threadidx);
    gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);
    gasneti_assert(iop->initiated_put_cnt >= numputs);
    GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&iop->put_cntr,numputs,&cnt));
    iop->initiated_put_cnt -= numputs;
    gasneti_sync_reads();  /* MLW: is this needed? */
}
#endif

/* ------------------------------------------------------------------------------------ */
/* GASNET-Internal OP Interface */
gasneti_eop_t *gasneti_eop_create(GASNETE_THREAD_FARG_ALONE) {
  gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);
  op->initiated_cnt = 1;
  return (gasneti_eop_t *)op;
}
gasneti_iop_t *gasneti_iop_register(unsigned int noperations, int isget GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t * const op = mythread->current_iop;
  gasneti_weakatomic_t * const pctr = (isget ? &(op->get_aux_cntr) : &(op->put_aux_cntr));
  gasnete_iop_check(op);
  if (noperations == 1) gasneti_weakatomic_increment(pctr, 0);
  else {
    #if defined(GASNETI_HAVE_WEAKATOMIC_ADD_SUB)
      gasneti_weakatomic_add(pctr, noperations, 0);
    #else /* yuk */
      while (noperations) {
        gasneti_weakatomic_increment(pctr, 0);
        noperations--;
      }
    #endif
  }
  gasnete_iop_check(op);
  return (gasneti_iop_t *)op;
}
void gasneti_eop_markdone(gasneti_eop_t *_eop) {
  /* cannot use gasnete_op_markdone here, because it's not AMSAFE - 
     it creates races with the app thread when run on the completion thread */
  gasnete_eop_t *eop = (gasnete_eop_t*)_eop;
  gasneti_assert(OPTYPE(eop) == OPTYPE_EXPLICIT);
  gasnete_eop_check(eop);
  GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&eop->cntr,eop->initiated_cnt));
  /* SET_OPSTATE(eop, OPSTATE_COMPLETE); -- not used */
}
void gasneti_iop_markdone(gasneti_iop_t *iop, unsigned int noperations, int isget) {
  gasnete_iop_t *op = (gasnete_iop_t *)iop;
  gasneti_weakatomic_t * const pctr = (isget ? &(op->get_aux_cntr) : &(op->put_aux_cntr));
  /* cannot use gasnete_iop_check here, because it's not AMSAFE - 
     it creates races with the app thread when run on the completion thread */
  gasneti_assert(gasneti_weakatomic_read(pctr, 0) > 0);
  gasneti_assert(OPTYPE(op) == OPTYPE_IMPLICIT);
  if (noperations == 1) gasneti_weakatomic_decrement(pctr, 0);
  else {
    #if defined(GASNETI_HAVE_WEAKATOMIC_ADD_SUB)
      gasneti_weakatomic_subtract(pctr, noperations, 0);
    #else /* yuk */
      while (noperations) {
        gasneti_weakatomic_decrement(pctr, 0);
        noperations--;
      }
    #endif
  }
}

/* --------------------------------------------------------------------------
 * This is the LAPI Header Handler that executes a gasnet_memset operation
 * on the remote node.
 * --------------------------------------------------------------------------
 */
void* gasnete_lapi_memset_hh(lapi_handle_t *context, void *uhdr, uint *uhdr_len,
			     ulong *msg_len, compl_hndlr_t **comp_h, void **uinfo)
{
    gasnete_memset_uhdr_t *u = (gasnete_memset_uhdr_t*)uhdr;

    memset((void*)(u->destLoc),u->value,u->nbytes);

    *comp_h = NULL;
    *uinfo = NULL;
    return NULL;
}

/* ------------------------------------------------------------------------------------ */
/*
  Blocking memory-to-memory transfers
  ===================================
*/
/* ------------------------------------------------------------------------------------ */
extern void gasnete_get_bulk (void *dest, gasnet_node_t node, void *src,
			      size_t nbytes GASNETE_THREAD_FARG)
{
    lapi_cntr_t c_cntr;
    int num_get = 0;
    int cur_cntr;

    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context, &c_cntr, 0));

    /* Issue as many gets as required.
     * Will generally only be one */
    gasneti_suspend_spinpollers();
    while (nbytes > 0) {
	ulong to_get = MIN(nbytes, gasnetc_max_lapi_data_size);
	GASNETC_LCHECK(LAPI_Get(gasnetc_lapi_context, (unsigned int)node, to_get,
				src,dest,NULL,&c_cntr));
	dest = (void*)((char*)dest + to_get);
	src = (void*)((char*)src + to_get);
	num_get++;
	nbytes -= to_get;
    }
    gasneti_resume_spinpollers();
    /* block until all gets complete */
    GASNETC_WAITCNTR(&c_cntr,num_get,&cur_cntr);
    gasneti_assert(cur_cntr == 0);
}

/* ------------------------------------------------------------------------------------ */
extern void gasnete_put_bulk (gasnet_node_t node, void *dest, void *src,
			      size_t nbytes GASNETE_THREAD_FARG)
{
    lapi_cntr_t  c_cntr;
    int num_put = 0;
    int cur_cntr;

    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context, &c_cntr, 0));

    /* Issue as many puts as required.
     * Will generally only be one */
    gasneti_suspend_spinpollers();
    while (nbytes > 0) {
	ulong to_put = MIN(nbytes, gasnetc_max_lapi_data_size);
	/* use op lapi counter as completion counter,
	 * and o_cntr as origin counter */
	GASNETC_LCHECK(LAPI_Put(gasnetc_lapi_context, (unsigned int) node, to_put,
				dest,src,NULL,NULL,&c_cntr));
	dest = (void*)((char*)dest + to_put);
	src = (void*)((char*)src + to_put);
	num_put++;
	nbytes -= to_put;
    }
    gasneti_resume_spinpollers();

    /* block until all complete */
    GASNETC_WAITCNTR_FBW(&c_cntr,num_put,&cur_cntr);
    gasneti_assert(cur_cntr == 0);
}

/* ------------------------------------------------------------------------------------ */
extern void gasnete_memset(gasnet_node_t node, void *dest, int val,
			   size_t nbytes GASNETE_THREAD_FARG)
{
    lapi_cntr_t c_cntr;
    int cur_cntr = 0;
    gasnete_memset_uhdr_t uhdr;

    /* We will use a LAPI active message and have the remote header handler
     * perform the memset operation.  More efficient than GASNET AMs because
     * we do not have to schedule a completion handler to issue a reply
     */
    uhdr.destLoc = (uintptr_t)dest;
    uhdr.value = val;
    uhdr.nbytes = nbytes;
	
    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context, &c_cntr, 0));
    gasneti_suspend_spinpollers();
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, (unsigned int)node,
			       gasnete_remote_memset_hh[node],
			       &uhdr, sizeof(gasnete_memset_uhdr_t), NULL, 0,
			       NULL, NULL, &c_cntr));
    gasneti_resume_spinpollers();
   

    /* block until complete */
    GASNETC_WAITCNTR_FBW(&c_cntr,1,&cur_cntr);
    gasneti_assert(cur_cntr == 0);
}


/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (explicit handle)
  ==========================================================
*/
/* ------------------------------------------------------------------------------------ */
extern gasnet_handle_t gasnete_get_nb_bulk (void *dest, gasnet_node_t node, void *src,
					    size_t nbytes GASNETE_THREAD_FARG)
{
    gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);

    /* Issue as many gets as required.
     * Will generally only be one */
    gasneti_suspend_spinpollers();
    while (nbytes > 0) {
	ulong to_get = MIN(nbytes, gasnetc_max_lapi_data_size);
	GASNETC_LCHECK(LAPI_Get(gasnetc_lapi_context, (unsigned int) node, to_get,
				src,dest,NULL,&op->cntr));
	dest = (void*)((char*)dest + to_get);
	src = (void*)((char*)src + to_get);
	op->initiated_cnt++;
	nbytes -= to_get;
    }
    gasneti_resume_spinpollers();
    return (gasnet_handle_t)op;
}

/* ------------------------------------------------------------------------------------ */
extern gasnet_handle_t gasnete_put_nb_bulk (gasnet_node_t node, void *dest, void *src,
					    size_t nbytes GASNETE_THREAD_FARG)
{
    gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);

    /* Issue as many puts as required.
     * Will generally only be one */
    gasneti_suspend_spinpollers();
    while (nbytes > 0) {
	ulong to_put = MIN(nbytes, gasnetc_max_lapi_data_size);
	/* use op lapi counter as completion counter */
	GASNETC_LCHECK(LAPI_Put(gasnetc_lapi_context, (unsigned int)node, to_put,
				dest,src,NULL,NULL,&op->cntr));
	dest = (void*)((char*)dest + to_put);
	src = (void*)((char*)src + to_put);
	op->initiated_cnt++;
	nbytes -= to_put;
    }
    gasneti_resume_spinpollers();

#if GASNETC_LAPI_FED_POLLBUG_WORKAROUND
    gasnete_wait_syncnb((gasnet_handle_t)op);
    return GASNET_INVALID_HANDLE;
#else
    return (gasnet_handle_t)op;
#endif
}

/* ------------------------------------------------------------------------------------ */
extern gasnet_handle_t gasnete_put_nb (gasnet_node_t node, void *dest, void *src,
				       size_t nbytes GASNETE_THREAD_FARG)
{
    gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);
    lapi_cntr_t  o_cntr;
    int num_put = 0;
    int cur_cntr;

    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context, &o_cntr, 0));

    /* Issue as many puts as required.
     * Will generally only be one */
    gasneti_suspend_spinpollers();
    while (nbytes > 0) {
	ulong to_put = MIN(nbytes, gasnetc_max_lapi_data_size);
	/* use op lapi counter as completion counter,
	 * and o_cntr as origin counter */
	GASNETC_LCHECK(LAPI_Put(gasnetc_lapi_context, (unsigned int) node, to_put,
				dest,src,NULL,&o_cntr,&op->cntr));
	dest = (void*)((char*)dest + to_put);
	src = (void*)((char*)src + to_put);
	num_put++;
	nbytes -= to_put;
    }
    gasneti_resume_spinpollers();
    op->initiated_cnt += num_put;
    /* Client allowed to modify src data after return.  Make sure operation
     * is complete at origin
     */
    GASNETC_WAITCNTR(&o_cntr,num_put,&cur_cntr);
    gasneti_assert(cur_cntr == 0);
    
#if GASNETC_LAPI_FED_POLLBUG_WORKAROUND
    gasnete_wait_syncnb((gasnet_handle_t)op);
    return GASNET_INVALID_HANDLE;
#else
    return (gasnet_handle_t)op;
#endif
}

/* ------------------------------------------------------------------------------------ */
extern gasnet_handle_t gasnete_memset_nb   (gasnet_node_t node, void *dest, int val,
					    size_t nbytes GASNETE_THREAD_FARG) {
    gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);
    lapi_cntr_t o_cntr;
    int cur_cntr = 0;
    gasnete_memset_uhdr_t uhdr;

    /* We will use a LAPI active message and have the remote header handler
     * perform the memset operation.  More efficient than GASNET AMs because
     * we do not have to schedule a completion handler to issue a reply
     */
    uhdr.destLoc = (uintptr_t)dest;
    uhdr.value = val;
    uhdr.nbytes = nbytes;
	
    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context, &o_cntr, 0));
    gasneti_suspend_spinpollers();
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, (unsigned int)node,
			       gasnete_remote_memset_hh[node],
			       &uhdr, sizeof(gasnete_memset_uhdr_t), NULL, 0,
			       NULL, &o_cntr, &op->cntr));
    gasneti_resume_spinpollers();
   
    op->initiated_cnt++;
    /* must insure operation has completed locally since uhdr is a stack variable.
     * This will ALMOST ALWAYS be true in the case of such a small message */
    GASNETC_WAITCNTR(&o_cntr,1,&cur_cntr);
    gasneti_assert(cur_cntr == 0);

#if GASNETC_LAPI_FED_POLLBUG_WORKAROUND
    gasnete_wait_syncnb((gasnet_handle_t)op);
    return GASNET_INVALID_HANDLE;
#else
    return (gasnet_handle_t)op;
#endif
}

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for explicit-handle non-blocking operations:
  ===========================================================
*/

extern int  gasnete_try_syncnb(gasnet_handle_t handle) {
    GASNETI_SAFE(gasneti_AMPoll());

    if (handle == GASNET_INVALID_HANDLE)
	return GASNET_OK;

    if (gasnete_op_isdone(handle)) {
	gasneti_sync_reads();
	gasnete_op_free(handle);
	return GASNET_OK;
    }
    else return GASNET_ERR_NOT_READY;
}

extern int  gasnete_try_syncnb_some (gasnet_handle_t *phandle, size_t numhandles) {
    int success = 0;
    int empty = 1;
    GASNETI_SAFE(gasneti_AMPoll());

    gasneti_assert(phandle);

    { int i;
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

    if (success || empty) return GASNET_OK;
    else return GASNET_ERR_NOT_READY;
}

extern int  gasnete_try_syncnb_all (gasnet_handle_t *phandle, size_t numhandles) {
    int success = 1;
    GASNETI_SAFE(gasneti_AMPoll());

    gasneti_assert(phandle);

    { int i;
    for (i = 0; i < numhandles; i++) {
	gasnete_op_t *op = phandle[i];
	if (op != GASNET_INVALID_HANDLE) {
	    if (gasnete_op_isdone(op)) {
		gasneti_sync_reads();
		gasnete_op_free(op);
		phandle[i] = GASNET_INVALID_HANDLE;
	    } else success = 0;
	}
    }
    }

    if (success) return GASNET_OK;
    else return GASNET_ERR_NOT_READY;
}

#if GASNETC_LAPI_FED_POLLBUG_WORKAROUND
extern void gasnete_wait_syncnb(gasnet_handle_t handle) {
    int cnt = 0;
    gasnete_op_t *op = handle;
    if (handle == GASNET_INVALID_HANDLE)
	return;
    gasneti_assert(op->threadidx == gasnete_mythread()->threadidx);
    if_pt (OPTYPE(op) == OPTYPE_EXPLICIT) {
	gasnete_eop_t *eop = (gasnete_eop_t*)op;
	gasneti_assert(OPSTATE(op) != OPSTATE_FREE);
        gasnete_eop_check(eop);
	if (eop->initiated_cnt > 0) {
	    GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&eop->cntr,eop->initiated_cnt,&cnt));
	    gasneti_assert(cnt == 0);
	    eop->initiated_cnt =0;
	}
    } else {
	gasnete_iop_t *iop = (gasnete_iop_t*)op;
        gasnete_iop_check(iop);
	if (iop->initiated_get_cnt > 0) {
	    GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&iop->get_cntr,iop->initiated_get_cnt,&cnt));
	    gasneti_assert(cnt == 0);
	    iop->initiated_get_cnt = 0;
	}
	if (iop->initiated_put_cnt > 0) {
	    GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&iop->put_cntr,iop->initiated_put_cnt,&cnt));
	    gasneti_assert(cnt == 0);
	    iop->initiated_put_cnt = 0;
	}
        if (gasneti_weakatomic_read(&iop->get_aux_cntr, 0)) /* avoid extra rmb when possible */
          GASNET_BLOCKUNTIL(gasneti_weakatomic_read(&iop->get_aux_cntr, 0) == 0);
        if (gasneti_weakatomic_read(&iop->put_aux_cntr, 0)) /* avoid extra rmb when possible */
          GASNET_BLOCKUNTIL(gasneti_weakatomic_read(&iop->put_aux_cntr, 0) == 0);
    }
    gasneti_sync_reads();
    gasnete_op_free(handle);
}
extern void gasnete_wait_syncnb_some(gasnet_handle_t *phandle, size_t numhandles) {
    gasneti_assert(phandle);

    { int i;
    for (i = 0; i < numhandles; i++) {
	gasnete_op_t *op = phandle[i];
	if (op != GASNET_INVALID_HANDLE) {
	    gasnete_wait_syncnb(op);
	    phandle[i] = GASNET_INVALID_HANDLE;
	    /* got one, just return */
	    break;
	}
    }
    }
}
extern void gasnete_wait_syncnb_all(gasnet_handle_t *phandle, size_t numhandles) {
    gasneti_assert(phandle);

    { int i;
    for (i = 0; i < numhandles; i++) {
	gasnete_op_t *op = phandle[i];
	if (op != GASNET_INVALID_HANDLE) {
	    gasnete_wait_syncnb(op);
	    phandle[i] = GASNET_INVALID_HANDLE;
	}
    }
    }
}
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (implicit handle)
  ==========================================================
*/

extern void gasnete_get_nbi_bulk (void *dest, gasnet_node_t node, void *src,
				  size_t nbytes GASNETE_THREAD_FARG)
{
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t *op = mythread->current_iop;

    gasneti_suspend_spinpollers();
    while (nbytes > 0) {
	ulong to_get = MIN(nbytes, gasnetc_max_lapi_data_size);
	GASNETC_LCHECK(LAPI_Get(gasnetc_lapi_context, (unsigned int)node, to_get,
				src,dest,NULL,&op->get_cntr));
	dest = (void*)((char*)dest + to_get);
	src = (void*)((char*)src + to_get);
	op->initiated_get_cnt++;
	nbytes -= to_get;
    }
    gasneti_resume_spinpollers();
}

/* ------------------------------------------------------------------------------------ */
extern void gasnete_put_nbi_bulk (gasnet_node_t node, void *dest, void *src,
				  size_t nbytes GASNETE_THREAD_FARG)
{
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t *op = mythread->current_iop;
    int num_put = 0;

    /* Issue as many puts as required.
     * Will generally only be one */
    gasneti_suspend_spinpollers();
    while (nbytes > 0) {
	ulong to_put = MIN(nbytes, gasnetc_max_lapi_data_size);
	/* use op lapi counter as completion counter */
	GASNETC_LCHECK(LAPI_Put(gasnetc_lapi_context, (unsigned int)node, to_put,
				dest,src,NULL,NULL,&op->put_cntr));
	dest = (void*)((char*)dest + to_put);
	src = (void*)((char*)src + to_put);
	nbytes -= to_put;
	num_put++;
    }
    op->initiated_put_cnt += num_put;
    gasneti_resume_spinpollers();

#if GASNETC_LAPI_FED_POLLBUG_WORKAROUND
    gasnete_wait_syncnbi_myputs(num_put GASNETE_THREAD_GET);
#endif    
}

/* ------------------------------------------------------------------------------------ */
extern void gasnete_put_nbi (gasnet_node_t node, void *dest, void *src,
			     size_t nbytes GASNETE_THREAD_FARG)
{
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t *op = mythread->current_iop;
    lapi_cntr_t  o_cntr;
    int num_put = 0;
    int cur_cntr;

    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context, &o_cntr, 0));

    /* Issue as many puts as required.
     * Will generally only be one */
    gasneti_suspend_spinpollers();
    while (nbytes > 0) {
	ulong to_put = MIN(nbytes, gasnetc_max_lapi_data_size);
	/* use op lapi counter as completion counter,
	 * and o_cntr as origin counter */
	GASNETC_LCHECK(LAPI_Put(gasnetc_lapi_context, (unsigned int)node, to_put,
				dest,src,NULL,&o_cntr,&op->put_cntr));
	dest = (void*)((char*)dest + to_put);
	src = (void*)((char*)src + to_put);
	num_put++;
	nbytes -= to_put;
    }
    gasneti_resume_spinpollers();
    op->initiated_put_cnt += num_put;
    /* Client allowed to modify src data after return.  Make sure operation
     * is complete at origin
     */
    GASNETC_WAITCNTR(&o_cntr,num_put,&cur_cntr);
    gasneti_assert(cur_cntr == 0);

#if GASNETC_LAPI_FED_POLLBUG_WORKAROUND
    gasnete_wait_syncnbi_myputs(num_put GASNETE_THREAD_GET);
#endif    
}

/* ------------------------------------------------------------------------------------ */
extern void gasnete_memset_nbi (gasnet_node_t node, void *dest, int val,
				size_t nbytes GASNETE_THREAD_FARG) {
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t *op = mythread->current_iop;
    lapi_cntr_t o_cntr;
    int cur_cntr = 0;
    gasnete_memset_uhdr_t uhdr;

    /* We will use a LAPI active message and have the remote header handler
     * perform the memset operation.  More efficient than GASNET AMs because
     * we do not have to schedule a completion handler to issue a reply
     */
    uhdr.destLoc = (uintptr_t)dest;
    uhdr.value = val;
    uhdr.nbytes = nbytes;
	
    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context, &o_cntr, 0));
    gasneti_suspend_spinpollers();
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, (unsigned int)node,
			       gasnete_remote_memset_hh[node],
			       &uhdr, sizeof(gasnete_memset_uhdr_t), NULL, 0,
			       NULL, &o_cntr, &op->put_cntr));
    gasneti_resume_spinpollers();
   
    op->initiated_put_cnt++;
    /* must insure operation has completed locally since uhdr is a stack variable.
     * This will ALMOST ALWAYS be true in the case of such a small message */
    GASNETC_WAITCNTR(&o_cntr,1,&cur_cntr);
    gasneti_assert(cur_cntr == 0);

#if GASNETC_LAPI_FED_POLLBUG_WORKAROUND
    gasnete_wait_syncnbi_myputs(1 GASNETE_THREAD_GET);
#endif    
}
/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for implicit-handle non-blocking operations:
  ===========================================================
*/

extern int  gasnete_try_syncnbi_gets(GASNETE_THREAD_FARG_ALONE) {
    {
	gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
	gasnete_iop_t *iop = mythread->current_iop;
	int cnt = 0;
	gasneti_assert(iop->threadidx == mythread->threadidx);
	gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);
#if GASNET_DEBUG
	if (iop->next != NULL)
	    gasneti_fatalerror("VIOLATION: attempted to call gasnete_try_syncnbi_gets() inside an NBI access region");
#endif
        if (gasneti_weakatomic_read(&iop->get_aux_cntr, 0) > 0) return GASNET_ERR_NOT_READY;
	if (iop->initiated_get_cnt > 0) {
	    GASNETC_LCHECK(LAPI_Getcntr(gasnetc_lapi_context,&iop->get_cntr,&cnt));
	    gasneti_assert(cnt <= iop->initiated_get_cnt);
	}
        if (iop->initiated_get_cnt == cnt) {
            if (cnt > 65000) { /* make sure we don't overflow the counters */
	      GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&iop->get_cntr,0));
              iop->initiated_get_cnt = 0;
            }
	    gasneti_sync_reads();
	    return GASNET_OK;
        } else return GASNET_ERR_NOT_READY;
    }
}

extern int  gasnete_try_syncnbi_puts(GASNETE_THREAD_FARG_ALONE) {
    {
	gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
	gasnete_iop_t *iop = mythread->current_iop;
	int cnt = 0;
	gasneti_assert(iop->threadidx == mythread->threadidx);
	gasneti_assert(iop->next == NULL);
	gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);
#if GASNET_DEBUG
	if (iop->next != NULL)
	    gasneti_fatalerror("VIOLATION: attempted to call gasnete_try_syncnbi_puts() inside an NBI access region");
#endif
        if (gasneti_weakatomic_read(&iop->put_aux_cntr, 0) > 0) return GASNET_ERR_NOT_READY;
	if (iop->initiated_put_cnt > 0) {
	    GASNETC_LCHECK(LAPI_Getcntr(gasnetc_lapi_context,&iop->put_cntr,&cnt));
	    gasneti_assert(cnt <= iop->initiated_put_cnt);
	}
        if (iop->initiated_put_cnt == cnt) {
            if (cnt > 65000) { /* make sure we don't overflow the counters */
	      GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&iop->put_cntr,0));
              iop->initiated_put_cnt = 0;
            }
	    gasneti_sync_reads();
	    return GASNET_OK;
        } else return GASNET_ERR_NOT_READY;
    }
}

#if GASNETC_LAPI_FED_POLLBUG_WORKAROUND
/* don't poll for put operations, polling for gets is ok */
extern void gasnete_wait_syncnbi_puts(GASNETE_THREAD_FARG_ALONE) {
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t *iop = mythread->current_iop;
    int cnt = 0;
    gasneti_assert(iop->threadidx == mythread->threadidx);
    gasneti_assert(iop->next == NULL);
    gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);
#if GASNET_DEBUG
    if (iop->next != NULL)
	gasneti_fatalerror("VIOLATION: attempted to call gasnete_wait_syncnbi_puts() inside an NBI access region");
#endif
    if (gasneti_weakatomic_read(&iop->put_aux_cntr, 0)) /* avoid extra rmb when possible */
      GASNET_BLOCKUNTIL(gasneti_weakatomic_read(&iop->put_aux_cntr, 0) == 0);
    if (iop->initiated_put_cnt > 0) {
	GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&iop->put_cntr,iop->initiated_put_cnt,&cnt));
	/* note that waitcntr decreemnts counter by amount waited for */
	gasneti_assert(cnt == 0);
	iop->initiated_put_cnt = 0;
	gasneti_sync_reads();
    } 
}

extern void gasnete_wait_syncnbi_all(GASNETE_THREAD_FARG_ALONE) {
    gasnete_wait_syncnbi_puts(GASNETE_THREAD_GET_ALONE);
    gasnete_wait_syncnbi_gets(GASNETE_THREAD_GET_ALONE);
}
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Implicit access region synchronization
  ======================================
*/
/*  This implementation allows recursive access regions, although the spec does not require that */
/*  operations are associated with the most immediately enclosing access region */
extern void  gasnete_begin_nbi_accessregion(int allowrecursion GASNETE_THREAD_FARG) {
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
/*
  Non-Blocking Value Get (explicit-handle)
  ========================================
*/
typedef struct _gasnet_valget_op_t {
    gasnet_handle_t handle;
    gasnet_register_value_t val;

    struct _gasnet_valget_op_t* next; /* for free-list only */
    gasnete_threadidx_t threadidx;  /*  thread that owns me */
} gasnet_valget_op_t;

extern gasnet_valget_handle_t gasnete_get_nb_val(gasnet_node_t node, void *src,
						 size_t nbytes GASNETE_THREAD_FARG)
{
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnet_valget_handle_t retval;
    gasneti_assert(nbytes > 0 && nbytes <= sizeof(gasnet_register_value_t));
    gasneti_boundscheck(node, src, nbytes);
    if (mythread->valget_free) {
	retval = mythread->valget_free;
	mythread->valget_free = retval->next;
        gasneti_memcheck(retval);
    } else {
	retval = (gasnet_valget_op_t*)gasneti_malloc(sizeof(gasnet_valget_op_t));
	retval->threadidx = mythread->threadidx;
    }

    retval->val = 0;
    if (gasnete_islocal(node)) {
      GASNETE_FAST_ALIGNED_MEMCPY(GASNETE_STARTOFBITS(&(retval->val),nbytes), src, nbytes);
      retval->handle = GASNET_INVALID_HANDLE;
    } else {
      retval->handle = gasnete_get_nb_bulk(GASNETE_STARTOFBITS(&(retval->val),nbytes), node, src, nbytes GASNETE_THREAD_PASS);
    }
    return retval;
}

extern gasnet_register_value_t gasnete_wait_syncnb_valget(gasnet_valget_handle_t handle) {
    gasnet_register_value_t val;
    gasnete_threaddata_t * const thread = gasnete_threadtable[handle->threadidx];
    gasneti_assert(thread == gasnete_mythread());
    handle->next = thread->valget_free; /* free before the wait to save time after the wait, */
    thread->valget_free = handle;       /*  safe because this thread is under our control */

    gasnete_wait_syncnb(handle->handle);
    val = handle->val;
    return val;
}

/* ------------------------------------------------------------------------------------ */
/*
  Barriers:
  =========
*/
static void gasnete_lapibarrier_init(int usegfence);
static void gasnete_lapibarrier_notify(int id, int flags);
static int gasnete_lapibarrier_wait(int id, int flags);
static int gasnete_lapibarrier_try(int id, int flags);

#define GASNETE_BARRIER_DEFAULT "LAPIGFENCE"
#define GASNETE_BARRIER_INIT() do {                         \
    if (GASNETE_ISBARRIER("LAPIGFENCE")) {                  \
      gasnete_barrier_notify = &gasnete_lapibarrier_notify; \
      gasnete_barrier_wait =   &gasnete_lapibarrier_wait;   \
      gasnete_barrier_try =    &gasnete_lapibarrier_try;    \
      gasnete_lapibarrier_init(1);                          \
    } else if (GASNETE_ISBARRIER("LAPIAM")) {               \
      gasnete_barrier_notify = &gasnete_lapibarrier_notify; \
      gasnete_barrier_wait =   &gasnete_lapibarrier_wait;   \
      gasnete_barrier_try =    &gasnete_lapibarrier_try;    \
      gasnete_lapibarrier_init(0);                          \
    }                                                       \
  } while (0)

/* allow reference implementation of barrier */
#define GASNETI_GASNET_EXTENDED_REFBARRIER_C 1
#include "gasnet_extended_refbarrier.c"
#undef GASNETI_GASNET_EXTENDED_REFBARRIER_C
/* ------------------------------------------------------------------------------------ */
/* LAPI barrier */
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
/*  LAPI MODS:  Replaced GASNET CORE AM calls with LAPI Amsend calls.
    This should run faster because it eliminates the need to schedule a
    LAPI completion handler to run the AM handler.
    Also, header handler of last client Amsend operations to notify
    schedules a completion handler to inform all clients the barrier
    has been reached.  This eliminates the need for Master node to
    notice the barrier has been reached.
*/

static int volatile barrier_value; /*  local barrier value */
static int volatile barrier_flags; /*  local barrier flags */
static int volatile barrier_phase = 0;  /*  2-phase operation to improve pipelining */
static int volatile barrier_response_done[2] = { 0, 0 }; /*  non-zero when barrier is complete 
                                                             also has mismatch bit set if root detected mismatch */
static int barrier_gfence;
/*  global state on P0 */
#define GASNETE_BARRIER_MASTER (gasneti_nodes-1)
static gasneti_mutex_t barrier_lock = GASNETI_MUTEX_INITIALIZER;
static int volatile barrier_consensus_value[2]; /*  consensus barrier value */
static int volatile barrier_consensus_value_present[2] = { 0, 0 }; /*  consensus barrier value found */
static int volatile barrier_consensus_mismatch[2] = { 0, 0 }; /*  mismatch bit set if root detected a mismatch */
static int volatile barrier_count[2] = { 0, 0 }; /*  count of how many remotes have notified (on P0) */

static void gasnete_lapibarrier_init(int usegfence) {
  barrier_gfence = usegfence;
}

/* LAPI Completion handler scheduled by HH on master node when the last node
 * has performed the notify.
 * Job is to send done message to all other nodes.
 */
void gasnete_lapi_barrier_ch(lapi_handle_t *context, void* user_info) {
    gasnete_barrier_uhdr_t * const uhdr = (gasnete_barrier_uhdr_t*)user_info;
    int i;
    lapi_cntr_t comp_cntr;
    lapi_cntr_t *barrier_completion = NULL;
    if (barrier_gfence) {
      /* when using Gfence, sender needs to ensure completion has run 
         (and broadcast any mismatches) before continuing
      */
      GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context, &comp_cntr, 0));
      barrier_completion = &comp_cntr;
    }
    /* Note: uhdr is a pointer to a static structure.  It will not
     * be used again until the next barrier has been reached by all
     * nodes (with the same phase).  At that point, all clients will
     * have had to received the active message.  This implies
     * the these AM calls will have completed locally.  That is,
     * we do not have to protect re-use of this uhdr by waiting
     * on a local counter variable.
     */
    #if GASNET_TRACE
      { const int phase = GASNETE_WIREFLAGS_PHASE(uhdr->wireflags);
        const int flags = GASNETE_WIREFLAGS_FLAGS(uhdr->wireflags);
        GASNETI_TRACE_PRINTF(B,("BARRIER_CH: phase %d, value %d, flags %d",
			        phase, uhdr->value, flags));
      }
    #endif

    /* inform all nodes (except local node) that barrier is complete */
    gasneti_suspend_spinpollers();
    for (i=0; i < gasneti_nodes; i++) {
	if ( i == gasneti_mynode ) continue;
	GASNETC_LCHECK(LAPI_Amsend(*context, (unsigned int)i,
				   gasnete_remote_barrier_hh[i],
				   uhdr, sizeof(gasnete_barrier_uhdr_t), NULL, 0,
				   NULL, NULL, barrier_completion));
    }
    gasneti_resume_spinpollers();

    /* when using gfence mechanism, need to ensure everyone has acknowledged the mismatch
       before this completion handler can finish (and inform the remote mismatch 
       initiator to proceed with the Gfence)
    */
    if (barrier_gfence) {
        int cur_cntr = 0;          
        GASNETC_WAITCNTR(&comp_cntr,gasneti_nodes-1,&cur_cntr);
        gasneti_assert(cur_cntr == 0);
    }
}

/* LAPI header handler to implement both notify and done requests on remote node */
void* gasnete_lapi_barrier_hh(lapi_handle_t *context, void *uhdr, uint *uhdr_len,
			      ulong *msg_len, compl_hndlr_t **comp_h, void **uinfo) {
    gasnete_barrier_uhdr_t const * const u = (gasnete_barrier_uhdr_t*)uhdr;
    const uint8_t wf = u->wireflags;
    const int value = u->value;
    const int phase = GASNETE_WIREFLAGS_PHASE(wf);
    const int is_notify = GASNETE_WIREFLAGS_ISNOTIFY(wf);
    int mismatch_firstdetect = 0;

    GASNETI_TRACE_PRINTF(B,("BARRIER_HH: node %d, notify %d, phase %d, value %d, wireflags %d, g_val %d, g_count %d",
			    gasneti_mynode,is_notify,phase,value,GASNETE_WIREFLAGS_FLAGS(wf),
			    barrier_consensus_value[phase],barrier_count[phase]));

    *comp_h = NULL;
    *uinfo = NULL;

    if (is_notify) {
	/* this is a notify header handler call */
	gasneti_assert(gasneti_mynode == GASNETE_BARRIER_MASTER);

      #if GASNETE_BARRIER_BYPASS_LOOPBACK_AMSEND
        gasneti_mutex_lock(&barrier_lock);
      #else
	/* Don't need a lock here because header handlers are
	 * run by LAPI dispatcher and thus guaranteed to run one-at-a-time.
	 * Varibles read and updated here are only done so in this function.
	 */
      #endif
        {   const int flags = GASNETE_WIREFLAGS_FLAGS(wf);
	    const int count = barrier_count[phase] + 1;
	    if (!(flags & (GASNET_BARRIERFLAG_ANONYMOUS|GASNET_BARRIERFLAG_MISMATCH)) && 
                !barrier_consensus_value_present[phase]) { /* first named notify to arrive */
		barrier_consensus_value[phase] = value;
		barrier_consensus_value_present[phase] = 1;
	    } else if_pf ((flags & GASNET_BARRIERFLAG_MISMATCH) ||
		       (!(flags & GASNET_BARRIERFLAG_ANONYMOUS) 
                       && barrier_consensus_value[phase] != value)) { /* mismatch reported or detected */
                if (barrier_gfence && barrier_consensus_mismatch[phase] == 0) /* mismatch first detected - broadcast it */
                  mismatch_firstdetect = 1;
		barrier_consensus_mismatch[phase] = GASNET_BARRIERFLAG_MISMATCH;
	    }
	    barrier_count[phase] = count; /* update count */

            if ((barrier_gfence && mismatch_firstdetect) ||
                (!barrier_gfence && (count == gasneti_nodes))) {
		/* schedule completion handler to notify all nodes that barrier has been reached.
		 * use a static uhdr to avoid allocation cost, phased to ensure race-freedom
		 */
                static gasnete_barrier_uhdr_t barrier_uhdr[2] = /* init .phase, which never changes */
                  { {0, GASNETE_WIREFLAGS_SET(0, 0, 0)} , {0, GASNETE_WIREFLAGS_SET(0, 1, 0)} }; 
		gasnete_barrier_uhdr_t * const uch = (gasnete_barrier_uhdr_t*)&barrier_uhdr[phase];
                const int mismatch = barrier_consensus_mismatch[phase];
                const int wireflags = GASNETE_WIREFLAGS_SET(mismatch, phase, 0);

                if (!barrier_gfence) gasneti_assert(phase == barrier_phase);

		uch->value = barrier_consensus_value[phase];
                uch->wireflags = wireflags;
		*uinfo = (void*)uch;
		*comp_h = gasnete_lapi_barrier_ch;
		/* Perform local state update to get the effects of a local done handler  
                 * Note that the completion handler will not send an AM to this node
		 */
	        barrier_response_done[phase] = 1 | mismatch;

		GASNETI_TRACE_PRINTF(B,("BARRIER_HH: REACHED %d, mismatch %d, SCHEDULING CH",
					gasneti_nodes, 
                                        GASNETE_WIREFLAGS_FLAGS(uch->wireflags)&GASNET_BARRIERFLAG_MISMATCH));
	    }
	}
      #if GASNETE_BARRIER_BYPASS_LOOPBACK_AMSEND
        gasneti_mutex_unlock(&barrier_lock);
      #endif
    } else {
	/* this is a done header handler call... update local state */
        if (!barrier_gfence) gasneti_assert(phase == barrier_phase);

        /* local mismatch and done signals are folded into the same variable write,
           to avoid the need for a local write barrier here between the two writes */
	barrier_response_done[phase] = 1 | (GASNETE_WIREFLAGS_FLAGS(u->wireflags)&GASNET_BARRIERFLAG_MISMATCH);
    }
    return NULL;
}

static void gasnete_lapibarrier_notify(int id, int flags) {
  gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
  if_pf(barrier_splitstate == INSIDE_BARRIER) 
      gasneti_fatalerror("gasnet_barrier_notify() called twice in a row");

  GASNETI_TRACE_PRINTF(B, ("BARRIER_NOTIFY(id=%i,flags=%i)", id, flags));
#if GASNETI_STATS_OR_TRACE
  barrier_notifytime = GASNETI_TICKS_NOW_IFENABLED(B);
#endif

  {
    const int phase = !barrier_phase; /*  enter new phase */
    barrier_phase = phase;
    barrier_value = id;
    barrier_flags = flags;

    if (gasneti_nodes > 1) {
    if (!barrier_gfence ||
       !(flags & GASNET_BARRIERFLAG_ANONYMOUS) ||
        (flags & GASNET_BARRIERFLAG_MISMATCH)) {
      /* use a static uhdr to avoid allocation cost, phased to ensure race-freedom */
      static gasnete_barrier_uhdr_t _uhdr[2]; 
      gasnete_barrier_uhdr_t * const uhdr = &_uhdr[phase];

      uhdr->value = barrier_value;
      uhdr->wireflags = GASNETE_WIREFLAGS_SET(flags, phase, 1);

      /*  send notify msg to master */
      /* tradeoff: we can check if mynode == GASNETE_BARRIER_MASTER and bypass some LAPI
         overhead for a loopback AM notification by invoking the header handler directly, 
         but this implies we need a lock in the header handler to ensure correctness
       */
    #if GASNETE_BARRIER_BYPASS_LOOPBACK_AMSEND
      if (gasneti_mynode == GASNETE_BARRIER_MASTER) {
        static uint uhdrlen = sizeof(gasnete_barrier_uhdr_t);
        static ulong msglen = 0;
        compl_hndlr_t *comp_h = NULL;
        void *uinfo = NULL;
        gasnete_lapi_barrier_hh(&gasnetc_lapi_context, uhdr, &uhdrlen,
                                &msglen, &comp_h, &uinfo);
        if (comp_h) (*comp_h)(&gasnetc_lapi_context, uinfo);
      } else 
    #endif
      {
        lapi_cntr_t comp_cntr;
        lapi_cntr_t *barrier_completion = NULL;
        if (barrier_gfence) {
          /* when using Gfence, sender needs to ensure completion has run 
             (and broadcast any mismatches) before continuing
          */
          GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context, &comp_cntr, 0));
          barrier_completion = &comp_cntr;
        }

        gasneti_suspend_spinpollers();
	GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context,
				   (unsigned int)GASNETE_BARRIER_MASTER,
				   gasnete_remote_barrier_hh[GASNETE_BARRIER_MASTER],
				   uhdr, sizeof(gasnete_barrier_uhdr_t), NULL, 0,
				   NULL, NULL, barrier_completion));
        gasneti_resume_spinpollers();

        if (barrier_gfence) { 
          int cur_cntr = 0;
          GASNETC_WAITCNTR(&comp_cntr,1,&cur_cntr);
          gasneti_assert(cur_cntr == 0);
        }
      }
     }

     if (barrier_gfence) { 
       LAPI_Gfence(gasnetc_lapi_context);
       barrier_response_done[phase] = 1 | barrier_response_done[phase];
     }

    } else {
	barrier_response_done[phase] = 1 | (flags & GASNET_BARRIERFLAG_MISMATCH);
    }

    /*  update state */
    barrier_splitstate = INSIDE_BARRIER;
    gasneti_sync_writes(); /* ensure all state changes committed before return */
  }
}

static int gasnete_lapibarrier_wait(int id, int flags) {
#if GASNETI_STATS_OR_TRACE
    gasneti_tick_t wait_start = GASNETI_TICKS_NOW_IFENABLED(B);
#endif
    int phase;
    gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
    phase = barrier_phase;
    if_pf(barrier_splitstate == OUTSIDE_BARRIER) 
	gasneti_fatalerror("gasnet_barrier_wait() called without a matching notify");

    GASNETI_TRACE_EVENT_TIME(B,BARRIER_NOTIFYWAIT,gasneti_ticks_now()-barrier_notifytime);

    /*  wait for response */
    gasneti_polluntil(barrier_response_done[phase]);

    GASNETI_TRACE_EVENT_TIME(B,BARRIER_WAIT,gasneti_ticks_now()-wait_start);

    /* if this is the master node, reset the global state */
    if (gasneti_mynode == GASNETE_BARRIER_MASTER) {
	barrier_count[phase] = 0;
	barrier_consensus_mismatch[phase] = 0;
	barrier_consensus_value_present[phase] = 0;
    }
    
    { const int global_mismatch = barrier_response_done[phase] & GASNET_BARRIERFLAG_MISMATCH;
      /*  update local state */
      barrier_splitstate = OUTSIDE_BARRIER;
      barrier_response_done[phase] = 0;
      gasneti_sync_writes(); /* ensure all state changes committed before return */
      if_pf((!(flags & GASNET_BARRIERFLAG_ANONYMOUS) && id != barrier_value) || /* local mismatch */
	    flags != barrier_flags || 
            global_mismatch) { /* global mismatch */
          return GASNET_ERR_BARRIER_MISMATCH;
      }
      else return GASNET_OK;
    }
}

static int gasnete_lapibarrier_try(int id, int flags) {
    gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
    if_pf(barrier_splitstate == OUTSIDE_BARRIER) 
	gasneti_fatalerror("gasnet_barrier_try() called without a matching notify");

    /* should we kick the network if not done? */

    if (barrier_response_done[barrier_phase]) {
	GASNETI_TRACE_EVENT_VAL(B,BARRIER_TRY,1);
	return gasnete_lapibarrier_wait(id, flags);
    } else {
	GASNETI_TRACE_EVENT_VAL(B,BARRIER_TRY,0);
	return GASNET_ERR_NOT_READY;
    }
}
/* ------------------------------------------------------------------------------------ */
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

/* ------------------------------------------------------------------------------------ */
/*
  Handlers:
  =========
*/
static gasnet_handlerentry_t const gasnete_handlers[] = {
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
    { 0, NULL }
};

extern gasnet_handlerentry_t const *gasnete_get_handlertable() {
    return gasnete_handlers;
}

/* ------------------------------------------------------------------------------------ */
