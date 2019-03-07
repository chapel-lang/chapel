/* $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testsplit.c $
 * Copyright (c) 2018, The Regents of the University of California
 */

#include <gasnetex.h>

#define SCRATCH_SIZE (2*1024*1024)

#ifndef TEST_SEGSZ
#define TEST_SEGSZ (PAGESZ + 5*SCRATCH_SIZE) // 5 teams's scratch + page for comms
#endif

#include <math.h> /* for sqrt() */
#include <test.h>

#ifndef SCRATCH_QUERY_FLAG
#define SCRATCH_QUERY_FLAG GEX_FLAG_TM_SCRATCH_SIZE_RECOMMENDED
//#define SCRATCH_QUERY_FLAG GEX_FLAG_TM_SCRATCH_SIZE_MIN
#endif

static gex_Client_t  myclient;
static gex_EP_t      myep;
static gex_TM_t      myteam;
static gex_Segment_t mysegment;
static gex_Rank_t    myrank;

// handler indices
#define hidx_pong_shorthandler       200
#define hidx_rank_shorthandler       201
#define hidx_rank_medlonghandler     202

// handler functions
static gasnett_atomic_t am_cntr = gasnett_atomic_init(0);
void pong_shorthandler(gex_Token_t token, gex_AM_Arg_t arg0) {
  gex_Token_Info_t info;
  gex_TI_t rc = gex_Token_Info(token, &info, GEX_TI_SRCRANK);
  assert_always((gex_Rank_t)arg0 == info.gex_srcrank);
  gasnett_atomic_increment(&am_cntr, 0);
}
void am_validate(gex_Token_t token, gex_AM_Arg_t arg0, gex_AM_Arg_t arg1) {
  assert_always((gex_Rank_t)arg0 == myrank);
  gex_Token_Info_t info;
  gex_TI_t rc = gex_Token_Info(token, &info, GEX_TI_SRCRANK);
  assert_always((gex_Rank_t)arg1 == info.gex_srcrank);
  gex_AM_ReplyShort1(token, hidx_pong_shorthandler, 0, (gex_AM_Arg_t)myrank);
}
void rank_shorthandler(gex_Token_t token, gex_AM_Arg_t arg0, gex_AM_Arg_t arg1) {
  am_validate(token, arg0, arg1);
}
void rank_medlonghandler(gex_Token_t token, void *buf, size_t nbytes,
                         gex_AM_Arg_t arg0, gex_AM_Arg_t arg1) {
  am_validate(token, arg0, arg1);
}

// handler table
gex_AM_Entry_t htable[] = {
  { hidx_pong_shorthandler,   pong_shorthandler,   GEX_FLAG_AM_REPLY  |GEX_FLAG_AM_SHORT,   1 },
  { hidx_rank_shorthandler,   rank_shorthandler,   GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT,   2 },
  { hidx_rank_medlonghandler, rank_medlonghandler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDLONG, 2 }
 };
#define HANDLER_TABLE_SIZE (sizeof(htable)/sizeof(gex_AM_Entry_t))

int main(int argc, char **argv)
{
  gex_Rank_t peer;

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testsplit", &argc, &argv, 0));

  test_init("testsplit", 1, "(nrows) (ncols)");

  myrank = gex_TM_QueryRank(myteam);
  gex_Rank_t nranks = gex_TM_QuerySize(myteam);

  gex_Rank_t nrows;
  if (argc > 1) {
    nrows = atoi(argv[1]);
  } else {
    /* search for as near to square as possible */
    nrows = sqrt(nranks);
    while (nranks % nrows) --nrows;
  }

  gex_Rank_t ncols;
  if (argc > 2) {
    ncols = atoi(argv[2]);
  } else {
    ncols = nranks / nrows;
  }
  assert_always(nrows*ncols == nranks);

  gex_Rank_t myrow = myrank / ncols;
  gex_Rank_t mycol = myrank % ncols;

  MSG0("Running split test with a %u-by-%u grid.", (int)nrows, (int)ncols);

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  BARRIER();

  // Will reserve all but first page of segment for scratch space
  uintptr_t scratch_addr = PAGESZ + (uintptr_t)TEST_MYSEG();
  uintptr_t scratch_end = TEST_SEGSZ + (uintptr_t)TEST_MYSEG();
  size_t scratch_sz;

  // Spec says NULL new_tm_p returns zero.
  scratch_sz = gex_TM_Split(NULL, myteam, 0, 1, 0, 0, GEX_FLAG_TM_SCRATCH_SIZE_MIN);
  assert_always(scratch_sz == 0);
  scratch_sz = gex_TM_Split(NULL, myteam, 0, 1, 0, 0, GEX_FLAG_TM_SCRATCH_SIZE_RECOMMENDED);
  assert_always(scratch_sz == 0);

  // Row team:
  gex_TM_t rowtm = myteam; // init just to check whether overwritten
  scratch_sz = gex_TM_Split(&rowtm, myteam, myrow, 1+2*mycol, 0, 0, SCRATCH_QUERY_FLAG);
  assert_always((scratch_addr + scratch_sz) <= scratch_end);
  gex_TM_Split(&rowtm, myteam, myrow, 1+2*mycol, (void*)scratch_addr, scratch_sz, 0);
  scratch_addr += scratch_sz;
  assert_always(rowtm != myteam);
  assert_always(gex_TM_QueryRank(rowtm) == mycol);
  assert_always(gex_TM_QuerySize(rowtm) == ncols);
  for (gex_Rank_t rank = 0; rank < ncols; ++rank) {
    gex_Rank_t jobrank = myrow*ncols + rank;
    assert_always(gex_TM_TranslateRankToJobrank(rowtm, rank) == jobrank);
    assert_always(gex_TM_TranslateJobrankToRank(rowtm, jobrank) == rank);
  }

  // Column team:
  gex_TM_t coltm = myteam; // init just to check whether overwritten
  scratch_sz = gex_TM_Split(&coltm, myteam, mycol, myrow, 0, 0, SCRATCH_QUERY_FLAG);
  assert_always((scratch_addr + scratch_sz) <= scratch_end);
  gex_TM_Split(&coltm, myteam, mycol, myrow, (void*)scratch_addr, scratch_sz, 0);
  scratch_addr += scratch_sz;
  assert_always(coltm != myteam);
  assert_always(gex_TM_QueryRank(coltm) == myrow);
  assert_always(gex_TM_QuerySize(coltm) == nrows);
  for (gex_Rank_t rank = 0; rank < nrows; ++rank) {
    gex_Rank_t jobrank = mycol + ncols*rank;
    assert_always(gex_TM_TranslateRankToJobrank(coltm, rank) == jobrank);
    assert_always(gex_TM_TranslateJobrankToRank(coltm, jobrank) == rank);
  }

  // Singleton team (also tests a 2nd-level split, of coltm):
  gex_TM_t onetm = coltm; // init just to check whether overwritten
  scratch_sz = gex_TM_Split(&onetm, coltm, myrank, 0, 0, 0, SCRATCH_QUERY_FLAG);
  assert_always((scratch_addr + scratch_sz) <= scratch_end);
  gex_TM_Split(&onetm, coltm, myrank, 0, (void*)scratch_addr, scratch_sz, 0);
  scratch_addr += scratch_sz;
  assert_always(onetm != coltm);
  assert_always(gex_TM_QueryRank(onetm) == 0);
  assert_always(gex_TM_QuerySize(onetm) == 1);
  assert_always(gex_TM_TranslateRankToJobrank(onetm, 0) == myrank);
  assert_always(gex_TM_TranslateJobrankToRank(onetm, myrank) == 0);

  // Odds only team (exercise new_tmp_p = NULL case):
  gex_TM_t oddtm = rowtm; // init just to check whether overwritten
  int odd = myrank & 1;
  gex_TM_t *new_tm_p = odd ? &oddtm : NULL;
  scratch_sz = gex_TM_Split(new_tm_p, rowtm, 0, 0, 0, 0, SCRATCH_QUERY_FLAG);
  assert_always((scratch_addr + scratch_sz) <= scratch_end);
  gex_TM_Split(new_tm_p, rowtm, 0, 0, (void*)scratch_addr, scratch_sz, 0);
  scratch_addr += scratch_sz;
  if (odd) {
    assert_always(oddtm != rowtm);
    gex_Rank_t size = gex_TM_QuerySize(oddtm);
    assert_always(size <= gex_TM_QuerySize(rowtm));
    // Check that tie-breaks on key==0 respect order in parent team.
    // Taking a short-cut here knowning parent (rowtm) is in jobrank order and contiguous.
    gex_Rank_t first = gex_TM_TranslateRankToJobrank(oddtm, 0);
    for (gex_Rank_t rank = 1; rank < size; ++rank) {
      gex_Rank_t jobrank = gex_TM_TranslateRankToJobrank(oddtm, rank);
      assert_always(jobrank == first + 2*rank);
    }
    // Check gex_TM_TranslateJobrankToRank() for a guaranteed non-member
    assert_always(GEX_RANK_INVALID == gex_TM_TranslateJobrankToRank(oddtm,0));
  } else {
    assert_always(oddtm == rowtm); // Should be unchanged
  }

  // "Rev" team reversing order of TM0
  gex_TM_t revtm = myteam; // init just to check whether overwritten
  const int alpha = 7, beta = 4;
  int mykey = alpha + beta * (nranks - (myrank + 1));
  scratch_sz = gex_TM_Split(&revtm, myteam, 0xf00, mykey, 0, 0, SCRATCH_QUERY_FLAG);
  assert_always((scratch_addr + scratch_sz) <= scratch_end);
  gex_TM_Split(&revtm, myteam, 0xf00, mykey, (void*)scratch_addr, scratch_sz, 0);
  scratch_addr += scratch_sz;
  assert_always(revtm != myteam);
  assert_always(gex_TM_QuerySize(revtm) == nranks);
  assert_always(gex_TM_QueryRank(revtm) == (nranks - (myrank + 1)));

  //
  // Some basic validation by communicating w/i the new teams
  //

  gex_Rank_t rank_var, *rank_ptr;
  gex_Rank_t *rank_arr = TEST_MYSEG();
  rank_arr[0] = myrank;
  rank_arr[1] = GEX_RANK_INVALID;
  rank_arr[2] = GEX_RANK_INVALID;
  BARRIER();

  // Try loopback Gets N ways:
  rank_ptr = TEST_MYSEG();
  rank_var = gex_RMA_GetBlockingVal(myteam, myrank, rank_ptr, sizeof(gex_Rank_t), 0);
  assert_always(rank_var == myrank);
  rank_var = GEX_RANK_INVALID;
  gex_RMA_GetBlocking(onetm, &rank_var, 0, rank_ptr, sizeof(gex_Rank_t), 0);
  assert_always(rank_var == myrank);
  rank_var = GEX_RANK_INVALID;
  gex_Event_Wait(gex_RMA_GetNB(rowtm, &rank_var, mycol, rank_ptr, sizeof(gex_Rank_t), 0));
  assert_always(rank_var == myrank);
  rank_var = GEX_RANK_INVALID;
  gex_RMA_GetNBI(coltm, &rank_var, myrow, rank_ptr, sizeof(gex_Rank_t), 0);
  gex_NBI_Wait(GEX_EC_GET, 0);
  assert_always(rank_var == myrank);
  BARRIER();

  // Blocking loop-back Put on singleton team
  peer = 0;
  rank_ptr = (gex_Rank_t *)TEST_SEG_TM(onetm, peer);
  assert_always(rank_ptr == rank_arr);
  rank_var = myrank ^ 42;
  gex_RMA_PutBlocking(onetm, 0, rank_ptr, &rank_var, sizeof(rank_var), 0);
  assert_always(rank_arr[0] == rank_var);

  // PutNB on row ring
  peer = (mycol+1)%ncols; // mycol and ncols are position in, and length of, the row
  rank_ptr = (gex_Rank_t *)TEST_SEG_TM(rowtm, peer);
  gex_Event_Wait(gex_RMA_PutNB(rowtm, peer, rank_ptr+1, &myrank, sizeof(myrank), GEX_EVENT_NOW, 0));
  gex_Event_Wait(gex_Coll_BarrierNB(rowtm, 0));
  assert_always(rank_arr[1] == gex_TM_TranslateRankToJobrank(rowtm, (mycol+ncols-1)%ncols));

  // PutNBI on col ring
  peer = (myrow+1)%nrows; // myrow and nrows are position in, and length of, the col
  rank_ptr = (gex_Rank_t *)TEST_SEG_TM(coltm, peer);
  gex_RMA_PutNBI(coltm, peer, rank_ptr+2, &myrank, sizeof(myrank), GEX_EVENT_NOW, 0);
  gex_NBI_Wait(GEX_EC_GET, 0);
  gex_Event_Wait(gex_Coll_BarrierNB(coltm, 0));
  assert_always(rank_arr[2] == gex_TM_TranslateRankToJobrank(coltm, (myrow+nrows-1)%nrows));

  // AM tests
  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, sizeof(htable)/sizeof(gex_AM_Entry_t)));
  BARRIER();
  peer = (mycol+1)%ncols;
  gex_AM_RequestShort2(rowtm, peer, hidx_rank_shorthandler, 0,
                       (gex_AM_Arg_t)gex_TM_TranslateRankToJobrank(rowtm, peer),
                       (gex_AM_Arg_t)myrank);
  peer = (myrow+1)%nrows;
  gex_AM_RequestMedium2(coltm, peer, hidx_rank_medlonghandler,
                        NULL, 0, GEX_EVENT_NOW, 0,
                        (gex_AM_Arg_t)gex_TM_TranslateRankToJobrank(coltm, peer),
                        (gex_AM_Arg_t)myrank);
  gex_AM_RequestLong2(onetm, 0, hidx_rank_medlonghandler,
                      NULL, 0, TEST_MYSEG(), GEX_EVENT_NOW, 0,
                      (gex_AM_Arg_t)myrank, (gex_AM_Arg_t)myrank);
  GASNET_BLOCKUNTIL(gasnett_atomic_read(&am_cntr,0) == 3);
  BARRIER();

  MSG("done.");

  gasnet_exit(0); /* for faster exit */
  return 0;
}
