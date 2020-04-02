/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amudp/amudp_spmd.cpp $
 * Description: AMUDP Implementations of SPMD operations (bootstrapping and parallel job control)
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#undef _PORTABLE_PLATFORM_H
#include <amx_portable_platform.h>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#if (PLATFORM_OS_LINUX || PLATFORM_OS_UCLINUX) && !defined(__USE_GNU)
  /* some Linuxes need this to pull in F_SETSIG */
  #define __USE_GNU
  #include <fcntl.h>
  #undef __USE_GNU
#else
  #include <fcntl.h>
#endif

#ifdef AMUDP_BLCR_ENABLED
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
  #include "libcr.h"
#endif

extern char **environ; 

#include <amudp_spmd.h>

#include "sockutil.h"
#include "socklist.h"
#include "sig.h"

#include "amudp_internal.h" // must come after any other headers

#define FD_STDIN 0
#define FD_STDOUT 1
#define FD_STDERR 2

#ifndef FREEZE_SLAVE
#define FREEZE_SLAVE  0
#endif

#if AMX_DEBUG_VERBOSE
  #define DEBUG_SLAVE(msg)  do {                             \
     if (AMX_ProcessLabel) AMX_Info("%s", msg);              \
     else AMX_Info("slave %i: %s", AMUDP_SPMDMYPROC, msg);   \
  } while (0)
  #define DEBUG_MASTER(msg) AMX_Info("master: %s", msg)
#else
  #define DEBUG_SLAVE(msg)  ((void)0)
  #define DEBUG_MASTER(msg) ((void)0)
#endif

#define AMUDP_SPMDSLAVE_ARGS "AMUDP_SLAVE_ARGS"

static int AMUDP_SPMDShutdown(int exitcode);

/* master only */
  static SOCKET AMUDP_SPMDListenSocket = INVALID_SOCKET; /* TCP bootstrapping listener */
  static SOCKET AMUDP_SPMDStdListenSocket[3];
  static SOCKET *AMUDP_SPMDSlaveSocket = NULL; /* table of TCP control sockets */
  static en_t *AMUDP_SPMDTranslation_name = NULL; 
  static tag_t *AMUDP_SPMDTranslation_tag = NULL; /* network byte order */
  int AMUDP_SPMDSpawnRunning = FALSE; /* true while spawn is active */
  int AMUDP_SPMDRedirectStdsockets; /* true if stdin/stdout/stderr should be redirected */

/* slave only */
  SOCKET AMUDP_SPMDControlSocket = INVALID_SOCKET; 
  static ep_t AMUDP_SPMDEndpoint = NULL;
  static eb_t AMUDP_SPMDBundle = NULL;
  static en_t AMUDP_SPMDName = {0};
  volatile int AMUDP_SPMDIsActiveControlSocket = 0; 
  static SOCKET newstd[3] = { INVALID_SOCKET, INVALID_SOCKET, INVALID_SOCKET };
  static int AMUDP_SPMDMYPROC = AMUDP_PROCID_NEXT; /* -1 requests next avail procid */
  static volatile int AMUDP_SPMDBarrierDone = 0; /* flag barrier as complete */
  static volatile int AMUDP_SPMDGatherDone = 0;  /* flag gather as complete */
  static volatile int AMUDP_SPMDGatherLen = 0;
  static void * volatile AMUDP_SPMDGatherData = NULL;
  int AMUDP_SPMDwakeupOnControlActivity = 0;
  int AMUDP_FailoverAcksOutstanding = 0;


/* master & slave */
  static int AMUDP_SPMDStartupCalled = 0;
  static int AMUDP_SPMDNUMPROCS = -1;
  static char *AMUDP_SPMDMasterEnvironment = NULL;

#ifdef AMUDP_BLCR_ENABLED
/* checkpoint/restart */
  int AMUDP_SPMDRestartActive = 0;
  static int AMUDP_SPMDNetworkDepth = 0;
#else
  #define AMUDP_SPMDRestartActive 0
#endif

// used to pass info - always stored in network byte order
// fields carefully ordered by size to avoid cross-platform struct packing differences
typedef struct {
  uint64_t networkpid;  // globally unique pid

  tag_t tag;            // tag for this processor
  
  int32_t procid;       // id for this processor
  int32_t numprocs;     // num procs in job

  int32_t depth;        // network depth
  uint32_t environtablesz; // size of environment table we're about to send

  uint16_t stdMaster[3]; // address of std listeners
  uint16_t _pad1; // ensure platform-independent table size

} AMUDP_SPMDBootstrapInfo_t;

/*
  Protocol for TCP bootstrapping/control sockets
  initialization: 
    slave->master (int32) - send my procid for init
    slave->master (en_t) - send my endpoint name for init
   if received procid == AMUDP_PROCID_ALLOC
    master->slave (int32 next_rank++)
   else
    master->slave (int32 sizeof(AMUDP_SPMDBootstrapInfo_t))
    master->slave (AMUDP_SPMDBootstrapInfo_t) 
    master->slave (AMUDP_SPMDTranslation_name (variable size)) 
    master->slave (AMUDP_SPMDTranslation_tag (variable size)) 
    master->slave (AMUDP_SPMDMasterEnvironment (variable size)) 

  master->slave messages
    "E"(int32 exitcode) - die now with this exit code
    "F"(int32 i)(old en_t)(new en_t) - slave i's NIC just failed over to new en_t
    "A"(int32 i) - (to slave i) slave acknowledged fail-over of slave i's NIC
    "B" - barrier complete
    "G"(int32 perproclen)(data) - end an AllGather, here's the result

  slave->master messages
    "E"(int32 exitcode) - exit with this code
    "F"(int32 i)(old en_t)(new en_t) - slave i's NIC just failed over to new en_t
    "A"(int32 i) - acknowledge fail-over of slave i's NIC
    "B" - enter barrier
    "G"(int32 i)(int32 perproclen)(data) - slave i begin an AllGather, here's the length and my data
*/
/* ------------------------------------------------------------------------------------ 
 *  misc helpers
 * ------------------------------------------------------------------------------------ */
static void flushStreams(const char *context) {
  if (!context) context = "flushStreams()";

  if (fflush(NULL)) { /* passing NULL to fflush causes it to flush all open FILE streams */
    perror("fflush");
    AMX_FatalErr("failed to fflush(NULL) in %s", context); 
  }
  if (fflush(stdout)) {
    perror("fflush");
    AMX_FatalErr("failed to flush stdout in %s", context); 
  }
  if (fflush(stderr)) {
    perror("fflush");
    AMX_FatalErr("failed to flush stderr in %s", context); 
  }
  fsync(FD_STDOUT); /* ignore errors for output is a console */
  fsync(FD_STDERR); /* ignore errors for output is a console */

  static int do_sync = -1;
  if (do_sync < 0) {
    /* Approximate match to GASNet's acceptance of 'Y|YES|y|yes|1' */
    char *envval = AMUDP_getenv_prefixed_withdefault("FS_SYNC", "NO");
    char c = envval[0];
    do_sync = ((c == '1') || (c == 'y') || (c == 'Y'));
  }
  if (do_sync) {
    sync();
  }
  AMX_sched_yield();
}
//------------------------------------------------------------------------------------
extern char *AMUDP_enStr(en_t en, char *buf) {
  static char pbuf[80];
  if (!buf) buf = pbuf;
  SockAddr tmp((sockaddr*)&en);
  sprintf(buf, "(%s:%i)", tmp.IPStr(), tmp.port());
  return buf;
}
extern char *AMUDP_tagStr(tag_t tag, char *buf) {
  static char pbuf[80];
  if (!buf) buf = pbuf;
  sprintf(buf, "0x%08x%08x", 
    (int)(uint32_t)(tag >> 32), 
    (int)(uint32_t)(tag & 0xFFFFFFFF));
  return buf;
}
//------------------------------------------------------------------------------------
static void setupStdSocket(SOCKET& ls, SocketList& list, SocketList& allList) {
  if ((int)list.getCount() < AMUDP_SPMDNUMPROCS) {
    SockAddr remoteAddr;
    SOCKET newsock = accept_socket(ls, remoteAddr);
    list.insert(newsock);
    allList.insert(newsock);
    if ((int)list.getCount() == AMUDP_SPMDNUMPROCS) {
      // close listener
      close_socket(ls);
      allList.remove(ls);
      ls = INVALID_SOCKET;
    }
  } else AMX_Err("master detected some unrecognized activity on a std listener");
}
//------------------------------------------------------------------------------------
static void handleStdOutput(FILE *fd, fd_set *psockset, SocketList& list, SocketList& allList, int nproc) {
  static size_t bufsz;
  static uint8_t *sbuf;
  static struct S_linebuf {
    size_t  len;
    uint8_t *buf;
  } *linebuf;
  static size_t linebufcnt;
  static SOCKET *tempSockArr;
  #ifndef AMUDP_STD_BUFSZ
  #define AMUDP_STD_BUFSZ 1024  // recv buffer size, should be large enough for decent bandwidth on multi-line chunks
  #endif
  #ifndef AMUDP_MAX_LINEBUFSZ
  #define AMUDP_MAX_LINEBUFSZ 1024*1024  // sanity limit for environment knob
  #endif

  if (!tempSockArr) { // first call, setup data structures
    tempSockArr = (SOCKET *)AMX_malloc(sizeof(SOCKET)*nproc);
    bufsz = atoi( AMUDP_getenv_prefixed_withdefault("LINEBUFFERSZ", AMX_STRINGIFY(AMUDP_STD_BUFSZ)) );
    if (bufsz == 0) { // line buffering disabled, use a static buffer
      bufsz = AMUDP_STD_BUFSZ;
      sbuf = (uint8_t *)AMX_malloc(bufsz);
    } else if (bufsz > AMUDP_MAX_LINEBUFSZ) {
      bufsz = AMUDP_MAX_LINEBUFSZ; 
    }
  }

  int numset;
  if ((numset = list.getIntersection(psockset, tempSockArr, nproc))) { // we have some active std sockets
    for (int i=0; i < numset; i++) {
      SOCKET s = tempSockArr[i];
      AMX_assert(FD_ISSET(s, psockset));
      ssize_t rsz = SOCKET_ERROR;
      if (sbuf) { // static buffering
        rsz = recv(s, sbuf, bufsz, 0);
        if (rsz > 0) { // other cases handled below
          AMX_assert(rsz <= (ssize_t)bufsz);
          fwrite(sbuf, 1, rsz, fd);
          fflush(fd);
          continue;
        }
      } else { // line buffering
        if ((size_t)s >= linebufcnt) { // grow directory
          void *newdir = AMX_calloc((size_t)s+1,sizeof(struct S_linebuf));
          if (linebufcnt > 0) {
            memcpy(newdir, linebuf, linebufcnt*sizeof(struct S_linebuf));
            AMX_free(linebuf);
          }
          linebuf = (struct S_linebuf*)newdir;
          linebufcnt = (size_t)s+1;
        }
        struct S_linebuf * const e = &linebuf[s];
        if (!e->buf) { // first use
          e->buf = (uint8_t *)AMX_malloc(bufsz);
        }
        AMX_assert(e->len < bufsz);
        rsz = recv(s, e->buf+e->len, bufsz-e->len, 0);
        if (rsz == 0) { // socket closed
          if (e->len) { // drain buffer
            fwrite(e->buf, 1, e->len, fd);
            fflush(fd);
            e->len = 0; 
          }
          // close handled below
        } else if (rsz > 0) {
          e->len += rsz;
          AMX_assert(e->len <= bufsz);
          size_t len = e->len;
          uint8_t *bol = e->buf;
          int wrote = 0;
          // could use memrchr here if it ever makes it into the standard..
          for (uint8_t *eol = bol+len-1; eol >= bol; eol--) {
            if (*eol == '\n') {
              size_t llen = eol-bol+1;
              AMX_assert(llen <= len);
              fwrite(bol, 1, llen, fd);
              wrote = 1;
              len -= llen;
              bol = eol+1;
              break;
            }
          } 
          if (len == bufsz) { // full buffer with no breaks, drain it
            fwrite(e->buf, 1, bufsz, fd);
            wrote = 1;
            e->len = 0;
          } else { // save partial-line tail
            if (len > 0) memmove(e->buf, bol, len);
            e->len = len;
          }
          if (wrote) fflush(fd);
          continue;
        }
      } // line buffering

      // handle recv errors
      if (rsz == SOCKET_ERROR) {
        DEBUG_MASTER("recv error in handleStdOutput, closing.");
        close_socket(s);
      } else if (rsz == 0) { // socket closed
        DEBUG_MASTER("dropping a std output socket...");
        close_socket(s);
        list.remove(s);
        allList.remove(s);
      }
    }
  }
}
//------------------------------------------------------------------------------------
#if USE_ASYNC_TCP_CONTROL
  extern "C" void AMUDP_SPMDControlSocketCallback(int sig) {
    AMUDP_SPMDIsActiveControlSocket = TRUE;
    AMX_VERBOSE_INFO(("got an AMUDP_SIGIO signal"));
    reghandler(AMUDP_SIGIO, AMUDP_SPMDControlSocketCallback);
  }
#endif
/* ------------------------------------------------------------------------------------ 
 *  basic inquiries
 * ------------------------------------------------------------------------------------ */
extern int AMUDP_SPMDNumProcs() {
  if (!AMUDP_SPMDStartupCalled) {
    AMX_Err("called AMUDP_SPMDNumProcs before AMUDP_SPMDStartup()");
    return -1;
  }
  AMX_assert(AMUDP_SPMDNUMPROCS >= 1);
  return AMUDP_SPMDNUMPROCS;
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_SPMDMyProc() {
  if (!AMUDP_SPMDStartupCalled) {
    AMX_Err("called AMUDP_SPMDMyProc before AMUDP_SPMDStartup()");
    return -1;
  }
  AMX_assert(AMUDP_SPMDMYPROC >= 0);
  return AMUDP_SPMDMYPROC;
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_SPMDIsWorker(char **argv) {
  if (AMUDP_SPMDStartupCalled) return 1; 
  else {
    const char *env_val = getenv(AMUDP_SPMDSLAVE_ARGS);
    return (env_val && (0 != atoi(env_val)));
  }
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_SPMDStartup(int *argc, char ***argv,
                             int nproc, int networkdepth, 
                             amudp_spawnfn_t spawnfn,
                             uint64_t *networkpid,
                             eb_t *eb, ep_t *ep) {
  
  if (AMUDP_SPMDStartupCalled) AMX_RETURN_ERR(RESOURCE);

  char *linebuf = AMUDP_getenv_prefixed("LINEBUFFERSZ");
  if (!linebuf || atoi(linebuf) > 0) { // ensure we line-buffer early output
    setvbuf(stdout, NULL, _IOLBF, BUFSIZ);
    setvbuf(stderr, NULL, _IOLBF, BUFSIZ);
  }

  /* we need a separate socklibinit for master 
     and to prevent AM_Terminate from murdering all our control sockets */
  if (!socklibinit()) AMX_RETURN_ERRFR(RESOURCE, AMUDP_SPMDStartup, "socklibinit() failed");

  const char *env_var = getenv(AMUDP_SPMDSLAVE_ARGS);
  const int slave_flag = env_var ? atoi(env_var) : 0;

  /* ------------------------------------------------------------------------------------ 
   *  I'm a master 
   * ------------------------------------------------------------------------------------ */
  if (! slave_flag) {
    int usingdefaultdegree = 0;
    uint64_t npid;
    if (nproc < 0 || nproc > (int)AMUDP_MAX_SPMDPROCS) AMX_RETURN_ERR(BAD_ARG);

    if (!argc || !argv) AMX_RETURN_ERR(BAD_ARG);

    #if AMX_DEBUG_VERBOSE
      AMX_SilentMode = 0;
    #else
      AMX_SilentMode = !AMUDP_getenv_prefixed("VERBOSEENV");
    #endif

    /* defaulting */
    if (networkdepth < 0) AMX_RETURN_ERR(BAD_ARG);
    if (networkdepth == 0) {
      networkdepth = atoi(
        AMUDP_getenv_prefixed_withdefault("NETWORKDEPTH", AMX_STRINGIFY(AMUDP_DEFAULT_NETWORKDEPTH)));
      if (networkdepth <= 0) networkdepth = AMUDP_DEFAULT_NETWORKDEPTH;
    }
    if (networkdepth > AMUDP_MAX_NETWORKDEPTH) { // provide useful error message
      AMX_FatalErr("NETWORKDEPTH must be <= %d", AMUDP_MAX_NETWORKDEPTH);
    }

    if (nproc == 0) { /* default to read from args */
      if (*argc > 1) nproc = atoi((*argv)[1]);
      if (nproc < 1) {
        fprintf(stderr, 
          "AMUDP SPMD Runtime Layer v%s, Copyright 2001, Dan Bonachea\n"
          "This program requires you specify the parallel degree\n"
          "as the first argument to %s\n" 
          , AMUDP_LIBRARY_VERSION_STR, (*argv)[0]);
        exit(1);
        AMX_RETURN_ERR(BAD_ARG);
      }
      
      usingdefaultdegree = 1;

      /* readjust params */
      (*argv)[1] = (*argv)[0];
      (*argv)++;
      (*argc)--;
    }

    AMUDP_SPMDNUMPROCS = nproc;

    { /* check job size */
      int maxtranslations = 0;
      int temp = AM_MaxNumTranslations(&maxtranslations);
      if (temp != AM_OK) {
        AMX_Err("Failed to AM_MaxNumTranslations() in AMUDP_SPMDStartup");
        AMX_RETURN(temp);
      } else if (AMUDP_SPMDNUMPROCS > maxtranslations) {
        AMX_Err("Too many nodes: AM_MaxNumTranslations (%d) less than number of requested nodes (%d)",
                maxtranslations, AMUDP_SPMDNUMPROCS);
        AMX_RETURN_ERR(RESOURCE);
      }
    }

    if (argv && !spawnfn && *argc > 1 && strlen((*argv)[1]) == 1) {
      for (int i=0; AMUDP_Spawnfn_Desc[i].abbrev; i++) {
        if (toupper((*argv)[1][0]) == toupper(AMUDP_Spawnfn_Desc[i].abbrev)) {
          spawnfn = AMUDP_Spawnfn_Desc[i].fnptr;
          break;
        }
      }
      if (spawnfn) {
        /* readjust params */
        (*argv)[1] = (*argv)[0];
        (*argv)++;
        (*argc)--;
      }
    }
    if (!spawnfn) {
      const char *spawnfn_str = AMUDP_getenv_prefixed_withdefault("SPAWNFN","S");
      if (spawnfn_str) {
        for (int i=0; AMUDP_Spawnfn_Desc[i].abbrev; i++) {
          if (toupper(spawnfn_str[0]) == toupper(AMUDP_Spawnfn_Desc[i].abbrev)) {
            spawnfn = AMUDP_Spawnfn_Desc[i].fnptr;
            break;
          }
        }
      }
    }
    if (!spawnfn) {
      fprintf(stderr, 
        "AMUDP SPMD Runtime Layer v%s, Copyright 2001, Dan Bonachea\n"
        "Usage: %s%s <spawnfn> program args...\n"
        " <spawnfn> = one of the following mechanisms for spawning remote workers:\n"
        , AMUDP_LIBRARY_VERSION_STR, (*argv)[0], (usingdefaultdegree?" <paralleldegree>":""));
      for (int i=0; AMUDP_Spawnfn_Desc[i].abbrev; i++) {
        fprintf(stderr, "    '%c'  %s\n",  
              toupper(AMUDP_Spawnfn_Desc[i].abbrev), AMUDP_Spawnfn_Desc[i].desc);
      }
      exit(1);
      AMX_RETURN_ERR(BAD_ARG);
    }

    // setup bootstrap info 
    AMUDP_SPMDBootstrapInfo_t bootstrapinfo;
    memset(&bootstrapinfo, 0, sizeof(bootstrapinfo)); // prevent valgrind warnings about sending uninit padding
    bootstrapinfo.numprocs = hton32(AMUDP_SPMDNUMPROCS);
    bootstrapinfo.depth = hton32(networkdepth);

    const char *masterHostname = getMyHostName();
    if (!AMX_SilentMode) AMX_Info("master host name: %s", masterHostname);

    // TCP socket lists
    SocketList allList(AMUDP_SPMDNUMPROCS*4+10); // a list of all active sockets
    SocketList coordList(AMUDP_SPMDNUMPROCS);    // a list of all coordination sockets
    SocketList stdinList(AMUDP_SPMDNUMPROCS);    // a list of all stdin routing sockets
    SocketList stdoutList(AMUDP_SPMDNUMPROCS);   // a list of all stdout routing sockets
    SocketList stderrList(AMUDP_SPMDNUMPROCS);   // a list of all stderr routing sockets
    SocketList * const stdList[3] = { &stdinList, &stdoutList, &stderrList };
    FILE       *stdFILE[3] = { stdin, stdout, stderr };
    AMUDP_SPMDSlaveSocket = (SOCKET*)AMX_malloc(AMUDP_SPMDNUMPROCS * sizeof(SOCKET));

    try {
      // create our TCP listen ports 
      unsigned short anyport = 0;
      AMUDP_SPMDListenSocket = listen_socket(anyport, false);
      allList.insert(AMUDP_SPMDListenSocket);

      for (int i=0; i <= 2; i++) {
        if (i == FD_STDIN) {  // disable stdin forwarding, for now
          AMUDP_SPMDStdListenSocket[i] = INVALID_SOCKET;
          bootstrapinfo.stdMaster[i] = hton16(0);
        } else {
          SOCKET s = listen_socket(anyport, false);
          AMUDP_SPMDStdListenSocket[i] = s;
          allList.insert(s);
          bootstrapinfo.stdMaster[i] = hton16(getsockname(s).port());
        }
      }
    } catch (xBase &exn) {
      AMX_RETURN_ERRFR(RESOURCE, AMUDP_SPMDStartup, exn.why());
    }

    { /* flatten a snapshot of the master's environment for transmission to slaves
       * here we assume the standard representation where a pointer to the environment 
       * is stored in a global variable 'environ' and the environment is represented as an array 
       * of null-terminated strings where each has the form 'key=value' and value may be empty, 
       * and the final string pointer is a NULL pointer
       * we flatten this into a list of null-terminated 'key=value' strings, 
       * terminated with a double-null
       */
      int i;
      int totalEnvSize = 0;
      for(i = 0; environ[i]; i++) 
        totalEnvSize += strlen(environ[i]) + 1;
      totalEnvSize++;

      AMUDP_SPMDMasterEnvironment = (char *)AMX_malloc(totalEnvSize);
      char *p = AMUDP_SPMDMasterEnvironment;
      p[0] = '\0';
      for(i = 0; environ[i]; i++) {
        strcpy(p, environ[i]);
        p += strlen(p) + 1;
      }
      *p = '\0';
      AMX_assert((p+1) - AMUDP_SPMDMasterEnvironment == totalEnvSize);
      bootstrapinfo.environtablesz = hton32(totalEnvSize);
    }

    // find the master addr
    SockAddr masterAddr = getsockname(AMUDP_SPMDListenSocket);
    const char *masterIPstr = AMUDP_getenv_prefixed_withdefault("MASTERIP", "");
    if (*masterIPstr) masterAddr = SockAddr(masterIPstr, masterAddr.port());
    if (masterAddr.IP() == 0) {
      try { /* requires master can resolve its own address */
        SockAddr dnsAddr = DNSLookup(getMyHostName());
        masterAddr = SockAddr(dnsAddr.IP(), masterAddr.port());
      } catch (xBase &exn) {
        AMX_Warn("Master %s failed to resolve its own hostname: %s%s",
          getMyHostName(),exn.why(),
          (USE_NUMERIC_MASTER_ADDR?"":"\nTry setting " AMX_ENV_PREFIX_STR "_MASTERIP")); 
        AMX_RETURN_ERRFR(RESOURCE, AMUDP_SPMDStartup, exn.why());
      }
    }

    // setup NULL-terminated array of extra environment vars for slave
    // Currently have only one such variable, "AMUDP_SLAVE_ARGS":
    //          flag[,master,[network]]
    //     flag: zero = this is not a slave
    //           positive = this is a slave and value is verbosity (1 = not verbose)
    //           -1 = this is a slave performing restart
    //   master: IP or hostname of the master node (require if flag != 0)
    //  network: value of [PREFIX]_WORKERIP if given
    char slave_env[1024] = AMUDP_SPMDSLAVE_ARGS "=";
    strncat(slave_env,
            (AMUDP_SPMDRestartActive ? "-1," : (AMX_SilentMode ? "1," : "2,")),
            sizeof(slave_env) - 1);
    ssize_t remain = sizeof(slave_env) - (strlen(slave_env) + 1);
    if (*masterIPstr) {
      strncat(slave_env, masterAddr.FTPStr(), remain);
    }
    else {
      #if USE_NUMERIC_MASTER_ADDR
        strncat(slave_env, masterAddr.FTPStr(), remain);
      #else
        char *tmp = slave_env + strlen(slave_env);
        snprintf(tmp, remain, "%s:%i", masterHostname, masterAddr.port());
      #endif
    }
    remain = sizeof(slave_env) - (strlen(slave_env) + 1);
    strncat(slave_env, ",", remain);
    // append WORKERIP which it is needed before the master env is sent
    { char *network = AMUDP_getenv_prefixed_withdefault("WORKERIP","");
      if (network && network[0]) {
        strncat(slave_env, network, remain-1);
      }
    }
    if (!remain) { // ran out of space!
      AMX_FatalErr("Error assembling arguments to SPMD worker threads. Exiting...");
    }
    char *extra_env[2] = { slave_env, NULL };

    { int masterpid = getpid();
      uint32_t masterIP = masterAddr.IP();
      npid = ((uint64_t)masterIP) << 32 | 
             (((uint64_t)masterpid) & 0xFFFF);
      bootstrapinfo.networkpid = hton64(npid);
      if (networkpid) *networkpid = npid;
    }

    // create and initialize the translation table that we'll fill in as slaves connect
    AMUDP_SPMDTranslation_name = (en_t*)AMX_malloc(AMUDP_SPMDNUMPROCS*sizeof(en_t));
    AMUDP_SPMDTranslation_tag = (tag_t*)AMX_malloc(AMUDP_SPMDNUMPROCS*sizeof(tag_t));
    for (int i=0; i < AMUDP_SPMDNUMPROCS; i++) {
      AMUDP_SPMDSlaveSocket[i] = INVALID_SOCKET;
      AMUDP_SPMDTranslation_tag[i] = hton64(npid | ((uint64_t)i) << 16);
    }

    AMUDP_SPMDRedirectStdsockets = strcmp(AMUDP_getenv_prefixed_withdefault("ROUTE_OUTPUT",(DISABLE_STDSOCKET_REDIRECT?"0":"1")),"0");

    // call system-specific spawning routine
    AMUDP_SPMDSpawnRunning = TRUE;
    if (!spawnfn(AMUDP_SPMDNUMPROCS, *argc, *argv, extra_env))
      AMX_FatalErr("Error spawning SPMD worker threads. Exiting...");
    AMUDP_SPMDSpawnRunning = FALSE;

    if (!AMUDP_SPMDRedirectStdsockets) {
      // spawn function disabled our stdsocket redirect - signal the slaves of this fact
      for (int i=0; i <= 2; i++)
        bootstrapinfo.stdMaster[i] = hton16(0);
    }

    // main communication loop for master
    try {
      int numSlavesAttached = 0;

      fd_set sockset;
      fd_set* psockset = &sockset;
      int numset; // helpers for coord socket
      SOCKET *tempSockArr = (SOCKET*)AMX_malloc(sizeof(SOCKET)*AMUDP_SPMDNUMPROCS);
      while (1) {
pollentry:
       #ifdef FD_SETSIZE /* Should always be present, but just in case */
        if (allList.getMaxFd() >= FD_SETSIZE)
          AMX_FatalErr("Open sockets exceed FD_SETSIZE=%d. Exiting...",FD_SETSIZE);
       #endif
        allList.makeFD_SET(psockset);

        if (select(allList.getMaxFd()+1, psockset, NULL, NULL, NULL) == -1) { // block for activity
          perror("select");
          exit(1);
        }
        //------------------------------------------------------------------------------------
        // stdin/stderr/stdout listeners - incoming connections
        for (int i=0; i<=2; i++) {
          SOCKET &s = AMUDP_SPMDStdListenSocket[i];
          if (s != INVALID_SOCKET && FD_ISSET(s, psockset))  {
             setupStdSocket(s, *stdList[i], allList); 
             // must re-select after accepting a connection that might alias a closed listener socket id
             goto pollentry; 
          }
        }
        //------------------------------------------------------------------------------------
        // stdout/err sockets - must come before possible exit to drain output
        handleStdOutput(stdout, psockset, stdoutList, allList, AMUDP_SPMDNUMPROCS);
        handleStdOutput(stderr, psockset, stderrList, allList, AMUDP_SPMDNUMPROCS);
        // stdin (illegal to receive anything here)
        if ((numset = stdinList.getIntersection(psockset, tempSockArr, AMUDP_SPMDNUMPROCS))) {
          for (int i=0; i < numset; i++) {
            SOCKET s = tempSockArr[i];
            AMX_assert(FD_ISSET(s, psockset));
            if (isClosed(s)) DEBUG_MASTER("dropping a stdinList socket...");
            else AMX_Err("Master got illegal input on a stdin socket");
            stdinList.remove(s);
            allList.remove(s);
          }
        }
        //------------------------------------------------------------------------------------
        // coordination listener
        if (AMUDP_SPMDListenSocket != INVALID_SOCKET && 
            FD_ISSET(AMUDP_SPMDListenSocket, psockset)) { // incoming connection on coordination socket
          //DEBUG_MASTER("got some activity on AMUDP_SPMDListenSocket");
          if (numSlavesAttached < AMUDP_SPMDNUMPROCS) { // attach a slave
            SockAddr remoteAddr;
            SOCKET newcoord = accept_socket(AMUDP_SPMDListenSocket, remoteAddr);

            #if USE_COORD_KEEPALIVE
            { // make sure we get connection termination notification in a timely manner
              int val = 1;
              if (setsockopt(newcoord, SOL_SOCKET, SO_KEEPALIVE, (char *)&val, sizeof(int)) == SOCKET_ERROR)
                DEBUG_MASTER("failed to setsockopt(SO_KEEPALIVE) on coord socket");
            }
            #endif

            { // receive bootstrapping info
              static int32_t next_procid = 0;
              int32_t procid, procid_nb;
              en_t name;

              recvAll(newcoord, &procid_nb, sizeof(procid_nb));
              recvAll(newcoord, &name, sizeof(name));
              procid = ntoh32(procid_nb);
              if (procid == AMUDP_PROCID_ALLOC) {
                // This is a request (e.g. by a spawner) for a procid assignment
                procid = next_procid++;
                procid_nb = hton32(procid);
                sendAll(newcoord, &procid_nb, sizeof(procid_nb));
                shutdown(newcoord, SHUT_RDWR);
                close_socket(newcoord);
              } else {
                // This is a slave connecting
                if (procid == AMUDP_PROCID_NEXT) procid = next_procid++;
                AMUDP_SPMDSlaveSocket[procid] = newcoord;
                AMUDP_SPMDTranslation_name[procid] = name;
                coordList.insert(newcoord);
                allList.insert(newcoord);
                numSlavesAttached++;
              }
            }

            if (numSlavesAttached == AMUDP_SPMDNUMPROCS) { // all have now reported in, so we can begin computation
              // close listener
              close_socket(AMUDP_SPMDListenSocket);
              allList.remove(AMUDP_SPMDListenSocket);
              AMUDP_SPMDListenSocket = INVALID_SOCKET;

              // sanity check: look for conflicting networks which prevent inter-slave comms
              int saw_local = 0, saw_nonlocal = 0;
              en_t worker_subnet;
              bool force_output = false;
              for (int i=0; i < AMUDP_SPMDNUMPROCS; i++) {
                if (SockAddr(&AMUDP_SPMDTranslation_name[i]).is_localhost()) saw_local++;
                else {
                  saw_nonlocal++;
                  worker_subnet = AMUDP_SPMDTranslation_name[i];
                }
              }
              if (saw_local && saw_nonlocal) {
                worker_subnet.sin_addr.s_addr &= 0x0000FFFFu;
                AMX_Warn("Detected that %i of %i workers are using the localhost network, "
                         "which may prevent communication between ranks.\n"
                         "    This might indicate a DNS misconfiguration on those nodes.\n"
                         "    You may be able to workaround this by requesting a particular subnet for worker comms, "
                         "ex: " AMX_ENV_PREFIX_STR "_WORKERIP=%s",
                         saw_local, AMUDP_SPMDNUMPROCS, SockAddr(&worker_subnet).IPStr());
                force_output = true;
              }

              int32_t bootstrapinfosz_nb = hton32(sizeof(bootstrapinfo));
              // transmit bootstrapping info
              for (int i=0; i < AMUDP_SPMDNUMPROCS; i++) {
                // fill out process-specific bootstrap info
                bootstrapinfo.procid = hton32(i);
                bootstrapinfo.tag = AMUDP_SPMDTranslation_tag[i];
                // send it
                sendAll(AMUDP_SPMDSlaveSocket[i], &bootstrapinfosz_nb, sizeof(int32_t));
                sendAll(AMUDP_SPMDSlaveSocket[i], &bootstrapinfo, sizeof(bootstrapinfo));
                sendAll(AMUDP_SPMDSlaveSocket[i], AMUDP_SPMDTranslation_name, AMUDP_SPMDNUMPROCS*sizeof(en_t));
                sendAll(AMUDP_SPMDSlaveSocket[i], AMUDP_SPMDTranslation_tag, AMUDP_SPMDNUMPROCS*sizeof(tag_t));
                sendAll(AMUDP_SPMDSlaveSocket[i], AMUDP_SPMDMasterEnvironment, ntoh32(bootstrapinfo.environtablesz));
              }
              if (!AMX_SilentMode || force_output) {
                AMX_Info("Endpoint table (nproc=%i):", AMUDP_SPMDNUMPROCS);
                for (int j=0; j < AMUDP_SPMDNUMPROCS; j++) {
                  char temp1[80], temp2[80];
                  AMX_Info(" P#%i:\t%s\ttag: %s", j, 
                             AMUDP_enStr(AMUDP_SPMDTranslation_name[j], temp1),
                             AMUDP_tagStr(ntoh64(AMUDP_SPMDTranslation_tag[j]), temp2));
                }
              }
            }
          } else AMX_Err("master detected some unrecognized activity on AMUDP_SPMDListenSocket");
        }
        //------------------------------------------------------------------------------------
        // coord sockets
        if ((numset = coordList.getIntersection(psockset, tempSockArr, AMUDP_SPMDNUMPROCS))) { // we have some active coord sockets
          //DEBUG_MASTER("got some activity on coord sockets");
          for (int i=0; i < numset; i++) {
            SOCKET s = tempSockArr[i];
            AMX_assert(FD_ISSET(s, psockset));
            if (isClosed(s)) {
              DEBUG_MASTER("dropping a coordList socket...");
              coordList.remove(s);
              allList.remove(s);

              #if ABORT_JOB_ON_NODE_FAILURE
                int exitCode = -1;
                int32_t exitCode_nb = hton32(exitCode);
                for (int i=0; i < (int)coordList.getCount(); i++) {
                  sendAll(coordList[i], "E");
                  sendAll(coordList[i], &exitCode_nb, sizeof(int32_t));
                  close_socket(coordList[i]);
                }
                if (!socklibend()) AMX_Err("master failed to socklibend()");
                DEBUG_MASTER("Lost a worker process - job aborting...");
                exit(exitCode);
              #endif
              continue;
            }
            char command;
            recvAll(s, &command, 1);
            switch(command) {
              case 'B': { // enter barrier
                static int AMUDP_SPMDBarrierCount = 0; /* number of processors that have entered barrier */
                AMUDP_SPMDBarrierCount++;
                if (AMUDP_SPMDBarrierCount == AMUDP_SPMDNUMPROCS) { // barrier complete
                  DEBUG_MASTER("Completed barrier");
                  // broadcast completion message
                  for (int i=0; i < (int)coordList.getCount(); i++) {
                    sendAll(coordList[i], "B");
                  }
                  AMUDP_SPMDBarrierCount = 0;
                }
                break;
              }

              case 'G': { // enter gather
                static int AMUDP_SPMDGatherCount = 0; /* number of processors that have sent gather messages */
                static int AMUDP_SPMDGatherLen = 0;
                static char *AMUDP_SPMDGatherBuf = NULL;
                int32_t len=0;
                int32_t len_nb=0;
                int32_t id=0;
                int32_t id_nb=0;
                try {
                  recvAll(s, &id_nb, sizeof(int32_t));
                  recvAll(s, &len_nb, sizeof(int32_t));
                } catch (xSocket& exn) {
                  AMX_Err("got exn while reading gather len: %s", exn.why());
                }
                id = ntoh32(id_nb);
                len = ntoh32(len_nb);
                AMX_assert(id >= 0 && id < AMUDP_SPMDNUMPROCS && len > 0);
                if (AMUDP_SPMDGatherCount == 0) { // first slave to report
                  AMX_assert(AMUDP_SPMDGatherBuf == NULL && AMUDP_SPMDGatherLen == 0);
                  AMUDP_SPMDGatherLen = len;
                  AMUDP_SPMDGatherBuf = (char *)AMX_malloc(AMUDP_SPMDGatherLen*AMUDP_SPMDNUMPROCS);
                } else AMX_assert(len == AMUDP_SPMDGatherLen);
                try {
                  recvAll(s, &(AMUDP_SPMDGatherBuf[AMUDP_SPMDGatherLen*id]), AMUDP_SPMDGatherLen);
                } catch (xSocket& exn) {
                  AMX_Err("got exn while reading gather data: %s", exn.why());
                }
                AMUDP_SPMDGatherCount++;
                if (AMUDP_SPMDGatherCount == AMUDP_SPMDNUMPROCS) { // gather complete
                  DEBUG_MASTER("Completed gather");
                  hton32a(&len);
                  // broadcast completion data
                  for (int i=0; i < (int)coordList.getCount(); i++) {
                    sendAll(coordList[i], "G");
                    sendAll(coordList[i], &len_nb, sizeof(int32_t));
                    sendAll(coordList[i], AMUDP_SPMDGatherBuf, AMUDP_SPMDGatherLen*AMUDP_SPMDNUMPROCS);
                  }
                  AMX_free(AMUDP_SPMDGatherBuf);
                  AMUDP_SPMDGatherBuf = NULL;
                  AMUDP_SPMDGatherCount = 0;
                  AMUDP_SPMDGatherLen = 0;
                }
                break;
              }

              case 'E': { // exit code
                // get slave terminate code
                int32_t exitCode_nb = -1;
                int exitCode = -1;
                try {
                  recvAll(s, &exitCode_nb, sizeof(int32_t));
                } catch (xSocket& exn) {
                  AMX_Err("got exn while reading exit code: %s", exn.why());
                }
                exitCode = ntoh32(exitCode_nb);
                // tell all other slaves to terminate
                // TODO: perhaps use an active message for this? for now, just rely on coord socket dying
                exitCode_nb = hton32(0);
                for (int i=0; i < (int)coordList.getCount(); i++) {
                  sendAll(coordList[i], "E");
                  sendAll(coordList[i], &exitCode_nb, sizeof(int32_t));
                  close_socket(coordList[i]);
                }
                /* bug 2029 - wait for any final stdout/stderr to arrive before shutdown */
                uint64_t wait_iter = 0;
                while (stdoutList.getCount() || stderrList.getCount()) { // await final output
                  if (!AMX_SilentMode && (!wait_iter++)) AMX_Info("Awaiting final slave outputs...");
                  for (int i=1; i <= 2; i++) {
                    if (stdList[i]->getCount()) {
                      stdList[i]->makeFD_SET(psockset);
                      handleStdOutput(stdFILE[i], psockset, *stdList[i], allList, stdList[i]->getCount());
                    }
                  }
                  AMX_sched_yield();
                }
                if (!socklibend()) AMX_Err("master failed to socklibend()");
                if (!AMX_SilentMode) AMX_Info("Exiting after AMUDP_SPMDExit(%i)...", exitCode);
                exit(exitCode);
                break;
              }

              default:
                AMX_Err("master got an unknown command on coord socket: %c", command);
            }
          }
          if (coordList.getCount() == 0) {
            DEBUG_MASTER("Exiting after losing all worker slave connections (noone called AMUDP_Exit())");
            exit(0); // program exit, noone called terminate
          }
        }
        //------------------------------------------------------------------------------------
      } // loop
    } catch (xSocket& exn) {
      AMX_FatalErr("Master got an xSocket: %s", exn.why());
    } catch (xBase& exn) {
      AMX_FatalErr("Master got an xBase: %s", exn.why());
    }
  }
  /* ------------------------------------------------------------------------------------ 
   *  I'm a worker slave 
   * ------------------------------------------------------------------------------------ */
  else {  
    #ifdef AMUDP_BLCR_ENABLED
      // Restart Step 1: Gets procid from master and restarts corresponding context file
      const int doRunRestart = (slave_flag < 0);
      // Not either of the restart cases:
      const int doFullBoostrap = !(doRunRestart || AMUDP_SPMDRestartActive);
    #else
      #define doRunRestart 0
      #define doFullBoostrap 1
    #endif

    int temp;

    /* propagate verbosity setting from master */
    AMX_SilentMode = (slave_flag < 2); // TODO: values >2 for more verbose

    if (doFullBoostrap) {
    #if FREEZE_SLAVE
      AMX_freezeForDebugger();
    #else
      /* do *not* use prefixed getenv here - want an independent freeze point */
      if (getenv("AMUDP_FREEZE")) AMX_freezeForDebugger();
    #endif
    }

    if (!eb || !ep) AMX_RETURN_ERR(BAD_ARG);
    if (doFullBoostrap && AM_Init() != AM_OK) {
      AMX_Err("Failed to AM_Init() in AMUDP_SPMDStartup");
      AMX_RETURN_ERRFR(RESOURCE, AMUDP_SPMDStartup, "AM_Init() failed");
    }

    // parse special env var with our arguments
    char * _slave_args = AMX_strdup(env_var);
    char * slave_args = _slave_args;
    SockAddr masterAddr;
    { // Strip required "flag," off beginning
      char *endptr;
      (void) strtol(slave_args, &endptr, 0);
      if (! endptr || (',' != endptr[0])) AMX_Err("Malformed arguments '%s' to slave process", env_var);
      slave_args = endptr + 1;
    }
    
    // extract appended WORKERIP which it is needed before the master env is sent
    // must extract this field even without HAVE_GETIFADDRS
    const char *network = "";
    { char *delimiter = strrchr(slave_args,',');
      if (delimiter != NULL) {
        network = delimiter+1;
        *delimiter = '\0';
      }
    }

    char *master_localhost_warning = NULL;
    { // extract master's address
      if (strchr(slave_args,',')) {
        masterAddr = SockAddr(slave_args);
      } else {
        char *IPStr = (char *)AMX_malloc(strlen(slave_args)+10);
        strcpy(IPStr, slave_args);
        char *portStr = strchr(IPStr, ':');
        if (!portStr) {
          AMX_Err("Malformed address argument passed to slave:'%s' (missing port)", slave_args);
          AMX_RETURN_ERR(BAD_ARG);
        }
        int masterPort = atoi(portStr+1);
        if (masterPort < 1 || masterPort > 65535) {
          AMX_Err("Malformed address argument passed to slave:'%s' (bad port=%i)", slave_args, masterPort);
          AMX_RETURN_ERR(BAD_ARG);
        }
        (*portStr) = '\0';
        try {
          masterAddr = SockAddr((uint32_t)DNSLookup(IPStr).IP(), (uint16_t)masterPort);
          if (masterAddr.is_localhost()) {
              // we resolved master to 127.x.y.z localhost network, which may cause problems..
              master_localhost_warning = (char *)AMX_malloc(1024);
              sprintf(master_localhost_warning, "slave %s resolved master hostname '%s' to the localhost network. "
                        "You may need to set " AMX_ENV_PREFIX_STR "_MASTERIP to the master's external IP address.", 
                        getMyHostName(), IPStr);
          }
        } catch (xSocket &exn) {
          AMX_RETURN_ERRFR(RESOURCE, AMUDP_SPMDStartup, "slave failed DNSLookup on master host name");
        }
        AMX_free(IPStr);
      }
    }

    try {
      if (!AMX_SilentMode) AMX_Info("slave connecting to %s:%i", masterAddr.IPStr(), masterAddr.port());

      try {
          AMUDP_SPMDControlSocket = connect_socket(masterAddr);
      } catch (xSocket& exn) { // check for common failure mode
          if (master_localhost_warning) AMX_Warn("%s",master_localhost_warning);
          throw; // re-throw
      }
      AMX_free(master_localhost_warning);

      #if USE_COORD_KEEPALIVE
      { // make sure we get connection termination notification in a timely manner
        int val = 1;
        if (setsockopt(AMUDP_SPMDControlSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&val, sizeof(int)) == SOCKET_ERROR)
          DEBUG_MASTER("failed to setsockopt(SO_KEEPALIVE) on coord socket");
      }
      #endif

      #ifdef AMUDP_BLCR_ENABLED
        if (doRunRestart) {
          // construct args for use by the caller
          static const char *new_argv[] =  { (*argv)[0], /* spawner */
                                             (*argv)[1], /* DIR     */
                                             env_var+1,  /* env_var w/ "-1" -> "1"  */
                                             NULL };
          *argc = 4;
          *argv = (char**)new_argv;

          // Get procid from master and return it to the caller
          int32_t procid_nb = hton32(AMUDP_PROCID_ALLOC);
          sendAll(AMUDP_SPMDControlSocket, &procid_nb, sizeof(procid_nb));
          sendAll(AMUDP_SPMDControlSocket, &AMUDP_SPMDName, sizeof(AMUDP_SPMDName));
          recvAll(AMUDP_SPMDControlSocket, &procid_nb, sizeof(procid_nb));
          shutdown(AMUDP_SPMDControlSocket, SHUT_RDWR);
          close_socket(AMUDP_SPMDControlSocket);

          return ntoh32(procid_nb);
        }
      #endif // AMUDP_BLCR_ENABLED

      /* here we assume the interface used to contact the master is the same 
         one to be used for UDP endpoints */
      SockAddr myinterface = getsockname(AMUDP_SPMDControlSocket);
      if (network && network[0]) {
        #if HAVE_GETIFADDRS // allow user to override our same-interface assumption
          SockAddr networkaddr(network, 0);
          char subnets[1024];
          if (! getIfaceAddr(networkaddr, myinterface, subnets, sizeof(subnets))) {
            AMX_Err("Failed to find interface on requested subnet %s. Available subnets: %s", network, subnets);
            AMX_RETURN(AM_ERR_RESOURCE);
          }
        #else
          AMX_Warn("WORKERIP set in the environment, but your platform lacks the required getifaddrs() support.  Ignoring WORKERIP.");
        #endif
      }
      if (!AMX_SilentMode) AMX_Info("slave using IP %s", myinterface.IPStr());
      AMUDP_SetUDPInterface(myinterface.IP());
      AMX_free(_slave_args);
        
      /* create endpoint and get name */
      temp = AM_AllocateBundle(AM_SEQ, &AMUDP_SPMDBundle);
      if (temp != AM_OK) {
        AMX_Err("Failed to create bundle in AMUDP_SPMDStartup");
        AMX_RETURN(temp);
      }
      temp = AM_AllocateEndpoint(AMUDP_SPMDBundle, &AMUDP_SPMDEndpoint, &AMUDP_SPMDName);
      if (temp != AM_OK) {
        AMX_Err("Failed to create endpoint in AMUDP_SPMDStartup");
        AMX_RETURN(temp);
      }

      // send our procid and endpoint name to the master
      int32_t procid_nb = hton32(AMUDP_SPMDMYPROC);
      sendAll(AMUDP_SPMDControlSocket, &procid_nb, sizeof(procid_nb));
      sendAll(AMUDP_SPMDControlSocket, &AMUDP_SPMDName, sizeof(AMUDP_SPMDName));

      // get information from master 
      // get the bootstrap info and translation table
      AMUDP_SPMDBootstrapInfo_t bootstrapinfo;
      int32_t bootstrapinfosz_nb;
      recvAll(AMUDP_SPMDControlSocket, &bootstrapinfosz_nb, sizeof(int32_t));
      int32_t bootstrapinfosz = ntoh32(bootstrapinfosz_nb);
      AMX_assert(bootstrapinfosz == sizeof(AMUDP_SPMDBootstrapInfo_t));
      recvAll(AMUDP_SPMDControlSocket, &bootstrapinfo, sizeof(AMUDP_SPMDBootstrapInfo_t));
      
      // unpack the bootstrapping info
      if (doFullBoostrap) {
        AMUDP_SPMDNUMPROCS = ntoh32(bootstrapinfo.numprocs);
        AMUDP_SPMDMYPROC = ntoh32(bootstrapinfo.procid);
      } else {
        if (AMUDP_SPMDNUMPROCS != (int32_t)ntoh32(bootstrapinfo.numprocs)) {
          AMX_Err("Restarting with wrong numprocs in AMUDP_SPMDStartup");
          AMX_RETURN_ERR(BAD_ARG);
        }
        if (AMUDP_SPMDMYPROC != (int32_t)ntoh32(bootstrapinfo.procid)) {
          AMX_Err("Restarting with wrong procid in AMUDP_SPMDStartup");
          AMX_RETURN_ERR(BAD_ARG);
        }
      }
      if (networkpid) *networkpid = ntoh64(bootstrapinfo.networkpid);

      // sanity checking on bootstrap info
      AMX_assert(AMUDP_SPMDNUMPROCS > 0 && AMUDP_SPMDNUMPROCS < (int)AMUDP_MAX_SPMDPROCS);
      AMX_assert(AMUDP_SPMDMYPROC >= 0 && AMUDP_SPMDMYPROC < (int)AMUDP_SPMDNUMPROCS);

      #if !DISABLE_STDSOCKET_REDIRECT
      for (int fd=0; fd <= 2; fd++) {
        if (bootstrapinfo.stdMaster[fd]) {
            // perform stdin/out/err redirection
            newstd[fd]  = connect_socket(SockAddr(masterAddr.IP(),ntoh16(bootstrapinfo.stdMaster[fd])));
            #if 0
              // disable buffering
              setvbuf(stdFILE[fd], NULL, _IONBF, 0);
            #endif
            if (dup2(newstd[fd], fd) < 0) { // redirect std FD to socket
              perror("dup2(std)");
              _exit(1); 
            }
        }
     }
     #endif

      // retrieve translation table
      en_t *tempTranslation_name = (en_t *)AMX_malloc(AMUDP_SPMDNUMPROCS*sizeof(en_t));
      tag_t *tempTranslation_tag = (tag_t *)AMX_malloc(AMUDP_SPMDNUMPROCS*sizeof(tag_t));
      AMX_assert(tempTranslation_name && tempTranslation_tag);
      recvAll(AMUDP_SPMDControlSocket, tempTranslation_name, AMUDP_SPMDNUMPROCS*sizeof(en_t));
      recvAll(AMUDP_SPMDControlSocket, tempTranslation_tag, AMUDP_SPMDNUMPROCS*sizeof(tag_t));

      AMX_assert(ntoh64(tempTranslation_tag[AMUDP_SPMDMYPROC]) == ntoh64(bootstrapinfo.tag));
      AMX_assert(enEqual(tempTranslation_name[AMUDP_SPMDMYPROC], AMUDP_SPMDName));


      // setup translation table
      temp = AM_SetNumTranslations(AMUDP_SPMDEndpoint, AMUDP_SPMDNUMPROCS);
      if (temp != AM_OK) {
        AMX_Err("Failed to AM_SetNumTranslations() in AMUDP_SPMDStartup");
        AMX_RETURN(temp);
      }
      for (int i = 0; i < AMUDP_SPMDNUMPROCS; i++) {
        temp = AM_Map(AMUDP_SPMDEndpoint, i, tempTranslation_name[i], ntoh64(tempTranslation_tag[i]));
        if (temp != AM_OK) {
          AMX_Err("Failed to AM_Map() in AMUDP_SPMDStartup");
          AMX_RETURN(temp);
        }
      }

      if (!AMX_ProcessLabel) {
        static char label[80];
        snprintf(label,sizeof(label),"Node %i",AMUDP_SPMDMYPROC);
        AMX_ProcessLabel = label;
      }

      AMX_free(tempTranslation_name);
      tempTranslation_name = NULL;
      AMX_free(tempTranslation_tag);
      tempTranslation_tag = NULL;

      // receive snapshot of master environment
      int environtablesz = ntoh32(bootstrapinfo.environtablesz);
      char *tempEnvironment = (char *)AMX_malloc(environtablesz);
      AMX_assert(tempEnvironment != NULL);
      recvAll(AMUDP_SPMDControlSocket, tempEnvironment, environtablesz);
      if (doFullBoostrap) {
        AMUDP_SPMDMasterEnvironment = tempEnvironment;
      } else  {
        // On restart we keep the environment from the initial run
        AMX_assert(AMUDP_SPMDMasterEnvironment != NULL);
        AMX_free(tempEnvironment);
      }
      
      /* allocate network buffers */
      if (doFullBoostrap) networkdepth = ntoh32(bootstrapinfo.depth);
      temp = AM_SetExpectedResources(AMUDP_SPMDEndpoint, AMUDP_SPMDNUMPROCS, networkdepth);
      if (temp != AM_OK) {
        AMX_Err("Failed to AM_SetExpectedResources() in AMUDP_SPMDStartup");
        AMX_RETURN(temp);
      }
      #ifdef AMUDP_BLCR_ENABLED
        AMUDP_SPMDNetworkDepth = networkdepth;
      #endif
      
      // set tag
      temp = AM_SetTag(AMUDP_SPMDEndpoint, ntoh64(bootstrapinfo.tag));
      if (temp != AM_OK) {
        AMX_Err("Failed to AM_SetTag() in AMUDP_SPMDStartup");
        AMX_RETURN(temp);
      }

    } catch (xSocket& exn) {
      AMX_FatalErr("Got an xSocket while spawning slave process: %s", exn.why());
    }

    *eb = AMUDP_SPMDBundle;
    *ep = AMUDP_SPMDEndpoint;
    AMUDP_SPMDStartupCalled = 1;

    /* Ensure that any children we fork() won't appear to be slaves */
    #if 1
      unsetenv(AMUDP_SPMDSLAVE_ARGS);
    #else
      putenv((char*)AMUDP_SPMDSLAVE_ARGS "=0");
    #endif

    #if USE_ASYNC_TCP_CONTROL
      // enable async notification
      reghandler(AMUDP_SIGIO, AMUDP_SPMDControlSocketCallback);
      if (fcntl(AMUDP_SPMDControlSocket, F_SETOWN, getpid())) {
        perror("fcntl(F_SETOWN, getpid())");
        AMX_FatalErr("Failed to fcntl(F_SETOWN, getpid()) on TCP control socket - try disabling USE_ASYNC_TCP_CONTROL");
      }
      if (fcntl(AMUDP_SPMDControlSocket, F_SETSIG, AMUDP_SIGIO)) {
        perror("fcntl(F_SETSIG)");
        AMX_FatalErr("Failed to fcntl(F_SETSIG, AMUDP_SIGIO) on TCP control socket - try disabling USE_ASYNC_TCP_CONTROL");
      }
      if (fcntl(AMUDP_SPMDControlSocket, F_SETFL, O_ASYNC|O_NONBLOCK)) { 
        perror("fcntl(F_SETFL, O_ASYNC|O_NONBLOCK)");
        AMX_FatalErr("Failed to fcntl(F_SETFL, O_ASYNC|O_NONBLOCK) on TCP control socket - try disabling USE_ASYNC_TCP_CONTROL");
      }
    #endif

    flushStreams("AMUDP_SPMDStartup"); // to get ENV FS_SYNC 

    if (!AMX_SilentMode) {
      char temp[80];
      tag_t tag = 0;
      AM_GetTag(AMUDP_SPMDEndpoint, &tag);
      AMX_Info("Slave %i/%i starting (tag=%s)...", 
        AMUDP_SPMDMyProc(), AMUDP_SPMDNumProcs(), AMUDP_tagStr(tag, temp));
    }

    return AM_OK;
  }
  /* ------------------------------------------------------------------------------------ */
  AMX_FatalErr("never reach here");
  return AM_OK;
}

/* ------------------------------------------------------------------------------------ 
 *  worker control handler
 * ------------------------------------------------------------------------------------ */
// called by slave to handle traffic on control socket
// sets controlMessagesServiced to indicate how many message serviced
extern int AMUDP_SPMDHandleControlTraffic(int *controlMessagesServiced) {
  if (AMUDP_SPMDControlSocket == INVALID_SOCKET) return AM_OK; // not running in SPMD mode
  #if USE_ASYNC_TCP_CONTROL
    ASYNC_CHECK(1);
    if_pt (!AMUDP_SPMDIsActiveControlSocket) return AM_OK; // nothing to do
    ASYNC_TCP_DISABLE();
    AMUDP_SPMDIsActiveControlSocket = FALSE; 
  #endif 
  if (controlMessagesServiced) *controlMessagesServiced = 0;
  
  while (1) { // service everything waiting
    if_pt (!inputWaiting(AMUDP_SPMDControlSocket,false)) {
      ASYNC_TCP_ENABLE();
      return AM_OK; // nothing more to do
    }

    try {
      SOCKET s = AMUDP_SPMDControlSocket;

      if (isClosed(s)) {
        DEBUG_SLAVE("master control socket slammed shut. Exiting...");
        AMUDP_SPMDShutdown(1);
      }

      // there's something waiting on the control socket for us - grab it
      char command;
      recvAll(s, &command, 1);
      switch(command) {
        case 'B': { // barrier complete
          AMX_assert(!AMUDP_SPMDBarrierDone);
          AMUDP_SPMDBarrierDone = 1; // flag completion
          break;
        }

        case 'G': { // gather complete
          AMX_assert(!AMUDP_SPMDGatherDone && AMUDP_SPMDGatherLen > 0 && AMUDP_SPMDGatherData != NULL);
          try {
            int32_t len_nb = -1;
            recvAll(s, &len_nb, sizeof(int32_t));
            int32_t len = ntoh32(len_nb);
            AMX_assert(len == AMUDP_SPMDGatherLen);
            recvAll(s, AMUDP_SPMDGatherData, AMUDP_SPMDGatherLen*AMUDP_SPMDNUMPROCS);
          } catch (xSocket& exn) {
            AMX_FatalErr("got exn while reading gather data: %s", exn.why());
          }
          AMUDP_SPMDGatherDone = 1; // flag completion
          break;
        }

        case 'E': { // exit code
          // get slave terminate code
          int32_t exitCode_nb = -1;
          int exitCode = -1;
          try {
            recvAll(s, &exitCode_nb, sizeof(int32_t));
            exitCode = ntoh32(exitCode_nb);
          } catch (xSocket& exn) {
            AMX_Err("got exn while reading exit code: %s", exn.why());
          }
          if (!AMX_SilentMode) AMX_Info("Exiting after exit signal from master (%i)...", exitCode);
          AMUDP_SPMDShutdown(exitCode);
          break;
        }

        default:
          AMX_FatalErr("slave got an unknown command on coord socket: %c", command);
        }
    } catch (xSocket& exn) {
      AMX_Err("Slave got an xSocket: %s. Exiting...", exn.why());
      AMUDP_SPMDShutdown(1);
    } catch (xBase& exn) {
      AMX_Err("Slave got an xBase: %s. Exiting...", exn.why());
      AMUDP_SPMDShutdown(1);
    }
    if (controlMessagesServiced) (*controlMessagesServiced)++;
  }
}
/* ------------------------------------------------------------------------------------ 
 *  process termination
 * ------------------------------------------------------------------------------------ */
static amudp_exitcallback_t AMUDP_SPMDExitCallback = NULL;
extern int AMUDP_SPMDSetExitCallback(amudp_exitcallback_t fp) {
  AMUDP_SPMDExitCallback = fp;
  return AM_OK;
}
extern "C" {
  void (*AMUDP_SPMDkillmyprocess)(int) = &_exit;
}

/* shutdown this process */
static int AMUDP_SPMDShutdown(int exitcode) {
  ASYNC_TCP_DISABLE_IGNOREERR(); /* (bug 765) prevent race where master has already reset async control socket */
  /* this function is not re-entrant - if someone tries, something is seriously wrong */
  { static int shutdownInProgress = FALSE;
    if (shutdownInProgress) AMX_FatalErr("recursive failure in AMUDP_SPMDShutdown"); 
    shutdownInProgress = TRUE;
  }

  flushStreams("AMUDP_SPMDShutdown");

  if (AMUDP_SPMDExitCallback) (*AMUDP_SPMDExitCallback)(exitcode);

  /* important to make this call to release resources */
  if (AM_Terminate() != AM_OK) 
    AMX_Err("failed to AM_Terminate() in AMUDP_SPMDExit()");

  flushStreams("AMUDP_SPMDShutdown");

  if (fclose(stdin))  AMX_VERBOSE_INFO(("failed to fclose stdin in AMUDP_SPMDExit(): %s(%i)",strerror(errno),errno)); 
  if (fclose(stdout)) AMX_VERBOSE_INFO(("failed to fclose stdout in AMUDP_SPMDExit(): %s(%i)",strerror(errno),errno)); 
  if (fclose(stderr)) AMX_VERBOSE_INFO(("failed to fclose stderr in AMUDP_SPMDExit(): %s(%i)",strerror(errno),errno)); 

  /* use normal shutdown and closesocket to ignore errors */
  for (int i=0; i <= 2; i++) {
    SOCKET s = newstd[i];
    if (s != INVALID_SOCKET) {
      shutdown(s, SHUT_RDWR);
      closesocket(s); 
    }
  }

  AMX_sched_yield();

  if (AMUDP_SPMDControlSocket != INVALID_SOCKET) {
    closesocket(AMUDP_SPMDControlSocket);
  }

  if (!socklibend()) AMX_Err("slave failed to socklibend()");

  AMUDP_SPMDStartupCalled = 0;
  DEBUG_SLAVE("exiting..");
  AMUDP_SPMDkillmyprocess(exitcode);
  AMX_FatalErr("AMUDP_SPMDkillmyprocess failed");
  return AM_OK;
}

extern int AMUDP_SPMDExit(int exitcode) {
  DEBUG_SLAVE("AMUDP_SPMDExit");
  if (!AMUDP_SPMDStartupCalled) AMX_RETURN_ERR(NOT_INIT);

  ASYNC_TCP_DISABLE_IGNOREERR(); /* (bug 765) prevent race where master has already reset async control socket */
  /* this function is not re-entrant - if someone tries, something is seriously wrong */
  { static int exitInProgress = FALSE;
    if (exitInProgress) AMX_FatalErr("recursive failure in AMUDP_SPMDExit"); 
    exitInProgress = TRUE;
  }

  flushStreams("AMUDP_SPMDExit");

  AMX_sched_yield();

  /* notify master we're exiting */

  // We disable exceptions on the following sendALL calls because the C++
  // spec warns that exceptions may not be usable in signal handlers, and
  // GASNet calls here when handling a fatal or termination signal.
  /* try */ {
    int exitcode_nb = hton32(exitcode);
    sendAll(AMUDP_SPMDControlSocket, "E", -1, false);
    sendAll(AMUDP_SPMDControlSocket, &exitcode_nb, sizeof(int32_t), false);
    while (1) { // swallow everything and wait for master to close
      char temp;
      int retval = recv(AMUDP_SPMDControlSocket, &temp, 1, 0); 
      if (retval == 0 || retval == SOCKET_ERROR) break;
    }
  } /* catch (xBase& ) { } */ // ignore errors that may happen on conn reset

  AMUDP_SPMDStartupCalled = 0;
  DEBUG_SLAVE("AMUDP_SPMDShutdown..");
  /* exit this proc gracefully */
  AMUDP_SPMDShutdown(0);
  AMX_FatalErr("AMUDP_SPMDShutdown failed");
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ 
 *  poll-wait for a flag to become non-zero as a result of a control message
 * ------------------------------------------------------------------------------------ */
static void AMUDP_SPMDWaitForControl(volatile int *done) {
  #if USE_BLOCKING_SPMD_BARRIER
    { int oldmask;
      AM_GetEventMask(AMUDP_SPMDBundle, &oldmask);
      // wait for completion
      AM_Poll(AMUDP_SPMDBundle);
      while (!*done) {
        AM_SetEventMask(AMUDP_SPMDBundle, AM_NOTEMPTY);
        AMUDP_SPMDwakeupOnControlActivity = 1;
        AM_WaitSema(AMUDP_SPMDBundle);
        AMUDP_SPMDwakeupOnControlActivity = 0;
        AM_Poll(AMUDP_SPMDBundle);
      }
      AM_SetEventMask(AMUDP_SPMDBundle, oldmask);
    }
  #else
    { uint32_t timeoutusec = 100;
      AM_Poll(AMUDP_SPMDBundle);
      while (!*done) {

        AMX_usleep(timeoutusec);

        AM_Poll(AMUDP_SPMDBundle);
        if (timeoutusec < 10000) timeoutusec *= 2;
      }
    }
  #endif
}
/* ------------------------------------------------------------------------------------ 
 *  barrier
 * ------------------------------------------------------------------------------------ */
extern int AMUDP_SPMDBarrier() {
  if (!AMUDP_SPMDStartupCalled) {
    AMX_Err("called AMUDP_SPMDBarrier before AMUDP_SPMDStartup()");
    AMX_RETURN_ERR(NOT_INIT);
  }

  flushStreams("AMUDP_SPMDBarrier");
  AMX_assert(AMUDP_SPMDBarrierDone == 0);
  ASYNC_TCP_DISABLE();
  sendAll(AMUDP_SPMDControlSocket, "B");
  ASYNC_TCP_ENABLE();

  AMUDP_SPMDWaitForControl(&AMUDP_SPMDBarrierDone);

  AMUDP_SPMDBarrierDone = 0;
  DEBUG_SLAVE("Leaving barrier");
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ 
 *  AMUDP_SPMDAllGather: gather len bytes from source buf on each node, concatenate them and write 
 *  them into the dest buffer (which must have length len*numnodes) in rank order
 * ------------------------------------------------------------------------------------ */
extern int AMUDP_SPMDAllGather(void *source, void *dest, size_t len) {
  if (!AMUDP_SPMDStartupCalled) {
    AMX_Err("called AMUDP_SPMDAllGather before AMUDP_SPMDStartup()");
    AMX_RETURN_ERR(NOT_INIT);
  }
  if (source == NULL) AMX_RETURN_ERR(BAD_ARG);
  if (dest == NULL) AMX_RETURN_ERR(BAD_ARG);
  if (len <= 0) AMX_RETURN_ERR(BAD_ARG);

  AMX_assert(AMUDP_SPMDGatherDone == 0);
  AMUDP_SPMDGatherData = dest;
  AMUDP_SPMDGatherLen = len;
  int32_t myid_nb = hton32(AMUDP_SPMDMYPROC);
  int32_t mylen_nb = hton32(len);

  ASYNC_TCP_DISABLE();
  sendAll(AMUDP_SPMDControlSocket, "G");
  sendAll(AMUDP_SPMDControlSocket, &myid_nb, sizeof(int));
  sendAll(AMUDP_SPMDControlSocket, &mylen_nb, sizeof(int));
  sendAll(AMUDP_SPMDControlSocket, source, len);
  ASYNC_TCP_ENABLE();
  
  AMUDP_SPMDWaitForControl(&AMUDP_SPMDGatherDone);

  AMUDP_SPMDGatherDone = 0;
  DEBUG_SLAVE("Leaving gather");
  return AM_OK;
}

/* ------------------------------------------------------------------------------------ 
 *  global getenv()
 * ------------------------------------------------------------------------------------ */
extern const char* AMUDP_SPMDgetenvMaster(const char *keyname) {
  if (!AMUDP_SPMDMasterEnvironment) {
    AMX_Err("called AMUDP_SPMDgetenvMaster before AMUDP_SPMDStartup()");
    return NULL;
  }

  char *p = AMUDP_SPMDMasterEnvironment;
  if (!keyname) return NULL;
  int keylen = strlen(keyname);
  while (*p) {
    if (!strncmp(keyname, p, keylen) && p[keylen] == '=') {
      return p + keylen + 1;
    }
    p += strlen(p) + 1;
  }
  return NULL; // not found
}

extern char *AMUDP_getenv_prefixed(const char *basekey) {
  char key[3][255];
  const char *val[3];
  int winner = -1;
  char *(*getfn)(const char *) = NULL;
  if (AMUDP_SPMDMasterEnvironment) getfn = (char *(*)(const char *))AMUDP_SPMDgetenvMaster;
  else getfn = (char *(*)(const char *))getenv;

  if (basekey == NULL || !*basekey) return NULL;
  sprintf(key[0], "%s_%s", AMX_ENV_PREFIX_STR, basekey);
  val[0] = getfn(key[0]);
  sprintf(key[1], "%s_%s", "AMUDP", basekey);
  val[1] = getfn(key[1]);
  strcpy(key[2], basekey);
  val[2] = getfn(key[2]);
  for (int i=0; i < 3; i++) {
    if (val[i] != NULL) {
      if (winner == -1) winner = i;
      else if (strcmp(val[winner], val[i])) {
        AMX_Warn("Both $%s and $%s are set, to different values. Using the former.",
          key[winner], key[i]);
      }
    }
  }
  if (winner == -1) return NULL;
  else return (char *)val[winner];
}

extern char *AMUDP_getenv_prefixed_withdefault(const char *basekey, const char *defaultval) {
  static int firsttime = 1;
  static int verboseenv = 0;
  char * retval = NULL;
  int usingdefault = 0;
  const char *dflt = "";
  if (firsttime) {
    #if AMX_DEBUG_VERBOSE
      verboseenv = 1;
    #else
      verboseenv = !!AMUDP_getenv_prefixed("VERBOSEENV");
    #endif
    firsttime = 0;
  }
  AMX_assert(defaultval != NULL);
  retval = AMUDP_getenv_prefixed(basekey);
  if (retval == NULL) {
    retval = (char *)defaultval;
    dflt = "   (default)";
    usingdefault = 1;
  }
#ifdef gasnett_envstr_display
  { char displaykey[255];
    sprintf(displaykey,"%s_%s",AMX_ENV_PREFIX_STR,basekey);
    gasnett_envstr_display(displaykey, retval, usingdefault);
  }
#else
  if (verboseenv && (AMUDP_SPMDMYPROC == -1 || AMUDP_SPMDMYPROC == 0)) {
    const char *displayval = retval;
    char displaykey[255];
    int width;
    if (strlen(retval) == 0) displayval = "*empty*";
    AMX_assert(strlen(basekey)+strlen(AMX_ENV_PREFIX_STR) < 200);
    sprintf(displaykey,"%s_%s",AMX_ENV_PREFIX_STR,basekey);
    width = MAX(10,55 - strlen(displaykey) - strlen(displayval));
    fprintf(stderr, "ENV parameter: %s = %s%*s\n", displaykey, displayval, width, dflt);
    fflush(stderr);
  }
#endif
  return retval;
}

#ifdef AMUDP_BLCR_ENABLED
/* ------------------------------------------------------------------------------------
 *  checkpoint/restart
 * ------------------------------------------------------------------------------------ */
extern void AMUDP_SPMDRunRestart(char *argv0, char *dir, int nproc) {
  // BLCR-TODO: return errors on bad args?
  AMX_assert(argv0 != NULL);
  AMX_assert(dir != NULL);
  AMX_assert(nproc > 0);
  {
    eb_t eb; ep_t ep;
    int argc = 2;
    char **argv = (char**)AMX_malloc(3*sizeof(char*));
    argv[0] = argv0;
    argv[1] = dir;
    argv[2] = NULL;
    AMUDP_SPMDRestartActive = 1;
    AMUDP_SPMDStartup(&argc, &argv, nproc, 0, NULL, NULL, &eb, &ep);
    AMX_FatalErr("never reach here");
  }
}
extern int AMUDP_SPMDRestartProcId(int *argc, char ***argv) {
  const char *env_var = getenv(AMUDP_SPMDSLAVE_ARGS);
  const int slave_flag = env_var ? atoi(env_var) : 0;
  AMX_assert(argv != NULL);
  if (!AMUDP_SPMDStartupCalled && slave_flag == -1) {
    eb_t eb; ep_t ep;
    return AMUDP_SPMDStartup(argc, argv, 0, 0, NULL, NULL, &eb, &ep);
  }
  return -1;
}
static int AMUDP_SPMDReStartup(int fd, eb_t *eb, ep_t *ep) {
  struct stat st;
  int temp;

  AMUDP_SPMDRestartActive = 1;

  // Get location of new master from our special fd
  temp = fstat(fd, &st);
  if (temp < 0) {
    AMX_Err("Failed to read restart-master");
    exit(1);
  }
  size_t len = st.st_size;
  char *env_var = (char*)AMX_malloc(sizeof(char)*len);
  size_t rc = read(fd, env_var, len);
  if (rc != len) {
    AMX_Err("Failed to read restart env_var");
    AMX_RETURN(temp);
  }
  AMX_assert(env_var[len-1] == '\0');
  setenv(AMUDP_SPMDSLAVE_ARGS, env_var, 1);
  AMX_free(env_var);

  // Free old bundle
  temp = AM_FreeBundle(AMUDP_SPMDBundle);
  if (temp != AM_OK) {
    AMX_Err("Failed to free bundle in AMUDP_SPMDStartup");
    AMX_RETURN(temp);
  }

  // Re-bootstrap from the new master
  AMUDP_SPMDStartupCalled = 0;
  temp = AMUDP_SPMDStartup(NULL, NULL,
                           0, AMUDP_SPMDNetworkDepth,
                           NULL, NULL, eb, ep);

  AMUDP_SPMDRestartActive = 0;
  return temp;
}
/* ------------------------------------------------------------------------------------ */
int AMUDP_SPMDCheckpoint(eb_t *eb, ep_t *ep, const char *dir) {
  AMX_assert(dir != NULL);

  /* Drain all sends */
  for (int i = 0; i < AMUDP_SPMDBundle->n_endpoints; i++) {
    ep_t ep = AMUDP_SPMDBundle->endpoints[i];
    AMX_assert(ep);
    while (ep->outstandingRequests) {
      AM_Poll(AMUDP_SPMDBundle);
    }
  }
  AMUDP_SPMDBarrier();

  /* Start -- equivalent to gasnet_checkpoint_create(dir) */
  size_t len = strlen(dir) + 19; // 19 = "/context.123456789\0"
  char *buf = (char*)AMX_malloc(len);
  snprintf(buf, len, "%s/context.%d", dir, AMUDP_SPMDMYPROC);

  int contextFd = -1;
  {
    const int flags = O_WRONLY|O_APPEND|O_CREAT|O_EXCL|O_LARGEFILE|O_TRUNC;
    const int mode = S_IRUSR;
    contextFd = open(buf, flags, mode); // BLCR-TODO: error checking
    if (contextFd < 0) {
      AMX_Err("Failed to create '%s' errno=%d(%s)", buf, errno, strerror(errno));
      AMX_free(buf);
      return -1;
    }
  }
  AMX_free(buf);
  /* End -- equivalent to gasnet_checkpoint_create(dir) */

  // Open a "masterFd" socket, and write fileno to start of the context file
  int masterFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  write(contextFd, &masterFd, sizeof(masterFd));
  // BLCR-TODO: error checking for socket()

  /* Start -- equivalent to gasnet_checkpoint_write(contextFD) */
  cr_checkpoint_args_t cr_args;
  cr_checkpoint_handle_t cr_handle;
  int retval;
  int rc;

  cr_initialize_checkpoint_args_t(&cr_args);
  cr_args.cr_scope  = CR_SCOPE_TREE;
  cr_args.cr_target = 0; /* self */
  cr_args.cr_fd = contextFd;

  rc = cr_request_checkpoint(&cr_args, &cr_handle);
  // BLCR-TODO: error checking for cr_request_checkpoint()

  do { // This loop is necessary because checkpointing self causes EINTR
    rc = cr_wait_checkpoint(&cr_handle, NULL);
    // BLCR-TODO: error checking for cr_wait_checkpoint()
  } while ((rc < 0) && (errno == EINTR));

  rc = cr_reap_checkpoint(&cr_handle);
  if (rc >= 0) {
    (void)close(cr_args.cr_fd);
    retval = 0; // Continue case
  } else if (errno == CR_ERESTARTED) {
    retval = 1; // Restart case
  } else {
    retval = -1; // ERROR case
  }
  /* END -- equivalent to gasnet_checkpoint_write(contextFD) */

  if (0 > retval) {
    // Continue case
    // Nothing to do here
  } else if (1 == retval) {
    // Restart case
    AMUDP_SPMDReStartup(masterFd, eb, ep);
    // BLCR-TODO: error checking for ReStartup
  } else {
    // ERROR case
    // BLCR-TODO: error handling/reporting
  }

  (void)close(masterFd);

  return retval;
}
#endif // AMUDP_BLCR_ENABLED
/* ------------------------------------------------------------------------------------ */
