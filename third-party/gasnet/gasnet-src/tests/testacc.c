/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testacc.c $
 * Description: GASNet memory model conformance test using a lock-protected accumulator
 * Copyright (c) 2019, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>
#include <gasnet_coll.h>
#include <gasnet_tools.h>

#include <test.h>

// Note that local 'want_val' and global 'data' accumulators have the same
// (unsigned) type and are kept synchronized such that overflow is harmless.
typedef uint32_t data_t;

int main(int argc, char **argv) {
  gex_Client_t      myclient;
  gex_EP_t          myep;
  gex_TM_t          myteam;
  gex_Segment_t     mysegment;

  int failed = 0;
  int iters = 0;

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testacc", &argc, &argv, 0));

  if (argc > 1) iters = atoi(argv[1]);
  if (iters <= 0) iters = 100000;

  gex_Rank_t myrank = gex_TM_QueryRank(myteam);
  gex_Rank_t nranks = gex_TM_QuerySize(myteam);

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ));

  test_init("testacc",0,"(iters)");
  MSG0("Running testacc with %d iterations", iters);

  TEST_SET_WAITMODE(1);

  /* Two shared vars, 'lock' and 'data', are on first and last process.
     Both sit at base of their segment (unless that would overlap). */

  gex_Rank_t lock_rank = 0;
  gex_Rank_t *lock_addr = (gex_Rank_t *) TEST_SEG(lock_rank);

  gex_Rank_t data_rank = nranks - 1;
  data_t *data_addr = (data_t *) TEST_SEG(data_rank) + (lock_rank == data_rank);

  /* initialize both vars locally: */
  if (lock_rank == myrank) *lock_addr = 0;
  if (data_rank == myrank) *data_addr = 0;

  /* now the fun begins... */
  gex_Event_Wait(gex_Coll_BarrierNB(myteam,0));
  {
    const gex_Rank_t next_rank = (myrank + 1) % nranks;
    const data_t incr = nranks * (nranks + 1) / 2;
    data_t want_val = myrank * (myrank + 1) / 2;

    for (int i = 0; i < iters; ++i, want_val += incr) {
      /* Acquire the lock: */
      while (myrank != gex_RMA_GetBlockingVal(myteam, lock_rank, lock_addr, sizeof(gex_Rank_t), 0)) {
        test_yield_if_polite();
        gasnet_AMPoll();
      }
      gasnett_local_rmb();

      /* Read and Write the accumulator: */
      data_t curr_val = gex_RMA_GetBlockingVal(myteam, data_rank, data_addr, sizeof(data_t), 0);
      data_t next_val = want_val + (myrank + 1);
      gex_RMA_PutBlockingVal(myteam, data_rank, data_addr, next_val, sizeof(data_t), 0);

      /* Pass lock ownership to the next process: */
      gasnett_local_wmb();
      gex_RMA_PutBlockingVal(myteam, lock_rank, lock_addr, next_rank, sizeof(gex_Rank_t), 0);

      /* Check value read from accumulator (outside of critical section): */
      if (curr_val != want_val) {
        const int msg_limit = 10;
        if (failed < msg_limit) {
          MSG("*** ERROR - FAILED at %d: difference = %ld\n", i, (long)(want_val-curr_val));
        } else if (failed == msg_limit) {
          MSG("*** NOTICE - suppressing errors beyond per-rank limit of %d", msg_limit);
        }
        ++failed;
      }

      TEST_PROGRESS_BAR(i, iters);
    }
  }
  gex_Event_Wait(gex_Coll_BarrierNB(myteam,0));

  int32_t errs = test_errs;
  gex_Event_Wait(
    gex_Coll_ReduceToAllNB(myteam, &errs, &errs,
                           GEX_DT_I32, sizeof(errs), 1,
                           GEX_OP_ADD, NULL, NULL, 0));

  if (errs) {
    MSG0("FAIL - detected %i errors", (int)errs);
  } else {
    MSG0("done.");
  }

  gasnet_exit(errs != 0);
  return 0; /* Not reached */
}
