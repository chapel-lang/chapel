/* $Source: /var/local/cvs/gasnet/tests/testteambcast.c,v $
 * $Date: 2010/04/16 22:28:15 $
 * $Revision: 1.2.6.2 $
 * See license.txt for terms
 * Rajesh Nishtala and Yili Zheng
 */

/* Description: basic GASNet team implementation test, team barrier
   test, and team broadcast test.  Column teams and row teams of a process grid are created and
   team barriers are performed on these teams. */

#include <gasnet.h>
#include <gasnet_coll.h>
#include <gasnet_coll_team.h>

#define SCRATCH_SIZE (4*1024*1024)
#define COLL_BUFF_SIZE (16*1024*1024)
#define SEG_PER_THREAD (SCRATCH_SIZE+2*COLL_BUFF_SIZE)
#define TEST_SEGSZ_EXPR (SEG_PER_THREAD)

#define MAX_SIZE 16384
#include <test.h>

int main(int argc, char **argv) 
{
  int mynode, nodes, iters=0;
  int64_t start,total;
  int i = 0;
  gasnet_node_t nrows, ncols, my_row, my_col;
  void *clientdata = NULL;
  gasnet_team_handle_t my_row_team, my_col_team;
  static uint8_t *A, *B, *C;
  static int *B_int, *C_int;
  int num_iters;
  size_t sz;
  
  gasnet_seginfo_t teamA_scratch;
  gasnet_seginfo_t teamB_scratch;
  gasnet_seginfo_t const * test_segs;
  GASNET_Safe(gasnet_init(&argc, &argv));
#if !GASNET_SEQ
  MSG0("WARNING: This test does not work for NON-SEQ builds yet.. skipping test\n");
  gasnet_exit(0);
#endif

  GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
  
  A = TEST_MYSEG();
  B = TEST_MYSEG() + SCRATCH_SIZE;
  C = B + SCRATCH_SIZE;
  B_int = (int*) B;
  C_int = (int*) C;
  gasnet_coll_init(NULL, 0, NULL, 0, 0);

  test_init("testteambcast", 1, "(nrows) (ncols) (iters)");

  mynode = gasnet_mynode();
  nodes = gasnet_nodes();
  test_segs = TEST_SEGINFO();
  
  teamA_scratch.addr = test_segs[mynode].addr;
  teamA_scratch.size = SCRATCH_SIZE/2;
  
  teamB_scratch.addr = (uint8_t*)teamA_scratch.addr + SCRATCH_SIZE/2;
  teamB_scratch.size = teamA_scratch.size;

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
                 
  my_row_team = gasnet_coll_team_split(GASNET_TEAM_ALL,
                                        my_row,
                                        my_col,
                                        &teamA_scratch);

  my_col_team = gasnet_coll_team_split(GASNET_TEAM_ALL,
                                        my_col,
                                        my_row,
                                        &teamB_scratch);

  if (my_col == 0) {
    printf("row team %u: Running team barrier test with row teams...\n",
           my_row);
    fflush(stdout);
  }

  BARRIER();
  start = TIME();
  for (i=0; i < iters*10; i++) {
    gasnete_coll_teambarrier_notify(my_row_team);            
    gasnete_coll_teambarrier_wait(my_row_team); 
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
  for (i=0; i < iters*10; i++) {
    gasnete_coll_teambarrier_notify(my_col_team);            
    gasnete_coll_teambarrier_wait(my_col_team); 
  }
  total = TIME() - start;
  
  if (my_row == 0) {
    printf("col team %u: total time: %8.3f sec  Avg column team Barrier latency: %8.3f us\n",
           my_col, ((float)total)/1000000, ((float)total)/(iters*10));
    fflush(stdout);
  }
  BARRIER();
  
  /*first do team all broadcast*/
  for (sz = 1; sz<MAX_SIZE; sz=sz*2) {
    int root = 0;
    for(i=0; i<sz; i++) {
      B_int[i] = mynode*sz+42+i;
      C_int[i] = -1;
    }
    BARRIER();
    gasnet_coll_broadcast(GASNET_TEAM_ALL, C_int, root, B_int, sz*sizeof(int), 
                          GASNET_COLL_IN_ALLSYNC|GASNET_COLL_OUT_NOSYNC|GASNET_COLL_LOCAL);
    BARRIER();
    for(i=0; i<sz; i++) {
      int expected = root*sz+42+i;
      if(expected != C_int[i]) {
        fprintf(stderr, "%d> %d %d (expecting %d)\n", mynode, i, C_int[i], expected);
        gasnet_exit(1);
      }
    }
    BARRIER();
    /*time this*/
    start = TIME();
    for(i=0; i<iters; i++) {
      gasnet_coll_broadcast(GASNET_TEAM_ALL, C_int, root, B_int, sz*sizeof(int), 
                            GASNET_COLL_IN_ALLSYNC|GASNET_COLL_OUT_NOSYNC|GASNET_COLL_LOCAL);
    }
    total = TIME() - start;
    
    if(mynode == 0){
      printf("%d> %d byte broadcast team all time: %8.3f usec\n",mynode,sz*sizeof(int),
           mynode,  ((float)total)/(iters));
      fflush(stdout);
    }
  }
  BARRIER();

  /*next do row broadcasts*/
  for (sz = 1; sz<MAX_SIZE; sz=sz*2) {
    for(i=0; i<sz; i++) {
      B_int[i] = mynode*sz+42+i;
      C_int[i] = -1;
    }
    BARRIER();
    gasnet_coll_broadcast(my_row_team, C_int, 0, B_int, sz*sizeof(int), 
                          GASNET_COLL_IN_ALLSYNC|GASNET_COLL_OUT_NOSYNC|GASNET_COLL_LOCAL);
    BARRIER();
    for(i=0; i<sz; i++) {
      int expected = my_row*ncols*sz+42+i;
      if(expected != C_int[i]) {
        fprintf(stderr, "%d> %d %d (expecting %d)\n", mynode, i, C_int[i], expected);
        gasnet_exit(1);
      }
    }
    BARRIER();
    /*time this*/
    start = TIME();
    for(i=0; i<iters; i++) {
      gasnet_coll_broadcast(my_row_team, C_int, 0, B_int, sz*sizeof(int), 
                            GASNET_COLL_IN_ALLSYNC|GASNET_COLL_OUT_NOSYNC|GASNET_COLL_LOCAL);
    }
    total = TIME() - start;
    
    if(my_col == 0){
      printf("%d> %d byte broadcast row team %u time: %8.3f usec\n",mynode,sz*sizeof(int),
           my_row,  ((float)total)/(iters));
      fflush(stdout);
    }
  }
  BARRIER();
  /*next do col broadcasts*/
  for (sz = 1; sz<MAX_SIZE; sz=sz*2) {
    for(i=0; i<sz; i++) {
      B_int[i] = mynode*sz+42+i;
      C_int[i] = -1;
    }
    BARRIER();
    gasnet_coll_broadcast(my_col_team, C_int, 0, B_int, sz*sizeof(int), 
                          GASNET_COLL_IN_ALLSYNC|GASNET_COLL_OUT_NOSYNC|GASNET_COLL_LOCAL);
    BARRIER();
    for(i=0; i<sz; i++) {
      int expected = my_col*sz+42+i;
      if(expected != C_int[i]) {
        fprintf(stderr, "%d> %d %d (expecting %d)\n", mynode, i, C_int[i], expected);
        gasnet_exit(1);
      }
    }
    BARRIER();
    /*time this*/
    start = TIME();
    for(i=0; i<iters; i++) {
      gasnet_coll_broadcast(my_col_team, C_int, 0, B_int, sz*sizeof(int), 
                            GASNET_COLL_IN_ALLSYNC|GASNET_COLL_OUT_NOSYNC|GASNET_COLL_LOCAL);
    }
    total = TIME() - start;
    
    if(my_row == 0){
      printf("%d> %d byte broadcast col team %u time: %8.3f usec\n",mynode,sz*sizeof(int),
           my_col,  ((float)total)/(iters));
      fflush(stdout);
    }
  }
    
  
  BARRIER();
  
  MSG("done.");

  gasnet_exit(0); /* for faster exit */
  return 0;
}
