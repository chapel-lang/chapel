/*   $Source: /var/local/cvs/gasnet/tests/testalign.c,v $
 *     $Date: 2010/04/24 03:52:58 $
 * $Revision: 1.18.42.2 $
 * Description: GASNet get/put alignment-sensitivity test
 *   measures flood throughput of GASNet gets and puts
 *   over varying payload alignments and fixed payload size
 * Copyright 2002-2004, Jaein Jeong and Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include "test.h"

#define DEFAULT_SZ	(32*1024)

#define PRINT_LATENCY 0
#define PRINT_THROUGHPUT 1

typedef struct {
	int datasize;
	int alignment;
	int iters;
	uint64_t time;
} stat_struct_t;

int insegment = 0;

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
		printf("Proc %2i - %5i byte %5i byte aligned : %7i iters,"
			   " latency %10i us total, %9.3f us ave. (%s)\n",
			myproc, st->datasize, st->alignment, st->iters, (int) st->time,
			((float)st->time) / st->iters,
			name);
		fflush(stdout);
		break;
	case PRINT_THROUGHPUT:
		printf((unitsMB?"Proc %2i - %5i byte %5i byte aligned : %7i iters, throughput %11.6f MB/sec (%s)\n":
                                "Proc %2i - %5i byte %5i byte aligned : %7i iters, throughput %11.3f KB/sec (%s)\n"),
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

	/* initialize statistics */
	init_stat(&st, nbytes, alignment);
	
	memset(locbuf+pad, 1, nbytes);

	BARRIER();
	
	if (iamsender) {
		/* measure the throughput of bulk put */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gasnet_put_bulk(peerproc, rembuf, locbuf+pad, nbytes);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender) {
		print_stat(myproc, &st, "put_bulk throughput", PRINT_THROUGHPUT);
	}	

	/* initialize statistics */
	init_stat(&st, nbytes, alignment);

	if (iamsender) {
		/* measure the throughput of bulk get */
		begin = TIME();
		for (i = 0; i < iters; i++) {
	 		gasnet_get_bulk(locbuf, peerproc, rembuf+pad, nbytes);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender) {
		print_stat(myproc, &st, "get_bulk throughput", PRINT_THROUGHPUT);
	}	
}

void oneway_nbi_test(int iters, int nbytes, int alignment)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;
    int pad = (alignment % PAGESZ);

	/* initialize statistics */
	init_stat(&st, nbytes, alignment);
	
	memset(locbuf+pad, 1, nbytes);

	BARRIER();
	
	if (iamsender) {
		/* measure the throughput of nonblocking implicit bulk put */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gasnet_put_nbi_bulk(peerproc, rembuf, locbuf+pad, nbytes);
		}
		gasnet_wait_syncnbi_puts();
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender) {
		print_stat(myproc, &st, "put_nbi_bulk throughput", PRINT_THROUGHPUT);
	}	

	/* initialize statistics */
	init_stat(&st, nbytes, alignment);

	if (iamsender) {
		/* measure the throughput of nonblocking implicit bulk get */
		begin = TIME();
		for (i = 0; i < iters; i++) {
	 		gasnet_get_nbi_bulk(locbuf, peerproc, rembuf+pad, nbytes);
		}
		gasnet_wait_syncnbi_gets();
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender) {
		print_stat(myproc, &st, "get_nbi_bulk throughput", PRINT_THROUGHPUT);
	}	
}

void oneway_nb_test(int iters, int nbytes, int alignment)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;
    gasnet_handle_t *handles;
    int pad = (alignment % PAGESZ);

	/* initialize statistics */
	init_stat(&st, nbytes, alignment);
	
	handles = (gasnet_handle_t*) test_malloc(sizeof(gasnet_handle_t) * iters);
	
	memset(locbuf+pad, 1, nbytes);

	BARRIER();
	
	if (iamsender) {
		/* measure the throughput of sending a message */
		begin = TIME();
                for (i = 0; i < iters; i++) {
                        handles[i] = gasnet_put_nb_bulk(peerproc, rembuf, locbuf+pad, nbytes);
                }
		gasnet_wait_syncnb_all(handles, iters); 
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender) {
		print_stat(myproc, &st, "put_nb_bulk throughput", PRINT_THROUGHPUT);
	}	
	
	/* initialize statistics */
	init_stat(&st, nbytes, alignment);

	if (iamsender) {
		/* measure the throughput of receiving a message */
		begin = TIME();
                for (i = 0; i < iters; i++) {
                    handles[i] = gasnet_get_nb_bulk(locbuf, peerproc, rembuf+pad, nbytes);
                } 
		gasnet_wait_syncnb_all(handles, iters); 
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender) {
		print_stat(myproc, &st, "get_nb_bulk throughput", PRINT_THROUGHPUT);
	}	
	
	test_free(handles);
}

int main(int argc, char **argv)
{
    int arg;
    int iters = 0;
    int size = 0;
    int j;
    int help = 0;   

    /* call startup */
    GASNET_Safe(gasnet_init(&argc, &argv));
    GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
    test_init("testalign", 1,
               "[-in|-out] (iters) (size)\n"
               "  The 'in' or 'out' option selects whether the initiator-side\n"
               "  memory is in the GASNet segment or not (default is not).\n"
               "  The -m option enables MB/sec units for bandwidth output (MB=2^20 bytes).");
    
    /* parse arguments */
    arg = 1;
    while (argc > arg) {
      if (!strcmp(argv[arg], "-in")) {
        insegment = 1;
        ++arg;
      } else if (!strcmp(argv[arg], "-out")) {
        insegment = 0;
        ++arg;
      } else if (!strcmp(argv[arg], "-m")) {
        unitsMB = 1;
        ++arg;
      } else if (argv[arg][0] == '-') {
        help = 1;
        ++arg;
      } else break;
    }
    if (help || argc > arg+2) test_usage();

    if (argc > arg) iters = atoi(argv[arg++]);
    if (!iters) iters = 1000;

    if (argc > arg) size = atoi(argv[arg++]);
    if (!size) size = DEFAULT_SZ;

    /* get SPMD info */
    myproc = gasnet_mynode();
    numprocs = gasnet_nodes();
    
    /* Only allow even number for numprocs */
    if (numprocs % 2 != 0) {
      MSG("WARNING: This test requires an even number of nodes. Test skipped.\n");
      gasnet_exit(0); /* exit 0 to prevent false negatives in test harnesses for smp-conduit */
    }

    /* Setting peer thread rank */
    peerproc = (myproc % 2) ? (myproc - 1) : (myproc + 1);
    iamsender = (myproc % 2 == 0);
    
    rembuf = (void *) TEST_SEG(peerproc);


    /* initialize global data in my thread */
    if (insegment) {
    	locbuf = (void *)TEST_MYSEG();
    } else {
	/* size + 1 page of alignment + initial alignment padding of PAGESZ-1 */
	uintptr_t tmp = (uintptr_t) test_malloc(size + 2 * PAGESZ - 1);
	locbuf = (void *)((tmp + PAGESZ - 1) & ~(PAGESZ - 1));
    }

      for (j = 1; j <= PAGESZ; j *= 2) oneway_test(iters, size, j);
      for (j = 1; j <= PAGESZ; j *= 2) oneway_nbi_test(iters, size, j);
      for (j = 1; j <= PAGESZ; j *= 2) oneway_nb_test(iters, size, j);

    BARRIER();
    gasnet_exit(0);

    return 0;

}
/* ------------------------------------------------------------------------------------ */
