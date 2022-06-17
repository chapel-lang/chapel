/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ucx-conduit/gasnet_core.c $
 * Description: GASNet ucx conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2019-2020, Mellanox Technologies LTD. All rights reserved.
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

GASNETI_IDENT(gasnetc_IdentString_AMMaxMediumDefault,
              "$GASNetAMMaxMediumDefault: " _STRINGIFY(GASNETC_UCX_MAX_MEDIUM_DFLT) " $");

enum {
  GASNETC_EXIT_ROLE_UNKNOWN,
  GASNETC_EXIT_ROLE_LEADER,
  GASNETC_EXIT_ROLE_MEMBER
};

static const char * volatile gasnetc_exit_state = "UNKNOWN STATE";

// NOTE: Please keep GASNETC_EXIT_STATE_MAXLEN fairly "tight" to bound the
// volume of garbage that might get printed in the event of memory corruption.
#define GASNETC_EXIT_STATE_MAXLEN 60

#if GASNET_DEBUG_VERBOSE
  #define GASNETC_TRACE_EXIT_STATE() do {                 \
        fprintf(stderr, "%d> EXIT STATE %s\n",            \
                (int)gasneti_mynode, gasnetc_exit_state); \
        fflush(NULL);                                     \
  } while (0)
#else
  #define GASNETC_TRACE_EXIT_STATE() ((void)0)
#endif

#define GASNETC_EXIT_STATE(st) do {                                      \
        gasneti_static_assert(sizeof(st) <= GASNETC_EXIT_STATE_MAXLEN+1);\
        gasnetc_exit_state = st;                                         \
        GASNETC_TRACE_EXIT_STATE();                                      \
  } while (0)

#define GASNETC_ROOT_NODE 0

int gasnetc_exit_running = 0;		/* boolean used to identify that exit process is started */

static int gasnetc_exit_in_signal = 0;  /* to avoid certain things in signal context */
extern void gasnetc_fatalsignal_callback(int sig) {
  gasnetc_exit_in_signal = 1;
}

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
#define GASNETC_DEFAULT_EXITTIMEOUT_MAX         480.0   // 8 min - extrapolated from Summit data in bug 4360
#define GASNETC_DEFAULT_EXITTIMEOUT_MIN           2.0   // 2 sec
#define GASNETC_DEFAULT_EXITTIMEOUT_FACTOR      0.25    // 1/4 second per process
static double gasnetc_exittimeout = GASNETC_DEFAULT_EXITTIMEOUT_MAX;


gex_AM_Entry_t *gasnetc_handler; // TODO-EX: will be replaced with per-EP tables

gasneti_spawnerfn_t const *gasneti_spawner = NULL;

gasneti_ucx_module_t gasneti_ucx_module;
static char *gasnetc_ucx_addr_array = NULL;

size_t gasnetc_sizeof_segment_t(void) {
  gasnetc_Segment_t segment;
  return sizeof(*segment);
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

  gasneti_static_assert(GASNETC_UCX_HDR_SIZE == sizeof(gasnetc_sreq_hdr_t));
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
  }
  do {
    status = ucp_request_check_status(request);
    ucp_worker_progress(gasneti_ucx_module.ucp_worker);
  } while (status == UCS_INPROGRESS);

  if (UCS_OK != status) {
    gasneti_fatalerror("UCX worker wait failed: %d, %s",
                       UCS_PTR_STATUS(request),
                       ucs_status_string(UCS_PTR_STATUS(request)));
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

static gasnetc_mem_info_t*
gasnetc_segment_register(void *seg_start, size_t segsize)
{
  GASNETI_TRACE_PRINTF(C,("Registering segment [%p, %p)", seg_start, (void*)(segsize + (uintptr_t)seg_start)));

  ucs_status_t status;
  gasnet_ep_info_t * my_ep_info = &gasneti_ucx_module.ep_tbl[gasneti_mynode];
  gasnetc_mem_info_t *mem_info;
  gasneti_list_t mem_info_list;

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

  return mem_info;
}

static int
gasnetc_segment_deregister(gasnetc_Segment_t segment)
{
  GASNETI_TRACE_PRINTF(C,("Deregistering segment [%p, %p)", segment->_addr, segment->_ub));

  gasnet_ep_info_t *my_ep_info = &gasneti_ucx_module.ep_tbl[gasneti_mynode];
  gasnetc_mem_info_t *mem_info = segment->mem_info;

  gasneti_list_rem(&my_ep_info->mem_tbl, mem_info);

  ucs_status_t status;
  status = ucp_mem_unmap(gasneti_ucx_module.ucp_context,
                        mem_info->mem_h);
  if (status != UCS_OK) {
    gasneti_console_message("WARNING", "ucp_mem_unmap failed: %s",
                            ucs_status_string(status));
    return GASNET_ERR_BAD_ARG;
  }

  ucp_rkey_buffer_release(mem_info->buffer);
  ucp_rkey_destroy(mem_info->rkey);

  gasneti_free(mem_info);

  return GASNET_OK;
}

// Exchange client segment info
static int
gasnetc_segment_exchange(gex_TM_t tm, gex_EP_t *eps, size_t num_eps)
{
  // Count/allocate local space
  size_t local_bytes = 0;
  for (gex_Rank_t i = 0; i < num_eps; ++i) {
    gasnetc_Segment_t segment = (gasnetc_Segment_t) gasneti_import_ep(eps[i])->_segment;
    if (!segment || !segment->_size) continue;
    gasnetc_mem_info_t* mem_info = segment->mem_info;
    gasneti_assert(mem_info);
    gasneti_assert(mem_info->bsize < UINT16_MAX);
    local_bytes += sizeof(gex_EP_Location_t)   // (jobrank,ep_index)
                 + sizeof(uint16_t)            // bsize
                 + mem_info->bsize;            // buffer
  }
  void *local = gasneti_malloc(local_bytes);

  // Pack
  size_t offset = 0;
  for (gex_Rank_t i = 0; i < num_eps; ++i) {
    gasnetc_Segment_t segment = (gasnetc_Segment_t) gasneti_import_ep(eps[i])->_segment;
    if (!segment || !segment->_size) continue;
    gasnetc_mem_info_t* mem_info = segment->mem_info;
    gasneti_assert(mem_info);
    uint16_t bsize = mem_info->bsize;
    gex_EP_Location_t loc;
    loc.gex_rank = gasneti_mynode;
    loc.gex_ep_index = gex_EP_QueryIndex(eps[i]);
    gasneti_mem_pack(local, &loc,              sizeof(loc),     0, offset);
    gasneti_mem_pack(local, &bsize,            sizeof(bsize),   0, offset);
    gasneti_mem_pack(local, mem_info->buffer,  bsize,           0, offset);
  }

  // Exchange
  void *global;
  size_t total_bytes = gasneti_blockingRotatedExchangeV(tm, local, local_bytes, &global, NULL);
  gasneti_free(local);

  // Unpack
  offset = 0;
  while (offset < total_bytes) {
    gex_EP_Location_t loc;
    gasneti_mem_unpack(&loc, global, sizeof(loc), 0, offset);

    uint16_t bsize;
    gasneti_mem_unpack(&bsize, global, sizeof(bsize), 0, offset);
    gasneti_assert(bsize);

    char * buffer = (char *)global + offset;
    offset += bsize;

    gex_Rank_t jobrank = loc.gex_rank;
    if (jobrank == gasneti_mynode) continue; // self

    // TODO-EX: when comms merged w/ conduit-indep, this will be avail in the exchanged data
    gasneti_assert(gasneti_seginfo_tbl[loc.gex_ep_index]);
    gasnet_seginfo_t *si = gasneti_seginfo_tbl[loc.gex_ep_index] + jobrank;
    void *addr = si->addr;
    uintptr_t length = si->size;

    gasnet_ep_info_t * ep_info = &gasneti_ucx_module.ep_tbl[jobrank];

    // Multiple calls to Publish must not create duplicate entries for the
    // same endpoint.
    // TODO: Current data structures are not sensitive to endpoint, nor do they
    // keep any reference count.  So, non-trivial work will be required to
    // support an eventual Unpublish API.
    // TODO: thread safety in list traversal?
    {
      gasnetc_mem_info_t* mem_info;
      int found = 0;
      GASNETI_LIST_FOREACH(mem_info, &ep_info->mem_tbl, gasnetc_mem_info_t) {
        found = ((mem_info->addr   == addr  ) &&
                 (mem_info->length == length));
        if (found) break;
      }
      if (found) continue;
    }

    gasnetc_mem_info_t* new_mem_info;
    GASNETI_LIST_ITEM_ALLOC(new_mem_info, gasnetc_mem_info_t, gasnetc_minfo_reset);

    new_mem_info->addr   = addr;
    new_mem_info->length = length;
    new_mem_info->bsize  = 0;
    new_mem_info->buffer = NULL;

    ucp_ep_h ep = GASNETC_UCX_GET_EP(jobrank);
    gasneti_rkey_unpack(ep, buffer, &new_mem_info->rkey);

    gasneti_list_enq(&ep_info->mem_tbl, new_mem_info); // TODO: thread safety?
  }

  gasneti_free(global);

  return 0;
}

// Exchange aux segment info
static int
gasnetc_segment_exchange_aux(gasnetc_mem_info_t* mem_info)
{
  gasneti_assert(mem_info->bsize <= UINT16_MAX);
  uint16_t bsize = mem_info->bsize;

  // identify max rkey size
  // TODO: bootstrap reduction?
  uint16_t rkey_max_size = bsize;
  uint16_t *rkey_sizes = gasneti_calloc(gasneti_nodes, sizeof(rkey_max_size));
  gasneti_bootstrapExchange(&rkey_max_size, sizeof(rkey_max_size), rkey_sizes);
  for (gex_Rank_t i = 0; i < gasneti_nodes; i++) {
    rkey_max_size = MAX(rkey_max_size, rkey_sizes[i]);
  }
  gasneti_free(rkey_sizes);

  // "pack" and exchange the mem_info->buffer
  // local_buf is needed in case mem_info->buffer is shorter than rkey_max_size
  void * recv_buf = gasneti_malloc(rkey_max_size * gasneti_nodes);
  void * local_buf = gasneti_malloc(rkey_max_size);
  memcpy(local_buf, mem_info->buffer, bsize);
  gasneti_bootstrapExchange(local_buf, rkey_max_size, recv_buf);
  gasneti_free(local_buf);

  /* TODO:
  * + When using PSHM we could store rkeys just once per supernode
  * + When not fully connected, we could utilize sparse storage
  */

  char * buffer = recv_buf;
  for (gex_Rank_t jobrank = 0; jobrank < gasneti_nodes; jobrank++, buffer += rkey_max_size) {
    if (jobrank == gasneti_mynode) {
      continue;
    }

    GASNETI_LIST_ITEM_ALLOC(mem_info, gasnetc_mem_info_t, gasnetc_minfo_reset);

    mem_info->addr   = gasneti_seginfo_aux[jobrank].addr;
    mem_info->length = gasneti_seginfo_aux[jobrank].size;
    mem_info->bsize  = 0;
    mem_info->buffer = NULL;

    ucp_ep_h ep = GASNETC_UCX_GET_EP(jobrank);
    gasneti_rkey_unpack(ep, buffer, &mem_info->rkey);

    gasnet_ep_info_t * ep_info = &gasneti_ucx_module.ep_tbl[jobrank];
    gasneti_list_enq(&ep_info->mem_tbl, mem_info); // TODO: thread safety?
  }

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
      // TODO: factor or merge w/ gasnetc_deregister_segment()
      if (gasneti_mynode == i) {
        status = ucp_mem_unmap(gasneti_ucx_module.ucp_context, mem_info->mem_h);
        if (status != UCS_OK) {
          gasneti_fatalerror("Attach segment failed: %s",
                             ucs_status_string(UCS_PTR_STATUS(status)));
        }
        ucp_rkey_buffer_release(mem_info->buffer);
      } else {
        gasneti_assert_ptr(mem_info->buffer ,==, NULL);
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
  gasnetc_send_fini();
  GASNETC_EXIT_STATE("gasnetc_send_fini called");

  /* cleanup UCX */
  gasnetc_connect_shutdown();
  GASNETC_EXIT_STATE("gasnetc_connect_shutdown called");

#if GASNETC_PIN_SEGMENT
  gasnetc_unpin_segment();
#endif
  ucp_worker_destroy(gasneti_ucx_module.ucp_worker);
  GASNETC_EXIT_STATE("ucp_worker_destroy called");
  gasnetc_recv_fini();
  GASNETC_EXIT_STATE("gasnetc_recv_fini called");
  ucp_cleanup(gasneti_ucx_module.ucp_context);
  GASNETC_EXIT_STATE("ucp_cleanup called");

  gasneti_free(gasneti_ucx_module.ep_tbl);

  // If exiting from AM handler context, then we hold the lock.
  // So to avoid errors, we just let it leak.
  // gasneti_mutex_destroy(&gasneti_ucx_module.ucp_worker_lock);
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
  int rc;

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

  // Process GASNET_UCX_MAX_MEDIUM
  // This must be done at least early enough to be used in setup of PSHM
  // Placement here allows for the possibility that UCX init may need the max medium
  { size_t lub_medium =
      gasneti_getenv_int_withdefault("GASNET_UCX_MAX_MEDIUM", GASNETC_UCX_MAX_MEDIUM_DFLT, 1);
    if (lub_medium < 512) {
      gasneti_fatalerror("GASNET_UCX_MAX_MEDIUM setting (%"PRIdSZ") is not valid.  "
                         "The value must be no less than 512.  "
                         "See ucx-conduit README for more details.",
                         lub_medium);
    }
    size_t max_med_overhead = GASNETI_ALIGNUP(GASNETC_UCX_HDR_SIZE + GASNETC_MAX_ARGS_SIZE, 8);
    gasnetc_ammed_bufsz = lub_medium + max_med_overhead;
  }

  /* Ensure uniform UCX_* env vars */
  gasneti_propagate_env("UCX_", GASNETI_PROPAGATE_ENV_PREFIX);

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
#ifdef GASNETC_UCX_THREADS
  gasneti_ucx_module.lock_cnt = 0;
  gasneti_ucx_module.lock_tid = GASNETE_INVALID_THREADIDX;
#endif

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
    gasneti_Client_t client = gasneti_alloc_client(clientName, flags);
    *client_p = gasneti_export_client(client);

    //  create the initial endpoint with internal handlers
    if (gex_EP_Create(ep_p, *client_p, GEX_EP_CAPABILITY_ALL, flags))
      GASNETI_RETURN_ERRR(RESOURCE,"Error creating initial endpoint");
    gasneti_EP_t ep = gasneti_import_ep(*ep_p);
    gasnetc_handler = ep->_amtbl; // TODO-EX: this global variable to be removed

    gasneti_TM_t tm = gasneti_alloc_tm(ep, gasneti_mynode, gasneti_nodes, flags);
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
  gasnetc_mem_info_t *mem_info = gasnetc_segment_register(auxbase, auxsize);
  gasnetc_segment_exchange_aux(mem_info);
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

  if (GASNET_OK != (rc = gasnetc_recv_init())) {
    return rc;
  }
  gasnetc_send_init();

  return GASNET_OK;
}

static void gasnetc_defaultSignalHandler(int sig) {
  gasneti_atomic_set(&gasnetc_exit_atsighandler, 1, 0);
  gasneti_defaultSignalHandler(sig);
}

/* ------------------------------------------------------------------------------------ */
extern int gasnetc_attach_primary(void) {
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

  /* (###) Optionally (but recommended) free spawner's idle resources.
   * Safe even if spawner collectives are used after attach
   */
  gasneti_spawner->Cleanup();

  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
int gasnetc_segment_create_hook(gex_Segment_t e_segment)
{
#if GASNETC_PIN_SEGMENT
  // Register the segment
  gasnetc_Segment_t segment = (gasnetc_Segment_t) gasneti_import_segment(e_segment);
  segment->mem_info = gasnetc_segment_register(segment->_addr, segment->_size);
  if (! segment->mem_info) {
    // TODO: non-fatal error handling:
    // Once gasnetc_segment_register() can return NULL on error, either it or
    // this path must cleanup any conduit-specific state prior to error return.
    // However, currently all failure modes in gasnetc_segment_register() call
    // gasneti_fatalerror().
    return GASNET_ERR_BAD_ARG;
  }
#endif
  return GASNET_OK;
}

void gasnetc_segment_destroy_hook(gasneti_Segment_t i_segment)
{
#if GASNETC_PIN_SEGMENT
  gasneti_assert_zeroret( gasnetc_segment_deregister((gasnetc_Segment_t) i_segment) );
#endif
}

int gasnetc_segment_attach_hook(gex_Segment_t e_segment, gex_TM_t e_tm)
{
  int rc = gasnetc_segment_create_hook(e_segment);
  if (rc) return rc;

#if GASNETC_PIN_SEGMENT
  // Exchange the RKeys
  gex_EP_t e_ep = gex_TM_QueryEP(e_tm);
  gasnetc_segment_exchange(e_tm, &e_ep, 1);
#endif

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
    gasneti_Client_t client = gasneti_alloc_client(clientName, flags);
    *client_p = gasneti_export_client(client);
    //  create the initial endpoint with internal handlers
    if (gex_EP_Create(ep_p, *client_p, GEX_EP_CAPABILITY_ALL, flags))
      GASNETI_RETURN_ERRR(RESOURCE,"Error creating initial endpoint");
  }
  gasneti_EP_t ep = gasneti_import_ep(*ep_p);

  // Do NOT move this prior to the gasneti_trace_init() call
  GASNETI_TRACE_PRINTF(O,("gex_Client_Init: name='%s' argc_p=%p argv_p=%p flags=%d",
                          clientName, (void *)argc, (void *)argv, flags));

  // TODO-EX: create team
  gasneti_TM_t tm = gasneti_init_done
                    ? gasneti_import_tm(gasnetc_bootstrap_tm) // gasnetc_init() creates very first TM
                    : gasneti_alloc_tm(ep, gasneti_mynode, gasneti_nodes, flags);
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

extern int gasnetc_ep_publishboundsegment_hook(
                gex_TM_t               tm,
                gex_EP_t               *eps,
                size_t                 num_eps,
                gex_Flags_t            flags)
{
#if GASNETC_PIN_SEGMENT
  // Conduit-dependent parts
  // TODO: merge comms into gasneti_EP_PublishBoundSegment().
  gasnetc_segment_exchange(tm, eps, num_eps);
#endif

  // Avoid race in which AMRequestLong triggers AMRepyLong before exchange completes remotely
  // TODO: barrier for multi-tm per-process
  gex_Event_Wait(gex_Coll_BarrierNB(tm, 0));

  return GASNET_OK;
}

// Conduit-specififc hook to run at end of gex_EP_Create()
int gasnetc_ep_init_hook(gasneti_EP_t i_ep)
{
  // Conduit-specific validation
  if (i_ep->_index) {
    // Current non-primordial EP support is RMA-only
    if (i_ep->_caps & ~GEX_EP_CAPABILITY_RMA) {
      // Unsupported capability/ies requested
      GASNETI_RETURN_ERRR(BAD_ARG,
                          "ucx-conduit supports only GEX_EP_CAPABILITY_RMA for non-primordial endpoints");
    }
  }

  return GASNET_OK;
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

  const char * state = gasnetc_exit_state;
  size_t state_len = gasneti_strnlen(state, GASNETC_EXIT_STATE_MAXLEN);

  /* note - can't call trace macros here, or even sprintf */
  if (sig == SIGALRM) {
    static const char msg[] = "gasnet_exit(): WARNING: timeout during exit... goodbye.  [";
    (void) write(STDERR_FILENO, msg, sizeof(msg) - 1);
    (void) write(STDERR_FILENO, state, state_len);
    (void) write(STDERR_FILENO, "]\n", 2);
  } else {
    static const char msg1[] = "gasnet_exit(): ERROR: signal ";
    static const char msg2[] = " received during exit... goodbye.  [";
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
    (void) write(STDERR_FILENO, state, state_len);
    (void) write(STDERR_FILENO, "]\n", 2);
  }
  (void) fsync(STDERR_FILENO);

  if (gasneti_atomic_decrement_and_test(&once, 0)) {
    /* We ask the bootstrap support to kill us, but only once */
    GASNETC_EXIT_STATE("in suicide timer");
    gasneti_reghandler(SIGALRM, gasnetc_exit_sighandler);
    gasneti_unblocksig(SIGALRM);
    alarm(MAX(5,gasnetc_exittimeout));
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
  GASNET_BEGIN_FUNCTION();
  int rc = 0;

  GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  if (!gasnetc_exit_running) {
    gasneti_atomic_set(&gasnetc_exit_code, exitcode, GASNETI_ATOMIC_WMB_POST);
    gasnetc_exit_running = 1;
#ifdef GASNETC_UCX_THREADS
    gasnetc_exit_thread = GASNETC_MY_THREADIDX;
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
  GASNET_BEGIN_FUNCTION();
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
  GASNET_BEGIN_FUNCTION();
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
      gasnetc_poll_sndrcv(GASNETC_LOCK_REGULAR GASNETI_THREAD_PASS);
      if (gasneti_atomic_read(&gasnetc_exit_reqs, 0)) return -1;
    } while (!(distance & gasneti_atomic_read(&gasnetc_exit_dist, 0)));
    exitcode = gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE);
  }

  return 0;
}

static void gasnetc_exit_role_reqh(gex_Token_t token) {
  gex_Rank_t src = gasnetc_msgsource(token);
  int local_role, result;

  /* What role would the local node get if the requester is made the leader? */
  local_role = (src == gasneti_mynode) ? GASNETC_EXIT_ROLE_LEADER : GASNETC_EXIT_ROLE_MEMBER;


  /* Try atomically to assume the proper role.  Result determines role of requester */
  result = gasneti_atomic_compare_and_swap(&gasnetc_exit_role, GASNETC_EXIT_ROLE_UNKNOWN, local_role, 0)
                ? GASNETC_EXIT_ROLE_LEADER : GASNETC_EXIT_ROLE_MEMBER;

  /* Inform the requester of the outcome. */
  GASNETI_SAFE(gasnetc_ReplySysShort(token, NULL, gasneti_handleridx(gasnetc_exit_role_reph),
           1, (gex_AM_Arg_t)result));

}

static void gasnetc_exit_role_reph(gex_Token_t token, gex_AM_Arg_t arg0) {

  int role = (int)arg0;

  gasneti_assert((role == GASNETC_EXIT_ROLE_LEADER) || (role == GASNETC_EXIT_ROLE_MEMBER));

  /* Set the role if not yet set.  Then assert that the assigned role has been assumed.
   * This way the assertion is checking that if the role was obtained by other means
   * (namely by receiving an exit request) it must match the election result. */
  gasneti_atomic_compare_and_swap(&gasnetc_exit_role, GASNETC_EXIT_ROLE_UNKNOWN, role, 0);
  gasneti_assert(gasneti_atomic_read(&gasnetc_exit_role, 0) == role);
}

/*
 * gasnetc_get_exit_role()
 *
 * This function returns the exit role immediately if known.  Otherwise it sends an AMRequest
 * to determine its role and then polls the network until the exit role is determined, either
 * by the reply to that request, or by a remote exit request.
 *
 * Includes a timeout to bound how long to poll for a reply, and the return value will
 * be GASNETC_EXIT_ROLE_UNKNOWN if it expires.
 * However, should still be called with an alarm timer in-force in case we get hung sending.
 *
 * Note that if we get here as a result of a remote exit request then our role has already been
 * set to "member" and we won't touch the network from inside the request handler.
 *
 * TODO: can/should an UCX-level atomic CAS replace the AM round-trip?
 */
static int gasnetc_get_exit_role(int64_t timeout_us)
{
  GASNET_BEGIN_FUNCTION();
  int role = gasneti_atomic_read(&gasnetc_exit_role, 0);

  gasneti_assert(timeout_us > 0);

  if (role == GASNETC_EXIT_ROLE_UNKNOWN) {
    gasneti_tick_t start_time = gasneti_ticks_now();

    // Don't know our role yet.  So, send an AM Request to determine our role
    GASNETI_SAFE(gasnetc_RequestSysShort(GASNETC_ROOT_NODE, NULL,
                                         gasneti_handleridx(gasnetc_exit_role_reqh), 0));

    // Now spin (w/ time limit) until somebody tells us our role
    do {
      gasnetc_poll_sndrcv(GASNETC_LOCK_REGULAR GASNETI_THREAD_PASS);
      role = gasneti_atomic_read(&gasnetc_exit_role, 0);
    } while ((role == GASNETC_EXIT_ROLE_UNKNOWN) &&
             (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 < timeout_us));
  }

  return role;
}

static int gasnetc_exit_leader(int exitcode, int64_t timeout_us) {
  GASNET_BEGIN_FUNCTION();
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
    gasnetc_poll_sndrcv(GASNETC_LOCK_REGULAR GASNETI_THREAD_PASS);
  }

  return 0;
}

/* gasnetc_exit_member
 *
 * We wait for a polite goodbye from the exit leader.
 *
 * Takes a timeout in us as an argument
 *
 * Returns 0 on success, non-zero on timeout.
 */
static int gasnetc_exit_member(int64_t timeout_us) {
  GASNET_BEGIN_FUNCTION(); // OK - not a critical-path
  gasneti_tick_t start_time;

  gasneti_assert(timeout_us > 0);

  start_time = gasneti_ticks_now();

  /* wait until the exit request is received from the leader */
  while (gasneti_atomic_read(&gasnetc_exit_reqs, 0) == 0) {
    if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) {
      return -1;
    }
    gasnetc_poll_sndrcv(GASNETC_LOCK_REGULAR GASNETI_THREAD_PASS);
  }

  /* wait until our reply has been placed on the wire */
  gasneti_sync_reads(); /* For non-atomic portion of gasnetc_exit_repl_oust */
  gasnetc_counter_wait(&gasnetc_exit_repl_oust, 1 GASNETI_THREAD_PASS);

  return 0;
}

static void gasnetc_exit_body(void) {
  GASNET_BEGIN_FUNCTION();
  int exitcode;
  int graceful = 0;
  int64_t timeout_us = gasnetc_exittimeout * 1.0e6;
  unsigned int timeout = (unsigned int)gasnetc_exittimeout;
  int role;
#if GASNET_DEBUG && GASNET_PAR
  {
    GASNETC_LOCK_UCX();
    gasneti_assert(GASNETC_MY_THREADIDX == gasnetc_exit_thread);
    GASNETC_UNLOCK_UCX();
  }
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

  // prevent possible GASNETI_CHECK_INJECT() failures when we communicate
  GASNETI_CHECK_INJECT_RESET();

  GASNETI_TRACE_PRINTF(C,("gasnet_exit(%i)\n", exitcode));

  /* Timed MAX(exitcode) reduction to clearly distinguish collective exit */
  alarm(2 + timeout); // +2 is margin of safety around the timed reduction
  graceful = (gasnetc_exit_reduce(exitcode, timeout_us) == 0);

  // A second alarm timer for most of the remaining exit steps
  // TODO: 120 is arbitrary and hard-coded
  alarm(MAX(120, timeout));

  gasnetc_sys_coll_fini();

  // Try to flush out all the output
  GASNETC_EXIT_STATE("flushing output");
  {
    gasneti_flush_streams();
    gasneti_trace_finish();
    gasneti_sched_yield();
  }

  if (!graceful) {
    // Timed reduction failed. So make a second attempt at a coordinated shutdown.
    // This has two global communication steps each with their own timeout interval
    exitcode = gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE);

    GASNETC_EXIT_STATE("performing non-collective exit");
    unsigned int prev_timeout = alarm(2 + timeout);
    role = gasnetc_get_exit_role(timeout_us);

    GASNETC_EXIT_STATE("coordinating shutdown");
    alarm(2 + timeout); // yet another alarm interval for the second comms step
    switch (role) {
    case GASNETC_EXIT_ROLE_LEADER:
      /* send all the remote exit requests and wait for the replies */
      graceful = (gasnetc_exit_leader(exitcode, timeout_us) == 0);
      break;
    case GASNETC_EXIT_ROLE_MEMBER:
      graceful = (gasnetc_exit_member(timeout_us) == 0);
      break;
    default:
      gasneti_assume(! graceful);
    }
    alarm(prev_timeout); // resume previous alarm
  }

  // Note we skip cleanly shutdown on non-collective exit or exit via signal
  if (graceful && !gasnetc_exit_in_signal) {
    GASNETC_EXIT_STATE("flushing ucx requests: waiting for sends completions");
    gasnetc_send_list_wait(GASNETC_LOCK_REGULAR GASNETI_THREAD_PASS);
    gasneti_bootstrapBarrier();
    GASNETC_EXIT_STATE("flushing ucx requests: waiting for recvs completions");
    while(gasnetc_poll_sndrcv(GASNETC_LOCK_REGULAR GASNETI_THREAD_PASS));
  }

  // One last alarm to cover the Fini or Abort
  // This has been observed to be the slowest step in some cases (see bug 4360)
  // TODO: 30 is arbitrary and hard-coded
  alarm(MAX(30, timeout));
  if (graceful) {
    GASNETC_EXIT_STATE("in gasneti_bootstrapFini()");
    gasneti_bootstrapFini();
    // TODO: this may belong earlier, but placement earlier leads to errors
    GASNETC_EXIT_STATE("ucx finalization");
    gasnetc_ucx_fini();
  } else {
    GASNETC_EXIT_STATE("in gasneti_bootstrapAbort()");
    gasneti_bootstrapAbort(exitcode);
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
  /* The leader will send this AM, but should _never_ receive it */
  gasneti_assert(gasneti_atomic_read(&gasnetc_exit_role, 0) != GASNETC_EXIT_ROLE_LEADER);

  /* We should never receive this AM multiple times */
  gasneti_assert(gasneti_atomic_read(&gasnetc_exit_reqs, 0) == 0);

  /* If we didn't already know, we are now certain our role is "member" */
  (void)gasneti_atomic_compare_and_swap(&gasnetc_exit_role, GASNETC_EXIT_ROLE_UNKNOWN, GASNETC_EXIT_ROLE_MEMBER, 0);

  /* Send a reply so the leader knows we are reachable */
  GASNETI_SAFE(gasnetc_ReplySysShort(token, &gasnetc_exit_repl_oust,
           gasneti_handleridx(gasnetc_exit_reph), /* no args */ 0));
  gasneti_sync_writes(); /* For non-atomic portion of gasnetc_exit_repl_oust */

  /* Count the exit requests, so gasnetc_exit_member() knows when to return */
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
  gasnetc_poll_sndrcv(GASNETC_LOCK_REGULAR GASNETI_THREAD_PASS);
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
  gasnetc_poll_sndrcv(GASNETC_LOCK_REGULAR GASNETI_THREAD_PASS);

  va_start(argptr, numargs);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    retval = gasnetc_nbrhd_RequestGeneric ( gasneti_Short, jobrank, handler,
                                            NULL, 0, NULL,
                                            0, numargs, argptr GASNETI_THREAD_PASS);
  } else {
    gasnetc_cbfunc_t      cbfunc    = counter ? gasnetc_cb_counter  : NULL;
    gasnetc_atomic_val_t *local_cnt = counter ? &counter->initiated : NULL;
    retval = gasnetc_am_reqrep_inner(GASNETC_UCX_AM_SHORT, jobrank, handler, 0,
                                     1, 0, numargs, argptr, NULL, 0, NULL,
                                     local_cnt, cbfunc
                                     GASNETI_THREAD_PASS);
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
  } else {
    gex_Rank_t jobrank = gasnetc_msgsource(token);
    gasnetc_cbfunc_t      cbfunc    = counter ? gasnetc_cb_counter  : NULL;
    gasnetc_atomic_val_t *local_cnt = counter ? &counter->initiated : NULL;
    retval = gasnetc_am_reqrep_inner(GASNETC_UCX_AM_SHORT, jobrank, handler, 0,
                                     0, 0, numargs, argptr, NULL, 0, NULL,
                                     local_cnt, cbfunc
                                     GASNETI_THREAD_PASS);
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
  GASNETC_IMMEDIATE_MAYBE_POLL(flags); /* (###) poll at least once, to assure forward progress */

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMRequestShort(tm,rank,handler,flags,numargs,argptr GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
}

#if !GASNET_NATIVE_NP_ALLOC_REQ_MEDIUM // (###)

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
  GASNETC_IMMEDIATE_MAYBE_POLL(flags); /* (###) poll at least once, to assure forward progress */

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMRequestMedium(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
}

#else // GASNET_NATIVE_NP_ALLOC_REQ_MEDIUM

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
//    #define GASNET_NATIVE_NP_ALLOC_REQ_MEDIUM 1
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
    GASNETI_TRACE_PREP_REQUESTMEDIUM(tm,rank,client_buf,least_payload,most_payload,flags,nargs);

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
    GASNETI_CHECK_SD(client_buf, least_payload, most_payload, sd);
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
#endif // GASNET_NATIVE_NP_ALLOC_REQ_MEDIUM

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
  GASNETC_IMMEDIATE_MAYBE_POLL(flags); /* (###) poll at least once, to assure forward progress */

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

#if !GASNET_NATIVE_NP_ALLOC_REP_MEDIUM // (###)

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

#else // GASNET_NATIVE_NP_ALLOC_REP_MEDIUM

// This provides a template implementing the following three external functions:
//     int gasnetc_AMReplyMediumM()
//     int gasnetc_AM_PrepareReplyMedium()
//     void gasnetc_AM_CommitReplyMediumM()
// See comments with GASNET_NATIVE_NP_ALLOC_REQ_MEDIUM for more information.

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
    GASNETI_TRACE_PREP_REPLYMEDIUM(token,client_buf,least_payload,most_payload,flags,nargs);

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
    GASNETI_CHECK_SD(client_buf, least_payload, most_payload, sd);
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

#endif // GASNET_NATIVE_NP_ALLOC_REP_MEDIUM

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
  GASNETC_COMMON_HANDLERS(),

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
