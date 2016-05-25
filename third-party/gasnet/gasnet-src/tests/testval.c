/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testval.c $
 * Description: GASNet value get/put performance test
 *   measures the ping-pong average round-trip time and
 *   average flood throughput of GASNet gets and puts
 *   over varying payload size and synchronization mechanisms
 * Copyright 2002, Jaein Jeong and Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include "gasnet.h"

#if PLATFORM_ARCH_64
int maxsz = 8;
#else
int maxsz = 4;
#endif

int iters = 0;

#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR (maxsz*iters)
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

gasnet_handlerentry_t handler_table[2];


int myproc;
int numprocs;
int peerproc = -1;
int iamsender = 0;
int unitsMB = 0;
int doputs = 1;
int dogets = 1;

uint8_t *tgtmem;

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


void roundtrip_test(int nbytes)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;
    gasnet_register_value_t reg = 1;

	/* initialize statistics */
	init_stat(&st, nbytes);
	
	BARRIER();
	
	if (iamsender && doputs) {
		/* measure the round-trip time of put */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			unsigned int offset = i * nbytes;
			gasnet_put_val(peerproc, tgtmem+offset, (gasnet_register_value_t)i, nbytes);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && doputs) {
		print_stat(myproc, &st, "put_val latency", PRINT_LATENCY);
	}	

	/* initialize statistics */
	init_stat(&st, nbytes);

	if (iamsender && dogets) {
		/* measure the round-trip time of get */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			unsigned int offset = i * nbytes;
	 		reg ^= gasnet_get_val(peerproc, tgtmem+offset, nbytes);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && dogets) {
		print_stat(myproc, &st, "get_val latency", PRINT_LATENCY);
	}	
}

void oneway_test(int nbytes)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;
    gasnet_register_value_t reg = 1;

	/* initialize statistics */
	init_stat(&st, nbytes);
	
	BARRIER();
	
	if (iamsender && doputs) {
		/* measure the throughput of put */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			unsigned int offset = i * nbytes;
			gasnet_put_val(peerproc, tgtmem+offset, (gasnet_register_value_t)i, nbytes);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && doputs) {
		print_stat(myproc, &st, "put_val throughput", PRINT_THROUGHPUT);
	}	

	/* initialize statistics */
	init_stat(&st, nbytes);

	if (iamsender && dogets) {
		/* measure the throughput of get */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			unsigned int offset = i * nbytes;
	 		reg ^= gasnet_get_val(peerproc, tgtmem+offset, nbytes);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && dogets) {
		print_stat(myproc, &st, "get_val throughput", PRINT_THROUGHPUT);
	}	
}


void roundtrip_nb_test(int nbytes)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;
    gasnet_handle_t hdlput;
    gasnet_valget_handle_t hdlget;
    gasnet_register_value_t reg = 1;

	/* initialize statistics */
	init_stat(&st, nbytes);
	
	BARRIER();
	
	if (iamsender && doputs) {
		/* measure the round-trip time of nonblocking put */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			unsigned int offset = i * nbytes;
			hdlput = gasnet_put_nb_val(peerproc, tgtmem+offset, (gasnet_register_value_t)i, nbytes);
			gasnet_wait_syncnb(hdlput);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && doputs) {
		print_stat(myproc, &st, "put_nb_val latency", PRINT_LATENCY);
	}	

	/* initialize statistics */
	init_stat(&st, nbytes);

	if (iamsender && dogets) {
		/* measure the round-trip time of nonblocking get */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			unsigned int offset = i * nbytes;
	 		hdlget = gasnet_get_nb_val(peerproc, tgtmem+offset, nbytes);
			reg ^= gasnet_wait_syncnb_valget(hdlget);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && dogets) {
		print_stat(myproc, &st, "get_nb_val latency", PRINT_LATENCY);
	}	

}

void oneway_nb_test(int nbytes)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;
    gasnet_handle_t *phandles;
    gasnet_valget_handle_t *ghandles;
    gasnet_register_value_t reg = 1;

	/* initialize statistics */
	init_stat(&st, nbytes);
	
	phandles = (gasnet_handle_t*) test_malloc(sizeof(gasnet_handle_t) * iters);
	ghandles = (gasnet_valget_handle_t*) test_malloc(sizeof(gasnet_valget_handle_t) * iters);
	
	BARRIER();
	
	if (iamsender && doputs) {
		/* measure the throughput of sending a message */
		begin = TIME();
                for (i = 0; i < iters; i++) {
			unsigned int offset = i * nbytes;
                        phandles[i] = gasnet_put_nb_val(peerproc, tgtmem+offset, (gasnet_register_value_t)i, nbytes);
                }
		gasnet_wait_syncnb_all(phandles, iters); 
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && doputs) {
		print_stat(myproc, &st, "put_nb_val throughput", PRINT_THROUGHPUT);
	}	
	
	/* initialize statistics */
	init_stat(&st, nbytes);

	if (iamsender && dogets) {
		/* measure the throughput of receiving a message */
		begin = TIME();
                for (i = 0; i < iters; i++) {
			unsigned int offset = i * nbytes;
                    ghandles[i] = gasnet_get_nb_val(peerproc, tgtmem+offset, nbytes);
                } 
                for (i = 0; i < iters; i++) {
		    reg ^= gasnet_wait_syncnb_valget(ghandles[i]);
                }
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && dogets) {
		print_stat(myproc, &st, "get_nb_val throughput", PRINT_THROUGHPUT);
	}	
	
	test_free(phandles);
	test_free(ghandles);
}


void roundtrip_nbi_test(int nbytes)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;

	/* initialize statistics */
	init_stat(&st, nbytes);
	
	BARRIER();
	
	if (iamsender && doputs) {
		/* measure the round-trip time of nonblocking implicit put */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			unsigned int offset = i * nbytes;
			gasnet_put_nbi_val(peerproc, tgtmem+offset, (gasnet_register_value_t)i, nbytes);

			gasnet_wait_syncnbi_puts();
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && doputs) {
		print_stat(myproc, &st, "put_nbi_val latency", PRINT_LATENCY);
	}	
}

void oneway_nbi_test(int nbytes)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    stat_struct_t st;

	/* initialize statistics */
	init_stat(&st, nbytes);
	
	BARRIER();
	
	if (iamsender && doputs) {
		/* measure the throughput of nonblocking implicit put */
		begin = TIME();
		for (i = 0; i < iters; i++) {
			unsigned int offset = i * nbytes;
			gasnet_put_nbi_val(peerproc, tgtmem+offset, (gasnet_register_value_t)i, nbytes);
		}
		gasnet_wait_syncnbi_puts();
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
	}
	
	BARRIER();
	
	if (iamsender && doputs) {
		print_stat(myproc, &st, "put_nbi_val throughput", PRINT_THROUGHPUT);
	}	
}

int main(int argc, char **argv)
{
    int min_payload, max_payload;
    void *myseg;
    void *alloc;
    int arg;
    int j;
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
      if (!strcmp(argv[arg], "-f")) {
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
    if (!iters) iters = 10000;
    if (argc > arg) { maxsz = atoi(argv[arg]); arg++; }
    if (!maxsz || maxsz > sizeof(gasnet_register_value_t)) maxsz = sizeof(gasnet_register_value_t);
    if (argc > arg) { TEST_SECTION_PARSE(argv[arg]); arg++; }

    GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
    test_init("testval",1, "[options] (iters) (maxsz) (test_sections)\n"
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

    /* get SPMD info */
    myproc = gasnet_mynode();
    numprocs = gasnet_nodes();

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
    tgtmem = TEST_SEG(peerproc);

        MSG0("Running %i iterations of %s%s%snon-bulk value put/get for sizes: %i...%i\n", 
          iters, 
          (firstlastmode ? "first/last " : ""),
          (fullduplexmode ? "full-duplex ": ""),
          (crossmachinemode ? "cross-machine ": ""),
          min_payload, max_payload);
        BARRIER();

        if (iamsender && !skipwarmup) { /* pay some warm-up costs */
           int i;
           int warm_iters = MIN(iters, 32767);  /* avoid hitting 65535-handle limit */
           gasnet_handle_t *ph = test_malloc(sizeof(gasnet_handle_t)*warm_iters);
           gasnet_valget_handle_t *gh = test_malloc(sizeof(gasnet_valget_handle_t)*warm_iters);
           gasnet_register_value_t reg = 1;
           for (i = 0; i < warm_iters; i++) {
              gasnet_put_val(peerproc, tgtmem, reg, max_payload);
              reg ^= gasnet_get_val(peerproc, tgtmem, max_payload);
              ph[i] = gasnet_put_nb_val(peerproc, tgtmem, reg, max_payload);
              gh[i] = gasnet_get_nb_val(peerproc, tgtmem, max_payload);
              gasnet_put_nbi_val(peerproc, tgtmem, reg, max_payload);
           }
           gasnet_wait_syncnbi_puts();
           gasnet_wait_syncnb_all(ph, warm_iters);
           test_free(ph);
           for (i = 0; i < warm_iters; i++) {
              reg ^= gasnet_wait_syncnb_valget(gh[i]);
           }
           test_free(gh);
        }

        BARRIER();

	if (TEST_SECTION_BEGIN_ENABLED()) 
        for (j = min_payload; j <= max_payload && j > 0; j *= 2)  roundtrip_test(j); 

  	if (TEST_SECTION_BEGIN_ENABLED()) 
        for (j = min_payload; j <= max_payload && j > 0; j *= 2)  oneway_test(j);

  	if (TEST_SECTION_BEGIN_ENABLED()) 
  	for (j = min_payload; j <= max_payload && j > 0; j *= 2)  roundtrip_nb_test(j);

  	if (TEST_SECTION_BEGIN_ENABLED()) 
  	for (j = min_payload; j <= max_payload && j > 0; j *= 2)  oneway_nb_test(j);

  	if (TEST_SECTION_BEGIN_ENABLED()) 
  	for (j = min_payload; j <= max_payload && j > 0; j *= 2)  roundtrip_nbi_test(j);

  	if (TEST_SECTION_BEGIN_ENABLED()) 
  	for (j = min_payload; j <= max_payload && j > 0; j *= 2)  oneway_nbi_test(j);

        BARRIER();

    gasnet_exit(0);

    return 0;

}
/* ------------------------------------------------------------------------------------ */
