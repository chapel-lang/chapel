/*   $Source$
 *     $Date$
 * $Revision$
 * Description: Configuration of firehose code to fit vapi-conduit
 * Copyright 2003, E. O. Lawrence Berekely National Laboratory
 * Terms of use are as specified in license.txt
 */

#ifndef _VAPI_FIREHOSE_FWD_H
#define _VAPI_FIREHOSE_FWD_H

#if !defined(GASNETC_IB_VAPI) && !defined(GASNETC_IB_VERBS)
  #define GASNETC_IB_VAPI       /* Default */
#endif
#if defined(GASNETC_IB_VAPI) && !defined(GASNETC_IB_VERBS)
  #undef GASNETC_IB_VAPI
  #define GASNETC_IB_VAPI 1
#elif !defined(GASNETC_IB_VAPI) && defined(GASNETC_IB_VERBS)
  #undef GASNETC_IB_VERBS
  #define GASNETC_IB_VERBS 1
#else
  #error "Only one of GASNETC_IB_VAPI or GASNETC_IB_VERBS may be defined"
#endif

#if GASNETC_IB_VAPI
  #include <vapi_types.h>
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
#elif GASNETC_IB_VERBS
  #include <infiniband/verbs.h>
  #define _FIREHOSE_VAPI_MR_HNDL_T	struct ibv_mr *
  #define _FIREHOSE_VAPI_LKEY_T		uint32_t
  #define _FIREHOSE_VAPI_RKEY_T		uint32_t
#else
  #error "Unknown IB API"
#endif

/* Set this here because we need it to match */
#define FH_BUCKET_SIZE	GASNET_PAGESIZE

#ifndef GASNETC_VAPI_MAX_HCAS
  /* Undefined means no multi-rail support */
  #define GASNETC_VAPI_MAX_HCAS 1
#endif

/* vapi-conduit uses firehose-region */
#define FIREHOSE_REGION

/* vapi-conduit allows completion callbacks to run in handlers */
#define FIREHOSE_COMPLETION_IN_HANDLER

/* vapi-conduit has a client_t */
#define FIREHOSE_CLIENT_T
typedef struct _firehose_client_t {
    _FIREHOSE_VAPI_MR_HNDL_T   handle[GASNETC_VAPI_MAX_HCAS];	/* used to release the region */
    _FIREHOSE_VAPI_LKEY_T      lkey[GASNETC_VAPI_MAX_HCAS];	/* used for local access by HCA */
    _FIREHOSE_VAPI_RKEY_T      rkey[GASNETC_VAPI_MAX_HCAS];	/* used for remote access by HCA */
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

/* Setup conduit-specific region parameters
 * Note that these are kept to sane sizes rather than the HCA limit
 * 128kB is the peak of the bandwidth curve and thus a good size.
 * With 32k * 128k = 4G we can pin upto 4GB of physical memory with these.
 * We don't yet deal well with many small regions.
 */
#define FIREHOSE_CLIENT_MAXREGIONS	32768
#define FIREHOSE_CLIENT_MAXREGION_SIZE	131072

/* Not intended for general use */
#undef _FIREHOSE_VAPI_MR_HNDL_T
#undef _FIREHOSE_VAPI_LKEY_T
#undef _FIREHOSE_VAPI_RKEY_T

#endif
