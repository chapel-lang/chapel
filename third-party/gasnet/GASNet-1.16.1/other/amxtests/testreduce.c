/*   $Source: /var/local/cvs/gasnet/other/amxtests/testreduce.c,v $
 *     $Date: 2006/04/11 03:23:45 $
 * $Revision: 1.6 $
 * Description: AMX test
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include "apputils.h"

#define REDUCE_HANDLER  1

int total = 0;
int numcalls = 0;
void reduce_request_handler(void *token, int val) {
  printf("reduce_request_handler got: %i\n", val);
  total += val;
  numcalls++;
}

int main(int argc, char **argv) {
  eb_t eb;
  ep_t ep;
  uint64_t networkpid;
  int myproc;
  int numprocs;

  TEST_STARTUP(argc, argv, networkpid, eb, ep, 0, 0, "");

  /* setup handlers */
  AM_Safe(AM_SetHandler(ep, REDUCE_HANDLER, reduce_request_handler));
  setupUtilHandlers(ep, eb);
  
  /* barrier */
  AM_Safe(AMX_SPMDBarrier());

  /* get SPMD info */
  myproc = AMX_SPMDMyProc();
  numprocs = AMX_SPMDNumProcs();

  /* compute */
  AM_Safe(AM_Request1(ep, 0, REDUCE_HANDLER, myproc));

  if (myproc == 0) {
    while (numcalls < numprocs) {
      #if 0 /* poll-sleep */
        AM_Safe(AM_Poll(eb));
        sleep(1);
      #else /* poll-block */
        AM_Safe(AM_SetEventMask(eb, AM_NOTEMPTY));
        AM_Safe(AM_WaitSema(eb));
        AM_Safe(AM_Poll(eb));
      #endif
      printf(".");
    }
    printf("Reduction result: %i\n",total);
    { /* verify result */
      int i, correcttotal=0;
      for (i = 0; i < numprocs; i++) correcttotal += i;
      if (total == correcttotal) printf("Result verified!\n");
      else printf("ERROR!!! Result incorrect! total=%i  correcttotal=%i\n", total, correcttotal);
    }
  }


  /* barrier */
  AM_Safe(AMX_SPMDBarrier());

  printGlobalStats();

  AM_Safe(AMX_SPMDBarrier());

  /* exit */
  AM_Safe(AMX_SPMDExit(0));

  return 0;
}
/* ------------------------------------------------------------------------------------ */
