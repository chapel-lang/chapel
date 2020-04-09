/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/ratomic/gasnet_ratomic_internal.h $
 * Description: GASNet Remote Atomics Internal Header
 * Copyright 2017, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_RATOMIC_INTERNAL_H
#define _GASNET_RATOMIC_INTERNAL_H

/*---------------------------------------------------------------------------------*/
//
// Common logic for RAtomic implementatons
//

#define GASNETI_AD_MAGIC           GASNETI_MAKE_MAGIC('A','D','_','t')
#define GASNETI_AD_BAD_MAGIC       GASNETI_MAKE_BAD_MAGIC('A','D','_','t')

extern gasneti_AD_t gasneti_alloc_ad(
                       gasneti_TM_t tm,
                       gex_DT_t dt,
                       gex_OP_t ops,
                       gex_Flags_t flags,
                       size_t alloc_size);
void gasneti_free_ad(gasneti_AD_t ad);

#ifdef GASNETI_AD_CREATE_HOOK
  extern void GASNETI_AD_CREATE_HOOK(
        gasneti_AD_t               real_ad,
        gasneti_TM_t               real_tm,
        gex_DT_t                   dt,
        gex_OP_t                   ops,
        gex_Flags_t                flags);
#endif 

//
// Initalizer for gasnete_ratomic[dtcode]_fn_tbl_t
//
#define GASNETE_RATOMIC_FN_TBL_INIT(prefix,dtcode) \
        _GASNETE_RATOMIC_FN_TBL_INIT1(prefix,dtcode##_isint)
// This extra pass expands the "isint" token prior to additional concatenation
#define _GASNETE_RATOMIC_FN_TBL_INIT1(prefix,isint) \
        _GASNETE_RATOMIC_FN_TBL_INIT2(prefix,isint)
#define _GASNETE_RATOMIC_FN_TBL_INIT2(prefix,isint) \
  { _GASNETE_RATOMIC_FN_TBL_INIT_INT##isint(prefix##_NB), \
    _GASNETE_RATOMIC_FN_TBL_INIT_INT##isint(prefix##_NBI) }
#define _GASNETE_RATOMIC_FN_TBL_INIT_INT0(prefix)    \
  { NULL,           NULL,           NULL,            \
    &prefix##_ADD,  &prefix##_SUB,  &prefix##_MULT,  \
    &prefix##_MIN,  &prefix##_MAX,                   \
    &prefix##_INC,  &prefix##_DEC,                   \
    NULL,           NULL,           NULL,            \
    &prefix##_FADD, &prefix##_FSUB, &prefix##_FMULT, \
    &prefix##_FMIN, &prefix##_FMAX,                  \
    &prefix##_FINC, &prefix##_FDEC,                  \
    &prefix##_SET,  &prefix##_GET,                   \
    &prefix##_SWAP, &prefix##_FCAS, &prefix##_CAS }
#define _GASNETE_RATOMIC_FN_TBL_INIT_INT1(prefix)    \
  { &prefix##_AND,  &prefix##_OR,   &prefix##_XOR,   \
    &prefix##_ADD,  &prefix##_SUB,  &prefix##_MULT,  \
    &prefix##_MIN,  &prefix##_MAX,                   \
    &prefix##_INC,  &prefix##_DEC,                   \
    &prefix##_FAND, &prefix##_FOR,  &prefix##_FXOR,  \
    &prefix##_FADD, &prefix##_FSUB, &prefix##_FMULT, \
    &prefix##_FMIN, &prefix##_FMAX,                  \
    &prefix##_FINC, &prefix##_FDEC,                  \
    &prefix##_SET,  &prefix##_GET,                   \
    &prefix##_SWAP, &prefix##_FCAS, &prefix##_CAS }

/*---------------------------------------------------------------------------------*/
//
// AM-based implementation
//

#ifndef GASNETE_BUILD_AMRATOMIC
  #define GASNETE_BUILD_AMRATOMIC 1
#endif

#if GASNETE_BUILD_AMRATOMIC
  extern void gasnete_amratomic_create_hook(
        gasneti_AD_t               real_ad,
        gasneti_TM_t               real_tm,
        gex_DT_t                   dt,
        gex_OP_t                   ops,
        gex_Flags_t                flags);

  #ifndef GASNETI_AD_CREATE_HOOK
    #define GASNETI_AD_CREATE_HOOK gasnete_amratomic_create_hook
  #endif

  // Force use of default "no" if tools are not suited to mixing
  // with RMA Put and Get.
  // TODO-EX: SIGNALSAFE is not the precise property we are looking for,
  // though it is accurate for the current tools implementations.
  #if GASNETI_ATOMIC32_NOT_SIGNALSAFE
    #undef GASNETE_AMRATOMIC_USE_RMA_gex_dt_I32
    #undef GASNETE_AMRATOMIC_USE_RMA_gex_dt_U32
    #undef GASNETE_AMRATOMIC_USE_RMA_gex_dt_FLT
  #endif
  #if GASNETI_ATOMIC64_NOT_SIGNALSAFE
    #undef GASNETE_AMRATOMIC_USE_RMA_gex_dt_I64
    #undef GASNETE_AMRATOMIC_USE_RMA_gex_dt_U64
    #undef GASNETE_AMRATOMIC_USE_RMA_gex_dt_DBL
  #endif

  // Default (subject to conduit override) to SET and GET via AM.
  // Each token GASNETE_AMRATOMIC_USE_RMA##dtcode should be 0 or 1:
  //   0: Implement SET and GET atomics via AMs
  //   1: Implement SET and GET atomics via gex_RMA_{Put,Get}...()
  #ifndef GASNETE_AMRATOMIC_USE_RMA_gex_dt_I32
    #define GASNETE_AMRATOMIC_USE_RMA_gex_dt_I32 0
  #endif
  #ifndef GASNETE_AMRATOMIC_USE_RMA_gex_dt_U32
    #define GASNETE_AMRATOMIC_USE_RMA_gex_dt_U32 0
  #endif
  #ifndef GASNETE_AMRATOMIC_USE_RMA_gex_dt_I64
    #define GASNETE_AMRATOMIC_USE_RMA_gex_dt_I64 0
  #endif
  #ifndef GASNETE_AMRATOMIC_USE_RMA_gex_dt_U64
    #define GASNETE_AMRATOMIC_USE_RMA_gex_dt_U64 0
  #endif
  #ifndef GASNETE_AMRATOMIC_USE_RMA_gex_dt_FLT
    #define GASNETE_AMRATOMIC_USE_RMA_gex_dt_FLT 0
  #endif
  #ifndef GASNETE_AMRATOMIC_USE_RMA_gex_dt_DBL
    #define GASNETE_AMRATOMIC_USE_RMA_gex_dt_DBL 0
  #endif
#endif

#endif
