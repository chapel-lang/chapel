/*   $Source$
 *     $Date$
 * $Revision$
 * Description: GASNet Collectives conduit header
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */



#ifndef _GASNET_COLL_INTERNAL_H
#define _GASNET_COLL_INTERNAL_H
#include <gasnet_coll.h>
#include <gasnet_handler.h>

#define GASNETI_COLL_FN_HEADER(FNNAME) 
/*---------------------------------------------------------------------------------*/
/* ***  Macros and Constants *** */
/*---------------------------------------------------------------------------------*/

#if GASNET_PAR
#define GASNETE_COLL_THREAD_LOCAL        (1<<29)
#endif
#define GASNETE_COLL_SUBORDINATE	 (1<<30)
#define GASNETE_COLL_USE_SCRATCH         (1<<28)
#define GASNETE_COLL_USE_SCRATCH_TREE    (1<<27)
#define GASNETE_COLL_USE_SCRATCH_DISSSEM (1<<26)
#define GASNETE_COLL_USE_TREE		 (1<<25)
#define GASNETE_COLL_NONROOT_SUBORDINATE (1<<24)

#define GASNETE_COLL_IN_MODE(flags) \
((flags) & (GASNET_COLL_IN_NOSYNC  | GASNET_COLL_IN_MYSYNC  | GASNET_COLL_IN_ALLSYNC))
#define GASNETE_COLL_OUT_MODE(flags) \
((flags) & (GASNET_COLL_OUT_NOSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_OUT_ALLSYNC))
#define GASNETE_COLL_SYNC_MODE(flags) \
((flags) & (GASNET_COLL_OUT_NOSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_OUT_ALLSYNC | \
            GASNET_COLL_IN_NOSYNC  | GASNET_COLL_IN_MYSYNC  | GASNET_COLL_IN_ALLSYNC))



/* Internal flags */
#define GASNETE_COLL_OP_COMPLETE	0x1
#define GASNETE_COLL_OP_INACTIVE	0x2



/*---------------------------------------------------------------------------------*/
/* Forward type decls and typedefs:                                                */
/*---------------------------------------------------------------------------------*/
struct gasnete_coll_op_t_;
typedef struct gasnete_coll_op_t_ gasnete_coll_op_t;

struct gasnete_coll_p2p_t_;
typedef struct gasnete_coll_p2p_t_ gasnete_coll_p2p_t;

union gasnete_coll_p2p_entry_t_;
typedef union gasnete_coll_p2p_entry_t_ gasnete_coll_p2p_entry_t;

struct gasnete_coll_p2p_send_struct;
typedef struct gasnete_coll_p2p_send_struct gasnete_coll_p2p_send_struct_t;

struct gasnete_coll_generic_data_t_;
typedef struct gasnete_coll_generic_data_t_ gasnete_coll_generic_data_t;

struct gasnete_coll_tree_type_t_;
typedef struct gasnete_coll_tree_type_t_ gasnete_coll_tree_type_t;

struct gasnete_coll_tree_data_t_;
typedef struct gasnete_coll_tree_data_t_ gasnete_coll_tree_data_t;

struct gasnete_coll_local_tree_geom_t_;
typedef struct gasnete_coll_local_tree_geom_t_ gasnete_coll_local_tree_geom_t;

struct gasnete_coll_tree_geom_t_;
typedef struct gasnete_coll_tree_geom_t_ gasnete_coll_tree_geom_t;

struct gasnete_coll_dissem_vector_t_;
typedef struct gasnete_coll_dissem_vector_t_ gasnete_coll_dissem_vector_t;

struct gasnete_coll_dissem_info_t_;
typedef struct gasnete_coll_dissem_info_t_ gasnete_coll_dissem_info_t;

struct gasnete_coll_op_status_t_;
typedef struct gasnete_coll_op_status_t_ gasnete_coll_op_status_t;

struct gasnete_coll_scratch_status_t_;
typedef struct gasnete_coll_scratch_status_t_ gasnete_coll_scratch_status_t;

struct gasnete_coll_scratch_req_t_;
typedef struct gasnete_coll_scratch_req_t_ gasnete_coll_scratch_req_t;

struct gasnete_coll_seg_interval_t_;
typedef struct gasnete_coll_seg_interval_t_ gasnete_coll_seg_interval_t;

struct gasnete_coll_autotune_info_t_;
typedef struct gasnete_coll_autotune_info_t_ gasnete_coll_autotune_info_t;

/*---------------------------------------------------------------------------------*/

extern size_t gasnete_coll_p2p_eager_min;
extern size_t gasnete_coll_p2p_eager_scale;


#ifndef GASNETE_COLL_IMAGE_OVERRIDE
/* gasnet_image_t must be large enough to index all threads that participate
* in collectives.  A conduit may override this if a smaller type will suffice.
* However, types larger than 32-bits won't pass as AM handler args.  So, for
* a larger type, many default things will require overrides.
*/
#if GASNET_SEQ
#define gasnete_coll_image_node(I)	I
#else
extern gasnet_node_t *gasnete_coll_image_to_node;
#define gasnete_coll_image_node(I)	\
(gasneti_assert(gasnete_coll_image_to_node != NULL), gasnete_coll_image_to_node[I])
#endif
#define gasnete_coll_image_is_local(I)	(gasneti_mynode == gasnete_coll_image_node(I))
#endif

#ifndef GASNETE_COLL_HANDLE_OVERRIDE
/* Handle type for collective ops: */
#if GASNET_PAR
struct gasnet_coll_handle_t_ {
  volatile int		done;
  gasnet_coll_handle_t	next;
};
#endif
#define GASNET_COLL_INVALID_HANDLE ((gasnet_coll_handle_t)NULL)
#endif

extern gasnet_coll_handle_t gasnete_coll_handle_create(GASNETE_THREAD_FARG_ALONE);
extern void gasnete_coll_handle_signal(gasnet_coll_handle_t handle GASNETE_THREAD_FARG);
extern int gasnete_coll_handle_done(gasnet_coll_handle_t handle GASNETE_THREAD_FARG);

/** Need to insert this here so that trees.h picks up all the forward declaration of the structs*/
/** but also before the internal strucutres use the trees*/
#include <gasnet_coll_trees.h>

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


/*---------------------------------------------------------------------------------*/
/* Data for a given tree-based operation */
struct gasnete_coll_tree_data_t_ {
  uint32_t			pipe_seg_size;
  uint32_t			sent_bytes;
  gasnete_coll_local_tree_geom_t	*geom;
};
#define GASNETE_COLL_MIN_SCRATCH_SIZE_DEFAULT 1024
#define GASNETE_COLL_MAX_SCRATCH_SIZE 0xffffffff

#ifndef GASNETE_COLL_SCRATCH_SIZE
/*set defult to 2 MB*/
#define GASNETE_COLL_SCRATCH_SIZE_DEFAULT (2*(1024*1024))
#endif

#if 0
#define GASNETE_COLL_MIN_LOC_SCRATCH_SIZE 256
#define GASNETE_COLL_MAX_LOC_SCRATCH_SIZE 0xffffffff
#ifndef GASNETE_COLL_OPT_LOC_SCRATCH_SIZE
/*set defult to 2 MB*/
#define GASNETE_COLL_OPT_LOC_SCRATCH_SIZE GASNETE_COLL_OPT_SCRATCH_SIZE_DEFAULT
#endif
#endif

/*---------------------------------------------------------------------------------*/

/* Type for collective teams: */
struct gasnete_coll_team_t_ {
#ifndef GASNETE_COLL_P2P_OVERRIDE
  /* Default implementation of point-to-point syncs
	 * does not (currently) have a team-specific portion.
	 */
#endif
  
  /* read-only fields: */
  uint32_t			team_id;
  int					global_team;
  
  
		
  /* tree geometry cache, each team should have its own cache .... */
  gasnete_coll_tree_geom_t *tree_geom_cache_head;
  gasnete_coll_tree_geom_t *tree_geom_cache_tail;
  
  /*dissem geometry cache, each team should have its own  ... */
  gasnete_coll_dissem_info_t *dissem_cache_head;
  gasnete_coll_dissem_info_t *dissem_cache_tail;
  
  /*my relative node id in this team*/
  gasnet_node_t myrank;
  
  /*total number of members in this team*/
  int total_ranks;
  
  /* scratch segments allocated on team creation*/
  gasnet_seginfo_t *scratch_segs;
  size_t smallest_scratch_seg;
  
  /*scratch space management*/
  gasnete_coll_scratch_status_t* scratch_status;
  
  /*autotuning info*/
  gasnete_coll_autotune_info_t* autotune_info;
  
  /*map of relative nodes in this team to actual nodes*/
  /*for TEAM_ALL this will just be a one-to-one mapping */
  /*not worrying about this yet*/
  /*gasnet_node_t *node_map; */
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
  
  /* a list of the ops for the scratch list management*/
  gasnete_coll_op_t *scratch_next, *scratch_prev;
  
  /* Linkage used by aggregation.
    * Access is serialized by specification+client: */
#ifndef GASNETE_COLL_AGG_OVERRIDE
  /* Defaule implementation of ops aggregation */
  gasnete_coll_op_t		*agg_next, *agg_prev, *agg_head;
#endif
  
#if GASNET_PAR
  struct {
    uint32_t			sequence;
  } threads;
#endif
  
  /* Read-only fields: */
  gasnete_coll_team_t		team;
  uint32_t			sequence;
  int				flags;
  gasnet_coll_handle_t	handle;
  
  /* Per-instance fields: */
  void			*data;
  gasnete_coll_poll_fn	poll_fn;
  
  /*positioons of the valide scratch space for this operation on the peers*/
  uint64_t *scratchpos;
  uint64_t myscratchpos;
  uint8_t active_scratch_op; /* is this op on the active scratch list?*/
  uint8_t waiting_scratch_op; /* is this op on the waiting scratch list?*/
  uint8_t waiting_for_reconfig_clear;
#if GASNET_DEBUG
  uint8_t scratch_op_freed;
#endif
  gasnete_coll_scratch_req_t *scratch_req; /* the associated scratch request with this op*/
  
  /* Hook for conduit-specific extensions/overrides */
#ifdef GASNETE_COLL_OP_EXTRA
  GASNETE_COLL_OP_EXTRA
#endif
};

struct gasnete_coll_seg_interval_t_ {
  uint32_t start;
  uint32_t end;
  gasnete_coll_seg_interval_t *next;
};

/*---------------------------------------------------------------------------------*/
/* Type for global synchronization */

#ifndef GASNETE_COLL_CONSENSUS_OVERRIDE
/* Scalar type, could be a pointer to a struct */
typedef uint32_t gasnete_coll_consensus_t;
#endif

extern gasnete_coll_consensus_t gasnete_coll_consensus_create(void);
extern int gasnete_coll_consensus_try(gasnete_coll_consensus_t id);
extern int gasnete_coll_consensus_wait(GASNETE_THREAD_FARG_ALONE);

/*---------------------------------------------------------------------------------*/
/* Type for point-to-point synchronization */

#ifndef GASNETE_COLL_P2P_EAGER_SCALE_DEFAULT
/* Number of bytes per-image to allocate for eager data */
#define GASNETE_COLL_P2P_EAGER_SCALE_DEFAULT	16
#endif
#ifndef GASNETE_COLL_P2P_EAGER_MIN_DEFAULT
/* Minumum number of bytes to allocate for eager data */
#define GASNETE_COLL_P2P_EAGER_MIN_DEFAULT		16
#endif

#ifndef GASNETE_COLL_SEG_SIZE_DEFAULT
/* set the Default Segment Size for Pipelining*/
#define GASNETE_COLL_SEG_SIZE_DEFAULT 1024
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
  gasneti_weakatomic_t	counter;
  
  /* Handler-safe lock (if needed) */
  gasnet_hsl_t		lock;
  
  /* manage intervals for segmented algorithms*/
  size_t seg_size;
  uint32_t num_segs_processed;
  gasnete_coll_seg_interval_t *seg_intervals;
  gasnete_coll_seg_interval_t *seg_free_list;
  
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
extern void gasnete_coll_p2p_advance(gasnete_coll_op_t *op, gasnet_node_t dstnode);
extern void gasnete_coll_p2p_counting_put(gasnete_coll_op_t *op, gasnet_node_t dstnode, void *dst,
                                   void *src, size_t nbytes);
extern void gasnete_coll_p2p_counting_putAsync(gasnete_coll_op_t *op, gasnet_node_t dstnode, void *dst,
                                        void *src, size_t nbytes);
extern void gasnete_coll_p2p_eager_put_tree(gasnete_coll_op_t *op, gasnet_node_t dstnode, 
                                           void *src, size_t size);
extern void gasnete_coll_p2p_send_rtrM(gasnete_coll_op_t *op, gasnete_coll_p2p_t *p2p,
                                       uint32_t offset, void * const *dstlist,
                                       gasnet_node_t node, size_t nbytes, uint32_t count);
extern void gasnete_coll_p2p_send_rtr(gasnete_coll_op_t *op, gasnete_coll_p2p_t *p2p,
                                      uint32_t offset, void *dst,
                                      gasnet_node_t node, size_t nbytes);
extern int gasnete_coll_p2p_send_done(gasnete_coll_p2p_t *p2p);
extern  int gasnete_coll_p2p_send_data(gasnete_coll_op_t *op, gasnete_coll_p2p_t *p2p,
                                       gasnet_node_t node, uint32_t offset,
                                       const void *src, size_t nbytes);
struct gasnete_coll_p2p_send_struct { void *addr; size_t sent; };
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

/* Helper for scaling of void pointers */
GASNETI_INLINE(gasnete_coll_scale_ptrM)
void gasnete_coll_scale_ptrM(void * out_ptr[], void * const in_ptr[], size_t elem_count, size_t elem_size, gasnet_image_t total_images) {
  int i;
  for(i=0; i<total_images; i++) {
    out_ptr[i] = (void *)((uintptr_t)in_ptr[i] + (elem_count * elem_size));
  }
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
  gasneti_sync_reads();
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
/* Forward decls and macros */

#if GASNET_PAR
#define GASNETE_COLL_FORWARD_FLAGS(flags) \
(((flags) & ~(GASNET_COLL_IN_ALLSYNC|GASNET_COLL_IN_MYSYNC|\
              GASNET_COLL_OUT_ALLSYNC|GASNET_COLL_OUT_MYSYNC|\
              GASNET_COLL_AGGREGATE|GASNETE_COLL_THREAD_LOCAL)) \
 | (GASNET_COLL_IN_NOSYNC|GASNET_COLL_OUT_NOSYNC|GASNETE_COLL_SUBORDINATE))
#else
#define GASNETE_COLL_FORWARD_FLAGS(flags) \
(((flags) & ~(GASNET_COLL_IN_ALLSYNC|GASNET_COLL_IN_MYSYNC|\
              GASNET_COLL_OUT_ALLSYNC|GASNET_COLL_OUT_MYSYNC|\
              GASNET_COLL_AGGREGATE)) \
 | (GASNET_COLL_IN_NOSYNC|GASNET_COLL_OUT_NOSYNC|GASNETE_COLL_SUBORDINATE))
#endif

/* XXX: Until we have gasnete_poll, a thread will only poll the collectives when
it does a try/wait on a collective handle.  So we must ensure a polling thread
does not initiate any RDMA or subordinate collective operation unless we can be
certain it will continue to poll until the RDMA or subordinate is done.
The cases we must protect against include threads that get an INVALID_HANDLE
for one or more ops and therefore may cease to poll before all collective ops
are completed.
The solution used here is to only allow a poller to initiate RDMA and/or
subordinate operations if it has a handle for the current operation.  While
we can't always know for sure, it is sufficient to only initiate when we
can *prove* the current thread has a handle for the current op:
+ The thread that initiated to op is the "owner" and must have a handle.
  Assigning ownership in this manner ensures that there is always at least
  one thread that may make progress on each outstanding op.  In some cases
  this is the only thread which is certain to poll again, so tracking of
  the "owner" is required.
+ (flags & (OUT_MYSYNC|OUT_ALLSYNC)) implies that ALL threads must have a
  handle for this op (note that subordinate ops are initiated by a single
                      thread, but this is OK since they always have NO/NO sync flags).  This
  is the only current improvement over the older CHECK_OWNER tests.
  The MY case won't work when we begin to signal threads individially as
  their data is delivered/consumed, but gasnete_poll() should be done
  before that.
  None of this is needed once gasnete_poll() will ensure that gasneti_AMPoll()
  will poll collectives as long as any remain unfinished.
  */
#if GASNETI_USE_TRUE_MUTEXES
#define GASNETE_COLL_MAY_INIT_FOR(op)	((GASNETE_COLL_GENERIC_DATA(op)->owner == GASNETE_MYTHREAD) || \
					 ((op)->flags & (GASNET_COLL_OUT_MYSYNC | GASNET_COLL_OUT_ALLSYNC)))
#define GASNETE_COLL_SET_OWNER(data)	(data)->owner = GASNETE_MYTHREAD
#else
#define GASNETE_COLL_MAY_INIT_FOR(op)	1
#if GASNET_DEBUG
#define GASNETE_COLL_SET_OWNER(data)	(data)->owner = GASNETE_MYTHREAD
#else
#define GASNETE_COLL_SET_OWNER(data)	do {} while (0)
#endif
#endif

/*---------------------------------------------------------------------------------*/
/* XXX: sequence and other stuff that will need to be per-team scoped: */

extern gasnet_coll_fn_entry_t *gasnete_coll_fn_tbl;
extern size_t gasnete_coll_fn_count;
extern uint32_t gasnete_coll_sequence;	/* arbitrary non-zero starting value */
extern gasnet_image_t *gasnete_coll_all_images;
extern gasnet_image_t *gasnete_coll_all_offset;
extern uint8_t gasnete_coll_fixed_image_count; /* 1 if all the nodes have teh same number of images and 0 else*/ 
extern gasnet_image_t gasnete_coll_total_images;
extern gasnet_image_t gasnete_coll_max_images;
extern gasnet_image_t gasnete_coll_my_images;	/* count of local images */
extern gasnet_image_t gasnete_coll_my_offset;	/* count of images before my first image */
#if !GASNET_SEQ
extern gasnet_node_t *gasnete_coll_image_to_node;
#endif
#if GASNET_PAR
extern int gasnete_coll_multi_images;	/* count of local images > 1 */
extern int gasnete_coll_multi_images_any;	/* count of any node's images > 1 */
#endif

#define GASNETE_COLL_1ST_IMAGE(LIST,NODE) \
(((void * const *)(LIST))[gasnete_coll_all_offset[(NODE)]])
#define GASNETE_COLL_MY_1ST_IMAGE(LIST,FLAGS) \
(((void * const *)(LIST))[((FLAGS) & GASNET_COLL_LOCAL) ? 0 : gasnete_coll_my_offset])

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



/*---------------------------------------------------------------------------------*/
/* Handles */

extern void gasnete_coll_save_handle(gasnet_handle_t *handle_p GASNETE_THREAD_FARG);
extern void gasnete_coll_save_coll_handle(gasnet_coll_handle_t *handle_p GASNETE_THREAD_FARG);
extern void gasnete_coll_sync_saved_handles(GASNETE_THREAD_FARG_ALONE);

/*---------------------------------------------------------------------------------*/
/* Synchronization for threads in PAR builds */
#if GASNET_PAR
extern int gasnete_coll_threads_ready1(gasnete_coll_op_t *op, void **list GASNETE_THREAD_FARG);
extern int gasnete_coll_threads_ready2(gasnete_coll_op_t *op, void **list1, void **list2 GASNETE_THREAD_FARG);
#else
#define gasnete_coll_threads_ready1(op,list1)	1
#define gasnete_coll_threads_ready2(op,list1,list2)	1
#endif


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
  size_t dist;
} gasnete_coll_scatter_args_t;

typedef struct {
  void * *dstlist;
#if !GASNET_SEQ
  gasnet_image_t srcimage;
#endif
  gasnet_node_t srcnode;
  void *src;
  size_t nbytes;
  size_t dist;
} gasnete_coll_scatterM_args_t;

typedef struct {
#if !GASNET_SEQ
  gasnet_image_t dstimage;
#endif
  gasnet_node_t dstnode;
  void *dst;
  void *src;
  size_t nbytes;
  size_t dist;
} gasnete_coll_gather_args_t;

typedef struct {
#if !GASNET_SEQ
  gasnet_image_t dstimage;
#endif
  gasnet_node_t dstnode;
  void *dst;
  void * *srclist;
  size_t nbytes;
  size_t dist;
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
  gasnete_coll_tree_data_t *tree_info;
  gasnete_coll_dissem_info_t *dissem_info;
  gasnet_handle_t			handle;
  gasnet_coll_handle_t		coll_handle;
  void				*private_data;
  
  
#if GASNET_PAR
  struct {
    gasneti_atomic_t			remaining;
    void				*data;
  }					threads;
#else
  void *addrs;
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
GASNETI_INLINE(all_threads)
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
                                  gasnete_coll_tree_data_t *tree_info, uint32_t sequence
                                  GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_generic_broadcastM_nb(gasnet_team_handle_t team,
                                   void * const dstlist[],
                                   gasnet_image_t srcimage, void *src,
                                   size_t nbytes, int flags,
                                   gasnete_coll_poll_fn poll_fn, int options,
                                   gasnete_coll_tree_data_t *tree_info, uint32_t sequence
                                   GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_generic_scatter_nb(gasnet_team_handle_t team,
                                void *dst,
                                gasnet_image_t srcimage, void *src,
                                size_t nbytes, size_t dist, int flags,
                                gasnete_coll_poll_fn poll_fn, int options,
                                gasnete_coll_tree_data_t *tree_info, uint32_t sequence
                                GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_generic_scatterM_nb(gasnet_team_handle_t team,
                                 void * const dstlist[],
                                 gasnet_image_t srcimage, void *src,
                                 size_t nbytes, size_t dist, int flags,
                                 gasnete_coll_poll_fn poll_fn, int options,
                                 gasnete_coll_tree_data_t *tree_info, uint32_t sequence
                                 GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_generic_gather_nb(gasnet_team_handle_t team,
                               gasnet_image_t dstimage, void *dst,
                               void *src,
                               size_t nbytes, size_t dist, int flags,
                               gasnete_coll_poll_fn poll_fn, int options,
                               gasnete_coll_tree_data_t *tree_info, uint32_t sequence
                               GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_generic_gatherM_nb(gasnet_team_handle_t team,
                                gasnet_image_t dstimage, void *dst,
                                void * const srclist[],
                                size_t nbytes, size_t dist, int flags,
                                gasnete_coll_poll_fn poll_fn, int options,
                                 gasnete_coll_tree_data_t *tree_info, uint32_t sequence
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
                                 void *private_data, gasnete_coll_dissem_info_t *dissem, uint32_t sequence
                                 GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_generic_exchangeM_nb(gasnet_team_handle_t team,
                                  void * const dstlist[], void * const srclist[],
                                  size_t nbytes, int flags,
                                  gasnete_coll_poll_fn poll_fn, int options,
                                  void *private_data, gasnete_coll_dissem_info_t *dissem, uint32_t sequence
                                  GASNETE_THREAD_FARG);




extern gasnete_coll_tree_data_t *gasnete_coll_tree_init(gasnete_coll_tree_type_t tree_type, gasnet_node_t rootnode, gasnete_coll_team_t team GASNETE_THREAD_FARG);
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
			   gasnete_coll_tree_type_t tree_type,
			   uint32_t sequence
			   GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcast_TreePutScratch(gasnet_team_handle_t team,
			   void *dst,
			   gasnet_image_t srcimage, void *src,
			   size_t nbytes, int flags,
			   gasnete_coll_tree_type_t tree_type,
			   uint32_t sequence
			   GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcast_TreePutSeg(gasnet_team_handle_t team,
                              void *dst,
                              gasnet_image_t srcimage, void *src,
                              size_t nbytes, int flags,
                              gasnete_coll_tree_type_t tree_type,
                              uint32_t sequence
                              GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcast_TreeGet(gasnet_team_handle_t team,
			   void *dst,
			   gasnet_image_t srcimage, void *src,
			   size_t nbytes, int flags,
			   gasnete_coll_tree_type_t tree_type,
			   uint32_t sequence
			   GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcast_TreeEager(gasnet_team_handle_t team,
			     void *dst,
			     gasnet_image_t srcimage, void *src,
			     size_t nbytes, int flags,
			     gasnete_coll_tree_type_t tree_type,
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
gasnete_coll_bcastM_TreePut(gasnet_team_handle_t team,
                            void * const dstlist[],
                            gasnet_image_t srcimage, void *src,
                            size_t nbytes, int flags, 
                            gasnete_coll_tree_type_t tree_type,    
                            uint32_t sequence
                            GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcastM_TreePutScratch(gasnet_team_handle_t team,
                                   void * const dstlist[],
                                   gasnet_image_t srcimage, void *src,
                                   size_t nbytes, int flags, 
                                   gasnete_coll_tree_type_t tree_type,    
                                   uint32_t sequence
                                   GASNETE_THREAD_FARG);


extern gasnet_coll_handle_t
gasnete_coll_bcastM_TreePutSeg(gasnet_team_handle_t team,
                               void * const dstlist[],
                               gasnet_image_t srcimage, void *src,
                               size_t nbytes, int flags,
                               gasnete_coll_tree_type_t tree_type,
                               uint32_t sequence
                               GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcastM_Eager(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_bcastM_TreeEager(gasnet_team_handle_t team,
                              void * const dstlist[],
                              gasnet_image_t srcimage, void *src,
                              size_t nbytes, int flags, 
                              gasnete_coll_tree_type_t tree_type,
                              uint32_t sequence
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
gasnete_coll_scat_TreePut(gasnet_team_handle_t team,
			   void *dst,
			   gasnet_image_t srcimage, void *src,
			   size_t nbytes, size_t dist, int flags,
			   gasnete_coll_tree_type_t tree_type,
			   uint32_t sequence
			   GASNETE_THREAD_FARG);


extern gasnet_coll_handle_t
gasnete_coll_scat_TreePutSeg(gasnet_team_handle_t team,
                          void *dst,
                          gasnet_image_t srcimage, void *src,
                          size_t nbytes, int flags,
                          gasnete_coll_tree_type_t tree_type,
                          uint32_t sequence
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
gasnete_coll_scatM_TreePut(gasnet_team_handle_t team,
                              void * const dstlist[],
                              gasnet_image_t srcimage, void *src,
                              size_t nbytes, size_t dist, int flags, 
                              gasnete_coll_tree_type_t tree_type,    
                              uint32_t sequence
                              GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_scatM_TreePutSeg(gasnet_team_handle_t team,
                              void * const dstlist[],
                              gasnet_image_t srcimage, void *src,
                              size_t nbytes, int flags, 
                              gasnete_coll_tree_type_t tree_type,    
                              uint32_t sequence
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
gasnete_coll_gath_TreePut(gasnet_team_handle_t team,
		      gasnet_image_t dstimage, void *dst,
		      void *src,
		      size_t nbytes, size_t dist, int flags, 
                      gasnete_coll_tree_type_t tree_type,
                      uint32_t sequence
                      GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_gath_TreePut(gasnet_team_handle_t team,
                             gasnet_image_t dstimage, void *dst,
                             void *src,
                             size_t nbytes, size_t dist, int flags, 
                             gasnete_coll_tree_type_t tree_type,
                             uint32_t sequence
                             GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_gath_TreePutSeg(gasnet_team_handle_t team,
                          gasnet_image_t dstimage, void *dst,
                          void *src,
                          size_t nbytes, int flags, 
                          gasnete_coll_tree_type_t tree_type,
                          uint32_t sequence
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
gasnete_coll_gathM_TreePut(gasnet_team_handle_t team,
                              gasnet_image_t dstimage, void *dst,
                              void * const srclist[],
                              size_t nbytes, size_t dist, int flags, 
                              gasnete_coll_tree_type_t tree_type,
                              uint32_t sequence
                              GASNETE_THREAD_FARG);
extern gasnet_coll_handle_t
gasnete_coll_gathM_TreePutSeg(gasnet_team_handle_t team,
                             gasnet_image_t dstimage, void *dst,
                             void * const srclist[],
                             size_t nbytes, int flags, 
                             gasnete_coll_tree_type_t tree_type,
                             uint32_t sequence
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

#if 0
extern gasnet_coll_handle_t
gasnete_coll_gall_TreePut(gasnet_team_handle_t team,
                          void *dst, void *src,
                          size_t nbytes, int flags, gasnete_coll_tree_type_t tree_type, 
                          uint32_t sequence
                          GASNETE_THREAD_FARG);
#endif
extern gasnet_coll_handle_t
gasnete_coll_gall_Dissem(gasnet_team_handle_t team,
                          void *dst, void *src,
                          size_t nbytes, int flags,  
                          uint32_t sequence
                          GASNETE_THREAD_FARG);
extern gasnet_coll_handle_t
gasnete_coll_gall_FlatPut(gasnet_team_handle_t team,
                          void *dst, void *src,
                          size_t nbytes, int flags,  
                          uint32_t sequence
                          GASNETE_THREAD_FARG);
extern gasnet_coll_handle_t
gasnete_coll_gall_RingPut(gasnet_team_handle_t team,
                          void *dst, void *src,
                          size_t nbytes, int flags,  
                          uint32_t sequence
                          GASNETE_THREAD_FARG);


/*---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_gallM_Dissem(gasnet_team_handle_t team,
			void * const dstlist[], void * const srclist[],
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_gallM_FlatPut(gasnet_team_handle_t team,
			void * const dstlist[], void * const srclist[],
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_gallM_Gath(gasnet_team_handle_t team,
			void * const dstlist[], void * const srclist[],
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);

/*---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_exchg_Dissem(gasnet_team_handle_t team,
			void *dst, void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_exchg_Gath(gasnet_team_handle_t team,
			void *dst, void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG);

/*---------------------------------------------------------------------------------*/

extern gasnet_coll_handle_t
gasnete_coll_exchgM_Dissem(gasnet_team_handle_t team,
			 void * const dstlist[], void * const srclist[],
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_exchgM_Gath(gasnet_team_handle_t team,
			 void * const dstlist[], void * const srclist[],
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG);

/*---------------------------------------------------------------------------------*/


/*#undef GASNETI_COLL_FN_HEADER*/

#endif
