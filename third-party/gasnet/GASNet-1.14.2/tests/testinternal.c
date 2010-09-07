/*   $Source: /var/local/cvs/gasnet/tests/testinternal.c,v $
 *     $Date: 2010/04/26 04:00:35 $
 * $Revision: 1.5.68.2 $
 * Description: GASNet internal diagnostic tests
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet.h>
#include <gasnet_tools.h>

#include <test.h>

/* ------------------------------------------------------------------------------------ */
int main(int argc, char **argv) {
  int iters = 0, threads=0;
  int arg = 1;
  gasnet_handlerentry_t *htable; int htable_cnt;
  char *test_sections = NULL;
  gasnett_diagnostic_gethandlers(&htable, &htable_cnt);

  GASNET_Safe(gasnet_init(&argc, &argv));
  GASNET_Safe(gasnet_attach(htable, htable_cnt, 
                            TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
  #if GASNET_PAR
    test_init("testinternal",0,"(iters) (threadcnt) (test_sections)");
  #else
    test_init("testinternal",0,"(iters) (test_sections)");
  #endif
  TEST_PRINT_CONDUITINFO();

  if (argc > arg) iters = atoi(argv[arg++]);
  if (iters < 1) iters = 1000;
  #if GASNET_PAR
  if (argc > arg) threads = atoi(argv[arg++]);
  #endif
  if (threads < 1) threads = 4;
  #if GASNET_PAR
  threads = test_thread_limit(threads);
  #endif
  if (argc > arg) test_sections = argv[arg++];

  #if GASNET_PAR
    MSG0("Running GASNet internal diagnostics with iters=%i and threads=%i", iters, threads);
  #else
    MSG0("Running GASNet internal diagnostics with iters=%i", iters);
  #endif

  BARRIER();
  test_errs = gasnett_run_diagnostics(iters, threads, test_sections, TEST_SEGINFO());
  BARRIER();

  if (test_errs) ERR("gasnett_run_diagnostics(%i) failed.", iters);

  gasnet_exit(test_errs);
  return 0;
}
