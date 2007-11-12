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

#ifdef GASNETC_LAPI_RDMA
#include <sys/atomic_op.h>
extern int gasnetc_lapi_use_rdma;
#endif

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
    gasneti_assert(GASNETI_MAX_THREADS <= (1U<<(sizeof(gasnete_threadidx_t)*8)));
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
#if GASNETC_LAPI_RDMA
        if_pt(gasnetc_lapi_use_rdma) {
          eop->local_p = 0;
          eop->origin_counter = NULL;
          eop->num_transfers = 0;
        }
#endif
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

#if GASNETC_LAPI_RDMA

void gasnete_free_network_buffer(gasnete_lapi_nb *nb);
/* Use bounce buffers for transfers below the following threshold */
/* 4K for now, will/should be user controlled later */
int gasnete_pin_threshold = (4*1024); 
int gasnete_pin_max = 16*1024*1024;
#endif

/*  query an op for completeness - for iop this means both puts and gets */

int gasnete_op_isdone(gasnete_op_t *op) 
{
    int cnt = 0;
    gasneti_assert(op->threadidx == gasnete_mythread()->threadidx);

    if_pt (OPTYPE(op) == OPTYPE_EXPLICIT) {
	gasnete_eop_t *eop = (gasnete_eop_t*)op;
	gasneti_assert(OPSTATE(op) != OPSTATE_FREE);
        gasnete_eop_check(eop);
#if GASNETC_LAPI_RDMA
        if(gasnetc_lapi_use_rdma) {
	  if (eop->initiated_cnt > 0) {
            gasneti_assert(!eop->local_p);
            /* Internal interface used for eop */
	    GASNETC_LCHECK(LAPI_Getcntr(gasnetc_lapi_context,&eop->cntr,&cnt));
	    gasneti_assert(cnt <= eop->initiated_cnt);
	    return (eop->initiated_cnt == cnt);
	  }
        
          if(eop->local_p) {
            return(1);
          }

	  GASNETC_LCHECK(LAPI_Getcntr(gasnetc_lapi_context,eop->origin_counter,&cnt));
	  gasneti_assert(cnt <= eop->num_transfers);
          if(eop->num_transfers == cnt) {
	    return (1);
          } else {
	    return (0);
          }
        } else {
#endif
	if (eop->initiated_cnt > 0) {
	    GASNETC_LCHECK(LAPI_Getcntr(gasnetc_lapi_context,&eop->cntr,&cnt));
	    gasneti_assert(cnt <= eop->initiated_cnt);
	}
	return (eop->initiated_cnt == cnt);
#if GASNETC_LAPI_RDMA
       }
#endif
    } else {
	gasnete_iop_t *iop = (gasnete_iop_t*)op;
        gasnete_iop_check(iop);
        if (gasneti_weakatomic_read(&iop->get_aux_cntr, 0) > 0 || 
            gasneti_weakatomic_read(&iop->put_aux_cntr, 0) > 0) return 0;
	if (iop->initiated_get_cnt > 0) {
	    GASNETC_LCHECK(LAPI_Getcntr(gasnetc_lapi_context,&iop->get_cntr,&cnt));
	    gasneti_assert(cnt <= iop->initiated_get_cnt);
	}
	if (iop->initiated_get_cnt != cnt) {
          return(0);
        }
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
    /* gasneti_sync_writes(); */
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
#if GASNETC_LAPI_RDMA /* The RDMA conduit zeros these out.  The other one doesn't */
        gasneti_assert(iop->initiated_get_cnt == 0);
        gasneti_assert(iop->initiated_put_cnt == 0);
#endif
        gasneti_assert(gasneti_weakatomic_read(&iop->get_aux_cntr, 0) == 0);
        gasneti_assert(gasneti_weakatomic_read(&iop->put_aux_cntr, 0) == 0);
        gasnete_iop_check(iop);
        gasneti_assert(iop->next == NULL);
	iop->next = thread->iop_free;
	thread->iop_free = iop;
    }
}


#if GASNETC_LAPI_FED_POLLBUG_WORKAROUND && !GASNETC_LAPI_RDMA
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


#if GASNETC_LAPI_RDMA || GASNETC_LAPI_FED_BUG_WORKAROUND
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
  /* gasneti_sync_writes(); */
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
  /* gasneti_sync_writes(); */
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

#if GASNETC_LAPI_RDMA

/* 
 * Try to resuse as much code as possible
 */

static pthread_mutex_t nb_lock = PTHREAD_MUTEX_INITIALIZER;
int64_t gasnet_lapi_bytes_pinned = 0;
/* Pin like there's no tomorrow! */
lapi_user_pvo_t gasnetc_lapi_get_local_pvo(lapi_long_t addr, size_t len)
{
	lapi_get_pvo_t new_pvo;
	new_pvo.Util_type = LAPI_XLATE_ADDRESS;
	new_pvo.length = len;
	new_pvo.usr_pvo = 0;
	new_pvo.address = (void *) addr;
	new_pvo.operation = LAPI_RDMA_ACQUIRE;
	/* Keep track of the PVOs and release the ones that have been around too long */
	GASNETC_LCHECK(LAPI_Util(gasnetc_lapi_context,(lapi_util_t *) &new_pvo));
        fetch_and_addlp(&gasnet_lapi_bytes_pinned, len);
	return(new_pvo.usr_pvo);
}

gasnetc_lapi_pvo *gasnetc_lapi_new_pvo()
{
   gasnetc_lapi_pvo *ret;
   int my_thread_id = gasnete_mythread()->threadidx;
   volatile gasnetc_lapi_pvo **current = (volatile gasnetc_lapi_pvo **) &(gasnetc_lapi_pvo_free_list[my_thread_id]);
   pthread_mutex_lock(&nb_lock);
   while(*current == NULL) {
     pthread_mutex_unlock(&nb_lock);
     gasneti_AMPoll();
     pthread_mutex_lock(&nb_lock);
   }
   ret = (gasnetc_lapi_pvo *) (*current);
   gasnetc_lapi_pvo_free_list[my_thread_id] = ret->next;
   ret->next = NULL;
   pthread_mutex_unlock(&nb_lock);
   return(ret);
}

void gasnetc_lapi_free_pvo(gasnetc_lapi_pvo *current)
{
  /* Add current to the free list */
  pthread_mutex_lock(&nb_lock);
  current->next = gasnetc_lapi_pvo_free_list[gasnete_mythread()->threadidx];
  gasnetc_lapi_pvo_free_list[gasnete_mythread()->threadidx] = current;
  pthread_mutex_unlock(&nb_lock);
}

void gasnetc_lapi_release_pvo(gasnetc_lapi_pvo *entry)
{
  lapi_get_pvo_t new_pvo;
  /* Unpin */
  new_pvo.Util_type = LAPI_XLATE_ADDRESS;
  new_pvo.length = 0;
  new_pvo.usr_pvo = entry->pvo;
  new_pvo.address = 0;
  new_pvo.operation = LAPI_RDMA_RELEASE;
  GASNETC_LCHECK(LAPI_Util(gasnetc_lapi_context, (lapi_util_t *) &new_pvo)); 
  fetch_and_addlp(&gasnet_lapi_bytes_pinned, entry->len);
  /* Return this guy to the free list */
  gasnetc_lapi_free_pvo(entry);
}

gasnete_lapi_nb *gasnete_free_nb_list_original = NULL;
char *gasnete_lapi_all_buffers = NULL;
gasnete_lapi_nb *gasnete_free_nb_list = NULL;
gasnete_lapi_nb *gasnete_active_nb_list = NULL;
#define GASNETE_LAPI_NUM_NB 1024
int gasnete_num_nb = GASNETE_LAPI_NUM_NB;
void gasnete_lapi_setup_nb()
{
  size_t total_pinned_region = gasnete_num_nb * gasnete_pin_threshold;
  char *all_data;
  size_t size_pinned_region = 0;
  lapi_get_pvo_t req;
  int count = 0;
  gasnete_free_nb_list_original = gasnete_free_nb_list = (gasnete_lapi_nb *) gasneti_malloc(gasnete_num_nb*sizeof(gasnete_lapi_nb));
  all_data = gasnete_lapi_all_buffers = (char *) gasneti_malloc(total_pinned_region);
  gasneti_assert(GASNETC_LAPI_PVO_EXTENT % gasnete_pin_threshold == 0);
  GASNETI_TRACE_PRINTF(C,("gasnete_lapi_setup_nb: node = %d pinned size = %ld #buffers = %d\n",gasneti_mynode,total_pinned_region,gasnete_num_nb));
  while(size_pinned_region < total_pinned_region) {
    int this_region_size = MIN(GASNETC_LAPI_PVO_EXTENT, total_pinned_region - size_pinned_region);
    int num_slices,s;
    req.Util_type = LAPI_XLATE_ADDRESS;
    req.length = this_region_size;
    req.usr_pvo =0;
    req.address = all_data + size_pinned_region;
    req.operation = LAPI_RDMA_ACQUIRE;
    GASNETC_LCHECK(LAPI_Util(gasnetc_lapi_context, (lapi_util_t *) &req));
    GASNETI_TRACE_PRINTF(C,("gasnete_lapi_setup_nb: %d got pvo %ld for network buffer\n",gasneti_mynode,(uint64_t) req.usr_pvo)); 
    num_slices = this_region_size/gasnete_pin_threshold;
    for(s = 0; s < num_slices; s++) {
      gasnete_free_nb_list[count].data = all_data + size_pinned_region + s*gasnete_pin_threshold;
      gasnete_free_nb_list[count].offset = s*gasnete_pin_threshold;
      gasnete_free_nb_list[count].pvo = req.usr_pvo;
      gasnete_free_nb_list[count].prev = NULL;
      if(count < gasnete_num_nb-1) {
        gasnete_free_nb_list[count].next = &(gasnete_free_nb_list[count+1]);
      } else {
        gasnete_free_nb_list[count].next = NULL;
      }
      count++;
    }
    size_pinned_region += this_region_size;
  }
}

void gasnete_lapi_free_nb()
{
  int i;
  lapi_get_pvo_t req;
  for(i=0;i < gasnete_num_nb;i++) {
    /* Only release the PVO once.  The guy with offset == 0 is in some sense
       the "head" */
    if(gasnete_free_nb_list_original[i].offset == 0) {
      req.Util_type = LAPI_XLATE_ADDRESS;
      req.length = 0;
      req.address = 0;
      req.usr_pvo =gasnete_free_nb_list_original[i].pvo;
      req.operation = LAPI_RDMA_RELEASE;
      GASNETC_LCHECK(LAPI_Util(gasnetc_lapi_context, (lapi_util_t *) &req)); 
    }
  }
  gasneti_free(gasnete_lapi_all_buffers);
  gasneti_free(gasnete_free_nb_list_original);
}

/* Need back pointers etc. so that reaping happens */
gasnete_lapi_nb *gasnete_get_free_network_buffer()
{
  gasnete_lapi_nb *ret, *current;
  volatile gasnete_lapi_nb **fl_ptr = (volatile gasnete_lapi_nb **) &gasnete_free_nb_list;
  int cnt = 0;
  pthread_mutex_lock(&nb_lock);
  while(*fl_ptr == NULL) {
    /* Need to give up the lock for a while.  Need to tune this */
    pthread_mutex_unlock(&nb_lock);
    gasneti_AMPoll();
    cnt++;
    if(cnt > 100000) {
      cnt = 0;
      printf("%d Spinning too much waiting for a network buffer?\n",gasneti_mynode);
    }
    pthread_mutex_lock(&nb_lock);
  }
  /* Remove from free list */
  ret = (gasnete_lapi_nb *) *fl_ptr;
  *fl_ptr = (*fl_ptr)->next;

  /* Place on active list */
  ret->next = gasnete_active_nb_list;
  ret->prev = NULL;
  if(gasnete_active_nb_list != NULL) {
    gasnete_active_nb_list->prev = ret;
  }
  gasnete_active_nb_list = ret;

 UNLOCK_AND_RETURN:
  pthread_mutex_unlock(&nb_lock);  /* This should take care of memory consistency nastiness, right? */
  return(ret);
}

void gasnete_free_network_buffer(gasnete_lapi_nb *nb)
{
  if(nb->get_p) {
    /* Copy out */
    memcpy(nb->user_buffer,nb->data,nb->user_length);
  }

  pthread_mutex_lock(&nb_lock);
  /* Remove from active list */
  if(gasnete_active_nb_list == nb) {
    gasnete_active_nb_list = nb->next;
    if(nb->next != NULL) {
      nb->next->prev = NULL;
    }
  } else {
    nb->prev->next = nb->next;
    if(nb->next != NULL) {
      nb->next->prev = nb->prev;
    }
  }

  /* Add back to free list */
  nb->next = gasnete_free_nb_list;
  gasnete_free_nb_list = nb; 
  pthread_mutex_unlock(&nb_lock);
}

void gasnete_lapi_reap_network_buffer(lapi_handle_t *hndl, void *user_data, lapi_sh_info_t *info)
{
  gasnete_lapi_nb *nb_id = (gasnete_lapi_nb *) user_data;
  int oldval = fetch_and_add(&(nb_id->num_waiting),-1);
  /* Bump up the origin counter */
  GASNETC_LCHECK(LAPI_Put(gasnetc_lapi_context, gasneti_mynode, 0,NULL, NULL, NULL, NULL, nb_id->origin_counter));
  if(oldval == 1) {
    /* Return this guy to the pool */
   gasnete_free_network_buffer(nb_id);
  }
}

void gasnete_lapi_reap_pvo(lapi_handle_t *hndl, void *user_data, lapi_sh_info_t *info)
{
  gasnetc_lapi_pvo *pvo_container = (gasnetc_lapi_pvo *) user_data; 
  gasnetc_lapi_release_pvo(pvo_container);
}

extern firehose_info_t gasnetc_firehose_info;
void gasnete_lapi_release_firehose(lapi_handle_t *hndl, void *user_data, lapi_sh_info_t *info)
{
  const firehose_request_t *fh_req = (firehose_request_t *) user_data;
  firehose_release(&fh_req,1);
}

#if GASNET_SEGMENT_EVERYTHING
typedef struct _gasnete_lapi_transfer_info_struct {
  lapi_long_t local_p;
  lapi_long_t offset;
  lapi_long_t remote_p;
  lapi_long_t ask_bytes;
  int64_t *initiated_bytes_ptr;
  int *num_transfers_p;
  int op;
  int node;
  lapi_remote_cxt_t rcxt; 
  gasnete_eop_t *eop;
  int tag;
  const firehose_request_t *fh_loc;
  int transfer_len;
} gasnete_lapi_transfer_info;

int pin_tag = 0;

typedef struct _gasnete_lapi_firehose_free_struct {
  const firehose_request_t *loc_fh;
  const firehose_request_t *rem_fh;
  atomic_p counter_p;
} gasnete_lapi_firehose_free_t;

int current_local_count = 0;
void gasnete_lapi_release_remote_local_firehose(lapi_handle_t *hndl, void *user_data, lapi_sh_info_t *info)
{
  gasnete_lapi_firehose_free_t *f_info = (gasnete_lapi_firehose_free_t *) user_data;
  firehose_release((const firehose_request_t **) &(f_info->loc_fh),1);
  fetch_and_add(&current_local_count,-1);
    
    fetch_and_add(f_info->counter_p,-1);
    firehose_release((const firehose_request_t **) &(f_info->rem_fh),1);
    gasneti_free(f_info->counter_p);
  gasneti_free(user_data);
}

/* Callback after remote stuff is pinned */
void gasnete_lapi_complete_transfer(void *context, const firehose_request_t *req, int allLocalHit)
{
  gasnete_lapi_transfer_info *info = (gasnete_lapi_transfer_info *) context;

  /* Ensure you got what you asked for */

  lapi_long_t nbytes = info->ask_bytes;
  lapi_long_t nbytes_transferred = 0;
  lapi_long_t remaining_bytes = nbytes;
  lapi_long_t local_p_to_long = info->local_p + info->offset;
  lapi_long_t remote_p_to_long = info->remote_p + info->offset;


  /* 
     Protocol so that we only release the remote firehose once 
     Start out variable at 1
     Each transfer increments it
     Each send completion handler decrements it
     This handler decrements it once after all transfers are done
     When it reaches 0, (old value = 1), we're done so release remote firehose 
  */
  atomic_p transfer_water_mark_p = (atomic_p) gasneti_malloc(sizeof(int));
  gasneti_assert(transfer_water_mark_p != NULL);
  *transfer_water_mark_p = 0;

  gasneti_assert((info->remote_p + info->ask_bytes) <= (req->addr + req->len));
    const firehose_request_t *fh_req;
    int source_offset, remote_offset;
    int transfer_len;
    lapi_xfer_t xfer_struct;

    /* Ask for less because of page size rounding up */
    /* lapi_long_t ask_bytes = MIN(remaining_bytes, gasnetc_firehose_info.max_LocalPinSize - ((local_p_to_long + nbytes_transferred) & (GASNETI_PAGESIZE-1))); */

    fh_req = info->fh_loc;
    /* This is how much I can do for now */
    transfer_len = info->transfer_len;

    /* These are the PVOs */ 
    xfer_struct.HwXfer.src_pvo = fh_req->client.pvo;
    xfer_struct.HwXfer.tgt_pvo = req->client.pvo;

    /* Get source and target offsets */
    remote_offset = remote_p_to_long + nbytes_transferred - (lapi_long_t) req->addr;
    xfer_struct.HwXfer.tgt_offset = remote_offset;
    source_offset = ((local_p_to_long + nbytes_transferred) - (lapi_long_t) fh_req->addr);
    xfer_struct.HwXfer.src_offset = source_offset;

    xfer_struct.HwXfer.Xfer_type = LAPI_RDMA_XFER;
    xfer_struct.HwXfer.tgt = info->node;
    xfer_struct.HwXfer.op = info->op;
    if(info->op == LAPI_RDMA_GET) {
      xfer_struct.HwXfer.rdma_tag = GASNETC_LAPI_RDMA_GET_TAG;
    } else {
      xfer_struct.HwXfer.rdma_tag = GASNETC_LAPI_RDMA_PUT_TAG;
    }
    xfer_struct.HwXfer.remote_cxt = info->rcxt.usr_rcxt;
    xfer_struct.HwXfer.len = info->ask_bytes;
    xfer_struct.HwXfer.shdlr = gasnete_lapi_release_remote_local_firehose;
    {
      gasnete_lapi_firehose_free_t *f_info = (gasnete_lapi_firehose_free_t *) gasneti_malloc(sizeof(gasnete_lapi_firehose_free_t));
      f_info->loc_fh = fh_req;
      f_info->rem_fh = req;

      /* For freeing the remote firehose request only once */
      f_info->counter_p = transfer_water_mark_p;
      fetch_and_add(transfer_water_mark_p, 1);

      xfer_struct.HwXfer.sinfo = (void *) f_info;
    xfer_struct.HwXfer.org_cntr = info->eop->origin_counter;
      
    /* Do the transfer */
    GASNETC_LCHECK (LAPI_Xfer (gasnetc_lapi_context, &xfer_struct));
    
    }
    
    /* Bump up counters */
    nbytes_transferred += transfer_len;
    remaining_bytes -= transfer_len;
    fetch_and_add(info->num_transfers_p,1);
    fetch_and_addlp(info->initiated_bytes_ptr, transfer_len);    
  gasneti_free(context);
}
#endif /* GASNET_SEGMENT_EVERYTHING */

extern gasnete_eop_t *gasnete_lapi_do_rdma(void *dest, gasnet_node_t node, void *origin, size_t nbytes, int op, lapi_cntr_t *origin_counter, gasnete_iop_t *iop GASNETE_THREAD_FARG)
{
  lapi_long_t remote_p_to_long;
  lapi_remote_cxt_t rcxt;
  lapi_xfer_t xfer_struct;	/* From the LAPI docs, the structure holding all the information needed for an RDMA */
  int total_transfers = 0;
  size_t nbytes_transferred = 0;
  int first_call;
  int transfer_len;
  lapi_long_t local_p_to_long;
  lapi_user_pvo_t remote_pvo, source_pvo;
  int allocated_tag;
  gasnetc_lapi_pvo *pvo_container = NULL;
  lapi_cntr_t *cptr;
  gasnete_eop_t *new_eop;
  int using_network_buffer = 0;
  int length_to_boundary, length_to_remote_boundary, chunk_remaining, source_offset, remote_offset;
  int rctxt_index;
  gasnete_lapi_nb *nb_id = NULL;

  /* Get an rctxt for this peer, round robin */
     
  rctxt_index = fetch_and_add(gasnetc_lapi_current_rctxt + node, 1) % gasnetc_rctxts_per_node;
  rcxt = gasnetc_remote_ctxts[node][rctxt_index];

  if(op == LAPI_RDMA_GET) {
    remote_p_to_long = (lapi_long_t) origin;
    local_p_to_long = (lapi_long_t) dest;
  } else {
    remote_p_to_long = (lapi_long_t) dest;
    local_p_to_long = (lapi_long_t) origin;
    gasneti_sync_reads();
  }

  /* Create an eop for this operation */
  new_eop = gasnete_eop_new(GASNETE_MYTHREAD);
  new_eop->origin_counter = NULL;
  new_eop->initiated_cnt = 0;

  GASNETI_TRACE_PRINTF(C,("gasnete_lapi_do_rdma: dest = %ld node = %d size = %ld op = %s\n",(uint64_t) remote_p_to_long, node, nbytes, op == LAPI_RDMA_GET ? "GET" : "PUT"));
  

  if(iop == NULL) {
    if(origin_counter != NULL) {
      new_eop->origin_counter = origin_counter;
    } else {
      new_eop->origin_counter = &(new_eop->cntr);
    }
    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context, new_eop->origin_counter, 0));
  } else {
    if(origin_counter != NULL) {
      new_eop->origin_counter = origin_counter;
    } else {
      if(op == LAPI_RDMA_GET) {
        /* One typically doesn't pass iops between threads, right?
    	 So this should be safe */
        new_eop->origin_counter = &(iop->get_cntr);
      } else {
        new_eop->origin_counter = &(iop->put_cntr);
      }
    }
  }

  cptr = NULL;

  /* Cannot do an RDMA with yourself.  For now do a memcpy and fake everything else */
  if(node == gasneti_mynode) {
    memcpy(dest,origin,nbytes);
    if(iop == NULL) {
      new_eop->origin_counter = NULL;
      new_eop->local_p = 1;
    }
    new_eop->num_transfers = 0;
    return(new_eop);
  } else {
    if(iop == NULL) {
      new_eop->local_p = 0;
    }
  }
  

#if GASNET_SEGMENT_EVERYTHING
  /* Be driven by remote pinning */

  lapi_long_t remaining_bytes = nbytes;
  nbytes_transferred = 0;
  const firehose_request_t *fh_req;
  int64_t initiated_bytes = 0;

  while(nbytes_transferred < nbytes) {
    /* Try to pin remote piece */

    lapi_long_t ask_bytes = MIN(remaining_bytes, gasnetc_firehose_info.max_RemotePinSize - ((remote_p_to_long + nbytes_transferred) & (GASNETI_PAGESIZE-1)));
    /* Trim for local */
    ask_bytes = MIN(ask_bytes,gasnetc_firehose_info.max_LocalPinSize - ((local_p_to_long + nbytes_transferred) & (GASNETI_PAGESIZE-1)));
    gasneti_assert(ask_bytes > 0);

    /* Do everything in the completion callback, for simplicity */
    gasnete_lapi_transfer_info *info = (gasnete_lapi_transfer_info *) gasneti_malloc(sizeof(gasnete_lapi_transfer_info));
    info->local_p = local_p_to_long;
    info->offset = nbytes_transferred;
    info->remote_p = remote_p_to_long;
    info->ask_bytes = ask_bytes;
    info->initiated_bytes_ptr = &initiated_bytes;
    info->num_transfers_p = &total_transfers;
    info->op = op;
    info->node = node;
    info->rcxt = rcxt;
    /* Pin local piece */
    info->fh_loc = firehose_local_pin(local_p_to_long + nbytes_transferred, ask_bytes,NULL);
    fetch_and_add(&current_local_count,1);
    info->transfer_len = ask_bytes;
    info->eop = new_eop;

    info->tag = pin_tag;
    pin_tag++;

    firehose_remote_pin(node, remote_p_to_long + nbytes_transferred, ask_bytes,
                        0, NULL,
                        NULL,
                        gasnete_lapi_complete_transfer, info);

    nbytes_transferred += ask_bytes;
    remaining_bytes -= ask_bytes;
  }


  /* Wait for all initiations to complete before proceeding
     because of a potential race if you try to sync on the handle
     before you even send stuff out */
  while(fetch_and_addlp(&initiated_bytes,0) != nbytes) {
    gasnetc_AMPoll();
  }
#else
  /* Clean out the descriptor */
  memset ((void *) &xfer_struct, 0, sizeof (xfer_struct));
  /* If the transfer is really small (for some definition of "really small") */

  GASNETI_TRACE_PRINTF(C,("gasnete_lapi_do_rdma: nbytes = %ld threshold = %d\n",nbytes,gasnete_pin_threshold));

  if(!gasnetc_use_firehose && (!(((local_p_to_long >= gasnetc_segbase_table[gasneti_mynode])
      && (local_p_to_long < gasnetc_segbase_table[gasneti_mynode] + gasneti_seginfo[gasneti_mynode].size))) && nbytes <= gasnete_pin_threshold)) {
      using_network_buffer = 1;
      /* Get a free buffer */
      GASNETI_TRACE_PRINTF(C,("gasnete_lapi_do_rdma: looking for network buffer\n"));
      nb_id = gasnete_get_free_network_buffer();
      /* Copy in for puts */
      if(op != LAPI_RDMA_GET) {
        memcpy(nb_id->data,(void *) local_p_to_long,nbytes);
      }

      nb_id->user_buffer = (void *) local_p_to_long;
      nb_id->user_length = nbytes;
      nb_id->get_p = (op == LAPI_RDMA_GET);
      nb_id->origin_counter = new_eop->origin_counter;
  } 

  /* Do something special if the origin is within the pinned segment or we can use a network buffer */
  if (using_network_buffer || ((local_p_to_long >= gasnetc_segbase_table[gasneti_mynode])
      && (local_p_to_long < gasnetc_segbase_table[gasneti_mynode] + gasneti_seginfo[gasneti_mynode].size))) {

    /* No need to pin origin, but you need to deal with misalignment 
       (in terms of the PVO boundaries)
       of the pinned regions of the origin and destination */

    while(nbytes_transferred < nbytes) {

      /* The number of bytes to either the end of the current PVO region
         or the end of the data */


      if(using_network_buffer) {
        GASNETI_TRACE_PRINTF(C,("gasnete_lapi_do_rdma: using network buffer nbytes = %ld\n",nbytes));
        chunk_remaining = length_to_boundary = nbytes;
        source_offset = nb_id->offset;
        source_pvo = nb_id->pvo;
      } else {
        length_to_boundary = (int) MIN(GASNETC_LAPI_PVO_EXTENT - ((local_p_to_long + nbytes_transferred - gasnetc_segbase_table[gasneti_mynode]) % GASNETC_LAPI_PVO_EXTENT),
		     nbytes - nbytes_transferred);
      
        /* Try to transfer this chunk of bytes.  It will either take
            one or two RDMA calls depending on whether or not it is entirely
            within a single PVO region at the target */
       
        chunk_remaining = length_to_boundary;
        source_offset = ((local_p_to_long + nbytes_transferred - gasnetc_segbase_table[gasneti_mynode]) % GASNETC_LAPI_PVO_EXTENT);
        source_pvo = gasnetc_pvo_table[(local_p_to_long + nbytes_transferred - gasnetc_segbase_table[gasneti_mynode])/
				         GASNETC_LAPI_PVO_EXTENT][gasneti_mynode];  
      }

      first_call=1; 
      do {

	remote_pvo = gasnetc_pvo_table[(remote_p_to_long + nbytes_transferred -
				       gasnetc_segbase_table[node]) / GASNETC_LAPI_PVO_EXTENT][node];
	length_to_remote_boundary = MIN(GASNETC_LAPI_PVO_EXTENT - ((remote_p_to_long + nbytes_transferred - gasnetc_segbase_table[node]) 
							      % GASNETC_LAPI_PVO_EXTENT),chunk_remaining);

        remote_offset = ((remote_p_to_long + nbytes_transferred - gasnetc_segbase_table[node]) % GASNETC_LAPI_PVO_EXTENT);

        if(using_network_buffer && first_call) {
          first_call=0;
          if(length_to_remote_boundary != nbytes) {
            nb_id->num_waiting = 2;
          } else {
            nb_id->num_waiting = 1;
          }
        }
	/* Send this off now */

	xfer_struct.HwXfer.src_pvo = source_pvo;
	xfer_struct.HwXfer.src_offset = source_offset;
	
	xfer_struct.HwXfer.tgt_pvo = remote_pvo;
	xfer_struct.HwXfer.tgt_offset = remote_offset;
	
	
	xfer_struct.HwXfer.Xfer_type = LAPI_RDMA_XFER;
	xfer_struct.HwXfer.tgt = node;
	xfer_struct.HwXfer.op = op;
	if(op == LAPI_RDMA_GET) {
	  xfer_struct.HwXfer.rdma_tag = GASNETC_LAPI_RDMA_GET_TAG;
	} else {
	  xfer_struct.HwXfer.rdma_tag = GASNETC_LAPI_RDMA_PUT_TAG;
	}
	xfer_struct.HwXfer.remote_cxt = rcxt.usr_rcxt;
	xfer_struct.HwXfer.len = length_to_remote_boundary;
        if(using_network_buffer) {
	  xfer_struct.HwXfer.shdlr = gasnete_lapi_reap_network_buffer;
	  xfer_struct.HwXfer.sinfo = (void *) nb_id;
	  xfer_struct.HwXfer.org_cntr = NULL;
        } else {
	  xfer_struct.HwXfer.shdlr = (scompl_hndlr_t *) NULL;
	  xfer_struct.HwXfer.sinfo = (void *) NULL;
	  xfer_struct.HwXfer.org_cntr = new_eop->origin_counter;
        }
        GASNETI_TRACE_PRINTF(C,("gasnete_lapi_do_rdma: transfer length = %d nbytes_transferred = %ld bytes remaining = %ld remote_offset=%d real remote offfset=%ld segment start = %ld GASNETC_LAPI_PVO_EXTENT=%ld offset in remote pvo table = %ld tag=%d remote pvo=%ld get = %d source_offset=%d real source offset=%ld offset in local pvo table=%ld\n",length_to_remote_boundary, nbytes_transferred, (uint64_t) chunk_remaining,remote_offset,(uint64_t) (remote_p_to_long + nbytes_transferred-gasnetc_segbase_table[node]),(uint64_t) gasnetc_segbase_table[node],GASNETC_LAPI_PVO_EXTENT,(uint64_t) (remote_p_to_long + nbytes_transferred-gasnetc_segbase_table[node])/GASNETC_LAPI_PVO_EXTENT, xfer_struct.HwXfer.rdma_tag,(uint64_t) remote_pvo,op == LAPI_RDMA_GET,source_offset, (uint64_t) (local_p_to_long + nbytes_transferred - gasnetc_segbase_table[gasneti_mynode]),(uint64_t) (local_p_to_long + nbytes_transferred - gasnetc_segbase_table[gasneti_mynode])/GASNETC_LAPI_PVO_EXTENT));
        
	GASNETC_LCHECK (LAPI_Xfer (gasnetc_lapi_context, &xfer_struct));
       
	total_transfers++;
	nbytes_transferred += length_to_remote_boundary;
	chunk_remaining -= length_to_remote_boundary;
	source_offset += length_to_remote_boundary;
      } while (chunk_remaining > 0);
      
    }
             
  } else {
    if(gasnetc_use_firehose) {
      lapi_long_t remaining_bytes = nbytes;
      const firehose_request_t *fh_req;
      while (nbytes_transferred < nbytes) {
        /* Try to pin local piece */
        /* Deal with alignment */
        lapi_long_t ask_bytes = MIN(remaining_bytes, gasnetc_firehose_info.max_LocalPinSize - ((local_p_to_long + nbytes_transferred) & (GASNETI_PAGESIZE-1)));

        /* Ask for less because of page size rounding up */
        fh_req  = firehose_local_pin(local_p_to_long + nbytes_transferred, ask_bytes,NULL);

        /* This is how much I can do for now */
        transfer_len = MIN(ask_bytes, fh_req->len - (local_p_to_long + nbytes_transferred - (lapi_long_t) fh_req->addr));

        /* These are the PVOs */ 
	xfer_struct.HwXfer.src_pvo = fh_req->client.pvo;
	xfer_struct.HwXfer.tgt_pvo =
	  gasnetc_pvo_table[(remote_p_to_long + nbytes_transferred -
	    gasnetc_segbase_table[node]) /
	   GASNETC_LAPI_PVO_EXTENT][node];

        /* Get source and target offsets */
        remote_offset = ((remote_p_to_long + nbytes_transferred - gasnetc_segbase_table[node]) % GASNETC_LAPI_PVO_EXTENT);
	xfer_struct.HwXfer.tgt_offset = remote_offset;

        /* Update because of remote alignment issues */
        transfer_len = MIN(transfer_len, GASNETC_LAPI_PVO_EXTENT - remote_offset);
  	source_offset = ((local_p_to_long + nbytes_transferred) - (lapi_long_t) fh_req->addr);
	xfer_struct.HwXfer.src_offset = source_offset;
        xfer_struct.HwXfer.Xfer_type = LAPI_RDMA_XFER;
        xfer_struct.HwXfer.tgt = node;
        xfer_struct.HwXfer.op = op;
        if(op == LAPI_RDMA_GET) {
	  xfer_struct.HwXfer.rdma_tag = GASNETC_LAPI_RDMA_GET_TAG;
        } else {
  	  xfer_struct.HwXfer.rdma_tag = GASNETC_LAPI_RDMA_PUT_TAG;
        }
        xfer_struct.HwXfer.remote_cxt = rcxt.usr_rcxt;
        xfer_struct.HwXfer.len = transfer_len;
        xfer_struct.HwXfer.shdlr = gasnete_lapi_release_firehose;
        xfer_struct.HwXfer.sinfo = (void *) fh_req;
        xfer_struct.HwXfer.org_cntr = new_eop->origin_counter;
      
        /* Do the transfer */
        GASNETC_LCHECK (LAPI_Xfer (gasnetc_lapi_context, &xfer_struct));
        total_transfers++;
        nbytes_transferred += transfer_len;
        remaining_bytes -= transfer_len; 
      }
    } else {
      first_call = 1;
      while (nbytes_transferred < nbytes) {
        /* Compute the number of bytes to be transferred in this step.  Try
         * to align the local and remote sides so that the maximum amount of data
         * is transferred with each call */
      
        /* Do this first to put a brake on pinning */
        pvo_container = gasnetc_lapi_new_pvo();
        if (first_call) {
      	  /* Transfer only up to the next PVO boundary on the remote side */
          remote_offset = ((remote_p_to_long + nbytes_transferred - gasnetc_segbase_table[node]) % GASNETC_LAPI_PVO_EXTENT);
  	  source_offset = 0;
	  transfer_len = MIN (nbytes - nbytes_transferred, GASNETC_LAPI_PVO_EXTENT - remote_offset);
	  xfer_struct.HwXfer.src_pvo = gasnetc_lapi_get_local_pvo(local_p_to_long + nbytes_transferred, transfer_len);
          pvo_container->len = transfer_len;
	  xfer_struct.HwXfer.tgt_pvo =
	    gasnetc_pvo_table[(remote_p_to_long + nbytes_transferred -
			    gasnetc_segbase_table[node]) /
			   GASNETC_LAPI_PVO_EXTENT][node];
	  xfer_struct.HwXfer.tgt_offset = remote_offset;
	  xfer_struct.HwXfer.src_offset = source_offset;
        } else {
	  /* Can now transfer in chunks of size GASNETC_LAPI_PVO_EXTENT */
	  transfer_len = MIN (GASNETC_LAPI_PVO_EXTENT, nbytes - nbytes_transferred);
	  xfer_struct.HwXfer.src_pvo = gasnetc_lapi_get_local_pvo (local_p_to_long + nbytes_transferred, transfer_len);
          pvo_container->len = transfer_len;
	  xfer_struct.HwXfer.tgt_pvo = gasnetc_pvo_table[(remote_p_to_long + nbytes_transferred -
						       gasnetc_segbase_table[node]) / GASNETC_LAPI_PVO_EXTENT][node];
	  xfer_struct.HwXfer.tgt_offset = 0;
	  xfer_struct.HwXfer.src_offset = 0;
        }

        pvo_container->pvo = xfer_struct.HwXfer.src_pvo;
        first_call = 0;
        xfer_struct.HwXfer.Xfer_type = LAPI_RDMA_XFER;
        xfer_struct.HwXfer.tgt = node;
        xfer_struct.HwXfer.op = op;
        if(op == LAPI_RDMA_GET) {
	  xfer_struct.HwXfer.rdma_tag = GASNETC_LAPI_RDMA_GET_TAG;
        } else {
  	  xfer_struct.HwXfer.rdma_tag = GASNETC_LAPI_RDMA_PUT_TAG;
        }
        xfer_struct.HwXfer.remote_cxt = rcxt.usr_rcxt;
        xfer_struct.HwXfer.len = transfer_len;
        xfer_struct.HwXfer.shdlr = gasnete_lapi_reap_pvo;
        xfer_struct.HwXfer.sinfo = (void *) pvo_container;
        xfer_struct.HwXfer.org_cntr = new_eop->origin_counter;
      
        /* Do the transfer */
        GASNETC_LCHECK (LAPI_Xfer (gasnetc_lapi_context, &xfer_struct));
        total_transfers++;
        nbytes_transferred += transfer_len;
      }
    }
  }
#endif /* GASNET_SEGMENT_EVERYTHING */
  /* Return an eop  with all the required information */
  new_eop->num_transfers = total_transfers;
  if(iop == NULL) {
    return(new_eop);
  } else {
    if(op == LAPI_RDMA_GET) {
      iop->initiated_get_cnt += total_transfers;
    } else {
      iop->initiated_put_cnt += total_transfers;
    }
    return(new_eop);
  }
}
#endif

/* ------------------------------------------------------------------------------------ */
extern void gasnete_get_bulk (void *dest, gasnet_node_t node, void *src,
			      size_t nbytes GASNETE_THREAD_FARG)
{
    lapi_cntr_t c_cntr;
    int num_get = 0;
    int cur_cntr;

#if GASNETC_LAPI_RDMA
    if_pt(gasnetc_lapi_use_rdma) {
      gasnete_eop_t *eop;
      GASNETI_TRACE_PRINTF(C,("gasnete_get_bulk\n"));
      GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context, &c_cntr, 0));
      /* gasneti_suspend_spinpollers(); */
      eop = gasnete_lapi_do_rdma(dest,node,src,nbytes,LAPI_RDMA_GET, &c_cntr,NULL GASNETE_THREAD_GET);
      /* gasneti_resume_spinpollers(); */
      /* Wait on this eop */
      GASNETI_TRACE_PRINTF(C,("gasnete_get_bulk: wait on sync\n"));
      GASNETC_LCHECK((LAPI_Waitcntr(gasnetc_lapi_context, eop->origin_counter,eop->num_transfers,&num_get)));

      gasnete_op_free((gasnete_op_t *)eop);
    } else {
#endif
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
#if GASNETC_LAPI_RDMA
    }
#endif
}

/* ------------------------------------------------------------------------------------ */
extern void gasnete_put_bulk (gasnet_node_t node, void *dest, void *src,
			      size_t nbytes GASNETE_THREAD_FARG)
{
    lapi_cntr_t  c_cntr;
    int num_put = 0;
    int cur_cntr;

#if GASNETC_LAPI_RDMA
    if_pt(gasnetc_lapi_use_rdma) {
      gasnete_eop_t *eop;
      GASNETI_TRACE_PRINTF(C,("gasnete_put_bulk target node=%d\n",node));
      GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context, &c_cntr, 0));
      /* gasneti_suspend_spinpollers(); */
      eop = gasnete_lapi_do_rdma(dest,node,src,nbytes,LAPI_RDMA_PUT, &c_cntr,NULL GASNETE_THREAD_GET);
      /* gasneti_resume_spinpollers(); */
      /* Wait on this eop */
      GASNETI_TRACE_PRINTF(C,("gasnete_put_bulk: wait on sync\n"));
      GASNETC_LCHECK((LAPI_Waitcntr(gasnetc_lapi_context, eop->origin_counter,eop->num_transfers,&num_put)));
      GASNETI_TRACE_PRINTF(C,("gasnete_put_bulk: wait done\n"));

      gasnete_op_free((gasnete_op_t *)eop);
    } else {
#endif
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
#if GASNETC_LAPI_RDMA
   }
#endif
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
#if GASNETC_LAPI_RDMA
    if_pt(gasnetc_lapi_use_rdma) {
      /* Need to clean up the calling sequence here */
      gasnete_eop_t *eop;
      GASNETI_TRACE_PRINTF(C,("gasnete_get_nb_bulk\n"));
      /* gasneti_suspend_spinpollers(); */
      eop = gasnete_lapi_do_rdma(dest,node,src,nbytes,LAPI_RDMA_GET, NULL, NULL GASNETE_THREAD_GET);
      /* gasneti_resume_spinpollers(); */
      return((gasnet_handle_t) eop);
    } else {
#endif
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
#if GASNETC_LAPI_RDMA
   }
#endif
}

/* ------------------------------------------------------------------------------------ */
extern gasnet_handle_t gasnete_put_nb_bulk (gasnet_node_t node, void *dest, void *src,
					    size_t nbytes GASNETE_THREAD_FARG)
{
#if GASNETC_LAPI_RDMA
    if_pt(gasnetc_lapi_use_rdma) {
      gasnete_eop_t *eop;
      GASNETI_TRACE_PRINTF(C,("gasnete_put_nb_bulk\n"));
      /* gasneti_suspend_spinpollers(); */
      eop = gasnete_lapi_do_rdma(dest,node,src,nbytes,LAPI_RDMA_PUT, NULL, NULL GASNETE_THREAD_GET);
      /* gasneti_resume_spinpollers(); */
      /* Don't wait for source completion */
      return((gasnet_handle_t) eop);
    } else {
#endif
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
#if GASNETC_LAPI_RDMA
   }
#endif
}

/* ------------------------------------------------------------------------------------ */
extern gasnet_handle_t gasnete_put_nb (gasnet_node_t node, void *dest, void *src,
				       size_t nbytes GASNETE_THREAD_FARG)
{
#if GASNETC_LAPI_RDMA
    if_pt(gasnetc_lapi_use_rdma) {
      gasnete_eop_t *eop;
      int cur_cntr;
      GASNETI_TRACE_PRINTF(C,("gasnete_put_nb\n"));
      /* gasneti_suspend_spinpollers(); */
      eop = gasnete_lapi_do_rdma(dest,node,src,nbytes,LAPI_RDMA_PUT, NULL, NULL GASNETE_THREAD_GET);
      /* gasneti_resume_spinpollers(); */
      /* Wait for the origin counter to indicate local completion */
      if(!eop->local_p) {
        GASNETC_WAITCNTR(eop->origin_counter,eop->num_transfers,&cur_cntr);
        eop->num_transfers = 0;
        gasneti_assert(cur_cntr == 0);
      }
      return((gasnet_handle_t) eop);
    } else {
#endif
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
#if GASNETC_LAPI_RDMA
  }
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
#if GASNETC_LAPI_RDMA
    if_pt(gasnetc_lapi_use_rdma) {
    /* gasneti_suspend_spinpollers(); */
    op->num_transfers = 1;
    op->local_p = 0;
    op->origin_counter = &(op->cntr);
    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context, op->origin_counter, 0));
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, (unsigned int)node,
			       gasnete_remote_memset_hh[node],
			       &uhdr, sizeof(gasnete_memset_uhdr_t), NULL, 0,
			       NULL, &o_cntr, op->origin_counter));
    /* gasneti_resume_spinpollers(); */
    } else {
#endif
    gasneti_suspend_spinpollers();
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, (unsigned int)node,
			       gasnete_remote_memset_hh[node],
			       &uhdr, sizeof(gasnete_memset_uhdr_t), NULL, 0,
			       NULL, &o_cntr, &op->cntr));
    gasneti_resume_spinpollers();
   
    op->initiated_cnt++;
#if GASNETC_LAPI_RDMA
    }
#endif
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
#if GASNETC_LAPI_RDMA
        gasnete_wait_syncnb(handle); /* PH: reset it, wait_syncnb frees it */
#else
	gasnete_op_free(handle); 
#endif
	return GASNET_OK;
    }
    else return GASNET_ERR_NOT_READY;
}

extern int gasnete_try_syncnb_some (gasnet_handle_t *phandle, size_t numhandles) {
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
#if GASNETC_LAPI_RDMA
                gasnete_wait_syncnb(op); /* PH: reset it */
#else
		gasnete_op_free(op);
#endif
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
#if GASNETC_LAPI_RDMA
                gasnete_wait_syncnb(op); /* PH: reset it */
#else
		gasnete_op_free(op);
#endif
		phandle[i] = GASNET_INVALID_HANDLE;
	    } else success = 0;
	}
    }
    }

    if (success) return GASNET_OK;
    else return GASNET_ERR_NOT_READY;
}

#if GASNETC_LAPI_RDMA
extern void gasnete_wait_syncnb_original(gasnet_handle_t handle);
extern void gasnete_wait_syncnb(gasnet_handle_t handle)
{
    if_pt(gasnetc_lapi_use_rdma) {
    gasnete_op_t *op = handle;
    int cnt = 0;
    int cnt2 = 0;
    gasneti_assert(op->threadidx == gasnete_mythread()->threadidx);
    if (op == GASNET_INVALID_HANDLE) {
      return;
    }

    if_pt (OPTYPE(op) == OPTYPE_EXPLICIT) {
	gasnete_eop_t *eop = (gasnete_eop_t*)op;
	gasneti_assert(OPSTATE(op) != OPSTATE_FREE);
        gasnete_eop_check(eop);
        if(eop->local_p) {
          gasneti_assert(eop->initiated_cnt == 0);
          goto END;
        }
	if (eop->initiated_cnt > 0) {
	    GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&eop->cntr,eop->initiated_cnt,&cnt));
	    gasneti_assert(cnt == 0);
	    eop->initiated_cnt =0;
            goto END;
	}
          GASNETC_LCHECK((LAPI_Waitcntr(gasnetc_lapi_context, eop->origin_counter,eop->num_transfers,&cnt)));
          gasneti_assert(cnt == 0);
    } else {
	gasnete_iop_t *iop = (gasnete_iop_t*)op;
        gasnete_iop_check(iop);
   
      GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&iop->get_cntr,iop->initiated_get_cnt,&cnt));
      GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&iop->put_cntr,iop->initiated_put_cnt,&cnt2));
      /* Got to do this because put_cntr is decremented */
      iop->initiated_put_cnt=0;
      iop->initiated_get_cnt=0;

      if (gasneti_weakatomic_read(&iop->get_aux_cntr, 0)) /* avoid extra rmb when possible */
        GASNET_BLOCKUNTIL(gasneti_weakatomic_read(&iop->get_aux_cntr, 0) == 0);
      if (gasneti_weakatomic_read(&iop->put_aux_cntr, 0)) /* avoid extra rmb when possible */
        GASNET_BLOCKUNTIL(gasneti_weakatomic_read(&iop->put_aux_cntr, 0) == 0);

    }
END:
    gasneti_sync_reads();
    gasnete_op_free(handle);
  
    } else {
      gasnete_wait_syncnb_original(handle);
    }
}

extern void gasnete_wait_syncnbi_gets(GASNETE_THREAD_FARG_ALONE) 
{
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t *iop = mythread->current_iop;
    int cnt = 0;
    gasneti_assert(iop->threadidx == mythread->threadidx);
    gasneti_assert(iop->next == NULL);
    gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);
#if GASNET_DEBUG
    if (iop->next != NULL)
	gasneti_fatalerror("VIOLATION: attempted to call gasnete_wait_syncnbi_gets() inside an NBI access region");
#endif

    GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&iop->get_cntr,iop->initiated_get_cnt,&cnt));
    gasneti_assert(cnt == 0);
    iop->initiated_get_cnt = 0;
    if (gasneti_weakatomic_read(&iop->get_aux_cntr, 0)) /* avoid extra rmb when possible */
      GASNET_BLOCKUNTIL(gasneti_weakatomic_read(&iop->get_aux_cntr, 0) == 0);
    /* gasneti_sync_mem(); */  /* Don't think this is necessary, PH */
}

#endif

#if !GASNETC_LAPI_RDMA && GASNETC_LAPI_FED_POLLBUG_WORKAROUND
extern void gasnete_wait_syncnbi_puts(GASNETE_THREAD_FARG_ALONE) 
{
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
    GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&iop->put_cntr,iop->initiated_put_cnt,&cnt));
    gasneti_assert(cnt == 0);
    iop->initiated_put_cnt = 0;
    if (gasneti_weakatomic_read(&iop->put_aux_cntr, 0)) /* avoid extra rmb when possible */
      GASNET_BLOCKUNTIL(gasneti_weakatomic_read(&iop->put_aux_cntr, 0) == 0);
    gasneti_sync_mem();
}
#endif


#if GASNETC_LAPI_RDMA
extern void gasnete_wait_syncnb_original(gasnet_handle_t handle) {
#elif GASNETC_LAPI_FED_POLLBUG_WORKAROUND
extern void gasnete_wait_syncnb(gasnet_handle_t handle) {
#endif
#if GASNETC_LAPI_RDMA || GASNETC_LAPI_FED_POLLBUG_WORKAROUND
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

extern void gasnete_wait_syncnb_all(gasnet_handle_t *phandle, size_t numhandles)
{
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

#if 0
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

#if GASNETC_LAPI_RDMA
    if_pt(gasnetc_lapi_use_rdma) {
    gasnete_eop_t *eop;
    GASNETI_TRACE_PRINTF(C,("gasnete_get_nbi_bulk\n"));
    /* gasneti_suspend_spinpollers(); */
    eop = gasnete_lapi_do_rdma(dest,node,src,nbytes,LAPI_RDMA_GET, NULL, op GASNETE_THREAD_GET);
    eop->origin_counter = NULL;
    gasnete_op_free((gasnete_op_t *) eop); 
    /* gasneti_resume_spinpollers(); */
   } else {
#endif
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
#if GASNETC_LAPI_RDMA
    }
#endif
}

/* ------------------------------------------------------------------------------------ */
extern void gasnete_put_nbi_bulk (gasnet_node_t node, void *dest, void *src,
				  size_t nbytes GASNETE_THREAD_FARG)
{
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t *op = mythread->current_iop;
#if GASNETC_LAPI_RDMA
    if_pt(gasnetc_lapi_use_rdma) {
    gasnete_eop_t *eop;
    /* gasneti_suspend_spinpollers(); */
    eop = gasnete_lapi_do_rdma(dest,node,src,nbytes,LAPI_RDMA_PUT, NULL, op GASNETE_THREAD_GET);
    eop->origin_counter = NULL;
    gasnete_op_free((gasnete_op_t *) eop); 
    /* gasneti_resume_spinpollers(); */
    } else {
#endif
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
#if GASNETC_LAPI_RDMA
    }
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
#if GASNETC_LAPI_RDMA
    if(gasnetc_lapi_use_rdma) {
    gasnete_eop_t *result;
    GASNETI_TRACE_PRINTF(C,("gasnete_put_nbi\n"));
    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context, &o_cntr, 0));
    /* gasneti_suspend_spinpollers(); */
    result = gasnete_lapi_do_rdma(dest,node,src,nbytes,LAPI_RDMA_PUT, &o_cntr, op GASNETE_THREAD_GET);
    /* gasneti_resume_spinpollers(); */
    GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,result->origin_counter,result->num_transfers,&cur_cntr));
    /* Because this actually completes the operation, act like it never happened and decrement the
       put count */
    op->initiated_put_cnt-=result->num_transfers;
    result->num_transfers = 0;
    result->origin_counter = NULL;
    gasnete_op_free((gasnete_op_t *) result); 
    gasneti_assert(cur_cntr == 0);
    } else {
#endif
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
#if GASNETC_LAPI_RDMA
    }
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
#if GASNETC_LAPI_RDMA
    if_pt(gasnetc_lapi_use_rdma) {
      GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, (unsigned int)node,
			       gasnete_remote_memset_hh[node],
			       &uhdr, sizeof(gasnete_memset_uhdr_t), NULL, 0,
			       NULL, &o_cntr, &(op->put_cntr)));
    } else {
#endif
    gasneti_suspend_spinpollers();
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, (unsigned int)node,
			       gasnete_remote_memset_hh[node],
			       &uhdr, sizeof(gasnete_memset_uhdr_t), NULL, 0,
			       NULL, &o_cntr, &op->put_cntr));
    gasneti_resume_spinpollers();
#if GASNETC_LAPI_RDMA
    }
#endif   
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
#if GASNETC_LAPI_RDMA
            if_pt(gasnetc_lapi_use_rdma) {
	    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&iop->get_cntr,0));
            iop->initiated_get_cnt = 0;
            } else {
#endif
            if (cnt > 65000) { /* make sure we don't overflow the counters */
	      GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&iop->get_cntr,0));
              iop->initiated_get_cnt = 0;
            }
#if GASNETC_LAPI_RDMA
           }
#endif
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

#if GASNETC_LAPI_RDMA
        if(gasnetc_lapi_use_rdma) {
        if (gasneti_weakatomic_read(&iop->put_aux_cntr, 0) > 0) return GASNET_ERR_NOT_READY;
        GASNETI_TRACE_PRINTF(C,("gasnete_try_syncnbi_puts\n"));
	GASNETC_LCHECK(LAPI_Getcntr(gasnetc_lapi_context,&iop->put_cntr,&cnt));
        if(iop->initiated_put_cnt == cnt) {
	    gasneti_sync_reads();
	    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&iop->put_cntr,0));
            iop->initiated_put_cnt = 0;
	    gasneti_sync_reads();
            return GASNET_OK;
        } else return GASNET_ERR_NOT_READY;
        } else {
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
#if GASNETC_LAPI_RDMA
        }
#endif
    }
}


/* don't poll for put operations, polling for gets is ok */
#if GASNETC_LAPI_RDMA
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

/* #define GASNETE_BARRIER_DEFAULT "LAPIGFENCE" See bug 2186 */
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

#if GASNETI_STATS_OR_TRACE
  static gasneti_tick_t barrier_notifytime;
#endif
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
