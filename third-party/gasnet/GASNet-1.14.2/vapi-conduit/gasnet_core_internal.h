/*   $Source: /var/local/cvs/gasnet/vapi-conduit/gasnet_core_internal.h,v $
 *     $Date: 2010/04/17 00:11:40 $
 * $Revision: 1.159.2.1 $
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

/* check for exit in progress */
extern gasneti_atomic_t gasnetc_exit_running;
#define GASNETC_IS_EXITING() gasneti_atomic_read(&gasnetc_exit_running, GASNETI_ATOMIC_RMB_PRE)

/* ------------------------------------------------------------------------------------ */
#define GASNETC_HANDLER_BASE  1 /* reserve 1-63 for the core API */
#define _hidx_gasnetc_auxseg_reqh             (GASNETC_HANDLER_BASE+0)
#define _hidx_gasnetc_amrdma_grant_reqh       (GASNETC_HANDLER_BASE+1)
/* add new core API handlers here and to the bottom of gasnet_core.c */

/* System-category handlers.
 * These form a separate AM handler space and are available even before _attach()
 */
#define _hidx_gasnetc_SYS_ack             0
#define _hidx_gasnetc_SYS_exit_role_req   1
#define _hidx_gasnetc_SYS_exit_role_rep   2
#define _hidx_gasnetc_SYS_exit_req        3
#define _hidx_gasnetc_SYS_exit_rep        4
#define _hidx_gasnetc_SYS_init_ping       5

/* ------------------------------------------------------------------------------------ */
/* handler table (recommended impl) */
#define GASNETC_MAX_NUMHANDLERS   256
extern gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS];
extern const gasneti_handler_fn_t gasnetc_sys_handler[GASNETC_MAX_NUMHANDLERS];

#if GASNET_PSHM
  #define GASNETC_SYS_HANDLER_FLAG (GASNETC_MAX_NUMHANDLERS << 1)
  #define gasnetc_get_handler(_idx) (((_idx)&GASNETC_SYS_HANDLER_FLAG) \
                                     ? gasnetc_sys_handler[(_idx)^GASNETC_SYS_HANDLER_FLAG] \
                                     : gasnetc_handler[(_idx)])
#endif

/* ------------------------------------------------------------------------------------ */
/* AM category (recommended impl if supporting PSHM) */
typedef enum {
  gasnetc_Short=0,
  gasnetc_Medium=1,
  gasnetc_Long=2,
  gasnetc_System=3 /* Conduit-specific addition */
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

#define GASNETC_RUN_HANDLER_SYS(isReq, hid, phandlerfn, token, pArgs, numargs) do {       \
    if (isReq) GASNETC_TRACE_SYSTEM_REQHANDLER(hid, token, numargs, pArgs);               \
    else       GASNETC_TRACE_SYSTEM_REPHANDLER(hid, token, numargs, pArgs);               \
    if (GASNET_PSHM || phandlerfn) { /* NULL handler only possible when !GASNET_PSHM */   \
      if (numargs == 0) (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token);       \
      else {                                                                              \
        gasnet_handlerarg_t *_args = (gasnet_handlerarg_t *)(pArgs); /* eval only once */ \
        switch (numargs) {                                                                \
          case 1:  (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0]); break; \
          case 2:  (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0], _args[1]); break;\
          default: gasneti_fatalerror("Illegal numargs=%i in GASNETC_RUN_HANDLER_SYS", (int)numargs); \
        }                                                                                 \
      }                                                                                   \
    }                                                                                     \
    GASNETI_TRACE_PRINTF(C,("AM%s_SYS_HANDLER: handler execution complete", (isReq?"REQUEST":"REPLY"))); \
  } while (0)  


#if GASNET_TRACE
  #define _GASNETC_TRACE_SYSTEM(name,dest,handler,numargs) do {                        \
    _GASNETI_TRACE_GATHERARGS(numargs);                                                \
    _GASNETI_STAT_EVENT(C,name);                                                       \
    GASNETI_TRACE_PRINTF(C,(#name": dest=%i handler=%i args:%s",dest,handler,argstr)); \
  } while(0)
  #define GASNETC_TRACE_SYSTEM_REQUEST(dest,handler,numargs) \
          _GASNETC_TRACE_SYSTEM(AMREQUEST_SYS,dest,handler,numargs)
  #define GASNETC_TRACE_SYSTEM_REPLY(dest,handler,numargs) \
          _GASNETC_TRACE_SYSTEM(AMREPLY_SYS,dest,handler,numargs)

  #define _GASNETC_TRACE_SYSTEM_HANDLER(name, handlerid, token, numargs, arghandle) do { \
    gasnet_node_t src;                                                                    \
    _GASNETI_TRACE_GATHERHANDLERARGS(numargs, arghandle);                                 \
    _GASNETI_STAT_EVENT(C,name);                                                          \
    if (gasnet_AMGetMsgSource(token,&src) != GASNET_OK)                                   \
	gasneti_fatalerror("gasnet_AMGetMsgSource() failed");                               \
    GASNETI_TRACE_PRINTF(C,(#name": src=%i handler=%i args:%s",                           \
      (int)src,(int)(handlerid),argstr));                                                 \
    GASNETI_TRACE_PRINTF(C,(#name": token: %s",                                           \
                      gasneti_formatdata(&token, sizeof(token))));                        \
    } while(0)
  #define GASNETC_TRACE_SYSTEM_REQHANDLER(handlerid, token, numargs, arghandle) \
         _GASNETC_TRACE_SYSTEM_HANDLER(AMREQUEST_SYS_HANDLER, handlerid, token, numargs, arghandle)
  #define GASNETC_TRACE_SYSTEM_REPHANDLER(handlerid, token, numargs, arghandle) \
         _GASNETC_TRACE_SYSTEM_HANDLER(AMREPLY_SYS_HANDLER, handlerid, token, numargs, arghandle)
#else
  #define GASNETC_TRACE_SYSTEM_REQUEST(dest,handler,numargs)
  #define GASNETC_TRACE_SYSTEM_REPLY(dest,handler,numargs)
  #define GASNETC_TRACE_SYSTEM_REQHANDLER(handlerid, token, numargs, arghandle) 
  #define GASNETC_TRACE_SYSTEM_REPHANDLER(handlerid, token, numargs, arghandle) 
#endif

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

/* Defined non-zero in gasnet_config.h to enable a progress thread for receiving AMs . */
#if GASNET_CONDUIT_VAPI
  #ifndef GASNETC_VAPI_RCV_THREAD
    #define GASNETC_IB_RCV_THREAD	0
  #else
    #define GASNETC_IB_RCV_THREAD	1
  #endif
#else
  /* XXX: rcv thread not yet implemented for IBV */
  #define GASNETC_IB_RCV_THREAD 0
#endif

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

/* ------------------------------------------------------------------------------------ */

/* Measures of concurency
 *
 * GASNETC_ANY_PAR	Non-zero if multiple threads can be executing in GASNet.
 * 			This is inclusive of the AM receive thread.
 * GASNETC_CLI_PAR	Non-zero if multiple _client_ threads can be executing in GASNet.
 * 			This excludes the AM receive thread.
 * These differ from GASNETI_THREADS and GASNETI_CLIENT_THREADS in that they don't count
 * GASNET_PARSYNC, since it has threads which do not enter GASNet concurrently.
 */

#if GASNET_PAR
  #define GASNETC_CLI_PAR	1
#else
  #define GASNETC_CLI_PAR	0
#endif

#define GASNETC_ANY_PAR		(GASNETC_CLI_PAR || GASNETC_IB_RCV_THREAD)

/* ------------------------------------------------------------------------------------ */

#if GASNETC_IB_MAX_HCAS > 1
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
  #define gasnetc_destroy_qp(_hca,_qp)		VAPI_destroy_qp((_hca),(_qp))
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

  /* Work-around bug 1861: node death on exit: */
  #undef gasnetc_close_hca
  #define gasnetc_close_hca(_hca)		((void)0)
  #undef gasnetc_dealloc_pd
  #define gasnetc_dealloc_pd(_hca,_pd)		((void)0)
#endif

/* Constants */
#define GASNETC_PORT_DOWN	GASNETC_IB_CHOOSE(PORT_DOWN,		IBV_PORT_DOWN)
#define GASNETC_PORT_INIT	GASNETC_IB_CHOOSE(PORT_INITIALIZE,	IBV_PORT_INIT)
#define GASNETC_PORT_ACTIVE	GASNETC_IB_CHOOSE(PORT_ACTIVE,		IBV_PORT_ACTIVE)
#define GASNETC_PORT_ARMED	GASNETC_IB_CHOOSE(PORT_ARMED,		IBV_PORT_ARMED)

#define GASNETC_ACL_LOC_WR	GASNETC_IB_CHOOSE(VAPI_EN_LOCAL_WRITE,	IBV_ACCESS_LOCAL_WRITE)
#define GASNETC_ACL_REM_WR	GASNETC_IB_CHOOSE(VAPI_EN_REMOTE_WRITE,	IBV_ACCESS_REMOTE_WRITE)
#define GASNETC_ACL_REM_RD	GASNETC_IB_CHOOSE(VAPI_EN_REMOTE_READ,	IBV_ACCESS_REMOTE_READ)

#define GASNETC_INVAL_MR_HNDL	GASNETC_IB_CHOOSE(VAPI_INVAL_HNDL,	NULL)

#define GASNETC_WC_SUCCESS	GASNETC_IB_CHOOSE(IB_COMP_SUCCESS,	IBV_WC_SUCCESS)
#define GASNETC_WC_FLUSH_ERR	GASNETC_IB_CHOOSE(IB_COMP_WR_FLUSH_ERR,	IBV_WC_WR_FLUSH_ERR)
#define GASNETC_WC_RDMA_READ	GASNETC_IB_CHOOSE(VAPI_CQE_SQ_RDMA_READ,IBV_WC_RDMA_READ)
#define GASNETC_WC_RDMA_WRITE	GASNETC_IB_CHOOSE(VAPI_CQE_SQ_RDMA_WRITE,IBV_WC_RDMA_WRITE)
#define GASNETC_WC_SEND		GASNETC_IB_CHOOSE(VAPI_CQE_SQ_SEND_DATA, IBV_WC_SEND)
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
typedef GASNETC_IB_CHOOSE(VAPI_mrw_acl_t,	enum ibv_access_flags)	gasnetc_acl_t;
typedef GASNETC_IB_CHOOSE(VAPI_wc_status_t,	enum ibv_wc_status)	gasnetc_wc_status_t;
typedef GASNETC_IB_CHOOSE(VAPI_wr_opcode_t,	enum ibv_wr_opcode)	gasnetc_wr_opcode_t;
typedef GASNETC_IB_CHOOSE(VAPI_cqe_num_t,	int)			gasnetc_cqe_cnt_t;

/* Handle types */
typedef GASNETC_IB_CHOOSE(VAPI_hca_hndl_t,	struct ibv_context *)	gasnetc_hca_hndl_t;
typedef GASNETC_IB_CHOOSE(VAPI_pd_hndl_t,	struct ibv_pd *)	gasnetc_pd_hndl_t;
typedef GASNETC_IB_CHOOSE(VAPI_mr_hndl_t,	struct ibv_mr *)	gasnetc_mr_hndl_t;
typedef GASNETC_IB_CHOOSE(VAPI_cq_hndl_t,	struct ibv_cq *)	gasnetc_cq_hndl_t;
typedef GASNETC_IB_CHOOSE(VAPI_qp_hndl_t,	struct ibv_qp *)	gasnetc_qp_hndl_t;
  
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

/* Forward decl */
struct gasnetc_cep_t_;
typedef struct gasnetc_cep_t_ gasnetc_cep_t;

/* Struct for assignment of AMRDMA peers */
typedef struct gasnetc_amrdma_balance_tbl_t_ {
  gasneti_weakatomic_val_t	count;
  gasnetc_cep_t			*cep;
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
  gasnetc_cq_hndl_t	rcv_cq;
  gasnetc_cq_hndl_t	snd_cq;
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
  int			total_qps; /* total over all peers */

  gasnetc_cep_t		**cep; /* array of ptrs to all ceps */

  void			*rbuf_alloc;
  gasneti_lifo_head_t	rbuf_freelist;

#if GASNET_CONDUIT_VAPI
  /* Rcv thread */
  EVAPI_compl_handler_hndl_t rcv_handler;
  void			*rcv_thread_priv;
#else
  /* No progress thread under ibv */
#endif

  /* AM-over-RMDA */
  gasnetc_memreg_t	amrdma_reg;
  gasneti_lifo_head_t	amrdma_freelist;
  struct {
    gasneti_weakatomic_val_t max_peers;
    gasneti_weakatomic_t count;
    gasnetc_cep_t	**cep;
  }	  amrdma_rcv;
  struct {
    gasneti_weakatomic_t	count;
    gasneti_weakatomic_val_t	mask;
#if GASNETI_THREADS
    gasneti_atomic_t		lock;
#endif
    gasneti_weakatomic_val_t	floor;
    gasnetc_amrdma_balance_tbl_t *table;
  }	  amrdma_balance;
} gasnetc_hca_t;

/* Keys in a cep, all replicated from other data */
struct gasnetc_cep_keys_ {
#if GASNETC_PIN_SEGMENT
  gasnetc_memreg_t	*seg_reg;
  gasnetc_rkey_t	*rkeys;	/* RKey(s) registered at attach time (== uint32_t) */
#endif
  gasnetc_lkey_t	rcv_lkey;
  gasnetc_lkey_t	snd_lkey;
  gasnetc_rkey_t	amrdma_rkey;
};

/* Structure for a cep (connection end-point) */
struct gasnetc_cep_t_ {
  char			_pad0[GASNETI_CACHE_LINE_BYTES];

  /* Read/write fields */
  gasneti_semaphore_t	sq_sema;	/* control in-flight ops (send queue slots) */
  gasneti_semaphore_t	am_rem;		/* control in-flight AM Requests (remote rcv queue slots)*/
  gasneti_semaphore_t	am_loc;		/* control unmatched rcv buffers (local rcv queue slots) */
  gasneti_semaphore_t	*snd_cq_sema_p;	/* control in-flight ops (send completion queue slots) */
  /* XXX: The atomics in the next 2 structs really should get padded to full cache lines */
  struct {	/* AM flow control coallescing */
  	gasneti_weakatomic_t	credit;
	gasneti_weakatomic_t	ack;
  } am_flow;
  struct {	/* AM-over-RDMA local state */
	gasneti_weakatomic_t	send_head, send_tail;
	gasneti_weakatomic_t	recv_head;
#if GASNETI_THREADS
	gasneti_mutex_t		ack_lock;
	uint32_t		ack_bits;
        gasneti_weakatomic_val_t recv_tail;
	char			_pad[GASNETI_CACHE_LINE_BYTES];
	union {
          gasneti_weakatomic_t	    spinlock;
	  char			    _pad[GASNETI_CACHE_LINE_BYTES];
        }			recv_busy[GASNETC_AMRDMA_DEPTH_MAX]; /* A weak spinlock */
#endif
	gasneti_weakatomic_t	eligable;	/* Number of AMs small enough for AMRDMA */
  } amrdma;

  char			_pad1[GASNETI_CACHE_LINE_BYTES];

  /* Read-only fields */
  struct gasnetc_cep_keys_ keys;
  gasneti_lifo_head_t	*rbuf_freelist;	/* Source of rcv buffers for AMs */
  gasnetc_hca_t		*hca;
  gasnetc_qp_hndl_t	qp_handle;
  gasnetc_hca_hndl_t	hca_handle;
  int			hca_index;
  gasnetc_epid_t	epid;		/* == uint32_t */
  gasnetc_amrdma_buf_t	*amrdma_loc;	
  uintptr_t		amrdma_rem;

  char			_pad2[GASNETI_CACHE_LINE_BYTES];
};

/* Routines in gasnet_core_sndrcv.c */
extern int gasnetc_sndrcv_init(void);
extern void gasnetc_sndrcv_fini(void);
extern void gasnetc_sndrcv_init_peer(gasnet_node_t node);
extern void gasnetc_sndrcv_init_misc(void);
extern void gasnetc_sndrcv_attach_peer(gasnet_node_t node);
extern void gasnetc_sndrcv_fini_peer(gasnet_node_t node);
extern void gasnetc_sndrcv_poll(void);
extern int gasnetc_RequestGeneric(gasnetc_category_t category,
				  int dest, gasnet_handler_t handler,
				  void *src_addr, int nbytes, void *dst_addr,
				  int numargs, gasnetc_counter_t *mem_oust, va_list argptr);
extern int gasnetc_ReplyGeneric(gasnetc_category_t category,
				gasnet_token_t token, gasnet_handler_t handler,
				void *src_addr, int nbytes, void *dst_addr,
				int numargs, gasnetc_counter_t *mem_oust, va_list argptr);

/* General routines in gasnet_core.c */
extern int gasnetc_pin(gasnetc_hca_t *hca, void *addr, size_t size, gasnetc_acl_t acl, gasnetc_memreg_t *reg);
extern void gasnetc_unpin(gasnetc_hca_t *hca, gasnetc_memreg_t *reg);
#define gasnetc_unmap(reg)	gasneti_munmap((void *)((reg)->addr), (reg)->len)

/* Global configuration variables */
extern int		gasnetc_op_oust_limit;
extern int		gasnetc_op_oust_pp;
extern int		gasnetc_am_oust_limit;
extern int		gasnetc_am_oust_pp;
extern int		gasnetc_bbuf_limit;
extern int		gasnetc_use_rcv_thread;
extern int		gasnetc_am_credits_slack;
extern int		gasnetc_num_qps;
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
extern int		gasnetc_amrdma_max_peers;
extern size_t		gasnetc_amrdma_limit;
extern int		gasnetc_amrdma_depth;
extern int		gasnetc_amrdma_slot_mask;
extern gasneti_weakatomic_val_t gasnetc_amrdma_cycle;


/* Global variables */
extern int		gasnetc_num_hcas;
extern gasnetc_hca_t	gasnetc_hca[GASNETC_IB_MAX_HCAS];
extern gasnetc_cep_t	*gasnetc_cep;
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

#endif
