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


#endif
