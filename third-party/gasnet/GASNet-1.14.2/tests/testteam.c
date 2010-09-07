/* $Source: /var/local/cvs/gasnet/tests/testteam.c,v $
 * $Date: 2010/04/16 22:28:15 $
 * $Revision: 1.2.4.1 $
 * LBNL 2009
 */

/* Description: basic GASNet team implementation test and team barrier
   test.  Column teams and row teams of a process grid are created and
   team barriers are performed on these teams. */

#include <gasnet.h>
#include <gasnet_coll.h>
#include <gasnet_coll_team.h>

#define SEG_PER_THREAD (2*1024*1024)
#define TEST_SEGSZ_EXPR (SEG_PER_THREAD)

#include <math.h> /* for sqrt() */
#include <test.h>

int main(int argc, char **argv) 
{
  int mynode, nodes, iters=0;
  int64_t start,total;
  int i = 0;
  gasnet_node_t nrows, ncols, my_row, my_col;
  void *clientdata = NULL;
  gasnet_team_handle_t my_row_team, my_col_team;
  static uint8_t *A, *B;

  
  
  gasnet_seginfo_t teamA_scratch;
  gasnet_seginfo_t teamB_scratch;
  gasnet_seginfo_t const * test_segs;
  GASNET_Safe(gasnet_init(&argc, &argv));

  GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
  
#if !GASNET_SEQ
  MSG0("WARNING: This test does not work for NON-SEQ builds yet.. skipping test\n");
  gasnet_exit(0);
#endif

  A = TEST_MYSEG();
  
  gasnet_coll_init(NULL, 0, NULL, 0, 0);

  test_init("test_team", 1, "(iters) (nrows) (ncols)");

  mynode = gasnet_mynode();
  nodes = gasnet_nodes();
  test_segs = TEST_SEGINFO();
  
  teamA_scratch.addr = test_segs[mynode].addr;
  teamA_scratch.size = test_segs[mynode].size/2;
  
  teamB_scratch.addr = (uint8_t*)teamA_scratch.addr + teamA_scratch.size;
  teamB_scratch.size = teamA_scratch.size;

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
           nrows, ncols, iters);
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
  for (i=0; i < iters; i++) {
    gasnete_coll_teambarrier_notify(my_row_team);            
    gasnete_coll_teambarrier_wait(my_row_team); 
  }
  total = TIME() - start;

  if (my_col == 0) {
    printf("row team %u: total time: %8.3f sec, avg row team Barrier latency: %8.3f us\n",
           my_row, ((float)total)/1000000, ((float)total)/iters);
    fflush(stdout);
  }

  if (my_row == 0) {
    printf("col team %u: Running team barrier test with column teams...\n",
           my_col);
    fflush(stdout);
  }

  BARRIER();
  start = TIME();
  for (i=0; i < iters; i++) {
    gasnete_coll_teambarrier_notify(my_col_team);            
    gasnete_coll_teambarrier_wait(my_col_team); 
  }
  total = TIME() - start;
  
  if (my_row == 0) {
    printf("col team %u: total time: %8.3f sec  Avg column team Barrier latency: %8.3f us\n",
           my_col, ((float)total)/1000000, ((float)total)/iters);
    fflush(stdout);
  }
  BARRIER();

  MSG("done.");

  gasnet_exit(0); /* for faster exit */
  return 0;
}
