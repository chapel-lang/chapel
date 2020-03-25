/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testalign.c $
 * Description: GASNet get/put alignment-sensitivity test
 *   measures flood throughput of GASNet gets and puts
 *   over varying payload alignments and fixed payload size
 * Copyright 2002-2004, Jaein Jeong and Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

int size = 0;
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR (size + PAGESZ)
#endif
#include "test.h"

#define DEFAULT_SZ	(32*1024)

#define PRINT_LATENCY 0
#define PRINT_THROUGHPUT 1

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

typedef struct {
	int datasize;
	int alignment;
	int iters;
	uint64_t time;
} stat_struct_t;

int insegment = 1;
int dogets = 1;
int doputs = 1;

int myproc;
int numprocs;
int peerproc;
int iamsender = 0;
int unitsMB = 0;

char *rembuf;
char *locbuf;

#define init_stat \
    GASNETT_TRACE_SETSOURCELINE(__FILE__,__LINE__), _init_stat
#define update_stat \
    GASNETT_TRACE_SETSOURCELINE(__FILE__,__LINE__), _update_stat
#define print_stat \
    GASNETT_TRACE_SETSOURCELINE(__FILE__,__LINE__), _print_stat

void _init_stat(stat_struct_t *st, int sz, int al)
{
	st->iters = 0;
	st->alignment = al;
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
		printf("%c: Proc %2i - %5i byte %5i byte aligned : %7i iters,"
			   " latency %10i us total, %9.3f us ave. (%s)\n",
			TEST_SECTION_NAME(),
			myproc, st->datasize, st->alignment, st->iters, (int) st->time,
			((float)st->time) / st->iters,
			name);
		fflush(stdout);
		break;
	case PRINT_THROUGHPUT:
		printf((unitsMB?"%c: Proc %2i - %5i byte %5i byte aligned : %7i iters, throughput %11.6f MB/sec (%s)\n":
                                "%c: Proc %2i - %5i byte %5i byte aligned : %7i iters, throughput %11.3f KB/sec (%s)\n"),
			TEST_SECTION_NAME(),
			myproc, st->datasize, st->alignment, st->iters,
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

void oneway_test(int iters, int nbytes, int alignment)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;
    int pad = (alignment % PAGESZ);

    if (doputs) {
	/* initialize statistics */
	init_stat(&st, nbytes, alignment);
	
	memset(locbuf+pad, 1, nbytes);

	BARRIER();
	
	if (iamsender) {
		/* measure the throughput of bulk put */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gex_RMA_PutBlocking(myteam, peerproc, rembuf, locbuf+pad, nbytes, 0);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender) {
		print_stat(myproc, &st, "PutBlocking throughput", PRINT_THROUGHPUT);
	}	
    }

    if (dogets) {
	/* initialize statistics */
	init_stat(&st, nbytes, alignment);

	if (iamsender) {
		/* measure the throughput of bulk get */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gex_RMA_GetBlocking(myteam, locbuf, peerproc, rembuf+pad, nbytes, 0);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender) {
		print_stat(myproc, &st, "GetBlocking throughput", PRINT_THROUGHPUT);
	}	
    }
}

void oneway_nbi_test(int iters, int nbytes, int alignment)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;
    int pad = (alignment % PAGESZ);

    if (doputs) {
	/* initialize statistics */
	init_stat(&st, nbytes, alignment);
	
	memset(locbuf+pad, 1, nbytes);

	BARRIER();
	
	if (iamsender) {
		/* measure the throughput of nonblocking implicit bulk put */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gex_RMA_PutNBI(myteam, peerproc, rembuf, locbuf+pad, nbytes, GEX_EVENT_DEFER, 0);
		}
		gex_NBI_Wait(GEX_EC_PUT,0);
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender) {
		print_stat(myproc, &st, "PutNBI+DEFER throughput", PRINT_THROUGHPUT);
	}	
    }

    if (dogets) {
	/* initialize statistics */
	init_stat(&st, nbytes, alignment);

	if (iamsender) {
		/* measure the throughput of nonblocking implicit get */
		begin = TIME();
		for (i = 0; i < iters; i++) {
	 		gex_RMA_GetNBI(myteam, locbuf, peerproc, rembuf+pad, nbytes, 0);
		}
		gex_NBI_Wait(GEX_EC_GET,0);
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender) {
		print_stat(myproc, &st, "GetNBI throughput", PRINT_THROUGHPUT);
	}	
    }
}

void oneway_nb_test(int iters, int nbytes, int alignment)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;
    gex_Event_t *events = (gex_Event_t*) test_malloc(sizeof(gex_Event_t) * iters);
	
    int pad = (alignment % PAGESZ);

    if (doputs) {
	/* initialize statistics */
	init_stat(&st, nbytes, alignment);
	
	memset(locbuf+pad, 1, nbytes);

	BARRIER();
	
	if (iamsender) {
		/* measure the throughput of sending a message */
		begin = TIME();
                for (i = 0; i < iters; i++) {
                        events[i] = gex_RMA_PutNB(myteam, peerproc, rembuf, locbuf+pad, nbytes, GEX_EVENT_DEFER, 0);
                }
		gex_Event_WaitAll(events, iters, 0);
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender) {
		print_stat(myproc, &st, "PutNB+DEFER throughput", PRINT_THROUGHPUT);
	}	
    }
	
    if (dogets) {
	/* initialize statistics */
	init_stat(&st, nbytes, alignment);

	if (iamsender) {
		/* measure the throughput of receiving a message */
		begin = TIME();
                for (i = 0; i < iters; i++) {
                    events[i] = gex_RMA_GetNB(myteam, locbuf, peerproc, rembuf+pad, nbytes, 0);
                } 
		gex_Event_WaitAll(events, iters, 0);
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender) {
		print_stat(myproc, &st, "GetNB throughput", PRINT_THROUGHPUT);
	}	
    }
	
    test_free(events);
}

int main(int argc, char **argv)
{
    int arg;
    int iters = 0;
    int j;
    int help = 0;   
    int crossmachinemode = 0;   

    /* call startup */
    GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testalign", &argc, &argv, 0));
    
    /* parse arguments */
    arg = 1;
    while (argc > arg) {
      if (!strcmp(argv[arg], "-in")) {
        insegment = 1;
        ++arg;
      } else if (!strcmp(argv[arg], "-out")) {
        insegment = 0;
        ++arg;
      } else if (!strcmp(argv[arg], "-g")) {
        dogets = 1; doputs = 0;
        ++arg;
      } else if (!strcmp(argv[arg], "-p")) {
        doputs = 1; dogets = 0;
        ++arg;
      } else if (!strcmp(argv[arg], "-c")) {
        crossmachinemode = 1;
        ++arg;
      } else if (!strcmp(argv[arg], "-m")) {
        unitsMB = 1;
        ++arg;
      } else if (argv[arg][0] == '-') {
        help = 1;
        ++arg;
      } else break;
    }

    if (argc > arg) iters = atoi(argv[arg++]);
    if (!iters) iters = 1000;

    if (argc > arg) size = atoi(argv[arg++]);
    if (!size) size = DEFAULT_SZ;

    if (argc > arg) { TEST_SECTION_PARSE(argv[arg]); arg++; }
 
    GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
    test_init("testalign", 1,
               "[options] (iters) (size) (test_sections)\n"
               "  The '-in' or '-out' option selects whether the initiator-side\n"
               "   memory is in the GASNet segment or not (default is 'in').\n"
               "  The -p/-g option selects puts only or gets only (default is both).\n"
               "  The -c option enables cross-machine pairing, default is nearest neighbor.\n"
               "  The -m option enables MB/sec units for bandwidth output (MB=2^20 bytes).");
    if (help || argc > arg) test_usage();

    /* get SPMD info */
    myproc = gex_TM_QueryRank(myteam);
    numprocs = gex_TM_QuerySize(myteam);
    
    /* Only allow even number for numprocs */
    if (numprocs % 2 != 0) {
      MSG0("WARNING: This test requires an even number of nodes. Test skipped.\n");
      gasnet_exit(0); /* exit 0 to prevent false negatives in test harnesses for smp-conduit */
    }

    /* Setting peer thread rank */
    if (crossmachinemode) {
      gex_Rank_t half =  numprocs / 2;
      iamsender = (myproc < half);
      peerproc = myproc + (iamsender ? half : -half);
    } else {
      peerproc = (myproc + 1) % numprocs;
      iamsender = !(myproc % 2);
    }
    
    rembuf = (void *) TEST_SEG(peerproc);

    MSG0("Running %i iterations of %sbulk %s%s%s with local addresses %sside the segment for size %i\n",
          iters,
          (crossmachinemode ? "cross-machine " : ""),
          (doputs             ? "put" : ""),
          ((doputs && dogets) ? "/"   : ""),
          (dogets             ? "get ": ""),
          insegment ? "in" : "out",
          size);

    /* initialize global data in my thread */
    if (insegment) {
    	locbuf = (void *)TEST_MYSEG();
    } else {
	/* size + 1 page of alignment + initial alignment padding of PAGESZ-1 */
	uintptr_t tmp = (uintptr_t) test_malloc(size + 2 * PAGESZ - 1);
	locbuf = (void *)((tmp + PAGESZ - 1) & ~(PAGESZ - 1));
    }

    BARRIER();

    if (TEST_SECTION_BEGIN_ENABLED())
      for (j = 1; j <= PAGESZ; j *= 2) oneway_test(iters, size, j);

    if (TEST_SECTION_BEGIN_ENABLED())
      for (j = 1; j <= PAGESZ; j *= 2) oneway_nbi_test(iters, size, j);

    if (TEST_SECTION_BEGIN_ENABLED())
      for (j = 1; j <= PAGESZ; j *= 2) oneway_nb_test(iters, size, j);

    BARRIER();
    gasnet_exit(0);

    return 0;

}
/* ------------------------------------------------------------------------------------ */
