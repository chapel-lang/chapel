/* $Source: /var/local/cvs/gasnet/tests/testcore1.c,v $
 * $Date: 2010/04/24 03:52:58 $
 * $Revision: 1.21.50.1 $
 * Copyright 2002, Christian Bell <csbell@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 *
 * Description: GASNet Core Monotonic checksum test
 * This stress tests the ability of the core to successfully send
 * AM Requests/Replies through get and/or put functions.
 *
 * Get version:
 * 1. Node A generates 'n' random seeds and keeps a local checksum for each
 *    . .(barrier)
 * 2. Node B gets the 'n' seeds from Node A and keeps its checksum for each
 *    . .(barrier)
 * 3. Node A gets the checksum hashes from Node B and compares them against
 *    its own copy once the gets are sync'd
 *
 * Put version:
 * Steps 2 and 3 are puts for each other node.
 */

#include "test.h"

#define DEBUG_TRACE
#define CHKSUM_LENGTH	8
#define CHKSUM_NUM	64
#define CHKSUM_TOTAL	CHKSUM_LENGTH*CHKSUM_NUM
#define TESTSafe(x, msg) do {					\
	    if (!(x)) {  printf msg; gasnet_exit(1); } } while (0)

typedef struct {
	uint32_t	seed;
	unsigned char	chksum[CHKSUM_TOTAL];
} monoseed_t;

monoseed_t	 *_mseed;

int	myproc;
int	numproc;
int	peerproc;
int	numprocs;
gasnet_seginfo_t *seginfo_table;

/* Test specific globals */
int		 chksum_iters = 0;
gasnett_atomic_t chksum_success  = gasnett_atomic_init(0);
gasnett_atomic_t chksum_received = gasnett_atomic_init(0);

#define CHKSUM_DUMP(chksum) do {			\
		int i = 0;				\
		uint8_t *c = (uint8_t *)chksum;		\
		printf("0x");				\
		for (i = 0; i < CHKSUM_TOTAL; i++)	\
			printf("%0x", c[i]);		\
	} while (0);

#ifdef VERBOSE
void
monoseed_trace(int iter, int seed, void *chksum_loc, void *chksum_rem)
{
	printf("%d> iter=%4d, seed=%12d, chksum_local=", myproc, iter, seed); 
	CHKSUM_DUMP(chksum_loc);
	
	if (chksum_rem != NULL) {
		printf(" chksum_remote=");
		CHKSUM_DUMP(chksum_rem);
	}
	printf("\n"); fflush(stdout);
}
#else
#define monoseed_trace(iter, seed, chksum_loc, chksum_rem)
#endif

void
chksum_gen(int seed, void *buf)
{
	int		i;
	uint64_t	chksum;
        uint8_t         *p = buf;

	chksum = test_checksum((void *)&seed, 4);
	for (i = 0; i < CHKSUM_NUM; i++) {
		chksum = test_checksum((void *)&chksum, CHKSUM_LENGTH);
		memcpy(p, &chksum, CHKSUM_LENGTH);
		p += CHKSUM_LENGTH;
	}
	return;
}

void
monoseed_init(int num)
{
	int 		i;

	if (myproc % 2 == 0) {
		_mseed = (monoseed_t *) test_malloc(sizeof(monoseed_t) * num);
	        srand((int)TIME());

		for (i = 0; i < num; i++) {
			_mseed[i].seed = (int) rand() + 1;
			chksum_gen(_mseed[i].seed, &_mseed[i].chksum);
		}
	}
	return;
}

void
chksum_test(int iters)
{
	int	i;
	int	iamsender, iamreceiver;
	int     received;
#ifdef VERBOSE
	int     nloop = 0;
#endif

	iamsender = (myproc % 2 == 0);
	iamreceiver = !iamsender;

	BARRIER();

	if (iamsender) {
		for (i = 0; i < iters; i++)
			GASNET_Safe(
			    gasnet_AMRequestShort2((gasnet_node_t)peerproc, 
				201, i, _mseed[i].seed));
	}

	while ( (received = gasnett_atomic_read(&chksum_received,0)) < iters ) {
		/*
		if (iamreceiver) {
			if (received % 5 == 0) {
				printf("sleep 1\n");
				sleep(1);
			}
		}
		*/
#ifdef VERBOSE
	    nloop++;
	    if (nloop % 1000 == 0) {
		printf("TEST[%d] nloop = %d chksum_received = %d\n",
		       myproc,nloop,received);
	    }
#endif
	    gasnet_AMPoll();
	}
#ifdef VERBOSE
	printf("TEST[%d] COMPLETE: nloop = %d chksum_received = %d\n",
	       myproc,nloop,received);
#endif

	BARRIER();

	if (iamsender) {
	        int success = gasnett_atomic_read(&chksum_success,0);
		printf("chksum_test(%d) passed %d/%d\n", chksum_iters, 
		    success, received);
	}
}


/*
 * Format is
 * AMRequestShort2(dest, chksum_reqh, i, seed)
 *
 * chksum_reqh(i, seed) generates the checksum and replies with a Medium
 *
 * AMReplyMedium(token, chksum_reph, src, nbytes, i)
 *
 * chksum_reph(i, src, nbytes) compares src[nbytes] to its copy of the
 * checksum at i
 */
void chksum_reqh(gasnet_token_t token, 
	gasnet_handlerarg_t iter, gasnet_handlerarg_t seed)
{
        unsigned char   chksum_reqbuf[CHKSUM_TOTAL];

	gasnett_atomic_increment(&chksum_received, 0);
	chksum_gen(seed, &chksum_reqbuf);
	monoseed_trace(iter, seed, &chksum_reqbuf, NULL);
	GASNET_Safe( 
	    gasnet_AMReplyMedium1(token, 202, &chksum_reqbuf, 
	        CHKSUM_TOTAL, iter));
	return;
}

void
chksum_reph(gasnet_token_t token, 
	void *buf, size_t nbytes, gasnet_handlerarg_t iter) 
{
	gasnett_atomic_increment(&chksum_received, 0);
	assert_always(iter < chksum_iters && iter >= 0);
	assert_always(nbytes == CHKSUM_TOTAL);
	monoseed_trace(iter, _mseed[iter].seed, &_mseed[iter].chksum, buf);
	if (memcmp(&_mseed[iter].chksum, buf, CHKSUM_LENGTH) == 0) 
  	        gasnett_atomic_increment(&chksum_success, 0);
	else {
		printf("iter %3d failed! chksum_local=", (int)iter);
		CHKSUM_DUMP(&_mseed[iter].chksum);
		printf(" chksum_remote=");
		CHKSUM_DUMP(buf);
		printf("\n");
	}
	return;
}
	
/* Test handlers */

int
main(int argc, char **argv)
{
	int	iters = 0;
	gasnet_handlerentry_t htable[] = {
		{ 201, chksum_reqh },
		{ 202, chksum_reph }
	};

	/* call startup */
        GASNET_Safe(gasnet_init(&argc, &argv));
        GASNET_Safe(gasnet_attach(htable, sizeof(htable)/sizeof(gasnet_handlerentry_t), TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
	test_init("testcore1",0,"(iters)");

        assert(CHKSUM_TOTAL <= gasnet_AMMaxMedium());

	if (argc > 1) iters = atoi(argv[1]);
	if (!iters) iters = 1000;
        if (argc > 2) test_usage();
	

	/* get SPMD info */
	chksum_iters = iters;
	myproc = gasnet_mynode();
	numprocs = gasnet_nodes();
        /* Only allow even number for numprocs */
        if (numprocs % 2 != 0) {
          MSG("WARNING: This test requires an even number of nodes. Test skipped.\n");
          gasnet_exit(0); /* exit 0 to prevent false negatives in test harnesses for smp-conduit */
        }
	peerproc = (myproc % 2) ? myproc-1 : myproc+1;

	seginfo_table = (gasnet_seginfo_t *) test_malloc(sizeof(gasnet_seginfo_t) * numprocs);

	printf("%d> starting monoseed_init(%d)\n", myproc, iters);
	monoseed_init(iters);
	printf("%d> starting chksums_test(%d)\n", myproc, iters);
	chksum_test(iters);

	gasnet_exit(0);
	return(0);
}
