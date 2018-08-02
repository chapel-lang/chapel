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
    } else if (!gasnet_mynode() && gasneti_getenv(#envname) && gasneti_getenv_yesno_withdefault(#envname, 0)) { \
      fprintf(stderr, "WARNING: %s is set in environment, but %s support is compiled out - setting ignored\n",  \
                      #envname, #enabler);                                                                      \
    }                                                                                                           \
  } while (0)
  #if !GASNETE_USE_AMPIPELINE
    int gasnete_vis_use_ampipe = 0; // dummy
  #endif
  GASNETE_VIS_ENV_YN(gasnete_vis_use_ampipe,GASNET_VIS_AMPIPE, GASNETE_USE_AMPIPELINE);
  #if GASNETE_USE_AMPIPELINE
    #ifndef GASNETE_VIS_MAXCHUNK_DEFAULT
    #define GASNETE_VIS_MAXCHUNK_DEFAULT gasnet_AMMaxMedium()-2*sizeof(void*)
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
  #endif
  #if !GASNETE_USE_REMOTECONTIG_GATHER_SCATTER
    int gasnete_vis_use_remotecontig = 0; // dummy
  #endif
  GASNETE_VIS_ENV_YN(gasnete_vis_use_remotecontig,GASNET_VIS_REMOTECONTIG, GASNETE_USE_REMOTECONTIG_GATHER_SCATTER);
}
/*---------------------------------------------------------------------------------*/

#define GASNETI_GASNET_REFVIS_C 1

#include "vis/gasnet_vector.c"

#include "vis/gasnet_indexed.c"

#include "vis/gasnet_strided.c"

#undef GASNETI_GASNET_REFVIS_C

/*---------------------------------------------------------------------------------*/
/* ***  Progress Function *** */
/*---------------------------------------------------------------------------------*/
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
  GASNETI_THREAD_LOOKUP /* TODO: remove this lookup */
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
    #ifdef GASNETE_PUTV_GATHER_SELECTOR
      case GASNETI_VIS_CAT_PUTV_GATHER:
        if (gasnete_try_syncnb(visop->handle) == GASNET_OK) {
          GASNETE_VISOP_SIGNAL_AND_FREE(visop, 0);
        }
      break;
    #endif
    #ifdef GASNETE_GETV_SCATTER_SELECTOR
      case GASNETI_VIS_CAT_GETV_SCATTER:
        if (gasnete_try_syncnb(visop->handle) == GASNET_OK) {
          gasnet_memvec_t const * const savedlst = (gasnet_memvec_t const *)(visop + 1);
          void const * const packedbuf = savedlst + visop->count;
          gasnete_memvec_unpack(visop->count, savedlst, packedbuf, 0, (size_t)-1);
          GASNETE_VISOP_SIGNAL_AND_FREE(visop, 1);
        }
      break;
    #endif
    #ifdef GASNETE_PUTI_GATHER_SELECTOR
      case GASNETI_VIS_CAT_PUTI_GATHER:
        if (gasnete_try_syncnb(visop->handle) == GASNET_OK) { 
          GASNETE_VISOP_SIGNAL_AND_FREE(visop, 0);
        }
      break;
    #endif
    #ifdef GASNETE_GETI_SCATTER_SELECTOR
      case GASNETI_VIS_CAT_GETI_SCATTER:
        if (gasnete_try_syncnb(visop->handle) == GASNET_OK) {
          void * const * const savedlst = (void * const *)(visop + 1);
          void const * const packedbuf = savedlst + visop->count;
          gasnete_addrlist_unpack(visop->count, savedlst, visop->len, packedbuf, 0, (size_t)-1);
          GASNETE_VISOP_SIGNAL_AND_FREE(visop, 1);
        }
      break;
    #endif
    #ifdef GASNETE_PUTS_GATHER_SELECTOR
      case GASNETI_VIS_CAT_PUTS_GATHER:
        if (gasnete_try_syncnb(visop->handle) == GASNET_OK) { 
          GASNETE_VISOP_SIGNAL_AND_FREE(visop, 0);
        }
      break;
    #endif
    #ifdef GASNETE_GETS_SCATTER_SELECTOR
      case GASNETI_VIS_CAT_GETS_SCATTER:
        if (gasnete_try_syncnb(visop->handle) == GASNET_OK) {
          size_t stridelevels = visop->len;
          size_t * const savedstrides = (size_t *)(visop + 1);
          size_t * const savedcount = savedstrides + stridelevels;
          void * const packedbuf = (void *)(savedcount + stridelevels + 1);
          gasnete_strided_unpack_all(visop->addr, savedstrides, savedcount, stridelevels, packedbuf);
          GASNETE_VISOP_SIGNAL_AND_FREE(visop, 1);
        }
      break;
    #endif
      default: gasneti_fatalerror("unrecognized visop category: %i", visop->type);
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
