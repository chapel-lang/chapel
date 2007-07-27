/*   $Source$
 *     $Date$
 * $Revision$
 * Description: GASNet Extended API Collective declarations
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_COLL_H
#define _GASNET_COLL_H

#include <gasnet.h>

GASNETI_BEGIN_EXTERNC

/*---------------------------------------------------------------------------------*/
/* Flag values: */

/* Sync flags - NO DEFAULT */
#define GASNET_COLL_IN_NOSYNC	(1<<0)
#define GASNET_COLL_IN_MYSYNC	(1<<1)
#define GASNET_COLL_IN_ALLSYNC	(1<<2)
#define GASNET_COLL_OUT_NOSYNC	(1<<3)
#define GASNET_COLL_OUT_MYSYNC	(1<<4)
#define GASNET_COLL_OUT_ALLSYNC	(1<<5)

#define GASNET_COLL_SINGLE	(1<<6)
#define GASNET_COLL_LOCAL	(1<<7)

#define GASNET_COLL_AGGREGATE	(1<<8)

#define GASNET_COLL_DST_IN_SEGMENT	(1<<9)
#define GASNET_COLL_SRC_IN_SEGMENT	(1<<10)

/* Scan (prefix reduction) flags - NO DEFAULT */
#define GASNET_COLL_INCLUSIVE_SCAN	(1<<11)
#define GASNET_COLL_EXCLUSIVE_SCAN	(1<<12)

/* Flag values for INTERNAL use */
#if GASNET_PAR
  #define GASNETE_COLL_THREAD_LOCAL	(1<<29)
#endif
#define GASNETE_COLL_SUBORDINATE	(1<<30)

#define GASNETE_COLL_IN_MODE(flags) \
	((flags) & (GASNET_COLL_IN_NOSYNC  | GASNET_COLL_IN_MYSYNC  | GASNET_COLL_IN_ALLSYNC))
#define GASNETE_COLL_OUT_MODE(flags) \
	((flags) & (GASNET_COLL_OUT_NOSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_OUT_ALLSYNC))
#define GASNETE_COLL_SYNC_MODE(flags) \
	((flags) & (GASNET_COLL_OUT_NOSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_OUT_ALLSYNC | \
		    GASNET_COLL_IN_NOSYNC  | GASNET_COLL_IN_MYSYNC  | GASNET_COLL_IN_ALLSYNC))

/* (prefix-)reduction function flags */
#define GASNET_COLL_AMSAFE	(1<<0)
#define GASNET_COLL_NONCOMM	(1<<1)

/* Internal flags */
#define GASNETE_COLL_OP_COMPLETE	0x1
#define GASNETE_COLL_OP_INACTIVE	0x2

/*---------------------------------------------------------------------------------*/

/* Forward type decls and typedefs: */
struct gasnete_coll_op_t_;
typedef struct gasnete_coll_op_t_ gasnete_coll_op_t;

struct gasnete_coll_p2p_t_;
typedef struct gasnete_coll_p2p_t_ gasnete_coll_p2p_t;

union gasnete_coll_p2p_entry_t_;
typedef union gasnete_coll_p2p_entry_t_ gasnete_coll_p2p_entry_t;

struct gasnete_coll_generic_data_t_;
typedef struct gasnete_coll_generic_data_t_ gasnete_coll_generic_data_t;

struct gasnete_coll_tree_data_t_;
typedef struct gasnete_coll_tree_data_t_ gasnete_coll_tree_data_t;

/*---------------------------------------------------------------------------------*/

#ifndef GASNETE_COLL_IMAGE_OVERRIDE
  /* gasnet_image_t must be large enough to index all threads that participate
   * in collectives.  A conduit may override this if a smaller type will suffice.
   * However, types larger than 32-bits won't pass as AM handler args.  So, for
   * a larger type, many default things will require overrides.
   */
  typedef uint32_t gasnet_image_t;
  #if GASNET_SEQ
    #define gasnete_coll_image_node(I)	I
  #else
    extern gasnet_node_t *gasnete_coll_image_to_node;
    #define gasnete_coll_image_node(I)	\
	(gasneti_assert(gasnete_coll_image_to_node != NULL), gasnete_coll_image_to_node[I])
  #endif
  #define gasnete_coll_image_is_local(I)	(gasneti_mynode == gasnete_coll_image_node(I))
#endif

/*---------------------------------------------------------------------------------*/

#ifndef GASNETE_COLL_HANDLE_OVERRIDE
  /* Handle type for collective ops: */
  #if GASNET_PAR
    struct gasnet_coll_handle_t_;
    typedef struct gasnet_coll_handle_t_ *gasnet_coll_handle_t;
    struct gasnet_coll_handle_t_ {
      volatile int		done;
      gasnet_coll_handle_t	next;
    };
  #else
    typedef volatile uintptr_t *gasnet_coll_handle_t;
  #endif
  #define GASNET_COLL_INVALID_HANDLE ((gasnet_coll_handle_t)NULL)
#endif

extern gasnet_coll_handle_t gasnete_coll_handle_create(GASNETE_THREAD_FARG_ALONE);
extern void gasnete_coll_handle_signal(gasnet_coll_handle_t handle GASNETE_THREAD_FARG);
extern int gasnete_coll_handle_done(gasnet_coll_handle_t handle GASNETE_THREAD_FARG);

/*---------------------------------------------------------------------------------*/
/* Operations of the active list */

extern gasneti_mutex_t gasnete_coll_active_lock;
extern void gasnete_coll_active_init(void);
extern void gasnete_coll_active_fini(void);
extern gasnete_coll_op_t *gasnete_coll_active_first(void);
extern gasnete_coll_op_t *gasnete_coll_active_next(gasnete_coll_op_t *op);
extern void gasnete_coll_active_new(gasnete_coll_op_t *op);
extern void gasnete_coll_active_ins(gasnete_coll_op_t *op);
extern void gasnete_coll_active_del(gasnete_coll_op_t *op);

/*---------------------------------------------------------------------------------*/

/* Functions, types, etc for computational collectives */

#ifndef GASNET_COLL_FN_HANDLE_T
  typedef gasnet_handlerarg_t gasnet_coll_fn_handle_t;
#endif

typedef void (*gasnet_coll_reduce_fn_t)(
	void *results, size_t result_count,
	const void *left_operands, size_t left_count,
	const void *right_operands,
	size_t elem_size, int flags, int arg);
/*
    results: Output array
	This argument is the address of the space into which the
	result(s) should be written.
    result_count: Result count
	The number of results to be generated.  Since there is
	a one-to-one correspondence between results and
	right-hand operands, this is also the length of the
	'right_operands' array. The client may safely assume that a
	callback invoked on behalf of a (non-prefix) reduce function
	will always have result_count == 1. Otherwise, it will be >= 1.
    left_operands: Optional left-hand operand array
	This argument is the address of 0 or more operands which lie
	to the left of those given by the 'right_operands' argument.
	The number of operands in this array is given by the
	'left_count' argument.  If 'left_count' is zero then this
	argument is ignored.
    left_count: Left-hand operand count
	The number of operands in the 'left_operands' array.
	The client may safely assume that a callback invoked on behalf 
	of a (non-prefix) reduce function will always have 
	left_count > 0. No other guarantees are provided.
    right_operands: Right-hand operand array
	This argument is the address of 1 or more operands which lie
	to the right of those given by the 'left_operands' argument.
	The number of operands in this array is given by the
	'result_count' argument.
    elem_size: Size of the operand and result elements.
	This argument gives the size of the data elements
	which are to be manipulated.
    flags: Flag values
	This argument gives information about the operation and
	is composed of a bitwise or of values.
	The value of 'flags' will include GASNET_COLL_NONCOMM
	if and only if the client passed this same flag to the
	gasnet_coll_init() function when registering this function.
	The remaining bits of 'flags' are reserved for future use and
	the client must not assume they will have any particular
	value.
    arg: Client-supplied argument
	This is the one argument passed into the gasnet_coll_init()
	function by the client when registering this function.

    On return (where "sum" and "+" are used in a generalized sense)
	  result[i] = sum(j=0..left_count-1, left_operands[j])
		    + sum(k=0..i, right_operands[k])

    For a single instance of the operation "a = b + c"
	results = &a
	results_count = 1
	left_operands = &b
	left_count = 1
	right_operands = &c

    For a (non-prefix) reduction a = sum(j=0..n-1, A[j])
	results = &a
	results_count = 1
	left_operands = A
	left_count = n-1
	right_operands = A + (n-1)

    For an inclusive prefix reduction B[i] = sum(j=0..i, A[j]) for i=0..n-1
	results = B
	results_count = n
	left_operands = NULL (ignored)
	left_count = 0
	right_operands = A

    For an exclusive prefix reduction B[i] = sum(j=0..i-1, A[j]) for i=1..n-1
	results = B + 1
	results_count = n - 1
	left_operands = A
	left_count = 1
	right_operands = A + 1
*/

typedef struct {
    gasnet_coll_reduce_fn_t	fnptr;
    unsigned int		flags;
} gasnet_coll_fn_entry_t;

/*---------------------------------------------------------------------------------*/

/* Handle type for collective teams: */
#ifndef GASNETE_COLL_TEAMS_OVERRIDE
    struct gasnete_coll_team_t_;
    typedef struct gasnete_coll_team_t_ *gasnete_coll_team_t;
    typedef gasnete_coll_team_t gasnet_team_handle_t;
    #define GASNET_TEAM_ALL	NULL
#endif

/* Type for collective teams: */
struct gasnete_coll_team_t_ {
    #ifndef GASNETE_COLL_P2P_OVERRIDE
	/* Default implementation of point-to-point syncs
	 * does not (currently) have a team-specific portion.
	 */
    #endif

    /* read-only fields: */
    uint32_t			team_id;

    /* XXX: Design not complete yet */

    /* Hook for conduit-specific extensions/overrides */
    #ifdef GASNETE_COLL_TEAM_EXTRA
      GASNETE_COLL_TEAM_EXTRA
    #endif
};

/*---------------------------------------------------------------------------------*/

/* Function pointer type for polling collective ops: */
typedef int (*gasnete_coll_poll_fn)(gasnete_coll_op_t* GASNETE_THREAD_FARG);

/* Type for collective ops: */
struct gasnete_coll_op_t_ {
    /* Linkage used by the thread-specific active ops list. */
    #ifndef GASNETE_COLL_LIST_OVERRIDE
	/* Default implementation of coll_ops active list */
	gasnete_coll_op_t	*active_next, **active_prev_p;
    #endif

    /* Linkage used by aggregation.
     * Access is serialized by specification+client: */
    #ifndef GASNETE_COLL_AGG_OVERRIDE
	/* Defaule implementation of ops aggregation */
    	gasnete_coll_op_t		*agg_next, *agg_prev, *agg_head;
    #endif

    #if GASNET_PAR
	struct {
	    uint32_t			sequence;
	}			threads;
    #endif

    /* Read-only fields: */
    gasnete_coll_team_t		team;
    uint32_t			sequence;
    int				flags;
    gasnet_coll_handle_t	handle;

    /* Per-instance fields: */
    void			*data;
    gasnete_coll_poll_fn	poll_fn;

    /* Hook for conduit-specific extensions/overrides */
    #ifdef GASNETE_COLL_OP_EXTRA
      GASNETE_COLL_OP_EXTRA
    #endif
};

/*---------------------------------------------------------------------------------*/
/* Type for global synchronization */

#ifndef GASNETE_COLL_CONSENSUS_OVERRIDE
    /* Scalar type, could be a pointer to a struct */
    typedef uint32_t gasnete_coll_consensus_t;
#endif

extern gasnete_coll_consensus_t gasnete_coll_consensus_create(void);
extern int gasnete_coll_consensus_try(gasnete_coll_consensus_t id);

/*---------------------------------------------------------------------------------*/
/* Type for point-to-point synchronization */

#ifndef GASNETE_COLL_P2P_EAGER_SCALE
    /* Number of bytes per-image to allocate for eager data */
    #define GASNETE_COLL_P2P_EAGER_SCALE	16
#endif
#ifndef GASNETE_COLL_P2P_EAGER_MIN
    /* Minumum number of bytes to allocate for eager data */
    #define GASNETE_COLL_P2P_EAGER_MIN		16
#endif

#ifndef GASNETE_COLL_P2P_OVERRIDE
    struct gasnete_coll_p2p_t_ {
	/* Linkage and bookkeeping */
	gasnete_coll_p2p_t	*p2p_next;
	gasnete_coll_p2p_t	*p2p_prev;

	/* Unique (team_id, sequence) tuple for the associated op */
	/* XXX: could play games w/ a single 64-bit field to speed comparisions */
	uint32_t		team_id;
	uint32_t		sequence;

	/* Volatile arrays of data and state for the point-to-point synchronization */
	uint8_t			*data;
	volatile uint32_t	*state;

	/* Handler-safe lock (if needed) */
	gasnet_hsl_t		lock;

	#ifdef GASNETE_COLL_P2P_EXTRA_FIELDS
	  GASNETE_COLL_P2P_EXTRA_FIELDS
	#endif
    };
#endif

extern void gasnete_coll_p2p_init(void);
extern void gasnete_coll_p2p_fini(void);
extern gasnete_coll_p2p_t *gasnete_coll_p2p_get(uint32_t team_id, uint32_t sequence);
extern void gasnete_coll_p2p_destroy(gasnete_coll_p2p_t *p2p);
extern void gasnete_coll_p2p_signalling_put(gasnete_coll_op_t *op, gasnet_node_t dstnode, void *dst,
                                            void *src, size_t nbytes, uint32_t pos, uint32_t state);
extern void gasnete_coll_p2p_signalling_putAsync(gasnete_coll_op_t *op, gasnet_node_t dstnode, void *dst,
						 void *src, size_t nbytes, uint32_t pos, uint32_t state);
extern void gasnete_coll_p2p_change_states(gasnete_coll_op_t *op, gasnet_node_t dstnode,
						 uint32_t count, uint32_t offset, uint32_t state);

/* Treat the eager buffer space at dstnode as an array of elements of length 'size'.
 * Copy 'count' elements to that buffer, starting at element 'offset' at the destination.
 * Set the corresponding entries of the state array to 'state'.
 */
extern void gasnete_coll_p2p_eager_putM(gasnete_coll_op_t *op, gasnet_node_t dstnode,
                                        void *src, uint32_t count, size_t size,
                                        uint32_t offset, uint32_t state);

/* Shorthand for gasnete_coll_p2p_eager_putM with count == 1 */
#ifndef gasnete_coll_p2p_eager_put
  GASNETI_INLINE(gasnete_coll_p2p_eager_put)
  void gasnete_coll_p2p_eager_put(gasnete_coll_op_t *op, gasnet_node_t dstnode,
                                  void *src, size_t size, uint32_t offset, uint32_t state) {
    gasnete_coll_p2p_eager_putM(op, dstnode, src, 1, size, offset, state);
  }
#endif
    
/* Treat the eager buffer space at dstnode as an array of (void *)s.
 * Copy 'count' elements to that buffer, starting at element 'offset' at the destination.
 * Set the corresponding entries of the state array to 'state'.
 */
#ifndef gasnete_coll_p2p_eager_addrM
  GASNETI_INLINE(gasnete_coll_p2p_eager_addrM)
  void gasnete_coll_p2p_eager_addrM(gasnete_coll_op_t *op, gasnet_node_t dstnode,
                                    void * addrlist[], uint32_t count,
				    uint32_t offset, uint32_t state) {
    gasnete_coll_p2p_eager_putM(op, dstnode, addrlist, count, sizeof(void *), offset, state);
  }
#endif

/* Shorthand for gasnete_coll_p2p_eager_addrM with count == 1, taking
 * the address argument by value rather than reference.
 */
#ifndef gasnete_coll_p2p_eager_addr
  GASNETI_INLINE(gasnete_coll_p2p_eager_addr)
  void gasnete_coll_p2p_eager_addr(gasnete_coll_op_t *op, gasnet_node_t dstnode,
                                   void *addr, uint32_t offset, uint32_t state) {
    gasnete_coll_p2p_eager_addrM(op, dstnode, &addr, 1, offset, state);
  }
#endif

/* Treat the eager buffer space on each node as an array of elements of length 'size'.
 * Send (to all but the local node) one element to position 'offset' of that array.
 * Set the corresponding entries of the state array to 'state'.
 * When 'scatter' == 0, the same local element is sent to all nodes (broadcast).
 * When 'scatter' != 0, the source is an array with elements of length 'size', with
 * the ith element sent to node i.
 */
#ifndef gasnete_coll_p2p_eager_put_all
  GASNETI_INLINE(gasnete_coll_p2p_eager_put_all)
  void gasnete_coll_p2p_eager_put_all(gasnete_coll_op_t *op, void *src, size_t size,
				      int scatter, uint32_t offset, uint32_t state) {
    gasnet_node_t i;

    if (scatter) {
      uintptr_t src_addr;

      /* Send to nodes to the "right" of ourself */
      src_addr = (uintptr_t)src + size * (gasneti_mynode + 1);
      for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i, src_addr += size) {
        gasnete_coll_p2p_eager_put(op, i, (void *)src_addr, size, offset, state);
      }
      /* Send to nodes to the "left" of ourself */
      src_addr = (uintptr_t)src;
      for (i = 0; i < gasneti_mynode; ++i, src_addr += size) {
        gasnete_coll_p2p_eager_put(op, i, (void *)src_addr, size, offset, state);
      }
    } else {
      /* Send to nodes to the "right" of ourself */
      for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i) {
        gasnete_coll_p2p_eager_put(op, i, src, size, offset, state);
      }
      /* Send to nodes to the "left" of ourself */
      for (i = 0; i < gasneti_mynode; ++i) {
        gasnete_coll_p2p_eager_put(op, i, src, size, offset, state);
      }
    }
  }
#endif

/* Loop over calls to gasnete_coll_p2p_eager_addr() to send the same
 * address to all nodes except the local node.
 */
#ifndef gasnete_coll_p2p_eager_addr_all
  GASNETI_INLINE(gasnete_coll_p2p_eager_addr_all)
  void gasnete_coll_p2p_eager_addr_all(gasnete_coll_op_t *op, void *addr,
				       uint32_t offset, uint32_t state) {
    gasnet_node_t i;

    /* Send to nodes to the "right" of ourself */
    for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i) {
      gasnete_coll_p2p_eager_addr(op, i, addr, offset, state);
    }
    /* Send to nodes to the "left" of ourself */
    for (i = 0; i < gasneti_mynode; ++i) {
      gasnete_coll_p2p_eager_addr(op, i, addr, offset, state);
    }
  }
#endif

/* Shorthand for gasnete_coll_p2p_change_state w/ count == 1 */
#ifndef gasnete_coll_p2p_change_state
  #define gasnete_coll_p2p_change_state(op, dstnode, offset, state) \
    gasnete_coll_p2p_change_states(op, dstnode, 1, offset, state)
#endif

/*---------------------------------------------------------------------------------*/

/* Helper for scaling of void pointers */
GASNETI_INLINE(gasnete_coll_scale_ptr)
void *gasnete_coll_scale_ptr(const void *ptr, size_t elem_count, size_t elem_size) {
    return (void *)((uintptr_t)ptr + (elem_count * elem_size));
}

/* Helper to perform in-memory broadcast */
GASNETI_INLINE(gasnete_coll_local_broadcast)
void gasnete_coll_local_broadcast(size_t count, void * const dstlist[], const void *src, size_t nbytes) {
    /* XXX: this could/should be segemented to cache reuse */
    while (count--) {
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(*dstlist, src, nbytes);
	dstlist++;
    }
    gasneti_sync_writes();	/* Ensure result is visible on all threads */
}

/* Helper to perform in-memory scatter */
GASNETI_INLINE(gasnete_coll_local_scatter)
void gasnete_coll_local_scatter(size_t count, void * const dstlist[], const void *src, size_t nbytes) {
    const uint8_t *src_addr = (const uint8_t *)src;

    while (count--) {
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(*dstlist, src_addr, nbytes);
	dstlist++;
	src_addr += nbytes;
    }
    gasneti_sync_writes();	/* Ensure result is visible on all threads */
}

/* Helper to perform in-memory gather */
GASNETI_INLINE(gasnete_coll_local_gather)
void gasnete_coll_local_gather(size_t count, void * dst, void * const srclist[], size_t nbytes) {
    uint8_t *dst_addr = (uint8_t *)dst;

    while (count--) {
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(dst_addr, *srclist, nbytes);
	dst_addr += nbytes;
	srclist++;
    }
    gasneti_sync_writes();	/* Ensure result is visible on all threads */
}

/*---------------------------------------------------------------------------------*/
/* Thread-specific data: */
typedef struct {
    gasnet_image_t			my_image;
    gasnet_image_t			my_local_image;
    gasnete_coll_op_t			*op_freelist;
    gasnete_coll_generic_data_t 	*generic_data_freelist;
    gasnete_coll_tree_data_t	 	*tree_data_freelist;

    /* Outstanding handles this thread must sync */
    struct {
	int				    used;
	int				    allocated;
	void				    *array;
    }					handles;

    /* Linkage used by the thread-specific handle freelist . */
    #ifndef GASNETE_COLL_HANDLE_OVERRIDE
	/* Default implementation of handle freelist */
        gasnet_coll_handle_t		handle_freelist;
    #endif

    /* Linkage used by the thread-specific active ops list. */
    #ifndef GASNETE_COLL_LIST_OVERRIDE
	/* Default implementation of coll_ops active list */
    #endif

    struct {
	uint32_t				sequence;
	int					hold_lock;
    } threads;

    /* XXX: more fields to come */

    /* Macro for conduit-specific extension */
    #ifdef GASNETE_COLL_THREADDATA_EXTRA
      GASNETE_COLL_THREADDATA_EXTRA
    #endif
} gasnete_coll_threaddata_t;

extern gasnete_coll_threaddata_t *gasnete_coll_new_threaddata(void);

/* At this point the type gasnete_threaddata_t might not be defined yet.
 * However, we know gasnete_coll_threaddata MUST be the second pointer.
 */
GASNETI_INLINE(_gasnete_coll_get_threaddata)
gasnete_coll_threaddata_t *
_gasnete_coll_get_threaddata(void *thread) {
    struct _prefix_of_gasnete_threaddata {
	void				*reserved_for_core;
	gasnete_coll_threaddata_t	*reserved_for_coll;
	/* We don't care about the rest */
    } *thread_local = (struct _prefix_of_gasnete_threaddata *)thread;
    gasnete_coll_threaddata_t *result = thread_local->reserved_for_coll;

    if_pf (result == NULL)
	thread_local->reserved_for_coll = result = gasnete_coll_new_threaddata();

    return result;
}

/* Used when thread data might not exist yet */
#define GASNETE_COLL_MYTHREAD	_gasnete_coll_get_threaddata(GASNETE_MYTHREAD)

/* Used when thread data must already exist */
#define GASNETE_COLL_MYTHREAD_NOALLOC \
    (gasneti_assert(((void **)GASNETE_MYTHREAD)[1] != NULL), \
     (gasnete_coll_threaddata_t *)(((void **)GASNETE_MYTHREAD)[1]))

/*---------------------------------------------------------------------------------*/

extern gasnete_coll_team_t gasnete_coll_team_lookup(uint32_t team_id);

extern gasnete_coll_op_t *
gasnete_coll_op_create(gasnete_coll_team_t team, uint32_t sequence, int flags GASNETE_THREAD_FARG);
extern void
gasnete_coll_op_destroy(gasnete_coll_op_t *op GASNETE_THREAD_FARG);

/* Aggregation interface: */
extern gasnet_coll_handle_t
gasnete_coll_op_submit(gasnete_coll_op_t *op, gasnet_coll_handle_t handle GASNETE_THREAD_FARG);
extern void gasnete_coll_op_complete(gasnete_coll_op_t *op, int poll_result GASNETE_THREAD_FARG);

extern void gasnete_coll_poll(GASNETE_THREAD_FARG_ALONE);

/*---------------------------------------------------------------------------------*/
/* Debugging and tracing macros */

#if GASNET_DEBUG
  /* Argument validation */
  extern void gasnete_coll_validate(gasnet_team_handle_t team,
                                    gasnet_image_t dstimage, const void *dstaddr, size_t dstlen, int dstisv,
                                    gasnet_image_t srcimage, const void *srcaddr, size_t srclen, int srcisv,
                                    int flags GASNETE_THREAD_FARG);
  #define GASNETE_COLL_VALIDATE(T,DI,DA,DL,DV,SI,SA,SL,SV,F) \
		gasnete_coll_validate(T,DI,DA,DL,DV,SI,SA,SL,SV,F GASNETE_THREAD_PASS)
#else
  #define GASNETE_COLL_VALIDATE(T,DI,DA,DL,DV,SI,SA,SL,SV,F)
#endif

#define GASNETE_COLL_VALIDATE_BROADCAST(T,D,R,S,N,F)   \
	GASNETE_COLL_VALIDATE(T,(gasnet_image_t)(-1),D,N,0,R,S,N,0,F)
#define GASNETE_COLL_VALIDATE_BROADCAST_M(T,D,R,S,N,F)   \
	GASNETE_COLL_VALIDATE(T,(gasnet_image_t)(-1),D,N,1,R,S,N,0,F)

#define GASNETE_COLL_VALIDATE_SCATTER(T,D,R,S,N,F)   \
	GASNETE_COLL_VALIDATE(T,(gasnet_image_t)(-1),D,N,0,R,S,(N)*gasneti_nodes,0,F)
#define GASNETE_COLL_VALIDATE_SCATTER_M(T,D,R,S,N,F)   \
	GASNETE_COLL_VALIDATE(T,(gasnet_image_t)(-1),D,N,1,R,S,(N)*gasneti_nodes,0,F)

#define GASNETE_COLL_VALIDATE_GATHER(T,R,D,S,N,F)     \
	GASNETE_COLL_VALIDATE(T,R,D,(N)*gasneti_nodes,0,(gasnet_image_t)(-1),S,N,0,F)
#define GASNETE_COLL_VALIDATE_GATHER_M(T,R,D,S,N,F)     \
	GASNETE_COLL_VALIDATE(T,R,D,(N)*gasneti_nodes,0,(gasnet_image_t)(-1),S,N,1,F)

#define GASNETE_COLL_VALIDATE_GATHER_ALL(T,D,S,N,F)                \
	GASNETE_COLL_VALIDATE(T,(gasnet_image_t)(-1),D,(N)*gasneti_nodes,0,(gasnet_image_t)(-1),S,N,0,F)
#define GASNETE_COLL_VALIDATE_GATHER_ALL_M(T,D,S,N,F)                \
	GASNETE_COLL_VALIDATE(T,(gasnet_image_t)(-1),D,(N)*gasneti_nodes,1,(gasnet_image_t)(-1),S,N,1,F)

#define GASNETE_COLL_VALIDATE_EXCHANGE(T,D,S,N,F)                  \
        GASNETE_COLL_VALIDATE(T,(gasnet_image_t)(-1),D,(N)*gasneti_nodes,0,(gasnet_image_t)(-1),S,(N)*gasneti_nodes,0,F)
#define GASNETE_COLL_VALIDATE_EXCHANGE_M(T,D,S,N,F)                  \
        GASNETE_COLL_VALIDATE(T,(gasnet_image_t)(-1),D,(N)*gasneti_nodes,1,(gasnet_image_t)(-1),S,(N)*gasneti_nodes,1,F)

/* XXX: following arg validations unimplemented */
#define GASNETE_COLL_VALIDATE_REDUCE(T,DI,D,S,SB,SO,ES,EC,FN,FA,F)
#define GASNETE_COLL_VALIDATE_REDUCE_M(T,DI,D,SL,SB,SO,ES,EC,FN,FA,F)
#define GASNETE_COLL_VALIDATE_SCAN(T,D,DB,DO,S,SB,SO,ES,EC,FN,FA,F)
#define GASNETE_COLL_VALIDATE_SCAN_M(T,DL,DB,DO,SL,SB,SO,ES,EC,FN,FA,F)

/*---------------------------------------------------------------------------------*/
/* In-segment checks */

/* Non-fatal check to determine if a given (node,addr,len) is legal as the
 * source of a gasnete_get*() AND the destination of a gasnete_put*().
 * By default this is just the in-segment bounds checks.
 *
 * However, for a purely AM based conduit this might always be true and other
 * conduits may also override this to allow for regions outside the normal
 * segment.  Note that this override relies on the fact that the gasnete_ calls
 * don't perform bounds checking on their own 
 */
#ifdef gasnete_coll_in_segment
  /* Keep the conduit-specific override */
#elif defined(GASNET_SEGMENT_EVERYTHING) || defined(GASNETI_SUPPORTS_OUTOFSEGMENT_PUTGET)
  #define gasnete_coll_in_segment(_node,_addr,_len)	1
  #define GASNETE_COLL_ALWAYS_IN_SEGMENT 1
#else
  #define gasnete_coll_in_segment(_node,_addr,_len) \
          gasneti_in_fullsegment(_node, _addr, _len)
  #define GASNETE_COLL_ALWAYS_IN_SEGMENT 0
#endif

/* The flags GASNET_COLL_SRC_IN_SEGMENT and GASNET_COLL_DST_IN_SEGMENT are just
 * assertions from the caller.  If they are NOT set, we will try to determine (when
 * possible) if the addresses are in-segment to allow a one-sided implementation
 * to be used.
 * gasnete_coll_segment_check and gasnete_coll_segment_checkM return a new set of flags.
 */
#ifndef gasnete_coll_segment_check
  #if GASNETE_COLL_ALWAYS_IN_SEGMENT
    GASNETI_INLINE(gasnete_coll_segment_check)
    int gasnete_coll_segment_check(int flags, 
                                   int dstrooted, gasnet_image_t dstimage, const void *dst, size_t dstlen,
                                   int srcrooted, gasnet_image_t srcimage, const void *src, size_t srclen) {
      /* Everything is reachable via get/put, regardless of segment */
      return (flags | GASNET_COLL_DST_IN_SEGMENT | GASNET_COLL_SRC_IN_SEGMENT);
    }
  #else
    GASNETI_INLINE(_gasnete_coll_segment_check_aux)
    int _gasnete_coll_segment_check_aux(int rooted, gasnet_image_t root, const void *addr, size_t len) {
      #if GASNET_ALIGNED_SEGMENTS
        /* It is always sufficient to check against node 0. */
        return gasnete_coll_in_segment(0, addr, len);
      #else
        if (rooted) {
	  /* Check the given address against the given node only */
	  return gasnete_coll_in_segment(gasnete_coll_image_node(root), addr, len);
        } else {
	  /* Check the given address against ALL nodes */
	  gasnet_node_t i;
	  for (i = 0; i < gasneti_nodes; ++i) {
	    if (!gasnete_coll_in_segment(i, addr, len)) {
	      return 0;
	    }
	  }
	  return 1;
        }
      #endif
    }

    GASNETI_INLINE(gasnete_coll_segment_check)
    int gasnete_coll_segment_check(int flags, 
                                   int dstrooted, gasnet_image_t dstimage, const void *dst, size_t dstlen,
                                   int srcrooted, gasnet_image_t srcimage, const void *src, size_t srclen) {
      /* Check destination if caller hasn't asserted that it is in-segment */
      if_pf (!(flags & GASNET_COLL_DST_IN_SEGMENT)) {
	if ((flags & GASNET_COLL_SINGLE) && _gasnete_coll_segment_check_aux(dstrooted, dstimage, dst, dstlen)) {
	  flags |= GASNET_COLL_DST_IN_SEGMENT;
	}
      } else {
	/* Cannot gasneti_assert(gasnete_coll_in_segment()) here, since dst might be in AUX segment */
      }
      /* Check source if caller hasn't asserted that it is in-segment */
      if_pf (!(flags & GASNET_COLL_SRC_IN_SEGMENT)) {
	if ((flags & GASNET_COLL_SINGLE) && _gasnete_coll_segment_check_aux(srcrooted, srcimage, src, srclen)) {
	  flags |= GASNET_COLL_SRC_IN_SEGMENT;
	}
      } else {
	/* Cannot gasneti_assert(gasnete_coll_in_segment()) here, since src might be in AUX segment */
      }
      return flags;
    }
  #endif
#endif

#ifndef gasnete_coll_segment_checkM
  #if GASNETE_COLL_ALWAYS_IN_SEGMENT
    GASNETI_INLINE(gasnete_coll_segment_checkM)
    int gasnete_coll_segment_checkM(int flags, 
                                    int dstrooted, gasnet_image_t dstimage, const void *dst, size_t dstlen,
                                    int srcrooted, gasnet_image_t srcimage, const void *src, size_t srclen) {
      /* Everything is reachable via get/put, regardless of segment */
      return (flags | GASNET_COLL_DST_IN_SEGMENT | GASNET_COLL_SRC_IN_SEGMENT);
    }
  #else
    GASNETI_INLINE(_gasnete_coll_segment_checkM_aux)
    int _gasnete_coll_segment_checkM_aux(int rooted, gasnet_image_t root, const void *addr, size_t len) {
      if (rooted) {
        /* Check the given address against the given node only */
        #if GASNET_ALIGNED_SEGMENTS /* always use node 0 for cache reuse */
          return gasnete_coll_in_segment(0, addr, len);
        #else
          return gasnete_coll_in_segment(gasnete_coll_image_node(root), addr, len);
        #endif
      } else {
        /* Check the given addresses against ALL nodes */
        void * const *addrlist = (void * const *)addr;
        gasnet_node_t i;
        for (i = 0; i < gasneti_nodes; ++i) {
	  #if GASNET_ALIGNED_SEGMENTS /* always use node 0 for cache reuse */
            if (!gasnete_coll_in_segment(0, addrlist[i], len)) {
              return 0;
            }
	  #else
            if (!gasnete_coll_in_segment(i, addrlist[i], len)) {
              return 0;
            }
	  #endif
        }
        return 1;
      }
    }

    GASNETI_INLINE(gasnete_coll_segment_checkM)
    int gasnete_coll_segment_checkM(int flags, 
                                    int dstrooted, gasnet_image_t dstimage, const void *dst, size_t dstlen,
                                    int srcrooted, gasnet_image_t srcimage, const void *src, size_t srclen) {
      /* Check destination if caller hasn't asserted that it is in-segment */
      if_pf (!(flags & GASNET_COLL_DST_IN_SEGMENT)) {
	if ((flags & GASNET_COLL_SINGLE) && _gasnete_coll_segment_checkM_aux(dstrooted, dstimage, dst, dstlen)) {
	  flags |= GASNET_COLL_DST_IN_SEGMENT;
	}
      } else {
	/* Cannot gasneti_assert(gasnete_coll_in_segment()) here, since dst might be in AUX segment */
      }
      /* Check source if caller hasn't asserted that it is in-segment */
      if_pf (!(flags & GASNET_COLL_SRC_IN_SEGMENT)) {
	if ((flags & GASNET_COLL_SINGLE) && _gasnete_coll_segment_checkM_aux(srcrooted, srcimage, src, srclen)) {
	  flags |= GASNET_COLL_SRC_IN_SEGMENT;
	}
      } else {
	/* Cannot gasneti_assert(gasnete_coll_in_segment()) here, since src might be in AUX segment */
      }
      return flags;
    }
  #endif
#endif

/*------------------------------------------------------------------------------------*/

/* gasnet_coll_init: Initialize GASNet collectives
 *
 *  images:     Array of gasnet_nodes() elements giving the number of
 *              images present on each node.  This must have the
 *              same contents on all nodes or the behavior is undefined.
 *		If NULL, then there is one image per node.
 *		In GASNET_SEQ mode, NULL is the only legal value.
 *  my_image:   If 'images' is non-NULL, this gives the image number of
 *              the calling thread.
 *  fn_tbl:     An array of type gasnet_coll_fn_entry_t, specifying
 *              the functions which can be invoked for the
 *              computational collectives.  This may safely differ
 *              in contents (but not size) across nodes.
 *              Upon return the 'handle' field of each entry is set
 *              to the value that must be passed to the collective.
 *  fn_count:   The number of entries in 'fn_tbl'.  Must agree across
 *              all nodes or the behavior is undefined.
 *  init_flags: Presently unused.  Must be 0.
 */
#ifndef gasnet_coll_init
  extern void gasnete_coll_init(const gasnet_image_t images[], gasnet_image_t my_image,
		  		gasnet_coll_fn_entry_t fn_tbl[], size_t fn_count,
		  		int init_flags GASNETE_THREAD_FARG);
  #define gasnet_coll_init(im,mi,fn,fc,fl) \
		gasnete_coll_init(im,mi,fn,fc,fl GASNETE_THREAD_GET)
#endif

/*---------------------------------------------------------------------------------*/

extern int gasnete_coll_try_sync(gasnet_coll_handle_t handle GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_try_sync)
int _gasnet_coll_try_sync(gasnet_coll_handle_t handle GASNETE_THREAD_FARG) {
  int result = GASNET_OK;
  if_pt (handle != GASNET_COLL_INVALID_HANDLE) {
    result = gasnete_coll_try_sync(handle GASNETE_THREAD_PASS);
    if (result)
      gasneti_sync_reads();
  }
  GASNETI_TRACE_COLL_TRYSYNC(COLL_TRY_SYNC,result);
  return result;
}
#define gasnet_coll_try_sync(handle) \
       _gasnet_coll_try_sync(handle GASNETE_THREAD_GET)

extern int gasnete_coll_try_sync_some(gasnet_coll_handle_t *phandle, size_t numhandles GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_try_sync_some)
int _gasnet_coll_try_sync_some(gasnet_coll_handle_t *phandle, size_t numhandles GASNETE_THREAD_FARG) {
  int result = gasnete_coll_try_sync_some(phandle, numhandles GASNETE_THREAD_PASS);
  if (result)
    gasneti_sync_reads();
  GASNETI_TRACE_COLL_TRYSYNC(COLL_TRY_SYNC_SOME,result);
  return result;
}
#define gasnet_coll_try_sync_some(phandle,numhandles) \
       _gasnet_coll_try_sync_some(phandle,numhandles GASNETE_THREAD_GET)

extern int gasnete_coll_try_sync_all(gasnet_coll_handle_t *phandle, size_t numhandles GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_try_sync_all)
int _gasnet_coll_try_sync_all(gasnet_coll_handle_t *phandle, size_t numhandles GASNETE_THREAD_FARG) {
  int result = gasnete_coll_try_sync_all(phandle, numhandles GASNETE_THREAD_PASS);
  if (result)
    gasneti_sync_reads();
  GASNETI_TRACE_COLL_TRYSYNC(COLL_TRY_SYNC_ALL,result);
  return result;
}
#define gasnet_coll_try_sync_all(phandle,numhandles) \
       _gasnet_coll_try_sync_all(phandle,numhandles GASNETE_THREAD_GET)

#ifdef gasnete_coll_wait_sync
  extern void
  gasnete_coll_wait_sync(gasnet_coll_handle_t handle GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_wait_sync)
  void gasnete_coll_wait_sync(gasnet_coll_handle_t handle GASNETE_THREAD_FARG) {
    if_pt (handle != GASNET_COLL_INVALID_HANDLE) {
      gasneti_waitwhile(gasnete_coll_try_sync(handle GASNETE_THREAD_PASS) == GASNET_ERR_NOT_READY);
    }
  }
#endif
GASNETI_INLINE(_gasnet_coll_wait_sync)
void _gasnet_coll_wait_sync(gasnet_coll_handle_t handle GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_WAITSYNC_BEGIN();
  gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  GASNETI_TRACE_COLL_WAITSYNC_END(COLL_WAIT_SYNC);
}
#define gasnet_coll_wait_sync(handle) \
       _gasnet_coll_wait_sync(handle GASNETE_THREAD_GET)

#ifdef gasnete_coll_wait_sync_some
  extern void
  gasnete_coll_wait_sync_some(gasnet_coll_handle_t *phandle, size_t numhandles GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_wait_sync_some)
  void gasnete_coll_wait_sync_some(gasnet_coll_handle_t *phandle, size_t numhandles GASNETE_THREAD_FARG) {
    gasneti_waitwhile(gasnete_coll_try_sync_some(phandle,numhandles GASNETE_THREAD_PASS) == GASNET_ERR_NOT_READY);
  }
#endif
GASNETI_INLINE(_gasnet_coll_wait_sync_some)
void _gasnet_coll_wait_sync_some(gasnet_coll_handle_t *phandle, size_t numhandles GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_WAITSYNC_BEGIN();
  gasnete_coll_wait_sync_some(phandle,numhandles GASNETE_THREAD_PASS);
  GASNETI_TRACE_COLL_WAITSYNC_END(COLL_WAIT_SYNC_SOME);
}
#define gasnet_coll_wait_sync_some(phandle,numhandles) \
       _gasnet_coll_wait_sync_some(phandle,numhandles GASNETE_THREAD_GET)

#ifdef gasnete_coll_wait_sync_all
  extern void
  gasnete_coll_wait_sync_all(gasnet_coll_handle_t *phandle, size_t numhandles GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_wait_sync_all)
  void gasnete_coll_wait_sync_all(gasnet_coll_handle_t *phandle, size_t numhandles GASNETE_THREAD_FARG) {
    gasneti_waitwhile(gasnete_coll_try_sync_all(phandle,numhandles GASNETE_THREAD_PASS) == GASNET_ERR_NOT_READY);
  }
#endif
GASNETI_INLINE(_gasnet_coll_wait_sync_all)
void _gasnet_coll_wait_sync_all(gasnet_coll_handle_t *phandle, size_t numhandles GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_WAITSYNC_BEGIN();
  gasnete_coll_wait_sync_all(phandle,numhandles GASNETE_THREAD_PASS);
  GASNETI_TRACE_COLL_WAITSYNC_END(COLL_WAIT_SYNC_ALL);
}
#define gasnet_coll_wait_sync_all(phandle,numhandles) \
       _gasnet_coll_wait_sync_all(phandle,numhandles GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_coll_broadcast_nb
  #define gasnete_coll_broadcast_nb gasnete_coll_broadcast_nb_default
#else
  extern gasnet_coll_handle_t
  gasnete_coll_broadcast_nb_default(gasnet_team_handle_t team,
                                    void *dst,
                                    gasnet_image_t srcimage, void *src,
                                    size_t nbytes, int flags, uint32_t sequence
                                    GASNETE_THREAD_FARG);
#endif
extern gasnet_coll_handle_t
gasnete_coll_broadcast_nb(gasnet_team_handle_t team,
                          void *dst,
                          gasnet_image_t srcimage, void *src,
                          size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_broadcast_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_coll_handle_t
_gasnet_coll_broadcast_nb(gasnet_team_handle_t team,
                          void *dst,
                          gasnet_image_t srcimage, void *src,
                          size_t nbytes, int flags GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t handle;
  GASNETI_TRACE_COLL_BROADCAST(COLL_BROADCAST_NB,team,dst,srcimage,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_BROADCAST(team,dst,srcimage,src,nbytes,flags);
  handle = gasnete_coll_broadcast_nb(team,dst,srcimage,src,nbytes,flags,0 GASNETE_THREAD_PASS);
  gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);
  return handle;
}
#define gasnet_coll_broadcast_nb(team,dst,srcimage,src,nbytes,flags) \
       _gasnet_coll_broadcast_nb(team,dst,srcimage,src,nbytes,flags GASNETE_THREAD_GET)

#ifdef gasnete_coll_broadcast
  extern gasnet_coll_handle_t
  gasnete_coll_broadcast(gasnet_team_handle_t team,
                         void *dst,
                         gasnet_image_t srcimage, void *src,
                         size_t nbytes, int flags GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_broadcast)
  void gasnete_coll_broadcast(gasnet_team_handle_t team,
                              void *dst,
                              gasnet_image_t srcimage, void *src,
                              size_t nbytes, int flags GASNETE_THREAD_FARG) {
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_broadcast_nb(team,dst,srcimage,src,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  }
#endif
GASNETI_INLINE(_gasnet_coll_broadcast)
void _gasnet_coll_broadcast(gasnet_team_handle_t team,
                            void *dst,
                            gasnet_image_t srcimage, void *src,
                            size_t nbytes, int flags GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_BROADCAST(COLL_BROADCAST,team,dst,srcimage,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_BROADCAST(team,dst,srcimage,src,nbytes,flags);
  gasnete_coll_broadcast(team,dst,srcimage,src,nbytes,flags GASNETE_THREAD_PASS);
}
#define gasnet_coll_broadcast(team,dst,srcimage,src,nbytes,flags) \
       _gasnet_coll_broadcast(team,dst,srcimage,src,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_coll_broadcastM_nb
  #define gasnete_coll_broadcastM_nb gasnete_coll_broadcastM_nb_default
#else
  extern gasnet_coll_handle_t
  gasnete_coll_broadcastM_nb_default(gasnet_team_handle_t team,
                                     void * const dstlist[],
                                     gasnet_image_t srcimage, void *src,
                                     size_t nbytes, int flags, uint32_t sequence
                                     GASNETE_THREAD_FARG);
#endif
extern gasnet_coll_handle_t
gasnete_coll_broadcastM_nb(gasnet_team_handle_t team,
                           void * const dstlist[],
                           gasnet_image_t srcimage, void *src,
                           size_t nbytes, int flags, uint32_t sequence
                           GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_broadcastM_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_coll_handle_t
_gasnet_coll_broadcastM_nb(gasnet_team_handle_t team,
                           void * const dstlist[],
                           gasnet_image_t srcimage, void *src,
                           size_t nbytes, int flags GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t handle;
  GASNETI_TRACE_COLL_BROADCAST_M(COLL_BROADCAST_M_NB,team,dstlist,srcimage,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_BROADCAST_M(team,dstlist,srcimage,src,nbytes,flags);
  handle = gasnete_coll_broadcastM_nb(team,dstlist,srcimage,src,nbytes,flags,0 GASNETE_THREAD_PASS);
  gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);
  return handle;
}
#define gasnet_coll_broadcastM_nb(team,dstlist,srcimage,src,nbytes,flags) \
       _gasnet_coll_broadcastM_nb(team,dstlist,srcimage,src,nbytes,flags GASNETE_THREAD_GET)

#ifdef gasnete_coll_broadcastM
  extern void
  gasnete_coll_broadcastM(gasnet_team_handle_t team,
                          void * const dstlist[],
                          gasnet_image_t srcimage, void *src,
                          size_t nbytes, int flags GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_broadcastM)
  void gasnete_coll_broadcastM(gasnet_team_handle_t team,
                               void * const dstlist[],
                               gasnet_image_t srcimage, void *src,
                               size_t nbytes, int flags GASNETE_THREAD_FARG) {
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_broadcastM_nb(team,dstlist,srcimage,src,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  }
#endif
GASNETI_INLINE(_gasnet_coll_broadcastM)
void _gasnet_coll_broadcastM(gasnet_team_handle_t team,
                             void * const dstlist[],
                             gasnet_image_t srcimage, void *src,
                             size_t nbytes, int flags GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_BROADCAST_M(COLL_BROADCAST_M,team,dstlist,srcimage,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_BROADCAST_M(team,dstlist,srcimage,src,nbytes,flags);
  gasnete_coll_broadcastM(team,dstlist,srcimage,src,nbytes,flags GASNETE_THREAD_PASS);
}
#define gasnet_coll_broadcastM(team,dstlist,srcimage,src,nbytes,flags) \
       _gasnet_coll_broadcastM(team,dstlist,srcimage,src,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_coll_scatter_nb
  #define gasnete_coll_scatter_nb gasnete_coll_scatter_nb_default
#else
  extern gasnet_coll_handle_t
  gasnete_coll_scatter_nb_default(gasnet_team_handle_t team,
                                  void *dst,
                                  gasnet_image_t srcimage, void *src,
                                  size_t nbytes, int flags, uint32_t sequence
                                  GASNETE_THREAD_FARG);
#endif
extern gasnet_coll_handle_t
gasnete_coll_scatter_nb(gasnet_team_handle_t team,
                        void *dst,
                        gasnet_image_t srcimage, void *src,
                        size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_scatter_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_coll_handle_t
_gasnet_coll_scatter_nb(gasnet_team_handle_t team,
                        void *dst,
                        gasnet_image_t srcimage, void *src,
                        size_t nbytes, int flags GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t handle;
  GASNETI_TRACE_COLL_SCATTER(COLL_SCATTER_NB,team,dst,srcimage,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_SCATTER(team,dst,srcimage,src,nbytes,flags);
  handle = gasnete_coll_scatter_nb(team,dst,srcimage,src,nbytes,flags,0 GASNETE_THREAD_PASS);
  gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);
  return handle;
}
#define gasnet_coll_scatter_nb(team,dst,srcimage,src,nbytes,flags) \
       _gasnet_coll_scatter_nb(team,dst,srcimage,src,nbytes,flags GASNETE_THREAD_GET)

#ifdef gasnete_coll_scatter
  extern void
  gasnete_coll_scatter(gasnet_team_handle_t team,
                       void *dst,
                       gasnet_image_t srcimage, void *src,
                       size_t nbytes, int flags GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_scatter)
  void gasnete_coll_scatter(gasnet_team_handle_t team,
                            void *dst,
                            gasnet_image_t srcimage, void *src,
                            size_t nbytes, int flags GASNETE_THREAD_FARG) {
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_scatter_nb(team,dst,srcimage,src,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  }
#endif
GASNETI_INLINE(_gasnet_coll_scatter)
void _gasnet_coll_scatter(gasnet_team_handle_t team,
                          void *dst,
                          gasnet_image_t srcimage, void *src,
                          size_t nbytes, int flags GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_SCATTER(COLL_SCATTER,team,dst,srcimage,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_SCATTER(team,dst,srcimage,src,nbytes,flags);
  gasnete_coll_scatter(team,dst,srcimage,src,nbytes,flags GASNETE_THREAD_PASS);
}
#define gasnet_coll_scatter(team,dst,srcimage,src,nbytes,flags) \
       _gasnet_coll_scatter(team,dst,srcimage,src,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_coll_scatterM_nb
  #define gasnete_coll_scatterM_nb gasnete_coll_scatterM_nb_default
#else
  extern gasnet_coll_handle_t
  gasnete_coll_scatterM_nb_default(gasnet_team_handle_t team,
                                   void * const dstlist[],
                                   gasnet_image_t srcimage, void *src,
                                   size_t nbytes, int flags, uint32_t sequence
                                   GASNETE_THREAD_FARG);
#endif
extern gasnet_coll_handle_t
gasnete_coll_scatterM_nb(gasnet_team_handle_t team,
                         void * const dstlist[],
                         gasnet_image_t srcimage, void *src,
                         size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_scatterM_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_coll_handle_t
_gasnet_coll_scatterM_nb(gasnet_team_handle_t team,
                         void * const dstlist[],
                         gasnet_image_t srcimage, void *src,
                         size_t nbytes, int flags GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t handle;
  GASNETI_TRACE_COLL_SCATTER_M(COLL_SCATTER_M_NB,team,dstlist,srcimage,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_SCATTER_M(team,dstlist,srcimage,src,nbytes,flags);
  handle = gasnete_coll_scatterM_nb(team,dstlist,srcimage,src,nbytes,flags,0 GASNETE_THREAD_PASS);
  gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);
  return handle;
}
#define gasnet_coll_scatterM_nb(team,dstlist,srcimage,src,nbytes,flags) \
       _gasnet_coll_scatterM_nb(team,dstlist,srcimage,src,nbytes,flags GASNETE_THREAD_GET)

#ifdef gasnete_coll_scatterM
  extern void
  gasnete_coll_scatterM(gasnet_team_handle_t team,
                        void * const dstlist[],
                        gasnet_image_t srcimage, void *src,
                        size_t nbytes, int flags GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_scatterM)
  void gasnete_coll_scatterM(gasnet_team_handle_t team,
                             void * const dstlist[],
                             gasnet_image_t srcimage, void *src,
                             size_t nbytes, int flags GASNETE_THREAD_FARG) {
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_scatterM_nb(team,dstlist,srcimage,src,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  }
#endif
GASNETI_INLINE(_gasnet_coll_scatterM)
void _gasnet_coll_scatterM(gasnet_team_handle_t team,
                           void * const dstlist[],
                           gasnet_image_t srcimage, void *src,
                           size_t nbytes, int flags GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_SCATTER_M(COLL_SCATTER_M,team,dstlist,srcimage,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_SCATTER_M(team,dstlist,srcimage,src,nbytes,flags);
  gasnete_coll_scatterM(team,dstlist,srcimage,src,nbytes,flags GASNETE_THREAD_PASS);
}
#define gasnet_coll_scatterM(team,dstlist,srcimage,src,nbytes,flags) \
       _gasnet_coll_scatterM(team,dstlist,srcimage,src,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_coll_gather_nb
  #define gasnete_coll_gather_nb gasnete_coll_gather_nb_default
#else
  extern gasnet_coll_handle_t
  gasnete_coll_gather_nb_default(gasnet_team_handle_t team,
                                 gasnet_image_t dstimage, void *dst,
                                 void *src,
                                 size_t nbytes, int flags, uint32_t sequence
                                 GASNETE_THREAD_FARG);
#endif
extern gasnet_coll_handle_t
gasnete_coll_gather_nb(gasnet_team_handle_t team,
                       gasnet_image_t dstimage, void *dst,
                       void *src,
                       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_gather_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_coll_handle_t
_gasnet_coll_gather_nb(gasnet_team_handle_t team,
                       gasnet_image_t dstimage, void *dst,
                       void *src,
                       size_t nbytes, int flags GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t handle;
  GASNETI_TRACE_COLL_GATHER(COLL_GATHER_NB,team,dstimage,dst,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_GATHER(team,dstimage,dst,src,nbytes,flags);
  handle = gasnete_coll_gather_nb(team,dstimage,dst,src,nbytes,flags,0 GASNETE_THREAD_PASS);
  gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);
  return handle;
}
#define gasnet_coll_gather_nb(team,dstimage,dst,src,nbytes,flags) \
       _gasnet_coll_gather_nb(team,dstimage,dst,src,nbytes,flags GASNETE_THREAD_GET)

#ifdef gasnete_coll_gather
  extern void
  gasnete_coll_gather(gasnet_team_handle_t team,
                      gasnet_image_t dstimage, void *dst,
                      void *src,
                      size_t nbytes, int flags GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_gather)
  void gasnete_coll_gather(gasnet_team_handle_t team,
                           gasnet_image_t dstimage, void *dst,
                           void *src,
                           size_t nbytes, int flags GASNETE_THREAD_FARG) {
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_gather_nb(team,dstimage,dst,src,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  }
#endif
GASNETI_INLINE(_gasnet_coll_gather)
void _gasnet_coll_gather(gasnet_team_handle_t team,
                         gasnet_image_t dstimage, void *dst,
                         void *src,
                         size_t nbytes, int flags GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_GATHER(COLL_GATHER,team,dstimage,dst,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_GATHER(team,dstimage,dst,src,nbytes,flags);
  gasnete_coll_gather(team,dstimage,dst,src,nbytes,flags GASNETE_THREAD_PASS);
}
#define gasnet_coll_gather(team,dstimage,dst,src,nbytes,flags) \
       _gasnet_coll_gather(team,dstimage,dst,src,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_coll_gatherM_nb
  #define gasnete_coll_gatherM_nb gasnete_coll_gatherM_nb_default
#else
  extern gasnet_coll_handle_t
  gasnete_coll_gatherM_nb_default(gasnet_team_handle_t team,
                                  gasnet_image_t dstimage, void *dst,
                                  void * const srclist[],
                                  size_t nbytes, int flags, uint32_t sequence
                                  GASNETE_THREAD_FARG);
#endif
extern gasnet_coll_handle_t
gasnete_coll_gatherM_nb(gasnet_team_handle_t team,
                        gasnet_image_t dstimage, void *dst,
                        void * const srclist[],
                        size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_gatherM_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_coll_handle_t
_gasnet_coll_gatherM_nb(gasnet_team_handle_t team,
                        gasnet_image_t dstimage, void *dst,
                        void * const srclist[],
                        size_t nbytes, int flags GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t handle;
  GASNETI_TRACE_COLL_GATHER_M(COLL_GATHER_M_NB,team,dstimage,dst,srclist,nbytes,flags);
  GASNETE_COLL_VALIDATE_GATHER_M(team,dstimage,dst,srclist,nbytes,flags);
  handle = gasnete_coll_gatherM_nb(team,dstimage,dst,srclist,nbytes,flags,0 GASNETE_THREAD_PASS);
  gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);
  return handle;
}
#define gasnet_coll_gatherM_nb(team,dstimage,dst,srclist,nbytes,flags) \
       _gasnet_coll_gatherM_nb(team,dstimage,dst,srclist,nbytes,flags GASNETE_THREAD_GET)

#ifdef gasnete_coll_gatherM
  extern void
  gasnete_coll_gatherM(gasnet_team_handle_t team,
                       gasnet_image_t dstimage, void *dst,
                       void * const srclist[],
                       size_t nbytes, int flags GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_gatherM)
  void gasnete_coll_gatherM(gasnet_team_handle_t team,
                            gasnet_image_t dstimage, void *dst,
                            void * const srclist[],
                            size_t nbytes, int flags GASNETE_THREAD_FARG) {
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_gatherM_nb(team,dstimage,dst,srclist,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  }
#endif
GASNETI_INLINE(_gasnet_coll_gatherM)
void _gasnet_coll_gatherM(gasnet_team_handle_t team,
                          gasnet_image_t dstimage, void *dst,
                          void * const srclist[],
                          size_t nbytes, int flags GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_GATHER_M(COLL_GATHER_M,team,dstimage,dst,srclist,nbytes,flags);
  GASNETE_COLL_VALIDATE_GATHER_M(team,dstimage,dst,srclist,nbytes,flags);
  gasnete_coll_gatherM(team,dstimage,dst,srclist,nbytes,flags GASNETE_THREAD_PASS);
}
#define gasnet_coll_gatherM(team,dstimage,dst,srclist,nbytes,flags) \
       _gasnet_coll_gatherM(team,dstimage,dst,srclist,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_coll_gather_all_nb
  #define gasnete_coll_gather_all_nb gasnete_coll_gather_all_nb_default
#else
  extern gasnet_coll_handle_t
  gasnete_coll_gather_all_nb_default(gasnet_team_handle_t team,
                                     void *dst, void *src,
                                     size_t nbytes, int flags, uint32_t sequence
                                     GASNETE_THREAD_FARG);
#endif
extern gasnet_coll_handle_t
gasnete_coll_gather_all_nb(gasnet_team_handle_t team,
                           void *dst, void *src,
                           size_t nbytes, int flags, uint32_t sequence
                           GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_gather_all_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_coll_handle_t
_gasnet_coll_gather_all_nb(gasnet_team_handle_t team,
                           void *dst, void *src,
                           size_t nbytes, int flags GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t handle;
  GASNETI_TRACE_COLL_GATHER_ALL(COLL_GATHER_ALL_NB,team,dst,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_GATHER_ALL(team,dst,src,nbytes,flags);
  handle = gasnete_coll_gather_all_nb(team,dst,src,nbytes,flags,0 GASNETE_THREAD_PASS);
  gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);
  return handle;
}
#define gasnet_coll_gather_all_nb(team,dst,src,nbytes,flags) \
       _gasnet_coll_gather_all_nb(team,dst,src,nbytes,flags GASNETE_THREAD_GET)

#ifdef gasnete_coll_gather_all
  extern void
  gasnete_coll_gather_all(gasnet_team_handle_t team,
                          void *dst, void *src,
                          size_t nbytes, int flags GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_gather_all)
  void gasnete_coll_gather_all(gasnet_team_handle_t team,
                               void *dst, void *src,
                               size_t nbytes, int flags GASNETE_THREAD_FARG) {
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_gather_all_nb(team,dst,src,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  }
#endif
GASNETI_INLINE(_gasnet_coll_gather_all)
void _gasnet_coll_gather_all(gasnet_team_handle_t team,
                             void *dst, void *src,
                             size_t nbytes, int flags GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_GATHER_ALL(COLL_GATHER_ALL,team,dst,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_GATHER_ALL(team,dst,src,nbytes,flags);
  gasnete_coll_gather_all(team,dst,src,nbytes,flags GASNETE_THREAD_PASS);
}
#define gasnet_coll_gather_all(team,dst,src,nbytes,flags) \
       _gasnet_coll_gather_all(team,dst,src,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_coll_gather_allM_nb
  #define gasnete_coll_gather_allM_nb gasnete_coll_gather_allM_nb_default
#else
  extern gasnet_coll_handle_t
  gasnete_coll_gather_allM_nb_default(gasnet_team_handle_t team,
                            void * const dstlist[], void * const srclist[],
                            size_t nbytes, int flags, uint32_t sequence
                            GASNETE_THREAD_FARG);
#endif
extern gasnet_coll_handle_t
gasnete_coll_gather_allM_nb(gasnet_team_handle_t team,
                            void * const dstlist[], void * const srclist[],
                            size_t nbytes, int flags, uint32_t sequence
                            GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_gather_allM_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_coll_handle_t
_gasnet_coll_gather_allM_nb(gasnet_team_handle_t team,
                            void * const dstlist[], void * const srclist[],
                            size_t nbytes, int flags GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t handle;
  GASNETI_TRACE_COLL_GATHER_ALL_M(COLL_GATHER_ALL_M_NB,team,dstlist,srclist,nbytes,flags);
  GASNETE_COLL_VALIDATE_GATHER_ALL_M(team,dstlist,srclist,nbytes,flags);
  handle = gasnete_coll_gather_allM_nb(team,dstlist,srclist,nbytes,flags,0 GASNETE_THREAD_PASS);
  gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);
  return handle;
}
#define gasnet_coll_gather_allM_nb(team,dstlist,srclist,nbytes,flags) \
       _gasnet_coll_gather_allM_nb(team,dstlist,srclist,nbytes,flags GASNETE_THREAD_GET)

#ifdef gasnete_coll_gather_allM
  extern void
  gasnete_coll_gather_allM(gasnet_team_handle_t team,
                           void * const dstlist[], void * const srclist[],
                           size_t nbytes, int flags GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_gather_allM)
  void gasnete_coll_gather_allM(gasnet_team_handle_t team,
                                void * const dstlist[], void * const srclist[],
                                size_t nbytes, int flags GASNETE_THREAD_FARG) {
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_gather_allM_nb(team,dstlist,srclist,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  }
#endif
GASNETI_INLINE(_gasnet_coll_gather_allM)
void _gasnet_coll_gather_allM(gasnet_team_handle_t team,
                              void * const dstlist[], void * const srclist[],
                              size_t nbytes, int flags GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_GATHER_ALL_M(COLL_GATHER_ALL_M,team,dstlist,srclist,nbytes,flags);
  GASNETE_COLL_VALIDATE_GATHER_ALL_M(team,dstlist,srclist,nbytes,flags);
  gasnete_coll_gather_allM(team,dstlist,srclist,nbytes,flags GASNETE_THREAD_PASS);
}
#define gasnet_coll_gather_allM(team,dstlist,srclist,nbytes,flags) \
       _gasnet_coll_gather_allM(team,dstlist,srclist,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_coll_exchange_nb
  #define gasnete_coll_exchange_nb gasnete_coll_exchange_nb_default
#else
  extern gasnet_coll_handle_t
  gasnete_coll_exchange_nb_default(gasnet_team_handle_t team,
                         void *dst, void *src,
                         size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG);
#endif
extern gasnet_coll_handle_t
gasnete_coll_exchange_nb(gasnet_team_handle_t team,
                         void *dst, void *src,
                         size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_exchange_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_coll_handle_t
_gasnet_coll_exchange_nb(gasnet_team_handle_t team,
                         void *dst, void *src,
                         size_t nbytes, int flags GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t handle;
  GASNETI_TRACE_COLL_EXCHANGE(COLL_EXCHANGE_NB,team,dst,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_EXCHANGE(team,dst,src,nbytes,flags);
  handle = gasnete_coll_exchange_nb(team,dst,src,nbytes,flags,0 GASNETE_THREAD_PASS);
  gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);
  return handle;
}
#define gasnet_coll_exchange_nb(team,dst,src,nbytes,flags) \
       _gasnet_coll_exchange_nb(team,dst,src,nbytes,flags GASNETE_THREAD_GET)

#ifdef gasnete_coll_exchange
  extern void
  gasnete_coll_exchange(gasnet_team_handle_t team,
                        void *dst, void *src,
                        size_t nbytes, int flags GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_exchange)
  void gasnete_coll_exchange(gasnet_team_handle_t team,
                             void *dst, void *src,
                             size_t nbytes, int flags GASNETE_THREAD_FARG) {
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_exchange_nb(team,dst,src,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  }
#endif
GASNETI_INLINE(_gasnet_coll_exchange)
void _gasnet_coll_exchange(gasnet_team_handle_t team,
                           void *dst, void *src,
                           size_t nbytes, int flags GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_EXCHANGE(COLL_EXCHANGE,team,dst,src,nbytes,flags);
  GASNETE_COLL_VALIDATE_EXCHANGE(team,dst,src,nbytes,flags);
  gasnete_coll_exchange(team,dst,src,nbytes,flags GASNETE_THREAD_PASS);
}
#define gasnet_coll_exchange(team,dst,src,nbytes,flags) \
       _gasnet_coll_exchange(team,dst,src,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_coll_exchangeM_nb
  #define gasnete_coll_exchangeM_nb gasnete_coll_exchangeM_nb_default
#else
  extern gasnet_coll_handle_t
  gasnete_coll_exchangeM_nb_default(gasnet_team_handle_t team,
                          void * const dstlist[], void * const srclist[],
                          size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG);
#endif
extern gasnet_coll_handle_t
gasnete_coll_exchangeM_nb(gasnet_team_handle_t team,
                          void * const dstlist[], void * const srclist[],
                          size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_exchangeM_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_coll_handle_t
_gasnet_coll_exchangeM_nb(gasnet_team_handle_t team,
                          void * const dstlist[], void * const srclist[],
                          size_t nbytes, int flags GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t handle;
  GASNETI_TRACE_COLL_EXCHANGE_M(COLL_EXCHANGE_M_NB,team,dstlist,srclist,nbytes,flags);
  GASNETE_COLL_VALIDATE_EXCHANGE_M(team,dstlist,srclist,nbytes,flags);
  handle = gasnete_coll_exchangeM_nb(team,dstlist,srclist,nbytes,flags,0 GASNETE_THREAD_PASS);
  gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);
  return handle;
}
#define gasnet_coll_exchangeM_nb(team,dstlist,srclist,nbytes,flags) \
       _gasnet_coll_exchangeM_nb(team,dstlist,srclist,nbytes,flags GASNETE_THREAD_GET)

#ifdef gasnete_coll_exchangeM
  extern void
  gasnete_coll_exchangeM(gasnet_team_handle_t team,
                         void * const dstlist[], void * const srclist[],
                         size_t nbytes, int flags GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_exchangeM)
  void gasnete_coll_exchangeM(gasnet_team_handle_t team,
                              void * const dstlist[], void * const srclist[],
                              size_t nbytes, int flags GASNETE_THREAD_FARG) {
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_exchangeM_nb(team,dstlist,srclist,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  }
#endif
GASNETI_INLINE(_gasnet_coll_exchangeM)
void _gasnet_coll_exchangeM(gasnet_team_handle_t team,
                            void * const dstlist[], void * const srclist[],
                            size_t nbytes, int flags GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_EXCHANGE_M(COLL_EXCHANGE_M,team,dstlist,srclist,nbytes,flags);
  GASNETE_COLL_VALIDATE_EXCHANGE_M(team,dstlist,srclist,nbytes,flags);
  gasnete_coll_exchangeM(team,dstlist,srclist,nbytes,flags GASNETE_THREAD_PASS);
}
#define gasnet_coll_exchangeM(team,dstlist,srclist,nbytes,flags) \
       _gasnet_coll_exchangeM(team,dstlist,srclist,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_coll_reduce_nb
  #define gasnete_coll_reduce_nb gasnete_coll_reduce_nb_default
#else
  extern gasnet_coll_handle_t
  gasnete_coll_reduce_nb_default(gasnet_team_handle_t team,
                       gasnet_image_t dstimage, void *dst,
                       void *src, size_t src_blksz, size_t src_offset,
                       size_t elem_size, size_t elem_count,
                       gasnet_coll_fn_handle_t func, int func_arg,
                       int flags GASNETE_THREAD_FARG);
#endif
extern gasnet_coll_handle_t
gasnete_coll_reduce_nb(gasnet_team_handle_t team,
                       gasnet_image_t dstimage, void *dst,
                       void *src, size_t src_blksz, size_t src_offset,
                       size_t elem_size, size_t elem_count,
                       gasnet_coll_fn_handle_t func, int func_arg,
                       int flags GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_reduce_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_coll_handle_t
_gasnet_coll_reduce_nb(gasnet_team_handle_t team,
                       gasnet_image_t dstimage, void *dst,
                       void *src, size_t src_blksz, size_t src_offset,
                       size_t elem_size, size_t elem_count,
                       gasnet_coll_fn_handle_t func, int func_arg,
                       int flags GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t handle;
  GASNETI_TRACE_COLL_REDUCE(COLL_REDUCE_NB,team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags);
  GASNETE_COLL_VALIDATE_REDUCE(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags);
  handle = gasnete_coll_reduce_nb(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_PASS);
  gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);
  return handle;
}
#define gasnet_coll_reduce_nb(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_reduce_nb(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_GET)

#ifdef gasnete_coll_reduce
  extern void
  gasnete_coll_reduce(gasnet_team_handle_t team,
                      gasnet_image_t dstimage, void *dst,
                      void *src, size_t src_blksz, size_t src_offset,
                      size_t elem_size, size_t elem_count,
                      gasnet_coll_fn_handle_t func, int func_arg,
                      int flags GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_reduce)
  void gasnete_coll_reduce(gasnet_team_handle_t team,
                           gasnet_image_t dstimage, void *dst,
                           void *src, size_t src_blksz, size_t src_offset,
                           size_t elem_size, size_t elem_count,
                           gasnet_coll_fn_handle_t func, int func_arg,
                           int flags GASNETE_THREAD_FARG) {
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_reduce_nb(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  }
#endif
GASNETI_INLINE(_gasnet_coll_reduce)
void _gasnet_coll_reduce(gasnet_team_handle_t team,
                         gasnet_image_t dstimage, void *dst,
                         void *src, size_t src_blksz, size_t src_offset,
                         size_t elem_size, size_t elem_count,
                         gasnet_coll_fn_handle_t func, int func_arg,
                         int flags GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_REDUCE(COLL_REDUCE,team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags);
  GASNETE_COLL_VALIDATE_REDUCE(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags);
  gasnete_coll_reduce(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_PASS);
}
#define gasnet_coll_reduce(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_reduce(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_GET);

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_coll_reduceM_nb
  #define gasnete_coll_reduceM_nb gasnete_coll_reduceM_nb_default
#else
  extern gasnet_coll_handle_t
  gasnete_coll_reduceM_nb_default(gasnet_team_handle_t team,
                        gasnet_image_t dstimage, void *dst,
                        void * const srclist[], size_t src_blksz, size_t src_offset,
                        size_t elem_size, size_t elem_count,
                        gasnet_coll_fn_handle_t func, int func_arg,
                        int flags GASNETE_THREAD_FARG);
#endif
extern gasnet_coll_handle_t
gasnete_coll_reduceM_nb(gasnet_team_handle_t team,
                        gasnet_image_t dstimage, void *dst,
                        void * const srclist[], size_t src_blksz, size_t src_offset,
                        size_t elem_size, size_t elem_count,
                        gasnet_coll_fn_handle_t func, int func_arg,
                        int flags GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_reduceM_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_coll_handle_t
_gasnet_coll_reduceM_nb(gasnet_team_handle_t team,
                        gasnet_image_t dstimage, void *dst,
                        void * const srclist[], size_t src_blksz, size_t src_offset,
                        size_t elem_size, size_t elem_count,
                        gasnet_coll_fn_handle_t func, int func_arg,
                        int flags GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t handle;
  GASNETI_TRACE_COLL_REDUCE_M(COLL_REDUCE_M_NB,team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags);
  GASNETE_COLL_VALIDATE_REDUCE_M(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags);
  handle = gasnete_coll_reduceM_nb(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_PASS);
  gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);
  return handle;
}
#define gasnet_coll_reduceM_nb(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_reduceM_nb(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_GET)

#ifdef gasnete_coll_reduceM
  extern void
  gasnete_coll_reduceM(gasnet_team_handle_t team,
                       gasnet_image_t dstimage, void *dst,
                       void * const srclist[], size_t src_blksz, size_t src_offset,
                       size_t elem_size, size_t elem_count,
                       gasnet_coll_fn_handle_t func, int func_arg,
                       int flags GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_reduceM)
  void gasnete_coll_reduceM(gasnet_team_handle_t team,
                            gasnet_image_t dstimage, void *dst,
                            void * const srclist[], size_t src_blksz, size_t src_offset,
                            size_t elem_size, size_t elem_count,
                            gasnet_coll_fn_handle_t func, int func_arg,
                            int flags GASNETE_THREAD_FARG) {
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_reduceM_nb(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  }
#endif
GASNETI_INLINE(_gasnet_coll_reduceM)
void _gasnet_coll_reduceM(gasnet_team_handle_t team,
                          gasnet_image_t dstimage, void *dst,
                          void * const srclist[], size_t src_blksz, size_t src_offset,
                          size_t elem_size, size_t elem_count,
                          gasnet_coll_fn_handle_t func, int func_arg,
                          int flags GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_REDUCE_M(COLL_REDUCE_M,team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags);
  GASNETE_COLL_VALIDATE_REDUCE_M(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags);
  gasnete_coll_reduceM(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_PASS);
}
#define gasnet_coll_reduceM(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_reduceM(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_GET);

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_coll_scan_nb
  #define gasnete_coll_scan_nb gasnete_coll_scan_nb_default
#else
  extern gasnet_coll_handle_t
  gasnete_coll_scan_nb_default(gasnet_team_handle_t team,
                     void *dst, size_t dst_blksz, size_t dst_offset,
                     void *src, size_t src_blksz, size_t src_offset,
                     size_t elem_size, size_t elem_count,
                     gasnet_coll_fn_handle_t func, int func_arg,
                     int flags GASNETE_THREAD_FARG);
#endif
extern gasnet_coll_handle_t
gasnete_coll_scan_nb(gasnet_team_handle_t team,
                     void *dst, size_t dst_blksz, size_t dst_offset,
                     void *src, size_t src_blksz, size_t src_offset,
                     size_t elem_size, size_t elem_count,
                     gasnet_coll_fn_handle_t func, int func_arg,
                     int flags GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_scan_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_coll_handle_t
_gasnet_coll_scan_nb(gasnet_team_handle_t team,
                     void *dst, size_t dst_blksz, size_t dst_offset,
                     void *src, size_t src_blksz, size_t src_offset,
                     size_t elem_size, size_t elem_count,
                     gasnet_coll_fn_handle_t func, int func_arg,
                     int flags GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t handle;
  GASNETI_TRACE_COLL_SCAN(COLL_SCAN_NB,team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags);
  GASNETE_COLL_VALIDATE_SCAN(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags);
  handle = gasnete_coll_scan_nb(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_PASS);
  gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);
  return handle;
}
#define gasnet_coll_scan_nb(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_scan_nb(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_GET)

#ifdef gasnete_coll_scan
  extern void
  gasnete_coll_scan(gasnet_team_handle_t team,
                    void *dst, size_t dst_blksz, size_t dst_offset,
                    void *src, size_t src_blksz, size_t src_offset,
                    size_t elem_size, size_t elem_count,
                    gasnet_coll_fn_handle_t func, int func_arg,
                    int flags GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_scan)
  void gasnete_coll_scan(gasnet_team_handle_t team,
                         void *dst, size_t dst_blksz, size_t dst_offset,
                         void *src, size_t src_blksz, size_t src_offset,
                         size_t elem_size, size_t elem_count,
                         gasnet_coll_fn_handle_t func, int func_arg,
                         int flags GASNETE_THREAD_FARG) {
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_scan_nb(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  }
#endif
GASNETI_INLINE(_gasnet_coll_scan)
void _gasnet_coll_scan(gasnet_team_handle_t team,
                       void *dst, size_t dst_blksz, size_t dst_offset,
                       void *src, size_t src_blksz, size_t src_offset,
                       size_t elem_size, size_t elem_count,
                       gasnet_coll_fn_handle_t func, int func_arg,
                       int flags GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_SCAN(COLL_SCAN,team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags);
  GASNETE_COLL_VALIDATE_SCAN(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags);
  gasnete_coll_scan(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_PASS);
}
#define gasnet_coll_scan(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_scan(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_GET);

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_coll_scanM_nb
  #define gasnete_coll_scanM_nb gasnete_coll_scanM_nb_default
#else
  extern gasnet_coll_handle_t
  gasnete_coll_scanM_nb_default(gasnet_team_handle_t team,
                      void * const dstlist[], size_t dst_blksz, size_t dst_offset,
                      void * const srclist[], size_t src_blksz, size_t src_offset,
                      size_t elem_size, size_t elem_count,
                      gasnet_coll_fn_handle_t func, int func_arg,
                      int flags GASNETE_THREAD_FARG);
#endif
extern gasnet_coll_handle_t
gasnete_coll_scanM_nb(gasnet_team_handle_t team,
                      void * const dstlist[], size_t dst_blksz, size_t dst_offset,
                      void * const srclist[], size_t src_blksz, size_t src_offset,
                      size_t elem_size, size_t elem_count,
                      gasnet_coll_fn_handle_t func, int func_arg,
                      int flags GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_scanM_nb) GASNETI_WARN_UNUSED_RESULT
gasnet_coll_handle_t
_gasnet_coll_scanM_nb(gasnet_team_handle_t team,
                      void * const dstlist[], size_t dst_blksz, size_t dst_offset,
                      void * const srclist[], size_t src_blksz, size_t src_offset,
                      size_t elem_size, size_t elem_count,
                      gasnet_coll_fn_handle_t func, int func_arg,
                      int flags GASNETE_THREAD_FARG) {
  gasnet_coll_handle_t handle;
  GASNETI_TRACE_COLL_SCAN_M(COLL_SCAN_M_NB,team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags);
  GASNETE_COLL_VALIDATE_SCAN_M(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags);
  handle = gasnete_coll_scanM_nb(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_PASS);
  gasnete_coll_poll(GASNETE_THREAD_PASS_ALONE);
  return handle;
}
#define gasnet_coll_scanM_nb(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_scanM_nb(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_GET)

#ifdef gasnete_coll_scanM
  extern void
  gasnete_coll_scanM(gasnet_team_handle_t team,
                     void * const dstlist[], size_t dst_blksz, size_t dst_offset,
                     void * const srclist[], size_t src_blksz, size_t src_offset,
                     size_t elem_size, size_t elem_count,
                     gasnet_coll_fn_handle_t func, int func_arg,
                     int flags GASNETE_THREAD_FARG);
#else
  GASNETI_INLINE(gasnete_coll_scanM)
  void gasnete_coll_scanM(gasnet_team_handle_t team,
                          void * const dstlist[], size_t dst_blksz, size_t dst_offset,
                          void * const srclist[], size_t src_blksz, size_t src_offset,
                          size_t elem_size, size_t elem_count,
                          gasnet_coll_fn_handle_t func, int func_arg,
                          int flags GASNETE_THREAD_FARG) {
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_scanM_nb(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  }
#endif
GASNETI_INLINE(_gasnet_coll_scanM)
void _gasnet_coll_scanM(gasnet_team_handle_t team,
                        void * const dstlist[], size_t dst_blksz, size_t dst_offset,
                        void * const srclist[], size_t src_blksz, size_t src_offset,
                        size_t elem_size, size_t elem_count,
                        gasnet_coll_fn_handle_t func, int func_arg,
                        int flags GASNETE_THREAD_FARG) {
  GASNETI_TRACE_COLL_SCAN_M(COLL_SCAN_M,team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags);
  GASNETE_COLL_VALIDATE_SCAN_M(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags);
  gasnete_coll_scanM(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_PASS);
}
#define gasnet_coll_scanM(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_scanM(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_GET);


/*---------------------------------------------------------------------------------*
 * Start of generic framework for reference implementations
 *---------------------------------------------------------------------------------*/

typedef struct {
    void *dst;
#if !GASNET_SEQ
    gasnet_image_t srcimage;
#endif
    gasnet_node_t srcnode;
    void *src;
    size_t nbytes;
} gasnete_coll_broadcast_args_t;

typedef struct {
    void * *dstlist;
#if !GASNET_SEQ
    gasnet_image_t srcimage;
#endif
    gasnet_node_t srcnode;
    void *src;
    size_t nbytes;
} gasnete_coll_broadcastM_args_t;

typedef struct {
    void *dst;
#if !GASNET_SEQ
    gasnet_image_t srcimage;
#endif
    gasnet_node_t srcnode;
    void *src;
    size_t nbytes;
} gasnete_coll_scatter_args_t;

typedef struct {
    void * *dstlist;
#if !GASNET_SEQ
    gasnet_image_t srcimage;
#endif
    gasnet_node_t srcnode;
    void *src;
    size_t nbytes;
} gasnete_coll_scatterM_args_t;

typedef struct {
#if !GASNET_SEQ
    gasnet_image_t dstimage;
#endif
    gasnet_node_t dstnode;
    void *dst;
    void *src;
    size_t nbytes;
} gasnete_coll_gather_args_t;

typedef struct {
#if !GASNET_SEQ
    gasnet_image_t dstimage;
#endif
    gasnet_node_t dstnode;
    void *dst;
    void * *srclist;
    size_t nbytes;
} gasnete_coll_gatherM_args_t;

typedef struct {
    void *dst;
    void *src;
    size_t nbytes;
} gasnete_coll_gather_all_args_t;

typedef struct {
    void * *dstlist;
    void * *srclist;
    size_t nbytes;
} gasnete_coll_gather_allM_args_t;

typedef struct {
    void *dst;
    void *src;
    size_t nbytes;
} gasnete_coll_exchange_args_t;

typedef struct {
    void * *dstlist;
    void * *srclist;
    size_t nbytes;
} gasnete_coll_exchangeM_args_t;


/* Options for gasnete_coll_generic_* */
#define GASNETE_COLL_GENERIC_OPT_INSYNC		0x0001
#define GASNETE_COLL_GENERIC_OPT_OUTSYNC	0x0002
#define GASNETE_COLL_GENERIC_OPT_P2P		0x0004

/* Macros for conditionally setting flags in gasnete_coll_generic_* options */
#define GASNETE_COLL_GENERIC_OPT_INSYNC_IF(COND)	((COND) ? GASNETE_COLL_GENERIC_OPT_INSYNC : 0)
#define GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(COND)	((COND) ? GASNETE_COLL_GENERIC_OPT_OUTSYNC : 0)
#define GASNETE_COLL_GENERIC_OPT_P2P_IF(COND)		((COND) ? GASNETE_COLL_GENERIC_OPT_P2P : 0)

struct gasnete_coll_generic_data_t_ {
    #if GASNETI_USE_TRUE_MUTEXES || GASNET_DEBUG
      void				*owner; /* has type gasnete_threaddata_t* (not yet defined) */
    #endif
    #if GASNET_DEBUG
      #define GASNETE_COLL_GENERIC_TAG(T)	_CONCAT(GASNETE_COLL_GENERIC_TAG_,T)
      #define GASNETE_COLL_GENERIC_SET_TAG(D,T)	(D)->tag = GASNETE_COLL_GENERIC_TAG(T)

      enum {
	/* Single-address interfaces: */
	GASNETE_COLL_GENERIC_TAG(broadcast),
	GASNETE_COLL_GENERIC_TAG(scatter),
	GASNETE_COLL_GENERIC_TAG(gather),
	GASNETE_COLL_GENERIC_TAG(gather_all),
	GASNETE_COLL_GENERIC_TAG(exchange),
	/* Multiple-address interfaces: */
	GASNETE_COLL_GENERIC_TAG(broadcastM),
	GASNETE_COLL_GENERIC_TAG(scatterM),
	GASNETE_COLL_GENERIC_TAG(gatherM),
	GASNETE_COLL_GENERIC_TAG(gather_allM),
	GASNETE_COLL_GENERIC_TAG(exchangeM)
	#if GASNET_PAR
	  /* Single-address/multi-thread interfaces: */
	  , GASNETE_COLL_GENERIC_TAG(broadcastT),
	  GASNETE_COLL_GENERIC_TAG(scatterT),
	  GASNETE_COLL_GENERIC_TAG(gatherT),
	  GASNETE_COLL_GENERIC_TAG(gather_allT),
	  GASNETE_COLL_GENERIC_TAG(exchangeT)
	#endif
	/* XXX: still need a few more for scan and reduce */

	/* Hook for conduit-specific extension */
	#ifdef GASNETE_COLL_GENERIC_TAG_EXTRA
	  , GASNETE_COLL_GENERIC_TAG_EXTRA
	#endif
      }					tag;
    #else
      #define GASNETE_COLL_GENERIC_SET_TAG(D,T)
    #endif

    int					state;
    int					options;
    gasnete_coll_consensus_t		in_barrier;
    gasnete_coll_consensus_t		out_barrier;
    gasnete_coll_p2p_t			*p2p;
    gasnet_handle_t			handle;
    gasnet_coll_handle_t		coll_handle;
    void				*private_data;

    #if GASNET_PAR
    struct {
      gasneti_atomic_t			remaining;
      void				*data;
    }					threads;
    #endif

    /* Hook for conduit-specific extension */
    #ifdef GASNETE_COLL_GENERIC_EXTRA
      GASNETE_COLL_GENERIC_EXTRA
    #endif

    union {
	/* Single-address interfaces: */
	gasnete_coll_broadcast_args_t		broadcast;
	gasnete_coll_scatter_args_t		scatter;
	gasnete_coll_gather_args_t		gather;
	gasnete_coll_gather_all_args_t		gather_all;
	gasnete_coll_exchange_args_t		exchange;
	/* Multiple-address interfaces: */
	gasnete_coll_broadcastM_args_t		broadcastM;
	gasnete_coll_scatterM_args_t		scatterM;
	gasnete_coll_gatherM_args_t		gatherM;
	gasnete_coll_gather_allM_args_t		gather_allM;
	gasnete_coll_exchangeM_args_t		exchangeM;

	/* XXX: still need a few more */

	/* Hook for conduit-specific extension */
	#ifdef GASNETE_COLL_GENERIC_ARGS_EXTRA
	  GASNETE_COLL_GENERIC_ARGS_EXTRA
	#endif
    }					args;
};
#define GASNETE_COLL_GENERIC_DATA(op) ((gasnete_coll_generic_data_t *)((op)->data))

/* Extract pointer to correct member of args union
 * Also does some consistency checking when debugging is enabled
 */
#define GASNETE_COLL_GENERIC_ARGS(D,T) \
		(gasneti_assert((D) != NULL),                               \
		 gasneti_assert((D)->tag == GASNETE_COLL_GENERIC_TAG(T)),   \
		 &((D)->args.T))


extern gasnete_coll_generic_data_t *gasnete_coll_generic_alloc(GASNETE_THREAD_FARG_ALONE);
void gasnete_coll_generic_free(gasnete_coll_generic_data_t *data GASNETE_THREAD_FARG);
extern gasnet_coll_handle_t gasnete_coll_op_generic_init(gasnete_coll_team_t team, int flags,
							 gasnete_coll_generic_data_t *data,
							 gasnete_coll_poll_fn poll_fn,
							 uint32_t sequence
							 GASNETE_THREAD_FARG);
extern int gasnete_coll_generic_syncnb(gasnete_coll_generic_data_t *data);

#if GASNET_PAR
  extern void gasnete_coll_threads_lock(int flags GASNETE_THREAD_FARG);
  extern void gasnete_coll_threads_unlock(GASNETE_THREAD_FARG_ALONE);
  extern int gasnete_coll_threads_first(GASNETE_THREAD_FARG_ALONE);
  extern gasnet_coll_handle_t gasnete_coll_threads_get_handle(GASNETE_THREAD_FARG_ALONE);
  extern void gasnete_coll_threads_insert(gasnete_coll_op_t *op GASNETE_THREAD_FARG);
  extern void gasnete_coll_threads_delete(gasnete_coll_op_t *op GASNETE_THREAD_FARG);
  GASNETI_INLINE(gasnete_coll_generic_all_threads)
  int gasnete_coll_generic_all_threads(gasnete_coll_generic_data_t *data) {
    int result;
    gasneti_assert(data != NULL);
    result = (gasneti_atomic_read(&data->threads.remaining, 0) == 0);
    if (result) {
      gasneti_sync_reads();
    }
    return result;
  }
#else
  #define gasnete_coll_threads_lock(flags)		do { } while (0)
  #define gasnete_coll_threads_unlock(thrarg)	do { } while (0)
  #define gasnete_coll_threads_first(thrarg)		1
  #define gasnete_coll_threads_get_handle(thrarg)	\
	(gasneti_fatalerror("Call to gasnete_coll_threads_get_handle() in non-PAR build"), \
	 GASNET_COLL_INVALID_HANDLE)
  #define gasnete_coll_threads_insert(op)		\
	gasneti_fatalerror("Call to gasnete_coll_threads_insert() in non-PAR build")
  #define gasnete_coll_threads_delete(op)		do { } while (0)
  #define gasnete_coll_generic_all_threads(data)	(1)
#endif

GASNETI_INLINE(gasnete_coll_generic_insync)
int gasnete_coll_generic_insync(gasnete_coll_generic_data_t *data) {
  gasneti_assert(data != NULL);
  return (!(data->options & GASNETE_COLL_GENERIC_OPT_INSYNC) ||
	  (gasnete_coll_consensus_try(data->in_barrier) == GASNET_OK));
}

GASNETI_INLINE(gasnete_coll_generic_outsync)
int gasnete_coll_generic_outsync(gasnete_coll_generic_data_t *data) {
  gasneti_assert(data != NULL);
  return (!(data->options & GASNETE_COLL_GENERIC_OPT_OUTSYNC) ||
	  (gasnete_coll_consensus_try(data->out_barrier) == GASNET_OK));
}

extern int gasnete_coll_generic_coll_sync(gasnet_coll_handle_t *p, size_t count GASNETE_THREAD_FARG);


extern gasnet_coll_handle_t
gasnete_coll_generic_broadcast_nb(gasnet_team_handle_t team,
                                  void *dst,
                                  gasnet_image_t srcimage, void *src,
                                  size_t nbytes, int flags,
                                  gasnete_coll_poll_fn poll_fn, int options,
                                  void *private_data, uint32_t sequence
                                  GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_generic_broadcastM_nb(gasnet_team_handle_t team,
                                   void * const dstlist[],
                                   gasnet_image_t srcimage, void *src,
                                   size_t nbytes, int flags,
                                   gasnete_coll_poll_fn poll_fn, int options,
                                   void *private_data, uint32_t sequence
                                   GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_generic_scatter_nb(gasnet_team_handle_t team,
                                void *dst,
                                gasnet_image_t srcimage, void *src,
                                size_t nbytes, int flags,
                                gasnete_coll_poll_fn poll_fn, int options,
                                void *private_data, uint32_t sequence
                                GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_generic_scatterM_nb(gasnet_team_handle_t team,
                                 void * const dstlist[],
                                 gasnet_image_t srcimage, void *src,
                                 size_t nbytes, int flags,
                                 gasnete_coll_poll_fn poll_fn, int options,
                                 void *private_data, uint32_t sequence
                                 GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_generic_gather_nb(gasnet_team_handle_t team,
                               gasnet_image_t dstimage, void *dst,
                               void *src,
                               size_t nbytes, int flags,
                               gasnete_coll_poll_fn poll_fn, int options,
                               void *private_data, uint32_t sequence
                               GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_generic_gatherM_nb(gasnet_team_handle_t team,
                                gasnet_image_t dstimage, void *dst,
                                void * const srclist[],
                                size_t nbytes, int flags,
                                gasnete_coll_poll_fn poll_fn, int options,
                                void *private_data, uint32_t sequence
                                GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_generic_gather_all_nb(gasnet_team_handle_t team,
                                   void *dst, void *src,
                                   size_t nbytes, int flags,
                                   gasnete_coll_poll_fn poll_fn, int options,
                                   void *private_data, uint32_t sequence
                                   GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_generic_gather_allM_nb(gasnet_team_handle_t team,
                                    void * const dstlist[], void * const srclist[],
                                    size_t nbytes, int flags,
                                    gasnete_coll_poll_fn poll_fn, int options,
                                    void *private_data, uint32_t sequence
                                    GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_generic_exchange_nb(gasnet_team_handle_t team,
                                 void *dst, void *src,
                                 size_t nbytes, int flags,
                                 gasnete_coll_poll_fn poll_fn, int options,
                                 void *private_data, uint32_t sequence
                                 GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_generic_exchangeM_nb(gasnet_team_handle_t team,
                                  void * const dstlist[], void * const srclist[],
                                  size_t nbytes, int flags,
                                  gasnete_coll_poll_fn poll_fn, int options,
                                  void *private_data, uint32_t sequence
                                  GASNETE_THREAD_FARG);

/*---------------------------------------------------------------------------------*
 * Start of generic framework for tree-based reference implementations
 *---------------------------------------------------------------------------------*/

typedef enum {
    GASNETE_COLL_TREE_KIND_CHAIN,
    GASNETE_COLL_TREE_KIND_BINARY,
    GASNETE_COLL_TREE_KIND_BINOMIAL,
    GASNETE_COLL_TREE_KIND_SEQUENTIAL,
#if 0
    GASNETE_COLL_TREE_KIND_CHAIN_SMP,
    GASNETE_COLL_TREE_KIND_BINARY_SMP,
    GASNETE_COLL_TREE_KIND_BINOMIAL_SMP,
    GASNETE_COLL_TREE_KIND_SEQUENTIAL_SMP,
#endif
#ifdef GASNETE_COLL_TREE_KIND_ENUM_EXTRA
    GASNETE_COLL_TREE_KIND_ENUM_EXTRA
#endif
    GASNETE_COLL_TREE_KIND_INVALID
} gasnete_coll_tree_kind_t;
                                                                                                              
/* Local view of the layout of a tree */
typedef struct {
    gasnet_node_t	parent;
    int			child_id;       /* I am which element of parent's child_list? */
    int			child_count;
    gasnet_node_t	*child_list;
    /* used only as keys when caching: */
    gasnete_coll_tree_kind_t	kind;
    gasnet_node_t		root;
    gasneti_weakatomic_t	ref_count;
} gasnete_coll_tree_geom_t;
                                                                                                              
/* Data for a given tree-based operation */
struct gasnete_coll_tree_data_t_ {
    uint32_t			pipe_seg_size;
    uint32_t			sent_bytes;
    gasnete_coll_tree_geom_t	*geom;
};
                                                                                                              
extern gasnete_coll_tree_data_t *gasnete_coll_tree_init(gasnete_coll_tree_kind_t kind, gasnet_node_t rootnode GASNETE_THREAD_FARG);
extern void gasnete_coll_tree_free(gasnete_coll_tree_data_t *tree GASNETE_THREAD_FARG);

/*---------------------------------------------------------------------------------*
 * Start of protypes for reference implementations
 *---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_bcast_Get(gasnet_team_handle_t team,
		       void *dst,
		       gasnet_image_t srcimage, void *src,
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcast_Put(gasnet_team_handle_t team,
		       void *dst,
		       gasnet_image_t srcimage, void *src,
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcast_Eager(gasnet_team_handle_t team,
			 void *dst,
			 gasnet_image_t srcimage, void *src,
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcast_RVGet(gasnet_team_handle_t team,
			 void *dst,
			 gasnet_image_t srcimage, void *src,
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcast_RVous(gasnet_team_handle_t team,
			 void *dst,
			 gasnet_image_t srcimage, void *src,
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcast_TreePut(gasnet_team_handle_t team,
			   void *dst,
			   gasnet_image_t srcimage, void *src,
			   size_t nbytes, int flags,
			   gasnete_coll_tree_kind_t kind,
			   uint32_t sequence
			   GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcast_TreeGet(gasnet_team_handle_t team,
			   void *dst,
			   gasnet_image_t srcimage, void *src,
			   size_t nbytes, int flags,
			   gasnete_coll_tree_kind_t kind,
			   uint32_t sequence
			   GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcast_TreeEager(gasnet_team_handle_t team,
			     void *dst,
			     gasnet_image_t srcimage, void *src,
			     size_t nbytes, int flags,
			     gasnete_coll_tree_kind_t kind,
			     uint32_t sequence
			     GASNETE_THREAD_FARG);

/*---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_bcastM_Get(gasnet_team_handle_t team,
			void * const dstlist[],
			gasnet_image_t srcimage, void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcastM_Put(gasnet_team_handle_t team,
			void * const dstlist[],
			gasnet_image_t srcimage, void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcastM_Eager(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcastM_RVGet(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcastM_RVous(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG);

/*---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_scat_Get(gasnet_team_handle_t team,
		      void *dst,
		      gasnet_image_t srcimage, void *src,
		      size_t nbytes, int flags, uint32_t sequence
                      GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_scat_Put(gasnet_team_handle_t team,
		      void *dst,
		      gasnet_image_t srcimage, void *src,
		      size_t nbytes, int flags, uint32_t sequence
                      GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_scat_Eager(gasnet_team_handle_t team,
		        void *dst,
		        gasnet_image_t srcimage, void *src,
		        size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_scat_RVGet(gasnet_team_handle_t team,
		        void *dst,
		        gasnet_image_t srcimage, void *src,
		        size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_scat_RVous(gasnet_team_handle_t team,
		        void *dst,
		        gasnet_image_t srcimage, void *src,
		        size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);

/*---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_scatM_Get(gasnet_team_handle_t team,
		       void * const dstlist[],
		       gasnet_image_t srcimage, void *src,
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_scatM_Put(gasnet_team_handle_t team,
		       void * const dstlist[],
		       gasnet_image_t srcimage, void *src,
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_scatM_Eager(gasnet_team_handle_t team,
		         void * const dstlist[],
		         gasnet_image_t srcimage, void *src,
		         size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_scatM_RVGet(gasnet_team_handle_t team,
		         void * const dstlist[],
		         gasnet_image_t srcimage, void *src,
		         size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_scatM_RVous(gasnet_team_handle_t team,
		         void * const dstlist[],
		         gasnet_image_t srcimage, void *src,
		         size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG);

/*---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_gath_Get(gasnet_team_handle_t team,
		      gasnet_image_t dstimage, void *dst,
		      void *src,
		      size_t nbytes, int flags, uint32_t sequence
                      GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_gath_Put(gasnet_team_handle_t team,
		      gasnet_image_t dstimage, void *dst,
		      void *src,
		      size_t nbytes, int flags, uint32_t sequence
                      GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_gath_Eager(gasnet_team_handle_t team,
		        gasnet_image_t dstimage, void *dst,
		        void *src,
		        size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_gath_RVPut(gasnet_team_handle_t team,
		        gasnet_image_t dstimage, void *dst,
		        void *src,
		        size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_gath_RVous(gasnet_team_handle_t team,
		        gasnet_image_t dstimage, void *dst,
		        void *src,
		        size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);

/*---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_gathM_Get(gasnet_team_handle_t team,
		       gasnet_image_t dstimage, void *dst,
		       void * const srclist[],
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_gathM_Put(gasnet_team_handle_t team,
		       gasnet_image_t dstimage, void *dst,
		       void * const srclist[],
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_gathM_Eager(gasnet_team_handle_t team,
		         gasnet_image_t dstimage, void *dst,
		         void * const srclist[],
		         size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_gathM_RVPut(gasnet_team_handle_t team,
		         gasnet_image_t dstimage, void *dst,
		         void * const srclist[],
		         size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_gathM_RVous(gasnet_team_handle_t team,
		         gasnet_image_t dstimage, void *dst,
		         void * const srclist[],
		         size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG);

/*---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_gall_Gath(gasnet_team_handle_t team,
		       void *dst, void *src,
		       size_t nbytes, int flags, uint32_t sequence
                       GASNETE_THREAD_FARG);

/*---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_gallM_Gath(gasnet_team_handle_t team,
			void * const dstlist[], void * const srclist[],
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);

/*---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_exchg_Gath(gasnet_team_handle_t team,
			void *dst, void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);

/*---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_exchgM_Gath(gasnet_team_handle_t team,
			 void * const dstlist[], void * const srclist[],
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG);

/*---------------------------------------------------------------------------------*/

GASNETI_END_EXTERNC

#endif
