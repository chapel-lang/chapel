/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ucx-conduit/gasnet_core.c $
 * Description: GASNet ucx conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2019, Mellanox Technologies LTD. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_ucx_req.h>
#include <gasnet_am.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>

GASNETI_IDENT(gasnetc_IdentString_Version, "$GASNetCoreLibraryVersion: " GASNET_CORE_VERSION_STR " $");
GASNETI_IDENT(gasnetc_IdentString_Name,    "$GASNetCoreLibraryName: " GASNET_CORE_NAME_STR " $");

enum {
  GASNETC_EXIT_ROLE_UNKNOWN,
  GASNETC_EXIT_ROLE_MASTER,
  GASNETC_EXIT_ROLE_SLAVE
};

#if GASNET_DEBUG
  static const char * volatile gasnetc_exit_state = "UNKNOWN STATE";
  #define GASNETC_EXIT_STATE(st) gasnetc_exit_state = st
#else
  #define GASNETC_EXIT_STATE(st) do {} while (0)
#endif

#define GASNETC_ROOT_NODE 0

int gasnetc_exit_running = 0;		/* boolean used to identify that exit process is started */

/* gasnete_threadidx_t used to identify what thread an exit process was started */
gasnete_threadidx_t gasnetc_exit_thread  = 0 ;

static gasneti_atomic_t gasnetc_exit_done = gasneti_atomic_init(0);	/* flag to show exit coordination done */
static gasneti_atomic_t gasnetc_exit_code = gasneti_atomic_init(0);	/* value to _exit() with */
static gasneti_atomic_t gasnetc_exit_dist = gasneti_atomic_init(0);	/* OR of reduce distances */
static gasneti_atomic_t gasnetc_exit_reds = gasneti_atomic_init(0);	/* count of reduce requests */
static gasneti_atomic_t gasnetc_exit_reqs = gasneti_atomic_init(0);	/* count of remote exit requests */
static gasneti_atomic_t gasnetc_exit_reps = gasneti_atomic_init(0);	/* count of remote exit replies */
static gasneti_atomic_t gasnetc_exit_role = gasneti_atomic_init(GASNETC_EXIT_ROLE_UNKNOWN);
static gasnetc_counter_t gasnetc_exit_repl_oust = GASNETC_COUNTER_INITIALIZER; /* track send of our AM reply */

/* If UCX routines are invoked from a signal handler function, the behavior of
 * the program is undefined. This flag uses to avoid use of UCX communications
 * in sighandler context.*/
static gasneti_atomic_t gasnetc_exit_atsighandler = gasneti_atomic_init(0);

static void gasnetc_atexit(int exitcode);

// gex_TM_t used for AM-based bootstrap collectives and exit handling
static gex_TM_t gasnetc_bootstrap_tm = NULL;

/* Exit coordination timeouts */
#define GASNETC_DEFAULT_EXITTIMEOUT_MAX		360.0	/* 6 minutes! */
#define GASNETC_DEFAULT_EXITTIMEOUT_MIN		2	/* 2 seconds */
#define GASNETC_DEFAULT_EXITTIMEOUT_FACTOR	0.25	/* 1/4 second */
static double gasnetc_exittimeout = GASNETC_DEFAULT_EXITTIMEOUT_MAX;


gex_AM_Entry_t const *gasnetc_get_handlertable(void);

gex_AM_Entry_t *gasnetc_handler; // TODO-EX: will be replaced with per-EP tables

gasneti_spawnerfn_t const *gasneti_spawner = NULL;

gasneti_ucx_module_t gasneti_ucx_module;
static char *gasnetc_ucx_addr_array = NULL;

size_t gasnetc_AMHeaderSize(void)
{
  return sizeof(gasnetc_sreq_hdr_t);
}

/* ------------------------------------------------------------------------------------ */
/*
  Bootstrap collectives
*/

static gex_Rank_t gasnetc_dissem_peers = 0;
static gex_Rank_t *gasnetc_dissem_peer = NULL;
static gex_Rank_t *gasnetc_exchange_rcvd = NULL;
static gex_Rank_t *gasnetc_exchange_send = NULL;

static void gasnetc_sys_coll_init(void)
{
  int i;
  const gex_Rank_t size = gasneti_nodes;
  const gex_Rank_t rank = gasneti_mynode;

  if (size == 1) {
    /* No network comms */
    goto done;
  }

  /* Construct vector of the dissemination peers */
  gasnetc_dissem_peers = 0;
  for (i = 1; i < size; i *= 2) {
    ++gasnetc_dissem_peers;
  }
  if (NULL == gasnetc_dissem_peer) {
    gasnetc_dissem_peer = gasneti_malloc(gasnetc_dissem_peers * sizeof(gex_Rank_t));
    gasneti_leak(gasnetc_dissem_peer);
  }
  for (i = 0; i < gasnetc_dissem_peers; ++i) {
    const gex_Rank_t distance = 1 << i;
    const gex_Rank_t peer = (distance <= rank) ? (rank - distance) : (rank + (size - distance));
    gasnetc_dissem_peer[i] = peer;
  }
  /* Compute the recv offset and send count for each step of exchange */
  gasnetc_exchange_rcvd = gasneti_malloc((gasnetc_dissem_peers+1) * sizeof(gex_Rank_t));
  gasnetc_exchange_send = gasneti_malloc(gasnetc_dissem_peers * sizeof(gex_Rank_t));
  {
    int step;
    for (step = 0; step < gasnetc_dissem_peers; ++step) {
      gasnetc_exchange_rcvd[step] = gasnetc_exchange_send[step] = 1 << step;
    }
    gasnetc_exchange_send[step-1] = gasneti_nodes - gasnetc_exchange_send[step-1];
    gasnetc_exchange_rcvd[step] = gasneti_nodes;
  }
done:
  // TODO: collectives
  //gasneti_assert(! gasneti_bootstrap_native_coll);
  //gasneti_bootstrap_native_coll = 1;
  //gasneti_spawner->Cleanup(); /* No futher use of ssh/mpi/pmi collectives */
  return;
}

static void gasnetc_sys_coll_fini(void)
{
  gasnetc_dissem_peers = 0;
  gasneti_free(gasnetc_dissem_peer);
  gasneti_free(gasnetc_exchange_rcvd);
  gasneti_free(gasnetc_exchange_send);

#if GASNET_DEBUG
  gasnetc_exchange_rcvd = NULL;
  gasnetc_exchange_send = NULL;
#endif
  //gasneti_bootstrap_native_coll = 0;
}

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* called at startup to check configuration sanity */
static void gasnetc_check_config(void) {
  gasneti_check_config_preinit();

  /* (###) add code to do some sanity checks on the number of nodes, handlers
   * and/or segment sizes */ 
}

GASNETI_INLINE(gasnetc_msgsource)
gex_Rank_t gasnetc_msgsource(gex_Token_t token) {
  gex_Rank_t sourceid;
  gasneti_assert(token);

  if (gasnetc_token_in_nbrhd(token)) {
    gex_Token_Info_t info;
    unsigned int rc = gasnetc_nbrhd_Token_Info(token, &info, GEX_TI_SRCRANK);
    gasneti_assert(rc & GEX_TI_SRCRANK);
    sourceid = info.gex_srcrank;
  } else {
    gasnetc_sreq_hdr_t *hdr = (gasnetc_sreq_hdr_t*)token;
    sourceid = hdr->src;
  }
  gasneti_assert(sourceid < gasneti_nodes);
  return sourceid;
}

static int gasnetc_connect_static(void)
{
  int i;
  ucs_status_t status;
  ucp_ep_params_t ep_params;


  for (i = 0; i < gasneti_nodes; ++i) {
    ep_params.field_mask = UCP_EP_PARAM_FIELD_REMOTE_ADDRESS;
    ep_params.address    = (ucp_address_t*)gasneti_ucx_module.ep_tbl[i].ucx_addr;
    status = ucp_ep_create(gasneti_ucx_module.ucp_worker, &ep_params,
                           &gasneti_ucx_module.ep_tbl[i].server_ep);
    if (UCS_OK != status) {
      return GASNET_ERR_NOT_INIT;
    }
  }
  return GASNET_OK;
}

static void gasnetc_connect_shutdown(void)
{
  for (int i = 0; i < gasneti_nodes; ++i) {
    gasneti_ucx_module.ep_tbl[i].ucx_addr = NULL;
    ucp_ep_destroy(gasneti_ucx_module.ep_tbl[i].server_ep);
  }
  gasneti_free(gasnetc_ucx_addr_array);
}

void gasnetc_ucx_empty_complete_cb(void *req, ucs_status_t status)
{
}

static int gasnetc_ucx_worker_flush(void)
{
  ucs_status_t status;
  ucs_status_ptr_t request;

  request = ucp_worker_flush_nb(gasneti_ucx_module.ucp_worker, 0,
                                gasnetc_ucx_empty_complete_cb);
  if_pt (NULL == request) {
      return GASNET_OK;
  } else if_pf(UCS_PTR_IS_ERR(request)) {
    gasneti_fatalerror("UCX worker wait failed: %d, %s", UCS_PTR_STATUS(request),
                       ucs_status_string(UCS_PTR_STATUS(request)));
    return GASNET_ERR_RESOURCE;
  }
  do {
    status = ucp_request_check_status(request);
    ucp_worker_progress(gasneti_ucx_module.ucp_worker);
  } while (status == UCS_INPROGRESS);

  if (UCS_OK != status) {
    gasneti_fatalerror("UCX worker wait failed: %d, %s",
                       UCS_PTR_STATUS(request),
                       ucs_status_string(UCS_PTR_STATUS(request)));
    return GASNET_ERR_RESOURCE;
  }
  ucp_request_free(request);

  return GASNET_OK;
}

#if GASNETC_PIN_SEGMENT
static int gasnetc_mem_map(void* addr, size_t size, gasnetc_mem_info_t *reg)
{
  ucp_mem_map_params_t mem_params;
  ucs_status_t status;

  memset(&mem_params, 0, sizeof(ucp_mem_map_params_t));
  mem_params.field_mask = UCP_MEM_MAP_PARAM_FIELD_ADDRESS |
                          UCP_MEM_MAP_PARAM_FIELD_LENGTH;
  mem_params.length = size;
  mem_params.address = addr;

  status = ucp_mem_map(gasneti_ucx_module.ucp_context, &mem_params, &reg->mem_h);
  if (status != UCS_OK) {
    gasneti_fatalerror("Attach segment failed: %s",
                       ucs_status_string(UCS_PTR_STATUS(status)));
  }

  return GASNET_OK;
}

static void gasnetc_minfo_reset(gasnetc_mem_info_t *minfo)
{
  gasneti_assert(NULL != minfo);
  memset(minfo, 0, sizeof(gasnetc_mem_info_t));
}

static int gasnetc_pin_segment(void *seg_start, size_t segsize,
                               gasneti_bootstrapExchangefn_t exchangefn)
{
  ucs_status_t status;
  int j;
  void * mem_info_buf = NULL;
  size_t mem_info_len;
  size_t info_offset = 0;
  size_t rkey_max_size = 0;
  gasnet_ep_info_t * my_ep_info = &gasneti_ucx_module.ep_tbl[gasneti_mynode];
  gex_Rank_t i;
  gasnetc_mem_info_t *mem_info;
  gasneti_list_t mem_info_list;
  size_t *rkey_sizes;

  gasneti_list_init(&mem_info_list);
  GASNETI_LIST_ITEM_ALLOC(mem_info, gasnetc_mem_info_t, gasnetc_minfo_reset);
  gasneti_list_enq(&mem_info_list, mem_info);

  status = gasnetc_mem_map((void *)seg_start, segsize, mem_info);
  if (status != UCS_OK) {
    gasneti_fatalerror("Memory map failed: %s",
                       ucs_status_string(UCS_PTR_STATUS(status)));
  }
  status = ucp_rkey_pack(gasneti_ucx_module.ucp_context,
                         mem_info->mem_h,
                         &mem_info->buffer,
                         &mem_info->bsize);
  if (status != UCS_OK) {
    ucp_mem_unmap(gasneti_ucx_module.ucp_context,
                  mem_info->mem_h);
    gasneti_fatalerror("rkey pack failed: %s",
                       ucs_status_string(UCS_PTR_STATUS(status)));
  }
  gasneti_rkey_unpack(my_ep_info->server_ep, mem_info->buffer,
                      &mem_info->rkey);
  mem_info->addr = (void *)seg_start;
  mem_info->length = segsize;

  /* move added mem_info to local table */
  gasneti_list_enq(&my_ep_info->mem_tbl, mem_info);

  /* identify max rkey size */
  rkey_max_size = MAX(rkey_max_size, mem_info->bsize);
  rkey_sizes = gasneti_calloc(gasneti_nodes, sizeof(size_t));
  (*exchangefn)(&rkey_max_size, sizeof(rkey_max_size), rkey_sizes);
  for (i = 0; i < gasneti_nodes; i++) {
    if (i == gasneti_mynode) {
      continue;
    }
    rkey_max_size = MAX(rkey_max_size, rkey_sizes[i]);
  }
  gasneti_free(rkey_sizes);

  /* pack my mem map info */
  mem_info_len =
      /* rkey size */ sizeof(uint64_t)
      +  /* rkey buf */ rkey_max_size
      + /* addr */ sizeof(uint64_t)
      + /* len */ sizeof(uint64_t);
  mem_info_buf = gasneti_calloc(1, mem_info_len);

  gasneti_mem_pack(mem_info_buf, &mem_info->bsize, sizeof(uint64_t),
                   0, info_offset);
  gasneti_mem_pack(mem_info_buf, mem_info->buffer,
                   mem_info->bsize, rkey_max_size, info_offset);
  gasneti_mem_pack(mem_info_buf, &mem_info->addr, sizeof(uint64_t),
                   0, info_offset);
  gasneti_mem_pack(mem_info_buf, &mem_info->length, sizeof(uint64_t),
                   0, info_offset);

  char * recv_buf = gasneti_malloc(mem_info_len * gasneti_nodes);

  /* TODO:
  * + When using PSHM we could store rkeys just once per supernode
  * + When not fully connected, we could utilize sparse storage
  */
  (*exchangefn)(mem_info_buf, mem_info_len, recv_buf);

  info_offset = 0;
  for (i = 0; i < gasneti_nodes; i++) {
    if (i == gasneti_mynode) {
      info_offset += mem_info_len;
      continue;
    }
    ucp_ep_h ep = GASNETC_UCX_GET_EP(i);
    gasnet_ep_info_t * ep_info = &gasneti_ucx_module.ep_tbl[i];

    GASNETI_LIST_ITEM_ALLOC(mem_info, gasnetc_mem_info_t, gasnetc_minfo_reset);
    gasneti_list_enq(&ep_info->mem_tbl, mem_info);

    gasneti_mem_unpack(&mem_info->bsize, recv_buf,
                       sizeof(uint64_t), 0, info_offset);
    mem_info->buffer =
        gasneti_calloc(1, mem_info->bsize);
    gasneti_mem_unpack(mem_info->buffer, recv_buf,
                       mem_info->bsize, rkey_max_size,
                       info_offset);
    gasneti_rkey_unpack(ep, mem_info->buffer, &mem_info->rkey);
    gasneti_mem_unpack(&mem_info->addr, recv_buf,
                       sizeof(uint64_t), 0, info_offset);
    gasneti_mem_unpack(&mem_info->length, recv_buf,
                       sizeof(uint64_t), 0, info_offset);
  }
  gasneti_assert(info_offset == mem_info_len * gasneti_nodes);

  gasneti_free(mem_info_buf);
  gasneti_free(recv_buf);

  return GASNET_OK;
}

static void gasnetc_unpin_segment(void)
{
  gex_Rank_t i;

  for (i = 0; i < gasneti_nodes; i++) {
    ucs_status_t status;
    gasnetc_mem_info_t *mem_info;
    gasneti_list_t *mem_tbl = &gasneti_ucx_module.ep_tbl[i].mem_tbl;

    while(NULL !=
          (mem_info = GASNETI_LIST_POP(mem_tbl, gasnetc_mem_info_t))) {
      if (gasneti_mynode == i) {
        status = ucp_mem_unmap(gasneti_ucx_module.ucp_context, mem_info->mem_h);
        if (status != UCS_OK) {
          gasneti_fatalerror("Attach segment failed: %s",
                             ucs_status_string(UCS_PTR_STATUS(status)));
        }
        ucp_rkey_buffer_release(mem_info->buffer);
      } else {
        gasneti_free(mem_info->buffer);
      }
      ucp_rkey_destroy(mem_info->rkey);
      GASNETI_LIST_RESET(mem_info)
      gasneti_free(mem_info);
    }
    gasneti_list_fini(mem_tbl);
  }
}
#endif // GASNETC_PIN_SEGMENT

static void gasnetc_ucx_fini(void)
{
  GASNETC_EXIT_STATE("gasnetc_ucx_fini called");
  gasnetc_ucx_worker_flush();
  GASNETC_EXIT_STATE("gasnetc_ucx_worker_flush called");
  gasnetc_sreq_list_free();
  GASNETC_EXIT_STATE("gasnetc_sreq_list_free called");
  gasnetc_am_req_pool_free();
  GASNETC_EXIT_STATE("gasnetc_am_req_pool_free called");

  /* cleanup UCX */
  gasnetc_connect_shutdown();
  GASNETC_EXIT_STATE("gasnetc_connect_shutdown called");

#if GASNETC_PIN_SEGMENT
  gasnetc_unpin_segment();
#endif
  ucp_worker_destroy(gasneti_ucx_module.ucp_worker);
  GASNETC_EXIT_STATE("ucp_worker_destroy called");
  gasnetc_rreq_list_free();
  GASNETC_EXIT_STATE("gasnetc_rreq_list_free called");
  gasnetc_buffer_pool_free();
  GASNETC_EXIT_STATE("gasnetc_buffer_pool_free called");
  ucp_cleanup(gasneti_ucx_module.ucp_context);
  GASNETC_EXIT_STATE("ucp_cleanup called");

  gasneti_free(gasneti_ucx_module.ep_tbl);
  gasneti_mutex_destroy(&gasneti_ucx_module.ucp_worker_lock);
}

static int gasnetc_init(gex_Client_t *client_p, gex_EP_t *ep_p,
                        const char *clientName,
                        int *argc, char ***argv, gex_Flags_t flags) {
  ucp_config_t *config;
  ucs_status_t status;
  ucp_params_t ucp_params;
  ucp_worker_params_t worker_params;
  gasnet_ep_info_t local_ep;
  ucp_address_t *ucx_local_addr;
  gex_Rank_t i;

  /*  check system sanity */
  gasnetc_check_config();

  if (gasneti_init_done) 
    GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already initialized");

  gasneti_freezeForDebugger();

  #if GASNET_DEBUG_VERBOSE
    /* note - can't call trace macros during gasnet_init because trace system not yet initialized */
    fprintf(stderr,"gasnetc_init(): about to spawn...\n"); fflush(stderr);
  #endif

  /* (###) bootstrap the nodes for your conduit - may need to modify if not using 
   * the unified spawning infrastructure 
   */
  gasneti_spawner = gasneti_spawnerInit(argc, argv, NULL, &gasneti_nodes, &gasneti_mynode);
  if (!gasneti_spawner) GASNETI_RETURN_ERRR(NOT_INIT, "GASNet job spawn failed");

  gasneti_init_done = 1;

  /* Must init timers after global env, and preferably before tracing */
  GASNETI_TICKS_INIT();

  #if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_init(): spawn successful - node %i/%i starting...\n", 
      gasneti_mynode, gasneti_nodes); fflush(stderr);
  #endif

  gasnetc_exittimeout = gasneti_get_exittimeout(GASNETC_DEFAULT_EXITTIMEOUT_MAX,
          GASNETC_DEFAULT_EXITTIMEOUT_MIN,
          GASNETC_DEFAULT_EXITTIMEOUT_FACTOR,
          GASNETC_DEFAULT_EXITTIMEOUT_MIN);

  /*
   * Initialize UCX
   */
  status = ucp_config_read("GASNET", NULL, &config);
  if (status != UCS_OK) {
    GASNETI_RETURN_ERRFR(RESOURCE, "Fail to read UCX config: %s",
                         ucs_status_string(status));
  }
  ucp_params.features        = UCP_FEATURE_TAG |
                               UCP_FEATURE_RMA |
                               UCP_FEATURE_AMO32 |
                               UCP_FEATURE_AMO64;
  ucp_params.request_size    = sizeof(gasnetc_ucx_request_t);
  ucp_params.request_init    = gasnetc_req_init;
  ucp_params.request_cleanup = NULL;
  ucp_params.field_mask      = UCP_PARAM_FIELD_FEATURES |
                               UCP_PARAM_FIELD_REQUEST_SIZE |
                               UCP_PARAM_FIELD_REQUEST_INIT |
                               UCP_PARAM_FIELD_REQUEST_CLEANUP;
  status = ucp_init(&ucp_params, config, &gasneti_ucx_module.ucp_context);
  ucp_config_release(config);
  if (UCS_OK != status) {
    return GASNET_ERR_NOT_INIT;
  }

  gasneti_mutex_init(&gasneti_ucx_module.ucp_worker_lock);

  worker_params.field_mask  = UCP_WORKER_PARAM_FIELD_THREAD_MODE;
  worker_params.thread_mode = UCS_THREAD_MODE_SINGLE;

  status = ucp_worker_create(gasneti_ucx_module.ucp_context, &worker_params,
                             &gasneti_ucx_module.ucp_worker);
  if (UCS_OK != status) {
    gasneti_fatalerror("Init failed: %s",
                       ucs_status_string(UCS_PTR_STATUS(status)));
  }

  status = ucp_worker_get_address(gasneti_ucx_module.ucp_worker,
                                  &local_ep.ucx_addr, &local_ep.ucx_addr_len);
  if (UCS_OK != status) {
    gasneti_fatalerror("Init failed: %s",
                       ucs_status_string(UCS_PTR_STATUS(status)));
  }

  /* Two-stage endpoint exchange:
   * 1 exchange EP sizes, get max ep-size
   * 2 use max ep size to exchange */
  gasneti_ucx_module.ep_tbl =
      gasneti_calloc(gasneti_nodes, sizeof(local_ep));

  size_t *ep_sizes = gasneti_calloc(gasneti_nodes, sizeof(*ep_sizes));
  size_t max_ep_size = 0;
  gasneti_bootstrapExchange(&local_ep.ucx_addr_len, sizeof(size_t), ep_sizes);
  for (i = 0; i < gasneti_nodes; i++) {
    gasneti_ucx_module.ep_tbl[i].ucx_addr_len = ep_sizes[i];
    max_ep_size = MAX(max_ep_size, ep_sizes[i]);
  }
  ucx_local_addr = gasneti_calloc(1, max_ep_size);
  GASNETI_MEMCPY(ucx_local_addr, local_ep.ucx_addr, local_ep.ucx_addr_len);
  gasnetc_ucx_addr_array = gasneti_calloc(gasneti_nodes, max_ep_size);
  gasneti_bootstrapExchange(ucx_local_addr, max_ep_size,
                            gasnetc_ucx_addr_array);
  for (i = 0; i < gasneti_nodes; i++) {
    size_t offset = max_ep_size * i;
    gasneti_ucx_module.ep_tbl[i].ucx_addr =
        (ucp_address_t*)(gasnetc_ucx_addr_array + offset);
  }

  ucp_worker_release_address(gasneti_ucx_module.ucp_worker, local_ep.ucx_addr);
  gasneti_free(ep_sizes);
  gasneti_free(ucx_local_addr);

  gasnetc_am_req_pool_alloc();
  gasnetc_buffer_pool_alloc();
  gasnetc_req_list_init();

  for (i = 0; i < gasneti_nodes; i++) {
    gasneti_list_init(&gasneti_ucx_module.ep_tbl[i].mem_tbl);
  }

  /* (###) Add code here to determine which GASNet nodes may share memory.
     The collection of nodes sharing memory are known as a "supernode".
     The (first) data structure to describe this is gasneti_nodemap[]:
        For all i: gasneti_nodemap[i] is the lowest node number collocated w/ node i
     where nodes are considered collocated if they have the same node "ID".
     Or in English:
       "gasneti_nodemap[] maps from node to first node on the same supernode."

     If the conduit has already communicated endpoint address information or
     a similar identifier that is unique per shared-memory compute node, then
     that info can be passed via arguments 2 through 4.
     Otherwise the conduit should pass a non-null bootstrapExchange function
     as argument 1 to use platform-specific IDs, such as gethostid().
     See gasneti_nodemapInit() in gasnet_internal.c for more usage documentation.
     See below for info on gasnetc_bootstrapExchange()

     If the conduit can build gasneti_nodemap[] w/o assistance, it should
     call gasneti_nodemapParse() after constructing it (instead of nodemapInit()).
  */
  gasneti_nodemapInit(gasneti_spawner->Exchange, NULL, 0, 0);

#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, you should initialize it here.
   * The 1st argument is normally gasneti_spawner->SNodeBroadcast or equivalent
   * The 2nd argument is the amount of shared memory space needed for any
   * conduit-specific uses.
   * The return value is a pointer to the space requested by the 2nd argument.
   * It is advisable that the conduit ensure pages in this space are touched,
   * possibly using gasneti_pshm_prefault(), prior to use of gasneti_segmentLimit()
   * or similar memory probes.
   */
  (void) gasneti_pshm_init(gasneti_spawner->SNodeBroadcast, 0);
#endif

  //  Create first Client, EP and TM *here*, for use in subsequent bootstrap collectives
  {
    //  allocate the client object
    gasneti_Client_t client = gasneti_alloc_client(clientName, flags, 0);
    *client_p = gasneti_export_client(client);

    //  create the initial endpoint with internal handlers
    if (gasnetc_EP_Create(ep_p, *client_p, flags))
      GASNETI_RETURN_ERRR(RESOURCE,"Error creating initial endpoint");
    gasneti_EP_t ep = gasneti_import_ep(*ep_p);
    gasnetc_handler = ep->_amtbl; // TODO-EX: this global variable to be removed

    gasneti_TM_t tm = gasneti_alloc_tm(ep, gasneti_mynode, gasneti_nodes, flags, 0);
    gasnetc_bootstrap_tm = gasneti_export_tm(tm);
  }

  /* allocate and attach an aux segment */
  gasnet_seginfo_t auxseg = gasneti_auxsegAttach((uintptr_t)-1, gasneti_bootstrapExchange);

  void *auxbase = auxseg.addr;
  uintptr_t auxsize = auxseg.size;

  uintptr_t limit = gasneti_segmentLimit((uintptr_t)-1, (uint64_t)-1,
                                         &gasneti_bootstrapExchange,
                                         &gasneti_bootstrapBarrier);

  /* determine Max{Local,GLobal}SegmentSize */
  gasneti_segmentInit(limit, &gasneti_bootstrapExchange, flags);

  /*
   * Establish connections with all nodes
   */
  if (GASNET_OK != (status = gasnetc_connect_static())) {
      return status;
  }

  gasnetc_sys_coll_init();

#if GASNETC_PIN_SEGMENT
  /* pin the aux segment and exchange the RKeys */
  gasnetc_pin_segment(auxbase, auxsize, &gasneti_bootstrapExchange);
#endif

  if (0 == gasneti_mynode) {
    fflush(NULL);
    fprintf(stderr,
      " WARNING: ucx-conduit is experimental and should not be used for\n"
      "          performance measurements.\n"
      "          Please see `ucx-conduit/README` for more details.\n");
    fflush(NULL);
  }
  gasneti_registerExitHandler(gasnetc_atexit);

  return GASNET_OK;
}

static void gasnetc_defaultSignalHandler(int sig) {
  gasneti_atomic_set(&gasnetc_exit_atsighandler, 1, 0);
  gasneti_defaultSignalHandler(sig);
}

/* ------------------------------------------------------------------------------------ */
static int gasnetc_attach_primary(void) {
  /* ------------------------------------------------------------------------------------ */
  /*  register fatal signal handlers */

  /* catch fatal signals and convert to SIGQUIT */
  gasneti_registerSignalHandlers(gasnetc_defaultSignalHandler);

  /*  (###) register any custom signal handlers required by your conduit 
   *        (e.g. to support interrupt-based messaging)
   */

  /* ------------------------------------------------------------------------------------ */
  /*  primary attach complete */
  gasneti_attach_done = 1;
  gasneti_spawner->Barrier();

  GASNETI_TRACE_PRINTF(C,("gasnetc_attach_primary(): primary attach complete"));

  gasnete_init(); /* init the extended API */

  gasneti_nodemapFini();

  /* ensure extended API is initialized across nodes */
  gasneti_spawner->Barrier();

  /* (###) Optionally tear down spawner's bootstrap collectives, 
   * ONLY if the spawner collectives are not used after attach
   */
  // gasneti_spawner->Cleanup();

  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
static int gasnetc_attach_segment(gex_Segment_t                 *segment_p,
                                  gex_TM_t                      tm,
                                  uintptr_t                     segsize,
                                  gasneti_bootstrapExchangefn_t exchangefn,
                                  gex_Flags_t                   flags) {
  /* ------------------------------------------------------------------------------------ */
  /*  register client segment  */

  gasnet_seginfo_t myseg = gasneti_segmentAttach(segment_p, 0, tm, segsize, exchangefn, flags);

#if GASNETC_PIN_SEGMENT
  /* pin the segment and exchange the RKeys */
  gasnetc_pin_segment(myseg.addr, myseg.size, exchangefn);
#endif

  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
// TODO-EX: this is a candidate for factorization (once we understand the per-conduit variations)
extern int gasnetc_attach( gex_TM_t               _tm,
                           gasnet_handlerentry_t  *table,
                           int                    numentries,
                           uintptr_t              segsize)
{
  GASNETI_TRACE_PRINTF(C,("gasnetc_attach(table (%i entries), segsize=%"PRIuPTR")",
                          numentries, segsize));
  gasneti_TM_t tm = gasneti_import_tm(_tm);
  gasneti_EP_t ep = tm->_ep;

  if (!gasneti_init_done) 
    GASNETI_RETURN_ERRR(NOT_INIT, "GASNet attach called before init");
  if (gasneti_attach_done) 
    GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already attached");

  /*  check argument sanity */
  #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    if ((segsize % GASNET_PAGESIZE) != 0) 
      GASNETI_RETURN_ERRR(BAD_ARG, "segsize not page-aligned");
    if (segsize > gasneti_MaxLocalSegmentSize) 
      GASNETI_RETURN_ERRR(BAD_ARG, "segsize too large");
  #else
    segsize = 0;
  #endif

  /*  primary attach  */
  if (GASNET_OK != gasnetc_attach_primary())
    GASNETI_RETURN_ERRR(RESOURCE,"Error in primary attach");

#if GASNETC_PIN_SEGMENT
    /*  register client segment  */
    gex_Segment_t seg; // g2ex segment is automatically saved by a hook
    /*  (###) may replace gasneti_defaultExchange with a conduit-specific exchange if available */
    if (GASNET_OK != gasnetc_attach_segment(&seg, _tm, segsize, gasneti_defaultExchange, GASNETI_FLAG_INIT_LEGACY))

      GASNETI_RETURN_ERRR(RESOURCE,"Error attaching segment");
#endif // GASNETC_PIN_SEGMENT

  /*  register client handlers */
  if (table && gasneti_amregister_legacy(ep->_amtbl, table, numentries) != GASNET_OK)
    GASNETI_RETURN_ERRR(RESOURCE,"Error registering handlers");

  /* ensure everything is initialized across all nodes */
  gasnet_barrier(0, GASNET_BARRIERFLAG_UNNAMED);

  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
// TODO-EX: this is a candidate for factorization (once we understand the per-conduit variations)
extern int gasnetc_Client_Init(
                               gex_Client_t            *client_p,
                               gex_EP_t                *ep_p,
                               gex_TM_t                *tm_p,
                               const char              *clientName,
                               int                     *argc,
                               char                    ***argv,
                               gex_Flags_t             flags)
{
  gasneti_assert(client_p);
  gasneti_assert(ep_p);
  gasneti_assert(tm_p);
  gasneti_assert(clientName);
#if !GASNET_NULL_ARGV_OK
  gasneti_assert(argc);
  gasneti_assert(argv);
#endif

  //  main init
  // TODO-EX: must split off per-client and per-endpoint portions
  if (!gasneti_init_done) {
    int retval = gasnetc_init(client_p, ep_p, clientName, argc, argv, flags);
    if (retval != GASNET_OK) GASNETI_RETURN(retval);
    gasneti_trace_init(argc, argv);
  } else {
    //  allocate the client object
    gasneti_Client_t client = gasneti_alloc_client(clientName, flags, 0);
    *client_p = gasneti_export_client(client);
    //  create the initial endpoint with internal handlers
    if (gasnetc_EP_Create(ep_p, *client_p, flags))
      GASNETI_RETURN_ERRR(RESOURCE,"Error creating initial endpoint");
  }
  gasneti_EP_t ep = gasneti_import_ep(*ep_p);

  // TODO-EX: create team
  gasneti_TM_t tm = gasneti_init_done
                    ? gasneti_import_tm(gasnetc_bootstrap_tm) // gasnetc_init() creates very first TM
                    : gasneti_alloc_tm(ep, gasneti_mynode, gasneti_nodes, flags, 0);
  *tm_p = gasneti_export_tm(tm);

  if (0 == (flags & GASNETI_FLAG_INIT_LEGACY)) {
    /*  primary attach  */
    if (GASNET_OK != gasnetc_attach_primary())
      GASNETI_RETURN_ERRR(RESOURCE,"Error in primary attach");

    /* ensure everything is initialized across all nodes */
    gasnet_barrier(0, GASNET_BARRIERFLAG_UNNAMED);
  }

  return GASNET_OK;
}

extern int gasnetc_Segment_Attach(
                gex_Segment_t          *segment_p,
                gex_TM_t               tm,
                uintptr_t              length)
{
  gasneti_assert(segment_p);

  #if GASNET_SEGMENT_EVERYTHING
    *segment_p = GEX_SEGMENT_INVALID;
    gex_Event_Wait(gex_Coll_BarrierNB(tm, 0));
    return GASNET_OK; 
  #endif

  /* (###) add code to create a segment collectively */
  if (GASNET_OK != gasnetc_attach_segment(segment_p, tm, length, gasneti_defaultExchange, 0))
    GASNETI_RETURN_ERRR(RESOURCE,"Error attaching segment");

  return GASNET_OK;
}

extern int gasnetc_EP_Create(gex_EP_t           *ep_p,
                             gex_Client_t       client,
                             gex_Flags_t        flags) {
  /* (###) add code here to create an endpoint belonging to the given client */
#if 1 // TODO-EX: This is a stub, which assumes 1 implicit call from ClientCreate
  static gasneti_mutex_t lock = GASNETI_MUTEX_INITIALIZER;
  gasneti_mutex_lock(&lock);
    static int once = 0;
    int prev = once;
    once = 1;
  gasneti_mutex_unlock(&lock);
  if (prev) gasneti_fatalerror("Multiple endpoints are not yet implemented");
#endif

  gasneti_EP_t ep = gasneti_alloc_ep(gasneti_import_client(client), flags, 0);
  *ep_p = gasneti_export_ep(ep);

  { /*  core API handlers */
    gex_AM_Entry_t *ctable = (gex_AM_Entry_t *)gasnetc_get_handlertable();
    int len = 0;
    int numreg = 0;
    gasneti_assert(ctable);
    while (ctable[len].gex_fnptr) len++; /* calc len */
    if (gasneti_amregister(ep->_amtbl, ctable, len, GASNETC_HANDLER_BASE, GASNETE_HANDLER_BASE, 0, &numreg) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering core API handlers");
    gasneti_assert_int(numreg ,==, len);
  }

  { /*  extended API handlers */
    gex_AM_Entry_t *etable = (gex_AM_Entry_t *)gasnete_get_handlertable();
    int len = 0;
    int numreg = 0;
    gasneti_assert(etable);
    while (etable[len].gex_fnptr) len++; /* calc len */
    if (gasneti_amregister(ep->_amtbl, etable, len, GASNETE_HANDLER_BASE, GASNETI_CLIENT_HANDLER_BASE, 0, &numreg) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering extended API handlers");
    gasneti_assert_int(numreg ,==, len);
  }

  return GASNET_OK;
}

extern int gasnetc_EP_RegisterHandlers(gex_EP_t                ep,
                                       gex_AM_Entry_t          *table,
                                       size_t                  numentries) {
  return gasneti_amregister_client(gasneti_import_ep(ep)->_amtbl, table, numentries);
}
/* ------------------------------------------------------------------------------------ */

/* gasnetc_exit_now
 *
 * First we set the atomic variable gasnetc_exit_done to allow the exit
 * of any threads which are spinning on it in gasnetc_exit().
 * Then this function tries hard to actually terminate the calling thread.
 * If for some unlikely reason gasneti_killmyprocess() returns, we abort().
 *
 * DOES NOT RETURN
 */
GASNETI_NORETURN
static void gasnetc_exit_now(int exitcode) {
  /* If anybody is still waiting, let them go */
  gasneti_atomic_set(&gasnetc_exit_done, 1, GASNETI_ATOMIC_WMB_POST);

  #if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_exit(): node %i/%i calling killmyprocess...\n",
      gasneti_mynode, gasneti_nodes); fflush(stderr);
  #endif
  gasneti_killmyprocess(exitcode);
  /* NOT REACHED */

  gasneti_reghandler(SIGABRT, SIG_DFL);
  gasneti_fatalerror("gasnetc_exit_now aborting...");
  /* NOT REACHED */
}

static void gasnetc_noop(void) { return; }
static void gasnetc_disable_AMs(void) {
  int i;
  for (i = GASNETE_HANDLER_BASE; i < GASNETC_MAX_NUMHANDLERS; ++i) {
    gasnetc_handler[i].gex_fnptr = (gex_AM_Fn_t)&gasnetc_noop;
  }
}

/* gasnetc_exit_sighandler
 *
 * This signal handler is for a last-ditch exit when a signal arrives while
 * attempting the graceful exit.  That includes SIGALRM if we get wedged.
 *
 * Just a (verbose) signal-handler wrapper for gasnetc_exit_now().
 *
 * DOES NOT RETURN
 */
static void gasnetc_exit_sighandler(int sig) {
  int exitcode = (int)gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE);
  static gasneti_atomic_t once = gasneti_atomic_init(1);

#if GASNET_DEBUG
  // protect until we reach reentrance check
  gasneti_reghandler(SIGALRM, _exit);
  gasneti_unblocksig(SIGALRM);
  alarm(30);
#endif

#if GASNET_DEBUG
  if (sig == SIGALRM) {
    static const char msg[] = "gasnet_exit(): WARNING: timeout during exit... goodbye.  [";
    const char * state = gasnetc_exit_state;
    (void) write(STDERR_FILENO, msg, sizeof(msg) - 1);
    (void) write(STDERR_FILENO, state, strlen(state));
    (void) write(STDERR_FILENO, "]\n", 2);
  } else {
    static const char msg1[] = "gasnet_exit(): ERROR: signal ";
    static const char msg2[] = " received during exit... goodbye.  [";
    const char * state = gasnetc_exit_state;
    char digit;

    (void) write(STDERR_FILENO, msg1, sizeof(msg1) - 1);

    /* assume sig < 100 */
    if (sig > 9) {
      digit = '0' + ((sig / 10) % 10);
      (void) write(STDERR_FILENO, &digit, 1);
    }
    digit = '0' + (sig % 10);
    (void) write(STDERR_FILENO, &digit, 1);

    (void) write(STDERR_FILENO, msg2, sizeof(msg2) - 1);
    (void) write(STDERR_FILENO, state, strlen(state));
    (void) write(STDERR_FILENO, "]\n", 2);
  }
#endif

  if (gasneti_atomic_decrement_and_test(&once, 0)) {
    /* We ask the bootstrap support to kill us, but only once */
    GASNETC_EXIT_STATE("in suicide timer");
    gasneti_reghandler(SIGALRM, gasnetc_exit_sighandler);
    gasneti_unblocksig(SIGALRM);
    alarm(5);
    gasneti_bootstrapAbort(exitcode);
  } else {
    gasnetc_exit_now(exitcode);
  }
  /* NOT REACHED */
}

/* gasnetc_exit_head
 *
 * All exit paths pass through here as the first step.
 * This function ensures that gasnetc_exit_code is written only once
 * by the first call.
 * It also lets the handler for remote exit requests know if a local
 * request has already begun.
 *
 * returns non-zero on the first call only
 * returns zero on all subsequent calls
 */
static int gasnetc_exit_head(int exitcode) {
  int rc = 0;

  GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  if (!gasnetc_exit_running) {
    gasneti_atomic_set(&gasnetc_exit_code, exitcode, GASNETI_ATOMIC_WMB_POST);
    gasnetc_exit_running = 1;
#ifdef GASNETC_UCX_THREADS
    {
      gasnete_threadidx_t threadidx;
      GASNETC_MY_THREAD_IDX(threadidx);
      gasnetc_exit_thread = threadidx;
    }
#endif
    rc = 1;
  }
  GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);

  return rc;
}

static void gasnetc_exit_tail(void) GASNETI_NORETURN;
static void gasnetc_exit_tail(void) {
  gasnetc_exit_now((int)gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE));
  /* NOT REACHED */
}

void gasnetc_exit_threads(void) {
#if GASNET_DEBUG
  GASNETC_LOCK_UCX();
  gasneti_assert(gasnetc_exit_running);
  GASNETC_UNLOCK_UCX();
#endif
  /* poll until it is time to exit */
  while (!gasneti_atomic_read(&gasnetc_exit_done, GASNETI_ATOMIC_RMB_PRE)) {
    gasneti_sched_yield(); /* NOT safe to use sleep() here - conflicts with alarm() */
  }
  gasnetc_exit_tail();
  /* NOT REACHED */
}

static int gasnetc_exit_reduce(int exitcode, int64_t timeout_us)
{
  gasneti_tick_t start_time = gasneti_ticks_now();
  int rc, i;

  gasneti_assert(timeout_us > 0);

  /* If the remote request has arrived then we've already failed */
  if (gasneti_atomic_read(&gasnetc_exit_reqs, 0)) return -1;

  GASNETC_EXIT_STATE("exitcode reduction: dissemination");
  for (i = 0; i < gasnetc_dissem_peers; ++i) {
    const uint32_t distance = 1 << i;
    rc = gasnetc_RequestSysShort(gasnetc_dissem_peer[i], NULL,
                                 gasneti_handleridx(gasnetc_exit_reduce_reqh),
                                 2, exitcode, distance);
    if (rc != GASNET_OK) return -1;
    do { /* wait for completion of the proper receive, which might arrive out of order */
      if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) return -1;
      gasnetc_req_poll(GASNETC_LOCK_REGULAR);
      if (gasneti_atomic_read(&gasnetc_exit_reqs, 0)) return -1;
    } while (!(distance & gasneti_atomic_read(&gasnetc_exit_dist, 0)));
    exitcode = gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE);
  }

  return 0;
}

static void gasnetc_exit_role_reqh(gex_Token_t token) {
  gex_Rank_t src = gasnetc_msgsource(token);
  int local_role, result;

  /* What role would the local node get if the requester is made the master? */
  local_role = (src == gasneti_mynode) ? GASNETC_EXIT_ROLE_MASTER : GASNETC_EXIT_ROLE_SLAVE;


  /* Try atomically to assume the proper role.  Result determines role of requester */
  result = gasneti_atomic_compare_and_swap(&gasnetc_exit_role, GASNETC_EXIT_ROLE_UNKNOWN, local_role, 0)
                ? GASNETC_EXIT_ROLE_MASTER : GASNETC_EXIT_ROLE_SLAVE;

  /* Inform the requester of the outcome. */
  GASNETI_SAFE(gasnetc_ReplySysShort(token, NULL, gasneti_handleridx(gasnetc_exit_role_reph),
           1, (gex_AM_Arg_t)result));

}

static void gasnetc_exit_role_reph(gex_Token_t token, gex_AM_Arg_t arg0) {

  int role = (int)arg0;

  gasneti_assert((role == GASNETC_EXIT_ROLE_MASTER) || (role == GASNETC_EXIT_ROLE_SLAVE));

  /* Set the role if not yet set.  Then assert that the assigned role has been assumed.
   * This way the assertion is checking that if the role was obtained by other means
   * (namely by receiving an exit request) it must match the election result. */
  gasneti_atomic_compare_and_swap(&gasnetc_exit_role, GASNETC_EXIT_ROLE_UNKNOWN, role, 0);
  gasneti_assert(gasneti_atomic_read(&gasnetc_exit_role, 0) == role);
}

static int gasnetc_get_exit_role(void)
{
  int role;
  int64_t timeout_us = gasnetc_exittimeout * 1.0e6;
  gex_Rank_t rank;
  gasneti_tick_t start_time;

  gasneti_assert(timeout_us > 0);

  role = gasneti_atomic_read(&gasnetc_exit_role, 0);
  if (GASNETC_EXIT_ROLE_UNKNOWN != role) {
    return role;
  }

  for (rank = 0; rank < gasneti_nodes; rank++) {
    GASNETI_SAFE(gasnetc_RequestSysShort(rank, NULL,
                   gasneti_handleridx(gasnetc_exit_role_reqh), 0));
    start_time = gasneti_ticks_now();
    /* Now spin until somebody tells us what our role is */
    do {
      gasnetc_req_poll(GASNETC_LOCK_REGULAR);
      role = gasneti_atomic_read(&gasnetc_exit_role, 0);
      if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 >
          timeout_us) {
        /* Go to try the next node */
        break;
      }
    } while (role == GASNETC_EXIT_ROLE_UNKNOWN);
    if (role != GASNETC_EXIT_ROLE_UNKNOWN) {
      return role;
    }
  }
  return GASNETC_EXIT_ROLE_UNKNOWN;
}

static int gasnetc_exit_master(int exitcode, int64_t timeout_us) {
  int i, rc;
  gasneti_tick_t start_time;

  gasneti_assert(timeout_us > 0);

  start_time = gasneti_ticks_now();

  /* Notify phase */
  for (i = 0; i < gasneti_nodes; ++i) {
    if (i == gasneti_mynode) continue;

    if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) return -1;
    rc = gasnetc_RequestSysShort(i, NULL,
                 gasneti_handleridx(gasnetc_exit_reqh),
             1, (gex_AM_Arg_t)exitcode);
    if (rc != GASNET_OK) return -1;
  }

  /* Wait phase - wait for replies from our N-1 peers */
  while (gasneti_atomic_read(&gasnetc_exit_reps, 0) < (gasneti_nodes - 1)) {
    if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) return -1;
    gasnetc_req_poll(GASNETC_LOCK_REGULAR);
  }

  return 0;
}

/* gasnetc_exit_slave
 *
 * We wait for a polite goodbye from the exit master.
 *
 * Takes a timeout in us as an argument
 *
 * Returns 0 on success, non-zero on timeout.
 */
static int gasnetc_exit_slave(int64_t timeout_us) {
  gasneti_tick_t start_time;

  gasneti_assert(timeout_us > 0);

  start_time = gasneti_ticks_now();

  /* wait until the exit request is received from the master */
  while (gasneti_atomic_read(&gasnetc_exit_reqs, 0) == 0) {
    if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) return -1;

    gasnetc_req_poll(GASNETC_LOCK_REGULAR);
  }

  /* wait until our reply has been placed on the wire */
  gasneti_sync_reads(); /* For non-atomic portion of gasnetc_exit_repl_oust */
  GASNET_BEGIN_FUNCTION(); // OK - not a critical-path
  gasnetc_counter_wait(&gasnetc_exit_repl_oust, 1 GASNETI_THREAD_PASS);

  return 0;
}

static void gasnetc_exit_body(void) {
  int exitcode;
  int graceful = 0;
  int64_t timeout_us = gasnetc_exittimeout * 1.0e6;
  int role;
#if GASNET_DEBUG && GASNET_PAR
  gasnete_threadidx_t threadidx;
  GASNETC_MY_THREAD_IDX(threadidx);
  GASNETC_LOCK_UCX();
  gasneti_assert(threadidx == gasnetc_exit_thread);
  GASNETC_UNLOCK_UCX();
#endif

  /* once we start a shutdown, ignore all future SIGQUIT signals or we risk reentrancy */
  (void)gasneti_reghandler(SIGQUIT, SIG_IGN);

  /* read exit code, stored by first caller to gasnetc_exit_head() */
  exitcode = gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE);

  /* Establish a last-ditch signal handler in case of failure. */
  alarm(0);
  gasneti_reghandler(SIGALRM, gasnetc_exit_sighandler);
  #if GASNET_DEBUG
    gasneti_reghandler(SIGABRT, SIG_DFL);
  #else
    gasneti_reghandler(SIGABRT, gasnetc_exit_sighandler);
  #endif
  gasneti_reghandler(SIGILL,  gasnetc_exit_sighandler);
  gasneti_reghandler(SIGSEGV, gasnetc_exit_sighandler);
  gasneti_reghandler(SIGFPE,  gasnetc_exit_sighandler);
  gasneti_reghandler(SIGBUS,  gasnetc_exit_sighandler);

  /* Disable processing of AMs, except core-specific ones */
  gasnetc_disable_AMs();

  GASNETI_TRACE_PRINTF(C,("gasnet_exit(%i)\n", exitcode));

  /* Timed MAX(exitcode) reduction to clearly distinguish collective exit */
  alarm(2 + (int)gasnetc_exittimeout);
  graceful = (gasnetc_exit_reduce(exitcode, timeout_us) == 0);

  gasnetc_sys_coll_fini();

  /* Try to flush out all the output, allowing upto 60s */
  GASNETC_EXIT_STATE("flushing output");
  alarm(60);
  {
    gasneti_flush_streams();
    gasneti_trace_finish();
    alarm(0);
    gasneti_sched_yield();
  }

  alarm(1 + (int)gasnetc_exittimeout);
  if (!graceful) {
    exitcode = gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE);

    GASNETC_EXIT_STATE("performing non-collective exit");
    alarm(10);
    role = gasnetc_get_exit_role();

    /* Attempt a coordinated shutdown */
    GASNETC_EXIT_STATE("coordinating shutdown");
    alarm(1 + (int)gasnetc_exittimeout);
    switch (role) {
    case GASNETC_EXIT_ROLE_MASTER:
      /* send all the remote exit requests and wait for the replies */
      graceful = (gasnetc_exit_master(exitcode, timeout_us) == 0);
      break;
    case GASNETC_EXIT_ROLE_SLAVE:
      graceful = (gasnetc_exit_slave(timeout_us) == 0);
      break;
    default:
        gasneti_fatalerror("invalid exit role");
    }
    GASNETC_EXIT_STATE("in gasneti_bootstrapAbort()");
    gasneti_bootstrapAbort(exitcode);
  } else {
    /* doing a poll of the receive queue while there are unreceived requests */
    alarm(10);
    /* waiting to completion all requests */
    GASNETC_EXIT_STATE("flushing ucx requests");
    gasnetc_send_list_wait(GASNETC_LOCK_REGULAR);
    while(gasnetc_req_poll(GASNETC_LOCK_REGULAR));

    alarm(10);
    GASNETC_EXIT_STATE("in gasneti_bootstrapFini()");
    gasneti_bootstrapFini();

    alarm(10);
    GASNETC_EXIT_STATE("ucx finalization");
    gasnetc_ucx_fini();
  }
  alarm(0);
}

extern void gasnetc_exit(int exitcode) {
  /* checking at sighandler now, ucx shouldn't be used in signal
   * handler function */
  if (gasneti_atomic_read(&gasnetc_exit_atsighandler, 0)) {
    gasneti_bootstrapAbort(exitcode);
    gasnetc_exit_tail();
    /* NOT REACHED */
  }
  gasnetc_exit_head(exitcode);
  gasnetc_exit_body();
  gasnetc_exit_tail();
  /* NOT REACHED */
}

/* gasnetc_atexit
 *
 * This is a simple (at,on_}exit() handler to achieve a hopefully graceful exit.
 * We use the functions gasnetc_exit_{head,body}() to coordinate the shutdown.
 * Note that we don't call gasnetc_exit_tail() since we anticipate the normal
 * exit() procedures to shutdown the multi-threaded process nicely and also
 * because with atexit() we don't have access to the exit code!
 *
 * With atexit(), we don't have access to the exit code to send to the other
 * nodes in the event this is a non-collective exit.  However, experience with at
 * lease one MPI suggests that when using MPI for bootstrap a non-zero return from
 * at least one executable is sufficient to produce that non-zero exit code from
 * the parallel job.  Therefore, we can "safely" pass 0 to our peers and still
 * expect to preserve a non-zero exit code for the GASNet job as a whole.  Of course
 * there is no _guarantee_ this will work with all bootstraps.
 */
static void gasnetc_atexit(int exitcode) {
  /* Check return from _head to avoid reentrance */
  if (gasnetc_exit_head(exitcode)) {
    gasnetc_exit_body();
  }
  return;
}

static void gasnetc_exit_reqh(gex_Token_t token, gex_AM_Arg_t arg0) {
  /* The master will send this AM, but should _never_ receive it */
  gasneti_assert(gasneti_atomic_read(&gasnetc_exit_role, 0) != GASNETC_EXIT_ROLE_MASTER);

  /* We should never receive this AM multiple times */
  gasneti_assert(gasneti_atomic_read(&gasnetc_exit_reqs, 0) == 0);

  /* If we didn't already know, we are now certain our role is "slave" */
  (void)gasneti_atomic_compare_and_swap(&gasnetc_exit_role, GASNETC_EXIT_ROLE_UNKNOWN, GASNETC_EXIT_ROLE_SLAVE, 0);

  /* Send a reply so the master knows we are reachable */
  gasnetc_counter_inc(&gasnetc_exit_repl_oust);
  GASNETI_SAFE(gasnetc_ReplySysShort(token, &gasnetc_exit_repl_oust,
           gasneti_handleridx(gasnetc_exit_reph), /* no args */ 0));
  gasneti_sync_writes(); /* For non-atomic portion of gasnetc_exit_repl_oust */

  /* Count the exit requests, so gasnetc_exit_slave() knows when to return */
  gasneti_atomic_increment(&gasnetc_exit_reqs, 0);

  /* Initiate an exit IFF this is the first we've heard of it */
  if (gasnetc_exit_head(arg0)) {
    gasneti_sighandlerfn_t handler;
    /* IMPORTANT NOTE
     * When we reach this point we are in a request handler which will never return.
     * Care should be taken to ensure this doesn't wedge the AM recv logic.
     *
     * This is currently safe because:
     * 1) request handlers are run w/ no locks held
     * 2) we poll for AMs in all the places we need them
     */

    /* To try and be reasonably robust, want to avoid performing the shutdown and exit from signal
     * context if we can avoid it.  However, we must raise SIGQUIT if the user has registered a handler.
     * Therefore we inspect what is registered before calling raise().
     *
     * XXX we don't do this atomically w.r.t the signal
     * XXX we don't do the right thing w/ SIG_ERR and SIG_HOLD
     */
    handler = gasneti_reghandler(SIGQUIT, SIG_IGN);
    if ((handler != gasneti_defaultSignalHandler) &&
#ifdef SIG_HOLD
  (handler != (gasneti_sighandlerfn_t)SIG_HOLD) &&
#endif
  (handler != (gasneti_sighandlerfn_t)SIG_ERR) &&
  (handler != (gasneti_sighandlerfn_t)SIG_IGN) &&
  (handler != (gasneti_sighandlerfn_t)SIG_DFL)) {
      (void)gasneti_reghandler(SIGQUIT, handler);
      #if 1
        raise(SIGQUIT);
        /* Note: Both ISO C and POSIX assure us that raise() won't return until after the signal handler
         * (if any) has executed.  However, if that handler calls gasnetc_exit(), we'll never return here. */
      #elif 0
  kill(getpid(),SIGQUIT);
      #else
  handler(SIGQUIT);
      #endif
    } else {
      /* No need to restore the handler, since _exit_body will set it to SIG_IGN anyway. */
    }

    gasnetc_exit_body();
    gasnetc_exit_tail();
    /* NOT REACHED */
  }

  return;
}

/* gasnetc_exit_reph
 *
 * Simply count replies
 */
static void gasnetc_exit_reph(gex_Token_t token) {
  gasneti_atomic_increment(&gasnetc_exit_reps, 0);
}

// TODO-EX:
// This conduit curently uses the extended-ref for all RMA.  That, in turn,
// uses AMs in an NBI access region.  Unfortunately, that means one cannot run
// progress functions that may communicate using NBI (or accurately then cannot
// call gex_NB_Test()).  Otherwise, assertions regarding access regions result.
//
// This avoids the problem by omitting progress functions on entry to AMRequest.
#define GASNETC_IMMEDIATE_MAYBE_POLL_NOPF(flag) \
        do { if (GASNETC_IMMEDIATE_WOULD_POLL(flag)) gasnetc_AMPoll(GASNETI_THREAD_PASS_ALONE); } while (0)
/* ------------------------------------------------------------------------------------ */
/*
  Misc. Active Message Functions
  ==============================
*/
#if GASNET_PSHM
/* (###) GASNETC_GET_HANDLER
 *   If your conduit will support PSHM, then there needs to be a way
 *   for PSHM to see your handler table.  If you use the recommended
 *   implementation then you don't need to do anything special.
 *   Othwerwise, #define GASNETC_GET_HANDLER in gasnet_core_fwd.h and
 *   implement gasnetc_get_handler() as a macro in
 *   gasnet_core_internal.h
 *
 * (###) Tokens and "nbrhd" (loopback and PSHM):
 *   To permit conduit-specific tokens to co-exist with ones used by the
 *   conduit-independent implementation of AMs within the neighborhood,
 *   the nbrhd implementation produces tokens with the least-significant
 *   bit set (assuming the conduit never will).  This restricts the
 *   conduit's implemention of tokens, but allows the common choice in
 *   which tokens are pointers to a type with alignment greater than 1.
 */
#endif

extern gex_TI_t gasnetc_Token_Info(
                gex_Token_t         token,
                gex_Token_Info_t    *info,
                gex_TI_t            mask)
{
  const gasnetc_sreq_hdr_t *hdr = (gasnetc_sreq_hdr_t*)token;

  gasneti_assert(token);
  gasneti_assert(info);

  /* (###) If your conduit is using the default support for AMs within
   * a Neighborhood (including loopback) then this hook is necessary.
   */
  if (gasnetc_token_in_nbrhd(token)) {
    return gasnetc_nbrhd_Token_Info(token, info, mask);
  }

  /* (###) Recommended implementation is to set all supported fields without
   * testing bits in 'mask'.  The exception to this would be for fields that
   * the conduit considers "costly" to populate.
   */
  gex_TI_t result = 0;

  /* (###) add code here to write the source into info->gex_srcrank */
  info->gex_srcrank = hdr->src;
  result |= GEX_TI_SRCRANK;

  /* (###) add code here to write the receiving EP into info->gex_ep */
  info->gex_ep = gasneti_THUNK_EP;
  result |= GEX_TI_EP;

  /* (###) add code here to write the address of the handle entry into info->gex_entry (optional) */
  //info->gex_entry = 0;//###;
  //result |= GEX_TI_ENTRY;

  /* (###) add code here to set boolean "is a request" field info->gex_is_req (optional) */
  //info->gex_is_req = real_token->u.generic.is_req;
  //result |= GEX_TI_IS_REQ;

  /* (###) add code here to set boolean "is a long" field info->gex_is_long (optional) */
  //info->gex_is_long = real_token->is_long;
  //result |= GEX_TI_IS_LONG;

  return GASNETI_TOKEN_INFO_RETURN(result, info, mask);
}

extern int gasnetc_AMPoll(GASNETI_THREAD_FARG_ALONE) {
  GASNETI_CHECKATTACH();
#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it make progress here. */
  gasneti_AMPSHMPoll(0 GASNETI_THREAD_PASS);
#endif
  /* protected progress of UCX */
  gasnetc_req_poll(GASNETC_LOCK_REGULAR);
  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
*/
extern int gasnetc_RequestSysShort(gex_Rank_t jobrank,
                                   gasnetc_counter_t *counter,
                                   gex_AM_Index_t handler,
                                   int numargs, ...) {
  GASNET_BEGIN_FUNCTION();
  int retval;
  va_list argptr;

  GASNETI_TRACE_AMREQUESTMEDIUM(gasneti_THUNK_TM,jobrank,handler,NULL,0,/*flags*/0,numargs);

  /* ensure AM progress, but NOT progress functions */
  gasnetc_req_poll_rcv(GASNETC_LOCK_REGULAR);

  va_start(argptr, numargs);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    retval = gasnetc_nbrhd_RequestGeneric ( gasneti_Short, jobrank, handler,
                                            NULL, 0, NULL,
                                            0, numargs, argptr GASNETI_THREAD_PASS);
    if_pf (counter) gasnetc_atomic_increment(&counter->completed, 0);
  } else {
    gasnetc_counter_t *counter_ptr = NULL;
    gasnetc_cbfunc_t cbfunc = NULL;
    gasnetc_atomic_val_t *local_cnt = NULL;
    if (counter) {
      counter_ptr = counter;
      cbfunc = gasnetc_cb_counter;
      local_cnt = &counter->initiated;
    }
    retval = gasnetc_am_reqrep_inner(GASNETC_UCX_AM_SHORT, jobrank, handler, 0,
                                     0, 0, numargs, argptr, NULL, 0, NULL,
                                     local_cnt, cbfunc,
                                     counter_ptr GASNETI_THREAD_PASS);
  }
  va_end(argptr);
  return retval;
}

extern int gasnetc_ReplySysShort(gex_Token_t token,
                               gasnetc_counter_t *counter,
                               gex_AM_Index_t handler,
                               int numargs, ...) {
  GASNET_BEGIN_FUNCTION();
  int retval;
  va_list argptr;
  gasneti_assert(token);
  GASNETI_TRACE_AMREPLYSHORT(token,handler,/*flags*/0,numargs);
  va_start(argptr, numargs);
  if_pt (gasnetc_token_in_nbrhd(token)) {
    retval = gasnetc_nbrhd_ReplyGeneric ( gasneti_Short, token, handler,
                                          NULL, 0, NULL,
                                          0, numargs, argptr);
    if_pf (counter) gasnetc_atomic_increment(&counter->completed, 0);
  } else {
    gex_Rank_t jobrank = gasnetc_msgsource(token);
    gasnetc_counter_t *counter_ptr = NULL;
    gasnetc_cbfunc_t cbfunc = NULL;
    gasnetc_atomic_val_t *local_cnt = NULL;
    if (counter) {
      counter_ptr = counter;
      cbfunc = gasnetc_cb_counter;
      local_cnt = &counter->initiated;
    }
    retval = gasnetc_am_reqrep_inner(GASNETC_UCX_AM_SHORT, jobrank, handler, 0,
                                     0, 0, numargs, argptr, NULL, 0, NULL,
                                     local_cnt, cbfunc,
                                     counter_ptr GASNETI_THREAD_PASS);
  }

  va_end(argptr);
  return retval;
}

GASNETI_INLINE(gasnetc_AMRequestShort)
int gasnetc_AMRequestShort( gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  int retval;
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    retval = gasnetc_nbrhd_RequestGeneric(gasneti_Short, jobrank, handler,
                                          0, 0, 0, flags, numargs,
                                          argptr GASNETI_THREAD_PASS);
  } else {
    retval = gasnetc_AM_ReqRepGeneric(GASNETC_UCX_AM_SHORT, jobrank, handler,
                                      NULL, flags, 1, numargs,
                                      argptr, NULL, 0, NULL GASNETI_THREAD_PASS);
  }

  return retval;
}

extern int gasnetc_AMRequestShortM( 
                            gex_TM_t tm,/* local context */
                            gex_Rank_t rank,       /* with tm, defines remote context */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            gex_Flags_t flags
                            GASNETI_THREAD_FARG,
                            int numargs, ...) {
  GASNETI_COMMON_AMREQUESTSHORT(tm,rank,handler,flags,numargs);
  GASNETC_IMMEDIATE_MAYBE_POLL_NOPF(flags); /* (###) poll at least once, to assure forward progress */

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMRequestShort(tm,rank,handler,flags,numargs,argptr GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
}

#if !GASNETC_HAVE_NP_REQ_MEDIUM // (###)

// This provides a template implementing the following two external functions:
//     int gasnetc_AMRequestMediumV()
//     int gasnetc_AMRequestMediumM()
//
// In this example both are in terms of a common inline function:
//     int gasnetc_AMRequestMedium()
// but that is not a requirement.
//
// This approach is suitable if your client does not wish to implement the
// Prepare and Commit required to provide a conduit-specialized implementation
// of Negotiated-Payload AMs.

GASNETI_INLINE(gasnetc_AMRequestMedium)
int gasnetc_AMRequestMedium(gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  int retval;
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);

  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    gasneti_leaf_finish(lc_opt); // synchronous LC
    retval = gasnetc_nbrhd_RequestGeneric( gasneti_Medium, jobrank, handler,
                                           source_addr, nbytes, 0, flags,
                                           numargs, argptr GASNETI_THREAD_PASS);
  } else {
    gasneti_leaf_finish(lc_opt); // TODO-EX: should support async local completion
    retval = gasnetc_AM_ReqRepGeneric(GASNETC_UCX_AM_MEDIUM, jobrank, handler,
                                      NULL, flags, 1, numargs,
                                      argptr, source_addr,
                                      nbytes, NULL GASNETI_THREAD_PASS);
  }

  return retval;
}

extern int gasnetc_AMRequestMediumV(
                            gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  return gasnetc_AMRequestMedium(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
}

extern int gasnetc_AMRequestMediumM( 
                            gex_TM_t tm,/* local context */
                            gex_Rank_t rank,       /* with tm, defines remote context */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            void *source_addr, size_t nbytes,   /* data payload */
                            gex_Event_t *lc_opt,       /* local completion of payload */
                            gex_Flags_t flags
                            GASNETI_THREAD_FARG,
                            int numargs, ...) {
  GASNETI_COMMON_AMREQUESTMEDIUM(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs);
  GASNETC_IMMEDIATE_MAYBE_POLL_NOPF(flags); /* (###) poll at least once, to assure forward progress */

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMRequestMedium(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
}

#else // GASNETC_HAVE_NP_REQ_MEDIUM

// This provides a template implementing the following three external functions:
//     int gasnetc_AMRequestMediumM()
//     int gasnetc_AM_PrepareRequestMedium()
//     void gasnetc_AM_CommitRequestMediumM()
//
// In this example all three are in terms of two inline functions:
//     int gasnetc_prepare_req_medium()
//     void gasnetc_commit_req_medium()
// but that is not a requirement.
//
// This example provides a specialized implementation of Negotiated-Payload
// RequestMedium (by providing gasnetc_AM_PrepareRequestMedium() and
// gasnetc_AM_CommitRequestMediumM()) and one must
//    #define GASNETC_HAVE_NP_REQ_MEDIUM 1
// in the conduit's gasnet_core_fwd.h to disable (conflicting) definitions in
// the reference implementation.

GASNETI_INLINE(gasnetc_prepare_req_medium)
int gasnetc_prepare_req_medium(
                       gasneti_AM_SrcDesc_t    sd,
                       const int               isFixed,
                       gex_TM_t                tm,
                       gex_Rank_t              rank,
                       const void             *client_buf,
                       size_t                  least_payload,
                       size_t                  most_payload,
                       gex_Event_t            *lc_opt,
                       gex_Flags_t             flags,
                       unsigned int            nargs
                       GASNETI_THREAD_FARG)
{
  // (###) Add code here to allocate resources (possibly unsuccessfully, if
  // flags includes GEX_FLAG_IMMEDIATE) for a Medium Request, storing arguments
  // in either the conduit-specific header or in the 'sd'.

  // No provisions are needed for communication within the neighborhood,
  // since this function is only called in the non-nbrhd case.

  // In this example:
  // + isFixed is 0 when called from gasnetc_AM_PrepareRequestMedium, and
  //   all other arguments are passed unchanged
  // + isFixed is 1 when called from gasnetc_AMRequestMediumM, and
  //   - client_buf is the source_buf argument to gasnetc_AMRequestMediumM
  //   - least_payload is unused
  //   - most_payload is the nbytes argument to gasnetc_AMRequestMediumM

  return 0; // or 1 if GEX_FLAG_IMMEDIATE passed and resource(s) unavailable
}

GASNETI_INLINE(gasnetc_commit_req_medium)
void gasnetc_commit_req_medium(
                       gasneti_AM_SrcDesc_t    sd,
                       const int               isFixed,
                       gex_AM_Index_t          handler,
                       size_t                  nbytes,
                       va_list                 argptr)
{
  // (###) Add code here to initiate a Medium Request using the union of the
  // arguments, and the values stored in 'sd' by the preceding call to
  // gasnetc_prepare_req_medium() or in locations referenced by 'sd'.

  // No provisions are needed for communication within the neighborhood,
  // since this function is only called in the non-nbrhd case.
}

extern int gasnetc_AMRequestMediumM(
                    gex_TM_t tm,                      /* local context */
                    gex_Rank_t rank,                  /* with tm, defines remote context */
                    gex_AM_Index_t handler,           /* index into destination ep's handler table */
                    void *source_addr, size_t nbytes, /* data payload */
                    gex_Event_t *lc_opt,              /* local completion of payload */
                    gex_Flags_t flags
                    GASNETI_THREAD_FARG,
                    int numargs, ...) {
  GASNETI_COMMON_AMREQUESTMEDIUM(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs);
  GASNETC_IMMEDIATE_MAYBE_POLL(flags); /* (###) poll at least once, to assure forward progress */

  va_list argptr;
  va_start(argptr, numargs);

  int retval;
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, jobrank);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    gasneti_leaf_finish(lc_opt); // synchronous LC
    retval = gasnetc_nbrhd_RequestGeneric(gasneti_Medium, jobrank, handler,
                                          source_addr, nbytes, NULL,
                                          flags, numargs, argptr GASNETI_THREAD_PASS);
  } else {
    struct gasneti_AM_SrcDesc the_sd;
    retval = gasnetc_prepare_req_medium(&the_sd,1,jobrank,source_addr,0,nbytes,
                                        lc_opt,flags,numargs GASNETI_THREAD_PASS);
    if (!retval) {
      gasnetc_commit_req_medium(&the_sd,1,handler,nbytes,argptr);
    }
  }

  va_end(argptr);
  return retval;
}

extern gex_AM_SrcDesc_t gasnetc_AM_PrepareRequestMedium(
                       gex_TM_t           tm,
                       gex_Rank_t         rank,
                       const void        *client_buf,
                       size_t             least_payload,
                       size_t             most_payload,
                       gex_Event_t       *lc_opt,
                       gex_Flags_t        flags
                       GASNETI_THREAD_FARG,
                       unsigned int       nargs)
{
    gasneti_AM_SrcDesc_t sd = gasneti_init_request_srcdesc(GASNETI_THREAD_PASS_ALONE);
    GASNETI_COMMON_PREP_REQ(sd,tm,rank,client_buf,least_payload,most_payload,NULL,lc_opt,flags,nargs,Medium);

    GASNETC_IMMEDIATE_MAYBE_POLL(flags); /* (###) poll at least once, to assure forward progress */

    flags &= ~(GEX_FLAG_AM_PREPARE_LEAST_CLIENT | GEX_FLAG_AM_PREPARE_LEAST_ALLOC);
    gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);

    if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
        sd = gasnetc_nbrhd_PrepareRequest(sd, gasneti_Medium, jobrank,
                                           client_buf, least_payload, most_payload,
                                           NULL, lc_opt, flags, nargs);
    } else {
        int imm;
        imm = gasnetc_prepare_req_medium(sd,0,jobrank,client_buf,least_payload,most_payload,
                                         lc_opt,flags,nargs GASNETI_THREAD_PASS);
        if (imm) {
            gasneti_reset_srcdesc(sd);
            sd = NULL; // GEX_AM_SRCDESC_NO_OP
        } else {
            gasneti_init_sd_poison(sd);
            sd->_is_nbrhd = 0;
        }
    }

    GASNETI_TRACE_PREP_RETURN(REQUEST_MEDIUM, sd);
    return gasneti_export_srcdesc(sd);
}

extern void gasnetc_AM_CommitRequestMediumM(
                       gex_AM_Index_t          handler,
                       size_t                  nbytes
                       GASNETI_THREAD_FARG,
                     #if GASNET_DEBUG
                       unsigned int            nargs_arg,
                     #endif
                       gex_AM_SrcDesc_t        sd_arg, ...)
{
    gasneti_AM_SrcDesc_t sd = gasneti_import_srcdesc(sd_arg);

    GASNETI_COMMON_COMMIT_REQ(sd,handler,nbytes,NULL,nargs_arg,Medium);

    va_list argptr;
    va_start(argptr, sd_arg);
    if (sd->_is_nbrhd) {
        gasnetc_nbrhd_CommitRequest(sd, gasneti_Medium, handler, nbytes, NULL, argptr);
    } else {
        gasnetc_commit_req_medium(sd,handler,nbytes,argptr);
    }
    va_end(argptr);

    gasneti_reset_srcdesc(sd);
}
#endif // GASNETC_HAVE_NP_REQ_MEDIUM

GASNETI_INLINE(gasnetc_AMRequestLong)
int gasnetc_AMRequestLong(  gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes, void *dest_addr,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  int retval;


  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    gasneti_leaf_finish(lc_opt); // synchronous LC
    retval = gasnetc_nbrhd_RequestGeneric( gasneti_Long, jobrank, handler,
                                           source_addr, nbytes, dest_addr,
                                           flags, numargs,
                                           argptr GASNETI_THREAD_PASS);
  } else {
    retval = gasnetc_AM_ReqRepGeneric(GASNETC_UCX_AM_LONG, jobrank, handler,
                                      lc_opt, flags, 1, numargs,
                                      argptr, source_addr,
                                      nbytes, dest_addr GASNETI_THREAD_PASS);
  }

  return retval;
}

extern int gasnetc_AMRequestLongV(
                            gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes, void *dest_addr,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  return gasnetc_AMRequestLong(tm,rank,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
}

extern int gasnetc_AMRequestLongM(
                            gex_TM_t tm,/* local context */
                            gex_Rank_t rank,       /* with tm, defines remote context */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            gex_Event_t *lc_opt,       /* local completion of payload */
                            gex_Flags_t flags
                            GASNETI_THREAD_FARG,
                            int numargs, ...) {
  GASNETI_COMMON_AMREQUESTLONG(tm,rank,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs);
  GASNETC_IMMEDIATE_MAYBE_POLL_NOPF(flags); /* (###) poll at least once, to assure forward progress */

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMRequestLong(tm,rank,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
}

GASNETI_INLINE(gasnetc_AMReplyShort)
int gasnetc_AMReplyShort(   gex_Token_t token, gex_AM_Index_t handler,
                            gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  int retval;

  if_pt (gasnetc_token_in_nbrhd(token)) {
    retval = gasnetc_nbrhd_ReplyGeneric( gasneti_Short, token, handler,
                                         0, 0, 0,
                                         flags, numargs, argptr);
  } else {
    GASNET_BEGIN_FUNCTION(); // TODO: could embed threadinfo in token
    gex_Rank_t jobrank = gasnetc_msgsource(token);
    retval = gasnetc_AM_ReqRepGeneric(GASNETC_UCX_AM_SHORT, jobrank, handler,
                                      NULL, flags, 0, numargs, argptr, NULL, 0,
                                      NULL GASNETI_THREAD_PASS);
  }

  return retval;
}

extern int gasnetc_AMReplyShortM( 
                            gex_Token_t token,     /* token provided on handler entry */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            gex_Flags_t flags,
                            int numargs, ...) {
  GASNETI_COMMON_AMREPLYSHORT(token,handler,flags,numargs);

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMReplyShort(token,handler,flags,numargs,argptr);
  va_end(argptr);
  return retval;
}

#if !GASNETC_HAVE_NP_REP_MEDIUM // (###)

// This provides a template implementing the following two external functions:
//     int gasnetc_AMReplyMediumV()
//     int gasnetc_AMReplyMediumM()
// See comments preceding gasnetc_AMRequestMedium() for more information.

GASNETI_INLINE(gasnetc_AMReplyMedium)
int gasnetc_AMReplyMedium(  gex_Token_t token, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  int retval;

  if_pt (gasnetc_token_in_nbrhd(token)) {
    gasneti_leaf_finish(lc_opt); // synchronous LC
    retval = gasnetc_nbrhd_ReplyGeneric( gasneti_Medium, token, handler,
                                         source_addr, nbytes, 0,
                                         flags, numargs, argptr);
  } else {
    GASNET_BEGIN_FUNCTION(); // TODO: could embed threadinfo in token
    gex_Rank_t jobrank = gasnetc_msgsource(token);
    gasneti_leaf_finish(lc_opt); // TODO-EX: should support async local completion
    retval = gasnetc_AM_ReqRepGeneric(GASNETC_UCX_AM_MEDIUM, jobrank, handler,
                                      NULL, flags, 0, numargs, argptr, source_addr,
                                      nbytes, NULL GASNETI_THREAD_PASS);
  }

  return retval;
}

extern int gasnetc_AMReplyMediumV(
                            gex_Token_t token, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  return gasnetc_AMReplyMedium(token,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr);
}

extern int gasnetc_AMReplyMediumM( 
                            gex_Token_t token,     /* token provided on handler entry */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            void *source_addr, size_t nbytes,   /* data payload */
                            gex_Event_t *lc_opt,       /* local completion of payload */
                            gex_Flags_t flags,
                            int numargs, ...) {
  GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,lc_opt,flags,numargs);

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMReplyMedium(token,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr);
  va_end(argptr);
  return retval;
}

#else // GASNETC_HAVE_NP_REP_MEDIUM

// This provides a template implementing the following three external functions:
//     int gasnetc_AMReplyMediumM()
//     int gasnetc_AM_PrepareReplyMedium()
//     void gasnetc_AM_CommitReplyMediumM()
// See comments with GASNETC_HAVE_NP_REQ_MEDIUM for more information.

GASNETI_INLINE(gasnetc_prepare_rep_medium)
int gasnetc_prepare_rep_medium(
                       gasneti_AM_SrcDesc_t    sd,
                       const int               isFixed,
                       gex_Token_t             token,
                       const void             *client_buf,
                       size_t                  least_payload,
                       size_t                  most_payload,
                       gex_Event_t            *lc_opt,
                       gex_Flags_t             flags,
                       unsigned int            nargs
                       GASNETI_THREAD_FARG)
{
  // (###) Add code here to allocate resources (possibly unsuccessfully, if
  // flags includes GEX_FLAG_IMMEDIATE) for a Medium Reply, storing arguments
  // in either the conduit-specific header or in the 'sd'.

  // No provisions are needed for communication within the neighborhood,
  // since this function is only called in the non-nbrhd case.

  // In this example:
  // + isFixed is 0 when called from gasnetc_AM_PrepareReplyMedium, and
  //   all other arguments are passed unchanged
  // + isFixed is 1 when called from gasnetc_AMReplyMediumM, and
  //   - client_buf is the source_buf argument to gasnetc_AMReplyMediumM
  //   - least_payload is unused
  //   - most_payload is the nbytes argument to gasnetc_AMReplyMediumM

  return 0; // or 1 if GEX_FLAG_IMMEDIATE passed and resource(s) unavailable
}

GASNETI_INLINE(gasnetc_commit_rep_medium)
void gasnetc_commit_rep_medium(
                       gasneti_AM_SrcDesc_t    sd,
                       const int               isFixed,
                       gex_AM_Index_t          handler,
                       size_t                  nbytes,
                       va_list                 argptr)
{
  // (###) Add code here to initiate a Medium Reply using the union of the
  // arguments, and the values stored in 'sd' by the preceding call to
  // gasnetc_prepare_req_medium() or in locations referenced by 'sd'.

  // No provisions are needed for communication within the neighborhood,
  // since this function is only called in the non-nbrhd case.
}

extern int gasnetc_AMReplyMediumM(
                    gex_Token_t token,                /* token provided on handler entry */
                    gex_AM_Index_t handler,           /* index into destination ep's handler table */
                    void *source_addr, size_t nbytes, /* data payload */
                    gex_Event_t *lc_opt,              /* local completion of payload */
                    gex_Flags_t flags,
                    int numargs, ...) {
  GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,lc_opt,flags,numargs);

  va_list argptr;
  va_start(argptr, numargs);

  int retval;
  if_pt (gasnetc_token_in_nbrhd(token)) {
    gasneti_leaf_finish(lc_opt); // synchronous LC
    retval = gasnetc_nbrhd_ReplyGeneric(gasneti_Medium, token, handler,
                                        source_addr, nbytes, NULL,
                                        flags, numargs, argptr);
  } else {
    // (###) post threadinfo extracted from token, or call GASNET_BEGIN_FUNCTION() instead:
    //GASNET_POST_THREADINFO(###);

    struct gasneti_AM_SrcDesc the_sd;
    retval = gasnetc_prepare_rep_medium(&the_sd,1,token,source_addr,0,nbytes,
                                        lc_opt,flags,numargs GASNETI_THREAD_PASS);
    if (!retval) {
      gasnetc_commit_rep_medium(&the_sd,1,handler,nbytes,argptr);
    }
  }

  va_end(argptr);
  return retval;
}

extern gex_AM_SrcDesc_t gasnetc_AM_PrepareReplyMedium(
                       gex_Token_t        token,
                       const void        *client_buf,
                       size_t             least_payload,
                       size_t             most_payload,
                       gex_Event_t       *lc_opt,
                       gex_Flags_t        flags
                       unsigned int       nargs)
{
    gasneti_AM_SrcDesc_t sd;
    flags &= ~(GEX_FLAG_AM_PREPARE_LEAST_CLIENT | GEX_FLAG_AM_PREPARE_LEAST_ALLOC);

    if (gasnetc_token_in_nbrhd(token)) {
        sd = gasnetc_nbrhd_PrepareReply(gasneti_Medium, token,
                                         client_buf, least_payload, most_payload,
                                         NULL, lc_opt, flags, nargs);
    } else {
        // (###) post threadinfo extracted from token, or call GASNET_BEGIN_FUNCTION() instead:
        //GASNET_POST_THREADINFO(###);
        sd = gasneti_init_reply_srcdesc(GASNETI_THREAD_PASS_ALONE);
        GASNETI_COMMON_PREP_REP(sd,token,client_buf,least_payload,most_payload,NULL,lc_opt,flags,nargs,Medium);

        int imm;
        imm = gasnetc_prepare_rep_medium(sd,0,token,client_buf,least_payload,most_payload,
                                        lc_opt,flags,nargs GASNETI_THREAD_PASS);
        if (imm) {
            gasneti_reset_srcdesc(sd);
            sd = NULL; // GEX_AM_SRCDESC_NO_OP
        } else {
            gasneti_init_sd_poison(sd);
            sd->_is_nbrhd = 0;
        }
    }

    GASNETI_TRACE_PREP_RETURN(REPLY_MEDIUM, sd);
    return gasneti_export_srcdesc(sd);
}

extern void gasnetc_AM_CommitReplyMediumM(
                       gex_AM_Index_t          handler,
                       size_t                  nbytes,
                     #if GASNET_DEBUG
                       unsigned int            nargs_arg,
                     #endif
                       gex_AM_SrcDesc_t        sd_arg, ...)
{
    gasneti_AM_SrcDesc_t sd = gasneti_import_srcdesc(sd_arg);

    GASNETI_COMMON_COMMIT_REP(sd,handler,nbytes,NULL,nargs_arg,Medium);

    va_list argptr;
    va_start(argptr, sd_arg);
    if (sd->_is_nbrhd) {
        gasnetc_nbrhd_CommitReply(sd, gasneti_Medium, handler, nbytes, NULL, argptr);
    } else {
        gasnetc_commit_rep_medium(sd,0,handler,nbytes,argptr);
    }
    va_end(argptr);

    gasneti_reset_srcdesc(sd);
}

#endif // GASNETC_HAVE_NP_REP_MEDIUM

GASNETI_INLINE(gasnetc_AMReplyLong)
int gasnetc_AMReplyLong(    gex_Token_t token, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes, void *dest_addr,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  int retval;

  if_pt (gasnetc_token_in_nbrhd(token)) {
    gasneti_leaf_finish(lc_opt); // synchronous LC
    retval = gasnetc_nbrhd_ReplyGeneric( gasneti_Long, token, handler,
                                         source_addr, nbytes, dest_addr,
                                         flags, numargs, argptr);
  } else {
    GASNET_BEGIN_FUNCTION(); // TODO: could embed threadinfo in token
    gex_Rank_t jobrank = gasnetc_msgsource(token);
    retval = gasnetc_AM_ReqRepGeneric(GASNETC_UCX_AM_LONG, jobrank, handler,
                                      lc_opt, flags, 0, numargs, argptr,
                                      source_addr, nbytes, dest_addr
                                      GASNETI_THREAD_PASS);
  }

  return retval;
}

extern int gasnetc_AMReplyLongV(
                            gex_Token_t token, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes, void *dest_addr,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  return gasnetc_AMReplyLong(token,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs,argptr);
}

extern int gasnetc_AMReplyLongM( 
                            gex_Token_t token,     /* token provided on handler entry */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            gex_Event_t *lc_opt,       /* local completion of payload */
                            gex_Flags_t flags,
                            int numargs, ...) {
  GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs);

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMReplyLong(token,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs,argptr);
  va_end(argptr);
  return retval;
}

/* ------------------------------------------------------------------------------------ */
/*
  Handler-safe locks
  ==================
*/
#if !GASNETC_NULL_HSL
extern void gasnetc_hsl_init   (gex_HSL_t *hsl) {
  GASNETI_CHECKATTACH();
  gasneti_mutex_init(&(hsl->lock));
}

extern void gasnetc_hsl_destroy(gex_HSL_t *hsl) {
  GASNETI_CHECKATTACH();
  gasneti_mutex_destroy(&(hsl->lock));
}

extern void gasnetc_hsl_lock   (gex_HSL_t *hsl) {
  GASNETI_CHECKATTACH();

  {
    #if GASNETI_STATS_OR_TRACE
      gasneti_tick_t startlock = GASNETI_TICKS_NOW_IFENABLED(L);
    #endif
    #if GASNETC_HSL_SPINLOCK
      if_pf (gasneti_mutex_trylock(&(hsl->lock)) == EBUSY) {
        if (gasneti_wait_mode == GASNET_WAIT_SPIN) {
          while (gasneti_mutex_trylock(&(hsl->lock)) == EBUSY) {
            gasneti_compiler_fence();
            gasneti_spinloop_hint();
          }
        } else {
          gasneti_mutex_lock(&(hsl->lock));
        }
      }
    #else
      gasneti_mutex_lock(&(hsl->lock));
    #endif
    #if GASNETI_STATS_OR_TRACE
      hsl->acquiretime = GASNETI_TICKS_NOW_IFENABLED(L);
      GASNETI_TRACE_EVENT_TIME(L, HSL_LOCK, hsl->acquiretime-startlock);
    #endif
  }
}

extern void gasnetc_hsl_unlock (gex_HSL_t *hsl) {
  GASNETI_CHECKATTACH();

  GASNETI_TRACE_EVENT_TIME(L, HSL_UNLOCK, GASNETI_TICKS_NOW_IFENABLED(L)-hsl->acquiretime);

  gasneti_mutex_unlock(&(hsl->lock));
}

extern int  gasnetc_hsl_trylock(gex_HSL_t *hsl) {
  GASNETI_CHECKATTACH();

  {
    int locked = (gasneti_mutex_trylock(&(hsl->lock)) == 0);

    GASNETI_TRACE_EVENT_VAL(L, HSL_TRYLOCK, locked);
    if (locked) {
      #if GASNETI_STATS_OR_TRACE
        hsl->acquiretime = GASNETI_TICKS_NOW_IFENABLED(L);
      #endif
    }

    return locked ? GASNET_OK : GASNET_ERR_NOT_READY;
  }
}
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Exit handling code
*/
#ifndef GASNETI_HAVE_ATOMIC_CAS
#error "required atomic compare-and-swap is not yet implemented for your CPU/OS/compiler"
#endif

/* gasnetc_exit_reduce_reqh: reduction on exitcode */
/* gasnetc_exit_reduce_reqh: reduction on exitcode */
static void gasnetc_exit_reduce_reqh(gex_Token_t token,
                                   gex_AM_Arg_t arg0,
                                   gex_AM_Arg_t arg1) {
  gasneti_atomic_val_t exitcode = arg0;
  gasneti_atomic_val_t distance = arg1;
  gasneti_atomic_val_t prevcode;

  do {
    prevcode = gasneti_atomic_read(&gasnetc_exit_code, 0);
  } while ((exitcode > prevcode) &&
           !gasneti_atomic_compare_and_swap(&gasnetc_exit_code, prevcode, exitcode, 0));
  if (distance) {
  #if defined(GASNETI_HAVE_ATOMIC_ADD_SUB)
    /* atomic OR via ADD since no bit will be set more than once */
    gasneti_assert(GASNETI_POWEROFTWO(distance));
    gasneti_atomic_add(&gasnetc_exit_dist, distance, GASNETI_ATOMIC_REL);
  #elif defined(GASNETI_HAVE_ATOMIC_CAS)
    /* atomic OR via C-A-S */
    uint32_t old_val;
    do {
      old_val = gasneti_atomic_read(&gasnetc_exit_dist, 0);
    } while (!gasneti_atomic_compare_and_swap(&gasnetc_exit_dist,
                                               old_val, old_val|distance,
                                               GASNETI_ATOMIC_REL));
  #else
    #error "required atomic compare-and-swap is not yet implemented for your CPU/OS/compiler"
  #endif
  } else {
    gasneti_atomic_increment(&gasnetc_exit_reds, GASNETI_ATOMIC_REL);
  }
}

/* ------------------------------------------------------------------------------------ */
/*
  Private Handlers:
  ================
  see mpi-conduit and extended-ref for examples on how to declare AM handlers here
  (for internal conduit use in bootstrapping, job management, etc.)
*/
static gex_AM_Entry_t const gasnetc_handlers[] = {
  #ifdef GASNETC_COMMON_HANDLERS
    GASNETC_COMMON_HANDLERS(),
  #endif

  /* ptr-width independent handlers */
  gasneti_handler_tableentry_no_bits(gasnetc_exit_reduce_reqh,2,REQUEST,SHORT,0),
  gasneti_handler_tableentry_no_bits(gasnetc_exit_role_reqh,0,REQUEST,SHORT,0),
  gasneti_handler_tableentry_no_bits(gasnetc_exit_role_reph,1,REPLY,SHORT,0),
  gasneti_handler_tableentry_no_bits(gasnetc_exit_reqh,1,REQUEST,SHORT,0),
  gasneti_handler_tableentry_no_bits(gasnetc_exit_reph,0,REPLY,SHORT,0),

  /* ptr-width dependent handlers */

  GASNETI_HANDLER_EOT
};

gex_AM_Entry_t const *gasnetc_get_handlertable(void) {
  return gasnetc_handlers;
}

/* ------------------------------------------------------------------------------------ */
