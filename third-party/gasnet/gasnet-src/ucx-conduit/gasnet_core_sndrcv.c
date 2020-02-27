/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ucx-conduit/gasnet_core_sndrcv.c $
 * Description: GASNet ucx conduit implementation, transport send/receive logic
 * Copyright 2019, Mellanox Technologies, Inc. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_am.h>
#include <gasnet_ucx_req.h>

GASNETI_INLINE(gasneti_ucx_progress)
int gasnetc_ucx_progress(void);

GASNETI_INLINE(gasnetc_am_req_reset)
void gasnetc_am_req_reset(gasnetc_am_req_t *am_req);

GASNETI_INLINE(gasnetc_buffer_reset)
void gasnetc_buffer_reset(gasnetc_buffer_t *buffer);

/* ------------------------------------------------------------------------------------ */
/*
  List functions
  ==============
*/
#if GASNET_DEBUG
#define GASNETI_LIST_MAGIC 0x54544545
#define GASNETI_DBG_LIST_ITEM_CHECK(item)                     \
do {                                                          \
  gasneti_list_item_t *_item = (gasneti_list_item_t*)(item);  \
  gasneti_assert(_item);                                      \
  gasneti_assert(_item->magic == GASNETI_LIST_MAGIC);         \
} while(0)

#define GASNETI_DBG_LIST_ITEM_SET_MAGIC(item)                 \
do {                                                          \
  gasneti_list_item_t *_item = (gasneti_list_item_t*)(item);  \
  gasneti_assert(_item);                                      \
  _item->magic = GASNETI_LIST_MAGIC;                          \
} while(0)
#else
#define GASNETI_DBG_LIST_ITEM_CHECK(item)       ((void)0)
#define GASNETI_DBG_LIST_ITEM_SET_MAGIC(item)   ((void)0)
#endif

#define GASNETI_LIST_RESET(item)                              \
do {                                                          \
  gasneti_list_item_t *_item = (gasneti_list_item_t*)(item);  \
  _item->next = NULL;                                         \
  _item->prev = NULL;                                         \
} while(0);

void gasneti_list_init(gasneti_list_t *list)
{
  gasneti_assert(list);
  list->head = gasneti_malloc(sizeof(gasneti_list_item_t));
  gasneti_assert(list->head && "Out of memory");
  list->tail = gasneti_malloc(sizeof(gasneti_list_item_t));
  gasneti_assert(list->tail && "Out of memory");
#if GASNET_DEBUG
  list->head->magic = 0;
  list->tail->magic = 0;
#endif
  list->head->next = list->tail;
  list->head->prev = NULL;

  list->tail->prev = list->head;
  list->tail->next = NULL;

  gasneti_assert(list->tail->prev);

  list->count = 0;
}

GASNETI_INLINE(gasneti_list_fini)
void gasneti_list_fini(gasneti_list_t *list)
{
  gasneti_assert(list);
  gasneti_assert(!list->count);
  gasneti_assert(list->head->next == list->tail);
  gasneti_assert(list->head == list->tail->prev);

  gasneti_free(list->head);
  gasneti_free(list->tail);
  /* protect the list */
  list->head = NULL;
  list->tail = NULL;
}

GASNETI_INLINE(gasneti_list_enq)
void gasneti_list_enq(gasneti_list_t *list, void *ptr)
{
    gasneti_assert(list);
    GASNETI_DBG_LIST_ITEM_CHECK(ptr);
    gasneti_assert(list->tail->prev);
    gasneti_assert(list->head->next);

    gasneti_list_item_t *item = (gasneti_list_item_t*)ptr;

    /* setup connection to the previous elem */
    item->prev = list->tail->prev;
    item->prev->next = item;

    /* setup connection to the dummy tail elem */
    item->next = list->tail;
    list->tail->prev = item;

    /* increase element count */
    list->count++;
}

GASNETI_INLINE(gasneti_list_deq)
gasneti_list_item_t *gasneti_list_deq(gasneti_list_t *list)
{
  gasneti_assert(list);
  gasneti_assert(list->tail->prev);
  gasneti_assert(list->head->next);

  gasneti_list_item_t *item = list->head->next;

  if (item == list->tail) {
    return NULL;
  }
  list->head->next = item->next;
  item->next->prev = list->head;

  item->next = NULL;
  item->prev = NULL;

  list->count--;
  return item;
}

GASNETI_INLINE(gasneti_list_begin)
gasneti_list_item_t *gasneti_list_begin(gasneti_list_t *list)
{
  gasneti_assert(list);
  gasneti_assert(list->head);
  return list->head;
}

GASNETI_INLINE(gasneti_list_end)
gasneti_list_item_t *gasneti_list_end(gasneti_list_t *list)
{
  gasneti_assert(list);
  gasneti_assert(list->tail);
  return list->tail;
}

GASNETI_INLINE(gasneti_list_next)
gasneti_list_item_t *gasneti_list_next(void *ptr)
{
  gasneti_list_item_t *item = (gasneti_list_item_t*)ptr;
  gasneti_assert(item);
  GASNETI_DBG_LIST_ITEM_CHECK(item->next);
  return item->next;
}

GASNETI_INLINE(gasneti_list_size)
size_t gasneti_list_size(gasneti_list_t *list)
{
  gasneti_assert(list);
  return list->count;
}

GASNETI_INLINE(gasneti_list_rem)
void *gasneti_list_rem(gasneti_list_t *list, void *ptr)
{
  gasneti_list_item_t *item = (gasneti_list_item_t*)ptr;
  gasneti_assert(list);
  GASNETI_DBG_LIST_ITEM_CHECK(item);
  if ((list->head == item) || (list->tail == item)) {
    return NULL;
  }

  item->prev->next = item->next;
  item->next->prev = item->prev;
  /* protect the list */
  item->next = item->prev = NULL;

  list->count--;
  return item;
}

#define GASNETI_LIST_POP(list, type)    \
  (gasneti_list_size(list) ? (type*) gasneti_list_deq(list) : (type*) NULL)

#define GASNETI_LIST_FOREACH(item, list, type)                  \
  for (item =  (type *) (list)->head->next;                     \
       item != (type *) (list)->tail;                           \
       item =  (type *) ((gasneti_list_item_t *) (item))->next)
/* ------------------------------------------------------------------------------------ */
/*
  Req/Mem pool functions
  ======================
*/
#define GASNETI_LIST_ITEM_ALLOC(item, type, reset_fn)           \
do {                                                            \
  item = (type *) gasneti_malloc(sizeof(type));                 \
  gasneti_assert(item && "Out of mem");                         \
  reset_fn(item);                                               \
  GASNETI_DBG_LIST_ITEM_SET_MAGIC(item);                        \
} while(0)

void gasnetc_am_req_pool_alloc(void)
{
  gasnetc_am_req_t *am_req;

  gasneti_list_init(&gasnet_ucx_module.am_req_pool);
  for (int i = 0; i < GASNETC_UCX_REQ_POOL_SIZE; i++) {
    GASNETI_LIST_ITEM_ALLOC(am_req, gasnetc_am_req_t, gasnetc_am_req_reset);
    gasneti_list_enq(&gasnet_ucx_module.am_req_pool, am_req);
  }
}

void gasnetc_am_req_pool_free(void)
{
  gasnetc_am_req_t *am_req;

  while(NULL != (am_req = GASNETI_LIST_POP(
                   &gasnet_ucx_module.am_req_pool, gasnetc_am_req_t))){
    gasneti_free(am_req);
  }
  gasneti_list_fini(&gasnet_ucx_module.am_req_pool);
}

void gasnetc_buffer_pool_alloc(void)
{
  gasnetc_buffer_t *buffer;

  gasneti_list_init(&gasnet_ucx_module.recv_pool);
  for (int i = 0; i < GASNETC_UCX_BUF_RECV_POOL_DFLT; i++) {
    GASNETI_LIST_ITEM_ALLOC(buffer, gasnetc_buffer_t, gasnetc_buffer_reset);
    buffer->type = GASNETC_BUF_RECV_POOL;
    gasneti_list_enq(&gasnet_ucx_module.recv_pool, buffer);
  }
  gasneti_list_init(&gasnet_ucx_module.send_pool);
  for (int i = 0; i < GASNETC_UCX_BUF_SEND_POOL_SIZE; i++) {
    GASNETI_LIST_ITEM_ALLOC(buffer, gasnetc_buffer_t, gasnetc_buffer_reset);
    buffer->type = GASNETC_BUF_SEND_POOL;
    gasneti_list_enq(&gasnet_ucx_module.send_pool, buffer);
  }
}

void gasnetc_buffer_pool_free(void)
{
  gasnetc_buffer_t *buffer;

  while(NULL != (buffer = GASNETI_LIST_POP(
                   &gasnet_ucx_module.recv_pool, gasnetc_buffer_t))){
    if (buffer->long_data_ptr) {
      gasneti_free(buffer->long_data_ptr);
    }
    gasneti_assert(buffer->type == GASNETC_BUF_RECV_POOL);
    gasneti_free(buffer);
  }
  while(NULL != (buffer = GASNETI_LIST_POP(
                   &gasnet_ucx_module.send_pool, gasnetc_buffer_t))){
    gasneti_assert(buffer->type == GASNETC_BUF_SEND_POOL);
    gasneti_free(buffer);
  }
  gasneti_list_fini(&gasnet_ucx_module.recv_pool);
  gasneti_list_fini(&gasnet_ucx_module.send_pool);
}

GASNETI_INLINE(gasnetc_am_req_get)
gasnetc_am_req_t *gasnetc_am_req_get(void)
{
  gasnetc_am_req_t *am_req;

  do {
    gasnetc_ucx_progress();

  } while (NULL == (am_req = GASNETI_LIST_POP(&gasnet_ucx_module.am_req_pool,
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
    } while (NULL == (buffer = GASNETI_LIST_POP(&gasnet_ucx_module.recv_pool,
                                                gasnetc_buffer_t)));
  } else {
    do {
      gasnetc_req_poll_rcv(GASNETC_LOCK_MODE_INLINE);
    } while (NULL == (buffer = GASNETI_LIST_POP(&gasnet_ucx_module.send_pool,
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
  gasneti_list_enq(&gasnet_ucx_module.am_req_pool, am_req);
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
    gasneti_list_enq(&gasnet_ucx_module.send_pool, buffer);
  } else {
    gasneti_list_enq(&gasnet_ucx_module.recv_pool, buffer);
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
                           gex_AM_Index_t handler, uint8_t is_req, int numargs,
                           va_list argptr, uint32_t nbytes,
                           void *dst_addr GASNETI_THREAD_FARG)
{
  int i;
  int args_size = sizeof(gex_AM_Arg_t) * numargs;
  int padding_size = 0;

  gasneti_assert(am_req);
  am_req->am_hdr.am_type  = am_type;
  am_req->am_hdr.handler  = handler;
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
  gasneti_list_init(&gasnet_ucx_module.send_list);
  gasneti_list_init(&gasnet_ucx_module.recv_list);
}

void gasnetc_rreq_list_free(void)
{
  gasnetc_ucx_request_t *request;

  while(NULL != (request = GASNETI_LIST_POP(
                   &gasnet_ucx_module.recv_list, gasnetc_ucx_request_t))){
    gasnetc_req_free(request);
  }
  gasneti_list_fini(&gasnet_ucx_module.recv_list);
}

void gasnetc_sreq_list_free(void)
{
  gasnetc_ucx_request_t *request;

  while(NULL != (request = GASNETI_LIST_POP(
                   &gasnet_ucx_module.send_list, gasnetc_ucx_request_t))){
    if (GASNETC_UCX_ACTIVE == request->status) {
      ucp_request_cancel(gasnet_ucx_module.ucp_worker, request);
      while (GASNETC_UCX_ACTIVE == request->status) {
        gasnetc_ucx_progress();
        // TODO-next: interrupt if stuck here for a long time
      }
      gasneti_list_rem(&gasnet_ucx_module.send_list, request);
      gasnetc_req_free(request);
    }
  }
  gasneti_list_fini(&gasnet_ucx_module.send_list);
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
  if (req->is_sync) {
    /* No cleanup here, it will be released after `gasnetc_wait_req` */
    return;
  }
exit:
  gasneti_list_rem(&gasnet_ucx_module.send_list, req);
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
    gasneti_list_enq(&gasnet_ucx_module.recv_list, req);
  }
  req->status = GASNETC_UCX_COMPLETE;
}

GASNETI_INLINE(gasnetc_wait_req)
void gasnetc_wait_req(gasnetc_ucx_request_t *req, uint8_t is_request)
{
  while (GASNETC_UCX_ACTIVE == req->status) {
    if (is_request) {
      /* Ensure full progress only for Requests
       * to avoid recursive "poll" while receiving. */
      gasnetc_req_poll(GASNETC_LOCK_MODE_REGULAR);
    } else {
      gasnetc_req_poll_rcv(GASNETC_LOCK_MODE_REGULAR);
    }
  }
  GASNETC_LOCK_ACQUIRE_REGULAR();
  gasneti_list_rem(&gasnet_ucx_module.send_list, req);
  gasnetc_req_free(req);
  GASNETC_LOCK_RELEASE_REGULAR();
}

GASNETI_INLINE(gasnetc_send_req)
gasnetc_ucx_request_t *gasnetc_send_req(gasnetc_am_req_t *am_req,
                                        gasnetc_buffer_t *buffer,
                                        uint8_t block)
{
  gasnetc_ucx_request_t *request = NULL;
  ucp_ep_h server_ep =
      gasnet_ucx_module.remote_ep_tbl[am_req->am_hdr.dst].server_ep;

  server_ep = gasnet_ucx_module.remote_ep_tbl[am_req->am_hdr.dst].server_ep;

  gasneti_assert(am_req->iov_count &&
                 (am_req->iov_count <= GASNETC_UCX_IOV_MAX)
                 && "Wrong iov count");

  request = ucp_tag_send_nb(server_ep, (void*)am_req->sendiov,
                            (size_t)am_req->iov_count, ucp_dt_make_iov(),
                            (ucp_tag_t)gasneti_mynode, gasnetc_ucx_send_handler);
  if (NULL == request) {
      /* request was completed in place */
      gasnetc_am_req_release(am_req);
      if (buffer) {
        gasnetc_buffer_release(buffer);
      }
      return NULL;
  }
  if_pf (UCS_PTR_IS_ERR(request)) {
    gasnetc_am_req_release(am_req);
    if (buffer) {
      gasnetc_buffer_release(buffer);
    }
    gasneti_fatalerror("UCX recv request failed: %s",
                       ucs_status_string(UCS_PTR_STATUS(request)));
    return NULL;
  }

  request->am_req = am_req;
  request->buffer = buffer;
  request->is_sync = block;
  request->status = GASNETC_UCX_ACTIVE;
  gasneti_list_enq(&gasnet_ucx_module.send_list, request);

  return request;
}
/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
*/
int gasnetc_AM_ReqRepGeneric(gasnetc_ucx_am_type_t am_type,
                             gex_Rank_t jobrank,
                             gex_AM_Index_t handler,
                             gex_Flags_t flags,
                             uint8_t is_request,
                             int numargs,
                             va_list argptr,
                             void *src_addr,
                             uint32_t nbytes,
                             void *dst_addr
                             GASNETI_THREAD_FARG)
{
  gasnetc_am_req_t *am_req;
  gasnetc_buffer_t *buffer = NULL;
  gasnetc_ucx_request_t *req;

  GASNETC_LOCK_ACQUIRE_REGULAR();
  am_req = gasnetc_am_req_get();
  gasneti_assert(am_req);

  /* format common data */
  gasnetc_am_req_format(am_req, am_type, jobrank, handler, is_request,
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
      // TODO-next: RDMA request support
      gasneti_assert(src_addr);
      gasneti_assert(dst_addr);
      /* pack payload */
      // TODO-next: use `ucp_put_nb` instead of this routine
      buffer = gasnetc_buffer_get(GASNETC_BUF_SEND_POOL);
      gasneti_assert(buffer);
      buffer->long_data_ptr = gasneti_malloc(nbytes);
      gasneti_assert(buffer->long_data_ptr);
      buffer->bytes_used = nbytes;
      GASNETI_MEMCPY(buffer->long_data_ptr, src_addr, nbytes);
      gasnetc_req_add_iov(am_req, buffer->long_data_ptr, nbytes);
      break;
  }

send:
  req = gasnetc_send_req(am_req, buffer, is_request);
  GASNETC_LOCK_RELEASE_REGULAR();

  if (req && is_request) {
    gasnetc_wait_req(req, is_request);
  }
  return GASNET_OK;
}

void gasnetc_ProcessRecv(void *buf, size_t size)
{
  gasnetc_sreq_hdr_t *am_hdr = (gasnetc_sreq_hdr_t*)buf;
  gex_AM_Index_t handler_id = am_hdr->handler;
  int numargs = am_hdr->numargs;
  int is_req = am_hdr->is_req;
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
      if (am_hdr->nbytes > 0) {
        gasneti_assert(am_hdr->dst_addr);
        data = (char*)((char*)buf  + sizeof(gasnetc_sreq_hdr_t) +
                       sizeof(gex_AM_Arg_t) * numargs);
        GASNETI_MEMCPY(am_hdr->dst_addr, data, nbytes);
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
      (status = ucp_worker_progress(gasnet_ucx_module.ucp_worker)) &&
        i < GASNETC_UCX_PROGRESS_CNT;
      i++);
  return status;
}

void gasnetc_req_poll_rcv(GASNETC_LOCK_MODE_ARG_ALONE)
{
  uint32_t probe_cnt = 0, probe_max;
  gasnetc_ucx_request_t *request = NULL;
  gasnetc_buffer_t *buffer;
  void *buf_ptr = NULL;
  ucp_tag_recv_info_t info_tag;
  ucp_tag_message_h msg_tag;

  GASNETC_LOCK_ACQUIRE();
  gasnetc_ucx_progress();

  /* Make sure there are enough entries in the buffer pool to receive
   * new messages if the buffer pool is exhausted do not check
   * (ucp_tag_probe_nb) for new messages just keep them in the ucx queue. */
  probe_max = gasneti_list_size(&gasnet_ucx_module.recv_pool);
  while((probe_cnt++) < MIN(probe_max, GASNETC_UCX_RCV_REAP_MAX)) {
    gasnetc_ucx_progress();
    /* check for new messages */
    msg_tag = ucp_tag_probe_nb(gasnet_ucx_module.ucp_worker, 0, 0, 1, &info_tag);
    if (NULL == msg_tag) {
      break;
    }
    /* Message arrived */
    buffer = gasnetc_buffer_get(GASNETC_BUF_RECV_POOL);
    gasneti_assert(buffer);
    buffer->bytes_used = info_tag.length;
    if (info_tag.length > GASNETC_MAX_MED) {
      buffer->long_data_ptr = gasneti_malloc(info_tag.length);
      gasneti_assert(buffer->long_data_ptr);
      buf_ptr = buffer->long_data_ptr;
    } else {
      buf_ptr = buffer->data;
    }
    request = (gasnetc_ucx_request_t*)
        ucp_tag_msg_recv_nb(gasnet_ucx_module.ucp_worker, buf_ptr,
                            info_tag.length, ucp_dt_make_contig(1), msg_tag,
                            gasneti_ucx_recv_handler);
    if (UCS_PTR_IS_ERR(request)) {
      gasnetc_buffer_release(buffer);
      gasneti_fatalerror("UCX recv request failed: %s",
                         ucs_status_string(UCS_PTR_STATUS(request)));
      return;
    }
    /* fill in the info for the request*/
    request->buffer = buffer;
    if (GASNETC_UCX_COMPLETE == request->status) {
      /* request was completed in place
       * and wasn't added to the receiving pool in the UCX recv handler,
       * so add it to local recv pool here */
      gasneti_list_enq(&gasnet_ucx_module.recv_list, request);
      continue;
    }
    request->status = GASNETC_UCX_ACTIVE;
  }
  GASNETC_LOCK_RELEASE();
}

void gasnetc_req_poll(GASNETC_LOCK_MODE_ARG_ALONE)
{
  gasnetc_ucx_request_t *request = NULL;
  gasneti_list_t local_recv_list;

  GASNETC_LOCK_ACQUIRE();
  /* poll recv requests and push to the receive queue */
  gasnetc_req_poll_rcv(GASNETC_LOCK_MODE_INLINE);
  gasnetc_ucx_progress();

  if (!gasneti_list_size(&gasnet_ucx_module.recv_list)) {
    goto exit;
  }
  gasneti_list_init(&local_recv_list);
  for (int i = 0; (i < GASNETC_UCX_MSG_HNDL_PER_POLL) &&
       (NULL != (request =
                 gasneti_req_probe_complete(&gasnet_ucx_module.recv_list)));
       i++) {
    gasneti_list_enq(&local_recv_list, request);
  }
  GASNETC_LOCK_RELEASE_REGULAR();
  /* handle recv messages */
  GASNETI_LIST_FOREACH(request, &local_recv_list, gasnetc_ucx_request_t) {
    gasneti_assert(request->buffer->bytes_used);
    gasnetc_req_process_nolock(request);
  }
  GASNETC_LOCK_ACQUIRE_REGULAR();
  /* release */
  while(NULL !=
        (request = GASNETI_LIST_POP(&local_recv_list, gasnetc_ucx_request_t))) {
    gasnetc_req_free(request);
  }
  gasneti_list_fini(&local_recv_list);

exit:
  GASNETC_LOCK_RELEASE();
  return;
}

void gasnetc_req_wait(GASNETC_LOCK_MODE_ARG_ALONE)
{
  size_t send_size;
  do {
    GASNETI_WAITHOOK();
    GASNETC_LOCK_ACQUIRE();
    gasnetc_req_poll(GASNETC_LOCK_MODE_INLINE);
    send_size = gasneti_list_size(&gasnet_ucx_module.send_list);
    GASNETC_LOCK_RELEASE();
  } while (send_size);

  GASNETI_SAFE(gasnet_barrier(0, GASNET_BARRIERFLAG_UNNAMED));
}
/* ------------------------------------------------------------------------------------ */
