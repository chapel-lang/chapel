/*   $Source: bitbucket.org:berkeleylab/gasnet.git/smp-conduit/gasnet_core.c $
 * Description: GASNet smp conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_am.h>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/select.h>

GASNETI_IDENT(gasnetc_IdentString_Version, "$GASNetCoreLibraryVersion: " GASNET_CORE_VERSION_STR " $");
GASNETI_IDENT(gasnetc_IdentString_Name,    "$GASNetCoreLibraryName: " GASNET_CORE_NAME_STR " $");

gex_AM_Entry_t const *gasnetc_get_handlertable(void);

gex_AM_Entry_t *gasnetc_handler; // TODO-EX: will be replaced with per-EP tables

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

/* ------------------------------------------------------------------------------------ */
/*
  PSHM Support Functions
  ======================
*/
#if GASNET_PSHM
#include <sys/types.h>
#include <sys/wait.h>

static int *gasnetc_fds = NULL;

#define GASNETC_DEFAULT_EXITTIMEOUT_MAX       5.
#define GASNETC_DEFAULT_EXITTIMEOUT_MIN       1.
#define GASNETC_DEFAULT_EXITTIMEOUT_FACTOR    0.1
static double gasnetc_exittimeout = GASNETC_DEFAULT_EXITTIMEOUT_MAX;

static struct gasnetc_exit_data {
  gasneti_atomic_t master;
  gasneti_atomic_t exitcode;
  volatile sig_atomic_t pid_tbl[1]; /* Variable length */
} *gasnetc_exit_data = NULL;
#define GASNETC_EXIT_DATA_SZ \
    gasneti_offsetof(struct gasnetc_exit_data, pid_tbl[gasneti_nodes])

#ifdef GASNETC_USE_SOCKETPAIR
  #include <sys/socket.h>
#endif
#ifdef HAVE_PR_SET_PDEATHSIG
  #include <sys/utsname.h>
  #include <sys/prctl.h>
  static int gasnetc_use_pdeathsig = 0;
#endif 

#ifndef GASNETC_REMOTEEXIT_SIGNAL
  #ifdef GASNETC_HAVE_O_ASYNC
    #define GASNETC_REMOTEEXIT_SIGNAL  SIGIO
  #elif defined(SIGURG)
    #define GASNETC_REMOTEEXIT_SIGNAL  SIGURG
  #else
    #define GASNETC_REMOTEEXIT_SIGNAL  SIGUSR1
  #endif
#endif

/* Retain a non-zero exit code (first one if possible) */
static void gasnetc_set_exitcode(int exitcode) {
  if (exitcode && gasnetc_exit_data) {
  #ifdef GASNETI_HAVE_ATOMIC_CAS
    /* Retain the first non-zero exit code */
    (void)gasneti_atomic_compare_and_swap(&gasnetc_exit_data->exitcode, 0,
                                          exitcode, GASNETI_ATOMIC_WMB_POST);
  #else
    /* Race is OK, since keeping first exit code is only desired, not required */
    if (!gasneti_atomic_read(&gasnetc_exit_data->exitcode, 0)) {
      gasneti_atomic_set(&gasnetc_exit_data->exitcode,
                         exitcode, GASNETI_ATOMIC_WMB_POST);
    }
  #endif
  }
}
static int gasnetc_get_exitcode(void) {
  /* assumes exit prior to allocation of gasnetc_exit_data is always an error */
  return gasnetc_exit_data ? gasneti_atomic_read(&gasnetc_exit_data->exitcode, 0) : -1;
}

static void gasnetc_exit_barrier_notify(int exitcode) {
  if (gasnetc_exit_data) {
    gasnetc_set_exitcode(exitcode);
    gasnetc_exit_data->pid_tbl[gasneti_mynode] = 0;
  }
}

static int gasnetc_exit_barrier_timed_wait(void) {
  if (gasnetc_exit_data) {
    int64_t timeout = 1e9 * gasnetc_exittimeout;
    gasneti_tick_t start_time = gasneti_ticks_now();
    gex_Rank_t i;

    gasneti_assert(timeout > 0);

    gasneti_local_rmb();
    for (i = 0; i < gasneti_nodes; i++) {
      int expire = 0;
      gasneti_waituntil(
        ( gasnetc_exit_data->pid_tbl[i] == 0 ) ||
        ( expire = (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) > timeout) ));
      if (expire) {
        /* Elect exactly one master */
        return gasneti_atomic_decrement_and_test(&gasnetc_exit_data->master, 0);
      }
    }
  }
  return 0;
}

/* TODO: use a process group (would require SIGT{STP,TIN,TOU} handling) */
static void gasnetc_signal_job(int sig) {
  if (gasnetc_exit_data) {
    gex_Rank_t i;
    for (i = 0; i < gasneti_nodes; i++) {
      pid_t pid = gasnetc_exit_data->pid_tbl[i];
      if (!pid || (i == gasneti_mynode)) continue;
      (void)kill(pid, sig);
      (void)kill(pid, SIGCONT);
    }
  }
}

extern void gasnetc_fatalsignal_cleanup_callback(int sig) {
  gasnetc_exit_barrier_notify(128 + sig);
  { // bug3624: pause to reduce the chance that concurrent crashes
    // across nodes might kill each other while backtraces are printing
    struct timeval tv; // use signal-safe sleep
    tv.tv_sec = 1; tv.tv_usec = 0;
    select(0, NULL, NULL, NULL, &tv);
  }
  gasnetc_signal_job(GASNETC_REMOTEEXIT_SIGNAL);
}

static void gasnetc_exit_sighand(int sig_recvd) {
  int sig_to_send = sig_recvd;
  int fatal = 0;

  switch (sig_recvd) {
    case SIGABRT: case SIGILL: case SIGSEGV: case SIGBUS: case SIGFPE:
      /* These signals indicates a bug in the exit handling code. */
      (void)gasneti_reghandler(sig_recvd, SIG_DFL); /* avoid recursion - do as early as possible */
      fprintf(stderr, "ERROR: exit code received fatal signal %d - Terminating\n", sig_recvd);
      sig_to_send = SIGKILL;
      fatal = 1;
      break;

    case SIGALRM: {
      /* This signal indicates a non-collective exit */
      static int count = 0;
      switch (count++) {
        case 0:  sig_to_send = GASNETC_REMOTEEXIT_SIGNAL; break;
        case 1:  sig_to_send = SIGTERM; break;
        default: sig_to_send = SIGKILL; break;
      }
      alarm((unsigned int)(1 + gasnetc_exittimeout));
      break;
    }
  }

  /* Signals are forwarded to the rest of the job */
  gasnetc_signal_job(sig_to_send);

  /* rearm */
  if (!fatal) {
    gasneti_reghandler(sig_recvd, gasnetc_exit_sighand);
  }
}

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
        gasneti_raise(SIGQUIT);
        /* Note: Both ISO C and POSIX assure us that raise() won't return until after the signal handler
         * (if any) has executed.  However, if that handler calls gasnetc_exit(), we'll never return here. */
      #elif 0
        kill(getpid(),SIGQUIT);
      #else
        handler(SIGQUIT);
      #endif
  }

  gasnetc_exit(0);
}

static int gasnetc_set_fl(int fd, unsigned int bits) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags >= 0) {
    flags |= bits;
    if (fcntl(fd, F_SETFL, flags) < 0)
      return -1;
  }
  return flags;
}

static int gasnetc_clr_fl(int fd, unsigned int bits) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags >= 0) {
    flags &= ~bits;
    if (fcntl(fd, F_SETFL, flags) < 0)
      return -1;
  }
  return flags;
}

#ifdef GASNETC_HAVE_O_ASYNC
static void gasnetc_arm_sigio(int fd) {
  if (gasnetc_set_fl(fd, O_ASYNC) > 0) {
    (void) fcntl(fd, F_SETOWN, getpid());
  }
}
static void gasnetc_disarm_sigio(int fd) {
  gasnetc_clr_fl(fd, O_ASYNC);
}
#endif


static void gasnetc_fork_children(void) {
  gex_Rank_t i;

  /* An initial pid table is kept in private memory */
  gasnetc_exit_data = gasneti_calloc(1, GASNETC_EXIT_DATA_SZ);
  gasnetc_exit_data->pid_tbl[0] = getpid();

  /* Space for pipes/sockets connecting node 0 to others */
  gasnetc_fds = gasneti_malloc(2 * gasneti_nodes * sizeof(int));
  gasneti_leak(gasnetc_fds);

  gasneti_assert(gasneti_mynode == 0);

  { /* set O_APPEND on stdout and stderr (same reasons as in bug 2136) */
    int rc;
    rc = gasnetc_set_fl(STDOUT_FILENO, O_APPEND);
    gasneti_assert( rc >= 0 );
    rc = gasnetc_set_fl(STDERR_FILENO, O_APPEND);
    gasneti_assert( rc >= 0 );
  }

  gasneti_reghandler(GASNETC_REMOTEEXIT_SIGNAL, gasnetc_remote_exit_sighand);

  for (i = 1; i < gasneti_nodes; i++) {
    int rc, fork_return;

    /* pipe or socket for intra-process bootstrap comms.
     * Sockets are used on systems where they can trigger a signal on disconnect.
     * Otherwise, we use pipes (which we assume are cheaper than PF_LOCAL sockets).
     * Note that we still try to arm pipes for SIGIO, but the behavior is less portable.
     */
#if defined(GASNETC_USE_SOCKETPAIR) && defined(GASNETC_HAVE_O_ASYNC)
  #if defined(PF_LOCAL)
    rc = socketpair(PF_LOCAL, SOCK_STREAM, 0, &gasnetc_fds[2 * i]);
  #elif defined(PF_UNIX)
    rc = socketpair(PF_UNIX, SOCK_STREAM, 0, &gasnetc_fds[2 * i]);
  #endif
#else
    rc = pipe(&gasnetc_fds[2 * i]);
#endif
    if (rc < 0) {
      gasneti_fatalerror("Failed to create control pipe/socket for process %i: (%d) %s",
                         i, errno, strerror(errno));
    }

    fork_return = fork();
    if (fork_return < 0) {
      gasnetc_signal_job(SIGTERM);
      gasneti_fatalerror("Failed to fork process %i: (%d) %s",
                         i, errno, strerror(errno));
    }
    if (fork_return) {
      /* I am parent */
      gasnetc_exit_data->pid_tbl[i] = fork_return;
      gasneti_assert_zeroret( close(gasnetc_fds[2 * i]) ); /* unused end of pipe/socket */
    } else {
      /* I am child */
      gasneti_mynode = i; 
      if (freopen("/dev/null", "r", stdin) != stdin) {
        gasneti_fatalerror("GASNet node %d failed to redirect STDIN", (int)i);
      }
      gasneti_free(gasnetc_exit_data);
      gasnetc_exit_data = NULL;
      #ifdef HAVE_PR_SET_PDEATHSIG
      if (gasnetc_use_pdeathsig) {
        /* Request generation of signal when parent exits */
        prctl(PR_SET_PDEATHSIG, GASNETC_REMOTEEXIT_SIGNAL);
      }
      #endif
      /* close unused end of pipes/sockets */
      for (i = 1; i <= gasneti_mynode; ++i) {
        gasneti_assert_zeroret( close(gasnetc_fds[2 * i + 1]) );
      }
      return;
    }
  }
}

static void gasnetc_join_children(void) {
  int children = gasneti_nodes - 1;

#if HAVE_SIGPROCMASK /* Is this ever NOT the case? */
  { /* In case we run nested in a SIGALRM-induced exit.
       We need this because alarm() may not do it for us. */
    sigset_t new_set, old_set;

    sigemptyset(&new_set);
    sigaddset(&new_set, SIGALRM);
    sigprocmask(SIG_UNBLOCK, &new_set, &old_set);
  }
#endif

  gasneti_reghandler(SIGALRM, gasnetc_exit_sighand);
  alarm((unsigned int)(1 + gasnetc_exittimeout));

  while (children) {
    int pid, status, rc;

    pid = wait(&status);
    if (pid < 0) {
      if (errno == EINTR) continue;  /* Probably the alarm */
      if (errno == ECHILD) break;    /* We lost count somehow. */

      gasnetc_signal_job(SIGTERM);
      gasneti_fatalerror("Unexpected wait() failure: %d(%s) - Terminating job", errno, strerror(errno));
    }
    --children;

    /* Job will return w/ first non-zero exit code observed */
    rc = -1; /* Unknown */
    if (WIFEXITED(status)) {
      rc = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
      rc = 128 + WTERMSIG(status); /* Consistent w/ most shells */
    }
    gasnetc_set_exitcode(rc);
  }

  alarm(0);
}

#if GASNET_PSHM
/* Broadcast usable prior to bring-up of PSHM
   This is used for the SNodeBcast fn in gasneti_pshm_init() */
static void gasnetc_bootstrapSNodeBroadcast(void *src, size_t len, void *dest, int root)
{
  ssize_t rc;
  int i;

  gasneti_assert(gasnetc_fds != NULL);
  gasneti_assert(root == 0);

  if (gasneti_mynode == 0) {
    for (i = 1; i < gasneti_nodes; ++i) {
      do {
        rc = write(gasnetc_fds[2 * i + 1], src, len);
        /* retry on interruption */
      } while ((rc == -1) && (errno == EINTR));
      gasneti_assert(rc == len);
    }
    memmove(dest, src, len);
  } else {
    do {
      rc = read(gasnetc_fds[2 * gasneti_mynode], dest, len);
      /* retry on interruption */
    } while ((rc == -1) && (errno == EINTR));
    if (!rc) {
      /* rc==0 occurs only if node0 exited prematurely */
      gasneti_fatalerror("Node0 died unexpectedly");
    }
    gasneti_assert(rc == len);
  }
}
#endif

static int gasnetc_get_pshm_nodecount(void)
{
  gex_Rank_t nodes = gasneti_getenv_int_withdefault("GASNET_PSHM_NODES", 0, 0);
  int politedefault;

  if (nodes > GASNETI_PSHM_MAX_NODES) { 
    gasneti_fatalerror("Nodes requested (%d) > maximum (%d)", (int)nodes,
                       GASNETI_PSHM_MAX_NODES);
  } else if (nodes == 0) {
    fprintf(stderr, "Warning: GASNET_PSHM_NODES not specified: running with 1 node\n");
    nodes = 1;
  }

  /* Set default to 'polite' synchronization if nodes > CPU's */
  politedefault = gasnett_cpu_count() > 0 && nodes > gasnett_cpu_count();
  gasnet_set_waitmode(politedefault ? GASNET_WAIT_BLOCK : GASNET_WAIT_SPIN);

  return nodes;
}

#endif  /* PSHM */
/* ------------------------------------------------------------------------------------ */

static int gasnetc_init(int *argc, char ***argv, gex_Flags_t flags) {
#if GASNET_PSHM
  int i;
#endif
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

  /* Must init timers after global env, and preferably before tracing */
  /* Note that we are intentionly doing this before we fork() */
  GASNETI_TICKS_INIT();

  /* add code here to bootstrap the nodes for your conduit */

  gasneti_mynode = 0;
  gasneti_nodes = 1;

#if GASNET_PSHM
  gasneti_nodes = gasnetc_get_pshm_nodecount();

  gasnetc_exittimeout =  gasneti_get_exittimeout(GASNETC_DEFAULT_EXITTIMEOUT_MAX,
                                                 GASNETC_DEFAULT_EXITTIMEOUT_MIN,
                                                 GASNETC_DEFAULT_EXITTIMEOUT_FACTOR,
                                                 GASNETC_DEFAULT_EXITTIMEOUT_MIN);

  #if defined(HAVE_PR_SET_PDEATHSIG) && !defined(GASNETC_USE_SOCKETPAIR)
  {
    /* check safety of prctl(PR_SET_PDEATHSIG, ...) */
    struct utsname name;
    if (0 == uname(&name)) {
      const char *dot = strchr(name.release,'.');
      if (NULL != dot) {
        int major = atoi(name.release);
        int minor = atoi(dot + 1);
        gasnetc_use_pdeathsig = ((100 * major + minor) >= 206); /* 2.6.0 kernel or newer */
      }
    }
  }
  #endif

  /* A fork in the road! */
  gasnetc_fork_children();
#endif

  /* enable tracing */
  gasneti_trace_init(argc, argv);

  /* Trivial all-zero nodemap */
  gasneti_nodemap = gasneti_calloc(gasneti_nodes, sizeof(gex_Rank_t));
  gasneti_nodemapParse();

  #if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_init(): spawn successful - node %i/%i starting...\n", 
      gasneti_mynode, gasneti_nodes); fflush(stderr);
  #endif

#if GASNET_PSHM
  #ifdef HAVE_PR_SET_PDEATHSIG
  if (gasnetc_use_pdeathsig){
    GASNETI_TRACE_PRINTF(C,("using PR_SET_PDEATHSIG for process control"));
  }
  #endif
  #ifdef GASNETC_USE_SOCKETPAIR
  GASNETI_TRACE_PRINTF(C,("using SIGIO for process control"));
  #endif

  {
    struct gasnetc_exit_data *tmp;

    tmp = gasneti_pshm_init(&gasnetc_bootstrapSNodeBroadcast, GASNETC_EXIT_DATA_SZ);
    if (!gasneti_mynode) {
      /* Relocate the pid table to shared space */
      memcpy(tmp, gasnetc_exit_data, GASNETC_EXIT_DATA_SZ);
      gasneti_free(gasnetc_exit_data);
      gasneti_atomic_set(&tmp->master, 1, 0);
      gasneti_atomic_set(&tmp->exitcode, 0, 0);
    }
    gasnetc_exit_data = tmp;
  }

  /* Done w/ bootstrap comms (move later if it becomes necessary) */
  if (0 == gasneti_mynode) {
    for (i = 1; i < gasneti_nodes; ++i) {
      const int fd = gasnetc_fds[2 * i + 1];
      #ifdef GASNETC_HAVE_O_ASYNC
        /* Arm for SIGIO when any child closes the socket/pipe */
        gasnetc_arm_sigio(fd);
      #else
        gasneti_assert_zeroret( close(fd) );
      #endif
    }
  } else {
    const int fd = gasnetc_fds[2 * gasneti_mynode];
    /* Arm for SIGIO when parent (node0) closes the socket/pipe */
    #ifdef GASNETC_HAVE_O_ASYNC
      gasnetc_arm_sigio(fd);
    #else
      gasneti_assert_zeroret( close(fd) );
    #endif
  }
#endif

  uintptr_t mmap_limit;
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

    mmap_limit = gasneti_segmentLimit((uintptr_t)-1, sharedLimit,
                                  &gasnetc_bootstrapExchange,
                                  &gasnetc_bootstrapBarrier);
  #else
    // TODO-EX: we can at least look at rlimits but such logic belongs in conduit-indep code
    mmap_limit = (intptr_t)-1;
  #endif

  /* allocate and attach an aux segment */

  (void) gasneti_auxsegAttach((uintptr_t)-1, &gasnetc_bootstrapExchange);

  /* determine Max{Local,GLobal}SegmentSize */
  gasneti_segmentInit(mmap_limit, &gasnetc_bootstrapExchange, flags);

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

  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
static int gasnetc_attach_primary(void) {
  /* ------------------------------------------------------------------------------------ */
  /*  register fatal signal handlers */

  /* catch fatal signals and convert to SIGQUIT */
  gasneti_registerSignalHandlers(gasneti_defaultSignalHandler);

  /*   register any custom signal handlers required by your conduit 
   *        (e.g. to support interrupt-based messaging)
   */

  // register process exit-time hook
  gasneti_registerExitHandler(gasnetc_exit);

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
  /* ------------------------------------------------------------------------------------ */
  /*  register client segment  */

  (void) gasneti_segmentAttach(segment_p, 0, tm, segsize, exchangefn, flags);

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
extern void gasnetc_exit(int exitcode) {
  /* once we start a shutdown, ignore all future SIGQUIT signals or we risk reentrancy */
  gasneti_reghandler(SIGQUIT, SIG_IGN);

#if GASNET_PSHM
  /* same goes for the remote exit signal */
  gasneti_reghandler(GASNETC_REMOTEEXIT_SIGNAL, SIG_IGN);
  #ifdef HAVE_PR_SET_PDEATHSIG
  if (gasneti_mynode && gasnetc_use_pdeathsig) {
    /* Disable generation of signal when parent exits */
    prctl(PR_SET_PDEATHSIG, 0);
  }
  #endif
  #ifdef GASNETC_HAVE_O_ASYNC
  {
    /* Disable generation of SIGIO when parent or children exits */
    if (0 == gasneti_mynode) {
      int i;
      for (i = 1; i < gasneti_nodes; ++i) {
        gasnetc_disarm_sigio(gasnetc_fds[2 * i + 1]);
      }
    } else {
      gasnetc_disarm_sigio(gasnetc_fds[2 * gasneti_mynode]);
    }
  }
  #endif

  gasnetc_exit_barrier_notify(exitcode);
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
#if GASNET_PSHM
  /* Attempt to coordinate the exit */
  gasneti_registerSignalHandlers(gasnetc_exit_sighand);
  if (gasnetc_exit_barrier_timed_wait()) {
    /* Invoke directly, not via SIGALRM */
    gasnetc_exit_sighand(SIGALRM);
  }

  if (gasneti_mynode == 0) {
    gasnetc_join_children();
  }
  exitcode = gasnetc_get_exitcode();
#endif

  gasneti_killmyprocess(exitcode);
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

#if GASNET_PSHM 
extern int gasnetc_AMPoll(GASNETI_THREAD_FARG_ALONE) {
  GASNETI_CHECKATTACH();
  return gasneti_AMPSHMPoll(0 GASNETI_THREAD_PASS);
}
#else
/* no polling required for smp-conduit */
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
*/

extern int gasnetc_AMRequestShortM( 
                            gex_TM_t tm,/* local context */
                            gex_Rank_t rank,       /* with tm, defines remote context */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            gex_Flags_t flags
                            GASNETI_THREAD_FARG,
                            int numargs, ...) {
  int retval;
  va_list argptr;
  gasneti_assert(!(flags & ~GEX_FLAG_IMMEDIATE)); // TODO-EX: only IMMEDIATE implemented
  GASNETI_COMMON_AMREQUESTSHORT(tm,rank,handler,flags,numargs);
  GASNETC_IMMEDIATE_MAYBE_POLL(flags); /* poll at least once, to assure forward progress */
  va_start(argptr, numargs); /*  pass in last argument */

    /*  call the generic requestor */
    gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
    retval = gasnetc_nbrhd_RequestGeneric(
                                  gasneti_Short,
                                  jobrank, handler, 
                                  0, 0, 0,
                                  flags, numargs, argptr
                                  GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
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
  int retval;
  va_list argptr;
  gasneti_assert(!(flags & ~GEX_FLAG_IMMEDIATE)); // TODO-EX: only IMMEDIATE implemented
  GASNETI_COMMON_AMREQUESTMEDIUM(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs);
  gasneti_leaf_finish(lc_opt); // always locally completed
  GASNETC_IMMEDIATE_MAYBE_POLL(flags); /* poll at least once, to assure forward progress */
  va_start(argptr, numargs); /*  pass in last argument */

    /*  call the generic requestor */
    gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
    retval = gasnetc_nbrhd_RequestGeneric(
                                  gasneti_Medium,
                                  jobrank, handler, 
                                  source_addr, nbytes, 0,
                                  flags, numargs, argptr
                                  GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
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
  int retval;
  va_list argptr;
  gasneti_assert(!(flags & ~GEX_FLAG_IMMEDIATE)); // TODO-EX: only IMMEDIATE implemented
  GASNETI_COMMON_AMREQUESTLONG(tm,rank,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs);
  gasneti_leaf_finish(lc_opt); // always locally completed
  GASNETC_IMMEDIATE_MAYBE_POLL(flags); /* poll at least once, to assure forward progress */
  va_start(argptr, numargs); /*  pass in last argument */

    /*  call the generic requestor */
    gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
    retval = gasnetc_nbrhd_RequestGeneric(
                                  gasneti_Long,
                                  jobrank, handler, 
                                  source_addr, nbytes, dest_addr,
                                  flags, numargs, argptr
                                  GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
}

extern int gasnetc_AMReplyShortM( 
                            gex_Token_t token,       /* token provided on handler entry */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            gex_Flags_t flags,
                            int numargs, ...) {
  int retval;
  va_list argptr;
  gasneti_assert(!(flags & ~GEX_FLAG_IMMEDIATE)); // TODO-EX: only IMMEDIATE implemented
  GASNETI_COMMON_AMREPLYSHORT(token,handler,flags,numargs);
  va_start(argptr, numargs); /*  pass in last argument */

    /*  call the generic requestor */
    retval = gasnetc_nbrhd_ReplyGeneric(
                                  gasneti_Short,
                                  token, handler, 
                                  0, 0, 0,
                                  flags, numargs, argptr);
  va_end(argptr);
  return retval;
}

extern int gasnetc_AMReplyMediumM( 
                            gex_Token_t token,     /* token provided on handler entry */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            void *source_addr, size_t nbytes,   /* data payload */
                            gex_Event_t *lc_opt,       /* local completion of payload */
                            gex_Flags_t flags,
                            int numargs, ...) {
  int retval;
  va_list argptr;
  gasneti_assert(!(flags & ~GEX_FLAG_IMMEDIATE)); // TODO-EX: only IMMEDIATE implemented
  GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,lc_opt,flags,numargs);
  gasneti_leaf_finish(lc_opt); // always locally completed
  va_start(argptr, numargs); /*  pass in last argument */

    /*  call the generic requestor */
    retval = gasnetc_nbrhd_ReplyGeneric(
                                  gasneti_Medium,
                                  token, handler, 
                                  source_addr, nbytes, 0,
                                  flags, numargs, argptr);
  va_end(argptr);
  return retval;
}

extern int gasnetc_AMReplyLongM( 
                            gex_Token_t token,     /* token provided on handler entry */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            gex_Event_t *lc_opt,       /* local completion of payload */
                            gex_Flags_t flags,
                            int numargs, ...) {
  int retval;
  va_list argptr;
  gasneti_assert(!(flags & ~GEX_FLAG_IMMEDIATE)); // TODO-EX: only IMMEDIATE implemented
  GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs);
  gasneti_leaf_finish(lc_opt); // always locally completed
  va_start(argptr, numargs); /*  pass in last argument */

    /*  call the generic requestor */
    retval = gasnetc_nbrhd_ReplyGeneric(
                                  gasneti_Long,
                                  token, handler, 
                                  source_addr, nbytes, dest_addr,
                                  flags, numargs, argptr);
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
