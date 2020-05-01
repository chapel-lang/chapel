#ifndef GASNET_ARIES_H
#define GASNET_ARIES_H

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <pmi_cray.h>
#include <gni_pub.h>
#include "gasnet_internal.h"
#include "gasnet_core_internal.h"
#include <gasnet_extended_internal.h>
#if GASNETC_GNI_UDREG
#include <udreg_pub.h>
#endif

#define GASNETC_STRICT_MEM_CONSISTENCY  1 /* use GNI_MEM_STRICT_PI_ORDERING */
#define GASNETC_RELAXED_MEM_CONSISTENCY 2 /* use GNI_MEM_RELAXED_PI_ORDERING */
#define GASNETC_NEITHER_MEM_CONSISTENCY 3 /* use neither */

#if defined(GASNET_PAR) && GASNETC_GNI_MULTI_DOMAIN
#define GASNETC_USE_MULTI_DOMAIN 1

#define GASNETC_DYNAMIC_DOMAIN_ALLOC 1
#define GASNETC_STATIC_DOMAIN_ALLOC 2

#define GASNETC_DOMAIN_ALLOC_POLICY GASNETC_STATIC_DOMAIN_ALLOC
#define GASNETC_DOMAIN_THREAD_DISTRIBUTION_BULK 1
#define GASNETC_DOMAIN_THREAD_DISTRIBUTION_ROUND_ROBIN 2

#if (GASNETC_DOMAIN_ALLOC_POLICY == GASNETC_DYNAMIC_DOMAIN_ALLOC) 
#define GASNETC_DOMAIN_THREAD_DISTRIBUTION GASNETC_DOMAIN_THREAD_DISTRIBUTION_ROUND_ROBIN
#else
#define GASNETC_DOMAIN_THREAD_DISTRIBUTION GASNETC_DOMAIN_THREAD_DISTRIBUTION_BULK
#endif
#define GASNETC_DEFAULT_DOMAIN 0
#define GASNETC_ALL_DOMAINS (-1)

#define GASNETC_DOMAIN_COUNT_DEFAULT 1
#define GASNETC_PTHREADS_PER_DOMAIN_DEFAULT 1

#define GASNETC_AM_DOMAIN_POLL_MASK_DEFAULT (0xff)

#define GASNETC_DIDX_POST(_val) const int _domain_idx = (_val)

#define GASNETC_DIDX_FARG_ALONE const int _domain_idx
#define GASNETC_DIDX_FARG       , GASNETC_DIDX_FARG_ALONE
#define GASNETC_DIDX_PASS_ALONE _domain_idx
#define GASNETC_DIDX_PASS       , GASNETC_DIDX_PASS_ALONE
#define GASNETC_DIDX            (_domain_idx)
#else
/* Multi domain support makes sense only for PAR mode. */
#define GASNETC_USE_MULTI_DOMAIN 0

#define GASNETC_DIDX_POST(_val)  const int _domain_idx = 0

#define GASNETC_DIDX_FARG_ALONE  void
#define GASNETC_DIDX_FARG        /*empty*/
#define GASNETC_DIDX_PASS_ALONE  /*empty*/
#define GASNETC_DIDX_PASS        /*empty*/
#define GASNETC_DIDX             (%%% invalid use of GASNETC_DIDX %%%)
#endif

/* debug support */
#define gasnetc_GNIT_Abort(...) gasneti_fatalerror(__VA_ARGS__)

#define gasnetc_GNIT_Log(...) gasneti_console_message("log", __VA_ARGS__)

/* global vars from environment */
extern int      gasnetc_dev_id;
extern uint32_t gasnetc_cookie;
extern uint32_t gasnetc_address;
extern uint8_t  gasnetc_ptag;

extern unsigned int gasnetc_log2_remote;

/* Set to 1 if want to use gasneti_spinlock_t rather than gasneti_mutex_t
 * By default, we will use mutex_t.
 */
#ifndef GASNETC_USE_SPINLOCK
#define GASNETC_USE_SPINLOCK 0
#endif

#if GASNETC_USE_SPINLOCK
typedef gasneti_atomic_t gasnetc_gni_lock_t;
#else
typedef gasneti_mutex_t gasnetc_gni_lock_t;
#endif

#if GASNETC_USE_MULTI_DOMAIN
extern gasnetc_gni_lock_t * gasnetc_gni_lock(void);
#define gasnetc_gni_lock_addr gasnetc_gni_lock()
#else
extern gasnetc_gni_lock_t gasnetc_gni_lock;
#define gasnetc_gni_lock_addr (&gasnetc_gni_lock)
#endif

extern gasnetc_gni_lock_t gasnetc_am_buffer_lock;

// Certain features require GNI-level EP for self and PSHM_peers
#if GASNETC_BUILD_GNIRATOMIC || GASNETC_BUILD_GNICE
  #define GASNETC_LOCAL_GNI_EP 1
#endif

typedef uint64_t gasnetc_notify_t;

/* NOTE: notify_type is "pre shifted" by 24 bits */
enum gc_notify_type {
  gc_notify_request = 0x01000000,
  gc_notify_reply   = 0x02000000,
  gc_notify_rvous   = 0x03000000
};

#define gc_build_notify(_type, _initiator, _target)\
  ((uint64_t)(_type) |  ((uint64_t)(_initiator)) |  ((uint64_t)(_target) << 26))

#define GASNETC_AM_INITIATOR_SLOTS 65536 // 16 bits
#define GASNETC_AM_TARGET_SLOTS       64 //  6 bits

#define gc_notify_get_type(n) ((n) & 0x03000000)
#define gc_notify_get_target_slot(n) ((uint8_t)(((n) >> 26) & (GASNETC_AM_TARGET_SLOTS-1)))
#define gc_notify_get_initiator_slot(n) ((uint16_t)((n) & (GASNETC_AM_INITIATOR_SLOTS-1)))
#define gc_notify_get_nonce(n) ((n) & 0xff000000) // type and target slot

// Bits 24 - 25 of REM_INST_ID, aligned to notify for case of long payloads
enum gc_instid_type {
  gc_instid_header    = 0x00000000,
  gc_instid_long_req  = gc_notify_request,
  gc_instid_long_rep  = gc_notify_reply,
  gc_instid_ctrl      = 0x03000000
};
#define GC_INSTID_MASK 0x03000000

typedef struct gasnetc_post_descriptor gasnetc_post_descriptor_t;

typedef struct {
  gex_Rank_t source;
  const gex_AM_Entry_t *entry;
  int need_reply;
  gasnetc_notify_t notify;  
  gasnetc_post_descriptor_t *deferred_reply;
#if GASNETI_THREADINFO_OPT
  gasnet_threadinfo_t threadinfo;
#endif
} gasnetc_token_t;

/* Control messages */
enum {
    GC_CTRL_CREDIT,
    GC_CTRL_SHUTDOWN 
};

/* AM message "commands" */
enum {
    GC_CMD_AM_SHORT,
    GC_CMD_AM_MEDIUM,
    GC_CMD_AM_LONG,
    GC_CMD_AM_LONG_PACKED
};

/* Encode AM header bits to fit in upper 32 bits of 64 bit word */
#define gasnetc_build_am_header(command, numargs, handler, nbytes) \
 (((uint64_t)(command) << 61) | \
  ((uint64_t)(numargs) << 56) | \
  ((uint64_t)(handler) << 48) | \
  ((uint64_t)(nbytes)  << 32))

#define gasnetc_am_command(n) (((n) >> 61) & 0x7)     /* using only 2 of 3 bits */
#define gasnetc_am_numargs(n) (((n) >> 56) & 0x1f)
#define gasnetc_am_handler(n) (((n) >> 48) & 0xff)
#define gasnetc_am_nbytes(n)  (((n) >> 32) & 0xffff)  /* default uses only 12 of 16 bits */

/* This type is used by an AMShort request or reply */
typedef struct {
  gex_AM_Arg_t args[gex_AM_MaxArgs()];
} gasnetc_am_short_packet_t;

/* This type is used by an AMMedium request or reply */
typedef struct {
  gex_AM_Arg_t args[gex_AM_MaxArgs()];
} gasnetc_am_medium_packet_t;

/* This type is used by an AMLong request or reply */
typedef struct {
  void *data;
#if GASNETC_LUB_LONG <= 0xFFFFFFFFU
  uint32_t data_length;
#else
  size_t data_length;
#endif
  gex_AM_Arg_t args[gex_AM_MaxArgs()];
} gasnetc_am_long_packet_t;

/* The various ways to interpret an arriving message
 * You can tell what it is by looking at the command field
 * in the header portion of the notify word
 */
typedef union gasnetc_packet_u {
  gasnetc_am_short_packet_t gasp;
  gasnetc_am_medium_packet_t gamp;
  gasnetc_am_long_packet_t galp;
} gasnetc_packet_t;
  
/* compute header len, padded to multiple of 8-bytes */
#define GASNETC_HEADLEN_AUX(type,nargs) \
        GASNETI_ALIGNUP_NOASSERT(gasneti_offsetof(type,args[nargs]),8)
#define GASNETC_HEADLEN(cat,nargs) \
        GASNETC_HEADLEN_AUX(gasnetc_am_##cat##_packet_t,(nargs))

/* maximum message size: */
#define GASNETC_CACHELINE_SIZE 64
#define GASNETC_MSG_MAXSIZE \
        GASNETI_ALIGNUP_NOASSERT((GASNETC_HEADLEN(medium, GASNETC_MAX_ARGS) \
                                 + GASNETC_LUB_MEDIUM), GASNETC_CACHELINE_SIZE)

/* max data one can pack into a message with a long header: */
extern size_t gasnetc_packedlong_cutover;
#ifdef __CRAY_MIC_KNL
  #define GASNETC_GNI_PACKEDLONG_CUTOVER_DEFAULT 2048
#else
  // TODO: ARM64?
  #define GASNETC_GNI_PACKEDLONG_CUTOVER_DEFAULT 3072
#endif

/* use the auxseg mechanism to allocate registered memory for bounce buffers */
/* we want this many post descriptors */
#define GASNETC_GNI_NUM_PD_DEFAULT (128 * 4)
/* we want this much space for bounce buffers */
#define GASNETC_GNI_BOUNCE_SIZE_DEFAULT (65536 * 4)
/* a particular get or put <= this size goes via bounce */
#if GASNETC_GNI_UDREG
#define GASNETC_GNI_GET_BOUNCE_REGISTER_CUTOVER_DEFAULT 4096
#define GASNETC_GNI_PUT_BOUNCE_REGISTER_CUTOVER_DEFAULT 4096
#else
#define GASNETC_GNI_GET_BOUNCE_REGISTER_CUTOVER_DEFAULT 8192
#define GASNETC_GNI_PUT_BOUNCE_REGISTER_CUTOVER_DEFAULT 8192
#endif
#define GASNETC_GNI_BOUNCE_REGISTER_CUTOVER_MAX 32768
/* a particular get or put <= this size goes via fma */
#define GASNETC_GNI_GET_FMA_RDMA_CUTOVER_DEFAULT 1023
#define GASNETC_GNI_PUT_FMA_RDMA_CUTOVER_DEFAULT 1023
#define GASNETC_GNI_FMA_RDMA_CUTOVER_MAX (4096*4)
/* space for immediate bounce buffer in the post descriptor */
#define GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE 128
/* how many concurrent dynamic memory registrations to allow */
#define GASNETC_GNI_MEMREG_DEFAULT 3072
/* Radix of inter-host CE tree */
#define GASNETC_GNI_CE_RADIX_DEFAULT 2

/* largest get that can be handled by gasnetc_rdma_get_unaligned() */
extern size_t gasnetc_max_get_unaligned;

/* completion actions: */
enum {
  _gc_post_reserved = 2,  /* Bits 0-2 hold offset for trimmed copy operations */
  /* mutually-exclusive data-movement actions */
  _gc_post_copy,
  _gc_post_copy_imm,  // memcpy()
  _gc_post_copy_amo4, // 4-byte assignment
  _gc_post_copy_amo8, // 8-byte assignment
  /* mutually-exclusive resource recovery actions */
  _gc_post_unbounce,
  _gc_post_unregister,
  _gc_post_unprepare,
  /* mutually-exclusive signaling actions */
  _gc_post_completion_flag,
  _gc_post_completion_cntr,
  _gc_post_completion_eop,
  _gc_post_completion_eam,
  _gc_post_completion_iam,
  _gc_post_completion_iput,
  _gc_post_completion_iget,
  _gc_post_completion_irmw,
  _gc_post_completion_amrv,
  /* local-completion variation(s) */
  _gc_post_lc_now,
  /* optionally suppress free of the gpd */
  _gc_post_keep_gpd,
};
#define GC_POST_COPY_TRIM 7 /* up to 6 bytes of overfetch to achive 4-byte aligned Gets */
#define GC_POST(name)           ((uint32_t)1 << _gc_post_##name)
#define GC_POST_COPY            GC_POST(copy)
#define GC_POST_COPY_IMM        GC_POST(copy_imm)
#define GC_POST_COPY_AMO4       GC_POST(copy_amo4)
#define GC_POST_COPY_AMO8       GC_POST(copy_amo8)
#define GC_POST_SEND            GC_POST(send)
#define GC_POST_UNBOUNCE        GC_POST(unbounce)
#define GC_POST_UNREGISTER      GC_POST(unregister)
#define GC_POST_UNPREPARE       GC_POST(unprepare)
#define GC_POST_COMPLETION_FLAG GC_POST(completion_flag)
#define GC_POST_COMPLETION_CNTR GC_POST(completion_cntr)
#define GC_POST_COMPLETION_EOP  GC_POST(completion_eop)
#define GC_POST_COMPLETION_EAM  GC_POST(completion_eam)
#define GC_POST_COMPLETION_IAM  GC_POST(completion_iam)
#define GC_POST_COMPLETION_IPUT GC_POST(completion_iput)
#define GC_POST_COMPLETION_IGET GC_POST(completion_iget)
#define GC_POST_COMPLETION_IRMW GC_POST(completion_irmw)
#define GC_POST_COMPLETION_AMRV GC_POST(completion_amrv)
#define GC_POST_LC_NOW          GC_POST(lc_now)
#define GC_POST_KEEP_GPD        GC_POST(keep_gpd)

#define GC_POST_COMPLETION_MASK (GC_POST_COMPLETION_FLAG | \
                                 GC_POST_COMPLETION_CNTR | \
                                 GC_POST_COMPLETION_EOP  | \
                                 GC_POST_COMPLETION_EAM  | \
                                 GC_POST_COMPLETION_IAM  | \
                                 GC_POST_COMPLETION_IPUT | \
                                 GC_POST_COMPLETION_IGET | \
                                 GC_POST_COMPLETION_IRMW | \
                                 GC_POST_COMPLETION_AMRV)

struct peer_struct_t_;
typedef struct peer_struct_t_ peer_struct_t;

typedef struct am_rvous_t_ {
  struct am_rvous_t_   *next;
  peer_struct_t        *peer;
  gasnetc_notify_t      notify;
  volatile int          ready;
} am_rvous_t;

struct gasnetc_post_descriptor {
  union { /* must be first for alignment */
    uint8_t immediate[GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE];
    gasneti_weakatomic_t counter;
    gasnetc_notify_t notify;
    gex_RMA_Value_t put_val;
    uint64_t u64;
    uint64_t u32;
    am_rvous_t am_rvous;
  #if GASNETC_GNI_UDREG
    udreg_entry_t *udreg_entry;
  #endif
  } u;
  gni_post_descriptor_t pd;
  #define gpd_completion pd.post_id
  #define gpd_get_src    pd.first_operand
  #define gpd_get_dst    pd.second_operand
  #define gpd_amo_result pd.sync_flag_value
  #define gpd_am_header  pd.sync_flag_value
  #define gpd_am_packet  pd.local_addr
  #define gpd_am_length  pd.length
  #define gpd_am_peer    pd.first_operand
  #define gpd_put_lc     pd.second_operand
  #define gpd_amo_len    pd.length
  #define gpd_amo_cmd    pd.amo_cmd
  #define gpd_amo_op1    pd.first_operand
  #define gpd_amo_op2    pd.second_operand
  #define gpd_ce_cmd     pd.ce_cmd
  #define gpd_ce_mode    pd.ce_mode
  #define gpd_ce_op1     pd.first_operand
  #define gpd_ce_op2     pd.second_operand
  uint32_t gpd_flags;
#if GASNETC_USE_MULTI_DOMAIN
  int domain_idx;
#endif
};

gasnetc_post_descriptor_t *
gasnetc_alloc_post_descriptor(gex_Flags_t flags GASNETC_DIDX_FARG) GASNETI_MALLOC;

void gasnetc_free_post_descriptor(gasnetc_post_descriptor_t *pd);

int gasnetc_try_pin(void *addr, uintptr_t size);

/* exit related */
volatile int gasnetc_shutdownInProgress;
double gasnetc_shutdown_seconds; /* number of seconds to poll before forceful shutdown */
int gasnetc_sys_exit(int *exitcode);
void gasnetc_sys_fini(void);

#if GASNETC_USE_MULTI_DOMAIN
void gasnetc_create_parallel_domain(gasnete_threadidx_t tidx);
int gasnetc_get_domain_idx(gasnete_threadidx_t tidx);
#endif

void gasnetc_init_gni(gasnet_seginfo_t seginfo);
void gasnetc_init_segment(gasnet_seginfo_t seginfo);
uintptr_t gasnetc_init_messaging(void);
void gasnetc_shutdown(void); /* clean up all gni state */

#if GASNETC_BUILD_GNICE
void gasnete_init_ce(void);
extern int gasnete_ce_available;

// Type for Aries CE results (in registered memory)
typedef struct gasnete_ce_result {
  volatile int done;
  char pad1[GASNETC_CACHELINE_SIZE - sizeof(int)];
  gni_ce_result_t output;
  char pad2[GASNETC_CACHELINE_SIZE - sizeof(gni_ce_result_t)];
} gasnete_ce_result_t;

// Post a CE operation, returning a result buffer
gasnete_ce_result_t *gasnetc_post_ce(gasnetc_post_descriptor_t *gpd);

// Test a CE operation
GASNETI_INLINE(gasnete_test_ce)
gni_return_t gasnete_test_ce(gasnete_ce_result_t *result)
{
  return result->done ? GNI_CeCheckResult(&result->output,0)
                      : GNI_RC_NOT_DONE;
}
  #if GASNET_PSHM
  typedef struct {
    volatile int done_phase;
    char pad[GASNETC_CACHELINE_SIZE - sizeof(int)];
    struct {
      volatile int phase;
      char pad[GASNETC_CACHELINE_SIZE - sizeof(int)];
    } rank[1]; // Used as flexible array member
  } gasnete_ce_gate_t;
  #define GASNETC_SIZEOF_CE_GATE_T(n) gasneti_offsetof(gasnete_ce_gate_t, rank[(n)])
  extern gasnete_ce_gate_t *gasnete_ce_gate;
  #endif
#endif

void gasnetc_poll_local_queue(GASNETC_DIDX_FARG_ALONE);
void gasnetc_poll(GASNETI_THREAD_FARG_ALONE);
#if GASNETC_USE_MULTI_DOMAIN
  void gasnetc_poll_single_domain(GASNETI_THREAD_FARG_ALONE);
#endif

size_t gasnetc_rdma_put_bulk(
                 gex_TM_t tm, gex_Rank_t rank,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd) GASNETI_WARN_UNUSED_RESULT;

size_t gasnetc_rdma_put_lc(
                 gex_TM_t tm, gex_Rank_t rank,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasneti_weakatomic_val_t *initiated_lc,
		 int last_eop_chunk, gasnetc_post_descriptor_t *gpd) GASNETI_WARN_UNUSED_RESULT;

void gasnetc_rdma_put_buff(
                 gex_TM_t tm, gex_Rank_t rank,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd);

void gasnetc_rdma_put_long(
                 gex_TM_t tm, gex_Rank_t rank,
                 void *dest_addr, void *source_addr,
                 size_t nbytes,
                 uint32_t gpd_flags,
                 void *completion,
                 uint32_t nonce
                 GASNETC_DIDX_FARG);

size_t gasnetc_rdma_get(
                 gex_TM_t tm, gex_Rank_t rank,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd) GASNETI_WARN_UNUSED_RESULT;

void gasnetc_rdma_get_unaligned(
                 gex_TM_t tm, gex_Rank_t rank,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd);

int gasnetc_rdma_get_buff(
                 gex_TM_t tm, gex_Rank_t rank,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd);

#if GASNETC_BUILD_GNIRATOMIC
void gasnetc_post_amo(
                 gasneti_TM_t i_tm, gex_Rank_t tgt_rank, void *tgt_addr,
                 gasnetc_post_descriptor_t *gpd, gex_Flags_t flags);
#endif


/* returns 1 if-and-only-if value was decremented. */
/* based on gasneti_semaphore_trydown() w/o padding or rmb */
GASNETI_INLINE(gasnetc_weakatomic_dec_if_positive)
int gasnetc_weakatomic_dec_if_positive(gasneti_weakatomic_t *p)
{
#if GASNETI_THREADS || defined(GASNETI_FORCE_TRUE_WEAKATOMICS)
  int swapped;
  do {
    const gasneti_weakatomic_val_t old = gasneti_weakatomic_read(p, 0);
    if_pf (old == 0) {
      return 0;       /* Note: "break" here generates infinite loop w/ pathcc 2.4 (bug 1620) */
    }
    swapped = gasneti_weakatomic_compare_and_swap(p, old, old - 1, GASNETI_ATOMIC_NONE);
  } while (GASNETT_PREDICT_FALSE(!swapped));
  return 1;
#else
  const gasneti_weakatomic_val_t old = gasneti_weakatomic_read(p, 0);
  if_pf (old == 0) return 0;
  gasneti_weakatomic_set(p, old-1, GASNETI_ATOMIC_NONE);
  return 1;
#endif
}

/* Given a value between 1 and INT_MAX, returns the smallest power of two
 * that is greater than or equal to the argument.
 * Specifically power of two arguments return themselves.
 */
GASNETI_INLINE(gasnetc_next_power_of_2)
int gasnetc_next_power_of_2(int x) {
  x -= 1;
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  x += 1;
  return x;
}

extern int gasnetc_send_am(gasnetc_post_descriptor_t *gpd);
gasnetc_post_descriptor_t *gasnetc_alloc_reply_post_descriptor(gex_Token_t t,
                                                               size_t length,
                                                               gex_Flags_t flags);
gasnetc_post_descriptor_t *gasnetc_alloc_request_post_descriptor(gex_Rank_t dest,
                                                                 size_t length,
                                                                 gex_Flags_t flags
                                                                 GASNETI_THREAD_FARG);
gasnetc_post_descriptor_t *gasnetc_alloc_request_post_descriptor_np(gex_Rank_t dest,
                                                                    size_t min_length,
                                                                    size_t max_length,
                                                                    gex_Flags_t flags
                                                                    GASNETI_THREAD_FARG);
gasnetc_post_descriptor_t *
gasnetc_alloc_request_post_descriptor_long(
                        gex_Rank_t jobrank,
                        size_t length,
                        gex_Flags_t flags,
                        int is_packed
                        GASNETI_THREAD_FARG);

/* Some common GPD idioms */

GASNETI_INLINE(gasnete_cntr_gpd)
gasnetc_post_descriptor_t *
gasnete_cntr_gpd(gasneti_weakatomic_val_t *initiated_p, gasnete_op_t *op,
                 uint32_t gpd_flags, gex_Flags_t flags GASNETC_DIDX_FARG)
{
  gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor(flags GASNETC_DIDX_PASS);
  if_pt (gpd) {
    gpd->gpd_flags = gpd_flags;
    gpd->gpd_completion = (uintptr_t) op;
    (*initiated_p) += 1;
  }
  return gpd;
}

// Allocate an eop with the initiated_cnt pre-incremented
GASNETI_INLINE(gasnete_eop_new_cnt)
gasnete_eop_t *gasnete_eop_new_cnt(gasneti_threaddata_t * const thread) {
  gasnete_eop_t *eop = gasnete_eop_new(thread);
  eop->initiated_cnt++;
  return eop;
}

// Free a never-used eop
GASNETI_INLINE(gasnete_consume_eop)
void gasnete_consume_eop(gasnete_eop_t *eop GASNETI_THREAD_FARG) {
  // decrement the initiated counter rather than atomically increment the completed counter
  eop->initiated_cnt -= 1;
  gasneti_assert(GASNETC_EOP_CNT_DONE(eop));
  SET_EVENT_DONE(eop, 0);
  gasnete_eop_free(eop GASNETI_THREAD_PASS);
}

#define GASNETE_EOP_CNTRS(_eop) \
        &(_eop)->initiated_cnt, ((gasnete_op_t*)(_eop)), GC_POST_COMPLETION_EOP
#define GASNETE_IOP_CNTRS_put(_iop) \
        &(_iop)->initiated_put_cnt, ((gasnete_op_t*)(_iop)), GC_POST_COMPLETION_IPUT
#define GASNETE_IOP_CNTRS_get(_iop) \
        &(_iop)->initiated_get_cnt, ((gasnete_op_t*)(_iop)), GC_POST_COMPLETION_IGET
#define GASNETE_IOP_CNTRS_rmw(_iop) \
        &(_iop)->initiated_rmw_cnt, ((gasnete_op_t*)(_iop)), GC_POST_COMPLETION_IRMW
#define GASNETE_IOP_CNTRS(_iop,_putget) \
        GASNETE_IOP_CNTRS_##_putget(_iop)

#endif /* GASNET_ARIES_H */

