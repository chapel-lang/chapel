/*   $Source: /var/local/cvs/gasnet/gasnet_coll.h,v $
 *     $Date: 2007/10/18 19:17:18 $
 * $Revision: 1.53 $
 * Description: GASNet Extended API Collective declarations
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_COLL_H
#define _GASNET_COLL_H

#include <gasnet.h>

GASNETI_BEGIN_EXTERNC
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

#define GASNET_COLL_SINGLE	(1<<6)
#define GASNET_COLL_LOCAL	(1<<7)

#define GASNET_COLL_AGGREGATE	(1<<8)

#define GASNET_COLL_DST_IN_SEGMENT	(1<<9)
#define GASNET_COLL_SRC_IN_SEGMENT	(1<<10)

/* Scan (prefix reduction) flags - NO DEFAULT */
#define GASNET_COLL_INCLUSIVE_SCAN	(1<<11)
#define GASNET_COLL_EXCLUSIVE_SCAN	(1<<12)

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

/* Handle type for collective teams: */
#ifndef GASNETE_COLL_TEAMS_OVERRIDE
struct gasnete_coll_team_t_;
typedef struct gasnete_coll_team_t_ *gasnete_coll_team_t;
typedef gasnete_coll_team_t gasnet_team_handle_t;
/*change this so even the TEAM_ALL has a default team allocated rather than NULL*/
gasnet_team_handle_t gasnete_coll_team_all;
#define GASNET_TEAM_ALL gasnete_coll_team_all
#endif

/*---------------------------------------------------------------------------------*
 * Start of generic framework for tree-based reference implementations
 *---------------------------------------------------------------------------------*/


extern void gasnet_coll_set_tree_kind(char *); 
extern void gasnet_coll_set_fanout(int); 
                                                                                                              

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
 *              Upon return the 'handle' field of each entry is set
 *              to the value that must be passed to the collective.
 *  fn_count:   The number of entries in 'fn_tbl'.  Must agree across
 *              all nodes or the behavior is undefined.
 *  init_flags: Presently unused.  Must be 0.
 */

#ifndef gasnet_coll_init
  GASNETI_COLL_FN_HEADER(gasnete_coll_init) 
  void gasnete_coll_init(const gasnet_image_t images[], gasnet_image_t my_image,
		  		gasnet_coll_fn_entry_t fn_tbl[], size_t fn_count,
		  		int init_flags GASNETE_THREAD_FARG);
  #define gasnet_coll_init(im,mi,fn,fc,fl) \
		gasnete_coll_init(im,mi,fn,fc,fl GASNETE_THREAD_GET)
#endif

/*---------------------------------------------------------------------------------*/

/* Include all the code for wait/try sync so that we can attempt to inline them into the user code to improve performance*/
/* some of these ops can be potential no-ops*/
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

extern int gasnete_coll_try_sync_some(gasnet_coll_handle_t *phandle, size_t numhandles GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_try_sync_some)
int _gasnet_coll_try_sync_some(gasnet_coll_handle_t *phandle, size_t numhandles GASNETE_THREAD_FARG) {
  int result = gasnete_coll_try_sync_some(phandle, numhandles GASNETE_THREAD_PASS);
  if (result)
    gasneti_sync_reads();
  GASNETI_TRACE_COLL_TRYSYNC(COLL_TRY_SYNC_SOME,result);
  return result;
}

extern int gasnete_coll_try_sync_all(gasnet_coll_handle_t *phandle, size_t numhandles GASNETE_THREAD_FARG);
GASNETI_INLINE(_gasnet_coll_try_sync_all)
int _gasnet_coll_try_sync_all(gasnet_coll_handle_t *phandle, size_t numhandles GASNETE_THREAD_FARG) {
  int result = gasnete_coll_try_sync_all(phandle, numhandles GASNETE_THREAD_PASS);
  if (result)
    gasneti_sync_reads();
  GASNETI_TRACE_COLL_TRYSYNC(COLL_TRY_SYNC_ALL,result);
  return result;
}


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


#define gasnet_coll_try_sync(handle) \
       _gasnet_coll_try_sync(handle GASNETE_THREAD_GET)

#define gasnet_coll_try_sync_some(phandle,numhandles) \
       _gasnet_coll_try_sync_some(phandle,numhandles GASNETE_THREAD_GET)

#define gasnet_coll_try_sync_all(phandle,numhandles) \
       _gasnet_coll_try_sync_all(phandle,numhandles GASNETE_THREAD_GET)

#define gasnet_coll_wait_sync(handle) \
       _gasnet_coll_wait_sync(handle GASNETE_THREAD_GET)

#define gasnet_coll_wait_sync_some(phandle,numhandles) \
       _gasnet_coll_wait_sync_some(phandle,numhandles GASNETE_THREAD_GET)

#define gasnet_coll_wait_sync_all(phandle,numhandles) \
       _gasnet_coll_wait_sync_all(phandle,numhandles GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/
/* no point trying to inline the collectives since they will involve non trivial communication costs*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_broadcast_nb) 
gasnet_coll_handle_t _gasnet_coll_broadcast_nb(gasnet_team_handle_t team,
                          void *dst,
                          gasnet_image_t srcimage, void *src,
                          size_t nbytes, int flags GASNETE_THREAD_FARG);

#define gasnet_coll_broadcast_nb(team,dst,srcimage,src,nbytes,flags) \
       _gasnet_coll_broadcast_nb(team,dst,srcimage,src,nbytes,flags GASNETE_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_broadcast) 
void _gasnet_coll_broadcast(gasnet_team_handle_t team,
                                   void *dst,
                                   gasnet_image_t srcimage, void *src,
                                   size_t nbytes, int flags GASNETE_THREAD_FARG);
#define gasnet_coll_broadcast(team,dst,srcimage,src,nbytes,flags) \
       _gasnet_coll_broadcast(team,dst,srcimage,src,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_broadcastM_nb) 
gasnet_coll_handle_t _gasnet_coll_broadcastM_nb(gasnet_team_handle_t team,
                           void * const dstlist[],
                           gasnet_image_t srcimage, void *src,
                           size_t nbytes, int flags GASNETE_THREAD_FARG);

#define gasnet_coll_broadcastM_nb(team,dstlist,srcimage,src,nbytes,flags) \
       _gasnet_coll_broadcastM_nb(team,dstlist,srcimage,src,nbytes,flags GASNETE_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_broadcastM) 
void _gasnet_coll_broadcastM(gasnet_team_handle_t team,
                                    void * const dstlist[],
                                    gasnet_image_t srcimage, void *src,
                                    size_t nbytes, int flags GASNETE_THREAD_FARG);
#define gasnet_coll_broadcastM(team,dstlist,srcimage,src,nbytes,flags) \
       _gasnet_coll_broadcastM(team,dstlist,srcimage,src,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_scatter_nb) 
gasnet_coll_handle_t _gasnet_coll_scatter_nb(gasnet_team_handle_t team,
                        void *dst,
                        gasnet_image_t srcimage, void *src,
                        size_t nbytes, int flags GASNETE_THREAD_FARG);

#define gasnet_coll_scatter_nb(team,dst,srcimage,src,nbytes,flags) \
       _gasnet_coll_scatter_nb(team,dst,srcimage,src,nbytes,flags GASNETE_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_scatter) 
void _gasnet_coll_scatter(gasnet_team_handle_t team,
                          void *dst, gasnet_image_t srcimage, void *src,
                                 size_t nbytes, int flags GASNETE_THREAD_FARG); 
#define gasnet_coll_scatter(team,dst,srcimage,src,nbytes,flags) \
       _gasnet_coll_scatter(team,dst,srcimage,src,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_scatterM_nb) 
gasnet_coll_handle_t _gasnet_coll_scatterM_nb(gasnet_team_handle_t team,
                         void * const dstlist[],
                         gasnet_image_t srcimage, void *src,
                         size_t nbytes, int flags GASNETE_THREAD_FARG);
#define gasnet_coll_scatterM_nb(team,dstlist,srcimage,src,nbytes,flags) \
       _gasnet_coll_scatterM_nb(team,dstlist,srcimage,src,nbytes,flags GASNETE_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_scatterM) 
void _gasnet_coll_scatterM(gasnet_team_handle_t team,
                                  void * const dstlist[],
                                  gasnet_image_t srcimage, void *src,
                                  size_t nbytes, int flags GASNETE_THREAD_FARG);
#define gasnet_coll_scatterM(team,dstlist,srcimage,src,nbytes,flags) \
       _gasnet_coll_scatterM(team,dstlist,srcimage,src,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/

GASNETI_COLL_FN_HEADER(_gasnet_coll_gather_nb) 
gasnet_coll_handle_t _gasnet_coll_gather_nb(gasnet_team_handle_t team,
                       gasnet_image_t dstimage, void *dst,
                       void *src,
                       size_t nbytes, int flags GASNETE_THREAD_FARG);
#define gasnet_coll_gather_nb(team,dstimage,dst,src,nbytes,flags) \
       _gasnet_coll_gather_nb(team,dstimage,dst,src,nbytes,flags GASNETE_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_gather) 
void _gasnet_coll_gather(gasnet_team_handle_t team,
                                gasnet_image_t dstimage, void *dst,
                                void *src,
                                size_t nbytes, int flags GASNETE_THREAD_FARG);
#define gasnet_coll_gather(team,dstimage,dst,src,nbytes,flags) \
       _gasnet_coll_gather(team,dstimage,dst,src,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/

GASNETI_COLL_FN_HEADER(_gasnet_coll_gatherM_nb) 
gasnet_coll_handle_t _gasnet_coll_gatherM_nb(gasnet_team_handle_t team,
                        gasnet_image_t dstimage, void *dst,
                        void * const srclist[],
                        size_t nbytes, int flags GASNETE_THREAD_FARG);
#define gasnet_coll_gatherM_nb(team,dstimage,dst,srclist,nbytes,flags) \
       _gasnet_coll_gatherM_nb(team,dstimage,dst,srclist,nbytes,flags GASNETE_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_gatherM) 
void _gasnet_coll_gatherM(gasnet_team_handle_t team,
                                 gasnet_image_t dstimage, void *dst,
                                 void * const srclist[],
                                 size_t nbytes, int flags GASNETE_THREAD_FARG);
#define gasnet_coll_gatherM(team,dstimage,dst,srclist,nbytes,flags) \
       _gasnet_coll_gatherM(team,dstimage,dst,srclist,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_gather_all_nb) 
gasnet_coll_handle_t _gasnet_coll_gather_all_nb(gasnet_team_handle_t team,
                           void *dst, void *src,
                           size_t nbytes, int flags GASNETE_THREAD_FARG) ;
#define gasnet_coll_gather_all_nb(team,dst,src,nbytes,flags) \
       _gasnet_coll_gather_all_nb(team,dst,src,nbytes,flags GASNETE_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_gather_all) 
void _gasnet_coll_gather_all(gasnet_team_handle_t team,
                                    void *dst, void *src,
                                    size_t nbytes, int flags GASNETE_THREAD_FARG);
#define gasnet_coll_gather_all(team,dst,src,nbytes,flags) \
       _gasnet_coll_gather_all(team,dst,src,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/

GASNETI_COLL_FN_HEADER(_gasnet_coll_gather_allM_nb) 
gasnet_coll_handle_t _gasnet_coll_gather_allM_nb(gasnet_team_handle_t team,
                            void * const dstlist[], void * const srclist[],
                            size_t nbytes, int flags GASNETE_THREAD_FARG);
#define gasnet_coll_gather_allM_nb(team,dstlist,srclist,nbytes,flags) \
       _gasnet_coll_gather_allM_nb(team,dstlist,srclist,nbytes,flags GASNETE_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_gather_allM) 
void _gasnet_coll_gather_allM(gasnet_team_handle_t team,
                              void * const dstlist[], void * const srclist[],
                              size_t nbytes, int flags GASNETE_THREAD_FARG) ;
#define gasnet_coll_gather_allM(team,dstlist,srclist,nbytes,flags) \
       _gasnet_coll_gather_allM(team,dstlist,srclist,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_exchange_nb) 
gasnet_coll_handle_t _gasnet_coll_exchange_nb(gasnet_team_handle_t team,
                         void *dst, void *src,
                         size_t nbytes, int flags GASNETE_THREAD_FARG) ;
#define gasnet_coll_exchange_nb(team,dst,src,nbytes,flags) \
       _gasnet_coll_exchange_nb(team,dst,src,nbytes,flags GASNETE_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_exchange) 
void _gasnet_coll_exchange(gasnet_team_handle_t team,
                                  void *dst, void *src,
                                  size_t nbytes, int flags GASNETE_THREAD_FARG);
#define gasnet_coll_exchange(team,dst,src,nbytes,flags) \
       _gasnet_coll_exchange(team,dst,src,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_exchangeM_nb) 
gasnet_coll_handle_t _gasnet_coll_exchangeM_nb(gasnet_team_handle_t team,
                          void * const dstlist[], void * const srclist[],
                          size_t nbytes, int flags GASNETE_THREAD_FARG) ;
#define gasnet_coll_exchangeM_nb(team,dstlist,srclist,nbytes,flags) \
       _gasnet_coll_exchangeM_nb(team,dstlist,srclist,nbytes,flags GASNETE_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_exchangeM) 
void _gasnet_coll_exchangeM(gasnet_team_handle_t team,
                                   void * const dstlist[], void * const srclist[],
                                   size_t nbytes, int flags GASNETE_THREAD_FARG);
#define gasnet_coll_exchangeM(team,dstlist,srclist,nbytes,flags) \
       _gasnet_coll_exchangeM(team,dstlist,srclist,nbytes,flags GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_reduce_nb) 
gasnet_coll_handle_t _gasnet_coll_reduce_nb(gasnet_team_handle_t team,
                       gasnet_image_t dstimage, void *dst,
                       void *src, size_t src_blksz, size_t src_offset,
                       size_t elem_size, size_t elem_count,
                       gasnet_coll_fn_handle_t func, int func_arg,
                       int flags GASNETE_THREAD_FARG) ;
#define gasnet_coll_reduce_nb(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_reduce_nb(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_reduce) 
void _gasnet_coll_reduce(gasnet_team_handle_t team,
                                gasnet_image_t dstimage, void *dst,
                                void *src, size_t src_blksz, size_t src_offset,
                                size_t elem_size, size_t elem_count,
                                gasnet_coll_fn_handle_t func, int func_arg,
                                int flags GASNETE_THREAD_FARG) ;
#define gasnet_coll_reduce(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_reduce(team,dstimage,dst,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_GET);

/*---------------------------------------------------------------------------------*/

GASNETI_COLL_FN_HEADER(_gasnet_coll_reduceM_nb) 
gasnet_coll_handle_t _gasnet_coll_reduceM_nb(gasnet_team_handle_t team,
                        gasnet_image_t dstimage, void *dst,
                        void * const srclist[], size_t src_blksz, size_t src_offset,
                        size_t elem_size, size_t elem_count,
                        gasnet_coll_fn_handle_t func, int func_arg,
                        int flags GASNETE_THREAD_FARG) ;
#define gasnet_coll_reduceM_nb(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_reduceM_nb(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_reduceM) 
void _gasnet_coll_reduceM(gasnet_team_handle_t team,
                                 gasnet_image_t dstimage, void *dst,
                                 void * const srclist[], size_t src_blksz, size_t src_offset,
                                 size_t elem_size, size_t elem_count,
                                 gasnet_coll_fn_handle_t func, int func_arg,
                                 int flags GASNETE_THREAD_FARG) ;
#define gasnet_coll_reduceM(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_reduceM(team,dstimage,dst,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_GET);

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_scan_nb) 
gasnet_coll_handle_t _gasnet_coll_scan_nb(gasnet_team_handle_t team,
                     void *dst, size_t dst_blksz, size_t dst_offset,
                     void *src, size_t src_blksz, size_t src_offset,
                     size_t elem_size, size_t elem_count,
                     gasnet_coll_fn_handle_t func, int func_arg,
                     int flags GASNETE_THREAD_FARG);
#define gasnet_coll_scan_nb(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_scan_nb(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_scan) 
void _gasnet_coll_scan(gasnet_team_handle_t team,
                              void *dst, size_t dst_blksz, size_t dst_offset,
                              void *src, size_t src_blksz, size_t src_offset,
                              size_t elem_size, size_t elem_count,
                              gasnet_coll_fn_handle_t func, int func_arg,
                              int flags GASNETE_THREAD_FARG) ;
#define gasnet_coll_scan(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_scan(team,dst,dst_blksz,dst_offset,src,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_GET);

/*---------------------------------------------------------------------------------*/

GASNETI_COLL_FN_HEADER(_gasnet_coll_scanM_nb) 
gasnet_coll_handle_t _gasnet_coll_scanM_nb(gasnet_team_handle_t team,
                      void * const dstlist[], size_t dst_blksz, size_t dst_offset,
                      void * const srclist[], size_t src_blksz, size_t src_offset,
                      size_t elem_size, size_t elem_count,
                      gasnet_coll_fn_handle_t func, int func_arg,
                      int flags GASNETE_THREAD_FARG);
#define gasnet_coll_scanM_nb(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_scanM_nb(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_scanM) 
void _gasnet_coll_scanM(gasnet_team_handle_t team,
                               void * const dstlist[], size_t dst_blksz, size_t dst_offset,
                               void * const srclist[], size_t src_blksz, size_t src_offset,
                               size_t elem_size, size_t elem_count,
                               gasnet_coll_fn_handle_t func, int func_arg,
                               int flags GASNETE_THREAD_FARG); 
#define gasnet_coll_scanM(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags) \
       _gasnet_coll_scanM(team,dstlist,dst_blksz,dst_offset,srclist,src_blksz,src_offset,elem_size,elem_count,func,func_arg,flags GASNETE_THREAD_GET);

#undef GASNETI_COLL_FN_HEADER

GASNETI_END_EXTERNC

#endif
