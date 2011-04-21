/*   $Source: /var/local/cvs/gasnet/vapi-conduit/firehose_fwd.h,v $
 *     $Date: 2008/03/05 23:54:28 $
 * $Revision: 1.15 $
 * Description: Configuration of firehose code to fit vapi-conduit
 * Copyright 2003, E. O. Lawrence Berekely National Laboratory
 * Terms of use are as specified in license.txt
 */

#ifndef _VAPI_FIREHOSE_FWD_H
#define _VAPI_FIREHOSE_FWD_H

#if defined(GASNET_CONDUIT_VAPI) && !defined(GASNET_CONDUIT_IBV)
  #undef GASNET_CONDUIT_VAPI
  #define GASNET_CONDUIT_VAPI 1
#elif !defined(GASNET_CONDUIT_VAPI) && defined(GASNET_CONDUIT_IBV)
  #undef GASNET_CONDUIT_IBV
  #define GASNET_CONDUIT_IBV 1
#else
  #error "Exactly one of GASNET_CONDUIT_VAPI or GASNET_CONDUIT_IBV must be defined"
#endif

#if GASNET_CONDUIT_VAPI
  #if PLATFORM_COMPILER_SUN_C
    /* Supress warnings about out-of-range constants in an enum (an explict 0xFFFFFFFF) */
    #pragma error_messages(off, E_ENUM_VAL_OVERFLOWS_INT_MAX)
  #endif
  #include <vapi_types.h>
  #if PLATFORM_COMPILER_SUN_C
    #pragma error_messages(default, E_ENUM_VAL_OVERFLOWS_INT_MAX)
  #endif
  #ifndef FIREHOSE_VAPI_USE_FMR
    /* Some versions of VAPI offer "Fast Memory Regions".
     * They really are faster, so we use them by default when available */
    #if HAVE_VAPI_FMR
      #define FIREHOSE_VAPI_USE_FMR 1
    #else
      #define FIREHOSE_VAPI_USE_FMR 0
    #endif
  #endif
  #if FIREHOSE_VAPI_USE_FMR
    #define _FIREHOSE_VAPI_MR_HNDL_T	EVAPI_fmr_hndl_t
  #else
    #define _FIREHOSE_VAPI_MR_HNDL_T	VAPI_mr_hndl_t
  #endif
  #define _FIREHOSE_VAPI_LKEY_T		VAPI_lkey_t
  #define _FIREHOSE_VAPI_RKEY_T		VAPI_rkey_t
  #ifdef GASNETC_VAPI_MAX_HCAS
    #define GASNETC_IB_MAX_HCAS GASNETC_VAPI_MAX_HCAS
  #else /* no multi-rail support */
    #define GASNETC_IB_MAX_HCAS 1
  #endif
#elif GASNET_CONDUIT_IBV
  #include <infiniband/verbs.h>
  #define _FIREHOSE_VAPI_MR_HNDL_T	struct ibv_mr *
  #define _FIREHOSE_VAPI_LKEY_T		uint32_t
  #define _FIREHOSE_VAPI_RKEY_T		uint32_t
  #define GASNETC_IB_MAX_
  #ifdef GASNETC_IBV_MAX_HCAS
    #define GASNETC_IB_MAX_HCAS GASNETC_IBV_MAX_HCAS
  #else /* no multi-rail support */
    #define GASNETC_IB_MAX_HCAS 1
  #endif

#else
  #error "Unknown IB API"
#endif

/* Set this here because we need it to match */
#define FH_BUCKET_SIZE	GASNET_PAGESIZE

/* vapi-conduit uses firehose-region */
#define FIREHOSE_REGION

/* vapi-conduit allows completion callbacks to run in handlers */
#define FIREHOSE_COMPLETION_IN_HANDLER

/* vapi-conduit has a client_t */
#define FIREHOSE_CLIENT_T
typedef struct _firehose_client_t {
    _FIREHOSE_VAPI_MR_HNDL_T   handle[GASNETC_IB_MAX_HCAS];	/* used to release the region */
    _FIREHOSE_VAPI_LKEY_T      lkey[GASNETC_IB_MAX_HCAS];	/* used for local access by HCA */
    _FIREHOSE_VAPI_RKEY_T      rkey[GASNETC_IB_MAX_HCAS];	/* used for remote access by HCA */
} firehose_client_t;

#ifndef GASNETC_PUTINMOVE_LIMIT_MAX
  /* Compile-time max bytes to piggyback on a put/miss.
   * Environment can always specify a lesser limit, but not larger.
   */
  #define GASNETC_PUTINMOVE_LIMIT_MAX 3072
#endif
typedef struct {
    void	*addr;
    size_t	len;
    char	data[GASNETC_PUTINMOVE_LIMIT_MAX];
} firehose_remotecallback_args_t;

#define FIREHOSE_REMOTE_CALLBACK_IN_HANDLER

/* Not intended for general use */
#undef _FIREHOSE_VAPI_MR_HNDL_T
#undef _FIREHOSE_VAPI_LKEY_T
#undef _FIREHOSE_VAPI_RKEY_T

#endif
