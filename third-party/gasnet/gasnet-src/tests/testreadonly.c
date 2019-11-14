/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testreadonly.c $
 * Description: GASNet read-only data correctness tests
 * Copyright (c) 2019, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>

#ifndef TEST_SEGSZ
  #define TEST_SEGSZ 4*GASNET_PAGESIZE
#endif
#include <test.h>

// Constant data, long enough not to xfer via an inline send, packed
// long, or similar copy-based mechanism.
//
// NOTE length of this string is limited by C99 only requiring 4095 byte
// strings.  We don't currently attempt to get near that max.  This means that
// a failure mode that delivers zeros beyond the length of this string will go
// undetected.  However, the failure modes seen for read-only data in the past
// don't transfer any data, making the length of the validation less critical.
static const char rodata[TEST_SEGSZ] =
"Fourscore and seven years ago our fathers brought forth, on this "
"continent, a new nation, conceived in liberty, and dedicated to the "
"proposition that all men are created equal.  Now we are engaged in a great "
"civil war, testing whether that nation, or any nation so conceived, and so "
"dedicated, can long endure.  We are met on a great battle-field of that "
"war.  We have come to dedicate a portion of that field, as a final "
"resting-place for those who here gave their lives, that that nation might "
"live.  It is altogether fitting and proper that we should do this.  But, "
"in a larger sense, we cannot dedicate, we cannot consecrate - we cannot "
"hallow - this ground.  The brave men, living and dead, who struggled here, "
"have consecrated it far above our poor power to add or detract.  The world "
"will little note, nor long remember what we say here, but it can never "
"forget what they did here.  It is for us the living, rather, to be "
"dedicated here to the unfinished work which they who fought here have thus "
"far so nobly advanced.  It is rather for us to be here dedicated to the "
"great task remaining before us - that from these honored dead we take "
"increased devotion to that cause for which they here gave the last full "
"measure of devotion - that we here highly resolve that these dead shall "
"not have died in vain - that this nation, under God, shall have a new "
"birth of freedom, and that government of the people, by the people, for "
"the people, shall not perish from the earth.";

static int flag = 0;
void handler(gex_Token_t token, void *buf, size_t len) { flag = 1; }
gex_AM_Entry_t htable[] = {
    { 0, handler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_LONG, 0, NULL, NULL }
};

int main(int argc, char **argv) {
  gex_Client_t    myclient;
  gex_EP_t        myep;
  gex_TM_t        myteam;
  gex_Segment_t   mysegment;

#if GASNET_CONDUIT_IBV
  // This is a hack to avoid triggering Bug 4008, which is believed
  // to be caused by a bug in Mellanox's software stack.
  setenv("GASNET_USE_ODP", "0", 0 /* NO overwrite if already set */);
#endif

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testreadonly", &argc, &argv, 0));
  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, sizeof(htable)/sizeof(gex_AM_Entry_t)));

  test_init("testreadonly",0,"(iters)");

  int iters = 0;
  if (argc > 1) iters = atoi(argv[1]);
  if (!iters) iters = 10;
  if (argc > 2) test_usage();

  gex_Rank_t rank = gex_TM_QueryRank(myteam);
  gex_Rank_t size = gex_TM_QuerySize(myteam);

  MSG0("Running read-only source test with %d iterations", iters);

  gex_Rank_t peer = (rank + 1) % size;

  void *src = (void*)rodata;
  void *dst = TEST_SEG(peer);
  void *loc = TEST_MYSEG();
  size_t max_sz = sizeof(rodata);
  int hidx = htable[0].gex_index;

  size_t max_long = gex_AM_MaxRequestLong(myteam, peer, GEX_EVENT_NOW, 0, 0);

   // TODO: Is there value to testing any of the following:
   //   + Non-blocking Puts
   //   + Long with async LC
   //   + ReplyLong
  for (int i = 0; i < iters; ++i) {
    for (size_t sz = 1; sz <= max_sz; sz *= 2) {
      gex_RMA_PutBlocking(myteam, peer, dst, src, sz, 0);
      gex_Event_Wait(gex_Coll_BarrierNB(myteam,0));
      if (memcmp(rodata, loc, sz)) {
        MSG("ERROR: Bad data from %lld byte gex_RMA_PutBlocking", (long long)sz);
      }
      memset(loc, 0xaa, max_sz);
      gex_Event_Wait(gex_Coll_BarrierNB(myteam,0));

      if (sz <= max_long) {
        gex_AM_RequestLong0(myteam, peer, hidx, src, sz, dst, GEX_EVENT_NOW, 0);
        GASNET_BLOCKUNTIL(flag);
        if (memcmp(rodata, loc, sz)) {
          MSG("ERROR: Bad data from %lld byte gex_AM_RequestLong0", (long long)sz);
        }
        memset(loc, 0x55, max_sz);
        flag = 0;
        gex_Event_Wait(gex_Coll_BarrierNB(myteam,0));
      }
    }
  }

  MSG0("done.");

  gasnet_exit(0);

  return 0; /* Not reached in most implementations */
}
