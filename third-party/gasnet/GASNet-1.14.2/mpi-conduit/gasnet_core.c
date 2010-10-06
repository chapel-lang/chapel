/*   $Source: /var/local/cvs/gasnet/mpi-conduit/gasnet_core.c,v $
 *     $Date: 2010/04/23 03:22:55 $
 * $Revision: 1.81.2.2 $
 * Description: GASNet MPI conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#include <gasnet_core_internal.h>

#include <ammpi_spmd.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>

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

gasneti_mutex_t gasnetc_AMlock = GASNETI_MUTEX_INITIALIZER; /*  protect access to AMMPI */

#if GASNET_PSHM
  gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS]; /* shadow handler table */
#endif /* GASNET_PSHM */

#if GASNETC_HSL_ERRCHECK || GASNET_TRACE
  extern void gasnetc_enteringHandler_hook(ammpi_category_t cat, int isReq, int handlerId, void *token, 
                                         void *buf, size_t nbytes, int numargs, uint32_t *args);
  extern void gasnetc_leavingHandler_hook(ammpi_category_t cat, int isReq);
#endif

#if GASNETC_HSL_ERRCHECK
  /* check a call is legally outside an NIS or HSL */
  void gasnetc_checkcallNIS(void);
  void gasnetc_checkcallHSL(void);
  void gasnetc_hsl_attach(void);
  #define CHECKCALLNIS() gasnetc_checkcallNIS()
  #define CHECKCALLHSL() gasnetc_checkcallHSL()
#else
  #define CHECKCALLNIS()
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

  gasneti_assert(GASNET_MAXNODES <= AMMPI_MAX_SPMDPROCS);
  gasneti_assert(AMMPI_MAX_NUMHANDLERS >= 256);
  gasneti_assert(AMMPI_MAX_SEGLENGTH == (uintptr_t)-1);

  gasneti_assert(GASNET_ERR_NOT_INIT == AM_ERR_NOT_INIT);
  gasneti_assert(GASNET_ERR_RESOURCE == AM_ERR_RESOURCE);
  gasneti_assert(GASNET_ERR_BAD_ARG  == AM_ERR_BAD_ARG);
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
  gasneti_assert(gasneti_nodes > 0 && gasneti_mynode < gasneti_nodes);
  if (gasneti_mynode == rootnode) memcpy(dest, src, len);
  GASNETI_AM_SAFE_NORETURN(retval,AMMPI_SPMDBroadcast(dest, len, rootnode));
  if_pf (retval) gasneti_fatalerror("failure in gasnetc_bootstrapBroadcast()");
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

    AMMPI_VerboseErrors = gasneti_VerboseErrors;
    AMMPI_SPMDkillmyprocess = gasneti_killmyprocess;
    #if !defined(GASNETI_DISABLE_MPI_INIT_THREAD) 
      { int res; 
      #if GASNETI_THREADS
        /* tell MPI to be thread-safe */
        res = AMMPI_SPMDSetThreadMode(1, &pstr, argc, argv);
      #else
        res = AMMPI_SPMDSetThreadMode(0, &pstr, argc, argv);
      #endif
        if (!res) { 
          #if GASNETI_THREADS
          { static char tmsg[255];
            sprintf(tmsg, "*** WARNING: The pthreaded version of mpi-conduit requires an MPI implementation "
                          "which supports threading mode MPI_THREAD_SERIALIZED, "
                          "but this implementation reports it can only support %s\n", pstr);
            #if GASNET_DEBUG_VERBOSE
              /* only show this in verbose mode, because some versions of MPICH (eg Quadrics version)
                 lie and report THREAD_SINGLE, when in actuality MPI_THREAD_SERIALIZED works just fine */
              if (!gasneti_getenv_yesno_withdefault("GASNET_QUIET",0)) fprintf(stderr, "%s", tmsg);
            #else
              tmsgstr = tmsg;
            #endif
          }
          #else
            fprintf(stderr,"unknown failure in AMMPI_SPMDSetThreadMode() => %s\n",pstr);
          #endif
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

    /* enable tracing */
    gasneti_trace_init(argc, argv);
    GASNETI_AM_SAFE(AMMPI_SPMDSetExitCallback(gasnetc_traceoutput));
    if (pstr)    GASNETI_TRACE_PRINTF(C,("AMMPI_SPMDSetThreadMode/MPI_Init_thread()=>%s",pstr));
    if (tmsgstr) GASNETI_TRACE_PRINTF(I,("%s",tmsgstr));

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
       to process the AMMPI_SPMD control messages required for job shutdown
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
        INITERR(RESOURCE,"Error registering variable-index client handlers");

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

    /*  AMMPI allows arbitrary registration with no further action  */
    if (segsize) {
      retval = AM_SetSeg(gasnetc_endpoint, segbase, segsize);
      if (retval != AM_OK) INITERR(RESOURCE, "AM_SetSeg() failed");
    }
    #if GASNETC_HSL_ERRCHECK || GASNET_TRACE
      #if !GASNETC_HSL_ERRCHECK
        if (GASNETI_TRACE_ENABLED(A))
      #endif
          GASNETI_AM_SAFE(AMMPI_SetHandlerCallbacks(gasnetc_endpoint,
            gasnetc_enteringHandler_hook, gasnetc_leavingHandler_hook));
    #endif

    #if GASNETC_HSL_ERRCHECK
      gasnetc_hsl_attach(); /* must precede attach_done to avoid inf recursion on malloc/hold_interrupts */
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
       if (gasnetc_AMlock.owner == GASNETI_THREADIDQUERY()) break;
     #endif
     if (!gasneti_mutex_trylock(&gasnetc_AMlock)) break;
     gasneti_sched_yield();
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
  CHECKCALLNIS();
  return gasneti_getSegmentInfo(seginfo_table, numentries);
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
    int tmp; /* AMMPI wants an int, but gasnet_node_t is uint32_t */
    GASNETI_AM_SAFE_NORETURN(retval, AMMPI_GetSourceId(token, &tmp));
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
  CHECKCALLNIS();
#if GASNET_PSHM
  gasneti_AMPSHMPoll(0);
#endif
  AMLOCK();
    GASNETI_AM_SAFE_NORETURN(retval, AM_Poll(gasnetc_bundle));
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
  CHECKCALLNIS();
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
               AMMPI_RequestVA(gasnetc_endpoint, dest, handler, 
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
  CHECKCALLNIS();
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
               AMMPI_RequestIVA(gasnetc_endpoint, dest, handler, 
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
  CHECKCALLNIS();
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
               AMMPI_RequestXferVA(gasnetc_endpoint, dest, handler, 
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
  CHECKCALLHSL();
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
              AMMPI_ReplyVA(token, handler, numargs, argptr));
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
  CHECKCALLHSL();
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
              AMMPI_ReplyIVA(token, handler, source_addr, nbytes, numargs, argptr));
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
  
  CHECKCALLHSL();
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
              AMMPI_ReplyXferVA(token, handler, source_addr, nbytes, dest_offset, numargs, argptr));
  }
  va_end(argptr);
  if_pf (retval) GASNETI_RETURN_ERR(RESOURCE);
  else return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
/*
  No-interrupt sections
  =====================
*/
/* AMMPI does not use interrupts, but we provide an optional error-checking implementation of 
   handler-safe locks to assist in debugging client code
 */

#if GASNETC_USE_INTERRUPTS 
  #error Interrupts not implemented
#endif
#if GASNETC_HSL_ERRCHECK
  typedef struct { /* per-thread HSL err-checking info */
    gasnet_hsl_t *locksheld;
    int inExplicitNIS;
    unsigned int inhandler;
    int inuse;
    gasneti_tick_t NIStimestamp;
  } gasnetc_hsl_errcheckinfo_t;
  static gasnetc_hsl_errcheckinfo_t _info_init = { NULL, 0, 0, 0 };
  static gasnetc_hsl_errcheckinfo_t _info_free = { NULL, 0, 0, 0 };

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
           and initialized during gasnet_attach */
        static gasnetc_hsl_errcheckinfo_t *hsl_errcheck_table = NULL;
        static gasneti_mutex_t hsl_errcheck_tablelock = GASNETI_MUTEX_INITIALIZER;
        int maxthreads = gasneti_max_threads();
        int idx;
        gasneti_mutex_lock(&hsl_errcheck_tablelock);
          if (!hsl_errcheck_table) 
            hsl_errcheck_table = gasneti_calloc(maxthreads,sizeof(gasnetc_hsl_errcheckinfo_t));        
          for (idx = 0; idx < maxthreads; idx++) {
            if (!hsl_errcheck_table[idx].inuse) break;
          }
          if (idx == maxthreads) gasneti_fatal_threadoverflow("HSL errorcheck");
          gasneti_assert(idx < maxthreads);
          info = &(hsl_errcheck_table[idx]);
          gasneti_assert(!info->inuse);
          memcpy(info, &_info_init, sizeof(gasnetc_hsl_errcheckinfo_t));
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


  extern void gasnetc_hold_interrupts(void) {
    GASNETI_CHECKATTACH();
    { gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
      if_pf (info == &_info_free) return; /* TODO: assert that we are in the thread destruction path */
      if (info->inhandler) { /* NIS calls ignored within a handler */
        GASNETI_TRACE_PRINTF(I,("Warning: Called gasnet_hold_interrupts within a handler context -- call ignored"));
        return;
      }
      if (info->locksheld) { /* NIS calls ignored while holding an HSL */
        GASNETI_TRACE_PRINTF(I,("Warning: Called gasnet_hold_interrupts while holding an HSL -- call ignored"));
        return;
      }
      if (info->inExplicitNIS)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to disable interrupts when they were already disabled");
      info->inExplicitNIS = 1;
      info->NIStimestamp = gasneti_ticks_now();
    }
  }
  extern void gasnetc_resume_interrupts(void) {
    GASNETI_CHECKATTACH();
    { gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
      if_pf (info == &_info_free) return; /* TODO: assert that we are in the thread destruction path */
      if (info->inhandler) { /* NIS calls ignored within a handler */
        GASNETI_TRACE_PRINTF(I,("Warning: Called gasnet_resume_interrupts within a handler context -- call ignored"));
        return;
      }
      if (info->locksheld) { /* NIS calls ignored while holding an HSL */
        GASNETI_TRACE_PRINTF(I,("Warning: Called gasnet_resume_interrupts while holding an HSL -- call ignored"));
        return;
      }
      if (!info->inExplicitNIS)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to resume interrupts when they were not disabled");
      { float NIStime = gasneti_ticks_to_ns(gasneti_ticks_now() - info->NIStimestamp)/1000.0;
        if (NIStime > GASNETC_NISTIMEOUT_WARNING_THRESHOLD) {
          fprintf(stderr,"HSL USAGE WARNING: held a no-interrupt section for a long interval (%8.3f sec)\n", NIStime/1000000.0);
          fflush(stderr);
        }
      }
      info->inExplicitNIS = 0;
    }
  }

  void gasnetc_checkcallNIS(void) {
    gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
    if (info->inExplicitNIS)
      gasneti_fatalerror("Illegal call to GASNet within a No-Interrupt Section");
    if (info->inhandler)
      gasneti_fatalerror("Illegal call to GASNet within a No-Interrupt Section (imposed by handler context)");
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
extern void gasnetc_hsl_init   (gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();
  #if GASNETC_HSL_ERRCHECK
  {
    if (hsl->tag == GASNETC_HSL_ERRCHECK_TAGINIT)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gasnet_hsl_init() a statically-initialized HSL");
  #if 0
    /* this causes false errors in Titanium, because object destructors aren't implemented */
    if (hsl->tag == GASNETC_HSL_ERRCHECK_TAGDYN)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gasnet_hsl_init() a previously-initialized HSL (or one you forgot to destroy)");
  #endif
    hsl->tag = GASNETC_HSL_ERRCHECK_TAGDYN;
    hsl->next = NULL;
    hsl->islocked = 0;
  }
  #endif

  gasneti_mutex_init(&(hsl->lock));
}

extern void gasnetc_hsl_destroy(gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();
  #if GASNETC_HSL_ERRCHECK
  {
    if (hsl->tag != GASNETC_HSL_ERRCHECK_TAGINIT && hsl->tag != GASNETC_HSL_ERRCHECK_TAGDYN)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gasnet_hsl_destroy() an uninitialized HSL");
    if (hsl->islocked)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gasnet_hsl_destroy() a locked HSL");
    hsl->tag = 0;
    gasneti_assert(!hsl->next);
  }
  #endif

  gasneti_mutex_destroy(&(hsl->lock));
}

extern void gasnetc_hsl_lock   (gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();
  #if GASNETC_HSL_ERRCHECK
  { gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
    gasnet_hsl_t *heldhsl = info->locksheld;
    if (hsl->tag != GASNETC_HSL_ERRCHECK_TAGINIT && hsl->tag != GASNETC_HSL_ERRCHECK_TAGDYN)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gasnet_hsl_lock() an uninitialized HSL");
    while (heldhsl) {
      if (heldhsl == hsl)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to recursively gasnet_hsl_lock() an HSL");
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

extern void gasnetc_hsl_unlock (gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();
  #if GASNETC_HSL_ERRCHECK
  { gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
    gasnet_hsl_t *heldhsl = info->locksheld;
    if (hsl->tag != GASNETC_HSL_ERRCHECK_TAGINIT && hsl->tag != GASNETC_HSL_ERRCHECK_TAGDYN)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gasnet_hsl_unlock() an uninitialized HSL");
    while (heldhsl) {
      if (heldhsl == hsl) break;
      heldhsl = heldhsl->next;
    }
    if (!heldhsl)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gasnet_hsl_unlock() an HSL I didn't own");
    if (info->locksheld != hsl)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gasnet_hsl_unlock() an HSL out of order");
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

extern int  gasnetc_hsl_trylock(gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();

  #if GASNETC_HSL_ERRCHECK
  { gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
    gasnet_hsl_t *heldhsl = info->locksheld;
    if (hsl->tag != GASNETC_HSL_ERRCHECK_TAGINIT && hsl->tag != GASNETC_HSL_ERRCHECK_TAGDYN)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to gasnet_hsl_trylock() an uninitialized HSL");
    while (heldhsl) {
      if (heldhsl == hsl)
        gasneti_fatalerror("HSL USAGE VIOLATION: tried to recursively gasnet_hsl_trylock() an HSL");
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

#if (!GASNETC_NULL_HSL && GASNETC_HSL_ERRCHECK) || GASNET_TRACE
  /* called when entering/leaving handler - also called when entering/leaving AM_Reply call */
  extern void gasnetc_enteringHandler_hook(ammpi_category_t cat, int isReq, int handlerId, void *token, 
                                           void *buf, size_t nbytes, int numargs, uint32_t *args) {
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
      default: gasneti_fatalerror("Unknown handler type in gasnetc_enteringHandler_hook(): %i", cat);
    }
    #if (!GASNETC_NULL_HSL && GASNETC_HSL_ERRCHECK)
    {
      gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
      if (info->locksheld)
          gasneti_fatalerror("HSL USAGE VIOLATION: tried to make a GASNet network call while holding an HSL");
      if (info->inExplicitNIS)
          gasneti_fatalerror("HSL USAGE VIOLATION: tried to make a GASNet network call with interrupts disabled");
      info->inhandler++;
    }
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
      default: gasneti_fatalerror("Unknown handler type in gasnetc_leavingHandler_hook(): %i", cat);
    }
    #if (!GASNETC_NULL_HSL && GASNETC_HSL_ERRCHECK)
    {
      gasnetc_hsl_errcheckinfo_t *info = gasnetc_get_errcheckinfo();
      gasneti_assert(info->inhandler > 0);
      gasneti_assert(!info->inExplicitNIS);
      if (info->locksheld)
          gasneti_fatalerror("HSL USAGE VIOLATION: tried to exit a handler while holding an HSL");
      info->inhandler--;
    }
    #endif
  }
#endif
/* ------------------------------------------------------------------------------------ */
/*
  Private Handlers:
  ================
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
