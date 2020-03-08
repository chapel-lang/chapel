/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ofi-conduit/gasnet_ofi.c $
 * Description: GASNet libfabric (OFI) conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2015-2017, Intel Corporation
 * Portions copyright 2018-2020, The Regents of the University of California.
 * Terms of use are as specified in license.txt
 */
#include <gasnet_core_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_ofi.h>

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

GASNETI_IDENT(gasnetc_IdentString_Providers,
              "$GASNetSupportedOFIProviders: " GASNETC_OFI_PROVIDER_LIST " $");

typedef struct gasnetc_ofi_recv_metadata {
    struct iovec iov;
    struct fi_msg am_buff_msg;
    struct gasnetc_ofi_ctxt am_buff_ctxt;
} gasnetc_ofi_recv_metadata_t;

#define NUM_OFI_ENDPOINTS 3

#define USE_AV_MAP 0
static addr_table_t  *addr_table;
#if USE_AV_MAP
#define GET_AM_REQUEST_DEST(dest) (fi_addr_t)(addr_table->table[(dest)*NUM_OFI_ENDPOINTS])
#define GET_AM_REPLY_DEST(dest) (fi_addr_t)(addr_table->table[(dest)*NUM_OFI_ENDPOINTS+1])
#define GET_RDMA_DEST(dest) (fi_addr_t)(addr_table->table[(dest)*NUM_OFI_ENDPOINTS+2])
#else
#define GET_AM_REQUEST_DEST(dest) (fi_addr_t)((dest)*NUM_OFI_ENDPOINTS)
#define GET_AM_REPLY_DEST(dest) (fi_addr_t)((dest)*NUM_OFI_ENDPOINTS+1)
#define GET_RDMA_DEST(dest) (fi_addr_t)((dest)*NUM_OFI_ENDPOINTS+2)
#endif

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
#define GET_REMOTEADDR(remote_addr, dest) (uintptr_t)((char*)remote_addr - (char*)gasneti_seginfo[dest].addr)
#else
#define GET_REMOTEADDR(remote_addr, dest) (uintptr_t)remote_addr
#endif


#define SCALABLE_NOT_AUTO_DETECTED (-1)

static short has_mr_scalable = SCALABLE_NOT_AUTO_DETECTED;
/* This pointer will only be malloced if GASNETC_OFI_HAS_MR_SCALABLE is
 * true at runtime */
static uint64_t* gasnetc_ofi_target_keys;
#ifndef GASNETC_OFI_HAS_MR_SCALABLE
#define GASNETC_OFI_HAS_MR_SCALABLE has_mr_scalable
#endif

/* FI_MR_BASIC requires addressing by full virtual address */
#define GET_REMOTEADDR_PER_MR_MODE(dest_addr, dest)\
    GASNETC_OFI_HAS_MR_SCALABLE ? GET_REMOTEADDR(dest_addr, dest) : (uintptr_t)dest_addr

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
#define OFI_INJECT_RETRY(lock, fxn, poll_type)\
    do {\
        GASNETC_OFI_LOCK_EXPR(lock, fxn);\
        while (ret == -FI_EAGAIN) {\
            GASNETI_WAITHOOK();\
            GASNETC_OFI_POLL_SELECTIVE(poll_type);\
            GASNETC_OFI_LOCK_EXPR(lock, fxn);\
        }\
    }while(0)

static gasneti_lifo_head_t ofi_am_request_pool = GASNETI_LIFO_INITIALIZER;
static gasneti_lifo_head_t ofi_am_reply_pool = GASNETI_LIFO_INITIALIZER;
static gasneti_lifo_head_t ofi_bbuf_pool = GASNETI_LIFO_INITIALIZER;
static gasneti_lifo_head_t ofi_bbuf_ctxt_pool = GASNETI_LIFO_INITIALIZER;

static size_t num_multirecv_buffs;
static size_t multirecv_buff_size;
static void* receive_region_start = NULL;

/* Variables for bounce buffering of non-blocking, non-bulk puts.
 * The gasnetc_ofi_bbuf_threshold variable is defined in gasnet_ofi.h
 * as it is needed in other files */
static void* bounce_region_start = NULL;
static size_t bounce_region_size = 0;
static size_t ofi_num_bbufs; 
static size_t ofi_bbuf_size;
#define OFI_MAX_NUM_BOUNCE_BUFFERS 32

static void* am_buffers_region_start = NULL;
static size_t am_buffers_region_size = 0;
static size_t max_am_send_buffs = 0;
static size_t num_init_am_send_buffs = 0;
static int out_of_send_buffers = 0;
static gasnetc_paratomic_t num_allocated_send_buffers = gasnetc_paratomic_init(0);
static size_t long_rma_threshold = 0;

static uint64_t             	max_buffered_send;
static uint64_t             	min_multi_recv;

static int using_psm_provider = 0;

gasneti_spawnerfn_t const *gasneti_spawner = NULL;

static gasnetc_ofi_recv_metadata_t* metadata_array;

/* Being able to see if there are pending operations that are not
 * completing is useful for debugging purposes */
#if GASNET_DEBUG
static gasnetc_paratomic_t pending_rdma = gasnetc_paratomic_init(0);
static gasnetc_paratomic_t pending_am = gasnetc_paratomic_init(0);
#endif

static int gasnetc_ofi_inited = 0;

#define OFI_CONDUIT_VERSION FI_VERSION(1, 0)

#if GASNET_PSHM
    #define gasnetc_AMPSHMPoll(repliesOnly) gasneti_AMPSHMPoll(repliesOnly GASNETI_THREAD_PASS)
#else
    #define gasnetc_AMPSHMPoll(repliesOnly) ((void)0)
#endif 

#define GASNETC_OFI_POLL_EVERYTHING() do { gasnetc_ofi_poll(); gasnetc_AMPSHMPoll(0);} while (0)
#define GASNETC_OFI_POLL_SELECTIVE(type) do {\
    if (type == OFI_POLL_ALL) {\
        gasnetc_ofi_am_recv_poll_cold(1);\
        gasnetc_AMPSHMPoll(0);\
    }\
    else {\
        gasnetc_AMPSHMPoll(1);\
    }\
    gasnetc_ofi_am_recv_poll_cold(0);\
    gasnetc_ofi_tx_poll();\
}while(0)


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
void gasnetc_ofi_handle_am(gasnetc_ofi_am_send_buf_t *header, int isreq, size_t msg_len, size_t nbytes);
void gasnetc_ofi_release_request_am(struct fi_cq_data_entry *re, void *buf);
void gasnetc_ofi_release_reply_am(struct fi_cq_data_entry *re, void *buf);
void gasnetc_ofi_tx_poll();
GASNETI_INLINE(gasnetc_ofi_am_recv_poll)
void gasnetc_ofi_am_recv_poll(int is_request);
void gasnetc_ofi_am_recv_poll_cold(int is_request) { // non-inline wrapper to avoid forced inlining on "cold" paths
  gasnetc_ofi_am_recv_poll(is_request);
}
GASNETI_NEVER_INLINE(gasnetc_fi_cq_readerr, // this wrapper silences a warning on gcc 4.8.5
ssize_t gasnetc_fi_cq_readerr(struct fid_cq *cq, struct fi_cq_err_entry *buf, uint64_t flags)) {
  return fi_cq_readerr(cq, buf, flags);
}

/* Reads any user-provided settings from the environment to avoid clogging up
 * the gasnetc_ofi_init() function with this code. */
static void gasnetc_ofi_read_env_vars() {
    const char* max_am_send_buffs_env =  "GASNET_OFI_MAX_SEND_BUFFS";
    const char* num_init_send_buffs_env = "GASNET_OFI_NUM_INITIAL_SEND_BUFFS";
    const char* max_err_string =  "%s must be greater than or equal to\n"
                                  "%s, which is set to %d in this run.\n";
    const char* init_err_string = "%s must be greater than or equal to 2.\n";
    max_am_send_buffs = gasneti_getenv_int_withdefault(max_am_send_buffs_env, 1000, 0);
    num_init_am_send_buffs = gasneti_getenv_int_withdefault(num_init_send_buffs_env, 500, 0);

    if (num_init_am_send_buffs < 2) {
        gasneti_fatalerror(init_err_string, num_init_send_buffs_env);
    }

    if (max_am_send_buffs < num_init_am_send_buffs) {
        gasneti_fatalerror(max_err_string, max_am_send_buffs_env, num_init_send_buffs_env, 
                (int)num_init_am_send_buffs);
    }

    /* The number of RMA requests to be issued before a tx_poll takes place */
    rdma_periodic_poll_threshold = gasneti_getenv_int_withdefault("GASNET_OFI_RMA_POLL_FREQ", 32, 0);

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

    const char* num_multirecv_buffs_env = "GASNET_OFI_NUM_RECEIVE_BUFFS";
    const char* multirecv_size_env = "GASNET_OFI_RECEIVE_BUFF_SIZE";
    num_multirecv_buffs = gasneti_getenv_int_withdefault(num_multirecv_buffs_env, 8, 0);

    multirecv_buff_size = gasneti_getenv_int_withdefault(multirecv_size_env, 1024*1024, 1);
    if (num_multirecv_buffs < 2)
        gasneti_fatalerror("%s must be at least 2.\n", num_multirecv_buffs_env);

    if (multirecv_buff_size < sizeof(gasnetc_ofi_am_buf_t)) {
        gasneti_fatalerror("%s must be at least %d bytes on this build.\n"
                "This is the size of the largest AM Medium plus the message header.\n", \
                multirecv_size_env, (int)sizeof(gasnetc_ofi_am_buf_t));
    }
    const char* long_rma_threshold_env = "GASNET_OFI_LONG_AM_RMA_THRESH";
    long_rma_threshold = gasneti_getenv_int_withdefault(long_rma_threshold_env, OFI_AM_MAX_DATA_LENGTH, 1);
    if (long_rma_threshold > OFI_AM_MAX_DATA_LENGTH) {
            gasneti_fatalerror(
                "The value given for %s exceeds the amount\n"
                "of data which can be packed into a medium message (%d bytes on this build).\n"
                "Use a lower value or reconfigure GASNet for a bigger medium message size using\n"
                "--with-ofi-max-medium=<new size>.\n",
                long_rma_threshold_env, (int)OFI_AM_MAX_DATA_LENGTH);
    }
}

/* The intention of separating this logic from gasnetc_ofi_init() is
 * to contain the complexity of supporting scalable endpoints in the future to
 * this function and the relevant get-address macros.
 */
static void ofi_setup_address_vector() {
  int ret = FI_SUCCESS;
  conn_entry_t *mapped_table;
  struct fi_av_attr   	av_attr 	= {0};

  /* Open Address Vector and bind the AV to the domain */
#if USE_AV_MAP
  av_attr.type        = FI_AV_MAP;
  addr_table          = (addr_table_t*)gasneti_malloc(gasneti_nodes * NUM_OFI_ENDPOINTS 
          * sizeof(conn_entry_t) + sizeof(addr_table_t));
  addr_table->size    = gasneti_nodes * NUM_OFI_ENDPOINTS;
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

  ret = fi_ep_bind(gasnetc_ofi_request_epfd, &gasnetc_ofi_avfd->fid, 0);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_ep_bind for avfd to am request epfd failed: %d\n", ret);

  ret = fi_ep_bind(gasnetc_ofi_reply_epfd, &gasnetc_ofi_avfd->fid, 0);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_ep_bind for avfd to am reply epfd failed: %d\n", ret);
}

static void ofi_exchange_addresses() {
  size_t reqnamelen = 0, repnamelen = 0, rdmanamelen = 0;
  char* on_node_addresses;
  int ret = FI_SUCCESS;

  /* Query each endpoint for its address length. While in most cases, these
   * lengths will be equal, there are some cases where they might not be. For
   * example, when using both IPv4 and IPv6. */
  ret = fi_getname(&gasnetc_ofi_request_epfd->fid, NULL, &reqnamelen);
  gasneti_assert(ret == -FI_ETOOSMALL);
  ret = fi_getname(&gasnetc_ofi_reply_epfd->fid, NULL, &repnamelen);
  gasneti_assert(ret == -FI_ETOOSMALL);
  ret = fi_getname(&gasnetc_ofi_rdma_epfd->fid, NULL, &rdmanamelen);
  gasneti_assert(ret == -FI_ETOOSMALL);

  size_t total_len = reqnamelen + repnamelen + rdmanamelen;
  on_node_addresses = gasneti_malloc(total_len);

  char* alladdrs = gasneti_malloc(gasneti_nodes*total_len);

  const char* getname_error_msg = "fi_getname failed for the %s endpoint with error code %d.\n";
  ret = fi_getname(&gasnetc_ofi_request_epfd->fid, on_node_addresses, &reqnamelen);
  if (FI_SUCCESS != ret) gasneti_fatalerror(getname_error_msg, "AM request", ret);
  ret = fi_getname(&gasnetc_ofi_reply_epfd->fid, on_node_addresses+reqnamelen, &repnamelen);
  if (FI_SUCCESS != ret) gasneti_fatalerror(getname_error_msg, "AM reply", ret);
  ret = fi_getname(&gasnetc_ofi_rdma_epfd->fid, on_node_addresses+reqnamelen+repnamelen, &rdmanamelen);
  if (FI_SUCCESS != ret) gasneti_fatalerror(getname_error_msg, "RDMA", ret);

  gasneti_bootstrapExchange(on_node_addresses, total_len, alladdrs);
  /* NOTE: If AV_MAP is ever to be supported, the NULL in the below call needs to be
   * changed to point to the AV structure. */
  ret = fi_av_insert(gasnetc_ofi_avfd, alladdrs, gasneti_nodes*NUM_OFI_ENDPOINTS, 
          NULL ,0ULL, NULL);
  if (gasneti_nodes*NUM_OFI_ENDPOINTS != ret) 
      gasneti_fatalerror("fi_av_insert failed. Expected: %d Actual: %d\n", gasneti_nodes*NUM_OFI_ENDPOINTS, ret);

  gasneti_free(alladdrs);
  gasneti_free(on_node_addresses);
}

/*------------------------------------------------
 * Initialize OFI conduit
 * ----------------------------------------------*/
int gasnetc_ofi_init(void)
{
  int ret = GASNET_OK;
  int result = GASNET_ERR_NOT_INIT;
  struct fi_info		*hints, *info;
  struct fi_cq_attr   	cq_attr 	= {0};
  size_t optlen;
  int num_locks; 
  int i;
  
  int high_perf_prov = 0;

  gasnetc_ofi_read_env_vars();
  /* Ensure uniform FI_* env vars */
  /* TODO: what about provider-specific env vars? */
  gasneti_propagate_env("FI_", GASNETI_PROPAGATE_ENV_PREFIX);

#if GASNETC_OFI_USE_THREAD_DOMAIN && GASNET_PAR
  gasneti_spinlock_init(&gasnetc_ofi_locks.big_lock);
#elif GASNET_PAR
  /* This lock is needed in PAR mode to protect the AM reference counting */
  gasneti_spinlock_init(&gasnetc_ofi_locks.rx_request_cq);
  gasneti_spinlock_init(&gasnetc_ofi_locks.rx_reply_cq);
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
  hints->caps			= FI_RMA | FI_MSG | FI_MULTI_RECV;
  /* mode: convey requirements for application to use fabric interfaces */
  hints->mode			= FI_CONTEXT;	/* fi_context is used for per
						   operation context parameter */
  /* addr_format: expected address format for AV/CM calls */
  hints->addr_format		= FI_FORMAT_UNSPEC;
  hints->tx_attr->op_flags	= FI_DELIVERY_COMPLETE;
  hints->ep_attr->type		= FI_EP_RDM; /* Reliable datagram */
  /* Threading mode is set by the configure script to FI_THREAD_DOMAIN if
   * using the psm2 provider and FI_THREAD_SAFE otherwise*/
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

  /* If the configure script detected a provider's mr_mode, then force
   * ofi to use that mode. */
  switch(GASNETC_OFI_HAS_MR_SCALABLE) {
      case 0:
          hints->domain_attr->mr_mode = FI_MR_BASIC;
          break;
      case SCALABLE_NOT_AUTO_DETECTED:
          hints->domain_attr->mr_mode = FI_MR_UNSPEC;
          break;
      default:
          hints->domain_attr->mr_mode = FI_MR_SCALABLE;
  }

  /* In libfabric v1.6, the psm2 provider transitioned to using separate
   * psm2 endpoints for each ofi endpoint, whereas in the past all communication
   * was multiplexed over a single psm2 endpoint. Setting this variable ensures
   * that unnecessary connections between remote endpoints which never communicate
   * are not made, which can cause slow tear-down. This variable is set before
   * calling fi_getinfo() as the provider may read its environment variables in
   * that function call. */
  int set_psm2_lazy_conn = 0;
  if ((FI_MAJOR_VERSION == 1 && FI_MINOR_VERSION >= 6) || FI_MAJOR_VERSION > 1) {
      if (!getenv("FI_PSM2_LAZY_CONN")) {
          set_psm2_lazy_conn = 1;
          setenv("FI_PSM2_LAZY_CONN", "1", 1);
      }
  }

  ret = fi_getinfo(OFI_CONDUIT_VERSION, NULL, NULL, 0ULL, hints, &info);
  if (FI_SUCCESS != ret) {
	  GASNETI_RETURN_ERRR(RESOURCE,
			  "No OFI providers found that could support the OFI conduit");
  }

  // Find the first entry for the most-preferred provider offered, if any.
  const char *supported_providers = GASNETC_OFI_PROVIDER_LIST;
  const char *q = supported_providers;
  while (*q) {
      while (*q == ' ') ++q;
      const char *r = strchr(q, ' ');
      int len = r ? r - q : strlen(q);
      char prov_name[64];
      strncpy(prov_name, q, len);
      prov_name[len] = '\0';
      for (struct fi_info *p = info; p; p = p->next) {
          if (!strcmp(p->fabric_attr->prov_name, prov_name)) {
              info = p;
              goto done;
          }
      }
      q += len;
  }
done:
  // Balk if provider was explicitly chosen at configure time and is not available now
  if (!strchr(supported_providers,' ') && strcmp(supported_providers, info->fabric_attr->prov_name)) {
      char *envvar = gasneti_getenv("FI_PROVIDER");
      gasneti_fatalerror(
          "OFI provider '%s' selected at configure time is not available at run time%s%s%s.",
          supported_providers,
          envvar ? " and/or has been overridden by FI_PROVIDER='" : "",
          envvar ? envvar : "",
          envvar ? "' in the environment" : "");
  }

  if (!strcmp(info->fabric_attr->prov_name, "psm2")){
      high_perf_prov = 1;
      using_psm_provider = 1;
  } else if (set_psm2_lazy_conn) {
      /* If we set this variable and are not using psm2, unset it in the
       * unlikely case that another library in the current application will
       * use ofi/psm2 */
      unsetenv("FI_PSM2_LAZY_CONN");
  }

  int quiet = gasneti_getenv_yesno_withdefault("GASNET_QUIET", 0);
#if GASNET_PAR
  if (!gasneti_mynode) {
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

  if (!high_perf_prov && !gasneti_mynode) {
          const char * msg = 
          "WARNING: Using OFI provider (%s), which has not been validated to provide\n"
          "WARNING: acceptable GASNet performance. You should consider using a more\n"
          "WARNING: hardware-appropriate GASNet conduit. See ofi-conduit/README.\n";
	  if (!quiet)
		  fprintf(stderr, msg, info->fabric_attr->prov_name);
  }

  if(info->domain_attr->mr_mode == FI_MR_SCALABLE) {
      has_mr_scalable = 1;
  } else {
      has_mr_scalable = 0;
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

  /* The intention here is to ensure that subsequent calls to fi_getinfo()
   * won't ever give us a different provider. This is likely unnecessary,
   * but it is good to be paranoid. */
  hints->domain_attr->name = gasneti_strdup(info->domain_attr->name);

  /* Allocate a new active endpoint for RDMA operations */
  hints->caps = FI_RMA;

  ret = fi_getinfo(OFI_CONDUIT_VERSION, NULL, NULL, 0ULL, hints, &info);
  if(FI_SUCCESS != ret)
      gasneti_fatalerror("fi_getinfo() failed querying for RMA endpoint: %d\n", ret);

  ret = fi_endpoint(gasnetc_ofi_domainfd, info, &gasnetc_ofi_rdma_epfd, NULL);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_endpoint for rdma failed: %d\n", ret);

  /* Allocate a new active endpoint for AM operations buffer */
  hints->caps     = FI_MSG | FI_MULTI_RECV;

  ret = fi_getinfo(OFI_CONDUIT_VERSION, NULL, NULL, 0ULL, hints, &info);
  if(FI_SUCCESS != ret)
      gasneti_fatalerror("fi_getinfo() failed querying for MSG endpoints: %d\n", ret);

  gasneti_free(hints->domain_attr->name);
  hints->domain_attr->name = NULL;

  ret = fi_endpoint(gasnetc_ofi_domainfd, info, &gasnetc_ofi_request_epfd, NULL);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_endpoint for am request endpoint failed: %d\n", ret);

  ret = fi_endpoint(gasnetc_ofi_domainfd, info, &gasnetc_ofi_reply_epfd, NULL);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_endpoint for am reply endpoint failed: %d\n", ret);

  /* Allocate a CQ that will be shared for both RDMA and AM tx ops */
  memset(&cq_attr, 0, sizeof(cq_attr));
  cq_attr.format    = FI_CQ_FORMAT_DATA; /* Provides data associated with a completion */
  ret = fi_cq_open(gasnetc_ofi_domainfd, &cq_attr, &gasnetc_ofi_tx_cqfd, NULL);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_cq_open for rdma_eqfd failed: %d\n", ret);

  /* Allocate recv completion queues for AMs */
  memset(&cq_attr, 0, sizeof(cq_attr));
  cq_attr.format    = FI_CQ_FORMAT_DATA;
  ret = fi_cq_open(gasnetc_ofi_domainfd, &cq_attr, &gasnetc_ofi_request_cqfd, NULL);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_cq_open for am request cq failed: %d\n", ret);

  ret = fi_cq_open(gasnetc_ofi_domainfd, &cq_attr, &gasnetc_ofi_reply_cqfd, NULL);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_cq_open for am reply cq failed: %d\n", ret);

  /* Bind CQs to endpoints */
  ret = fi_ep_bind(gasnetc_ofi_rdma_epfd, &gasnetc_ofi_tx_cqfd->fid, FI_TRANSMIT | FI_RECV);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_ep_bind for tx_cq to rdma_epfd failed: %d\n", ret);

  ret = fi_ep_bind(gasnetc_ofi_request_epfd, &gasnetc_ofi_tx_cqfd->fid, FI_TRANSMIT);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_ep_bind for tx_cq to am request CQ failed: %d\n", ret);

  ret = fi_ep_bind(gasnetc_ofi_reply_epfd, &gasnetc_ofi_tx_cqfd->fid, FI_TRANSMIT);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_ep_bind for tx_cq to am reply CQ failed: %d\n", ret);

  ret = fi_ep_bind(gasnetc_ofi_request_epfd, &gasnetc_ofi_request_cqfd->fid, FI_RECV);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_ep_bind for am request cq to am_request_epfd failed: %d\n", ret);

  ret = fi_ep_bind(gasnetc_ofi_reply_epfd, &gasnetc_ofi_reply_cqfd->fid, FI_RECV);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_ep_bind for am reply cq to am_reply_epfd failed: %d\n", ret);

  /* Low-water mark for shared receive buffer */
  min_multi_recv = OFI_AM_MAX_DATA_LENGTH + offsetof(gasnetc_ofi_am_send_buf_t,buf.long_buf)
                    + offsetof(gasnetc_ofi_am_long_buf_t, data);
  optlen = min_multi_recv;
  ret	 = fi_setopt(&gasnetc_ofi_request_epfd->fid, FI_OPT_ENDPOINT, FI_OPT_MIN_MULTI_RECV,
		  &optlen,
		  sizeof(optlen));
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_setopt for am request epfd failed: %d\n", ret);
  ret	 = fi_setopt(&gasnetc_ofi_reply_epfd->fid, FI_OPT_ENDPOINT, FI_OPT_MIN_MULTI_RECV,
		  &optlen,
		  sizeof(optlen));
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_setopt for am reply epfd failed: %d\n", ret);

  /* Cutoff to use fi_inject */
  max_buffered_send = info->tx_attr->inject_size;

  ofi_setup_address_vector();

  /* Enable endpoints */
  ret = fi_enable(gasnetc_ofi_rdma_epfd);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_enable for rdma failed: %d\n", ret);
  ret = fi_enable(gasnetc_ofi_request_epfd);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_enable for am request ep failed: %d\n", ret);
  ret = fi_enable(gasnetc_ofi_reply_epfd);
  if (FI_SUCCESS != ret) gasneti_fatalerror("fi_enable for am reply ep failed: %d\n", ret);

  ofi_exchange_addresses();

  fi_freeinfo(hints);

  if (!GASNETC_OFI_HAS_MR_SCALABLE) {
      gasnetc_ofi_target_keys = gasneti_malloc(sizeof(uint64_t)*gasneti_nodes);
      gasneti_assert(gasnetc_ofi_target_keys);
  }

  receive_region_start = gasneti_malloc_aligned(GASNETI_PAGESIZE, multirecv_buff_size*num_multirecv_buffs);
  metadata_array = gasneti_malloc(sizeof(gasnetc_ofi_recv_metadata_t)*num_multirecv_buffs);

  for(i = 0; i < num_multirecv_buffs; i++) {
        gasnetc_ofi_recv_metadata_t* metadata = metadata_array + i;
        metadata->iov.iov_base = ((char*)receive_region_start) + multirecv_buff_size*i;
        metadata->iov.iov_len = multirecv_buff_size;
        metadata->am_buff_msg.msg_iov = &metadata->iov;
        metadata->am_buff_msg.iov_count = 1;
        metadata->am_buff_msg.addr = FI_ADDR_UNSPEC;
        metadata->am_buff_msg.desc = NULL;
        metadata->am_buff_msg.context = &metadata->am_buff_ctxt.ctxt;
        metadata->am_buff_msg.data = 0;
        metadata->am_buff_ctxt.index = i;
        metadata->am_buff_ctxt.final_cntr = 0;
        metadata->am_buff_ctxt.event_cntr = 0;
        gasnetc_paratomic_set(&metadata->am_buff_ctxt.consumed_cntr, 0, 0);
        metadata->am_buff_ctxt.metadata = metadata;
		/* Post buffers for Active Messages */
        if (i % 2 == 0)
            ret = fi_recvmsg(gasnetc_ofi_request_epfd, &metadata->am_buff_msg, FI_MULTI_RECV);
        else
            ret = fi_recvmsg(gasnetc_ofi_reply_epfd, &metadata->am_buff_msg, FI_MULTI_RECV);

		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_recvmsg failed: %d\n", ret);
	}

  
  /* Allocate bounce buffers*/
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

  /* We need to keep count of how many buffers we allocate so we can place a
   * limit on them */
  gasnetc_paratomic_set(&num_allocated_send_buffers, num_init_am_send_buffs, 0);
  if (max_am_send_buffs == num_init_am_send_buffs) 
      out_of_send_buffers = 1;

  am_buffers_region_size = GASNETI_PAGE_ALIGNUP(num_init_am_send_buffs*sizeof(gasnetc_ofi_am_buf_t));
  am_buffers_region_start = gasneti_malloc_aligned(GASNETI_PAGESIZE, am_buffers_region_size);
  gasneti_leak_aligned(am_buffers_region_start);

  /* Add the buffers to the stack in reverse order to be friendly to the cache. */
  gasnetc_ofi_am_buf_t * bufp = (gasnetc_ofi_am_buf_t*)am_buffers_region_start + (num_init_am_send_buffs - 1);

  for (i = 0; i < (int)num_init_am_send_buffs/2; i++) {
     bufp->callback = gasnetc_ofi_release_request_am;
     gasneti_lifo_push(&ofi_am_request_pool, bufp);
     bufp--;
  }  
  for (; i < (int)num_init_am_send_buffs; i++) {
      bufp->callback = gasnetc_ofi_release_reply_am;
      gasneti_lifo_push(&ofi_am_reply_pool, bufp);
      bufp--;
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

    for(i = 0; i < num_multirecv_buffs; i++) {
        gasnetc_ofi_recv_metadata_t* metadata = metadata_array + i;
        gasnetc_ofi_ctxt_t am_buff_ctxt = metadata->am_buff_ctxt;
      /* cancel the multi-recv */
        if (i % 2 == 0)
            ret = fi_cancel(&gasnetc_ofi_request_epfd->fid, &am_buff_ctxt.ctxt);
        else
            ret = fi_cancel(&gasnetc_ofi_reply_epfd->fid, &am_buff_ctxt.ctxt);
    }

  #if GASNETI_CLIENT_THREADS
    /* Unsafe to free resources if other threads may be using them */
  #else
    gasneti_free(metadata_array);
    gasneti_free_aligned(receive_region_start);
  #endif

  if(fi_close(&gasnetc_ofi_reply_epfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am reply epfd failed\n");
  }

  if(fi_close(&gasnetc_ofi_request_epfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am request epfd failed\n");
  }

  if(fi_close(&gasnetc_ofi_rdma_epfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close rdma epfd failed\n");
  }

  if(fi_close(&gasnetc_ofi_rdma_mrfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close mrfd failed\n");
  }

  if(fi_close(&gasnetc_ofi_tx_cqfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am scqfd failed\n");
  }

  if(fi_close(&gasnetc_ofi_reply_cqfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am reply cqfd failed\n");
  }
  if(fi_close(&gasnetc_ofi_request_cqfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am request cqfd failed\n");
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
GASNETI_INLINE(gasnetc_ofi_handle_am)
void gasnetc_ofi_handle_am(gasnetc_ofi_am_send_buf_t *header, int isreq, size_t msg_len, size_t nbytes)
{
	uint8_t *addr;
	int handler = header->handler;
    const gex_AM_Entry_t * const handler_entry = &gasnetc_handler[handler];
    gex_AM_Fn_t handler_fn = handler_entry->gex_fnptr;
	gex_AM_Arg_t *args;
	int numargs = header->argnum;
    int data_offset;
    gex_Token_t token = (gex_Token_t)header;
	switch(header->type) {
		case OFI_AM_SHORT:
            args = (gex_AM_Arg_t *)header->buf.short_buf.data;
			GASNETI_RUN_HANDLER_SHORT(isreq, handler, handler_fn, token, args, numargs);
			break;
		case OFI_AM_MEDIUM:
            data_offset = GASNETI_ALIGNUP(sizeof(gex_AM_Arg_t)*numargs, GASNETI_MEDBUF_ALIGNMENT);
            args = (gex_AM_Arg_t *)header->buf.medium_buf.data;
			addr = header->buf.medium_buf.data + data_offset;
			GASNETI_RUN_HANDLER_MEDIUM(isreq, handler, handler_fn, token, args, numargs, addr, nbytes);
			break;
		case OFI_AM_LONG:
            data_offset = sizeof(gex_AM_Arg_t)*numargs;
            args = (gex_AM_Arg_t *)header->buf.long_buf.data;
			addr = header->buf.long_buf.dest_ptr;
			GASNETI_RUN_HANDLER_LONG(isreq, handler, handler_fn, token, args, numargs, addr, nbytes);
			break;
		case OFI_AM_LONG_MEDIUM:
            data_offset = sizeof(gex_AM_Arg_t)*numargs;
            args = (gex_AM_Arg_t *)header->buf.long_buf.data;
			addr = header->buf.long_buf.dest_ptr;
			memcpy(addr, header->buf.long_buf.data + data_offset, nbytes);
			GASNETI_RUN_HANDLER_LONG(isreq, handler, handler_fn, token, args, numargs, addr, nbytes);
			break;
		default:
			gasneti_fatalerror("undefined header type in gasnetc_ofi_handle_am: %d\n",
					header->type);
	}
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
				gasnete_eop_t *eop = gasneti_container_of(ptr, gasnete_eop_t, ofi);
				GASNETE_EOP_MARKDONE(eop);
			}
			break;
		case OFI_TYPE_IGET:
			{
				gasnete_iop_t *iop = gasneti_container_of(ptr, gasnete_iop_t, get_ofi);
				gasneti_weakatomic_increment(&(iop->completed_get_cnt), 0);
			}
			break;
		case OFI_TYPE_IPUT:
			{
				gasnete_iop_t *iop = gasneti_container_of(ptr, gasnete_iop_t, put_ofi);
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
void gasnetc_ofi_release_request_am(struct fi_cq_data_entry *re, void *buf)
{
	gasnetc_ofi_am_buf_t *header = (gasnetc_ofi_am_buf_t*)buf;
	gasneti_lifo_push(&ofi_am_request_pool, header);
}
void gasnetc_ofi_release_reply_am(struct fi_cq_data_entry *re, void *buf)
{
	gasnetc_ofi_am_buf_t *header = (gasnetc_ofi_am_buf_t*)buf;
	gasneti_lifo_push(&ofi_am_reply_pool, header);
}

/* Get a send buffer */
GASNETI_INLINE(gasnetc_ofi_am_header)
gasnetc_ofi_am_buf_t *gasnetc_ofi_am_header(int isreq GASNETI_THREAD_FARG)
{
    gasneti_lifo_head_t* pool;
    int poll_type;
    if (isreq) {
        pool = &ofi_am_request_pool;
    } 
    else {
        pool = &ofi_am_reply_pool;
    }

	gasnetc_ofi_am_buf_t *header = gasneti_lifo_pop(pool);
    if_pt (header) 
        return header;
    else if (!out_of_send_buffers) {
        // Poll the tx queue and retry the pool before allocating another buffer
        gasnetc_ofi_tx_poll();
        header = gasneti_lifo_pop(pool);
        if (header) return header;

        int tmp = gasnetc_paratomic_add(&num_allocated_send_buffers, 1, GASNETI_ATOMIC_ACQ);
        if (tmp > max_am_send_buffs ) {
            goto ofi_spin_for_buffer;
        }
        else if (tmp == max_am_send_buffs) {
            /* This update is not threadsafe. This is okay though, as it is only
             * to prevent continuously incrementing the atomic counter after all 
             * buffers have been allocated.*/
            out_of_send_buffers = 1; 
        }
        
        header = gasneti_malloc(sizeof(gasnetc_ofi_am_buf_t));
        gasneti_leak(header);
        header->callback = isreq ? gasnetc_ofi_release_request_am : gasnetc_ofi_release_reply_am;
        return header;
    }
ofi_spin_for_buffer:
    poll_type = isreq ? OFI_POLL_ALL : OFI_POLL_REPLY;
    do {
        GASNETC_OFI_POLL_SELECTIVE(poll_type);
        GASNETI_WAITHOOK();
        header = gasneti_lifo_pop(pool);
    } while(NULL == header);
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
                   gasnetc_fi_cq_readerr(gasnetc_ofi_tx_cqfd, &e ,0));
                if_pf (gasnetc_is_exit_error(e)) return;
                gasneti_fatalerror("fi_cq_read for tx_poll failed with error: %s\n", fi_strerror(e.err));
            } 
            else
                gasneti_fatalerror("fi_cq_read for tx_poll returned unexpected error code: %d\n", ret);
        } 
        else {
            for (i = 0; i < ret; i++) {
                if (re[i].flags & FI_SEND) {
#if GASNET_DEBUG
                    gasnetc_paratomic_decrement(&pending_am, 0);
#endif
                    gasnetc_ofi_am_buf_t *header = (gasnetc_ofi_am_buf_t *)re[i].op_context;
                    header->callback(&re[i], header);
                }
                else if(re[i].flags & FI_WRITE || re[i].flags & FI_READ) {
#if GASNET_DEBUG
                    gasnetc_paratomic_decrement(&pending_rdma, 0);
#endif
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

GASNETI_INLINE(gasnetc_ofi_am_recv_poll)
void gasnetc_ofi_am_recv_poll(int is_request)
{
	int ret = 0;
    int post_ret = 0;
	struct fi_cq_data_entry re = {0};
	struct fi_cq_err_entry e = {0};
    struct fid_ep * ep;
    struct fid_cq * cq;
    gasneti_atomic_t * lock_p;
    if (is_request) {
        ep = gasnetc_ofi_request_epfd;
        cq = gasnetc_ofi_request_cqfd;
#if GASNET_PAR && !GASNETC_OFI_USE_THREAD_DOMAIN
        lock_p = &gasnetc_ofi_locks.rx_request_cq;
#endif
    }
    else {
        ep = gasnetc_ofi_reply_epfd;
        cq = gasnetc_ofi_reply_cqfd;
#if GASNET_PAR && !GASNETC_OFI_USE_THREAD_DOMAIN
        lock_p = &gasnetc_ofi_locks.rx_reply_cq;
#endif
    }


    /* Read from Completion Queue */
    if(EBUSY == GASNETC_OFI_PAR_TRYLOCK(lock_p)) return;

    ret = fi_cq_read(cq, (void *)&re, 1);

    if (ret == -FI_EAGAIN) {
        GASNETC_OFI_PAR_UNLOCK(lock_p);
        return;
    } 
    if_pf (ret < 0) {
        gasnetc_fi_cq_readerr(cq, &e ,0);
        GASNETC_OFI_PAR_UNLOCK(lock_p);
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
    GASNETC_OFI_PAR_UNLOCK(lock_p);

    if_pt (re.flags & FI_RECV) {
        /* re.data contains the number of bytes transferred in a medium or long message */
        gasnetc_ofi_handle_am(re.buf, is_request, re.len, re.data);
    }

    /* The atomic here ensures that the buffer is not reposted while an AM handler is
     * still running. */
    uint64_t tmp = gasnetc_paratomic_add(&header->consumed_cntr, 1, GASNETI_ATOMIC_ACQ);
    if_pf (tmp == (GASNETI_ATOMIC_MAX & header->final_cntr)) {
        gasnetc_ofi_recv_metadata_t* metadata = header->metadata;
        struct fi_msg* am_buff_msg = &metadata->am_buff_msg;
        GASNETC_OFI_LOCK(&gasnetc_ofi_locks.am_rx);
        post_ret = fi_recvmsg(ep, am_buff_msg, FI_MULTI_RECV);
        GASNETC_OFI_UNLOCK(&gasnetc_ofi_locks.am_rx);
        if_pf (FI_SUCCESS != post_ret) gasneti_fatalerror("fi_recvmsg failed inside am_recv_poll: %d\n", ret);
    }
}

/* General progress function */
void gasnetc_ofi_poll()
{
    gasnetc_ofi_tx_poll();
    gasnetc_ofi_am_recv_poll(1); /* requests */
    gasnetc_ofi_am_recv_poll(0); /* replies */
}

/*------------------------------------------------
 * OFI conduit am send functions
 * ----------------------------------------------*/

int gasnetc_ofi_am_send_short(gex_Rank_t dest, gex_AM_Index_t handler,
                     int numargs, va_list argptr, int isreq GASNETI_THREAD_FARG)
{
	int ret = FI_SUCCESS;
	gex_AM_Arg_t *arglist;
	int i;
    unsigned int len;
	gasnetc_ofi_am_buf_t *header;
	gasnetc_ofi_am_send_buf_t *sendbuf;
    struct fid_ep* ep;
    fi_addr_t am_dest;
    int poll_type;
    if (isreq) {
        ep = gasnetc_ofi_request_epfd;
        am_dest = GET_AM_REQUEST_DEST(dest);
        poll_type = OFI_POLL_ALL;
    } 
    else {
        ep = gasnetc_ofi_reply_epfd;
        am_dest = GET_AM_REPLY_DEST(dest);
        poll_type = OFI_POLL_REPLY;
    }

	/* Get a send buffer */
	header = gasnetc_ofi_am_header(isreq GASNETI_THREAD_PASS);

	/* Fill in the arguments */
	sendbuf = &header->sendbuf;
	arglist = (gex_AM_Arg_t*) sendbuf->buf.short_buf.data;
	for (i = 0 ; i < numargs ; ++i) {
		arglist[i] = va_arg(argptr, gex_AM_Arg_t);
	}

	/* Copy arg and handle into the buffer */
	sendbuf->handler = (uint8_t) handler;
	sendbuf->sourceid = gasneti_mynode;
	sendbuf->type = OFI_AM_SHORT;
	sendbuf->argnum = numargs;

	len = sizeof(gex_AM_Arg_t) * numargs + offsetof(gasnetc_ofi_am_send_buf_t, buf.short_buf);
    /* Alignment is added in order to ensure that all messages will be placed into the
     * multi-recv buffer at an 8 byte aligned address. This is due to medium message
     * payloads needing to be placed at an 8-byte aligned address. */
    len = GASNETI_ALIGNUP(len, GASNETI_MEDBUF_ALIGNMENT);

	if(len <= max_buffered_send) {
            OFI_INJECT_RETRY(&gasnetc_ofi_locks.am_tx,
                ret = fi_inject(ep, sendbuf, len, am_dest), poll_type);
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_inject for short am failed: %d\n", ret);

		/* Data buffer is ready for reuse, handle it by callback function */
		header->callback(NULL, header);
	} else {
            OFI_INJECT_RETRY(&gasnetc_ofi_locks.am_tx,
                ret = fi_send(ep, sendbuf, len, NULL, am_dest, &header->ctxt), poll_type);
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_send for short am failed: %d\n", ret);
#if GASNET_DEBUG
		gasnetc_paratomic_increment(&pending_am,0);
#endif
	}
	return ret;
}

int gasnetc_ofi_am_send_medium(gex_Rank_t dest, gex_AM_Index_t handler, 
                     void *source_addr, size_t nbytes,   /* data payload */
                     int numargs, va_list argptr, int isreq GASNETI_THREAD_FARG)
{
	int ret = FI_SUCCESS;
	gex_AM_Arg_t *arglist;
	int i;
    unsigned int len;
	gasnetc_ofi_am_buf_t *header;
	gasnetc_ofi_am_send_buf_t *sendbuf;
    struct fid_ep* ep;
    fi_addr_t am_dest;
    int poll_type;
    if (isreq) {
        ep = gasnetc_ofi_request_epfd;
        am_dest = GET_AM_REQUEST_DEST(dest);
        poll_type = OFI_POLL_ALL;
	gasneti_assert (nbytes <= gex_AM_LUBRequestMedium());
    } 
    else {
        ep = gasnetc_ofi_reply_epfd;
        am_dest = GET_AM_REPLY_DEST(dest);
        poll_type = OFI_POLL_REPLY;
	gasneti_assert (nbytes <= gex_AM_LUBReplyMedium());
    }


	/* Get a send buffer */
	header = gasnetc_ofi_am_header(isreq GASNETI_THREAD_PASS);

	/* Fill in the arguments */
	sendbuf = &header->sendbuf;
	arglist = (gex_AM_Arg_t*) sendbuf->buf.medium_buf.data;
	for (i = 0 ; i < numargs ; ++i) {
		arglist[i] = va_arg(argptr, gex_AM_Arg_t);
	}
	len = GASNETI_ALIGNUP(sizeof(gex_AM_Arg_t)*numargs, GASNETI_MEDBUF_ALIGNMENT);

	memcpy((uint8_t *)(sendbuf->buf.medium_buf.data)+ len, source_addr, nbytes);

	len += (nbytes + offsetof(gasnetc_ofi_am_send_buf_t, buf.medium_buf));
    len = GASNETI_ALIGNUP(len, GASNETI_MEDBUF_ALIGNMENT);

	/* Copy arg and handle into the buffer */
	sendbuf->handler = (uint8_t) handler;
	sendbuf->sourceid = gasneti_mynode;
	sendbuf->type = OFI_AM_MEDIUM;
	sendbuf->argnum = numargs;

	if(len <= max_buffered_send) {
            OFI_INJECT_RETRY(&gasnetc_ofi_locks.am_tx,
                ret = fi_injectdata(ep, sendbuf, len, nbytes, am_dest), poll_type);
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_inject for medium ashort failed: %d\n", ret);
		header->callback(NULL, header);
	} else {
            OFI_INJECT_RETRY(&gasnetc_ofi_locks.am_tx,
                ret = fi_senddata(ep, sendbuf, len, NULL, nbytes, am_dest, &header->ctxt), poll_type);
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_send for medium am failed: %d\n", ret);
#if GASNET_DEBUG
		gasnetc_paratomic_increment(&pending_am,0);
#endif
	}

	return ret;
}

int gasnetc_ofi_am_send_long(gex_Rank_t dest, gex_AM_Index_t handler,
		               void *source_addr, size_t nbytes,   /* data payload */
		               void *dest_addr,
		               int numargs, va_list argptr, int isreq, int isasync
                               GASNETI_THREAD_FARG)
{
	int ret = FI_SUCCESS;
	gex_AM_Arg_t *arglist;
	int i;
    unsigned int len;
	gasnetc_ofi_am_buf_t *header;
	gasnetc_ofi_am_send_buf_t *sendbuf;
    struct fid_ep* ep;
    fi_addr_t am_dest;
    int poll_type;
    if (isreq) {
        ep = gasnetc_ofi_request_epfd;
        am_dest = GET_AM_REQUEST_DEST(dest);
        poll_type = OFI_POLL_ALL;
    } 
    else {
        ep = gasnetc_ofi_reply_epfd;
        am_dest = GET_AM_REPLY_DEST(dest);
        poll_type = OFI_POLL_REPLY;
    }

	if(isreq)
		gasneti_assert (nbytes <= gex_AM_LUBRequestLong());
	else
		gasneti_assert (nbytes <= gex_AM_LUBReplyLong());

	/* Get a send buffer */
	header = gasnetc_ofi_am_header(isreq GASNETI_THREAD_PASS);

	/* Fill in the arguments */
	sendbuf = &header->sendbuf;
	arglist = (gex_AM_Arg_t*) sendbuf->buf.long_buf.data;
	for (i = 0 ; i < numargs ; ++i) {
		arglist[i] = va_arg(argptr, gex_AM_Arg_t);
	}
	len = sizeof(gex_AM_Arg_t)*numargs;
#if !GASNET_PSHM
	if(dest == gasneti_mynode) {
		memcpy(dest_addr, source_addr, nbytes);
		sendbuf->type = OFI_AM_LONG;
	} else
#endif
	if(len + nbytes < long_rma_threshold)
	{
		/* Pack the payload if it's small enough */
		memcpy(sendbuf->buf.long_buf.data + len, source_addr, nbytes);
		len += nbytes;
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
				GASNETC_OFI_POLL_SELECTIVE(poll_type);
                GASNETC_OFI_LOCK_EXPR(&gasnetc_ofi_locks.rdma_tx, 
				    OFI_WRITE(gasnetc_ofi_rdma_epfd, source_addr, nbytes, dest, dest_addr, &lam_ctxt.ctxt));
			}
			if (FI_SUCCESS != ret) 
				gasneti_fatalerror("fi_write failed for AM long: %d\n", ret);
#if GASNET_DEBUG
			gasnetc_paratomic_increment(&pending_rdma,0);
#endif

			/* Because the order is not guaranteed between different ep, */
			/* we send the am part after confirming the large rdma operation */
			/* is successful. */
			while(!lam_ctxt.data_sent) {
				GASNETC_OFI_POLL_SELECTIVE(poll_type);
			}
		sendbuf->type = OFI_AM_LONG;
	}
    len += offsetof(gasnetc_ofi_am_long_buf_t, data) + offsetof(gasnetc_ofi_am_send_buf_t, buf.long_buf);
    len = GASNETI_ALIGNUP(len, GASNETI_MEDBUF_ALIGNMENT);

	/* Copy arg and handle into the buffer */
	sendbuf->handler = (uint8_t) handler;
	sendbuf->sourceid = gasneti_mynode;
	sendbuf->argnum = numargs;
	sendbuf->buf.long_buf.dest_ptr = dest_addr;

	if(len <= max_buffered_send) {
        OFI_INJECT_RETRY(&gasnetc_ofi_locks.am_tx,
            ret = fi_injectdata(ep, sendbuf, len, nbytes, am_dest), poll_type);
		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_inject for long ashort failed: %d\n", ret);
		header->callback(NULL, header);
	} else {
        OFI_INJECT_RETRY(&gasnetc_ofi_locks.am_tx,
            ret = fi_senddata(ep, sendbuf, len, NULL, nbytes, am_dest, &header->ctxt), poll_type);

		if (FI_SUCCESS != ret) gasneti_fatalerror("fi_send for long am failed: %d\n", ret);
#if GASNET_DEBUG
		gasnetc_paratomic_increment(&pending_am,0);
#endif
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
int gasnetc_rdma_put_non_bulk(gex_Rank_t dest, void* dest_addr, void* src_addr, 
        size_t nbytes, gasnetc_ofi_op_ctxt_t* ctxt_ptr GASNETI_THREAD_FARG)
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
             ret = fi_writemsg(gasnetc_ofi_rdma_epfd, &msg, FI_INJECT | FI_DELIVERY_COMPLETE ), OFI_POLL_ALL);
        if_pf (FI_SUCCESS != ret)
            gasneti_fatalerror("fi_writemsg with FI_INJECT failed: %d\n", ret);

#if GASNET_DEBUG
        gasnetc_paratomic_increment(&pending_rdma,0);
#endif
        GASNETC_STAT_EVENT(NB_PUT_INJECT);
        return 0;
    } 
    /* Bounce buffers are needed */
    else if (nbytes <= gasnetc_ofi_bbuf_threshold) {
        uintptr_t dest_ptr = (uintptr_t)dest_addr;
        int num_bufs_needed = (nbytes + ofi_bbuf_size - 1) / ofi_bbuf_size;
        size_t bytes_to_copy;
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
            gasneti_assert(bytes_to_copy <= ofi_bbuf_size);
            buf_container = buffs[i];
            gasneti_lifo_push(&bbuf_ctxt->bbuf_list, buf_container);
            memcpy(buf_container->buf, (void*)src_ptr, bytes_to_copy);

            OFI_INJECT_RETRY(&gasnetc_ofi_locks.rdma_tx,
                OFI_WRITE(gasnetc_ofi_rdma_epfd, buf_container->buf, bytes_to_copy, 
                    dest, dest_ptr, bbuf_ctxt), OFI_POLL_ALL);

            if_pf (FI_SUCCESS != ret)
                gasneti_fatalerror("fi_writemsg for bounce buffered data failed: %d\n", ret);

#if GASNET_DEBUG
            gasnetc_paratomic_increment(&pending_rdma,0);
#endif

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
        gasnetc_rdma_put(dest, dest_addr, src_addr, nbytes, ctxt_ptr GASNETI_THREAD_PASS);
        GASNETC_STAT_EVENT(NB_PUT_BLOCK);
        return 1;
    }

}

void
gasnetc_rdma_put(gex_Rank_t dest, void *dest_addr, void *src_addr, size_t nbytes,
		gasnetc_ofi_op_ctxt_t *ctxt_ptr GASNETI_THREAD_FARG)
{
	int ret = FI_SUCCESS;

	((gasnetc_ofi_op_ctxt_t *)ctxt_ptr)->callback = gasnetc_ofi_handle_rdma;

    PERIODIC_RMA_POLL();
    OFI_INJECT_RETRY(&gasnetc_ofi_locks.rdma_tx,
        OFI_WRITE(gasnetc_ofi_rdma_epfd, src_addr, nbytes, dest, dest_addr, ctxt_ptr), OFI_POLL_ALL);
	if (FI_SUCCESS != ret)
		gasneti_fatalerror("fi_write for normal message failed: %d\n", ret);
#if GASNET_DEBUG
	gasnetc_paratomic_increment(&pending_rdma,0);
#endif
}

void
gasnetc_rdma_get(void *dest_addr, gex_Rank_t dest, void * src_addr, size_t nbytes,
		gasnetc_ofi_op_ctxt_t *ctxt_ptr GASNETI_THREAD_FARG)
{
	int ret = FI_SUCCESS;

	((gasnetc_ofi_op_ctxt_t *)ctxt_ptr)->callback = gasnetc_ofi_handle_rdma;

    PERIODIC_RMA_POLL();

    OFI_INJECT_RETRY(&gasnetc_ofi_locks.rdma_tx,
        OFI_READ(gasnetc_ofi_rdma_epfd, dest_addr, nbytes, dest, src_addr, ctxt_ptr), OFI_POLL_ALL);

	if (FI_SUCCESS != ret)
		gasneti_fatalerror("fi_read failed: %d\n", ret);
#if GASNET_DEBUG
	gasnetc_paratomic_increment(&pending_rdma,0);
#endif
}

void
gasnetc_rdma_put_wait(gex_Event_t oph GASNETI_THREAD_FARG)
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
gasnetc_rdma_get_wait(gex_Event_t oph GASNETI_THREAD_FARG)
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
