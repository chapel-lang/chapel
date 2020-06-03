/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/firehose/firehose_internal.h $
 * Description: Internal Header file
 * Copyright 2004, Christian Bell <csbell@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include <inttypes.h>
#include <gasnet_internal.h>	/* gasnet mutex */

/*
 * If we are building against a threaded client, firehose-smp support needs to
 * be hooked in
 */
#ifdef GASNET_PAR
#define FIREHOSE_SMP 1
#else
#define FIREHOSE_SMP 0
#endif

/*
 * The following define is used only when users do not specify
 * the amount of memory required for the MACVICTIM_M parameter.
 * A conduit can also provide a default.
 */
#ifndef FH_MAXVICTIM_TO_PHYSMEM_RATIO
#define FH_MAXVICTIM_TO_PHYSMEM_RATIO   0.25
#endif

/* Some conduits may be able to support running both the completion and remote
 * callbacks from within an AM Handler, in which case there is no need for the
 * client to make progress through firehose_poll().
 */
#if defined(FIREHOSE_REMOTE_CALLBACK_IN_HANDLER) && \
    defined(FIREHOSE_COMPLETION_IN_HANDLER)
#define FH_POLL_NOOP
#endif

/* If unspecified in the firehose_fwd.h, we default to using gasneti_AMPoll().
 */
#ifndef FIREHOSE_AMPOLL
#define FIREHOSE_AMPOLL() gasneti_AMPoll()
#endif

extern int fh_verbose;

/* 
 * Locks
 */

#ifdef DEBUG_LOCAL_TABLE /* NOTE: not yet implemented for page */
  extern void _fhi_debug_local_table(const char *, int line);
  #define fhi_debug_local_table() _fhi_debug_local_table(GASNETI_CURRENT_FUNCTION, __LINE__)
  extern int fhc_LocalReserved; /* space reserved for upcoming pin */
#else
  #define fhi_debug_local_table() ((void)0)
#endif

extern gasneti_mutex_t		fh_table_lock;

#define FH_TABLE_LOCK		do { gasneti_mutex_lock(&fh_table_lock);               \
				     gasneti_compiler_fence();                         \
				     gasneti_assert(FHC_MAXVICTIM_BUCKETS_AVAIL >= 0); \
				     fhi_debug_local_table();                          \
				} while (0)
#define FH_TABLE_UNLOCK		do { gasneti_assert(FHC_MAXVICTIM_BUCKETS_AVAIL >= 0); \
				     fhi_debug_local_table();                          \
				     gasneti_compiler_fence();                         \
				     gasneti_mutex_unlock(&fh_table_lock);             \
				} while (0)
#define FH_TABLE_ASSERT_LOCKED	gasneti_mutex_assertlocked(&fh_table_lock)
#define FH_TABLE_ASSERT_UNLOCKED gasneti_mutex_assertunlocked(&fh_table_lock)

#ifndef FH_POLL_NOOP /* Don't need a pollq_lock when we don't poll */
  extern gasneti_mutex_t		fh_pollq_lock;

  #define FH_POLLQ_LOCK		gasneti_mutex_lock(&fh_pollq_lock)
  #define FH_POLLQ_UNLOCK	gasneti_mutex_unlock(&fh_pollq_lock)
#else
  #define FH_POLLQ_LOCK		!!! error - no firehose polling !!!
  #define FH_POLLQ_UNLOCK	!!! error - no firehose polling !!!
#endif

/* 
 * INIT FLAGS
 * fhi_InitFlags -
 *     Holds value of 'flags' argument passed to firehose_init().
 *     This makes these flags available to the firehose_fini and
 *     to the _init and _fini plugins.
 */
extern uint32_t fhi_InitFlags;

/* 
 * LOCAL COUNTERS
 *    XXX/PHH: actually counts of private_t's, rather than bucket_t's
 *
 * fhc_LocalOnlyBucketsPinned - incrementing counter
 *     Amount of buckets pinned by the local node or in the FIFO
 *     (localref > 0 OR remoteref == 0).  This count must be less than
 *     or equal to fhc_MaxVictimBuckets in order to avoid deadlocks.
 *
 * fhc_LocalVictimFifoBuckets - incrementing counter
 *     Amount of buckets currently contained in the Local Victim FIFO. 
 *
 * fhc_MaxVictimBuckets - static count
 *     Maximum amount of victims that may be pinned other than M.
 *     fhc_LocalOnlyBucketsPinned <= fhc_MaxVictimBuckets
 */

extern int	fhc_LocalOnlyBucketsPinned;
extern int	fhc_LocalVictimFifoBuckets;
extern int	fhc_MaxVictimBuckets;

#define FHC_MAXVICTIM_BUCKETS_AVAIL 					\
		(fhc_MaxVictimBuckets - fhc_LocalOnlyBucketsPinned)

/* 
 * REMOTE COUNTERS
 *
 * fhc_RemoteBucketsM - static count
 *    Amount of per-node firehoses that can be mapped as established by the
 *    firehose 'M' parameter.
 *
 * fhc_MaxRemoteBuckets - static count
 *     Maximum number of buckets that can be pinned in a single AM call.
 * 
 * fhc_RemoteBucketsUsed[0..nodes-1] - Array of incrementing counters
 *    Amount of buckets currently used by the current node.
 *
 * fhc_RemoteVictimFifoBuckets[0..nodes-1] - Array of incrementing counters
 *     Available amount of remote buckets that can be used without sending
 *     replacement buckets.
 *
 */
extern int	 fhc_RemoteBucketsM;
extern int	 fhc_MaxRemoteBuckets;
extern int	*fhc_RemoteBucketsUsed;
extern int	*fhc_RemoteVictimFifoBuckets;

#define FHI_REMOTE_AVAIL(node)                                               \
    (                                                                        \
        (fhc_RemoteBucketsM - fhc_RemoteBucketsUsed[node]) /* Free energy */ \
        + fhc_RemoteVictimFifoBuckets[node] /* FIFO */                       \
    )

#ifndef FH_BUCKET_SIZE
#define FH_BUCKET_SIZE	GASNET_PAGESIZE
#endif

#ifndef FH_BUCKET_SHIFT
#define FH_BUCKET_SHIFT GASNETT_PAGESHIFT
#endif

/* Utility Macros */
#define FH_CACHE_LINE_BYTES	GASNETI_CACHE_LINE_BYTES
#define FH_PAGE_MASK		(GASNET_PAGESIZE-1)
#define FH_ADDR_ALIGN(addr)	(GASNETI_ALIGNDOWN(addr, FH_BUCKET_SIZE))
#define FH_SIZE_ALIGN(addr,len)	(GASNETI_ALIGNUP(addr+len, FH_BUCKET_SIZE)-\
				 GASNETI_ALIGNDOWN(addr, FH_BUCKET_SIZE))
#define FH_NUM_BUCKETS(addr,len)(FH_SIZE_ALIGN(addr,len)>>FH_BUCKET_SHIFT)
#define FH_ASSERT_BUCKET_ADDR(bucket) (gasneti_assert((bucket) % FH_BUCKET_SIZE == 0))

#if !defined(GASNET_MAXNODES)
  #error "GASNET_MAXNODES undefined"
#elif GASNET_MAXNODES <= GASNET_PAGESIZE
  #define FH_KEY_PACKED 1
  typedef uintptr_t              fh_key_t;
  #define FH_KEYMAKE(addr,node)  ((addr) | (node))
  #define FH_KEY_EQ(x,y)         ((x) == (y))
  #define FH_KEY2INT(x)          ((intptr_t)(x))

  #define FH_NODE(priv)    ((*(fh_key_t*)(priv)) & FH_PAGE_MASK)
  #define FH_BADDR(priv)   ((*(fh_key_t*)(priv)) & ~FH_PAGE_MASK)
#else
  #define FH_KEY_STRUCT 1
  typedef struct {
    uintptr_t     addr;
    uintptr_t     node; /* Might instead hold len in region case */
  }	  fh_key_t;
  GASNETI_INLINE(fh_keymake)
  fh_key_t fh_keymake(uintptr_t addr, gex_Rank_t node)
  { fh_key_t key; key.addr = addr; key.node = node; return key; }
  #define FH_KEYMAKE(addr,node) fh_keymake(addr,node)
  #define FH_KEY_EQ(x,y)   (((x).addr == (y).addr) && ((x).node == ((y).node)))
  #define FH_KEY2INT(x)    ((intptr_t)(x).addr ^ (intptr_t)(x).node)

  #define FH_NODE(priv)    (((fh_key_t*)(priv))->node)
  #define FH_BADDR(priv)   (((fh_key_t*)(priv))->addr)
#endif

/* fh_bucket_t
 *
 * The firehose bucket type is a descriptor for a single page (or multiple amount
 * of pages according to the ability for the underlying memory allocator to
 * allocate in multiples of GASNET_PAGESIZE).
 *
 * The current implementation equates one bucket to one page.
 *
 * Under both firehose-page and firehose-region, bucket descriptors for all the
 * buckets contained in the region to be pinned are added to the firehose hash
 * table (for both remote and local pins).
 */

#if PLATFORM_ARCH_32
typedef uint16_t	fh_refc_uint_t;
#else
typedef uint32_t	fh_refc_uint_t;
#endif

/* The 'refcount' type is stored as a union with the fh_tqe_prev field
 * and must therefore be the same size as a pointer */
typedef struct _fh_refc_t {
	fh_refc_uint_t	refc_l;
	fh_refc_uint_t	refc_r;
}
fh_refc_t;

/*
 * Bucket and private types
 */
#if GASNET_DEBUG
  #define DEBUG_BUCKETS
#endif
#ifdef DEBUG_BUCKETS
  typedef enum { fh_local_fifo, fh_remote_fifo, fh_pending, fh_pending_commit, fh_used, fh_unused }
  fh_bstate_t;
  #define FH_BSTATE_ASSERT(entry, state) gasneti_assert((entry)->fh_state == state)
  #define FH_BSTATE_SET(entry, state)	 (entry)->fh_state = state
#else
  #define FH_BSTATE_ASSERT(entry, state)
  #define FH_BSTATE_SET(entry, state)
#endif

struct _fh_bucket_t; /* forward decl of type */

struct _firehose_private_t {
        fh_key_t         fh_key;                 /* cached key for hash table */

        void            *fh_next;		 /* linked list in hash table */
						 /* _must_ be in this order */

	/* FIFO and refcount */
	firehose_private_t *fh_tqe_next;	/* -1 when not in FIFO, 
						   NULL when end of list,
						   else next pointer in FIFO */
    union {
	firehose_private_t **fh_tqe_prev;	/* refcount when not in FIFO,
						   prev pointer otherwise    */
	fh_refc_t fh_refc;
    } u;

	#ifdef DEBUG_BUCKETS
	fh_bstate_t	fh_state;
	int		prepinned;
	#endif

	/* Region-specific additional fields: */
	#ifdef FIREHOSE_REGION
	size_t			len;
	struct _fh_bucket_t	*bucket;	/* pointer to first bucket */
	int			visible;

	#ifdef FIREHOSE_CLIENT_T
	firehose_client_t	client;
	#endif /* CLIENT_T */
	#endif /* REGION */
};

#define FH_BUCKET_REFC(priv) (&(priv)->u.fh_refc)

/* Local and Remote buckets can be in various states.
 *
 * Local buckets can be in either of these two states:
 *   1. in FIFO (fh_tqe_next != FH_USED_TAG)
 *   2. in USE  (fh_tqe_next == FH_USED_TAG)
 * The NEW state indicates that the bucket is in the process of
 * being pinned.
 *
 * Remote buckets can be in either of these three states 
 *   1. in USE  (fh_tqe_next == FH_USED_TAG)
 *      a) COMMITTED (refcounts represent a real value)
 *      b) PENDING (LOCAL reference count == FH_REMOTE_PENDING_TAG)
 *      c) ** For SMP-page only:
 *         PENDING COMMIT (LOCAL reference count ==
 *                               FH_REMOTE_PENDING_UNCOMMITTED_TAG)
 *   2. in FIFO (fh_tqe_next != FH_USED_TAG)
 */

/*
 * Local firehose states
 * This assumes that the remote reference count will never exceed the tag
 *
 * Tags used for FH_LOCAL_STATE()
 */
#define FH_LOCAL_PENDING_TAG		((firehose_private_t *) -1)
#define FH_LOCAL_INUSE_TAG		((firehose_private_t *) -2)
#define FH_LOCAL_FIFO_TAG		((firehose_private_t *) -3)
#define FH_COMPLETION_END_TAG		((firehose_private_t *) -4)

#define FH_HAS_COMPLETION_CALLBACK(priv) ((priv)->fh_tqe_next != NULL && \
		                   (priv)->fh_tqe_next <= FH_COMPLETION_END_TAG)

#define FH_LOCAL_STATE(priv) ((priv)->fh_tqe_next < FH_LOCAL_INUSE_TAG \
			     ? FH_LOCAL_FIFO_TAG : (priv)->fh_tqe_next)

#define FH_IS_LOCAL_FIFO(priv)    ((priv)->fh_tqe_next < FH_LOCAL_INUSE_TAG)
#define FH_IS_LOCAL_PENDING(priv) ((priv)->fh_tqe_next == FH_LOCAL_PENDING_TAG)
#define FH_IS_LOCAL_INUSE(priv)   ((priv)->fh_tqe_next == FH_LOCAL_INUSE_TAG)

#define FH_SET_LOCAL_FIFO(priv)  (FH_LOCAL_STATE(priv) = \
	  FH_LOCAL_STATE(priv) >= FH_LOCAL_INUSE_TAG ? NULL : FH_LOCAL_STATE(priv))
#define FH_SET_LOCAL_PENDING(priv) (priv)->fh_tqe_next = FH_LOCAL_PENDING_TAG
#define FH_SET_LOCAL_INUSE(priv)   (priv)->fh_tqe_next = FH_LOCAL_INUSE_TAG

/*
 * Remote firehose states
 *
 * WARNING: These states unfortunately make the code very fragile
 */
#define FH_REMOTE_INUSE_TAG			((firehose_private_t *) -1)
#define FH_REMOTE_PENDING_TAG			((fh_refc_uint_t) -1)
#define FH_REMOTE_PENDING_UNCOMMITTED_TAG	((fh_refc_uint_t) -2)

#define FH_IS_REMOTE_FIFO(priv)	   ((priv)->fh_tqe_next != FH_REMOTE_INUSE_TAG && \
		                     FH_BUCKET_REFC(priv)->refc_l < FH_REMOTE_PENDING_UNCOMMITTED_TAG)
#define FH_IS_REMOTE_INUSE(priv)   ((priv)->fh_tqe_next == FH_REMOTE_INUSE_TAG && \
		                     FH_BUCKET_REFC(priv)->refc_l == 0)
#define FH_IS_REMOTE_PENDING(priv) (FH_BUCKET_REFC(priv)->refc_l==FH_REMOTE_PENDING_TAG)
#define FH_IS_REMOTE_PENDING_UNCOMMITTED(priv)				          \
                    (FH_BUCKET_REFC(priv)->refc_l==FH_REMOTE_PENDING_UNCOMMITTED_TAG)

#define FH_SET_REMOTE_INUSE(priv) do { (priv)->fh_tqe_next = FH_REMOTE_INUSE_TAG; \
	                                FH_BUCKET_REFC(priv)->refc_l = 0; } while (0)
#define FH_SET_REMOTE_PENDING(priv) FH_BUCKET_REFC(priv)->refc_l = FH_REMOTE_PENDING_TAG
#define FH_SET_REMOTE_PENDING_UNCOMMITTED(priv)	 			        \
			FH_BUCKET_REFC(priv)->refc_l = FH_REMOTE_PENDING_UNCOMMITTED_TAG

/*
 * Both -page and -region implement these functions.
 *
 * Reusable functions are found in firehose.c and flavour-specific 
 * functions should be in firehose_page.c and firehose_region.c
 *                                                                       */
/* ##################################################################### */

void	fh_init_plugin(uintptr_t max_pinnable_memory,
		       size_t max_regions, size_t max_region_size,
		       const firehose_region_t *prepinned_regions,
                       size_t num_prepinned, firehose_info_t *info);
void	fh_fini_plugin(void);

/* ##################################################################### */
/* Request type freelists (COMMON)                                       */
/* ##################################################################### */
/* Flags */
#define FH_FLAG_FHREQ	 0x01	/* firehose supplied the request_t */
#define FH_FLAG_PINNED	 0x02
#define FH_FLAG_PENDING  0x04	/* Used in -PAGE only */
#define FH_FLAG_INFLIGHT 0x08

/* ##################################################################### */
/* Firehose Hash Table Utility (COMMON, firehose_hash.c)                 */
/* The hash table utility functions can be used for hashing buckets and  */
/* regions (in firehose-region).                                         */
/* ##################################################################### */

struct _fh_hash_t;
typedef struct _fh_hash_t fh_hash_t;

#if 0 /* We now #include <firehose_hash.c> into firehose_{page,region}.c */
fh_hash_t *	fh_hash_create(size_t entries);
void		fh_hash_destroy(fh_hash_t *hash);
void *		fh_hash_find(fh_hash_t *hash, fh_key_t key);
void *		fh_hash_insert(fh_hash_t *hash, fh_key_t key, void *newval);
void *		fh_hash_next(fh_hash_t *hash, void *val);
void		fh_hash_replace(fh_hash_t *hash, void *val, void *newval);
void		fh_hash_apply(fh_hash_t *hash, void (*fn)(void *val, void *arg), void *arg);
#endif

/* ##################################################################### */
/* FIFO (local and remote) management operations (COMMON, firehose.c)    */
/* ##################################################################### */
		/* Return a descriptor given an existing private_t */
fh_refc_t *	fh_priv_release_local(int local_ref,
				      firehose_private_t *);
fh_refc_t *	fh_priv_release_remote(gex_Rank_t node,
				       firehose_private_t *);
		/* Acquire and exisiting private_t (increments refcount) */
fh_refc_t *	fh_priv_acquire_local(int local_ref,
				      firehose_private_t *);
fh_refc_t *	fh_priv_acquire_remote(gex_Rank_t node,
				       firehose_private_t *);
		/* Wait for local firehoses to release/reuse */
int		fh_WaitLocalFirehoses(int count, firehose_region_t *region);
		/* Wait for remote firehoses to release/reuse */
int		fh_WaitRemoteFirehoses(gex_Rank_t node, int count,
					firehose_region_t *region);
		/* Adjust for possible overcommit and then pin */
void		fh_AdjustLocalFifoAndPin(gex_Rank_t node,
					firehose_region_t *reg_pin,
					size_t pin_num);


/* ##################################################################### */
/* fhi_RegionPool_t (COMMON, firehose.c)                                 */
/* ##################################################################### */
typedef
struct _fhi_RegionPool_t {
	/* 
	 * Used internally 
	 */ 
	size_t		 		len;
	struct _fhi_RegionPool_t	*fh_tqe_next;

	/* 
	 * User modifiable fields 
	 */
	firehose_region_t	*regions;
	size_t			 regions_num;
	size_t			 buckets_num;

	/*
	 * Pad the struct to inhibit false sharing
	 */
	uint8_t			 _pad[MAX(1,(ssize_t)(FH_CACHE_LINE_BYTES-
				      3*sizeof(size_t)-2*sizeof(void*)))];
}
fhi_RegionPool_t;

/* Default size, in regions, of region pool entries */
#if defined(FIREHOSE_PAGE)
  /* Used to gather up to one region per page */
  #define FH_REGIONPOOL_DEFAULT_COUNT	32768
#elif defined(FIREHOSE_REGION)
  /* Until page accounting is done, always use 1 region */
  #define FH_REGIONPOOL_DEFAULT_COUNT	1
#endif

extern fhi_RegionPool_t * fhi_AllocRegionPool(int count);
extern void fhi_FreeRegionPool(fhi_RegionPool_t *rpool);

/* ##################################################################### */
/* Misc functions (specific to page and region)                          */
/* ##################################################################### */
int	fh_region_ispinned(gex_Rank_t node, uintptr_t addr, size_t len);
int	fh_region_partial(gex_Rank_t node, uintptr_t *addr_p, size_t *len_p);

/* ##################################################################### */
/* Misc functions (COMMON, firehose.c)                                   */
/* ##################################################################### */
int64_t fh_getenv(const char *var, unsigned long multiplier);

/* Common Queue Macros for Firehose FIFO and Local Bucket FIFO */
#define FH_TAILQ_HEAD(name, type)	\
struct name {				\
	struct type	*fh_tqh_first;	\
	struct type	**fh_tqh_last;	\
}
#define FH_STAILQ_HEAD(name,type)	FH_TAILQ_HEAD(name,type)

/* QUEUE functions (based on the BSD TAILQ and STAILQ macros of
 * /usr/include/sys/queue.h) */
#define FH_TAILQ_FIRST(head)	((head)->fh_tqh_first)
#define FH_TAILQ_LAST(head)	((head)->fh_tqh_last)
#define FH_TAILQ_EMPTY(head)	((head)->fh_tqh_first == NULL)
#define FH_TAILQ_NEXT(elem)	((elem)->fh_tqe_next)
#define FH_TAILQ_PREV(elem)	((elem)->u.fh_tqe_prev)

#define FH_STAILQ_FIRST(head)	((head)->fh_tqh_first)
#define FH_STAILQ_LAST(head)	((head)->fh_tqh_last)
#define FH_STAILQ_EMPTY(head)	((head)->fh_tqh_first == NULL)
#define FH_STAILQ_NEXT(elem)	((elem)->fh_tqe_next)

/* Doubles/single list initialization */
#define FH_STAILQ_HEAD_INITIALIZER(head)  { NULL, &(head).fh_tqh_first }
#define FH_TAILQ_HEAD_INITIALIZER(head)   { NULL, &(head).fh_tqh_first }

#define FH_TAILQ_INIT(head)	do {				\
	FH_TAILQ_FIRST((head)) = NULL;				\
	FH_TAILQ_LAST(head) = &FH_TAILQ_FIRST((head));		\
} while (0)
#define FH_STAILQ_INIT(head)	FH_TAILQ_INIT(head)

/* Double/single list tail addition */
#define FH_TAILQ_INSERT_TAIL(head, elem) do {				\
	FH_TAILQ_NEXT(elem) = NULL;					\
	FH_TAILQ_PREV(elem) = FH_TAILQ_LAST(head);			\
	*(FH_TAILQ_LAST(head)) = (elem);				\
	FH_TAILQ_LAST(head) = &FH_TAILQ_NEXT(elem);			\
} while (0)
#define	FH_STAILQ_INSERT_TAIL(head, elem) do {				\
	FH_STAILQ_NEXT(elem) = NULL;					\
	*(FH_STAILQ_LAST(head)) = (elem);				\
	FH_STAILQ_LAST(head) = &FH_STAILQ_NEXT(elem);			\
} while (0)

/* Double/single list head addition */
#define FH_TAILQ_INSERT_HEAD(head, elem) do {				\
	if ((FH_TAILQ_NEXT(elem) = FH_TAILQ_FIRST(head)) == NULL)	\
		FH_TAILQ_LAST(head) = &FH_TAILQ_NEXT(elem);		\
	else                                                            \
		FH_TAILQ_PREV(FH_TAILQ_FIRST(head)) =			\
			&FH_TAILQ_NEXT(elem);				\
	FH_TAILQ_FIRST(head) = (elem);					\
	FH_TAILQ_PREV(elem) = &FH_TAILQ_FIRST(head);			\
} while (0)
#define FH_STAILQ_INSERT_HEAD(head, elem) do {				\
	if ((FH_STAILQ_NEXT(elem) = FH_STAILQ_FIRST(head)) == NULL)	\
		FH_STAILQ_LAST(head) = &FH_STAILQ_NEXT(elem);		\
	FH_STAILQ_FIRST(head) = (elem);					\
} while (0);

#define FH_STAILQ_MERGE(head1, head2) do {				\
	*(FH_STAILQ_LAST(head1)) = FH_STAILQ_FIRST(head2);		\
	FH_STAILQ_LAST(head1) = FH_STAILQ_LAST(head2);			\
} while (0)

/* Double remove anywhere in the list */
#define FH_TAILQ_REMOVE(head, elem) do {				\
	if (FH_TAILQ_NEXT(elem) != NULL)				\
		FH_TAILQ_PREV(FH_TAILQ_NEXT(elem)) = 			\
			FH_TAILQ_PREV(elem);				\
	else								\
		FH_TAILQ_LAST(head) = FH_TAILQ_PREV(elem);		\
	*(FH_TAILQ_PREV(elem)) = FH_TAILQ_NEXT(elem);			\
} while (0)

/* Single remove from head only */
#define	FH_STAILQ_REMOVE_HEAD(head) do {				\
	if ((FH_STAILQ_FIRST((head)) =					\
	     FH_STAILQ_NEXT(FH_STAILQ_FIRST((head)))) == NULL)		\
		FH_STAILQ_LAST(head) = &FH_STAILQ_FIRST(head);		\
} while (0)

/* Double/single foreach over the list */
#define FH_TAILQ_FOREACH(head, var)					\
	for ((var) = FH_TAILQ_FIRST(head); (var) != NULL;		\
	     (var) = FH_TAILQ_NEXT(var))
#define FH_STAILQ_FOREACH(head, var)					\
	for ((var) = FH_STAILQ_FIRST(head); (var) != NULL;		\
	     (var) = FH_STAILQ_NEXT(var))

/* ##################################################################### */
/* Firehose/Bucket FIFOs and Callback Polling queues                     */
/* ##################################################################### */
FH_TAILQ_HEAD(_fh_fifoq_t, _firehose_private_t);
typedef struct _fh_fifoq_t	fh_fifoq_t;

FH_STAILQ_HEAD(_fh_pollq_t, _fh_callback_t);
typedef struct _fh_pollq_t	fh_pollq_t;


/* There is also a pollqueue which is drained by firehose_poll */
#ifndef FH_POLL_NOOP
extern fh_pollq_t	fh_CallbackFifo;
#endif

/* 
 * There is a queue under FIREHOSE_SMP for local buckets seen as pending while
 * in an AM handler
 */
#if defined(FIREHOSE_PAGE) && FIREHOSE_SMP
FH_TAILQ_HEAD(_fh_locpendq_t, _fh_remote_callback_t);
typedef struct _fh_locpendq_t	fh_locpendq_t;

extern fh_locpendq_t	fhsmp_LocalPendingList;
extern void		fhsmp_ServiceLocalPendingList(void);
#endif

/* Each node has a FirehoseFifo */
extern fh_fifoq_t	*fh_RemoteNodeFifo;
extern fh_fifoq_t	fh_LocalFifo;

/* This type is used to abstract the use of different callback types in the
 * same fifo.  The 'flags' parameter is used as a tag to differentiate both
 * types.
 */
typedef
struct _fh_callback_t {
	uint32_t	 	flags;
	struct _fh_callback_t	*fh_tqe_next;
}
fh_callback_t;

#define FH_CALLBACK_TYPE_REMOTE		0x01
#define FH_CALLBACK_TYPE_COMPLETION	0x02
#define FH_CALLBACK_TYPE_PENDING	0x04

typedef
struct _fh_remote_callback_t {
	uint32_t		flags;
	struct _fh_remote_callback_t	*fh_tqe_next;
    union {
	struct _fh_remote_callback_t	**fh_tqe_prev; /* used in locpendq */
	uintptr_t                       unused;
    } u;

	gex_Rank_t			node;
	firehose_remotecallback_args_t	args;

	firehose_region_t		*pin_list;
	size_t				 pin_list_num;
	size_t				 reply_len;

	void 				*context;
}
fh_remote_callback_t;

typedef
struct _fh_completion_callback_t {
	uint32_t		flags;
	struct _fh_completion_callback_t	*fh_tqe_next;

	firehose_completed_fn_t	callback;
	firehose_request_t	*request;
	void			*context;
}
fh_completion_callback_t;
#define FH_COMPLETION_END  ((fh_completion_callback_t *)(FH_COMPLETION_END_TAG))

fh_completion_callback_t *	fh_alloc_completion_callback(void);
void	fh_free_completion_callback(fh_completion_callback_t *rc);

/* ##################################################################### */
/* Firehose internal pinning functions                                   */
/* ##################################################################### */
/* See documentation in firehose_page.c                                  */
void	fh_acquire_local_region(firehose_request_t *);
void	fh_commit_try_local_region(firehose_request_t *);
void	fh_release_local_region(firehose_request_t *);

void	fh_acquire_remote_region(firehose_request_t *req,
				firehose_completed_fn_t callback, 
				void *context, uint32_t flags,
		        	firehose_remotecallback_args_fn_t args_fn);
void	fh_commit_try_remote_region(firehose_request_t *);
void	fh_release_remote_region(firehose_request_t *);
int	fh_move_request(gex_Rank_t node,
			firehose_region_t *new_reg, size_t r_new,
			firehose_region_t *old_reg, size_t r_old,
			void *context);
int	fh_find_pending_callbacks(gex_Rank_t node,
				  firehose_region_t *region,
				  int nreg, void *context,
				  fh_pollq_t *PendQ);


/* ##################################################################### */
/* Firehose AM-related things (page/region independent)                  */
/* ##################################################################### */
void fh_send_firehose_reply(fh_remote_callback_t *);
extern gex_AM_Entry_t fh_am_handlers[];
/* Initial value of index for gasnet registration */
#define _hidx_fh_am_move_reqh                   0
#define _hidx_fh_am_move_reph                   0
/* Index into the fh_am_handlers table to obtain the gasnet registered index */
#define _fh_hidx_fh_am_move_reqh                0
#define _fh_hidx_fh_am_move_reph                1
#define fh_handleridx(reqh)     (fh_am_handlers[ _fh_hidx_ ## reqh ].gex_index)

/* ##################################################################### */
/* FIFO (local and remote) management operations (page/region specific)  */
/* ##################################################################### */
int	fh_FreeVictim(int count, firehose_region_t *reg,
			fh_fifoq_t *fifo_head);

GASNETI_INLINE(fhi_FreeVictimLocal)
int fhi_FreeVictimLocal(int count, firehose_region_t *reg)
{
	gasneti_assert(count <= fhc_LocalVictimFifoBuckets);
	return fh_FreeVictim(count, reg, &fh_LocalFifo);
}

GASNETI_INLINE(fhi_FreeVictimRemote)
int fhi_FreeVictimRemote(gex_Rank_t node, int count, firehose_region_t *reg)
{
	gasneti_assert(count <= fhc_RemoteVictimFifoBuckets[node]);
	return fh_FreeVictim(count, reg, &fh_RemoteNodeFifo[node]);
}

/* How many buffers (of buffers) to allocate to use as bucket descriptors in
 * hash table */
#define FH_BUCKETS_BUFS	1024

/*
 * Macros to implement do/while and foreach over the region.  When a reference
 * to 'end' is made, it refers to 'start + len - 1'.
 */
#define FH_FOREACH_BUCKET(start,end,bucket_addr)			\
		for ((bucket_addr) = (start); (bucket_addr) <= (end);	\
		    (bucket_addr) += FH_BUCKET_SIZE)
#define FH_FOREACH_BUCKET_REV(start,end,bucket_addr)			\
		for ((bucket_addr) = FH_ADDR_ALIGN(end);		\
			(bucket_addr) >= (start);			\
			(bucket_addr) -= FH_BUCKET_SIZE)
#define FH_DO_BUCKET(start,bucket_addr)					\
		(bucket_addr) = (start); do {
#define FH_WHILE_BUCKET(end,bucket_addr)				\
		} while ((bucket_addr) <= (end) && 			\
			(bucket_addr) += FH_BUCKET_SIZE)

#define FH_FOREACH_BUCKET_IN_POOL(i,pool,bucket_addr,bucket_end)	\
	for (i=0; i < pool->regions_num; i++)				\
	    for (bucket_addr = pool->regions[i].addr,			\
		 bucket_end = pool->regions[i].addr +			\
		              pool->regions[i].len - 1;			\
		 bucket_addr <= bucket_end; bucket_addr += FH_BUCKET_SIZE)

/*
 * Macros to copy client_t to and from region/request
 */
#ifdef FIREHOSE_CLIENT_T
#define FH_COPY_REGION_TO_REQUEST(req, reg) do {			\
		(req)->addr = (uintptr_t) (reg)->addr;			\
		(req)->len  = (size_t) (reg)->len;			\
		memcpy(&((req)->client), &((reg)->client), 		\
		    sizeof(firehose_client_t));				\
	} while (0)
#define FH_COPY_REQUEST_TO_REGION(reg, req) do {			\
		(reg)->addr = (uintptr_t) (req)->addr;			\
		(reg)->len  = (size_t) (req)->len;			\
		memcpy(&((reg)->client), &((req)->client), 		\
		    sizeof(firehose_client_t));				\
	} while (0)
#else
#define FH_COPY_REGION_TO_REQUEST(req, reg) do {			\
		(req)->addr = (uintptr_t) (reg)->addr;			\
		(req)->len  = (size_t) (reg)->len;			\
	} while (0)
#define FH_COPY_REQUEST_TO_REGION(reg, req) do {			\
		(reg)->addr = (uintptr_t) (req)->addr;			\
		(reg)->len  = (size_t) (req)->len;			\
	} while (0)
#endif

#if GASNET_TRACE
#ifdef FIREHOSE_REGION
  /* Total hack:
   * FH_NODE(priv) doesn't work in firehose-region because the
   * "private_t" doesn't have the node info in the "key" field.
   * This hack lets us grab the info from the "key" field of the first
   * bucket w/o exposing the bucket_t outside of firehose_region.c
   */
  #define FH_PRIV_NODE(p) ((p)->bucket ? FH_NODE((p)->bucket) : GEX_RANK_INVALID)
#else
  #define FH_PRIV_NODE(p) FH_NODE(p)
#endif
#define FH_TRACE_BUCKET(bd, bmsg) 					\
	do {								\
		char	msg[64];					\
		fh_refc_t *rp = FH_BUCKET_REFC(bd);			\
		if (FH_PRIV_NODE(bd) != gasneti_mynode) {			\
			if (FH_IS_REMOTE_PENDING(bd)) 			\
				sprintf(msg, "rrefc=%d PENDING",	\
				    rp->refc_r);			\
			else if (FH_IS_REMOTE_FIFO(bd))			\
				sprintf(msg, "IN FIFO");		\
			else						\
				sprintf(msg, "rrefc=%d", rp->refc_r);   \
		}							\
		else {							\
			if (FH_IS_LOCAL_FIFO(bd))			\
				sprintf(msg, "IN FIFO");		\
			else						\
				sprintf(msg, "rrefc=%d lrefc=%d",	\
				    rp->refc_r, rp->refc_l);		\
		}							\
		GASNETI_TRACE_PRINTF(C,					\
		    ("Firehose Bucket %s %s node=%d,addr="              \
		     GASNETI_LADDRFMT",%s", #bmsg,                      \
		     FH_PRIV_NODE(bd) == gasneti_mynode ? "Local ":"Remote", \
		     (int) FH_PRIV_NODE(bd),                            \
		     GASNETI_LADDRSTR(FH_BADDR(bd)), msg));		\
	} while (0)

#define FH_NUMPINNED_DECL	int _fh_numpinned = 0
#define FH_NUMPINNED_INC	_fh_numpinned++
#define FH_NUMPINNED_TRACE_LOCAL	GASNETI_TRACE_EVENT_VAL(C, \
					BUCKET_LOCAL_PINS, _fh_numpinned)
#define FH_NUMPINNED_TRACE_REMOTE	GASNETI_TRACE_EVENT_VAL(C, \
					BUCKET_REMOTE_PINS, _fh_numpinned)
#else
#define FH_TRACE_BUCKET(bd, bmsg)
#define FH_NUMPINNED_DECL
#define FH_NUMPINNED_INC
#define FH_NUMPINNED_TRACE_LOCAL
#define FH_NUMPINNED_TRACE_REMOTE
#endif


/*
 * Conduit Features	gm-conduit	vapi-conduit	sci-conduit
 * ------------------------------------------------------------------
 * flavour		page		region		?
 * client_t		no		yes		yes
 * bind callback	no		yes		yes
 * unbind callback	no		yes		yes
 *
 * Callbacks		gm-conduit	vapi-conduit	sci-conduit
 * ------------------------------------------------------------------
 * move callback	unpins/pins	repins ?	unpins,	
 * 							selects segmentId,
 * 							stores sci_local_segment_t
 *
 * bind callback	n/a		?		connects to segmentId,
 * 							stores sci_remote_segment_t
 *
 * unbind callback	n/a		?		disconnects sci_remote_segment_t
 */
