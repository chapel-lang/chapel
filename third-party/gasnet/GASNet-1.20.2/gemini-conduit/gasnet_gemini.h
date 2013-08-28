#ifndef GASNET_GEMINI_H
#define GASNET_GEMINI_H

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <pmi_cray.h>
#include <assert.h>
#include <gni_pub.h>
#include "gasnet_internal.h"
#include "gasnet_core_internal.h"
#include <gasnet_extended_internal.h>

#define GASNETC_STRICT_MEM_CONSISTENCY  1 /* use GNI_MEM_STRICT_PI_ORDERING */
#define GASNETC_RELAXED_MEM_CONSISTENCY 2 /* use GNI_MEM_RELAXED_PI_ORDERING */
#define GASNETC_DEFAULT_MEM_CONSISTENCY 3 /* use neither */
#define GASNETC_DEFAULT_RDMA_MEM_CONSISTENCY  GASNETC_RELAXED_MEM_CONSISTENCY

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

/* Set to 1 if want to use gasneti_spinlock_t rather than gasneti_mutex_t
 * By default, we will use mutex_t.
 */
#ifndef GASNETC_USE_SPINLOCK
#define GASNETC_USE_SPINLOCK 0
#endif

#if GASNETC_USE_SPINLOCK
typedef gasneti_atomic_t gasnetc_gni_lock_t;
#define GASNETC_INITLOCK_GNI() gasneti_spinlock_init(&gasnetc_gni_lock)
#define GASNETC_LOCK_GNI() gasneti_spinlock_lock(&gasnetc_gni_lock)
#define GASNETC_UNLOCK_GNI() gasneti_spinlock_unlock(&gasnetc_gni_lock)
#else
typedef gasneti_mutex_t gasnetc_gni_lock_t;
#define GASNETC_INITLOCK_GNI() gasneti_mutex_init(&gasnetc_gni_lock)
#define GASNETC_LOCK_GNI() gasneti_mutex_lock(&gasnetc_gni_lock)
#define GASNETC_UNLOCK_GNI() gasneti_mutex_unlock(&gasnetc_gni_lock)
#endif
extern gasnetc_gni_lock_t gasnetc_gni_lock;

typedef struct {
  gasnet_node_t source;
  int need_reply;
} gasnetc_token_t;

/* Control messages */
enum {
    GC_CTRL_CREDIT = 0,
    GC_CTRL_SHUTDOWN
};

enum {
    GC_CMD_NULL = 0, /* zero GC_Header marks free mailboxes */
    GC_CMD_AM_SHORT = 1,
    GC_CMD_AM_MEDIUM,
    GC_CMD_AM_LONG
};


typedef struct GC_Header {
  uint32_t command : 2;        /* GC_CMD_AM_* */
  uint32_t is_req  : 1;        /* 1=request, 0=reply */
  uint32_t credit  : 1;        /* piggybacked credit in addition to one implied by a Reply */
  uint32_t misc    : 15;       /* msg-dependent field (e.g. nbytes in a Medium) */
  uint32_t numargs : 5;        /* number of GASNet arguments */
  uint32_t handler : 8;        /* index of GASNet handler */
} GC_Header_t;


/* This type is used by an AMShort request or reply */
typedef struct {
  GC_Header_t header;
  gasnet_handlerarg_t args[gasnet_AMMaxArgs()];
} gasnetc_am_short_packet_t;

/* This type is used by an AMMedium request or reply */
typedef struct {
  GC_Header_t header;
  gasnet_handlerarg_t args[gasnet_AMMaxArgs()];
} gasnetc_am_medium_packet_t;

/* This type is used by an AMLong request or reply */
typedef struct {
  GC_Header_t header;
#if GASNETC_MAX_LONG <= 0xFFFFFFFFU
  uint32_t data_length;
#else
  size_t data_length;
#endif
  void *data;
  gasnet_handlerarg_t args[gasnet_AMMaxArgs()];
} gasnetc_am_long_packet_t;

/* The various ways to interpret an arriving message
 * You can tell what it is by looking at the command field
 * in the GC_Header_t
 */
typedef union gasnetc_eq_packet {
  GC_Header_t header; /* must be first */
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

void gasnetc_get_am_credit(uint32_t pe);

void gasnetc_init_post_descriptor_pool(void);

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

void  *gasnetc_alloc_bounce_buffer(void) GASNETI_MALLOC;
void gasnetc_free_bounce_buffer(void *buf);


void gasnetc_init_bounce_buffer_pool(void);

/* largest get that can be handled by gasnetc_rdma_get_unaligned() */
extern size_t gasnetc_max_get_unaligned;

/* largest put that gasnetc_rdma_put() will locally complete before return */
extern size_t gasnetc_max_put_lc;

/* send/copy, unbounce/unregister, flag/eop are each mutually exclusive pairs */
#define GC_POST_COPY_TRIM 7 /* up to 6 bytes of overfetch to achive 4-byte aligned Gets */
#define GC_POST_COPY 8
#define GC_POST_SEND 16
#define GC_POST_UNBOUNCE 32
#define GC_POST_UNREGISTER 64
#define GC_POST_COMPLETION_FLAG 128
#define GC_POST_COMPLETION_OP 256
#define GC_POST_GET 512
#define GC_POST_KEEP_GPD 1024
#define GC_POST_SMSG_BUF 2048

/* WARNING: if sizeof(gasnetc_post_descriptor_t) changes, then
 * you must update the value in gasneti_pd_auxseg_IdentString */
typedef struct gasnetc_post_descriptor {
  gni_post_descriptor_t pd; /* must be first */
  #define gpd_completion pd.post_id
  #define gpd_get_src    pd.first_operand
  #define gpd_get_dst    pd.second_operand
  union {
    uint8_t immediate[GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE];
    gasnetc_packet_t packet;
  } u;
  uint32_t flags;
  gasnet_node_t dest;
} gasnetc_post_descriptor_t;

gasnetc_post_descriptor_t *gasnetc_alloc_post_descriptor(void) GASNETI_MALLOC;
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



void gasnetc_init_segment(void *segment_start, size_t segment_size);
uintptr_t gasnetc_init_messaging(void);
void gasnetc_shutdown(void); /* clean up all gni state */


void gasnetc_poll_local_queue(void);
void gasnetc_poll(void);

int gasnetc_send_smsg(gasnet_node_t dest, 
            gasnetc_post_descriptor_t *gpd,
            gasnetc_packet_t *msg, size_t length);

void gasnetc_rdma_put_bulk(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd);

int gasnetc_rdma_put(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd);

void gasnetc_rdma_put_buff(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd);

void gasnetc_rdma_get(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd);

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

GASNETI_INLINE(gasnetc_weakatomic_swap)
gasneti_weakatomic_val_t gasnetc_weakatomic_swap(gasneti_weakatomic_t *p, gasneti_weakatomic_val_t newval)
{
#if GASNETI_THREADS || defined(GASNETI_FORCE_TRUE_WEAKATOMICS)
  #if GASNETI_HAVE_ATOMIC_SWAP
    return gasneti_atomic_swap(p, newval, GASNETI_ATOMIC_NONE);
  #else
    gasneti_atomic_val_t oldval;
    do {
      oldval = gasneti_atomic_read(p, GASNETI_ATOMIC_NONE);
    } while ((oldval != newval) &&
               !gasneti_atomic_compare_and_swap(p, oldval, newval, GASNETI_ATOMIC_NONE));
    return oldval;
  #endif
#else
  const gasneti_weakatomic_val_t oldval = gasneti_weakatomic_read(p, GASNETI_ATOMIC_NONE);
  gasneti_weakatomic_set(p, newval, GASNETI_ATOMIC_NONE);
  return oldval;
#endif
}

#endif /* GASNET_GEMINI_H */

