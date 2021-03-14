/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testinternal.c $
 * Description: GASNet internal diagnostic tests
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>
#include <gasnet_tools.h>

#include <test.h>

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

/* ------------------------------------------------------------------------------------ */
int main(int argc, char **argv) {
  int iters = 0, threads=0;
  int arg = 1;
  gex_AM_Entry_t *htable; int htable_cnt;
  char *test_sections = NULL;
  gasnett_diagnostic_gethandlers(&htable, &htable_cnt);

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testinternal", &argc, &argv, 0));
  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, htable_cnt));
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


  gex_Rank_t myrank = gex_TM_QueryRank(myteam);
  gex_Rank_t numrank = gex_TM_QuerySize(myteam);
  gex_Rank_t peer = (myrank + 1) % numrank;

  BARRIER();
  void *myseg = TEST_SEG(myrank);
  void *peerseg = TEST_SEG(peer);
  test_errs = gasnett_run_diagnostics(iters, threads, test_sections, myteam,
                                      myseg, peer, peerseg);
  BARRIER();

  if (test_errs) ERR("gasnett_run_diagnostics(%i) failed.", iters);

  gasnet_exit(test_errs);
  return 0;
}
