#ifndef GASNET_GEMINI_H
#define GASNET_GEMINI_H

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

#define GASNETC_STRICT_MEM_CONSISTENCY  1 /* use GNI_MEM_STRICT_PI_ORDERING */
#define GASNETC_RELAXED_MEM_CONSISTENCY 2 /* use GNI_MEM_RELAXED_PI_ORDERING */
#define GASNETC_DEFAULT_MEM_CONSISTENCY 3 /* use neither */
#define GASNETC_DEFAULT_RDMA_MEM_CONSISTENCY  GASNETC_RELAXED_MEM_CONSISTENCY

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

#define GASNETC_AM_DOMAIN_POLL_MASK_DEFAULT (0x7f)

#define GASNETC_DIDX_POST(_val) const int _domain_idx = (_val)

#define GASNETC_DIDX_FARG_ALONE const int _domain_idx
#define GASNETC_DIDX_FARG       , GASNETC_DIDX_FARG_ALONE
#define GASNETC_DIDX_PASS_ALONE _domain_idx
#define GASNETC_DIDX_PASS       , GASNETC_DIDX_PASS_ALONE
#define GASNETC_DIDX            (_domain_idx)
#else
/* Multi domain support makes sense only for PAR mode. */
#define GASNETC_USE_MULTI_DOMAIN 0

#define GASNETC_DIDX_POST(_val)  GASNETI_UNUSED const int _domain_idx = 0

#define GASNETC_DIDX_FARG_ALONE  void
#define GASNETC_DIDX_FARG        /*empty*/
#define GASNETC_DIDX_PASS_ALONE  /*empty*/
#define GASNETC_DIDX_PASS        /*empty*/
#define GASNETC_DIDX             (%%% invalid use of GASNETC_DIDX %%%)
#endif

/* debug support */
#define gasnetc_GNIT_Abort(msg, args...) do {			  \
    fprintf(stderr, "node %d error %s: " msg "\n", gasneti_mynode,	  \
	    gasnett_current_loc, ##args);		  \
    gasnett_fatalerror("fatalerror (see above)");	  \
  } while(0)

#define gasnetc_GNIT_Log(msg, args...) do {			  \
    fprintf(stderr, "node %d log %s: " msg "\n", gasneti_mynode,	  \
	    gasnett_current_loc, ##args);		  \
    fflush(stderr); \
  } while(0)


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

typedef uint64_t gasnetc_notify_t;

typedef struct {
  gasnet_node_t source;
  int need_reply;
  gasnetc_notify_t notify;  
} gasnetc_token_t;

/* Control messages */
enum {
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
#define gasnetc_am_nbytes(n)  (((n) >> 32) & 0xffff)  /* using only 10 of 16 bits */

/* This type is used by an AMShort request or reply */
typedef struct {
  gasnet_handlerarg_t args[gasnet_AMMaxArgs()];
} gasnetc_am_short_packet_t;

/* This type is used by an AMMedium request or reply */
typedef struct {
  gasnet_handlerarg_t args[gasnet_AMMaxArgs()];
} gasnetc_am_medium_packet_t;

/* This type is used by an AMLong request or reply */
typedef struct {
  void *data;
#if GASNETC_MAX_LONG <= 0xFFFFFFFFU
  uint32_t data_length;
#else
  size_t data_length;
#endif
  gasnet_handlerarg_t args[gasnet_AMMaxArgs()];
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
        GASNETI_ALIGNUP_NOASSERT(offsetof(type,args)+(nargs * sizeof(uint32_t)),8)
#define GASNETC_HEADLEN(cat,nargs) \
        GASNETC_HEADLEN_AUX(gasnetc_am_##cat##_packet_t,(nargs))

/* maximum SMSG size: */
#define GASNETC_CACHELINE_SIZE 64
#define GASNETC_MSG_MAXSIZE \
        GASNETI_ALIGNUP_NOASSERT((GASNETC_HEADLEN(medium, gasnet_AMMaxArgs()) \
                          + gasnet_AMMaxMedium()), GASNETC_CACHELINE_SIZE)

/* max data one can pack into SMSG with a long header: */
/* TODO: runtime control of cut-off via an env var */
#if GASNET_CONDUIT_GEMINI
/* On Gemini it doesn't pay to pack more than 128 bytes or so */
#define GASNETC_MAX_PACKED_LONG(nargs) \
        (128 - GASNETC_HEADLEN(long, (nargs)))
#else
/* On Aries is pays to pack as much as possible */
#define GASNETC_MAX_PACKED_LONG(nargs) \
        (GASNETC_MSG_MAXSIZE - GASNETC_HEADLEN(long, (nargs)))
#endif

void gasnetc_init_post_descriptor_pool(GASNETC_DIDX_FARG_ALONE);
void gasnetc_init_bounce_buffer_pool(GASNETC_DIDX_FARG_ALONE);

/* use the auxseg mechanism to allocate registered memory for bounce buffers */
/* we want this many post descriptors */
#define GASNETC_GNI_NUM_PD_DEFAULT (128 * 4)
/* we want this much space for bounce buffers */
#define GASNETC_GNI_BOUNCE_SIZE_DEFAULT (65536 * 4)
/* a particular get or put <= this size goes via bounce */
#define GASNETC_GNI_GET_BOUNCE_REGISTER_CUTOVER_DEFAULT 8192
#define GASNETC_GNI_PUT_BOUNCE_REGISTER_CUTOVER_DEFAULT 8192
#define GASNETC_GNI_BOUNCE_REGISTER_CUTOVER_MAX 32768
/* a particular get or put <= this size goes via fma */
#define GASNETC_GNI_GET_FMA_RDMA_CUTOVER_DEFAULT 4096
#define GASNETC_GNI_PUT_FMA_RDMA_CUTOVER_DEFAULT 4096
#define GASNETC_GNI_FMA_RDMA_CUTOVER_MAX (4096*4)
/* space for immediate bounce buffer in the post descriptor */
#define GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE 128
/* how many concurrent dynamic memory registrations to allow */
#define GASNETC_GNI_MEMREG_DEFAULT 16 /* 0 = unbounded.  TODO: tune/probe? */


/* largest get that can be handled by gasnetc_rdma_get_unaligned() */
extern size_t gasnetc_max_get_unaligned;

/* largest put that gasnetc_rdma_put_lc() will accept */
extern size_t gasnetc_max_put_lc;

/* send/copy, unbounce/unregister, flag/eop are each mutually exclusive pairs */
#define GC_POST_COPY_TRIM 7 /* up to 6 bytes of overfetch to achive 4-byte aligned Gets */
#define GC_POST_COPY 8
#define GC_POST_COMPLETION_SEND 16
#define GC_POST_UNBOUNCE 32
#define GC_POST_UNREGISTER 64
#define GC_POST_COMPLETION_FLAG 128
#define GC_POST_COMPLETION_CNTR 256
#define GC_POST_KEEP_GPD 512

/* WARNING: if sizeof(gasnetc_post_descriptor_t) changes, then
 * you must update the value in gasneti_pd_auxseg_IdentString */
typedef struct gasnetc_post_descriptor {
  gni_post_descriptor_t pd; /* must be first */
  #define gpd_completion pd.post_id
  #define gpd_get_src    pd.first_operand
  #define gpd_get_dst    pd.second_operand
  #define gpd_am_header  pd.sync_flag_value
  #define gpd_am_packet  pd.local_addr
  #define gpd_am_peer    pd.first_operand
  union {
    uint8_t immediate[GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE];
    gasneti_weakatomic_t counter;
    gasnetc_notify_t notify;
  } u;
  uint32_t flags;
#if GASNETC_USE_MULTI_DOMAIN
  int domain_idx;
#endif
} gasnetc_post_descriptor_t;

gasnetc_post_descriptor_t *gasnetc_alloc_post_descriptor(GASNETC_DIDX_FARG_ALONE) GASNETI_MALLOC;

void gasnetc_free_post_descriptor(gasnetc_post_descriptor_t *pd);

int gasnetc_try_pin(void *addr, uintptr_t size);

/* default fraction of phys mem to assume is pinnable under CNL */
#ifndef GASNETC_DEFAULT_PHYSMEM_PINNABLE_RATIO
#define GASNETC_DEFAULT_PHYSMEM_PINNABLE_RATIO 0.80
#endif

/* exit related */
volatile int gasnetc_shutdownInProgress;
double gasnetc_shutdown_seconds; /* number of seconds to poll before forceful shutdown */
int gasnetc_sys_exit(int *exitcode);

#if GASNETC_USE_MULTI_DOMAIN
void gasnetc_create_parallel_domain(gasnete_threadidx_t tidx);
int gasnetc_get_domain_idx(gasnete_threadidx_t tidx);
#endif

void gasnetc_init_segment(void *segment_start, size_t segment_size);
uintptr_t gasnetc_init_messaging(void);
void gasnetc_shutdown(void); /* clean up all gni state */


void gasnetc_poll_local_queue(GASNETC_DIDX_FARG_ALONE);
void gasnetc_poll(GASNETC_DIDX_FARG_ALONE);

size_t gasnetc_rdma_put_bulk(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd) GASNETI_WARN_UNUSED_RESULT;

void gasnetc_rdma_put_lc(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd);

void gasnetc_rdma_put_buff(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd);

size_t gasnetc_rdma_get(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd) GASNETI_WARN_UNUSED_RESULT;

void gasnetc_rdma_get_unaligned(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd);

int gasnetc_rdma_get_buff(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd);

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

extern int gasnetc_send_control(gasnet_node_t dest, uint8_t op, uint16_t arg);

extern int gasnetc_send_am(gasnetc_post_descriptor_t *gpd);
gasnetc_post_descriptor_t *gasnetc_alloc_reply_post_descriptor(gasnet_token_t t,
                                                               size_t length);
gasnetc_post_descriptor_t *gasnetc_alloc_request_post_descriptor(gasnet_node_t dest, 
                                                                 size_t length);

#endif /* GASNET_GEMINI_H */

