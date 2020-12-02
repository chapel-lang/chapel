/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ibv-conduit/gasnet_core_internal.h $
 * Description: GASNet ibv conduit header for internal definitions in Core API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <gasnet_internal.h>
#include <firehose.h>

#if GASNETC_IBV_ODP
  #define GASNETI_NEED_VERBS_EXP_H 1
#endif

#include <infiniband/verbs.h>
#if GASNETI_NEED_VERBS_EXP_H
  #include <infiniband/verbs_exp.h>
#endif

/* TODO: flatten these? */
  #if HAVE_IBV_SRQ
    #define GASNETC_IBV_SRQ 1
    #if !GASNET_PSHM
      #undef GASNETC_IBV_XRC
    #elif HAVE_IBV_CMD_OPEN_XRCD
      #define GASNETC_IBV_XRC 1
      #define GASNETC_IBV_XRC_OFED 1
      typedef struct ibv_xrcd gasnetc_xrcd_t;
    #elif HAVE_IBV_OPEN_XRC_DOMAIN
      #define GASNETC_IBV_XRC 1
      #define GASNETC_IBV_XRC_MLNX 1
      typedef struct ibv_xrc_domain gasnetc_xrcd_t;
    #endif
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

/* check (even in optimized build) for ibv errors */
#define GASNETC_IBV_CHECK(rc,msg) \
    if_pf ((rc) != 0) \
      { gasneti_fatalerror("Unexpected error %s (rc=%d errno=%d) %s",strerror(errno),(rc), errno,(msg)); }
#define GASNETC_IBV_CHECK_PTR(ptr,msg) GASNETC_IBV_CHECK((ptr)==NULL,(msg))

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
#define GASNETC_NODE2CEP(_ep,_node) ((_ep)->cep_table[_node])


/*
 * In theory all resources should be recovered automatically at process exit.
 * However, a least Solaris 11.2 has been seen to eventually begin returning
 * ENOSPC from ibv_create_cq() after a few thousand tests have run.
 * So, we will make a best-effort to at least destroy QPs and CQs.
 * This is also needed for BLCR-based checkpoint/restart suport.
 */
#if PLATFORM_OS_SOLARIS || GASNET_BLCR || GASNET_DEBUG
  #define GASNETC_IBV_SHUTDOWN 1
#endif

/* ------------------------------------------------------------------------------------ */
/* Core handlers.
 * These are registered early and are available even before _attach()
 */
#define _hidx_gasnetc_ack                     0 /* Special case */
#define _hidx_gasnetc_exchg_reqh              (GASNETC_HANDLER_BASE+0)
#define _hidx_gasnetc_exit_reduce_reqh        (GASNETC_HANDLER_BASE+1)
#define _hidx_gasnetc_exit_role_reqh          (GASNETC_HANDLER_BASE+2)
#define _hidx_gasnetc_exit_role_reph          (GASNETC_HANDLER_BASE+3)
#define _hidx_gasnetc_exit_reqh               (GASNETC_HANDLER_BASE+4)
#define _hidx_gasnetc_exit_reph               (GASNETC_HANDLER_BASE+5)
#define _hidx_gasnetc_sys_barrier_reqh        (GASNETC_HANDLER_BASE+6)
#define _hidx_gasnetc_sys_exchange_reqh       (GASNETC_HANDLER_BASE+7)
#define _hidx_gasnetc_sys_flush_reph          (GASNETC_HANDLER_BASE+8)
#define _hidx_gasnetc_sys_close_reqh          (GASNETC_HANDLER_BASE+9)
/* add new core API handlers here and to the bottom of gasnet_core.c */

/* ------------------------------------------------------------------------------------ */
/* Configure gasnet_event_internal.h and gasnet_event.c */
// TODO-EX: prefix needs to move from "extended" to "core"

#define gasnete_op_atomic_(_id) gasnetc_atomic_##_id

#define GASNETE_HAVE_LC

#define GASNETE_CONDUIT_EOP_FIELDS \
  gasnetc_atomic_val_t initiated_cnt; \
  gasnetc_atomic_t     completed_cnt; \
  gasnetc_atomic_val_t initiated_alc; \
  gasnetc_atomic_t     completed_alc;

#define GASNETE_EOP_ALLOC_EXTRA(_eop) do { \
    gasnetc_atomic_set(&(_eop)->completed_cnt, 0 , 0); \
    gasnetc_atomic_set(&(_eop)->completed_alc, 0 , 0); \
  } while (0)

#define GASNETE_EOP_PREP_FREE_EXTRA(_eop) do { \
    gasneti_assert(gasnetc_atomic_read(&(_eop)->completed_cnt, 0) \
                   == ((_eop)->initiated_cnt & GASNETI_ATOMIC_MAX)); \
    gasneti_assert(gasnetc_atomic_read(&(_eop)->completed_alc, 0) \
                   == ((_eop)->initiated_alc & GASNETI_ATOMIC_MAX)); \
  } while (0)

#define _GASNETE_EOP_NEW_EXTRA GASNETE_EOP_PREP_FREE_EXTRA

/* ------------------------------------------------------------------------------------ */
/* Internal threads */

/* GASNETC_USE_CONN_THREAD enables a progress thread for establishing dynamic connections. */
#if GASNETC_DYNAMIC_CONNECT && GASNETC_IBV_CONN_THREAD
  #define GASNETC_USE_CONN_THREAD 1
#else
  #define GASNETC_USE_CONN_THREAD 0
#endif

/* GASNETC_USE_RCV_THREAD enables a progress thread for running AMs. */
#if GASNETC_IBV_RCV_THREAD
  #define GASNETC_USE_RCV_THREAD 1
#else
  #define GASNETC_USE_RCV_THREAD 0
#endif

/* ------------------------------------------------------------------------------------ */
/* Measures of concurency
 *
 * GASNETC_ANY_PAR      Non-zero if multiple threads can be executing in GASNet.
 *                      This is inclusive of the AM receive thread.
 * GASNETC_CLI_PAR      Non-zero if multiple _client_ threads can be executing in GASNet.
 *                      This excludes the AM receive thread.
 * These differ from GASNETI_THREADS and GASNETI_CLIENT_THREADS in that they don't count
 * GASNET_PARSYNC, since it has threads which do not enter GASNet concurrently.
 */

#if GASNET_PAR
  #define GASNETC_CLI_PAR 1
#else
  #define GASNETC_CLI_PAR 0
#endif

#define GASNETC_ANY_PAR         (GASNETC_CLI_PAR || GASNETC_USE_RCV_THREAD)

/* ------------------------------------------------------------------------------------ */

#if GASNETC_PIN_SEGMENT
  /* Max firehose per op is one per local scatter/gather segment */
  #define GASNETC_MAX_FH	GASNETC_SND_SG
#else
  /* Max firehose per op is one per local scatter/gather segment + one remote */
  #define GASNETC_MAX_FH	(GASNETC_SND_SG + 1)
#endif

/* Define non-zero to allow loopback AMs to be assembled on the stack */
#define GASNETC_LOOPBACK_AMS_ON_STACK 1

/* ------------------------------------------------------------------------------------ */

#define GASNETC_ARGSEND_AUX(s,nargs) gasneti_offsetof(s,args[nargs])

typedef struct {
  gex_AM_Arg_t	args[GASNETC_MAX_ARGS];
} gasnetc_shortmsg_t;
#define GASNETC_MSG_SHORT_ARGSEND(nargs) GASNETC_ARGSEND_AUX(gasnetc_shortmsg_t,nargs)

typedef struct {
  uint32_t		nBytes;	/* 16 bits would be sufficient if we ever need the space */
  gex_AM_Arg_t	args[GASNETC_MAX_ARGS];
} gasnetc_medmsg_t;
#define GASNETC_MSG_MED_ARGSEND(nargs) /* Note 8-byte alignment for payload */ \
		GASNETI_ALIGNUP(GASNETC_ARGSEND_AUX(gasnetc_medmsg_t,nargs), 8)
#define GASNETC_MSG_MED_DATA(msg,nargs) \
		((void *)((uintptr_t)(msg) + GASNETC_MSG_MED_ARGSEND(nargs)))

typedef struct {
  uintptr_t		destLoc;
  int32_t		nBytes;
  gex_AM_Arg_t	args[GASNETC_MAX_ARGS];
} gasnetc_longmsg_t;
#define GASNETC_MSG_LONG_ARGSEND(nargs)  GASNETC_ARGSEND_AUX(gasnetc_longmsg_t,nargs)
#define GASNETC_MSG_LONG_DATA(msg,nargs) (void *)(&msg->longmsg.args[(unsigned int)nargs])

typedef union {
  uint8_t		raw[GASNETC_BUFSZ];
  gasnetc_shortmsg_t	shortmsg;
  gasnetc_medmsg_t	medmsg;
  gasnetc_longmsg_t	longmsg;
} gasnetc_buffer_t;

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
  #define GASNETC_HONOR_RLIMIT_MEMLOCK 1
#endif

/* Use alloca()?  (e.g. to work-around bug 2079) */
#ifdef GASNETI_USE_ALLOCA
  /* Keep defn */
#elif HAVE_ALLOCA && !PLATFORM_COMPILER_PGI
  #define GASNETI_USE_ALLOCA 1
#endif
#if GASNETI_USE_ALLOCA && HAVE_ALLOC_H
  #include <alloca.h>
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
  #define GASNETC_PARSEQ _PAR
  #define gasnetc_cons_atomic(_id) _CONCAT(gasneti_atomic_,_id)
#else
  #define GASNETC_PARSEQ _SEQ
  #define gasnetc_cons_atomic(_id) _CONCAT(gasneti_nonatomic_,_id)
#endif

#define GASNETC_SEMA_INITIALIZER  GASNETI_CONS_SEMA(GASNETC_PARSEQ,INITIALIZER)
#define gasnetc_sema_t            gasneti_cons_sema(GASNETC_PARSEQ,t)
#define gasnetc_sema_init         gasneti_cons_sema(GASNETC_PARSEQ,init)
#define gasnetc_sema_read         gasneti_cons_sema(GASNETC_PARSEQ,read)
#define gasnetc_sema_up           gasneti_cons_sema(GASNETC_PARSEQ,up)
#define gasnetc_sema_up_n         gasneti_cons_sema(GASNETC_PARSEQ,up_n)
#define gasnetc_sema_trydown      gasneti_cons_sema(GASNETC_PARSEQ,trydown)
#define gasnetc_sema_trydown_partial \
                                  gasneti_cons_sema(GASNETC_PARSEQ,trydown_partial)

#define GASNETC_LIFO_INITIALIZER  GASNETI_CONS_LIFO(GASNETC_PARSEQ,INITIALIZER)
#define gasnetc_lifo_head_t       gasneti_cons_lifo(GASNETC_PARSEQ,head_t)
#define gasnetc_lifo_init         gasneti_cons_lifo(GASNETC_PARSEQ,init)
#define gasnetc_lifo_pop          gasneti_cons_lifo(GASNETC_PARSEQ,pop)
#define gasnetc_lifo_push         gasneti_cons_lifo(GASNETC_PARSEQ,push)
#define gasnetc_lifo_push_many    gasneti_cons_lifo(GASNETC_PARSEQ,push_many)
#define gasnetc_lifo_link         gasneti_lifo_link
#define gasnetc_lifo_next         gasneti_lifo_next

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

/* ------------------------------------------------------------------------------------ */
/* Wrap mmap and munmap so we can do without them if required */

#if HAVE_MMAP
  #include <sys/mman.h> /* For MAP_FAILED */
  #define GASNETC_MMAP_FAILED        MAP_FAILED
  #define gasnetc_mmap(_size)        gasneti_mmap(_size)
  #define gasnetc_munmap(_ptr,_size) gasneti_munmap(_ptr,_size)
#else
  #define GASNETI_MMAP_GRANULARITY   (((size_t)1)<<22)  /* 4 MB */
  #define GASNETI_MMAP_LIMIT         (((size_t)1)<<31)  /* 2 GB */
  #define GASNETC_MMAP_FAILED        NULL
  extern int gasnetc_munmap(void *addr, size_t size); 
  GASNETI_INLINE(gasnetc_mmap)
  void *gasnetc_mmap(size_t size) {
    void *result;
    return posix_memalign(&result, GASNET_PAGESIZE, size) ? GASNETC_MMAP_FAILED : result;
  }
#endif

/* ------------------------------------------------------------------------------------ */
/* Type and ops for rdma counters */

typedef struct {
    gasnetc_atomic_t     completed;
    gasnetc_atomic_val_t initiated;
} gasnetc_counter_t;

#define GASNETC_COUNTER_INITIALIZER   {gasnetc_atomic_init(0), 0}

#define gasnetc_counter_init(P)       do { gasnetc_atomic_set(&(P)->completed, 0, 0); \
                                           (P)->initiated = 0;                         \
                                      } while (0)
#define gasnetc_counter_done(P)       (((P)->initiated & GASNETI_ATOMIC_MAX) == \
                                           gasnetc_atomic_read(&(P)->completed, 0))

#define gasnetc_counter_inc(P)		do { (P)->initiated++; } while (0)
#define gasnetc_counter_inc_by(P,v)	do { (P)->initiated += (v); } while (0)
#define gasnetc_counter_inc_if(P)	do { if(P) gasnetc_counter_inc(P); } while (0)
#define gasnetc_counter_inc_if_pf(P)	do { if_pf(P) gasnetc_counter_inc(P); } while (0)
#define gasnetc_counter_inc_if_pt(P)	do { if_pt(P) gasnetc_counter_inc(P); } while (0)

#define gasnetc_counter_dec(P)		gasnetc_atomic_increment(&(P)->completed, 0)
#define gasnetc_counter_dec_by(P,v)	gasnetc_atomic_add(&(P)->completed,(v),0)
#define gasnetc_counter_dec_if(P)	do { if(P) gasnetc_counter_dec(P); } while (0)
#define gasnetc_counter_dec_if_pf(P)	do { if_pf(P) gasnetc_counter_dec(P); } while (0)
#define gasnetc_counter_dec_if_pt(P)	do { if_pt(P) gasnetc_counter_dec(P); } while (0)

/* Wait until given counter is marked as done.
 * Note that no AMPoll is done in the best case.
 */
extern void gasnetc_counter_wait_aux(gasnetc_counter_t *counter, int handler_context GASNETI_THREAD_FARG);
GASNETI_INLINE(gasnetc_counter_wait)
void gasnetc_counter_wait(gasnetc_counter_t *counter, int handler_context GASNETI_THREAD_FARG) {
  if_pf (!gasnetc_counter_done(counter)) {
    gasnetc_counter_wait_aux(counter, handler_context GASNETI_THREAD_PASS);
  }
  gasneti_sync_reads();
} 

/* ------------------------------------------------------------------------------------ */

#if (GASNETC_IB_MAX_HCAS > 1)
  #define GASNETC_FOR_ALL_HCA_INDEX(h)	for (h = 0; h < gasnetc_num_hcas; ++h)
  #define GASNETC_FOR_ALL_HCA(p)	for (p = &gasnetc_hca[0]; p < &gasnetc_hca[gasnetc_num_hcas]; ++p)

  // Need a couple cache lines for dummy AMO accesses
  extern gasneti_auxseg_request_t gasnetc_fence_auxseg_alloc(gasnet_seginfo_t *auxseg_info);
  #define GASNETC_AUXSEG_FNS() gasnetc_fence_auxseg_alloc,

  // Use AMO after Put to fence for strict memory model adherence
  extern int gasnetc_use_fenced_puts;
  #define GASNETC_USE_FENCED_PUTS gasnetc_use_fenced_puts
#else
  #define GASNETC_FOR_ALL_HCA_INDEX(h)	for (h = 0; h < 1; ++h)
  #define GASNETC_FOR_ALL_HCA(p)	for (p = &gasnetc_hca[0]; p < &gasnetc_hca[1]; ++p)

  #define GASNETC_USE_FENCED_PUTS 0
#endif

/* ------------------------------------------------------------------------------------ */

// Either 0 or 1 to control use of IBV_SEND_SIGNALED
// Currently only for fencing on multi-rail
#define GASNETC_USE_SEND_SIGNALLED GASNETC_USE_FENCED_PUTS

/* ------------------------------------------------------------------------------------ */

/* Description of a pre-pinned memory region */
typedef struct {
  struct ibv_mr *	handle;	/* used to release or modify the region */
  uintptr_t		addr;
  size_t		len;
} gasnetc_memreg_t;

#if GASNETI_CONDUIT_THREADS
  typedef struct {
    /* Initialized by create_cq or spawn_progress_thread: */
    pthread_t               thread_id;
    uint64_t                prev_time;
    uint64_t                min_ns;
    struct ibv_cq *         cq;
    struct ibv_comp_channel *compl;
    volatile int            done;
    /* Initialized by client: */
    void                    (*fn)(struct ibv_wc *, void *);
    void                    *fn_arg;
  } gasnetc_progress_thread_t;
#else
  typedef void gasnetc_progress_thread_t;
#endif

/*
 * gasnetc_epid_d is node and qp encoded together
 * passing just a node (the default) means any qp to that node
 */
typedef uint32_t gasnetc_epid_t;

/* The 'epid' type holds 'node' in the low 16 bits.
 * The upper 16 bits holds a qp index (qpi).
 * A qpi of zero is a wildcard (an 'unbound' epid).
 * Therefore, setting epid=node means "use any qp for that node".
 * Non-zero qpi is 1 + the array index of the desired queue pair.
 */
#define gasnetc_epid2node(E)	((E)&0xffff)
#define gasnetc_epid2qpi(E)	((E)>>16)
#define gasnetc_epid(N,Q)	((N)|(((Q)+1)<<16))

/* Forward decl */
struct gasnetc_cep_t_;
typedef struct gasnetc_cep_t_ gasnetc_cep_t;

/* Structure for an HCA */
typedef struct {
  struct ibv_context *	handle;
  gasnetc_memreg_t	rcv_reg;
  gasnetc_memreg_t	snd_reg;
  gasnetc_memreg_t      aux_reg;
#if GASNETC_PIN_SEGMENT
  uint32_t              *rkeys; // RKeys registered at attach time
#endif
#if GASNETC_IBV_ODP
  struct {
    struct ibv_mr *     handle;
    uint32_t            lkey;
  } implicit_odp;
#endif
  uint32_t              *aux_rkeys;
#if GASNETC_IBV_SRQ
  struct ibv_srq	*rqst_srq;
  struct ibv_srq	*repl_srq;
  gasnetc_sema_t	am_sema;
#endif
  gasnetc_sema_t	*snd_cq_sema_p;
#if GASNETC_IBV_XRC
  gasnetc_xrcd_t *xrc_domain;
#endif
  struct ibv_cq *	rcv_cq;
  struct ibv_cq *	snd_cq; /* Includes Reply AMs when SRQ in use */
  struct ibv_pd *	pd;
  int			hca_index;
  const char		*hca_id;
  struct ibv_device_attr	hca_cap;
  int			qps; /* qps per peer */
  int			max_qps; /* maximum total over all peers */
  int			num_qps; /* current total over all peers */

  void			*rbufs;
  gasnetc_lifo_head_t	rbuf_freelist;

#if GASNETC_USE_RCV_THREAD
  /* Rcv thread */
  gasnetc_progress_thread_t rcv_thread;
  struct gasnetc_rbuf_s     *rcv_thread_priv;
 #if GASNETI_THREADINFO_OPT
  gasnet_threadinfo_t       rcv_threadinfo;
 #endif
#endif
} gasnetc_hca_t;

/* Structure for a cep (connection end-point) */
struct gasnetc_cep_t_ {
  /* Read/write fields */
  int                   used;           /* boolean - true if cep has sent traffic */
  gasnetc_sema_t	am_rem;		/* control in-flight AM Requests (remote rcv queue slots)*/
  gasnetc_sema_t	*snd_cq_sema_p;	/* control in-flight ops (send completion queue slots) */
  gasnetc_sema_t	*sq_sema_p;	/* Pointer to a sq_sema */
  /* XXX: The atomics in the next 2 structs really should get padded to full cache lines */
  struct {	/* AM flow control coallescing */
  	gasnetc_atomic_t    credit;
  } am_flow;

#if GASNETI_THREADS
  char			_pad1[GASNETI_CACHE_LINE_BYTES];
#endif

  /* Read-only fields - many duplicated from fields in cep->hca */
#if GASNETC_PIN_SEGMENT
  uint32_t      rkey;
#endif
#if (GASNETC_IB_MAX_HCAS > 1)
  uint32_t      rcv_lkey;
  uint32_t      snd_lkey;
#endif
  gasnetc_lifo_head_t	*rbuf_freelist;	/* Source of rcv buffers for AMs */
  gasnetc_hca_t		*hca;
  struct ibv_qp *       qp_handle;
#if (GASNETC_IB_MAX_HCAS > 1)
  int			hca_index;
#endif
  gasnetc_epid_t	epid;		/* == uint32_t */
#if GASNETC_IBV_SRQ
  struct ibv_srq        *srq;
  uint32_t              rcv_qpn;
#endif
#if GASNETC_IBV_XRC_OFED
  struct ibv_qp         *rcv_qp;
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
  uint8_t               port_num;       /* Port number */
  uint16_t              pkey_index;
  struct                ibv_port_attr    port;           /* Port info */
  int                   rd_atom;
  uint16_t             *remote_lids;
} gasnetc_port_info_t;

// Conduit-specific EP type
typedef struct gasnetc_EP_t_ {
  GASNETI_EP_COMMON // conduit-indep part as prefix

  gasnetc_cep_t     **cep_table;     // QP, flow-control, etc
} *gasnetc_EP_t;
extern gasnetc_EP_t gasnetc_ep0;

// Conduit-specific Segment type
typedef struct gasnetc_Segment_t_ {
  GASNETI_SEGMENT_COMMON // conduit-indep part as prefix

  int idx; // location in segment table

#if GASNETC_PIN_SEGMENT
  // memory registation info (per-HCA)
  uint32_t            seg_lkey[GASNETC_IB_MAX_HCAS];
  #if GASNETC_IBV_SHUTDOWN
    gasnetc_memreg_t  seg_reg[GASNETC_IB_MAX_HCAS];
  #endif
#endif
} *gasnetc_Segment_t;

/* Description of a receive buffer.
 *
 * Note that use of the freelist will overwrite the first sizeof(void *) bytes (linkage).
 * Note the macros following the typedef are used to hide the existence of the union.
 */
typedef struct gasnetc_rbuf_s {
  union {
    void 			*linkage;
    struct {
      /* Fields intialized at recv time: */
    #if GASNETI_THREADINFO_OPT
      gasnet_threadinfo_t       threadinfo;
    #endif
    #if GASNET_DEBUG
      int                   	handlerRunning;
    #endif
      int                   	needReply;
      uint32_t              	flags;
    }				am;
  } u;

  /* Field that changes each time the rbuf is posted */
  gasnetc_cep_t			*cep;

  /* Fields fixed for life of the rbuf as it is reused */
  int				rr_is_rdma;	/* is AM-over-RMDA? */
  struct ibv_recv_wr        	rr_desc;        /* recv request descriptor */
  struct ibv_sge			rr_sg;          /* single-entry scatter list */
  gasnetc_EP_t                  rr_ep;
} gasnetc_rbuf_t;
#if GASNET_DEBUG
#define rbuf_handlerRunning	u.am.handlerRunning
#endif
#define rbuf_needReply		u.am.needReply
#define rbuf_flags		u.am.flags
#define rbuf_threadinfo         u.am.threadinfo

#define GASNETC_OP_NEEDS_FENCE 0x1000 // Flag bit
typedef enum {
	GASNETC_OP_FREE,
	GASNETC_OP_AM,
	GASNETC_OP_ATOMIC,
	GASNETC_OP_GET_ZEROCP,
#if GASNETC_PIN_SEGMENT && GASNETC_FH_OPTIONAL
	GASNETC_OP_GET_BOUNCE,
#endif
#if GASNETC_IB_MAX_HCAS > 1
	GASNETC_OP_FENCE,
#endif
#if !GASNETC_PIN_SEGMENT
	GASNETC_OP_PUT_INMOVE,
#endif
        // Long payload puts do NOT need fencing (see bug 4049)
	GASNETC_OP_LONG_ZEROCP,
	GASNETC_OP_LONG_BOUNCE,
        // Following all have GASNETC_OP_NEEDS_FENCE bit set
	GASNETC_OP_PUT_INLINE = GASNETC_OP_NEEDS_FENCE,
	GASNETC_OP_PUT_ZEROCP,
	GASNETC_OP_PUT_BOUNCE,
	GASNETC_OP_INVALID
} gasnetc_sreq_opcode_t;

/* Use of IB's 32-bit immediate data:
 *   0-7: handlerID
 *   8-9: category
 * 10-14: numargs (5 bits, but only 0-GASNETC_MAX_ARGS are legal values)
 *    15: request (0) or reply (1)
 * 16-31: source node // TODO-EX: how to scale out past this limit?
 */

#define GASNETC_MSG_HANDLERID(flags)    ((gex_AM_Index_t)(flags))
#define GASNETC_MSG_CATEGORY(flags)     ((gasneti_category_t)(((flags) >> 8) & 0x3))
#define GASNETC_MSG_NUMARGS(flags)      (((flags) >> 10) & 0x1f)
#define GASNETC_MSG_ISREPLY(flags)      ((flags) & (1<<15))
#define GASNETC_MSG_ISREQUEST(flags)    (!GASNETC_MSG_ISREPLY(flags))
#define GASNETC_MSG_SRCIDX(flags)       ((gex_Rank_t)((flags) >> 16) & 0xffff)

#define GASNETC_MSG_GENFLAGS(isreq, cat, nargs, hand, srcidx)   \
 (gasneti_assert(0 == ((srcidx) & ~0xffff)),    \
  gasneti_assert(0 == ((nargs)  & ~0x1f)),      \
  gasneti_assert(0 == ((cat)    & ~3)),         \
  gasneti_assert((nargs) <= GASNETC_MAX_ARGS),  \
  gasneti_assert((srcidx) < gasneti_nodes),     \
  (uint32_t)(  ((nargs)   << 10        )        \
             | ((isreq)   ? 0 : (1<<15))        \
             | ((srcidx)  << 16        )        \
             | ((cat)     << 8         )        \
             | ((hand)                 )))

typedef void (*gasnetc_cb_t)(gasnetc_atomic_val_t *);

/* Description of a send request.
 *
 * Note that use of the freelist will overwrite the first sizeof(void *) bytes.
 */
typedef struct gasnetc_sreq_t_ {
  /* List linkage */
  struct gasnetc_sreq_t_	*next;

  /* Opcode for completion, and as tag for union */
  gasnetc_sreq_opcode_t		opcode;

  /* Communication end point */
  gasnetc_epid_t		epid;
  gasnetc_cep_t			*cep;

  /* Completion callback and initiated counter */
  struct {
    gasnetc_cb_t           cb;
    gasnetc_atomic_val_t   *data;
  }                             comp;

#if GASNETC_PIN_SEGMENT
  /* Firehose, bounce buffers, and AMs are mutually exclusive. */
  union {
    struct { /* Firehose data */
      int			count;
      const firehose_request_t	*ptr[GASNETC_MAX_FH];
    } fh;
    struct { /* Bounce buffer data */
      gasnetc_buffer_t		*buff;
      void			*addr;	/* local address for bounced GETs */
      size_t			len;	/* length for bounced GETs */
    } bb;
    struct { /* AM buffer */
      gasnetc_buffer_t		*buff;
    } am;
#if GASNETC_IB_MAX_HCAS > 1
    struct { // Atomic used to fence a multi-rail Put
      struct gasnetc_sreq_t_    *sreq;
    } fence;
#endif
  } u;
  #define fh_count	u.fh.count
  #define fh_ptr	u.fh.ptr
  #define bb_buff	u.bb.buff
  #define bb_addr	u.bb.addr
  #define bb_len	u.bb.len
  #define am_buff	u.am.buff
  #define fence_sreq    u.fence.sreq
#else
  /* Firehose, and AMs are mutually exclusive. */
  union {
    /* Firehose data */
    struct {
      int			count;
      const firehose_request_t	*ptr[GASNETC_MAX_FH];
      size_t			len;
      size_t			putinmove;	/* bytes piggybacked on an Move AM */
      uintptr_t			loc_addr;
      uintptr_t			rem_addr;
      gasnetc_EP_t              ep;
      gasnetc_buffer_t		*bbuf;
      gasnetc_atomic_t		ready;	/* 0 when loc and rem both ready */
      gasnetc_counter_t		*oust;	/* fh transactions outstanding */
      /* source memory refs completed (local completion) */
      gasnetc_atomic_val_t      *lc;
      gasnetc_cb_t              lc_cb;
    } fh;
    struct { /* AM buffer */
      gasnetc_buffer_t		*buff;
    } am;
#if GASNETC_IB_MAX_HCAS > 1
    struct { // Atomic used to fence a multi-rail Put
      struct gasnetc_sreq_t_    *sreq;
    } fence;
#endif
  } u;
  #define fh_count	u.fh.count
  #define fh_ptr	u.fh.ptr
  #define fh_len	u.fh.len
  #define fh_putinmove	u.fh.putinmove
  #define fh_loc_addr	u.fh.loc_addr
  #define fh_rem_addr	u.fh.rem_addr
  #define fh_ep         u.fh.ep
  #define fh_bbuf	u.fh.bbuf
  #define fh_ready	u.fh.ready
  #define fh_oust	u.fh.oust
  #define fh_lc 	u.fh.lc
  #define fh_lc_cb 	u.fh.lc_cb
  #define am_buff	u.am.buff
  #define fence_sreq    u.fence.sreq
#endif
} gasnetc_sreq_t;

/* Temporary buffer space used for constructing AMs on the stack */
typedef union {         
  gasnetc_shortmsg_t	shortmsg;
  gasnetc_medmsg_t	medmsg;
  gasnetc_longmsg_t	longmsg;
  uint8_t		raw[128];
} gasnetc_am_tmp_buf_t;

#if GASNETC_IB_MAX_HCAS > 1
  #define GASNETC_SND_LKEY(_cep)         ((_cep)->snd_lkey)
  #define GASNETC_RCV_LKEY(_cep)         ((_cep)->rcv_lkey)
  #define GASNETC_SEG_LKEY(_ep,_cep)     (((gasnetc_Segment_t)(_ep)->_segment)->seg_lkey[(_cep)->hca_index])
#else
  #define GASNETC_SND_LKEY(_cep)         (gasnetc_hca[0].snd_reg.handle->lkey)
  #define GASNETC_RCV_LKEY(_cep)         (gasnetc_hca[0].rcv_reg.handle->lkey)
  #define GASNETC_SEG_LKEY(_ep,_cep)     (((gasnetc_Segment_t)(_ep)->_segment)->seg_lkey[0])
#endif
#define GASNETC_SEG_RKEY(_cep)           ((_cep)->rkey)

/* ------------------------------------------------------------------------------------ */

/* Routines in gasnet_core_connect.c */
#if GASNETC_IBV_XRC
extern int gasnetc_xrc_init(void **shared_mem_p);
#endif
extern int gasnetc_connect_init(gasnetc_EP_t ep0); // TODO-EX: multi-ep support?
extern int gasnetc_connect_fini(gasnetc_EP_t ep0); // TODO-EX: multi-ep support?
#if GASNETC_IBV_SHUTDOWN
extern void gasnetc_connect_shutdown(gasnetc_EP_t ep0); // TODO-EX: multi-ep support?
#endif
#if GASNETC_DYNAMIC_CONNECT
extern gasnetc_cep_t *gasnetc_connect_to(gasnetc_EP_t ep, gex_Rank_t node);
extern void gasnetc_conn_implied_ack(gasnetc_EP_t ep, gex_Rank_t node);
extern void gasnetc_conn_rcv_wc(struct ibv_wc *comp);
extern void gasnetc_conn_snd_wc(struct ibv_wc *comp);
#endif

/* Callback functions in gasnet_core_sndrcv.c */
 /* eop: */
extern void gasnetc_cb_eop_alc(gasnetc_atomic_val_t *);
extern void gasnetc_cb_eop_put(gasnetc_atomic_val_t *);
extern void gasnetc_cb_eop_get(gasnetc_atomic_val_t *);
 /* iop within nbi-accessregion: */
extern void gasnetc_cb_nar_alc(gasnetc_atomic_val_t *);
extern void gasnetc_cb_nar_put(gasnetc_atomic_val_t *);
extern void gasnetc_cb_nar_get(gasnetc_atomic_val_t *);
 /* iop not in nbi-accessregion: */
extern void gasnetc_cb_iop_alc(gasnetc_atomic_val_t *);
extern void gasnetc_cb_iop_put(gasnetc_atomic_val_t *);
extern void gasnetc_cb_iop_get(gasnetc_atomic_val_t *);
 /* gasnetc_counter_t: */
extern void gasnetc_cb_counter(gasnetc_atomic_val_t *);
extern void gasnetc_cb_counter_rel(gasnetc_atomic_val_t *);

/* Routines in gasnet_core_sndrcv.c */
extern int gasnetc_create_cq(struct ibv_context *, int,
                             struct ibv_cq * *, int *,
                             gasnetc_progress_thread_t *);
extern int gasnetc_sndrcv_limits(void);
extern int gasnetc_sndrcv_init(gasnetc_EP_t);
extern void gasnetc_sys_flush_reph(gex_Token_t, gex_AM_Arg_t);
extern void gasnetc_sys_close_reqh(gex_Token_t);
extern void gasnetc_sndrcv_quiesce(void);
extern int gasnetc_sndrcv_shutdown(void);
extern void gasnetc_sndrcv_init_peer(gex_Rank_t node, gasnetc_cep_t *cep);
extern void gasnetc_sndrcv_init_inline(void);
extern void gasnetc_sndrcv_attach_peer(gex_Rank_t node, gasnetc_cep_t *cep);
extern void gasnetc_sndrcv_start_thread(void);
extern void gasnetc_sndrcv_stop_thread(int block);
extern void gasnetc_sndrcv_poll(int handler_context);
extern int gasnetc_rdma_put(
                  gex_TM_t tm, gex_Rank_t rank,
                  void *src_ptr, void *dst_ptr, size_t nbytes, gex_Flags_t flags,
                  gasnetc_atomic_val_t *local_cnt, gasnetc_cb_t local_cb,
                  gasnetc_atomic_val_t *remote_cnt, gasnetc_cb_t remote_cb
                  GASNETI_THREAD_FARG);
extern int gasnetc_rdma_long_put(
                  gasnetc_EP_t ep, gasnetc_cep_t *cep,
                  void *src_ptr, void *dst_ptr, size_t nbytes, gex_Flags_t flags,
                  gasnetc_atomic_val_t *local_cnt, gasnetc_cb_t local_cb
                  GASNETI_THREAD_FARG);
extern int gasnetc_rdma_get(
                  gex_TM_t tm, gex_Rank_t rank,
                  void *src_ptr, void *dst_ptr, size_t nbytes, gex_Flags_t flags,
                  gasnetc_atomic_val_t *remote_cnt, gasnetc_cb_t remote_cb
                  GASNETI_THREAD_FARG);

extern gasnetc_buffer_t *gasnetc_get_bbuf(int block GASNETI_THREAD_FARG) GASNETI_MALLOC;
GASNETI_MALLOCP(gasnetc_get_bbuf)
extern gasnetc_sreq_t *gasnetc_get_sreq(gasnetc_sreq_opcode_t opcode GASNETI_THREAD_FARG) GASNETI_MALLOC;
GASNETI_MALLOCP(gasnetc_get_sreq)

extern gasnetc_epid_t gasnetc_epid_select_qpi(gasnetc_cep_t *ceps, gasnetc_epid_t epid);
#if GASNETC_DYNAMIC_CONNECT
  extern gasnetc_cep_t *gasnetc_bind_cep_inner(gasnetc_EP_t ep, gasnetc_epid_t epid, gasnetc_sreq_t *sreq, int is_reply);
  #define gasnetc_bind_cep(ep,id,s)       gasnetc_bind_cep_inner((ep),(id),(s),0)
  #define gasnetc_bind_cep_am(ep,id,s,i)  gasnetc_bind_cep_inner((ep),(id),(s),(i))
#else
  extern gasnetc_cep_t *gasnetc_bind_cep_inner(gasnetc_EP_t ep, gasnetc_epid_t epid, gasnetc_sreq_t *sreq);
  #define gasnetc_bind_cep(ep,id,s)       gasnetc_bind_cep_inner((ep),(id),(s))
  #define gasnetc_bind_cep_am(ep,id,s,i)  gasnetc_bind_cep_inner((ep),(id),(s))
#endif
extern void gasnetc_snd_post_common(
                  gasnetc_sreq_t *sreq, struct ibv_send_wr *sr_desc,
                  int is_inline GASNETI_THREAD_FARG);

extern void gasnetc_poll_rcv_hca(gasnetc_EP_t ep, gasnetc_hca_t *hca, int limit GASNETI_THREAD_FARG);
extern void gasnetc_poll_rcv_all(gasnetc_EP_t ep, int limit GASNETI_THREAD_FARG);
extern void gasnetc_do_poll(int poll_rcv, int poll_snd GASNETI_THREAD_FARG);
#define gasnetc_poll_rcv()    gasnetc_do_poll(1,0 GASNETI_THREAD_PASS)
#define gasnetc_poll_snd()    gasnetc_do_poll(0,1 GASNETI_THREAD_PASS)
#define gasnetc_poll_both()   gasnetc_do_poll(1,1 GASNETI_THREAD_PASS)

/* Routines in gasnet_core_thread.c */
#if GASNETI_CONDUIT_THREADS
extern void gasnetc_spawn_progress_thread(gasnetc_progress_thread_t *pthr);
extern void gasnetc_stop_progress_thread(gasnetc_progress_thread_t *pthr, int block);
#endif

/* General routines in gasnet_core.c */
extern gex_Rank_t gasnetc_msgsource(gex_Token_t token);
extern int gasnetc_pin(gasnetc_hca_t *hca, void *addr, size_t size, enum ibv_access_flags acl, gasnetc_memreg_t *reg);
extern void gasnetc_unpin(gasnetc_hca_t *hca, gasnetc_memreg_t *reg);
#define gasnetc_unmap(reg)	gasnetc_munmap((void *)((reg)->addr), (reg)->len)

/* Bootstrap support */
extern gasneti_spawnerfn_t const *gasneti_spawner;

/* This indirection allows us to drop-in a native implementation after init */
extern void gasneti_bootstrapBarrier(void);
extern void gasneti_bootstrapExchange(void *src, size_t len, void *dest);
#define gasneti_bootstrapBroadcast      (*(gasneti_spawner->Broadcast))
#define gasneti_bootstrapSNodeBroadcast (*(gasneti_spawner->SNodeBroadcast))
#define gasneti_bootstrapAlltoall       (*(gasneti_spawner->Alltoall))
#define gasneti_bootstrapAbort          (*(gasneti_spawner->Abort))
#define gasneti_bootstrapCleanup        (*(gasneti_spawner->Cleanup))
#define gasneti_bootstrapFini           (*(gasneti_spawner->Fini))

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
extern size_t           gasnetc_am_inline_limit_sndrcv;
#if GASNETC_FH_OPTIONAL
  #define GASNETC_USE_FIREHOSE	GASNETT_PREDICT_TRUE(gasnetc_use_firehose)
  extern int		gasnetc_use_firehose;
#else
  #define GASNETC_USE_FIREHOSE	1
#endif
extern enum ibv_mtu    gasnetc_max_mtu;
extern int              gasnetc_qp_timeout;
extern int              gasnetc_qp_retry_count;

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

#if GASNETC_IBV_ODP
  extern int			gasnetc_use_odp;
#else
  #define gasnetc_use_odp	0
#endif

/* Global variables */
extern int		gasnetc_num_hcas;
extern gasnetc_hca_t	gasnetc_hca[GASNETC_IB_MAX_HCAS];
extern uintptr_t	gasnetc_max_msg_sz;
extern size_t   	gasnetc_put_stripe_sz, gasnetc_put_stripe_split;
extern size_t   	gasnetc_get_stripe_sz, gasnetc_get_stripe_split;
#if GASNETC_PIN_SEGMENT
  extern uintptr_t		gasnetc_seg_start;
  extern uintptr_t		gasnetc_seg_len;
#endif
extern size_t			gasnetc_fh_align;
extern size_t			gasnetc_fh_align_mask;
extern firehose_info_t		gasnetc_firehose_info;
extern gasnetc_port_info_t      *gasnetc_port_tbl;
extern int                      gasnetc_num_ports;
extern gex_Rank_t            gasnetc_remote_nodes;
#if GASNETC_DYNAMIC_CONNECT
  extern gasnetc_sema_t         gasnetc_zero_sema;
#endif

/* ------------------------------------------------------------------------------------ */
/* Small (inline) helper functions */

#define GASNETC_DECL_SR_DESC(_name, _sg_lst_len)            \
        struct ibv_send_wr _name[1];                        \
        struct ibv_sge _CONCAT(_name,_sg_lst)[_sg_lst_len]; \
        void *_CONCAT(_name,_dummy) = _gasnetc_sr_desc_init(_name, _CONCAT(_name,_sg_lst)) /* note intentional lack of final semicolon */
GASNETI_INLINE(_gasnetc_sr_desc_init)
void *_gasnetc_sr_desc_init(struct ibv_send_wr *result, struct ibv_sge *sg_lst_p)
{
#if GASNET_DEBUG
  result->num_sge = 0; /* invalid to ensure caller sets it */
#endif
  result->sg_list = sg_lst_p;
  return result;
}

GASNETI_INLINE(gasnetc_get_cep)
gasnetc_cep_t *gasnetc_get_cep(gasnetc_EP_t ep, gex_Rank_t node) {
  gasnetc_cep_t *result = GASNETC_NODE2CEP(ep, node);
#if GASNETC_DYNAMIC_CONNECT
  if_pf (!result) {
    result = gasnetc_connect_to(ep, node);
  }
#endif
  return result;
}

#if GASNETC_PIN_SEGMENT
/* Test if a given addr is in a given GASNet segment or not.
 * Returns non-zero if address is inside the segment.
 * This test is used under the assumption that the client's arguments
 * to Put or Get will always correspond to a region which is entirely
 * IN or entirely OUT of the segment.
 */
GASNETI_INLINE(gasnetc_in_segment)
int gasnetc_in_segment(const gasnetc_Segment_t seg, uintptr_t addr, size_t len) {
  if_pf (!seg) return 0;
  uint64_t offset = (uint64_t)addr - ((uint64_t)(uintptr_t)seg->_addr); // negative is a LARGE positive
  int result = (offset < ((uint64_t)seg->_size));
  gasneti_assume(!result || (addr+len <= (uintptr_t)seg->_ub)); // single-segment assumption
  return result;
}
GASNETI_INLINE(gasnetc_in_bound_segment)
int gasnetc_in_bound_segment(const gasnetc_EP_t ep, uintptr_t addr, size_t len) {
  gasneti_assert(ep);
  return gasnetc_in_segment((gasnetc_Segment_t)ep->_segment, addr, len);
}
#endif

/* ------------------------------------------------------------------------------------ */
/* To return failures internally - suited for both integer and pointer returns
 * We distinguish only 2 cases - temporary lack of resources or permanent failure
 */

#define GASNETC_OK       0
#define GASNETC_FAIL_IMM 1
#define GASNETC_FAIL_ERR 2

// To test a pointer value that could be valid, NULL or one of these two "FAIL" codes
#define GASNETC_FAILED_PTR(p) ((uintptr_t)(p)&0x3)


/* ------------------------------------------------------------------------------------ */
/* System AM Request/Reply Functions
 * These can be called between init and attach.
 * They have an optional counter allowing one to test/block for local completion,
 * and take an epid a dest to optionally allow selection of a specific QP.
 */

extern int gasnetc_RequestSysShort(gasnetc_epid_t dest,
                                   gasnetc_counter_t *counter, /* counter for local completion */
                                   gex_AM_Index_t handler,
                                   int numargs, ...);
extern int gasnetc_RequestSysMedium(gasnetc_epid_t dest,
                                    gasnetc_counter_t *counter, /* counter for local completion */
                                    gex_AM_Index_t handler,
                                    void *source_addr, size_t nbytes,
                                    int numargs, ...);

extern int gasnetc_ReplySysShort(gex_Token_t token,
                                 gasnetc_counter_t *counter, /* counter for local completion */
                                 gex_AM_Index_t handler,
                                 int numargs, ...);
extern int gasnetc_ReplySysMedium(gex_Token_t token,
                                  gasnetc_counter_t *counter, /* counter for local completion */
                                  gex_AM_Index_t handler,
                                  void *source_addr, size_t nbytes,
                                  int numargs, ...);

#endif
