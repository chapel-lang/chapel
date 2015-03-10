/*   $Source: bitbucket.org:berkeleylab/gasnet.git/pami-conduit/gasnet_core.c $
 * Description: GASNet PAMI conduit Implementation
 * Copyright 2012, Lawrence Berkeley National Laboratory
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#include <gasnet_core_internal.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>

GASNETI_IDENT(gasnetc_IdentString_Version, "$GASNetCoreLibraryVersion: " GASNET_CORE_VERSION_STR " $");
GASNETI_IDENT(gasnetc_IdentString_Name,    "$GASNetCoreLibraryName: " GASNET_CORE_NAME_STR " $");

gasnet_handlerentry_t const *gasnetc_get_handlertable(void);

gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS]; /* handler table (recommended impl) */

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

static int gasnetc_init(int *argc, char ***argv) {
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
     * The 1st argument is normally "&gasnetc_bootstrapExchange" (described below).
     * The 2nd argument is the amount of shared memory space needed for any
     * conduit-specific uses.  The return value is a pointer to the space
     * requested by the 2nd argument.
     */
    (void) gasneti_pshm_init(&gasnetc_bootstrapExchange, 0);
  #endif

  /* TODO: when PSHM support is enabled, consider moving table to shared mem */
  { int i;
    gasnetc_endpoint_tbl = gasneti_malloc(gasneti_nodes * sizeof(pami_endpoint_t));
    for (i = 0; i < gasneti_nodes; ++i) {
      gasnetc_endpoint_tbl[i] = PAMI_ENDPOINT_NULL;
    }
  }

  #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    { 
      /* TODO: probe max memory registration */
      uintptr_t limit = gasneti_mmapLimit((uintptr_t)-1, (uint64_t)-1,
                                          &gasnetc_bootstrapExchange,
                                          &gasnetc_bootstrapBarrier);
      gasneti_segmentInit(limit, &gasnetc_bootstrapExchange);
    }
  #elif GASNET_SEGMENT_EVERYTHING
    /* segment is everything - nothing to do */
  #else
    #error Bad segment config
  #endif

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

  gasneti_auxseg_init(); /* adjust max seg values based on auxseg */

  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
extern int gasnet_init(int *argc, char ***argv) {
  int retval = gasnetc_init(argc, argv);
  if (retval != GASNET_OK) GASNETI_RETURN(retval);
#if 0 /* was done in gasnetc_init() to allow tracing of init steps */
  gasneti_trace_init(argc, argv);
#endif
  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
static char checkuniqhandler[256] = { 0 };
static int gasnetc_reghandlers(gasnet_handlerentry_t *table, int numentries,
                               int lowlimit, int highlimit,
                               int dontcare, int *numregistered) {
  int i;
  *numregistered = 0;
  for (i = 0; i < numentries; i++) {
    int newindex;

    if ((table[i].index == 0 && !dontcare) || 
        (table[i].index && dontcare)) continue;
    else if (table[i].index) newindex = table[i].index;
    else { /* deterministic assignment of dontcare indexes */
      for (newindex = lowlimit; newindex <= highlimit; newindex++) {
        if (!checkuniqhandler[newindex]) break;
      }
      if (newindex > highlimit) {
        char s[255];
        snprintf(s, sizeof(s), "Too many handlers. (limit=%i)", highlimit - lowlimit + 1);
        GASNETI_RETURN_ERRR(BAD_ARG, s);
      }
    }

    /*  ensure handlers fall into the proper range of pre-assigned values */
    if (newindex < lowlimit || newindex > highlimit) {
      char s[255];
      snprintf(s, sizeof(s), "handler index (%i) out of range [%i..%i]", newindex, lowlimit, highlimit);
      GASNETI_RETURN_ERRR(BAD_ARG, s);
    }

    /* discover duplicates */
    if (checkuniqhandler[newindex] != 0) 
      GASNETI_RETURN_ERRR(BAD_ARG, "handler index not unique");
    checkuniqhandler[newindex] = 1;

    /* register the handler */
    gasnetc_handler[(gasnet_handler_t)newindex] = (gasneti_handler_fn_t)table[i].fnptr;

    /* The check below for !table[i].index is redundant and present
     * only to defeat the over-aggressive optimizer in pathcc 2.1
     */
    if (dontcare && !table[i].index) table[i].index = newindex;

    (*numregistered)++;
  }
  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int gasnetc_attach(gasnet_handlerentry_t *table, int numentries,
                          uintptr_t segsize, uintptr_t minheapoffset) {
  void *segbase = NULL;
  
  GASNETI_TRACE_PRINTF(C,("gasnetc_attach(table (%i entries), segsize=%lu, minheapoffset=%lu)",
                          numentries, (unsigned long)segsize, (unsigned long)minheapoffset));

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
    if ((minheapoffset % GASNET_PAGESIZE) != 0) /* round up the minheapoffset to page sz */
      minheapoffset = ((minheapoffset / GASNET_PAGESIZE) + 1) * GASNET_PAGESIZE;
  #else
    segsize = 0;
    minheapoffset = 0;
  #endif

  segsize = gasneti_auxseg_preattach(segsize); /* adjust segsize for auxseg reqts */

  /* ------------------------------------------------------------------------------------ */
  /*  register handlers */
  { int i;
    for (i = 0; i < GASNETC_MAX_NUMHANDLERS; i++) 
      gasnetc_handler[i] = (gasneti_handler_fn_t)&gasneti_defaultAMHandler;
  }
  { /*  core API handlers */
    gasnet_handlerentry_t *ctable = (gasnet_handlerentry_t *)gasnetc_get_handlertable();
    int len = 0;
    int numreg = 0;
    gasneti_assert(ctable);
    while (ctable[len].fnptr) len++; /* calc len */
    if (gasnetc_reghandlers(ctable, len, 1, 63, 0, &numreg) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering core API handlers");
    gasneti_assert(numreg == len);
  }

  { /*  extended API handlers */
    gasnet_handlerentry_t *etable = (gasnet_handlerentry_t *)gasnete_get_handlertable();
    int len = 0;
    int numreg = 0;
    gasneti_assert(etable);
    while (etable[len].fnptr) len++; /* calc len */
    if (gasnetc_reghandlers(etable, len, 64, 127, 0, &numreg) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering extended API handlers");
    gasneti_assert(numreg == len);
  }

  if (table) { /*  client handlers */
    int numreg1 = 0;
    int numreg2 = 0;

    /*  first pass - assign all fixed-index handlers */
    if (gasnetc_reghandlers(table, numentries, 128, 255, 0, &numreg1) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering fixed-index client handlers");

    /*  second pass - fill in dontcare-index handlers */
    if (gasnetc_reghandlers(table, numentries, 128, 255, 1, &numreg2) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering fixed-index client handlers");

    gasneti_assert(numreg1 + numreg2 == numentries);
  }

  /* ------------------------------------------------------------------------------------ */
  /*  register fatal signal handlers */

  /* catch fatal signals and convert to SIGQUIT */
  gasneti_registerSignalHandlers(gasneti_defaultSignalHandler);

  /*  (###) register any custom signal handlers required by your conduit 
   *        (e.g. to support interrupt-based messaging)
   */

  /* ------------------------------------------------------------------------------------ */
  /*  register segment  */

  gasneti_seginfo = (gasnet_seginfo_t *)gasneti_malloc(gasneti_nodes*sizeof(gasnet_seginfo_t));
  gasneti_leak(gasneti_seginfo);

  #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    if (segsize == 0) segbase = NULL; /* no segment */
    else {
      /* (###) add code here to choose and register a segment 
         (ensuring alignment across all nodes if this conduit sets GASNET_ALIGNED_SEGMENTS==1) 
         you can use gasneti_segmentAttach() here if you used gasneti_segmentInit() above
      */
      gasneti_segmentAttach(segsize, minheapoffset, gasneti_seginfo, &gasnetc_bootstrapExchange);
      segbase = gasneti_seginfo[gasneti_mynode].addr;
      segsize = gasneti_seginfo[gasneti_mynode].size;

      gasneti_assert(((uintptr_t)segbase) % GASNET_PAGESIZE == 0);
      gasneti_assert(segsize % GASNET_PAGESIZE == 0);

      /* After local segment is attached, call optional client-provided hook
         (###) should call BEFORE any conduit-specific pinning/registration of the segment
       */
      if (gasnet_client_attach_hook) {
        gasnet_client_attach_hook(segbase, segsize);
      }

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
        gasnetc_bootstrapExchange(&gasnetc_mymemreg, sizeof(pami_memregion_t), gasnetc_memreg);
      }
    }
  #else
  { /* GASNET_SEGMENT_EVERYTHING */
    gasnet_node_t i;
    for (i=0; i<gasneti_nodes; i++) {
      gasneti_seginfo[i].addr = (void *)0;
      gasneti_seginfo[i].size = (uintptr_t)-1;
    }
    segbase = (void *)0;
    segsize = (uintptr_t)-1;
    /* (###) add any code here needed to setup GASNET_SEGMENT_EVERYTHING support */

    /* After local segment is attached, call optional client-provided hook
       (###) should call BEFORE any conduit-specific pinning/registration of the segment
     */
    if (gasnet_client_attach_hook) {
      gasnet_client_attach_hook(segbase, segsize);
    }
  }
  #endif

  /* ------------------------------------------------------------------------------------ */
  /*  gather segment information */

  /* (###) add code here to gather the segment assignment info into 
           gasneti_seginfo on each node (may be possible to use AMShortRequest here)
   */

  /* ------------------------------------------------------------------------------------ */
  /*  primary attach complete */
  gasneti_attach_done = 1;
  gasnetc_bootstrapBarrier();

  GASNETI_TRACE_PRINTF(C,("gasnetc_attach(): primary attach complete"));

  gasneti_assert(gasneti_seginfo[gasneti_mynode].addr == segbase &&
         gasneti_seginfo[gasneti_mynode].size == segsize);

  gasneti_auxseg_attach(); /* provide auxseg */

  gasnete_init(); /* init the extended API */

  gasneti_nodemapFini();

  /* ensure extended API is initialized across nodes */
  gasnetc_bootstrapBarrier();

  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
#if HAVE_ON_EXIT
static void gasnetc_on_exit(int exitcode, void *arg) {
    gasnetc_exit(exitcode);
}
#else
static void gasnetc_atexit(void) {
    gasnetc_exit(0);
}
#endif

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

#if HAVE_ON_EXIT
  on_exit(gasnetc_on_exit, NULL);
#else
  atexit(gasnetc_atexit);
#endif

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
/*
  Misc. Active Message Functions
  ==============================
*/
#if GASNET_PSHM
/* (###) GASNETC_GET_HANDLER
 *   If your conduit will support PSHM, then there needs to be a way
 *   for PSHM to see your handler table.  If you use the recommended
 *   implementation (gasnetc_handler[]) then you don't need to do
 *   anything special.  Othwerwise, #define GASNETC_GET_HANDLER in
 *   gasnet_core_fwd.h and implement gasnetc_get_handler() here, or
 *   as a macro or inline in gasnet_core_internal.h
 *
 * (###) GASNETC_TOKEN_CREATE
 *   If your conduit will support PSHM, then there needs to be a way
 *   for the conduit-specific and PSHM token spaces to co-exist.
 *   The default PSHM implementation produces tokens with the least-
 *   significant bit set and assumes the conduit never will.  If that
 *   is true, you don't need to do anything special here.
 *   If your conduit cannot use the default PSHM token code, then
 *   #define GASNETC_TOKEN_CREATE in gasnet_core_fwd.h and implement
 *   the associated routines described in gasnet_pshm.h.  That code
 *   could be functions located here, or could be macros or inlines
 *   in gasnet_core_internal.h.
 */
#endif

static pami_send_hint_t gasnetc_null_send_hint;
static gasnet_node_t *gasnetc_loopback_token = &gasneti_mynode;
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
typedef union {
    gasnetc_genmsg_t      generic;
    gasnetc_shortmsg_t    shortmsg;
    gasnetc_medmsg_t      medmsg;
    gasnetc_longmsg_t     longmsg;
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
  const int is_req = ((gasnetc_token_t *)cookie)->generic.is_req;
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
  const int is_req = ((gasnetc_token_t *)cookie)->generic.is_req;
  gasnetc_put_big_token(cookie);
  if (is_req) gasnetc_put_request_credit();
}

GASNETI_ALWAYS_INLINE(run_short)
void run_short(gasnetc_token_t *token) {
  gasnetc_shortmsg_t            *header = &token->shortmsg;
  const int                      is_req = header->is_req;
  const gasnet_handler_t     handler_id = header->handler;
  const gasneti_handler_fn_t handler_fn = gasnetc_handler[handler_id];
  const gasnet_handlerarg_t       *args = header->args;
  const int                     numargs = header->numargs;

#if GASNET_DEBUG
  /* We copy generic portion to allow writes to rep_sent */
  gasnetc_genmsg_t local_token = token->generic;
  local_token.rep_sent = 0;
  token = (gasnetc_token_t *)(&local_token);
#endif

  GASNETI_RUN_HANDLER_SHORT(is_req,handler_id,handler_fn,token,args,numargs);
}

GASNETI_ALWAYS_INLINE(run_medium)
void run_medium(gasnetc_token_t *token) {
  gasnetc_medmsg_t              *header = &token->medmsg;
  const int                      is_req = header->is_req;
  const gasnet_handler_t     handler_id = header->handler;
  const gasneti_handler_fn_t handler_fn = gasnetc_handler[handler_id];
  const gasnet_handlerarg_t       *args = header->args;
  const int                     numargs = header->numargs;
  void * const                     data = GASNETC_TOKEN_PAYLOAD(token);
  const size_t                   nbytes = header->nbytes;

#if GASNET_DEBUG
  header->rep_sent = 0;
#endif
  GASNETI_RUN_HANDLER_MEDIUM(is_req,handler_id,handler_fn,token,args,numargs,data,nbytes);
}

GASNETI_ALWAYS_INLINE(run_long)
void run_long(gasnetc_token_t *token) {
  gasnetc_longmsg_t             *header = &token->longmsg;
  const int                      is_req = header->is_req;
  const gasnet_handler_t     handler_id = header->handler;
  const gasneti_handler_fn_t handler_fn = gasnetc_handler[handler_id];
  const gasnet_handlerarg_t       *args = header->args;
  const int                     numargs = header->numargs;
  void * const                     data = (void*)header->addr;
  const size_t                   nbytes = header->nbytes;

#if GASNET_DEBUG
  header->rep_sent = 0;
#endif
  GASNETI_RUN_HANDLER_LONG(is_req,handler_id,handler_fn,token,args,numargs,data,nbytes);
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

  run_short((gasnetc_token_t *)head_addr);
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

  memcpy(token, head_addr, head_size);

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
    memcpy(token, head_addr, head_size);
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
    memcpy(token, head_addr, head_size);
    /* instruct PAMI how to deliver payload */
    recv->cookie = token;
    recv->local_fn = &am_Long_event;
    recv->addr = data;
#if 0 /* the hints recv_contiguous and recv_copy ensure we can ignore these */
    recv->type = PAMI_TYPE_BYTE;
    recv->offset = 0;
    recv->data_fn = PAMI_DATA_COPY;
    recv->data_cookie = NULL;
#endif
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

  gasneti_assert_always(GASNETC_MAX_MED_RESRV >= sizeof(gasnetc_token_t));

  conf[0].name = PAMI_CONTEXT_DISPATCH_ID_MAX;
  rc = PAMI_Context_query(gasnetc_context, conf, 1);
  GASNETC_PAMI_CHECK(rc, "calling PAMI_Context_query() for DISPATCH_ID_MAX");
  GASNETI_TRACE_PRINTF(C,("DISPATCH_ID_MAX = %ld", (long)conf[0].value.intval));
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
  GASNETI_TRACE_PRINTF(C,("PAMI_DISPATCH_SEND_IMMEDIATE_MAX = %ld",
                          (long)gasnetc_send_imm_max));
  gasnetc_recv_imm_max = conf[1].value.intval;
  GASNETI_TRACE_PRINTF(C,("PAMI_DISPATCH_RECV_IMMEDIATE_MAX = %ld",
                          (long)gasnetc_recv_imm_max));

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
                             (sizeof(gasnetc_medmsg_t) + gasnet_AMMaxMedium()))
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

  return GASNET_OK;
}

extern int gasnetc_AMGetMsgSource(gasnet_token_t token, gasnet_node_t *srcindex) {
  gasnet_node_t sourceid;
  GASNETI_CHECKATTACH();
  GASNETI_CHECK_ERRR((!token),BAD_ARG,"bad token");
  GASNETI_CHECK_ERRR((!srcindex),BAD_ARG,"bad src ptr");

#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let the PSHM code
   * have a chance to recognize the token first, as shown here. */
  if (gasneti_AMPSHMGetMsgSource(token, &sourceid) != GASNET_OK)
#endif
  {
    /* (###) add code here to write the source index into sourceid. */
    sourceid = *(gasnet_node_t *)token;
  }

  gasneti_assert(sourceid < gasneti_nodes);
  *srcindex = sourceid;
  return GASNET_OK;
}

extern int gasnetc_AMPoll(void) {
  int retval;
  GASNETI_CHECKATTACH();

#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it make progress here. */
  gasneti_AMPSHMPoll(0);
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
      (_args)[_i] = va_arg(_argptr, gasnet_handlerarg_t);        \
    }                                                            \
  } while (0)

#if GASNET_DEBUG
  #define GASNETC_AM_VALIDATE_TOKEN(_cat,_token) do {            \
      gasnetc_token_t *real_token = (gasnetc_token_t *)(_token); \
      gasneti_assert(real_token->_cat##msg.is_req);              \
      gasneti_assert(!real_token->_cat##msg.rep_sent);           \
      real_token->_cat##msg.rep_sent = 1;                        \
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

extern int gasnetc_AMRequestShortM( 
                            gasnet_node_t dest,       /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            int numargs, ...) {
  int retval = GASNET_OK;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTSHORT(dest,handler,numargs);
  gasneti_AMPoll();
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it check the dest first. */
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Short, dest, handler,
                                           0, 0, 0,
                                           numargs, argptr);
  } else
#else
  if (dest == gasneti_mynode) {
    const gasneti_handler_fn_t handler_fn = gasnetc_handler[handler];
    gasnet_handlerarg_t args[GASNETC_MAX_ARGS];
    GASNETC_AM_COPY_ARGS(args, numargs, argptr);
    GASNETI_RUN_HANDLER_SHORT(1,handler,handler_fn,gasnetc_loopback_token,args,numargs);
  } else
#endif
  {
    /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
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
    cmd.dest = gasnetc_endpoint(dest);
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
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestMediumM( 
                            gasnet_node_t dest,      /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            int numargs, ...) {
  int retval = GASNET_OK;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTMEDIUM(dest,handler,source_addr,nbytes,numargs);
  gasneti_AMPoll();
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it check the dest first. */
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Medium, dest, handler,
                                           source_addr, nbytes, 0,
                                           numargs, argptr);
  } else
#else
  if (dest == gasneti_mynode) {
    const gasneti_handler_fn_t handler_fn = gasnetc_handler[handler];
    gasnet_handlerarg_t args[GASNETC_MAX_ARGS];
    void *dest_addr = alloca(nbytes); 
    gasneti_assert(0 == ((uintptr_t)dest_addr % GASNETI_MEDBUF_ALIGNMENT));
    GASNETC_AM_COPY_ARGS(args, numargs, argptr);
    memcpy(dest_addr, source_addr, nbytes);
    GASNETI_RUN_HANDLER_MEDIUM(1,handler,handler_fn,gasnetc_loopback_token,args,numargs,dest_addr,nbytes);
  } else
#endif
  {
    /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */
    /* TODO: send in-place if fits w/i immediate limit */

    pami_send_t cmd;
    pami_result_t rc;
    gasnetc_medmsg_t *msg_p = &(gasnetc_get_big_token()->medmsg);
    char * payload = GASNETC_TOKEN_PAYLOAD(msg_p);

    GASNETC_AM_MSG_COMMON((*msg_p), handler, numargs, argptr, 1);
    msg_p->nbytes = nbytes;
    memcpy(payload, source_addr, nbytes);

    /* TODO: If register bounce buffers, then apply appropriate hint (here or at Dispatch_set?) */
    cmd.send.header.iov_base = (char *)msg_p;
    cmd.send.header.iov_len = GASNETC_ARGSEND(med, numargs);
    cmd.send.data.iov_base = payload;
    cmd.send.data.iov_len = nbytes;
    cmd.send.dest = gasnetc_endpoint(dest);
    cmd.send.dispatch = GASNETC_DISP_MED;
    cmd.send.hints = gasnetc_null_send_hint;
    cmd.events.cookie = (void*)msg_p;
    cmd.events.local_fn = &gasnetc_cb_big_token;
    cmd.events.remote_fn = NULL;

    gasnetc_get_request_credit();

    GASNETC_PAMI_LOCK(gasnetc_context);
    rc = PAMI_Send(gasnetc_context, &cmd);
    GASNETC_PAMI_CHECK(rc, "from PAMI_Send(AMRequestMedium)");
    rc = PAMI_Context_advance(gasnetc_context, 1);
    GASNETC_PAMI_CHECK_ADVANCE(rc, "advancing an AMRequestMedium");
    GASNETC_PAMI_UNLOCK(gasnetc_context);
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestLongM( gasnet_node_t dest,        /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  int retval = GASNET_OK;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTLONG(dest,handler,source_addr,nbytes,dest_addr,numargs);
  gasneti_AMPoll();
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it check the dest first. */
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Long, dest, handler,
                                           source_addr, nbytes, dest_addr,
                                           numargs, argptr);
  } else
#else
  if (dest == gasneti_mynode) {
    const gasneti_handler_fn_t handler_fn = gasnetc_handler[handler];
    gasnet_handlerarg_t args[GASNETC_MAX_ARGS];
    GASNETC_AM_COPY_ARGS(args, numargs, argptr);
    memcpy(dest_addr, source_addr, nbytes);
    GASNETI_RUN_HANDLER_LONG(1,handler,handler_fn,gasnetc_loopback_token,args,numargs,dest_addr,nbytes);
  } else
#endif
  {
    /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */
    pami_send_t cmd;
    pami_result_t rc;
    gasnetc_longmsg_t msg;
    volatile unsigned int counter = 0;

    GASNETC_AM_MSG_COMMON(msg, handler, numargs, argptr, 1);
    msg.addr = (uintptr_t)dest_addr;
    msg.nbytes = nbytes;

    /* TODO: Register segment and apply appropriate hint(s) here or at Dispatch_set */
    cmd.send.header.iov_base = (char *)&msg;
    cmd.send.header.iov_len = GASNETC_ARGSEND(long, numargs);
    cmd.send.data.iov_base = (char *)source_addr;
    cmd.send.data.iov_len = nbytes;
    cmd.send.dest = gasnetc_endpoint(dest);
    cmd.send.dispatch = GASNETC_DISP_LONG;
    cmd.send.hints = gasnetc_null_send_hint;
    cmd.events.cookie = (void*)&counter;
    cmd.events.local_fn = &gasnetc_cb_inc_uint;
    cmd.events.remote_fn = NULL;

    gasnetc_get_request_credit();

    GASNETC_PAMI_LOCK(gasnetc_context);
    rc = PAMI_Send(gasnetc_context, &cmd);
    GASNETC_PAMI_CHECK(rc, "from PAMI_Send(AMRequestLong)");
    rc = PAMI_Context_advance(gasnetc_context, 1);
    GASNETC_PAMI_CHECK_ADVANCE(rc, "advancing an AMRequestLong");
    GASNETC_PAMI_UNLOCK(gasnetc_context);

    gasneti_polluntil(counter == 1); /* stall for local completion */
    gasnetc_put_request_credit();
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestLongAsyncM( gasnet_node_t dest,        /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  int retval = GASNET_OK;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTLONGASYNC(dest,handler,source_addr,nbytes,dest_addr,numargs);
  gasneti_AMPoll();
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it check the dest first. */
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Long, dest, handler,
                                           source_addr, nbytes, dest_addr,
                                           numargs, argptr);
  } else
#else
  if (dest == gasneti_mynode) {
    const gasneti_handler_fn_t handler_fn = gasnetc_handler[handler];
    gasnet_handlerarg_t args[GASNETC_MAX_ARGS];
    GASNETC_AM_COPY_ARGS(args, numargs, argptr);
    memcpy(dest_addr, source_addr, nbytes);
    GASNETI_RUN_HANDLER_LONG(1,handler,handler_fn,gasnetc_loopback_token,args,numargs,dest_addr,nbytes);
  } else
#endif
  {
    /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */
    pami_send_t cmd;
    pami_result_t rc;
    gasnetc_longmsg_t *msg_p = &(gasnetc_get_token()->longmsg);

    GASNETC_AM_MSG_COMMON((*msg_p), handler, numargs, argptr, 1);
    msg_p->addr = (uintptr_t)dest_addr;
    msg_p->nbytes = nbytes;

    /* TODO: Register segment and apply appropriate hint(s) here or at Dispatch_set */
    cmd.send.header.iov_base = (char *)msg_p;
    cmd.send.header.iov_len = GASNETC_ARGSEND(long, numargs);
    cmd.send.data.iov_base = (char *)source_addr;
    cmd.send.data.iov_len = nbytes;
    cmd.send.dest = gasnetc_endpoint(dest);
    cmd.send.dispatch = GASNETC_DISP_LONG;
    cmd.send.hints = gasnetc_null_send_hint;
    cmd.events.cookie = (void*)msg_p;
    cmd.events.local_fn = &gasnetc_cb_token;
    cmd.events.remote_fn = NULL;

    gasnetc_get_request_credit();

    GASNETC_PAMI_LOCK(gasnetc_context);
    rc = PAMI_Send(gasnetc_context, &cmd);
    GASNETC_PAMI_CHECK(rc, "from PAMI_Send(AMRequestLongAsync)");
    rc = PAMI_Context_advance(gasnetc_context, 1);
    GASNETC_PAMI_CHECK_ADVANCE(rc, "advancing an AMRequestLongAsync");
    GASNETC_PAMI_UNLOCK(gasnetc_context);
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyShortM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            int numargs, ...) {
  int retval = GASNET_OK;
  va_list argptr;
  GASNETI_COMMON_AMREPLYSHORT(token,handler,numargs);
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it check the token first. */
  if_pt (gasnetc_token_is_pshm(token)) {
    retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Short, token, handler,
                                         0, 0, 0,
                                         numargs, argptr);
  } else
#else
  if (token == gasnetc_loopback_token) {
    const gasneti_handler_fn_t handler_fn = gasnetc_handler[handler];
    gasnet_handlerarg_t args[GASNETC_MAX_ARGS];
    GASNETC_AM_COPY_ARGS(args, numargs, argptr);
    GASNETI_RUN_HANDLER_SHORT(0,handler,handler_fn,gasnetc_loopback_token,args,numargs);
  } else
#endif
  { 
    /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */
    pami_send_immediate_t cmd;
    pami_result_t rc;
    gasnetc_shortmsg_t msg;

    gasnet_node_t dest;
    GASNETI_SAFE(gasnetc_AMGetMsgSource(token, &dest));

    GASNETC_AM_VALIDATE_TOKEN(short, token);
    GASNETC_AM_MSG_COMMON(msg, handler, numargs, argptr, 0);

    cmd.header.iov_base = (char *)&msg;
    cmd.header.iov_len = GASNETC_ARGSEND(short, numargs);
    cmd.data.iov_base = NULL;
    cmd.data.iov_len = 0;
    cmd.dest = gasnetc_endpoint(dest);
    cmd.dispatch = GASNETC_DISP_SHORT;
    cmd.hints = gasnetc_null_send_hint;

    /* Lock is held in handler context */
    rc = PAMI_Send_immediate(gasnetc_context, &cmd);
    GASNETC_PAMI_CHECK(rc, "from PAMI_Send_immediate(AMReplyShort)");
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyMediumM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            int numargs, ...) {
  int retval = GASNET_OK;
  va_list argptr;
  GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,numargs);
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it check the token first. */
  if_pt (gasnetc_token_is_pshm(token)) {
    retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Medium, token, handler,
                                         source_addr, nbytes, 0,
                                         numargs, argptr);
  } else
#else
  if (token == gasnetc_loopback_token) {
    const gasneti_handler_fn_t handler_fn = gasnetc_handler[handler];
    gasnet_handlerarg_t args[GASNETC_MAX_ARGS];
    void *dest_addr = alloca(nbytes); 
    gasneti_assert(0 == ((uintptr_t)dest_addr % GASNETI_MEDBUF_ALIGNMENT));
    GASNETC_AM_COPY_ARGS(args, numargs, argptr);
    memcpy(dest_addr, source_addr, nbytes);
    GASNETI_RUN_HANDLER_MEDIUM(0,handler,handler_fn,gasnetc_loopback_token,args,numargs,dest_addr,nbytes);
  } else
#endif
  {
    /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */
    /* TODO: send in-place if fits w/i immediate limit */

    pami_send_t cmd;
    pami_result_t rc;
    gasnetc_medmsg_t *msg_p = &(gasnetc_get_big_token()->medmsg);
    char * payload = GASNETC_TOKEN_PAYLOAD(msg_p);

    gasnet_node_t dest;
    GASNETI_SAFE(gasnetc_AMGetMsgSource(token, &dest));

    GASNETC_AM_VALIDATE_TOKEN(med, token);
    GASNETC_AM_MSG_COMMON((*msg_p), handler, numargs, argptr, 0);
    msg_p->nbytes = nbytes;
    memcpy(payload, source_addr, nbytes);

    /* TODO: If register bounce buffers, then apply appropriate hint (here or at Dispatch_set?) */
    cmd.send.header.iov_base = (char *)msg_p;
    cmd.send.header.iov_len = GASNETC_ARGSEND(med, numargs);
    cmd.send.data.iov_base = payload;
    cmd.send.data.iov_len = nbytes;
    cmd.send.dest = gasnetc_endpoint(dest);
    cmd.send.dispatch = GASNETC_DISP_MED;
    cmd.send.hints = gasnetc_null_send_hint;
    cmd.events.cookie = (void*)msg_p;
    cmd.events.local_fn = &gasnetc_cb_big_token;
    cmd.events.remote_fn = NULL;

    /* Lock is held in handler context */
    rc = PAMI_Send(gasnetc_context, &cmd);
    GASNETC_PAMI_CHECK(rc, "from PAMI_Send(AMReplyMedium)");
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyLongM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  int retval = GASNET_OK;
  va_list argptr;
  GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,numargs); 
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it check the token first. */
  if_pt (gasnetc_token_is_pshm(token)) {
    retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Long, token, handler,
                                         source_addr, nbytes, dest_addr,
                                         numargs, argptr);
  } else
#else
  if (token == gasnetc_loopback_token) {
    const gasneti_handler_fn_t handler_fn = gasnetc_handler[handler];
    gasnet_handlerarg_t args[GASNETC_MAX_ARGS];
    GASNETC_AM_COPY_ARGS(args, numargs, argptr);
    memcpy(dest_addr, source_addr, nbytes);
    GASNETI_RUN_HANDLER_LONG(0,handler,handler_fn,gasnetc_loopback_token,args,numargs,dest_addr,nbytes);
  } else
#endif
  {
    /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */
    /* TODO: send in-place if fits w/i immediate limit */

    pami_send_t cmd;
    pami_result_t rc;
    gasnetc_longmsg_t *msg_p = &(gasnetc_get_big_token()->longmsg);
    char * payload = GASNETC_TOKEN_PAYLOAD(msg_p);

    gasnet_node_t dest;
    GASNETI_SAFE(gasnetc_AMGetMsgSource(token, &dest));

    GASNETC_AM_VALIDATE_TOKEN(long, token);
    GASNETC_AM_MSG_COMMON((*msg_p), handler, numargs, argptr, 0);
    msg_p->nbytes = nbytes;
    msg_p->addr = (uintptr_t)dest_addr;
    memcpy(payload, source_addr, nbytes);

    /* TODO: If register bounce buffers, then apply appropriate hint (here or at Dispatch_set?) */
    cmd.send.header.iov_base = (char *)msg_p;
    cmd.send.header.iov_len = GASNETC_ARGSEND(long, numargs);
    cmd.send.data.iov_base = payload;
    cmd.send.data.iov_len = nbytes;
    cmd.send.dest = gasnetc_endpoint(dest);
    cmd.send.dispatch = GASNETC_DISP_LONG;
    cmd.send.hints = gasnetc_null_send_hint;
    cmd.events.cookie = (void*)msg_p;
    cmd.events.local_fn = &gasnetc_cb_big_token;
    cmd.events.remote_fn = NULL;

    /* Lock is held in handler context */
    rc = PAMI_Send(gasnetc_context, &cmd);
    GASNETC_PAMI_CHECK(rc, "from PAMI_Send(AMReplyLong)");
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

/* ------------------------------------------------------------------------------------ */
/*
  No-interrupt sections
  =====================
  This section is only required for conduits that may use interrupt-based handler dispatch
  See the GASNet spec and http://www.cs.berkeley.edu/~bonachea/upc/gasnet.html for
    philosophy and hints on efficiently implementing no-interrupt sections
  Note: the extended-ref implementation provides a thread-specific void* within the 
    gasnete_threaddata_t data structure which is reserved for use by the core 
    (and this is one place you'll probably want to use it)
*/
#if GASNETC_USE_INTERRUPTS
  #error interrupts not implemented
  extern void gasnetc_hold_interrupts(void) {
    GASNETI_CHECKATTACH();
    /* add code here to disable handler interrupts for _this_ thread */
  }
  extern void gasnetc_resume_interrupts(void) {
    GASNETI_CHECKATTACH();
    /* add code here to re-enable handler interrupts for _this_ thread */
  }
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Handler-safe locks
  ==================
*/
#if !GASNETC_NULL_HSL
extern void gasnetc_hsl_init   (gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();
  gasneti_mutex_init(&(hsl->lock));

  #if GASNETC_USE_INTERRUPTS
    /* add code here to init conduit-specific HSL state */
    #error interrupts not implemented
  #endif
}

extern void gasnetc_hsl_destroy(gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();
  gasneti_mutex_destroy(&(hsl->lock));

  #if GASNETC_USE_INTERRUPTS
    /* add code here to cleanup conduit-specific HSL state */
    #error interrupts not implemented
  #endif
}

extern void gasnetc_hsl_lock   (gasnet_hsl_t *hsl) {
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

  #if GASNETC_USE_INTERRUPTS
    /* conduits with interrupt-based handler dispatch need to add code here to 
       disable handler interrupts on _this_ thread, (if this is the outermost
       HSL lock acquire and we're not inside an enclosing no-interrupt section)
     */
    #error interrupts not implemented
  #endif
}

extern void gasnetc_hsl_unlock (gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();

  #if GASNETC_USE_INTERRUPTS
    /* conduits with interrupt-based handler dispatch need to add code here to 
       re-enable handler interrupts on _this_ thread, (if this is the outermost
       HSL lock release and we're not inside an enclosing no-interrupt section)
     */
    #error interrupts not implemented
  #endif

  GASNETI_TRACE_EVENT_TIME(L, HSL_UNLOCK, GASNETI_TICKS_NOW_IFENABLED(L)-hsl->acquiretime);

  gasneti_mutex_unlock(&(hsl->lock));
}

extern int  gasnetc_hsl_trylock(gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();

  {
    int locked = (gasneti_mutex_trylock(&(hsl->lock)) == 0);

    GASNETI_TRACE_EVENT_VAL(L, HSL_TRYLOCK, locked);
    if (locked) {
      #if GASNETI_STATS_OR_TRACE
        hsl->acquiretime = GASNETI_TICKS_NOW_IFENABLED(L);
      #endif
      #if GASNETC_USE_INTERRUPTS
        /* conduits with interrupt-based handler dispatch need to add code here to 
           disable handler interrupts on _this_ thread, (if this is the outermost
           HSL lock acquire and we're not inside an enclosing no-interrupt section)
         */
        #error interrupts not implemented
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
static gasnet_handlerentry_t const gasnetc_handlers[] = {
  #ifdef GASNETC_AUXSEG_HANDLERS
    GASNETC_AUXSEG_HANDLERS(),
  #endif
  /* ptr-width independent handlers */

  /* ptr-width dependent handlers */

  { 0, NULL }
};

gasnet_handlerentry_t const *gasnetc_get_handlertable(void) {
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
