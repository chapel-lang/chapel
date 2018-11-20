/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testrand.c $
 * Description: GASNet get/put performance test
 *   measures measures the total time to write to each page of the
 *   remote test segment, using blocking puts in a random order.
 *   This is meant to measure the cost of dynamic pinning.
 * Copyright 2002-4, Jaein Jeong and Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

/************************************************************
	testrand.c:
		measures the cost of randomly ordered writes
		to the remote segment to stress test dynamic
		pinning.
		
*************************************************************/

#include <gasnetex.h>

uintptr_t maxsz = 0;
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR ((uintptr_t)maxsz)
#endif

#include "test.h"

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

int myproc;
int numprocs;
int peerproc;
unsigned int seed = 0;
int nbytes = 8;

void *remmem;
void *locmem;

#undef rem_addr /* AIX 5.3 header bug */

void do_test(void) {GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end;
    int iamsender = (myproc % 2 == 0);
    int pagesz = MAX(PAGESZ, nbytes);
    int pages = maxsz / pagesz;
    void **loc_addr = test_malloc(pages * sizeof(void *));
    void **rem_addr = test_malloc(pages * sizeof(void *));
    
	if (iamsender) {
		/* create in-order arrays of page addresses */
		for (i = 0; i < pages; ++i) {
		    loc_addr[i] = (void *)((uintptr_t)locmem + (i * pagesz));
		    rem_addr[i] = (void *)((uintptr_t)remmem + (i * pagesz));
		}
		/* permute the arrays separately */
		for (i = 0; i < pages - 1; ++i) {
		    int j;
		    void *tmp;
		   
		    j = TEST_RAND(0,pages - 1 - i);
		    tmp = loc_addr[i+j];
		    loc_addr[i+j] = loc_addr[i];
		    loc_addr[i] = tmp;
		   
		    j = TEST_RAND(0,pages - 1 - i);
		    tmp = rem_addr[i+j];
		    rem_addr[i+j] = rem_addr[i];
		    rem_addr[i] = tmp;
		}
	}

	BARRIER();

	if (iamsender) {
		GASNETT_TRACE_SETSOURCELINE(__FILE__,__LINE__);
		begin = TIME();
		for (i = 0; i < pages; ++i) {
		    gex_RMA_PutBlocking(myteam, peerproc, rem_addr[i], loc_addr[i], nbytes, 0);
		}
		end = TIME();
		printf("Proc %3i - %5i bytes, seed %10u, %7i pages: %12i us total, %9.3f us ave. per page\n",
			myproc, nbytes, seed, pages, (int)(end-begin), ((double)(end-begin))/pages);
                fflush(stdout);
                sleep(1);
	}

	BARRIER();
}

int main(int argc, char **argv) {

    /* call startup */
    GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testrand", &argc, &argv, 0));

    /* parse arguments */
    if (argc > 1) nbytes = atoi(argv[1]);
    if (argc > 2) {
      maxsz = atol(argv[2]);
      maxsz = MAX(maxsz, nbytes);
      maxsz = alignup(maxsz, PAGESZ);
    } else maxsz = 1024*1024;
    if (argc > 3) seed = atoi(argv[3]);
    if (!seed) seed = (int)TIME();
    TEST_SRAND(seed);

    #ifdef GASNET_SEGMENT_EVERYTHING
      if (maxsz > TEST_SEGSZ) { MSG("maxsz must be <= %"PRIuPTR" on GASNET_SEGMENT_EVERYTHING",(uintptr_t)TEST_SEGSZ); gasnet_exit(1); }
    #endif
    GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
    test_init("testrand",1, "nbytes (segsz) (seed)");
    if ((argc < 2) || (argc > 4)) test_usage();

    /* get SPMD info */
    myproc = gex_TM_QueryRank(myteam);
    numprocs = gex_TM_QuerySize(myteam);
    
    /* Only allow even number for numprocs */
    if (numprocs % 2 != 0) {
      MSG0("WARNING: This test requires an even number of nodes. Test skipped.\n");
      gasnet_exit(0); /* exit 0 to prevent false negatives in test harnesses for smp-conduit */
    }
    
    /* Setting peer thread rank */
    peerproc = (myproc % 2) ? (myproc - 1) : (myproc + 1);
    
    remmem = (void *) TEST_SEG(peerproc);
    locmem = (void *) TEST_MYSEG();

    do_test();

    gasnet_exit(0);

    return 0;

}


/* ------------------------------------------------------------------------------------ */
