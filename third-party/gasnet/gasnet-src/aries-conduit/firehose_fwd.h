/*   $Source: bitbucket.org:berkeleylab/gasnet.git/aries-conduit/firehose_fwd.h $
 * Description: Configuration of firehose code to fit aries-conduit
 * Copyright 2015, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _ARIES_FIREHOSE_FWD_H
#define _ARIES_FIREHOSE_FWD_H

#include <gni_pub.h>

/* Set this here because we need it to match */
#define FH_BUCKET_SIZE	GASNET_PAGESIZE

/* aries-conduit uses firehose-region */
#define FIREHOSE_REGION

/* aries-conduit allows completion callbacks to run in handlers */
#define FIREHOSE_COMPLETION_IN_HANDLER

/* aries-conduit has a client_t */
#define FIREHOSE_CLIENT_T
typedef gni_mem_handle_t firehose_client_t;

/* this is just the example, and is unused */
typedef struct _firehose_remotecallback_args_t {
        uintptr_t       local_addr;
        uintptr_t       remote_addr;
        size_t          nbytes;
}
firehose_remotecallback_args_t;

/* remote callback is not used - this is forward-looking */
#define FIREHOSE_REMOTE_CALLBACK_IN_HANDLER

/* since victim fifo is large try hard to cleanup */
#define FH_CLEAN_COVERED_AGGRESSIVE

/* Mark a client_t as invalid (for caching of failures) */
/* Zeroing of qword2 is enough, since it holds the (non-zero) length. */
GASNETI_ALWAYS_INLINE(gasneti_invalidate_client_t)
void gasneti_invalidate_client_t(firehose_client_t *client) {
  client->qword2 = 0;
}

/* Check the validity of a client_t */
GASNETI_ALWAYS_INLINE(gasneti_valid_client_t)
int gasneti_valid_client_t(const firehose_client_t *client) {
  return (0 != client->qword2);
}

/* May merge a region if and only if the client_t is valid */
#define FH_MAY_MERGE gasneti_valid_client_t

#endif
