/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_coll_internal.h $
 * Description: GASNet Collectives conduit header
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */



#ifndef _GASNET_COLL_INTERNAL_H
#define _GASNET_COLL_INTERNAL_H

#define GASNETI_NEED_GASNET_COLL_H 1
#define GASNETI_NEED_GASNET_VIS_H 1
#include <gasnet_internal.h>

#ifdef GASNETE_COLL_NEEDS_CORE
#include <gasnet_core_internal.h>
#endif

#include <coll/gasnet_team.h>

// Upon implementing GASNETI_MEMCPY() (with assertions), it was discovered
// that the collectives have been using GASNETE_FAST_UNALIGNED_MEMCPY() in
// at least some places where GASNETI_MEMCPY_SAFE_IDENTICAL() should
// have been used instead (to allow for src == dst for in-place collectives).
//
// TODO: audit/update the individual calls to GASNETE_FAST_UNALIGNED_MEMCPY
#undef GASNETE_FAST_UNALIGNED_MEMCPY
#define GASNETE_FAST_UNALIGNED_MEMCPY GASNETI_MEMCPY_SAFE_IDENTICAL

#define GASNETI_COLL_FN_HEADER(FNNAME) 
/*---------------------------------------------------------------------------------*/
/* ***  Macros and Constants *** */
/*---------------------------------------------------------------------------------*/


#define GASNETE_COLL_SUBORDINATE	       (1<<30)
#define GASNETE_COLL_USE_SCRATCH         (1<<28)
#define GASNETE_COLL_USE_SCRATCH_TREE    (1<<27)
#define GASNETE_COLL_USE_SCRATCH_DISSSEM (1<<26)
#define GASNETE_COLL_USE_TREE		         (1<<25)
#define GASNETE_COLL_NONROOT_SUBORDINATE (1<<24)
#define GASNETE_COLL_SKIP (1<<23)

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
/* ***  Handler Indices *** */
/*---------------------------------------------------------------------------------*/
/* conduits may override this to relocate the ref-coll handlers */
#ifndef GASNETE_COLL_HANDLER_BASE
#define GASNETE_COLL_HANDLER_BASE 119
#endif

#define _hidx_gasnete_coll_p2p_memcpy_reqh          (GASNETE_COLL_HANDLER_BASE+0)
#define _hidx_gasnete_coll_p2p_short_reqh           (GASNETE_COLL_HANDLER_BASE+1)
#define _hidx_gasnete_coll_p2p_med_reqh             (GASNETE_COLL_HANDLER_BASE+2)
#define _hidx_gasnete_coll_p2p_long_reqh            (GASNETE_COLL_HANDLER_BASE+3)
#define _hidx_gasnete_coll_p2p_med_tree_reqh        (GASNETE_COLL_HANDLER_BASE+4)
#define _hidx_gasnete_coll_p2p_advance_reqh         (GASNETE_COLL_HANDLER_BASE+5)
#define _hidx_gasnete_coll_p2p_put_and_advance_reqh (GASNETE_COLL_HANDLER_BASE+6)
#define _hidx_gasnete_coll_p2p_med_counting_reqh    (GASNETE_COLL_HANDLER_BASE+7)
#define _hidx_gasnete_coll_p2p_seg_put_reqh         (GASNETE_COLL_HANDLER_BASE+8)

// TODO-EX: this is taking the last extended handler slot, but should be allocated more "normally"
#define GASNETE_COLL_NUM_SCRATCH_HANDLERS 1
#ifndef GASNETE_COLL_SCRATCH_HANDLER_BASE
#define GASNETE_COLL_SCRATCH_HANDLER_BASE (GASNETE_COLL_HANDLER_BASE-GASNETE_COLL_NUM_SCRATCH_HANDLERS)
#endif
#define _hidx_gasnete_coll_scratch_update_reqh (GASNETE_COLL_SCRATCH_HANDLER_BASE+0)

// TODO-EX: this is taking the next-to-last extended handler slot, but should be allocated more "normally"
#define GASNETE_COLL_NUM_TEAM_HANDLERS 1
#ifndef GASNETE_COLL_TEAM_HANDLER_BASE
#define GASNETE_COLL_TEAM_HANDLER_BASE (GASNETE_COLL_SCRATCH_HANDLER_BASE-GASNETE_COLL_NUM_SCRATCH_HANDLERS)
#endif
#define _hidx_gasnete_coll_teamid_reqh (GASNETE_COLL_TEAM_HANDLER_BASE+0)

/*---------------------------------------------------------------------------------*/
/* Forward type decls and typedefs:                                                */
/*---------------------------------------------------------------------------------*/
struct gasnete_coll_op_t_;
typedef struct gasnete_coll_op_t_ gasnete_coll_op_t;

struct gasnete_coll_p2p_t_;
typedef struct gasnete_coll_p2p_t_ gasnete_coll_p2p_t;

union gasnete_coll_p2p_entry_t_;
typedef union gasnete_coll_p2p_entry_t_ gasnete_coll_p2p_entry_t;

struct gasnete_tm_p2p_send_struct;
typedef struct gasnete_tm_p2p_send_struct gasnete_tm_p2p_send_struct_t;

struct gasnete_coll_generic_data_t_;
typedef struct gasnete_coll_generic_data_t_ gasnete_coll_generic_data_t;

struct gasnete_coll_tree_type_t_;
typedef struct gasnete_coll_tree_type_t_ *gasnete_coll_tree_type_t;

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


struct gasnete_coll_implementation_t_;
typedef struct gasnete_coll_implementation_t_ *gasnete_coll_implementation_t;

/*---------------------------------------------------------------------------------*/

extern size_t gasnete_coll_p2p_eager_min;
extern size_t gasnete_coll_p2p_eager_scale;
extern size_t gasnete_coll_p2p_eager_buffersz;


#ifndef GASNETE_COLL_IMAGE_OVERRIDE
/* gasnet_image_t must be large enough to index all threads that participate
* in collectives.  A conduit may override this if a smaller type will suffice.
* However, types larger than 32-bits won't pass as AM handler args.  So, for
* a larger type, many default things will require overrides.
*/
#define gasnete_coll_image_is_local(TEAM, I)	((TEAM)->myrank == (I))
#endif

// gasnete_coll_eop_t is either an eop or linked-list thereof
#ifndef GASNETE_COLL_HANDLE_OVERRIDE
# if GASNET_PAR
    typedef struct gasnete_coll_eop_t_ {
      gasneti_eop_t              *eop;
      struct gasnete_coll_eop_t_ *next;
    } *gasnete_coll_eop_t;
# else
    typedef gasneti_eop_t *gasnete_coll_eop_t;
# endif
#endif

extern gasnete_coll_eop_t gasnete_coll_eop_create(GASNETI_THREAD_FARG_ALONE);
extern void gasnete_coll_eop_signal(gasnete_coll_eop_t eop GASNETI_THREAD_FARG);

/** Need to insert this here so that trees.h picks up all the forward declaration of the structs*/
/** but also before the internal strucutres use the trees*/
#include <coll/gasnet_trees.h>

/*---------------------------------------------------------------------------------*/
/* Operations of the active list */

extern void gasnete_coll_active_init(void);
extern void gasnete_coll_active_fini(void);
/*---------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------*/
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

#define GASNETE_COLL_MAX_NUM_SEGS 2048
/*---------------------------------------------------------------------------------*/
/* Type for global synchronization */

#ifndef GASNETE_COLL_CONSENSUS_OVERRIDE
/* Scalar type, could be a pointer to a struct */
typedef uint32_t gasnete_coll_consensus_t;
#if 0
struct gasnete_coll_consensus_t_ {
  struct gasnete_coll_consensus_t_* next; /*linkage for free list*/
  uint32_t id;
  gex_Event_t handle; /*handle for the nonblocking*/
};

typedef struct gasnete_coll_consensus_t_* gasnete_coll_consensus_t;
#endif
#endif

extern gasnete_coll_consensus_t gasnete_coll_consensus_create(gasnete_coll_team_t team);
extern void gasnete_coll_consensus_free(gasnete_coll_team_t team, gasnete_coll_consensus_t consensus);
extern int gasnete_coll_consensus_try(gasnete_coll_team_t team, gasnete_coll_consensus_t id);
extern int gasnete_coll_consensus_barrier(gasnete_coll_team_t team GASNETI_THREAD_FARG);

/*---------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------*/
/*data structure to contain state for team barrier*/

#ifndef GASNETE_AMDBARRIER_MAXSTEP
#define GASNETE_AMDBARRIER_MAXSTEP 32
#endif

typedef void (*gasnete_all_barrier_notify)(gasnete_coll_team_t team, int id, int flags);
typedef int (*gasnete_all_barrier_wait)(gasnete_coll_team_t team, int id, int flags);
typedef int (*gasnete_all_barrier_try)(gasnete_coll_team_t team, int id, int flags);
typedef int (*gasnete_all_barrier)(gasnete_coll_team_t team, int id, int flags);
typedef int (*gasnete_all_barrier_result)(gasnete_coll_team_t team, int *id);

typedef enum {
  GASNETE_COLL_BARRIER_ENVDEFAULT=0,
  GASNETE_COLL_BARRIER_DISSEM,
  GASNETE_COLL_BARRIER_AMDISSEM,
  GASNETE_COLL_BARRIER_RDMADISSEM,
  GASNETE_COLL_BARRIER_AMCENTRAL
#ifdef GASNETE_COLL_CONDUIT_BARRIERS
  , GASNETE_COLL_CONDUIT_BARRIERS
#endif
} gasnete_coll_barrier_type_t;

/* intialize the barriers for a given team */
extern void gasnete_coll_barrier_init(gasnete_coll_team_t _team, int _barrier_type,
                                      gex_Rank_t *_nodes, gex_Rank_t *_super_nodes);

/* "peers" are sets of nodes at distances +/- powers of two, taken from some parent set */
typedef struct {
  unsigned int   num; /* ceil(log_2(ranks)) */
  gex_Rank_t *fwd; /* fwd[i] is global rank of member (myrank + 2^i) */
#if 0 /* Not used yet */
  gex_Rank_t *bwd; /* bwd[i] is global rank of member (myrank - 2^i) */
#endif
} gasnete_coll_peer_list_t;

/* Type for collective teams: */
struct gasnete_coll_team_t_ {
  /* read-only fields: */
  uint32_t			team_id;
  int					global_team;
  gex_TM_t                      e_tm;
  
  /* tree geometry cache, each team should have its own cache .... */
  gasnete_coll_tree_geom_t *tree_geom_cache_head;
  gasnete_coll_tree_geom_t *tree_geom_cache_tail;
  gasneti_mutex_t tree_geom_cache_lock;
  void *tree_construction_scratch;
  
  /*dissem geometry cache, each team should have its own  ... */
  gasnete_coll_dissem_info_t *dissem_cache_head;
  gasnete_coll_dissem_info_t *dissem_cache_tail;
  gasneti_mutex_t dissem_cache_lock;
  
  /*my relative node id in this team*/
  gex_Rank_t myrank;
  
  /*total number of members in this team*/
  gex_Rank_t total_ranks;
  

  /* ranks of the processes in the team */
  gex_Rank_t *rel2act_map; /* need to be initialized */

  /* nodes in the team at distances +/- powers of two */
  gasnete_coll_peer_list_t peers;

#if GASNET_PSHM
  /* Info about the supernode(s) */
  struct {
    gex_Rank_t node_count;
    gex_Rank_t node_rank;
    gex_Rank_t grp_count;
    gex_Rank_t grp_rank;
  } supernode;
  /* supernode-reps in the team at distances +/- powers of two in supernode space */
  gasnete_coll_peer_list_t supernode_peers;
#endif

  /* scratch segments allocated on team creation*/
  gasnet_seginfo_t *scratch_segs;
  size_t smallest_scratch_seg;
  
  /*scratch space management*/
  gasnete_coll_scratch_status_t* scratch_status;
  gasnete_coll_scratch_req_t* scratch_free_list;
  
  /*autotuning info*/
  gasnete_coll_autotune_info_t* autotune_info;
  
  uint32_t sequence;	/* arbitrary non-zero starting value */

#if GASNET_PAR && GASNET_DEBUG
  gasneti_mutex_t threads_mutex;
#endif
  
  /*Stuff for consensus*/
  uint32_t consensus_issued_id;
  uint32_t consensus_id;
  
  /*Stuff for barrier*/
  enum { OUTSIDE_BARRIER, INSIDE_BARRIER } barrier_splitstate;
  void *barrier_data;
  gasnete_all_barrier_notify barrier_notify;
  gasnete_all_barrier_try barrier_try;
  gasnete_all_barrier_wait barrier_wait;
  gasnete_all_barrier barrier;
  gasnete_all_barrier_result barrier_result;
  gasneti_progressfn_t barrier_pf;
  
#ifndef GASNETE_COLL_P2P_OVERRIDE
  /* Default implementation of point-to-point syncs */
  #ifndef GASNETE_COLL_P2P_TABLE_SIZE
    #define GASNETE_COLL_P2P_TABLE_SIZE 16
  #endif

  gex_HSL_t p2p_lock; /* Protects freelist and table */
  gasnete_coll_p2p_t *p2p_freelist;
  gasnete_coll_p2p_t *p2p_table[GASNETE_COLL_P2P_TABLE_SIZE];
#endif
  
  /* Hook for conduit-specific extensions/overrides */
#ifdef GASNETE_COLL_TEAM_EXTRA
  GASNETE_COLL_TEAM_EXTRA
#endif
};

#define GASNETE_COLL_REL2ACT(TEAM, IDX) ((TEAM) == GASNET_TEAM_ALL ? IDX : (TEAM)->rel2act_map[IDX])

/*---------------------------------------------------------------------------------*/

/* Serialization of polling collective ops: */
extern gasneti_mutex_t gasnete_coll_poll_lock;

/* Function pointer type for polling collective ops: */
typedef int (*gasnete_coll_poll_fn)(gasnete_coll_op_t* GASNETI_THREAD_FARG);

/* Type for collective ops: */
struct gasnete_coll_op_t_ {
  /* Linkage used by the thread-specific active ops list. */
#ifndef GASNETE_COLL_LIST_OVERRIDE
  /* Default implementation of coll_ops active list */
  gasnete_coll_op_t	*active_next, **active_prev_p;
#endif
  
  /* a list of the ops for the scratch list management*/
  gasnete_coll_op_t *scratch_next, *scratch_prev;
  
  /* Read-only fields: */
  gasnete_coll_team_t		team;
  gex_TM_t                      e_tm;
  uint32_t			sequence;
  int				flags;
  gasnete_coll_eop_t            eop; // a container of eops for PAR
  
  /* Per-instance fields: */
  void			*data;
  gasnete_coll_poll_fn	poll_fn;
  
  /* positions of the valid scratch space for this operation on the peers*/
  uintptr_t *scratchpos;
  uintptr_t myscratchpos;
  uint8_t active_scratch_op; /* is this op on the active scratch list?*/
  uint8_t waiting_scratch_op; /* is this op on the waiting scratch list?*/
  uint8_t waiting_for_reconfig_clear;
#if GASNET_DEBUG
  uint8_t scratch_op_freed;
#endif
  gasnete_coll_scratch_req_t *scratch_req; /* the associated scratch request with this op*/
  int num_coll_params;
  uint32_t param_list[GASNET_COLL_NUM_PARAM_TYPES];/*contains teh parameters*/
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
  gasnete_coll_p2p_t	**p2p_prev_p;
  
  /* Unique (team_id, sequence) tuple for the associated op */
#if GASNET_DEBUG
  uint32_t		team_id; /* Only needed when debugging */
#endif
  uint32_t		sequence;
  
  /* Volatile arrays of data and state for the point-to-point synchronization */
  uint8_t		*data;
  volatile uint32_t	*state;
  gasneti_weakatomic_t	*counter;
    
  /* Handler-safe lock (if needed) */
  gex_HSL_t		lock;
  
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

extern gasnete_coll_p2p_t *gasnete_coll_p2p_get(uint32_t team_id, uint32_t sequence);
extern void gasnete_coll_p2p_destroy(gasnete_coll_p2p_t *p2p);
extern void gasnete_coll_p2p_counting_put(gasnete_coll_op_t *op, gex_Rank_t dstnode, void *dst,
                                          void *src, size_t nbytes, uint32_t idx);
extern void gasnete_coll_p2p_counting_eager_put(gasnete_coll_op_t *op, gex_Rank_t dstnode,
                                                void *src, size_t nbytes, size_t offset_size, uint32_t offset, uint32_t idx);
extern void gasnete_coll_p2p_counting_putAsync(gasnete_coll_op_t *op, gex_Rank_t dstnode, void *dst,
                                               void *src, size_t nbytes, uint32_t idx);
extern void gasnete_coll_p2p_eager_put_tree(gasnete_coll_op_t *op, gex_Rank_t dstnode,
                                            void *src, size_t size);

extern int gasnete_tm_p2p_send_rtr(
                        gasnete_coll_op_t *op, gasnete_coll_p2p_t *p2p,
                        gex_Rank_t rank, uint32_t offset,
                        void *dst, size_t nbytes,
                        gex_Flags_t flags GASNETI_THREAD_FARG);
extern int gasnete_tm_p2p_send_data(
                        gasnete_coll_op_t *op, gasnete_coll_p2p_t *p2p,
                        gex_Rank_t rank, uint32_t offset,
                        const void *src, size_t nbytes,
                        gex_Flags_t flags GASNETI_THREAD_FARG);
GASNETI_INLINE(gasnete_tm_p2p_send_done)
int gasnete_tm_p2p_send_done(gasnete_coll_p2p_t *p2p) {
  // NOTE: caller is responsible for ACQ, if any
  return (0 == gasneti_weakatomic_read(&p2p->counter[0], GASNETI_ATOMIC_NONE));
}
struct gasnete_tm_p2p_send_struct { void *addr; size_t sent; };

/* Update one or more states w/o delivering any data */
GASNETI_INLINE(gasnete_tm_p2p_change_states)
int gasnete_tm_p2p_change_states(
                        gasnete_coll_op_t *op,
                        gex_TM_t tm, gex_Rank_t rank,
                        gex_Flags_t flags,
                        uint32_t count, uint32_t offset, uint32_t state
                        GASNETI_THREAD_FARG)
{
  // TODO-EX: flags |= INTERNAL to prevent tracing
  return gex_AM_RequestShort5(tm, rank, gasneti_handleridx(gasnete_coll_p2p_short_reqh), flags,
                              op->team->team_id, op->sequence, count, offset, state);
}
#define gasnete_tm_p2p_change_state(op,tm,rank,flags,offset,stateTI) \
        gasnete_tm_p2p_change_states(op,tm,rank,flags,1,offset,stateTI)

// TODO-EX: deprecate and remove:
#define gasnete_coll_p2p_change_states(op,node,count,offset,state) \
        gasneti_assert_zeroret(                                \
        gasnete_tm_p2p_change_states(op,gasneti_THUNK_TM,node, \
                                     0,count,offset,state GASNETI_THREAD_GET))
#define gasnete_coll_p2p_change_state(op,node,offset,state) \
        gasneti_assert_zeroret(                                \
        gasnete_tm_p2p_change_states(op,gasneti_THUNK_TM,node, \
                                     0,1,offset,state GASNETI_THREAD_GET))

/* Advance counter[idx] */
GASNETI_INLINE(gasnete_tm_p2p_advance)
int gasnete_tm_p2p_advance(
                        gasnete_coll_op_t *op,
                        gex_TM_t tm, gex_Rank_t rank,
                        gex_Flags_t flags, uint32_t idx
                        GASNETI_THREAD_FARG)
{
  // TODO-EX: flags |= INTERNAL to prevent tracing
  return gex_AM_RequestShort3(tm, rank, gasneti_handleridx(gasnete_coll_p2p_advance_reqh), flags,
                              op->team->team_id, op->sequence, idx);
}

// TODO-EX: deprecate and remove:
#define gasnete_coll_p2p_advance(op,node,idx) \
        gasneti_assert_zeroret(                                \
        gasnete_tm_p2p_advance(op,gasneti_THUNK_TM,node,0,idx GASNETI_THREAD_GET))


/* Put up to gex_AM_LUBRequestLong() bytes, signalling the recipient */
GASNETI_INLINE(gasnete_tm_p2p_signalling_put)
int gasnete_tm_p2p_signalling_put(
                        gasnete_coll_op_t *op,
                        gex_TM_t tm, gex_Rank_t rank,
                        void *dst, const void *src, size_t nbytes,
                        gex_Event_t *lc_opt, gex_Flags_t flags,
                        uint32_t offset, uint32_t state
                        GASNETI_THREAD_FARG)
{
  // TODO-EX: flags |= INTERNAL to prevent tracing
  return gex_AM_RequestLong5(tm, rank, gasneti_handleridx(gasnete_coll_p2p_long_reqh),
                             (void*)src, nbytes, dst, lc_opt, flags,
                             op->team->team_id, op->sequence, 1, offset, state);
}


// TODO-EX: deprecate and remove:
#define gasnete_coll_p2p_signalling_put(op,node,dst,src,nbytes,pos,state)            \
        gasneti_assert_zeroret(                                                      \
        gasnete_tm_p2p_signalling_put(op,gasneti_THUNK_TM,node,dst,src,nbytes,       \
                                      GEX_EVENT_NOW,0,pos,state GASNETI_THREAD_GET))
// NOTE: this has *not* been Async since the loss of LongAsync
#define gasnete_coll_p2p_signalling_putAsync gasnete_coll_p2p_signalling_put


/* Treat the eager buffer space at dstnode as an array of elements of length 'size'.
* Copy 'count' elements to that buffer, starting at element 'offset' at the destination.
* Set the corresponding entries of the state array to 'state'.
*/
extern int gasnete_tm_p2p_eager_putM(
                        gasnete_coll_op_t *op,
                        gex_TM_t tm, gex_Rank_t rank,
                        const void *src, uint32_t count, size_t size,
                        gex_Event_t *lc_opt, gex_Flags_t flags,
                        uint32_t offset, uint32_t state
                        GASNETI_THREAD_FARG);

// Specialization of gasnete_tm_p2p_eager_putM with count == 1
GASNETI_INLINE(gasnete_tm_p2p_eager_put)
int gasnete_tm_p2p_eager_put(
                        gasnete_coll_op_t *op,
                        gex_TM_t tm, gex_Rank_t rank,
                        const void *src, size_t size,
                        gex_Event_t *lc_opt, gex_Flags_t flags,
                        uint32_t offset, uint32_t state
                        GASNETI_THREAD_FARG)
{
  // TODO-EX: flags |= INTERNAL to prevent tracing
  return gex_AM_RequestMedium6(tm, rank, gasneti_handleridx(gasnete_coll_p2p_med_reqh),
                               (void*)src, size, lc_opt, flags,
                               op->team->team_id, op->sequence, 1, offset, state, size);
}
    

// TODO-EX: deprecate and remove
#define gasnete_coll_p2p_eager_putM(op,node,src,count,size,offset,state)            \
        gasneti_assert_zeroret(                                                     \
        gasnete_tm_p2p_eager_putM(op,gasneti_THUNK_TM,node,src,count,size,          \
                                  GEX_EVENT_NOW,0,offset,state GASNETI_THREAD_GET))
#define gasnete_coll_p2p_eager_put(op,node,src,size,offset,state)                   \
        gasneti_assert_zeroret(                                                     \
        gasnete_tm_p2p_eager_put(op,gasneti_THUNK_TM,node,src,size,                 \
                                 GEX_EVENT_NOW,0,offset,state GASNETI_THREAD_GET))


/* Treat the eager buffer space at dstnode as an array of (void *)s.
* Copy 'count' elements to that buffer, starting at element 'offset' at the destination.
* Set the corresponding entries of the state array to 'state'.
*/
#ifndef gasnete_coll_p2p_eager_addrM
GASNETI_INLINE(gasnete_coll_p2p_eager_addrM)
void gasnete_coll_p2p_eager_addrM(gasnete_coll_op_t *op, gex_Rank_t dstnode,
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
void gasnete_coll_p2p_eager_addr(gasnete_coll_op_t *op, gex_Rank_t dstnode,
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
  gex_Rank_t i;
  
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
                                     uint32_t offset, uint32_t state, gasnet_team_handle_t team) {
  gex_Rank_t i;
  
  /* Send to nodes to the "right" of ourself */
  for (i = team->myrank + 1; i < team->total_ranks; ++i) {
    gasnete_coll_p2p_eager_addr(op, GASNETE_COLL_REL2ACT(team, i), addr, offset, state);
  }
  /* Send to nodes to the "left" of ourself */
  for (i = 0; i < team->myrank; ++i) {
    gasnete_coll_p2p_eager_addr(op, GASNETE_COLL_REL2ACT(team, i), addr, offset, state);
  }
}
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

/* Helper to perform in-memory rotation */
GASNETI_INLINE(gasnete_coll_local_rotate_left)
void gasnete_coll_local_rotate_left(void *dst, const void *src, size_t elem_size, size_t num_elem, int rotation_amt) {
  gasneti_sync_reads();
  gasneti_assert(rotation_amt >= 0);
  GASNETI_MEMCPY_SAFE_EMPTY(dst, gasnete_coll_scale_ptr(src, elem_size, rotation_amt), (num_elem-rotation_amt)*elem_size);
  GASNETI_MEMCPY_SAFE_EMPTY(gasnete_coll_scale_ptr(dst, elem_size, num_elem-rotation_amt), src, (rotation_amt)*elem_size);
  gasneti_sync_writes();
}
GASNETI_INLINE(gasnete_coll_local_rotate_right)
void gasnete_coll_local_rotate_right(void *dst, const void *src, size_t elem_size, size_t num_elem, int rotation_amt) {
  /*make sure we can read the data*/
  gasneti_sync_reads();
  gasneti_assert(rotation_amt >= 0);
  GASNETI_MEMCPY_SAFE_EMPTY(gasnete_coll_scale_ptr(dst, elem_size, rotation_amt), src,(num_elem-rotation_amt)*elem_size);
  GASNETI_MEMCPY_SAFE_EMPTY(dst, gasnete_coll_scale_ptr(src, elem_size, num_elem-rotation_amt), (rotation_amt)*elem_size);
  gasneti_sync_writes();
}


/* Helper to perform in-memory broadcast */
GASNETI_INLINE(gasnete_coll_local_broadcast)
void gasnete_coll_local_broadcast(size_t count, void * const dstlist[], const void *src, size_t nbytes) {
  /* XXX: this could/should be segemented to cache reuse */
  while (count>0) {
    GASNETI_MEMCPY_SAFE_IDENTICAL(*dstlist, src, nbytes);
    dstlist++;
    count--;
  }
  gasneti_sync_writes();	/* Ensure result is visible on all threads */
}

/* Helper to perform in-memory scatter */
GASNETI_INLINE(gasnete_coll_local_scatter)
void gasnete_coll_local_scatter(size_t count, void * const dstlist[], const void *src, size_t nbytes) {
  const uint8_t *src_addr = (const uint8_t *)src;
  
  while (count>0) {
    GASNETI_MEMCPY_SAFE_IDENTICAL(*dstlist, src_addr, nbytes);
    dstlist++;
    src_addr += nbytes;
    count --;
  }
  gasneti_sync_writes();	/* Ensure result is visible on all threads */
}

/* Helper to perform in-memory gather */
GASNETI_INLINE(gasnete_coll_local_gather)
void gasnete_coll_local_gather(size_t count, void * dst, void * const srclist[], size_t nbytes) {
  uint8_t *dst_addr = (uint8_t *)dst;
  gasneti_sync_reads();
  while (count>0) {
    GASNETI_MEMCPY_SAFE_IDENTICAL(dst_addr, *srclist, nbytes);
    dst_addr += nbytes;
    srclist++;
    count--;
  }
  gasneti_sync_writes();	/* Ensure result is visible on all threads */
}


/*---------------------------------------------------------------------------------*/
/* Thread-specific data: */
struct gasnete_coll_threaddata_t_ {
  gasnete_coll_op_t			*op_freelist;
  gasnete_coll_generic_data_t 	*generic_data_freelist;
  
  /* Linkage used by the thread-specific handle freelist . */
#ifndef GASNETE_COLL_HANDLE_OVERRIDE
#if GASNET_PAR
  /* Default implementation of eop freelist */
  gasnete_coll_eop_t                     eop_freelist;
#endif
#endif
  
  /* Linkage used by the thread-specific active ops list. */
#ifndef GASNETE_COLL_LIST_OVERRIDE
  /* Default implementation of coll_ops active list */
#endif
  
  /* Recursion control */
  int in_poll;

  /* Macro for conduit-specific extension */
#ifdef GASNETE_COLL_THREADDATA_EXTRA
  GASNETE_COLL_THREADDATA_EXTRA
#endif
};

extern gasnete_coll_threaddata_t *gasnete_coll_new_threaddata(void);

GASNETI_INLINE(_gasnete_coll_get_threaddata)
gasnete_coll_threaddata_t *
_gasnete_coll_get_threaddata(gasneti_threaddata_t *mythread) {
  gasnete_coll_threaddata_t *result = mythread->gasnete_coll_threaddata;
  if_pf (result == NULL) {
    mythread->gasnete_coll_threaddata = result = gasnete_coll_new_threaddata();
  }
  return result;
}

/* Used when thread data might not exist yet */
#define GASNETE_COLL_MYTHREAD        _gasnete_coll_get_threaddata(GASNETI_MYTHREAD)

/* Used when thread data must already exist */
#define GASNETE_COLL_MYTHREAD_NOALLOC \
(gasneti_assert(GASNETI_MYTHREAD->gasnete_coll_threaddata), GASNETI_MYTHREAD->gasnete_coll_threaddata)

/*---------------------------------------------------------------------------------*/

extern gasnete_coll_team_t gasnete_coll_team_lookup(uint32_t team_id);

extern gasnete_coll_op_t *
gasnete_coll_op_create(gasnete_coll_team_t team, uint32_t sequence, int flags GASNETI_THREAD_FARG);
extern void
gasnete_coll_op_destroy(gasnete_coll_op_t *op GASNETI_THREAD_FARG);

/*---------------------------------------------------------------------------------*/
/* Debugging and tracing macros */

#if GASNET_DEBUG
/* Argument validation */
extern void gasnete_coll_validate(gasnet_team_handle_t team,
                                  gex_Rank_t dstrank, const void *dstaddr, size_t dstlen,
                                  gex_Rank_t srcrank, const void *srcaddr, size_t srclen,
                                  int flags GASNETI_THREAD_FARG);
#define GASNETE_COLL_VALIDATE(T,DI,DA,DL,SI,SA,SL,F) \
gasnete_coll_validate(T,DI,DA,DL,SI,SA,SL,F GASNETI_THREAD_PASS)
#else
#define GASNETE_COLL_VALIDATE(T,DI,DA,DL,SI,SA,SL,F)
#endif

#define GASNETE_COLL_VALIDATE_BROADCAST(T,D,R,S,N,F)   \
GASNETE_COLL_VALIDATE(T,GEX_RANK_INVALID,D,N,R,S,N,F)

#define GASNETE_COLL_VALIDATE_SCATTER(T,D,R,S,N,F)   \
GASNETE_COLL_VALIDATE(T,GEX_RANK_INVALID,D,N,R,S,(N)*gasneti_nodes,F)

#define GASNETE_COLL_VALIDATE_GATHER(T,R,D,S,N,F)     \
GASNETE_COLL_VALIDATE(T,R,D,(N)*gasneti_nodes,GEX_RANK_INVALID,S,N,F)

#define GASNETE_COLL_VALIDATE_GATHER_ALL(T,D,S,N,F)                \
GASNETE_COLL_VALIDATE(T,GEX_RANK_INVALID,D,(N)*gasneti_nodes,GEX_RANK_INVALID,S,N,F)

#define GASNETE_COLL_VALIDATE_EXCHANGE(T,D,S,N,F)                  \
GASNETE_COLL_VALIDATE(T,GEX_RANK_INVALID,D,(N)*gasneti_nodes,GEX_RANK_INVALID,S,(N)*gasneti_nodes,F)

/* XXX: following arg validation unimplemented */
#define GASNETE_COLL_VALIDATE_REDUCE(T,DI,D,S,SB,SO,ES,EC,FN,FA,F)


/*---------------------------------------------------------------------------------*/
/* Forward decls and macros */

#define GASNETE_COLL_FORWARD_FLAGS(flags) \
(((flags) & ~(GASNET_COLL_IN_ALLSYNC|GASNET_COLL_IN_MYSYNC|\
              GASNET_COLL_OUT_ALLSYNC|GASNET_COLL_OUT_MYSYNC)) \
 | (GASNET_COLL_IN_NOSYNC|GASNET_COLL_OUT_NOSYNC|GASNETE_COLL_SUBORDINATE))

/*---------------------------------------------------------------------------------*/
/* In-segment checks */

/*
* For a purely AM based conduit internal in-segment checks might always be true and other
* conduits may also override this to allow for regions outside the normal
* segment.  Note that this override relies on the fact that the gasnete_ calls
* don't perform bounds checking on their own 
*/
#if defined(GASNET_SEGMENT_EVERYTHING) || defined(GASNETI_SUPPORTS_OUTOFSEGMENT_PUTGET)
#define GASNETE_COLL_ALWAYS_IN_SEGMENT 1
#else
#define GASNETE_COLL_ALWAYS_IN_SEGMENT 0
#endif

/* The flags GASNET_COLL_SRC_IN_SEGMENT and GASNET_COLL_DST_IN_SEGMENT are just
* assertions from the caller.  If they are NOT set, we will try to determine (when
* possible) if the addresses are in-segment to allow a one-sided implementation
* to be used.
* gasnete_coll_segment_check returns a new set of flags.
*/
#ifndef gasnete_coll_segment_check
#if GASNETE_COLL_ALWAYS_IN_SEGMENT
GASNETI_INLINE(gasnete_coll_segment_check)
int gasnete_coll_segment_check(gasnete_coll_team_t team, int flags, 
                               int dstrooted, gasnet_image_t dstimage, const void *dst, size_t dstlen,
                               int srcrooted, gasnet_image_t srcimage, const void *src, size_t srclen) {
  /* Everything is reachable via get/put, regardless of segment */
  return (flags | GASNET_COLL_DST_IN_SEGMENT | GASNET_COLL_SRC_IN_SEGMENT);
}
#else
GASNETI_INLINE(gasnete_coll_segment_check)
int gasnete_coll_segment_check(gasnete_coll_team_t team, int flags, 
                               int dstrooted, gasnet_image_t dstimage, const void *dst, size_t dstlen,
                               int srcrooted, gasnet_image_t srcimage, const void *src, size_t srclen) {
  /* Only (removed) single-valued addresing benefited here */
  return flags;
}
#endif
#endif

/*---------------------------------------------------------------------------------*/
/* Events to test for progress */

extern void gasnete_coll_save_event(gex_Event_t *event_p);
extern void gasnete_coll_sync_saved_events(GASNETI_THREAD_FARG_ALONE);

/*---------------------------------------------------------------------------------*
* Start of generic framework for reference implementations
*---------------------------------------------------------------------------------*/

typedef struct {
  void *dst;
  gex_Rank_t srcnode;
  void *src;
  size_t nbytes;
} gasnete_coll_broadcast_args_t;

typedef struct {
  void *dst;
  gex_Rank_t srcnode;
  void *src;
  size_t nbytes;
  size_t dist;
} gasnete_coll_scatter_args_t;

typedef struct {
  gex_Rank_t dstnode;
  void *dst;
  void *src;
  size_t nbytes;
  size_t dist;
} gasnete_coll_gather_args_t;

typedef struct {
  void *dst;
  void *src;
  size_t nbytes;
} gasnete_coll_gather_all_args_t;

typedef struct {
  void *dst;
  void *src;
  size_t nbytes;
} gasnete_coll_exchange_args_t;

typedef struct {
  gex_Rank_t          root;
  void *              dst;
  const void *        src;
  gex_DT_t            dt;
  size_t              dt_sz;
  size_t              dt_cnt;
  gex_Coll_ReduceFn_t op_fnptr;
  void *              op_cdata;
} gasnete_tm_reduce_args_t;

typedef struct {
  void *              dst;
  const void *        src;
  gex_DT_t            dt;
  size_t              dt_sz;
  size_t              dt_cnt;
  gex_OP_t            opcode;
  gex_Coll_ReduceFn_t op_fnptr;
  void *              op_cdata;
} gasnete_tm_reduce_all_args_t;

/* Options for gasnete_coll_generic_* */
#define GASNETE_COLL_GENERIC_OPT_INSYNC		0x0001
#define GASNETE_COLL_GENERIC_OPT_OUTSYNC	0x0002
#define GASNETE_COLL_GENERIC_OPT_P2P		0x0004

/* Macros for conditionally setting flags in gasnete_coll_generic_* options */
#define GASNETE_COLL_GENERIC_OPT_INSYNC_IF(COND)	((COND) ? GASNETE_COLL_GENERIC_OPT_INSYNC : 0)
#define GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(COND)	((COND) ? GASNETE_COLL_GENERIC_OPT_OUTSYNC : 0)
#define GASNETE_COLL_GENERIC_OPT_P2P_IF(COND)		((COND) ? GASNETE_COLL_GENERIC_OPT_P2P : 0)

struct gasnete_coll_generic_data_t_ {
#if GASNET_DEBUG
#define GASNETE_COLL_GENERIC_TAG(T)	_CONCAT(GASNETE_COLL_GENERIC_TAG_,T)
#define GASNETE_COLL_GENERIC_SET_TAG(D,T)	(D)->tag = GASNETE_COLL_GENERIC_TAG(T)
  
  enum {
    /* Single-address (legacy) interfaces: */
    GASNETE_COLL_GENERIC_TAG(broadcast),
    GASNETE_COLL_GENERIC_TAG(scatter),
    GASNETE_COLL_GENERIC_TAG(gather),
    GASNETE_COLL_GENERIC_TAG(gather_all),
    GASNETE_COLL_GENERIC_TAG(exchange),
    GASNETE_COLL_GENERIC_TAG(reduce),

    /* GEX (tm-based) interfaces: */
    GASNETE_COLL_GENERIC_TAG(tm_reduce),
    GASNETE_COLL_GENERIC_TAG(tm_reduce_all)
    
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
  gasnete_coll_local_tree_geom_t        *tree_geom;
  gasnete_coll_dissem_info_t *dissem_info;
  gex_Event_t			handle;
  gex_Event_t			handle2;
  gex_Event_t		coll_handle;
  void				*private_data;
  
  /* Hook for conduit-specific extension */
#ifdef GASNETE_COLL_GENERIC_EXTRA
  GASNETE_COLL_GENERIC_EXTRA
#endif
    
    union {
      /* Single-address (legacy) interfaces: */
      gasnete_coll_broadcast_args_t		broadcast;
      gasnete_coll_scatter_args_t		scatter;
      gasnete_coll_gather_args_t		gather;
      gasnete_coll_gather_all_args_t		gather_all;
      gasnete_coll_exchange_args_t		exchange;

      /* GEX interfaces: */
      gasnete_tm_reduce_args_t                  tm_reduce;
      gasnete_tm_reduce_all_args_t              tm_reduce_all;

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


extern gasnete_coll_generic_data_t *gasnete_coll_generic_alloc(GASNETI_THREAD_FARG_ALONE);
void gasnete_coll_generic_free(gasnete_coll_team_t team, gasnete_coll_generic_data_t *data GASNETI_THREAD_FARG);
extern gex_Event_t
gasnete_coll_op_generic_init_with_scratch(gasnete_coll_team_t team, int flags,
                                          gasnete_coll_generic_data_t *data,
                                          gasnete_coll_poll_fn poll_fn,
                                          uint32_t sequence,
                                          gasnete_coll_scratch_req_t *scratch_req,
                                          int num_params,
                                          uint32_t *param_list,
                                          gasnete_coll_local_tree_geom_t *tree_geom
                                          GASNETI_THREAD_FARG);

extern int gasnete_coll_generic_syncnb(gasnete_coll_generic_data_t *data);

// TODO-EX: to be replaced with multi-EP equivalents:
#if GASNET_PAR && GASNET_DEBUG
  extern void gasnete_coll_threads_lock(gasnete_coll_team_t team, int flags GASNETI_THREAD_FARG);
  extern void gasnete_coll_threads_unlock(gasnete_coll_team_t team GASNETI_THREAD_FARG);
#else
  #define gasnete_coll_threads_lock(team, flags)  do { } while (0)
  #define gasnete_coll_threads_unlock(thrarg)     do { } while (0)
#endif

// TODO-EX: deprecate and remove:
#define gasnete_coll_threads_get_handle(thrarg) (gasneti_unreachable(), GEX_EVENT_INVALID)
#define gasnete_coll_generic_all_threads(data)  (1)


GASNETI_INLINE(gasnete_coll_generic_insync)
int gasnete_coll_generic_insync(gasnete_coll_team_t team, gasnete_coll_generic_data_t *data) {
  gasneti_assert(data != NULL);
  return (!(data->options & GASNETE_COLL_GENERIC_OPT_INSYNC) ||
	  (gasnete_coll_consensus_try(team, data->in_barrier) == GASNET_OK));
}

GASNETI_INLINE(gasnete_coll_generic_outsync)
int gasnete_coll_generic_outsync(gasnete_coll_team_t team, gasnete_coll_generic_data_t *data) {
  gasneti_assert(data != NULL);
  return (!(data->options & GASNETE_COLL_GENERIC_OPT_OUTSYNC) ||
	  (gasnete_coll_consensus_try(team, data->out_barrier) == GASNET_OK));
}

/* Optional UP half-barrier over the collective tree.
 * No memory fences. */
GASNETI_INLINE(gasnete_coll_generic_upsync)
int gasnete_coll_generic_upsync(gasnete_coll_op_t *op, gex_Rank_t rootnode,
                                    const int counter, const int count) {
  gasnete_coll_generic_data_t * const data = op->data;
  if (gasneti_weakatomic_read(&data->p2p->counter[counter], 0) == count) {
    if (op->team->myrank != rootnode) {
      gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(data->tree_geom)),0);
    }
    return 1;
  }
  return 0;
}

/* Optional UP half-barrier over the collective tree.
 * Root node will rmb() and non-root will wmb() to ensure that writes
 * to root's memory will be read by root.  This is appropriate to the
 * needs of a "push" based broadcast or scatter. */
GASNETI_INLINE(gasnete_coll_generic_upsync_acq)
int gasnete_coll_generic_upsync_acq(gasnete_coll_op_t *op, gex_Rank_t rootnode,
                                    const int counter, const int count) {
  gasnete_coll_generic_data_t * const data = op->data;
  if (gasneti_weakatomic_read(&data->p2p->counter[counter], 0) == count) {
    if (op->team->myrank != rootnode) {
      gasneti_local_wmb();
      gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, GASNETE_COLL_TREE_GEOM_PARENT(data->tree_geom)),0);
    } else {
      gasneti_local_rmb();
    }
    return 1;
  }
  return 0;
}

extern int gasnete_coll_generic_coll_sync(gex_Event_t *p, size_t count GASNETI_THREAD_FARG);

// Legacy "generic" layer

extern gex_Event_t
gasnete_coll_generic_broadcast_nb(gasnet_team_handle_t team,
                                  void *dst,
                                  gasnet_image_t srcimage, void *src,
                                  size_t nbytes, int flags,
                                  gasnete_coll_poll_fn poll_fn, int options,
                                  gasnete_coll_local_tree_geom_t *geom_info, uint32_t sequence,
                                  int num_params, uint32_t *param_list
                                  GASNETI_THREAD_FARG);

extern gex_Event_t
gasnete_coll_generic_scatter_nb(gasnet_team_handle_t team,
                                void *dst,
                                gasnet_image_t srcimage, void *src,
                                size_t nbytes, size_t dist, int flags,
                                gasnete_coll_poll_fn poll_fn, int options,
                                gasnete_coll_local_tree_geom_t *geom_info, uint32_t sequence,
                                int num_params, uint32_t *param_list
                                GASNETI_THREAD_FARG);

extern gex_Event_t
gasnete_coll_generic_gather_nb(gasnet_team_handle_t team,
                               gasnet_image_t dstimage, void *dst,
                               void *src,
                               size_t nbytes, size_t dist, int flags,
                               gasnete_coll_poll_fn poll_fn, int options,
                               gasnete_coll_local_tree_geom_t *geom_info, uint32_t sequence,
                               int num_params, uint32_t *param_list
                               GASNETI_THREAD_FARG);

extern gex_Event_t
gasnete_coll_generic_gather_all_nb(gasnet_team_handle_t team,
                                   void *dst, void *src,
                                   size_t nbytes, int flags,
                                   gasnete_coll_poll_fn poll_fn, int options,
                                   void *private_data, uint32_t sequence,
                                   int num_params, uint32_t *param_list
                                   GASNETI_THREAD_FARG);

extern gex_Event_t
gasnete_coll_generic_exchange_nb(gasnet_team_handle_t team,
                                 void *dst, void *src,
                                 size_t nbytes, int flags,
                                 gasnete_coll_poll_fn poll_fn, int options,
                                 void *private_data, gasnete_coll_dissem_info_t *dissem, uint32_t sequence,
                                 int num_params, uint32_t *param_list
                                 GASNETI_THREAD_FARG);

// Legacy "nb_default" layer

extern gex_Event_t
gasnete_coll_broadcast_nb_default(gasnet_team_handle_t team,
                                  void *dst,
                                  gasnet_image_t srcimage, void *src,
                                  size_t nbytes, int flags, uint32_t sequence
                                  GASNETI_THREAD_FARG);
extern gex_Event_t
gasnete_coll_scatter_nb_default(gasnet_team_handle_t team,
                                void *dst,
                                gasnet_image_t srcimage, void *src,
                                size_t nbytes, int flags, uint32_t sequence
                                GASNETI_THREAD_FARG);
extern gex_Event_t
gasnete_coll_gather_nb_default(gasnet_team_handle_t team,
                               gasnet_image_t dstimage, void *dst,
                               void *src,
                               size_t nbytes, int flags, uint32_t sequence
                               GASNETI_THREAD_FARG);
extern gex_Event_t
gasnete_coll_gather_all_nb_default(gasnet_team_handle_t team,
                                   void *dst, void *src,
                                   size_t nbytes, int flags, uint32_t sequence
                                   GASNETI_THREAD_FARG);
extern gex_Event_t
gasnete_coll_exchange_nb_default(gasnet_team_handle_t team,
                                 void *dst, void *src,
                                 size_t nbytes, int flags, uint32_t sequence
                                 GASNETI_THREAD_FARG);

// GEX "generic" layer

extern gex_Event_t
gasnete_tm_generic_reduce_nb(gex_TM_t tm, gex_Rank_t root, void *dst, const void *src,
                             gex_DT_t dt, size_t dt_sz, size_t dt_cnt,
                             gex_OP_t opcode, gex_Coll_ReduceFn_t fnptr, void *cdata,
                             int coll_flags, gasnete_coll_poll_fn poll_fn, int options,
                             gasnete_coll_local_tree_geom_t *geom_info, uint32_t sequence,
                             int num_params, uint32_t *param_list,
                             gasnete_coll_scratch_req_t *scratch_req
                             GASNETI_THREAD_FARG);

extern gex_Event_t
gasnete_tm_generic_reduce_all_nb(
                             gex_TM_t tm, void *dst, const void *src,
                             gex_DT_t dt, size_t dt_sz, size_t dt_cnt,
                             gex_OP_t opcode, gex_Coll_ReduceFn_t fnptr, void *cdata,
                             int coll_flags, gasnete_coll_poll_fn poll_fn, int options,
                             gasnete_coll_local_tree_geom_t *geom_info, uint32_t sequence,
                             int num_params, uint32_t *param_list,
                             gasnete_coll_scratch_req_t *scratch_req
                             GASNETI_THREAD_FARG);

/*---------------------------------------------------------------------------------*
* Start of protypes for reference implementations
*---------------------------------------------------------------------------------*/

#define GASNETE_COLL_DECLARE_BCAST_ALG(FUNC_EXT)\
extern gex_Event_t \
gasnete_coll_bcast_##FUNC_EXT(gasnet_team_handle_t team,\
                       void * dst,\
                       gasnet_image_t srcimage, void *src,\
                       size_t nbytes, int flags,\
                       gasnete_coll_implementation_t coll_params,\
                       uint32_t sequence\
                       GASNETI_THREAD_FARG)

GASNETE_COLL_DECLARE_BCAST_ALG(Eager);
GASNETE_COLL_DECLARE_BCAST_ALG(RVGet);
GASNETE_COLL_DECLARE_BCAST_ALG(TreeRVGet);
GASNETE_COLL_DECLARE_BCAST_ALG(RVous);
GASNETE_COLL_DECLARE_BCAST_ALG(TreePutScratch);
GASNETE_COLL_DECLARE_BCAST_ALG(TreePutSeg);
GASNETE_COLL_DECLARE_BCAST_ALG(ScatterAllgather);
GASNETE_COLL_DECLARE_BCAST_ALG(TreeEager);

/*---------------------------------------------------------------------------------*/

#define GASNETE_COLL_DECLARE_SCATTER_ALG(FUNC_EXT)\
extern gex_Event_t \
gasnete_coll_scat_##FUNC_EXT(gasnet_team_handle_t team,\
                             void *dst,\
                             gasnet_image_t srcimage, void *src,\
                             size_t nbytes, size_t dist, int flags,\
                             gasnete_coll_implementation_t coll_params,\
                             uint32_t sequence\
                             GASNETI_THREAD_FARG)

GASNETE_COLL_DECLARE_SCATTER_ALG(TreePut);
GASNETE_COLL_DECLARE_SCATTER_ALG(TreePutNoCopy);
GASNETE_COLL_DECLARE_SCATTER_ALG(TreePutSeg);
GASNETE_COLL_DECLARE_SCATTER_ALG(TreeEager);
GASNETE_COLL_DECLARE_SCATTER_ALG(Eager);
GASNETE_COLL_DECLARE_SCATTER_ALG(RVGet);
GASNETE_COLL_DECLARE_SCATTER_ALG(RVous);

/*---------------------------------------------------------------------------------*/

#define GASNETE_COLL_DECLARE_GATHER_ALG(FUNC_EXT)\
extern gex_Event_t \
gasnete_coll_gath_##FUNC_EXT(gasnet_team_handle_t team,\
                             gasnet_image_t dstimage, void *dst,\
                             void *src,\
                             size_t nbytes, size_t dist, int flags, \
                             gasnete_coll_implementation_t coll_params,\
                             uint32_t sequence\
                             GASNETI_THREAD_FARG)

GASNETE_COLL_DECLARE_GATHER_ALG(TreePut);
GASNETE_COLL_DECLARE_GATHER_ALG(TreePutNoCopy);
GASNETE_COLL_DECLARE_GATHER_ALG(TreePutSeg);
GASNETE_COLL_DECLARE_GATHER_ALG(TreeEager);
GASNETE_COLL_DECLARE_GATHER_ALG(Eager);
GASNETE_COLL_DECLARE_GATHER_ALG(RVPut);
GASNETE_COLL_DECLARE_GATHER_ALG(RVous);

/*---------------------------------------------------------------------------------*/

#define GASNETE_COLL_DECLARE_GATHER_ALL_ALG(FUNC_EXT)\
  extern gex_Event_t                                           \
  gasnete_coll_gall_##FUNC_EXT(gasnet_team_handle_t team,               \
                               void *dst, void *src,                    \
                               size_t nbytes, int flags,                \
                               gasnete_coll_implementation_t coll_params, \
                               uint32_t sequence                        \
                               GASNETI_THREAD_FARG)

GASNETE_COLL_DECLARE_GATHER_ALL_ALG(Gath);
GASNETE_COLL_DECLARE_GATHER_ALL_ALG(EagerDissem);
GASNETE_COLL_DECLARE_GATHER_ALL_ALG(Dissem);
GASNETE_COLL_DECLARE_GATHER_ALL_ALG(FlatEagerPut);

/*---------------------------------------------------------------------------------*/

#define GASNETE_COLL_DECLARE_EXCHANGE_ALG(FUNC_EXT)\
extern gex_Event_t \
gasnete_coll_exchg_##FUNC_EXT(gasnet_team_handle_t team,\
                              void *dst, void *src,\
                              size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence\
                              GASNETI_THREAD_FARG)

GASNETE_COLL_DECLARE_EXCHANGE_ALG(Dissem2);
GASNETE_COLL_DECLARE_EXCHANGE_ALG(Dissem3);
GASNETE_COLL_DECLARE_EXCHANGE_ALG(Dissem4);
GASNETE_COLL_DECLARE_EXCHANGE_ALG(Dissem8);
GASNETE_COLL_DECLARE_EXCHANGE_ALG(FlatScratch);
GASNETE_COLL_DECLARE_EXCHANGE_ALG(Gath);
GASNETE_COLL_DECLARE_EXCHANGE_ALG(RVPut);

/*---------------------------------------------------------------------------------*/

#define GASNETE_TM_REDUCE_ARGS \
                             gex_TM_t tm, gex_Rank_t root,\
                             void *dst, const void *src,\
                             gex_DT_t dt, size_t dt_sz, size_t dt_cnt,\
                             gex_OP_t op, gex_Coll_ReduceFn_t op_fnptr, void *op_cdata,\
                             int coll_flags, \
                             gasnete_coll_implementation_t coll_params,\
                             uint32_t sequence\
                             GASNETI_THREAD_FARG
#define GASNETE_TM_DECLARE_REDUCE_ALG(FUNC_EXT) \
    extern gex_Event_t gasnete_tm_reduce_##FUNC_EXT(GASNETE_TM_REDUCE_ARGS)
typedef gex_Event_t (*gasnete_tm_reduce_fn_ptr_t)(GASNETE_TM_REDUCE_ARGS);

GASNETE_TM_DECLARE_REDUCE_ALG(BinomialEager);
GASNETE_TM_DECLARE_REDUCE_ALG(TreePut);
GASNETE_TM_DECLARE_REDUCE_ALG(TreePutSeg);

/*---------------------------------------------------------------------------------*/

#define GASNETE_TM_REDUCE_ALL_ARGS \
                             gex_TM_t tm, void *dst, const void *src,\
                             gex_DT_t dt, size_t dt_sz, size_t dt_cnt,\
                             gex_OP_t op, gex_Coll_ReduceFn_t op_fnptr, void *op_cdata,\
                             int coll_flags, \
                             gasnete_coll_implementation_t coll_params,\
                             uint32_t sequence\
                             GASNETI_THREAD_FARG
#define GASNETE_TM_DECLARE_REDUCE_ALL_ALG(FUNC_EXT) \
    extern gex_Event_t gasnete_tm_reduce_all_##FUNC_EXT(GASNETE_TM_REDUCE_ALL_ARGS)
typedef gex_Event_t (*gasnete_tm_reduce_all_fn_ptr_t)(GASNETE_TM_REDUCE_ALL_ARGS);

GASNETE_TM_DECLARE_REDUCE_ALL_ALG(Bcast);

/*---------------------------------------------------------------------------------*/
// Reduction operators
// TODO-EX: this is not intended to be the final implemenation (or naming)

#define GASNETE_TM_REDUCE_FOREACH_DT(FN) \
  FN(I32) FN(U32) FN(I64) FN(U64) FN(FLT) FN(DBL)
#define GASNETE_SHRINKRAY_DECL(DT) \
    extern void gasnete_shrinkray_gex_dt_##DT (        \
            const void * arg1,         \
            void *       arg2_and_out, \
            size_t       count ,       \
            const void * cdata);
GASNETE_TM_REDUCE_FOREACH_DT(GASNETE_SHRINKRAY_DECL)

/*---------------------------------------------------------------------------------*/
// Bit-twiddling helpers
// TODO: some may still be subject to additional optimization
//

// Count consecutive zero bits from the right (least-significant) end */
// TODO: default (no builtins) version can be improved upon
GASNETI_INLINE(gasnete_coll_ctz_u32) GASNETI_CONST
unsigned int gasnete_coll_ctz_u32(uint32_t v) {
#if GASNETI_HAVE_CC_BUILTIN_CTZ
  return v ? __builtin_ctz(v) : 32;
#elif HAVE_FFS
  return v ? (ffs(v)-1) : 32;
#else
  unsigned int c = 32;
  if (v) {
    for (c=0; !(v&1); ++c) v >>= 1;
  }
  return c;
#endif
}
GASNETI_CONSTP(gasnete_coll_ctz_u32)

//
// Family of functions for (fast) floor(log_2(v)) OR -1 for v=0
//

// The compiler-independent (table-lookup) portions of the following
// function to find the fast log2 of integers are adapted from:
// http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogLookup
// (accessed July 10, 2008)

static const int8_t LogTable256[] = {
     -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
      4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
      5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
      5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

GASNETI_INLINE(gasnete_coll_log2_u32) GASNETI_CONST
int gasnete_coll_log2_u32(uint32_t v) {
#if GASNETI_HAVE_CC_BUILTIN_CLZ && (SIZEOF_INT == 4)
  return v ? (31 - __builtin_clz(v)) : -1;
#elif GASNETI_HAVE_CC_BUILTIN_CLZL && (SIZEOF_LONG == 4)
  return v ? (31 - __builtin_clzl(v)) : -1;
#elif GASNETI_HAVE_CC_BUILTIN_CLZLL && (SIZEOF_LONG_LONG == 4)
  return v ? (31 - __builtin_clzll(v)) : -1;
#else
  int r;
  uint32_t t, tt;

  if ((tt = v >> 16)) {
    r = (t = tt >> 8) ? 24 + LogTable256[t] : 16 + LogTable256[tt];
  } else {
    r = (t =  v >> 8) ?  8 + LogTable256[t] :      LogTable256[v];
  }

  return r;
#endif
}
GASNETI_CONSTP(gasnete_coll_log2_u32)

GASNETI_INLINE(gasnete_coll_log2_u64) GASNETI_CONST
int gasnete_coll_log2_u64(uint64_t v) {
#if GASNETI_HAVE_CC_BUILTIN_CLZ && (SIZEOF_INT == 8)
  return v ? (63 - __builtin_clz(v)) : -1;
#elif GASNETI_HAVE_CC_BUILTIN_CLZL && (SIZEOF_LONG == 8)
  return v ? (63 - __builtin_clzl(v)) : -1;
#elif GASNETI_HAVE_CC_BUILTIN_CLZLL && (SIZEOF_LONG_LONG == 8)
  return v ? (63 - __builtin_clzll(v)) : -1;
#else
  int r;
  uint64_t t, tt;

  if ((tt = v >> 48)) {
    r = (t = tt >> 8) ? 56 + LogTable256[t] : 48 + LogTable256[tt];
  } else if ((tt = v>>32)) {
    r = (t = tt >> 8) ? 40 + LogTable256[t] : 32 + LogTable256[tt];
  } else if ((tt = v >> 16)) {
    r = (t = tt >> 8) ? 24 + LogTable256[t] : 16 + LogTable256[tt];
  } else {
    r = (t =  v >> 8) ?  8 + LogTable256[t] :      LogTable256[v];
  }

  return r;
#endif
}
GASNETI_CONSTP(gasnete_coll_log2_u64)

// Aliases
#define gasnete_coll_log2_rank(v) gasnete_coll_log2_u32(v)
#if (SIZEOF_SIZE_T == 4)
  #define gasnete_coll_log2_sz(v) gasnete_coll_log2_u32(v)
#elif (SIZEOF_SIZE_T == 8)
  #define gasnete_coll_log2_sz(v) gasnete_coll_log2_u64(v)
#else
  #error Unknown SIZEOF_SIZE_T
#endif

/*---------------------------------------------------------------------------------*/
// Binomial geometry helpers
// In these 'rel_rank' is '(self - root) % nranks'

// Relative rank in binomial tree rooted at 'root'
GASNETI_INLINE(gasnete_tm_binom_rel_root) GASNETI_PURE
gex_Rank_t gasnete_tm_binom_rel_root(gex_TM_t const tm, const gex_Rank_t root) {
  const gex_Rank_t self = gex_TM_QueryRank(tm);
  const gex_Rank_t size = gex_TM_QuerySize(tm);
  return (self >= root) ? (self - root) : (self + size - root);
}
GASNETI_PUREP(gasnete_tm_binom_rel_root)

// Size of local binomial subtree, including self
// TODO-EX: broken for teams of size 2^31 or larger
GASNETI_INLINE(gasnete_tm_binom_subtree_size) GASNETI_PURE
gex_Rank_t gasnete_tm_binom_subtree_size(gex_TM_t const tm, const gex_Rank_t rel_rank) {
  const gex_Rank_t size = gex_TM_QuerySize(tm);
  gasneti_assert(size < 0x80000000);
  const gex_Rank_t remain = size - rel_rank;
  const gex_Rank_t fullsize = (rel_rank & (-rel_rank));
  return (!fullsize || (fullsize > remain)) ? remain : fullsize;
}
GASNETI_PUREP(gasnete_tm_binom_subtree_size)

// Count of direct children in binomial subtree
GASNETI_INLINE(gasnete_tm_binom_children) GASNETI_PURE
gex_Rank_t gasnete_tm_binom_children(gex_TM_t const tm, const gex_Rank_t rel_rank) {
  return 1 + gasnete_coll_log2_rank(gasnete_tm_binom_subtree_size(tm, rel_rank) - 1);
}
GASNETI_PUREP(gasnete_tm_binom_children)

// Rank (not relative) of parent
// TODO-EX: broken for teams of size 2^31 or larger
GASNETI_INLINE(gasnete_tm_binom_parent) GASNETI_PURE
gex_Rank_t gasnete_tm_binom_parent(gex_TM_t const tm, const gex_Rank_t rel_rank) {
  const gex_Rank_t self = gex_TM_QueryRank(tm);
  const gex_Rank_t size = gex_TM_QuerySize(tm);
  gasneti_assert(size < 0x80000000);
  const gex_Rank_t fullsize = (rel_rank & (-rel_rank));
  return (self >= fullsize) ? (self - fullsize) : (self + size - fullsize);
}
GASNETI_PUREP(gasnete_tm_binom_parent)

// Rank among siblings (e.g. 0 for first child, 1 for second, etc.)
GASNETI_INLINE(gasnete_tm_binom_age) GASNETI_PURE
gex_Rank_t gasnete_tm_binom_age(gex_TM_t const tm, const gex_Rank_t rel_rank) {
  return gasnete_coll_ctz_u32(rel_rank);
}
GASNETI_PUREP(gasnete_tm_binom_age)

/*---------------------------------------------------------------------------------*/
/* Conduit specific extension hooks: */
/* These may be unused, but there is no harm in prototyping them. */

extern void gasnete_coll_init_conduit(void);
extern void gasnete_coll_team_init_conduit(gasnet_team_handle_t team);
extern void gasnete_coll_team_fini_conduit(gasnet_team_handle_t team);

/*---------------------------------------------------------------------------------*/
// Helpers for uint32_t sequence numbers (which, by their nature, may wrap-around).

// Assert that absolute difference is less than 2^30 (give or take 1)
#if GASNET_DEBUG
  GASNETI_INLINE(gasnete_coll_seq32_safe)
  void gasnete_coll_seq32_safe(uint32_t _u, uint32_t _v) {
    if (_u - _v + 0x40000000u >= 0x80000000u) {
      gasneti_fatalerror("Absolute difference between unsigned 32-bit sequence "
                         "numbers %u and %u (0x%x and 0x%x) is larger than 2^30",
                         (unsigned int)_u, (unsigned int)_v,
                         (unsigned int)_u, (unsigned int)_v);
    }
  }
  #define GASNETE_COLL_SEQ32_SAFE(u,v) gasnete_coll_seq32_safe((u),(v))
#else
  #define GASNETE_COLL_SEQ32_SAFE(u,v) ((void)0)
#endif

// GASNETE_COLL_SEQ32_{LT,LE,GT,GE}(u,v)  true (non-zero) if "u {<,<=,>,>=} v"
//   In all cases the comparison takes into consideration the wrap around.
//   Also asserts that the difference remains less than half-way to the
//   point at which wrap-around would cause ambiguity.
GASNETI_INLINE(gasnete_coll_seq32_ge)
int gasnete_coll_seq32_ge(uint32_t _u, uint32_t _v) {
  GASNETE_COLL_SEQ32_SAFE(_u,_v);
  return _u - _v < 0x80000000u;
}
#define GASNETE_COLL_SEQ32_GE(u,v) gasnete_coll_seq32_ge((u),(v))
#define GASNETE_COLL_SEQ32_GT(u,v) gasnete_coll_seq32_ge((u),(uint32_t)(v)+1)
#define GASNETE_COLL_SEQ32_LE(u,v) gasnete_coll_seq32_ge((v),(u))
#define GASNETE_COLL_SEQ32_LT(u,v) gasnete_coll_seq32_ge((v),(uint32_t)(u)+1)

/*---------------------------------------------------------------------------------*/

#endif
