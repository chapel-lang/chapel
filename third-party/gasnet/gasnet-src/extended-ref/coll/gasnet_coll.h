/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_coll.h $
 * Description: GASNet Extended API Collective declarations
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_COLL_H
#define _GASNET_COLL_H

#include <gasnet.h>

GASNETI_BEGIN_EXTERNC
GASNETI_BEGIN_NOWARN
#define GASNETI_COLL_FN_HEADER(FNNAME) extern
/*---------------------------------------------------------------------------------*/
/* Flag values: */

/* Sync flags - NO DEFAULT */
#define GASNET_COLL_IN_NOSYNC	(1<<0)
#define GASNET_COLL_IN_MYSYNC	(1<<1)
#define GASNET_COLL_IN_ALLSYNC	(1<<2)
#define GASNET_COLL_OUT_NOSYNC	(1<<3)
#define GASNET_COLL_OUT_MYSYNC	(1<<4)
#define GASNET_COLL_OUT_ALLSYNC	(1<<5)

#define GASNET_COLL_SYNC_FLAG_MASK (0x3F)

#define GASNET_COLL_SINGLE	(1<<6)
#define GASNET_COLL_LOCAL	(1<<7)

#define GASNET_COLL_AGGREGATE	(1<<8)
#define GASNET_COLL_FIXED_THREADS_PER_NODE (1<<9)

#define GASNET_COLL_DST_IN_SEGMENT	(1<<10)
#define GASNET_COLL_SRC_IN_SEGMENT	(1<<11)

/*sane default reduction operators*/
#define GASNET_COLL_REDUCE_OP_SUM (1<<12)
#define GASNET_COLL_REDUCE_OP_MAX (1<<13)
#define GASNET_COLL_REDUCE_OP_MIN (1<<14)
#define GASNET_COLL_REDUCE_OP_CUSTOM (1<<15)

/* Scan (prefix reduction) flags - NO DEFAULT */
#define GASNET_COLL_INCLUSIVE_SCAN	(1<<16)
#define GASNET_COLL_EXCLUSIVE_SCAN	(1<<17)


#define GASNET_COLL_DISABLE_AUTOTUNE (1<<18)
#define GASNET_COLL_NO_IMAGES (1<<19)

/* (prefix-)reduction function flags */
#define GASNET_COLL_AMSAFE	(1<<0)
#define GASNET_COLL_NONCOMM	(1<<1)


#ifndef GASNETE_COLL_IMAGE_OVERRIDE
  /* gasnet_image_t must be large enough to index all threads that participate
   * in collectives.  A conduit may override this if a smaller type will suffice.
   * However, types larger than 32-bits won't pass as AM handler args.  So, for
   * a larger type, many default things will require overrides.
   */
  typedef uint32_t gasnet_image_t;
#endif

/*---------------------------------------------------------------------------------*/

#ifndef GASNETE_COLL_HANDLE_OVERRIDE
  /* Handle type for collective ops: */
  #if GASNET_PAR
    struct gasnet_coll_handle_t_;
    typedef struct gasnet_coll_handle_t_ *gasnet_coll_handle_t;
  #else
    typedef volatile uintptr_t *gasnet_coll_handle_t;
  #endif
  #define GASNET_COLL_INVALID_HANDLE ((gasnet_coll_handle_t)NULL)
#endif




/* Functions, types, etc for computational collectives */

#ifndef GASNET_COLL_FN_HANDLE_T
  typedef gasnet_handlerarg_t gasnet_coll_fn_handle_t;
#endif

typedef void (*gasnet_coll_reduce_fn_t)(
	void *_results, size_t _result_count,
	const void *_left_operands, size_t _left_count,
	const void *_right_operands,
	size_t _elem_size, int _flags, int _arg);
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

/* Handle type for collective teams: */
#ifndef GASNETE_COLL_TEAMS_OVERRIDE
struct gasnete_coll_team_t_;
typedef struct gasnete_coll_team_t_ *gasnete_coll_team_t;
typedef gasnete_coll_team_t gasnet_team_handle_t;
/*change this so even the TEAM_ALL has a default team allocated rather than NULL*/
#endif

#ifndef GASNET_TEAM_ALL
extern gasnet_team_handle_t gasnete_coll_team_all;
#define GASNET_TEAM_ALL gasnete_coll_team_all
#endif

extern gasnet_node_t gasnete_coll_team_rank2node(gasnete_coll_team_t _team, int _rank);
extern gasnet_node_t gasnete_coll_team_node2rank(gasnete_coll_team_t _team, gasnet_node_t _node);
extern gasnet_node_t gasnete_coll_team_size(gasnete_coll_team_t _team);

#define gasnet_coll_team_rank2node(TEAM, RANK) gasnete_coll_team_rank2node(TEAM, RANK)
#define gasnet_coll_team_node2rank(TEAM, NODE) gasnete_coll_team_node2rank(TEAM, NODE)
#define gasnet_coll_team_size(TEAM) gasnete_coll_team_size(TEAM)

extern gasnet_team_handle_t gasnete_coll_team_split(gasnete_coll_team_t _parent_team, gasnet_node_t _color,
						    gasnet_node_t _relrank, void *_clientdata GASNETI_THREAD_FARG);

GASNETI_INLINE(_gasnet_coll_team_split)
     gasnet_team_handle_t _gasnet_coll_team_split(gasnet_team_handle_t _parent_team, gasnet_node_t _color, gasnet_node_t _relrank, 
						  void *_clientdata GASNETI_THREAD_FARG) {
  return gasnete_coll_team_split(_parent_team, _color, _relrank, _clientdata GASNETI_THREAD_PASS);
  
}

#define gasnet_coll_team_split(parent_team, color, relrank, clientdata)  \
  _gasnet_coll_team_split(parent_team, color, relrank, clientdata GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/


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
 *  fn_count:   The number of entries in 'fn_tbl'.  Must agree across
 *              all nodes or the behavior is undefined.
 *  init_flags: Presently unused.  Must be 0.
 */

#ifndef gasnet_coll_init
  GASNETI_COLL_FN_HEADER(gasnete_coll_init) 
  void gasnete_coll_init(const gasnet_image_t _images[], gasnet_image_t _my_image,
		  		gasnet_coll_fn_entry_t _fn_tbl[], size_t _fn_count,
		  		int _init_flags GASNETI_THREAD_FARG);
  #define gasnet_coll_init(im,mi,fn,fc,fl) \
		gasnete_coll_init(im,mi,fn,fc,fl GASNETI_THREAD_GET)
#endif

/*---------------------------------------------------------------------------------*/


/* STUFF FOR COLLECTIVE AUTOTUNING*/
/*---------------------------------------------------------------------------------*
 * Prototypes for external interface to try different collective trees (only works for GASNet Team All)
 * Note that the preffered way for changing these values is in the environment rather than these functions themselves
 *---------------------------------------------------------------------------------*/

typedef enum {GASNET_COLL_BROADCAST_OP=0, 
  GASNET_COLL_BROADCASTM_OP, 
  GASNET_COLL_SCATTER_OP, 
  GASNET_COLL_SCATTERM_OP, 
  GASNET_COLL_GATHER_OP, 
  GASNET_COLL_GATHERM_OP, 
  GASNET_COLL_GATHER_ALL_OP,
  GASNET_COLL_GATHER_ALLM_OP,
  GASNET_COLL_EXCHANGE_OP,
  GASNET_COLL_EXCHANGEM_OP, 
  GASNET_COLL_REDUCE_OP,
  GASNET_COLL_REDUCEM_OP,
  GASNET_COLL_NUM_COLL_OPTYPES
} gasnet_coll_optype_t;

typedef enum {GASNET_COLL_PIPE_SEG_SIZE, GASNET_COLL_DISSEM_RADIX, GASNET_COLL_TREE_TYPE,
              /*check to see if hte conduit has added any new tuning parameters to this list*/
#ifdef GASNETE_COLL_CONDUIT_TUNING_PARAMETERS
              GASNETE_COLL_CONDUIT_TUNING_PARAMETERS ,
#endif 
              GASNET_COLL_NUM_PARAM_TYPES} gasnet_coll_tuning_param_type_t ;



typedef void (*gasnet_coll_overlap_sample_work_t)(void *_arg);


void gasnete_coll_loadTuningState(char *_filename, gasnete_coll_team_t _team GASNETI_THREAD_FARG);
#define gasnet_coll_loadTuningState(FILENAME, TEAM) gasnete_coll_loadTuningState(FILENAME, TEAM GASNETI_THREAD_GET)


void gasnete_coll_dumpTuningState(char *_filename, gasnete_coll_team_t _team GASNETI_THREAD_FARG);
#define gasnet_coll_dumpTuningState(FILENAME, TEAM) gasnete_coll_dumpTuningState(FILENAME, TEAM GASNETI_THREAD_GET)

void gasnete_coll_dumpProfile(char *_filename, gasnete_coll_team_t _team GASNETI_THREAD_FARG);
#define gasnet_coll_dumpProfile(FILENAME, TEAM) gasnete_coll_dumpProfile(FILENAME, TEAM GASNETI_THREAD_GET)

#define gasnet_coll_tune_generic_op(team, op, coll_args, flags, fnptr, work_arg, best_algidx, num_params, best_param, best_tree) \
gasnete_coll_tune_generic_op(team, op, coll_args, flags, fnptr, work_arg, best_algidx, num_params, best_param,  best_tree GASNETI_THREAD_GET)

#ifdef _GASNET_COLL_INTERNAL_H
// The fields of this struct violate public header naming conventions,
// but it is only used internally by the autotuner so hide it from clients
typedef struct gasnet_coll_args_t_ {
  uint8_t **dst; 
  uint8_t **src; 
  gasnet_image_t rootimg; 
  size_t src_blksz;
  size_t src_offset;
  size_t elem_size; 
  /*elem count will be nbytes / elem_size*/
  size_t nbytes;
  size_t dist;
  gasnet_coll_fn_handle_t func; 
  int func_arg;
} gasnet_coll_args_t;
#define GASNET_COLL_ARGS_INITIALIZER { NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0 }

void gasnete_coll_tune_generic_op(gasnet_team_handle_t _team, gasnet_coll_optype_t _op, 
                                  gasnet_coll_args_t _coll_args, int _flags,
                                  gasnet_coll_overlap_sample_work_t _fnptr, void *_sample_work_arg,
                                  /*returned by the algorithm*/
                                  uint32_t *_best_algidx, uint32_t *_num_params, uint32_t **_best_param, char **_best_tree GASNETI_THREAD_FARG);
#endif // _GASNET_COLL_INTERNAL_H

extern int gasnet_coll_get_num_tree_classes(gasnet_team_handle_t _team, gasnet_coll_optype_t _optype);
extern void gasnet_coll_set_tree_kind(gasnet_team_handle_t _team, int _tree_type, int _fanout, gasnet_coll_optype_t _optype); 
extern void gasnet_coll_set_dissem_limit(gasnet_team_handle_t _team, size_t _dissemlimit, gasnet_coll_optype_t _optype); 

/*---------------------------------------------------------------------------------*/
/* Collectives tracing */
#if GASNETI_STATS_OR_TRACE
  /* In what follows, "????"")" protects us from evil trigraphs */
  #if 0
    /* XXX Not yet implemented */
    extern char * gasnete_coll_format_addrlist(const void *addrlist[], int flags);
  #else
    #define gasnete_coll_format_addrlist(list,flags) gasneti_extern_strdup("[LIST]")
  #endif 
  #define GASNETI_TRACE_COLL_BROADCAST(name,team,dst,root,src,nbytes,flags) do {                           \
    GASNETI_TRACE_EVENT_VAL(W,name,nbytes);                                                                \
    if (GASNETI_TRACE_ENABLED(D)) {                                                                        \
      if ((flags & GASNET_COLL_SINGLE) || (root == gasnet_mynode())) {                                     \
        GASNETI_TRACE_PRINTF(D,(#name ": " GASNETI_LADDRFMT " <- " GASNETI_RADDRFMT                        \
				" (nbytes=%" PRIuSZ " team=%p flags=0x%x)\n",                              \
			        GASNETI_LADDRSTR(dst), GASNETI_RADDRSTR(root,src),                         \
      			        (size_t)nbytes, (void *)team, flags));                                     \
      } else {                                                                                             \
        GASNETI_TRACE_PRINTF(D,(#name ": " GASNETI_LADDRFMT " <- (%i,????"")"                              \
				" (nbytes=%" PRIuSZ " team=%p flags=0x%x)\n",                              \
			        GASNETI_LADDRSTR(dst), (int)(root),                                        \
      			        (size_t)nbytes, (void *)team, flags));                                     \
      }                                                                                                    \
    }                                                                                                      \
  } while (0)
  #define GASNETI_TRACE_COLL_BROADCAST_M(name,team,dstlist,root,src,nbytes,flags) do {                     \
    GASNETI_TRACE_EVENT_VAL(W,name,nbytes);                                                                \
    if (GASNETI_TRACE_ENABLED(D)) {                                                                        \
      char *_dstlist = gasnete_coll_format_addrlist(dstlist,flags);                                        \
      if ((flags & GASNET_COLL_SINGLE) || (root == gasnet_mynode())) {                                     \
        GASNETI_TRACE_PRINTF(D,(#name ": %s <- " GASNETI_RADDRFMT                                          \
				" (nbytes=%" PRIuSZ " team=%p flags=0x%x)\n",                              \
			        _dstlist, GASNETI_RADDRSTR(root,src),                                      \
      			        (size_t)nbytes, (void *)team, flags));                                     \
      } else {                                                                                             \
        GASNETI_TRACE_PRINTF(D,(#name ": %s <- (%i,????"")"                                                \
				" (nbytes=%" PRIuSZ " team=%p flags=0x%x)\n",                              \
			        _dstlist, (int)(root),                                                     \
      			        (size_t)nbytes, (void *)team, flags));                                     \
      }                                                                                                    \
      gasneti_extern_free(_dstlist);                                                                       \
    }                                                                                                      \
  } while (0)
  #define GASNETI_TRACE_COLL_SCATTER(name,team,dst,root,src,nbytes,flags) \
	GASNETI_TRACE_COLL_BROADCAST(name,team,dst,root,src,nbytes,flags)
  #define GASNETI_TRACE_COLL_SCATTER_M(name,team,dstlist,root,src,nbytes,flags) \
	GASNETI_TRACE_COLL_BROADCAST_M(name,team,dstlist,root,src,nbytes,flags)
  #define GASNETI_TRACE_COLL_GATHER(name,team,root,dst,src,nbytes,flags) do {                              \
    GASNETI_TRACE_EVENT_VAL(W,name,nbytes);                                                                \
    if (GASNETI_TRACE_ENABLED(D)) {                                                                        \
      if ((flags & GASNET_COLL_SINGLE) || (root == gasnet_mynode())) {                                     \
        GASNETI_TRACE_PRINTF(D,(#name ": " GASNETI_RADDRFMT " <- " GASNETI_LADDRFMT                        \
				" (nbytes=%" PRIuSZ " team=%p flags=0x%x)\n",                              \
			        GASNETI_RADDRSTR(root,dst), GASNETI_LADDRSTR(src),                         \
      			        (size_t)nbytes, (void *)team, flags));                                     \
      } else {                                                                                             \
        GASNETI_TRACE_PRINTF(D,(#name ": (%i,????"") <- " GASNETI_LADDRFMT                                 \
				" (nbytes=%" PRIuSZ " team=%p flags=0x%x)\n",                              \
			        (int)(root), GASNETI_LADDRSTR(src),                                        \
      			        (size_t)nbytes, (void *)team, flags));                                     \
      }                                                                                                    \
    }                                                                                                      \
  } while (0)
  #define GASNETI_TRACE_COLL_GATHER_M(name,team,root,dst,srclist,nbytes,flags) do {                        \
    GASNETI_TRACE_EVENT_VAL(W,name,nbytes);                                                                \
    if (GASNETI_TRACE_ENABLED(D)) {                                                                        \
      char *_srclist = gasnete_coll_format_addrlist(srclist,flags);                                        \
      if ((flags & GASNET_COLL_SINGLE) || (root == gasnet_mynode())) {                                     \
        GASNETI_TRACE_PRINTF(D,(#name ": " GASNETI_RADDRFMT " <- %s"                                       \
				" (nbytes=%" PRIuSZ " team=%p flags=0x%x)\n",                              \
			        GASNETI_RADDRSTR(root,dst), _srclist,                                      \
      			        (size_t)nbytes, (void *)team, flags));                                     \
      } else {                                                                                             \
        GASNETI_TRACE_PRINTF(D,(#name ": (%i,????"") <- %s"                                                \
				" (nbytes=%" PRIuSZ " team=%p flags=0x%x)\n",                              \
			        (int)(root), _srclist,                                                     \
      			        (size_t)nbytes, (void *)team, flags));                                     \
      }                                                                                                    \
      gasneti_extern_free(_srclist);                                                                       \
    }                                                                                                      \
  } while (0)
  #define GASNETI_TRACE_COLL_GATHER_ALL(name,team,dst,src,nbytes,flags) do {                               \
    GASNETI_TRACE_EVENT_VAL(W,name,nbytes);                                                                \
    if (GASNETI_TRACE_ENABLED(D)) {                                                                        \
      GASNETI_TRACE_PRINTF(D,(#name ": " GASNETI_LADDRFMT " <- " GASNETI_LADDRFMT                          \
			      " (nbytes=%" PRIuSZ " team=%p flags=0x%x)\n",                                \
			      GASNETI_LADDRSTR(dst), GASNETI_LADDRSTR(src),                                \
      			      (size_t)nbytes, (void *)team, flags));                                       \
    }                                                                                                      \
  } while (0)
  #define GASNETI_TRACE_COLL_GATHER_ALL_M(name,team,dstlist,srclist,nbytes,flags) do {                     \
    GASNETI_TRACE_EVENT_VAL(W,name,nbytes);                                                                \
    if (GASNETI_TRACE_ENABLED(D)) {                                                                        \
      char *_srclist = gasnete_coll_format_addrlist(srclist,flags);                                        \
      char *_dstlist = gasnete_coll_format_addrlist(dstlist,flags);                                        \
      GASNETI_TRACE_PRINTF(D,(#name ": %s <- %s"                                                           \
			      " (nbytes=%" PRIuSZ " team=%p flags=0x%x)\n",                                \
			      _dstlist, _srclist,                                                          \
      			      (size_t)nbytes, (void *)team, flags));                                       \
      gasneti_extern_free(_dstlist);                                                                       \
      gasneti_extern_free(_srclist);                                                                       \
    }                                                                                                      \
  } while (0)
  #define GASNETI_TRACE_COLL_EXCHANGE(name,team,dst,src,nbytes,flags) \
	GASNETI_TRACE_COLL_GATHER_ALL(name,team,dst,src,nbytes,flags)
  #define GASNETI_TRACE_COLL_EXCHANGE_M(name,team,dstlist,srclist,nbytes,flags) \
	GASNETI_TRACE_COLL_GATHER_ALL_M(name,team,dstlist,srclist,nbytes,flags)
  #define GASNETI_TRACE_COLL_REDUCE(name,team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) do { \
    GASNETI_TRACE_EVENT_VAL(W,name,elem_count);                                                            \
    /* XXX: No detail implemented */                                                                       \
  } while (0)
  #define GASNETI_TRACE_COLL_REDUCE_M(name,team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) do { \
    GASNETI_TRACE_EVENT_VAL(W,name,elem_count);                                                            \
    /* XXX: No detail implemented */                                                                       \
  } while (0)
  #define GASNETI_TRACE_COLL_SCAN(name,team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) do { \
    GASNETI_TRACE_EVENT_VAL(W,name,elem_count);                                                            \
    /* XXX: No detail implemented */                                                                       \
  } while (0)
  #define GASNETI_TRACE_COLL_SCAN_M(name,team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) do { \
    GASNETI_TRACE_EVENT_VAL(W,name,elem_count);                                                            \
    /* XXX: No detail implemented */                                                                       \
  } while (0)
  #define GASNETI_TRACE_COLL_WAITSYNC_BEGIN() \
	        gasneti_tick_t _waitstart = GASNETI_TICKS_NOW_IFENABLED(X)
#else
  #define GASNETI_TRACE_COLL_BROADCAST(name,team,dst,root,src,nbytes,flags)
  #define GASNETI_TRACE_COLL_BROADCAST_M(name,team,dstlist,root,src,nbytes,flags)
  #define GASNETI_TRACE_COLL_SCATTER(name,team,dst,root,src,nbytes,flags)
  #define GASNETI_TRACE_COLL_SCATTER_M(name,team,dstlist,root,src,nbytes,flags)
  #define GASNETI_TRACE_COLL_GATHER(name,team,root,dst,src,nbytes,flags)
  #define GASNETI_TRACE_COLL_GATHER_M(name,team,root,dst,srclist,nbytes,flags)
  #define GASNETI_TRACE_COLL_GATHER_ALL(name,team,dst,src,nbytes,flags)
  #define GASNETI_TRACE_COLL_GATHER_ALL_M(name,team,dstlist,srclist,nbytes,flags)
  #define GASNETI_TRACE_COLL_EXCHANGE(name,team,dst,src,nbytes,flags)
  #define GASNETI_TRACE_COLL_EXCHANGE_M(name,team,dstlist,srclist,nbytes,flags)
  #define GASNETI_TRACE_COLL_REDUCE(name,team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags)
  #define GASNETI_TRACE_COLL_REDUCE_M(name,team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags)
  #define GASNETI_TRACE_COLL_SCAN(name,team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags)
  #define GASNETI_TRACE_COLL_SCAN_M(name,team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags)
  #define GASNETI_TRACE_COLL_WAITSYNC_BEGIN() \
		static char _dummy_COLL_WAITSYNC = (char)sizeof(_dummy_COLL_WAITSYNC)
#endif
#define GASNETI_TRACE_COLL_TRYSYNC(name,success) \
	GASNETI_TRACE_EVENT_VAL(X,name,((success) == GASNET_OK?1:0))
#define GASNETI_TRACE_COLL_WAITSYNC_END(name) \
	GASNETI_TRACE_EVENT_TIME(X,name,GASNETI_TICKS_NOW_IFENABLED(X) - _waitstart)

/*---------------------------------------------------------------------------------*/
/* Include all the code for wait/try sync so that we can attempt to inline them into the user code to improve performance*/
/* some of these ops can be potential no-ops*/
extern int gasnete_coll_try_sync(gasnet_coll_handle_t _handle GASNETI_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_try_sync)
int _gasnet_coll_try_sync(gasnet_coll_handle_t _handle GASNETI_THREAD_FARG) {
  int _result = GASNET_OK;
  if_pt (_handle != GASNET_COLL_INVALID_HANDLE) {
    _result = gasnete_coll_try_sync(_handle GASNETI_THREAD_PASS);
    if (_result)
      gasneti_sync_reads();
  }
  GASNETI_TRACE_COLL_TRYSYNC(COLL_TRY_SYNC,_result);
  return _result;
}

extern int gasnete_coll_try_sync_some(gasnet_coll_handle_t *_phandle, size_t _numhandles GASNETI_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_try_sync_some)
int _gasnet_coll_try_sync_some(gasnet_coll_handle_t *_phandle, size_t _numhandles GASNETI_THREAD_FARG) {
  int _result = gasnete_coll_try_sync_some(_phandle, _numhandles GASNETI_THREAD_PASS);
  if (_result)
    gasneti_sync_reads();
  GASNETI_TRACE_COLL_TRYSYNC(COLL_TRY_SYNC_SOME,_result);
  return _result;
}

extern int gasnete_coll_try_sync_all(gasnet_coll_handle_t *_phandle, size_t _numhandles GASNETI_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_try_sync_all)
int _gasnet_coll_try_sync_all(gasnet_coll_handle_t *_phandle, size_t _numhandles GASNETI_THREAD_FARG) {
  int _result = gasnete_coll_try_sync_all(_phandle, _numhandles GASNETI_THREAD_PASS);
  if (_result)
    gasneti_sync_reads();
  GASNETI_TRACE_COLL_TRYSYNC(COLL_TRY_SYNC_ALL,_result);
  return _result;
}


#ifdef gasnete_coll_wait_sync
extern void
gasnete_coll_wait_sync(gasnet_coll_handle_t _handle GASNETI_THREAD_FARG);
#else
GASNETI_INLINE(gasnete_coll_wait_sync)
void gasnete_coll_wait_sync(gasnet_coll_handle_t _handle GASNETI_THREAD_FARG) {
  if_pt (_handle != GASNET_COLL_INVALID_HANDLE) {
    gasneti_waitwhile(gasnete_coll_try_sync(_handle GASNETI_THREAD_PASS) == GASNET_ERR_NOT_READY);
  }
}
#endif

GASNETI_INLINE(_gasnet_coll_wait_sync)
void _gasnet_coll_wait_sync(gasnet_coll_handle_t _handle GASNETI_THREAD_FARG) {
  GASNETI_TRACE_COLL_WAITSYNC_BEGIN();
  gasnete_coll_wait_sync(_handle GASNETI_THREAD_PASS);
  GASNETI_TRACE_COLL_WAITSYNC_END(COLL_WAIT_SYNC);
}


#ifdef gasnete_coll_wait_sync_some
extern void
gasnete_coll_wait_sync_some(gasnet_coll_handle_t *_phandle, size_t _numhandles GASNETI_THREAD_FARG);
#else
GASNETI_INLINE(gasnete_coll_wait_sync_some)
void gasnete_coll_wait_sync_some(gasnet_coll_handle_t *_phandle, size_t _numhandles GASNETI_THREAD_FARG) {
  gasneti_waitwhile(gasnete_coll_try_sync_some(_phandle,_numhandles GASNETI_THREAD_PASS) == GASNET_ERR_NOT_READY);
}
#endif
GASNETI_INLINE(_gasnet_coll_wait_sync_some)
void _gasnet_coll_wait_sync_some(gasnet_coll_handle_t *_phandle, size_t _numhandles GASNETI_THREAD_FARG) {
  GASNETI_TRACE_COLL_WAITSYNC_BEGIN();
  gasnete_coll_wait_sync_some(_phandle,_numhandles GASNETI_THREAD_PASS);
  GASNETI_TRACE_COLL_WAITSYNC_END(COLL_WAIT_SYNC_SOME);
}

#ifdef gasnete_coll_wait_sync_all
extern void
gasnete_coll_wait_sync_all(gasnet_coll_handle_t *_phandle, size_t _numhandles GASNETI_THREAD_FARG);
#else
GASNETI_INLINE(gasnete_coll_wait_sync_all)
void gasnete_coll_wait_sync_all(gasnet_coll_handle_t *_phandle, size_t _numhandles GASNETI_THREAD_FARG) {
  gasneti_waitwhile(gasnete_coll_try_sync_all(_phandle,_numhandles GASNETI_THREAD_PASS) == GASNET_ERR_NOT_READY);
}
#endif
GASNETI_INLINE(_gasnet_coll_wait_sync_all)
void _gasnet_coll_wait_sync_all(gasnet_coll_handle_t *_phandle, size_t _numhandles GASNETI_THREAD_FARG) {
  GASNETI_TRACE_COLL_WAITSYNC_BEGIN();
  gasnete_coll_wait_sync_all(_phandle,_numhandles GASNETI_THREAD_PASS);
  GASNETI_TRACE_COLL_WAITSYNC_END(COLL_WAIT_SYNC_ALL);
}

extern void gasnete_coll_barrier_notify(gasnete_coll_team_t _team, int _id, int _flags GASNETI_THREAD_FARG);
extern int gasnete_coll_barrier_try(gasnete_coll_team_t _team, int _id, int _flags GASNETI_THREAD_FARG);
extern int gasnete_coll_barrier_wait(gasnete_coll_team_t _team, int _id, int _flags GASNETI_THREAD_FARG);
extern int gasnete_coll_barrier(gasnete_coll_team_t _team, int _id, int _flags GASNETI_THREAD_FARG);
extern int gasnete_coll_barrier_result(gasnete_coll_team_t _team, int *_id GASNETI_THREAD_FARG);


#define gasnet_coll_barrier_notify(team, id, flags) gasnete_coll_barrier_notify(team, id, flags GASNETI_THREAD_GET)
#define gasnet_coll_barrier_try(team, id, flags) gasnete_coll_barrier_try(team, id, flags GASNETI_THREAD_GET)
#define gasnet_coll_barrier_wait(team, id, flags) gasnete_coll_barrier_wait(team, id, flags GASNETI_THREAD_GET)
#define gasnet_coll_barrier(team, id, flags) gasnete_coll_barrier(team, id, flags GASNETI_THREAD_GET)
#define gasnet_coll_barrier_result(team, id) gasnete_coll_barrier_result(team, id GASNETI_THREAD_GET)

#define gasnet_coll_try_sync(handle) \
       _gasnet_coll_try_sync(handle GASNETI_THREAD_GET)

#define gasnet_coll_try_sync_some(phandle,numhandles) \
       _gasnet_coll_try_sync_some(phandle,numhandles GASNETI_THREAD_GET)

#define gasnet_coll_try_sync_all(phandle,numhandles) \
       _gasnet_coll_try_sync_all(phandle,numhandles GASNETI_THREAD_GET)

#define gasnet_coll_wait_sync(handle) \
       _gasnet_coll_wait_sync(handle GASNETI_THREAD_GET)

#define gasnet_coll_wait_sync_some(phandle,numhandles) \
       _gasnet_coll_wait_sync_some(phandle,numhandles GASNETI_THREAD_GET)

#define gasnet_coll_wait_sync_all(phandle,numhandles) \
       _gasnet_coll_wait_sync_all(phandle,numhandles GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/
/* no point trying to inline the collectives since they will involve non trivial communication costs*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_broadcast_nb) 
gasnet_coll_handle_t _gasnet_coll_broadcast_nb(gasnet_team_handle_t _team,
                          void *_dst,
                          gasnet_image_t _srcimage, void *_src,
                          size_t _nbytes, int _flags GASNETI_THREAD_FARG);

#define gasnet_coll_broadcast_nb(team,dst,srcimage,src,nbytes,flags) \
       _gasnet_coll_broadcast_nb(team,dst,srcimage,src,nbytes,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_broadcast) 
void _gasnet_coll_broadcast(gasnet_team_handle_t _team,
                                   void *_dst,
                                   gasnet_image_t _srcimage, void *_src,
                                   size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_broadcast(team,dst,srcimage,src,nbytes,flags) \
       _gasnet_coll_broadcast(team,dst,srcimage,src,nbytes,flags GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_broadcastM_nb) 
gasnet_coll_handle_t _gasnet_coll_broadcastM_nb(gasnet_team_handle_t _team,
                           void * const _dstlist[],
                           gasnet_image_t _srcimage, void *_src,
                           size_t _nbytes, int _flags GASNETI_THREAD_FARG);

#define gasnet_coll_broadcastM_nb(team,dstlist,srcimage,src,nbytes,flags) \
       _gasnet_coll_broadcastM_nb(team,dstlist,srcimage,src,nbytes,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_broadcastM) 
void _gasnet_coll_broadcastM(gasnet_team_handle_t _team,
                                    void * const _dstlist[],
                                    gasnet_image_t _srcimage, void *_src,
                                    size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_broadcastM(team,dstlist,srcimage,src,nbytes,flags) \
       _gasnet_coll_broadcastM(team,dstlist,srcimage,src,nbytes,flags GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_scatter_nb) 
gasnet_coll_handle_t _gasnet_coll_scatter_nb(gasnet_team_handle_t _team,
                        void *_dst,
                        gasnet_image_t _srcimage, void *_src,
                        size_t _nbytes, int _flags GASNETI_THREAD_FARG);

#define gasnet_coll_scatter_nb(team,dst,srcimage,src,nbytes,flags) \
       _gasnet_coll_scatter_nb(team,dst,srcimage,src,nbytes,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_scatter) 
void _gasnet_coll_scatter(gasnet_team_handle_t _team,
                          void *_dst, gasnet_image_t _srcimage, void *_src,
                                 size_t _nbytes, int _flags GASNETI_THREAD_FARG); 
#define gasnet_coll_scatter(team,dst,srcimage,src,nbytes,flags) \
       _gasnet_coll_scatter(team,dst,srcimage,src,nbytes,flags GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_scatterM_nb) 
gasnet_coll_handle_t _gasnet_coll_scatterM_nb(gasnet_team_handle_t _team,
                         void * const _dstlist[],
                         gasnet_image_t _srcimage, void *_src,
                         size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_scatterM_nb(team,dstlist,srcimage,src,nbytes,flags) \
       _gasnet_coll_scatterM_nb(team,dstlist,srcimage,src,nbytes,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_scatterM) 
void _gasnet_coll_scatterM(gasnet_team_handle_t _team,
                                  void * const _dstlist[],
                                  gasnet_image_t _srcimage, void *_src,
                                  size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_scatterM(team,dstlist,srcimage,src,nbytes,flags) \
       _gasnet_coll_scatterM(team,dstlist,srcimage,src,nbytes,flags GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/

GASNETI_COLL_FN_HEADER(_gasnet_coll_gather_nb) 
gasnet_coll_handle_t _gasnet_coll_gather_nb(gasnet_team_handle_t _team,
                       gasnet_image_t _dstimage, void *_dst,
                       void *_src,
                       size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_gather_nb(team,dstimage,dst,src,nbytes,flags) \
       _gasnet_coll_gather_nb(team,dstimage,dst,src,nbytes,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_gather) 
void _gasnet_coll_gather(gasnet_team_handle_t _team,
                                gasnet_image_t _dstimage, void *_dst,
                                void *_src,
                                size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_gather(team,dstimage,dst,src,nbytes,flags) \
       _gasnet_coll_gather(team,dstimage,dst,src,nbytes,flags GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/

GASNETI_COLL_FN_HEADER(_gasnet_coll_gatherM_nb) 
gasnet_coll_handle_t _gasnet_coll_gatherM_nb(gasnet_team_handle_t _team,
                        gasnet_image_t _dstimage, void *_dst,
                        void * const _srclist[],
                        size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_gatherM_nb(team,dstimage,dst,srclist,nbytes,flags) \
       _gasnet_coll_gatherM_nb(team,dstimage,dst,srclist,nbytes,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_gatherM) 
void _gasnet_coll_gatherM(gasnet_team_handle_t _team,
                                 gasnet_image_t _dstimage, void *_dst,
                                 void * const _srclist[],
                                 size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_gatherM(team,dstimage,dst,srclist,nbytes,flags) \
       _gasnet_coll_gatherM(team,dstimage,dst,srclist,nbytes,flags GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_gather_all_nb) 
gasnet_coll_handle_t _gasnet_coll_gather_all_nb(gasnet_team_handle_t _team,
                           void *_dst, void *_src,
                           size_t _nbytes, int _flags GASNETI_THREAD_FARG) ;
#define gasnet_coll_gather_all_nb(team,dst,src,nbytes,flags) \
       _gasnet_coll_gather_all_nb(team,dst,src,nbytes,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_gather_all) 
void _gasnet_coll_gather_all(gasnet_team_handle_t _team,
                                    void *_dst, void *_src,
                                    size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_gather_all(team,dst,src,nbytes,flags) \
       _gasnet_coll_gather_all(team,dst,src,nbytes,flags GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/

GASNETI_COLL_FN_HEADER(_gasnet_coll_gather_allM_nb) 
gasnet_coll_handle_t _gasnet_coll_gather_allM_nb(gasnet_team_handle_t _team,
                            void * const _dstlist[], void * const _srclist[],
                            size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_gather_allM_nb(team,dstlist,srclist,nbytes,flags) \
       _gasnet_coll_gather_allM_nb(team,dstlist,srclist,nbytes,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_gather_allM) 
void _gasnet_coll_gather_allM(gasnet_team_handle_t _team,
                              void * const _dstlist[], void * const _srclist[],
                              size_t _nbytes, int _flags GASNETI_THREAD_FARG) ;
#define gasnet_coll_gather_allM(team,dstlist,srclist,nbytes,flags) \
       _gasnet_coll_gather_allM(team,dstlist,srclist,nbytes,flags GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_exchange_nb) 
gasnet_coll_handle_t _gasnet_coll_exchange_nb(gasnet_team_handle_t _team,
                         void *_dst, void *_src,
                         size_t _nbytes, int _flags GASNETI_THREAD_FARG) ;
#define gasnet_coll_exchange_nb(team,dst,src,nbytes,flags) \
       _gasnet_coll_exchange_nb(team,dst,src,nbytes,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_exchange) 
void _gasnet_coll_exchange(gasnet_team_handle_t _team,
                                  void *_dst, void *_src,
                                  size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_exchange(team,dst,src,nbytes,flags) \
       _gasnet_coll_exchange(team,dst,src,nbytes,flags GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_exchangeM_nb) 
gasnet_coll_handle_t _gasnet_coll_exchangeM_nb(gasnet_team_handle_t _team,
                          void * const _dstlist[], void * const _srclist[],
                          size_t _nbytes, int _flags GASNETI_THREAD_FARG) ;
#define gasnet_coll_exchangeM_nb(team,dstlist,srclist,nbytes,flags) \
       _gasnet_coll_exchangeM_nb(team,dstlist,srclist,nbytes,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_exchangeM) 
void _gasnet_coll_exchangeM(gasnet_team_handle_t _team,
                                   void * const _dstlist[], void * const _srclist[],
                                   size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_exchangeM(team,dstlist,srclist,nbytes,flags) \
       _gasnet_coll_exchangeM(team,dstlist,srclist,nbytes,flags GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_reduce_nb) 
gasnet_coll_handle_t _gasnet_coll_reduce_nb(gasnet_team_handle_t _team,
                       gasnet_image_t _dstimage, void *_dst,
                       void *_src, size_t _src_blksz, size_t _src_offset,
                       size_t _elem_size, size_t _elem_count,
                       gasnet_coll_fn_handle_t _func, int _func_arg,
                       int _flags GASNETI_THREAD_FARG) ;
#define gasnet_coll_reduce_nb(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_reduce_nb(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_reduce) 
void _gasnet_coll_reduce(gasnet_team_handle_t _team,
                                gasnet_image_t _dstimage, void *_dst,
                                void *_src, size_t _src_blksz, size_t _src_offset,
                                size_t _elem_size, size_t _elem_count,
                                gasnet_coll_fn_handle_t _func, int _func_arg,
                                int _flags GASNETI_THREAD_FARG) ;
#define gasnet_coll_reduce(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_reduce(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETI_THREAD_GET);

/*---------------------------------------------------------------------------------*/

GASNETI_COLL_FN_HEADER(_gasnet_coll_reduceM_nb) 
gasnet_coll_handle_t _gasnet_coll_reduceM_nb(gasnet_team_handle_t _team,
                        gasnet_image_t _dstimage, void *_dst,
                        void * const _srclist[], size_t _src_blksz, size_t _src_offset,
                        size_t _elem_size, size_t _elem_count,
                        gasnet_coll_fn_handle_t _func, int _func_arg,
                        int _flags GASNETI_THREAD_FARG) ;
#define gasnet_coll_reduceM_nb(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_reduceM_nb(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_reduceM) 
void _gasnet_coll_reduceM(gasnet_team_handle_t _team,
                                 gasnet_image_t _dstimage, void *_dst,
                                 void * const _srclist[], size_t _src_blksz, size_t _src_offset,
                                 size_t _elem_size, size_t _elem_count,
                                 gasnet_coll_fn_handle_t _func, int _func_arg,
                                 int _flags GASNETI_THREAD_FARG) ;
#define gasnet_coll_reduceM(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_reduceM(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETI_THREAD_GET);

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_scan_nb) 
gasnet_coll_handle_t _gasnet_coll_scan_nb(gasnet_team_handle_t _team,
                     void *_dst, size_t _dst_blksz, size_t _dst_offset,
                     void *_src, size_t _src_blksz, size_t _src_offset,
                     size_t _elem_size, size_t _elem_count,
                     gasnet_coll_fn_handle_t _func, int _func_arg,
                     int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_scan_nb(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_scan_nb(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_scan) 
void _gasnet_coll_scan(gasnet_team_handle_t _team,
                              void *_dst, size_t _dst_blksz, size_t _dst_offset,
                              void *_src, size_t _src_blksz, size_t _src_offset,
                              size_t _elem_size, size_t _elem_count,
                              gasnet_coll_fn_handle_t _func, int _func_arg,
                              int _flags GASNETI_THREAD_FARG) ;
#define gasnet_coll_scan(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_scan(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETI_THREAD_GET);

/*---------------------------------------------------------------------------------*/

GASNETI_COLL_FN_HEADER(_gasnet_coll_scanM_nb) 
gasnet_coll_handle_t _gasnet_coll_scanM_nb(gasnet_team_handle_t _team,
                      void * const _dstlist[], size_t _dst_blksz, size_t _dst_offset,
                      void * const _srclist[], size_t _src_blksz, size_t _src_offset,
                      size_t _elem_size, size_t _elem_count,
                      gasnet_coll_fn_handle_t _func, int _func_arg,
                      int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_scanM_nb(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_scanM_nb(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_scanM) 
void _gasnet_coll_scanM(gasnet_team_handle_t _team,
                               void * const _dstlist[], size_t _dst_blksz, size_t _dst_offset,
                               void * const _srclist[], size_t _src_blksz, size_t _src_offset,
                               size_t _elem_size, size_t _elem_count,
                               gasnet_coll_fn_handle_t _func, int _func_arg,
                               int _flags GASNETI_THREAD_FARG); 
#define gasnet_coll_scanM(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_scanM(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETI_THREAD_GET);

#undef GASNETI_COLL_FN_HEADER

GASNETI_END_NOWARN
GASNETI_END_EXTERNC

#endif
