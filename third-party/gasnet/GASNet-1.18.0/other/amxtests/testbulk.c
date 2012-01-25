/*   $Source: /var/local/cvs/gasnet/other/amxtests/testbulk.c,v $
 *     $Date: 2009/04/02 17:15:55 $
 * $Revision: 1.13 $
 * Description: AMX test
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include "apputils.h"


#define BULK_REQ_HANDLER 1
#define BULK_REP_HANDLER 2

int myproc;
int numprocs;

int size = -1;
int msg_size = 0;

int nummsgs = 0;

int done = 0;
uint32_t *VMseg;

static void bulk_request_handler(void *token, void *buf, int nbytes, int arg) {
  #if DEBUG
  uint32_t *recvdbuf = (uint32_t *)buf;
  #endif
  #if VERBOSE
    printf("%i: bulk_request_handler(). starting...\n", myproc); fflush(stdout);
  #endif

  assert(arg == 666);
  assert(nbytes == size % AM_MaxLong() || nbytes == AM_MaxLong());
  assert(buf == ((uint8_t *)VMseg) + 100);
  /* assert(done < 2*nummsgs); */

  #if DEBUG
    /*  verify the result */
    { int i;
      for (i = 0; i < nbytes/4; i++) {
        if (recvdbuf[i] != (uint32_t)i) 
          AMX_FatalErr("%i: ERROR: mismatched data recvdbuf[%i]=%i\n", myproc, i, (int)recvdbuf[i]);
      }
    }
  #endif

  #if VERBOSE
    printf("%i: bulk_request_handler(). sending reply...\n", myproc); fflush(stdout);
  #endif


  AM_Safe(AM_Reply0(token, BULK_REP_HANDLER));
  done++;
}

static void bulk_reply_handler(void *token, int ctr, int dest, int val) {
  /* assert(done < 2*nummsgs); */

  #if VERBOSE
    printf("%i: bulk_reply_handler()\n", myproc); fflush(stdout);
  #endif
  done++;
}

int main(int argc, char **argv) {
  eb_t eb;
  ep_t ep;
  uint64_t networkpid;
  int64_t begin, end, total;
  int polling = 1;
  int fullduplex = 0;
  int rightguy;
  uint32_t *srcmem;
  int iters = 0;

  TEST_STARTUP(argc, argv, networkpid, eb, ep, 1, 4, "iters (bulkmsgsize) (Poll/Block) (Half/Full)");

  /* setup handlers */
  AM_Safe(AM_SetHandler(ep, BULK_REQ_HANDLER, bulk_request_handler));
  AM_Safe(AM_SetHandler(ep, BULK_REP_HANDLER, bulk_reply_handler));

  setupUtilHandlers(ep, eb);

  /* get SPMD info */
  myproc = AMX_SPMDMyProc();
  numprocs = AMX_SPMDNumProcs();

  if (argc > 1) iters = atoi(argv[1]);
  if (!iters) iters = 1;
  if (argc > 2) size = atoi(argv[2]);
  if (size == -1) size = 512;
  if (argc > 3) {
    switch(argv[3][0]) {
      case 'p': case 'P': polling = 1; break;
      case 'b': case 'B': polling = 0; break;
      default: printf("polling must be 'P' or 'B'..\n"); AMX_SPMDExit(1);
    }
  }
  if (argc > 4) {
    switch(argv[4][0]) {
      case 'h': case 'H': fullduplex = 0; break;
      case 'f': case 'F': fullduplex = 1; break;
      default: printf("duplex must be H or F..\n"); AMX_SPMDExit(1);
    }
  }
  if (!fullduplex && numprocs > 1 && numprocs % 2 != 0) {
     printf("half duplex requires an even number of processors\n"); AMX_SPMDExit(1);
  }
  msg_size = (size > AM_MaxLong() ? AM_MaxLong() : size);
  nummsgs = (size % AM_MaxLong() == 0 ? size / AM_MaxLong() : (size / AM_MaxLong())+1);
  srcmem = (uint32_t *)malloc(msg_size);
  memset(srcmem, 0, msg_size);
  VMseg = (uint32_t *)malloc(msg_size+100);
  memset(VMseg, 0, msg_size+100);
  AM_Safe(AM_SetSeg(ep, VMseg, msg_size+100));

  rightguy = (myproc + 1) % numprocs;

  { /*  init my source mem */
    int i;
    int numints = msg_size/4;
    for (i=0; i < numints; i++) srcmem[i] = i;
  }

  AM_Safe(AMX_SPMDBarrier());


  if (myproc == 0) printf("Running %s bulk test sz=%i...\n", (fullduplex?"full-duplex":"half-duplex"), size);

  begin = getCurrentTimeMicrosec();

  if (fullduplex || myproc % 2 == 1 || numprocs == 1) {
    int q;
    for (q=0; q<iters; q++) {
      int j;
      msg_size = AM_MaxLong();
      for (j = 0; j < nummsgs; j++) {
	      if (j == nummsgs-1 && size % AM_MaxLong() != 0) msg_size = size % AM_MaxLong(); /*  last one */
        #if VERBOSE_PING
	        printf("%i: sending request...", myproc); fflush(stdout);
        #endif
	      AM_Safe(AM_RequestXfer1(ep, rightguy, 100, BULK_REQ_HANDLER, srcmem, msg_size, 666));
      }
    }
  }

  { int expectedmsgs = nummsgs*iters;
    if (numprocs == 1 || fullduplex) expectedmsgs *= 2;

    if (polling) { /* poll until everyone done */
      while (done<expectedmsgs) {
        AM_Safe(AM_Poll(eb));
      }
    } else {
      while (done<expectedmsgs) {
        AM_Safe(AM_SetEventMask(eb, AM_NOTEMPTY)); 
        AM_Safe(AM_WaitSema(eb));
        AM_Safe(AM_Poll(eb));
      }
    }
  }

  end = getCurrentTimeMicrosec();

  total = end - begin;
  if (fullduplex || myproc % 2 == 1 || numprocs == 1) 
    printf("Slave %i: %i microseconds total, throughput: %8.3f KB/sec\n", 
      myproc, (int)total, (float)(((float)1000000)*size*iters/((int)total))/1024.0);
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
