/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/vis/gasnet_refvis.c $
 * Description: Reference implementation of GASNet Vector, Indexed & Strided
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_vis_internal.h>

#include <gasnet_refvis.h>

/*---------------------------------------------------------------------------------*/
/* *** VIS Init *** */
/*---------------------------------------------------------------------------------*/
static int gasnete_vis_isinit = 0;

#if GASNETE_USE_AMPIPELINE
static int gasnete_vis_use_ampipe;
static size_t gasnete_vis_put_maxchunk;
static size_t gasnete_vis_get_maxchunk;
#endif
#if GASNETE_USE_REMOTECONTIG_GATHER_SCATTER
static int gasnete_vis_use_remotecontig;
#endif

extern void gasnete_vis_init(void) {
  gasneti_assert(!gasnete_vis_isinit);
  gasnete_vis_isinit = 1;
  GASNETI_TRACE_PRINTF(C,("gasnete_vis_init()"));

  #define GASNETE_VIS_ENV_YN(varname, envname, enabler) do {                                                    \
    if (enabler) {                                                                                              \
      varname = gasneti_getenv_yesno_withdefault(#envname, enabler##_DEFAULT);                                  \
    } else if (!gasneti_mynode && gasneti_getenv(#envname) && gasneti_getenv_yesno_withdefault(#envname, 0)) {  \
      fprintf(stderr, "WARNING: %s is set in environment, but %s support is compiled out - setting ignored\n",  \
                      #envname, #enabler);                                                                      \
    }                                                                                                           \
  } while (0)
  #if !GASNETE_USE_AMPIPELINE
    int gasnete_vis_use_ampipe = 0; // dummy
  #endif
  GASNETE_VIS_ENV_YN(gasnete_vis_use_ampipe,GASNET_VIS_AMPIPE, GASNETE_USE_AMPIPELINE);
  #if GASNETE_USE_AMPIPELINE
  if (gasnete_vis_use_ampipe) {
    #ifndef GASNETE_VIS_MAXCHUNK_DEFAULT
    #define GASNETE_VIS_MAXCHUNK_DEFAULT MIN(gex_AM_LUBRequestMedium(),gex_AM_LUBReplyMedium())-2*sizeof(void*)
    #endif
    #ifndef GASNETE_VIS_PUT_MAXCHUNK_DEFAULT
    #define GASNETE_VIS_PUT_MAXCHUNK_DEFAULT GASNETE_VIS_MAXCHUNK_DEFAULT
    #endif
    #ifndef GASNETE_VIS_GET_MAXCHUNK_DEFAULT
    #define GASNETE_VIS_GET_MAXCHUNK_DEFAULT GASNETE_VIS_MAXCHUNK_DEFAULT
    #endif
    int gasnete_vis_maxchunk_set = !!gasneti_getenv("GASNET_VIS_MAXCHUNK");
    size_t gasnete_vis_maxchunk = GASNETE_VIS_MAXCHUNK_DEFAULT;
    gasnete_vis_maxchunk = gasneti_getenv_int_withdefault("GASNET_VIS_MAXCHUNK", gasnete_vis_maxchunk, 1);
    gasnete_vis_put_maxchunk = GASNETE_VIS_PUT_MAXCHUNK_DEFAULT;
    gasnete_vis_put_maxchunk = gasneti_getenv_int_withdefault("GASNET_VIS_PUT_MAXCHUNK", 
                                 (gasnete_vis_maxchunk_set ? gasnete_vis_maxchunk : gasnete_vis_put_maxchunk), 1);
    gasnete_vis_get_maxchunk = GASNETE_VIS_GET_MAXCHUNK_DEFAULT;
    gasnete_vis_get_maxchunk = gasneti_getenv_int_withdefault("GASNET_VIS_GET_MAXCHUNK", 
                                 (gasnete_vis_maxchunk_set ? gasnete_vis_maxchunk : gasnete_vis_get_maxchunk), 1);
  } else { // !gasnete_vis_use_ampipe
    gasnete_vis_put_maxchunk = 0;
    gasnete_vis_get_maxchunk = 0;
  }
  #endif
  #if !GASNETE_USE_REMOTECONTIG_GATHER_SCATTER
    int gasnete_vis_use_remotecontig = 0; // dummy
  #endif
  GASNETE_VIS_ENV_YN(gasnete_vis_use_remotecontig,GASNET_VIS_REMOTECONTIG, GASNETE_USE_REMOTECONTIG_GATHER_SCATTER);
}
/*---------------------------------------------------------------------------------*/
// Peer completion support
//

#include <gasnet_am.h>

#include "gasnet_event_internal.h" // TODO-EX: REMOVE THIS
// VILE HACK: internal op interface lacks the capability to manipulate op ALC that we need 
// for correct interoperation of peer completion and ALC on the same VIS put

typedef struct {
  gasneti_vis_op_t visop;  // must be first

  gex_TM_t tm;
  gex_Rank_t rank;
  gex_Event_t lc;
} gasneti_vispc_op_t;

extern void gasnete_VIS_SetPeerCompletionHandler(gex_AM_Index_t handler,
        const void *source_addr, size_t nbytes, gex_Flags_t flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_PRINTF(A,("VIS_SetPeerCompletionHandler: handler=%i, source_addr="GASNETI_LADDRFMT", nbytes=%"PRIuSZ,
                          (int)handler, GASNETI_LADDRSTR(source_addr),nbytes)); 
  gasnete_vis_threaddata_t * const td = GASNETE_VIS_MYTHREAD; 
  gasnete_vis_pcinfo_t * const pcinfo = &(td->pcinfo);
  if (!handler) { // disarm
    pcinfo->_handler = 0; 
  } else { // arm
    gasneti_assert_uint(nbytes ,<=, GEX_VIS_MAX_PEERCOMPLETION);
    gasneti_assert(!nbytes || source_addr);
    pcinfo->_handler = handler; 
    pcinfo->_nbytes = nbytes; 
    pcinfo->_srcaddr = source_addr; 
  }
}
GASNETI_INLINE(gasnete_VIS_pcwrap)
gex_Event_t gasnete_VIS_pcwrap(gasnete_synctype_t const synctype, // manifest constant
                               gex_TM_t tm, gex_Rank_t rank, gex_Flags_t flags,
                               gex_Event_t const evt GASNETI_THREAD_FARG) {
  gasnete_vis_threaddata_t * const td = GASNETE_VIS_MYTHREAD; 
  gasnete_vis_pcinfo_t * const pcinfo = &(td->pcinfo);
  gasneti_assert(pcinfo->_handler);

  GASNETI_TRACE_PRINTF(D,("VIS_PeerCompletionHandler scheduled: handler=%i, source_addr="GASNETI_LADDRFMT", nbytes=%"PRIuSZ,
                          (int)pcinfo->_handler, GASNETI_LADDRSTR(pcinfo->_srcaddr), pcinfo->_nbytes));

  if (evt == GEX_EVENT_INVALID || // synchronously complete
      (synctype == gasnete_synctype_b && (gex_Event_Wait(evt),1))) { // blocking
    gex_Event_t lc;
    gex_Event_t *lc_opt;
    switch (synctype) {
      case gasnete_synctype_b:   lc_opt = GEX_EVENT_NOW; lc = GEX_EVENT_INVALID; break;
      case gasnete_synctype_nb:  lc_opt = &lc; break;
      case gasnete_synctype_nbi: lc_opt = GEX_EVENT_GROUP; lc = GEX_EVENT_INVALID; break;
      default: gasneti_unreachable_error(("Invalid synctype=%i",(int)synctype));
    }
    gex_AM_RequestMedium1(tm, rank, _hidx_gasnete_vis_pcthunk_reqh, (void *)(pcinfo->_srcaddr), pcinfo->_nbytes, lc_opt, 0, pcinfo->_handler);
    pcinfo->_handler = 0; // reset
    return lc;
  } else { // schedule deferred initiator-chaining
    gasneti_vispc_op_t * const vispcop = gasneti_malloc(sizeof(gasneti_vispc_op_t)+GEX_VIS_MAX_PEERCOMPLETION);
    gasneti_vis_op_t * const visop = &(vispcop->visop);
    vispcop->tm =   tm; 
    vispcop->rank = rank; 
    visop->type = GASNETI_VIS_CAT_PUTPC_CHAIN;
    visop->count =  pcinfo->_handler;
    pcinfo->_handler = 0; // reset
    visop->len =    pcinfo->_nbytes;
    visop->event =  evt;
    if (flags & GEX_FLAG_ENABLE_LEAF_LC) { // client also requesting LC
      #if GASNETE_HAVE_LC 
        vispcop->lc = gex_Event_QueryLeaf(evt, GEX_EC_LC);
        // TODO-EX: remove this event_internal vileness
        // set ALC in-flight for the client's op
        #define VISOP_EXTRA do {                                                                      \
          if (vispcop->lc) {                                                                          \
            if (synctype == gasnete_synctype_nb) GASNETE_EOP_LC_START((gasnete_eop_t *)(visop->eop)); \
            else                                 GASNETE_IOP_LC_START((gasnete_iop_t *)visop->iop);   \
          }                                                                                           \
        } while (0)
      #else
        #if GASNET_DEBUG
          vispcop->lc = GEX_EVENT_INVALID; // for assertions only
        #endif
        #define VISOP_EXTRA ((void)0)
      #endif
      // bounce-buffer the PC payload if the client requested LC
      // this is REQUIRED for conduits lacking ALC signalling support, to correctly implement synchronous LC
      // this is an optimization for other conduits, to ensure we can report VIS payload LC to the client when it happens
      void *pcpayload = vispcop+1;
      GASNETI_MEMCPY_SAFE_EMPTY(pcpayload, pcinfo->_srcaddr, pcinfo->_nbytes);
      visop->addr = pcpayload;
    } else {
      visop->addr =   (void*)pcinfo->_srcaddr;
      vispcop->lc = GEX_EVENT_INVALID;
    }
    GASNETE_PUSH_VISOP_RETURN(td, visop, synctype, 0, VISOP_EXTRA);
    #undef VISOP_EXTRA
  }
}
extern int         gasnete_VIS_pcwrapBlocking(_GASNETE_VIS_PCWRAP_ARGS) {
  return (int)(intptr_t)gasnete_VIS_pcwrap(gasnete_synctype_b, _tm, _rank, _flags, _evt GASNETI_THREAD_PASS);
}
extern int         gasnete_VIS_pcwrapNBI     (_GASNETE_VIS_PCWRAP_ARGS) {
  return (int)(intptr_t)gasnete_VIS_pcwrap(gasnete_synctype_nbi, _tm, _rank, _flags, _evt GASNETI_THREAD_PASS);
}
extern gex_Event_t gasnete_VIS_pcwrapNB      (_GASNETE_VIS_PCWRAP_ARGS) {
  return gasnete_VIS_pcwrap(gasnete_synctype_nb, _tm, _rank, _flags, _evt GASNETI_THREAD_PASS);
}
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_vis_run_pchandler)
void gasnete_vis_run_pchandler(gex_Token_t token, void *addr, size_t nbytes, gex_AM_Index_t handler_id) {

  gex_Token_Info_t info;
  gex_TI_t rc = gex_Token_Info(token, &info, GEX_TI_SRCRANK|GEX_TI_EP);
  gasneti_assert((rc & GEX_TI_SRCRANK) && (rc & GEX_TI_EP));

  gex_AM_Entry_t *entry = gasnetc_get_hentry(info.gex_ep,handler_id);
  gasneti_amtbl_check(entry, 0, gasneti_Medium, 0);

  gasnetc_nbrhd_token_t my_token;
  gex_Token_t thunk_token = gasnetc_nbrhd_token_init(&my_token, info.gex_srcrank, entry, 0);
  my_token.ti.gex_is_long = 0;

  gex_AM_Fn_t handler_fn = entry->gex_fnptr;

  GASNETI_RUN_HANDLER_MEDIUM(0,handler_id,handler_fn,thunk_token,NULL,0,addr,nbytes);
}
 
GASNETI_INLINE(gasnete_vis_pcthunk_reqh_inner)
void gasnete_vis_pcthunk_reqh_inner(gex_Token_t token, void *addr, size_t nbytes, gex_AM_Arg_t _chandler) {
  gex_AM_Index_t handler_id = (gex_AM_Index_t)_chandler;
  gasneti_assert(handler_id == _chandler);
  gasnete_vis_run_pchandler(token, addr, nbytes, handler_id);
}
MEDIUM_HANDLER(gasnete_vis_pcthunk_reqh,1,1,
              (token,addr,nbytes, a0),
              (token,addr,nbytes, a0));
/*---------------------------------------------------------------------------------*/

#define GASNETI_GASNET_REFVIS_C 1

#include "vis/gasnet_vector.c"

#include "vis/gasnet_indexed.c"

#define GASNETE_STRIDED_VERSION 2.0
#include "vis/gasnet_strided.c"

GASNETI_IDENT(gasneti_IdentString_StridedVersion,  "$GASNetStridedVersion: " _STRINGIFY(GASNETE_STRIDED_VERSION)" $");
GASNETI_IDENT(gasneti_IdentString_StridedLoopDims, "$GASNetStridedLoopingDims: "_STRINGIFY(GASNETE_LOOPING_DIMS)" $");
GASNETI_IDENT(gasneti_IdentString_StridedDirDims,  "$GASNetStridedDirectDims: " _STRINGIFY(GASNETE_DIRECT_DIMS)" $");
#if GASNETE_USE_AMPIPELINE
GASNETI_IDENT(gasneti_IdentString_VISNPAM,         "$GASNetVISNPAM: " _STRINGIFY(GASNETE_VIS_NPAM)" $");
#endif
GASNETI_IDENT(gasneti_IdentString_VISMinPackBuf,   "$GASNetVISMinPackBuffer: " _STRINGIFY(GASNETE_VIS_MIN_PACKBUFFER)" $");

#undef GASNETI_GASNET_REFVIS_C

/*---------------------------------------------------------------------------------*/
/* ***  Progress Function *** */
/*---------------------------------------------------------------------------------*/
gasnete_vis_epdata_t gasnete_vis_epdata_THUNK;

/* signal a visop dummy eop/iop, unlink it and free it */
#define GASNETE_VISOP_SIGNAL_AND_FREE(visop, isget) do { \
    GASNETE_VISOP_SIGNAL(visop, isget);                  \
    GASNETI_PROGRESSFNS_DISABLE(gasneti_pf_vis,COUNTED); \
    *lastp = visop->next; /* unlink */                   \
    gasneti_free(visop);                                 \
    goto visop_removed;                                  \
  } while (0)

extern void gasneti_vis_progressfn(void) { 
#if PLATFORM_COMPILER_SUN_C
  /* disable warnings triggered by nesting switch-inside-for */
  #pragma error_messages(off, E_LOOP_NOT_ENTERED_AT_TOP)
#endif
  GASNET_BEGIN_FUNCTION(); /* TODO: remove this lookup */
  gasnete_vis_threaddata_t *td = GASNETE_VIS_MYTHREAD; 
  gasneti_vis_op_t **lastp = &(td->active_ops);
  if (td->progressfn_active) return; /* prevent recursion */
  td->progressfn_active = 1;
  for (lastp = &(td->active_ops); *lastp; ) {
    gasneti_vis_op_t * const visop = *lastp;
    #ifdef GASNETE_VIS_PROGRESSFN_EXTRA
           GASNETE_VIS_PROGRESSFN_EXTRA(visop, lastp)
    #endif
    switch (visop->type) {
      case GASNETI_VIS_CAT_PUTPC_CHAIN: {
        gasneti_vispc_op_t * const vispcop = (gasneti_vispc_op_t *)visop;
        #if GASNETE_HAVE_LC 
          // forward ALC if it exists and the client requested it
          if (vispcop->lc) {
            if (gasnete_test(vispcop->lc GASNETI_THREAD_PASS) == GASNET_OK) {
              vispcop->lc = GEX_EVENT_INVALID;
              if (visop->eop) GASNETE_EOP_LC_FINISH((gasnete_eop_t *)(visop->eop));
              else            GASNETE_IOP_LC_FINISH((gasnete_iop_t *)(visop->iop));  
            } else break; // no ALC yet, so cannot have operation completion
          }
        #endif
        if (gasnete_test(visop->event GASNETI_THREAD_PASS) == GASNET_OK) {
          // could potentially delay visop free until ALC of this medium payload,
          // but given the small size it's probably synchronously complete for most conduits anyhow
          gasneti_assert(!vispcop->lc);
          gex_AM_RequestMedium1(vispcop->tm, vispcop->rank, _hidx_gasnete_vis_pcthunk_reqh, 
                                visop->addr, visop->len, GEX_EVENT_NOW, 0, (uint8_t)visop->count);
          GASNETE_VISOP_SIGNAL_AND_FREE(visop, 0);
        }
      break;
    }
    #ifdef GASNETE_PUTV_GATHER_SELECTOR
      case GASNETI_VIS_CAT_PUTV_GATHER:
        if (gasnete_test(visop->event GASNETI_THREAD_PASS) == GASNET_OK) {
          GASNETE_VISOP_SIGNAL_AND_FREE(visop, 0);
        }
      break;
    #endif
    #ifdef GASNETE_GETV_SCATTER_SELECTOR
      case GASNETI_VIS_CAT_GETV_SCATTER:
        if (gasnete_test(visop->event GASNETI_THREAD_PASS) == GASNET_OK) {
          gex_Memvec_t const * const savedlst = (gex_Memvec_t const *)(visop + 1);
          void const * const packedbuf = savedlst + visop->count;
          gasnete_memvec_unpack(visop->count, savedlst, packedbuf, 0, (size_t)-1);
          GASNETE_VISOP_SIGNAL_AND_FREE(visop, 1);
        }
      break;
    #endif
    #ifdef GASNETE_PUTI_GATHER_SELECTOR
      case GASNETI_VIS_CAT_PUTI_GATHER:
        if (gasnete_test(visop->event GASNETI_THREAD_PASS) == GASNET_OK) {
          GASNETE_VISOP_SIGNAL_AND_FREE(visop, 0);
        }
      break;
    #endif
    #ifdef GASNETE_GETI_SCATTER_SELECTOR
      case GASNETI_VIS_CAT_GETI_SCATTER:
        if (gasnete_test(visop->event GASNETI_THREAD_PASS) == GASNET_OK) {
          void * const * const savedlst = (void * const *)(visop + 1);
          void const * const packedbuf = savedlst + visop->count;
          gasnete_addrlist_unpack(visop->count, savedlst, visop->len, packedbuf, 0, (size_t)-1);
          GASNETE_VISOP_SIGNAL_AND_FREE(visop, 1);
        }
      break;
    #endif
    #ifdef GASNETE_PUTS_GATHER_SELECTOR
      case GASNETI_VIS_CAT_PUTS_GATHER:
        if (gasnete_test(visop->event GASNETI_THREAD_PASS) == GASNET_OK) {
          GASNETE_VISOP_SIGNAL_AND_FREE(visop, 0);
        }
      break;
    #endif
    #ifdef GASNETE_GETS_SCATTER_SELECTOR
      case GASNETI_VIS_CAT_GETS_SCATTER:
        if (gasnete_test(visop->event GASNETI_THREAD_PASS) == GASNET_OK) {
          size_t stridelevels = visop->len;
          size_t * const savedstrides = (size_t *)(visop + 1);
          size_t * const savedcount = savedstrides + stridelevels;
          void * const packedbuf = (void *)(savedcount + stridelevels + 1);
          gasnete_strided_unpack_all(visop->addr, savedstrides, savedcount, stridelevels, packedbuf);
          GASNETE_VISOP_SIGNAL_AND_FREE(visop, 1);
        }
      break;
    #endif
      default: gasneti_unreachable_error(("unrecognized visop category: 0x%x", (int)visop->type));
    }
    lastp = &(visop->next); /* advance */
    visop_removed: ;
  }
  td->progressfn_active = 0;
#if PLATFORM_COMPILER_SUN_C
  /* resume default treatment of the message we suppressed */
  #pragma error_messages(default, E_LOOP_NOT_ENTERED_AT_TOP)
#endif
}

/*---------------------------------------------------------------------------------*/
