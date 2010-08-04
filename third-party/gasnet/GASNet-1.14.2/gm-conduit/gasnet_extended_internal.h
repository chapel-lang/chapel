/*   $Source: /var/local/cvs/gasnet/gm-conduit/gasnet_extended_internal.h,v $
 *     $Date: 2010/04/17 01:38:44 $
 * $Revision: 1.36.8.1 $
 * Description: GASNet header for internal definitions in Extended API
 * Copyright 2002, Christian Bell <csbell@cs.berkeley.edu>
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_EXTENDED_INTERNAL_H
#define _GASNET_EXTENDED_INTERNAL_H

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#include <firehose.h>

/* ------------------------------------------------------------------------------------ */
#if GASNETI_STATS_OR_TRACE
  #define GASNETC_FIREHOSE_TRACE
  typedef
  enum gasnetc_fh_stats { 
	fh_none, fh_onesided, fh_one, fh_many
  } gasnetc_fh_stats_t;
#endif

/* gasnet_handle_t is a void* pointer to a gasnete_op_t, 
   which is either a gasnete_eop_t or an gasnete_iop_t
   */
typedef struct _gasnete_op_t {
	uint8_t			flags;		/*  flags - type tag */
	gasnete_threadidx_t	threadidx;	/*  thread that owns me */
} gasnete_op_t;

/* for compactness, eops address each other in the free list using a gasnete_eopaddr_t */ 
typedef union _gasnete_eopaddr_t {
  struct {
    uint8_t _bufferidx;
    uint8_t _eopidx;
  } compaddr;
  uint16_t fulladdr;
} gasnete_eopaddr_t;
#define bufferidx compaddr._bufferidx
#define eopidx compaddr._eopidx

#define gasnete_eopaddr_equal(addr1,addr2) ((addr1).fulladdr == (addr2).fulladdr)
#define gasnete_eopaddr_isnil(addr) ((addr).fulladdr == EOPADDR_NIL.fulladdr)
#define EOPADDR_NIL gasnete_EOPADDR_NIL

/* -------------------------------------------------------------------------- */
/* Explicit ops, used when operations are known to require only one send in
 * order to complete */
struct _gasnete_iop_t;
typedef struct _gasnete_eop_t {
	uint8_t		flags;	/*  state flags */

	gasnete_threadidx_t	threadidx;  /*  thread that owns me */

	const firehose_request_t	*req_local;
	const firehose_request_t	req_remote;

	gasnet_node_t		node;		/* also in req_remote.node */
	uintptr_t		dest;
	uintptr_t		src;
	uint32_t		len;

	struct _gasnete_iop_t	*iop;
	struct _gasnete_eop_t	*next;		/* when used in FIFO */

	#ifdef GASNETC_FIREHOSE_TRACE
	gasnetc_fh_stats_t	fh_stats;
	gasneti_tick_t	starttime;
	#endif

	gasnete_eopaddr_t	addr;      /*  next cell while in free list, 
					       my own eopaddr_t while in use */
} gasnete_eop_t;

/* -------------------------------------------------------------------------- */
/* Implicit ops, used by the extended reference implementation */
typedef struct _gasnete_iop_t {
  	uint8_t		flags;	/*  state flags */

	gasnete_threadidx_t	threadidx;  /*  thread that owns me */
	gasnet_node_t		node;
  	uintptr_t		dest;		/* remote RDMA addr */
	uintptr_t		src;		/* source addr */
	uint32_t		len;		/* length */

	int	initiated_get_cnt;     /*  count of get ops initiated */
	int	initiated_put_cnt;     /*  count of put ops initiated */

	struct _gasnete_iop_t *next;    /*  next cell while in free list, deferred 
				            iop while being filled */

	/*  make sure the counters live on different cache lines for SMP's */
	uint8_t pad[MAX(8,(ssize_t)(GASNETI_CACHE_LINE_BYTES - sizeof(void*) - sizeof(int)))]; 

	gasneti_weakatomic_t completed_get_cnt;     /*  count of get ops completed */
	gasneti_weakatomic_t completed_put_cnt;     /*  count of put ops completed */
} gasnete_iop_t;

/* -------------------------------------------------------------------------- */
typedef struct _gasnete_threaddata_t {
  GASNETE_COMMON_THREADDATA_FIELDS /* MUST come first, for reserved ptrs */

	int eop_num_bufs;	/*  number of valid buffer entries */
	gasnete_eop_t		*eop_bufs[256]; /*  buffers of eops */
	gasnete_eopaddr_t	eop_free;   /*  free list of eops */

	int	in_gm_unknown;

	/*  stack of iops - head is active iop servicing new implicit ops */
	gasnete_iop_t *current_iop;  
	gasnete_iop_t *iop_free;      /*  free list of iops */
} gasnete_threaddata_t;
/* -------------------------------------------------------------------------- */

/* gasnete_op_t flags field */
#define OPTYPE_EXPLICIT		0x00  /* gasnete_eop_new() relies on this value */
#define OPTYPE_IMPLICIT		0x80
#define OPTYPE(op)		((op)->flags & 0x80)
GASNETI_INLINE(SET_OPTYPE)
void SET_OPTYPE(gasnete_op_t *op, uint8_t type) {
	op->flags = (op->flags & 0x7F) | (type & 0x80);
	gasneti_assert(OPTYPE(op) == type);
}

/*  state */
#define OPSTATE_FREE		0   /* gasnete_eop_new() relies on this value */
#define OPSTATE_INFLIGHT	1
#define OPSTATE_COMPLETE	2
#define OPSTATE(op)		((op)->flags & 0x03) 
#define OPMISC_NONAMBUF		4
#define OPMISC_AMBUF		8
#define OPMISC(op)		((op)->flags & 0x0C)
GASNETI_INLINE(SET_OPSTATE)
void SET_OPSTATE(gasnete_eop_t *op, uint8_t state) {
	op->flags = (op->flags & 0xFC) | (state & 0x03);
	/* RACE: If we are marking the op COMPLETE, don't assert for completion
	 * state as another thread spinning on the op may already have changed
	 * the state. */
	gasneti_assert(state == OPSTATE_COMPLETE ? 1 : OPSTATE(op) == state);
}

GASNETI_INLINE(SET_OPMISC)
void SET_OPMISC(gasnete_eop_t *op, uint8_t misc) {
	op->flags = (op->flags & 0xF3) | (misc & 0x0C);
	gasneti_assert(OPMISC(op) == misc);
}

/* New op creation, gets new op and marks it in flight */
gasnete_eop_t	*gasnete_eop_new(gasnete_threaddata_t * const thread);
gasnete_iop_t	*gasnete_iop_new(gasnete_threaddata_t * const thread);

/*  query an eop for completeness */
int		gasnete_op_isdone(gasnete_op_t *op);

/*  mark an op done - isget ignored for explicit ops */
void		gasnete_op_markdone(gasnete_op_t *op, int isget);
void		gasnete_op_free(gasnete_op_t *op);

#define GASNETE_EOPADDR_TO_PTR(threaddata, eopaddr)                      \
      (gasneti_memcheck(threaddata),                                     \
       gasneti_assert(!gasnete_eopaddr_isnil(eopaddr)),                  \
       gasneti_assert((eopaddr).bufferidx < (threaddata)->eop_num_bufs), \
       gasneti_memcheck((threaddata)->eop_bufs[(eopaddr).bufferidx]),    \
       (threaddata)->eop_bufs[(eopaddr).bufferidx] + (eopaddr).eopidx)

#if GASNET_DEBUG
  /* check an in-flight/complete eop */
  #define gasnete_eop_check(eop) do {                                \
    gasnete_threaddata_t * _th;                                      \
    gasneti_assert(OPTYPE(eop) == OPTYPE_EXPLICIT);                  \
    gasneti_assert(OPSTATE(eop) == OPSTATE_INFLIGHT ||               \
                   OPSTATE(eop) == OPSTATE_COMPLETE);                \
    gasnete_assert_valid_threadid((eop)->threadidx);                 \
    _th = gasnete_threadtable[(eop)->threadidx];                     \
    gasneti_assert(GASNETE_EOPADDR_TO_PTR(_th, (eop)->addr) == eop); \
  } while (0)
  #define gasnete_iop_check(iop) do {                         \
    int _temp; gasnete_iop_t *_tmp_next;                      \
    gasneti_memcheck(iop);                                    \
    _tmp_next = (iop)->next;                                  \
    if (_tmp_next != NULL) _gasnete_iop_check(_tmp_next);     \
    gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);           \
    gasnete_assert_valid_threadid((iop)->threadidx);          \
    _temp = gasneti_weakatomic_read(&((iop)->completed_put_cnt),0); \
    if (_temp <= 65000) /* prevent race condition on reset */ \
      gasneti_assert((iop)->initiated_put_cnt >= _temp);      \
    _temp = gasneti_weakatomic_read(&((iop)->completed_get_cnt),0); \
    if (_temp <= 65000) /* prevent race condition on reset */ \
      gasneti_assert((iop)->initiated_get_cnt >= _temp);      \
  } while (0)
  extern void _gasnete_iop_check(gasnete_iop_t *iop);
#else
  #define gasnete_eop_check(eop)   ((void)0)
  #define gasnete_iop_check(iop)   ((void)0)
#endif

/* 1 = scatter newly allocated eops across cache lines to 
 *     reduce false sharing */
#define GASNETE_SCATTER_EOPS_ACROSS_CACHELINES    1 

/* -------------------------------------------------------------------------- */
/* Extended threads support */
extern const gasnete_eopaddr_t	EOPADDR_NIL;

void            gasnete_begin_nbi_accessregion(
			int allowrecursion GASNETE_THREAD_FARG);
gasnet_handle_t gasnete_end_nbi_accessregion(GASNETE_THREAD_FARG_ALONE);

/* GM RDMA registration */
extern int gasnete_getrdma_enabled;

/* Extended reference functions */
gasnet_handle_t gasnete_extref_put_nb(gasnet_node_t node, void *dest, 
			void *src, size_t nbytes GASNETE_THREAD_FARG);
void gasnete_extref_put_nbi      (gasnet_node_t node, void *dest, void *src, 
			size_t nbytes GASNETE_THREAD_FARG);
gasnet_handle_t gasnete_extref_memset_nb   (gasnet_node_t node, void *dest, 
			int val, size_t nbytes GASNETE_THREAD_FARG);
void gasnete_extref_memset_nbi   (gasnet_node_t node, void *dest, int val, 
			size_t nbytes GASNETE_THREAD_FARG);
#if 0 /* UNUSED */
gasnet_handle_t gasnete_extref_get_nb_bulk(void *dest, gasnet_node_t node, 
			void *src, size_t nbytes GASNETE_THREAD_FARG);
gasnet_handle_t gasnete_extref_put_nb_bulk (gasnet_node_t node, void *dest, 
			void *src, size_t nbytes GASNETE_THREAD_FARG);
void gasnete_extref_get_nbi_bulk (void *dest, gasnet_node_t node, void *src, 
			size_t nbytes GASNETE_THREAD_FARG);
void gasnete_extref_put_nbi_bulk (gasnet_node_t node, void *dest, void *src, 
			size_t nbytes GASNETE_THREAD_FARG);
#endif

/* Additional support for core AMReplyLongAsync */
#if PLATFORM_ARCH_32
#define LONGASYNC_REP(cnt32, cnt64, args) \
	gasnetc_AMReplyLongAsync ## cnt32 args
#elif PLATFORM_ARCH_64
#define LONGASYNC_REP(cnt32, cnt64, args) \
	gasnetc_AMReplyLongAsync ## cnt64 args
#endif

#define GASNETE_HANDLER_BASE  64 /* reserve 64-127 for the extended API */
#define _hidx_gasnete_amdbarrier_notify_reqh (GASNETE_HANDLER_BASE+0) 
#define _hidx_gasnete_amcbarrier_notify_reqh (GASNETE_HANDLER_BASE+1) 
#define _hidx_gasnete_amcbarrier_done_reqh   (GASNETE_HANDLER_BASE+2)
#if 0 /* UNUSED: No AM-based Gets in gm-conduit */
#define _hidx_gasnete_extref_get_reqh        (GASNETE_HANDLER_BASE+3)
#define _hidx_gasnete_extref_get_reph        (GASNETE_HANDLER_BASE+4)
#define _hidx_gasnete_extref_getlong_reqh    (GASNETE_HANDLER_BASE+5)
#define _hidx_gasnete_extref_getlong_reph    (GASNETE_HANDLER_BASE+6)
#endif
#define _hidx_gasnete_extref_put_reqh        (GASNETE_HANDLER_BASE+7)
#define _hidx_gasnete_extref_putlong_reqh    (GASNETE_HANDLER_BASE+8)
#define _hidx_gasnete_extref_memset_reqh     (GASNETE_HANDLER_BASE+9)
#define _hidx_gasnete_extref_markdone_reph   (GASNETE_HANDLER_BASE+10)

#define _hidx_gasnete_get_dma_reqh           (GASNETE_HANDLER_BASE+11)
#define _hidx_gasnete_get_dma_reph           (GASNETE_HANDLER_BASE+12)

#endif
