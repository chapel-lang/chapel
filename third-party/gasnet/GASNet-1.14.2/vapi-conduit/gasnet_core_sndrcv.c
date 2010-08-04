/*   $Source: /var/local/cvs/gasnet/vapi-conduit/gasnet_core_sndrcv.c,v $
 *     $Date: 2010/04/17 00:11:40 $
 * $Revision: 1.245.2.2 $
 * Description: GASNet vapi conduit implementation, transport send/receive logic
 * Copyright 2003, LBNL
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#include <gasnet_core_internal.h>

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
     gasneti_lifo_push(&gasnetc_bbuf_freelist,*)
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

/* Remove when post-list code is fixed or permanently removed */
#define GASNETC_USE_POST_LIST 0

/* ------------------------------------------------------------------------------------ *
 *  Global variables                                                                    *
 * ------------------------------------------------------------------------------------ */
size_t					gasnetc_fh_align;
size_t					gasnetc_fh_align_mask;
size_t                   		gasnetc_inline_limit;
size_t                   		gasnetc_am_inline_limit_sndrcv = 0;
size_t                   		gasnetc_am_inline_limit_rdma = 0;
size_t                   		gasnetc_bounce_limit;
size_t					gasnetc_packedlong_limit;
#if !GASNETC_PIN_SEGMENT
  size_t				gasnetc_putinmove_limit;
  size_t				gasnetc_putinmove_limit_adjusted = 0;
#endif
int					gasnetc_use_rcv_thread = GASNETC_IB_RCV_THREAD;
#if GASNETC_FH_OPTIONAL
  int					gasnetc_use_firehose = 1;
#endif
int					gasnetc_am_credits_slack;
int					gasnetc_num_qps;
int					gasnetc_amrdma_max_peers;
size_t					gasnetc_amrdma_limit;
int					gasnetc_amrdma_depth;
int					gasnetc_amrdma_slot_mask;
gasneti_weakatomic_val_t		gasnetc_amrdma_cycle;

/* ------------------------------------------------------------------------------------ *
 *  File-scoped types                                                                   *
 * ------------------------------------------------------------------------------------ */

/* Description of a receive buffer.
 *
 * Note that use of the freelist will overwrite the first sizeof(void *) bytes (linkage).
 * Note the macros following the typedef are used to hide the existence of the union.
 */
typedef struct {
  union {
    void 			*linkage;
    struct {
      /* Fields intialized at recv time: */
      int                   	needReply;
      int                   	handlerRunning;
      uint32_t              	flags;
    }				am;
  } u;

  /* Field that changes each time the rbuf is posted */
  gasnetc_cep_t			*cep;

  /* Fields fixed for life of the rbuf as it is reused */
  int				rr_is_rdma;	/* is AM-over-RMDA? */
  gasnetc_rcv_wr_t        	rr_desc;        /* recv request descriptor */
  gasnetc_sge_t			rr_sg;          /* single-entry scatter list */
} gasnetc_rbuf_t;
#define rbuf_needReply		u.am.needReply
#define rbuf_handlerRunning	u.am.handlerRunning
#define rbuf_flags		u.am.flags

typedef enum {
	GASNETC_OP_FREE,
	GASNETC_OP_AM,
	GASNETC_OP_AM_BLOCK,
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

#if GASNETC_USE_POST_LIST
  /* Number of Work Request entries */
  uint32_t			count;
#endif

  /* Completion counters */
  gasnetc_counter_t		*mem_oust;	/* source memory refs outstanding (local completion)*/
  gasnetc_counter_t		*req_oust;	/* requests outstanding (remote completion)*/

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
      gasneti_weakatomic_t	ready;	/* 0 when loc and rem both ready */
      gasnetc_counter_t		*oust;	/* fh transactions outstanding */
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
 * Unlike gasnete_threaddata_t, this is associated w/ conduit-internal threads as well.
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

static gasneti_lifo_head_t		gasnetc_bbuf_freelist = GASNETI_LIFO_INITIALIZER;

static gasneti_semaphore_t		*gasnetc_cq_semas;
static gasnetc_cep_t			**gasnetc_node2cep;

#if GASNETC_PIN_SEGMENT
  static uintptr_t			*gasnetc_seg_ends;
#endif

#if GASNETI_THREADS
  GASNETI_THREADKEY_DEFINE(gasnetc_per_thread_key);
#else
  static gasnetc_per_thread_t gasnetc_per_thread;
#endif

/* ------------------------------------------------------------------------------------ *
 *  File-scoped functions and macros                                                    *
 * ------------------------------------------------------------------------------------ */

#ifndef GASNETC_PERTHREAD_OPT
  #if GASNETI_THREADS
    #define GASNETC_PERTHREAD_OPT 1
  #else
    #define GASNETC_PERTHREAD_OPT 0
  #endif
#endif

#if GASNETC_PERTHREAD_OPT
  #define GASNETC_PERTHREAD_FARG_ALONE	void * const GASNETI_RESTRICT _core_threadinfo
  #define GASNETC_PERTHREAD_FARG	, GASNETC_PERTHREAD_FARG_ALONE
  #define GASNETC_PERTHREAD_PASS_ALONE	(_core_threadinfo)
  #define GASNETC_PERTHREAD_PASS	, GASNETC_PERTHREAD_PASS_ALONE
  #define GASNETC_MY_PERTHREAD()	((gasnetc_per_thread_t *)_core_threadinfo)
  #define GASNETC_PERTHREAD_LOOKUP	void * const _core_threadinfo = (void *)gasnetc_my_perthread()
#else
  #define GASNETC_PERTHREAD_FARG_ALONE
  #define GASNETC_PERTHREAD_FARG
  #define GASNETC_PERTHREAD_PASS_ALONE
  #define GASNETC_PERTHREAD_PASS
  #define GASNETC_MY_PERTHREAD()	(gasnetc_my_perthread())
  #define GASNETC_PERTHREAD_LOOKUP	const char _core_threadinfo_dummy = sizeof(_core_threadinfo_dummy) /* no semicolon */
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
void gasnetc_alloc_sreqs(gasnetc_sreq_t **head_p, gasnetc_sreq_t **tail_p)
{
  const int count = GASNETC_SREQS_GROWTHCNT;
  size_t bytes = GASNETI_ALIGNUP(sizeof(gasnetc_sreq_t), GASNETI_CACHE_LINE_BYTES);
  gasnetc_sreq_t *ptr = gasneti_malloc_aligned(GASNETI_CACHE_LINE_BYTES, count * bytes);
  int i;
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

void gasnetc_per_thread_init(gasnetc_per_thread_t *td)
{
  gasnetc_sreq_t *tail;
  gasnetc_alloc_sreqs(&td->sreqs, &tail);
  tail->next = td->sreqs;
}

#if GASNETI_THREADS
  GASNETI_INLINE(gasnetc_my_perthread) GASNETI_CONST
  gasnetc_per_thread_t *gasnetc_my_perthread(void) {
    gasnetc_per_thread_t *retval = gasneti_threadkey_get_noinit(gasnetc_per_thread_key);
    if_pf (retval == NULL) {
      retval = gasneti_malloc_aligned(GASNETI_CACHE_LINE_BYTES,
				  GASNETI_ALIGNUP(sizeof(gasnetc_per_thread_t), GASNETI_CACHE_LINE_BYTES));
      gasnete_register_threadcleanup(gasnetc_free_aligned, retval);
      gasneti_threadkey_set_noinit(gasnetc_per_thread_key, retval);
      gasnetc_per_thread_init(retval);
    }
    gasneti_assert(retval != NULL);
    return retval;
  }
  GASNETI_CONSTP(gasnetc_my_perthread)
#else
  #define gasnetc_my_perthread() (&gasnetc_per_thread)
#endif

static int
gasnetc_create_cq(gasnetc_hca_hndl_t hca_hndl, gasnetc_cqe_cnt_t req_size,
		  gasnetc_cq_hndl_t *cq_p, gasnetc_cqe_cnt_t *act_size)
{
#if GASNET_CONDUIT_VAPI
  return VAPI_create_cq(hca_hndl, req_size, cq_p, act_size);
#else
  gasnetc_cq_hndl_t result = ibv_create_cq(hca_hndl, req_size, NULL, NULL, 0);
  if_pt (result != NULL) {
    *cq_p = result;
    *act_size = result->cqe;
    return 0;
  } else {
    return 1;
  }
#endif
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

#define GASNETC_SND_LKEY(_cep)		((_cep)->keys.snd_lkey)
#define GASNETC_RCV_LKEY(_cep)		((_cep)->keys.rcv_lkey)
#define GASNETC_SEG_LKEY(_cep, _index)	((_cep)->keys.seg_reg[_index].lkey)
#define GASNETC_SEG_RKEY(_cep, _index)	((_cep)->keys.rkeys[_index])
#if GASNETC_IB_MAX_HCAS > 1
  #define GASNETC_FH_RKEY(_cep, _fhptr)	((_fhptr)->client.rkey[(_cep)->hca_index])
  #define GASNETC_FH_LKEY(_cep, _fhptr)	((_fhptr)->client.lkey[(_cep)->hca_index])
#else
  #define GASNETC_FH_RKEY(_cep, _fhptr)	((_fhptr)->client.rkey[0])
  #define GASNETC_FH_LKEY(_cep, _fhptr)	((_fhptr)->client.lkey[0])
#endif

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
void *gasnetc_sr_desc_init(gasnetc_snd_wr_t *result, gasnetc_sge_t *sg_lst_p, int sg_lst_len, int count)
{
  #if GASNETC_USE_POST_LIST
    int i;
    for (i=0; i<count; ++i, sg_lst_p += sg_lst_len) {
          #if GASNET_DEBUG
	    result[i].gasnetc_f_wr_num_sge = 0; /* invalid to ensure caller sets it */
	  #endif
	  result[i].gasnetc_f_wr_sg_list = sg_lst_p;
    }
  #else
    gasneti_assert(count == 1);
    #if GASNET_DEBUG
      result->gasnetc_f_wr_num_sge = 0; /* invalid to ensure caller sets it */
    #endif
    result->gasnetc_f_wr_sg_list = sg_lst_p;
  #endif
  
  return result;
}
#define GASNETC_DECL_SR_DESC(_name, _sg_lst_len, _count)                \
	gasnetc_snd_wr_t _name[_count];                                   \
	gasnetc_sge_t _CONCAT(_name,_sg_lst)[_count*_sg_lst_len];       \
	GASNETI_UNUSED                                                  \
	void *_CONCAT(_name,_dummy) = gasnetc_sr_desc_init(_name, _CONCAT(_name,_sg_lst), _sg_lst_len, _count) /* note intentional lack of final semicolon */

/* Use of IB's 32-bit immediate data:
 *   0-7: handlerID
 *   8-9: category
 * 10-14: numargs (5 bits, but only 0-GASNETC_MAX_ARGS are legal values)
 *    15: request (0) or reply (1)
 * 16-29: source node (14 bit LID space in IB)
 *    30: carries AM flow-control info
 *    31: UNUSED
 */

#define GASNETC_MSG_HANDLERID(flags)    ((gasnet_handler_t)(flags))
#define GASNETC_MSG_CATEGORY(flags)     ((gasnetc_category_t)(((flags) >> 8) & 0x3))
#define GASNETC_MSG_NUMARGS(flags)      (((flags) >> 10) & 0x1f)
#define GASNETC_MSG_ISREPLY(flags)      ((flags) & (1<<15))
#define GASNETC_MSG_ISREQUEST(flags)    (!GASNETC_MSG_ISREPLY(flags))
#define GASNETC_MSG_SRCIDX(flags)       ((gasnet_node_t)((flags) >> 16) & 0x3fff)
#define GASNETC_MSG_FLOW(flags)         ((flags) & (1<<30))

#define GASNETC_MSG_GENFLAGS(isreq, cat, nargs, hand, srcidx, flow)   \
 (gasneti_assert(0 == ((srcidx) & ~0x3fff)),    \
  gasneti_assert(0 == ((nargs)  & ~0x1f)),      \
  gasneti_assert(0 == ((cat)    & ~3)),         \
  gasneti_assert((nargs) <= GASNETC_MAX_ARGS),  \
  gasneti_assert((srcidx) < gasneti_nodes),     \
  (uint32_t)(  ((flow)    ? (1<<30) : 0)        \
             | ((nargs)   << 10        )        \
             | ((isreq)   ? 0 : (1<<15))        \
             | ((srcidx)  << 16        )        \
             | ((cat)     << 8         )        \
             | ((hand)                 )))

/* Work around apparent thread-safety bug in VAPI_poll_cq (and peek as well?) */
#if (GASNET_CONDUIT_VAPI && GASNETC_VAPI_POLL_LOCK) || \
    (GASNET_CONDUIT_IBV && GASNETC_IBV_POLL_LOCK)
  static gasneti_mutex_t gasnetc_cq_poll_lock = GASNETI_MUTEX_INITIALIZER;
  #define CQ_LOCK	gasneti_mutex_lock(&gasnetc_cq_poll_lock);
  #define CQ_UNLOCK	gasneti_mutex_unlock(&gasnetc_cq_poll_lock);
#else
  #define CQ_LOCK	do {} while (0)
  #define CQ_UNLOCK	do {} while (0)
#endif

#define gasnetc_poll_rcv()		gasnetc_do_poll(1,0)
#define gasnetc_poll_snd()		gasnetc_do_poll(0,1)
#define gasnetc_poll_both()		gasnetc_do_poll(1,1)

/* Post a work request to the receive queue of the given endpoint */
GASNETI_INLINE(gasnetc_rcv_post)
void gasnetc_rcv_post(gasnetc_cep_t *cep, gasnetc_rbuf_t *rbuf) {
  int vstat;

  gasneti_assert(cep);
  gasneti_assert(rbuf);

  /* check for attempted loopback traffic */
  gasneti_assert((cep - gasnetc_cep)/gasnetc_num_qps != gasneti_mynode);
  
  rbuf->cep = cep;
  rbuf->rr_sg.lkey = GASNETC_RCV_LKEY(cep);
  GASNETI_TRACE_PRINTF(D,("POST_RR rbuf=%p peer=%d qp=%d hca=%d lkey=0x%08x", 
			  rbuf, gasnetc_epid2node(cep->epid),
			  gasnetc_epid2qpi(cep->epid) - 1, cep->hca_index,
			  (unsigned int)(rbuf->rr_sg.lkey)));
#if GASNET_CONDUIT_VAPI
  vstat = VAPI_post_rr(cep->hca_handle, cep->qp_handle, &rbuf->rr_desc);
#else
  {
    struct ibv_recv_wr *bad_wr;
    vstat = ibv_post_recv(cep->qp_handle, &rbuf->rr_desc, &bad_wr);
  }
#endif

  if_pt (vstat == 0) {
    /* normal return */
    return;
  } else if (GASNETC_IS_EXITING()) {
    /* disconnected by another thread */
    gasnetc_exit(0);
  } else {
    /* unexpected error */
    GASNETC_VAPI_CHECK(vstat, "while posting a receive work request");
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
  gasneti_assert(cep->amrdma_loc == NULL);

  GASNETI_TRACE_PRINTF(C,("AMRDMA_GRANT_SND to node=%d qp=%d\n", (int)gasnetc_epid2node(cep->epid), (int)gasnetc_epid2qpi(cep->epid)-1));

  cep->amrdma_loc = gasneti_lifo_pop(&hca->amrdma_freelist);
  if (cep->amrdma_loc != NULL) {
    int count;
    int i;

    for (i = 0; i < gasnetc_amrdma_depth; ++i) {
      gasnetc_amrdma_hdr_t *hdr = (gasnetc_amrdma_hdr_t *)cep->amrdma_loc[i];
      hdr->length       = hdr->zeros       = 0;
      hdr->length_again = hdr->zeros_again = ~0;
      gasneti_assert(gasneti_count0s(hdr, GASNETC_AMRDMA_SZ) == (GASNETC_AMRDMA_SZ - 4));
#if GASNETI_THREADS
      gasneti_weakatomic_set(&cep->amrdma.recv_busy[i].spinlock, 0, 0);
#endif
    }

    count = gasneti_weakatomic_read(&hca->amrdma_rcv.count, 0);
    gasneti_assert(count < hca->total_qps);
    gasneti_assert(count < hca->amrdma_rcv.max_peers);
    hca->amrdma_rcv.cep[count] = cep;
    gasneti_weakatomic_set(&hca->amrdma_rcv.count, count+1, GASNETI_ATOMIC_REL);

    GASNETI_SAFE(
	SHORT_REQ(3,4,(gasnetc_epid2node(cep->epid), gasneti_handleridx(gasnetc_amrdma_grant_reqh),
		       (gasnet_handlerarg_t)gasnetc_epid2qpi(cep->epid),
		       (gasnet_handlerarg_t)hca->amrdma_reg.rkey,
		       PACK(cep->amrdma_loc))));
  }
}

void gasnetc_amrdma_eligable(gasnetc_cep_t *cep) {
  gasnetc_hca_t * const hca = cep->hca;
  gasneti_weakatomic_val_t interval = gasneti_weakatomic_add(&hca->amrdma_balance.count, 1, 0);

  gasneti_weakatomic_increment(&cep->amrdma.eligable, 0);

#if GASNETI_THREADS
  #define GASNETC_TRY_BALANCE_LOCK(_hca) gasneti_spinlock_trylock(&(_hca)->amrdma_balance.lock)
#else
  #define GASNETC_TRY_BALANCE_LOCK(_hca) 0
#endif

  if_pf (!(interval & hca->amrdma_balance.mask) && !GASNETC_TRY_BALANCE_LOCK(hca)) {
    /* GASNETC_AMRDMA_REDUCE(X) is amount by which ALL counts X are reduced each round */
    #define GASNETC_AMRDMA_REDUCE(X)		((X)>>1)
    /* GASNETC_AMRDMA_BOOST(FLOOR) is amount by which SELECTED counts X are boosted */
    #define GASNETC_AMRDMA_BOOST(FLOOR)	((FLOOR)>>1)

    gasnetc_amrdma_balance_tbl_t *tbl = hca->amrdma_balance.table;
    int tbl_size = 0;
    int i;

    /* Pass 1: Collect all peers w/ counts >= floor, while also "decaying" the counters.
     * This is the only part that should be O(gasnet_nodes) on average.
     */
    for (i = 0; i < hca->total_qps; ++i) {
      gasneti_weakatomic_val_t x, y;

      cep = hca->cep[i];
      x = gasneti_weakatomic_read(&cep->amrdma.eligable, 0);
      y = GASNETC_AMRDMA_REDUCE(x);
      gasneti_weakatomic_subtract(&cep->amrdma.eligable, y, 0);

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
      gasneti_weakatomic_val_t new_floor = tbl[0].count;
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
      gasneti_weakatomic_val_t boost = GASNETC_AMRDMA_BOOST(hca->amrdma_balance.floor);
      for (i = 0; i < tbl_size; ++i) {
        cep = tbl[i].cep;
        if (!cep->amrdma_loc) {
          gasnetc_amrdma_grant(hca, cep);
        }
        gasneti_weakatomic_add(&cep->amrdma.eligable, boost, 0);
      }
    }

    if (gasneti_weakatomic_read(&hca->amrdma_rcv.count, 0) == hca->amrdma_rcv.max_peers) {
      /* Disable this logic if the limit has been reached (since we lack REVOKE)*/
      return; /* YES - we really mean to return w/o unlocking */
    }

#if GASNETI_THREADS
    gasneti_spinlock_unlock(&hca->amrdma_balance.lock);
#endif
  }
}

/* GASNETI_INLINE(gasnetc_processPacket) */
void gasnetc_processPacket(gasnetc_cep_t *cep, gasnetc_rbuf_t *rbuf, uint32_t flags) {
  gasnetc_buffer_t *buf = (gasnetc_buffer_t *)(uintptr_t)(rbuf->rr_sg.addr);
  gasnet_handler_t handler_id = GASNETC_MSG_HANDLERID(flags);
  gasneti_handler_fn_t handler_fn = gasnetc_handler[handler_id];
  gasnetc_category_t category = GASNETC_MSG_CATEGORY(flags);
  int full_numargs = GASNETC_MSG_NUMARGS(flags);
  int user_numargs = full_numargs;
  gasnet_handlerarg_t *args;

  #if GASNET_PSHM
    gasneti_assert(!gasneti_pshm_in_supernode(GASNETC_MSG_SRCIDX(flags)));
    gasneti_assert(cep != NULL);
  #endif

  rbuf->rbuf_needReply = GASNETC_MSG_ISREQUEST(flags);
  rbuf->rbuf_handlerRunning = 1;
  rbuf->rbuf_flags = flags;

  /* Locate arguments */
  switch (category) {
    case gasnetc_System:
    case gasnetc_Short:
      args = buf->shortmsg.args;
      break;

    case gasnetc_Medium:
      args = buf->medmsg.args;
      break;

    case gasnetc_Long:
      args = buf->longmsg.args;
      break;

    default:
    gasneti_fatalerror("invalid AM category on recv");
  }

  if_pt (GASNET_PSHM || (cep != NULL)) { /* Process any flow control info, unless loopback */
    int credits = 0;

    if (GASNETC_MSG_FLOW(flags)) {
      int acks = (args[0] >> 8) & 0xff;
      credits = args[0] & 0xff;

      if (acks) {
        gasneti_assert(acks <= gasnetc_amrdma_depth);
        gasneti_assert(acks > 0);
        gasneti_weakatomic_add(&cep->amrdma.send_head, acks, 0);
      }
      if (credits) {
        gasneti_semaphore_up_n(&cep->am_loc, credits);
      }

      GASNETI_TRACE_PRINTF(C,("RCV_AM_CREDITS credits=%d acks=%d\n", credits, acks));

      args += 1;
      user_numargs -= 1;
    }

    /* Available remotely posted (request) buffers */
    if (GASNETC_MSG_ISREPLY(flags)) { ++credits; } /* Credit for self */
    if (credits) {
      gasneti_semaphore_up_n(&cep->am_rem, credits);
    }
  }

  /* Run the handler */
  switch (category) {
    case gasnetc_System:
      {
        handler_fn = gasnetc_sys_handler[handler_id];
        GASNETC_RUN_HANDLER_SYS(GASNETC_MSG_ISREQUEST(flags),handler_id,handler_fn,rbuf,args,user_numargs);
      }
      break;

    case gasnetc_Short:
      { 
        GASNETI_RUN_HANDLER_SHORT(GASNETC_MSG_ISREQUEST(flags),handler_id,handler_fn,rbuf,args,user_numargs);
      }
      break;

    case gasnetc_Medium:
      {
        void * data = GASNETC_MSG_MED_DATA(buf, full_numargs);
        size_t nbytes = buf->medmsg.nBytes;
        GASNETI_RUN_HANDLER_MEDIUM(GASNETC_MSG_ISREQUEST(flags),handler_id,handler_fn,rbuf,args,user_numargs,data,nbytes);
      }
      break;

    case gasnetc_Long:
      { 
        void * data = (void *)(buf->longmsg.destLoc);
	size_t nbytes = buf->longmsg.nBytes & 0x7fffffff;
	if (buf->longmsg.nBytes & 0x80000000) {
	  /* Must relocate the payload which is packed like a Medium. */
	  gasneti_assert(nbytes <= GASNETC_MAX_PACKEDLONG);
	  gasneti_assert((nbytes <= gasnetc_packedlong_limit) ||
			 (!GASNETC_PIN_SEGMENT && GASNETC_MSG_ISREPLY(flags)));
	  memcpy(data, GASNETC_MSG_LONG_DATA(buf, full_numargs), (size_t)nbytes);
	}
        GASNETI_RUN_HANDLER_LONG(GASNETC_MSG_ISREQUEST(flags),handler_id,handler_fn,rbuf,args,user_numargs,data,(size_t)nbytes);
      }
      break;
  }
  
  rbuf->rbuf_handlerRunning = 0;
}

#if GASNETC_SND_REAP_COLLECT
  #define _GASNETC_COLLECT_BBUF(_test,_bbuf) do { \
      void *_tmp = (void*)(_bbuf);                \
      _test((_tmp != NULL)) {                     \
        gasneti_lifo_link(bbuf_tail, _tmp);   \
        bbuf_tail = _tmp;                         \
      }                                           \
    } while(0)
  #define GASNETC_FREE_BBUFS() do {    \
      if (bbuf_tail != &bbuf_dummy) {  \
        gasneti_lifo_push_many(&gasnetc_bbuf_freelist, gasneti_lifo_next(&bbuf_dummy), bbuf_tail); \
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
        gasneti_lifo_push(&gasnetc_bbuf_freelist,_tmp); \
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
void gasnetc_dump_cqs(gasnetc_wc_t *comp, gasnetc_hca_t *hca, const int is_snd)) {
  static gasnet_hsl_t lock = GASNET_HSL_INITIALIZER;
  int vstat;
  gasnetc_wc_status_t status = GASNETC_WC_SUCCESS;
  int count = 0;
  const char *label;
  int max_other_cq;

  gasnet_hsl_lock(&lock);

  if (is_snd) {
    gasnetc_sreq_t *sreq = (gasnetc_sreq_t *)(uintptr_t)comp->gasnetc_f_wr_id;
    gasnet_node_t node = gasnetc_epid2node(sreq->cep->epid);
    int qpi = gasnetc_epid2qpi(sreq->cep->epid);
    if (comp->status == GASNETC_WC_RETRY_EXC_ERR) {
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
    CQ_LOCK;
    vstat = is_snd ? gasnetc_poll_rcv_cq(hca, comp)
		   : gasnetc_poll_snd_cq(hca, comp);
    CQ_UNLOCK;
    if (vstat != 0) {
      /* use an invalid value to ensure output is generated on the last pass */
#if GASNET_CONDUIT_IBV
      comp->status = (enum ibv_wc_status)(-1);
#else
      comp->status = -1;
#endif
    }
    if (comp->status == status) {
      ++count;
    } else {
      if (count) {
        switch (status) {
	  case GASNETC_WC_SUCCESS:
            fprintf(stderr, "@ %d> - %s %d op(s) OK\n", gasneti_mynode, label, count);
	    break;
	  case GASNETC_WC_FLUSH_ERR:
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
  } while ((status != -1) && (count <= max_other_cq));
  if (count > max_other_cq) {
    fprintf(stderr, "@ %d> - %s CQ contains impossibly large WCE count with status %d\n", gasneti_mynode, label, status);
  }
  gasnet_hsl_unlock(&lock);
}

/* Try to pull completed entries (if any) from the send CQ(s). */
static int gasnetc_snd_reap(int limit) {
  int rc, count;
  gasnetc_wc_t comp;
  #if GASNETC_SND_REAP_COLLECT
    int i, fh_num = 0;
    void *bbuf_dummy;
    void *bbuf_tail = &bbuf_dummy;
    const firehose_request_t *fh_ptrs[GASNETC_SND_REAP_LIMIT * GASNETC_MAX_FH];
  #endif

  #if GASNETC_IB_MAX_HCAS > 1
    /* Simple round-robin (w/ a harmless multi-thread race) */
    gasnetc_hca_t *hca;
    static volatile int index = 0;
    int tmp = index;
    index = ((tmp == 0) ? gasnetc_num_hcas : tmp) - 1;
    hca = &gasnetc_hca[tmp];
  #else
    gasnetc_hca_t *hca = &gasnetc_hca[0];
  #endif

  gasneti_assert(limit <= GASNETC_SND_REAP_LIMIT);

  for (count = 0; count < limit; ++count) {
    CQ_LOCK;
    rc = gasnetc_poll_snd_cq(hca, &comp);
    CQ_UNLOCK;

    if_pt (rc == GASNETC_POLL_CQ_EMPTY) {
      /* CQ empty - we are done */
      break;
    } else if_pt (rc == GASNETC_POLL_CQ_OK) {
      if_pt (comp.status == GASNETC_WC_SUCCESS) {
        gasnetc_sreq_t *sreq = (gasnetc_sreq_t *)(uintptr_t)comp.gasnetc_f_wr_id;
        if_pt (sreq) {
	  gasneti_assert(sreq->opcode != GASNETC_OP_INVALID);
	  #if GASNETC_USE_POST_LIST
	    gasneti_semaphore_up_n(&sreq->cep->sq_sema, sreq->count);
	  #else
	    gasneti_semaphore_up(&sreq->cep->sq_sema);
	  #endif
	  gasneti_semaphore_up(sreq->cep->snd_cq_sema_p);

	  switch (sreq->opcode) {
          #if GASNETC_PIN_SEGMENT && GASNETC_FH_OPTIONAL
	  case GASNETC_OP_GET_BOUNCE:	/* Bounce-buffer GET */
	    gasneti_assert(comp.opcode == GASNETC_WC_RDMA_READ);
	    gasneti_assert(sreq->req_oust != NULL);
	    gasneti_assert(sreq->mem_oust == NULL);
	    gasneti_assert(!GASNETC_USE_FIREHOSE); /* Only possible when firehose disabled */
	    gasneti_assert(sreq->bb_buff != NULL);
	    gasneti_assert(sreq->bb_addr != NULL);
	    gasneti_assert(sreq->bb_len > 0);
	    memcpy(sreq->bb_addr, sreq->bb_buff, sreq->bb_len);
            gasneti_sync_writes();
            gasnetc_counter_dec(sreq->req_oust);
	    GASNETC_COLLECT_BBUF(sreq->bb_buff);
	    break;
          #endif

	  case GASNETC_OP_GET_ZEROCP:	/* Zero-copy GET */
	    gasneti_assert(comp.opcode == GASNETC_WC_RDMA_READ);
	    gasneti_assert(sreq->req_oust != NULL);
	    gasneti_assert(sreq->mem_oust == NULL);
            gasnetc_counter_dec(sreq->req_oust);
	    GASNETC_COLLECT_FHS();
	    break;

	  case GASNETC_OP_PUT_BOUNCE:	/* Bounce-buffer PUT */
	    gasneti_assert(comp.opcode == GASNETC_WC_RDMA_WRITE);
	    gasneti_assert(sreq->mem_oust == NULL);
            gasnetc_counter_dec_if(sreq->req_oust);
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
	    gasneti_assert(comp.opcode == GASNETC_WC_RDMA_WRITE);
	    gasneti_assert(sreq->mem_oust == NULL);
            gasnetc_counter_dec_if(sreq->req_oust);
            #if GASNETC_PIN_SEGMENT
	    gasneti_assert(sreq->fh_count == 0);
	    #else
	    GASNETC_COLLECT_FHS();
	    #endif
	    break;

	  case GASNETC_OP_PUT_ZEROCP:	/* Zero-copy PUT */
	    gasneti_assert(comp.opcode == GASNETC_WC_RDMA_WRITE);
	    gasneti_assert((sreq->mem_oust == NULL) || (sreq->req_oust == NULL));
	    if (sreq->req_oust != NULL) {
              gasnetc_counter_dec(sreq->req_oust);
	    } else if (sreq->mem_oust != NULL) {
	      gasnetc_counter_dec(sreq->mem_oust);
	    }
	    GASNETC_COLLECT_FHS();
	    break;

	  case GASNETC_OP_AM_BLOCK:	/* AM send (System w/ handle) */
	    gasneti_assert((comp.opcode == GASNETC_WC_SEND) ||
			   (comp.opcode == GASNETC_WC_RDMA_WRITE));
	    gasneti_assert(sreq->req_oust != NULL);
	    gasneti_assert(sreq->mem_oust == NULL);
            gasnetc_counter_dec(sreq->req_oust);
	    GASNETC_COLLECT_BBUF_IF(sreq->am_buff);
	    break;

	  case GASNETC_OP_AM:		/* AM send (normal) */
	    gasneti_assert((comp.opcode == GASNETC_WC_SEND) ||
			   (comp.opcode == GASNETC_WC_RDMA_WRITE));
	    gasneti_assert(sreq->req_oust == NULL);
	    gasneti_assert(sreq->mem_oust == NULL);
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
        gasneti_fatalerror("failed to connect (snd)");
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
      GASNETC_VAPI_CHECK(rc, "while reaping the send queue");
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

/* Take *unbound* epid, return a qp number */
GASNETI_INLINE(gasnetc_epid_select_qpi)
gasnetc_epid_t gasnetc_epid_select_qpi(gasnetc_cep_t *ceps, gasnetc_epid_t epid,
				       gasnetc_wr_opcode_t op, size_t len) {
#if GASNETC_IB_MAX_HCAS > 1
  gasnetc_epid_t qpi = gasnetc_epid2qpi(epid);

  if_pt (qpi == 0) {
#if 0
    /* Select by largest space avail */
    uint32_t space, best_space;
    int i;
    gasneti_assert(op != GASNETC_WR_SEND_WITH_IMM); /* AMs never wildcard */
    qpi = 0;
    best_space = gasneti_semaphore_read(&ceps[0].sq_sema);
    for (i = 1; i < gasnetc_num_qps; ++i) {
      space = gasneti_semaphore_read(&ceps[i].sq_sema);
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
  } else {
    --qpi; /* offset */
  }

  gasneti_assert(qpi < gasnetc_num_qps);
  return qpi;
#else
  return 0;
#endif
}

/* Take (sreq,op,len) and bind the sreq to a specific (not wildcard) qp */
GASNETI_INLINE(gasnetc_bind_cep)
gasnetc_cep_t *gasnetc_bind_cep(gasnetc_epid_t epid, gasnetc_sreq_t *sreq,
				gasnetc_wr_opcode_t op, size_t len) {
  gasnetc_cep_t *ceps = gasnetc_node2cep[gasnetc_epid2node(epid)];
  gasnetc_cep_t *cep;
  int qpi;

  /* Loop until space is available on the selected SQ for 1 new entry.
   * If we hold the last one then threads sending to the same node will stall. */
  qpi = gasnetc_epid_select_qpi(ceps, epid, op, len);
  cep = &ceps[qpi];
  if_pf (!gasneti_semaphore_trydown(&cep->sq_sema)) {
    GASNETC_TRACE_WAIT_BEGIN();
    do {
      if (!gasnetc_snd_reap(1)) {
        GASNETI_WAITHOOK();
      }
      /* Redo load balancing choice */
      qpi = gasnetc_epid_select_qpi(ceps, epid, op, len);
      cep = &ceps[qpi];
    } while (!gasneti_semaphore_trydown(&cep->sq_sema));
    GASNETC_TRACE_WAIT_END(POST_SR_STALL_SQ);
  }

  sreq->epid = gasnetc_epid(epid, qpi);
  sreq->cep = cep;

  return cep;
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
    old = gasneti_weakatomic_read(&cep->am_flow.credit, 0);
    if (old >= gasnetc_am_credits_slack) {
      /* MUST send back a reply */
      GASNETI_SAFE(gasnetc_ReplySystem((gasnet_token_t)rbuf, NULL,
				        gasneti_handleridx(gasnetc_SYS_ack), 0 /* no args */));
      break;
    }
  } while (!gasneti_weakatomic_compare_and_swap(&cep->am_flow.credit, old, old+1, 0));
}

GASNETI_INLINE(gasnetc_rcv_am)
void gasnetc_rcv_am(const gasnetc_wc_t *comp, gasnetc_rbuf_t **spare_p) {
  gasnetc_rbuf_t emergency_spare;
  gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)(uintptr_t)comp->gasnetc_f_wr_id;
  uint32_t flags = comp->imm_data;
  gasnetc_cep_t *cep = rbuf->cep;
  gasnetc_rbuf_t *spare;

  GASNETC_STAT_EVENT(RCV_AM_SNDRCV);

  if (GASNETC_MSG_ISREPLY(flags)) {
#if GASNETI_STATS_OR_TRACE
    gasneti_tick_t _starttime = ((gasnetc_buffer_t *)(uintptr_t)(rbuf->rr_sg.addr))->stamp;
    GASNETI_TRACE_EVENT_TIME(C,AM_ROUNDTRIP_TIME,gasneti_ticks_now()-_starttime);
#endif

    /* Now process the packet */
    gasnetc_processPacket(cep, rbuf, flags);

    /* Return the rcv buffer to the free list */
    gasneti_lifo_push(cep->rbuf_freelist, rbuf);
  } else {
    /* Post a replacement buffer before processing the request.
     * This ensures that the credit sent with the reply will
     * have a corresponding buffer available at this end */
    spare = (*spare_p) ? (*spare_p) : gasneti_lifo_pop(cep->rbuf_freelist);
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
      emergency_spare.cep = rbuf->cep;
      gasneti_assert(rbuf->rr_is_rdma == 0);
  
      gasnetc_rcv_post(cep, rbuf);

      rbuf = &emergency_spare;
      GASNETC_STAT_EVENT(ALLOC_AM_SPARE);
      GASNETI_TRACE_PRINTF(C,("ALLOC_AM_SPARE\n"));
    }

    /* Now process the packet */
    gasnetc_processPacket(cep, rbuf, flags);

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

static int gasnetc_rcv_reap(gasnetc_hca_t *hca, int limit, gasnetc_rbuf_t **spare_p) {
  int vstat;
  gasnetc_wc_t comp;
  int count;

  for (count = 0; count < limit; ++count) {
    CQ_LOCK;
    vstat = gasnetc_poll_rcv_cq(hca, &comp);
    CQ_UNLOCK;

    if_pt (vstat == GASNETC_POLL_CQ_EMPTY) {
      /* CQ empty - we are done */
      break;
    } else if_pt (vstat == GASNETC_POLL_CQ_OK) {
      if_pt (comp.status == GASNETC_WC_SUCCESS) {
        gasnetc_rcv_am(&comp, spare_p);
      } else if (GASNETC_IS_EXITING()) {
        /* disconnected */
	break;	/* can't exit since we can be called in exit path */
      } else if (!gasneti_attach_done) {
        gasneti_fatalerror("failed to connect (rcv)");
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
      GASNETC_VAPI_CHECK(vstat, "while reaping the recv queue");
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

GASNETI_INLINE(gasnetc_amrdma_zeros)
int gasnetc_amrdma_zeros(uint32_t flags, const void *buf, unsigned int length) {
  return gasneti_count0s_uint32_t(flags) + gasneti_count0s(buf, length);
}

GASNETI_INLINE(gasnetc_rcv_amrdma)
int gasnetc_rcv_amrdma(gasnetc_cep_t *cep) {
  const int recv_head = gasneti_weakatomic_read(&cep->amrdma.recv_head, 0);
  const int recv_slot = recv_head & gasnetc_amrdma_slot_mask;
  volatile gasnetc_amrdma_hdr_t * hdr = (volatile gasnetc_amrdma_hdr_t *)cep->amrdma_loc[recv_slot];
  gasnetc_buffer_t * msg_in;
  gasnetc_rbuf_t rbuf;
  uint32_t flags;
  int length, checksum, zeros;
#if !GASNETI_USE_ALLOCA
  void *tmp_buffer = NULL;
#endif

#if GASNETI_THREADS
  gasneti_weakatomic_t *slot_lock = &cep->amrdma.recv_busy[recv_slot].spinlock;

  /* First try a non-atomic "peek" and then try to acquire the spinlock */
  if (gasneti_weakatomic_read(slot_lock, 0) ||
      ((length = hdr->length) != hdr->length_again) ||
      ((checksum = hdr->zeros) != hdr->zeros_again) ||
      !gasneti_weakatomic_compare_and_swap(slot_lock, 0, 1, GASNETI_ATOMIC_ACQ)) {
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
  gasneti_assert(length > 0);
  gasneti_assert(length <= gasnetc_amrdma_limit);

  flags = hdr->immediate_data;
  zeros = gasneti_count0s_uint32_t(flags);
  msg_in = (gasnetc_buffer_t *)(hdr + 1);
  if (GASNETC_MSG_CATEGORY(flags) == gasnetc_Medium) {
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

#if GASNETI_THREADS
  if ((zeros != checksum) ||
      !gasneti_weakatomic_compare_and_swap(&cep->amrdma.recv_head, recv_head, recv_head+1, 0)) {
    /* If CAS failed then we've been "left behind" and are looking at the wrong slot */
    gasneti_weakatomic_set(slot_lock, 0, 0); /* No _REL, since nothing global was written */
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
  gasneti_weakatomic_increment(&cep->amrdma.recv_head, 0);
#endif

  GASNETC_STAT_EVENT(RCV_AM_RDMA);

  /* Account for any recv buffer that was reserved for the reply, but not used.
   * Must preced credit processing in gasnetc_processPacket (bug 2359) */
  if (GASNETC_MSG_ISREPLY(flags)) {
    gasneti_semaphore_up(&cep->am_loc);
  }

  /* Process the packet, includes running handler and processing credits/acks */
  rbuf.cep = cep;
  rbuf.rr_is_rdma = 1;
  gasnetc_processPacket(cep, &rbuf, flags);

  /* Mark slot free locally prior to enabling the ack */
  hdr->length = 0; hdr->length_again = -1;
  hdr->zeros = 0;  hdr->zeros_again = -1;
  hdr->immediate_data = 0;
  memset(msg_in, 0, length);

#if GASNETI_THREADS
  gasneti_weakatomic_set(slot_lock, 0, GASNETI_ATOMIC_REL);

  /* We must gather acks to keep them in-order even when handler completions are not */
  /* XXX: could be done lockless via recv_tail and ack_bits packed in gasneti_atomic64_t? */
  gasneti_mutex_lock(&cep->amrdma.ack_lock);
  { int count;
    const int recv_tail = cep->amrdma.recv_tail;
    uint32_t bits = cep->amrdma.ack_bits | (1 << (recv_head - recv_tail));

    gasneti_assert(bits != 0);

  #if 1
    for (count = 0; bits & 1; ++count) {
      bits >>= 1;
    }
  #else /* XXX: Use ffs() if/when we are sure it is faster (and available). */
    count = ffs(~bits);
    bits >>= count;
  #endif

    cep->amrdma.ack_bits = bits;
    if_pt (count) {
      cep->amrdma.recv_tail += count;
      gasneti_weakatomic_add(&cep->am_flow.ack, count, 0);
    }
  }
  gasneti_mutex_unlock(&cep->amrdma.ack_lock);
#else
  gasneti_weakatomic_increment(&cep->am_flow.ack, 0);
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
void gasnetc_poll_rcv_hca(gasnetc_hca_t *hca, int limit) {
  int count = gasneti_weakatomic_read(&hca->amrdma_rcv.count, 0);
  int limit2 = count + 1;

  /* BUG1652: full solution may require more atomicity when the polling set is changing? */

  /* Poll round-robin over the AMRDMA landing zones and the CQ */
  while (limit && limit2--) {
    /* NOTE: bug 1586 work-around requires the volatile casts */
    static int prev = 0;
    int index = *(volatile int *)(&prev); /* The associated data race is harmless */
    index = (index == 0) ? count : (index - 1);
    *(volatile int *)(&prev) = index;

    gasneti_assert(limit > 0);
    gasneti_assert(limit2 >= 0);
 
    if (index < count) { /* (index > count) is possible if another thread read a higher count */
      /* Poll for AM-over-RDMA */
      gasnetc_cep_t * cep;
      /* cep = (gasnetc_cep_t *)gasneti_atomic_ptr_read(&hca->amrdma_rcv.cep[index]); */
      cep = hca->amrdma_rcv.cep[index];
      if (cep && gasnetc_rcv_amrdma(cep)) --limit;
    } else {
      /* Poll for AM in recv CQ */
      gasnetc_rbuf_t *spare = NULL;
      (void)gasnetc_rcv_reap(hca, limit, &spare);
      if (spare) {
        gasneti_lifo_push(&hca->rbuf_freelist, spare);
      }
    }
  }
}

GASNETI_INLINE(gasnetc_do_poll)
void gasnetc_do_poll(int poll_rcv, int poll_snd) {
  if (poll_rcv) {
  #if GASNETC_IB_MAX_HCAS > 1
    /* Simple round-robin (w/ a harmless multi-thread race) */
    gasnetc_hca_t *hca;
    static volatile int index = 0;
    int tmp = index;
    index = ((tmp == 0) ? gasnetc_num_hcas : tmp) - 1;
    hca = &gasnetc_hca[tmp];
  #else
    gasnetc_hca_t *hca = &gasnetc_hca[0];
  #endif
    gasnetc_poll_rcv_hca(hca, GASNETC_RCV_REAP_LIMIT);
  #if GASNET_PSHM
    gasneti_AMPSHMPoll(0);
  #endif
  }

  if (poll_snd) {
    (void)gasnetc_snd_reap(GASNETC_SND_REAP_LIMIT);
  }
}

/* allocate a send request structure */
GASNETI_INLINE(gasnetc_get_sreq) GASNETI_MALLOC
gasnetc_sreq_t *gasnetc_get_sreq(gasnetc_sreq_opcode_t opcode GASNETC_PERTHREAD_FARG) {
  gasnetc_per_thread_t *td = GASNETC_MY_PERTHREAD();
  gasnetc_sreq_t *sreq;

  /* 1) First try the oldest sreq in our list */
  sreq = td->sreqs;
  gasneti_assert(sreq != NULL);
  if_pf (sreq->opcode != GASNETC_OP_FREE) {
    /* 2) Next poll all CQs and then check the oldest sreq again */
    int h;
    GASNETC_FOR_ALL_HCA_INDEX(h) {
      (void)gasnetc_snd_reap(1);
    }
    if_pf (sreq->opcode != GASNETC_OP_FREE) {
      /* 3) Next scan ahead, skipping over in-flight firehose misses for instance */
      do {
        sreq = sreq->next;
      } while ((sreq->opcode != GASNETC_OP_FREE) && (sreq != td->sreqs));
      if_pf (sreq->opcode != GASNETC_OP_FREE) {
        /* 4) Finally allocate more */
        gasnetc_sreq_t *head, *tail;
        gasnetc_alloc_sreqs(&head, &tail);
        tail->next = sreq->next;
        sreq = (sreq->next = head);
      }
    }
  }
  gasneti_assert(sreq->opcode == GASNETC_OP_FREE);

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
  sreq->mem_oust = NULL;
  sreq->req_oust = NULL;
  #if !GASNETC_PIN_SEGMENT
    sreq->fh_oust = NULL;
  #endif

  gasneti_assert(opcode != GASNETC_OP_FREE);
  sreq->opcode = opcode;

  return sreq;
}

/* allocate a pre-pinned bounce buffer */
GASNETI_INLINE(gasnetc_get_bbuf) GASNETI_MALLOC
gasnetc_buffer_t *gasnetc_get_bbuf(int block) {
  gasnetc_buffer_t *bbuf = NULL;

  GASNETC_TRACE_WAIT_BEGIN();
  GASNETC_STAT_EVENT(GET_BBUF);

  bbuf = gasneti_lifo_pop(&gasnetc_bbuf_freelist);
  if_pf (!bbuf) {
    gasnetc_poll_snd();
    bbuf = gasneti_lifo_pop(&gasnetc_bbuf_freelist);
    if (block) {
      while (!bbuf) {
        GASNETI_WAITHOOK();
        gasnetc_poll_snd();
        bbuf = gasneti_lifo_pop(&gasnetc_bbuf_freelist);
      }
    }
    GASNETC_TRACE_WAIT_END(GET_BBUF_STALL);
  }
  gasneti_assert((bbuf != NULL) || !block);

  return bbuf;
}

#if GASNET_TRACE || GASNET_DEBUG
GASNETI_INLINE(gasnetc_snd_validate)
void gasnetc_snd_validate(gasnetc_sreq_t *sreq, gasnetc_snd_wr_t *sr_desc, int count, const char *type) {
  int i, j;

  gasneti_assert(sreq);
  gasneti_assert(sreq->cep);
  gasneti_assert((sreq->cep - gasnetc_cep)/gasnetc_num_qps != gasneti_mynode); /* detects loopback */
  gasneti_assert(sr_desc);
  gasneti_assert(sr_desc->gasnetc_f_wr_num_sge >= 1);
  gasneti_assert(sr_desc->gasnetc_f_wr_num_sge <= GASNETC_SND_SG);
  gasneti_assert(count > 0);
  gasneti_assert(type);

  GASNETI_TRACE_PRINTF(D,("%s sreq=%p peer=%d qp=%d hca=%d\n", type, sreq,
			  gasnetc_epid2node(sreq->cep->epid),
			  gasnetc_epid2qpi(sreq->cep->epid) - 1,
			  sreq->cep->hca_index));
  for (i = 0; i < count; ++i, ++sr_desc) {
    uintptr_t r_addr = sr_desc->gasnetc_f_wr_rem_addr;

    switch (sr_desc->opcode) {
    case GASNETC_WR_SEND_WITH_IMM:
      GASNETI_TRACE_PRINTF(D,("%s op=SND\n", type));
      for (j = 0; j < sr_desc->gasnetc_f_wr_num_sge; ++j) {
        uintptr_t l_addr = sr_desc->gasnetc_f_wr_sg_list[j].addr;
        size_t    len    = sr_desc->gasnetc_f_wr_sg_list[j].gasnetc_f_sg_len;
	unsigned  lkey   = sr_desc->gasnetc_f_wr_sg_list[j].lkey;
        GASNETI_TRACE_PRINTF(D,("  %i: lkey=0x%08x len=%lu local=[%p-%p] remote=N/A\n",
			        j, lkey, (unsigned long)len,
			        (void *)l_addr, (void *)(l_addr + (len - 1))));
      }
      break;

    case GASNETC_WR_RDMA_WRITE:
      GASNETI_TRACE_PRINTF(D,("%s op=PUT rkey=0x%08x\n", type, (unsigned int)sr_desc->gasnetc_f_wr_rkey));
      for (j = 0; j < sr_desc->gasnetc_f_wr_num_sge; ++j) {
        uintptr_t l_addr = sr_desc->gasnetc_f_wr_sg_list[j].addr;
        size_t    len    = sr_desc->gasnetc_f_wr_sg_list[j].gasnetc_f_sg_len;
	unsigned  lkey   = sr_desc->gasnetc_f_wr_sg_list[j].lkey;
        GASNETI_TRACE_PRINTF(D,("  %i: lkey=0x%08x len=%lu local=[%p-%p] remote=[%p-%p]\n",
			        j, lkey, (unsigned long)len,
				(void *)l_addr, (void *)(l_addr + (len - 1)),
				(void *)r_addr, (void *)(r_addr + (len - 1))));
	r_addr += len;
      }
      break;

    case GASNETC_WR_RDMA_READ:
      GASNETI_TRACE_PRINTF(D,("%s op=GET rkey=0x%08x\n", type, (unsigned int)sr_desc->gasnetc_f_wr_rkey));
      for (j = 0; j < sr_desc->gasnetc_f_wr_num_sge; ++j) {
        uintptr_t l_addr = sr_desc->gasnetc_f_wr_sg_list[j].addr;
        size_t    len    = sr_desc->gasnetc_f_wr_sg_list[j].gasnetc_f_sg_len;
	unsigned  lkey   = sr_desc->gasnetc_f_wr_sg_list[j].lkey;
        GASNETI_TRACE_PRINTF(D,("  %i: lkey=0x%08x len=%lu local=[%p-%p] remote=[%p-%p]\n",
			        j, lkey, (unsigned long)len,
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

      for (i = 0; i < sr_desc->gasnetc_f_wr_num_sge; ++i) {
        sum += sr_desc->gasnetc_f_wr_sg_list[i].gasnetc_f_sg_len;
        gasneti_assert(sr_desc->gasnetc_f_wr_sg_list[i].gasnetc_f_sg_len != 0);
        gasneti_assert(sr_desc->gasnetc_f_wr_sg_list[i].gasnetc_f_sg_len <= gasnetc_max_msg_sz);
        gasneti_assert(sr_desc->gasnetc_f_wr_sg_list[i].gasnetc_f_sg_len <= sum); /* check for overflow of 'sum' */
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
void gasnetc_snd_post_common(gasnetc_sreq_t *sreq, gasnetc_snd_wr_t *sr_desc, int is_inline) {
  gasnetc_cep_t * const cep = sreq->cep;
  int vstat;

  /* Must be bound to a qp by now */
  gasneti_assert(cep != NULL );
  gasneti_assert(gasnetc_epid2node(sreq->epid) != gasneti_mynode);

  gasneti_assert(sreq->opcode != GASNETC_OP_FREE);
  gasneti_assert(sreq->opcode != GASNETC_OP_INVALID);

  /* Loop until space is available for 1 new entry on the CQ.
   * If we hold the last one then threads sending to ANY node will stall. */
  if_pf (!gasneti_semaphore_trydown(cep->snd_cq_sema_p)) {
    GASNETC_TRACE_WAIT_BEGIN();
    do {
      GASNETI_WAITHOOK();
      gasnetc_poll_snd();
    } while (!gasneti_semaphore_trydown(cep->snd_cq_sema_p));
    GASNETC_TRACE_WAIT_END(POST_SR_STALL_CQ);
  }

  #if GASNETC_USE_POST_LIST
    sreq->count = 1;
  #endif

  /* setup some invariant fields */
  sr_desc[0].gasnetc_f_wr_id = (uintptr_t)sreq;
#if GASNET_CONDUIT_VAPI
  sr_desc[0].comp_type = VAPI_SIGNALED;
  sr_desc[0].set_se    = 0;
  sr_desc[0].fence     = 0;
#endif

  /* Trace and debug */
  if (is_inline) {
    GASNETC_STAT_EVENT(POST_INLINE_SR);
    gasnetc_snd_validate(sreq, sr_desc, 1, "POST_INLINE_SR");
  } else {
    GASNETC_STAT_EVENT_VAL(POST_SR, sr_desc->gasnetc_f_wr_num_sge);
    gasnetc_snd_validate(sreq, sr_desc, 1, "POST_SR");
  }

  /* Post it */
#if GASNET_CONDUIT_VAPI
  if (is_inline) {
    vstat = EVAPI_post_inline_sr(cep->hca_handle, cep->qp_handle, sr_desc);
  } else {
    vstat = VAPI_post_sr(cep->hca_handle, cep->qp_handle, sr_desc);
  }
#else
  {
    struct ibv_send_wr *bad_wr;
    sr_desc->next = NULL;
    sr_desc->send_flags = is_inline ? (enum ibv_send_flags)(IBV_SEND_SIGNALED | IBV_SEND_INLINE)
                                    : IBV_SEND_SIGNALED;
    vstat = ibv_post_send(cep->qp_handle, sr_desc, &bad_wr);
  }
#endif

  if_pt (vstat == 0) {
    /* SUCCESS, the request is posted */
    return;
  } else if (GASNETC_IS_EXITING()) {
    /* disconnected by another thread */
    gasnetc_exit(0);
  } else {
    /* unexpected error */
    GASNETC_VAPI_CHECK(vstat, is_inline ? "while posting an inline send work request"
    					: "while posting a send work request");
  }
}
#define gasnetc_snd_post(x,y)		gasnetc_snd_post_common(x,y,0)
#define gasnetc_snd_post_inline(x,y)	gasnetc_snd_post_common(x,y,1)

#if GASNETC_USE_POST_LIST
/* XXX: Broken now that FAST uses firehose, too.
 * In particular we don't do anything with firehose resources if we needed to
 * split the request across multiple sreqs.  The easiest way to correlate the
 * firehose_request_t's with the sr_desc's at the current time is by examining
 * the rkeys.
 * This code has also not been kept up-to-date as mutil-qp and multi-rail
 * support was added.
 * Finally, there is no support for this in OpenIB, to which we hope to port.
 */
GASNETI_INLINE(gasnetc_snd_post_list_common)
void gasnetc_snd_post_list_common(gasnetc_sreq_t *sreq, gasnetc_snd_wr_t *sr_desc, uint32_t count) {
  gasneti_semaphore_t *sq_sema;
  gasneti_semaphore_t *cq_sema;
  uint32_t tmp;
  int i;

  /* Loop until space is available on the SQ for at least 1 new entry.
   * If we hold the last one then threads sending to the same node will stall. */
  sq_sema = &sreq->cep->sq_sema;
  tmp = gasneti_semaphore_trydown_partial(sq_sema, count);
  if_pf (!tmp) {
    GASNETC_TRACE_WAIT_BEGIN();
    do {
      GASNETI_WAITHOOK();
      gasnetc_poll_snd();
      tmp = gasneti_semaphore_trydown_partial(sq_sema, count);
    } while (!tmp);
    GASNETC_TRACE_WAIT_END(POST_SR_STALL_SQ);
  }

  /* Loop until space is available for 1 new entry on the CQ.
   * If we hold the last one then threads sending to ANY node will stall. */
  cq_sema = sreq->cep->snd_cq_sema_p;
  if_pf (!gasneti_semaphore_trydown(cq_sema)) {
    GASNETC_TRACE_WAIT_BEGIN();
    do {
      GASNETI_WAITHOOK();
      gasnetc_poll_snd();
    } while (!gasneti_semaphore_trydown(cq_sema));
    GASNETC_TRACE_WAIT_END(POST_SR_STALL_CQ);
  }

  /* setup some invariant fields */
  sreq->count = tmp;
  tmp -= 1;
  for (i = 0; i < tmp; ++i) {
    #if GASNET_DEBUG	/* unused otherwise */
      sr_desc[i].gasnetc_f_wr_id      = 0;
    #endif
    sr_desc[i].comp_type = VAPI_UNSIGNALED;
    sr_desc[i].set_se    = 0;
    sr_desc[i].fence     = 0;
  }
  sr_desc[tmp].gasnetc_f_wr_id        = (uintptr_t)sreq;
  sr_desc[tmp].comp_type = VAPI_SIGNALED;
  sr_desc[tmp].set_se    = 0;
  sr_desc[tmp].fence     = 0;
}

/* Post multiple work requests to the send queue of the given endpoint */
GASNETI_INLINE(gasnetc_snd_post_list)
void gasnetc_snd_post_list(gasnetc_sreq_t *sreq, int count, gasnetc_snd_wr_t *sr_desc) {

  gasneti_assert(sr_desc->opcode != GASNETC_WR_SEND_WITH_IMM); /* Can't (yet?) handle SENDs (AMs) */
  gasneti_assert(GASNETC_USE_FIREHOSE || (sreq->bb_buff == NULL)); /* Can't (yet?) handle BB GET/PUT */

  GASNETC_STAT_EVENT_VAL(SND_POST_LIST,count);

  do {
    gasnetc_sreq_t *next = NULL;
    int vstat;

    gasnetc_snd_post_list_common(sreq, sr_desc, count);
    gasneti_assert(sreq->count >= 1);
    gasneti_assert(sreq->count <= count);

    if_pf (sreq->count < count) {
      /* If there is not enough SQ space, so we split the request list */
      /* XXX: this is where we are most broken w.r.t. firehose resources */
      next = gasnetc_get_sreq();
      next->ep = sreq->cep;
      next->opcode =
      next->mem_oust = sreq->mem_oust;  sreq->mem_oust = NULL;
      next->req_oust = sreq->req_oust;  sreq->req_oust = NULL;
    }

    GASNETC_STAT_EVENT_VAL(POST_SR_LIST,sreq->count);
    gasnetc_snd_validate(sreq, sr_desc, sreq->count, "POST_SR_LIST");

    vstat = EVAPI_post_sr_list(sreq->cep->hca_handle, sreq->cep->qp_handle, sreq->count, sr_desc);

    if_pt (vstat == 0) {
      /* SUCCESS, the requests are posted */
    } else if (GASNETC_IS_EXITING()) {
      /* disconnected by another thread */
      gasnetc_exit(0);
    } else {
      /* unexpected error */
      GASNETC_VAPI_CHECK(vstat, "while posting multiple send work requests");
    }

    count -= sreq->count;
    sr_desc += sreq->count;
    sreq = next;
  } while (sreq != NULL);
}
#endif

static void gasnetc_rcv_thread(gasnetc_hca_hndl_t	hca_hndl,
			       gasnetc_cq_hndl_t	cq_hndl,
			       void			*context) {
#if GASNETC_IB_RCV_THREAD
  GASNETC_TRACE_WAIT_BEGIN();
  gasnetc_hca_t *hca = context;
  int vstat;

  gasneti_assert(hca_hndl == hca->handle);
  gasneti_assert(cq_hndl == hca->rcv_cq);

  (void)gasnetc_rcv_reap(hca, (int)(((unsigned int)-1)>>1), (gasnetc_rbuf_t **)&hca->rcv_thread_priv);

  vstat = VAPI_req_comp_notif(hca_hndl, cq_hndl, VAPI_NEXT_COMP);

  if_pf (vstat != VAPI_OK) {
    if (GASNETC_IS_EXITING()) {
      /* disconnected by another thread */
      gasnetc_exit(0);
    } else {
      GASNETC_VAPI_CHECK(vstat, "from VAPI_req_comp_notif()");
    }
  }

  (void)gasnetc_rcv_reap(hca, (int)(((unsigned int)-1)>>1), (gasnetc_rbuf_t **)&hca->rcv_thread_priv);
  GASNETC_TRACE_WAIT_END(RCV_THREAD_WAKE);
#else
  gasneti_fatalerror("unexpected call to gasnetc_rcv_thread");
#endif
}

/* Try to claim the next slot */
GASNETI_INLINE(gasnetc_get_amrdma_slot)
int gasnetc_get_amrdma_slot(gasnetc_cep_t *cep, size_t msg_len) {
  uint32_t send_tail;

  gasneti_assert(msg_len != 0);
  if (!cep->amrdma_rem || (msg_len > gasnetc_amrdma_limit)) {
    return -1;
  }

#if GASNETI_THREADS
  while (1) {
    send_tail = gasneti_weakatomic_read(&cep->amrdma.send_tail, 0);
    if (send_tail == gasneti_weakatomic_read(&cep->amrdma.send_head, 0)) { return -1; }
    if (gasneti_weakatomic_compare_and_swap(&cep->amrdma.send_tail, send_tail, send_tail + 1, 0)) { break; }
    GASNETI_WAITHOOK();
  }
#else
  send_tail = gasneti_weakatomic_read(&cep->amrdma.send_tail, 0);
  if (send_tail == gasneti_weakatomic_read(&cep->amrdma.send_head, 0)) { return -1; }
  gasneti_weakatomic_increment(&cep->amrdma.send_tail, 0);
#endif

  return (send_tail & gasnetc_amrdma_slot_mask);
}

GASNETI_INLINE(gasnetc_encode_amrdma)
size_t gasnetc_encode_amrdma(gasnetc_cep_t *cep, gasnetc_snd_wr_t *sr_desc, int send_slot) {
  const size_t msg_len = sr_desc->gasnetc_f_wr_sg_list[0].gasnetc_f_sg_len;

  gasneti_assert(send_slot >= 0);
  gasneti_assert(send_slot < gasnetc_amrdma_depth);

  /* Build header */
  { 
    void * const data = (void *)(uintptr_t)sr_desc->gasnetc_f_wr_sg_list[0].addr;
    gasnetc_amrdma_hdr_t * const hdr = (gasnetc_amrdma_hdr_t *)data - 1;
    const uint32_t flags = sr_desc->imm_data;

    hdr->length = hdr->length_again = msg_len;
    hdr->zeros = hdr->zeros_again = gasnetc_amrdma_zeros(flags, data, msg_len);
    hdr->immediate_data = flags;
  }

  { /* Fix up the descriptor */
    const int new_len = msg_len + sizeof(gasnetc_amrdma_hdr_t);
    sr_desc->gasnetc_f_wr_sg_list[0].addr -= sizeof(gasnetc_amrdma_hdr_t);
    sr_desc->gasnetc_f_wr_sg_list[0].gasnetc_f_sg_len = new_len;
    sr_desc->opcode = GASNETC_WR_RDMA_WRITE;
    sr_desc->gasnetc_f_wr_rem_addr = cep->amrdma_rem + (send_slot << GASNETC_AMRDMA_SZ_LG2);
    sr_desc->gasnetc_f_wr_rkey = cep->keys.amrdma_rkey;

    gasneti_assert(new_len <= GASNETC_AMRDMA_SZ);
    return (size_t)new_len;
  }
}

GASNETI_INLINE(gasnetc_ReqRepGeneric)
int gasnetc_ReqRepGeneric(gasnetc_category_t category, gasnetc_rbuf_t *token,
			  int dest, gasnet_handler_t handler,
			  void *src_addr, int nbytes, void *dst_addr,
			  int numargs, gasnetc_counter_t *mem_oust,
			  gasnetc_counter_t *req_oust, va_list argptr) {
#if GASNET_PSHM /* PSHM code handles all "local" AMs including the loopback case */
  gasneti_assert(!gasneti_pshm_in_supernode(dest));
#else
  if_pt (dest == gasneti_mynode) {
    /* Local Case */
    gasnet_handlerarg_t *args;
    int i;
    #if GASNETC_LOOPBACK_AMS_ON_STACK
      char tmp_buf[GASNETC_BUFSZ+8];
      gasnetc_buffer_t * const buf = (gasnetc_buffer_t *)GASNETI_ALIGNUP(tmp_buf, 8);
    #else
      static gasneti_lifo_head_t buf_freelist = GASNETI_LIFO_INITIALIZER;
      gasnetc_buffer_t *buf = gasneti_lifo_pop(&buf_freelist);
      if_pf (buf == NULL) {
	buf = gasneti_malloc(GASNETC_BUFSZ);
      }
    #endif

    switch (category) {
    case gasnetc_System: /* Currently System == Short.  Fall through... */
    case gasnetc_Short:
      args = buf->shortmsg.args;
      break;

    case gasnetc_Medium:
      memcpy(GASNETC_MSG_MED_DATA(buf, numargs), src_addr, nbytes);
      buf->medmsg.nBytes = nbytes;
      args = buf->medmsg.args;
      break;

    case gasnetc_Long:
      memcpy(dst_addr, src_addr, nbytes);
      buf->longmsg.destLoc = (uintptr_t)dst_addr;
      buf->longmsg.nBytes  = nbytes;
      args = buf->longmsg.args;
      break;

    default:
      gasneti_fatalerror("invalid AM category on send");
      /* NOT REACHED */
    }

    /* copy args */
    for (i=0; i < numargs; ++i) {
      args[i] = va_arg(argptr, gasnet_handlerarg_t);
    }
  
    /* process the loopback AM */
    {
      uint32_t flags = GASNETC_MSG_GENFLAGS(!token, category, numargs, handler, gasneti_mynode, 0);
      gasnetc_rbuf_t rbuf;
      rbuf.rr_sg.addr = (uintptr_t)buf;
      #if GASNET_DEBUG
        rbuf.cep = NULL;	/* ensure field is not used */
      #endif

      gasnetc_processPacket(NULL, &rbuf, flags);
    }

    #if !GASNETC_LOOPBACK_AMS_ON_STACK
      gasneti_lifo_push(&buf_freelist, buf);
    #endif
  } else
#endif /* !GASNET_PSHM */
  {
    /* Remote Case */
    gasnetc_buffer_t *buf, *buf_alloc = NULL;
    gasnet_handlerarg_t *args;
    size_t msg_len;
    int i;
    int have_flow;
    int packedlong = 0;
    int rdma_slot;
    gasnetc_epid_t epid;
    gasnetc_cep_t *cep;
    char tmp_buf[sizeof(gasnetc_am_tmp_buf_t) + 8];
  
    /* For a Reply, we must go back via the same qp that the Request came in on.
     * For a Request, we bind to a qp now to be sure everything goes on one qp.
     */
    if (token) {
      cep = token->cep;
      epid = cep->epid;
    } else {
      int qpi;
      cep = gasnetc_node2cep[dest];
#if 0
      /* Bind to a specific queue pair, selecting by largest credits */
      qpi = 0;
      if (gasnetc_num_qps > 1) {
        uint32_t best_credits;
        int i;
        /* gasnetc_poll_snd(); here? */
        best_credits = gasneti_semaphore_read(&cep[0].am_rem);
        for (i = 1; i < gasnetc_num_qps; ++i) {
	  const uint32_t tmp = gasneti_semaphore_read(&cep[i].am_rem);
	  if (tmp > best_credits) {
	    best_credits = tmp;
	    qpi = i;
	  }
        }
      }
#else
      { size_t rough_len = MIN(GASNETC_BUFSZ, 4*numargs + nbytes);
        qpi = gasnetc_epid_select_qpi(cep, dest, GASNETC_WR_SEND_WITH_IMM, rough_len);
      }
#endif
      epid = gasnetc_epid(dest, qpi);
      cep += qpi;
    }
  
    /* Reserve space for extra arguments if we *might* carry flow control
     * data.  We need to know numargs before we allocate a large enough
     * buffer, which could block and thus delay the credit update.  So, we
     * allow a race where we allocate space for the args, but end up sending
     * pointless zero values in them.
     */
    have_flow = gasneti_weakatomic_read(&cep->am_flow.credit, 0) ||
		gasneti_weakatomic_read(&cep->am_flow.ack, 0);
    if (have_flow) numargs += 1;
  
    /* Figure out msg_len so we know if we can use inline or not.
     * Also, if using firehose then Long requests may need AMs for moves.
     * Thus we MUST do any RDMA before getting credits.  It can't hurt to queue
     * the Long RDMA as early as possible even when firehose is not in use.
     */
    switch (category) {
    case gasnetc_System: /* Currently System == Short.  Fall through... */
    case gasnetc_Short:
      msg_len = GASNETC_MSG_SHORT_ARGSEND(numargs);
      if (!msg_len) msg_len = 1; /* Mellanox bug (zero-length sends) work-around */
      break;
  
    case gasnetc_Medium:
      /* XXX: When nbytes == 0 we still round up the header to 8-bytes */
      msg_len = GASNETC_MSG_MED_ARGSEND(numargs) + nbytes;
      break;
  
    case gasnetc_Long:
      msg_len = GASNETC_MSG_LONG_ARGSEND(numargs);
      /* Start moving the Long payload if possible */
      if (nbytes) {
        if ((nbytes <= gasnetc_packedlong_limit) || (!GASNETC_PIN_SEGMENT && token)) {
	  /* Small enough to send like a Medium, or a Reply when using remote firehose. */
	  msg_len += nbytes;
	  packedlong = 1;
        } else {
          /* XXX check for error returns */
          #if GASNETC_PIN_SEGMENT
	    /* Queue the RDMA.  We can count on point-to-point ordering to deliver payload before header */
            (void)gasnetc_rdma_put(epid, src_addr, dst_addr, nbytes, mem_oust, NULL);
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
	    (void)gasnetc_rdma_put_fh(epid, src_addr, dst_addr, nbytes, mem_oust, NULL, &am_oust);
	    gasnetc_counter_wait(&am_oust, 0);
          #endif
        }
      }
      break;
  
    default:
      gasneti_fatalerror("invalid AM category on send");
      /* NOT REACHED */
    }
  
    if (!token) {
      /* NEXT, get the flow-control credit needed for Requests.
       * This order ensures that we never hold the last pinned buffer
       * while spinning on the rcv queue waiting for credits.
       * Note that we need a credit even for AM-over-RDMA, to avoid
       * posting all of our rbufs to one QP.
       */
      {
        gasneti_semaphore_t * const sema = &(cep->am_rem);
        GASNETC_STAT_EVENT(GET_AMREQ_CREDIT);
        if_pf (!gasneti_semaphore_trydown(sema)) {
          GASNETC_TRACE_WAIT_BEGIN();
          do {
	    GASNETI_WAITHOOK();
            gasnetc_poll_rcv_hca(cep->hca, 1);
          } while (!gasneti_semaphore_trydown(sema));
          GASNETC_TRACE_WAIT_END(GET_AMREQ_CREDIT_STALL);
        }
      }
  
      /* Post the rbuf needed for the Reply */
      if (gasneti_semaphore_trydown(&cep->am_loc)) {
        /* We'll use one that was left over due to ACK coalescing or reply via rdma */
      } else {
        gasnetc_rbuf_t *rbuf = gasneti_lifo_pop(cep->rbuf_freelist);
        if_pf (rbuf == NULL) {
          GASNETC_TRACE_WAIT_BEGIN();
          do {
	    GASNETI_WAITHOOK();
            gasnetc_poll_rcv_hca(cep->hca, 1);
	    if (gasneti_semaphore_trydown(&cep->am_loc)) {
	      break;
	    }
	    rbuf = gasneti_lifo_pop(cep->rbuf_freelist);
          } while (rbuf == NULL);
          GASNETC_TRACE_WAIT_END(GET_AMREQ_BUFFER_STALL);
        }
        if (rbuf) {
	  gasnetc_rcv_post(cep, rbuf);
	}
      }
    }
  
    /* Try to obtain a slot for AM-over-RDMA */
    rdma_slot = gasnetc_get_amrdma_slot(cep, msg_len);
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
    if (rdma_slot >= 0) {
      if (msg_len <= gasnetc_am_inline_limit_rdma) {
        buf = (gasnetc_buffer_t *)GASNETI_ALIGNUP(tmp_buf, 8);
      } else {
	buf = (buf_alloc = gasnetc_get_bbuf(1));
      }
      buf = (gasnetc_buffer_t *)((uintptr_t)buf + sizeof(gasnetc_amrdma_hdr_t));
    } else {
      if (msg_len <= gasnetc_am_inline_limit_sndrcv) { /* sndrcv has higher limit */
        buf = (gasnetc_buffer_t *)GASNETI_ALIGNUP(tmp_buf, 8);
      } else {
	buf = (buf_alloc = gasnetc_get_bbuf(1));
      }
    }
    switch (category) {
    case gasnetc_System: /* Currently System == Short.  Fall through... */
    case gasnetc_Short:
      args = buf->shortmsg.args;
      break;
  
    case gasnetc_Medium:
      buf->medmsg.nBytes = nbytes;
      args = buf->medmsg.args;
      memcpy(GASNETC_MSG_MED_DATA(buf, numargs), src_addr, nbytes);
      break;
  
    case gasnetc_Long:
      buf->longmsg.destLoc = (uintptr_t)dst_addr;
      buf->longmsg.nBytes  = nbytes;
      if (packedlong) {
        /* Pack like a Medium */
        gasneti_assert(nbytes <= GASNETC_MAX_PACKEDLONG);
        buf->longmsg.nBytes |= 0x80000000; /* IDs the packedlong case */
        memcpy(GASNETC_MSG_LONG_DATA(buf, numargs), src_addr, nbytes);
      }
      args = buf->longmsg.args;
      break;
    }
   
    /* Assemble an array of arguments. */
    i = 0;
    if (have_flow) {
      /* acks and credits travel packed in a "prefixed" argument, remaining args are shifted */
      uint32_t acks;
      uint32_t credits;

      /* "Grab" info w/ atomic load-and-clear operations: */
#if GASNETI_THREADS
      do {
        acks = gasneti_weakatomic_read(&cep->am_flow.ack, 0);
      } while (acks && !gasneti_weakatomic_compare_and_swap(&cep->am_flow.ack, acks, 0, 0));
      do {
        credits = gasneti_weakatomic_read(&cep->am_flow.credit, 0);
      } while (credits && !gasneti_weakatomic_compare_and_swap(&cep->am_flow.credit, credits, 0, 0));
#else
      acks = gasneti_weakatomic_read(&cep->am_flow.ack, 0);
      gasneti_weakatomic_set(&cep->am_flow.ack, 0, 0);
      credits = gasneti_weakatomic_read(&cep->am_flow.credit, 0);
      gasneti_weakatomic_set(&cep->am_flow.credit, 0, 0);
#endif
      gasneti_assert(acks <= 255);
      gasneti_assert(credits <= 255);

      args[0] = credits | (acks << 8);
      i = 1;

      GASNETI_TRACE_PRINTF(C,("SND_AM_CREDITS credits=%d acks=%d\n", credits, acks));
    }
    for (/*EMPTY*/; i < numargs; ++i) {
      args[i] = va_arg(argptr, gasnet_handlerarg_t);
    }
  
    /* Add/forward optional timestamp */
    #if GASNETI_STATS_OR_TRACE
      buf->stamp = token ? ((gasnetc_buffer_t *)(uintptr_t)(token->rr_sg.addr))->stamp : GASNETI_TICKS_NOW_IFENABLED(C);
    #endif
  
    /* send the AM */
    {
      GASNETC_PERTHREAD_LOOKUP;	/* XXX: Reply could hide this in the token */
      GASNETC_DECL_SR_DESC(sr_desc, 1, 1);
      gasnetc_sreq_t *sreq;

      sr_desc->imm_data   = GASNETC_MSG_GENFLAGS(!token, category, numargs, handler,
						 gasneti_mynode, have_flow);
      sr_desc->opcode     = GASNETC_WR_SEND_WITH_IMM;
      sr_desc->gasnetc_f_wr_num_sge = 1;
      sr_desc->gasnetc_f_wr_sg_list[0].addr             = (uintptr_t)buf;
      sr_desc->gasnetc_f_wr_sg_list[0].gasnetc_f_sg_len = msg_len;
      sr_desc->gasnetc_f_wr_sg_list[0].lkey             = GASNETC_SND_LKEY(cep);
  
      sreq = gasnetc_get_sreq(GASNETC_OP_AM GASNETC_PERTHREAD_PASS);
      sreq->am_buff = buf_alloc;
      if_pf (req_oust) {
        gasnetc_counter_inc(req_oust);
        sreq->req_oust = req_oust;
        sreq->opcode = GASNETC_OP_AM_BLOCK;
      }
  
      (void)gasnetc_bind_cep(epid, sreq, GASNETC_WR_SEND_WITH_IMM, msg_len);

      if (rdma_slot >= 0) {
        msg_len = gasnetc_encode_amrdma(sreq->cep, sr_desc, rdma_slot);
        gasneti_assert((msg_len <= gasnetc_inline_limit) || (buf_alloc != NULL));
      }

      gasnetc_snd_post_common(sreq, sr_desc, (msg_len <= gasnetc_inline_limit));
    }
  } 

  GASNETI_RETURN(GASNET_OK);
}

#if GASNETC_PIN_SEGMENT
/*
 * ###############################################################
 * Static helper functions for RDMA when the segment is pre-pinned
 * ###############################################################
 */

/* Test if a given (addr, len) is in the local GASNet segment or not.
 * Returns non-zero if starting address is outside the segment
 * and adjusts len to describe a region that is fully out of segment.
 * Len is unchanged if start is in the segment.
 */
GASNETI_INLINE(gasnetc_unpinned)
int gasnetc_unpinned(uintptr_t start, size_t *len_p) {
  size_t len = *len_p;
  uintptr_t end = start + (len - 1);

  if_pt ((start >= gasnetc_seg_start) && (end <= gasnetc_seg_end)) {
    /* FULLY IN */
    return 0;
  }

  if_pt ((start > gasnetc_seg_end) || (end < gasnetc_seg_start)) {
    /* FULLY OUT */
    return 1;
  }

  /* Partials: */
  if (start < gasnetc_seg_start) {
    /* Starts OUT, ends IN */
    *len_p = gasnetc_seg_start - start;
    return 1;
  } else {
    gasneti_assert(end > gasnetc_seg_end);
    return 0;
  }
}

/* Assemble and post a bounce-buffer PUT or GET */
GASNETI_INLINE(gasnetc_bounce_common)
void gasnetc_bounce_common(gasnetc_epid_t epid, int rkey_index, uintptr_t rem_addr, size_t len, gasnetc_sreq_t *sreq, gasnetc_wr_opcode_t op) {
  GASNETC_DECL_SR_DESC(sr_desc, GASNETC_SND_SG, 1);
  gasnetc_cep_t *cep;

  sr_desc->opcode      = op;
  sr_desc->gasnetc_f_wr_num_sge  = 1;
  sr_desc->gasnetc_f_wr_rem_addr = rem_addr;
  sr_desc->gasnetc_f_wr_sg_list[0].addr = (uintptr_t)sreq->bb_buff;
  sr_desc->gasnetc_f_wr_sg_list[0].gasnetc_f_sg_len = len;

  cep = gasnetc_bind_cep(epid, sreq, op, len);
  sr_desc->gasnetc_f_wr_rkey = GASNETC_SEG_RKEY(cep, rkey_index);
  sr_desc->gasnetc_f_wr_sg_list[0].lkey = GASNETC_SND_LKEY(cep);

  gasnetc_snd_post(sreq, sr_desc);
}

/* Assemble and post a zero-copy PUT or GET using either the seg_reg table or
 * firehose to obtain the lkeys.  Both cases delay the bind to a qp until the
 * total xfer len is known.
 */
GASNETI_INLINE(gasnetc_zerocp_common)
size_t gasnetc_zerocp_common(gasnetc_epid_t epid, int rkey_index, uintptr_t loc_addr, uintptr_t rem_addr, size_t len, gasnetc_sreq_t *sreq, gasnetc_wr_opcode_t op) {
  GASNETC_DECL_SR_DESC(sr_desc, GASNETC_SND_SG, 1);
  gasnetc_cep_t *cep;
  size_t remain, count;
  int seg;

  sr_desc->opcode = op;
  sr_desc->gasnetc_f_wr_rem_addr = rem_addr;

  if_pf (!gasnetc_unpinned(loc_addr, &len)) {
    /* loc_addr is in-segment */
    const uintptr_t end = loc_addr + (len - 1);
    const int base = (loc_addr - gasnetc_seg_start) >> gasnetc_pin_maxsz_shift;
    remain = len;
    sreq->fh_count = 0;
    for (seg = 0; remain && (seg < GASNETC_SND_SG); ++seg) {
      const int index = base + seg;
      gasneti_assert(index >= 0);
      gasneti_assert(index < gasnetc_seg_reg_count);
      gasneti_assert(index == (loc_addr - gasnetc_seg_start) >> gasnetc_pin_maxsz_shift);

      /* Note seg_reg boundaries are HCA-independent */
      if (end > gasnetc_hca[0].seg_reg[index].end) {
        count = (gasnetc_hca[0].seg_reg[index].end - loc_addr) + 1;
      } else {
	count = remain;
      }
      sr_desc->gasnetc_f_wr_sg_list[seg].addr = loc_addr;
      sr_desc->gasnetc_f_wr_sg_list[seg].gasnetc_f_sg_len = count;
      sr_desc->gasnetc_f_wr_num_sge = seg + 1;

      loc_addr += count;
      remain -= count;
    }
    gasneti_assert(sr_desc->gasnetc_f_wr_num_sge > 0);
    gasneti_assert(remain < len);
    len -= remain;
    cep = gasnetc_bind_cep(epid, sreq, op, len);
    for (seg = 0; seg < sr_desc->gasnetc_f_wr_num_sge; ++seg) {
      /* Xlate index to actual lkey */
      sr_desc->gasnetc_f_wr_sg_list[seg].lkey = GASNETC_SEG_LKEY(cep, base+seg);
    }
  } else {
    const firehose_request_t *fh_loc = gasnetc_fh_aligned_local_pin(loc_addr, len);
    remain = len;
    for (seg = 0; fh_loc != NULL; ++seg) {
      sreq->fh_ptr[seg] = fh_loc;
      sreq->fh_count = seg + 1;
      count = MIN(remain, (fh_loc->addr + fh_loc->len - loc_addr));
      sr_desc->gasnetc_f_wr_sg_list[seg].addr = loc_addr;
      sr_desc->gasnetc_f_wr_sg_list[seg].gasnetc_f_sg_len = count;
      loc_addr += count;
      remain -= count;
      if (!remain || seg == (GASNETC_SND_SG-1)) {
	break; /* End of xfer or sg list */
      }

      /* We hold a local firehose already, we can only 'try' or risk deadlock */
      fh_loc = firehose_try_local_pin(loc_addr, 1, NULL);
    }
    gasneti_assert(sreq->fh_count > 0);
    sr_desc->gasnetc_f_wr_num_sge = sreq->fh_count;
    gasneti_assert(remain < len);
    len -= remain;
    cep = gasnetc_bind_cep(epid, sreq, op, len);
    for (seg = 0; seg < sr_desc->gasnetc_f_wr_num_sge; ++seg) {
      /* Xlate to actual lkeys */
      sr_desc->gasnetc_f_wr_sg_list[seg].lkey = GASNETC_FH_LKEY(cep, sreq->fh_ptr[seg]);
    }
  }

  sr_desc->gasnetc_f_wr_rkey = GASNETC_SEG_RKEY(cep, rkey_index);

  gasnetc_snd_post(sreq, sr_desc);

  gasneti_assert(len > 0);
  return len;
}

GASNETI_INLINE(gasnetc_get_rkey_index)
int gasnetc_get_rkey_index(const gasnetc_epid_t epid, uintptr_t start, size_t *len_p) {
#if GASNET_ALIGNED_SEGMENTS
  const uintptr_t segbase = gasnetc_seg_start;
#else
  const gasnet_seginfo_t *seginfo = &gasneti_seginfo[gasnetc_epid2node(epid)];
  const uintptr_t segbase = (uintptr_t)seginfo->addr;
  const uintptr_t seglen  = (uintptr_t)seginfo->size;
#endif
  size_t len = *len_p;
  uintptr_t end = start + (len - 1);
  uintptr_t tmp;
  int index;

  gasneti_assert(start >= segbase);

  /* Compute in which region of the segment the address lies */
  index = (start - segbase) >> gasnetc_pin_maxsz_shift;
  gasneti_assert(index >= 0);
  gasneti_assert(index < gasnetc_max_regs);

  /* Now trim length to the end of the region */
#if GASNET_ALIGNED_SEGMENTS
  /* gasnetc_seg_ends values are absolute */
  tmp = gasnetc_seg_ends[index];
#else
  /* gasnetc_seg_ends values are relative */
  tmp = MIN(gasnetc_seg_ends[index], seglen) + segbase;
#endif
  if (end > tmp) {
    *len_p = (tmp - start) + 1;
  }
  gasneti_assert(((start + (*len_p-1) - segbase) >> gasnetc_pin_maxsz_shift) == index);

  return index;
}

/* Helper for rdma puts: inline send case */
static void gasnetc_do_put_inline(const gasnetc_epid_t epid, int rkey_index,
                                  uintptr_t src, uintptr_t dst, size_t nbytes,
                                  gasnetc_counter_t *req_oust
				  GASNETC_PERTHREAD_FARG) {
  GASNETC_DECL_SR_DESC(sr_desc, 1, 1);
  gasnetc_cep_t *cep;
  gasnetc_sreq_t *sreq;

  GASNETI_TRACE_EVENT_VAL(C, RDMA_PUT_INLINE, nbytes);

  gasneti_assert(nbytes != 0);
  gasneti_assert(nbytes <= gasnetc_inline_limit);

  sreq = gasnetc_get_sreq(GASNETC_OP_PUT_INLINE GASNETC_PERTHREAD_PASS);
  sreq->fh_count = 0;
  if (req_oust) {
    gasnetc_counter_inc(req_oust);
    sreq->req_oust = req_oust;
  }

  sr_desc->opcode      = GASNETC_WR_RDMA_WRITE;
  sr_desc->gasnetc_f_wr_num_sge  = 1;
  sr_desc->gasnetc_f_wr_rem_addr = dst;
  sr_desc->gasnetc_f_wr_sg_list[0].addr             = src;
  sr_desc->gasnetc_f_wr_sg_list[0].gasnetc_f_sg_len = nbytes;

  cep = gasnetc_bind_cep(epid, sreq, GASNETC_WR_RDMA_WRITE, nbytes);
  sr_desc->gasnetc_f_wr_rkey = GASNETC_SEG_RKEY(cep, rkey_index);

  gasnetc_snd_post_inline(sreq, sr_desc);
}
      
/* Helper for rdma puts: bounce buffer case */
static void gasnetc_do_put_bounce(const gasnetc_epid_t epid, int rkey_index,
                                  uintptr_t src, uintptr_t dst, size_t nbytes,
                                  gasnetc_counter_t *req_oust
				  GASNETC_PERTHREAD_FARG) {
  GASNETI_TRACE_EVENT_VAL(C, RDMA_PUT_BOUNCE, nbytes);

  gasneti_assert(nbytes != 0);

  do {
    gasnetc_sreq_t * const sreq = gasnetc_get_sreq(GASNETC_OP_PUT_BOUNCE GASNETC_PERTHREAD_PASS);
    const size_t count = MIN(GASNETC_BUFSZ, nbytes);

    sreq->bb_buff = gasnetc_get_bbuf(1);
    memcpy(sreq->bb_buff, (void *)src, count);
    if (req_oust) {
      gasnetc_counter_inc(req_oust);
      sreq->req_oust = req_oust;
    }

    gasnetc_bounce_common(epid, rkey_index, dst, count, sreq, GASNETC_WR_RDMA_WRITE);

    src += count;
    dst += count;
    nbytes -= count;
  } while (nbytes);
}

/* Helper for rdma puts: zero copy case */
static void gasnetc_do_put_zerocp(const gasnetc_epid_t epid, int rkey_index,
                                  uintptr_t src, uintptr_t dst, size_t nbytes,
                                  gasnetc_counter_t *mem_oust, gasnetc_counter_t *req_oust
				  GASNETC_PERTHREAD_FARG) {
  GASNETI_TRACE_EVENT_VAL(C, RDMA_PUT_ZEROCP, nbytes);

  gasneti_assert(nbytes != 0);

  /* loop over local pinned regions */
  do {
    gasnetc_sreq_t * const sreq = gasnetc_get_sreq(GASNETC_OP_PUT_ZEROCP GASNETC_PERTHREAD_PASS);
    size_t count;

    /* The init or the sync (or neither) might wait on completion, but never both */
    if (mem_oust != NULL) {
      gasnetc_counter_inc(mem_oust);
      sreq->mem_oust = mem_oust;
    } else if (req_oust != NULL) {
      gasnetc_counter_inc(req_oust);
      sreq->req_oust = req_oust;
    }

    count = gasnetc_zerocp_common(epid, rkey_index, src, dst, nbytes, sreq, GASNETC_WR_RDMA_WRITE);
    gasneti_assert(count <= nbytes);

    src += count;
    dst += count;
    nbytes -= count;
  } while (nbytes);
}

#if GASNETC_FH_OPTIONAL /* Only available if Firehose has been disabled */
/* Helper for rdma gets: bounce buffer case */
static void gasnetc_do_get_bounce(const gasnetc_epid_t epid, int rkey_index,
                                  uintptr_t src, uintptr_t dst, size_t nbytes,
                                  gasnetc_counter_t *req_oust
				  GASNETC_PERTHREAD_FARG) {
  GASNETI_TRACE_EVENT_VAL(C, RDMA_GET_BOUNCE, nbytes);

  gasneti_assert(nbytes != 0);
  gasneti_assert(req_oust != NULL);

  do {
    gasnetc_sreq_t * const sreq = gasnetc_get_sreq(GASNETC_OP_GET_BOUNCE GASNETC_PERTHREAD_PASS);
    const size_t count = MIN(GASNETC_BUFSZ, nbytes);

    sreq->bb_addr  = (void *)dst;
    sreq->bb_len   = count;
    sreq->bb_buff  = gasnetc_get_bbuf(1);
    sreq->req_oust = req_oust;
    gasnetc_counter_inc(req_oust);

    gasnetc_bounce_common(epid, rkey_index, src, count, sreq, GASNETC_WR_RDMA_READ);

    src += count;
    dst += count;
    nbytes -= count;
  } while (nbytes);
}
#endif

/* Helper for rdma gets: zero copy case */
static void gasnetc_do_get_zerocp(const gasnetc_epid_t epid, int rkey_index,
                                  uintptr_t src, uintptr_t dst, size_t nbytes,
                                  gasnetc_counter_t *req_oust
				  GASNETC_PERTHREAD_FARG) {
  GASNETI_TRACE_EVENT_VAL(C, RDMA_GET_ZEROCP, nbytes);

  gasneti_assert(nbytes != 0);
  gasneti_assert(req_oust != NULL);

  /* loop over local pinned regions */
  do {
    gasnetc_sreq_t * const sreq = gasnetc_get_sreq(GASNETC_OP_GET_ZEROCP GASNETC_PERTHREAD_PASS);
    size_t count;

    sreq->req_oust = req_oust;
    gasnetc_counter_inc(req_oust);

    count = gasnetc_zerocp_common(epid, rkey_index, dst, src, nbytes, sreq, GASNETC_WR_RDMA_READ);
    gasneti_assert(count <= nbytes);

    src += count;
    dst += count;
    nbytes -= count;
  } while (nbytes);
}

#else /* !GASNETC_PIN_SEGMENT */
/*
 * ###################################################################
 * Static helper functions for RDMA when the segment is NOT pre-pinned
 * ###################################################################
 */
GASNETI_INLINE(gasnetc_fh_put_inline)
void gasnetc_fh_put_inline(gasnetc_sreq_t *sreq) {
  GASNETC_DECL_SR_DESC(sr_desc, 1, 1);
  const firehose_request_t * const fh_rem = sreq->fh_ptr[0];
  size_t len = sreq->fh_len;
  gasnetc_counter_t *mem_oust;
  gasnetc_cep_t *cep;

  gasneti_assert(fh_rem != NULL);
  gasneti_assert(sreq->fh_rem_addr >= fh_rem->addr);
  gasneti_assert(sreq->fh_rem_addr + (len - 1) <= fh_rem->addr + (fh_rem->len - 1));

  sr_desc->opcode      = GASNETC_WR_RDMA_WRITE;
  sr_desc->gasnetc_f_wr_num_sge  = 1;
  sr_desc->gasnetc_f_wr_rem_addr = sreq->fh_rem_addr;
  sr_desc->gasnetc_f_wr_sg_list[0].addr = sreq->fh_loc_addr;
  sr_desc->gasnetc_f_wr_sg_list[0].gasnetc_f_sg_len = len;

  mem_oust = sreq->mem_oust;
  sreq->mem_oust = NULL;

  cep = gasnetc_bind_cep(sreq->epid, sreq, GASNETC_WR_RDMA_WRITE, len);
  sr_desc->gasnetc_f_wr_rkey = GASNETC_FH_RKEY(cep, fh_rem);

  gasnetc_snd_post_inline(sreq, sr_desc);

  gasnetc_counter_dec_if_pf(mem_oust); /* The inline put already copied it */
}

GASNETI_INLINE(gasnetc_fh_put_bounce)
void gasnetc_fh_put_bounce(gasnetc_sreq_t *orig_sreq) {
  GASNETC_PERTHREAD_LOOKUP;
  GASNETC_DECL_SR_DESC(sr_desc, 1, 1);
  const firehose_request_t * const fh_rem = orig_sreq->fh_ptr[0];
  gasnetc_epid_t epid = orig_sreq->epid;
  size_t nbytes = orig_sreq->fh_len;
  gasnetc_cep_t *cep;
  uintptr_t src = orig_sreq->fh_loc_addr;
  uintptr_t dst = orig_sreq->fh_rem_addr;
  gasnetc_counter_t *mem_oust;

  gasneti_assert(nbytes != 0);
  gasneti_assert(orig_sreq->fh_rem_addr >= fh_rem->addr);
  gasneti_assert(orig_sreq->fh_rem_addr + (nbytes - 1) <= fh_rem->addr + (fh_rem->len - 1));

  /* Use full bounce buffers until just one buffer worth of data remains */
  while (nbytes > GASNETC_BUFSZ) {
    gasnetc_sreq_t * const sreq = gasnetc_get_sreq(GASNETC_OP_PUT_BOUNCE GASNETC_PERTHREAD_PASS);
    sreq->fh_bbuf = gasnetc_get_bbuf(1);
    memcpy(sreq->fh_bbuf, (void *)src, GASNETC_BUFSZ);
    sreq->fh_count = 0;

    sr_desc->opcode      = GASNETC_WR_RDMA_WRITE;
    sr_desc->gasnetc_f_wr_num_sge  = 1;
    sr_desc->gasnetc_f_wr_rem_addr = dst;
    sr_desc->gasnetc_f_wr_sg_list[0].addr = (uintptr_t)sreq->fh_bbuf;
    sr_desc->gasnetc_f_wr_sg_list[0].gasnetc_f_sg_len  = GASNETC_BUFSZ;

    cep = gasnetc_bind_cep(epid, sreq, GASNETC_WR_RDMA_WRITE, GASNETC_BUFSZ);
    sr_desc->gasnetc_f_wr_rkey = GASNETC_FH_RKEY(cep, fh_rem);
    sr_desc->gasnetc_f_wr_sg_list[0].lkey = GASNETC_SND_LKEY(cep);

    /* Send all ops on same qp to get point-to-point ordering for proper fh_release() */
    epid = sreq->epid;

    gasnetc_snd_post(sreq, sr_desc);

    src += GASNETC_BUFSZ;
    dst += GASNETC_BUFSZ;
    nbytes -= GASNETC_BUFSZ;
  }

  /* Send out the last buffer w/ the original resource */
  gasneti_assert(nbytes <= GASNETC_BUFSZ);

  mem_oust = orig_sreq->mem_oust;
  orig_sreq->mem_oust = NULL;

  orig_sreq->fh_bbuf = gasnetc_get_bbuf(1);
  memcpy(orig_sreq->fh_bbuf, (void *)src, nbytes);
  gasnetc_counter_dec_if_pf(mem_oust);

  sr_desc->opcode      = GASNETC_WR_RDMA_WRITE;
  sr_desc->gasnetc_f_wr_num_sge  = 1;
  sr_desc->gasnetc_f_wr_rem_addr = dst;
  sr_desc->gasnetc_f_wr_sg_list[0].addr = (uintptr_t)orig_sreq->fh_bbuf;
  sr_desc->gasnetc_f_wr_sg_list[0].gasnetc_f_sg_len  = nbytes;

  cep = gasnetc_bind_cep(epid, orig_sreq, GASNETC_WR_RDMA_WRITE, nbytes);
  sr_desc->gasnetc_f_wr_rkey = GASNETC_FH_RKEY(cep, fh_rem);
  sr_desc->gasnetc_f_wr_sg_list[0].lkey = GASNETC_SND_LKEY(cep);

  gasnetc_snd_post(orig_sreq, sr_desc);
}

GASNETI_INLINE(gasnetc_fh_post)
void gasnetc_fh_post(gasnetc_sreq_t *sreq, gasnetc_wr_opcode_t op) {
  GASNETC_DECL_SR_DESC(sr_desc, GASNETC_SND_SG, 1);
  gasnetc_sge_t *sg_entry;
  gasnetc_cep_t *cep;
  uintptr_t loc_addr;
  size_t remain;
  int i;

  gasneti_assert(sreq->fh_count >= 2);
  gasneti_assert(sreq->fh_count <= GASNETC_MAX_FH);
  gasneti_assert(sreq->fh_ptr[0] != NULL);
  gasneti_assert(sreq->fh_ptr[1] != NULL);

  sr_desc->opcode = op;
  sr_desc->gasnetc_f_wr_rem_addr = sreq->fh_rem_addr;
  sr_desc->gasnetc_f_wr_num_sge = sreq->fh_count - 1;

  remain = sreq->fh_len;
  loc_addr = sreq->fh_loc_addr;
  sg_entry = sr_desc->gasnetc_f_wr_sg_list;

  cep = gasnetc_bind_cep(sreq->epid, sreq, op, sreq->fh_len);
  sr_desc->gasnetc_f_wr_rkey = GASNETC_FH_RKEY(cep, sreq->fh_ptr[0]);

  for (i = 1; i < sreq->fh_count; ++i) {
    const firehose_request_t *fh_req = sreq->fh_ptr[i];
    uintptr_t next = fh_req->addr + fh_req->len;
    size_t nbytes = MIN(remain, (next - loc_addr));

    gasneti_assert(loc_addr < next);
    gasneti_assert(remain > 0);
    gasneti_assert(nbytes > 0);

    sg_entry->addr = loc_addr;
    sg_entry->gasnetc_f_sg_len = nbytes;
    sg_entry->lkey = GASNETC_FH_LKEY(cep, fh_req);

    ++sg_entry;
    remain -= nbytes;
    loc_addr += nbytes;
  }
  gasneti_assert(remain == 0);

  gasnetc_snd_post(sreq, sr_desc);
}

static void gasnetc_fh_do_put(gasnetc_sreq_t *sreq) {
  gasnetc_counter_t * const am_oust = sreq->fh_oust;

  switch (sreq->opcode) {
    case GASNETC_OP_PUT_INMOVE:
      /* All done in the AM.  Complete the sreq here since snd_reap will never see it. */
      gasneti_assert(sreq->fh_len == 0);
      gasnetc_counter_dec_if(sreq->req_oust);
      gasneti_assert(sreq->fh_count > 0);
      firehose_release(sreq->fh_ptr, sreq->fh_count);
      sreq->opcode = GASNETC_OP_FREE;
      break;

    case GASNETC_OP_PUT_INLINE:
      gasneti_assert(sreq->fh_len > 0);
      GASNETI_TRACE_EVENT_VAL(C, RDMA_PUT_INLINE, sreq->fh_len);
      gasnetc_fh_put_inline(sreq);
      break;

    case GASNETC_OP_PUT_BOUNCE:
      gasneti_assert(sreq->fh_len > 0);
      GASNETI_TRACE_EVENT_VAL(C, RDMA_PUT_BOUNCE, sreq->fh_len);
      gasnetc_fh_put_bounce(sreq);
      break;

    case GASNETC_OP_PUT_ZEROCP:
      gasneti_assert(sreq->fh_len > 0);
      GASNETI_TRACE_EVENT_VAL(C, RDMA_PUT_ZEROCP, sreq->fh_len);
      gasnetc_fh_post(sreq, GASNETC_WR_RDMA_WRITE);
      break;

    default:
      gasneti_fatalerror("invalid opcode in sreq");
  }

  gasnetc_counter_dec_if_pf(am_oust);
}

#define gasnetc_sreq_is_ready(sreq) \
  gasneti_weakatomic_decrement_and_test(&((sreq)->fh_ready), GASNETI_ATOMIC_REL)

static void gasnetc_fh_put_cb(void *context, const firehose_request_t *fh_rem, int allLocalHit) {
  gasnetc_sreq_t *sreq = context;

  gasneti_assert(fh_rem != NULL);
  sreq->fh_ptr[0] = fh_rem;

  if (gasnetc_sreq_is_ready(sreq)) {
    gasnetc_fh_do_put(sreq);
  }
}

static void gasnetc_fh_do_get(gasnetc_sreq_t *sreq) {
  GASNETI_TRACE_EVENT_VAL(C, RDMA_GET_ZEROCP, sreq->fh_len);
  gasnetc_fh_post(sreq, GASNETC_WR_RDMA_READ);
}

static void gasnetc_fh_get_cb(void *context, const firehose_request_t *fh_rem, int allLocalHit) {
  gasnetc_sreq_t *sreq = context;

  sreq->fh_ptr[0] = fh_rem;

  if (gasnetc_sreq_is_ready(sreq)) {
    gasnetc_fh_do_get(sreq);
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

    return offsetof(firehose_remotecallback_args_t, data[0]) + len;
}

GASNETI_INLINE(gasnetc_fh_put_helper)
size_t gasnetc_fh_put_helper(gasnet_node_t node, gasnetc_sreq_t *sreq,
		          uintptr_t loc_addr, uintptr_t rem_addr, size_t len) {
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
    gasneti_weakatomic_set(&sreq->fh_ready, 2, 0);
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
	((sreq->mem_oust != NULL) && (len <= gasnetc_bounce_limit))) {
    sreq->fh_count = 1; /* Just the remote one */
  } else {
    len = gasnetc_get_local_fh(sreq, loc_addr, len);
  }

  if_pf (len <= putinmove) {
    /* AM is carrying at least as much as we could pin locally */
    len = putinmove;
    sreq->fh_len = 0;
    sreq->opcode = GASNETC_OP_PUT_INMOVE;
    sreq->mem_oust = NULL; /* Already fully copied in AM payload */
    gasnetc_counter_inc_if(sreq->req_oust);
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
        gasnetc_counter_inc_if(sreq->mem_oust);
      } else { /* Memory will be copied synchronously before return */
	sreq->mem_oust = NULL;
      }
      gasnetc_counter_inc_if(sreq->req_oust);
    } else if ((nbytes <= gasnetc_bounce_limit) && (sreq->mem_oust != NULL)) {
      /* Bounce buffer use for non-bulk puts (upto a limit) */
      sreq->opcode = GASNETC_OP_PUT_BOUNCE;
      if_pf (fh_rem == NULL) { /* Memory will be copied asynchronously */
        gasnetc_counter_inc(sreq->mem_oust);
      } else { /* Memory will be copied synchronously before return */
	sreq->mem_oust = NULL;
      }
      gasnetc_counter_inc_if(sreq->req_oust);
    } else {
      /* Use the local firehose(s) obtained earlier */
      sreq->opcode = GASNETC_OP_PUT_ZEROCP;
      /* The init or the sync (or neither) might wait on completion, but never both */
      if (sreq->mem_oust != NULL) {
        gasnetc_counter_inc(sreq->mem_oust);
        sreq->req_oust = NULL;
      } else if (sreq->req_oust != NULL) {
        gasnetc_counter_inc(sreq->req_oust);
      }
    }
  }
  gasneti_assert(sreq->opcode != GASNETC_OP_INVALID);

  if ((fh_rem != NULL) || gasnetc_sreq_is_ready(sreq)) {
    gasnetc_fh_do_put(sreq);
  }

  gasneti_assert(len >= putinmove);
  gasneti_assert(len > 0);
  return len;
}

GASNETI_INLINE(gasnetc_fh_get_helper)
size_t gasnetc_fh_get_helper(gasnet_node_t node, gasnetc_sreq_t *sreq,
		          uintptr_t loc_addr, uintptr_t rem_addr, size_t len) {
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
    gasneti_weakatomic_set(&sreq->fh_ready, 2, 0);
    len = gasnetc_fh_aligned_len(rem_addr, len);
    (void)firehose_remote_pin(node, rem_addr, len, 0, NULL,
			      NULL, &gasnetc_fh_get_cb, sreq);
  }

  len = sreq->fh_len = gasnetc_get_local_fh(sreq, loc_addr, len);

  if ((fh_rem != NULL) || gasnetc_sreq_is_ready(sreq)) {
    gasnetc_fh_do_get(sreq);
  }

  gasneti_assert(len > 0);
  return len;
}
#endif

/* ------------------------------------------------------------------------------------ *
 *  Externally visible functions                                                        *
 * ------------------------------------------------------------------------------------ */

extern int gasnetc_sndrcv_init(void) {
  gasnetc_hca_t		*hca;
  gasnetc_cqe_cnt_t	act_size;
  int			vstat;
  gasnetc_buffer_t	*buf;
  gasnetc_rbuf_t	*rbuf;
  int 			padded_size, h, i;
  int			op_oust_per_qp;
  int			am_repl_per_qp;
  int			am_rqst_per_qp;
  size_t		size;

  /*
   * Check/compute limits before allocating anything
   */

  if (gasnetc_op_oust_limit == 0) { /* 0 = automatic limit computation */
    op_oust_per_qp = gasnetc_hca[0].hca_cap.gasnetc_f_max_cqe / gasnetc_hca[0].total_qps;
    for (h = 1; h < gasnetc_num_hcas; ++h) {
      op_oust_per_qp = MIN(op_oust_per_qp,
		          (gasnetc_hca[h].hca_cap.gasnetc_f_max_cqe / gasnetc_hca[h].total_qps));
    }
  } else {
    op_oust_per_qp = gasnetc_op_oust_limit / gasnetc_num_qps;
    GASNETC_FOR_ALL_HCA(hca) {
      int tmp = hca->total_qps * op_oust_per_qp;
      if (tmp > hca->hca_cap.gasnetc_f_max_cqe) {
        GASNETI_RETURN_ERRR(RESOURCE, "GASNET_NETWORKDEPTH_{PP,TOTAL} exceed HCA capabilities");
      }
    }
  }
  op_oust_per_qp = MIN(op_oust_per_qp, gasnetc_op_oust_pp);
  gasnetc_op_oust_limit = gasnetc_num_qps * op_oust_per_qp;
  GASNETI_TRACE_PRINTF(C, ("Final/effective GASNET_NETWORKDEPTH_TOTAL = %d", gasnetc_op_oust_limit));

  /* AM recv buffer allocation.  There are 5 roles a rcv buffer might fill (counts per HCA):
   * (1) Either 0 or 1 for use by the AM rcv thread.
   * (2) Exactly (gasnetc_am_oust_pp * hca->total_qps) used to catch Requests
   * (3) Upto (gasnetc_am_oust_limit * hca->qps) used to catch Replies
   * (4) Upto (gasnetc_am_credits_slack * hca->total_qps) that are assocaited with coallesced
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
   */
  am_rqst_per_qp = gasnetc_am_oust_pp * (gasneti_nodes - 1);
  if (gasnetc_am_oust_limit == 0) {
    /* 0 = automatic limit computations.
     * Find the largest possible value of (3) within HCA limits */
    am_repl_per_qp = am_rqst_per_qp; /* Replies never exceed Requests */
    GASNETC_FOR_ALL_HCA(hca) {
      int tmp = hca->hca_cap.gasnetc_f_max_cqe	/* Total CQ space */
			- (gasnetc_use_rcv_thread ? 1 : 0) /* Rcv thread's spare */
			- (am_rqst_per_qp * hca->qps); /* To catch Requests */
      tmp /= hca->qps;
      am_repl_per_qp = MIN(am_repl_per_qp, tmp);
    }
  } else {
    am_repl_per_qp = MIN((gasnetc_am_oust_limit / gasnetc_num_qps), am_rqst_per_qp);
    if (!am_repl_per_qp) {
      am_repl_per_qp = 1;	/* Ensure at least 1 */
    }
    GASNETC_FOR_ALL_HCA(hca) {
      int tmp = hca->qps * (am_rqst_per_qp + am_repl_per_qp) + (gasnetc_use_rcv_thread ? 1 : 0);
      if (tmp > hca->hca_cap.gasnetc_f_max_cqe) {
        GASNETI_RETURN_ERRR(RESOURCE, "GASNET_AM_CREDIT_{PP,TOTAL} exceed HCA capabilities");
      }
    }
  }
  gasnetc_am_oust_limit = gasnetc_num_qps * am_repl_per_qp;
  GASNETI_TRACE_PRINTF(C, ("Final/effective GASNET_AM_CREDITS_TOTAL = %d", gasnetc_am_oust_limit));

  if (gasneti_nodes > 1) {
    gasnetc_am_credits_slack = MIN(gasnetc_am_credits_slack, gasnetc_am_oust_pp - 1);
    GASNETC_FOR_ALL_HCA(hca) {
      /* Ensure credit coallescing can't deadlock a Request (bug 1418) */
      int limit = hca->qps * am_repl_per_qp - (gasneti_nodes - 1); /* might be negative */
      while (gasnetc_am_credits_slack && (gasnetc_am_credits_slack * hca->total_qps > limit)) {
	--gasnetc_am_credits_slack; /* easier to loop than get rounded arithmetic right */
      }
    }
  }
  gasnetc_am_credits_slack = MIN(gasnetc_am_credits_slack, 256);
  GASNETI_TRACE_PRINTF(C, ("Final/effective GASNET_AM_CREDITS_SLACK = %d", gasnetc_am_credits_slack));

  if (gasnetc_bbuf_limit == 0) { /* 0 = automatic limit computation */
    /* We effectively count local AMs against gasnetc_op_oust_limit for simplicity,
     * but only expect one in-flight per thread anyway. */
    gasnetc_bbuf_limit = gasnetc_op_oust_limit;
  } else {
    gasnetc_bbuf_limit = MIN(gasnetc_bbuf_limit, gasnetc_op_oust_limit);
  }
  if (gasneti_nodes == 1) {
    /* no AM or RDMA on the wire, but still need bufs for constructing AMs */
    gasnetc_bbuf_limit = gasnetc_num_qps * gasnetc_am_oust_pp;
  }
  GASNETI_TRACE_PRINTF(C, ("Final/effective GASNET_BBUF_COUNT = %d", gasnetc_bbuf_limit));

  /*
   * setup RCV resources
   */

  /* create one RCV CQ per HCA */
  GASNETC_FOR_ALL_HCA(hca) {
    int rcv_count = hca->qps * (am_rqst_per_qp + am_repl_per_qp) + (gasnetc_use_rcv_thread ? 1 : 0);
    vstat = gasnetc_create_cq(hca->handle, rcv_count, &hca->rcv_cq, &act_size);
    GASNETC_VAPI_CHECK(vstat, "from gasnetc_create_cq(rcv_cq)");
    gasneti_assert(act_size >= rcv_count);
    /* We don't set rcv_count = act_size here, as that could nearly double the memory allocated below */

    gasneti_lifo_init(&hca->amrdma_freelist);

    if (gasneti_nodes > 1) {
#if GASNET_CONDUIT_VAPI
      if (gasnetc_use_rcv_thread) {
        /* create the RCV thread */
        vstat = EVAPI_set_comp_eventh(hca->handle, hca->rcv_cq, &gasnetc_rcv_thread,
				      hca, &hca->rcv_handler);
        GASNETC_VAPI_CHECK(vstat, "from EVAPI_set_comp_eventh()");
        vstat = VAPI_req_comp_notif(hca->handle, hca->rcv_cq, VAPI_NEXT_COMP);
        GASNETC_VAPI_CHECK(vstat, "from VAPI_req_comp_notif()");
      }
#endif

      /* Allocated pinned memory for receive buffers */
      size = rcv_count * sizeof(gasnetc_buffer_t);
      buf = gasneti_mmap(size);
      if_pf (buf == MAP_FAILED) {
        buf = NULL;
      } else {
        vstat = gasnetc_pin(hca, buf, size, GASNETC_ACL_LOC_WR, &hca->rcv_reg);
        if (vstat != 0) {
	  gasneti_munmap(buf, size);
          buf = NULL;
        }
      }
      if_pf (buf == NULL) {
        (void)gasnetc_destroy_cq(hca->handle, hca->snd_cq);
        (void)gasnetc_destroy_cq(hca->handle, hca->rcv_cq);
	/* XXX: also unwind CQ and reg for previous HCAs */
        GASNETI_RETURN_ERRR(RESOURCE, "Unable to allocate pinned memory for AM recv buffers");
      }
  
      /* Allocated normal memory for receive descriptors (rbuf's) */
      padded_size = GASNETI_ALIGNUP(sizeof(gasnetc_rbuf_t), GASNETI_CACHE_LINE_BYTES);
      hca->rbuf_alloc = gasneti_malloc(rcv_count*padded_size + GASNETI_CACHE_LINE_BYTES-1);
  
      /* Initialize the rbuf's */
      gasneti_lifo_init(&hca->rbuf_freelist);
      rbuf = (gasnetc_rbuf_t *)GASNETI_ALIGNUP(hca->rbuf_alloc, GASNETI_CACHE_LINE_BYTES);
      for (i = 0; i < rcv_count; ++i) {
        rbuf->rr_is_rdma         = 0;
        rbuf->rr_desc.gasnetc_f_wr_num_sge = 1;
        rbuf->rr_desc.gasnetc_f_wr_sg_list = &rbuf->rr_sg;
        rbuf->rr_desc.gasnetc_f_wr_id      = (uintptr_t)rbuf;	/* CQE will point back to this request */
#if GASNET_CONDUIT_VAPI
        rbuf->rr_desc.opcode     = VAPI_RECEIVE;
        rbuf->rr_desc.comp_type  = VAPI_SIGNALED;
#else
        rbuf->rr_desc.next = NULL;
#endif
        rbuf->rr_sg.gasnetc_f_sg_len = GASNETC_BUFSZ;
        rbuf->rr_sg.addr             = (uintptr_t)&buf[i];
        gasneti_lifo_push(&hca->rbuf_freelist, rbuf);
  
        rbuf = (gasnetc_rbuf_t *)((uintptr_t)rbuf + padded_size);
      }
#if GASNET_CONDUIT_VAPI
      if (gasnetc_use_rcv_thread) {
        hca->rcv_thread_priv = gasneti_lifo_pop(&hca->rbuf_freelist);
        gasneti_assert(hca->rcv_thread_priv != NULL);
      }
#endif
      
      /* Initialize resources for AM-over-RDMA */
      gasneti_weakatomic_set(&hca->amrdma_rcv.count, 0, 0);
      if (gasnetc_amrdma_max_peers) {
	const int max_peers = hca->amrdma_rcv.max_peers;
	size_t alloc_size = GASNETI_PAGE_ALIGNUP(max_peers * (gasnetc_amrdma_depth << GASNETC_AMRDMA_SZ_LG2) + GASNETC_AMRDMA_PAD);
	void *buf = gasneti_mmap(alloc_size);

        if_pf (buf == MAP_FAILED) {
          buf = NULL;
        } else {
          vstat = gasnetc_pin(hca, buf, alloc_size, (gasnetc_acl_t)(GASNETC_ACL_LOC_WR | GASNETC_ACL_REM_WR), &hca->amrdma_reg);
          if (vstat != 0) {
	    gasneti_munmap(buf, size);
            buf = NULL;
          }
        }
        if_pf (buf == NULL) {
	  /* XXX: unwind here? */
	  gasneti_fatalerror("Unable to allocate pinned memory for AM-over-RDMA");
        }
	buf = (void *)((uintptr_t)buf + GASNETC_AMRDMA_PAD);
	for (i = 0; i < max_peers; ++i) {
	  gasneti_lifo_push(&hca->amrdma_freelist, buf);
	  buf = (void *)((uintptr_t)buf + (gasnetc_amrdma_depth << GASNETC_AMRDMA_SZ_LG2));
	}

        hca->amrdma_rcv.cep = gasneti_calloc(max_peers, sizeof(gasnetc_cep_t *));

        gasneti_weakatomic_set(&hca->amrdma_balance.count, 0, 0);
        hca->amrdma_balance.mask = gasnetc_amrdma_cycle ? (gasnetc_amrdma_cycle - 1) : 0;
#if GASNETI_THREADS
        gasneti_spinlock_init(&hca->amrdma_balance.lock);
#endif
        hca->amrdma_balance.floor = 1;
        hca->amrdma_balance.table = gasneti_calloc(hca->total_qps, sizeof(gasnetc_amrdma_balance_tbl_t));
      }
    }
  }

  /*
   * setup SND resources
   */

  /* create the SND CQ and associated semaphores */
  gasnetc_cq_semas = (gasneti_semaphore_t *)
	  GASNETI_ALIGNUP(gasneti_malloc(gasnetc_num_hcas*sizeof(gasneti_semaphore_t)
				  	 + GASNETI_CACHE_LINE_BYTES - 1),
			  GASNETI_CACHE_LINE_BYTES);
  GASNETC_FOR_ALL_HCA_INDEX(h) {
    hca = &gasnetc_hca[h];
    vstat = gasnetc_create_cq(hca->handle, hca->total_qps * op_oust_per_qp, &hca->snd_cq, &act_size);
    GASNETC_VAPI_CHECK(vstat, "from gasnetc_create_cq(snd_cq)");
    gasneti_assert(act_size >= hca->total_qps * op_oust_per_qp);
    /* We use actual size here, since the memory has been allocated anyway */
    gasneti_semaphore_init(&gasnetc_cq_semas[h], act_size, act_size);
  }

  /* Allocated pinned memory for AMs and bounce buffers */
  size = gasnetc_bbuf_limit * sizeof(gasnetc_buffer_t);
  buf = gasneti_mmap(size);
  if_pf (buf == MAP_FAILED) {
    buf = NULL;
  } else {
    GASNETC_FOR_ALL_HCA_INDEX(h) {
      vstat = gasnetc_pin(&gasnetc_hca[h], buf, size,
		          GASNETC_ACL_LOC_WR, &gasnetc_hca[h].snd_reg);
      if (vstat != 0) {
	for (h -= 1; h >= 0; --h) {
	  gasnetc_unpin(&gasnetc_hca[h], &gasnetc_hca[h].snd_reg);
	}
        gasneti_munmap(buf, size);
        buf = NULL;
	break;
      }
    }
  }
  if_pf (buf == NULL) {
    GASNETC_FOR_ALL_HCA(hca) {
      if (gasneti_nodes > 1) {
#if GASNET_CONDUIT_VAPI
        if (gasnetc_use_rcv_thread) {
	  vstat = EVAPI_clear_comp_eventh(hca->handle, hca->rcv_handler);
        }
#endif
        gasneti_free(hca->rbuf_alloc);
        gasnetc_unpin(hca, &hca->rcv_reg);
        gasnetc_unmap(&hca->rcv_reg);
      }
      (void)gasnetc_destroy_cq(hca->handle, hca->snd_cq);
      (void)gasnetc_destroy_cq(hca->handle, hca->rcv_cq);
      GASNETI_RETURN_ERRR(RESOURCE, "Unable to allocate pinned memory for AM/bounce buffers");
    }
  }
  for (i = 0; i < gasnetc_bbuf_limit; ++i) {
    gasneti_lifo_push(&gasnetc_bbuf_freelist, buf);
    ++buf;
  }

  gasnetc_node2cep = (gasnetc_cep_t **)
	  GASNETI_ALIGNUP(gasneti_malloc(gasneti_nodes*sizeof(gasnetc_cep_t *)
				  	 + GASNETI_CACHE_LINE_BYTES - 1),
			  GASNETI_CACHE_LINE_BYTES);

  /* Init thread-local data */
#if GASNETI_THREADS
  gasneti_threadkey_init(gasnetc_per_thread_key);
#else
  gasnetc_per_thread_init(&gasnetc_per_thread);
#endif

  return GASNET_OK;
}

extern void gasnetc_sndrcv_init_peer(gasnet_node_t node) {
  gasnetc_cep_t *cep;
  int i, j;
  
  cep = gasnetc_node2cep[node] = &(gasnetc_cep[node * gasnetc_num_qps]);

  if (node != gasneti_mynode) {
    for (i = 0; i < gasnetc_num_qps; ++i, ++cep) {
      gasnetc_hca_t *hca = cep->hca;
      cep->epid = gasnetc_epid(node, i);
      cep->rbuf_freelist = &hca->rbuf_freelist;

      /* "Cache" the local keys associated w/ this cep */
      cep->keys.rcv_lkey = hca->rcv_reg.lkey;
      cep->keys.snd_lkey = hca->snd_reg.lkey;

      /* Initialize local AM-over-RDMA info */
      gasneti_weakatomic_set(&cep->amrdma.send_head, gasnetc_amrdma_depth, 0);
      gasneti_weakatomic_set(&cep->amrdma.send_tail, 0, 0);
      gasneti_weakatomic_set(&cep->amrdma.recv_head, 0, 0);
#if GASNETI_THREADS
      gasneti_mutex_init(&cep->amrdma.ack_lock);
      cep->amrdma.ack_bits = 0;
      cep->amrdma.recv_tail = 0;
#endif
      gasneti_weakatomic_set(&cep->amrdma.eligable, 0, 0);
      cep->amrdma_loc = NULL;

      /* Prepost one rcv buffer for each possible incomming request */
      for (j = 0; j < gasnetc_am_oust_pp; ++j) {
        gasnetc_rcv_post(cep, gasneti_lifo_pop(cep->rbuf_freelist));
      }

      /* Setup semaphores/counters */
      gasneti_semaphore_init(&cep->sq_sema, gasnetc_op_oust_pp, gasnetc_op_oust_pp);
      gasneti_semaphore_init(&cep->am_rem, gasnetc_am_oust_pp, gasnetc_am_oust_pp);
      gasneti_semaphore_init(&cep->am_loc, 0, gasnetc_am_oust_pp);
      gasneti_weakatomic_set(&cep->am_flow.credit, 0, 0);
      gasneti_weakatomic_set(&cep->am_flow.ack, 0, 0);
      cep->snd_cq_sema_p = &gasnetc_cq_semas[cep->hca_index];
    }
  } else {
    /* Should never use these for loopback */
    for (i = 0; i < gasnetc_num_qps; ++i, ++cep) {
      cep->epid = gasnetc_epid(node, i);
      gasneti_semaphore_init(&cep->sq_sema, 0, 0);
      gasneti_semaphore_init(&cep->am_rem, 0, 0);
      gasneti_semaphore_init(&cep->am_loc, 0, 0);
      gasneti_weakatomic_set(&cep->am_flow.credit, 0, 0);
      gasneti_weakatomic_set(&cep->am_flow.ack, 0, 0);
      gasneti_weakatomic_set(&cep->amrdma.eligable, 0, 0);
      cep->amrdma_loc = NULL;
    }
  }
}

extern void gasnetc_sndrcv_init_misc(void) {
  gasnetc_am_inline_limit_sndrcv = MIN(gasnetc_inline_limit, sizeof(gasnetc_am_tmp_buf_t));
  gasnetc_am_inline_limit_rdma = MAX(GASNETC_AMRDMA_HDRSZ, gasnetc_am_inline_limit_sndrcv) - GASNETC_AMRDMA_HDRSZ;
#if !GASNETC_PIN_SEGMENT
  gasnetc_putinmove_limit_adjusted = gasnetc_putinmove_limit
	  				? (gasnetc_putinmove_limit + gasnetc_inline_limit)
					: 0;
#endif
}

extern void gasnetc_sndrcv_attach_peer(gasnet_node_t node) {
#if GASNETC_PIN_SEGMENT
  gasnetc_cep_t *cep = gasnetc_node2cep[node];
  int i;

  for (i = 0; i < gasnetc_num_qps; ++i, ++cep) {
    gasnetc_hca_t *hca = cep->hca;
    cep->keys.seg_reg = (node == gasneti_mynode) ? NULL : hca->seg_reg;
    cep->keys.rkeys   = (node == gasneti_mynode) ? NULL : &hca->rkeys[node * gasnetc_max_regs];
  }

  if (node == gasneti_mynode) { /* Needed exactly once */
    gasnetc_seg_ends = gasneti_malloc(gasnetc_max_regs * sizeof(uintptr_t));
    for (i = 0; i < gasnetc_max_regs; ++i) {
#if GASNET_ALIGNED_SEGMENTS
      /* gasnetc_seg_ends values are absolute */
      gasnetc_seg_ends[i] = (gasnetc_seg_start - 1) + ((uintptr_t)(i+1) << gasnetc_pin_maxsz_shift);
#else
      /* gasnetc_seg_ends values are relative */
      gasnetc_seg_ends[i] = ((uintptr_t)(i+1) << gasnetc_pin_maxsz_shift) - 1;
#endif
    }
#if GASNET_ALIGNED_SEGMENTS
    gasneti_assert(i == gasnetc_max_regs);
    if (gasnetc_seg_ends[i-1] < gasnetc_seg_start) {
      /* Fixup any wrap-around */
      gasnetc_seg_ends[i-1] = ~((uintptr_t)0);
    }
#else
    /* Fixup is in gasnetc_get_rkey_index() due to differing lengths */
#endif
  }
#else
  /* Nothing currently needed */
#endif
}

extern void gasnetc_sndrcv_fini(void) {
  gasnetc_hca_t *hca;
#if 0 /* See below */
  int vstat;
#endif

  GASNETC_FOR_ALL_HCA(hca) {
    if (gasneti_nodes > 1) {
#if GASNET_CONDUIT_VAPI
      if (gasnetc_use_rcv_thread) {
        int vstat = EVAPI_clear_comp_eventh(hca->handle, hca->rcv_handler);
        GASNETC_VAPI_CHECK(vstat, "from EVAPI_clear_comp_eventh()");
      }
#endif

      gasnetc_unpin(hca, &hca->rcv_reg);
      gasnetc_unmap(&hca->rcv_reg);
      gasnetc_unpin(hca, &hca->snd_reg);
      gasnetc_unmap(&hca->snd_reg);

      gasneti_free(hca->rbuf_alloc);
    }

#if 0
    /* SEGVs seen here w/ VAPI on lambda.hcs.ufl.edu (bug 1433) and w/ OpenIB on Jacqaurd-dev.
     * We probably need to drain the queues completely before destroying them. */
    vstat = gasnetc_destroy_cq(hca->handle, hca->rcv_cq);
    GASNETC_VAPI_CHECK(vstat, "from gasnetc_destroy_cq(rcv_cq)");
    vstat = gasnetc_destroy_cq(hca->handle, hca->snd_cq);
    GASNETC_VAPI_CHECK(vstat, "from gasnetc_destroy_cq(snd_cq)");
#endif
  }
}

extern void gasnetc_sndrcv_fini_peer(gasnet_node_t node) {
  int vstat;
  int i;

  if (node != gasneti_mynode) {
    gasnetc_cep_t *cep = gasnetc_node2cep[node];
    for (i = 0; i < gasnetc_num_qps; ++i, ++cep) {
      vstat = gasnetc_destroy_qp(cep->hca_handle, cep->qp_handle);
      GASNETC_VAPI_CHECK(vstat, "from gasnetc_destroy_qp()");
    }
  }
}

/* Just gasnetc_AMPoll w/o CHECKATTACH */
extern void gasnetc_sndrcv_poll(void) {
  gasnetc_poll_both();
}

extern void gasnetc_counter_wait_aux(gasnetc_counter_t *counter, int handler_context)
{
  const int initiated = counter->initiated;
  if (handler_context) {
    do {
      /* must not poll rcv queue in hander context */
      GASNETI_WAITHOOK();
      gasnetc_poll_snd();
    } while (initiated != gasneti_weakatomic_read(&(counter->completed), 0));
  } else {
    do {
      GASNETI_WAITHOOK();
      gasnetc_poll_both();
      GASNETI_PROGRESSFNS_RUN();
    } while (initiated != gasneti_weakatomic_read(&(counter->completed), 0));
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
extern int gasnetc_rdma_put(gasnetc_epid_t epid, void *src_ptr, void *dst_ptr, size_t nbytes, gasnetc_counter_t *mem_oust, gasnetc_counter_t *req_oust) {
  GASNETC_PERTHREAD_LOOKUP;
  uintptr_t src = (uintptr_t)src_ptr;
  uintptr_t dst = (uintptr_t)dst_ptr;

  gasneti_assert(nbytes != 0);
  
  do {
    /* Loop over contiguous pinned regions on remote end */
    size_t count = nbytes;
    const int rkey_index = gasnetc_get_rkey_index(epid, dst, &count);

    if (count <= gasnetc_inline_limit) {
      /* Use a short-cut for sends that are short enough.
       *
       * Note that we do this based only on the size of the request, without bothering to check whether
       * the caller cares about local completion, or whether zero-copy is possible.
       * We do this is because the cost of this small copy is cheaper than the alternative logic.
       */
      gasnetc_do_put_inline(epid, rkey_index, src, dst, count, req_oust GASNETC_PERTHREAD_PASS);
    } else
#if GASNETC_FH_OPTIONAL 
    if_pf (!GASNETC_USE_FIREHOSE && gasnetc_unpinned(src, &count)) {
      /* Firehose disabled.  Use bounce buffers since src is out-of-segment */
      gasnetc_do_put_bounce(epid, rkey_index, src, dst, count, req_oust GASNETC_PERTHREAD_PASS);
    } else
#endif
    if ((count <= gasnetc_bounce_limit) && (mem_oust != NULL)) {
      /* Because VAPI lacks any indication of "local" completion, the only ways to
       * implement non-bulk puts (mem_oust != NULL) are as fully blocking puts, or
       * with bounce buffers.  So, if a non-bulk put is "not too large" use bounce
       * buffers.
       */
      gasnetc_do_put_bounce(epid, rkey_index, src, dst, count, req_oust GASNETC_PERTHREAD_PASS);
    } else {
      /* Here is the general case */
      gasnetc_do_put_zerocp(epid, rkey_index, src, dst, count, mem_oust, req_oust GASNETC_PERTHREAD_PASS);
    }

    src += count;
    dst += count;
    nbytes -= count;
  } while (nbytes);

  return 0;
}

/* Perform an RDMA get
 *
 * Uses zero-copy (with firehose if the destination is not pre-pinned).
 * If firehose is disabled, then bounce buffers are used for unpinned destinations.
 */
extern int gasnetc_rdma_get(gasnetc_epid_t epid, void *src_ptr, void *dst_ptr, size_t nbytes, gasnetc_counter_t *req_oust) {
  GASNETC_PERTHREAD_LOOKUP;
  uintptr_t src = (uintptr_t)src_ptr;
  uintptr_t dst = (uintptr_t)dst_ptr;

  gasneti_assert(nbytes != 0);
  gasneti_assert(req_oust != NULL);

  do {
    /* Loop over contiguous pinned regions on remote end */
    size_t count = nbytes;
    const int rkey_index = gasnetc_get_rkey_index(epid, src, &count);

#if GASNETC_FH_OPTIONAL 
    if_pf (!GASNETC_USE_FIREHOSE && gasnetc_unpinned(dst, &count)) {
      /* Firehose disabled.  Use bounce buffers since dst is out-of-segment */
      gasnetc_do_get_bounce(epid, rkey_index, src, dst, count, req_oust GASNETC_PERTHREAD_PASS);
    } else
#endif
    {
      gasnetc_do_get_zerocp(epid, rkey_index, src, dst, count, req_oust GASNETC_PERTHREAD_PASS);
    }

    src += count;
    dst += count;
    nbytes -= count;
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
extern int gasnetc_rdma_put_fh(gasnetc_epid_t epid, void *src_ptr, void *dst_ptr, size_t nbytes, gasnetc_counter_t *mem_oust, gasnetc_counter_t *req_oust, gasnetc_counter_t *am_oust) {
  GASNETC_PERTHREAD_LOOKUP;
  uintptr_t src = (uintptr_t)src_ptr;
  uintptr_t dst = (uintptr_t)dst_ptr;

  gasneti_assert(nbytes != 0);

  do {
    gasnetc_sreq_t * const sreq = gasnetc_get_sreq(GASNETC_OP_INVALID GASNETC_PERTHREAD_PASS);
    size_t count;

    sreq->epid = epid;
    sreq->fh_bbuf = NULL;
 
    sreq->mem_oust = mem_oust;
    sreq->req_oust = req_oust;
    sreq->fh_oust = am_oust;

    count = gasnetc_fh_put_helper(epid, sreq, src, dst, nbytes);

    src += count;
    dst += count;
    nbytes -= count;
  } while (nbytes);

  gasnetc_poll_rcv(); /* Progress may depend on firehose AM Reply */
  return 0;
}

/* Perform an RDMA get */
extern int gasnetc_rdma_get(gasnetc_epid_t epid, void *src_ptr, void *dst_ptr, size_t nbytes, gasnetc_counter_t *req_oust) {
  GASNETC_PERTHREAD_LOOKUP;
  uintptr_t src = (uintptr_t)src_ptr;
  uintptr_t dst = (uintptr_t)dst_ptr;

  gasneti_assert(nbytes != 0);
  gasneti_assert(req_oust != NULL);

  do {
    gasnetc_sreq_t * const sreq = gasnetc_get_sreq(GASNETC_OP_GET_ZEROCP GASNETC_PERTHREAD_PASS);
    size_t count;

    sreq->epid = epid;
 
    sreq->req_oust = req_oust;
    gasnetc_counter_inc(req_oust);

    count = gasnetc_fh_get_helper(epid, sreq, dst, src, nbytes);

    src += count;
    dst += count;
    nbytes -= count;
  } while (nbytes);

  gasnetc_poll_rcv(); /* Progress may depend on firehose AM Reply */
  return 0;
}
#endif

/* Putv - contiguous remote dst, vector local src
 *
 * Initial naive implementation
 */
extern int gasnetc_rdma_putv(gasnetc_epid_t epid, size_t srccount, gasnet_memvec_t const srclist[], void *dst_ptr, gasnetc_counter_t *mem_oust, gasnetc_counter_t *req_oust) {
  while (srccount) {
    /* XXX: check return value for errors */
    (void)gasnetc_rdma_put(epid, srclist->addr, dst_ptr, srclist->len, mem_oust, req_oust);
    --srccount;
    ++srclist;
  }

  return 0;
}

/* Getv - contiguous remote src, vector local dst
 *
 * Initial naive implementation
 */
extern int gasnetc_rdma_getv(gasnetc_epid_t epid, void *src_ptr, size_t dstcount, gasnet_memvec_t const dstlist[], gasnetc_counter_t *req_oust) {
  while (dstcount) {
    /* XXX: check return value for errors */
    (void)gasnetc_rdma_get(epid, src_ptr, dstlist->addr, dstlist->len, req_oust);
    --dstcount;
    ++dstlist;
  }

  return 0;
}

extern int gasnetc_RequestGeneric(gasnetc_category_t category,
				  int dest, gasnet_handler_t handler,
				  void *src_addr, int nbytes, void *dst_addr,
				  int numargs, gasnetc_counter_t *mem_oust, va_list argptr) {
  /* ensure progress */
  gasnetc_poll_rcv();
  GASNETI_PROGRESSFNS_RUN();

#if GASNET_PSHM
  if_pt (gasneti_pshm_in_supernode(dest)) {
    return gasneti_AMPSHM_RequestGeneric(category, dest, handler,
                                         src_addr, nbytes, dst_addr,
                                         numargs, argptr);
  }
#endif

  return gasnetc_ReqRepGeneric(category, NULL, dest, handler,
                               src_addr, nbytes, dst_addr,
                               numargs, mem_oust, NULL, argptr);
}

extern int gasnetc_ReplyGeneric(gasnetc_category_t category,
				gasnet_token_t token, gasnet_handler_t handler,
				void *src_addr, int nbytes, void *dst_addr,
				int numargs, gasnetc_counter_t *mem_oust, va_list argptr) {
  gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)token;
  int retval;

#if GASNET_PSHM
  if_pt (gasnetc_token_is_pshm(token)) {
      return gasneti_AMPSHM_ReplyGeneric(category, token, handler,
                                         src_addr, nbytes, dst_addr,
                                         numargs, argptr);
  }
#endif

  gasneti_assert(rbuf);
  gasneti_assert(rbuf->rbuf_handlerRunning);
  gasneti_assert(GASNETC_MSG_ISREQUEST(rbuf->rbuf_flags));
  gasneti_assert(rbuf->rbuf_needReply);

  retval = gasnetc_ReqRepGeneric(category, rbuf, GASNETC_MSG_SRCIDX(rbuf->rbuf_flags), handler,
				 src_addr, nbytes, dst_addr,
				 numargs, mem_oust, NULL, argptr);

  rbuf->rbuf_needReply = 0;
  return retval;
}

extern int gasnetc_RequestSystem(gasnet_node_t dest,
			         gasnetc_counter_t *req_oust,
                                 gasnet_handler_t handler,
                                 int numargs, ...) {
  int retval;
  va_list argptr;

  gasnetc_poll_rcv();	/* ensure progress */

  GASNETC_TRACE_SYSTEM_REQUEST(dest,handler,numargs);

  va_start(argptr, numargs);
#if GASNET_PSHM
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Short, dest,
                                           handler | GASNETC_SYS_HANDLER_FLAG,
                                           0, 0, 0,
                                           numargs, argptr); 
    /* No need to touch req_oust */
  } else
#endif
  retval = gasnetc_ReqRepGeneric(gasnetc_System, NULL, dest, handler,
				 NULL, 0, NULL, numargs, NULL, req_oust, argptr);
  va_end(argptr);
  return retval;
}

extern int gasnetc_ReplySystem(gasnet_token_t token,
			       gasnetc_counter_t *req_oust,
                               gasnet_handler_t handler,
                               int numargs, ...) {
  int retval;
  va_list argptr;
  gasnet_node_t dest;

  GASNETI_SAFE_PROPAGATE(gasnet_AMGetMsgSource(token, &dest));
  GASNETC_TRACE_SYSTEM_REPLY(dest,handler,numargs);

  va_start(argptr, numargs);
#if GASNET_PSHM
  if_pt (gasnetc_token_is_pshm(token)) {
    retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Short, token,
                                         handler | GASNETC_SYS_HANDLER_FLAG,
                                         0, 0, 0,
                                         numargs, argptr);
  } else
#endif
  {
    gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)token;
    gasneti_assert(rbuf);
    retval = gasnetc_ReqRepGeneric(gasnetc_System, rbuf, dest, handler,
				   NULL, 0, NULL, numargs, NULL, req_oust, argptr);
    rbuf->rbuf_needReply = 0;
  }
  va_end(argptr);
  return retval;
}

/* ------------------------------------------------------------------------------------ */
/*
  Misc. Active Message Functions
  ==============================
*/
extern int gasnetc_AMGetMsgSource(gasnet_token_t token, gasnet_node_t *srcindex) {
  gasnet_node_t sourceid;

  GASNETI_CHECK_ERRR((!token),BAD_ARG,"bad token");
  GASNETI_CHECK_ERRR((!srcindex),BAD_ARG,"bad src ptr");

#if GASNET_PSHM
  if (gasneti_AMPSHMGetMsgSource(token, &sourceid) != GASNET_OK)
#endif
  {
    uint32_t flags = ((gasnetc_rbuf_t *)token)->rbuf_flags;

    if_pf (GASNETC_MSG_CATEGORY(flags) != gasnetc_System) {
      GASNETI_CHECKATTACH();
    }

    sourceid = GASNETC_MSG_SRCIDX(flags);
  }

  gasneti_assert(sourceid < gasneti_nodes);
  *srcindex = sourceid;
  return GASNET_OK;
}

extern int gasnetc_AMPoll(void) {
#if 0 /* Timings show peek optimization is no longer effective */
  int h, work;

  GASNETI_CHECKATTACH();

  /* XXX: multi-rail must either peek all, or give up on the peek optimization */
  work = 0;
  CQ_LOCK;
  GASNETC_FOR_ALL_HCA_INDEX(h) {
    if ((gasnetc_peek_rcv_cq(&gasnetc_hca[h], 1) == 0) ||
        (gasnetc_peek_snd_cq(&gasnetc_hca[h], 1) == 0)) {
      work = 1;
      break;
    }
  }
  CQ_UNLOCK;

  if_pf (work) {
    gasnetc_poll_both();
  }
#else
  GASNETI_CHECKATTACH();
  gasnetc_poll_both();
#endif

  return GASNET_OK;
}
