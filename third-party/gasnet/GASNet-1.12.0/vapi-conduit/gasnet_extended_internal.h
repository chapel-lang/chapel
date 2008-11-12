/*   $Source: /var/local/cvs/gasnet/vapi-conduit/gasnet_extended_internal.h,v $
 *     $Date: 2008/10/24 22:20:17 $
 * $Revision: 1.23 $
 * Description: GASNet header for internal definitions in Extended API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_EXTENDED_INTERNAL_H
#define _GASNET_EXTENDED_INTERNAL_H

#include <gasnet_internal.h>
#include <gasnet_handler.h>

/* ------------------------------------------------------------------------------------ */

enum {
  gasnete_opExplicit = 0,	/* gasnete_eop_new() relies on this value */
  gasnete_opImplicit
};

/* gasnet_handle_t is a void* pointer to a gasnete_op_t, 
   which is either a gasnete_eop_t or an gasnete_iop_t
   */
typedef struct _gasnete_op_t {
  uint8_t type;                   /*  type tag */
  gasnete_threadidx_t threadidx;  /*  thread that owns me */
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

typedef struct _gasnete_eop_t {
  uint8_t type;                   /*  type tag */
  gasnete_threadidx_t threadidx;  /*  thread that owns me */
  gasnete_eopaddr_t addr;         /*  next cell while in free list, my own eopaddr_t while in use */
  gasnetc_counter_t req_oust;
} gasnete_eop_t;

typedef struct _gasnete_iop_t {
  uint8_t type;                   /*  type tag */
  gasnete_threadidx_t threadidx;  /*  thread that owns me */
  uint16_t _unused;

  struct _gasnete_iop_t *next;    /*  next cell while in free list */

  /*  make sure the counters live on a distinct cache line for SMPs */
  uint8_t _pad1[MAX(8,(ssize_t)(GASNETI_CACHE_LINE_BYTES - 2*sizeof(void *)))];
  gasnetc_counter_t get_req_oust;     /*  count of get ops outstanding */
  gasnetc_counter_t put_req_oust;     /*  count of put ops outstanding */
  uint8_t _pad2[MAX(8,(ssize_t)(GASNETI_CACHE_LINE_BYTES - 2*sizeof(gasnetc_counter_t)))];
} gasnete_iop_t;

/* ------------------------------------------------------------------------------------ */
typedef struct _gasnete_threaddata_t {
  void *gasnetc_threaddata;     /* pointer reserved for use by the core */
  void *gasnete_coll_threaddata;/* pointer reserved for use by the collectives */
  void *gasnete_vis_threaddata; /* pointer reserved for use by the VIS implementation */

  gasnete_threadidx_t threadidx;

  gasnete_eop_t *eop_bufs[256]; /*  buffers of eops for memory management */
  int eop_num_bufs;             /*  number of valid buffer entries */
  gasnete_eopaddr_t eop_free;   /*  free list of eops */

  gasnete_iop_t *current_iop;   /* active iop servicing new implicit ops */
  gasnete_iop_t *default_iop;   /* iop used when no access region is active */

  gasnete_iop_t *iop_free;      /*  free list of iops */

  struct _gasnet_valget_op_t *valget_free; /* free list of valget cells */
} gasnete_threaddata_t;
/* ------------------------------------------------------------------------------------ */

/*  get a new op */
gasnete_eop_t *gasnete_eop_new(gasnete_threaddata_t *thread);
gasnete_iop_t *gasnete_iop_new(gasnete_threaddata_t *thread);

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
    gasneti_assert((eop)->type == gasnete_opExplicit);               \
    _th = gasnete_threadtable[(eop)->threadidx];                     \
    gasneti_assert(GASNETE_EOPADDR_TO_PTR(_th, (eop)->addr) == eop); \
  } while (0)
  #define gasnete_iop_check(iop) do {                         \
    gasnete_iop_t *_tmp_next;                                 \
    gasneti_memcheck(iop);                                    \
    _tmp_next = (iop)->next;                                  \
    if (_tmp_next != NULL) _gasnete_iop_check(_tmp_next);     \
    gasneti_assert((iop)->type == gasnete_opImplicit);        \
    gasneti_assert((iop)->threadidx < gasnete_numthreads);    \
    gasneti_memcheck(gasnete_threadtable[(iop)->threadidx]);  \
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
#define _hidx_gasnete_amdbarrier_notify_reqh (GASNETE_HANDLER_BASE+0) 
#define _hidx_gasnete_amcbarrier_notify_reqh (GASNETE_HANDLER_BASE+1) 
#define _hidx_gasnete_amcbarrier_done_reqh   (GASNETE_HANDLER_BASE+2)
#define _hidx_gasnete_done_reph              (GASNETE_HANDLER_BASE+3)
#define _hidx_gasnete_memset_reqh            (GASNETE_HANDLER_BASE+4)
/* add new extended API handlers here and to the bottom of gasnet_extended.c */

#endif
