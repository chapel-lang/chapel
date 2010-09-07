/*   $Source: /var/local/cvs/gasnet/tests/testcore4.c,v $
 *     $Date: 2010/04/26 15:48:47 $
 * $Revision: 1.6.4.3 $
 * Description: GASNet Active Messages conformance/correctness test
 * Copyright (c) 2010, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnet.h>

uintptr_t maxsz = 0;
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR (2*(uintptr_t)maxsz)
#endif
#include <test.h>

#if PLATFORM_ARCH_32
  #define MAX_ARGS 8
#else
  #define MAX_ARGS  16
#endif

gasnet_handlerarg_t rand_args[MAX_ARGS];
#define RAND_ARG(idx) (rand_args[(idx)-1])

uint8_t *rand_payload;
size_t medsz, longsz;
int iters = 10;

gasnet_node_t mynode = 0;
gasnet_node_t peer = 0;
uint8_t *myseg = NULL;
uint8_t *peerseg = NULL;

#define hidx_mybase 200

#define hidx_ping_shorthandler   (hidx_mybase + 1)
#define hidx_pong_shorthandler   (hidx_mybase + 2)

#define hidx_ping_medhandler     (hidx_mybase + 3)
#define hidx_pong_medhandler     (hidx_mybase + 4)

#define hidx_ping_longhandler    (hidx_mybase + 5)
#define hidx_pong_longhandler    (hidx_mybase + 6)

/* For > 0 args we use first arg to distinguish request/reply */
#define hidx_Shandler(args)      (hidx_mybase + 3*args + 4)
#define hidx_Mhandler(args)      (hidx_mybase + 3*args + 5)
#define hidx_Lhandler(args)      (hidx_mybase + 3*args + 6)

/* Preprocess-time iterator with distinct base case */
#define HITER1(base,macro)     base
#define HITER2(base,macro)     HITER1(base,macro)  macro(2)
#define HITER3(base,macro)     HITER2(base,macro)  macro(3)
#define HITER4(base,macro)     HITER3(base,macro)  macro(4)
#define HITER5(base,macro)     HITER4(base,macro)  macro(5)
#define HITER6(base,macro)     HITER5(base,macro)  macro(6)
#define HITER7(base,macro)     HITER6(base,macro)  macro(7)
#define HITER8(base,macro)     HITER7(base,macro)  macro(8)
#define HITER9(base,macro)     HITER8(base,macro)  macro(9)
#define HITER10(base,macro)    HITER9(base,macro)  macro(10)
#define HITER11(base,macro)    HITER10(base,macro) macro(11)
#define HITER12(base,macro)    HITER11(base,macro) macro(12)
#define HITER13(base,macro)    HITER12(base,macro) macro(13)
#define HITER14(base,macro)    HITER13(base,macro) macro(14)
#define HITER15(base,macro)    HITER14(base,macro) macro(15)
#define HITER16(base,macro)    HITER15(base,macro) macro(16)

#define HARG_(val)  , RAND_ARG(val)
#define HARGS(args) HITER##args(arg1+RAND_ARG(1),HARG_)

#define HARGPROTO_(val) , gasnet_handlerarg_t arg##val
#define HARGPROTO(args) HITER##args(gasnet_handlerarg_t arg1,HARGPROTO_)

/* Simpler iterator over required arg counts */
#if PLATFORM_ARCH_32
  #define HFOREACH(macro) \
    macro(1)  macro(2)  macro(3)  macro(4) \
    macro(5)  macro(6)  macro(7)  macro(8)
#else
  #define HFOREACH(macro) \
    macro(1)  macro(2)  macro(3)  macro(4) \
    macro(5)  macro(6)  macro(7)  macro(8) \
    macro(9)  macro(10) macro(11) macro(12)\
    macro(13) macro(14) macro(15) macro(16)
#endif

#define SARGS(dest,args) (dest, hidx_Shandler(args), HARGS(args))
#define MARGS(dest,args) (dest, hidx_Mhandler(args), rand_payload, medsz, HARGS(args))
#define LARGS(dest,args,isRep) (dest, hidx_Lhandler(args), rand_payload, \
                                longsz, peerseg + isRep*longsz, HARGS(args))

/* NOTE: This extra step appears needed for pgcc (bug 2796) */
#define DO_CALL(fn,args) GASNET_Safe(fn args)

enum {
    op_done,
    op_srep,
    op_mrep,
    op_lrep
};

#define HCHECK(val) ; assert_always(arg##val == RAND_ARG(val))
#define HBODY(args) do {                                           \
    gasnet_handlerarg_t operation = arg1 - RAND_ARG(1);            \
    gasnet_node_t srcid;                                           \
    gasnet_AMGetMsgSource(token, &srcid);                          \
    assert_always(srcid == peer);                                  \
    HITER##args((void)0,HCHECK);                                   \
    arg1 = op_done;                                                \
    switch(operation) {                                            \
      case op_done:                                                \
        flag++;                                                    \
        break;                                                     \
      case op_srep:                                                \
        DO_CALL(gasnet_AMReplyShort##args, SARGS(token,args));     \
        break;                                                     \
      case op_mrep:                                                \
        DO_CALL(gasnet_AMReplyMedium##args, MARGS(token,args));    \
        break;                                                     \
      case op_lrep:                                                \
        DO_CALL(gasnet_AMReplyLong##args, LARGS(token,args,1));    \
        break;                                                     \
      default:                                                     \
        FATALERR("Invalid operation = %d", operation);             \
    }                                                              \
  } while(0);
#define HDEFN(args) \
    void Shandler##args(gasnet_token_t token, HARGPROTO(args)) \
        { HBODY(args); } \
    void Mhandler##args(gasnet_token_t token, void *buf, size_t nbytes, HARGPROTO(args))\
        { MSGCHECK(medsz); HBODY(args); } \
    void Lhandler##args(gasnet_token_t token, void *buf, size_t nbytes, HARGPROTO(args))\
        { MSGCHECK(longsz); memset(buf, 0xa5, nbytes); HBODY(args); }

#define HTABLE(args)                          \
  { hidx_Shandler(args), Shandler##args },    \
  { hidx_Mhandler(args), Mhandler##args },    \
  { hidx_Lhandler(args), Lhandler##args },

#define HTEST(args) \
  MSG0("testing %d-argument AM calls", args);                        \
  for (i = 0; i < iters; ++i) {                                      \
    gasnet_handlerarg_t arg1;                                        \
    int goal = flag + 1;                                             \
    randomize();                                                     \
    arg1 = op_srep;                                                  \
      DO_CALL(gasnet_AMRequestShort##args,SARGS(peer,args));         \
      GASNET_BLOCKUNTIL(flag == goal); ++goal;                       \
    arg1 = op_mrep;                                                  \
      DO_CALL(gasnet_AMRequestMedium##args,MARGS(peer,args));        \
      GASNET_BLOCKUNTIL(flag == goal); ++goal;                       \
    arg1 = op_lrep;                                                  \
      DO_CALL(gasnet_AMRequestLong##args,LARGS(peer,args,0));        \
      GASNET_BLOCKUNTIL(flag == goal); ++goal;                       \
      DO_CALL(gasnet_AMRequestLongAsync##args,LARGS(peer,args,0));   \
      GASNET_BLOCKUNTIL(flag == goal); ++goal;                       \
  }

#define MSGCHECK(size) do {                                \
    assert_always(nbytes == size);                         \
    if (memcmp(buf, rand_payload, nbytes))                 \
      FATALERR("Payload verification failed in %s",        \
               GASNETT_CURRENT_FUNCTION);                  \
  } while(0)

/* Define all the handlers */
volatile int flag = 0;
HFOREACH(HDEFN)
void ping_shorthandler(gasnet_token_t token) {
    GASNET_Safe(gasnet_AMReplyShort0(token, hidx_pong_shorthandler));
}
void pong_shorthandler(gasnet_token_t token) {
	  flag++;
}
void ping_medhandler(gasnet_token_t token, void *buf, size_t nbytes) {
    MSGCHECK(medsz);
    GASNET_Safe(gasnet_AMReplyMedium0(token, hidx_pong_medhandler, rand_payload, nbytes));
}
void pong_medhandler(gasnet_token_t token, void *buf, size_t nbytes) {
    MSGCHECK(medsz);
    flag++;
}
void ping_longhandler(gasnet_token_t token, void *buf, size_t nbytes) {
    MSGCHECK(longsz);
    GASNET_Safe(gasnet_AMReplyLong0(token, hidx_pong_longhandler, rand_payload, nbytes, peerseg));
}
void pong_longhandler(gasnet_token_t token, void *buf, size_t nbytes) {
    MSGCHECK(longsz);
    flag++;
}

static void randomize(void) {
  int i;
  BARRIER();
  for (i=0; i<MAX_ARGS; ++i) {
     rand_args[i] = TEST_RAND(0,RAND_MAX-1);
  }
  for (i=0; i<maxsz; ++i) {
     rand_payload[i] = TEST_RAND(0,255);
  }
  BARRIER();
}

int main(int argc, char **argv) {
  unsigned int seed;
  int i;
  gasnet_handlerentry_t htable[] = { 
    HFOREACH(HTABLE)
    { hidx_ping_shorthandler,  ping_shorthandler  },
    { hidx_pong_shorthandler,  pong_shorthandler  },
    { hidx_ping_medhandler,    ping_medhandler    },
    { hidx_pong_medhandler,    pong_medhandler    },
    { hidx_ping_longhandler,   ping_longhandler   },
    { hidx_pong_longhandler,   pong_longhandler   }
  };

  GASNET_Safe(gasnet_init(&argc, &argv));

  if (argc > 1) iters = atoi(argv[1]);
  if (iters <= 0) iters = 10;

  if (argc > 2) maxsz = atoi(argv[2]);
  if (maxsz <= 0) maxsz = 2*1024*1024;
  medsz = gasnet_AMMaxMedium();
  medsz = MIN(maxsz, medsz);
  longsz = MIN(gasnet_AMMaxLongRequest(), gasnet_AMMaxLongReply());
  longsz = MIN(maxsz, longsz);
  maxsz = MAX(medsz,longsz);

  if (argc > 3) seed = atoi(argv[3]);
  if (!seed) seed = (int)TIME();

  GASNET_Safe(gasnet_attach(htable, sizeof(htable)/sizeof(gasnet_handlerentry_t),
                            TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));

  test_init("testcore4", 0, "(iters) (maxsz) (seed)");
  if (argc > 4) test_usage();

  TEST_BCAST(&seed, 0, &seed, sizeof(seed));
  TEST_SRAND(seed);

  TEST_PRINT_CONDUITINFO();

  mynode = gasnet_mynode();
  peer = mynode ^ 1;
  if (peer == gasnet_nodes()) {
    /* w/ odd # of nodes, last one talks to self */
    peer = mynode;
  }

  myseg = TEST_MYSEG();
  peerseg = TEST_SEG(peer);
  rand_payload = test_malloc(maxsz);


  MSG0("Running %i iterations of AM interface tests with medsz=%ld longsz=%ld seed=%u",
       iters, (long)medsz, (long)longsz, seed);

  MSG0("testing 0-argument AM calls");
  for (i = 0; i < iters; ++i) {
    int goal = flag + 1;
    randomize();

    GASNET_Safe(gasnet_AMRequestShort0(peer, hidx_ping_shorthandler));
    GASNET_BLOCKUNTIL(flag == goal); ++goal;

    GASNET_Safe(gasnet_AMRequestMedium0(peer, hidx_ping_medhandler, rand_payload, medsz));
    GASNET_BLOCKUNTIL(flag == goal); ++goal;

    GASNET_Safe(gasnet_AMRequestLong0(peer, hidx_ping_longhandler, rand_payload, longsz, peerseg));
    GASNET_BLOCKUNTIL(flag == goal); ++goal;
 
    GASNET_Safe(gasnet_AMRequestLongAsync0(peer, hidx_ping_longhandler, rand_payload, longsz, peerseg));
    GASNET_BLOCKUNTIL(flag == goal); ++goal;
  }

  /* Now 1 ... 8 or 16 */
  HFOREACH(HTEST)
  BARRIER();

  MSG("done.");

  gasnet_exit(0);
  return 0;
}

/* ------------------------------------------------------------------------------------ */
