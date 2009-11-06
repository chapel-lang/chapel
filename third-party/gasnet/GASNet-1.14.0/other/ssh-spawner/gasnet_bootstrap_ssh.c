/*   $Source: /var/local/cvs/gasnet/other/ssh-spawner/gasnet_bootstrap_ssh.c,v $
 *     $Date: 2009/09/18 23:33:38 $
 * $Revision: 1.68 $
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
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/select.h>
#if HAVE_NETINET_TCP_H
  #include <netinet/tcp.h>
#endif
#if HAVE_PR_SET_PDEATHSIG
  /* Under Linux we can ask to be sent a given signal when our parent exits.
   * We use if mainly because of a bug in some versions of OpenSSH that
   * fail to kill spawned children when the ssh exits.  However, this will
   * also make us more resistant to runaways if somebody starts sending
   * SIGKILL to our processes (which leaves us no other way to cleanup
   * gracefully).  Not safe on some Linux kernels.
   */
  #include <sys/prctl.h>
  #include <signal.h>
#endif
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>

#ifndef GASNET_SOCKLEN_T
  #error "Don't know socklen_t or equivalent"
#endif

/* NOTES

   This is a ssh-based (or rsh is you want) spawner for GASNet.  It is
   intended to be conduit-neutral.
  
   In the interest of scalability the ssh processes are started up in
   a balanced N-ary tree, where N is determined at compile-time (the
   value of "OUT_DEGREE", set below.  Typically we want this value to
   be resonably large, since deep trees would result in multiple steps
   of forwarding for standard I/O (which is performed entirely by the
   ssh processes at this point).

   The process corresponding to gasnet node zero is the root of this
   tree, but it also has a parent: the original process started by the
   user (or upcrun, etc.), the "master" process.  This allows for the
   possibility that the spawner is launched somewhere other than a
   compute node, such as a front end node.

   In addition to the tree of ssh connections, there is a TCP socket
   created between each process and its parent (including between the
   root and the master).  This socket is used for control information,
   during startup.  For instance, the the environment and arguments
   are transferred over this socket.

   The control sockets are used to send each process only a portion of
   the list of host names.  Rather than send the entire list, each processs
   receives only its own host name and that of any children it may have in
   the tree.

   The spawner is able to (in most cases) avoid orphaned processes
   by using TCP out-of-band data to generate a SIGURG.  The handler for
   this signal will send the out-of-band data both up and down the tree
   before exiting.

   If a child has the same hostname as its parent, it will be started
   directly, rather than via ssh.

   The tree structure is used to provide scalable implementations of
   the following "service" routines for use during the bootstrap, as
   required in the template-conduit:
      extern void gasneti_bootstrapBarrier_ssh(void);
      extern void gasneti_bootstrapExchange_ssh(void *src, size_t len, void *dest);
      extern void gasneti_bootstrapBroadcast_ssh(void *src, size_t len, void *dest, int rootnode);
   
   Additionally, the following is useful (at least in vapi-conduit)
   for exchanging endpoint identifiers in a scalable manner:
      extern void gasneti_bootstrapAlltoall_ssh(void *src, size_t len, void *dest);

   If demand exists, scalable Scatter and Gather are possible.

   The following are needed to handle startup and termination:
      extern void gasneti_bootstrapInit_ssh(int *argc_p, char ***argv_p,
                                            gasnet_node_t *nodes_p,
                                            gasnet_node_t *mynode_p);
      extern void gasneti_bootstrapFini_ssh(void);
      extern void gasneti_bootstrapAbort_ssh(int exitcode);
   In the case of normal termination, all nodes should call
   gasneti_bootstrapFini_ssh() before they call exit().  In the event that
   gasnet is unable to arrange for an orderly shutdown, a call to
   gasneti_bootstrapAbort_ssh() will try to force all processes to exit
   with the given exit code.

   To control the spawner, there are a few environment variables, all
   of which are processed only by the master process (which send the
   relavent information on to the others via the control sockets).
   See README for documentation on these variables.

   XXX: still to do
   + Make flat-tree a runtime switch rather than compile time.
   + Consider making OUT_DEGREE a runtime variable.
   + Implement "custom" spawner in the spirit of udp-conduit.
   + Look at udp-conduit for things missing from this list. :-)
   + We probably leak small strings in a few places.

 */

/* Defaults if conduit has not set these values */
#ifndef GASNETI_BOOTSTRAP_OUT_DEGREE
  #define GASNETI_BOOTSTRAP_OUT_DEGREE 32
#endif
#ifndef GASNETI_BOOTSTRAP_LOCAL_SPAWN
  #define GASNETI_BOOTSTRAP_LOCAL_SPAWN 0
#endif
#ifndef GASNETI_BOOTSTRAP_FLAT_TREE
  #define GASNETI_BOOTSTRAP_FLAT_TREE 1
#endif

#define WHITESPACE " \t\n\r"
#define SSH_SERVERS_DELIM_CHARS  ",/;:" WHITESPACE

extern char **environ;
#ifndef ENV_PREFIX
  #define ENV_PREFIX "GASNET_"
#endif

enum {
  BOOTSTRAP_CMD_NO_OP,
  BOOTSTRAP_CMD_FINI0,
  BOOTSTRAP_CMD_FINI1,
  BOOTSTRAP_CMD_BARR0,
  BOOTSTRAP_CMD_BARR1,
  BOOTSTRAP_CMD_BCAST,
  BOOTSTRAP_CMD_EXCHG,
  BOOTSTRAP_CMD_TRANS
};

/* Master & slaves */
  static int is_master = 0;
  static int is_verbose = 0;
  static gasnet_node_t nproc = 0;
  static char cwd[1024];
  static int devnull = -1;
  static int listener = -1;
  static int listen_port = -1;
  static const char *argv0;
  static char **nodelist;
  static char **ssh_argv = NULL;
  static int ssh_argc = 0;
  static const char *wrapper = NULL;
  static char *master_env = NULL;
  static size_t master_env_len = 0;
  static struct child {
    int			sock;
    pid_t		pid;	/* pid of ssh (or locally exec()ed app) */
    gasnet_node_t	rank;
    gasnet_node_t	procs;	/* size in procs of subtree rooted at this child */
    gasnet_node_t	nodes;	/* size in nodes of subtree rooted at this child */
    char **		nodelist;
  } *child = NULL;
  static int children = 0;
  static volatile int accepted = 0;
  static int finalized = 0;
  static gasneti_atomic_t live = gasneti_atomic_init(0);
  static volatile int in_abort = 0;
/* Slaves only */
  static gasnet_node_t myproc = (gasnet_node_t)(-1L);
  static gasnet_node_t tree_procs = (gasnet_node_t)(-1L);
  static gasnet_node_t tree_nodes = (gasnet_node_t)(-1L);
  static int parent = -1; /* socket */
  static int mypid;
/* Master only */
  static volatile int exit_status = 0;
  static gasnet_node_t nnodes = 0;	/* nodes, as distinct from procs */
  static pid_t *all_pids;

static void gather_pids(void);

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
  char *result = gasneti_strdup("'");
  char *p, *q, *tmp;

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
  char expandedmsg[255];

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

static void kill_one(const char *rem_host, pid_t rem_pid) {
  pid_t pid;
 
  gasneti_assert(is_master);
  gasneti_assert(rem_host != NULL);

  if (rem_pid == 0) return;

  pid = fork();
  if (pid < 0) {
    gasneti_fatalerror("fork() failed");
  } else if (pid == 0) {
    (void)dup2(STDIN_FILENO, devnull);
#if !GASNET_DEBUG
    (void)dup2(STDOUT_FILENO, devnull);
    (void)dup2(STDERR_FILENO, devnull);
#endif
    ssh_argv[ssh_argc] = (/* noconst */ char *)rem_host;
    ssh_argv[ssh_argc+1] = sappendf(NULL, "cd %s; exec %s -GASNET-SPAWN-kill %d",
				      quote_arg(cwd), quote_arg(argv0), rem_pid);
    execvp(ssh_argv[0], ssh_argv);
    gasneti_fatalerror("execvp(ssh kill) failed");
  }
  BOOTSTRAP_VERBOSE(("[-1] Pid %d killing %s:%d\n", pid, rem_host, (int)rem_pid));
  gasneti_atomic_increment(&live, 0);
}

static void clean_up(void)
{
  gasnet_node_t p_quot = nproc / nnodes;
  gasnet_node_t p_rem = nproc % nnodes;
  gasnet_node_t j, rank;

  gasneti_assert(is_master);
  fprintf(stderr, "Cleaning up orphaned processes...\n");

  gasneti_reghandler(SIGQUIT, SIG_DFL);
  gasneti_reghandler(SIGINT,  SIG_DFL);
  gasneti_reghandler(SIGTERM, SIG_DFL);
  gasneti_reghandler(SIGHUP,  SIG_DFL);
  gasneti_reghandler(SIGPIPE, SIG_DFL);

  for (j = 0, rank = 0; j < nnodes; ++j) {
    gasnet_node_t i;
    for (i = p_quot + ((j<p_rem)?1:0); i != 0; --i, ++rank) {
      kill_one(nodelist[j], all_pids[rank]);
    }
  }
}

/* Note that these are fired off asynchronously */
static void signal_one(const char *rem_host, pid_t rem_pid, int sig) {
  /* XXX: could avoid ssh for local case */
  pid_t pid;
 
  gasneti_assert(is_master);
  gasneti_assert(rem_host != NULL);

  if (rem_pid == 0) return;

  pid = fork();
  if (pid < 0) {
    gasneti_fatalerror("fork() failed");
  } else if (pid == 0) {
    BOOTSTRAP_VERBOSE(("[-1] Sending signal %d to %s:%d\n", sig, rem_host, (int)rem_pid));
    (void)dup2(STDIN_FILENO, devnull);
#if !GASNET_DEBUG
    (void)dup2(STDOUT_FILENO, devnull);
    (void)dup2(STDERR_FILENO, devnull);
#endif
    ssh_argv[ssh_argc] = (/* noconst */ char *)rem_host;
    ssh_argv[ssh_argc+1] = sappendf(NULL, "sh -c 'kill -s %d %d 2>/dev/null'", sig, rem_pid);
    execvp(ssh_argv[0], ssh_argv);
    gasneti_fatalerror("execvp(ssh kill) failed");
  }
  gasneti_atomic_increment(&live, 0);
}

static void signal_all(int sig)
{
  gasnet_node_t p_quot = nproc / nnodes;
  gasnet_node_t p_rem = nproc % nnodes;
  gasnet_node_t j, rank;

  gasneti_assert(is_master);

  for (j = 0, rank = 0; j < nnodes; ++j) {
    gasnet_node_t i;
    for (i = p_quot + ((j<p_rem)?1:0); i != 0; --i, ++rank) {
      signal_one(nodelist[j], all_pids[rank], sig);
    }
  }
}

/* master forwards signals if possible */
static void sigforward(int sig)
{
  gasneti_assert(is_master);

  /* If we get it a second time don't forward */
  gasneti_reghandler(sig, SIG_DFL);

  if (child) {
    BOOTSTRAP_VERBOSE(("[-1] Forwarding signal %d\n", sig));
#if 0
    signal_one(nodelist[0], all_pids[0], sig);
#else
    signal_all(sig);
#endif
  } else {
    BOOTSTRAP_VERBOSE(("[-1] Resending signal %d to self\n", sig));
    raise(sig);
  }
}

static void do_oob(unsigned char exitcode) {
  const int flags = MSG_OOB 
#ifdef MSG_DONTWAIT
	  		| MSG_DONTWAIT
#endif
#ifdef MSG_NOSIGNAL
			| MSG_NOSIGNAL
#endif
			;
  int j;

  gasneti_reghandler(SIGURG, SIG_IGN);
  gasneti_reghandler(SIGPIPE, SIG_IGN);

  if (!is_master) {
    send(parent, &exitcode, 1, flags);
  }
  if (child) {
    for (j = 0; j < children; ++j) {
      (void)send(child[j].sock, &exitcode, 1, flags);
    }
  }
}

/* Cause everything to exit.
 * In the master process this returns to allow a full cleanup.
 * In slave processes there is no returning.
 */
static void do_abort(unsigned char exitcode) {
  /* avoid reentrance */
  if (in_abort) {
    return;
  } else {
    in_abort = 1;
  }

  do_oob(exitcode);
  if (is_master) {
    clean_up();
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
  gasneti_assert(pid);

  gasneti_atomic_decrement(&live, 0);
  BOOTSTRAP_VERBOSE(("[%d] Reaped pid %d (%d left)\n",
		     is_master ? -1 : myproc, (int)pid, (int)gasneti_atomic_read(&live, 0)));

  if (child) {
    int j;

    for (j = 0; j < children; ++j) {
      if (pid == child[j].pid) {
        (void)close(child[j].sock);
	if (WIFEXITED(status)) {
	  if (exit_status == 0) exit_status = WEXITSTATUS(status);
	  BOOTSTRAP_VERBOSE(("[%d] Process %d exited with status %d\n",
				  is_master ? -1 : myproc, child[j].rank, WEXITSTATUS(status)));
	} else if (WIFSIGNALED(status)) {
	  if (exit_status == 0) exit_status = WTERMSIG(status);
	  BOOTSTRAP_VERBOSE(("[%d] Process %d died with signal %d\n",
				  is_master ? -1 : myproc, child[j].rank, WTERMSIG(status)));
	} else {
	  BOOTSTRAP_VERBOSE(("[%d] Process %d exited with status unknown\n",
				  is_master ? -1 : myproc, child[j].rank));
	}
        if (!finalized) {
	  BOOTSTRAP_VERBOSE(("[%d] Process %d exited before finalize\n", is_master ? -1 : myproc, child[j].rank));
	  do_abort(-1);
	}
	break;
      }
    }
  }

  if (accepted < children) {
    gasneti_fatalerror("One or more processes died before setup was completed");
  }
}

static void reaper(int sig)
{
  pid_t pid;
  int status;

  if (sig) {
    gasneti_assert(sig == SIGCHLD);
    gasneti_reghandler(SIGCHLD, &reaper);
  } else {
    gasneti_reghandler(SIGCHLD, SIG_DFL);
  }

  while((pid = waitpid(-1,&status,WNOHANG)) > 0) {
    reap_one(pid, status);
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
			    is_master ? -1 : myproc, gasneti_atomic_read(&live, 0)));
    sigsuspend(&old_set);
  }
}

static void sigurg_handler(int sig)
{
  unsigned char exitcode = 255;
  int j;

  BOOTSTRAP_VERBOSE(("[%d] Received SIGURG\n", is_master ? -1 : (int)myproc));

  /* We need to read our single byte of urgent data here.
   * Since we don't know which socket sent it, we just
   * try them all.  MSG_OOB is supposed to always be
   * non-blocking.  If multiple sockets have OOB data
   * pending, we don't care which one we read.
   */
  (void)recv(parent, &exitcode, 1, MSG_OOB);
  if (child) {
    for (j = 0; j < children; ++j) {
      (void)recv(child[j].sock, &exitcode, 1, MSG_OOB);
    }
  }

  do_abort(exitcode);
  /* NOT REACHED */
}

static void do_write(int fd, const void *buf, size_t len)
{
  const char *p = (const char *)buf;
  while (len) {
    ssize_t rc = write(fd, p, len);
    if_pf (rc <= 0) {
      do_abort(-1);
      break;
    }
    p += rc;
    len -= rc;
  }
}

static void do_write_string(int fd, const char *string) {
  size_t len = string ? strlen(string) : 0;
  do_write(fd, &len, sizeof(len));
  do_write(fd, string, len);
}

static void do_read(int fd, void *buf, size_t len)
{
  char *p = (char *)buf;
  while (len) {
    ssize_t rc = read(fd, p, len);
    if_pf (rc <= 0) {
      do_abort(-1);
      break;
    }
    p += rc;
    len -= rc;
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
      memcpy(p , string, i); p += i;
      gasneti_assert((uintptr_t)(p-tmp) < (sizeof(tmp)-1));
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
            gasneti_assert((uintptr_t)(p-tmp) < (sizeof(tmp)-1));
	  } else {
	    /* Keep the backslash */
	    memcpy(p , string, 2); p += 2;
            gasneti_assert((uintptr_t)(p-tmp) < (sizeof(tmp)-1));
	  }
	  string += 2;
	  break;
	case '\'':
	  ++string;
	  i = strcspn(string, "\'");
	  if (string[i] != '\'') {
	    die(1, "unbalanced ' %s", where);
	  }
          memcpy(p , string, i); p += i;
          gasneti_assert((uintptr_t)(p-tmp) < (sizeof(tmp)-1));
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
      gasneti_assert((uintptr_t)(p-tmp) < sizeof(tmp));
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

/* wrapper that maps default/empty to NULL */
static char *my_getenv(const char *key) {
  char *env_string = gasneti_getenv_withdefault(key, "");
  return ((env_string != NULL) && strlen(env_string)) ? env_string : NULL;
}

static void configure_ssh(void) {
  char *env_string;
  char *ssh_argv0;
  char **ssh_options = NULL;
  int is_openssh = 0;
  int optcount = 0;
  int i, argi;

  /* Determine the ssh command */
  ssh_argv0 = gasneti_getenv_withdefault(ENV_PREFIX "SSH_CMD", "ssh");

  /* Check for OpenSSH */
  {
    char *cmd = sappendf(NULL, "%s -v 2>&1 | grep OpenSSH >/dev/null 2>/dev/null", ssh_argv0);
    is_openssh = (0 == system(cmd));
    gasneti_free(cmd);
    BOOTSTRAP_VERBOSE(("Configuring for OpenSSH\n"));
  }

  /* Check for user-supplied options */
  if ((env_string = my_getenv(ENV_PREFIX "SSH_OPTIONS")) != NULL) {
    ssh_options = parse_options(env_string, &optcount, "while parsing " ENV_PREFIX "SSH_OPTIONS");
  }

  /* Now build the command line */
  ssh_argc = optcount + (is_openssh ? 9 : 1);
  ssh_argv = gasneti_calloc((ssh_argc + 3 /* host + cmd + NULL = 3 */), sizeof(char *));
  ssh_argv[0] = ssh_argv0;
  argi = 1;
  if (is_openssh) {
    ssh_argv[argi++] = (char *)"-o"; ssh_argv[argi++] = (char *)"StrictHostKeyChecking no";
    ssh_argv[argi++] = (char *)"-o"; ssh_argv[argi++] = (char *)"FallBackToRsh no";
    ssh_argv[argi++] = (char *)"-o"; ssh_argv[argi++] = (char *)"BatchMode yes";
    ssh_argv[argi++] = (char *)"-o"; ssh_argv[argi++] = (char *)"ForwardX11 no";
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

/* Build an array of hostnames from a file */
static char ** parse_nodefile(const char *filename) {
  char **result = NULL;
  gasnet_node_t i;
  FILE *fp;

  BOOTSTRAP_VERBOSE(("Parsing nodefile '%s'\n", filename));
  fp = fopen(filename, "r");
  if (!fp) {
    die(1, "failed to open nodefile '%s'", filename);
  }

  result = gasneti_malloc(nnodes * sizeof(char *));
  for (i = 0; i < nnodes;) {
    static char buf[1024];
    char *p;

    if (!fgets(buf, sizeof(buf), fp)) {
      /* ran out of lines */
      die(1, "Out of lines in nodefile '%s'", filename);
    }
 
    p = buf;
    while (*p && strchr(WHITESPACE, *p)) ++p; /* eat leading whitespace */
    if (*p != '#') {
      p[strcspn(p, WHITESPACE)] = '\0';
      result[i] = gasneti_strdup(p);
#if 0
      /* XXX: Eat consecutive duplicates ?
       * When running w/ PBS and nodes=2:ppn=2 we see NODE0,NODE0,NODE1,NODE1
       * while running the same config w/ LSF gives just NODE0,NODE1.
       * We'd get consistent results by squeezing out the PBS style dups,
       * but the upcrun handling currently doesn't do this so we don't either.
       */
      if ((i > 0) && !strcmp(result[i], result[i-1])) {
        gasneti_free(result[i]);
	continue;
      }
#endif
      ++i;
      BOOTSTRAP_VERBOSE(("\t%s\n", p));
    }
  }

  (void)fclose(fp);

  return result;
}

/* Build an array of hostnames from a delimited string */
static char ** parse_servers(const char *list) {
  static const char *delims = SSH_SERVERS_DELIM_CHARS;
  char **result = NULL;
  char *string, *alloc;
  gasnet_node_t i;

  alloc = string = gasneti_strdup(list);
  result = gasneti_malloc(nnodes * sizeof(char *));
  BOOTSTRAP_VERBOSE(("Parsing servers list '%s'\n", string));
  for (i = 0; i < nnodes; ++i) {
    char *p;
    while (*string && strchr(delims,*string)) ++string; /* eat leading delimiters */
    if (!*string) {
      die(1, "Too few hosts in " ENV_PREFIX "SSH_SERVERS");
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

  if (nproc < nnodes) {
    fprintf(stderr, "Warning: %d nodes is larger than %d processes, nodes reduced to %d\n", nnodes, nproc, nproc);
    nnodes = nproc;
  }

  if ((env_string = my_getenv(ENV_PREFIX "SSH_NODEFILE")) != NULL) {
    nodelist = parse_nodefile(env_string);
  } else if ((env_string = my_getenv(ENV_PREFIX "SSH_SERVERS")) != NULL) {
    nodelist = parse_servers(env_string);
  } else if ((env_string = my_getenv("PBS_NODEFILE")) != NULL) {
    nodelist = parse_nodefile(env_string);
  } else if ((env_string = my_getenv("PE_HOSTFILE")) != NULL) {
    char *filename = sappendf(NULL, "%s/machines", gasneti_getenv_withdefault("TMPDIR",""));
    nodelist = parse_nodefile(filename);
    gasneti_free(filename);
  } else if ((env_string = my_getenv("SSS_HOSTLIST")) != NULL) {
    nodelist = parse_servers(env_string);
  } else if ((env_string = my_getenv("LSB_HOSTS")) != NULL) {
    nodelist = parse_servers(env_string);
  } else {
    die(1, "No " ENV_PREFIX "SSH_NODEFILE or " ENV_PREFIX "SSH_SERVERS in environment");
  }
}

static void send_nodelist(int s, int count, char ** list) {
  gasnet_node_t i;

  /* length of list is already known to the recipient */
  count = MAX(count, 1);
  for (i = 0; i < count; ++i) {
    do_write_string(s, list[i]);
  }
}

static void recv_nodelist(int s, int count) {
  gasnet_node_t i;
  count = MAX(count, 1);

  nodelist = gasneti_malloc(count * sizeof(char *));
  for (i = 0; i < count; ++i) {
    nodelist[i] = do_read_string(s);
  }
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

    gasneti_assert(is_master);

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
        memcpy(q, p, tmp);
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
  for (i = 0; i < ssh_argc; ++i) {
    ssh_argv[i] = do_read_string(s);
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
  argv = gasneti_calloc(argc+1, sizeof(char **));
  for (i = 0; i < argc; ++i) {
    argv[i] = do_read_string(s);
  }
  argv[argc] = NULL;

  *argc_p = argc;
  *argv_p = argv;
}

static void pre_spawn(int count) {
  struct sockaddr_in sock_addr;
  GASNET_SOCKLEN_T addr_len;
  char *env_string;

  /* Get the cwd */
  if ((env_string = my_getenv(ENV_PREFIX "SSH_REMOTE_PATH")) != NULL) {
    strncpy(cwd, env_string, sizeof(cwd));
  } else if (!getcwd(cwd, sizeof(cwd))) {
    gasneti_fatalerror("getcwd() failed");
  }

  /* Open /dev/null */
  devnull = open("/dev/null", O_RDWR);
  if (devnull < 0) {
    gasneti_fatalerror("open(/dev/null) failed");
  }
  (void)fcntl(devnull, F_SETFD, FD_CLOEXEC);

  /* Create listening socket */
  if ((listener = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    gasneti_fatalerror("listener = socket() failed");
  }
  (void)fcntl(listener, F_SETFD, FD_CLOEXEC);
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
    static const int one = 1;
    gasnet_node_t child_id;
    struct child *ch = NULL;
    int s;

    reaper(SIGCHLD); /* Take notice of children dying while blocked in accept() */
    if ((s = accept(listener, (struct sockaddr *)&sock_addr, &addr_len)) < 0) {
      gasneti_fatalerror("accept() failed w/ errno=%d", errno);
    }
    reaper(0); /* Disarm signal handler */

    (void)fcntl(s, F_SETFD, FD_CLOEXEC);
    (void)ioctl(s, SIOCSPGRP, &mypid); /* Enable SIGURG delivery on OOB data */
    (void)setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (char *) &one, sizeof(one));
    do_read(s, &child_id, sizeof(gasnet_node_t));
    gasneti_assert(child_id < children);
    ch = &(child[child_id]);
    child[child_id].sock = s;
    gasneti_assert(ch->rank < nproc);
    gasneti_assert(ch->procs > 0);
    gasneti_assert(ch->procs <= nproc);
    do_write(s, &ch->rank, sizeof(gasnet_node_t));
    do_write(s, &nproc, sizeof(gasnet_node_t));
    do_write(s, &ch->procs, sizeof(gasnet_node_t));
    do_write(s, &ch->nodes, sizeof(gasnet_node_t));
    send_env(s);
    send_nodelist(s, ch->nodes, ch->nodelist);
    send_ssh_argv(s);
    do_write_string(s, wrapper);
    send_argv(s, argc, argv);
    ++accepted;
  }

  /* Close listener */
  close(listener);

  if (!is_master) {
    close(devnull);

    /* Free the nodelist and ssh_argv */
    if (myproc != (gasnet_node_t)(-1L)) {
      gasnet_node_t i;
      int j;

      for (i = 0; i < tree_nodes; ++i) {
        gasneti_free(nodelist[i]);
      }
      gasneti_free(nodelist);

      for (j = 0; j < ssh_argc; ++j) {
        gasneti_free(ssh_argv[j]);
      }
      gasneti_free(ssh_argv);
    }
  }
}

static void do_connect(gasnet_node_t child_id, const char *parent_name, int parent_port, int *argc_p, char ***argv_p) {
  struct sockaddr_in sock_addr;
  GASNET_SOCKLEN_T addr_len;
  static const int one = 1;
  struct hostent *h = gethostbyname(parent_name);
  int rc, retry = 4;

  gasneti_assert(!is_master);

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
  (void)fcntl(parent, F_SETFD, FD_CLOEXEC);
  (void)ioctl(parent, SIOCSPGRP, &mypid); /* Enable SIGURG delivery on OOB data */
  (void)setsockopt(parent, IPPROTO_TCP, TCP_NODELAY, (char *) &one, sizeof(one));
  do_write(parent, &child_id, sizeof(gasnet_node_t));
  do_read(parent, &myproc, sizeof(gasnet_node_t));
  do_read(parent, &nproc, sizeof(gasnet_node_t));
  do_read(parent, &tree_procs, sizeof(gasnet_node_t));
  do_read(parent, &tree_nodes, sizeof(gasnet_node_t));
  gasneti_assert(nproc > 0);
  gasneti_assert(myproc < nproc);
  gasneti_assert(tree_procs > 0);
  gasneti_assert(tree_procs <= nproc);
  recv_env(parent);
  recv_nodelist(parent, tree_nodes);
  recv_ssh_argv(parent);
  wrapper = do_read_string(parent);
  recv_argv(parent, argc_p, argv_p);
  BOOTSTRAP_VERBOSE(("[%d] connected\n", myproc));
}

static void spawn_one(gasnet_node_t child_id, const char *myhost) {
  const char *host = child[child_id].nodelist ? child[child_id].nodelist[0] : nodelist[0];
  pid_t pid;
  int is_local = (GASNETI_BOOTSTRAP_LOCAL_SPAWN && (!host || !strcmp(host, myhost)));

  child[child_id].pid = pid = fork();

  if (pid < 0) {
    gasneti_fatalerror("fork() failed");
  } else if (pid == 0) {
    /* For all children except the root do </dev/null */
    if (child[child_id].rank != 0) {
      if (dup2(STDIN_FILENO, devnull) < 0) {
        gasneti_fatalerror("dup2(STDIN_FILENO, /dev/null) failed");
      }
    }
    if (is_local && !wrapper) {
      /* XXX: if we are clever enough, we might be able to "unwind" w/o the exec() */
      /* XXX: Should add wrapper support? */
      BOOTSTRAP_VERBOSE(("[%d] spawning process %d on %s via fork()\n",
			 (is_master ? -1 : (int)myproc),
			 (int)child[child_id].rank, myhost));
      execlp(argv0, argv0, "-GASNET-SPAWN-slave", "localhost",
	     sappendf(NULL, "%d", listen_port),
	     sappendf(NULL, "%d", (int)child_id),
	     is_verbose ? "-v" : NULL,
	     NULL);
      gasneti_fatalerror("execlp(sh) failed");
    } else {
      #if HAVE_PR_SET_PDEATHSIG
	/* If parent exits before us (an abnormal condition) then we exit too */
	(void)prctl(PR_SET_PDEATHSIG, SIGHUP);
      #endif
      BOOTSTRAP_VERBOSE(("[%d] spawning process %d on %s via %s\n",
			 (is_master ? -1 : (int)myproc),
			 (int)child[child_id].rank, host, ssh_argv[0]));
      ssh_argv[ssh_argc] = (/* noconst */ char *)host;
      ssh_argv[ssh_argc+1] = sappendf(NULL, "cd %s; exec %s %s -GASNET-SPAWN-slave %s %d %d%s",
				      quote_arg(cwd),
				      (wrapper ? wrapper : ""),
				      quote_arg(argv0),
				      myhost, listen_port, (int)child_id,
				      (is_verbose ? " -v" : ""));
      execvp(ssh_argv[0], ssh_argv);
      gasneti_fatalerror("execvp(ssh) failed");
    }
  }
  gasneti_atomic_increment(&live, 0);
}

static void do_spawn(int argc, char **argv, char *myhost) {
  int j;

  pre_spawn(children);
  for (j = 0; j < children; ++j) {
    spawn_one(j, myhost);
  }
  post_spawn(children, argc, argv);
}

static void usage(const char *argv0) {
  die(1, "usage: %s [-GASNET-SPAWN-master] [-v] NPROC[:NODES] [--] [ARGS...]", argv0);
}

static void do_kill(int argc, char **argv) GASNETI_NORETURN;
static void do_kill(int argc, char **argv) {
  int pid;
  int rc;
  int loops = 30;

  if ((argc != 3) || ((pid = atoi(argv[2])) < 1)) {
    _exit(-1);
  }

  /* First try the polite signal */
  rc = kill(pid, SIGTERM);

  /* Loop until process is gone (kill fails) or time runs out */
  while ((rc == 0) && (loops--)) {
    (void)sleep(1);
    rc = kill(pid,  0); /* just checks if it is still alive and ours */
  }

  /* Be forcefull if still around */
  if (rc == 0) { 
    (void)kill(pid, SIGKILL);
  }

  _exit(0);
}

extern int (*gasneti_verboseenv_fn)(void);

static void do_master(int argc, char **argv) GASNETI_NORETURN;
static void do_master(int argc, char **argv) {
  char myhost[1024];
  char *p;
  int argi=1;

  #if PLATFORM_OS_LINUX
  { /* Work around for bug 2136 by setting O_APPEND on stdout and stderr */
    int tmp;
    tmp = fcntl(STDOUT_FILENO, F_GETFL, 0);
    if (tmp >= 0) (void)fcntl(STDOUT_FILENO, F_SETFL, tmp | O_APPEND);
    tmp = fcntl(STDERR_FILENO, F_GETFL, 0);
    if (tmp >= 0) (void)fcntl(STDERR_FILENO, F_SETFL, tmp | O_APPEND);
  }
  #endif

  is_master = 1;
  gasneti_reghandler(SIGURG, &sigurg_handler);

  if ((argi < argc) && (strcmp(argv[argi], "-GASNET-SPAWN-master") == 0)) {
    argi++;
  }
  if ((argi < argc) && (strcmp(argv[argi], "-v") == 0)) {
    is_verbose = 1;
    argi++;
  }
  if ((argi < argc) && (strncmp(argv[argi], "-W", 2) == 0)) {
    wrapper = &argv[argi][2];
    argi++;
  }
  if (argi >= argc) usage(argv[0]); /* ran out of args */

  nproc = atoi(argv[argi]);
  if (nproc < 1) usage(argv[0]); /* bad argument */
  p = strchr(argv[argi], ':');
  if (p) {
    nnodes = atoi(p+1);
    if (nnodes < 1) usage(argv[0]); /* bad argument */
  } else {
    nnodes = nproc;
  }
  BOOTSTRAP_VERBOSE(("Spawning '%s': %d processes on %d nodes\n", argv[0], (int)nproc, (int)nnodes));
  argi++;

  if ((argi < argc) && (strcmp(argv[argi], "--") == 0)) {
    argi++;
  }

  argv[argi-1] = argv[0];
  argc -= argi-1;
  argv += argi-1;

  if (gethostname(myhost, sizeof(myhost)) < 0) {
    die(1, "gethostname() failed");
  }

  /* Enable VERBOSEENV */
  gasneti_verboseenv_fn = NULL;

  configure_ssh();
  build_nodelist();
  all_pids = gasneti_calloc(nproc, sizeof(pid_t));

  /* Arrange to forward termination signals */
  gasneti_reghandler(SIGQUIT, &sigforward);
  gasneti_reghandler(SIGINT,  &sigforward);
  gasneti_reghandler(SIGTERM, &sigforward);
  gasneti_reghandler(SIGHUP,  &sigforward);
  gasneti_reghandler(SIGPIPE, &sigforward);

  /* Configure child(ren) */
  #if GASNETI_BOOTSTRAP_FLAT_TREE
  {
    gasnet_node_t p_quot = nproc / nnodes;
    gasnet_node_t p_rem = nproc % nnodes;
    gasnet_node_t rank;
    int j;

    children = nproc;
    child = gasneti_calloc(children, sizeof(struct child));
    for (j = 0, rank = 0; j < nnodes; ++j) {
      gasnet_node_t i;
      for (i = p_quot + ((j<p_rem)?1:0); i != 0; --i, ++rank) {
        child[rank].rank = rank;
        child[rank].procs = 1;
        child[rank].nodes = 1;
        child[rank].nodelist = nodelist+j;
      }
    }
  }
  #else
    children = 1;
    child = gasneti_calloc(children, sizeof(struct child));
    child[0].rank = 0;
    child[0].procs = nproc;
    child[0].nodes = nnodes;
    child[0].nodelist = nodelist;
  #endif

  /* Start the process(es) */
  mypid = getpid();
  do_spawn(argc, argv, myhost);

  /* Locate all procs */
  gather_pids();

  /* Wait on the child(ren) */
#if GASNETI_BOOTSTRAP_FLAT_TREE
  { int done = 0;

    while (!done && !in_abort) {
      fd_set fds;
      char cmd;
      int i, rc, maxfd;

      /* Use select to find a fd w/ available work or EOF */
      FD_ZERO(&fds);
      maxfd = 0;
      for (i = 0; i < children; ++i) {
        FD_SET(child[i].sock, &fds);
	maxfd = MAX(maxfd, child[i].sock);
      }
      rc = select(maxfd+1, &fds, NULL, NULL, NULL);
      gasneti_assert(rc != 0);
      if (rc < 0) {
	if (errno == EINTR) {
	  continue;
	} else {
	  do_abort(-1);
	  break;
	}
      }

      /* Peek 1 command byte */
      for (i = 0; i < children; ++i) {
	if (FD_ISSET(child[i].sock, &fds)) {
	  break;
	}
      }
      gasneti_assert(i < children);
      rc = recv(child[i].sock, &cmd, sizeof(cmd), MSG_PEEK);
      if (rc != sizeof(cmd)) {
	/* do_read() hit EOF because a child has died */
	break;
      }

      switch (cmd) {
        case BOOTSTRAP_CMD_NO_OP:
          do_read(child[i].sock, &cmd, sizeof(cmd));
	  break;
        case BOOTSTRAP_CMD_FINI0:
	  gasneti_bootstrapFini_ssh();
	  done = 1;
	  break;
        case BOOTSTRAP_CMD_BARR0:
	  gasneti_bootstrapBarrier_ssh();
	  break;
        case BOOTSTRAP_CMD_BCAST:
	  gasneti_bootstrapBroadcast_ssh(NULL, 0, NULL, -1);
	  break;
        case BOOTSTRAP_CMD_EXCHG:
	  gasneti_bootstrapExchange_ssh(NULL, 0, NULL);
	  break;
        case BOOTSTRAP_CMD_TRANS:
	  gasneti_bootstrapAlltoall_ssh(NULL, 0, NULL);
	  break;
	    
        default:
	  fprintf(stderr, "Spawner protocol error\n");
	  do_abort(-1);
	  done = 1;
      }
    }
  }
#else
  {
    char cmd;
    ssize_t rc;
    do {
      rc = read(child[0].sock, &cmd, sizeof(cmd));
      if (rc == sizeof(cmd)) {
        gasneti_assert(cmd == BOOTSTRAP_CMD_FINI0);
        finalized = 1;
        cmd = BOOTSTRAP_CMD_FINI1;
        (void)write(child[0].sock, &cmd, sizeof(cmd));
      }
    } while ((rc < 0) && (errno == EINTR));
  }
#endif

  /* Wait for all children to terminate */
  wait_for_all();

  BOOTSTRAP_VERBOSE(("[-1] Exit with status %d\n", (int)(unsigned char)exit_status));
  exit (exit_status);
}

static void do_slave(int *argc_p, char ***argv_p, gasnet_node_t *nodes_p, gasnet_node_t *mynode_p)
{
  int argc = *argc_p;
  char **argv = *argv_p;
  gasnet_node_t child_id;
  const char *parent_name;
  int parent_port;

  is_master = 0;
  gasneti_reghandler(SIGURG, &sigurg_handler);

  #if HAVE_PR_SET_PDEATHSIG
    /* If parent exits before us (an abnormal condition) then trigger an abort */
    (void)prctl(PR_SET_PDEATHSIG, SIGURG);
  #endif

  if ((argc < 5) || (argc > 6)){
    gasneti_fatalerror("Invalid command line in slave process");
  }
  parent_name = argv[2];
  parent_port = atoi(argv[3]);
  child_id = atoi(argv[4]);
  if (argc == 6) {
    gasneti_assert(!strcmp("-v",argv[5]));
    is_verbose = 1;
  }

  mypid = getpid();

  /* Connect w/ parent to find out who we are */
  do_connect(child_id, parent_name, parent_port, argc_p, argv_p);

  /* Start any children */
  if (tree_procs > 1) {
    gasnet_node_t p_quot, p_rem; /* quotient and remainder of nproc/nodes */
    gasnet_node_t n_quot, n_rem; /* quotient and remainder of nodes/GASNETI_BOOTSTRAP_OUT_DEGREE */
    gasnet_node_t local_procs; /* the local processes (proc-per-node), excluding self */
    gasnet_node_t rank, j;
    char **sublist;

    p_quot = tree_procs / tree_nodes;
    p_rem = tree_procs % tree_nodes;

    local_procs = p_quot + (p_rem?1:0) - 1;
    p_rem -= (p_rem?1:0);

    /* Children = (local_procs other than self) + (child nodes) */
    children = local_procs + MIN(GASNETI_BOOTSTRAP_OUT_DEGREE, (tree_nodes - 1));
    child = gasneti_calloc(children, sizeof(struct child));
    rank = myproc + 1;

    /* Map out the local processes */
    for (j = 0; j < local_procs; ++j) {
	child[j].rank = rank++;
	child[j].procs = 1;
	child[j].nodes = 0; /* N/A */
        child[j].nodelist = nodelist;
    }

    /* Map out the child nodes */
    n_quot = (tree_nodes - 1) / GASNETI_BOOTSTRAP_OUT_DEGREE;
    n_rem = (tree_nodes - 1) % GASNETI_BOOTSTRAP_OUT_DEGREE;
    sublist = nodelist + 1;
    for (j = local_procs; rank < (myproc + tree_procs); j++) {
      gasnet_node_t nodes = n_quot + (n_rem?1:0);
      gasnet_node_t procs = (nodes * p_quot) + MIN(p_rem, nodes);
      n_rem -= (n_rem?1:0);
      p_rem -= MIN(p_rem, nodes);

      child[j].rank = rank;
      child[j].procs = procs;
      child[j].nodes = nodes;
      child[j].nodelist = sublist;
      sublist += nodes;
      rank += procs;
    }
  
    /* Prepare to reap children */
    gasneti_reghandler(SIGCHLD, &reaper);

    /* Spawn them */
    do_spawn(*argc_p, *argv_p, nodelist[0]);
  }

  gather_pids();

  *nodes_p = nproc;
  *mynode_p = myproc;
  gasneti_conduit_getenv = &do_getenv;
}

#if !GASNETI_BOOTSTRAP_FLAT_TREE
/* dest is >= len*tree_procs, used as temp space on all but root */
static void do_gath0(void *src, size_t len, void *dest)
{
  int j;
  void *tmp;

  memcpy(dest, src, len);
  tmp = (void *)((uintptr_t)dest + len);
  for (j = 0; j < children; ++j) {
    gasnet_node_t procs = child[j].procs;
    do_read(child[j].sock, tmp, len*procs);
    tmp = (void *)((uintptr_t)tmp + len*procs);
  }
  if (myproc) {
    do_write(parent, dest, len * tree_procs);
  }
}

/* src is >= len*tree_procs, used as temp space on all but root */
static void do_scat0(void *src, size_t len, void *dest)
{
  int j;
  void *tmp;

  if (myproc) {
    do_read(parent, src, len * tree_procs);
  }
  memcpy(dest, src, len);
  tmp = (void *)((uintptr_t)src + len);
  for (j = 0; j < children; ++j) {
    gasnet_node_t procs = child[j].procs;
    do_write(child[j].sock, tmp, len*procs);
    tmp = (void *)((uintptr_t)tmp + len*procs);
  }
}

static void do_bcast0(size_t len, void *dest) {
  int j;

  if (myproc) {
    do_read(parent, dest, len);
  }
  for (j = 0; j < children; ++j) {
    do_write(child[j].sock, dest, len);
  }
}
#endif

static void gather_pids(void) {
#if GASNETI_BOOTSTRAP_FLAT_TREE
  if (is_master) {
    int j;
    for (j = 0; j < children; ++j) {
      do_read(child[j].sock, &all_pids[j], sizeof(pid_t));
    }
  } else {
    do_write(parent, &mypid, sizeof(pid_t));
  }
#else
  if (is_master) {
    do_read(child[0].sock, all_pids, sizeof(pid_t) * nproc);
  } else {
    pid_t *pids = gasneti_malloc(sizeof(pid_t) * tree_procs);

    /* Collect rows from our subtree (gather) */
    do_gath0(&mypid, sizeof(pid_t), pids);

    if (!myproc) {
      do_write(parent, pids, sizeof(pid_t) * nproc);
    }

    gasneti_free(pids);
  }
#endif
}

/*----------------------------------------------------------------------------------------------*/

/* gasneti_bootstrapInit
 *
 * Upon return:
 *   + argc and argv are those the user specified
 *   + *nodes_p and *mynode_p are set
 *   + the global environment is available via gasneti_getenv()
 * There is no barrier at the end, so it is possible that in a multi-level
 * tree, there are still some processes not yet spawned.  This is OK, since
 * we assume that at least one gasneti_bootstrap*() collectives will follow.
 * Not waiting here allows any subsequent that first collective to overlap
 * with the spawning.
 */
void gasneti_bootstrapInit_ssh(int *argc_p, char ***argv_p, gasnet_node_t *nodes_p, gasnet_node_t *mynode_p) {
  int argc = *argc_p;
  char **argv = *argv_p;

  if (argc < 2) {
    usage(argv[0]);
  }

  argv0 = argv[0];

  if (strcmp(argv[1], "-GASNET-SPAWN-slave") == 0) {
    do_slave(argc_p, argv_p, nodes_p, mynode_p);
  } else if (strcmp(argv[1], "-GASNET-SPAWN-kill") == 0) {
    do_kill(argc, argv); /* Does not return */
  } else {
    do_master(argc, argv); /* Does not return */
  }
}

/* gasneti_bootstrapFini
 */
void gasneti_bootstrapFini_ssh(void) {
  char cmd;
  int j;

#if GASNETI_BOOTSTRAP_FLAT_TREE
  if (is_master) {
    for (j = 0; j < children; ++j) {
      do_read(child[j].sock, &cmd, sizeof(cmd));
      if (cmd != BOOTSTRAP_CMD_FINI0) return;
    }
    if (in_abort) return;
    finalized = 1;
    cmd = BOOTSTRAP_CMD_FINI1;
    for (j = 0; j < children; ++j) {
      do_write(child[j].sock, &cmd, sizeof(cmd));
    }
  } else {
    cmd = BOOTSTRAP_CMD_FINI0;
    do_write(parent, &cmd, sizeof(cmd));
    do_read(parent, &cmd, sizeof(cmd));
    gasneti_assert(cmd == BOOTSTRAP_CMD_FINI1);
  }
#else
  gasneti_assert(!is_master);
  for (j = 0; j < children; ++j) {
    do_read(child[j].sock, &cmd, sizeof(cmd));
    if (cmd != BOOTSTRAP_CMD_FINI0) return;
  }
  finalized = 1;
  cmd = BOOTSTRAP_CMD_FINI0;
  do_write(parent, &cmd, sizeof(cmd));
  if (!myproc) {
    do_read(parent, &cmd, sizeof(cmd));
    gasneti_assert(cmd == BOOTSTRAP_CMD_FINI1);
  }
  do_bcast0(sizeof(cmd), &cmd);
  gasneti_assert(cmd == BOOTSTRAP_CMD_FINI1);

  /* Close our control sockets */
  if (child) {
    for (j = 0; j < children; ++j) {
      (void)close(child[j].sock);
    }
  }
  (void)close(parent);

#if GASNET_DEBUG
  /* Wait for all children to exit */
  wait_for_all();
#endif
#endif
}

/* gasneti_bootstrapAbort
 *
 * Force immediate (abnormal) termination.
 */
void gasneti_bootstrapAbort_ssh(int exitcode) {
  gasneti_assert(!is_master);
  do_abort((unsigned char)exitcode);
  gasneti_fatalerror("do_abort failed.");
  /* NOT REACHED */
}

void gasneti_bootstrapBarrier_ssh(void) {
  char cmd;
  int j;

#if GASNETI_BOOTSTRAP_FLAT_TREE
  if (is_master) {
    for (j = 0; j < children; ++j) {
      do_read(child[j].sock, &cmd, sizeof(cmd));
      if_pf (cmd == BOOTSTRAP_CMD_FINI0) {
	/* looks like an exit between gasnet_init() and gasnet_attach() */
	do_abort(255);
	break;
      }
      gasneti_assert(cmd == BOOTSTRAP_CMD_BARR0 || in_abort);
    }
    if (in_abort) return;
    cmd = BOOTSTRAP_CMD_BARR1;
    for (j = 0; j < children; ++j) {
      do_write(child[j].sock, &cmd, sizeof(cmd));
    }
  } else {
    cmd = BOOTSTRAP_CMD_BARR0;
    do_write(parent, &cmd, sizeof(cmd));
    do_read(parent, &cmd, sizeof(cmd));
    gasneti_assert(cmd == BOOTSTRAP_CMD_BARR1);
  }
#else
  gasneti_assert(!is_master);
  /* UP */
  for (j = 0; j < children; ++j) {
    do_read(child[j].sock, &cmd, sizeof(cmd));
    gasneti_assert(cmd == BOOTSTRAP_CMD_BARR0);
  }
  if (myproc) {
    cmd = BOOTSTRAP_CMD_BARR0;
    do_write(parent, &cmd, sizeof(cmd));
  }

  /* DOWN */
  cmd = BOOTSTRAP_CMD_BARR1;
  do_bcast0(sizeof(cmd), &cmd);
  gasneti_assert(cmd == BOOTSTRAP_CMD_BARR1);
#endif
}

void gasneti_bootstrapExchange_ssh(void *src, size_t len, void *dest) {
  int j;

#if GASNETI_BOOTSTRAP_FLAT_TREE
  if (is_master) {
    char cmd, *tmp, *p;
    for (j = 0; j < children; ++j) {
      do_read(child[j].sock, &cmd, sizeof(cmd));
      if_pf (cmd == BOOTSTRAP_CMD_FINI0) {
	/* looks like an exit between gasnet_init() and gasnet_attach() */
	do_abort(255);
	break;
      }
      gasneti_assert(cmd == BOOTSTRAP_CMD_EXCHG || in_abort);
      do_read(child[j].sock, &len, sizeof(len));
    }
    if (in_abort) return;
    tmp = gasneti_malloc(len*nproc);
    for (j = 0, p = tmp; j < children; ++j, p += len) {
      do_read(child[j].sock, p, len);
    }
    for (j = 0; j < children; ++j) {
      do_write(child[j].sock, tmp, len*nproc);
    }
    gasneti_free(tmp);
  } else {
    char cmd = BOOTSTRAP_CMD_EXCHG;
    do_write(parent, &cmd, sizeof(cmd));
    do_write(parent, &len, sizeof(len));
    do_write(parent, src, len);
    do_read(parent, dest, len*nproc);
  }
#else
  gasneti_assert(!is_master);
  /* Gather data up the tree, assembling partial results in-place in dest */
  do_gath0(src, len, (void *)((uintptr_t)dest + len*myproc));

  /* Move data down, reducing traffic by sending
     only parts that a given node did not send to us */
  if (myproc) {
    gasnet_node_t next = myproc + tree_procs;
    do_read(parent, dest, len*myproc);
    do_read(parent, (void *)((uintptr_t)dest + len*next), len*(nproc - next));
  }
  for (j = 0; j < children; ++j) {
    gasnet_node_t next = child[j].rank + child[j].procs;
    do_write(child[j].sock, dest, len*child[j].rank);
    do_write(child[j].sock, (void *)((uintptr_t)dest + len*next), len*(nproc - next));
  }
#endif
}

void gasneti_bootstrapAlltoall_ssh(void *src, size_t len, void *dest) {
#if GASNETI_BOOTSTRAP_FLAT_TREE
  if (is_master) {
    char cmd, *tmp, *tmp2, *p, *q;
    gasnet_node_t j;
    size_t row_len;
    for (j = 0; j < children; ++j) {
      do_read(child[j].sock, &cmd, sizeof(cmd));
      if_pf (cmd == BOOTSTRAP_CMD_FINI0) {
	/* looks like an exit between gasnet_init() and gasnet_attach() */
	do_abort(255);
	break;
      }
      gasneti_assert(cmd == BOOTSTRAP_CMD_TRANS || in_abort);
      do_read(child[j].sock, &len, sizeof(len));
    }
    if (in_abort) return;
    row_len = len * nproc;
    tmp = gasneti_malloc(row_len*nproc);
    for (j = 0, p = tmp; j < children; ++j, p += row_len) {
      do_read(child[j].sock, p, row_len);
    }
    tmp2 = gasneti_malloc(row_len);
    for (j = 0; j < nproc; ++j) {
      gasnet_node_t k;
      for (k = 0; k < j; ++k) {
	p = tmp + (j*row_len + k*len);
	q = tmp + (k*row_len + j*len);
        memcpy(tmp2, p, len);
        memcpy(p, q, len);
        memcpy(q, tmp2, len);
      }
    }
    gasneti_free(tmp2);
    for (j = 0, p = tmp; j < children; ++j, p += row_len) {
      do_write(child[j].sock, p, row_len);
    }
    gasneti_free(tmp);
  } else {
    char cmd = BOOTSTRAP_CMD_TRANS;
    do_write(parent, &cmd, sizeof(cmd));
    do_write(parent, &len, sizeof(len));
    do_write(parent, src, len*nproc);
    do_read(parent, dest, len*nproc);
  }
#else
  size_t row_len = len * nproc;
  char *tmp;
                                                                                                              
  gasneti_assert(!is_master);
  tmp = gasneti_malloc(row_len * tree_procs);

  /* Collect rows from our subtree (gather) */
  do_gath0(src, row_len, tmp);

  /* Transpose at root, using dest for free temporary space */
  if (!myproc) {
    gasnet_node_t i;
    for (i = 0; i < nproc; ++i) {
      gasnet_node_t k;
      for (k = 0; k < i; ++k) {
	void *p = tmp + (i*row_len + k*len);
	void *q = tmp + (k*row_len + i*len);
        memcpy(dest, p, len);
        memcpy(p, q, len);
        memcpy(q, dest, len);
      }
    }
  }

  /* Move data back down (scatter) */
  do_scat0(tmp, row_len, dest);

  gasneti_free(tmp);
#endif
}

void gasneti_bootstrapBroadcast_ssh(void *src, size_t len, void *dest, int rootnode) {
  int j;

#if GASNETI_BOOTSTRAP_FLAT_TREE
  if (is_master) {
    char cmd, *tmp;
    for (j = 0; j < children; ++j) {
      do_read(child[j].sock, &cmd, sizeof(cmd));
      if_pf (cmd == BOOTSTRAP_CMD_FINI0) {
	/* looks like an exit between gasnet_init() and gasnet_attach() */
	do_abort(255);
	break;
      }
      gasneti_assert(cmd == BOOTSTRAP_CMD_BCAST || in_abort);
      do_read(child[j].sock, &len, sizeof(len));
      do_read(child[j].sock, &rootnode, sizeof(rootnode));
    }
    if (in_abort) return;
    tmp = gasneti_malloc(len);
    do_read(child[rootnode].sock, tmp, len);
    for (j = 0; j < children; ++j) {
      do_write(child[j].sock, tmp, len);
    }
    gasneti_free(tmp);
  } else {
    char cmd = BOOTSTRAP_CMD_BCAST;
    do_write(parent, &cmd, sizeof(cmd));
    do_write(parent, &len, sizeof(len));
    do_write(parent, &rootnode, sizeof(rootnode));
    if (myproc == rootnode) {
      do_write(parent, src, len);
    }
    do_read(parent, dest, len);
  }
#else
  gasneti_assert(!is_master);
  /* Move up the tree to proc 0 */
  if (rootnode != 0) {
    if (rootnode == myproc) {
      do_write(parent, src, len);
    } else if ((rootnode > myproc) && (rootnode < (myproc + tree_procs))) {
      /* Forward from child to parent */
      for (j = 0; (rootnode >= child[j].rank + child[j].procs); ++j) {
	/* searching for proper child */
      }
      do_read(child[j].sock, dest, len);
      if (myproc) {
        do_write(parent, dest, len);
      }
    }
  } else if (!myproc) {
    memcpy(dest, src, len);
  }

  /* Now move it down */
  do_bcast0(len, dest);
#endif
}
