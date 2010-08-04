/*   $Source: /var/local/cvs/gasnet/tests/testlogGP.c,v $
 *     $Date: 2010/04/24 03:52:58 $
 * $Revision: 1.31.10.1 $
 * Description: GASNet logGP tester.
 *   measures the ping-pong average round-trip time and
 *   average flood throughput of GASNet gets and puts
 *   over varying payload size and synchronization mechanisms
 * Copyright 2002, Jaein Jeong and Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#define TEST_DELAY 1
#include "test.h"

#define GASNET_HEADNODE 0

enum {
	PRINT_EEL,
	PRINT_OVERHEAD,
	PRINT_GAP,
	PRINT_BIG_G
};

typedef struct {
	int datasize;
	int iters;
	int64_t time;
} stat_struct_t;

gasnet_handlerentry_t handler_table[2];

int myproc;
int numprocs;
int peerproc = -1;
int iamsender = 0;
int pollcnt = 0;

void *mymem;
void *peermem;

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

void _update_stat(stat_struct_t *st, int64_t temptime, int iters)
{
	st->iters += iters;
	st->time += temptime;
} 

void _print_stat(int myproc, stat_struct_t *st, const char *name, int operation)
{
	switch (operation) {
	case PRINT_EEL:
		printf("Proc %2i - %7i byte : %5i iters,"
			   " %10i us elapsed    = %9.3f us/msg (%s)\n",
			myproc, st->datasize, st->iters, (int) st->time,
			(0.5*(float)st->time) / st->iters,
			name);
		fflush(stdout);
		break;
	case PRINT_OVERHEAD:
		printf("Proc %2i - %7i byte : %5i iters,"
			   " %10i us difference = %9.3f us/msg (%s)\n",
			myproc, st->datasize, st->iters, (int) st->time,
			((float)st->time) / st->iters,
			name);
		fflush(stdout);
		break;
	case PRINT_GAP:
		printf("Proc %2i - %7i byte : %5i iters,"
			   " %10i us elapsed    = %9.3f us/msg (%s)\n",
			myproc, st->datasize, st->iters, (int) st->time,
			((float)st->time) / st->iters,
			name);
		fflush(stdout);
		break;
	case PRINT_BIG_G:
		printf("Proc %2i - %7i byte : %5i iters,"
			   " %10i us elapsed    = %9.3f us/Kb  (%s)\n",
			myproc, st->datasize, st->iters, (int) st->time,
			(1024.0*(float)st->time) / ((float)st->iters * (float)st->datasize),
			name);
		fflush(stdout);
		break;
	default:
		printf("ERROR\n");
		break;
	}
}


void put_tests(int iters, int nbytes)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end, delay_time, loops;
    stat_struct_t st;

	memset(mymem, 0, nbytes);

	BARRIER();
	
	if (iamsender) {
		/* measure the round-trip time of put */
		init_stat(&st, nbytes);
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gasnet_put(peerproc, peermem, mymem, nbytes);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
		print_stat(myproc, &st, "put: EEL - put", PRINT_EEL);
	}
	
	BARRIER();
	
	if (iamsender) {
    		/* measure baseline (no cpu loop) gap for nonblocking explicit bulk puts */
		init_stat(&st, nbytes);
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gasnet_handle_t h = gasnet_put_nb_bulk(peerproc, peermem, mymem, nbytes);
			gasnet_wait_syncnb(h);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);

    		/* Seek number of loops needed to exceed running time by 20% or more */
		delay_time = 1.2 * st.time;
		loops = test_calibrate_delay(iters, pollcnt, &delay_time);

		/* Now measure overhead */
		init_stat(&st, nbytes);
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gasnet_handle_t h = gasnet_put_nb_bulk(peerproc, peermem, mymem, nbytes);
			test_delay(loops, pollcnt);
			gasnet_wait_syncnb(h);
		}
		end = TIME();
	 	update_stat(&st, (end - begin) - delay_time, iters);
		print_stat(myproc, &st, "put: o_i - put_nb_bulk", PRINT_OVERHEAD);
	}

	BARRIER();

	/* target-side overhead takes more work: */
	if (iamsender) {
		for (i = 0; i < iters; i++) {
			gasnet_handle_t h = gasnet_put_nb_bulk(peerproc, peermem, mymem, nbytes);
			gasnet_wait_syncnb(h);
		}
	} else {
		init_stat(&st, nbytes);
		begin = TIME();
	}
	BARRIER();
	if (iamsender) {
		/* DO NOTHING */
	} else {
		end = TIME();
	 	update_stat(&st, (end - begin), iters);

		delay_time = 1.2 * st.time;
		loops = test_calibrate_delay(iters, pollcnt, &delay_time);
	}
	BARRIER();
	if (iamsender) {
		for (i = 0; i < iters; i++) {
			gasnet_handle_t h = gasnet_put_nb_bulk(peerproc, peermem, mymem, nbytes);
			gasnet_wait_syncnb(h);
		}
	} else {
		init_stat(&st, nbytes);
		begin = TIME();
		for (i = 0; i < iters; i++) {
			test_delay(loops, pollcnt);
		}
		end = TIME();
	 	update_stat(&st, (end - begin) - delay_time, iters);
		print_stat(myproc, &st, "put: o_t - put_nb_bulk", PRINT_OVERHEAD);
	}

	BARRIER();

	if (iamsender) {
		/* measure the throughput of nonblocking implicit put */
		init_stat(&st, nbytes);
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gasnet_put_nbi(peerproc, peermem, mymem, nbytes);
		}
		gasnet_wait_syncnbi_puts();
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
		print_stat(myproc, &st, "put: gap - put_nbi", PRINT_GAP);
	}
	
	BARRIER();

	if (iamsender) {
		/* measure the throughput of nonblocking implicit bulk put */
		init_stat(&st, nbytes);
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gasnet_put_nbi_bulk(peerproc, peermem, mymem, nbytes);
		}
		gasnet_wait_syncnbi_puts();
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
		print_stat(myproc, &st, "put: G   - put_nbi_bulk", PRINT_BIG_G);
	}
}	


void get_tests(int iters, int nbytes)
{GASNET_BEGIN_FUNCTION();
    int i;
    int64_t begin, end, delay_time, loops;
    stat_struct_t st;

	memset(mymem, 0, nbytes);

	BARRIER();

	if (iamsender) {
		/* measure the round-trip time of get */
		init_stat(&st, nbytes);
		begin = TIME();
		for (i = 0; i < iters; i++) {
	 		gasnet_get(mymem, peerproc, peermem, nbytes);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
		print_stat(myproc, &st, "get: EEL - get", PRINT_EEL);
	}
	
	BARRIER();
	
	if (iamsender) {
    		/* measure baseline (no cpu loop) gap for nonblocking explicit bulk gets */
		init_stat(&st, nbytes);
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gasnet_handle_t h = gasnet_get_nb_bulk(mymem, peerproc, peermem, nbytes);
			gasnet_wait_syncnb(h);
		}
		end = TIME();
	 	update_stat(&st, (end - begin), iters);

    		/* Seek number of loops needed to exceed running time by 20% or more */
		delay_time = 1.2 * st.time;
		loops = test_calibrate_delay(iters, pollcnt, &delay_time);

		/* Now measure overhead */
		init_stat(&st, nbytes);
		begin = TIME();
		for (i = 0; i < iters; i++) {
			gasnet_handle_t h = gasnet_get_nb_bulk(mymem, peerproc, peermem, nbytes);
			test_delay(loops, pollcnt);
			gasnet_wait_syncnb(h);
		}
		end = TIME();
	 	update_stat(&st, (end - begin) - delay_time, iters);
		print_stat(myproc, &st, "get: o_i - get_nb_bulk", PRINT_OVERHEAD);
	}

	BARRIER();

	/* target-side overhead takes more work: */
	if (iamsender) {
		for (i = 0; i < iters; i++) {
			gasnet_handle_t h = gasnet_get_nb_bulk(mymem, peerproc, peermem, nbytes);
			gasnet_wait_syncnb(h);
		}
	} else {
		init_stat(&st, nbytes);
		begin = TIME();
	}
	BARRIER();
	if (iamsender) {
		/* DO NOTHING */
	} else {
		end = TIME();
	 	update_stat(&st, (end - begin), iters);

		delay_time = 1.2 * st.time;
		loops = test_calibrate_delay(iters, pollcnt, &delay_time);
	}
	BARRIER();
	if (iamsender) {
		for (i = 0; i < iters; i++) {
			gasnet_handle_t h = gasnet_get_nb_bulk(mymem, peerproc, peermem, nbytes);
			gasnet_wait_syncnb(h);
		}
	} else {
		init_stat(&st, nbytes);
		begin = TIME();
		for (i = 0; i < iters; i++) {
			test_delay(loops, pollcnt);
		}
		end = TIME();
	 	update_stat(&st, (end - begin) - delay_time, iters);
		print_stat(myproc, &st, "get: o_t - get_nb_bulk", PRINT_OVERHEAD);
	}

	BARRIER();
	
	if (iamsender) {
		/* measure the throughput of nonblocking implicit get */
		init_stat(&st, nbytes);
		begin = TIME();
		for (i = 0; i < iters; i++) {
	 		gasnet_get_nbi(mymem, peerproc, peermem, nbytes);
		}
		gasnet_wait_syncnbi_gets();
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
		print_stat(myproc, &st, "get: gap - get_nbi", PRINT_GAP);
	}
	
	BARRIER();
	
	if (iamsender) {
		/* measure the throughput of nonblocking implicit bulk put */
		init_stat(&st, nbytes);
		begin = TIME();
		for (i = 0; i < iters; i++) {
	 		gasnet_get_nbi_bulk(mymem, peerproc, peermem, nbytes);
		}
		gasnet_wait_syncnbi_gets();
		end = TIME();
	 	update_stat(&st, (end - begin), iters);
    		print_stat(myproc, &st, "get: G   - get_nbi_bulk", PRINT_BIG_G);
	}
}


int main(int argc, char **argv)
{
    int iters = 0;
    int i;
    char usagestr[255];
   
    /* call startup */
    GASNET_Safe(gasnet_init(&argc, &argv));
    GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
    sprintf(usagestr, "iters pollcnt sizes...\n"
                      "    sizes are limited to %ld", (long)TEST_SEGSZ);
    test_init("testlogGP",1, usagestr);
    

    /* parse arguments */
    if (argc < 4) test_usage();

    iters = atoi(argv[1]);
    if (!iters) iters = 1;

    pollcnt = atoi(argv[2]);

    /* get SPMD info */
    myproc = gasnet_mynode();
    numprocs = gasnet_nodes();

    /* Only allow even number for numprocs */
    if (numprocs % 2 != 0) {
      MSG0("WARNING: This test requires an even number of nodes. Test skipped.\n");
      gasnet_exit(0); /* exit 0 to prevent false negatives in test harnesses for smp-conduit */
    }
    
    /* initialize global data in my thread */
    mymem = (void *) TEST_MYSEG();
    
    /* Setting peer thread rank */
    peerproc = (myproc % 2) ? (myproc - 1) : (myproc + 1);
    iamsender = (myproc % 2 == 0);
    
    peermem = (void *) TEST_SEG(peerproc);

    for (i = 3; i < argc; ++i) {
        int size = atoi(argv[i]);

        if (size < 0 || size > TEST_SEGSZ) {
            MSG0("size is limited to <= %ld\n", (long)TEST_SEGSZ);
            continue;
        }

	put_tests(iters, size); 
	get_tests(iters, size); 
    }

    BARRIER();
    gasnet_exit(0);

    return 0;

}
/* ------------------------------------------------------------------------------------ */
