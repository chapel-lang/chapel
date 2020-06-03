/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/vis/gasnet_vis_internal.h $
 * Description: Internal definitions for GASNet Vector, Indexed & Strided implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_VIS_INTERNAL_H
#define _GASNET_VIS_INTERNAL_H

#define GASNETI_NEED_GASNET_VIS_H 1
#include <gasnet_internal.h>

/*---------------------------------------------------------------------------------*/
/* ***  VIS state *** */
/*---------------------------------------------------------------------------------*/
/* represents a VIS operation in flight */
typedef struct gasneti_vis_op_S {
  struct gasneti_vis_op_S *next;
  uint8_t type;
  void *addr;
  gasneti_eop_t *eop;
  gasneti_iop_t *iop;
  gasneti_weakatomic_t packetcnt;
  size_t count;
  size_t len;
  gex_Event_t event;
} gasneti_vis_op_t;

#define SMD_SELF  0
#define SMD_PEER 1
// gasneti_vis_smd_dim_t represents the meta data parameters for a particular dimension
typedef struct {
  size_t    count;     // dimensional extent
  ptrdiff_t stride[2]; // dimensional stride in bytes, [0]=self [1]=peer
} gasneti_vis_smd_dim_t;

// gasneti_vis_smd_t represents complete information about a strided transfer
// in a format convenient for applying transformations
typedef struct {
  // -----------------------------------------------------------------------
  // post-analysis stats:
  #if GASNET_DEBUG
    int have_stats;             // true iff the fields in this section are valid
  #endif
  size_t totalsz;               // the total bytes of data in the transfer
  size_t elemcnt;               // number of elements in the transfer, aka dual-lcontig_segments
                                // Note that post-optimization the following properties hold:
                                //   dual-lcontig_sz == elemsz
                                //   dual-lcontig_dims == 0
  size_t lcontig_dims[2];       // highest stridelevel with linear contiguity in this region
                                // eg. zero if only the bottom level is linear contiguous,
                                // and stridelevels if the entire region is linear contiguous
  size_t lcontig_sz[2];         // size of the linear contiguous segments in this region
  size_t lcontig_segments[2];   // number of linear contiguous segments in this region
  // -----------------------------------------------------------------------
  // normative metadata:
  size_t stridelevels;          // dimensional cardinality
  size_t elemsz;                // dual-lcontig_sz (post-optimization)
  void  *addr[2];               // base addresses [0]=self [1]=peer
  gasneti_vis_smd_dim_t dim[1]; // per-dimension metadata,
                                // actually [stridelevels] entries (flexible array member)
  // DO NOT PUT ANYTHING HERE
} gasneti_vis_smd_t;

// gasneti_strided_op_t "is a" gasneti_vis_op_t that represents a strided operation in flight
// the embedded metadata may have been optimized/transformed relative to user's input
typedef struct {
  gasneti_vis_op_t visop; // must be first
  void *bouncebuf;        // separate subobject to free on destruction, otherwise NULL
  void *scratch;          // scratch space at the end of this object
  gasneti_vis_smd_t smd;  // variable-length strided metadata, must be last!
  // DO NOT PUT ANYTHING HERE
} gasneti_strided_op_t;

/* per-EP state for VIS */
typedef struct {
  gasneti_mutex_t _poll_lock;
  gasneti_vis_op_t _active_ops;
} gasnete_vis_epdata_t;
extern gasnete_vis_epdata_t gasnete_vis_epdata_THUNK;

/* per-thread state for VIS */
typedef struct {
  gasnete_vis_pcinfo_t pcinfo; // must come first for use in public header

  // visop management
  // TODO-EX: Rework this
  gasneti_vis_op_t *active_ops;
  gasneti_vis_op_t *free_ops;
  int progressfn_active;
  #ifdef GASNETE_VIS_THREADDATA_EXTRA
    GASNETE_VIS_THREADDATA_EXTRA
  #endif
} gasnete_vis_threaddata_t;

static void gasnete_vis_cleanup_threaddata(void *_td) {
  gasnete_vis_threaddata_t *td = (gasnete_vis_threaddata_t *)_td;
  gasneti_vis_op_t *op;
  #ifdef GASNETE_VIS_THREADDATA_EXTRA_CLEANUP
    GASNETE_VIS_THREADDATA_EXTRA_CLEANUP(td);
  #endif
  gasneti_assert(td->active_ops == NULL);
  while ((op = td->free_ops) != NULL) {
    td->free_ops = op->next;
    gasneti_free(op);
  }
  gasneti_free(td);
}

GASNETI_INLINE(gasnete_vis_new_threaddata) GASNETI_MALLOC
gasnete_vis_threaddata_t *gasnete_vis_new_threaddata(void) {
  gasnete_vis_threaddata_t *result = gasneti_calloc(1,sizeof(*result));
  #ifdef GASNETE_VIS_THREADDATA_EXTRA_INIT
    GASNETE_VIS_THREADDATA_EXTRA_INIT(result);
  #endif
  gasnete_register_threadcleanup(gasnete_vis_cleanup_threaddata, result);
  return result;
}

#define _GASNETE_VIS_MYTHREAD(mythread)     \
        (mythread->gasnete_vis_threaddata ? \
         mythread->gasnete_vis_threaddata : \
        (mythread->gasnete_vis_threaddata = gasnete_vis_new_threaddata()))
#define GASNETE_VIS_MYTHREAD        _GASNETE_VIS_MYTHREAD(GASNETI_MYTHREAD)

#define GASNETI_VIS_CAT_PUTV_GATHER       1
#define GASNETI_VIS_CAT_GETV_SCATTER      2
#define GASNETI_VIS_CAT_PUTI_GATHER       3
#define GASNETI_VIS_CAT_GETI_SCATTER      4
#define GASNETI_VIS_CAT_PUTS_GATHER       5
#define GASNETI_VIS_CAT_GETS_SCATTER      6
#define GASNETI_VIS_CAT_PUTV_AMPIPELINE   7
#define GASNETI_VIS_CAT_GETV_AMPIPELINE   8
#define GASNETI_VIS_CAT_PUTI_AMPIPELINE   9
#define GASNETI_VIS_CAT_GETI_AMPIPELINE   10
#define GASNETI_VIS_CAT_PUTS_AMPIPELINE   11
#define GASNETI_VIS_CAT_GETS_AMPIPELINE   12
#define GASNETI_VIS_CAT_PUTPC_CHAIN       13

/*---------------------------------------------------------------------------------*/
/* VISOP manipulation */

/* create a dummy eop/iop based on synctype, save it in visop */
#define GASNETE_VISOP_SETUP(visop, synctype, isget) do {              \
    if (synctype == gasnete_synctype_nbi) {                           \
      visop->eop = NULL;                                              \
      visop->iop = gasneti_iop_register(1,isget GASNETI_THREAD_PASS); \
    } else {                                                          \
      visop->eop = gasneti_eop_create(GASNETI_THREAD_PASS_ALONE);     \
      visop->iop = NULL;                                              \
    }                                                                 \
} while (0)

/* Must not reference visop, which may no longer exist */
#define GASNETE_VISOP_RETURN_VOLATILE(eop, synctype) do {            \
    switch (synctype) {                                              \
      case gasnete_synctype_b: {                                     \
        gex_Event_t h = gasneti_eop_to_event(eop);                   \
        gasnete_wait(h GASNETI_THREAD_PASS);                         \
        return GEX_EVENT_INVALID;                                    \
      }                                                              \
      case gasnete_synctype_nb:                                      \
        return gasneti_eop_to_event(eop);                            \
      case gasnete_synctype_nbi:                                     \
        return GEX_EVENT_INVALID;                                    \
      default: gasneti_unreachable_error(("bad synctype: 0x%x",(int)(synctype))); \
        return GEX_EVENT_INVALID; /* avoid warning on MIPSPro */     \
    }                                                                \
} while (0)

#define GASNETE_VISOP_RETURN(visop, synctype) \
    GASNETE_VISOP_RETURN_VOLATILE(visop->eop, synctype)

/* signal a visop dummy eop/iop */
#define GASNETE_VISOP_SIGNAL(visop, isget) do {       \
    gasneti_assert(visop->eop || visop->iop);         \
    if (visop->eop) gasneti_eop_markdone(visop->eop); \
    else gasneti_iop_markdone(visop->iop, 1, isget);  \
  } while (0)

// ops with a single completion event
#define GASNETE_START_ONEOP(op, handle, synctype, isget) do {         \
    if (synctype == gasnete_synctype_nbi) {                           \
      op = (void *)gasneti_iop_register(1,isget GASNETI_THREAD_PASS); \
      handle = GEX_EVENT_INVALID;                                     \
    } else {                                                          \
      op = (void *)gasneti_eop_create(GASNETI_THREAD_PASS_ALONE);     \
      handle = gasneti_eop_to_event((gasneti_eop_t *)op);             \
    }                                                                 \
  } while (0)
#define GASNETE_RETURN_ONEOP(handle, synctype) do {                   \
    if_pf (synctype == gasnete_synctype_b) {                          \
      gasnete_wait(handle GASNETI_THREAD_PASS);                       \
      return GEX_EVENT_INVALID;                                       \
    } else return handle;                                             \
  } while (0)
      

/* do GASNETE_VISOP_SETUP, push the visop on the thread-specific list 
   and do GASNETE_VISOP_RETURN */
#define GASNETE_PUSH_VISOP_RETURN(td, visop, synctype, isget, extra) do {   \
    GASNETE_VISOP_SETUP(visop, synctype, isget);                     \
    GASNETI_PROGRESSFNS_ENABLE(gasneti_pf_vis,COUNTED);              \
    visop->next = td->active_ops; /* push on thread-specific list */ \
    td->active_ops = visop;                                          \
    extra;                                                           \
    GASNETE_VISOP_RETURN(visop, synctype);                           \
} while (0)
/*---------------------------------------------------------------------------------*/
/* ***  Individual put/get helpers *** */
/*---------------------------------------------------------------------------------*/
/* helper for vis functions implemented atop other GASNet operations
   start a recursive NBI access region, if appropriate */
#define GASNETE_START_NBIREGION(synctype) do {               \
  if (synctype != gasnete_synctype_nbi)                      \
    gasnete_begin_nbi_accessregion(0,1 GASNETI_THREAD_PASS); \
  } while(0)
/* finish a region started with GASNETE_START_NBIREGION,
   block if required, and return the appropriate event */
#define GASNETE_END_NBIREGION_AND_RETURN(synctype) do {                               \
    switch (synctype) {                                                               \
      case gasnete_synctype_nb:                                                       \
        return gasnete_end_nbi_accessregion(0 GASNETI_THREAD_PASS);                   \
      case gasnete_synctype_b:                                                        \
        gasnete_wait(gasnete_end_nbi_accessregion(0 GASNETI_THREAD_PASS) GASNETI_THREAD_PASS); \
        return GEX_EVENT_INVALID;                                                     \
      case gasnete_synctype_nbi:                                                      \
        return GEX_EVENT_INVALID;                                                     \
      default: gasneti_unreachable_error(("bad synctype: 0x%x",(int)synctype));       \
        return GEX_EVENT_INVALID; /* avoid warning on MIPSPro */                      \
    }                                                                                 \
  } while(0)

#define GASNETE_PUT_INDIV(tm, rank, dstaddr, srcaddr, nbytes, lc_opt) do { \
    gasneti_assert((nbytes) > 0);                                       \
    gasneti_boundscheck_allowoutseg((tm), (rank), (dstaddr), (nbytes)); \
    gasnete_put_nbi((tm), (rank), (dstaddr), (srcaddr), (nbytes),       \
                         (lc_opt), 0 GASNETI_THREAD_PASS);              \
  } while (0)

#define GASNETE_GET_INDIV(tm, rank, dstaddr, srcaddr, nbytes) do {      \
    gasneti_assert((nbytes) > 0);                                       \
    gasneti_boundscheck_allowoutseg((tm), (rank), (srcaddr), (nbytes)); \
    gasnete_get_nbi((tm), (dstaddr), (rank), (srcaddr), (nbytes),       \
                         0 GASNETI_THREAD_PASS);                        \
  } while (0)

// Put/get for degenerate case, where this single op represents the entire operation
// Casts from int -> gex_Event_t in NBI/Blocking cases are valid because they only
// care about zero versus non-zero.
// NOTE: cannot use gasnete_* variants here, as they are currently non-functional on smp/nopshm

#define GASNETE_PUT_DEGEN(retval, synctype, tm, rank, dstaddr, srcaddr, nbytes, flags) do { \
    gasneti_assert((nbytes) > 0);                                                   \
    gasneti_boundscheck_allowoutseg((tm), (rank), (dstaddr), (nbytes));             \
    switch (synctype) {                                                             \
      case gasnete_synctype_nb: {                                                   \
        gex_Event_t _lc_dummy;                                                      \
        (retval) = _gex_RMA_PutNB ((tm), (rank), (dstaddr), (srcaddr), (nbytes),    \
                    (((flags) & GEX_FLAG_ENABLE_LEAF_LC) ? &_lc_dummy : GEX_EVENT_DEFER), \
                    (flags) GASNETI_THREAD_PASS);                                   \
        break; }                                                                    \
      case gasnete_synctype_nbi:                                                    \
        (retval) = (gex_Event_t)(intptr_t)                                          \
                   _gex_RMA_PutNBI((tm), (rank), (dstaddr), (srcaddr), (nbytes),    \
                    (((flags) & GEX_FLAG_ENABLE_LEAF_LC) ? GEX_EVENT_GROUP : GEX_EVENT_DEFER),\
                    (flags) GASNETI_THREAD_PASS);                                   \
        break;                                                                      \
      case gasnete_synctype_b:                                                      \
        (retval) = (gex_Event_t)(intptr_t)                                          \
              _gex_RMA_PutBlocking((tm), (rank), (dstaddr), (srcaddr), (nbytes),    \
                                          (flags) GASNETI_THREAD_PASS);             \
        break;                                                                      \
      default: gasneti_unreachable_error(("Invalid synctype=%i",(int)(synctype)));  \
    }                                                                               \
  } while (0)

#define GASNETE_GET_DEGEN(retval, synctype, tm, dstaddr, rank, srcaddr, nbytes, flags) do { \
    gasneti_assert((nbytes) > 0);                                                   \
    gasneti_boundscheck_allowoutseg((tm), (rank), (srcaddr), (nbytes));             \
    switch (synctype) {                                                             \
      case gasnete_synctype_nb:                                                     \
        (retval) = _gex_RMA_GetNB ((tm), (dstaddr), (rank), (srcaddr), (nbytes),    \
                                          (flags) GASNETI_THREAD_PASS);             \
        break;                                                                      \
      case gasnete_synctype_nbi:                                                    \
        (retval) = (gex_Event_t)(intptr_t)                                          \
                   _gex_RMA_GetNBI((tm), (dstaddr), (rank), (srcaddr), (nbytes),    \
                                          (flags) GASNETI_THREAD_PASS);             \
        break;                                                                      \
      case gasnete_synctype_b:                                                      \
        (retval) = (gex_Event_t)(intptr_t)                                          \
              _gex_RMA_GetBlocking((tm), (dstaddr), (rank), (srcaddr), (nbytes),    \
                                          (flags) GASNETI_THREAD_PASS);             \
        break;                                                                      \
      default: gasneti_unreachable_error(("Invalid synctype=%i",(int)(synctype)));  \
    }                                                                               \
  } while (0)

/*---------------------------------------------------------------------------------*/
/* packing/unpacking helpers */
#define _GASNETE_PACK_HELPER(packed, unpacked, sz)   GASNETI_MEMCPY((packed), (unpacked), (sz))
#define _GASNETE_UNPACK_HELPER(packed, unpacked, sz) GASNETI_MEMCPY((unpacked), (packed), (sz))

// GASNETE_VIS_MIN_PACKBUFFER is used to round up packing buffers to a large enough size
// so that glibc malloc will hopefully give us memory with better access behavior, noticeably 
// improving throughput on systems like KNL.
#ifndef GASNETE_VIS_MIN_PACKBUFFER
#define GASNETE_VIS_MIN_PACKBUFFER 8192
#endif

GASNETI_INLINE(gasnete_visbuf_malloc) GASNETI_MALLOC
void *gasnete_visbuf_malloc(size_t _sz) {
  return gasneti_malloc(MAX(_sz,GASNETE_VIS_MIN_PACKBUFFER));
}

/*---------------------------------------------------------------------------------*/
/* packetization */
typedef struct {
  size_t firstidx;
  size_t firstoffset;
  size_t lastidx;
  size_t lastlen;
} gasnete_packetdesc_t;

extern void gasnete_packetize_verify(gasnete_packetdesc_t *pt, size_t ptidx, int lastpacket,
                              size_t count, size_t len, gex_Memvec_t const *list);

/*---------------------------------------------------------------------------------*/
// AM helpers
#if PLATFORM_ARCH_32
#define HARGS(c32,c64) c32
#else
#define HARGS(c32,c64) c64
#endif
// GASNETE_VIS_NPAM:
// 0 = Use FP AM
// 1 = Use NP AM with a fixed-payload-size algorithm
// 2 = Use NP AM with a negotiated-payload size
// Default is 1 for conduits with a "real" NP AM implementation, and 0 elsewhere
#ifndef GASNETE_VIS_NPAM
  #if GASNETC_HAVE_NP_REQ_MEDIUM && GASNETC_HAVE_NP_REP_MEDIUM
    #define GASNETE_VIS_NPAM 1
  #else
    #define GASNETE_VIS_NPAM 0
  #endif
#endif
#if !(GASNETE_VIS_NPAM == 0 || GASNETE_VIS_NPAM == 1 || GASNETE_VIS_NPAM == 2)
#error Incorrect GASNETE_VIS_NPAM definition - must be in {0,1,2}
#endif

/*---------------------------------------------------------------------------------*/

#if PLATFORM_COMPILER_SUN_C
  /* disable a harmless warning */
  #pragma error_messages(off, E_STATEMENT_NOT_REACHED)
#endif

#endif
