/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testimm.c $
 * Description: GASNet Active Messages IMMEDIATE test
 * Copyright (c) 2018, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

// TODOs:
//  + Additional schedule options:
//    - Randomized
//      May have static randomization at start of run, or dynamic per-operation
//    - Work-sharing option
//      Rather than statically partitioning the ops per-rank, just keep a total count
//    - "nbrhd-balanced" in which consecutive operations are to distinct nbrhds
//      This should reduce ingress bottlenecks, but may not "fit" if nbrhds have unequal sizes
//  + Synchronous (GEX_EVENT_NOW) operation(s)

#include <gasnetex.h>

static size_t param_SZ = 0;
#define TEST_SEGSZ_EXPR ((uintptr_t)param_SZ)

#include <test.h>

static gex_Client_t      myclient;
static gex_EP_t          myep;
static gex_TM_t          myteam;
static gex_Segment_t     mysegment;

gex_Rank_t myrank = 0;
gex_Rank_t numrank = 0;

gex_RankInfo_t *nbrhdinfo = NULL;
gex_Rank_t nbrhdsize; // size of the neighborhood, or 1 for PSHM-only

static int64_t param_Z = 0;
static long param_B = 0;
static long param_N = 0;

static int in_segment = 1;

static enum {
  TEST_POLL_NEXT,
  TEST_POLL_RETRY,
  TEST_POLL_LAZY,
  TEST_POLL_ALWAYS
} poll_mode;

static void *local_addr;
static long *remain;

#define hidx_noop_handler 200

void noop_handler(gex_Token_t token, void *buf, size_t nbytes) {
  // does absolutely nothing
}

void doMed(gex_Flags_t imm_flag);
void doLong(gex_Flags_t imm_flag);
void doPut(gex_Flags_t imm_flag);
void doGet(gex_Flags_t imm_flag);

int main(int argc, char **argv) {
  gex_AM_Entry_t htable[] = { 
    { hidx_noop_handler, noop_handler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDLONG, 0 }
  };

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testimm", &argc, &argv, 0));

  myrank = gex_TM_QueryRank(myteam);
  numrank = gex_TM_QuerySize(myteam);

  int enable_given = 0;
  int enable_med   = 0;
  int enable_long  = 0;
  int enable_put   = 0;
  int enable_get   = 0;

  int force_mixed  = 0;

  int help = 0;
  int arg = 1;
  while (argc > arg) {
    if (!strcmp(argv[arg], "-z")) {
      ++arg;
      if (argc > arg) { param_Z = atol(argv[arg]); arg++; }
      else help = 1;
      param_Z = MAX(param_Z, 0);
    } else if (!strcmp(argv[arg], "-b")) {
      ++arg;
      if (argc > arg) { param_B = atol(argv[arg]); arg++; }
      else help = 1;
    } else if (!strcmp(argv[arg], "-m")) {
      ++arg;
      enable_med = enable_given = 1;
    } else if (!strcmp(argv[arg], "-l")) {
      ++arg;
      enable_long = enable_given = 1;
    } else if (!strcmp(argv[arg], "-p")) {
      ++arg;
      enable_put = enable_given = 1;
    } else if (!strcmp(argv[arg], "-g")) {
      ++arg;
      enable_get = enable_given = 1;
    } else if (!strcmp(argv[arg], "-mixed")) {
      ++arg;
      force_mixed = 1;
    } else if (!strcmp(argv[arg], "-in")) {
      ++arg;
      in_segment = 1;
    } else if (!strcmp(argv[arg], "-out")) {
      ++arg;
      in_segment = 0;
    } else if (!strcmp(argv[arg], "-poll-next")) {
      ++arg;
      poll_mode = TEST_POLL_NEXT;
    } else if (!strcmp(argv[arg], "-poll-retry")) {
      ++arg;
      poll_mode = TEST_POLL_RETRY;
    } else if (!strcmp(argv[arg], "-poll-lazy")) {
      ++arg;
      poll_mode = TEST_POLL_LAZY;
    } else if (!strcmp(argv[arg], "-poll-always")) {
      ++arg;
      poll_mode = TEST_POLL_ALWAYS;
    } else if (argv[arg][0] == '-') {
      help = 1;
      ++arg;
    } else break;
  }

  if (!enable_given) {
    enable_med = enable_long = 1;
  }

  if (argc > arg) { param_N = atol(argv[arg]); ++arg; }
  if (!param_N) param_N = 10000;

  if (argc > arg) { param_SZ = atoi(argv[arg]); ++arg; }
  if (!param_SZ) { param_SZ = 1024*1024; }
  if (enable_med) {
    param_SZ = MIN(param_SZ, gex_AM_LUBRequestMedium());
  }
  if (enable_long) {
    param_SZ = MIN(param_SZ, gex_AM_LUBRequestLong());
  }

  if (!param_Z) param_Z = 500;

  if (!param_B) param_B = param_N; // what UPC would call indefinite layout

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, sizeof(htable)/sizeof(gex_AM_Entry_t)));

  test_init("testimm",1, "[options] (iters) (msgsz)\n"
             "  The '-in' or '-out' option selects whether the requestor's\n"
             "        buffer is in the GASNet segment or not (default is 'in').\n"
             "  The '-m/-l/-p/-g' options enable, respectively, timing of the\n"
             "        RequestMedium, RequestLong, PutNBI and GetNBI operations.\n"
             "        The default is to test all four operations, except omitting\n"
             "        Put and Get in runs with only shared-memory communication.\n"
             "  -mixed\n"
             "        By default this test excludes passive ranks that are reachable\n"
             "        via shared-memory communication, unless that is *all* ranks.\n"
             "        This option overrides this behavior, allowing the test to use\n"
             "        a mix of shared-memory and network communication.  This test\n"
             "        does not exercise \"best practices\" for this configuration.\n"
             "  -z <interval_us>\n"
             "        Sets the minimum interval (in us) between poll calls by passive\n"
             "        ranks (default is 500).  Actual inter-poll delays may be longer.\n"
             "  -b <blocksz>\n"
             "        Sets block size of nominal communication schedule, where\n"
             "        \"nominal\" means the schedule applied in the absence of\n"
             "        back-pressure indication due to GEX_FLAG_IMMEDIATE.\n"
             "           0 (default) sends all to each rank before advancing\n"
             "           1 sends only one per rank before advancing\n"
             "           N sends N per rank before advancing\n"
             "  -poll-* options select the polling policy (default is -poll-next):\n"
             "    -poll-next   poll upon back pressure, advancing to the next peer\n"
             "    -poll-retry  poll upon back pressure, retrying the same peer once\n"
             "    -poll-lazy   advance to the next peer upon back pressure,\n"
             "                 but poll only between loops over peers\n"
             "    -poll-always advance to the next peer upon back pressure,\n"
             "                 but poll before every IMMEDIATE operation\n"
             "  Note that maxsz will be reduced if RequestMedium or RequestLong are\n"
             "  to be timed and maxsz would exceed the respective LUBRequest limit.\n"
           );
  if (help || argc > arg) test_usage();

  if (numrank < 2) {
    MSG0("WARNING: This test requires two or more ranks.  Test skipped.\n");
    gasnet_exit(0); /* exit 0 to prevent false negatives in test harness */
  }

  TEST_PRINT_CONDUITINFO();

  char *space = NULL;
  char nbrhd_warning[64] = "";
  gex_System_QueryNbrhdInfo(&nbrhdinfo, &nbrhdsize, NULL);
  int nbrhd_only = (nbrhdsize == numrank);
  if (!myrank) {
    if (nbrhdsize == 1) {
      // The passive ranks are all OUTSIDE our neighborhood
    } else if (nbrhdsize == numrank) {
      // The passive ranks are all INSIDE our neighborhood
      nbrhdinfo = NULL; // suppress filtering
      nbrhdsize = 1;    // and correct reported passive rank count
      assert(nbrhd_only);
    #if !GASNET_CONDUIT_SMP // would be "just noise" for smp-conduit
      strcpy(nbrhd_warning, "\n  WARNING: all ranks are reachable via shared-memory");
    #endif
    } else if (force_mixed) {
      // Mixed case: but commandline ask us not to omit ranks within our neighborhood
      snprintf(nbrhd_warning, sizeof(nbrhd_warning),
               "\n  WARNING: %d shared-memory rank%s allowed by -mixed option",
               (nbrhdsize-1), (nbrhdsize>2)?"s":"");
      nbrhdinfo = NULL; // suppress filtering
      nbrhdsize = 1;    // and correct reported passive rank count
    } else {
      // Mixed case: we will omit ranks within our neighborhood
      snprintf(nbrhd_warning, sizeof(nbrhd_warning),
               "\n  WARNING: %d shared-memory rank%s omitted",
               (nbrhdsize-1), (nbrhdsize>2)?"s":"");
    }

    if (in_segment) {
      local_addr = TEST_MYSEG();
    } else {
      space = test_malloc(param_SZ + PAGESZ);
      local_addr = alignup_ptr(space, PAGESZ);
    }
    remain = test_malloc(sizeof(long) * numrank);
  }

  if (!enable_given) {
    enable_put = enable_get = !nbrhd_only;
  }

  MSG0("Running testimm with %d ranks\n"
       "  active rank issues %ld operations per rank, each of length %ld\n"
       "  active rank's local addresses %sside the segment\n"
       "  nominal schedule issues operations in blocks of %ld\n"
       "  polling policy is \"%s\"\n"
       "  %d passive rank%s sleep (at least) %"PRIi64"us between polls\n"
       "  operations timed:%s%s%s%s"
       "%s",
       numrank,
       param_N, (long)param_SZ,
       (in_segment ? "in" : "out"),
       param_B,
       (poll_mode==TEST_POLL_NEXT   ? "next" :
       (poll_mode==TEST_POLL_RETRY  ? "retry" :
       (poll_mode==TEST_POLL_ALWAYS ? "always" :
                                      "lazy"))),
       (numrank-nbrhdsize), (numrank-nbrhdsize>1)?"s":"", param_Z,
       enable_med?" RequestMedium":"", enable_long?" RequestLong":"",
       enable_put?" PutNBI":"", enable_get?" GetNBI":"",
       nbrhd_warning
      );

  if (enable_med) {
    doMed(0);
    doMed(GEX_FLAG_IMMEDIATE);
  }
  if (enable_long) {
    doLong(0);
    doLong(GEX_FLAG_IMMEDIATE);
  }
  if (enable_put) {
    doPut(0);
    doPut(GEX_FLAG_IMMEDIATE);
  }
  if (enable_get) {
    doGet(0);
    doGet(GEX_FLAG_IMMEDIATE);
  }

  if (!myrank) {
    test_free(space);
    test_free(remain);
  }

  BARRIER();
  MSG("done.");

  gasnet_exit(0);
  return 0;
}

void passive(void) {
  uint64_t interval_ns = 1000 * param_Z;
  gex_Event_t bar = gex_Coll_BarrierNB(myteam,0);
  do {
    gasnett_nsleep(interval_ns);
    gasnet_AMPoll();
  } while (gex_Event_Test(bar) != GASNET_OK);
}

#define ACTIVE(OPERATION, SYNC) do {                    \
  int done;                                             \
  do {                                                  \
    done = 1;                                           \
    for (gex_Rank_t r = 1; r < numrank; ++r) {          \
      long count, limit = MIN(param_B, remain[r]);      \
      for (count = 0; count < limit; ++count) {         \
        int did_retry = 0;                              \
        if ((poll_mode == TEST_POLL_ALWAYS) && imm_flag)\
          gasnet_AMPoll();                              \
      retry:                                            \
        if ( OPERATION ) {                              \
          if (did_retry) break;                         \
          if (poll_mode == TEST_POLL_LAZY) break;       \
          gasnet_AMPoll();                              \
          if (poll_mode == TEST_POLL_NEXT) break;       \
          did_retry = 1;                                \
          goto retry;                                   \
        }                                               \
      }                                                 \
      remain[r] -= count;                               \
      done &= !remain[r];                               \
    }                                                   \
    if ((poll_mode == TEST_POLL_LAZY) && imm_flag)      \
      gasnet_AMPoll();                                  \
  } while (!done);                                      \
  gex_NBI_Wait(SYNC,0);                                 \
} while (0)

void init_remain(void) {
  for (gex_Rank_t r = 1; r < numrank; ++r) remain[r] = param_N;
  if (nbrhdinfo) { // Optionally exclude PSHM-peers by zeroing their remain[]
    for (gex_Rank_t i = 0; i < nbrhdsize; ++i) remain[nbrhdinfo[i].gex_jobrank] = 0;
  }
}

void report(const char *name, gex_Flags_t imm_flag, double elapsed, double *prev) {
  char delta[32];
  if (*prev > 0.0) {
    snprintf(delta, sizeof(delta), ", %+.2f%% improvement", 100.*((*prev) - elapsed)/(*prev));
  } else {
    delta[0] = '\0';
  }
  double bw = ((double)param_N * param_SZ) / (elapsed * 1024 * 1024);
  printf("%-8s IMM=%c completed in %7.3f seconds (%7.3f MiB/s)%s\n",
         name, imm_flag?'Y':'N', elapsed, bw, delta);
  *prev = elapsed;
}

void doMed(gex_Flags_t imm_flag) {
  if (myrank) {
    passive();
  } else {
    init_remain();
    gasnett_tick_t start_ticks = gasnett_ticks_now();
    ACTIVE( gex_AM_RequestMedium0(myteam, r, hidx_noop_handler, local_addr, param_SZ,
                                  GEX_EVENT_GROUP, imm_flag),
            GEX_EC_AM );
    gasnett_tick_t end_ticks = gasnett_ticks_now();
    double elapsed = 1e-9 * gasnett_ticks_to_ns(end_ticks - start_ticks);

    static double prev;
    report("MEDIUM:", imm_flag, elapsed, &prev);

    gex_Event_Wait(gex_Coll_BarrierNB(myteam,0));
  }
}

void doLong(gex_Flags_t imm_flag) {
  if (myrank) {
    passive();
  } else {
    init_remain();
    gasnett_tick_t start_ticks = gasnett_ticks_now();
    ACTIVE( gex_AM_RequestLong0(myteam, r, hidx_noop_handler, local_addr, param_SZ,
                                TEST_SEG(r), GEX_EVENT_GROUP, imm_flag),
            GEX_EC_AM );
    gasnett_tick_t end_ticks = gasnett_ticks_now();
    double elapsed = 1e-9 * gasnett_ticks_to_ns(end_ticks - start_ticks);

    static double prev;
    report("LONG:", imm_flag, elapsed, &prev);

    gex_Event_Wait(gex_Coll_BarrierNB(myteam,0));
  }
}

void doPut(gex_Flags_t imm_flag) {
  if (myrank) {
    passive();
  } else {
    init_remain();
    gasnett_tick_t start_ticks = gasnett_ticks_now();
    ACTIVE( gex_RMA_PutNBI(myteam, r, TEST_SEG(r), local_addr, param_SZ,
                           GEX_EVENT_DEFER, imm_flag),
            GEX_EC_PUT );
    gasnett_tick_t end_ticks = gasnett_ticks_now();
    double elapsed = 1e-9 * gasnett_ticks_to_ns(end_ticks - start_ticks);

    static double prev;
    report("PUT:", imm_flag, elapsed, &prev);

    gex_Event_Wait(gex_Coll_BarrierNB(myteam,0));
  }
}

void doGet(gex_Flags_t imm_flag) {
  if (myrank) {
    passive();
  } else {
    init_remain();
    gasnett_tick_t start_ticks = gasnett_ticks_now();
    ACTIVE( gex_RMA_GetNBI(myteam, local_addr, r, TEST_SEG(r), param_SZ, imm_flag),
            GEX_EC_PUT );
    gasnett_tick_t end_ticks = gasnett_ticks_now();
    double elapsed = 1e-9 * gasnett_ticks_to_ns(end_ticks - start_ticks);

    static double prev;
    report("GET:", imm_flag, elapsed, &prev);

    gex_Event_Wait(gex_Coll_BarrierNB(myteam,0));
  }
}
