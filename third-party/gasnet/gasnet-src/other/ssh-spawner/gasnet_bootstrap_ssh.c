/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/ssh-spawner/gasnet_bootstrap_ssh.c $
 * Description: GASNet conduit-independent ssh-based spawner
 * Copyright 2005, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */
#include <gasnet_internal.h>
#include <gasnet_core_internal.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/select.h>
#if HAVE_NETINET_TCP_H
  #include <netinet/tcp.h>
#endif
#if HAVE_SYS_UIO_H
  #include <sys/uio.h>
#endif
#if HAVE_SYS_SOCKIO_H
  #include <sys/sockio.h>
#endif
#ifdef HAVE_PR_SET_PDEATHSIG
  /* Under Linux we can ask to be sent a given signal when our parent exits.
   * We use if mainly because of a bug in some versions of OpenSSH that
   * fail to kill spawned children when the ssh exits.  However, this will
   * also make us more resistant to runaways if somebody starts sending
   * SIGKILL to our processes (which leaves us no other way to cleanup
   * gracefully).  Not safe on some Linux kernels.
   */
  #include <sys/utsname.h>
  #include <sys/prctl.h>
  static int use_pdeathsig = 0;
#endif
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <limits.h>

#if GASNET_BLCR
  #include <libcr.h>
#endif

#ifndef GASNET_SOCKLEN_T
  #error "Don't know socklen_t or equivalent"
#endif

#ifndef PATH_MAX
  #define PATH_MAX 1024
#endif

/* NOTES

   This is a ssh-based (or rsh if you want) spawner for GASNet.  It is
   intended to be conduit-neutral.
  
   In the interest of scalability the ssh processes are started up in a
   balanced N-ary tree, where N can be controlled at run time (via env
   var GASNET_SSH_OUT_DEGREE).  Typically we want this value to be
   resonably large, since deep trees would result in multiple steps of
   forwarding for standard I/O (which is performed entirely by the ssh
   processes at this point).  IF GASNETI_SSH_OUT_DEGREE is set to zero
   then the tree effectively has inifinite out-degree and the tree of
   control processes (defined below) is only one level deep.

   The leaf processes are assigned gasnet ranks ("rank processes"), while
   internal nodes in the tree are known as "control proceses".  The root
   of the tree is always a control process (even when running a single
   rank) and is also known as the "master process".  Each control process
   may have both rank processes and additional control processes as
   children.  In normal use, there is a single control process per node.

   NOTE: currently "normal use" is defined to include
   listing each node at most once in GASNET_SSH_SERVERS (or equivalent),
   because currently we start one control per entry in the node list.
   It is planned to eliminate such duplication in the future.

   In addition to the tree of ssh connections, there is a control socket
   created between each process and its parent (this is true for both
   the rank and control processes).  This socket is used for control
   information, during startup.  For instance, the the environment and
   arguments are transferred over this socket.

   The control sockets are used to send each control process only a
   portion of the list of host names.  Rather than send the entire list,
   each processs receives the hostnames of any children it may have.

   The spawner is able to (in most cases) avoid orphaned processes
   by using TCP out-of-band data to generate a SIGURG.  The handler for
   this signal will send the out-of-band data both up and down the tree
   before exiting.

   If a child has the same hostname as its parent, it will be started
   directly, rather than via ssh IFF GASNETI_BOOTSTRAP_LOCAL_SPAWN is 1.
   By default this is enabled.

   The tree structure is used to provide scalable implementations of
   the following "service" routines for use during the bootstrap, as
   required in the template-conduit:
      void Barrier(void);
      void Exchange(void *src, size_t len, void *dest);
      void Broadcast(void *src, size_t len, void *dest, int rootnode);
      void SNodeBroadcast(void *src, size_t len, void *dest, int rootnode);
   
   Additionally, the following is useful (at least in ibv-conduit)
   for exchanging endpoint identifiers in a scalable manner:
      void Alltoall(void *src, size_t len, void *dest);

   If demand exists, scalable Scatter and Gather are possible.

   The following are needed to handle startup and termination:
      extern gasneti_spawnerfn_t const * gasneti_bootstrapInit_ssh(int *argc_p, char ***argv_p,
                                            gex_Rank_t *nodes_p,
                                            gex_Rank_t *mynode_p);
      void Fini(void);
      void Abort(int exitcode);

   In the case of normal termination, all nodes should call
   Fini() before they call exit().  In the event
   that gasnet is unable to arrange for an orderly shutdown, a call to
   Abort() will try to force all processes to exit
   with the given exit code.

   To control the spawner, there are a few environment variables, all of
   which are processed only by the master process (which send the
   relavent information on to the others via the control sockets).  See
   README for documentation on these variables.

   The following are (conditional on GASNET_BLCR) provided for support
   of BLCR-based checkpoint, restart and rollback:
      int PreCheckpoint(int fd);
      int PostCheckpoint(int fd, int is_restart);
      int Rollback(const char *dir);

   XXX: still to do
   + Group same-node when appears multiple times in list
   + Give master its own rank children too?
   + Implement "custom" spawner in the spirit of udp-conduit.
   + Look at udp-conduit for things missing from this list. :-)
   + Can write/writev loops use O_NONBLOCK to write portions to each
     socket w/o blocking?

 */

GASNETI_IDENT(gasnetc_IdentString_HaveSSHSpawner, "$GASNetSSHSpawner: 1 $");

static gasneti_spawnerfn_t const spawnerfn;

#ifndef GASNETI_BOOTSTRAP_LOCAL_SPAWN
  #define GASNETI_BOOTSTRAP_LOCAL_SPAWN 1
#endif

#define WHITESPACE " \t\n\r"
#define SSH_SERVERS_DELIM_CHARS  ",/;:" WHITESPACE

extern char **environ;
#ifndef ENV_PREFIX
  #define ENV_PREFIX "GASNET_"
#endif

enum {
  BOOTSTRAP_CMD_FINI0,
  BOOTSTRAP_CMD_FINI1,
  BOOTSTRAP_CMD_BARR0,
  BOOTSTRAP_CMD_BARR1,
  BOOTSTRAP_CMD_BCAST0,
  BOOTSTRAP_CMD_BCAST1,
  BOOTSTRAP_CMD_EXCHG0,
  BOOTSTRAP_CMD_EXCHG1,
  BOOTSTRAP_CMD_TRANS0,
  BOOTSTRAP_CMD_TRANS1,
  BOOTSTRAP_CMD_SNBCAST0,
  BOOTSTRAP_CMD_SNBCAST1,
#if GASNET_BLCR
  BOOTSTRAP_CMD_RSTRT_ARGS,
  BOOTSTRAP_CMD_ROLLBACK,
#endif
};

static const int c_one  = 1;
static const int c_zero = 0;

static int is_root = 0;
static int is_control = 0;
static int is_verbose = 0;
static char args_delim = ':';
static gex_Rank_t nranks = 0;
static char cwd[PATH_MAX];
static int listener = -1;
static int listen_port = -1;
static const char *argv0 = "[unknown]";
static int null_init = 0;
static char **nodelist;
static char **ssh_argv = NULL;
static int ssh_argc = 0;
static const char *wrapper = NULL;
static const char *envcmd = "env";
static char *master_env = NULL;
static size_t master_env_len = 0;
static struct child {
  int                 sock;
  pid_t               pid;
  gex_Rank_t       rank;
  gex_Rank_t       tree_ranks; /* ranks in this sub-tree, including self (1 for rank procs) */
  gex_Rank_t       tree_nodes; /* nodes in this sub-tree, including self (1 for rank procs) */
  char **             nodelist;
#if GASNET_BLCR
  cr_restart_handle_t rstrt_handle;
  char                rstrt_args;
  enum {
    RSTRT_STATE_RUNNING = 0,    /* Normal state */
    RSTRT_STATE_REQUESTED,      /* Restart or rollback request has been issued */
    /* Nothing else, yet. */
  }                   rstrt_state;
#endif
} *child = NULL;
static volatile int initialized = 0;
static int finalized = 0;
static gasneti_atomic_t live = gasneti_atomic_init(0);
static volatile int in_abort = 0;
static gex_Rank_t out_degree = GASNETI_DEFAULT_SSH_OUT_DEGREE;
static struct fds {
  fd_set set;
  int    max;
} child_fds, all_fds;
static int parent = -1; /* socket */
static gex_Rank_t myrank = 0;
static int myname = -1;
static int children = 0;
static int ctrl_children = 0;
static gex_Rank_t tree_ranks = GEX_RANK_INVALID;
static gex_Rank_t tree_nodes = GEX_RANK_INVALID;
static int mypid;
static volatile int exit_status = 0;
static gex_Rank_t nnodes = 0;	/* nodes, as distinct from ranks */
static int nnodes_set = 0;		/* non-zero if nnodes set explicitly */

#if GASNET_BLCR
/* BLCR-based checkpoint/restart */
  static int is_restart = 0;

  static int blcr_rollback(gex_Rank_t rank, const char *dir);
  static int blcr_restart(gex_Rank_t rank, const char *dir);
  static int blcr_reap(gex_Rank_t rank, int block);

  static int blcr_max_requests = 8; /* BLCR-TODO: env var to control this arbitrary limit */
  static int blcr_live_requests = 0;

  #define RSTRT_VERBOSE          (1<<0)
  #define RSTRT_CMD_RESTART      (1<<1)
  #define RSTRT_CMD_ROLLBACK     (1<<2)

  #ifdef CR_RSTRT_TARGET_AUTO
    #define HAVE_BLCR_RSTRT_TARGET 1
  #endif
#else
  #define is_restart 0
#endif

GASNETI_FORMAT_PRINTF(do_verbose,1,2,
static void do_verbose(const char *fmt, ...)) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  fflush(stderr);
  va_end(args);
}
#define BOOTSTRAP_VERBOSE(ARGS)		if_pf (is_verbose) do_verbose ARGS

GASNETI_FORMAT_PRINTF(sappendf,2,3,
static char *sappendf(char *s, const char *fmt, ...)) {
  va_list args;
  int old_len, add_len;

  /* compute length of thing to append */
  va_start(args, fmt);
  add_len = vsnprintf(NULL, 0, fmt, args);
  va_end(args);

  /* grow the string, including space for '\0': */
  if (s) {
    old_len = strlen(s);
    s = gasneti_realloc(s, old_len + add_len + 1);
  } else {
    old_len = 0;
    s = gasneti_malloc(add_len + 1);
  }

  /* append */
  va_start(args, fmt);
  vsprintf((s+old_len), fmt, args);
  va_end(args);

  return s;
}

/* Add single quotes around a string, taking care of any existing quotes */
static char *quote_arg(const char *arg) {
  char *p, *q, *tmp;
  char *result;

  result = gasneti_strdup("'");
  p = tmp = gasneti_strdup(arg);
  while ((q = strchr(p, '\'')) != NULL) {
    *q = '\0';
    result = sappendf(result, "%s'\\''", p);
    p = q + 1;
  }
  result = sappendf(result, "%s'", p);
  gasneti_free(tmp);
  return result;
}

/* Like gasneti_fatalerror, but w/o dumping core
 * This is used for probable user errors
 */
GASNETI_FORMAT_PRINTF(die,2,3, 
GASNETI_NORETURN
static void die(int exitcode, const char *msg, ...)) {
  va_list argptr;
  char expandedmsg[1024];

  strcpy(expandedmsg, "*** ERROR: ");
  strcat(expandedmsg, msg);
  strcat(expandedmsg, "\n");
  va_start(argptr, msg);
    vfprintf(stderr, expandedmsg, argptr);
    fflush(stderr);
  va_end(argptr);
  gasneti_killmyprocess(exitcode);
}


/* Fetch a variable from the environment on the master node.
 * (more or less copied from amudp_spmd.cpp)
 */
static char *do_getenv(const char *var) {
  if (master_env && var && (*var != '\0')) {
    char *p = master_env;
    size_t len = strlen(var);

    while (*p) {
      if (!strncmp(var, p, len) && (p[len] == '=')) {
        return p + len + 1;
      } else {
        p += strlen(p) + 1;
      }
    }
  }
  return NULL;
}

static void do_propagate_env(const char * keyname, int flags) {
  if (master_env) {
    gasneti_propagate_env_helper(master_env, keyname, flags);
  }
}

#if HAVE_SETPGID || HAVE_SETPGRP
  /* signals sent to entire process groups */
  #define pid_to_kill(pid) (-(pid))
  static int my_setpgid(pid_t pid)
  {
  #if HAVE_SETPGID
    return setpgid(pid, pid);
  #elif SETPGRP_VOID
    /* Note this case is a no-op in the parent */
    return pid ? 0 : setpgrp();
  #else
    return setpgrp(pid, pid);
  #endif
  }
#else
  /* signals sent to individual processes only */
  #define pid_to_kill(pid) (pid)
  #define my_setpgid(pid) (0)
#endif


/* returns count of signals sent */
static int signal_rank_procs(int signo)
{
  gex_Rank_t j;
  int count = 0;

  for (j = ctrl_children; j < children; ++j) { /* loop over rank processes only */
    pid_t pid = pid_to_kill(child[j].pid);
    if (! pid) continue;
    if (kill(pid, signo) < 0) {
      if ((kill(pid, 0) < 0) && (errno == ESRCH || errno == EPERM)) {
        /* Dead or recycled pid - not a failure */
        child[j].pid = 0;
      }
    } else {
      if (signo) BOOTSTRAP_VERBOSE(("[%d] kill(rank=%d, %d)\n", myname, child[j].rank, signo));
      ++count;
    }
  }
  return count;
}

static void kill_all_ranks(void)
{
  int done, loops = 30;

  gasneti_assert_always(is_control);

  gasneti_reghandler(SIGQUIT, SIG_DFL);
  gasneti_reghandler(SIGINT,  SIG_DFL);
  gasneti_reghandler(SIGTERM, SIG_DFL);
  gasneti_reghandler(SIGHUP,  SIG_DFL);
  gasneti_reghandler(SIGPIPE, SIG_DFL);

  /* First try to ensure that processes are not stopped */
  (void) signal_rank_procs(SIGCONT);

  /* First try the polite signal */
  done = !signal_rank_procs(SIGTERM);

  /* Loop until all rank processes are gone (kill fails) or time runs out */
  while (!done && (loops--)) {
    (void)sleep(1);
    done = !signal_rank_procs(0);
  }

  /* Be forcefull if still around */
  if (!done) signal_rank_procs(SIGKILL);
}

/* Send SIGURG up and down the tree
 * If bit 8 is set send ONLY toward leaves (signal forwarding)
 * If bit 8 is clear send BOTH directions (abnormal exit handling)
 */
static void do_oob(unsigned char byte) {
  const int oob_flags = MSG_OOB 
#ifdef MSG_DONTWAIT
	  		| MSG_DONTWAIT
#endif
#ifdef MSG_NOSIGNAL
			| MSG_NOSIGNAL
#endif
			;
  int j;

  gasneti_reghandler(SIGPIPE, SIG_IGN);
  if (0x80 & byte) gasneti_reghandler(SIGURG, SIG_IGN);

  for (j = 0; j < children; ++j) {
    if (j >= ctrl_children) continue;
    (void)send(child[j].sock, &byte, 1, oob_flags);
  }
  if (!(0x80 & byte) && !is_root) {
    (void)send(parent, &byte, 1, oob_flags);
  }
}

/* master forwards fatal signals if possible */
static void sigforward(int sig)
{
  gasneti_assert_always(is_control);

  {
    gasneti_sighandlerfn_t fp;
    switch (sig) {
      /* Termination signals are forwarded only once */
      case SIGQUIT: case SIGINT:
      case SIGTERM: case SIGHUP:
        gasneti_reghandler(sig, SIG_DFL);
        break;

      /* Other signals are forwarded every time they occur */
      default:
        gasneti_reghandler(sig, &sigforward);
        break;
    }
  }

  BOOTSTRAP_VERBOSE(("[%d] Forwarding signal %d\n", myname, sig));
  do_oob(sig | 0x80);
  signal_rank_procs(sig);
}

static void signal_forward(int enable) {
  gasneti_sighandlerfn_t fp = enable ? &sigforward : SIG_DFL;

  /* Termination signals */
  gasneti_reghandler(SIGQUIT, fp);
  gasneti_reghandler(SIGINT,  fp);
  gasneti_reghandler(SIGTERM, fp);
  gasneti_reghandler(SIGHUP,  fp);

  /* Some non-fatal signals (e.g. for freeze/unfreeze or backtrace) */
  /* TODO: parse environment for GASNET_FREEZE_SIGNAL and GASNET_BACKTRACE_SIGNAL? */
  #ifdef SIGCONT
    gasneti_reghandler(SIGCONT, fp);
  #endif
  #ifdef SIGUSR1
    gasneti_reghandler(SIGUSR1, fp);
  #endif
  #ifdef SIGUSR2
    gasneti_reghandler(SIGUSR2, fp);
  #endif
}


/* Cause everything to exit.
 * In a control process this returns to allow a full cleanup.
 * In worker processes there is no returning.
 */
static void do_abort(unsigned char exitcode) {
  /* avoid reentrance */
  if (in_abort) {
    return;
  } else {
    in_abort = 1;
  }

#if HAVE_PR_SET_PDEATHSIG
  /* disarm signal, if any, for termination of our parent */
  if (use_pdeathsig) {
    (void)prctl(PR_SET_PDEATHSIG, 0);
  }
#endif

  if (is_control) {
    signal_forward(0);
    do_oob(exitcode & 0x7f);
    kill_all_ranks();
  } else {
    gasneti_killmyprocess(exitcode);

    /* paranoia... */
    gasneti_reghandler(SIGABRT, SIG_DFL);
    gasneti_fatalerror("do_abort aborting...");

    /* NOT REACHED */
  }
}

static void reap_one(pid_t pid, int status)
{
  gasneti_assert_always(pid);

  gasneti_atomic_decrement(&live, 0);

  if (child) {
    int j;

    for (j = 0; j < children; ++j) {
      if (pid == child[j].pid) {
        const char *kind = (j < ctrl_children) ? "Ctrl" : "Rank";
        const char *fini = finalized ? "" : " before finalize";
        const int sock = child[j].sock;
        if (sock) (void)close(sock);
        child[j].pid = 0;
	if (WIFEXITED(status)) {
          int tmp = WEXITSTATUS(status);
	  if (exit_status == 0) exit_status = tmp;
	  BOOTSTRAP_VERBOSE(("[%d] %s proc %d exited with status %d%s\n",
				  myname, kind, child[j].rank, tmp, fini));
          if (!sock && (j < ctrl_children)) { // Ctrl proc which did not yet connect
            const char *host = child[j].nodelist ? child[j].nodelist[0] : nodelist[0];
            fprintf(stderr, "*** Failed to start processes on %s\n", host);
          }
	} else if (WIFSIGNALED(status)) {
          int tmp = WTERMSIG(status);
	  if (exit_status == 0) exit_status = tmp;
	  BOOTSTRAP_VERBOSE(("[%d] %s proc %d died with signal %d%s\n",
				  myname, kind, child[j].rank, tmp, fini));
	} else {
	  BOOTSTRAP_VERBOSE(("[%d] %s proc %d exited with unknown stats%s\n",
				  myname, kind, child[j].rank, fini));
	}
        if (!finalized) {
	  do_abort(exit_status ? exit_status : -1);
	}
	break;
      }
    }
    if (j == children) {
      BOOTSTRAP_VERBOSE(("[%d] Reaped unknown pid %d\n", myname, (int)pid));
    }
  }

  if (initialized != children) {
    gasneti_fatalerror("One or more processes died before setup was completed");
  }
}

static void reaper(int sig)
{
  gasneti_assert_always(!sig || sig == SIGCHLD);
  gasneti_reghandler(SIGCHLD, sig ? &reaper : SIG_DFL);

  {
    pid_t pid;
    int status;
    while((pid = waitpid(-1,&status,WNOHANG)) > 0) {
      reap_one(pid, status);
    }
  }
}

static void wait_for_all(void)
{
  sigset_t child_set;
  sigset_t old_set;

  sigemptyset(&child_set);
  sigaddset(&child_set, SIGCHLD);
  sigprocmask(SIG_BLOCK, &child_set, &old_set);

  /* Call reaper() to collect any children that may have exited before
   * we got here.
   * Also calls gasneti_reghandler(SIGCHLD, &reaper) for us.
   */
  reaper(SIGCHLD);

  while (gasneti_atomic_read(&live, 0)) {
    BOOTSTRAP_VERBOSE(("[%d] Sigsuspend with %d children left\n",
			    myname, gasneti_atomic_read(&live, 0)));
    sigsuspend(&old_set);
  }
}

static void sigurg_handler(int sig)
{
  unsigned char byte = 127;

  BOOTSTRAP_VERBOSE(("[%d] Received SIGURG\n", myname));

  if (is_control) {
    /* We need to read our single byte of OOB data here.
     * Use of select() can tell us who sent it to us.
     * MSG_OOB is supposed to always be non-blocking.i
     * If multiple sockets have OOB data we read them all.
     * If the SIGURG came fron kill() rather than OOB,
     * then we'll keep the value 127, leading to an abort.
     */

    struct timeval timeout = {0,0};
    fd_set fds = all_fds.set;
    int count = select(all_fds.max+1, NULL, NULL, &fds, &timeout);
    if (!count) {
      BOOTSTRAP_VERBOSE(("[%d] sigurg with NO exception fds\n", myname));
      do_abort(-1);
    }
    while (count) {
      int j, fd = -1;
      for (j = 0; j < ctrl_children; ++j) { /* Only check the control sockets */
        if (FD_ISSET(child[j].sock, &fds)) {
          fd = child[j].sock;
          break;
        }
      }
      if ((fd < 0) && (parent >= 0) && FD_ISSET(parent, &fds)) {
        fd = parent;
      }
      if (fd < 0) break; /* Unknown source! */
      (void)recv(fd, &byte, 1, MSG_OOB);
      FD_CLR(fd, &fds);
      --count;
    }
  }

  if (0x80 & byte) {
    /* signal forwarding */
    gasneti_reghandler(SIGURG, &sigurg_handler); /* rearm */
    signal_rank_procs(0x7f & byte);
  } else {
    /* abortive exit (or failure to locate any OOB data) */
    gasneti_reghandler(SIGURG, SIG_IGN); /* ignore */
    do_abort(byte);
  }
}

static void do_write(int fd, const void *buf, size_t len)
{
  const char *p = (const char *)buf;
  while (len) {
    ssize_t rc = write(fd, p, len);
    if_pf (!rc || ((rc < 0) && (errno != EINTR))) {
      fprintf(stderr, "Spawner: write() returned %d, errno = %d(%s)\n",
              (int)rc, errno, strerror(errno));
      do_abort(-1);
    }
    if_pf (in_abort) break;
    if_pf (rc < 0) continue; // EINTR
    p += rc;
    len -= rc;
  }
}

/* NOTE that unlike writev() we clobber the iovec */
static void do_writev(int fd, struct iovec *iov, int iovcnt)
{
#if defined(IOV_MAX)
  static int iov_max = IOV_MAX;
#elif defined(MAXIOV)
  static int iov_max = MAXIOV;
#else
  static int iov_max = 1024;
#endif

  while (iovcnt) {
    ssize_t rc;

    while (0 == iov->iov_len) {
      ++iov;
      if (0 == --iovcnt) return;
    }

    rc = writev(fd, iov, MIN(iovcnt, iov_max));
    if_pf ((rc < 0) && (errno == EINVAL) && (iov_max > 32)) {
      iov_max /= 2;
      continue;
    }
    if_pf (!rc || ((rc < 0) && (errno != EINTR))) {
      fprintf(stderr, "Spawner: writev() returned %d, errno = %d(%s)\n",
              (int)rc, errno, strerror(errno));
      do_abort(-1);
    }
    if_pf (in_abort) break;
    if_pf (rc < 0) continue; // EINTR

    do {
      size_t len = iov->iov_len;
      if (rc >= len) {
      #if GASNET_DEBUG
        iov->iov_base = NULL; /* detect reuse, which is forbidden */
      #endif
        ++iov;
        --iovcnt;
        rc -= len;
      } else {
        iov->iov_base = (void*)((uintptr_t)iov->iov_base + rc);
        iov->iov_len -= rc;
        break;
      }
    } while (rc);
  }
}

static void do_write_string(int fd, const char *string) {
  size_t len = string ? strlen(string) : 0;
  struct iovec iov[2];
  iov[0].iov_base = (void *)&len;
  iov[0].iov_len  = sizeof(len);
  iov[1].iov_base = (void *)string;
  iov[1].iov_len  = len;
  do_writev(fd, iov, 2);
}

static void do_read(int fd, void *buf, size_t len)
{
  char *p = (char *)buf;
  while (len) {
    ssize_t rc = read(fd, p, len);
    if_pf (!rc) {
      fprintf(stderr, "Spawner: read() returned 0 (EOF)\n");
      do_abort(-1);
    } else if_pf ((rc < 0) && (errno != EINTR)) {
      fprintf(stderr, "Spawner: read() returned %d, errno = %d(%s)\n",
              (int)rc, errno, strerror(errno));
      do_abort(-1);
    }
    if_pf (in_abort) break;
    if_pf (rc < 0) continue; // EINTR
    p += rc;
    len -= rc;
  }
}

/* NOTE that unlike readv() we clobber the iovec */
static void do_readv(int fd, struct iovec *iov, int iovcnt)
{
#if defined(IOV_MAX)
  static int iov_max = IOV_MAX;
#elif defined(MAXIOV)
  static int iov_max = MAXIOV;
#else
  static int iov_max = 1024;
#endif
{ size_t len = 0;
  int c;
  for(c=0; c<iovcnt;++c) len += iov[c].iov_len;
}

  while (iovcnt) {
    ssize_t rc;

    while (0 == iov->iov_len) {
      ++iov;
      if (0 == --iovcnt) return;
    }

    rc = readv(fd, iov, MIN(iovcnt, iov_max));
    if_pf ((rc < 0) && (errno == EINVAL) && (iov_max > 32)) {
      iov_max /= 2;
      continue;
    }
    if_pf (!rc) {
      fprintf(stderr, "Spawner: readv() returned 0 (EOF)\n");
      do_abort(-1);
    } else if_pf ((rc < 0) && (errno != EINTR)) {
      fprintf(stderr, "Spawner: readv() returned %d, errno = %d(%s)\n",
              (int)rc, errno, strerror(errno));
      do_abort(-1);
    }
    if_pf (in_abort) break;
    if_pf (rc < 0) continue; // EINTR

    do {
      size_t len = iov->iov_len;
      if (rc >= len) {
      #if GASNET_DEBUG
        iov->iov_base = NULL; /* detect reuse, which is forbidden */
      #endif
        ++iov;
        --iovcnt;
        rc -= len;
      } else {
        iov->iov_base = (void*)((uintptr_t)iov->iov_base + rc);
        iov->iov_len -= rc;
        break;
      }
    } while (rc);
  }
}

static char *do_read_string(int fd) {
  char *result = NULL;
  size_t len;

  do_read(fd, &len, sizeof(size_t));
  if (len) {
    result = gasneti_malloc(len + 1);
    do_read(fd, result, len);
    result[len] = '\0';
  }

  return result;
}

static int my_socketpair(int sv[2]) {
  #if defined(PF_LOCAL)
    const int domain = PF_LOCAL;
  #elif defined(PF_UNIX)
    const int domain = PF_UNIX;
  #endif
    int rc = socketpair(domain, SOCK_STREAM, 0, sv);
  #if GASNET_BLCR
    if (! rc) {
      /* POSIX does not require that (sv[0] < sv[1]).
       * However, restart relies on it so that every rank process has the same "parent" fd.
       */
      if (sv[0] > sv[1]) { int tmp=sv[0]; sv[0]=sv[1]; sv[1]=tmp; } /* SWAP */
    #if GASNET_DEBUG
      { static int sv0 = -1;
        if (sv0 < 0) sv0 =  sv[0];
        else gasneti_assert_always(sv0 == sv[0]);
      }
      #endif
    }
  #endif
    return rc;
}

static int options_helper(char **list, const char *string, const char *where)
{
  int count = 0;
  int in_quotes = 0;
  const char *special[] = {WHITESPACE "\\\"'",	/* special chars outside dbl quotes */
			   "\\\""};		/* special chars inside dbl quotes */
  
  if (!string) {
    return 0;
  }

  /* Outer loop adds a word to the list on each pass
     with the possible exception of the last
   */
  while (*string) {
    char tmp[1024];
    char *p = tmp;
    while (*string && strchr(WHITESPACE,*string)) ++string; /* eat leading whitespace */
    if (!*string) { break; /* reached end of string */ }
    /* This loop brings together pieces of a "word", possible w/ quotes */
    in_quotes = 0;
    do {
      int i = strcspn(string, special[in_quotes]);
      GASNETI_MEMCPY(p , string, i); p += i;
      gasneti_assert_always((uintptr_t)(p-tmp) < (sizeof(tmp)-1));
      string += i;
      switch (*string) {
	case '\0':
	  break;
	case '\\':
	  if (!string[1]) {
	    die(1, "string ends with \\ %s", where);
	  } else if (strchr(special[in_quotes],string[1])) {
	    /* Drop the backslash if it quotes a special character */
            *(p++) = string[1];
            gasneti_assert_always((uintptr_t)(p-tmp) < (sizeof(tmp)-1));
	  } else {
	    /* Keep the backslash */
	    GASNETI_MEMCPY(p , string, 2); p += 2;
            gasneti_assert_always((uintptr_t)(p-tmp) < (sizeof(tmp)-1));
	  }
	  string += 2;
	  break;
	case '\'':
	  ++string;
	  i = strcspn(string, "\'");
	  if (string[i] != '\'') {
	    die(1, "unbalanced ' %s", where);
	  }
          GASNETI_MEMCPY(p , string, i); p += i;
          gasneti_assert_always((uintptr_t)(p-tmp) < (sizeof(tmp)-1));
	  string += i + 1;
	  break;
	case '"':
	  ++string;
	  in_quotes = !in_quotes;
	  break;
	default: /* WHITESPACE */
	  break;
      }
    } while (*string && (in_quotes || !strchr(WHITESPACE,*string)));
    if (in_quotes) {
      die(1, "unbalanced \" %s", where);
    }
    if (list) {
      gasneti_assert_always((uintptr_t)(p-tmp) < sizeof(tmp));
      *p = '\0';
      list[count] = gasneti_strdup(tmp);
    }
    ++count;
  }
  if (list) {
    list[count] = NULL;
  }
  return count;
}

/* Parse a string into an array of "words", following shell rules for '," and \ */
static char **parse_options(const char *string, int *count_p, const char *where)
{
  int count;
  char **list;

  /* First parse pass will just count the words */
  count = options_helper(NULL, string, where);
  list = gasneti_malloc(sizeof(char *) * (count+1));

  /* Second pass fills the list of words */
  (void)options_helper(list, string, where);

  if (count_p) *count_p = count;
  return list;
}

/* wrappers that map unset/empty to NULL */
static const char *my_getenv_withdefault(const char *key, const char *defval) {
  char *env_string = gasneti_getenv_withdefault(key, defval);
  return ((env_string != NULL) && strlen(env_string)) ? env_string : NULL;
}
static const char *my_getenv(const char *key) {
  return my_getenv_withdefault(key, NULL);
}

static void configure_ssh(void) {
  const char *env_string;
  const char *ssh_argv0;
  char **ssh_options = NULL;
  int is_openssh = 0;
  int optcount = 0;
  int i, argi;

  envcmd = my_getenv_withdefault(ENV_PREFIX "ENVCMD", "env");

  /* Determine the ssh command */
  ssh_argv0 = my_getenv_withdefault(ENV_PREFIX "SSH_CMD", GASNETI_DEFAULT_SSH_CMD);
  if (ssh_argv0 == NULL) {
      BOOTSTRAP_VERBOSE(("Ignoring empty value in environment variable " ENV_PREFIX "SSH_CMD\n"));
      ssh_argv0 = GASNETI_DEFAULT_SSH_CMD;
  }

  /* Check for OpenSSH */
  {
    char *cmd = sappendf(NULL, "%s -V 2>&1 | grep OpenSSH >/dev/null 2>/dev/null", ssh_argv0);
    is_openssh = (0 == system(cmd));
    gasneti_free(cmd);
    BOOTSTRAP_VERBOSE(("Configuring for OpenSSH\n"));
  }

  /* Check for user-supplied options */
  if ((env_string = my_getenv_withdefault(ENV_PREFIX "SSH_OPTIONS", GASNETI_DEFAULT_SSH_OPTIONS)) != NULL) {
    ssh_options = parse_options(env_string, &optcount, "while parsing " ENV_PREFIX "SSH_OPTIONS");
  }

  /* Now build the command line */
  ssh_argc = optcount + (is_openssh ? 10 : 1);
  ssh_argv = gasneti_calloc((ssh_argc + 3 /* host + cmd + NULL = 3 */), sizeof(char *));
  ssh_argv[0] = gasneti_strdup(ssh_argv0);
  argi = 1;
  if (is_openssh) {
    ssh_argv[argi++] = (char *)"-o"; ssh_argv[argi++] = (char *)"StrictHostKeyChecking no";
    ssh_argv[argi++] = (char *)"-o"; ssh_argv[argi++] = (char *)"FallBackToRsh no";
    ssh_argv[argi++] = (char *)"-o"; ssh_argv[argi++] = (char *)"BatchMode yes";
    ssh_argv[argi++] = (char *)"-o"; ssh_argv[argi++] = (char *)"ForwardX11 no";
    ssh_argv[argi++] = (char *)"-q";
  }
  if (optcount) {
    for (i=0; i<optcount; ++i, ++argi) {
      ssh_argv[argi] = ssh_options[i];
    }
    gasneti_free(ssh_options);
  }

  BOOTSTRAP_VERBOSE(("Constructed ssh command line:\n"));
  for (i=0; i<ssh_argc; ++i) {
    BOOTSTRAP_VERBOSE(("\t%s\n", ssh_argv[i]));
  }
  BOOTSTRAP_VERBOSE(("\tHOST\n\tCMD\n"));
}

/* Reduce nnodes when presented with a short nodelist */
static char ** short_nodelist(char **nodelist, gex_Rank_t count) {
  if (nnodes_set) {
    fprintf(stderr, "WARNING: Request for %d nodes ignored because only %d nodes are available.\n", nnodes, count);
    fflush(stderr);
  }

  nnodes = count;
  nodelist = gasneti_realloc(nodelist, nnodes * sizeof(char *));
  BOOTSTRAP_VERBOSE(("Node count set to available: %d\n", (int)nnodes));

  return nodelist;
}

/* Build an array of hostnames from a stdio stream */
static char ** parse_nodestream(FILE *fp) {
  char **result = NULL;
  gex_Rank_t i;

  result = gasneti_malloc(nnodes * sizeof(char *));
  for (i = 0; i < nnodes;) {
    static char buf[1024];
    char *p;

    if (!fgets(buf, sizeof(buf), fp)) {
      /* ran out of lines */
      result = short_nodelist(result, i);
      break;
    }
 
    p = buf;
    while (*p && strchr(WHITESPACE, *p)) ++p; /* eat leading whitespace */
    if (*p != '#') {
      p[strcspn(p, WHITESPACE)] = '\0';
      result[i] = gasneti_strdup(p);

      /* Discard consecutive duplicates unless nnodes was given explicitly.
       * When running w/ PBS and nodes=2:ppn=2 we see NODE0,NODE0,NODE1,NODE1
       * while running the same config w/ LSF gives just NODE0,NODE1.
       * We get consistent results by squeezing out the PBS style dups.
       * TODO: O(N^2) work to remove ALL duplicates?
       */
      if (!nnodes_set && (i > 0) && !strcmp(result[i], result[i-1])) {
        gasneti_free(result[i]);
	continue;
      }
      ++i;
      BOOTSTRAP_VERBOSE(("\t%s\n", p));
    }
  }

  return result;
}

/* Build an array of hostnames from a file */
static char ** parse_nodefile(const char *filename) {
  char **result = NULL;
  FILE *fp;

  BOOTSTRAP_VERBOSE(("Parsing nodefile '%s'\n", filename));
  fp = fopen(filename, "r");
  if (!fp) {
    die(1, "failed to open nodefile '%s'", filename);
  }

  result = parse_nodestream(fp);

  (void)fclose(fp);

  return result;
}

/* Build an array of hostnames from a pipe */
static char ** parse_nodepipe(const char *cmd) {
  char **result = NULL;
  FILE *fp;

  BOOTSTRAP_VERBOSE(("Parsing nodes from command '%s'\n", cmd));
  fp = popen(cmd, "r");
  if (!fp) {
    die(1, "failed to spawn command '%s'", cmd);
  }

  result = parse_nodestream(fp);

  (void)pclose(fp);

  return result;
}

/* Build an array of hostnames from a delimited string */
static char ** parse_servers(const char *list) {
  static const char *delims = SSH_SERVERS_DELIM_CHARS;
  char **result = NULL;
  char *string, *alloc;
  gex_Rank_t i;

  alloc = string = gasneti_strdup(list);
  result = gasneti_malloc(nnodes * sizeof(char *));
  BOOTSTRAP_VERBOSE(("Parsing servers list '%s'\n", string));
  for (i = 0; i < nnodes; ++i) {
    char *p;
    while (*string && strchr(delims,*string)) ++string; /* eat leading delimiters */
    if (!*string) {
      /* ran out of words */
      result = short_nodelist(result, i);
      break;
    }
    p = string;
    string += strcspn(string, delims);
    if (*string) *(string++) = '\0';
    result[i] = gasneti_strdup(p);
    BOOTSTRAP_VERBOSE(("\t%s\n", result[i]));
  }
  gasneti_free(alloc);

  return result;
}

static void build_nodelist(void)
{
  const char *env_string;

  if ((env_string = my_getenv_withdefault(ENV_PREFIX "SSH_NODEFILE",
                                          GASNETI_DEFAULT_SSH_NODEFILE)) != NULL) {
    nodelist = parse_nodefile(env_string);
  } else if ((env_string = my_getenv(ENV_PREFIX "SSH_SERVERS")) != NULL) {
    nodelist = parse_servers(env_string);
  } else if ((env_string = my_getenv(ENV_PREFIX "NODEFILE")) != NULL) {
    nodelist = parse_nodefile(env_string);
  } else if ((env_string = my_getenv("PBS_NODEFILE")) != NULL) {
    nodelist = parse_nodefile(env_string);
  } else if ((env_string = my_getenv("PE_HOSTFILE")) != NULL) {
    nodelist = parse_nodefile(env_string);
  } else if ((env_string = my_getenv("SSS_HOSTLIST")) != NULL) {
    nodelist = parse_servers(env_string);
  } else if ((env_string = my_getenv("LSB_HOSTS")) != NULL) {
    nodelist = parse_servers(env_string);
  } else if (my_getenv("SLURM_JOB_ID") != NULL) {
    nodelist = parse_nodepipe("scontrol show hostname");
  } else {
    die(1, "No " ENV_PREFIX "SSH_NODEFILE, " ENV_PREFIX "SSH_SERVERS, or " ENV_PREFIX "NODEFILE in environment");
  }
}

static void send_nodelist(int s, int count, char ** list) {
  gex_Rank_t i;

  /* length of list is already known to the recipient */
  count = MAX(count, 1);
  for (i = 0; i < count; ++i) {
    do_write_string(s, list[i]);
  }
}

static void recv_nodelist(int s, int count) {
  gex_Rank_t i;
  count = MAX(count, 1);

  nodelist = gasneti_malloc(count * sizeof(char *));
  gasneti_leak(nodelist);
  for (i = 0; i < count; ++i) {
    nodelist[i] = do_read_string(s);
    gasneti_leak(nodelist[i]);
  }
}

static void send_identity(int s, struct child *ch) {
  struct iovec iov[4];
  iov[0].iov_base = (void*) &nranks;
  iov[0].iov_len  = sizeof(gex_Rank_t);
  iov[1].iov_base = (void*) &ch->rank;
  iov[1].iov_len  = sizeof(gex_Rank_t);
  iov[2].iov_base = (void*) &ch->tree_ranks;
  iov[2].iov_len  = sizeof(gex_Rank_t);
  iov[3].iov_base = (void*) &ch->tree_nodes;
  iov[3].iov_len  = sizeof(gex_Rank_t);
  do_writev(s, iov, 4);
}

static void recv_identity(int s) {
  struct iovec iov[4];
  iov[0].iov_base = (void*) &nranks;
  iov[0].iov_len  = sizeof(gex_Rank_t);
  iov[1].iov_base = (void*) &myrank;
  iov[1].iov_len  = sizeof(gex_Rank_t);
  iov[2].iov_base = (void*) &tree_ranks;
  iov[2].iov_len  = sizeof(gex_Rank_t);
  iov[3].iov_base = (void*) &tree_nodes;
  iov[3].iov_len  = sizeof(gex_Rank_t);
  do_readv(s, iov, 4);
  gasneti_assert_always(nranks > 0);
  gasneti_assert_always(myrank < nranks);
}

/*
 * Send environment as a big char[] with \0 between each 'VAR=VAL'
 * and a double \0 to terminate. (inspired by amudp)
 */
static void send_env(int s) {
  if (!master_env) {
    int i;
    const char *p;
    char *q;
    size_t rlen = strlen(ENV_PREFIX "SSH_");

    gasneti_assert_always(is_control);

    /* First pass over environment to get its size */
    master_env_len = 1; /* for the doubled \0 at the end */
    for (i = 0, p = environ[0]; p != NULL; p = environ[++i]) {
      if (!strncmp(ENV_PREFIX "SSH_", p, rlen)) {
        /* We parse these ourselves, don't forward */
      } else {
        master_env_len += strlen(p) + 1;
      }
    }

    /* Append all the strings together */
    q = master_env = gasneti_malloc(master_env_len);
    for (i = 0, p = environ[0]; p != NULL; p = environ[++i]) {
      if (!strncmp(ENV_PREFIX "SSH_", p, rlen)) {
        /* We parse these ourselves, don't forward */
      } else {
        size_t tmp = strlen(p) + 1;
        GASNETI_MEMCPY(q, p, tmp);
        q += tmp;
      }
    }
    *q = '\0';
  }

  /* send it */
  do_write(s, &master_env_len, sizeof(master_env_len));
  do_write(s, master_env, master_env_len);
}

static void recv_env(int s) {
  do_read(s, &master_env_len, sizeof(master_env_len));
  master_env = gasneti_malloc(master_env_len);
  gasneti_leak(master_env);
  do_read(s, master_env, master_env_len);
}

static void send_ssh_argv(int s) {
  int i;

  do_write(s, &ssh_argc, sizeof(int));
  for (i = 0; i < ssh_argc; ++i) {
    do_write_string(s, ssh_argv[i]);
  }
}

static void recv_ssh_argv(int s) {
  int i;

  do_read(s, &ssh_argc, sizeof(int));
  ssh_argv = gasneti_calloc(ssh_argc+3, sizeof(char *));
  gasneti_leak(ssh_argv);
  for (i = 0; i < ssh_argc; ++i) {
    ssh_argv[i] = do_read_string(s);
    gasneti_leak(ssh_argv[i]);
  }
}

static void send_argv(int s, int argc, char * const *argv) {
  int i;

  do_write(s, &argc, sizeof(int));
  for (i = 0; i < argc; ++i) {
    do_write_string(s, argv[i]);
  }
}

static void recv_argv(int s, int *argc_p, char ***argv_p) {
  int argc, i;
  char **argv;

  do_read(s, &argc, sizeof(int));
  argv = gasneti_malloc((argc+1) * sizeof(char*));
  gasneti_leak(argv);
  for (i = 0; i < argc; ++i) {
    argv[i] = do_read_string(s);
    gasneti_leak(argv[i]);
  }
  argv[argc] = NULL;

  *argc_p = argc;
  *argv_p = argv;
  argv0 = argv[0];
}

static int fcntl_setfl(int fd, int flag) {
  int rc;
  rc = fcntl(fd, F_GETFL, 0);
  if (rc >= 0) rc = fcntl(fd, F_SETFL, rc | flag);
  return rc;
}

#if 0 /* Unused */
static int fcntl_clrfl(int fd, int flag) {
  int rc;
  rc = fcntl(fd, F_GETFL, 0);
  if (rc >= 0) rc = fcntl(fd, F_SETFL, rc & ~flag);
  return rc;
}
#endif

static int fcntl_setfd(int fd, int flag) {
  int rc;
  rc = fcntl(fd, F_GETFD, 0);
  if (rc >= 0) rc = fcntl(fd, F_SETFD, rc | flag);
  return rc;
}

static int fcntl_clrfd(int fd, int flag) {
  int rc;
  rc = fcntl(fd, F_GETFD, 0);
  if (rc >= 0) rc = fcntl(fd, F_SETFD, rc & ~flag);
  return rc;
}

static void fd_sets_init(void) {
  FD_ZERO(&child_fds.set);
  child_fds.max = -1;
  FD_ZERO(&all_fds.set);
  all_fds.max = -1;
}

static void fd_sets_add(int fd) {
  FD_SET(fd, &all_fds.set);
  all_fds.max = MAX(all_fds.max, fd);
  if (fd != parent) {
    FD_SET(fd, &child_fds.set);
    child_fds.max = MAX(child_fds.max, fd);
  }
}


static void pre_spawn(int count) {
  struct sockaddr_in sock_addr;
  GASNET_SOCKLEN_T addr_len;
  const char *env_string;

  /* Get the cwd */
  if ((env_string = my_getenv(ENV_PREFIX "SSH_REMOTE_PATH")) != NULL) {
    strncpy(cwd, env_string, sizeof(cwd)-1);
    cwd[sizeof(cwd) - 1] = '\0';
  } else if (!getcwd(cwd, sizeof(cwd))) {
    gasneti_fatalerror("getcwd() failed");
  }

  /* Create listening socket */
  if ((listener = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    gasneti_fatalerror("listener = socket() failed");
  }
  (void)fcntl_setfd(listener, FD_CLOEXEC);
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_port = 0;
  sock_addr.sin_addr.s_addr = INADDR_ANY;
  addr_len = sizeof(sock_addr);
  if (bind(listener, (struct sockaddr *)&sock_addr, addr_len) < 0) {
    gasneti_fatalerror("bind() failed");
  }
  if (listen(listener, count) < 0) {
    gasneti_fatalerror("listen() failed w/ errno=%d", errno);
  }
  if (getsockname(listener, (struct sockaddr *)&sock_addr, &addr_len) < 0) {
    gasneti_fatalerror("getsockname() failed");
  }
  listen_port = ntohs(sock_addr.sin_port);
}

static void post_spawn(int count, int argc, char * const *argv) {
  /* Accept count connections */
  while (count--) {
    struct sockaddr_in sock_addr;
    GASNET_SOCKLEN_T addr_len = sizeof(sock_addr);
    gex_Rank_t child_id;
    struct child *ch = NULL;
    int s;

    reaper(SIGCHLD); /* Take notice of children dying while blocked in accept() */
    if ((s = accept(listener, (struct sockaddr *)&sock_addr, &addr_len)) < 0) {
      gasneti_fatalerror("accept() failed w/ errno=%d", errno);
    }
    reaper(0); /* Disarm signal handler */

    fd_sets_add(s);
    gasneti_assert_always(mypid == getpid());
    (void)ioctl(s, SIOCSPGRP, &mypid); /* Enable SIGURG delivery on OOB data */
    (void)fcntl_setfd(s, FD_CLOEXEC);

#ifdef TCP_CORK
    (void)setsockopt(s, IPPROTO_TCP, TCP_CORK, (char *) &c_one, sizeof(c_one));
#endif
    do_read(s, &child_id, sizeof(gex_Rank_t));
    gasneti_assert_always(child_id < children);
    ch = &(child[child_id]);
    gasneti_assert_always(ch->rank < nranks);
    ch->sock = s;

    send_identity(s, ch);
    send_argv(s, argc, argv);
    send_env(s);
    if (ch->tree_nodes > 1) {
      do_write(s, &out_degree, sizeof(gex_Rank_t));
      send_nodelist(s, ch->tree_nodes - 1, ch->nodelist + 1);
      send_ssh_argv(s);
      do_write_string(s, wrapper);
    }

#ifdef TCP_CORK
    (void)setsockopt(s, IPPROTO_TCP, TCP_CORK, (char *) &c_zero, sizeof(c_zero));
#endif
#ifdef TCP_NODELAY
    (void)setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (char *) &c_one, sizeof(c_one));
#endif

    initialized++;
  }

  /* Close listener */
  close(listener);
}

static void do_connect(const char *spawn_args, int *argc_p, char ***argv_p)
{
  gex_Rank_t child_id;
  const char *parent_name;
  int parent_port;

  /* Extract child_id, parent_port and parent_name from spawn args
   * Format of spawn_args: "id:port:host"
   *  :    indicates the delimiter character (args_delim)
   *  id   is the non-negative integer child_id (rank among my parent's children)
   *  port is the positive integer TCP port number
   *  host is the parent's hostname, address or 'localhost' (everything after the last delimiter)
   */
  {
    const char *p = spawn_args;
    char *endptr;
    long ltmp;

    gasneti_assert_always(p && *p);

    /* Required non-negative child id */
    child_id = ltmp = strtol(p,&endptr,0);
    if ((endptr == p) || (ltmp < 0) || ((long)child_id != ltmp)) {
      die(1, "Unable to parse child id in " ENV_PREFIX "SPAWN_ARGS");
    }
    if (*endptr != args_delim) {
      die(1, "Unable to parse " ENV_PREFIX "SPAWN_ARGS");
    }
    p = endptr + 1;

    /* Required non-negative port */
    parent_port = ltmp = strtol(p,&endptr,0);
    if ((endptr == p) || (ltmp < 0) || ((long)parent_port != ltmp)) {
      die(1, "Unable to parse port number in " ENV_PREFIX "SPAWN_ARGS");
    }
    if (*endptr != args_delim) {
      die(1, "Unable to parse " ENV_PREFIX "SPAWN_ARGS");
    }
    p = endptr + 1;

    /* Parent_name is everything that remains */
    parent_name = p;
  }

  /* Connect */
  {
    struct sockaddr_in sock_addr;
    GASNET_SOCKLEN_T addr_len;
    struct hostent *h = gethostbyname(parent_name);
    int rc, retry = 4;

    if (h == NULL) {
      gasneti_fatalerror("gethostbyname(%s) failed", parent_name);
    }
    if ((parent = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      gasneti_fatalerror("parent = socket() failed");
    }
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(parent_port);
    sock_addr.sin_addr = *(struct in_addr *)(h->h_addr_list[0]);
    addr_len = sizeof(sock_addr);
    while ((rc = connect(parent, (struct sockaddr *)&sock_addr, addr_len)) < 0) {
      if ((errno != ECONNREFUSED) || !retry) {
        gasneti_fatalerror("connect(host=%s, port=%d) failed w/ errno=%d", parent_name, parent_port, errno);
      } else {
        sleep(1);
        --retry;
      }
    }
  }

  fd_sets_add(parent);
  gasneti_assert_always(mypid == getpid());
  (void)ioctl(parent, SIOCSPGRP, &mypid); /* Enable SIGURG delivery on OOB data */
  (void)fcntl_setfd(parent, FD_CLOEXEC);

#ifdef TCP_NODELAY
  (void)setsockopt(parent, IPPROTO_TCP, TCP_NODELAY, (char *) &c_one, sizeof(c_one));
#endif

  do_write(parent, &child_id, sizeof(gex_Rank_t));

  recv_identity(parent);
  recv_argv(parent, argc_p, argv_p);
  recv_env(parent);

  if (tree_nodes > 1) {
    do_read(parent, &out_degree, sizeof(out_degree));
    recv_nodelist(parent, tree_nodes - 1);
    recv_ssh_argv(parent);
    wrapper = do_read_string(parent);
    gasneti_leak((/*non-const*/ void *)wrapper);
  }

  gasneti_getenv_hook = &do_getenv;
  gasneti_propagate_env_hook = &do_propagate_env;
  envcmd = my_getenv_withdefault(ENV_PREFIX "ENVCMD", "env");

  myname = myrank;
  BOOTSTRAP_VERBOSE(("[%d] Connected via fd=%d\n", myname, parent));
}

static void spawn_one_control(gex_Rank_t child_id, const char *cmdline, const char *my_host) {
  const char *host = child[child_id].nodelist ? child[child_id].nodelist[0] : nodelist[0];
  pid_t pid;
  int is_local = (GASNETI_BOOTSTRAP_LOCAL_SPAWN && (!host || !strcmp(host, my_host)));

#if GASNET_BLCR
  /* Avoid issues with BLCR restoring a pty */
  is_local &= !isatty(STDIN_FILENO);
#endif

  child[child_id].pid = pid = fork();

  if (pid < 0) {
    gasneti_fatalerror("fork() failed");
  } else if (pid == 0) {
    char *cmd;
    cmd = sappendf(NULL, "cd %s; exec %s %s " ENV_PREFIX "SPAWN_CONTROL=ssh "
                                              ENV_PREFIX "SPAWN_ARGS='%c%s%c%d%c%d%c%s' "
                                              "%s",
                                      quote_arg(cwd),
                                      (wrapper ? wrapper : ""),
                                      envcmd,
                                      (is_restart ? 'D' : 'C'),
                                      (is_verbose ? "v" : ""), args_delim,
                                      (int)child_id, args_delim,
                                      listen_port, args_delim,
                                      (is_local ? "localhost" : my_host),
                                      cmdline);
    if (is_local) {
      /* XXX: if we are clever enough, we might be able to "unwind" w/o the exec() */
      BOOTSTRAP_VERBOSE(("[%d] spawning process %d on %s via fork()\n",
			 myname,
			 (int)child[child_id].rank, my_host));
      execlp("sh", "sh", "-c", cmd, NULL);
      gasneti_fatalerror("execlp(sh) failed");
    } else {
      #if HAVE_PR_SET_PDEATHSIG
      if (use_pdeathsig) {
	/* If its parent exits early (an abnormal condition) then we want ssh to exit too */
	(void)prctl(PR_SET_PDEATHSIG, SIGHUP);
      }
      #endif
      BOOTSTRAP_VERBOSE(("[%d] spawning process %d on %s via %s\n",
			 myname,
			 (int)child[child_id].rank, host, ssh_argv[0]));
      ssh_argv[ssh_argc] = (/* noconst */ char *)host;
      ssh_argv[ssh_argc+1] = cmd;
      execvp(ssh_argv[0], ssh_argv);
      gasneti_fatalerror("execvp(ssh) failed");
    }
  }
  gasneti_atomic_increment(&live, 0);
}

/* do_worker could probably be called "undo_control" */
static int do_worker(void) {
  gasneti_reghandler(SIGURG, SIG_DFL);
  gasneti_reghandler(SIGCHLD, SIG_DFL);

  gasneti_free(child-1); /* NOTE: was offset at allocation time */
  child = NULL;
  children = 0;
  ctrl_children = 0;

  if (tree_nodes > 1) {
    int i;

    for (i = 0; i < tree_nodes - 1; ++i) {
      gasneti_free(nodelist[i]);
    }
    gasneti_free(nodelist);
    nodelist = NULL;

    for (i = 0; i < ssh_argc; ++i) {
      gasneti_free(ssh_argv[i]);
    }
    gasneti_free(ssh_argv);
    ssh_argv = NULL;

    gasneti_free((/*non-const*/ void *)wrapper);
    wrapper = NULL;
  }
  tree_nodes = GEX_RANK_INVALID;
  tree_ranks = GEX_RANK_INVALID;

  (void)fcntl_setfd(parent, FD_CLOEXEC);

#if HAVE_PR_SET_PDEATHSIG
  if (use_pdeathsig) {
    /* If our parent exits early (an abnormal condition) then we should too */
    (void)prctl(PR_SET_PDEATHSIG, SIGHUP);
  }
#endif

  BOOTSTRAP_VERBOSE(("[r%d] Connected via fd=%d\n", myrank, parent));

  return GASNET_OK;
}


/* Fork the rank process(es) */
static void spawn_rank(int argc, char **argv) {
  int j;

#if GASNET_BLCR
  const char *restart_dir = NULL;
  if (is_restart) {
    gasneti_assert_always(argc == 2);
    gasneti_assert_always(argv && argv[1]);
    restart_dir = argv[1];
  }
#endif

  for (j = ctrl_children; j < children; ++j) {
    int rc, sv[2];

    rc = my_socketpair(sv);
    if (rc < 0) {
      /* TODO: is any recovery possible? */
      gasneti_fatalerror("socketpair() failed!");
    }

    child[j].sock = sv[1];
    fd_sets_add(sv[1]);
    (void)fcntl_setfd(sv[1], FD_CLOEXEC);

  #if GASNET_BLCR
    if (is_restart) {
      rc = blcr_restart(child[j].rank, restart_dir);
      if (rc < 0) {
        /* BLCR-TODO: is any recovery possible? */
        gasneti_fatalerror("cr restart failed!");
      }
      /* Don't have the actual pid until the request is reaped */
    } else
  #endif
    {
      pid_t pid = fork();
      if (!pid) {
        int k;
        for (k = 0; k <= j; ++k) {
          (void)close(child[k].sock);
        }

        (void)close(parent);
        parent = sv[0];

        signal_forward(0);
        is_control = 0;
        myrank = child[j].rank;
        myname = -911;
        mypid  = getpid();
        (void)my_setpgid(0);

        return;
      } else if (pid < 0) {
        gasneti_fatalerror("fork() failed!");
      }
      (void)my_setpgid(pid);
      child[j].pid = pid;
      ++initialized;
    }

    (void)close(sv[0]);

    gasneti_atomic_increment(&live, 0);
  }
}

/* Spawn control procs via ssh (or fork() when possible) */
static void spawn_ctrl(int argc, char **argv) {
  static char my_host[1024];
  char *cmdline = quote_arg(argv[0]);
  int j;

  const char *masterip;
  if (is_root && (NULL != (masterip = my_getenv(ENV_PREFIX "MASTERIP")))) {
    strncpy(my_host, masterip, sizeof(my_host) - 1);
    my_host[sizeof(my_host) - 1] = '\0';
  } else if (gethostname(my_host, sizeof(my_host)) < 0) {
    die(1, "gethostname() failed");
  }

  /* Before we began to support gasnet_init(NULL,NULL) the workers were spawned
   * with no arguments, and the gasnet_init() call actual *populated* the passed
   * argc_p and argv_p with the command line sent over the control socket.
   * NOW if we see a NULL-init, then we need to use the full command line in order
   * to be certain that main() will see the arguments.
   */
  if (null_init) {
    for (j = 1; j < argc; ++j) {
      char *tmp = quote_arg(argv[j]);
      cmdline = sappendf(cmdline, " %s", tmp);
      gasneti_free(tmp);
    }
  }

  pre_spawn(ctrl_children);
  for (j = 0; j < ctrl_children; ++j) {
    spawn_one_control(j, cmdline, my_host);
  }
  post_spawn(ctrl_children, argc, argv);

  gasneti_free(cmdline);
}

/*----------------------------------------------------------------------------------------------*/

static int do_select(int num_fds, fd_set *rd_fds, fd_set *wr_fds)
{
  int rc;

  do {
    rc = select(num_fds, rd_fds, wr_fds, NULL, NULL);
    if_pf (!rc || ((rc < 0) && (errno != EINTR))) do_abort(-1);
    if_pf (in_abort) return -1;
  } while (rc <= 0);

  return rc;
}

/* Return index of a writable child, removing it from the fd_set */
static int next_write(fd_set *fds)
{
  int j, rc;
  fd_set write_fds = *fds;

  rc = do_select(child_fds.max+1, NULL, &write_fds);
  if (in_abort) return -1;

  for (j = 0; j < children; ++j) {
    if (FD_ISSET(child[j].sock, &write_fds)) {
      FD_CLR(child[j].sock, fds);
      return j;
    }
  }

  /* NOT REACHED */
  return -1; 
}

#define WRITE_EACH_CHILD(_remain, _child, _fdset)         \
  if (0 != (_remain = children))                          \
    for (_fdset = child_fds.set;                          \
         _remain && ((_child = next_write(&(_fdset))),1); \
         --_remain)

static void build_all2all_iov(struct iovec *iov, uint8_t *buf, size_t len, int rank, int size)
{
  size_t row_len = len * nranks;
  size_t run_len = row_len - len * size;
  uint8_t *p = buf + row_len * (rank - myrank);
  int i;

  /* First partial */
  iov[0].iov_base = p;
  iov[0].iov_len  = len * rank;

  /* Skip that partial and the "local" piece */
  p += len * (rank + size);

  /* Contiguous "wrap around" runs */
  for (i = 1; i < size; ++i) {
    iov[i].iov_base = p;
    iov[i].iov_len  = run_len;
    p += row_len;
  }

  /* Final partial */
  iov[size].iov_base = p;
  iov[size].iov_len  = run_len - len * rank;
}

/* In-place square matrix transpose
 * TODO: there are better algorithms than this */
static void transpose(uint8_t *buf, size_t len, size_t n)
{
  const size_t row_len = len * nranks;
  uint8_t *tmp = gasneti_malloc(len);
  gex_Rank_t j, k;
  uint8_t *p0, *q0;
  uint8_t *p1, *q1;

  for (  j = 0, p0 = q0 = buf;     j < n;  ++j, q0 += len, p0 += row_len) {
    for (k = 0, p1 = p0, q1 = q0;  k < j;  ++k, p1 += len, q1 += row_len) {
      GASNETI_MEMCPY(tmp, p1, len);
      GASNETI_MEMCPY(p1,  q1, len);
      GASNETI_MEMCPY(q1, tmp, len);
    }
  }
}

static void cmd_FINI(char cmd, int i) {
  /* Comands: */
  const char cmd0 = BOOTSTRAP_CMD_FINI0;
  const char cmd1 = BOOTSTRAP_CMD_FINI1;

  /* State: */
  static int count = 0;

  if (cmd == cmd0) {
    if (++count < children) return;
    count = 0;
    if (! is_root) {
      do_write(parent, &cmd0, sizeof(cmd0));
      return;
    }
  }

  {
    fd_set fds;
    int j, k;
    WRITE_EACH_CHILD(j,k,fds) {
      if (k < 0) return;
      do_write(child[k].sock, &cmd1, sizeof(cmd1));
    }
    finalized = 1;
  }
}

static void cmd_BARR(char cmd, int i) {
  /* Comands: */
  const char cmd0 = BOOTSTRAP_CMD_BARR0;
  const char cmd1 = BOOTSTRAP_CMD_BARR1;

  /* State: */
  static int count = 0;

  if (cmd == cmd0) {
    if (++count < children) return;
    count = 0;
    if (! is_root) {
      do_write(parent, &cmd0, sizeof(cmd0));
      return;
    }
  }

  {
    fd_set fds;
    int j, k;
    WRITE_EACH_CHILD(j,k,fds) {
      if (k < 0) return;
      do_write(child[k].sock, &cmd1, sizeof(cmd1));
    }
  }
}

static void cmd_BCAST(char cmd, int i) {
  /* Comands: */
  const char cmd0 = BOOTSTRAP_CMD_BCAST0;
  const char cmd1 = BOOTSTRAP_CMD_BCAST1;

  /* State: none */

  /* Local: */
  int s = child[i].sock;
  static size_t len;
  struct iovec iov[3];
  uint8_t *data;

  if (cmd == cmd0) {
    do_read(s, &len, sizeof(len));
    data = gasneti_malloc(len);
    do_read(s, data, len);

    if (! is_root) {
      iov[0].iov_base = (void*)&cmd0;
      iov[0].iov_len  = sizeof(cmd0);
      iov[1].iov_base = (void*)&len;
      iov[1].iov_len  = sizeof(len);
      iov[2].iov_base = data;
      iov[2].iov_len  = len;
      do_writev(parent, iov, 3);
    }
  } else {
    do_read(s, &len, sizeof(len));
    data = gasneti_malloc(len);
    do_read(s, data, len);
  }

  {
    fd_set fds;
    int j, k;
    WRITE_EACH_CHILD(j,k,fds) {
      if (k < 0) return;
      if (k == i) continue;
      iov[0].iov_base = (void*)&cmd1;
      iov[0].iov_len  = sizeof(cmd1);
      iov[1].iov_base = &len;
      iov[1].iov_len  = sizeof(len);
      iov[2].iov_base = data;
      iov[2].iov_len  = len;
      do_writev(child[k].sock, iov, 3);
    }
    gasneti_free(data);
  }
}

static void cmd_EXCHG(char cmd, int i) {
  /* Comands: */
  const char cmd0 = BOOTSTRAP_CMD_EXCHG0;
  const char cmd1 = BOOTSTRAP_CMD_EXCHG1;

  /* State: */
  static uint8_t *data = NULL;
  static int count = 0;
  static size_t len;

  /* Local: */
  struct iovec iov[3];
  int s = child[i].sock;

  if (cmd == cmd0) {
    static size_t exchg_len;

    do_read(s, &len, sizeof(len));
    if (!count) {
      gasneti_assert_always(!data);
      data = gasneti_malloc(len * nranks);
      exchg_len = len;
    } else {
      gasneti_assert_always_int(len ,==, exchg_len);
    }
    do_read(s, data + len * child[i].rank, len * child[i].tree_ranks);

    if (++count < children) return;
    count = 0;

    if (! is_root) {
      iov[0].iov_base = (void*)&cmd0;
      iov[0].iov_len  = sizeof(cmd0);
      iov[1].iov_base = (void*)&len;
      iov[1].iov_len  = sizeof(len);
      iov[2].iov_base = data + len * myrank;
      iov[2].iov_len  = len * tree_ranks;
      do_writev(parent, iov, 3);
      return;
    }
  } else {
    gex_Rank_t next = myrank + tree_ranks;
    iov[0].iov_base = data;
    iov[0].iov_len  = len * myrank;
    iov[1].iov_base = data + len*next;
    iov[1].iov_len  = len * (nranks - next);
    do_readv(s, iov, 2);
  }

  {
    gex_Rank_t rank, next;
    fd_set fds;
    int j, k;

    WRITE_EACH_CHILD(j,k,fds) {
      if (k < 0) return;
      rank = child[k].rank;
      next = rank + child[k].tree_ranks;
      iov[0].iov_base = (void*)&cmd1;
      iov[0].iov_len  = sizeof(cmd1);
      iov[1].iov_base = data;
      iov[1].iov_len  = len * rank;
      iov[2].iov_base = data + len*next;
      iov[2].iov_len  = len * (nranks - next);
      do_writev(child[k].sock, iov, 3);
    }
    gasneti_free(data);
    data = NULL;
  }
}

static void cmd_TRANS(char cmd, int i) {
  /* Comands: */
  const char cmd0 = BOOTSTRAP_CMD_TRANS0;
  const char cmd1 = BOOTSTRAP_CMD_TRANS1;

  /* State: */
  static uint8_t *data = NULL;
  static struct iovec *iov = NULL;
  static int count = 0;
  static size_t len;

  /* Local: */
  int s = child[i].sock;

  if (cmd == cmd0) {
    static size_t trans_len;
    size_t row_len;

    do_read(s, &len, sizeof(len));
    row_len = len * nranks;
    if (!count) {
      gasneti_assert_always(!data);
      data = gasneti_malloc(row_len * tree_ranks);
      iov = gasneti_calloc(3 + tree_ranks, sizeof(struct iovec));
      trans_len = len;
    } else {
      gasneti_assert_always_int(len ,==, trans_len);
    }
    build_all2all_iov(iov, data, len, child[i].rank, child[i].tree_ranks);
    do_readv(s, iov, child[i].tree_ranks + 1);

    if (++count < children) return;
    count = 0;

    transpose(data + len*myrank, len, tree_ranks);
    if (! is_root) {
      iov[0].iov_base = (void*)&cmd0;
      iov[0].iov_len  = sizeof(cmd0);
      iov[1].iov_base = (void*)&len;
      iov[1].iov_len  = sizeof(len);
      build_all2all_iov(iov+2, data, len, myrank, tree_ranks);
      do_writev(parent, iov, 3 + tree_ranks);
      return;
    }
  } else {
    build_all2all_iov(iov, data, len, myrank, tree_ranks);
    do_readv(s, iov, 1 + tree_ranks);
  }

  {
    fd_set fds;
    int j, k;
    WRITE_EACH_CHILD(j,k,fds) {
      if (k < 0) return;
      iov[0].iov_base = (void*)&cmd1;
      iov[0].iov_len  = sizeof(cmd1);
      build_all2all_iov(iov+1, data, len, child[k].rank, child[k].tree_ranks);
      do_writev(child[k].sock, iov, 2 + child[k].tree_ranks);
    }
    gasneti_free(data);
    gasneti_free(iov);
    data = NULL;
    iov = NULL;
  }
}

/* TODO: this gets *much* easier if/when we truly have a single control proc per node */
static void cmd_SNBCAST(char cmd, int i) {
  /* Comands: */
  const char cmd0 = BOOTSTRAP_CMD_SNBCAST0;
  const char cmd1 = BOOTSTRAP_CMD_SNBCAST1;

  /* State: */
  static uint8_t *data = NULL;
  static gex_Rank_t *roots = NULL;
  static struct iovec *iov = NULL;
  static int count = 0;
  static size_t len;

  /* Local: */
  int s = child[i].sock;

  if (cmd == cmd0) {
    static size_t snbcast_len;
    gex_Rank_t *r;

    do_read(s, &len, sizeof(len));
    if (!count) {
      gasneti_assert_always(!data);
      data = gasneti_malloc(tree_ranks * len);
      roots = gasneti_malloc(tree_ranks * sizeof(gex_Rank_t));
      if (! is_root) {
        iov = gasneti_calloc(3 + children, sizeof(struct iovec));
      }
      snbcast_len = len;
    } else {
      gasneti_assert_always_int(len ,==, snbcast_len);
    }

    {
      gex_Rank_t offset = (child[i].rank - myrank);
      gex_Rank_t *r = roots + offset;
      int j, root_count;
      do_read(s, r, child[i].tree_ranks * sizeof(gex_Rank_t));
      for (j = root_count = 0; j < child[i].tree_ranks; ++j) {
        root_count += (r[j] == child[i].rank + j);
      }
      do_read(s, data + len * offset, root_count * len);
      if (! is_root) {
        int order = (i < ctrl_children) ? (i + (children - ctrl_children)) : (i - ctrl_children);
        iov[3 + order].iov_base = data + len * offset;
        iov[3 + order].iov_len  = root_count * len;
      }
    }

    if (++count < children) return;
    count = 0;

    if (is_root) {
      uint8_t **index = gasneti_calloc(nranks, sizeof(uint8_t*));
      uint8_t *p, *tmp = gasneti_malloc(nranks * len);
      gex_Rank_t r = 0;
      int j, k;
      for (j = 0; j < children; ++j) {
        r = child[j].rank;
        p = data + len * r;
        for (k = 0; k < child[j].tree_ranks; ++k, ++r) {
          if (r == roots[r]) {
            index[r] = p;
            p += len;
          }
        }
      }
      for (r = 0; r < nranks; ++r) {
        gasneti_assert_always(index[roots[r]]);
        GASNETI_MEMCPY(tmp + len * r, index[roots[r]], len);
      }
      gasneti_free(data);
      gasneti_free(roots);
      gasneti_free(index);
      data = tmp;
    } else {
      iov[0].iov_base = (void*)&cmd0;
      iov[0].iov_len  = sizeof(cmd0);
      iov[1].iov_base = (void*)&len;
      iov[1].iov_len  = sizeof(len);
      iov[2].iov_base = roots;
      iov[2].iov_len  = tree_ranks * sizeof(gex_Rank_t);
      do_writev(parent, iov, 3 + children);
      gasneti_free(roots);
      gasneti_free(iov);
      return;
    }
  } else {
    do_read(s, data, len * tree_ranks);
  }

  {
    fd_set fds;
    int j, k;
    WRITE_EACH_CHILD(j,k,fds) {
      struct iovec iov[2];
      iov[0].iov_base = (void*)&cmd1;
      iov[0].iov_len  = sizeof(cmd1);
      iov[1].iov_base = data + len * (child[k].rank - myrank);
      iov[1].iov_len  = len * child[k].tree_ranks;
      do_writev(child[k].sock, iov, 2);
    }
    gasneti_free(data);
    data = NULL;
  }
}

#if GASNET_BLCR /* BLCR-specific commands (control and rank merged) */

/* Child restarting from checkpoint is asking parent for its "restart args" */
static void cmd_rstrt_args(int j, char *args_p) {
  /* BLCR-TODO: error checking? */
  if (is_control) {
    if (child[j].rstrt_args & RSTRT_CMD_RESTART) ++initialized;
    do_write(child[j].sock, &child[j].rstrt_args, sizeof(child[j].rstrt_args));
    (void) blcr_reap(child[j].rank, 1); /* BLCR-TODO: error reporting/recovery */
  } else {
    char cmd = BOOTSTRAP_CMD_RSTRT_ARGS;
    do_write(parent, &cmd, sizeof(cmd));
    do_read(parent, args_p, sizeof(*args_p));
  }
}

/* Child is requesting that parent roll it back to the specified context */
static void cmd_rollback(int j, const char *dir) {
  /* BLCR-TODO: error checking? */
  int rc;
  if (is_control) {
    dir = do_read_string(child[j].sock);
  #if HAVE_BLCR_RSTRT_TARGET
    rc = blcr_rollback(child[j].rank, dir);
    gasneti_assert_always(!rc); /* BLCR-TODO: proper error checking/recovery */
  #else
    /* Must first coordinate exit of the child */
    {
      char cmd = BOOTSTRAP_CMD_ROLLBACK;
      sigset_t child_set, old_set;
      int status;
      pid_t pid;
      sigemptyset(&child_set);
      sigaddset(&child_set, SIGCHLD);
      sigprocmask(SIG_BLOCK, &child_set, &old_set);
      do_write(child[j].sock, &cmd, sizeof(cmd)); /* ACK the rollback command */
      do {
        pid = waitpid(child[j].pid, &status, 0);
      } while ((pid < 0) && (errno == EINTR) && !in_abort);
      sigprocmask(SIG_SETMASK, &old_set, NULL);
      if (in_abort) return;
      gasneti_assert_always(pid == child[j].pid);
      gasneti_assert_always(WIFEXITED(status) && !WEXITSTATUS(status));
    }
    /* Now we can restart, but need a new socket pair */
    {
      int sv[2];
      (void) close(child[j].sock);
      rc = my_socketpair(sv);
      if (rc < 0) {
        /* BLCR-TODO: is any recovery possible? */
        gasneti_fatalerror("socketpair() failed!");
      }
      child[j].sock = sv[1];
      (void) fcntl_setfd(sv[1], FD_CLOEXEC);
      blcr_rollback(child[j].rank, dir);
      gasneti_assert_always(!rc); /* BLCR-TODO: proper error checking/recovery */
      (void) close(sv[0]);
    }
  #endif
    gasneti_free((void*)dir);
  } else {
    char cmd = BOOTSTRAP_CMD_ROLLBACK;
    do_write(parent, &cmd, sizeof(cmd));
    do_write_string(parent, dir);
    do_read(parent, &cmd, sizeof(cmd)); /* block for either the ACK or for rollback */
  #if HAVE_BLCR_RSTRT_TARGET
    gasneti_fatalerror("Returned from a rollback request");
  #else
    gasneti_assert_always(cmd == BOOTSTRAP_CMD_ROLLBACK);
    _exit(0);
  #endif
  }
}
#endif

static void dispatch(char cmd, int k) {
  switch (cmd) {
    case BOOTSTRAP_CMD_FINI0:
    case BOOTSTRAP_CMD_FINI1:
      cmd_FINI(cmd, k);
      break;

    case BOOTSTRAP_CMD_BARR0:
    case BOOTSTRAP_CMD_BARR1:
      cmd_BARR(cmd, k);
      break;

    case BOOTSTRAP_CMD_BCAST0:
    case BOOTSTRAP_CMD_BCAST1:
      cmd_BCAST(cmd, k);
      break;

    case BOOTSTRAP_CMD_EXCHG0:
    case BOOTSTRAP_CMD_EXCHG1:
      cmd_EXCHG(cmd, k);
      break;

    case BOOTSTRAP_CMD_TRANS0:
    case BOOTSTRAP_CMD_TRANS1:
      cmd_TRANS(cmd, k);
      break;

    case BOOTSTRAP_CMD_SNBCAST0:
    case BOOTSTRAP_CMD_SNBCAST1:
      cmd_SNBCAST(cmd, k);
      break;

  #if GASNET_BLCR
    case BOOTSTRAP_CMD_RSTRT_ARGS:
      cmd_rstrt_args(k, NULL);
      break;

    case BOOTSTRAP_CMD_ROLLBACK:
      cmd_rollback(k, NULL);
      break;
  #endif

    default:
      fprintf(stderr, "Spawner protocol error\n");
      do_abort(-1);
  }
}

static void event_loop(void) GASNETI_NORETURN;
static void event_loop(void)
{
    int done = 0;

    siginterrupt(SIGCHLD, 1);
    reaper(SIGCHLD);

    while (!finalized && !in_abort) {
      char cmd;
      int k, rc;

      /* Use select to find a fd w/ available work or EOF */
      {
        static int next = 0; /* fairness: start search after prev result */
        int n = all_fds.max + 1;
        fd_set read_fds = all_fds.set;
        int j;

        rc = do_select(n, &read_fds, NULL);
        if (in_abort) break;

        for (j = 0, k = next; j <= children; ++j, ++k) {
          if (k == children) k = is_root ? 0 : -1;
          if (FD_ISSET(child[k].sock, &read_fds)) {
            break;
          }
        }
        next = k + 1;
      }
      gasneti_assert_always(k >= -1);
      gasneti_assert_always(k < children);

      /* Read 1 command byte */
      do_read(child[k].sock, &cmd, sizeof(cmd));
      if (in_abort) break;

      dispatch(cmd, k);
    }

    /* Wait for all children to terminate */
    wait_for_all();

    BOOTSTRAP_VERBOSE(("[%d] Exit with status %d\n", myname, (int)(unsigned char)exit_status));
    exit (exit_status);
}

/* rank process wait for a specific command byte from parent */
static void wait_cmd(char the_cmd) {
  do {
    char cmd;
    do_read(parent, &cmd, sizeof(cmd));
    if ((cmd == the_cmd) || in_abort) return;
    /* TODO: dispatch any valid rank-process commands (none yet) */
    do_abort(127);
  } while (!in_abort);
}

/*----------------------------------------------------------------------------------------------*/

extern int (*gasneti_verboseenv_fn)(void);

static void usage(const char *argv0) {
  die(1,
      "usage: %s [-GASNET-SPAWN-master] [OPTIONS] NPROC[:NODES] [--] [ARGS...]\n"
      "    NPROC: required count of processes to launch\n"
      "    NODES: optional count of nodes to use (defaults to NPROC)\n"
      "       --: optional separator\n"
      "  ARGS...: optional arguments passed to launched processes\n"
      "  OPTIONS:\n"
      "      -v        enables verbose output from spawner\n"
      "      -W[arg]   prefix launch of remote ssh processes with [arg]\n"
      "                example: \"-W'env LD_LIBRARY_PATH=/usr/local/lib64'\"\n"
  #if GASNET_BLCR
      "      -R        Restart from a checkpoint directory given as [ARGS...]\n"
  #endif
      , argv0);
}

#if GASNET_MAXNODES <= INT_MAX
  typedef div_t nodediv_t;
  #define nodediv div
#else
  typedef ldiv_t nodediv_t;
  #define nodediv ldiv
#endif

/* Allocate 'n' from the passed nodediv_t */
static gex_Rank_t nodediv_alloc(nodediv_t *x, gex_Rank_t n) {
  gex_Rank_t rem = MIN(x->rem, n);
  x->rem -= rem;
  return n * x->quot + rem;
}

/* Work common to do_master and do_control */
static void do_common(int argc, char **argv)
{
  gex_Rank_t rank_children;

  /* Arrange to forward various signals */
  signal_forward(1);

  /* Layout children: */
  {
    char **sublist;
    gex_Rank_t rank;
    nodediv_t ppn, npc = {0,0};
    int j;

    ppn = nodediv(tree_ranks, tree_nodes);
    if (is_root) {
      rank_children = 0;
      ctrl_children = MIN(tree_nodes, out_degree);
      npc = nodediv(tree_nodes, ctrl_children);
    } else {
      rank_children = nodediv_alloc(&ppn, 1);
      ctrl_children = MIN(tree_nodes - 1, out_degree);
      if (ctrl_children) {
        npc = nodediv(tree_nodes - 1, ctrl_children);
      }
    }
    children = ctrl_children + rank_children;

    child = gasneti_calloc(1 + children, sizeof(struct child));
    gasneti_leak(child);

    /* parent socket stored as child[-1] */
    child[0].sock = parent;
    ++child;

    /* rank processes take the lower ranks, but the higher slots in child[] */
    rank = myrank;
    for (j = ctrl_children; j < children; ++j) {
      child[j].rank       = rank;
      child[j].tree_ranks = 1;
      child[j].tree_nodes = 1;
      rank += 1;
    }
    gasneti_assert_always(rank == myrank + rank_children);

    /* ctrl processes take the upper ranks, but the lower slots in child[] */
    sublist = nodelist;
    for (j = 0; j < ctrl_children; ++j) {
      const gex_Rank_t nodes = nodediv_alloc(&npc, 1);
      const gex_Rank_t procs = nodediv_alloc(&ppn, nodes);
      child[j].rank       = rank;
      child[j].tree_ranks = procs;
      child[j].tree_nodes = nodes;
      child[j].nodelist   = sublist;
      rank += procs;
      sublist += nodes;
    }
    gasneti_assert_always(rank == myrank + tree_ranks);
  }

  if (ctrl_children) spawn_ctrl(argc, argv);

  if (rank_children) spawn_rank(argc, argv);
  if (!is_control) return; /* A rank process return from spawn_rank */

  event_loop();

  /* NOT REACHED */
}

/* The "main()" for the root master process */
static void do_master(const char *spawn_args, int *argc_p, char ***argv_p) GASNETI_NORETURN;
static void do_master(const char *spawn_args, int *argc_p, char ***argv_p) {
  int argc    = *argc_p;
  char **argv = *argv_p;
  long lnproc = 0;
  long lnnodes = 0;

  is_root = 1;
  is_control = 1;

  fd_sets_init();
  gasneti_reghandler(SIGURG, &sigurg_handler);
  siginterrupt(SIGURG, 1);

  if (NULL == spawn_args) { /* Explicit-master support */
    int argi;

    gasneti_assert_always(argc && argv);
    gasneti_assert_always(0 == strcmp(argv[1], "-GASNET-SPAWN-master"));

    /* Optional args in any order */
    for (argi = 2; argi < argc; ++argi) {
      if (0 == strcmp(argv[argi], "-v")) {
        is_verbose = 1;
      } else if (0 == strncmp(argv[argi], "-W", 2)) {
        wrapper = &argv[argi][2];
    #if GASNET_BLCR
      } else if (0 == strcmp(argv[argi], "-R")) {
        is_restart = 1;
    #endif
      } else {
        break;
      }
    }

    /* Required "nproc" optionally followed by ":nnodes" */
    if (argi >= argc) {
      usage(argv0); /* ran out of args */
    } else {
      const char *p = argv[argi];
      char *endptr;
      lnproc = strtol(p,&endptr,0);
      if ((endptr != p) && (':' == *endptr)) {
        const char *q = endptr + 1;
        lnnodes = strtol(q,&endptr,0);
        nnodes_set = (endptr != q);
      }
      if ((endptr == p) || *endptr) {
        die(1, "Unable to parse process and node count in '%s'", p);
      }
    }
    argi++;

    /* Optional "--" */
    if ((argi < argc) && (strcmp(argv[argi], "--") == 0)) {
      argi++;
    }

    /* Splice out all the args we consumed above */
    argv[argi-1] = argv[0];
    argc -= argi-1;
    argv += argi-1;

  #if GASNET_BLCR
    if (is_restart && (argc != 2)) {
      usage(argv0); /* require exactly one argument */
    }
  #endif
  } else {
    /* Format of spawn_args: "fd:N:[M]:[W]"
     *  :   indicates the delimiter character
     *  fd  is an integer file descriptor to provide he command line
     *  N   is the positive integer process count
     *  M   is the positive node count, or empty
     *  W   is the "wrapper" and is everything (possibly empty) after the last delimiter
     */
    const char *p = spawn_args;
    char *endptr;
    int argv_fd;

    gasneti_assert_always(p && *p);

    /* Required fd number (non-negative unless restarting) */
    argv_fd = strtol(p,&endptr,0);
    if ((endptr == p) || (*endptr != args_delim) || (argv_fd < 0)) {
      die(1, "Failed to parse " ENV_PREFIX "SPAWN_ARGS");
    }
    p = endptr + 1;

    /* Required process count */
    lnproc = strtol(p,&endptr,0);
    if ((endptr == p) || (*endptr != args_delim)) {
      die(1, "Failed to parse " ENV_PREFIX "SPAWN_ARGS");
    }
    p = endptr + 1;

    /* Optional node count */
    lnnodes = strtol(p,&endptr,0);
    if ((endptr == p) && (*p == args_delim)) {
      /* Not present */
    } else if ((endptr == p) || (*endptr != args_delim)) {
      die(1, "Failed to parse " ENV_PREFIX "SPAWN_ARGS");
    } else {
      nnodes_set = 1;
    }
    p = endptr + 1;

    /* Wrapper is whatever remains */
    wrapper = p;

    /* Load the command line (if necessary) from argv_fd */
    if (is_restart || !argv) {
      size_t len;
      char *q, *cmdline;
      int i;
      { struct stat s;
        if (fstat(argv_fd, &s) < 0) {
          die(1, "Unable to read the %s from temporary file %d(%s)",
                 is_restart ? "restart directory" : "command line",
                 errno, strerror(errno));
        }
        len = s.st_size;
      }
      gasneti_leak(cmdline = gasneti_malloc(len));
      do_read(argv_fd, cmdline, len);
      for (q = cmdline, argc = 0; q < (cmdline+len); ++argc) {
        q += strlen(q) + 1;
      }
      argv = gasneti_malloc((argc+1) * sizeof(char*));
      gasneti_leak((void*)argv);
      for (q = cmdline, i = 0; i < argc; ++i) {
        argv[i] = q;
        q += strlen(q) + 1;
      }
      argv[argc] = NULL;
      argv0 = argv[0];
    }
    (void)close(argv_fd);
  }

  nranks = lnproc;
  if ((lnproc < 1) || ((long)nranks != lnproc)) { /* Non-positive or Overflow */
    die(1, "Process count %ld is out-of-range of gex_Rank_t", lnproc);
  }
  /* BLCR-TODO: validate (or overwrite?) nranks when restarting */

  if (nnodes_set) {
    nnodes = lnnodes;
    if ((lnnodes < 1) || ((long)nnodes != lnnodes)) { /* Non-positive or Overflow */
      die(1, "Node count %ld is out-of-range of gex_Rank_t", lnnodes);
    }
    if (nnodes > nranks) {
      fprintf(stderr, "WARNING: requested node count reduced from %d to process count of %d\n",
                      (int)nnodes, (int)nranks);
      fflush(stderr);
      nnodes = nranks;
    }
    BOOTSTRAP_VERBOSE(("Spawning '%s': %d processes on %d nodes\n", argv0, (int)nranks, (int)nnodes));
  } else {
    nnodes = nranks;
    BOOTSTRAP_VERBOSE(("Spawning '%s': %d processes\n", argv0, (int)nranks));
  }

  {
    const char *str = my_getenv(ENV_PREFIX "SSH_OUT_DEGREE");
    if (str) out_degree = atoi(str);
    if (out_degree <= 0) out_degree = nnodes; /* unbounded */
  }

  /* Enable VERBOSEENV */
  gasneti_verboseenv_fn = NULL;

  configure_ssh();

  build_nodelist(); /* May reduce nnodes */

  tree_ranks = nranks;
  tree_nodes = nnodes;

  /* Work common to all ctrl processes: */
  do_common(argc, argv);

  /* NOT REACHED */
  gasneti_fatalerror("Unexpected return from do_common()");
}

/* This is "main()" for control procs other than the root */
static void do_control(const char *spawn_args, int *argc_p, char ***argv_p)
{
  is_root = 0;
  is_control = 1;

  fd_sets_init();
  gasneti_reghandler(SIGURG, &sigurg_handler);
  siginterrupt(SIGURG, 1);

  #if HAVE_PR_SET_PDEATHSIG
  if (use_pdeathsig) {
    /* If parent exits before us (an abnormal condition) then trigger an abort */
    (void)prctl(PR_SET_PDEATHSIG, SIGURG);
  }
  #endif

  /* Connect w/ parent */
  do_connect(spawn_args, argc_p, argv_p);

  /* Work common to all ctrl processes: */
  do_common(*argc_p, *argv_p);

  /* Only rank process should ever return from do_common() */
  gasneti_assert_always(! is_control);
}

/*----------------------------------------------------------------------------------------------*/

/* gasneti_bootstrapInit
 *
 * Upon return:
 *   + argc and argv are those the user specified
 *   + *nodes_p and *mynode_p are set
 *   + the global environment is available via gasneti_getenv()
 *
 * There is no barrier at the end, so it is possible that in a multi-level
 * tree, there are still some processes not yet spawned.  This is OK, since
 * we assume that at least one gasneti_bootstrap*() collectives will follow.
 * Not waiting here allows any subsequent that first collective to overlap
 * with the spawning.
 */
extern gasneti_spawnerfn_t const * gasneti_bootstrapInit_ssh(int *argc_p, char ***argv_p, gex_Rank_t *nodes_p, gex_Rank_t *mynode_p) {
  const char *spawner, *spawn_args;                                                                             int explicit_master = 0;

  null_init = !(argc_p && argv_p);

  if (!null_init && (*argc_p > 1) && !strcmp((*argv_p)[1], "-GASNET-SPAWN-master")) {
    /* Force legacy explict-master support: */
    explicit_master = 1;
    spawn_args = "XX"; // unused, but avoids "may be used uninitialized" warnings
  } else {
    spawner    = my_getenv(ENV_PREFIX "SPAWN_CONTROL");
    spawn_args = my_getenv(ENV_PREFIX "SPAWN_ARGS");
    if (!spawner || !spawn_args || strcmp(spawner, "ssh") || (strlen(spawn_args) < 2)) {
      return NULL;
    }
    gasnett_unsetenv(ENV_PREFIX "SPAWN_ARGS");
  }

  /* Use a "shadow" cmdline if necessary to have a place to receive them */
  if (null_init) {
    static int dummy_argc = 0;
    static char **dummy_argv = NULL;
    argc_p = &dummy_argc;
    argv_p = &dummy_argv;
  } else {
    argv0 = (*argv_p)[0];
  }

  /* set O_APPEND on stdout and stderr (see bug 2136) */
  (void)fcntl_setfl(STDOUT_FILENO, O_APPEND);
  (void)fcntl_setfl(STDERR_FILENO, O_APPEND);

  #ifdef HAVE_PR_SET_PDEATHSIG
  { /* check safety of prctl(PR_SET_PDEATHSIG, ...) */
    struct utsname name;
    if (0 == uname(&name)) {
      const char *dot = strchr(name.release,'.');
      if (NULL != dot) {
        int major = atoi(name.release);
        int minor = atoi(dot + 1);
	use_pdeathsig = ((100 * major + minor) >= 206); /* 2.6.0 kernel or newer */
      }
    }
  }
  #endif

  mypid = getpid();

  if (explicit_master) {
    do_master(NULL, argc_p, argv_p); /* Does not return */
  } else {
    /* Common processing:
     * Format of leading portion of spawn_args is 2 or 3 charachters: "C[v]d"
     *   C = single command character used to dispatch switch() below
     *   v = optional 'v' to enable verbose operation
     *   d = delimiter - the single character in this position will separate the per-command args
     * The commands are passes the portion starting at the delimiter character
     * See do_[foo]() for the per-command arguments
     */
    char spawn_cmd = spawn_args[0];
    is_verbose = (spawn_args[1] == 'v');
    args_delim = spawn_args[1 + is_verbose];
    spawn_args += (2 + is_verbose);

    switch (spawn_cmd) {
    #if GASNET_BLCR
      case 'R': is_restart = 1; GASNETI_FALLTHROUGH
    #endif
      case 'M':  /* The master (root control process) */
        do_master(spawn_args, argc_p, argv_p); /* Does not return */
        break;

    #if GASNET_BLCR
      case 'D': is_restart = 1; GASNETI_FALLTHROUGH
    #endif
      case 'C':  /* Non-root control process */
        do_control(spawn_args, argc_p, argv_p);
        break;

      default:
        return NULL;
    }
  }

  /* Reach here only in the rank processes */
  gasneti_assert_always(! is_control);

  gasneti_getenv_hook = &do_getenv;
  gasneti_propagate_env_hook = &do_propagate_env;
  *nodes_p  = nranks;
  *mynode_p = myrank;

  if (do_worker() == GASNET_OK) return &spawnerfn;
  else return NULL;
}

/* bootstrapFini
 */
static void bootstrapFini(void) {
  char cmd0 = BOOTSTRAP_CMD_FINI0;
  char cmd1 = BOOTSTRAP_CMD_FINI1;

  do_write(parent, &cmd0, sizeof(cmd0));

  wait_cmd(cmd1);
}

/* bootstrapAbort
 *
 * Force immediate (abnormal) termination.
 */
static void bootstrapAbort(int exitcode) {
  BOOTSTRAP_VERBOSE(("[r%d] Abort(%d)\n", myrank, exitcode));
  do_abort((unsigned char)exitcode);
  gasneti_fatalerror("do_abort failed.");
  /* NOT REACHED */
}

static void bootstrapBarrier(void) {
  const char cmd0 = BOOTSTRAP_CMD_BARR0;
  const char cmd1 = BOOTSTRAP_CMD_BARR1;

  do_write(parent, &cmd0, sizeof(cmd0));

  wait_cmd(cmd1);
}

static void bootstrapExchange(void *src, size_t len, void *dest) {
  char cmd0 = BOOTSTRAP_CMD_EXCHG0;
  char cmd1 = BOOTSTRAP_CMD_EXCHG1;
  const gex_Rank_t next = myrank + 1;
  struct iovec iov[3];

  iov[0].iov_base = &cmd0;
  iov[0].iov_len  = sizeof(cmd0);
  iov[1].iov_base = (void *)&len;
  iov[1].iov_len  = sizeof(len);
  iov[2].iov_base = src;
  iov[2].iov_len  = len;
  do_writev(parent, iov, 3);

  GASNETI_MEMCPY_SAFE_IDENTICAL((uint8_t*)dest + len*myrank, src, len);

  iov[0].iov_base = dest;
  iov[0].iov_len  = len * myrank;
  iov[1].iov_base = (uint8_t*)dest + len * next;
  iov[1].iov_len  = len * (nranks - next);
  wait_cmd(cmd1);
  do_readv(parent, iov, 2);
}

static void bootstrapAlltoall(void *src, size_t len, void *dest) {
  char cmd0 = BOOTSTRAP_CMD_TRANS0;
  char cmd1 = BOOTSTRAP_CMD_TRANS1;
  struct iovec iov[4];

  iov[0].iov_base = &cmd0;
  iov[0].iov_len  = sizeof(cmd0);
  iov[1].iov_base = (void *)&len;
  iov[1].iov_len  = sizeof(len);
  build_all2all_iov(iov+2, src, len, myrank, 1);
  do_writev(parent, iov, 4);

  GASNETI_MEMCPY_SAFE_IDENTICAL((uint8_t*)dest + len*myrank, (uint8_t*)src + len*myrank, len);

  build_all2all_iov(iov, dest, len, myrank, 1);
  wait_cmd(cmd1);
  do_readv(parent, iov, 2);
}

static void bootstrapBroadcast(void *src, size_t len, void *dest, int rootnode) {
  const char cmd0 = BOOTSTRAP_CMD_BCAST0;
  const char cmd1 = BOOTSTRAP_CMD_BCAST1;
  struct iovec iov[3];

  if (myrank == rootnode) {
    iov[0].iov_base = (void *)&cmd0;
    iov[0].iov_len  = sizeof(cmd0);
    iov[1].iov_base = (void *)&len;
    iov[1].iov_len  = sizeof(len);
    iov[2].iov_base = src;
    iov[2].iov_len  = len;
    do_writev(parent, iov, 3);

    GASNETI_MEMCPY_SAFE_IDENTICAL(dest, src, len);
  } else {
    size_t bcast_len;
    wait_cmd(cmd1);
    iov[0].iov_base = (void *)&bcast_len;
    iov[0].iov_len  = sizeof(bcast_len);
    iov[1].iov_base = dest;
    iov[1].iov_len  = len;
    do_readv(parent, iov, 2);
    gasneti_assert_always_int(len ,==, bcast_len);
  }
}

static void bootstrapSNodeBroadcast(void *src, size_t len, void *dest, int rootnode_arg) {
  char cmd0 = BOOTSTRAP_CMD_SNBCAST0;
  char cmd1 = BOOTSTRAP_CMD_SNBCAST1;
  const gex_Rank_t rootnode = rootnode_arg;
  struct iovec iov[4];

  iov[0].iov_base = &cmd0;
  iov[0].iov_len  = sizeof(cmd0);
  iov[1].iov_base = (void *)&len;
  iov[1].iov_len  = sizeof(len);
  iov[2].iov_base = (void *)&rootnode;
  iov[2].iov_len  = sizeof(rootnode);
  iov[3].iov_base = src;
  iov[3].iov_len  = len;
  do_writev(parent, iov, (myrank == rootnode) ? 4 : 3);

  wait_cmd(cmd1);
  do_read(parent, dest, len);
}

static void bootstrapCleanup(void) {
  /* TODO: anything we can free at end of bootstrap collectives? */
}

/*----------------------------------------------------------------------------------------------*/
#if GASNET_BLCR
/* Optional support for checkpoint/restart via BLCR
 * NOTE: BLCR uses "cr_" as a namespace prefix.  So our use of "blcr_" is safe.
 * TODO: move most (all?) of this to gasnet_blcr.c
 */

/* Returns non-zero if done, 0 if not */
static int blcr_reap(gex_Rank_t rank, int block) {
  const int k = ctrl_children + (rank - myrank); /* Index in child[] */

  if (child[k].rstrt_state == RSTRT_STATE_REQUESTED) {
    struct timeval tv_zero = {0,0};
    int rc;

    /* NOTE: reap of a rollback returns 0, making use of cr_poll_restart() ambiguous.
     * So, we will use distinct wait and reap operations.
     */

    do {
      rc = cr_wait_restart(&child[k].rstrt_handle, block ? NULL : &tv_zero);
    } while ((rc < 0) && (errno == EINTR) && !in_abort);
    if (in_abort) return -1;
    if (!rc) return 0;
    if (rc < 0) {
      fprintf(stderr, "cr_wait_restart(rank %d) failed\n", rank);
      do_abort(127);
      return -1;
    }

    do {
      rc = cr_reap_restart(&child[k].rstrt_handle);
    } while ((rc < 0) && (errno == EINTR) && !in_abort);
    if (in_abort) return -1;
    if (rc < 0) {
      fprintf(stderr, "cr_reap_restart(rank %d) failed\n", rank);
      do_abort(127);
      return -1;
    }

    if (! child[k].pid) child[k].pid = (pid_t)rc;
    child[k].rstrt_state = RSTRT_STATE_RUNNING;
    blcr_live_requests -= 1;
  }

  return 1;
}

static int blcr_rstart_request(gex_Rank_t rank, const char *dir, char rstrt_args) {
    const int rollback = (rstrt_args & RSTRT_CMD_ROLLBACK);
    char *filename = sappendf(NULL, "%s/context.%d", dir, rank);
    cr_restart_args_t args;
    int j, k;
    int fd, rc;

    /* Limit the number of concurrent restart requests in-flight */
    if (blcr_live_requests == blcr_max_requests) {
      /* First: non-blocking scan of all lower ranks */
      for (j = myrank; j < rank; ++j) {
        (void) blcr_reap(j, 0);
      }
      /* Second: block for the oldest request that was not already complete */
      if (blcr_live_requests == blcr_max_requests) {
        for (j = myrank; j < rank; ++j) {
          k = ctrl_children + (j - myrank);
          gasneti_assert_always(child[k].rank == j);
          if (! child[k].rstrt_handle) continue;
          (void) blcr_reap(j, 1);
          break;
        }
        gasneti_assert_always(j < rank);
      }
      gasneti_assert_always(blcr_live_requests < blcr_max_requests);
    }

    fd = open(filename, O_RDONLY|O_LARGEFILE);
    if (fd < 0) {
      /* BLCR-TODO: error reporting/recovery */
      fprintf(stderr, "open(%s) failed\n", filename);
      do_abort(127);
      return fd;
    }

    cr_initialize_restart_args_t(&args);
    args.cr_flags = CR_RSTRT_ASYNC_ERR | CR_RSTRT_RESTORE_PID | CR_RSTRT_RESTORE_PGID;
    args.cr_fd = fd;
  #if HAVE_BLCR_RSTRT_TARGET
    if (rollback) {
      args.cr_target = CR_RSTRT_TARGET_AUTO;
      args.cr_scope  = CR_SCOPE_PROC;
    }
  #endif

    k = ctrl_children + (rank - myrank);
    rc = cr_request_restart(&args, &child[k].rstrt_handle);
    if (rc < 0) {
      /* BLCR-TODO: error reporting/recovery */
      fprintf(stderr, "cr_request_restart(%s) failed\n", filename);
      do_abort(127);
      return rc;
    }
    child[k].rstrt_args = rstrt_args;
    child[k].rstrt_state = RSTRT_STATE_REQUESTED;

    (void)close(fd);
    gasneti_free(filename);

    blcr_live_requests += 1;

    BOOTSTRAP_VERBOSE(("[%d] Requested %s of rank %d\n",
                            myname, rollback?"rollback":"restart", rank));

    return 0; /* This will become (temporarily) the child's pid */
}

static int blcr_restart(gex_Rank_t rank, const char *dir) {
    return blcr_rstart_request(rank, dir, RSTRT_CMD_RESTART | (is_verbose?RSTRT_VERBOSE:0));
}


static int blcr_rollback(gex_Rank_t rank, const char *dir) {
    return blcr_rstart_request(rank, dir, RSTRT_CMD_ROLLBACK | (is_verbose?RSTRT_VERBOSE:0));
}

static int bootstrapRollback(const char *dir) {
  if (dir) {
    (void)fcntl_clrfd(parent, FD_CLOEXEC);
    cmd_rollback(-1, dir);
    /* NOT REACHED */
  }
  return GASNET_OK;
}

static int bootstrapPreCheckpoint(int fd) {
  return GASNET_OK;
}

static int bootstrapPostCheckpoint(int fd, int restart) {
  if (restart) {
    char restart_args;
    cmd_rstrt_args(-1, &restart_args);

    (void)fcntl_setfd(parent, FD_CLOEXEC);

    is_verbose = (restart_args & RSTRT_VERBOSE);
    restart_args &= ~RSTRT_VERBOSE;

    switch (restart_args) {
      case RSTRT_CMD_RESTART:
        BOOTSTRAP_VERBOSE(("[r%d] Rank proc restarted\n", myrank));
        break;

      case RSTRT_CMD_ROLLBACK:
        BOOTSTRAP_VERBOSE(("[r%d] Rank proc rolled-back\n", myrank));
        break;

      default:
        fprintf(stderr, "Spawner/restart protocol error\n");
        do_abort(-1);
    }

    (void)my_setpgid(0); /* BLCR-TODO: this is almost certainly unnecessary (but harmless) */
  }

  return GASNET_OK;
}

#endif /* GASNET_BLCR */

/*----------------------------------------------------------------------------------------------*/

static gasneti_spawnerfn_t const spawnerfn = {
  bootstrapBarrier,
  bootstrapExchange,
  bootstrapBroadcast,
  bootstrapSNodeBroadcast,
  bootstrapAlltoall,
  bootstrapAbort,
  bootstrapCleanup,
  bootstrapFini,
#if GASNET_BLCR
  bootstrapPreCheckpoint,
  bootstrapPostCheckpoint,
  bootstrapRollback,
#endif
};

