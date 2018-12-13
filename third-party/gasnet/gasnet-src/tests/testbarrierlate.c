/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testbarrierlate.c $
 * Description: GASNet barrier performance test
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>

#define TEST_DELAY 1
#include <test.h>

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

int main(int argc, char **argv) {
  struct delay_s {
    int64_t	delay_us;
    int64_t	delay_loops;
  } delay_params;
  int mynode, nodes, iters=0;
  int64_t start,total,delay_us,baseline_us;
  int64_t min_time, max_time, avg_time;
  int64_t delay_loops = 0;
  int j, i = 0;
  int pause_len;
  int pollcnt = 0;

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testbarrierlate", &argc, &argv, 0));
  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  test_init("testbarrierlate",1,"(iters) (pollcnt)");

  mynode = gex_TM_QueryRank(myteam);
  nodes = gex_TM_QuerySize(myteam);

  if (argc > 1) iters = atoi(argv[1]);
  if (!iters) iters = 10000;
  if (argc > 2) pollcnt = atoi(argv[2]);
  if (argc > 3) test_usage();

  if (mynode == 0) {
      printf("Running barrier late arrival test with %i iterations, pollcnt=%i...\n",iters, pollcnt);
      fflush(stdout);
  }
  BARRIER();

  /* warmup */
  for (i=0; i < MIN(100,iters/100); i++) {
    gasnet_barrier_notify(0, GASNET_BARRIERFLAG_ANONYMOUS);            
    GASNET_Safe(gasnet_barrier_wait(0, GASNET_BARRIERFLAG_ANONYMOUS)); 
  }
  BARRIER();

  start = TIME();
  for (i=0; i < iters; i++) {
    gasnet_barrier_notify(0, GASNET_BARRIERFLAG_ANONYMOUS);            
    GASNET_Safe(gasnet_barrier_wait(0, GASNET_BARRIERFLAG_ANONYMOUS)); 
  }
  baseline_us = TIME() - start;

  BARRIER();

  if (mynode == 0) {
      printf("Total time: %8.3f sec  Avg Anon. Barrier latency: %8.3f us\n",
        ((float)baseline_us)/1000000, ((float)baseline_us)/iters);
      fflush(stdout);
  }

  /* Calibrate a delay loop.  Given "iters" and "delay_us", we determine the argument
   * we need when calling test_delay() iters times, to get a _total_ delay no less than
   * delay_us.  The value of delay_us is overwritten with the achieved delay.
   * We calibrate the delay on exactly one node to avoid spoiling timings on
   * overcommitted CPUs.  Nodes not performing the calibration sleep for at least twice
   * the time we are calibrating for.  (No way to be sure this is enough, since
   * calibration is iterative.)
   */
  BARRIER();
  pause_len = 1 + 4 * (baseline_us + 999999)/1000000;

  if (mynode == 0) {
      struct delay_s *p = (struct delay_s *)TEST_MYSEG();

      start = TIME();
      printf("Calibrating delay loop (expect at least a %d sec pause)...\n", pause_len);
      fflush(stdout);
      p->delay_us = 2*baseline_us;	/* delay at least two full barrier times */
      p->delay_loops = test_calibrate_delay(iters, pollcnt, &(p->delay_us));
  } else {
      sleep(pause_len);
  }
  BARRIER();
  gex_RMA_GetBlocking(myteam, &delay_params, 0, TEST_SEG(0), sizeof(struct delay_s), 0);
  delay_us = delay_params.delay_us;
  delay_loops = delay_params.delay_loops;
  if (mynode == 0) {
    printf("Calibration complete (actual pause = %5.3f sec).\n", (float)((TIME()-start)/1000000.0));
    printf("Ideal loop time = %8.3f sec.\n", (float)(delay_us)/1000000.0);
    fflush(stdout);
  }

  /* Take turns being late to notify
   * We insert a delay before the _notify() on one node.
   * This simulates a load imbalance between barriers.
   * The time reported is how much the barrier costs excluding the delay.
   * This reported time will often be less than the full barrier because
   * some progress was made by the other nodes.
   */
  avg_time = 0;
  max_time = 0;
  min_time = (int64_t)1 << 62;	/* good enough */
  for (j=0; j < nodes; j++) {
    BARRIER();
 
    start = TIME();
    for (i=0; i < iters; i++) {
      if (j == mynode) {
        test_delay(delay_loops, pollcnt);
      }
      gasnet_barrier_notify(0, GASNET_BARRIERFLAG_ANONYMOUS);
      GASNET_Safe(gasnet_barrier_wait(0, GASNET_BARRIERFLAG_ANONYMOUS)); 
    }
    total = TIME() - start;

    if (mynode == 0) {
      printf("Total time: %8.3f sec  Late-notify test on node %d\n", ((float)total)/1000000, j);
      fflush(stdout);
    }

    total -= delay_us;
    avg_time += total;
    min_time = MIN(min_time, total);
    max_time = MAX(max_time, total);
  }
  avg_time /= nodes;

  if (mynode == 0) {
    printf("Total difference: %8.3f sec  Late notify() Anon. Barrier net latency, minimum: %8.3f us (%6.2f%%)\n", ((float)min_time)/1000000, ((float)min_time)/iters, ((float)min_time * 100.)/baseline_us);
    printf("Total difference: %8.3f sec  Late notify() Anon. Barrier net latency, maximum: %8.3f us (%6.2f%%)\n", ((float)max_time)/1000000, ((float)max_time)/iters, ((float)max_time * 100.)/baseline_us);
    printf("Total difference: %8.3f sec  Late notify() Anon. Barrier net latency, average: %8.3f us (%6.2f%%)\n", ((float)avg_time)/1000000, ((float)avg_time)/iters, ((float)avg_time * 100.)/baseline_us);
    fflush(stdout);
  }

  /* Take turns being late to wait
   * We insert a delay between the _notify() and _wait() on one node.
   * This simulates a load imbalance between barrier notify and wait.
   * The time reported is how much the barrier costs excluding the delay.
   * This reported time will often be less than the full barrier because
   * some progress was made by the other nodes.
   */
  avg_time = 0;
  max_time = 0;
  min_time = (int64_t)1 << 62;	/* good enough */
  for (j=0; j < nodes; j++) {
    BARRIER();

    start = TIME();
    for (i=0; i < iters; i++) {
      gasnet_barrier_notify(0, GASNET_BARRIERFLAG_ANONYMOUS);
      if (j == mynode) {
        test_delay(delay_loops, pollcnt);
      }
      GASNET_Safe(gasnet_barrier_wait(0, GASNET_BARRIERFLAG_ANONYMOUS)); 
    }
    total = TIME() - start;

    if (mynode == 0) {
      printf("Total time: %8.3f sec  Late-wait test on node %d\n", ((float)total)/1000000, j);
      fflush(stdout);
    }

    total -= delay_us;
    avg_time += total;
    min_time = MIN(min_time, total);
    max_time = MAX(max_time, total);
  }
  avg_time /= nodes;

  if (mynode == 0) {
    printf("Total difference: %8.3f sec  Late wait() Anon. Barrier net latency, minimum: %8.3f us (%6.2f%%)\n", ((float)min_time)/1000000, ((float)min_time)/iters, ((float)min_time * 100.)/baseline_us);
    printf("Total difference: %8.3f sec  Late wait() Anon. Barrier net latency, maximum: %8.3f us (%6.2f%%)\n", ((float)max_time)/1000000, ((float)max_time)/iters, ((float)max_time * 100.)/baseline_us);
    printf("Total difference: %8.3f sec  Late wait() Anon. Barrier net latency, average: %8.3f us (%6.2f%%)\n", ((float)avg_time)/1000000, ((float)avg_time)/iters, ((float)avg_time * 100.)/baseline_us);
    fflush(stdout);
  }

  BARRIER();

  MSG("done.");

  gasnet_exit(0);
  return 0;
}
