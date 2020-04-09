/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testcontend.c $
 *
 * Description: GASNet threaded contention tester.
 *   The test initializes GASNet and forks off up to 256 threads.  
 *  The test measures the level of inter-thread contention for local 
 *  network resources with various different usage patterns.
 *
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include "test.h"

#ifndef GASNET_PAR
#error This test can only be built for GASNet PAR configuration
#endif

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

static gex_Rank_t myrank;
static gex_Rank_t numranks;

typedef struct {
  int activecnt;
  int passivecnt;
} threadcnt_t;

typedef gex_AM_Arg_t harg_t;

/* configurable parameters */
#define DEFAULT_ITERS 50
int	iters = DEFAULT_ITERS;
int amactive;
int peer = -1;
char *peerseg = NULL;
int threads;
gasnett_atomic_t pong;
volatile int signal_done = 0;
#define thread_barrier() PTHREAD_BARRIER(threads)

int revthreads = 0;
#define ARG2THREAD(arg) (revthreads?(threads-1)-(int)(intptr_t)args:(int)(intptr_t)args)

typedef void * (*threadmain_t)(void *args);

/* AM Handlers */
void	ping_shorthandler(gex_Token_t token);
void 	pong_shorthandler(gex_Token_t token);

void	markdone_shorthandler(gex_Token_t token);

#define hidx_ping_shorthandler        201
#define hidx_pong_shorthandler        202
#define hidx_markdone_shorthandler    203

gex_AM_Entry_t htable[] = { 
	{ hidx_ping_shorthandler,     ping_shorthandler,     GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 0 },
	{ hidx_pong_shorthandler,     pong_shorthandler,     GEX_FLAG_AM_REPLY|GEX_FLAG_AM_SHORT, 0 },
	{ hidx_markdone_shorthandler, markdone_shorthandler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 0 },
};
#define HANDLER_TABLE_SIZE (sizeof(htable)/sizeof(gex_AM_Entry_t))

#define SPINPOLL_UNTIL(cond) do { while (!(cond)) gasnet_AMPoll(); } while (0)

#define BARRIER_UNTIL(cond) do {                                          \
      if (mythread == 0) SPINPOLL_UNTIL(cond);                            \
      else if (mythread == 1) {                                           \
        /* one thread sits in barrier during test */                      \
        gasnet_barrier_notify(0,GASNET_BARRIERFLAG_ANONYMOUS);            \
        GASNET_Safe(gasnet_barrier_wait(0,GASNET_BARRIERFLAG_ANONYMOUS)); \
      }                                                                   \
  } while (0)

    
int _havereport = 0;
char _reportstr[644];
const char *getreport(void) {
  if (_havereport) {
    _havereport = 0;
    return _reportstr;
  } else return NULL;
}
void report(gasnett_tick_t ticks) {
  double timeus = (double)gasnett_ticks_to_us(ticks);
  snprintf(_reportstr, sizeof(_reportstr),
     "%7.3f us\t%5.3f sec", 
     timeus/iters, timeus/1000000);
  _havereport = 1;
}

/* testing functions */

#define AMPINGPONG(fnname, POLLUNTIL)                                                   \
  void * fnname(void *args) {                                                           \
    int mythread = ARG2THREAD(args);                                                    \
    static int nonzero_present = 0;                                                     \
    gasnett_tick_t start, end;                                                          \
    signal_done = 0;                                                                    \
    if (mythread != 0) nonzero_present = 1;                                             \
    thread_barrier();                                                                   \
    if (mythread == 0) {                                                                \
      int i;                                                                            \
      gasnett_atomic_set(&pong,0,0);                                                    \
      start = gasnett_ticks_now();                                                      \
      for (i = 0; i < iters; i++) {                                                     \
        gex_AM_RequestShort0(myteam, peer, hidx_ping_shorthandler, 0);              \
        POLLUNTIL(gasnett_atomic_read(&pong,0) > i);                                    \
      }                                                                                 \
      end = gasnett_ticks_now();                                                        \
      gex_AM_RequestShort0(myteam, peer, hidx_markdone_shorthandler, 0);            \
      gex_AM_RequestShort0(myteam, myrank, hidx_markdone_shorthandler, 0); \
      if (!nonzero_present) {                                                           \
        mythread = 1; /* ensure it runs once, impersonating thread1 */                  \
        POLLUNTIL(signal_done);                                                         \
        mythread = 0;                                                                   \
      }                                                                                 \
    } else {                                                                            \
      POLLUNTIL(signal_done);                                                           \
    }                                                                                   \
    thread_barrier();                                                                   \
    nonzero_present = 0;                                                                \
    if (mythread == 0 && amactive) report(end-start);                                   \
    return NULL;                                                                        \
  }

AMPINGPONG(ampingpong_poll_active, SPINPOLL_UNTIL)
AMPINGPONG(ampingpong_block_active, GASNET_BLOCKUNTIL)

AMPINGPONG(ampingpong_barrier_active, BARRIER_UNTIL)

#define PUTGETPINGPONG(fnname, POLLUNTIL, putgetstmt)                                   \
  void * fnname(void *args) {                                                           \
    int64_t tmp = 0;                                                                    \
    int mythread = ARG2THREAD(args);                                                    \
    static int nonzero_present = 0;                                                     \
    gasnett_tick_t start, end;                                                          \
    signal_done = 0;                                                                    \
    if (mythread != 0) nonzero_present = 1;                                             \
    thread_barrier();                                                                   \
    if (mythread == 0) {                                                                \
      int i;                                                                            \
      gasnett_atomic_set(&pong,0,0);                                                    \
      start = gasnett_ticks_now();                                                      \
      for (i = 0; i < iters; i++) {                                                     \
        putgetstmt;                                                                     \
      }                                                                                 \
      end = gasnett_ticks_now();                                                        \
      gex_AM_RequestShort0(myteam, peer, hidx_markdone_shorthandler, 0);            \
      gex_AM_RequestShort0(myteam, myrank, hidx_markdone_shorthandler, 0); \
      if (!nonzero_present) {                                                           \
        mythread = 1; /* ensure it runs once, impersonating thread1 */                  \
        POLLUNTIL(signal_done);                                                         \
        mythread = 0;                                                                   \
      }                                                                                 \
    } else {                                                                            \
      POLLUNTIL(signal_done);                                                           \
    }                                                                                   \
    thread_barrier();                                                                   \
    nonzero_present = 0;                                                                \
    if (mythread == 0 && amactive) report(end-start);                                   \
    return NULL;                                                                        \
  }

PUTGETPINGPONG(put_poll_active, SPINPOLL_UNTIL, gex_RMA_PutBlocking(myteam, peer, peerseg, &tmp, 8, 0))
PUTGETPINGPONG(get_poll_active, SPINPOLL_UNTIL, gex_RMA_GetBlocking(myteam, &tmp, peer, peerseg, 8, 0))
PUTGETPINGPONG(put_block_active, GASNET_BLOCKUNTIL, gex_RMA_PutBlocking(myteam, peer, peerseg, &tmp, 8, 0))
PUTGETPINGPONG(get_block_active, GASNET_BLOCKUNTIL, gex_RMA_GetBlocking(myteam, &tmp, peer, peerseg, 8, 0))

PUTGETPINGPONG(put_barrier_active, BARRIER_UNTIL, gex_RMA_PutBlocking(myteam, peer, peerseg, &tmp, 8, 0))
PUTGETPINGPONG(get_barrier_active, BARRIER_UNTIL, gex_RMA_GetBlocking(myteam, &tmp, peer, peerseg, 8, 0))

#define PGFIGHT(fnname, putgetstmt_loner, putgetstmt_rest)                              \
  void * fnname(void *args) {                                                           \
    int64_t tmp = 0;                                                                    \
    int mythread = ARG2THREAD(args);                                                    \
    gasnett_tick_t start, end;                                                          \
    signal_done = 0;                                                                    \
    thread_barrier();                                                                   \
    if (mythread == 0) {                                                                \
      int i;                                                                            \
      start = gasnett_ticks_now();                                                      \
      for (i = 0; i < iters; i++) {                                                     \
        putgetstmt_loner;                                                               \
      }                                                                                 \
      end = gasnett_ticks_now();                                                        \
      gex_AM_RequestShort0(myteam, peer, hidx_markdone_shorthandler, 0);            \
      gex_AM_RequestShort0(myteam, myrank, hidx_markdone_shorthandler, 0); \
    } else {                                                                            \
      while(!signal_done) {                                                             \
        putgetstmt_rest;                                                                \
      }                                                                                 \
    }                                                                                   \
    thread_barrier();                                                                   \
    if (mythread == 0 && amactive) report(end-start);                                   \
    return NULL;                                                                        \
  }                                                                                     \

PGFIGHT(put_put_active, gex_RMA_PutBlocking(myteam, peer, peerseg, &tmp, 8, 0), gex_RMA_PutBlocking(myteam, peer, peerseg, &tmp, 8, 0))
PGFIGHT(put_get_active, gex_RMA_PutBlocking(myteam, peer, peerseg, &tmp, 8, 0), gex_RMA_GetBlocking(myteam, &tmp, peer, peerseg, 8, 0))
PGFIGHT(get_put_active, gex_RMA_GetBlocking(myteam, &tmp, peer, peerseg, 8, 0), gex_RMA_PutBlocking(myteam, peer, peerseg, &tmp, 8, 0))
PGFIGHT(get_get_active, gex_RMA_GetBlocking(myteam, &tmp, peer, peerseg, 8, 0), gex_RMA_GetBlocking(myteam, &tmp, peer, peerseg, 8, 0))

void * poll_passive(void *args) {
  int mythread = ARG2THREAD(args);
  signal_done = 0;
  thread_barrier();
  while (!signal_done) gasnet_AMPoll();
  thread_barrier();
  return NULL;
}
void * block_passive(void *args) {
  int mythread = ARG2THREAD(args);
  signal_done = 0;
  thread_barrier();
  GASNET_BLOCKUNTIL(signal_done);
  thread_barrier();
  return NULL;
}
void * barrier_passive(void *args) {
  int mythread = ARG2THREAD(args);
  signal_done = 0;
  thread_barrier();
  while (!signal_done) gasnet_AMPoll();
  if (mythread == 0) { /* match the barrier the active side is waiting for */
    gasnet_barrier_notify(0,GASNET_BARRIERFLAG_ANONYMOUS);
    GASNET_Safe(gasnet_barrier_wait(0,GASNET_BARRIERFLAG_ANONYMOUS));
  }
  thread_barrier();
  return NULL;
}

typedef struct {
  const char *desc;
  threadmain_t activefunc;
  threadmain_t passivefunc;
} fntable_t;

fntable_t fntable[] = {
  { "AM Ping-pong vs. spin-AMPoll()", ampingpong_poll_active, poll_passive },
  { "AM Ping-pong vs. BLOCKUNTIL",    ampingpong_block_active, block_passive },
  { "gex_RMA_PutBlocking vs. spin-AMPoll()", put_poll_active, poll_passive },
  { "gex_RMA_PutBlocking vs. BLOCKUNTIL",    put_block_active, block_passive },
  { "gex_RMA_GetBlocking vs. spin-AMPoll()", get_poll_active, poll_passive },
  { "gex_RMA_GetBlocking vs. BLOCKUNTIL",    get_block_active, block_passive },
  { "gex_RMA_PutBlocking vs. gex_RMA_PutBlocking",    put_put_active, poll_passive },
  { "gex_RMA_PutBlocking vs. gex_RMA_GetBlocking",    put_get_active, poll_passive },
  { "gex_RMA_GetBlocking vs. gex_RMA_PutBlocking",    get_put_active, poll_passive },
  { "gex_RMA_GetBlocking vs. gex_RMA_GetBlocking",    get_get_active, poll_passive },
  { "AM Ping-pong vs. local barrier", ampingpong_barrier_active, barrier_passive },
  { "gex_RMA_PutBlocking vs. local barrier",   put_barrier_active, barrier_passive },
  { "gex_RMA_GetBlocking vs. local barrier",   get_barrier_active, barrier_passive },
};
#define NUM_FUNC (sizeof(fntable)/sizeof(fntable_t))
int tcountentries;
threadcnt_t *tcount;

void *workerthread(void *args) {
  int fnidx;
  int mythread = ARG2THREAD(args);
  for (fnidx = 0; fnidx < NUM_FUNC; fnidx++) {
    int tcountpos;

    if (mythread == 0) TEST_SECTION_BEGIN();
    thread_barrier();
    if (!TEST_SECTION_ENABLED()) {
      thread_barrier();
      continue;
    }

    if (mythread == 0 && myrank == 0) {
        MSG("%c: --------------------------------------------------------------------------",
            TEST_SECTION_NAME());
        MSG("%c: Running test %s", TEST_SECTION_NAME(), fntable[fnidx].desc);
        MSG("%c: --------------------------------------------------------------------------",
            TEST_SECTION_NAME());
        MSG("%c: Active-end threads\tPassive-end threads\t  IterTime\tTotalTime",
            TEST_SECTION_NAME());
        MSG("%c: --------------------------------------------------------------------------",
            TEST_SECTION_NAME());
    }

    for (tcountpos = 0; tcountpos < tcountentries; tcountpos++) {
      threadmain_t mainfn = amactive ? fntable[fnidx].activefunc : fntable[fnidx].passivefunc;
      int participating_threads = amactive ? tcount[tcountpos].activecnt : tcount[tcountpos].passivecnt;
      thread_barrier();
      if (mythread < participating_threads) mainfn(args);
      else { /* match barriers */
        thread_barrier();
        thread_barrier();
      }
      thread_barrier();
      if (mythread == 0 && amactive) { 
        const char *rpt = getreport();
        if (rpt) MSG("%c:\t   %d\t\t\t  %d\t\t%s", TEST_SECTION_NAME(),
          tcount[tcountpos].activecnt, tcount[tcountpos].passivecnt, rpt);
      }
    }
  }
  return NULL;
}
int main(int argc, char **argv) {
	int maxthreads = 4;
	int i;
	int arg;
	int help = 0;
        threadcnt_t *ptcount;

	GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testcontend", &argc, &argv, 0));
        GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
        GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, HANDLER_TABLE_SIZE));

        myrank = gex_TM_QueryRank(myteam);
        numranks = gex_TM_QuerySize(myteam);

	test_init("testcontend",1,"[options] (maxthreads) (iters) (test_sections)\n"
                  "  The -rev option reverses thread numbering");

        arg = 1;
        while (argc > arg) {
          if (!strcmp(argv[arg], "-rev")) {
            revthreads = 1;
            ++arg;
          } else if (argv[arg][0] == '-') {
            help = 1;
            ++arg;
          } else break;
        }
        if (argc > arg) { maxthreads = atoi(argv[arg]); ++arg; }
        if (argc > arg) { iters = atoi(argv[arg]); ++arg; }
        if (argc > arg) { TEST_SECTION_PARSE(argv[arg]); ++arg; }
        if (argc > arg || help) test_usage();

	if (maxthreads > TEST_MAXTHREADS || maxthreads < 1) {
	  printf("Threads must be between 1 and %i\n", TEST_MAXTHREADS);
	  gasnet_exit(-1);
	}
	maxthreads = test_thread_limit(maxthreads);
        if (numranks % 2 != 0) {
    	  MSG0("WARNING: This test requires an even number of nodes. Test skipped.\n");
    	  gasnet_exit(0); /* exit 0 to prevent false negatives in test harnesses for smp-conduit */
        }
        if (myrank == 0) {
          MSG("Running testcontend with 1..%i threads and %i iterations", maxthreads, iters);
        }
        tcountentries = 3 * maxthreads;
        tcount = test_malloc(tcountentries * sizeof(threadcnt_t));
        ptcount = tcount;
        for (i = 1; i <= maxthreads; i++) { ptcount->activecnt = i; ptcount->passivecnt = 1; ptcount++; }
        for (i = 1; i <= maxthreads; i++) { ptcount->activecnt = 1; ptcount->passivecnt = i; ptcount++; }
        for (i = 1; i <= maxthreads; i++) { ptcount->activecnt = i; ptcount->passivecnt = i; ptcount++; }
        peer = (myrank + 1) % numranks;
        amactive = (myrank % 2 == 0);

        peerseg = TEST_SEG(peer);

        /* create all worker threads */
        threads = maxthreads;
        test_createandjoin_pthreads(maxthreads, &workerthread, NULL, 0);

        BARRIER();
	if (myrank == 0) MSG("Tests complete");
        BARRIER();

	gasnet_exit(0);

	return 0;
}

/****************************************************************/
/* AM Handlers */
void ping_shorthandler(gex_Token_t token) {
  gex_AM_ReplyShort0(token, hidx_pong_shorthandler, 0);
}

void pong_shorthandler(gex_Token_t token) {
  gasnett_atomic_increment(&pong,0);
}

void markdone_shorthandler(gex_Token_t token) {
  signal_done = 1;
}

