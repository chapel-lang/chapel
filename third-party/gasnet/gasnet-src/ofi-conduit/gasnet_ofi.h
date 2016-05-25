/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ofi-conduit/gasnet_ofi.h $
 * Description: GASNet libfabric (OFI) conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2015, Intel Corporation
 * Terms of use are as specified in license.txt
 */
#ifndef GASNET_OFI_H
#define GASNET_OFI_H

#include <rdma/fabric.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_errno.h>

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
#define gasnetc_paratomic_read       gasnetc_paratomic(read)
#define gasnetc_paratomic_increment  gasnetc_paratomic(increment)
#define gasnetc_paratomic_decrement  gasnetc_paratomic(decrement)

/* Typedefs */
typedef struct fid_ep*            fid_ep_t;
typedef struct fid_fabric*        fid_fabric_t;
typedef struct fid_domain*        fid_domain_t;
typedef struct fid_cq*            fid_cq_t;
typedef struct fid_av*            fid_av_t;
typedef struct fid_mr*            fid_mr_t;

fid_fabric_t    gasnetc_ofi_fabricfd;
fid_domain_t    gasnetc_ofi_domainfd;
fid_av_t        gasnetc_ofi_avfd;

fid_ep_t        gasnetc_ofi_rdma_epfd;
fid_cq_t        gasnetc_ofi_rdma_cqfd;
fid_mr_t        gasnetc_ofi_rdma_mrfd;

fid_ep_t        gasnetc_ofi_am_epfd;
fid_cq_t        gasnetc_ofi_am_scqfd;
fid_cq_t        gasnetc_ofi_am_rcqfd;

/* Address table data */
typedef void*                     conn_entry_t;
typedef struct
{
  conn_entry_t dest;
}addr_entry_t;

typedef struct
{
  int               	size;
  addr_entry_t 			table[0];
}addr_table_t;

typedef enum OFI_OP_TYPE {
  OFI_TYPE_AM = 1,
  OFI_TYPE_AM_DATA,
  OFI_TYPE_EGET,
  OFI_TYPE_EPUT,
  OFI_TYPE_IGET,
  OFI_TYPE_IPUT
} ofi_op_type;

typedef enum OFI_AM_TYPE {
  OFI_AM_SHORT = 1,
  OFI_AM_MEDIUM,
  OFI_AM_LONG,
  OFI_AM_LONG_MEDIUM
} ofi_am_type;

typedef  void (*event_callback_fn) (struct fi_cq_data_entry *re, void *buf);
typedef  void (*rdma_callback_fn) (void *buf);

typedef struct ofi_am_send_buf {
  ofi_am_type			type;
  int 					len;
  int 					isreq;
  uint8_t 				handler;
  gasnet_node_t			sourceid;
  uint8_t 				argnum;
  void 					*dest_ptr;
  size_t 				nbytes;
  uint8_t 				data[] __attribute__((aligned(GASNETI_MEDBUF_ALIGNMENT)));
} ofi_am_send_buf_t;

typedef struct ofi_am_buf {
  struct fi_context 	ctxt;
  event_callback_fn 	callback;
  ofi_am_send_buf_t 	sendbuf;
} ofi_am_buf_t;

typedef struct ofi_ctxt {
  struct fi_context 	ctxt;
  event_callback_fn		callback;
  int 					index;
} ofi_ctxt_t;

typedef struct ofi_op_ctxt {
  struct fi_context 	ctxt;
  rdma_callback_fn		callback;
  ofi_op_type			type;
  int					data_sent;
} ofi_op_ctxt_t;

typedef struct gasnetc_ofi_token {
  gasnet_node_t 		sourceid;
} gasnetc_ofi_token_t;

int gasnetc_ofi_init(int *argc, char ***argv,
		gasnet_node_t *nodes_p, gasnet_node_t *mynode_p);
void gasnetc_ofi_poll(int blocking_polling);
void gasnetc_ofi_attach(void *segbase, uintptr_t segsize);
void gasnetc_ofi_exit();

/* Active Messages Send Functions */
int gasnetc_ofi_am_send_short(gasnet_node_t dest, gasnet_handler_t handler, 
		int numargs, va_list argptr, int isreq);
int gasnetc_ofi_am_send_medium(gasnet_node_t dest, gasnet_handler_t handler, 
		void *source_addr, size_t nbytes,
		int numargs, va_list argptr, int isreq);
int gasnetc_ofi_am_send_long(gasnet_node_t dest, gasnet_handler_t handler,
		void *source_addr, size_t nbytes,
		void *dest_addr, int numargs, va_list argptr, int isReq, int isAsync);

/* One-siede PUT/GET Functions */
void gasnetc_rdma_put(gasnet_node_t node, void *dest, void * src, size_t nbytes,
		void *ctxt_ptr);
void gasnetc_rdma_get(void *dest, gasnet_node_t node, void * src, size_t nbytes,
		void *ctxt_ptr);
void gasnetc_rdma_put_wait(gasnet_handle_t op);
void gasnetc_rdma_get_wait(gasnet_handle_t op);

int gasnetc_exit_in_progress;

#endif /*GASNET_OFI_H*/
