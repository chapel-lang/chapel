/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ucx-conduit/gasnet_ucx_req.h $
 * Description: GASNet ucx conduit implementation, transport send/receive logic
 * Copyright 2019, Mellanox Technologies, Inc. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#ifndef GASNET_UCX_REQ_H
#define GASNET_UCX_REQ_H

#include <gasnet_core_internal.h>

#define GASNETC_UCX_PROGRESS_CNT          8
#define GASNETC_UCX_RCV_REAP_MAX          10
#define GASNETC_UCX_MSG_HNDL_PER_POLL     10
#define GASNETC_UCX_IOV_MAX               4
#define GASNETC_UCX_BUF_SEND_POOL_SIZE    8
#define GASNETC_UCX_BUF_RECV_POOL_DFLT    GASNETC_UCX_RCV_REAP_MAX
#define GASNETC_UCX_REQ_POOL_SIZE         GASNETC_UCX_BUF_SEND_POOL_SIZE

typedef enum {
  GASNETC_UCX_INIT = 0,
  GASNETC_UCX_ACTIVE,
  GASNETC_UCX_COMPLETE,
  GASNETC_UCX_CANCEL,
  GASNETC_UCX_FAILED
} gasnetc_ucx_req_status_t;

typedef struct {
  gex_Rank_t            dst;
  gex_Rank_t            src;
  gex_AM_Index_t        handler;
  gasnetc_ucx_am_type_t am_type;
  uint8_t               is_req   : 1;
  uint8_t               is_packed: 1;
  uint8_t               numargs  : 5;
  uint32_t              nbytes;
  /* for Long AM only */
  void                 *dst_addr;
} gasnetc_sreq_hdr_t;

typedef enum {
  GASNETC_BUF_RECV_POOL,
  GASNETC_BUF_SEND_POOL
} gasnetc_buf_pool_type_t;

typedef struct {
  GASNETC_LIST_CLASS;
  char                    data[GASNETC_MAX_MED];
  /* shoud be removed when RDMA will be implemented for Long messages */
  void                   *long_data_ptr;
  size_t                  bytes_used;
  gasnetc_buf_pool_type_t type;
} gasnetc_buffer_t;

typedef struct {
  GASNETC_LIST_CLASS;
  gasnetc_sreq_hdr_t   am_hdr;
  gex_AM_Arg_t         args[GASNETC_MAX_ARGS];
  ucp_dt_iov_t         sendiov[GASNETC_UCX_IOV_MAX];
  uint32_t             iov_count;
} gasnetc_am_req_t;

typedef struct {
    GASNETC_LIST_CLASS;
    gasnetc_ucx_req_status_t  status;
    gasnetc_am_req_t         *am_req;
    gasnetc_buffer_t         *buffer;
    uint8_t                   is_sync;
    int                       is_packed;
    void                     *result_p;
    struct {
      gasnetc_cbfunc_t        cbfunc;
      gasneti_atomic_val_t   *cbdata;
    } completion;
} gasnetc_ucx_request_t;

extern void gasnetc_req_init(void *request);

#endif /* GASNET_UCX_REQ_H */
