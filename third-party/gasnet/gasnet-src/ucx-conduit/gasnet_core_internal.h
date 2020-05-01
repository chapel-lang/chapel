/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ucx-conduit/gasnet_core_internal.h $
 * Description: GASNet ucx conduit header for internal definitions in Core API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2019, Mellanox Technologies LTD. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <gasnet_internal.h>

#include <ucp/api/ucp.h>
#include <ucs/type/status.h>

/*  whether or not to use spin-locking for HSL's */
#define GASNETC_HSL_SPINLOCK 1

/* ------------------------------------------------------------------------------------ */
#define _hidx_gasnetc_exchg_reqh              (GASNETC_HANDLER_BASE+0)
#define _hidx_gasnetc_exit_reduce_reqh        (GASNETC_HANDLER_BASE+1)
#define _hidx_gasnetc_exit_role_reqh          (GASNETC_HANDLER_BASE+2)
#define _hidx_gasnetc_exit_role_reph          (GASNETC_HANDLER_BASE+3)
#define _hidx_gasnetc_exit_reqh               (GASNETC_HANDLER_BASE+4)
#define _hidx_gasnetc_exit_reph               (GASNETC_HANDLER_BASE+5)

/* add new core API handlers here and to the bottom of gasnet_core.c */

/* ------------------------------------------------------------------------------------ */
/* handler table (temporary global impl) */
extern gex_AM_Entry_t *gasnetc_handler;

/* ------------------------------------------------------------------------------------ */
/* Configure gasnet_event_internal.h and gasnet_event.c */
// TODO-EX: prefix needs to move from "extended" to "core"

// (###) Define as needed if iop counters should use something other than weakatomics:
/* #define gasnete_op_atomic_(_id) gasnetc_atomic_##_id */

// (###) Define if conduit performs local-completion detection:
/* #define GASNETE_HAVE_LC */

#define GASNETE_HAVE_LC

/* Atomics wrappers
 *
 * Only for GASNETC_ANY_PAR do we need true atomics.
 * In particular neither PARSYNC nor CONN_THREAD introduce concurrency,
 * but use of "weak" atomics would pay the unnecessary costs for those.
 */
#if GASNET_PAR
  #define GASNETC_PARSEQ _PAR
  #define gasnetc_cons_atomic(_id) _CONCAT(gasneti_atomic_,_id)
#else
  #define GASNETC_PARSEQ _SEQ
  #define gasnetc_cons_atomic(_id) _CONCAT(gasneti_nonatomic_,_id)
#endif

typedef gasnetc_cons_atomic(t)            gasnetc_atomic_t;
typedef gasnetc_cons_atomic(val_t)        gasnetc_atomic_val_t;
#define gasnetc_atomic_init               gasnetc_cons_atomic(init)
#define gasnetc_atomic_read               gasnetc_cons_atomic(read)
#define gasnetc_atomic_set                gasnetc_cons_atomic(set)
#define gasnetc_atomic_increment          gasnetc_cons_atomic(increment)
#define gasnetc_atomic_decrement_and_test gasnetc_cons_atomic(decrement_and_test)
#define gasnetc_atomic_compare_and_swap   gasnetc_cons_atomic(compare_and_swap)
#define gasnetc_atomic_swap               gasnetc_cons_atomic(swap)
#define gasnetc_atomic_add                gasnetc_cons_atomic(add)
#define gasnetc_atomic_subtract           gasnetc_cons_atomic(subtract)

#define GASNETE_CONDUIT_EOP_FIELDS \
  gasnetc_atomic_val_t initiated_cnt; \
  gasnetc_atomic_t     completed_cnt; \
  gasnetc_atomic_val_t initiated_alc; \
  gasnetc_atomic_t     completed_alc;

typedef void (*gasnetc_cbfunc_t)(gasnetc_atomic_val_t *);

typedef struct {
    gasnetc_atomic_t     completed;
    gasnetc_atomic_val_t initiated;
} gasnetc_counter_t;

#define GASNETC_COUNTER_INITIALIZER   {gasnetc_atomic_init(0), 0}

#define gasnetc_counter_inc(P)		do { (P)->initiated++; } while (0)
#define gasnetc_counter_done(P)       (((P)->initiated & GASNETI_ATOMIC_MAX) == \
                                           gasnetc_atomic_read(&(P)->completed, 0))

typedef enum {
  gasnetc_comptype_eop_alc,
  gasnetc_comptype_eop_get,
  gasnetc_comptype_eop_put
} gasnetc_comptype_t;

/* Callback functions in gasnet_core_sndrcv.c */
extern int gasnetc_complete_eop(gasnete_eop_t *eop, gasnetc_comptype_t type);
/* eop: */
extern void gasnetc_cb_eop_alc(gasnetc_atomic_val_t *);
extern void gasnetc_cb_eop_put(gasnetc_atomic_val_t *);
extern void gasnetc_cb_eop_get(gasnetc_atomic_val_t *);
/* iop within nbi-accessregion: */
extern void gasnetc_cb_nar_alc(gasnetc_atomic_val_t *);
extern void gasnetc_cb_nar_put(gasnetc_atomic_val_t *);
extern void gasnetc_cb_nar_get(gasnetc_atomic_val_t *);
extern void gasnetc_cb_nar_rmw(gasnetc_atomic_val_t *);
/* iop not in nbi-accessregion: */
extern void gasnetc_cb_iop_alc(gasnetc_atomic_val_t *);
extern void gasnetc_cb_iop_put(gasnetc_atomic_val_t *);
extern void gasnetc_cb_iop_get(gasnetc_atomic_val_t *);
extern void gasnetc_cb_iop_rmw(gasnetc_atomic_val_t *);
/* gasnetc_counter_t */
extern void gasnetc_cb_counter(gasnetc_atomic_val_t *cnt);
extern void gasnetc_cb_counter_rel(gasnetc_atomic_val_t *cnt);

extern void gasnetc_counter_wait(gasnetc_counter_t *counter,
                                 int handler_context GASNETI_THREAD_FARG);

/*
 * Bootstrap support
 */
extern gasneti_spawnerfn_t const *gasneti_spawner;

#define gasneti_bootstrapBarrier        (*(gasneti_spawner->Barrier))
#define gasneti_bootstrapExchange       (*(gasneti_spawner->Exchange))
#define gasneti_bootstrapBroadcast      (*(gasneti_spawner->Broadcast))
#define gasneti_bootstrapSNodeBroadcast (*(gasneti_spawner->SNodeBroadcast))
#define gasneti_bootstrapAlltoall       (*(gasneti_spawner->Alltoall))
#define gasneti_bootstrapAbort          (*(gasneti_spawner->Abort))
#define gasneti_bootstrapCleanup        (*(gasneti_spawner->Cleanup))
#define gasneti_bootstrapFini           (*(gasneti_spawner->Fini))

#if GASNET_PAR
#define GASNETC_UCX_THREADS
#endif

/* check for exit in progress */
extern int gasnetc_exit_running;
extern gasnete_threadidx_t gasnetc_exit_thread;

#ifdef GASNETC_UCX_THREADS
#define GASNETC_MY_THREAD_IDX(__threadidx)                                \
  do {                                                                    \
    GASNET_BEGIN_FUNCTION();                                              \
    (__threadidx) = GASNETI_MYTHREAD->threadidx;                          \
  } while (0)

#define GASNETC_LOCK_UCX()                                                \
  do {                                                                    \
    gasneti_mutex_lock(&gasneti_ucx_module.ucp_worker_lock);              \
  } while (0)

#define GASNETC_UNLOCK_UCX()                                              \
  do {                                                                    \
    gasneti_mutex_unlock(&gasneti_ucx_module.ucp_worker_lock);            \
  } while (0)

#define GASNETC_LOCK_ACQUIRE_REGULAR()                                    \
  do {                                                                    \
    gasneti_mutex_lock(&gasneti_ucx_module.ucp_worker_lock);              \
    if_pf (gasnetc_exit_running) {                                        \
      gasnete_threadidx_t threadidx;                                      \
      GASNETC_MY_THREAD_IDX(threadidx);                                   \
      if (gasnetc_exit_thread != threadidx) {                             \
              gasneti_mutex_unlock(&gasneti_ucx_module.ucp_worker_lock);  \
              gasnetc_exit_threads();                                     \
      }                                                                   \
    }                                                                     \
  } while(0)
#define GASNETC_LOCK_RELEASE_REGULAR()                          \
  do {                                                          \
    gasneti_mutex_unlock(&gasneti_ucx_module.ucp_worker_lock);  \
  } while(0)

#define GASNETC_LOCK_ACQUIRE(mode)                              \
  do {                                                          \
    switch(mode) {                                              \
    case GASNETC_LOCK_REGULAR:                                  \
      GASNETC_LOCK_ACQUIRE_REGULAR();                           \
      break;                                                    \
    case GASNETC_LOCK_INLINE:                                   \
      break;                                                    \
    }                                                           \
  } while(0)

#define GASNETC_LOCK_RELEASE(mode)                              \
  do {                                                          \
    switch(mode) {                                              \
    case GASNETC_LOCK_REGULAR:                                  \
      GASNETC_LOCK_RELEASE_REGULAR();                           \
      break;                                                    \
    case GASNETC_LOCK_INLINE:                                   \
      break;                                                    \
    }                                                           \
  } while(0)
#else
#define GASNETC_LOCK_ACQUIRE(lmode)           ((void)0)
#define GASNETC_LOCK_RELEASE(lmode)           ((void)0)
#define GASNETC_LOCK_UCX()                    ((void)0)
#define GASNETC_UNLOCK_UCX()                  ((void)0)
#endif

#if GASNET_DEBUG_VERBOSE
#define GASNETC_UCX_DEBUG_PRINT(fmt, ...)                       \
do {                                                            \
  gasneti_console_message("DEBUG", fmt "\n", ## __VA_ARGS__);   \
} while (0)
#else
#define GASNETC_UCX_DEBUG_PRINT(fmt, ...) ((void)0)
#endif

/* AM message type */
typedef enum {
  GASNETC_UCX_AM_SHORT = 0,
  GASNETC_UCX_AM_MEDIUM,
  GASNETC_UCX_AM_LONG
} gasnetc_ucx_am_type_t;

typedef enum {
  GASNETC_LOCK_INLINE,
  GASNETC_LOCK_REGULAR
} gasnetc_lock_mode_t;

typedef void (*gasneti_list_item_free_fn)(void *);

typedef struct gasneti_list_item_s {
#if GASNET_DEBUG
  int magic;
#endif
  struct gasneti_list_item_s *next, *prev;
} gasneti_list_item_t;

typedef struct gasneti_list_s {
  gasneti_list_item_t *head, *tail;
  size_t count;
} gasneti_list_t;

#define GASNETC_LIST_CLASS gasneti_list_item_t super

typedef struct _gasnetc_mem_info {
  GASNETC_LIST_CLASS;
  void      * buffer;
  size_t      bsize;
  void      * addr;
  size_t      length;
  ucp_rkey_h  rkey;
  ucp_mem_h   mem_h;
} gasnetc_mem_info_t;

typedef struct _gasnet_ep_info {
    ucp_ep_h server_ep;
    ucp_address_t *ucx_addr;
    size_t ucx_addr_len;
    gasneti_list_t mem_tbl;
} gasnet_ep_info_t;

typedef struct _gasneti_ucx_module {
    ucp_context_h               ucp_context;
    ucp_worker_h                ucp_worker;
    gasneti_mutex_t             ucp_worker_lock;
    gasnet_ep_info_t          * ep_tbl;
    gasneti_list_t              send_pool;   /* buffer pool */
    gasneti_list_t              recv_pool;   /* recv buffer pool */
    gasneti_list_t              am_req_pool; /* AM requests pool */
    gasneti_list_t              recv_list;   /* list of completed but not handled requests */
    gasneti_list_t              send_list;   /* list of pending send requests */
} gasneti_ucx_module_t;

typedef struct {
  gex_Rank_t source;
  const gex_AM_Entry_t *entry;
  int is_request;
#if GASNETI_THREADINFO_OPT
  gasnet_threadinfo_t threadinfo;
#endif
} gasnetc_token_t;

typedef enum {
  gasnetc_rdma_op_put,
  gasnetc_rdma_op_get,
} gasnetc_rdma_op_t;

typedef ucs_status_ptr_t (*gasnetc_putget_fn_t)(ucp_ep_h ep, void *buffer,
                                  uint32_t nbytes, void *remote_addr,
                                  ucp_rkey_h rkey, ucp_send_callback_t cb);

#define GASNETC_PUTGET_FNNAME(rop) gasnetc_##rop##_req

#define GASNETC_PUTGET_FNDEF(rop)                                               \
GASNETI_INLINE(GASNETC_PUTGET_FNNAME(rop))                                      \
ucs_status_ptr_t GASNETC_PUTGET_FNNAME(rop)(ucp_ep_h ep, void *buffer,          \
                            uint32_t nbytes, void *remote_addr, ucp_rkey_h rkey,\
                            ucp_send_callback_t cb)                             \
{                                                                               \
  return ucp_##rop##_nb(ep, buffer, nbytes, (uint64_t)remote_addr, rkey, cb);   \
}

GASNETC_PUTGET_FNDEF(put)
GASNETC_PUTGET_FNDEF(get)

#define gasnetc_putget_fn(isput, ep, buffer, nbytes, addr, rkey, cb)        \
(isput) ? GASNETC_PUTGET_FNNAME(put)(ep, buffer, nbytes, addr, rkey, cb) :  \
          GASNETC_PUTGET_FNNAME(get)(ep, buffer, nbytes, addr, rkey, cb)

extern gasneti_ucx_module_t gasneti_ucx_module;

#define GASNETC_UCX_GET_EP(_rank) \
  gasneti_ucx_module.ep_tbl[_rank].server_ep;

#define gasneti_mem_pack(_dst, _src, _len, _pad, _off)    \
do {                                                      \
  gasneti_assert(_pad ? (_pad >= _len) : 1);              \
  GASNETI_MEMCPY(((char*)(_dst)) + (_off), _src, _len);   \
  (_off) += (_pad > 0 ? _pad : _len);                     \
} while(0);

#define gasneti_mem_unpack(_dst, _src, _len, _pad, _off)  \
do {                                                      \
  gasneti_assert(_pad ? (_pad >= _len) : 1);              \
  GASNETI_MEMCPY(_dst, ((char*)(_src)) + (_off), _len);   \
  (_off) += (_pad > 0 ? _pad : _len);                     \
} while(0);

#define gasneti_rkey_unpack(_ep, _buf, _rkey_ptr)         \
do {                                                      \
  ucs_status_t status;                                    \
  status = ucp_ep_rkey_unpack(_ep, _buf,  _rkey_ptr);     \
  if (status != UCS_OK) {                                 \
    gasneti_fatalerror("rkey unpack failed: %s",          \
      ucs_status_string(UCS_PTR_STATUS(status)));         \
  }                                                       \
} while(0);

#define GASNETC_ADDR_IN_RANGE(seg_addr, seg_size, addr, len)      \
            ( ( ((uintptr_t)seg_addr) <= ((uintptr_t)addr) ) &&   \
              ( ((uintptr_t)seg_addr) + (uintptr_t)seg_size >=    \
                    (((uintptr_t)addr) + (uintptr_t)len) ) )

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
                             GASNETI_THREAD_FARG);

extern void gasnetc_req_list_init(void);
extern void gasnetc_sreq_list_free(void);
extern void gasnetc_rreq_list_free(void);
extern void gasnetc_am_req_pool_alloc(void);
extern void gasnetc_am_req_pool_free(void);
extern void gasnetc_buffer_pool_alloc(void);
extern void gasnetc_buffer_pool_free(void);
extern int gasnetc_req_poll(gasnetc_lock_mode_t lmode);
extern void gasnetc_req_poll_rcv(gasnetc_lock_mode_t lmode);
extern void gasnetc_ProcessRecv(void *buf, size_t size);
extern void gasnetc_send_list_wait(gasnetc_lock_mode_t lmode);
extern gasnetc_mem_info_t * gasnetc_find_mem_info(void *addr, int nbytes,
                                                  gex_Rank_t rank);
extern int gasnetc_ucx_putget_inner(int is_put, gex_Rank_t jobrank,
                                    void *buffer, uint32_t nbytes,
                                    void *remote_addr,
                                    gasnetc_atomic_val_t *local_cnt,
                                    gasnetc_cbfunc_t local_cb,
                                    gasnetc_atomic_val_t *remote_cnt,
                                    gasnetc_cbfunc_t remote_cb);
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
           GASNETI_THREAD_FARG);
extern int gasnetc_RequestSysShort(gex_Rank_t jobrank,
                                   gasnetc_counter_t *counter,
                                   gex_AM_Index_t handler,
                                   int numargs, ...);
extern int gasnetc_ReplySysShort(gex_Token_t token,
                                 gasnetc_counter_t *counter,
                                 gex_AM_Index_t handler,
                                 int numargs, ...);
extern void gasnetc_exit_threads(void);
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

#define GASNETI_LIST_ITEM_ALLOC(item, type, reset_fn)           \
do {                                                            \
  void (*reset)(type *) = reset_fn;                             \
  item = (type *) gasneti_malloc(sizeof(type));                 \
  if (NULL != reset) {                                          \
    reset(item);                                                \
  }                                                             \
  GASNETI_DBG_LIST_ITEM_SET_MAGIC(item);                        \
} while(0)

#define GASNETI_LIST_RESET(item)                              \
do {                                                          \
  gasneti_list_item_t *_item = (gasneti_list_item_t*)(item);  \
  _item->next = NULL;                                         \
  _item->prev = NULL;                                         \
} while(0);

GASNETI_INLINE(gasneti_list_init)
void gasneti_list_init(gasneti_list_t *list)
{
  gasneti_assert(list);
  list->head = gasneti_malloc(sizeof(gasneti_list_item_t));
  list->tail = gasneti_malloc(sizeof(gasneti_list_item_t));
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

#endif // _GASNET_CORE_INTERNAL_H
