/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testhsl.c $
 * Description: GASNet HSL correctness test
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>

#include <test.h>

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

int peer = -1;
int flag = 0;
uint64_t iters = 100;
gex_HSL_t globallock = GEX_HSL_INITIALIZER;

void okhandler3(gex_Token_t token) {
  gex_HSL_Lock(&globallock);
  flag++;
  gex_HSL_Unlock(&globallock);
}


void badhandler1(gex_Token_t token) {
  gex_HSL_Lock(&globallock);
}
void badhandler2(gex_Token_t token) {
  gex_HSL_Lock(&globallock);
  gex_AM_ReplyShort0(token, 250, 0);
}

uint64_t counter = 0;
void increq(gex_Token_t token) {
  gex_HSL_Lock(&globallock);
  counter++;
  gex_HSL_Unlock(&globallock);
  gex_AM_ReplyShort0(token, 222, 0);
}
gex_HSL_t replock = GEX_HSL_INITIALIZER;
uint64_t repcounter = 0;
void increp(gex_Token_t token) {
  gex_HSL_Lock(&replock);
  repcounter++;
  gex_HSL_Unlock(&replock);
}


void donothing(gex_Token_t token) {
}

#if GASNET_PAR
  int NUM_THREADS = 4;
  void * thread_fn(void *arg);
#endif

int main(int argc, char **argv) {
  int mynode, nodes;
  gex_AM_Entry_t htable[] = { 
    { 203, okhandler3,  GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 0 },

    { 221, increq,      GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 0 },
    { 222, increp,      GEX_FLAG_AM_REPLY|GEX_FLAG_AM_SHORT, 0 },

    { 231, badhandler1, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 0 },
    { 232, badhandler2, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 0 },

    { 250, donothing,   GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 0 }
  };

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testhsl", &argc, &argv, 0));
  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, sizeof(htable)/sizeof(gex_AM_Entry_t)));
  test_init("testhsl",0,"(0|errtestnum:1..16)");

  mynode = gex_TM_QueryRank(myteam);
  nodes = gex_TM_QuerySize(myteam);
  peer = (mynode + 1) % nodes;

  if (argc < 2) test_usage();
  {
    int errtest = atoi(argv[1]);
    gex_HSL_t lock1 = GEX_HSL_INITIALIZER;
    gex_HSL_t lock2;
    gex_HSL_Init(&lock2);

    MSG0("testing legal local cases...");
    gex_HSL_Lock(&lock1);
    gex_HSL_Lock(&lock2);
    assert(mynode == gex_TM_QueryRank(myteam));
    assert(nodes == gex_TM_QuerySize(myteam));
    gex_HSL_Unlock(&lock2);
    gex_HSL_Unlock(&lock1);

    assert_always(gex_HSL_Trylock(&lock1) == GASNET_OK);
    gex_HSL_Unlock(&lock1);

    BARRIER();
    MSG0("testing legal AM cases...");

    gex_AM_RequestShort0(myteam, peer, 203, 0);
    GASNET_BLOCKUNTIL(flag == 1);

    BARRIER();

   if (errtest) {
    int dummy = 0;
    MSG0("testing illegal case %i...", errtest);
    switch(errtest) {
      case 1:
        gex_HSL_Init(&lock1);
      break;
      case 2:
        gex_HSL_Destroy(&lock1);
        gex_HSL_Destroy(&lock1);
      break;
      case 3:
        gex_HSL_Unlock(&lock1);
      break;
      case 4:
        gex_HSL_Lock(&lock1);
        gex_HSL_Lock(&lock2);
        gex_HSL_Unlock(&lock1);
      break;
      case 5:
        gex_HSL_Lock(&lock1);
        gex_HSL_Lock(&lock1);
      break;
      case 6:
        dummy += gex_HSL_Trylock(&lock1);
        dummy += gex_HSL_Trylock(&lock1);
      break;
      case 7:
        gex_HSL_Lock(&lock1);
        gasnet_AMPoll();
      break;
      case 8:
        gex_AM_RequestShort0(myteam, mynode, 231, 0);
        GASNET_BLOCKUNTIL(0);
      break;
      case 9:
        gex_AM_RequestShort0(myteam, mynode, 232, 0);
        GASNET_BLOCKUNTIL(0);
      break;
      case 10:
        gex_HSL_Lock(&lock1);
        gex_AM_RequestShort0(myteam, mynode, 250, 0);
        gex_HSL_Unlock(&lock1);
      break;
      case 11:
        gex_HSL_Lock(&lock1);
        sleep(2);
        gex_HSL_Unlock(&lock1);
        goto done;
      break;
      case 12:
        dummy += gex_HSL_Trylock(&lock1);
        sleep(2);
        gex_HSL_Unlock(&lock1);
        goto done;
      break;
      default:
        ERR("bad err test num.");
        test_usage();
    }
    FATALERR("FAILED: err test failed.");
   } else {
  #if GASNET_PAR
    MSG0("Spawning pthreads...");
    NUM_THREADS = test_thread_limit(NUM_THREADS);
    test_createandjoin_pthreads(NUM_THREADS, &thread_fn, NULL, 0);
  #endif
   }
  }

done:
  BARRIER();

  MSG0("done.");

  BARRIER();
  gasnet_exit(0);
  return 0;
}

#if GASNET_PAR

#undef MSG0
#undef ERR
#define MSG0 THREAD_MSG0(id)
#define ERR  THREAD_ERR(id)

void * thread_fn(void *arg) {
  int id = (int)(uintptr_t)arg;
  uint64_t iters2 = iters*100;
  uint64_t i;

  counter = 0; repcounter = 0;
  PTHREAD_BARRIER(NUM_THREADS);

  MSG0("hsl exclusion test, local-only...");
    for (i=0;i<iters2;i++) {
      if (i&1) {
        gex_HSL_Lock(&globallock);
      } else {
        int retval;
        while ((retval=gex_HSL_Trylock(&globallock)) != GASNET_OK) {
          assert_always(retval == GASNET_ERR_NOT_READY);
        }
      }
      counter++;
      gex_HSL_Unlock(&globallock);
    }

    PTHREAD_LOCALBARRIER(NUM_THREADS);

    if (counter != (NUM_THREADS * iters2)) 
      ERR("failed hsl test: counter=%"PRIu64" expecting=%"PRIu64, 
          counter, (NUM_THREADS * iters2));

  PTHREAD_BARRIER(NUM_THREADS);
  counter = 0; repcounter = 0;
  PTHREAD_BARRIER(NUM_THREADS);

  MSG0("hsl exclusion test, AM-only...");
    for (i=0;i<iters;i++) {
      gex_AM_RequestShort0(myteam, peer, 221, 0);
    }
    GASNET_BLOCKUNTIL(repcounter == NUM_THREADS * iters);
    PTHREAD_BARRIER(NUM_THREADS);

    if (counter != (NUM_THREADS * iters)) 
      ERR("failed hsl test: counter=%"PRIu64" expecting=%"PRIu64, 
          counter, (NUM_THREADS * iters));

  PTHREAD_BARRIER(NUM_THREADS);
  counter = 0; repcounter = 0;
  PTHREAD_BARRIER(NUM_THREADS);

  MSG0("hsl exclusion test, AM & local...");
    for (i=0;i<iters;i++) {
      gex_AM_RequestShort0(myteam, peer, 221, 0);
      if (i&1) {
        gex_HSL_Lock(&globallock);
      } else {
        int retval;
        while ((retval=gex_HSL_Trylock(&globallock)) != GASNET_OK) {
          assert_always(retval == GASNET_ERR_NOT_READY);
        }
      }
      counter++;
      gex_HSL_Unlock(&globallock);
    }
    GASNET_BLOCKUNTIL(repcounter == NUM_THREADS * iters);
    PTHREAD_BARRIER(NUM_THREADS);

    if (counter != (2 * NUM_THREADS * iters)) 
      ERR("failed hsl test: counter=%"PRIu64" expecting=%"PRIu64, 
          counter, (2 * NUM_THREADS * iters));

  PTHREAD_BARRIER(NUM_THREADS);
  counter = 0; repcounter = 0;
  PTHREAD_BARRIER(NUM_THREADS);

  return NULL;
}

#endif
