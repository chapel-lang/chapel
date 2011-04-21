/*   $Source: /var/local/cvs/gasnet/tests/testam.c,v $
 *     $Date: 2009/05/15 20:56:06 $
 * $Revision: 1.32 $
 * Description: GASNet Active Messages performance test
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet.h>
uintptr_t maxsz = 0;
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR ((uintptr_t)maxsz)
#endif
#include <test.h>

int mynode = 0;
void *myseg = NULL;
int sender, recvr;
int peer;
void *peerseg = NULL;

void report(const char *desc, int64_t totaltime, int iters, uintptr_t sz, int rt) {
  if (sender) {
      char nodestr[10];
      if (gasnet_nodes() > 2) sprintf(nodestr,"%i: ",mynode);
      else nodestr[0] = '\0';
      printf("%c: %s%-46s: %6.3f sec %8.3f us", TEST_SECTION_NAME(),
        nodestr, desc, ((double)totaltime)/1000000, ((double)totaltime)/iters);
      if (sz) printf("  %7.3f MB/s", 
        (((double)sz)*(rt?2:1)*iters/(1024*1024)) / (((double)totaltime)/1000000));
      printf("\n");
      fflush(stdout);
  }
}

gasnet_hsl_t inchsl = GASNET_HSL_INITIALIZER;
#define INC(var) do {           \
    gasnet_hsl_lock(&inchsl);   \
    var++;                      \
    gasnet_hsl_unlock(&inchsl); \
  } while (0)

/* ------------------------------------------------------------------------------------ */
#define hidx_ping_shorthandler   201
#define hidx_pong_shorthandler   202

#define hidx_ping_medhandler     203
#define hidx_pong_medhandler     204

#define hidx_ping_longhandler    205
#define hidx_pong_longhandler    206

#define hidx_ping_shorthandler_flood   207
#define hidx_pong_shorthandler_flood   208

#define hidx_ping_medhandler_flood     209
#define hidx_pong_medhandler_flood     210

#define hidx_ping_longhandler_flood    211
#define hidx_pong_longhandler_flood    212

#define hidx_done_shorthandler   213

volatile int flag = 0;

void ping_shorthandler(gasnet_token_t token) {
  GASNET_Safe(gasnet_AMReplyShort0(token, hidx_pong_shorthandler));
}
void pong_shorthandler(gasnet_token_t token) {
  flag++;
}


void ping_medhandler(gasnet_token_t token, void *buf, size_t nbytes) {
  GASNET_Safe(gasnet_AMReplyMedium0(token, hidx_pong_medhandler, buf, nbytes));
}
void pong_medhandler(gasnet_token_t token, void *buf, size_t nbytes) {
  flag++;
}


void ping_longhandler(gasnet_token_t token, void *buf, size_t nbytes) {
  GASNET_Safe(gasnet_AMReplyLong0(token, hidx_pong_longhandler, buf, nbytes, peerseg));
}

void pong_longhandler(gasnet_token_t token, void *buf, size_t nbytes) {
  flag++;
}
/* ------------------------------------------------------------------------------------ */
void ping_shorthandler_flood(gasnet_token_t token) {
  GASNET_Safe(gasnet_AMReplyShort0(token, hidx_pong_shorthandler_flood));
}
void pong_shorthandler_flood(gasnet_token_t token) {
  INC(flag);
}


void ping_medhandler_flood(gasnet_token_t token, void *buf, size_t nbytes) {
  GASNET_Safe(gasnet_AMReplyMedium0(token, hidx_pong_medhandler_flood, buf, nbytes));
}
void pong_medhandler_flood(gasnet_token_t token, void *buf, size_t nbytes) {
  INC(flag);
}


void ping_longhandler_flood(gasnet_token_t token, void *buf, size_t nbytes) {
  GASNET_Safe(gasnet_AMReplyLong0(token, hidx_pong_longhandler_flood, buf, nbytes, peerseg));
}

void pong_longhandler_flood(gasnet_token_t token, void *buf, size_t nbytes) {
  INC(flag);
}


volatile int done = 0;
void done_shorthandler(gasnet_token_t token) {
  done = 1;
}

/* ------------------------------------------------------------------------------------ */
int iters=0;
int pollers=0;
int i = 0;
uintptr_t maxmed, maxlongreq, maxlongrep;
void *doAll(void*);

int main(int argc, char **argv) {
  int arg=1;

  gasnet_handlerentry_t htable[] = { 
    { hidx_ping_shorthandler,  ping_shorthandler  },
    { hidx_pong_shorthandler,  pong_shorthandler  },
    { hidx_ping_medhandler,    ping_medhandler    },
    { hidx_pong_medhandler,    pong_medhandler    },
    { hidx_ping_longhandler,   ping_longhandler   },
    { hidx_pong_longhandler,   pong_longhandler   },

    { hidx_ping_shorthandler_flood,  ping_shorthandler_flood  },
    { hidx_pong_shorthandler_flood,  pong_shorthandler_flood  },
    { hidx_ping_medhandler_flood,    ping_medhandler_flood    },
    { hidx_pong_medhandler_flood,    pong_medhandler_flood    },
    { hidx_ping_longhandler_flood,   ping_longhandler_flood   },
    { hidx_pong_longhandler_flood,   pong_longhandler_flood   },

    { hidx_done_shorthandler,  done_shorthandler  }
  };

  GASNET_Safe(gasnet_init(&argc, &argv));

  if ((argc-arg >= 2) && !strcmp(argv[arg], "-p")) {
#if GASNET_PAR
    pollers = atoi(argv[arg+1]);
    arg += 2;
#else
    if (gasnet_mynode() == 0) {
      fprintf(stderr, "testam %s\n", GASNET_CONFIG_STRING);
      fprintf(stderr, "ERROR: The -p option is only available in the PAR configuration.\n");
      fflush(NULL);
    }
    sleep(1);
    gasnet_exit(1);
#endif
  }
  if (argc-arg >= 1) iters = atoi(argv[arg]);
  if (!iters) iters = 1000;
  if (argc-arg >= 2) maxsz = atoi(argv[arg+1]);
  if (!maxsz) maxsz = 2*1024*1024;
  if (argc-arg >= 3) TEST_SECTION_PARSE(argv[arg+2]);

  GASNET_Safe(gasnet_attach(htable, sizeof(htable)/sizeof(gasnet_handlerentry_t),
                            TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
#if GASNET_PAR
  test_init("testam", 1, "[-p polling_threads] (iters) (maxsz) (test_sections)");
#else
  test_init("testam", 1, "(iters) (maxsz) (test_sections)");
#endif
  if (argc-arg >= 4) test_usage();
  mynode = gasnet_mynode();

  TEST_PRINT_CONDUITINFO();

  myseg = TEST_MYSEG();

  maxmed = MIN(maxsz, gasnet_AMMaxMedium());
  maxlongreq = MIN(maxsz, gasnet_AMMaxLongRequest());
  maxlongrep = MIN(maxsz, gasnet_AMMaxLongReply());
  peer = mynode ^ 1;
  if (peer == gasnet_nodes()) {
    /* w/ odd # of nodes, last one talks to self */
    peer = mynode;
  }
  peerseg = TEST_SEG(peer);
  sender = mynode % 2 == 0;
  recvr = (!sender || (gasnet_nodes()%2 == 1 && mynode == gasnet_nodes()-1));

  BARRIER();

  if (mynode == 0) {
#if GASNET_PAR
      printf("Running AM performance test with %i iterations and %i extra recvr polling threads...\n",iters,pollers);
#else
      printf("Running AM performance test with %i iterations...\n",iters);
#endif
      printf("   Msg Sz  Description                             Total time   Avg. time   Bandwidth\n"
             "   ------  -----------                             ----------   ---------   ---------\n");
      fflush(stdout);
  }
#if GASNET_PAR
  if (pollers)
    test_createandjoin_pthreads(pollers+1,doAll,NULL,0);
  else
#endif
    doAll(NULL);

  MSG("done.");

  gasnet_exit(0);
  return 0;
}

/* ------------------------------------------------------------------------------------ */
void doAMShort(void) {
    GASNET_BEGIN_FUNCTION();

    if (sender) { /* warm-up */
      flag = 0;                                                                                  
      for (i=0; i < iters; i++) {
        GASNET_Safe(gasnet_AMRequestShort0(peer, hidx_ping_shorthandler_flood));
      }
      GASNET_BLOCKUNTIL(flag == iters);
      GASNET_Safe(gasnet_AMRequestShort0(peer, hidx_ping_shorthandler));
      GASNET_BLOCKUNTIL(flag == iters+1);
    }
    BARRIER();
    /* ------------------------------------------------------------------------------------ */
    if (TEST_SECTION_BEGIN_ENABLED() && sender) {
      int64_t start = TIME();
      flag = -1;
      for (i=0; i < iters; i++) {
        GASNET_Safe(gasnet_AMRequestShort0(peer, hidx_ping_shorthandler));
        GASNET_BLOCKUNTIL(flag == i);
      }
      report("        AMShort     ping-pong roundtrip ReqRep",TIME() - start, iters, 0, 1);
    }

    BARRIER();
    /* ------------------------------------------------------------------------------------ */
    if (TEST_SECTION_ENABLED()) {
      int64_t start = TIME();
      flag = -1;
      BARRIER();
      if (sender && recvr) {
        assert(peer == mynode);
        for (i=0; i < iters; i++) {
          int lim = i << 1;
          GASNET_Safe(gasnet_AMRequestShort0(peer, hidx_pong_shorthandler));
          GASNET_BLOCKUNTIL(flag == lim);
          lim++;
          GASNET_Safe(gasnet_AMRequestShort0(peer, hidx_pong_shorthandler));
          GASNET_BLOCKUNTIL(flag == lim);
        }
      } else if (sender) {
        for (i=0; i < iters; i++) {
          GASNET_Safe(gasnet_AMRequestShort0(peer, hidx_pong_shorthandler));
          GASNET_BLOCKUNTIL(flag == i);
        }
      } else if (recvr) {
        for (i=0; i < iters; i++) {
          GASNET_BLOCKUNTIL(flag == i);
          GASNET_Safe(gasnet_AMRequestShort0(peer, hidx_pong_shorthandler));
        }
      }
      report("        AMShort     ping-pong roundtrip ReqReq",TIME() - start, iters, 0, 1);

    if (mynode == 0) { printf("\n"); fflush(stdout); }
    }
    BARRIER();
    /* ------------------------------------------------------------------------------------ */
   if (TEST_SECTION_ENABLED()) {
    if (sender) {
      int64_t start = TIME();
      flag = 0;
      BARRIER();
      for (i=0; i < iters; i++) {
        GASNET_Safe(gasnet_AMRequestShort0(peer, hidx_pong_shorthandler_flood));
      }
      if (recvr) GASNET_BLOCKUNTIL(flag == iters);
      BARRIER();
      report("        AMShort     flood     one-way   Req",TIME() - start, iters, 0, 0);
    } else {
      flag = 0;
      BARRIER();
      GASNET_BLOCKUNTIL(flag == iters);
      BARRIER();
    }

    if (mynode == 0) { printf("\n"); fflush(stdout); }
   }
    BARRIER();
    /* ------------------------------------------------------------------------------------ */
    if (TEST_SECTION_ENABLED() && sender) {
      int64_t start = TIME();
      flag = 0;
      for (i=0; i < iters; i++) {
        GASNET_Safe(gasnet_AMRequestShort0(peer, hidx_ping_shorthandler_flood));
      }
      GASNET_BLOCKUNTIL(flag == iters);
      report("        AMShort     flood     roundtrip ReqRep",TIME() - start, iters, 0, 1);

      if (mynode == 0) { printf("\n"); fflush(stdout); }
    }
    BARRIER();
}

/* ------------------------------------------------------------------------------------ */
#define ADVANCESZ(sz, maxsz) do {                   \
        if (!sz) sz = 1;                            \
        else if (sz < maxsz && sz*2 > maxsz) {      \
           /* indicate final non-power-of-two sz */ \
           if (!gasnet_mynode()) printf(" max:\n"); \
           sz = maxsz;                              \
        } else sz *= 2;                             \
  } while (0)

#define TESTAM_PERF(DESC_STR, AMREQUEST, PING_HIDX, PONG_HIDX,                   \
                                                      MAXREQ, MAXREP, DEST) do { \
    uintptr_t MAXREQREP = MIN(MAXREQ, MAXREP);                                   \
    if (sender) { /* warm-up */                                                  \
      flag = 0;                                                                  \
      for (i=0; i < iters; i++) {                                                \
        GASNET_Safe(AMREQUEST(peer, PING_HIDX##_flood, myseg, MAXREQREP DEST));  \
      }                                                                          \
      GASNET_BLOCKUNTIL(flag == iters);                                          \
      GASNET_Safe(AMREQUEST(peer, PING_HIDX, myseg, MAXREQREP DEST));            \
      GASNET_BLOCKUNTIL(flag == iters+1);                                        \
    }                                                                            \
    BARRIER();                                                                   \
    /* ---------------------------------------------------------- */             \
    if (TEST_SECTION_BEGIN_ENABLED()) {                                          \
      uintptr_t sz;                                                              \
      char msg[255];                                                             \
      for (sz = 0; sz <= MAXREQREP; ) {                                          \
        sprintf(msg, "%7llu "DESC_STR" ping-pong roundtrip ReqRep",              \
                     (unsigned long long)sz);                                    \
        BARRIER();                                                               \
        if (sender) {                                                            \
          int64_t start = TIME();                                                \
          flag = -1;                                                             \
          for (i=0; i < iters; i++) {                                            \
            GASNET_Safe(AMREQUEST(peer, PING_HIDX, myseg, sz DEST));             \
            GASNET_BLOCKUNTIL(flag == i);                                        \
          }                                                                      \
          report(msg,TIME() - start, iters, sz, 1);                              \
        }                                                                        \
        BARRIER();                                                               \
        ADVANCESZ(sz, MAXREQREP);                                                \
      }                                                                          \
      if (mynode == 0) { printf("\n"); fflush(stdout); }                         \
    }                                                                            \
    BARRIER();                                                                   \
    /* ---------------------------------------------------------- */             \
    if (TEST_SECTION_BEGIN_ENABLED()) {                                          \
      uintptr_t sz;                                                              \
      char msg[255];                                                             \
      for (sz = 0; sz <= MAXREQ; ) {                                             \
        sprintf(msg, "%7llu "DESC_STR" ping-pong roundtrip ReqReq",              \
                     (unsigned long long)sz);                                    \
        BARRIER();                                                               \
        {                                                                        \
          int64_t start = TIME();                                                \
          flag = -1;                                                             \
          BARRIER();                                                             \
          if (sender && recvr) {                                                 \
            assert(peer == mynode);                                              \
            for (i=0; i < iters; i++) {                                          \
              int lim = i << 1;                                                  \
              GASNET_Safe(AMREQUEST(peer, PONG_HIDX, myseg, sz DEST));           \
              GASNET_BLOCKUNTIL(flag == lim);                                    \
              lim++;                                                             \
              GASNET_Safe(AMREQUEST(peer, PONG_HIDX, myseg, sz DEST));           \
              GASNET_BLOCKUNTIL(flag == lim);                                    \
            }                                                                    \
          } else if (sender) {                                                   \
            for (i=0; i < iters; i++) {                                          \
              GASNET_Safe(AMREQUEST(peer, PONG_HIDX, myseg, sz DEST));           \
              GASNET_BLOCKUNTIL(flag == i);                                      \
            }                                                                    \
          } else if (recvr) {                                                    \
            for (i=0; i < iters; i++) {                                          \
              GASNET_BLOCKUNTIL(flag == i);                                      \
              GASNET_Safe(AMREQUEST(peer, PONG_HIDX, myseg, sz DEST));           \
            }                                                                    \
          }                                                                      \
          report(msg,TIME() - start, iters, sz, 1);                              \
        }                                                                        \
        BARRIER();                                                               \
        ADVANCESZ(sz, MAXREQ);                                                   \
      }                                                                          \
      if (mynode == 0) { printf("\n"); fflush(stdout); }                         \
    }                                                                            \
    BARRIER();                                                                   \
    /* ---------------------------------------------------------- */             \
    if (TEST_SECTION_BEGIN_ENABLED()) {                                          \
      uintptr_t sz;                                                              \
      char msg[255];                                                             \
      for (sz = 0; sz <= MAXREQ; ) {                                             \
        flag = 0;                                                                \
        sprintf(msg, "%7llu "DESC_STR" flood     one-way   Req",                 \
                     (unsigned long long)sz);                                    \
        BARRIER();                                                               \
        if (sender) {                                                            \
          int64_t start = TIME();                                                \
          for (i=0; i < iters; i++) {                                            \
            GASNET_Safe(AMREQUEST(peer, PONG_HIDX##_flood, myseg, sz DEST));     \
          }                                                                      \
          if (recvr) GASNET_BLOCKUNTIL(flag == iters);                           \
          BARRIER();                                                             \
          report(msg,TIME() - start, iters, sz, 0);                              \
        } else {                                                                 \
          GASNET_BLOCKUNTIL(flag == iters);                                      \
          BARRIER();                                                             \
        }                                                                        \
        BARRIER();                                                               \
        ADVANCESZ(sz, MAXREQ);                                                   \
      }                                                                          \
      if (mynode == 0) { printf("\n"); fflush(stdout); }                         \
    }                                                                            \
    BARRIER();                                                                   \
    /* ---------------------------------------------------------- */             \
    if (TEST_SECTION_BEGIN_ENABLED()) {                                          \
      uintptr_t sz;                                                              \
      char msg[255];                                                             \
      for (sz = 0; sz <= MAXREQREP; ) {                                          \
        sprintf(msg, "%7llu "DESC_STR" flood     roundtrip ReqRep",              \
                     (unsigned long long)sz);                                    \
        BARRIER();                                                               \
        if (sender) {                                                            \
          int64_t start = TIME();                                                \
          flag = 0;                                                              \
          for (i=0; i < iters; i++) {                                            \
            GASNET_Safe(AMREQUEST(peer, PING_HIDX##_flood, myseg, sz DEST));     \
          }                                                                      \
          GASNET_BLOCKUNTIL(flag == iters);                                      \
          report(msg,TIME() - start, iters, sz, 1);                              \
        }                                                                        \
        BARRIER();                                                               \
        ADVANCESZ(sz, MAXREQREP);                                                \
      }                                                                          \
      if (mynode == 0) { printf("\n"); fflush(stdout); }                         \
    }                                                                            \
    BARRIER();                                                                   \
    /* ---------------------------------------------------------- */             \
    if (TEST_SECTION_BEGIN_ENABLED()) {                                          \
      uintptr_t sz; int64_t start;                                               \
      char msg[255];                                                             \
      for (sz = 0; sz <= MAXREQREP; ) {                                          \
        sprintf(msg, "%7llu "DESC_STR" flood     roundtrip ReqReq",              \
                     (unsigned long long)sz);                                    \
        flag = 0;                                                                \
        BARRIER();                                                               \
        start = TIME();                                                          \
        for (i=0; i < iters; i++) {                                              \
          GASNET_Safe(AMREQUEST(peer, PONG_HIDX##_flood, myseg, sz DEST));       \
        }                                                                        \
        GASNET_BLOCKUNTIL(flag == iters);                                        \
        report(msg,TIME() - start, iters, sz, 1);                                \
                                                                                 \
        BARRIER();                                                               \
        ADVANCESZ(sz, MAXREQREP);                                                \
      }                                                                          \
      if (mynode == 0) { printf("\n"); fflush(stdout); }                         \
    }                                                                            \
    BARRIER();                                                                   \
  } while (0)

  #define MEDDEST
  #define LONGDEST , peerseg
/* ------------------------------------------------------------------------------------ */
void doAMMed(void) {
  GASNET_BEGIN_FUNCTION();
  TESTAM_PERF("AMMedium   ",    gasnet_AMRequestMedium0,    hidx_ping_medhandler,  hidx_pong_medhandler,  maxmed, maxmed, MEDDEST);
}
/* ------------------------------------------------------------------------------------ */
void doAMLong(void) {
  GASNET_BEGIN_FUNCTION();
  TESTAM_PERF("AMLong     ",      gasnet_AMRequestLong0,      hidx_ping_longhandler, hidx_pong_longhandler, maxlongreq, maxlongrep, LONGDEST);
}
/* ------------------------------------------------------------------------------------ */
void doAMLongAsync(void) {
  GASNET_BEGIN_FUNCTION();
  TESTAM_PERF("AMLongAsync", gasnet_AMRequestLongAsync0, hidx_ping_longhandler, hidx_pong_longhandler, maxlongreq, maxlongrep, LONGDEST);
}
/* ------------------------------------------------------------------------------------ */
void *doAll(void *ptr) {
  if (ptr) {
    if (recvr) GASNET_BLOCKUNTIL(done);
  } else {
    doAMShort();
    doAMMed();
    doAMLong();
    doAMLongAsync();
    if (recvr) GASNET_Safe(gasnet_AMRequestShort0(mynode, hidx_done_shorthandler));
  }
  return NULL;
}
/* ------------------------------------------------------------------------------------ */
