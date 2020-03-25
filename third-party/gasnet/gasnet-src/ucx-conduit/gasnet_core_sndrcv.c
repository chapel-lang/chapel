/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ucx-conduit/gasnet_core_sndrcv.c $
 * Description: GASNet ucx conduit implementation, transport send/receive logic
 * Copyright 2019, Mellanox Technologies, Inc. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_am.h>
#include <gasnet_ucx_req.h>
#include <gasnet_event_internal.h>

GASNETI_INLINE(gasneti_ucx_progress)
int gasnetc_ucx_progress(void);

GASNETI_INLINE(gasnetc_am_req_reset)
void gasnetc_am_req_reset(gasnetc_am_req_t *am_req);

GASNETI_INLINE(gasnetc_buffer_reset)
void gasnetc_buffer_reset(gasnetc_buffer_t *buffer);
/* ------------------------------------------------------------------------------------ */
/*
  File-scoped completion callbacks
  ================================
*/
extern int gasnetc_complete_eop(gasnete_eop_t *eop, gasnetc_comptype_t type)
{ // Advance and test the proper counter
  gasnete_op_t *op = (gasnete_op_t*)eop;
  gasnetc_atomic_val_t completed;
  gasnetc_atomic_val_t initiated;

  switch (type) {
    case gasnetc_comptype_eop_alc:
      completed = gasnetc_atomic_add(&eop->completed_alc, 1, GASNETI_ATOMIC_ACQ);
      initiated = eop->initiated_alc;
      break;
    case gasnetc_comptype_eop_put:
      completed = gasnetc_atomic_add(&eop->completed_cnt, 1, GASNETI_ATOMIC_ACQ);
      initiated = eop->initiated_cnt;
      break;
    case gasnetc_comptype_eop_get:
      completed = gasnetc_atomic_add(&eop->completed_cnt, 1, GASNETI_ATOMIC_ACQ | GASNETI_ATOMIC_REL);
      initiated = eop->initiated_cnt;
      break;
    default:
      gasneti_unreachable();
  }

  if (completed == (initiated & GASNETI_ATOMIC_MAX)) {
    switch (type) {
      case gasnetc_comptype_eop_alc:
        GASNETE_EOP_LC_FINISH(op);
        break;
      case gasnetc_comptype_eop_put:
      case gasnetc_comptype_eop_get:
        GASNETE_EOP_MARKDONE(op);
        break;
      default:
        gasneti_unreachable();
    }
    return 1;
  }
  return 0;
}
// EOP completion callbacks
extern void gasnetc_cb_eop_alc(gasnetc_atomic_val_t *p) {
  gasnete_eop_t *eop = gasneti_container_of(p, gasnete_eop_t, initiated_alc);
  gasnete_eop_check(eop);
  (void) gasnetc_complete_eop(eop, gasnetc_comptype_eop_alc);
}
extern void gasnetc_cb_eop_put(gasnetc_atomic_val_t *p) {
  gasnete_eop_t *eop = gasneti_container_of(p, gasnete_eop_t, initiated_cnt);
  gasnete_eop_check(eop);
  (void) gasnetc_complete_eop(eop, gasnetc_comptype_eop_put);
}
extern void gasnetc_cb_eop_get(gasnetc_atomic_val_t *p) {
  gasnete_eop_t *eop = gasneti_container_of(p, gasnete_eop_t, initiated_cnt);
  gasnete_eop_check(eop);
  (void) gasnetc_complete_eop(eop, gasnetc_comptype_eop_get);
}

// NAR (nbi-accessregion) completion callbacks
extern void gasnetc_cb_nar_alc(gasnetc_atomic_val_t *p) {
  gasnete_iop_t *iop = gasneti_container_of(p, gasnete_iop_t, initiated_alc_cnt);
  gasnete_iop_check(iop);
  GASNETE_IOP_CNT_FINISH_REG(iop, alc, 1, GASNETI_ATOMIC_NONE);
}
extern void gasnetc_cb_nar_put(gasnetc_atomic_val_t *p) {
  gasnete_iop_t *iop = gasneti_container_of(p, gasnete_iop_t, initiated_put_cnt);
  gasnete_iop_check(iop);
  GASNETE_IOP_CNT_FINISH_REG(iop, put, 1, GASNETI_ATOMIC_NONE);
}
extern void gasnetc_cb_nar_get(gasnetc_atomic_val_t *p) {
  gasnete_iop_t *iop = gasneti_container_of(p, gasnete_iop_t, initiated_get_cnt);
  gasnete_iop_check(iop);
  GASNETE_IOP_CNT_FINISH_REG(iop, get, 1, GASNETI_ATOMIC_REL);
}
extern void gasnetc_cb_nar_rmw(gasnetc_atomic_val_t *p) {
  gasnete_iop_t *iop = gasneti_container_of(p, gasnete_iop_t, initiated_rmw_cnt);
  gasnete_iop_check(iop);
  GASNETE_IOP_CNT_FINISH_REG(iop, rmw, 1, GASNETI_ATOMIC_NONE);
}

// IOP (non accessregion) completion callbacks
extern void gasnetc_cb_iop_alc(gasnetc_atomic_val_t *p) {
  gasnete_iop_t *iop = gasneti_container_of(p, gasnete_iop_t, initiated_alc_cnt);
  gasnete_iop_check(iop);
  GASNETE_IOP_CNT_FINISH_INT(iop, alc, 1, GASNETI_ATOMIC_NONE);
}
extern void gasnetc_cb_iop_put(gasnetc_atomic_val_t *p) {
  gasnete_iop_t *iop = gasneti_container_of(p, gasnete_iop_t, initiated_put_cnt);
  gasnete_iop_check(iop);
  GASNETE_IOP_CNT_FINISH_INT(iop, put, 1, GASNETI_ATOMIC_NONE);
}
extern void gasnetc_cb_iop_get(gasnetc_atomic_val_t *p) {
  gasnete_iop_t *iop = gasneti_container_of(p, gasnete_iop_t, initiated_get_cnt);
  gasnete_iop_check(iop);
  GASNETE_IOP_CNT_FINISH_INT(iop, get, 1, GASNETI_ATOMIC_REL);
}
extern void gasnetc_cb_iop_rmw(gasnetc_atomic_val_t *p) {
  gasnete_iop_t *iop = gasneti_container_of(p, gasnete_iop_t, initiated_rmw_cnt);
  gasnete_iop_check(iop);
  GASNETE_IOP_CNT_FINISH_INT(iop, rmw, 1, GASNETI_ATOMIC_NONE);
}

// gasnetc_counter_t completion callbacks
extern void gasnetc_cb_counter(gasnetc_atomic_val_t *cnt) {
  gasnetc_counter_t *counter = gasneti_container_of(cnt, gasnetc_counter_t, initiated);
  gasnetc_atomic_increment(&counter->completed, 0);
}
extern void gasnetc_cb_counter_rel(gasnetc_atomic_val_t *cnt) {
  gasnetc_counter_t *counter = gasneti_container_of(cnt, gasnetc_counter_t, initiated);
  gasnetc_atomic_increment(&counter->completed, GASNETI_ATOMIC_REL);
}

extern void gasnetc_counter_wait(gasnetc_counter_t *counter,
                                 int handler_context GASNETI_THREAD_FARG)
{
  const gasnetc_atomic_val_t initiated = (counter->initiated & GASNETI_ATOMIC_MAX);
  gasnetc_atomic_t * const completed = &counter->completed;

  if_pf (!gasnetc_counter_done(counter)) {
    if (handler_context) {
      do {
        GASNETI_WAITHOOK();
        gasnetc_req_poll(GASNETC_LOCK_REGULAR);
      } while (initiated != gasnetc_atomic_read(completed, 0));
    } else {
      do {
        GASNETI_WAITHOOK();
        gasnetc_req_poll_rcv(GASNETC_LOCK_REGULAR);
      } while (initiated != gasnetc_atomic_read(completed, 0));
    }
  }
}
/* ------------------------------------------------------------------------------------ */
/*
  Req/Mem pool functions
  ======================
*/
void gasnetc_am_req_pool_alloc(void)
{
  gasnetc_am_req_t *am_req;

  gasneti_list_init(&gasneti_ucx_module.am_req_pool);
  for (int i = 0; i < GASNETC_UCX_REQ_POOL_SIZE; i++) {
    GASNETI_LIST_ITEM_ALLOC(am_req, gasnetc_am_req_t, gasnetc_am_req_reset);
    gasneti_list_enq(&gasneti_ucx_module.am_req_pool, am_req);
  }
}

void gasnetc_am_req_pool_free(void)
{
  gasnetc_am_req_t *am_req;

  while(NULL != (am_req = GASNETI_LIST_POP(
                   &gasneti_ucx_module.am_req_pool, gasnetc_am_req_t))){
    gasneti_free(am_req);
  }
  gasneti_list_fini(&gasneti_ucx_module.am_req_pool);
}

void gasnetc_buffer_pool_alloc(void)
{
  gasnetc_buffer_t *buffer;

  gasneti_list_init(&gasneti_ucx_module.recv_pool);
  for (int i = 0; i < GASNETC_UCX_BUF_RECV_POOL_DFLT; i++) {
    GASNETI_LIST_ITEM_ALLOC(buffer, gasnetc_buffer_t, gasnetc_buffer_reset);
    buffer->type = GASNETC_BUF_RECV_POOL;
    gasneti_list_enq(&gasneti_ucx_module.recv_pool, buffer);
  }
  gasneti_list_init(&gasneti_ucx_module.send_pool);
  for (int i = 0; i < GASNETC_UCX_BUF_SEND_POOL_SIZE; i++) {
    GASNETI_LIST_ITEM_ALLOC(buffer, gasnetc_buffer_t, gasnetc_buffer_reset);
    buffer->type = GASNETC_BUF_SEND_POOL;
    gasneti_list_enq(&gasneti_ucx_module.send_pool, buffer);
  }
}

void gasnetc_buffer_pool_free(void)
{
  gasnetc_buffer_t *buffer;

  while(NULL != (buffer = GASNETI_LIST_POP(
                   &gasneti_ucx_module.recv_pool, gasnetc_buffer_t))){
    if (buffer->long_data_ptr) {
      gasneti_free(buffer->long_data_ptr);
    }
    gasneti_assert(buffer->type == GASNETC_BUF_RECV_POOL);
    gasneti_free(buffer);
  }
  while(NULL != (buffer = GASNETI_LIST_POP(
                   &gasneti_ucx_module.send_pool, gasnetc_buffer_t))){
    gasneti_assert(buffer->type == GASNETC_BUF_SEND_POOL);
    gasneti_free(buffer);
  }
  gasneti_list_fini(&gasneti_ucx_module.recv_pool);
  gasneti_list_fini(&gasneti_ucx_module.send_pool);
}

GASNETI_INLINE(gasnetc_am_req_get)
gasnetc_am_req_t *gasnetc_am_req_get(void)
{
  gasnetc_am_req_t *am_req;

  do {
    gasnetc_ucx_progress();

  } while (NULL == (am_req = GASNETI_LIST_POP(&gasneti_ucx_module.am_req_pool,
                                              gasnetc_am_req_t)));
  return am_req;
}

GASNETI_INLINE(gasnetc_buffer_get)
gasnetc_buffer_t *gasnetc_buffer_get(gasnetc_buf_pool_type_t type)
{
  gasnetc_buffer_t *buffer;

  if (GASNETC_BUF_RECV_POOL == type) {
    do {
      gasnetc_ucx_progress();
    } while (NULL == (buffer = GASNETI_LIST_POP(&gasneti_ucx_module.recv_pool,
                                                gasnetc_buffer_t)));
  } else {
    do {
      gasnetc_req_poll_rcv(GASNETC_LOCK_INLINE);
    } while (NULL == (buffer = GASNETI_LIST_POP(&gasneti_ucx_module.send_pool,
                                                gasnetc_buffer_t)));
  }
  gasneti_assert(buffer->type == type);

  return buffer;
}

GASNETI_INLINE(gasnetc_am_req_reset)
void gasnetc_am_req_reset(gasnetc_am_req_t *am_req)
{
  gasneti_assert(am_req && "Wrong req pointer");
  GASNETI_LIST_RESET(am_req);
  am_req->iov_count = 0;
  memset(&am_req->am_hdr, 0, sizeof(am_req->am_hdr));
}

GASNETI_INLINE(gasnetc_am_req_release)
void gasnetc_am_req_release(gasnetc_am_req_t *am_req)
{
  GASNETI_DBG_LIST_ITEM_CHECK(am_req);
  gasnetc_am_req_reset(am_req);
  gasneti_list_enq(&gasneti_ucx_module.am_req_pool, am_req);
}

GASNETI_INLINE(gasnetc_buffer_reset)
void gasnetc_buffer_reset(gasnetc_buffer_t *buffer)
{
  gasneti_assert(buffer && "Wrong req pointer");
  GASNETI_LIST_RESET(buffer);
  buffer->bytes_used = 0;
  buffer->long_data_ptr = NULL;
}

GASNETI_INLINE(gasnetc_buffer_release)
void gasnetc_buffer_release(gasnetc_buffer_t *buffer)
{
  GASNETI_DBG_LIST_ITEM_CHECK(buffer);
  gasneti_assert((buffer->type == GASNETC_BUF_RECV_POOL) ||
                 (buffer->type == GASNETC_BUF_SEND_POOL));

  /* shoud be removed when RDMA will be implemented for Long messages */
  if (NULL != buffer->long_data_ptr) {
    gasneti_free(buffer->long_data_ptr);
  }
  gasnetc_buffer_reset(buffer);
  if (buffer->type == GASNETC_BUF_SEND_POOL) {
    gasneti_list_enq(&gasneti_ucx_module.send_pool, buffer);
  } else {
    gasneti_list_enq(&gasneti_ucx_module.recv_pool, buffer);
  }
}

/*
  Format send request
  ===================
*/
GASNETI_INLINE(gasnetc_req_add_iov)
void gasnetc_req_add_iov(gasnetc_am_req_t *am_req, void *buffer, size_t nbytes)
{
  gasneti_assert((am_req->iov_count <=  GASNETC_UCX_IOV_MAX)
                 && "Wrong iov count");
  gasneti_assert(nbytes);

  am_req->sendiov[am_req->iov_count].buffer = buffer;
  am_req->sendiov[am_req->iov_count].length = nbytes;
  am_req->iov_count++;
}

GASNETI_INLINE(gasnetc_am_req_format)
void gasnetc_am_req_format(gasnetc_am_req_t *am_req,
                           gasnetc_ucx_am_type_t am_type, gex_Rank_t rank,
                           gex_AM_Index_t handler, uint8_t is_packed,
                           uint8_t is_req, int numargs,
                           va_list argptr, uint32_t nbytes,
                           void *dst_addr GASNETI_THREAD_FARG)
{
  int i;
  int args_size = sizeof(gex_AM_Arg_t) * numargs;
  int padding_size = 0;

  gasneti_assert(am_req);
  am_req->am_hdr.am_type  = am_type;
  am_req->am_hdr.handler  = handler;
  am_req->am_hdr.is_packed = is_packed;
  am_req->am_hdr.is_req   = is_req;
  am_req->am_hdr.dst      = rank;
  am_req->am_hdr.src      = gasneti_mynode;
  am_req->am_hdr.numargs  = numargs;
  am_req->am_hdr.nbytes   = nbytes;
  am_req->am_hdr.dst_addr = dst_addr;
  gasnetc_req_add_iov(am_req, &am_req->am_hdr, sizeof(am_req->am_hdr));

  gasneti_assert(args_size <= (sizeof(gex_AM_Arg_t) * GASNETC_MAX_ARGS));
  if (numargs) {
    for (i = 0; i < numargs; i++) {
      am_req->args[i] = va_arg(argptr, gex_AM_Arg_t);
    }
    gasnetc_req_add_iov(am_req, am_req->args, args_size);
  }
  if (GASNETC_UCX_AM_MEDIUM == am_type) {
    /* the payload following the arguments must be aligned
     * to GASNETI_MEDBUF_ALIGNMENT */
    padding_size =
        GASNETI_ALIGNUP(args_size + sizeof(gasnetc_sreq_hdr_t),
                        GASNETI_MEDBUF_ALIGNMENT) -
        sizeof(gasnetc_sreq_hdr_t) - args_size;
    if (padding_size) {
      gasneti_assert(padding_size <= (sizeof(gex_AM_Arg_t) * GASNETC_MAX_ARGS));
      /* use `am_req->args` for padding */
      gasnetc_req_add_iov(am_req, am_req->args, padding_size);
    }
  }
}
/* ------------------------------------------------------------------------------------ */

/*
  RMA functions
  =============
*/
gasnetc_mem_info_t * gasnetc_find_mem_info(void *addr, int nbytes, gex_Rank_t rank)
{
  // TODO-future: use UCS rcache
  gasnetc_mem_info_t *mem_info;

  GASNETI_LIST_FOREACH(mem_info, &gasneti_ucx_module.ep_tbl[rank].mem_tbl,
                       gasnetc_mem_info_t) {
    if (GASNETC_ADDR_IN_RANGE(mem_info->addr, mem_info->length, addr, nbytes)) {
      return mem_info;
    }
  }

  return NULL;
}

static
void gasnetc_ucx_rma_cb(void *request, ucs_status_t status)
{
  gasnetc_ucx_request_t *req = (gasnetc_ucx_request_t*) request;

  if (status != UCS_OK) {
    gasneti_fatalerror("UCX RDMA operation failed: %s",
                       ucs_status_string(status));
  }
  if (req->completion.cbfunc) {
    req->completion.cbfunc(req->completion.cbdata);
  }
  req->completion.cbfunc = NULL;
  req->completion.cbdata = NULL;
  ucp_request_free(request);
  return;
}

int gasnetc_ucx_putget_inner(int is_put, gex_Rank_t jobrank,
                             void *buffer, uint32_t nbytes, void *remote_addr,
                             gasnetc_atomic_val_t *local_cnt,
                             gasnetc_cbfunc_t local_cb,
                             gasnetc_atomic_val_t *remote_cnt,
                             gasnetc_cbfunc_t remote_cb)
{
  gasnetc_ucx_request_t *req;
  ucp_ep_h ep = GASNETC_UCX_GET_EP(jobrank);
  gasnetc_mem_info_t * minfo;
  int immediate = 0;

  minfo = gasnetc_find_mem_info(remote_addr, nbytes, jobrank);
  if (NULL == minfo) {
    gasneti_fatalerror("rkey cannot found");
  }
  if (local_cnt) (*local_cnt)++;
  
  req = gasnetc_putget_fn(is_put, ep, buffer, nbytes, remote_addr,
                          minfo->rkey, gasnetc_ucx_rma_cb);
  if (NULL == req) {
    /* completed inplace */
    if (local_cb) {
      local_cb(local_cnt);
    }
    immediate = 1;
  } else {
    if_pf (UCS_PTR_IS_ERR(req)) {
      gasneti_fatalerror("UCX RDMA put failed: %s",
                         ucs_status_string(UCS_PTR_STATUS(req)));
    }
    req->completion.cbdata = local_cnt;
    req->completion.cbfunc = local_cb;
  }

  if (remote_cnt) {
    (*remote_cnt)++;
    req = ucp_ep_flush_nb(ep, 0, gasnetc_ucx_rma_cb);
    if (req == NULL) {
      remote_cb(remote_cnt);
    } else {
      req->completion.cbdata = remote_cnt;
      req->completion.cbfunc = remote_cb;
    }
  }

  return immediate;
}

GASNETI_INLINE(gasnetc_ucx_am_put)
int gasnetc_ucx_am_put(gex_Rank_t jobrank, void *src_addr,
                       uint32_t nbytes, void *dest_addr,
                       gasnetc_atomic_val_t *cnt, gasnetc_cbfunc_t cbfunc)
{
  return gasnetc_ucx_putget_inner(1, jobrank, src_addr, nbytes, dest_addr,
                                  cnt, cbfunc, NULL, NULL);
}
/* ------------------------------------------------------------------------------------ */
/*
  Send/recv requests
  ==================
*/
GASNETI_INLINE(gasnetc_req_reset)
void gasnetc_req_reset(void *request)
{
  gasnetc_ucx_request_t *req = (gasnetc_ucx_request_t *) request;
  req->status = GASNETC_UCX_INIT;
  req->buffer = NULL;
  req->am_req = NULL;
  req->is_sync = 0;
}

void gasnetc_req_init(void *request)
{
  GASNETI_DBG_LIST_ITEM_SET_MAGIC(request);
  gasnetc_req_reset(request);
}

GASNETI_INLINE(gasnetc_req_free)
void gasnetc_req_free(void *req)
{
  gasnetc_ucx_request_t *request = (gasnetc_ucx_request_t *) req;

  if (request->am_req) {
    gasnetc_am_req_release(request->am_req);
  }
  if (request->buffer) {
    gasnetc_buffer_release(request->buffer);
  }
  gasnetc_req_reset(request);
  ucp_request_free(request);
}

void gasnetc_req_list_init(void)
{
  gasneti_list_init(&gasneti_ucx_module.send_list);
  gasneti_list_init(&gasneti_ucx_module.recv_list);
}

void gasnetc_rreq_list_free(void)
{
  gasnetc_ucx_request_t *request;

  while(NULL != (request = GASNETI_LIST_POP(
                   &gasneti_ucx_module.recv_list, gasnetc_ucx_request_t))){
    gasnetc_req_free(request);
  }
  gasneti_list_fini(&gasneti_ucx_module.recv_list);
}

void gasnetc_sreq_list_free(void)
{
  gasnetc_ucx_request_t *request;

  while(NULL != (request = GASNETI_LIST_POP(
                   &gasneti_ucx_module.send_list, gasnetc_ucx_request_t))){
    if (GASNETC_UCX_ACTIVE == request->status) {
      ucp_request_cancel(gasneti_ucx_module.ucp_worker, request);
      while (GASNETC_UCX_ACTIVE == request->status) {
        gasnetc_ucx_progress();
        // TODO-next: interrupt if stuck here for a long time
      }
      gasneti_list_rem(&gasneti_ucx_module.send_list, request);
      gasnetc_req_free(request);
    }
  }
  gasneti_list_fini(&gasneti_ucx_module.send_list);
}

GASNETI_INLINE(gasneti_probe_recv_complete)
gasnetc_ucx_request_t *gasneti_req_probe_complete(gasneti_list_t *req_list)
{
  gasnetc_ucx_request_t *req;

  if (NULL != (req = (gasnetc_ucx_request_t*)gasneti_list_deq(req_list))) {
    gasneti_assert(GASNETC_UCX_COMPLETE == req->status);
    gasneti_assert(req->buffer->bytes_used);
  }
  return req;
}

GASNETI_INLINE(gasnetc_req_process_nolock)
void gasnetc_req_process_nolock(gasnetc_ucx_request_t *req)
{
  gasneti_assert(GASNETC_UCX_COMPLETE == req->status);

  if (req->buffer->long_data_ptr) {
    /* shoud be removed when RDMA will be implemented for Long messages */
    gasnetc_ProcessRecv(req->buffer->long_data_ptr, req->buffer->bytes_used);
  } else {
    gasnetc_ProcessRecv(req->buffer->data, req->buffer->bytes_used);
  }
}

static void gasnetc_ucx_send_handler(void *request, ucs_status_t status)
{
  gasnetc_ucx_request_t *req = (gasnetc_ucx_request_t *)request;

  req->status = GASNETC_UCX_COMPLETE;
  if (UCS_ERR_CANCELED == status) {
    req->status = GASNETC_UCX_CANCEL;
    return;
  }
  if (req->completion.cbfunc) {
    req->completion.cbfunc(req->completion.cbdata);
  }
  req->completion.cbfunc = NULL;
  req->completion.cbdata = NULL;
  if (req->is_sync) {
    /* No cleanup here, it will be released after `gasnetc_wait_req` */
    return;
  }
exit:
  gasneti_list_rem(&gasneti_ucx_module.send_list, req);
  gasnetc_req_free(req);
}

static void gasneti_ucx_recv_handler(void *request, ucs_status_t status,
                                     ucp_tag_recv_info_t *info)
{
  gasnetc_ucx_request_t *req = (gasnetc_ucx_request_t*) request;

  if (UCS_ERR_CANCELED == status) {
    req->status = GASNETC_UCX_CANCEL;
    return;
  }
  if (status != UCS_OK) {
    req->status = GASNETC_UCX_FAILED;
    return;
  }
  /* if the request was completed immediately it has
   * `GASNETC_UCX_INIT` status and cannot be pushed
   * into the receiving pool here because of not enough data */
  if (req->status != GASNETC_UCX_INIT) {
    /* enqueue the complete request to process it later */
    gasneti_assert(info->length == req->buffer->bytes_used);
    gasneti_list_enq(&gasneti_ucx_module.recv_list, req);
  }
  req->status = GASNETC_UCX_COMPLETE;
}

GASNETI_INLINE(gasnetc_req_wait)
void gasnetc_req_wait(gasnetc_ucx_request_t *req, uint8_t is_request)
{
  while (GASNETC_UCX_ACTIVE == req->status) {
    if (is_request) {
      /* Ensure full progress only for Requests
       * to avoid recursive "poll" while receiving. */
      gasnetc_req_poll(GASNETC_LOCK_REGULAR);
    } else {
      gasnetc_req_poll_rcv(GASNETC_LOCK_REGULAR);
    }
  }
  GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  gasneti_list_rem(&gasneti_ucx_module.send_list, req);
  gasnetc_req_free(req);
  GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);
}

GASNETI_INLINE(gasnetc_send_req)
gasnetc_ucx_request_t *gasnetc_send_req(gasnetc_am_req_t *am_req,
                                        gasnetc_buffer_t *buffer,
                                        uint8_t block,
                                        gasnetc_atomic_val_t *local_cnt,
                                        gasnetc_cbfunc_t local_cb)
{
  gasnetc_ucx_request_t *request = NULL;
  ucp_ep_h server_ep =
      gasneti_ucx_module.ep_tbl[am_req->am_hdr.dst].server_ep;

  server_ep = gasneti_ucx_module.ep_tbl[am_req->am_hdr.dst].server_ep;

  gasneti_assert(am_req->iov_count &&
                 (am_req->iov_count <= GASNETC_UCX_IOV_MAX)
                 && "Wrong iov count");

  request = ucp_tag_send_nb(server_ep, (void*)am_req->sendiov,
                            (size_t)am_req->iov_count, ucp_dt_make_iov(),
                            (ucp_tag_t)gasneti_mynode, gasnetc_ucx_send_handler);
  if (NULL == request) {
      /* request was completed in place */
      if (local_cb) {
        local_cb(local_cnt);
      }
      gasnetc_am_req_release(am_req);
      if (buffer) {
        gasnetc_buffer_release(buffer);
      }
      goto exit;
  }
  if_pf (UCS_PTR_IS_ERR(request)) {
    gasnetc_am_req_release(am_req);
    if (buffer) {
      gasnetc_buffer_release(buffer);
    }
    gasneti_fatalerror("UCX recv request failed: %s",
                       ucs_status_string(UCS_PTR_STATUS(request)));
    /* Doesn't return */
  }

  request->am_req = am_req;
  request->buffer = buffer;
  request->is_sync = block;
  request->completion.cbdata = local_cnt;
  request->completion.cbfunc = local_cb;
  request->status = GASNETC_UCX_ACTIVE;
  gasneti_list_enq(&gasneti_ucx_module.send_list, request);

exit:
  return request;
}
/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
*/
int gasnetc_am_reqrep_inner(gasnetc_ucx_am_type_t am_type,
           gex_Rank_t jobrank,
           gex_AM_Index_t handler,
           gex_Flags_t flags,
           uint8_t is_request,
           uint8_t is_sync,
           int numargs,
           va_list argptr,
           void *src_addr,
           uint32_t nbytes,
           void *dst_addr,
           gasnetc_atomic_val_t *local_cnt,
           gasnetc_cbfunc_t local_cb,
           gasnetc_counter_t *counter
           GASNETI_THREAD_FARG)
{
  gasnetc_am_req_t *am_req;
  gasnetc_buffer_t *buffer = NULL;
  gasnetc_ucx_request_t *req;
  const int is_packed =
#if GASNETC_PIN_SEGMENT
      0;
#else
      1;
#endif

  GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  am_req = gasnetc_am_req_get();
  gasneti_assert(am_req);

  /* format common data */
  gasnetc_am_req_format(am_req, am_type, jobrank, handler, is_packed, is_request,
            numargs, argptr, nbytes, dst_addr GASNETI_THREAD_PASS);
  if (!nbytes) {
    goto send;
  }

  switch (am_type) {
    case GASNETC_UCX_AM_SHORT:
      break;
    case GASNETC_UCX_AM_MEDIUM: {
      gasneti_assert(src_addr);
      gasneti_assert(nbytes <= GASNETC_MAX_MED);
      /* get buffer from pool */
      buffer = gasnetc_buffer_get(GASNETC_BUF_SEND_POOL);
      gasneti_assert(buffer);
      /* pack payload */
      GASNETI_MEMCPY(buffer->data, src_addr, nbytes);
      buffer->bytes_used = nbytes;
      GASNETI_DBG_LIST_ITEM_CHECK(buffer);
      gasneti_assert(NULL == buffer->long_data_ptr);
      gasnetc_req_add_iov(am_req, buffer->data, nbytes);
      break;
    }
    case GASNETC_UCX_AM_LONG:
      gasneti_assert(src_addr);
      gasneti_assert(dst_addr);
#if GASNETC_PIN_SEGMENT
      {
        int status;
        status = gasnetc_ucx_am_put(jobrank, src_addr, nbytes, dst_addr,
            local_cnt, local_cb);
        /* reset a local completion for next operation, it is already handled */
        local_cnt = NULL;
        local_cb = NULL;
        /* checking if put status is completed inline */
        if (!status && is_sync) {
          gasneti_assert(counter);
          GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);
          gasnetc_counter_wait(counter, is_request GASNETI_THREAD_PASS);
          GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
        }
      }
#else
      buffer = gasnetc_buffer_get(GASNETC_BUF_SEND_POOL);
      gasneti_assert(buffer);
      buffer->long_data_ptr = gasneti_malloc(nbytes);
      buffer->bytes_used = nbytes;
      GASNETI_MEMCPY(buffer->long_data_ptr, src_addr, nbytes);
      gasnetc_req_add_iov(am_req, buffer->long_data_ptr, nbytes);
#endif
      break;
  }

send:
  req = gasnetc_send_req(am_req, buffer, is_sync, local_cnt, local_cb);
  GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);

  if (req && is_sync) {
    gasnetc_req_wait(req, is_request);
  }
  return GASNET_OK;
}

int gasnetc_AM_ReqRepGeneric(gasnetc_ucx_am_type_t am_type,
                             gex_Rank_t jobrank,
                             gex_AM_Index_t handler,
                             gex_Event_t *lc_opt,
                             gex_Flags_t flags,
                             uint8_t is_request,
                             int numargs,
                             va_list argptr,
                             void *src_addr,
                             uint32_t nbytes,
                             void *dst_addr
                             GASNETI_THREAD_FARG)
{
  int retval;
  gasnetc_counter_t *counter_ptr = NULL;
  gasnetc_cbfunc_t cbfunc = NULL;
  gasnetc_atomic_val_t *local_cnt = NULL;
  uint8_t is_sync = is_request;

  if (GASNETC_UCX_AM_LONG == am_type) {
#if GASNETC_PIN_SEGMENT
    gasnetc_counter_t counter = GASNETC_COUNTER_INITIALIZER;
    gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;

    counter_ptr = &counter;

    if (gasneti_leaf_is_pointer(lc_opt)) {
      gasnete_eop_t *eop = gasnete_eop_new(mythread);
      GASNETE_EOP_LC_START(eop);
      local_cnt = &eop->initiated_alc;
      cbfunc = gasnetc_cb_eop_alc;
      *lc_opt = gasneti_op_event(eop, gasnete_eop_event_alc);
      is_sync = 0;
    } else if (lc_opt == GEX_EVENT_GROUP) {
      gasnete_iop_t *iop = mythread->current_iop;
      local_cnt = &iop->initiated_alc_cnt;
      cbfunc = iop->next ? gasnetc_cb_nar_alc : gasnetc_cb_iop_alc;
      is_sync = 0;
    } else {
      gasneti_assert(lc_opt == GEX_EVENT_NOW);
      local_cnt = &counter.initiated;
      cbfunc = gasnetc_cb_counter;
      is_sync = 1;
    }
#else
    gasneti_leaf_finish(lc_opt); // synchronous local completion
#endif
  }
  retval = gasnetc_am_reqrep_inner(am_type, jobrank, handler, flags, is_request, is_sync,
                                   numargs, argptr, src_addr, nbytes, dst_addr,
                                   local_cnt, cbfunc,
                                   counter_ptr GASNETI_THREAD_PASS);
  return retval;
}

void gasnetc_ProcessRecv(void *buf, size_t size)
{
  gasnetc_sreq_hdr_t *am_hdr = (gasnetc_sreq_hdr_t*)buf;
  gex_AM_Index_t handler_id = am_hdr->handler;
  int numargs = am_hdr->numargs;
  int is_req = am_hdr->is_req;
  int is_packed = am_hdr->is_packed;
  gasnetc_ucx_am_type_t am_type = am_hdr->am_type;
  gex_AM_Arg_t *args = (gex_AM_Arg_t*)((char*)buf + sizeof(gasnetc_sreq_hdr_t));
  const gex_AM_Fn_t handler_fn = gasnetc_handler[handler_id].gex_fnptr;
  size_t nbytes = am_hdr->nbytes;
  char *data = NULL;
  gex_Token_t token_ptr = (gex_Token_t)am_hdr;

  switch(am_type) {
    case GASNETC_UCX_AM_SHORT:
      gasneti_assert(size == (sizeof(gasnetc_sreq_hdr_t) +
                              sizeof(gex_AM_Arg_t) * numargs));
      GASNETI_RUN_HANDLER_SHORT(is_req, handler_id, handler_fn, token_ptr,
                                args, numargs);
      break;
    case GASNETC_UCX_AM_MEDIUM:
      gasneti_assert(size == (GASNETI_ALIGNUP(sizeof(gasnetc_sreq_hdr_t) +
                                              sizeof(gex_AM_Arg_t) * numargs,
                                              GASNETI_MEDBUF_ALIGNMENT) +
                               nbytes));
      data = (char*)((char*)buf +
                     GASNETI_ALIGNUP(sizeof(gasnetc_sreq_hdr_t) +
                                     sizeof(gex_AM_Arg_t) * numargs,
                                     GASNETI_MEDBUF_ALIGNMENT));
      GASNETI_RUN_HANDLER_MEDIUM(is_req, handler_id, handler_fn,
                                 token_ptr, args, numargs, data, nbytes);
      break;
    case GASNETC_UCX_AM_LONG: {
      if_pf (is_packed) {
        if (am_hdr->nbytes > 0) {
          gasneti_assert(am_hdr->dst_addr);
          data = (char*)((char*)buf  + sizeof(gasnetc_sreq_hdr_t) +
                         sizeof(gex_AM_Arg_t) * numargs);
          GASNETI_MEMCPY(am_hdr->dst_addr, data, nbytes);
        }
      }
      GASNETI_RUN_HANDLER_LONG(is_req, handler_id, handler_fn, token_ptr, args,
                               numargs, am_hdr->dst_addr, nbytes);
      break;
    }
  }
}

GASNETI_INLINE(gasneti_ucx_progress)
int gasnetc_ucx_progress(void)
{
  int status;
  for(int i = 0;
      (status = ucp_worker_progress(gasneti_ucx_module.ucp_worker)) &&
        i < GASNETC_UCX_PROGRESS_CNT;
      i++);
  return status;
}

void gasnetc_req_poll_rcv(gasnetc_lock_mode_t lmode)
{
  GASNET_BEGIN_FUNCTION();
  uint32_t probe_cnt = 0, probe_max;
  gasnetc_ucx_request_t *request = NULL;
  gasnetc_buffer_t *buffer;
  void *buf_ptr = NULL;
  ucp_tag_recv_info_t info_tag;
  ucp_tag_message_h msg_tag;

  GASNETC_LOCK_ACQUIRE(lmode);
  gasnetc_ucx_progress();

  /* Make sure there are enough entries in the buffer pool to receive
   * new messages if the buffer pool is exhausted do not check
   * (ucp_tag_probe_nb) for new messages just keep them in the ucx queue. */
  probe_max = gasneti_list_size(&gasneti_ucx_module.recv_pool);
  while((probe_cnt++) < MIN(probe_max, GASNETC_UCX_RCV_REAP_MAX)) {
    gasnetc_ucx_progress();
    /* check for new messages */
    msg_tag = ucp_tag_probe_nb(gasneti_ucx_module.ucp_worker, 0, 0, 1, &info_tag);
    if (NULL == msg_tag) {
      break;
    }
    /* Message arrived */
    buffer = gasnetc_buffer_get(GASNETC_BUF_RECV_POOL);
    gasneti_assert(buffer);
    buffer->bytes_used = info_tag.length;
    if (info_tag.length > GASNETC_MAX_MED) {
      buffer->long_data_ptr = gasneti_malloc(info_tag.length);
      buf_ptr = buffer->long_data_ptr;
    } else {
      buf_ptr = buffer->data;
    }
    
    request = (gasnetc_ucx_request_t*)
        ucp_tag_msg_recv_nb(gasneti_ucx_module.ucp_worker, buf_ptr,
                            info_tag.length, ucp_dt_make_contig(1), msg_tag,
                            gasneti_ucx_recv_handler);
    if (UCS_PTR_IS_ERR(request)) {
      gasnetc_buffer_release(buffer);
      gasneti_fatalerror("UCX recv request failed: %s",
                         ucs_status_string(UCS_PTR_STATUS(request)));
      /* gasneti_fatalerror doesn't return */
    }
    /* fill in the info for the request*/
    request->buffer = buffer;
    
    if (GASNETC_UCX_COMPLETE == request->status) {
      /* request was completed in place
       * and wasn't added to the receiving pool in the UCX recv handler,
       * so add it to local recv pool here */
      gasneti_list_enq(&gasneti_ucx_module.recv_list, request);
      continue;
    }
    request->status = GASNETC_UCX_ACTIVE;
  }
  GASNETC_LOCK_RELEASE(lmode);
#if GASNET_PSHM
  if (lmode == GASNETC_LOCK_REGULAR) {
    gasneti_AMPSHMPoll(0 GASNETI_THREAD_PASS);
  } else if (lmode == GASNETC_LOCK_INLINE) {
    /* `gasneti_AMPSHMPoll` should be called outside the lock */
    GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);
    gasneti_AMPSHMPoll(0 GASNETI_THREAD_PASS);
    GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  }
#endif
}

int gasnetc_req_poll(gasnetc_lock_mode_t lmode)
{
  GASNET_BEGIN_FUNCTION();
  int recv_list_size = 0;
  gasnetc_ucx_request_t *request = NULL;
  gasneti_list_t local_recv_list;

  GASNETC_LOCK_ACQUIRE(lmode);
  /* poll recv requests and push to the receive queue */
  gasnetc_req_poll_rcv(GASNETC_LOCK_INLINE);
  gasnetc_ucx_progress();

  recv_list_size = gasneti_list_size(&gasneti_ucx_module.recv_list);
  if (!recv_list_size) {
    goto exit;
  }
  gasneti_list_init(&local_recv_list);
  for (int i = 0; (i < GASNETC_UCX_MSG_HNDL_PER_POLL) &&
       (NULL != (request =
                 gasneti_req_probe_complete(&gasneti_ucx_module.recv_list)));
       i++) {
    gasneti_list_enq(&local_recv_list, request);
  }
  GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);
  /* handle recv messages */
  GASNETI_LIST_FOREACH(request, &local_recv_list, gasnetc_ucx_request_t) {
    gasneti_assert(request->buffer->bytes_used);
    gasnetc_req_process_nolock(request);
  }
  GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  /* release */
  while(NULL !=
        (request = GASNETI_LIST_POP(&local_recv_list, gasnetc_ucx_request_t))) {
    gasnetc_req_free(request);
  }
  gasneti_list_fini(&local_recv_list);

exit:
  GASNETC_LOCK_RELEASE(lmode);
  return recv_list_size;
}

void gasnetc_send_list_wait(gasnetc_lock_mode_t lmode)
{
  size_t send_size;
  do {
    GASNETI_WAITHOOK();
    GASNETC_LOCK_ACQUIRE(lmode);
    gasnetc_req_poll(GASNETC_LOCK_INLINE);
    send_size = gasneti_list_size(&gasneti_ucx_module.send_list);
    GASNETC_LOCK_RELEASE(lmode);
  } while (send_size);

  GASNETI_SAFE(gasnet_barrier(0, GASNET_BARRIERFLAG_UNNAMED));
}
/* ------------------------------------------------------------------------------------ */
