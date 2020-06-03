/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testqueue.c $
 * Description: GASNet put/get injection performance test
 *   measures the average non-blocking put/get injection time 
 *   for increasing number of back-to-back operations
 *   over varying payload size and synchronization mechanisms
 *   reveals software-imposed queue depth backpressure limitations
 * Copyright 2002, Jaein Jeong and Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>
int maxsz = 0;
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR ((uintptr_t)(maxsz + alignup(maxsz,PAGESZ)))
#endif
#include "test.h"

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

int insegment = 1;

int myproc;
int numprocs;
int peerproc = -1;
int iamsender = 0;
int iamrecver = 0;
int multisender = 0;

int min_payload;
int max_payload;
int maxdepth = 0;

char *tgtmem;
void *msgbuf;
gex_Event_t *events;
volatile gex_RMA_Value_t regval = 5551212;

#define hidx_ping_shorthandler   201
#define hidx_ping_medhandler     202
#define hidx_ping_longhandler    203

gasnett_atomic_t amcount = gasnett_atomic_init(0);

void ping_shorthandler(gex_Token_t token) {
  gasnett_atomic_increment(&amcount,0);
}
void ping_medhandler(gex_Token_t token, void *buf, size_t nbytes) {
  gasnett_atomic_increment(&amcount,0);
}
void ping_longhandler(gex_Token_t token, void *buf, size_t nbytes) {
  gasnett_atomic_increment(&amcount,0);
}

gex_AM_Entry_t htable[] = { 
  { hidx_ping_shorthandler, ping_shorthandler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 0 },
  { hidx_ping_medhandler,   ping_medhandler,   GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDIUM, 0 },
  { hidx_ping_longhandler,  ping_longhandler,  GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_LONG, 0 }
};

int iters = 0;
int arg;
void *myseg;
void *alloc;
int firstlastmode = 0;
int fullduplexmode = 0;
int help = 0;   
int do_puts = 0, do_gets = 0, do_amshort = 0, do_ammedium = 0, do_amlong = 0;
int numflavors = 0;
int numsync = 0;
int do_bulk = 0, do_nonbulk = 0, do_value = 0;
int do_implicit = 0, do_explicit = 0, do_blocking = 0;

void do_bulkputs(void);
void do_nonbulkputgets(void);
void do_valueputgets(void);
void do_blockingputgets(void);
void do_amtests(void);

int main(int argc, char **argv) {
    /* call startup */
    GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testqueue", &argc, &argv, 0));

    /* parse arguments */
    arg = 1;
    while (argc > arg) {
      if (!strcmp(argv[arg], "-in")) {
        insegment = 1;
        ++arg;
      } else if (!strcmp(argv[arg], "-out")) {
        insegment = 0;
        ++arg;
      } else if (!strcmp(argv[arg], "-f")) {
        firstlastmode = 1;
        ++arg;
      } else if (!strcmp(argv[arg], "-a")) {
        fullduplexmode = 1;
        ++arg;
      } else if (!strcmp(argv[arg], "-p")) {
        do_puts = 1; numflavors++;
        ++arg;
      } else if (!strcmp(argv[arg], "-g")) {
        do_gets = 1; numflavors++;
        ++arg;
      } else if (!strcmp(argv[arg], "-s")) {
        do_amshort = 1; numflavors++;
        ++arg;
      } else if (!strcmp(argv[arg], "-m")) {
        do_ammedium = 1; numflavors++;
        ++arg;
      } else if (!strcmp(argv[arg], "-l")) {
        do_amlong = 1; numflavors++;
        ++arg;
      } else if (!strcmp(argv[arg], "-b")) {
        do_bulk = 1;
        ++arg;
      } else if (!strcmp(argv[arg], "-n")) {
        do_nonbulk = 1; 
        ++arg;
      } else if (!strcmp(argv[arg], "-v")) {
        do_value = 1; 
        ++arg;
      } else if (!strcmp(argv[arg], "-i")) {
        do_implicit = 1; numsync++;
        ++arg;
      } else if (!strcmp(argv[arg], "-e")) {
        do_explicit = 1; numsync++;
        ++arg;
      } else if (!strcmp(argv[arg], "-k")) {
        do_blocking = 1; numsync++;
        ++arg;
      } else if (argv[arg][0] == '-') {
        help = 1;
        ++arg;
      } else break;
    }
    if (fullduplexmode && firstlastmode) help = 1;
    if (argc > arg+3) help = 1;

    if (argc > arg) { iters = atoi(argv[arg]); arg++; }
    if (!iters) iters = 10;
    if (argc > arg) { maxdepth = atoi(argv[arg]); arg++; }
    if (!maxdepth) maxdepth = 1024; /* 1024 default */
    if (argc > arg) { maxsz = atoi(argv[arg]); arg++; }
    if (!maxsz) maxsz = 2*1024*1024; /* 2 MB default */

    #ifdef GASNET_SEGMENT_EVERYTHING
      if (maxsz > TEST_SEGSZ) { MSG("maxsz must be <= %"PRIuPTR" on GASNET_SEGMENT_EVERYTHING",(uintptr_t)TEST_SEGSZ); gasnet_exit(1); }
    #endif
    GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
    GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, sizeof(htable)/sizeof(gex_AM_Entry_t)));
    test_init("testqueue",1,"[-in|-out|-a|-f] [type options] (iters) (maxdepth) (maxsz)\n"
               "  The '-in' or '-out' option selects whether the initiator-side\n"
               "    memory is in the GASNet segment or not (default is 'in').\n"
               "  The -a option enables full-duplex mode, where all nodes send.\n"
               "  The -f option enables 'first/last' mode, where the first node\n"
               "  sends to the last, while all other nodes sit idle.\n"
               "  Test types to run: (defaults to everything)\n"
               "   -p : puts\n"
               "   -g : gets\n"
               "   -s : AMShort\n"
               "   -m : AMMedium\n"
               "   -l : AMLong\n"
               "   -n : Test non-bulk put/gets\n"
               "   -b : Test bulk put/gets\n"
               "   -v : Test value-based put/gets\n"
               "   -i : Test implicit-handle put/gets/medium/long\n"
               "   -e : Test explicit-handle put/gets/medium/long\n"
               "   -k : Test blocking put/gets/medium/long\n");
    if (help) test_usage();

    min_payload = 1;
    max_payload = maxsz;

    if (numflavors == 0) { /* default to all */
      do_puts = 1; 
      do_gets = 1; 
      do_amshort = 1;
      do_ammedium = 1;
      do_amlong = 1;
    }
    if (numsync == 0) { /* default to all */
      do_implicit = 1;
      do_explicit = 1;
      do_blocking = 1;
    }
    if (!do_bulk && !do_nonbulk && !do_value) {
      do_bulk = 1;
      do_nonbulk = 1;
      do_value = 1;
    }

    if (!do_implicit && !do_explicit && !do_blocking) {
      do_implicit = 1;
      do_explicit = 1;
      do_blocking = 1;
    }

    if (max_payload < min_payload) {
      printf("ERROR: maxsz must be >= %i\n",min_payload);
      gasnet_exit(1);
    }

    /* get SPMD info */
    myproc = gex_TM_QueryRank(myteam);
    numprocs = gex_TM_QuerySize(myteam);

    if (!firstlastmode) {
      /* Only allow 1 or even number for numprocs */
      if (numprocs > 1 && numprocs % 2 != 0) {
        MSG0("WARNING: This test requires a unary or even number of nodes. Test skipped.\n");
        gasnet_exit(0); /* exit 0 to prevent false negatives in test harnesses for smp-conduit */
      }
    }
    
    /* Setting peer thread rank */
    if (firstlastmode) {
      peerproc = numprocs-1;
      iamsender = (myproc == 0);
      iamrecver = (myproc == numprocs-1);
      multisender = 0;
    } else if (numprocs == 1) {
      peerproc = 0;
      iamsender = 1;
      iamrecver = 1;
      multisender = 0;
    } else { 
      peerproc = (myproc % 2) ? (myproc - 1) : (myproc + 1);
      iamsender = (fullduplexmode || myproc % 2 == 0);
      iamrecver = (fullduplexmode || !iamsender);
      multisender = (fullduplexmode || numprocs >= 4);
    }
    multisender = 1; /* messes up output on some systems */

    
    myseg = TEST_SEG(myproc);
    tgtmem = (char*)TEST_SEG(peerproc) + alignup(maxsz, PAGESZ);

    if (insegment) {
	msgbuf = (void *) myseg;
    } else {
	alloc = (void *) test_calloc(maxsz+PAGESZ,1); /* use calloc to prevent valgrind warnings */
        msgbuf = (void *) alignup(((uintptr_t)alloc), PAGESZ); /* ensure page alignment of base */
    }
    assert(((uintptr_t)msgbuf) % PAGESZ == 0);

    MSG0("Running %squeue test with local addr %sside segment, iters=%i, maxdepth=%i, sz: %i...%i", 
      firstlastmode ? "first/last " : (fullduplexmode ? "full-duplex ": ""),
      insegment ? "in" : "out", 
      iters, 
      maxdepth,
      min_payload, max_payload);
    MSG0("x-axis: queue depth, y-axis: message size, injection time in microseconds\n");
    BARRIER();

    events = (gex_Event_t *) test_malloc(sizeof(gex_Event_t) * maxdepth);

    do_bulkputs();
    do_nonbulkputgets();
    do_valueputgets();
    do_blockingputgets();
    do_amtests();

    BARRIER();
    test_free(events);
    if (!insegment) {
	test_free(alloc);
    }

    gasnet_exit(0);

    return 0;

}

    #define QUEUE_TEST(OPDESC, OP, SYNC, RECVRSYNC, PAYLOAD_LIMIT) do {     \
      int depth, payload, last_payload;                                     \
      BARRIER();                                                            \
      MSG0("\n%s\n--------------------\n", OPDESC);                         \
      { char header[1024];                                                  \
        char *pheader = header;                                             \
        size_t hused = 0;                                                   \
        snprintf(pheader, sizeof(header) - hused, "        ");              \
        pheader += strlen(pheader); hused = pheader - header;               \
        for (depth = 1; depth <= maxdepth; depth *= 2) {                    \
          snprintf(pheader, sizeof(header) - hused, " %7i", depth);         \
          pheader += strlen(pheader); hused = pheader - header;             \
        }                                                                   \
        PUTS0(header);                                                      \
      }                                                                     \
      last_payload = (((PAYLOAD_LIMIT) <= 0) ? max_payload :                \
                      MIN(max_payload, (PAYLOAD_LIMIT)));                   \
      for (payload = min_payload; payload <= last_payload; payload *= 2) {  \
        char row[1024];                                                     \
        char *prow = row;                                                   \
        size_t rused = 0;                                                   \
        if (payload < 0) break; /* Overflow */                              \
        snprintf(prow, sizeof(row) - rused, "%-8i", payload);               \
        prow += strlen(prow); rused = prow - row;                           \
        if (!multisender) {                                                 \
          fputs(row,stdout); fflush(stdout);                                \
          prow = row; rused = 0;                                            \
        }                                                                   \
        depth = 1;                                                          \
        if (iamsender) { /* Prime i-cache, free-lists, firehose, etc. */    \
          int i = 0;                                                        \
          OP;                                                               \
          { SYNC; }                                                         \
        }                                                                   \
        if (iamrecver) { RECVRSYNC; }                                       \
        for (depth = 1; depth <= maxdepth; depth *= 2) {                    \
          BARRIER();                                                        \
                                                                            \
          if (iamsender) {                                                  \
            int iter,i;                                                     \
            gasnett_tick_t total = 0,                                       \
                           min = GASNETT_TICK_MAX,                          \
                           max = GASNETT_TICK_MIN;                          \
            for (iter = 0; iter < iters; iter++) {                          \
              gasnett_tick_t begin, end, thistime;                          \
              BARRIER();                                                    \
              /* measure time to inject depth operations of payload sz */   \
              begin = gasnett_ticks_now();                                  \
                for (i = 0; i < depth; i++) {                               \
                  OP;                                                       \
                }                                                           \
              end = gasnett_ticks_now();                                    \
              { SYNC; }                                                     \
              if (iamrecver) { RECVRSYNC; }                                 \
              BARRIER();                                                    \
              thistime = (end - begin);                                     \
              total += thistime;                                            \
              min = MIN(min,thistime);                                      \
              max = MAX(max,thistime);                                      \
            }                                                               \
            { double avgus = gasnett_ticks_to_ns(total) / 1000.0 /          \
                             (double)iters / (double)depth;                 \
              double minus = gasnett_ticks_to_ns(min) / 1000.0 / (double)depth; \
              double maxus = gasnett_ticks_to_ns(max) / 1000.0 / (double)depth; \
              int prec;                                                     \
              if (avgus < 1000.0) prec = 3;                                 \
              else if (avgus < 10000.0) prec = 2;                           \
              else if (avgus < 100000.0) prec = 1;                          \
              else prec = 0;                                                \
              snprintf(prow, sizeof(row) - rused, " %7.*f", prec, avgus);   \
              prow += strlen(prow); rused = prow - row;                     \
              if (!multisender) {                                           \
                  fputs(row,stdout); fflush(stdout);                        \
                  prow = row; rused = 0;                                    \
              }                                                             \
            }                                                               \
          } else {                                                          \
            int i;                                                          \
            for (i = 0; i < iters; i++) {                                   \
              BARRIER();                                                    \
              if (iamrecver) { RECVRSYNC; }                                 \
              BARRIER();                                                    \
            }                                                               \
          }                                                                 \
        }                                                                   \
        if (iamsender) {                                                    \
          puts(row); fflush(stdout);                                        \
        }                                                                   \
      }                                                                     \
    } while (0)

void do_bulkputs(void) {
    if (do_puts && do_bulk && do_explicit) {
      QUEUE_TEST("gex_RMA_PutNB/bulk", 
                 events[i] = gex_RMA_PutNB(myteam, peerproc, tgtmem, msgbuf, payload, GEX_EVENT_DEFER, 0), 
                 gex_Event_WaitAll(events, depth, 0), (void)0, 0);
    }

    if (do_puts && do_bulk && do_implicit) {
      QUEUE_TEST("gex_RMA_PutNBI/bulk", 
                 gex_RMA_PutNBI(myteam, peerproc, tgtmem, msgbuf, payload, GEX_EVENT_DEFER, 0), 
                 gex_NBI_Wait(GEX_EC_ALL,0), (void)0, 0);
    }
}
void do_nonbulkputgets(void) {
    if (do_puts && do_nonbulk && do_explicit) {
      QUEUE_TEST("gex_RMA_PutNB", 
                 events[i] = gex_RMA_PutNB(myteam, peerproc, tgtmem, msgbuf, payload, GEX_EVENT_NOW, 0), 
                 gex_Event_WaitAll(events, depth, 0), (void)0, 0);
    }

    if (do_gets && do_explicit) {
      QUEUE_TEST("gex_RMA_GetNB", 
                 events[i] = gex_RMA_GetNB(myteam, msgbuf, peerproc, tgtmem, payload, 0), 
                 gex_Event_WaitAll(events, depth, 0), (void)0, 0);
    }

    if (do_puts && do_nonbulk && do_implicit) {
      QUEUE_TEST("gex_RMA_PutNBI", 
                 gex_RMA_PutNBI(myteam, peerproc, tgtmem, msgbuf, payload, GEX_EVENT_NOW, 0), 
                 gex_NBI_Wait(GEX_EC_ALL,0), (void)0, 0);
    }

    if (do_gets && do_implicit) {
      QUEUE_TEST("gex_RMA_GetNBI", 
                 gex_RMA_GetNBI(myteam, msgbuf, peerproc, tgtmem, payload, 0), 
                 gex_NBI_Wait(GEX_EC_ALL,0), (void)0, 0);
    }
}
void do_valueputgets(void) {
    if (do_puts && do_value && do_explicit) {
      QUEUE_TEST("gex_RMA_PutNBVal",
                 events[i] = gex_RMA_PutNBVal(myteam, peerproc, tgtmem, regval, payload, 0),
                 gex_Event_WaitAll(events, depth, 0),
                 (void)0, SIZEOF_GEX_RMA_VALUE_T);
    }

    if (do_puts && do_value && do_implicit) {
      QUEUE_TEST("gex_RMA_PutNBIVal",
                 gex_RMA_PutNBIVal(myteam, peerproc, tgtmem, regval, payload, 0),
                 gex_NBI_Wait(GEX_EC_ALL,0),
                 (void)0, SIZEOF_GEX_RMA_VALUE_T);
    }

}
void do_blockingputgets(void) {
    if (do_puts && do_blocking) {
      QUEUE_TEST("gex_RMA_PutBlocking (BLOCKING - represents round-trip latency)", 
                 gex_RMA_PutBlocking(myteam, peerproc, tgtmem, msgbuf, payload, 0),
                 (void)0, (void)0, 0);
    }

    if (do_gets && do_blocking) {
      QUEUE_TEST("gex_RMA_GetBlocking (BLOCKING - represents round-trip latency)",
                 gex_RMA_GetBlocking(myteam, msgbuf, peerproc, tgtmem, payload, 0),
                 (void)0, (void)0, 0);
    }

    if (do_puts && do_value && do_blocking) {
      QUEUE_TEST("gex_RMA_PutBlockingVal (BLOCKING - represents round-trip latency)",
                 gex_RMA_PutBlockingVal(myteam, peerproc, tgtmem, regval, payload, 0),
                 (void)0, (void)0, SIZEOF_GEX_RMA_VALUE_T);
    }

    if (do_gets && do_value && do_blocking) {
      QUEUE_TEST("gex_RMA_GetBlockingVal (BLOCKING - represents round-trip latency)", 
                 regval ^= gex_RMA_GetBlockingVal(myteam, peerproc, tgtmem, payload, 0), 
                 (void)0, (void)0, SIZEOF_GEX_RMA_VALUE_T);
    }
}
void do_amtests(void) {
    if (do_amshort) {
      gasnett_atomic_set(&amcount, 0, 0);
      QUEUE_TEST("gex_AM_RequestShort0", 
                 gex_AM_RequestShort0(myteam, peerproc, hidx_ping_shorthandler, 0), (void)0,
                { assert(iamrecver);
                  GASNET_BLOCKUNTIL(gasnett_atomic_read(&amcount,0) == depth); 
                  gasnett_atomic_set(&amcount, 0, 0); }, 
                 min_payload);
    }

    if (do_ammedium && do_blocking) {
      gasnett_atomic_set(&amcount, 0, 0);
      QUEUE_TEST("gex_AM_RequestMedium0 (blocking for LC)",
                 gex_AM_RequestMedium0(myteam, peerproc, hidx_ping_medhandler,
                                           msgbuf, payload, GEX_EVENT_NOW, 0), (void)0,
                { assert(iamrecver);
                  GASNET_BLOCKUNTIL(gasnett_atomic_read(&amcount,0) == depth); 
                  gasnett_atomic_set(&amcount, 0, 0); }, 
                 gex_AM_MaxRequestMedium(myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,0));
    }
    if (do_ammedium && do_explicit) {
      QUEUE_TEST("gex_AM_RequestMedium0 (explicit handle for LC)",
                 gex_AM_RequestMedium0(myteam, peerproc, hidx_ping_medhandler,
                                         msgbuf, payload, &events[i], 0),
                 gex_Event_WaitAll(events, depth, 0),
                { assert(iamrecver);
                  GASNET_BLOCKUNTIL(gasnett_atomic_read(&amcount,0) == depth);
                  gasnett_atomic_set(&amcount, 0, 0); },
                 gex_AM_MaxRequestMedium(myteam,GEX_RANK_INVALID,NULL,0,0));
    }
    if (do_ammedium && do_implicit) {
      QUEUE_TEST("gex_AM_RequestMedium0 (implicit handle for LC)",
                 gex_AM_RequestMedium0(myteam, peerproc, hidx_ping_medhandler,
                                         msgbuf, payload, GEX_EVENT_GROUP, 0),
                 gex_NBI_Wait(GEX_EC_ALL,0),
                { assert(iamrecver);
                  GASNET_BLOCKUNTIL(gasnett_atomic_read(&amcount,0) == depth);
                  gasnett_atomic_set(&amcount, 0, 0); },
                 gex_AM_MaxRequestMedium(myteam,GEX_RANK_INVALID,GEX_EVENT_GROUP,0,0));
    }

    if (do_amlong && do_blocking) {
      gasnett_atomic_set(&amcount, 0, 0);
      QUEUE_TEST("gex_AM_RequestLong0 (blocking for LC)",
                 gex_AM_RequestLong0(myteam, peerproc, hidx_ping_longhandler,
                                         msgbuf, payload, tgtmem, GEX_EVENT_NOW, 0), (void)0,
                { assert(iamrecver);
                  GASNET_BLOCKUNTIL(gasnett_atomic_read(&amcount,0) == depth); 
                  gasnett_atomic_set(&amcount, 0, 0); }, 
                 gex_AM_MaxRequestLong(myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,0));
    }
    if (do_amlong && do_explicit) {
      QUEUE_TEST("gex_AM_RequestLong0 (explicit handle for LC)",
                 gex_AM_RequestLong0(myteam, peerproc, hidx_ping_longhandler,
                                         msgbuf, payload, tgtmem, &events[i], 0),
                 gex_Event_WaitAll(events, depth, 0),
                { assert(iamrecver);
                  GASNET_BLOCKUNTIL(gasnett_atomic_read(&amcount,0) == depth);
                  gasnett_atomic_set(&amcount, 0, 0); },
                 gex_AM_MaxRequestLong(myteam,GEX_RANK_INVALID,NULL,0,0));
    }
    if (do_amlong && do_implicit) {
      QUEUE_TEST("gex_AM_RequestLong0 (implicit handle for LC)",
                 gex_AM_RequestLong0(myteam, peerproc, hidx_ping_longhandler,
                                         msgbuf, payload, tgtmem, GEX_EVENT_GROUP, 0),
                 gex_NBI_Wait(GEX_EC_ALL,0),
                { assert(iamrecver);
                  GASNET_BLOCKUNTIL(gasnett_atomic_read(&amcount,0) == depth);
                  gasnett_atomic_set(&amcount, 0, 0); },
                 gex_AM_MaxRequestLong(myteam,GEX_RANK_INVALID,GEX_EVENT_GROUP,0,0));
    }
}



/* ------------------------------------------------------------------------------------ */
