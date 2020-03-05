/*   $Source: bitbucket.org:berkeleylab/gasnet.git/udp-conduit/gasnet_core.c $
 * Description: GASNet UDP conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_am.h>

#if GASNET_BLCR
#include <gasnet_blcr.h>
#endif

#include <amudp_spmd.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>

GASNETI_IDENT(gasnetc_IdentString_Version, "$GASNetCoreLibraryVersion: " GASNET_CORE_VERSION_STR " $");
GASNETI_IDENT(gasnetc_IdentString_Name,    "$GASNetCoreLibraryName: " GASNET_CORE_NAME_STR " $");

gex_AM_Entry_t const *gasnetc_get_handlertable(void);

gex_AM_Entry_t *gasnetc_handler; // TODO-EX: will be replaced with per-EP tables

// TODO-EX: This is a hack to support multiple segments w/ a single AM EP
#ifndef GASNETC_MOCK_EVERYTHING
#define GASNETC_MOCK_EVERYTHING 1
#endif

static void gasnetc_traceoutput(int);

static uint64_t gasnetc_networkpid;
eb_t gasnetc_bundle;
ep_t gasnetc_endpoint;

gasneti_mutex_t gasnetc_AMlock = GASNETI_MUTEX_INITIALIZER; /*  protect access to AMUDP */
volatile int gasnetc_AMLockYield = 0;

#if GASNET_TRACE || GASNET_DEBUG
  extern void gasnetc_enteringHandler_hook(amudp_category_t cat, int isReq, int handlerId, void *token, 
                                         void *buf, size_t nbytes, int numargs, uint32_t *args);
  extern void gasnetc_leavingHandler_hook(amudp_category_t cat, int isReq);
#endif

#if defined(GASNET_CSPAWN_CMD)
  #define GASNETC_DEFAULT_SPAWNFN C
  GASNETI_IDENT(gasnetc_IdentString_Default_CSpawnCommand, "$GASNetCSpawnCommand: " GASNET_CSPAWN_CMD " $");
#else /* AMUDP implicit ssh startup */
  #define GASNETC_DEFAULT_SPAWNFN S
#endif
GASNETI_IDENT(gasnetc_IdentString_DefaultSpawnFn, "$GASNetDefaultSpawnFunction: " _STRINGIFY(GASNETC_DEFAULT_SPAWNFN) " $");

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* called at startup to check configuration sanity */
static void gasnetc_check_config(void) {
  gasneti_check_config_preinit();

  gasneti_static_assert(GASNET_MAXNODES <= AMUDP_MAX_SPMDPROCS);
  gasneti_static_assert(AMUDP_MAX_NUMHANDLERS >= 256);
  gasneti_static_assert(AMUDP_MAX_SEGLENGTH == (uintptr_t)-1);

  gasneti_static_assert(GASNET_ERR_NOT_INIT == AM_ERR_NOT_INIT);
  gasneti_static_assert(GASNET_ERR_RESOURCE == AM_ERR_RESOURCE);
  gasneti_static_assert(GASNET_ERR_BAD_ARG  == AM_ERR_BAD_ARG);
}

void gasnetc_bootstrapBarrier(void) {
   int retval;
   AM_ASSERT_LOCKED(); /* need this because SPMDBarrier may poll */
   GASNETI_AM_SAFE_NORETURN(retval,AMUDP_SPMDBarrier());
   if_pf (retval) gasneti_fatalerror("failure in gasnetc_bootstrapBarrier()");
}

void gasnetc_bootstrapExchange(void *src, size_t len, void *dest) {
  int retval;
  GASNETI_AM_SAFE_NORETURN(retval,AMUDP_SPMDAllGather(src, dest, len));
  if_pf (retval) gasneti_fatalerror("failure in gasnetc_bootstrapExchange()");
}

#if GASNET_PSHM /* Used only in call to gasneti_pshm_init() */
/* Naive (poorly scaling) "reference" implementation via gasnetc_bootstrapExchange() */
static void gasnetc_bootstrapSNodeBroadcast(void *src, size_t len, void *dest, int rootnode) {
  void *tmp = gasneti_malloc(len * gasneti_nodes);
  gasneti_assert(NULL != src);
  gasnetc_bootstrapExchange(src, len, tmp);
  GASNETI_MEMCPY(dest, (void*)((uintptr_t)tmp + (len * rootnode)), len);
  gasneti_free(tmp);
}
#endif

#define INITERR(type, reason) do {                                      \
   if (gasneti_VerboseErrors) {                                         \
     fprintf(stderr, "GASNet initialization encountered an error: %s\n" \
      "  in %s at %s:%i\n",                                             \
      #reason, GASNETI_CURRENT_FUNCTION,  __FILE__, __LINE__);          \
   }                                                                    \
   retval = GASNET_ERR_ ## type;                                        \
   goto done;                                                           \
 } while (0)

static int gasnetc_init(int *argc, char ***argv, gex_Flags_t flags) {
  int retval = GASNET_OK;

  /*  check system sanity */
  gasnetc_check_config();

  /* --------- begin Master code ------------ */
  if (!AMUDP_SPMDIsWorker(argv?*argv:NULL)) {
    /* assume we're an implicit master 
       (we don't currently support explicit workers spawned 
        without using the AMUDP SPMD API)   
     */
    int num_nodes;
    int i;
    char spawnfn;
    amudp_spawnfn_t fp = (amudp_spawnfn_t)NULL;

    if (!argv) {
      gasneti_fatalerror("implicit-master without argv not supported - use amudprun");
    }

    /* pretend we're node 0, for purposes of verbose env reporting */
    gasneti_init_done = 1;
    gasneti_mynode = 0;

    #if defined(GASNET_CSPAWN_CMD)
    { /* set configure default cspawn cmd */
      const char *cmd = gasneti_getenv_withdefault("GASNET_CSPAWN_CMD",GASNET_CSPAWN_CMD);
      gasneti_setenv("GASNET_CSPAWN_CMD",cmd);
    }
    #endif

    /* parse node count from command line */
    if (*argc < 2) {
      fprintf(stderr, "GASNet: Missing parallel node count\n");
      fprintf(stderr, "GASNet: Specify node count as first argument, or use upcrun/tcrun spawner script to start job\n");
      fprintf(stderr, "GASNet: Usage '%s <num_nodes> {program arguments}'\n", (*argv)[0]);
      exit(-1);
    }
    /*
     * argv[1] is number of nodes; argv[0] is program name; argv is
     * list of arguments including program name and number of nodes.
     * We need to remove argv[1] when the argument array is passed
     * to the tic_main().
     */
    num_nodes = atoi((*argv)[1]);
    if (num_nodes < 1) {
      fprintf (stderr, "GASNet: Invalid number of nodes: %s\n", (*argv)[1]);
      fprintf (stderr, "GASNet: Usage '%s <num_nodes> {program arguments}'\n", (*argv)[0]);
      exit (1);
    }

    /* remove the num_nodes argument */
    for (i = 1; i < (*argc)-1; i++) {
      (*argv)[i] = (*argv)[i+1];
    }
    (*argv)[(*argc)-1] = NULL;
    (*argc)--;

    /* get spawnfn */
    spawnfn = *gasneti_getenv_withdefault("GASNET_SPAWNFN", _STRINGIFY(GASNETC_DEFAULT_SPAWNFN));

    { /* ensure we pass the effective spawnfn to worker env */
      char spawnstr[2];
      spawnstr[0] = toupper(spawnfn);
      spawnstr[1] = '\0';
      gasneti_setenv("GASNET_SPAWNFN",spawnstr);
    }

    /* ensure reliable localhost operation by forcing use of 127.0.0.1
     * setting GASNET_MASTERIP to the empty string will prevent this */
    if (('L' == toupper(spawnfn)) && !gasneti_getenv("GASNET_MASTERIP")) {
      gasneti_setenv("GASNET_MASTERIP","127.0.0.1");
    }

    for (i=0; AMUDP_Spawnfn_Desc[i].abbrev; i++) {
      if (toupper(spawnfn) == toupper(AMUDP_Spawnfn_Desc[i].abbrev)) {
        fp = AMUDP_Spawnfn_Desc[i].fnptr;
        break;
      }
    }

    if (!fp) {
      fprintf (stderr, "GASNet: Invalid spawn function specified in GASNET_SPAWNFN\n");
      fprintf (stderr, "GASNet: The following mechanisms are available:\n");
      for (i=0; AMUDP_Spawnfn_Desc[i].abbrev; i++) {
        fprintf(stderr, "    '%c'  %s\n",  
              toupper(AMUDP_Spawnfn_Desc[i].abbrev), AMUDP_Spawnfn_Desc[i].desc);
      }
      exit(1);
    }

    #if GASNET_DEBUG_VERBOSE
      /* note - can't call trace macros during gasnet_init because trace system not yet initialized */
      fprintf(stderr,"gasnetc_init(): about to spawn...\n"); fflush(stderr);
    #endif

    retval = AMUDP_SPMDStartup(argc, argv, 
      num_nodes, 0, fp,
      NULL, &gasnetc_bundle, &gasnetc_endpoint);
    /* master startup should never return */
    gasneti_fatalerror("master AMUDP_SPMDStartup() failed");
  }

  /* --------- begin Worker code ------------ */
  AMLOCK();
    if (gasneti_init_done) 
      INITERR(NOT_INIT, "GASNet already initialized");

    gasneti_freezeForDebugger();

    AMX_VerboseErrors = gasneti_VerboseErrors;
    AMUDP_SPMDkillmyprocess = gasneti_killmyprocess;

#if GASNETI_CALIBRATE_TSC
    // Early x86*/Linux timer initialization before AMUDP_SPMDStartup()
    //
    // udp-conduit does not support user-provided values for GASNET_TSC_RATE*
    // (which fine-tune timer calibration on x86/Linux).  This is partially due
    // to a dependency cycle at startup with envvar propagation, but more
    // importantly because the retransmission algorithm (and hence all conduit
    // comms) rely on gasnet timers to be accurate (at least approximately), so
    // we don't allow the user to weaken or disable their calibration.
    gasneti_unsetenv("GASNET_TSC_RATE");
    gasneti_unsetenv("GASNET_TSC_RATE_TOLERANCE");
    gasneti_unsetenv("GASNET_TSC_RATE_HARD_TOLERANCE");
    GASNETI_TICKS_INIT();
#endif

    /*  perform job spawn */
    retval = AMUDP_SPMDStartup(argc, argv, 
      0, 0, NULL, /* dummies */
      &gasnetc_networkpid, &gasnetc_bundle, &gasnetc_endpoint);
    if (retval != AM_OK) INITERR(RESOURCE, "slave AMUDP_SPMDStartup() failed");
    gasneti_init_done = 1; /* enable early to allow tracing */

    gasneti_getenv_hook = (/* cast drops const */ gasneti_getenv_fn_t*)&AMUDP_SPMDgetenvMaster;
    gasneti_mynode = AMUDP_SPMDMyProc();
    gasneti_nodes = AMUDP_SPMDNumProcs();

#if !GASNETI_CALIBRATE_TSC
    /* Must init timers after global env, and preferably before tracing */
    GASNETI_TICKS_INIT();
#endif

    /* enable tracing */
    gasneti_trace_init(argc, argv);
    GASNETI_AM_SAFE(AMUDP_SPMDSetExitCallback(gasnetc_traceoutput));

    /* for local spawn, assume we want to wait-block */
    if (gasneti_getenv("GASNET_SPAWNFN") && *gasneti_getenv("GASNET_SPAWNFN") == 'L') { 
      GASNETI_TRACE_PRINTF(C,("setting gasnet_set_waitmode(GASNET_WAIT_BLOCK) for localhost spawn"));
      gasnet_set_waitmode(GASNET_WAIT_BLOCK);
    }

    #if GASNET_DEBUG_VERBOSE
      fprintf(stderr,"gasnetc_init(): spawn successful - node %i/%i starting...\n", 
        gasneti_mynode, gasneti_nodes); fflush(stderr);
    #endif

    void *mynodeid;
    uint64_t local_id;
    if (gasneti_getenv_yesno_withdefault("GASNET_USE_GETHOSTID", 0)) {
      // Use gasneti_gethostid() to construct the nodemap
      mynodeid = NULL;
    } else {
      // Use (hash of) hostname and the local IP address to construct the nodemap
      en_t my_name;
      GASNETI_AM_SAFE( AM_GetTranslationName(gasnetc_endpoint, gasneti_mynode, &my_name) );
      uint64_t csum = gasneti_hosthash();
      local_id = GASNETI_MAKEWORD(GASNETI_HIWORD(csum) ^ GASNETI_LOWORD(csum),
                                  *(uint32_t *)(&my_name.sin_addr));
      mynodeid = &local_id;
    }
    gasneti_nodemapInit(&gasnetc_bootstrapExchange, mynodeid, sizeof(local_id), 0);

    #if GASNET_PSHM
      gasneti_pshm_init(&gasnetc_bootstrapSNodeBroadcast, 0);
    #endif

    uintptr_t mmap_limit;
    #if HAVE_MMAP
    {
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

      mmap_limit = gasneti_segmentLimit((uintptr_t)-1, sharedLimit,
                                  &gasnetc_bootstrapExchange,
                                  &gasnetc_bootstrapBarrier);
    }
    #else
      // TODO-EX: we can at least look at rlimits but such logic belongs in conduit-indep code
      mmap_limit = (intptr_t)-1;
    #endif

    /* allocate and attach an aux segment */

    (void) gasneti_auxsegAttach((uintptr_t)-1, &gasnetc_bootstrapExchange);

    /* determine Max{Local,GLobal}SegmentSize */
    gasneti_segmentInit(mmap_limit, &gasnetc_bootstrapExchange, flags);

    #if GASNET_BLCR
      gasneti_checkpoint_guid = gasnetc_networkpid;
      gasneti_checkpoint_init(NULL);
    #endif

  AMUNLOCK();

  gasneti_assert(retval == GASNET_OK);
  return retval;

done: /*  error return while locked */
  AMUNLOCK();
  GASNETI_RETURN(retval);
}

/* ------------------------------------------------------------------------------------ */
extern void _gasnetc_set_waitmode(int wait_mode) {
  if (wait_mode == GASNET_WAIT_BLOCK) {
    AMUDP_PoliteSync = 1;
  } else {
    AMUDP_PoliteSync = 0;
  }
}
/* ------------------------------------------------------------------------------------ */
extern int gasnetc_amregister(gex_AM_Index_t index, gex_AM_Entry_t *entry) {
  if (AM_SetHandler(gasnetc_endpoint, (handler_t)index, entry->gex_fnptr) != AM_OK)
    GASNETI_RETURN_ERRR(RESOURCE, "AM_SetHandler() failed while registering handlers");
  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
static gasnet_seginfo_t myseg;
static int gasnetc_attach_primary(void) {
  int retval = GASNET_OK;

  AMLOCK();
    /* pause to make sure all nodes have called attach 
       if a node calls gasnet_exit() between init/attach, then this allows us
       to process the AMUDP_SPMD control messages required for job shutdown
     */
    gasnetc_bootstrapBarrier();

    /* ------------------------------------------------------------------------------------ */
    /*  register fatal signal handlers */

    /* catch fatal signals and convert to SIGQUIT */
    gasneti_registerSignalHandlers(gasneti_defaultSignalHandler);

    // register process exit-time hook
    gasneti_registerExitHandler(gasnetc_exit);

    #if GASNET_TRACE || GASNET_DEBUG
     #if !GASNET_DEBUG
      if (GASNETI_TRACE_ENABLED(A))
     #endif
        GASNETI_AM_SAFE(AMUDP_SetHandlerCallbacks(gasnetc_endpoint,
          gasnetc_enteringHandler_hook, gasnetc_leavingHandler_hook));
    #endif

    #if GASNETC_MOCK_EVERYTHING
      retval = AM_SetSeg(gasnetc_endpoint, NULL, (uintptr_t)-1);
      if (retval != AM_OK) INITERR(RESOURCE, "AM_SetSeg() failed");
    #endif

    /* ------------------------------------------------------------------------------------ */
    /*  primary attach complete */
    gasneti_attach_done = 1;
    gasnetc_bootstrapBarrier();
  AMUNLOCK();

  GASNETI_TRACE_PRINTF(C,("gasnetc_attach_primary(): primary attach complete\n"));

  gasnete_init(); /* init the extended API */

  gasneti_nodemapFini();

  /* ensure extended API is initialized across nodes */
  AMLOCK();
    gasnetc_bootstrapBarrier();
  AMUNLOCK();

  gasneti_assert(retval == GASNET_OK);
  return retval;

done: /*  error return while locked */
  AMUNLOCK();
  GASNETI_RETURN(retval);
}
/* ------------------------------------------------------------------------------------ */
static int gasnetc_attach_segment(gex_Segment_t                 *segment_p,
                                  gex_TM_t                      tm,
                                  uintptr_t                     segsize,
                                  gasneti_bootstrapExchangefn_t exchangefn,
                                  gex_Flags_t                   flags) {
    int retval = GASNET_OK;

    /* ------------------------------------------------------------------------------------ */
    /*  register client segment  */

    gasnet_seginfo_t myseg = gasneti_segmentAttach(segment_p, 0, tm, segsize, exchangefn, flags);

#if !GASNETC_MOCK_EVERYTHING
    /*  AMUDP allows arbitrary registration with no further action  */
    if (segsize) {
      retval = AM_SetSeg(gasnetc_endpoint, myseg.addr, myseg.size);
      if (retval != AM_OK) INITERR(RESOURCE, "AM_SetSeg() failed");
    }
#endif

done:
    GASNETI_RETURN(retval);
}
/* ------------------------------------------------------------------------------------ */
// TODO-EX: this is a candidate for factorization (once we understand the per-conduit variations)
extern int gasnetc_attach( gex_TM_t               _tm,
                           gasnet_handlerentry_t  *table,
                           int                    numentries,
                           uintptr_t              segsize)
{
  int retval = GASNET_OK;

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
    if (GASNET_OK != gasnetc_attach_segment(&seg, _tm, segsize, gasneti_defaultExchange, GASNETI_FLAG_INIT_LEGACY))
      GASNETI_RETURN_ERRR(RESOURCE,"Error attaching segment");
  #endif

  AMLOCK();
    /*  register client handlers */
    if (table && gasneti_amregister_legacy(ep->_amtbl, table, numentries) != GASNET_OK)
      INITERR(RESOURCE,"Error registering handlers");
  AMUNLOCK();

  /* ensure everything is initialized across all nodes */
  gasnet_barrier(0, GASNET_BARRIERFLAG_UNNAMED);

  return GASNET_OK;

done: /*  error return while locked */
  AMUNLOCK();
  GASNETI_RETURN(retval);
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
static int gasnetc_exitcalled = 0;
static void gasnetc_traceoutput(int exitcode) {
  if (!gasnetc_exitcalled) {
    gasneti_flush_streams();
    gasneti_trace_finish();
  }
}
extern void gasnetc_trace_finish(void) {
  /* dump AMUDP statistics */
  if (GASNETI_STATS_ENABLED(C) ) {
    const char *statdump;
    int isglobal = 0;
    int retval = 0;
    amudp_stats_t stats = AMUDP_initial_stats;

    /* bug 2181 - lock state is unknown, eg we may be in handler context */
    AMLOCK_CAUTIOUS();

    if (isglobal) {
      /* TODO: tricky bit - if this exit is collective, we can display more interesting and useful
         statistics with collective cooperation. But there's no easy way to know for sure whether
         the exit is collective.
       */

      /* TODO: want a bootstrap barrier here for global stats to ensure network is 
         quiescent, but no way to do this unless we know things are collective */

      if (gasneti_mynode != 0) {
          GASNETI_AM_SAFE_NORETURN(retval, AMUDP_GetEndpointStatistics(gasnetc_endpoint, &stats)); /* get statistics */
        /* TODO: send stats to zero */
      } else {
        amudp_stats_t *remote_stats = NULL;
        /* TODO: gather stats from all nodes */
          GASNETI_AM_SAFE_NORETURN(retval, AMUDP_AggregateStatistics(&stats, remote_stats));
      }
    } else {
        GASNETI_AM_SAFE_NORETURN(retval, AMUDP_GetEndpointStatistics(gasnetc_endpoint, &stats)); /* get statistics */
    }

    if ((gasneti_mynode == 0 || !isglobal) && !retval) {
      GASNETI_STATS_PRINTF(C,("--------------------------------------------------------------------------------"));
      GASNETI_STATS_PRINTF(C,("AMUDP Statistics:"));
      if (!isglobal)
        GASNETI_STATS_PRINTF(C,("*** AMUDP stat dump reflects only local node info, because gasnet_exit is non-collective ***"));
      statdump = AMUDP_DumpStatistics(NULL, &stats, isglobal);
      GASNETI_STATS_PRINTF(C,("\n%s",statdump)); /* note, dump has embedded '%' chars */
      GASNETI_STATS_PRINTF(C,("--------------------------------------------------------------------------------"));
    }
  }
}
extern void gasnetc_fatalsignal_callback(int sig) {
  if (gasnetc_exitcalled) {
  /* if we get a fatal signal during exit, it's almost certainly a signal-safety or UDP shutdown
     issue and not a client bug, so don't bother reporting it verbosely, 
     just die silently
   */
    #if 0
      gasneti_fatalerror("gasnetc_fatalsignal_callback aborting...");
    #endif
    gasneti_killmyprocess(1);
  }
}

extern void gasnetc_exit(int exitcode) {
  /* once we start a shutdown, ignore all future SIGQUIT signals or we risk reentrancy */
  gasneti_reghandler(SIGQUIT, SIG_IGN);
  gasnetc_exitcalled = 1;

  {  /* ensure only one thread ever continues past this point */
    static gasneti_mutex_t exit_lock = GASNETI_MUTEX_INITIALIZER;
    gasneti_mutex_lock(&exit_lock);
  }

  GASNETI_TRACE_PRINTF(C,("gasnet_exit(%i)\n", exitcode));

  gasneti_flush_streams();
  gasneti_trace_finish();
  gasneti_sched_yield();

  /* bug2181: try to prevent races where we exit while other local pthreads are in AMUDP
     can't use a blocking lock here, because may be in a signal context
  */
  AMLOCK_CAUTIOUS();

  AMUDP_SPMDExit(exitcode);
  gasneti_fatalerror("AMUDP_SPMDExit failed!");
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
 */
#endif

GASNETI_INLINE(gasnetc_msgsource)
gex_Rank_t gasnetc_msgsource(gex_Token_t token) {
  #if GASNET_PSHM
    gasneti_assert(! gasnetc_token_in_nbrhd(token));
  #endif
    gasneti_assert(token);

    int tmp; /* AMUDP wants an int, but gex_Rank_t is uint32_t */
    gasneti_assert_zeroret(AMUDP_GetSourceId(token, &tmp));
    gasneti_assert(tmp >= 0);
    gex_Rank_t sourceid = tmp;
    gasneti_assert_uint(sourceid ,<, gasneti_nodes);
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

  gex_TI_t result = 0;

  info->gex_srcrank = gasnetc_msgsource(token);
  result |= GEX_TI_SRCRANK;

  info->gex_ep = gasneti_THUNK_EP;
  result |= GEX_TI_EP;

#if 0 // TODO-EX: need to implement this
  /* (###) add code here to write the address of the handle entry into info->gex_entry (optional) */
  info->gex_entry = ###;
  result |= GEX_TI_ENTRY;
#endif

  return GASNETI_TOKEN_INFO_RETURN(result, info, mask);
}

extern int gasnetc_AMPoll(GASNETI_THREAD_FARG_ALONE) {
  int retval;
  GASNETI_CHECKATTACH();
#if GASNET_PSHM
  gasneti_AMPSHMPoll(0 GASNETI_THREAD_PASS);
#endif
  AMLOCK();
  // In single-nbrhd case never need to poll the network for client AMs.
  // However, we'll still check for control traffic for orderly exit handling.
  if (gasneti_mysupernode.grp_count > 1) {
    GASNETI_AM_SAFE_NORETURN(retval,AM_Poll(gasnetc_bundle));
  } else {
    // TODO-EX: a lock-free peek would allow elimination of a lock cycle
    GASNETI_AM_SAFE_NORETURN(retval,AMUDP_SPMDHandleControlTraffic(NULL));
  }
  AMUNLOCK();
  if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
  else return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
  TODO-EX: "nbrhd" support means we could remove the unreachable loopback paths in AMUDP
*/

GASNETI_INLINE(gasnetc_AMRequestShort)
int gasnetc_AMRequestShort( gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  int retval;
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    GASNETC_IMMEDIATE_MAYBE_POLL(flags); /* poll at least once, to assure forward progress */
    retval = gasnetc_nbrhd_RequestGeneric( gasneti_Short, jobrank, handler,
                                           0, 0, 0,
                                           flags, numargs, argptr GASNETI_THREAD_PASS);
  } else {
    AMLOCK_TOSEND();
      GASNETI_AM_SAFE_NORETURN(retval,
               AMUDP_RequestVA(gasnetc_endpoint, jobrank, handler, 
                               numargs, argptr));
    AMUNLOCK();
    if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
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

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMRequestShort(tm,rank,handler,flags,numargs,argptr GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
}

GASNETI_INLINE(gasnetc_AMRequestMedium)
int gasnetc_AMRequestMedium(gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  int retval;
  gasneti_leaf_finish(lc_opt); // always locally completed
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    GASNETC_IMMEDIATE_MAYBE_POLL(flags); /* poll at least once, to assure forward progress */
    retval = gasnetc_nbrhd_RequestGeneric( gasneti_Medium, jobrank, handler,
                                           source_addr, nbytes, 0,
                                           flags, numargs, argptr GASNETI_THREAD_PASS);
  } else {
    AMLOCK_TOSEND();
      GASNETI_AM_SAFE_NORETURN(retval,
               AMUDP_RequestIVA(gasnetc_endpoint, jobrank, handler, 
                                source_addr, nbytes, 
                                numargs, argptr));
    AMUNLOCK();
    if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
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

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMRequestMedium(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
}

GASNETI_INLINE(gasnetc_AMRequestLong)
int gasnetc_AMRequestLong(  gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes, void *dest_addr,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  int retval;
  gasneti_leaf_finish(lc_opt); // always locally completed
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
      GASNETC_IMMEDIATE_MAYBE_POLL(flags); /* poll at least once, to assure forward progress */
      retval = gasnetc_nbrhd_RequestGeneric( gasneti_Long, jobrank, handler,
                                             source_addr, nbytes, dest_addr,
                                             flags, numargs, argptr GASNETI_THREAD_PASS);
  } else {
    uintptr_t dest_offset;
#if GASNETC_MOCK_EVERYTHING
    dest_offset = (uintptr_t)dest_addr;
#else
    dest_offset = ((uintptr_t)dest_addr) - (uintptr_t)gasneti_client_seginfo(tm, rank)->addr;
#endif

    AMLOCK_TOSEND();
      GASNETI_AM_SAFE_NORETURN(retval,
               AMUDP_RequestXferVA(gasnetc_endpoint, jobrank, handler, 
                                   source_addr, nbytes, 
                                   dest_offset, 0,
                                   numargs, argptr));
    AMUNLOCK();
    if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
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
    AM_ASSERT_LOCKED();
    GASNETI_AM_SAFE_NORETURN(retval,
              AMUDP_ReplyVA(token, handler, numargs, argptr));
    if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
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

GASNETI_INLINE(gasnetc_AMReplyMedium)
int gasnetc_AMReplyMedium(  gex_Token_t token, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  int retval;
  gasneti_leaf_finish(lc_opt); // always locally completed
  if_pt (gasnetc_token_in_nbrhd(token)) {
       retval = gasnetc_nbrhd_ReplyGeneric( gasneti_Medium, token, handler,
                                            source_addr, nbytes, 0,
                                            flags, numargs, argptr);
  } else {
    AM_ASSERT_LOCKED();
    GASNETI_AM_SAFE_NORETURN(retval,
              AMUDP_ReplyIVA(token, handler, source_addr, nbytes, numargs, argptr));
    if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
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

GASNETI_INLINE(gasnetc_AMReplyLong)
int gasnetc_AMReplyLong(    gex_Token_t token, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes, void *dest_addr,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  int retval;
  gasneti_leaf_finish(lc_opt); // always locally completed
  if_pt (gasnetc_token_in_nbrhd(token)) {
      retval = gasnetc_nbrhd_ReplyGeneric( gasneti_Long, token, handler,
                                           source_addr, nbytes, dest_addr,
                                           flags, numargs, argptr);
  } else {
    uintptr_t dest_offset;

#if GASNETC_MOCK_EVERYTHING
    dest_offset = (uintptr_t)dest_addr;
#else
    gex_Rank_t dest = gasnetc_msgsource(token);
    dest_offset = ((uintptr_t)dest_addr) - (uintptr_t)gasneti_client_seginfo(gasneti_THUNK_TM, dest)->addr;
#endif

    AM_ASSERT_LOCKED();
    GASNETI_AM_SAFE_NORETURN(retval,
              AMUDP_ReplyXferVA(token, handler, source_addr, nbytes, dest_offset, numargs, argptr));
    if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
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
#if GASNET_TRACE || GASNET_DEBUG
  /* called when entering/leaving handler - also called when entering/leaving AM_Reply call */
  extern void gasnetc_enteringHandler_hook(amudp_category_t cat, int isReq, int handlerId, void *token, 
                                           void *buf, size_t nbytes, int numargs, uint32_t *args) {
    #if GASNET_DEBUG
      // TODO-EX: per-EP table
      const gex_AM_Entry_t * const handler_entry = &gasnetc_handler[handlerId];
      gasneti_amtbl_check(handler_entry, numargs, (gasneti_category_t)cat, isReq);
    #endif
    switch (cat) {
      case amudp_Short:
        if (isReq) GASNETI_TRACE_AMSHORT_REQHANDLER(handlerId, token, numargs, args);
        else       GASNETI_TRACE_AMSHORT_REPHANDLER(handlerId, token, numargs, args);
        break;
      case amudp_Medium:
        if (isReq) GASNETI_TRACE_AMMEDIUM_REQHANDLER(handlerId, token, buf, nbytes, numargs, args);
        else       GASNETI_TRACE_AMMEDIUM_REPHANDLER(handlerId, token, buf, nbytes, numargs, args);
        break;
      case amudp_Long:
        if (isReq) GASNETI_TRACE_AMLONG_REQHANDLER(handlerId, token, buf, nbytes, numargs, args);
        else       GASNETI_TRACE_AMLONG_REPHANDLER(handlerId, token, buf, nbytes, numargs, args);
        break;
      default: gasneti_unreachable_error(("Unknown handler type in gasnetc_enteringHandler_hook(): 0x%x",(int)cat));
    }
  }
  extern void gasnetc_leavingHandler_hook(amudp_category_t cat, int isReq) {
    switch (cat) {
      case amudp_Short:
        GASNETI_TRACE_PRINTF(A,("AM%s_SHORT_HANDLER: handler execution complete", (isReq?"REQUEST":"REPLY"))); \
        break;
      case amudp_Medium:
        GASNETI_TRACE_PRINTF(A,("AM%s_MEDIUM_HANDLER: handler execution complete", (isReq?"REQUEST":"REPLY"))); \
        break;
      case amudp_Long:
        GASNETI_TRACE_PRINTF(A,("AM%s_LONG_HANDLER: handler execution complete", (isReq?"REQUEST":"REPLY"))); \
        break;
      default: gasneti_unreachable_error(("Unknown handler type in gasnetc_leavingHandler_hook(): 0x%x",(int)cat));
    }
  }
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Checkpoint/restart
  ==================
  thin wrappers around AMUDP-level support
*/

#if GASNET_BLCR

/* NON-collective checkpoint request */
int gasnet_checkpoint(const char *dir) {
  int i, rc;

  gasneti_flush_streams();

  AMLOCK();
  rc = AMUDP_SPMDCheckpoint(&gasnetc_bundle, &gasnetc_endpoint, dir);
  if (rc > 0) {
    /* Handlers */
    for (i=0; i<GASNETC_MAX_NUMHANDLERS; i++) {
      if (gasnetc_handler[i].gex_index) {
        AM_SetHandler(gasnetc_endpoint, (handler_t)i, gasnetc_handler[i].gex_fnptr);
        /* BLCR-TODO: error-checking */
      }
    }

    /* Segment */
#if GASNETC_MOCK_EVERYTHING
    i = AM_SetSeg(gasnetc_endpoint, NULL, (uintptr_t)-1);
#else
    i = AM_SetSeg(gasnetc_endpoint, myseg.addr, myseg.size);
#endif
    /* BLCR-TODO: error-checking */

    #if GASNET_TRACE || GASNET_DEBUG
     #if !GASNET_DEBUG
      if (GASNETI_TRACE_ENABLED(A))
     #endif
        GASNETI_AM_SAFE(AMUDP_SetHandlerCallbacks(gasnetc_endpoint,
          gasnetc_enteringHandler_hook, gasnetc_leavingHandler_hook));
    #endif
  }
  AMUNLOCK();

#if GASNET_DEBUG_VERBOSE
  fprintf(stderr, "Node %d %s checkpoint\n", gasneti_mynode, rc?"restart from":"continue after");
#endif

  return rc;
}

/* Collective checkpoint request */

int gasnet_all_checkpoint(const char *dir_arg) {
  const char *dir;
  int rc;
  dir = gasneti_checkpoint_dir(dir_arg);
  gasnet_barrier(0, GASNET_BARRIERFLAG_ANONYMOUS);
  rc = gasnet_checkpoint(dir);
  if (!dir_arg) gasneti_free((void*)dir);
  gasnet_barrier(0, GASNET_BARRIERFLAG_ANONYMOUS);
  return rc;
}

#endif /* GASNET_BLCR */
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
