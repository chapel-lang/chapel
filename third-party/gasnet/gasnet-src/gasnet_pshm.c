/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_pshm.c $
 * Description: GASNet infrastructure for shared memory communications
 * Copyright 2012, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>

#if GASNET_PSHM /* Otherwise file is empty */

#include <gasnet_core_internal.h> /* for gasnetc_handler[] */
#include <gasnet_am.h> /* for gasneti_prepare_alloc_buffer() */

#include <sys/types.h>
#include <signal.h>

#if defined(GASNETI_USE_GENERIC_ATOMICOPS) || defined(GASNETI_USE_OS_ATOMICOPS)
  #error "GASNet PSHM support requires Native atomics"
#endif

/* payload memory available for outstanding requests in units of max message size */
#define GASNETI_PSHM_NETWORK_DEPTH_DEFAULT (32UL)
#define GASNETI_PSHM_NETWORK_DEPTH_MIN     (4UL)
#define GASNETI_PSHM_NETWORK_DEPTH_MAX     ((unsigned long)GASNETI_ATOMIC_MAX/GASNETI_PSHMNET_ALLOC_MAXSZ)

/* Global vars */
gasneti_pshmnet_t *gasneti_request_pshmnet = NULL;
gasneti_pshmnet_t *gasneti_reply_pshmnet = NULL;
 
/* Structure for PSHM intra-supernode barrier */
gasneti_pshm_barrier_t *gasneti_pshm_barrier = NULL; /* lives in shared space */

static unsigned long gasneti_pshmnet_network_depth = GASNETI_PSHM_NETWORK_DEPTH_DEFAULT;
static uintptr_t gasneti_pshmnet_queue_mem = 0;

static void *gasnetc_pshmnet_region = NULL;

static struct gasneti_pshm_info {
    gasneti_atomic_t    bootstrap_barrier_cnt;
    char _pad1[GASNETI_CACHE_PAD(sizeof(gasneti_atomic_t))];
    gasneti_atomic_t    bootstrap_barrier_gen;
    char _pad2[GASNETI_CACHE_PAD(sizeof(gasneti_atomic_t))];
    /* early_barrier will be overwritten by other vars after its completion */
    /* sig_atomic_t should be wide enough to avoid word-tearing, right? */
    union {
      volatile sig_atomic_t val;
      char _pad[GASNETI_CACHE_LINE_BYTES];
   }                    early_barrier[1]; /* variable length array */
} *gasneti_pshm_info = NULL;
#define GASNETI_PSHM_BSB_LIMIT (GASNETI_ATOMIC_MAX - 2)

#define round_up_to_pshmpage(size_or_addr)               \
        GASNETI_ALIGNUP(size_or_addr, GASNETI_PSHMNET_PAGESIZE)

#define pshmnet_get_struct_addr_from_field_addr(structname, fieldname, fieldaddr) \
        ((structname*)(((uintptr_t)fieldaddr) - offsetof(structname,fieldname)))

static void (*gasnetc_pshm_abort_callback)(void);

void gasneti_pshm_prefault(void *addr, size_t len) {
  uint8_t *p = addr;
  // Full pages:
  for (size_t i = 0; i < len; i+= GASNET_PAGESIZE) { p[i] = 0; }
  // Final byte, possibly redundant
  p[len-1] = 0;
}

void *gasneti_pshm_init(gasneti_bootstrapBroadcastfn_t snodebcastfn, size_t aux_sz) {
  size_t vnetsz, mmapsz;
  int discontig = 0;
  gex_Rank_t i;
#if !GASNET_CONDUIT_SMP
  gex_Rank_t j;
#endif

  gasneti_assert(snodebcastfn != NULL);  /* NULL snodebcastfn no longer supported */

  gasneti_assert_always_uint(gasneti_nodemap_local_count ,<=, GASNETI_PSHM_MAX_NODES);
    
  gasneti_pshm_nodes = gasneti_nodemap_local_count;
  gasneti_pshm_mynode = gasneti_nodemap_local_rank;
  gasneti_pshm_firstnode = gasneti_nodemap_local[0];

#if GASNET_CONDUIT_SMP
  gasneti_assert_uint(gasneti_pshm_nodes ,==, gasneti_nodes);
  gasneti_assert_uint(gasneti_pshm_mynode ,==, gasneti_mynode);
  gasneti_assert_uint(gasneti_pshm_firstnode ,==, 0);
#else
  /* Determine if local supernode members are numbered contiguously */
  for (i = 1; i < gasneti_nodemap_local_count; ++i) {
    if (gasneti_nodemap_local[i] != gasneti_pshm_firstnode + i) {
      discontig = 1;
      break;
    }
  }
#endif

  gasneti_assert(gasneti_nodemap_global_count > 0);

  /* compute size of vnet shared memory region */
  vnetsz = gasneti_pshmnet_memory_needed(gasneti_pshm_nodes); 
  mmapsz = (2*vnetsz);
  size_t info_sz;
  { // gasneti_pshm_info contains different data (of variable length) at different times

    // 1. Space for the bootstrap barrier (long-lived)
    // The later allocations assume alignment
    size_t sz1 = offsetof(struct gasneti_pshm_info, early_barrier);
    gasneti_assert_uint(sz1 ,==, GASNETI_ALIGNUP(sz1, GASNETI_CACHE_LINE_BYTES));
    gasneti_assert_uint(sz1 ,==, GASNETI_ALIGNUP(sz1, sizeof(gasneti_pshm_rank_t)));

    // 2a. Space for the early barrier (short-lived)
    size_t sz2a = gasneti_pshm_nodes * sizeof(gasneti_pshm_info->early_barrier[0]);

    // 2b. Space for long-lived data, recycling space from the early barrier
    // gasneti_pshm_firsts:
    size_t sz2b = (gasneti_nodemap_global_count * sizeof(gex_Rank_t));
    // optional gasneti_pshm_rankmap[], properly aligned for its type
    if (discontig) {
      sz2b = GASNETI_ALIGNUP(sz2b, sizeof(gasneti_pshm_rank_t));
      sz2b += gasneti_nodes * sizeof(gasneti_pshm_rank_t);
    }
    // PSHM intra-node barrier (cache aligned)
    sz2b = GASNETI_ALIGNUP(sz2b, GASNETI_CACHE_LINE_BYTES);
    sz2b += sizeof(gasneti_pshm_barrier_t) +
	       (gasneti_pshm_nodes - 1) * sizeof(gasneti_pshm_barrier->node);

    // final info_sz required:
    info_sz = sz1 + MAX(sz2a, sz2b);

    // total to request:
    mmapsz += round_up_to_pshmpage(info_sz);
  }
  mmapsz += round_up_to_pshmpage(aux_sz);

  /* setup vnet shared memory region for AM infrastructure and supernode barrier.
   */
  gasnetc_pshmnet_region = gasneti_mmap_vnet(mmapsz, snodebcastfn);
  gasneti_assert_always_uint((((uintptr_t)gasnetc_pshmnet_region) % GASNETI_PSHMNET_PAGESIZE) ,==, 0);
  if (gasnetc_pshmnet_region == NULL) {
    const int save_errno = errno;
    char buf[16];
    gasneti_unlink_vnet();
    gasneti_fatalerror("Failed to mmap %s for intra-node shared memory communication, errno=%s(%i)",
                       gasneti_format_number(mmapsz, buf, sizeof(buf), 1),
                       strerror(save_errno), save_errno);
  }
  
  /* Prepare the shared info struct (including bootstrap barrier) */
  gasneti_pshm_info = (struct gasneti_pshm_info *)((uintptr_t)gasnetc_pshmnet_region + 2*vnetsz);

  /* "early" barrier which protects initialization of the real barrier counter. */
  gasneti_local_wmb();
  if (gasneti_pshm_mynode) {
    gasneti_pshm_info->early_barrier[gasneti_pshm_mynode].val = 1;
    gasneti_waituntil(gasneti_pshm_info->early_barrier[0].val != 0);
  } else {
    for (i = 1; i < gasneti_pshm_nodes; ++i) {
      gasneti_waituntil(gasneti_pshm_info->early_barrier[i].val != 0);
    }
    // We prefault at init time to avoid risk of SIG{BUS,SEGV} later (part of bug 3693).
    // We do so here to avoid colliding with `early_barrier` (the cause of bug 3943).
    gasneti_pshm_prefault(gasneti_pshm_info, info_sz);
    gasneti_atomic_set(&gasneti_pshm_info->bootstrap_barrier_cnt, gasneti_pshm_nodes, 0);
    gasneti_atomic_set(&gasneti_pshm_info->bootstrap_barrier_gen, 0, 0);
    gasneti_local_wmb();
    gasneti_pshm_info->early_barrier[0].val = 1;
  }

  /* Unlink the shared memory file to prevent leaks.
   * "early" barrier above ensures all procs have attached. */
  gasneti_unlink_vnet();

  /* Carve out various allocations, reusing the "early barrier" space. */
  gasneti_pshmnet_bootstrapBarrier();
  {
    uintptr_t addr = (uintptr_t)&gasneti_pshm_info->early_barrier;
    /* gasneti_pshm_firsts, an array of gasneti_nodemap_global_count*sizeof(gex_Rank_t): */
    gasneti_pshm_firsts = (gex_Rank_t *)addr;
    addr += gasneti_nodemap_global_count * sizeof(gex_Rank_t);
    /* optional rankmap: */
    if (discontig) {
      addr = GASNETI_ALIGNUP(addr, sizeof(gasneti_pshm_rank_t));
      gasneti_pshm_rankmap = (gasneti_pshm_rank_t *)addr;
      addr += gasneti_nodes * sizeof(gasneti_pshm_rank_t);
    }
    /* intra-supernode barrier: */
    addr = GASNETI_ALIGNUP(addr, GASNETI_CACHE_LINE_BYTES);
    gasneti_pshm_barrier = (gasneti_pshm_barrier_t *)addr;
    addr += sizeof(gasneti_pshm_barrier_t) +
	    (gasneti_pshm_nodes-1) * sizeof(gasneti_pshm_barrier->node);
  }

  /* Populate gasneti_pshm_firsts[] */
  if (!gasneti_pshm_mynode) gasneti_pshm_firsts[0] = 0;
#if !GASNET_CONDUIT_SMP
  for (i=j=1; i<gasneti_nodes; ++i) {
    if (gasneti_nodemap[i] == i) {
      if (!gasneti_pshm_mynode) gasneti_pshm_firsts[j] = i;
      j += 1;
      gasneti_assert_uint(j ,<=, gasneti_nodemap_global_count);
    }
  }
#endif
#if GASNET_DEBUG
  /* Validate gasneti_pshm_firsts[] */
  if (gasneti_pshm_mynode == 0) {
    for (i=0; i<gasneti_nodemap_global_count; ++i) {
      if (i) gasneti_assert_uint(gasneti_pshm_firsts[i] ,>, gasneti_pshm_firsts[i-1]);
      gasneti_assert_uint(gasneti_nodemap[gasneti_pshm_firsts[i]] ,==, gasneti_pshm_firsts[i]);
    }
  }
#endif

  /* construct rankmap, if any, with first node doing all the work. */
  if (discontig) {
    if (gasneti_pshm_mynode == 0) { /* First node does all the work */
      memset(gasneti_pshm_rankmap, 0xff, gasneti_nodes * sizeof(gasneti_pshm_rank_t));
      for (i = 0; i < gasneti_pshm_nodes; ++i) {
        gasneti_pshm_rankmap[gasneti_nodemap_local[i]] = i;
      }
    }
  }

  /* Collective call to initialize Shared AM "networks" */
  gasneti_request_pshmnet = 
          gasneti_pshmnet_init(gasnetc_pshmnet_region,
                               vnetsz, gasneti_pshm_nodes);
  gasneti_reply_pshmnet =
          gasneti_pshmnet_init((void*)((uintptr_t)gasnetc_pshmnet_region + vnetsz),
                               vnetsz, gasneti_pshm_nodes);

  /* Ensure all peers are initialized before return */
  gasneti_pshmnet_bootstrapBarrier();

  /* Return the conduit's portion, if any */
  return aux_sz ? (void*)((uintptr_t)gasnetc_pshmnet_region +
                          mmapsz - round_up_to_pshmpage(aux_sz))
                : NULL;
}


/*******************************************************************************
 * PSHM global variables:
 ******************************************************************************/
gasneti_pshm_rank_t gasneti_pshm_nodes = 0;
gex_Rank_t gasneti_pshm_firstnode = (gex_Rank_t)(-1);
gasneti_pshm_rank_t gasneti_pshm_mynode = (gasneti_pshm_rank_t)(-1);
/* vectors constructed in shared space: */
gasneti_pshm_rank_t *gasneti_pshm_rankmap = NULL;
gex_Rank_t *gasneti_pshm_firsts = NULL;

/*******************************************************************************
 * "PSHM Net":  message header formats
 * These come early because their sizes influence allocation
 ******************************************************************************/

// NOTE:
// If sizes of these types changes, such that gasneti_pshmnet_allocator_block_t
// also changes size, one must update GASNETC_MAX_MEDIUM_NBRHD_DFLT in gasnetex.h
// to ensure a tight fit in 64KB:
//   sizeof(gasneti_pshmnet_allocator_block_t) == 65536

/* TODO: Could/should we squeeze unused args out of a Medium.*/
/* TODO: Pack category and numargs together (makes assumtion about ranges) */

typedef struct {
  gex_Rank_t source;
  gex_AM_Index_t handler_id;
  uint8_t category;      /* AM msg type: short, med, long */
  uint8_t numargs;
  gex_AM_Arg_t args[GASNETC_MAX_ARGS_NBRHD];
} gasneti_AMPSHM_msg_t;
typedef gasneti_AMPSHM_msg_t gasneti_AMPSHM_shortmsg_t;

typedef struct {
  gasneti_AMPSHM_msg_t msg;
  uint32_t numbytes;
  uint8_t  mediumdata[4 + GASNETC_MAX_MEDIUM_NBRHD]; /* +4 to deal with 4 or 8-byte alignment */
} gasneti_AMPSHM_medmsg_t;

typedef struct {
  gasneti_AMPSHM_msg_t msg;
  uint32_t numbytes;
  void *   longdata;
} gasneti_AMPSHM_longmsg_t;

typedef union {
  gasneti_AMPSHM_shortmsg_t Short;
  gasneti_AMPSHM_medmsg_t   Medium;
  gasneti_AMPSHM_longmsg_t  Long;
} gasneti_AMPSHM_maxmsg_t;

/* atomic operations on queue tail */
#if defined(GASNETI_HAVE_ATOMIC_CAS)
  typedef gasneti_atomic_t gasneti_pshmnet_tail_t;
  #define gasneti_pshmnet_tail_init(_t)\
                  gasneti_atomic_set((_t),0,0)
  #define gasneti_pshmnet_tail_cas(_t,_o,_n)\
                  gasneti_atomic_compare_and_swap((_t),(_o),(_n),0)
  #define gasneti_pshmnet_tail_swap(_t,_v)\
                  gasneti_atomic_swap((_t),(_v),GASNETI_ATOMIC_REL)
#elif defined(GASNETI_HAVE_ATOMIC_ADD_SUB)
  typedef struct {
    gasneti_atomic_t last_ticket;
    gasneti_atomic_t curr_ticket;
    gasneti_atomic_val_t value;
  } gasneti_pshmnet_tail_t;

  GASNETI_INLINE(gasneti_pshmnet_tail_init)
  void gasneti_pshmnet_tail_init(gasneti_pshmnet_tail_t *t) {
    t->value = 0;
    gasneti_atomic_set(&t->last_ticket, 0, 0);
    gasneti_atomic_set(&t->curr_ticket, 1, 0);
  }
  GASNETI_INLINE(gasneti_pshmnet_tail_lock)
  gasneti_atomic_val_t gasneti_pshmnet_tail_lock(gasneti_pshmnet_tail_t *t) {
    const gasneti_atomic_val_t my_ticket = gasneti_atomic_add(&t->last_ticket, 1, 0);
    gasneti_waituntil(my_ticket == gasneti_atomic_read(&t->curr_ticket, 0)); /* includes RMB */
    return my_ticket;
  }
  GASNETI_INLINE(gasneti_pshmnet_tail_unlock)
  void gasneti_pshmnet_tail_unlock(gasneti_pshmnet_tail_t *t, gasneti_atomic_val_t my_ticket) {
    gasneti_atomic_set(&t->curr_ticket, my_ticket+1, GASNETI_ATOMIC_REL);
  }
  GASNETI_INLINE(gasneti_pshmnet_tail_cas)
  int gasneti_pshmnet_tail_cas(gasneti_pshmnet_tail_t *t, gasneti_atomic_val_t oldval, gasneti_atomic_val_t newval) {
    gasneti_atomic_val_t my_ticket = gasneti_pshmnet_tail_lock(t);
    int result;
      if_pt (result = (t->value == oldval))
        t->value = newval;
    gasneti_pshmnet_tail_unlock(t, my_ticket);
    return result;
  }
  GASNETI_INLINE(gasneti_pshmnet_tail_swap)
  gasneti_atomic_val_t gasneti_pshmnet_tail_swap(gasneti_pshmnet_tail_t *t, gasneti_atomic_val_t newval) {
    gasneti_atomic_val_t my_ticket = gasneti_pshmnet_tail_lock(t);
    gasneti_atomic_val_t result;
      result = t->value;
      t->value = newval;
    gasneti_pshmnet_tail_unlock(t, my_ticket);
    return result;
  }
#else
  #error "Platform is missing both atomic ADD and atomic CAS"
#endif

/* queue of descriptors for messages received
 *
 * Based on Nemesis queues as documented in
 * D. Buntinas, G. Mercier, and W. Gropp, "Design and Evaluation of Nemesis,
 * a Scalable, Low-Latency, Message-Passing Communication Subsystem",
 * in Proc. CCGRID, 2006, pp.521-530.
 */
typedef struct gasneti_pshmnet_queue {
  /* Producers' cache line: */
  gasneti_pshmnet_tail_t tail;
  volatile gasneti_atomic_val_t head;
  char _pad0[GASNETI_CACHE_PAD(sizeof(gasneti_pshmnet_tail_t)
                             + sizeof(gasneti_atomic_val_t))];
  /* Consumers' cache line: */
  volatile gasneti_atomic_val_t shead; /* shadow head */
  char _pad1[GASNETI_CACHE_PAD(sizeof(gasneti_atomic_val_t))];
} gasneti_pshmnet_queue_t;

struct gasneti_pshmnet_allocator;  /* forward definition */

/* message payload metadata */
typedef struct gasneti_pshmnet_payload {
  volatile gasneti_atomic_val_t next;
  struct gasneti_pshmnet_allocator *allocator;
  gasneti_pshm_rank_t from;
  size_t len;
  gasneti_AMPSHM_maxmsg_t data;
} gasneti_pshmnet_payload_t;

/******************************************************************************
 * Payload memory allocator interface.
 *
 * Keep the payload allocator interface clean, so we can change algorithms.
 ******************************************************************************/

/* Memory layout of allocator.
 *
 * Logically, we have this layout:
 *  
 *    1) Atomic used by allocator as 'in use' bit
 *    2) A gasneti_pshmnet_payload_t, used by pshmnet
 *
 * Allocator returns #2 to the caller
 * NOTE: placement of #1 ensures no reference to offset==0 is ever used
 */ 

typedef struct {
  gasneti_atomic_t in_use;
  gasneti_pshmnet_payload_t payload;
} gasneti_pshmnet_allocator_block_t;

#define GASNETI_PSHMNET_ALLOC_MAXSZ \
    round_up_to_pshmpage(sizeof(gasneti_pshmnet_allocator_block_t))
#define GASNETI_PSHMNET_ALLOC_MAXPG (GASNETI_PSHMNET_ALLOC_MAXSZ >> GASNETI_PSHMNET_PAGESHIFT)

#define GASNETI_PSHMNET_MAX_PAYLOAD \
    (GASNETI_PSHMNET_ALLOC_MAXSZ - offsetof(gasneti_pshmnet_allocator_block_t, payload.data))

#define GASNETI_PSHMNET_MIN_PAYLOAD \
    (GASNETI_PSHMNET_PAGESIZE - offsetof(gasneti_pshmnet_allocator_block_t, payload.data))

size_t gasneti_pshmnet_max_payload(void) {
  return GASNETI_PSHMNET_MAX_PAYLOAD;
}

/* This implementation uses a circular queue of variable-sized payloads.
 * This data structure lives entirely in private memory.
 *
 * The allocator's per-block metadata is external to the blocks, and is
 * stored in the "length" array.  There is a length entry for each "page",
 * but only the entries corresponding to the first page of each block have
 * defined values.  The "length[]", "next" and "count" are all in units
 * of GASNETI_PSHMNET_PAGESIZE (which must be a power-of-2).
 */
typedef struct gasneti_pshmnet_allocator {
  void *region;
  unsigned int next;
  unsigned int count;
  unsigned int length[1]; /* Variable length */
} gasneti_pshmnet_allocator_t;

/* WARNING: the amount requested from this allocator must be less than 
 * or equal to GASNETI_PSHMNET_MAX_PAYLOAD (which is at least as large
 * as sizeof(gasneti_pshmnet_payload_t)).
 * - returns NULL if no memory available
 */
static gasneti_pshmnet_allocator_t *gasneti_pshmnet_init_allocator(void *region, size_t len);
static gasneti_pshmnet_payload_t * gasneti_pshmnet_alloc(gasneti_pshmnet_allocator_t *a, size_t nbytes);
/* Frees memory.  Note that this must be callable by a different node */
static void gasneti_pshmnet_free(gasneti_pshmnet_payload_t *p);

/******************************************************************************
 * </Payload memory allocator interface>
 ******************************************************************************/

/* Per node view of 'network' of queues in supernode 
 * This struct is stored in private memory. 
 */
struct gasneti_pshmnet {
  gasneti_pshm_rank_t nodecount;    /* nodes in supernode */
  gasneti_pshmnet_queue_t *queues;  /* array of queue heads */
  gasneti_pshmnet_queue_t *my_queue;
  /* only need to see one's own allocator */
  gasneti_pshmnet_allocator_t *my_allocator;
#if GASNET_PAR || GASNETI_CONDUIT_THREADS
  gasneti_mutex_t dequeue_lock; /* serializes dequeue operations */
#endif
  gasneti_mutex_t alloc_lock;   /* serializes buffer allocation */
};

#define gasneti_assert_align(p, align) \
        gasneti_assert((((uintptr_t)p) % align) == 0)

/* Macros for determining the offset and the real address, used for
 * the addresses inside the pshmnet region */
#define gasneti_pshm_offset(addr) \
               (gasneti_assert(addr != gasnetc_pshmnet_region), \
                (gasneti_atomic_val_t)((uintptr_t)(addr) - (uintptr_t)gasnetc_pshmnet_region))
#if PLATFORM_COMPILER_PGI && PLATFORM_COMPILER_VERSION_LT(10,0,0)
/* PGI 9.0-0 truncates the macro version to 32-bits! (older than 9.0 not tested) */
GASNETI_INLINE(gasneti_pshm_addr)
void * gasneti_pshm_addr(uintptr_t offset) {
  gasneti_assert(offset);
  return (void*)(offset + (uintptr_t)gasnetc_pshmnet_region);
}
#else
#define gasneti_pshm_addr(offset) \
               (gasneti_assert(offset), \
                (void *)((uintptr_t)(offset) + (uintptr_t)gasnetc_pshmnet_region))
#endif


static uintptr_t get_queue_mem(int nodes) 
{
  /* Need enough to send the full queue depth at max size each */
  size_t pernode;

  gasneti_pshmnet_network_depth =
          gasneti_getenv_int_withdefault("GASNET_PSHM_NETWORK_DEPTH", 
                                         GASNETI_PSHM_NETWORK_DEPTH_DEFAULT, 0);
  
  if (gasneti_pshmnet_network_depth < GASNETI_PSHM_NETWORK_DEPTH_MIN) {
    fprintf(stderr, "WARNING: GASNET_PSHM_NETWORK_DEPTH (%lu) less than min: using %lu\n",
            gasneti_pshmnet_network_depth, GASNETI_PSHM_NETWORK_DEPTH_MIN);
    gasneti_pshmnet_network_depth = GASNETI_PSHM_NETWORK_DEPTH_MIN;
  } else if (gasneti_pshmnet_network_depth > GASNETI_PSHM_NETWORK_DEPTH_MAX) {
    fprintf(stderr, "WARNING: GASNET_PSHM_NETWORK_DEPTH (%lu) greater than max: using %lu\n",
            gasneti_pshmnet_network_depth, GASNETI_PSHM_NETWORK_DEPTH_MAX);
    gasneti_pshmnet_network_depth = GASNETI_PSHM_NETWORK_DEPTH_MAX;
  }

  pernode = GASNETI_PSHMNET_ALLOC_MAXSZ * gasneti_pshmnet_network_depth;
  gasneti_assert(pernode > 0);

  /* round up to multiple of allocator page size */
  return round_up_to_pshmpage(pernode);
}

static size_t gasneti_pshmnet_memory_needed_pernode(gasneti_pshm_rank_t nodes)
{
  /* Space for the message payloads */
  if_pf (!gasneti_pshmnet_queue_mem) {
    gasneti_pshmnet_queue_mem = get_queue_mem(nodes);
  }
  return round_up_to_pshmpage(gasneti_pshmnet_queue_mem);
}

static size_t gasneti_pshmnet_memory_needed_once(gasneti_pshm_rank_t nodes)
{
  /* Space for the queue headers */
  return round_up_to_pshmpage(nodes * sizeof(gasneti_pshmnet_queue_t));
}

size_t gasneti_pshmnet_memory_needed(gasneti_pshm_rank_t nodes)
{
  size_t pernode = gasneti_pshmnet_memory_needed_pernode(nodes);
  size_t once = gasneti_pshmnet_memory_needed_once(nodes);
  return once + nodes * pernode;
}

/* Initializes the pshmnet region. Called from each node twice: 
 * to initialize pshmnet_request and pshmnet_reply */
gasneti_pshmnet_t *
gasneti_pshmnet_init(void *region, size_t regionlen, gasneti_pshm_rank_t pshmnodes)
{
  gasneti_pshmnet_t *vnet;
  size_t szonce, szpernode;
  void *myregion;

  /* make sure that our max buffer size fits all possible AMs */
  gasneti_assert(sizeof(gasneti_AMPSHM_maxmsg_t) <= GASNETI_PSHMNET_MAX_PAYLOAD);

  gasneti_assert((offsetof(gasneti_AMPSHM_medmsg_t, mediumdata) % 4) == 0);

  // Check GASNETC_MAX_MEDIUM_NBRHD_DFLT (if used) provides tight fit
  #if (PLATFORM_OS_LINUX || PLATFORM_OS_DARWIN) && \
      (PLATFORM_ARCH_X86 || PLATFORM_ARCH_X86_64)
    // Arbitrary choice of frequently-tested ABIs known to provide tight fit
    gasneti_assert((GASNETC_MAX_MEDIUM_NBRHD != GASNETC_MAX_MEDIUM_NBRHD_DFLT) || \
                   (sizeof(gasneti_pshmnet_allocator_block_t) == 65536));
  #else
    // Other ABIs may have less restrictive alignments (allow 16-byte slack)
    gasneti_assert((GASNETC_MAX_MEDIUM_NBRHD != GASNETC_MAX_MEDIUM_NBRHD_DFLT) || \
                   ((sizeof(gasneti_pshmnet_allocator_block_t) <= 65536) && \
                    (sizeof(gasneti_pshmnet_allocator_block_t) >= 65536 - 16)));
  #endif

  szpernode = gasneti_pshmnet_memory_needed_pernode(pshmnodes);
  szonce = gasneti_pshmnet_memory_needed_once(pshmnodes);

  if (regionlen < (szonce + szpernode * pshmnodes))
    gasneti_fatalerror("Internal error: not enough memory for pshmnet: \n"
                       " given %"PRIuPTR" effective bytes, but need %"PRIuPTR, 
                       (uintptr_t)regionlen, (uintptr_t)(szonce + szpernode * pshmnodes));

  vnet = gasneti_malloc(sizeof(gasneti_pshmnet_t));
  vnet->nodecount = pshmnodes;
#if GASNET_PAR || GASNETI_CONDUIT_THREADS
  gasneti_mutex_init(&vnet->dequeue_lock);
#endif
  gasneti_mutex_init(&vnet->alloc_lock);

  /* initialize my own allocator */
  myregion = (void *)((uintptr_t)region + (szpernode * gasneti_pshm_mynode));
  gasneti_assert_align(myregion, GASNETI_PSHMNET_PAGESIZE);
  vnet->my_allocator = gasneti_pshmnet_init_allocator(myregion, gasneti_pshmnet_queue_mem);

  /* initialize my own queue header */
  vnet->queues = (gasneti_pshmnet_queue_t*)((uintptr_t)region + szpernode * pshmnodes);
  gasneti_assert_align(vnet->queues, GASNETI_PSHMNET_PAGESIZE);
  vnet->my_queue = &vnet->queues[gasneti_pshm_mynode];
  vnet->my_queue->head = 0;
  vnet->my_queue->shead = 0;
  gasneti_pshmnet_tail_init(&vnet->my_queue->tail);

  gasneti_leak(vnet);
  return vnet;
}


void * gasneti_pshmnet_get_send_buffer(gasneti_pshmnet_t *vnet, size_t nbytes, 
                                       gasneti_pshm_rank_t target /* currently unused */)
{
  gasneti_pshmnet_payload_t *p;
  void *retval = NULL;
  
  gasneti_assert_uint(nbytes ,<=, GASNETI_PSHMNET_MAX_PAYLOAD);

  p = gasneti_pshmnet_alloc(vnet->my_allocator, nbytes);
  if (p != NULL) {
    p->next = 0;
    p->from = gasneti_pshm_mynode;
    p->allocator = vnet->my_allocator;
    retval = &p->data;
  }
  
  return retval;
}

void gasneti_pshmnet_deliver_send_buffer(gasneti_pshmnet_t *vnet, void *buf, 
                                         size_t nbytes, gasneti_pshm_rank_t target)
{
  gasneti_pshmnet_payload_t *p =
          pshmnet_get_struct_addr_from_field_addr(gasneti_pshmnet_payload_t, data, buf);
  gasneti_pshmnet_queue_t *q = &vnet->queues[target];
  gasneti_atomic_val_t my_offset = gasneti_pshm_offset(p);
  gasneti_atomic_val_t prev_offset;

  p->len = nbytes;

  /* Nemesis enqueue: */
  prev_offset = gasneti_pshmnet_tail_swap(&q->tail, my_offset);
  if (prev_offset) {
    gasneti_pshmnet_payload_t *prev = gasneti_pshm_addr(prev_offset);
    prev->next = my_offset;
  } else {
    q->head = my_offset;
  }
}

GASNETI_INLINE(gasneti_pshmnet_queue_peek)
int gasneti_pshmnet_queue_peek(const gasneti_pshmnet_queue_t * const q)
{
  return q->shead || q->head;
}

int gasneti_pshmnet_recv(gasneti_pshmnet_t *vnet, void **pbuf, size_t *psize, 
                         gasneti_pshm_rank_t *pfrom)
{
  gasneti_atomic_val_t head;
  gasneti_pshmnet_payload_t *p = NULL;
  gasneti_pshmnet_queue_t *q = vnet->my_queue;

#if GASNET_PAR || GASNETI_CONDUIT_THREADS
  if (gasneti_pshmnet_queue_peek(q)) {
    gasneti_mutex_lock(&vnet->dequeue_lock);
#endif
    /* Nemesis dequeue: */
    head = q->shead;
    if (!head && q->head) {
      head = q->shead = q->head;
      q->head = 0;
    }
    if_pt (head) {
      register gasneti_atomic_val_t next;
      p = gasneti_pshm_addr(head);
      gasneti_local_rmb(); /* ACQ */
      /* NOTE: Unlike in the Nemesis paper, we loop on *both* p->next and
       * cas(tail) to allow weaker memory models which may reorder their
       * respective reads/writes.  This is preferred over adding any memory
       * fence(s) to the race-free case.
       */
      while (GASNETT_PREDICT_FALSE(0 == (next = p->next)) &&
             GASNETT_PREDICT_FALSE(!gasneti_pshmnet_tail_cas(&q->tail, head, 0))) {
        GASNETI_WAITHOOK(); /* waituntil() has excess RMB */
      }
      q->shead = next;
    }
#if GASNET_PAR || GASNETI_CONDUIT_THREADS
    gasneti_mutex_unlock(&vnet->dequeue_lock);
  }
#endif

  if (p) {
    *pbuf  = &p->data;
    *psize = p->len;
    *pfrom = p->from;
  }
  
  return !p;
}


/* Note the current behavior if a user forgets to call this function is
 * NASTY--the message stays marked as state==BUSY, which will cause
 * senders to think the queue is full.  This could cause
 * deadlock and/or lots of confusion (for me it was the latter).
 */
void gasneti_pshmnet_recv_release(gasneti_pshmnet_t *vnet, void *buf)
{
  /* Address we handed out was the addr of the 'data' field */
  gasneti_pshmnet_payload_t *p = 
    pshmnet_get_struct_addr_from_field_addr(gasneti_pshmnet_payload_t,
                                            data, buf);
  gasneti_pshmnet_free(p);
}


/******************************************************************************
 * PSHMnet bootstrap barrier
 * - TODO: only good a finite number of times before it wraps!
 ******************************************************************************/
static void do_pshmnet_barrier(int do_poll)
{
  static gasneti_atomic_val_t generation = 0;
  gasneti_atomic_val_t curr, target;

  gasneti_assert(gasneti_pshm_info != NULL);
  gasneti_assert(gasneti_pshm_nodes > 0);

#if GASNET_DEBUG
  curr = gasneti_atomic_read(&gasneti_pshm_info->bootstrap_barrier_gen, 0);
  gasneti_assert((curr == generation) || (curr >= GASNETI_PSHM_BSB_LIMIT));
#endif

  if (gasneti_atomic_decrement_and_test(&gasneti_pshm_info->bootstrap_barrier_cnt, 0)) {
    gasneti_atomic_set(&gasneti_pshm_info->bootstrap_barrier_cnt, gasneti_pshm_nodes, 0);
    gasneti_atomic_increment(&gasneti_pshm_info->bootstrap_barrier_gen, GASNETI_ATOMIC_REL);
  }

  target = generation + 1;
  gasneti_assert_always(target < GASNETI_PSHM_BSB_LIMIT); /* Die if we were ever to reach the limit */

  if (do_poll) {
    gasneti_pollwhile((curr = gasneti_atomic_read(&gasneti_pshm_info->bootstrap_barrier_gen, 0)) == generation);
  } else {
    gasneti_waitwhile((curr = gasneti_atomic_read(&gasneti_pshm_info->bootstrap_barrier_gen, 0)) == generation);
  }
  gasneti_assert_uint(curr ,==, target);
  if_pf (curr >= GASNETI_PSHM_BSB_LIMIT) {
    if (gasnetc_pshm_abort_callback) gasnetc_pshm_abort_callback();
    gasneti_fatalerror("PSHM bootstrap barrier exceeded GASNETI_PSHM_BSB_LIMIT");
  }

  generation = target;

  if (do_poll) {
    // Ensuring all AMPoll() calls cease before resuming non-AM traffic
    do_pshmnet_barrier(0);
  }
}
void gasneti_pshmnet_bootstrapBarrier(void)     { do_pshmnet_barrier(0); }
void gasneti_pshmnet_bootstrapBarrierPoll(void) { do_pshmnet_barrier(1); }

/******************************************************************************
 * "critical sections" in which we notify peers if we abort() while
 * they are potentially blocked in gasneti_pshmnet_bootstrapBarrier().
 * These DO NOT nest (but there is no checking to ensure that).
 ******************************************************************************/
static struct {
  int sig;
  gasneti_sighandlerfn_t old_hand;
} gasneti_pshm_catch_signals[] = {
#ifdef SIGABRT
  { SIGABRT, NULL },
#endif
#ifdef SIGILL
  { SIGILL,  NULL },
#endif
#ifdef SIGSEGV
  { SIGSEGV, NULL },
#endif
#ifdef SIGBUS
  { SIGBUS,  NULL },
#endif
#ifdef SIGFPE
  { SIGFPE,  NULL },
#endif
#ifdef SIGINT
  { SIGINT,  NULL },
#endif
#ifdef SIGTERM
  { SIGTERM, NULL },
#endif
#ifdef SIGQUIT
  { SIGQUIT, NULL },
#endif
#ifdef SIGPIPE
  { SIGPIPE, NULL },
#endif
#ifdef SIGHUP
  { SIGHUP,  NULL },
#endif
  { 0, NULL }
};

static void gasneti_pshm_abort_handler(int sig) {
  // Invoke callback, if any
  if (gasnetc_pshm_abort_callback) gasnetc_pshm_abort_callback();

  // Force others to exit from barrier:
  gasneti_atomic_set(&gasneti_pshm_info->bootstrap_barrier_gen, GASNETI_PSHM_BSB_LIMIT, 0);

  // Best-effort message if this is not due to gasneti_fatalerror()
  if (sig != SIGABRT) {
    const char msg1[] = "*** FATAL ERROR: fatal ";
    const char msg2[] = " while mapping shared memory\n";
    const char *signame = gasnett_signame_fromval(sig);
    if (!signame) signame = "signal";
    char msg[128] = { '\0', };
    gasneti_assert(strlen(msg1) + strlen(signame) + strlen(msg2) + 1 <= sizeof(msg));
    strcat(strcat(strcat(msg, msg1), signame), msg2);
    int ignore = write(STDERR_FILENO, msg, strlen(msg));
  }

  // Reraise the signal
  for (int i = 0; gasneti_pshm_catch_signals[i].sig; ++i) {
    if (gasneti_pshm_catch_signals[i].sig != sig) continue;
    gasneti_reghandler(sig, gasneti_pshm_catch_signals[i].old_hand);
    break;
  }
#if HAVE_SIGPROCMASK /* Is this ever NOT the case? */
  { sigset_t new_set, old_set;
    sigemptyset(&new_set);
    sigaddset(&new_set, sig);
    sigprocmask(SIG_UNBLOCK, &new_set, &old_set);
  }
#endif
  gasneti_raise(sig);
}

void gasneti_pshm_cs_enter(void (*callback)(void))
{
  gasnetc_pshm_abort_callback = callback;
  for (int i = 0; gasneti_pshm_catch_signals[i].sig; ++i) {
    gasneti_pshm_catch_signals[i].old_hand =
      gasneti_reghandler(gasneti_pshm_catch_signals[i].sig,
                         &gasneti_pshm_abort_handler);
  }
}

void gasneti_pshm_cs_leave(void)
{
  gasnetc_pshm_abort_callback = NULL;
  for (int i = 0; gasneti_pshm_catch_signals[i].sig; ++i) {
    gasneti_reghandler(gasneti_pshm_catch_signals[i].sig,
                       gasneti_pshm_catch_signals[i].old_hand);
  }
}

/******************************************************************************
 * Helpers for PSHMnet bootstrap broadcast and exchange
 ******************************************************************************/

/* Sends data to all peers excluding self */
static void gasneti_pshmnet_coll_send(gasneti_pshmnet_t *vnet, void *src, size_t len)
{
  gasneti_pshm_rank_t i;
  void *msg;

  for (i = 0; i < vnet->nodecount; i++) {
    if (i == gasneti_pshm_mynode) continue;
    gasneti_waitwhile (NULL == (msg = gasneti_pshmnet_get_send_buffer(vnet, len, i)));
    memcpy(msg, src, len);
    gasneti_pshmnet_deliver_send_buffer(vnet, msg, len, i);
  }
}

/* Recive data from any peer excluding self, placing data according to srcidx*stride */
static void gasneti_pshmnet_coll_recv(gasneti_pshmnet_t *vnet, size_t stride, void *dest)
{
  gasneti_pshm_rank_t from;
  void *msg, *dest_elem;
  size_t len;

  gasneti_waitwhile (gasneti_pshmnet_recv(vnet, &msg, &len, &from));
  dest_elem = (void*)((uintptr_t)dest + (stride * from));
  memcpy(dest_elem, msg, len);
  gasneti_pshmnet_recv_release(vnet, msg);
}

/******************************************************************************
 * PSHMnet bootstrap broadcast
 * - Rootpshmnode is supernode-local rank
 * - Barriers ensure ordering w.r.t sends that may follow
 ******************************************************************************/
void gasneti_pshmnet_bootstrapBroadcast(gasneti_pshmnet_t *vnet, void *src, 
                                        size_t len, void *dst, int rootpshmnode)
{
  uintptr_t src_addr = (uintptr_t)src;
  uintptr_t dst_addr = (uintptr_t)dst;
  size_t remain = len;

  gasneti_assert(vnet != NULL);
  gasneti_assert_uint(vnet->nodecount ,==, gasneti_pshm_nodes);

  while (remain) {
    size_t nbytes = MIN(remain, GASNETI_PSHMNET_MAX_PAYLOAD);

    if (gasneti_pshm_mynode == rootpshmnode) {
      gasneti_pshmnet_coll_send(vnet, (void*)src_addr, nbytes);
    } else {
      gasneti_pshmnet_coll_recv(vnet, 0, (void*)dst_addr);
    }

    src_addr += nbytes;
    dst_addr += nbytes;
    remain -= nbytes;

    gasneti_pshmnet_bootstrapBarrier();
  }
  if (gasneti_pshm_mynode == rootpshmnode) {
    memmove(dst, src, len);
  }
}

/******************************************************************************
 * PSHMnet bootstrap exchange
 * - Barriers ensure ordering w.r.t sends that may follow
 ******************************************************************************/
void gasneti_pshmnet_bootstrapExchange(gasneti_pshmnet_t *vnet, void *src, 
                                       size_t len, void *dst)
{
  uintptr_t src_addr = (uintptr_t)src;
  uintptr_t dst_addr = (uintptr_t)dst;
  size_t remain = len;

  gasneti_assert(vnet != NULL);
  gasneti_assert_uint(vnet->nodecount ,==, gasneti_pshm_nodes);

  /* All nodes broadcast their contribution in turn */
  while (remain) {
    size_t nbytes = MIN(remain, GASNETI_PSHMNET_MAX_PAYLOAD);
    gasneti_pshm_rank_t i;

    for (i = 0; i < vnet->nodecount; i++) {
      if (gasneti_pshm_mynode == i) {
        gasneti_pshmnet_coll_send(vnet, (void*)src_addr, nbytes);
      } else {
        gasneti_pshmnet_coll_recv(vnet, len, (void*)dst_addr);
      }
    }

    src_addr += nbytes;
    dst_addr += nbytes;
    remain -= nbytes;

    gasneti_pshmnet_bootstrapBarrier();
  }
  memmove((void*)((uintptr_t)dst + (gasneti_pshm_mynode*len)), src, len);
}

/******************************************************************************
 * PSHMnet bootstrap gather
 * - Barriers ensure ordering w.r.t sends that may follow
 ******************************************************************************/
void gasneti_pshmnet_bootstrapGather(gasneti_pshmnet_t *vnet, void *src, 
                                     size_t len, void *dst, int rootpshmnode)
{
  uintptr_t src_addr = (uintptr_t)src;
  uintptr_t dst_addr = (uintptr_t)dst;
  size_t remain = len;
  void *msg;

  gasneti_assert(vnet != NULL);
  gasneti_assert_uint(vnet->nodecount ,==, gasneti_pshm_nodes);

  /* All nodes send their contribution in chunks */
  while (remain) {
    size_t nbytes = MIN(remain, GASNETI_PSHMNET_MAX_PAYLOAD);

    if (gasneti_pshm_mynode == rootpshmnode) {
      gasneti_pshm_rank_t i;
      for (i = 0; i < vnet->nodecount - 1; i++) {
        gasneti_pshm_rank_t msg_from;
        size_t msg_len;

        gasneti_waitwhile (gasneti_pshmnet_recv(vnet, &msg, &msg_len, &msg_from));
        gasneti_assert_uint(msg_len ,==, nbytes);
        memcpy((void*)(dst_addr + (len * msg_from)), msg, msg_len);
        gasneti_pshmnet_recv_release(vnet, msg);
      }
    } else {
      gasneti_waitwhile (NULL == (msg = gasneti_pshmnet_get_send_buffer(vnet, nbytes, rootpshmnode)));
      memcpy(msg, (void*)src_addr, nbytes);
      gasneti_pshmnet_deliver_send_buffer(vnet, msg, nbytes, rootpshmnode);
    }

    src_addr += nbytes;
    dst_addr += nbytes;
    remain -= nbytes;

    gasneti_pshmnet_bootstrapBarrier(); 
  }
  if (gasneti_pshm_mynode == rootpshmnode) {
    memmove((void*)((uintptr_t)dst + (gasneti_pshm_mynode*len)), src, len);
  }
}

/******************************************************************************
 * Allocator implementation
 ******************************************************************************/

static gasneti_pshmnet_allocator_t *gasneti_pshmnet_init_allocator(void *region, size_t len)
{
  int count = len >> GASNETI_PSHMNET_PAGESHIFT;
  gasneti_pshmnet_allocator_block_t *tmp;

  /* This implementation doesn't need to put allocator within shared memory.
   * If a later one does, consider increasing the size returned by
   * get_queue_mem()
   */
  gasneti_pshmnet_allocator_t *a = gasneti_malloc(sizeof(gasneti_pshmnet_allocator_t) +
                                                  (count-1) * sizeof(unsigned int));
  gasneti_leak(a);

  /* make sure we've arranged for page alignment */
  gasneti_assert_align(GASNETI_PSHMNET_ALLOC_MAXSZ, GASNETI_PSHMNET_PAGESIZE);
  gasneti_assert_align(region, GASNETI_PSHMNET_PAGESIZE);

  /* Ensure affinity to writer and prevent later starvation */
  gasneti_pshm_prefault(region, len);

  /* Initial state is one large free block */
  a->next = 0;
  a->count = count;
  a->length[0] = count;
  a->region = tmp = region;
  gasneti_atomic_set(&tmp->in_use, 0, 0);

  return a;
}


/* Basic page-granular first-fit allocator
   This allocator is NOT thread-safe - the callers are responsible for serialization. */
static gasneti_pshmnet_payload_t *
gasneti_pshmnet_alloc(gasneti_pshmnet_allocator_t *a, size_t nbytes)
{
  void *retval = NULL;
  unsigned int curr;
  unsigned int needed;
  int remain;
  void *region = a->region;

  nbytes += offsetof(gasneti_pshmnet_allocator_block_t, payload.data);
  gasneti_assert_uint(nbytes ,<=, GASNETI_PSHMNET_ALLOC_MAXSZ);

  needed = (nbytes + GASNETI_PSHMNET_PAGESIZE - 1) >> GASNETI_PSHMNET_PAGESHIFT;
  gasneti_assert_uint(needed ,<=, GASNETI_PSHMNET_ALLOC_MAXPG);

  curr = a->next;
  remain = a->count;
  do {
    unsigned int length = a->length[curr];
    gasneti_pshmnet_allocator_block_t *next_block;
    gasneti_pshmnet_allocator_block_t * const block = 
            (gasneti_pshmnet_allocator_block_t*)
                      ((uintptr_t)region + (curr << GASNETI_PSHMNET_PAGESHIFT));

    if (!gasneti_atomic_read(&block->in_use, GASNETI_ATOMIC_ACQ)) {
      while (length < needed) {
        unsigned int next = curr + length;
        gasneti_assert_uint(next ,<=, a->count);
        if (next == a->count) break; /* hit end of region */
        next_block = (gasneti_pshmnet_allocator_block_t*)
                     ((uintptr_t)block + (length << GASNETI_PSHMNET_PAGESHIFT));
        if (gasneti_atomic_read(&next_block->in_use, GASNETI_ATOMIC_ACQ)) break; /* hit busy block */
        length += a->length[next];
      }

      if (length >= needed) {
        unsigned int next = curr + needed;

        if (length > needed) { /* Split it */
          gasneti_assert_uint(next ,<, a->count);
          a->length[next] = length - needed;
          next_block = (gasneti_pshmnet_allocator_block_t*)
                       ((uintptr_t)block + (needed << GASNETI_PSHMNET_PAGESHIFT));
          gasneti_atomic_set(&next_block->in_use, 0, 0);
        }

        a->length[curr] = needed;
        gasneti_atomic_set(&block->in_use, 1, 0);
        retval = &block->payload;

        if ((curr =next) == a->count) curr = 0;
        break; /* do {} while (remain); */
      }

      /* Assume write is cheaper than branching again when length is unchanged */
      a->length[curr] = length;
    }

    remain -= length;
    if ((curr += length) == a->count) curr = 0;
  } while (remain > 0); /* could be negative if merging took us past our starting point */
  a->next = curr;
  return retval;
}

static void gasneti_pshmnet_free(gasneti_pshmnet_payload_t *p)
{
  /* Address we handed out was the addr of the 'payload' field */
  gasneti_pshmnet_allocator_block_t *block = 
      pshmnet_get_struct_addr_from_field_addr(gasneti_pshmnet_allocator_block_t,
                                              payload, p);
  gasneti_assert_ptr(p ,==, &block->payload);
  /* assert block is page-aligned */
  gasneti_assert_uint( (((uintptr_t)block) % GASNETI_PSHMNET_PAGESIZE) ,==, 0);

  gasneti_atomic_set(&block->in_use, 0, GASNETI_ATOMIC_REL);
}

/******************************************************************************
 * AMPSHM:  Active Message API over PSHMnet
 ******************************************************************************/

/* The mediumdata field may not be aligned */
#define GASNETI_AMPSHM_MSG_MEDDATA_OFFSET \
   (offsetof(gasneti_pshmnet_allocator_block_t, payload.data.Medium.mediumdata)&7)
#define GASNETI_AMPSHM_MSG_MEDDATA_SHIFT \
   (GASNETI_AMPSHM_MSG_MEDDATA_OFFSET?(8-GASNETI_AMPSHM_MSG_MEDDATA_OFFSET):0)

#define GASNETI_AMPSHM_MSG_CATEGORY(msg)      (((gasneti_AMPSHM_msg_t*)msg)->category)
#define GASNETI_AMPSHM_MSG_HANDLERID(msg)     (((gasneti_AMPSHM_msg_t*)msg)->handler_id)
#define GASNETI_AMPSHM_MSG_NUMARGS(msg)       (((gasneti_AMPSHM_msg_t*)msg)->numargs)
#define GASNETI_AMPSHM_MSG_SOURCE(msg)        (((gasneti_AMPSHM_msg_t*)msg)->source)
#define GASNETI_AMPSHM_MSG_ARGS(msg)          (((gasneti_AMPSHM_msg_t*)msg)->args)
#define GASNETI_AMPSHM_MSG_MED_NUMBYTES(msg)  (((gasneti_AMPSHM_medmsg_t*)msg)->numbytes)
#define GASNETI_AMPSHM_MSG_MED_DATA(msg)      ((((gasneti_AMPSHM_medmsg_t*)msg)->mediumdata) + \
                                               GASNETI_AMPSHM_MSG_MEDDATA_SHIFT)
#define GASNETI_AMPSHM_MSG_LONG_NUMBYTES(msg) (((gasneti_AMPSHM_longmsg_t*)msg)->numbytes)
#define GASNETI_AMPSHM_MSG_LONG_DATA(msg)     (((gasneti_AMPSHM_longmsg_t*)msg)->longdata)

// Free space after the long header, GASNETI_MEDBUF_ALIGNMENT aligned for use by NP-AM Longs
// Note "GASNETI_MEDBUF_ALIGNMENT-4" allows for either 4- or 8-byte natural alignment.
#define GASNETI_AMPSHM_MSG_LONG_INLINE        (GASNETI_PSHMNET_MIN_PAYLOAD -       \
                                               (sizeof(gasneti_AMPSHM_longmsg_t) + \
                                                (GASNETI_MEDBUF_ALIGNMENT - 4)))
#define GASNETI_AMPSHM_MSG_LONG_TMP(msg)      ((uint8_t*)                                          \
                                               GASNETI_ALIGNUP((gasneti_AMPSHM_longmsg_t*)msg + 1, \
                                                               GASNETI_MEDBUF_ALIGNMENT))

#define GASNETI_AMPSHM_MAX_REPLY_PER_POLL 10
#define GASNETI_AMPSHM_MAX_REQUEST_PER_POLL 10

/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasneti_AMPSHM_service_incoming_msg)
int gasneti_AMPSHM_service_incoming_msg(gasneti_pshmnet_t *vnet, int isReq GASNETI_THREAD_FARG)
{
  void *msg;
  size_t msgsz;
  gasneti_pshm_rank_t from;
  gasneti_category_t category;
  gex_AM_Index_t handler_id;
  gex_AM_Fn_t handler_fn;
  int numargs;
  gex_AM_Arg_t *args;

  gasneti_assert(vnet != NULL);

  if (gasneti_pshmnet_recv(vnet, &msg, &msgsz, &from))
    return -1;

  handler_id = GASNETI_AMPSHM_MSG_HANDLERID(msg);
  category = (gasneti_category_t) GASNETI_AMPSHM_MSG_CATEGORY(msg);
  gasneti_assert((category == gasneti_Short) || 
                 (category == gasneti_Medium) || 
                 (category == gasneti_Long));
  gex_EP_t ep = NULL; // TODO-EX: get true value
  gex_AM_Entry_t *entry = gasnetc_get_hentry(ep,handler_id);
  gasnetc_nbrhd_token_t my_token;
  gex_Token_t token = gasnetc_nbrhd_token_init(&my_token, GASNETI_AMPSHM_MSG_SOURCE(msg), entry, isReq);
#if GASNETI_THREADINFO_OPT
  my_token.threadinfo = GASNETI_MYTHREAD;
#endif

  handler_fn = entry->gex_fnptr;
  numargs = GASNETI_AMPSHM_MSG_NUMARGS(msg);
  args = GASNETI_AMPSHM_MSG_ARGS(msg);

  gasneti_amtbl_check(entry, numargs, category, isReq);

  switch (category) {
    case gasneti_Short:
      { 
        GASNETC_NBRHD_ENTERING_HANDLER_HOOK(category,isReq,handler_id,token,NULL,0,numargs,args);
        my_token.ti.gex_is_long = 0;
        GASNETI_RUN_HANDLER_SHORT(isReq,handler_id,handler_fn,token,args,numargs);
      }
      break;
    case gasneti_Medium:
      {
        void * data = GASNETI_AMPSHM_MSG_MED_DATA(msg);
        size_t nbytes = GASNETI_AMPSHM_MSG_MED_NUMBYTES(msg);
        GASNETC_NBRHD_ENTERING_HANDLER_HOOK(category,isReq,handler_id,token,data,nbytes,numargs,args);
        my_token.ti.gex_is_long = 0;
        GASNETI_RUN_HANDLER_MEDIUM(
          isReq,handler_id,handler_fn,token,args,numargs,data,nbytes);
      }
      break;
    case gasneti_Long:
      { 
        void * data = GASNETI_AMPSHM_MSG_LONG_DATA(msg);
        size_t nbytes = GASNETI_AMPSHM_MSG_LONG_NUMBYTES(msg);
        GASNETC_NBRHD_ENTERING_HANDLER_HOOK(category,isReq,handler_id,token,data,nbytes,numargs,args);
        my_token.ti.gex_is_long = 1;
        GASNETI_RUN_HANDLER_LONG(
            isReq,handler_id,handler_fn,token,args,numargs,data,nbytes);
      }
      break;
  }
  GASNETC_NBRHD_LEAVING_HANDLER_HOOK(category,isReq);
  gasneti_pshmnet_recv_release(vnet, msg);
  return 0;
}

/* ------------------------------------------------------------------------------------ */
int gasneti_AMPSHMPoll(int repliesOnly GASNETI_THREAD_FARG)
{
  int i = 0;

#if 0
  /* We skip CHECKATTACH to allow "early" internal use by conduits. */
  GASNETI_CHECKATTACH();
#endif

  if (gasneti_pshmnet_queue_peek(gasneti_reply_pshmnet->my_queue)) {
    for (i = 0; i < GASNETI_AMPSHM_MAX_REPLY_PER_POLL; i++) 
      if (gasneti_AMPSHM_service_incoming_msg(gasneti_reply_pshmnet, 0 GASNETI_THREAD_PASS))
        break;
  }
  if (!repliesOnly && gasneti_pshmnet_queue_peek(gasneti_request_pshmnet->my_queue)) {
    for (i = 0; i < GASNETI_AMPSHM_MAX_REQUEST_PER_POLL; i++) 
      if (gasneti_AMPSHM_service_incoming_msg(gasneti_request_pshmnet, 1 GASNETI_THREAD_PASS))
        break;
  }
  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
/*
 * Active Message Request Functions
 * ================================
 */

static void * ampshm_buf_alloc(
        gasneti_pshmnet_t *vnet,
        int category, int isReq, gasneti_pshm_rank_t target,
        size_t nbytes, gex_Flags_t flags GASNETI_THREAD_FARG)
{
    /* calculate size of buffer needed */
    size_t msgsz;
    switch (category) {
      case gasneti_Short:
        msgsz = sizeof(gasneti_AMPSHM_shortmsg_t);
        break;
      case gasneti_Medium:
        msgsz = sizeof(gasneti_AMPSHM_medmsg_t) - (GASNETC_MAX_MEDIUM_NBRHD - nbytes);
        break;
      case gasneti_Long:
        msgsz = sizeof(gasneti_AMPSHM_longmsg_t);
        break;
      default: gasneti_unreachable_error(("Invalid category=%i",(int)category));
    }
    gasneti_assert_uint(msgsz ,<=, sizeof(gasneti_AMPSHM_maxmsg_t));

    /* Get buffer, poll if busy (unless IMMEDIATE)
       Lock serializes allocation so small messages can't starve large ones */
    gasneti_mutex_t *lock = &vnet->alloc_lock;

    void *msg;
    if (flags & GEX_FLAG_IMMEDIATE) {
      if (gasneti_mutex_trylock(lock)) return NULL;
      msg = gasneti_pshmnet_get_send_buffer(vnet, msgsz, target);
      if (!msg) goto out_immediate;
    } else {
      gasneti_mutex_lock(lock);
      while (!(msg = gasneti_pshmnet_get_send_buffer(vnet, msgsz, target))) {
        /* If reply, only poll reply network: avoids deadlock  */
        if (isReq) gasnetc_AMPoll(GASNETI_THREAD_PASS_ALONE); /* No progress functions */
        else gasneti_AMPSHMPoll(1 GASNETI_THREAD_PASS);
        GASNETI_WAITHOOK();
      }
    }
out_immediate:
    gasneti_mutex_unlock(lock);
    return msg;
}

GASNETI_INLINE(gasnetc_ampshm_msgsource)
gex_Rank_t gasnetc_ampshm_msgsource(gex_Token_t token) {
    return ((gasnetc_nbrhd_token_t *)(1^(uintptr_t)token))->ti.gex_srcrank;
}

// After sd, next 3 params (isFixed, isReq, category) will be manifest constants
// which should lead to specialization of the code upon inlining.
GASNETI_INLINE(ampshm_prepare_inner)
int ampshm_prepare_inner(
                   gasneti_AM_SrcDesc_t sd, const int isFixed,
                   const int isReq, const int category,
                   gex_Rank_t jobrank, const void *client_buf,
                   size_t least_payload, size_t most_payload,
                   void *dest_addr, gex_Event_t *lc_opt,
                   gex_Flags_t flags, unsigned int nargs
                   GASNETI_THREAD_FARG)
{
  // Sanity checks:
  gasneti_assert(isFixed || (category == gasneti_Medium) || (category == gasneti_Long));
  gasneti_assert(!dest_addr || (category == gasneti_Long));
  gasneti_assert(sd);

  // Determine PSHM peer
  gasneti_assert(gasneti_pshm_jobrank_in_supernode(jobrank));
  gasneti_pshm_rank_t pshmrank = gasneti_pshm_jobrank_to_local_rank(jobrank);

  // Determine the xfer size
  size_t size;
  int inline_long = 0;
  if (isFixed) {
    size = most_payload;
  } else if (category == gasneti_Medium) {
    size = MIN(most_payload, GASNETC_MAX_MEDIUM_NBRHD);
  } else {
    size = MIN(most_payload, GASNETC_MAX_LONG_NBRHD);
    // For small enough Long use the free space after the header to avoid malloc/free
    inline_long = (size <= GASNETI_AMPSHM_MSG_LONG_INLINE);
  }

  gasneti_assert(sd->_tofree == NULL);  // check this before possible IMMEDIATE failure

  // Allocate our buffer (honoring IMMEDIATE)
  gasneti_pshmnet_t *vnet = (isReq ? gasneti_request_pshmnet : gasneti_reply_pshmnet);
  void *msg = ampshm_buf_alloc(vnet, category, isReq, pshmrank, size, flags GASNETI_THREAD_PASS);
  gasneti_assert(msg || (flags & GEX_FLAG_IMMEDIATE));
  if (!msg) return 1;

  // Outputs consumed by commit
  sd->_void_p = msg;
  sd->_pshm._pshmrank = pshmrank;
  sd->_pshm._jobrank = jobrank;
  GASNETI_AMPSHM_MSG_NUMARGS(msg) = nargs;
  gasneti_assert_uint( GASNETI_AMPSHM_MSG_NUMARGS(msg) ,==, nargs ); // truncated?

  // Outputs for the client
  sd->_size = size;
  if (isFixed) {
    sd->_addr = (/*non-const*/void *)client_buf;
  } else if (client_buf) {
    sd->_addr = (/*non-const*/void *)client_buf;
    gasneti_leaf_finish(lc_opt);
  } else if (category == gasneti_Medium) {
    sd->_gex_buf = sd->_addr = GASNETI_AMPSHM_MSG_MED_DATA(msg);
  } else if (inline_long) {
    sd->_gex_buf = sd->_addr = GASNETI_AMPSHM_MSG_LONG_TMP(msg);
  } else {
    sd->_tofree = gasneti_prepare_alloc_buffer(sd);
  }

  return 0;
}

// After sd, next 3 params (isFixed, isReq, category) will be manifest constants
// which should lead to specialization of the code upon inlining.
GASNETI_INLINE(ampshm_comit_inner)
void ampshm_commit_inner(
                   gasneti_AM_SrcDesc_t sd, const int isFixed,
                   const int isReq, const int category,
                   gex_AM_Index_t handler, size_t nbytes,
                   void *dest_addr, va_list argptr)
{
  // Sanity checks:
  gasneti_assert(isFixed || (category == gasneti_Medium) || (category == gasneti_Long));
  gasneti_assert(!dest_addr || (category == gasneti_Long));
  gasneti_assert(sd);

  void *msg = sd->_void_p;

  /* Fill in message header */
  GASNETI_AMPSHM_MSG_CATEGORY(msg)  = category;
  gasneti_assert_uint( GASNETI_AMPSHM_MSG_CATEGORY(msg) ,==, category); // truncated?
  GASNETI_AMPSHM_MSG_HANDLERID(msg) = handler;
  GASNETI_AMPSHM_MSG_SOURCE(msg)    = gasneti_mynode;
  const int nargs = GASNETI_AMPSHM_MSG_NUMARGS(msg);
  for (int i = 0; i < nargs; i++) {
    GASNETI_AMPSHM_MSG_ARGS(msg)[i] = (gex_AM_Arg_t)va_arg(argptr, gex_AM_Arg_t);
  }

  // Stage payload to final location
  switch (category) {
    case gasneti_Short:
        /* Nothing to do */
        break;
    case gasneti_Medium: {
        GASNETI_AMPSHM_MSG_MED_NUMBYTES(msg) = nbytes;
        gasneti_assert_uint( GASNETI_AMPSHM_MSG_MED_NUMBYTES(msg) ,==, nbytes ); // truncated?
        void *data = GASNETI_AMPSHM_MSG_MED_DATA(msg);
        if (isFixed || (data != sd->_addr)) GASNETI_MEMCPY_SAFE_EMPTY(data, sd->_addr, nbytes);
        break;
    }
    case gasneti_Long: {
        GASNETI_AMPSHM_MSG_LONG_DATA(msg) = dest_addr;
        GASNETI_AMPSHM_MSG_LONG_NUMBYTES(msg) = nbytes;
        gasneti_assert_uint( GASNETI_AMPSHM_MSG_LONG_NUMBYTES(msg) ,==, nbytes ); // truncated?
        void *data = gasneti_pshm_jobrank_addr2local(sd->_pshm._jobrank, dest_addr);
        GASNETI_MEMCPY_SAFE_EMPTY(data, sd->_addr, nbytes);
        break;
    }
    default: gasneti_unreachable_error(("Invalid category=%i",(int)category));
  }

  /* Deliver message */
  gasneti_pshmnet_t *vnet = (isReq ? gasneti_request_pshmnet : gasneti_reply_pshmnet);
  gasneti_pshmnet_deliver_send_buffer(vnet, msg, 0 /*msgsz unused*/, sd->_pshm._pshmrank);

  if (sd->_tofree) { // Branch to avoid free(NULL) library call overhead for NPAM/cb
    gasneti_free(sd->_tofree);
    sd->_tofree = NULL;
  }
}

// First 1 params (category, isReq) will be manifest constants
// which should lead to specialization of the code upon inlining.
GASNETI_INLINE(gasnetc_AMPSHM_ReqRepGeneric)
int gasnetc_AMPSHM_ReqRepGeneric(gasneti_category_t category, int isReq, gex_Rank_t jobrank,
                                 gex_AM_Index_t handler, void *source_addr, size_t nbytes,
                                 void *dest_addr, gex_Flags_t flags, int numargs, va_list argptr
                                 GASNETI_THREAD_FARG)
{
  if (jobrank == gasneti_mynode) {
    return gasnetc_loopback_ReqRepGeneric(
                         isReq, category, handler,
                         source_addr, nbytes, dest_addr,
                         flags, numargs, argptr GASNETI_THREAD_PASS);
  }

  struct gasneti_AM_SrcDesc the_sd;
  the_sd._tofree = NULL;

  int imm = ampshm_prepare_inner(
                &the_sd, 1, isReq, category, jobrank, source_addr, 0, nbytes,
                dest_addr, NULL, flags, numargs GASNETI_THREAD_PASS);
  if (imm) return imm;

  gasneti_assume(the_sd._tofree == NULL); // in case the optimizer lost track
  ampshm_commit_inner(&the_sd, 1, isReq, category, handler, nbytes, dest_addr, argptr);

  return GASNET_OK;
}

// After sd, next 2 params (isReq, category) will be manifest constants
// which should lead to specialization of the code upon inlining.
GASNETI_INLINE(ampshm_prepare)
int ampshm_prepare(gasneti_AM_SrcDesc_t sd,
                   const int isReq, const gasneti_category_t category,
                   gex_Rank_t jobrank, const void *client_buf,
                   size_t least_payload, size_t most_payload,
                   void *dest_addr, gex_Event_t *lc_opt,
                   gex_Flags_t flags, unsigned int nargs)
{
  // Check for loopback
  // TODO-EX: TBD: move outward to "nbrhd" layer or leave here?
  int loopback = (jobrank == gasneti_mynode);
  sd->_pshm._loopback = loopback;
  if (loopback) {
    return gasnetc_loopback_Prepare(sd,isReq,category,client_buf,least_payload,most_payload,
                                    dest_addr,lc_opt,flags,nargs);
  }

  GASNET_POST_THREADINFO(sd->_thread);
  return ampshm_prepare_inner(
                sd, 0, isReq, category, jobrank, client_buf, least_payload, most_payload,
                dest_addr, lc_opt, flags, nargs GASNETI_THREAD_PASS);
}

// After sd, next 2 params (isReq, category) will be manifest constants
// which should lead to specialization of the code upon inlining.
GASNETI_INLINE(ampshm_comit)
void ampshm_commit(gasneti_AM_SrcDesc_t sd,
                   const int isReq, const gasneti_category_t category,
                   gex_AM_Index_t handler, size_t nbytes,
                   void *dest_addr, va_list argptr)
{
  // Check for loopback
  // TODO-EX: TBD: move outward to "nbrhd" layer or leave here?
  if (sd->_pshm._loopback) {
    gasnetc_loopback_Commit(sd,isReq,category,handler,nbytes,dest_addr,argptr);
    return;
  }

  ampshm_commit_inner(sd, 0, isReq, category, handler, nbytes, dest_addr, argptr);
}

//
// AM Request/Reply external interface
//
// TODO-EX: GASNETI_THREAD_FARG
//

int gasneti_AMPSHM_RequestShort(gex_Rank_t jobrank, gex_AM_Index_t handler,
                                gex_Flags_t flags, int numargs, va_list argptr
                                GASNETI_THREAD_FARG)
{
  gasneti_assert(gasneti_pshm_jobrank_in_supernode(jobrank));
  return gasnetc_AMPSHM_ReqRepGeneric(gasneti_Short, 1, jobrank, handler, NULL,
                                      0, NULL, flags, numargs, argptr
                                      GASNETI_THREAD_PASS);
}

int gasneti_AMPSHM_RequestMedium(gex_Rank_t jobrank, gex_AM_Index_t handler,
                                 void *source_addr, size_t nbytes,
                                 gex_Flags_t flags, int numargs, va_list argptr
                                 GASNETI_THREAD_FARG)
{
  gasneti_assert(gasneti_pshm_jobrank_in_supernode(jobrank));
  return gasnetc_AMPSHM_ReqRepGeneric(gasneti_Medium, 1, jobrank, handler, source_addr,
                                      nbytes, NULL, flags, numargs, argptr
                                      GASNETI_THREAD_PASS);
}

int gasneti_AMPSHM_RequestLong(gex_Rank_t jobrank, gex_AM_Index_t handler,
                               void *source_addr, size_t nbytes, void *dest_addr,
                               gex_Flags_t flags, int numargs, va_list argptr
                               GASNETI_THREAD_FARG)
{
  gasneti_assert(gasneti_pshm_jobrank_in_supernode(jobrank));
  return gasnetc_AMPSHM_ReqRepGeneric(gasneti_Long, 1, jobrank, handler, source_addr,
                                      nbytes, dest_addr, flags, numargs, argptr
                                      GASNETI_THREAD_PASS);
}

int gasneti_AMPSHM_ReplyShort(gex_Token_t token, gex_AM_Index_t handler,
                              gex_Flags_t flags, int numargs, va_list argptr)
{
  GASNETI_POST_THREADINFO_FROM_NBRHD_TOKEN(token);
  gex_Rank_t jobrank = gasnetc_ampshm_msgsource(token);
  return gasnetc_AMPSHM_ReqRepGeneric(gasneti_Short, 0, jobrank, handler, NULL,
                                        0, NULL, flags, numargs, argptr
                                        GASNETI_THREAD_PASS);
}

int gasneti_AMPSHM_ReplyMedium(gex_Token_t token, gex_AM_Index_t handler,
                               void *source_addr, size_t nbytes,
                               gex_Flags_t flags, int numargs, va_list argptr)
{
  GASNETI_POST_THREADINFO_FROM_NBRHD_TOKEN(token);
  gex_Rank_t jobrank = gasnetc_ampshm_msgsource(token);
  return gasnetc_AMPSHM_ReqRepGeneric(gasneti_Medium, 0, jobrank, handler, source_addr,
                                        nbytes, NULL, flags, numargs, argptr
                                        GASNETI_THREAD_PASS);
}

int gasneti_AMPSHM_ReplyLong(gex_Token_t token, gex_AM_Index_t handler,
                             void *source_addr, size_t nbytes, void *dest_addr,
                             gex_Flags_t flags, int numargs, va_list argptr)
{
  GASNETI_POST_THREADINFO_FROM_NBRHD_TOKEN(token);
  gex_Rank_t jobrank = gasnetc_ampshm_msgsource(token);
  return gasnetc_AMPSHM_ReqRepGeneric(gasneti_Long, 0, jobrank, handler, source_addr,
                                        nbytes, dest_addr, flags, numargs, argptr
                                        GASNETI_THREAD_PASS);
}


int gasnetc_AMPSHM_PrepareRequestMedium(
                        gasneti_AM_SrcDesc_t sd,
                        gex_Rank_t           jobrank,
                        const void          *client_buf,
                        size_t               least_payload,
                        size_t               most_payload,
                        gex_Event_t         *lc_opt,
                        gex_Flags_t          flags,
                        unsigned int         nargs)
{
  return ampshm_prepare(sd, 1, gasneti_Medium, jobrank, client_buf, least_payload, most_payload,
                        NULL, lc_opt, flags, nargs);
}

int gasnetc_AMPSHM_PrepareReplyMedium(
                        gasneti_AM_SrcDesc_t sd,
                        gex_Token_t          token,
                        const void          *client_buf,
                        size_t               least_payload,
                        size_t               most_payload,
                        gex_Event_t         *lc_opt,
                        gex_Flags_t          flags,
                        unsigned int         nargs)
{
#if GASNET_DEBUG
  sd->_dest._reply._token = token;
#endif
  gex_Rank_t jobrank = gasnetc_ampshm_msgsource(token);
  return ampshm_prepare(sd, 0, gasneti_Medium, jobrank, client_buf, least_payload, most_payload,
                        NULL, lc_opt, flags, nargs);
}

int gasnetc_AMPSHM_PrepareRequestLong(
                        gasneti_AM_SrcDesc_t sd,
                        gex_Rank_t           jobrank,
                        const void          *client_buf,
                        size_t               least_payload,
                        size_t               most_payload,
                        void                *dest_addr,
                        gex_Event_t         *lc_opt,
                        gex_Flags_t          flags,
                        unsigned int         nargs)
{
  return ampshm_prepare(sd, 1, gasneti_Long, jobrank, client_buf, least_payload, most_payload,
                        dest_addr, lc_opt, flags, nargs);
}

int gasnetc_AMPSHM_PrepareReplyLong(
                        gasneti_AM_SrcDesc_t sd,
                        gex_Token_t          token,
                        const void          *client_buf,
                        size_t               least_payload,
                        size_t               most_payload,
                        void                *dest_addr,
                        gex_Event_t         *lc_opt,
                        gex_Flags_t          flags,
                        unsigned int         nargs)
{
#if GASNET_DEBUG
  sd->_dest._reply._token = token;
#endif
  gex_Rank_t jobrank = gasnetc_ampshm_msgsource(token);
  return ampshm_prepare(sd, 0, gasneti_Long, jobrank, client_buf, least_payload, most_payload,
                        dest_addr, lc_opt, flags, nargs);
}

void gasnetc_AMPSHM_CommitRequestMedium(
                gasneti_AM_SrcDesc_t sd,
                gex_AM_Index_t handler,
                size_t nbytes, va_list argptr)
{
  ampshm_commit(sd, 1, gasneti_Medium, handler, nbytes, NULL, argptr);
}

void gasnetc_AMPSHM_CommitReplyMedium(
                gasneti_AM_SrcDesc_t sd,
                gex_AM_Index_t handler,
                size_t nbytes, va_list argptr)
{
  ampshm_commit(sd, 0, gasneti_Medium, handler, nbytes, NULL, argptr);
}

void gasnetc_AMPSHM_CommitRequestLong(
                gasneti_AM_SrcDesc_t sd,
                gex_AM_Index_t handler,
                size_t nbytes, void *dest_addr, va_list argptr)
{
  ampshm_commit(sd, 1, gasneti_Long, handler, nbytes, dest_addr, argptr);
}

void gasnetc_AMPSHM_CommitReplyLong(
                gasneti_AM_SrcDesc_t sd,
                gex_AM_Index_t handler,
                size_t nbytes, void *dest_addr, va_list argptr)
{
  ampshm_commit(sd, 0, gasneti_Long, handler, nbytes, dest_addr, argptr);
}

#endif /* GASNET_PSHM */
