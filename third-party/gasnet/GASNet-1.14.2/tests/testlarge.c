/*   $Source: /var/local/cvs/gasnet/tests/testlarge.c,v $
 *     $Date: 2010/04/24 03:52:58 $
 * $Revision: 1.47.2.2 $
 * Description: GASNet bulk get/put performance test
 *   measures the ping-pong average round-trip time and
 *   average flood throughput of GASNet bulk gets and puts
 *   over varying payload size and synchronization mechanisms
 * Copyright 2002, Jaein Jeong and Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

/************************************************************
	testlarge.c:
		measures the bandwidth of get and put
		for large messages with payload size (512 .. limit bytes)
		also measures the barrier time.
		
*************************************************************/

int maxsz = 0;
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR ((uintptr_t)maxsz)
#endif
#include "test.h"

#define GASNET_HEADNODE 0
#define PRINT_LATENCY 0
#define PRINT_THROUGHPUT 1

typedef struct {
	int datasize;
	int iters;
	uint64_t time;
	double max_throughput;
} stat_struct_t;

gasnet_handlerentry_t handler_table[2];

int insegment = 0;

int myproc;
int numprocs;
int peerproc = -1;
int iamsender = 0;
int unitsMB = 0;
int doputs = 1;
int dogets = 1;

int min_payload;
int max_payload;

char *tgtmem;
void *msgbuf;

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
		printf("%c: %3i - %10i byte : %7i iters,"
			   " latency %12i us total, %9.3f us ave. (%s)\n",
                        TEST_SECTION_NAME(),
			myproc, st->datasize, st->iters, (int) st->time,
			((double)st->time) / st->iters,
			name);
		fflush(stdout);
		break;
	case PRINT_THROUGHPUT:
		printf((unitsMB ? "%c: %3i - %10i byte : %7i iters, throughput %11.6f MB/sec (%s)\n":
                                  "%c: %3i - %10i byte : %7i iters, throughput %11.3f KB/sec (%s)\n"),
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

void bulk_test(int iters) {GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t stget, stput;
    int payload;
    
	for (payload = min_payload; payload <= max_payload && payload > 0; payload *= 2) {
		init_stat(&stput, payload);

		BARRIER();
	
		if (iamsender && doputs) {
			/* measure the throughput of sending a message */
			begin = TIME();
			for (i = 0; i < iters; i++) {
				gasnet_put_bulk(peerproc, tgtmem, msgbuf, payload);
			}
			end = TIME();
		 	update_stat(&stput, (end - begin), iters);
		}
	
		BARRIER();

		if (iamsender && doputs) {
			print_stat(myproc, &stput, "put_bulk throughput", PRINT_THROUGHPUT);
		}	
	
		init_stat(&stget, payload);

		if (iamsender && dogets) {
			/* measure the throughput of receiving a message */
			begin = TIME();
			for (i = 0; i < iters; i++) {
			    gasnet_get_bulk(msgbuf, peerproc, tgtmem, payload);
			}
			end = TIME();
		 	update_stat(&stget, (end - begin), iters);
		}
	
		BARRIER();

		if (iamsender && dogets) {
			print_stat(myproc, &stget, "get_bulk throughput", PRINT_THROUGHPUT);
		}	

	}

}

void bulk_test_nbi(int iters) {GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t stget, stput;
    int payload;
    
	for (payload = min_payload; payload <= max_payload && payload > 0; payload *= 2) {
		init_stat(&stput, payload);

		BARRIER();
	
		if (iamsender && doputs) {
			/* measure the throughput of sending a message */
			begin = TIME();
			for (i = 0; i < iters; i++) {
				gasnet_put_nbi_bulk(peerproc, tgtmem, msgbuf, payload);
			}
			gasnet_wait_syncnbi_puts();
			end = TIME();
		 	update_stat(&stput, (end - begin), iters);
		}
	
		BARRIER();

		if (iamsender && doputs) {
			print_stat(myproc, &stput, "put_nbi_bulk throughput", PRINT_THROUGHPUT);
		}	
	
		init_stat(&stget, payload);

		if (iamsender && dogets) {
			/* measure the throughput of receiving a message */
			begin = TIME();
			for (i = 0; i < iters; i++) {
			    gasnet_get_nbi_bulk(msgbuf, peerproc, tgtmem, payload);
			}
			gasnet_wait_syncnbi_gets();
			end = TIME();
		 	update_stat(&stget, (end - begin), iters);
		}
	
		BARRIER();

		if (iamsender && dogets) {
			print_stat(myproc, &stget, "get_nbi_bulk throughput", PRINT_THROUGHPUT);
		}	

	}

}

void bulk_test_nb(int iters) {GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t stget, stput;
    gasnet_handle_t *handles;
    int payload;
    
	handles = (gasnet_handle_t *) test_malloc(sizeof(gasnet_handle_t) * iters);

	for (payload = min_payload; payload <= max_payload && payload > 0; payload *= 2) {
		init_stat(&stput, payload);

		BARRIER();
	
		if (iamsender && doputs) {
			/* measure the throughput of sending a message */
			begin = TIME();
			for (i = 0; i < iters; i++) {
				handles[i] = gasnet_put_nb_bulk(peerproc, tgtmem, msgbuf, payload);
			}
			gasnet_wait_syncnb_all(handles, iters);
			end = TIME();
		 	update_stat(&stput, (end - begin), iters);
		}
	
		BARRIER();
       
		if (iamsender && doputs) {
			print_stat(myproc, &stput, "put_nb_bulk throughput", PRINT_THROUGHPUT);
		}	
	
		init_stat(&stget, payload);

		if (iamsender && dogets) {
			/* measure the throughput of receiving a message */
			begin = TIME();
			for (i = 0; i < iters; i++) {
			    handles[i] = gasnet_get_nb_bulk(msgbuf, peerproc, tgtmem, payload);
			}
			gasnet_wait_syncnb_all(handles, iters);
			end = TIME();
		 	update_stat(&stget, (end - begin), iters);
		}
	
		BARRIER();

		if (iamsender && dogets) {
			print_stat(myproc, &stget, "get_nb_bulk throughput", PRINT_THROUGHPUT);
		}	

	}

	test_free(handles);
}


int main(int argc, char **argv)
{
    int iters = 0;
    int arg;
    void *myseg;
    void *alloc;
    int firstlastmode = 0;
    int fullduplexmode = 0;
    int crossmachinemode = 0;
    int skipwarmup = 0;
    int help = 0;   

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
    if (!maxsz) maxsz = 2*1024*1024; /* 2 MB default */
    if (argc > arg) { TEST_SECTION_PARSE(argv[arg]); arg++; }

    #ifdef GASNET_SEGMENT_EVERYTHING
      if (maxsz > TEST_SEGSZ) { ERR("maxsz must be <= %lu on GASNET_SEGMENT_EVERYTHING",(unsigned long)TEST_SEGSZ); gasnet_exit(1); }
    #endif
    GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
    test_init("testlarge",1, "[options] (iters) (maxsz) (test_sections)\n"
               "  The '-in' or '-out' option selects whether the initiator-side\n"
               "   memory is in the GASNet segment or not (default is not).\n"
               "  The -p/-g option selects puts only or gets only (default is both).\n"
               "  The -s option skips warm-up iterations\n"
               "  The -m option enables MB/sec units for bandwidth output (MB=2^20 bytes).\n"
               "  The -a option enables full-duplex mode, where all nodes send.\n"
               "  The -c option enables cross-machine pairing, default is nearest neighbor.\n"
               "  The -f option enables 'first/last' mode, where the first/last\n"
               "   nodes communicate with each other, while all other nodes sit idle.");
    if (help || argc > arg) test_usage();
    
    min_payload = 16;
    max_payload = maxsz;

    if (max_payload < min_payload) {
      ERR("maxsz must be >= %i\n",min_payload);
      test_usage();
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
      peerproc = (myproc == 0 ? numprocs-1 : 0);
      iamsender = (fullduplexmode ? myproc == 0 || myproc == numprocs-1 : myproc == 0);
    } else if (numprocs == 1) {
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
    tgtmem = TEST_SEG(peerproc);

        if (insegment) {
	    msgbuf = (void *) myseg;
        } else {
	    alloc = (void *) test_calloc(maxsz+PAGESZ,1); /* calloc prevents valgrind warnings */
            msgbuf = (void *) alignup(((uintptr_t)alloc), PAGESZ); /* ensure page alignment of base */
        }
        assert(((uintptr_t)msgbuf) % PAGESZ == 0);

        if (myproc == 0) 
          MSG("Running %i iterations of %s%s%sbulk put/get with local addresses %sside the segment for sizes: %i...%i\n", 
          iters, 
          (firstlastmode ? "first/last " : ""),
          (fullduplexmode ? "full-duplex ": ""),
          (crossmachinemode ? "cross-machine ": ""),
          insegment ? "in" : "out", 
          min_payload, max_payload);
        BARRIER();

        if (iamsender && !skipwarmup) { /* pay some warm-up costs */
           int i;
           int warm_iters = MIN(iters, 32767);	/* avoid hitting 65535-handle limit */
           gasnet_handle_t *h = test_malloc(2*sizeof(gasnet_handle_t)*warm_iters);
           for (i = 0; i < warm_iters; i++) {
              gasnet_put_bulk(peerproc, tgtmem, msgbuf, 8);
              gasnet_get_bulk(msgbuf, peerproc, tgtmem, 8);
              gasnet_put_nbi_bulk(peerproc, tgtmem, msgbuf, 8);
              gasnet_get_nbi_bulk(msgbuf, peerproc, tgtmem, 8);
              h[i] = gasnet_put_nb_bulk(peerproc, tgtmem, msgbuf, 8);
              h[i+warm_iters] = gasnet_get_nb_bulk(msgbuf, peerproc, tgtmem, 8);
           }
           gasnet_put_bulk(peerproc, tgtmem, msgbuf, max_payload);
           gasnet_get_bulk(msgbuf, peerproc, tgtmem, max_payload);
           gasnet_wait_syncnb_all(h, warm_iters*2);
           gasnet_wait_syncnbi_all();
           test_free(h);
        }

        BARRIER();

	if (TEST_SECTION_BEGIN_ENABLED()) bulk_test(iters);
	if (TEST_SECTION_BEGIN_ENABLED()) bulk_test_nbi(iters);
	if (TEST_SECTION_BEGIN_ENABLED()) bulk_test_nb(iters);

        BARRIER();
        if (!insegment) {
	    test_free(alloc);
	}

    gasnet_exit(0);

    return 0;

}


/* ------------------------------------------------------------------------------------ */
