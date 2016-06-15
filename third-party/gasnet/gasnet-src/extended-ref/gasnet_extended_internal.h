/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/gasnet_extended_internal.h $
 * Description: GASNet header for internal definitions in Extended API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_EXTENDED_INTERNAL_H
#define _GASNET_EXTENDED_INTERNAL_H

#include <gasnet_internal.h>
#ifdef GASNETE_EXTENDED_NEEDS_CORE
#include <gasnet_core_internal.h>
#endif

/* ------------------------------------------------------------------------------------ */

#if defined(GASNETE_EOP_COUNTED)
#  ifdef GASNETE_EOP_BOOLEAN
#    error "Only one of GASNETE_EOP_COUNTED or GASNETE_EOP_BOOLEAN may be defined"
#  endif
#  undef GASNETE_EOP_COUNTED
#  define GASNETE_EOP_COUNTED 1
#  define GASNETE_EOP_BOOLEAN 0
#else
#  undef GASNETE_EOP_BOOLEAN
#  define GASNETE_EOP_BOOLEAN 1
#  define GASNETE_EOP_COUNTED 0
#endif

/* ------------------------------------------------------------------------------------ */

/* gasnet_handle_t is a void* pointer to a gasnete_op_t, 
   which is either a gasnete_eop_t or an gasnete_iop_t
   */
typedef struct _gasnete_op_t {
  uint8_t flags;                  /*  flags - type tag */
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
  uint8_t flags;                  /*  state flags */
  gasnete_threadidx_t threadidx;  /*  thread that owns me */
  gasnete_eopaddr_t addr;         /*  next cell while in free list, my own eopaddr_t while in use */
  #if GASNETE_EOP_COUNTED
  gasneti_weakatomic_val_t initiated_cnt;
  gasneti_weakatomic_t     completed_cnt;
  #endif
  #ifdef GASNETE_CONDUIT_EOP_FIELDS
  GASNETE_CONDUIT_EOP_FIELDS
  #endif
} gasnete_eop_t;

typedef struct _gasnete_iop_t {
  uint8_t flags;                  /*  state flags */
  gasnete_threadidx_t threadidx;  /*  thread that owns me */
  uint16_t _unused;
  gasneti_weakatomic_val_t initiated_get_cnt;     /*  count of get ops initiated */
  gasneti_weakatomic_val_t initiated_put_cnt;     /*  count of put ops initiated */

  struct _gasnete_iop_t *next;    /*  next cell while in free list, deferred iop while being filled */

  /*  make sure the initiated/completed counters live on different cache lines for SMP's */
  uint8_t pad[GASNETI_CACHE_PAD(sizeof(void*) + sizeof(gasneti_weakatomic_val_t))];

  gasneti_weakatomic_t completed_get_cnt;     /*  count of get ops completed */
  gasneti_weakatomic_t completed_put_cnt;     /*  count of put ops completed */

  #ifdef GASNETE_CONDUIT_IOP_FIELDS
  GASNETE_CONDUIT_IOP_FIELDS
  #endif
} gasnete_iop_t;

/* ------------------------------------------------------------------------------------ */
typedef struct _gasnete_threaddata_t {
  GASNETE_COMMON_THREADDATA_FIELDS /* MUST come first, for reserved ptrs */

  gasnete_eop_t *eop_bufs[256]; /*  buffers of eops for memory management */
  int eop_num_bufs;             /*  number of valid buffer entries */
  gasnete_eopaddr_t eop_free;   /*  free list of eops */

  /*  stack of iops - head is active iop servicing new implicit ops */
  gasnete_iop_t *current_iop;  

  gasnete_iop_t *iop_free;      /*  free list of iops */

  #ifdef GASNETE_CONDUIT_THREADDATA_FIELDS
  GASNETE_CONDUIT_THREADDATA_FIELDS
  #endif
} gasnete_threaddata_t;

/* ------------------------------------------------------------------------------------ */

/* gasnete_op_t flags field */
#define OPTYPE_EXPLICIT               0x00  /*  gasnete_eop_new() relies on this value */
#define OPTYPE_IMPLICIT               0x80
#define OPTYPE(op) ((op)->flags & 0x80)
GASNETI_INLINE(SET_OPTYPE)
void SET_OPTYPE(gasnete_op_t *op, uint8_t type) {
  op->flags = (op->flags & 0x7F) | (type & 0x80);
}

/*  state - only valid for explicit ops */
#define OPSTATE_FREE      0   /*  gasnete_eop_new() relies on this value */
#define OPSTATE_INFLIGHT  1
#define OPSTATE_COMPLETE  2
#define OPSTATE(op) ((op)->flags & 0x03) 
GASNETI_INLINE(SET_OPSTATE)
void SET_OPSTATE(gasnete_eop_t *op, uint8_t state) {
  op->flags = (op->flags & 0xFC) | (state & 0x03);
  /* RACE: If we are marking the op COMPLETE, don't assert for completion
   * state as another thread spinning on the op may already have changed
   * the state. */
  gasneti_assert(state == OPSTATE_COMPLETE ? 1 : OPSTATE(op) == state);
}

/* gasnete_op_t flag bits reserved for conduit-specific uses.
 * guaranteed not to conflict with use in extendef-ref and
 * are preserved by SET_OP{STATE,TYPE}() */
#define OPFLAG_CONDUIT0 0x04
#define OPFLAG_CONDUIT1 0x08
#define OPFLAG_CONDUIT2 0x10

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
    gasnete_iop_t *_tmp_next;                                 \
    gasneti_weakatomic_val_t _temp;                           \
    gasneti_memcheck(iop);                                    \
    _tmp_next = (iop)->next;                                  \
    if (_tmp_next != NULL) _gasnete_iop_check(_tmp_next);     \
    gasneti_assert(OPTYPE(iop) == OPTYPE_IMPLICIT);           \
    gasnete_assert_valid_threadid((iop)->threadidx);          \
    _temp = gasneti_weakatomic_read(&((iop)->completed_put_cnt), GASNETI_ATOMIC_RMB_POST); \
    gasneti_assert((((iop)->initiated_put_cnt - _temp) & GASNETI_ATOMIC_MAX) < (GASNETI_ATOMIC_MAX/2)); \
    _temp = gasneti_weakatomic_read(&((iop)->completed_get_cnt), GASNETI_ATOMIC_RMB_POST); \
    gasneti_assert((((iop)->initiated_get_cnt - _temp) & GASNETI_ATOMIC_MAX) < (GASNETI_ATOMIC_MAX/2)); \
  } while (0)
  extern void _gasnete_iop_check(gasnete_iop_t *iop);
#else
  #define gasnete_eop_check(eop)   ((void)0)
  #define gasnete_iop_check(iop)   ((void)0)
#endif

#ifndef GASNETE_IOP_CNTDONE
#define GASNETE_IOP_CNTDONE(_iop, _putget) \
  (gasneti_weakatomic_read(&(_iop)->completed_##_putget##_cnt, 0) \
          == ((_iop)->initiated_##_putget##_cnt & GASNETI_ATOMIC_MAX))
#endif

#if GASNETE_EOP_COUNTED
  #define GASNETE_EOP_DONE(_eop) \
    (gasneti_weakatomic_read(&(_eop)->completed_cnt, 0) \
          == ((_eop)->initiated_cnt & GASNETI_ATOMIC_MAX))
  #define GASNETE_EOP_MARKDONE(_eop) do {                        \
      gasneti_assert(!GASNETE_EOP_DONE(_eop));                   \
      gasneti_weakatomic_increment(&((_eop)->completed_cnt), 0); \
    } while (0)
#else
  #define GASNETE_EOP_DONE(_eop) (OPSTATE(_eop) == OPSTATE_COMPLETE)
  #define GASNETE_EOP_MARKDONE(_eop) do {      \
      gasneti_assert(!GASNETE_EOP_DONE(_eop)); \
      SET_OPSTATE((_eop), OPSTATE_COMPLETE);   \
    } while (0)
#endif

/*  1 = scatter newly allocated eops across cache lines to reduce false sharing */
#define GASNETE_SCATTER_EOPS_ACROSS_CACHELINES    1 

/* ------------------------------------------------------------------------------------ */
/* called at startup to check configuration sanity if using any portion of AMRef */
extern void gasnete_check_config_amref(void);

/* ------------------------------------------------------------------------------------ */

#define GASNETE_HANDLER_BASE  64 /* reserve 64-127 for the extended API */
#define _hidx_gasnete_amdbarrier_notify_reqh (GASNETE_HANDLER_BASE+0) 
#define _hidx_gasnete_amcbarrier_notify_reqh (GASNETE_HANDLER_BASE+1) 
#define _hidx_gasnete_amcbarrier_done_reqh   (GASNETE_HANDLER_BASE+2)
#define _hidx_gasnete_amref_get_reqh         (GASNETE_HANDLER_BASE+3)
#define _hidx_gasnete_amref_get_reph         (GASNETE_HANDLER_BASE+4)
#define _hidx_gasnete_amref_getlong_reqh     (GASNETE_HANDLER_BASE+5)
#define _hidx_gasnete_amref_getlong_reph     (GASNETE_HANDLER_BASE+6)
#define _hidx_gasnete_amref_put_reqh         (GASNETE_HANDLER_BASE+7)
#define _hidx_gasnete_amref_putlong_reqh     (GASNETE_HANDLER_BASE+8)
#define _hidx_gasnete_amref_memset_reqh      (GASNETE_HANDLER_BASE+9)
#define _hidx_gasnete_amref_markdone_reph    (GASNETE_HANDLER_BASE+10)
/* add new extended API handlers here and to the bottom of gasnet_extended.c */

#endif
