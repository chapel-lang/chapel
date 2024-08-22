/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testprogthr.c $
 * Description: GASNet progress threads tests
 * Copyright 2024, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

// WARNING: This test exercises an EXPERIMENTAL feature.

// WARNING: This test may use internal knowledge of the current
// implementation to perform open-box testing.
// Such uses are marked and should not be cloned into client code.

#include <gasnetex.h>
#include <gasnet_tools.h>

#ifndef TEST_SEGSZ
  #define TEST_SEGSZ PAGESZ
#endif
#include <test.h>

#ifdef HAVE_PTHREAD_H
  #include <pthread.h>
#endif

static gex_Client_t    myclient;
static gex_EP_t        myep;
static gex_TM_t        myteam;
static gex_Segment_t   mysegment;

static gex_Rank_t      myrank;
static gex_Rank_t      nranks;

static gex_Rank_t      next_rank;
static void*           next_seg;

#define hidx_ping_handler     GEX_AM_INDEX_BASE+0
#define hidx_pong_handler     GEX_AM_INDEX_BASE+1

volatile int flag = 0;

void ping_handler(gex_Token_t token) {
  gex_AM_ReplyShort0(token, hidx_pong_handler, 0);
}
void pong_handler(gex_Token_t token) {
  flag++;
}

static void do_comms(void) {
  // Simple AM Short ping-pong
  { int prev = flag;
    gex_AM_RequestShort0(myteam, next_rank, hidx_ping_handler, 0);
    GASNET_BLOCKUNTIL(flag == prev+1);
  }
  // Back-to-back Put and Get
  { int snd = myrank + 100;
    int rcv = -1;
    gex_RMA_PutBlocking(myteam, next_rank, next_seg, &snd, sizeof(int), 0);
    gex_RMA_GetBlocking(myteam, &rcv, next_rank, next_seg, sizeof(int), 0);
    assert_always(rcv == snd);
  }
}

int main(int argc, char **argv) {

  // Opt-in via environment vaiables only if the current conduit
  // has been built with the corresponding support (may avoid warnings).
#if GASNET_RCV_THREAD
  gasnett_setenv("GASNET_RCV_THREAD", "YES");
#endif
#if GASNET_SND_THREAD
  gasnett_setenv("GASNET_SND_THREAD", "YES");
#endif

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testprogthr", &argc, &argv, GEX_FLAG_DEFER_THREADS));

  gex_AM_Entry_t htable[] = {
    { hidx_ping_handler,  ping_handler,  GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 0 },
    { hidx_pong_handler,  pong_handler,  GEX_FLAG_AM_REPLY  |GEX_FLAG_AM_SHORT, 0 },
  };

  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, sizeof(htable)/sizeof(gex_AM_Entry_t)));

  test_init("testprogthr",0,"");

  myrank = gex_System_QueryJobRank();
  nranks = gex_System_QueryJobSize();
  next_rank = (myrank + 1) % nranks;

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  next_seg = TEST_SEG(next_rank);
  gex_Event_Wait(gex_Coll_BarrierNB(myteam,0));

  // Perform some communication
  for (int i = 0; i <100; ++i) do_comms();
  BARRIER();

  unsigned int count;
  const gex_ProgressThreadInfo_t *info;
  GASNET_Safe( gex_System_QueryProgressThreads(myclient, &count, &info, 0) );
  MSG("Conduit has %u progress thread(s)", count);

#ifdef HAVE_PTHREAD_H
  int have_rcv_thread = 0;
  // Spec says order doesn't matter.  So we'll create threads in reverse order.
  for (int i = (count-1); i >= 0; i--) {
    assert_always( info[i].gex_progress_fn );
    assert_always( info[i].gex_device_list );

    // NOTE: The following are tests of current implementation behaviors, using
    //       internal knowledge which is subject to change at any time.
    //       Therefore, it is unsafe to clone anything from these open-box tests.
    // BEGIN open-box testing
    assert_always( info[i].gex_progress_arg );
    assert_always( strlen(info[i].gex_device_list) );
    // END open-box testing

  #if !GASNET_RCV_THREAD
    if (info[i].gex_thread_roles & GEX_THREAD_ROLE_RCV) {
      ERR("Unexpected GEX_THREAD_ROLE_RCV");
    }
  #endif
  #if !GASNET_SND_THREAD
    if (info[i].gex_thread_roles & GEX_THREAD_ROLE_SND) {
      ERR("Unexpected GEX_THREAD_ROLE_SND");
    }
  #endif

    have_rcv_thread |= (info[i].gex_thread_roles & GEX_THREAD_ROLE_RCV);
    const char *roles_str;
    switch (info[i].gex_thread_roles) {
      case 0:
        roles_str = "NONE";
        ERR("Unexpected gex_thread_roles == 0");
        break;
      case GEX_THREAD_ROLE_RCV:
        roles_str = "RCV";
        break;
      case GEX_THREAD_ROLE_SND:
        roles_str = "SND";
        break;
      case (GEX_THREAD_ROLE_RCV|GEX_THREAD_ROLE_SND):
        roles_str = "RCV+SND";
        break;
      default:
        roles_str = "UNKNOWN ROLE(S)";
        break;
    }
    pthread_t tid;
    check_zeroret( pthread_create(&tid, NULL, info[i].gex_progress_fn, info[i].gex_progress_arg) );
    MSG("Created tid %p for %s on device(s) %s",
        (void*)(uintptr_t)tid, roles_str, info[i].gex_device_list);
    sleep(1); // an attempt to yield
  }

  if (have_rcv_thread) assert_always(GASNET_HIDDEN_AM_CONCURRENCY_LEVEL);
  assert_always(have_rcv_thread == !!gex_System_QueryHiddenAMConcurrencyLevel());

  // Perform some communication
  for (int i = 0; i <100; ++i) do_comms();
#else
  if (count) {
    MSG0("WARNING: Thread spawning skipped due to lack of pthreads support.");
  }
#endif

  BARRIER();
  MSG0("done.");

  gasnet_exit(!! test_errs);

  return 0;
}
