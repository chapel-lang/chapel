/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testteambarrier.c $
 * Description: GASNet barrier performance test
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>
#include <gasnet_coll.h>

#include <test.h>
typedef struct {
  int my_local_thread;
  int mythread;

  char _pad[GASNETT_CACHE_LINE_BYTES];
  
} thread_data_t;

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

int mynode, nodes, iters, threads_per_node=0;

#define MYBARRIER() \
    GASNET_Safe(gasnet_coll_barrier(GASNET_TEAM_ALL, 0, GASNET_BARRIERFLAG_UNNAMED))

void *thread_main(void *arg) {
  thread_data_t *td = (thread_data_t*) arg;
  int i;
  int64_t start,total;

  MYBARRIER();
  if (td->mythread == 0) {
#if GASNET_PAR
      printf("Running team barrier test with %i iterations and %i threads/proc...\n",iters,threads_per_node);
#else
      printf("Running team barrier test with %i iterations...\n",iters);
#endif
      fflush(stdout);
  }
  
  MYBARRIER();
  
  start = TIME();
  for (i=0; i < iters; i++) {
    gasnet_coll_barrier_notify(GASNET_TEAM_ALL, i, 0);            
    GASNET_Safe(gasnet_coll_barrier_wait(GASNET_TEAM_ALL, i, 0));
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
    gasnet_coll_barrier_notify(GASNET_TEAM_ALL, 0, GASNET_BARRIERFLAG_ANONYMOUS);            
    GASNET_Safe(gasnet_coll_barrier_wait(GASNET_TEAM_ALL, 0, GASNET_BARRIERFLAG_ANONYMOUS));
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
  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testteambarrier", &argc, &argv, 0));
  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));


  
  mynode = gex_TM_QueryRank(myteam);
  nodes = gex_TM_QuerySize(myteam);

  if (argc > 1) iters = atoi(argv[1]);
  if (!iters) iters = 10000;
#if GASNET_PAR
  test_init("testteambarrier", 2, "(iters) (threadcount) (test sections)");
  if (argc > 2) {
    threads_per_node = atoi(argv[2]);
  } else {
    if (gasnett_getenv_yesno_withdefault("GASNET_TEST_POLITE_SYNC",0)) {
      /* May overcommit only if somebody already expected it */
      threads_per_node = gasnett_cpu_count();
    } else {
      threads_per_node = gasnett_cpu_count() / TEST_LOCALPROCS();
    } 
    threads_per_node = MIN(threads_per_node, 8);
    threads_per_node = test_thread_limit(threads_per_node);
    threads_per_node = MAX(threads_per_node, 1);
  } 
  if (threads_per_node > TEST_MAXTHREADS || threads_per_node < 1) {
    printf("ERROR: Threads must be between 1 and %d\n", TEST_MAXTHREADS);
    exit(EXIT_FAILURE);
  }
  // NO MULTI-IMAGE SUPPORT IN CURRENT COLLECTIVES
  if (threads_per_node > 1) {
    MSG0("WARNING: thread count reduced to 1 (no multi-image support)");
    threads_per_node = 1;
  }
  if (argc > 3) TEST_SECTION_PARSE(argv[3]);
  if (argc > 4) test_usage();
#else
  test_init("testteambarrier", 1, "(iters) (test sections)");
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
  TEST_SET_WAITMODE(threads_per_node);
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
