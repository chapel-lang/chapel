/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amx/testreadwrite.c $
 * Description: AMX test
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include "apputils.h"

#define MAX_PROCS 255
static uint32_t vals[MAX_PROCS];
uint32_t *myvals = vals;
uint32_t *pvals[MAX_PROCS];

static uint32_t readarray[MAX_PROCS];

int main(int argc, char **argv) {
  eb_t eb;
  ep_t ep;
  uint64_t networkpid;
  int myproc;
  int numprocs;
  int k;
  int iters = 0;
  int errs = 0;

  TEST_STARTUP(argc, argv, networkpid, eb, ep, 1, 1, "iters");

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

  /* gather pointers to static data, to handle non-uniform address spaces */
  AM_Safe(AMX_SPMDAllGather(&myvals, pvals, sizeof(myvals))); 

  for (k=0;k < iters; k++) {
    /* set left neighbor's array */
    int i;
    int leftP = myproc-1;
    if (leftP == -1) leftP = numprocs-1;
    for (i=0;i<MAX_PROCS;i++) writeWord(leftP, pvals[leftP]+i, k);
    writeSync();

    AM_Safe(AMX_SPMDBarrier()); /* barrier */

    { /* read right neighbor's array  */
      int i;
      int rightP = myproc+1;
      if (rightP == numprocs) rightP = 0;

      for (i=0;i<MAX_PROCS;i++) readWord(&readarray[i], rightP, pvals[rightP]+i);
      readSync();

      /* verify */
      for (i=0;i<MAX_PROCS;i++) {
        if (((int)readarray[i]) != k) {
          printf("ERROR: Proc %i READ/WRITE TEST FAILED : readarray[%i] = %i   k = %i\n", myproc, i, (int)readarray[i], k);
          fflush(stdout);
          errs++;
          break;
        }
      }
    }

    AM_Safe(AMX_SPMDBarrier()); /* barrier */

  }

  if (!errs) {
    printf("Proc %i verified.\n", myproc);
    fflush(stdout);
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
