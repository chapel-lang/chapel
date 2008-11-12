/*   $Source: /var/local/cvs/gasnet/other/amxtests/testping.c,v $
 *     $Date: 2006/04/11 03:23:45 $
 * $Revision: 1.10 $
 * Description: AMX test
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include "apputils.h"

#define PING_REQ_HANDLER 1
#define PING_REP_HANDLER 2

static volatile int numleft;

int myproc;
int numprocs;

static void ping_request_handler(void *token) {
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

/* usage: testping  numprocs  spawnfn  iters  P/B
 */
int main(int argc, char **argv) {
  eb_t eb;
  ep_t ep;
  uint64_t networkpid;
  int64_t begin, end, total;
  int polling = 1;
  int k;
  int iters = 0;

  TEST_STARTUP(argc, argv, networkpid, eb, ep, 1, 2, "iters (Poll/Block)");

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

  if (numprocs == 1) numleft = 2*iters;
  else if (myproc == 0) numleft = (numprocs-1)*iters;
  else numleft = iters;

  AM_Safe(AMX_SPMDBarrier());

  if (myproc == 0) printf("Running %i iterations of ping test...\n", iters);

  begin = getCurrentTimeMicrosec();

  if (myproc != 0 || numprocs == 1) { /* everybody sends packets to 0 */
    for (k=0;k < iters; k++) {
      #if VERBOSE
        printf("%i: sending request...", myproc); fflush(stdout);
      #endif
      AM_Safe(AM_Request0(ep, 0, PING_REQ_HANDLER));
    }
  }

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
