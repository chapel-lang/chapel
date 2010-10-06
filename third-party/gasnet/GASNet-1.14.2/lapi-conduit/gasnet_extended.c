/*   $Source: /var/local/cvs/gasnet/lapi-conduit/gasnet_extended.c,v $
 *     $Date: 2010/04/17 02:36:35 $
 * $Revision: 1.115.2.1 $
 * Description: GASNet Extended API Reference Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_extended_internal.h>

#ifdef GASNETC_LAPI_RDMA
extern int gasnetc_lapi_use_rdma;
#endif

static const gasnete_eopaddr_t EOPADDR_NIL = { { 0xFF, 0xFF } };
extern void _gasnete_iop_check(gasnete_iop_t *iop) { gasnete_iop_check(iop); }

/* ====================================================================
 * LAPI Structures and constants
 * ====================================================================
 */
void** gasnete_remote_memset_hh;
void** gasnete_remote_barrier_hh;

#if GASNETC_LAPI_RDMA
/* Bound the number of in-use PVOs.
 * Note that initial value is 0, but gasnete_new_threaddata() adds to it.
 */
static gasneti_semaphore_t gasnete_lapi_pvo_sema = GASNETI_SEMAPHORE_INITIALIZER(0,0);

extern firehose_info_t gasnetc_firehose_info;
#endif

#if GASNET_DEBUG
  /* assertions in free_op() expect the counters zeroed every time we sync */
  #define GASNETC_NBI_CNTR_LIMIT 0
#else
  /* just want to prevent counter overflow */
  #define GASNETC_NBI_CNTR_LIMIT 65000
#endif 

/* ------------------------------------------------------------------------------------ */
/*
  Extended API Common Code
  ========================
  Factored bits of extended API code common to most conduits, overridable when necessary
*/

#if GASNETC_LAPI_RDMA
    /* Increase the limit on in-flight PVOs */
  #define GASNETE_NEW_THREADDATA_CALLBACK(threaddata) \
    gasneti_semaphore_up_n(&gasnete_lapi_pvo_sema, GASNETC_MAX_PVOS_PER_THREAD)
#endif

#include "gasnet_extended_common.c"

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* called at startup to check configuration sanity */
static void gasnete_check_config(void) {
  gasneti_check_config_postattach();

  gasneti_assert_always(gasnete_eopaddr_isnil(EOPADDR_NIL));
  GASNETE_WIREFLAGS_SANITYCHECK();
}

extern void gasnete_init(void) {
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
        eop->origin_counter = NULL;
        eop->num_transfers = 0;
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
	int result = 1; /* Assume success if both counts are zero */
	gasneti_assert(OPSTATE(op) != OPSTATE_FREE);
        gasnete_eop_check(eop);

	if (eop->initiated_cnt > 0) {
	    GASNETC_LCHECK(LAPI_Getcntr(gasnetc_lapi_context,&eop->cntr,&cnt));
	    gasneti_assert(cnt <= eop->initiated_cnt);
	    result = (eop->initiated_cnt == cnt);
          #if GASNET_DEBUG
	    if (result) {
		/* To match wait_syncnb() behavior and thus satisfy assertion in op_free() */
		GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&eop->cntr,cnt,&eop->initiated_cnt));
	    }
          #endif
	}
#if GASNETC_LAPI_RDMA
        else if (eop->num_transfers > 0) {
	    GASNETC_LCHECK(LAPI_Getcntr(gasnetc_lapi_context,eop->origin_counter,&cnt));
	    gasneti_assert(cnt <= eop->num_transfers);
	    result = (eop->num_transfers == cnt);
          #if GASNET_DEBUG
	    if (result) {
		/* To match wait_syncnb() behavior and thus satisfy assertion in op_free() */
		GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,eop->origin_counter,cnt,&eop->num_transfers));
	    }
          #endif
        }
#endif
	return result;
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
	if (iop->initiated_put_cnt != cnt) {
	    return(0);
	}
#if GASNET_DEBUG
	/* To match wait_syncnb() behavior and thus satisfy assertions in op_free() */
	GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&iop->get_cntr,iop->initiated_get_cnt,&iop->initiated_get_cnt));
	GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&iop->put_cntr,iop->initiated_put_cnt,&iop->initiated_put_cnt));
#endif
	return (1);
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
        gasneti_assert(eop->initiated_cnt == 0);
#if GASNETC_LAPI_RDMA
        gasneti_assert(eop->num_transfers == 0);
#endif
        gasnete_eop_check(eop);
	SET_OPSTATE(eop, OPSTATE_FREE);
	eop->addr = thread->eop_free;
	thread->eop_free = addr;
    } else {
	gasnete_iop_t *iop = (gasnete_iop_t *)op;
        gasneti_assert(iop->initiated_get_cnt == 0);
        gasneti_assert(iop->initiated_put_cnt == 0);
        gasneti_assert(gasneti_weakatomic_read(&iop->get_aux_cntr, 0) == 0);
        gasneti_assert(gasneti_weakatomic_read(&iop->put_aux_cntr, 0) == 0);
        gasnete_iop_check(iop);
        gasneti_assert(iop->next == NULL);
	iop->next = thread->iop_free;
	thread->iop_free = iop;
    }
}


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

#if !GASNET_SEGMENT_EVERYTHING
#define gasnetc_lapi_find_pvo(_node, _offset) \
	(gasnetc_pvo_table[(_node)][(_offset) >> GASNETC_LAPI_PVO_EXTENT_BITS])

/* Pin like there's no tomorrow! */
static lapi_user_pvo_t gasnetc_lapi_get_local_pvo(lapi_long_t addr, size_t len)
{
	lapi_get_pvo_t new_pvo;
	new_pvo.Util_type = LAPI_XLATE_ADDRESS;
	new_pvo.length = len;
	new_pvo.usr_pvo = 0;
	new_pvo.address = (void *) addr;
	new_pvo.operation = LAPI_RDMA_ACQUIRE;
	/* Keep track of the PVOs and release the ones that have been around too long */
	GASNETC_LCHECK(LAPI_Util(gasnetc_lapi_context,(lapi_util_t *) &new_pvo));
	return(new_pvo.usr_pvo);
}

static void gasnetc_lapi_new_pvo(void)
{
  /* XXX: We once managed a free list of small wrapper structs here,
   * so se could pass the user_pvo and len to the LAPI send completion
   * handler.  Now we just pass the user_pvo in the sinfo field and
   * don't bother with the small struct.  HOWEVER, that only works
   * because void* and lapi_user_pvo_t are both 64 bits.
   * If we wish to extend LAPI-RDMA support to 32-bit builds, we'll
   * want to replace the semaphore with a gasneti_lifo_t to manage
   * small structs again (and this function will need to return them).
   * -PHH
   */
  if_pf (!gasneti_semaphore_trydown(&gasnete_lapi_pvo_sema)) {
     /* TODO: stats/trace the stall count and duration? -PHH */
     do {
       gasneti_AMPoll();
     } while (!gasneti_semaphore_trydown(&gasnete_lapi_pvo_sema));
   }
}

static void gasnetc_lapi_release_pvo(lapi_user_pvo_t pvo)
{
  lapi_get_pvo_t new_pvo;
  /* Unpin */
  new_pvo.Util_type = LAPI_XLATE_ADDRESS;
  new_pvo.length = 0;
  new_pvo.usr_pvo = pvo;
  new_pvo.address = 0;
  new_pvo.operation = LAPI_RDMA_RELEASE;
  GASNETC_LCHECK(LAPI_Util(gasnetc_lapi_context, (lapi_util_t *) &new_pvo)); 

  gasneti_semaphore_up(&gasnete_lapi_pvo_sema);
}

static gasnete_lapi_nb *gasnete_free_nb_list_original = NULL;
static char *gasnete_lapi_all_buffers = NULL;
static gasneti_lifo_head_t gasnete_free_nb_list = GASNETI_LIFO_INITIALIZER;
#define GASNETE_LAPI_NUM_NB 1024
static int gasnete_num_nb = GASNETE_LAPI_NUM_NB;
extern void gasnete_lapi_setup_nb(void)
{
  size_t total_pinned_region = gasnete_num_nb * gasnete_pin_threshold;
  char *all_data;
  size_t size_pinned_region = 0;
  lapi_get_pvo_t req;
  int count = 0;
  gasnete_free_nb_list_original = (gasnete_lapi_nb *) gasneti_malloc(gasnete_num_nb*sizeof(gasnete_lapi_nb));
  all_data = gasnete_lapi_all_buffers = (char *) gasneti_malloc(total_pinned_region);
  gasneti_assert(GASNETC_LAPI_PVO_EXTENT % gasnete_pin_threshold == 0);
  GASNETI_TRACE_PRINTF(C,("gasnete_lapi_setup_nb: pinned size = 0x%lx #buffers = %d",(ulong)total_pinned_region,gasnete_num_nb));
  while(size_pinned_region < total_pinned_region) {
    int this_region_size = MIN(GASNETC_LAPI_PVO_EXTENT, total_pinned_region - size_pinned_region);
    int num_slices,s;
    req.Util_type = LAPI_XLATE_ADDRESS;
    req.length = this_region_size;
    req.usr_pvo =0;
    req.address = all_data + size_pinned_region;
    req.operation = LAPI_RDMA_ACQUIRE;
    GASNETC_LCHECK(LAPI_Util(gasnetc_lapi_context, (lapi_util_t *) &req));
    GASNETI_TRACE_PRINTF(C,("gasnete_lapi_setup_nb: got pvo 0x%lx for network buffer",(ulong) req.usr_pvo)); 
    num_slices = this_region_size/gasnete_pin_threshold;
    for(s = 0; s < num_slices; s++) {
      gasnete_free_nb_list_original[count].data = all_data + size_pinned_region + s*gasnete_pin_threshold;
      gasnete_free_nb_list_original[count].offset = s*gasnete_pin_threshold;
      gasnete_free_nb_list_original[count].pvo = req.usr_pvo;
      if(count < gasnete_num_nb-1) {
        gasneti_lifo_link(&gasnete_free_nb_list_original[count], &gasnete_free_nb_list_original[count+1]);
      }
      count++;
    }
    size_pinned_region += this_region_size;
  }
  gasneti_lifo_push_many(&gasnete_free_nb_list, &gasnete_free_nb_list_original[0], &gasnete_free_nb_list_original[gasnete_num_nb-1]);
}

extern void gasnete_lapi_free_nb(void)
{
  int i;
  lapi_get_pvo_t req;
  for(i=0;i < gasnete_num_nb;i++) {
    /* Only release each PVO once.  The guy with offset == 0 is in some sense
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

static gasnete_lapi_nb *gasnete_get_free_network_buffer(void)
{
  gasnete_lapi_nb *ret = gasneti_lifo_pop(&gasnete_free_nb_list);

  if_pf (!ret) {
    /* TODO: stats/trace the stall count and duration? -PHH */
    int cnt = 0;
    do {
      /* Need to tune this? */
      gasneti_AMPoll();
      cnt++;
      if(cnt > 100000) {
        cnt = 0;
        printf("%d Spinning too much waiting for a network buffer?\n",gasneti_mynode);
      }
      ret = gasneti_lifo_pop(&gasnete_free_nb_list);
    } while (!ret);
  }

  return(ret);
}

static void gasnete_lapi_reap_network_buffer(lapi_handle_t *hndl, void *user_data, lapi_sh_info_t *info)
{
  gasnete_lapi_nb *nb_id = (gasnete_lapi_nb *) user_data;
  lapi_cntr_t *origin_counter = nb_id->origin_counter;

  if (gasneti_weakatomic_decrement_and_test(&(nb_id->num_waiting),GASNETI_ATOMIC_RMB_PRE)) {
    /* Copy out GET if needed*/
    if(nb_id->get_length) {
      memcpy(nb_id->get_buffer, nb_id->data, nb_id->get_length);
      gasneti_sync_writes();
    }

    /* Return this guy to the pool */
    gasneti_lifo_push(&gasnete_free_nb_list, nb_id);
  }

  /* Bump up the origin counter (ICK, loopback Put to get it incremented) */
  GASNETC_LCHECK(LAPI_Put(gasnetc_lapi_context, gasneti_mynode, 0,NULL, NULL, NULL, origin_counter, NULL));
}

static void gasnete_lapi_reap_pvo(lapi_handle_t *hndl, void *user_data, lapi_sh_info_t *info)
{
  gasneti_assert(sizeof(lapi_user_pvo_t) <= sizeof(void *));
  gasnetc_lapi_release_pvo((lapi_user_pvo_t) user_data);
}

static void gasnete_lapi_release_firehose(lapi_handle_t *hndl, void *user_data, lapi_sh_info_t *info)
{
  const firehose_request_t *fh_req = (firehose_request_t *) user_data;
  firehose_release(&fh_req,1);
}

#else /* !GASNET_SEGMENT_EVERYTHING */

static gasneti_lifo_head_t gasnete_lapi_transfer_info_freelist = GASNETI_LIFO_INITIALIZER;

typedef struct _gasnete_lapi_transfer_info_struct {
  lapi_long_t local_p;
  lapi_long_t remote_p;
  lapi_long_t nbytes;
  gasneti_weakatomic_t *num_transfers_p;
  int op;
  int node;
  lapi_user_cxt_t remote_cxt;
  lapi_cntr_t *origin_counter;
  const firehose_request_t *fh[2];
} gasnete_lapi_transfer_info;

static gasnete_lapi_transfer_info *gasnete_lapi_alloc_transfer_info(void)
{
  gasnete_lapi_transfer_info *ret = gasneti_lifo_pop(&gasnete_lapi_transfer_info_freelist);
  if_pf(!ret) {
    ret = gasneti_malloc(sizeof(gasnete_lapi_transfer_info));
  }
  return ret;
}

static void gasnete_lapi_release_transfer_info(lapi_handle_t *hndl, void *user_data, lapi_sh_info_t *info)
{
  gasnete_lapi_transfer_info *my_info = (gasnete_lapi_transfer_info *) user_data;
  firehose_release(my_info->fh, 2);
  gasneti_lifo_push(&gasnete_lapi_transfer_info_freelist, my_info);
}

/* Callback after remote stuff is pinned */
static void gasnete_lapi_complete_transfer(void *context, const firehose_request_t *req, int allLocalHit)
{
    lapi_xfer_t xfer_struct;
    gasnete_lapi_transfer_info *info = (gasnete_lapi_transfer_info *) context;
    lapi_long_t nbytes = info->nbytes;
    const firehose_request_t *fh_loc = info->fh[0];

    gasneti_assert((info->remote_p + nbytes) <= (req->addr + req->len));

    /* Build the xfer from info and the remote firehose req */
    xfer_struct.HwXfer.src_pvo = fh_loc->client.pvo;
    xfer_struct.HwXfer.src_offset = info->local_p - (lapi_long_t) fh_loc->addr;
    xfer_struct.HwXfer.tgt_pvo = req->client.pvo;
    xfer_struct.HwXfer.tgt_offset = info->remote_p - (lapi_long_t) req->addr;
    xfer_struct.HwXfer.Xfer_type = LAPI_RDMA_XFER;
    xfer_struct.HwXfer.tgt = info->node;
    xfer_struct.HwXfer.op = info->op;
    xfer_struct.HwXfer.remote_cxt = info->remote_cxt;
    xfer_struct.HwXfer.len = nbytes;
    xfer_struct.HwXfer.org_cntr = info->origin_counter;
      
    /* Setup for eventual release of info & firehoses */
    info->fh[1] = req;
    xfer_struct.HwXfer.shdlr = gasnete_lapi_release_transfer_info;
    xfer_struct.HwXfer.sinfo = (void *) info;

    {
      /* Fetch pointer now to avoid race against shdlr freeing info */
      gasneti_weakatomic_t *num_transfers_p = info->num_transfers_p;

      /* Do the transfer */
      GASNETC_LCHECK (LAPI_Xfer (gasnetc_lapi_context, &xfer_struct));
    
      gasneti_weakatomic_increment(num_transfers_p, 0);
   }
}
#endif /* GASNET_SEGMENT_EVERYTHING */

/* Note: Pointers are passed as remote/local, their identity as src/dest depends on op */
static gasnete_eop_t *gasnete_lapi_do_rdma(gasnet_node_t node, void *remote_ptr, void *local_ptr, const size_t nbytes, int op, lapi_cntr_t *origin_counter GASNETE_THREAD_FARG)
{
  lapi_long_t remaining_bytes = nbytes;
  lapi_remote_cxt_t rcxt;
  gasnete_eop_t *new_eop;
  int total_transfers = 0;
#if GASNET_SEGMENT_EVERYTHING
  lapi_long_t local_addr, remote_addr;
  gasneti_weakatomic_t initiated_xfers = gasneti_weakatomic_init(0); /* incremented by callback */
#else
  lapi_xfer_t xfer_struct;
  int transfer_len;
  int in_local_seg;
  int source_offset, remote_offset;
  lapi_long_t remote_segment_offset;
  gasnete_lapi_nb *nb_id = NULL;
#endif

  gasneti_assert(nbytes != 0);
  gasneti_assert(node != gasneti_mynode);

  /* Get an rctxt for this peer, round robin */
#if !defined(GASNETI_HAVE_WEAKATOMIC_ADD_SUB)
#error "lapi-conduit requires atomic add support"
#endif
  {
    gasneti_weakatomic_t *ctr = &(gasnetc_lapi_current_rctxt[node]);
    gasneti_weakatomic_val_t rctxt_index = (gasneti_weakatomic_add(ctr, 1, 0) & gasnetc_rctxts_per_node_mask);
    rcxt = gasnetc_remote_ctxts[node][rctxt_index];
  }

  /* XXX: This was here before me.  I won't remove it w/o first understanding it.
   * The CVS commit that first put it here says only "LAPI RDMA cleanup. No config changes yet."
   * -PHH
   */
  if(op == LAPI_RDMA_PUT) {
    gasneti_sync_reads();
  }

  /* Create an eop for this operation */
  new_eop = gasnete_eop_new(GASNETE_MYTHREAD);

  /* Default origin counter as needed */
  if(origin_counter == NULL) {
    origin_counter = &(new_eop->cntr);
  }
  new_eop->origin_counter = origin_counter;

  GASNETI_TRACE_PRINTF(C,("gasnete_lapi_do_rdma: loc = %p rem = %d:%p size = %ld op = %s\n", local_ptr, node, remote_ptr, nbytes, op == LAPI_RDMA_GET ? "GET" : "PUT"));
  
#if GASNET_SEGMENT_EVERYTHING
  /* Be driven by remote pinning */

  local_addr =  (lapi_long_t) local_ptr;
  remote_addr = (lapi_long_t) remote_ptr;

  do {
    gasnete_lapi_transfer_info *info = gasnete_lapi_alloc_transfer_info();

    /* Try to pin remote piece */
    lapi_long_t ask_bytes = MIN(remaining_bytes, gasnetc_firehose_info.max_RemotePinSize - (remote_addr & (GASNETI_PAGESIZE-1)));
    /* Trim for local */
    ask_bytes = MIN(ask_bytes,gasnetc_firehose_info.max_LocalPinSize - (local_addr & (GASNETI_PAGESIZE-1)));
    gasneti_assert(ask_bytes > 0);

    /* Do everything in the completion callback, for simplicity */
    info->local_p = local_addr;
    info->remote_p = remote_addr;
    info->nbytes = ask_bytes;
    info->num_transfers_p = &initiated_xfers;
    info->op = op;
    info->node = node;
    info->remote_cxt = rcxt.usr_rcxt;
    /* Pin local piece */
    info->fh[0] = firehose_local_pin(local_addr, ask_bytes,NULL);
    info->origin_counter = origin_counter;

    /* XXX: We would hide some of the AM latency if the remote pin request was first -PHH */
    firehose_remote_pin(node, remote_addr, ask_bytes,
                        0, NULL,
                        NULL,
                        gasnete_lapi_complete_transfer, info);

    total_transfers++;
    local_addr += ask_bytes;
    remote_addr += ask_bytes;
    remaining_bytes -= ask_bytes;
  } while(remaining_bytes);


  /* Wait for all initiations to complete before proceeding
     because of a potential race if you try to sync on the handle
     before you even send stuff out */
  while(gasneti_weakatomic_read(&initiated_xfers,0) != total_transfers) {
    gasnetc_AMPoll();
  }
  gasneti_sync_reads();
#else /* GASNET_SEGMENT_EVERYTHING */
  GASNETI_TRACE_PRINTF(C,("gasnete_lapi_do_rdma: nbytes = %ld threshold = %d\n",nbytes,gasnete_pin_threshold));
  /* Set "constant" elements of the xfer_struct */
  xfer_struct.HwXfer.Xfer_type = LAPI_RDMA_XFER;
  xfer_struct.HwXfer.tgt = node;
  xfer_struct.HwXfer.op = op;
  xfer_struct.HwXfer.remote_cxt = rcxt.usr_rcxt;

  /* Note that in computing in_local_seg, we don't bother to check for an xfer
   * that crosses gasnetc_my_segtop.  That should be impossible, so assert. */
  in_local_seg = ((lapi_long_t) local_ptr >= gasnetc_my_segbase) &&
                 ((lapi_long_t) local_ptr <= gasnetc_my_segtop);
  gasneti_assert(!in_local_seg || ((((lapi_long_t) local_ptr) + nbytes - 1) <= gasnetc_my_segtop));

  /* Remote addr as offsets from remote segment and remote PVO */
  remote_segment_offset = ((lapi_long_t) remote_ptr) - gasnetc_segbase_table[node];
  remote_offset = remote_segment_offset & GASNETC_LAPI_PVO_EXTENT_MASK;

  /* If the transfer is really small (for some definition of "really small") */
  if(!gasnetc_use_firehose && !in_local_seg && (nbytes <= gasnete_pin_threshold)) {
      /* Get a free buffer */
      GASNETI_TRACE_PRINTF(C,("gasnete_lapi_do_rdma: looking for network buffer\n"));
      nb_id = gasnete_get_free_network_buffer();
      if(op != LAPI_RDMA_GET) {
        /* Copy in for puts */
        memcpy(nb_id->data,local_ptr,nbytes);
        nb_id->get_length = 0;
      } else {
        /* Save addr/len for gets */
        nb_id->get_buffer = local_ptr;
        nb_id->get_length = nbytes;
      }

      nb_id->origin_counter = origin_counter;
      gasneti_weakatomic_set(&nb_id->num_waiting,	 /* do we cross 1 or 2 PVOs? */
			 ((GASNETC_LAPI_PVO_EXTENT - remote_offset < nbytes) ? 2 : 1), 0);
  } 

  /* Do something special if the origin is within the pinned segment or we can use a network buffer */
  if ((nb_id != NULL) || in_local_seg) {
    /* Offsets of local addr relative to base of segment */
    lapi_long_t local_segment_offset = ((lapi_long_t) local_ptr) - gasnetc_my_segbase;

    /* No need to pin origin, but you need to deal with misalignment 
       (in terms of the PVO boundaries)
       of the pinned regions of the origin and destination */

    xfer_struct.HwXfer.sinfo = (void *) nb_id;
    do {
      /* The number of bytes to either the end of the current PVO region
         or the end of the data */
      int chunk_remaining;

      if(nb_id != NULL) { /* Using network buffer */
        GASNETI_TRACE_PRINTF(C,("gasnete_lapi_do_rdma: using network buffer nbytes = %ld\n",nbytes));
        xfer_struct.HwXfer.shdlr = gasnete_lapi_reap_network_buffer;
        xfer_struct.HwXfer.org_cntr = NULL;
        xfer_struct.HwXfer.src_pvo = nb_id->pvo;
        source_offset = nb_id->offset;
        chunk_remaining = nbytes;
      } else {
        xfer_struct.HwXfer.shdlr = (scompl_hndlr_t *) NULL;
        xfer_struct.HwXfer.org_cntr = origin_counter;
        xfer_struct.HwXfer.src_pvo = gasnetc_lapi_find_pvo(gasneti_mynode, local_segment_offset);
        source_offset = local_segment_offset & GASNETC_LAPI_PVO_EXTENT_MASK;
        chunk_remaining = (int) MIN(GASNETC_LAPI_PVO_EXTENT - source_offset, remaining_bytes);
      }

      do {
        /* Try to transfer this chunk of bytes.  It will take exactly
            one or two RDMA calls depending on whether or not it is entirely
            within a single PVO region at the target */
	transfer_len = MIN(GASNETC_LAPI_PVO_EXTENT - remote_offset, chunk_remaining);

	/* Local offset (PVO was set outside this loop) */
	xfer_struct.HwXfer.src_offset = source_offset;

	/* Remote offset and PVO */
	xfer_struct.HwXfer.tgt_offset = remote_offset;
	xfer_struct.HwXfer.tgt_pvo = gasnetc_lapi_find_pvo(node, remote_segment_offset);
	
	xfer_struct.HwXfer.len = transfer_len;
        GASNETI_TRACE_PRINTF(C,("gasnete_lapi_do_rdma: transfer length = %d nbytes_transferred = %ld bytes remaining = %ld remote_offset=%d real remote offfset=%ld segment start = %ld GASNETC_LAPI_PVO_EXTENT=%ld offset in remote pvo table = %ld remote pvo=0x%lx get = %d source_offset=%d real source offset=%ld offset in local pvo table=%ld\n",transfer_len, (uint64_t) (nbytes - remaining_bytes), (uint64_t) chunk_remaining,remote_offset,(uint64_t) remote_segment_offset,(uint64_t) gasnetc_segbase_table[node],GASNETC_LAPI_PVO_EXTENT,(uint64_t) remote_segment_offset/GASNETC_LAPI_PVO_EXTENT, (ulong) xfer_struct.HwXfer.tgt_pvo,op == LAPI_RDMA_GET,source_offset, (uint64_t) local_segment_offset,(uint64_t) local_segment_offset/GASNETC_LAPI_PVO_EXTENT));
        
	GASNETC_LCHECK (LAPI_Xfer (gasnetc_lapi_context, &xfer_struct));
	total_transfers++;
       
	source_offset += transfer_len;
	remaining_bytes -= transfer_len;
	local_segment_offset += transfer_len;
	remote_segment_offset += transfer_len;
        remote_offset = remote_segment_offset & GASNETC_LAPI_PVO_EXTENT_MASK;
	chunk_remaining -= transfer_len;
	/* IF we make a second pass, it will be aligned remotely */
	gasneti_assert(!chunk_remaining || !remote_offset);
      } while (chunk_remaining);

      /* If using a network buffer, the inner loop MUST have completed the xfer */
      gasneti_assert((nb_id == NULL) || !remaining_bytes);
    } while(remaining_bytes);
             
  } else {
    lapi_long_t local_addr = (lapi_long_t) local_ptr;

    xfer_struct.HwXfer.org_cntr = origin_counter;
    if(gasnetc_use_firehose) {
      xfer_struct.HwXfer.shdlr = gasnete_lapi_release_firehose;
      do {
        /* Try to pin local piece, while dealing w/ alignment */
        /* We may need to ask for less than max_LocalPinSize because of page size rounding up */
        lapi_long_t ask_bytes = MIN(remaining_bytes, gasnetc_firehose_info.max_LocalPinSize - (local_addr & (GASNETI_PAGESIZE-1)));
        const firehose_request_t *fh_req = firehose_local_pin(local_addr, ask_bytes, NULL);

        /* Compute how much I can do for now before hitting the end of either PVO */
  	source_offset = (local_addr - (lapi_long_t) fh_req->addr);
        transfer_len = MIN(ask_bytes, fh_req->len - source_offset);
        transfer_len = MIN(transfer_len, GASNETC_LAPI_PVO_EXTENT - remote_offset);

        /* Local offset and PVO */
	xfer_struct.HwXfer.src_pvo = fh_req->client.pvo;
	xfer_struct.HwXfer.src_offset = source_offset;

        /* Remote offset and PVO */
	xfer_struct.HwXfer.tgt_pvo = gasnetc_lapi_find_pvo(node, remote_segment_offset);
	xfer_struct.HwXfer.tgt_offset = remote_offset;

        xfer_struct.HwXfer.len = transfer_len;
        xfer_struct.HwXfer.sinfo = (void *) fh_req;
      
        /* Do the transfer */
        GASNETC_LCHECK (LAPI_Xfer (gasnetc_lapi_context, &xfer_struct));
        total_transfers++;

        remote_segment_offset += transfer_len; 
        remote_offset = remote_segment_offset & GASNETC_LAPI_PVO_EXTENT_MASK;
        local_addr += transfer_len; 
        remaining_bytes -= transfer_len; 
      } while (remaining_bytes);
    } else {
      xfer_struct.HwXfer.src_offset = 0;
      xfer_struct.HwXfer.shdlr = gasnete_lapi_reap_pvo;
      do {
        /* Compute the number of bytes to be transferred in this step.  Try
         * to align the local and remote sides so that the maximum amount of data
         * is transferred with each call */
      
        /* Do this first to put a brake on pinning */
        gasnetc_lapi_new_pvo();

      	/* Transfer only up to the next PVO boundary on the remote side */
	transfer_len = MIN (remaining_bytes, GASNETC_LAPI_PVO_EXTENT - remote_offset);

        /* Local offset and PVO */
	xfer_struct.HwXfer.src_pvo = gasnetc_lapi_get_local_pvo(local_addr, transfer_len);

        /* Remote offset and PVO */
	xfer_struct.HwXfer.tgt_offset = remote_offset;
	xfer_struct.HwXfer.tgt_pvo = gasnetc_lapi_find_pvo(node, remote_segment_offset);

        xfer_struct.HwXfer.len = transfer_len;
        xfer_struct.HwXfer.sinfo = (void *) xfer_struct.HwXfer.src_pvo;
      
        /* Do the transfer */
        GASNETC_LCHECK (LAPI_Xfer (gasnetc_lapi_context, &xfer_struct));
        total_transfers++;

        local_addr += transfer_len;
        remote_segment_offset += transfer_len;
        remote_offset = 0;
        remaining_bytes -= transfer_len;
	/* IF we make a second (or subsequent) pass, it will be aligned remotely */
	gasneti_assert(!remaining_bytes || !(remote_segment_offset & GASNETC_LAPI_PVO_EXTENT_MASK));
      } while (remaining_bytes);
    }
  }
#endif /* GASNET_SEGMENT_EVERYTHING */
  /* Return an eop  with all the required information */
  new_eop->num_transfers = total_transfers;
  return(new_eop);
}
#endif

/* ------------------------------------------------------------------------------------ */
extern void gasnete_get_bulk (void *dest, gasnet_node_t node, void *src,
			      size_t nbytes GASNETE_THREAD_FARG)
{
#if GASNETC_LAPI_RDMA
    if_pt(gasnetc_lapi_use_rdma) {
      gasnete_eop_t *eop;
      GASNETI_TRACE_PRINTF(C,("gasnete_get_bulk\n"));
      /* gasneti_suspend_spinpollers(); */
      eop = gasnete_lapi_do_rdma(node,src,dest,nbytes,LAPI_RDMA_GET,NULL GASNETE_THREAD_PASS);
      gasnete_eop_check(eop);
      /* gasneti_resume_spinpollers(); */
      /* Wait on this eop */
      GASNETI_TRACE_PRINTF(C,("gasnete_get_bulk: wait on sync\n"));
      GASNETC_WAITCNTR(eop->origin_counter,eop->num_transfers,&eop->num_transfers);
      gasneti_assert(eop->num_transfers == 0);
      gasnete_op_free((gasnete_op_t *)eop);
    } else
#endif
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
}

/* ------------------------------------------------------------------------------------ */
extern void gasnete_put_bulk (gasnet_node_t node, void *dest, void *src,
			      size_t nbytes GASNETE_THREAD_FARG)
{
#if GASNETC_LAPI_RDMA
    if_pt(gasnetc_lapi_use_rdma) {
      gasnete_eop_t *eop;
      GASNETI_TRACE_PRINTF(C,("gasnete_put_bulk target node=%d\n",node));
      /* gasneti_suspend_spinpollers(); */
      eop = gasnete_lapi_do_rdma(node,dest,src,nbytes,LAPI_RDMA_PUT,NULL GASNETE_THREAD_PASS);
      gasnete_eop_check(eop);
      /* gasneti_resume_spinpollers(); */
      /* Wait on this eop */
      GASNETI_TRACE_PRINTF(C,("gasnete_put_bulk: wait on sync\n"));
      GASNETC_WAITCNTR(eop->origin_counter,eop->num_transfers,&eop->num_transfers);
      gasneti_assert(eop->num_transfers == 0);
      gasnete_op_free((gasnete_op_t *)eop);
      GASNETI_TRACE_PRINTF(C,("gasnete_put_bulk: wait done\n"));
    } else
#endif
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
      gasnete_eop_t *eop;
      GASNETI_TRACE_PRINTF(C,("gasnete_get_nb_bulk\n"));
      /* gasneti_suspend_spinpollers(); */
      eop = gasnete_lapi_do_rdma(node,src,dest,nbytes,LAPI_RDMA_GET,NULL GASNETE_THREAD_PASS);
      gasnete_eop_check(eop);
      /* gasneti_resume_spinpollers(); */
      return((gasnet_handle_t) eop);
    } else
#endif
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
      eop = gasnete_lapi_do_rdma(node,dest,src,nbytes,LAPI_RDMA_PUT,NULL GASNETE_THREAD_PASS);
      gasnete_eop_check(eop);
      /* gasneti_resume_spinpollers(); */
      /* Don't wait for source completion */
      return((gasnet_handle_t) eop);
    } else
#endif
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

    return (gasnet_handle_t)op;
  }
}

/* ------------------------------------------------------------------------------------ */
extern gasnet_handle_t gasnete_put_nb (gasnet_node_t node, void *dest, void *src,
				       size_t nbytes GASNETE_THREAD_FARG)
{
#if GASNETC_LAPI_RDMA
    if_pt(gasnetc_lapi_use_rdma) {
      gasnete_eop_t *eop;
      GASNETI_TRACE_PRINTF(C,("gasnete_put_nb\n"));
      /* gasneti_suspend_spinpollers(); */
      eop = gasnete_lapi_do_rdma(node,dest,src,nbytes,LAPI_RDMA_PUT,NULL GASNETE_THREAD_PASS);
      gasnete_eop_check(eop);
      /* gasneti_resume_spinpollers(); */

      /* XXX: non-bulk put is implemented as fully blocking */
      GASNETC_WAITCNTR(eop->origin_counter,eop->num_transfers,&eop->num_transfers);
      gasneti_assert(eop->num_transfers == 0);
      gasnete_op_free((gasnete_op_t *)eop);

      return GASNET_INVALID_HANDLE;
    } else
#endif
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
    
    return (gasnet_handle_t)op;
  }
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
    op->origin_counter = &(op->cntr);
    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context, op->origin_counter, 0));
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, (unsigned int)node,
			       gasnete_remote_memset_hh[node],
			       &uhdr, sizeof(gasnete_memset_uhdr_t), NULL, 0,
			       NULL, &o_cntr, op->origin_counter));
    /* gasneti_resume_spinpollers(); */
  } else
#endif
  {
    gasneti_suspend_spinpollers();
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, (unsigned int)node,
			       gasnete_remote_memset_hh[node],
			       &uhdr, sizeof(gasnete_memset_uhdr_t), NULL, 0,
			       NULL, &o_cntr, &op->cntr));
    gasneti_resume_spinpollers();
   
    op->initiated_cnt++;
  }
    /* must insure operation has completed locally since uhdr is a stack variable.
     * This will ALMOST ALWAYS be true in the case of such a small message */
    GASNETC_WAITCNTR(&o_cntr,1,&cur_cntr);
    gasneti_assert(cur_cntr == 0);

    return (gasnet_handle_t)op;
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

#if GASNETC_LAPI_RDMA
extern void gasnete_wait_syncnb(gasnet_handle_t handle) {
    gasnete_op_t *op = handle;
    if (handle == GASNET_INVALID_HANDLE)
	return;
    gasneti_assert(op->threadidx == gasnete_mythread()->threadidx);
    if_pt (OPTYPE(op) == OPTYPE_EXPLICIT) {
	gasnete_eop_t *eop = (gasnete_eop_t*)op;
	gasneti_assert(OPSTATE(op) != OPSTATE_FREE);
        gasnete_eop_check(eop);
	if (eop->initiated_cnt > 0) {
	    GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&eop->cntr,eop->initiated_cnt,&eop->initiated_cnt));
	    gasneti_assert(eop->initiated_cnt == 0);
	}
	else  if (eop->num_transfers > 0) {
	  gasneti_assert(eop->origin_counter != NULL);
          GASNETC_LCHECK((LAPI_Waitcntr(gasnetc_lapi_context,eop->origin_counter,eop->num_transfers,&eop->num_transfers)));
	  gasneti_assert(eop->num_transfers == 0);
	}
    } else {
	gasnete_iop_t *iop = (gasnete_iop_t*)op;
        gasnete_iop_check(iop);
	if (iop->initiated_get_cnt > 0) {
	    GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&iop->get_cntr,iop->initiated_get_cnt,&iop->initiated_get_cnt));
	    gasneti_assert(iop->initiated_get_cnt == 0);
	}
	if (iop->initiated_put_cnt > 0) {
	    GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&iop->put_cntr,iop->initiated_put_cnt,&iop->initiated_put_cnt));
	    gasneti_assert(iop->initiated_put_cnt == 0);
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
    eop = gasnete_lapi_do_rdma(node,src,dest,nbytes,LAPI_RDMA_GET,&(op->get_cntr) GASNETE_THREAD_PASS);
    op->initiated_get_cnt += eop->num_transfers;
  #if GASNET_DEBUG
    eop->origin_counter = NULL; /* Otherwise eop_check will incorrectly bounds check against iop's counter */
    eop->num_transfers = 0; /* Otherwise op_free will fail assertion */
  #endif
    gasnete_op_free((gasnete_op_t *) eop); 
    /* gasneti_resume_spinpollers(); */
  } else
#endif
  {
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
    eop = gasnete_lapi_do_rdma(node,dest,src,nbytes,LAPI_RDMA_PUT,&(op->put_cntr) GASNETE_THREAD_PASS);
    op->initiated_put_cnt += eop->num_transfers;
  #if GASNET_DEBUG
    eop->origin_counter = NULL; /* Otherwise eop_check will incorrectly bounds check against iop's counter */
    eop->num_transfers = 0; /* Otherwise op_free will fail assertion */
  #endif
    gasnete_op_free((gasnete_op_t *) eop); 
    /* gasneti_resume_spinpollers(); */
  } else
#endif
  {
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
  }
}

/* ------------------------------------------------------------------------------------ */
extern void gasnete_put_nbi (gasnet_node_t node, void *dest, void *src,
			     size_t nbytes GASNETE_THREAD_FARG)
{
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
#if GASNETC_LAPI_RDMA
    if(gasnetc_lapi_use_rdma) {
      gasnete_eop_t *eop;
      GASNETI_TRACE_PRINTF(C,("gasnete_put_nbi\n"));
      /* gasneti_suspend_spinpollers(); */
      eop = gasnete_lapi_do_rdma(node,dest,src,nbytes,LAPI_RDMA_PUT,NULL GASNETE_THREAD_PASS);
      /* gasneti_resume_spinpollers(); */
      /* Because this is fully blocking, we don't advance op->initiated_put_cnt here */

      /* XXX: non-bulk put is implemented as fully blocking */
      GASNETC_WAITCNTR(eop->origin_counter,eop->num_transfers,&eop->num_transfers);
      gasneti_assert(eop->num_transfers == 0);
      gasnete_op_free((gasnete_op_t *)eop);
    } else
#endif
  {
    gasnete_iop_t *op = mythread->current_iop;
    int num_put = 0;
    int cur_cntr;
    lapi_cntr_t  o_cntr;
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
  }
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
    } else
#endif
  {
    gasneti_suspend_spinpollers();
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, (unsigned int)node,
			       gasnete_remote_memset_hh[node],
			       &uhdr, sizeof(gasnete_memset_uhdr_t), NULL, 0,
			       NULL, &o_cntr, &op->put_cntr));
    gasneti_resume_spinpollers();
  }
    op->initiated_put_cnt++;
    /* must insure operation has completed locally since uhdr is a stack variable.
     * This will ALMOST ALWAYS be true in the case of such a small message */
    GASNETC_WAITCNTR(&o_cntr,1,&cur_cntr);
    gasneti_assert(cur_cntr == 0);
}

/* ------------------------------------------------------------------------------------ */
/*
  Synchronization for implicit-handle non-blocking operations:
  ===========================================================
*/

extern int  gasnete_try_syncnbi_gets(GASNETE_THREAD_FARG_ALONE) {
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
            if_pf (cnt > GASNETC_NBI_CNTR_LIMIT) { /* make sure we don't overflow the counters */
	      GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&iop->get_cntr,0));
              iop->initiated_get_cnt = 0;
            }
	    gasneti_sync_reads();
	    return GASNET_OK;
        } else return GASNET_ERR_NOT_READY;
}

extern int  gasnete_try_syncnbi_puts(GASNETE_THREAD_FARG_ALONE) {
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
            if_pf (cnt > GASNETC_NBI_CNTR_LIMIT) { /* make sure we don't overflow the counters */
	      GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&iop->put_cntr,0));
              iop->initiated_put_cnt = 0;
            }
	    gasneti_sync_reads();
	    return GASNET_OK;
        } else return GASNET_ERR_NOT_READY;
}

#if GASNETC_LAPI_RDMA
extern void gasnete_wait_syncnbi_puts(GASNETE_THREAD_FARG_ALONE) {
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t *iop = mythread->current_iop;
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
	GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&iop->put_cntr,iop->initiated_put_cnt,&iop->initiated_put_cnt));
	gasneti_sync_reads();
    } 
    gasneti_assert(iop->initiated_put_cnt == 0);
}

extern void gasnete_wait_syncnbi_gets(GASNETE_THREAD_FARG_ALONE) 
{
    gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
    gasnete_iop_t *iop = mythread->current_iop;
    gasneti_assert(iop->threadidx == mythread->threadidx);
    gasneti_assert(iop->next == NULL);
    gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);
#if GASNET_DEBUG
    if (iop->next != NULL)
	gasneti_fatalerror("VIOLATION: attempted to call gasnete_wait_syncnbi_gets() inside an NBI access region");
#endif
    if (gasneti_weakatomic_read(&iop->get_aux_cntr, 0)) /* avoid extra rmb when possible */
      GASNET_BLOCKUNTIL(gasneti_weakatomic_read(&iop->get_aux_cntr, 0) == 0);
    if (iop->initiated_get_cnt > 0) {
	GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context,&iop->get_cntr,iop->initiated_get_cnt,&iop->initiated_get_cnt));
	/* gasneti_sync_mem(); */  /* Don't think this is necessary, PH */
    }
    gasneti_assert(iop->initiated_get_cnt == 0);
}

extern void gasnete_wait_syncnbi_all(GASNETE_THREAD_FARG_ALONE) {
    gasnete_wait_syncnbi_puts(GASNETE_THREAD_PASS_ALONE);
    gasnete_wait_syncnbi_gets(GASNETE_THREAD_PASS_ALONE);
}
#endif /* GASNETC_LAPI_RDMA */

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
  Barriers:
  =========
*/
static void gasnete_lapibarrier_init(int usegfence);
static void gasnete_lapibarrier_notify(gasnete_coll_team_t team, int id, int flags);
static int gasnete_lapibarrier_wait(gasnete_coll_team_t team, int id, int flags);
static int gasnete_lapibarrier_try(gasnete_coll_team_t team, int id, int flags);

/* #define GASNETE_BARRIER_DEFAULT "LAPIGFENCE" See bug 2186 */
#define GASNETE_BARRIER_READENV() do {                                   \
  if (GASNETE_ISBARRIER("LAPIGFENCE"))                                   \
    gasnete_coll_default_barrier_type = GASNETE_COLL_BARRIER_LAPIGFENCE; \
  else if (GASNETE_ISBARRIER("LAPIAM"))                                  \
    gasnete_coll_default_barrier_type = GASNETE_COLL_BARRIER_LAPIAM;     \
} while (0)

#define GASNETE_BARRIER_INIT(TEAM, BARRIER_TYPE) do {       \
    if ((BARRIER_TYPE) == GASNETE_COLL_BARRIER_LAPIGFENCE && (TEAM)==GASNET_TEAM_ALL) { \
      (TEAM)->barrier_notify = &gasnete_lapibarrier_notify; \
      (TEAM)->barrier_wait =   &gasnete_lapibarrier_wait;   \
      (TEAM)->barrier_try =    &gasnete_lapibarrier_try;    \
      (TEAM)->barrier_pf =     NULL;                        \
      gasnete_lapibarrier_init(1);                          \
    } else if ((BARRIER_TYPE) == GASNETE_COLL_BARRIER_LAPIAM && (TEAM)==GASNET_TEAM_ALL) { \
      (TEAM)->barrier_notify = &gasnete_lapibarrier_notify; \
      (TEAM)->barrier_wait =   &gasnete_lapibarrier_wait;   \
      (TEAM)->barrier_try =    &gasnete_lapibarrier_try;    \
      (TEAM)->barrier_pf =     NULL;                        \
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
static void gasnete_lapibarrier_notify(gasnete_coll_team_t team, int id, int flags) {
  gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
  if_pf(team->barrier_splitstate == INSIDE_BARRIER) 
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
    team->barrier_splitstate = INSIDE_BARRIER;
    gasneti_sync_writes(); /* ensure all state changes committed before return */
  }
}

static int gasnete_lapibarrier_wait(gasnete_coll_team_t team, int id, int flags) {
#if GASNETI_STATS_OR_TRACE
    gasneti_tick_t wait_start = GASNETI_TICKS_NOW_IFENABLED(B);
#endif
    int phase;
    gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
    phase = barrier_phase;
    if_pf(team->barrier_splitstate == OUTSIDE_BARRIER) 
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
      team->barrier_splitstate = OUTSIDE_BARRIER;
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

static int gasnete_lapibarrier_try(gasnete_coll_team_t team, int id, int flags) {
    gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
    if_pf(team->barrier_splitstate == OUTSIDE_BARRIER) 
	gasneti_fatalerror("gasnet_barrier_try() called without a matching notify");

    /* should we kick the network if not done? */

    if (barrier_response_done[barrier_phase]) {
	GASNETI_TRACE_EVENT_VAL(B,BARRIER_TRY,1);
	return gasnete_lapibarrier_wait(team, id, flags);
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

extern gasnet_handlerentry_t const *gasnete_get_handlertable(void) {
    return gasnete_handlers;
}

/* ------------------------------------------------------------------------------------ */
