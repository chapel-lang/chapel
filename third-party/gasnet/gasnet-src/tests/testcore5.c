/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testcore5.c $
 * Description: GASNet Active Messages var args conformance/correctness test
 * Copyright (c) 2013, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>
#include <gasnet_coll.h>

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

size_t maxsz = 0;
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR (2*(uintptr_t)maxsz)
#endif
#include <test.h>

#if PLATFORM_ARCH_32
  #define MAX_ARGS 8
#else
  #define MAX_ARGS  16
#endif

gex_AM_Arg_t rand_args[MAX_ARGS];
#define RAND_ARG(idx) (rand_args[(idx)-1])

uint8_t *rand_payload;
int iters = 10;

gex_Rank_t mynode = 0;
gex_Rank_t peer = 0;
gex_Rank_t from = 0;
uint8_t *myseg = NULL;
uint8_t *peerseg = NULL;
uint8_t *fromseg = NULL;

#define hidx_mybase 150

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

#define HARGPROTO_(val) , gex_AM_Arg_t arg##val
#define HARGPROTO(args) HITER##args(gex_AM_Arg_t arg1,HARGPROTO_)

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

#define MSZ(args) \
        MIN(maxsz,MIN(gex_AM_MaxRequestMedium(myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,args), \
                      gex_AM_MaxReplyMedium  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,args)))
#define LSZ(args) \
        MIN(maxsz,MIN(gex_AM_MaxRequestLong(myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,args), \
                      gex_AM_MaxReplyLong  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,args)))

#define DEST(n) myteam, (n)
#define SARGS(dest,args) (dest, hidx_Shandler(args), 0, HARGS(args))
#define MARGS(dest,args) (dest, hidx_Mhandler(args), rand_payload, MSZ(args), \
                          GEX_EVENT_NOW, 0, HARGS(args))
#define LARGS(dest,args,isRep) (dest, hidx_Lhandler(args), rand_payload, \
                                LSZ(args), (isRep?fromseg:peerseg) + isRep*LSZ(args), \
                                GEX_EVENT_NOW, 0, HARGS(args))

/* NOTE: This extra step appears needed for pgcc (bug 2796) */
#define DO_CALL(fn,args) (fn args)

enum {
    op_done,
    op_srep,
    op_mrep,
    op_lrep
};

#define HCHECK(val) ; assert_always(arg##val == RAND_ARG(val))
#define HBODY(args) do {                                           \
    gex_AM_Arg_t operation = arg1 - RAND_ARG(1);          \
    gex_Rank_t srcid = test_msgsource(token);                      \
    assert_always(srcid == (operation == op_done ? peer : from));  \
    HITER##args((void)0,HCHECK);                                   \
    arg1 = op_done;                                                \
    switch(operation) {                                            \
      case op_done:                                                \
        flag++;                                                    \
        break;                                                     \
      case op_srep:                                                \
        DO_CALL(gex_AM_ReplyShort, SARGS(token,args));     \
        break;                                                     \
      case op_mrep:                                                \
        DO_CALL(gex_AM_ReplyMedium, MARGS(token,args));    \
        break;                                                     \
      case op_lrep:                                                \
        DO_CALL(gex_AM_ReplyLong, LARGS(token,args,1));    \
        break;                                                     \
      default:                                                     \
        FATALERR("Invalid operation = %d", (int)operation);        \
    }                                                              \
  } while(0);
#define HDEFN(args) \
    void Shandler##args(gex_Token_t token, HARGPROTO(args)) \
        { HBODY(args); } \
    void Mhandler##args(gex_Token_t token, void *buf, size_t nbytes, HARGPROTO(args))\
        { MSGCHECK(MSZ(args)); HBODY(args); } \
    void Lhandler##args(gex_Token_t token, void *buf, size_t nbytes, HARGPROTO(args))\
        { MSGCHECK(LSZ(args)); memset(buf, 0xa5, nbytes); HBODY(args); }

#define HTABLE(args)                          \
  { hidx_Shandler(args), Shandler##args, GEX_FLAG_AM_REQREP|GEX_FLAG_AM_SHORT, args },    \
  { hidx_Mhandler(args), Mhandler##args, GEX_FLAG_AM_REQREP|GEX_FLAG_AM_MEDIUM, args },    \
  { hidx_Lhandler(args), Lhandler##args, GEX_FLAG_AM_REQREP|GEX_FLAG_AM_LONG, args },

#define HTEST(args) \
  MSG0("testing %d-argument AM calls", args);                        \
  for (i = 0; i < iters; ++i) {                                      \
    gex_AM_Arg_t arg1;                                      \
    int goal = flag + 1;                                             \
    randomize();                                                     \
    arg1 = op_srep;                                                  \
      DO_CALL(gex_AM_RequestShort,SARGS(DEST(peer),args));       \
      GASNET_BLOCKUNTIL(flag == goal); ++goal;                       \
    arg1 = op_mrep;                                                  \
      DO_CALL(gex_AM_RequestMedium,MARGS(DEST(peer),args));      \
      GASNET_BLOCKUNTIL(flag == goal); ++goal;                       \
    arg1 = op_lrep;                                                  \
      DO_CALL(gex_AM_RequestLong,LARGS(DEST(peer),args,0));      \
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
void ping_shorthandler(gex_Token_t token) {
    gex_AM_ReplyShort(token, hidx_pong_shorthandler, 0);
}
void pong_shorthandler(gex_Token_t token) {
	  flag++;
}
void ping_medhandler(gex_Token_t token, void *buf, size_t nbytes) {
    MSGCHECK(MSZ(0));
    gex_AM_ReplyMedium(token, hidx_pong_medhandler, rand_payload, nbytes, GEX_EVENT_NOW, 0);
}
void pong_medhandler(gex_Token_t token, void *buf, size_t nbytes) {
    MSGCHECK(MSZ(0));
    flag++;
}
void ping_longhandler(gex_Token_t token, void *buf, size_t nbytes) {
    MSGCHECK(LSZ(0)); memset(buf, 0xa5, nbytes);
    gex_AM_ReplyLong(token, hidx_pong_longhandler, rand_payload, nbytes, fromseg + LSZ(0), GEX_EVENT_NOW, 0);
}
void pong_longhandler(gex_Token_t token, void *buf, size_t nbytes) {
    MSGCHECK(LSZ(0)); memset(buf, 0xa5, nbytes);
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
  size_t medsz, longsz;
  unsigned int seed = 0;
  int i;
  gex_AM_Entry_t htable[] = { 
    HFOREACH(HTABLE)
    { hidx_ping_shorthandler, ping_shorthandler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 0 },
    { hidx_pong_shorthandler, pong_shorthandler, GEX_FLAG_AM_REPLY|GEX_FLAG_AM_SHORT, 0 },
    { hidx_ping_medhandler,   ping_medhandler,   GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDIUM, 0 },
    { hidx_pong_medhandler,   pong_medhandler,   GEX_FLAG_AM_REPLY|GEX_FLAG_AM_MEDIUM, 0 },
    { hidx_ping_longhandler,  ping_longhandler,  GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_LONG, 0 },
    { hidx_pong_longhandler,  pong_longhandler,  GEX_FLAG_AM_REPLY|GEX_FLAG_AM_LONG, 0 }
  };

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testcore5", &argc, &argv, 0));

  if (argc > 1) iters = atoi(argv[1]);
  if (iters <= 0) iters = 10;

  if (argc > 2) maxsz = (size_t)gasnett_parse_int(argv[2], 1);
  if (maxsz <= 0) maxsz = 2*1024*1024;
  medsz = MAX(gex_AM_MaxRequestMedium(myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,0),
              gex_AM_MaxReplyMedium  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,0));
  medsz = MIN(maxsz, medsz);
  longsz = MAX(gex_AM_MaxRequestLong  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,0),
               gex_AM_MaxReplyLong    (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,0));
  longsz = MIN(maxsz, longsz);
  maxsz = MAX(medsz,longsz);

  if (argc > 3) seed = atoi(argv[3]);
  if (!seed) seed = (int)TIME();

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, sizeof(htable)/sizeof(gex_AM_Entry_t)));

  test_init("testcore5", 0, "(iters) (maxsz) (seed)");
  if (argc > 4) test_usage();

  TEST_BCAST(&seed, 0, &seed, sizeof(seed));
  TEST_SRAND(seed);

  TEST_PRINT_CONDUITINFO();

  mynode = gex_TM_QueryRank(myteam);
  gex_Rank_t nnodes = gex_TM_QuerySize(myteam);
  if (nnodes%2) {
    // w/ odd # of ranks, last one talks to self
    int last = nnodes - 1;
    if (mynode == last) {
      peer = from = mynode;
    } else {
      peer = (mynode + 1) % last;
      from = (mynode + last - 1) % last;
    }
  } else {
    peer = (mynode + 1) % nnodes;
    from = (mynode + nnodes - 1) % nnodes;
  }

  myseg = TEST_MYSEG();
  peerseg = TEST_SEG(peer);
  fromseg = TEST_SEG(from);
  rand_payload = test_malloc(maxsz);


  MSG0("Running %i iterations of var args AM interface tests with medsz=%"PRIuPTR" longsz=%"PRIuPTR" seed=%u",
       iters, (uintptr_t)medsz, (uintptr_t)longsz, seed);

  MSG0("testing 0-argument AM calls");
  for (i = 0; i < iters; ++i) {
    int goal = flag + 1;
    randomize();

    gex_AM_RequestShort(myteam, peer, hidx_ping_shorthandler, 0);
    GASNET_BLOCKUNTIL(flag == goal); ++goal;

    gex_AM_RequestMedium(myteam, peer, hidx_ping_medhandler, rand_payload, MSZ(0), GEX_EVENT_NOW, 0);
    GASNET_BLOCKUNTIL(flag == goal); ++goal;

    gex_AM_RequestLong(myteam, peer, hidx_ping_longhandler, rand_payload, LSZ(0), peerseg, GEX_EVENT_NOW, 0);
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
