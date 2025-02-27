/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testexit.c $
 * Description: GASNet gasnet_exit correctness test
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>
#include <gasnet_tools.h>

#include <test.h>
#include <signal.h>

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

int mynode, nodes;
int peer = -1;
int testid = 0;
int use_allnodes = 0;
int use_threads = 0;
int numpthreads = 4;
volatile int signal_bt = 0;
#define thread_barrier() PTHREAD_BARRIER(numpthreads)

/* test various modes of exiting a GASNet program.
   Basically, none of these should hang or leave orphaned processes.
   Those with non-collective exit should cause the SIGQUIT handler to 
    fire on the non-exiting nodes.
*/
const char *testdesc_seq[] = {
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
  "non-collective gasnet_exit(14) following PrepareRequestMedium()",
  "non-collective gasnet_exit(15) following PrepareReplyMedium()",
};
#define NUMTEST_SEQ (sizeof(testdesc_seq)/sizeof(char*))
#define TESTBASE_SEQ 1

const char *testdesc_par[] = {
#ifdef GASNET_PAR
  "collective gasnet_exit(51) from all pthreads on all nodes",
  "non-collective gasnet_exit(52) from one pthread, other local in barrier",
  "non-collective gasnet_exit(53) from one pthread, others in spin-loop",
  "non-collective gasnet_exit(54) from one pthread, others in poll-loop",
  "non-collective gasnet_exit(55) from one pthread, others sending messages",
  #define NUMTEST_PAR (sizeof(testdesc_par)/sizeof(char*))
#else
  NULL
  #define NUMTEST_PAR 0
#endif
};
#define TESTBASE_PAR 51

const char *testid2desc(int testid) {
  unsigned int maybe_seq = testid - TESTBASE_SEQ;
  if (maybe_seq < NUMTEST_SEQ) return testdesc_seq[maybe_seq];
  unsigned int maybe_par = testid - TESTBASE_PAR;
  if (maybe_par < NUMTEST_PAR) return testdesc_par[maybe_par];
  return "ERROR";
}

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
void do_crash_test(int crashid);

static char *peerseg;
static char *replyseg;

#define hidx_exit_handler		201
#define hidx_noop_handler               202
#define hidx_ping_handler               203
#define hidx_npam_handler               204

void test_exit_handler(gex_Token_t token, gex_AM_Arg_t exitcode) {
  gasnet_exit((int)exitcode);
}

void ping_handler(gex_Token_t token, void *buf, size_t nbytes) {
  static int x = 1; 
  x = !x;/* harmless race */
  if (x) 
    gex_AM_ReplyMedium0(token, hidx_noop_handler, buf, nbytes, GEX_EVENT_NOW, 0);
  else
    gex_AM_ReplyLong0(token, hidx_noop_handler, buf, nbytes, replyseg, GEX_EVENT_NOW, 0);
}

void noop_handler(gex_Token_t token, void *buf, size_t nbytes) {
}

void npam_handler(gex_Token_t token, gex_AM_Arg_t exitcode) {
 gex_AM_SrcDesc_t sd;
 sd = gex_AM_PrepareReplyMedium(token, NULL, 0, 0, NULL, 0, 0);
 if (mynode == nodes-1) { gasnet_exit(exitcode); }
 gex_AM_CommitReplyMedium0(sd, hidx_noop_handler, 0);
}

#ifdef GASNET_PAR
void *workerthread(void *args) {
  int mythread = (int)(intptr_t)args;
  thread_barrier();
  switch (testid) {
    case 51:
      gasnet_exit(testid);
      break;
    case 52:
      if (mynode == 0) {
        if (mythread == 0) { 
          sleep(1); 
          gasnet_exit(testid); 
          MSG("TEST FAILED!!");
        } else if (mythread == 1) BARRIER();
      } else {
        if (mythread == 0) while(1) GASNET_Safe(gasnet_AMPoll());
      }
      while(1) ;
      break;
    case 53:
      if (mynode == 0 && mythread == 0) { 
          sleep(1); 
          gasnet_exit(testid); 
      } else while(1);
      break;
    case 54:
      if (mynode == 0 && mythread == 0) { 
          sleep(1); 
          gasnet_exit(testid); 
      } else while(1) GASNET_Safe(gasnet_AMPoll());
      break;
    case 55:
      if (mynode == 0 && mythread == 0) { 
          sleep(1); 
          gasnet_exit(testid); 
      } else {
        int junk = 42;
        int lim = MIN(MIN(MIN(MIN(
                        gex_AM_MaxRequestMedium(myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,0),
                        gex_AM_MaxReplyMedium  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,0)),
                        gex_AM_MaxRequestLong  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,0)),
                        gex_AM_MaxReplyLong    (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,0)),
                        (TEST_SEGSZ / 2));
        char *p = malloc(lim);
        while (1) {
          switch (rand() % 18) {
            case 0:  GASNET_Safe(gasnet_AMPoll()); break;
            case 1:  gex_AM_RequestMedium0(myteam, peer, hidx_noop_handler, p, 4, GEX_EVENT_NOW, 0); break;
            case 2:  gex_AM_RequestMedium0(myteam, peer, hidx_ping_handler, p, 4, GEX_EVENT_NOW, 0); break;
            case 3:  gex_AM_RequestMedium0(myteam, peer, hidx_noop_handler, p, lim, GEX_EVENT_NOW, 0); break;
            case 4:  gex_AM_RequestMedium0(myteam, peer, hidx_ping_handler, p, lim, GEX_EVENT_NOW, 0); break;
            case 5:  gex_AM_RequestLong0(myteam, peer, hidx_noop_handler, p, 4, peerseg, GEX_EVENT_NOW, 0); break;
            case 6:  gex_AM_RequestLong0(myteam, peer, hidx_ping_handler, p, 4, peerseg, GEX_EVENT_NOW, 0); break;
            case 7:  gex_AM_RequestLong0(myteam, peer, hidx_noop_handler, p, lim, peerseg, GEX_EVENT_NOW, 0); break;
            case 8:  gex_AM_RequestLong0(myteam, peer, hidx_ping_handler, p, lim, peerseg, GEX_EVENT_NOW, 0); break;
            case 9:  gex_RMA_PutBlocking(myteam, peer, peerseg, &junk, sizeof(int), 0); break;
            case 10: gex_RMA_GetBlocking(myteam, &junk, peer, peerseg, sizeof(int), 0); break;
            case 11: gex_RMA_PutBlocking(myteam, peer, peerseg, p, lim, 0); break;
            case 12: gex_RMA_GetBlocking(myteam, p, peer, peerseg, lim, 0); break;
            case 13: gex_RMA_PutNBI(myteam, peer, peerseg, &junk, sizeof(int), GEX_EVENT_NOW, 0); break;
            case 14: gex_RMA_GetNBI(myteam, &junk, peer, peerseg, sizeof(int), 0); break;
            case 15: gex_RMA_PutNBI(myteam, peer, peerseg, p, lim, GEX_EVENT_NOW, 0); break;
            case 16: gex_RMA_GetNBI(myteam, p, peer, peerseg, lim, 0); break;
            case 17: gex_NBI_Wait(GEX_EC_ALL,0); break;
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
  static char usagestr[MAXLINE*(NUMTEST_SEQ+NUMTEST_PAR+NUMCRASHTEST)];
  char testdescstr[MAXLINE];
  gex_AM_Entry_t htable[] = { 
    { hidx_exit_handler, test_exit_handler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 1 },
    { hidx_ping_handler, ping_handler,      GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDLONG, 0 },
    { hidx_noop_handler, noop_handler,      GEX_FLAG_AM_REQREP|GEX_FLAG_AM_MEDLONG, 0 },
    { hidx_npam_handler, npam_handler,      GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 1 },
  };

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testexit", &argc, &argv, 0));
  { int i = 200+NUMCRASHTEST;
    #ifdef GASNET_PAR
      i += 200;
      snprintf(usagestr,sizeof(usagestr),
             "[-r] (exittestnum:%i..%i,%i..%i | crashtestnum:100..%i) (num_pthreads)",
             (int)TESTBASE_SEQ, (int)(TESTBASE_SEQ+NUMTEST_SEQ-1),
             (int)TESTBASE_PAR, (int)(TESTBASE_PAR+NUMTEST_PAR-1),
             i-1);
    #else
      snprintf(usagestr,sizeof(usagestr),
             "[-r] (exittestnum:%i..%i | crashtestnum:100..%i)",
             (int)TESTBASE_SEQ, (int)(TESTBASE_SEQ+NUMTEST_SEQ-1),
             i-1);
    #endif
    strcat(usagestr, "\n  -r: reverse the node numbering");
    strcat(usagestr, "\n\n Exit tests:\n");
    for (i = 0; i < NUMTEST_SEQ; i++) {
      char tmp[MAXLINE];
      snprintf(tmp,MAXLINE,"  %3i: %s\n", i+TESTBASE_SEQ, testdesc_seq[i]);
      strcat(usagestr, tmp);
    }
    for (i = 0; i < NUMTEST_PAR; i++) {
      char tmp[MAXLINE];
      snprintf(tmp,MAXLINE,"  %3i: %s\n", i+TESTBASE_PAR, testdesc_par[i]);
      strcat(usagestr, tmp);
    }
    strcat(usagestr, "\n Crash tests: (add 100 to activate all nodes");
    #ifdef GASNET_PAR
      strcat(usagestr,", add 200 to use multiple threads");
    #endif
    strcat(usagestr, ")\n");
    for (i = 0; i < NUMCRASHTEST; i++) {
      char tmp[MAXLINE];
      snprintf(tmp,MAXLINE,"  %3i: %s\n", i+100, crashtestdesc[i]);
      strcat(usagestr, tmp);
    }
  }
  test_init_early("testexit",0,usagestr);

  mynode = gex_TM_QueryRank(myteam);
  nodes = gex_TM_QuerySize(myteam);

  argv++; argc--;
  if (argc > 0 && !strcmp(*argv, "-r")) { mynode = nodes-(mynode+1); argv++; argc--; }
  if (argc > 0) { testid = atoi(*argv); argv++; argc--; }
  #ifdef GASNET_PAR
    if (argc > 0) { numpthreads = atoi(*argv); argv++; argc--; }
    numpthreads = test_thread_limit(numpthreads);
  #endif
  unsigned int maybe_seq = testid - TESTBASE_SEQ;
  unsigned int maybe_par = testid - TESTBASE_PAR;
  if (argc > 0 || testid <= 0 || 
      ((testid < 100) && (maybe_seq >= NUMTEST_SEQ) && (maybe_par >= NUMTEST_PAR)) ||
      numpthreads <= 1) test_usage_early();

  peer = (mynode + 1) % nodes;

  if (testid < 100) {
    snprintf(testdescstr, sizeof(testdescstr), "Running exit test %i: %s",testid, testid2desc(testid));
  } else {
    int dispid = testid;
    const char *thread = "", *node = "";
    #ifdef GASNET_PAR
      if (testid >= 300) {
        thread = ", from one pthread w/others in thread barrier";
        testid -= 200; use_threads = 1;
      }
    #endif
    if (testid >= 200) {
      node = ", with all nodes active";
      testid -= 100; use_allnodes = 1;
    }
    if (testid < 100 || testid >= 100+NUMCRASHTEST) test_usage_early();
    snprintf(testdescstr, sizeof(testdescstr), "Running crash test %i: %s%s%s",dispid, 
      crashtestdesc[testid-100],thread,node);
  }
  if (testid == 6 || testid == 7) {
    PUTS0(testdescstr);
    gasnett_sched_yield();
    sleep(1);
    if (testid == 6) {
      gasnet_exit(testid);
      FATALERR("gasnet_exit failed");
    } else if (testid == 7 && mynode == nodes - 1) {
      gasnet_exit(testid);
      FATALERR("gasnet_exit failed");
    }
  }

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, sizeof(htable)/sizeof(gex_AM_Entry_t)));

  /* register a SIGQUIT handler, as permitted by GASNet spec */
  gasnett_reghandler(SIGQUIT, testSignalHandler);

  peerseg = TEST_SEG(peer);
  replyseg = (char *)TEST_SEG((mynode + nodes - 1) % nodes) + (TEST_SEGSZ / 2);

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
      gex_AM_RequestShort1(myteam, peer, hidx_exit_handler, 0, testid);
      while(1) GASNET_Safe(gasnet_AMPoll());
      break;
    case 11:
      if (mynode == 0) { 
        gex_AM_RequestShort1(myteam, nodes-1, hidx_exit_handler, 0, testid);
      }
      while(1) GASNET_Safe(gasnet_AMPoll());
      break;
    case 12:
      if (mynode == nodes-1) { 
        gex_AM_RequestShort1(myteam, mynode, hidx_exit_handler, 0, testid);
      }
      while(1) GASNET_Safe(gasnet_AMPoll());
      break;
    case 13:
      gex_AM_RequestShort1(myteam, nodes-1, hidx_exit_handler, 0, testid);
      while(1) GASNET_Safe(gasnet_AMPoll());
      break;
    case 14: {
      gex_AM_SrcDesc_t sd;
      sd = gex_AM_PrepareRequestMedium(myteam, peer, NULL, 0, 0, NULL, 0, 0);
      if (mynode == nodes-1) { gasnet_exit(testid); }
      gex_AM_CommitRequestMedium0(sd, hidx_noop_handler, 0);
      while(1) GASNET_Safe(gasnet_AMPoll());
      break;
    }
    case 15:
      gex_AM_RequestShort1(myteam, peer, hidx_npam_handler, 0, testid);
      while(1) GASNET_Safe(gasnet_AMPoll());
      break;
  default: 
    #ifdef GASNET_PAR
      if ((testid >= TESTBASE_PAR) && (testid < TESTBASE_PAR + NUMTEST_PAR))  {
        test_createandjoin_pthreads(numpthreads, &workerthread, NULL, 0);
      }
    #endif
      if (testid >= 100 && testid < 100+NUMCRASHTEST) {
      #ifdef GASNET_PAR
        if (use_threads) {
          test_createandjoin_pthreads(numpthreads, &workerthread, NULL, 0);
        } else
      #endif
        do_crash_test(testid);
      } else {
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
      if (use_allnodes || mynode == nodes-1) { sleep(1); gasnett_print_backtrace(STDERR_FILENO); }
      BARRIER();
      gasnet_exit(0);
      break;
    case 101:
      if (use_allnodes || mynode == nodes-1) { 
        sleep(1); 
        signal_bt = 1;
        kill(getpid(), SIGQUIT);
        while (signal_bt != 2) gasnett_sched_yield(); /* await delivery */
      } else BARRIER();
      BARRIER();
      gasnet_exit(0);
      break;
    case 102:
      if (use_allnodes || mynode == nodes-1) { 
        gasnett_fatalerror("Synthetic fatal error");
        FATALERR("gasnett_fatalerror FAILED!!");
      }
      BARRIER();
      break;
    case 103:
      if (use_allnodes || mynode == nodes-1) { 
        abort();
        FATALERR("abort() FAILED!!");
      }
      BARRIER();
      break;
    case 104:
      if (use_allnodes || mynode == nodes-1) { 
        static char volatile *p = NULL;
        *p = *p + 10;
        FATALERR("Failed to generate a segmentation fault");
      }
      BARRIER();
      break;
    case 105:
      if (use_allnodes || mynode == nodes-1) { 
        static uint64_t myarr[3];
        static char *p = ((char *)(myarr+1))+1;
        *(uint16_t volatile *)p = *(uint16_t volatile *)p + 10;
        *(uint32_t volatile *)p = *(uint32_t volatile *)p + 10;
        *(uint64_t volatile *)p = *(uint64_t volatile *)p + 10;
        *(float volatile *)p = *(float volatile *)p + 10;
        *(double volatile *)p = *(double volatile *)p + 10;
        gasnett_sched_yield();
        sleep(1);
        MSG("Failed to generate implicit SIGBUS, trying raise(SIGBUS)..");
        raise(SIGBUS);
        gasnett_sched_yield();
        sleep(1);
        MSG("Failed to generate implicit SIGBUS, trying kill(%li,SIGBUS)..",(long)getpid());
        kill(getpid(), SIGBUS);
        gasnett_sched_yield();
        sleep(1);
        FATALERR("Failed to generate a bus error");
      }
      BARRIER();
      break;
    case 106:
      if (use_allnodes || mynode == nodes-1) { 
        static double volatile d = 0.0;
        static int volatile i = 0;
        d = 16.0 / d;
        d = 1.0E30;
        for (;i < 100;i++) d *= d;
        d = 1.0;
        for (;i < 1000;i++) d /= 1.0E30;
        i = 16 / i;
        gasnett_sched_yield();
        sleep(1);
        MSG("Failed to generate implicit SIGFPE, trying raise(SIGFPE)..");
        raise(SIGFPE);
        gasnett_sched_yield();
        sleep(1);
        MSG("Failed to generate implicit SIGFPE, trying kill(%li,SIGFPE)..",(long)getpid());
        kill(getpid(), SIGFPE);
        gasnett_sched_yield();
        sleep(1);
        FATALERR("Failed to generate a floating-point exception");
      }
      BARRIER();
      break;
    default: FATALERR("bad test id: %i", crashid);
  }
}
