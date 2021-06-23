/* $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testsegment.c $
 * Copyright (c) 2020, The Regents of the University of California
 *
 * Description: GASNet "disaggregated Attach" test.
 *
 * This test binds a segment to a primordial endpoint using
 * gex_Segment_Create(), gex_EP_BindSegment(), and gex_EP_PublishBoundSegment()
 * rather than gex_Segment_Attach(), performing RMA, AM and VIS operations
 * to verify correctness.
 */

// WARNING: This test exercises an EXPERIMENTAL feature.
// One should not clone the logic in this test as a template for use of the
// gex_Segment_Create, gex_EP_BindSegment, and gex_EP_PublishBoundSegment APIs

#include <gasnetex.h>
#include <gasnet_coll.h>

// Unused
#ifndef TEST_SEGSZ
#define TEST_SEGSZ PAGESZ
#endif

#include <test.h>

//  ------------------------------------------------------------------------------------

static gex_Client_t  myclient;
static gex_EP_t      myep;
static gex_TM_t      myteam;
static gex_Rank_t    myrank, nranks;

//  ------------------------------------------------------------------------------------

// NOT fully general, but sufficient for this test
#if PLATFORM_ARCH_32
  #define PTR_NARGS       1
  #define PTR_ARGS        gex_AM_Arg_t arg0
  #define PTR_PACK(ptr)   ((gex_AM_Arg_t)(ptr))
  #define PTR_UNPACK()    ((void *)arg0)
#elif PLATFORM_ARCH_64
  #define PTR_NARGS       2
  #define PTR_ARGS        gex_AM_Arg_t arg0, gex_AM_Arg_t arg1
  #define PTR_PACK(ptr)   ((gex_AM_Arg_t)TEST_HIWORD(ptr)), \
                          ((gex_AM_Arg_t)TEST_LOWORD(ptr))
  #define PTR_UNPACK()    ((void *)((((uint64_t)(arg0)) << 32) | \
                                    (((uint64_t)(arg1)) & 0xFFFFFFFF)))
#endif

//  ------------------------------------------------------------------------------------

static volatile int ping_rcvd = 0;
static volatile int pong_rcvd = 0;

#define hidx_ping       200
#define hidx_pong       201

static void ping_handler(gex_Token_t token, void *buf, size_t nbytes, PTR_ARGS) {
  assert_always(! ping_rcvd);
  ping_rcvd = 1;

  // Payload value is our jobrank, address is in the handler arg(s)
  gex_AM_ReplyLong0(token, hidx_pong, &myrank, sizeof(gex_Rank_t), PTR_UNPACK(), GEX_EVENT_NOW, 0);
}

static void pong_handler(gex_Token_t token, void *buf, size_t nbytes) {
  assert_always(! pong_rcvd);
  pong_rcvd = 1;
}

// handler table
gex_AM_Entry_t htable[] = {
  { hidx_ping, ping_handler,   GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_LONG, PTR_NARGS },
  { hidx_pong, pong_handler,   GEX_FLAG_AM_REPLY  |GEX_FLAG_AM_LONG,  0 }
 };
#define HANDLER_TABLE_SIZE (sizeof(htable)/sizeof(gex_AM_Entry_t))

//  ------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
  int seedoffset = 0;
  int client_segment = 1;
  int random_segment = 0;
  int page_align = 0;

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testsegment", &argc, &argv, 0));

  int help = 0;
  int argi = 1;
  while (argc > argi) {
    if (!strcmp(argv[argi], "-client-seg")) {
      client_segment = 1;
      ++argi;
    } else if (!strcmp(argv[argi], "-gasnet-seg")) {
      client_segment = 0;
      ++argi;
    } else if (!strcmp(argv[argi], "-random-seg")) {
      random_segment = 1;
      ++argi;
    } else if (!strcmp(argv[argi], "-page-align")) {
      page_align = 1;
      ++argi;
    } else if (!strcmp(argv[argi], "-no-page-align")) {
      page_align = 0;
      ++argi;
    } else if (argv[argi][0] == '-') {
      help = 1;
      ++argi;
    } else break;
  }

  test_init("testsegment", 0, "[options] (seed)\n"
               " Segment allocation options:\n"
               "     -client-seg:  Test client-allocated segments (default)\n"
               "     -gasnet-seg:  Test GASNet-allocated segments\n"
               "     -random-seg:  Test with a random mix of the two options above\n"
               " Segment alignment options:\n"
               "  -no-page-align:  Use unaligned address and length (default)\n"
               "     -page-align:  Use page-aligned adddress and length\n"
               " seed      seed offset for PRNG \n");
  if (argi < argc) { seedoffset = atoi(argv[argi]); argi++; }
  if (argi < argc || help) test_usage();

  myrank = gex_TM_QueryRank(myteam);
  nranks = gex_TM_QuerySize(myteam);

  if (seedoffset == 0) {
    seedoffset = (((unsigned int)TIME()) & 0xFFFF);
    // TEST_BCAST() requires a gex_Segment_Attach() call, which we intentionally omit
    gex_Event_Wait(gex_Coll_BroadcastNB(myteam, 0, &seedoffset, &seedoffset, sizeof(seedoffset), 0));
  }
  TEST_SRAND(myrank+seedoffset);

  MSG0("Running segment bind/publish test with %saligned address and length, seed=%d",
       page_align?"page-":"un", seedoffset);
  if (random_segment) {
    client_segment = TEST_RAND_ONEIN(2);
    MSG("This process using a %s-allocated segment", client_segment?"client":"gasnet");
  } else {
    MSG0("Using %s-allocated segment on all processes", client_segment?"client":"gasnet");
  }

  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, sizeof(htable)/sizeof(gex_AM_Entry_t)));

  {
    unsigned int offset = page_align ? 0 : 16;

    // Test creation of a GASNet-allocated segment
    gex_Segment_t g_segment = GEX_SEGMENT_INVALID;
    size_t g_segment_size = GASNET_PAGESIZE - offset;
    GASNET_Safe(gex_Segment_Create(&g_segment, myclient, NULL, g_segment_size, GEX_MK_HOST, 0));
    if ((g_segment == GEX_SEGMENT_INVALID) ||
        (gex_Segment_QueryAddr(g_segment) == NULL) ||
        (gex_Segment_QuerySize(g_segment) < g_segment_size)) {
      ERR("FAILED GASNET-ALLOCATED SEGMENT CREATE TEST");
    }

    // Test creation of a client-allocated segment
    // TODO: should also cover client allocation from mmap(), stack and static data.
    gex_Segment_t c_segment = GEX_SEGMENT_INVALID;
    uint8_t *c_segment_mem = (uint8_t *) test_malloc(GASNET_PAGESIZE);
    uint8_t *c_segment_addr = c_segment_mem + offset;
    size_t c_segment_size = GASNET_PAGESIZE - 2*offset;
    GASNET_Safe(gex_Segment_Create(&c_segment, myclient, c_segment_addr, c_segment_size, GEX_MK_HOST, 0));
    if ((c_segment == GEX_SEGMENT_INVALID) ||
        (gex_Segment_QueryAddr(c_segment) != c_segment_addr) ||
        (gex_Segment_QuerySize(c_segment) != c_segment_size)) {
      ERR("FAILED CLIENT-ALLOCATED SEGMENT CREATE TEST");
    }

    // Test pre-bind (no segments yet) Publish
    // Should not fail, nor interfere with the post-Bind use of Publish
    if (GASNET_OK != gex_EP_PublishBoundSegment(myteam, &myep, 1, 0)) {
      ERR("FAILED EARLY SEGMENT PUBLISH TEST");
    }

    // Pick a segment to test and (TODO:) destroy the other
    gex_Segment_t seg;
    void *    seg_addr;
    uintptr_t seg_size;
    if (client_segment) {
      seg      = c_segment;
      seg_addr = c_segment_addr;
      seg_size = c_segment_size;
      // GASNET_Safe(gex_Segment_Destroy(g_segment, 0));
    } else {
      seg      = g_segment;
      seg_addr = gex_Segment_QueryAddr(g_segment);
      seg_size = gex_Segment_QuerySize(g_segment);
      // GASNET_Safe(gex_Segment_Destroy(c_segment, 0));
    }

    // Bind the chosen segments and validate
    gex_EP_BindSegment(myep, seg, 0);
    {
      void *tmp_addr;
      size_t tmp_size;
      gex_Event_Wait( gex_EP_QueryBoundSegmentNB(myteam, myrank, &tmp_addr, NULL, &tmp_size, 0) );
      if ((seg != gex_EP_QuerySegment(myep)) ||
          (tmp_addr != seg_addr) ||
          (tmp_size != seg_size)) {
        ERR("FAILED SEGMENT EP BIND TEST");
      }
    }

    // Publish the segment over a permuted temporary team,
    // consisting all odds in reverse order followed by evens in reverse order.
    // Additionally, we perform the Publish in two calls with random membership.
    {
      gex_TM_t tmp_tm = GEX_TM_INVALID;
      int key = (myrank & 1 ? 0 : nranks) + (nranks - myrank);
      gex_TM_Split(&tmp_tm, myteam, 0, key, NULL, 0, GEX_FLAG_TM_NO_SCRATCH);
      assert_always(tmp_tm != GEX_TM_INVALID);
      assert_always(nranks == gex_TM_QuerySize(tmp_tm));
      int coin_flip = TEST_RAND_ONEIN(2);
      if ((GASNET_OK != gex_EP_PublishBoundSegment(tmp_tm, &myep,  coin_flip, 0)) ||
          (GASNET_OK != gex_EP_PublishBoundSegment(tmp_tm, &myep, !coin_flip, 0))) {
        ERR("FAILED PERMUTED SEGMENT PUBLISH TEST");
      }
      GASNET_Safe(gex_TM_Destroy(tmp_tm, NULL, 0));
    }

    // Prepare for comms
    gex_Rank_t peer = (myrank + 1) % nranks;
    void *loc_base, *rem_base;
    gex_Event_Wait( gex_EP_QueryBoundSegmentNB(myteam, peer, &rem_base, NULL, NULL, 0) );
    loc_base = seg_addr;

    // TODO: validate gex_EP_QueryBoundSegmentNB() for some non-trivial set of
    // processes, not just self and the one peer chosen for communication.

    // TODO: fix PSHM support and remove this mess:
    int sender = 1; // Initiates Put, Get and Request
    int target = 1; // Target of a sender
    gex_Rank_t nbrhd_set_size;
    gex_System_QueryMyPosition(&nbrhd_set_size, NULL, NULL, NULL);
    if (nbrhd_set_size != nranks) {
      // More ranks than nbrhds => at least one non-singleton nbrhd
      gex_Rank_t anti_peer = (myrank + nranks - 1) % nranks; // Sends us a Request
      gex_RankInfo_t *nbrhdinfo;
      gex_Rank_t nbrhdsize;
      gex_System_QueryNbrhdInfo(&nbrhdinfo, &nbrhdsize, NULL);
      for (gex_Rank_t i = 0; i < nbrhdsize; ++i) {
        if (nbrhdinfo[i].gex_jobrank == anti_peer) {
          target = 0;
        }
        if (nbrhdinfo[i].gex_jobrank == peer) {
          // Would communicate w/i the nbrhd, which is not currently supported
          MSG("WARNING: skipping intra-nbrhd initiator role on this process.");
          sleep(2);
          sender = 0;
        }
      }
    #if !GASNET_CONDUIT_SMP
      MSG0("NOTE: setting GASNET_SUPERNODE_MAXSIZE=1 will enable skipped tests.");
    #endif
    }

    // Put, Get and AMLong to exercise the segment
    // TODO: Test collectives w/ scratch space carved out of the segment.
    // TODO: Can Long payloads be made large to prevent packed-long optimizations?
    {
      gex_Rank_t *loc_array = (gex_Rank_t *)loc_base;
      loc_array[0] = myrank;            // Source of Gets
      loc_array[1] = GEX_RANK_INVALID;  // Destination of Put
      loc_array[2] = GEX_RANK_INVALID;  // Destination of RequestLong
      loc_array[3] = GEX_RANK_INVALID;  // Destination of ReplyLong
      loc_array[4] = GEX_RANK_INVALID;  // Destination of loopback Put

      BARRIER();

      if (sender) {
        int failed = 0;
        gex_Rank_t *rem_array = (gex_Rank_t *)rem_base;
        gex_Rank_t read_val;

        gex_Event_t get_ev =
              gex_RMA_GetNB(myteam, &read_val, peer, rem_array, sizeof(gex_Rank_t), 0);
        gex_RMA_PutBlocking(myteam, peer, rem_array + 1, &peer, sizeof(gex_Rank_t), 0);
        gex_AM_RequestLong(myteam, peer, hidx_ping, &peer, sizeof(gex_Rank_t), rem_array + 2,
                           GEX_EVENT_NOW, 0, PTR_PACK(loc_array + 3));

        // Validate Get result
        gex_Event_Wait(get_ev);
        failed += (read_val != peer);

        // Validate AMReplyLong payload
        GASNET_BLOCKUNTIL(pong_rcvd);
        failed += (loc_array[3] != peer);

        if (failed) {
          ERR("Initiator checks FAILED %d test(s).", failed);
        } else {
          MSG("Initiator checks passed.");
        }
      }

      // Validate Put and AMRequestLong payload (if any)
      // Ping follows blocking Put (Put+Sync+Send), thus proving Put completion
      if (target) {
        int failed = 0;
        GASNET_BLOCKUNTIL(ping_rcvd);
        failed += (loc_array[1] != myrank); // Put
        failed += (loc_array[2] != myrank); // RequestLong payload
        if (failed) {
          ERR("Target checks FAILED %d test(s).", failed);
        } else {
          MSG("Target checks passed.");
        }
      }

      {
        // Alawys test loopback
        int failed = 0;
        gex_Rank_t read_val = gex_RMA_GetBlockingVal(myteam, myrank, loc_array, sizeof(gex_Rank_t), 0);
        failed += (read_val != myrank);
        gex_RMA_PutBlockingVal(myteam, myrank, loc_array + 4, myrank, sizeof(gex_Rank_t), 0);
        failed += (loc_array[4] != myrank);
        if (failed) {
          ERR("Loopback checks FAILED %d test(s).", failed);
        } else {
          MSG("Loopback checks passed.");
        }
      }
    }

    // Test no-op (redundant) Publish
    if (GASNET_OK != gex_EP_PublishBoundSegment(myteam, &myep, 1, 0)) {
      ERR("FAILED NO-OP SEGMENT PUBLISH TEST");
    }
  }

  BARRIER();
  MSG("done.");

  gasnet_exit(0);
  return 0;
}
