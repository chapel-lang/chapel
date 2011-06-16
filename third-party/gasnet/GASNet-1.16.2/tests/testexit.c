/*   $Source: /var/local/cvs/gasnet/tests/testexit.c,v $
 *     $Date: 2009/10/04 00:33:27 $
 * $Revision: 1.29 $
 * Description: GASNet gasnet_exit correctness test
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet.h>
#include <gasnet_tools.h>

#include <test.h>
#include <signal.h>

int mynode, nodes;
int peer = -1;
int testid = 0;
int numpthreads = 4;
volatile int signal_bt = 0;
#define thread_barrier() PTHREAD_BARRIER(numpthreads)

/* test various modes of exiting a GASNet program.
   Basically, none of these should hang or leave orphaned processes.
   Those with non-collective exit should cause the SIGQUIT handler to 
    fire on the non-exiting nodes.
*/
const char *testdesc[] = {
  "simultaneous collective gasnet_exit(1)",
  "simultaneous return from main()... exit_code 2",
  "non-collective gasnet_exit(3), others in barrier",
  "non-collective SIGINT, others in barrier ... exit_code 4",
  "non-collective gasnet_exit(5), others in spin-loop",
  "collective gasnet_exit(6) between init()/attach()",
  "non-collective gasnet_exit(7) between init()/attach()",
  "non-collective return(8) from main(), others in barrier",
  "non-collective return(9) from main(), others in spin-loop",
  "collective gasnet_exit(10) from AM handlers on all nodes",
  "non-collective gasnet_exit(11) from AM handler on one node",
  "non-collective gasnet_exit(12) from AM handler on one node (loopback)",
  "non-collective gasnet_exit(13) from AM handler on one node (N requests)",
#ifdef GASNET_PAR
  "collective gasnet_exit(14) from all pthreads on all nodes",
  "non-collective gasnet_exit(15) from one pthread, other local in barrier",
  "non-collective gasnet_exit(16) from one pthread, others in spin-loop",
  "non-collective gasnet_exit(17) from one pthread, others in poll-loop",
  "non-collective gasnet_exit(18) from one pthread, others sending messages",
#endif
};
#define NUMTEST (sizeof(testdesc)/sizeof(char*))

const char *crashtestdesc[] = {
  "gasnett_print_backtrace() from main",
  "gasnett_print_backtrace() from SIGQUIT handler",
  "gasnett_fatalerror()",
  "abort()",
  "segmentation fault",
  "bus error",
  "floating-point exception"
};
#define NUMCRASHTEST (sizeof(crashtestdesc)/sizeof(char*))
#ifdef GASNET_PAR
#define NUMCRASHTEST_WITH_PAR (NUMCRASHTEST*2)
#else
#define NUMCRASHTEST_WITH_PAR NUMCRASHTEST
#endif
void do_crash_test(int crashid);

#define hidx_exit_handler		201
#define hidx_noop_handler               202
#define hidx_ping_handler               203

void test_exit_handler(gasnet_token_t token, gasnet_handlerarg_t exitcode) {
  gasnet_exit((int)exitcode);
}

void ping_handler(gasnet_token_t token, void *buf, size_t nbytes) {
  static int x = 1; 
  gasnet_node_t src;
  gasnet_AMGetMsgSource(token, &src);
  x = !x;/* harmless race */
  if (x) 
    GASNET_Safe(gasnet_AMReplyMedium0(token, hidx_noop_handler, buf, nbytes));
  else
    GASNET_Safe(gasnet_AMReplyLong0(token, hidx_noop_handler, buf, nbytes, TEST_SEG(src)));
}

void noop_handler(gasnet_token_t token, void *buf, size_t nbytes) {
}

#ifdef GASNET_PAR
void *workerthread(void *args) {
  int mythread = (int)(intptr_t)args;
  thread_barrier();
  switch (testid) {
    case 14:
      gasnet_exit(14);
      break;
    case 15:
      if (mynode == 0) {
        if (mythread == 0) { 
          sleep(1); 
          gasnet_exit(15); 
          MSG("TEST FAILED!!");
        } else if (mythread == 1) BARRIER();
      } else {
        if (mythread == 0) while(1) GASNET_Safe(gasnet_AMPoll());
      }
      while(1) ;
      break;
    case 16:
      if (mynode == 0 && mythread == 0) { 
          sleep(1); 
          gasnet_exit(16); 
      } else while(1);
      break;
    case 17:
      if (mynode == 0 && mythread == 0) { 
          sleep(1); 
          gasnet_exit(17); 
      } else while(1) GASNET_Safe(gasnet_AMPoll());
      break;
    case 18:
      if (mynode == 0 && mythread == 0) { 
          sleep(1); 
          gasnet_exit(18); 
      } else {
        int junk;
        int lim = MIN(MIN(MIN(gasnet_AMMaxMedium(), gasnet_AMMaxLongRequest()), gasnet_AMMaxLongReply()), TEST_SEGSZ);
        char *p = malloc(lim);
        char *peerseg = TEST_SEG(peer);
        while (1) {
          switch (rand() % 18) {
            case 0:  GASNET_Safe(gasnet_AMPoll()); break;
            case 1:  GASNET_Safe(gasnet_AMRequestMedium0(peer, hidx_noop_handler, p, 4)); break;
            case 2:  GASNET_Safe(gasnet_AMRequestMedium0(peer, hidx_ping_handler, p, 4)); break;
            case 3:  GASNET_Safe(gasnet_AMRequestMedium0(peer, hidx_noop_handler, p, lim)); break;
            case 4:  GASNET_Safe(gasnet_AMRequestMedium0(peer, hidx_ping_handler, p, lim)); break;
            case 5:  GASNET_Safe(gasnet_AMRequestLong0(peer, hidx_noop_handler, p, 4, peerseg)); break;
            case 6:  GASNET_Safe(gasnet_AMRequestLong0(peer, hidx_ping_handler, p, 4, peerseg)); break;
            case 7:  GASNET_Safe(gasnet_AMRequestLong0(peer, hidx_noop_handler, p, lim, peerseg)); break;
            case 8:  GASNET_Safe(gasnet_AMRequestLong0(peer, hidx_ping_handler, p, lim, peerseg)); break;
            case 9:  gasnet_put(peer, peerseg, &junk, sizeof(int)); break;
            case 10: gasnet_get(&junk, peer, peerseg, sizeof(int)); break;
            case 11: gasnet_put(peer, peerseg, p, lim); break;
            case 12: gasnet_get(p, peer, peerseg, lim); break;
            case 13: gasnet_put_nbi(peer, peerseg, &junk, sizeof(int)); break;
            case 14: gasnet_get_nbi(&junk, peer, peerseg, sizeof(int)); break;
            case 15: gasnet_put_nbi(peer, peerseg, p, lim); break;
            case 16: gasnet_get_nbi(p, peer, peerseg, lim); break;
            case 17: gasnet_wait_syncnbi_all(); break;
          }
        }
      }
      break;
    default:
      if (testid >= 100 && testid < 100+NUMCRASHTEST) {
        if (mythread == 0) do_crash_test(testid);
        thread_barrier();
      } else {
        FATALERR("bad test id");
      }
  }

  /* if we ever reach here, something really bad happenned */
  FATALERR("TEST FAILED!!");
  return NULL;
}
#endif

typedef void (*test_sighandlerfn_t)(int);
void testSignalHandler(int sig) {
  if (sig != SIGQUIT) {
    FATALERR("got an unexpected signal!");
  } else if (signal_bt == 1) {
      gasnett_print_backtrace(STDERR_FILENO);
      signal_bt = 2;
      return;
  } else {
    MSG("in SIGQUIT handler, calling gasnet_exit(4)...");
    gasnet_exit(4);
  }
}

int main(int argc, char **argv) {
  #define MAXLINE 255
  static char usagestr[MAXLINE*(NUMTEST+NUMCRASHTEST_WITH_PAR)];
  char testdescstr[MAXLINE];
  gasnet_handlerentry_t htable[] = { 
    { hidx_exit_handler, test_exit_handler },
    { hidx_ping_handler, ping_handler },
    { hidx_noop_handler, noop_handler },
  };

  GASNET_Safe(gasnet_init(&argc, &argv));
  { int i;
    sprintf(usagestr,"(exittestnum:1..%i | crashtestnum:100..%i)", (int)NUMTEST, (int)(100+NUMCRASHTEST_WITH_PAR-1));
    #ifdef GASNET_PAR
      strcat(usagestr, " (num_pthreads)");
    #endif
    strcat(usagestr, "\n\n Exit tests:\n");
    for (i = 0; i < NUMTEST; i++) {
      char tmp[MAXLINE];
      snprintf(tmp,MAXLINE,"  %3i: %s\n", i+1, testdesc[i]);
      strcat(usagestr, tmp);
    }
    strcat(usagestr, "\n Crash tests:\n");
    for (i = 0; i < NUMCRASHTEST; i++) {
      char tmp[MAXLINE];
      snprintf(tmp,MAXLINE,"  %3i: %s\n", i+100, crashtestdesc[i]);
      strcat(usagestr, tmp);
    }
  #ifdef GASNET_PAR
    for (i = 0; i < NUMCRASHTEST; i++) {
      char tmp[MAXLINE];
      snprintf(tmp,MAXLINE,"  %3i: %s from one pthread, others in thread barrier\n", 
                   (int)(i+100+NUMCRASHTEST), crashtestdesc[i]);
      strcat(usagestr, tmp);
    }
  #endif
  }
  test_init_early("testexit",0,usagestr);
  MSG("hostname is: %s (pid=%i)", gasnett_gethostname(), (int)getpid());

  mynode = gasnet_mynode();
  nodes = gasnet_nodes();

  peer = mynode ^ 1;
  if (peer == nodes) {
    /* w/ odd # of nodes, last one talks to self */
    peer = mynode;
  }

  argv++; argc--;
  if (argc > 0) { testid = atoi(*argv); argv++; argc--; }
  #ifdef GASNET_PAR
    if (argc > 0) { numpthreads = atoi(*argv); argv++; argc--; }
  #endif
  if (argc > 0 || testid <= 0 || 
      (testid > NUMTEST && testid < 100) || 
      (testid >= 100+NUMCRASHTEST_WITH_PAR) || 
      numpthreads <= 1) test_usage_early();

  if (testid < 100) {
    sprintf(testdescstr, "Running exit test %i: %s",testid, testdesc[testid-1]);
  } else if (testid-100<NUMCRASHTEST) {
    sprintf(testdescstr, "Running crash test %i: %s",testid, 
            crashtestdesc[testid-100]);
  } else {
    sprintf(testdescstr, "Running crash test %i: %s from one pthread, others in thread barrier",testid, 
      crashtestdesc[testid-100-NUMCRASHTEST]);
  }
  if (testid == 6 || testid == 7) {
    PUTS0(testdescstr);
    gasnett_sched_yield();
    sleep(1);
    if (testid == 6) {
      gasnet_exit(6);
      FATALERR("gasnet_exit failed");
    } else if (testid == 7 && mynode == nodes - 1) {
      gasnet_exit(7);
      FATALERR("gasnet_exit failed");
    }
  }

  GASNET_Safe(gasnet_attach(htable,  sizeof(htable)/sizeof(gasnet_handlerentry_t),
	                    TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));

  /* register a SIGQUIT handler, as permitted by GASNet spec */
  gasnett_reghandler(SIGQUIT, testSignalHandler);

  TEST_SEG(mynode);

  BARRIER();
  PUTS0(testdescstr);
  BARRIER();

  switch (testid) {
    case 1: 
      gasnet_exit(testid);
      break;
    case 2: 
      return testid;
      break;
    case 3: 
      if (mynode == nodes-1) { sleep(1); gasnet_exit(testid); }
      else BARRIER();
      break;
    case 4: 
      if (mynode == nodes-1) { 
        sleep(1); 
        /*raise(SIGINT); */
        kill(getpid(), SIGINT); /* more reliable */
        while (1) gasnett_sched_yield(); /* await delivery */
      } else BARRIER();
      break;
    case 5: 
      if (mynode == nodes-1) { sleep(1); gasnet_exit(testid); }
      else while(1);
      break;
    case 8: 
      if (mynode == nodes-1) { sleep(1); return testid; }
      else BARRIER();
      break;
    case 9: 
      if (mynode == nodes-1) { sleep(1); return testid; }
      else while(1);
      break;
    case 10:
      GASNET_Safe(gasnet_AMRequestShort1(peer, hidx_exit_handler, testid));
      while(1) GASNET_Safe(gasnet_AMPoll());
      break;
    case 11:
      if (mynode == 0) { 
        GASNET_Safe(gasnet_AMRequestShort1(nodes-1, hidx_exit_handler, testid));
      }
      while(1) GASNET_Safe(gasnet_AMPoll());
      break;
    case 12:
      if (mynode == nodes-1) { 
        GASNET_Safe(gasnet_AMRequestShort1(mynode, hidx_exit_handler, testid));
      }
      while(1) GASNET_Safe(gasnet_AMPoll());
      break;
    case 13:
      GASNET_Safe(gasnet_AMRequestShort1(nodes-1, hidx_exit_handler, testid));
      while(1) GASNET_Safe(gasnet_AMPoll());
      break;
  #ifdef GASNET_PAR
    case 14: case 15: case 16: case 17: case 18: {
      test_createandjoin_pthreads(numpthreads, &workerthread, NULL, 0);
      break;
    }
  #endif
  default: 
      if (testid >= 100 && testid < 100+NUMCRASHTEST) {
        do_crash_test(testid);
      }
    #ifdef GASNET_PAR
      else if (testid >= 100+NUMCRASHTEST && testid < 100+2*NUMCRASHTEST) {
        testid -= NUMCRASHTEST;
        test_createandjoin_pthreads(numpthreads, &workerthread, NULL, 0);
      }
    #endif
      else {
        FATALERR("bad test id: %i", testid);
      }
  }

  /* if we ever reach here, something really bad happenned */
  FATALERR("TEST FAILED!!");
  return 0;
}

void do_crash_test(int crashid) {
  switch(crashid) {
    case 100:
      if (mynode == nodes-1) { sleep(1); gasnett_print_backtrace(STDERR_FILENO); }
      BARRIER();
      gasnet_exit(0);
      break;
    case 101:
      if (mynode == nodes-1) { 
        sleep(1); 
        signal_bt = 1;
        kill(getpid(), SIGQUIT);
        while (signal_bt != 2) gasnett_sched_yield(); /* await delivery */
      } else BARRIER();
      BARRIER();
      gasnet_exit(0);
      break;
    case 102:
      if (mynode == nodes-1) { 
        gasnett_fatalerror("Synthetic fatal error");
        FATALERR("gasnett_fatalerror FAILED!!");
      }
      BARRIER();
      break;
    case 103:
      if (mynode == nodes-1) { 
        abort();
        FATALERR("abort() FAILED!!");
      }
      BARRIER();
      break;
    case 104:
      if (mynode == nodes-1) { 
        static char volatile *p = NULL;
        *p = *p + 10;
        FATALERR("Failed to generate a segmentation fault");
      }
      BARRIER();
      break;
    case 105:
      if (mynode == nodes-1) { 
        static uint64_t myarr[3];
        static char *p = ((char *)(myarr+1))+1;
        *(uint16_t volatile *)p = *(uint16_t volatile *)p + 10;
        *(uint32_t volatile *)p = *(uint32_t volatile *)p + 10;
        *(uint64_t volatile *)p = *(uint64_t volatile *)p + 10;
        *(float volatile *)p = *(float volatile *)p + 10;
        *(double volatile *)p = *(double volatile *)p + 10;
        gasnett_sched_yield();
        kill(getpid(), SIGBUS);
        gasnett_sched_yield();
        FATALERR("Failed to generate a bus error");
      }
      BARRIER();
      break;
    case 106:
      if (mynode == nodes-1) { 
        static double volatile d = 0.0;
        static int volatile i = 0;
        d = 16.0 / d;
        d = 1.0E30;
        for (;i < 100;i++) d *= d;
        d = 1.0;
        for (;i < 1000;i++) d /= 1.0E30;
        i = 16 / i;
        gasnett_sched_yield();
        kill(getpid(), SIGFPE);
        gasnett_sched_yield();
        FATALERR("Failed to generate a floating-point exception");
      }
      BARRIER();
      break;
    default: FATALERR("bad test id: %i", crashid);
  }
}
