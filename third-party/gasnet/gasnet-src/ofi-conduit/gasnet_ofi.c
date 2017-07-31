/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ofi-conduit/gasnet_ofi.c $
 * Description: GASNet libfabric (OFI) conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2015, Intel Corporation
 * Terms of use are as specified in license.txt
 */
#include <gasnet_core_internal.h>
#include <gasnet_ofi.h>
#include <gasnet_extended_internal.h>
#include <gasnet_handler.h>

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
#define GET_REMOTEADDR(remote_addr, dest) (uintptr_t)((char*)remote_addr - (char*)gasneti_seginfo[dest].addr)
#else
#define GET_REMOTEADDR(remote_addr, dest) (uintptr_t)remote_addr
#endif

/* FI_MR_BASIC requires addressing by full virtual address */
#define GET_REMOTEADDR_PER_MR_MODE(dest_addr, dest)\
    GASNETC_OFI_HAS_MR_SCALABLE ? GET_REMOTEADDR(dest_addr, dest) : (uintptr_t)dest_addr


static short has_mr_scalable = 0;
/* This pointer will only be malloced if GASNETC_OFI_HAS_MR_SCALABLE is
 * true at runtime */
static uint64_t* gasnetc_ofi_target_keys;
#ifndef GASNETC_OFI_HAS_MR_SCALABLE
#define GASNETC_OFI_HAS_MR_SCALABLE has_mr_scalable
#endif

#define GASNETC_OFI_GET_MR_KEY(dest) (gasneti_assert(!GASNETC_OFI_HAS_MR_SCALABLE),\
        gasnetc_ofi_target_keys[dest])

#define OFI_WRITE(ep, src_addr, nbytes, dest, dest_addr, ctxt_ptr)\
    do {\
        if (GASNETC_OFI_HAS_MR_SCALABLE){\
            ret = fi_write(ep, src_addr, nbytes, NULL, GET_RDMA_DEST(dest), \
                GET_REMOTEADDR(dest_addr, dest), 0, ctxt_ptr);\
        }\
        else {\
            ret = fi_write(ep, src_addr, nbytes, NULL, GET_RDMA_DEST(dest), \
                (uintptr_t)dest_addr, GASNETC_OFI_GET_MR_KEY(dest), ctxt_ptr);\
        }\
    } while(0)

#define OFI_READ(ep, dest_buf, nbytes, src, src_addr, ctxt_ptr)\
    do {\
        if (GASNETC_OFI_HAS_MR_SCALABLE) {\
            ret = fi_read(ep, dest_buf, nbytes, NULL, GET_RDMA_DEST(src), \
                GET_REMOTEADDR(src_addr, src), 0, ctxt_ptr);\
        }\
        else {\
            ret = fi_read(ep, dest_buf, nbytes, NULL, GET_RDMA_DEST(src), \
                (uintptr_t)src_addr, GASNETC_OFI_GET_MR_KEY(dest),ctxt_ptr);\
        }\
    } while(0)

/* Poll periodically on RMA injection to ensure efficient progress.
 * This is a data race, but it is safe as polling here is unnecessary, it
 * simply improves performance in case of floods of RMA requests*/
static int rdma_poll_frequency = 0;
static int rdma_periodic_poll_threshold; /* Set via environment variable in init() */
#define PERIODIC_RMA_POLL() do{\
    if_pf(rdma_poll_frequency++ >= rdma_periodic_poll_threshold){\
        rdma_poll_frequency=0;\
        gasnetc_ofi_tx_poll();\
    }} while(0)

/* In this case, inject means "inject into the network". It is not specific
 * to the fi_inject/fi_inject_write functions. fxn must assign a return value
 * to an int named "ret" in the scope in which it is called.*/
#define OFI_INJECT_RETRY(lock, fxn)\
    do {\
        GASNETC_OFI_LOCK_EXPR(lock, fxn);\
        while (ret == -FI_EAGAIN) {\
            GASNETI_WAITHOOK();\
            GASNETC_OFI_POLL_EVERYTHING();\
            GASNETC_OFI_LOCK_EXPR(lock, fxn);\
        }\
    }while(0)

static gasneti_lifo_head_t ofi_am_pool = GASNETI_LIFO_INITIALIZER;
static gasneti_lifo_head_t ofi_bbuf_pool = GASNETI_LIFO_INITIALIZER;
static gasneti_lifo_head_t ofi_bbuf_ctxt_pool = GASNETI_LIFO_INITIALIZER;

#define OFI_AM_NUM_BLOCK 	8
#define OFI_AM_BLOCK_SIZE 	1*1024*1024

/* Variables for bounce buffering of non-blocking, non-bulk puts.
 * The gasnetc_ofi_bbuf_threshold variable is defined in gasnet_ofi.h
 * as it is needed in other files */
static void* bounce_region_start = NULL;
static size_t bounce_region_size = 0;
static size_t ofi_num_bbufs; 
static size_t ofi_bbuf_size;
#define OFI_MAX_NUM_BOUNCE_BUFFERS 32

static uint64_t             	max_buffered_send;
static uint64_t             	min_multi_recv;

static int using_psm_provider = 0;

gasneti_spawnerfn_t const *gasneti_spawner = NULL;

static struct iovec *am_iov;
static struct fi_msg *am_buff_msg;
static gasnetc_ofi_ctxt_t *am_buff_ctxt = NULL;

static gasnetc_paratomic_t pending_rdma = gasnetc_paratomic_init(0);
static gasnetc_paratomic_t pending_am = gasnetc_paratomic_init(0);

static int gasnetc_ofi_inited = 0;

#define OFI_CONDUIT_VERSION FI_VERSION(1, 0)

#if GASNET_PSHM
#define GASNETC_OFI_POLL_EVERYTHING() do { gasnetc_ofi_poll(); gasneti_AMPSHMPoll(0);} while (0)
#else
#define GASNETC_OFI_POLL_EVERYTHING() do { gasnetc_ofi_poll();} while (0)
#endif

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

/*-------------------------------------------------
 * Function Declarations
 *-------------------------------------------------*/
GASNETI_INLINE(gasnetc_ofi_handle_am)
void gasnetc_ofi_handle_am(struct fi_cq_data_entry *re, void *buf);

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
  int num_locks; 
  int i;
  int iov_len = OFI_AM_BLOCK_SIZE;
  int num_iov = OFI_AM_NUM_BLOCK;
  
  conn_entry_t *mapped_table;
  int high_perf_prov = 0;

  gasneti_spawner = gasneti_spawnerInit(argc, argv, NULL, &gasneti_nodes, &gasneti_mynode);
  if (!gasneti_spawner) GASNETI_RETURN_ERRR(NOT_INIT, "GASNet job spawn failed");

  /* Ensure uniform FI_* env vars */
  /* TODO: what about provider-specific env vars? */
  gasneti_propagate_env("FI_", GASNETI_PROPAGATE_ENV_PREFIX);

#if GASNETC_OFI_USE_THREAD_DOMAIN && GASNET_PAR
  gasneti_spinlock_init(&gasnetc_ofi_locks.big_lock);
#elif GASNET_PAR
  /* This lock is needed in PAR mode to protect the AM reference counting */
  gasneti_spinlock_init(&gasnetc_ofi_locks.rx_cq);
#endif
#if 0
  gasneti_spinlock_init(&gasnetc_ofi_locks.tx_cq);
  gasneti_spinlock_init(&gasnetc_ofi_locks.am_rx);
  gasneti_spinlock_init(&gasnetc_ofi_locks.am_tx);
  gasneti_spinlock_init(&gasnetc_ofi_locks.rdma_rx);
  gasneti_spinlock_init(&gasnetc_ofi_locks.rdma_tx);
#endif

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
  /* Threading mode is set by the configure script to FI_THREAD_DOMAIN if
   * using the psm or psm2 provider and FI_THREAD_SAFE otherwise*/
#if GASNETC_OFI_USE_THREAD_DOMAIN || !GASNET_PAR
  hints->domain_attr->threading			= FI_THREAD_DOMAIN;
#else
  hints->domain_attr->threading			= FI_THREAD_SAFE;
#endif

  hints->domain_attr->control_progress	= FI_PROGRESS_MANUAL;
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

  if (!strcmp(info->fabric_attr->prov_name, "psm") ||
		  !strcmp(info->fabric_attr->prov_name, "psm2")){
	  high_perf_prov = 1;
      using_psm_provider = 1;
  }

  int quiet = gasneti_getenv_yesno_withdefault("GASNET_QUIET", 0);
#if GASNET_PAR
  if(!*mynode_p) {
      if (!using_psm_provider && GASNETC_OFI_USE_THREAD_DOMAIN) {
          const char * msg =
            "WARNING: Using OFI provider \"%s\" when the ofi-conduit was configured for FI_THREAD_DOMAIN\n"
            "(possibly because the psm or psm2 provider was detected at configure time). In GASNET_PAR mode,\n"
            "this has the effect of using a global lock instead of fine-grained locking. If this causes \n"
            "undesirable performance in PAR, reconfigure GASNet using: --with-ofi-provider=%s --disable-thread-domain\n";
          if (!quiet)
              fprintf(stderr, msg, info->fabric_attr->prov_name, info->fabric_attr->prov_name);
      }
  }
#endif

  if (!high_perf_prov && !*mynode_p) {
          const char * msg = 
          "WARNING: Using OFI provider (%s), which has not been validated to provide\n"
          "WARNING: acceptable GASNet performance. You should consider using a more\n"
          "WARNING: hardware-appropriate GASNet conduit. See ofi-conduit/README.\n";
	  if (!quiet)
		  fprintf(stderr, msg, info->fabric_attr->prov_name);
  }

  if(info->domain_attr->mr_mode == FI_MR_SCALABLE) {
      has_mr_scalable = 1;
  }
  if (GASNETC_OFI_HAS_MR_SCALABLE != has_mr_scalable) {
      gasneti_fatalerror("The statically-determined value for GASNETC_OFI_HAS_MR_SCALABLE=%i does\n"
                         "  not match the memory registration support that the (%s) provider reported.\n"
                         "  This could happen if a provider that previously only supported FI_MR_BASIC\n"
                         "  added support for FI_MR_SCALABLE, or if the wrong provider was selected at runtime.\n"
                         "  Use configure option --%s-ofi-mr-scalable to correct this.",
                         GASNETC_OFI_HAS_MR_SCALABLE, 
                         info->fabric_attr->prov_name,
                         (has_mr_scalable ? "enable" : "disable"));
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

  /* Allocate a CQ that will be shared for both RDMA and AM tx ops */
  memset(&cq_attr, 0, sizeof(cq_attr));
  cq_attr.format    = FI_CQ_FORMAT_DATA; /* Provides data associated with a completion */
  ret = fi_cq_open(gasnetc_ofi_domainfd, &cq_attr, &gasnetc_ofi_tx_cqfd, NULL);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_cq_open for rdma_eqfd failed: %d\n", ret);

  /* Allocate recv completion queues for AMs */
  memset(&cq_attr, 0, sizeof(cq_attr));
  cq_attr.format    = FI_CQ_FORMAT_DATA;
  ret = fi_cq_open(gasnetc_ofi_domainfd, &cq_attr, &gasnetc_ofi_am_rcqfd, NULL);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_cq_open for am recv cq failed: %d\n", ret);

  /* Bind CQs to endpoints */
  ret = fi_ep_bind(gasnetc_ofi_rdma_epfd, &gasnetc_ofi_tx_cqfd->fid, FI_TRANSMIT);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_ep_bind for tx_cq to rdma_epfd failed: %d\n", ret);

  ret = fi_ep_bind(gasnetc_ofi_am_epfd, &gasnetc_ofi_tx_cqfd->fid, FI_TRANSMIT);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_ep_bind for tx_cq to am_epfd failed: %d\n", ret);

  ret = fi_ep_bind(gasnetc_ofi_am_epfd, &gasnetc_ofi_am_rcqfd->fid, FI_RECV);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_ep_bind for am_rcqfd to am_epfd failed: %d\n", ret);

  /* Low-water mark for shared receive buffer */
  min_multi_recv = OFI_AM_MAX_DATA_LENGTH + offsetof(gasnetc_ofi_am_send_buf_t,data);
  optlen = min_multi_recv;
  ret	 = fi_setopt(&gasnetc_ofi_am_epfd->fid, FI_OPT_ENDPOINT, FI_OPT_MIN_MULTI_RECV,
		  &optlen,
		  sizeof(optlen));
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_setopt for am_epfd failed: %d\n", ret);

  /* Cutoff to use fi_inject */
  max_buffered_send = info->tx_attr->inject_size;

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

  gasneti_nodemapInit(gasneti_bootstrapExchange, NULL, 0, 0);

  /* Get the address of AM endpoint and publish to other nodes through bootstrap
   * exchange function */
  ret = fi_getname(&gasnetc_ofi_am_epfd->fid, sockname, &socknamelen);
  ret = fi_getname(&gasnetc_ofi_rdma_epfd->fid, sockname+socknamelen, &socknamelen);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_getepname failed: %d\n", ret);
  alladdrs = gasneti_malloc(gasneti_nodes*socknamelen*2);
  gasneti_bootstrapExchange(&sockname, socknamelen*2, alladdrs);

  ret = fi_av_insert(gasnetc_ofi_avfd, alladdrs, gasneti_nodes*2, (fi_addr_t*)mapped_table,
          0ULL, NULL);
  if (gasneti_nodes*2 != ret) 
      gasneti_fatalerror("fi_av_insert failed. Expected: %d Actual: %d\n", gasneti_nodes*2, ret);
  
  gasneti_free(alladdrs);

  fi_freeinfo(hints);

  if (!GASNETC_OFI_HAS_MR_SCALABLE) {
      gasnetc_ofi_target_keys = gasneti_malloc(sizeof(uint64_t)*gasneti_nodes);
      gasneti_assert(gasnetc_ofi_target_keys);
  }

  /* Receive buffers to post */
  am_iov = (struct iovec *) gasneti_malloc(sizeof(struct iovec)*num_iov);
  am_buff_msg = (struct fi_msg *) gasneti_malloc(sizeof(struct fi_msg)*num_iov);
  am_buff_ctxt = (gasnetc_ofi_ctxt_t *) gasneti_malloc(sizeof(gasnetc_ofi_ctxt_t)*num_iov);

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
        am_buff_ctxt[i].final_cntr = 0;
        am_buff_ctxt[i].event_cntr = 0;
        gasnetc_paratomic_set(&am_buff_ctxt[i].consumed_cntr, 0, 0);
		/* Post buffers for Active Messages */
		ret = fi_recvmsg(gasnetc_ofi_am_epfd, &am_buff_msg[i], FI_MULTI_RECV);
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_recvmsg failed: %d\n", ret);
	}

  /* The number of RMA requests to be issued before a tx_poll takes place */
  rdma_periodic_poll_threshold = gasneti_getenv_int_withdefault("GASNET_OFI_RMA_POLL_FREQ", 32, 0);

  /* Allocate bounce buffers*/
  ofi_num_bbufs = gasneti_getenv_int_withdefault("GASNET_OFI_NUM_BBUFS", 64, 0);
  ofi_bbuf_size = gasneti_getenv_int_withdefault("GASNET_OFI_BBUF_SIZE", GASNET_PAGESIZE, 1);
  gasnetc_ofi_bbuf_threshold = gasneti_getenv_int_withdefault("GASNET_OFI_BBUF_THRESHOLD", 4*ofi_bbuf_size, 1);

  if (ofi_num_bbufs < gasnetc_ofi_bbuf_threshold/ofi_bbuf_size)
      gasneti_fatalerror("The number of bounce buffers must be greater than or equal to the bounce\n"
              "buffer threshold divided by the bounce buffer size. See the ofi-conduit README.\n");

  if (gasnetc_ofi_bbuf_threshold/ofi_bbuf_size > OFI_MAX_NUM_BOUNCE_BUFFERS) {
      gasneti_fatalerror("The ofi-conduit limits the max number of bounce buffers used in the non-blocking\n"
              "put path to %d. Your selections for the bounce buffer tuning parameters exceed this. If you\n" 
              "truly need more than %d bounce buffers, edit the OFI_MAX_NUM_BOUNCE_BUFFERS macro in\n" 
              "gasnet_ofi.c and recompile.\n", OFI_MAX_NUM_BOUNCE_BUFFERS, OFI_MAX_NUM_BOUNCE_BUFFERS);
  }

  bounce_region_size = GASNETI_PAGE_ALIGNUP(ofi_num_bbufs * ofi_bbuf_size);
  bounce_region_start = gasneti_malloc_aligned(GASNETI_PAGESIZE, bounce_region_size);

  gasneti_leak_aligned(bounce_region_start);
  /* Progress backwards so that when these buffers are added to the stack, they
   * will come off of it in order by address */
  char* buf = (char*)bounce_region_start + (ofi_num_bbufs-1)*ofi_bbuf_size;
  for (i = 0; i < (int)ofi_num_bbufs; i++) {
      gasneti_assert(buf);
      gasnetc_ofi_bounce_buf_t* container = gasneti_malloc(sizeof(gasnetc_ofi_bounce_op_ctxt_t));
      container->buf = buf;
      gasneti_lifo_push(&ofi_bbuf_pool, container);
      buf -= ofi_bbuf_size;
  }
  gasnetc_ofi_inited = 1;
  return GASNET_OK;
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

#if GASNET_PAR && GASNETC_OFI_USE_THREAD_DOMAIN
  // Attempt to obtain (and *never* release) the big_lock in bounded time
  const uint64_t timeout_ns = 10 * 1000000000L; // TODO: arbitrary 10s
  const gasneti_tick_t t_start = gasneti_ticks_now();
  while (EBUSY == GASNETC_OFI_TRYLOCK(&gasnetc_ofi_locks.big_lock)) {
    if (timeout_ns < gasneti_ticks_to_ns(gasneti_ticks_now() - t_start)) {
      return; // Give up after time out
    }
    GASNETI_WAITHOOK();
  }
#endif

  while(gasnetc_paratomic_read(&pending_am,0) ||
      gasnetc_paratomic_read(&pending_rdma,0))
    GASNETC_OFI_POLL_EVERYTHING();

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
    #elif GASNETI_CLIENT_THREADS
      /* Unsafe to free AM buffers if other threads may be using them */
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

  if(fi_close(&gasnetc_ofi_tx_cqfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am scqfd failed\n");
  }

  if(fi_close(&gasnetc_ofi_am_rcqfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am rcqfd failed\n");
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

  if (!GASNETC_OFI_HAS_MR_SCALABLE)
      gasneti_free(gasnetc_ofi_target_keys);
#if USE_AV_MAP
  gasneti_free(addr_table);
#endif
}

/*------------------------------------------------
 * OFI conduit callback functions
 * ----------------------------------------------*/

/* Handle Active Messages */
GASNETI_INLINE(gasnetc_ofi_handle_am_common)
void gasnetc_ofi_handle_am_common(gasnetc_ofi_am_send_buf_t *header)
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
void gasnetc_ofi_handle_local_am(gasnetc_ofi_am_buf_t *buf)
{
    gasnetc_ofi_handle_am_common(&buf->sendbuf);
    gasneti_lifo_push(&ofi_am_pool, buf);
}
#endif

/* Handle incoming Active Messages */
GASNETI_INLINE(gasnetc_ofi_handle_am)
void gasnetc_ofi_handle_am(struct fi_cq_data_entry *re, void *buf)
{
    gasnetc_ofi_handle_am_common((gasnetc_ofi_am_send_buf_t*)re->buf);
}

/* Handle RDMA completion as the initiator */
GASNETI_INLINE(gasnetc_ofi_handle_rdma)
void gasnetc_ofi_handle_rdma(void *buf)
{

	gasnetc_ofi_op_ctxt_t *ptr = (gasnetc_ofi_op_ctxt_t*)buf;

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
	gasnetc_ofi_am_buf_t *header = (gasnetc_ofi_am_buf_t*)buf;
	gasneti_lifo_push(&ofi_am_pool, header);
}

/* Get a send buffer */
GASNETI_INLINE(gasnetc_ofi_am_header)
gasnetc_ofi_am_buf_t *gasnetc_ofi_am_header(void)
{
	gasnetc_ofi_am_buf_t *header = gasneti_lifo_pop(&ofi_am_pool);
	if_pf (NULL == header) {
		header = gasneti_malloc(sizeof(gasnetc_ofi_am_buf_t));
		header->callback = gasnetc_ofi_release_am;
		gasneti_leak(header);
	}
    return header;
}

GASNETI_INLINE(gasnetc_ofi_get_bounce_ctxt)
gasnetc_ofi_bounce_op_ctxt_t* gasnetc_ofi_get_bounce_ctxt(void)
{
    gasnetc_ofi_bounce_op_ctxt_t* ctxt = gasneti_lifo_pop(&ofi_bbuf_ctxt_pool);
    if (NULL == ctxt) {
        ctxt = gasneti_calloc(1,sizeof(gasnetc_ofi_bounce_op_ctxt_t));
        gasneti_lifo_init(&ctxt->bbuf_list);
        gasneti_leak(ctxt);
    }
    return ctxt;
}

void gasnetc_ofi_handle_bounce_rdma(void *buf)
{
    gasnetc_ofi_bounce_op_ctxt_t* op = (gasnetc_ofi_bounce_op_ctxt_t*) buf;
    if (gasnetc_paratomic_decrement_and_test(&op->cntr, 0)) {
        gasnetc_ofi_op_ctxt_t* orig_op = op->orig_op;
        gasnetc_ofi_bounce_buf_t * bbuf_to_return;
        while (NULL != (bbuf_to_return = gasneti_lifo_pop(&op->bbuf_list)))
            gasneti_lifo_push(&ofi_bbuf_pool, bbuf_to_return);
        /* These completions will always be RDMA, so call the callback directly */
        gasnetc_ofi_handle_rdma(orig_op); 
        gasneti_lifo_push(&ofi_bbuf_ctxt_pool, op);
    }
}

/*------------------------------------------------
 * Pre-post or pin-down memory
 * ----------------------------------------------*/
void gasnetc_ofi_attach(void *segbase, uintptr_t segsize)
{
	int ret = FI_SUCCESS;
    uint64_t local_mr_key;

	/* Pin-down Memory Region */
#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
	ret = fi_mr_reg(gasnetc_ofi_domainfd, segbase, segsize, FI_REMOTE_READ | FI_REMOTE_WRITE, 0ULL, 0ULL, 0ULL, &gasnetc_ofi_rdma_mrfd, NULL);
#else
	ret = fi_mr_reg(gasnetc_ofi_domainfd, (void *)0, UINT64_MAX, FI_REMOTE_READ | FI_REMOTE_WRITE, 0ULL, 0ULL, 0ULL, &gasnetc_ofi_rdma_mrfd, NULL);
    if (!GASNETC_OFI_HAS_MR_SCALABLE) {
        gasneti_fatalerror("GASNET_SEGMENT_EVERYTHING is not supported when using FI_MR_BASIC.\n"
                           "Pick an OFI provider that supports FI_MR_SCALABLE if EVERYTHING\n"
                           "is needed.\n");
    }
#endif
	if (FI_SUCCESS != ret) gasneti_fatalerror("fi_mr_reg for rdma failed: %d\n", ret);

    /* Exchange memory keys with other nodes.*/
    if (!GASNETC_OFI_HAS_MR_SCALABLE) {
        local_mr_key = fi_mr_key(gasnetc_ofi_rdma_mrfd);
        gasneti_bootstrapExchange(&local_mr_key, sizeof(uint64_t),
                gasnetc_ofi_target_keys);
    }

}


/*------------------------------------------------
 * OFI conduit network poll function
 * ----------------------------------------------*/

/* TX progress function: Handles both AM and RDMA outgoing operations */
void gasnetc_ofi_tx_poll()
{
	int ret = 0;
    int i;
	struct fi_cq_data_entry re[GASNETC_OFI_NUM_COMPLETIONS];
	struct fi_cq_err_entry e;

	/* Read from Completion Queue */
    /* In the case of using one global lock, a try-lock could prevent progress from
     * occurring if the big lock is being held often by another thread. Just lock in
     * this case */
#if GASNETC_OFI_USE_THREAD_DOMAIN
    GASNETC_OFI_LOCK(&gasnetc_ofi_locks.tx_cq);
#else
    /* If another thread already has the queue lock, return as it is already
     * processing the queue */
    if(EBUSY == GASNETC_OFI_TRYLOCK(&gasnetc_ofi_locks.tx_cq)) return;
#endif
    ret = fi_cq_read(gasnetc_ofi_tx_cqfd, (void *)&re, GASNETC_OFI_NUM_COMPLETIONS);
    GASNETC_OFI_UNLOCK(&gasnetc_ofi_locks.tx_cq);
	if (ret != -FI_EAGAIN)
	{
		if_pf (ret < 0) {
            if (-FI_EAVAIL == ret) {
                GASNETC_OFI_LOCK_EXPR(&gasnetc_ofi_locks.tx_cq,
                   fi_cq_readerr(gasnetc_ofi_tx_cqfd, &e ,0));
                if_pf (gasnetc_is_exit_error(e)) return;
                gasneti_fatalerror("fi_cq_read for tx_poll failed with error: %s\n", fi_strerror(e.err));
            } 
            else
                gasneti_fatalerror("fi_cq_read for tx_poll returned unexpected error code: %d\n", ret);
        } 
        else {
            for (i = 0; i < ret; i++) {
                if (re[i].flags & FI_SEND) {
                    gasnetc_paratomic_decrement(&pending_am, 0);
                    gasnetc_ofi_am_buf_t *header = (gasnetc_ofi_am_buf_t *)re[i].op_context;
                    header->callback(&re[i], header);
                }
                else if(re[i].flags & FI_WRITE || re[i].flags & FI_READ) {
                    gasnetc_paratomic_decrement(&pending_rdma, 0);
                    gasnetc_ofi_op_ctxt_t *header = (gasnetc_ofi_op_ctxt_t *)re[i].op_context;
                    header->callback(header);
                }
                else {
                    gasneti_fatalerror("Unknown completion type received for gasnetc_ofi_tx_poll\n");
                }
            }
        }
    }
}

GASNETI_PLEASE_INLINE(gasnetc_ofi_am_recv_poll)
void gasnetc_ofi_am_recv_poll()
{
	int ret = 0;
    int post_ret = 0;
	struct fi_cq_data_entry re = {0};
	struct fi_cq_err_entry e = {0};

    /* Read from Completion Queue */

    if(EBUSY == GASNETC_OFI_PAR_TRYLOCK(&gasnetc_ofi_locks.rx_cq)) return;

    ret = fi_cq_read(gasnetc_ofi_am_rcqfd, (void *)&re, 1);

    if (ret == -FI_EAGAIN) {
        GASNETC_OFI_PAR_UNLOCK(&gasnetc_ofi_locks.rx_cq);
        return;
    } 
    if_pf (ret < 0) {
        fi_cq_readerr(gasnetc_ofi_am_rcqfd, &e ,0);
        GASNETC_OFI_PAR_UNLOCK(&gasnetc_ofi_locks.rx_cq);
        if_pf (gasnetc_is_exit_error(e)) return;
        gasneti_fatalerror("fi_cq_read for am_recv_poll failed with error: %s\n", fi_strerror(e.err));
    }

    gasnetc_ofi_ctxt_t *header;
    header = (gasnetc_ofi_ctxt_t *)re.op_context;
    /* Count number of completions read for this posted buffer */
    header->event_cntr++;

    /* Record the total number of completions read */
    if_pf (re.flags & FI_MULTI_RECV) {
        header->final_cntr = header->event_cntr;
    }
    GASNETC_OFI_PAR_UNLOCK(&gasnetc_ofi_locks.rx_cq);

    if_pt (re.flags & FI_RECV) header->callback(&re, header);

    /* The atomic here ensures that the buffer is not reposted while an AM handler is
     * still running. */
    uint64_t tmp = gasnetc_paratomic_add(&header->consumed_cntr, 1, GASNETI_ATOMIC_ACQ);
    if_pf (tmp == (GASNETI_ATOMIC_MAX & header->final_cntr)) {
        GASNETC_OFI_LOCK(&gasnetc_ofi_locks.am_rx);
        post_ret = fi_recvmsg(gasnetc_ofi_am_epfd, &(am_buff_msg[header->index]), FI_MULTI_RECV);
        GASNETC_OFI_UNLOCK(&gasnetc_ofi_locks.am_rx);
        if_pf (FI_SUCCESS != post_ret) gasneti_fatalerror("fi_recvmsg failed inside am_recv_poll: %d\n", ret);
    }
}

/* General progress function */
void gasnetc_ofi_poll()
{
    /* These poll functions should be inlined by the compiler */
    if(gasnetc_paratomic_read(&pending_rdma,0) || gasnetc_paratomic_read(&pending_am,0))
        gasnetc_ofi_tx_poll();

    /* We always need to check for incoming AMs */
    gasnetc_ofi_am_recv_poll();
}


/*------------------------------------------------
 * OFI conduit am send functions
 * ----------------------------------------------*/

int gasnetc_ofi_am_send_short(gasnet_node_t dest, gasnet_handler_t handler,
                     int numargs, va_list argptr, int isreq)
{
	int ret = FI_SUCCESS;
	gasnet_handlerarg_t *arglist;
	int i;
    unsigned int len;
	gasnetc_ofi_am_buf_t *header;
	gasnetc_ofi_am_send_buf_t *sendbuf;

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

	len = GASNETI_ALIGNUP(sendbuf->len + offsetof(gasnetc_ofi_am_send_buf_t, data), GASNETI_MEDBUF_ALIGNMENT);

#if !GASNET_PSHM
	if (dest == gasneti_mynode) {
		gasnetc_ofi_handle_local_am(header);
		return 0;
	}
#endif

	if(len <= max_buffered_send) {
            OFI_INJECT_RETRY(&gasnetc_ofi_locks.am_tx,
                ret = fi_inject(gasnetc_ofi_am_epfd, sendbuf, len, GET_AM_DEST(dest)));
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_inject for short am failed: %d\n", ret);

		/* Data buffer is ready for reuse, handle it by callback function */
		header->callback(NULL, header);
	} else {
            OFI_INJECT_RETRY(&gasnetc_ofi_locks.am_tx,
                ret = fi_send(gasnetc_ofi_am_epfd, sendbuf, len, NULL, GET_AM_DEST(dest), &header->ctxt));
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
	int i;
    unsigned int len;
	gasnetc_ofi_am_buf_t *header;
	gasnetc_ofi_am_send_buf_t *sendbuf;

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

	len = GASNETI_ALIGNUP(sendbuf->len + offsetof(gasnetc_ofi_am_send_buf_t, data), GASNETI_MEDBUF_ALIGNMENT);

#if !GASNET_PSHM
	if (dest == gasneti_mynode) {
		gasnetc_ofi_handle_local_am(header);
		return 0;
	}
#endif

	if(len <= max_buffered_send) {
            OFI_INJECT_RETRY(&gasnetc_ofi_locks.am_tx,
                ret = fi_inject(gasnetc_ofi_am_epfd, sendbuf, len, GET_AM_DEST(dest)));
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_inject for medium ashort failed: %d\n", ret);
		header->callback(NULL, header);
	} else {
            OFI_INJECT_RETRY(&gasnetc_ofi_locks.am_tx,
                ret = fi_send(gasnetc_ofi_am_epfd, sendbuf, len, NULL, GET_AM_DEST(dest), &header->ctxt));
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_send for medium am failed: %d\n", ret);
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
	int i;
    unsigned int len;
	gasnetc_ofi_am_buf_t *header;
	gasnetc_ofi_am_send_buf_t *sendbuf;

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
			gasnetc_ofi_op_ctxt_t lam_ctxt;
			lam_ctxt.type = OFI_TYPE_AM_DATA;
			lam_ctxt.data_sent = 0;
			lam_ctxt.callback = gasnetc_ofi_handle_rdma;

			GASNETC_OFI_LOCK_EXPR(&gasnetc_ofi_locks.rdma_tx, 
                OFI_WRITE(gasnetc_ofi_rdma_epfd, source_addr, nbytes, dest, dest_addr, &lam_ctxt.ctxt));
			while (ret == -FI_EAGAIN) {
				GASNETC_OFI_POLL_EVERYTHING();
                GASNETC_OFI_LOCK_EXPR(&gasnetc_ofi_locks.rdma_tx, 
				    OFI_WRITE(gasnetc_ofi_rdma_epfd, source_addr, nbytes, dest, dest_addr, &lam_ctxt.ctxt));
			}
			if (FI_SUCCESS != ret) 
				gasneti_fatalerror("fi_write failed for AM long: %d\n", ret);
			gasnetc_paratomic_increment(&pending_rdma,0);

			/* Because the order is not guaranteed between different ep, */
			/* we send the am part after confirming the large rdma operation */
			/* is successful. */
			while(!lam_ctxt.data_sent) {
				GASNETC_OFI_POLL_EVERYTHING();
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

	len = GASNETI_ALIGNUP(sendbuf->len + offsetof(gasnetc_ofi_am_send_buf_t, data), 
			GASNETI_MEDBUF_ALIGNMENT);

#if !GASNET_PSHM
	if (dest == gasneti_mynode) {
		gasnetc_ofi_handle_local_am(header);
		return 0;
	}
#endif

	if(len <= max_buffered_send) {
        OFI_INJECT_RETRY(&gasnetc_ofi_locks.am_tx,
            ret = fi_inject(gasnetc_ofi_am_epfd, sendbuf, len, GET_AM_DEST(dest)));
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_inject for long ashort failed: %d\n", ret);
		header->callback(NULL, header);
	} else {
        OFI_INJECT_RETRY(&gasnetc_ofi_locks.am_tx,
            ret = fi_send(gasnetc_ofi_am_epfd, sendbuf, len, NULL, GET_AM_DEST(dest), &header->ctxt));

		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_send for long am failed: %d\n", ret);
		gasnetc_paratomic_increment(&pending_am,0);
	}

	return ret;
}

GASNETI_INLINE(get_bounce_bufs)
int get_bounce_bufs(int n, gasnetc_ofi_bounce_buf_t ** arr) {
    int i,j;
    gasnetc_ofi_bounce_buf_t* buf_container;

    for (i = 0; i < n; i++) {
        buf_container = gasneti_lifo_pop(&ofi_bbuf_pool);
        if (!buf_container) {
            for (j = i -1; j >= 0; j--) {
                gasneti_lifo_push(&ofi_bbuf_pool, arr[j]);
            }
            return 0;
        }
        arr[i] = buf_container;
    }
    return 1;
}

/*------------------------------------------------
 * OFI conduit one-sided put/get functions
 * ----------------------------------------------*/

/* There is not a good semantic match between GASNet and OFI in the non-blocking,
 * non-bulk puts due to local completion requirements. This function handles this 
 * special case. 
 *
 * Returns non-zero if a wait is needed for remote completion. Returns 0 if the
 * buffer may be safely returned to the app */
int gasnetc_rdma_put_non_bulk(gasnet_node_t dest, void* dest_addr, void* src_addr, 
        size_t nbytes, gasnetc_ofi_op_ctxt_t* ctxt_ptr)
{

    int i;
    int ret = FI_SUCCESS;
    uintptr_t src_ptr = (uintptr_t)src_addr;
    uintptr_t dest_ptr = GET_REMOTEADDR(dest_addr, dest);

    ((gasnetc_ofi_op_ctxt_t *)ctxt_ptr)->callback = gasnetc_ofi_handle_rdma;

    PERIODIC_RMA_POLL();

    /* The payload can be injected without need for a bounce buffer */
    if (nbytes <= max_buffered_send) {
        uintptr_t dest_ptr = GET_REMOTEADDR_PER_MR_MODE(dest_addr, dest);
        struct fi_msg_rma msg;
        msg.desc = 0;
        msg.addr = GET_RDMA_DEST(dest);
        struct iovec iovec;
        struct fi_rma_iov rma_iov;
        iovec.iov_base = src_addr;
        iovec.iov_len = nbytes;
        rma_iov.addr = dest_ptr;
        rma_iov.len = nbytes;
        rma_iov.key = GASNETC_OFI_HAS_MR_SCALABLE ? 0 : GASNETC_OFI_GET_MR_KEY(dest);

        msg.context = ctxt_ptr;
        msg.msg_iov = &iovec;
        msg.iov_count = 1;
        msg.rma_iov = &rma_iov;
        msg.rma_iov_count = 1;
        
        OFI_INJECT_RETRY(&gasnetc_ofi_locks.rdma_tx,
             ret = fi_writemsg(gasnetc_ofi_rdma_epfd, &msg, FI_INJECT | FI_DELIVERY_COMPLETE ));
        if_pf (FI_SUCCESS != ret)
            gasneti_fatalerror("fi_writemsg with FI_INJECT failed: %d\n", ret);

        gasnetc_paratomic_increment(&pending_rdma,0);
        GASNETC_STAT_EVENT(NB_PUT_INJECT);
        return 0;
    } 
    /* Bounce buffers are needed */
    else if (nbytes <= gasnetc_ofi_bbuf_threshold) {
        uintptr_t dest_ptr = (uintptr_t)dest_addr;
        int num_bufs_needed = nbytes / ofi_bbuf_size;
        size_t bytes_to_copy;
        if(num_bufs_needed == 0) num_bufs_needed = 1;
        gasnetc_ofi_bounce_buf_t* buffs[OFI_MAX_NUM_BOUNCE_BUFFERS];

        /* If there are not enough bounce buffers available, simply block as
         * we don't know when more will become available */
        ret = get_bounce_bufs(num_bufs_needed, buffs);
        if (!ret) goto block_anyways;

        gasnetc_ofi_bounce_op_ctxt_t * bbuf_ctxt = gasnetc_ofi_get_bounce_ctxt();
        bbuf_ctxt->orig_op = ctxt_ptr;
        bbuf_ctxt->callback = gasnetc_ofi_handle_bounce_rdma;
        gasnetc_paratomic_set(&bbuf_ctxt->cntr, num_bufs_needed, 0);

        i = 0;
        gasnetc_ofi_bounce_buf_t* buf_container;

        while (num_bufs_needed > 0) {
            bytes_to_copy = num_bufs_needed != 1 ? ofi_bbuf_size : nbytes;
            buf_container = buffs[i];
            gasneti_lifo_push(&bbuf_ctxt->bbuf_list, buf_container);
            memcpy(buf_container->buf, (void*)src_ptr, bytes_to_copy);

            OFI_INJECT_RETRY(&gasnetc_ofi_locks.rdma_tx,
                OFI_WRITE(gasnetc_ofi_rdma_epfd, buf_container->buf, bytes_to_copy, 
                    dest, dest_ptr, bbuf_ctxt));

            if_pf (FI_SUCCESS != ret)
                gasneti_fatalerror("fi_writemsg for bounce buffered data failed: %d\n", ret);

            gasnetc_paratomic_increment(&pending_rdma,0);

            /* Update our pointers to locations in memory */
            nbytes -= ofi_bbuf_size; 
            dest_ptr += ofi_bbuf_size;
            src_ptr += ofi_bbuf_size;
            num_bufs_needed--;
            i++;
        }

        GASNETC_STAT_EVENT(NB_PUT_BOUNCE);
        return 0;
    }
    /* We tried our best to optimize this. Just wait for remote completion */
    else {
block_anyways:
        gasnetc_rdma_put(dest, dest_addr, src_addr, nbytes, ctxt_ptr);
        GASNETC_STAT_EVENT(NB_PUT_BLOCK);
        return 1;
    }

}

void
gasnetc_rdma_put(gasnet_node_t dest, void *dest_addr, void *src_addr, size_t nbytes,
		gasnetc_ofi_op_ctxt_t *ctxt_ptr)
{
	int ret = FI_SUCCESS;

	((gasnetc_ofi_op_ctxt_t *)ctxt_ptr)->callback = gasnetc_ofi_handle_rdma;

    PERIODIC_RMA_POLL();
    OFI_INJECT_RETRY(&gasnetc_ofi_locks.rdma_tx,
        OFI_WRITE(gasnetc_ofi_rdma_epfd, src_addr, nbytes, dest, dest_addr, ctxt_ptr));
	if (FI_SUCCESS != ret)
		gasneti_fatalerror("fi_write for normal message failed: %d\n", ret);
	gasnetc_paratomic_increment(&pending_rdma,0);
}

void
gasnetc_rdma_get(void *dest_addr, gasnet_node_t dest, void * src_addr, size_t nbytes,
		gasnetc_ofi_op_ctxt_t *ctxt_ptr)
{
	int ret = FI_SUCCESS;

	((gasnetc_ofi_op_ctxt_t *)ctxt_ptr)->callback = gasnetc_ofi_handle_rdma;

    PERIODIC_RMA_POLL();

    OFI_INJECT_RETRY(&gasnetc_ofi_locks.rdma_tx,
                     OFI_READ(gasnetc_ofi_rdma_epfd, dest_addr, nbytes, dest, src_addr, ctxt_ptr));

	if (FI_SUCCESS != ret)
		gasneti_fatalerror("fi_read failed: %d\n", ret);
	gasnetc_paratomic_increment(&pending_rdma,0);
}

void
gasnetc_rdma_put_wait(gasnet_handle_t oph)
{
	gasnete_op_t *op = (gasnete_op_t*) oph;

	if (OPTYPE(op) == OPTYPE_EXPLICIT) {
		gasnete_eop_t *eop = (gasnete_eop_t *)op;
		while (!GASNETE_EOP_DONE(eop)) {
			GASNETC_OFI_POLL_EVERYTHING();
		}
	} else {
		gasnete_iop_t *iop = (gasnete_iop_t *)op;
		while (!GASNETE_IOP_CNTDONE(iop,put)) {
			GASNETC_OFI_POLL_EVERYTHING();
		}
	}
}

void
gasnetc_rdma_get_wait(gasnet_handle_t oph)
{
	gasnete_op_t *op = (gasnete_op_t*) oph;

	if (OPTYPE(op) == OPTYPE_EXPLICIT) {
		gasnete_eop_t *eop = (gasnete_eop_t *)op;
		while (!GASNETE_EOP_DONE(eop)) {
			GASNETC_OFI_POLL_EVERYTHING();
		}
	} else {
		gasnete_iop_t *iop = (gasnete_iop_t *)op;
		while (!GASNETE_IOP_CNTDONE(iop,get)) {
			GASNETC_OFI_POLL_EVERYTHING();
		}
	}
}
