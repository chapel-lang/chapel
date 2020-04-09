/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testcore3.c $
 * Description: GASNet Active Messages conformance test
 * Copyright (c) 2010, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>
#include <test.h>

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

gex_Rank_t mynode = 0;
gex_Rank_t peer = 0;
void *myseg = NULL;
void *peerseg = NULL;

void *addr_tbl[] = {
    (void*)NULL, /* Will become myseg */
    (void*)NULL,
    (void*)(uintptr_t)1,
    (void*)(~(uintptr_t)0)
};
#define NUM_ADDRS (sizeof(addr_tbl)/sizeof(addr_tbl[0]))


#define hidx_ping_medhandler     201
#define hidx_pong_medhandler     202

#define hidx_ping_longhandler    203
#define hidx_pong_longhandler    204

volatile int flag = 0;

void ping_medhandler(gex_Token_t token, void *buf, size_t nbytes, gex_AM_Arg_t addr_idx) {
  void *source_addr = addr_tbl[(int)addr_idx];
  gex_AM_ReplyMedium0(token, hidx_pong_medhandler, source_addr, 0, GEX_EVENT_NOW, 0);
}
void pong_medhandler(gex_Token_t token, void *buf, size_t nbytes) {
  flag++;
}

void ping_longhandler(gex_Token_t token, void *buf, size_t nbytes, gex_AM_Arg_t addr_idx) {
  void *source_addr = addr_tbl[(int)addr_idx];
  gex_AM_ReplyLong0(token, hidx_pong_longhandler, source_addr, 0, peerseg, GEX_EVENT_NOW, 0);
}
void pong_longhandler(gex_Token_t token, void *buf, size_t nbytes) {
  flag++;
}

/* ------------------------------------------------------------------------------------ */

static void testAMSrcAddr(void);

int main(int argc, char **argv) {
  gex_AM_Entry_t htable[] = { 
    { hidx_ping_medhandler,  ping_medhandler,  GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDIUM, 1 },
    { hidx_pong_medhandler,  pong_medhandler,  GEX_FLAG_AM_REPLY|GEX_FLAG_AM_MEDIUM, 0 },
    { hidx_ping_longhandler, ping_longhandler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_LONG, 1 },
    { hidx_pong_longhandler, pong_longhandler, GEX_FLAG_AM_REPLY|GEX_FLAG_AM_LONG, 0 }
  };

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testcore3", &argc, &argv, 0));
  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, sizeof(htable)/sizeof(gex_AM_Entry_t)));

  test_init("testcore3", 0, "[no argument]");
  if (argc > 1) test_usage();

  TEST_PRINT_CONDUITINFO();

  mynode = gex_TM_QueryRank(myteam);
  gex_Rank_t nnodes = gex_TM_QuerySize(myteam);
  peer = mynode + 1;
  if (peer == nnodes) {
    // w/ odd # of nodes, last one talks to self, else to 0
    peer = (nnodes%2) ? mynode : 0;
  }

  addr_tbl[0] = myseg = TEST_MYSEG();
  peerseg = TEST_SEG(peer);

  BARRIER();

  testAMSrcAddr();

  MSG("done.");

  gasnet_exit(0);
  return 0;
}

/* ------------------------------------------------------------------------------------ */
void testAMSrcAddr(void) {
    GASNET_BEGIN_FUNCTION();
    int i;

    flag = 0;

    /* GASNet spec says we ignore source_addr when nbytes==0.
     * In this case the spec leaves the value of "buf" passed the the handler undefined.
     * So, we pass the index, i, to allow the Reply to test all the values as well.
     */
    for (i = 0; i < NUM_ADDRS; ++i) {
      void *source_addr = addr_tbl[i];
      int goal = flag + 1;

      gex_AM_RequestMedium1(myteam, peer, hidx_ping_medhandler, source_addr, 0, GEX_EVENT_NOW, 0, i);
      GASNET_BLOCKUNTIL(flag == goal); ++goal;
      gex_AM_RequestLong1(myteam, peer, hidx_ping_longhandler, source_addr, 0, peerseg, GEX_EVENT_NOW, 0, i);
      GASNET_BLOCKUNTIL(flag == goal); ++goal;
    }

    BARRIER();
}
