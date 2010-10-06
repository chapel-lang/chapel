/*   $Source: /var/local/cvs/gasnet/other/ammpi/ammpi_spmd.c,v $
 *     $Date: 2009/03/30 02:40:43 $
 * $Revision: 1.42 $
 * Description: AMMPI Implementations of SPMD operations (bootstrapping and parallel job control)
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#include <ammpi_internal.h>
#include <ammpi_spmd.h>

#include <stdio.h>
#ifdef WIN32
  #include <process.h>
#else
  #include <unistd.h>
  #if (PLATFORM_OS_LINUX || PLATFORM_OS_UCLINUX) && !defined(__USE_GNU)
    /* some Linuxes need this to pull in F_SETSIG */
    #define __USE_GNU
    #include <fcntl.h>
    #undef __USE_GNU
  #else
    #include <fcntl.h>
  #endif
#endif


#define FD_STDIN 0
#define FD_STDOUT 1
#define FD_STDERR 2

#ifndef FREEZE_SLAVE
  #define FREEZE_SLAVE  0
#endif
/* non-static to prevent a SunC problem (bug1626) */
volatile int ammpi_frozen = TRUE;
/*  all this to make sure we get a full stack frame for debugger */
static void _freezeForDebugger(int depth) {
  if (!depth) _freezeForDebugger(1);
  else {
    volatile int i = 0;
    while (ammpi_frozen) {
      i++;
      ammpi_sched_yield();
    }
  }
}
static void freezeForDebugger(void) {
  char name[255];
  gethostname(name, 255);
  fprintf(stderr,"slave frozen for debugger: host=%s  pid=%i\n", name, getpid()); fflush(stderr);
  _freezeForDebugger(0);
}

#if AMMPI_DEBUG_VERBOSE
  #define DEBUG_MSG(msg)  do { fprintf(stderr,"slave %i: %s\n", AMMPI_SPMDMYPROC, msg); fflush(stderr); } while(0)
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
  if (!context) context = "flushStreams()";

  if (fflush(NULL)) { /* passing NULL to fflush causes it to flush all open FILE streams */
    perror("fflush");
    AMMPI_FatalErr("failed to fflush(NULL) in %s", context); 
  }
  if (fflush(stdout)) {
    perror("fflush");
    AMMPI_FatalErr("failed to flush stdout in %s", context); 
  }
  if (fflush(stderr)) {
    perror("fflush");
    AMMPI_FatalErr("failed to flush stderr in %s", context); 
  }
  fsync(STDOUT_FILENO); /* ignore errors for output is a console */
  fsync(STDERR_FILENO); /* ignore errors for output is a console */
  #if PLATFORM_OS_MTA
    mta_sync();
  #elif !PLATFORM_OS_CATAMOUNT
    sync();
  #endif
  ammpi_sched_yield();
}
/* ------------------------------------------------------------------------------------ */
extern char *AMMPI_enStr(en_t en, char *buf) {
  AMMPI_assert(buf);
  sprintf(buf, "(%i)", en.mpirank);
  return buf;
}
extern char *AMMPI_tagStr(tag_t tag, char *buf) {
  AMMPI_assert(buf);
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
    AMMPI_Err("called AMMPI_SPMDNumProcs before AMMPI_SPMDStartup()");
    return -1;
  }
  AMMPI_assert(AMMPI_SPMDNUMPROCS >= 1);
  return AMMPI_SPMDNUMPROCS;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_SPMDMyProc(void) {
  if (!AMMPI_SPMDStartupCalled) {
    AMMPI_Err("called AMMPI_SPMDMyProc before AMMPI_SPMDStartup()");
    return -1;
  }
  AMMPI_assert(AMMPI_SPMDMYPROC >= 0);
  return AMMPI_SPMDMYPROC;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_SPMDSetThreadMode(int usingthreads, const char **provided_level, int *argc, char ***argv) {
  int initialized = 0;
  if (AMMPI_SPMDStartupCalled) AMMPI_RETURN_ERR(RESOURCE);
  MPI_SAFE(MPI_Initialized(&initialized));
  if (initialized) { 
    *provided_level = "MPI already initialized";
    return 1; /* MPI already initialized */
  }
  #if MPI_VERSION >= 2 || (defined(MPI_THREAD_SINGLE) && defined(MPI_THREAD_SERIALIZED))
    { /* init MPI and tell it to be thread-safe */
      int required = (usingthreads ? MPI_THREAD_SERIALIZED : MPI_THREAD_SINGLE);
      int provided = -1;
      MPI_SAFE(MPI_Init_thread(argc, argv, required, &provided));
      switch (provided) {
      #ifdef MPI_THREAD_SINGLE
        case MPI_THREAD_SINGLE: *provided_level = "MPI_THREAD_SINGLE"; break;
      #endif
      #ifdef MPI_THREAD_FUNNELED
        case MPI_THREAD_FUNNELED: *provided_level = "MPI_THREAD_FUNNELED"; break;
      #endif
      #ifdef MPI_THREAD_SERIALIZED
        case MPI_THREAD_SERIALIZED: *provided_level = "MPI_THREAD_SERIALIZED"; break;
      #endif
      #ifdef MPI_THREAD_MULTIPLE
        case MPI_THREAD_MULTIPLE: *provided_level = "MPI_THREAD_MULTIPLE"; break;
      #endif
        default: *provided_level = "UNKNOWN VALUE";
      }
      return (provided >= required);
    }
  #else
    *provided_level = "MPI-1 compatibility mode";
    return 1;
  #endif
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_SPMDStartup(int *argc, char ***argv,
                             int networkdepth, 
                             uint64_t *networkpid,
                             eb_t *eb, ep_t *ep) {
  uint64_t prvnetworkpid;
  tag_t mytag;
  
  if (AMMPI_SPMDStartupCalled) AMMPI_RETURN_ERR(RESOURCE);
  if (!argc || !argv) AMMPI_RETURN_ERR(BAD_ARG);

  { /* initialize MPI, if necessary */
    int initialized = 0;
    MPI_SAFE(MPI_Initialized(&initialized));
    if (!initialized) {
      MPI_SAFE(MPI_Init(argc, argv));
    }
  }

  /* defaulting */
  if (networkdepth < 0) AMMPI_RETURN_ERR(BAD_ARG);
  if (networkdepth == 0) {
    const char *netdepth_str = getenv("AMMPI_NETWORKDEPTH");
    if (netdepth_str) networkdepth = atoi(netdepth_str);
    if (networkdepth <= 0) networkdepth = AMMPI_DEFAULT_NETWORKDEPTH;
  }

  #if FREEZE_SLAVE
    freezeForDebugger();
  #else
    if (getenv("AMMPI_FREEZE")) freezeForDebugger();
  #endif

  if (!eb || !ep) AMMPI_RETURN_ERR(BAD_ARG);
  if (AM_Init() != AM_OK) {
    AMMPI_Err("Failed to AM_Init() in AMMPI_SPMDStartup");
    AMMPI_RETURN_ERRFR(RESOURCE, AMMPI_SPMDStartup, "AM_Init() failed");
  }

  MPI_SAFE(MPI_Comm_rank(MPI_COMM_WORLD, &AMMPI_SPMDMYPROC));
  MPI_SAFE(MPI_Comm_size(MPI_COMM_WORLD, &AMMPI_SPMDNUMPROCS));
  AMMPI_assert(AMMPI_SPMDNUMPROCS > 0);
  AMMPI_assert(AMMPI_SPMDMYPROC >= 0 && AMMPI_SPMDMYPROC < AMMPI_SPMDNUMPROCS);

  { /* check job size */
    int temp, maxtranslations = 0;
    if (AMMPI_SPMDNUMPROCS > AMMPI_MAX_SPMDPROCS) {
      AMMPI_Err("Too many MPI nodes: %d (max is %d)", AMMPI_SPMDNUMPROCS, AMMPI_MAX_SPMDPROCS);
      AMMPI_RETURN_ERR(RESOURCE);
    }
    temp = AM_MaxNumTranslations(&maxtranslations);
    if (temp != AM_OK) {
      AMMPI_Err("Failed to AM_MaxNumTranslations() in AMMPI_SPMDStartup");
      AMMPI_RETURN(temp);
    } else if (AMMPI_SPMDNUMPROCS > maxtranslations) {
      AMMPI_Err("Too many nodes: AM_MaxNumTranslations (%d) less than number of MPI nodes (%d)",
              maxtranslations, AMMPI_SPMDNUMPROCS);
      AMMPI_RETURN_ERR(RESOURCE);
    }
  }

  #if AMMPI_DEBUG_VERBOSE
    fprintf(stderr, "slave %i/%i starting...\n", AMMPI_SPMDMYPROC, AMMPI_SPMDNUMPROCS);
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
    if (!mypid) mypid = (int)AMMPI_getMicrosecondTimeStamp() | 0x1; /* ensure nonzero pid */
    AMMPI_assert(mypid);
    MPI_SAFE(MPI_Allreduce(&mypid, &networkpidtemp, 1, MPI_INT, MPI_SUM, AMMPI_SPMDMPIComm));
    prvnetworkpid = (uint64_t)networkpidtemp;
    mytag = (((tag_t)prvnetworkpid) << 32 ) | (tag_t)AMMPI_SPMDMYPROC;
    AMMPI_assert(mytag != AM_ALL);
    AMMPI_assert(mytag != AM_NONE);
  }

  { /* create endpoint and get name */
    int temp = AM_AllocateBundle(AM_SEQ, &AMMPI_SPMDBundle);
    if (temp != AM_OK) {
      AMMPI_Err("Failed to create bundle in AMMPI_SPMDStartup");
      AMMPI_RETURN(temp);
    }
  
    temp = AM_AllocateEndpoint(AMMPI_SPMDBundle, &AMMPI_SPMDEndpoint, &AMMPI_SPMDName);
    if (temp != AM_OK) {
      AMMPI_Err("Failed to create endpoint in AMMPI_SPMDStartup");
      AMMPI_RETURN(temp);
    }
  }
  
  { int i, temp;
    en_t *namebuf = AMMPI_malloc(sizeof(en_t)*AMMPI_SPMDNUMPROCS);
    tag_t *tagbuf = AMMPI_malloc(sizeof(tag_t)*AMMPI_SPMDNUMPROCS);

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
      AMMPI_Err("Failed to AM_SetNumTranslations() in AMMPI_SPMDStartup");
      AMMPI_RETURN(temp);
    }
    for (i = 0; i < AMMPI_SPMDNUMPROCS; i++) {
      temp = AM_Map(AMMPI_SPMDEndpoint, i, namebuf[i], tagbuf[i]);
      if (temp != AM_OK) {
        AMMPI_Err("Failed to AM_Map() in AMMPI_SPMDStartup");
        AMMPI_RETURN(temp);
      }
    }
    AMMPI_free(namebuf);
    AMMPI_free(tagbuf);
  }

  { /* allocate network buffers */
    int temp = AM_SetExpectedResources(AMMPI_SPMDEndpoint, AMMPI_SPMDNUMPROCS, networkdepth);
    if (temp != AM_OK) {
      AMMPI_Err("Failed to AM_SetExpectedResources() in AMMPI_SPMDStartup");
      AMMPI_RETURN(temp);
    }
  }

  { /* set tag */
    int temp = AM_SetTag(AMMPI_SPMDEndpoint, mytag);
    if (temp != AM_OK) {
      AMMPI_Err("Failed to AM_SetTag() in AMMPI_SPMDStartup");
      AMMPI_RETURN(temp);
    }
  }

  AMMPI_RegisterControlMessageHandler(AMMPI_SPMDEndpoint, AMMPI_SPMDControlMessageHandler);

  *eb = AMMPI_SPMDBundle;
  *ep = AMMPI_SPMDEndpoint;
  if (networkpid)
    *networkpid = prvnetworkpid;
  AMMPI_SPMDStartupCalled = 1;

  MPI_SAFE(MPI_Barrier(AMMPI_SPMDMPIComm)); /* wait for all control handlers to be registered */

  #if AMMPI_DEBUG_VERBOSE
  { char temp[80];
    tag_t tag;
    AM_GetTag(AMMPI_SPMDEndpoint, &tag);
    fprintf(stderr, "Slave %i/%i starting (tag=%s)...\n", 
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
      AMMPI_assert(AMMPI_SPMDMYPROC == 0);
      AMMPI_SPMDBarrierCount++; 
      break;
    }
    case 'B': { /*  barrier complete */
      AMMPI_assert(AMMPI_SPMDMYPROC != 0);
      AMMPI_assert(AMMPI_SPMDBarrierDone == 0);
      AMMPI_SPMDBarrierDone = 1; 
      break;
    }
    case 'E': { /*  exit */
      AMMPI_SPMDShutdown(messageArg);
      break;
    }
    default: 
      AMMPI_Err("unrecognized AMMPI SPMD control message - ignoring..."); 
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
    if (shutdownInProgress) AMMPI_FatalErr("recursion failure in AMMPI_SPMDShutdown"); 
    shutdownInProgress = TRUE;
  }

  if (AMMPI_SPMDExitCallback) (*AMMPI_SPMDExitCallback)(exitcode);

  flushStreams("AMMPI_SPMDExit");

 #if !PLATFORM_OS_CNL /* multi-node CNL hangs on exit if you close the streams */
  if (fclose(stdin)) {
  #if AMMPI_DEBUG_VERBOSE
    AMMPI_Err("failed to fclose stdin in AMMPI_SPMDExit()"); 
    perror("fclose");
  #endif
  }
  if (fclose(stdout)) {
  #if AMMPI_DEBUG_VERBOSE
    AMMPI_Err("failed to fclose stdout in AMMPI_SPMDExit()"); 
    perror("fclose");
  #endif
  }
  if (fclose(stderr)) {
  #if AMMPI_DEBUG_VERBOSE
    AMMPI_Err("failed to fclose stderr in AMMPI_SPMDExit()"); 
    perror("fclose");
  #endif
  }
 #endif

  ammpi_sched_yield();

  if (AM_Terminate() != AM_OK) 
    AMMPI_Err("failed to AM_Terminate() in AMMPI_SPMDExit()");

  #if 0
    MPI_SAFE(MPI_Abort(AMMPI_SPMDMPIComm, exitcode));
  #endif

  MPI_SAFE(MPI_Comm_free(&AMMPI_SPMDMPIComm));
  AMMPI_SPMDMPIComm = MPI_COMM_WORLD;

  MPI_SAFE(MPI_Finalize());

  AMMPI_SPMDStartupCalled = 0;
  DEBUG_MSG("exiting..");
  AMMPI_SPMDkillmyprocess(exitcode);
  AMMPI_FatalErr("AMMPI_SPMDkillmyprocess failed");
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_SPMDExit(int exitcode) {
  int i;
  if (!AMMPI_SPMDStartupCalled) AMMPI_RETURN_ERR(NOT_INIT);

  /* this function is not re-entrant - if someone tries, something is seriously wrong */
  { static int exitInProgress = FALSE;
    if (exitInProgress) AMMPI_FatalErr("recursion failure in AMMPI_SPMDExit"); 
    exitInProgress = TRUE;
  }

  /* tell others to exit */
  for (i = 0; i < AMMPI_SPMDNUMPROCS; i++) {
    en_t remoteName;
    if (AM_GetTranslationName(AMMPI_SPMDEndpoint, i, &remoteName) == AM_OK &&
        !AMMPI_enEqual(remoteName, AMMPI_SPMDName)) {
      if (AMMPI_SendControlMessage(AMMPI_SPMDEndpoint, remoteName, 2, (int32_t)'E', (int32_t)exitcode) != AM_OK)
        AMMPI_Err("Failed to AMMPI_SendControlMessage in AMMPI_SPMDExit()");
    }
  }

  /* exit this proc */
  AMMPI_SPMDShutdown(exitcode);
  AMMPI_FatalErr("AMMPI_SPMDShutdown failed");
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
    AMMPI_Err("called AMMPI_SPMDBarrier before AMMPI_SPMDStartup()");
    AMMPI_RETURN_ERR(NOT_INIT);
  }

  flushStreams("AMMPI_SPMDBarrier");

  /* Barrier algorithm:
   *  everybody but 0 sends a ready signal to 0 (who counts them)
   *  0 sends back a done signal when everybody has reported
   */
  AMMPI_assert(AMMPI_SPMDBarrierDone == 0);
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
        ammpi_usleep(timeoutusec);
        AM_Poll(AMMPI_SPMDBundle);
        if (timeoutusec < 10000) timeoutusec *= 2;
      #endif
    }
    AMMPI_SPMDBarrierCount = 0; /*  reset */

    /*  report success to others */
    for (i = 1; i < AMMPI_SPMDNUMPROCS; i++) {
      en_t remoteName;
      if (AM_GetTranslationName(AMMPI_SPMDEndpoint, i, &remoteName) != AM_OK)
        AMMPI_RETURN_ERR(RESOURCE);
      if (AMMPI_SendControlMessage(AMMPI_SPMDEndpoint, remoteName, 2, (int32_t)'B', (int32_t)0) != AM_OK)
        AMMPI_RETURN_ERR(RESOURCE);
    }
  } else { /* proc non-zero */
    en_t remoteName;
    if (AM_GetTranslationName(AMMPI_SPMDEndpoint, 0, &remoteName) != AM_OK)
      AMMPI_RETURN_ERR(RESOURCE);

    /*  signal zero */
    if (AMMPI_SendControlMessage(AMMPI_SPMDEndpoint, remoteName, 2, (int32_t)'R', (int32_t)0) != AM_OK)
      AMMPI_RETURN_ERR(RESOURCE);

    /*  wait for completion */
    AM_Poll(AMMPI_SPMDBundle);
    while (!AMMPI_SPMDBarrierDone) {
      #if AMMPI_BLOCKING_SPMD_BARRIER
        AM_SetEventMask(AMMPI_SPMDBundle, AM_NOTEMPTY);
        AM_WaitSema(AMMPI_SPMDBundle);
        AM_Poll(AMMPI_SPMDBundle);
      #else
        ammpi_usleep(timeoutusec);
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
  if (!AMMPI_SPMDStartupCalled) AMMPI_RETURN_ERR(RESOURCE);

  MPI_SAFE(MPI_Allgather(source, len, MPI_BYTE,
                         dest, len, MPI_BYTE,
                         AMMPI_SPMDMPIComm));

  return AM_OK;
}
extern int AMMPI_SPMDBroadcast(void *buf, size_t len, int rootid) {
  if (!AMMPI_SPMDStartupCalled) AMMPI_RETURN_ERR(RESOURCE);

  MPI_SAFE(MPI_Bcast(buf, len, MPI_BYTE,
                     rootid,
                     AMMPI_SPMDMPIComm));

  return AM_OK;
}

/* ------------------------------------------------------------------------------------ */
