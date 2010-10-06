/*   $Source: /var/local/cvs/gasnet/other/firehose/firehose.c,v $
 *     $Date: 2009/08/15 10:01:31 $
 * $Revision: 1.38 $
 * Description: 
 * Copyright 2004, Christian Bell <csbell@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include <firehose.h>
#include <firehose_internal.h>

static firehose_request_t *fh_request_freehead = NULL;
static firehose_request_t *fh_request_new(firehose_request_t *ureq, int block);
static void fh_request_free(firehose_request_t *req);

/* Should we poll in the try/partial calls if we might be short on handles? */
#ifndef FH_POLL_FOR_HANDLES
#define FH_POLL_FOR_HANDLES 0
#endif

/* ##################################################################### */
/* LOCKS, FIFOS, ETC.                                                    */
/* ##################################################################### */

/* The following lock, referred to as the "table" lock, must be held for every
 * firehose operation that modifies the state of the firehose table.  It must
 * be held during most of the firehose operations - adding/removing to the hash
 * table, adding/removing from the local and victim FIFOs.
 */
gasneti_mutex_t         fh_table_lock = GASNETI_MUTEX_INITIALIZER;

#ifndef FH_POLL_NOOP 
  /* This lock protects the poll FIFO queue, used to enqueue callbacks. */
  gasneti_mutex_t         fh_pollq_lock = GASNETI_MUTEX_INITIALIZER;
#endif

/* Firehose FIFOs */
fh_fifoq_t      fh_LocalFifo = FH_TAILQ_HEAD_INITIALIZER(fh_LocalFifo);
fh_fifoq_t      *fh_RemoteNodeFifo = NULL;

/* Local Limits & Counters */
int fhc_LocalOnlyBucketsPinned;
int fhc_LocalVictimFifoBuckets;
int fhc_MaxVictimBuckets;

/* Remote Limits & Counters */
int fhc_RemoteBucketsM;
int fhc_MaxRemoteBuckets;
int *fhc_RemoteBucketsUsed;
int *fhc_RemoteVictimFifoBuckets;

/* Freelist of fhi_RegionPool_t */ 
static FH_STAILQ_HEAD(_fhi_regpool_list_t, _fhi_RegionPool_t) fhi_regpool_list;
static int fhi_regpool_num = 0;
static int fhi_regpool_numbig = 0;

/* Misc */
int fh_verbose = 0;

/* ##################################################################### */
/* PUBLIC FIREHOSE INTERFACE                                             */
/* ##################################################################### */
/* firehose_init()
 * firehose_fini()
 *    
 * firehose_local_pin() 
 *     calls fh_acquire_local_region()
 *
 * firehose_try_local_pin() 
 *     calls fh_commit_try_local_region()
 *
 * firehose_partial_local_pin()
 *     calls fh_commit_try_local_region()
 *
 * firehose_remote_pin()
 *     calls fh_acquire_remote_region()
 *
 * firehose_try_local_pin() 
 *     calls fh_commit_try_remote_region()
 *
 * firehose_partial_local_pin()
 *     calls fh_commit_try_remote_region()
 *
 * firehose_release()
 *     calls fh_release_local_region() or fh_release_remote_region()
 */

uint32_t	fhi_InitFlags = 0;

extern void
firehose_init(uintptr_t max_pinnable_memory,
	      size_t max_regions, size_t max_region_size,
	      const firehose_region_t *prepinned_regions,
              size_t num_prepinned, uint32_t flags, firehose_info_t *info)
{
	int	i;

	fh_verbose = gasneti_getenv_yesno_withdefault("GASNET_FIREHOSE_VERBOSE", 0);

	/* Make sure the refc field in buckets can also be used as a FIFO
	 * pointer */
	gasneti_assert(sizeof(fh_refc_t) == sizeof(void *));

	gasneti_assert(FH_MAXVICTIM_TO_PHYSMEM_RATIO >= 0 && 
		       FH_MAXVICTIM_TO_PHYSMEM_RATIO <= 1);

	/* validate the prepinned regions list */
	for (i = 0; i < num_prepinned; i++) {
		const firehose_region_t *region = &prepinned_regions[i];
		if (region->addr % FH_BUCKET_SIZE != 0)
			gasneti_fatalerror("firehose_init: prepinned "
					"region is not aligned on a bucket "
					"boundary (addr = %p)",
					(void *) region->addr);
                                                                                
		if (region->len % FH_BUCKET_SIZE != 0)
			gasneti_fatalerror("firehose_init: prepinned "
					"region is not a multiple of firehose "
					"bucket size in length (len = %d)",
					(int) region->len);
	}
                                                                                

	FH_TABLE_LOCK;

	/* Initialize the local firehose FIFO queue */
	FH_TAILQ_INIT(&fh_LocalFifo);

	/* hit the request_t freelist for first allocation */
	{
		firehose_request_t *req = fh_request_new(NULL, 0);
		gasneti_assert(req != NULL);
		fh_request_free(req);
	}

        /* Allocate the per-node FIFOs and counters */
	if (!(flags & FIREHOSE_INIT_FLAG_LOCAL_ONLY)) {
		fh_RemoteNodeFifo = (fh_fifoq_t *) 
			gasneti_malloc(gasneti_nodes * sizeof(fh_fifoq_t));
		fhc_RemoteBucketsUsed = (int *)
			gasneti_malloc(gasneti_nodes * sizeof(int));
		fhc_RemoteVictimFifoBuckets = (int *)
			gasneti_malloc(gasneti_nodes * sizeof(int));
		for (i = 0; i < gasneti_nodes; i++) {
			FH_TAILQ_INIT(&fh_RemoteNodeFifo[i]);
			fhc_RemoteBucketsUsed[i] = 0;
			fhc_RemoteVictimFifoBuckets[i] = 0;
		}
	}

        /* Init the region pool freelist */
        FH_STAILQ_INIT(&fhi_regpool_list);

	/* Initialize -page OR -region specific data. _MUST_ be the last thing
	 * called before return */
	fhi_InitFlags = flags;
	fh_init_plugin(max_pinnable_memory, max_regions, max_region_size,
		       prepinned_regions, num_prepinned, info);

	FH_TABLE_UNLOCK;

	return;
}

#define FH_REQUEST_ALLOC_PERIDX	256
#define FH_REQUEST_ALLOC_MAXIDX	256
static firehose_request_t	*fh_request_bufs[FH_REQUEST_ALLOC_MAXIDX] = { 0 };

extern int fh_dacount;

/*
 * XXX should call from gasnet_exit(), fatal or not
 *
 */
void
firehose_fini()
{
	int	i;

	fh_fini_plugin();

	/* Free the per-node firehose FIFO queues and counters */
	if (!(fhi_InitFlags & FIREHOSE_INIT_FLAG_LOCAL_ONLY)) {
		gasneti_free(fh_RemoteNodeFifo);
		gasneti_free(fhc_RemoteBucketsUsed);
		gasneti_free(fhc_RemoteVictimFifoBuckets);
	}

	/* Free the region pool freelist */
	while (!FH_STAILQ_EMPTY(&fhi_regpool_list)) {
		fhi_RegionPool_t *rpool;
		rpool = FH_STAILQ_FIRST(&fhi_regpool_list);
		FH_STAILQ_REMOVE_HEAD(&fhi_regpool_list);
		gasneti_free(rpool);
	}

	/* Deallocate the arrays of request_t buffers used, if applicable */
	for (i = 0; i < FH_REQUEST_ALLOC_MAXIDX; i++) {
		if (fh_request_bufs[i] == NULL)
			break;
		gasneti_free(fh_request_bufs[i]);
	}

	return;
}

/* firehose_poll()
 *
 * Empties the Callback Fifo Queue.
 *
 * XXX should make fh_callback_t allocated from freelists.
 */
#ifndef FH_POLL_NOOP
fh_pollq_t	fh_CallbackFifo = FH_STAILQ_HEAD_INITIALIZER(fh_CallbackFifo);

void
firehose_poll()
{
	fh_callback_t	*fhc;

	while (!FH_STAILQ_EMPTY(&fh_CallbackFifo)) {
		FH_POLLQ_LOCK;

		if (!FH_STAILQ_EMPTY(&fh_CallbackFifo)) {
			fhc = FH_STAILQ_FIRST(&fh_CallbackFifo);
			FH_STAILQ_REMOVE_HEAD(&fh_CallbackFifo);
			FH_POLLQ_UNLOCK;

			#ifndef FIREHOSE_COMPLETION_IN_HANDLER
			if (fhc->flags & FH_CALLBACK_TYPE_COMPLETION) {
				fh_completion_callback_t *cc =
					(fh_completion_callback_t *) fhc;
				/* we flag a local hit if this completion was
				 * not from an in-flight request */
				int localhit = 
				    !(cc->request->flags & FH_FLAG_INFLIGHT);
				cc->callback(cc->context, cc->request, localhit);
				fh_free_completion_callback(cc);
			}
			#endif

			#ifndef FIREHOSE_REMOTE_CALLBACK_IN_HANDLER
			#ifndef FIREHOSE_COMPLETION_IN_HANDLER
			else
			#endif
			if (fhc->flags & FH_CALLBACK_TYPE_REMOTE) {
				fh_remote_callback_t *rc =
					(fh_remote_callback_t *) fhc;
				firehose_remote_callback(rc->node, 
					rc->pin_list, rc->pin_list_num, 
					&(rc->args));

				/* Send an AMRequest to the reply handler */
				fh_send_firehose_reply(rc);
				gasneti_free(rc->pin_list);
				gasneti_free(fhc);
				continue;
			}
			#endif
		}
		else
			FH_POLLQ_UNLOCK;
	}

	return;
}
#endif

extern const firehose_request_t *
firehose_local_pin(uintptr_t addr, size_t nbytes, firehose_request_t *ureq)
{
	firehose_request_t	*req = NULL;

	FH_TABLE_LOCK;

	req = fh_request_new(ureq, 1);
	gasneti_assert(req != NULL);

	req->node   = gasneti_mynode;
	req->addr   = FH_ADDR_ALIGN(addr);
	req->len    = FH_SIZE_ALIGN(addr,nbytes);
	req->flags |= FH_FLAG_PINNED;
	GASNETI_TRACE_EVENT_VAL(C,FH_LOCAL_PIN,(req->len >> FH_BUCKET_SHIFT));

	/* HIT/MISS tracing is done in fh_acquire_local_region() */
	fh_acquire_local_region(req);

	FH_TABLE_UNLOCK;

	return req;
}

extern const firehose_request_t *
firehose_try_local_pin(uintptr_t addr, size_t len, firehose_request_t *ureq)
{
	firehose_request_t	*req = NULL;

	addr = FH_ADDR_ALIGN(addr);
	len  = FH_SIZE_ALIGN(addr,len);
	GASNETI_TRACE_EVENT_VAL(C,FH_TRY_LOCAL_PIN,(len >> FH_BUCKET_SHIFT));

#if FH_POLL_FOR_HANDLES
	if_pf (!ureq && !fh_request_freehead)
		FIREHOSE_AMPOLL();
#endif

	FH_TABLE_LOCK;
	if (fh_region_ispinned(gasneti_mynode, addr, len)) {
		req = fh_request_new(ureq, 0);
		if_pt (req != NULL) {
			req->node   = gasneti_mynode;
			req->addr   = addr;
			req->len    = len;
			req->flags |= FH_FLAG_PINNED;

			fh_commit_try_local_region(req);
			GASNETI_TRACE_EVENT(C,FH_TRY_LOCAL_HIT);
		}
		else {
			GASNETI_TRACE_EVENT(C,FH_TRY_LOCAL_FAIL);
		}
	}
	else {
		GASNETI_TRACE_EVENT(C,FH_TRY_LOCAL_MISS);
	}
	FH_TABLE_UNLOCK;

	return req;
}

extern const firehose_request_t *
firehose_partial_local_pin(uintptr_t addr, size_t len,
                           firehose_request_t *ureq)
{
	firehose_request_t	*req = NULL;

	addr = FH_ADDR_ALIGN(addr);
	len  = FH_SIZE_ALIGN(addr,len);
	GASNETI_TRACE_EVENT_VAL(C,FH_PARTIAL_LOCAL_PIN,(len >> FH_BUCKET_SHIFT));

#if FH_POLL_FOR_HANDLES
	if_pf (!ureq && !fh_request_freehead)
		FIREHOSE_AMPOLL();
#endif

	FH_TABLE_LOCK;
	if (fh_region_partial(gasneti_mynode, &addr, &len)) {
		req = fh_request_new(ureq, 0);
		if_pt (req != NULL) {
			req->node   = gasneti_mynode;
			req->addr   = addr;
			req->len    = len;
			req->flags |= FH_FLAG_PINNED;

			fh_commit_try_local_region(req);
			GASNETI_TRACE_EVENT(C,FH_PARTIAL_LOCAL_HIT);
		}
		else {
			GASNETI_TRACE_EVENT(C,FH_PARTIAL_LOCAL_FAIL);
		}
	}
	else {
		GASNETI_TRACE_EVENT(C,FH_PARTIAL_LOCAL_MISS);
	}
	FH_TABLE_UNLOCK;

	return req;
}

extern const firehose_request_t *
firehose_remote_pin(gasnet_node_t node, uintptr_t addr, size_t len,
		    uint32_t flags, firehose_request_t *ureq,
		    firehose_remotecallback_args_fn_t remote_args_callback,
		    firehose_completed_fn_t callback, void *context)
{
	firehose_request_t	*req = NULL;

	if_pf (node == gasneti_mynode)
		gasneti_fatalerror("Cannot request a Remote pin on a local node.");

	gasneti_assert(remote_args_callback == NULL ? 1 : 
		       (flags & FIREHOSE_FLAG_ENABLE_REMOTE_CALLBACK));

	FH_TABLE_LOCK;

	req = fh_request_new(ureq, 1);
	gasneti_assert(req != NULL);

	req->node = node;
	req->addr = FH_ADDR_ALIGN(addr); 
	req->len  = FH_SIZE_ALIGN(addr,len);
	GASNETI_TRACE_EVENT_VAL(C,FH_REMOTE_PIN,(req->len >> FH_BUCKET_SHIFT));

	fh_acquire_remote_region(req, callback, context, flags, remote_args_callback);

	/* Note that fh_acquire_remote_region unlocks before returning */
	FH_TABLE_ASSERT_UNLOCKED;

	if (req->flags & FH_FLAG_PINNED) {
		/* If the request could be entirely pinned, process the
		 * callback or return to user.  If it could not be pinned, the
		 * callback will be subsequently called from within the
		 * firehose library */
		GASNETI_TRACE_EVENT(C, FH_REMOTE_HIT);

		if (flags & FIREHOSE_FLAG_RETURN_IF_PINNED) {
			return req;
		}
		else {
			GASNETI_TRACE_PRINTF(C, 
			    ("Firehose callback req=%p", req));
			callback(context, req, 1);
		}
	}
	else if (req->flags & FH_FLAG_INFLIGHT) {
		GASNETI_TRACE_EVENT(C, FH_REMOTE_MISS);
	}
	else {
		GASNETI_TRACE_EVENT(C, FH_REMOTE_PENDING);
	}

	return NULL;
}

extern const firehose_request_t *
firehose_try_remote_pin(gasnet_node_t node, uintptr_t addr, size_t len,
			uint32_t flags, firehose_request_t *ureq)
{
	firehose_request_t	*req = NULL;

	if_pf (node == gasneti_mynode)
		gasneti_fatalerror("Cannot request a Remote pin on a local node.");

	addr = FH_ADDR_ALIGN(addr);
	len  = FH_SIZE_ALIGN(addr,len);
	GASNETI_TRACE_EVENT_VAL(C,FH_TRY_REMOTE_PIN,(len >> FH_BUCKET_SHIFT));

#if FH_POLL_FOR_HANDLES
	if_pf (!ureq && !fh_request_freehead)
		FIREHOSE_AMPOLL();
#endif

	FH_TABLE_LOCK;

	if (fh_region_ispinned(node, addr, len)) {
		req = fh_request_new(ureq, 0);
		if_pt (req != NULL) {
			req->node = node;
			req->addr = addr;
			req->len  = len;

			fh_commit_try_remote_region(req);
			GASNETI_TRACE_EVENT(C,FH_TRY_REMOTE_HIT);
		}
		else {
			GASNETI_TRACE_EVENT(C,FH_TRY_REMOTE_FAIL);
		}
	}
	else {
		GASNETI_TRACE_EVENT(C,FH_TRY_REMOTE_MISS);
	}
	FH_TABLE_UNLOCK;

	return req;
}

extern const firehose_request_t *
firehose_partial_remote_pin(gasnet_node_t node, uintptr_t addr,
                            size_t len, uint32_t flags,
                            firehose_request_t *ureq)
{
	firehose_request_t	*req = NULL;

	if_pf (node == gasneti_mynode)
		gasneti_fatalerror("Cannot request a Remote pin on a local node.");

	addr = FH_ADDR_ALIGN(addr);
	len  = FH_SIZE_ALIGN(addr,len);
	GASNETI_TRACE_EVENT_VAL(C,FH_PARTIAL_REMOTE_PIN,(len >> FH_BUCKET_SHIFT));

#if FH_POLL_FOR_HANDLES
	if_pf (!ureq && !fh_request_freehead)
		FIREHOSE_AMPOLL();
#endif

	FH_TABLE_LOCK;

	if (fh_region_partial(node, &addr, &len)) {
		req = fh_request_new(ureq, 0);
		if_pt (req != NULL) {
			req->node = node;
			req->addr = addr;
			req->len  = len;

			fh_commit_try_remote_region(req);
			GASNETI_TRACE_EVENT(C,FH_PARTIAL_REMOTE_HIT);
		}
		else {
			GASNETI_TRACE_EVENT(C,FH_PARTIAL_REMOTE_FAIL);
		}
	}
	else {
		GASNETI_TRACE_EVENT(C,FH_PARTIAL_REMOTE_MISS);
	}
	FH_TABLE_UNLOCK;

	return req;
}

extern void
firehose_release(firehose_request_t const **reqs, int numreqs)
{
	int			i;

	GASNETI_TRACE_EVENT_VAL(C, FH_RELEASE, numreqs);

	FH_TABLE_LOCK;

	for (i = 0; i < numreqs; i++) {
		gasneti_assert(!(reqs[i]->flags & FH_FLAG_PENDING));

		if (reqs[i]->node == gasneti_mynode)
			fh_release_local_region(
				(firehose_request_t *) reqs[i]);
		else
			fh_release_remote_region(
				(firehose_request_t *) reqs[i]);

		if (reqs[i]->flags & FH_FLAG_FHREQ)
			fh_request_free((firehose_request_t *) reqs[i]);
	}

	FH_TABLE_UNLOCK;

	return;
}

/* ##################################################################### */
/* COMMON FIREHOSE INTERFACE                                             */
/* ##################################################################### */

/* TODO: allocate from a pool */
fh_completion_callback_t *
fh_alloc_completion_callback()
{
	fh_completion_callback_t *cc;

	FH_TABLE_ASSERT_LOCKED;

	cc = gasneti_malloc(sizeof(fh_completion_callback_t));
	if_pf (cc == NULL)
		gasneti_fatalerror("malloc in remote callback");
	cc->flags = FH_CALLBACK_TYPE_COMPLETION;

	return cc;
}

void
fh_free_completion_callback(fh_completion_callback_t *cc)
{
	/* OK to free w/o the LOCK */

	gasneti_free(cc);
	return;
}

/* Although clients can pass a pointer to a request_t, the alternative is to
 * have the firehose library allocate a request_t and return it.  For the
 * latter case, allocation is done using a freelist allocator and the internal
 * pointer is used to link the request_t.
 */
static int			 fh_request_bufidx = 0;

static firehose_request_t *
fh_request_new(firehose_request_t *ureq, int block)
{
	firehose_request_t	*req;

	FH_TABLE_ASSERT_LOCKED;

	if_pt (ureq != NULL) {
		req = ureq;
		req->flags = 0;
		req->internal = NULL;
		return req;
	}

	if_pt (fh_request_freehead != NULL) {
		req = fh_request_freehead;
		fh_request_freehead = (firehose_request_t *) req->internal;
	}
	else if_pf (fh_request_bufidx == FH_REQUEST_ALLOC_MAXIDX) {
#if GASNETI_STATS_OR_TRACE
		gasneti_tick_t ticks = GASNETI_TICKS_NOW_IFENABLED(C);
#endif

		/* If !block we cannot poll even once because calls to
		 * fh_commit_*() could break if we release the lock here.
		 */
		if (!block)
			return NULL;

		do {
			FH_TABLE_UNLOCK;
			FIREHOSE_AMPOLL();
			FH_TABLE_LOCK;
		} while (fh_request_freehead == NULL);
		req = fh_request_freehead;
		fh_request_freehead = (firehose_request_t *) req->internal;

#if GASNETI_STATS_OR_TRACE
		ticks = GASNETI_TICKS_NOW_IFENABLED(C) - ticks;
		GASNETI_TRACE_EVENT_TIME(C, FH_REQUEST_STALL, ticks);
#endif
	}
	else {
		firehose_request_t	*buf;
		int			 i;


		GASNETI_STAT_EVENT_VAL(C, FH_REQUEST_ALLOC, FH_REQUEST_ALLOC_PERIDX);

		buf = (firehose_request_t *)
			gasneti_malloc(FH_REQUEST_ALLOC_PERIDX*
				       sizeof(firehose_request_t));

		fh_request_bufs[fh_request_bufidx] = buf;
		fh_request_bufidx++;

		memset(buf, 0, FH_REQUEST_ALLOC_PERIDX*
		       sizeof(firehose_request_t));

		for (i = 1; i < FH_REQUEST_ALLOC_PERIDX-1; i++)
			buf[i].internal = (firehose_private_t *) &buf[i+1];

		buf[i].internal = NULL;
		req = &buf[0];
		fh_request_freehead = &buf[1];
	}

	req->flags = FH_FLAG_FHREQ;
	req->internal = NULL;
			    
	return req;
}

static void
fh_request_free(firehose_request_t *req)
{
	/* Firehose allocated request, not pending */
	gasneti_assert((req->flags & (FH_FLAG_FHREQ | FH_FLAG_PENDING)) == FH_FLAG_FHREQ);

	/* XXX: could use a distinct lock here */
	req->internal = (firehose_private_t *) fh_request_freehead;
	fh_request_freehead = req;

	return;
}

/* region/page must provide implementations of these functions */

/* Data structures 
 *
 * Table(s) of firehose_private_t
 *   Adding: Entries are added once a private_t is pinned locally or a
 *           mapped to a remote location.
 *   Removing: Local private_t's are removed once memory is unpinned locally.
 *             Remote private_t's are removed when an AM move is required and
 *             that private_t had been selected for replacement.
 *   non-common implementation
 *
 */

/* 
 * fh_getenv()
 *
 * Read firehose environment variables, default to units given 
 *
 * Recognizes modifiers [Mm][Kk][Gg] in numbers 
 */ 
unsigned long fh_getenv(const char *var, unsigned long multiplier) {
  const char *env = gasneti_getenv(var);
  return gasneti_parse_int(env, multiplier);
}

/* 
 * Common data structures 
 *
 * Local Victim FIFO of firehose_private_t (oldest at head, newest at tail)
 *   Pushing: Adjacent entries are usually pushed in reverse-address order.
 *            This allows a subsequent popping operation to optimistically
 *            construct contiguous region_t's (for firehose-page).
 *   Popping: Entries are usually removed so as to create one contiguous
 *            region_t.
 *
 * Per-node firehose victim FIFO of firehose_private_t
 *   Pushing: Firehoses which reach a refcount of zero are added to the
 *            per-node firehose victim FIFO for possible replacement or reuse.
 *   Popping: An entry is removed when a node decides that it has used up all
 *            it's firehoses to a remote node and needs replacements.
 *
 * XXX/PHH: update to drop 'bucket' terminology here:
 *
 * Bucket state transitions
 *
 * Each bucket (whether local or remote) can be either pinned or unpinned.
 * Local buckets have a remote (R) and local (L) refcount whereas remote
 * buckets only have remote refcounts.
 *
 * Remote bucket handling is straightforward -- if R=0, the bucket is in the
 * remote fifo, and if R>0, it is in use.
 *
 * Local bucket handling is complicated by the L refcount and the necessity to
 * maintain the 'fhc_LocalOnlyBucketsPinned' counter (shown as LOnly below).
 *
 *********************************
 * LOCAL BUCKET STATE TRANSITIONS
 *********************************
 * Each state transition is triggered by acquire and release.
 *
 *           R L        
 *          .---.        
 *       A. |0 0| (UNPINNED)
 *          `---'          
 *          |  ^         
 *          |  | LOnly--
 *  LOnly++ |  |        
 *          V  |                              R L 
 *          .---. (PINNED)                   .---.
 *       B. |0 0| (IN FIFO) <-- -- -- -- --> |0 1| C. (PINNED)
 *          `---'                            `---'
 *          |  ^                               ^ 
 *          |  | LOnly++                       |
 *  LOnly-- |  |                               |
 *          V  |                LOnly--        V
 *          .---.           <-- -- -- -- --  .---.
 *       E. |1 0| (PINNED)   -- -- -- -- --> |1 1| D. (PINNED)
 *          `---'               LOnly++      `---'
 *
 * All transitions  _TO_  state 'B' add    the bucket to the FIFO
 * All transitions _FROM_ state 'B' remove the bucket to the FIFO
 *
 *********************************
 * REMOTE BUCKET STATE TRANSITIONS
 *********************************
 * State transitions triggers are indicated in the diagram
 * 
 *            C.                               B.
 *          .---. (PINNED)   acquire(),R=1   .---.
 *          |R=0| (IN FIFO) <-- -- -- -- --> |R>0| (PINNED)
 *          `---'            release(),R=0   `---'
 *                                             ^ 
 *                                             |  Firehose reply
 *                                             | 
 *                                             |
 *                                           .---. (UNPINNED, PENDING PIN)
 *          Firehose request -- -- -- -- --> |R>0| -- --.
 *          first acquire()                  `---'      |
 *                                        A.  ^         |  acquire()
 *                                            |_ __ __ / 
 *
 * - Some transitions from 'B' are missing, the transition to 'C' only happens
 *   when the reference count reaches zero.
 * - Subsequent acquires on a bucket pending pin (state 'A') cause firehose
 *   requests to be queued up at the sender.  In other words, completions can
 *   be coalesced by a single firehose reply.
 *
 *--
 * Acquiring a bucket increments the refcount (either R or L)
 * Release a bucket decrements the refcount (ether R or L)
 *
 * These functions return the new reference count for the incremented count.
 *
 * The name of the function indicates where the memory resides (local, remote).
 * In the local case, the 'local_ref' boolean argument indicates where the
 * request originated and in the remote case the 'node' argument indicates
 * where the memory resides.
 */

fh_refc_t *
fh_priv_acquire_local(int local_ref, firehose_private_t *entry)
{
	fh_refc_t	*rp = FH_BUCKET_REFC(entry);

	/*
	 * 'local_ref' is TRUE if we are acquiring a local bucket for the
	 *		local node (ie: fh_local_pin).  
	 * 'local_ref' is FALSE if we are acquireing a local bucket from a
	 *		firehose request (fh_am_move).
	 */
	gasneti_assert((local_ref == 0) || (local_ref == 1));

	FH_TABLE_ASSERT_LOCKED;
	
	/* 
	 * If the bucket is a local, if can contain both local and remote
	 * reference counts.
	 *
	 */
	gasneti_assert(entry != NULL);

	if (FH_IS_LOCAL_FIFO(entry)) {
		/* Bucket started in state "B" and is
		 * now entering state (local_ref ? "C" : "E")
		 */
		#if PLATFORM_COMPILER_SUN_C
		  gasneti_compiler_fence(); /* Work around bug 1857 */
		#endif
		FH_TAILQ_REMOVE(&fh_LocalFifo, entry);
		FH_BSTATE_ASSERT(entry, fh_local_fifo);

		rp->refc_l = local_ref;
		rp->refc_r = !local_ref;

		/* We must dec LOnly if entering state "E" */
		fhc_LocalOnlyBucketsPinned -= !local_ref;
		fhc_LocalVictimFifoBuckets--;
		FH_SET_LOCAL_INUSE(entry);
		FH_BSTATE_SET(entry, fh_used);

		FH_TRACE_BUCKET(entry, ACQFIFO);
	}
	else {
		/* Bucket started in state "C", "D" or "E" */
		FH_BSTATE_ASSERT(entry, fh_used);
		if (local_ref) {
			/* Bucket is entering state "C" or "D".  We
			 * must inc LOnly if coming from state "E" */
			fhc_LocalOnlyBucketsPinned += (rp->refc_l == 0);
			rp->refc_l++;
			FH_TRACE_BUCKET(entry, ACQUIRE);
		}
		else {
			/* Bucket is entering state "D" or "E" */
			rp->refc_r++;
			FH_TRACE_BUCKET(entry, ACQUIRE);
		}
	}

	gasneti_assert(FHC_MAXVICTIM_BUCKETS_AVAIL >= 0);

	return rp;
}

fh_refc_t *
fh_priv_acquire_remote(gasnet_node_t node, firehose_private_t *entry)
{
	fh_refc_t	*rp = FH_BUCKET_REFC(entry);

	FH_TABLE_ASSERT_LOCKED;
	
	gasneti_assert(entry != NULL);

	/* If the bucket is a remote bucket, the node cannot be equal to
	 * gasneti_mynode */
	gasneti_assert(node != gasneti_mynode);

	if (FH_IS_REMOTE_FIFO(entry)) {
	    FH_BSTATE_ASSERT(entry, fh_remote_fifo);
	    FH_TAILQ_REMOVE(&fh_RemoteNodeFifo[node], entry);

	    fhc_RemoteVictimFifoBuckets[node]--;
	    rp->refc_r = 1;
			
	    FH_SET_REMOTE_INUSE(entry);
	    FH_BSTATE_SET(entry, fh_used);
	    FH_TRACE_BUCKET(entry, ACQFIFO);
	}
	else {
	    gasneti_assert(!FH_IS_REMOTE_PENDING_UNCOMMITTED(entry));
	    gasneti_assert(rp->refc_r > 0);
	    rp->refc_r++;
	    FH_TRACE_BUCKET(entry, ACQUIRE);
	}

	return rp;
}

fh_refc_t *
fh_priv_release_local(int local_ref, firehose_private_t *entry)
{
	fh_refc_t	*rp = FH_BUCKET_REFC(entry);

	/*
	 * 'local_ref' is TRUE if we are releasing a local bucket for the
	 *		local node
	 * 'local_ref' is FALSE if we are releasing a local bucket from a
	 *		firehose request
	 *
	 */
	gasneti_assert((local_ref == 0) || (local_ref == 1));

	FH_TABLE_ASSERT_LOCKED;

	gasneti_assert(entry != NULL);
	FH_BSTATE_ASSERT(entry, fh_used);
	gasneti_assert(!FH_IS_LOCAL_FIFO(entry));

	if (local_ref) {
		gasneti_assert(rp->refc_l > 0);
	}
	else {
		gasneti_assert(rp->refc_r > 0);
	}

	rp->refc_l -= local_ref;
	rp->refc_r -= !local_ref;

	/* As a result, the bucket may be unused */
	if (rp->refc_r == 0 && rp->refc_l == 0) {
		/* Have entered state "B" (FIFO) */
		#ifdef FIREHOSE_REGION
		if (!entry->visible)
		    FH_TAILQ_INSERT_HEAD(&fh_LocalFifo, entry);
		else
		#endif
		    FH_TAILQ_INSERT_TAIL(&fh_LocalFifo, entry);

		/* We must inc LOnly if coming from state "E" */
		fhc_LocalOnlyBucketsPinned += !local_ref;
		fhc_LocalVictimFifoBuckets++;

		FH_BSTATE_SET(entry, fh_local_fifo);
		gasneti_assert(FH_IS_LOCAL_FIFO(entry));
		FH_TRACE_BUCKET(entry, ADDFIFO);
	}
	else {
		/* We must dec LOnly if entering state "E" from "D" */
		fhc_LocalOnlyBucketsPinned -= (rp->refc_l == 0 && local_ref);

		FH_TRACE_BUCKET(entry, RELEASE);
	}

	return rp;
}

fh_refc_t *
fh_priv_release_remote(gasnet_node_t node, firehose_private_t *entry)
{
	fh_refc_t	*rp = FH_BUCKET_REFC(entry);

	FH_TABLE_ASSERT_LOCKED;

	gasneti_assert(entry != NULL);
	FH_BSTATE_ASSERT(entry, fh_used);

	/* The bucket is a remote bucket, and it cannot contain any local
	 * refcounts.  Also, it should not be pending as pending buckets are
	 * handled separately */
	gasneti_assert(node != gasneti_mynode);
	gasneti_assert(FH_IS_REMOTE_INUSE(entry));
	gasneti_assert(rp->refc_r > 0);

	rp->refc_r--;

	if (rp->refc_r == 0) {
		#ifdef FIREHOSE_REGION
		if (!entry->visible)
		    FH_TAILQ_INSERT_HEAD(&fh_RemoteNodeFifo[node], entry);
		else
		#endif
		    FH_TAILQ_INSERT_TAIL(&fh_RemoteNodeFifo[node], entry);

		fhc_RemoteVictimFifoBuckets[node]++;

		FH_BSTATE_SET(entry, fh_remote_fifo);
		FH_TRACE_BUCKET(entry, ADDFIFO);
	}
	else {
		FH_TRACE_BUCKET(entry, RELEASE);
	}

	return rp;
}
/*
 * Waiting/Polling for local and remote firehoses
 *
 * The WaitLocalFirehoses() and WaitRemoteFirehoses() functions stall on the
 * appropriate FIFO waiting on the total number of firehoses available for
 * replacement.  The replacement firehoses are placed in the supplied array
 * or regions, which must be large enough to hold 'count' regions (worst
 * case is one region per firehose).  These functions return the number of
 * regions in the array, which may be less than the number of requested
 * firehoses if coalescing was possible.
 *
 * WaitLocalFirehoses() has the additional property of enforcing the limit
 * fhc_LocalOnlyBucketsPinned <= fhc_MaxVictimBuckets.
 *
 * TODO:  Firehose should implement a deadlock-free and starvation-free polling
 *        mechanism for threaded clients.
 */

int
fh_WaitLocalFirehoses(int count, firehose_region_t *region)
{
	int			b_remain, b_avail, r_freed;
	firehose_region_t	*reg = region;

	FH_TABLE_ASSERT_LOCKED;

	/* First grab any "free" buckets due to a less-than-full FIFO */
	gasneti_assert(FHC_MAXVICTIM_BUCKETS_AVAIL >= 0);
	b_avail = MIN(count, FHC_MAXVICTIM_BUCKETS_AVAIL);
	fhc_LocalOnlyBucketsPinned += b_avail;
#ifdef DEBUG_LOCAL_TABLE
	fhc_LocalReserved += b_avail;
#endif  

	b_remain = count - b_avail;

	if (b_remain == 0)
		return 0;

	GASNETI_TRACE_PRINTF(C, ("Firehose Polls Local pinned needs to recover"
	    " %d buckets from FIFO (currently %d buckets)", b_remain,
	    fhc_LocalVictimFifoBuckets));

	while (b_remain > 0) {
		b_avail = MIN(b_remain, fhc_LocalVictimFifoBuckets);

		if (b_avail > 0) {
			/* Adjusts LocalVictimFifoBuckets count */
			r_freed = fhi_FreeVictimLocal(b_avail, reg);
			fhc_LocalVictimFifoBuckets -= b_avail;
			b_remain -= b_avail;
			reg += r_freed;
		}
		else {
			FH_TABLE_UNLOCK;
			FIREHOSE_AMPOLL();
			FH_TABLE_LOCK;

			/* May have had a D->E state transition */
			b_avail = MIN(b_remain, FHC_MAXVICTIM_BUCKETS_AVAIL);
			fhc_LocalOnlyBucketsPinned += b_avail;
			b_remain -= b_avail;
		}

#ifdef DEBUG_LOCAL_TABLE
		fhc_LocalReserved += b_avail;
#endif  
	}

	gasneti_assert(FHC_MAXVICTIM_BUCKETS_AVAIL >= 0);

	return (int) (reg - region);
}

int
fh_WaitRemoteFirehoses(gasnet_node_t node, int count, 
			firehose_region_t *region)
{
	int			b_remain, b_avail, r_freed;
	firehose_region_t	*reg = region;

	FH_TABLE_ASSERT_LOCKED;

	b_remain = count;

	GASNETI_TRACE_PRINTF(C, 
	   ("Firehose Polls Remote firehoses requires %d buckets (FIFO=%d)",
	   count, fhc_RemoteVictimFifoBuckets[node]));

	while (b_remain > 0) {
		b_avail = MIN(b_remain, fhc_RemoteVictimFifoBuckets[node]);

		if (b_avail > 0) {
			r_freed = fhi_FreeVictimRemote(node, b_avail, reg);
			fhc_RemoteVictimFifoBuckets[node] -= b_avail;
			b_remain -= b_avail;
			reg += r_freed;
		}
		else {
			FH_TABLE_UNLOCK;
			FIREHOSE_AMPOLL();
			FH_TABLE_LOCK;
		}
	}

	gasneti_assert(fhc_RemoteVictimFifoBuckets[node] >= 0);
	gasneti_assert(reg - region > 0);

	return (int) (reg - region);
}

/* fh_AdjustLocalFifoAndPin(): Given a vector of regions to pin, this
 *    function makes the call to firehose_move_callback() by also including
 *    a list of regions to unpin if the victim FIFO was overcommitted.
 *      * called when releasing locally (w/ reg_pin == NULL)
 *	* called from the move AM handler
 */
void
fh_AdjustLocalFifoAndPin(gasnet_node_t node, firehose_region_t *reg_pin,
			size_t pin_num)
{
	int			b_unpin;

	FH_TABLE_ASSERT_LOCKED;

	/* Check if the local FIFO is overcommitted.  If so, we build a list of
	 * regions to unpin from the head of the FIFO (oldest victim).*/
	b_unpin = fhc_LocalOnlyBucketsPinned - fhc_MaxVictimBuckets;

	if (b_unpin > 0) {
                fhi_RegionPool_t *rpool;
		GASNETI_TRACE_PRINTF(C, 
		    ("Firehose Overcommitted FIFO by %d buckets", b_unpin));

		rpool = fhi_AllocRegionPool(b_unpin);
		rpool->buckets_num = b_unpin;
		rpool->regions_num =
			fhi_FreeVictimLocal(b_unpin, rpool->regions);

		fhc_LocalVictimFifoBuckets -= b_unpin;
		fhc_LocalOnlyBucketsPinned -= b_unpin;
		gasneti_assert(FHC_MAXVICTIM_BUCKETS_AVAIL >= 0);

		FH_TABLE_UNLOCK;
		firehose_move_callback(node, rpool->regions, 
				rpool->regions_num, reg_pin, pin_num);
		FH_TABLE_LOCK;

		fhi_FreeRegionPool(rpool);
	}
	else if (pin_num > 0) {
		FH_TABLE_UNLOCK;
		firehose_move_callback(node, NULL, 0, reg_pin, pin_num);
		FH_TABLE_LOCK;
	}

	return;
}

/*********************************
 * Freelist of fhi_RegionPool_t
 *********************************/

fhi_RegionPool_t *
fhi_AllocRegionPool(int count)
{
	fhi_RegionPool_t *rpool;

	FH_TABLE_ASSERT_LOCKED;

	rpool = FH_STAILQ_FIRST(&fhi_regpool_list);

	if_pf (count > FH_REGIONPOOL_DEFAULT_COUNT || rpool == NULL) {

		rpool = (fhi_RegionPool_t *) 
			gasneti_malloc(sizeof(fhi_RegionPool_t));
		rpool->regions_num = 0;
		rpool->buckets_num = 0;

		if (count > FH_REGIONPOOL_DEFAULT_COUNT) {
			rpool->len     = sizeof(firehose_region_t) * count;
			rpool->regions = (firehose_region_t *) 
					    gasneti_malloc(rpool->len);
			if_pf (rpool->regions == NULL)
				gasneti_fatalerror("malloc in RegionPool");
			fhi_regpool_numbig++;
			return rpool;
		}
		else {
			count          = FH_REGIONPOOL_DEFAULT_COUNT;
			rpool->len     = FH_REGIONPOOL_DEFAULT_COUNT * 
						sizeof(firehose_region_t);
			rpool->regions = (firehose_region_t *) 
					    gasneti_malloc(rpool->len);
			if_pf (rpool->regions == NULL)
				gasneti_fatalerror("malloc in RegionPool");

			fhi_regpool_num++;
			return rpool;
		}
	}
	else {
		FH_STAILQ_REMOVE_HEAD(&fhi_regpool_list);
		return rpool;
	}
}

void
fhi_FreeRegionPool(fhi_RegionPool_t *rpool)
{
	FH_TABLE_ASSERT_LOCKED;

	gasneti_assert(rpool != NULL);

	if_pf (rpool->len > 
	   FH_REGIONPOOL_DEFAULT_COUNT*sizeof(firehose_region_t)) {
		gasneti_free(rpool->regions);
		gasneti_free(rpool);
	}
	else {
		rpool->regions_num = 0;
		rpool->buckets_num = 0;
		FH_STAILQ_INSERT_TAIL(&fhi_regpool_list, rpool);
		gasneti_assert(!FH_STAILQ_EMPTY(&fhi_regpool_list));
	}

	return;
}

/*********************************
 * AM-related functions
 *********************************/

/*
 * Firehose AM Request Handler
 */
void
fh_am_move_reqh_inner(gasnet_token_t token, void *addr, size_t nbytes,
		      gasnet_handlerarg_t flags,
		      gasnet_handlerarg_t r_new,
		      gasnet_handlerarg_t r_old,
		      void *context)
{
	firehose_region_t	*new_reg, *old_reg;
	gasnet_node_t		node;
	int			ret = 1;
	int			hit_pending = 0;
	int			remote_callback = 0;

	gasnet_AMGetMsgSource(token, &node);

	new_reg = (firehose_region_t *) addr;
	old_reg = new_reg + r_new;

	#ifdef FIREHOSE_UNEXPORT_CALLBACK
	if (r_old > 0)
		firehose_unexport_callback(node, old_reg, r_old);
	#endif

	ret = fh_move_request(node, new_reg, r_new, old_reg, r_old, context);
	hit_pending = (ret == -1);
	remote_callback = (flags & FIREHOSE_FLAG_ENABLE_REMOTE_CALLBACK);

	#ifdef FIREHOSE_EXPORT_CALLBACK
	    if (r_new > 0)
		firehose_export_callback(node, new_reg, r_new);
	#endif

	/* If ret == -1, we hit a local bucket marked as pending (another
	 * thread has marked it to be pinned but has not updated the table
	 * saying the bucket is now pinned.  We handle this by enqueueing the
	 * request in a special "pending queue"
	 */
	gasneti_assert(hit_pending == -1 ? FIREHOSE_SMP : (1));

	/* If the user requires to run a remote callback, and the
	 * callback is not to be run in place, run it */ 

	#ifdef FIREHOSE_REMOTE_CALLBACK_IN_HANDLER
	/* Handle this case separately */
	if (remote_callback) {
	    firehose_remotecallback_args_t	*args =
		   (firehose_remotecallback_args_t *)(old_reg + r_old);
	    gasneti_assert(!hit_pending);

	    firehose_remote_callback(node, 
		(const firehose_region_t *) new_reg, r_new, args);

	    MEDIUM_REP(2,3,
		(token,
		fh_handleridx(fh_am_move_reph),
		new_reg,
		sizeof(firehose_region_t) * r_new,
		r_new,
		PACK(context)));

	    return;
	}
	#else
	if (hit_pending || remote_callback) {
	    fh_remote_callback_t *rc = (fh_remote_callback_t *) 
		gasneti_malloc(sizeof(fh_remote_callback_t));

	    rc->flags = 0;
	    rc->node  = node;
	    rc->pin_list_num = r_new;

	    rc->reply_len = sizeof(firehose_region_t) * r_new;
	    rc->context   = context;
	    rc->pin_list  = (firehose_region_t *)
			    gasneti_malloc(sizeof(firehose_region_t)*r_new);
	    memcpy(rc->pin_list, new_reg, rc->reply_len);

	    if (remote_callback) {
		firehose_remotecallback_args_t	*args = 
		    (firehose_remotecallback_args_t *)(old_reg + r_old);
		memcpy(&(rc->args), args, sizeof(firehose_remotecallback_args_t));
		rc->flags |= FH_CALLBACK_TYPE_REMOTE;
	    }

	    #if defined(FIREHOSE_PAGE) && FIREHOSE_SMP
	    if (hit_pending) {
		  rc->flags |= FH_CALLBACK_TYPE_PENDING;
		  FH_POLLQ_LOCK;
		  FH_TAILQ_INSERT_TAIL(&fhsmp_LocalPendingList, rc);
		  FH_POLLQ_UNLOCK;
	    }
	    else {
	    #endif
		FH_POLLQ_LOCK;
		FH_STAILQ_INSERT_TAIL(&fh_CallbackFifo, (fh_callback_t *) rc);
		FH_POLLQ_UNLOCK;
	    #if defined(FIREHOSE_PAGE) && FIREHOSE_SMP
	    }
	    #endif
	}
	#endif /* REMOTE_CALLBACK_IN_HANDLER */

	else {
		MEDIUM_REP(2,3,
			   (token,
			    fh_handleridx(fh_am_move_reph),
			    new_reg,
			    sizeof(firehose_region_t) * r_new,
			    r_new,
			    PACK(context)));
	}

	return;
}
MEDIUM_HANDLER(fh_am_move_reqh,4,5,
	       (token,addr,nbytes, a0, a1, a2, UNPACK (a3    )),
	       (token,addr,nbytes, a0, a1, a2, UNPACK2(a3, a4)));

/*
 * Firehose AM Reply Handler
 * Process (run or queue) pending completion callbacks which are satisfied
 * by the reply.
 */
void
fh_am_move_reph_inner(gasnet_token_t token, void *addr,
		      size_t nbytes, gasnet_handlerarg_t r_new,
		      void *context)
{
	firehose_region_t	*regions = (firehose_region_t *) addr;
	fh_pollq_t		pendCallbacks;
	int			numpend;
	gasnet_node_t		node;

	gasnet_AMGetMsgSource(token, &node);

	/* 
	 * At least one pending request is attached to a bucket, so process them
	 * and dynamically create a list in pendCallbacks
	 */

	FH_TABLE_LOCK;

	numpend = 
	    fh_find_pending_callbacks(node, regions, r_new, context, &pendCallbacks);

	FH_TABLE_UNLOCK;

	if (numpend > 0) {
		#ifdef FIREHOSE_COMPLETION_IN_HANDLER
		fh_completion_callback_t	*ccb, *ccb2;
		int				 hitlocal;

		ccb = (fh_completion_callback_t *)FH_STAILQ_FIRST(&pendCallbacks);
		while (ccb != NULL) {
			gasneti_assert(ccb != FH_COMPLETION_END);
			ccb2 = FH_STAILQ_NEXT(ccb);
			gasneti_assert(!(ccb->request->flags & FH_FLAG_PENDING));
			/* If this request was not in flight, the referenced
			 * pages are now pinned and the callback is called as
			 * if we have a hit */
			hitlocal = !(ccb->request->flags & FH_FLAG_INFLIGHT);
			ccb->callback(ccb->context, ccb->request, hitlocal);
			fh_free_completion_callback(ccb);
			ccb = ccb2;
		}
		#else
		
		FH_POLLQ_LOCK;
		gasneti_assert(!FH_STAILQ_EMPTY(&pendCallbacks));
		FH_STAILQ_MERGE(&fh_CallbackFifo, &pendCallbacks);
		gasneti_assert(!FH_STAILQ_EMPTY(&fh_CallbackFifo));
		FH_POLLQ_UNLOCK;
		#endif
	}
	else {
		gasneti_assert(FH_STAILQ_FIRST(&pendCallbacks) == NULL);
	}

	return;
}
MEDIUM_HANDLER(fh_am_move_reph,2,3,
	       (token,addr,nbytes, a0, UNPACK (a1    )),
	       (token,addr,nbytes, a0, UNPACK2(a1, a2)));


void
fh_send_firehose_reply(fh_remote_callback_t *rc)
{
	FH_TABLE_ASSERT_UNLOCKED;
	/* Run the "reply" handler as a request */
	MEDIUM_REQ(2,3,
	    (rc->node, fh_handleridx(fh_am_move_reph),
	     rc->pin_list, rc->reply_len, rc->pin_list_num,
	     PACK(rc->context)));
}

gasnet_handlerentry_t fh_am_handlers[] = {
        /* ptr-width dependent handlers */
        gasneti_handler_tableentry_with_bits(fh_am_move_reqh),
        gasneti_handler_tableentry_with_bits(fh_am_move_reph),
        { 0, NULL }
};

gasnet_handlerentry_t *
firehose_get_handlertable(void) {
        return fh_am_handlers;
}
