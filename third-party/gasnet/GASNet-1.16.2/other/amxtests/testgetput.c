/*   $Source: /var/local/cvs/gasnet/other/amxtests/testgetput.c,v $
 *     $Date: 2006/05/31 08:17:44 $
 * $Revision: 1.11 $
 * Description: AMX test
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include "apputils.h"

#define MAX_PROCS 255
static uint32_t vals[MAX_PROCS];

int main(int argc, char **argv) {
  eb_t eb;
  ep_t ep;
  uint64_t networkpid;
  int myproc;
  int numprocs;
  int k;
  int iters = 0;

  TEST_STARTUP(argc, argv, networkpid, eb, ep, 1, 1, "iters");

  TEST_32BIT_ONLY();

  /* setup handlers */
  setupUtilHandlers(ep, eb);

  /* get SPMD info */
  myproc = AMX_SPMDMyProc();
  numprocs = AMX_SPMDNumProcs();

  if (argc > 1) iters = atoi(argv[1]);
  if (!iters) iters = 1;
  if (myproc == 0) {
    printf("Running %i iterations of get/put test...\n", iters);
    fflush(stdout);
  }

  for (k=0;k < iters; k++) {
    /* set just my val */
    int i;
    for (i=0;i<MAX_PROCS;i++) vals[i] = (uint32_t)(-1);
    vals[myproc] = myproc;

    AM_Safe(AMX_SPMDBarrier()); /* barrier */

    { /* try some gets */
      int i;
      int sum = 0;
      int verify = 0;
      for (i = 0; i < numprocs; i++) {
        sum += getWord(i, &vals[i]); /*  get each peer's value and add them up */
        verify += i;
      }
      if (verify != sum) {
        printf("Proc %i GET TEST FAILED : sum = %i   verify = %i\n", myproc, sum, verify);
        fflush(stdout);
      }
      #if VERBOSE
        else {
          printf("Proc %i verified.\n", myproc);
          fflush(stdout);
        }
      #endif
    }

    AM_Safe(AMX_SPMDBarrier()); /* barrier */

    { /* try some puts */
      int i;
      for (i = 0; i < numprocs; i++) {
        putWord(i, &vals[myproc], myproc); /*  push our value to correct position on each peer */
      }
      AM_Safe(AMX_SPMDBarrier()); /* barrier */
      for (i = 0; i < numprocs; i++) {
        if (((int)vals[i]) != i) {
          printf("Proc %i PUT TEST FAILED : i = %i   vals[i] = %i\n", myproc, i, (int)vals[i]);
          break;
        }
      }
      #if VERBOSE
        if (i == numprocs) {
          printf("Proc %i verified.\n", myproc);
          fflush(stdout);
        }
      #endif
    }
  }

  /* dump stats */
  AM_Safe(AMX_SPMDBarrier());
  printGlobalStats();
  AM_Safe(AMX_SPMDBarrier());

  /* exit */
  AM_Safe(AMX_SPMDExit(0));

  return 0;
}
/* ------------------------------------------------------------------------------------ */
