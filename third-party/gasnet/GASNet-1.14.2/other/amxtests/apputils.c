/*   $Source: /var/local/cvs/gasnet/other/amxtests/apputils.c,v $
 *     $Date: 2009/03/30 02:40:47 $
 * $Revision: 1.18 $
 * Description: AMX Application utilities
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#include "apputils.h"
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
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
static void stats_request_handler(void *token, void *buf, int nbytes, int32_t procnum) {
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
#ifndef UETH
#ifdef WIN32
  int64_t getCurrentTimeMicrosec(void) {
    static int status = -1;
    static double multiplier;
    if (status == -1) { /*  first time run */
      LARGE_INTEGER freq;
      if (!QueryPerformanceFrequency(&freq)) status = 0; /*  don't have high-perf counter */
      else {
        multiplier = 1000000 / (double)freq.QuadPart;
        status = 1;
      }
    }
    if (status) { /*  we have a high-performance counter */
      LARGE_INTEGER count;
      QueryPerformanceCounter(&count);
      return (int64_t)(multiplier * count.QuadPart);
    } else { /*  no high-performance counter */
      /*  this is a millisecond-granularity timer that wraps every 50 days */
      return (GetTickCount() * 1000);
    }
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
#define REQ_32BITPTRS() \
  if (sizeof(void *) != 4) AMX_FatalErr("This test not supported on 64-bit ptr architectures.")
/* ------------------------------------------------------------------------------------ */
#ifndef APPUTILS_OMIT_READWRITE
/*  synchronous gets and puts */
static void get_reply_handler(void *token, int ctr, int dest, int val) {
  uint32_t *pctr = (uint32_t *)(uintptr_t)ctr;
  uint32_t *pdest = (uint32_t *)(uintptr_t)dest;
  
  assert(pctr);
  assert(pdest);
  *pdest = (uint32_t)val;
  *pctr = TRUE;
}

static void get_request_handler(void *token, int ctr, int dest, int addr) {
  uint32_t *paddr = (uint32_t *)(uintptr_t)addr;

  assert(paddr);

  AM_Safe(AM_Reply3(token, GET_REP_HANDLER, 
                    ctr, dest, *paddr));
}

uint32_t getWord(int proc, void *addr) {
  volatile uint32_t getdone = FALSE;
  volatile uint32_t getval = 0;
  REQ_32BITPTRS();
  AM_Safe(AM_Request3(ep, proc, GET_REQ_HANDLER, 
                     (int)(uintptr_t)&getdone, (int)(uintptr_t)&getval, 
                     (int)(uintptr_t)addr));
  while (!getdone) AM_PollBlock(eb);
  return getval;
}
/* ------------------------------------------------------------------------------------ */
static void put_reply_handler(void *token, int ctr) {
  uint32_t *pctr = (uint32_t *)(uintptr_t)ctr;
  assert(pctr);
  *pctr = TRUE;
}

static void put_request_handler(void *token, int ctr, int dest, int val) {
  uint32_t *paddr = (uint32_t *)(uintptr_t)dest;
  assert(paddr);
  *paddr = (uint32_t)val;

  AM_Safe(AM_Reply1(token, PUT_REP_HANDLER, 
                    ctr));
}

void putWord(int proc, void *addr, uint32_t val) {
  volatile uint32_t putdone = FALSE;
  REQ_32BITPTRS();
  AM_Safe(AM_Request3(ep, proc, PUT_REQ_HANDLER, 
                      (int)(uintptr_t)&putdone, (int)(uintptr_t)addr, 
                      (int)val));
  while (!putdone) AM_PollBlock(eb);
  return;
}
/* ------------------------------------------------------------------------------------ */
/*  asynchronous reads and writes */
static volatile uint32_t readCtr = 0;
static void read_reply_handler(void *token, int ctr, int dest, int val) {
  uint32_t *pctr = (uint32_t *)(uintptr_t)ctr;
  uint32_t *pdest = (uint32_t *)(uintptr_t)dest;

  assert(pctr);
  assert(pdest);
  *pdest = (uint32_t)val;
  (*pctr)--;
}

static void read_request_handler(void *token, int ctr, int dest, int addr) {
  uint32_t *paddr = (uint32_t *)(uintptr_t)addr;
  assert(paddr);

  AM_Safe(AM_Reply3(token, READ_REP_HANDLER, 
                    ctr, dest, *paddr));
}


void readWord(void *destaddr, int proc, void *addr) {
  REQ_32BITPTRS();
  AM_Safe(AM_Request3(ep, proc, READ_REQ_HANDLER, 
                      (int)(uintptr_t)&readCtr, (int)(uintptr_t)destaddr, 
                      (int)(uintptr_t)addr));
  readCtr++;
  return;
}

void readSync(void) {
  while (readCtr) AM_PollBlock(eb);
}
/* ------------------------------------------------------------------------------------ */
static volatile uint32_t writeCtr = 0;
static void write_reply_handler(void *token, int ctr) {
  uint32_t *pctr = (uint32_t *)(uintptr_t)ctr;
  assert(pctr);
  (*pctr)--;
}

static void write_request_handler(void *token, int ctr, int dest, int val) {
  uint32_t *paddr = (uint32_t *)(uintptr_t)dest;
  assert(paddr);
  *paddr = (uint32_t)val;

  AM_Safe(AM_Reply1(token, WRITE_REP_HANDLER, 
                    ctr));
}

void writeWord(int proc, void *addr, uint32_t val) {
  REQ_32BITPTRS();
  AM_Safe(AM_Request3(ep, proc, WRITE_REQ_HANDLER, 
                      (int)(uintptr_t)&writeCtr, (int)(uintptr_t)addr, (int)val));
  writeCtr++;
  return;
}

void writeSync(void) {
  while (writeCtr) AM_PollBlock(eb);
}
#endif
/* ------------------------------------------------------------------------------------ */
void free_resource_handler(int sig) {
  sleep(2);
  AMX_SPMDExit(-1);
}
/* ------------------------------------------------------------------------------------ */
void setupUtilHandlers(ep_t activeep, eb_t activeeb) {
  assert(activeep && activeeb);
  ep = activeep;
  eb = activeeb;

  AM_Safe(AM_SetHandler(ep, STATS_REQ_HANDLER, (amx_handler_fn_t)stats_request_handler));

#ifndef APPUTILS_OMIT_READWRITE
 if (sizeof(void*) == 4) {
  AM_Safe(AM_SetHandler(ep, GET_REQ_HANDLER, (amx_handler_fn_t)get_request_handler));
  AM_Safe(AM_SetHandler(ep, GET_REP_HANDLER, (amx_handler_fn_t)get_reply_handler));
  AM_Safe(AM_SetHandler(ep, PUT_REQ_HANDLER, (amx_handler_fn_t)put_request_handler));
  AM_Safe(AM_SetHandler(ep, PUT_REP_HANDLER, (amx_handler_fn_t)put_reply_handler));

  AM_Safe(AM_SetHandler(ep, READ_REQ_HANDLER, (amx_handler_fn_t)read_request_handler));
  AM_Safe(AM_SetHandler(ep, READ_REP_HANDLER, (amx_handler_fn_t)read_reply_handler));
  AM_Safe(AM_SetHandler(ep, WRITE_REQ_HANDLER, (amx_handler_fn_t)write_request_handler));
  AM_Safe(AM_SetHandler(ep, WRITE_REP_HANDLER, (amx_handler_fn_t)write_reply_handler));
 }
#endif

  #if !PLATFORM_OS_MSWINDOWS
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
  #endif
}
/* ------------------------------------------------------------------------------------ */
