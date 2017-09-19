/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ofi-conduit/gasnet_core.c $
 * Description: GASNet libfabric (OFI) conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2015, Intel Corporation
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#include <gasnet_core_internal.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include <gasnet_ofi.h>

GASNETI_IDENT(gasnetc_IdentString_Version, "$GASNetCoreLibraryVersion: " GASNET_CORE_VERSION_STR " $");
GASNETI_IDENT(gasnetc_IdentString_Name,    "$GASNetCoreLibraryName: " GASNET_CORE_NAME_STR " $");

gasnet_handlerentry_t const *gasnetc_get_handlertable(void);
#if HAVE_ON_EXIT
static void gasnetc_on_exit(int, void*);
#else
static void gasnetc_atexit(void);
#endif

/* Exit coordination timeouts */
#define GASNETC_DEFAULT_EXITTIMEOUT_MAX         360.0   /* 6 minutes! */
#define GASNETC_DEFAULT_EXITTIMEOUT_MIN         10      /* 10 seconds */
#define GASNETC_DEFAULT_EXITTIMEOUT_FACTOR      0.25    /* 1/4 second */
static double gasnetc_exittimeout = GASNETC_DEFAULT_EXITTIMEOUT_MAX;

gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS]; /* handler table (recommended impl) */

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* called at startup to check configuration sanity */
static void gasnetc_check_config(void) {
  gasneti_check_config_preinit();
}

static int gasnetc_init(int *argc, char ***argv) 
{
  int ret = GASNET_OK;
  /*  check system sanity */
  gasnetc_check_config();

  if (gasneti_init_done) 
    GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already initialized");

  gasneti_freezeForDebugger();

  #if GASNET_DEBUG_VERBOSE
    /* note - can't call trace macros during gasnet_init because trace system not yet initialized */
    fprintf(stderr,"gasnetc_init(): about to spawn...\n"); fflush(stderr);
  #endif

  /* bootstrap the nodes for ofi conduit */
  ret = gasnetc_ofi_init(argc, argv, &gasneti_nodes, &gasneti_mynode);
  if (GASNET_OK != ret)
	 return ret;

  #if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_init(): spawn successful - node %i/%i starting...\n", 
      gasneti_mynode, gasneti_nodes); fflush(stderr);
  #endif

  #if GASNET_PSHM
  gasneti_pshm_init(gasneti_bootstrapSNodeBroadcast, 0);
  #endif

  #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
  { uintptr_t limit;
    limit = gasneti_mmapLimit((uintptr_t)-1, (uint64_t)-1,
                              gasneti_bootstrapExchange,
                              gasneti_bootstrapBarrier);
    gasneti_segmentInit(limit, gasneti_bootstrapExchange);
  }
  #elif GASNET_SEGMENT_EVERYTHING
    /* segment is everything - nothing to do */
  #else
    #error Bad segment config
  #endif

  gasneti_init_done = 1;  

  gasneti_auxseg_init(); /* adjust max seg values based on auxseg */

  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
extern int gasnet_init(int *argc, char ***argv) 
{
  int retval = gasnetc_init(argc, argv);
  if (retval != GASNET_OK) GASNETI_RETURN(retval);
  gasneti_trace_init(argc, argv);
  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int gasnetc_attach(gasnet_handlerentry_t *table, int numentries,
                          uintptr_t segsize, uintptr_t minheapoffset) {
  void *segbase = NULL;
  
  GASNETI_TRACE_PRINTF(C,("gasnetc_attach(table (%i entries), segsize=%"PRIuPTR", minheapoffset=%"PRIuPTR")",
                          numentries, segsize, minheapoffset));

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
    if (gasneti_amregister(ctable, len, 1, 63, 0, &numreg) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering core API handlers");
    gasneti_assert(numreg == len);
  }

  { /*  extended API handlers */
    gasnet_handlerentry_t *etable = (gasnet_handlerentry_t *)gasnete_get_handlertable();
    int len = 0;
    int numreg = 0;
    gasneti_assert(etable);
    while (etable[len].fnptr) len++; /* calc len */
    if (gasneti_amregister(etable, len, 64, 127, 0, &numreg) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering extended API handlers");
    gasneti_assert(numreg == len);
  }

  if (table) { /*  client handlers */
    int numreg1 = 0;
    int numreg2 = 0;

    /*  first pass - assign all fixed-index handlers */
    if (gasneti_amregister(table, numentries, 128, 255, 0, &numreg1) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering fixed-index client handlers");

    /*  second pass - fill in dontcare-index handlers */
    if (gasneti_amregister(table, numentries, 128, 255, 1, &numreg2) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering variable-index client handlers");

    gasneti_assert(numreg1 + numreg2 == numentries);
  }

  /* ------------------------------------------------------------------------------------ */
  /*  register fatal signal handlers */

  /* catch fatal signals and convert to SIGQUIT */
  gasneti_registerSignalHandlers(gasneti_defaultSignalHandler);

  /* ------------------------------------------------------------------------------------ */
  /*  setup fo rexit coordination */

  gasnetc_exittimeout = gasneti_get_exittimeout(GASNETC_DEFAULT_EXITTIMEOUT_MAX,
                                                GASNETC_DEFAULT_EXITTIMEOUT_MIN,
                                                GASNETC_DEFAULT_EXITTIMEOUT_FACTOR,
                                                GASNETC_DEFAULT_EXITTIMEOUT_MIN);
  #if HAVE_ON_EXIT
    on_exit(gasnetc_on_exit, NULL);
  #else
    atexit(gasnetc_atexit);
  #endif

  /* ------------------------------------------------------------------------------------ */
  /*  register segment  */

  gasneti_seginfo = (gasnet_seginfo_t *)gasneti_malloc(gasneti_nodes*sizeof(gasnet_seginfo_t));
  gasneti_leak(gasneti_seginfo);

  #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    if (segsize == 0) segbase = NULL; /* no segment */
    else {
      gasneti_segmentAttach(segsize, minheapoffset, gasneti_seginfo, gasneti_bootstrapExchange);
      segbase = gasneti_seginfo[gasneti_mynode].addr;
      segsize = gasneti_seginfo[gasneti_mynode].size;
      gasneti_assert(((uintptr_t)segbase) % GASNET_PAGESIZE == 0);
      gasneti_assert(segsize % GASNET_PAGESIZE == 0);
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
  }
  #endif

  gasnetc_ofi_attach(segbase, segsize);

  /* After local segment is attached, call optional client-provided hook
     (###) should call BEFORE any conduit-specific pinning/registration of the segment
   */
  if (gasnet_client_attach_hook) {
    gasnet_client_attach_hook(segbase, segsize);
  }

  /* ------------------------------------------------------------------------------------ */
  /*  primary attach complete */
  gasneti_attach_done = 1;
  gasneti_bootstrapBarrier();

  GASNETI_TRACE_PRINTF(C,("gasnetc_attach(): primary attach complete"));

  gasneti_assert(gasneti_seginfo[gasneti_mynode].addr == segbase &&
         gasneti_seginfo[gasneti_mynode].size == segsize);

  /* (###) exchange_fn is optional (may be NULL) and is only used with GASNET_SEGMENT_EVERYTHING
           if your conduit has an optimized bootstrapExchange pass it in place of NULL
   */
  gasneti_auxseg_attach(NULL); /* provide auxseg */

  gasnete_init(); /* init the extended API */

  gasneti_nodemapFini();

  /* ensure extended API is initialized across nodes */
  gasneti_bootstrapBarrier();

  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
int gasnetc_exit_in_progress = 0;

#if HAVE_ON_EXIT
static void gasnetc_on_exit(int exitcode, void *arg) {
  if (!gasnetc_exit_in_progress)
    gasnetc_exit(exitcode);
}
#else
static void gasnetc_atexit(void) {
  if (!gasnetc_exit_in_progress)
    gasnetc_exit(0);
}
#endif

/* This signal handler is for a last-ditch exit when a signal arrives while
 * attempting the graceful exit.  That includes SIGALRM if we get wedged.
 * DOES NOT RETURN
 */
static void gasnetc_exit_sighandler(int sig) {
  static int once = 1;

  if (once) {
    /* We ask the bootstrap support to kill us, but only once */
    once = 0;
    gasneti_reghandler(SIGALRM, gasnetc_exit_sighandler);
    alarm(5);
    gasneti_bootstrapAbort(127);
  } else {
    gasneti_killmyprocess(127);
    gasneti_reghandler(SIGABRT, SIG_DFL);
    gasneti_fatalerror("gasnetc_exit aborting...");
  }

  /* NOT REACHED */
}

/* AM Handlers for exit handling */
static void gasnetc_noop(void) { return; }
static gasneti_atomic_t gasnetc_exit_dist = gasneti_atomic_init(0);     /* OR of reduce distances */
static void gasnetc_exit_reqh(gasnet_token_t token,
                              gasnet_handlerarg_t arg0,
                              gasnet_handlerarg_t arg1) {
  gasneti_atomic_val_t distance = arg1;
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

  if (!gasnetc_exit_in_progress)
    gasnetc_exit((int)arg0);
}

/* Coordinate a global exit, returning non-zero on success */
static int gasnetc_exit_coordinate(int exitcode) {
  /* Disable processing of user's AMs, to avoid reentrance if user's handler exits */
  for (int i = GASNETE_HANDLER_BASE; i < GASNETC_MAX_NUMHANDLERS; ++i) {
    gasnetc_handler[i] = (gasneti_handler_fn_t)&gasnetc_noop;
  }

  /* Coordinate using dissemination-pattern, with timeout.
   * lg(N) rounds each of which sends and recvs 1 AM
   */
  const uint64_t timeout_ns = gasnetc_exittimeout * 1000000000L;
  const gasneti_tick_t t_start = gasneti_ticks_now();
  for (int distance = 1; distance < gasneti_nodes; distance *= 2) {
    int peer = (gasneti_mynode + distance) % gasneti_nodes;
    int ret = gasnetc_AMRequestShortM(peer, gasneti_handleridx(gasnetc_exit_reqh),
                                      2, exitcode, distance);
    if (ret != GASNET_OK) return 0;
    do { /* wait for completion of the proper receive, which might arrive out of order */
      if (timeout_ns < gasneti_ticks_to_ns(gasneti_ticks_now() - t_start)) return 0;
      gasnetc_AMPoll();
    } while (!(distance & gasneti_atomic_read(&gasnetc_exit_dist, 0)));
  }
  return 1;
}

extern void gasnetc_exit(int exitcode) {
  gasnetc_exit_in_progress = 1;
  gasneti_sync_writes();

  /* once we start a shutdown, ignore all future SIGQUIT signals or we risk reentrancy */
  gasneti_reghandler(SIGQUIT, SIG_IGN);

  {  /* ensure only one thread ever continues past this point */
    static gasneti_mutex_t exit_lock = GASNETI_MUTEX_INITIALIZER;
    gasneti_mutex_lock(&exit_lock);
  }

  GASNETI_TRACE_PRINTF(C,("gasnet_exit(%i)\n", exitcode));

  /* Establish a last-ditch signal handler in case of failure. */
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

  /* Prior to attach we cannot send AMs to coordinate the exit */
  if (! gasneti_attach_done) {
    fprintf(stderr, "WARNING: GASNet ofi-conduit may not shutdown cleanly when gasnet_exit() is called before gasnet_attach()\n");
    gasneti_bootstrapAbort(exitcode);
    gasneti_killmyprocess(exitcode);
  }

  const int timeout = (unsigned int)gasnetc_exittimeout;
  alarm(2 + timeout);
  if (gasnetc_exit_coordinate(exitcode)) {
    alarm(timeout);
    gasnetc_ofi_exit();
  }
  alarm(0);

  gasneti_flush_streams();
  gasneti_trace_finish();
  gasneti_sched_yield();

  alarm(timeout);
  gasneti_bootstrapFini();
  alarm(0);
  gasneti_killmyprocess(exitcode);
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

extern int gasnetc_AMGetMsgSource(gasnet_token_t token, gasnet_node_t *srcindex) {
  gasnet_node_t sourceid;
  GASNETI_CHECKATTACH();
  GASNETI_CHECK_ERRR((!token),BAD_ARG,"bad token");
  GASNETI_CHECK_ERRR((!srcindex),BAD_ARG,"bad src ptr");

#if GASNET_PSHM
  /* If your conduit will support PSHM, let the PSHM code
   * have a chance to recognize the token first, as shown here. */
  if (gasneti_AMPSHMGetMsgSource(token, &sourceid) != GASNET_OK)
#endif
  {
    /* add code here to write the source index into sourceid. */
    sourceid = ((gasnetc_ofi_am_send_buf_t*)token)->sourceid;
  }

  gasneti_assert(sourceid < gasneti_nodes);
  *srcindex = sourceid;
  return GASNET_OK;
}

extern int gasnetc_AMPoll(void) {
  int retval;
  GASNETI_CHECKATTACH();

#if GASNET_PSHM
  /* If your conduit will support PSHM, let it make progress here. */
  gasneti_AMPSHMPoll(0);
#endif

  /* add code here to run your AM progress engine */
  /* should be a generic polling */
  gasnetc_ofi_poll();

  return GASNET_OK;
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
  gasneti_AMPoll(); /* poll at least once, to assure forward progress */
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Short, dest, handler,
                                           0, 0, 0,
                                           numargs, argptr);
  } else
#endif
  {
    /* OFI send short active message */
    retval = gasnetc_ofi_am_send_short(dest, handler, numargs, argptr, 1);
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestMediumM( 
                            gasnet_node_t dest,      /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTMEDIUM(dest,handler,source_addr,nbytes,numargs);
  gasneti_AMPoll(); /* poll at least once, to assure forward progress */
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Medium, dest, handler,
                                           source_addr, nbytes, 0,
                                           numargs, argptr);
  } else
#endif
  {
    retval = gasnetc_ofi_am_send_medium(dest, handler, source_addr, nbytes, numargs, argptr, 1);
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestLongM( gasnet_node_t dest,        /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTLONG(dest,handler,source_addr,nbytes,dest_addr,numargs);
  gasneti_AMPoll(); /* poll at least once, to assure forward progress */
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Long, dest, handler,
                                           source_addr, nbytes, dest_addr,
                                           numargs, argptr);
  } else
#endif
  {
    retval = gasnetc_ofi_am_send_long(dest, handler, source_addr, nbytes, dest_addr, numargs, argptr, 1, 0);
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestLongAsyncM( gasnet_node_t dest,        /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTLONGASYNC(dest,handler,source_addr,nbytes,dest_addr,numargs);
  gasneti_AMPoll(); /* poll at least once, to assure forward progress */
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Long, dest, handler,
                                           source_addr, nbytes, dest_addr,
                                           numargs, argptr);
  } else
#endif
  {
    retval = gasnetc_ofi_am_send_long(dest, handler, source_addr, nbytes, dest_addr, numargs, argptr, 1, 1);
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
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
    retval = gasnetc_ofi_am_send_short(((gasnetc_ofi_am_send_buf_t*)token)->sourceid, handler, numargs, argptr, 0);
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
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
    retval = gasnetc_ofi_am_send_medium(((gasnetc_ofi_am_send_buf_t*)token)->sourceid, handler, source_addr, nbytes, numargs, argptr, 0);
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
    retval = gasnetc_ofi_am_send_long(((gasnetc_ofi_am_send_buf_t*)token)->sourceid, handler, source_addr, nbytes, dest_addr, numargs, argptr, 0, 0);
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

/* ------------------------------------------------------------------------------------ */
/*
  No-interrupt sections
  =====================
  This section is only required for conduits that may use interrupt-based handler dispatch
  See the GASNet spec and http://gasnet.lbl.gov/dist/docs/gasnet.html for
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
  gasneti_handler_tableentry_no_bits(gasnetc_exit_reqh),

  /* ptr-width dependent handlers */

  { 0, NULL }
};

gasnet_handlerentry_t const *gasnetc_get_handlertable(void) {
  return gasnetc_handlers;
}

/* ------------------------------------------------------------------------------------ */
