/*   $Source: /var/local/cvs/gasnet/tests/testmisc.c,v $
 *     $Date: 2010/04/16 22:28:15 $
 * $Revision: 1.45.2.1 $
 * Description: GASNet misc performance test
 *   Measures the overhead associated with a number of purely local 
 *   operations that involve no communication. 
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <test.h>

int mynode = 0;
int iters=0;
void *myseg = NULL;
int accuracy = 0;

void report(const char *desc, int64_t totaltime, int iters) {
  if (mynode == 0) {
      char format[80];
      sprintf(format, "%c: %%-50s: %%%i.%if s  %%%i.%if us\n", 
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

void null_shorthandler(gasnet_token_t token) {
}

void justreply_shorthandler(gasnet_token_t token) {
  gasnet_AMReplyShort0(token, hidx_null_shorthandler);
}

void null_medhandler(gasnet_token_t token, void *buf, size_t nbytes) {
}

void justreply_medhandler(gasnet_token_t token, void *buf, size_t nbytes) {
  gasnet_AMReplyMedium0(token, hidx_null_medhandler, buf, nbytes);
}

void null_longhandler(gasnet_token_t token, void *buf, size_t nbytes) {
}

void justreply_longhandler(gasnet_token_t token, void *buf, size_t nbytes) {
  gasnet_AMReplyLong0(token, hidx_null_longhandler, buf, nbytes, buf);
}
/* ------------------------------------------------------------------------------------ */
/* This tester measures the performance of a number of miscellaneous GASNet functions 
   that don't involve actual communication, to assist in evaluating the overhead of 
   the GASNet layer itself
 */
int main(int argc, char **argv) {
  gasnet_handlerentry_t htable[] = { 
    { hidx_null_shorthandler,       null_shorthandler },
    { hidx_justreply_shorthandler,  justreply_shorthandler },
    { hidx_null_medhandler,         null_medhandler },
    { hidx_justreply_medhandler,    justreply_medhandler },
    { hidx_null_longhandler,        null_longhandler },
    { hidx_justreply_longhandler,   justreply_longhandler }
  };

  GASNET_Safe(gasnet_init(&argc, &argv));
  GASNET_Safe(gasnet_attach(htable, sizeof(htable)/sizeof(gasnet_handlerentry_t),
                            TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
  test_init("testmisc",1,"(iters) (accuracy_digits) (test_sections)");

  mynode = gasnet_mynode();
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
    else report(#op, tickcvt(end - start), iters);         \
  }
#define TIME_OPERATION(desc, op) TIME_OPERATION_FULL(desc, {}, op, {})

char p[1];
gasnet_hsl_t hsl = GASNET_HSL_INITIALIZER;
gasnett_atomic_t a = gasnett_atomic_init(0);
gasnett_atomic32_t a32 = gasnett_atomic32_init(0);
gasnett_atomic64_t a64 = gasnett_atomic64_init(0);
int32_t temp = 0;
gasnett_tick_t timertemp = 0;
int8_t bigtemp[1024];
gasnet_handle_t handles[8];
/* ------------------------------------------------------------------------------------ */
void doit1(void) { GASNET_BEGIN_FUNCTION();

    { int i; for (i=0;i<8;i++) handles[i] = GASNET_INVALID_HANDLE; }

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
    
    TIME_OPERATION("Loopback do-nothing gasnet_AMRequestShort0()",
      { gasnet_AMRequestShort0(mynode, hidx_null_shorthandler); });

    TIME_OPERATION("Loopback do nothing AM short request-reply",
      { gasnet_AMRequestShort0(mynode, hidx_justreply_shorthandler); });

    TIME_OPERATION("Loopback do-nothing gasnet_AMRequestMedium0()",
      { gasnet_AMRequestMedium0(mynode, hidx_null_medhandler, p, 0); });

    TIME_OPERATION("Loopback do nothing AM medium request-reply",
      { gasnet_AMRequestMedium0(mynode, hidx_justreply_medhandler, p, 0); });

    TIME_OPERATION("Loopback do-nothing gasnet_AMRequestLong0()",
      { gasnet_AMRequestLong0(mynode, hidx_null_medhandler, p, 0, myseg); });

    TIME_OPERATION("Loopback do nothing AM long request-reply",
      { gasnet_AMRequestLong0(mynode, hidx_justreply_medhandler, p, 0, myseg); });

    doit2();
}
/* ------------------------------------------------------------------------------------ */
volatile int val_true = 1;
volatile int val_false = 0;
int val_junk = 0;
void doit2(void) { GASNET_BEGIN_FUNCTION();

    TEST_SECTION_BEGIN();
    TIME_OPERATION("hold/resume interrupts",
      { gasnet_hold_interrupts(); gasnet_resume_interrupts(); });

    #if defined(GASNET_PAR) || defined (GASNET_PARSYNC)
      { static gasnett_mutex_t mutex = GASNETT_MUTEX_INITIALIZER;
        TIME_OPERATION("lock/unlock uncontended pthread mutex",
          { gasnett_mutex_lock(&mutex); gasnett_mutex_unlock(&mutex); });
      }
    #endif

    TIME_OPERATION("lock/unlock uncontended HSL (" _STRINGIFY(TEST_PARSEQ) " mode)",
      { gasnet_hsl_lock(&hsl); gasnet_hsl_unlock(&hsl); });

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
    TIME_OPERATION_FULL("gasnett_atomic32_compare_and_swap (result=1)", { gasnett_atomic32_set(&a32,0,0); },
			{ gasnett_atomic32_compare_and_swap(&a32,0,0,0); }, {});
    TIME_OPERATION_FULL("gasnett_atomic32_compare_and_swap (result=0)", { gasnett_atomic32_set(&a32,1,0); },
			{ gasnett_atomic32_compare_and_swap(&a32,0,0,0); }, {});

    TEST_SECTION_BEGIN();
    TIME_OPERATION("gasnett_atomic64_read", gasnett_atomic64_read(&a64,0));
    TIME_OPERATION("gasnett_atomic64_set", gasnett_atomic64_set(&a64,1,0));
    TIME_OPERATION_FULL("gasnett_atomic64_compare_and_swap (result=1)", { gasnett_atomic64_set(&a64,0,0); },
			{ gasnett_atomic64_compare_and_swap(&a64,0,0,0); }, {});
    TIME_OPERATION_FULL("gasnett_atomic64_compare_and_swap (result=0)", { gasnett_atomic64_set(&a64,1,0); },
			{ gasnett_atomic64_compare_and_swap(&a64,0,0,0); }, {});

    doit3();
}
/* ------------------------------------------------------------------------------------ */
GASNETT_THREADKEY_DEFINE(key);
void doit3(void) { 
  void * volatile x = 0;
  volatile gasnet_threadinfo_t ti;
  volatile uintptr_t y = 0;

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
    TIME_OPERATION("local 4-byte gasnet_put",
      { gasnet_put(mynode, myseg, &temp, 4); });

    TIME_OPERATION("local 4-byte gasnet_put_nb",
      { gasnet_wait_syncnb(gasnet_put_nb(mynode, myseg, &temp, 4)); });

    TIME_OPERATION_FULL("local 4-byte gasnet_put_nbi", {},
      { gasnet_put_nbi(mynode, myseg, &temp, 4); },
      { gasnet_wait_syncnbi_puts(); });

    TIME_OPERATION("local 4-byte gasnet_put_bulk",
      { gasnet_put_bulk(mynode, myseg, &temp, 4); });

    TIME_OPERATION("local 4-byte gasnet_put_nb_bulk",
      { gasnet_wait_syncnb(gasnet_put_nb_bulk(mynode, myseg, &temp, 4)); });

    TIME_OPERATION_FULL("local 4-byte gasnet_put_nbi_bulk", {},
      { gasnet_put_nbi(mynode, myseg, &temp, 4); },
      { gasnet_wait_syncnbi_puts(); });

    TIME_OPERATION("local 4-byte gasnet_put_val",
      { gasnet_put_val(mynode, myseg, temp, 4); });

    TIME_OPERATION("local 4-byte gasnet_put_nb_val",
      { gasnet_wait_syncnb(gasnet_put_nb_val(mynode, myseg, temp, 4)); });

    TIME_OPERATION_FULL("local 4-byte gasnet_put_nbi_val", {},
      { gasnet_put_nbi_val(mynode, myseg, temp, 4); },
      { gasnet_wait_syncnbi_puts(); });

    TIME_OPERATION("local 1024-byte gasnet_put_bulk",
      { gasnet_put_bulk(mynode, myseg, &bigtemp, 1024); });

    doit5();
}
/* ------------------------------------------------------------------------------------ */
void doit5(void) { GASNET_BEGIN_FUNCTION();

    TIME_OPERATION("local 4-byte gasnet_get",
      { gasnet_get(&temp, mynode, myseg, 4); });

    TIME_OPERATION("local 4-byte gasnet_get_nb",
      { gasnet_wait_syncnb(gasnet_get_nb(&temp, mynode, myseg, 4)); });

    TIME_OPERATION_FULL("local 4-byte gasnet_get_nbi", {},
      { gasnet_get_nbi(&temp, mynode, myseg, 4); },
      { gasnet_wait_syncnbi_gets(); });

    TIME_OPERATION("local 4-byte gasnet_get_bulk",
      { gasnet_get_bulk(&temp, mynode, myseg, 4); });

    TIME_OPERATION("local 4-byte gasnet_get_nb_bulk",
      { gasnet_wait_syncnb(gasnet_get_nb_bulk(&temp, mynode, myseg, 4)); });

    TIME_OPERATION_FULL("local 4-byte gasnet_get_nbi_bulk", {},
      { gasnet_get_nbi_bulk(&temp, mynode, myseg, 4); },
      { gasnet_wait_syncnbi_gets(); });

    TIME_OPERATION("local 4-byte gasnet_get_val",
      { temp = (int32_t)gasnet_get_val(mynode, myseg, 4); });

    TIME_OPERATION("local 4-byte gasnet_get_nb_val",
      { gasnet_valget_handle_t handle = gasnet_get_nb_val(mynode, myseg, 4);
        temp = (int32_t)gasnet_wait_syncnb_valget(handle);
      });

    TIME_OPERATION("local 1024-byte gasnet_get_bulk",
      { gasnet_get_bulk(&bigtemp, mynode, myseg, 1024); });

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
    TIME_OPERATION("do-nothing gasnet_wait_syncnb()",
      { gasnet_wait_syncnb(GASNET_INVALID_HANDLE);  });

    TIME_OPERATION("do-nothing gasnet_try_syncnb()",
      { GASNETI_UNUSED int junk = gasnet_try_syncnb(GASNET_INVALID_HANDLE); });

    TIME_OPERATION("do-nothing gasnet_wait_syncnb_all() (8 handles)",
      { gasnet_wait_syncnb_all(handles, 8); });

    TIME_OPERATION("do-nothing gasnet_wait_syncnb_some() (8 handles)",
      { gasnet_wait_syncnb_some(handles, 8); });

    TIME_OPERATION("do-nothing gasnet_try_syncnb_all() (8 handles)",
      { gasnet_try_syncnb_all(handles, 8);  });

    TIME_OPERATION("do-nothing gasnet_try_syncnb_some() (8 handles)",
      { gasnet_try_syncnb_some(handles, 8); });

    TIME_OPERATION("do-nothing gasnet_wait_syncnbi_all()",
      { gasnet_wait_syncnbi_all(); });

    TIME_OPERATION("do-nothing gasnet_wait_syncnbi_puts()",
      { gasnet_wait_syncnbi_puts(); });

    TIME_OPERATION("do-nothing gasnet_wait_syncnbi_gets()",
      { gasnet_wait_syncnbi_gets(); });

    TIME_OPERATION("do-nothing gasnet_try_syncnbi_all()",
      { GASNETI_UNUSED int junk = gasnet_try_syncnbi_all(); });

    TIME_OPERATION("do-nothing gasnet_try_syncnbi_puts()",
      { GASNETI_UNUSED int junk = gasnet_try_syncnbi_puts(); });

    TIME_OPERATION("do-nothing gasnet_try_syncnbi_gets()",
      { GASNETI_UNUSED int junk = gasnet_try_syncnbi_gets(); });

    TIME_OPERATION("do-nothing begin/end nbi accessregion",
      { gasnet_begin_nbi_accessregion();
        gasnet_wait_syncnb(gasnet_end_nbi_accessregion());
      });

    TEST_SECTION_BEGIN();
    TIME_OPERATION("single-node barrier",
      { gasnet_barrier_notify(0,GASNET_BARRIERFLAG_ANONYMOUS);            
        gasnet_barrier_wait(0,GASNET_BARRIERFLAG_ANONYMOUS); 
      });
    if (TEST_SECTION_ENABLED() && (gasnet_nodes() > 1))
      MSG0("Note: this is actually the barrier time for %i nodes, "
           "since you're running with more than one node.\n", (int)gasnet_nodes());

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
      { GASNETI_UNUSED int junk = gasnett_count0s(s, 1024); });
    TIME_OPERATION("1024-byte gasnett_count0s_copy()",
      { GASNETI_UNUSED int junk = gasnett_count0s_copy(d, s, 1024); });
    TIME_OPERATION("1024-byte gasnett_count0s() + memcpy()",
      { GASNETI_UNUSED int junk = gasnett_count0s(s, 1024);
        (void)memcpy(d,s,1024);
      });

    s += sizeof(void*) / 2;
    d += sizeof(void*) / 2;
    TIME_OPERATION("unaligned 1024-byte gasnett_count0s()",
      { GASNETI_UNUSED int junk = gasnett_count0s(s, 1024); });
    TIME_OPERATION("unaligned 1024-byte gasnett_count0s_copy()",
      { GASNETI_UNUSED int junk = gasnett_count0s_copy(d, s, 1024); });
    TIME_OPERATION("unaligned 1024-byte gasnett_count0s() + memcpy()",
      { GASNETI_UNUSED int junk = gasnett_count0s(s, 1024);
        (void)memcpy(d,s,1024);
      });

    s -= 1;
    d += 1;
    TIME_OPERATION("misaligned 1024-byte gasnett_count0s_copy()",
      { GASNETI_UNUSED int junk = gasnett_count0s_copy(d, s, 1024); });
    TIME_OPERATION("misaligned 1024-byte gasnett_count0s() + memcpy()",
      { GASNETI_UNUSED int junk = gasnett_count0s(s, 1024);
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

