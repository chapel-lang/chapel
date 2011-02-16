/*   $Source: /var/local/cvs/gasnet/other/firehose/firehose_page.c,v $
 *     $Date: 2009/04/27 21:37:04 $
 * $Revision: 1.57 $
 * Description: 
 * Copyright 2004, Christian Bell <csbell@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include <firehose.h>
#include <firehose_internal.h>
#include <gasnet.h>

#ifdef FIREHOSE_PAGE

#include <firehose_hash.c> /* For possible inlining */

typedef firehose_private_t fh_bucket_t;


/* 
 * There is currently no support for bind callbacks in firehose-page 
 * as no client_t is currently envisioned in known -page clients. 
 */

#if defined(FIREHOSE_BIND_CALLBACK) || defined(FIREHOSE_UNBIND_CALLBACK)
  #error firehose-page currently has no support for bind/unbind callbacks
#endif

/* 
 * For b_num buckets to be pinned and a function that uses coalescing to
 * create regions to be pinned, a worst case number of buckets is
 * calculated as follows:
 *
 * For each region (contiguous buckets), a worst case of (buckets+1)/2 new
 * region_t's are required to hold unpinned buckets.
 *    
 * For example, new_r = { regA, regB } and new_num = 2.
 *
 * regA = 5 buckets, 3 unpinned of the form 10101
 * regB = 3 buckets, 2 unpinned of the form 101
 *
 * yields a calulation of (5+1)/2 + (3+1)/2 = 3 + 2  = 5
 *
 * Another example, new_r = { regA, regB, regC } and new_num = 3.
 *
 * regA = 1 unpinned bucket
 * regB = 1 unpinned bucket
 * regC = 1 unpinned bucket
 *
 * yields a calculation of 3 * (1+1)/2 = 3 * 1 = 3
 *
 */

#define FH_MIN_REGIONS_FOR_BUCKETS(buckets)	(((buckets)+1)>>1)
#define FH_MAX_BUCKETS_FOR_REGIONS(regions)	(((regions)<<1)-1)

/* ##################################################################### */
/* PAGE-SPECIFIC FUNCTIONS                                               */
/* ##################################################################### */

/*
 * Local region handling
 *
 * Local regions can either be acquired by a local pin request or from an
 * AM handler as part of a remote firehose pin request.  All three of the
 * functions below are called from both AM handler context and local pin
 * request:
 *
 * 1. AcquireLocalRegionsList(): Given a list of input regions to pin, an
 *    unpinned list of regions is returned by way of a RegionPool.
 *      * called by fh_acquire_local_region() 
 *      * called by fh_am_move_reqh()
 *
 * 2. ReleaseLocalRegionsList(): Given a list of input regions to unpin,
 *    buckets reaching a refcount of zero are appended to (and possibly
 *    overcommit) the local victim FIFO.
 *      * called by fh_release_local_region() 
 *      * called by fh_am_move_reqh()
 */

int	fhi_AcquireLocalRegionsList(int local_ref, 
		firehose_region_t *region, size_t reg_num, 
		fhi_RegionPool_t *rpool);

void	fhi_ReleaseLocalRegionsList(int local_ref, firehose_region_t *reg, 
				size_t reg_num);

void	fh_dump_counters(void);

/* ##################################################################### */
/* BUFFERS AND QUEUES                                                    */
/* ##################################################################### */

/* 
 * GLOBAL SCRATCH BUFFERS
 *
 * The following two scratch buffers are used as temporary arrays to store
 * both bucket addresses and firehose bucket entry pointers (they are
 * allocated at startup).
 *
 * fh_temp_buckets[] is used to store bucket addresses between the
 * TryAcquireRemoteRegion() and CoalesceBuckets() phases of a remote pin
 * request.
 *
 * fh_temp_bucket_ptrs[] is used exclusively by FlushPendingRequests() as
 * part of a two phase operation in first marking pending buckets as
 * non-pending and then processing requests that were attached to these
 * pending buckets.
 */
static int		fh_max_regions = 0;
static uintptr_t *	fh_temp_buckets = NULL;
static fh_bucket_t **	fh_temp_bucket_ptrs = NULL;

static void fh_dump_fhparams(FILE *fp);

/*
 * The bucket table
 */
fh_hash_t	*fh_BucketTable;

/*
 * Some data structures for firehose-region smp
 *
 * da[]	    Array of deadlock avoidance integers (one per node)
 */
#if FIREHOSE_SMP
#define	FH_DACOUNT_MAX	    30

int	*fh_da;
int	 fh_local_da = 0;
int	 fh_localpending = 0;

/* XXX debug */
int	fh_dacount = 0;

fh_locpendq_t  fhsmp_LocalPendingList = 
		    FH_TAILQ_HEAD_INITIALIZER(fhsmp_LocalPendingList);

static gasneti_cond_t fh_local_da_cv = GASNETI_COND_INITIALIZER;

#define FH_UPYL                                                      \
  do {                                                               \
      FH_TABLE_UNLOCK;                                               \
      FIREHOSE_AMPOLL();                                             \
      gasneti_sched_yield();  /* Should this be GASNET_WAITHOOK? */  \
      FH_TABLE_LOCK;                                                 \
  } while (0)

#define FH_UPYUL                                                     \
  do {                                                               \
      FH_TABLE_UNLOCK;                                               \
      FIREHOSE_AMPOLL();                                             \
      gasneti_sched_yield();  /* Should this be GASNET_WAITHOOK? */  \
      FIREHOSE_AMPOLL();                                             \
      FH_TABLE_LOCK;                                                 \
  } while (0)
#endif /* FIREHOSE_SMP */

#define FHI_AVAIL(node)  (						  \
     (fhc_RemoteBucketsM - fhc_RemoteBucketsUsed[node]) /* Free energy */ \
        + fhc_RemoteVictimFifoBuckets[node] /* FIFO */                    \
    )

/* ##################################################################### */
/* UTILITY FUNCTIONS FOR REGIONS AND BUCKETS                             */
/* ##################################################################### */

static fh_bucket_t      *fh_buckets_bufs[FH_BUCKETS_BUFS] = { 0 };
static fh_bucket_t	*fh_buckets_freehead = NULL;
static int		 fh_buckets_bufidx = 0;
static int		 fh_buckets_per_alloc = 0;

static void
fh_bucket_init_freelist(int max_buckets_pinned)
{
	FH_TABLE_ASSERT_LOCKED;

	/* XXX this should probably be further aligned. . */
	fh_buckets_per_alloc = (int) MAX( 
	    ((max_buckets_pinned + (FH_BUCKETS_BUFS-1)) / FH_BUCKETS_BUFS),
	    (1024));

	fh_buckets_freehead = NULL; 

	return;
}

GASNETI_INLINE(fh_bucket_lookup)
fh_bucket_t *
fh_bucket_lookup(gasnet_node_t node, uintptr_t bucket_addr)
{
	FH_TABLE_ASSERT_LOCKED;

	FH_ASSERT_BUCKET_ADDR(bucket_addr);

	return (fh_bucket_t *)
			fh_hash_find(fh_BucketTable,
				     FH_KEYMAKE(bucket_addr, node));
}

static fh_bucket_t *
fh_bucket_add(gasnet_node_t node, uintptr_t bucket_addr)
{
	fh_bucket_t	*entry;

	FH_TABLE_ASSERT_LOCKED;
	FH_ASSERT_BUCKET_ADDR(bucket_addr);

	/* allocate a new bucket for the table */
	if (fh_buckets_freehead != NULL) {
		entry = fh_buckets_freehead;
		fh_buckets_freehead = entry->fh_next;
	}
	else {
		fh_bucket_t	*buf;
		int		 i;

		if (fh_buckets_bufidx == FH_BUCKETS_BUFS)
			gasneti_fatalerror("Firehose: Ran out of "
				"hash entries (limit=%d)",
				FH_BUCKETS_BUFS*fh_buckets_per_alloc);

		buf = (fh_bucket_t *) 
			gasneti_malloc(fh_buckets_per_alloc*
				       sizeof(fh_bucket_t));
		if (buf == NULL)
			gasneti_fatalerror("Couldn't allocate buffer "
			    "of buckets");

		memset(buf, 0, fh_buckets_per_alloc*sizeof(fh_bucket_t));

		fh_buckets_bufs[fh_buckets_bufidx] = buf;
		fh_buckets_bufidx++;

		for (i = 1; i < fh_buckets_per_alloc-1; i++)
			buf[i].fh_next = &buf[i+1];

		buf[i].fh_next = NULL;
		entry = &buf[0];
		entry->fh_next = NULL;

		fh_buckets_freehead = &buf[1];
	}

	entry->fh_key = FH_KEYMAKE(bucket_addr, node);

	fh_hash_insert(fh_BucketTable, entry->fh_key, entry);
	gasneti_assert(entry ==
		(fh_bucket_t *)fh_hash_find(fh_BucketTable, entry->fh_key));

	return entry;
}

static void
fh_bucket_remove(fh_bucket_t *bucket)
{
	void *entry;

	FH_TABLE_ASSERT_LOCKED;

	entry = fh_hash_insert(fh_BucketTable, bucket->fh_key, NULL);
	gasneti_assert(entry == (void *)bucket);

	memset(bucket, 0, sizeof(fh_bucket_t));
	bucket->fh_next = fh_buckets_freehead;
	fh_buckets_freehead = bucket;
}

/*
 * Add a bucket addr to a pool of regions, and attempt to coalesce it in the
 * process
 */
GASNETI_INLINE(fh_bucket_AddtoPool)
void
fh_bucket_AddtoPool(fhi_RegionPool_t *rpool, uintptr_t bucket_addr)
{
    int	last_r = rpool->regions_num - 1;
    FH_TABLE_ASSERT_LOCKED;

    /* First region/bucket */
    if_pf (last_r < 0) {
	rpool->regions[0].addr = bucket_addr;
	rpool->regions[0].len  = FH_BUCKET_SIZE;
	rpool->regions_num = 1;
	gasneti_assert(rpool->buckets_num == 0);
    }
    else if ((rpool->regions[last_r].addr + rpool->regions[last_r].len) 
	      == bucket_addr) 
    {
	/* Current bucket_addr is contiguous to the last one */
	rpool->regions[last_r].len += FH_BUCKET_SIZE;
    }
    else {
	/* Current bucket_addr is *not* contiguous to the last one */
	last_r++;
	rpool->regions[last_r].addr = bucket_addr;
	rpool->regions[last_r].len  = FH_BUCKET_SIZE;
	rpool->regions_num++;
    }

    /* Always increment bucket count */
    rpool->buckets_num++;
}

/*
 * Remove the last bucket from the pool and return its address
 */
uintptr_t
fh_bucket_PopfromPool(fhi_RegionPool_t *rpool)
{
    firehose_region_t	*last_reg;
    uintptr_t		 last_addr = 0;

    last_reg = &rpool->regions[rpool->regions_num];

    /* Last region has one bucket, and it is the last */
    if (last_reg->len == FH_BUCKET_SIZE) {
	last_addr = last_reg->addr;
	last_reg->addr = 0;
	last_reg->len = 0;
	rpool->buckets_num--;
	if (rpool->regions_num > 0)
	    rpool->regions_num--;
    }
    /* Last region is multi-bucket */
    else {
	last_reg->len -= FH_BUCKET_SIZE;
	rpool->buckets_num--;
	last_addr = last_reg->addr + last_reg->len - FH_BUCKET_SIZE;

    }

    return last_addr;
}

/* fh_region_ispinned(node, addr, len)
 * 
 * Returns non-null if the entire region is already pinned 
 *
 * Uses fh_bucket_lookup() to query if the current page is pinned.
 */
int
fh_region_ispinned(gasnet_node_t node, uintptr_t addr, size_t len)
{
 	uintptr_t	bucket_addr;
	uintptr_t	end_addr = addr + len - 1;
	fh_bucket_t	*bd;
	int		is_local = (node == gasneti_mynode);

	FH_TABLE_ASSERT_LOCKED;
 	FH_FOREACH_BUCKET(addr, end_addr, bucket_addr) {
		bd = fh_bucket_lookup(node, bucket_addr);

		/* 
		 * Upon lookup, the bucket can either not be present in the
		 * hash table in which case it is certainly unpinned, or it
		 * can be in the table but be pending.  If the bucket is
		 * pending a firehose move, the region cannot be declared as
		 * pinned.
		 */
		if (bd == NULL || (!is_local && FH_IS_REMOTE_PENDING(bd)))
			return 0;
	}
	return 1;
}

/* fh_commit_try_local_region(req)
 *
 * Applies the result of a successful 'try'.  So the assumption holds that
 * the buckets in 'req' are already pinned.
 */
void
fh_commit_try_local_region(firehose_request_t *req)
{
    uintptr_t	end_addr, bucket_addr;
    fh_bucket_t	*bd;

    FH_TABLE_ASSERT_LOCKED;
    gasneti_assert(req->node == gasneti_mynode);

    /* Make sure the size of the region respects the local limits */
    gasneti_assert(
	FH_NUM_BUCKETS(req->addr, req->len) <= fhc_MaxVictimBuckets);

    end_addr = req->addr + req->len - 1;
				
    FH_FOREACH_BUCKET(req->addr, end_addr, bucket_addr) 
    {
	gasneti_assert(bucket_addr > 0);
	bd = fh_bucket_lookup(gasneti_mynode, bucket_addr);
	gasneti_assert(bd != NULL);
	fh_priv_acquire_local(1, bd);
    }

    return;
}

void
fh_commit_try_remote_region(firehose_request_t *req)
{
	uintptr_t	bucket_addr, end_addr  = req->addr + req->len - 1;
	fh_bucket_t	*bd;
	gasnet_node_t	node = req->node;

	FH_TABLE_ASSERT_LOCKED;

	/* Make sure the size of the region respects the remote limits */
	gasneti_assert(FH_NUM_BUCKETS(req->addr, req->len)
						<= fhc_MaxRemoteBuckets);

 	FH_FOREACH_BUCKET(req->addr, end_addr, bucket_addr) {
		bd = fh_bucket_lookup(node, bucket_addr);
		fh_priv_acquire_remote(node, bd);
	}
	return;
}

/* fh_release_local_region(request)
 *
 * DECrements/unpins pages covered in 
 *     [request->addr, request->addr+request->len].
 */
void
fh_release_local_region(firehose_request_t *request)
{
	firehose_region_t	reg;
	int			b_total;

	FH_TABLE_ASSERT_LOCKED;

	b_total = FH_NUM_BUCKETS(request->addr, request->len);
	FH_COPY_REQUEST_TO_REGION(&reg, request);

	fhi_ReleaseLocalRegionsList(1, &reg, 1);
	fh_AdjustLocalFifoAndPin(gasneti_mynode, NULL, 0);

	return;
}

/*
 * fh_FreeVictim(count, region_array, head)
 *
 * This function removes 'count' buckets from the victim FIFO (local or
 * remote), and fills the region_array with regions suitable for
 * move_callback.
 * It returns the amount of regions (not buckets) created in the 
 * region_array.
 *
 * NOTE: it is up to the caller to make sure the region array can fit at most
 *       'buckets_topin' regions (ie: uncontiguous in the victim FIFO).
 *
 */

int
fh_FreeVictim(int buckets, firehose_region_t *reg, fh_fifoq_t *fifo_head)
{
	int		i, j;
	fh_bucket_t	*bd;
	uintptr_t	next_addr = 0;

	FH_TABLE_ASSERT_LOCKED;

	/* There must be enough buckets in the victim FIFO to unpin.  This
	 * criteria should always hold true per the constraints on
	 * fhc_LocalOnlyBucketsPinned. */
	for (i = 0, j = -1; i < buckets; i++) {
		bd = FH_TAILQ_FIRST(fifo_head);

		if (i > 0 && FH_BADDR(bd) == next_addr)
			reg[j].len += FH_BUCKET_SIZE;
		else {
			++j;
			reg[j].addr = FH_BADDR(bd);
			reg[j].len = FH_BUCKET_SIZE;
		}

		FH_TRACE_BUCKET(bd, REMFIFO);

		/* Remove the bucket descriptor from the FIFO and hash */
		FH_TAILQ_REMOVE(fifo_head, bd);
		fh_bucket_remove(bd);

		/* Next contiguous bucket address */
		next_addr = FH_BADDR(bd) + FH_BUCKET_SIZE;
	}
	gasneti_assert(buckets == j+1);
	return j+1;
}

static uintptr_t   fh_M         = (uintptr_t) 0;
static uintptr_t   fh_Maxvictim = (uintptr_t) 0;
static uintptr_t   fh_MaxPinnableMemory = (uintptr_t) 0;

/*
 * Given an amount of max_pinnable_memory, return what the largest 'M'
 * parameter can be.
 */

#define FH_PRINTMB(d)	    ((int)((d)/(1024*1024)))

void
firehose_get_params(uintptr_t max_pinnable_memory, 
		 uintptr_t *M, uintptr_t *Maxvictim)
{
    uintptr_t nM, nMaxvictim;
    int dfltM, dfltMaxvictim;

    if (fh_MaxPinnableMemory == max_pinnable_memory && fh_M && fh_Maxvictim) {
	*M = fh_M;
	*Maxvictim = fh_Maxvictim;
	return;
    }

    fh_MaxPinnableMemory = max_pinnable_memory;

    nM = fh_getenv("GASNET_FIREHOSE_M", (1<<20));
    dfltM = !nM;
    nMaxvictim   = fh_getenv("GASNET_FIREHOSE_MAXVICTIM_M", (1<<20));
    dfltMaxvictim = !nMaxvictim;

    /* First assign values based on either what the user passed or what
     * is determined to be the best M and maxvictim parameters based on
     * max_pinnable_memory and FH_MAXVICTIM_TO_PHYSMEM_RATIO */

    if (nM == 0 && nMaxvictim == 0) {
    	nM         = max_pinnable_memory * (1-FH_MAXVICTIM_TO_PHYSMEM_RATIO);
    	nMaxvictim = max_pinnable_memory * FH_MAXVICTIM_TO_PHYSMEM_RATIO;
    }
    else if (nM == 0) {
	if_pf (nMaxvictim > max_pinnable_memory)
	    gasneti_fatalerror("GASNET_FIREHOSE_MAXVICTIM_M (%d Mb) is larger "
		"than the amount of determined pinnable memory (%d Mb)\n", 
		FH_PRINTMB(nMaxvictim), FH_PRINTMB(max_pinnable_memory));
    	nM = max_pinnable_memory - nMaxvictim;
    }
    else if (nMaxvictim == 0) {
	if_pf (nM > max_pinnable_memory)
	    gasneti_fatalerror("GASNET_FIREHOSE_MAXVICTIM_M (%d Mb) is larger "
		"than the amount of determined pinnable memory (%d Mb)\n", 
		FH_PRINTMB(nM), FH_PRINTMB(max_pinnable_memory));
    	nMaxvictim = max_pinnable_memory - nM;
    }
    gasneti_envint_display("GASNET_FIREHOSE_M", nM, dfltM, 1);
    gasneti_envint_display("GASNET_FIREHOSE_MAXVICTIM_M", nMaxvictim, dfltMaxvictim, 1);

    /* 
     * Validate firehose parameters parameters 
     */ 
    {
        uintptr_t M_min = FH_BUCKET_SIZE * gasneti_nodes * 32;
        uintptr_t maxvictim_min = FH_BUCKET_SIZE * 4096;

        if_pf (nM < M_min)
    	gasneti_fatalerror("GASNET_FIREHOSE_M is less"
    		    "than the minimum %lu (%lu buckets)", 
		    (unsigned long) M_min, 
    		    (unsigned long) (M_min >> FH_BUCKET_SHIFT));

        if_pf (nMaxvictim < maxvictim_min)
    	gasneti_fatalerror("GASNET_MAXVICTIM_M is less than the "
    		    "minimum %lu (%lu buckets)", 
		    (unsigned long) maxvictim_min,
    		    (unsigned long) (maxvictim_min >> FH_BUCKET_SHIFT));
    }

    fh_M = *M = nM;
    fh_Maxvictim = *Maxvictim = nMaxvictim;
    return;
}


/* fh_init_plugin()
 *
 * This function is only called from firehose_init and allows -page OR
 * -region to run plugin specific code.
 */

void
fh_init_plugin(uintptr_t max_pinnable_memory,
	       size_t max_regions, size_t max_region_size,
	       const firehose_region_t *regions, size_t num_prepinned,
	       firehose_info_t *fhinfo)
{
    int	      i;
    uintptr_t M, maxvictim, firehoses, m_prepinned = 0;
    size_t    b_prepinned = 0;

    if (fhi_InitFlags & FIREHOSE_INIT_FLAG_LOCAL_ONLY) {
      fprintf(stderr, "Warning: firehose_page does not yet implement FIREHOSE_INIT_FLAG_LOCAL_ONLY (resource allocation may not be as desired).\n");
    }
    if (fhi_InitFlags & FIREHOSE_INIT_FLAG_UNPIN_ON_FINI) {
      fprintf(stderr, "Warning: firehose_page does not yet implement FIREHOSE_INIT_FLAG_UNPIN_ON_FINI (resources may not be released on exit as desired).\n");
    }

        /* Initialize the Bucket table to 128k lists */
    fh_BucketTable = fh_hash_create((1<<17));

    /* 
     * In -page, we ignore regions. . there should not be a limit on the
     * number of regions 
     */ 

    if (max_regions != 0)
    	gasneti_fatalerror("firehose-page does not support a "
    			   "limitation on the number of regions");
    if (max_region_size != 0)
    	gasneti_fatalerror("firehose-page does not support a "
    			   "limitation on region size");
    /*
     * Prepin optimization: PHASE 1.
     *
     * Count the number of buckets that are set as prepinned.
     *
     */
    if (num_prepinned > 0) {
        int	i;

        for (i = 0; i < num_prepinned; i++) {
    	gasneti_assert(regions[i].addr % FH_BUCKET_SIZE == 0);
    	gasneti_assert(regions[i].len % FH_BUCKET_SIZE == 0);
    	b_prepinned += 
    	    FH_NUM_BUCKETS(regions[i].addr,regions[i].len);

        }
    }
    m_prepinned = FH_BUCKET_SIZE * b_prepinned;

    firehose_get_params(max_pinnable_memory, (uintptr_t *)&M, 
		     (uintptr_t *)&maxvictim);

    if_pf (M < m_prepinned)
    	gasneti_fatalerror("Too much memory prepinned (%lu) for current "
    		    "GASNET_FIREHOSE_M parameter (%lu)", 
		    (unsigned long) m_prepinned, (unsigned long) M);

    fh_MaxPinnableMemory = max_pinnable_memory;
    if (fh_verbose)
	fh_dump_fhparams(stderr);

    GASNETI_TRACE_PRINTF(C, ("Firehose M=(%d MB,%ld), MAXVICTIM_M=(%d MB,%ld)", 
		    FH_PRINTMB(M), (unsigned long) M, 
		    FH_PRINTMB(maxvictim), (unsigned long) maxvictim));

    /* 
     * Set local parameters
     */
    fhc_LocalOnlyBucketsPinned = b_prepinned;
    fhc_LocalVictimFifoBuckets = 0;
    fhc_MaxVictimBuckets = (maxvictim + m_prepinned) >> FH_BUCKET_SHIFT;

    /* 
     * Set remote parameters
     */
    firehoses = (M - m_prepinned) >> FH_BUCKET_SHIFT;
    fhc_RemoteBucketsM = gasneti_nodes > 1
    			? firehoses / (gasneti_nodes-1)
    			: firehoses;

    /* Initialize bucket freelist with the total amount of buckets
     * to be pinned (including the ones the client passed) */
    fh_bucket_init_freelist(firehoses + fhc_MaxVictimBuckets);

    /*
     * Prepin optimization: PHASE 2.
     *
     * In this phase, the firehose parameters have been validated and the
     * buckets are added to the firehose table and sent to the FIFO
     *
     */
    if (num_prepinned > 0) {
        uintptr_t	bucket_addr, end_addr;
        fh_bucket_t	*bd;

        for (i = 0; i < num_prepinned; i++) {
	    end_addr = regions[i].addr + regions[i].len - 1;

	    FH_FOREACH_BUCKET(regions[i].addr, end_addr, bucket_addr) {

		bd = fh_bucket_add(gasneti_mynode, bucket_addr);

		FH_BSTATE_SET(bd, fh_used);
		FH_SET_LOCAL_INUSE(bd);
		FH_BUCKET_REFC(bd)->refc_l = 1;
		FH_BUCKET_REFC(bd)->refc_r = 0;
		#ifdef DEBUG_BUCKETS
		bd->prepinned = 1;
		#endif
		/*
		 * Eventually, we may want to have an option where the client
		 * passes a list of regions to prepin, but allow that these
		 * pinned pages be unpinned as replacements later */
		#if 0
		if (gasneti_getenv_withdefault("GASNET_FIREHOSE_REUSE_PREPINNED",0)
		    fh_priv_release_local(1, bd);
		#endif
	    }
        }
        GASNETI_TRACE_PRINTF(C, ("Firehose Prepinned (%p,%d) total=%d bytes",
	     (void *) regions[0].addr, (int) regions[0].len,
	     (int) b_prepinned));
    }

    /* 
     * Set fields in the firehose information type, according to the
     * limits established by the firehose parameters.
     */
    {
        unsigned	med_regions, med_buckets;

        med_regions = (gasnet_AMMaxMedium() 
    			- sizeof(firehose_remotecallback_args_t))
    			/ sizeof(firehose_region_t);

    	/* 
    	 * For med_regions possible regions in the AMMedium, the
    	 * worse case is drawn up as the following, a 1-for-1
    	 * replacement of old and new regions, which leaves a worse
    	 * case of med_regions/2.
    	 *
    	 */

        med_buckets  = FH_MAX_BUCKETS_FOR_REGIONS(med_regions);
        fhc_MaxRemoteBuckets = MIN(med_buckets/2, fhc_RemoteBucketsM);

        fhinfo->max_RemoteRegions = 0;
        fhinfo->max_LocalRegions= 0;

        fhinfo->max_LocalPinSize  = 
    	    fhc_MaxVictimBuckets * FH_BUCKET_SIZE;
        fhinfo->max_RemotePinSize = 
    	    fhc_MaxRemoteBuckets * FH_BUCKET_SIZE;
        fh_max_regions = 
		MAX(
		    FH_MIN_REGIONS_FOR_BUCKETS(fhc_MaxVictimBuckets),
    		    FH_MIN_REGIONS_FOR_BUCKETS(fhc_MaxRemoteBuckets)
		);

        GASNETI_TRACE_PRINTF(C, 
    	    ("Firehose M=%ld (fh=%ld),maxregions=%d,prepinned=%ld (buckets=%d)",
    	    (unsigned long) M, (unsigned long) firehoses, fh_max_regions, 
	    (unsigned long) m_prepinned, (int) b_prepinned));
        GASNETI_TRACE_PRINTF(C, ("Firehose Maxvictim=%ld (fh=%d)",
    	    (unsigned long) maxvictim, fhc_MaxVictimBuckets));

        GASNETI_TRACE_PRINTF(C, 
    	    ("MaxLocalPinSize=%d\tMaxRemotePinSize=%d", 
    	    (int) fhinfo->max_LocalPinSize,
                    (int) fhinfo->max_RemotePinSize));
    }

    /*
     * Allocate temporary buffers (temp_buckets and temp_bucket_ptrs) for
     * use in managing remote pinned regions.
     *
     */
    fh_temp_buckets = (uintptr_t *)
    	gasneti_malloc(sizeof(uintptr_t) * fh_max_regions);

    fh_temp_bucket_ptrs = (fh_bucket_t **)
    	gasneti_malloc(sizeof(fh_bucket_t *) * fh_max_regions);

    #if FIREHOSE_SMP
    fh_da = (int *) gasneti_malloc(sizeof(int) * gasneti_nodes);
    memset(fh_da, 0, sizeof(int)*gasneti_nodes);
    #endif

    return;
}

#ifdef DEBUG_BUCKETS
static void
fh_priv_check_fn(void *val, void *arg)
{
    fh_bucket_t *bd = val;
    int live = (FH_NODE(bd) == gasneti_mynode)
			? (!FH_IS_LOCAL_FIFO(bd) && FH_BUCKET_REFC(bd)->refc_l)
			: (!FH_IS_REMOTE_FIFO(bd) && FH_BUCKET_REFC(bd)->refc_r);

    if_pf (live && !bd->prepinned) {
	/* XXX: promote to fatalerror? */
	fprintf(stderr, "WARNING: firehose leak detected on node %d - %d:%p\n",
		(int)gasneti_mynode, (int)FH_NODE(bd), (void*)FH_BADDR(bd));
    }
}
#endif

void
fh_fini_plugin(void)
{
	int			i;

#ifdef DEBUG_BUCKETS
	if (fh_verbose) {
	    FH_TABLE_LOCK;
	    fh_hash_apply(fh_BucketTable, &fh_priv_check_fn, NULL);
	    FH_TABLE_UNLOCK;
	}
#endif

        /* Deallocate the arrays of bucket buffers used, if applicable */
        for (i = 0; i < FH_BUCKETS_BUFS; i++) {
                if (fh_buckets_bufs[i] == NULL)
                        break;
                gasneti_free(fh_buckets_bufs[i]);
        }

	fh_hash_destroy(fh_BucketTable);

	return;
}

/* fh_region_partial(node, addr_p, len_p)
 *
 * Search for first range of pinned pages in the given range
 * and if succesful, overwrite the region with the pinned range.
 *
 * Returns non-zero if any pinned pages were found.
 */
int
fh_region_partial(gasnet_node_t node, uintptr_t *addr_p, size_t *len_p)
{
	uintptr_t	tmp_addr = 0;
	uintptr_t	addr, end_addr, bucket_addr;
	size_t		len;
	fh_bucket_t	*bd;
	int		is_local = (node == gasneti_mynode);

	addr     = *addr_p;
	len      = *len_p;
	end_addr = addr + len - 1;

	FH_TABLE_ASSERT_LOCKED;
        FH_FOREACH_BUCKET(addr, end_addr, bucket_addr) {
                bd = fh_bucket_lookup(node, bucket_addr);
		if ((bd != NULL) &&
		    (is_local || !FH_IS_REMOTE_PENDING(bd))) {
			/* found first pinned page */
			tmp_addr = bucket_addr;
			break;
		}
	}
	addr = tmp_addr;

	if_pf (tmp_addr == 0) {
		/* No pinned pages found in the requested region */
		return 0;
	}

	/* Search remainder of the interval, limiting the resulting length */
	len  = (end_addr - tmp_addr) + 1;	/* bytes remaining */
	if (is_local) {
		len = MIN(len, fhc_MaxVictimBuckets << FH_BUCKET_SHIFT);
	} else {
		len = MIN(len, fhc_MaxRemoteBuckets << FH_BUCKET_SHIFT);
	}
	end_addr = tmp_addr + (len - 1);

	tmp_addr += FH_BUCKET_SIZE;	/* first page known pinned */
	if_pt (tmp_addr != 0) { /* guards against wrap around */
       		FH_FOREACH_BUCKET(tmp_addr, end_addr, bucket_addr) {
               		bd = fh_bucket_lookup(node, bucket_addr);
			if ((bd == NULL) ||
			    (!is_local && FH_IS_REMOTE_PENDING(bd))) {
				/* found an unpinned page */
				len = bucket_addr - addr;
				break;
			}
		}
	}

	*addr_p = addr;
	*len_p  = len;
		
	return 1;
}

/* ##################################################################### */
/* PAGE-SPECIFIC INTERNAL FUNCTIONS                                      */
/* ##################################################################### */

/* fhi_AcquireLocalRegionsList
 *
 * This function is used as a utility function for 'acquiring' new buckets,
 * and is used both for client-initiated local pinning and local pinning
 * from AM handlers.  The function differentiates these two with the
 * 'local_ref' parameter.
 *
 * It's main purpose is to filter out the buckets that are already pinned
 * from the input list of regions.  This means incrementing the refcount
 * for buckets already pinned and returning only a region of buckets that
 * are currently _not_ pinned.  This function does not call the
 * client-supplied move_callback.
 *
 * The function returns the amount of buckets (not regions) contained in
 * the buildregion type (the amount of regions can be queried from the
 * type).
 *
 * The rpool is updated to reflect the number of buckets and regions that
 * were written.
 */

int
fhi_AcquireLocalRegionsList(int local_ref, firehose_region_t *region,
			    size_t reg_num, fhi_RegionPool_t *rpool)
{
	int			i, saw_pending = 0;
	uintptr_t		bucket_addr, end_addr;
	fh_bucket_t		*bd;

	rpool->regions_num = 0;
	rpool->buckets_num = 0;

	for (i = 0; i < reg_num; i++) {

	    end_addr = region[i].addr + region[i].len - 1;
				
	    FH_FOREACH_BUCKET(region[i].addr, end_addr, bucket_addr) 
	    {
		gasneti_assert(bucket_addr > 0);
		bd = fh_bucket_lookup(gasneti_mynode, bucket_addr);

		if (bd == NULL) {
		    /* Bucket is not pinned, add to the pool to be pinnned */
		    bd = fh_bucket_add(gasneti_mynode, bucket_addr);
		    FH_SET_LOCAL_PENDING(bd);
		    FH_BSTATE_SET(bd, fh_pending);
		    FH_TRACE_BUCKET(bd, PENDING);
		    fh_bucket_AddtoPool(rpool, bucket_addr);
		}
		else if (!FH_IS_LOCAL_PENDING(bd)) {
		    /* 
		     * The bucket is already pinned, increment refc
		     */
		    fh_priv_acquire_local(local_ref, bd);
		    FH_BSTATE_ASSERT(bd, fh_used);
		}
		else {
		    /* The bucket is pending.
		     * Only AM handler-initiated pins can see pending
		     * buckets */
		    gasneti_assert(!local_ref);
		    FH_BUCKET_REFC(bd)->refc_r += 1;
		    saw_pending = 1;
		}
	    }
	}

	if (saw_pending) {
	    gasneti_assert(!local_ref);
	
	    return -1;
	}
	else {
	    return rpool->buckets_num;
	}
}

/* 
 * fhi_ReleaseLocalRegionsList
 *
 * This function releases a list of regions and builds a new list of
 * regions to be unpinned by way of the client-supplied move_callback().
 *
 * By releasing buckets and adding them to the FIFO, it is possible that
 * the FIFO become overcommitted.  Overcommitting to the FIFO is permitted
 * as we are looping over the regions as long as a check is subsequently
 * made.
 *
 * We process each region in the reverse order in order to ease coalescing
 * when popping victims from the victim FIFO.
 *
 */
void
fhi_ReleaseLocalRegionsList(int local_ref, firehose_region_t *reg, 
				size_t reg_num)
{
	int			i;
	uintptr_t		bucket_addr, end_addr;
	fh_bucket_t		*bd;

	FH_TABLE_ASSERT_LOCKED;

	for (i = 0; i < reg_num; i++) {
		end_addr = reg[i].addr + reg[i].len - 1;

		GASNETI_TRACE_PRINTF(C, 
		    ("Firehose ReleaseLocalRegions ("GASNETI_LADDRFMT", %d)",
		    GASNETI_LADDRSTR(reg[i].addr), (int)reg[i].len));
				
 		FH_FOREACH_BUCKET_REV(reg[i].addr, end_addr, bucket_addr) 
		{
			bd = fh_bucket_lookup(gasneti_mynode, bucket_addr);
			gasneti_assert(bd != NULL);

			fh_priv_release_local(local_ref, bd);
		}
	}
	return;
}

/* ##################################################################### */
/* LOCAL PINNING                                                         */
/* ##################################################################### */
/* fhi_InitLocalRegionsList(local_ref, region, reg_num)
 *
 * This function adds all the buckets contained in the list of regions to the
 * hash table and initializes either the local or remote refcount to 1.
 *
 * It is used in fh_acquire_local_region() and fh_am_move_reqh_inner().
 *
 */
void
fhi_InitLocalRegionsList(int local_ref, firehose_region_t *region, 
					      int numreg)
{
	uintptr_t	end_addr, bucket_addr;
	fh_bucket_t	*bd;
	int		i;
	unsigned int	loc, rem;

	gasneti_assert((local_ref == 0) || (local_ref == 1));

	loc = local_ref;
	rem = !local_ref;

	FH_TABLE_ASSERT_LOCKED;

	/* Once pinned, We can walk over the regions to be pinned and
	 * set the reference count to 1. */
	for (i = 0; i < numreg; i++) {
		end_addr = region[i].addr + region[i].len - 1;

		gasneti_assert(region[i].addr > 0);

		FH_FOREACH_BUCKET(region[i].addr,end_addr,bucket_addr) {
			/* 
			 * Normally, the bucket will not already exist in the
			 * table.  However, in some threaded configurations 
			 * it is possible for another thread to add the 
			 * bucket (and pin the associated memory) while this
			 * current thread unlocked the table lock.
			 */
			#if GASNET_PAR || GASNETI_CONDUIT_THREADS
			bd = fh_bucket_lookup(gasneti_mynode, bucket_addr);
			if_pf (bd != NULL) {
				FH_BSTATE_ASSERT(bd, fh_pending);
				FH_SET_LOCAL_INUSE(bd);
				FH_BSTATE_SET(bd, fh_used);
				FH_BUCKET_REFC(bd)->refc_l += loc;
				FH_BUCKET_REFC(bd)->refc_r += rem;
				FH_TRACE_BUCKET(bd, INIT++);
			}
			else 
			#endif
			{
				bd = fh_bucket_add(gasneti_mynode, bucket_addr);
				FH_SET_LOCAL_INUSE(bd);
				FH_BSTATE_SET(bd, fh_used);
				FH_BUCKET_REFC(bd)->refc_l = loc;
				FH_BUCKET_REFC(bd)->refc_r = rem;
				FH_TRACE_BUCKET(bd, INIT);
			}
		}
	}

	return;
}

/*
 * This function is called by the Firehose reply once a firehose request to
 * pin functions covered into a region completes.
 *
 * The function walks over each bucket in the region and identifies the
 * buckets that were marked as 'pending'.  These 'pending buckets' may or
 * may not have requests associated to them.  In the former case, requests
 * pending a callback are identified and may be added to a list of
 * callbacks to be run (algorithm documented below).
 *
 * The function returns the amount of callbacks that were added to the list
 * of pending requests pointing to the 'reqpend' parameter.
 *
 */

/*
 * Phase 2. For each bucket containing a completion callback, either
 *          move the callback to the next unpinned bucket or actually
 *          run the callback handler if all buckets as part of that
 *          request are pinned.
 */
int
fh_PendingCallbacksProgress(gasnet_node_t node, fh_bucket_t **bd_list, 
			      int num_buckets, fh_pollq_t *PendQ)
{
    fh_bucket_t	*bd, *bdi;
    int		 i, callspend = 0;
    uintptr_t	 next_addr, end_addr, bucket_addr;

    fh_completion_callback_t	*ccb, *next_ccb;
    firehose_request_t		*req;

    FH_TABLE_ASSERT_LOCKED;	/* uses fh_temp_bucket_ptrs */

    for (i=0; i<num_buckets; i++) {
	bd = bd_list[i];
	next_addr = FH_BADDR(bd) + FH_BUCKET_SIZE;
	ccb = (fh_completion_callback_t *) bd->fh_tqe_next;
	gasneti_assert(ccb != NULL);
	
	FH_SET_REMOTE_INUSE(bd);
	FH_BSTATE_SET(bd, fh_used);
	FH_TRACE_BUCKET(bd, PENDING->INUSE had callback);

	while (ccb != FH_COMPLETION_END) {
	    int remain_pending = 0;
	    next_ccb = FH_STAILQ_NEXT(ccb);
	    gasneti_assert(ccb->flags & FH_CALLBACK_TYPE_COMPLETION);
	    req = ccb->request;
	    gasneti_assert(req && (req->flags & FH_FLAG_PENDING));
	    gasneti_assert(next_ccb != NULL);

	    GASNETI_TRACE_PRINTF(C,
		("Firehose Advance Callback bd=%p (%p,%d), req=%p, next=%p",
		bd, (void *) FH_BADDR(bd), (int) FH_NODE(bd), req, next_ccb));

	    /* Assume no other buckets are pending */
	    end_addr = req->addr + req->len - 1;

	    /* Walk through each bucket in the region until a pending bucket
	     * is found.  If none can be found, the callback can be called.
	     */
	    FH_FOREACH_BUCKET(next_addr, end_addr, bucket_addr) {
		bdi = fh_bucket_lookup(node, bucket_addr);
		gasneti_assert(bdi != NULL);
		gasneti_assert(!FH_IS_REMOTE_PENDING_UNCOMMITTED(bdi));

		if (FH_IS_REMOTE_PENDING(bdi)) {
		    if (bdi->fh_tqe_next == NULL)
			ccb->fh_tqe_next = FH_COMPLETION_END;
		    else
			ccb->fh_tqe_next = 
			    (fh_completion_callback_t *) bdi->fh_tqe_next;
		    bdi->fh_tqe_next = (fh_bucket_t *) ccb;

		    remain_pending++;
		    break;
		}
	    }

	    /* If the ccb is not pending any more, it has not been attached
	     * to any other bucket and its callback can be executed */
	    if (!remain_pending) {
		req->flags &= ~FH_FLAG_PENDING;
		FH_STAILQ_INSERT_TAIL(PendQ, (fh_callback_t *) ccb);
		GASNETI_TRACE_PRINTF(C,
		    ("Firehose Pending Request (%p,%d) "
		     "enqueued  %p for callback", (void *) req->addr, 
		     (int) req->len, req));
		callspend++;
	    }

	    ccb = next_ccb;
	} /* end while */

    } /* end for */

#if 0
    gasneti_assert(callspend > 0);
    gasneti_assert(!FH_STAILQ_EMPTY(PendQ));
#endif

    return callspend;
}

/*
 * Phase 1: fh_find_pending_callbacks
 *	    Identify which buckets have completion callbacks attached to them
 *	    and save them in fh_temp_bucket_ptrs.
 *
 *	    Called by fh_am_move_reph(), the handler called once the firehose
 *	    reply is recieved.
 */
int
fh_find_pending_callbacks(gasnet_node_t node, firehose_region_t *region,
			  int nreg, void *context, fh_pollq_t *PendQ)
{
    int		numpend = 0, callspend = 0;
    uintptr_t	end_addr, bucket_addr;
    fh_bucket_t	*bd;
    int		i;

    FH_TABLE_ASSERT_LOCKED;	/* uses fh_temp_bucket_ptrs */
    gasneti_assert(node != gasneti_mynode);

    FH_STAILQ_INIT(PendQ);

    for (i = 0; i < nreg; i++) {
	end_addr = region[i].addr + region[i].len - 1;
	gasneti_assert(region[i].addr > 0);

	FH_FOREACH_BUCKET(region[i].addr,end_addr,bucket_addr) {
	    bd = fh_bucket_lookup(node, bucket_addr);
	    gasneti_assert(bd != NULL);

	    /* Make sure the bucket was set as pending */
	    gasneti_assert(FH_IS_REMOTE_PENDING(bd));
	    FH_BSTATE_ASSERT(bd, fh_pending);

	    /* if there is a pending request on the bucket, save it
	     * in the temp array */
	    if (FH_HAS_COMPLETION_CALLBACK(bd)) {
		gasneti_assert(bd->fh_tqe_next != NULL);
		fh_temp_bucket_ptrs[numpend] = bd;
		numpend++;
		gasneti_assert(numpend < fh_max_regions);
	    }
	    else {
		FH_SET_REMOTE_INUSE(bd);
		FH_BSTATE_SET(bd, fh_used);
		FH_TRACE_BUCKET(bd, PENDING->INUSE);
	    }
	}
    }

    callspend = 
	fh_PendingCallbacksProgress(node, fh_temp_bucket_ptrs, numpend, PendQ);

    return callspend;
}

GASNETI_INLINE(fh_PendingCallbacksEnqueue)
void
fh_PendingCallbacksEnqueue(firehose_request_t *req,
			fh_bucket_t *bd, 
			fh_completion_callback_t *ccb,
			fh_completion_callback_t *ccb_next)
{
    fh_completion_callback_t *ccb_new;

    gasneti_assert(req->internal == NULL);
    gasneti_assert(!(req->flags & FH_FLAG_PENDING));

    if (ccb_next == NULL)
	ccb_next = FH_COMPLETION_END;

    ccb_new = fh_alloc_completion_callback();
    memcpy(ccb_new, ccb, sizeof(fh_completion_callback_t));

    /* Set prev's next and cur's next */
    bd->fh_tqe_next = (fh_bucket_t *) ccb_new;
    ccb_new->fh_tqe_next = ccb_next;

    req->flags |= FH_FLAG_PENDING;
    req->internal = (firehose_private_t *) ccb_new;

    GASNETI_TRACE_PRINTF(C, 
        ("Firehose CallbackEnqueue bd=%p (%p,%d), ccb=%p, req=%p", 
        bd, (void *) FH_BADDR(bd), (int) FH_NODE(bd), bd->fh_tqe_next, req));

    return;
}

/* ##################################################################### */
/* Firehose SMP ONLY related functions					 */
/* ##################################################################### */

#if FIREHOSE_SMP
extern void
fhsmp_ServiceLocalPendingList(void)
{
    int		i;
    uintptr_t	bucket_addr, end_addr;
    fh_bucket_t *bd;

    fh_locpendq_t   pendCallbacks;

    fh_remote_callback_t *rc, *rc2;

    FH_TABLE_ASSERT_LOCKED;

    FH_POLLQ_LOCK;
    FH_TAILQ_INIT(&pendCallbacks);

    rc = (fh_remote_callback_t *) FH_TAILQ_FIRST(&fhsmp_LocalPendingList);

    while (rc != NULL) {
	int has_pending = 0;

	/* Verify that none of the buckets are pending anymore */
	for (i = 0; i < rc->pin_list_num; i++) {
	    end_addr = rc->pin_list[i].addr + rc->pin_list[i].len - 1;

	    FH_FOREACH_BUCKET(rc->pin_list[i].addr,end_addr,bucket_addr) {
		bd = fh_bucket_lookup(gasneti_mynode,bucket_addr);
		gasneti_assert(bd != NULL);

		if (FH_IS_LOCAL_PENDING(bd)) {
		    has_pending++;
		    rc = (fh_remote_callback_t *) FH_TAILQ_NEXT(rc);
		    goto next_rc;
		}
	    }
	}

next_rc:
	/* No buckets are pending any longer, run the callback */
	if (!has_pending) {
	    rc2 = rc;
	    rc = (fh_remote_callback_t *) FH_TAILQ_NEXT(rc);
	    FH_TAILQ_REMOVE(&fhsmp_LocalPendingList, rc2);
	    FH_TAILQ_INSERT_TAIL(&pendCallbacks, rc2);
	}
    }

    FH_POLLQ_UNLOCK;

    /*
     * If we have local pending AMs to process, process them without holding
     * the table lock 
     */
    if (!FH_TAILQ_EMPTY(&pendCallbacks)) {
	FH_TABLE_UNLOCK;

	while (!FH_TAILQ_EMPTY(&pendCallbacks)) {
	    rc = FH_TAILQ_FIRST(&pendCallbacks);

	    if (rc->flags & FH_CALLBACK_TYPE_REMOTE) {
		firehose_remote_callback(rc->node, rc->pin_list, 
			    rc->pin_list_num, &(rc->args));
	    }
	    /* Send an AMRequest to the reply handler */
	    fh_send_firehose_reply(rc);

	    gasneti_free(rc->pin_list);
	    gasneti_free(rc);
	    FH_TAILQ_REMOVE(&pendCallbacks, rc);
	}

	FH_TABLE_LOCK;
    }

    return;
}

/*
 * ConsumeLocalBucket(num, unpin_p)
 *
 * Given 'num' new buckets to be pinned, an equal number in replacements
 * must be found.  The replacement can come from "Free Energy" (unused
 * buckets) or from FIFO victims.
 */
int
fhsmp_ConsumeLocalBucket(int b_num, fhi_RegionPool_t *unpin_p)
{
        int             b_remain, b_avail;

        FH_TABLE_ASSERT_LOCKED;
        gasneti_assert(FHC_MAXVICTIM_BUCKETS_AVAIL >= 0);

	/*
	 * 1. Try to steal "Free Energy", if available.
	 */
        b_avail = MIN(b_num, FHC_MAXVICTIM_BUCKETS_AVAIL);
        fhc_LocalOnlyBucketsPinned += b_avail;

        b_remain = b_num - b_avail;

	/* "Free Energy" may have fulfilled all our needs */
        if (b_remain == 0)
                return 0;

	/*
	 * 2. Try to reap some Victims
	 */
        b_avail = MIN(b_remain, fhc_LocalVictimFifoBuckets);

        if (b_avail > 0) {
                int                     r_freed;
                firehose_region_t       *reg;

                /* Append to unpin_p */
                reg = &(unpin_p->regions[unpin_p->regions_num]);

                /* Adjusts LocalVictimFifoBuckets count */
                r_freed = fhi_FreeVictimLocal(b_avail, reg);
                fhc_LocalVictimFifoBuckets -= b_avail;

                b_remain -= b_avail;
		unpin_p->buckets_num += b_avail;
                unpin_p->regions_num += r_freed;
        }

        gasneti_assert(FHC_MAXVICTIM_BUCKETS_AVAIL >= 0);

        return b_remain;
}

int
fhsmp_TryAcquireLocalRegion(firehose_request_t *req, fhi_RegionPool_t *pin_p,
			    fh_bucket_t **reused_bds, int *num_reused)
{
    uintptr_t	bucket_addr;
    int		i, reused = 0;
    uintptr_t	end_addr = req->addr + (uintptr_t)req->len - 1;
    fh_bucket_t	*bd;

    gasnet_node_t node = req->node;

    FH_TABLE_ASSERT_LOCKED;

    /*
     * Go through each bucket, and return -1 as soon as we see a pending
     * bucket.  The caller is advised that the contents of pin_p is
     * undefined if return is -1.
     */
    FH_FOREACH_BUCKET(req->addr, end_addr, bucket_addr) 
    {
	bd = fh_bucket_lookup(node, bucket_addr);

	if (bd != NULL) {
	    if (FH_IS_LOCAL_PENDING(bd)) /* abort */ 
		return -1;

	    reused_bds[reused] = bd;
	    reused++;
	    gasneti_assert(reused < fh_max_regions);
	}
	else {
	    bd = fh_bucket_add(node, bucket_addr);
	    FH_SET_LOCAL_PENDING(bd);
	    FH_BSTATE_SET(bd, fh_pending);
	    FH_BUCKET_REFC(bd)->refc_l = 1;
	    FH_BUCKET_REFC(bd)->refc_r = 0;
	    FH_TRACE_BUCKET(bd, PENDING);
	    fh_bucket_AddtoPool(pin_p, bucket_addr);
	}
    }

    /* 
     * Now that we know the try operation will not abort, we can actually
     * acquire the buckets that we can reuse (those already pinned).
     */

    for (i = 0; i < reused; i++) {
	bd = reused_bds[i];
	fh_priv_acquire_local(1, bd);
	FH_BSTATE_ASSERT(bd, fh_used);
    }

    *num_reused = reused;

    return pin_p->buckets_num;
}

fh_bucket_t *
fhsmp_ConsumeRemoteBucket(gasnet_node_t node, fhi_RegionPool_t *unpin_p)
{
    fh_fifoq_t	*fifo_head = &fh_RemoteNodeFifo[node];
    fh_refc_t	*rp;
    fh_bucket_t	*bd;

    FH_TABLE_ASSERT_LOCKED;

    /* First, attempt to use any available free energy */
    if (fhc_RemoteBucketsUsed[node] < fhc_RemoteBucketsM) {
	fhc_RemoteBucketsUsed[node]++;
	return (fh_bucket_t *) NULL;
    }

    /* When reclaiming a bucket from the FIFO, we increment its reference
     * count even though the intention is not to use that particular
     * bucket.  This approach avoids the assocaited page to be double
     * pinned, which could happen if the bucket were simply removed from
     * the firehose table.
     *
     * We check later on if the refcount was incremented by more than 1
     * (meaning another client wants to use up the page).
     */
    if (fhc_RemoteVictimFifoBuckets[node] <= 0) 
	fh_dump_counters();

    gasneti_assert(fhc_RemoteVictimFifoBuckets[node] > 0);

    bd = FH_TAILQ_FIRST(fifo_head);

    FH_TAILQ_REMOVE(fifo_head, bd);
    FH_BSTATE_ASSERT(bd, fh_remote_fifo);
    FH_BSTATE_SET(bd, fh_used);
    FH_SET_REMOTE_INUSE(bd);
    FH_TRACE_BUCKET(bd, REMFIFO in Consume);

    rp = FH_BUCKET_REFC(bd);
    rp->refc_l = 0;
    rp->refc_r = 1;

    fhc_RemoteVictimFifoBuckets[node]--;

    fh_bucket_AddtoPool(unpin_p, FH_BADDR(bd));

    return bd;
}

/*
 * This function estimates the number of buckets required to move in order
 * to satisfy the remote request at node 'node' for buckets covering
 * [start,end).  
 *
 * Returns:
 *   -1	    Deadlock detected during estimation
 *    0	    No additional buckets to pin
 *   >0	    Number of buckets to pin over [start,end)
 */
int
fhsmp_EstimateRemoteRequest(int *myda, int *dacount, int *n_pending, 
			    gasnet_node_t node, uintptr_t start, uintptr_t end)
{
    int		count, pending = 0;
    uintptr_t	bucket_addr;
    fh_bucket_t	*bd;

inner_again:
    count = 0;

    FH_FOREACH_BUCKET(start, end, bucket_addr)
    {
	bd = fh_bucket_lookup(node, bucket_addr);
	if (bd == NULL) {
	    /* We have a "pure" miss, increment the count */
	    count++;
	}
	else if (FH_IS_REMOTE_INUSE(bd)) {
	    /* do nothing */
	}
	else if (FH_IS_REMOTE_PENDING(bd)) {
	    pending++;
	}
	else if (FH_IS_REMOTE_PENDING_UNCOMMITTED(bd)) {
	    /* If we hit a non-committed pending bucket, increment the
	     * deadlock avoidance count and handle the case appropriately
	     * (whether we own or not the da bit).
	     */

	    ++(*dacount);
	    if (*myda) {
	        /* We own the da bit! */
	        gasneti_assert(fh_da[node] > 0);
	    }
	    else if (fh_da[node]) {
		/* Someone else owns the da bit, abandon! */
	        *n_pending = pending;
	        return -1;
	    }
	    else if (*dacount >= FH_DACOUNT_MAX) {
	        *myda = fh_da[node] = 1;
		fh_dacount++;
	        gasneti_sync_writes();
	    }

	    /* unlock, poll, yield, lock and try again */
	    FH_UPYL;
	    goto inner_again;
	}
	else {
	    FH_BSTATE_ASSERT(bd, fh_remote_fifo);

	    /* The bucket exists but only in the FIFO, which means we must use
	     * it at least as a replacement in our request */
	    count++;
	}
    }

    *n_pending = pending;

    return count;
}

/*
 * If n_pending == 0, we had a complete hit.
 */
void
fhsmp_PinRemoteAllHit(firehose_request_t *req, int n_pending, uintptr_t start, uintptr_t end)
{
    uintptr_t	 bucket_addr;
    fh_bucket_t *bd;

    gasnet_node_t node = req->node;

    FH_FOREACH_BUCKET(start, end, bucket_addr) {
	bd = fh_bucket_lookup(node, bucket_addr);

	gasneti_assert(bd && FH_BUCKET_REFC(bd)->refc_r > 0);
	fh_priv_acquire_remote(node, bd);
    }

    req->flags |= FH_FLAG_PINNED;
    return;
}

/*
 * In PinNoLog, we have already established that there are sufficient
 * resources (in Free Energy or in the FIFO) in order to satisfy the remote
 * pinning request -- we can proceed without polling.
 */
void
fhsmp_PinRemoteNoLog(firehose_request_t *req,
		    fh_completion_callback_t *ccb,
		    uintptr_t start, uintptr_t end,
		    fhi_RegionPool_t *pin_p,
		    fhi_RegionPool_t *unpin_p)
{
    uintptr_t	 bucket_addr;
    fh_bucket_t *bd;
    int		 first_pending = 1, new_b;

    gasnet_node_t node = req->node;

    pin_p->buckets_num = 0;
    pin_p->regions_num = 0;
    unpin_p->buckets_num = 0;
    unpin_p->regions_num = 0;

    FH_TABLE_ASSERT_LOCKED;

    FH_FOREACH_BUCKET(start, end, bucket_addr) {
	bd = fh_bucket_lookup(node, bucket_addr);

	if (bd == NULL) {
	    /* 
	     * Miss, add bucket to the table and the pool of regions
	     */
	    bd = fh_bucket_add(node, bucket_addr);
	    fh_bucket_AddtoPool(pin_p, bucket_addr);

	    FH_BUCKET_REFC(bd)->refc_r = 1;
	    FH_SET_REMOTE_PENDING(bd);
	    FH_BSTATE_SET(bd, fh_pending);
	    FH_TRACE_BUCKET(bd, INIT Pending Commit);

	    if (first_pending) {
		fh_PendingCallbacksEnqueue(req, bd, ccb, FH_COMPLETION_END);
		first_pending = 0;
	    }
	}
	else if (FH_IS_REMOTE_PENDING(bd)) {
	    /* hit on a PENDING bucket, attach ourselves to it if we havn't
	     * done so on any other pending bucket */
	    FH_BUCKET_REFC(bd)->refc_r++;
	    gasneti_assert(FH_BUCKET_REFC(bd)->refc_r > 0);
	    FH_TRACE_BUCKET(bd, PENDING added);

	    if (first_pending) {
		fh_PendingCallbacksEnqueue(req, bd, ccb,
		    (fh_completion_callback_t *) bd->fh_tqe_next);
		first_pending = 0;
	    }
	}
	else {
	    /* The bucket exists and is *NOT* pending, however make sure we
	     * can't see remote pending *non-committed* buckets */
	    gasneti_assert(!FH_IS_REMOTE_PENDING_UNCOMMITTED(bd));

	    /*
	    if (FH_IS_REMOTE_PENDING(bd))
		printf("OK, problem is here. . PinNoLog!\n");
		*/
	    fh_priv_acquire_remote(node, bd);
	}
    }

    new_b = pin_p->buckets_num;

    if (new_b > 0) {
	/* Use up what we can in "free energy" */
	int tmp = MIN(new_b, fhc_RemoteBucketsM - fhc_RemoteBucketsUsed[node]);
	new_b -= tmp;
	fhc_RemoteBucketsUsed[node] += tmp;

	/* Whatever's left will require replacement buckets, to be found in
	 * the remote FIFO.  We made sure to remove buckets we want to pin
	 * from the FIFO a few lines above */
	if (new_b > 0) {
	    gasneti_assert(new_b <= fhc_RemoteVictimFifoBuckets[node]);
	    unpin_p->buckets_num = new_b;
	    unpin_p->regions_num = 
		fhi_FreeVictimRemote(node, new_b, unpin_p->regions);
	    fhc_RemoteVictimFifoBuckets[node] -= new_b;
	}
    }

    return;
}

/*
 * RemotePinWithLog
 *
 * This function makes the best progress it can given that 'n_avail' resources
 * are available.  Presumably, this means recovering whatever is in the FIFO
 * (or in "free energy") in order to make progress over the region of interval
 * [start,end).
 *
 * This function is only called the first time an attempt to pin buckets is
 * made.  Subsequent free resources use RemotePinWithLogAgain().
 *
 */

uintptr_t
fhsmp_RemotePinWithLog(int n_avail, gasnet_node_t node, 
		 uintptr_t start, uintptr_t end, 
	         fhi_RegionPool_t *pin_p, fhi_RegionPool_t *unpin_p)
{
    uintptr_t	 bucket_addr, rem_addr;
    fh_bucket_t *bd;

    FH_TABLE_ASSERT_LOCKED;
    FH_FOREACH_BUCKET(start, end, bucket_addr) {
	bd = fh_bucket_lookup(node, bucket_addr);

	if (bd != NULL) {
	    /* EstimateRemoteRequest guarentees that we can't see
	     * remote+noncommitted buckets
	     */
	    gasneti_assert(!FH_IS_REMOTE_PENDING_UNCOMMITTED(bd));

	    /*
	     * If we remove it from the fifo, we actually decrease the number
	     * of available resources.
	     */
	    if (FH_IS_REMOTE_FIFO(bd))
		n_avail--;

	    fh_priv_acquire_remote(node, bd);
	}
	else if (n_avail > 0) {
	    try_this_again:
	    /* Try to claim a "free" bucket, or one currently in the FIFO */
	    bd = fhsmp_ConsumeRemoteBucket(node, unpin_p);

	    /* 
	     * Try to claim a "free" bucket or one in the FIFO. After that,
	     * make sure we aren't free-ing a bucket to be used subsequently.
	     */
	    if (bd != NULL && FH_BADDR(bd) >= start && FH_BADDR(bd) <= end) {
		rem_addr = fh_bucket_PopfromPool(unpin_p);
		gasneti_assert(rem_addr == FH_BADDR(bd));
		fh_priv_release_remote(node, bd); /* return to end of FIFO */
		FH_BSTATE_ASSERT(bd, fh_remote_fifo);
		--n_avail;
		if (!n_avail)
		    return bucket_addr;
		goto
		    try_this_again;
	    }

	    bd = fh_bucket_add(node, bucket_addr);

	    FH_BUCKET_REFC(bd)->refc_r = 1;
	    FH_SET_REMOTE_PENDING_UNCOMMITTED(bd);
	    FH_BSTATE_SET(bd, fh_pending_commit);
	    FH_TRACE_BUCKET(bd, INIT Pending Commit /w Log);

	    fh_bucket_AddtoPool(pin_p, bucket_addr);
	    --n_avail;
	}
	else {
	    /* No more resources to claim, return where we are */
	    return bucket_addr;
	}
    }
    
    /* We already know that we can't fulfill all the required resources, so
     * it's impossible to reach this point.
     */
    gasneti_fatalerror("Reached unreachable code");
    return (uintptr_t) 0;
}

/*
 * RemotePinWithLogAgain.
 */
uintptr_t
fhsmp_RemotePinWithLogAgain(int n_avail, gasnet_node_t node, 
		      uintptr_t start, uintptr_t end, 
	              fhi_RegionPool_t *pin_p, fhi_RegionPool_t *unpin_p)
{
    uintptr_t	 bucket_addr, rem_addr;
    fh_bucket_t *bd;

    FH_TABLE_ASSERT_LOCKED;
    FH_FOREACH_BUCKET(start, end, bucket_addr) {
	bd = fh_bucket_lookup(node, bucket_addr);

	if (bd != NULL) {
	    /* 
	     * If we see a pending, uncomitted bucket, stop making progress and
	     * retry later.
	     */

	    if (FH_IS_REMOTE_PENDING_UNCOMMITTED(bd))
		return bucket_addr;

	    if (FH_IS_REMOTE_FIFO(bd))
		n_avail--;

	    fh_priv_acquire_remote(node, bd);
	}
	else if (n_avail > 0) {
	    try_this_again:
	    /* Try to claim a "free" bucket, or one currently in the FIFO */
	    bd = fhsmp_ConsumeRemoteBucket(node, unpin_p);

	    /* Try to claim a "free" bucket or one in the FIFO. After that,
	     * make sure we aren't free-ing a bucket to be used subsequently.
	     */
	    if (bd != NULL && FH_BADDR(bd) >= start && FH_BADDR(bd) <= end) {
		rem_addr = fh_bucket_PopfromPool(unpin_p);
		gasneti_assert(rem_addr == FH_BADDR(bd));
		fh_priv_release_remote(node, bd); /* return to end of FIFO */
		FH_BSTATE_ASSERT(bd, fh_remote_fifo);
		--n_avail;
		if (!n_avail) {
		    return bucket_addr;
		}
		goto
		    try_this_again;
	    }

	    bd = fh_bucket_add(node, bucket_addr);

	    FH_BUCKET_REFC(bd)->refc_r = 1;
	    FH_SET_REMOTE_PENDING_UNCOMMITTED(bd);
	    FH_BSTATE_SET(bd, fh_pending_commit);
	    FH_TRACE_BUCKET(bd, INIT Pending Commit /w LogAgain);

	    fh_bucket_AddtoPool(pin_p, bucket_addr);
	    --n_avail;
	}
	else {
	    /* As far as we could go with input 'n_avail' resources */
	    return bucket_addr;
	}
    }

    /* At this point, we have enough resources to finish */
    return end + 1;
}

/*
 * Return zero if another thread hit one of the buckets contained 
 * in our unpin pool.
 */
int
fhsmp_RevalidateResources(gasnet_node_t node, fhi_RegionPool_t *unpin_p)
{
    int		i;
    uintptr_t	bucket_addr, bucket_end;
    fh_bucket_t *bd;

    FH_FOREACH_BUCKET_IN_POOL(i, unpin_p, bucket_addr, bucket_end) {

	bd = fh_bucket_lookup(node, bucket_addr);

	gasneti_assert(bd != NULL);
	gasneti_assert(FH_BUCKET_REFC(bd)->refc_r > 0);

	/* Someone hit on a bucket we used in our unpin pool */
	if (FH_BUCKET_REFC(bd)->refc_r != 1) {
	    return 0;
	}
    }

    return 1;
}

/*
 * In Local Rollback, we rollback the resources in this order:
 *   1. Return all the buckets we planned to reuse
 *   2. Remove all new PENDING buckets added to the table (** a possible
 *      optimization here is to leave the buckets in the table and mark them as
 *      garbage, skipping the remove+add step required subsequently).
 *   3. Return all "Free Energy".  Free energy is the total number of buckets
 *      to pin, minus the ones we have in unpin, minus the ones remaining.
 *   4. Return all buckets contained in unpin_p to the FIFO.
 */

void
fhsmp_LocalRollback(fhi_RegionPool_t *pin_p, fhi_RegionPool_t *unpin_p,
		    fh_bucket_t **reused_bds, int b_reused, int b_remain)
{
    int		b_free;
    int		b_unpin, i;
    uintptr_t	bucket_end, bucket_addr;
    fh_bucket_t *bd;

    /*
     * 1. Decrement all the buckets we could reuse
     */
    for (i=0; i < b_reused; i++)
	fh_priv_release_local(1, reused_bds[i]);
    
    /*
     * 2. Destroy all buckets we marked as "PENDING"
     */
    FH_FOREACH_BUCKET_IN_POOL(i, pin_p, bucket_addr, bucket_end) {

	bd = fh_bucket_lookup(gasneti_mynode, bucket_addr);

	gasneti_assert(bd != NULL);
	gasneti_assert(FH_IS_LOCAL_PENDING(bd));
	gasneti_assert(FH_BUCKET_REFC(bd)->refc_l == 1);

	fh_bucket_remove(bd);
    }

    /*
     * 3. Return "Free Energy", defined as
     *    Buckets requested - (Buckets reaped + Buckets in Free Energy)
     *
     *    These buckets are subtracted from LocalOnlyBucketsPinned as we
     *    will *NOT* be pinning buckets in pin_p and these buckets should
     *    not count against our limit
     */
    b_unpin = unpin_p == NULL ? 0 : unpin_p->buckets_num;
    b_free  = pin_p->buckets_num - (b_unpin + b_remain);
    gasneti_assert(b_free >= 0);
    fhc_LocalOnlyBucketsPinned -= b_free;


    /*
     * 4. Return all buckets reaped from the FIFO, if any.
     *    We actually have to recreate the buckets since we removed them in
     *    order to prevent other threads to hit on FIFO buckets that were to be
     *    removed.
     */

    if (b_unpin > 0) {
	FH_FOREACH_BUCKET_IN_POOL(i, unpin_p, bucket_addr, bucket_end) {

	    gasneti_assert(fh_bucket_lookup(gasneti_mynode, bucket_addr) == NULL);
	    bd = fh_bucket_add(gasneti_mynode, bucket_addr);
	    FH_SET_LOCAL_INUSE(bd);
	    FH_BUCKET_REFC(bd)->refc_l = 1;
	    FH_BUCKET_REFC(bd)->refc_r = 0;
	    fh_priv_release_local(1, bd);
	    FH_BSTATE_ASSERT(bd, fh_local_fifo);
	}
    }

    /*
     * We should *NOT* have overflowed the LocalOnlyBucketsPinned counter
     */
    gasneti_assert(fhc_LocalOnlyBucketsPinned <= fhc_MaxVictimBuckets);

    return;
}

void
fhsmp_RemoteRollback(gasnet_node_t node, 
	       uintptr_t start_addr, uintptr_t saved_addr,
	       fhi_RegionPool_t *pin_p, fhi_RegionPool_t *unpin_p)
{
    uintptr_t	 bucket_addr, bucket_end;
    uintptr_t	 end_addr = saved_addr - 1;
    fh_bucket_t *bd;
    int		 i;

    /* May not have acquired any buckets if saved addr is start addr */
    if (saved_addr == start_addr) {
	gasneti_assert(!pin_p || pin_p->buckets_num == 0);
	gasneti_assert(!unpin_p || unpin_p->buckets_num == 0);
	return;
    }

    /*
     * 1. Separate the buckets we used into the ones we created (set as
     *    PENDING_UNCOMMITTED) and others we used (we can simply decrement).
     */

    FH_FOREACH_BUCKET(start_addr, end_addr, bucket_addr) {
	bd = fh_bucket_lookup(node, bucket_addr);
	if (bd == NULL)
	    gasneti_fatalerror("start=%p,save=%p and no bucket found at %p\n",
			    (void*)start_addr,(void *)saved_addr,(void*)bucket_addr);
	gasneti_assert(bd != NULL);

	if (FH_IS_REMOTE_PENDING_UNCOMMITTED(bd)) {
	    /*
	     * We created this bucket (and set it pending and not committed),
	     * it falls within the "handled" range
	     */
	    FH_TRACE_BUCKET(bd, Rollback destroy);
	    fh_bucket_remove(bd);

	}
	else if (FH_IS_REMOTE_PENDING(bd)) {
	    /* Some other request had this bucket pending */
	    gasneti_assert(FH_BUCKET_REFC(bd)->refc_r > 1);
	    FH_BUCKET_REFC(bd)->refc_r--;
	}
	else {
	    fh_priv_release_remote(node, bd);
	    FH_TRACE_BUCKET(bd, Rollback Release);
	}

    }

    /*
     * 2. Return to the FIFO all buckets we were able to reap.  Buckets in our
     *    unpin pool may also have a refcount > 1, in which case we had to
     *    rollback because another thread wanted to use a bucket in our unpin
     *    list.
     */
    FH_FOREACH_BUCKET_IN_POOL(i, unpin_p, bucket_addr, bucket_end) {

	bd = fh_bucket_lookup(node, bucket_addr);
	gasneti_assert(bd != NULL);
	fh_priv_release_remote(node, bd); /* return to FIFO, if possible */
    }


    /*
    * Restore the buckets taken as "free energy"
     */
    GASNETI_TRACE_PRINTF(C, ("Rollback free energy = %d", 
	(unsigned int) (pin_p->buckets_num - unpin_p->buckets_num)));
    fhc_RemoteBucketsUsed[node] +=
        (pin_p->buckets_num - unpin_p->buckets_num);

    return;
}

/*
 * Once we've acquired the required resources, the calling thread can commit.
 * We 
 */
void
fhsmp_Commit(firehose_request_t *req,
	     fh_completion_callback_t *ccb,
	     uintptr_t start_addr, uintptr_t end_addr,
	     fhi_RegionPool_t *pin_p, fhi_RegionPool_t *unpin_p)
{
    /*
     * Commit all buckets we created, setting them pending and now committed.
     * Also, we hang a callback on the first pending bucket
     */
    uintptr_t	 bucket_addr, bucket_end;
    fh_bucket_t *bd;
    int		 i, first_pending = 1;

    gasnet_node_t node = req->node;

    GASNETI_TRACE_PRINTF(C, ("Firehose SMP Commit (req=%p)", req));

    FH_FOREACH_BUCKET_IN_POOL(i, pin_p, bucket_addr, bucket_end) {
	bd = fh_bucket_lookup(node, bucket_addr);
	gasneti_assert(bd != NULL);
	gasneti_assert(FH_BUCKET_REFC(bd)->refc_r == 1);
	gasneti_assert(FH_IS_REMOTE_PENDING_UNCOMMITTED(bd));

	FH_SET_REMOTE_PENDING(bd);
	FH_BSTATE_SET(bd, fh_pending);
	FH_TRACE_BUCKET(bd, Pending->Committed);

	if (first_pending) {
	    fh_PendingCallbacksEnqueue(req, bd, ccb, FH_COMPLETION_END);
	    first_pending = 0;
	}
    }

    FH_FOREACH_BUCKET_IN_POOL(i, unpin_p, bucket_addr, bucket_end) {
	bd = fh_bucket_lookup(node, bucket_addr);
	gasneti_assert(bd != NULL);
	gasneti_assert(FH_BUCKET_REFC(bd)->refc_r == 1);

	fh_bucket_remove(bd);
    }

    /*
     * If we havn't attached any callback yet, either we hit entirely on
     * pending buckets or it so happens that all the buckets we were waiting
     * for are now pinned.
     *
     * XXX There is some room for optimization here, by handling this case
     * earlier.
     */

    if (!(req->flags & FH_FLAG_PENDING)) {
	uintptr_t end_addr = req->addr + req->len - 1;

	FH_FOREACH_BUCKET(req->addr, end_addr, bucket_addr) {
	    bd = fh_bucket_lookup(node, bucket_addr);
	    gasneti_assert(FH_IS_REMOTE_INUSE(bd) || FH_IS_REMOTE_PENDING(bd));

	    if (FH_IS_REMOTE_PENDING(bd)) {
		fh_PendingCallbacksEnqueue(req, bd, ccb, 
		    (fh_completion_callback_t *) bd->fh_tqe_next);
		break;
	    }
	}
    }

    return;
}

/* 
 * fh_acquire_local_region(region)
 *
 * In acquiring local pages covered over the region, pin calls are coalesced.
 * Acquiring a page may lead to a pin call but always results in the page
 * reference count being incremented.
 *
 * Under firehose-page, acquiring means finding bucket descriptors for each
 * bucket in the region and incrementing the bucket descriptor's reference
 * count.
 *
 * Called by fh_local_pin() (firehose_local_pin, firehose_local_try_pin)
 */

void
fh_acquire_local_region(firehose_request_t *req)
{
    int			b_total, b_new, b_reused;
    int			outer_limit, inner_limit;
    int			my_da = 0;
    int			outer_count = 0;
    fhi_RegionPool_t	*pin_p = NULL, *unpin_p = NULL;
    fh_bucket_t		*bd;

    static gasneti_cond_t inflight_condvar = GASNETI_COND_INITIALIZER;

    FH_TABLE_ASSERT_LOCKED;

    gasneti_assert(req->node == gasneti_mynode);

    b_total = FH_NUM_BUCKETS(req->addr, req->len);

    /* Make sure the size of the region respects the local limits. */
    /* XXX Should split up larger sizes into smaller requests */
    gasneti_assert(b_total <= fhc_MaxVictimBuckets);

    #if 0
    outer_limit = (gasnete_approx_num_threads() - 1);
    #endif
    outer_limit = 2;
    inner_limit = (b_total / 10);   /* ??? */

again:
    gasneti_assert(pin_p == NULL && unpin_p == NULL);
    pin_p        = fhi_AllocRegionPool(FH_MIN_REGIONS_FOR_BUCKETS(b_total));
    unpin_p = NULL;

    FH_TABLE_ASSERT_LOCKED;
    outer_count++;

    if_pf (my_da) {
	/* If we assert my_da, make sure local da is set */
	gasneti_assert(fh_local_da);
    }
    if_pf (fh_local_da) {
	/* Someone else owns fhi_local_da, wait for them to finish */
	do {
	    gasneti_cond_wait(&fh_local_da_cv, &fh_table_lock);
	} while (fh_local_da);	/* loop until we win the race */
    }
    else if_pf (outer_count > outer_limit) {
        /* take ownership of fhi_local_da */
        my_da = fh_local_da = 1;
        /* give others a chance to release resources */
        FH_TABLE_UNLOCK;
        FIREHOSE_AMPOLL();
        gasneti_sched_yield();
        FIREHOSE_AMPOLL();
        FH_TABLE_LOCK;
    }

    /* Try to acquire local buckets */
    b_new = 
	fhsmp_TryAcquireLocalRegion(req, pin_p, fh_temp_bucket_ptrs, &b_reused);

    /* We saw no pendings and have zero or more to pin */
    if_pt (b_new >= 0) {
	int inner_count = 0;
	int b_remain;
	int i;
	uintptr_t bucket_end, bucket_addr;

	unpin_p = fhi_AllocRegionPool(b_new);

        b_remain = fhsmp_ConsumeLocalBucket(b_new, unpin_p);

	/* 
	 * If b_remain is greater than zero, we have to wait for requests
	 * currently in flight to fulfill our replacement needs.  This
	 * condition is satisfied when b_remain reaches 0.
	 */
        if_pf (b_remain > 0) {
            do {
		/* Unlock, poll, yield, poll, lock */
		FH_UPYUL;
		FH_TABLE_ASSERT_LOCKED;

                inner_count++;
                if_pf (my_da) {
                    /* We already "own" fhi_local_da, no checks needed */
                    gasneti_assert(fh_local_da);
                }
                else if_pf (fh_local_da) {
                    /* Somebody else "owns" fhi_local_da.
                       We must back off and start from scratch. */
		    fhsmp_LocalRollback(pin_p, unpin_p, fh_temp_bucket_ptrs, 
				        b_reused, b_remain);
                    fhi_FreeRegionPool(pin_p);
                    fhi_FreeRegionPool(unpin_p);
                    pin_p = NULL;
                    unpin_p = NULL;
                    goto again; /* will recheck for (fhi_local_da != 0) */
                }
                else if_pf (inner_count > inner_limit) {
                    /* take ownership of fhi_local_da */
                    my_da = fh_local_da = 1;
                }
            } while 
	      ((b_remain = fhsmp_ConsumeLocalBucket(b_remain, unpin_p)));
        }

        /* 
	 * We have everything we need... commit
	 */
	if (pin_p->regions_num) {
	    GASNETI_TRACE_EVENT(C,FH_LOCAL_MISS);
	}
	else {
	    GASNETI_TRACE_EVENT(C,FH_LOCAL_HIT);
	}

	FH_TABLE_ASSERT_LOCKED;
        FH_TABLE_UNLOCK;
        gasneti_assert(pin_p != NULL);
        gasneti_assert(unpin_p != NULL);
        firehose_move_callback(gasneti_mynode,
                            unpin_p->regions, unpin_p->regions_num,
                            pin_p->regions, pin_p->regions_num);
        FH_TABLE_LOCK;
	FH_TABLE_ASSERT_LOCKED;
        fhi_FreeRegionPool(unpin_p);
	unpin_p = NULL;

	/* Must remove the PENDING bit on buckets we just pinned and actually
	 * initialize the buckets.  */
	FH_FOREACH_BUCKET_IN_POOL(i, pin_p, bucket_addr, bucket_end) {
	    bd = fh_bucket_lookup(gasneti_mynode, bucket_addr);

	    gasneti_assert(bd != NULL);
	    gasneti_assert(FH_IS_LOCAL_PENDING(bd));

	    FH_BSTATE_SET(bd, fh_used);
	    FH_SET_LOCAL_INUSE(bd);
	    FH_TRACE_BUCKET(bd, INIT NONPENDING);
	}

        fhi_FreeRegionPool(pin_p);
	pin_p = NULL;

	gasneti_cond_broadcast(&inflight_condvar);

        if_pf (my_da) {
            gasneti_assert(fh_local_da);
            FH_TABLE_UNLOCK;
            FIREHOSE_AMPOLL();
            FH_TABLE_LOCK;
            fh_local_da = 0;
            gasneti_cond_broadcast(&fh_local_da_cv);
        }

	fhsmp_ServiceLocalPendingList();
    }
    else {
        /* We saw one of more PENDING buckets.  We unwind, wait
         * for them to be completed and then start over at the
         * begining (for a maximum of outer_limit times before
         * asserting fh_local_da.
         */
	fhsmp_LocalRollback(pin_p, NULL, NULL, 0, 0);
        fhi_FreeRegionPool(pin_p);
	pin_p = NULL;
        gasneti_cond_wait(&inflight_condvar, &fh_table_lock);
        goto again; /* will recheck for (fh_local_da != 0) */
    }


    FH_TABLE_ASSERT_LOCKED;

    return;
}

void
fh_acquire_remote_region(firehose_request_t *req,
			 firehose_completed_fn_t callback, void *context,
			 uint32_t flags, 
			 firehose_remotecallback_args_fn_t args_fn)
{
    int	da_count = 0, b_total;
    int	my_da = 0;
    int n_buckets, n_avail, n_pending, n_avail_old;

    gasnet_node_t   node = req->node;
    uintptr_t	    end_addr = req->addr + req->len - 1;
    uintptr_t	    start_addr = req->addr;
    uintptr_t	    saved_addr, bucket_addr;

    fhi_RegionPool_t	     *pin_p = NULL, *unpin_p = NULL;
    fh_completion_callback_t  ccb;

    FH_TABLE_ASSERT_LOCKED;

    /* Make sure the size of the region respects the remote limits */
    gasneti_assert(req->node != gasneti_mynode);
    b_total = FH_NUM_BUCKETS(req->addr, req->len);

    FH_TABLE_ASSERT_LOCKED;

    req->internal = NULL;

    /* Fill in a completion callback */
    ccb.flags = FH_CALLBACK_TYPE_COMPLETION;
    ccb.fh_tqe_next = FH_COMPLETION_END;
    ccb.callback = callback;
    ccb.request  = req;
    ccb.context  = context;

    GASNETI_TRACE_PRINTF(C, ("in fh_acquire_remote_region"));

outer_again:
    n_pending = 0;

    /* Wait for deadlock to end */
    while (fh_da[node]) { FH_UPYL; }
    n_avail = FHI_AVAIL(node);

    n_buckets = 
	fhsmp_EstimateRemoteRequest(&my_da, &da_count, &n_pending, node, 
	    start_addr, end_addr);

    /*
     * No buckets to pin, the hit can be either perfect or exclusively on
     * pending buckets
     */
    if (n_buckets == 0) {
	fh_bucket_t *bd;
	/* 
	 * Perfect hit: no pending buckets
	 */
	if (n_pending == 0) {
	    FH_FOREACH_BUCKET(start_addr, end_addr, bucket_addr) {
		bd = fh_bucket_lookup(node, bucket_addr);
		gasneti_assert(bd && FH_BUCKET_REFC(bd)->refc_r > 0);
		fh_priv_acquire_remote(node, bd);
	    }
	    req->flags |= FH_FLAG_PINNED;
	}
	/*
	 * Hit entirely on buckets currently in flight, don't bother sending
	 * AM. just attach a callback on the first bucket found to be pending.
	 */
	else {
	    int has_pending = 0;
	    FH_FOREACH_BUCKET(start_addr, end_addr, bucket_addr) {
		bd = fh_bucket_lookup(node, bucket_addr);
		gasneti_assert(FH_IS_REMOTE_INUSE(bd) || FH_IS_REMOTE_PENDING(bd));

		fh_priv_acquire_remote(node, bd);

		if (!has_pending && FH_IS_REMOTE_PENDING(bd)) {
		    fh_PendingCallbacksEnqueue(req, bd, &ccb, 
			(fh_completion_callback_t *) bd->fh_tqe_next);
		    has_pending++;
		}
	    }
	    gasneti_assert(req->flags & FH_FLAG_PENDING);
	}

	goto done;
    }

    if (n_buckets < 0 && !my_da)
	goto outer_again;

    /*
     * At this point, we will remain the sole owners of the table lock until we
     * release it in order to poll.
     */
    n_avail = FHI_AVAIL(node);

    pin_p   = fhi_AllocRegionPool(FH_MIN_REGIONS_FOR_BUCKETS(b_total));
    unpin_p = fhi_AllocRegionPool(FH_MIN_REGIONS_FOR_BUCKETS(n_buckets));

    if (my_da) {
	gasneti_assert(fh_da[node]);
	goto won_da;
    }

    /* 
     * 1. Easy Path: We have enough resources already 
     */
    if (n_buckets <= n_avail) {
	fhsmp_PinRemoteNoLog(req, &ccb, start_addr, end_addr, pin_p, unpin_p);
	gasneti_assert(!(req->flags & FH_FLAG_PINNED));
	goto send_am; /* skip hard path */
    }

    /*
     * 2. Hard Path: Not enough resources, we may have to poll.
     */
    saved_addr = 
	fhsmp_RemotePinWithLog(n_avail,node,start_addr,end_addr,pin_p,unpin_p);
    gasneti_assert(saved_addr < end_addr);

    while (da_count < FH_DACOUNT_MAX) {
        da_count++;

	/* Someone else asserted fh_da: start over */
	if_pf (fh_da[node]) {
	    gasneti_assert(!my_da);
	    fhsmp_RemoteRollback(node, start_addr, saved_addr, pin_p, unpin_p);
            fhi_FreeRegionPool(pin_p);
            fhi_FreeRegionPool(unpin_p);
            pin_p = NULL;
            unpin_p = NULL;
	    goto outer_again;
	}

	/*fh_dump_counters();*/

	/* Poll, hoping that more resources are available */
	FH_UPYL;
	n_avail = FHI_AVAIL(node);

	/* We may have lost some buckets, stolen by another thread */
	if (!fhsmp_RevalidateResources(node, unpin_p)) {
	    /* XXX: room for optimization here where we could try to
	     * replace lost buckets with some from n_avail */
	    GASNETI_TRACE_PRINTF(C, 
		("Firehose ROLLBACK: Lost some unpin buckets"));
	    fhsmp_RemoteRollback(node, start_addr, saved_addr, pin_p, unpin_p);
            fhi_FreeRegionPool(pin_p);
            fhi_FreeRegionPool(unpin_p);
            pin_p = NULL;
            unpin_p = NULL;
	    goto outer_again;
	}

	saved_addr = 
	    fhsmp_RemotePinWithLogAgain(n_avail,node,saved_addr,
			                end_addr,pin_p,unpin_p);

	/* In testing for 'end+1', we assume the last page in memory will
	 * never appear in the code path */
	if (saved_addr == (end_addr+1)) {
	    fhsmp_Commit(req, &ccb, start_addr, end_addr, pin_p, unpin_p);
	    goto send_am;
	}
    }

    fhsmp_RemoteRollback(node, start_addr, saved_addr, pin_p, unpin_p);

    /* We can *WIN* the deadlock avoidance bit race */
    if (!fh_da[node]) {
	n_avail_old = FHI_AVAIL(node);
	my_da = fh_da[node] = 1;
	fh_dacount++;

    won_da:
	/* Wait for sufficient resources */
	do {
	    FH_UPYL;
	    n_avail = FHI_AVAIL(node);
	} 
	while (fhsmp_EstimateRemoteRequest(&my_da, &da_count, &n_pending, node, 
	                  start_addr, end_addr) > FHI_AVAIL(node));
	/* Once we are out of the polling loop, other threads have
	 * relinquished their resources and we have satisfied our
	 * requirements */
	fhsmp_PinRemoteNoLog(req, &ccb, start_addr, end_addr,
				 pin_p, unpin_p);
	fh_da[node] = 0;
	if (req->flags & FH_FLAG_PINNED) 
	    goto done;	/* Turns out the operation is now free */
	else
	    goto send_am;
    }

    /* We *LOST* the deadlock avoidance race bit -- Rollback */
    else {
        fhi_FreeRegionPool(pin_p);
        fhi_FreeRegionPool(unpin_p);
        pin_p = NULL;
        unpin_p = NULL;
	goto outer_again;
    }

send_am:
    if (pin_p && pin_p->regions_num > 0) {
	size_t	args_len = 0;
	size_t	pin_r    = pin_p->regions_num;
	size_t	unpin_r  = unpin_p->regions_num;
	size_t	tot_r	 = pin_r + unpin_r;

	firehose_region_t   *reg_alloc;

	if (flags & FIREHOSE_FLAG_ENABLE_REMOTE_CALLBACK)
		args_len = sizeof(firehose_remotecallback_args_t);

	reg_alloc = (void *)
	    alloca(sizeof(firehose_region_t) * (pin_r+unpin_r+args_len));

	/* Copy new and replacement regions */
	memcpy(reg_alloc, pin_p->regions, pin_r*sizeof(firehose_region_t));
	memcpy(reg_alloc + pin_r, unpin_p->regions,
		    unpin_r * sizeof(firehose_region_t));

        fhi_FreeRegionPool(pin_p);
        fhi_FreeRegionPool(unpin_p);
	FH_TABLE_UNLOCK;

	/* Construct remote callback args if required */
	if (args_len > 0) {
	    args_len = args_fn(context,
			       (firehose_remotecallback_args_t *)(reg_alloc + tot_r));
	}

	#ifdef FIREHOSE_UNBIND_CALLBACK
	if (unpin_r > 0)
	    firehose_unbind_callback(node, reg_alloc + pin_r, unpin_r);
	#endif

	req->flags |= FH_FLAG_INFLIGHT;

	MEDIUM_REQ(4,5,
		    (node,
		    fh_handleridx(fh_am_move_reqh),
		    reg_alloc, 
		    sizeof(firehose_region_t)*tot_r+args_len,
		    (uint32_t) flags,
		    (uint32_t) pin_r,
		    (uint32_t) unpin_r,
		    PACK(NULL)));

	goto done_unlocked;
    }
    else {
        fhi_FreeRegionPool(pin_p);
        fhi_FreeRegionPool(unpin_p);
    }

done:
    /* Only set the PINNED flag if the request is not set on any
     * pending buckets */
    if (!(req->flags & FH_FLAG_PENDING))
	req->flags |= FH_FLAG_PINNED;
    FH_TABLE_UNLOCK;
done_unlocked:

    FH_TABLE_ASSERT_UNLOCKED;
    return;
}

#else /* !FIREHOSE_SMP */

/* ##################################################################### */
/* Firehose UNI ONLY related functions					 */
/* ##################################################################### */

/* 
 * fhuni_CoalesceBuckets(buckets_ptr_vec, num_buckets, regions_array)
 *
 * Helper function to coalesce contiguous buckets into the regions array.
 *
 * The function loops over the bucket descriptors in the 'buckets' array in
 * the hopes of creating the smallest amount of region_t in the 'regions'
 * array.
 * This is made possible by coalescing buckets found to be contiguous in
 * memory by looking at the previous bucekt descriptors in the 'buckets'
 * array.
 *
 * It is probably not worth our time making the coalescing process smarter by
 * searching through the whole 'buckets' array each time.
 *
 * The function returns the amount of regions in the region array.
 *
 */
int
fhuni_CoalesceBuckets(uintptr_t *bucket_list, size_t num_buckets,
		firehose_region_t *regions)
{
	int		i, j = -1; /* new buckets created */
	uintptr_t	addr_next = 0, bucket_addr;

	FH_TABLE_ASSERT_LOCKED;

	gasneti_assert(num_buckets > 0);
	/* Coalesce consequentive pages into a single region_t */
	for (i = 0; i < num_buckets; i++) {
		bucket_addr = bucket_list[i];
		if (i > 0 && bucket_addr == addr_next)
			regions[j].len += FH_BUCKET_SIZE;
		else {
			j++;
			regions[j].addr = bucket_addr;
			regions[j].len  = FH_BUCKET_SIZE;
		}

		addr_next = bucket_addr + FH_BUCKET_SIZE;
	}

	gasneti_assert(regions[j].addr > 0);
	return j+1;
}
	
int
fhuni_TryAcquireRemoteRegion(firehose_request_t *req, 
			fh_completion_callback_t *ccb,
			int *new_regions)
{
 	uintptr_t	bucket_addr, end_addr, next_addr = 0;
	int		unpinned = 0;
	int		new_r = 0, b_num;
	int		has_callback;
	fh_bucket_t	*bd;
	gasnet_node_t	node;

	end_addr = req->addr + (uintptr_t) req->len - 1;

	FH_TABLE_ASSERT_LOCKED;

	gasneti_assert(req != NULL);
	gasneti_assert(req->node != gasneti_mynode);
	node = req->node;
	has_callback = (req->flags & FH_FLAG_PENDING);

	b_num = FH_NUM_BUCKETS(req->addr, req->len);

 	FH_FOREACH_BUCKET(req->addr, end_addr, bucket_addr) {

	    bd = fh_bucket_lookup(node, bucket_addr);

	    if (bd == NULL) {
	        /* We add the bucket but set it PENDING */
	        fh_temp_buckets[unpinned] = bucket_addr;

	        bd = fh_bucket_add(node, bucket_addr);
		FH_BUCKET_REFC(bd)->refc_r++;
	        FH_SET_REMOTE_PENDING(bd);
	        FH_BSTATE_SET(bd, fh_pending);
	        FH_TRACE_BUCKET(bd, INIT);

	        if (next_addr != bucket_addr)
		   new_r++;

	        next_addr = bucket_addr + FH_BUCKET_SIZE;
	        unpinned++;

		gasneti_assert(unpinned <= fh_max_regions);

	        if (!has_callback) {
	            fh_PendingCallbacksEnqueue(req, bd, ccb, FH_COMPLETION_END);
		    has_callback = 1;
		}
	    }
	    else if (FH_IS_REMOTE_PENDING(bd)) {
		/* The bucket is pending: We increment the 
		 * refcount and attach a callback if we have not 
		 * attached a callback elsewhere 
		 */

		if (!has_callback) {
		    fh_PendingCallbacksEnqueue(req, bd, ccb, 
			(fh_completion_callback_t *) bd->fh_tqe_next);
		    has_callback = 1;
		}

		FH_BUCKET_REFC(bd)->refc_r++;
		gasneti_assert(FH_BUCKET_REFC(bd)->refc_r > 0);
		FH_TRACE_BUCKET(bd, PENDING);
	    }
	    else {
		/* The bucket exists and is *NOT* pending */
		fh_priv_acquire_remote(node, bd);
	    }
	}

	*new_regions = new_r;

	return unpinned;
}

/* 
 * fh_acquire_local_region(region)
 *
 * In acquiring local pages covered over the region, pin calls are coalesced.
 * Acquiring a page may lead to a pin call but always results in the page
 * reference count being incremented.
 *
 * Under firehose-page, acquiring means finding bucket descriptors for each
 * bucket in the region and incrementing the bucket descriptor's reference
 * count.
 *
 * Called by fh_local_pin() (firehose_local_pin, firehose_local_try_pin)
 */

void
fh_acquire_local_region(firehose_request_t *req)
{
	int			b_num, b_total;
	firehose_region_t	region;
	fhi_RegionPool_t	*pin_p;


	FH_TABLE_ASSERT_LOCKED;

	gasneti_assert(req->node == gasneti_mynode);

	b_total = FH_NUM_BUCKETS(req->addr, req->len);
	/* Make sure the size of the region respects the local limits */
	gasneti_assert(b_total <= fhc_MaxVictimBuckets);

	pin_p = fhi_AllocRegionPool(FH_MIN_REGIONS_FOR_BUCKETS(b_total));
	region.addr = req->addr;
	region.len  = req->len;
	b_num = fhi_AcquireLocalRegionsList(1, &region, 1, pin_p);

	gasneti_assert(b_num >= 0);

	/* b_num contains the number of new Buckets to be pinned.  We 
	 * may have to unpin Buckets in order to respect the threshold on
	 * locally pinned buckets. */
	if (b_num > 0) {
		fhi_RegionPool_t	*unpin_p;

		unpin_p = 
		    fhi_AllocRegionPool(FH_MIN_REGIONS_FOR_BUCKETS(b_num));

		unpin_p->regions_num = 
			fh_WaitLocalFirehoses(b_num, unpin_p->regions);

		FH_TABLE_UNLOCK;
		firehose_move_callback(gasneti_mynode,
				unpin_p->regions, unpin_p->regions_num,
				pin_p->regions, pin_p->regions_num);
		FH_TABLE_LOCK;

		fhi_InitLocalRegionsList(1, 
					 pin_p->regions, pin_p->regions_num);

		fhi_FreeRegionPool(unpin_p);
		GASNETI_TRACE_EVENT(C,FH_LOCAL_MISS);
	}
	else {
		GASNETI_TRACE_EVENT(C,FH_LOCAL_HIT);
	}

	fhi_FreeRegionPool(pin_p);

	return;
}

/* fh_acquire_remote_region(request, callback, context, flags,
 *                          remotecallback_args_fn)
 *
 * The function only requests a remote pin operation (AM) if one of the pages
 * covered in the region is not known to be pinned on the remote host.  Unless
 * the entire region hits the remote firehose hash, the value of the internal
 * pointer is set to FH_REQ_UNPINNED and a request for remote pages to be
 * pinned is enqueued.
 *
 * NOTE: this function begins with the table lock held an concludes with the
 * table lock released!!!
 * XXX/PHH: Can the UNBIND and AM stuff move to firehose.c to avoid this?
 *      If so, we must stop using alloca() here!  Instead of alloca(), we
 *      should pass the AM-args around, eliminating the scalar arguments
 *      entirely in favor of packing a struct provided by the caller
 *      (in firehose.c), and replacing 'args'.
 *      That means either calling alloca() for the maximum size
 *      unconditionally in the caller, or equivalently having an automatic
 *	variable or thread-local buffer of maximum size.
 */

void
fh_acquire_remote_region(firehose_request_t *req,
			 firehose_completed_fn_t callback, void *context,
			 uint32_t flags, 
			 firehose_remotecallback_args_fn_t args_fn)
{
	int			 notpinned, new_r = 0;
	fh_completion_callback_t  ccb;
	gasnet_node_t node;

	FH_TABLE_ASSERT_LOCKED;

	node = req->node;
	req->internal = NULL;

	/* Fill in a completion callback struct temporarily as it may be used
	 * in fhuni_TryAcquireRemoteRegion() */
	ccb.flags = FH_CALLBACK_TYPE_COMPLETION;
	ccb.fh_tqe_next = FH_COMPLETION_END;
	ccb.callback = callback;
	ccb.request  = req;
	ccb.context  = context;

	/* Writes the non-pinned buckets to temp_buckets array */
	notpinned = fhuni_TryAcquireRemoteRegion(req, &ccb, &new_r);

	GASNETI_TRACE_PRINTF(C, 
	    ("Firehose fh_acquire_remote_region %d ("GASNETI_LADDRFMT",%d) "
	     "(%d buckets unpinned, flags=0x%x)",
	     node, GASNETI_LADDRSTR(req->addr), (int) req->len,
	     notpinned, req->flags));

	/* 
	 * In moving remote regions, none of the temp arrays can be used, as
	 * the AM call has to be done without holding the TABLE lock.  For this
	 * reason, alloca() is used to acquire a temporary array of regions.
	 */

	if (notpinned > 0) {
		int			replace_b, old_r = 0, free_b;
		void			*reg_alloc;
		firehose_region_t	*reg_alloc_new, *reg_alloc_old;
		int			args_len = 0;

		gasneti_assert(req->internal != NULL);
		gasneti_assert(req->flags & FH_FLAG_PENDING);

		/* Make sure the size of the region respects the remote limits */
		if_pf(notpinned > fhc_RemoteBucketsM)
		    gasneti_fatalerror("Region of %ld bytes too large for only "
		       "%d available remote firehoses (%ld bytes)",
		       (unsigned long) req->len, fhc_RemoteBucketsM,
		       (unsigned long) fhc_RemoteBucketsM*FH_BUCKET_SIZE);

		/* If the remote victim fifo is not full, no replacements are
		 * necessary */
		free_b = MIN(notpinned,
			    fhc_RemoteBucketsM - fhc_RemoteBucketsUsed[node]);
		replace_b = notpinned - free_b;

		/* See if we need any args */
		if (flags & FIREHOSE_FLAG_ENABLE_REMOTE_CALLBACK)
			args_len = sizeof(firehose_remotecallback_args_t);

		/* We've calculated 'new_r' regions will be sufficient for the
		 * replacement buckets and estimate a worst-case of 'replace_r'
		 * will be required for replacement firehoses 
		 * XXX should keep stats on the size of the alloca */
		reg_alloc = (void *)
			alloca(sizeof(firehose_region_t) * (new_r+replace_b) +
			       args_len);
		reg_alloc_new = (firehose_region_t *) reg_alloc;

		/* Coalesce new buckets into a minimal amount of regions */
		new_r = fhuni_CoalesceBuckets(fh_temp_buckets, notpinned,
				reg_alloc_new);

		gasneti_assert(new_r > 0);
		reg_alloc_old = reg_alloc_new + new_r;

		/* Find replacement buckets if required */
		if (replace_b > 0)
			old_r = fh_WaitRemoteFirehoses(node, replace_b,
					reg_alloc_old);
		else
			old_r = 0;

		fhc_RemoteBucketsUsed[node] += (notpinned - replace_b);

		gasneti_assert(fhc_RemoteBucketsUsed[node] <= fhc_RemoteBucketsM);
		FH_TABLE_UNLOCK;

		/* Construct remote callback args if required */
		if (args_len > 0) {
			args_len = args_fn(context,
					   (firehose_remotecallback_args_t *)(reg_alloc_old + old_r));
		}

		#ifdef FIREHOSE_UNBIND_CALLBACK
		if (old_r > 0)
			firehose_unbind_callback(node, reg_alloc_old, old_r);
		#endif

		req->flags |= FH_FLAG_INFLIGHT;

                MEDIUM_REQ(4,5,
			   (node,
			    fh_handleridx(fh_am_move_reqh),
			    reg_alloc, 
			    sizeof(firehose_region_t)*(new_r+old_r)+args_len, 
			    flags,
			    new_r,
			    old_r,
			    PACK(NULL)));
	}
	else {
		/* Only set the PINNED flag if the request is not set on any
		 * pending buckets */
		if (!(req->flags & FH_FLAG_PENDING))
			req->flags |= FH_FLAG_PINNED;
		FH_TABLE_UNLOCK;
	}

	FH_TABLE_ASSERT_UNLOCKED;
}
#endif /* FIREHOSE_SMP */

/*
 * XXX not smp-ized
 *
 * fh_release_remote_region(request)
 *
 * This function releases every page in the region described in the firehose
 * request type.
 *
 * Loop over each bucket in reverse order
 * If the reference count reaches zero, push the descriptor at the head of
 * the victim FIFO
 */

void
fh_release_remote_region(firehose_request_t *request)
{
	uintptr_t	end_addr, bucket_addr;
	fh_bucket_t	*bd;

	FH_TABLE_ASSERT_LOCKED;

	end_addr = request->addr + request->len - 1;

	GASNETI_TRACE_PRINTF(C, ("Firehose release_remote_region("
				 GASNETI_LADDRFMT", %d) "GASNETI_LADDRFMT,
				 GASNETI_LADDRSTR(request->addr),
				 (int) request->len,
				 GASNETI_LADDRSTR(request)));

	/* Process region in reverse order so regions can be later coalesced in
	 * the proper order (lower to higher address) from the FIFO */
	FH_FOREACH_BUCKET_REV(request->addr, end_addr, bucket_addr) {
		bd = fh_bucket_lookup(request->node, bucket_addr);
		gasneti_assert(bd != NULL);
		FH_BSTATE_ASSERT(bd, fh_used);
		gasneti_assert(!FH_IS_REMOTE_PENDING(bd));

		fh_priv_release_remote(request->node, bd);
	}

	gasneti_assert(
	    fhc_RemoteVictimFifoBuckets[request->node] <= fhc_RemoteBucketsM);

	return;
}


/* ##################################################################### */
/* ACTIVE MESSAGES                                                       */ 
/* ##################################################################### */
int
fh_move_request(gasnet_node_t node,
		firehose_region_t *new_reg, size_t r_new,
		firehose_region_t *old_reg, size_t r_old,
		void *context)
{
	fhi_RegionPool_t	*rpool;
	int			b_num, i, r_alloc;

	FH_TABLE_ASSERT_UNLOCKED;
	FH_TABLE_LOCK;

	GASNETI_TRACE_PRINTF(C, ("Firehose move request: new=%d, old=%d",
				 (int) r_new, (int) r_old));

	/* Loop over the new regions to count the worst case number of
	 * regions we will need to describe their unpinned subset. */
	for (i=0, r_alloc=0; i < r_new; ++i) {
		r_alloc += FH_MIN_REGIONS_FOR_BUCKETS(
			   FH_NUM_BUCKETS(
				new_reg[i].addr, new_reg[i].len));
	}

	rpool = fhi_AllocRegionPool(r_alloc);
	b_num = fhi_AcquireLocalRegionsList(0, new_reg, r_new, rpool);

	GASNETI_TRACE_PRINTF(C, ("Firehose move request: pin new=%d",
				 (int) rpool->buckets_num));

	/* The next function may overcommit the fifo before the call to
	 * actually pin new regions is issued. */
	fhi_ReleaseLocalRegionsList(0, old_reg, r_old);

	fh_AdjustLocalFifoAndPin(node, rpool->regions, rpool->regions_num);
	fhi_InitLocalRegionsList(0, rpool->regions, rpool->regions_num);
	fhi_FreeRegionPool(rpool);

	FH_TABLE_UNLOCK;

	return b_num;
}

static
void
fh_dump_fhparams(FILE *fp)
{
    if (gasneti_mynode)
	return;

    if (fp == NULL)
	fp = stderr;

    fprintf(fp, "MaxPinnable Memory = %8u MB, %lu bytes\n", 
		FH_PRINTMB(fh_MaxPinnableMemory), 
		(unsigned long) fh_MaxPinnableMemory);
    fprintf(fp, "Firehose M         = %8u MB, %lu bytes\n", 
		FH_PRINTMB(fh_M), (unsigned long) fh_M);
    fprintf(fp, "Firehose MaxVictim = %8u MB, %lu bytes\n", 
		FH_PRINTMB(fh_Maxvictim), (unsigned long) fh_Maxvictim);
    fflush(fp);
    return;
}

void
fh_dump_counters(void)
{
    int 		i;

    /* Local counters */
    printf("%d> MaxVictimB=%d, Local[Only/Fifo]=[%d/%d]\n",
	gasneti_mynode, fhc_MaxVictimBuckets, fhc_LocalOnlyBucketsPinned, 
	fhc_LocalVictimFifoBuckets);

    /* Remote counters */
    for (i = 0; i < gasneti_nodes; i++) {
	if (i == gasneti_mynode)
	    continue;
	printf("%d> RemoteBuckets on %2d = [Used=%6d/FIFO=%6d,"
	    "total=%6d,n_avail=%6d]\n", gasneti_mynode, i, fhc_RemoteBucketsUsed[i], 
	    fhc_RemoteVictimFifoBuckets[i], fhc_RemoteBucketsM, FHI_AVAIL(i));
    }
}
#endif
