/*   $Source: /var/local/cvs/gasnet/tests/testteambarrier.c,v $
 *     $Date: 2009/09/28 00:28:37 $
 * $Revision: 1.3 $
 * Description: GASNet barrier performance test
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet.h>
#include <gasnet_coll.h>

#include <test.h>
typedef struct {
  int my_local_thread;
  int mythread;

  char _pad[GASNETT_CACHE_LINE_BYTES];
  
} thread_data_t;

int mynode, nodes, iters, threads_per_node=0;

#define MYBARRIER() do {\
    gasnet_coll_barrier_notify(GASNET_TEAM_ALL, 0, GASNET_BARRIERFLAG_ANONYMOUS | GASNET_BARRIERFLAG_IMAGES);\
    GASNET_Safe(gasnet_coll_barrier_wait(GASNET_TEAM_ALL, 0, GASNET_BARRIERFLAG_ANONYMOUS | GASNET_BARRIERFLAG_IMAGES));\
} while (0)

void *thread_main(void *arg) {
  thread_data_t *td = (thread_data_t*) arg;
  int i;
  int64_t start,total;
#if GASNET_PAR
  gasnet_image_t *imagearray = test_malloc(nodes * sizeof(gasnet_image_t));
  for (i=0; i<nodes; ++i) { imagearray[i] = threads_per_node; }
  gasnet_coll_init(imagearray, td->mythread, NULL, 0, 0);
  test_free(imagearray);
#else
  gasnet_coll_init(NULL, 0, NULL, 0, 0);
#endif


  MYBARRIER();
  if (td->mythread == 0) {
      printf("Running barrier test with %i iterations...\n",iters);
      fflush(stdout);
  }
  
  MYBARRIER();
  
  start = TIME();
  for (i=0; i < iters; i++) {
    gasnet_coll_barrier_notify(GASNET_TEAM_ALL, i, GASNET_BARRIERFLAG_IMAGES);            
    GASNET_Safe(gasnet_coll_barrier_wait(GASNET_TEAM_ALL, i, GASNET_BARRIERFLAG_IMAGES)); 
  }
  total = TIME() - start;

  MYBARRIER();

  if (td->mythread == 0) {
      printf("Total time: %8.3f sec  Avg Named Barrier latency: %8.3f us\n",
        ((float)total)/1000000, ((float)total)/iters);
      fflush(stdout);
  }
  MYBARRIER();

  start = TIME();
  for (i=0; i < iters; i++) {
    gasnet_coll_barrier_notify(GASNET_TEAM_ALL, 0, GASNET_BARRIERFLAG_ANONYMOUS | GASNET_BARRIERFLAG_IMAGES);            
    GASNET_Safe(gasnet_coll_barrier_wait(GASNET_TEAM_ALL, 0, GASNET_BARRIERFLAG_ANONYMOUS | GASNET_BARRIERFLAG_IMAGES)); 
  }
  total = TIME() - start;

  MYBARRIER();

  if (td->mythread == 0) {
      printf("Total time: %8.3f sec  Avg Anon. Barrier latency: %8.3f us\n",
        ((float)total)/1000000, ((float)total)/iters);
      fflush(stdout);
  }

  MYBARRIER();
  
  return NULL;
}
int main(int argc, char **argv) {


  int i = 0;
  thread_data_t *td_arr; 
  GASNET_Safe(gasnet_init(&argc, &argv));
  GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));


  
  mynode = gasnet_mynode();
  nodes = gasnet_nodes();

  if (argc > 1) iters = atoi(argv[1]);
  if (!iters) iters = 10000;
#if GASNET_PAR
  test_init("testteambarrier", 2, "(iters) (threadcount)");
  if (argc > 2) {
    threads_per_node = atoi(argv[2]);
  } else {
    threads_per_node = gasnett_cpu_count(); 
    threads_per_node = MIN(threads_per_node, 8);
  }
  if (threads_per_node > TEST_MAXTHREADS || threads_per_node < 1) {
    printf("ERROR: Threads must be between 1 and %d\n", TEST_MAXTHREADS);
    exit(EXIT_FAILURE);
  }
  if (argc > 3) TEST_SECTION_PARSE(argv[3]);
  if (argc > 4) test_usage();
#else
  test_init("testteambarrier", 1, "(iters)");
  threads_per_node = 1;
  if (argc > 3) test_usage();
  if (argc > 2) TEST_SECTION_PARSE(argv[2]);
#endif  
  TEST_SET_WAITMODE(threads_per_node);

  td_arr = (thread_data_t*) test_malloc(sizeof(thread_data_t)*threads_per_node);
  for(i=0; i<threads_per_node; i++) {
    td_arr[i].my_local_thread = i;
    td_arr[i].mythread = mynode*threads_per_node+i;
  }
#if GASNET_PAR
  test_createandjoin_pthreads(threads_per_node, &thread_main, td_arr, sizeof(thread_data_t));
#else
  thread_main(&td_arr[0]);
#endif
  test_free(td_arr);
  

  gasnet_coll_barrier_notify(GASNET_TEAM_ALL, 0, GASNET_BARRIERFLAG_ANONYMOUS); 
  GASNET_Safe(gasnet_coll_barrier_wait(GASNET_TEAM_ALL, 0, GASNET_BARRIERFLAG_ANONYMOUS));
  MSG("done.");
  gasnet_exit(0);
  return 0;
}
