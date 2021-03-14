/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amx/testping.c $
 * Description: AMX test
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include "apputils.h"

static volatile int numleft;

int myproc;
int numprocs;

/* usage: testoutput  numprocs  spawnfn  iters  
 */
int main(int argc, char **argv) {
  eb_t eb;
  ep_t ep;
  uint64_t networkpid;
  int64_t begin, end, total;
  int k,j;
  int iters = 0;
  int maxwidth = 0;
  char *buf;
  char sym;

  TEST_STARTUP(argc, argv, networkpid, eb, ep, 1, 2, "iters (maxwidth)");

  setupUtilHandlers(ep, eb);

  /* get SPMD info */
  myproc = AMX_SPMDMyProc();
  numprocs = AMX_SPMDNumProcs();

  if (argc > 1) iters = atoi(argv[1]);
  if (!iters) iters = 1;
  if (argc > 2) maxwidth = atoi(argv[2]);
  if (maxwidth < 1) maxwidth = 80;

  buf = malloc(maxwidth+4);
  if (myproc < 10) sym = '0'+myproc;
  else if (myproc < 36) sym = 'A'+(myproc-10);
  else sym = '*';
  memset(buf, sym, maxwidth+4);

  AM_Safe(AMX_SPMDBarrier());

  if (myproc == 0) printf("Running %i iterations of output test...\n", iters);

  begin = getCurrentTimeMicrosec();

  for (j=1; j <= maxwidth; j++) {
    for (k=0;k < iters; k++) {
      fwrite(buf, j, 1, stdout);
      fputc('\n', stdout);
    }
    fflush(stdout);
  }

  end = getCurrentTimeMicrosec();

  total = end - begin;
  printf("Worker %i: %i microseconds total, throughput: %.3f KB/sec\n", 
    myproc, (int)total, ((float)iters)*(maxwidth/2)/1024/(total/1.0E6));

  AM_Safe(AMX_SPMDBarrier());

  /* final output 1, deliberately dangling line */
  fwrite(buf, 4, 1, stdout);

  AM_Safe(AMX_SPMDBarrier());

  /* final output 2, deliberately dangling line */
  fwrite(buf, 4, 1, stdout);
 
  /* exit */
  AM_Safe(AMX_SPMDExit(0));

  return 0;
}
/* ------------------------------------------------------------------------------------ */
