/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ibv-conduit/gasnet_core_sndrcv.c $
 * Description: GASNet ibv conduit implementation, transport send/receive logic
 * Copyright 2003, LBNL
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_am.h>
#include <gasnet_ibv.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>

/* ------------------------------------------------------------------------------------ *
 *  Configuration                                                                       *
 * ------------------------------------------------------------------------------------ */

#if GASNETC_PIN_SEGMENT
  /* Max firehose per op is one per local scatter/gather segment */
  #define GASNETC_MAX_FH	GASNETC_SND_SG
#else
  /* Max firehose per op is one per local scatter/gather segment + one remote */
  #define GASNETC_MAX_FH	(GASNETC_SND_SG + 1)
#endif

/* If running w/ threads (locks) we want to coalesce calls to
     gasnetc_lifo_push(&gasnetc_bbuf_freelist,*)
   and
     firehose_release().
   However, when no threads (no locks) are present, we don't
   want to pay the overhead for coalescing.
*/
#if GASNETC_ANY_PAR
  #define GASNETC_SND_REAP_COLLECT 1
#else
  #define GASNETC_SND_REAP_COLLECT 0
#endif

/* Define non-zero to allow loopback AMs to be assembled on the stack */
#define GASNETC_LOOPBACK_AMS_ON_STACK 1

/* ------------------------------------------------------------------------------------ *
 *  Global variables                                                                    *
 * ------------------------------------------------------------------------------------ */
size_t					gasnetc_fh_align;
size_t					gasnetc_fh_align_mask;
size_t                                  gasnetc_inline_limit;
size_t                   		gasnetc_bounce_limit;
size_t					gasnetc_packedlong_limit; // TODO-EX: adjust w/ nargs?
#if !GASNETC_PIN_SEGMENT
  size_t				gasnetc_putinmove_limit;
#endif
int					gasnetc_use_rcv_thread = GASNETC_USE_RCV_THREAD;
#if GASNETC_FH_OPTIONAL
  int					gasnetc_use_firehose = 1;
#endif
#if GASNETC_IBV_SRQ
  int					gasnetc_use_srq = 1;
#endif
#if GASNETC_IBV_XRC
  int					gasnetc_use_xrc = 1;
#endif
int					gasnetc_am_credits_slack;
int					gasnetc_am_credits_slack_orig;
int					gasnetc_alloc_qps;
int					gasnetc_num_qps;
int					gasnetc_amrdma_max_peers;
size_t					gasnetc_amrdma_limit;
int					gasnetc_amrdma_depth;
int					gasnetc_amrdma_slot_mask;
gasnetc_atomic_val_t			gasnetc_amrdma_cycle;
gasnetc_cep_t				**gasnetc_node2cep = NULL;
gex_Rank_t                           gasnetc_remote_nodes = 0;

/* ------------------------------------------------------------------------------------ *
 *  File-scoped types                                                                   *
 * ------------------------------------------------------------------------------------ */

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

typedef enum {
	GASNETC_OP_FREE,
	GASNETC_OP_AM,
	GASNETC_OP_GET_ZEROCP,
#if GASNETC_PIN_SEGMENT && GASNETC_FH_OPTIONAL
	GASNETC_OP_GET_BOUNCE,
#endif
	GASNETC_OP_PUT_INLINE,
	GASNETC_OP_PUT_ZEROCP,
	GASNETC_OP_PUT_BOUNCE,
#if !GASNETC_PIN_SEGMENT
	GASNETC_OP_PUT_INMOVE,
#endif
	GASNETC_OP_INVALID
} gasnetc_sreq_opcode_t;

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
  } u;
  #define fh_count	u.fh.count
  #define fh_ptr	u.fh.ptr
  #define bb_buff	u.bb.buff
  #define bb_addr	u.bb.addr
  #define bb_len	u.bb.len
  #define am_buff	u.am.buff
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
  } u;
  #define fh_count	u.fh.count
  #define fh_ptr	u.fh.ptr
  #define fh_len	u.fh.len
  #define fh_putinmove	u.fh.putinmove
  #define fh_loc_addr	u.fh.loc_addr
  #define fh_rem_addr	u.fh.rem_addr
  #define fh_bbuf	u.fh.bbuf
  #define fh_ready	u.fh.ready
  #define fh_oust	u.fh.oust
  #define fh_lc 	u.fh.lc
  #define fh_lc_cb 	u.fh.lc_cb
  #define am_buff	u.am.buff
#endif
} gasnetc_sreq_t;

/* Temporary buffer space used for constructing AMs on the stack */
typedef union {         
  gasnetc_shortmsg_t	shortmsg;
  gasnetc_medmsg_t	medmsg;
  gasnetc_longmsg_t	longmsg;
  uint8_t		raw[128];
} gasnetc_am_tmp_buf_t;

/* Per-thread data
 * Unlike gasneti_threaddata_t, this is associated w/ conduit-internal threads as well.
 */
typedef struct {
  /* Thread-local list of sreq's. */
  gasnetc_sreq_t	*sreqs;
  
  /* Nothing else yet, but lockfree algorithms for x84_64 and ia64 will also need
   * some thread-local data if they are ever implemented. */
} gasnetc_per_thread_t;

/* ------------------------------------------------------------------------------------ *
 *  File-scoped variables
 * ------------------------------------------------------------------------------------ */

static size_t                           gasnetc_am_inline_limit_sndrcv = 0;
static size_t                           gasnetc_am_inline_limit_rdma = 0;
#if !GASNETC_PIN_SEGMENT
  static size_t                         gasnetc_putinmove_limit_adjusted = 0;
#endif

static gasnetc_lifo_head_t		gasnetc_bbuf_freelist = GASNETC_LIFO_INITIALIZER;

static gasnetc_sema_t			*gasnetc_cq_semas = NULL;

/* Shared between gasnetc_sndrcv_{limits,init}() */
static int gasnetc_op_oust_per_qp;
static int gasnetc_am_repl_per_qp;
static int gasnetc_am_rqst_per_qp;
static int gasnetc_am_rbufs_per_qp;

/* ------------------------------------------------------------------------------------ *
 *  File-scoped completion callbacks
 * ------------------------------------------------------------------------------------ */

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

// gasnetc_counter_t completion callbacks
extern void gasnetc_cb_counter(gasnetc_atomic_val_t *cnt) {
  gasnetc_counter_t *counter = gasneti_container_of(cnt, gasnetc_counter_t, initiated);
  gasnetc_atomic_increment(&counter->completed, 0);
}
extern void gasnetc_cb_counter_rel(gasnetc_atomic_val_t *cnt) {
  gasnetc_counter_t *counter = gasneti_container_of(cnt, gasnetc_counter_t, initiated);
  gasnetc_atomic_increment(&counter->completed, GASNETI_ATOMIC_REL);
}


/* ------------------------------------------------------------------------------------ *
 *  File-scoped functions and macros                                                    *
 * ------------------------------------------------------------------------------------ */

#if GASNETI_MAX_THREADS > 1
  /* Note: first word of thread data is reserved for core */
  #define gasnetc_my_perthread() (gasnetc_per_thread_t *)(*(void**)(GASNETI_MYTHREAD))
  /* Since we use system-level AMs between gasnetc_sndrcv_init and gasnete_init(),
     the TLD will get initialized at first use in the AM Request path anyway. */
  #define gasnetc_per_thread_setup() ((void)0)
#else
  static gasnetc_per_thread_t gasnetc_per_thread;
  #define gasnetc_my_perthread() (&gasnetc_per_thread)
  #define gasnetc_per_thread_setup() gasnetc_per_thread_init(&gasnetc_per_thread)
#endif

static void gasnetc_free_aligned(void *ptr) {
  gasneti_free_aligned(ptr);
}

#define GASNETC_SREQS_GROWTHCNT 32 /* sreq list always grown by this size increment */
static int gasnetc_snd_reap(int);

static void gasnetc_free_sreqs(void *_ptr) {
  gasnetc_sreq_t *ptr = (gasnetc_sreq_t *)_ptr;
  int i;
  /* sreqs for AM sends may still be live on the adapter and thus unsafe to free
   */
  for (i = 0; i < GASNETC_SREQS_GROWTHCNT; i++) {
    while (ptr->opcode != GASNETC_OP_FREE) {
      gasnetc_snd_reap(1);
      if (ptr->opcode != GASNETC_OP_FREE) gasneti_sched_yield();
    }
    ptr = (gasnetc_sreq_t *)GASNETI_ALIGNUP(ptr+1, GASNETI_CACHE_LINE_BYTES);
  }
  gasneti_free_aligned(_ptr);
}

GASNETI_INLINE(gasnetc_alloc_sreqs)
void gasnetc_alloc_sreqs(gasnetc_sreq_t **head_p, gasnetc_sreq_t **tail_p GASNETI_THREAD_FARG)
{
  const int count = GASNETC_SREQS_GROWTHCNT;
  size_t bytes = GASNETI_ALIGNUP(sizeof(gasnetc_sreq_t), GASNETI_CACHE_LINE_BYTES);
  gasnetc_sreq_t *ptr = gasneti_malloc_aligned(GASNETI_CACHE_LINE_BYTES, count * bytes);
  int i;
  gasneti_leak_aligned(ptr);
  gasnete_register_threadcleanup(gasnetc_free_sreqs, ptr);
  *head_p = ptr;
  for (i = 1; i < count; ++i, ptr = ptr->next) {
    ptr->next = (gasnetc_sreq_t *)((uintptr_t)ptr + bytes);
    ptr->opcode = GASNETC_OP_FREE;
  }
  ptr->opcode = GASNETC_OP_FREE;
  *tail_p = ptr;
  GASNETC_STAT_EVENT_VAL(ALLOC_SREQ, count);
}

static
void gasnetc_per_thread_init(gasnetc_per_thread_t *td)
{
  GASNET_BEGIN_FUNCTION(); // OK - not a critical-path
  gasnetc_sreq_t *tail;
  gasnetc_alloc_sreqs(&td->sreqs, &tail GASNETI_THREAD_PASS);
  tail->next = td->sreqs;
}

extern void
gasnetc_new_threaddata_callback(void **core_threadinfo) {
    gasnetc_per_thread_t *result;
#if GASNETI_MAX_THREADS > 1
    result = gasneti_malloc_aligned(GASNETI_CACHE_LINE_BYTES,
                                    GASNETI_ALIGNUP(sizeof(gasnetc_per_thread_t),
                                                    GASNETI_CACHE_LINE_BYTES));
    gasneti_leak_aligned(result);
    gasnete_register_threadcleanup(gasnetc_free_aligned, result);
#else
    result = &gasnetc_per_thread;
#endif
    gasnetc_per_thread_init(result);
    *core_threadinfo = (void*) result;
}

/* ------------------------------------------------------------------------------------ */

extern int
gasnetc_create_cq(struct ibv_context * hca_hndl, int req_size,
		  struct ibv_cq * *cq_p, int *act_size,
		  gasnetc_progress_thread_t *pthr_p)
{
  struct ibv_comp_channel * compl = NULL;
  struct ibv_cq * result;
 #if GASNETI_CONDUIT_THREADS
  if (pthr_p) {
    compl = ibv_create_comp_channel(hca_hndl);
    GASNETC_IBV_CHECK_PTR(compl, "from ibv_create_comp_channel");
  }
 #endif
  result = ibv_create_cq(hca_hndl, req_size, NULL, compl, 0);
  GASNETC_IBV_CHECK_PTR(result, "from ibv_create_cq()");
 #if GASNETI_CONDUIT_THREADS
  if (pthr_p) {
    int rc = ibv_req_notify_cq(result, 0);
    GASNETC_IBV_CHECK(rc, "while requesting cq events");
    memset(pthr_p, 0, sizeof(*pthr_p));
    pthr_p->compl = compl;
    pthr_p->cq = result;
  }
 #endif
  if_pt (result != NULL) {
    *cq_p = result;
    *act_size = result->cqe;
    return 0;
  } else {
    return 1;
  }
}

/* The 'epid' type holds 'node' in the low 16 bits.
 * The upper 16 bits holds a qp index (qpi).
 * A qpi of zero is a wildcard (an 'unbound' epid).
 * Therefore, setting epid=node means "use any qp for that node".
 * Non-zero qpi is 1 + the array index of the desired queue pair.
 */
#define gasnetc_epid2node(E)	((E)&0xffff)
#define gasnetc_epid2qpi(E)	((E)>>16)
#define gasnetc_epid(N,Q)	((N)|(((Q)+1)<<16))

#define GASNETC_SEG_RKEY(_cep, _index)	((_cep)->rkeys[_index])
#if GASNETC_IB_MAX_HCAS > 1
  #define GASNETC_HCA_IDX(_cep)		((_cep)->hca_index)
  #define GASNETC_SND_LKEY(_cep)	((_cep)->snd_lkey)
  #define GASNETC_RCV_LKEY(_cep)	((_cep)->rcv_lkey)
  #define GASNETC_SEG_LKEY(_cep, _index) ((_cep)->seg_lkeys[_index])
#else
  #define GASNETC_HCA_IDX(_cep)		0
  #define GASNETC_SND_LKEY(_cep)	(gasnetc_hca[0].snd_reg.handle->lkey)
  #define GASNETC_RCV_LKEY(_cep)	(gasnetc_hca[0].rcv_reg.handle->lkey)
  #define GASNETC_SEG_LKEY(_cep, _index) (gasnetc_hca[0].seg_lkeys[_index])
#endif
#define GASNETC_FH_RKEY(_cep, _fhptr)	((_fhptr)->client.rkey[GASNETC_HCA_IDX(_cep)])
#define GASNETC_FH_LKEY(_cep, _fhptr)	((_fhptr)->client.lkey[GASNETC_HCA_IDX(_cep)])

// TODO-EX: following functions are a hack which wrap GASNETC_SEG_[LR]KEY()
// to map negative index values to the auxseg keys instead, and will be
// replaced with more general multi-registration support later.
#if GASNETC_PIN_SEGMENT
  GASNETI_INLINE(gasnetc_seg_rkey)
  uint32_t gasnetc_seg_rkey(gasnetc_cep_t *cep, int index)
  {
    return (index >= 0) ? GASNETC_SEG_RKEY(cep, index)
                        : cep->hca->aux_rkeys[gasnetc_epid2node(cep->epid)];
  }
  #undef GASNETC_SEG_RKEY
  #define GASNETC_SEG_RKEY gasnetc_seg_rkey
  GASNETI_INLINE(gasnetc_seg_lkey)
  uint32_t gasnetc_seg_lkey(gasnetc_cep_t *cep, int index)
  {
    return (index >= 0) ? GASNETC_SEG_LKEY(cep, index)
                        : cep->hca->aux_reg.handle->lkey;
  }
  #undef GASNETC_SEG_LKEY
  #define GASNETC_SEG_LKEY gasnetc_seg_lkey
#endif // GASNETC_PIN_SEGMENT


/* This limits the amount we ask for in a firehose_{local,remote}_pin() call,
 * to enourage a steady-state layout of firehoses that has start and end addresses
 * at multpiles of gasnetc_fh_align and length 2*gasnetc_fh_align.
 * This will look sort of like two courses of bricks.
 */
GASNETI_INLINE(gasnetc_fh_aligned_len)
size_t gasnetc_fh_aligned_len(uintptr_t start, size_t len) {
  size_t result = 2 * gasnetc_fh_align - (start & gasnetc_fh_align_mask);
  return MIN(len, result);
}

GASNETI_INLINE(gasnetc_fh_aligned_local_pin)
const firehose_request_t *gasnetc_fh_aligned_local_pin(uintptr_t start, size_t len) {
  return firehose_local_pin(start, gasnetc_fh_aligned_len(start, len), NULL);
}

GASNETI_INLINE(gasnetc_sr_desc_init)
void *gasnetc_sr_desc_init(struct ibv_send_wr *result, struct ibv_sge *sg_lst_p)
{
    #if GASNET_DEBUG
      result->num_sge = 0; /* invalid to ensure caller sets it */
    #endif
    result->sg_list = sg_lst_p;
  
  return result;
}
#define GASNETC_DECL_SR_DESC(_name, _sg_lst_len)                        \
	struct ibv_send_wr _name[1];                                      \
	struct ibv_sge _CONCAT(_name,_sg_lst)[_sg_lst_len];              \
	void *_CONCAT(_name,_dummy) = gasnetc_sr_desc_init(_name, _CONCAT(_name,_sg_lst)) /* note intentional lack of final semicolon */

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

#define gasnetc_poll_rcv()		gasnetc_do_poll(1,0 GASNETI_THREAD_PASS)
#define gasnetc_poll_snd()		gasnetc_do_poll(0,1 GASNETI_THREAD_PASS)
#define gasnetc_poll_both()		gasnetc_do_poll(1,1 GASNETI_THREAD_PASS)

/* Post a work request to the receive queue of the given endpoint */
GASNETI_INLINE(gasnetc_rcv_post)
void gasnetc_rcv_post(gasnetc_cep_t *cep, gasnetc_rbuf_t *rbuf) {
  int vstat;

  gasneti_assert(cep);
  gasneti_assert(rbuf);

  /* check for attempted loopback traffic */
  gasneti_assert(!GASNETI_NBRHD_JOBRANK_IS_LOCAL(gasnetc_epid2node(cep->epid)));
  
  rbuf->cep = cep;
  rbuf->rr_sg.lkey = GASNETC_RCV_LKEY(cep);
  GASNETI_TRACE_PRINTF(D,("POST_RR rbuf=%p hca=%d lkey=0x%08x", 
			  (void *)rbuf, GASNETC_HCA_IDX(cep),
			  (unsigned int)(rbuf->rr_sg.lkey)));
  if (!gasnetc_use_srq) {
    GASNETI_TRACE_PRINTF(D,("POST_RR rbuf=%p peer=%d qp=%d",
			    (void *)rbuf, gasnetc_epid2node(cep->epid),
			    gasnetc_epid2qpi(cep->epid) - 1));
  }

  {
    struct ibv_recv_wr *bad_wr;
  #if GASNETC_IBV_SRQ
    if (cep->srq) { /* Equivalent to gasnetc_use_srq, but we need this value anyway */
      vstat = ibv_post_srq_recv(cep->srq, &rbuf->rr_desc, &bad_wr);
    } else
  #endif
    {
      vstat = ibv_post_recv(cep->qp_handle, &rbuf->rr_desc, &bad_wr);
    }
  }

  if_pt (vstat == 0) {
    /* normal return */
    return;
  } else if (GASNETC_IS_EXITING()) {
    /* disconnected by another thread */
    gasnetc_exit(0);
  } else {
    /* unexpected error */
    GASNETC_IBV_CHECK(vstat, "while posting a receive work request");
  }
}

/* gasnetc_do_select
 * See http://en.wikipedia.org/wiki/Selection_algorithm
 * For an overview of algorithms for the Selection problem.
 * Given that gasnetc_amrdma_max_peers is "small", we are ignoring all the good
 * "order statistic" stuff and performing a simple in-place Shell sort under the
 * assumption that the number of peers that passed the floor filter is small
 * enough not to warrant anything more complex.
 * For more on Shell sort see (http://en.wikipedia.org/wiki/Shell_sort) 
 * XXX: for large enough 'size' we should switch algorithms
 */
static void gasnetc_shell_sort_inner(gasnetc_amrdma_balance_tbl_t *A, int size, int step) {
  int i;

  #define SORT_CMP(X,Y) ((X).count <= (Y).count)

  for (i = step; i < size; ++i) {
    gasnetc_amrdma_balance_tbl_t tmp = A[i];
    int j;
    for (j = i; ((j >= step) && SORT_CMP(A[j-step], tmp)); j -= step) {
      A[j] = A[j - step];
    }
    A[j] = tmp;
  }
}
static void gasnetc_do_select(gasnetc_hca_t *hca, int size) {
  struct gasnetc_amrdma_balance_tbl_t_ *A = hca->amrdma_balance.table;
  if (size >= 93) gasnetc_shell_sort_inner(A, size, 31);
  if (size >= 51) gasnetc_shell_sort_inner(A, size, 17);
  if (size >= 27) gasnetc_shell_sort_inner(A, size, 9);
  if (size >= 15) gasnetc_shell_sort_inner(A, size, 5);
  if (size >= 9) gasnetc_shell_sort_inner(A, size, 3);
  gasnetc_shell_sort_inner(A, size, 1);
}

static void gasnetc_amrdma_grant(gasnetc_hca_t *hca, gasnetc_cep_t *cep) {
  gex_Rank_t node = gasnetc_epid2node(cep->epid);
  int qpi = gasnetc_epid2qpi(cep->epid);

  if (gasnetc_use_srq) { /* Cross-over keeping in mind that qpi is 1-based */
    if (qpi > gasnetc_num_qps) {
      qpi -= gasnetc_num_qps;
    } else {
      qpi += gasnetc_num_qps;
    }
  }

  GASNETI_TRACE_PRINTF(C,("AMRDMA_GRANT_SND to node=%d qp=%d\n", (int)node, (qpi - 1)));

  gasneti_assert(cep->amrdma_recv == NULL);
  cep->amrdma_recv = gasnetc_amrdma_recv_alloc(hca);
  if (cep->amrdma_recv != NULL) {
    int count = gasnetc_atomic_read(&hca->amrdma_rcv.count, 0);
    gasneti_assert(count < hca->num_qps);
    gasneti_assert(count < hca->amrdma_rcv.max_peers);
    hca->amrdma_rcv.cep[count] = cep;
    gasnetc_atomic_set(&hca->amrdma_rcv.count, count+1, GASNETI_ATOMIC_REL);

    gex_AM_RequestShort(gasneti_THUNK_TM, node, gasneti_handleridx(gasnetc_amrdma_grant_reqh), 0,
		            (gex_AM_Arg_t)qpi,
		            (gex_AM_Arg_t)hca->amrdma_reg.handle->rkey,
		            PACK(cep->amrdma_recv->addr));
  }
}

void gasnetc_amrdma_balance_one(gasnetc_hca_t *hca) {
    /* GASNETC_AMRDMA_REDUCE(X) is amount by which ALL counts X are reduced each round */
    #define GASNETC_AMRDMA_REDUCE(X)		((X)>>1)
    /* GASNETC_AMRDMA_BOOST(FLOOR) is amount by which SELECTED counts X are boosted */
    #define GASNETC_AMRDMA_BOOST(FLOOR)	((FLOOR)>>1)

    gasnetc_amrdma_balance_tbl_t *tbl = hca->amrdma_balance.table;
    int tbl_size = 0;
    int i;

    /* Pass 1: Collect all peers w/ counts >= floor, while also "decaying" the counters.
     * This is the only part that should be O(gasneti_nodes) on average.
     */
    for (i = 0; i < hca->num_qps; ++i) {
      gasnetc_atomic_val_t x, y;
      gasnetc_cep_t *cep = hca->cep[i];

      x = gasnetc_atomic_read(&cep->amrdma_eligable, 0);
      y = GASNETC_AMRDMA_REDUCE(x);
      gasnetc_atomic_subtract(&cep->amrdma_eligable, y, 0);

      if (x >= hca->amrdma_balance.floor) {
        tbl[tbl_size].count = x - y;
        tbl[tbl_size].cep = cep;
        tbl_size++;
      }
    }

    /* Pass 2: "Select" the top hca->amrdma_rcv.max_peers peers and
     * find the new floor (the min count from among the selected peers).
     */
    if (tbl_size > hca->amrdma_rcv.max_peers) {
      gasnetc_do_select(hca, tbl_size);
      tbl_size = hca->amrdma_rcv.max_peers;
      /* XXX: we know the current selection mechanism will leave the table sorted. */
      hca->amrdma_balance.floor = tbl[tbl_size-1].count + GASNETC_AMRDMA_BOOST(tbl[tbl_size-1].count);
    } else if (tbl_size == hca->amrdma_rcv.max_peers) {
      /* "select" the entire table, and find MIN for new floor */
      gasnetc_atomic_val_t new_floor = tbl[0].count;
      for (i = 1; i < tbl_size; ++i) {
        new_floor = MIN(new_floor, tbl[i].count);
      }
      hca->amrdma_balance.floor = new_floor + GASNETC_AMRDMA_BOOST(new_floor);
    } else {
      /* "select" the entire table, but leave the floor unchanged */
    }

    /* Pass 3:
     * + Grant any newly selected peers
     * + "Boost" the selected peers to encourage re-selection on the next pass
     */
    {
      gasnetc_atomic_val_t boost = GASNETC_AMRDMA_BOOST(hca->amrdma_balance.floor);
      for (i = 0; i < tbl_size; ++i) {
        gasnetc_cep_t *cep = tbl[i].cep;
        if (!cep->amrdma_recv) {
          gasnetc_amrdma_grant(hca, cep);
        }
        gasnetc_atomic_add(&cep->amrdma_eligable, boost, 0);
      }
    }

    if (gasnetc_atomic_read(&hca->amrdma_rcv.count, 0) == hca->amrdma_rcv.max_peers) {
      /* Disable additional balancing if the limit has been reached (since we lack REVOKE) */
      hca->amrdma_balance.mask = ~(gasnetc_atomic_val_t)0;
    }
}

enum {
  GASNETC_AMRDMA_STATE_RUN = 0, /* normal state */
  GASNETC_AMRDMA_STATE_PENDING, /* balance requested */
  GASNETC_AMRDMA_STATE_LOCKED  /* balance in progress */
};

void gasnetc_amrdma_balance(void) {
  gasnetc_hca_t *hca;
  GASNETC_FOR_ALL_HCA(hca) {
    if (gasnetc_atomic_compare_and_swap(&hca->amrdma_balance.state,
                                        GASNETC_AMRDMA_STATE_PENDING,
                                        GASNETC_AMRDMA_STATE_LOCKED,
                                        GASNETI_ATOMIC_ACQ_IF_TRUE)) {
      GASNETI_PROGRESSFNS_DISABLE(gasnetc_pf_amrdma, COUNTED);
      gasnetc_amrdma_balance_one(hca);
      gasnetc_atomic_set(&hca->amrdma_balance.state, GASNETC_AMRDMA_STATE_RUN, GASNETI_ATOMIC_REL);
    }
  }
}

void gasnetc_amrdma_eligable(gasnetc_cep_t *cep) {
  gasnetc_hca_t * const hca = cep->hca;
  gasnetc_atomic_val_t interval = gasnetc_atomic_add(&hca->amrdma_balance.count, 1, 0);

  gasnetc_atomic_increment(&cep->amrdma_eligable, 0);

  if_pf (0 == (interval & hca->amrdma_balance.mask)) {
    GASNETI_PROGRESSFNS_ENABLE(gasnetc_pf_amrdma, COUNTED);
    if (! gasnetc_atomic_compare_and_swap(&hca->amrdma_balance.state,
                                          GASNETC_AMRDMA_STATE_RUN,
                                          GASNETC_AMRDMA_STATE_PENDING,
                                          GASNETI_ATOMIC_REL)) {
      /* Lost race on change of amrdma_balance.state */
      GASNETI_PROGRESSFNS_DISABLE(gasnetc_pf_amrdma, COUNTED);
    }
  }
}

/* GASNETI_INLINE(gasnetc_processPacket) */
void gasnetc_processPacket(gasnetc_cep_t *cep, gasnetc_rbuf_t *rbuf, uint32_t flags GASNETI_THREAD_FARG) {
  gasnetc_EP_t ep = rbuf->rr_ep;
  gasneti_assert(ep == gasnetc_ep0);
  gasnetc_buffer_t * const buf = (gasnetc_buffer_t *)(uintptr_t)(rbuf->rr_sg.addr);
  const gex_AM_Index_t handler_id = GASNETC_MSG_HANDLERID(flags);
  const gex_AM_Entry_t * const handler_entry = &ep->_amtbl[handler_id];
  const gex_AM_Fn_t handler_fn = handler_entry->gex_fnptr;
  const gasneti_category_t category = GASNETC_MSG_CATEGORY(flags);
  const int isreq = GASNETC_MSG_ISREQUEST(flags);
  int full_numargs = GASNETC_MSG_NUMARGS(flags);
  int user_numargs = full_numargs;
  const gex_Token_t token = (gex_Token_t)rbuf;
  gex_AM_Arg_t *args;

  gasneti_assert(!GASNETI_NBRHD_JOBRANK_IS_LOCAL(GASNETC_MSG_SRCIDX(flags)));
  gasneti_assert(cep != NULL);

#if GASNETI_THREADINFO_OPT
  rbuf->rbuf_threadinfo = GASNETI_MYTHREAD;
#endif
  rbuf->rbuf_needReply = isreq;
#if GASNET_DEBUG
  rbuf->rbuf_handlerRunning = 1;
#endif
  rbuf->rbuf_flags = flags;

  /* Locate arguments */
  switch (category) {
    case gasneti_Short:
      args = buf->shortmsg.args;
      break;

    case gasneti_Medium:
      args = buf->medmsg.args;
      break;

    case gasneti_Long:
      args = buf->longmsg.args;
      break;

    default: gasneti_unreachable_error(("Invalid category in gasnetc_processPacket: 0x%x",(int)category));
  }

  { /* Process any flow control info */
    int credits = 0;

    if (full_numargs == GASNETC_MAX_ARGS) {
      int acks = (args[0] >> 8) & 0xff;
      credits = args[0] & 0xff;
      full_numargs = (args[0] >> 16) & 0x1f;
      user_numargs = full_numargs - 1;

      gasneti_assert(!gasnetc_use_srq || !credits);

      if (acks) {
        gasneti_assert(acks <= gasnetc_amrdma_depth);
        gasneti_assert(acks > 0);
        gasnetc_atomic_add(&cep->amrdma_send->head, acks, 0);
      }
      if (credits) {
        gasnetc_sema_up_n(&cep->am_loc, credits);
      }

      GASNETI_TRACE_PRINTF(C,("RCV_AM_CREDITS credits=%d acks=%d\n", credits, acks));

      args += 1;
    }

    /* Available remotely posted (request) buffers */
    credits += (isreq ^ 1); /* Credit for self if this is a reply */
    if (credits) {
      gasnetc_sema_up_n(&cep->am_rem, credits);
    }
  }
  
  /* Ack? */
  if (!handler_id) return;

  gasneti_amtbl_check(handler_entry, user_numargs, category, isreq);

  /* Run the handler */
  switch (category) {
    case gasneti_Short:
      { 
        GASNETI_RUN_HANDLER_SHORT(isreq,handler_id,handler_fn,token,args,user_numargs);
      }
      break;

    case gasneti_Medium:
      {
        void * data = GASNETC_MSG_MED_DATA(buf, full_numargs);
        size_t nbytes = buf->medmsg.nBytes;
        GASNETI_RUN_HANDLER_MEDIUM(isreq,handler_id,handler_fn,token,args,user_numargs,data,nbytes);
      }
      break;

    case gasneti_Long:
      { 
        void * data = (void *)(buf->longmsg.destLoc);
	size_t nbytes = buf->longmsg.nBytes & 0x7fffffff;
	if (buf->longmsg.nBytes & 0x80000000) {
	  /* Must relocate the payload which is packed like a Medium. */
	  gasneti_assert(nbytes <= GASNETC_MAX_PACKEDLONG);
	  gasneti_assert((nbytes <= gasnetc_packedlong_limit) ||
			 (!GASNETC_PIN_SEGMENT && !isreq));
	  memcpy(data, GASNETC_MSG_LONG_DATA(buf, full_numargs), (size_t)nbytes);
	}
        GASNETI_RUN_HANDLER_LONG(isreq,handler_id,handler_fn,token,args,user_numargs,data,(size_t)nbytes);
      }
      break;
    default: gasneti_unreachable_error(("Invalid category in gasnetc_processPacket: 0x%x",(int)category));
  }
  
#if GASNET_DEBUG
  rbuf->rbuf_handlerRunning = 0;
#endif
}

#if GASNETC_SND_REAP_COLLECT
  #define _GASNETC_COLLECT_BBUF(_test,_bbuf) do { \
      void *_tmp = (void*)(_bbuf);                \
      _test((_tmp != NULL)) {                     \
        gasnetc_lifo_link(bbuf_tail, _tmp);   \
        bbuf_tail = _tmp;                         \
      }                                           \
    } while(0)
  #define GASNETC_FREE_BBUFS() do {    \
      if (bbuf_tail != &bbuf_dummy) {  \
        gasnetc_lifo_push_many(&gasnetc_bbuf_freelist, gasnetc_lifo_next(&bbuf_dummy), bbuf_tail); \
      }                                \
    } while(0)
  #define GASNETC_COLLECT_FHS() do {                    \
      gasneti_assert(sreq->fh_count >= 0);              \
      gasneti_assert(sreq->fh_count <= GASNETC_MAX_FH); \
      for (i=0; i<sreq->fh_count; ++i, ++fh_num) {      \
	fh_ptrs[fh_num] = sreq->fh_ptr[i];              \
      }                                                 \
    } while(0)
  #define GASNETC_FREE_FHS() do {        \
    if (fh_num) {                        \
      gasneti_assert(fh_num <= GASNETC_SND_REAP_LIMIT * GASNETC_MAX_FH); \
      firehose_release(fh_ptrs, fh_num); \
    }                                    \
  } while(0)
#else
  #define _GASNETC_COLLECT_BBUF(_test,_bbuf) do {          \
      void *_tmp = (void*)(_bbuf);                         \
      _test((_tmp != NULL)) {                              \
        gasnetc_lifo_push(&gasnetc_bbuf_freelist,_tmp); \
      }                                                    \
    } while(0)
  #define GASNETC_FREE_BBUFS()	do {} while (0)
  #define GASNETC_COLLECT_FHS() do {                      \
      gasneti_assert(sreq->fh_count >= 0);                \
      if (sreq->fh_count > 0) {                           \
        gasneti_assert(sreq->fh_count <= GASNETC_MAX_FH); \
        firehose_release(sreq->fh_ptr, sreq->fh_count);   \
      }                                                   \
    } while(0)
  #define GASNETC_FREE_FHS()	do {} while (0)
#endif

#define GASNETC_ALWAYS(X) gasneti_assert(X); if(1)
#define GASNETC_COLLECT_BBUF(_bbuf) _GASNETC_COLLECT_BBUF(GASNETC_ALWAYS,(_bbuf))
#define GASNETC_COLLECT_BBUF_IF(_bbuf) _GASNETC_COLLECT_BBUF(if,(_bbuf))
  

GASNETI_NEVER_INLINE(gasnetc_dump_cqs,
void gasnetc_dump_cqs(struct ibv_wc *comp, gasnetc_hca_t *hca, const int is_snd)) {
  static gex_HSL_t lock = GEX_HSL_INITIALIZER;
  enum ibv_wc_status status = IBV_WC_SUCCESS;
  int count = 0;
  const char *label;
  int max_other_cq;

  gex_HSL_Lock(&lock);

  if (is_snd) {
  #if GASNETC_DYNAMIC_CONNECT && !GASNETC_USE_CONN_THREAD
    const int is_ud = (comp->wr_id & 1);
  #else
    const int is_ud = 0;
  #endif
    gasnetc_sreq_t *sreq = (gasnetc_sreq_t *)(uintptr_t)comp->wr_id;
    int node = is_ud ? -1 : gasnetc_epid2node(sreq->cep->epid);
    int qpi = is_ud ? 1 : gasnetc_epid2qpi(sreq->cep->epid);
    if (comp->status == IBV_WC_RETRY_EXC_ERR) {
      fprintf(stderr, "@ %d> snd status=TIMEOUT opcode=%d dst_node=%d dst_qp=%d\n", gasneti_mynode, comp->opcode, (int)node, qpi-1);
    } else {
      fprintf(stderr, "@ %d> snd status=%d opcode=%d dst_node=%d dst_qp=%d\n", gasneti_mynode, comp->status, comp->opcode, (int)node, qpi-1);
    }
    label = "rcv";
    max_other_cq = hca->qps * 2 * gasnetc_am_oust_pp * (gasneti_nodes - 1);
  } else {
    fprintf(stderr, "@ %d> rcv comp->status=%d\n", gasneti_mynode, comp->status);
    label = "snd";
    max_other_cq = hca->qps * gasnetc_op_oust_pp;
  }

  do { /* Drain the other CQ */
    int rc = ibv_poll_cq((is_snd ? hca->rcv_cq : hca->snd_cq), 1, comp);
    if (rc != 0) {
      /* use an invalid value to ensure output is generated on the last pass */
      comp->status = (enum ibv_wc_status)(-1);
    }
    if (comp->status == status) {
      ++count;
    } else {
      if (count) {
        switch (status) {
	  case IBV_WC_SUCCESS:
            fprintf(stderr, "@ %d> - %s %d op(s) OK\n", gasneti_mynode, label, count);
	    break;
	  case IBV_WC_WR_FLUSH_ERR:
            fprintf(stderr, "@ %d> - %s %d op(s) FLUSHED by error\n", gasneti_mynode, label, count);
	    break;
	  default:
            fprintf(stderr, "@ %d> - %s %d op(s) with status=%d\n", gasneti_mynode, label, count, status);
	    break;
        }
      }
      count = 1;
      status = comp->status;
    }
  } while ((status != (enum ibv_wc_status)(-1)) && (count <= max_other_cq));
  if (count > max_other_cq) {
    fprintf(stderr, "@ %d> - %s CQ contains impossibly large WCE count with status %d\n", gasneti_mynode, label, status);
  }
  gex_HSL_Unlock(&lock);
}

/* Try to pull completed entries (if any) from the send CQ(s). */
static int gasnetc_snd_reap(int limit) {
  int count;
  struct ibv_wc comp;
  #if GASNETC_SND_REAP_COLLECT
    int i, fh_num = 0;
    void *bbuf_dummy;
    void *bbuf_tail = &bbuf_dummy;
    const firehose_request_t *fh_ptrs[GASNETC_SND_REAP_LIMIT * GASNETC_MAX_FH];
  #endif

  #if GASNETC_IB_MAX_HCAS > 1
    /* Simple round-robin (w/ a harmless multi-thread race) */
    /* Note use of casts to volatile are require to work around bug 1586 */
    static int index = 0;
    int tmp = *(volatile int *)(&index);
    *(volatile int *)(&index) = ((tmp == 0) ? gasnetc_num_hcas : tmp) - 1;
    gasnetc_hca_t *hca = &gasnetc_hca[tmp];
  #else
    gasnetc_hca_t *hca = &gasnetc_hca[0];
  #endif

  gasneti_assert(limit <= GASNETC_SND_REAP_LIMIT);

  for (count = 0; count < limit; ++count) {
    int rc = ibv_poll_cq(hca->snd_cq, 1, &comp);
    if_pt (rc == 0) {
      /* CQ empty - we are done */
      break;
    } else if_pt (rc == 1) {
      if_pt (comp.status == IBV_WC_SUCCESS) {
        gasnetc_sreq_t *sreq = (gasnetc_sreq_t *)(uintptr_t)comp.wr_id;
      #if GASNETC_DYNAMIC_CONNECT && !GASNETC_USE_CONN_THREAD
        if_pf (comp.wr_id & 1) {
          gasnetc_conn_snd_wc(&comp);
        } else
      #endif
        if_pt (sreq) {
	  gasneti_assert(sreq->opcode != GASNETC_OP_INVALID);
	  gasnetc_sema_up(GASNETC_CEP_SQ_SEMA(sreq->cep));
	  gasnetc_sema_up(sreq->cep->snd_cq_sema_p);

	  switch (sreq->opcode) {
          #if GASNETC_PIN_SEGMENT && GASNETC_FH_OPTIONAL
	  case GASNETC_OP_GET_BOUNCE:	/* Bounce-buffer GET */
	    gasneti_assert(comp.opcode == IBV_WC_RDMA_READ);
	    gasneti_assert(sreq->comp.cb != NULL);
	    gasneti_assert(!GASNETC_USE_FIREHOSE); /* Only possible when firehose disabled */
	    gasneti_assert(sreq->bb_buff != NULL);
	    gasneti_assert(sreq->bb_addr != NULL);
	    gasneti_assert(sreq->bb_len > 0);
	    memcpy(sreq->bb_addr, sreq->bb_buff, sreq->bb_len);
	    sreq->comp.cb(sreq->comp.data);
	    GASNETC_COLLECT_BBUF(sreq->bb_buff);
	    break;
          #endif

	  case GASNETC_OP_GET_ZEROCP:	/* Zero-copy GET */
	    gasneti_assert(comp.opcode == IBV_WC_RDMA_READ);
	    gasneti_assert(sreq->comp.cb != NULL);
            sreq->comp.cb(sreq->comp.data);
	    GASNETC_COLLECT_FHS();
	    break;

	  case GASNETC_OP_PUT_BOUNCE:	/* Bounce-buffer PUT */
	    gasneti_assert(comp.opcode == IBV_WC_RDMA_WRITE);
            if (sreq->comp.cb != NULL) {
              sreq->comp.cb(sreq->comp.data);
            }
            #if GASNETC_PIN_SEGMENT
	    gasneti_assert(sreq->bb_buff);
	    GASNETC_COLLECT_BBUF(sreq->bb_buff);
	    #else
	    gasneti_assert(sreq->fh_bbuf);
	    GASNETC_COLLECT_BBUF(sreq->fh_bbuf);
	    GASNETC_COLLECT_FHS();
	    #endif
	    break;

	  case GASNETC_OP_PUT_INLINE:	/* Inline PUT */
	    gasneti_assert(comp.opcode == IBV_WC_RDMA_WRITE);
            if (sreq->comp.cb != NULL) {
              sreq->comp.cb(sreq->comp.data);
            }
            #if GASNETC_PIN_SEGMENT
	    gasneti_assert(sreq->fh_count == 0);
	    #else
	    GASNETC_COLLECT_FHS();
	    #endif
	    break;

	  case GASNETC_OP_PUT_ZEROCP:	/* Zero-copy PUT */
	    gasneti_assert(comp.opcode == IBV_WC_RDMA_WRITE);
	    if (sreq->comp.cb != NULL) {
              sreq->comp.cb(sreq->comp.data);
	    }
	    GASNETC_COLLECT_FHS();
	    break;

	  case GASNETC_OP_AM:		/* AM send */
	    gasneti_assert((comp.opcode == IBV_WC_SEND) ||
			   (comp.opcode == IBV_WC_RDMA_WRITE));
	    if (sreq->comp.cb != NULL) {
              sreq->comp.cb(sreq->comp.data);
	    }
	    GASNETC_COLLECT_BBUF_IF(sreq->am_buff);
	    break;

	  default:
	    gasneti_fatalerror("Reaped send with invalid/unknown opcode %d", (int)sreq->opcode);
	  }

	  /* Mark sreq free */
	  sreq->opcode = GASNETC_OP_FREE;
        } else {
          gasneti_fatalerror("snd_reap reaped NULL sreq");
          break;
        }
      } else if (GASNETC_IS_EXITING()) {
        /* disconnected */
	break;	/* can't exit since we can be called in exit path */
      } else if (!gasneti_attach_done) {
        gasneti_fatalerror("failed to connect (snd) status=%d", comp.status);
        break;
      } else {
#if 1 
	gasnetc_dump_cqs(&comp, hca, 1);
#endif
        gasneti_fatalerror("aborting on reap of failed send");
        break;
      }
    } else if (GASNETC_IS_EXITING()) {
      /* disconnected by another thread */
      gasnetc_exit(0);
    } else {
      GASNETC_IBV_CHECK(rc, "while reaping the send queue");
    }
  }

  if (count) {
    GASNETC_STAT_EVENT_VAL(SND_REAP,count);
    gasneti_sync_writes();	/* push out our OP_FREE writes */
  }

  /* Release any firehoses and bounce buffers we've collected */
  GASNETC_FREE_FHS();
  GASNETC_FREE_BBUFS();

  return count;
}

GASNETI_INLINE(gasnetc_get_cep)
gasnetc_cep_t *gasnetc_get_cep(gex_Rank_t node) {
  gasnetc_cep_t *result = GASNETC_NODE2CEP(node);
#if GASNETC_DYNAMIC_CONNECT
  if_pf (!result) {
    result = gasnetc_connect_to(node);
  }
#endif
  return result;
}

/* Take *unbound* epid, return a qp number */
GASNETI_INLINE(gasnetc_epid_select_qpi)
gasnetc_epid_t gasnetc_epid_select_qpi(gasnetc_cep_t *ceps, gasnetc_epid_t epid,
				       enum ibv_wr_opcode op, size_t len) {
  gasnetc_epid_t qpi = gasnetc_epid2qpi(epid);

  if_pt (qpi == 0) {
 #if 0
    /* Select by largest space avail */
    uint32_t space, best_space;
    int i;
    gasneti_assert(op != IBV_WR_SEND_WITH_IMM); /* AMs never wildcard */
    qpi = 0;
    best_space = gasnetc_sema_read(GASNETC_CEP_SQ_SEMA(ceps+0));
    for (i = 1; i < gasnetc_num_qps; ++i) {
      space = gasnetc_sema_read(GASNETC_CEP_SQ_SEMA(ceps+1));
      if (space > best_space) {
        best_space = space;
        qpi = i;
      }
    }
 #else
    /* Simple round-robin (w/ a harmless multi-thread race) */
    /* Note use of casts to volatile are require to work around bug 1586 */
    static int prev = 0;
    qpi = *(volatile int *)(&prev);
    qpi = ((qpi == 0) ? gasnetc_num_qps : qpi) - 1;
    *(volatile int *)(&prev) = qpi;
 #endif
    gasneti_assert(qpi < gasnetc_num_qps);
  } else {
    --qpi; /* offset */
    gasneti_assert(qpi < gasnetc_alloc_qps);
  }

  return qpi;
}

/* Take (sreq,op,len) and bind the sreq to a specific (not wildcard) qp */
GASNETI_INLINE(gasnetc_bind_cep_inner)
gasnetc_cep_t *gasnetc_bind_cep_inner(gasnetc_epid_t epid, gasnetc_sreq_t *sreq,
				      enum ibv_wr_opcode op, size_t len, int is_reply) {
  gasnetc_cep_t *ceps = gasnetc_get_cep(gasnetc_epid2node(epid));
  gasnetc_cep_t *cep;
  int qpi;

  /* Loop until space is available on the selected SQ for 1 new entry.
   * If we hold the last one then threads sending to the same node will stall. */
  qpi = gasnetc_epid_select_qpi(ceps, epid, op, len);
  cep = &ceps[qpi];
  if_pf (!gasnetc_sema_trydown(GASNETC_CEP_SQ_SEMA(cep))) {
    GASNETC_TRACE_WAIT_BEGIN();

  #if GASNETC_DYNAMIC_CONNECT
    /* Close the one dynamic connection race condition. */
    if ((GASNETC_CEP_SQ_SEMA(cep) == &gasnetc_zero_sema) && is_reply) {
      /* We are in the "gap" between RTR and RTS and waiting for the ACK.
       * However, since we are trying to send an AM Reply we KNOW that
       * the ACK was sent since we only Reply in response to a Request.
       * The Passive node reaches RTS at the time it sends its ACK and
       * thus cannot send us a Request until ready to send the ACK.
       */
      gasnetc_conn_implied_ack(gasnetc_epid2node(epid));
    }
  #endif

    do {
      if (!gasnetc_snd_reap(1)) {
        GASNETI_WAITHOOK();
      }
      /* Redo load balancing choice */
      qpi = gasnetc_epid_select_qpi(ceps, epid, op, len);
      cep = &ceps[qpi];
    } while (!gasnetc_sema_trydown(GASNETC_CEP_SQ_SEMA(cep)));
    GASNETC_TRACE_WAIT_END(POST_SR_STALL_SQ);
  }
  cep->used = 1;

  sreq->epid = gasnetc_epid(epid, qpi);
  sreq->cep = cep;

  return cep;
}

#define gasnetc_bind_cep(e,s,o,l) gasnetc_bind_cep_inner((e),(s),(o),(l),0)

GASNETI_INLINE (gasnetc_ack)
void gasnetc_ack(gasnetc_rbuf_t *rbuf) {
  #if GASNET_DEBUG
    rbuf->rbuf_handlerRunning = 1; /* To satisfy assertion on Reply path */
  #endif
    GASNETI_SAFE(gasnetc_ReplySysShort((gex_Token_t)rbuf, NULL,
                                       gasneti_handleridx(gasnetc_ack), 0));
}

GASNETI_INLINE (gasnetc_hidden_ack)
void gasnetc_hidden_ack(gasnetc_rbuf_t *rbuf, gasnetc_cep_t *cep) {
  /* A race might result in sending non-coalesced ACKs if a Request
   * or Reply in another thread picks up one we expect to find.
   * However, we'll always send the correct total number of credits
   * and we'll never have more than gasnetc_am_credits_slack delayed.
   */
  uint32_t old;
  do {
    old = gasnetc_atomic_read(&cep->am_flow.credit, 0);
    if (old >= gasnetc_am_credits_slack) {
      /* MUST send back a reply */
      gasnetc_ack(rbuf);
      break;
    }
    gasneti_assert(!gasnetc_use_srq); /* No coalescing when using SRQ */
  } while (!gasnetc_atomic_compare_and_swap(&cep->am_flow.credit, old, old+1, 0));
}

GASNETI_INLINE(gasnetc_rcv_am)
void gasnetc_rcv_am(const struct ibv_wc *comp, gasnetc_rbuf_t **spare_p GASNETI_THREAD_FARG) {
  gasnetc_rbuf_t emergency_spare;
  gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)(uintptr_t)comp->wr_id;
  const uint32_t flags = comp->imm_data;
  gasnetc_cep_t *cep = rbuf->cep;
  gasnetc_rbuf_t *spare;
  const int isrep = GASNETC_MSG_ISREPLY(flags);

  GASNETC_STAT_EVENT(RCV_AM_SNDRCV);

#if GASNETC_IBV_SRQ
  if (gasnetc_use_srq) {
    gasnetc_cep_t *orig_cep = cep;
    gasnetc_hca_t *hca = cep->hca;

#if GASNETI_STATS_OR_TRACE
    if (isrep) {
      gasneti_tick_t _starttime = ((gasnetc_buffer_t *)(uintptr_t)(rbuf->rr_sg.addr))->stamp;
      GASNETI_TRACE_EVENT_TIME(C,AM_ROUNDTRIP_TIME,gasneti_ticks_now()-_starttime);
    }
#endif

    /* SRQ means rbuf->cep is "inexact", so must reconstruct */
    cep = GASNETC_NODE2CEP(GASNETC_MSG_SRCIDX(flags));
    if (!isrep) {
      cep += gasnetc_num_qps; /* Search top half of table */
    }
    if (gasnetc_num_qps > 1) {
      int i;
      for (i=0; i<gasnetc_num_qps; ++i, ++cep) {
        if ((cep->rcv_qpn == comp->qp_num) && (cep->hca == hca)) break;
      }
      gasneti_assert(i < gasnetc_num_qps);
    }

    /* All flow-control and any Reply belong to opposite member of the pair */
    if (isrep) {
      cep += gasnetc_num_qps;
    } else {
      cep -= gasnetc_num_qps;
    }
    rbuf->cep = cep;

    /* Process and repost w/o any fancy tricks to keep credits perfectly accurate */
    gasnetc_processPacket(cep, rbuf, flags GASNETI_THREAD_PASS);
    if_pf (rbuf->rbuf_needReply) {
      /* MUST send back a reply - no coallescing */
      gasnetc_ack(rbuf);
    }

    gasnetc_rcv_post(orig_cep, rbuf);
    if (isrep) {
      gasnetc_sema_up(&hca->am_sema);
    }
  } else
#endif
  if (isrep) {
#if GASNETI_STATS_OR_TRACE
    gasneti_tick_t _starttime = ((gasnetc_buffer_t *)(uintptr_t)(rbuf->rr_sg.addr))->stamp;
    GASNETI_TRACE_EVENT_TIME(C,AM_ROUNDTRIP_TIME,gasneti_ticks_now()-_starttime);
#endif

    /* Now process the packet */
    gasnetc_processPacket(cep, rbuf, flags GASNETI_THREAD_PASS);

    /* Return the rcv buffer to the free list */
    gasnetc_lifo_push(cep->rbuf_freelist, rbuf);
  } else {
    /* Post a replacement buffer before processing the request.
     * This ensures that the credit sent with the reply will
     * have a corresponding buffer available at this end. */
    spare = (*spare_p) ? (*spare_p) : gasnetc_lifo_pop(cep->rbuf_freelist);
    if_pt (spare) {
      /* This is the normal case */
      gasnetc_rcv_post(cep, spare);
      *spare_p = rbuf;	/* recv'd rbuf becomes the spare for next pass (if any) */
    } else {
      /* Because we don't have any "spare" rbuf available to post we copy the recvd
       * message to a temporary (non-pinned) buffer so we can repost rbuf.
       */
      gasnetc_buffer_t *buf = gasneti_malloc(sizeof(gasnetc_buffer_t));
      memcpy(buf, (void *)(uintptr_t)rbuf->rr_sg.addr, sizeof(gasnetc_buffer_t));
      emergency_spare.rr_sg.addr = (uintptr_t)buf;
      emergency_spare.rr_is_rdma = 0;
      emergency_spare.rr_ep = rbuf->rr_ep;
      emergency_spare.cep = rbuf->cep;
      gasneti_assert(rbuf->rr_is_rdma == 0);
  
      gasnetc_rcv_post(cep, rbuf);

      rbuf = &emergency_spare;
      GASNETC_STAT_EVENT(ALLOC_AM_SPARE);
      GASNETI_TRACE_PRINTF(C,("ALLOC_AM_SPARE\n"));
    }

    /* Now process the packet */
    gasnetc_processPacket(cep, rbuf, flags GASNETI_THREAD_PASS);

    /* Finalize flow control */
    if_pf (rbuf->rbuf_needReply) {
      gasnetc_hidden_ack(rbuf, cep);
    }

    /* Free the temporary buffer, if any */
    if_pf (!spare) {
      gasneti_free((void *)(uintptr_t)emergency_spare.rr_sg.addr);
    }
  }

  if ((comp->byte_len <= gasnetc_amrdma_limit) && gasneti_attach_done && gasnetc_amrdma_max_peers) {
    gasnetc_amrdma_eligable(cep);
  }
}

static int gasnetc_rcv_reap(gasnetc_hca_t *hca, const int limit, gasnetc_rbuf_t **spare_p GASNETI_THREAD_FARG) {
  struct ibv_wc comp;
  int count;

  for (count = 0; count < limit; ++count) {
    int rc = ibv_poll_cq(hca->rcv_cq, 1, &comp);
    if_pt (rc == 0) {
      /* CQ empty - we are done */
      break;
    } else if_pt (rc == 1) {
      if_pt (comp.status == IBV_WC_SUCCESS) {
      #if GASNETC_DYNAMIC_CONNECT && !GASNETC_USE_CONN_THREAD
        if_pf (comp.wr_id & 1) {
          gasnetc_conn_rcv_wc(&comp);
          break; /* lower latency (and fewer implied ACKS) if we cease polling */
        }
      #endif
        gasnetc_rcv_am(&comp, spare_p GASNETI_THREAD_PASS);
      } else if (GASNETC_IS_EXITING()) {
        /* disconnected */
	break;	/* can't exit since we can be called in exit path */
      } else if (!gasneti_attach_done) {
        gasneti_fatalerror("failed to connect (rcv) status=%d", comp.status);
        break;
      } else {
#if 1
	gasnetc_dump_cqs(&comp, hca, 0);
#endif
        gasneti_fatalerror("aborting on reap of failed recv");
	break;
      }
    } else if (GASNETC_IS_EXITING()) {
      /* disconnected by another thread */
      gasnetc_exit(0);
    } else {
      GASNETC_IBV_CHECK(rc, "while reaping the recv queue");
    }
  } 

  if (count) {
    GASNETC_STAT_EVENT_VAL(RCV_REAP,count);

    #if !GASNETC_PIN_SEGMENT
    /* Handler might have queued work for firehose */
    firehose_poll();
    #endif
  }

  return count;
}

GASNETI_INLINE(gasnetc_rcv_amrdma)
int gasnetc_rcv_amrdma(gasnetc_EP_t ep, gasnetc_cep_t *cep GASNETI_THREAD_FARG) {
  gasnetc_amrdma_recv_t *recv_state = cep->amrdma_recv;
  const int recv_head = gasnetc_atomic_read(&recv_state->head, 0);
  const int recv_slot = recv_head & gasnetc_amrdma_slot_mask;
  volatile gasnetc_amrdma_hdr_t * hdr = (volatile gasnetc_amrdma_hdr_t *)recv_state->addr[recv_slot];
  gasnetc_buffer_t * msg_in;
  gasnetc_rbuf_t rbuf;
  uint32_t flags;
  int length, checksum, zeros;
#if !GASNETI_USE_ALLOCA
  void *tmp_buffer = NULL;
#endif

#if GASNETC_ANY_PAR
  gasnetc_atomic_t *slot_lock = &recv_state->busy[recv_slot].spinlock;

  /* First try a non-atomic "peek" and then try to acquire the spinlock */
  if (gasnetc_atomic_read(slot_lock, 0) ||
      ((length = hdr->length) != hdr->length_again) ||
      ((checksum = hdr->zeros) != hdr->zeros_again) ||
      !gasnetc_atomic_compare_and_swap(slot_lock, 0, 1, GASNETI_ATOMIC_ACQ)) {
    /* Another thread is working on this slot or no AM is waiting */
    return 0;
  }
#else
  if (((length = hdr->length) != hdr->length_again) ||
      ((checksum = hdr->zeros) != hdr->zeros_again)) {
    /* no AM is waiting */
    return 0;
  }
#endif
  gasneti_assert(GASNETC_ALLOW_0BYTE_MSG ? (length >= 0) : (length > 0));
  gasneti_assert(length <= gasnetc_amrdma_limit);

  flags = hdr->immediate_data;
  zeros = gasneti_count0s_uint32_t(flags);
  msg_in = (gasnetc_buffer_t *)(hdr + 1);
  if (GASNETC_MSG_CATEGORY(flags) == gasneti_Medium) {
    /* Relocate the Medium to avoid problems w/ "late zeros" and provide 8-byte alignment */
    /* Note: no harm here if flags is not correct, since length must by ok */
#if GASNETI_USE_ALLOCA
    void *tmp = alloca(length + 8);
#else
    /* XXX: Work around bug 2079 ("stack overflow" from alloca() w/ PGI compiler)
       TODO: Try freelist or thread-specific buffers (of max size)? */
    void *tmp = tmp_buffer = gasneti_malloc(length + 8);
#endif
    tmp = (void*)GASNETI_ALIGNUP(tmp, 8);
    zeros += gasneti_count0s_copy(tmp, msg_in, length);
    rbuf.rr_sg.addr = (uintptr_t)tmp;
  } else {
    zeros += gasneti_count0s(msg_in, length);
    rbuf.rr_sg.addr = (uintptr_t)msg_in;
  }

#if GASNETC_ANY_PAR
  if ((zeros != checksum) ||
      !gasnetc_atomic_compare_and_swap(&recv_state->head, recv_head, recv_head+1, 0)) {
    /* If CAS failed then we've been "left behind" and are looking at the wrong slot */
    gasnetc_atomic_set(slot_lock, 0, 0); /* No _REL, since nothing global was written */
    #if !GASNETI_USE_ALLOCA
      if (tmp_buffer) gasneti_free(tmp_buffer);
    #endif
    return 0;
  }
#else
  if (zeros != checksum) {
    #if !GASNETI_USE_ALLOCA
      if (tmp_buffer) gasneti_free(tmp_buffer);
    #endif
    return 0;
  }
  gasnetc_atomic_increment(&recv_state->head, 0);
#endif

  GASNETC_STAT_EVENT(RCV_AM_RDMA);

  /* Account for any recv buffer that was reserved for the reply, but not used.
   * Must precede credit processing in gasnetc_processPacket (bug 2359) */
  if (GASNETC_MSG_ISREPLY(flags)) {
#if GASNETC_IBV_SRQ
    if (gasnetc_use_srq) {
      gasnetc_sema_up(&cep->hca->am_sema);
    } else
#endif
    gasnetc_sema_up(&cep->am_loc);
  }

  /* Process the packet, includes running handler and processing credits/acks */
  rbuf.cep = cep;
  rbuf.rr_is_rdma = 1;
  rbuf.rr_ep = ep;
  gasnetc_processPacket(cep, &rbuf, flags GASNETI_THREAD_PASS);

  /* Mark slot free locally prior to enabling the ack */
  hdr->length = 0; hdr->length_again = -1;
  hdr->zeros = 0;  hdr->zeros_again = -1;
  hdr->immediate_data = 0;
  memset(msg_in, 0, length);

#if GASNETC_ANY_PAR
  gasnetc_atomic_set(slot_lock, 0, GASNETI_ATOMIC_REL);

  /* We must gather acks to keep them in-order even when handler completions are not */
  /* XXX: could be done lockless via recv_tail and ack_bits packed in gasneti_atomic64_t? */
  gasneti_mutex_lock(&recv_state->ack_lock);
  { int count;
    const int recv_tail = recv_state->tail;
    uint32_t bits = recv_state->ack_bits | (1 << (recv_head - recv_tail));

    gasneti_assert(bits != 0);

  #if GASNETI_HAVE_CC_BUILTIN_CTZ
    if_pt (~bits) {
      count = __builtin_ctz(~bits);
      bits >>= count;
    } else {
      count = 32;
      bits = 0;
    }
  #else
    for (count = 0; bits & 1; ++count) {
      bits >>= 1;
    }
  #endif

    recv_state->ack_bits = bits;
    if_pt (count) {
      recv_state->tail += count;
      gasnetc_atomic_add(&cep->am_flow.ack, count, 0);
    }
  }
  gasneti_mutex_unlock(&recv_state->ack_lock);
#else
  gasnetc_atomic_increment(&cep->am_flow.ack, 0);
#endif

  /* Finalize flow control */
  if_pf (rbuf.rbuf_needReply) {
    gasnetc_hidden_ack(&rbuf, cep);
  }
  
  gasneti_assert(gasneti_attach_done && gasnetc_amrdma_max_peers);
  gasnetc_amrdma_eligable(cep);

#if !GASNETI_USE_ALLOCA
  if (tmp_buffer) gasneti_free(tmp_buffer);
#endif
  return 1;
}

GASNETI_INLINE(gasnetc_poll_rcv_hca)
void gasnetc_poll_rcv_hca(gasnetc_EP_t ep, gasnetc_hca_t *hca, int limit GASNETI_THREAD_FARG) {
  int count = gasnetc_atomic_read(&hca->amrdma_rcv.count, 0);
  int limit2 = count + 1;

  /* BUG1652: full solution may require more atomicity when the polling set is changing? */

  /* Poll round-robin over the AMRDMA landing zones and the CQ */
  while (limit && limit2--) {
    int index = hca->amrdma_rcv.prev; /* The associated data race is harmless */
    index = (index == 0) ? count : (index - 1);
    hca->amrdma_rcv.prev = index;

    gasneti_assert(limit > 0);
    gasneti_assert(limit2 >= 0);
 
    if (index < count) { /* (index > count) is possible if another thread read a higher count */
      /* Poll for AM-over-RDMA */
      gasnetc_cep_t * cep;
      /* cep = (gasnetc_cep_t *)gasneti_atomic_ptr_read(&hca->amrdma_rcv.cep[index]); */
      cep = hca->amrdma_rcv.cep[index];
      if (cep && gasnetc_rcv_amrdma(ep, cep GASNETI_THREAD_PASS)) --limit;
    } else {
      /* Poll for AM in recv CQ */
      gasnetc_rbuf_t *spare = NULL;
      (void)gasnetc_rcv_reap(hca, limit, &spare GASNETI_THREAD_PASS);
      if (spare) {
        gasnetc_lifo_push(&hca->rbuf_freelist, spare);
      }
    }
  }
}

GASNETI_INLINE(gasnetc_do_poll)
void gasnetc_do_poll(int poll_rcv, int poll_snd GASNETI_THREAD_FARG) {
  const gasnetc_EP_t ep = gasnetc_ep0; // TODO-EX: replace this via args
  if (poll_rcv) {
  #if GASNETC_IB_MAX_HCAS > 1
    /* Simple round-robin (w/ a harmless multi-thread race) */
    /* Note use of casts to volatile are require to work around bug 1586 */
    static int index = 0;
    int tmp = *(volatile int *)(&index);
    *(volatile int *)(&index) = ((tmp == 0) ? gasnetc_num_hcas : tmp) - 1;
    gasnetc_hca_t *hca = &gasnetc_hca[tmp];
  #else
    gasnetc_hca_t *hca = &gasnetc_hca[0];
  #endif
    gasnetc_poll_rcv_hca(ep, hca, GASNETC_RCV_REAP_LIMIT GASNETI_THREAD_PASS);
  #if GASNET_PSHM
    gasneti_AMPSHMPoll(0 GASNETI_THREAD_PASS);
  #endif
  }

  if (poll_snd) {
    (void)gasnetc_snd_reap(GASNETC_SND_REAP_LIMIT);
  }
}

/* helper for allocation of a send request structure */
GASNETI_NEVER_INLINE(gasnetc_get_sreq_miss,
gasnetc_sreq_t *gasnetc_get_sreq_miss(gasnetc_sreq_t * const oldest GASNETI_THREAD_FARG))
{
    gasnetc_sreq_t *sreq = oldest;

    /* 2) Next poll all CQs and then check the oldest sreq again */
    int h;
    GASNETC_FOR_ALL_HCA_INDEX(h) {
      (void)gasnetc_snd_reap(1);
    }
    if_pf (sreq->opcode != GASNETC_OP_FREE) {
      /* 3) Next scan ahead, skipping over in-flight firehose misses for instance */
      do {
        sreq = sreq->next;
      } while ((sreq->opcode != GASNETC_OP_FREE) && (sreq != oldest));
      if_pf (sreq->opcode != GASNETC_OP_FREE) {
        /* 4) Finally allocate more */
        gasnetc_sreq_t *head, *tail;
        gasneti_assert(sreq == oldest);
        gasnetc_alloc_sreqs(&head, &tail GASNETI_THREAD_PASS);
        tail->next = sreq->next;
        sreq = (sreq->next = head);
      }
    }

    return sreq;
}

/* allocate a send request structure */
GASNETI_INLINE(gasnetc_get_sreq) GASNETI_MALLOC
gasnetc_sreq_t *gasnetc_get_sreq(gasnetc_sreq_opcode_t opcode GASNETI_THREAD_FARG) {
  gasnetc_per_thread_t *td = gasnetc_my_perthread();
  gasnetc_sreq_t *sreq;

  /* 1) First try the oldest sreq in our list */
  sreq = td->sreqs;
  gasneti_assert(sreq != NULL);
  if_pf (sreq->opcode != GASNETC_OP_FREE) {
    /* steps 2...4 above */
    sreq = gasnetc_get_sreq_miss(sreq GASNETI_THREAD_PASS);
  }

  td->sreqs = sreq->next;
  gasneti_assert(td->sreqs != NULL);

  #if GASNET_DEBUG
    /* invalidate field(s) which should always be set by caller */
    sreq->epid = ~0;
    sreq->cep = NULL;
    sreq->fh_count = -1;
    #if !GASNETC_PIN_SEGMENT
    sreq->fh_len = ~0;
    #endif
  #endif

  /* Assume no counters */
  sreq->comp.cb = NULL;
  #if !GASNETC_PIN_SEGMENT
    sreq->fh_lc_cb = NULL;
    sreq->fh_oust = NULL;
  #endif

  gasneti_assert(sreq->opcode == GASNETC_OP_FREE);
  gasneti_assert(opcode != GASNETC_OP_FREE);
  sreq->opcode = opcode;

  return sreq;
}

/* allocate a pre-pinned bounce buffer */
GASNETI_INLINE(gasnetc_get_bbuf) GASNETI_MALLOC
gasnetc_buffer_t *gasnetc_get_bbuf(int block GASNETI_THREAD_FARG) {
  gasnetc_buffer_t *bbuf = NULL;

  GASNETC_TRACE_WAIT_BEGIN();
  GASNETC_STAT_EVENT(GET_BBUF);

  bbuf = gasnetc_lifo_pop(&gasnetc_bbuf_freelist);
  if_pf (!bbuf) {
    gasnetc_poll_snd();
    bbuf = gasnetc_lifo_pop(&gasnetc_bbuf_freelist);
    if (block) {
      while (!bbuf) {
        GASNETI_WAITHOOK();
        gasnetc_poll_snd();
        bbuf = gasnetc_lifo_pop(&gasnetc_bbuf_freelist);
      }
      GASNETC_TRACE_WAIT_END(GET_BBUF_STALL);
    }
  }
  gasneti_assert((bbuf != NULL) || !block);

  return bbuf;
}

#if GASNET_TRACE || GASNET_DEBUG
GASNETI_INLINE(gasnetc_snd_validate)
void gasnetc_snd_validate(gasnetc_sreq_t *sreq, struct ibv_send_wr *sr_desc, int count, const char *type) {
  int i, j;

  gasneti_assert(sreq);
  gasneti_assert(sreq->cep);
  gasneti_assert(!GASNETI_NBRHD_JOBRANK_IS_LOCAL(gasnetc_epid2node(sreq->cep->epid)));
  gasneti_assert(sr_desc);
  gasneti_assert(sr_desc->num_sge >= 1);
  gasneti_assert(sr_desc->num_sge <= GASNETC_SND_SG);
  gasneti_assert(count > 0);
  gasneti_assert(type);

  GASNETI_TRACE_PRINTF(D,("%s sreq=%p peer=%d qp=%d hca=%d\n", type, (void *)sreq,
			  gasnetc_epid2node(sreq->cep->epid),
			  gasnetc_epid2qpi(sreq->cep->epid) - 1,
			  GASNETC_HCA_IDX(sreq->cep)));
  for (i = 0; i < count; ++i, ++sr_desc) {
    uintptr_t r_addr = sr_desc->wr.rdma.remote_addr;

    switch (sr_desc->opcode) {
    case IBV_WR_SEND_WITH_IMM:
      GASNETI_TRACE_PRINTF(D,("%s op=SND\n", type));
      for (j = 0; j < sr_desc->num_sge; ++j) {
        uintptr_t l_addr = sr_desc->sg_list[j].addr;
        size_t    len    = sr_desc->sg_list[j].length;
	unsigned  lkey   = sr_desc->sg_list[j].lkey;
        GASNETI_TRACE_PRINTF(D,("  %i: lkey=0x%08x len=%"PRIuPTR" local=[%p-%p] remote=N/A\n",
			        j, lkey, (uintptr_t)len,
			        (void *)l_addr, (void *)(l_addr + (len - 1))));
      }
      break;

    case IBV_WR_RDMA_WRITE:
      GASNETI_TRACE_PRINTF(D,("%s op=PUT rkey=0x%08x\n", type, (unsigned int)sr_desc->wr.rdma.rkey));
      for (j = 0; j < sr_desc->num_sge; ++j) {
        uintptr_t l_addr = sr_desc->sg_list[j].addr;
        size_t    len    = sr_desc->sg_list[j].length;
	unsigned  lkey   = sr_desc->sg_list[j].lkey;
        GASNETI_TRACE_PRINTF(D,("  %i: lkey=0x%08x len=%"PRIuPTR" local=[%p-%p] remote=[%p-%p]\n",
			        j, lkey, (uintptr_t)len,
				(void *)l_addr, (void *)(l_addr + (len - 1)),
				(void *)r_addr, (void *)(r_addr + (len - 1))));
	r_addr += len;
      }
      break;

    case IBV_WR_RDMA_READ:
      GASNETI_TRACE_PRINTF(D,("%s op=GET rkey=0x%08x\n", type, (unsigned int)sr_desc->wr.rdma.rkey));
      for (j = 0; j < sr_desc->num_sge; ++j) {
        uintptr_t l_addr = sr_desc->sg_list[j].addr;
        size_t    len    = sr_desc->sg_list[j].length;
	unsigned  lkey   = sr_desc->sg_list[j].lkey;
        GASNETI_TRACE_PRINTF(D,("  %i: lkey=0x%08x len=%"PRIuPTR" local=[%p-%p] remote=[%p-%p]\n",
			        j, lkey, (uintptr_t)len,
				(void *)l_addr, (void *)(l_addr + (len - 1)),
				(void *)r_addr, (void *)(r_addr + (len - 1))));
	r_addr += len;
      }
      break;

    default:
      gasneti_fatalerror("Invalid operation %d for %s\n", sr_desc->opcode, type);
    }

    /* check for reasonable message sizes
     * With SEND 0-bytes triggers a Mellanox bug
     * With RDMA ops, 0-bytes makes no sense.
     */
    #if GASNET_DEBUG
    {
      u_int32_t	sum = 0;

      for (i = 0; i < sr_desc->num_sge; ++i) {
        sum += sr_desc->sg_list[i].length;
        gasneti_assert(GASNETC_ALLOW_0BYTE_MSG ||
                       (sr_desc->sg_list[i].length != 0));
        gasneti_assert(sr_desc->sg_list[i].length <= gasnetc_max_msg_sz);
        gasneti_assert(sr_desc->sg_list[i].length <= sum); /* check for overflow of 'sum' */
      }

      gasneti_assert(sum <= gasnetc_max_msg_sz);
    }
    #endif
  }
}
#else /* DEBUG || TRACE */
  #define gasnetc_snd_validate(a,b,c,d)	do {} while (0)
#endif /* DEBUG || TRACE */


GASNETI_INLINE(gasnetc_snd_post_common)
void gasnetc_snd_post_common(gasnetc_sreq_t *sreq, struct ibv_send_wr *sr_desc, int is_inline GASNETI_THREAD_FARG) {
  gasnetc_cep_t * const cep = sreq->cep;
  int vstat;

  /* Must be bound to a qp by now */
  gasneti_assert(cep != NULL );
  gasneti_assert(!GASNETI_NBRHD_JOBRANK_IS_LOCAL(gasnetc_epid2node(sreq->epid)));

  gasneti_assert(sreq->opcode != GASNETC_OP_FREE);
  gasneti_assert(sreq->opcode != GASNETC_OP_INVALID);

  /* A valid callback will never take NULL as its data */
  gasneti_assert(sreq->comp.cb == NULL || sreq->comp.data != NULL);

  /* Loop until space is available for 1 new entry on the CQ.
   * If we hold the last one then threads sending to ANY node will stall. */
  if_pf (!gasnetc_sema_trydown(cep->snd_cq_sema_p)) {
    GASNETC_TRACE_WAIT_BEGIN();
    do {
      GASNETI_WAITHOOK();
      gasnetc_poll_snd();
    } while (!gasnetc_sema_trydown(cep->snd_cq_sema_p));
    GASNETC_TRACE_WAIT_END(POST_SR_STALL_CQ);
  }

  /* setup some invariant fields */
  sr_desc[0].wr_id = (uintptr_t)sreq;

  /* Trace and debug */
  if (is_inline) {
    GASNETC_STAT_EVENT(POST_INLINE_SR);
    gasnetc_snd_validate(sreq, sr_desc, 1, "POST_INLINE_SR");
  } else {
    GASNETC_STAT_EVENT_VAL(POST_SR, sr_desc->num_sge);
    gasnetc_snd_validate(sreq, sr_desc, 1, "POST_SR");
  }

  /* Post it */
  {
    struct ibv_send_wr *bad_wr;
    sr_desc->next = NULL;
    sr_desc->send_flags = is_inline ? IBV_SEND_INLINE : (enum ibv_send_flags)0;
  #if GASNETC_IBV_XRC_OFED
    sr_desc->qp_type.xrc.remote_srqn = cep->xrc_remote_srq_num; /* Even if unused */
  #elif GASNETC_IBV_XRC_MLNX
    sr_desc->xrc_remote_srq_num = cep->xrc_remote_srq_num; /* Even if unused */
  #endif
    vstat = ibv_post_send(cep->qp_handle, sr_desc, &bad_wr);
  }

  if_pt (vstat == 0) {
    /* SUCCESS, the request is posted */
    return;
  } else if (GASNETC_IS_EXITING()) {
    /* disconnected by another thread */
    gasnetc_exit(0);
  } else {
    /* unexpected error */
    GASNETC_IBV_CHECK(vstat, is_inline ? "while posting an inline send work request"
    					: "while posting a send work request");
  }
}
#define gasnetc_snd_post(x,y)		gasnetc_snd_post_common(x,y,0 GASNETI_THREAD_PASS)
#define gasnetc_snd_post_inline(x,y)	gasnetc_snd_post_common(x,y,1 GASNETI_THREAD_PASS)

#if GASNETC_USE_RCV_THREAD
static void gasnetc_rcv_thread(struct ibv_wc *comp_p, void *arg)
{
  gasnetc_hca_t * const hca = (gasnetc_hca_t *)arg;
  gasnetc_rbuf_t ** const spare_p = &hca->rcv_thread_priv;

#if GASNETI_THREADINFO_OPT
  if_pf (! hca->rcv_threadinfo) hca->rcv_threadinfo = _gasneti_mythread_slow();
  GASNET_POST_THREADINFO(hca->rcv_threadinfo);
#endif

  gasneti_assert(gasnetc_use_rcv_thread);

  if_pf (comp_p->status != IBV_WC_SUCCESS) {
    gasnetc_dump_cqs(comp_p, hca, 0);
    gasneti_fatalerror("aborting on reap of failed AM recv");
  }
  #if GASNETC_DYNAMIC_CONNECT && !GASNETC_USE_CONN_THREAD
  else if_pf (comp_p->wr_id & 1) {
    gasnetc_conn_rcv_wc(comp_p);
  }
  #endif
  else {
    gasnetc_rcv_am(comp_p, spare_p GASNETI_THREAD_PASS);
    GASNETC_STAT_EVENT_VAL(RCV_REAP, 1);
  #if !GASNETC_PIN_SEGMENT
    /* Handler might have queued work for firehose */
    firehose_poll();
  #endif 
    GASNETI_PROGRESSFNS_RUN();
  }
}
#endif /* GASNETC_USE_RCV_THREAD */

/* Try to claim the next slot */
GASNETI_INLINE(gasnetc_get_amrdma_slot)
int gasnetc_get_amrdma_slot(gasnetc_cep_t *cep, size_t msg_len) {
  gasnetc_amrdma_send_t *send_state = cep->amrdma_send;
  uint32_t send_tail;

  gasneti_assert(GASNETC_ALLOW_0BYTE_MSG || (msg_len != 0));
  if (!send_state || (msg_len > gasnetc_amrdma_limit)) {
    return -1;
  }

#if GASNETC_ANY_PAR
  while (1) {
    send_tail = gasnetc_atomic_read(&send_state->tail, 0);
    if (send_tail == gasnetc_atomic_read(&send_state->head, 0)) { return -1; }
    if (gasnetc_atomic_compare_and_swap(&send_state->tail, send_tail, send_tail + 1, 0)) { break; }
    GASNETI_WAITHOOK();
  }
#else
  send_tail = gasnetc_atomic_read(&send_state->tail, 0);
  if (send_tail == gasnetc_atomic_read(&send_state->head, 0)) { return -1; }
  gasnetc_atomic_increment(&send_state->tail, 0);
#endif

  return (send_tail & gasnetc_amrdma_slot_mask);
}

GASNETI_INLINE(gasnetc_encode_amrdma)
size_t gasnetc_encode_amrdma(gasnetc_cep_t *cep, struct ibv_send_wr *sr_desc, int send_slot) {
  size_t len0 = sr_desc->sg_list[0].length;
  size_t len1 = sr_desc->sg_list[1].length;

  gasneti_assert(send_slot >= 0);
  gasneti_assert(send_slot < gasnetc_amrdma_depth);

  /* Build header */
  { 
    void * const data = (void *)(uintptr_t)sr_desc->sg_list[0].addr;
    gasnetc_amrdma_hdr_t * const hdr = (gasnetc_amrdma_hdr_t *)data - 1;
    const uint32_t flags = sr_desc->imm_data;
    const int zeros = gasneti_count0s_uint32_t(flags) +
                      gasneti_count0s(data, len0) +
                      (len1 ? gasneti_count0s((void *)(uintptr_t)sr_desc->sg_list[1].addr, len1) : 0);

    hdr->length = hdr->length_again = len0 + len1;
    hdr->zeros  = hdr->zeros_again  = zeros;
    hdr->immediate_data = flags;
  }

  { /* Fix up the descriptor */
    sr_desc->sg_list[0].addr -= sizeof(gasnetc_amrdma_hdr_t);
    sr_desc->sg_list[0].length = (len0 += sizeof(gasnetc_amrdma_hdr_t));
    sr_desc->opcode = IBV_WR_RDMA_WRITE;
    sr_desc->wr.rdma.remote_addr = cep->amrdma_send->addr + (send_slot << GASNETC_AMRDMA_SZ_LG2);
    sr_desc->wr.rdma.rkey = cep->amrdma_send->rkey;

    gasneti_assert((len0 + len1) <= GASNETC_AMRDMA_SZ);
    gasneti_assert((len0 + len1) <= GASNETC_BUFSZ);
    return (size_t)(len0 + len1);
  }
}

#if GASNETC_PIN_SEGMENT
GASNETI_INLINE(gasnetc_seg_index) int gasnetc_seg_index(uintptr_t offset);
GASNETI_INLINE(gasnetc_seg_one_reg) int gasnetc_seg_one_reg(uintptr_t addr, size_t len);
#endif

GASNETI_INLINE(gasnetc_ReqRepGeneric)
int gasnetc_ReqRepGeneric(gasnetc_EP_t ep,
                          gasneti_category_t category, gasnetc_rbuf_t * const token,
			  gasnetc_epid_t dest, gex_AM_Index_t handler,
			  void *src_addr, int nbytes, void *dst_addr,
			  gex_Flags_t flags, int numargs,
			  gasnetc_atomic_val_t *local_cnt,
                          gasnetc_cb_t local_cb,
			  gasnetc_counter_t *counter, va_list argptr
                          GASNETI_THREAD_FARG) {
  #if GASNETC_IBV_SHUTDOWN
    /* Currently only the shutdown code uses dest to specify a "bound" value */
    const gex_Rank_t node = gasnetc_epid2node(dest);
    const gasnetc_epid_t dest_qpi = gasnetc_epid2qpi(dest);
  #else
    const gex_Rank_t node = dest;
    gasneti_assert(gasnetc_epid2qpi(dest) == 0);
  #endif
    gasneti_assert(!GASNETI_NBRHD_JOBRANK_IS_LOCAL(node));

    /* Remote Case */
    gasnetc_buffer_t *buf, *buf_alloc = NULL;
    gex_AM_Arg_t *args;
    size_t len0;     // Length of first (possibly only) message segment
    size_t len1 = 0; // Length of second (payload) message segment if using gather
    int i;
    int immediate = flags & GEX_FLAG_IMMEDIATE;
    int fail_type = GASNETC_FAIL_IMM;
    int have_flow;
    int packedlong = 0;
    int rdma_slot;
    gasnetc_epid_t epid;
    gasnetc_cep_t *cep;
    char tmp_buf[sizeof(gasnetc_am_tmp_buf_t) + 8];
  
    /* For a Reply, we must go back via the "same" qp that the Request came in on.
     * With SRQ the token already includes the cross-over to the Reply channel.
     * For a Request, we bind to a qp now to be sure everything goes on one qp.
     */
    if (token) {
      gasneti_assert(token->rbuf_handlerRunning);
      gasneti_assert(GASNETC_MSG_ISREQUEST(token->rbuf_flags));
      gasneti_assert(token->rbuf_needReply);

      cep = token->cep;
      epid = cep->epid;
#if GASNETC_IBV_SHUTDOWN /* Currently only the shutdown code uses dest to specify a "bound" value */
    } else if (dest_qpi) {
      epid = dest;
      cep = gasnetc_get_cep(node) + (dest_qpi - 1);
#endif
    } else {
      /* TODO: could bind by largest avail credits (or at least favor non-zero over zero) */
      const int qp_offset = gasnetc_use_srq ? gasnetc_num_qps : 0;
      int qpi;
      cep = gasnetc_get_cep(node) + qp_offset;
#if 0
      /* Bind to a specific queue pair, selecting by largest credits */
      qpi = 0;
      if (gasnetc_num_qps > 1) {
        uint32_t best_credits;
        int i;
        /* gasnetc_poll_snd(); here? */
        best_credits = gasnetc_sema_read(&cep[0].am_rem);
        for (i = 1; i < gasnetc_num_qps; ++i) {
	  const uint32_t tmp = gasnetc_sema_read(&cep[i].am_rem);
	  if (tmp > best_credits) {
	    best_credits = tmp;
	    qpi = i;
	  }
        }
      }
#else
      { size_t rough_len = MIN(GASNETC_BUFSZ, 4*numargs + nbytes);
        qpi = gasnetc_epid_select_qpi(cep, dest, IBV_WR_SEND_WITH_IMM, rough_len);
      }
#endif
      epid = gasnetc_epid(node, qpi + qp_offset);
      cep += qpi;
    }
    gasneti_assert(epid == cep->epid);
  
    /* Reserve space for extra arguments if we *might* carry flow control
     * data.  We need to know numargs before we allocate a large enough
     * buffer, which could block and thus delay the credit update.  So, we
     * allow a race where we allocate space for the args, but end up sending
     * pointless zero values in them.
     */
    have_flow = (gasnetc_atomic_read(&cep->am_flow.credit, 0) ||
		 gasnetc_atomic_read(&cep->am_flow.ack, 0));
    if (have_flow) numargs += 1;
  
    /* Figure out length so we know if we can use inline or not.
     * Also, if using firehose then Long requests may need AMs for moves.
     * Thus we MUST do any RDMA before getting credits.  It can't hurt to queue
     * the Long RDMA as early as possible even when firehose is not in use.
     */
    switch (category) {
    case gasneti_Short:
      len0 = GASNETC_MSG_SHORT_ARGSEND(numargs);
#if !GASNETC_ALLOW_0BYTE_MSG
      if (!len0) len0 = 4; /* Mellanox bug (zero-length sends) work-around */
#endif
      break;
  
    case gasneti_Medium:
      /* XXX: When nbytes == 0 we still round up the header to 8-bytes */
    #if GASNETC_PIN_SEGMENT
      len0 = GASNETC_MSG_MED_ARGSEND(numargs);
      if (nbytes) {
        // Use gather (len1 != 0) IFF:
        //  + !gasnetc_cb_counter: else desire synchronous LC (GEX_EVENT_NOW)
        //  + gasnetc_seg_one_reg: else not in-segment w/ a single registration
        // TODO: could gather w/ local firehose
        if ((local_cb != gasnetc_cb_counter) && gasnetc_seg_one_reg((uintptr_t)src_addr, nbytes)) {
          len1 = nbytes;
        } else {
          len0 += nbytes;
        }
      }
    #else
      len0 = GASNETC_MSG_MED_ARGSEND(numargs) + nbytes;
    #endif
      break;
  
    case gasneti_Long:
      len0 = GASNETC_MSG_LONG_ARGSEND(numargs);
      /* Start moving the Long payload if possible */
      if (nbytes) {
        if ((nbytes <= gasnetc_packedlong_limit) || (!GASNETC_PIN_SEGMENT && token)) {
	  /* Small enough to send like a Medium, or a Reply when using remote firehose. */
        #if GASNETC_PIN_SEGMENT
          // See Medium case, above, for explanation of when we use gather
          if ((local_cb != gasnetc_cb_counter) && gasnetc_seg_one_reg((uintptr_t)src_addr, nbytes)) {
            len1 = nbytes;
          } else {
            len0 += nbytes;
          }
        #else
          len0 += nbytes;
        #endif
	  packedlong = 1;
        } else {
          // TODO-EX: should we pass anything more than 'immediate' to the payload Put
          #if GASNETC_PIN_SEGMENT
	    /* Queue the RDMA.  We can count on point-to-point ordering to deliver payload before header */
            int rc = gasnetc_rdma_put(epid, src_addr, dst_addr, nbytes, immediate,
                                      local_cnt, local_cb, NULL, NULL
                                      GASNETI_THREAD_PASS);
            if (rc) {
              // TODO-EX: stats/trace for FAIL_IMM case
              fail_type = rc;
              goto out_no_put;
            }
          #else
	    /* Point-to-point ordering still holds, but only once the RDMA is actually queued.
	     * In the case of a firehose hit, the RDMA is already queued before return from
	     * gasnetc_rdma_put_fh().  On a miss, however, we'll need to spin on am_oust to
	     * determine when all the RDMA is actually queued.
	     * It would have been nice to move the wait down further in this function, but
	     * that would lead to deadlock if we hold the resources needed to queue the RDMA.
	     */
	    gasnetc_counter_t am_oust = GASNETC_COUNTER_INITIALIZER;
	    gasneti_assert(!token);	/* Replies MUST have been caught above */
	    int rc = gasnetc_rdma_put_fh(epid, src_addr, dst_addr, nbytes, immediate,
                                         local_cnt, local_cb, NULL, NULL, &am_oust
                                         GASNETI_THREAD_PASS);
            if (rc) {
              // TODO-EX: stats/trace for FAIL_IMM case
              fail_type = rc;
              goto out_no_put;
            }
	    gasnetc_counter_wait(&am_oust, 0 GASNETI_THREAD_PASS);
          #endif
            immediate = 0; // No turing back now!
            // TODO-EX: can we delay the Put to avoid disabling 'immediate'?
        }
      }
      break;
  
    default: gasneti_unreachable_error(("Invalid category in gasnetc_ReqRepGeneric: 0x%x",(int)category));
    }
  
    if (!token) {
      /* NEXT, get the flow-control credit needed for Requests.
       * This order ensures that we never hold the last pinned buffer
       * while spinning on the rcv queue waiting for credits.
       * Note that we need a credit even for AM-over-RDMA, to avoid
       * posting all of our rbufs to one QP.
       */
      {
        gasnetc_sema_t * const sema = &(cep->am_rem);
        GASNETC_STAT_EVENT(GET_AMREQ_CREDIT);
        if_pf (!gasnetc_sema_trydown(sema)) {
          GASNETC_TRACE_WAIT_BEGIN();
          if (immediate) {
          #if GASNETC_IMMEDIATE_AMPOLLS
            // A *full* Poll, but only once
            gasnetc_poll_rcv_hca(ep, cep->hca, GASNETC_RCV_REAP_LIMIT GASNETI_THREAD_PASS);
            if (!gasnetc_sema_trydown(sema)) {
              // TODO-EX: stats/trace for this as distinct from ..._STALL
              goto out_no_credit;
            }
          #else
            goto out_no_credit;
          #endif
          } else {
            do {
	      GASNETI_WAITHOOK();
              gasnetc_poll_rcv_hca(ep, cep->hca, 1 GASNETI_THREAD_PASS);
            } while (!gasnetc_sema_trydown(sema));
            GASNETC_TRACE_WAIT_END(GET_AMREQ_CREDIT_STALL);
          }
        }
      }
  
      /* Post or account for the rbuf needed for the Reply */
#if GASNETC_IBV_SRQ
      if (gasnetc_use_srq) {
        gasnetc_sema_t * const sema = &(cep->hca->am_sema);
        if_pf (!gasnetc_sema_trydown(sema)) {
          GASNETC_TRACE_WAIT_BEGIN();
          if (immediate) {
          #if GASNETC_IMMEDIATE_AMPOLLS
            // A *full* Poll, but only once
            gasnetc_poll_rcv_hca(ep, cep->hca, GASNETC_RCV_REAP_LIMIT GASNETI_THREAD_PASS);
            if (!gasnetc_sema_trydown(sema)) {
              // TODO-EX: stats/trace for this as distinct from ..._STALL
              goto out_no_rbuf;
            }
          #else
            goto out_no_rbuf;
          #endif
          } else {
            do {
	      GASNETI_WAITHOOK();
              gasnetc_poll_rcv_hca(ep, cep->hca, 1 GASNETI_THREAD_PASS);
            } while (!gasnetc_sema_trydown(sema));
          }
          GASNETC_TRACE_WAIT_END(GET_AMREQ_BUFFER_STALL);
        }
        gasneti_assert(NULL == gasnetc_lifo_pop(cep->rbuf_freelist));
      } else
#endif
      if (gasnetc_sema_trydown(&cep->am_loc)) {
        /* We'll use one that was left over due to ACK coalescing or reply via rdma */
      } else {
        gasnetc_rbuf_t *rbuf = gasnetc_lifo_pop(cep->rbuf_freelist);
        if_pf (rbuf == NULL) {
          GASNETC_TRACE_WAIT_BEGIN();
          if (immediate) {
          #if GASNETC_IMMEDIATE_AMPOLLS
            // A *full* Poll, but only once
            gasnetc_poll_rcv_hca(ep, cep->hca, GASNETC_RCV_REAP_LIMIT GASNETI_THREAD_PASS);
	    if (!gasnetc_sema_trydown(&cep->am_loc) &&
	        !(rbuf = gasnetc_lifo_pop(cep->rbuf_freelist))) {
              // TODO-EX: stats/trace for this as distinct from ..._STALL
              goto out_no_rbuf;
            }
          #else
            goto out_no_rbuf;
          #endif
          } else {
            do {
	      GASNETI_WAITHOOK();
              gasnetc_poll_rcv_hca(ep, cep->hca, 1 GASNETI_THREAD_PASS);
	      if (gasnetc_sema_trydown(&cep->am_loc)) {
	        break;
	      }
	      rbuf = gasnetc_lifo_pop(cep->rbuf_freelist);
            } while (rbuf == NULL);
            GASNETC_TRACE_WAIT_END(GET_AMREQ_BUFFER_STALL);
          }
        }
        if (rbuf) {
	  gasnetc_rcv_post(cep, rbuf);
	}
      }
    }
  
    // Try to obtain a slot for AM-over-RDM *unless* IMMEDIATE
    // In the IMMEDIATE case we cannot (in general) deallocate the slot
    size_t msg_len = len0 + len1;
    rdma_slot = immediate ? -1 : gasnetc_get_amrdma_slot(cep, msg_len);
    if (rdma_slot < 0) {
      GASNETC_STAT_EVENT(SND_AM_SNDRCV);
    } else {
      GASNETC_STAT_EVENT(SND_AM_RDMA);
    }

    /* Now get a buffer and start building the message.
     * If we can be certain that the message will be small enough for an inline send/put,
     * then we'll use a buffer on the stack (saving us from accessing the shared pool of
     * bounce buffers, which might block).
     */
    const size_t max_inline = (rdma_slot >= 0) ? gasnetc_am_inline_limit_rdma
                                               : gasnetc_am_inline_limit_sndrcv;
    const size_t buf_offset = (rdma_slot >= 0) ? sizeof(gasnetc_amrdma_hdr_t) : 0;
    if (msg_len <= max_inline) {
      buf = (gasnetc_buffer_t *)GASNETI_ALIGNUP(tmp_buf, 8);
    } else if (immediate) {
      buf = (buf_alloc = gasnetc_get_bbuf(0 GASNETI_THREAD_PASS));
      if (!buf) {
        // TODO-EX: stats/trace for this as distinct from ..._STALL
        goto out_no_buffer;
      }
    } else {
      buf = (buf_alloc = gasnetc_get_bbuf(1 GASNETI_THREAD_PASS));
    }
    buf = (gasnetc_buffer_t *)((uintptr_t)buf + buf_offset);

    switch (category) {
    case gasneti_Short:
      args = buf->shortmsg.args;
      break;
  
    case gasneti_Medium:
      buf->medmsg.nBytes = nbytes;
      args = buf->medmsg.args;
      if (!len1) memcpy(GASNETC_MSG_MED_DATA(buf, numargs), src_addr, nbytes);
      break;
  
    case gasneti_Long:
      buf->longmsg.destLoc = (uintptr_t)dst_addr;
      buf->longmsg.nBytes  = nbytes;
      if (packedlong) {
        /* Pack like a Medium */
        gasneti_assert(nbytes <= GASNETC_MAX_PACKEDLONG);
        buf->longmsg.nBytes |= 0x80000000; /* IDs the packedlong case */
        if (!len1) memcpy(GASNETC_MSG_LONG_DATA(buf, numargs), src_addr, nbytes);
      }
      args = buf->longmsg.args;
      break;

    default: gasneti_unreachable_error(("Invalid category in gasnetc_ReqRepGeneric: 0x%x",(int)category));
    }
   
    /* Assemble an array of arguments. */
    i = 0;
    if (have_flow) {
      /* acks and credits travel packed in a "prefixed" argument, remaining args are shifted */

      /* "Grab" info w/ atomic load-and-clear operations: */
      const uint32_t acks = gasnetc_atomic_swap(&cep->am_flow.ack, 0, 0);
      const uint32_t credits = gasnetc_atomic_swap(&cep->am_flow.credit, 0, 0);

      gasneti_assert(acks <= 255);
      gasneti_assert(credits <= 255);

      args[0] = credits | (acks << 8) | (numargs << 16);
      i = 1;

      GASNETI_TRACE_PRINTF(C,("SND_AM_CREDITS credits=%d acks=%d\n", credits, acks));
    }
    for (/*EMPTY*/; i < numargs; ++i) {
      args[i] = va_arg(argptr, gex_AM_Arg_t);
    }
  
    /* Add/forward optional timestamp */
    #if GASNETI_STATS_OR_TRACE
      buf->stamp = token ? ((gasnetc_buffer_t *)(uintptr_t)(token->rr_sg.addr))->stamp : GASNETI_TICKS_NOW_IFENABLED(C);
    #endif
  
    /* send the AM */
    {
      GASNETC_DECL_SR_DESC(sr_desc, 2);
      gasnetc_sreq_t *sreq;
      int numargs_field = have_flow ? GASNETC_MAX_ARGS : numargs;

      sr_desc->imm_data   = GASNETC_MSG_GENFLAGS(!token, category, numargs_field, handler,
						 gasneti_mynode);
      sr_desc->opcode     = IBV_WR_SEND_WITH_IMM;
      sr_desc->num_sge    = 1;
      sr_desc->sg_list[0].addr   = (uintptr_t)buf;
      sr_desc->sg_list[0].length = len0;
      sr_desc->sg_list[0].lkey   = GASNETC_SND_LKEY(cep);
      sr_desc->sg_list[1].length = len1;
  
      sreq = gasnetc_get_sreq(GASNETC_OP_AM GASNETI_THREAD_PASS);
      sreq->am_buff = buf_alloc;

      if_pf (counter) { // Caller requires remote completion indication
        gasneti_assert(!len1);
        sreq->comp.cb = gasnetc_cb_counter;
        sreq->comp.data = &counter->initiated;
      }
      #if GASNETC_PIN_SEGMENT
      else if (len1) { // Gather-on-send to concatenate header and payload
        const uintptr_t offset = (uintptr_t)src_addr - gasnetc_seg_start;
        sr_desc->num_sge = 2;
        sr_desc->sg_list[1].addr   = (uintptr_t)src_addr;
        sr_desc->sg_list[1].lkey   = GASNETC_SEG_LKEY(cep, gasnetc_seg_index(offset));
        sreq->comp.cb = local_cb;
        sreq->comp.data = local_cnt;
        ++(*local_cnt);
      }
      #endif
  
      (void)gasnetc_bind_cep_inner(epid, sreq, IBV_WR_SEND_WITH_IMM, msg_len, token != NULL);

      if (rdma_slot >= 0) {
        msg_len = gasnetc_encode_amrdma(sreq->cep, sr_desc, rdma_slot);
        gasneti_assert((msg_len <= gasnetc_inline_limit) || (buf_alloc != NULL));
      }

      gasnetc_snd_post_common(sreq, sr_desc, (msg_len <= gasnetc_inline_limit) GASNETI_THREAD_PASS);
    }

    if (token) token->rbuf_needReply = 0;

    GASNETI_RETURN(GASNET_OK); // Normal return

    // ---- Begin exception paths for remote case ---

  out_no_buffer:

    if (!token) { // Account for the posted-but-not-consumed reply buffer
    #if GASNETC_IBV_SRQ
      if (gasnetc_use_srq) {
        gasnetc_sema_up(&cep->hca->am_sema);
      } else
    #endif
      gasnetc_sema_up(&cep->am_loc);
    }

  out_no_rbuf:

    if (!token) { // Restore the allocated credit, if any
      gasnetc_sema_up(&cep->am_rem);
    }

  out_no_credit:

  out_no_put:

    gasneti_assert(!token || token->rbuf_needReply);

    return fail_type; // TODO-EX: the will distinguish IMMEDIATE vs (resilient) failure
}

#if GASNETC_PIN_SEGMENT
/*
 * ###############################################################
 * Static helper functions for RDMA when the segment is pre-pinned
 * ###############################################################
 */

/* Test if a given addr is in the local GASNet segment or not.
 * Returns non-zero if address is outside the segment.
 * This test is used under the assumption that the client's arguments
 * to Put or Get will always correspond to a region which is entirely
 * IN or entirely OUT of the segment.
 */
GASNETI_INLINE(gasnetc_unpinned)
int gasnetc_unpinned(uintptr_t addr) {
  const uintptr_t offset = (addr - gasnetc_seg_start); /* negative is a LARGE positive */
  return (offset > gasnetc_seg_len);
}

/* Convert from offset to the index of the corresponding registration.
   In a single registration case this always returns 0.
   This is independent of node and HCA.
*/
GASNETI_INLINE(gasnetc_seg_index)
int gasnetc_seg_index(uintptr_t offset) {
  return (offset >> gasnetc_pin_maxsz_shift);
}

/* Convert from offset to bytes remaining in the corresponding registration.
   In a single registration case, this returns gasnetc_max_msg_sz.
   Otherwise we have (gasnetc_pin_maxsz <= gasnetc_max_msg_sz) by construction.
   This is independent of node and HCA.
*/
GASNETI_INLINE(gasnetc_seg_remain)
int gasnetc_seg_remain(uintptr_t offset) {
  return (gasnetc_pin_maxsz - (offset & gasnetc_pin_maxsz_mask));
}

/* Is argument range in-segment within a *single* registration */
GASNETI_INLINE(gasnetc_seg_one_reg)
int gasnetc_seg_one_reg(uintptr_t addr, size_t len) {
  const uintptr_t offset = (addr - gasnetc_seg_start); /* negative is a LARGE positive */
  return ((offset <= gasnetc_seg_len) && (len <= gasnetc_seg_remain(offset)));
}

/* Assemble and post a bounce-buffer PUT or GET */
GASNETI_INLINE(gasnetc_bounce_common)
void gasnetc_bounce_common(gasnetc_epid_t epid, int rkey_index, struct ibv_send_wr *sr_desc, size_t len, gasnetc_sreq_t *sreq, enum ibv_wr_opcode op GASNETI_THREAD_FARG) {
  gasnetc_cep_t *cep;

  sr_desc->opcode      = op;
  sr_desc->num_sge     = 1;
  sr_desc->sg_list[0].addr = (uintptr_t)sreq->bb_buff;
  sr_desc->sg_list[0].length = len;

  cep = gasnetc_bind_cep(epid, sreq, op, len);
  sr_desc->wr.rdma.rkey = GASNETC_SEG_RKEY(cep, rkey_index);
  sr_desc->sg_list[0].lkey = GASNETC_SND_LKEY(cep);

  gasnetc_snd_post(sreq, sr_desc);
  sr_desc->wr.rdma.remote_addr += len;
}

/* Assemble and all-but-post a zero-copy PUT or GET using either the seg_lkeys table or
 * firehose to obtain the lkeys.  Both cases delay the bind to a qp until the
 * total xfer len is known.
 */
GASNETI_INLINE(gasnetc_zerocp_common)
size_t gasnetc_zerocp_common(gasnetc_epid_t epid, int rkey_index, struct ibv_send_wr *sr_desc, size_t len, gasnetc_sreq_t *sreq, enum ibv_wr_opcode op GASNETI_THREAD_FARG) {
  uintptr_t loc_addr = sr_desc->sg_list[0].addr;
  gasnetc_cep_t *cep;
  size_t remain = len;

  sr_desc->opcode = op;

  // TODO-EX:
  //     All uses of loc_auxseg are a temporary hack
  //     The idea is to allow a negative (base+seg) which gets mapped later to aux_reg
  //     This will be replaced by general multi-registration support later
  const int loc_auxseg = gasneti_in_auxsegment(NULL/*tm*/, gasneti_mynode, (void*)loc_addr, len);

  if_pf (!gasnetc_unpinned(loc_addr) || loc_auxseg) {
    /* loc_addr is in-segment */
    const uintptr_t offset = loc_addr - gasnetc_seg_start;
    const int base = loc_auxseg?-GASNETC_SND_SG: gasnetc_seg_index(offset);
    size_t count = loc_auxseg?remain: MIN(remain, gasnetc_seg_remain(offset));
    int seg;
    sreq->fh_count = 0;
    for (seg = 0; remain && (seg < GASNETC_SND_SG); ++seg) {
      gasneti_assert((base + seg) == gasnetc_seg_index(loc_addr - gasnetc_seg_start) || loc_auxseg);

      sr_desc->sg_list[seg].addr = loc_addr;
      sr_desc->sg_list[seg].length = count;

      loc_addr += count;
      remain -= count;

      /* 2nd and subsequent (if any) xfers are aligned to start of a registration */
      count = MIN(remain, gasnetc_pin_maxsz);
    }
    gasneti_assert(seg > 0);
    sr_desc->num_sge = seg;
    gasneti_assert(remain < len);
    len -= remain;
    cep = gasnetc_bind_cep(epid, sreq, op, len);
    for (seg = 0; seg < sr_desc->num_sge; ++seg) {
      /* Xlate index to actual lkey */
      sr_desc->sg_list[seg].lkey = GASNETC_SEG_LKEY(cep, base+seg);
    }
  } else {
    const firehose_request_t *fh_loc = gasnetc_fh_aligned_local_pin(loc_addr, len);
    int seg;
    for (seg = 0; fh_loc != NULL; ++seg) {
      const size_t count = MIN(remain, (fh_loc->addr + fh_loc->len - loc_addr));
      sreq->fh_ptr[seg] = fh_loc;
      sreq->fh_count = seg + 1;
      sr_desc->sg_list[seg].addr = loc_addr;
      sr_desc->sg_list[seg].length = count;
      loc_addr += count;
      remain -= count;
      if (!remain || seg == (GASNETC_SND_SG-1)) {
	break; /* End of xfer or sg list */
      }

      /* We hold a local firehose already, we can only 'try' or risk deadlock */
      fh_loc = firehose_try_local_pin(loc_addr, 1, NULL);
    }
    gasneti_assert(sreq->fh_count > 0);
    sr_desc->num_sge = sreq->fh_count;
    gasneti_assert(remain < len);
    len -= remain;
    cep = gasnetc_bind_cep(epid, sreq, op, len);
    for (seg = 0; seg < sr_desc->num_sge; ++seg) {
      /* Xlate to actual lkeys */
      sr_desc->sg_list[seg].lkey = GASNETC_FH_LKEY(cep, sreq->fh_ptr[seg]);
    }
  }

  sr_desc->wr.rdma.rkey = GASNETC_SEG_RKEY(cep, rkey_index);

  gasneti_assert(len > 0);
  return len;
}

/* Helper for rdma puts: inline send case */
GASNETI_INLINE(gasnetc_do_put_inline)
void gasnetc_do_put_inline(const gasnetc_epid_t epid, int rkey_index,
                                  struct ibv_send_wr *sr_desc,
                                  size_t nbytes,
                                  gasnetc_atomic_val_t *remote_cnt, gasnetc_cb_t remote_cb
				  GASNETI_THREAD_FARG) {
  gasnetc_cep_t *cep;
  gasnetc_sreq_t *sreq;

  GASNETI_TRACE_EVENT_VAL(C, RDMA_PUT_INLINE, nbytes);

  gasneti_assert(nbytes != 0);
  gasneti_assert(nbytes <= gasnetc_inline_limit);

  sreq = gasnetc_get_sreq(GASNETC_OP_PUT_INLINE GASNETI_THREAD_PASS);
  sreq->fh_count = 0;
  if (remote_cnt) {
    ++(*remote_cnt);
    sreq->comp.data = remote_cnt;
    sreq->comp.cb = remote_cb;
  }

  sr_desc->opcode      = IBV_WR_RDMA_WRITE;
  sr_desc->num_sge     = 1;
  sr_desc->sg_list[0].length = nbytes;

  cep = gasnetc_bind_cep(epid, sreq, IBV_WR_RDMA_WRITE, nbytes);
  sr_desc->wr.rdma.rkey = GASNETC_SEG_RKEY(cep, rkey_index);

  gasnetc_snd_post_inline(sreq, sr_desc);
  sr_desc->wr.rdma.remote_addr += nbytes;
  sr_desc->sg_list[0].addr += nbytes;
}
      
/* Helper for rdma puts: bounce buffer case */
GASNETI_INLINE(gasnetc_do_put_bounce)
void gasnetc_do_put_bounce(const gasnetc_epid_t epid, int rkey_index,
                                  struct ibv_send_wr *sr_desc,
                                  size_t nbytes,
                                  gasnetc_atomic_val_t *remote_cnt, gasnetc_cb_t remote_cb
				  GASNETI_THREAD_FARG) {
  uintptr_t src = sr_desc->sg_list[0].addr;
  GASNETI_TRACE_EVENT_VAL(C, RDMA_PUT_BOUNCE, nbytes);

  gasneti_assert(nbytes != 0);

  do {
    gasnetc_sreq_t * const sreq = gasnetc_get_sreq(GASNETC_OP_PUT_BOUNCE GASNETI_THREAD_PASS);
    const size_t count = MIN(GASNETC_BUFSZ, nbytes);

    sreq->bb_buff = gasnetc_get_bbuf(1 GASNETI_THREAD_PASS);
    memcpy(sreq->bb_buff, (void *)src, count);
    if (remote_cnt) {
      ++(*remote_cnt);
      sreq->comp.data = remote_cnt;
      sreq->comp.cb = remote_cb;
    }

    gasnetc_bounce_common(epid, rkey_index, sr_desc, count, sreq, IBV_WR_RDMA_WRITE GASNETI_THREAD_PASS);

    src += count;
    nbytes -= count;
  } while (nbytes);
  sr_desc->sg_list[0].addr = src;
}

/* Helper for rdma puts: zero copy case */
GASNETI_INLINE(gasnetc_do_put_zerocp)
void gasnetc_do_put_zerocp(const gasnetc_epid_t epid, int rkey_index,
                                  struct ibv_send_wr *sr_desc,
                                  size_t nbytes,
                                  gasnetc_atomic_val_t *cnt, gasnetc_cb_t cb
				  GASNETI_THREAD_FARG) {
  GASNETI_TRACE_EVENT_VAL(C, RDMA_PUT_ZEROCP, nbytes);

  gasneti_assert(nbytes != 0);

  /* loop over local pinned regions */
  do {
    gasnetc_sreq_t * const sreq = gasnetc_get_sreq(GASNETC_OP_PUT_ZEROCP GASNETI_THREAD_PASS);
    size_t count = gasnetc_zerocp_common(epid, rkey_index, sr_desc, nbytes, sreq,
                                         IBV_WR_RDMA_WRITE GASNETI_THREAD_PASS);
    gasneti_assert(count <= nbytes);
    nbytes -= count;

    if (cnt) {
      ++(*cnt);
      sreq->comp.data = cnt;
      sreq->comp.cb = cb;
    }

    gasnetc_snd_post(sreq, sr_desc);
    sr_desc->wr.rdma.remote_addr += count;
    sr_desc->sg_list[0].addr += count;
  } while (nbytes);
}

/* Helper for rdma gets: bounce buffer case */
GASNETI_INLINE(gasnetc_do_get_bounce)
void gasnetc_do_get_bounce(const gasnetc_epid_t epid, int rkey_index,
                                  struct ibv_send_wr *sr_desc,
                                  size_t nbytes,
                                  gasnetc_atomic_val_t *remote_cnt, gasnetc_cb_t remote_cb
				  GASNETI_THREAD_FARG) {
#if GASNETC_FH_OPTIONAL /* Only reachable if Firehose has been disabled */
  uintptr_t dst = sr_desc->sg_list[0].addr;
  GASNETI_TRACE_EVENT_VAL(C, RDMA_GET_BOUNCE, nbytes);

  gasneti_assert(nbytes != 0);
  gasneti_assert(remote_cnt != NULL);

  do {
    gasnetc_sreq_t * const sreq = gasnetc_get_sreq(GASNETC_OP_GET_BOUNCE GASNETI_THREAD_PASS);
    const size_t count = MIN(GASNETC_BUFSZ, nbytes);

    nbytes -= count;
    if (nbytes) ++(*remote_cnt); // Do NOT advance prior to the last injection

    sreq->bb_addr  = (void *)dst;
    sreq->bb_len   = count;
    sreq->bb_buff  = gasnetc_get_bbuf(1 GASNETI_THREAD_PASS);
    sreq->comp.cb = remote_cb;
    sreq->comp.data = remote_cnt;

    gasnetc_bounce_common(epid, rkey_index, sr_desc, count, sreq, IBV_WR_RDMA_READ GASNETI_THREAD_PASS);

    dst += count;
  } while (nbytes);
  sr_desc->sg_list[0].addr = dst;
#else
  gasneti_fatalerror("unreachble call to gasnetc_do_get_bounce()");
#endif
}

/* Helper for rdma gets: zero copy case */
GASNETI_INLINE(gasnetc_do_get_zerocp)
void gasnetc_do_get_zerocp(const gasnetc_epid_t epid, int rkey_index,
                                  struct ibv_send_wr *sr_desc,
                                  size_t nbytes,
                                  gasnetc_atomic_val_t *remote_cnt, gasnetc_cb_t remote_cb
				  GASNETI_THREAD_FARG) {
  GASNETI_TRACE_EVENT_VAL(C, RDMA_GET_ZEROCP, nbytes);

  gasneti_assert(nbytes != 0);
  gasneti_assert(remote_cnt != NULL);

  /* loop over local pinned regions */
  do {
    gasnetc_sreq_t * const sreq = gasnetc_get_sreq(GASNETC_OP_GET_ZEROCP GASNETI_THREAD_PASS);
    size_t count = gasnetc_zerocp_common(epid, rkey_index, sr_desc, nbytes, sreq,
                                         IBV_WR_RDMA_READ GASNETI_THREAD_PASS);
    gasneti_assert(count <= nbytes);

    nbytes -= count;
    if (nbytes) ++(*remote_cnt); // Do NOT advance prior to the last injection

    sreq->comp.cb = remote_cb;
    sreq->comp.data = remote_cnt;

    gasnetc_snd_post(sreq, sr_desc);
    sr_desc->wr.rdma.remote_addr += count;
    sr_desc->sg_list[0].addr += count;
  } while (nbytes);
}

#else /* !GASNETC_PIN_SEGMENT */
/*
 * ###################################################################
 * Static helper functions for RDMA when the segment is NOT pre-pinned
 * ###################################################################
 */
GASNETI_INLINE(gasnetc_fh_put_inline)
void gasnetc_fh_put_inline(gasnetc_sreq_t *sreq GASNETI_THREAD_FARG) {
  GASNETC_DECL_SR_DESC(sr_desc, 1);
  const firehose_request_t * const fh_rem = sreq->fh_ptr[0];
  size_t len = sreq->fh_len;
  gasnetc_atomic_val_t *lc;
  gasnetc_cb_t lc_cb;
  gasnetc_cep_t *cep;

  gasneti_assert(fh_rem != NULL);
  gasneti_assert(sreq->fh_rem_addr >= fh_rem->addr);
  gasneti_assert(sreq->fh_rem_addr + (len - 1) <= fh_rem->addr + (fh_rem->len - 1));

  sr_desc->opcode      = IBV_WR_RDMA_WRITE;
  sr_desc->num_sge     = 1;
  sr_desc->wr.rdma.remote_addr = sreq->fh_rem_addr;
  sr_desc->sg_list[0].addr = sreq->fh_loc_addr;
  sr_desc->sg_list[0].length = len;

  lc_cb = sreq->fh_lc_cb;
  lc = sreq->fh_lc;

  cep = gasnetc_bind_cep(sreq->epid, sreq, IBV_WR_RDMA_WRITE, len);
  sr_desc->wr.rdma.rkey = GASNETC_FH_RKEY(cep, fh_rem);

  gasnetc_snd_post_inline(sreq, sr_desc);

  if_pf (lc_cb) lc_cb(lc); /* locally complete */
}

GASNETI_INLINE(gasnetc_fh_put_bounce)
void gasnetc_fh_put_bounce(gasnetc_sreq_t *orig_sreq GASNETI_THREAD_FARG) {
  GASNETC_DECL_SR_DESC(sr_desc, 1);
  const firehose_request_t * const fh_rem = orig_sreq->fh_ptr[0];
  gasnetc_epid_t epid = orig_sreq->epid;
  size_t nbytes = orig_sreq->fh_len;
  gasnetc_cep_t *cep;
  uintptr_t src = orig_sreq->fh_loc_addr;
  uintptr_t dst = orig_sreq->fh_rem_addr;

  gasneti_assert(nbytes != 0);
  gasneti_assert(orig_sreq->fh_rem_addr >= fh_rem->addr);
  gasneti_assert(orig_sreq->fh_rem_addr + (nbytes - 1) <= fh_rem->addr + (fh_rem->len - 1));

  /* Use full bounce buffers until just one buffer worth of data remains */
  while (nbytes > GASNETC_BUFSZ) {
    gasnetc_sreq_t * const sreq = gasnetc_get_sreq(GASNETC_OP_PUT_BOUNCE GASNETI_THREAD_PASS);
    sreq->fh_bbuf = gasnetc_get_bbuf(1 GASNETI_THREAD_PASS);
    memcpy(sreq->fh_bbuf, (void *)src, GASNETC_BUFSZ);
    sreq->fh_count = 0;

    sr_desc->opcode      = IBV_WR_RDMA_WRITE;
    sr_desc->num_sge     = 1;
    sr_desc->wr.rdma.remote_addr = dst;
    sr_desc->sg_list[0].addr = (uintptr_t)sreq->fh_bbuf;
    sr_desc->sg_list[0].length  = GASNETC_BUFSZ;

    cep = gasnetc_bind_cep(epid, sreq, IBV_WR_RDMA_WRITE, GASNETC_BUFSZ);
    sr_desc->wr.rdma.rkey = GASNETC_FH_RKEY(cep, fh_rem);
    sr_desc->sg_list[0].lkey = GASNETC_SND_LKEY(cep);

    /* Send all ops on same qp to get point-to-point ordering for proper fh_release() */
    epid = sreq->epid;

    gasnetc_snd_post(sreq, sr_desc);

    src += GASNETC_BUFSZ;
    dst += GASNETC_BUFSZ;
    nbytes -= GASNETC_BUFSZ;
  }

  /* Send out the last buffer w/ the original resource */
  gasneti_assert(nbytes <= GASNETC_BUFSZ);

  orig_sreq->fh_bbuf = gasnetc_get_bbuf(1 GASNETI_THREAD_PASS);
  memcpy(orig_sreq->fh_bbuf, (void *)src, nbytes);
  if_pf (orig_sreq->fh_lc_cb) orig_sreq->fh_lc_cb(orig_sreq->fh_lc); /* locally complete */

  sr_desc->opcode      = IBV_WR_RDMA_WRITE;
  sr_desc->num_sge     = 1;
  sr_desc->wr.rdma.remote_addr = dst;
  sr_desc->sg_list[0].addr = (uintptr_t)orig_sreq->fh_bbuf;
  sr_desc->sg_list[0].length  = nbytes;

  cep = gasnetc_bind_cep(epid, orig_sreq, IBV_WR_RDMA_WRITE, nbytes);
  sr_desc->wr.rdma.rkey = GASNETC_FH_RKEY(cep, fh_rem);
  sr_desc->sg_list[0].lkey = GASNETC_SND_LKEY(cep);

  gasnetc_snd_post(orig_sreq, sr_desc);
}

GASNETI_INLINE(gasnetc_fh_post)
void gasnetc_fh_post(gasnetc_sreq_t *sreq, enum ibv_wr_opcode op GASNETI_THREAD_FARG) {
  GASNETC_DECL_SR_DESC(sr_desc, GASNETC_SND_SG);
  struct ibv_sge *sg_entry;
  gasnetc_cep_t *cep;
  uintptr_t loc_addr;
  size_t remain;
  int i;

  gasneti_assert(sreq->fh_count >= 2);
  gasneti_assert(sreq->fh_count <= GASNETC_MAX_FH);
  gasneti_assert(sreq->fh_ptr[0] != NULL);
  gasneti_assert(sreq->fh_ptr[1] != NULL);

  sr_desc->opcode = op;
  sr_desc->wr.rdma.remote_addr = sreq->fh_rem_addr;
  sr_desc->num_sge = sreq->fh_count - 1;

  remain = sreq->fh_len;
  loc_addr = sreq->fh_loc_addr;
  sg_entry = sr_desc->sg_list;

  cep = gasnetc_bind_cep(sreq->epid, sreq, op, sreq->fh_len);
  sr_desc->wr.rdma.rkey = GASNETC_FH_RKEY(cep, sreq->fh_ptr[0]);

  for (i = 1; i < sreq->fh_count; ++i) {
    const firehose_request_t *fh_req = sreq->fh_ptr[i];
    uintptr_t next = fh_req->addr + fh_req->len;
    size_t nbytes = MIN(remain, (next - loc_addr));

    gasneti_assert(loc_addr < next);
    gasneti_assert(remain > 0);
    gasneti_assert(nbytes > 0);

    sg_entry->addr = loc_addr;
    sg_entry->length = nbytes;
    sg_entry->lkey = GASNETC_FH_LKEY(cep, fh_req);

    ++sg_entry;
    remain -= nbytes;
    loc_addr += nbytes;
  }
  gasneti_assert(remain == 0);

  gasnetc_snd_post(sreq, sr_desc);
}

static void gasnetc_fh_do_put(gasnetc_sreq_t *sreq GASNETI_THREAD_FARG) {
  gasnetc_counter_t * const am_oust = sreq->fh_oust;

  switch (sreq->opcode) {
    case GASNETC_OP_PUT_INMOVE:
      /* All done in the AM.  Complete the sreq here since snd_reap will never see it. */
      gasneti_assert(sreq->fh_len == 0);
      if (sreq->comp.cb != NULL) {
        sreq->comp.cb(sreq->comp.data);
      }
      gasneti_assert(sreq->fh_count > 0);
      firehose_release(sreq->fh_ptr, sreq->fh_count);
      sreq->opcode = GASNETC_OP_FREE;
      break;

    case GASNETC_OP_PUT_INLINE:
      gasneti_assert(sreq->fh_len > 0);
      GASNETI_TRACE_EVENT_VAL(C, RDMA_PUT_INLINE, sreq->fh_len);
      gasnetc_fh_put_inline(sreq GASNETI_THREAD_PASS);
      break;

    case GASNETC_OP_PUT_BOUNCE:
      gasneti_assert(sreq->fh_len > 0);
      GASNETI_TRACE_EVENT_VAL(C, RDMA_PUT_BOUNCE, sreq->fh_len);
      gasnetc_fh_put_bounce(sreq GASNETI_THREAD_PASS);
      break;

    case GASNETC_OP_PUT_ZEROCP:
      gasneti_assert(sreq->fh_len > 0);
      GASNETI_TRACE_EVENT_VAL(C, RDMA_PUT_ZEROCP, sreq->fh_len);
      gasnetc_fh_post(sreq, IBV_WR_RDMA_WRITE GASNETI_THREAD_PASS);
      break;

    default: gasneti_unreachable_error(("Invalid opcode in gasnetc_fh_do_put sreq: 0x%x",(int)sreq->opcode));
  }

  gasnetc_counter_dec_if_pf(am_oust);
}

#define gasnetc_sreq_is_ready(sreq) \
  gasnetc_atomic_decrement_and_test(&((sreq)->fh_ready), GASNETI_ATOMIC_REL)

static void gasnetc_fh_put_cb(void *context, const firehose_request_t *fh_rem, int allLocalHit) {
  gasnetc_sreq_t *sreq = context;

  gasneti_assert(fh_rem != NULL);
  sreq->fh_ptr[0] = fh_rem;

  if (gasnetc_sreq_is_ready(sreq)) {
    GASNET_BEGIN_FUNCTION(); // TODO: THREAD_FARG for firehose callbacks?
    gasnetc_fh_do_put(sreq GASNETI_THREAD_PASS);
  }
}

static void gasnetc_fh_do_get(gasnetc_sreq_t *sreq GASNETI_THREAD_FARG) {
  GASNETI_TRACE_EVENT_VAL(C, RDMA_GET_ZEROCP, sreq->fh_len);
  gasnetc_fh_post(sreq, IBV_WR_RDMA_READ GASNETI_THREAD_PASS);
}

static void gasnetc_fh_get_cb(void *context, const firehose_request_t *fh_rem, int allLocalHit) {
  gasnetc_sreq_t *sreq = context;

  sreq->fh_ptr[0] = fh_rem;

  if (gasnetc_sreq_is_ready(sreq)) {
    GASNET_BEGIN_FUNCTION(); // TODO: THREAD_FARG for firehose callbacks?
    gasnetc_fh_do_get(sreq GASNETI_THREAD_PASS);
  }

  gasneti_assert(sreq->fh_oust == NULL);
}

GASNETI_INLINE(gasnetc_get_local_fh)
size_t gasnetc_get_local_fh(gasnetc_sreq_t *sreq, uintptr_t loc_addr, size_t len) {
  size_t remain;
  int i;

  gasneti_assert(len != 0);

  for (i = 1, remain = len; (remain && (i < GASNETC_MAX_FH)); ++i) {
    const firehose_request_t *fh_loc = firehose_try_local_pin(loc_addr, 1, NULL);
    if (!fh_loc) {
      break;
    } else {
      size_t nbytes = MIN(remain, (fh_loc->addr + fh_loc->len - loc_addr));
      sreq->fh_ptr[i] = fh_loc;
      remain -= nbytes;
      loc_addr += nbytes;
    }
  }
  if (i > 1) {
    sreq->fh_count = i;
    len -= remain;
  } else {
    const firehose_request_t *fh_loc = gasnetc_fh_aligned_local_pin(loc_addr, len);
    len = MIN(remain, (fh_loc->addr + fh_loc->len - loc_addr));
    sreq->fh_ptr[1] = fh_loc;
    sreq->fh_count = 2;
  }

  return len;
}

static size_t gasnetc_fh_put_args_fn(void * context, firehose_remotecallback_args_t *args) {
    gasnetc_sreq_t *sreq = context;
    const size_t len = MIN(gasnetc_putinmove_limit, sreq->fh_len);

    args->addr = (void *)(sreq->fh_rem_addr);
    sreq->fh_putinmove = args->len = len;
    memcpy(args->data, (void *)(sreq->fh_loc_addr), len);

    return gasneti_offsetof(firehose_remotecallback_args_t, data[len]);
}

GASNETI_INLINE(gasnetc_fh_put_helper)
size_t gasnetc_fh_put_helper(
                gasnetc_epid_t epid,
                gasnetc_sreq_t *sreq,
                gasnetc_atomic_val_t *local_cnt,
                gasnetc_atomic_val_t *remote_cnt,
                uintptr_t loc_addr,
                uintptr_t rem_addr,
                size_t len
                GASNETI_THREAD_FARG) {
  const gex_Rank_t node = gasnetc_epid2node(epid);
  const firehose_request_t *fh_rem;
  size_t putinmove = sreq->fh_putinmove = 0;

  sreq->fh_rem_addr = rem_addr;
  sreq->fh_loc_addr = loc_addr;

  /* See how much (if any) is already pinned.  A call to firehose_partial_remote_pin()
   * might acquire a firehose for a region starting above rem_addr.  By instead calling
   * firehose_try_remote_pin() with len==1, we get a *contiguous* firehose if available.
   * We count on the implementation of firehose region giving out the largest region
   * that covers our request.
   */
  fh_rem = firehose_try_remote_pin(node, rem_addr, 1, 0, NULL);

  if_pt (fh_rem != NULL) {
    /* HIT in remote firehose table - some initial part of the region is pinned */
    sreq->fh_ptr[0] = fh_rem;
    gasneti_assert(rem_addr >= fh_rem->addr);
    gasneti_assert(rem_addr <= (fh_rem->addr + fh_rem->len - 1));
    len = sreq->fh_len = MIN(len, (fh_rem->addr + fh_rem->len - rem_addr));
    sreq->fh_oust = NULL; /* No asynchrony on a HIT */
  } else {
    /* MISS: Some initial part (or all) of the region is unpinned */
    uint32_t flags = 0;
    firehose_remotecallback_args_fn_t args_fn = NULL;
    gasnetc_atomic_set(&sreq->fh_ready, 2, 0);
    len = sreq->fh_len = gasnetc_fh_aligned_len(rem_addr, len);
    if (len <= gasnetc_putinmove_limit_adjusted) {
      /* Put-in-move optimization used only if the entire xfer can be
       * piggybacked, or if the remainder fits in an inline.
       */
      flags = FIREHOSE_FLAG_ENABLE_REMOTE_CALLBACK;
      args_fn = &gasnetc_fh_put_args_fn;
    }
    (void)firehose_remote_pin(node, rem_addr, len, flags, NULL,
			      args_fn, &gasnetc_fh_put_cb, sreq);
    putinmove = sreq->fh_putinmove;
    if (putinmove) {
      GASNETI_TRACE_EVENT_VAL(C, RDMA_PUT_IN_MOVE, putinmove);
    }
    gasnetc_counter_inc_if(sreq->fh_oust);
  }

  /* If the original request can be completed entirely w/o use of
   * local firehoses then do so.  We don't worry about anything
   * done by the put-in-move optimization, under the assumption that
   * the original request len is representative of future requests.
   */
  if ((len <= gasnetc_inline_limit) ||
	((local_cnt != NULL) && (len <= gasnetc_bounce_limit))) {
    sreq->fh_count = 1; /* Just the remote one */
  } else {
    len = gasnetc_get_local_fh(sreq, loc_addr, len);
  }

  if_pf (len <= putinmove) {
    /* AM is carrying at least as much as we could pin locally */
    len = putinmove;
    sreq->fh_len = 0;
    sreq->opcode = GASNETC_OP_PUT_INMOVE;
    if (remote_cnt != NULL) {
	++(*remote_cnt);
    }
  } else {
    /* Adjust sreq for len (which may have been reduced for local alignment)
     * and for any data piggybacked on the AM (if any).
     */
    size_t nbytes = len - putinmove; 

    sreq->fh_len = nbytes;
    sreq->fh_rem_addr += putinmove;
    sreq->fh_loc_addr += putinmove;

    if (nbytes <= gasnetc_inline_limit) {
      /* Inline when small enough */
      sreq->opcode = GASNETC_OP_PUT_INLINE;
      if_pf (fh_rem == NULL) { /* Memory will be copied asynchronously */
	if (local_cnt) ++(*local_cnt);
      } else { /* Memory will be copied synchronously before return */
	sreq->fh_lc_cb = NULL;
      }
      if (remote_cnt != NULL) {
	++(*remote_cnt);
      }
    } else if ((nbytes <= gasnetc_bounce_limit) && (local_cnt != NULL)) {
      /* Bounce buffer use for non-bulk puts (upto a limit) */
      sreq->opcode = GASNETC_OP_PUT_BOUNCE;
      if_pf (fh_rem == NULL) { /* Memory will be copied asynchronously */
	++(*local_cnt);
      } else { /* Memory will be copied synchronously before return */
	sreq->fh_lc_cb = NULL;
      }
      if (remote_cnt != NULL) {
	++(*remote_cnt);
      }
    } else {
      /* Use the local firehose(s) obtained earlier */
      sreq->opcode = GASNETC_OP_PUT_ZEROCP;
      /* The init or the sync (or neither) might wait on completion, but never both */
      if (local_cnt != NULL) {
	++(*local_cnt);
	sreq->comp.data = local_cnt;
	sreq->comp.cb = sreq->fh_lc_cb;
      } else if (remote_cnt != NULL) {
	++(*remote_cnt);
      }
    }
  }
  gasneti_assert(sreq->opcode != GASNETC_OP_INVALID);

  if ((fh_rem != NULL) || gasnetc_sreq_is_ready(sreq)) {
    gasnetc_fh_do_put(sreq GASNETI_THREAD_PASS);
  }

  gasneti_assert(len >= putinmove);
  gasneti_assert(len > 0);
  return len;
}

GASNETI_INLINE(gasnetc_fh_get_helper)
size_t gasnetc_fh_get_helper(gasnetc_epid_t epid, gasnetc_sreq_t *sreq,
                             uintptr_t loc_addr, uintptr_t rem_addr,
                             size_t len, gasnetc_atomic_val_t *remote_cnt
                             GASNETI_THREAD_FARG) {
  const size_t orig_len = len;
  const gex_Rank_t node = gasnetc_epid2node(epid);
  const firehose_request_t *fh_rem;

  sreq->fh_rem_addr = rem_addr;
  sreq->fh_loc_addr = loc_addr;

  /* See how much (if any) is already pinned.  A call to firehose_partial_remote_pin()
   * might acquire a firehose for a region starting above rem_addr.  By instead calling
   * firehose_try_remote_pin() with len==1, we get a *contiguous* firehose if available.
   * We count on the implementation of firehose region giving out the largest region
   * that covers our request.
   */
  fh_rem = firehose_try_remote_pin(node, rem_addr, 1, 0, NULL);

  if_pt (fh_rem != NULL) {
    /* HIT in remote firehose table - some initial part of the region is pinned */
    sreq->fh_ptr[0] = fh_rem;
    gasneti_assert(rem_addr >= fh_rem->addr);
    gasneti_assert(rem_addr <= (fh_rem->addr + fh_rem->len - 1));
    len = MIN(len, (fh_rem->addr + fh_rem->len - rem_addr));
  } else {
    /* MISS: Some initial part (or all) of the region is unpinned */
    gasnetc_atomic_set(&sreq->fh_ready, 2, 0);
    len = gasnetc_fh_aligned_len(rem_addr, len);
    (void)firehose_remote_pin(node, rem_addr, len, 0, NULL,
			      NULL, &gasnetc_fh_get_cb, sreq);
  }

  len = sreq->fh_len = gasnetc_get_local_fh(sreq, loc_addr, len);

  if (len != orig_len) ++(*remote_cnt); // Do NOT advance prior to the last injection

  if ((fh_rem != NULL) || gasnetc_sreq_is_ready(sreq)) {
    gasnetc_fh_do_get(sreq GASNETI_THREAD_PASS);
  }

  gasneti_assert(len > 0);
  return len;
}
#endif

/* ------------------------------------------------------------------------------------ *
 *  Externally visible functions                                                        *
 * ------------------------------------------------------------------------------------ */

/*
 * Check/compute limits before allocating anything
 */
extern int gasnetc_sndrcv_limits(void) {
  gasnetc_hca_t		*hca;
  int 			h;
  const int 		rcv_spare = (gasnetc_use_rcv_thread ? 1 : 0);
  const int 		rcv_spares = gasnetc_num_hcas * rcv_spare;

  gasnetc_remote_nodes = gasneti_nodes - (GASNET_PSHM ? gasneti_nodemap_local_count : 1);

  /* Count normal qps to be placed on each HCA */
  if (gasneti_nodes == 1) {
    GASNETC_FOR_ALL_HCA(hca) {
      /* Avoid a later division by zero */
      hca->max_qps = 1;
      hca->qps = 1;
    }
  } else {
    /* XXX: this logic depends on the current gasnetc_select_port() logic;
     * in particular on the simple node-independent repetition of ports. */
    int i;
    for (i = 0; i < gasnetc_num_qps; ++i) {
      hca = &gasnetc_hca[gasnetc_port_tbl[i % gasnetc_num_ports].hca_index];
      hca->qps += 1;
      hca->max_qps += gasnetc_remote_nodes;
    }
  }

  /* Ops outstanding per peer and total: */
  if (gasnetc_op_oust_limit == 0) { /* 0 = automatic limit computation */
    gasnetc_op_oust_per_qp = INT_MAX;
    GASNETC_FOR_ALL_HCA_INDEX(h) {
      gasneti_atomic_val_t tmp = MIN(GASNETI_ATOMIC_MAX, gasnetc_hca[h].hca_cap.max_cqe);
      gasnetc_op_oust_per_qp = MIN(gasnetc_op_oust_per_qp, (tmp / gasnetc_hca[h].qps));
    }
  } else {
    gasnetc_op_oust_per_qp = MIN(GASNETI_ATOMIC_MAX, gasnetc_op_oust_limit) / gasnetc_num_qps;
    GASNETC_FOR_ALL_HCA(hca) {
      int tmp = hca->qps * gasnetc_op_oust_per_qp;
      if (tmp > hca->hca_cap.max_cqe) {
        GASNETI_RETURN_ERRR(RESOURCE, "GASNET_NETWORKDEPTH_{PP,TOTAL} exceed HCA capabilities");
      }
    }
  }
  gasnetc_op_oust_pp /= gasnetc_num_qps;
  gasnetc_op_oust_per_qp = MIN(gasnetc_op_oust_per_qp, gasnetc_op_oust_pp*(gasneti_nodes-1));
  gasnetc_op_oust_limit = gasnetc_num_qps * gasnetc_op_oust_per_qp;
  GASNETI_TRACE_PRINTF(I, ("Final/effective GASNET_NETWORKDEPTH_TOTAL = %d", gasnetc_op_oust_limit));

  /* AM recv buffer allocation.  There are 5 roles a rcv buffer might fill (counts per HCA):
   * (1) Either 0 or 1 for use by the AM rcv thread.
   * (2) Exactly (gasnetc_am_oust_pp * hca->max_qps) used to catch Requests
   * (3) Upto (gasnetc_am_oust_limit * hca->qps) used to catch Replies
   * (4) Upto (gasnetc_am_credits_slack * hca->max_qps) that are assocaited with coallesced
   *     credits and thus with Replies that did not occur.  These get recycled the next time a
   *     Request is sent on the corresponding QP, but are not free to move to any other QP.
   * (5) Free
   * For "accounting" (1) and (2) are exact requirements and (3), (4) and (5) are lumped
   * together, with the added requirement that (4) < (3) to avoid a deadlock in which
   * credit coallescing has tied up all the rcv buffers (bug 1418).
   * Thus we allocate (1) + (2) + (3) buffers (and CQ slots) and adjust gasnetc_am_credits_slack
   * as needed to ensure (4) < (3)
   * We also (silently) reduce gasnetc_am_oust_limit to account for the fact that Replies
   * can never out number Requests.
   *
   * Note that we use (gasneti_nodes - 1) rather than gasnetc_remote_nodes.  This is because
   * gasnetc_remote_nodes may vary among processes, possibly leading to making different
   * gasnet_use_srq decisions across nodes.
   */
  gasnetc_am_oust_pp /= gasnetc_num_qps;
  gasnetc_am_rqst_per_qp = gasnetc_am_oust_pp * (gasneti_nodes - 1);
  if (gasnetc_am_oust_limit == 0) {
    /* 0 = automatic limit computations.
     * Find the largest possible value of (3) within HCA limits */
    gasnetc_am_repl_per_qp = gasnetc_am_rqst_per_qp; /* Replies never exceed Requests */
    GASNETC_FOR_ALL_HCA(hca) {
      int tmp = hca->hca_cap.max_cqe	/* Total CQ space */
			- rcv_spare /* Rcv thread's spare */
			- (gasnetc_am_rqst_per_qp * hca->qps); /* To catch Requests */
      tmp /= hca->qps;
      gasnetc_am_repl_per_qp = MIN(gasnetc_am_repl_per_qp, tmp);
    }
  } else {
    gasnetc_am_repl_per_qp = MIN((gasnetc_am_oust_limit / gasnetc_num_qps), gasnetc_am_rqst_per_qp);
    if (!gasnetc_am_repl_per_qp) {
      gasnetc_am_repl_per_qp = 1;	/* Ensure at least 1 */
    }
    GASNETC_FOR_ALL_HCA(hca) {
      int tmp = hca->qps * (gasnetc_am_rqst_per_qp + gasnetc_am_repl_per_qp) + rcv_spare;
      if (tmp > hca->hca_cap.max_cqe) {
        GASNETI_RETURN_ERRR(RESOURCE, "GASNET_AM_CREDIT_{PP,TOTAL} exceed HCA capabilities");
      }
    }
  }
  gasnetc_am_oust_limit = gasnetc_num_qps * gasnetc_am_repl_per_qp;
  GASNETI_TRACE_PRINTF(I, ("Final/effective GASNET_AM_CREDITS_TOTAL = %d", gasnetc_am_oust_limit));

  if (gasnetc_remote_nodes > 0) {
    gasnetc_am_credits_slack = MIN(gasnetc_am_credits_slack, gasnetc_am_oust_pp - 1);
    GASNETC_FOR_ALL_HCA(hca) {
      /* Ensure credit coallescing can't deadlock a Request (bug 1418) */
      int limit = hca->qps * gasnetc_am_repl_per_qp - gasnetc_remote_nodes; /* might be negative */
      while (gasnetc_am_credits_slack && (gasnetc_am_credits_slack * hca->max_qps > limit)) {
	--gasnetc_am_credits_slack; /* easier to loop than get rounded arithmetic right */
      }
    }
  }
  gasnetc_am_credits_slack = MIN(gasnetc_am_credits_slack, 256);
  GASNETI_TRACE_PRINTF(I, ("Final/effective GASNET_AM_CREDITS_SLACK = %d", gasnetc_am_credits_slack));

  if (gasnetc_bbuf_limit == 0) { /* 0 = automatic limit computation */
    /* We effectively count local AMs against gasnetc_op_oust_limit for simplicity,
     * but only expect one in-flight per thread anyway. */
    gasnetc_bbuf_limit = gasnetc_op_oust_limit;
  } else {
    gasnetc_bbuf_limit = MIN(gasnetc_bbuf_limit, gasnetc_op_oust_limit);
  }
  if (gasnetc_remote_nodes == 0) {
    #if GASNET_PSHM
      /* PSHM will handle all of the loopback traffic */
    #else
      /* no AM or RDMA on the wire, but still need bufs for constructing AMs */
      gasnetc_bbuf_limit = gasnetc_num_qps * gasnetc_am_oust_pp;
    #endif
  }
  /* SRQ may raise this.  So, report is deferred. */

  gasnetc_am_rbufs_per_qp = gasnetc_am_rqst_per_qp + gasnetc_am_repl_per_qp;
#if GASNETC_IBV_SRQ
  /* TODO: the parameter GASNET_RBUF_LIMIT is *not* being applied as documented
   * in the README, and the reporting below confuses matters even more by
   * mixing the definitions.  So, we should fix one or the other so that they
   * actually match.  Fixing the code is probably preferable, since the
   * alternative definition (used by the current code) is hard to explain/use.
   */
  if (gasnetc_use_srq) {
    unsigned int srq_wr_per_qp = (gasnetc_rbuf_limit - rcv_spares) / gasnetc_num_qps;
    int orig = gasnetc_rbuf_limit;
    int tmp;

    /* Ensure each path has some reasonable miniumum.
     * Since this is not scaled w/ nodes it could safely be much larger than this.
     */
    const int min_wr_per_qp = 2;
    if (srq_wr_per_qp && (srq_wr_per_qp < min_wr_per_qp)) {
      srq_wr_per_qp = min_wr_per_qp;
      fprintf(stderr,
              "WARNING: Requested GASNET_RBUF_COUNT %d increased to %d\n",
              orig, gasnetc_num_qps * srq_wr_per_qp + rcv_spares);
    }

    /* Check against HCA limits */
    GASNETC_FOR_ALL_HCA(hca) {
      unsigned int tmp = hca->hca_cap.max_srq_wr / hca->qps;
      if (!srq_wr_per_qp || (tmp < srq_wr_per_qp)) {
        srq_wr_per_qp = tmp;
      }
    }
    gasnetc_rbuf_limit = gasnetc_num_qps * srq_wr_per_qp + rcv_spares;

    /* Warn only if reduced relative to an explicit  non-zero value */
    if (gasnetc_rbuf_set && orig && (gasnetc_rbuf_limit < orig)) {
      fprintf(stderr,
              "WARNING: Requested GASNET_RBUF_COUNT %d reduced by HCA's max_srq_wr to %d\n",
              orig, gasnetc_rbuf_limit);
    }

    /* As per README:
       GASNET_USE_SRQ < 0: Use SRQ only if memory savings would result
     */
    tmp = MIN(gasnetc_am_rqst_per_qp, srq_wr_per_qp) + gasnetc_am_repl_per_qp;
    gasneti_assert(gasnetc_rbuf_limit != 0);
    GASNETI_TRACE_PRINTF(I, ("Final/effective GASNET_RBUF_COUNT = %d (SRQ limit: %d, w/o SRQ: %d)",
                             tmp * gasnetc_num_qps + rcv_spares,
                             gasnetc_rbuf_limit,
                             gasnetc_am_rbufs_per_qp * gasnetc_num_qps + rcv_spares));
    if ((gasnetc_use_srq < 0) && (tmp == gasnetc_am_rbufs_per_qp)) {
      GASNETI_TRACE_PRINTF(I, ("SRQ disabled because GASNET_USE_SRQ = -1 and no buffer savings would result"));
      gasnetc_use_srq = 0;
    } else {
      GASNETI_TRACE_PRINTF(I, ("SRQ enabled"));
      gasnetc_am_rqst_per_qp = tmp - gasnetc_am_repl_per_qp;
      gasnetc_am_rbufs_per_qp = tmp;
      gasnetc_use_srq = 1;
      gasnetc_am_credits_slack = 0;
      /* Need to ensure some BBUFs avail even if max number of AM Requests are all blocked */
      gasnetc_bbuf_limit = MAX(gasnetc_bbuf_limit, MIN(64, gasnetc_op_oust_limit) + gasnetc_am_oust_limit);
    }
  } else {
    GASNETI_TRACE_PRINTF(I, ("Final/effective GASNET_RBUF_COUNT = %d", gasnetc_am_rbufs_per_qp * gasnetc_num_qps + rcv_spares));
    GASNETI_TRACE_PRINTF(I, ("SRQ disabled"));
    gasnetc_use_srq = 0;
  }
  /* gasnetc_use_srq is just 0 or 1 from here on */
#else
  GASNETI_TRACE_PRINTF(I, ("Final/effective GASNET_RBUF_COUNT = %d", gasnetc_am_rbufs_per_qp * gasnetc_num_qps + rcv_spares));
#endif
  GASNETI_TRACE_PRINTF(I, ("Final/effective GASNET_BBUF_COUNT = %d", gasnetc_bbuf_limit));
  gasnetc_am_credits_slack_orig = gasnetc_am_credits_slack;

  gasnetc_alloc_qps = gasnetc_num_qps; /* Default w/o SRQ or XRC */
#if GASNETC_IBV_SRQ
  if (gasnetc_use_srq) {
    gasnetc_alloc_qps = 2 * gasnetc_num_qps;
    GASNETC_FOR_ALL_HCA(hca) {
      hca->max_qps *= 2;
    }
  }
 #if GASNETC_IBV_XRC
  else if (gasnetc_use_xrc) {
    /* No SRQ means no XRC either */
    gasnetc_use_xrc = 0;
  }
  GASNETI_TRACE_PRINTF(I, ("XRC %sabled", gasnetc_use_xrc ? "en" : "dis"));
 #endif
#endif

  /* sanity/bounds checks */
  GASNETC_FOR_ALL_HCA(hca) {
    const unsigned int max_qp = hca->hca_cap.max_qp;
    const unsigned int max_qp_wr = hca->hca_cap.max_qp_wr;

    if_pf (hca->max_qps > max_qp) {
      GASNETI_RETURN_ERRR(RESOURCE, "job size exceeds HCA capabilities");
    }
    if_pf (gasnetc_am_oust_pp * 2 > max_qp_wr) {
      GASNETI_RETURN_ERRR(RESOURCE, "GASNET_AM_CREDITS_PP exceeds HCA capabilities");
    }
    if_pf (gasnetc_op_oust_pp > max_qp_wr) {
      GASNETI_RETURN_ERRR(RESOURCE, "GASNET_NETWORKDEPTH_PP exceeds HCA capabilities");
    }
  }

  return GASNET_OK;
}

#if GASNETC_IBV_SRQ
static struct ibv_srq * gasnetc_create_srq(gasnetc_hca_t *hca, const int max_wr) {
  struct ibv_srq *result = NULL;
  #if GASNETC_IBV_XRC_OFED
    if (gasnetc_use_xrc) {
      struct ibv_srq_init_attr_ex attr;
      memset(&attr, 0, sizeof(struct ibv_srq_init_attr_ex));
      attr.attr.max_wr = max_wr;
      attr.attr.max_sge = 1;
      attr.comp_mask = IBV_SRQ_INIT_ATTR_TYPE | IBV_SRQ_INIT_ATTR_XRCD |
                       IBV_SRQ_INIT_ATTR_CQ | IBV_SRQ_INIT_ATTR_PD;
      attr.srq_type = IBV_SRQT_XRC;
      attr.xrcd = hca->xrc_domain;
      attr.cq = hca->rcv_cq;
      attr.pd = hca->pd;
      result = ibv_create_srq_ex(hca->handle, &attr);
    } else
  #elif GASNETC_IBV_XRC_MLNX
    if (gasnetc_use_xrc) {
      struct ibv_srq_init_attr attr;
      memset(&attr, 0, sizeof(attr));
      attr.attr.max_wr = max_wr;
      attr.attr.max_sge = 1;
      result = ibv_create_xrc_srq(hca->pd, hca->xrc_domain, hca->rcv_cq, &attr);
    } else
  #endif
    {
      struct ibv_srq_init_attr attr;
      memset(&attr, 0, sizeof(attr));
      attr.attr.max_wr = max_wr;
      attr.attr.max_sge = 1;
      result = ibv_create_srq(hca->pd, &attr);
    }
    return result;
}
#endif

extern int gasnetc_sndrcv_init(gasnetc_EP_t ep) {
  gasnetc_hca_t		*hca;
  int	act_size;
  int			vstat;
  gasnetc_buffer_t	*buf;
  gasnetc_rbuf_t	*rbuf;
  int 			padded_size, h, i;
  const int 		rcv_spare = (gasnetc_use_rcv_thread ? 1 : 0);
  size_t		size;
  int			ud_rcvs = 0;

#if GASNETC_DYNAMIC_CONNECT
  /* Default to handling 4 + 2*lg(remote_nodes) incomming UD requests and 4 outgoing */
  gasnetc_ud_rcvs = 1;
  while ((1 << gasnetc_ud_rcvs) < (int)gasnetc_remote_nodes) {
    ++gasnetc_ud_rcvs;
  }
  gasnetc_ud_rcvs = 4 + 2 * gasnetc_ud_rcvs;
  gasnetc_ud_snds = 4;
  gasnetc_ud_rcvs = gasneti_getenv_int_withdefault("GASNET_CONNECT_RCVS", gasnetc_ud_rcvs, 0);
  gasnetc_ud_snds = gasneti_getenv_int_withdefault("GASNET_CONNECT_SNDS", gasnetc_ud_snds, 0);
  GASNETI_TRACE_PRINTF(I, ("Buffers for dynamic connections: rcv=%d snd=%d",
                            gasnetc_ud_rcvs, gasnetc_ud_snds));
  #if !GASNETC_USE_CONN_THREAD
  ud_rcvs = gasnetc_ud_rcvs;
  #endif
#endif

  /*
   * setup RCV resources
   */

  /* create one RCV CQ per HCA */
  GASNETC_FOR_ALL_HCA(hca) {
    const int rcv_count = hca->qps * gasnetc_am_rbufs_per_qp + rcv_spare;
    const int cqe_count = rcv_count + (!hca->hca_index ? ud_rcvs : 0);
    gasnetc_progress_thread_t *rcv_thread = NULL;
  #if GASNETC_USE_RCV_THREAD
    if (gasnetc_use_rcv_thread) rcv_thread = &hca->rcv_thread;
  #endif
    vstat = gasnetc_create_cq(hca->handle, cqe_count, &hca->rcv_cq, &act_size, rcv_thread);
    GASNETC_IBV_CHECK(vstat, "from gasnetc_create_cq(rcv_cq)");
    GASNETI_TRACE_PRINTF(I, ("Recv CQ length: requested=%d actual=%d", (int)cqe_count, (int)act_size));
    gasneti_assert(act_size >= cqe_count);
    /* We don't set rcv_count = act_size here, as that could nearly double the memory allocated below */

    gasnetc_lifo_init(&hca->amrdma_freelist);

    if (gasnetc_remote_nodes) {
      /* Allocated pinned memory for receive buffers */
      size = GASNETI_PAGE_ALIGNUP(rcv_count * sizeof(gasnetc_buffer_t));
      buf = gasnetc_mmap(size);
      if_pf (buf == GASNETC_MMAP_FAILED) {
        buf = NULL;
      } else {
        vstat = gasnetc_pin(hca, buf, size, IBV_ACCESS_LOCAL_WRITE, &hca->rcv_reg);
        if (vstat != 0) {
	  gasnetc_munmap(buf, size);
          buf = NULL;
        }
      }
      if_pf (buf == NULL) {
        GASNETI_RETURN_ERRR(RESOURCE, "Unable to allocate pinned memory for AM recv buffers");
      }

#if GASNETC_IBV_SRQ
      /* create two shared recv queues per HCA */
      if (gasnetc_use_srq) {
        const int rqst_count = hca->qps * gasnetc_am_rqst_per_qp;
        const int repl_count = hca->qps * gasnetc_am_repl_per_qp;

        hca->rqst_srq = gasnetc_create_srq(hca, rqst_count);
        GASNETC_IBV_CHECK_PTR(hca->rqst_srq, "from gasnetc_create_srq(Request)");

        hca->repl_srq = gasnetc_create_srq(hca, repl_count);
        GASNETC_IBV_CHECK_PTR(hca->repl_srq, "from gasnetc_create_srq(Reply)");

        gasnetc_sema_init(&hca->am_sema, rqst_count, rqst_count);
      }
#endif
  
      /* Allocated normal memory for receive descriptors (rbuf's) */
      padded_size = GASNETI_ALIGNUP(sizeof(gasnetc_rbuf_t), GASNETI_CACHE_LINE_BYTES);
      if (NULL == hca->rbufs) {
        hca->rbufs = gasneti_malloc_aligned(GASNETI_CACHE_LINE_BYTES, rcv_count*padded_size);
        gasneti_leak_aligned(hca->rbufs);
      }
  
      /* Initialize the rbuf's */
      gasnetc_lifo_init(&hca->rbuf_freelist);
      rbuf = hca->rbufs;
      for (i = 0; i < rcv_count; ++i) {
        rbuf->rr_is_rdma         = 0;
        rbuf->rr_desc.num_sge    = 1;
        rbuf->rr_desc.sg_list    = &rbuf->rr_sg;
        rbuf->rr_desc.wr_id      = (uintptr_t)rbuf;	/* CQE will point back to this request */
        rbuf->rr_desc.next       = NULL;
        rbuf->rr_sg.length       = GASNETC_BUFSZ;
        rbuf->rr_sg.addr         = (uintptr_t)&buf[i];
        rbuf->rr_ep              = ep;
        gasnetc_lifo_push(&hca->rbuf_freelist, rbuf);
  
        rbuf = (gasnetc_rbuf_t *)((uintptr_t)rbuf + padded_size);
      }
#if GASNETC_USE_RCV_THREAD
      if (gasnetc_use_rcv_thread) {
        hca->rcv_thread_priv = gasnetc_lifo_pop(&hca->rbuf_freelist);
        gasneti_assert(hca->rcv_thread_priv != NULL);
      }
#endif
      
      /* Initialize resources for AM-over-RDMA */
      if (NULL == hca->cep) {
        hca->cep = gasneti_calloc(hca->max_qps, sizeof(gasnetc_cep_t *));
        gasneti_leak(hca->cep);
      }
      gasnetc_atomic_set(&hca->amrdma_rcv.count, 0, 0);
      hca->amrdma_rcv.prev = 0;
      if (gasnetc_amrdma_max_peers && hca->max_qps) {
	const int max_peers = hca->amrdma_rcv.max_peers = MIN(gasnetc_amrdma_max_peers, hca->max_qps);
	size_t alloc_size = GASNETI_PAGE_ALIGNUP(max_peers * (gasnetc_amrdma_depth << GASNETC_AMRDMA_SZ_LG2) + GASNETC_AMRDMA_PAD);
	void *buf = gasnetc_mmap(alloc_size);

        if_pf (buf == GASNETC_MMAP_FAILED) {
          buf = NULL;
        } else {
          vstat = gasnetc_pin(hca, buf, alloc_size, (enum ibv_access_flags)(IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE), &hca->amrdma_reg);
          if (vstat != 0) {
	    gasnetc_munmap(buf, size);
            buf = NULL;
          }
        }
        if_pf (buf == NULL) {
	  /* XXX: unwind here? */
	  gasneti_fatalerror("Unable to allocate pinned memory for AM-over-RDMA");
        }
	buf = (void *)((uintptr_t)buf + GASNETC_AMRDMA_PAD);
	for (i = 0; i < max_peers; ++i) {
	  gasnetc_lifo_push(&hca->amrdma_freelist, buf);
	  buf = (void *)((uintptr_t)buf + (gasnetc_amrdma_depth << GASNETC_AMRDMA_SZ_LG2));
	}

        if (NULL == hca->amrdma_rcv.cep) {
          hca->amrdma_rcv.cep = gasneti_calloc(max_peers, sizeof(gasnetc_cep_t *));
          gasneti_leak(hca->amrdma_rcv.cep);
        }

        gasnetc_atomic_set(&hca->amrdma_balance.count, 0, 0);
        hca->amrdma_balance.mask = gasnetc_amrdma_cycle ? (gasnetc_amrdma_cycle - 1) : 0;
        gasnetc_atomic_set(&hca->amrdma_balance.state, GASNETC_AMRDMA_STATE_RUN, 0);
        hca->amrdma_balance.floor = 1;
        if (NULL == hca->amrdma_balance.table) {
          hca->amrdma_balance.table = gasneti_calloc(hca->max_qps, sizeof(gasnetc_amrdma_balance_tbl_t));
          gasneti_leak(hca->amrdma_balance.table);
        }
      }
    }
  }

  /*
   * setup SND resources
   */

  /* create the SND CQ and associated semaphores */
  if (NULL == gasnetc_cq_semas) {
    gasnetc_cq_semas = (gasnetc_sema_t *)
	  gasneti_malloc_aligned(GASNETI_CACHE_LINE_BYTES, gasnetc_num_hcas*sizeof(gasnetc_sema_t));
    gasneti_leak_aligned(gasnetc_cq_semas);
  }
  gasnetc_op_oust_per_qp = MAX(1, gasnetc_op_oust_per_qp); /* Avoid error in single-node case */
  GASNETC_FOR_ALL_HCA(hca) {
    const int rqst_count = gasnetc_use_srq ? gasnetc_am_repl_per_qp : 0;
    const int cqe_count = hca->qps * (gasnetc_op_oust_per_qp + rqst_count);
    vstat = gasnetc_create_cq(hca->handle, cqe_count, &hca->snd_cq, &act_size, NULL);
    GASNETC_IBV_CHECK(vstat, "from gasnetc_create_cq(snd_cq)");
    GASNETI_TRACE_PRINTF(I, ("Send CQ length: requested=%d actual=%d", (int)cqe_count, (int)act_size));
    gasneti_assert(act_size >= cqe_count);
    /* We use actual size here, since the memory has been allocated anyway */
    gasnetc_sema_init(&gasnetc_cq_semas[hca->hca_index], act_size, act_size);
    hca->snd_cq_sema_p = &gasnetc_cq_semas[hca->hca_index];
  }

  /* Allocated pinned memory for AMs and bounce buffers
   * TODO: Can/should we *USE* any extra allocated due to rounding-up? */
 if (gasnetc_bbuf_limit) {
  size = GASNETI_PAGE_ALIGNUP(gasnetc_bbuf_limit * sizeof(gasnetc_buffer_t));
  buf = gasnetc_mmap(size);
  if_pf (buf == GASNETC_MMAP_FAILED) {
    buf = NULL;
  } else {
    GASNETC_FOR_ALL_HCA_INDEX(h) {
      vstat = gasnetc_pin(&gasnetc_hca[h], buf, size,
		          IBV_ACCESS_LOCAL_WRITE, &gasnetc_hca[h].snd_reg);
      if (vstat != 0) {
	for (h -= 1; h >= 0; --h) {
	  gasnetc_unpin(&gasnetc_hca[h], &gasnetc_hca[h].snd_reg);
	}
        gasnetc_munmap(buf, size);
        buf = NULL;
	break;
      }
    }
  }
  if_pf (buf == NULL) {
      GASNETI_RETURN_ERRR(RESOURCE, "Unable to allocate pinned memory for AM/bounce buffers");
  }
  gasnetc_lifo_init(&gasnetc_bbuf_freelist);
  for (i = 0; i < gasnetc_bbuf_limit; ++i) {
    gasnetc_lifo_push(&gasnetc_bbuf_freelist, buf);
    ++buf;
  }
 }

  /* Init thread-local data */
  gasnetc_per_thread_setup();

  return GASNET_OK;
}

extern void gasnetc_sndrcv_init_peer(gex_Rank_t node, gasnetc_cep_t *cep) {
  int i, j;

  if (!GASNETI_NBRHD_JOBRANK_IS_LOCAL(node)) {
    for (i = 0; i < gasnetc_alloc_qps; ++i, ++cep) {
      gasnetc_hca_t *hca = cep->hca;
      cep->epid = gasnetc_epid(node, i);
      cep->rbuf_freelist = &hca->rbuf_freelist;

    #if GASNETC_IB_MAX_HCAS > 1
      /* "Cache" the local keys associated w/ this cep */
      cep->rcv_lkey = hca->rcv_reg.handle->lkey;
      cep->snd_lkey = hca->snd_reg.handle->lkey;
    #endif

      /* Initialize local AM-over-RDMA info */
      gasnetc_atomic_set(&cep->amrdma_eligable, 0, 0);
      cep->amrdma_send = NULL;
      cep->amrdma_recv = NULL;
      hca->cep[hca->num_qps++] = cep;
      gasneti_assert(hca->num_qps <= hca->max_qps);

      if (gasnetc_use_srq) {
        /* Prepost to SRQ for exactly one peer (after which the lifo is empty) */
        gasnetc_rbuf_t *rbuf = gasnetc_lifo_pop(cep->rbuf_freelist);
        if (rbuf) {
          int count = (i < gasnetc_num_qps) ? gasnetc_am_repl_per_qp : gasnetc_am_rqst_per_qp;
          for (j = 1; j < count; ++j) {
            gasnetc_rcv_post(cep, rbuf);
            rbuf = gasnetc_lifo_pop(cep->rbuf_freelist);
          }
          gasnetc_rcv_post(cep, rbuf);
        }
      } else
      for (j = 0; j < gasnetc_am_oust_pp; ++j) {
        /* Prepost one rcv buffer for each possible incomming request */
        gasnetc_rcv_post(cep, gasnetc_lifo_pop(cep->rbuf_freelist));
      }

      /* Setup semaphores/counters */
      /* sq_sema now set when QP is created */
      if (gasnetc_use_srq && (i < gasnetc_num_qps)) {
        gasnetc_sema_init(&cep->am_rem, 0, 0);
        gasnetc_sema_init(&cep->am_loc, 0, 0);
      } else {
        gasnetc_sema_init(&cep->am_rem, gasnetc_am_oust_pp, gasnetc_am_oust_pp);
        gasnetc_sema_init(&cep->am_loc, 0, gasnetc_am_oust_pp);
      }
      gasnetc_atomic_set(&cep->am_flow.credit, 0, 0);
      gasnetc_atomic_set(&cep->am_flow.ack, 0, 0);
      cep->snd_cq_sema_p = &gasnetc_cq_semas[GASNETC_HCA_IDX(cep)];
    }
  } else {
    /* Should never use these for loopback or same supernode */
    /* XXX: is this now unreachable with new connect code? */
    for (i = 0; i < gasnetc_alloc_qps; ++i, ++cep) {
      cep->epid = gasnetc_epid(node, i);
    #if GASNETC_IBV_XRC
      gasneti_assert(GASNETC_CEP_SQ_SEMA(cep) == NULL);
    #else
      gasnetc_sema_init(GASNETC_CEP_SQ_SEMA(cep), 0, 0);
    #endif
      gasnetc_sema_init(&cep->am_rem, 0, 0);
      gasnetc_sema_init(&cep->am_loc, 0, 0);
      gasnetc_atomic_set(&cep->am_flow.credit, 0, 0);
      gasnetc_atomic_set(&cep->am_flow.ack, 0, 0);
      gasnetc_atomic_set(&cep->amrdma_eligable, 0, 0);
    }
  }
}

extern void gasnetc_sndrcv_init_inline(void) {
  gasnetc_am_inline_limit_sndrcv = MIN(gasnetc_inline_limit, sizeof(gasnetc_am_tmp_buf_t));
  gasnetc_am_inline_limit_rdma = MAX(GASNETC_AMRDMA_HDRSZ, gasnetc_am_inline_limit_sndrcv) - GASNETC_AMRDMA_HDRSZ;
#if !GASNETC_PIN_SEGMENT
  gasnetc_putinmove_limit_adjusted = gasnetc_putinmove_limit
	  				? (gasnetc_putinmove_limit + gasnetc_inline_limit)
					: 0;
#endif
}

extern void gasnetc_sndrcv_attach_peer(gex_Rank_t node, gasnetc_cep_t *cep) {
#if GASNETC_PIN_SEGMENT
  int i;

  for (i = 0; i < gasnetc_alloc_qps; ++i, ++cep) {
    gasnetc_hca_t *hca = cep->hca;
  #if GASNETC_IB_MAX_HCAS > 1
    cep->seg_lkeys = hca->seg_lkeys;
  #endif
    cep->rkeys   = &hca->rkeys[node * gasnetc_max_regs];
  }
#else
  /* Nothing currently needed */
#endif
}

#if GASNETC_IBV_SHUTDOWN
static void
gasnetc_unpin_unmap(gasnetc_hca_t *hca, gasnetc_memreg_t *reg) {
  if (reg->len) {
    gasnetc_unpin(hca, reg);
    gasnetc_unmap(reg);
  }
}

void gasnetc_sys_flush_reph(gex_Token_t token, gex_AM_Arg_t credits) {
  gasnetc_cep_t *cep = ((gasnetc_rbuf_t *)token)->cep;

  gasneti_assert(! gasnetc_use_srq); /* SRQ prohibits credit coallescing */
  gasneti_assert(0 != credits); /* Should not have sent otherwise */

  /* May reduce trafic by sending banked credits back sooner: */
  gasnetc_am_credits_slack = 0;

  if (--credits) { /* Since this is a Reply, one credit has already been posted */
    gasnetc_sema_up_n(&cep->am_loc, credits);
    gasnetc_sema_up_n(&cep->am_rem, credits);
  }
}

static int gasnetc_close_recvd[16]; /* Note 16-bit GASNET_MAXNODES */

void gasnetc_sys_close_reqh(gex_Token_t token) {
  gex_Rank_t peer = gasnetc_msgsource(token);
  int distance, shift;

  distance = (peer > gasneti_mynode) ? peer - gasneti_mynode
                                     : peer + (gasneti_nodes - gasneti_mynode);
  gasneti_assert(GASNETI_POWEROFTWO(distance));

  shift = 0;
  while ((1<<shift) != distance) ++shift;

  gasneti_assert(!gasnetc_close_recvd[shift]);
  gasnetc_close_recvd[shift] = 1;

  if (! GASNETI_NBRHD_JOBRANK_IS_LOCAL(peer)) {
    gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)token;
    gasneti_assert(rbuf->rbuf_needReply);
    rbuf->rbuf_needReply = 0; /* we are terminating flow control */
  }
}

extern void
gasnetc_sndrcv_quiesce(void) {
  GASNET_BEGIN_FUNCTION(); // OK - not a critical-path
  gasnetc_hca_t *hca;

  /* suspend credit coallescing (if any) and return any banked credits */
  if (! gasnetc_use_srq) {
    gex_Rank_t i;

    gasnetc_rbuf_t rbuf;
  #if GASNETI_THREADINFO_OPT
    rbuf.rbuf_threadinfo = GASNETI_MYTHREAD;
  #endif
  #if GASNETI_STATS_OR_TRACE
    gasneti_tick_t stamp = GASNETI_TICK_MIN; /* BLCR-TODO: stats should exclude this */
    rbuf.rr_sg.addr = (uintptr_t) &stamp;
  #endif

    gasnetc_am_credits_slack = 0;
    for (i = 0; i < gasneti_nodes; ++i) { /* Stagger to avoid hot-spotting */
      gex_Rank_t node = (i < gasneti_nodes - gasneti_mynode)
                               ? (gasneti_mynode + i)
                               : (gasneti_mynode - (gasneti_nodes - i));
      gasnetc_cep_t *cep = GASNETC_NODE2CEP(node);
      int qpi;
      if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(node) || !cep) continue;
      for (qpi = 0; qpi < gasnetc_alloc_qps; ++qpi, ++cep) {
        int cr = gasnetc_atomic_swap(&cep->am_flow.credit, 0, 0);
        if (!cr) continue;
        /* Since the banked credits count remote recv buffers we can send a "Phantom Reply".
         * This avoids soliciting a potentially unnecessary Reply.
         */
        int fake_hidx = gasneti_handleridx(gasnetc_sys_close_reqh);
        rbuf.cep = cep;
        rbuf.rbuf_needReply = 1;
      #if GASNET_DEBUG
        rbuf.rbuf_handlerRunning = 1;
      #endif
        rbuf.rbuf_flags = GASNETC_MSG_GENFLAGS(1, gasneti_Short, 0, fake_hidx, node);
        rbuf.rr_ep = gasnetc_ep0;
        gasnetc_ReplySysShort((gex_Token_t)&rbuf, NULL, gasneti_handleridx(gasnetc_sys_flush_reph), 1, cr);
      }
    }
  }

  /* drain in-flight AMs by allocating all of the AM credits */
  {
    gex_Rank_t node;
    for (node = 0; node < gasneti_nodes; ++node) {
      gasnetc_cep_t *cep = GASNETC_NODE2CEP(node);
      int qpi_offset = gasnetc_use_srq ? gasnetc_num_qps : 0;
      int qpi;
      if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(node) || !cep) continue;
      for (qpi = qpi_offset, cep += qpi_offset; qpi < gasnetc_alloc_qps; ++qpi, ++cep) {
        int remain = gasnetc_am_oust_pp;
        gasnetc_sema_t *sema = &cep->am_rem;
        while (0 != (remain -= gasnetc_sema_trydown_partial(sema, remain))) {
          GASNETI_WAITHOOK();
          gasnetc_poll_both();
        }
      }
    }
  }

  { /* Dissemination barrier via special AM Request hander which does NOT return credits */
    unsigned int shift, distance;
    for (shift = 0, distance = 1; distance < gasneti_nodes; ++shift, distance *= 2) {
      gex_Rank_t peer = (distance <= gasneti_mynode) ? gasneti_mynode - distance
                                                        : gasneti_mynode + (gasneti_nodes - distance);
      if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(peer)) {
        /* BLCR-TODO: this might be a problem between init and attach? */
        gex_AM_RequestShort0(gasneti_THUNK_TM, peer, gasneti_handleridx(gasnetc_sys_close_reqh), 0);
      } else {
        static gasnetc_counter_t dummy = GASNETC_COUNTER_INITIALIZER; /* So PFs don't run */
        const int qp_offset = gasnetc_use_srq ? gasnetc_num_qps : 0;
        gasnetc_cep_t *cep = gasnetc_get_cep(peer) + qp_offset;
        gasnetc_sema_up(&cep->am_rem);
        /* OK if some other AM Request gets in this gap; we'll block for the reply. */
        gasnetc_RequestSysShort(cep->epid, &dummy, gasneti_handleridx(gasnetc_sys_close_reqh), 0);
      }
      while (! gasnetc_close_recvd[shift]) {
        GASNETI_WAITHOOK();
        gasnetc_poll_both();
      }
      gasnetc_close_recvd[shift] = 0;
    }
  }

  /* complete ALL send/put/get by allocating all of the CQ slots */
  GASNETC_FOR_ALL_HCA(hca) {
    int remain = hca->snd_cq->cqe;
    gasnetc_sema_t *sema = hca->snd_cq_sema_p;

    while (0 != (remain -= gasnetc_sema_trydown_partial(sema, remain))) {
      GASNETI_WAITHOOK();
      gasnetc_poll_both();
    }
  }

  /* Resume credit coallescing (in any) */
  gasnetc_am_credits_slack = gasnetc_am_credits_slack_orig;
}

extern int gasnetc_sndrcv_shutdown(void) {
  gasnetc_hca_t *hca;
  int rc;

  GASNETC_FOR_ALL_HCA(hca) {
  #if GASNETC_IBV_SRQ
    if (gasnetc_use_srq && gasnetc_remote_nodes) {
      rc = ibv_destroy_srq(hca->rqst_srq);
      GASNETC_IBV_CHECK(rc, "from ibv_destroy_srq(request)");
      rc = ibv_destroy_srq(hca->repl_srq);
      GASNETC_IBV_CHECK(rc, "from ibv_destroy_srq(reply)");
    }
  #endif

    rc = ibv_destroy_cq(hca->rcv_cq);
    GASNETC_IBV_CHECK(rc, "from ibv_destroy_cq(rcv_cq)");
    rc = ibv_destroy_cq(hca->snd_cq);
    GASNETC_IBV_CHECK(rc, "from ibv_destroy_cq(snd_cq)");

  #if GASNETC_USE_RCV_THREAD
    if (gasnetc_use_rcv_thread) {
      rc = ibv_destroy_comp_channel(hca->rcv_thread.compl);
      GASNETC_IBV_CHECK(rc, "from ibv_destroy_comp_chanel(rcv_thread)");
    }
  #endif

    gasnetc_unpin_unmap(hca, &hca->snd_reg);
    gasnetc_unpin_unmap(hca, &hca->rcv_reg);
    gasnetc_unpin_unmap(hca, &hca->amrdma_reg);
  }

  return GASNET_OK;
}
#endif

#if GASNETC_USE_RCV_THREAD
extern void gasnetc_sndrcv_start_thread(void) {
  if (gasnetc_remote_nodes && gasnetc_use_rcv_thread) {
    int rcv_max_rate = gasneti_getenv_int_withdefault("GASNET_RCV_THREAD_RATE", 0, 0);
    gasnetc_hca_t *hca;

    GASNETC_FOR_ALL_HCA(hca) {
      /* spawn the RCV thread */
      hca->rcv_thread.fn = gasnetc_rcv_thread;
      hca->rcv_thread.fn_arg = hca;
      if (rcv_max_rate > 0) {
        hca->rcv_thread.min_ns = ((uint64_t)1E9) / rcv_max_rate;
      }
    #if GASNETI_THREADINFO_OPT
      hca->rcv_threadinfo = NULL;
    #endif
      gasnetc_spawn_progress_thread(&hca->rcv_thread);
    }
  }
}

extern void gasnetc_sndrcv_stop_thread(int block) {
  if (gasnetc_remote_nodes && gasnetc_use_rcv_thread) {
    gasnetc_hca_t *hca;

    GASNETC_FOR_ALL_HCA(hca) {
      /* stop the RCV thread if we have started it */
      if (hca->rcv_thread.fn == gasnetc_rcv_thread) {
        gasnetc_stop_progress_thread(&hca->rcv_thread, block);
      }
    }
  }
}
#endif

extern gasnetc_amrdma_send_t *gasnetc_amrdma_send_alloc(uint32_t rkey, void *addr) {
  gasnetc_amrdma_send_t *result = gasneti_malloc(sizeof(gasnetc_amrdma_send_t));
  gasneti_leak(result);

  gasnetc_atomic_set(&result->head, gasnetc_amrdma_depth, 0);
  gasnetc_atomic_set(&result->tail, 0, 0);
  result->rkey = rkey;
  result->addr = (uintptr_t)addr;
  gasneti_sync_writes();

  return result;
}

extern gasnetc_amrdma_recv_t *gasnetc_amrdma_recv_alloc(gasnetc_hca_t *hca) {
  gasnetc_amrdma_buf_t *addr = gasnetc_lifo_pop(&hca->amrdma_freelist);
  gasnetc_amrdma_recv_t *result = NULL;

  if (addr != NULL) {
    int i;

    result = gasneti_malloc(sizeof(gasnetc_amrdma_recv_t));
    gasneti_leak(result);
    gasnetc_atomic_set(&result->head, 0, 0);
    result->addr = addr;
  #if GASNETC_ANY_PAR
    gasneti_mutex_init(&result->ack_lock);
    result->ack_bits = 0;
    result->tail = 0;
  #endif

    for (i = 0; i < gasnetc_amrdma_depth; ++i) {
      gasnetc_amrdma_hdr_t *hdr = (gasnetc_amrdma_hdr_t *)addr[i];
      hdr->length       = hdr->zeros       = 0;
      hdr->length_again = hdr->zeros_again = ~0;
      gasneti_assert(gasneti_count0s(hdr, GASNETC_AMRDMA_SZ) == (GASNETC_AMRDMA_SZ - 4));
  #if GASNETC_ANY_PAR
      gasnetc_atomic_set(&result->busy[i].spinlock, 0, 0);
  #endif
    }
  }

  return result;
}

/* Just gasnetc_AMPoll w/o CHECKATTACH when !handler_context */
extern void gasnetc_sndrcv_poll(int handler_context) {
  GASNET_BEGIN_FUNCTION(); // OK - this is used only in init/exit paths
  gasnetc_do_poll(!handler_context, 1 GASNETI_THREAD_PASS);
}

extern void gasnetc_counter_wait_aux(gasnetc_counter_t *counter, int handler_context GASNETI_THREAD_FARG)
{
  const gasnetc_atomic_val_t initiated = (counter->initiated & GASNETI_ATOMIC_MAX);
  gasnetc_atomic_t * const completed = &counter->completed;

  if (handler_context) {
    do {
      /* must not poll rcv queue in hander context */
      GASNETI_WAITHOOK();
      gasnetc_poll_snd();
    } while (initiated != gasnetc_atomic_read(completed, 0));
  } else {
    do {
      GASNETI_WAITHOOK();
      gasnetc_poll_both();
      GASNETI_PROGRESSFNS_RUN();
    } while (initiated != gasnetc_atomic_read(completed, 0));
  }
}

#if GASNETC_PIN_SEGMENT
/*
 * ############################################
 * RDMA ops used when the segment is pre-pinned
 * ############################################
 */

/* Perform an RDMA put
 *
 * Uses inline if possible, bounce buffers if "small enough" and the caller is planning to wait
 * for local completion.  Otherwise zero-copy is used (with firehose if the source is not pre-pinned).
 * If firehose is disabled, then bounce buffers are used for unpinned sources.
 */
// TODO-EX:
//   Curently we may need to pre-increment (*local_cnt) and (*remote_cnt) in
//   order to prevent the counters from becoming balanced (as observed in the
//   corresponding callback) before the final ibv-level operation has been
//   injected.  That requires a coresponding atomic-add (via the completion
//   callback) of the completed counter at the end.  Ideally we would be able
//   to manage all the increments of the initiated counters (as done in
//   gasnetc_rdma_get()) such that premature counter balance would be
//   impossible.  HOWEVER, attempts to do so w/o significant rewrites have
//   failed so far.  The main issues are:
//     + One or both of the counters might not advance at all
//     + Identifying the *last* increment of local_cnt or remote_cnt
//   For certain cases we may be able to resolve both of those issues, notably
//   the cases in which only one of local_cnt or remote_cnt is non-NULL.
//
//   Currently we entirely avoid the "bias" of the counter(s) only for the
//   case of the inline put.
extern int gasnetc_rdma_put(
                gasnetc_epid_t epid,
                void *src_ptr, void *dst_ptr,
                size_t nbytes,
                gex_Flags_t flags,
                gasnetc_atomic_val_t *local_cnt,
                gasnetc_cb_t local_cb,
                gasnetc_atomic_val_t *remote_cnt,
                gasnetc_cb_t remote_cb
                GASNETI_THREAD_FARG) {
  GASNETC_DECL_SR_DESC(sr_desc, GASNETC_SND_SG);
  uintptr_t src = (uintptr_t)src_ptr;
  uintptr_t dst = (uintptr_t)dst_ptr;
#if GASNET_ALIGNED_SEGMENTS
  uintptr_t offset = dst - gasnetc_seg_start;
#else
  uintptr_t offset = dst - (uintptr_t)gasneti_seginfo[gasnetc_epid2node(epid)].addr;
#endif

  // TODO-EX:
  //     All uses of rem_auxseg are a temporary hack
  //     The idea is to allow a negative rkey_index which gets mapped later to aux_reg
  //     This will be replaced by general multi-registration support later
  const int rem_auxseg = gasneti_in_auxsegment(NULL/*tm*/, gasnetc_epid2node(epid), dst_ptr, nbytes);

  gasneti_assert(offset < gasneti_seginfo[gasnetc_epid2node(epid)].size || rem_auxseg);
  gasneti_assert(nbytes != 0);
  
  sr_desc->wr.rdma.remote_addr = dst;
  sr_desc_sg_lst[0].addr = src;

  /* Use a short-cut for sends that are short enough and target a single registration.
   *
   * Note that we do this based only on the size and alignment, without checking whether
   * the caller cares about local completion, or whether zero-copy is possible.
   */
  if ((nbytes <= gasnetc_inline_limit) && (nbytes <= gasnetc_seg_remain(offset) || rem_auxseg))
  {
    const int rkey_index = rem_auxseg?-1: gasnetc_seg_index(offset);
    gasnetc_do_put_inline(epid, rkey_index, sr_desc, nbytes, remote_cnt, remote_cb GASNETI_THREAD_PASS);
    return 0;
  }

  // May need to do a bit of extra work to prevent premature counter balance
  const int bias_remote_cnt = (remote_cb == gasnetc_cb_eop_put);
  if (bias_remote_cnt) ++(*remote_cnt);

  // Distinct cases depending on whether LC matters or not
  // TODO-EX: this may suggest 2 distinct functions are in order?
  if (local_cb) {
    const int bias_local_cnt  = (local_cb  == gasnetc_cb_eop_alc);
    if (bias_local_cnt) ++(*local_cnt);
    do {
      /* Loop over contiguous pinned regions on remote end */
      const int rkey_index = rem_auxseg?-1: gasnetc_seg_index(offset);
      const size_t rem = gasnetc_seg_remain(offset);
      const size_t count = rem_auxseg?nbytes: MIN(nbytes, rem);

      if ((count <= gasnetc_bounce_limit) ||
          (!GASNETC_USE_FIREHOSE && gasnetc_unpinned(sr_desc_sg_lst[0].addr) && !rem_auxseg)) {
        /* Because IB lacks any indication of "local" completion, the only ways to
         * implement non-bulk puts are as fully blocking puts, or with bounce buffers.
         * So, if a non-bulk put is "not too large" use bounce buffers.
         *   OR
         * Firehose disabled.  Must use bounce buffers when src is out-of-segment.
         */
        gasnetc_do_put_bounce(epid, rkey_index, sr_desc, count,
                              remote_cnt, remote_cb GASNETI_THREAD_PASS);
      } else {
        gasnetc_do_put_zerocp(epid, rkey_index, sr_desc, count,
                              local_cnt, local_cb GASNETI_THREAD_PASS);
      }

      offset += count;
      nbytes -= count;
    #if GASNET_DEBUG
      dst += count;
      gasneti_assert(sr_desc->wr.rdma.remote_addr == dst);
      src += count;
      gasneti_assert(sr_desc_sg_lst[0].addr == src);
    #endif
    } while (nbytes);
    if (bias_local_cnt) local_cb(local_cnt);
  } else {
    do {
      /* Loop over contiguous pinned regions on remote end */
      const int rkey_index = rem_auxseg?-1: gasnetc_seg_index(offset);
      const size_t rem = gasnetc_seg_remain(offset);
      const size_t count = rem_auxseg?nbytes: MIN(nbytes, rem);

      if (!GASNETC_USE_FIREHOSE && gasnetc_unpinned(sr_desc_sg_lst[0].addr) && !rem_auxseg) {
         // Firehose disabled.  Must use bounce buffers when src is out-of-segment.
        gasnetc_do_put_bounce(epid, rkey_index, sr_desc, count,
                              remote_cnt, remote_cb GASNETI_THREAD_PASS);
      } else {
        gasnetc_do_put_zerocp(epid, rkey_index, sr_desc, count,
                              remote_cnt, remote_cb GASNETI_THREAD_PASS);
      }

      offset += count;
      nbytes -= count;
    #if GASNET_DEBUG
      dst += count;
      gasneti_assert(sr_desc->wr.rdma.remote_addr == dst);
      src += count;
      gasneti_assert(sr_desc_sg_lst[0].addr == src);
    #endif
    } while (nbytes);
  }

  if (bias_remote_cnt) remote_cb(remote_cnt);

  return 0;
}

/* Perform an RDMA get
 *
 * Uses zero-copy (with firehose if the destination is not pre-pinned).
 * If firehose is disabled, then bounce buffers are used for unpinned destinations.
 *
 * This function takes care to keep its increments of (*remote_cnt) an *extra*
 * step ahead of the ibv-level injections.  This means it is not possible for
 * the initiated and completed counters to balance before to the last injection.
 */
extern int gasnetc_rdma_get(
                gasnetc_epid_t epid,
                void *src_ptr, void *dst_ptr,
                size_t nbytes,
                gex_Flags_t flags,
                gasnetc_atomic_val_t *remote_cnt,
                gasnetc_cb_t remote_cb
                GASNETI_THREAD_FARG) {
  GASNETC_DECL_SR_DESC(sr_desc, GASNETC_SND_SG);
  uintptr_t dst = (uintptr_t)dst_ptr;
  uintptr_t src = (uintptr_t)src_ptr;
#if GASNET_ALIGNED_SEGMENTS
  uintptr_t offset = src - gasnetc_seg_start;
#else
  uintptr_t offset = src - (uintptr_t)gasneti_seginfo[gasnetc_epid2node(epid)].addr;
#endif

  // TODO-EX:
  //     All uses of {loc,rem_}auxseg are a temporary hack
  //     The idea is to allow a negative [lr]key_index which gets mapped later to aux_reg
  //     This will be replaced by general multi-registration support later
  const int loc_auxseg = gasneti_in_auxsegment(NULL/*tm*/, gasneti_mynode, dst_ptr, nbytes);
  const int rem_auxseg = gasneti_in_auxsegment(NULL/*tm*/, gasnetc_epid2node(epid), src_ptr, nbytes);

  gasneti_assert(offset < gasneti_seginfo[gasnetc_epid2node(epid)].size || rem_auxseg);
  gasneti_assert(nbytes != 0);
  gasneti_assert(remote_cnt != NULL);

  ++(*remote_cnt);

  sr_desc->wr.rdma.remote_addr = src;
  sr_desc_sg_lst[0].addr = dst;
  do {
    /* Loop over contiguous pinned regions on remote end */
    const int rkey_index = rem_auxseg?-1: gasnetc_seg_index(offset);
    const size_t rem = gasnetc_seg_remain(offset);
    const size_t count = rem_auxseg?nbytes: MIN(nbytes, rem);
    nbytes -= count;

    if (nbytes) ++(*remote_cnt); // Do NOT advance prior to the last injection

    if (!GASNETC_USE_FIREHOSE && gasnetc_unpinned(sr_desc_sg_lst[0].addr) && !loc_auxseg) {
      /* Firehose disabled.  Use bounce buffers since dst is out-of-segment */
      gasnetc_do_get_bounce(epid, rkey_index, sr_desc, count, remote_cnt, remote_cb GASNETI_THREAD_PASS);
    } else
    {
      gasnetc_do_get_zerocp(epid, rkey_index, sr_desc, count, remote_cnt, remote_cb GASNETI_THREAD_PASS);
    }

    offset += count;
  #if GASNET_DEBUG
    src += count;
    gasneti_assert(sr_desc->wr.rdma.remote_addr == src);
    dst += count;
    gasneti_assert(sr_desc_sg_lst[0].addr == dst);
  #endif
  } while (nbytes);

  return 0;
}
#else
/*
 * ###########################################
 * RDMA ops when the segment is NOT pre-pinned
 * ###########################################
 */
/* RDMA put */
// TODO-EX: see comment prior to gasnetc_rdma_put() regarding counters
extern int gasnetc_rdma_put_fh(
                gasnetc_epid_t epid,
                void *src_ptr, void *dst_ptr,
                size_t nbytes,
                gex_Flags_t flags,
                gasnetc_atomic_val_t *local_cnt,
                gasnetc_cb_t local_cb,
                gasnetc_atomic_val_t *remote_cnt,
                gasnetc_cb_t remote_cb,
                gasnetc_counter_t *am_oust
                GASNETI_THREAD_FARG) {
  uintptr_t src = (uintptr_t)src_ptr;
  uintptr_t dst = (uintptr_t)dst_ptr;

  gasneti_assert(nbytes != 0);

  // May need to do a bit of extra work to prevent premature counter balance
  const int bias_local_cnt  = (local_cb  == gasnetc_cb_eop_alc);
  if (bias_local_cnt) ++(*local_cnt);
  const int bias_remote_cnt = (remote_cb == gasnetc_cb_eop_put);
  if (bias_remote_cnt) ++(*remote_cnt);

  do {
    gasnetc_sreq_t * const sreq = gasnetc_get_sreq(GASNETC_OP_INVALID GASNETI_THREAD_PASS);
    size_t count;

    sreq->epid = epid;
 
    sreq->comp.cb = remote_cb;
    sreq->comp.data = remote_cnt;
    sreq->fh_lc = local_cnt;
    sreq->fh_lc_cb = local_cb;
    sreq->fh_oust = am_oust;

    count = gasnetc_fh_put_helper(epid, sreq, local_cnt, remote_cnt, src, dst, nbytes GASNETI_THREAD_PASS);

    src += count;
    dst += count;
    nbytes -= count;
  } while (nbytes);

  // Fix the bias, if any, we introduced above
  if (bias_local_cnt)  local_cb(local_cnt);
  if (bias_remote_cnt) remote_cb(remote_cnt);

  gasnetc_poll_rcv(); /* Progress may depend on firehose AM Reply */
  return 0;
}

/* Perform an RDMA get
 *
 * This function takes care to keep its increments of (*remote_cnt) an *extra*
 * step ahead of the ibv-level injections.  This means it is not possible for
 * the initiated and completed counters to balance before to the last injection.
 * So, a caller does not need to take any additional "insurance".
 */
extern int gasnetc_rdma_get(
                gasnetc_epid_t epid,
                void *src_ptr, void *dst_ptr,
                size_t nbytes,
                gex_Flags_t flags,
                gasnetc_atomic_val_t *remote_cnt,
                gasnetc_cb_t remote_cb
                GASNETI_THREAD_FARG) {
  uintptr_t src = (uintptr_t)src_ptr;
  uintptr_t dst = (uintptr_t)dst_ptr;

  gasneti_assert(nbytes != 0);
  gasneti_assert(remote_cnt != NULL);

  ++(*remote_cnt);

  do {
    gasnetc_sreq_t * const sreq = gasnetc_get_sreq(GASNETC_OP_GET_ZEROCP GASNETI_THREAD_PASS);
    size_t count;

    sreq->epid = epid;
 
    sreq->comp.cb = remote_cb;
    sreq->comp.data = remote_cnt;

    count = gasnetc_fh_get_helper(epid, sreq, dst, src, nbytes, remote_cnt GASNETI_THREAD_PASS);

    src += count;
    dst += count;
    nbytes -= count;
  } while (nbytes);

  gasnetc_poll_rcv(); /* Progress may depend on firehose AM Reply */
  return 0;
}
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
*/

extern int gasnetc_RequestSysShort(gasnetc_epid_t dest,
                                 gasnetc_counter_t *counter,
                                 gex_AM_Index_t handler,
                                 int numargs, ...) {
  GASNET_BEGIN_FUNCTION();
  int retval;
  va_list argptr;

  const gex_Rank_t jobrank = gasnetc_epid2node(dest);
  GASNETI_TRACE_AMREQUESTSHORT(gasneti_THUNK_TM,jobrank,handler,/*flags*/0,numargs);

  /* ensure AM progress, but NOT progress functions */
  gasnetc_poll_rcv();

  gasnetc_EP_t ep = gasnetc_ep0;
  va_start(argptr, numargs);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    retval = gasnetc_nbrhd_RequestGeneric ( gasneti_Short, jobrank, handler,
                                            NULL, 0, NULL,
                                            0, numargs, argptr GASNETI_THREAD_PASS);
    if_pf (counter) gasnetc_atomic_increment(&counter->completed, 0);
  } else {
    retval = gasnetc_ReqRepGeneric(ep, gasneti_Short, NULL, dest, handler,
                                   NULL, 0, NULL, 0, numargs,
                                   &counter->initiated, gasnetc_cb_counter, counter,
                                   argptr GASNETI_THREAD_PASS);
  }
  va_end(argptr);
  return retval;
}

extern int gasnetc_RequestSysMedium(gasnetc_epid_t dest,
                                    gasnetc_counter_t *counter,
                                    gex_AM_Index_t handler,
                                    void *source_addr, size_t nbytes,
                                    int numargs, ...) {
  GASNET_BEGIN_FUNCTION();
  int retval;
  va_list argptr;

  const gex_Rank_t jobrank = gasnetc_epid2node(dest);
  GASNETI_TRACE_AMREQUESTMEDIUM(gasneti_THUNK_TM,jobrank,handler,source_addr,nbytes,/*flags*/0,numargs);

  /* ensure AM progress, but NOT progress functions */
  gasnetc_poll_rcv();

  gasnetc_EP_t ep = gasnetc_ep0;
  va_start(argptr, numargs);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    retval = gasnetc_nbrhd_RequestGeneric ( gasneti_Medium, jobrank, handler,
                                            source_addr, nbytes, NULL,
                                            0, numargs, argptr GASNETI_THREAD_PASS);
    if_pf (counter) gasnetc_atomic_increment(&counter->completed, 0);
  } else {
    retval = gasnetc_ReqRepGeneric(ep, gasneti_Medium, NULL, dest, handler,
                                   source_addr, nbytes, NULL, 0, numargs,
                                   &counter->initiated, gasnetc_cb_counter, counter,
                                   argptr GASNETI_THREAD_PASS);
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_ReplySysShort(gex_Token_t token,
                               gasnetc_counter_t *counter,
                               gex_AM_Index_t handler,
                               int numargs, ...) {
  int retval;
  va_list argptr;
  gasneti_assert(token);
  GASNETI_TRACE_AMREPLYSHORT(token,handler,/*flags*/0,numargs);
  va_start(argptr, numargs);
  if_pt (gasnetc_token_in_nbrhd(token)) {
    retval = gasnetc_nbrhd_ReplyGeneric ( gasneti_Short, token, handler,
                                          NULL, 0, NULL,
                                          0, numargs, argptr);
    if_pf (counter) gasnetc_atomic_increment(&counter->completed, 0);
  } else {
    gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)token;
    GASNET_POST_THREADINFO(rbuf->rbuf_threadinfo);
    gasnetc_EP_t ep = rbuf->rr_ep;
    gasneti_assert(ep == gasnetc_ep0);
    retval = gasnetc_ReqRepGeneric(ep, gasneti_Short, rbuf, GASNETC_MSG_SRCIDX(rbuf->rbuf_flags), handler,
                                   NULL, 0, NULL, 0, numargs,
                                   &counter->initiated, gasnetc_cb_counter, counter,
                                   argptr GASNETI_THREAD_PASS);
  }
  va_end(argptr);
  return retval;
}

extern int gasnetc_ReplySysMedium(gex_Token_t token,
                                  gasnetc_counter_t *counter,
                                  gex_AM_Index_t handler,
                                  void *source_addr, size_t nbytes,
                                  int numargs, ...) {
  int retval;
  va_list argptr;
  gasneti_assert(token);
  GASNETI_TRACE_AMREPLYMEDIUM(token,handler,source_addr,nbytes,/*flags*/0,numargs); 
  va_start(argptr, numargs);
  if_pt (gasnetc_token_in_nbrhd(token)) {
    retval = gasnetc_nbrhd_ReplyGeneric ( gasneti_Medium, token, handler,
                                          source_addr, nbytes, NULL,
                                          0, numargs, argptr);
    if_pf (counter) gasnetc_atomic_increment(&counter->completed, 0);
  } else {
    gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)token;
    GASNET_POST_THREADINFO(rbuf->rbuf_threadinfo);
    gasnetc_EP_t ep = rbuf->rr_ep;
    gasneti_assert(ep == gasnetc_ep0);
    retval = gasnetc_ReqRepGeneric(ep, gasneti_Medium, rbuf, GASNETC_MSG_SRCIDX(rbuf->rbuf_flags), handler,
                                   source_addr, nbytes, NULL, 0, numargs,
                                   &counter->initiated, gasnetc_cb_counter, counter,
                                   argptr GASNETI_THREAD_PASS);
  }
  va_end(argptr);
  return retval;
}

/* ------------------------------------------------------------------------------------ */
// AM "cores" - no poll or progress fns

GASNETI_INLINE(gasnetc_AMRequestShort)
int gasnetc_AMRequestShort( gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  int retval;
  gasneti_assert(tm);
  gasnetc_EP_t ep = (gasnetc_EP_t)gasneti_import_ep(gex_TM_QueryEP(tm));
  gasneti_assert(ep == gasnetc_ep0);
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    retval = gasnetc_nbrhd_RequestGeneric ( gasneti_Short, jobrank, handler,
                                            NULL, 0, NULL,
                                            0, numargs, argptr GASNETI_THREAD_PASS);
  } else {
    retval = gasnetc_ReqRepGeneric(ep, gasneti_Short, NULL, jobrank, handler,
                                   NULL, 0, NULL,
                                   flags, numargs, NULL, NULL, NULL,
                                   argptr GASNETI_THREAD_PASS);
  }
  return (retval == GASNETC_FAIL_IMM);
}

GASNETI_INLINE(gasnetc_AMRequestMedium)
int gasnetc_AMRequestMedium(gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes, 
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  int retval;
  gasneti_assert(tm);
  gasnetc_EP_t ep = (gasnetc_EP_t)gasneti_import_ep(gex_TM_QueryEP(tm));
  gasneti_assert(ep == gasnetc_ep0);
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    gasneti_leaf_finish(lc_opt); // Always synchronous local completion
    retval = gasnetc_nbrhd_RequestGeneric ( gasneti_Medium, jobrank, handler,
                                            source_addr, nbytes, NULL,
                                            flags, numargs, argptr GASNETI_THREAD_PASS);
  } else {
    gasnetc_counter_t    counter = GASNETC_COUNTER_INITIALIZER;
    gasnetc_atomic_val_t *local_cnt, start_cnt;
    gasnetc_cb_t         local_cb;
    gasnete_eop_t        *eop = NULL;

    if (gasneti_leaf_is_pointer(lc_opt)) {
      eop = _gasnete_eop_new(GASNETI_MYTHREAD);
      *lc_opt = (gex_Event_t)eop;
      GASNETE_EOP_LC_START(eop);
      start_cnt = eop->initiated_alc;
      local_cnt = &eop->initiated_alc;
      local_cb = gasnetc_cb_eop_alc;
    } else if (lc_opt == GEX_EVENT_NOW) {
      local_cnt = &counter.initiated;
      local_cb = gasnetc_cb_counter;
    } else if (lc_opt == GEX_EVENT_GROUP) {
      gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
      gasnete_iop_t *op = mythread->current_iop;
      local_cnt = &op->initiated_alc_cnt;
      local_cb = op->next ? gasnetc_cb_nar_alc : gasnetc_cb_iop_alc;
    } else {
      gasneti_fatalerror("Invalid lc_opt argument to RequestMedium");
    }

    retval = gasnetc_ReqRepGeneric(ep, gasneti_Medium, NULL, jobrank, handler,
                                   source_addr, nbytes, NULL,
                                   flags, numargs, local_cnt, local_cb, NULL,
                                   argptr GASNETI_THREAD_PASS);

    if (eop && (start_cnt == eop->initiated_alc)) {
      // Synchronous LC - reset LC state and pass-back INVALID_HANDLE as result
      GASNETE_EOP_LC_FINISH(eop);
      *lc_opt = GEX_EVENT_INVALID;
      gasnete_eop_free(eop GASNETI_THREAD_PASS);
    } else if (lc_opt == GEX_EVENT_NOW) {
#if 0 // Currently always synchronous LC when (local_cb == gasnetc_cb_counter)
      /* block for local completion of payload transfer */
      gasnetc_counter_wait(&counter, 0 GASNETI_THREAD_PASS);
#else
      gasneti_assert(counter.initiated == 0);
      gasneti_assert(gasnetc_atomic_read(&counter.completed,0) == 0);
#endif
    }
  }
  return (retval == GASNETC_FAIL_IMM);
}

GASNETI_INLINE(gasnetc_AMRequestLong)
int gasnetc_AMRequestLong(  gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes, void *dest_addr,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  int retval;
  gasneti_assert(tm);
  gasnetc_EP_t ep = (gasnetc_EP_t)gasneti_import_ep(gex_TM_QueryEP(tm));
  gasneti_assert(ep == gasnetc_ep0);
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    gasneti_leaf_finish(lc_opt); // Always synchronous local completion
    retval = gasnetc_nbrhd_RequestGeneric ( gasneti_Long, jobrank, handler,
                                            source_addr, nbytes, dest_addr,
                                            flags, numargs, argptr GASNETI_THREAD_PASS);
  } else {
    gasnetc_counter_t    counter = GASNETC_COUNTER_INITIALIZER;
    gasnetc_atomic_val_t *local_cnt, start_cnt;
    gasnetc_cb_t         local_cb;
    gasnete_eop_t        *eop = NULL;

    if (gasneti_leaf_is_pointer(lc_opt)) {
      if (flags & GASNETI_FLAG_LC_OPT_IN) {
        gasnete_op_t *op = gasneti_event_op(*lc_opt);
        if (OPTYPE(op) == OPTYPE_IMPLICIT) {
          gasnete_iop_t *iop = (gasnete_iop_t*)op;
          gasneti_assert(gasneti_event_idx(*lc_opt) == gasnete_iop_event_alc);
          gasneti_assert(iop->next); // Within an NBI access region
          local_cnt = &iop->initiated_alc_cnt;
          local_cb = gasnetc_cb_nar_alc;
        } else {
          eop = (gasnete_eop_t*)op;
          gasneti_assert(gasneti_event_idx(*lc_opt) == gasnete_eop_event_alc);
          GASNETE_EOP_LC_START(eop);
          start_cnt = eop->initiated_alc;
          local_cnt = &eop->initiated_alc;
          local_cb = gasnetc_cb_eop_alc;
        }
      } else {
        eop = _gasnete_eop_new(GASNETI_MYTHREAD);
        *lc_opt = (gex_Event_t)eop;
        GASNETE_EOP_LC_START(eop);
        start_cnt = eop->initiated_alc;
        local_cnt = &eop->initiated_alc;
        local_cb = gasnetc_cb_eop_alc;
      }
    } else if (lc_opt == GEX_EVENT_NOW) {
      local_cnt = &counter.initiated;
      local_cb = gasnetc_cb_counter;
    } else if (lc_opt == GEX_EVENT_GROUP) {
      gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
      gasnete_iop_t *op = mythread->current_iop;
      local_cnt = &op->initiated_alc_cnt;
      local_cb = op->next ? gasnetc_cb_nar_alc : gasnetc_cb_iop_alc;
    } else {
      gasneti_fatalerror("Invalid lc_opt argument to RequestLong");
    }

    retval = gasnetc_ReqRepGeneric(ep, gasneti_Long, NULL, jobrank, handler,
                                   source_addr, nbytes, dest_addr,
                                   flags, numargs, local_cnt, local_cb, NULL,
                                   argptr GASNETI_THREAD_PASS);

    if (lc_opt == GEX_EVENT_NOW) {
      /* block for local completion of RDMA transfer */
      gasnetc_counter_wait(&counter, 0 GASNETI_THREAD_PASS);
    } else if (eop && (start_cnt == eop->initiated_alc)) {
      // Synchronous LC - reset LC state and pass-back INVALID_HANDLE as result
      GASNETE_EOP_LC_FINISH(eop);
      *lc_opt = GEX_EVENT_INVALID;
      if (!(flags & GASNETI_FLAG_LC_OPT_IN)) {
        gasnete_eop_free(eop GASNETI_THREAD_PASS);
      }
    }
  }
  return (retval == GASNETC_FAIL_IMM);
}

GASNETI_INLINE(gasnetc_AMReplyShort)
int gasnetc_AMReplyShort(   gex_Token_t token, gex_AM_Index_t handler,
                            gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  int retval;
  gasneti_assert(token);
  if_pt (gasnetc_token_in_nbrhd(token)) {
    retval = gasnetc_nbrhd_ReplyGeneric ( gasneti_Short, token, handler,
                                          NULL, 0, NULL,
                                          flags, numargs, argptr);
  } else {
    gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)token;
    GASNET_POST_THREADINFO(rbuf->rbuf_threadinfo);
    gasnetc_EP_t ep = rbuf->rr_ep;
    gasneti_assert(ep == gasnetc_ep0);

    retval = gasnetc_ReqRepGeneric(ep, gasneti_Short, rbuf, GASNETC_MSG_SRCIDX(rbuf->rbuf_flags), handler,
                                   NULL, 0, NULL, 
                                   flags, numargs, NULL, NULL,
                                   NULL, argptr GASNETI_THREAD_PASS);
    gasneti_assert(!rbuf->rbuf_needReply);
  }
  return (retval == GASNETC_FAIL_IMM);
}

GASNETI_INLINE(gasnetc_AMReplyMedium)
int gasnetc_AMReplyMedium(  gex_Token_t token, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  int retval;
  gasneti_assert(token);
  if_pt (gasnetc_token_in_nbrhd(token)) {
    gasneti_leaf_finish(lc_opt); // Always synchronous local completion
    retval = gasnetc_nbrhd_ReplyGeneric ( gasneti_Medium, token, handler,
                                          source_addr, nbytes, NULL,
                                          flags, numargs, argptr);
  } else {
    gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)token;
    GASNET_POST_THREADINFO(rbuf->rbuf_threadinfo);
    gasnetc_EP_t ep = rbuf->rr_ep;
    gasneti_assert(ep == gasnetc_ep0);

    gasnetc_counter_t    counter = GASNETC_COUNTER_INITIALIZER;
    gasnetc_atomic_val_t *local_cnt, start_cnt;
    gasnetc_cb_t         local_cb;
    gasnete_eop_t        *eop = NULL;

    if (gasneti_leaf_is_pointer(lc_opt)) {
      eop = _gasnete_eop_new(GASNETI_MYTHREAD);
      *lc_opt = (gex_Event_t)eop;
      GASNETE_EOP_LC_START(eop);
      start_cnt = eop->initiated_alc;
      local_cnt = &eop->initiated_alc;
      local_cb = gasnetc_cb_eop_alc;
    } else if (lc_opt == GEX_EVENT_NOW) {
      local_cnt = &counter.initiated;
      local_cb = gasnetc_cb_counter;
    } else {
      gasneti_fatalerror("Invalid lc_opt argument to ReplyMedium");
    }
  
    retval = gasnetc_ReqRepGeneric(ep, gasneti_Medium, rbuf, GASNETC_MSG_SRCIDX(rbuf->rbuf_flags), handler,
                                   source_addr, nbytes, NULL,
                                   flags, numargs, local_cnt, local_cb, NULL,
                                   argptr GASNETI_THREAD_PASS);
    gasneti_assert(!rbuf->rbuf_needReply || (flags & GEX_FLAG_IMMEDIATE));

    if (eop && (start_cnt == eop->initiated_alc)) {
      // Synchronous LC - reset LC state and pass-back INVALID_HANDLE as result
      GASNETE_EOP_LC_FINISH(eop);
      *lc_opt = GEX_EVENT_INVALID;
      gasnete_eop_free(eop GASNETI_THREAD_PASS);
    } else if (lc_opt == GEX_EVENT_NOW) {
    #if 0 // Currently always synchronous LC when (local_cb == gasnetc_cb_counter)
      /* block for local completion of payload transfer */
      gasnetc_counter_wait(&counter, 1 /* handler context */ GASNETI_THREAD_PASS);
    #else
      gasneti_assert(counter.initiated == 0);
      gasneti_assert(gasnetc_atomic_read(&counter.completed,0) == 0);
    #endif
    }
  }
  return (retval == GASNETC_FAIL_IMM);
}

GASNETI_INLINE(gasnetc_AMReplyLong)
int gasnetc_AMReplyLong(    gex_Token_t token, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes, void *dest_addr,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  int retval;
  gasneti_assert(token);
  if_pt (gasnetc_token_in_nbrhd(token)) {
    gasneti_leaf_finish(lc_opt); // Always synchronous local completion
    retval = gasnetc_nbrhd_ReplyGeneric ( gasneti_Long, token, handler,
                                          source_addr, nbytes, dest_addr,
                                          flags, numargs, argptr);
  } else {
    gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)token;
    GASNET_POST_THREADINFO(rbuf->rbuf_threadinfo);
    gasnetc_EP_t ep = rbuf->rr_ep;
    gasneti_assert(ep == gasnetc_ep0);

  #if GASNETC_PIN_SEGMENT
    gasnetc_counter_t    counter = GASNETC_COUNTER_INITIALIZER;
    gasnetc_atomic_val_t *local_cnt, start_cnt;
    gasnetc_cb_t         local_cb;
    gasnete_eop_t        *eop = NULL;

    if (gasneti_leaf_is_pointer(lc_opt)) {
      if (flags & GASNETI_FLAG_LC_OPT_IN) {
        gasnete_op_t *op = gasneti_event_op(*lc_opt);
        if (OPTYPE(op) == OPTYPE_IMPLICIT) {
          gasnete_iop_t *iop = (gasnete_iop_t*)op;
          gasneti_assert(gasneti_event_idx(*lc_opt) == gasnete_iop_event_alc);
          gasneti_assert(iop->next); // Within an NBI access region
          local_cnt = &iop->initiated_alc_cnt;
          local_cb = gasnetc_cb_nar_alc;
        } else {
          eop = (gasnete_eop_t*)op;
          gasneti_assert(gasneti_event_idx(*lc_opt) == gasnete_eop_event_alc);
          GASNETE_EOP_LC_START(eop);
          start_cnt = eop->initiated_alc;
          local_cnt = &eop->initiated_alc;
          local_cb = gasnetc_cb_eop_alc;
        }
      } else {
        eop = _gasnete_eop_new(GASNETI_MYTHREAD);
        *lc_opt = (gex_Event_t)eop;
        GASNETE_EOP_LC_START(eop);
        start_cnt = eop->initiated_alc;
        local_cnt = &eop->initiated_alc;
        local_cb = gasnetc_cb_eop_alc;
      }
    } else if (lc_opt == GEX_EVENT_NOW) {
      local_cnt = &counter.initiated;
      local_cb = gasnetc_cb_counter;
    } else {
      gasneti_fatalerror("Invalid lc_opt argument to ReplyLong");
    }

    retval = gasnetc_ReqRepGeneric(ep, gasneti_Long, rbuf, GASNETC_MSG_SRCIDX(rbuf->rbuf_flags), handler,
                                   source_addr, nbytes, dest_addr,
                                   flags, numargs, local_cnt, local_cb, NULL,
                                   argptr GASNETI_THREAD_PASS);

    if (lc_opt == GEX_EVENT_NOW) {
      /* block for local completion of RDMA transfer */
      gasnetc_counter_wait(&counter, 1 /* handler context */ GASNETI_THREAD_PASS);
    } else if (eop && (start_cnt == eop->initiated_alc)) {
      // Synchronous LC - reset LC state and pass-back INVALID_HANDLE as result
      GASNETE_EOP_LC_FINISH(eop);
      *lc_opt = GEX_EVENT_INVALID;
      if (!(flags & GASNETI_FLAG_LC_OPT_IN)) {
        gasnete_eop_free(eop GASNETI_THREAD_PASS);
      }
    }
  #else
    gasneti_leaf_finish(lc_opt); // Always "packed long", and thus locally-complete
    retval = gasnetc_ReqRepGeneric(ep, gasneti_Long, rbuf, GASNETC_MSG_SRCIDX(rbuf->rbuf_flags), handler,
                                   source_addr, nbytes, dest_addr,
                                   flags, numargs, NULL, NULL, NULL,
                                   argptr GASNETI_THREAD_PASS);
  #endif

    gasneti_assert(!rbuf->rbuf_needReply || (flags & GEX_FLAG_IMMEDIATE));
  }
  return (retval == GASNETC_FAIL_IMM);
}

/* ------------------------------------------------------------------------------------ */

extern int gasnetc_AMRequestShortM( 
                            gex_TM_t tm,/* local context */
                            gex_Rank_t rank,       /* with tm, defines remote context */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            gex_Flags_t flags
                            GASNETI_THREAD_FARG,
                            int numargs, ...)
{
  GASNETI_COMMON_AMREQUESTSHORT(tm,rank,handler,flags,numargs);

  /* ensure progress */
  if (GASNETC_IMMEDIATE_WOULD_POLL(flags)) {
    gasnetc_poll_rcv();
    GASNETI_PROGRESSFNS_RUN();
  }

  va_list argptr;
  va_start(argptr, numargs);
  int retval = gasnetc_AMRequestShort(tm,rank,handler,flags,numargs,argptr GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
}

extern int gasnetc_AMRequestMediumV(
                            gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  return gasnetc_AMRequestMedium(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
}

extern int gasnetc_AMRequestMediumM( 
                            gex_TM_t tm,/* local context */
                            gex_Rank_t rank,       /* with tm, defines remote context */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            void *source_addr, size_t nbytes,   /* data payload */
                            gex_Event_t *lc_opt,       /* local completion of payload */
                            gex_Flags_t flags
                            GASNETI_THREAD_FARG,
                            int numargs, ...)
{
  GASNETI_COMMON_AMREQUESTMEDIUM(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs);

  /* ensure progress */
  if (GASNETC_IMMEDIATE_WOULD_POLL(flags)) {
    gasnetc_poll_rcv();
    GASNETI_PROGRESSFNS_RUN();
  }

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMRequestMedium(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
  va_end(argptr);
  return (retval == GASNETC_FAIL_IMM);
}

extern int gasnetc_AMRequestLongV(
                            gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes, void *dest_addr,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  return gasnetc_AMRequestLong(tm,rank,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
}

extern int gasnetc_AMRequestLongM(
                            gex_TM_t tm,/* local context */
                            gex_Rank_t rank,       /* with tm, defines remote context */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            gex_Event_t *lc_opt,       /* local completion of payload */
                            gex_Flags_t flags
                            GASNETI_THREAD_FARG,
                            int numargs, ...)
{
  GASNETI_COMMON_AMREQUESTLONG(tm,rank,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs);

  /* ensure progress */
  if (GASNETC_IMMEDIATE_WOULD_POLL(flags)) {
    gasnetc_poll_rcv();
    GASNETI_PROGRESSFNS_RUN();
  }

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMRequestLong(tm,rank,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
}

extern int gasnetc_AMReplyShortM( 
                            gex_Token_t token,     /* token provided on handler entry */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            gex_Flags_t flags,
                            int numargs, ...)
{
  gasneti_assert(token);
  GASNETI_COMMON_AMREPLYSHORT(token,handler,flags,numargs);

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMReplyShort(token,handler,flags,numargs,argptr);
  va_end(argptr);
  return retval;
}

extern int gasnetc_AMReplyMediumV(
                            gex_Token_t token, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  return gasnetc_AMReplyMedium(token,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr);
}

extern int gasnetc_AMReplyMediumM( 
                            gex_Token_t token,     /* token provided on handler entry */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            void *source_addr, size_t nbytes,   /* data payload */
                            gex_Event_t *lc_opt,       /* local completion of payload */
                            gex_Flags_t flags,
                            int numargs, ...)
{
  gasneti_assert(token);
  GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,lc_opt,flags,numargs);

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMReplyMedium(token,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr);
  va_end(argptr);
  return retval;
}

extern int gasnetc_AMReplyLongV(
                            gex_Token_t token, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes, void *dest_addr,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  return gasnetc_AMReplyLong(token,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs,argptr);
}

extern int gasnetc_AMReplyLongM( 
                            gex_Token_t token,     /* token provided on handler entry */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            gex_Event_t *lc_opt,       /* local completion of payload */
                            gex_Flags_t flags,
                            int numargs, ...)
{
  gasneti_assert(token);
  GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs);

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMReplyLong(token,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs,argptr);
  va_end(argptr);
  return retval;
}

/* ------------------------------------------------------------------------------------ */
/*
  Misc. Active Message Functions
  ==============================
*/
#if GASNET_PSHM
/* (###) GASNETC_GET_HANDLER
 *   If your conduit will support PSHM, then there needs to be a way
 *   for PSHM to see your handler table.  If you use the recommended
 *   implementation then you don't need to do anything special.
 *   Othwerwise, #define GASNETC_GET_HANDLER in gasnet_core_fwd.h and
 *   implement gasnetc_get_handler() as a macro in
 *   gasnet_core_internal.h
 */
#endif

// NOTE: unlike other conduits this gets used outside the file, and w/ AMPSHM tokens too!
gex_Rank_t gasnetc_msgsource(gex_Token_t token) {
  gex_Rank_t sourceid;
  gasneti_assert(token);

  if (gasnetc_token_in_nbrhd(token)) {
    gex_Token_Info_t info;
    unsigned int rc = gasnetc_nbrhd_Token_Info(token, &info, GEX_TI_SRCRANK);
    gasneti_assert(rc & GEX_TI_SRCRANK);
    sourceid = info.gex_srcrank;
  } else {
    uint32_t flags = ((gasnetc_rbuf_t *)token)->rbuf_flags;

    if (GASNETC_MSG_HANDLERID(flags) >= GASNETE_HANDLER_BASE) {
      GASNETI_CHECKATTACH();
    }

    sourceid = GASNETC_MSG_SRCIDX(flags);
  }

  gasneti_assert(sourceid < gasneti_nodes);
  return sourceid;
}

extern gex_TI_t gasnetc_Token_Info(
                gex_Token_t         token,
                gex_Token_Info_t    *info,
                gex_TI_t            mask)
{
  gasneti_assert(token);
  gasneti_assert(info);

  if (gasnetc_token_in_nbrhd(token)) {
    return gasnetc_nbrhd_Token_Info(token, info, mask);
  }

  const gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)token;
  uint32_t flags = rbuf->rbuf_flags;
  gex_TI_t result = 0;

  if (GASNETC_MSG_HANDLERID(flags) >= GASNETE_HANDLER_BASE) GASNETI_CHECKATTACH();

  info->gex_srcrank = GASNETC_MSG_SRCIDX(flags);
  result |= GEX_TI_SRCRANK;

#if GASNET_TRACE
  // TRACE of source of bootstrap AMs can reach here before gasneti_THUNK_TM is set
  info->gex_ep = gasneti_THUNK_TM ? gasneti_THUNK_EP : NULL;
#else
  info->gex_ep = gasneti_THUNK_EP;
#endif
  result |= GEX_TI_EP;

  info->gex_entry = &rbuf->rr_ep->_amtbl[GASNETC_MSG_HANDLERID(flags)];
  result |= GEX_TI_ENTRY;

  info->gex_is_req = GASNETC_MSG_ISREQUEST(flags);
  result |= GEX_TI_IS_REQ;

  info->gex_is_long = (gasneti_Long == GASNETC_MSG_CATEGORY(flags));
  result |= GEX_TI_IS_LONG;

  return GASNETI_TOKEN_INFO_RETURN(result, info, mask);
}

extern int gasnetc_AMPoll(GASNETI_THREAD_FARG_ALONE) {
  GASNETI_CHECKATTACH();
  gasnetc_poll_both();

  return GASNET_OK;
}
