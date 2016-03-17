/*
 * Description: GASNet MXM Send Request Definition
 * Copyright (c)  2012, Mellanox Technologies LTD. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#ifndef GASNET_MXM_REQ_H
#define GASNET_MXM_REQ_H

#include <gasnet_core_internal.h>

/*** gasnet_mxm_sent_req_t
 * This struct holds all the data that is needed
 * to be used in MXM's send request:
 *    - two fixed "header" fields for use as a _gasnete_op_t
 *    - MXM send request itself
 *    - IOV of 2 entries
 *    - buffer for arguments
 *    - additional info for SEND after PUT operation - dst_addr and length
 *    - user data that was possibly copied in from the user buffer
 */
typedef struct gasnet_mxm_send_req {
    /* first two must align with _gasnete_op_t */
    uint8_t flags;                   /*  flags - type tag */
    gasnete_threadidx_t threadidx;   /*  thread that owns me */
    mxm_send_req_t       mxm_sreq;
    mxm_req_buffer_t     sendiov[2];
    gasnet_handlerarg_t  args_buf[GASNETC_MAX_ARGS];
    uint64_t             long_info[2];
    gasneti_atomic_t     ref_count;    
} gasnet_mxm_send_req_t;

extern gasnet_mxm_send_req_t * gasnetc_alloc_send_req(void);
extern void gasnetc_free_send_req(void * p_sreq);


struct gasnet_mxm_recv_req {
    struct gasnet_mxm_recv_req *next;
    mxm_recv_req_t  mxm_rreq;
};

extern gasnet_mxm_recv_req_t * gasnetc_alloc_recv_req(void);
extern void gasnetc_free_recv_req(gasnet_mxm_recv_req_t *p_rreq);

#endif /* GASNET_MXM_REQ_H */
