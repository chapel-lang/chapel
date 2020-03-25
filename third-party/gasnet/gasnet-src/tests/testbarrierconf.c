/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testbarrierconf.c $
 * Description: GASNet barrier performance test
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>

#include <test.h>

#ifndef PERFORM_MIXED_NAMED_ANON_TESTS
#define PERFORM_MIXED_NAMED_ANON_TESTS 1
#endif


static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

static int do_try = 0;
GASNETT_INLINE(my_barrier_wait)
int my_barrier_wait(int value, int flags) {
  int rc;
  if (do_try) {
    rc = gasnet_barrier_try(value, flags);
    while (rc == GASNET_ERR_NOT_READY) {
      gasnett_sched_yield();
      rc = gasnet_barrier_try(value, flags);
    }
  } else {
    rc = gasnet_barrier_wait(value, flags);
  }
  return rc;
}

#define hidx_done_shorthandler   200
volatile int done = 0;
void done_shorthandler(gex_Token_t token) { done = 1; }
gex_AM_Entry_t htable[] = {
    { hidx_done_shorthandler,  done_shorthandler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 0, NULL, NULL }
};

static void * doTest(void *arg);

static int mynode, nodes, iters;

int main(int argc, char **argv) {
  int pollers = 0;
  int arg;

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testbarrierconf", &argc, &argv, 0));
  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, 1));

#if GASNET_PAR
  test_init("testbarrierconf", 0, "[-t] [-p polling_threads] (iters)\n"
            "  The -p option gives a number of polling threads to spawn (default is 0).\n"
            "  The -t option replaces barrier_wait calls with looping on barrier_try");
#else
  test_init("testbarrierconf", 0, "[-t] (iters)\n"
            "  The -t option replaces barrier_wait calls with looping on barrier_try");
#endif

  arg = 1;
  while (argc-arg >= 1) {
   if (!strcmp(argv[arg], "-p")) {
#ifdef GASNET_PAR
    if (argc-arg < 2) {
      if (mynode == 0) {
        fprintf(stderr, "testbarrierconf %s\n", GASNET_CONFIG_STRING);
        fprintf(stderr, "ERROR: The -p option requires an argument.\n");
        fflush(NULL);
      }
      sleep(1);
      gasnet_exit(1);
    }
    pollers = test_thread_limit(atoi(argv[arg+1])+1)-1;
    arg += 2;
#else
    if (mynode == 0) {
      fprintf(stderr, "testbarrierconf %s\n", GASNET_CONFIG_STRING);
      fprintf(stderr, "ERROR: The -p option is only available in the PAR configuration.\n");
      fflush(NULL);
    }
    sleep(1);
    gasnet_exit(1);
#endif
   } else if (!strcmp(argv[arg], "-t")) {
    do_try = 1;
    arg += 1;
   } else break;
  }
  if (argc-arg >= 1) iters = atoi(argv[arg]);
  if (iters <= 0) iters = 1000;
  if (argc-arg >= 2) test_usage();

  mynode = gex_TM_QueryRank(myteam);
  nodes = gex_TM_QuerySize(myteam);

  if (mynode == 0) {
      const char * mode = do_try ? "try" : "wait";
#ifdef GASNET_PAR
      printf("Running barrier_%s conformance test with %d iterations and %i extra polling threads...\n", mode, iters,pollers);
#else
      printf("Running barrier_%s conformance test with %d iterations...\n", mode, iters);
#endif
      fflush(stdout);
  }
  BARRIER();

  TEST_SET_WAITMODE_RESERVE(pollers+1, 1);
#ifdef GASNET_PAR
  if (pollers)
      test_createandjoin_pthreads(pollers+1,doTest,NULL,0);
  else
#endif
      doTest(NULL);

  MSG("done.");

  gasnet_exit(0);
  return 0;
}

static void * doTest(void *arg) {
  int i = 0;
  int result;

  if (arg) {
    /* I am a polling thread */
    GASNET_BLOCKUNTIL(done);
    return NULL;
  }

  BARRIER();

  if (!PERFORM_MIXED_NAMED_ANON_TESTS) {
    if (mynode == 0) {
      MSG("WARNING: skipping tests which mix named and anonymous barriers, "
          "which are known to fail in this configuration");
    }
    BARRIER();
  }

  /* Test for required failures: */
  for (i = 0; i < iters; ++i) {
    /* node 0 indicates mismatch on entry: */
    gasnet_barrier_notify(0, !mynode ? GASNET_BARRIERFLAG_MISMATCH : 0);
    result = my_barrier_wait(0, !mynode ? GASNET_BARRIERFLAG_MISMATCH : 0);
    if (result != GASNET_ERR_BARRIER_MISMATCH) {
      MSG("ERROR: Failed to detect barrier mismatch indicated on notify.");
      gasnet_exit(1);
    }

    /* node 0 indicates mismatch on blocking barrier: */
    result = gasnet_barrier(0, !mynode ? GASNET_BARRIERFLAG_MISMATCH : 0);
    if (result != GASNET_ERR_BARRIER_MISMATCH) {
      MSG("ERROR: Failed to detect barrier mismatch indicated in blocking barrier.");
      gasnet_exit(1);
    }

    /* ids differ between notify and wait */
    gasnet_barrier_notify(0, 0);
    result = my_barrier_wait(1, 0);
    if (result != GASNET_ERR_BARRIER_MISMATCH) {
      MSG("ERROR: Failed to detect mismatch between id at notify and wait.");
      gasnet_exit(1);
    }

    /* Flags can (as of GASNet 1.20) differ between notify and wait: */
    gasnet_barrier_notify(0, GASNET_BARRIERFLAG_ANONYMOUS);
    result = my_barrier_wait(0, 0);
    if (result != GASNET_OK) {
      MSG("ERROR: Failed to allow anonymous notify with named wait.");
      gasnet_exit(1);
    }
    if (!gasnet_barrier_result(&result)) {
      MSG("ERROR: Wrong gasnet_barrier_result() return from anonymous notify with named wait.");
      gasnet_exit(1);
    }
    gasnet_barrier_notify(0, 0);
    result = my_barrier_wait(0, GASNET_BARRIERFLAG_ANONYMOUS);
    if (result != GASNET_OK) {
      MSG("ERROR: Failed to allow named notify with anonymous wait.");
      gasnet_exit(1);
    }
    if (gasnet_barrier_result(&result)) {
      MSG("ERROR: Wrong gasnet_barrier_result() return from named notify with anonymous wait.");
      gasnet_exit(1);
    } else if (0 != result) {
      MSG("ERROR: Wrong gasnet_barrier_result() value from named notify with anonymous wait.");
      gasnet_exit(1);
    }

    // GASNet-EX barriers can overlap
    { gex_Event_t ev[8];
      const int count = sizeof(ev) / sizeof(*ev);
      // Initiate multiple split-phase barriers:
      for (int i = 0; i < count; ++i) {
        ev[i] = gex_Coll_BarrierNB(myteam, 0);
      }
      // Retire them out-of-order
      for (int i = 0; i < count; ++i) {
        gex_Event_Wait(ev[(mynode + i) % count]);
      }
    }

    if (nodes > 1) {
      int j;

      for (j = 0; j < nodes; ++j) {
       if (PERFORM_MIXED_NAMED_ANON_TESTS) {
        /* Mix many named with one anonymous: */
        if (mynode == j) {
          gasnet_barrier_notify(12345, GASNET_BARRIERFLAG_ANONYMOUS);
          result = my_barrier_wait(12345, GASNET_BARRIERFLAG_ANONYMOUS);
        } else {
          gasnet_barrier_notify(5551212, 0);
          result = my_barrier_wait(5551212, 0);
        }
        if (result != GASNET_OK) {
          MSG("ERROR: Failed to match anon notify on node %d with named notify elsewhere.", j);
          gasnet_exit(1);
        }
        if (gasnet_barrier_result(&result)) {
          MSG("ERROR: Wrong gasnet_barrier_result() return from mixed test #1");
          gasnet_exit(1);
        } else if (5551212 != result) {
          MSG("ERROR: Wrong gasnet_barrier_result() value from mixed test #1");
          gasnet_exit(1);
        }

        /* Mix many named with one anonymous (blocking): */
        if (mynode == j) {
          result = gasnet_barrier(12345, GASNET_BARRIERFLAG_ANONYMOUS);
        } else {
          result = gasnet_barrier(5551212, 0);
        }
        if (result != GASNET_OK) {
          MSG("ERROR: Failed to match anon node %d with named barier elsewhere.", j);
          gasnet_exit(1);
        }
        if (gasnet_barrier_result(&result)) {
          MSG("ERROR: Wrong gasnet_barrier_result() return from mixed test #1b");
          gasnet_exit(1);
        } else if (5551212 != result) {
          MSG("ERROR: Wrong gasnet_barrier_result() value from mixed test #1b");
          gasnet_exit(1);
        }

        /* Mix many named with one anonymous notify plus named wait: */
        if (mynode == j) {
          gasnet_barrier_notify(12345, GASNET_BARRIERFLAG_ANONYMOUS);
          result = my_barrier_wait(5551212, 0);
        } else {
          gasnet_barrier_notify(5551212, 0);
          result = my_barrier_wait(5551212, 0);
        }
        if (result != GASNET_OK) {
          MSG("ERROR: Failed to match anon notify and named wait on node %d with named notify elsewhere.", j);
          gasnet_exit(1);
        }
        if (gasnet_barrier_result(&result)) {
          MSG("ERROR: Wrong gasnet_barrier_result() return from mixed test #2");
          gasnet_exit(1);
        } else if (5551212 != result) {
          MSG("ERROR: Wrong gasnet_barrier_result() value from mixed test #2");
          gasnet_exit(1);
        }

        /* Mix many named with one anonymous notify plus MISnamed wait: */
        if (mynode == j) {
          gasnet_barrier_notify(12345, GASNET_BARRIERFLAG_ANONYMOUS);
          result = my_barrier_wait(12345, 0);
          if (result != GASNET_ERR_BARRIER_MISMATCH) {
            MSG("ERROR: Failed to detect anon notify and mis-named wait on node %d with named notify elsewhere.", j);
            gasnet_exit(1);
          }
        } else {
          gasnet_barrier_notify(5551212, 0);
          result = my_barrier_wait(5551212, 0);
          /* neither required not prohibited from signalling an error here. */
        }

        /* Mix one named with many anonymous: */
        if (mynode == j) {
          gasnet_barrier_notify(0xcafef00d, 0);
          result = my_barrier_wait(0xcafef00d, 0);
        } else {
          gasnet_barrier_notify(911, GASNET_BARRIERFLAG_ANONYMOUS);
          result = my_barrier_wait(911, GASNET_BARRIERFLAG_ANONYMOUS);
        }
        if (result != GASNET_OK) {
          MSG("ERROR: Failed to match named notify on node %d with anon notify elsewhere.", j);
          gasnet_exit(1);
        }
        if (gasnet_barrier_result(&result)) {
          MSG("ERROR: Wrong gasnet_barrier_result() return from mixed test #4");
          gasnet_exit(1);
        } else if (0xcafef00d != result) {
          MSG("ERROR: Wrong gasnet_barrier_result() value from mixed test #4");
          gasnet_exit(1);
        }

        /* Mix one named with many anonymous (blocking): */
        if (mynode == j) {
          result = gasnet_barrier(0xcafef00d, 0);
        } else {
          result = gasnet_barrier(911, GASNET_BARRIERFLAG_ANONYMOUS);
        }
        if (result != GASNET_OK) {
          MSG("ERROR: Failed to match named barrier on node %d with anon elsewhere.", j);
          gasnet_exit(1);
        }
        if (gasnet_barrier_result(&result)) {
          MSG("ERROR: Wrong gasnet_barrier_result() return from mixed test #4b");
          gasnet_exit(1);
        } else if (0xcafef00d != result) {
          MSG("ERROR: Wrong gasnet_barrier_result() value from mixed test #4b");
          gasnet_exit(1);
        }
      }

	    /* All named (blocking): */
        result = gasnet_barrier(i, 0);
        if (result != GASNET_OK) {
          MSG("ERROR: Failed to pass on properly matched barrier");
          gasnet_exit(1);
        }
        if (gasnet_barrier_result(&result)) {
          MSG("ERROR: Wrong gasnet_barrier_result() return from matching barrier");
          gasnet_exit(1);
        } else if (i != result) {
          MSG("ERROR: Wrong gasnet_barrier_result() value from matching barrier");
          gasnet_exit(1);
        }

	    /* All anon (blocking): */
        result = gasnet_barrier(i^j, GASNET_BARRIERFLAG_ANONYMOUS);
        if (result != GASNET_OK) {
          MSG("ERROR: Failed to pass on all-anonymous barrier");
          gasnet_exit(1);
        }
        if (!gasnet_barrier_result(&result)) {
          MSG("ERROR: Wrong gasnet_barrier_result() return from all-anonymous barrier");
          gasnet_exit(1);
        }

	    /* All named: */
        gasnet_barrier_notify(i, 0);
        result = my_barrier_wait(i, 0);
        if (result != GASNET_OK) {
          MSG("ERROR: Failed to pass on properly matched notify/wait");
          gasnet_exit(1);
        }
        if (gasnet_barrier_result(&result)) {
          MSG("ERROR: Wrong gasnet_barrier_result() return from matching notify/wait");
          gasnet_exit(1);
        } else if (i != result) {
          MSG("ERROR: Wrong gasnet_barrier_result() value from matching notify/wait");
          gasnet_exit(1);
        }

	    /* All anon: */
        gasnet_barrier_notify(i^j, GASNET_BARRIERFLAG_ANONYMOUS);
        result = my_barrier_wait(i^j, 0);
        if (result != GASNET_OK) {
          MSG("ERROR: Failed to pass on all-anonymous notify");
          gasnet_exit(1);
        }
        if (!gasnet_barrier_result(&result)) {
          MSG("ERROR: Wrong gasnet_barrier_result() return from all-anonymous notify");
          gasnet_exit(1);
        }

        /* Mismatched id: */
        gasnet_barrier_notify(mynode == j, 0);
        result = my_barrier_wait(mynode == j, 0);
        if (result != GASNET_ERR_BARRIER_MISMATCH) {
          MSG("ERROR: Failed to detect different notify id on node %d.", j);
          gasnet_exit(1);
        }

        /* Mismatched id (blocking): */
        result = gasnet_barrier(mynode == j, 0);
        if (result != GASNET_ERR_BARRIER_MISMATCH) {
          MSG("ERROR: Failed to detect different barrier id on node %d.", j);
          gasnet_exit(1);
        }

        /* Node j indicates mismatch on entry: */
        gasnet_barrier_notify(0, (j == mynode) ? GASNET_BARRIERFLAG_MISMATCH : 0);
        result = my_barrier_wait(0, (j == mynode) ? GASNET_BARRIERFLAG_MISMATCH : 0);
        if (result != GASNET_ERR_BARRIER_MISMATCH) {
          MSG("ERROR: Failed to detect barrier mismatch indicated on notify by node %d.", j);
          gasnet_exit(1);
        }

        /* Node j indicates mismatch on barrier: */
        result = gasnet_barrier(0, (j == mynode) ? GASNET_BARRIERFLAG_MISMATCH : 0);
        if (result != GASNET_ERR_BARRIER_MISMATCH) {
          MSG("ERROR: Failed to detect barrier mismatch indicated by node %d.", j);
          gasnet_exit(1);
        }

#if 0 /* TBD: is this case clearly defined by the spec? */
        /* Node j indicates anon+mismatch on entry: */
        gasnet_barrier_notify(0, GASNET_BARRIERFLAG_ANONYMOUS |
                              ((j == mynode) ? GASNET_BARRIERFLAG_MISMATCH : 0));
        result = my_barrier_wait(0, GASNET_BARRIERFLAG_ANONYMOUS |
                                 ((j == mynode) ? GASNET_BARRIERFLAG_MISMATCH : 0));
        if (result != GASNET_ERR_BARRIER_MISMATCH) {
          MSG("ERROR: Failed to detect anonymous barrier mismatch indicated on notify by node %d.");
          gasnet_exit(1);
        }
#endif
      }
    } else if (i == 0) { /* DOB: only warn once per run */
      MSG0("WARNING: pair mismatch tests skipped (only 1 node)");
    }

    if (nodes > 2 && 
        i < MAX(2,iters/1000)) { // limit iterations of the node^2 test below
      int j, k;

      for (j = 0; j < nodes; ++j) {
        for (k = 0; k < nodes; ++k) {
	  if (k == j) continue;

         if (PERFORM_MIXED_NAMED_ANON_TESTS) {
          /* Mix two names and anonymous: */
          if (mynode == j) {
            gasnet_barrier_notify(1592, 0);
            result = my_barrier_wait(1592, 0);
          } else if (mynode == k) {
            gasnet_barrier_notify(1776, 0);
            result = my_barrier_wait(1776, 0);
          } else {
            gasnet_barrier_notify(12345, GASNET_BARRIERFLAG_ANONYMOUS);
            result = my_barrier_wait(12345, GASNET_BARRIERFLAG_ANONYMOUS);
          }
          if (result != GASNET_ERR_BARRIER_MISMATCH) {
            MSG("ERROR: Failed to detect mismatched names (on %d and %d) intermixed with anon.", j, k);
            gasnet_exit(1);
          }

          /* Mix two names and anonymous (blocking): */
          if (mynode == j) {
            result = gasnet_barrier(1592, 0);
          } else if (mynode == k) {
            result = gasnet_barrier(1776, 0);
          } else {
            result = gasnet_barrier(12345, GASNET_BARRIERFLAG_ANONYMOUS);
          }
          if (result != GASNET_ERR_BARRIER_MISMATCH) {
            MSG("ERROR: Failed to detect mismatched names (on %d and %d) intermixed with anon.", j, k);
            gasnet_exit(1);
          }

          /* Mix one named with many anonymous, of which one gives MISnamed wait: */
          if (mynode == j) {
            gasnet_barrier_notify(511, GASNET_BARRIERFLAG_ANONYMOUS);
            result = my_barrier_wait(511, 0);
            if (result != GASNET_ERR_BARRIER_MISMATCH) {
              MSG("ERROR: Failed to detect anon notify and mis-named wait on node %d with one named notify elsewhere.", k);
              gasnet_exit(1);
            }
          } else {
            gasnet_barrier_notify(42, (mynode == k) ? 0: GASNET_BARRIERFLAG_ANONYMOUS);
            result = my_barrier_wait(42, (mynode == k) ? 0: GASNET_BARRIERFLAG_ANONYMOUS);
            /* neither required not prohibited from signalling an error here. */
          }
         } 
        }
      }
    } else if (i == 0) { /* DOB: only warn once per run */
      MSG0("WARNING: multiway mismatch tests skipped (less than 3 nodes)");
    }
    TEST_PROGRESS_BAR(i, iters);
    BARRIER();
  }

  gex_AM_RequestShort0(myteam, mynode, hidx_done_shorthandler, 0);
  return NULL;
}
