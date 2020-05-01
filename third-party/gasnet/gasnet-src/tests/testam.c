/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testam.c $
 * Description: GASNet Active Messages performance test
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>

int numnode = 0;
uintptr_t maxsz = 0;
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR (((numnode&1)?2:1)*(uintptr_t)alignup(maxsz,SIZEOF_GEX_RMA_VALUE_T))
#endif
#include <test.h>

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

int mynode = 0;
void *myseg = NULL;
int sender, recvr;
int peer;
uintptr_t max_step = 0;
void *request_addr = NULL;
void *reply_addr = NULL;

gex_Event_t *lc_opt = GEX_EVENT_NOW;
gex_Event_t *np_req_lc_opt = GEX_EVENT_NOW;
gex_Event_t *np_rep_lc_opt = GEX_EVENT_NOW;

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

gex_HSL_t inchsl = GEX_HSL_INITIALIZER;
#define INC(var) do {           \
    gex_HSL_Lock(&inchsl);   \
    var++;                      \
    gex_HSL_Unlock(&inchsl); \
  } while (0)


/* ------------------------------------------------------------------------------------ */

static enum {
  SRC_NOOP = 0,  // send untouched source buffer (default)
  SRC_GENERATE,  // "generate" payload (w/o memory reads)
  SRC_MEMCPY,    // memcpy fixed payload to source buffer
} src_mode;

static int use_np = 0;
static int np_cbuf = 0;
static void* zero_buffer;

GASNETT_INLINE(prep_payload)
void prep_payload(uint8_t *dst, size_t len) {
  if (!len) return;
  assert(! ((uintptr_t)dst % SIZEOF_GEX_RMA_VALUE_T));
  switch (src_mode) {
    case SRC_NOOP:
#if DEBUG
      if (use_np) (*dst)++; // defeat poison check
#endif
      return;
      break;
    case SRC_MEMCPY:
      memcpy(dst, zero_buffer, len);
      break;
    case SRC_GENERATE: {
      uint8_t *p = dst;
      for (size_t words = len / SIZEOF_GEX_RMA_VALUE_T; words; --words) {
        *(gex_RMA_Value_t *)p = words;
        p += SIZEOF_GEX_RMA_VALUE_T;
      }
      for (size_t bytes = len % SIZEOF_GEX_RMA_VALUE_T; bytes; --bytes) {
        *(p++) = bytes;
      }
      break;
    }
  }
}

#define RequestMedium0(tm,rank,hidx,src_addr,nbytes,lc_opt,flags) do {                      \
  if (use_np) {                                                                             \
    void *cbuf = np_cbuf ? src_addr : NULL;                                                 \
    gex_AM_SrcDesc_t sd =                                                                   \
            gex_AM_PrepareRequestMedium(tm,rank,cbuf,nbytes,nbytes,np_req_lc_opt,0,0);      \
    assert(gex_AM_SrcDescSize(sd) == nbytes);                                               \
    prep_payload(gex_AM_SrcDescAddr(sd), nbytes);                                           \
    gex_AM_CommitRequestMedium0(sd, hidx, nbytes);                                          \
  } else {                                                                                  \
    prep_payload(src_addr, nbytes);                                                         \
    gex_AM_RequestMedium0(tm, rank, hidx, src_addr, nbytes, lc_opt, flags);                 \
  }                                                                                         \
} while (0)

#define ReplyMedium0(token,hidx,src_addr,nbytes,lc_opt,flags) do {                          \
  if (use_np) {                                                                             \
    void *cbuf = np_cbuf ? src_addr : NULL;                                                 \
    gex_AM_SrcDesc_t sd =                                                                   \
            gex_AM_PrepareReplyMedium(token,cbuf,nbytes,nbytes,np_rep_lc_opt,0,0);          \
    assert(gex_AM_SrcDescSize(sd) == nbytes);                                               \
    prep_payload(gex_AM_SrcDescAddr(sd), nbytes);                                           \
    gex_AM_CommitReplyMedium0(sd, hidx, nbytes);                                            \
  } else {                                                                                  \
    prep_payload(src_addr, nbytes);                                                         \
    gex_AM_ReplyMedium0(token, hidx, src_addr, nbytes, lc_opt, flags);                      \
  }                                                                                         \
} while (0)

#define RequestLong0(tm,rank,hidx,src_addr,nbytes,dst_addr,lc_opt,flags) do {               \
  if (use_np) {                                                                             \
    void *cbuf = np_cbuf ? src_addr : NULL;                                                 \
    gex_AM_SrcDesc_t sd =                                                                   \
            gex_AM_PrepareRequestLong(tm,rank,cbuf,nbytes,nbytes,dst_addr,np_req_lc_opt,0,0);\
    assert(gex_AM_SrcDescSize(sd) == nbytes);                                               \
    prep_payload(gex_AM_SrcDescAddr(sd), nbytes);                                           \
    gex_AM_CommitRequestLong0(sd, hidx, nbytes, dst_addr);                                  \
  } else {                                                                                  \
    prep_payload(src_addr, nbytes);                                                         \
    gex_AM_RequestLong0(tm, rank, hidx, src_addr, nbytes, dst_addr, lc_opt, flags);         \
  }                                                                                         \
} while (0)

#define ReplyLong0(token,hidx,src_addr,nbytes,dst_addr,lc_opt,flags) do {                   \
  if (use_np) {                                                                             \
    void *cbuf = np_cbuf ? src_addr : NULL;                                                 \
    gex_AM_SrcDesc_t sd =                                                                   \
             gex_AM_PrepareReplyLong(token,cbuf,nbytes,nbytes,dst_addr,np_rep_lc_opt,0,0);  \
    assert(gex_AM_SrcDescSize(sd) == nbytes);                                               \
    prep_payload(gex_AM_SrcDescAddr(sd), nbytes);                                           \
    gex_AM_CommitReplyLong0(sd, hidx, nbytes, dst_addr);                                    \
  } else {                                                                                  \
    prep_payload(src_addr, nbytes);                                                         \
    gex_AM_ReplyLong0(token, hidx, src_addr, nbytes, dst_addr, lc_opt, flags);              \
  }                                                                                         \
} while (0)


/* ------------------------------------------------------------------------------------ */
gex_AM_Entry_t htable[];
#define hidx_ping_shorthandler   htable[0].gex_index
#define hidx_pong_shorthandler   htable[1].gex_index
#define hidx_ping_medhandler     htable[2].gex_index
#define hidx_pong_medhandler     htable[3].gex_index
#define hidx_ping_longhandler    htable[4].gex_index
#define hidx_pong_longhandler    htable[5].gex_index
#define hidx_ping_shorthandler_flood   htable[6].gex_index
#define hidx_pong_shorthandler_flood   htable[7].gex_index
#define hidx_ping_medhandler_flood     htable[8].gex_index
#define hidx_pong_medhandler_flood     htable[9].gex_index
#define hidx_ping_longhandler_flood    htable[10].gex_index
#define hidx_pong_longhandler_flood    htable[11].gex_index
#define hidx_done_shorthandler   htable[12].gex_index


volatile int flag = 0;

void ping_shorthandler(gex_Token_t token) {
  gex_AM_ReplyShort0(token, hidx_pong_shorthandler, 0);
}
void pong_shorthandler(gex_Token_t token) {
  flag++;
}


void ping_medhandler(gex_Token_t token, void *buf, size_t nbytes) {
  ReplyMedium0(token, hidx_pong_medhandler, buf, nbytes, GEX_EVENT_NOW, 0);
}
void pong_medhandler(gex_Token_t token, void *buf, size_t nbytes) {
  flag++;
}


void ping_longhandler(gex_Token_t token, void *buf, size_t nbytes) {
  ReplyLong0(token, hidx_pong_longhandler, buf, nbytes, reply_addr, GEX_EVENT_NOW, 0);
}

void pong_longhandler(gex_Token_t token, void *buf, size_t nbytes) {
  flag++;
}
/* ------------------------------------------------------------------------------------ */
void ping_shorthandler_flood(gex_Token_t token) {
  gex_AM_ReplyShort0(token, hidx_pong_shorthandler_flood, 0);
}
void pong_shorthandler_flood(gex_Token_t token) {
  INC(flag);
}


void ping_medhandler_flood(gex_Token_t token, void *buf, size_t nbytes) {
  ReplyMedium0(token, hidx_pong_medhandler_flood, buf, nbytes, GEX_EVENT_NOW, 0);
}
void pong_medhandler_flood(gex_Token_t token, void *buf, size_t nbytes) {
  INC(flag);
}


void ping_longhandler_flood(gex_Token_t token, void *buf, size_t nbytes) {
  ReplyLong0(token, hidx_pong_longhandler_flood, buf, nbytes, reply_addr, GEX_EVENT_NOW, 0);
}

void pong_longhandler_flood(gex_Token_t token, void *buf, size_t nbytes) {
  INC(flag);
}


volatile int done = 0;
void done_shorthandler(gex_Token_t token) {
  done = 1;
}
/* ------------------------------------------------------------------------------------ */
gex_AM_Entry_t htable[] = {
    { 0, ping_shorthandler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 0, NULL, NULL  },
    { 0, pong_shorthandler, GEX_FLAG_AM_REQREP|GEX_FLAG_AM_SHORT, 0, NULL, NULL  },

    { 0, ping_medhandler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDIUM, 0, NULL, NULL    },
    { 0, pong_medhandler, GEX_FLAG_AM_REQREP|GEX_FLAG_AM_MEDIUM, 0, NULL, NULL    },

    { 0, ping_longhandler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_LONG, 0, NULL, NULL   },
    { 0, pong_longhandler, GEX_FLAG_AM_REQREP|GEX_FLAG_AM_LONG, 0, NULL, NULL   },

    { 0, ping_shorthandler_flood, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 0, NULL, NULL  },
    { 0, pong_shorthandler_flood, GEX_FLAG_AM_REQREP|GEX_FLAG_AM_SHORT, 0, NULL, NULL  },

    { 0, ping_medhandler_flood, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDIUM, 0, NULL, NULL    },
    { 0, pong_medhandler_flood, GEX_FLAG_AM_REQREP|GEX_FLAG_AM_MEDIUM, 0, NULL, NULL    },

    { 0, ping_longhandler_flood, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_LONG, 0, NULL, NULL   },
    { 0, pong_longhandler_flood, GEX_FLAG_AM_REQREP|GEX_FLAG_AM_LONG, 0, NULL, NULL   },

    { 0, done_shorthandler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 0, NULL, NULL  }
};

/* ------------------------------------------------------------------------------------ */
int crossmachinemode = 0;
int insegment = 1;
int asynclc = 0;
int iters=0;
int pollers=0;
int i = 0;
uintptr_t maxmedreq, maxmedrep, maxlongreq, maxlongrep;
void *doAll(void*);

int main(int argc, char **argv) {
  int help=0;
  int arg=1;

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testam", &argc, &argv, 0));

  mynode = gex_TM_QueryRank(myteam);
  numnode = gex_TM_QuerySize(myteam);

  arg = 1;
  while (argc > arg) {
    if (!strcmp(argv[arg], "-p")) {
#if GASNET_PAR
      pollers = test_thread_limit(atoi(argv[arg+1])+1)-1;
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
    } else if (!strcmp(argv[arg], "-sync-req")) {
      asynclc = 0;
      lc_opt = GEX_EVENT_NOW;
      ++arg;
    } else if (!strcmp(argv[arg], "-async-req")) {
      asynclc = 1;
      lc_opt = GEX_EVENT_GROUP;
      ++arg;
    } else if (!strcmp(argv[arg], "-fp")) {
      use_np = 0;
      ++arg;
    } else if (!strcmp(argv[arg], "-np-cb")) {
      use_np = 1;
      np_cbuf = 1;
      ++arg;
    } else if (!strcmp(argv[arg], "-np-gb")) {
      use_np = 1;
      np_cbuf = 0;
      ++arg;
    } else if (!strcmp(argv[arg], "-src-noop")) {
      src_mode = SRC_NOOP;
      ++arg;
    } else if (!strcmp(argv[arg], "-src-generate")) {
      src_mode = SRC_GENERATE;
      ++arg;
    } else if (!strcmp(argv[arg], "-src-memcpy")) {
      src_mode = SRC_MEMCPY;
      ++arg;
    } else if (!strcmp(argv[arg], "-max-step")) {
      ++arg;
      if (argc > arg) { max_step = atoi(argv[arg]); arg++; }
      else help = 1;
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

  if (!max_step) max_step = maxsz;

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, sizeof(htable)/sizeof(gex_AM_Entry_t)));

#if GASNET_PAR
  #define PAR_USAGE \
               "  The -p option gives the number of polling threads, specified as\n" \
               "    a non-negative integer argument (default is no polling threads).\n"
#else
  #define PAR_USAGE ""
#endif
  test_init("testam", 1, "[options] (iters) (maxsz) (test_sections)\n"
               "  The '-in' or '-out' option selects whether the requestor's\n"
               "    buffer is in the GASNet segment or not (default is 'in').\n"
               PAR_USAGE
               "  The '-max-step N' option selects the maximum step between payload sizes,\n"
               "    which by default advance by doubling until the max size is reached.\n"
               "  The '-sync-req' or '-async-req' option selects synchronous or asynchronous\n"
               "    local completion of Medium and Long Requests (default is synchronous).\n"
               "  The '-fp', '-np-gb' or '-np-cb' option selects Fixed- or Negotiated-Payload\n"
               "    for Medium and Long AMs, as follows:\n"
               "      -fp:     Fixed-Payload (default)\n"
               "      -np-gb:  Negotiated-Payload with GASNet-provided buffer\n"
               "      -np-cb:  Negotiated-Payload with client-provided buffer\n"
               "  The '-src-*' options select treatment of the payload buffer used for\n"
               "    Medium and Long AMs, as follows:\n"
               "      -src-noop:      no per-operation initialization (default)\n"
               "      -src-generate:  initialized (w/o memory reads) on each AM injection\n"
               "      -src-memcpy:    initialized using memcpy() on each AM injection\n"
               "  The -c option enables cross-machine pairing (default is nearest neighbor).\n");
  if (help || argc > arg) test_usage();

  TEST_PRINT_CONDUITINFO();

  if (insegment) {
    myseg = TEST_MYSEG();
  } else {
    char *space = test_malloc(alignup(maxsz,PAGESZ) + PAGESZ);
    myseg = alignup_ptr(space, PAGESZ);
  }

  if (src_mode == SRC_MEMCPY) {
    zero_buffer = test_calloc(maxsz, 1);
  }

  np_req_lc_opt = np_cbuf ? lc_opt        : NULL;
  np_rep_lc_opt = np_cbuf ? GEX_EVENT_NOW : NULL;

  if (crossmachinemode) {
    if ((numnode%2) && (mynode == numnode-1)) {
      sender = 1;
      peer = mynode;
    } else {
      gex_Rank_t half = numnode / 2;
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

  gex_Event_t *tmp_lc_opt = use_np ? np_req_lc_opt : lc_opt;
  gex_Flags_t flags = use_np ? ( np_cbuf ? GEX_FLAG_AM_PREPARE_LEAST_CLIENT
                                         : GEX_FLAG_AM_PREPARE_LEAST_ALLOC) : 0;
  maxmedreq  = MIN(maxsz, gex_AM_MaxRequestMedium(myteam,peer,tmp_lc_opt,flags,0));
  maxmedrep  = MIN(maxsz, gex_AM_MaxReplyMedium  (myteam,peer,GEX_EVENT_NOW,flags,0));
  maxlongreq = MIN(maxsz, gex_AM_MaxRequestLong  (myteam,peer,tmp_lc_opt,flags,0));
  maxlongrep = MIN(maxsz, gex_AM_MaxReplyLong    (myteam,peer,GEX_EVENT_NOW,flags,0));

  recvr = !sender || (peer == mynode);

  // Long Request and Reply (distinct for loopback)
  reply_addr = TEST_SEG(peer);
  request_addr = (peer == mynode) ? (void*)((uintptr_t)reply_addr + alignup(maxsz,SIZEOF_GEX_RMA_VALUE_T))
                                  : reply_addr;

  BARRIER();

#if GASNET_PAR
  #define PAR_FMT "  %i extra recvr polling threads\n"
  #define PAR_ARG ,pollers
#else
  #define PAR_FMT /*empty*/
  #define PAR_ARG /*empty*/
#endif
  if (mynode == 0) {
      printf("Running %i iterations of %s AM performance with:\n"
             "  local addresses %sside the segment%s\n"
             "  %ssynchronous LC for Requests%s\n"
             "  %s\n"
             "  %s\n"
             PAR_FMT
             "  ...\n",
             iters,
             (crossmachinemode ? "cross-machine ": ""),
             (insegment ? "in" : "out"),
             (insegment ? " (default)" : ""),
             (asynclc ? "a": ""),
             (asynclc ? "": " (default)"),
             (!use_np   ? "fixed-Payload (default)"
              :(np_cbuf ? "negotiated-Payload with client-provided buffer"
                        : "negotiated-Payload with GASNet-provided buffer")),
             ((src_mode == SRC_NOOP)     ? "no payload initialization (default)"
             :(src_mode == SRC_GENERATE) ? "payload initialized by computation"
                                         : "payload initialized using memcpy()")
             PAR_ARG
            );
      printf("   Msg Sz  Description                             Total time   Avg. time   Bandwidth\n"
             "   ------  -----------                             ----------   ---------   ---------\n");
      fflush(stdout);
  }
#if GASNET_PAR
  TEST_SET_WAITMODE(pollers+1);
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
        gex_AM_RequestShort0(myteam, peer, hidx_ping_shorthandler_flood, 0);
      }
      GASNET_BLOCKUNTIL(flag == iters);
      gex_AM_RequestShort0(myteam, peer, hidx_ping_shorthandler, 0);
      GASNET_BLOCKUNTIL(flag == iters+1);
    }
    BARRIER();
    /* ------------------------------------------------------------------------------------ */
    if (TEST_SECTION_BEGIN_ENABLED() && sender) {
      int64_t start = TIME();
      flag = -1;
      for (i=0; i < iters; i++) {
        gex_AM_RequestShort0(myteam, peer, hidx_ping_shorthandler, 0);
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
          gex_AM_RequestShort0(myteam, peer, hidx_pong_shorthandler, 0);
          GASNET_BLOCKUNTIL(flag == lim);
          lim++;
          gex_AM_RequestShort0(myteam, peer, hidx_pong_shorthandler, 0);
          GASNET_BLOCKUNTIL(flag == lim);
        }
      } else if (sender) {
        for (i=0; i < iters; i++) {
          gex_AM_RequestShort0(myteam, peer, hidx_pong_shorthandler, 0);
          GASNET_BLOCKUNTIL(flag == i);
        }
      } else if (recvr) {
        for (i=0; i < iters; i++) {
          GASNET_BLOCKUNTIL(flag == i);
          gex_AM_RequestShort0(myteam, peer, hidx_pong_shorthandler, 0);
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
        gex_AM_RequestShort0(myteam, peer, hidx_pong_shorthandler_flood, 0);
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
        gex_AM_RequestShort0(myteam, peer, hidx_ping_shorthandler_flood, 0);
      }
      GASNET_BLOCKUNTIL(flag == iters);
      report("        AMShort     flood     roundtrip ReqRep",TIME() - start, iters, 0, 1);

      if (mynode == 0) { printf("\n"); fflush(stdout); }
    }
    BARRIER();
}

/* ------------------------------------------------------------------------------------ */

#define ADVANCESZ(sz, maxsz) do {                   \
        int step = MIN(max_step, sz);               \
        if (!sz) sz = 1;                            \
        else if (sz < maxsz && sz+step > maxsz) {   \
           /* indicate final non-power-of-two sz */ \
           if (!mynode) printf(" max:\n"); \
           sz = maxsz;                              \
        } else sz += step;                          \
  } while (0)

#define TESTAM_PERF(DESC_STR, AMREQUEST, PING_HIDX, PONG_HIDX,                   \
                                               MAXREQ, MAXREP, DEST) do {        \
    uintptr_t MAXREQREP = MIN(MAXREQ, MAXREP);                                   \
    if (sender) { /* warm-up */                                                  \
      flag = 0;                                                                  \
      AMREQUEST(myteam, peer, PING_HIDX, myseg,                                  \
                MAXREQREP DEST, lc_opt, 0);                                      \
      GASNET_BLOCKUNTIL(flag == 1);                                              \
      if (asynclc) gex_NBI_Wait(GEX_EC_AM,0);                                    \
      for (i=0; i < iters; i++) {                                                \
        AMREQUEST(myteam, peer, PING_HIDX##_flood, myseg,                        \
                  MAXREQREP DEST, lc_opt, 0);                                    \
      }                                                                          \
      GASNET_BLOCKUNTIL(flag == iters+1);                                        \
      if (asynclc) gex_NBI_Wait(GEX_EC_AM,0);                                    \
    }                                                                            \
    BARRIER();                                                                   \
    /* ---------------------------------------------------------- */             \
    if (TEST_SECTION_BEGIN_ENABLED()) {                                          \
      uintptr_t sz;                                                              \
      char msg[255];                                                             \
      for (sz = 0; sz <= MAXREQREP; ) {                                          \
        snprintf(msg, sizeof(msg),                                               \
                 "%7"PRIuPTR" "DESC_STR" ping-pong roundtrip ReqRep", sz);       \
        BARRIER();                                                               \
        if (sender) {                                                            \
          int64_t start = TIME();                                                \
          flag = -1;                                                             \
          for (i=0; i < iters; i++) {                                            \
            AMREQUEST(myteam, peer, PING_HIDX, myseg,                            \
                      sz DEST, lc_opt, 0);                                       \
            GASNET_BLOCKUNTIL(flag == i);                                        \
          }                                                                      \
          if (asynclc) gex_NBI_Wait(GEX_EC_AM,0);                                \
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
        snprintf(msg, sizeof(msg),                                               \
                 "%7"PRIuPTR" "DESC_STR" ping-pong roundtrip ReqReq", sz);       \
        BARRIER();                                                               \
        {                                                                        \
          int64_t start = TIME();                                                \
          flag = -1;                                                             \
          BARRIER();                                                             \
          if (sender && recvr) {                                                 \
            assert(peer == mynode);                                              \
            for (i=0; i < iters; i++) {                                          \
              int lim = i << 1;                                                  \
              AMREQUEST(myteam, peer, PONG_HIDX, myseg, sz DEST, lc_opt, 0);     \
              GASNET_BLOCKUNTIL(flag == lim);                                    \
              lim++;                                                             \
              AMREQUEST(myteam, peer, PONG_HIDX, myseg, sz DEST, lc_opt, 0);     \
              GASNET_BLOCKUNTIL(flag == lim);                                    \
            }                                                                    \
          } else if (sender) {                                                   \
            for (i=0; i < iters; i++) {                                          \
              AMREQUEST(myteam, peer, PONG_HIDX, myseg, sz DEST, lc_opt, 0);     \
              GASNET_BLOCKUNTIL(flag == i);                                      \
            }                                                                    \
          } else if (recvr) {                                                    \
            for (i=0; i < iters; i++) {                                          \
              GASNET_BLOCKUNTIL(flag == i);                                      \
              AMREQUEST(myteam, peer, PONG_HIDX, myseg, sz DEST, lc_opt, 0);     \
            }                                                                    \
          }                                                                      \
          if (asynclc) gex_NBI_Wait(GEX_EC_AM,0);                                \
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
        snprintf(msg, sizeof(msg),                                               \
                 "%7"PRIuPTR" "DESC_STR" flood     one-way   Req", sz);          \
        BARRIER();                                                               \
        if (sender) {                                                            \
          int64_t start = TIME();                                                \
          for (i=0; i < iters; i++) {                                            \
            AMREQUEST(myteam, peer, PONG_HIDX##_flood, myseg, sz DEST, lc_opt, 0); \
          }                                                                      \
          if (recvr) GASNET_BLOCKUNTIL(flag == iters);                           \
          if (asynclc) gex_NBI_Wait(GEX_EC_AM,0);                                \
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
        snprintf(msg, sizeof(msg),                                               \
                 "%7"PRIuPTR" "DESC_STR" flood     roundtrip ReqRep", sz);       \
        BARRIER();                                                               \
        if (sender) {                                                            \
          int64_t start = TIME();                                                \
          flag = 0;                                                              \
          for (i=0; i < iters; i++) {                                            \
            AMREQUEST(myteam, peer, PING_HIDX##_flood, myseg, sz DEST, lc_opt, 0); \
          }                                                                      \
          GASNET_BLOCKUNTIL(flag == iters);                                      \
          if (asynclc) gex_NBI_Wait(GEX_EC_AM,0);                                \
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
      for (sz = 0; sz <= MAXREQ; ) {                                             \
        snprintf(msg, sizeof(msg),                                               \
                 "%7"PRIuPTR" "DESC_STR" flood     two-way   Req", sz);          \
        flag = 0;                                                                \
        BARRIER();                                                               \
        start = TIME();                                                          \
        for (i=0; i < iters; i++) {                                              \
          AMREQUEST(myteam, peer, PONG_HIDX##_flood, myseg, sz DEST, lc_opt, 0); \
        }                                                                        \
        GASNET_BLOCKUNTIL(flag == iters);                                        \
        if (asynclc) gex_NBI_Wait(GEX_EC_AM,0);                                  \
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
  #define LONGDEST , request_addr
/* ------------------------------------------------------------------------------------ */
void doAMMed(void) {
  GASNET_BEGIN_FUNCTION();
  TESTAM_PERF("AMMedium   ",    RequestMedium0,           hidx_ping_medhandler,  hidx_pong_medhandler,  maxmedreq, maxmedrep, MEDDEST);
}
/* ------------------------------------------------------------------------------------ */
void doAMLong(void) {
  GASNET_BEGIN_FUNCTION();
  TESTAM_PERF("AMLong     ",      RequestLong0,             hidx_ping_longhandler, hidx_pong_longhandler, maxlongreq, maxlongrep, LONGDEST);
}
/* ------------------------------------------------------------------------------------ */
void *doAll(void *ptr) {
  if (ptr) {
    if (recvr) GASNET_BLOCKUNTIL(done);
  } else {
    doAMShort();
    doAMMed();
    doAMLong();
    if (recvr) gex_AM_RequestShort0(myteam, mynode, hidx_done_shorthandler, 0);
  }
  return NULL;
}
/* ------------------------------------------------------------------------------------ */
