/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testam.c $
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
int numnode = 0;
void *myseg = NULL;
int sender, recvr;
int peer;
void *peerseg = NULL;

void report(const char *desc, int64_t totaltime, int iters, uintptr_t sz, int rt) {
  if (sender) {
      char nodestr[10];
      if (numnode > 2) snprintf(nodestr,sizeof(nodestr),"%i: ",mynode);
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
int crossmachinemode = 0;
int insegment = 1;
int iters=0;
int pollers=0;
int i = 0;
uintptr_t maxmed, maxlongreq, maxlongrep;
void *doAll(void*);

int main(int argc, char **argv) {
  int help=0;
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

  mynode = gasnet_mynode();
  numnode = gasnet_nodes();

  arg = 1;
  while (argc > arg) {
    if (!strcmp(argv[arg], "-p")) {
#if GASNET_PAR
      pollers = atoi(argv[arg+1]);
      arg += 2;
#else
      if (0 == mynode) {
        fprintf(stderr, "testam %s\n", GASNET_CONFIG_STRING);
        fprintf(stderr, "ERROR: The -p option is only available in the PAR configuration.\n");
        fflush(NULL);
      }
      sleep(1);
      gasnet_exit(1);
#endif
    } else if (!strcmp(argv[arg], "-in")) {
      insegment = 1;
      ++arg;
    } else if (!strcmp(argv[arg], "-out")) {
      insegment = 0;
      ++arg;
    } else if (!strcmp(argv[arg], "-c")) {
      crossmachinemode = 1;
      ++arg;
    } else if (argv[arg][0] == '-') {
      help = 1;
      ++arg;
    } else break;
  }

  if (argc > arg) { iters = atoi(argv[arg]); ++arg; }
  if (!iters) iters = 1000;
  if (argc > arg) { maxsz = atoi(argv[arg]); ++arg; }
  if (!maxsz) maxsz = 2*1024*1024;
  if (argc > arg) { TEST_SECTION_PARSE(argv[arg]); ++arg; }

  GASNET_Safe(gasnet_attach(htable, sizeof(htable)/sizeof(gasnet_handlerentry_t),
                            TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
#if GASNET_PAR
  test_init("testam", 1, "[options] (iters) (maxsz) (test_sections)\n"
               "  The '-in' or '-out' option selects whether the requestor's\n"
               "    buffer is in the GASNet segment or not (default is 'in').\n"
               "  The -p option gives the number of polling threads, specified as\n"
               "    a non-negative integer argument (default is no polling threads).\n"
               "  The -c option enables cross-machine pairing (default is nearest neighbor).\n");
#else
  test_init("testam", 1, "[options] (iters) (maxsz) (test_sections)\n"
               "  The '-in' or '-out' option selects whether the requestor's\n"
               "    buffer is in the GASNet segment or not (default is 'in').\n"
               "  The -c option enables cross-machine pairing (default is nearest neighbor).\n");
#endif
  if (help || argc > arg) test_usage();

  TEST_PRINT_CONDUITINFO();

  if (insegment) {
    myseg = TEST_MYSEG();
  } else {
    char *space = test_malloc(alignup(maxsz,PAGESZ) + PAGESZ);
    myseg = alignup_ptr(space, PAGESZ);
  }

  maxmed = MIN(maxsz, gasnet_AMMaxMedium());
  maxlongreq = MIN(maxsz, gasnet_AMMaxLongRequest());
  maxlongrep = MIN(maxsz, gasnet_AMMaxLongReply());

  if (crossmachinemode) {
    if ((numnode%2) && (mynode == numnode-1)) {
      sender = 1;
      peer = mynode;
    } else {
      gasnet_node_t half = numnode / 2;
      sender = (mynode < half);
      peer = sender ? (mynode + half) : (mynode - half);
    }
  } else {
    peer = mynode ^ 1;
    sender = mynode % 2 == 0;
    if (peer == numnode) {
      peer = mynode;
    }
  }

  recvr = !sender || (peer == mynode);

  peerseg = TEST_SEG(peer);

  BARRIER();

  if (mynode == 0) {
      printf("Running %sAM performance test with %i iterations"
#if GASNET_PAR
             " and %i extra recvr polling threads"
#endif
             "...\n",
             (crossmachinemode ? "cross-machine ": ""),
             iters
#if GASNET_PAR
             ,pollers
#endif
            );
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
           if (!mynode) printf(" max:\n"); \
           sz = maxsz;                              \
        } else sz *= 2;                             \
  } while (0)

#define TESTAM_PERF(DESC_STR, AMREQUEST, PING_HIDX, PONG_HIDX,                   \
                                               MAXREQ, MAXREP, ASYNC, DEST) do { \
    uintptr_t MAXREQREP = MIN(MAXREQ, MAXREP);                                   \
    if (sender) { /* warm-up */                                                  \
      flag = 0;                                                                  \
      GASNET_Safe(AMREQUEST(peer, PING_HIDX, myseg, MAXREQREP DEST));            \
      GASNET_BLOCKUNTIL(flag == 1);                                              \
      if (!ASYNC) {                                                              \
        for (i=0; i < iters; i++) {                                              \
          GASNET_Safe(AMREQUEST(peer, PING_HIDX##_flood, myseg, MAXREQREP DEST));\
        }                                                                        \
        GASNET_BLOCKUNTIL(flag == iters+1);                                      \
      }                                                                          \
    }                                                                            \
    BARRIER();                                                                   \
    /* ---------------------------------------------------------- */             \
    if (TEST_SECTION_BEGIN_ENABLED()) {                                          \
      uintptr_t sz;                                                              \
      char msg[255];                                                             \
      for (sz = 0; sz <= MAXREQREP; ) {                                          \
        snprintf(msg, sizeof(msg), "%7llu "DESC_STR" ping-pong roundtrip ReqRep",\
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
    if (!ASYNC && TEST_SECTION_BEGIN_ENABLED()) {                                \
      uintptr_t sz;                                                              \
      char msg[255];                                                             \
      for (sz = 0; sz <= MAXREQ; ) {                                             \
        snprintf(msg, sizeof(msg), "%7llu "DESC_STR" ping-pong roundtrip ReqReq",\
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
    if (!ASYNC && TEST_SECTION_BEGIN_ENABLED()) {                                \
      uintptr_t sz;                                                              \
      char msg[255];                                                             \
      for (sz = 0; sz <= MAXREQ; ) {                                             \
        flag = 0;                                                                \
        snprintf(msg, sizeof(msg), "%7llu "DESC_STR" flood     one-way   Req",   \
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
        snprintf(msg, sizeof(msg), "%7llu "DESC_STR" flood     roundtrip ReqRep",\
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
    if (!ASYNC && TEST_SECTION_BEGIN_ENABLED()) {                                \
      uintptr_t sz; int64_t start;                                               \
      char msg[255];                                                             \
      for (sz = 0; sz <= MAXREQ; ) {                                             \
        snprintf(msg, sizeof(msg), "%7llu "DESC_STR" flood     two-way   Req",   \
                     (unsigned long long)sz);                                    \
        flag = 0;                                                                \
        BARRIER();                                                               \
        start = TIME();                                                          \
        for (i=0; i < iters; i++) {                                              \
          GASNET_Safe(AMREQUEST(peer, PONG_HIDX##_flood, myseg, sz DEST));       \
        }                                                                        \
        GASNET_BLOCKUNTIL(flag == iters);                                        \
        report(msg,TIME() - start, iters, sz, 0);                                \
                                                                                 \
        BARRIER();                                                               \
        ADVANCESZ(sz, MAXREQ);                                                   \
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
  TESTAM_PERF("AMMedium   ",    gasnet_AMRequestMedium0,    hidx_ping_medhandler,  hidx_pong_medhandler,  maxmed, maxmed, 0, MEDDEST);
}
/* ------------------------------------------------------------------------------------ */
void doAMLong(void) {
  GASNET_BEGIN_FUNCTION();
  TESTAM_PERF("AMLong     ",      gasnet_AMRequestLong0,      hidx_ping_longhandler, hidx_pong_longhandler, maxlongreq, maxlongrep, 0, LONGDEST);
}
/* ------------------------------------------------------------------------------------ */
void doAMLongAsync(void) {
  GASNET_BEGIN_FUNCTION();
  TESTAM_PERF("AMLongAsync", gasnet_AMRequestLongAsync0, hidx_ping_longhandler, hidx_pong_longhandler, maxlongreq, maxlongrep, 1, LONGDEST);
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
