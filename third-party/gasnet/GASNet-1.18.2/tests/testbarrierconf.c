/*   $Source: /var/local/cvs/gasnet/tests/testbarrierconf.c,v $
 *     $Date: 2008/10/28 05:43:47 $
 * $Revision: 1.14 $
 * Description: GASNet barrier performance test
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet.h>

#include <test.h>

#if defined(GASNETE_USING_ELANFAST_BARRIER) 
  #define PERFORM_MIXED_NAMED_ANON_TESTS (!GASNETE_USING_ELANFAST_BARRIER())
#elif defined(GASNETE_USING_DCMFFAST_BARRIER)
  #define PERFORM_MIXED_NAMED_ANON_TESTS (!GASNETE_USING_DCMFFAST_BARRIER())
#else
  #define PERFORM_MIXED_NAMED_ANON_TESTS 1
#endif

int main(int argc, char **argv) {
  int mynode, nodes;
  int result;
  int iters = -1;
  int i = 0;

  GASNET_Safe(gasnet_init(&argc, &argv));
  GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
  test_init("testbarrierconf", 0, "(iters)");

  mynode = gasnet_mynode();
  nodes = gasnet_nodes();
  if (argc > 1) {
    iters = atoi(argv[1]);
  }
  if (iters < 0) {
    iters = 1000;
  }
  if (argc > 2) test_usage();

  if (mynode == 0) {
      printf("Running barrier conformance test with %d iterations...\n", iters);
      fflush(stdout);
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
    result = gasnet_barrier_wait(0, !mynode ? GASNET_BARRIERFLAG_MISMATCH : 0);
    if (result != GASNET_ERR_BARRIER_MISMATCH) {
      MSG("ERROR: Failed to detect barrier mismatch indicated on notify.");
      gasnet_exit(1);
    }

    /* ids differ between notify and wait */
    gasnet_barrier_notify(0, 0);
    result = gasnet_barrier_wait(1, 0);
    if (result != GASNET_ERR_BARRIER_MISMATCH) {
      MSG("ERROR: Failed to detect mismatch between id at notify and wait.");
      gasnet_exit(1);
    }

    /* Flags differ between notify and wait: */
    gasnet_barrier_notify(0, GASNET_BARRIERFLAG_ANONYMOUS);
    result = gasnet_barrier_wait(0, 0);
    if (result != GASNET_ERR_BARRIER_MISMATCH) {
      MSG("ERROR: Failed to detect anonymous notify with named wait.");
      gasnet_exit(1);
    }
    gasnet_barrier_notify(0, 0);
    result = gasnet_barrier_wait(0, GASNET_BARRIERFLAG_ANONYMOUS);
    if (result != GASNET_ERR_BARRIER_MISMATCH) {
      MSG("ERROR: Failed to detect named notify with anonymous wait.");
      gasnet_exit(1);
    }

    if (nodes > 1) {
      int j;

      for (j = 0; j < nodes; ++j) {
       if (PERFORM_MIXED_NAMED_ANON_TESTS) {
        /* Mix many named with one anonymous: */
        if (mynode == j) {
          gasnet_barrier_notify(12345, GASNET_BARRIERFLAG_ANONYMOUS);
          result = gasnet_barrier_wait(12345, GASNET_BARRIERFLAG_ANONYMOUS);
        } else {
          gasnet_barrier_notify(5551212, 0);
          result = gasnet_barrier_wait(5551212, 0);
        }
        if (result != GASNET_OK) {
          MSG("ERROR: Failed to match anon notify on node %d with named notify elsewhere.", j);
          gasnet_exit(1);
        }

        /* Mix one named with many anonymous: */
        if (mynode == j) {
          gasnet_barrier_notify(0xcafef00d, 0);
          result = gasnet_barrier_wait(0xcafef00d, 0);
        } else {
          gasnet_barrier_notify(911, GASNET_BARRIERFLAG_ANONYMOUS);
          result = gasnet_barrier_wait(911, GASNET_BARRIERFLAG_ANONYMOUS);
        }
        if (result != GASNET_OK) {
          MSG("ERROR: Failed to match named notify on node %d with anon notify elsewhere.", j);
          gasnet_exit(1);
        }
      }
        /* Mismatched id: */
        gasnet_barrier_notify(mynode == j, 0);
        result = gasnet_barrier_wait(mynode == j, 0);
        if (result != GASNET_ERR_BARRIER_MISMATCH) {
          MSG("ERROR: Failed to detect different id on node %d.", j);
          gasnet_exit(1);
        }
      }
    } else if (i == 0) { /* DOB: only warn once per run */
      MSG("WARNING: pair mismatch tests skipped (only 1 node)");
    }

    if (nodes > 2) {
      int j, k;

      for (j = 0; j < nodes; ++j) {
        for (k = 0; k < nodes; ++k) {
	  if (k == j) continue;

         if (PERFORM_MIXED_NAMED_ANON_TESTS) {
          /* Mix two names and anonymous: */
          if (mynode == j) {
            gasnet_barrier_notify(1592, 0);
            result = gasnet_barrier_wait(1592, 0);
          } else if (mynode == k) {
            gasnet_barrier_notify(1776, 0);
            result = gasnet_barrier_wait(1776, 0);
          } else {
            gasnet_barrier_notify(12345, GASNET_BARRIERFLAG_ANONYMOUS);
            result = gasnet_barrier_wait(12345, GASNET_BARRIERFLAG_ANONYMOUS);
          }
          if (result != GASNET_ERR_BARRIER_MISMATCH) {
            MSG("ERROR: Failed to detect mismatched names intermixed with anon.");
            gasnet_exit(1);
          }
         } 
        }
      }
    } else if (i == 0) { /* DOB: only warn once per run */
      MSG("WARNING: multiway mismatch tests skipped (less than 3 nodes)");
    }
    TEST_PROGRESS_BAR(i, iters);
    BARRIER();
  }

  MSG("done.");

  gasnet_exit(0);
  return 0;
}
