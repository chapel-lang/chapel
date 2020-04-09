/*   $Source: bitbucket.org:berkeleylab/gasnet.git/mpi-conduit/gasnet_core.c $
 * Description: GASNet MPI conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_am.h>

#include <ammpi_spmd.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>

GASNETI_IDENT(gasnetc_IdentString_Version, "$GASNetCoreLibraryVersion: " GASNET_CORE_VERSION_STR " $");
GASNETI_IDENT(gasnetc_IdentString_Name,    "$GASNetCoreLibraryName: " GASNET_CORE_NAME_STR " $");

gex_AM_Entry_t const *gasnetc_get_handlertable(void);

gex_AM_Entry_t *gasnetc_handler; // TODO-EX: will be replaced with per-EP tables

// TODO-EX: This is a hack to support multiple segments w/ a single AM EP
#ifndef GASNETC_MOCK_EVERYTHING
#define GASNETC_MOCK_EVERYTHING 1
#endif

static void gasnetc_traceoutput(int);

eb_t gasnetc_bundle;
ep_t gasnetc_endpoint;

gasneti_mutex_t gasnetc_AMlock = GASNETI_MUTEX_INITIALIZER; /*  protect access to AMMPI */

#if GASNETC_HSL_ERRCHECK || GASNET_TRACE || GASNET_DEBUG
  extern void gasnetc_enteringHandler_hook(ammpi_category_t cat, int isReq, int handlerId, void *token, 
                                         void *buf, size_t nbytes, int numargs, uint32_t *args);
  extern void gasnetc_leavingHandler_hook(ammpi_category_t cat, int isReq);
#endif

#if GASNETC_HSL_ERRCHECK
  /* check a call is legally outside Handler Context or HSL */
  void gasnetc_checkcallHC(void);
  void gasnetc_checkcallHSL(void);
  void gasnetc_hsl_attach(void);
  #define CHECKCALLHC()  gasnetc_checkcallHC()
  #define CHECKCALLHSL() gasnetc_checkcallHSL()
#else
  #define CHECKCALLHC()
  #define CHECKCALLHSL()
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* called at startup to check configuration sanity */
static void gasnetc_check_config(void) {
  gasneti_check_config_preinit();

  gasneti_static_assert(GASNET_MAXNODES <= AMMPI_MAX_SPMDPROCS);
  gasneti_static_assert(AMMPI_MAX_NUMHANDLERS >= 256);
  gasneti_static_assert(AMMPI_MAX_SEGLENGTH == (uintptr_t)-1);

  gasneti_static_assert(GASNET_ERR_NOT_INIT == AM_ERR_NOT_INIT);
  gasneti_static_assert(GASNET_ERR_RESOURCE == AM_ERR_RESOURCE);
  gasneti_static_assert(GASNET_ERR_BAD_ARG  == AM_ERR_BAD_ARG);

#if GASNET_PSHM
  gasneti_static_assert(gasneti_Short  == (gasneti_category_t) ammpi_Short);
  gasneti_static_assert(gasneti_Medium == (gasneti_category_t) ammpi_Medium);
  gasneti_static_assert(gasneti_Long   == (gasneti_category_t) ammpi_Long);
#endif
}

void gasnetc_bootstrapBarrier(void) {
   int retval;
   AM_ASSERT_LOCKED(); /* need this because SPMDBarrier may poll */
   GASNETI_AM_SAFE_NORETURN(retval,AMMPI_SPMDBarrier());
   if_pf (retval) gasneti_fatalerror("failure in gasnetc_bootstrapBarrier()");
}
void gasnetc_bootstrapExchange(void *src, size_t len, void *dest) {
  int retval;
  GASNETI_AM_SAFE_NORETURN(retval,AMMPI_SPMDAllGather(src, dest, len));
  if_pf (retval) gasneti_fatalerror("failure in gasnetc_bootstrapExchange()");
}
void gasnetc_bootstrapBroadcast(void *src, size_t len, void *dest, int rootnode) {
  int retval;
  gasneti_assert(gasneti_nodes > 0);
  gasneti_assert_uint(gasneti_mynode ,<, gasneti_nodes);
  if (gasneti_mynode == rootnode) GASNETI_MEMCPY_SAFE_IDENTICAL(dest, src, len);
  GASNETI_AM_SAFE_NORETURN(retval,AMMPI_SPMDBroadcast(dest, len, rootnode));
  if_pf (retval) gasneti_fatalerror("failure in gasnetc_bootstrapBroadcast()");
}
#if GASNET_PSHM /* Used only in call to gasneti_pshm_init() */
/* Naive (poorly scaling) "reference" implementation via gasnetc_bootstrapExchange() */
static void gasnetc_bootstrapSNodeBroadcast(void *src, size_t len, void *dest, int rootnode) {
  void *tmp = gasneti_malloc(len * gasneti_nodes);
  void *self = src ? src : gasneti_malloc(len); /* Ensure never NULL */
  gasnetc_bootstrapExchange(self, len, tmp);
  GASNETI_MEMCPY(dest, (void*)((uintptr_t)tmp + (len * rootnode)), len);
  if (self != src) gasneti_free(self);
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
  int networkdepth = 0;
  const char *pstr = NULL;
  const char *tmsgstr = NULL;

  AMLOCK();
    if (gasneti_init_done) 
      INITERR(NOT_INIT, "GASNet already initialized");
    gasneti_init_done = 1; /* enable early to allow tracing */

    /*  check system sanity */
    gasnetc_check_config();

    gasneti_freezeForDebugger();

    #if GASNET_DEBUG_VERBOSE
      /* note - can't call trace macros during gasnet_init because trace system not yet initialized */
      fprintf(stderr,"gasnetc_init(): about to spawn...\n"); fflush(stderr);
    #endif

    /*  choose network depth */
    networkdepth = gasnett_getenv_int_withdefault("GASNET_NETWORKDEPTH", GASNETC_DEFAULT_NETWORKDEPTH, 0);
    if (networkdepth <= 1) networkdepth = GASNETC_DEFAULT_NETWORKDEPTH;

    AMX_VerboseErrors = gasneti_VerboseErrors;
    AMMPI_SPMDkillmyprocess = gasneti_killmyprocess;
    #if !GASNETI_DISABLE_MPI_INIT_THREAD
    { // this scope silences a warning on Cray C about INITERR bypassing this initialization:
      #if GASNETI_THREADS
        int usingthreads = 1;
        #define GASNETI_THREADMODE_MSG \
                      "*** WARNING: The thread-safe version of mpi-conduit recommends an MPI implementation\n" \
                      "*** WARNING: which supports at least MPI_THREAD_SERIALIZED to ensure correct operation.\n"
      #else
        int usingthreads = 0;
        #define GASNETI_THREADMODE_MSG 
      #endif
      // for verbose documentation only:
      gasnett_getenv_withdefault("GASNET_MPI_THREAD", (usingthreads?"MPI_THREAD_SERIALIZED":"MPI_THREAD_SINGLE"));
      if (!AMMPI_SPMDSetThreadMode(usingthreads, &pstr, argc, argv)) { 
        // Some versions of MPI lie and report THREAD_SINGLE, when in actuality MPI_THREAD_SERIALIZED seems to work just fine.
        // User can ignore this warning or hide it by setting GASNET_MPI_THREAD or GASNET_QUIET if they want to "live dangerously".
        static char tmsg[1024];
        snprintf(tmsg, sizeof(tmsg),
                      "*** WARNING: This MPI implementation reports it can only support %s.\n"
                      GASNETI_THREADMODE_MSG
                      "*** WARNING: You can override the requested thread mode by setting GASNET_MPI_THREAD.\n"
                      , pstr);
        tmsgstr = tmsg;
      }
    }
    #endif

    /*  perform job spawn */
    retval = AMMPI_SPMDStartup(argc, argv, networkdepth, NULL, &gasnetc_bundle, &gasnetc_endpoint);
    if (retval != AM_OK) INITERR(RESOURCE, "AMMPI_SPMDStartup() failed");

    gasneti_mynode = AMMPI_SPMDMyProc();
    gasneti_nodes = AMMPI_SPMDNumProcs();

    /* a number of MPI job spawners fail to propagate the environment to all compute nodes */
    /* do this before trace_init to make sure it gets right environment */
    gasneti_setupGlobalEnvironment(gasneti_nodes, gasneti_mynode, 
                                   gasnetc_bootstrapExchange, gasnetc_bootstrapBroadcast);

    /* Must init timers after global env, and preferably before tracing */
    GASNETI_TICKS_INIT();

    /* enable tracing */
    gasneti_trace_init(argc, argv);
    GASNETI_AM_SAFE(AMMPI_SPMDSetExitCallback(gasnetc_traceoutput));
    if (pstr)    GASNETI_TRACE_PRINTF(C,("AMMPI_SPMDSetThreadMode/MPI_Init_thread()=>%s",pstr));
    if (tmsgstr) GASNETI_TRACE_PRINTF(I,("%s",tmsgstr));
    if (tmsgstr && !gasneti_mynode &&
        !gasneti_getenv_yesno_withdefault("GASNET_QUIET",0)) { fprintf(stderr, "%s", tmsgstr); fflush(stderr); }

    #if GASNET_DEBUG_VERBOSE
      fprintf(stderr,"gasnetc_init(): spawn successful - node %i/%i starting...\n", 
        gasneti_mynode, gasneti_nodes); fflush(stderr);
    #endif

    gasneti_nodemapInit(&gasnetc_bootstrapExchange, NULL, 0, 0);

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

  AMUNLOCK();

  gasneti_assert(retval == GASNET_OK);
  return retval;

done: /*  error return while locked */
  AMUNLOCK();
  GASNETI_RETURN(retval);
}

/* ------------------------------------------------------------------------------------ */
extern int gasnetc_amregister(gex_AM_Index_t index, gex_AM_Entry_t *entry) {
  if (AM_SetHandler(gasnetc_endpoint, (handler_t)index, entry->gex_fnptr) != AM_OK)
    GASNETI_RETURN_ERRR(RESOURCE, "AM_SetHandler() failed while registering handlers");
  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
static int gasnetc_attach_primary(void) {
  int retval = GASNET_OK;
  
  AMLOCK();
    /* pause to make sure all nodes have called attach 
       if a node calls gasnet_exit() between init/attach, then this allows us
       to process the AMMPI_SPMD control messages required for job shutdown
     */
    gasnetc_bootstrapBarrier();

    /* ------------------------------------------------------------------------------------ */
    /*  register fatal signal handlers */

    /* catch fatal signals and convert to SIGQUIT */
    gasneti_registerSignalHandlers(gasneti_defaultSignalHandler);

    // register process exit-time hook
    gasneti_registerExitHandler(gasnetc_exit);

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
    /*  AMMPI allows arbitrary registration with no further action  */
    if (segsize) {
      retval = AM_SetSeg(gasnetc_endpoint, myseg.addr, myseg.size);
      if (retval != AM_OK) INITERR(RESOURCE, "AM_SetSeg() failed");
    }
#endif

    #if GASNETC_HSL_ERRCHECK || GASNET_TRACE || GASNET_DEBUG
      #if !(GASNETC_HSL_ERRCHECK || GASNET_DEBUG)
        if (GASNETI_TRACE_ENABLED(A))
      #endif
          GASNETI_AM_SAFE(AMMPI_SetHandlerCallbacks(gasnetc_endpoint,
            gasnetc_enteringHandler_hook, gasnetc_leavingHandler_hook));
    #endif

    #if GASNETC_HSL_ERRCHECK
      gasnetc_hsl_attach(); /* must precede attach_done to avoid inf recursion on malloc/hold_interrupts */
      // TODO-EX: Is this recursion still an issue w/ removal of NIS?
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

  AMLOCK();
    #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
      /*  register client segment  */
      gex_Segment_t seg; // g2ex segment is automatically saved by a hook
      if (GASNET_OK != gasnetc_attach_segment(&seg, _tm, segsize, gasnetc_bootstrapExchange, GASNETI_FLAG_INIT_LEGACY))
        INITERR(RESOURCE,"Error attaching segment");
    #endif

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

extern void gasnetc_fatalsignal_callback(int sig) {
  if (gasnetc_exitcalled) {
  /* if we get a fatal signal during exit, it's almost certainly a signal-safety or network shutdown
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

  #ifdef GASNETE_EXIT_CALLBACK
    /* callback for native conduits using an mpi-conduit core 
       this should cleanup extended API resources (only) 
       and then return so that MPI can be shutdown properly
     */
    GASNETE_EXIT_CALLBACK(exitcode);
  #endif

  gasneti_flush_streams();
  gasneti_trace_finish();
  gasneti_sched_yield();

  { int i;
   /* try to prevent races where we exit while other local pthreads are in MPI 
     can't use a blocking lock here, because may be in a signal context
   */
   for (i=0; i < 5; i++) {
     #if GASNET_DEBUG
       /* ignore recursive lock attempts */
       if (_gasneti_mutex_heldbyme(&gasnetc_AMlock)) break;
     #endif
     if (!gasneti_mutex_trylock(&gasnetc_AMlock)) break;
     else gasneti_sched_yield();
   }
  }

  AMMPI_SPMDExit(exitcode);
  gasneti_fatalerror("AMMPI_SPMDExit failed");
}

/* ------------------------------------------------------------------------------------ */
/*
  Job Environment Queries
  =======================
*/
extern int gasneti_getSegmentInfo(gasnet_seginfo_t *seginfo_table, int numentries);

extern int gasnetc_getSegmentInfo(gasnet_seginfo_t *seginfo_table, int numentries) {
  CHECKCALLHC();
  return gasneti_getSegmentInfo(seginfo_table, numentries);
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
    gasneti_assert(! gasnetc_token_in_nbrhd(token));
    gasneti_assert(token);

    int tmp; /* AMMPI wants an int, but gex_Rank_t is uint32_t */
    gasneti_assert_zeroret(AMMPI_GetSourceId(token, &tmp));
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
  CHECKCALLHC();
#if GASNET_PSHM
  gasneti_AMPSHMPoll(0 GASNETI_THREAD_PASS);
#endif
  AMLOCK();
  static int cntr;
  // In single-nbrhd case never need to poll the network for client AMs.
  // However, we'll still AM_Poll() every 256th call for orderly exit handling.
  if ((gasneti_mysupernode.grp_count > 1) || !(0xff & cntr++)) {
    GASNETI_AM_SAFE_NORETURN(retval, AM_Poll(gasnetc_bundle));
  } else {
    retval = 0;
  }
  AMUNLOCK();
  if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
  else return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
  TODO-EX: "nbrhd" support means we could remove the unreachable loopback paths in AMMPI.
*/

GASNETI_INLINE(gasnetc_AMRequestShort)
int gasnetc_AMRequestShort( gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  CHECKCALLHC();
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
               AMMPI_RequestVA(gasnetc_endpoint, jobrank, handler, 
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
  CHECKCALLHC();
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
  CHECKCALLHC();
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
               AMMPI_RequestIVA(gasnetc_endpoint, jobrank, handler, 
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
  CHECKCALLHC();
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
               AMMPI_RequestXferVA(gasnetc_endpoint, jobrank, handler, 
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
              AMMPI_ReplyVA(token, handler, numargs, argptr));
    if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
  }
  return retval;
}

extern int gasnetc_AMReplyShortM( 
                            gex_Token_t token,     /* token provided on handler entry */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            gex_Flags_t flags,
                            int numargs, ...) {
  CHECKCALLHSL();
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
              AMMPI_ReplyIVA(token, handler, source_addr, nbytes, numargs, argptr));
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
  CHECKCALLHSL();
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
              AMMPI_ReplyXferVA(token, handler, source_addr, nbytes, dest_offset, numargs, argptr));
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
  CHECKCALLHSL();
  GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs);

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMReplyLong(token,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs,argptr);
  va_end(argptr);
  return retval;
}

/* ------------------------------------------------------------------------------------ */
/* AMMPI provides an optional error-checking implementation of 
   handler-safe locks to assist in debugging client code
 */

#if GASNETC_HSL_ERRCHECK
  typedef struct { /* per-thread HSL err-checking info */
    gex_HSL_t *locksheld;
    unsigned int inhandler;
    int inuse;
  } gasnetc_hsl_errcheckinfo_t;
  static gasnetc_hsl_errcheckinfo_t _info_init = { NULL, 0, 0 };
  static gasnetc_hsl_errcheckinfo_t _info_free = { NULL, 0, 0 };

  #if GASNETI_CLIENT_THREADS
    /*  pthread thread-specific ptr to our info (or NULL for a thread never-seen before) */
    GASNETI_THREADKEY_DEFINE(gasnetc_hsl_errcheckinfo);

    static void gasnetc_hsl_cleanup_threaddata(void *_td) {
      gasnetc_hsl_errcheckinfo_t *info = (gasnetc_hsl_errcheckinfo_t *)_td;
      gasneti_assert(info->inuse);
      if (info->inhandler)
        gasneti_fatalerror("HSL USAGE VIOLATION: thread exit within AM handler");
      if (info->locksheld) GASNETI_TRACE_PRINTF(I,("Thread exiting while holding HSL locks"));
      info->inuse = 0;
      gasneti_threadkey_set(gasnetc_hsl_errcheckinfo, &_info_free);
    }
  
    static gasnetc_hsl_errcheckinfo_t *gasnetc_get_errcheckinfo(void) {
      gasnetc_hsl_errcheckinfo_t *info = gasneti_threadkey_get(gasnetc_hsl_errcheckinfo);
      if_pt (info) return info;

      /*  first time we've seen this thread - need to set it up */
      { /* it's unsafe to call malloc or gasneti_malloc here after attach,
           because we may be within a hold_interrupts call, so table is single-level
           and initialized during gasnet_attach */ // TODO-EX: Still true w/ removal of NIS?
        static gasnetc_hsl_errcheckinfo_t *hsl_errcheck_table = NULL;
        static gasneti_mutex_t hsl_errcheck_tablelock = GASNETI_MUTEX_INITIALIZER;
        int maxthreads = gasneti_max_threads();
        int idx;
        gasneti_mutex_lock(&hsl_errcheck_tablelock);
          if (!hsl_errcheck_table)  {
            hsl_errcheck_table = gasneti_calloc(maxthreads,sizeof(gasnetc_hsl_errcheckinfo_t));        
            gasneti_leak(hsl_errcheck_table);
          }
          for (idx = 0; idx < maxthreads; idx++) {
            if (!hsl_errcheck_table[idx].inuse) break;
          }
          if (idx == maxthreads) gasneti_fatal_threadoverflow("HSL errorcheck");
          gasneti_assert(idx < maxthreads);
          info = &(hsl_errcheck_table[idx]);
          gasneti_assert(!info->inuse);
          GASNETI_MEMCPY(info, &_info_init, sizeof(gasnetc_hsl_errcheckinfo_t));
          info->inuse = 1;
        gasneti_mutex_unlock(&hsl_errcheck_tablelock);
        gasneti_threadkey_set(gasnetc_hsl_errcheckinfo, info);
        gasnete_register_threadcleanup(gasnetc_hsl_cleanup_threaddata, info);
        return info;
      }
    }
  #else
    static gasnetc_hsl_errcheckinfo_t *gasnetc_get_errcheckinfo(void) {
      return &_info_init;
    }
  #endif
  extern void gasnetc_hsl_attach(void) {
    gasnetc_get_errcheckinfo();
  }

  void gasnetc_checkcallHC(void) {
    gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
    if (info->inhandler)
      gasneti_fatalerror("Illegal call to GASNet within by handler context");
    gasnetc_checkcallHSL();
  }
  void gasnetc_checkcallHSL(void) {
    gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
    if (info->locksheld)
      gasneti_fatalerror("Illegal call to GASNet while holding a Handler-Safe Lock");
  }
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Handler-safe locks
  ==================
*/

#if !GASNETC_NULL_HSL
extern void gasnetc_hsl_init   (gex_HSL_t *hsl) {
  GASNETI_CHECKATTACH();
  #if GASNETC_HSL_ERRCHECK
  {
    if (hsl->tag == GASNETC_HSL_ERRCHECK_TAGINIT)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gex_HSL_Init() a statically-initialized HSL");
  #if 0
    /* this causes false errors in Titanium, because object destructors aren't implemented */
    if (hsl->tag == GASNETC_HSL_ERRCHECK_TAGDYN)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gex_HSL_Init() a previously-initialized HSL (or one you forgot to destroy)");
  #endif
    hsl->tag = GASNETC_HSL_ERRCHECK_TAGDYN;
    hsl->next = NULL;
    hsl->islocked = 0;
  }
  #endif

  gasneti_mutex_init(&(hsl->lock));
}

extern void gasnetc_hsl_destroy(gex_HSL_t *hsl) {
  GASNETI_CHECKATTACH();
  #if GASNETC_HSL_ERRCHECK
  {
    if (hsl->tag != GASNETC_HSL_ERRCHECK_TAGINIT && hsl->tag != GASNETC_HSL_ERRCHECK_TAGDYN)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gex_HSL_Destroy() an uninitialized HSL");
    if (hsl->islocked)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gex_HSL_Destroy() a locked HSL");
    hsl->tag = 0;
    gasneti_assert(!hsl->next);
  }
  #endif

  gasneti_mutex_destroy(&(hsl->lock));
}

extern void gasnetc_hsl_lock   (gex_HSL_t *hsl) {
  GASNETI_CHECKATTACH();
  #if GASNETC_HSL_ERRCHECK
  { gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
    gex_HSL_t *heldhsl = info->locksheld;
    if (hsl->tag != GASNETC_HSL_ERRCHECK_TAGINIT && hsl->tag != GASNETC_HSL_ERRCHECK_TAGDYN)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gex_HSL_Lock() an uninitialized HSL");
    while (heldhsl) {
      if (heldhsl == hsl)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to recursively gex_HSL_Lock() an HSL");
      heldhsl = heldhsl->next;
    }
  }
  #endif

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

  #if GASNETC_HSL_ERRCHECK
  { gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
    hsl->islocked = 1;
    hsl->next = info->locksheld;
    info->locksheld = hsl;
    hsl->timestamp = gasneti_ticks_now();
  }
  #endif
}

extern void gasnetc_hsl_unlock (gex_HSL_t *hsl) {
  GASNETI_CHECKATTACH();
  #if GASNETC_HSL_ERRCHECK
  { gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
    gex_HSL_t *heldhsl = info->locksheld;
    if (hsl->tag != GASNETC_HSL_ERRCHECK_TAGINIT && hsl->tag != GASNETC_HSL_ERRCHECK_TAGDYN)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gex_HSL_Unlock() an uninitialized HSL");
    while (heldhsl) {
      if (heldhsl == hsl) break;
      heldhsl = heldhsl->next;
    }
    if (!heldhsl)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gex_HSL_Unlock() an HSL I didn't own");
    if (info->locksheld != hsl)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gex_HSL_Unlock() an HSL out of order");
    { float NIStime = gasneti_ticks_to_ns(gasneti_ticks_now() - hsl->timestamp)/1000.0;
      if (NIStime > GASNETC_NISTIMEOUT_WARNING_THRESHOLD) {
        fprintf(stderr,"HSL USAGE WARNING: held an HSL for a long interval (%8.3f sec)\n", NIStime/1000000.0);
        fflush(stderr);
      }
    }
    hsl->islocked = 0;
    info->locksheld = hsl->next;
  }
  #endif

  GASNETI_TRACE_EVENT_TIME(L, HSL_UNLOCK, GASNETI_TICKS_NOW_IFENABLED(L)-hsl->acquiretime);

  gasneti_mutex_unlock(&(hsl->lock));
}

extern int  gasnetc_hsl_trylock(gex_HSL_t *hsl) {
  GASNETI_CHECKATTACH();

  #if GASNETC_HSL_ERRCHECK
  { gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
    gex_HSL_t *heldhsl = info->locksheld;
    if (hsl->tag != GASNETC_HSL_ERRCHECK_TAGINIT && hsl->tag != GASNETC_HSL_ERRCHECK_TAGDYN)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gex_HSL_Trylock() an uninitialized HSL");
    while (heldhsl) {
      if (heldhsl == hsl)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to recursively gex_HSL_Trylock() an HSL");
      heldhsl = heldhsl->next;
    }
  }
  #endif

  {
    int locked = (gasneti_mutex_trylock(&(hsl->lock)) == 0);

    GASNETI_TRACE_EVENT_VAL(L, HSL_TRYLOCK, locked);
    if (locked) {
      #if GASNETI_STATS_OR_TRACE
        hsl->acquiretime = GASNETI_TICKS_NOW_IFENABLED(L);
      #endif
      #if GASNETC_HSL_ERRCHECK
      { gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
        hsl->islocked = 1;
        hsl->next = info->locksheld;
        info->locksheld = hsl;
        hsl->timestamp = gasneti_ticks_now();
      }
      #endif
    }

    return locked ? GASNET_OK : GASNET_ERR_NOT_READY;
  }
}
#endif

#if GASNETC_HSL_ERRCHECK && !GASNETC_NULL_HSL
  extern void gasnetc_enteringHandler_hook_hsl(int cat, int isReq, int handlerId, gex_Token_t token,
                                               void *buf, size_t nbytes, int numargs,
                                               gex_AM_Arg_t *args) {
    gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
    if (info->locksheld)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to make a GASNet network call while holding an HSL");
    info->inhandler++;
  }
  extern void gasnetc_leavingHandler_hook_hsl(int cat, int isReq) {
    gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
    gasneti_assert(info->inhandler > 0);
    if (info->locksheld)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to exit a handler while holding an HSL");
    info->inhandler--;
  }
#endif /* GASNETC_HSL_ERRCHECK && !GASNETC_NULL_HSL */

#if (!GASNETC_NULL_HSL && GASNETC_HSL_ERRCHECK) || GASNET_TRACE || GASNET_DEBUG
  /* called when entering/leaving handler - also called when entering/leaving AM_Reply call */
  extern void gasnetc_enteringHandler_hook(ammpi_category_t cat, int isReq, int handlerId, void *token, 
                                           void *buf, size_t nbytes, int numargs, uint32_t *args) {
    #if GASNET_DEBUG
      // TODO-EX: per-EP table
      const gex_AM_Entry_t * const handler_entry = &gasnetc_handler[handlerId];
      gasneti_amtbl_check(handler_entry, numargs, (gasneti_category_t)cat, isReq);
    #endif
    switch (cat) {
      case ammpi_Short:
        if (isReq) GASNETI_TRACE_AMSHORT_REQHANDLER(handlerId, token, numargs, args);
        else       GASNETI_TRACE_AMSHORT_REPHANDLER(handlerId, token, numargs, args);
        break;
      case ammpi_Medium:
        if (isReq) GASNETI_TRACE_AMMEDIUM_REQHANDLER(handlerId, token, buf, nbytes, numargs, args);
        else       GASNETI_TRACE_AMMEDIUM_REPHANDLER(handlerId, token, buf, nbytes, numargs, args);
        break;
      case ammpi_Long:
        if (isReq) GASNETI_TRACE_AMLONG_REQHANDLER(handlerId, token, buf, nbytes, numargs, args);
        else       GASNETI_TRACE_AMLONG_REPHANDLER(handlerId, token, buf, nbytes, numargs, args);
        break;
      default: gasneti_unreachable_error(("Unknown handler type in gasnetc_enteringHandler_hook(): 0x%x",(int)cat));
    }
    #if (!GASNETC_NULL_HSL && GASNETC_HSL_ERRCHECK)
      gasnetc_enteringHandler_hook_hsl(cat, isReq, handlerId, token, buf, nbytes,
                                       numargs, (gex_AM_Arg_t *)args);
    #endif
  }
  extern void gasnetc_leavingHandler_hook(ammpi_category_t cat, int isReq) {
    switch (cat) {
      case ammpi_Short:
        GASNETI_TRACE_PRINTF(A,("AM%s_SHORT_HANDLER: handler execution complete", (isReq?"REQUEST":"REPLY"))); \
        break;
      case ammpi_Medium:
        GASNETI_TRACE_PRINTF(A,("AM%s_MEDIUM_HANDLER: handler execution complete", (isReq?"REQUEST":"REPLY"))); \
        break;
      case ammpi_Long:
        GASNETI_TRACE_PRINTF(A,("AM%s_LONG_HANDLER: handler execution complete", (isReq?"REQUEST":"REPLY"))); \
        break;
      default: gasneti_unreachable_error(("Unknown handler type in gasnetc_leavingHandler_hook(): 0x%x",(int)cat));
    }
    #if (!GASNETC_NULL_HSL && GASNETC_HSL_ERRCHECK)
      gasnetc_leavingHandler_hook_hsl(cat, isReq);
    #endif
  }
#endif
/* ------------------------------------------------------------------------------------ */
/*
  Private Handlers:
  ================
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
