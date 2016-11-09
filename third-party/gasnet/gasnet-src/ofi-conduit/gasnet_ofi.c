/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ofi-conduit/gasnet_ofi.c $
 * Description: GASNet libfabric (OFI) conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2015, Intel Corporation
 * Terms of use are as specified in license.txt
 */
#include <gasnet_internal.h>
#include <gasnet_ofi.h>
#include <gasnet_extended_internal.h>
#include <gasnet_handler.h>
#include <gasnet_core_internal.h>

#include <pmi-spawner/gasnet_bootstrap_internal.h>
#include <ssh-spawner/gasnet_bootstrap_internal.h>
#include <mpi-spawner/gasnet_bootstrap_internal.h>

#include <rdma/fabric.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_errno.h>

#if HAVE_SYS_UIO_H
#include <sys/uio.h> /* For struct iovec */
#endif

#define USE_AV_MAP 0
static addr_table_t  *addr_table;
#if USE_AV_MAP
#define GET_AM_DEST(dest) (fi_addr_t)(addr_table->table[dest*2].dest)
#define GET_RDMA_DEST(dest) (fi_addr_t)(addr_table->table[dest*2+1].dest)
#else
#define GET_AM_DEST(dest) (fi_addr_t)(dest*2)
#define GET_RDMA_DEST(dest) (fi_addr_t)(dest*2+1)
#endif

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
#define GET_DSTADDR(dest_addr, dest) (uintptr_t)((char*)dest_addr - (char*)gasneti_seginfo[dest].addr)
#define GET_SRCADDR(src_addr, dest) (uintptr_t)((char*)src_addr - (char*)gasneti_seginfo[dest].addr)
#else
#define GET_DSTADDR(dest_addr, dest) (uintptr_t)dest_addr
#define GET_SRCADDR(src_addr, dest) (uintptr_t)src_addr
#endif

static gasneti_lifo_head_t ofi_am_pool = GASNETI_LIFO_INITIALIZER;

#define OFI_AM_NUM_BLOCK 	8
#define OFI_AM_BLOCK_SIZE 	1*1024*1024

static uint64_t             	max_buffered_send;
static uint64_t             	min_multi_recv;

extern void (*gasneti_bootstrapBarrier_p)(void);
extern void (*gasneti_bootstrapExchange_p)(void *src, size_t len, void *dest);
extern void (*gasneti_bootstrapFini_p)(void);
extern void (*gasneti_bootstrapAbort_p)(int exitcode);
extern void (*gasneti_bootstrapAlltoall_p)(void *src, size_t len, void *dest);
extern void (*gasneti_bootstrapBroadcast_p)(void *src, size_t len, void *dest, int rootnode);
extern void (*gasneti_bootstrapSNodeCast_p)(void *src, size_t len, void *dest, int rootnode);
extern void (*gasneti_bootstrapCleanup_p)(void);

static struct iovec *am_iov;
static struct fi_msg *am_buff_msg;
static ofi_ctxt_t *am_buff_ctxt = NULL;

static gasnetc_paratomic_t pending_rdma = gasnetc_paratomic_init(0);
static gasnetc_paratomic_t pending_am = gasnetc_paratomic_init(0);

static int gasnetc_ofi_inited = 0;

gasnetc_ofi_state_t gasnetc_ofi_state;

#define OFI_CONDUIT_VERSION FI_VERSION(1, 0)
#define NUM_CQ_ENTRIES 2

#if GASNET_PAR
static inline int gasnetc_is_exiting(void) {
    gasneti_sync_reads();
    return gasnetc_exit_in_progress;
}
#define gasnetc_is_exit_error(e) \
  (gasnetc_is_exiting() && ((e).err == FI_SUCCESS || (e).err == FI_ECANCELED || (e).err == EACCES))
#else
#define gasnetc_is_exit_error(e) 0
#endif

/*------------------------------------------------
 * Initialize OFI conduit
 * ----------------------------------------------*/

int gasnetc_ofi_init(int *argc, char ***argv, 
                     gasnet_node_t *nodes_p, gasnet_node_t *mynode_p)
{
  int ret = GASNET_OK;
  int result = GASNET_ERR_NOT_INIT;
  struct fi_info		*hints, *info;
  struct fi_cq_attr   	cq_attr 	= {0};
  struct fi_av_attr   	av_attr 	= {0};
  char   sockname[256], *alladdrs;
  size_t socknamelen = sizeof(sockname)/2;
  size_t optlen;
  conn_entry_t *mapped_table;
  int high_perf_prov = 0;

  const char *not_set = "(not set)";
  char *spawner = gasneti_getenv_withdefault("GASNET_SPAWNER", not_set);

  /* Bootstrapinit */
#if HAVE_SSH_SPAWNER
  if ((!strcmp(spawner, "ssh") || (spawner == not_set)) &&
      GASNET_OK == (result = gasneti_bootstrapInit_ssh(argc, argv, nodes_p, mynode_p))) {
    gasneti_bootstrapFini_p     = &gasneti_bootstrapFini_ssh;
    gasneti_bootstrapAbort_p    = &gasneti_bootstrapAbort_ssh;
    gasneti_bootstrapBarrier_p  = &gasneti_bootstrapBarrier_ssh;
    gasneti_bootstrapExchange_p = &gasneti_bootstrapExchange_ssh;
    gasneti_bootstrapAlltoall_p = &gasneti_bootstrapAlltoall_ssh;
    gasneti_bootstrapBroadcast_p= &gasneti_bootstrapBroadcast_ssh;
    gasneti_bootstrapSNodeCast_p= &gasneti_bootstrapSNodeBroadcast_ssh;
    gasneti_bootstrapCleanup_p  = &gasneti_bootstrapCleanup_ssh;
  } else
#endif
#if HAVE_MPI_SPAWNER
  if (!strcmp(spawner, "mpi")) {
    result = gasneti_bootstrapInit_mpi(argc, argv, nodes_p, mynode_p);
    gasneti_bootstrapFini_p     = &gasneti_bootstrapFini_mpi;
    gasneti_bootstrapAbort_p    = &gasneti_bootstrapAbort_mpi;
    gasneti_bootstrapBarrier_p  = &gasneti_bootstrapBarrier_mpi;
    gasneti_bootstrapExchange_p = &gasneti_bootstrapExchange_mpi;
    gasneti_bootstrapAlltoall_p = &gasneti_bootstrapAlltoall_mpi;
    gasneti_bootstrapBroadcast_p= &gasneti_bootstrapBroadcast_mpi;
    gasneti_bootstrapSNodeCast_p= &gasneti_bootstrapSNodeBroadcast_mpi;
    gasneti_bootstrapCleanup_p  = &gasneti_bootstrapCleanup_mpi;
  } else
#endif
#if HAVE_PMI_SPAWNER
  if ((!strcmp(spawner, "pmi") || (spawner == not_set)) &&
      GASNET_OK == (result = gasneti_bootstrapInit_pmi(argc, argv, nodes_p, mynode_p))) {
    gasneti_bootstrapFini_p     = &gasneti_bootstrapFini_pmi;
    gasneti_bootstrapAbort_p    = &gasneti_bootstrapAbort_pmi;
    gasneti_bootstrapBarrier_p  = &gasneti_bootstrapBarrier_pmi;
    gasneti_bootstrapExchange_p = &gasneti_bootstrapExchange_pmi;
    gasneti_bootstrapAlltoall_p = &gasneti_bootstrapAlltoall_pmi;
    gasneti_bootstrapBroadcast_p= &gasneti_bootstrapBroadcast_pmi;
    gasneti_bootstrapSNodeCast_p= &gasneti_bootstrapSNodeBroadcast_pmi;
    gasneti_bootstrapCleanup_p  = &gasneti_bootstrapCleanup_pmi;
  } else
#endif
  {
    gasneti_fatalerror("Requested spawner \"%s\" is unknown or not supported in this build", spawner);
  }

  memset(&gasnetc_ofi_state, 0, sizeof(gasnetc_ofi_state_t));

  gasneti_spinlock_init(&gasnetc_ofi_state.lock);

  /* OFI initialization */

  /* Alloc hints*/
  hints = fi_allocinfo();
  if (!hints) gasneti_fatalerror("fi_allocinfo for hints failed\n");

  /* caps: fabric interface capabilities */
  hints->caps			= FI_RMA;		/* RMA read/write operations */
  hints->caps         |= FI_MSG;       /* send/recv messages */
  hints->caps         |= FI_MULTI_RECV; /* support posting multi-recv
												buffer */
  /* mode: convey requirements for application to use fabric interfaces */
  hints->mode			= FI_CONTEXT;	/* fi_context is used for per
											   operation context parameter */
  /* addr_format: expected address format for AV/CM calls */
  hints->addr_format		= FI_FORMAT_UNSPEC;
  hints->tx_attr->op_flags	= FI_DELIVERY_COMPLETE|FI_COMPLETION;
  hints->rx_attr->op_flags	= FI_MULTI_RECV|FI_COMPLETION;
  hints->ep_attr->type		= FI_EP_RDM; /* Reliable datagram */
  hints->domain_attr->threading			= FI_THREAD_DOMAIN;
  hints->domain_attr->control_progress	= FI_PROGRESS_AUTO;
  /* resource_mgmt: FI_RM_ENABLED - provider protects against overrunning 
	 local and remote resources. */
  hints->domain_attr->resource_mgmt		= FI_RM_ENABLED;
  /* av_type: type of address vectores that are usable with this domain */
  hints->domain_attr->av_type			= FI_AV_TABLE; /* type AV index */

  ret = fi_getinfo(OFI_CONDUIT_VERSION, NULL, NULL, 0ULL, hints, &info);
  if (FI_SUCCESS != ret) {
	  GASNETI_RETURN_ERRR(RESOURCE,
			  "No OFI providers found that could support the OFI conduit");
  }

  /* FIXME: walk list of providers and implement some
   * selection logic */

  if (!strncmp(info->fabric_attr->prov_name, "psm", 7) ||
		  !strncmp(info->fabric_attr->prov_name, "psm2", 3))
	  high_perf_prov = 1;

  if (!high_perf_prov && !*mynode_p) {
          const char * msg = 
          "WARNING: Using OFI provider (%s), which has not been validated to provide\n"
          "WARNING: acceptable GASNet performance. You should consider using a more\n"
          "WARNING: hardware-appropriate GASNet conduit. See ofi-conduit/README.\n";
	  if (!gasneti_getenv_int_withdefault("GASNET_QUIET", 0, 1))
		  fprintf(stderr, msg, info->fabric_attr->prov_name);
  }

  /* Open the fabric provider */
  ret = fi_fabric(info->fabric_attr, &gasnetc_ofi_fabricfd, NULL);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_fabric failed: %d\n", ret);

  /* Open a fabric access domain, also referred to as a resource domain */
  ret = fi_domain(gasnetc_ofi_fabricfd, info, &gasnetc_ofi_domainfd, NULL);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_domain failed: %d\n", ret);

  /* Allocate a new active endpoint for RDMA operations */
  info->caps      = FI_RMA;       /* RMA read/write operations */
  info->rx_attr->op_flags = 0;
  ret = fi_endpoint(gasnetc_ofi_domainfd, info, &gasnetc_ofi_rdma_epfd, NULL);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_endpoint for rdma failed: %d\n", ret);

  /* Allocate a new active endpoint for AM operations */
  info->caps          = FI_MSG;       /* send/recv messages */
  info->caps          |= FI_MULTI_RECV; /* support posting multi-recv
												buffer */
  info->rx_attr->op_flags = FI_MULTI_RECV|FI_COMPLETION;
  ret = fi_endpoint(gasnetc_ofi_domainfd, info, &gasnetc_ofi_am_epfd, NULL);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_endpoint for am failed: %d\n", ret);

  /* Allocate a new completion queue for RDMA operations */
  memset(&cq_attr, 0, sizeof(cq_attr));
  cq_attr.format    = FI_CQ_FORMAT_DATA; /* Provides data associated with a completion */
  ret = fi_cq_open(gasnetc_ofi_domainfd, &cq_attr, &gasnetc_ofi_rdma_cqfd, NULL);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_cq_open for rdma_eqfd failed: %d\n", ret);

  /* Allocate send/recv completion queues for AM operations */
  memset(&cq_attr, 0, sizeof(cq_attr));
  cq_attr.format    = FI_CQ_FORMAT_DATA;
  ret = fi_cq_open(gasnetc_ofi_domainfd, &cq_attr, &gasnetc_ofi_am_scqfd, NULL);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_cq_open for am send cq faled: %d\n", ret);
  ret = fi_cq_open(gasnetc_ofi_domainfd, &cq_attr, &gasnetc_ofi_am_rcqfd, NULL);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_cq_open for am recv cq failed: %d\n", ret);

  /* Bind CQs to endpoints */
  ret = fi_ep_bind(gasnetc_ofi_rdma_epfd, &gasnetc_ofi_rdma_cqfd->fid, FI_SEND);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_ep_bind for rdma_cqfd to rdma_epfd failed: %d\n", ret);

  ret = fi_ep_bind(gasnetc_ofi_am_epfd, &gasnetc_ofi_am_scqfd->fid, FI_SEND);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_ep_bind for am_scqfd to am_epfd failed: %d\n", ret);

  ret = fi_ep_bind(gasnetc_ofi_am_epfd, &gasnetc_ofi_am_rcqfd->fid, FI_RECV);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_ep_bind for am_rcqfd to am_epfd failed: %d\n", ret);

  /* Low-water mark for shared receive buffer */
  min_multi_recv = OFI_AM_MAX_DATA_LENGTH + offsetof(ofi_am_send_buf_t,data);
  optlen = min_multi_recv;
  ret	 = fi_setopt(&gasnetc_ofi_am_epfd->fid, FI_OPT_ENDPOINT, FI_OPT_MIN_MULTI_RECV,
		  &optlen,
		  sizeof(optlen));
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_setopt for am_epfd failed: %d\n", ret);

  /* Open Address Vector and bind the AV to the domain */
#if USE_AV_MAP
  av_attr.type        = FI_AV_MAP;
  addr_table          = (addr_table_t*)gasneti_malloc(gasneti_nodes * 2 * sizeof(conn_entry_t) + sizeof(addr_table_t));
  addr_table->size    = gasneti_nodes * 2;
  mapped_table        = addr_table->table;
#else
  av_attr.type        = FI_AV_TABLE;
  mapped_table        = NULL;
#endif
  ret = fi_av_open(gasnetc_ofi_domainfd, &av_attr, &gasnetc_ofi_avfd, NULL);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_av_open failed: %d\n", ret);

  /* Bind AV to endpoints, both RDMA/AM endpoints share the same AV object */
  ret = fi_ep_bind(gasnetc_ofi_rdma_epfd, &gasnetc_ofi_avfd->fid, 0);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_ep_bind for avfd to rdma_epfd failed: %d\n", ret);

  ret = fi_ep_bind(gasnetc_ofi_am_epfd, &gasnetc_ofi_avfd->fid, 0);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_ep_bind for avfd to am_epfd failed: %d\n", ret);

  /* Enable endpoints */
  ret = fi_enable(gasnetc_ofi_rdma_epfd);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_enable for rdma failed: %d\n", ret);
  ret = fi_enable(gasnetc_ofi_am_epfd);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_enable for am failed: %d\n", ret);

  gasneti_nodemapInit(gasneti_bootstrapExchange_p, NULL, 0, 0);

  /* Get the address of AM endpoint and publish to other nodes through bootstrap
   * exchange function */
  ret = fi_getname(&gasnetc_ofi_am_epfd->fid, sockname, &socknamelen);
  ret = fi_getname(&gasnetc_ofi_rdma_epfd->fid, sockname+socknamelen, &socknamelen);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_getepname failed: %d\n", ret);
  alladdrs = gasneti_malloc(gasneti_nodes*socknamelen*2);
  (*gasneti_bootstrapExchange_p)(&sockname, socknamelen*2, alladdrs);
  if (gasneti_nodes*2 != fi_av_insert(gasnetc_ofi_avfd, alladdrs, gasneti_nodes*2,
			  (fi_addr_t*)mapped_table, 0ULL, NULL)) {
  	gasneti_fatalerror("fi_av_insert failed: %d\n", ret);
  }
  gasneti_free(alladdrs);

  fi_freeinfo(hints);

  gasnetc_ofi_inited = 1;

  return ret;
}

/*------------------------------------------------
 * OFI conduit exit function
 * ----------------------------------------------*/
void gasnetc_ofi_exit(void)
{
  int i;
  int ret = FI_SUCCESS;

  if (!gasnetc_ofi_inited)
	  return;

  while(gasnetc_paratomic_read(&pending_am,0) ||
      gasnetc_paratomic_read(&pending_rdma,0))
    gasnetc_ofi_poll(0);

  /* Drain up CQ before cancel the multi-recv requests */
  { struct fi_cq_data_entry re = {0};
    while(fi_cq_read(gasnetc_ofi_am_rcqfd, (void *)&re, 1) != -FI_EAGAIN);
  }

  if(am_buff_ctxt) {
    for(i = 0; i < OFI_AM_NUM_BLOCK; i++) {
      /* cancel the multi-recv */
      ret = fi_cancel(&gasnetc_ofi_am_epfd->fid, &am_buff_ctxt[i].ctxt);
    #if 0 /* If exiting from a AM handler context, or multi-threaded, then cancel could fail */
      if (FI_SUCCESS != ret) gasneti_fatalerror("failed fi_cancel the %d am_buff_msg\n", i);
      gasneti_free(am_iov[i].iov_base);
    #else
      if (FI_SUCCESS == ret)
        gasneti_free(am_iov[i].iov_base);
    #endif
    }
  #if GASNETI_CLIENT_THREADS
    /* Unsafe to free resources if other threads may be using them */
  #else
    gasneti_free(am_buff_ctxt);
    gasneti_free(am_iov);
    gasneti_free(am_buff_msg);
  #endif
  }

  if(fi_close(&gasnetc_ofi_rdma_mrfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close mrfd failed\n");
  }

  if(fi_close(&gasnetc_ofi_am_epfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am epfd failed\n");
  }

  if(fi_close(&gasnetc_ofi_rdma_epfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close rdma epfd failed\n");
  }

  if(fi_close(&gasnetc_ofi_am_scqfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am scqfd failed\n");
  }

  if(fi_close(&gasnetc_ofi_am_rcqfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am rcqfd failed\n");
  }

  if(fi_close(&gasnetc_ofi_rdma_cqfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close write eqfd failed\n");
  }

  if(fi_close(&gasnetc_ofi_avfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close av failed\n");
  }

  if(fi_close(&gasnetc_ofi_domainfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close domainfd failed\n");
  }

  if(fi_close(&gasnetc_ofi_fabricfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close fabricfd failed\n");
  }

#if USE_AV_MAP
  gasneti_free(addr_table);
#endif
}

/*------------------------------------------------
 * OFI conduit callback functions
 * ----------------------------------------------*/

/* Handle Active Messages */
GASNETI_INLINE(gasnetc_ofi_handle_am_common)
void gasnetc_ofi_handle_am_common(ofi_am_send_buf_t *header)
{
	uint8_t *addr;
	int nbytes;
	int isreq = header->isreq;
	int handler = header->handler;
	gasneti_handler_fn_t handler_fn = gasnetc_handler[handler];
	gasnetc_ofi_token_t token; 
	gasnetc_ofi_token_t *token_p = &token; 
	gasnet_handlerarg_t *args = (gasnet_handlerarg_t *)header->data;
	int numargs = header->argnum;
	token.sourceid = header->sourceid;

	switch(header->type) {
		case OFI_AM_SHORT:
			GASNETI_RUN_HANDLER_SHORT(isreq, handler, handler_fn, token_p, args, numargs);
			break;
		case OFI_AM_MEDIUM:
			nbytes = header->nbytes;
			addr = header->data + header->len - nbytes;
			GASNETI_RUN_HANDLER_MEDIUM(isreq, handler, handler_fn, token_p, args, numargs, addr, nbytes);
			break;
		case OFI_AM_LONG:
			addr = header->dest_ptr;
			nbytes = header->nbytes;
			GASNETI_RUN_HANDLER_LONG(isreq, handler, handler_fn, token_p, args, numargs, addr, nbytes);
			break;
		case OFI_AM_LONG_MEDIUM:
			addr = header->dest_ptr;
			nbytes = header->nbytes;
			memcpy(addr, header->data+header->len-nbytes, nbytes);
			GASNETI_RUN_HANDLER_LONG(isreq, handler, handler_fn, token_p, args, numargs, addr, nbytes);
			break;
		default:
			gasneti_fatalerror("undefined header type in gasnetc_ofi_handle_am: %d\n",
					header->type);
	}
}

#if !GASNET_PSHM
/* Handle Active Messages from self (not necessary if PSHM is enabled) */
GASNETI_INLINE(gasnetc_ofi_handle_local_am)
void gasnetc_ofi_handle_local_am(ofi_am_buf_t *buf)
{
    gasnetc_ofi_handle_am_common(&buf->sendbuf);
    gasneti_lifo_push(&ofi_am_pool, buf);
}
#endif

/* Handle incoming Active Messages */
GASNETI_INLINE(gasnetc_ofi_handle_am)
void gasnetc_ofi_handle_am(struct fi_cq_data_entry *re, void *buf)
{
    gasnetc_ofi_handle_am_common((ofi_am_send_buf_t*)re->buf);
}

/* Handle RDMA completion as the initiator */
GASNETI_INLINE(gasnetc_ofi_handle_rdma)
void gasnetc_ofi_handle_rdma(void *buf)
{
	ofi_op_ctxt_t *ptr = (ofi_op_ctxt_t*)buf;

	switch (ptr->type) {
		case OFI_TYPE_EGET:
		case OFI_TYPE_EPUT:
			{
				gasnete_eop_t *eop = (gasnete_eop_t *)container_of(ptr, gasnete_eop_t, ofi);
				GASNETE_EOP_MARKDONE(eop);
			}
			break;
		case OFI_TYPE_IGET:
			{
				gasnete_iop_t *iop = (gasnete_iop_t *)container_of(ptr, gasnete_iop_t, get_ofi);
				gasneti_weakatomic_increment(&(iop->completed_get_cnt), 0);
			}
			break;
		case OFI_TYPE_IPUT:
			{
				gasnete_iop_t *iop = (gasnete_iop_t *)container_of(ptr, gasnete_iop_t, put_ofi);
				gasneti_weakatomic_increment(&(iop->completed_put_cnt), 0);
			}
			break;
		case OFI_TYPE_AM_DATA:
			{
				ptr->data_sent = 1;
			}
			break;
		default:
			gasneti_fatalerror("receive undefined OP type in gasnetc_ofi_rdma_poll: %d\n", ptr->type);

	}
}

/* Release ACKed send buffer */
GASNETI_INLINE(gasnetc_ofi_release_am)
void gasnetc_ofi_release_am(struct fi_cq_data_entry *re, void *buf)
{
	ofi_am_buf_t *header = (ofi_am_buf_t*)buf;
	gasneti_lifo_push(&ofi_am_pool, header);
}

/* Get a send buffer */
GASNETI_INLINE(gasnetc_ofi_am_header)
ofi_am_buf_t *gasnetc_ofi_am_header(void)
{
	ofi_am_buf_t *header = gasneti_lifo_pop(&ofi_am_pool);
	if_pf (NULL == header) {
		header = gasneti_malloc(sizeof(ofi_am_buf_t));
		header->callback = gasnetc_ofi_release_am;
		gasneti_leak(header);
	}
    return header;
}

/*------------------------------------------------
 * Pre-post or pin-down memory
 * ----------------------------------------------*/
void gasnetc_ofi_attach(void *segbase, uintptr_t segsize)
{
	int ret = FI_SUCCESS;
	int i;
	int iov_len = OFI_AM_BLOCK_SIZE;
	int num_iov = OFI_AM_NUM_BLOCK;
	am_iov = (struct iovec *) gasneti_malloc(sizeof(struct iovec)*num_iov);
	am_buff_msg = (struct fi_msg *) gasneti_malloc(sizeof(struct fi_msg)*num_iov);
	am_buff_ctxt = (ofi_ctxt_t *) gasneti_malloc(sizeof(ofi_ctxt_t)*num_iov);

	/* Pin-down Memory Region */
#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
	ret = fi_mr_reg(gasnetc_ofi_domainfd, segbase, segsize, FI_REMOTE_READ | FI_REMOTE_WRITE, 0ULL, 0ULL, FI_MR_SCALABLE, &gasnetc_ofi_rdma_mrfd, NULL);
#else
	ret = fi_mr_reg(gasnetc_ofi_domainfd, (void *)0, UINT64_MAX, FI_REMOTE_READ | FI_REMOTE_WRITE, 0ULL, 0ULL, FI_MR_SCALABLE, &gasnetc_ofi_rdma_mrfd, NULL);
#endif
	if (FI_SUCCESS != ret) gasneti_fatalerror("fi_mr_reg for rdma failed: %d\n", ret);

	for(i = 0; i < num_iov; i++) {
		am_iov[i].iov_base		= gasneti_malloc(iov_len);
		am_iov[i].iov_len		= iov_len;
		am_buff_msg[i].msg_iov		= &am_iov[i];
		am_buff_msg[i].iov_count 	= 1;
		am_buff_msg[i].addr 		= FI_ADDR_UNSPEC;
		am_buff_msg[i].desc	  	= NULL;
		am_buff_msg[i].context 		= &am_buff_ctxt[i].ctxt;
		am_buff_msg[i].data 		= 0;
		am_buff_ctxt[i].callback	= gasnetc_ofi_handle_am;
		am_buff_ctxt[i].index		= i;
		/* Post buffers for Active Messages */
		ret = fi_recvmsg(gasnetc_ofi_am_epfd, &am_buff_msg[i], FI_MULTI_RECV);
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_recvmsg failed: %d\n", ret);
	}

}


/*------------------------------------------------
 * OFI conduit network poll function
 * ----------------------------------------------*/

/* RDMA progress function */
static void gasnetc_ofi_rdma_poll(int blocking_poll)
{
	int ret = 0;
	struct fi_cq_data_entry re = {0};
	struct fi_cq_err_entry e = {0};

	/* Read from Completion Queue */
	GASNETC_OFI_LOCK();
	ret = fi_cq_read(gasnetc_ofi_rdma_cqfd, (void *)&re, 1);
	GASNETC_OFI_UNLOCK();
	if (ret != -FI_EAGAIN)
	{
		if (ret < 0) {
			int err_sz = 0;
			err_sz = fi_cq_readerr(gasnetc_ofi_rdma_cqfd, &e ,0);
                        if_pf (gasnetc_is_exit_error(e)) return;
			gasneti_fatalerror("fi_cq_read for rdma_ep failed with error: %s, err_sz %d\n", 
					fi_strerror(e.err), err_sz);
		} else {
			/* got a RDMA ACK message, update handler */
			ofi_op_ctxt_t *ptr = (ofi_op_ctxt_t *)re.op_context;
			ptr->callback(re.op_context);
			gasnetc_paratomic_decrement(&pending_rdma,0);
		}
	}
}

/* AM progress function */
static void gasnetc_ofi_am_send_poll(int blocking_poll)
{
	int ret = 0;
	struct fi_cq_data_entry re = {0};
	struct fi_cq_err_entry e = {0};

	/* Read from Completion Queue */
	GASNETC_OFI_LOCK();
	ret = fi_cq_read(gasnetc_ofi_am_scqfd, (void *)&re, 1);
	GASNETC_OFI_UNLOCK();
	if (ret != -FI_EAGAIN)
	{
		if (ret < 0) {
			fi_cq_readerr(gasnetc_ofi_am_scqfd, &e ,0);
                        if_pf (gasnetc_is_exit_error(e)) return;
			gasneti_fatalerror("fi_cq_read for am_send_poll failed with error: %s\n", fi_strerror(e.err));
		} else {
			ofi_am_buf_t *header;
			header = (ofi_am_buf_t *)re.op_context;
			header->callback(&re, header);
			gasnetc_paratomic_decrement(&pending_am, 0);
		}
	}
}

static void gasnetc_ofi_am_recv_poll(int blocking_poll)
{
	int ret = 0;
	struct fi_cq_data_entry re = {0};
	struct fi_cq_err_entry e = {0};

	/* Read from Completion Queue */
	GASNETC_OFI_LOCK();
	ret = fi_cq_read(gasnetc_ofi_am_rcqfd, (void *)&re, 1);
	GASNETC_OFI_UNLOCK();
	if (ret != -FI_EAGAIN)
	{
		if (ret < 0) {
			fi_cq_readerr(gasnetc_ofi_am_rcqfd, &e ,0);
                        if_pf (gasnetc_is_exit_error(e)) return;
			gasneti_fatalerror("fi_cq_read for am_recv_poll failed with error: %s\n", fi_strerror(e.err));
		} else {
			if(re.flags & FI_MULTI_RECV)
			{
				/* One pre-post buffer is used up, re-link it */
				ofi_ctxt_t *header;
				header = (ofi_ctxt_t *)re.op_context;
				if (re.len < min_multi_recv && re.len != 0)
					header->callback(&re, header);
				ret = fi_recvmsg(gasnetc_ofi_am_epfd, &(am_buff_msg[header->index]), FI_MULTI_RECV);
				if (FI_SUCCESS != ret) gasneti_fatalerror("fi_recvmsg failed inside am_poll: %d\n", ret);
			} else {
				ofi_am_buf_t *header;
				header = (ofi_am_buf_t *)re.op_context;
				header->callback(&re, header);
			}
		}
	}
}

/* General progress function */
void gasnetc_ofi_poll(int blocking_poll)
{
	while(gasnetc_paratomic_read(&pending_rdma,0))
		gasnetc_ofi_rdma_poll(blocking_poll);

	while(gasnetc_paratomic_read(&pending_am,0))
		gasnetc_ofi_am_send_poll(blocking_poll);

	gasnetc_ofi_am_recv_poll(blocking_poll);

	return;
}


/*------------------------------------------------
 * OFI conduit am send functions
 * ----------------------------------------------*/

int gasnetc_ofi_am_send_short(gasnet_node_t dest, gasnet_handler_t handler,
                     int numargs, va_list argptr, int isreq)
{
	int ret = FI_SUCCESS;
	gasnet_handlerarg_t *arglist;
	int i, len;
	ofi_am_buf_t *header;
	ofi_am_send_buf_t *sendbuf;

	/* Get a send buffer */
	header = gasnetc_ofi_am_header();

	/* Fill in the arguments */
	sendbuf = &header->sendbuf;
	sendbuf->len = 0;
	arglist = (gasnet_handlerarg_t*) sendbuf->data;
	for (i = 0 ; i < numargs ; ++i) {
		arglist[i] = va_arg(argptr, gasnet_handlerarg_t);
		sendbuf->len += sizeof(gasnet_handlerarg_t);
	}

	/* Copy arg and handle into the buffer */
	sendbuf->isreq = isreq;
	sendbuf->handler = (uint8_t) handler;
	sendbuf->sourceid = gasneti_mynode;
	sendbuf->type = OFI_AM_SHORT;
	sendbuf->argnum = numargs;

	len = GASNETI_ALIGNUP(sendbuf->len + offsetof(ofi_am_send_buf_t, data), GASNETI_MEDBUF_ALIGNMENT);

#if !GASNET_PSHM
	if (dest == gasneti_mynode) {
		gasnetc_ofi_handle_local_am(header);
		return 0;
	}
#endif

	if(len < max_buffered_send) {
		GASNETC_OFI_LOCK();
		ret = fi_inject(gasnetc_ofi_am_epfd, sendbuf, len, GET_AM_DEST(dest));
		GASNETC_OFI_UNLOCK();
		while (ret == -FI_EAGAIN) {
			gasnetc_ofi_am_send_poll(0);
			GASNETC_OFI_LOCK();
			ret = fi_inject(gasnetc_ofi_am_epfd, sendbuf, len, GET_AM_DEST(dest));
			GASNETC_OFI_UNLOCK();
		}
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_inject for short ashort failed: %d\n", ret);

		/* Data buffer is ready for reuse, handle it by callback function */
		header->callback(NULL, header);
	} else {
		GASNETC_OFI_LOCK();
		ret = fi_send(gasnetc_ofi_am_epfd, sendbuf, len, NULL, GET_AM_DEST(dest), &header->ctxt);
		GASNETC_OFI_UNLOCK();
		while (ret == -FI_EAGAIN) {
			gasnetc_ofi_am_send_poll(0);
			ret = fi_send(gasnetc_ofi_am_epfd, sendbuf, len, NULL, GET_AM_DEST(dest), &header->ctxt);
		}
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_send for short am failed: %d\n", ret);
		gasnetc_paratomic_increment(&pending_am,0);
	}
	return ret;
}

int gasnetc_ofi_am_send_medium(gasnet_node_t dest, gasnet_handler_t handler, 
                     void *source_addr, size_t nbytes,   /* data payload */
                     int numargs, va_list argptr, int isreq)
{
	int ret = FI_SUCCESS;
	gasnet_handlerarg_t *arglist;
	int i, len;
	ofi_am_buf_t *header;
	ofi_am_send_buf_t *sendbuf;

	gasneti_assert (nbytes <= gasnet_AMMaxMedium());

	/* Get a send buffer */
	header = gasnetc_ofi_am_header();

	/* Fill in the arguments */
	sendbuf = &header->sendbuf;
	sendbuf->len = 0;
	arglist = (gasnet_handlerarg_t*) sendbuf->data;
	for (i = 0 ; i < numargs ; ++i) {
		arglist[i] = va_arg(argptr, gasnet_handlerarg_t);
		sendbuf->len += sizeof(gasnet_handlerarg_t);
	}
	sendbuf->len = GASNETI_ALIGNUP(sendbuf->len, GASNETI_MEDBUF_ALIGNMENT);

	memcpy((uint8_t *)(sendbuf->data)+sendbuf->len, source_addr, nbytes);
	sendbuf->len += nbytes;

	/* Copy arg and handle into the buffer */
	sendbuf->isreq = isreq;
	sendbuf->handler = (uint8_t) handler;
	sendbuf->sourceid = gasneti_mynode;
	sendbuf->type = OFI_AM_MEDIUM;
	sendbuf->argnum = numargs;
	sendbuf->nbytes = nbytes;

	len = GASNETI_ALIGNUP(sendbuf->len + offsetof(ofi_am_send_buf_t, data), GASNETI_MEDBUF_ALIGNMENT);

#if !GASNET_PSHM
	if (dest == gasneti_mynode) {
		gasnetc_ofi_handle_local_am(header);
		return 0;
	}
#endif

	if(len < max_buffered_send) {
		GASNETC_OFI_LOCK();
		ret = fi_inject(gasnetc_ofi_am_epfd, sendbuf, len, GET_AM_DEST(dest));
		GASNETC_OFI_UNLOCK();
		while (ret == -FI_EAGAIN) {
			gasnetc_ofi_am_send_poll(0);
			GASNETC_OFI_LOCK();
			ret = fi_inject(gasnetc_ofi_am_epfd, sendbuf, len, GET_AM_DEST(dest));
			GASNETC_OFI_UNLOCK();
		}
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_inject for medium ashort failed: %d\n", ret);
		header->callback(NULL, header);
	} else {
		GASNETC_OFI_LOCK();
		ret = fi_send(gasnetc_ofi_am_epfd, sendbuf, len, NULL, GET_AM_DEST(dest), &header->ctxt);
		GASNETC_OFI_UNLOCK();
		while (ret == -FI_EAGAIN) {
			gasnetc_ofi_am_send_poll(0);
			GASNETC_OFI_LOCK();
			ret = fi_send(gasnetc_ofi_am_epfd, sendbuf, len, NULL, GET_AM_DEST(dest), &header->ctxt);
			GASNETC_OFI_UNLOCK();
		}
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_send for meduim am failed: %d\n", ret);
		gasnetc_paratomic_increment(&pending_am,0);
	}

	return ret;
}

int gasnetc_ofi_am_send_long(gasnet_node_t dest, gasnet_handler_t handler,
		               void *source_addr, size_t nbytes,   /* data payload */
		               void *dest_addr,
		               int numargs, va_list argptr, int isreq, int isasync)
{
	int ret = FI_SUCCESS;
	gasnet_handlerarg_t *arglist;
	int i, len;
	ofi_am_buf_t *header;
	ofi_am_send_buf_t *sendbuf;

	if(isreq)
		gasneti_assert (nbytes <= gasnet_AMMaxLongRequest());
	else
		gasneti_assert (nbytes <= gasnet_AMMaxLongReply());

	/* Get a send buffer */
	header = gasnetc_ofi_am_header();

	/* Fill in the arguments */
	sendbuf = &header->sendbuf;
	sendbuf->len = 0;
	arglist = (gasnet_handlerarg_t*) sendbuf->data;
	for (i = 0 ; i < numargs ; ++i) {
		arglist[i] = va_arg(argptr, gasnet_handlerarg_t);
		sendbuf->len += sizeof(gasnet_handlerarg_t);
	}
	sendbuf->len = GASNETI_ALIGNUP(sendbuf->len, GASNETI_MEDBUF_ALIGNMENT);

#if !GASNET_PSHM
	if(dest == gasneti_mynode) {
		memcpy(dest_addr, source_addr, nbytes);
		sendbuf->type = OFI_AM_LONG;
	} else
#endif
	if(sendbuf->len + nbytes < OFI_AM_MAX_DATA_LENGTH)
	{
		/* Pack the payload if it's small enough */
		memcpy(sendbuf->data + sendbuf->len, source_addr, nbytes);
		sendbuf->len += nbytes;
		sendbuf->type = OFI_AM_LONG_MEDIUM;
	} else {
		/* Launch the long data payload transfer with RMA operation */
			ofi_op_ctxt_t lam_ctxt;
			lam_ctxt.type = OFI_TYPE_AM_DATA;
			lam_ctxt.data_sent = 0;
			lam_ctxt.callback = gasnetc_ofi_handle_rdma;
			GASNETC_OFI_LOCK();
			ret = fi_write(gasnetc_ofi_rdma_epfd, source_addr, nbytes, NULL, 
					GET_RDMA_DEST(dest), GET_DSTADDR(dest_addr, dest), 0, &lam_ctxt.ctxt);
			GASNETC_OFI_UNLOCK();
			while (ret == -FI_EAGAIN) {
				gasnetc_ofi_rdma_poll(0);
				ret = fi_write(gasnetc_ofi_rdma_epfd, source_addr, nbytes, NULL, 
						GET_RDMA_DEST(dest), GET_DSTADDR(dest_addr, dest), 0, &lam_ctxt.ctxt);
			}
			if (FI_SUCCESS != ret) 
				gasneti_fatalerror("fi_write failed for AM long: %d\n", ret);
			gasnetc_paratomic_increment(&pending_rdma,0);

			/* Because the order is not guaranteed between different ep, */
			/* we send the am part after confirming the large rdma operation */
			/* is successful. */
			while(!lam_ctxt.data_sent) {
#if GASNET_PSHM
				gasneti_AMPSHMPoll(0);
#endif
				gasnetc_ofi_rdma_poll(0);
			}
		sendbuf->type = OFI_AM_LONG;
	}

	/* Copy arg and handle into the buffer */
	sendbuf->isreq = isreq;
	sendbuf->handler = (uint8_t) handler;
	sendbuf->sourceid = gasneti_mynode;
	sendbuf->argnum = numargs;
	sendbuf->dest_ptr = dest_addr;
	sendbuf->nbytes = nbytes;

	len = GASNETI_ALIGNUP(sendbuf->len + offsetof(ofi_am_send_buf_t, data), 
			GASNETI_MEDBUF_ALIGNMENT);

#if !GASNET_PSHM
	if (dest == gasneti_mynode) {
		gasnetc_ofi_handle_local_am(header);
		return 0;
	}
#endif

	if(len < max_buffered_send) {
		GASNETC_OFI_LOCK();
		ret = fi_inject(gasnetc_ofi_am_epfd, sendbuf, len, GET_AM_DEST(dest));
		GASNETC_OFI_UNLOCK();
		while (ret == -FI_EAGAIN) {
			gasnetc_ofi_am_send_poll(0);
			ret = fi_inject(gasnetc_ofi_am_epfd, sendbuf, len, GET_AM_DEST(dest));
		}
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_inject for long ashort failed: %d\n", ret);
		header->callback(NULL, header);
	} else {
		GASNETC_OFI_LOCK();
		ret = fi_send(gasnetc_ofi_am_epfd, sendbuf, len, NULL, GET_AM_DEST(dest), &header->ctxt);
		GASNETC_OFI_UNLOCK();
		while (ret == -FI_EAGAIN) {
			gasnetc_ofi_am_send_poll(0);
			GASNETC_OFI_LOCK();
			ret = fi_send(gasnetc_ofi_am_epfd, sendbuf, len, NULL, GET_AM_DEST(dest), &header->ctxt);
			GASNETC_OFI_UNLOCK();
		}
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_send for long am failed: %d\n", ret);
		gasnetc_paratomic_increment(&pending_am,0);
	}

	return ret;
}

/*------------------------------------------------
 * OFI conduit one-sided put/get functions
 * ----------------------------------------------*/

void
gasnetc_rdma_put(gasnet_node_t dest, void *dest_addr, void *src_addr, size_t nbytes,
		void *ctxt_ptr)
{
	int ret = FI_SUCCESS;

	((ofi_op_ctxt_t *)ctxt_ptr)->callback = gasnetc_ofi_handle_rdma;
	GASNETC_OFI_LOCK();
	ret = fi_write(gasnetc_ofi_rdma_epfd, src_addr, nbytes, NULL, GET_RDMA_DEST(dest), 
			GET_DSTADDR(dest_addr, dest), 0, ctxt_ptr);
	GASNETC_OFI_UNLOCK();
	while (ret == -FI_EAGAIN) {
		gasnetc_ofi_rdma_poll(0);
		GASNETC_OFI_LOCK();
		ret = fi_write(gasnetc_ofi_rdma_epfd, src_addr, nbytes, NULL, GET_RDMA_DEST(dest), 
				GET_DSTADDR(dest_addr, dest), 0, ctxt_ptr);
		GASNETC_OFI_UNLOCK();
	}
	if (FI_SUCCESS != ret)
		gasneti_fatalerror("fi_write for normal message failed: %d\n", ret);
	gasnetc_paratomic_increment(&pending_rdma,0);
}

void
gasnetc_rdma_get(void *dest_addr, gasnet_node_t dest, void * src_addr, size_t nbytes,
		void *ctxt_ptr)
{
	int ret = FI_SUCCESS;

	((ofi_op_ctxt_t *)ctxt_ptr)->callback = gasnetc_ofi_handle_rdma;
	GASNETC_OFI_LOCK();
	ret = fi_read(gasnetc_ofi_rdma_epfd, dest_addr, nbytes, NULL, GET_RDMA_DEST(dest), 
			GET_SRCADDR(src_addr, dest), 0, ctxt_ptr);
	GASNETC_OFI_UNLOCK();
	while (ret == -FI_EAGAIN) {
		gasnetc_ofi_rdma_poll(0);
		GASNETC_OFI_LOCK();
		ret = fi_read(gasnetc_ofi_rdma_epfd, dest_addr, nbytes, NULL, GET_RDMA_DEST(dest), 
				GET_SRCADDR(src_addr, dest), 0, ctxt_ptr);
		GASNETC_OFI_UNLOCK();
	}
	if (FI_SUCCESS != ret)
		gasneti_fatalerror("fi_read failed: %d\n", ret);
	gasnetc_paratomic_increment(&pending_rdma,0);
}

void
gasnetc_rdma_put_wait(gasnet_handle_t oph)
{
	int ret;
	gasnete_op_t *op = (gasnete_op_t*) oph;

	if (OPTYPE(op) == OPTYPE_EXPLICIT) {
		gasnete_eop_t *eop = (gasnete_eop_t *)op;
		while (!GASNETE_EOP_DONE(eop)) {
#if GASNET_PSHM
			gasneti_AMPSHMPoll(0);
#endif
			gasnetc_ofi_rdma_poll(0);
		}
	} else {
		gasnete_iop_t *iop = (gasnete_iop_t *)op;
		while (!GASNETE_IOP_CNTDONE(iop,put)) {
#if GASNET_PSHM
			gasneti_AMPSHMPoll(0);
#endif
			gasnetc_ofi_rdma_poll(0);
		}
	}
}

void
gasnetc_rdma_get_wait(gasnet_handle_t oph)
{
	int ret;
	gasnete_op_t *op = (gasnete_op_t*) oph;

	if (OPTYPE(op) == OPTYPE_EXPLICIT) {
		gasnete_eop_t *eop = (gasnete_eop_t *)op;
		while (!GASNETE_EOP_DONE(eop)) {
#if GASNET_PSHM
			gasneti_AMPSHMPoll(0);
#endif
			gasnetc_ofi_rdma_poll(0);
		}
	} else {
		gasnete_iop_t *iop = (gasnete_iop_t *)op;
		while (!GASNETE_IOP_CNTDONE(iop,get)) {
#if GASNET_PSHM
			gasneti_AMPSHMPoll(0);
#endif
			gasnetc_ofi_rdma_poll(0);
		}
	}
}
