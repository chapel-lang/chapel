/*   $Source: bitbucket.org:berkeleylab/gasnet.git/pami-conduit/gasnet_core.c $
 * Description: GASNet PAMI conduit Implementation
 * Copyright 2012, Lawrence Berkeley National Laboratory
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_am.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>

GASNETI_IDENT(gasnetc_IdentString_Version, "$GASNetCoreLibraryVersion: " GASNET_CORE_VERSION_STR " $");
GASNETI_IDENT(gasnetc_IdentString_Name,    "$GASNetCoreLibraryName: " GASNET_CORE_NAME_STR " $");

gex_AM_Entry_t const *gasnetc_get_handlertable(void);

gex_AM_Entry_t *gasnetc_handler; // TODO-EX: will be replaced with per-EP tables

/* ------------------------------------------------------------------------------------ */
/* Defaults for environment variables */
#define GASNETC_AMPOLL_MAX_DEFAULT 16
#define GASNETC_NETWORKDEPTH_DEFAULT 1024

/* ------------------------------------------------------------------------------------ */
/* Global Data */
pami_client_t      gasnetc_pami_client;
pami_context_t     gasnetc_context; /* XXX: More than one */
pami_geometry_t    gasnetc_world_geom;
pami_endpoint_t    *gasnetc_endpoint_tbl;
size_t             gasnetc_num_contexts;            
pami_memregion_t   gasnetc_mymemreg;
pami_memregion_t   *gasnetc_memreg = NULL;
pami_memregion_t   gasnetc_myauxreg;
pami_memregion_t   *gasnetc_auxreg = NULL;
size_t             gasnetc_send_imm_max;
size_t             gasnetc_recv_imm_max;

/* exit-time coordination on a distinct context: */
static pami_context_t gasnetc_exit_context;

/* ------------------------------------------------------------------------------------ */
/* Static Data */

/* ------------------------------------------------------------------------------------ */
/* Forward Decls */

static int gasnetc_exit_init(int use_exit_geom);
static int gasnetc_am_init(void);
static void gasnetc_bootstrapSNodeBroadcast(void *src, size_t len, void *dst, int rootnode);

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

static int gasnetc_init(int *argc, char ***argv, gex_Flags_t flags) {
  pami_result_t rc;
  int use_exit_geom;

  /*  check system sanity */
  gasnetc_check_config();

  if (gasneti_init_done) 
    GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already initialized");

  gasneti_freezeForDebugger();

  #if GASNET_DEBUG_VERBOSE
    /* note - can't call trace macros during gasnet_init because trace system not yet initialized */
    fprintf(stderr,"gasnetc_init(): about to spawn...\n"); fflush(stderr);
  #endif

  /* (###) add code here to bootstrap the nodes for your conduit */

  /* For error messages only */
  gasneti_mynode = -1;
  gasneti_nodes  = -1;

  rc = PAMI_Client_create("GASNet", &gasnetc_pami_client, NULL, 0);
  GASNETC_PAMI_CHECK(rc, "calling PAMI_Client_create");

  { pami_configuration_t conf[3];
    conf[0].name = PAMI_CLIENT_TASK_ID;
    conf[1].name = PAMI_CLIENT_NUM_TASKS;
    conf[2].name = PAMI_CLIENT_NUM_CONTEXTS;

    rc = PAMI_Client_query(gasnetc_pami_client, conf, 3);
    GASNETC_PAMI_CHECK(rc, "calling PAMI_Client_query()");

    gasneti_mynode = conf[0].value.intval;
    gasneti_nodes  = conf[1].value.intval;
    gasnetc_num_contexts = conf[2].value.intval;
  }

  /* Must init timers after global env, and preferably before tracing */
  GASNETI_TICKS_INIT();

  /* Now enable tracing of all the following steps */
  gasneti_init_done = 1; /* required to allow tracing */
  gasneti_trace_init(argc, argv);

  /* Should we dedicate a context to exit coordination? */
  use_exit_geom = gasneti_getenv_yesno_withdefault("GASNET_PAMI_EXIT_CONTEXT", 0);

  { pami_context_t contexts[2];
    pami_configuration_t conf[1];
    conf[0].name = PAMI_CLIENT_CONST_CONTEXTS; /* promise equal num context across nodes */
    conf[0].value.intval = 1;
    rc = PAMI_Context_createv(gasnetc_pami_client, conf, 1, contexts, (use_exit_geom ? 2 : 1));
    GASNETC_PAMI_CHECK(rc, "calling PAMI_Context_createv");
    gasnetc_context = contexts[0];
    gasnetc_exit_context = contexts[1];
  }

  rc = PAMI_Geometry_world(gasnetc_pami_client, &gasnetc_world_geom);
  GASNETC_PAMI_CHECK(rc, "calling PAMI_Geometry_world()");

  gasneti_assert_zeroret(gasnetc_exit_init(use_exit_geom));
  gasneti_assert_zeroret(gasnetc_am_init());

  #if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_init(): spawn successful - node %i/%i starting...\n", 
      gasneti_mynode, gasneti_nodes); fflush(stderr);
  #endif

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
     Otherwise the conduit should pass a non-null gasnetc_bootstrapExchange
     as argument 1 to use platform-specific IDs, such as gethostid().
     See gasneti_nodemapInit() in gasnet_internal.c for more usage documentation.
     See below for info on gasnetc_bootstrapExchange()

     If the conduit can build gasneti_nodemap[] w/o assistance, it should
     call gasneti_nodemapParse() after constructing it (instead of nodemapInit()).
  */
  gasneti_nodemapInit(&gasnetc_bootstrapExchange, NULL, 0, 0);

  #if GASNET_PSHM
    /* (###) If your conduit will support PSHM, you should initialize it here.
     * The 1st argument is normally "&gasnetc_bootstrapSNodeBroadcast" or equivalent
     * The 2nd argument is the amount of shared memory space needed for any
     * conduit-specific uses.
     * The return value is a pointer to the space requested by the 2nd argument.
     * It is advisable that the conduit ensure pages in this space are touched,
     * possibly using gasneti_pshm_prefault(), prior to use of gasneti_segmentLimit()
     * or similar memory probes.
     */
    (void) gasneti_pshm_init(&gasnetc_bootstrapSNodeBroadcast, 0);
  #endif

  /* TODO: when PSHM support is enabled, consider moving table to shared mem */
  { int i;
    gasnetc_endpoint_tbl = gasneti_malloc(gasneti_nodes * sizeof(pami_endpoint_t));
    for (i = 0; i < gasneti_nodes; ++i) {
      gasnetc_endpoint_tbl[i] = PAMI_ENDPOINT_NULL;
    }
  }

#if HAVE_MMAP
  // Bound per-host (sharedLimit) argument to gasneti_segmentLimit()
  // while properly reserving space for aux segments.
  uint64_t sharedLimit = gasneti_sharedLimit();
  uint64_t hostAuxSegs = gasneti_myhost.node_count * gasneti_auxseg_preinit();
  if (sharedLimit <= hostAuxSegs) {
    gasneti_fatalerror("per-host segment limit %"PRIu64" is too small to accommodate %i aux segments, "
                       "total size %"PRIu64". You may need to adjust OS shared memory limits.",
                       sharedLimit, gasneti_myhost.node_count, hostAuxSegs);
  }
  sharedLimit -= hostAuxSegs;
#else
  #error "pami-conduit requires mmap() support" // Bug 3542
#endif

  uintptr_t mmap_limit = gasneti_segmentLimit((uintptr_t)-1, sharedLimit,
                                          &gasnetc_bootstrapExchange,
                                          &gasnetc_bootstrapBarrier);

  /* allocate and attach an aux segment */
  gasneti_auxsegAttach((uintptr_t)-1, &gasnetc_bootstrapExchange);

  void *auxbase = gasneti_seginfo_aux[gasneti_mynode].addr;
  uintptr_t auxsize = gasneti_seginfo_aux[gasneti_mynode].size;

  /* Register auxseg w/ PAMI and exchange the "keys" */
  { size_t regsize;
    rc = PAMI_Memregion_create(gasnetc_context,
                               auxbase, auxsize,
                               &regsize, &gasnetc_myauxreg);
  #if GASNETI_ARCH_IBMPE
    if (rc == PAMI_ERROR) {
      gasneti_fatalerror("Failed to pin the GASNet auxseg.  This may mean you have not enabled bulk xfers in your LoadLeveler script or poe command line.");
    }
  #endif
    GASNETC_PAMI_CHECK(rc, "registering the auxseg");
    if (regsize < auxsize) {
      /* TODO: If we still fail here, Put/Get will still work.  So, only warn? */
      gasneti_fatalerror("Unable to pin the GASNet auxseg");
    }

    gasnetc_auxreg = gasneti_malloc(gasneti_nodes * sizeof(pami_memregion_t));
    gasnetc_bootstrapExchange(&gasnetc_myauxreg, sizeof(pami_memregion_t), gasnetc_auxreg);
  }


  /* determine Max{Local,GLobal}SegmentSize */
  gasneti_segmentInit(mmap_limit, &gasnetc_bootstrapExchange, flags);

  #if 0 /* Current supported systems ensure environment is propogated */
    /* Enable this if you wish to use the default GASNet services for broadcasting 
        the environment from one compute node to all the others (for use in gasnet_getenv(),
        which needs to return environment variable values from the "spawning console").
        You need to provide two functions (gasnetc_bootstrapExchange and gasnetc_bootstrapBroadcast)
        which the system can safely and immediately use to broadcast and exchange information 
        between nodes (gasnetc_bootstrapBroadcast is optional but highly recommended).
        See gasnet/other/mpi-spawner/gasnet_bootstrap_mpi.c for definitions of these two
        functions in terms of MPI collective operations.
       This system assumes that at least one of the compute nodes has a copy of the 
        full environment from the "spawning console" (if this is not true, you'll need to
        implement something yourself to get the values from the spawning console)
       If your job system already always propagates environment variables to all the compute
        nodes, then you probably don't need this.
     */
    gasneti_setupGlobalEnvironment(gasneti_nodes, gasneti_mynode, 
                                   gasnetc_bootstrapExchange, gasnetc_bootstrapBroadcast);
  #endif

#if 0 /* was done above to allow early init of tracing */
  gasneti_init_done = 1;  
#endif

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

  /* ------------------------------------------------------------------------------------ */
  /*  primary attach complete */
  gasneti_attach_done = 1;
  gasnetc_bootstrapBarrier();

  GASNETI_TRACE_PRINTF(C,("gasnetc_attach_primary(): primary attach complete"));

  gasnete_init(); /* init the extended API */

  gasneti_nodemapFini();

  /* ensure extended API is initialized across nodes */
  gasnetc_bootstrapBarrier();

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

  gasneti_assert(((uintptr_t)segbase) % GASNET_PAGESIZE == 0);
  gasneti_assert(segsize % GASNET_PAGESIZE == 0);

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

  {
      /* Register w/ PAMI and exchange the "keys" */
      /* TODO: should this move to gasnete_init()? */
      if (gasneti_getenv_yesno_withdefault("GASNET_REGISTER_SEGMENT", 1)) {
        size_t regsize;
        pami_result_t rc;

        rc = PAMI_Memregion_create(gasnetc_context, segbase, segsize,
                                   &regsize, &gasnetc_mymemreg);
      #if GASNETI_ARCH_IBMPE
        if (rc == PAMI_ERROR) {
          gasneti_fatalerror("Failed to pin the GASNet segment.  This may mean you have not enabled bulk xfers in your LoadLeveler script or poe command line.");
        }
      #endif
        GASNETC_PAMI_CHECK(rc, "registering the segment");
        if (regsize < segsize) {
          /* TODO: probe max at init time instead of failing here. */
          /* TODO: If we still fail here, Put/Get will still work.  So, only warn? */
          gasneti_fatalerror("Unable to pin an adequate GASNet segment");
        }

        gasnetc_memreg = gasneti_malloc(gasneti_nodes * sizeof(pami_memregion_t));
        (*exchangefn)(&gasnetc_mymemreg, sizeof(pami_memregion_t), gasnetc_memreg);
      }
  }

  /* ------------------------------------------------------------------------------------ */
  /*  gather segment information */

  /* (###) add code here to gather the segment assignment info into
           gasneti_seginfo on each node (may be possible to use AMShortRequest here)
           If gasneti_segmentAttach() was used above, this is already done.
     Done in gasneti_segmentAttach(), above.
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
    if (GASNET_OK != gasnetc_attach_segment(&seg, _tm, segsize, gasnetc_bootstrapExchange, GASNETI_FLAG_INIT_LEGACY))
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
  #if 0
    /* called within gasnetc_init to allow init tracing */
    gasneti_trace_init(argc, argv);
  #endif
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

  // TODO-EX: remove when this limitation is removed
  static int once = 1;
  if (once) once = 0;
  else gasneti_fatalerror("gex_Segment_Attach: current implementation can be called at most once");

  #if GASNET_SEGMENT_EVERYTHING
    *segment_p = GEX_SEGMENT_INVALID;
    gex_Event_Wait(gex_Coll_BarrierNB(tm, 0));
    return GASNET_OK; 
  #endif

  /* create a segment collectively */
  // TODO-EX: this implementation only works *once*
  // TODO-EX: should be using the team's exchange function if possible
  // TODO-EX: need to pass proper flags (e.g. pshm and bind) instead of 0
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
    gasneti_assert(numreg == len);
  }

  { /*  extended API handlers */
    gex_AM_Entry_t *etable = (gex_AM_Entry_t *)gasnete_get_handlertable();
    int len = 0;
    int numreg = 0;
    gasneti_assert(etable);
    while (etable[len].gex_fnptr) len++; /* calc len */
    if (gasneti_amregister(ep->_amtbl, etable, len, GASNETE_HANDLER_BASE, GASNETI_CLIENT_HANDLER_BASE, 0, &numreg) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering extended API handlers");
    gasneti_assert(numreg == len);
  }

  return GASNET_OK;
}

extern int gasnetc_EP_RegisterHandlers(gex_EP_t                ep,
                                       gex_AM_Entry_t          *table,
                                       size_t                  numentries) {
  return gasneti_amregister_client(gasneti_import_ep(ep)->_amtbl, table, numentries);
}
/* ------------------------------------------------------------------------------------ */

/* Exit coordination timeouts */
#define GASNETC_DEFAULT_EXITTIMEOUT_MAX         360.0   /* 6 minutes! */
#define GASNETC_DEFAULT_EXITTIMEOUT_MIN         2       /* 2 seconds */
#define GASNETC_DEFAULT_EXITTIMEOUT_FACTOR      0.25    /* 1/4 second */
static double gasnetc_exittimeout = GASNETC_DEFAULT_EXITTIMEOUT_MAX;

/* Exit coordination vars */
static uint8_t gasnetc_exitcode = 0;
static pami_xfer_t gasnetc_exit_reduce_op;

static pami_geometry_t gasnetc_exit_geom;

static int gasnetc_exit_init(int use_exit_geom) {
  gasnetc_exittimeout = gasneti_get_exittimeout(GASNETC_DEFAULT_EXITTIMEOUT_MAX,
                                                GASNETC_DEFAULT_EXITTIMEOUT_MIN,
                                                GASNETC_DEFAULT_EXITTIMEOUT_FACTOR,
                                                GASNETC_DEFAULT_EXITTIMEOUT_MIN);

  if (use_exit_geom) {
    static pami_geometry_range_t slices[1]; /* static is required here! */
    pami_configuration_t conf[1];
    volatile unsigned int done;
    pami_result_t rc;

    /* Create a geometry from the contexts we've dedicated to the exit processing */
    conf[0].name = PAMI_GEOMETRY_OPTIMIZE;
    conf[0].value.intval = 0;
    slices[0].lo = 0;
    slices[0].hi = gasneti_nodes - 1;
    done = 0;
    rc = PAMI_Geometry_create_taskrange(gasnetc_pami_client, /* context_offset: */ 1,
                                        conf, 1,
                                        &gasnetc_exit_geom, gasnetc_world_geom,
                                        /* id: */ 2, slices, 1,
                                        gasnetc_context,
                                        &gasnetc_cb_inc_uint, (void*)&done);
    GASNETC_PAMI_CHECK(rc, "from PAMI_Geometry_create_taskrange()");
    while (!done) {
      GASNETC_PAMI_LOCK(gasnetc_context);
      rc = PAMI_Context_advance(gasnetc_context, 1);
      GASNETC_PAMI_CHECK_ADVANCE(rc, "advancing PAMI_Geometry_create_taskrange()");
      GASNETC_PAMI_UNLOCK(gasnetc_context);
    }
  } else {
     gasnetc_exit_context = gasnetc_context;
     gasnetc_exit_geom    = gasnetc_world_geom;
  }

  memset(&gasnetc_exit_reduce_op, 0, sizeof(gasnetc_exit_reduce_op));
  gasnetc_dflt_coll_alg(gasnetc_exit_geom, PAMI_XFER_ALLREDUCE, &gasnetc_exit_reduce_op.algorithm);

  // register process exit-time hook
  gasneti_registerExitHandler(gasnetc_exit);

  return GASNET_OK;
}

static int gasnetc_exit_reduce(void) {
  gasneti_tick_t start_time = gasneti_ticks_now();
  int64_t timeout_ns = gasnetc_exittimeout * 1.0e9;
  static uint8_t exitcode;
  pami_result_t rc;

  gasneti_weakatomic_t counter = gasneti_weakatomic_init(0);
  gasnetc_exit_reduce_op.cookie = (void *)&counter;
  gasnetc_exit_reduce_op.cb_done = &gasnetc_cb_inc_release;
  gasnetc_exit_reduce_op.options.multicontext = PAMI_HINT_DISABLE;

  gasnetc_exit_reduce_op.cmd.xfer_allreduce.sndbuf = (char*)&gasnetc_exitcode;
  gasnetc_exit_reduce_op.cmd.xfer_allreduce.stype = PAMI_TYPE_UNSIGNED_CHAR;
  gasnetc_exit_reduce_op.cmd.xfer_allreduce.stypecount = 1;
  gasnetc_exit_reduce_op.cmd.xfer_allreduce.rcvbuf = (char*)&exitcode;
  gasnetc_exit_reduce_op.cmd.xfer_allreduce.rtype = PAMI_TYPE_UNSIGNED_CHAR;
  gasnetc_exit_reduce_op.cmd.xfer_allreduce.rtypecount = 1;
  gasnetc_exit_reduce_op.cmd.xfer_allreduce.op = PAMI_DATA_MAX;
  gasnetc_exit_reduce_op.cmd.xfer_allreduce.data_cookie = NULL;
  gasnetc_exit_reduce_op.cmd.xfer_allreduce.commutative = 1;


  GASNETC_PAMI_LOCK(gasnetc_exit_context);
  rc = PAMI_Collective(gasnetc_exit_context, &gasnetc_exit_reduce_op);
  GASNETC_PAMI_UNLOCK(gasnetc_exit_context);
  if (rc != PAMI_SUCCESS) return 1;

  while (! gasneti_weakatomic_read(&counter, 0)) { /* TODO: Factor poll-with-timeout? */
    GASNETC_PAMI_LOCK(gasnetc_exit_context);
    rc = PAMI_Context_advance(gasnetc_exit_context, 1);
    GASNETC_PAMI_UNLOCK(gasnetc_exit_context);
    if ((rc != PAMI_SUCCESS && rc != PAMI_EAGAIN) ||
        (timeout_ns < gasneti_ticks_to_ns(gasneti_ticks_now() - start_time))) {
      return 1;
    }
  }

  gasneti_sync_reads();
  gasnetc_exitcode = exitcode;

  return 0;
}

extern void gasnetc_exit(int exitcode) {
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

  /* (###) add code here to terminate the job across _all_ nodes 
           with gasneti_killmyprocess(exitcode) (not regular exit()), preferably
           after raising a SIGQUIT to inform the client of the exit
  */

  /* Detect collective exit while performing reduce(MAX(exitcode))
   * The reduction has a timeout to distinguish non-collective exits */
  gasnetc_exitcode = exitcode;
  if (0 != gasnetc_exit_reduce()) {
    /* Failed to coordinate shutdown */
    /* XXX: can we raise SIGQUIT remotely, etc. (see bug 2785) */
    gasnetc_exitcode = 1; /* on both BG/Q and IBM PE this forces global termination */
  }
  gasneti_killmyprocess(gasnetc_exitcode);

  gasneti_fatalerror("gasnetc_exit failed!");
}

/* ------------------------------------------------------------------------------------ */
/* SNodeBroadcast for gasneti_pshm_init() */

#if GASNET_PSHM
#define gasneti_snodebcast_sz MAX(SIZEOF_VOID_P,GASNETI_PSHM_UNIQUE_LEN)

static uint8_t SNodeBcast_result[gasneti_snodebcast_sz];
static int SNodeBcast_done = 0;

static void SNodeBcast_dispatch(pami_context_t context, void *cookie,
                                const void *head_addr, size_t head_size,
                                const void *pipe_addr, size_t pipe_size,
                                pami_endpoint_t origin, pami_recv_t *recv)
{
  gasneti_assert(!SNodeBcast_done);
  gasneti_assert(head_size <= gasneti_snodebcast_sz);
  gasneti_assert(!recv); /* Never use payload */

  memcpy(SNodeBcast_result, head_addr, head_size);
  gasneti_sync_writes();
  SNodeBcast_done = 1;
}

static void gasnetc_bootstrapSNodeBroadcast(void *src, size_t len, void *dest, int rootnode) {
#if 0 /* Exchange in gasneti_nodemapInit() is sufficient to ensure Dispatch set */
  gasnetc_bootstrapBarrier();
#endif

  if (rootnode == gasneti_mynode) {
    pami_send_immediate_t cmd;
    pami_result_t rc;
    int i;

    memmove(dest, src, len);

    memset(&cmd, 0, sizeof(cmd));
    cmd.header.iov_base = src;
    cmd.header.iov_len = len;
    cmd.data.iov_base = NULL;
    cmd.data.iov_len = 0;
    cmd.dispatch = GASNETC_DISP_SNODE_BCAST;
    cmd.hints.use_shmem = PAMI_HINT_ENABLE;

    for (i = 0; i < gasneti_nodemap_local_count; ++i) {
      if (i == gasneti_nodemap_local_rank) continue;
      PAMI_Endpoint_create(gasnetc_pami_client, gasneti_nodemap_local[i], 0, &cmd.dest);
      rc = PAMI_Send_immediate(gasnetc_context, &cmd);
      GASNETC_PAMI_CHECK(rc, "from PAMI_Send_immediate(SNodeBcast)");
      rc = PAMI_Context_advance(gasnetc_context, 1);
      GASNETC_PAMI_CHECK_ADVANCE(rc, "advancing SNnodeBcast");
    }
  } else {
    while (!SNodeBcast_done) {
      int rc = PAMI_Context_advance(gasnetc_context, 1);
      GASNETC_PAMI_CHECK_ADVANCE(rc, "advancing SNnodeBcast");
      gasneti_spinloop_hint();
    }
    gasneti_sync_reads();
    memcpy(dest, SNodeBcast_result, len);
  }
}
#endif

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
 */
#endif

static pami_send_hint_t gasnetc_null_send_hint;
static size_t      gasnetc_ampoll_max;

static void noop_dispatch(pami_context_t context, void *cookie,
                          const void *head_addr, size_t head_size,
                          const void *pipe_addr, size_t pipe_size,
                          pami_endpoint_t origin, pami_recv_t *recv)
{ gasneti_fatalerror("in un-reachable NOOP dispatcher"); }

/* Bound the number of AM requests we have outstanding */

static gasneti_semaphore_t gasnetc_requests_oust;

GASNETI_INLINE(gasnetc_get_request_credit)
void gasnetc_get_request_credit(void) {
  if_pf (!gasneti_semaphore_trydown(&gasnetc_requests_oust)) {
    do {
      gasneti_AMPoll();
      gasneti_spinloop_hint();
    } while (!gasneti_semaphore_trydown(&gasnetc_requests_oust));
  }
}

GASNETI_INLINE(gasnetc_put_request_credit)
void gasnetc_put_request_credit(void) {
  gasneti_semaphore_up(&gasnetc_requests_oust);
}

/* AM "run" functions inlined into event and dispatch functions invoked by PAMI */

/* The received header (and debugging metadata) contitute our "token" */
typedef struct {
  union { // Portion sent/rcvd
    gasnetc_genmsg_t      generic;
    gasnetc_shortmsg_t    shortmsg;
    gasnetc_medmsg_t      medmsg;
    gasnetc_longmsg_t     longmsg;
  } u;
  // Portion constructed in memory
  const gex_AM_Entry_t *entry;
  int is_long;
} gasnetc_token_t;


/* XXX: Does serialization of handlers loosen the sync requirements here? */
static gasneti_lifo_head_t gasnetc_token_pool = GASNETI_LIFO_INITIALIZER;

GASNETI_INLINE(gasnetc_get_token)
gasnetc_token_t * gasnetc_get_token(void) {
  gasnetc_token_t *token = gasneti_lifo_pop(&gasnetc_token_pool);
  if_pf (!token) {
    token = gasneti_malloc(sizeof(gasnetc_token_t));
  }
  return token;
}

GASNETI_INLINE(gasnetc_put_token)
void gasnetc_put_token(gasnetc_token_t *token) {
  gasneti_lifo_push(&gasnetc_token_pool, token);
}

/* callback to free a token and release request credit, if any */
extern void gasnetc_cb_token(pami_context_t context, void *cookie, pami_result_t status) {
  const int is_req = ((gasnetc_token_t *)cookie)->u.generic.is_req;
  gasnetc_put_token(cookie);
  if (is_req) gasnetc_put_request_credit();
}


/* A token and a payload together in larger block form a "big_token" */

#define GASNETC_TOKEN_PAYLOAD(_token) ((char *)(_token) + GASNETC_MAX_MED_RESRV)

/* XXX: Does serialization of handlers loosen the sync requirements here? */
static gasneti_lifo_head_t gasnetc_big_token_pool = GASNETI_LIFO_INITIALIZER;

GASNETI_INLINE(gasnetc_get_big_token)
gasnetc_token_t * gasnetc_get_big_token(void) {
  gasnetc_token_t *token = gasneti_lifo_pop(&gasnetc_big_token_pool);
  if_pf (!token) {
    token = gasneti_malloc(GASNETC_MAX_MED_ALLOC);
  }
  return token;
}

GASNETI_INLINE(gasnetc_put_big_token)
void gasnetc_put_big_token(gasnetc_token_t *token) {
  gasneti_lifo_push(&gasnetc_big_token_pool, token);
}

/* callback to free a big_token and release request credit, if any */
extern void gasnetc_cb_big_token(pami_context_t context, void *cookie, pami_result_t status) {
  const int is_req = ((gasnetc_token_t *)cookie)->u.generic.is_req;
  gasnetc_put_big_token(cookie);
  if (is_req) gasnetc_put_request_credit();
}

GASNETI_INLINE(run_short)
void run_short(gasnetc_shortmsg_t *header) {
  const int                      is_req = header->is_req;
  const gex_AM_Index_t   handler_id = header->handler;
  const gex_AM_Fn_t handler_fn = gasnetc_handler[handler_id].gex_fnptr;
  const gex_AM_Arg_t     *args = header->args;
  const int                     numargs = header->numargs;

  /* copy generic portion to allow writes, but don't need to copy args */
  gasnetc_token_t rw_token;
  gex_Token_t client_token = (gex_Token_t)(&rw_token);
  rw_token.u.generic = *(gasnetc_genmsg_t*)header;
#if GASNET_DEBUG
  rw_token.u.generic.rep_sent = 0;
#endif
  rw_token.entry = &gasnetc_handler[handler_id];
  rw_token.is_long = 0;
  gasneti_amtbl_check(rw_token.entry, numargs, gasneti_Short, is_req);
  GASNETI_RUN_HANDLER_SHORT(is_req,handler_id,handler_fn,client_token,args,numargs);
}

GASNETI_INLINE(run_medium)
void run_medium(gasnetc_token_t *token) {
  gasnetc_medmsg_t              *header = &token->u.medmsg;
  const int                      is_req = header->is_req;
  const gex_AM_Index_t     handler_id = header->handler;
  const gex_AM_Fn_t handler_fn = gasnetc_handler[handler_id].gex_fnptr;
  const gex_AM_Arg_t     *args = header->args;
  const int                     numargs = header->numargs;
  void * const                     data = GASNETC_TOKEN_PAYLOAD(token);
  const size_t                   nbytes = header->nbytes;
  const gex_Token_t   client_token = (gex_Token_t)token;

#if GASNET_DEBUG
  header->rep_sent = 0;
#endif
  token->entry = &gasnetc_handler[handler_id];
  token->is_long = 0;
  gasneti_amtbl_check(token->entry, numargs, gasneti_Medium, is_req);
  GASNETI_RUN_HANDLER_MEDIUM(is_req,handler_id,handler_fn,client_token,args,numargs,data,nbytes);
}

GASNETI_INLINE(run_long)
void run_long(gasnetc_token_t *token) {
  gasnetc_longmsg_t             *header = &token->u.longmsg;
  const int                      is_req = header->is_req;
  const gex_AM_Index_t     handler_id = header->handler;
  const gex_AM_Fn_t handler_fn = gasnetc_handler[handler_id].gex_fnptr;
  const gex_AM_Arg_t     *args = header->args;
  const int                     numargs = header->numargs;
  void * const                     data = (void*)header->addr;
  const size_t                   nbytes = header->nbytes;
  const gex_Token_t   client_token = (gex_Token_t)token;

#if GASNET_DEBUG
  header->rep_sent = 0;
#endif
  token->entry = &gasnetc_handler[handler_id];
  token->is_long = 1;
  gasneti_amtbl_check(token->entry, numargs, gasneti_Long, is_req);
  GASNETI_RUN_HANDLER_LONG(is_req,handler_id,handler_fn,client_token,args,numargs,data,nbytes);
}

/* AM event functions, run when payload has been written */

/* No Short event functions, since only "immediate" is implemented for Short */

static void am_Med_event(pami_context_t context, void *cookie, pami_result_t status)
{
  gasnetc_token_t * const token = (gasnetc_token_t *)cookie;
  GASNETC_PAMI_CHECK(status, "while receiving AM Medium payload");
  run_medium(token);
  gasnetc_put_big_token(token);
}

static void am_Long_event(pami_context_t context, void *cookie, pami_result_t status)
{
  gasnetc_token_t * const token = (gasnetc_token_t *)cookie;
  GASNETC_PAMI_CHECK(status, "while receiving AM Long payload");
  run_long(token);
  gasnetc_put_token(token);
}

/* AM dispatch functions, run as soon as header has arrived. */

static void am_Short_dispatch(
                           pami_context_t context, void *cookie,
                           const void *head_addr, size_t head_size,
                           const void *pipe_addr, size_t pipe_size,
                           pami_endpoint_t origin, pami_recv_t *recv)
{
  gasnetc_shortmsg_t *shortmsg = (gasnetc_shortmsg_t *)head_addr;

  gasneti_assert(head_size == GASNETC_ARGSEND(short, shortmsg->numargs));
  gasneti_assert(!recv); /* Never use payload */

  run_short(shortmsg);
}

static void am_Med_dispatch(
                           pami_context_t context, void *cookie,
                           const void *head_addr, size_t head_size,
                           const void *pipe_addr, size_t pipe_size,
                           pami_endpoint_t origin, pami_recv_t *recv)
{
  gasnetc_medmsg_t *medmsg = (gasnetc_medmsg_t *)head_addr;
  gasnetc_token_t *token = gasnetc_get_big_token();
  void *data = GASNETC_TOKEN_PAYLOAD(token);

  gasneti_assert(head_size == GASNETC_ARGSEND(med, medmsg->numargs));

  memcpy(&token->u, head_addr, head_size);

  if (!recv) {
    /* Entire message has arrived - copy to aligned memory and run now */
    memcpy(data, pipe_addr, pipe_size);
    gasneti_assert(pipe_size == medmsg->nbytes);
    run_medium(token);
    gasnetc_put_big_token(token);
  } else {
    /* instruct PAMI how to deliver payload */
    recv->cookie = token;
    recv->local_fn = &am_Med_event;
    recv->addr = data;
#if 0 /* the hints recv_contiguous and recv_copy ensure we can ignore these */
    recv->type = PAMI_TYPE_BYTE;
    recv->offset = 0;
    recv->data_fn = PAMI_DATA_COPY;
    recv->data_cookie = NULL;
#endif
  }
}

static void am_Long_dispatch(
                           pami_context_t context, void *cookie,
                           const void *head_addr, size_t head_size,
                           const void *pipe_addr, size_t pipe_size,
                           pami_endpoint_t origin, pami_recv_t *recv)
{
  gasnetc_longmsg_t *longmsg = (gasnetc_longmsg_t *)head_addr;
  void * const data = (void*)longmsg->addr;

  gasneti_assert(head_size == GASNETC_ARGSEND(long, longmsg->numargs));

#if PLATFORM_COMPILER_CLANG
  if_pf (!recv) { /* PAMI or Clang bug: we've disabled this explicitly! */
    /* Entire message has arrived - copy data and run now */
    gasnetc_token_t *token = gasnetc_get_token();
    memcpy(&token->u, head_addr, head_size);
    gasneti_assert(pipe_size == longmsg->nbytes);
    memcpy((void*)longmsg->addr, pipe_addr, pipe_size);
    run_long(token);
    gasnetc_put_token(token);
    return;
  }
#else
  gasneti_assert(recv != NULL); /* Due to recv_immediate = PAMI_HINT_DISABLE */
#endif

  { /* Only our header has arrived - setup copy data and async run */
    gasnetc_token_t *token = gasnetc_get_token();
    memcpy(&token->u, head_addr, head_size);
    /* instruct PAMI how to deliver payload */
    recv->cookie = token;
    recv->local_fn = &am_Long_event;
    recv->addr = data;
    // the hints recv_contiguous and recv_copy should ensure we can ignore these,
    // but evidence shows that is not the case with PAMI_Send_immediate
    recv->type = PAMI_TYPE_BYTE;
    recv->offset = 0;
    recv->data_fn = PAMI_DATA_COPY;
    recv->data_cookie = NULL;
  }
}

static int gasnetc_am_init(void) {
  pami_dispatch_hint_t hints;
  pami_dispatch_callback_function fn;
  pami_configuration_t conf[2];
  pami_result_t rc;

  gasnetc_ampoll_max = gasneti_getenv_int_withdefault("GASNET_AMPOLL_MAX", GASNETC_AMPOLL_MAX_DEFAULT, 0);
  gasnetc_ampoll_max = MAX(1, gasnetc_ampoll_max); /* Min is 1 */

  memset(&gasnetc_null_send_hint, 0, sizeof(gasnetc_null_send_hint));
#if GASNET_PSHM
  gasnetc_null_send_hint.use_shmem = PAMI_HINT_DISABLE;
#endif

  { gasnetc_token_t t;
    gasneti_assert_always(GASNETC_MAX_MED_RESRV >= sizeof(t.u));
  }

  conf[0].name = PAMI_CONTEXT_DISPATCH_ID_MAX;
  rc = PAMI_Context_query(gasnetc_context, conf, 1);
  GASNETC_PAMI_CHECK(rc, "calling PAMI_Context_query() for DISPATCH_ID_MAX");
  GASNETI_TRACE_PRINTF(C,("DISPATCH_ID_MAX = %"PRIuPTR, (uintptr_t)conf[0].value.intval));
  gasneti_assert_always(conf[0].value.intval >= (GASNETC_NUM_DISP-1));

  memset(&hints, 0, sizeof(hints));
  memset(&fn, 0, sizeof(fn));

  /* TODO: Others hints? */
  hints.multicontext = PAMI_HINT_DISABLE;
  hints.recv_contiguous = PAMI_HINT_ENABLE;
  hints.recv_copy = PAMI_HINT_ENABLE;
#if GASNET_PSHM
  hints.use_shmem = PAMI_HINT_DISABLE;
#endif

  /* Must register a dummy dispatch just so we can query immediate limits.  Sigh. */
  fn.p2p = &noop_dispatch;
  rc = PAMI_Dispatch_set(gasnetc_context, GASNETC_DISP_NOOP, fn, NULL, hints);
  GASNETC_PAMI_CHECK(rc, "registering GASNETC_DISP_NOOP");

  /* Query immediate limits */
  conf[0].name = PAMI_DISPATCH_SEND_IMMEDIATE_MAX;
  conf[1].name = PAMI_DISPATCH_RECV_IMMEDIATE_MAX;
  rc = PAMI_Dispatch_query(gasnetc_context, GASNETC_DISP_NOOP, conf, 2);
  GASNETC_PAMI_CHECK(rc, "querying DISPATCH immediate limits");
  gasnetc_send_imm_max = conf[0].value.intval;
  GASNETI_TRACE_PRINTF(C,("PAMI_DISPATCH_SEND_IMMEDIATE_MAX = %"PRIuPTR,
                          (uintptr_t)gasnetc_send_imm_max));
  gasnetc_recv_imm_max = conf[1].value.intval;
  GASNETI_TRACE_PRINTF(C,("PAMI_DISPATCH_RECV_IMMEDIATE_MAX = %"PRIuPTR,
                          (uintptr_t)gasnetc_recv_imm_max));

  /* Register dispatch fn for AMShort */
  hints.long_header = (gasnetc_recv_imm_max >= sizeof(gasnetc_shortmsg_t))
                      ? PAMI_HINT_DISABLE : PAMI_HINT_ENABLE;
  hints.recv_immediate = (gasnetc_recv_imm_max >= sizeof(gasnetc_shortmsg_t))
                         ? PAMI_HINT_ENABLE : PAMI_HINT_DEFAULT;
  fn.p2p = &am_Short_dispatch;
  rc = PAMI_Dispatch_set(gasnetc_context, GASNETC_DISP_SHORT, fn, NULL, hints);
  GASNETC_PAMI_CHECK(rc, "registering GASNETC_DISP_SHORT");

  /* Register dispatch fn for AMMedium */
  hints.long_header = (gasnetc_recv_imm_max >= sizeof(gasnetc_medmsg_t))
                      ? PAMI_HINT_DISABLE : PAMI_HINT_ENABLE;
  hints.recv_immediate = (gasnetc_recv_imm_max >= 
                             (sizeof(gasnetc_medmsg_t) + GASNETC_MAX_MEDIUM))
                         ? PAMI_HINT_ENABLE : PAMI_HINT_DEFAULT;
  fn.p2p = &am_Med_dispatch;
  rc = PAMI_Dispatch_set(gasnetc_context, GASNETC_DISP_MED, fn, NULL, hints);
  GASNETC_PAMI_CHECK(rc, "registering GASNETC_DISP_MED");

  /* Register dispatch fn for AMLong */
  hints.long_header = (gasnetc_recv_imm_max >= sizeof(gasnetc_longmsg_t))
                      ? PAMI_HINT_DISABLE : PAMI_HINT_ENABLE;
  hints.recv_immediate = PAMI_HINT_DISABLE; /* force "async" delivery */
  fn.p2p = &am_Long_dispatch;
  rc = PAMI_Dispatch_set(gasnetc_context, GASNETC_DISP_LONG, fn, NULL, hints);
  GASNETC_PAMI_CHECK(rc, "registering GASNETC_DISP_LONG");

  { 
    unsigned int depth =
        gasneti_getenv_int_withdefault("GASNET_NETWORKDEPTH", GASNETC_NETWORKDEPTH_DEFAULT, 0);
    depth = MAX(depth, 4); /* Min value is 4 */
    gasneti_semaphore_init(&gasnetc_requests_oust, depth, depth);
  }

#if GASNET_PSHM
  hints.use_shmem = PAMI_HINT_ENABLE;
  hints.long_header = (gasnetc_recv_imm_max >= gasneti_snodebcast_sz)
                      ? PAMI_HINT_DISABLE : PAMI_HINT_ENABLE;
  hints.recv_immediate = (gasnetc_recv_imm_max >= gasneti_snodebcast_sz)
                         ? PAMI_HINT_ENABLE : PAMI_HINT_DEFAULT;
  fn.p2p = &SNodeBcast_dispatch;
  rc = PAMI_Dispatch_set(gasnetc_context, GASNETC_DISP_SNODE_BCAST, fn, NULL, hints);
  if (rc != PAMI_SUCCESS) { /* On failure retry w/o use_shmem hint */
    hints.use_shmem = PAMI_HINT_DISABLE;
    rc = PAMI_Dispatch_set(gasnetc_context, GASNETC_DISP_SNODE_BCAST, fn, NULL, hints);
  }
  GASNETC_PAMI_CHECK(rc, "registering GASNETC_DISP_SNODE_BCAST");
#endif

  return GASNET_OK;
}

GASNETI_INLINE(gasnetc_msgsource)
gex_Rank_t gasnetc_msgsource(gex_Token_t token) {
  gasneti_assert(! gasnetc_token_in_nbrhd(token));
  gasneti_assert(token);
  gex_Rank_t sourceid = *(gex_Rank_t *)token;
  gasneti_assert(sourceid < gasneti_nodes);
  return sourceid;
}

extern gex_TI_t gasnetc_Token_Info(
                gex_Token_t         token,
                gex_Token_Info_t    *info,
                gex_TI_t            mask)
{
  gasneti_assert(token);
  gasneti_assert(info);

  if (gasnetc_token_in_nbrhd(token)) {
    return gasnetc_nbrhd_Token_Info(token, info, mask);
  }

  gasnetc_token_t *real_token = (gasnetc_token_t *)(token);
  gex_TI_t result = 0;

  info->gex_srcrank = gasnetc_msgsource(token);
  result |= GEX_TI_SRCRANK;

  info->gex_ep = gasneti_THUNK_EP;
  result |= GEX_TI_EP;

  info->gex_entry = real_token->entry;
  result |= GEX_TI_ENTRY;

  info->gex_is_req = real_token->u.generic.is_req;
  result |= GEX_TI_IS_REQ;

  info->gex_is_long = real_token->is_long;
  result |= GEX_TI_IS_LONG;

  return GASNETI_TOKEN_INFO_RETURN(result, info, mask);
}

extern int gasnetc_AMPoll(GASNETI_THREAD_FARG_ALONE) {
  int retval;
  GASNETI_CHECKATTACH();

#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it make progress here. */
  gasneti_AMPSHMPoll(0 GASNETI_THREAD_PASS);
#endif

  /* (###) add code here to run your AM progress engine */
#if GASNET_PAR
  PAMI_Context_trylock_advancev(&gasnetc_context, 1, gasnetc_ampoll_max);
#else
  PAMI_Context_advance(gasnetc_context, gasnetc_ampoll_max);
#endif

  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
*/

#define GASNETC_AM_COPY_ARGS(_args, _numargs, _argptr)           \
  do {                                                           \
    int _i;                                                      \
    for (_i = 0; _i < _numargs; ++_i) {                          \
      (_args)[_i] = va_arg(_argptr, gex_AM_Arg_t);      \
    }                                                            \
  } while (0)

#if GASNET_DEBUG
  #define GASNETC_AM_VALIDATE_TOKEN(_cat,_token) do {            \
      gasnetc_token_t *real_token = (gasnetc_token_t *)(_token); \
      gasneti_assert(real_token->u._cat##msg.is_req);          \
      gasneti_assert(!real_token->u._cat##msg.rep_sent);       \
      real_token->u._cat##msg.rep_sent = 1;                    \
    } while (0)
#else
  #define GASNETC_AM_VALIDATE_TOKEN(_cat,_token) do { } while (0)
#endif

#define GASNETC_AM_MSG_COMMON(_msg, _handler, _numargs, _argptr, _is_req) \
  do {                                                           \
    (_msg).srcnode = gasneti_mynode;                             \
    (_msg).handler = _handler;                                   \
    (_msg).numargs = _numargs;                                   \
    (_msg).is_req  = _is_req;                                    \
    GASNETC_AM_COPY_ARGS((_msg).args, _numargs, _argptr);        \
  } while (0)

GASNETI_INLINE(gasnetc_AMRequestShort)
int gasnetc_AMRequestShort(
                gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                gex_Flags_t flags,
                int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  int retval = GASNET_OK;
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    retval = gasnetc_nbrhd_RequestGeneric(gasneti_Short, jobrank, handler,
                                          NULL, 0, NULL,
                                          flags, numargs, argptr GASNETI_THREAD_PASS);
  } else {
    /* (###) add code here to read the arguments using va_arg(argptr, gex_AM_Arg_t)
             and send the active message 
     */
    pami_send_immediate_t cmd;
    pami_result_t rc;
    gasnetc_shortmsg_t msg;

    GASNETC_AM_MSG_COMMON(msg, handler, numargs, argptr, 1);

    cmd.header.iov_base = (char *)&msg;
    cmd.header.iov_len = GASNETC_ARGSEND(short, numargs);
    cmd.data.iov_base = NULL;
    cmd.data.iov_len = 0;
    cmd.dest = gasnetc_endpoint(jobrank);
    cmd.dispatch = GASNETC_DISP_SHORT;
    cmd.hints = gasnetc_null_send_hint;

    gasnetc_get_request_credit();

    GASNETC_PAMI_LOCK(gasnetc_context);
    rc = PAMI_Send_immediate(gasnetc_context, &cmd);
    GASNETC_PAMI_CHECK(rc, "from PAMI_Send_immediate(AMRequestShort)");
    rc = PAMI_Context_advance(gasnetc_context, 1);
    GASNETC_PAMI_CHECK_ADVANCE(rc, "advancing an AMRequestShort");
    GASNETC_PAMI_UNLOCK(gasnetc_context);

    gasnetc_put_request_credit();
  }
  return retval;
}

extern int gasnetc_AMRequestShortM(
                gex_TM_t tm,
                gex_Rank_t rank,
                gex_AM_Index_t handler,
                gex_Flags_t flags
                GASNETI_THREAD_FARG,
                int numargs, ...)
{
  GASNETI_COMMON_AMREQUESTSHORT(tm,rank,handler,flags,numargs);
  GASNETC_IMMEDIATE_MAYBE_POLL(flags); /* poll at least once, to assure forward progress */

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMRequestShort(tm,rank,handler,flags,numargs,argptr GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
}

GASNETI_INLINE(gasnetc_AMRequestMedium)
int gasnetc_AMRequestMedium(
                gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                /*const*/ void *source_addr, size_t nbytes,
                gex_Event_t *lc_opt, gex_Flags_t flags,
                int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  int retval = GASNET_OK;
  gasneti_leaf_finish(lc_opt); // TODO-EX: should support async local completion
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    retval = gasnetc_nbrhd_RequestGeneric(gasneti_Medium, jobrank, handler,
                                          source_addr, nbytes, NULL,
                                          flags, numargs, argptr GASNETI_THREAD_PASS);
  } else {
    /* (###) add code here to read the arguments using va_arg(argptr, gex_AM_Arg_t)
             and send the active message 
     */
    pami_send_t cmd;
    pami_result_t rc;
    gasnetc_medmsg_t msg;
    gasnetc_medmsg_t *msg_p;
    char * payload;

    const size_t header_len = GASNETC_ARGSEND(med, numargs);
    const int send_imm = (nbytes + header_len) <= gasnetc_send_imm_max;

    if (send_imm) {
        msg_p = &msg;
        payload = source_addr;
    } else {
        msg_p = &(gasnetc_get_big_token()->u.medmsg);
        payload = GASNETC_TOKEN_PAYLOAD(msg_p);
        memcpy(payload, source_addr, nbytes);
        cmd.events.cookie = (void*)msg_p;
        cmd.events.local_fn = &gasnetc_cb_big_token;
        cmd.events.remote_fn = NULL;
    }

    GASNETC_AM_MSG_COMMON((*msg_p), handler, numargs, argptr, 1);
    msg_p->nbytes = nbytes;

    /* TODO: If register bounce buffers, then apply appropriate hint (here or at Dispatch_set?) */
    cmd.send.header.iov_base = (char *)msg_p;
    cmd.send.header.iov_len = header_len;
    cmd.send.data.iov_base = payload;
    cmd.send.data.iov_len = nbytes;
    cmd.send.dest = gasnetc_endpoint(jobrank);
    cmd.send.dispatch = GASNETC_DISP_MED;
    cmd.send.hints = gasnetc_null_send_hint;

    gasnetc_get_request_credit();

    GASNETC_PAMI_LOCK(gasnetc_context);
    if (send_imm) {
        rc = PAMI_Send_immediate(gasnetc_context, &cmd.send);
        GASNETC_PAMI_CHECK(rc, "from PAMI_Send_immediate(AMRequestMedium)");
    } else {
        rc = PAMI_Send(gasnetc_context, &cmd);
        GASNETC_PAMI_CHECK(rc, "from PAMI_Send(AMRequestMedium)");
    }
    rc = PAMI_Context_advance(gasnetc_context, 1);
    GASNETC_PAMI_CHECK_ADVANCE(rc, "advancing an AMRequestMedium");
    GASNETC_PAMI_UNLOCK(gasnetc_context);

    if (send_imm) gasnetc_put_request_credit();
  }
  return retval;
}

extern int gasnetc_AMRequestMediumV(
                gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                /*const*/ void *source_addr, size_t nbytes,
                gex_Event_t *lc_opt, gex_Flags_t flags,
                int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  return gasnetc_AMRequestMedium(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
}

extern int gasnetc_AMRequestMediumM(
                gex_TM_t tm,
                gex_Rank_t rank,
                gex_AM_Index_t handler,
                /*const*/ void *source_addr,
                size_t nbytes,
                gex_Event_t *lc_opt,
                gex_Flags_t flags
                GASNETI_THREAD_FARG,
                int numargs, ...)
{
  GASNETI_COMMON_AMREQUESTMEDIUM(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs);
  GASNETC_IMMEDIATE_MAYBE_POLL(flags); /* poll at least once, to assure forward progress */

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMRequestMedium(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
}

GASNETI_INLINE(gasnetc_AMRequestLong)
int gasnetc_AMRequestLong(
                gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                /*const*/ void *source_addr, size_t nbytes, void *dest_addr,
                gex_Event_t *lc_opt, gex_Flags_t flags,
                int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  int retval = GASNET_OK;
  gasneti_leaf_finish(lc_opt); // TODO-EX: should support async local completion
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    retval = gasnetc_nbrhd_RequestGeneric(gasneti_Long, jobrank, handler,
                                          source_addr, nbytes, dest_addr,
                                          flags, numargs, argptr GASNETI_THREAD_PASS);
  } else {
    /* (###) add code here to read the arguments using va_arg(argptr, gex_AM_Arg_t)
             and send the active message 
     */
    pami_send_t cmd;
    pami_result_t rc;
    gasnetc_longmsg_t msg;
    volatile unsigned int counter = 0;

    const size_t header_len = GASNETC_ARGSEND(long, numargs);
    const int send_imm = (nbytes + header_len) <= gasnetc_send_imm_max;

    if (!send_imm) {
        cmd.events.cookie = (void*)&counter;
        cmd.events.local_fn = &gasnetc_cb_inc_uint;
        cmd.events.remote_fn = NULL;
    }

    GASNETC_AM_MSG_COMMON(msg, handler, numargs, argptr, 1);
    msg.addr = (uintptr_t)dest_addr;
    msg.nbytes = nbytes;

    /* TODO: Register segment and apply appropriate hint(s) here or at Dispatch_set */
    cmd.send.header.iov_base = (char *)&msg;
    cmd.send.header.iov_len = header_len;
    cmd.send.data.iov_base = (char *)source_addr;
    cmd.send.data.iov_len = nbytes;
    cmd.send.dest = gasnetc_endpoint(jobrank);
    cmd.send.dispatch = GASNETC_DISP_LONG;
    cmd.send.hints = gasnetc_null_send_hint;

    gasnetc_get_request_credit();

    GASNETC_PAMI_LOCK(gasnetc_context);
    if (send_imm) {
        rc = PAMI_Send_immediate(gasnetc_context, &cmd.send);
        GASNETC_PAMI_CHECK(rc, "from PAMI_Send_immediate(AMRequestLong)");
    } else {
        rc = PAMI_Send(gasnetc_context, &cmd);
        GASNETC_PAMI_CHECK(rc, "from PAMI_Send(AMRequestLong)");
    }
    rc = PAMI_Context_advance(gasnetc_context, 1);
    GASNETC_PAMI_CHECK_ADVANCE(rc, "advancing an AMRequestLong");
    GASNETC_PAMI_UNLOCK(gasnetc_context);

    if (!send_imm) gasneti_polluntil(counter == 1); /* stall for local completion */
    gasnetc_put_request_credit();
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
                gex_TM_t tm,   // Names a local context ("return address")
                gex_Rank_t rank,          // Together with 'tm', names a remote context
                gex_AM_Index_t handler,    // Index into handler table of remote context
                /*const*/ void *source_addr,   // Payload address (or OFFSET)
                size_t nbytes,                 // Payload length
                void *dest_addr,               // Payload destination address (or OFFSET)
                gex_Event_t *lc_opt,  // Local completion control (see above)
                gex_Flags_t flags         // Flags to control this operation
                GASNETI_THREAD_FARG,
                int numargs, ...)
{
  GASNETI_COMMON_AMREQUESTLONG(tm,rank,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs);
  GASNETC_IMMEDIATE_MAYBE_POLL(flags); /* poll at least once, to assure forward progress */

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMRequestLong(tm,rank,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
}

GASNETI_INLINE(gasnetc_AMReplyShort)
int gasnetc_AMReplyShort(
                gex_Token_t token, gex_AM_Index_t handler,
                gex_Flags_t flags,
                int numargs, va_list argptr)
{
  int retval = GASNET_OK;
  if_pt (gasnetc_token_in_nbrhd(token)) {
    retval = gasnetc_nbrhd_ReplyGeneric(gasneti_Short, token, handler,
                                        NULL, 0, NULL,
                                        flags, numargs, argptr);
  } else {
    /* (###) add code here to read the arguments using va_arg(argptr, gex_AM_Arg_t)
             and send the active message 
     */
    pami_send_immediate_t cmd;
    pami_result_t rc;
    gasnetc_shortmsg_t msg;

    gex_Rank_t jobrank = gasnetc_msgsource(token);

    GASNETC_AM_VALIDATE_TOKEN(short, token);
    GASNETC_AM_MSG_COMMON(msg, handler, numargs, argptr, 0);

    cmd.header.iov_base = (char *)&msg;
    cmd.header.iov_len = GASNETC_ARGSEND(short, numargs);
    cmd.data.iov_base = NULL;
    cmd.data.iov_len = 0;
    cmd.dest = gasnetc_endpoint(jobrank);
    cmd.dispatch = GASNETC_DISP_SHORT;
    cmd.hints = gasnetc_null_send_hint;

    /* Lock is held in handler context */
    rc = PAMI_Send_immediate(gasnetc_context, &cmd);
    GASNETC_PAMI_CHECK(rc, "from PAMI_Send_immediate(AMReplyShort)");
  }
  return retval;
}

extern int gasnetc_AMReplyShortM(
                gex_Token_t token,
                gex_AM_Index_t handler,
                gex_Flags_t flags,
                int numargs, ...)
{
  GASNETI_COMMON_AMREPLYSHORT(token,handler,flags,numargs);

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMReplyShort(token,handler,flags,numargs,argptr);
  va_end(argptr);
  return retval;
}

GASNETI_INLINE(gasnetc_AMReplyMedium)
int gasnetc_AMReplyMedium(
                gex_Token_t token, gex_AM_Index_t handler,
                /*const*/ void *source_addr, size_t nbytes,
                gex_Event_t *lc_opt, gex_Flags_t flags,
                int numargs, va_list argptr)
{
  int retval = GASNET_OK;
  gasneti_leaf_finish(lc_opt); // TODO-EX: should support async local completion
  if_pt (gasnetc_token_in_nbrhd(token)) {
    retval = gasnetc_nbrhd_ReplyGeneric(gasneti_Medium, token, handler,
                                        source_addr, nbytes, NULL,
                                        flags, numargs, argptr);
  } else {
    /* (###) add code here to read the arguments using va_arg(argptr, gex_AM_Arg_t)
             and send the active message 
     */
    pami_send_t cmd;
    pami_result_t rc;
    gasnetc_medmsg_t msg;
    gasnetc_medmsg_t *msg_p;
    char * payload;

    const size_t header_len = GASNETC_ARGSEND(med, numargs);
    const int send_imm = (nbytes + header_len) <= gasnetc_send_imm_max;

    if (send_imm) {
        msg_p = &msg;
        payload = source_addr;
    } else {
        msg_p = &(gasnetc_get_big_token()->u.medmsg);
        payload = GASNETC_TOKEN_PAYLOAD(msg_p);
        memcpy(payload, source_addr, nbytes);
        cmd.events.cookie = (void*)msg_p;
        cmd.events.local_fn = &gasnetc_cb_big_token;
        cmd.events.remote_fn = NULL;
    }

    gex_Rank_t jobrank = gasnetc_msgsource(token);

    GASNETC_AM_VALIDATE_TOKEN(med, token);
    GASNETC_AM_MSG_COMMON((*msg_p), handler, numargs, argptr, 0);
    msg_p->nbytes = nbytes;

    /* TODO: If register bounce buffers, then apply appropriate hint (here or at Dispatch_set?) */
    cmd.send.header.iov_base = (char *)msg_p;
    cmd.send.header.iov_len = header_len;
    cmd.send.data.iov_base = payload;
    cmd.send.data.iov_len = nbytes;
    cmd.send.dest = gasnetc_endpoint(jobrank);
    cmd.send.dispatch = GASNETC_DISP_MED;
    cmd.send.hints = gasnetc_null_send_hint;

    /* Lock is held in handler context */
    if (send_imm) {
        rc = PAMI_Send_immediate(gasnetc_context, &cmd.send);
        GASNETC_PAMI_CHECK(rc, "from PAMI_Send_immediate(AMReplyMedium)");
    } else {
        rc = PAMI_Send(gasnetc_context, &cmd);
        GASNETC_PAMI_CHECK(rc, "from PAMI_Send(AMReplyMedium)");
    }
  }
  return retval;
}

extern int gasnetc_AMReplyMediumV(
                gex_Token_t token, gex_AM_Index_t handler,
                /*const*/ void *source_addr, size_t nbytes,
                gex_Event_t *lc_opt, gex_Flags_t flags,
                int numargs, va_list argptr)
{
  return gasnetc_AMReplyMedium(token,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr);
}

extern int gasnetc_AMReplyMediumM(
                gex_Token_t token,
                gex_AM_Index_t handler,
                /*const*/ void *source_addr,
                size_t nbytes,
                gex_Event_t *lc_opt,
                gex_Flags_t flags,
                int numargs, ...)
{
  GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,lc_opt,flags,numargs);

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMReplyMedium(token,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr);
  va_end(argptr);
  return retval;
}

GASNETI_INLINE(gasnetc_AMReplyLong)
int gasnetc_AMReplyLong(
                gex_Token_t token, gex_AM_Index_t handler,
                /*const*/ void *source_addr, size_t nbytes, void *dest_addr,
                gex_Event_t *lc_opt, gex_Flags_t flags,
                int numargs, va_list argptr)
{
  int retval = GASNET_OK;
  gasneti_leaf_finish(lc_opt); // TODO-EX: should support async local completion
  if_pt (gasnetc_token_in_nbrhd(token)) {
    retval = gasnetc_nbrhd_ReplyGeneric(gasneti_Long, token, handler,
                                        source_addr, nbytes, dest_addr,
                                        flags, numargs, argptr);
  } else {
    /* (###) add code here to read the arguments using va_arg(argptr, gex_AM_Arg_t)
             and send the active message 
     */
    pami_send_t cmd;
    pami_result_t rc;
    gasnetc_longmsg_t msg;
    gasnetc_longmsg_t *msg_p;
    char * payload;

    const size_t header_len = GASNETC_ARGSEND(long, numargs);
    const int send_imm = (nbytes + header_len) <= gasnetc_send_imm_max;

    if (send_imm) {
        msg_p = &msg;
        payload = source_addr;
    } else {
        msg_p = &(gasnetc_get_big_token()->u.longmsg);
        payload = GASNETC_TOKEN_PAYLOAD(msg_p);
        memcpy(payload, source_addr, nbytes);
        cmd.events.cookie = (void*)msg_p;
        cmd.events.local_fn = &gasnetc_cb_big_token;
        cmd.events.remote_fn = NULL;
    }

    gex_Rank_t jobrank = gasnetc_msgsource(token);

    GASNETC_AM_VALIDATE_TOKEN(long, token);
    GASNETC_AM_MSG_COMMON((*msg_p), handler, numargs, argptr, 0);
    msg_p->nbytes = nbytes;
    msg_p->addr = (uintptr_t)dest_addr;

    /* TODO: If register bounce buffers, then apply appropriate hint (here or at Dispatch_set?) */
    cmd.send.header.iov_base = (char *)msg_p;
    cmd.send.header.iov_len = header_len;
    cmd.send.data.iov_base = payload;
    cmd.send.data.iov_len = nbytes;
    cmd.send.dest = gasnetc_endpoint(jobrank);
    cmd.send.dispatch = GASNETC_DISP_LONG;
    cmd.send.hints = gasnetc_null_send_hint;

    /* Lock is held in handler context */
    if (send_imm) {
        rc = PAMI_Send_immediate(gasnetc_context, &cmd.send);
        GASNETC_PAMI_CHECK(rc, "from PAMI_Send_immediate(AMReplyLong)");
    } else {
        rc = PAMI_Send(gasnetc_context, &cmd);
        GASNETC_PAMI_CHECK(rc, "from PAMI_Send(AMReplyLong)");
    }
  }
  return retval;
}

extern int gasnetc_AMReplyLongV(
                gex_Token_t token, gex_AM_Index_t handler,
                /*const*/ void *source_addr, size_t nbytes, void *dest_addr,
                gex_Event_t *lc_opt, gex_Flags_t flags,
                int numargs, va_list argptr)
{
  return gasnetc_AMReplyLong(token,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs,argptr);
}

extern int gasnetc_AMReplyLongM(
                gex_Token_t token,
                gex_AM_Index_t handler,
                /*const*/ void *source_addr,
                size_t nbytes,
                void *dest_addr,
                gex_Event_t *lc_opt,
                gex_Flags_t flags,
                int numargs, ...)
{
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
/* Completion counter call-backs */

/* callback to increment a simple (non-atomic) counter */
extern void gasnetc_cb_inc_uint(pami_context_t context, void *cookie, pami_result_t status) {
  volatile unsigned int *counter_p = (volatile unsigned int *)cookie;
  ++(*counter_p);
}

/* callback to increment an atomic counter */
extern void gasnetc_cb_inc_atomic(pami_context_t context, void *cookie, pami_result_t status) {
  gasneti_weakatomic_t *counter_p = (gasneti_weakatomic_t *)cookie;
  gasneti_weakatomic_increment(counter_p, 0);
}

/* callback to increment an atomic counter, with RELease */
extern void gasnetc_cb_inc_release(pami_context_t context, void *cookie, pami_result_t status) {
  gasneti_weakatomic_t *counter_p = (gasneti_weakatomic_t *)cookie;
  gasneti_weakatomic_increment(counter_p, GASNETI_ATOMIC_REL);
}

/* callback to free the cookie */
extern void gasnetc_cb_free(pami_context_t context, void *cookie, pami_result_t status) {
  gasneti_free(cookie);
}

/* ------------------------------------------------------------------------------------ */
