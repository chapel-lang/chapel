/*   $Source: /var/local/cvs/gasnet/other/amxtests/testbounce.c,v $
 *     $Date: 2006/05/11 09:43:42 $
 * $Revision: 1.10 $
 * Description: AMX test
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include "apputils.h"


#define SMALL_REQ_HANDLER 1
#define SMALL_REP_HANDLER 2
#define MEDIUM_REQ_HANDLER 3
#define MEDIUM_REP_HANDLER 4
#define LARGE_REQ_HANDLER 5
#define LARGE_REP_HANDLER 6

int myproc;
int numprocs;
int partner;

volatile int count = 0;

volatile int done = 0;
uint32_t *VMseg;

static void large_request_handler(void *token, void *buf, int nbytes, int arg) {
  uint32_t *recvdbuf = (uint32_t *)buf;
  #if VERBOSE
    printf("%i: large_request_handler(). starting...", myproc); fflush(stdout);
  #endif

  assert(arg == 666);
  assert(buf == ((uint8_t *)VMseg) + 100);
  assert(nbytes == AM_MaxLong());

  /*  verify the result */
  { int i;
    for (i = 0; i < AM_MaxLong()/4; i++) {
      if (recvdbuf[i] != (uint32_t)((count << 16) + i))
        AMX_FatalErr("%i: ERROR: mismatched data recvdbuf[%i]=%i\n", myproc, i, (int)recvdbuf[i]);
    }
    count++;
    for (i = 0; i < AM_MaxLong()/4; i++) {
      recvdbuf[i] = (uint32_t)((count << 16) + i);
    }
    if (numprocs > 1) count++;
  }

  #if VERBOSE
    printf("%i: large_request_handler(). sending reply...", myproc); fflush(stdout);
  #endif


  AM_Safe(AM_ReplyXfer1(token, 100, LARGE_REP_HANDLER, buf, nbytes, 666));
  done++;
}

static void large_reply_handler(void *token, void *buf, int nbytes, int arg) {
  uint32_t *recvdbuf = (uint32_t *)buf;
  /* assert(done < 2*nummsgs); */

  #if VERBOSE
    printf("%i: bulk_reply_handler()\n", myproc); fflush(stdout);
  #endif

  assert(arg == 666);
  assert(buf == ((uint8_t *)VMseg) + 100);
  assert(nbytes == AM_MaxLong());

  if (numprocs > 1) count++;
  /*  verify the result */
  { int i;
    for (i = 0; i < AM_MaxLong()/4; i++) {
      if (recvdbuf[i] != (uint32_t)((count << 16) + i))
        AMX_FatalErr("%i: ERROR: mismatched data recvdbuf[%i]=%i\n", myproc, i, (int)recvdbuf[i]);
    }
  }
  count++;

  done = 1;
}

int main(int argc, char **argv) {
  eb_t eb;
  ep_t ep;
  uint64_t networkpid;
  int64_t begin, end, total;
  int polling = 1;
  int iters = 0;

  TEST_STARTUP(argc, argv, networkpid, eb, ep, 1, 2, "iters (Poll/Block)");

  /* setup handlers */
  AM_Safe(AM_SetHandler(ep, LARGE_REQ_HANDLER, large_request_handler));
  AM_Safe(AM_SetHandler(ep, LARGE_REP_HANDLER, large_reply_handler));

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
  if (numprocs % 2 != 0 && numprocs > 1) {
     printf("requires an even or unary number of processors\n"); AMX_SPMDExit(1);
  }
  VMseg = (uint32_t *)malloc(AM_MaxLong()+100);
  memset(VMseg, 0, AM_MaxLong()+100);
  AM_Safe(AM_SetSeg(ep, VMseg, AM_MaxLong()+100));

  if (myproc % 2 == 0) partner = (myproc + 1) % numprocs;
  else partner = (myproc - 1);

  AM_Safe(AMX_SPMDBarrier());

  if (myproc == 0) printf("Running %i iterations of bulk bounce test...\n", iters);

  begin = getCurrentTimeMicrosec();

  if (myproc % 2 == 1 || numprocs == 1) {
    int q;
    for (q=0; q<iters; q++) {
      /*  init my source mem */
      int i;
      uint32_t *srcmem = (uint32_t *)(((uint8_t*)VMseg)+100);
      for (i = 0; i < AM_MaxLong()/4; i++) {
        srcmem[i] = (uint32_t)((count << 16) + i);
      }
      #if VERBOSE
	printf("%i: sending request...", myproc); fflush(stdout);
      #endif
      
      done = 0;
      
      AM_Safe(AM_RequestXfer1(ep, partner, 100, LARGE_REQ_HANDLER, srcmem, AM_MaxLong(), 666));

      if (polling) { /* poll until everyone done */
        while (!done) {
          AM_Safe(AM_Poll(eb));
        }
      } else {
        while (!done) {
          AM_Safe(AM_SetEventMask(eb, AM_NOTEMPTY)); 
          AM_Safe(AM_WaitSema(eb));
          AM_Safe(AM_Poll(eb));
        }
      }
    }
  } else {
    if (polling) { /* poll until everyone done */
      while (count<iters*2) {
        AM_Safe(AM_Poll(eb));
      }
    } else {
      while (count<iters*2) {
        AM_Safe(AM_SetEventMask(eb, AM_NOTEMPTY)); 
        AM_Safe(AM_WaitSema(eb));
        AM_Safe(AM_Poll(eb));
      }
    }
  }

  end = getCurrentTimeMicrosec();

  total = end - begin;
  printf("Slave %i: %i microseconds total, throughput: %8.3f KB/sec\n", 
    myproc, (int)total, (float)(((float)1000000)*AM_MaxLong()*iters/((int)total))/1024.0);
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
