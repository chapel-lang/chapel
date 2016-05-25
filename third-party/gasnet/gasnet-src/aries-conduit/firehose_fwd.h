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


#endif
