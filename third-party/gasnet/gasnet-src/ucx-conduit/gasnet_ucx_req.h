/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ucx-conduit/gasnet_ucx_req.h $
 * Description: GASNet ucx conduit implementation, transport send/receive logic
 * Copyright 2019-2020, Mellanox Technologies, Inc. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#ifndef GASNET_UCX_REQ_H
#define GASNET_UCX_REQ_H

#include <gasnet_core_internal.h>

#define GASNETC_UCX_PROGRESS_CNT          8
#define GASNETC_UCX_RCV_REAP_MAX          10
#define GASNETC_UCX_MSG_HNDL_PER_POLL     10
#define GASNETC_UCX_BUF_SEND_POOL_SIZE    8
#define GASNETC_UCX_BUF_RECV_POOL_DFLT    GASNETC_UCX_RCV_REAP_MAX
#define GASNETC_UCX_REQ_POOL_SIZE         GASNETC_UCX_BUF_SEND_POOL_SIZE

#if !GASNETC_PIN_SEGMENT
#define GASNETC_UCX_IOV_MAX               2
#endif

typedef enum {
  GASNETC_UCX_INIT = 0,
  GASNETC_UCX_ACTIVE,
  GASNETC_UCX_COMPLETE,
  GASNETC_UCX_CANCEL,
  GASNETC_UCX_FAILED
} gasnetc_ucx_req_status_t;

#if GASNET_DEBUG
#define GASNETC_HDR_MAGIC 0x11223344
#endif

typedef struct {
#if GASNET_DEBUG
  int magic;
#endif
  gex_Rank_t            dst;
  gex_Rank_t            src;
  gex_AM_Index_t        handler;
  gasnetc_ucx_am_type_t am_type;
  uint8_t               is_req   : 1;
  uint8_t               is_packed: 1;
  uint8_t               numargs  : 5;
  uint32_t              size;
  uint32_t              payload_size;
  /* for Long AM only */
  void                 *dst_addr;
} gasnetc_sreq_hdr_t;

typedef struct {
  GASNETC_LIST_CLASS;
  char  *data;
  size_t bytes_used;
#if !GASNETC_PIN_SEGMENT
  char  *long_data_ptr;
  size_t long_bytes_used;
#endif
} gasnetc_buffer_t;

typedef struct {
  GASNETC_LIST_CLASS;
  gasnetc_sreq_hdr_t   *am_hdr;
  gex_AM_Arg_t         *args;
  gasnetc_buffer_t      buffer;
#if !GASNETC_PIN_SEGMENT
  ucp_dt_iov_t          sendiov[GASNETC_UCX_IOV_MAX];
  uint32_t              iov_count;
#endif
} gasnetc_am_req_t;

typedef struct {
    GASNETC_LIST_CLASS;
    gasnetc_ucx_req_status_t  status;
    gasnetc_am_req_t         *am_req;
    gasnetc_buffer_t          buffer;
    uint8_t                   is_sync;
    int                       is_packed;
    void                     *result_p;
    struct {
      gasnetc_cbfunc_t        cbfunc;
      gasneti_atomic_val_t   *cbdata;
    } completion;
    ucs_status_t              ucs_status;
} gasnetc_ucx_request_t;

#define GASNETC_BUF_DATA(__buf)               ((__buf).data)
#define GASNETC_BUF_PTR(__buf)                ((__buf).data + (__buf).bytes_used)
#define GASNETC_BUF_SIZE(__buf)               ((__buf).bytes_used)
#define GASNETC_BUF_SET_OFFSET(__buf, __offs) ((__buf).bytes_used = (__offs))

#define GASNETC_AM_HDR_ADD_BYTES(__req, __bytes) \
  ((__req)->am_hdr->size += __bytes)

#define GASNETC_BUF_ADD_SEND_BYTES(__req, __offs) \
do {                                              \
  GASNETC_AM_HDR_ADD_BYTES(__req, __offs);        \
  (__req)->buffer.bytes_used += (__offs);         \
} while(0)


#if GASNETC_PIN_SEGMENT
#define GASNETC_BUF_RESET(__buf)              ((__buf).bytes_used = 0)
#else
#define GASNETC_BUF_LDATA(__buf)              ((__buf).long_data_ptr)
#define GASNETC_BUF_LSIZE(__buf)              ((__buf).long_bytes_used)
#define GASNETC_BUF_RESET(__buf)              \
do {                                          \
  (__buf).bytes_used = 0;                     \
  (__buf).long_bytes_used = 0;                \
} while(0)
#endif

extern void gasnetc_req_init(void *request);
extern int  gasnetc_recv_init(void);
extern void gasnetc_recv_fini(void);

#endif /* GASNET_UCX_REQ_H */
