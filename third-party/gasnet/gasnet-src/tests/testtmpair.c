/* $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testtmpair.c $
 * Copyright (c) 2020, The Regents of the University of California
 *
 * Description: Test of gex_TM_Pair() for communication initiation
 */

#include <gasnetex.h>
#include <gasnet_vis.h>

struct test_segment {
    gex_Rank_t local[6];     // Local in-segment temporaries

    gex_Rank_t get_src;      // Source of various Gets
    gex_Rank_t put_dst[6];   // Destinations of various Puts x6

    gex_Rank_t fp_req_dst;   // FPAM LongRequest dst
    gex_Rank_t fp_rep_dst;   // FPAM LongReply dst
    gex_Rank_t np_req_dst;   // NPAM LongRequest dst
    gex_Rank_t np_rep_dst;   // NPAM LongReply dst

    gex_Rank_t vis_src;      // VIS Gets
    gex_Rank_t vis_dst[6];   // VIS Puts x6
};

#ifndef TEST_SEGSZ
#define TEST_SEGSZ_EXPR sizeof(struct test_segment)
#endif

#include <test.h>

#if 1
  // To count failures, not reporting detail
  #define CHECK(cond) failed += !(cond)
#else
  // To stop at first failure
  #define CHECK(cond) assert(cond)
#endif

//  ------------------------------------------------------------------------------------

static gex_Client_t  myclient;
static gex_Segment_t mysegment;
static gex_EP_t      myep;
static gex_TM_t      myteam;
static gex_Rank_t    myrank, nranks;
static gex_Rank_t    next, prev;

//  ------------------------------------------------------------------------------------

static gex_Rank_t *fp_reply_dst;
static gex_Rank_t *np_reply_dst;

enum {
  hidx_short_ping = GEX_AM_INDEX_BASE,
  hidx_short_pong,
  hidx_med_ping,
  hidx_med_pong,
  hidx_long_ping,
  hidx_long_pong,
  hidx_npmed_ping,
  hidx_npmed_pong,
  hidx_nplong_ping,
  hidx_nplong_pong,
};

#define HANDLER_DECLS(hname) \
  static gasnett_atomic_t hname##_cntr = gasnett_atomic_init(0); \
  static gex_Rank_t hname##_from = GEX_RANK_INVALID;

#define HANDLER_COMMON(hname) do { \
  gex_Token_Info_t info; \
  gex_TI_t rc = gex_Token_Info(token, &info, GEX_TI_SRCRANK); \
  hname##_from = info.gex_srcrank; \
  gasnett_atomic_increment(&hname##_cntr, GASNETT_ATOMIC_REL); \
} while (0)

HANDLER_DECLS(short_ping)
static void short_ping_handler(gex_Token_t token) {
  HANDLER_COMMON(short_ping);
  gex_AM_ReplyShort0(token, hidx_short_pong, 0);
}

HANDLER_DECLS(short_pong)
static void short_pong_handler(gex_Token_t token) {
  HANDLER_COMMON(short_pong);
}

HANDLER_DECLS(med_ping)
static void med_ping_handler(gex_Token_t token, void *buf, size_t nbytes) {
  HANDLER_COMMON(med_ping);
  gex_AM_ReplyMedium0(token, hidx_med_pong, buf, nbytes, GEX_EVENT_NOW, 0);
  assert(myrank == *(gex_Rank_t*)buf);
}

HANDLER_DECLS(med_pong)
static void med_pong_handler(gex_Token_t token, void *buf, size_t nbytes) {
  HANDLER_COMMON(med_pong);
  assert(next == *(gex_Rank_t*)buf);
}

HANDLER_DECLS(long_ping)
static void long_ping_handler(gex_Token_t token, void *buf, size_t nbytes) {
  HANDLER_COMMON(long_ping);
  gex_AM_ReplyLong0(token, hidx_long_pong, buf, nbytes, fp_reply_dst, GEX_EVENT_NOW, 0);
  assert(myrank == *(gex_Rank_t*)buf);
}

HANDLER_DECLS(long_pong)
static void long_pong_handler(gex_Token_t token, void *buf, size_t nbytes) {
  HANDLER_COMMON(long_pong);
  assert(next == *(gex_Rank_t*)buf);
}

HANDLER_DECLS(npmed_ping)
static void npmed_ping_handler(gex_Token_t token, void *buf, size_t nbytes) {
  HANDLER_COMMON(npmed_ping);
  // Not bothering w/ the complexity of NPAM here since token is indep of TM-Pair
  gex_AM_ReplyMedium0(token, hidx_npmed_pong, buf, nbytes, GEX_EVENT_NOW, 0);
  assert(myrank == *(gex_Rank_t*)buf);
}

HANDLER_DECLS(npmed_pong)
static void npmed_pong_handler(gex_Token_t token, void *buf, size_t nbytes) {
  HANDLER_COMMON(npmed_pong);
  assert(next == *(gex_Rank_t*)buf);
}

HANDLER_DECLS(nplong_ping)
static void nplong_ping_handler(gex_Token_t token, void *buf, size_t nbytes) {
  HANDLER_COMMON(nplong_ping);
  // Not bothering w/ the complexity of NPAM here since token is indep of TM-Pair
  gex_AM_ReplyLong0(token, hidx_nplong_pong, buf, nbytes, np_reply_dst, GEX_EVENT_NOW, 0);
  assert(myrank == *(gex_Rank_t*)buf);
}

HANDLER_DECLS(nplong_pong)
static void nplong_pong_handler(gex_Token_t token, void *buf, size_t nbytes) {
  HANDLER_COMMON(nplong_pong);
  assert(next == *(gex_Rank_t*)buf);
}

// handler table
gex_AM_Entry_t htable[] = {
  { hidx_short_ping,  short_ping_handler,  GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT,  0 },
  { hidx_short_pong,  short_pong_handler,  GEX_FLAG_AM_REPLY  |GEX_FLAG_AM_SHORT,  0 },
  { hidx_med_ping,    med_ping_handler,    GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDIUM, 0 },
  { hidx_med_pong,    med_pong_handler,    GEX_FLAG_AM_REPLY  |GEX_FLAG_AM_MEDIUM, 0 },
  { hidx_long_ping,   long_ping_handler,   GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_LONG,   0 },
  { hidx_long_pong,   long_pong_handler,   GEX_FLAG_AM_REPLY  |GEX_FLAG_AM_LONG,   0 },
  { hidx_npmed_ping,  npmed_ping_handler,  GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDIUM, 0 },
  { hidx_npmed_pong,  npmed_pong_handler,  GEX_FLAG_AM_REPLY  |GEX_FLAG_AM_MEDIUM, 0 },
  { hidx_nplong_ping, nplong_ping_handler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_LONG,   0 },
  { hidx_nplong_pong, nplong_pong_handler, GEX_FLAG_AM_REPLY  |GEX_FLAG_AM_LONG,   0 }
 };
#define HANDLER_TABLE_SIZE (sizeof(htable)/sizeof(gex_AM_Entry_t))

//  ------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testtmpair", &argc, &argv, 0));

  // TODO: control over how many EPs and how they are paired (loc,rem)
  test_init("testpair", 0, "(test_sections)");
  if (argc > 1) TEST_SECTION_PARSE(argv[1]);
  if (argc > 2) test_usage();

  myrank = gex_TM_QueryRank(myteam);
  nranks = gex_TM_QuerySize(myteam);

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ));
  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, HANDLER_TABLE_SIZE));

  next = (myrank + 1) % nranks;
  prev = (myrank + nranks - 1) % nranks;

  const int num_eps = MIN(4, GASNET_MAXEPS);  // TODO: command line arg to control this
  gex_EP_t *eps = test_malloc(num_eps * sizeof(gex_EP_t));
  gex_Segment_t *segs = test_malloc(num_eps * sizeof(gex_Segment_t));
  gex_EP_Capabilities_t ep_caps = 0;
  eps[0] = myep;
  segs[0] = mysegment;
  {
    // Limit capabilities to those required by the enabled tests
    for (gex_EP_Index_t idx = 0; idx < num_eps; ++idx) {
      if (TEST_SECTION_BEGIN_ENABLED()) ep_caps |= GEX_EP_CAPABILITY_RMA; // Get
      if (TEST_SECTION_BEGIN_ENABLED()) ep_caps |= GEX_EP_CAPABILITY_RMA; // Put
      if (TEST_SECTION_BEGIN_ENABLED()) ep_caps |= GEX_EP_CAPABILITY_AM;  // AM
      if (TEST_SECTION_BEGIN_ENABLED()) ep_caps |= GEX_EP_CAPABILITY_VIS; // VIS
    }
    test_section = '\0';

    // Limit capabilities to those currently implementd by the current conduit
  #if GASNET_CONDUIT_IBV
    ep_caps &= GEX_EP_CAPABILITY_RMA;
  #elif GASNET_MAXEPS > 1
    MSG0("Update required in testtmpair.c for conduit-specific capabilities.");
  #endif

    for (gex_EP_Index_t idx = 1; idx < num_eps; ++idx) {
      GASNET_Safe(gex_EP_Create(eps+idx, myclient, ep_caps, 0));
    }

    for (gex_EP_Index_t idx = 1; idx < num_eps; ++idx) {
      GASNET_Safe(gex_Segment_Create(segs+idx, myclient, NULL, TEST_SEGSZ_EXPR, GEX_MK_HOST, 0));
      gex_EP_BindSegment(eps[idx], segs[idx], 0);
    }
    gex_EP_PublishBoundSegment(myteam, eps+1, num_eps-1, 0);
  }

  // Paranoia to prevent accidental use:
  myteam = GEX_TM_INVALID;

  for (int iter = 0; iter < num_eps; ++iter) {
    gex_EP_Index_t loc_idx, rem_idx;

    // TODO: "mix it up", such that non-equal local and remote indices communicate.
    // However, that requires addition thought to managing two local segments if we
    // are to continue using in-segment local addresses.
    loc_idx = rem_idx = iter;

    gex_EP_Capabilities_t test_caps = (loc_idx || rem_idx) ? ep_caps : GEX_EP_CAPABILITY_ALL;

    gex_TM_t pair = gex_TM_Pair(eps[loc_idx], rem_idx);

    const size_t rank_sz = sizeof(gex_Rank_t);

    struct test_segment *loc_seg;
    if (!loc_idx) {
      // Required for GASNET_SEGMENT_EVERYTHING, buy always correct
      loc_seg = (struct test_segment *) TEST_MYSEG();
    } else {
      loc_seg = gex_Segment_QueryAddr(segs[loc_idx]);
    }

    struct test_segment *rem_seg;
    struct test_segment *prev_seg;
    if (!rem_idx) {
      // Required for GASNET_SEGMENT_EVERYTHING, buy always correct
      rem_seg = (struct test_segment *) TEST_SEG(next);
      prev_seg = (struct test_segment *) TEST_SEG(prev);
    } else {
      gex_Event_Wait( gex_EP_QueryBoundSegmentNB(pair, next, (void**)&rem_seg, NULL, NULL, 0) );
      gex_Event_Wait( gex_EP_QueryBoundSegmentNB(pair, prev, (void**)&prev_seg, NULL, NULL, 0) );
    }

    loc_seg->get_src = myrank;
    for (int i = 0; i < 6; ++i) {
      loc_seg->put_dst[i] = GEX_RANK_INVALID;
    }
    loc_seg->fp_req_dst = GEX_RANK_INVALID;
    loc_seg->fp_req_dst = GEX_RANK_INVALID;
    loc_seg->np_rep_dst = GEX_RANK_INVALID;
    loc_seg->np_rep_dst = GEX_RANK_INVALID;
    loc_seg->vis_src = prev;
    for (int i = 0; i < 6; ++i) {
      loc_seg->vis_dst[i] = GEX_RANK_INVALID;
    }

    // Cannot use QueryBound in handler context
    fp_reply_dst = &prev_seg->fp_rep_dst;
    np_reply_dst = &prev_seg->np_rep_dst;

    BARRIER();

    // RMA Get tests
    if (!TEST_SECTION_BEGIN_ENABLED()) {
      // Nothing to do
    } else if (! (test_caps & GEX_EP_CAPABILITY_RMA)) {
      MSG0("%c: Skipping RMA Get tests for pair (%d,%d) - RMA not yet supported for non-primordial EPs",
           TEST_SECTION_NAME(), loc_idx, rem_idx);
    } else {
      MSG0("%c: Starting RMA Get tests for pair (%d,%d)",
           TEST_SECTION_NAME(), loc_idx, rem_idx);

      int rc;
      gex_Event_t ev;
      gex_Rank_t *dst_array = loc_seg->local;
      gex_Rank_t *src_addr = &rem_seg->get_src;

      for (int i = 0; i < 4; ++i) {
        dst_array[i] = GEX_RANK_INVALID;
      }
      gex_Rank_t *dst = &dst_array[0];

      ev =                gex_RMA_GetNB(pair, dst++, next, src_addr, rank_sz, 0);
      rc =               gex_RMA_GetNBI(pair, dst++, next, src_addr, rank_sz, 0);
      assert(!rc);
      rc =          gex_RMA_GetBlocking(pair, dst++, next, src_addr, rank_sz, 0);
      assert(!rc);
      *(dst++) = gex_RMA_GetBlockingVal(pair,        next, src_addr, rank_sz, 0);

      gex_Event_Wait(ev);
      gex_NBI_Wait(GEX_EC_GET, 0);

      int failed = 0;
      for (int i = 0; i < 4; ++i) {
        CHECK(dst_array[i] == next);
      }

      if (failed) {
        ERR("Tests of Get APIs FAILED %d test(s).", failed);
      }
    }

    // RMA Put tests
    if (!TEST_SECTION_BEGIN_ENABLED()) {
      // Nothing to do
    } else if (! (test_caps & GEX_EP_CAPABILITY_RMA)) {
      MSG0("%c: Skipping RMA Put tests for pair (%d,%d) - RMA not yet supported for non-primordial EPs",
           TEST_SECTION_NAME(), loc_idx, rem_idx);
    } else {
      MSG0("%c: Starting RMA Put tests for pair (%d,%d)",
           TEST_SECTION_NAME(), loc_idx, rem_idx);

      int rc;
      gex_Event_t ev[2];
      gex_Rank_t *dst = &rem_seg->put_dst[0];
      gex_Rank_t *src = &loc_seg->local[0];
      *src = next;

      ev[0] =       gex_RMA_PutNB(pair, next, dst++, src,  rank_sz, GEX_EVENT_NOW, 0);
      ev[1] =    gex_RMA_PutNBVal(pair, next, dst++, next, rank_sz, 0);
      rc =         gex_RMA_PutNBI(pair, next, dst++, src,  rank_sz, GEX_EVENT_NOW, 0);
      assert(!rc);
      rc =      gex_RMA_PutNBIVal(pair, next, dst++, next, rank_sz, 0);
      assert(!rc);
      rc =    gex_RMA_PutBlocking(pair, next, dst++, src,  rank_sz, 0);
      assert(!rc);
      rc = gex_RMA_PutBlockingVal(pair, next, dst++, next, rank_sz, 0);
      assert(!rc);

      gex_Event_WaitAll(ev, sizeof(ev)/sizeof(ev[0]), 0);
      gex_NBI_Wait(GEX_EC_PUT, 0);

      BARRIER();

      int failed = 0;
      for (int i = 0; i < 6; ++i) {
        CHECK(loc_seg->put_dst[i] == myrank);
        loc_seg->put_dst[i] = myrank; // prevent cascading failure, such as in VIS
      }

      if (failed) {
        ERR("Tests of Put APIs FAILED %d test(s).", failed);
      }
    }

    // TODO: Long tests probably don't use RMA on conduits w/ "packed long"
    if (!TEST_SECTION_BEGIN_ENABLED()) { // AM Tests
      // Nothing to do
    } else if (! (test_caps & GEX_EP_CAPABILITY_AM)) {
      MSG0("%c: Skipping AM Request tests for pair (%d,%d) - AMs not yet supported for non-primordial EPs",
           TEST_SECTION_NAME(), loc_idx, rem_idx);
    } else {
      MSG0("%c: Starting AM Request tests for pair (%d,%d)",
           TEST_SECTION_NAME(), loc_idx, rem_idx);

      static gasnett_atomic_val_t cntr_target = 0;

      // Payload limit queries
      size_t maxmedreq          = gex_AM_MaxRequestMedium(pair,next,GEX_EVENT_NOW,0,0);
      assert_always(maxmedreq  >= gex_AM_LUBRequestMedium());
      size_t maxmedrep          = gex_AM_MaxReplyMedium  (pair,next,GEX_EVENT_NOW,0,0);
      assert_always(maxmedrep  >= gex_AM_LUBReplyMedium());
      size_t maxlongreq         = gex_AM_MaxRequestLong  (pair,next,GEX_EVENT_NOW,0,0);
      assert_always(maxlongreq >= gex_AM_LUBRequestLong());
      size_t maxlongrep         = gex_AM_MaxReplyLong    (pair,next,GEX_EVENT_NOW,0,0);
      assert_always(maxlongrep >= gex_AM_LUBReplyLong());

      // FPAM injection
      gex_Rank_t *fp_dst = &rem_seg->fp_req_dst;
      gex_AM_RequestShort0 (pair, next, hidx_short_ping,                      0);
      gex_AM_RequestMedium0(pair, next, hidx_med_ping,
                            &next, sizeof(gex_Rank_t),         GEX_EVENT_NOW, 0);
      gex_AM_RequestLong0  (pair, next, hidx_long_ping,
                            &next, sizeof(gex_Rank_t), fp_dst, GEX_EVENT_NOW, 0);

      // NPAM injection
      gex_Rank_t *np_dst = &rem_seg->np_req_dst;
      gex_AM_SrcDesc_t sd;
      sd = gex_AM_PrepareRequestMedium(pair, next, &next, rank_sz, rank_sz,       GEX_EVENT_NOW, 1, 0);
      gex_AM_CommitRequestMedium0(sd, hidx_npmed_ping,  rank_sz);
      sd =   gex_AM_PrepareRequestLong(pair, next, &next, rank_sz, rank_sz, NULL, GEX_EVENT_NOW, 0, 0);
      gex_AM_CommitRequestLong0  (sd, hidx_nplong_ping, rank_sz, np_dst);

      cntr_target += 1;
      GASNET_BLOCKUNTIL((cntr_target <= gasnett_atomic_read(&short_ping_cntr,  0)) &&
                        (cntr_target <= gasnett_atomic_read(&short_pong_cntr,  0)) &&
                        (cntr_target <= gasnett_atomic_read(&med_ping_cntr,    0)) &&
                        (cntr_target <= gasnett_atomic_read(&med_pong_cntr,    0)) &&
                        (cntr_target <= gasnett_atomic_read(&long_ping_cntr,   0)) &&
                        (cntr_target <= gasnett_atomic_read(&long_pong_cntr,   0)) &&
                        (cntr_target <= gasnett_atomic_read(&npmed_ping_cntr,  0)) &&
                        (cntr_target <= gasnett_atomic_read(&npmed_pong_cntr,  0)) &&
                        (cntr_target <= gasnett_atomic_read(&nplong_ping_cntr, 0)) &&
                        (cntr_target <= gasnett_atomic_read(&nplong_pong_cntr, 0)));
      // Note the ACQUIRE semantics of BLOCKUNTIL

      int failed = 0;

      CHECK(cntr_target == gasnett_atomic_read(&short_ping_cntr,0));
      CHECK(short_ping_from == prev);

      CHECK(cntr_target == gasnett_atomic_read(&short_pong_cntr,0));
      CHECK(short_pong_from == next);

      CHECK(cntr_target == gasnett_atomic_read(&med_ping_cntr,0));
      CHECK(med_ping_from == prev);

      CHECK(cntr_target == gasnett_atomic_read(&med_pong_cntr,0));
      CHECK(med_pong_from == next);

      CHECK(cntr_target == gasnett_atomic_read(&long_ping_cntr,0));
      CHECK(long_ping_from == prev);
      CHECK(loc_seg->fp_req_dst == myrank);

      CHECK(cntr_target == gasnett_atomic_read(&long_pong_cntr,0));
      CHECK(long_pong_from == next);
      CHECK(loc_seg->fp_rep_dst == next);

      CHECK(cntr_target == gasnett_atomic_read(&npmed_ping_cntr,0));
      CHECK(npmed_ping_from == prev);

      CHECK(cntr_target == gasnett_atomic_read(&npmed_pong_cntr,0));
      CHECK(npmed_pong_from == next);

      CHECK(cntr_target == gasnett_atomic_read(&nplong_ping_cntr,0));
      CHECK(nplong_ping_from == prev);
      CHECK(loc_seg->np_req_dst == myrank);

      CHECK(cntr_target == gasnett_atomic_read(&nplong_pong_cntr,0));
      CHECK(nplong_pong_from == next);
      CHECK(loc_seg->np_rep_dst == next);

      if (failed) {
        ERR("Tests of AM APIs FAILED %d test(s).", failed);
      }
    }

    if (!TEST_SECTION_BEGIN_ENABLED()) { // VIS Indexed Tests
      // Nothing to do
    } else if (! (test_caps & GEX_EP_CAPABILITY_VIS)) {
      MSG0("%c: Skipping VIS Indxed tests for pair (%d,%d) - VIS not yet supported for non-primordial EPs",
           TEST_SECTION_NAME(), loc_idx, rem_idx);
    } else {
      MSG0("%c: Starting VIS Indexed tests for pair (%d,%d)",
           TEST_SECTION_NAME(), loc_idx, rem_idx);

      // gex_VIS_{Vector,Indexed,Strided}{Put,Get}{NB,NBI,Blocking} == 18 entry points
      // TODO: currently we cover only Indexed, which we *hope* is representative

      gex_Rank_t S[2]; S[0] = myrank; S[1] = next;
      gex_Rank_t D[6] = { GEX_RANK_INVALID, GEX_RANK_INVALID, GEX_RANK_INVALID,
                          GEX_RANK_INVALID, GEX_RANK_INVALID, GEX_RANK_INVALID};

      void *loc_src_list[2];
            loc_src_list[0] = S+0;
            loc_src_list[1] = S+1;
      void *rem_src_list[2];
            rem_src_list[0] = &rem_seg->get_src;
            rem_src_list[1] = &rem_seg->vis_src;

      int dst_idx[] = {1,0,2,3,5,4};
      void *loc_dst_list[6];
      void *rem_dst_list[6];
      for (int i = 0; i < 6; ++i) {
        loc_dst_list[i] = dst_idx[i] + D;
        rem_dst_list[i] = dst_idx[i] + rem_seg->vis_dst;
      }

      gex_Event_t puti_ev =
            gex_VIS_IndexedPutNB(pair, next, 2, rem_dst_list+0, rank_sz,
                                             2, loc_src_list,   rank_sz, 0);
           gex_VIS_IndexedPutNBI(pair, next, 2, rem_dst_list+2, rank_sz,
                                             2, loc_src_list,   rank_sz, 0);
      gex_VIS_IndexedPutBlocking(pair, next, 2, rem_dst_list+4, rank_sz,
                                             2, loc_src_list,   rank_sz, 0);


      gex_Event_t geti_ev =
            gex_VIS_IndexedGetNB(pair,       2, loc_dst_list+0, rank_sz,
                                       next, 2, rem_src_list,   rank_sz, 0);
           gex_VIS_IndexedGetNBI(pair,       2, loc_dst_list+2, rank_sz,
                                       next, 2, rem_src_list,   rank_sz, 0);
      gex_VIS_IndexedGetBlocking(pair,       2, loc_dst_list+4, rank_sz,
                                       next, 2, rem_src_list,   rank_sz, 0);

      gex_Event_Wait(puti_ev);
      gex_Event_Wait(geti_ev);
      gex_NBI_Wait(GEX_EC_GET | GEX_EC_PUT, 0);

      BARRIER();

      int failed = 0;

      CHECK(loc_seg->vis_dst[0] == myrank);
      CHECK(loc_seg->vis_dst[1] == prev);
      CHECK(loc_seg->vis_dst[2] == prev);
      CHECK(loc_seg->vis_dst[3] == myrank);
      CHECK(loc_seg->vis_dst[4] == myrank);
      CHECK(loc_seg->vis_dst[5] == prev);
      CHECK(D[0] == myrank);
      CHECK(D[1] == next);
      CHECK(D[2] == next);
      CHECK(D[3] == myrank);
      CHECK(D[4] == myrank);
      CHECK(D[5] == next);

      if (failed) {
        ERR("Tests of VIS APIs FAILED %d test(s).", failed);
      }
    }
  }

  BARRIER();
  MSG("done.");

  gasnet_exit(0);
  return 0;
}
