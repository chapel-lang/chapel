/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_pshm.c $
 * Description: GASNet infrastructure for shared memory communications
 * Copyright 2012, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>

#if GASNET_PSHM /* Otherwise file is empty */

#include <gasnet_core_internal.h> /* for gasnetc_{Short,Medium,Long} and gasnetc_handler[] */

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

void *gasneti_pshm_init(gasneti_bootstrapExchangefn_t exchangefn, size_t aux_sz) {
  size_t vnetsz, mmapsz;
  int discontig = 0;
  gasneti_pshm_rank_t *pshm_max_nodes;
  gasnet_node_t i;
#if !GASNET_CONDUIT_SMP
  gasnet_node_t j;
#endif

  gasneti_assert(exchangefn != NULL);  /* NULL exchangefn no longer supported */

  /* Testing if the number of PSHM nodes is always smaller than GASNETI_PSHM_MAX_NODES */
  pshm_max_nodes = gasneti_calloc(gasneti_nodes, sizeof(gasneti_pshm_rank_t));
  for(i=0; i<gasneti_nodes; i++){
    /* Note combination of post-increment and == guard against overflow */
    if ((pshm_max_nodes[gasneti_nodemap[i]]++) == GASNETI_PSHM_MAX_NODES){
      if (gasneti_mynode == gasneti_nodemap[i]){
        gasneti_fatalerror("PSHM nodes requested on node '%s' exceeds maximum (%d)\n", 
                        gasneti_gethostname(), GASNETI_PSHM_MAX_NODES);
      } else {
        gasneti_fatalerror("PSHM nodes requested on some node exceeds maximum (%d)\n", 
                        GASNETI_PSHM_MAX_NODES);
      }
    }
  }
  gasneti_free(pshm_max_nodes);
    
  gasneti_pshm_nodes = gasneti_nodemap_local_count;
  gasneti_pshm_firstnode = gasneti_nodemap[gasneti_mynode];
  gasneti_pshm_mynode = gasneti_nodemap_local_rank;

#if GASNET_CONDUIT_SMP
  gasneti_assert(gasneti_pshm_nodes == gasneti_nodes);
  gasneti_assert(gasneti_pshm_firstnode == 0);
  gasneti_assert(gasneti_pshm_mynode == gasneti_mynode);
#else
  gasneti_assert(gasneti_nodemap[0] == 0);
  for (i=1; i<gasneti_nodes; ++i) {
    /* Determine if supernode members are numbered contiguously */
    if (gasneti_nodemap[i-1] > gasneti_nodemap[i]) {
      discontig = 1;
      break;
    }
  }
#endif

  gasneti_assert(gasneti_pshm_supernodes > 0);

  /* compute size of vnet shared memory region */
  vnetsz = gasneti_pshmnet_memory_needed(gasneti_pshm_nodes); 
  mmapsz = (2*vnetsz);
  { /* gasneti_pshm_info contains multiple variable-length arrays in the same space */
    size_t info_sz;
    /* space for gasneti_pshm_firsts: */
    info_sz = gasneti_pshm_supernodes * sizeof(gasnet_node_t);
    /* optional space for gasneti_pshm_rankmap: */
    if (discontig) {
      info_sz = GASNETI_ALIGNUP(info_sz, sizeof(gasneti_pshm_rank_t));
      info_sz += gasneti_nodes * sizeof(gasneti_pshm_rank_t);
    }
    /* space for the PSHM intra-node barrier */
    info_sz = GASNETI_ALIGNUP(info_sz, GASNETI_CACHE_LINE_BYTES);
    info_sz += sizeof(gasneti_pshm_barrier_t) +
	       (gasneti_pshm_nodes - 1) * sizeof(gasneti_pshm_barrier->node);
    /* space for early barrier, sharing space with the items above: */
    info_sz = MAX(info_sz, gasneti_pshm_nodes * sizeof(gasneti_pshm_info->early_barrier[0]));
    info_sz += offsetof(struct gasneti_pshm_info, early_barrier);
    /* final space requested: */
    mmapsz += round_up_to_pshmpage(info_sz);
  }
  mmapsz += round_up_to_pshmpage(aux_sz);

  /* setup vnet shared memory region for AM infrastructure and supernode barrier.
   */
  gasnetc_pshmnet_region = gasneti_mmap_vnet(mmapsz, exchangefn);
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
  if (gasneti_pshm_mynode == 0) {
    gasneti_atomic_set(&gasneti_pshm_info->bootstrap_barrier_cnt, gasneti_pshm_nodes, 0);
    gasneti_atomic_set(&gasneti_pshm_info->bootstrap_barrier_gen, 0, 0);
  }

  /* "early" barrier which protects initialization of the real barrier counter. */
  gasneti_local_wmb();
  if (gasneti_pshm_mynode) {
    gasneti_pshm_info->early_barrier[gasneti_pshm_mynode].val = 1;
    gasneti_waituntil(gasneti_pshm_info->early_barrier[0].val != 0);
  } else {
    for (i = 1; i < gasneti_pshm_nodes; ++i) {
      gasneti_waituntil(gasneti_pshm_info->early_barrier[i].val != 0);
    }
    gasneti_pshm_info->early_barrier[0].val = 1;
  }

  /* Unlink the shared memory file to prevent leaks.
   * "early" barrier above ensures all procs have attached. */
  gasneti_unlink_vnet();

  /* Carve out various allocations, reusing the "early barrier" space. */
  gasneti_pshmnet_bootstrapBarrier();
  {
    uintptr_t addr = (uintptr_t)&gasneti_pshm_info->early_barrier;
    /* gasneti_pshm_firsts, an array of gasneti_pshm_supernodes*sizeof(gasnet_node_t): */
    gasneti_pshm_firsts = (gasnet_node_t *)addr;
    addr += gasneti_pshm_supernodes * sizeof(gasnet_node_t);
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
      gasneti_assert(j <= gasneti_pshm_supernodes);
    }
  }
#endif
#if GASNET_DEBUG
  /* Validate gasneti_pshm_firsts[] */
  if (gasneti_pshm_mynode == 0) {
    for (i=0; i<gasneti_pshm_supernodes; ++i) {
      gasneti_assert(!i || (gasneti_pshm_firsts[i] > gasneti_pshm_firsts[i-1]));
      gasneti_assert(gasneti_nodemap[gasneti_pshm_firsts[i]] == gasneti_pshm_firsts[i]);
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

/* Defaults if gasnet_core_fwd.h doesn't #define these preprocessor tokens */
#ifndef GASNETC_MAX_ARGS_PSHM
  /* Assumes gasnet_AMMaxArgs() expands to a compile-time constant */
  #define GASNETC_MAX_ARGS_PSHM   (gasnet_AMMaxArgs())
#endif
#ifndef GASNETC_MAX_MEDIUM_PSHM
  /* Assumes gasnet_AMMaxMedium() expands to a compile-time constant */
  #define GASNETC_MAX_MEDIUM_PSHM (gasnet_AMMaxMedium())
#endif
#ifndef GASNETC_GET_HANDLER
  /* Assumes conduit has gasnetc_handler[] as in template-conduit */
  #define gasnetc_get_handler(_h) (gasnetc_handler[(_h)])
#endif
#ifndef GASNETC_TOKEN_CREATE
  /* Our default implementation is suitable for conduits that use a pointer
   * for gasnet_token_t.  We generate tokens with the least-significant bit
   * set.  This distinguishes them from a valid pointer, but still allows a
   * (token != NULL) assertion.
   */
  #if GASNET_DEBUG
    typedef struct {
      gasnet_node_t srcNode;
      int isReq;
      int replySent;
    } gasneti_ampshm_token_t;

    static gasnet_token_t gasnetc_token_create(gasnet_node_t src, int isReq) {
      gasneti_ampshm_token_t *my_token = gasneti_malloc(sizeof(gasneti_ampshm_token_t));
      gasneti_assert(!((uintptr_t)my_token & 1));
      gasneti_assert(gasneti_pshm_in_supernode(src));
      my_token->srcNode = src;
      my_token->isReq = isReq;
      my_token->replySent = 0;
      return (gasnet_token_t)(1|(uintptr_t)my_token);
    }

    #define gasnetc_token_destroy(tok) gasneti_free((void*)(1^(uintptr_t)(tok)))

    extern void gasnetc_token_reply(gasnet_token_t token) {
      gasneti_ampshm_token_t *my_token = (gasneti_ampshm_token_t *)(1^(uintptr_t)token);
      gasneti_assert(gasnetc_token_is_pshm(token));
      gasneti_assert(my_token);
      gasneti_assert(my_token->isReq);
      gasneti_assert(!my_token->replySent);
      my_token->replySent = 1;
    }

    extern int gasneti_AMPSHMGetMsgSource(gasnet_token_t token, gasnet_node_t *src_ptr) {
      int retval = GASNET_ERR_BAD_ARG;
      if (gasnetc_token_is_pshm(token)) {
        gasneti_ampshm_token_t *my_token = (gasneti_ampshm_token_t *)(1^(uintptr_t)token);
        gasnet_node_t tmp = my_token->srcNode;
        gasneti_assert(gasneti_pshm_in_supernode(tmp));
        *src_ptr = tmp;
        retval = GASNET_OK;
      }
      return retval;
    }
  #else
    /* We encode the source in a uintptr_t but ignore isReq.  */
    #define gasnetc_token_create(_src, _isReq) \
      ((gasnet_token_t)(1 | ((uintptr_t)(_src) << 1)))
    #define gasnetc_token_destroy(tok) ((void)0)
  #endif
#endif


/*******************************************************************************
 * PSHM global variables:
 ******************************************************************************/
gasneti_pshm_rank_t gasneti_pshm_nodes = 0;
gasnet_node_t gasneti_pshm_firstnode = (gasnet_node_t)(-1);
gasneti_pshm_rank_t gasneti_pshm_mynode = (gasneti_pshm_rank_t)(-1);
/* vectors constructed in shared space: */
gasneti_pshm_rank_t *gasneti_pshm_rankmap = NULL;
gasnet_node_t *gasneti_pshm_firsts = NULL;

/*******************************************************************************
 * "PSHM Net":  message header formats
 * These come early because their sizes influence allocation
 ******************************************************************************/

/* TODO: Could/should we squeeze unused args out of a Medium.*/
/* TODO: Pack category and numargs together (makes assumtion about ranges) */

typedef struct {
  uint8_t category;      /* AM msg type: short, med, long */
  uint8_t numargs;
  gasnetc_handler_t handler_id;
  gasnet_node_t source;
  gasnet_handlerarg_t args[GASNETC_MAX_ARGS_PSHM];
} gasneti_AMPSHM_msg_t;
typedef gasneti_AMPSHM_msg_t gasneti_AMPSHM_shortmsg_t;

typedef struct {
  gasneti_AMPSHM_msg_t msg;
#if (GASNETI_MAX_MEDIUM_PSHM < 65536) /* GASNET<C>_MAX_MEDIUM_PSHM often not a preprocess-time constant */
  uint16_t numbytes;
#else
  uint32_t numbytes;
#endif
  uint8_t  mediumdata[6 + GASNETC_MAX_MEDIUM_PSHM]; /* Is 2, 4 or 8-byte aligned */
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
#if GASNET_PAR
  /* serializes dequeue operations */
  gasneti_mutex_t lock;
#endif
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
GASNETI_ALWAYS_INLINE(gasneti_pshm_addr)
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
gasneti_pshmnet_init(void *start, size_t nbytes, gasneti_pshm_rank_t pshmnodes)
{
  gasneti_pshmnet_t *vnet;
  size_t szonce, szpernode, regionlen;
  void *region, *myregion;

  /* make sure that our max buffer size fits all possible AMs */
  gasneti_assert(sizeof(gasneti_AMPSHM_maxmsg_t) <= GASNETI_PSHMNET_MAX_PAYLOAD);

  region = start;
  region = (void *)round_up_to_pshmpage(region);

  szpernode = gasneti_pshmnet_memory_needed_pernode(pshmnodes);
  szonce = gasneti_pshmnet_memory_needed_once(pshmnodes);

  regionlen = nbytes - ( ((uintptr_t)region)-((uintptr_t)start));
  if (regionlen < (szonce + szpernode * pshmnodes))
    gasneti_fatalerror("Internal error: not enough memory for pshmnet: \n"
                       " given %lu effective bytes, but need %lu", 
                       (unsigned long)regionlen, (unsigned long)(szonce + szpernode * pshmnodes));

  vnet = gasneti_malloc(sizeof(gasneti_pshmnet_t));
  vnet->nodecount = pshmnodes;
#if GASNET_PAR
  gasneti_mutex_init(&vnet->lock);
#endif

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
  
  gasneti_assert(nbytes <= GASNETI_PSHMNET_MAX_PAYLOAD);

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

GASNETI_ALWAYS_INLINE(gasneti_pshmnet_queue_peek)
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

#if GASNET_PAR
  if (gasneti_pshmnet_queue_peek(q)) {
    gasneti_mutex_lock(&vnet->lock);
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
#if GASNET_PAR
    gasneti_mutex_unlock(&vnet->lock);
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
void gasneti_pshmnet_bootstrapBarrier(void)
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

  gasneti_waitwhile((curr = gasneti_atomic_read(&gasneti_pshm_info->bootstrap_barrier_gen, 0)) < target);
  if_pf (curr >= GASNETI_PSHM_BSB_LIMIT) gasnet_exit(1);

  generation = target;
}

/******************************************************************************
 * "critical sections" in which we notify peers if we abort() while
 * they are potentially blocked in gasneti_pshmnet_bootstrapBarrier().
 * These DO NOT nest (but there is no checking to ensure that).
 ******************************************************************************/
static gasneti_sighandlerfn_t gasneti_prev_abort_handler = NULL;

static void gasneti_pshm_abort_handler(int sig) {
  gasneti_atomic_set(&gasneti_pshm_info->bootstrap_barrier_gen, GASNETI_PSHM_BSB_LIMIT, 0);

  gasneti_reghandler(SIGABRT, gasneti_prev_abort_handler);
#if HAVE_SIGPROCMASK /* Is this ever NOT the case? */
  { sigset_t new_set, old_set;
    sigemptyset(&new_set);
    sigaddset(&new_set, SIGABRT);
    sigprocmask(SIG_UNBLOCK, &new_set, &old_set);
  }
#endif
  raise(SIGABRT);
}

void gasneti_pshm_cs_enter(void)
{
  gasneti_prev_abort_handler = gasneti_reghandler(SIGABRT, &gasneti_pshm_abort_handler);
}

void gasneti_pshm_cs_leave(void)
{
  gasneti_reghandler(SIGABRT, gasneti_prev_abort_handler);
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
  gasneti_assert(vnet->nodecount == gasneti_pshm_nodes);

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
  gasneti_assert(vnet->nodecount == gasneti_pshm_nodes);

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
  gasneti_assert(vnet->nodecount == gasneti_pshm_nodes);

  /* All nodes send their contribution in chunks */
  while (remain) {
    size_t nbytes = MIN(remain, GASNETI_PSHMNET_MAX_PAYLOAD);

    if (gasneti_pshm_mynode == rootpshmnode) {
      gasneti_pshm_rank_t i;
      for (i = 0; i < vnet->nodecount - 1; i++) {
        gasneti_pshm_rank_t msg_from;
        size_t msg_len;

        gasneti_waitwhile (gasneti_pshmnet_recv(vnet, &msg, &msg_len, &msg_from));
        gasneti_assert(msg_len == nbytes);
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
  gasneti_assert(nbytes <= GASNETI_PSHMNET_ALLOC_MAXSZ);

  needed = (nbytes + GASNETI_PSHMNET_PAGESIZE - 1) >> GASNETI_PSHMNET_PAGESHIFT;
  gasneti_assert(needed <= GASNETI_PSHMNET_ALLOC_MAXPG);

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
        gasneti_assert (next <= a->count);
        if (next == a->count) break; /* hit end of region */
        next_block = (gasneti_pshmnet_allocator_block_t*)
                     ((uintptr_t)block + (length << GASNETI_PSHMNET_PAGESHIFT));
        if (gasneti_atomic_read(&next_block->in_use, GASNETI_ATOMIC_ACQ)) break; /* hit busy block */
        length += a->length[next];
      }

      if (length >= needed) {
        unsigned int next = curr + needed;

        if (length > needed) { /* Split it */
          gasneti_assert (next < a->count);
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
  gasneti_assert(p == &block->payload);
  /* assert block is page-aligned */
  gasneti_assert( (((uintptr_t)block) % GASNETI_PSHMNET_PAGESIZE) == 0);

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

#define GASNETI_AMPSHM_MAX_REPLY_PER_POLL 10
#define GASNETI_AMPSHM_MAX_REQUEST_PER_POLL 10

#ifndef GASNETC_ENTERING_HANDLER_HOOK
  /* extern void enterHook(int cat, int isReq, int handlerId, gasnet_token_t *token,
   *                       void *buf, size_t nbytes, int numargs, gasnet_handlerarg_t *args);
   */
  #define GASNETC_ENTERING_HANDLER_HOOK(cat,isReq,handlerId,token,buf,nbytes,numargs,args) ((void)0)
#endif
#ifndef GASNETC_LEAVING_HANDLER_HOOK
  /* extern void leaveHook(int cat, int isReq);
   */
  #define GASNETC_LEAVING_HANDLER_HOOK(cat,isReq) ((void)0)
#endif

/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasneti_AMPSHM_service_incoming_msg)
int gasneti_AMPSHM_service_incoming_msg(gasneti_pshmnet_t *vnet, int isReq)
{
  void *msg;
  size_t msgsz;
  gasneti_pshm_rank_t from;
  int category;
  gasnetc_handler_t handler_id;
  gasneti_handler_fn_t handler_fn;
  int numargs;
  gasnet_handlerarg_t *args;
  gasnet_token_t token;

  gasneti_assert(vnet != NULL);

  if (gasneti_pshmnet_recv(vnet, &msg, &msgsz, &from))
    return -1;

  token = gasnetc_token_create(GASNETI_AMPSHM_MSG_SOURCE(msg), isReq);
  category = GASNETI_AMPSHM_MSG_CATEGORY(msg);
  gasneti_assert((category == gasnetc_Short) || 
                 (category == gasnetc_Medium) || 
                 (category == gasnetc_Long));
  handler_id = GASNETI_AMPSHM_MSG_HANDLERID(msg);
  handler_fn = gasnetc_get_handler(handler_id);
  numargs = GASNETI_AMPSHM_MSG_NUMARGS(msg);
  args = GASNETI_AMPSHM_MSG_ARGS(msg);

  switch (category) {
    case gasnetc_Short:
      { 
        GASNETC_ENTERING_HANDLER_HOOK(category,isReq,handler_id,token,NULL,0,numargs,args);
        GASNETI_RUN_HANDLER_SHORT(isReq,handler_id,handler_fn,token,args,numargs);
      }
      break;
    case gasnetc_Medium:
      {
        void * data = GASNETI_AMPSHM_MSG_MED_DATA(msg);
        size_t nbytes = GASNETI_AMPSHM_MSG_MED_NUMBYTES(msg);
        GASNETC_ENTERING_HANDLER_HOOK(category,isReq,handler_id,token,data,nbytes,numargs,args);
        GASNETI_RUN_HANDLER_MEDIUM(
          isReq,handler_id,handler_fn,token,args,numargs,data,nbytes);
      }
      break;
    case gasnetc_Long:
      { 
        void * data = GASNETI_AMPSHM_MSG_LONG_DATA(msg);
        size_t nbytes = GASNETI_AMPSHM_MSG_LONG_NUMBYTES(msg);
        GASNETC_ENTERING_HANDLER_HOOK(category,isReq,handler_id,token,data,nbytes,numargs,args);
        GASNETI_RUN_HANDLER_LONG(
            isReq,handler_id,handler_fn,token,args,numargs,data,nbytes);
      }
      break;
  }
  GASNETC_LEAVING_HANDLER_HOOK(category,isReq);
  gasnetc_token_destroy(token);
  gasneti_pshmnet_recv_release(vnet, msg);
  return 0;
}

/* ------------------------------------------------------------------------------------ */
int gasneti_AMPSHMPoll(int repliesOnly)
{
  int i = 0;

#if 0
  /* We skip CHECKATTACH to allow "early" internal use by conduits. */
  GASNETI_CHECKATTACH();
#endif

  if (gasneti_pshmnet_queue_peek(gasneti_reply_pshmnet->my_queue)) {
    for (i = 0; i < GASNETI_AMPSHM_MAX_REPLY_PER_POLL; i++) 
      if (gasneti_AMPSHM_service_incoming_msg(gasneti_reply_pshmnet, 0))
        break;
  }
  if (!repliesOnly && gasneti_pshmnet_queue_peek(gasneti_request_pshmnet->my_queue)) {
    for (i = 0; i < GASNETI_AMPSHM_MAX_REQUEST_PER_POLL; i++) 
      if (gasneti_AMPSHM_service_incoming_msg(gasneti_request_pshmnet, 1))
        break;
  }
  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
/*
 * Active Message Request Functions
 * ================================
 */

/* Loopback AMs use buffers from this free pool.
 * Worst case this pool grows to two per threads (one request and one reply).
 * TODO: per-thread buffers (as in smp-conduit) would remove contention, but
 * requires mofiying the conduit-specific code for threaddata.
 */
static gasneti_lifo_head_t loopback_freepool = GASNETI_LIFO_INITIALIZER;

int gasnetc_AMPSHM_ReqRepGeneric(int category, int isReq, gasnet_node_t dest,
                                 gasnetc_handler_t handler, void *source_addr, size_t nbytes, 
                                 void *dest_addr, int numargs, va_list argptr) 
{
  gasneti_pshmnet_t *vnet = (isReq ? gasneti_request_pshmnet : gasneti_reply_pshmnet);
  gasneti_pshm_rank_t target = gasneti_pshm_local_rank(dest);
  size_t msgsz = 0;
  int i;
  void *msg;
  int loopback = (dest == gasneti_mynode);

  gasneti_assert(vnet != NULL);
  gasneti_assert(target < gasneti_pshm_nodes);

  if (loopback) {
    msg = gasneti_lifo_pop(&loopback_freepool);
    if_pf (msg == NULL) {
      /* Grow the free pool with buffers sized and aligned for the largest Medium */
      void *tmp = gasneti_malloc(sizeof(gasneti_AMPSHM_medmsg_t)+7);
      uintptr_t offset = (uintptr_t)GASNETI_AMPSHM_MSG_MED_DATA(tmp) & 7;
      /* Align the (macro-adjusted) Medium payload field, not the msg itself */
      msg = (void*)((uintptr_t)tmp + (offset ? (8-offset) : 0));
    }
  } else {
    static gasneti_mutex_t req_lock = GASNETI_MUTEX_INITIALIZER;
    static gasneti_mutex_t rep_lock = GASNETI_MUTEX_INITIALIZER;
    gasneti_mutex_t *lock;

    /* calculate size of buffer needed */
    switch (category) {
      case gasnetc_Short:
        msgsz = sizeof(gasneti_AMPSHM_shortmsg_t);
        break;
      case gasnetc_Medium:
        msgsz = sizeof(gasneti_AMPSHM_medmsg_t) - (GASNETC_MAX_MEDIUM_PSHM - nbytes);
        break;
      case gasnetc_Long:
        msgsz = sizeof(gasneti_AMPSHM_longmsg_t);
        break;
      default:
        gasneti_fatalerror("internal error: unknown msg category");
    }
    gasneti_assert(msgsz <= sizeof(gasneti_AMPSHM_maxmsg_t)); 

    /* Get buffer, poll if busy.
       Lock serializes allocation so small messages can't starve large ones */
    lock = isReq ? &req_lock : &rep_lock;
    gasneti_mutex_lock(lock);
    while (!(msg = gasneti_pshmnet_get_send_buffer(vnet, msgsz, target))) {
      /* If reply, only poll reply network: avoids deadlock  */
      if (isReq) gasnetc_AMPoll(); /* No progress functions */
      else gasneti_AMPSHMPoll(1);
      GASNETI_WAITHOOK();
    }
    gasneti_mutex_unlock(lock);
  }

  /* Fill in message */
  GASNETI_AMPSHM_MSG_CATEGORY(msg) = category;
  GASNETI_AMPSHM_MSG_HANDLERID(msg) = handler;
  GASNETI_AMPSHM_MSG_NUMARGS(msg) = numargs;
  GASNETI_AMPSHM_MSG_SOURCE(msg) = gasneti_mynode;
  for(i = 0; i < numargs; i++) 
    GASNETI_AMPSHM_MSG_ARGS(msg)[i] = (gasnet_handlerarg_t)va_arg(argptr, int);

  /* Detect truncation if our field widths were too small */
  gasneti_assert( GASNETI_AMPSHM_MSG_CATEGORY(msg) == category );
  gasneti_assert( GASNETI_AMPSHM_MSG_NUMARGS(msg) == numargs );

  switch (category) {
    case gasnetc_Short:
      break;
    case gasnetc_Medium:
      GASNETI_AMPSHM_MSG_MED_NUMBYTES(msg) = nbytes;
      gasneti_assert( GASNETI_AMPSHM_MSG_MED_NUMBYTES(msg) == nbytes ); /* truncation check */
      memcpy(GASNETI_AMPSHM_MSG_MED_DATA(msg), source_addr, nbytes);
      break;
    case gasnetc_Long: {
      void *local_dest_addr = gasneti_pshm_addr2local(dest, dest_addr);

      GASNETI_AMPSHM_MSG_LONG_DATA(msg) = dest_addr; 
      GASNETI_AMPSHM_MSG_LONG_NUMBYTES(msg) = nbytes;
      gasneti_assert( GASNETI_AMPSHM_MSG_LONG_NUMBYTES(msg) == nbytes ); /* truncation check */
      /* deliver_msg call, below, contains write flush, so don't need here */
      memcpy(local_dest_addr, source_addr, nbytes);
      break;
    }
  }

  /* Deliver message */
  if (loopback) {
    gasneti_handler_fn_t handler_fn = gasnetc_get_handler(handler); 
    gasnet_token_t token = gasnetc_token_create(gasneti_mynode, isReq);
    gasnet_handlerarg_t *args = GASNETI_AMPSHM_MSG_ARGS(msg);
    switch (category) {
      case gasnetc_Short:
        GASNETC_ENTERING_HANDLER_HOOK(category,isReq,handler,token,NULL,0,numargs,args);
        GASNETI_RUN_HANDLER_SHORT(isReq,handler,handler_fn,token,args,numargs);

        break;
      case gasnetc_Medium:
        GASNETC_ENTERING_HANDLER_HOOK(category,isReq,handler,token,
                                      GASNETI_AMPSHM_MSG_MED_DATA(msg),nbytes,numargs,args);
        GASNETI_RUN_HANDLER_MEDIUM(isReq, handler, handler_fn, token, args, numargs,
                                   GASNETI_AMPSHM_MSG_MED_DATA(msg), nbytes);
        break;
      case gasnetc_Long:
        gasneti_local_wmb(); /* sync memcpy, above */
        GASNETC_ENTERING_HANDLER_HOOK(category,isReq,handler,token,dest_addr,nbytes,numargs,args);
        GASNETI_RUN_HANDLER_LONG(isReq, handler, handler_fn, token, args, numargs,
                                 dest_addr, nbytes);
        break;
    }
    GASNETC_LEAVING_HANDLER_HOOK(category,isReq);
    gasneti_lifo_push(&loopback_freepool, msg);
    gasnetc_token_destroy(token);
  } else {
    gasneti_pshmnet_deliver_send_buffer(vnet, msg, msgsz, target);
  }
  return GASNET_OK;
}

#endif /* GASNET_PSHM */
