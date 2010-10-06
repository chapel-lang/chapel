/*   $Source: /var/local/cvs/gasnet/smp-conduit/gasnet_core.c,v $
 *     $Date: 2010/04/17 02:36:40 $
 * $Revision: 1.51.2.3 $
 * Description: GASNet smp conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
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
#if HAVE_ON_EXIT
static void gasnetc_on_exit(int, void*);
#else
static void gasnetc_atexit(void);
#endif

#if !GASNETI_CLIENT_THREADS
  void *_gasnetc_mythread = NULL;
#endif

gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS]; /* handler table (recommended impl) */

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* called at startup to check configuration sanity */
static void gasnetc_check_config(void) {
  gasneti_check_config_preinit();

  /* add code to do some sanity checks on the number of nodes, handlers
   * and/or segment sizes */ 
}

static void gasnetc_bootstrapExchange(void *src, size_t len, void *dest) {
  #if GASNET_PSHM
    gasneti_assert(gasneti_request_pshmnet != NULL);
    gasneti_pshmnet_bootstrapExchange(gasneti_request_pshmnet, src, len, dest);
  #else
    gasneti_assert(gasneti_nodes == 1); /* trivial because we only have one node */
    memmove(dest, src, len);
  #endif
}

static void gasnetc_bootstrapBroadcast(void *src, size_t len, void *dest, int rootnode) {
  #if GASNET_PSHM
    gasneti_assert(gasneti_request_pshmnet != NULL);
    gasneti_pshmnet_bootstrapBroadcast(gasneti_request_pshmnet, src, len, dest, rootnode);
  #else
    gasneti_assert(gasneti_nodes == 1); /* trivial because we only have one node */
    gasneti_assert(rootnode == 0);
    memmove(dest, src, len);
  #endif
}

static void gasnetc_bootstrapBarrier(void) {
  /* add code here to implement an external barrier 
      this barrier should not rely on AM or the GASNet API because it's used 
      during bootstrapping before such things are fully functional
     It need not be particularly efficient, because we only call it a few times
      and only during bootstrapping - it just has to work correctly
     If your underlying spawning or batch system provides barrier functionality,
      that would probably be a good choice for this
   */
  #if GASNET_PSHM
    gasneti_pshmnet_bootstrapBarrier();
  #else
    gasneti_assert(gasneti_nodes == 1); /* trivial because we only have one node */
  #endif
}

#if GASNET_PSHM
#include <sys/types.h>
#include <sys/wait.h>

/* exit timeout is limit on inter-exit gap, not first-to-last interval */
#define GASNETC_DEFAULT_EXITTIMEOUT_MAX       20.
#define GASNETC_DEFAULT_EXITTIMEOUT_MIN       10.
#define GASNETC_DEFAULT_EXITTIMEOUT_FACTOR     0.25

static volatile sig_atomic_t *gasnetc_child_tbl;
static volatile sig_atomic_t gasnetc_child_count = 0;
static volatile sig_atomic_t gasnetc_exit_timeout = (sig_atomic_t)GASNETC_DEFAULT_EXITTIMEOUT_MAX;

#ifndef GASNETC_REMOTEEXIT_SIGNAL
  #ifdef SIGURG
    #define GASNETC_REMOTEEXIT_SIGNAL  SIGURG
  #else
    #define GASNETC_REMOTEEXIT_SIGNAL  SIGUSR1
  #endif
#endif

/* TODO: use a process group (would require SIGT{STP,TIN,TOU} handling) */
static void gasnetc_signal_job(int sig) {
  int i;
  for (i = 0; i < gasnetc_child_count; i++) {
    (void)kill(gasnetc_child_tbl[i], sig);
    (void)kill(gasnetc_child_tbl[i], SIGCONT);
  }
}

static void gasnetc_exit_sighand(int sig_recvd) {
  int sig_to_send = sig_recvd;
  switch (sig_recvd) {
    case SIGABRT: case SIGILL: case SIGSEGV: case SIGBUS: case SIGFPE:
      /* These signals indicates a bug in this "parent". */
      fprintf(stderr, "ERROR: Parent process received fatal signal %d - Terminating\n", sig_recvd);
      sig_to_send = SIGTERM;
      break;

    case SIGALRM: {
      /* This signal indicates a non-collective exit */
      static int count = 0;
      switch (count++) {
        case 0:  sig_to_send = GASNETC_REMOTEEXIT_SIGNAL; break;
        case 1:  sig_to_send = SIGTERM; break;
        default: sig_to_send = SIGKILL; break;
      }
      alarm(gasnetc_exit_timeout);
      break;
    }
  }

  /* Signals are forwarded to the job */
  gasnetc_signal_job(sig_to_send);

  /* rearm */
  gasneti_reghandler(sig_recvd, gasnetc_exit_sighand);
}

/* NOTE: This runs in the childen */
static void gasnetc_remote_exit_sighand(int sig) {
  gasneti_sighandlerfn_t handler;

  /* Run the SIGQUIT handler, if any */
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
  }

  gasnetc_exit(99);
}

static void gasnetc_fork_children(void) {
  int gasnetc_exit_code = 0;
  int i, rc;

  gasnetc_child_tbl = gasneti_malloc(gasneti_nodes * sizeof(sig_atomic_t));

  for (i = 0; i < gasneti_nodes; i++) {
    int fork_return = fork();
    if (fork_return < 0) {
      gasnetc_signal_job(SIGTERM);
      gasneti_fatalerror("Fork failed!");
    }
    if (fork_return) {
      /* I am parent */
      gasnetc_child_tbl[gasnetc_child_count++] = fork_return;
    } else {
      /* I am child */
      gasneti_reghandler(GASNETC_REMOTEEXIT_SIGNAL, gasnetc_remote_exit_sighand);
      gasneti_free((void*)gasnetc_child_tbl);
      gasneti_mynode = i; 
      if (gasneti_mynode != 0) {
        if (freopen("/dev/null", "r", stdin) != stdin) {
          gasneti_fatalerror("GASNet node %d failed to redirect STDIN", i);
        }
      }
      return;
    }
  }

  /* If I get here I am the parent and NOT a gasnet application process */

  if (freopen("/dev/null", "r", stdin) != stdin) {
    gasnetc_signal_job(SIGTERM);
    gasneti_fatalerror("Master process failed to redirect STDIN");
  }

  gasneti_registerSignalHandlers(gasnetc_exit_sighand);
  gasneti_reghandler(SIGALRM, gasnetc_exit_sighand);
  gasneti_reghandler(SIGCHLD, SIG_DFL);

  gasnetc_exit_timeout = gasneti_get_exittimeout(GASNETC_DEFAULT_EXITTIMEOUT_MAX,
                                                 GASNETC_DEFAULT_EXITTIMEOUT_MIN,
                                                 GASNETC_DEFAULT_EXITTIMEOUT_FACTOR,
                                                 GASNETC_DEFAULT_EXITTIMEOUT_MIN);

  while (gasnetc_child_count) {
    int status;

    rc = wait(&status);
    if (rc < 0) {
      if (errno == EINTR) continue;

      gasnetc_signal_job(SIGTERM);
      gasneti_fatalerror("Unexpected wait() failure: %d(%s) - Terminating job", errno, strerror(errno));
    }

    /* Remove the deceased from the table of children */
    gasnetc_child_count -= 1;
    for (i = 0; i < gasnetc_child_count; ++i) {
      if (gasnetc_child_tbl[i] == rc) {
        gasnetc_child_tbl[i] = gasnetc_child_tbl[gasnetc_child_count];
        break;
      }
    }

    /* Job will return w/ first non-zero exit code we observe */
    rc = 0;
    if (WIFEXITED(status)) {
      rc = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
      gasnetc_exit_code = 254;
    }
    if (rc && !gasnetc_exit_code) {
      gasnetc_exit_code = rc;
    }

    alarm(gasnetc_exit_timeout);
  }

  gasneti_registerSignalHandlers(SIG_DFL);
  alarm(0);

  gasneti_killmyprocess(gasnetc_exit_code);
}

static int gasnetc_get_pshm_nodecount(void)
{
  gasnet_node_t nodes = gasneti_getenv_int_withdefault("GASNET_PSHM_NODES", 0, 0);
  int polite_wait, politedefault;

  if (nodes > GASNETI_PSHM_MAX_NODES) { 
    gasneti_fatalerror("Nodes requested (%d) > maximum (%d)", nodes,
                       GASNETI_PSHM_MAX_NODES);
  } else if (nodes == 0) {
    fprintf(stderr, "Warning: GASNET_PSHM_NODES not specified: running with 1 node\n");
    nodes = 1;
  }

  /* Set up 'polite' synchronization if nodes > CPU's and/or user specifies
   * setting */
  politedefault = gasnett_cpu_count() > 0 && nodes > gasnett_cpu_count();
  polite_wait = gasnett_getenv_yesno_withdefault("GASNET_POLITE_SYNC",politedefault);
  if (politedefault) {
    fprintf(stderr,
      "WARNING: Running more processes (%i) than there are physical CPU's (%i)\n",
       nodes, gasnett_cpu_count());
    if (polite_wait) {
      fprintf(stderr,
        "         enabling \"polite\" synchronization algorithms\n");
    } else {
      fprintf(stderr,
        "         but setting GASNET_POLITE_SYNC=\"%s\" in your environment has\n"
        "         disabled \"polite\" synchronization algorithms\n"
        "         Results of this run are not suitable for benchmarking\n",
        gasnet_getenv("GASNET_POLITE_SYNC"));
    }
  } else if (polite_wait) {
    fprintf(stderr,"WARNING: GASNET_POLITE_SYNC=\"%s\" is set in your environment\n"
        "         enabling \"polite\", low-performance synchronization algorithms\n",
        gasnet_getenv("GASNET_POLITE_SYNC"));
  }
  fflush(stderr);
  gasnet_set_waitmode(polite_wait ? GASNET_WAIT_BLOCK : GASNET_WAIT_SPIN);
  return nodes;
}

#endif 

static int gasnetc_init(int *argc, char ***argv) {
  /*  check system sanity */
  gasnetc_check_config();

  if (gasneti_init_done) 
    GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already initialized");
  gasneti_init_done = 1; /* enable early to allow tracing */

    gasneti_freezeForDebugger();

  #if GASNET_DEBUG_VERBOSE
    /* note - can't call trace macros during gasnet_init because trace system not yet initialized */
    fprintf(stderr,"gasnetc_init(): about to spawn...\n"); fflush(stderr);
  #endif

  /* add code here to bootstrap the nodes for your conduit */

  gasneti_mynode = 0;
  gasneti_nodes = 1;

#if GASNET_PSHM
  gasneti_nodes = gasnetc_get_pshm_nodecount();

  /* Create unique names for shmem files.
   * We do this here, since we get a chicken-and-egg problem if we
   * were to call gasneti_pshm_init() with our bootstrapExchange.
   * PLUS its just plain simpler to do this pre-fork().
   */
  gasneti_pshm_nodes = gasneti_nodes;
  (void)gasneti_pshm_makenames(NULL);

  /* A fork in the road! */
  gasnetc_fork_children();
#endif

  /* enable tracing */
  gasneti_trace_init(argc, argv);

  /* Trivial all-zero nodemap */
  gasneti_nodemap = gasneti_calloc(gasneti_nodes, sizeof(gasnet_node_t));
  gasneti_nodemapParse();

  #if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_init(): spawn successful - node %i/%i starting...\n", 
      gasneti_mynode, gasneti_nodes); fflush(stderr);
  #endif

  #if GASNET_PSHM
    gasneti_pshm_init(NULL, 0);
  #endif

  #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
      { uintptr_t limit;
        #if HAVE_MMAP && GASNET_PSHM
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

  #if 0
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

  gasneti_auxseg_init(); /* adjust max seg values based on auxseg */

  return GASNET_OK;
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
    /*  add code here to register table[i].fnptr 
             on index (gasnet_handler_t)newindex */
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

  /*   register any custom signal handlers required by your conduit 
   *        (e.g. to support interrupt-based messaging)
   */

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
    gasneti_assert(((uintptr_t)gasneti_seginfo[gasneti_mynode].addr) % GASNET_PAGESIZE == 0);
    gasneti_assert(gasneti_seginfo[gasneti_mynode].size % GASNET_PAGESIZE == 0);
  #else
    /* GASNET_SEGMENT_EVERYTHING */
    { int i;
      for (i=0;i<gasneti_nodes;i++) {
        gasneti_seginfo[i].addr = (void *)0;
        gasneti_seginfo[i].size = (uintptr_t)-1;
      }
    }
  #endif
  segbase = gasneti_seginfo[gasneti_mynode].addr;
  segsize = gasneti_seginfo[gasneti_mynode].size;

  /* ------------------------------------------------------------------------------------ */
  /*  gather segment information */

  /*  add code here to gather the segment assignment info into 
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

extern void gasnetc_exit(int exitcode) {
  /* once we start a shutdown, ignore all future SIGQUIT signals or we risk reentrancy */
  gasneti_reghandler(SIGQUIT, SIG_IGN);

#if GASNET_PSHM
  /* same goes for the remote exit signal */
  gasneti_reghandler(GASNETC_REMOTEEXIT_SIGNAL, SIG_IGN);
#endif

  {  /* ensure only one thread ever continues past this point */
    static gasneti_mutex_t exit_lock = GASNETI_MUTEX_INITIALIZER;
    gasneti_mutex_lock(&exit_lock);
  }

  GASNETI_TRACE_PRINTF(C,("gasnet_exit(%i)\n", exitcode));

  gasneti_flush_streams();
  gasneti_trace_finish();
  gasneti_sched_yield();

  /*  add code here to terminate the job across _all_ nodes 
           with gasneti_killmyprocess(exitcode) (not regular exit()), preferably
           after raising a SIGQUIT to inform the client of the exit
  */

  gasneti_killmyprocess(exitcode);
}

/* ------------------------------------------------------------------------------------ */
/*
  Misc. Active Message Functions
  ==============================
*/
extern int gasnetc_AMGetMsgSource(gasnet_token_t token, gasnet_node_t *srcindex) {
  gasnet_node_t sourceid;
  GASNETI_CHECKATTACH();
  #if GASNET_DEBUG || GASNET_PSHM
    GASNETI_CHECK_ERRR((!token),BAD_ARG,"bad token");
  #else
    GASNETI_CHECK_ERRR((token),BAD_ARG,"bad token");
  #endif
  GASNETI_CHECK_ERRR((!srcindex),BAD_ARG,"bad src ptr");

  /* add code here to write the source index into sourceid */
#if GASNET_PSHM
  GASNETI_SAFE_PROPAGATE(gasneti_AMPSHMGetMsgSource(token, &sourceid));
#else
  sourceid = 0;
#endif

  gasneti_assert(sourceid < gasneti_nodes);
  *srcindex = sourceid;
  return GASNET_OK;
}

#if GASNET_PSHM 
extern int gasnetc_AMPoll(void) {
  GASNETI_CHECKATTACH();
  return gasneti_AMPSHMPoll(0);
}
#else
/* no polling required for smp-conduit */
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
*/

static void gasnetc_cleanup_threaddata(void *_td) {
  void **corethreadinfo = (void **)_td;
  gasneti_free_aligned(*corethreadinfo);
  *corethreadinfo = NULL;
}

GASNETI_INLINE(gasnetc_ReqRepGeneric)
int gasnetc_ReqRepGeneric(gasnetc_category_t category, int isReq,
                         int dest, gasnet_handler_t handler, 
                         void *source_addr, int nbytes, void *dest_ptr, 
                         int numargs, va_list argptr) {
  gasnet_handlerarg_t pargs[GASNETC_MAX_ARGS];
  #if GASNET_DEBUG  
    gasnetc_bufdesc_t _descbuf; 
    gasnetc_bufdesc_t *desc = &_descbuf;
    desc->isReq = isReq;
    desc->handlerRunning = 1;
    desc->replyIssued = 0;
  #else
    void * const desc = NULL;
  #endif

  gasneti_assert(dest == gasneti_mynode);
  gasneti_assert(numargs >= 0 && numargs <= GASNETC_MAX_ARGS);

  { int i;
    for(i=0; i < numargs; i++) {
      pargs[i] = (gasnet_handlerarg_t)va_arg(argptr, int);
    }
  }

  switch (category) {
    case gasnetc_Short:
      { 
        GASNETI_RUN_HANDLER_SHORT(isReq,handler,gasnetc_handler[handler],desc,pargs,numargs);
      }
    break;
    case gasnetc_Medium:
      { 
        void **corethreadinfo = gasnetc_mythread();
        uint8_t *buf = NULL;
        gasneti_assert(corethreadinfo);
        if (!*corethreadinfo) { /* ensure 8-byte alignment of medium payload */
          *corethreadinfo = gasneti_malloc_aligned(GASNETI_MEDBUF_ALIGNMENT,sizeof(gasnetc_threadinfo_t));
          gasnete_register_threadcleanup(gasnetc_cleanup_threaddata, corethreadinfo);
        }
        if (isReq) buf = ((gasnetc_threadinfo_t *)*corethreadinfo)->requestBuf;
        else       buf = ((gasnetc_threadinfo_t *)*corethreadinfo)->replyBuf;

        memcpy(buf, source_addr, nbytes);

        GASNETI_RUN_HANDLER_MEDIUM(isReq,handler,gasnetc_handler[handler],desc,pargs,numargs,buf,nbytes);
      }
    break;
    case gasnetc_Long:
      { 
        if_pt(dest_ptr != source_addr) memcpy(dest_ptr, source_addr, nbytes);

        GASNETI_RUN_HANDLER_LONG(isReq,handler,gasnetc_handler[handler],desc,pargs,numargs,dest_ptr,nbytes);
      }
    break;
    default: gasneti_fatalerror("bad AM category");
  }
  #if GASNET_DEBUG  
    desc->handlerRunning = 0;
  #endif
  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
static int gasnetc_RequestGeneric(gasnetc_category_t category, 
                         int dest, gasnet_handler_t handler, 
                         void *source_addr, int nbytes, void *dest_ptr, 
                         int numargs, va_list argptr) {
  gasneti_AMPoll(); /* ensure progress */

#if GASNET_PSHM
  return gasneti_AMPSHM_RequestGeneric(category, dest, handler, source_addr, nbytes, 
                                      dest_ptr, numargs, argptr); 
#else
  return gasnetc_ReqRepGeneric(category, 1, dest, handler, 
                               source_addr, nbytes, dest_ptr, 
                               numargs, argptr); 
#endif
}
/* ------------------------------------------------------------------------------------ */
static int gasnetc_ReplyGeneric(gasnetc_category_t category, 
                         gasnet_token_t token, gasnet_handler_t handler, 
                         void *source_addr, int nbytes, void *dest_ptr, 
                         int numargs, va_list argptr) {
#if GASNET_PSHM
  return gasneti_AMPSHM_ReplyGeneric(category, token, handler, source_addr, nbytes, 
                                     dest_ptr, numargs, argptr); 
#else
  int retval;
  gasnet_node_t sourceid = 0;
  #if GASNET_DEBUG  
    gasnetc_bufdesc_t *reqdesc = (gasnetc_bufdesc_t *)token;

    gasneti_assert(reqdesc->handlerRunning);
    gasneti_assert(!reqdesc->replyIssued);
    gasneti_assert(reqdesc->isReq);
    reqdesc->replyIssued = 1;
  #endif
  
  retval = gasnetc_ReqRepGeneric(category, 0, sourceid, handler, 
                                 source_addr, nbytes, dest_ptr, 
                                 numargs, argptr); 
  return retval;
#endif
}
/* ------------------------------------------------------------------------------------ */

extern int gasnetc_AMRequestShortM( 
                            gasnet_node_t dest,       /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTSHORT(dest,handler,numargs);
  va_start(argptr, numargs); /*  pass in last argument */

    /*  call the generic requestor */
    retval = gasnetc_RequestGeneric(gasnetc_Short, 
                                  dest, handler, 
                                  0, 0, 0,
                                  numargs, argptr);
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
  va_start(argptr, numargs); /*  pass in last argument */

    /*  call the generic requestor */
    retval = gasnetc_RequestGeneric(gasnetc_Medium, 
                                  dest, handler, 
                                  source_addr, nbytes, 0,
                                  numargs, argptr);
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
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /*  call the generic requestor */
  retval = gasnetc_RequestGeneric(gasnetc_Long, 
                                  dest, handler, 
                                  source_addr, nbytes, dest_addr,
                                  numargs, argptr);
#else

    /*  call the generic requestor */
    retval = gasnetc_RequestGeneric(gasnetc_Long, 
                                  dest, handler, 
                                  source_addr, nbytes, dest_addr,
                                  numargs, argptr);
#endif
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
  va_start(argptr, numargs); /*  pass in last argument */

#if GASNET_PSHM
  /*  call the generic requestor */
  retval = gasnetc_RequestGeneric(gasnetc_Long, 
                                  dest, handler, 
                                  source_addr, nbytes, dest_addr,
                                  numargs, argptr);
#else
    /*  call the generic requestor */
    retval = gasnetc_RequestGeneric(gasnetc_Long, 
                                  dest, handler, 
                                  source_addr, nbytes, dest_addr,
                                  numargs, argptr);
#endif
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

    /*  call the generic requestor */
    retval = gasnetc_ReplyGeneric(gasnetc_Short, 
                                  token, handler, 
                                  0, 0, 0,
                                  numargs, argptr);
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

    /*  call the generic requestor */
    retval = gasnetc_ReplyGeneric(gasnetc_Medium, 
                                  token, handler, 
                                  source_addr, nbytes, 0,
                                  numargs, argptr);
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
  /*  call the generic requestor */
  retval = gasnetc_ReplyGeneric(gasnetc_Long, 
                                token, handler, 
                                source_addr, nbytes, dest_addr,
                                numargs, argptr);
#else
    /*  call the generic requestor */
    retval = gasnetc_ReplyGeneric(gasnetc_Long, 
                                  token, handler, 
                                  source_addr, nbytes, dest_addr,
                                  numargs, argptr);
#endif
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
