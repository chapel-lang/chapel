/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testmisc.c $
 * Description: GASNet misc performance test
 *   Measures the overhead associated with a number of purely local 
 *   operations that involve no communication. 
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <test.h>

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

int mynode = 0;
int iters=0;
void *myseg = NULL;
int accuracy = 0;

void report(const char *desc, int64_t totaltime, int iters) {
  if (mynode == 0) {
      char format[80];
      snprintf(format, sizeof(format), "%c: %%-50s: %%%i.%if s  %%%i.%if us\n", 
              TEST_SECTION_NAME(), (4+accuracy), accuracy, (4+accuracy), accuracy);
      printf(format, desc, totaltime/1.0E9, (totaltime/1000.0)/iters);
      fflush(stdout);
  }
}

/* placed in a function to avoid excessive inlining */
gasnett_tick_t ticktime(void) { return gasnett_ticks_now(); }
uint64_t tickcvt(gasnett_tick_t ticks) { return gasnett_ticks_to_ns(ticks); }

void doit1(void);
void doit2(void);
void doit3(void);
void doit4(void);
void doit5(void);
void doit6(void);
void doit7(void);
void doit8(void);
/* ------------------------------------------------------------------------------------ */
#define hidx_null_shorthandler        201
#define hidx_justreply_shorthandler   202

#define hidx_null_medhandler          203
#define hidx_justreply_medhandler     204

#define hidx_null_longhandler         205
#define hidx_justreply_longhandler    206

void null_shorthandler(gex_Token_t token) {
}

void justreply_shorthandler(gex_Token_t token) {
  gex_AM_ReplyShort0(token, hidx_null_shorthandler, 0);
}

void null_medhandler(gex_Token_t token, void *buf, size_t nbytes) {
}

void justreply_medhandler(gex_Token_t token, void *buf, size_t nbytes) {
  gex_AM_ReplyMedium0(token, hidx_null_medhandler, buf, nbytes, GEX_EVENT_NOW, 0);
}

void null_longhandler(gex_Token_t token, void *buf, size_t nbytes) {
}

void justreply_longhandler(gex_Token_t token, void *buf, size_t nbytes) {
  gex_AM_ReplyLong0(token, hidx_null_longhandler, buf, nbytes, buf, GEX_EVENT_NOW, 0);
}
/* ------------------------------------------------------------------------------------ */
/* This tester measures the performance of a number of miscellaneous GASNet functions 
   that don't involve actual communication, to assist in evaluating the overhead of 
   the GASNet layer itself
 */
int main(int argc, char **argv) {
  gex_AM_Entry_t htable[] = { 
    { hidx_null_shorthandler,      null_shorthandler,      GEX_FLAG_AM_REQREP|GEX_FLAG_AM_SHORT, 0 },
    { hidx_justreply_shorthandler, justreply_shorthandler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 0 },
    { hidx_null_medhandler,        null_medhandler,        GEX_FLAG_AM_REQREP|GEX_FLAG_AM_MEDIUM, 0 },
    { hidx_justreply_medhandler,   justreply_medhandler,   GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDIUM, 0 },
    { hidx_null_longhandler,       null_longhandler,       GEX_FLAG_AM_REQREP|GEX_FLAG_AM_LONG, 0 },
    { hidx_justreply_longhandler,  justreply_longhandler,  GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_LONG, 0 }
  };

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testmisc", &argc, &argv, 0));
  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, sizeof(htable)/sizeof(gex_AM_Entry_t)));
  test_init("testmisc",1,"(iters) (accuracy_digits) (test_sections)");

  mynode = gex_TM_QueryRank(myteam);
  myseg = TEST_MYSEG();

  if (argc > 1) iters = atoi(argv[1]);
  if (!iters) iters = 100000;

  if (argc > 2) accuracy = atoi(argv[2]);
  if (!accuracy) accuracy = 3;

  if (argc > 3) TEST_SECTION_PARSE(argv[3]);

  if (argc > 4) test_usage();

  if (mynode == 0) {
      printf("Running misc performance test with %i iterations...\n",iters);
      printf("%-50s    Total time    Avg. time\n"
             "%-50s    ----------    ---------\n", "", "");
      fflush(stdout);
  }

  doit1();
  MSG("done.");

  gasnet_exit(0);
  return 0;
}

#define TIME_OPERATION_FULL(desc, preop, op, postop)       \
  if (TEST_SECTION_ENABLED())                              \
  { int i, _iters = iters, _warmupiters = MAX(1,iters/10); \
    gasnett_tick_t start,end;  /* use ticks interface */   \
    BARRIER();                 /* for best accuracy */     \
    preop;       /* warm-up */                             \
    for (i=0; i < _warmupiters; i++) { op; }               \
    postop;                                                \
    BARRIER();                                             \
    start = ticktime();                                    \
    preop;                                                 \
    for (i=0; i < _iters; i++) { op; }                     \
    postop;                                                \
    end = ticktime();                                      \
    BARRIER();                                             \
    if (((const char *)(desc)) && ((char*)(desc))[0])      \
      report((desc), tickcvt(end - start), iters);         \
    else report("Line " _STRINGIFY(__LINE__), tickcvt(end - start), iters); \
  }
#define TIME_OPERATION(desc, op) TIME_OPERATION_FULL(desc, {}, op, {})

char p[1];
gex_HSL_t hsl = GEX_HSL_INITIALIZER;
gasnett_atomic_t a = gasnett_atomic_init(0);
gasnett_atomic32_t a32 = gasnett_atomic32_init(0);
gasnett_atomic64_t a64 = gasnett_atomic64_init(0);
int32_t temp = 0;
gasnett_tick_t timertemp = 0;
int8_t bigtemp[1024];
gex_Event_t events[8];
/* ------------------------------------------------------------------------------------ */
void doit1(void) { GASNET_BEGIN_FUNCTION();

    { int i; for (i=0;i<8;i++) {
        events[i] = GEX_EVENT_INVALID;
    } }

    TEST_SECTION_BEGIN();
    TIME_OPERATION("Tester overhead", {});
    
    TIME_OPERATION("gasnett_ticks_now()",
      { timertemp = gasnett_ticks_now(); });
    
    TIME_OPERATION("gasnett_ticks_to_us()",
      { timertemp = (gasnett_tick_t)gasnett_ticks_to_us(timertemp); });
    
    TIME_OPERATION("gasnett_ticks_to_ns()",
      { timertemp = (gasnett_tick_t)gasnett_ticks_to_ns(timertemp); });
    
    TEST_SECTION_BEGIN();
    TIME_OPERATION("Do-nothing gasnet_AMPoll()",
      { gasnet_AMPoll(); });
    
    TIME_OPERATION("Loopback do-nothing gex_AM_RequestShort0()",
      { gex_AM_RequestShort0(myteam, mynode, hidx_null_shorthandler, 0); });

    TIME_OPERATION("Loopback do nothing AM short request-reply",
      { gex_AM_RequestShort0(myteam, mynode, hidx_justreply_shorthandler, 0); });

    TIME_OPERATION("Loopback do-nothing gex_AM_RequestMedium0()",
      { gex_AM_RequestMedium0(myteam, mynode, hidx_null_medhandler, p, 0, GEX_EVENT_NOW, 0); });

    TIME_OPERATION("Loopback do nothing AM medium request-reply",
      { gex_AM_RequestMedium0(myteam, mynode, hidx_justreply_medhandler, p, 0, GEX_EVENT_NOW, 0); });

    TIME_OPERATION("Loopback do-nothing gex_AM_RequestLong0()",
      { gex_AM_RequestLong0(myteam, mynode, hidx_null_longhandler, p, 0, myseg, GEX_EVENT_NOW, 0); });

    TIME_OPERATION("Loopback do nothing AM long request-reply",
      { gex_AM_RequestLong0(myteam, mynode, hidx_justreply_longhandler, p, 0, myseg, GEX_EVENT_NOW, 0); });

    doit2();
}
/* ------------------------------------------------------------------------------------ */
volatile int val_true = 1;
volatile int val_false = 0;
int val_junk = 0;
void doit2(void) { GASNET_BEGIN_FUNCTION();

    TEST_SECTION_BEGIN();

    #if defined(GASNET_PAR) || defined (GASNET_PARSYNC)
      { static gasnett_mutex_t mutex = GASNETT_MUTEX_INITIALIZER;
        TIME_OPERATION("lock/unlock uncontended gasnet mutex",
          { gasnett_mutex_lock(&mutex); gasnett_mutex_unlock(&mutex); });
      }
      { static gasnett_rwlock_t rwlock = GASNETT_RWLOCK_INITIALIZER;
        TIME_OPERATION("rdlock/unlock uncontended gasnet rwlock",
          { gasnett_rwlock_rdlock(&rwlock); gasnett_rwlock_unlock(&rwlock); });
        TIME_OPERATION("wrlock/unlock uncontended gasnet rwlock",
          { gasnett_rwlock_wrlock(&rwlock); gasnett_rwlock_unlock(&rwlock); });
      }
    #endif

    TIME_OPERATION("lock/unlock uncontended HSL (" _STRINGIFY(TEST_PARSEQ) " mode)",
      { gex_HSL_Lock(&hsl); gex_HSL_Unlock(&hsl); });

    TEST_SECTION_BEGIN();
    #define MESSY(i) ((((i+14)*i)+(i+23)*i)&4)
    TIME_OPERATION("if_pf correct",
      { if_pf(val_false) val_false ^= MESSY(i); else val_junk++; });
    
    TIME_OPERATION("if_pf incorrect",
      { if_pf(val_true) val_junk++; else val_true ^= MESSY(i); });
    
    TIME_OPERATION("if_pt correct",
      { if_pt(val_true) val_junk++; else val_true ^= MESSY(i); });
    
    TIME_OPERATION("if_pt incorrect",
      { if_pt(val_false) val_false ^= MESSY(i); else val_junk++;});
    
    TEST_SECTION_BEGIN();
    TIME_OPERATION("gasnett_local_wmb", gasnett_local_wmb());
    TIME_OPERATION("gasnett_local_rmb", gasnett_local_rmb());
    TIME_OPERATION("gasnett_local_mb", gasnett_local_mb());

    TEST_SECTION_BEGIN();
    TIME_OPERATION("gasnett_atomic_read", gasnett_atomic_read(&a,0));
    TIME_OPERATION("gasnett_atomic_set", gasnett_atomic_set(&a,1,0));
    TIME_OPERATION("gasnett_atomic_increment", gasnett_atomic_increment(&a,0));
    TIME_OPERATION("gasnett_atomic_increment.rel", gasnett_atomic_increment(&a,GASNETT_ATOMIC_REL));
    TIME_OPERATION("gasnett_atomic_decrement", gasnett_atomic_decrement(&a,0));
    TIME_OPERATION("gasnett_atomic_decrement.acq", gasnett_atomic_decrement(&a,GASNETT_ATOMIC_ACQ));
    TIME_OPERATION("gasnett_atomic_decrement_and_test", gasnett_atomic_decrement_and_test(&a,0));
    TIME_OPERATION("gasnett_atomic_decrement_and_test.acq", gasnett_atomic_decrement_and_test(&a,GASNETT_ATOMIC_ACQ));

#if defined(GASNETT_HAVE_ATOMIC_CAS)
    TIME_OPERATION_FULL("gasnett_atomic_compare_and_swap (result=1)", { gasnett_atomic_set(&a,0,0); },
			{ gasnett_atomic_compare_and_swap(&a,0,0,0); }, {});
    TIME_OPERATION_FULL("gasnett_atomic_compare_and_swap.acq (result=1)", { gasnett_atomic_set(&a,0,0); },
			{ gasnett_atomic_compare_and_swap(&a,0,0,GASNETT_ATOMIC_ACQ); }, {});
    TIME_OPERATION_FULL("gasnett_atomic_compare_and_swap (result=0)", { gasnett_atomic_set(&a,1,0); },
			{ gasnett_atomic_compare_and_swap(&a,0,0,0); }, {});
    TIME_OPERATION_FULL("gasnett_atomic_compare_and_swap.acq (result=0)", { gasnett_atomic_set(&a,1,0); },
			{ gasnett_atomic_compare_and_swap(&a,0,0,GASNETT_ATOMIC_ACQ); }, {});
    TIME_OPERATION("gasnett_atomic_swap", gasnett_atomic_swap(&a,i,0));
    TIME_OPERATION("gasnett_atomic_swap.acq", gasnett_atomic_swap(&a,i,GASNETT_ATOMIC_ACQ));
    TIME_OPERATION("gasnett_atomic_swap.rel", gasnett_atomic_swap(&a,i,GASNETT_ATOMIC_REL));
#endif

#if defined(GASNETT_HAVE_ATOMIC_ADD_SUB)
    TIME_OPERATION("gasnett_atomic_add", gasnett_atomic_add(&a,i,0));
    TIME_OPERATION("gasnett_atomic_add.rel", gasnett_atomic_add(&a,i,GASNETT_ATOMIC_REL));
    TIME_OPERATION("gasnett_atomic_subtract", gasnett_atomic_subtract(&a,i,0));
    TIME_OPERATION("gasnett_atomic_subtract.acq", gasnett_atomic_subtract(&a,i,GASNETT_ATOMIC_ACQ));
#endif

    TEST_SECTION_BEGIN();
    TIME_OPERATION("gasnett_atomic32_read", gasnett_atomic32_read(&a32,0));
    TIME_OPERATION("gasnett_atomic32_set", gasnett_atomic32_set(&a32,1,0));
    TIME_OPERATION("gasnett_atomic32_increment", gasnett_atomic32_increment(&a32,0));
    TIME_OPERATION("gasnett_atomic32_decrement", gasnett_atomic32_decrement(&a32,0));
    TIME_OPERATION("gasnett_atomic32_decrement_and_test", gasnett_atomic32_decrement_and_test(&a32,0));
    TIME_OPERATION_FULL("gasnett_atomic32_compare_and_swap (result=1)", { gasnett_atomic32_set(&a32,0,0); },
			{ gasnett_atomic32_compare_and_swap(&a32,0,0,0); }, {});
    TIME_OPERATION_FULL("gasnett_atomic32_compare_and_swap (result=0)", { gasnett_atomic32_set(&a32,1,0); },
			{ gasnett_atomic32_compare_and_swap(&a32,0,0,0); }, {});
    TIME_OPERATION("gasnett_atomic32_swap", gasnett_atomic32_swap(&a32,i,0));
    TIME_OPERATION("gasnett_atomic32_add", gasnett_atomic32_add(&a32,i,0));
    TIME_OPERATION("gasnett_atomic32_subtract", gasnett_atomic32_subtract(&a32,i,0));

    TEST_SECTION_BEGIN();
    TIME_OPERATION("gasnett_atomic64_read", gasnett_atomic64_read(&a64,0));
    TIME_OPERATION("gasnett_atomic64_set", gasnett_atomic64_set(&a64,1,0));
    TIME_OPERATION("gasnett_atomic64_increment", gasnett_atomic64_increment(&a64,0));
    TIME_OPERATION("gasnett_atomic64_decrement", gasnett_atomic64_decrement(&a64,0));
    TIME_OPERATION("gasnett_atomic64_decrement_and_test", gasnett_atomic64_decrement_and_test(&a64,0));
    TIME_OPERATION_FULL("gasnett_atomic64_compare_and_swap (result=1)", { gasnett_atomic64_set(&a64,0,0); },
			{ gasnett_atomic64_compare_and_swap(&a64,0,0,0); }, {});
    TIME_OPERATION_FULL("gasnett_atomic64_compare_and_swap (result=0)", { gasnett_atomic64_set(&a64,1,0); },
			{ gasnett_atomic64_compare_and_swap(&a64,0,0,0); }, {});
    TIME_OPERATION("gasnett_atomic64_swap", gasnett_atomic64_swap(&a64,i,0));
    TIME_OPERATION("gasnett_atomic64_add", gasnett_atomic64_add(&a64,i,0));
    TIME_OPERATION("gasnett_atomic64_subtract", gasnett_atomic64_subtract(&a64,i,0));

    doit3();
}
/* ------------------------------------------------------------------------------------ */
GASNETT_THREADKEY_DEFINE(key);
void doit3(void) { 
  void * volatile x = 0;
  volatile gasnet_threadinfo_t ti;
  static volatile uintptr_t y = 0;

  TEST_SECTION_BEGIN();
  { GASNET_BEGIN_FUNCTION();

    gasnett_threadkey_init(key);
    TIME_OPERATION("gasnett_threadkey_get (" _STRINGIFY(TEST_PARSEQ) " mode)",
      { x = gasnett_threadkey_get(key); });
    TIME_OPERATION("gasnett_threadkey_set (" _STRINGIFY(TEST_PARSEQ) " mode)",
      { gasnett_threadkey_set(key, x); });
    TIME_OPERATION("gasnett_threadkey_get_noinit (" _STRINGIFY(TEST_PARSEQ) " mode)",
      { x = gasnett_threadkey_get_noinit(key); });
    TIME_OPERATION("gasnett_threadkey_set_noinit (" _STRINGIFY(TEST_PARSEQ) " mode)",
      { gasnett_threadkey_set_noinit(key, x); });
  }

  TEST_SECTION_BEGIN();
  /* TODO: How to suppress unused var warnings in the timings of
   * GASNET_BEGIN_FUNCTION and GASNET_POST_THREADINFO w/o knowledge of
   * how they are implemented AND w/o adding operations that would
   * effect the timings.
   */
  TIME_OPERATION("GASNET_BEGIN_FUNCTION (" _STRINGIFY(TEST_PARSEQ) " mode)", 
      { GASNET_BEGIN_FUNCTION(); });
  TIME_OPERATION("GASNET_BEGIN_FUNCTION (" _STRINGIFY(TEST_PARSEQ) " mode) w/possible use", 
      { GASNET_BEGIN_FUNCTION(); if (y) y ^= (uintptr_t)GASNET_GET_THREADINFO(); });
  memset((void *)&ti,0,sizeof(ti));
  TIME_OPERATION("GASNET_POST_THREADINFO (" _STRINGIFY(TEST_PARSEQ) " mode)", 
      { GASNET_POST_THREADINFO(ti); });
  { GASNET_BEGIN_FUNCTION();
    TIME_OPERATION("GASNET_GET_THREADINFO (w/ BEGIN) (" _STRINGIFY(TEST_PARSEQ) " mode)", 
      { y ^= (uintptr_t)GASNET_GET_THREADINFO(); });
  }
  TIME_OPERATION("GASNET_GET_THREADINFO (no BEGIN) (" _STRINGIFY(TEST_PARSEQ) " mode)", 
      { y ^= (uintptr_t)GASNET_GET_THREADINFO(); });

  doit4();
}
/* ------------------------------------------------------------------------------------ */
void doit4(void) { GASNET_BEGIN_FUNCTION();

    TEST_SECTION_BEGIN();
    TIME_OPERATION("local 4-byte gex_RMA_PutBlocking",
      { gex_RMA_PutBlocking(myteam, mynode, myseg, &temp, 4, 0); });

    TIME_OPERATION("local 4-byte gex_RMA_PutNB",
      { gex_Event_Wait(gex_RMA_PutNB(myteam, mynode, myseg, &temp, 4, GEX_EVENT_NOW, 0)); });

    TIME_OPERATION_FULL("local 4-byte gex_RMA_PutNBI", {},
      { gex_RMA_PutNBI(myteam, mynode, myseg, &temp, 4, GEX_EVENT_NOW, 0); },
      { gex_NBI_Wait(GEX_EC_PUT, 0); });

    TIME_OPERATION("local 4-byte gex_RMA_PutNB/bulk",
      { gex_Event_Wait(gex_RMA_PutNB(myteam, mynode, myseg, &temp, 4, GEX_EVENT_DEFER, 0)); });

    TIME_OPERATION_FULL("local 4-byte gex_RMA_PutNBI/bulk", {},
      { gex_RMA_PutNBI(myteam, mynode, myseg, &temp, 4, GEX_EVENT_DEFER, 0); },
      { gex_NBI_Wait(GEX_EC_PUT, 0); });

    TIME_OPERATION("local 4-byte gex_RMA_PutBlockingVal",
      { gex_RMA_PutBlockingVal(myteam, mynode, myseg, temp, 4, 0); });

    TIME_OPERATION("local 4-byte gex_RMA_PutNBVal",
      { gex_Event_Wait(gex_RMA_PutNBVal(myteam, mynode, myseg, temp, 4, 0)); });

    TIME_OPERATION_FULL("local 4-byte gex_RMA_PutNBIVal", {},
      { gex_RMA_PutNBIVal(myteam, mynode, myseg, temp, 4, 0); },
      { gex_NBI_Wait(GEX_EC_PUT, 0); });

    TIME_OPERATION("local 1024-byte gex_RMA_PutBlocking",
      { gex_RMA_PutBlocking(myteam, mynode, myseg, &bigtemp, 1024, 0); });

    doit5();
}
/* ------------------------------------------------------------------------------------ */
void doit5(void) { GASNET_BEGIN_FUNCTION();

    TIME_OPERATION("local 4-byte gex_RMA_GetBlocking",
      { gex_RMA_GetBlocking(myteam, &temp, mynode, myseg, 4, 0); });

    TIME_OPERATION("local 4-byte gex_RMA_GetNB",
      { gex_Event_Wait(gex_RMA_GetNB(myteam, &temp, mynode, myseg, 4, 0)); });

    TIME_OPERATION_FULL("local 4-byte gex_RMA_GetNBI", {},
      { gex_RMA_GetNBI(myteam, &temp, mynode, myseg, 4, 0); },
      { gex_NBI_Wait(GEX_EC_GET, 0); });

    TIME_OPERATION("local 4-byte gex_RMA_GetBlockingVal",
      { temp = (int32_t)gex_RMA_GetBlockingVal(myteam, mynode, myseg, 4, 0); });

    TIME_OPERATION("local 1024-byte gex_RMA_GetBlocking",
      { gex_RMA_GetBlocking(myteam, &bigtemp, mynode, myseg, 1024, 0); });

    doit6();
}
/* ------------------------------------------------------------------------------------ */
void doit6(void) { GASNET_BEGIN_FUNCTION();

    { int32_t temp1 = 0;
      int32_t temp2 = 0;
      int32_t volatile *ptemp1 = &temp1;
      int32_t volatile *ptemp2 = &temp2;
      TIME_OPERATION("local 4-byte assignment",
        { *(ptemp1) = *(ptemp2); });
    }

    { int8_t temp1[1024];
      int8_t temp2[1024];
      TIME_OPERATION("local 1024-byte memcpy",
        { memcpy(temp1, temp2, 1024); });
    }

    doit7();
}
/* ------------------------------------------------------------------------------------ */
void doit7(void) { GASNET_BEGIN_FUNCTION();

    TEST_SECTION_BEGIN();
    TIME_OPERATION("do-nothing gex_Event_Wait()",
      { gex_Event_Wait(GEX_EVENT_INVALID);  });

    TIME_OPERATION("do-nothing gex_Event_Test()",
      { int junk = gex_Event_Test(GEX_EVENT_INVALID); });

    TIME_OPERATION("do-nothing gex_Event_WaitAll() (8 events)",
      { gex_Event_WaitAll(events, 8, 0); });

    TIME_OPERATION("do-nothing gex_Event_WaitSome() (8 events)",
      { gex_Event_WaitSome(events, 8, 0); });

    TIME_OPERATION("do-nothing gex_Event_TestAll() (8 events)",
      { gex_Event_TestAll(events, 8, 0);  });

    TIME_OPERATION("do-nothing gex_Event_TestSome() (8 events)",
      { gex_Event_TestSome(events, 8, 0); });


    TIME_OPERATION("do-nothing gex_NBI_Wait(ALL)",
      { gex_NBI_Wait(GEX_EC_ALL, 0); });

    TIME_OPERATION("do-nothing gex_NBI_Wait(PUT)",
      { gex_NBI_Wait(GEX_EC_PUT, 0); });

    TIME_OPERATION("do-nothing gex_NBI_Wait(GET)",
      { gex_NBI_Wait(GEX_EC_GET, 0); });

    TIME_OPERATION("do-nothing gex_NBI_Test(ALL)",
      { int junk = gex_NBI_Test(GEX_EC_ALL, 0); });

    TIME_OPERATION("do-nothing gex_NBI_Test(PUT)",
      { int junk = gex_NBI_Test(GEX_EC_PUT, 0); });

    TIME_OPERATION("do-nothing gex_NBI_Test(GET)",
      { int junk = gex_NBI_Test(GEX_EC_GET, 0); });

    TIME_OPERATION("do-nothing begin/end nbi accessregion",
      { gex_NBI_BeginAccessRegion(0);
        gex_Event_Wait(gex_NBI_EndAccessRegion(0));
      });

    TEST_SECTION_BEGIN();
    TIME_OPERATION("single-node barrier",
      { gasnet_barrier_notify(0,GASNET_BARRIERFLAG_ANONYMOUS);            
        gasnet_barrier_wait(0,GASNET_BARRIERFLAG_ANONYMOUS); 
      });
    if (TEST_SECTION_ENABLED() && (gex_TM_QuerySize(myteam) > 1))
      MSG0("Note: this is actually the barrier time for %i nodes, "
           "since you're running with more than one node.\n",
           (int)gex_TM_QuerySize(myteam));

    doit8();
}
/* ------------------------------------------------------------------------------------ */
void doit8(void) { GASNET_BEGIN_FUNCTION();
    /* buffers, aligned and not on the stack */
    static long src[(1024 + sizeof(void*)) / sizeof(long)];
    static long dst[(1024 + sizeof(void*)) / sizeof(long)];
    const char *s = (const char *)src;
    char *d = (char *)dst;

    TEST_SECTION_BEGIN();
    if (TEST_SECTION_ENABLED()) {
      int i;
      for (i = 0; i < sizeof(src); i++) {
        ((uint8_t*)src)[i] = TEST_RAND(0,255);
      }
    }

    TIME_OPERATION("1024-byte gasnett_count0s()",
      { int junk = gasnett_count0s(s, 1024); });
    TIME_OPERATION("1024-byte gasnett_count0s_copy()",
      { int junk = gasnett_count0s_copy(d, s, 1024); });
    TIME_OPERATION("1024-byte gasnett_count0s() + memcpy()",
      { int junk = gasnett_count0s(s, 1024);
        (void)memcpy(d,s,1024);
      });

    s += sizeof(void*) / 2;
    d += sizeof(void*) / 2;
    TIME_OPERATION("unaligned 1024-byte gasnett_count0s()",
      { int junk = gasnett_count0s(s, 1024); });
    TIME_OPERATION("unaligned 1024-byte gasnett_count0s_copy()",
      { int junk = gasnett_count0s_copy(d, s, 1024); });
    TIME_OPERATION("unaligned 1024-byte gasnett_count0s() + memcpy()",
      { int junk = gasnett_count0s(s, 1024);
        (void)memcpy(d,s,1024);
      });

    s -= 1;
    d += 1;
    TIME_OPERATION("misaligned 1024-byte gasnett_count0s_copy()",
      { int junk = gasnett_count0s_copy(d, s, 1024); });
    TIME_OPERATION("misaligned 1024-byte gasnett_count0s() + memcpy()",
      { int junk = gasnett_count0s(s, 1024);
        (void)memcpy(d,s,1024);
      });

    { volatile int temp;
      int volatile *ptr = &temp;
      TIME_OPERATION("gasnett_count0s_uint32_t()",
        { (*ptr) = gasnett_count0s_uint32_t((uint32_t)i); });
      TIME_OPERATION("gasnett_count0s_uint64_t()",
        { (*ptr) = gasnett_count0s_uint64_t((uint64_t)i); });
    }
}
/* ------------------------------------------------------------------------------------ */

