/*   $Source: /var/local/cvs/gasnet/udp-conduit/gasnet_core.c,v $
 *     $Date: 2010/04/17 00:47:22 $
 * $Revision: 1.41.2.1 $
 * Description: GASNet UDP conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#include <gasnet_core_internal.h>

#include <amudp_spmd.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>

GASNETI_IDENT(gasnetc_IdentString_Version, "$GASNetCoreLibraryVersion: " GASNET_CORE_VERSION_STR " $");
GASNETI_IDENT(gasnetc_IdentString_Name,    "$GASNetCoreLibraryName: " GASNET_CORE_NAME_STR " $");

gasnet_handlerentry_t const *gasnetc_get_handlertable(void);
static void gasnetc_traceoutput(int);
#if HAVE_ON_EXIT
static void gasnetc_on_exit(int, void*);
#else
static void gasnetc_atexit(void);
#endif

eb_t gasnetc_bundle;
ep_t gasnetc_endpoint;

gasneti_mutex_t gasnetc_AMlock = GASNETI_MUTEX_INITIALIZER; /*  protect access to AMUDP */
volatile int gasnetc_AMLockYield = 0;

#if GASNET_PSHM
  gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS]; /* shadow handler table */
#endif /* GASNET_PSHM */

#if GASNET_TRACE
  extern void gasnetc_enteringHandler_hook(amudp_category_t cat, int isReq, int handlerId, void *token, 
                                         void *buf, size_t nbytes, int numargs, uint32_t *args);
  extern void gasnetc_leavingHandler_hook(amudp_category_t cat, int isReq);
#endif

#if defined(GASNET_CSPAWN_CMD)
  #define GASNETC_DEFAULT_SPAWNFN C
  GASNETI_IDENT(gasnetc_IdentString_Default_CSpawnCommand, "$GASNetCSpawnCommand: " GASNET_CSPAWN_CMD " $");
#elif defined(REXEC)
  #define GASNETC_DEFAULT_SPAWNFN R
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

  gasneti_assert(GASNET_MAXNODES <= AMUDP_MAX_SPMDPROCS);
  gasneti_assert(AMUDP_MAX_NUMHANDLERS >= 256);
  gasneti_assert(AMUDP_MAX_SEGLENGTH == (uintptr_t)-1);

  gasneti_assert(GASNET_ERR_NOT_INIT == AM_ERR_NOT_INIT);
  gasneti_assert(GASNET_ERR_RESOURCE == AM_ERR_RESOURCE);
  gasneti_assert(GASNET_ERR_BAD_ARG  == AM_ERR_BAD_ARG);
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

#define INITERR(type, reason) do {                                      \
   if (gasneti_VerboseErrors) {                                         \
     fprintf(stderr, "GASNet initialization encountered an error: %s\n" \
      "  in %s at %s:%i\n",                                             \
      #reason, GASNETI_CURRENT_FUNCTION,  __FILE__, __LINE__);          \
   }                                                                    \
   retval = GASNET_ERR_ ## type;                                        \
   goto done;                                                           \
 } while (0)

static int gasnetc_init(int *argc, char ***argv) {
  int retval = GASNET_OK;

  /*  check system sanity */
  gasnetc_check_config();

  /* --------- begin Master code ------------ */
  if (!AMUDP_SPMDIsWorker(*argv)) { 
    /* assume we're an implicit master 
       (we don't currently support explicit workers spawned 
        without using the AMUDP SPMD API)   
     */
    int num_nodes;
    int i;
    char spawnfn;
    amudp_spawnfn_t fp = (amudp_spawnfn_t)NULL;

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
      char spawnstr[255];
      sprintf(spawnstr,"%c",toupper(spawnfn));
      gasneti_setenv("GASNET_SPAWNFN",spawnstr);
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

    AMUDP_VerboseErrors = gasneti_VerboseErrors;
    AMUDP_SPMDkillmyprocess = gasneti_killmyprocess;

    /*  perform job spawn */
    retval = AMUDP_SPMDStartup(argc, argv, 
      0, 0, NULL, /* dummies */
      NULL, &gasnetc_bundle, &gasnetc_endpoint);
    if (retval != AM_OK) INITERR(RESOURCE, "slave AMUDP_SPMDStartup() failed");
    gasneti_init_done = 1; /* enable early to allow tracing */

    gasneti_conduit_getenv = (/* cast drops const */ gasneti_getenv_fn_t*)&AMUDP_SPMDgetenvMaster;
    gasneti_mynode = AMUDP_SPMDMyProc();
    gasneti_nodes = AMUDP_SPMDNumProcs();

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

    gasneti_nodemapInit(&gasnetc_bootstrapExchange, NULL, 0, 0);

    #if GASNET_PSHM
      gasneti_pshm_init(&gasnetc_bootstrapExchange, 0);
    #endif

    #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    { uintptr_t limit;
      #if HAVE_MMAP
        limit = gasneti_mmapLimit((uintptr_t)-1, (uint64_t)-1,
                                  &gasnetc_bootstrapExchange,
                                  &gasnetc_bootstrapBarrier);
      #else
        limit = (intptr_t)-1;
      #endif
      gasneti_segmentInit(limit, &gasnetc_bootstrapExchange);
    }
    #elif GASNET_SEGMENT_EVERYTHING
      /* segment is everything - nothing to do */
    #else
      #error Bad segment config
    #endif

  AMUNLOCK();

  gasneti_auxseg_init(); /* adjust max seg values based on auxseg */

  gasneti_assert(retval == GASNET_OK);
  return retval;

done: /*  error return while locked */
  AMUNLOCK();
  GASNETI_RETURN(retval);
}

/* ------------------------------------------------------------------------------------ */
extern int gasnet_init(int *argc, char ***argv) {
  int retval = gasnetc_init(argc, argv);
  if (retval != GASNET_OK) GASNETI_RETURN(retval);
  #if 0
    /* called within gasnet_init to allow init tracing */
    gasneti_trace_init(argc, argv);
  #endif
  return GASNET_OK;
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
        sprintf(s,"Too many handlers. (limit=%i)", highlimit - lowlimit + 1);
        GASNETI_RETURN_ERRR(BAD_ARG, s);
      }
    }

    /*  ensure handlers fall into the proper range of pre-assigned values */
    if (newindex < lowlimit || newindex > highlimit) {
      char s[255];
      sprintf(s, "handler index (%i) out of range [%i..%i]", newindex, lowlimit, highlimit);
      GASNETI_RETURN_ERRR(BAD_ARG, s);
    }

    /* discover duplicates */
    if (checkuniqhandler[newindex] != 0) 
      GASNETI_RETURN_ERRR(BAD_ARG, "handler index not unique");
    checkuniqhandler[newindex] = 1;

    /* register the handler */
    if (AM_SetHandler(gasnetc_endpoint, (handler_t)newindex, table[i].fnptr) != AM_OK) 
      GASNETI_RETURN_ERRR(RESOURCE, "AM_SetHandler() failed while registering handlers");
#if GASNET_PSHM
    /* Maintain a shadown handler table for AMPSHM */
    gasnetc_handler[(gasnet_handler_t)newindex] = (gasneti_handler_fn_t)table[i].fnptr;
#endif

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
  int retval = GASNET_OK;
  void *segbase = NULL;
  
  GASNETI_TRACE_PRINTF(C,("gasnetc_attach(table (%i entries), segsize=%lu, minheapoffset=%lu)",
                          numentries, (unsigned long)segsize, (unsigned long)minheapoffset));
  AMLOCK();
    if (!gasneti_init_done) 
      INITERR(NOT_INIT, "GASNet attach called before init");
    if (gasneti_attach_done) 
      INITERR(NOT_INIT, "GASNet already attached");

    /* pause to make sure all nodes have called attach 
       if a node calls gasnet_exit() between init/attach, then this allows us
       to process the AMUDP_SPMD control messages required for job shutdown
     */
    gasnetc_bootstrapBarrier();

    /*  check argument sanity */
    #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
      if ((segsize % GASNET_PAGESIZE) != 0) 
        INITERR(BAD_ARG, "segsize not page-aligned");
      if (segsize > gasneti_MaxLocalSegmentSize) 
        INITERR(BAD_ARG, "segsize too large");
      if ((minheapoffset % GASNET_PAGESIZE) != 0) /* round up the minheapoffset to page sz */
        minheapoffset = ((minheapoffset / GASNET_PAGESIZE) + 1) * GASNET_PAGESIZE;
    #else
      segsize = 0;
      minheapoffset = 0;
    #endif

    segsize = gasneti_auxseg_preattach(segsize); /* adjust segsize for auxseg reqts */

    /* ------------------------------------------------------------------------------------ */
    /*  register handlers */
#if GASNET_PSHM
    /* Initialize AMPSHM's shadow handler table */
    { int i;
      for (i=0; i<GASNETC_MAX_NUMHANDLERS; i++)
          gasnetc_handler[i]=(gasneti_handler_fn_t)&gasneti_defaultAMHandler;
    }
#endif
    { /*  core API handlers */
      gasnet_handlerentry_t *ctable = (gasnet_handlerentry_t *)gasnetc_get_handlertable();
      int len = 0;
      int numreg = 0;
      gasneti_assert(ctable);
      while (ctable[len].fnptr) len++; /* calc len */
      if (gasnetc_reghandlers(ctable, len, 1, 63, 0, &numreg) != GASNET_OK)
        INITERR(RESOURCE,"Error registering core API handlers");
      gasneti_assert(numreg == len);
    }

    { /*  extended API handlers */
      gasnet_handlerentry_t *etable = (gasnet_handlerentry_t *)gasnete_get_handlertable();
      int len = 0;
      int numreg = 0;
      gasneti_assert(etable);
      while (etable[len].fnptr) len++; /* calc len */
      if (gasnetc_reghandlers(etable, len, 64, 127, 0, &numreg) != GASNET_OK)
        INITERR(RESOURCE,"Error registering extended API handlers");
      gasneti_assert(numreg == len);
    }

    if (table) { /*  client handlers */
      int numreg1 = 0;
      int numreg2 = 0;

      /*  first pass - assign all fixed-index handlers */
      if (gasnetc_reghandlers(table, numentries, 128, 255, 0, &numreg1) != GASNET_OK)
        INITERR(RESOURCE,"Error registering fixed-index client handlers");

      /*  second pass - fill in dontcare-index handlers */
      if (gasnetc_reghandlers(table, numentries, 128, 255, 1, &numreg2) != GASNET_OK)
        INITERR(RESOURCE,"Error registering fixed-index client handlers");

      gasneti_assert(numreg1 + numreg2 == numentries);
    }

    /* ------------------------------------------------------------------------------------ */
    /*  register fatal signal handlers */

    /* catch fatal signals and convert to SIGQUIT */
    gasneti_registerSignalHandlers(gasneti_defaultSignalHandler);

#if HAVE_ON_EXIT
    on_exit(gasnetc_on_exit, NULL);
#else
    atexit(gasnetc_atexit);
#endif

    /* ------------------------------------------------------------------------------------ */
    /*  register segment  */

    gasneti_seginfo = (gasnet_seginfo_t *)gasneti_malloc(gasneti_nodes*sizeof(gasnet_seginfo_t));

    #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
      gasneti_segmentAttach(segsize, minheapoffset, gasneti_seginfo, &gasnetc_bootstrapExchange);
    #else /* GASNET_SEGMENT_EVERYTHING */
      { int i;
        for (i=0;i<gasneti_nodes;i++) {
          gasneti_seginfo[i].addr = (void *)0;
          gasneti_seginfo[i].size = (uintptr_t)-1;
        }
      }
    #endif
    segbase = gasneti_seginfo[gasneti_mynode].addr;
    segsize = gasneti_seginfo[gasneti_mynode].size;

    /*  AMUDP allows arbitrary registration with no further action  */
    if (segsize) {
      retval = AM_SetSeg(gasnetc_endpoint, segbase, segsize);
      if (retval != AM_OK) INITERR(RESOURCE, "AM_SetSeg() failed");
    }

    #if GASNET_TRACE
      if (GASNETI_TRACE_ENABLED(A))
        GASNETI_AM_SAFE(AMUDP_SetHandlerCallbacks(gasnetc_endpoint,
          gasnetc_enteringHandler_hook, gasnetc_leavingHandler_hook));
    #endif

    /* ------------------------------------------------------------------------------------ */
    /*  primary attach complete */
    gasneti_attach_done = 1;
    gasnetc_bootstrapBarrier();
  AMUNLOCK();

  GASNETI_TRACE_PRINTF(C,("gasnetc_attach(): primary attach complete\n"));

  gasneti_auxseg_attach(); /* provide auxseg */

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
#if HAVE_ON_EXIT
static void gasnetc_on_exit(int exitcode, void *arg) {
  gasnetc_exit(exitcode);
}
#else
static void gasnetc_atexit(void) {
  gasnetc_exit(0);
}
#endif
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

      if (gasnet_mynode() != 0) {
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

    if ((gasnet_mynode() == 0 || !isglobal) && !retval) {
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
extern int gasnetc_AMGetMsgSource(gasnet_token_t token, gasnet_node_t *srcindex) {
  int retval;
  gasnet_node_t sourceid;
  GASNETI_CHECKATTACH();
  GASNETI_CHECK_ERRR((!token),BAD_ARG,"bad token");
  GASNETI_CHECK_ERRR((!srcindex),BAD_ARG,"bad src ptr");

#if GASNET_PSHM
  if (gasneti_AMPSHMGetMsgSource(token, &sourceid) != GASNET_OK)
#endif
  {
    int tmp; /* AMUDP wants an int, but gasnet_node_t is uint16_t */
    GASNETI_AM_SAFE_NORETURN(retval,AMUDP_GetSourceId(token, &tmp));
    if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
    gasneti_assert(tmp >= 0);
    sourceid = tmp;
  } 

    gasneti_assert(sourceid < gasneti_nodes);
    *srcindex = sourceid;
    return GASNET_OK;
}

extern int gasnetc_AMPoll(void) {
  int retval;
  GASNETI_CHECKATTACH();
#if GASNET_PSHM
  gasneti_AMPSHMPoll(0);
#endif
  AMLOCK();
    GASNETI_AM_SAFE_NORETURN(retval,AM_Poll(gasnetc_bundle));
  AMUNLOCK();
  if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
  else return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
*/

extern int gasnetc_AMRequestShortM( 
                            gasnet_node_t dest,       /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTSHORT(dest,handler,numargs);
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Short, dest, handler,
                                           0, 0, 0,
                                           numargs, argptr);
  } else
#endif
  {
    AMLOCK_TOSEND();
      GASNETI_AM_SAFE_NORETURN(retval,
               AMUDP_RequestVA(gasnetc_endpoint, dest, handler, 
                               numargs, argptr));
    AMUNLOCK();
  }
  va_end(argptr);
  if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
  else return GASNET_OK;
}

extern int gasnetc_AMRequestMediumM( 
                            gasnet_node_t dest,      /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTMEDIUM(dest,handler,source_addr,nbytes,numargs);
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Medium, dest, handler,
                                           source_addr, nbytes, 0,
                                           numargs, argptr);
  } else
#endif
  {
    if_pf (!nbytes) source_addr = (void*)(uintptr_t)1; /* Bug 2774 - anything but NULL */

    AMLOCK_TOSEND();
      GASNETI_AM_SAFE_NORETURN(retval,
               AMUDP_RequestIVA(gasnetc_endpoint, dest, handler, 
                                source_addr, nbytes, 
                                numargs, argptr));
    AMUNLOCK();
  }
  va_end(argptr);
  if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
  else return GASNET_OK;
}

extern int gasnetc_AMRequestLongM( gasnet_node_t dest,        /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTLONG(dest,handler,source_addr,nbytes,dest_addr,numargs);
  
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  if_pt (gasneti_pshm_in_supernode(dest)) {
      retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Long, dest, handler,
                                             source_addr, nbytes, dest_addr,
                                             numargs, argptr);
  } else
#endif
  {
    uintptr_t dest_offset;
    dest_offset = ((uintptr_t)dest_addr) - ((uintptr_t)gasneti_seginfo[dest].addr);

    if_pf (!nbytes) source_addr = (void*)(uintptr_t)1; /* Bug 2774 - anything but NULL */

    AMLOCK_TOSEND();
      GASNETI_AM_SAFE_NORETURN(retval,
               AMUDP_RequestXferVA(gasnetc_endpoint, dest, handler, 
                                   source_addr, nbytes, 
                                   dest_offset, 0,
                                   numargs, argptr));
    AMUNLOCK();
  }
  va_end(argptr);
  if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
  else return GASNET_OK;
}

extern int gasnetc_AMReplyShortM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREPLYSHORT(token,handler,numargs);
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  if_pt (gasnetc_token_is_pshm(token)) {
      retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Short, token, handler,
                                           0, 0, 0,
                                           numargs, argptr);
  } else
#endif
  {
    AM_ASSERT_LOCKED();
    GASNETI_AM_SAFE_NORETURN(retval,
              AMUDP_ReplyVA(token, handler, numargs, argptr));
  }
  va_end(argptr);
  if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
  else return GASNET_OK;
}

extern int gasnetc_AMReplyMediumM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,numargs);
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  if_pt (gasnetc_token_is_pshm(token)) {
       retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Medium, token, handler,
                                            source_addr, nbytes, 0,
                                            numargs, argptr);
  } else
#endif
  {
    if_pf (!nbytes) source_addr = (void*)(uintptr_t)1; /* Bug 2774 - anything but NULL */

    AM_ASSERT_LOCKED();
    GASNETI_AM_SAFE_NORETURN(retval,
              AMUDP_ReplyIVA(token, handler, source_addr, nbytes, numargs, argptr));
  }
  va_end(argptr);
  if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
  else return GASNET_OK;
}

extern int gasnetc_AMReplyLongM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  int retval;
  va_list argptr;
  
  GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,numargs); 

  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  if_pt (gasnetc_token_is_pshm(token)) {
      retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Long, token, handler,
                                           source_addr, nbytes, dest_addr,
                                           numargs, argptr);
  } else
#endif
  {
    gasnet_node_t dest;
    uintptr_t dest_offset;

    GASNETI_SAFE_PROPAGATE(gasnet_AMGetMsgSource(token, &dest));
    dest_offset = ((uintptr_t)dest_addr) - ((uintptr_t)gasneti_seginfo[dest].addr);

    if_pf (!nbytes) source_addr = (void*)(uintptr_t)1; /* Bug 2774 - anything but NULL */

    AM_ASSERT_LOCKED();
    GASNETI_AM_SAFE_NORETURN(retval,
              AMUDP_ReplyXferVA(token, handler, source_addr, nbytes, dest_offset, numargs, argptr));
  }
  va_end(argptr);
  if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
  else return GASNET_OK;
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
#if GASNET_TRACE
  /* called when entering/leaving handler - also called when entering/leaving AM_Reply call */
  extern void gasnetc_enteringHandler_hook(amudp_category_t cat, int isReq, int handlerId, void *token, 
                                           void *buf, size_t nbytes, int numargs, uint32_t *args) {
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
      default: gasneti_fatalerror("Unknown handler type in gasnetc_enteringHandler_hook(): %i", cat);
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
      default: gasneti_fatalerror("Unknown handler type in gasnetc_leavingHandler_hook(): %i", cat);
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
