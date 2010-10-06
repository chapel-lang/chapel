/*   $Source: /var/local/cvs/gasnet/tests/testcontend.c,v $
 *     $Date: 2010/04/24 03:52:58 $
 * $Revision: 1.16.6.1 $
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

typedef struct {
  int activecnt;
  int passivecnt;
} threadcnt_t;

typedef gasnet_handlerarg_t harg_t;

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

typedef void * (*threadmain_t)(void *args);

/* AM Handlers */
void	ping_shorthandler(gasnet_token_t token);
void 	pong_shorthandler(gasnet_token_t token);

void	ping_medhandler(gasnet_token_t token, void *buf, size_t nbytes);
void	pong_medhandler(gasnet_token_t token, void *buf, size_t nbytes);

void	ping_longhandler(gasnet_token_t token, void *buf, size_t nbytes);
void	pong_longhandler(gasnet_token_t token, void *buf, size_t nbytes);

void	markdone_shorthandler(gasnet_token_t token);
void	noop_shorthandler(gasnet_token_t token);


#define hidx_ping_shorthandler        201
#define hidx_pong_shorthandler        202
#define hidx_ping_medhandler          203
#define hidx_pong_medhandler          204
#define hidx_ping_longhandler         205
#define hidx_pong_longhandler         206
#define hidx_markdone_shorthandler    207
#define hidx_noop_shorthandler        208

gasnet_handlerentry_t htable[] = { 
	{ hidx_ping_shorthandler,  ping_shorthandler  },
	{ hidx_pong_shorthandler,  pong_shorthandler  },
	{ hidx_ping_medhandler,    ping_medhandler    },
	{ hidx_pong_medhandler,    pong_medhandler    },
	{ hidx_ping_longhandler,   ping_longhandler   },
	{ hidx_pong_longhandler,   pong_longhandler   },
	{ hidx_markdone_shorthandler,   markdone_shorthandler   },
	{ hidx_noop_shorthandler,   noop_shorthandler   },
};
#define HANDLER_TABLE_SIZE (sizeof(htable)/sizeof(gasnet_handlerentry_t))

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
char _reportstr[255];
const char *getreport(void) {
  if (_havereport) {
    _havereport = 0;
    return _reportstr;
  } else return NULL;
}
void report(gasnett_tick_t ticks) {
  double timeus = (double)gasnett_ticks_to_us(ticks);
  sprintf(_reportstr, 
     "%7.3f us\t%5.3f sec", 
     timeus/iters, timeus/1000000);
  _havereport = 1;
}

/* testing functions */

#define AMPINGPONG(fnname, POLLUNTIL)                                                   \
  void * fnname(void *args) {                                                           \
    int mythread = (int)(intptr_t)args;                                                 \
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
        GASNET_Safe(gasnet_AMRequestShort0(peer, hidx_ping_shorthandler));              \
        POLLUNTIL(gasnett_atomic_read(&pong,0) > i);                                    \
      }                                                                                 \
      end = gasnett_ticks_now();                                                        \
      GASNET_Safe(gasnet_AMRequestShort0(peer, hidx_markdone_shorthandler));            \
      GASNET_Safe(gasnet_AMRequestShort0(gasnet_mynode(), hidx_markdone_shorthandler)); \
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
    int mythread = (int)(intptr_t)args;                                                 \
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
      GASNET_Safe(gasnet_AMRequestShort0(peer, hidx_markdone_shorthandler));            \
      GASNET_Safe(gasnet_AMRequestShort0(gasnet_mynode(), hidx_markdone_shorthandler)); \
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

PUTGETPINGPONG(put_poll_active, SPINPOLL_UNTIL, gasnet_put(peer, peerseg, &tmp, 8))
PUTGETPINGPONG(get_poll_active, SPINPOLL_UNTIL, gasnet_get(&tmp, peer, peerseg, 8))
PUTGETPINGPONG(put_block_active, GASNET_BLOCKUNTIL, gasnet_put(peer, peerseg, &tmp, 8))
PUTGETPINGPONG(get_block_active, GASNET_BLOCKUNTIL, gasnet_get(&tmp, peer, peerseg, 8))

PUTGETPINGPONG(put_barrier_active, BARRIER_UNTIL, gasnet_put(peer, peerseg, &tmp, 8))
PUTGETPINGPONG(get_barrier_active, BARRIER_UNTIL, gasnet_get(&tmp, peer, peerseg, 8))

#define PGFIGHT(fnname, putgetstmt_loner, putgetstmt_rest)                              \
  void * fnname(void *args) {                                                           \
    int64_t tmp = 0;                                                                    \
    int mythread = (int)(intptr_t)args;                                                 \
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
      GASNET_Safe(gasnet_AMRequestShort0(peer, hidx_markdone_shorthandler));            \
      GASNET_Safe(gasnet_AMRequestShort0(gasnet_mynode(), hidx_markdone_shorthandler)); \
    } else {                                                                            \
      while(!signal_done) {                                                             \
        putgetstmt_rest;                                                                \
      }                                                                                 \
    }                                                                                   \
    thread_barrier();                                                                   \
    if (mythread == 0 && amactive) report(end-start);                                   \
    return NULL;                                                                        \
  }                                                                                     \

PGFIGHT(put_put_active, gasnet_put(peer, peerseg, &tmp, 8), gasnet_put(peer, peerseg, &tmp, 8))
PGFIGHT(put_get_active, gasnet_put(peer, peerseg, &tmp, 8), gasnet_get(&tmp, peer, peerseg, 8))
PGFIGHT(get_put_active, gasnet_get(&tmp, peer, peerseg, 8), gasnet_put(peer, peerseg, &tmp, 8))
PGFIGHT(get_get_active, gasnet_get(&tmp, peer, peerseg, 8), gasnet_get(&tmp, peer, peerseg, 8))

void * poll_passive(void *args) {
  GASNETI_UNUSED
  int mythread = (int)(intptr_t)args;
  signal_done = 0;
  thread_barrier();
  while (!signal_done) gasnet_AMPoll();
  thread_barrier();
  return NULL;
}
void * block_passive(void *args) {
  GASNETI_UNUSED
  int mythread = (int)(intptr_t)args;
  signal_done = 0;
  thread_barrier();
  GASNET_BLOCKUNTIL(signal_done);
  thread_barrier();
  return NULL;
}
void * barrier_passive(void *args) {
  int mythread = (int)(intptr_t)args;
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
  { "gasnet_put vs. spin-AMPoll()", put_poll_active, poll_passive },
  { "gasnet_put vs. BLOCKUNTIL",    put_block_active, block_passive },
  { "gasnet_get vs. spin-AMPoll()", get_poll_active, poll_passive },
  { "gasnet_get vs. BLOCKUNTIL",    get_block_active, block_passive },
  { "gasnet_put vs. gasnet_put",    put_put_active, poll_passive },
  { "gasnet_put vs. gasnet_get",    put_get_active, poll_passive },
  { "gasnet_get vs. gasnet_put",    get_put_active, poll_passive },
  { "gasnet_get vs. gasnet_get",    get_get_active, poll_passive },
  { "AM Ping-pong vs. local barrier", ampingpong_barrier_active, barrier_passive },
  { "gasnet_put vs. local barrier",   put_barrier_active, barrier_passive },
  { "gasnet_get vs. local barrier",   get_barrier_active, barrier_passive },
};
#define NUM_FUNC (sizeof(fntable)/sizeof(fntable_t))
int tcountentries;
threadcnt_t *tcount;

void *workerthread(void *args) {
  int fnidx;
  int mythread = (int)(intptr_t)args;
  for (fnidx = 0; fnidx < NUM_FUNC; fnidx++) {
    int tcountpos;

    thread_barrier();
    if (mythread == 0 && gasnet_mynode() == 0) {
        MSG("--------------------------------------------------------------------------");
        MSG("Running test %s", fntable[fnidx].desc);
        MSG("--------------------------------------------------------------------------");
        MSG(" Active-end threads\tPassive-end threads\tIterTime\tTotalTime");
        MSG("--------------------------------------------------------------------------");
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
        if (rpt) MSG("\t   %d\t\t\t  %d\t\t%s", 
          tcount[tcountpos].activecnt, tcount[tcountpos].passivecnt, rpt);
      }
    }
  }
  return NULL;
}
int main(int argc, char **argv) {
	int maxthreads = 4;
	int i;
        threadcnt_t *ptcount;

	GASNET_Safe(gasnet_init(&argc, &argv));
    	GASNET_Safe(gasnet_attach(htable, HANDLER_TABLE_SIZE, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
	test_init("testcontend",1,"(maxthreads) (iters)");

	if (argc >= 2) maxthreads = atoi(argv[1]);
	if (argc >= 3) iters = atoi(argv[2]);
        if (argc > 3) test_usage();

	if (maxthreads > TEST_MAXTHREADS || maxthreads < 1) {
	  printf("Threads must be between 1 and %i\n", TEST_MAXTHREADS);
	  gasnet_exit(-1);
	}
	maxthreads = test_thread_limit(maxthreads);
        if (gasnet_nodes() % 2 != 0) {
    	  MSG("WARNING: This test requires an even number of nodes. Test skipped.\n");
    	  gasnet_exit(0); /* exit 0 to prevent false negatives in test harnesses for smp-conduit */
        }
        if (gasnet_mynode() == 0) {
          MSG("Running testcontend with 1..%i threads and %i iterations", maxthreads, iters);
        }
        tcountentries = 3 * maxthreads;
        tcount = test_malloc(tcountentries * sizeof(threadcnt_t));
        ptcount = tcount;
        for (i = 1; i <= maxthreads; i++) { ptcount->activecnt = i; ptcount->passivecnt = 1; ptcount++; }
        for (i = 1; i <= maxthreads; i++) { ptcount->activecnt = 1; ptcount->passivecnt = i; ptcount++; }
        for (i = 1; i <= maxthreads; i++) { ptcount->activecnt = i; ptcount->passivecnt = i; ptcount++; }
        peer = gasnet_mynode() ^ 1;
        amactive = (gasnet_mynode() % 2 == 0);

        peerseg = TEST_SEG(peer);

        /* create all worker threads */
        threads = maxthreads;
        test_createandjoin_pthreads(maxthreads, &workerthread, NULL, 0);

        BARRIER();
	if (gasnet_mynode() == 0) MSG("Tests complete");
        BARRIER();

	gasnet_exit(0);

	return 0;
}

/****************************************************************/
/* AM Handlers */
void ping_shorthandler(gasnet_token_t token) {
  GASNET_Safe(gasnet_AMReplyShort0(token, hidx_pong_shorthandler));
}

void pong_shorthandler(gasnet_token_t token) {
  gasnett_atomic_increment(&pong,0);
}

void ping_medhandler(gasnet_token_t token, void *buf, size_t nbytes) {
  GASNET_Safe(gasnet_AMReplyMedium0(token, hidx_pong_medhandler, buf, nbytes));
}

void pong_medhandler(gasnet_token_t token, void *buf, size_t nbytes) {
  gasnett_atomic_increment(&pong,0);
}

void ping_longhandler(gasnet_token_t token, void *buf, size_t nbytes) {
  GASNET_Safe(gasnet_AMReplyLong0(token, hidx_pong_longhandler, buf, nbytes, peerseg));
}

void pong_longhandler(gasnet_token_t token, void *buf, size_t nbytes) {
  gasnett_atomic_increment(&pong,0);
}

void noop_shorthandler(gasnet_token_t token) {
}

void markdone_shorthandler(gasnet_token_t token) {
  signal_done = 1;
}

