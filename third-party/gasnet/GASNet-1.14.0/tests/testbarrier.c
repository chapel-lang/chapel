/*   $Source: /var/local/cvs/gasnet/tests/testbarrier.c,v $
 *     $Date: 2006/01/28 21:21:46 $
 * $Revision: 1.19 $
 * Description: GASNet barrier performance test
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet.h>

#include <test.h>

int main(int argc, char **argv) {
  int mynode, nodes, iters=0;
  int64_t start,total;
  int i = 0;

  GASNET_Safe(gasnet_init(&argc, &argv));
  GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
  test_init("testbarrier", 1, "(iters)");
  mynode = gasnet_mynode();
  nodes = gasnet_nodes();

  if (argc > 1) iters = atoi(argv[1]);
  if (!iters) iters = 10000;
  if (argc > 2) test_usage();

  if (mynode == 0) {
      printf("Running barrier test with %i iterations...\n",iters);
      fflush(stdout);
  }
  BARRIER();

  start = TIME();
  for (i=0; i < iters; i++) {
    gasnet_barrier_notify(i, 0);            
    GASNET_Safe(gasnet_barrier_wait(i, 0)); 
  }
  total = TIME() - start;

  BARRIER();

  if (mynode == 0) {
      printf("Total time: %8.3f sec  Avg Named Barrier latency: %8.3f us\n",
        ((float)total)/1000000, ((float)total)/iters);
      fflush(stdout);
  }
  BARRIER();

  start = TIME();
  for (i=0; i < iters; i++) {
    gasnet_barrier_notify(0, GASNET_BARRIERFLAG_ANONYMOUS);            
    GASNET_Safe(gasnet_barrier_wait(0, GASNET_BARRIERFLAG_ANONYMOUS)); 
  }
  total = TIME() - start;

  BARRIER();

  if (mynode == 0) {
      printf("Total time: %8.3f sec  Avg Anon. Barrier latency: %8.3f us\n",
        ((float)total)/1000000, ((float)total)/iters);
      fflush(stdout);
  }

  BARRIER();

  MSG("done.");

  gasnet_exit(0);
  return 0;
}
