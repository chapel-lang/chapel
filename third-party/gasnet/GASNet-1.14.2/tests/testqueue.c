/*   $Source: /var/local/cvs/gasnet/tests/testqueue.c,v $
 *     $Date: 2010/04/24 03:52:58 $
 * $Revision: 1.16.2.2 $
 * Description: GASNet put/get injection performance test
 *   measures the average non-blocking put/get injection time 
 *   for increasing number of back-to-back operations
 *   over varying payload size and synchronization mechanisms
 *   reveals software-imposed queue depth backpressure limitations
 * Copyright 2002, Jaein Jeong and Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include "gasnet.h"
int maxsz = 0;
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR ((uintptr_t)maxsz)
#endif
#include "test.h"

int insegment = 0;

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
gasnet_handle_t *handles;

#define hidx_ping_shorthandler   201
#define hidx_ping_medhandler     202
#define hidx_ping_longhandler    203

gasnett_atomic_t amcount = gasnett_atomic_init(0);

void ping_shorthandler(gasnet_token_t token) {
  gasnett_atomic_increment(&amcount,0);
}
void ping_medhandler(gasnet_token_t token, void *buf, size_t nbytes) {
  gasnett_atomic_increment(&amcount,0);
}
void ping_longhandler(gasnet_token_t token, void *buf, size_t nbytes) {
  gasnett_atomic_increment(&amcount,0);
}

gasnet_handlerentry_t htable[] = { 
  { hidx_ping_shorthandler,  ping_shorthandler  },
  { hidx_ping_medhandler,    ping_medhandler    },
  { hidx_ping_longhandler,   ping_longhandler   }
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
int do_bulk = 0, do_nonbulk = 0;
int do_implicit = 0, do_explicit = 0, do_blocking = 0;

void do_bulkputgets(void);
void do_nonbulkputgets(void);
void do_blockingputgets(void);
void do_amtests(void);

int main(int argc, char **argv) {
    /* call startup */
    GASNET_Safe(gasnet_init(&argc, &argv));

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
      if (maxsz > TEST_SEGSZ) { MSG("maxsz must be <= %lu on GASNET_SEGMENT_EVERYTHING",(unsigned long)TEST_SEGSZ); gasnet_exit(1); }
    #endif
    GASNET_Safe(gasnet_attach(htable, sizeof(htable)/sizeof(gasnet_handlerentry_t), 
                              TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
    test_init("testqueue",1,"[-in|-out|-a|-f] (iters) (maxdepth) (maxsz)\n"
               "  The 'in' or 'out' option selects whether the initiator-side\n"
               "  memory is in the GASNet segment or not (default is not).\n"
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
               "   -i : Test implicit-handle put/gets\n"
               "   -e : Test explicit-handle put/gets\n"
               "   -k : Test blocking put/gets\n");
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
    if (!do_bulk && !do_nonbulk) {
      do_bulk = 1;
      do_nonbulk = 1;
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
    myproc = gasnet_mynode();
    numprocs = gasnet_nodes();

    if (!firstlastmode) {
      /* Only allow 1 or even number for numprocs */
      if (numprocs > 1 && numprocs % 2 != 0) {
        MSG("WARNING: This test requires a unary or even number of nodes. Test skipped.\n");
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
    tgtmem = TEST_SEG(peerproc);

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

    handles = (gasnet_handle_t *) test_malloc(sizeof(gasnet_handle_t) * maxdepth);

    do_bulkputgets();
    do_nonbulkputgets();
    do_blockingputgets();
    do_amtests();

    BARRIER();
    test_free(handles);
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
        sprintf(pheader, "        "); pheader += strlen(pheader);           \
        for (depth = 1; depth <= maxdepth; depth *= 2) {                    \
          sprintf(pheader, " %7i", depth); pheader += strlen(pheader);      \
        }                                                                   \
        PUTS0(header);                                                      \
      }                                                                     \
      last_payload = (((PAYLOAD_LIMIT) <= 0) ? max_payload :                \
                      MIN(max_payload, (PAYLOAD_LIMIT)));                   \
      for (payload = min_payload; payload <= last_payload; payload *= 2) {  \
        char row[1024];                                                     \
        char *prow = row;                                                   \
        if (payload < 0) break; /* Overflow */                              \
        sprintf(prow, "%-8i", payload); prow += strlen(prow);               \
        if (!multisender) { printf("%s",row); fflush(stdout); prow = row; } \
        depth = 1;                                                          \
        if (iamsender) { /* Prime i-cache, free-lists, firehose, etc. */    \
          GASNETI_UNUSED /* 'i' not used in all expansions of QUEUE_TEST */ \
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
              /* '{min,max}us' not used in all expansions of QUEUE_TEST */  \
              GASNETI_UNUSED                                                \
              double minus = gasnett_ticks_to_ns(min) / 1000.0 / (double)depth; \
              GASNETI_UNUSED                                                \
              double maxus = gasnett_ticks_to_ns(max) / 1000.0 / (double)depth; \
              int prec;                                                     \
              if (avgus < 1000.0) prec = 3;                                 \
              else if (avgus < 10000.0) prec = 2;                           \
              else if (avgus < 100000.0) prec = 1;                          \
              else prec = 0;                                                \
              sprintf(prow, " %7.*f", prec, avgus); prow += strlen(prow);   \
              if (!multisender) {                                           \
                  printf("%s",row); fflush(stdout); prow = row;             \
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
          printf("%s\n", row); fflush(stdout);                              \
        }                                                                   \
      }                                                                     \
    } while (0)

void do_bulkputgets(void) {
    if (do_puts && do_bulk && do_explicit) {
      QUEUE_TEST("gasnet_put_nb_bulk", 
                 handles[i] = gasnet_put_nb_bulk(peerproc, tgtmem, msgbuf, payload), 
                 gasnet_wait_syncnb_all(handles, depth), (void)0, 0);
    }

    if (do_gets && do_bulk && do_explicit) {
      QUEUE_TEST("gasnet_get_nb_bulk", 
                 handles[i] = gasnet_get_nb_bulk(msgbuf, peerproc, tgtmem, payload), 
                 gasnet_wait_syncnb_all(handles, depth), (void)0, 0);
    }

    if (do_puts && do_bulk && do_implicit) {
      QUEUE_TEST("gasnet_put_nbi_bulk", 
                 gasnet_put_nbi_bulk(peerproc, tgtmem, msgbuf, payload), 
                 gasnet_wait_syncnbi_all(), (void)0, 0);
    }

    if (do_gets && do_bulk && do_implicit) {
      QUEUE_TEST("gasnet_get_nbi_bulk", 
                 gasnet_get_nbi_bulk(msgbuf, peerproc, tgtmem, payload), 
                 gasnet_wait_syncnbi_all(), (void)0, 0);
    }
}
void do_nonbulkputgets(void) {
    if (do_puts && do_nonbulk && do_explicit) {
      QUEUE_TEST("gasnet_put_nb", 
                 handles[i] = gasnet_put_nb(peerproc, tgtmem, msgbuf, payload), 
                 gasnet_wait_syncnb_all(handles, depth), (void)0, 0);
    }

    if (do_gets && do_nonbulk && do_explicit) {
      QUEUE_TEST("gasnet_get_nb", 
                 handles[i] = gasnet_get_nb(msgbuf, peerproc, tgtmem, payload), 
                 gasnet_wait_syncnb_all(handles, depth), (void)0, 0);
    }

    if (do_puts && do_nonbulk && do_implicit) {
      QUEUE_TEST("gasnet_put_nbi", 
                 gasnet_put_nbi(peerproc, tgtmem, msgbuf, payload), 
                 gasnet_wait_syncnbi_all(), (void)0, 0);
    }

    if (do_gets && do_nonbulk && do_implicit) {
      QUEUE_TEST("gasnet_get_nbi", 
                 gasnet_get_nbi(msgbuf, peerproc, tgtmem, payload), 
                 gasnet_wait_syncnbi_all(), (void)0, 0);
    }
}
void do_blockingputgets(void) {
    if (do_puts && do_nonbulk && do_blocking) {
      QUEUE_TEST("gasnet_put (BLOCKING - represents round-trip latency)", 
                 gasnet_put(peerproc, tgtmem, msgbuf, payload), 
                 (void)0, (void)0, 0);
    }

    if (do_gets && do_nonbulk && do_blocking) {
      QUEUE_TEST("gasnet_get (BLOCKING - represents round-trip latency)", 
                 gasnet_get(msgbuf, peerproc, tgtmem, payload), 
                 (void)0, (void)0, 0);
    }

    if (do_puts && do_bulk && do_blocking) {
      QUEUE_TEST("gasnet_put_bulk (BLOCKING - represents round-trip latency)", 
                 gasnet_put_bulk(peerproc, tgtmem, msgbuf, payload), 
                 (void)0, (void)0, 0);
    }

    if (do_gets && do_bulk && do_blocking) {
      QUEUE_TEST("gasnet_get_bulk (BLOCKING - represents round-trip latency)", 
                 gasnet_get_bulk(msgbuf, peerproc, tgtmem, payload), 
                 (void)0, (void)0, 0);
    }
}
void do_amtests(void) {
    if (do_amshort) {
      gasnett_atomic_set(&amcount, 0, 0);
      QUEUE_TEST("gasnet_AMRequestShort0", 
                 gasnet_AMRequestShort0(peerproc, hidx_ping_shorthandler), (void)0,
                { assert(iamrecver);
                  GASNET_BLOCKUNTIL(gasnett_atomic_read(&amcount,0) == depth); 
                  gasnett_atomic_set(&amcount, 0, 0); }, 
                 min_payload);
    }

    if (do_ammedium) {
      gasnett_atomic_set(&amcount, 0, 0);
      QUEUE_TEST("gasnet_AMRequestMedium0", 
                 gasnet_AMRequestMedium0(peerproc, hidx_ping_medhandler, msgbuf, payload), (void)0,
                { assert(iamrecver);
                  GASNET_BLOCKUNTIL(gasnett_atomic_read(&amcount,0) == depth); 
                  gasnett_atomic_set(&amcount, 0, 0); }, 
                 gasnet_AMMaxMedium());
    }

    if (do_amlong) {
      gasnett_atomic_set(&amcount, 0, 0);
      QUEUE_TEST("gasnet_AMRequestLong0", 
                 gasnet_AMRequestLong0(peerproc, hidx_ping_medhandler, msgbuf, payload, tgtmem), (void)0,
                { assert(iamrecver);
                  GASNET_BLOCKUNTIL(gasnett_atomic_read(&amcount,0) == depth); 
                  gasnett_atomic_set(&amcount, 0, 0); }, 
                 gasnet_AMMaxLongRequest());
    }
}



/* ------------------------------------------------------------------------------------ */
