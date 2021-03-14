/* $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testteam.c $
 * Copyright (c) 2009, The Regents of the University of California
 */

/* Description: basic test of split and barrier using row and col teams */

#include <gasnetex.h>

#ifndef TEST_SEGSZ
#define TEST_SEGSZ (2*1024*1024)
#endif

#include <math.h> /* for sqrt() */
#include <test.h>

static gex_Client_t      myclient;
static gex_EP_t          myep;
static gex_TM_t          myteam;
static gex_Segment_t     mysegment;

int main(int argc, char **argv) 
{
  int iters=0;
  int64_t start,total;
  gex_Rank_t nrows, ncols, my_row, my_col;
  gex_TM_t my_row_tm, my_col_tm;

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testteam", &argc, &argv, 0));

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));

  test_init("testteam", 1, "(iters) (nrows) (ncols)");

  gex_Rank_t mynode = gex_TM_QueryRank(myteam);
  gex_Rank_t nodes = gex_TM_QuerySize(myteam);
  
  if (argc > 4)
    test_usage();

  if (argc > 1) iters = atoi(argv[1]);
  if (!iters) iters = 10000;

  if (argc > 2) {
    nrows = atoi(argv[2]);
  } else {
    /* search for as near to square as possible */
    nrows = sqrt(nodes);
    while (nodes % nrows) --nrows;
  }
  if (argc > 3) {
    ncols = atoi(argv[3]);
  } else {
    ncols = nodes / nrows;
  }
  assert_always(nrows*ncols == nodes);

  MSG0("Running team test with a %u-by-%u grid and %i iterations...\n",
           (int)nrows, (int)ncols, iters);
  BARRIER();
                 
  my_row = mynode / ncols;
  my_col = mynode % ncols;
                 
  struct {
    uint8_t *addr;
    size_t   size;
  } teamA_scratch, teamB_scratch;

  {
    uint8_t  *addr = TEST_MYSEG();
    uintptr_t size = TEST_SEGSZ / 2;

    assert_always(size >= gex_TM_Split(&my_row_tm, myteam, my_row, my_col, 0, 0,
                                       GEX_FLAG_TM_SCRATCH_SIZE_MIN));
    assert_always(size >= gex_TM_Split(&my_col_tm, myteam, my_col, my_row, 0, 0,
                                       GEX_FLAG_TM_SCRATCH_SIZE_MIN));

    teamA_scratch.addr = addr;
    teamA_scratch.size = size;
  
    teamB_scratch.addr = addr + size;
    teamB_scratch.size = size;
  }

  gex_TM_Split(&my_row_tm, myteam, my_row, 1+2*my_col /*gaps*/,
               teamA_scratch.addr, teamA_scratch.size, 0);

  gex_TM_Split(&my_col_tm, myteam, my_col, my_row,
               teamB_scratch.addr, teamB_scratch.size, 0);

  if (my_col == 0) {
    printf("row team %u: Running team barrier test with row teams...\n",
           (int)my_row);
    fflush(stdout);
  }

  BARRIER();
  start = TIME();
  for (int i=0; i < iters; i++) {
    gex_Event_Wait(gex_Coll_BarrierNB(my_row_tm, 0));
  }
  total = TIME() - start;

  if (my_col == 0) {
    printf("row team %u: total time: %8.3f sec, avg row team Barrier latency: %8.3f us\n",
           (int)my_row, ((float)total)/1000000, ((float)total)/iters);
    fflush(stdout);
  }

  if (my_row == 0) {
    printf("col team %u: Running team barrier test with column teams...\n",
           (int)my_col);
    fflush(stdout);
  }

  BARRIER();
  start = TIME();
  for (int i=0; i < iters; i++) {
    gex_Event_Wait(gex_Coll_BarrierNB(my_row_tm, 0));
  }
  total = TIME() - start;
  
  if (my_row == 0) {
    printf("col team %u: total time: %8.3f sec  Avg column team Barrier latency: %8.3f us\n",
           (int)my_col, ((float)total)/1000000, ((float)total)/iters);
    fflush(stdout);
  }
  BARRIER();

  MSG("done.");

  gasnet_exit(0); /* for faster exit */
  return 0;
}
