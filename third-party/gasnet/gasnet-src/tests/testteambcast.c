/* $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testteambcast.c $
 * See license.txt for terms
 * Rajesh Nishtala and Yili Zheng
 */

/* Description: basic GASNet team implementation test, team barrier
   test, and team broadcast test.  Column teams and row teams of a process grid are created and
   team barriers are performed on these teams. */

#include <gasnetex.h>
#include <gasnet_coll.h>

#define MAX_SIZE 8192
#define SCRATCH_SIZE (4*1024*1024)
#define COLL_BUFF_SIZE (SIZEOF_INT * MAX_SIZE)
#define TEST_SEGSZ (SCRATCH_SIZE + 2*COLL_BUFF_SIZE)

#include <test.h>

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

int main(int argc, char **argv) 
{
  int iters=0;
  int64_t start,total;
  gex_Rank_t nrows, ncols, my_row, my_col;
  gex_TM_t my_row_tm, my_col_tm;
  
  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testteambcast", &argc, &argv, 0));

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  
  uint8_t *A = TEST_MYSEG();
  uint8_t *B = A + SCRATCH_SIZE;
  uint8_t *C = B + COLL_BUFF_SIZE;
  int *SrcArray = (int*) B;
  int *DstArray = (int*) C;

  test_init("testteambcast", 1, "(nrows) (ncols) (iters)");

  gex_Rank_t mynode = gex_TM_QueryRank(myteam);
  gex_Rank_t nodes = gex_TM_QuerySize(myteam);
  
  if (argc > 4)
    test_usage();

  if (argc > 1) {
    nrows = atoi(argv[1]);
  } else {
    nrows = 1 + !(nodes & 1); /* 1 if odd, 2 if even */
  }
  if (argc > 2) {
    ncols = atoi(argv[2]);
  } else {
    ncols = nodes / nrows;
  }
  assert_always(nrows*ncols == nodes);

  if (argc > 3) iters = atoi(argv[3]);
  if (!iters) iters = 20;

  if (mynode == 0) {
    printf("Running team test with a %u-by-%u grid and %i iterations...\n",
           nrows, ncols, iters);
    fflush(stdout);
  }
  BARRIER();
                 
  my_row = mynode / ncols;
  my_col = mynode % ncols;
                 
  struct {
    uint8_t *addr;
    size_t   size;
  } teamA_scratch, teamB_scratch;

  {
    uint8_t  *addr = A;
    uintptr_t size = SCRATCH_SIZE / 2;

    assert_always(size >= gex_TM_Split(&my_row_tm, myteam, my_row, my_col, 0, 0,
                                       GEX_FLAG_TM_SCRATCH_SIZE_MIN));
    assert_always(size >= gex_TM_Split(&my_col_tm, myteam, my_col, my_row, 0, 0,
                                       GEX_FLAG_TM_SCRATCH_SIZE_MIN));

    teamA_scratch.addr = addr;
    teamA_scratch.size = size;

    teamB_scratch.addr = addr + size;
    teamB_scratch.size = size;
  }

  
  gex_TM_Split(&my_row_tm, myteam, my_row, my_col,
               teamA_scratch.addr, teamA_scratch.size, 0);

  gex_TM_Split(&my_col_tm, myteam, my_col, my_row,
               teamB_scratch.addr, teamB_scratch.size, 0);

  if (my_col == 0) {
    printf("row team %u: Running team barrier test with row teams...\n",
           my_row);
    fflush(stdout);
  }

  BARRIER();
  start = TIME();
  for (int i=0; i < iters*10; i++) {
    gex_Event_Wait(gex_Coll_BarrierNB(my_row_tm, 0));
  }
  total = TIME() - start;

  if (my_col == 0) {
    printf("row team %u: total time: %8.3f sec, avg row team Barrier latency: %8.3f us\n",
           my_row, ((float)total)/1000000, ((float)total)/(iters*10));
    fflush(stdout);
  }

  if (my_row == 0) {
    printf("col team %u: Running team barrier test with column teams...\n",
           my_col);
    fflush(stdout);
  }

  BARRIER();
  start = TIME();
  for (int i=0; i < iters*10; i++) {
    gex_Event_Wait(gex_Coll_BarrierNB(my_col_tm, 0));
  }
  total = TIME() - start;
  
  if (my_row == 0) {
    printf("col team %u: total time: %8.3f sec  Avg column team Barrier latency: %8.3f us\n",
           my_col, ((float)total)/1000000, ((float)total)/(iters*10));
    fflush(stdout);
  }
  BARRIER();
  
  /*first do team0 broadcast*/
  for (size_t sz = 1; sz<=MAX_SIZE; sz=sz*2) {
    int root = 0;
    for (int i=0; i<sz; i++) {
      SrcArray[i] = mynode*sz+42+i;
      DstArray[i] = -1;
    }
    BARRIER();
    gex_Event_Wait(gex_Coll_BroadcastNB(myteam, root, DstArray, SrcArray, sz*sizeof(int), 0));
    BARRIER();
    for (int i=0; i<sz; i++) {
      int expected = root*sz+42+i;
      if (expected != DstArray[i]) {
        fprintf(stderr, "%d> %d %d (expecting %d)\n", mynode, i, DstArray[i], expected);
        gasnet_exit(1);
      }
    }
    BARRIER();
    /*time this*/
    start = TIME();
    for (int i=0; i<iters; i++) {
      gex_Event_Wait(gex_Coll_BroadcastNB(myteam, root, DstArray, SrcArray, sz*sizeof(int), 0));
    }
    total = TIME() - start;
    
    if(mynode == 0){
      printf("%d> %"PRIuPTR" byte broadcast team all time: %8.3f usec\n",mynode,(uintptr_t)sz*sizeof(int),
           ((float)total)/(iters));
      fflush(stdout);
    }
  }
  BARRIER();

  /*next do row broadcasts*/
  for (size_t sz = 1; sz<=MAX_SIZE; sz=sz*2) {
    for (int i=0; i<sz; i++) {
      SrcArray[i] = mynode*sz+42+i;
      DstArray[i] = -1;
    }
    BARRIER();
    gex_Event_Wait(gex_Coll_BroadcastNB(my_row_tm, 0, DstArray, SrcArray, sz*sizeof(int), 0));
    BARRIER();
    for (int i=0; i<sz; i++) {
      int expected = my_row*ncols*sz+42+i;
      if (expected != DstArray[i]) {
        fprintf(stderr, "%d> %d %d (expecting %d)\n", mynode, i, DstArray[i], expected);
        gasnet_exit(1);
      }
    }
    BARRIER();
    /*time this*/
    start = TIME();
    for (int i=0; i<iters; i++) {
      gex_Event_Wait(gex_Coll_BroadcastNB(my_row_tm, 0, DstArray, SrcArray, sz*sizeof(int), 0));
    }
    total = TIME() - start;
    
    if(my_col == 0){
      printf("%d> %"PRIuPTR" byte broadcast row team %u time: %8.3f usec\n",mynode,(uintptr_t)sz*sizeof(int),
           my_row,  ((float)total)/(iters));
      fflush(stdout);
    }
  }
  BARRIER();
  /*next do col broadcasts*/
  for (size_t sz = 1; sz<=MAX_SIZE; sz=sz*2) {
    for (int i=0; i<sz; i++) {
      SrcArray[i] = mynode*sz+42+i;
      DstArray[i] = -1;
    }
    BARRIER();
    gex_Event_Wait(gex_Coll_BroadcastNB(my_col_tm, 0, DstArray, SrcArray, sz*sizeof(int), 0));
    BARRIER();
    for (int i=0; i<sz; i++) {
      int expected = my_col*sz+42+i;
      if (expected != DstArray[i]) {
        fprintf(stderr, "%d> %d %d (expecting %d)\n", mynode, i, DstArray[i], expected);
        gasnet_exit(1);
      }
    }
    BARRIER();
    /*time this*/
    start = TIME();
    for (int i=0; i<iters; i++) {
      gex_Event_Wait(gex_Coll_BroadcastNB(my_col_tm, 0, DstArray, SrcArray, sz*sizeof(int), 0));
    }
    total = TIME() - start;
    
    if(my_row == 0){
      printf("%d> %"PRIuPTR" byte broadcast col team %u time: %8.3f usec\n",mynode,(uintptr_t)sz*sizeof(int),
           my_col,  ((float)total)/(iters));
      fflush(stdout);
    }
  }
    
  
  BARRIER();
  
  MSG("done.");

  gasnet_exit(0); /* for faster exit */
  return 0;
}
