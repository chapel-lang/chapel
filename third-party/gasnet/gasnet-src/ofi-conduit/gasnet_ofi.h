/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ofi-conduit/gasnet_ofi.h $
 * Description: GASNet libfabric (OFI) conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2015-2017, Intel Corporation
 * Portions copyright 2018-2020, The Regents of the University of California.
 * Terms of use are as specified in license.txt
 */
#ifndef GASNET_OFI_H
#define GASNET_OFI_H

#include <gasnet_am.h>

#include <rdma/fabric.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_errno.h>

#define OFI_AM_MAX_DATA_LENGTH \
  GASNETI_ALIGNUP_NOASSERT(GASNETC_OFI_MAX_MEDIUM + \
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

struct fid_fabric*    gasnetc_ofi_fabricfd;
struct fid_domain*    gasnetc_ofi_domainfd;
struct fid_av*        gasnetc_ofi_avfd;
struct fid_cq*        gasnetc_ofi_tx_cqfd; /* CQ for both AM and RDMA tx ops */

struct fid_ep*        gasnetc_ofi_rdma_epfd;
struct fid_mr*        gasnetc_ofi_rdma_mrfd;

struct fid_ep*        gasnetc_ofi_request_epfd;
struct fid_ep*        gasnetc_ofi_reply_epfd;
struct fid_cq*        gasnetc_ofi_request_cqfd;
struct fid_cq*        gasnetc_ofi_reply_cqfd;

/* The cut off of when to fully block for a non-blocking put*/
size_t gasnetc_ofi_bbuf_threshold; 
/* Address table data */
typedef void*                     conn_entry_t;
typedef struct
{
  int               	size;
  conn_entry_t 			table[];
}addr_table_t;

typedef enum GASNETC_OFI_AM_TYPE {
  OFI_AM_SHORT = 0,
  OFI_AM_MEDIUM,
  OFI_AM_LONG,
  OFI_AM_LONG_MEDIUM
} gasnetc_ofi_am_type;

typedef  void (*event_callback_fn) (struct fi_cq_data_entry *re, void *buf);


typedef struct gasnetc_ofi_am_short_buf {
    gex_AM_Arg_t     data[gex_AM_MaxArgs()];

} gasnetc_ofi_am_short_buf_t;

typedef struct gasnetc_ofi_am_medium_buf {
   uint8_t 				data[OFI_AM_MAX_DATA_LENGTH]
                            __attribute__((aligned(GASNETI_MEDBUF_ALIGNMENT)));
   
} gasnetc_ofi_am_medium_buf_t;

typedef struct gasnetc_ofi_am_long_buf {
  void 					*dest_ptr;
  uint8_t 				data[OFI_AM_MAX_DATA_LENGTH];

} gasnetc_ofi_am_long_buf_t;

typedef struct gasnetc_ofi_am_send_buf {
    gasnetc_ofi_am_type type:2;
    uint8_t argnum:6;
    uint8_t handler;
    gex_Rank_t sourceid;
    union {
        gasnetc_ofi_am_short_buf_t short_buf;
        gasnetc_ofi_am_medium_buf_t medium_buf;
        gasnetc_ofi_am_long_buf_t long_buf;
    } buf;
} gasnetc_ofi_am_send_buf_t;

typedef struct gasnetc_ofi_am_buf {
  struct fi_context 	ctxt;
  event_callback_fn 	callback;
  gasnetc_ofi_am_send_buf_t 	sendbuf;
} gasnetc_ofi_am_buf_t;


typedef struct gasnetc_ofi_ctxt {
  struct fi_context 	ctxt;
  void * metadata;
  int 					index;
  char _pad0[GASNETI_CACHE_PAD(sizeof(int))];
  gasnetc_paratomic_t   consumed_cntr;
  char _pad1[GASNETI_CACHE_PAD(sizeof(gasnetc_paratomic_t))];
  uint64_t final_cntr;
  char _pad2[GASNETI_CACHE_PAD(sizeof(uint64_t))];
  uint64_t event_cntr;
} gasnetc_ofi_ctxt_t;


/* The following struct is for storing certain dynamically allocated
 * objects in pools. The GASNet headers state that the first sizeof(void*)
 * bytes of objects used in its pool functions need to be unused for list
 * linkage. */
typedef struct gasnetc_ofi_bounce_buf {
    uintptr_t linkage;
    void* buf;
} gasnetc_ofi_bounce_buf_t;

typedef struct gasnetc_ofi_bounce_op_ctxt {
    struct fi_context 	ctxt;
    rdma_callback_fn		callback;
    /* bounce buffers to return to the pool */
    gasneti_lifo_head_t bbuf_list;
    /* Pointer to the original context for the "big" request */
    gasnetc_ofi_op_ctxt_t*      orig_op;
    /* Counter to determine when the bbuf transfers are done */
    gasnetc_paratomic_t cntr;
} gasnetc_ofi_bounce_op_ctxt_t;

int gasnetc_ofi_init(void);
void gasnetc_ofi_poll(void);
void gasnetc_ofi_attach(void *segbase, uintptr_t segsize);
void gasnetc_ofi_exit(void);

/* Active Messages Send Functions */
int gasnetc_ofi_am_send_short(gex_Rank_t dest, gex_AM_Index_t handler, 
		int numargs, va_list argptr, int isreq GASNETI_THREAD_FARG);
int gasnetc_ofi_am_send_medium(gex_Rank_t dest, gex_AM_Index_t handler, 
		void *source_addr, size_t nbytes,
		int numargs, va_list argptr, int isreq GASNETI_THREAD_FARG);
int gasnetc_ofi_am_send_long(gex_Rank_t dest, gex_AM_Index_t handler,
		void *source_addr, size_t nbytes,
		void *dest_addr, int numargs, va_list argptr, int isReq, int isAsync
                GASNETI_THREAD_FARG);

/* One-siede PUT/GET Functions */
void gasnetc_rdma_put(gex_Rank_t node, void *dest, void * src, size_t nbytes,
		gasnetc_ofi_op_ctxt_t *ctxt_ptr GASNETI_THREAD_FARG);
void gasnetc_rdma_get(void *dest, gex_Rank_t node, void * src, size_t nbytes,
		gasnetc_ofi_op_ctxt_t *ctxt_ptr GASNETI_THREAD_FARG);

GASNETI_INLINE(gasnetc_rdma_put_will_block)
int gasnetc_rdma_put_will_block (size_t nbytes) {
    return nbytes > gasnetc_ofi_bbuf_threshold ? 1 : 0;
} 

int gasnetc_rdma_put_non_bulk(gex_Rank_t dest, void* dest_addr, void* src_addr, 
        size_t nbytes, gasnetc_ofi_op_ctxt_t* ctxt_ptr GASNETI_THREAD_FARG);
void gasnetc_rdma_put_wait(gex_Event_t op GASNETI_THREAD_FARG);
void gasnetc_rdma_get_wait(gex_Event_t op GASNETI_THREAD_FARG);

int gasnetc_exit_in_progress;

#endif /*GASNET_OFI_H*/
