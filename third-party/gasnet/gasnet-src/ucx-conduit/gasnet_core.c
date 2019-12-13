/*   $Source: bitbucket.org:berkeleylab/gasnet.git/template-conduit/gasnet_core.c $
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

gex_AM_Entry_t const *gasnetc_get_handlertable(void);

gex_AM_Entry_t *gasnetc_handler; // TODO-EX: will be replaced with per-EP tables

gasneti_spawnerfn_t const *gasneti_spawner = NULL;

gasnet_ucx_module_t gasnet_ucx_module;

size_t gasnetc_AMHeaderSize(void)
{
  return sizeof(gasnetc_sreq_hdr_t);
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
  gasneti_assert(! gasnetc_token_in_nbrhd(token));
  gasneti_assert(token);
  gasnetc_sreq_hdr_t *hdr = (gasnetc_sreq_hdr_t*)token;
  gasneti_assert(hdr->src < gasneti_nodes);
  return hdr->src;
}

static int gasnetc_connect_static(void)
{
  int i;
  ucs_status_t status;
  ucp_ep_params_t ep_params;


  for (i = 0; i < gasneti_nodes; ++i) {
    ep_params.field_mask = UCP_EP_PARAM_FIELD_REMOTE_ADDRESS;
    ep_params.address    = (ucp_address_t*)gasnet_ucx_module.remote_ep_tbl[i].ucx_addr;
    status = ucp_ep_create(gasnet_ucx_module.ucp_worker, &ep_params,
                           &gasnet_ucx_module.remote_ep_tbl[i].server_ep);
    if (UCS_OK != status) {
      return GASNET_ERR_NOT_INIT;
    }
  }
  return GASNET_OK;
}

static void gasnetc_connect_shutdown(void)
{
  for (int i = 0; i < gasneti_nodes; ++i) {
    ucp_ep_destroy(gasnet_ucx_module.remote_ep_tbl[i].server_ep);
  }
}

void gasnetc_ucx_empty_complete_cb(void *req, ucs_status_t status)
{
}

static int gasnetc_ucx_worker_flush(void)
{
  ucs_status_t status;
  ucs_status_ptr_t request;

  request = ucp_worker_flush_nb(gasnet_ucx_module.ucp_worker, 0,
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
    ucp_worker_progress(gasnet_ucx_module.ucp_worker);
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

static void gasnetc_fini(void)
{
  gasnetc_ucx_worker_flush();
  gasneti_bootstrapFini();
  gasneti_nodemapFini();
  gasnetc_sreq_list_free();
  gasnetc_am_req_pool_free();

  /* cleanup UCX */
  gasnetc_connect_shutdown();
  ucp_worker_destroy(gasnet_ucx_module.ucp_worker);
  gasnetc_rreq_list_free();
  gasnetc_buffer_pool_free();
  ucp_cleanup(gasnet_ucx_module.ucp_context);

  gasneti_free(gasnet_ucx_module.remote_ep_tbl);
  gasneti_mutex_destroy(&gasnet_ucx_module.ucp_worker_lock);
}

static int gasnetc_init(int *argc, char ***argv, gex_Flags_t flags) {
  ucp_config_t *config;
  ucs_status_t status;
  ucp_params_t ucp_params;
  ucp_worker_params_t worker_params;
  gasnet_ucx_ep_conn_info_t local_ep;
  ucp_address_t *ucx_local_addr;

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

  /* Must init timers after global env, and preferably before tracing */
  GASNETI_TICKS_INIT();

  #if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_init(): spawn successful - node %i/%i starting...\n", 
      gasneti_mynode, gasneti_nodes); fflush(stderr);
  #endif

  /*
   * Initialize UCX
   */
  status = ucp_config_read("GASNET", NULL, &config);
  if (status != UCS_OK) {
    GASNETI_RETURN_ERRFR(RESOURCE, "Fail to read UCX config: %s",
                         ucs_status_string(status));
  }
  ucp_params.features        = UCP_FEATURE_TAG;
  ucp_params.request_size    = sizeof(gasnetc_ucx_request_t);
  ucp_params.request_init    = gasnetc_req_init;
  ucp_params.request_cleanup = NULL;
  ucp_params.field_mask      = UCP_PARAM_FIELD_FEATURES |
                               UCP_PARAM_FIELD_REQUEST_SIZE |
                               UCP_PARAM_FIELD_REQUEST_INIT |
                               UCP_PARAM_FIELD_REQUEST_CLEANUP;
  status = ucp_init(&ucp_params, config, &gasnet_ucx_module.ucp_context);
  ucp_config_release(config);
  if (UCS_OK != status) {
    return GASNET_ERR_NOT_INIT;
  }

  gasneti_mutex_init(&gasnet_ucx_module.ucp_worker_lock);

  worker_params.field_mask  = UCP_WORKER_PARAM_FIELD_THREAD_MODE;
#ifdef GASNETC_UCX_THREADS
  worker_params.thread_mode = UCS_THREAD_MODE_MULTI;
#else
  worker_params.thread_mode = UCS_THREAD_MODE_SINGLE;
#endif

  status = ucp_worker_create(gasnet_ucx_module.ucp_context, &worker_params,
                             &gasnet_ucx_module.ucp_worker);
  if (UCS_OK != status) {
    return GASNET_ERR_NOT_INIT;
  }

  status = ucp_worker_get_address(gasnet_ucx_module.ucp_worker,
                                  &ucx_local_addr, &local_ep.ucx_addr_len);

  // TODO-next: support any size of EPs
  gasneti_assert(local_ep.ucx_addr_len <= GASNETC_UCX_MAX_ADDR_LEN);

  memcpy(local_ep.ucx_addr, ucx_local_addr, local_ep.ucx_addr_len);
  ucp_worker_release_address(gasnet_ucx_module.ucp_worker, ucx_local_addr);

  gasnetc_am_req_pool_alloc();
  gasnetc_buffer_pool_alloc();
  gasnetc_req_list_init();

  gasnet_ucx_module.remote_ep_tbl =
      gasneti_calloc(gasneti_nodes, sizeof(local_ep));

  /* TODO-next: perform two-stage endpoint exchange:
   * 1 exchange EP sizes, get max ep-size
   * 2 use max ep size to exchange */
  gasneti_bootstrapExchange(&local_ep, sizeof(local_ep),
                            gasnet_ucx_module.remote_ep_tbl);

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

  /* allocate and attach an aux segment */

  gasneti_auxsegAttach((uintptr_t)-1, gasneti_bootstrapExchange);

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
  gasneti_init_done = 1;

  if (0 == gasneti_mynode) {
    fflush(NULL);
    fprintf(stderr,
      " WARNING: ucx-conduit is experimental and should not be used for\n"
      "          performance measurements.\n"
      "          Please see `ucx-conduit/README` for more details.\n");
    fflush(NULL);
  }

  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
static int gasnetc_attach_primary(void) {
  /* ------------------------------------------------------------------------------------ */
  /*  register fatal signal handlers */

  /* catch fatal signals and convert to SIGQUIT */
  gasneti_registerSignalHandlers(gasneti_defaultSignalHandler);

  /*  (###) register any custom signal handlers required by your conduit 
   *        (e.g. to support interrupt-based messaging)
   */

  // register process exit-time hook
  gasneti_registerExitHandler(gasnetc_exit);

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
  // TODO-EX: crude detection of multiple calls until we support them
  gasneti_assert(NULL == gasneti_seginfo[0].addr);

  /* ------------------------------------------------------------------------------------ */
  /*  register segment  */

  gasneti_segmentAttach(segsize, gasneti_seginfo, exchangefn, flags);

  void *segbase = gasneti_seginfo[gasneti_mynode].addr;
  segsize = gasneti_seginfo[gasneti_mynode].size;

  gasneti_assert_uint(((uintptr_t)segbase) % GASNET_PAGESIZE ,==, 0);
  gasneti_assert_uint(segsize % GASNET_PAGESIZE ,==, 0);

  gasneti_EP_t ep = gasneti_import_tm(tm)->_ep;
  ep->_segment = gasneti_alloc_segment(ep->_client, segbase, segsize, flags, 0);
  gasneti_legacy_segment_attach_hook(ep);
  *segment_p = gasneti_export_segment(ep->_segment);

  /* After local segment is attached, call optional client-provided hook
     (###) should call BEFORE any conduit-specific pinning/registration of the segment
   */
  if (gasnet_client_attach_hook) {
    gasnet_client_attach_hook(segbase, segsize);
  }

  /* ------------------------------------------------------------------------------------ */
  /*  gather segment information */

  /* (###) add code here to gather the segment assignment info into 
           gasneti_seginfo on each node (may be possible to use AMShortRequest here)
           If gasneti_segmentAttach() was used above, this is already done.
   */

  gasneti_assert(gasneti_seginfo[gasneti_mynode].addr == segbase &&
                 gasneti_seginfo[gasneti_mynode].size == segsize);

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

  #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    /*  register client segment  */
    gex_Segment_t seg; // g2ex segment is automatically saved by a hook
    /*  (###) may replace gasneti_defaultExchange with a conduit-specific exchange if available */
    if (GASNET_OK != gasnetc_attach_segment(&seg, _tm, segsize, gasneti_defaultExchange, GASNETI_FLAG_INIT_LEGACY))

      GASNETI_RETURN_ERRR(RESOURCE,"Error attaching segment");
  #endif

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
    int retval = gasnetc_init(argc, argv, flags);
    if (retval != GASNET_OK) GASNETI_RETURN(retval);
    gasneti_trace_init(argc, argv);
  }

  //  allocate the client object
  gasneti_Client_t client = gasneti_alloc_client(clientName, flags, 0);
  *client_p = gasneti_export_client(client);

  //  create the initial endpoint with internal handlers
  if (gasnetc_EP_Create(ep_p, *client_p, flags))
    GASNETI_RETURN_ERRR(RESOURCE,"Error creating initial endpoint");
  gasneti_EP_t ep = gasneti_import_ep(*ep_p);
  gasnetc_handler = ep->_amtbl; // TODO-EX: this global variable to be removed

  // TODO-EX: create team
  gasneti_TM_t tm = gasneti_alloc_tm(ep, gasneti_mynode, gasneti_nodes, flags, 0);
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

extern void gasnetc_exit(int exitcode) {
  /* waiting to completion all requests */
  gasnetc_req_wait(GASNETC_LOCK_MODE_REGULAR);

  /* once we start a shutdown, ignore all future SIGQUIT signals or we risk reentrancy */
  gasneti_reghandler(SIGQUIT, SIG_IGN);

  {  /* ensure only one thread ever continues past this point */
    static gasneti_mutex_t exit_lock = GASNETI_MUTEX_INITIALIZER;
    gasneti_mutex_lock(&exit_lock);
  }

  GASNETI_TRACE_PRINTF(C,("gasnet_exit(%i)\n", exitcode));

  gasneti_flush_streams();
  gasneti_trace_finish();
  gasneti_sched_yield();

  /* TODO-next: implement distributed graceful exit */

  /* (###) add code here to terminate the job across _all_ nodes 
           with gasneti_killmyprocess(exitcode) (not regular exit()), preferably
           after raising a SIGQUIT to inform the client of the exit
           Should include a call to gasneti_spawner->Fini() on normal exit
           or gasneti_spawner->Abort() for an abortive exit
  */
  gasnetc_fini();

  gasneti_killmyprocess(exitcode); /* last chance */
  gasneti_fatalerror("gasnetc_exit failed!");
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
  gasneti_AMPSHMPoll(0);
#endif
  /* protected progress of UCX */
  gasnetc_req_poll(GASNETC_LOCK_MODE_REGULAR);
  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
*/

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
                                      flags, 1, numargs,
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
                                      flags, 1, numargs,
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
    gasneti_leaf_finish(lc_opt); // TODO-EX: should support async local completion
    retval = gasnetc_AM_ReqRepGeneric(GASNETC_UCX_AM_LONG, jobrank, handler,
                                      flags, 1, numargs,
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
                                      flags, 0, numargs, argptr, NULL, 0, NULL GASNETI_THREAD_PASS);
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
                                      flags, 0, numargs, argptr, source_addr,
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
    gasneti_leaf_finish(lc_opt); // TODO-EX: should support async local completion
    retval = gasnetc_AM_ReqRepGeneric(GASNETC_UCX_AM_LONG, jobrank, handler,
                                      flags, 0, numargs, argptr, source_addr,
                                      nbytes, dest_addr GASNETI_THREAD_PASS);
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

  /* ptr-width dependent handlers */

  GASNETI_HANDLER_EOT
};

gex_AM_Entry_t const *gasnetc_get_handlertable(void) {
  return gasnetc_handlers;
}

/* ------------------------------------------------------------------------------------ */
