/*   $Source: /var/local/cvs/gasnet/tests/testbarrier.c,v $
 *     $Date: 2010/04/16 22:28:15 $
 * $Revision: 1.19.72.1 $
 * Description: GASNet barrier performance test
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet.h>

#include <test.h>

int mynode, nodes, iters = 0;

#define hidx_done_shorthandler   200
volatile int done = 0;
void done_shorthandler(gasnet_token_t token) {
	  done = 1;
}
gasnet_handlerentry_t htable[] = { { hidx_done_shorthandler,  done_shorthandler  } };

static void * doTest(void *);

int main(int argc, char **argv) {
  int pollers = 0;
  int arg = 1;

  GASNET_Safe(gasnet_init(&argc, &argv));
  GASNET_Safe(gasnet_attach(htable, 1, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));

#if GASNET_PAR
  test_init("testbarrier", 1, "[-p polling_threads] (iters)");
#else
  test_init("testbarrier", 1, "(iters)");
#endif
  mynode = gasnet_mynode();
  nodes = gasnet_nodes();

  if ((argc-arg >= 2) && !strcmp(argv[arg], "-p")) {
#if GASNET_PAR
    pollers = atoi(argv[arg+1]);
    arg += 2;
#else
    if (gasnet_mynode() == 0) {
      fprintf(stderr, "testbarrier %s\n", GASNET_CONFIG_STRING);
      fprintf(stderr, "ERROR: The -p option is only available in the PAR configuration.\n");
      fflush(NULL);
    }
    sleep(1);
    gasnet_exit(1);
#endif
  }
  if (argc-arg >= 1) iters = atoi(argv[arg]);
  if (!iters) iters = 10000;
  if (argc-arg >= 2) test_usage();

#if !defined(GASNET_PAR)
  MSG0("Running barrier test with %i iterations...\n",iters);
#else
  MSG0("Running barrier test with %i iterations and %i extra polling threads...\n",iters,pollers);

  if (pollers)
      test_createandjoin_pthreads(pollers+1,doTest,NULL,0);
  else
#endif
      doTest(NULL);

  BARRIER();

  MSG("done.");

  gasnet_exit(0);
  return 0;
}

static void * doTest(void *arg) {
  int64_t start,total;
  int i = 0;

  if (arg) {
    /* I am a polling thread */
    GASNET_BLOCKUNTIL(done);
    return NULL;
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

  GASNET_Safe(gasnet_AMRequestShort0(mynode, hidx_done_shorthandler));
  return NULL;
}
