/*   $Source: /var/local/cvs/gasnet/vapi-conduit/gasnet_core_internal.h,v $
 *     $Date: 2012/03/06 19:56:11 $
 * $Revision: 1.229 $
 * Description: GASNet vapi conduit header for internal definitions in Core API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <stddef.h>	/* for offsetof() */

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#include <firehose.h>

/* Check that firehose_fwd.h picked an IB API for us */
#if !defined(GASNET_CONDUIT_VAPI) && !defined(GASNET_CONDUIT_IBV)
  #error "One of GASNET_CONDUIT_VAPI or GASNET_CONDUIT_IBV must be defined"
#endif

#if HAVE_SSH_SPAWNER
  #include <ssh-spawner/gasnet_bootstrap_internal.h>
#endif
#if HAVE_MPI_SPAWNER
  #include <mpi-spawner/gasnet_bootstrap_internal.h>
#endif

#if GASNET_CONDUIT_VAPI
  #include <vapi.h>
  #include <evapi.h>
  #include <vapi_common.h>
#else
  #include <infiniband/verbs.h>
  #if HAVE_IBV_SRQ
    #define GASNETC_IBV_SRQ 1
    #if HAVE_IBV_XRC
      #define GASNETC_IBV_XRC 1
    #endif
  #endif
#endif

#if HAVE_MMAP
  #include <sys/mman.h> /* For MAP_FAILED */
#endif

/*  whether or not to use spin-locking for HSL's */
#define GASNETC_HSL_SPINLOCK 1

/* GASNETC_FH_OPTIONAL: whether or not firehose can be switched OFF at runtime */
/* Enabled by default for DEBUG builds.  For NDEBUG builds, can force at compile time. */
#if defined(GASNETC_FH_OPTIONAL)
  /* Leave as-is */
#elif GASNET_DEBUG
  #define GASNETC_FH_OPTIONAL 1
#endif

/* check (even in optimized build) for VAPI errors */
#if GASNET_CONDUIT_VAPI
  #define GASNETC_VAPI_CHECK(rc,msg) \
    if_pf ((rc) != 0) \
      { gasneti_fatalerror("Unexpected error %s %s",VAPI_strerror_sym(rc),(msg)); }
#else
  #define GASNETC_VAPI_CHECK(rc,msg) \
    if_pf ((rc) != 0) \
      { gasneti_fatalerror("Unexpected error %s (rc=%d errno=%d) %s",strerror(errno),(rc), errno,(msg)); }
  #define GASNETC_VAPI_CHECK_PTR(ptr,msg) GASNETC_VAPI_CHECK((ptr)==NULL,(msg))
#endif

/* Identify nodes we do NOT use IB to communicate with */
#if GASNET_PSHM
  #define gasnetc_non_ib(_node) gasneti_pshm_in_supernode(_node)
#else
  #define gasnetc_non_ib(_node) ((_node) == gasneti_mynode)
#endif

#define GASNETC_CEP_SQ_SEMA(_cep) ((_cep)->sq_sema_p)

#if GASNETC_IBV_SRQ 
  #define GASNETC_QPI_IS_REQ(_qpi) ((_qpi) >= gasnetc_num_qps)
#else
  #define GASNETC_QPI_IS_REQ(_qpi) (0)
#endif

/* check for exit in progress */
extern gasneti_atomic_t gasnetc_exit_running;
#define GASNETC_IS_EXITING() gasneti_atomic_read(&gasnetc_exit_running, GASNETI_ATOMIC_RMB_PRE)

/* May eventually be a hash? */
#define GASNETC_NODE2CEP(_node) (gasnetc_node2cep[_node])

/* ------------------------------------------------------------------------------------ */
/* Core handlers.
 * These are registered early and are available even before _attach()
 */
#define GASNETC_HANDLER_BASE  1 /* reserve 1-63 for the core API */
#define _hidx_gasnetc_ack                     0 /* Special case */
#define _hidx_gasnetc_auxseg_reqh             (GASNETC_HANDLER_BASE+0)
#define _hidx_gasnetc_amrdma_grant_reqh       (GASNETC_HANDLER_BASE+1)
#define _hidx_gasnetc_exit_reduce_reqh        (GASNETC_HANDLER_BASE+2)
#define _hidx_gasnetc_exit_role_reqh          (GASNETC_HANDLER_BASE+3)
#define _hidx_gasnetc_exit_role_reph          (GASNETC_HANDLER_BASE+4)
#define _hidx_gasnetc_exit_reqh               (GASNETC_HANDLER_BASE+5)
#define _hidx_gasnetc_exit_reph               (GASNETC_HANDLER_BASE+6)
#define _hidx_gasnetc_sys_barrier_reqh        (GASNETC_HANDLER_BASE+7)
#define _hidx_gasnetc_sys_exchange_reqh       (GASNETC_HANDLER_BASE+8)
/* add new core API handlers here and to the bottom of gasnet_core.c */

#ifndef GASNETE_HANDLER_BASE
  #define GASNETE_HANDLER_BASE  64 /* reserve 64-127 for the extended API */
#elif GASNETE_HANDLER_BASE != 64
  #error "GASNETE_HANDLER_BASE mismatch between core and extended"
#endif

/* ------------------------------------------------------------------------------------ */
/* handler table (recommended impl) */
#define GASNETC_MAX_NUMHANDLERS   256
extern gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS];

/* ------------------------------------------------------------------------------------ */
/* AM category (recommended impl if supporting PSHM) */
typedef enum {
  gasnetc_Short=0,
  gasnetc_Medium=1,
  gasnetc_Long=2
} gasnetc_category_t;

/* ------------------------------------------------------------------------------------ */

#define GASNETC_ARGSEND_AUX(s,nargs) \
	(offsetof(s,args)+((nargs)*sizeof(gasnet_handlerarg_t)))

typedef struct {
#if GASNETI_STATS_OR_TRACE
  gasneti_tick_t	stamp;
#endif
  gasnet_handlerarg_t	args[GASNETC_MAX_ARGS];	
} gasnetc_shortmsg_t;
#define GASNETC_MSG_SHORT_ARGSEND(nargs) GASNETC_ARGSEND_AUX(gasnetc_shortmsg_t,nargs)

typedef struct {
#if GASNETI_STATS_OR_TRACE
  gasneti_tick_t	stamp;
#endif
  uint32_t		nBytes;	/* 16 bits would be sufficient if we ever need the space */
  gasnet_handlerarg_t	args[GASNETC_MAX_ARGS];	
} gasnetc_medmsg_t;
#define GASNETC_MSG_MED_ARGSEND(nargs) /* Note 8-byte alignment for payload */ \
		GASNETI_ALIGNUP(GASNETC_ARGSEND_AUX(gasnetc_medmsg_t,nargs), 8)
#define GASNETC_MSG_MED_DATA(msg,nargs) \
		((void *)((uintptr_t)(msg) + GASNETC_MSG_MED_ARGSEND(nargs)))

typedef struct {
#if GASNETI_STATS_OR_TRACE
  gasneti_tick_t	stamp;
#endif
  uintptr_t		destLoc;
  int32_t		nBytes;
  gasnet_handlerarg_t	args[GASNETC_MAX_ARGS];	
} gasnetc_longmsg_t;
#define GASNETC_MSG_LONG_ARGSEND(nargs)  GASNETC_ARGSEND_AUX(gasnetc_longmsg_t,nargs)
#define GASNETC_MSG_LONG_DATA(msg,nargs) (void *)(&msg->longmsg.args[(unsigned int)nargs])

typedef union {
  uint8_t		raw[GASNETC_BUFSZ];
#if GASNETI_STATS_OR_TRACE
  gasneti_tick_t	stamp;
#endif
  gasnetc_shortmsg_t	shortmsg;
  gasnetc_medmsg_t	medmsg;
  gasnetc_longmsg_t	longmsg;
} gasnetc_buffer_t;

/* ------------------------------------------------------------------------------------ */
/* System AM Request/Reply Functions
 * These can be called between init and attach.
 * They have an optional counter allowing one to test/block for local completion.
 */

extern int gasnetc_RequestSysShort(gasnet_node_t dest,
                                   gasnetc_counter_t *req_oust, /* counter for local completion */
                                   gasnet_handler_t handler,
                                   int numargs, ...);
extern int gasnetc_RequestSysMedium(gasnet_node_t dest,
                                    gasnetc_counter_t *req_oust, /* counter for local completion */
                                    gasnet_handler_t handler,
                                    void *source_addr, size_t nbytes,
                                    int numargs, ...);

extern int gasnetc_ReplySysShort(gasnet_token_t token,
                                 gasnetc_counter_t *req_oust, /* counter for local completion */
                                 gasnet_handler_t handler,
                                 int numargs, ...);
extern int gasnetc_ReplySysMedium(gasnet_token_t token,
                                  gasnetc_counter_t *req_oust, /* counter for local completion */
                                  gasnet_handler_t handler,
                                  void *source_addr, size_t nbytes,
                                  int numargs, ...);

/* ------------------------------------------------------------------------------------ */

#if GASNETI_STATS_OR_TRACE
  #define GASNETC_TRACE_WAIT_BEGIN() \
    gasneti_tick_t _waitstart = GASNETI_TICKS_NOW_IFENABLED(C)
#else 
  #define GASNETC_TRACE_WAIT_BEGIN() \
    static char _dummy = (char)sizeof(_dummy)
#endif

#define GASNETC_TRACE_WAIT_END(name) \
  GASNETI_TRACE_EVENT_TIME(C,name,gasneti_ticks_now() - _waitstart)

#define GASNETC_STAT_EVENT(name) \
  _GASNETI_STAT_EVENT(C,name)
#define GASNETC_STAT_EVENT_VAL(name,val) \
  _GASNETI_STAT_EVENT_VAL(C,name,val)

/* ------------------------------------------------------------------------------------ */
/* Configuration */

/* Maximum number of segments to gather on send */
#define GASNETC_SND_SG	4

/* maximum number of ops reaped from the send CQ per poll */
#ifndef GASNETC_SND_REAP_LIMIT
  #define GASNETC_SND_REAP_LIMIT	32
#endif

/* maximum number of ops reaped from the recv CQ per poll */
#ifndef GASNETC_RCV_REAP_LIMIT
  #define GASNETC_RCV_REAP_LIMIT	16
#endif

/* Define non-zero if we want to allow the mlock rlimit to bound the
 * amount of memory we will pin. */
#ifndef GASNETC_HONOR_RLIMIT_MEMLOCK
  #define GASNETC_HONOR_RLIMIT_MEMLOCK 0
#endif

/* Use alloca()?  (e.g. to work-around bug 2079) */
#ifdef GASNETI_USE_ALLOCA
  /* Keep defn */
#elif !PLATFORM_COMPILER_PGI
  #define GASNETI_USE_ALLOCA 1
#endif

/* Can one send a 0-byte payload?
 * TODO: autoconf or runtime probe if/when we can determine which systems need this
 */
#define GASNETC_ALLOW_0BYTE_MSG 0

/* Should dynamic connections use TCP-style rtt estimation? */
#ifndef GASNETC_CONN_USE_SRTT
  #define GASNETC_CONN_USE_SRTT 1
#endif

/* ------------------------------------------------------------------------------------ */

/* Semaphore, lifo and atomics wrappers
 *
 * Only for GASNETC_ANY_PAR do we need true atomics.
 * In particular neither PARSYNC nor CONN_THREAD introduce concurrency,
 * but use of "weak" atomics would pay the unnecessary costs for those.
 */
#if GASNETC_ANY_PAR
  typedef gasneti_semaphore_t gasnetc_sema_t;
  #define GASNETC_SEMA_INITIALIZER GASNETI_SEMAPHORE_INITIALIZER
  #define gasnetc_sema_init    gasneti_semaphore_init
  #define gasnetc_sema_read    gasneti_semaphore_read
  #define gasnetc_sema_up      gasneti_semaphore_up
  #define gasnetc_sema_up_n    gasneti_semaphore_up_n
  #define gasnetc_sema_trydown gasneti_semaphore_trydown

  typedef gasneti_lifo_head_t gasnetc_lifo_head_t;
  #define GASNETC_LIFO_INITIALIZER GASNETI_LIFO_INITIALIZER
  #define gasnetc_lifo_init      gasneti_lifo_init
  #define gasnetc_lifo_link      gasneti_lifo_link
  #define gasnetc_lifo_next      gasneti_lifo_next
  #define gasnetc_lifo_push      gasneti_lifo_push
  #define gasnetc_lifo_push_many gasneti_lifo_push_many
  #define gasnetc_lifo_pop       gasneti_lifo_pop

  typedef gasneti_atomic_t     gasnetc_atomic_t;
  typedef gasneti_atomic_val_t gasnetc_atomic_val_t;
  #define gasnetc_atomic_read      gasneti_atomic_read
  #define gasnetc_atomic_set       gasneti_atomic_set
  #define gasnetc_atomic_increment gasneti_atomic_increment
  #define gasnetc_atomic_decrement_and_test \
                                   gasneti_atomic_decrement_and_test
  #define gasnetc_atomic_compare_and_swap \
                                   gasneti_atomic_compare_and_swap
  #define gasnetc_atomic_add       gasneti_atomic_add
  #define gasnetc_atomic_subtract  gasneti_atomic_subtract
#else
  #define GASNETC_SEMA_MAX GASNETI_ATOMIC_MAX
  #if GASNET_DEBUG
    typedef struct {
      gasneti_atomic_val_t count;
      gasneti_atomic_val_t limit;
    } gasnetc_sema_t;
    #define GASNETC_SEMA_INITIALIZER(count,limit) { (count), (limit) }
    #define GASNETC_SEMA_CHECK(_s)        do {                      \
      gasneti_assert((_s)->count <= GASNETC_SEMA_MAX);              \
      gasneti_assert(((_s)->count <= (_s)->limit) || !(_s)->limit); \
    } while (0)
  #else
    typedef struct {
      gasneti_atomic_val_t count;
    } gasnetc_sema_t;
    #define GASNETC_SEMA_INITIALIZER(count,limit) { (count) }
    #define GASNETC_SEMA_CHECK(_s) do { } while (0)
  #endif

  GASNETI_INLINE(gasnetc_sema_init)
  void gasnetc_sema_init(gasnetc_sema_t *s,
                         gasneti_atomic_val_t value,
                         gasneti_atomic_val_t limit) {
    s->count = value;
  #if GASNET_DEBUG
    s->limit = limit;
  #endif
    GASNETC_SEMA_CHECK(s);
  }
  GASNETI_INLINE(gasnetc_sema_read)
  gasneti_atomic_val_t gasnetc_sema_read(gasnetc_sema_t *s) {
    GASNETC_SEMA_CHECK(s);
    return s->count;
  }
  GASNETI_INLINE(gasnetc_sema_up)
  void gasnetc_sema_up(gasnetc_sema_t *s) {
    GASNETC_SEMA_CHECK(s);
    s->count += 1;
    GASNETC_SEMA_CHECK(s);
  }
  GASNETI_INLINE(gasnetc_sema_up_n)
  void gasnetc_sema_up_n(gasnetc_sema_t *s, gasneti_atomic_val_t n) {
    GASNETC_SEMA_CHECK(s);
    s->count += n;
    GASNETC_SEMA_CHECK(s);
  }
  GASNETI_INLINE(gasnetc_sema_trydown)
  int gasnetc_sema_trydown(gasnetc_sema_t *s) {
    int retval;
    GASNETC_SEMA_CHECK(s);
    retval = s->count;
    if_pt (retval != 0)
      s->count -= 1;
    GASNETC_SEMA_CHECK(s);
    return retval;
  }

  typedef struct {
    void **head;
  } gasnetc_lifo_head_t;
  #define GASNETC_LIFO_INITIALIZER  { NULL }

  GASNETI_INLINE(gasnetc_lifo_init)
  void gasnetc_lifo_init(gasnetc_lifo_head_t *lifo) {
    gasneti_assert(lifo != NULL);
    lifo->head = NULL;
  }
  GASNETI_INLINE(_gasnetc_lifo_push)
  void _gasnetc_lifo_push(gasnetc_lifo_head_t *lifo, void **head, void **tail) {
    *tail = lifo->head;
    lifo->head = head;
  }
  GASNETI_INLINE(gasnetc_lifo_push)
  void gasnetc_lifo_push(gasnetc_lifo_head_t *lifo, void *elem) {
    gasneti_assert(lifo != NULL);
    gasneti_assert(elem != NULL);
    _gasnetc_lifo_push(lifo, elem, elem);
  }
  GASNETI_INLINE(gasnetc_lifo_push_many)
  void gasnetc_lifo_push_many(gasnetc_lifo_head_t *lifo, void *head, void *tail) {
    gasneti_assert(lifo != NULL);
    gasneti_assert(head != NULL);
    gasneti_assert(tail != NULL);
    _gasnetc_lifo_push(lifo, head, tail);
  }
  GASNETI_INLINE(gasnetc_lifo_pop) GASNETI_MALLOC
  void *gasnetc_lifo_pop(gasnetc_lifo_head_t *lifo) {
    void **elem;
    gasneti_assert(lifo != NULL);
    elem = lifo->head;
    if_pt (elem != NULL) {
      lifo->head = *elem;
    }
    return (void *)elem;
  }
  GASNETI_INLINE(gasnetc_lifo_link)
  void gasnetc_lifo_link(void *p, void *q) {
    gasneti_assert(p != NULL);
    gasneti_assert(q != NULL);
    *((void **)p) = q;
  }
  GASNETI_INLINE(gasnetc_lifo_next)
  void *gasnetc_lifo_next(void *elem) {
    gasneti_assert(elem != NULL);
    return *((void **)elem);
  }

  typedef gasneti_atomic_val_t gasnetc_atomic_t;
  typedef gasneti_atomic_val_t gasnetc_atomic_val_t;
  GASNETI_INLINE(gasnetc_atomic_read)
  gasnetc_atomic_val_t gasnetc_atomic_read(gasnetc_atomic_t *p, int flags) {
    return *p;
  }
  GASNETI_INLINE(gasnetc_atomic_set)
  void gasnetc_atomic_set(gasnetc_atomic_t *p, gasnetc_atomic_val_t val, int flags) {
    *p = val;
  }
  GASNETI_INLINE(gasnetc_atomic_increment)
  void gasnetc_atomic_increment(gasnetc_atomic_t *p, int flags) {
    ++(*p);
  }
  GASNETI_INLINE(gasnetc_atomic_decrement_and_test)
  int gasnetc_atomic_decrement_and_test(gasnetc_atomic_t *p, int flags) {
    return (0 == --(*p));
  }
  GASNETI_INLINE(gasnetc_atomic_compare_and_swap)
  int gasnetc_atomic_compare_and_swap(gasnetc_atomic_t *p, gasnetc_atomic_val_t oldval, gasnetc_atomic_val_t newval, int flags) {
    if (*p == oldval) {
      *p = newval;
      return 1;
    } else {
      return 0;
    }
  }
  GASNETI_INLINE(gasnetc_atomic_add)
  gasnetc_atomic_val_t gasnetc_atomic_add(gasnetc_atomic_t *p, gasnetc_atomic_val_t op, int flags) {
    return ((*p) += op);
  }
  GASNETI_INLINE(gasnetc_atomic_subtract)
  gasnetc_atomic_val_t gasnetc_atomic_subtract(gasnetc_atomic_t *p, gasnetc_atomic_val_t op, int flags) {
    return ((*p) -= op);
  }
#endif

/* ------------------------------------------------------------------------------------ */

#if (GASNETC_IB_MAX_HCAS > 1)
  #define GASNETC_FOR_ALL_HCA_INDEX(h)	for (h = 0; h < gasnetc_num_hcas; ++h)
  #define GASNETC_FOR_ALL_HCA(p)	for (p = &gasnetc_hca[0]; p < &gasnetc_hca[gasnetc_num_hcas]; ++p)
#else
  #define GASNETC_FOR_ALL_HCA_INDEX(h)	for (h = 0; h < 1; ++h)
  #define GASNETC_FOR_ALL_HCA(p)	for (p = &gasnetc_hca[0]; p < &gasnetc_hca[1]; ++p)
#endif

/* ------------------------------------------------------------------------------------ */
/* Map VAPI and IBV to a common gasnetc_ prefix */

#if GASNET_CONDUIT_VAPI
  #define GASNETC_IB_CHOOSE(X,Y)		X

  #define gasnetc_close_hca(_hca)		EVAPI_release_hca_hndl(_hca)
  #define gasnetc_alloc_pd(_hca)		VAPI_alloc_pd((_hca)->handle, &((_hca)->pd))
  #define gasnetc_dealloc_pd(_hca,_pd)		VAPI_dealloc_pd((_hca),(_pd))
  #define gasnetc_poll_snd_cq(_hca,_comp_p)	VAPI_poll_cq((_hca)->handle,(_hca)->snd_cq,(_comp_p))
  #define gasnetc_poll_rcv_cq(_hca,_comp_p)	VAPI_poll_cq((_hca)->handle,(_hca)->rcv_cq,(_comp_p))
  #define gasnetc_peek_snd_cq(_hca,_num)	EVAPI_peek_cq((_hca)->handle,(_hca)->snd_cq,(_num))
  #define gasnetc_peek_rcv_cq(_hca,_num)	EVAPI_peek_cq((_hca)->handle,(_hca)->rcv_cq,(_num))
  #define gasnetc_destroy_cq(_hca,_cq)		VAPI_destroy_cq((_hca),(_cq))
  #define gasnetc_destroy_qp(_hca,_qp)		VAPI_destroy_qp((_hca)->handle,(_qp))
  #define gasnetc_dereg_mr(_hca,_mr)		VAPI_deregister_mr((_hca),(_mr))
  #define gasnetc_query_port(_hca,_num,_port_p)	VAPI_query_hca_port_prop((_hca),(_num),(_port_p))
#else
  #define GASNETC_IB_CHOOSE(X,Y)		Y

  #define gasnetc_close_hca(_hca)		((void)ibv_close_device(_hca))
  #define gasnetc_alloc_pd(_hca)		(((_hca)->pd = ibv_alloc_pd((_hca)->handle)) == NULL)
  #define gasnetc_dealloc_pd(_hca,_pd)		((void)ibv_dealloc_pd(_pd))
  #define gasnetc_poll_snd_cq(_hca,_comp_p)	ibv_poll_cq((_hca)->snd_cq,1,(_comp_p))
  #define gasnetc_poll_rcv_cq(_hca,_comp_p)	ibv_poll_cq((_hca)->rcv_cq,1,(_comp_p))
  #define gasnetc_peek_snd_cq(_hca,_num)	ERROR___no_peek_cq_support
  #define gasnetc_peek_rcv_cq(_hca,_num)	ERROR___no_peek_cq_support
  #define gasnetc_destroy_cq(_hca,_cq)		ibv_destroy_cq(_cq)
  #define gasnetc_destroy_qp(_hca,_qp)		ibv_destroy_qp(_qp)
  #define gasnetc_dereg_mr(_hca,_mr)		ibv_dereg_mr(_mr)
  #define gasnetc_query_port(_hca,_num,_port_p)	ibv_query_port((_hca),(_num),(_port_p))
#endif

/* Constants */
#define GASNETC_PORT_DOWN	GASNETC_IB_CHOOSE(PORT_DOWN,		IBV_PORT_DOWN)
#define GASNETC_PORT_INIT	GASNETC_IB_CHOOSE(PORT_INITIALIZE,	IBV_PORT_INIT)
#define GASNETC_PORT_ACTIVE	GASNETC_IB_CHOOSE(PORT_ACTIVE,		IBV_PORT_ACTIVE)
#define GASNETC_PORT_ARMED	GASNETC_IB_CHOOSE(PORT_ARMED,		IBV_PORT_ARMED)

#define GASNETC_ACL_LOC_WR	GASNETC_IB_CHOOSE(VAPI_EN_LOCAL_WRITE,	IBV_ACCESS_LOCAL_WRITE)
#define GASNETC_ACL_REM_WR	GASNETC_IB_CHOOSE(VAPI_EN_REMOTE_WRITE,	IBV_ACCESS_REMOTE_WRITE)
#define GASNETC_ACL_REM_RD	GASNETC_IB_CHOOSE(VAPI_EN_REMOTE_READ,	IBV_ACCESS_REMOTE_READ)

#define GASNETC_INVAL_HNDL	GASNETC_IB_CHOOSE(VAPI_INVAL_HNDL,	NULL)

#define GASNETC_WC_SUCCESS	GASNETC_IB_CHOOSE(IB_COMP_SUCCESS,	IBV_WC_SUCCESS)
#define GASNETC_WC_FLUSH_ERR	GASNETC_IB_CHOOSE(IB_COMP_WR_FLUSH_ERR,	IBV_WC_WR_FLUSH_ERR)
#define GASNETC_WC_RDMA_READ	GASNETC_IB_CHOOSE(VAPI_CQE_SQ_RDMA_READ,IBV_WC_RDMA_READ)
#define GASNETC_WC_RDMA_WRITE	GASNETC_IB_CHOOSE(VAPI_CQE_SQ_RDMA_WRITE,IBV_WC_RDMA_WRITE)
#define GASNETC_WC_SEND		GASNETC_IB_CHOOSE(VAPI_CQE_SQ_SEND_DATA, IBV_WC_SEND)
#define GASNETC_WC_RECV		GASNETC_IB_CHOOSE(VAPI_CQE_RQ_SEND_DATA, IBV_WC_RECV)
#define GASNETC_WC_RETRY_EXC_ERR GASNETC_IB_CHOOSE(VAPI_RETRY_EXC_ERR, IBV_WC_RETRY_EXC_ERR)

#define GASNETC_WR_RDMA_READ	GASNETC_IB_CHOOSE(VAPI_RDMA_READ,	IBV_WR_RDMA_READ)
#define GASNETC_WR_RDMA_WRITE	GASNETC_IB_CHOOSE(VAPI_RDMA_WRITE,	IBV_WR_RDMA_WRITE)
#define GASNETC_WR_SEND_WITH_IMM GASNETC_IB_CHOOSE(VAPI_SEND_WITH_IMM,	IBV_WR_SEND_WITH_IMM)

#define GASNETC_POLL_CQ_OK	GASNETC_IB_CHOOSE(VAPI_OK,		1)
#define GASNETC_POLL_CQ_EMPTY	GASNETC_IB_CHOOSE(VAPI_CQ_EMPTY,	0)

/* Integer types */
typedef GASNETC_IB_CHOOSE(VAPI_lkey_t,		uint32_t)		gasnetc_lkey_t;
typedef GASNETC_IB_CHOOSE(VAPI_rkey_t,		uint32_t)		gasnetc_rkey_t;
typedef GASNETC_IB_CHOOSE(IB_port_t,		uint8_t)		gasnetc_port_t;
typedef GASNETC_IB_CHOOSE(IB_lid_t,		uint16_t)		gasnetc_lid_t;
typedef GASNETC_IB_CHOOSE(VAPI_mrw_acl_t,	enum ibv_access_flags)	gasnetc_acl_t;
typedef GASNETC_IB_CHOOSE(VAPI_wc_status_t,	enum ibv_wc_status)	gasnetc_wc_status_t;
typedef GASNETC_IB_CHOOSE(VAPI_wr_opcode_t,	enum ibv_wr_opcode)	gasnetc_wr_opcode_t;
typedef GASNETC_IB_CHOOSE(VAPI_cqe_num_t,	int)			gasnetc_cqe_cnt_t;
typedef GASNETC_IB_CHOOSE(VAPI_qp_num_t,	uint32_t)		gasnetc_qpn_t;

/* Handle types */
typedef GASNETC_IB_CHOOSE(VAPI_hca_hndl_t,	struct ibv_context *)	gasnetc_hca_hndl_t;
typedef GASNETC_IB_CHOOSE(VAPI_pd_hndl_t,	struct ibv_pd *)	gasnetc_pd_hndl_t;
typedef GASNETC_IB_CHOOSE(VAPI_mr_hndl_t,	struct ibv_mr *)	gasnetc_mr_hndl_t;
typedef GASNETC_IB_CHOOSE(VAPI_cq_hndl_t,	struct ibv_cq *)	gasnetc_cq_hndl_t;
typedef GASNETC_IB_CHOOSE(VAPI_qp_hndl_t,	struct ibv_qp *)	gasnetc_qp_hndl_t;

/* Misc */
typedef GASNETC_IB_CHOOSE(EVAPI_compl_handler_hndl_t, struct ibv_comp_channel *) gasnetc_comp_handler_t;
  
/* Attribute/capability structs */
typedef GASNETC_IB_CHOOSE(VAPI_hca_cap_t,	struct ibv_device_attr)	gasnetc_hca_cap_t;
typedef GASNETC_IB_CHOOSE(VAPI_hca_port_t,	struct ibv_port_attr)	gasnetc_hca_port_t;

/* Work requests and related stucts */
typedef GASNETC_IB_CHOOSE(VAPI_rr_desc_t,	struct ibv_recv_wr)	gasnetc_rcv_wr_t;
typedef GASNETC_IB_CHOOSE(VAPI_sr_desc_t,	struct ibv_send_wr)	gasnetc_snd_wr_t;
typedef GASNETC_IB_CHOOSE(VAPI_wc_desc_t,	struct ibv_wc)		gasnetc_wc_t;
typedef GASNETC_IB_CHOOSE(VAPI_sg_lst_entry_t,	struct ibv_sge)		gasnetc_sge_t;

/* Field names in gasnetc_hca_cap_t */
#define gasnetc_f_max_mr	GASNETC_IB_CHOOSE(max_num_mr,		max_mr)
#define gasnetc_f_max_qp	GASNETC_IB_CHOOSE(max_num_qp,		max_qp)
#define gasnetc_f_max_cq	GASNETC_IB_CHOOSE(max_num_cq,		max_cq)
#define gasnetc_f_max_cqe	GASNETC_IB_CHOOSE(max_num_ent_cq,	max_cqe)
#define gasnetc_f_max_sge	GASNETC_IB_CHOOSE(max_num_sg_ent,	max_sge)
#define gasnetc_f_max_qp_wr	GASNETC_IB_CHOOSE(max_qp_ous_wr,	max_qp_wr)
#define gasnetc_f_max_qp_rd_atom GASNETC_IB_CHOOSE(max_qp_ous_rd_atom,	max_qp_rd_atom)
#define gasnetc_f_max_ee_rd_atom GASNETC_IB_CHOOSE(max_ee_ous_rd_atom,	max_ee_rd_atom)
#define gasnetc_f_phys_port_cnt	GASNETC_IB_CHOOSE(phys_port_num,	phys_port_cnt)

/* Field names in work requests and the associated scatter/gather lists */
#define gasnetc_f_wr_id		GASNETC_IB_CHOOSE(id,			wr_id)
#define gasnetc_f_wr_num_sge	GASNETC_IB_CHOOSE(sg_lst_len,		num_sge)
#define gasnetc_f_wr_sg_list	GASNETC_IB_CHOOSE(sg_lst_p,		sg_list)
#define gasnetc_f_wr_rem_addr	GASNETC_IB_CHOOSE(remote_addr,		wr.rdma.remote_addr)
#define gasnetc_f_wr_rkey	GASNETC_IB_CHOOSE(r_key,		wr.rdma.rkey)
#define gasnetc_f_sg_len	GASNETC_IB_CHOOSE(len,			length)

/* ------------------------------------------------------------------------------------ */

/* Description of a pre-pinned memory region */
typedef struct {
  gasnetc_mr_hndl_t	handle;	/* used to release or modify the region */
  gasnetc_lkey_t	lkey;	/* used for local access by HCA */
  gasnetc_rkey_t	rkey;	/* used for remote access by HCA */
  uintptr_t		addr;
  size_t		len;
  uintptr_t		end;	/* inclusive */
} gasnetc_memreg_t;

typedef struct {
	/* Length excludes immediate data but zeros includes it */
	int16_t		length;	
	int16_t		length_again;
	int16_t		zeros;
	int16_t		zeros_again;
	/* Immediate data that vapi would otherwise send in its own header */
	uint32_t	immediate_data;
} gasnetc_amrdma_hdr_t;

#define GASNETC_AMRDMA_HDRSZ    sizeof(gasnetc_amrdma_hdr_t)
#define GASNETC_AMRDMA_SZ	4096 /* Keep to a power-of-2 */  /* XXX: should determine automatically */
#define GASNETC_AMRDMA_SZ_LG2	12 /* log-base-2(GASNETC_AMRDMA_SZ) */
#define GASNETC_AMRDMA_PAD      (GASNETI_ALIGNUP(GASNETC_AMRDMA_HDRSZ,SIZEOF_VOID_P) - GASNETC_AMRDMA_HDRSZ)
#define GASNETC_AMRDMA_LIMIT_MAX (GASNETC_AMRDMA_SZ - GASNETC_AMRDMA_HDRSZ - GASNETC_AMRDMA_PAD)
typedef char gasnetc_amrdma_buf_t[GASNETC_AMRDMA_SZ];

#define GASNETC_DEFAULT_AMRDMA_MAX_PEERS 32
#define GASNETC_AMRDMA_DEPTH_MAX	32	/* Power-of-2 <= 32 */
#define GASNETC_DEFAULT_AMRDMA_DEPTH	16
#define GASNETC_DEFAULT_AMRDMA_LIMIT	GASNETC_AMRDMA_LIMIT_MAX
#define GASNETC_DEFAULT_AMRDMA_CYCLE	1024	/* 2^i, Number of AM rcvs before hot-peer heuristic */

#if GASNETI_CONDUIT_THREADS
  typedef struct {
    /* Initialized by create_cq or spawn_progress_thread: */
    pthread_t               thread_id;
    uint64_t                prev_time;
    uint64_t                min_us;
    gasnetc_hca_hndl_t      hca;
    gasnetc_cq_hndl_t       cq;
    gasnetc_comp_handler_t  compl;
    volatile int            done;
    /* Initialized by client: */
    void                    (*fn)(gasnetc_wc_t *, void *);
    void                    *fn_arg;
  } gasnetc_progress_thread_t;
#else
  typedef void gasnetc_progress_thread_t;
#endif

/* Forward decl */
struct gasnetc_cep_t_;
typedef struct gasnetc_cep_t_ gasnetc_cep_t;

/* Struct for assignment of AMRDMA peers */
typedef struct gasnetc_amrdma_balance_tbl_t_ {
  gasnetc_atomic_val_t	count;
  gasnetc_cep_t		*cep;
} gasnetc_amrdma_balance_tbl_t;

/* Structure for an HCA */
typedef struct {
  gasnetc_hca_hndl_t	handle;
  gasnetc_memreg_t	rcv_reg;
  gasnetc_memreg_t	snd_reg;
#if GASNETC_PIN_SEGMENT
  gasnetc_memreg_t	*seg_reg;
  gasnetc_rkey_t	*rkeys;	/* RKey(s) registered at attach time */
#endif
#if GASNETC_IBV_SRQ
  struct ibv_srq	*rqst_srq;
  struct ibv_srq	*repl_srq;
  gasnetc_sema_t	am_sema;
#endif
  gasnetc_sema_t	*snd_cq_sema_p;
#if GASNETC_IBV_XRC
  struct ibv_xrc_domain *xrc_domain;
#endif
  gasnetc_cq_hndl_t	rcv_cq;
  gasnetc_cq_hndl_t	snd_cq; /* Includes Reply AMs when SRQ in use */
  gasnetc_pd_hndl_t	pd;
#if FIREHOSE_VAPI_USE_FMR
  EVAPI_fmr_t		fmr_props;
#endif
  int			hca_index;
  const char		*hca_id;
  gasnetc_hca_cap_t	hca_cap;
#if GASNET_CONDUIT_VAPI
  VAPI_hca_vendor_t	hca_vendor;
#else
  /* Part of hca_cap under ibv */
#endif
  int			qps; /* qps per peer */
  int			max_qps; /* maximum total over all peers */
  int			num_qps; /* current total over all peers */

  gasnetc_cep_t		**cep; /* array of ptrs to all ceps */

  void			*rbufs;
  gasnetc_lifo_head_t	rbuf_freelist;

#if GASNETC_IB_RCV_THREAD
  /* Rcv thread */
  gasnetc_progress_thread_t rcv_thread;
  void                      *rcv_thread_priv;
#endif

  /* AM-over-RMDA */
  gasnetc_memreg_t	amrdma_reg;
  gasnetc_lifo_head_t	amrdma_freelist;
  struct {
    gasnetc_atomic_val_t max_peers;
    gasnetc_atomic_t	count;
    gasnetc_cep_t	**cep;
  }	  amrdma_rcv;
  struct {
    gasnetc_atomic_t		count;
    gasnetc_atomic_val_t	mask;
#if GASNETC_ANY_PAR
    gasneti_atomic_t		lock;
#endif
    gasnetc_atomic_val_t	floor;
    gasnetc_amrdma_balance_tbl_t *table;
  }	  amrdma_balance;
} gasnetc_hca_t;

/* Structure for AM-over-RDMA sender state */
typedef struct {
  gasnetc_atomic_t	head, tail;
  gasnetc_rkey_t	rkey;
  uintptr_t		addr;	/* write ONCE */
} gasnetc_amrdma_send_t;

/* Structure for AM-over-RDMA receiver state */
typedef struct {
  gasnetc_amrdma_buf_t	*addr;	/* write ONCE */
  gasnetc_atomic_t	head;
#if GASNETC_ANY_PAR
  gasnetc_atomic_val_t tail;
  gasneti_mutex_t	ack_lock;
  uint32_t		ack_bits;
  char			_pad[GASNETI_CACHE_LINE_BYTES];
  union {
    gasnetc_atomic_t        spinlock;
    char		    _pad[GASNETI_CACHE_LINE_BYTES];
  }			busy[GASNETC_AMRDMA_DEPTH_MAX]; /* A weak spinlock array */
#endif
} gasnetc_amrdma_recv_t;

/* Structure for a cep (connection end-point) */
struct gasnetc_cep_t_ {
  /* Read/write fields */
  int                   used;           /* boolean - true if cep has sent traffic */
  gasnetc_sema_t	am_rem;		/* control in-flight AM Requests (remote rcv queue slots)*/
  gasnetc_sema_t	am_loc;		/* control unmatched rcv buffers (local rcv queue slots) */
  gasnetc_sema_t	*snd_cq_sema_p;	/* control in-flight ops (send completion queue slots) */
  gasnetc_sema_t	*sq_sema_p;	/* Pointer to a sq_sema */
  /* XXX: The atomics in the next 2 structs really should get padded to full cache lines */
  struct {	/* AM flow control coallescing */
  	gasnetc_atomic_t    credit;
	gasnetc_atomic_t    ack;
  } am_flow;
  /* AM-over-RDMA local state */
  gasnetc_atomic_t	amrdma_eligable;	/* Number of AMs small enough for AMRDMA */
  gasnetc_amrdma_send_t *amrdma_send;
  gasnetc_amrdma_recv_t *amrdma_recv;

#if GASNETI_THREADS
  char			_pad1[GASNETI_CACHE_LINE_BYTES];
#endif

  /* Read-only fields - many duplicated from fields in cep->hca */
#if GASNETC_PIN_SEGMENT
  gasnetc_rkey_t	*rkeys;	/* RKey(s) registered at attach time (== uint32_t) */
#endif
#if GASNETC_PIN_SEGMENT && (GASNETC_IB_MAX_HCAS > 1)
  gasnetc_memreg_t	*seg_reg;
#endif
#if (GASNETC_IB_MAX_HCAS > 1)
  gasnetc_lkey_t	rcv_lkey;
  gasnetc_lkey_t	snd_lkey;
#endif
  gasnetc_lifo_head_t	*rbuf_freelist;	/* Source of rcv buffers for AMs */
  gasnetc_hca_t		*hca;
  gasnetc_qp_hndl_t	qp_handle;
#if GASNET_CONDUIT_VAPI
  gasnetc_hca_hndl_t	hca_handle;
#endif
#if (GASNETC_IB_MAX_HCAS > 1)
  int			hca_index;
#endif
  gasnetc_epid_t	epid;		/* == uint32_t */
#if GASNETC_IBV_SRQ
  struct ibv_srq	*srq;
  gasnetc_qpn_t		rcv_qpn;
#endif
#if GASNETC_IBV_XRC
  uint32_t		xrc_remote_srq_num;
#endif

#if GASNETI_THREADS
  char			_pad2[GASNETI_CACHE_LINE_BYTES];
#endif
};

/* Info used while probing for HCAs/ports */
typedef struct {
  int                   hca_index;      /* Slot in gasnetc_hca[] */
  gasnetc_port_t        port_num;       /* Port number */
  gasnetc_hca_port_t    port;           /* Port info */
  int                   rd_atom;
  gasnetc_lid_t         *remote_lids;
} gasnetc_port_info_t;

/* Routines in gasnet_core_connect.c */
#if GASNETC_IBV_XRC
extern int gasnetc_xrc_init(void);
#endif
extern int gasnetc_connect_init(void);
extern int gasnetc_connect_fini(void);
#if GASNETC_DYNAMIC_CONNECT
extern gasnetc_cep_t *gasnetc_connect_to(gasnet_node_t node);
extern void gasnetc_conn_implied_ack(gasnet_node_t node);
extern void gasnetc_conn_rcv_wc(gasnetc_wc_t *comp);
extern void gasnetc_conn_snd_wc(gasnetc_wc_t *comp);
#endif

/* Routines in gasnet_core_sndrcv.c */
extern int gasnetc_create_cq(gasnetc_hca_hndl_t, gasnetc_cqe_cnt_t,
                             gasnetc_cq_hndl_t *, gasnetc_cqe_cnt_t *,
                             gasnetc_progress_thread_t *);
extern int gasnetc_sndrcv_limits(void);
extern int gasnetc_sndrcv_init(void);
extern void gasnetc_sndrcv_init_peer(gasnet_node_t node, gasnetc_cep_t *cep);
extern void gasnetc_sndrcv_init_inline(void);
extern void gasnetc_sndrcv_attach_peer(gasnet_node_t node, gasnetc_cep_t *cep);
extern void gasnetc_sndrcv_attach_segment(void);
extern void gasnetc_sndrcv_start_thread(void);
extern void gasnetc_sndrcv_stop_thread(void);
extern gasnetc_amrdma_send_t *gasnetc_amrdma_send_alloc(gasnetc_rkey_t rkey, void *addr);
extern gasnetc_amrdma_recv_t *gasnetc_amrdma_recv_alloc(gasnetc_hca_t *hca);
extern void gasnetc_sndrcv_poll(int handler_context);
extern int gasnetc_RequestGeneric(gasnetc_category_t category,
				  int dest, gasnet_handler_t handler,
				  void *src_addr, int nbytes, void *dst_addr,
				  int numargs, gasnetc_counter_t *mem_oust,
				  gasnetc_counter_t *req_oust, va_list argptr);
extern int gasnetc_ReplyGeneric(gasnetc_category_t category,
				gasnet_token_t token, gasnet_handler_t handler,
				void *src_addr, int nbytes, void *dst_addr,
				int numargs, gasnetc_counter_t *mem_oust,
				gasnetc_counter_t *req_oust, va_list argptr);

/* Routines in gasnet_core_thread.c */
#if GASNETI_CONDUIT_THREADS
extern void gasnetc_spawn_progress_thread(gasnetc_progress_thread_t *pthr);
extern void gasnetc_stop_progress_thread(gasnetc_progress_thread_t *pthr);
#endif

/* General routines in gasnet_core.c */
extern int gasnetc_pin(gasnetc_hca_t *hca, void *addr, size_t size, gasnetc_acl_t acl, gasnetc_memreg_t *reg);
extern void gasnetc_unpin(gasnetc_hca_t *hca, gasnetc_memreg_t *reg);
#define gasnetc_unmap(reg)	gasneti_munmap((void *)((reg)->addr), (reg)->len)

/* Bootstrap support */
extern void (*gasneti_bootstrapFini_p)(void);
extern void (*gasneti_bootstrapAbort_p)(int exitcode);
extern void (*gasneti_bootstrapAlltoall_p)(void *src, size_t len, void *dest);
extern void (*gasneti_bootstrapBroadcast_p)(void *src, size_t len, void *dest, int rootnode);
#define gasneti_bootstrapFini           (*gasneti_bootstrapFini_p)
#define gasneti_bootstrapAbort          (*gasneti_bootstrapAbort_p)
#define gasneti_bootstrapAlltoall       (*gasneti_bootstrapAlltoall_p)
#define gasneti_bootstrapBroadcast      (*gasneti_bootstrapBroadcast_p)
extern void gasneti_bootstrapBarrier(void);
extern void gasneti_bootstrapExchange(void *src, size_t len, void *dest);

/* Global configuration variables */
extern int		gasnetc_op_oust_limit;
extern int		gasnetc_op_oust_pp;
extern int		gasnetc_am_oust_limit;
extern int		gasnetc_am_oust_pp;
extern int		gasnetc_bbuf_limit;
#if GASNETC_DYNAMIC_CONNECT
  extern int		gasnetc_ud_rcvs;
  extern int		gasnetc_ud_snds;
#else
  #define		gasnetc_ud_rcvs 0
  #define		gasnetc_ud_snds 0
#endif
extern int		gasnetc_use_rcv_thread;
extern int		gasnetc_am_credits_slack;
extern int		gasnetc_alloc_qps;    /* Number of QPs per node in gasnetc_ceps[] */
extern int		gasnetc_num_qps;      /* How many QPs to use per peer */
extern size_t		gasnetc_packedlong_limit;
extern size_t		gasnetc_inline_limit;
extern size_t		gasnetc_bounce_limit;
#if !GASNETC_PIN_SEGMENT
  extern size_t		gasnetc_putinmove_limit;
#endif
#if GASNETC_FH_OPTIONAL
  #define GASNETC_USE_FIREHOSE	gasnetc_use_firehose
  extern int		gasnetc_use_firehose;
#else
  #define GASNETC_USE_FIREHOSE	1
#endif
extern int              gasnetc_qp_timeout;
extern int              gasnetc_qp_retry_count;
extern int		gasnetc_amrdma_max_peers;
extern size_t		gasnetc_amrdma_limit;
extern int		gasnetc_amrdma_depth;
extern int		gasnetc_amrdma_slot_mask;
extern gasnetc_atomic_val_t gasnetc_amrdma_cycle;

#if GASNETC_IBV_SRQ
  extern int			gasnetc_rbuf_limit;
  extern int			gasnetc_rbuf_set;
  extern int			gasnetc_use_srq;
#else
  #define gasnetc_use_srq	0
#endif

#if GASNETC_IBV_XRC
  extern int			gasnetc_use_xrc;
#else
  #define gasnetc_use_xrc	0
#endif

/* Global variables */
extern int		gasnetc_num_hcas;
extern gasnetc_hca_t	gasnetc_hca[GASNETC_IB_MAX_HCAS];
extern uintptr_t	gasnetc_max_msg_sz;
#if GASNETC_PIN_SEGMENT
  extern int			gasnetc_seg_reg_count;
  extern int			gasnetc_max_regs; /* max of gasnetc_seg_reg_count over all nodes */
  extern uintptr_t		gasnetc_seg_start;
  extern uintptr_t		gasnetc_seg_end;
  extern uint64_t		gasnetc_pin_maxsz;
  extern unsigned int		gasnetc_pin_maxsz_shift;
#endif
extern size_t			gasnetc_fh_align;
extern size_t			gasnetc_fh_align_mask;
extern firehose_info_t		gasnetc_firehose_info;
extern gasnetc_port_info_t      *gasnetc_port_tbl;
extern int                      gasnetc_num_ports;
extern gasnetc_cep_t            **gasnetc_node2cep;
extern gasnet_node_t            gasnetc_remote_nodes;
#if GASNETC_DYNAMIC_CONNECT
  extern gasnetc_sema_t         gasnetc_zero_sema;
#endif

#endif
