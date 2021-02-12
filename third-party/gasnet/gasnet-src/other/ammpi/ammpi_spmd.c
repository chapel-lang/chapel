/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/ammpi/ammpi_spmd.c $
 * Description: AMMPI Implementations of SPMD operations (bootstrapping and parallel job control)
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#if _FORTIFY_SOURCE > 0 && __OPTIMIZE__ <= 0 /* silence an annoying MPICH/Linux warning */
#undef _FORTIFY_SOURCE
#endif

#include <stdio.h>
#include <unistd.h>

#include <ammpi_spmd.h>
#include "ammpi_internal.h" /* must come after any other headers */

#ifndef FREEZE_WORKER
  #define FREEZE_WORKER  0
#endif

#if AMX_DEBUG_VERBOSE
  #define DEBUG_MSG(msg)  do { fprintf(stderr,"worker %i: %s\n", AMMPI_SPMDMYPROC, msg); fflush(stderr); } while(0)
#else
  #define DEBUG_MSG(msg)  do {} while(0) /* prevent silly warnings about empty statements */
#endif

static ep_t AMMPI_SPMDEndpoint = NULL;
static eb_t AMMPI_SPMDBundle = NULL;
static en_t AMMPI_SPMDName = {0};
static volatile int AMMPI_SPMDBarrierDone = 0; /* flag barrier as complete */
static volatile int AMMPI_SPMDBarrierCount = 0;
void AMMPI_SPMDHandleControlMessage(void *token, int32_t messageType, int32_t messageArg);
static int AMMPI_SPMDShutdown(int exitcode);
static ammpi_handler_fn_t AMMPI_SPMDControlMessageHandler = (ammpi_handler_fn_t)&AMMPI_SPMDHandleControlMessage;

static int AMMPI_SPMDStartupCalled = 0;
static int AMMPI_SPMDNUMPROCS = -1;
static int AMMPI_SPMDMYPROC = -1;
static MPI_Comm AMMPI_SPMDMPIComm;

/* ------------------------------------------------------------------------------------ 
 *  misc helpers
 * ------------------------------------------------------------------------------------ */
static void flushStreams(const char *context) {
  static int do_sync = -1;
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
  fsync(STDOUT_FILENO); /* ignore errors for output is a console */
  fsync(STDERR_FILENO); /* ignore errors for output is a console */

  if (do_sync < 0) {
    /* Approximate match to GASNet's acceptance of 'Y|YES|y|yes|1' */
    char c, *envval;
    envval = getenv("GASNET_FS_SYNC");
    if (NULL == envval) envval = getenv("AMMPI_FS_SYNC");
    c = envval ? envval[0] : '0';
    do_sync = ((c == '1') || (c == 'y') || (c == 'Y'));
  }
  if (do_sync) {
    sync();
  }
  AMX_sched_yield();
}
/* ------------------------------------------------------------------------------------ */
extern char *AMMPI_enStr(en_t en, char *buf) {
  static char pbuf[80];
  if (!buf) buf = pbuf;
  sprintf(buf, "(%i)", en.mpirank);
  return buf;
}
extern char *AMMPI_tagStr(tag_t tag, char *buf) {
  static char pbuf[80];
  if (!buf) buf = pbuf;
  sprintf(buf, "0x%08x%08x", 
    (unsigned int)(uint32_t)(tag >> 32), 
    (unsigned int)(uint32_t)(tag & 0xFFFFFFFF));
  return buf;
}
/* ------------------------------------------------------------------------------------ 
 *  basic inquiries
 * ------------------------------------------------------------------------------------ */
extern int AMMPI_SPMDNumProcs(void) {
  if (!AMMPI_SPMDStartupCalled) {
    AMX_Err("called AMMPI_SPMDNumProcs before AMMPI_SPMDStartup()");
    return -1;
  }
  AMX_assert(AMMPI_SPMDNUMPROCS >= 1);
  return AMMPI_SPMDNUMPROCS;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_SPMDMyProc(void) {
  if (!AMMPI_SPMDStartupCalled) {
    AMX_Err("called AMMPI_SPMDMyProc before AMMPI_SPMDStartup()");
    return -1;
  }
  AMX_assert(AMMPI_SPMDMYPROC >= 0);
  return AMMPI_SPMDMYPROC;
}
/* ------------------------------------------------------------------------------------ */
#ifndef HAVE_MPI_INIT_THREAD
#define HAVE_MPI_INIT_THREAD (MPI_VERSION >= 2)
#endif
#ifndef HAVE_MPI_QUERY_THREAD
#define HAVE_MPI_QUERY_THREAD (MPI_VERSION >= 2)
#endif
static int threadstr2int(const char *str) {
  char tmp[80];
  char *p;
  int ret = -1;
  strncpy(tmp, str, sizeof(tmp)-1);
  for (p = tmp; *p; p++) if (*p >= 'a' && *p <= 'z') *p -= 'a'-'A'; /* upper-case */
  #if HAVE_MPI_INIT_THREAD
    if (strstr(tmp,"SINGLE")) return MPI_THREAD_SINGLE;
    if (strstr(tmp,"FUNNELED")) return MPI_THREAD_FUNNELED;
    if (strstr(tmp,"SERIALIZED")) return MPI_THREAD_SERIALIZED;
    if (strstr(tmp,"MULTIPLE")) return MPI_THREAD_MULTIPLE;
    ret = MPI_THREAD_SINGLE-1;
  #endif
  return ret;
}
static const char *threadint2str(int id) {
  switch (id) {
  #if HAVE_MPI_INIT_THREAD
      case MPI_THREAD_SINGLE:     return "MPI_THREAD_SINGLE";
      case MPI_THREAD_FUNNELED:   return "MPI_THREAD_FUNNELED";
      case MPI_THREAD_SERIALIZED: return "MPI_THREAD_SERIALIZED";
      case MPI_THREAD_MULTIPLE:   return "MPI_THREAD_MULTIPLE";
      default: return "UNKNOWN VALUE";
  #endif
  }
}
extern int AMMPI_SPMDSetThreadMode(int usingthreads, const char **provided_level, int *argc, char ***argv) {
  int initialized = 0;
  if (AMMPI_SPMDStartupCalled) AMX_RETURN_ERR(RESOURCE);
  #if !HAVE_MPI_INIT_THREAD
    *provided_level = "MPI-1 compatibility mode";
    return 1;
  #else
    { int required = (usingthreads ? MPI_THREAD_SERIALIZED : MPI_THREAD_SINGLE);
      int provided = -1;
      const char *override = getenv("AMMPI_MPI_THREAD");
      if (!override) override = getenv("GASNET_MPI_THREAD");
      if (override) {
        int overreq = threadstr2int(override);
        if (overreq >= MPI_THREAD_SINGLE) required = overreq;
        else { fprintf(stderr,"WARNING: Ignoring unrecognized GASNET_MPI_THREAD value."); fflush(stderr); }
      }
      MPI_SAFE(MPI_Initialized(&initialized));
      if (initialized) {  /* MPI already init, query current thread support level */
        #if HAVE_MPI_QUERY_THREAD
          MPI_SAFE(MPI_Query_thread(&provided));
        #else
          provided = required;
        #endif
      } else { /* init MPI and request our needed level of thread safety */
        MPI_SAFE(MPI_Init_thread(argc, argv, required, &provided));
      }
      *provided_level = threadint2str(provided);
      return (provided >= required);
    }
  #endif
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_SPMDStartup(int *argc, char ***argv,
                             int networkdepth, 
                             uint64_t *networkpid,
                             eb_t *eb, ep_t *ep) {
  uint64_t prvnetworkpid;
  tag_t mytag;
  
  if (AMMPI_SPMDStartupCalled) AMX_RETURN_ERR(RESOURCE);

  { /* initialize MPI, if necessary */
    int initialized = 0;
    MPI_SAFE(MPI_Initialized(&initialized));
    if (!initialized) {
  #if MPI_VERSION < 2
      if (!argc || !argv) AMX_RETURN_ERR(BAD_ARG);
  #endif
      MPI_SAFE(MPI_Init(argc, argv));
    }
  }

  /* defaulting */
  if (networkdepth < 0) AMX_RETURN_ERR(BAD_ARG);
  if (networkdepth == 0) {
    const char *netdepth_str = getenv("AMMPI_NETWORKDEPTH");
    if (netdepth_str) networkdepth = atoi(netdepth_str);
    if (networkdepth <= 0) networkdepth = AMMPI_DEFAULT_NETWORKDEPTH;
  }

  #if FREEZE_WORKER
    AMX_freezeForDebugger();
  #else
    if (getenv("AMMPI_FREEZE")) AMX_freezeForDebugger();
  #endif

  if (!eb || !ep) AMX_RETURN_ERR(BAD_ARG);
  if (AM_Init() != AM_OK) {
    AMX_Err("Failed to AM_Init() in AMMPI_SPMDStartup");
    AMX_RETURN_ERRFR(RESOURCE, AMMPI_SPMDStartup, "AM_Init() failed");
  }

  MPI_SAFE(MPI_Comm_rank(MPI_COMM_WORLD, &AMMPI_SPMDMYPROC));
  MPI_SAFE(MPI_Comm_size(MPI_COMM_WORLD, &AMMPI_SPMDNUMPROCS));
  AMX_assert(AMMPI_SPMDNUMPROCS > 0);
  AMX_assert(AMMPI_SPMDMYPROC >= 0 && AMMPI_SPMDMYPROC < AMMPI_SPMDNUMPROCS);
  if (!AMX_ProcessLabel) {
    static char label[80];
    snprintf(label,sizeof(label),"Node %i",AMMPI_SPMDMYPROC);
    AMX_ProcessLabel = label;
  }

  { /* check job size */
    int temp, maxtranslations = 0;
    if (AMMPI_SPMDNUMPROCS > AMMPI_MAX_SPMDPROCS) {
      AMX_Err("Too many MPI nodes: %d (max is %d)", AMMPI_SPMDNUMPROCS, AMMPI_MAX_SPMDPROCS);
      AMX_RETURN_ERR(RESOURCE);
    }
    temp = AM_MaxNumTranslations(&maxtranslations);
    if (temp != AM_OK) {
      AMX_Err("Failed to AM_MaxNumTranslations() in AMMPI_SPMDStartup");
      AMX_RETURN(temp);
    } else if (AMMPI_SPMDNUMPROCS > maxtranslations) {
      AMX_Err("Too many nodes: AM_MaxNumTranslations (%d) less than number of MPI nodes (%d)",
              maxtranslations, AMMPI_SPMDNUMPROCS);
      AMX_RETURN_ERR(RESOURCE);
    }
  }

  #if AMX_DEBUG_VERBOSE
    fprintf(stderr, "worker %i/%i starting...\n", AMMPI_SPMDMYPROC, AMMPI_SPMDNUMPROCS);
    fflush(stderr);
  #endif

  { /* setup comm for isolation */
    MPI_Group world_group;
    MPI_SAFE(MPI_Comm_group(MPI_COMM_WORLD, &world_group));
    MPI_SAFE(MPI_Comm_create(MPI_COMM_WORLD, world_group, &AMMPI_SPMDMPIComm));
    MPI_SAFE(MPI_Group_free(&world_group));
  }

  { int mypid = getpid();
    int networkpidtemp = 0;
    if (!mypid) mypid = (int)AMX_getMicrosecondTimeStamp() | 0x1; /* ensure nonzero pid */
    AMX_assert(mypid);
    MPI_SAFE(MPI_Allreduce(&mypid, &networkpidtemp, 1, MPI_INT, MPI_SUM, AMMPI_SPMDMPIComm));
    prvnetworkpid = (uint64_t)networkpidtemp;
    mytag = (((tag_t)prvnetworkpid) << 32 ) | (tag_t)AMMPI_SPMDMYPROC;
    AMX_assert(mytag != AM_ALL);
    AMX_assert(mytag != AM_NONE);
  }

  { /* create endpoint and get name */
    int temp = AM_AllocateBundle(AM_SEQ, &AMMPI_SPMDBundle);
    if (temp != AM_OK) {
      AMX_Err("Failed to create bundle in AMMPI_SPMDStartup");
      AMX_RETURN(temp);
    }
  
    temp = AM_AllocateEndpoint(AMMPI_SPMDBundle, &AMMPI_SPMDEndpoint, &AMMPI_SPMDName);
    if (temp != AM_OK) {
      AMX_Err("Failed to create endpoint in AMMPI_SPMDStartup");
      AMX_RETURN(temp);
    }
  }
  
  { int i, temp;
    en_t *namebuf = AMX_malloc(sizeof(en_t)*AMMPI_SPMDNUMPROCS);
    tag_t *tagbuf = AMX_malloc(sizeof(tag_t)*AMMPI_SPMDNUMPROCS);

    /* gather names */
    MPI_SAFE(MPI_Allgather(&AMMPI_SPMDName, sizeof(en_t), MPI_BYTE, 
                           namebuf, sizeof(en_t), MPI_BYTE, 
                           AMMPI_SPMDMPIComm));
    /* gather tags */
    MPI_SAFE(MPI_Allgather(&mytag, sizeof(tag_t), MPI_BYTE, 
                           tagbuf, sizeof(tag_t), MPI_BYTE, 
                           AMMPI_SPMDMPIComm));

    /* setup translation table */
    temp = AM_SetNumTranslations(AMMPI_SPMDEndpoint, AMMPI_SPMDNUMPROCS);
    if (temp != AM_OK) {
      AMX_Err("Failed to AM_SetNumTranslations() in AMMPI_SPMDStartup");
      AMX_RETURN(temp);
    }
    for (i = 0; i < AMMPI_SPMDNUMPROCS; i++) {
      temp = AM_Map(AMMPI_SPMDEndpoint, i, namebuf[i], tagbuf[i]);
      if (temp != AM_OK) {
        AMX_Err("Failed to AM_Map() in AMMPI_SPMDStartup");
        AMX_RETURN(temp);
      }
    }
    AMX_free(namebuf);
    AMX_free(tagbuf);
  }

  { /* allocate network buffers */
    int temp = AM_SetExpectedResources(AMMPI_SPMDEndpoint, AMMPI_SPMDNUMPROCS, networkdepth);
    if (temp != AM_OK) {
      AMX_Err("Failed to AM_SetExpectedResources() in AMMPI_SPMDStartup");
      AMX_RETURN(temp);
    }
  }

  { /* set tag */
    int temp = AM_SetTag(AMMPI_SPMDEndpoint, mytag);
    if (temp != AM_OK) {
      AMX_Err("Failed to AM_SetTag() in AMMPI_SPMDStartup");
      AMX_RETURN(temp);
    }
  }

  AMMPI_RegisterControlMessageHandler(AMMPI_SPMDEndpoint, AMMPI_SPMDControlMessageHandler);

  *eb = AMMPI_SPMDBundle;
  *ep = AMMPI_SPMDEndpoint;
  if (networkpid)
    *networkpid = prvnetworkpid;
  AMMPI_SPMDStartupCalled = 1;

  MPI_SAFE(MPI_Barrier(AMMPI_SPMDMPIComm)); /* wait for all control handlers to be registered */

  #if AMX_DEBUG_VERBOSE
  { char temp[80];
    tag_t tag;
    AM_GetTag(AMMPI_SPMDEndpoint, &tag);
    fprintf(stderr, "worker %i/%i starting (tag=%s)...\n", 
      AMMPI_SPMDMyProc(), AMMPI_SPMDNumProcs(), AMMPI_tagStr(tag, temp));
    fflush(stderr);
  }
  #endif

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
void AMMPI_SPMDHandleControlMessage(void *token, int32_t messageType, int32_t messageArg) {
  switch (messageType) {
    case 'R': { /*  barrier ready */
      AMX_assert(AMMPI_SPMDMYPROC == 0);
      AMMPI_SPMDBarrierCount++; 
      break;
    }
    case 'B': { /*  barrier complete */
      AMX_assert(AMMPI_SPMDMYPROC != 0);
      AMX_assert(AMMPI_SPMDBarrierDone == 0);
      AMMPI_SPMDBarrierDone = 1; 
      break;
    }
    case 'E': { /*  exit */
      AMMPI_SPMDShutdown(messageArg);
      break;
    }
    default: 
      AMX_Err("unrecognized AMMPI SPMD control message - ignoring..."); 
  }
}

/* ------------------------------------------------------------------------------------ 
 *  process termination
 * ------------------------------------------------------------------------------------ */
static void (*AMMPI_SPMDExitCallback)(int) = NULL;
extern int AMMPI_SPMDSetExitCallback(void (*fp)(int)) {
  AMMPI_SPMDExitCallback = fp;
  return AM_OK;
}
void (*AMMPI_SPMDkillmyprocess)(int) = &_exit;

static int AMMPI_SPMDShutdown(int exitcode) {
  /* this function is not re-entrant - if someone tries, something is seriously wrong */
  { static int shutdownInProgress = FALSE;
    if (shutdownInProgress) AMX_FatalErr("recursion failure in AMMPI_SPMDShutdown"); 
    shutdownInProgress = TRUE;
  }

  if (AMMPI_SPMDExitCallback) (*AMMPI_SPMDExitCallback)(exitcode);

  flushStreams("AMMPI_SPMDExit");

 #if !PLATFORM_OS_CNL /* multi-node CNL hangs on exit if you close the streams */
  if (fclose(stdin)) {
  #if AMX_DEBUG_VERBOSE
    AMX_Err("failed to fclose stdin in AMMPI_SPMDExit()"); 
    perror("fclose");
  #endif
  }
  if (fclose(stdout)) {
  #if AMX_DEBUG_VERBOSE
    AMX_Err("failed to fclose stdout in AMMPI_SPMDExit()"); 
    perror("fclose");
  #endif
  }
  if (fclose(stderr)) {
  #if AMX_DEBUG_VERBOSE
    AMX_Err("failed to fclose stderr in AMMPI_SPMDExit()"); 
    perror("fclose");
  #endif
  }
 #endif

  AMX_sched_yield();

  if (AM_Terminate() != AM_OK) 
    AMX_Err("failed to AM_Terminate() in AMMPI_SPMDExit()");

  #if 0
    MPI_SAFE(MPI_Abort(AMMPI_SPMDMPIComm, exitcode));
  #endif

  MPI_SAFE(MPI_Comm_free(&AMMPI_SPMDMPIComm));
  AMMPI_SPMDMPIComm = MPI_COMM_WORLD;

  MPI_SAFE(MPI_Finalize());

  AMMPI_SPMDStartupCalled = 0;
  DEBUG_MSG("exiting..");
  AMMPI_SPMDkillmyprocess(exitcode);
  AMX_FatalErr("AMMPI_SPMDkillmyprocess failed");
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_SPMDExit(int exitcode) {
  int i;
  if (!AMMPI_SPMDStartupCalled) AMX_RETURN_ERR(NOT_INIT);

  /* this function is not re-entrant - if someone tries, something is seriously wrong */
  { static int exitInProgress = FALSE;
    if (exitInProgress) AMX_FatalErr("recursion failure in AMMPI_SPMDExit"); 
    exitInProgress = TRUE;
  }

  /* tell others to exit */
  for (i = 0; i < AMMPI_SPMDNUMPROCS; i++) {
    en_t remoteName;
    if (AM_GetTranslationName(AMMPI_SPMDEndpoint, i, &remoteName) == AM_OK &&
        !AMMPI_enEqual(remoteName, AMMPI_SPMDName)) {
      if (AMMPI_SendControlMessage(AMMPI_SPMDEndpoint, remoteName, 2, (int32_t)'E', (int32_t)exitcode) != AM_OK)
        AMX_Err("Failed to AMMPI_SendControlMessage in AMMPI_SPMDExit()");
    }
  }

  /* exit this proc */
  AMMPI_SPMDShutdown(exitcode);
  AMX_FatalErr("AMMPI_SPMDShutdown failed");
  return 0;
}
/* ------------------------------------------------------------------------------------ 
 *  barrier
 * ------------------------------------------------------------------------------------ */
extern int AMMPI_SPMDBarrier(void) {
  int oldmask;
  #if !AMMPI_BLOCKING_SPMD_BARRIER
    uint32_t timeoutusec = 100;
  #endif

  if (!AMMPI_SPMDStartupCalled) {
    AMX_Err("called AMMPI_SPMDBarrier before AMMPI_SPMDStartup()");
    AMX_RETURN_ERR(NOT_INIT);
  }

  flushStreams("AMMPI_SPMDBarrier");

  /* Barrier algorithm:
   *  everybody but 0 sends a ready signal to 0 (who counts them)
   *  0 sends back a done signal when everybody has reported
   */
  AMX_assert(AMMPI_SPMDBarrierDone == 0);
  AM_GetEventMask(AMMPI_SPMDBundle, &oldmask);

  if (AMMPI_SPMDMYPROC == 0) { /* proc zero */
    int i;
    if (AMMPI_SPMDNUMPROCS == 1) return AM_OK;

    AMMPI_SPMDBarrierCount++; /*  count myself */

    /*  wait for each processor to report */
    AM_Poll(AMMPI_SPMDBundle);
    while (AMMPI_SPMDBarrierCount != AMMPI_SPMDNUMPROCS) {
      #if AMMPI_BLOCKING_SPMD_BARRIER
        AM_SetEventMask(AMMPI_SPMDBundle, AM_NOTEMPTY);
        AM_WaitSema(AMMPI_SPMDBundle);
        AM_Poll(AMMPI_SPMDBundle);
      #else
        AMX_usleep(timeoutusec);
        AM_Poll(AMMPI_SPMDBundle);
        if (timeoutusec < 10000) timeoutusec *= 2;
      #endif
    }
    AMMPI_SPMDBarrierCount = 0; /*  reset */

    /*  report success to others */
    for (i = 1; i < AMMPI_SPMDNUMPROCS; i++) {
      en_t remoteName;
      if (AM_GetTranslationName(AMMPI_SPMDEndpoint, i, &remoteName) != AM_OK)
        AMX_RETURN_ERR(RESOURCE);
      if (AMMPI_SendControlMessage(AMMPI_SPMDEndpoint, remoteName, 2, (int32_t)'B', (int32_t)0) != AM_OK)
        AMX_RETURN_ERR(RESOURCE);
    }
  } else { /* proc non-zero */
    en_t remoteName;
    if (AM_GetTranslationName(AMMPI_SPMDEndpoint, 0, &remoteName) != AM_OK)
      AMX_RETURN_ERR(RESOURCE);

    /*  signal zero */
    if (AMMPI_SendControlMessage(AMMPI_SPMDEndpoint, remoteName, 2, (int32_t)'R', (int32_t)0) != AM_OK)
      AMX_RETURN_ERR(RESOURCE);

    /*  wait for completion */
    AM_Poll(AMMPI_SPMDBundle);
    while (!AMMPI_SPMDBarrierDone) {
      #if AMMPI_BLOCKING_SPMD_BARRIER
        AM_SetEventMask(AMMPI_SPMDBundle, AM_NOTEMPTY);
        AM_WaitSema(AMMPI_SPMDBundle);
        AM_Poll(AMMPI_SPMDBundle);
      #else
        AMX_usleep(timeoutusec);
        AM_Poll(AMMPI_SPMDBundle);
        if (timeoutusec < 10000) timeoutusec *= 2;
      #endif
    }
    AMMPI_SPMDBarrierDone = 0; /*  reset */
  }

  AM_SetEventMask(AMMPI_SPMDBundle, oldmask);
  DEBUG_MSG("Leaving barrier");
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ 
 *  bootstrapping helpers
 * ------------------------------------------------------------------------------------ */
extern int AMMPI_SPMDAllGather(void *source, void *dest, size_t len) {
  if (!AMMPI_SPMDStartupCalled) AMX_RETURN_ERR(RESOURCE);

  MPI_SAFE(MPI_Allgather(source, len, MPI_BYTE,
                         dest, len, MPI_BYTE,
                         AMMPI_SPMDMPIComm));

  return AM_OK;
}
extern int AMMPI_SPMDBroadcast(void *buf, size_t len, int rootid) {
  if (!AMMPI_SPMDStartupCalled) AMX_RETURN_ERR(RESOURCE);

  MPI_SAFE(MPI_Bcast(buf, len, MPI_BYTE,
                     rootid,
                     AMMPI_SPMDMPIComm));

  return AM_OK;
}

/* ------------------------------------------------------------------------------------ */
