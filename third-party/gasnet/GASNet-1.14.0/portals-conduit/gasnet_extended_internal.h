/*   $Source: /var/local/cvs/gasnet/portals-conduit/gasnet_extended_internal.h,v $
 *     $Date: 2008/12/26 05:31:06 $
 * $Revision: 1.7 $
 * Description: GASNet header for internal definitions in Extended API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_EXTENDED_INTERNAL_H
#define _GASNET_EXTENDED_INTERNAL_H

#include <gasnet_internal.h>
#include <gasnet_handler.h>

#ifndef GASNETE_USEAM
/* MLW: debug: use AMs for extended ref implementation
 * THIS SHOULD BE 0 by default!
 */
#define GASNETE_USEAM 0
#endif

/* ------------------------------------------------------------------------------------ */

/* for compactness, eops and iops address each other in the free list using a gasnete_opaddr_t */ 
typedef union _gasnete_opaddr_t {
  struct {
    uint8_t _bufferidx;
    uint8_t _opidx;
  } compaddr;
  uint16_t fulladdr;
} gasnete_opaddr_t;
#define bufferidx compaddr._bufferidx
#define opidx compaddr._opidx

/* gasnet_handle_t is a void* pointer to a gasnete_op_t, 
 * which is either a gasnete_eop_t or an gasnete_iop_t
 * In the portals conduit, we require that both eops and iops can be accessed
 * by a compact (24-bit) representation [threadidx,addr].  
 * This representation will be packed into a substring of the match bits of a
 * Portals Put/Get operation.  When we extract the compact representation,
 * we must be able to determine:
 * (1) if it references an eop or iop.  Check the most significant bit if threadidx.
 * (2) recover an actual pointer to the eop/iop.  This is done by referencing
 *         eop_bufs[_bufferidx] + _opidx 
 *     or  iop_bufs[_bufferidx] + _opidx
 * from the threads threaddata structure.
 */
typedef struct _gasnete_op_t {
  uint8_t flags;                  /*  flags - type tag */
  gasnete_threadidx_t threadidx;  /*  Leading bit indicate eop/iop, 7 lower bits are thread that owns me */
  gasnete_opaddr_t addr;          /*  next cell while in free list, my own opaddr_t while in use */
} gasnete_op_t;

const gasnete_opaddr_t gasnete_opaddr_nil;
#define gasnete_opaddr_equal(addr1,addr2) ((addr1).fulladdr == (addr2).fulladdr)
#define gasnete_opaddr_isnil(addr) ((addr).fulladdr == gasnete_opaddr_nil.fulladdr)

typedef struct _gasnete_eop_t {
  uint8_t flags;                  /*  state flags */
  gasnete_threadidx_t threadidx;  /*  thread that owns me */
  gasnete_opaddr_t addr;          /*  next cell while in free list, my own opaddr_t while in use */
} gasnete_eop_t;

typedef struct _gasnete_iop_t {
  uint8_t flags;                  /*  state flags */
  gasnete_threadidx_t threadidx;  /*  thread that owns me */
  gasnete_opaddr_t addr;          /*  my own opaddr_t for use as compact pointer */
  struct _gasnete_iop_t *next;    /*  used for free list and iop stack */
  int initiated_get_cnt;     /*  count of get ops initiated */
  int initiated_put_cnt;     /*  count of put ops initiated */

  /*  make sure the counters live on different cache lines for SMP's */
  uint8_t pad[MAX(8,(ssize_t)(GASNETI_CACHE_LINE_BYTES))]; 

  gasneti_weakatomic_t completed_get_cnt;     /*  count of get ops completed */
  gasneti_weakatomic_t completed_put_cnt;     /*  count of put ops completed */
} gasnete_iop_t;

/* ------------------------------------------------------------------------------------ */
typedef struct _gasnete_threaddata_t {
  GASNETE_COMMON_THREADDATA_FIELDS /* MUST come first, for reserved ptrs */

  gasnete_eop_t *eop_bufs[256]; /*  buffers of eops for memory management */
  int eop_num_bufs;             /*  number of valid eop buffer entries */
  gasnete_opaddr_t eop_free;    /*  free list of eops */
#if GASNETI_STATS_OR_TRACE
  int eop_inuse;                /*  count of number of eops currently in use */
  int eop_hwm;                  /*  the high water mark of allocated eops */
#endif

  gasnete_iop_t *iop_bufs[256]; /*  buffers of iops for memory management */
  int iop_num_bufs;             /*  number of valid iop buffer entries (generally just one) */
  gasnete_iop_t *iop_free;      /*  free list of iops */

  /*  stack of iops - head is active iop servicing new implicit ops */
  gasnete_iop_t *current_iop;  

  /* counter used by non-blocking non-bulk put ops to delay returning to caller before local
   * completion.  Initialized by calling thread, incremented and read by gasnete_put_nb*
   * operations (in some cases) decremented by event handler functions.
   * Since these will not execute in a signal handler, they are weakatomic.
   */
  gasneti_weakatomic_t local_completion_count;

} gasnete_threaddata_t;
extern gasnete_threaddata_t *gasnete_threadtable[];

/* ------------------------------------------------------------------------------------ */

/* gasnete_op_t flags field */
#define OPTYPE_EXPLICIT               0x00  /*  gasnete_eop_new() relies on this value */
#define OPTYPE_IMPLICIT               0x80
#define OPTYPE(op) ((op)->threadidx & 0x80)
GASNETI_INLINE(SET_OPTYPE)
void SET_OPTYPE(gasnete_op_t *op, uint8_t type) {
  op->threadidx = (op->threadidx & 0x7F) | (type & 0x80);
}
GASNETI_INLINE(SET_THREADID)
void SET_THREADID(gasnete_op_t *op, uint8_t threadid) {
  op->threadidx = (threadid & 0x7F) | (op->threadidx & 0x80);
}
GASNETI_INLINE(SET_EOP_THREADID)
void SET_EOP_THREADID(gasnete_eop_t *eop, uint8_t threadid) {
  eop->threadidx = (threadid & 0x7F) | OPTYPE_EXPLICIT;
}
GASNETI_INLINE(SET_IOP_THREADID)
void SET_IOP_THREADID(gasnete_iop_t *iop, uint8_t threadid) {
  iop->threadidx = (threadid & 0x7F) | OPTYPE_IMPLICIT;
}

#define GASNETE_THREADID(th) ((th) & 0x7F)
#define GASNETE_OP_THREADID(op) GASNETE_THREADID((op)->threadidx)
#define GASNETE_IS_EOP(op) (((op)->threadidx & 0x80) == OPTYPE_EXPLICIT)
#define GASNETE_IS_IOP(op) (((op)->threadidx & 0x80) == OPTYPE_IMPLICIT)

/*  state - only valid for explicit ops */
/*  MLW: iops also use state, but only OPSTATE_FREE or OPSTATE_INFLIGHT */
#define OPSTATE_FREE      0   /*  gasnete_eop_new() relies on this value */
#define OPSTATE_INFLIGHT  1
#define OPSTATE_COMPLETE  2
#define OPSTATE(op) ((op)->flags & 0x03) 
GASNETI_INLINE(SET_OPSTATE)
void SET_OPSTATE(gasnete_op_t *op, uint8_t state) {
  op->flags = (op->flags & 0xFC) | (state & 0x03);
  /* RACE: If we are marking the op COMPLETE, don't assert for completion
   * state as another thread spinning on the op may already have changed
   * the state. */
  gasneti_assert(state == OPSTATE_COMPLETE ? 1 : OPSTATE(op) == state);
}

/*  get a new op and mark it in flight */
gasnete_eop_t *gasnete_eop_new(gasnete_threaddata_t *thread);
gasnete_iop_t *gasnete_iop_new(gasnete_threaddata_t *thread);
/*  query an eop for completeness */
int gasnete_op_isdone(gasnete_op_t *op);
/*  mark an op done - isget ignored for explicit ops */
void gasnete_op_markdone(gasnete_op_t *op, int isget);
/*  free an op */
void gasnete_op_free(gasnete_op_t *op);


#define GASNETE_EOPADDR_TO_PTR(threaddata, opaddr)                      \
      (gasneti_memcheck(threaddata),                                    \
       gasneti_assert(!gasnete_opaddr_isnil(opaddr)),                   \
       gasneti_assert((opaddr).bufferidx < (threaddata)->eop_num_bufs), \
       gasneti_memcheck((threaddata)->eop_bufs[(opaddr).bufferidx]),    \
       (threaddata)->eop_bufs[(opaddr).bufferidx] + (opaddr).opidx)

#define GASNETE_IOPADDR_TO_PTR(threaddata, opaddr)                      \
      (gasneti_memcheck(threaddata),                                    \
       gasneti_assert(!gasnete_opaddr_isnil(opaddr)),                   \
       gasneti_assert((opaddr).bufferidx < (threaddata)->iop_num_bufs), \
       gasneti_memcheck((threaddata)->iop_bufs[(opaddr).bufferidx]),    \
       (threaddata)->iop_bufs[(opaddr).bufferidx] + (opaddr).opidx)


GASNETI_INLINE(gasnete_opaddr_to_ptr)
gasnete_op_t *gasnete_opaddr_to_ptr(gasnete_threadidx_t threadid, gasnete_opaddr_t opaddr)
{
  gasnete_threaddata_t *th = gasnete_threadtable[GASNETE_THREADID(threadid)];
  return ((threadid & OPTYPE_IMPLICIT)  == OPTYPE_IMPLICIT
	  ? (gasnete_op_t*)(GASNETE_IOPADDR_TO_PTR(th,opaddr))
	  : (gasnete_op_t*)(GASNETE_EOPADDR_TO_PTR(th,opaddr))
	  );
}


#if GASNET_DEBUG
  /* check an in-flight/complete eop */
  #define gasnete_eop_check(eop) do {                                \
    gasnete_threaddata_t * _th;                                      \
    gasneti_assert(OPTYPE(eop) == OPTYPE_EXPLICIT);                  \
    gasneti_assert(OPSTATE(eop) == OPSTATE_INFLIGHT ||               \
                   OPSTATE(eop) == OPSTATE_COMPLETE);                \
    gasnete_assert_valid_threadid(GASNETE_OP_THREADID(eop));         \
    _th = gasnete_threadtable[GASNETE_OP_THREADID(eop)];             \
    gasneti_assert(GASNETE_EOPADDR_TO_PTR(_th, (eop)->addr) == eop); \
  } while (0)
  #define gasnete_iop_check(iop) do {                         \
    int _temp;                                                \
    gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);           \
    gasneti_assert(OPSTATE(iop) == OPSTATE_INFLIGHT);         \
    gasnete_assert_valid_threadid(GASNETE_OP_THREADID(iop));  \
    _temp = gasneti_weakatomic_read(&((iop)->completed_put_cnt), 0); \
    if (_temp <= 65000) /* prevent race condition on reset */ \
      gasneti_assert((iop)->initiated_put_cnt >= _temp);      \
    _temp = gasneti_weakatomic_read(&((iop)->completed_get_cnt), 0); \
    if (_temp <= 65000) /* prevent race condition on reset */ \
      gasneti_assert((iop)->initiated_get_cnt >= _temp);      \
  } while (0)
  extern void _gasnete_iop_check(gasnete_iop_t *iop);
#else
  #define gasnete_eop_check(eop)   ((void)0)
  #define gasnete_iop_check(iop)   ((void)0)
#endif

/*  1 = scatter newly allocated eops across cache lines to reduce false sharing */
#define GASNETE_SCATTER_EOPS_ACROSS_CACHELINES    1 


/* ------------------------------------------------------------------------------------ */

#define GASNETE_HANDLER_BASE  64 /* reserve 64-127 for the extended API */
#if GASNETE_USEAM
#define _hidx_gasnete_amdbarrier_notify_reqh (GASNETE_HANDLER_BASE+0) 
#define _hidx_gasnete_amcbarrier_notify_reqh (GASNETE_HANDLER_BASE+1) 
#define _hidx_gasnete_amcbarrier_done_reqh   (GASNETE_HANDLER_BASE+2)
#define _hidx_gasnete_get_reqh               (GASNETE_HANDLER_BASE+3)
#define _hidx_gasnete_get_reph               (GASNETE_HANDLER_BASE+4)
#define _hidx_gasnete_getlong_reqh           (GASNETE_HANDLER_BASE+5)
#define _hidx_gasnete_getlong_reph           (GASNETE_HANDLER_BASE+6)
#define _hidx_gasnete_put_reqh               (GASNETE_HANDLER_BASE+7)
#define _hidx_gasnete_putlong_reqh           (GASNETE_HANDLER_BASE+8)
#define _hidx_gasnete_memset_reqh            (GASNETE_HANDLER_BASE+9)
#define _hidx_gasnete_markdone_reph          (GASNETE_HANDLER_BASE+10)
#else
#define _hidx_gasnete_amdbarrier_notify_reqh (GASNETE_HANDLER_BASE+0) 
#define _hidx_gasnete_amcbarrier_notify_reqh (GASNETE_HANDLER_BASE+1) 
#define _hidx_gasnete_amcbarrier_done_reqh   (GASNETE_HANDLER_BASE+2)
#define _hidx_gasnete_memset_reqh            (GASNETE_HANDLER_BASE+3)
#define _hidx_gasnete_markdone_reph          (GASNETE_HANDLER_BASE+4)
#endif
/* add new extended API handlers here and to the bottom of gasnet_extended.c */

#endif
