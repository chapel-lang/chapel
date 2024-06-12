/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ofi-conduit/gasnet_ofi.h $
 * Description: GASNet libfabric (OFI) conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2015-2017, Intel Corporation
 * Portions copyright 2018-2020, The Regents of the University of California.
 * Terms of use are as specified in license.txt
 */
#ifndef _GASNET_OFI_H
#define _GASNET_OFI_H

#include <gasnet_am.h>

#include <rdma/fabric.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_errno.h>

// Fatal error with error code ": number(string)" appended
#define gasnetc_ofi_fatalerror(desc,ret) do { \
    int _gof_ret = (ret); \
    gasneti_fatalerror("%s: %d(%s)", (desc), _gof_ret, fi_strerror(- _gof_ret)); \
  } while (0)

// IFF ret is not SUCCESS generate a fatal error message with the error code
#define GASNETC_OFI_CHECK_RET(ret,desc) do { \
    int _gocr_ret = (ret); \
    if_pf (FI_SUCCESS != _gocr_ret) gasnetc_ofi_fatalerror(desc,_gocr_ret); \
  } while (0)

#define OFI_AM_MAX_DATA_LENGTH \
  GASNETI_ALIGNUP_NOASSERT(gasnetc_ofi_max_medium + \
                           GASNETI_ALIGNUP_NOASSERT(sizeof(gex_AM_Arg_t) * gex_AM_MaxArgs(), \
                                                    GASNETI_MEDBUF_ALIGNMENT), \
                           GASNETI_MEDBUF_ALIGNMENT)

/* Want true atomic operations only in PAR mode, not PARSYNC or SEQ.
   However, "weak" atomics treat PARSYNC the same as PAR.  So, we
   "roll our own" set of "paratomic" ops using the provided sets of
   "gasneti_atomic_" and "gasneti_nonatomic_" operations and types.
*/
#if GASNET_PAR
  #define gasnetc_paratomic(_id) _CONCAT(gasneti_atomic_,_id)
#else
  #define gasnetc_paratomic(_id) _CONCAT(gasneti_nonatomic_,_id)
#endif
typedef gasnetc_paratomic(t)         gasnetc_paratomic_t;
#define gasnetc_paratomic_init       gasnetc_paratomic(init)
#define gasnetc_paratomic_set        gasnetc_paratomic(set)
#define gasnetc_paratomic_read       gasnetc_paratomic(read)
#define gasnetc_paratomic_increment  gasnetc_paratomic(increment)
#define gasnetc_paratomic_add        gasnetc_paratomic(add)
#define gasnetc_paratomic_subtract   gasnetc_paratomic(subtract)
#define gasnetc_paratomic_decrement  gasnetc_paratomic(decrement)
#define gasnetc_paratomic_decrement_and_test  gasnetc_paratomic(decrement_and_test)

extern struct fid_fabric*    gasnetc_ofi_fabricfd;
extern struct fid_domain*    gasnetc_ofi_domainfd;
extern struct fid_cq*        gasnetc_ofi_tx_cqfd; /* CQ for both AM and RDMA tx ops */

extern struct fid_ep*        gasnetc_ofi_rdma_epfd;

extern struct fid_ep*        gasnetc_ofi_request_epfd;
extern struct fid_ep*        gasnetc_ofi_reply_epfd;
extern struct fid_cq*        gasnetc_ofi_request_cqfd;
extern struct fid_cq*        gasnetc_ofi_reply_cqfd;

extern size_t                gasnetc_max_rma_size;

/* The cut off of when to fully block for a non-blocking put*/
extern size_t gasnetc_ofi_bbuf_threshold;

typedef enum GASNETC_OFI_AM_TYPE {
  OFI_AM_SHORT = 0,
  OFI_AM_MEDIUM,
  OFI_AM_LONG,
  OFI_AM_LONG_MEDIUM
} gasnetc_ofi_am_type;

typedef struct gasnetc_ofi_am_short_buf {
    gex_AM_Arg_t     data[gex_AM_MaxArgs()];

} gasnetc_ofi_am_short_buf_t;

typedef struct gasnetc_ofi_am_medium_buf {
  uint8_t      data[1] // Flexible array member. Actual size OFI_AM_MAX_DATA_LENGTH
               __attribute__((aligned(GASNETI_MEDBUF_ALIGNMENT)));
   
} gasnetc_ofi_am_medium_buf_t;

typedef struct gasnetc_ofi_am_long_buf {
  void         *dest_ptr;
  uint8_t      data[1]; // Flexible array member. Actual size OFI_AM_MAX_DATA_LENGTH
} gasnetc_ofi_am_long_buf_t;

typedef struct gasnetc_ofi_am_send_buf {
    gasnetc_ofi_am_type type:2;  // actual range: 4-valued enum
    uint8_t argnum:5;            // actual range: 0...16, inclusive
    uint8_t isreq:1;             // actual range: Boolean
    uint8_t overhead;
    uint8_t handler;
    uint8_t reserved;            // unused padding
    gex_Rank_t sourceid;
    union {
        gasnetc_ofi_am_short_buf_t short_buf;
        gasnetc_ofi_am_medium_buf_t medium_buf;
        gasnetc_ofi_am_long_buf_t long_buf;
    } buf;
} gasnetc_ofi_am_send_buf_t;

// NOTE: first sizeof(void*) is overwritten when on freelist
typedef struct gasnetc_ofi_send_ctxt {
  struct fi_context             ctxt;
  gasneti_lifo_head_t           *pool;
  gasnetc_ofi_am_send_buf_t     sendbuf;
} gasnetc_ofi_send_ctxt_t;
#define GASNETC_SIZEOF_AM_BUF_T \
        (offsetof(gasnetc_ofi_send_ctxt_t, sendbuf.buf.long_buf.data) + OFI_AM_MAX_DATA_LENGTH)

typedef struct gasnetc_ofi_recv_ctxt {
  struct fi_context ctxt; // An opaque array of an even number of void*
  uint64_t event_cntr;
#if GASNETC_OFI_RETRY_RECVMSG
  struct gasnetc_ofi_recv_ctxt *next;
  char _pad0[GASNETI_CACHE_PAD(sizeof(struct fi_context) + sizeof(uint64_t) + sizeof(void*))];
#else
  char _pad0[GASNETI_CACHE_PAD(sizeof(struct fi_context) + sizeof(uint64_t))];
#endif

  // accessed as a pair except when recycling the multi-recv buffer
  uint64_t final_cntr;
  gasnetc_paratomic_t   consumed_cntr;
} gasnetc_ofi_recv_ctxt_t;


/* The following struct is for storing certain dynamically allocated
 * objects in pools. The GASNet headers state that the first sizeof(void*)
 * bytes of objects used in its pool functions need to be unused for list
 * linkage. */
typedef struct gasnetc_ofi_bounce_buf {
    uintptr_t linkage;
    void* buf;
} gasnetc_ofi_bounce_buf_t;

// NOTE: first sizeof(void*) is overwritten when on freelist
typedef struct gasnetc_ofi_bounce_op_ctxt {
    /* Pointer to the original context for the "big" request */
    gasnetc_ofi_nb_op_ctxt_t*      orig_op;
    /* bounce buffers to return to the pool */
    gasneti_lifo_head_t bbuf_list; // persistent - should not be first
    // Callback to run upon completion
    gasnetc_rdma_callback_fn callback; // persistent - should not be first
    /* Counter to determine when the bbuf transfers are done */
    gasnetc_paratomic_t cntr;
} gasnetc_ofi_bounce_op_ctxt_t;

typedef struct gasnetc_ofi_blocking_op_ctxt {
    gasnetc_rdma_callback_fn callback;
    volatile int          complete;
} gasnetc_ofi_blocking_op_ctxt_t;

// Conduit-specific Endpoint type
typedef struct gasnetc_EP_t_ {
  GASNETI_EP_COMMON // conduit-indep part as prefix

  // conduit-specific fields
  struct fid_mr*        mrfd;
#if GASNET_HAVE_MK_CLASS_MULTIPLE
  int                   device_only_segment; // non-zero iff the bound segment is INaccessible to host load/store
#endif
} *gasnetc_EP_t;

void gasnetc_auxseg_register(gasnet_seginfo_t si);
int gasnetc_ep_bindsegment(gasneti_EP_t ep, gasneti_Segment_t segment);
int gasnetc_ep_unbindsegment(gasneti_EP_t ep);
void gasnetc_segment_exchange(gex_TM_t tm, gex_EP_t *eps, size_t num_eps);

int gasnetc_ofi_init(void);
void gasnetc_ofi_poll(void);
void gasnetc_ofi_exit(void);

/* Active Messages Send Functions */
int gasnetc_ofi_am_send_short(gex_Rank_t dest, gex_AM_Index_t handler, 
        int numargs, va_list argptr, int isreq, gex_Flags_t flags
        GASNETI_THREAD_FARG);
int gasnetc_ofi_am_send_medium(gex_Rank_t dest, gex_AM_Index_t handler, 
        void *source_addr, size_t nbytes,
        int numargs, va_list argptr, int isreq, gex_Flags_t flags
        GASNETI_THREAD_FARG);
int gasnetc_ofi_am_send_long(gex_Rank_t dest, gex_AM_Index_t handler,
        void *source_addr, size_t nbytes, void *dest_addr,
        int numargs, va_list argptr, int isreq, gex_Flags_t flags
        GASNETI_THREAD_FARG);

// Active Message Prepare/Commit
extern gasneti_AM_SrcDesc_t gasnetc_ofi_PrepareMedium(
                gasneti_AM_SrcDesc_t  sd,
                int                   isreq,
                gex_Rank_t            jobrank,
                const void           *client_buf,
                size_t                size,
                gex_Flags_t           flags,
                unsigned int          numargs
                GASNETI_THREAD_FARG);
void gasnetc_ofi_CommitMedium(
                gasneti_AM_SrcDesc_t   sd,
                int                    isreq,
                gex_AM_Index_t         handler,
                size_t                 nbytes,
                va_list                argptr
                GASNETI_THREAD_FARG);

// One-sided PUT/GET Functions
int gasnetc_rdma_put(gex_TM_t tm, gex_Rank_t rank, void *dest, void * src, size_t nbytes,
                     gasnetc_ofi_nb_op_ctxt_t *ctxt_ptr, int alc, gex_Flags_t flags
                     GASNETI_THREAD_FARG);
int gasnetc_rdma_get(void *dest, gex_TM_t tm, gex_Rank_t rank, void * src, size_t nbytes,
                     gasnetc_ofi_nb_op_ctxt_t *ctxt_ptr, gex_Flags_t flags
                     GASNETI_THREAD_FARG);

gex_Event_t gasnetc_rdma_put_non_bulk(gex_TM_t tm, gex_Rank_t rank, void* dest_addr, void* src_addr,
        size_t nbytes, gasnetc_ofi_nb_op_ctxt_t* ctxt_ptr, gex_Flags_t flags GASNETI_THREAD_FARG);

extern int gasnetc_exit_in_progress;

// Multi-step conversion between function pointer and void* is needed to avoid
// pedantic warnings from some compilers which don't understand the C spec.
#define gasnetc_assert_callback_eq(p,f) \
        gasneti_assert_ptr((void *)(uintptr_t)(p)->callback ,==, (void *)(uintptr_t)(f))

// Maxium number of events (AMs and multi-recv recycles) per poll
#ifndef GASNETC_OFI_EVENTS_PER_POLL
#define GASNETC_OFI_EVENTS_PER_POLL 16
#endif

// Conversion of EP index to memory registration key,
// where -1 is used for the aux seg
#define _GASNETC_CLIENT_KEY_BASE     1
#define GASNETC_EPIDX_TO_KEY(epidx)  ((epidx)+_GASNETC_CLIENT_KEY_BASE)
#define GASNETC_AUX_KEY              GASNETC_EPIDX_TO_KEY(-1)

#endif /*_GASNET_OFI_H*/
