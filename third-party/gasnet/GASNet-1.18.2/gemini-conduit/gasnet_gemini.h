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

#define GASNETC_DEBUG 0

#ifdef GASNETC_DEBUG
#define GC_DEBUG(x) x
#define STATS(x) x
#endif
extern int gasnetc_debug;

/* debug support */
#define gasnetc_GNIT_Abort(msg, args...) do {			  \
    fprintf(stderr, "node %d error %s: " msg "\n", gasneti_mynode,	  \
	    gasnett_current_loc, ##args);		  \
    gasnett_print_backtrace(2);				  \
    gasnett_fatalerror("fatalerror");			  \
  } while(0)

#define gasnetc_GNIT_Log(msg, args...) do {			  \
    fprintf(stderr, "node %d log %s: " msg "\n", gasneti_mynode,	  \
	    gasnett_current_loc, ##args);		  \
  } while(0)



/* Set to 1 if want to use gasneti_spinlock_t rather than gasneti_mutex_t
 * in XXX
 * By default, we will use mutex_t.
 * copied from portals conduit
 */
#ifndef GASNETC_USE_SPINLOCK
#define GASNETC_USE_SPINLOCK 0
#endif

#if GASNETC_USE_SPINLOCK
gasneti_atomic_t gasnetc_gni_lock;
#define GASNETC_INITLOCK_GNI() gasneti_spinlock_init(&gasnetc_gni_lock)
#define GASNETC_LOCK_GNI() gasneti_spinlock_lock(&gasnetc_gni_lock)
#define GASNETC_UNLOCK_GNI() gasneti_spinlock_unlock(&gasnetc_gni_lock)



typedef gasneti_atomic_t gasnetc_queuelock_t;
#define GASNETC_INITLOCK_QUEUE(ptr) gasneti_spinlock_init(&((ptr)->lock))
#define GASNETC_TRYLOCK_QUEUE(ptr) gasneti_spinlock_trylock(&((ptr)->lock))
#define GASNETC_LOCK_QUEUE(ptr) gasneti_spinlock_lock(&((ptr)->lock))
#define GASNETC_UNLOCK_QUEUE(ptr) gasneti_spinlock_unlock(&((ptr)->lock))
#define GASNETC_INITLOCK_NODE(srcnode)			\
  gasneti_spinlock_init(&gasnetc_conn_queue[srcnode].lock)
#define GASNETC_TRYLOCK_NODE(srcnode)				\
  gasneti_spinlock_trylock(&gasnetc_conn_queue[srcnode].lock)
#define GASNETC_LOCK_NODE(srcnode)				\
  gasneti_spinlock_lock(&gasnetc_conn_queue[srcnode].lock)
#define GASNETC_UNLOCK_NODE(srcnode)			\
  gasneti_spinlock_unlock(&gasnetc_conn_queue[srcnode].lock)
#else
gasneti_mutex_t gasnetc_gni_lock;
#define GASNETC_INITLOCK_GNI() gasneti_mutex_init(&gasnetc_gni_lock)
#define GASNETC_LOCK_GNI() gasneti_mutex_lock(&gasnetc_gni_lock)
#define GASNETC_UNLOCK_GNI() gasneti_mutex_unlock(&gasnetc_gni_lock)

typedef gasneti_mutex_t gasnetc_queuelock_t;
#define GASNETC_INITLOCK_QUEUE(ptr) gasneti_mutex_init(&((ptr)->lock))
#define GASNETC_TRYLOCK_QUEUE(ptr) gasneti_mutex_trylock(&((ptr)->lock))
#define GASNETC_LOCK_QUEUE(ptr) gasneti_mutex_lock(&((ptr)->lock))
#define GASNETC_UNLOCK_QUEUE(ptr) gasneti_mutex_unlock(&((ptr)->lock))
#define GASNETC_INITLOCK_NODE(srcnode)			\
  gasneti_mutex_init(&gasnetc_conn_queue[srcnode].lock)
#define GASNETC_TRYLOCK_NODE(srcnode)				\
  gasneti_mutex_trylock(&gasnetc_conn_queue[srcnode].lock)
#define GASNETC_LOCK_NODE(srcnode)				\
  gasneti_mutex_lock(&gasnetc_conn_queue[srcnode].lock)
#define GASNETC_UNLOCK_NODE(srcnode)			\
  gasneti_mutex_unlock(&gasnetc_conn_queue[srcnode].lock)
#endif

#if GASNET_SEQ
  #define GASNETC_UNLOCK_GNI_IF_SEQ() GASNETC_UNLOCK_GNI()
  #define GASNETC_UNLOCK_GNI_IF_PAR() ((void)0)
#else
  #define GASNETC_UNLOCK_GNI_IF_SEQ() ((void)0)
  #define GASNETC_UNLOCK_GNI_IF_PAR() GASNETC_UNLOCK_GNI()
#endif

typedef struct {
  gasnet_node_t source;
  int need_reply;
} gasnetc_token_t;

/* convert from a pointer to a field of a struct to the struct */
#define gasnetc_get_struct_addr_from_field_addr(structname, fieldname, fieldaddr) \
        ((structname*)(((uintptr_t)fieldaddr) - offsetof(structname,fieldname)))


enum GC_CMD { /* AM Request types must have ODD values */
    GC_CMD_NULL = 0,
    GC_CMD_AM_SHORT = 1,
    GC_CMD_AM_SHORT_REPLY,
    GC_CMD_AM_MEDIUM,
    GC_CMD_AM_MEDIUM_REPLY,
    GC_CMD_AM_LONG,
    GC_CMD_AM_LONG_REPLY,
    GC_CMD_AM_NOP_REPLY,
    GC_CMD_SYS_SHUTDOWN_REQUEST   /* from portals-conduit */
};


typedef struct GC_Header {
  uint32_t command : 4;        /* GC_CMD */
  uint32_t misc    : 15;       /* msg-dependent field (e.g. nbytes in a Medium) */
  uint32_t numargs : 5;        /* number of GASNet arguments */
  uint32_t handler : 8;        /* index of GASNet handler */
} GC_Header_t;


/* This type is used to return credits */
typedef struct gasnetc_am_nop_packet {
  GC_Header_t header;
} gasnetc_am_nop_packet_t;

/* This type is used by an AMShort request or reply */
typedef struct {
  GC_Header_t header;
  uint32_t args[gasnet_AMMaxArgs()];
} gasnetc_am_short_packet_t;

/* This type is used by an AMMedium request or reply */
typedef struct {
  GC_Header_t header;
  uint32_t args[gasnet_AMMaxArgs()];
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
  uint32_t args[gasnet_AMMaxArgs()];
} gasnetc_am_long_packet_t;

/* This type is used for the exitcode reduction */
typedef struct gasnetc_sys_shutdown_packet {
  GC_Header_t header;
} gasnetc_sys_shutdown_packet_t;

/* The various ways to interpret an arriving message
 * You can tell what it is by looking at the command field
 * in the GC_Header_t
 */
typedef union gasnetc_eq_packet {
  gasnetc_am_nop_packet_t ganp;
  gasnetc_am_short_packet_t gasp;
  gasnetc_am_medium_packet_t gamp;
  gasnetc_am_long_packet_t galp;
  gasnetc_sys_shutdown_packet_t gssp;
} gasnetc_packet_t;
  
/* compute header len, padded to multiple of 8-bytes */
#define GASNETC_HEADLEN_AUX(type,nargs) \
        GASNETI_ALIGNUP_NOASSERT(offsetof(type,args)+(nargs * sizeof(uint32_t)),8)
#define GASNETC_HEADLEN(cat,nargs) \
        GASNETC_HEADLEN_AUX(gasnetc_am_##cat##_packet_t,(nargs))

/* maximum SMSG size: */
#define GASNETC_CACHELINE_SIZE 64
#define GASNETC_MSG_MAXSIZE \
        GASNETI_ALIGNUP((GASNETC_HEADLEN(medium, gasnet_AMMaxArgs()) \
                          + gasnet_AMMaxMedium()), GASNETC_CACHELINE_SIZE)

/* max data one can pack into SMSG with a long header: */
/* XXX: note 8-byte "fudge" because Larry K. has indicated some overhead exists */
/* TODO: runtime control of cut-off via an env var */
#define GASNETC_MAX_PACKED_LONG(nargs) \
        (GASNETC_MSG_MAXSIZE - GASNETC_HEADLEN(long, (nargs)) - 8)

/* Routines in gc_utils.c */

uint32_t *gasnetc_UGNI_AllAddr;

void *gasnetc_gather_nic_addresses(void);

void gasnetc_GNIT_Job_size(int *nranks);
void gasnetc_GNIT_Rank(int *inst_id);
char gasnetc_GNIT_Ptag(void);
int gasnetc_GNIT_Cookie(void);
int gasnetc_GNIT_Device_Id(void);
void gasnetc_GNIT_Allgather(void *local, long length, void *global);
void gasnetc_GNIT_Finalize();
void gasnetc_GNIT_Barrier();


void gasnetc_get_am_credit(uint32_t pe);
void gasnetc_return_am_credit(uint32_t pe);
void gasnetc_get_fma_credit(uint32_t pe);
void gasnetc_return_fma_credit(uint32_t pe);

void gasnetc_send_am_nop(uint32_t pe);


typedef struct gasnetc_queue_item {
  struct gasnetc_queue_item *next;    /* pointer to next item on q, or NULL if last */
  /*  struct gasnetc_queue_item *prev; */
  struct gasnetc_queue *queue;        /* pointer to queue we are on, or NULL if not */
} gasnetc_queue_item_t;


typedef struct gasnetc_queue {
  gasnetc_queue_item_t *head;
  gasnetc_queue_item_t *tail;
  gasnetc_queuelock_t lock;
} gasnetc_queue_t;

void gasnetc_queue_init(gasnetc_queue_t *q);
void gasnetc_queue_item_init(gasnetc_queue_item_t *qi);

gasnetc_queue_item_t *gasnetc_queue_pop(gasnetc_queue_t *q);
void gasnetc_queue_push(gasnetc_queue_t *q, gasnetc_queue_item_t *qi);
void gasnetc_queue_enqueue(gasnetc_queue_t *q, gasnetc_queue_item_t *qi);
void gasnetc_queue_enqueue_no_lock(gasnetc_queue_t *q, gasnetc_queue_item_t *qi);

void gasnetc_init_post_descriptor_pool(void);

/* use the auxseg mechanism to allocate registered memory for bounce buffers */
/* we want at least this many post descriptors */
#define GASNETC_GNI_MIN_NUM_PD_DEFAULT 4096
/* and preferably this much space */
#define GASNETC_GNI_NUM_PD_DEFAULT (4096 * 4)
/* we want at least this much space for bounce buffers */
#define GASNETC_GNI_MIN_BOUNCE_SIZE_DEFAULT 65536
/* and preferably this much space */
#define GASNETC_GNI_BOUNCE_SIZE_DEFAULT (65536 * 4)
/* a particular message up to this size goes via bounce */
#define GASNETC_GNI_BOUNCE_REGISTER_CUTOVER_DEFAULT 8192
#define GASNETC_GNI_BOUNCE_REGISTER_CUTOVER_MAX 32768
/* a particular message up to this size goes via fma */
#define GASNETC_GNI_FMA_RDMA_CUTOVER_DEFAULT 4096
#define GASNETC_GNI_FMA_RDMA_CUTOVER_MAX 4096
#define GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE 128
gasnet_seginfo_t gasnetc_bounce_buffers;   /* fields addr and size */
gasnet_seginfo_t gasnetc_pd_buffers;   /* fields addr and size */

void  *gasnetc_alloc_bounce_buffer();
void gasnetc_free_bounce_buffer(void *buf);


void gasnetc_init_bounce_buffer_pool(void);
uint32_t gasnetc_bounce_register_cutover;
uint32_t gasnetc_fma_rdma_cutover;

#define GC_POST_SEND 1
#define GC_POST_UNBOUNCE 2
#define GC_POST_UNREGISTER 4
#define GC_POST_COPY 8
#define GC_POST_INC 16
#define GC_POST_GET 32
#define GC_POST_COMPLETION_EOP 64
#define GC_POST_COMPLETION_FLAG 128

typedef struct gasnetc_post_descriptor {
  gasnetc_queue_item_t qi;  /* not needed XXX */
  void *bounce_buffer;
  void *get_target;
  uint32_t get_nbytes;
  uint32_t flags;
  gasnet_node_t dest;
  gni_mem_handle_t mem_handle;
  gasnete_op_t *completion;
  gni_post_descriptor_t pd;
  union {
    gasnetc_am_long_packet_t galp;
    char immediate[GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE];
  } u;
} gasnetc_post_descriptor_t;

gasnetc_post_descriptor_t *gasnetc_alloc_post_descriptor();
void gasnetc_free_post_descriptor(gasnetc_post_descriptor_t *pd);

/* default fraction of phys mem to assume is pinnable under CNL */
#ifndef GASNETC_DEFAULT_PHYSMEM_PINNABLE_RATIO
#define GASNETC_DEFAULT_PHYSMEM_PINNABLE_RATIO 0.75
#endif

/* exit related */
int gasnetc_exitcode;
volatile int gasnetc_shutdownInProgress;
double gasnetc_shutdown_seconds; /* number of seconds to poll before forceful shutdown */
int gasnetc_sys_exit(int *exitcode);



int gasnetc_gem_init(char **errstringp);
void gasnetc_init_segment(void *segment_start, size_t segment_size);
uintptr_t gasnetc_init_messaging(void);
void gasnetc_shutdown(void); /* clean up all gni state */


void gasnetc_poll_local_queue(void);
void gasnetc_poll(void);

int gasnetc_send(gasnet_node_t dest, 
	     void *header, int header_length, 
	    void *data, int data_length);

void gasnetc_rdma_put(gasnet_node_t dest,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd);

void gasnetc_rdma_get(gasnet_node_t dest,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd);

#endif
