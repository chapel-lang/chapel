/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amx/apputils.c $
 * Description: AMX Application utilities
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#include "apputils.h"
#if HAVE_GASNET_TOOLS
  #define GASNETT_LITE_MODE
  #include <gasnet_tools.h>
#else
  #include <time.h>
  #include <sys/time.h>
#endif
#include <unistd.h>
#include <sys/select.h>
#include <signal.h>
#include <stdlib.h>

/*  init by setupUtilHandlers */
static ep_t ep = NULL;
static eb_t eb = NULL;

/* all handler indices are defined here and registered at bottom of file */

#define STATS_REQ_HANDLER     (APPUTIL_HANDLER_BASE+0)

#define GET_REQ_HANDLER       (APPUTIL_HANDLER_BASE+1)
#define GET_REP_HANDLER       (APPUTIL_HANDLER_BASE+2)
#define PUT_REQ_HANDLER       (APPUTIL_HANDLER_BASE+3)
#define PUT_REP_HANDLER       (APPUTIL_HANDLER_BASE+4)

#define READ_REQ_HANDLER      (APPUTIL_HANDLER_BASE+5)
#define READ_REP_HANDLER      (APPUTIL_HANDLER_BASE+6)
#define WRITE_REQ_HANDLER     (APPUTIL_HANDLER_BASE+7)
#define WRITE_REP_HANDLER     (APPUTIL_HANDLER_BASE+8)


/* ------------------------------------------------------------------------------------ */
/*  statistics dump */
/* ------------------------------------------------------------------------------------ */
static int statscalls = 0;
static amx_stats_t globalStats;
static void stats_request_handler(void *token, void *buf, size_t nbytes, int32_t procnum) {
  assert(nbytes == sizeof(amx_stats_t));
  AM_Safe(AMX_AggregateStatistics(&globalStats, (amx_stats_t *)buf));
  statscalls++;
}

void printGlobalStats(void) {
  amx_stats_t stats;
  statscalls = 0; 
  globalStats = AMX_initial_stats;

  AM_Safe(AMX_SPMDBarrier()); /* make sure we're done sending msgs for now */

  { /* problem - we use messages to implement above barrier -
     * make sure P0 gets all the barrier replies, or above counts won't match up
     */
    int64_t time = getCurrentTimeMicrosec();
    while ((int)(getCurrentTimeMicrosec() - time) < 10000) {
      sleep(0);
      AM_Safe(AM_Poll(eb)); 
    }
  }

  AM_Safe(AMX_GetEndpointStatistics(ep, &stats)); /* get statistics */
  AM_Safe(AMX_SPMDBarrier()); /* don't let stats msgs interfere */

  assert(sizeof(amx_stats_t) < AM_MaxMedium());
  AM_Safe(AM_RequestI1(ep, 0, STATS_REQ_HANDLER, &stats, sizeof(amx_stats_t), AMX_SPMDMyProc())); /* send to zero */

  if (AMX_SPMDMyProc() == 0) {
    uint32_t timeoutusec = 100;
    while (statscalls < AMX_SPMDNumProcs()) {
      #if USE_BLOCKING_SPMD_BARRIER
        AM_Safe(AM_SetEventMask(eb, AM_NOTEMPTY));
        AM_Safe(AM_WaitSema(eb));
        AM_Safe(AM_Poll(eb));
      #else
        struct timeval tv;
        tv.tv_sec  = timeoutusec / 1000000;
        tv.tv_usec = timeoutusec % 1000000;
        select(1, NULL, NULL, NULL, &tv); /* sleep a little while */

        AM_Safe(AM_Poll(eb));
        if (timeoutusec < 10000) timeoutusec *= 2;
      #endif
    }
    fprintf(stderr, "--------------------------------------------------\n"
                    "Global stats:\n");
    AMX_DumpStatistics(stderr, &globalStats, 1);
    fprintf(stderr, "--------------------------------------------------\n");
    fflush(stderr);
    sleep(1); /* HACK: give a little time for this output to reach master */
  }

  AM_Safe(AMX_SPMDBarrier()); /* just to keep things clean */

}
/* ------------------------------------------------------------------------------------ */
#if HAVE_GASNET_TOOLS
  int64_t getCurrentTimeMicrosec(void) {
    gasnett_tick_t now = gasnett_ticks_now();
    int64_t retval = gasnett_ticks_to_ns(now)/1000;
    return retval;
  }
#else
  int64_t getCurrentTimeMicrosec(void) {
    int64_t retval;
    struct timeval tv;
    if (gettimeofday(&tv, NULL))
      AMX_FatalErr("gettimeofday failed: %s",strerror(errno));
    retval = ((int64_t)tv.tv_sec) * 1000000 + tv.tv_usec;
    return retval;
  }
#endif
/* ------------------------------------------------------------------------------------ */
extern void outputTimerStats(void) {
  int iters = 10000;
  int64_t starttime = getCurrentTimeMicrosec();
  int64_t lasttime = starttime;
  int32_t mindiff = 1 << 30;
  
  int i;
  for (i=0; i < iters; i++) {
    int64_t time = getCurrentTimeMicrosec();
    int32_t diff = (int32_t)(time - lasttime);
    if (diff < mindiff) mindiff = diff;
    lasttime = time;
  }

  { int64_t endtime = getCurrentTimeMicrosec();
    double avgtime = ((int32_t)(endtime-starttime))/((double)(iters+1));
    double granularity = avgtime - mindiff;
    fprintf(stdout, "Timer overhead:      %f us per call\n"
                    "Timer mindiff:       %i us\n"
                    "Timer granularity: +-%f us\n", 
                    (float)avgtime, (int)mindiff, (float)granularity);
    fflush(stdout);
  }
}
/* ------------------------------------------------------------------------------------ */
#define MAKEDWORD(hi,lo) ((((uint64_t)(hi)) << 32) | (((uint64_t)(lo)) & 0xFFFFFFFF))
#undef  HIWORD /* prevent a conflict on cygwin */
#define HIWORD(arg)      ((uint32_t)(((uint64_t)(uintptr_t)(arg)) >> 32))
#undef  LOWORD /* prevent a conflict on cygwin */
#define LOWORD(arg)      ((uint32_t)((uint64_t)(uintptr_t)(arg) & 0xFFFFFFFF))

/*  synchronous gets and puts */
static void get_reply_handler(void *token, int ctrH, int ctrL, int destH, int destL, int val) {
  uint32_t *pctr = (uint32_t *)(uintptr_t)MAKEDWORD(ctrH,ctrL);
  uint32_t *pdest = (uint32_t *)(uintptr_t)MAKEDWORD(destH,destL);
  
  assert(pctr);
  assert(pdest);
  *pdest = (uint32_t)val;
  *pctr = TRUE;
}

static void get_request_handler(void *token, int ctrH, int ctrL, int destH, int destL, int addrH, int addrL) {
  uint32_t *paddr = (uint32_t *)(uintptr_t)MAKEDWORD(addrH,addrL);

  assert(paddr);

  AM_Safe(AM_Reply5(token, GET_REP_HANDLER, 
                    ctrH, ctrL, destH, destL, *paddr));
}

uint32_t getWord(int proc, void *addr) {
  volatile uint32_t getdone = FALSE;
  volatile uint32_t getval = 0;

  AM_Safe(AM_Request6(ep, proc, GET_REQ_HANDLER, 
                     HIWORD(&getdone), LOWORD(&getdone),
                     HIWORD(&getval), LOWORD(&getval),
                     HIWORD(addr), LOWORD(addr)));

  while (!getdone) AM_PollBlock(eb);
  return getval;
}
/* ------------------------------------------------------------------------------------ */
static void put_reply_handler(void *token, int ctrH, int ctrL) {
  uint32_t *pctr = (uint32_t *)(uintptr_t)MAKEDWORD(ctrH,ctrL);
  assert(pctr);
  *pctr = TRUE;
}

static void put_request_handler(void *token, int ctrH, int ctrL, int destH, int destL, int val) {
  uint32_t *paddr = (uint32_t *)(uintptr_t)MAKEDWORD(destH,destL);
  assert(paddr);
  *paddr = (uint32_t)val;

  AM_Safe(AM_Reply2(token, PUT_REP_HANDLER, 
                    ctrH, ctrL));
}

void putWord(int proc, void *addr, uint32_t val) {
  volatile uint32_t putdone = FALSE;

  AM_Safe(AM_Request5(ep, proc, PUT_REQ_HANDLER, 
                      HIWORD(&putdone), LOWORD(&putdone),
                      HIWORD(addr), LOWORD(addr),
                      (int)val));

  while (!putdone) AM_PollBlock(eb);
  return;
}
/* ------------------------------------------------------------------------------------ */
/*  asynchronous reads and writes */
static volatile uint32_t readCtr = 0;
static void read_reply_handler(void *token, int ctrH, int ctrL, int destH, int destL, int val) {
  uint32_t *pctr = (uint32_t *)(uintptr_t)MAKEDWORD(ctrH,ctrL);
  uint32_t *pdest = (uint32_t *)(uintptr_t)MAKEDWORD(destH,destL);

  assert(pctr);
  assert(pdest);
  *pdest = (uint32_t)val;
  (*pctr)--;
}

static void read_request_handler(void *token, int ctrH, int ctrL, int destH, int destL, int addrH, int addrL) {
  uint32_t *paddr = (uint32_t *)(uintptr_t)MAKEDWORD(addrH,addrL);
  assert(paddr);

  AM_Safe(AM_Reply5(token, READ_REP_HANDLER, 
                    ctrH, ctrL, destH, destL, *paddr));
}


void readWord(void *destaddr, int proc, void *addr) {
  readCtr++;

  AM_Safe(AM_Request6(ep, proc, READ_REQ_HANDLER, 
                     HIWORD(&readCtr), LOWORD(&readCtr),
                     HIWORD(destaddr), LOWORD(destaddr),
                     HIWORD(addr), LOWORD(addr)));
}

void readSync(void) {
  while (readCtr) AM_PollBlock(eb);
}
/* ------------------------------------------------------------------------------------ */
static volatile uint32_t writeCtr = 0;
static void write_reply_handler(void *token, int ctrH, int ctrL) {
  uint32_t *pctr = (uint32_t *)(uintptr_t)MAKEDWORD(ctrH,ctrL);
  assert(pctr);
  (*pctr)--;
}

static void write_request_handler(void *token, int ctrH, int ctrL, int destH, int destL, int val) {
  uint32_t *paddr = (uint32_t *)(uintptr_t)MAKEDWORD(destH,destL);
  assert(paddr);
  *paddr = (uint32_t)val;

  AM_Safe(AM_Reply2(token, WRITE_REP_HANDLER, 
                    ctrH, ctrL));
}

void writeWord(int proc, void *addr, uint32_t val) {
  writeCtr++;

  AM_Safe(AM_Request5(ep, proc, WRITE_REQ_HANDLER, 
                      HIWORD(&writeCtr), LOWORD(&writeCtr),
                      HIWORD(addr), LOWORD(addr),
                      (int)val));
}

void writeSync(void) {
  while (writeCtr) AM_PollBlock(eb);
}
/* ------------------------------------------------------------------------------------ */
void free_resource_handler(int sig) {
    static int first = 1;
    if (first) {
      /* Avoid recursion if a fatal signal is raised while exiting */
      signal (SIGHUP,  SIG_DFL);
      signal (SIGINT,  SIG_DFL);
      signal (SIGQUIT, SIG_DFL);
      signal (SIGTERM, SIG_DFL);
      signal (SIGABRT, SIG_DFL);
      signal (SIGFPE,  SIG_DFL);
      signal (SIGSEGV, SIG_DFL);
      signal (SIGBUS,  SIG_DFL);

      switch (sig) {
        case SIGABRT: case SIGFPE: case SIGSEGV: case SIGBUS:
        { /* Signal context may call write(), but not fprintf() */
          char digits[] = "0123456789";
          char msg[] = "XXXX: Terminating on fatal signal XX\n";
          const size_t len = sizeof(msg); /* Includes \n and \0 */
          int myproc = AMX_SPMDMyProc();
          static int ignoreerr;
          msg[0] = (myproc < 1000) ? ' ' : digits[(myproc / 1000) % 10];
          msg[1] = (myproc < 100 ) ? ' ' : digits[(myproc / 100 ) % 10];
          msg[2] = (myproc < 10  ) ? ' ' : digits[(myproc / 10  ) % 10];
          msg[3] =                         digits[(myproc       ) % 10];
          msg[len-3] = digits[sig % 10];
          msg[len-4] = (sig < 10) ? ' ' : digits[sig / 10];
          ignoreerr += write(STDERR_FILENO, msg, len - 1);
        }
      }
      first = 0;
    }
    
  sleep(2);
  AMX_SPMDExit(-1);
}
/* ------------------------------------------------------------------------------------ */
void setupUtilHandlers(ep_t activeep, eb_t activeeb) {
  assert(activeep && activeeb);
  ep = activeep;
  eb = activeeb;

  AM_Safe(AM_SetHandler(ep, STATS_REQ_HANDLER, (amx_handler_fn_t)stats_request_handler));

  AM_Safe(AM_SetHandler(ep, GET_REQ_HANDLER, (amx_handler_fn_t)get_request_handler));
  AM_Safe(AM_SetHandler(ep, GET_REP_HANDLER, (amx_handler_fn_t)get_reply_handler));
  AM_Safe(AM_SetHandler(ep, PUT_REQ_HANDLER, (amx_handler_fn_t)put_request_handler));
  AM_Safe(AM_SetHandler(ep, PUT_REP_HANDLER, (amx_handler_fn_t)put_reply_handler));

  AM_Safe(AM_SetHandler(ep, READ_REQ_HANDLER, (amx_handler_fn_t)read_request_handler));
  AM_Safe(AM_SetHandler(ep, READ_REP_HANDLER, (amx_handler_fn_t)read_reply_handler));
  AM_Safe(AM_SetHandler(ep, WRITE_REQ_HANDLER, (amx_handler_fn_t)write_request_handler));
  AM_Safe(AM_SetHandler(ep, WRITE_REP_HANDLER, (amx_handler_fn_t)write_reply_handler));

    /* some MPI implementations don't cleanup well and leave orphaned nodes
     * if we allow a node to crash without shutting down properly 
     */
    signal (SIGHUP,  free_resource_handler);
    signal (SIGINT,  free_resource_handler);
    signal (SIGQUIT, free_resource_handler);
    signal (SIGTERM, free_resource_handler);
    signal (SIGABRT, free_resource_handler);
    signal (SIGFPE,  free_resource_handler);
    signal (SIGSEGV, free_resource_handler);
    signal (SIGBUS, free_resource_handler);
}
/* ------------------------------------------------------------------------------------ */
