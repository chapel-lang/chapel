/*   $Source: bitbucket.org:berkeleylab/gasnet.git/template-conduit/gasnet_core_internal.h $
 * Description: GASNet ucx conduit header for internal definitions in Core API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
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

#define GASNETC_UCX_MAX_ADDR_LEN 1024

#if GASNET_PAR
#define GASNETC_UCX_THREADS
#endif

#ifdef GASNETC_UCX_THREADS
#define GASNETC_LOCK_MODE_ARG_ALONE gasnetc_lock_mode_t __lkmod
#define GASNETC_LOCK_MODE_ARG       , GASNETC_LOCK_MODE_ARG_ALONE
#define GASNETC_LOCK_MODE_VALUE     __lkmod
#define GASNETC_LOCK_MODE_REGULAR   GASNETC_LOCK_REGULAR
#define GASNETC_LOCK_MODE_INLINE    GASNETC_LOCK_INLINE
#define GASNETC_LOCK_ACQUIRE()                                  \
  do {                                                          \
    if (GASNETC_LOCK_REGULAR == GASNETC_LOCK_MODE_VALUE) {      \
      gasneti_mutex_lock(&gasnet_ucx_module.ucp_worker_lock);   \
    }                                                           \
  } while(0)
#define GASNETC_LOCK_RELEASE()                                  \
  do {                                                          \
    if (GASNETC_LOCK_REGULAR == GASNETC_LOCK_MODE_VALUE) {      \
      gasneti_mutex_unlock(&gasnet_ucx_module.ucp_worker_lock); \
    }                                                           \
  } while(0)
#define GASNETC_LOCK_ACQUIRE_REGULAR()                          \
  do {                                                          \
    gasneti_mutex_lock(&gasnet_ucx_module.ucp_worker_lock);     \
  } while(0)
#define GASNETC_LOCK_RELEASE_REGULAR()                          \
  do {                                                          \
    gasneti_mutex_unlock(&gasnet_ucx_module.ucp_worker_lock);   \
  } while(0)
#else
#define GASNETC_LOCK_MODE_ARG_ALONE
#define GASNETC_LOCK_MODE_ARG
#define GASNETC_LOCK_MODE_VALUE
#define GASNETC_LOCK_ACQUIRE()            ((void)0)
#define GASNETC_LOCK_RELEASE()            ((void)0)
#define GASNETC_LOCK_ACQUIRE_REGULAR()    ((void)0)
#define GASNETC_LOCK_RELEASE_REGULAR()    ((void)0)
#define GASNETC_LOCK_MODE_REGULAR
#define GASNETC_LOCK_MODE_INLINE
#endif

//#define GASNETC_UCX_DEBUG_ENABLE

#ifdef GASNETC_UCX_DEBUG_ENABLE
#define GASNETC_UCX_DEBUG_PRINT(fmt, ...)                \
do {                                                     \
  fprintf(stderr, "[%s:%u] %s:%u: " fmt "\n",            \
    gasneti_gethostname(), gasneti_mynode, __FUNCTION__, \
      __LINE__, ## __VA_ARGS__);                         \
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

typedef struct _gasnet_ucx_ep_conn_info {
    ucp_ep_h server_ep;
    char ucx_addr[GASNETC_UCX_MAX_ADDR_LEN];
    size_t ucx_addr_len;
} gasnet_ucx_ep_conn_info_t;

typedef struct _gasnet_ucx_module {
    ucp_context_h               ucp_context;
    ucp_worker_h                ucp_worker;
    gasneti_mutex_t             ucp_worker_lock;
    gasnet_ucx_ep_conn_info_t * remote_ep_tbl;
    gasneti_list_t              send_pool;   /* buffer pool */
    gasneti_list_t              recv_pool;   /* recv buffer pool */
    gasneti_list_t              am_req_pool; /* AM requests pool */
    gasneti_list_t              recv_list;   /* list of completed but not handled requests */
    gasneti_list_t              send_list;   /* list of pending send requests */
} gasnet_ucx_module_t;

typedef struct {
  gex_Rank_t source;
  const gex_AM_Entry_t *entry;
  int is_request;
#if GASNETI_THREADINFO_OPT
  gasnet_threadinfo_t threadinfo;
#endif
} gasnetc_token_t;

extern gasnet_ucx_module_t gasnet_ucx_module;

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
                             GASNETI_THREAD_FARG);

extern void gasnetc_req_list_init(void);
extern void gasnetc_sreq_list_free(void);
extern void gasnetc_rreq_list_free(void);
extern void gasnetc_am_req_pool_alloc(void);
extern void gasnetc_am_req_pool_free(void);
extern void gasnetc_buffer_pool_alloc(void);
extern void gasnetc_buffer_pool_free(void);
extern void gasnetc_req_poll(GASNETC_LOCK_MODE_ARG_ALONE);
extern void gasnetc_req_poll_rcv(GASNETC_LOCK_MODE_ARG_ALONE);
extern void gasnetc_ProcessRecv(void *buf, size_t size);
extern void gasnetc_req_wait(GASNETC_LOCK_MODE_ARG_ALONE);

#endif
