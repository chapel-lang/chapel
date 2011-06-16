/*   $Source: /var/local/cvs/gasnet/other/amxtests/testlatencyM.c,v $
 *     $Date: 2006/10/17 13:19:12 $
 * $Revision: 1.14 $
 * Description: AMX test
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include "apputils.h"

/* non-pipelined version of ping tester, using AMMediums of a given size */

#define PING_REQ_HANDLER 1
#define PING_REP_HANDLER 2

static volatile int numleft;

int myproc;
int numprocs;
eb_t eb;
ep_t ep;

static void ping_request_handler(void *token, void *msg, int nbytes) {
  numleft--;

  #if VERBOSE
    printf("%i: ping_request_handler(). sending reply...\n", myproc); fflush(stdout);
  #endif

  AM_Safe(AM_Reply0(token, PING_REP_HANDLER));
}

static void ping_reply_handler(void *token) {

  #if VERBOSE
    printf("%i: ping_reply_handler()\n", myproc); fflush(stdout);
  #endif

  numleft--;
}

void mywait(int polling) {
  if (polling) { /* poll until everyone done */
    while (numleft) {
      AM_Safe(AM_Poll(eb));
    }
  } else {
    while (numleft) {
      AM_Safe(AM_SetEventMask(eb, AM_NOTEMPTY)); 
      AM_Safe(AM_WaitSema(eb));
      AM_Safe(AM_Poll(eb));
    }
  }
}

/* usage: testlatency  numprocs  spawnfn  iters  P/B msgsz
 */
int main(int argc, char **argv) {
  uint64_t networkpid;
  int64_t begin, end, total;
  int polling = 1;
  int k;
  int iters = 0;
  int msgsz = 0;
  char *msg=NULL;

  TEST_STARTUP(argc, argv, networkpid, eb, ep, 1, 3, "iters (Poll/Block) (msgsize)");

  /* setup handlers */
  AM_Safe(AM_SetHandler(ep, PING_REQ_HANDLER, ping_request_handler));
  AM_Safe(AM_SetHandler(ep, PING_REP_HANDLER, ping_reply_handler));

  setupUtilHandlers(ep, eb);

  /* get SPMD info */
  myproc = AMX_SPMDMyProc();
  numprocs = AMX_SPMDNumProcs();

  if (argc > 1) iters = atoi(argv[1]);
  if (!iters) iters = 1;
  if (argc > 2) {
    switch(argv[2][0]) {
      case 'p': case 'P': polling = 1; break;
      case 'b': case 'B': polling = 0; break;
      default: printf("polling must be 'P' or 'B'..\n"); AMX_SPMDExit(1);
    }
  }

  if (argc > 3) msgsz = atoi(argv[3]);
  if (!msgsz) msgsz = 1;

  outputTimerStats();

  AM_Safe(AMX_SPMDBarrier());

  if (myproc == 0) printf("Running %i iterations of latency test (MSGSZ=%i)...\n", iters, msgsz);
  if (myproc == 0 && numprocs > 1) numleft = (numprocs-1)*iters;
  AM_Safe(AMX_SPMDBarrier());

  msg = (char *)calloc(1,msgsz);

  begin = getCurrentTimeMicrosec();

  if (myproc == 0 && numprocs > 1) {
    mywait(polling);
  } else { /* everybody sends packets to 0 */
    int expect = (numprocs > 1 ? 1 : 2);
    for (k=0;k < iters; k++) {
      numleft = expect;
      #if VERBOSE
        printf("%i: sending request...", myproc); fflush(stdout);
      #endif
      AM_Safe(AM_RequestI0(ep, 0, PING_REQ_HANDLER, msg, msgsz));
      mywait(polling);
    }
  }
  
  end = getCurrentTimeMicrosec();

  total = end - begin;
  if (myproc != 0 || numprocs == 1) printf("Slave %i: %i microseconds total, throughput: %i requests/sec (%.3f us / request)\n", 
    myproc, (int)total, (int)(((float)1000000)*iters/((int)total)), ((double)total)/iters);
  else printf("Slave 0 done.\n");
  fflush(stdout);

  /* dump stats */
  AM_Safe(AMX_SPMDBarrier());
  printGlobalStats();
  AM_Safe(AMX_SPMDBarrier());

  /* exit */
  AM_Safe(AMX_SPMDExit(0));

  return 0;
}
/* ------------------------------------------------------------------------------------ */
