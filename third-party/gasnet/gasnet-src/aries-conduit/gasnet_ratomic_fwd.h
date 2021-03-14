/*   $Source: bitbucket.org:berkeleylab/gasnet.git/aries-conduit/gasnet_ratomic_fwd.h $
 * Description: GASNet Remote Atomics API Header (aries-conduit specific forward decls)
 * Copyright 2017, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_RATOMIC_FWD_H
#define _GASNET_RATOMIC_FWD_H

// Enable use of RMA Put and Get for atomic SET and GET operations within
// the AM-based atomics.
//
// GNI RMA of naturally aligned 4 and 8 byte values are "tools safe" (free of
// word-tearing and intermediate values).  So long as the Tools have those same
// properties, we allow the AM-based atomics to use RMA Put and Get for the
// atomic SET and GET operations.
#if 1
  #define GASNETE_AMRATOMIC_USE_RMA_gex_dt_I32 1
  #define GASNETE_AMRATOMIC_USE_RMA_gex_dt_U32 1
  #define GASNETE_AMRATOMIC_USE_RMA_gex_dt_FLT 1
#endif
#if 1
  #define GASNETE_AMRATOMIC_USE_RMA_gex_dt_I64 1
  #define GASNETE_AMRATOMIC_USE_RMA_gex_dt_U64 1
  #define GASNETE_AMRATOMIC_USE_RMA_gex_dt_DBL 1
#endif

// Build GNI remote atomics by default
#if defined(GASNETC_BUILD_GNIRATOMIC) && !GASNETC_BUILD_GNIRATOMIC
  #undef GASNETC_BUILD_GNIRATOMIC
#else
  #undef GASNETC_BUILD_GNIRATOMIC
  #define GASNETC_BUILD_GNIRATOMIC 1
#endif

#if GASNETC_BUILD_GNIRATOMIC
  #define GASNETI_AD_CREATE_HOOK gasnete_gniratomic_create_hook

  /* stats needed by the GNI-specific atomics implementation */
  #ifndef GASNETI_RATOMIC_STATS
    #define GASNETI_RATOMIC_STATS(CNT,VAL,TIME)    \
        /* Currently empty */
  #endif

  // Cannot assume always safe to use GASNet tools - need to chech each AD
  #define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_I32 0
  #define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_U32 0
  #define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_I64 0
  #define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_U64 0
  #define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_FLT 0
  #define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_DBL 0
#else // NOT building GNI-specific atomics
  /* stats needed by the RAtomic reference implementation */
  #ifndef GASNETI_RATOMIC_STATS
    #define GASNETI_RATOMIC_STATS(CNT,VAL,TIME)    \
        /* Currently empty */
  #endif

  #define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_I32 1
  #define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_U32 1
  #define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_I64 1
  #define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_U64 1
  #define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_FLT 1
  #define GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE_gex_dt_DBL 1
#endif

#endif // _GASNET_RATOMIC_FWD_H
