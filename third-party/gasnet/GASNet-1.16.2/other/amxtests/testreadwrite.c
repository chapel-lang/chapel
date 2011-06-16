/*   $Source: /var/local/cvs/gasnet/other/amxtests/testreadwrite.c,v $
 *     $Date: 2006/05/31 08:17:44 $
 * $Revision: 1.9 $
 * Description: AMX test
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include "apputils.h"

#define MAX_PROCS 255
static uint32_t vals[MAX_PROCS];
static uint32_t readarray[MAX_PROCS];

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
    printf("Running %i iterations of read/write test...\n", iters);
    fflush(stdout);
  }

  for (k=0;k < iters; k++) {
    /* set left neighbor's array */
    int i;
    int leftP = myproc-1;
    if (leftP == -1) leftP = numprocs-1;
    for (i=0;i<MAX_PROCS;i++) writeWord(leftP, &vals[i], k);
    writeSync();

    AM_Safe(AMX_SPMDBarrier()); /* barrier */

    { /* read right neighbor's array  */
      int i;
      int rightP = myproc+1;
      if (rightP == numprocs) rightP = 0;

      for (i=0;i<MAX_PROCS;i++) readWord(&readarray[i], rightP, &vals[i]);
      readSync();

      /* verify */
      for (i=0;i<MAX_PROCS;i++) {
        if (((int)readarray[i]) != k) {
          printf("Proc %i READ/WRITE TEST FAILED : readarray[%i] = %i   k = %i\n", myproc, i, (int)readarray[i], k);
          fflush(stdout);
          break;
        }
      }
      #if DEBUG
        if (i != MAX_PROCS) {
          printf("Proc %i verified.\n", myproc);
          fflush(stdout);
          }
      #endif
    }

    AM_Safe(AMX_SPMDBarrier()); /* barrier */

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
