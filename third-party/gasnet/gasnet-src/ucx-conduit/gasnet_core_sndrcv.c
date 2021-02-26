/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ucx-conduit/gasnet_core_sndrcv.c $
 * Description: GASNet ucx conduit implementation, transport send/receive logic
 * Copyright 2019-2020, Mellanox Technologies, Inc. All rights reserved.
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

GASNETI_INLINE(gasnetc_req_free)
void gasnetc_req_free(void *req);

GASNETI_INLINE(gasnetc_am_req_release)
void gasnetc_rreq_release(gasnetc_ucx_request_t *req);
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
                                 int outside_handler_context GASNETI_THREAD_FARG)
{
  const gasnetc_atomic_val_t initiated = (counter->initiated & GASNETI_ATOMIC_MAX);
  gasnetc_atomic_t * const completed = &counter->completed;

  if_pf (!gasnetc_counter_done(counter)) {
    if (outside_handler_context) {
      do {
        GASNETI_WAITHOOK();
        gasnetc_poll_sndrcv(GASNETC_LOCK_REGULAR GASNETI_THREAD_PASS);
      } while (initiated != gasnetc_atomic_read(completed, 0));
    } else {
      do {
        GASNETI_WAITHOOK();
        gasnetc_poll_snd(GASNETC_LOCK_REGULAR GASNETI_THREAD_PASS);
      } while (initiated != gasnetc_atomic_read(completed, 0));
    }
  }
}
/* ------------------------------------------------------------------------------------ */
/*
  Req/Mem pool functions
  ======================
*/
void gasnetc_send_init(void)
{
  gasnetc_am_req_t *am_req;

  gasneti_list_init(&gasneti_ucx_module.send_queue);
  gasneti_list_init(&gasneti_ucx_module.sreq_free);

  for (int i = 0; i < GASNETC_UCX_REQ_POOL_SIZE; i++) {
    GASNETI_LIST_ITEM_ALLOC(am_req, gasnetc_am_req_t, gasnetc_am_req_reset);
    am_req->buffer.data = gasneti_malloc_aligned(GASNETI_MEDBUF_ALIGNMENT,
                                                 GASNETC_MAX_MED);
    GASNETC_BUF_RESET(am_req->buffer);
    gasneti_list_enq(&gasneti_ucx_module.sreq_free, am_req);
  }
}

void gasnetc_send_fini(void)
{
  gasnetc_am_req_t *am_req;
  gasnetc_ucx_request_t *request;

  /* wait and release queued send requests */
  while(NULL != (request = GASNETI_LIST_POP(
                   &gasneti_ucx_module.send_queue, gasnetc_ucx_request_t))){
    if (GASNETC_UCX_ACTIVE == request->status) {
      ucp_request_cancel(gasneti_ucx_module.ucp_worker, request);
      while (GASNETC_UCX_ACTIVE == request->status) {
        gasnetc_ucx_progress();
        // TODO-next: interrupt if stuck here for a long time
      }
      gasneti_list_rem(&gasneti_ucx_module.send_queue, request);
      gasnetc_req_free(request);
    }
  }
  gasneti_list_fini(&gasneti_ucx_module.send_queue);

  /* release pool of send requests */
  while(NULL != (am_req = GASNETI_LIST_POP(
                   &gasneti_ucx_module.sreq_free, gasnetc_am_req_t))){
    gasneti_free_aligned(am_req->buffer.data);
    gasneti_free(am_req);
  }
  gasneti_list_fini(&gasneti_ucx_module.sreq_free);
}

GASNETI_INLINE(gasnetc_am_req_get)
gasnetc_am_req_t *gasnetc_am_req_get(void)
{
  gasnetc_am_req_t *am_req;

  do {
    gasnetc_ucx_progress();
  } while (NULL == (am_req = GASNETI_LIST_POP(&gasneti_ucx_module.sreq_free,
                                              gasnetc_am_req_t)));
  return am_req;
}

GASNETI_INLINE(gasnetc_am_req_reset)
void gasnetc_am_req_reset(gasnetc_am_req_t *am_req)
{
  gasneti_assert(am_req && "Wrong req pointer");
  GASNETI_LIST_RESET(am_req);
#if !GASNETC_PIN_SEGMENT
  am_req->iov_count = 0;
#endif
  GASNETC_BUF_RESET(am_req->buffer);
  memset(&am_req->am_hdr, 0, sizeof(am_req->am_hdr));
}

GASNETI_INLINE(gasnetc_am_req_release)
void gasnetc_am_req_release(gasnetc_am_req_t *am_req)
{
  GASNETI_DBG_LIST_ITEM_CHECK(am_req);
#if !GASNETC_PIN_SEGMENT
  if (am_req->buffer.long_data_ptr) {
    gasneti_free(am_req->buffer.long_data_ptr);
    am_req->buffer.long_data_ptr = NULL;
  }
#endif
  gasnetc_am_req_reset(am_req);
  gasneti_list_enq(&gasneti_ucx_module.sreq_free, am_req);
}

/*
  Format send request
  ===================
*/
#if !GASNETC_PIN_SEGMENT
GASNETI_INLINE(gasnetc_req_add_iov)
void gasnetc_req_add_iov(gasnetc_am_req_t *am_req, void *buffer, size_t nbytes)
{
  gasneti_assert((am_req->iov_count !=  GASNETC_UCX_IOV_MAX)
                 && "Wrong iov count");
  gasneti_assert(nbytes);

  am_req->sendiov[am_req->iov_count].buffer = buffer;
  am_req->sendiov[am_req->iov_count].length = nbytes;
  am_req->iov_count++;
  GASNETC_AM_HDR_ADD_BYTES(am_req, nbytes);
}
#endif

GASNETI_INLINE(gasnetc_am_req_format)
void gasnetc_am_req_format(gasnetc_am_req_t *am_req,
                           gasnetc_ucx_am_type_t am_type, gex_Rank_t rank,
                           gex_AM_Index_t handler, uint8_t is_packed,
                           uint8_t is_req, int numargs,
                           va_list argptr, uint32_t nbytes,
                           void *dst_addr GASNETI_THREAD_FARG)
{
  int i;
  int padding_size = 0;

  gasneti_assert(am_req);
  gasneti_assert(am_req->buffer.data);
  gasneti_assert(0 == GASNETC_BUF_SIZE(am_req->buffer));

  am_req->am_hdr = (gasnetc_sreq_hdr_t*)GASNETC_BUF_PTR(am_req->buffer);
  am_req->am_hdr->size = 0;

  GASNETC_BUF_ADD_SEND_BYTES(am_req, sizeof(gasnetc_sreq_hdr_t));

#if GASNET_DEBUG
  am_req->am_hdr->magic = GASNETC_HDR_MAGIC;
#endif
  am_req->am_hdr->am_type  = am_type;
  am_req->am_hdr->handler  = handler;
  am_req->am_hdr->is_packed = is_packed;
  am_req->am_hdr->is_req   = is_req;
  am_req->am_hdr->dst      = rank;
  am_req->am_hdr->src      = gasneti_mynode;
  am_req->am_hdr->numargs  = numargs;
  am_req->am_hdr->payload_size = nbytes;
  am_req->am_hdr->dst_addr = dst_addr;

  am_req->args = (gex_AM_Arg_t*)GASNETC_BUF_PTR(am_req->buffer);
  GASNETC_BUF_ADD_SEND_BYTES(am_req, GASNETC_ARGS_SIZE(numargs));

  gasneti_assert(GASNETC_ARGS_SIZE(numargs) <= GASNETC_MAX_ARGS_SIZE);
  if (numargs) {
    for (i = 0; i < numargs; i++) {
      am_req->args[i] = va_arg(argptr, gex_AM_Arg_t);
    }
  }
  if (GASNETC_UCX_AM_MEDIUM == am_type) {
    /* the payload following the arguments must be aligned
     * to GASNETI_MEDBUF_ALIGNMENT */
    padding_size = GASNETC_AMMED_PADDING_SIZE(numargs);
    if (padding_size) {
      gasneti_assert(padding_size < GASNETI_MEDBUF_ALIGNMENT);
      /* use `am_req->args` for padding */
      GASNETC_BUF_ADD_SEND_BYTES(am_req, padding_size);
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
  gasnetc_req_reset(request);
  ucp_request_free(request);
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
  gasneti_list_rem(&gasneti_ucx_module.send_queue, req);
  gasnetc_req_free(req);
}

GASNETI_INLINE(gasnetc_req_wait)
void gasnetc_req_wait(gasnetc_ucx_request_t *req, uint8_t is_request
                      GASNETI_THREAD_FARG)
{
  while (GASNETC_UCX_ACTIVE == req->status) {
    if (is_request) {
      /* Ensure full progress only for Requests
       * to avoid recursive "poll" while receiving. */
      gasnetc_poll_sndrcv(GASNETC_LOCK_REGULAR GASNETI_THREAD_PASS);
    } else {
      gasnetc_poll_snd(GASNETC_LOCK_REGULAR GASNETI_THREAD_PASS);
    }
  }
  GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  gasneti_list_rem(&gasneti_ucx_module.send_queue, req);
  gasnetc_req_free(req);
  GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);
}

GASNETI_INLINE(gasnetc_send_req)
gasnetc_ucx_request_t *gasnetc_send_req(gasnetc_am_req_t *am_req,
                                        uint8_t block,
                                        gasnetc_atomic_val_t *local_cnt,
                                        gasnetc_cbfunc_t local_cb)
{
  gasnetc_ucx_request_t *request = NULL;
  ucp_ep_h server_ep =
      gasneti_ucx_module.ep_tbl[am_req->am_hdr->dst].server_ep;
  void *src_ptr;
  size_t count;
  ucp_datatype_t datatype;

  server_ep = gasneti_ucx_module.ep_tbl[am_req->am_hdr->dst].server_ep;

#if GASNETC_PIN_SEGMENT
  src_ptr = (void*)GASNETC_BUF_DATA(am_req->buffer);
  count = GASNETC_BUF_SIZE(am_req->buffer);
  datatype = ucp_dt_make_contig(1);
#else
  if_pt (!am_req->iov_count) {
    src_ptr = (void*)GASNETC_BUF_DATA(am_req->buffer);
    count = GASNETC_BUF_SIZE(am_req->buffer);
    datatype = ucp_dt_make_contig(1);
  } else {
    gasneti_assert((am_req->iov_count <= GASNETC_UCX_IOV_MAX)
                   && "Wrong iov count");
    src_ptr = (void*)am_req->sendiov;
    count = (size_t)am_req->iov_count;
    datatype = ucp_dt_make_iov();
  }
#endif

  if (local_cnt) (*local_cnt)++;

  request = ucp_tag_send_nb(server_ep, src_ptr, count, datatype,
      (ucp_tag_t)gasneti_mynode, gasnetc_ucx_send_handler);
  if_pf (UCS_PTR_IS_ERR(request)) {
    gasnetc_am_req_release(am_req);
    gasneti_fatalerror("UCX recv request failed: %s",
                       ucs_status_string(UCS_PTR_STATUS(request)));
    /* Doesn't return */
  }
  if (NULL == request) {
      /* request was completed in place */
      if (local_cb) {
        local_cb(local_cnt);
      }
      gasnetc_am_req_release(am_req);
      goto exit;
  }

  request->am_req = am_req;
  request->is_sync = block;
  request->completion.cbdata = local_cnt;
  request->completion.cbfunc = local_cb;
  request->status = GASNETC_UCX_ACTIVE;
  gasneti_list_enq(&gasneti_ucx_module.send_queue, request);

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
  gasnetc_ucx_request_t *req;

  GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  am_req = gasnetc_am_req_get();
  gasneti_assert(am_req);

#define __am_req_format(__is_packed) \
  gasnetc_am_req_format(am_req, am_type, jobrank, handler, __is_packed, \
                        is_request, numargs, argptr, nbytes, dst_addr \
                        GASNETI_THREAD_PASS)

  if (!nbytes) {
    /* GASNETC_UCX_AM_SHORT always goes this way */
    __am_req_format(0);
    goto send;
  }

  switch (am_type) {
    case GASNETC_UCX_AM_SHORT :
      /* only to silence the warning */
      break;
    case GASNETC_UCX_AM_MEDIUM: {
      __am_req_format(0);
      gasneti_assert(src_addr);
      gasneti_assert(nbytes <= GASNETC_MAX_MED);
      /* pack payload */
      GASNETI_MEMCPY(GASNETC_BUF_PTR(am_req->buffer), src_addr, nbytes);
      GASNETC_BUF_ADD_SEND_BYTES(am_req, nbytes);
      break;
    }
    case GASNETC_UCX_AM_LONG:
      gasneti_assert(src_addr);
      gasneti_assert(dst_addr);
#if GASNETC_PIN_SEGMENT
      {
        int status;
        __am_req_format(0);
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
      __am_req_format(1);
      if (nbytes <= GASNETC_MAX_MED_(numargs)) {
        GASNETI_MEMCPY(GASNETC_BUF_PTR(am_req->buffer), src_addr, nbytes);
        GASNETC_BUF_ADD_SEND_BYTES(am_req, nbytes);
      } else {
        gasnetc_req_add_iov(am_req, GASNETC_BUF_DATA(am_req->buffer),
                            GASNETC_BUF_SIZE(am_req->buffer));
        am_req->buffer.long_data_ptr = gasneti_malloc(nbytes);
        GASNETI_MEMCPY(am_req->buffer.long_data_ptr, src_addr, nbytes);
        am_req->buffer.long_bytes_used = nbytes;
        gasnetc_req_add_iov(am_req, am_req->buffer.long_data_ptr, nbytes);
      }
#endif
      break;
  }

send:
  req = gasnetc_send_req(am_req, is_sync, local_cnt, local_cb);
  GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);

  if (req && is_sync) {
    gasnetc_req_wait(req, is_request GASNETI_THREAD_PASS);
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
#if !GASNETC_PIN_SEGMENT
  int is_packed = am_hdr->is_packed;
#endif
  gasnetc_ucx_am_type_t am_type = am_hdr->am_type;
  gex_AM_Arg_t *args = (gex_AM_Arg_t*)((char*)buf + sizeof(gasnetc_sreq_hdr_t));
  const gex_AM_Fn_t handler_fn = gasnetc_handler[handler_id].gex_fnptr;
  size_t nbytes = am_hdr->payload_size;
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
#if !GASNETC_PIN_SEGMENT
      if_pf (is_packed) {
        if (am_hdr->payload_size > 0) {
          gasneti_assert(am_hdr->dst_addr);
          data = (char*)((char*)buf + sizeof(gasnetc_sreq_hdr_t) +
                         sizeof(gex_AM_Arg_t) * numargs);
          GASNETI_MEMCPY(am_hdr->dst_addr, data, nbytes);
        }
      }
#endif
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

GASNETI_INLINE(gasnetc_recv_post)
void gasnetc_recv_post(gasnetc_ucx_request_t *req) {
  GASNETI_DBG_LIST_ITEM_CHECK(req);
  gasnetc_req_reset(req);
  req->ucs_status =
      ucp_tag_recv_nbr(gasneti_ucx_module.ucp_worker, req->buffer.data,
                       GASNETC_MAX_MED, ucp_dt_make_contig(1), 0, 0,
                       req);
  if_pf (req->ucs_status < 0) {
    gasneti_fatalerror("UCX post request failed: %s",
                       ucs_status_string(UCS_PTR_STATUS(req->ucs_status)));
  }
}

#if GASNETC_PIN_SEGMENT
int gasnetc_recv_init(void)
{
  int i;
  gasnetc_ucx_request_t *req;
  ucp_context_attr_t attr;
  ucs_status_t status;

  gasneti_list_init(&gasneti_ucx_module.recv_queue);

  attr.field_mask = UCP_ATTR_FIELD_REQUEST_SIZE;
  status = ucp_context_query(gasneti_ucx_module.ucp_context, &attr);
  gasneti_ucx_module.request_size = attr.request_size;

  for (i = 0; i < GASNETC_UCX_RCV_REAP_MAX; i++) {
    void *ucx_req = gasneti_malloc(gasneti_ucx_module.request_size +
                                   sizeof(gasnetc_ucx_request_t));
    req = (gasnetc_ucx_request_t*)
        (((char*) ucx_req) + gasneti_ucx_module.request_size);

    gasnetc_req_init(req);
    req->buffer.data = gasneti_malloc_aligned(GASNETI_MEDBUF_ALIGNMENT,
                                              GASNETC_MAX_MED);
    gasneti_list_enq(&gasneti_ucx_module.recv_queue, req);
    gasnetc_recv_post(req);
  }

  return GASNET_OK;
}

void gasnetc_recv_fini(void)
{
  gasnetc_ucx_request_t *req;
  void *ucx_req;

  while(NULL != (req = GASNETI_LIST_POP(&gasneti_ucx_module.recv_queue,
                                        gasnetc_ucx_request_t))) {
    ucx_req = ((char*) req) - gasneti_ucx_module.request_size;
    gasneti_free_aligned(req->buffer.data);
    gasneti_free(ucx_req);
  }
  gasneti_list_fini(&gasneti_ucx_module.recv_queue);
}

#else
int gasnetc_recv_init(void)
{
  int i;
  gasnetc_am_req_t *rreq;

  gasneti_list_init(&gasneti_ucx_module.recv_queue);
  gasneti_list_init(&gasneti_ucx_module.rreq_free);

  for (i = 0; i < GASNETC_UCX_RCV_REAP_MAX; i++) {
    GASNETI_LIST_ITEM_ALLOC(rreq, gasnetc_am_req_t, gasnetc_am_req_reset);
    rreq->buffer.data = gasneti_malloc_aligned(GASNETI_MEDBUF_ALIGNMENT,
                                               GASNETC_MAX_MED);
    rreq->buffer.long_data_ptr = NULL;
    GASNETC_BUF_RESET(rreq->buffer);
    gasneti_list_enq(&gasneti_ucx_module.rreq_free, rreq);
  }

  return GASNET_OK;
}

void gasnetc_recv_fini(void)
{
  gasnetc_am_req_t *rreq;
  gasnetc_ucx_request_t *ucx_req;

  while (NULL != (ucx_req = GASNETI_LIST_POP(&gasneti_ucx_module.recv_queue,
                                             gasnetc_ucx_request_t))) {
    gasnetc_rreq_release(ucx_req);
  }
  gasneti_list_fini(&gasneti_ucx_module.recv_queue);

  while (NULL != (rreq = GASNETI_LIST_POP(&gasneti_ucx_module.rreq_free,
                                          gasnetc_am_req_t))) {
    gasneti_free_aligned(rreq->buffer.data);
#if !GASNETC_PIN_SEGMENT
    gasneti_assert(!rreq->buffer.long_bytes_used);
    gasneti_assert(!rreq->buffer.long_data_ptr);
#endif
    gasneti_free(rreq);
  }
  gasneti_list_fini(&gasneti_ucx_module.rreq_free);
}

#endif

#if GASNETC_PIN_SEGMENT
int gasnetc_poll_sndrcv(gasnetc_lock_mode_t lmode GASNETI_THREAD_FARG)
{
  gasnetc_ucx_request_t *req = NULL;
  gasnetc_sreq_hdr_t *am_hdr;

  GASNETC_LOCK_ACQUIRE(lmode);
  gasnetc_ucx_progress();

  req = gasneti_list_head(&gasneti_ucx_module.recv_queue);
  while (ucp_request_is_completed(req)) {
    req = GASNETI_LIST_POP(&gasneti_ucx_module.recv_queue,
                           gasnetc_ucx_request_t);
    am_hdr = (gasnetc_sreq_hdr_t*)req->buffer.data;
    GASNETC_BUF_SET_OFFSET(req->buffer, am_hdr->size);
    gasnetc_ProcessRecv(GASNETC_BUF_DATA(req->buffer),
                        GASNETC_BUF_SIZE(req->buffer));
    gasnetc_recv_post(req);
    gasneti_list_enq(&gasneti_ucx_module.recv_queue, req);
  }
  gasnetc_ucx_progress();
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
  return 0;
}

void gasnetc_poll_snd(gasnetc_lock_mode_t lmode GASNETI_THREAD_FARG)
{
  GASNETC_LOCK_ACQUIRE(lmode);
  gasnetc_ucx_progress();
  GASNETC_LOCK_RELEASE(lmode);
}

#else
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
    gasneti_assert(req->am_req &&
                   ((info->length == GASNETC_BUF_SIZE(req->am_req->buffer)) ||
                    (info->length == GASNETC_BUF_LSIZE(req->am_req->buffer))));
    gasneti_list_enq(&gasneti_ucx_module.recv_queue, req);
  }
  req->status = GASNETC_UCX_COMPLETE;
}

void gasnetc_poll_snd(gasnetc_lock_mode_t lmode GASNETI_THREAD_FARG)
{
  uint32_t probe_cnt = 0, probe_max;
  gasnetc_ucx_request_t *request = NULL;
  void *buf_ptr = NULL;
  ucp_tag_recv_info_t info_tag;
  ucp_tag_message_h msg_tag;
  gasnetc_am_req_t *rreq;

  GASNETC_LOCK_ACQUIRE(lmode);
  gasnetc_ucx_progress();

  /* Make sure there are enough entries in the buffer pool to receive
   * new messages if the buffer pool is exhausted do not check
   * (ucp_tag_probe_nb) for new messages just keep them in the ucx queue. */
  probe_max = gasneti_list_size(&gasneti_ucx_module.rreq_free);
  while((probe_cnt++) < MIN(probe_max, GASNETC_UCX_RCV_REAP_MAX)) {
    gasnetc_ucx_progress();
    /* check for new messages */
    msg_tag = ucp_tag_probe_nb(gasneti_ucx_module.ucp_worker, 0, 0, 1, &info_tag);
    if (NULL == msg_tag) {
      break;
    }
    /* Message arrived */
    rreq = GASNETI_LIST_POP(&gasneti_ucx_module.rreq_free, gasnetc_am_req_t);

    if (info_tag.length > GASNETC_MAX_MED) {
      rreq->buffer.long_data_ptr = gasneti_malloc(info_tag.length);
      rreq->buffer.long_bytes_used = info_tag.length;
      buf_ptr = rreq->buffer.long_data_ptr;
    } else {
      rreq->buffer.bytes_used = info_tag.length;
      buf_ptr = rreq->buffer.data;
    }

    request = (gasnetc_ucx_request_t*)
        ucp_tag_msg_recv_nb(gasneti_ucx_module.ucp_worker, buf_ptr,
                            info_tag.length, ucp_dt_make_contig(1), msg_tag,
                            gasneti_ucx_recv_handler);
    if (UCS_PTR_IS_ERR(request)) {
      gasneti_fatalerror("UCX recv request failed: %s",
                         ucs_status_string(UCS_PTR_STATUS(request)));
      /* gasneti_fatalerror doesn't return */
    }
    /* fill in the info for the request*/
    request->am_req = rreq;

    if (GASNETC_UCX_COMPLETE == request->status) {
      /* request was completed in place
       * and wasn't added to the receiving pool in the UCX recv handler,
       * so add it to local recv pool here */
      gasneti_list_enq(&gasneti_ucx_module.recv_queue, request);
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

GASNETI_INLINE(gasneti_probe_recv_complete)
gasnetc_ucx_request_t *gasneti_req_probe_complete(gasneti_list_t *req_list)
{
  gasnetc_ucx_request_t *req;

  if (NULL != (req = (gasnetc_ucx_request_t*)gasneti_list_deq(req_list))) {
    gasneti_assert(GASNETC_UCX_COMPLETE == req->status);
    gasneti_assert(req->am_req->buffer.bytes_used ||
                   req->am_req->buffer.long_bytes_used);
  }
  return req;
}

GASNETI_INLINE(gasnetc_rreq_release)
void gasnetc_rreq_release(gasnetc_ucx_request_t *req)
{
  gasnetc_am_req_t *am_req = req->am_req;
  if (GASNETC_BUF_LSIZE(am_req->buffer)) {
    gasneti_free(GASNETC_BUF_LDATA(am_req->buffer));
    GASNETC_BUF_LDATA(am_req->buffer) = NULL;
  }
  GASNETC_BUF_RESET(am_req->buffer);
  gasneti_list_enq(&gasneti_ucx_module.rreq_free, am_req);
  gasnetc_req_reset(req);
  ucp_request_release(req);
}

int gasnetc_poll_sndrcv(gasnetc_lock_mode_t lmode GASNETI_THREAD_FARG)
{
  int recv_list_size = 0;
  gasnetc_ucx_request_t *request = NULL;
  gasneti_list_t local_recv_list;

  GASNETC_LOCK_ACQUIRE(lmode);
  /* poll recv requests and push to the receive queue */
  gasnetc_poll_snd(GASNETC_LOCK_INLINE GASNETI_THREAD_PASS);
  gasnetc_ucx_progress();

  recv_list_size = gasneti_list_size(&gasneti_ucx_module.recv_queue);
  if_pt (!recv_list_size) {
    goto exit;
  }
  gasneti_list_init(&local_recv_list);
  for (int i = 0; (i < GASNETC_UCX_MSG_HNDL_PER_POLL) &&
       (NULL != (request =
                 gasneti_req_probe_complete(&gasneti_ucx_module.recv_queue)));
       i++) {
    gasneti_list_enq(&local_recv_list, request);
  }
  GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);
  /* handle recv messages */
  GASNETI_LIST_FOREACH(request, &local_recv_list, gasnetc_ucx_request_t) {
    gasneti_assert(request->am_req->buffer.bytes_used ||
                   request->am_req->buffer.long_bytes_used);
    if_pt (GASNETC_BUF_SIZE(request->am_req->buffer)) {
      gasnetc_ProcessRecv(GASNETC_BUF_DATA(request->am_req->buffer),
                          GASNETC_BUF_SIZE(request->am_req->buffer));
    } else {
      gasnetc_ProcessRecv(GASNETC_BUF_LDATA(request->am_req->buffer),
                          GASNETC_BUF_LSIZE(request->am_req->buffer));
    }
  }
  GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  /* release */
  while(NULL !=
        (request = GASNETI_LIST_POP(&local_recv_list, gasnetc_ucx_request_t))) {
    gasnetc_rreq_release(request);
  }
  gasneti_list_fini(&local_recv_list);

exit:
  GASNETC_LOCK_RELEASE(lmode);
  return recv_list_size;
}

#endif

void gasnetc_send_list_wait(gasnetc_lock_mode_t lmode GASNETI_THREAD_FARG)
{
  size_t send_size;
  do {
    GASNETI_WAITHOOK();
    GASNETC_LOCK_ACQUIRE(lmode);
    gasnetc_poll_sndrcv(GASNETC_LOCK_INLINE GASNETI_THREAD_PASS);
    send_size = gasneti_list_size(&gasneti_ucx_module.send_queue);
    GASNETC_LOCK_RELEASE(lmode);
  } while (send_size);
}
/* ------------------------------------------------------------------------------------ */
