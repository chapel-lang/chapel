/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testthreads.c $
 *
 * Description: GASNet threaded tester.
 *   The test initializes GASNet and forks off up to 256 threads.  Each of
 *   these threads randomly chooses from a set of communication operations for
 *   a given amount of iterations.  The idea is to detect race errors by having
 *   many threads concurrently use different GASNet communication operations.
 *
 * Copyright 2003, Christian Bell <csbell@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include "test.h"

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

static gex_Rank_t myrank;
static gex_Rank_t numranks;

#if !defined(GASNET_PAR)
  #ifdef TEST_MPI
   #ifdef GASNET_SEQ
    /* special hacks to allow testmpi-seq */
    #define TEST_SEGZ_PER_THREAD TEST_SEGSZ
    #define TEST_MAXTHREADS 1
   #endif
  #else
    #error This test can only be built for GASNet PAR configuration
  #endif
#endif

typedef 
struct _threaddata_t {
	int	tid;		/* global thread id */
	int	ltid;		/* local thread id (index into each node's
				   array of threaddata_t) */
	int	tid_peer;	/* global thread id of remote peer thread */
	int	tid_peer_local; /* global thread id of local peer thread */

	volatile int	flag;
	char _pad[GASNETT_CACHE_LINE_BYTES];
} 
threaddata_t;

typedef void (*testfunc_t)(threaddata_t *);
typedef gex_AM_Arg_t harg_t;

/* configurable parameters */
#define DEFAULT_ITERS 50
int	iters = DEFAULT_ITERS;
int	sleep_min_us = 1;
int	sleep_max_us = 250000;
int	amiters_max = 50;
int     verbose = 0;
int     amtrace = 0;
int     threadstress = 0;

#if PLATFORM_COMPILER_TINY
/* Appears unable to expand GASNETT_TRACE_SETSOURCELINE multiple times per line.
 * The instance in MSG should be sufficient, right? */
#define ACTION_PRINTF \
  if (verbose) MSG
#else
#define ACTION_PRINTF \
  if (GASNETT_TRACE_SETSOURCELINE(__FILE__,__LINE__), verbose) MSG
#endif

int     sizes[] = { 0, /* gex_AM_LUBRequestMedium()-1  */
                    0, /* gex_AM_LUBRequestMedium()    */
                    0, /* gex_AM_LUBRequestMedium()+1  */
                    0, /* gex_AM_LUBReplyMedium()-1    */
                    0, /* gex_AM_LUBReplyMedium()      */
                    0, /* gex_AM_LUBReplyMedium()+1    */
                    0, /* gex_AM_LUBRequestLong()-1 */
                    0, /* gex_AM_LUBRequestLong()   */
                    0, /* gex_AM_LUBRequestLong()+1 */
                    0, /* gex_AM_LUBReplyLong()-1   */
                    0, /* gex_AM_LUBReplyLong()     */
                    0, /* gex_AM_LUBReplyLong()+1   */
                    /* some other interesting fixed values */
                    0, 1, 9, 128, 256, 1024, 2048, 4095, 4096, 4097, 
                    16384, 30326, TEST_SEGZ_PER_THREAD };

#define	SIZES_NUM	(sizeof(sizes)/sizeof(int))
#define RANDOM_SIZE()	(sizes[ (rand() % SIZES_NUM)])

int		AM_loopback = 0;
#if GASNET_PAR
int		threads_num = 4;
#else
int		threads_num = 1;
#endif
gex_Rank_t	*tt_thread_map;
void		**tt_addr_map;
threaddata_t	*tt_thread_data;
void            *myseg;

#ifdef GASNET_PAR
#define thread_barrier() PTHREAD_BARRIER(threads_num)
#else
#define thread_barrier() ((void)0)
#endif

void	alloc_thread_data(int threads);
void	free_thread_data(void);
void *	threadmain(void *args);

/* GASNet Test functions */
void	test_sleep(threaddata_t *tdata);
void	test_put(threaddata_t *tdata);
void	test_get(threaddata_t *tdata);
void	test_amshort(threaddata_t *tdata);
void	test_ammedium(threaddata_t *tdata);
void	test_amlong(threaddata_t *tdata);
#if TEST_MPI
void init_test_mpi(int *argc, char ***argv);
void attach_test_mpi(void);
void mpi_barrier(threaddata_t *tdata);
void test_mpi(threaddata_t *tdata);

void mpi_handler(gex_Token_t token, harg_t tid, harg_t sz);
void mpi_probehandler(gex_Token_t token, harg_t tid);
void mpi_replyhandler(gex_Token_t token, harg_t tid);
#endif

testfunc_t	test_functions_all[] = {
	test_sleep, test_put, test_get, test_amshort, test_ammedium, test_amlong
#if TEST_MPI
        , test_mpi
#endif
};

#define NUM_FUNCTIONS	(sizeof(test_functions_all)/sizeof(testfunc_t))
/* This array remains uninitialized */
testfunc_t	test_functions[NUM_FUNCTIONS] = { 0 };
static int	functions_num = 0;

/* AM Handlers */
void	ping_shorthandler(gex_Token_t token, harg_t tid);
void 	pong_shorthandler(gex_Token_t token, harg_t tid);

void	ping_medhandler(gex_Token_t token, void *buf, size_t nbytes, 
		harg_t tid, harg_t repsz);
void	pong_medhandler(gex_Token_t token, void *buf, size_t nbytes, 
		harg_t tid);

void	ping_longhandler(gex_Token_t token, void *buf, size_t nbytes,
		harg_t tid, harg_t target_id, harg_t repsz);
void	pong_longhandler(gex_Token_t token, void *buf, size_t nbytes, 
		harg_t tid);

#define hidx_ping_shorthandler   201
#define hidx_pong_shorthandler   202
#define hidx_ping_medhandler     203
#define hidx_pong_medhandler     204
#define hidx_ping_longhandler    205
#define hidx_pong_longhandler    206
#define hidx_mpi_handler         207
#define hidx_mpi_probehandler    208
#define hidx_mpi_replyhandler    209

gex_AM_Entry_t htable[] = { 
	{ hidx_ping_shorthandler, ping_shorthandler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 1 },
	{ hidx_pong_shorthandler, pong_shorthandler, GEX_FLAG_AM_REPLY|GEX_FLAG_AM_SHORT, 1 },
	{ hidx_ping_medhandler,   ping_medhandler,   GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDIUM, 2 },
	{ hidx_pong_medhandler,   pong_medhandler,   GEX_FLAG_AM_REPLY|GEX_FLAG_AM_MEDIUM, 1 },
	{ hidx_ping_longhandler,  ping_longhandler,  GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_LONG, 3 },
	{ hidx_pong_longhandler,  pong_longhandler,  GEX_FLAG_AM_REPLY|GEX_FLAG_AM_LONG, 1 },
      #if TEST_MPI
	{ hidx_mpi_handler,       mpi_handler,       GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 2 },
	{ hidx_mpi_probehandler,  mpi_probehandler,  GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 1 },
	{ hidx_mpi_replyhandler,  mpi_replyhandler,  GEX_FLAG_AM_REPLY|GEX_FLAG_AM_SHORT, 1 },
      #endif
};
#define HANDLER_TABLE_SIZE (sizeof(htable)/sizeof(gex_AM_Entry_t))

	

int
main(int argc, char **argv)
{
	int		i;
        const char *getopt_str;
        int opt_p=0, opt_g=0, opt_m=0;
        int opt_S=0, opt_M=0, opt_L=0;

        #if TEST_MPI
          init_test_mpi(&argc, &argv);
          getopt_str = "pgSMLamlvtdi:";
        #else
          getopt_str = "pgSMLalvtdi:";
        #endif

	GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testthreads", &argc, &argv, 0));
        GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
        GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, HANDLER_TABLE_SIZE));

        myrank = gex_TM_QueryRank(myteam);
        numranks = gex_TM_QuerySize(myteam);
	myseg = TEST_SEG(myrank);

        #if TEST_MPI
          #define TEST_MPI_USAGE  "  -m  use MPI calls                              \n"
        #else
          #define TEST_MPI_USAGE  ""
        #endif
        #if GASNET_PAR
          #define TEST_THREAD_USAGE " [<threads_per_node>]\n\n" \
	    "<threads_per_node> must be between 1 and %u       \n"
          #define TEST_THREAD_USAGE_ARGS (unsigned int)TEST_MAXTHREADS,
        #else
          #define TEST_THREAD_USAGE  "\n\n"
          #define TEST_THREAD_USAGE_ARGS
        #endif
        static char usage[2048];
        snprintf(usage, sizeof(usage),
            "[ -pgSMLalvtd ] [ -i <iters> ]"
            TEST_THREAD_USAGE
	    "no options means run all tests with %i iterations\n"
	    "options:                                      \n"
	    "  -p  use puts                                   \n"
	    "  -g  use gets                                   \n"
	    "  -S  use Active Message Shorts                  \n"
	    "  -M  use Active Message Mediums                 \n"
	    "  -L  use Active Message Longs                   \n"
	    "  -a  use all Active Messages (-S -M -L)         \n"
	    "  -l  use local Active Messages                  \n"
            TEST_MPI_USAGE
	    "  -v  output information about actions taken     \n"
	    "  -t  include AM handler actions with -v         \n"
	    "  -d  dynamic thread creation stress test        \n"
	    "  -i <iters> use <iters> iterations per thread   \n",
            TEST_THREAD_USAGE_ARGS DEFAULT_ITERS);
	test_init("testthreads",0, usage);

	while ((i = getopt (argc, argv, getopt_str)) != EOF) {
          switch (i) {
		case 'p': opt_p = 1; break;
		case 'g': opt_g = 1; break;
		case 'S': opt_S = 1; break;
		case 'M': opt_M = 1; break;
		case 'L': opt_L = 1; break;
		case 'a': opt_S = opt_M = opt_L = 1; break;
                case 'm': opt_m = 1; break;
		case 'l': AM_loopback = 1; break;
		case 'i': iters = atoi(optarg); break;
                case 'v': verbose = 1; break;
                case 't': amtrace = 1; break;
                case 'd': threadstress = 1; break;
		default: test_usage();
          }
	}

        if (opt_p) test_functions[functions_num++] = test_put;
        if (opt_g) test_functions[functions_num++] = test_get;
        if (opt_S) test_functions[functions_num++] = test_amshort;
        if (opt_M) test_functions[functions_num++] = test_ammedium;
        if (opt_L) test_functions[functions_num++] = test_amlong;
        #if TEST_MPI
          if (opt_m) test_functions[functions_num++] = test_mpi;
        #endif
        if (amtrace) verbose = 1;

	/* Assume all test functions if no option is passed */
	if (functions_num  == 0) {
		MSG("running all functions!");
		
		memcpy(test_functions, test_functions_all, 
				sizeof(test_functions_all));
		functions_num = NUM_FUNCTIONS;
	}

	argc -= optind;

	if (argc > 1) test_usage();
	else if (argc == 1) {
		argv += optind;
                threads_num = atoi(argv[0]);
	}
        #if GASNET_PAR
	  threads_num = test_thread_limit(threads_num);
        #endif
	if (threads_num < 1) {
		printf("ERROR: Threads must be between 1 and %u\n",(unsigned int)TEST_MAXTHREADS);
		exit(EXIT_FAILURE);
	}
	if (numranks == 1 && threads_num == 1) {
		printf("ERROR: Threads must be greater than 1 when running a single process\n");
		exit(EXIT_FAILURE);
	}

        /* limit sizes to a reasonable size */
        #define LIMIT(sz) MIN(sz,4194304)
        { int sz = 0;
          sizes[sz++] = LIMIT(gex_AM_LUBRequestMedium()-1);
          sizes[sz++] = LIMIT(gex_AM_LUBRequestMedium());
          sizes[sz++] = LIMIT(gex_AM_LUBRequestMedium()+1);
          sizes[sz++] = LIMIT(gex_AM_LUBReplyMedium()-1);
          sizes[sz++] = LIMIT(gex_AM_LUBReplyMedium());
          sizes[sz++] = LIMIT(gex_AM_LUBReplyMedium()+1);
          sizes[sz++] = LIMIT(gex_AM_LUBRequestLong()-1);
          sizes[sz++] = LIMIT(gex_AM_LUBRequestLong());
          sizes[sz++] = LIMIT(gex_AM_LUBRequestLong()+1);
          sizes[sz++] = LIMIT(gex_AM_LUBReplyLong()-1);
          sizes[sz++] = LIMIT(gex_AM_LUBReplyLong());
          sizes[sz++] = LIMIT(gex_AM_LUBReplyLong()+1);
          assert(sizes[sz] == 0);
        }

	alloc_thread_data(threads_num);
        #if TEST_MPI
          attach_test_mpi();
        #endif

        #ifdef GASNET_PAR
          if (threadstress) {
            int spawniters = MAX(1,iters/threads_num);
            int i;
            MSG("Dynamic thread creation stress test, %d gasnet threads, (%d at a time)", spawniters*threads_num, threads_num);
            iters = 10; /* enough iters to ensure we get thread registration */
            for (i = 0; i < spawniters; i++) {
              test_createandjoin_pthreads(threads_num, &threadmain, tt_thread_data, sizeof(threaddata_t));
              TEST_PROGRESS_BAR(i, spawniters);
            }
          } else {
            MSG("Forking %d gasnet threads and running %d iterations", threads_num, iters);
            test_createandjoin_pthreads(threads_num, &threadmain, tt_thread_data, sizeof(threaddata_t));
          }
        #else /* for testmpi-seq and -parsync */
         #ifdef GASNET_SEQ
  	  MSG("Running with 1 thread/node for GASNET_SEQ mode");
         #else
  	  MSG("Running with 1 thread/node for GASNET_PARSYNC mode");
         #endif
          threadmain(tt_thread_data);
        #endif

        BARRIER();

	free_thread_data();

	MSG("Tests complete");

        BARRIER();

	gasnet_exit(0);

	return 0;
}

void *
threadmain(void *args)
{
	int	i, idx;

	testfunc_t	func;
	threaddata_t	*td = (threaddata_t *) args;

	TEST_SRAND(((int)TIME()) * td->tid);

	thread_barrier();

	if (!threadstress) MSG("tid=%3d> starting.", td->tid);

	for (i = 0; i < iters; i++) {
		idx = TEST_RAND(0,functions_num-1);
		func = test_functions[idx];
		assert(func != NULL);

		func(td);
                if (td->ltid == 0 && !threadstress) TEST_PROGRESS_BAR(i, iters);
	}

	thread_barrier();
	if (!threadstress) MSG("tid=%3d> done.", td->tid);

	return NULL;
}

void
alloc_thread_data(int threads)
{
	int	tot_threads = numranks * threads;

	tt_thread_map = (gex_Rank_t *) test_malloc(sizeof(gex_Rank_t) * tot_threads);
	tt_thread_data = (threaddata_t *) test_malloc(sizeof(threaddata_t) * threads);
	tt_addr_map = (void **) test_malloc(sizeof(void *) * tot_threads);

	/* Initialize the thread to node map array and local thread data */
	{
		int 	i, j, tid, base;
		void	*segbase;

		threaddata_t	*td;
		for (i = 0; i < numranks; i++) {
			segbase = TEST_SEG(i);

			base = i * threads;
			for (j = 0; j < threads; j++) {
				tid = base + j;
				tt_thread_map[tid] = i;
				tt_addr_map[tid] = (void *) 
				    ((uintptr_t) segbase + 
				     (uintptr_t) (j * TEST_SEGZ_PER_THREAD));

				if (i == myrank) {
					td = &tt_thread_data[j];

					td->tid = tid;
					td->ltid = j;
					td->tid_peer_local = base + 
						((j+1) % threads);
					td->tid_peer = (numranks == 1)
                                            ? ((tid+1) % threads)
                                            : ((tid+threads) % tot_threads);
                                        assert_always(td->tid_peer != tid);
				}
			}
		}
	}
}


void
free_thread_data(void)
{
	test_free(tt_thread_map);
	test_free(tt_addr_map);
	test_free(tt_thread_data);
}

/****************************************************************/
/* AM Handlers */
#if PLATFORM_COMPILER_TINY
/* Appears unable to expand GASNETT_TRACE_SETSOURCELINE multiple times per line.
 * The instance in MSG should be sufficient, right? */
#define PRINT_AM(x) \
  if (amtrace) ACTION_PRINTF x
#else
#define PRINT_AM(x) \
  if (GASNETT_TRACE_SETSOURCELINE(__FILE__,__LINE__), amtrace) ACTION_PRINTF x
#endif

void 
ping_shorthandler(gex_Token_t token, harg_t idx) 
{
        gex_Rank_t node = test_msgsource(token);

	PRINT_AM(("node=%2d> AMShort Request for (%d,%d)", 
			(int)myrank, (int)node, (int)idx));
        assert(idx >= 0 && idx < threads_num);
        assert(node < numranks);
	gex_AM_ReplyShort1(token, hidx_pong_shorthandler, 0, idx);
}

void 
pong_shorthandler(gex_Token_t token, harg_t idx) 
{
	int	tid = tt_thread_data[idx].tid;
	PRINT_AM(("node=%2d> AMShort Reply for tid=%d, (%d,%d)", 
			(int)myrank, tid, (int)myrank, (int)idx));
        assert(idx >= 0 && idx < threads_num);
        assert(tid >= 0 && tid < threads_num*numranks);
	tt_thread_data[idx].flag++;
}

void 
ping_medhandler(gex_Token_t token, void *buf, size_t nbytes, harg_t idx, harg_t repsz)
{
        gex_Rank_t node = test_msgsource(token);

	PRINT_AM(("node=%2d> AMMedium Request for (%d,%d)", 
			(int)myrank, (int)node, (int)idx));
        assert(idx >= 0 && idx < threads_num);
        assert(node < numranks);
        assert(nbytes <= gex_AM_MaxRequestMedium(myteam,node,GEX_EVENT_NOW,0,2));
        assert((uintptr_t)buf+nbytes < (uintptr_t)myseg ||
               (uintptr_t)buf >= (uintptr_t)myseg + TEST_SEGSZ);
        nbytes = MIN(nbytes, (size_t)(uint32_t)repsz);
	gex_AM_ReplyMedium1(token, hidx_pong_medhandler, buf, nbytes, GEX_EVENT_NOW, 0, idx);
}
void 
pong_medhandler(gex_Token_t token, void *buf, size_t nbytes, 
		gex_AM_Arg_t idx)
{
	int	tid = tt_thread_data[idx].tid;

        gex_Rank_t node = test_msgsource(token);

	PRINT_AM(("node=%2d> AMMedium Reply for tid=%d, (%d,%d)", 
			(int)myrank, tid, (int)myrank, (int)idx));
        assert(idx >= 0 && idx < threads_num);
        assert(tid >= 0 && tid < threads_num*numranks);
        assert(nbytes <= gex_AM_MaxReplyMedium(myteam,node,GEX_EVENT_NOW,0,1));
        assert((uintptr_t)buf+nbytes < (uintptr_t)myseg ||
               (uintptr_t)buf >= (uintptr_t)myseg + TEST_SEGSZ);
	tt_thread_data[idx].flag++;
}

void 
ping_longhandler(gex_Token_t token, void *buf, size_t nbytes, harg_t idx, harg_t target_id, harg_t repsz)
{
	int		tid;
	void		*paddr;

        gex_Rank_t node = test_msgsource(token);

	tid = node * threads_num + idx;
	paddr = tt_addr_map[tid];

	PRINT_AM(("node=%2d> AMLong Request for (%d,%d)", 
			(int)myrank, (int)node, (int)idx));
        assert(idx >= 0 && idx < threads_num);
        assert(node < numranks);
        assert(nbytes <= gex_AM_MaxRequestLong(myteam,node,GEX_EVENT_NOW,0,3));
        assert(buf == tt_addr_map[target_id]);
        assert((uintptr_t)buf + nbytes <= (uintptr_t)myseg + TEST_SEGSZ);
        nbytes = MIN(nbytes, (size_t)(uint32_t)repsz);
	gex_AM_ReplyLong1(token, hidx_pong_longhandler, buf, nbytes, paddr, GEX_EVENT_NOW, 0, idx);
}

void 
pong_longhandler(gex_Token_t token, void *buf, size_t nbytes, harg_t idx) {
	int	tid = tt_thread_data[idx].tid;

	gex_Rank_t node = test_msgsource(token);

	PRINT_AM(("node=%2d> AMLong Reply for tid=%d, (%d,%d)", 
			(int)myrank, tid, (int)myrank, (int)idx));
        assert(idx >= 0 && idx < threads_num);
        assert(tid >= 0 && tid < threads_num*numranks);
        assert(nbytes <= gex_AM_MaxReplyLong(myteam,node,GEX_EVENT_NOW,0,1));
        assert(buf == tt_addr_map[myrank * threads_num + idx]);
        assert((uintptr_t)buf + nbytes <= (uintptr_t)myseg + TEST_SEGSZ);
	tt_thread_data[idx].flag++;
}

/****************************************************************/
/* GASNet testers */

void
test_sleep(threaddata_t *tdata)
{
	unsigned	usecs = (unsigned) sleep_min_us + 
				(rand() % (sleep_max_us - sleep_min_us));
	ACTION_PRINTF("tid=%3d> sleeping %.3f millisecs", tdata->tid, usecs/1000.0);
        { uint64_t goal = gasnett_ticks_to_us(gasnett_ticks_now()) + usecs;
          while (gasnett_ticks_to_us(gasnett_ticks_now()) < goal) 
            gasnett_sched_yield();
        }
	ACTION_PRINTF("tid=%3d> awaking", tdata->tid);
}

void
test_put(threaddata_t *tdata)
{
	int	peer = tdata->tid_peer;
	int	node = tt_thread_map[peer];
	void	*laddr = tt_addr_map[tdata->tid];
	void	*raddr = tt_addr_map[peer];
	int	 len;
	do {
		len = RANDOM_SIZE();
	} while (len > TEST_SEGZ_PER_THREAD);

	ACTION_PRINTF("tid=%3d> put (%p,%8d) -> tid=%3d,node=%d,addr=%p",
			tdata->tid, laddr, len, peer, node, raddr);

	gex_RMA_PutBlocking(myteam, node, raddr, laddr, len, 0);
}

void
test_get(threaddata_t *tdata)
{
	int	peer = tdata->tid_peer;
	int	node = tt_thread_map[peer];
	void	*laddr = tt_addr_map[tdata->tid];
	void	*raddr = tt_addr_map[peer];
	int	 len;
	do {
		len = RANDOM_SIZE();
	} while (len > TEST_SEGZ_PER_THREAD);

	ACTION_PRINTF("tid=%3d> get (%p,%8d) <- tid=%3d,node=%d,addr=%p",
			tdata->tid, laddr, len, peer, node, raddr);

	gex_RMA_GetBlocking(myteam, laddr, node, raddr, len, 0);
}

#define RANDOM_PEER(tdata)					\
	(AM_loopback ? 						\
		(rand() % 2 == 0 ? tdata->tid_peer		\
				 : tdata->tid_peer_local)	\
	: tdata->tid_peer)

void
test_amshort(threaddata_t *tdata)
{
	int 	 	peer = RANDOM_PEER(tdata);
	int		node = tt_thread_map[peer];

	ACTION_PRINTF("tid=%3d> AMShortRequest to tid=%3d", tdata->tid, peer);
	tdata->flag = -1;
        gasnett_local_wmb();
	gex_AM_RequestShort1(myteam, node, hidx_ping_shorthandler, 0, tdata->ltid);
	GASNET_BLOCKUNTIL(tdata->flag == 0);
	tdata->flag = -1;

	ACTION_PRINTF("tid=%3d> AMShortRequest to tid=%3d complete.", tdata->tid, peer);
}

void
test_ammedium(threaddata_t *tdata)
{
	int 	 	peer = RANDOM_PEER(tdata);
	int		node = tt_thread_map[peer];
	void		*laddr = tt_addr_map[tdata->tid];
	size_t	 	len;

	do {
		len = RANDOM_SIZE();
        } while (len > gex_AM_MaxRequestMedium(myteam,node,GEX_EVENT_NOW,0,2));
		
	ACTION_PRINTF("tid=%3d> AMMediumRequest (sz=%7d) to tid=%3d", tdata->tid, (int)len, peer);
	tdata->flag = -1;
        gasnett_local_wmb();
	gex_AM_RequestMedium2(myteam, node, hidx_ping_medhandler, laddr, len,
                                  GEX_EVENT_NOW, 0, tdata->ltid,
                                  (harg_t)MIN((size_t)0xfffffffU,
                                              gex_AM_MaxReplyMedium(myteam,node,GEX_EVENT_NOW,0,1)));
	GASNET_BLOCKUNTIL(tdata->flag == 0);
	tdata->flag = -1;

	ACTION_PRINTF("tid=%3d> AMMediumRequest to tid=%3d complete.", tdata->tid, peer);
}


void
test_amlong(threaddata_t *tdata)
{
	int 	 	peer = RANDOM_PEER(tdata);
	int		node = tt_thread_map[peer];
	void		*laddr = tt_addr_map[tdata->tid];
	void		*raddr = tt_addr_map[peer];
	size_t	 	len;

	do {
		len = RANDOM_SIZE();
        } while ((len > gex_AM_MaxRequestLong(myteam,node,GEX_EVENT_NOW,0,3))
              || (len > TEST_SEGZ_PER_THREAD));
		
	tdata->flag = -1;
        gasnett_local_wmb();
	ACTION_PRINTF("tid=%3d> AMLongRequest (sz=%7d) to tid=%3d", tdata->tid, (int)len, peer);

	gex_AM_RequestLong3(myteam, node, hidx_ping_longhandler, laddr, len, raddr,
                                GEX_EVENT_NOW, 0, tdata->ltid, peer,
                                (harg_t)MIN((size_t)0xfffffffU,
                                            gex_AM_MaxReplyLong(myteam,node,GEX_EVENT_NOW,0,1)));
	GASNET_BLOCKUNTIL(tdata->flag == 0);
	tdata->flag = -1;

	ACTION_PRINTF("tid=%3d> AMLongRequest to tid=%3d complete.", tdata->tid, peer);
}

