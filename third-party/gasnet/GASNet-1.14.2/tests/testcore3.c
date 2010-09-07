/*   $Source: /var/local/cvs/gasnet/tests/testcore3.c,v $
 *     $Date: 2010/04/16 22:28:15 $
 * $Revision: 1.3.4.2 $
 * Description: GASNet Active Messages conformance test
 * Copyright (c) 2010, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnet.h>
#include <test.h>

gasnet_node_t mynode = 0;
gasnet_node_t peer = 0;
void *myseg = NULL;
void *peerseg = NULL;

void *addr_tbl[] = {
    (void*)NULL, /* Will become myseg */
    (void*)NULL,
    (void*)(uintptr_t)1,
    (void*)(~(uintptr_t)0)
};
#define NUM_ADDRS (sizeof(addr_tbl)/sizeof(addr_tbl[0]))

#define GASNET_Safe_SrcAddr(fncall) do {                             \
    int _retval;                                                     \
    if ((_retval = fncall) != GASNET_OK) {                           \
      fprintf(stderr, "ERROR calling: %s\n"                          \
                   " with source_addr = %p\n"                        \
                   " at: %s:%i\n"                                    \
                   " error: %s (%s)\n",                              \
              #fncall, source_addr, __FILE__, __LINE__,              \
              gasnet_ErrorName(_retval), gasnet_ErrorDesc(_retval)); \
      fflush(stderr);                                                \
      gasnet_exit(_retval);                                          \
    }                                                                \
  } while(0)


#define hidx_ping_medhandler     201
#define hidx_pong_medhandler     202

#define hidx_ping_longhandler    203
#define hidx_pong_longhandler    204

volatile int flag = 0;

void ping_medhandler(gasnet_token_t token, void *buf, size_t nbytes, gasnet_handlerarg_t addr_idx) {
  void *source_addr = addr_tbl[(int)addr_idx];
  GASNET_Safe_SrcAddr(gasnet_AMReplyMedium0(token, hidx_pong_medhandler, source_addr, 0));
}
void pong_medhandler(gasnet_token_t token, void *buf, size_t nbytes) {
  flag++;
}

void ping_longhandler(gasnet_token_t token, void *buf, size_t nbytes, gasnet_handlerarg_t addr_idx) {
  void *source_addr = addr_tbl[(int)addr_idx];
  GASNET_Safe_SrcAddr(gasnet_AMReplyLong0(token, hidx_pong_longhandler, source_addr, 0, peerseg));
}
void pong_longhandler(gasnet_token_t token, void *buf, size_t nbytes) {
  flag++;
}

/* ------------------------------------------------------------------------------------ */

static void testAMSrcAddr(void);

int main(int argc, char **argv) {
  gasnet_handlerentry_t htable[] = { 
    { hidx_ping_medhandler,    ping_medhandler    },
    { hidx_pong_medhandler,    pong_medhandler    },
    { hidx_ping_longhandler,   ping_longhandler   },
    { hidx_pong_longhandler,   pong_longhandler   }
  };

  GASNET_Safe(gasnet_init(&argc, &argv));
  GASNET_Safe(gasnet_attach(htable, sizeof(htable)/sizeof(gasnet_handlerentry_t),
                            TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));

  test_init("testcore3", 0, "[no argument]");
  if (argc > 1) test_usage();

  TEST_PRINT_CONDUITINFO();

  mynode = gasnet_mynode();
  peer = mynode ^ 1;
  if (peer == gasnet_nodes()) {
    /* w/ odd # of nodes, last one talks to self */
    peer = mynode;
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

      GASNET_Safe_SrcAddr(gasnet_AMRequestMedium1(peer, hidx_ping_medhandler, source_addr, 0, i));
      GASNET_BLOCKUNTIL(flag == goal); ++goal;
      GASNET_Safe_SrcAddr(gasnet_AMRequestLong1(peer, hidx_ping_longhandler, source_addr, 0, peerseg, i));
      GASNET_BLOCKUNTIL(flag == goal); ++goal;
      GASNET_Safe_SrcAddr(gasnet_AMRequestLongAsync1(peer, hidx_ping_longhandler, source_addr, 0, peerseg, i));
      GASNET_BLOCKUNTIL(flag == goal); ++goal;
    }

    BARRIER();
}
