/* $Source: /var/local/cvs/gasnet/gm-conduit/gasnet_extended_firehose.c,v $
 * $Date: 2010/04/17 03:14:25 $
 * $Revision: 1.58.4.1 $
 * Description: GASNet GM conduit Firehose DMA Registration Algorithm
 * Copyright 2002, Christian Bell <csbell@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include <gasnet_internal.h>
#ifdef GASNETC_FIREHOSE
#include <gasnet_extended_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_handler.h>
#include <sys/mman.h>

#define GASNETE_FH_HAVE_TOKEN		0
#define GASNETE_FH_POLL_TOKEN		1

extern void gasnetc_callback_ambuffer(struct gm_port *, void *, gm_status_t);
void	    gasnete_fh_request_get_am(void *_gop, const firehose_request_t *req, 
				      int allLocalHit);
#ifdef GASNETC_GM_2
void gasnete_fh_request_get_rdma(void *, const firehose_request_t *, int);
#endif

int gasnete_getrdma_enabled = 1;    /* default to being enabled, on GM 2.x */

GASNETI_INLINE(gasnete_fh_request_get)
void 
gasnete_fh_request_get(void *op, const firehose_request_t *req, int loc)
{
  #ifdef GASNETC_GM_2
    if (gasnete_getrdma_enabled)
	gasnete_fh_request_get_rdma(op,req,loc);
    else
	gasnete_fh_request_get_am(op,req,loc);
  #else
    gasnete_fh_request_get_am(op,req,loc);
  #endif
   return;
}

void
gasnete_fh_request_get_fn(void *op, const firehose_request_t *req, int loc)
{
    gasnete_fh_request_get(op,req,loc);
    return;
}

/* ------------------------------------------------------------------------ */
/* Tracing Firehose */
#ifdef GASNETC_FIREHOSE_TRACE
#define GASNETE_FIREHOSE_TRACE_PUTGET(eop, putget)		\
	do {							\
	    switch(eop->fh_stats) {				\
		case fh_onesided: GASNETI_TRACE_EVENT_TIME(C,	\
			    FIREHOSE_ ## putget ## _ONESIDED, 	\
			    GASNETI_TICKS_NOW_IFENABLED(C)-	\
			    eop->starttime); break;		\
		case fh_one: GASNETI_TRACE_EVENT_TIME(C,	\
			    FIREHOSE_ ## putget ## _ONE, 	\
			    GASNETI_TICKS_NOW_IFENABLED(C)-	\
			    eop->starttime); break;		\
		case fh_many: GASNETI_TRACE_EVENT_TIME(C,	\
			    FIREHOSE_ ## putget ## _MANY, 	\
			    GASNETI_TICKS_NOW_IFENABLED(C)-	\
			    eop->starttime); break;		\
		case fh_none: gasneti_fatalerror("eop stats");	\
		default: break;					\
	    }							\
	} while (0)
#else
#define GASNETE_FIREHOSE_TRACE_PUTGET(eop, putget)
#endif

extern
int 
firehose_move_callback(gasnet_node_t node, 
		const firehose_region_t *unpin_list, size_t unpin_num, 
		firehose_region_t *pin_list, size_t pin_num)
{
	int	 i;
	int	locked = GASNETE_GM_IN_UNKNOWN();
	gm_status_t status;

	if (!locked)
		gasneti_mutex_lock(&gasnetc_lock_gm);

	for (i = 0; i < unpin_num; i++) {
		gasneti_assert(unpin_list[i].addr % GASNET_PAGESIZE == 0);
		gasneti_assert(unpin_list[i].len % GASNET_PAGESIZE == 0);
		status = gm_deregister_memory(_gmc.port, 
			  (void *) unpin_list[i].addr, unpin_list[i].len);
		if (status != GM_SUCCESS) 
		    gasneti_fatalerror("gm_deregister_memory() failed for "
		       "page located at %p (%s)", (void*)unpin_list[i].addr, 
		       gm_strerror(status));
		GASNETI_TRACE_PRINTF(C,
		  ("Firehose unpinlocal = %p, %d", (void *) unpin_list[i].addr,
			(int)unpin_list[i].len));
	}
	GASNETI_TRACE_EVENT_VAL(C, FIREHOSE_LOCALUNPIN_PAGES, unpin_num);

	for (i = 0; i < pin_num; i++) {
                int perm_adj = 0;
              retry_register:
		gasneti_assert(pin_list[i].addr % GASNET_PAGESIZE == 0);
		gasneti_assert(pin_list[i].len % GASNET_PAGESIZE == 0);
		status = gm_register_memory(_gmc.port, (void *)
				pin_list[i].addr, pin_list[i].len);
		if (status != GM_SUCCESS) {
                  if (perm_adj) {
		    gasneti_fatalerror("gm_register_memory() failed for "
		       "page located at %p (%s)", (void*)pin_list[i].addr, 
		       gm_strerror(status));
                  } else { /* bug 1036 - GM cannot register read-only data memory */
                    int j, num_pages = pin_list[i].len / GASNET_PAGESIZE;
                    char *p;
                    static char tst = 0;
                    /* check for readability before we mess with mprotect */
		    GASNETI_TRACE_PRINTF(C,("gm_register_memory() failed for "
		       "page located at %p len=%i pages (%s) -- checking readability", 
                       (void*)pin_list[i].addr, num_pages, gm_strerror(status)));
                    p = (void *)pin_list[i].addr;
                    for (j = 0 ; j < num_pages; j++) {
                      tst += *p; /* if you get a seg fault here, it means firehose tried to register unmapped memory */
                      p += GASNET_PAGESIZE;
                    }
                    p = (void *)pin_list[i].addr;
                    for (j = 0 ; j < num_pages; j++) {
		      GASNETI_TRACE_PRINTF(C,("Attempting mprotect for page located at %p", (void*)p));
                      if (mprotect(p, GASNET_PAGESIZE, PROT_READ|PROT_WRITE))
                        gasneti_fatalerror("mprotect failed in firehose_move_callback: %s", strerror(errno));
                      p += GASNET_PAGESIZE;
                    }
                    perm_adj = 1;
                    goto retry_register;
                  }
                }
		GASNETI_TRACE_PRINTF(C,
		  ("Firehose pinlocal = %p, %d", (void *) pin_list[i].addr,
			(int)pin_list[i].len));
	}
	GASNETI_TRACE_EVENT_VAL(C, FIREHOSE_LOCALPIN_PAGES, pin_num);

	if (!locked)
		gasneti_mutex_unlock(&gasnetc_lock_gm);

	return 0;
}

/* ##################################################################### */
/* PUTS                                                                  */
/* ##################################################################### */
void
gasnete_fh_callback_put(struct gm_port *p, void *context, 
			      gm_status_t status) {
        GASNET_BEGIN_FUNCTION(); /* thread cache for *_IN_UNKNOWN */
	gasnete_eop_t		*pop = (gasnete_eop_t *) context;
	gasnet_node_t		node = pop->node;
	gasneti_tick_t      starttime = GASNETI_TICKS_NOW_IFENABLED(C);
	const firehose_request_t	*fhreqs[2];
	int				numreqs = 1;

	gasneti_mutex_assertlocked(&gasnetc_lock_gm);
	gasneti_assert(pop != NULL);
	gasneti_assert(node != gasneti_mynode && node < gasneti_nodes);

	if_pf (status != GM_SUCCESS)
	    gasnetc_callback_error(status, NULL);
	gasnetc_token_lo_release();

	GASNETI_TRACE_PRINTF(C, 
	    ("Firehose decrement remote refcount for (%p,%d) on node %d (op=%p,%p,%d)\n",
	     (void *) pop->dest, pop->len, (unsigned) node, (void *) pop, 
	     (void *)pop->req_remote.addr, (int)pop->req_remote.len));

	fhreqs[0] = &(pop->req_remote);

	/* Puts use an ambuffer, while bulk puts send from a pinned location */
	if (OPMISC(pop) == OPMISC_AMBUF) {
		gasnetc_bufdesc_t	*bufd;
		bufd = (gasnetc_bufdesc_t *) GASNETC_BUFDESC_PTR(pop->src);
		GASNETC_ASSERT_BUFDESC_PTR(bufd, pop->src);
		gasnetc_callback_ambuffer(p, (void *) bufd, status);
	}
	else  {
		fhreqs[1] = pop->req_local;
		numreqs++;
		#ifdef GASNET_SEGMENT_FAST
		/* Only release locally for "fast" segment */
		firehose_release(fhreqs+1, 1);
		#endif
	}

	#ifndef GASNET_SEGMENT_FAST
	GASNETE_GM_SET_IN_UNKNOWN();
	firehose_release(fhreqs, numreqs);
	GASNETE_GM_UNSET_IN_UNKNOWN();
	#endif

	GASNETE_FIREHOSE_TRACE_PUTGET(pop, PUT);

	/* If this was associated to an iop, increment put completed count */
	if (pop->iop != NULL) {
		gasneti_weakatomic_increment(&(pop->iop->completed_put_cnt),0/*Rel?*/);
		gasneti_free(pop); /* free a "dummy" eop */
        } else {
	        gasnete_op_markdone((gasnete_op_t *)pop, 0);
        }

	GASNETI_TRACE_EVENT_TIME(C, FIREHOSE_MOVE_LOCAL,
		    GASNETI_TICKS_NOW_IFENABLED(C)-starttime);

	return;
}

void
gasnete_fh_request_put(void *_pop, const firehose_request_t *req,
			int allLocalHit)
{
	gasnete_eop_t	*pop = (gasnete_eop_t *) _pop;
	gm_status_t	status;
	gasnet_node_t	node;

	gasneti_assert(pop != NULL);
	gasneti_assert(pop->src > 0 && pop->dest > 0);
	node = pop->node;
	gasneti_assert(node != gasneti_mynode && node < gasneti_nodes);
	gasneti_assert(pop->len > 0);
	gasneti_assert(req == &(pop->req_remote));

	gasneti_mutex_lock(&gasnetc_lock_gm);
	gasnetc_token_lo_poll();

	GASNETI_TRACE_PRINTF(C, 
	    ("Firehose directed send(%p): (%d,%p) <- (%p,%d)", 
	     (void *) pop, (unsigned) node, (void *) pop->dest, 
	     (void *) pop->src, pop->len));

	#if GASNETI_STATS_OR_TRACE
	if (!allLocalHit)
	    pop->fh_stats = pop->len > 4096 ? fh_many : fh_one;
	#endif

	GASNETC_GM_PUT(
	    _gmc.port, (void *) pop->src, (gm_remote_ptr_t) pop->dest,
	    (unsigned long) pop->len, GM_LOW_PRIORITY,
	    gasnetc_nodeid(node), gasnetc_portid(node),
	    gasnete_fh_callback_put, (void *) pop);
	gasneti_mutex_unlock(&gasnetc_lock_gm);
	return;
}

GASNETI_INLINE(gasnete_firehose_put_bulk)
gasnet_handle_t
gasnete_firehose_put_bulk(gasnet_node_t node, void *dest, void *src, 
			  size_t nbytes, gasnete_iop_t *iop GASNETE_THREAD_FARG)
{
	gasnete_eop_t	*pop;

	if (iop) /* use a "dummy" eop */
          pop = gasneti_calloc(1, sizeof(gasnete_eop_t));
        else 
          pop = gasnete_eop_new(GASNETE_MYTHREAD);
	pop->src = (uintptr_t) src;
	pop->dest = (uintptr_t) dest;
	pop->len = (uint32_t) nbytes;
	pop->node = node;
	pop->iop = iop;
	SET_OPMISC(pop, OPMISC_NONAMBUF);
	#if GASNETI_STATS_OR_TRACE
	pop->fh_stats  = fh_onesided;
	pop->starttime = GASNETI_TICKS_NOW_IFENABLED(C);
	#endif

	/* If we were dealing with implicit put, increment the iop */
	if (pop->iop != NULL)
		pop->iop->initiated_put_cnt++;

	pop->req_local = 
	    firehose_local_pin((uintptr_t) src, nbytes, NULL);

	#ifdef GASNET_SEGMENT_FAST
	    gasnete_fh_request_put(pop, &(pop->req_remote), 1);
	#else
	    firehose_remote_pin(node, (uintptr_t) dest, nbytes,
		0, (firehose_request_t *) &(pop->req_remote), NULL,
		gasnete_fh_request_put, pop);
	#endif

        return (iop?(gasnete_op_t *)iop:(gasnete_op_t *)pop);
}

extern gasnet_handle_t
gasnete_put_nb_bulk (gasnet_node_t node, void *dest, void *src, 
		     size_t nbytes GASNETE_THREAD_FARG)
{
	gasnet_handle_t	handle;
	GASNETI_CHECKPSHM_PUT(UNALIGNED,H);
	GASNETI_TRACE_PRINTF(C, 
	    ("gasnete_put_nb_bulk Firehose (%d,%p <- %p,%d bytes)",
	    (unsigned) node, dest, src, (int)nbytes));

	handle = gasnete_firehose_put_bulk(node, dest, src, nbytes, 
		    NULL GASNETE_THREAD_PASS);
	return handle;
}

extern void
gasnete_put_nbi_bulk (gasnet_node_t node, void *dest, void *src, 
		      size_t nbytes GASNETE_THREAD_FARG)
{
	gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
	gasnete_iop_t *iop = mythread->current_iop;
	GASNETI_CHECKPSHM_PUT(UNALIGNED,V);

	GASNETI_TRACE_PRINTF(C, 
	    ("gasnete_put_nbi_bulk Firehose (%d,%p <- %p,%d bytes)",
	    (unsigned) node, dest, src, (int)nbytes));

	gasnete_firehose_put_bulk(node, dest, src, nbytes, iop GASNETE_THREAD_PASS);
	return;
}

GASNETI_INLINE(gasnete_firehose_put)
gasnet_handle_t
gasnete_firehose_put(gasnet_node_t node, void *dest, void *src, size_t nbytes,
		     gasnete_iop_t *iop GASNETE_THREAD_FARG)
{
	gasnete_eop_t		*pop;
	gasnetc_bufdesc_t	*bufd;

	gasneti_assert(nbytes <= GASNETC_AM_LEN);
	bufd = gasnetc_AMRequestPool_block();

	if (iop) /* use a "dummy" eop */
          pop = gasneti_calloc(1, sizeof(gasnete_eop_t));
        else 
          pop = gasnete_eop_new(GASNETE_MYTHREAD);
	pop->node = node;
	pop->src = (uintptr_t) bufd->buf;
	pop->dest = (uintptr_t) dest;
	pop->len = (uint32_t) nbytes;
	pop->iop = iop;
	SET_OPMISC(pop, OPMISC_AMBUF);
	#if GASNETI_STATS_OR_TRACE
	pop->starttime = GASNETI_TICKS_NOW_IFENABLED(C);
	pop->fh_stats  = fh_onesided;
	#endif
	GASNETE_FAST_UNALIGNED_MEMCPY(bufd->buf, src, nbytes);

	/* If we were dealing with implicit put, increment the iop */
	if (iop != NULL)
		iop->initiated_put_cnt++;

	#ifdef GASNET_SEGMENT_FAST
	    gasnete_fh_request_put(pop, &(pop->req_remote), 1);
	#else
	    firehose_remote_pin(node, (uintptr_t) dest, nbytes,
		0, (firehose_request_t *) &(pop->req_remote), NULL,
		gasnete_fh_request_put, pop);
	#endif
	
        return (iop?(gasnete_op_t *)iop:(gasnete_op_t *)pop);
}

/*
 * In the non-bulk version of put, we always need a source copy of the local
 * data before sending it off, which doesn't require a local memory
 * registration.  
 *
 * By using AMRequestLong, the core API will attempt to query if the
 * destination is pinned and will leverage DMAs if possible.  The difference
 * between AMRequestLong and put_bulk is that the latter will _try_ to have the
 * remote memory pinned before issuing the DMA while the former is simply a
 * lookup/fallback approach - if the destination is not pinned, Mediums are
 * used.
 */
extern gasnet_handle_t 
gasnete_put_nb (gasnet_node_t node, void *dest, void *src, 
		size_t nbytes GASNETE_THREAD_FARG)
{
	gasnet_handle_t	handle;
	GASNETI_CHECKPSHM_PUT(ALIGNED,H);

	GASNETI_TRACE_PRINTF(C, 
	    ("gasnete_put_nb Firehose (%d,%p <- %p,%d bytes)",
	    (unsigned) node, dest, src, (int)nbytes));

	if_pf (nbytes > GASNETC_AM_LEN)
	    handle = gasnete_extref_put_nb(node, dest, src, nbytes 
		                           GASNETE_THREAD_PASS);
	else
	    handle = gasnete_firehose_put(node, dest, src, nbytes, 
					  NULL GASNETE_THREAD_PASS);
	return handle;
}

extern void
gasnete_put_nbi(gasnet_node_t node, void *dest, void *src, 
		size_t nbytes GASNETE_THREAD_FARG)
{
	gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
	gasnete_iop_t *iop = mythread->current_iop;
	GASNETI_CHECKPSHM_PUT(ALIGNED,V);
	GASNETI_TRACE_PRINTF(C, 
	    ("gasnete_put_nbi Firehose (%d,%p <- %p,%d bytes)",
	    (unsigned) node, dest, src, (int)nbytes));

	if_pf (nbytes > GASNETC_AM_LEN)
	    gasnete_extref_put_nbi(node, dest, src, nbytes 
			           GASNETE_THREAD_PASS);
	else
	    gasnete_firehose_put(node, dest, src, nbytes, 
			         iop GASNETE_THREAD_PASS);

	return;
}

/* ##################################################################### */
/* GETS                                                                  */
/* ##################################################################### */
/*
 * Gets can either be rdma-based or AM-based if RDMA gets are disabled.  In
 * RDMA-based operation, we use the remote-callback extension available in
 * Firehose whereby the payload associated to the get operation is sent before
 * the firehose reply (which optimizes firehose misses to be a single roundtrip
 * instead of two).
 */
GASNETI_INLINE(gasnete_get_fh_done)
void
gasnete_get_fh_done(gasnete_eop_t *eop)
{
	const firehose_request_t	*fhreqs[2];

	gasneti_assert(eop->src > 0 && eop->len > 0);

	GASNETI_TRACE_PRINTF(C, 
	    ("Firehose decrement remote refcount for (%p,%d) on node %d\n",
	     (void *) eop->src, eop->len, (unsigned) eop->node));

	/* Gets with DMA are a result of a local pin and remote pin request */
	fhreqs[0] = eop->req_local;

	#ifdef GASNET_SEGMENT_FAST
	    firehose_release(fhreqs, 1);
	#else
	    fhreqs[1] = &(eop->req_remote);
	    firehose_release(fhreqs, 2);
	#endif

	GASNETE_FIREHOSE_TRACE_PUTGET(eop, GET);

	if (eop->iop != NULL) {
		gasneti_weakatomic_increment(&(eop->iop->completed_get_cnt),0/*Rel?*/);
		gasneti_free(eop); /* free a "dummy" eop */
        } else {
	        gasnete_op_markdone((gasnete_op_t *) eop, 1);
        }

	return;
}

extern int firehose_remote_callback(gasnet_node_t node, 
		const firehose_region_t *pin_list, size_t num_pinned,
		firehose_remotecallback_args_t *args)
{
	gasneti_mutex_lock(&gasnetc_lock_gm);
	gasnetc_token_lo_poll();

	GASNETC_GM_PUT(_gmc.port, (void *) args->local_addr,
	   (gm_remote_ptr_t) args->remote_addr, (unsigned long) args->nbytes,
	   GM_LOW_PRIORITY, gasnetc_nodeid(node), gasnetc_portid(node),
	    gasnetc_callback_lo, NULL);

	GASNETI_TRACE_PRINTF(C, 
	    ("Firehose RDMA PUT(rev) (%p -> %d,%p @ %d bytes)", 
	     (void *) args->local_addr, node, (void *) args->remote_addr, 
	     (int)args->nbytes));

	gasneti_mutex_unlock(&gasnetc_lock_gm);

	return 0;
}

#ifdef GASNETC_GM_2
/* 
 * In GM 2.0, we can use directed receives (gm_get) once the remote region is
 * known to be pinned 
 */
void gasnete_fh_callback_get_rdma(struct gm_port *p, void *context, gm_status_t status) {
        GASNET_BEGIN_FUNCTION(); /* thread cache for *_IN_UNKNOWN */
	gasnete_eop_t			*gop = (gasnete_eop_t *) context;

	gasneti_mutex_assertlocked(&gasnetc_lock_gm);
	gasneti_assert(gop != NULL);
	gasneti_assert(gop->node!=gasneti_mynode && gop->node < gasneti_nodes);

	if_pf (status != GM_SUCCESS)
	    gasnetc_callback_error(status, NULL);
	gasnetc_token_lo_release();

	/* trace it, release the get and mark the op done */
	GASNETE_GM_SET_IN_UNKNOWN();
	gasnete_get_fh_done(gop);
	GASNETE_GM_UNSET_IN_UNKNOWN();

	return;
}

void
gasnete_fh_request_get_rdma(void *_gop, const firehose_request_t *req,
			int allLocalHit)
{
	gasnete_eop_t	*gop = (gasnete_eop_t *) _gop;
	gasnet_node_t	node;

	gasneti_assert(gop != NULL);
	gasneti_assert(gop->src > 0 && gop->dest > 0);
	node = gop->node;
	gasneti_assert(node != gasneti_mynode && node < gasneti_nodes);
	gasneti_assert(gop->len > 0);

	/* If the get callback hit the firehose cache (allLocalHit > 0), we can
	 * send a one-sided get.  If not, this callback is called after the the
	 * remote node has sent a one-sided put in place of an initatior RDMA
	 * get */

	if (allLocalHit) {
		gasneti_mutex_lock(&gasnetc_lock_gm);
		gasnetc_token_lo_poll();

		gm_get(_gmc.port, (gm_remote_ptr_t) gop->src,
		    (void *) gop->dest, (gm_size_t) gop->len, 
		    GM_LOW_PRIORITY, 
		    gasnetc_nodeid(node), gasnetc_portid(node),
		    gasnete_fh_callback_get_rdma, (void *) gop);

		GASNETI_TRACE_PRINTF(C, 
		    ("Firehose RDMA GET(op=%p): %p <- (%d,%p) (%d bytes)", 
		     (void *) gop, (void *) gop->dest, (unsigned) node, 
		     (void *) gop->src, gop->len));
	
		gasneti_mutex_unlock(&gasnetc_lock_gm);
	}
	else {
		#if GASNETI_STATS_OR_TRACE
		gop->fh_stats = gop->len > 4096 ? fh_many : fh_one;
		#endif

		/* The callback is called after the remote node has DMAd a put
		 * into the local memory.  The get can be be released and marked
		 * as done */
		GASNETI_TRACE_PRINTF(C, 
		    ("Firehose RDMA GET w/ PutRev (op=%p): %p <- (%d,%p) (%d bytes)", 
		     (void *) gop, (void *) gop->dest, (unsigned) node, 
		     (void *) gop->src, gop->len));

		gasnete_get_fh_done(gop);
	}

	return;
}
#endif

/*
 * AM Handler: Reply to get into a pinned memory location
 */
GASNETI_INLINE(gasnete_get_dma_reph_inner)
void gasnete_get_dma_reph_inner(gasnet_token_t token, void *op) {
        GASNET_BEGIN_FUNCTION(); /* thread cache for *_IN_UNKNOWN */
	gasnete_eop_t	*gop = (gasnete_eop_t *) op;

	GASNETE_GM_SET_IN_UNKNOWN();
	gasnete_get_fh_done(gop);
	GASNETE_GM_UNSET_IN_UNKNOWN();
}
LONG_HANDLER(gasnete_get_dma_reph,1,2, 
    (token, UNPACK(a0)    ),
    (token, UNPACK2(a0,a1)));

/* In GM 1.x, we can send a request for a ReplyLongAsync which essentially
 * translates to doing a put in the reverse direction */
GASNETI_INLINE(gasnete_get_dma_reqh_inner)
void
gasnete_get_dma_reqh_inner(gasnet_token_t token, 
				    gasnet_handlerarg_t nbytes, 
				    void *dest, void *src, void *op, void *op2)
{
	gasneti_assert(op != NULL && op2 != NULL); /* XXX this _was_ a bug on alvarez */
	/* The memory should already be pinned per a previous pin request */
	GASNETI_SAFE(
	    LONGASYNC_REP(1,2, (token,
	    gasneti_handleridx(gasnete_get_dma_reph), src, nbytes,
	    dest, PACK(op))));
}
SHORT_HANDLER(gasnete_get_dma_reqh,5,9, 
    (token, a0, UNPACK(a1),     UNPACK(a2),     UNPACK(a3),	UNPACK(a4)    ),
    (token, a0, UNPACK2(a1,a2), UNPACK2(a3,a4), UNPACK2(a5, a6),UNPACK2(a7,a8)));

void
gasnete_fh_request_get_am(void *_gop, const firehose_request_t *req, int allLocalHit)
{
	gasnete_eop_t	*gop = (gasnete_eop_t *) _gop;

	gasneti_assert(gop != NULL);
	gasneti_assert(gop->src != 0 && gop->dest != 0);
	gasneti_assert(gop->node != gasneti_mynode && gop->node < gasneti_nodes);

	/* If the remote pages are known to be pinned, send a request for RDMA
	 * */
	if (allLocalHit) {
		SHORT_REQ(5, 9,
		    (gop->node, gasneti_handleridx(gasnete_get_dma_reqh), 
		     gop->len,
		     PACK(gop->dest), PACK(gop->src), PACK(gop), PACK(gop)));

	}
	/* If the request completed with a remote roundtrip, the remote node
	 * used a DMA put to complete the get request.  Just release and mark
	 * done. */
	else {
	    GASNETI_TRACE_PRINTF(C, 
		("Firehose RDMA GET w/ PutRev (%p): %p <- (%d,%p) (%d bytes)", 
		gop, (void *) gop->dest, (unsigned) gop->node, 
		(void *) gop->src, gop->len));

	    #if GASNETI_STATS_OR_TRACE
	    gop->fh_stats = gop->len > 4096 ? fh_many : fh_one;
	    #endif

	    gasnete_get_fh_done(gop);
	}

	return;
}

static size_t gasnete_fh_remote_args_fn(void *context, firehose_remotecallback_args_t *args)
{
	gasnete_eop_t	*gop = context;

	/* Since Put is in reverse direction, the source is the local address
	 * and the destination is the remote address */
	args->local_addr  = gop->src;
	args->remote_addr = gop->dest;
	args->nbytes      = gop->len;

	return sizeof(firehose_remotecallback_args_t);
}

GASNETI_INLINE(gasnete_firehose_get)
gasnet_handle_t
gasnete_firehose_get(void *dest, gasnet_node_t node, void *src, 
		     size_t nbytes, gasnete_iop_t *iop GASNETE_THREAD_FARG)
{
	/* Request a Get in terms of a DMA put */
	gasnete_eop_t	*gop;

	firehose_remotecallback_args_t	args;

	if (iop) /* use a "dummy" eop */
          gop = gasneti_calloc(1, sizeof(gasnete_eop_t));
        else 
          gop = gasnete_eop_new(GASNETE_MYTHREAD);
	gop->dest = (uintptr_t) dest;
	gop->src = (uintptr_t) src;
	gop->len = nbytes;
	gop->node = node;
	gop->iop = iop;
	SET_OPMISC(gop, OPMISC_NONAMBUF);
	#if GASNETI_STATS_OR_TRACE
	gop->starttime = GASNETI_TICKS_NOW_IFENABLED(C);
	gop->fh_stats = fh_onesided;
	#endif

	if (iop != NULL)
		iop->initiated_get_cnt++;

	/* Always pin locally before sending the remote pin request. */
	gop->req_local = 
	    firehose_local_pin((uintptr_t) dest, nbytes, NULL);

	#ifdef GASNET_SEGMENT_FAST
	    /* We know the remote segment is pinned */
	    gasnete_fh_request_get(gop, &(gop->req_remote), 1);
	#else
	    /* Since Put is in reverse direction, the source is the local address
	     * and the destination is the remote address */
	    args.local_addr  = (uintptr_t) src;
	    args.remote_addr = (uintptr_t) dest;
	    args.nbytes      = nbytes;

	    firehose_remote_pin(node, (uintptr_t) src, nbytes,
		FIREHOSE_FLAG_ENABLE_REMOTE_CALLBACK,
		(firehose_request_t *) &(gop->req_remote),
		gasnete_fh_remote_args_fn,
		gasnete_fh_request_get_fn, gop);
	#endif

        return (iop?(gasnete_op_t *)iop:(gasnete_op_t *)gop);
}

extern gasnet_handle_t
gasnete_get_nb_bulk (void *dest, gasnet_node_t node, void *src, 
		     size_t nbytes GASNETE_THREAD_FARG)
{
	gasneti_boundscheck(node, src, nbytes);
	GASNETI_CHECKPSHM_GET(UNALIGNED,H);

	GASNETI_TRACE_PRINTF(C, 
	    ("gasnete_get_nb_bulk Firehose (%p <- %d,%p @ %d bytes)",
	    dest, (unsigned) node, src, (int)nbytes));

	return gasnete_firehose_get(dest, node, src, nbytes, 
		    NULL GASNETE_THREAD_PASS);
}

extern void
gasnete_get_nbi_bulk (void *dest, gasnet_node_t node, void *src, 
		      size_t nbytes GASNETE_THREAD_FARG)
{
	gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
	gasnete_iop_t *iop = mythread->current_iop;
	GASNETI_CHECKPSHM_GET(UNALIGNED,V);

	gasneti_boundscheck(node, src, nbytes);

	GASNETI_TRACE_PRINTF(C, 
	    ("gasnete_get_nb_bulk Firehose (%p <- %d,%p @ %d bytes)",
	    dest, (unsigned) node, src, (int)nbytes));
	gasnete_firehose_get(dest, node, src, nbytes, iop GASNETE_THREAD_PASS);

	return;
}

/* ##################################################################### */
/* Handlers                                                              */
/* ##################################################################### */
static gasnet_handlerentry_t const gasnete_handlers[] = {
	gasneti_handler_tableentry_with_bits(gasnete_get_dma_reqh),
	gasneti_handler_tableentry_with_bits(gasnete_get_dma_reph),
	{ 0, NULL }
};

extern gasnet_handlerentry_t const *gasnete_get_handlertable(void) {
	return gasnete_handlers;
}

#endif
