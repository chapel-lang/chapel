/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_vis_internal.h,v $
 *     $Date: 2009/05/12 02:16:38 $
 * $Revision: 1.20 $
 * Description: Internal definitions for GASNet Vector, Indexed & Strided implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_VIS_INTERNAL_H
#define _GASNET_VIS_INTERNAL_H

#include <gasnet_internal.h>
#include <gasnet_vis.h>

/*---------------------------------------------------------------------------------*/
/* ***  VIS state *** */
/*---------------------------------------------------------------------------------*/
/* represents a VIS operation in flight */
typedef struct gasneti_vis_op_S {
  struct gasneti_vis_op_S *next;
  uint8_t type;
  void *addr;
  #if GASNETI_HAVE_EOP_INTERFACE
    gasneti_eop_t *eop;
    gasneti_iop_t *iop;
  #endif
  gasneti_weakatomic_t packetcnt;
  size_t count;
  size_t len;
  gasnet_handle_t handle;
} gasneti_vis_op_t;

/* per-thread state for VIS */
typedef struct {
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

/* gasnete_threaddata_t might not be defined yet, but VIS ptr must be 3rd */
#define GASNETE_VIS_MYTHREAD (((void **)GASNETE_MYTHREAD)[2] ? \
        ((void **)GASNETE_MYTHREAD)[2] :                       \
        (((void **)GASNETE_MYTHREAD)[2] = gasnete_vis_new_threaddata()))

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

/*---------------------------------------------------------------------------------*/
/* VISOP manipulation */
#if GASNETI_HAVE_EOP_INTERFACE
/* create a dummy eop/iop based on synctype, save it in visop */
#define GASNETE_VISOP_SETUP(visop, synctype, isget) do {              \
    if (synctype == gasnete_synctype_nbi) {                           \
      visop->eop = NULL;                                              \
      visop->iop = gasneti_iop_register(1,isget GASNETE_THREAD_PASS); \
    } else {                                                          \
      visop->eop = gasneti_eop_create(GASNETE_THREAD_PASS_ALONE);     \
      visop->iop = NULL;                                              \
    }                                                                 \
} while (0)

/* Must not reference visop, which may no longer exist */
#define GASNETE_VISOP_RETURN_VOLATILE(eop, synctype) do {            \
    switch (synctype) {                                              \
      case gasnete_synctype_b: {                                     \
        gasnet_handle_t h = gasneti_eop_to_handle(eop);              \
        gasnete_wait_syncnb(h);                                      \
        return GASNET_INVALID_HANDLE;                                \
      }                                                              \
      case gasnete_synctype_nb:                                      \
        return gasneti_eop_to_handle(eop);                           \
      case gasnete_synctype_nbi:                                     \
        return GASNET_INVALID_HANDLE;                                \
      default: gasneti_fatalerror("bad synctype");                   \
        return GASNET_INVALID_HANDLE; /* avoid warning on MIPSPro */ \
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
#else
#define GASNETE_ERROR_NO_EOP_INTERFACE() gasneti_fatalerror("Tried to invoke GASNETE_VISOP_SIGNAL without GASNETI_HAVE_EOP_INTERFACE at %s:%i",__FILE__,__LINE__)
#define GASNETE_VISOP_SIGNAL(visop, isget) GASNETE_ERROR_NO_EOP_INTERFACE()
#define GASNETE_VISOP_SIGNAL(visop, isget) GASNETE_ERROR_NO_EOP_INTERFACE()
#define GASNETE_VISOP_SIGNAL(visop, isget) GASNETE_ERROR_NO_EOP_INTERFACE()
#endif

/* do GASNETE_VISOP_SETUP, push the visop on the thread-specific list 
   and do GASNETE_VISOP_RETURN */
#define GASNETE_PUSH_VISOP_RETURN(td, visop, synctype, isget) do {   \
    GASNETE_VISOP_SETUP(visop, synctype, isget);                     \
    GASNETI_PROGRESSFNS_ENABLE(gasneti_pf_vis,COUNTED);              \
    visop->next = td->active_ops; /* push on thread-specific list */ \
    td->active_ops = visop;                                          \
    GASNETE_VISOP_RETURN(visop, synctype);                           \
} while (0)
/*---------------------------------------------------------------------------------*/
/* ***  Individual put/get helpers *** */
/*---------------------------------------------------------------------------------*/
/* helper for vis functions implemented atop other GASNet operations
   start a recursive NBI access region, if appropriate */
#define GASNETE_START_NBIREGION(synctype, islocal) do {    \
  if (synctype != gasnete_synctype_nbi && !islocal)        \
    gasnete_begin_nbi_accessregion(1 GASNETE_THREAD_PASS); \
  } while(0)
/* finish a region started with GASNETE_START_NBIREGION,
   block if required, and return the appropriate handle */
#define GASNETE_END_NBIREGION_AND_RETURN(synctype, islocal) do {                      \
    if (islocal) return GASNET_INVALID_HANDLE;                                        \
    switch (synctype) {                                                               \
      case gasnete_synctype_nb:                                                       \
        return gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);               \
      case gasnete_synctype_b:                                                        \
        gasnete_wait_syncnb(gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE)); \
        return GASNET_INVALID_HANDLE;                                                 \
      case gasnete_synctype_nbi:                                                      \
        return GASNET_INVALID_HANDLE;                                                 \
      default: gasneti_fatalerror("bad synctype");                                    \
        return GASNET_INVALID_HANDLE; /* avoid warning on MIPSPro */                  \
    }                                                                                 \
  } while(0)

#define GASNETE_PUT_INDIV(islocal, dstnode, dstaddr, srcaddr, nbytes) do {      \
    gasneti_assert(nbytes > 0);                                                 \
    gasneti_boundscheck_allowoutseg(dstnode, dstaddr, nbytes);                  \
    gasneti_assert(islocal == (dstnode == gasneti_mynode));                     \
    if (islocal) GASNETE_FAST_UNALIGNED_MEMCPY((dstaddr), (srcaddr), (nbytes)); \
    else gasnete_put_nbi_bulk((dstnode), (dstaddr), (srcaddr), (nbytes)         \
                                GASNETE_THREAD_PASS);                           \
  } while (0)

#define GASNETE_GET_INDIV(islocal, dstaddr, srcnode, srcaddr, nbytes) do {      \
    gasneti_assert(nbytes > 0);                                                 \
    gasneti_boundscheck_allowoutseg(srcnode, srcaddr, nbytes);                  \
    gasneti_assert(islocal == (srcnode == gasneti_mynode));                     \
    if (islocal) GASNETE_FAST_UNALIGNED_MEMCPY((dstaddr), (srcaddr), (nbytes)); \
    else gasnete_get_nbi_bulk((dstaddr), (srcnode), (srcaddr), (nbytes)         \
                                GASNETE_THREAD_PASS);                           \
  } while (0)


/*---------------------------------------------------------------------------------*/
/* packing/unpacking helpers */
#define _GASNETE_PACK_HELPER(packed, unpacked, sz) \
        GASNETE_FAST_UNALIGNED_MEMCPY((packed), (unpacked), (sz))
#define _GASNETE_UNPACK_HELPER(packed, unpacked, sz) \
        GASNETE_FAST_UNALIGNED_MEMCPY((unpacked), (packed), (sz))

/*---------------------------------------------------------------------------------*/
/* packetization */
typedef struct {
  size_t firstidx;
  size_t firstoffset;
  size_t lastidx;
  size_t lastlen;
} gasnete_packetdesc_t;

extern void gasnete_packetize_verify(gasnete_packetdesc_t *pt, size_t ptidx, int lastpacket,
                              size_t count, size_t len, gasnet_memvec_t const *list);

/*---------------------------------------------------------------------------------*/
/* GASNETE_METAMACRO_ASC/DESC##maxval(fn) is a meta-macro that iteratively expands the fn_INT(x,y) macro 
   with ascending or descending integer arguments. The base case (value zero) is expanded as fn_BASE().
   maxval must be an integer in the range 0..GASNETE_METAMACRO_DEPTH_MAX
   This would be cleaner if we could use recursive macro expansion, but it seems at least gcc disallows
   this - if a macro invocation X(...) is found while expanding a different invocation of X (even with 
   different arguments), the nested invocation is left unexpanded 
*/

#define GASNETE_METAMACRO_DEPTH_MAX 8

#define GASNETE_METAMACRO_ASC0(fn) fn##_BASE()
#define GASNETE_METAMACRO_ASC1(fn) GASNETE_METAMACRO_ASC0(fn) fn##_INT(1,0)
#define GASNETE_METAMACRO_ASC2(fn) GASNETE_METAMACRO_ASC1(fn) fn##_INT(2,1)
#define GASNETE_METAMACRO_ASC3(fn) GASNETE_METAMACRO_ASC2(fn) fn##_INT(3,2)
#define GASNETE_METAMACRO_ASC4(fn) GASNETE_METAMACRO_ASC3(fn) fn##_INT(4,3)
#define GASNETE_METAMACRO_ASC5(fn) GASNETE_METAMACRO_ASC4(fn) fn##_INT(5,4)
#define GASNETE_METAMACRO_ASC6(fn) GASNETE_METAMACRO_ASC5(fn) fn##_INT(6,5)
#define GASNETE_METAMACRO_ASC7(fn) GASNETE_METAMACRO_ASC6(fn) fn##_INT(7,6)
#define GASNETE_METAMACRO_ASC8(fn) GASNETE_METAMACRO_ASC7(fn) fn##_INT(8,7)

#define GASNETE_METAMACRO_DESC0(fn) fn##_BASE()
#define GASNETE_METAMACRO_DESC1(fn) fn##_INT(1,0) GASNETE_METAMACRO_DESC0(fn) 
#define GASNETE_METAMACRO_DESC2(fn) fn##_INT(2,1) GASNETE_METAMACRO_DESC1(fn) 
#define GASNETE_METAMACRO_DESC3(fn) fn##_INT(3,2) GASNETE_METAMACRO_DESC2(fn) 
#define GASNETE_METAMACRO_DESC4(fn) fn##_INT(4,3) GASNETE_METAMACRO_DESC3(fn) 
#define GASNETE_METAMACRO_DESC5(fn) fn##_INT(5,4) GASNETE_METAMACRO_DESC4(fn) 
#define GASNETE_METAMACRO_DESC6(fn) fn##_INT(6,5) GASNETE_METAMACRO_DESC5(fn) 
#define GASNETE_METAMACRO_DESC7(fn) fn##_INT(7,6) GASNETE_METAMACRO_DESC6(fn) 
#define GASNETE_METAMACRO_DESC8(fn) fn##_INT(8,7) GASNETE_METAMACRO_DESC7(fn) 

/*---------------------------------------------------------------------------------*/

#if PLATFORM_COMPILER_SUN_C
  /* disable a harmless warning */
  #pragma error_messages(off, E_STATEMENT_NOT_REACHED)
#endif

#endif
