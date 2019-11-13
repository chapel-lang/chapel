/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ibv-conduit/firehose_fwd.h $
 * Description: Configuration of firehose code to fit ibv-conduit
 * Copyright 2003, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _IBV_FIREHOSE_FWD_H
#define _IBV_FIREHOSE_FWD_H

  #if defined(__GNUC__) && !defined(inline)
    /* ANSI-ify the IB Verbs headers */
    #define inline __inline__
    #include <infiniband/verbs.h>
    #undef inline
  #else
    #include <infiniband/verbs.h>
  #endif
  #ifdef GASNETC_IBV_MAX_HCAS
    #define GASNETC_IB_MAX_HCAS GASNETC_IBV_MAX_HCAS
  #else /* no multi-rail support */
    #define GASNETC_IB_MAX_HCAS 1
  #endif

/* Set this here because we need it to match */
#define FH_BUCKET_SIZE	GASNET_PAGESIZE

/* ibv-conduit uses firehose-region */
#define FIREHOSE_REGION

/* ibv-conduit allows completion callbacks to run in handlers */
#define FIREHOSE_COMPLETION_IN_HANDLER

/* ibv-conduit has a client_t */
#define FIREHOSE_CLIENT_T
typedef struct _firehose_client_t {
    struct ibv_mr *   handle[GASNETC_IB_MAX_HCAS];	/* used to release the region */
    uint32_t          lkey[GASNETC_IB_MAX_HCAS];	/* used for local access by HCA */
    uint32_t          rkey[GASNETC_IB_MAX_HCAS];	/* used for remote access by HCA */
} firehose_client_t;

#if GASNET_SEGMENT_FAST
  /* unused, but cannot be empty */
  typedef char firehose_remotecallback_args_t;
#else // Using remote firehose
 #ifndef GASNETC_PUTINMOVE_LIMIT_MAX
  /* Compile-time max bytes to piggyback on a put/miss.
   * Environment can always specify a lesser limit, but not larger.
   *
   * Total size of firehose_remotecallback_args_t (below) must be  no larger than
   * fits in a Medium payload, along with firehose's overhead which is
   *    (1 + FH_MAX_UNPIN_REM) * sizeof(firehose_region_t)
   * Where sizeof(firehose_region_t) is
   *    2*SIZEOF_VOID_P + sizeof(firehose_client_t)
   * For a 64-bit platform and default values of FH_MAX_UNPIN_REM and GASNETC_IB_MAX_HCAS:
   *   GASNETC_PUTINMOVE_LIMIT_MAX <= GASNETC_MAX_MEDIUM - 256
   * For a 32-bit platform
   *   GASNETC_PUTINMOVE_LIMIT_MAX <= GASNETC_MAX_MEDIUM - 168
   * NOTE: these are correct at the time of writting, but subject to change.
   * Unfortunately we can't currently work everything out with the preprocessor.
   */
  #if (GASNETC_BUFSZ >= 4096)
    /* Diminishing returns as the value is increased beyond 3k */
    #define GASNETC_PUTINMOVE_LIMIT_MAX 3072
  #else
    /* WARNING: too large values of GASNETC_IB_MAX_HCAS could be a problem */
    #error "Since GASNETC_BUFSZ is set to a non-default value, GASNETC_PUTINMOVE_LIMIT_MAX must also be set"
  #endif
 #endif
  typedef struct {
    void	*addr;
    size_t	len;
    char	data[GASNETC_PUTINMOVE_LIMIT_MAX];
  } firehose_remotecallback_args_t;
#endif

#define FIREHOSE_REMOTE_CALLBACK_IN_HANDLER

// Check the validity of a client_t
GASNETI_INLINE(gasneti_valid_client_t)
int gasneti_valid_client_t(const firehose_client_t *client) {
  return (NULL != client->handle[0]);
}

// Permit merge of a given region if and only if the client_t is "valid"
#define FH_MAY_MERGE gasneti_valid_client_t

#endif
