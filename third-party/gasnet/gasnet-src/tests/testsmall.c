/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testsmall.c $
 * Description: GASNet non-bulk get/put performance test
 *   measures the ping-pong average round-trip time and
 *   average flood throughput of GASNet gets and puts
 *   over varying payload size and synchronization mechanisms
 * Copyright 2002, Jaein Jeong and Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

int numprocs;
int maxsz = 0;
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR (2*(uintptr_t)alignup(maxsz,PAGESZ))
#endif
#include "test.h"

#define GASNET_HEADNODE 0
#define PRINT_LATENCY 0
#define PRINT_THROUGHPUT 1

typedef struct {
	int datasize;
	int iters;
	uint64_t time;
} stat_struct_t;

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

gex_AM_Entry_t handler_table[2];

int insegment = 1;

int myproc;
int peerproc = -1;
int iamsender = 0;
int unitsMB = 0;
int doputs = 1;
int dogets = 1;

void *tgtmem;
char *msgbuf;

#define init_stat \
  GASNETT_TRACE_SETSOURCELINE(__FILE__,__LINE__), _init_stat
#define update_stat \
  GASNETT_TRACE_SETSOURCELINE(__FILE__,__LINE__), _update_stat
#define print_stat \
  GASNETT_TRACE_SETSOURCELINE(__FILE__,__LINE__), _print_stat

void _init_stat(stat_struct_t *st, int sz)
{
	st->iters = 0;
	st->datasize = sz;
	st->time = 0;
}

void _update_stat(stat_struct_t *st, uint64_t temptime, int iters)
{
	st->iters += iters;
	st->time += temptime;
} 

void _print_stat(int myproc, stat_struct_t *st, const char *name, int operation)
{
	switch (operation) {
	case PRINT_LATENCY:
		printf("%c: %2i - %10i byte : %7i iters,"
			   " latency %10i us total, %9.3f us ave. (%s)\n",
                        TEST_SECTION_NAME(),
			myproc, st->datasize, st->iters, (int) st->time,
			((double)st->time) / st->iters,
			name);
		fflush(stdout);
		break;
	case PRINT_THROUGHPUT:
		printf((unitsMB?"%c: %2i - %10i byte : %7i iters, throughput %11.6f MB/sec (%s)\n":
                                "%c: %2i - %10i byte : %7i iters, throughput %11.3f KB/sec (%s)\n"),
                        TEST_SECTION_NAME(),
			myproc, st->datasize, st->iters,
			((int)st->time == 0 ? 0.0 :
                        (1000000.0 * st->datasize * st->iters / 
                          (unitsMB?(1024.0*1024.0):1024.0)) / ((int)st->time)),
			name);
		fflush(stdout);
		break;
	default:
		break;
	}
}


void roundtrip_test(int iters, int nbytes)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;

	/* initialize statistics */
	init_stat(&st, nbytes);
	
	memset(msgbuf, 1, nbytes);

	BARRIER();
	
	if (iamsender && doputs) {
		/* measure the round-trip time of put */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gex_RMA_PutBlocking(myteam, peerproc, tgtmem, msgbuf, nbytes, 0);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && doputs) {
		print_stat(myproc, &st, "PutBlocking latency", PRINT_LATENCY);
	}	

	/* initialize statistics */
	init_stat(&st, nbytes);

	if (iamsender && dogets) {
		/* measure the round-trip time of get */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gex_RMA_GetBlocking(myteam, msgbuf, peerproc, tgtmem, nbytes, 0);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && dogets) {
		print_stat(myproc, &st, "GetBlocking latency", PRINT_LATENCY);
	}	
}

void oneway_test(int iters, int nbytes)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;

	/* initialize statistics */
	init_stat(&st, nbytes);
	
	memset(msgbuf, 1, nbytes);

	BARRIER();
	
	if (iamsender && doputs) {
		/* measure the throughput of put */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gex_RMA_PutBlocking(myteam, peerproc, tgtmem, msgbuf, nbytes, 0);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && doputs) {
		print_stat(myproc, &st, "PutBlocking throughput", PRINT_THROUGHPUT);
	}	

	/* initialize statistics */
	init_stat(&st, nbytes);

	if (iamsender && dogets) {
		/* measure the throughput of get */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gex_RMA_GetBlocking(myteam, msgbuf, peerproc, tgtmem, nbytes, 0);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && dogets) {
		print_stat(myproc, &st, "GetBlocking throughput", PRINT_THROUGHPUT);
	}	
}


void roundtrip_nbi_test(int iters, int nbytes)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;

	/* initialize statistics */
	init_stat(&st, nbytes);
	
	memset(msgbuf, 1, nbytes);

	BARRIER();
	
	if (iamsender && doputs) {
		/* measure the round-trip time of nonblocking implicit put */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gex_RMA_PutNBI(myteam, peerproc, tgtmem, msgbuf, nbytes, GEX_EVENT_NOW, 0);
			gex_NBI_Wait(GEX_EC_PUT,0);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && doputs) {
		print_stat(myproc, &st, "PutNBI+NOW latency", PRINT_LATENCY);
	}	


	/* initialize statistics */
	init_stat(&st, nbytes);

	if (iamsender && dogets) {
		/* measure the round-trip time of nonblocking implicit get */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gex_RMA_GetNBI(myteam, msgbuf, peerproc, tgtmem, nbytes, 0);
			gex_NBI_Wait(GEX_EC_GET,0);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && dogets) {
		print_stat(myproc, &st, "GetNBI latency", PRINT_LATENCY);
	}	

}

void oneway_nbi_test(int iters, int nbytes)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;

	/* initialize statistics */
	init_stat(&st, nbytes);
	
	memset(msgbuf, 1, nbytes);

	BARRIER();
	
	if (iamsender && doputs) {
		/* measure the throughput of nonblocking implicit put */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gex_RMA_PutNBI(myteam, peerproc, tgtmem, msgbuf, nbytes, GEX_EVENT_NOW, 0);
		}
		gex_NBI_Wait(GEX_EC_PUT,0);
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && doputs) {
		print_stat(myproc, &st, "PutNBI+NOW throughput", PRINT_THROUGHPUT);
	}	

	/* initialize statistics */
	init_stat(&st, nbytes);

	if (iamsender && dogets) {
		/* measure the throughput of nonblocking implicit get */
		begin = TIME();
		for (i = 0; i < iters; i++) {
	 		gex_RMA_GetNBI(myteam, msgbuf, peerproc, tgtmem, nbytes, 0);
		}
		gex_NBI_Wait(GEX_EC_GET,0);
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && dogets) {
		print_stat(myproc, &st, "GetNBI throughput", PRINT_THROUGHPUT);
	}	
}


void roundtrip_nb_test(int iters, int nbytes)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;
    gex_Event_t hdlget, hdlput;

	/* initialize statistics */
	init_stat(&st, nbytes);
	
	memset(msgbuf, 1, nbytes);

	BARRIER();
	
	if (iamsender && doputs) {
		/* measure the round-trip time of nonblocking put */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			hdlput = gex_RMA_PutNB(myteam, peerproc, tgtmem, msgbuf, nbytes, GEX_EVENT_NOW, 0);
			gex_Event_Wait(hdlput);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && doputs) {
		print_stat(myproc, &st, "PutNB+NOW latency", PRINT_LATENCY);
	}	

	/* initialize statistics */
	init_stat(&st, nbytes);

	if (iamsender && dogets) {
		/* measure the round-trip time of nonblocking get */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			hdlget = gex_RMA_GetNB(myteam, msgbuf, peerproc, tgtmem, nbytes, 0);
			gex_Event_Wait(hdlget);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && dogets) {
		print_stat(myproc, &st, "GetNB latency", PRINT_LATENCY);
	}	

}

void oneway_nb_test(int iters, int nbytes)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;
    /*gex_Event_t hdlget, hdlput;*/
    gex_Event_t *events;

	/* initialize statistics */
	init_stat(&st, nbytes);
	
	events = (gex_Event_t*) test_malloc(sizeof(gex_Event_t) * iters);
	
	memset(msgbuf, 1, nbytes);

	BARRIER();
	
	if (iamsender && doputs) {
		/* measure the throughput of sending a message */
		begin = TIME();
		/*for (i = 0; i < iters; i++) {
			hdlput = gex_RMA_PutNB(myteam, peerproc, tgtmem, msgbuf, nbytes, GEX_EVENT_NOW, 0);
		        gex_Event_Wait(hdlput);
		}*/
                for (i = 0; i < iters; i++) {
                        events[i] = gex_RMA_PutNB(myteam, peerproc, tgtmem, msgbuf, nbytes, GEX_EVENT_NOW, 0);
                }
		gex_Event_WaitAll(events, iters, 0);
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && doputs) {
		print_stat(myproc, &st, "PutNB+NOW throughput", PRINT_THROUGHPUT);
	}	
	
	/* initialize statistics */
	init_stat(&st, nbytes);

	if (iamsender && dogets) {
		/* measure the throughput of receiving a message */
		begin = TIME();
		/*for (i = 0; i < iters; i++) {
		    hdlget = gex_RMA_GetNB(myteam, msgbuf, peerproc, tgtmem, nbytes, 0);
		    gex_Event_Wait(hdlget);
		}*/
                for (i = 0; i < iters; i++) {
                    events[i] = gex_RMA_GetNB(myteam, msgbuf, peerproc, tgtmem, nbytes, 0);
                } 
		gex_Event_WaitAll(events, iters, 0);
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && dogets) {
		print_stat(myproc, &st, "GetNB throughput", PRINT_THROUGHPUT);
	}	
	
	test_free(events);
}

int main(int argc, char **argv)
{
    int min_payload, max_payload;
    void *myseg;
    void *alloc = NULL;
    int arg;
    int iters = 0;
    int j;
    int firstlastmode = 0;
    int fullduplexmode = 0;
    int crossmachinemode = 0;
    int skipwarmup = 0;
    int help = 0;   
   
    /* call startup */
    GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testsmall", &argc, &argv, 0));
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
      } else if (!strcmp(argv[arg], "-c")) {
        crossmachinemode = 1;
        ++arg;
      } else if (!strcmp(argv[arg], "-a")) {
        fullduplexmode = 1;
        ++arg;
      } else if (!strcmp(argv[arg], "-m")) {
        unitsMB = 1;
        ++arg;
      } else if (!strcmp(argv[arg], "-p")) {
        dogets = 0; doputs = 1;
        ++arg;
      } else if (!strcmp(argv[arg], "-g")) {
        dogets = 1; doputs = 0;
        ++arg;
      } else if (!strcmp(argv[arg], "-s")) {
        skipwarmup = 1;
        ++arg;
      } else if (argv[arg][0] == '-') {
        help = 1;
        ++arg;
      } else break;
    }

    if (argc > arg) { iters = atoi(argv[arg]); arg++; }
    if (!iters) iters = 1000;
    if (argc > arg) { maxsz = atoi(argv[arg]); arg++; }
    if (!maxsz) maxsz = 2048; /* 2 KB default */
    if (argc > arg) { TEST_SECTION_PARSE(argv[arg]); arg++; }

    /* get SPMD info (needed for segment size) */
    myproc = gex_TM_QueryRank(myteam);
    numprocs = gex_TM_QuerySize(myteam);

    #ifdef GASNET_SEGMENT_EVERYTHING
      if (maxsz > TEST_SEGSZ/2) { MSG("maxsz must be <= %"PRIuPTR" on GASNET_SEGMENT_EVERYTHING", (uintptr_t)(TEST_SEGSZ/2)); gasnet_exit(1); }
    #endif
    GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
    test_init("testsmall",1, "[options] (iters) (maxsz) (test_sections)\n"
               "  The '-in' or '-out' option selects whether the initiator-side\n"
               "   memory is in the GASNet segment or not (default is 'in').\n"
               "  The -p/-g option selects puts only or gets only (default is both).\n"
               "  The -s option skips warm-up iterations\n"
               "  The -m option enables MB/sec units for bandwidth output (MB=2^20 bytes).\n"
               "  The -a option enables full-duplex mode, where all nodes send.\n"
               "  The -c option enables cross-machine pairing, default is nearest neighbor.\n"
               "  The -f option enables 'first/last' mode, where the first/last\n"
               "   nodes communicate with each other, while all other nodes sit idle.\n");
    if (help || argc > arg) test_usage();

    min_payload = 1;
    max_payload = maxsz;

    if (max_payload < min_payload) {
      ERR("maxsz must be >= %i\n",min_payload);
      test_usage();
    }

    if (!firstlastmode) {
      /* Only allow 1 or even number for numprocs */
      if (numprocs > 1 && numprocs % 2 != 0) {
        MSG0("WARNING: This test requires a unary or even number of nodes. Test skipped.\n");
        gasnet_exit(0); /* exit 0 to prevent false negatives in test harnesses for smp-conduit */
      }
    }
    
    /* Setting peer thread rank */
    if (firstlastmode) {
      peerproc = (myproc == 0 ? numprocs-1 : 0);
      iamsender = (fullduplexmode ? myproc == 0 || myproc == numprocs-1 : myproc == 0);
    }  else if (numprocs == 1) {
      peerproc = 0;
      iamsender = 1;
    } else if (crossmachinemode) {
      if (myproc < numprocs / 2) {
        peerproc = myproc + numprocs/2;
        iamsender = 1;
      } else {
        peerproc = myproc - numprocs/2;
        iamsender = fullduplexmode;
      }
    } else { 
      peerproc = (myproc % 2) ? (myproc - 1) : (myproc + 1);
      iamsender = (fullduplexmode || myproc % 2 == 0);
    }

    
    myseg = TEST_SEG(myproc);
    tgtmem = (void*)(alignup(maxsz,PAGESZ) + (uintptr_t)TEST_SEG(peerproc));

        if (insegment) {
	    msgbuf = (void *) myseg;
        } else {
	    alloc = (void *) test_calloc((maxsz+PAGESZ)*2,1); /* calloc prevents valgrind warnings */
            msgbuf = (void *) alignup(((uintptr_t)alloc), PAGESZ); /* ensure page alignment of base */
        }
        assert(((uintptr_t)msgbuf) % PAGESZ == 0);
        if (myproc == 0) 
          MSG("Running %i iterations of %s%s%snon-bulk put/get with local addresses %sside the segment for sizes: %i...%i\n", 
          iters, 
          (firstlastmode ? "first/last " : ""),
          (fullduplexmode ? "full-duplex ": ""),
          (crossmachinemode ? "cross-machine ": ""),
          insegment ? "in" : "out", 
          min_payload, max_payload);
        BARRIER();

        if (iamsender && !skipwarmup) { /* pay some warm-up costs */
           int i;
           int warm_iters = MIN(iters, 32767);  /* avoid hitting 65535-handle limit */
           gex_Event_t *h = test_malloc(2*sizeof(gex_Event_t)*warm_iters);
           for (i = 0; i < warm_iters; i++) {
              gex_RMA_PutBlocking(myteam, peerproc, tgtmem, msgbuf, 8, 0);
              gex_RMA_GetBlocking(myteam, msgbuf, peerproc, tgtmem, 8, 0);
              gex_RMA_PutNBI(myteam, peerproc, tgtmem, msgbuf, 8, GEX_EVENT_NOW, 0);
              gex_RMA_GetNBI(myteam, msgbuf, peerproc, tgtmem, 8, 0);
              h[i] = gex_RMA_PutNB(myteam, peerproc, tgtmem, msgbuf, 8, GEX_EVENT_NOW, 0);
              h[i+warm_iters] = gex_RMA_GetNB(myteam, msgbuf, peerproc, tgtmem, 8, 0);
           }
           gex_RMA_PutBlocking(myteam, peerproc, tgtmem, msgbuf, max_payload, 0);
           gex_RMA_GetBlocking(myteam, msgbuf, peerproc, tgtmem, max_payload, 0);
           gex_Event_WaitAll(h, 2*warm_iters, 0);
           gex_NBI_Wait(GEX_EC_ALL,0);
           test_free(h);
        }

        BARRIER();

        /* Double payload at each iter, but include max_payload which may not be power-of-2 */
        #define NEXT_SZ(sz) (MIN(sz*2,max_payload)+(sz==max_payload))

	if (TEST_SECTION_BEGIN_ENABLED()) 
        for (j = min_payload; j <= max_payload && j > 0; j = NEXT_SZ(j))  roundtrip_test(iters, j); 

  	if (TEST_SECTION_BEGIN_ENABLED()) 
        for (j = min_payload; j <= max_payload && j > 0; j = NEXT_SZ(j))  oneway_test(iters, j);

  	if (TEST_SECTION_BEGIN_ENABLED()) 
  	for (j = min_payload; j <= max_payload && j > 0; j = NEXT_SZ(j))  roundtrip_nbi_test(iters, j);

  	if (TEST_SECTION_BEGIN_ENABLED()) 
  	for (j = min_payload; j <= max_payload && j > 0; j = NEXT_SZ(j))  oneway_nbi_test(iters, j);

  	if (TEST_SECTION_BEGIN_ENABLED()) 
  	for (j = min_payload; j <= max_payload && j > 0; j = NEXT_SZ(j))  roundtrip_nb_test(iters, j);

  	if (TEST_SECTION_BEGIN_ENABLED()) 
  	for (j = min_payload; j <= max_payload && j > 0; j = NEXT_SZ(j))  oneway_nb_test(iters, j);

        BARRIER();
        if (alloc) test_free(alloc);

    gasnet_exit(0);

    return 0;

}
/* ------------------------------------------------------------------------------------ */
