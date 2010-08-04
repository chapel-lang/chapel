/*   $Source: /var/local/cvs/gasnet/tests/testgasnet.c,v $
 *     $Date: 2010/04/16 22:28:15 $
 * $Revision: 1.66.2.1 $
 * Description: General GASNet correctness tests
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet.h>

/* limit segsz to prevent stack overflows for seg_everything tests */
#define TEST_MAXTHREADS 1
#include <test.h>

#define TEST_GASNET 1
#define SHORT_REQ_BASE 128
#include <other/amxtests/testam.h>

/* Define to get one big function that pushes the gcc inliner heursitics */
#undef TESTGASNET_NO_SPLIT

TEST_BACKTRACE_DECLS();

void doit(int partner, int *partnerseg);
void doit2(int partner, int *partnerseg);
void doit3(int partner, int *partnerseg);
void doit4(int partner, int *partnerseg);
void doit5(int partner, int *partnerseg);

/* ------------------------------------------------------------------------------------ */
#if GASNET_SEGMENT_EVERYTHING
  typedef struct {
    void *static_seg;
    void *common_seg;
    void *malloc_seg;
    void *sbrk_seg;
    void *mmap_seg;
    void *stack_seg;
  } test_everything_seginfo_t;
  test_everything_seginfo_t myinfo;
  test_everything_seginfo_t partnerinfo;
  int done = 0;
  void seg_everything_reqh(gasnet_token_t token) {
    GASNET_Safe(gasnet_AMReplyMedium0(token, 251, &myinfo, sizeof(test_everything_seginfo_t)));
  }
  void seg_everything_reph(gasnet_token_t token, void *buf, size_t nbytes) {
    assert(nbytes == sizeof(test_everything_seginfo_t));
    memcpy(&partnerinfo, buf, nbytes);
    gasnett_local_wmb();
    done = 1;
  }
  #define EVERYTHING_SEG_HANDLERS() \
    { 250, (handler_fn_t)seg_everything_reqh }, \
    { 251, (handler_fn_t)seg_everything_reph },

  char _static_seg[TEST_SEGSZ+PAGESZ] = {1};
  char _common_seg[TEST_SEGSZ+PAGESZ];
  void everything_tests(int partner) {
    char _stack_seg[TEST_SEGSZ+PAGESZ];

    if (gasnet_mynode() == 0) MSG("*** gathering data segment info for SEGMENT_EVERYTHING tests...");
    BARRIER();
    myinfo.static_seg = alignup_ptr(&_static_seg, PAGESZ);
    myinfo.common_seg = alignup_ptr(&_common_seg, PAGESZ);
    myinfo.malloc_seg = alignup_ptr(test_malloc(TEST_SEGSZ+PAGESZ), PAGESZ);
    myinfo.sbrk_seg = alignup_ptr(sbrk(TEST_SEGSZ+PAGESZ), PAGESZ);
    #ifdef HAVE_MMAP
      myinfo.mmap_seg = alignup_ptr(gasnett_mmap(TEST_SEGSZ+PAGESZ), PAGESZ);
    #endif
    myinfo.stack_seg = alignup_ptr(&_stack_seg, PAGESZ);
    BARRIER();
    /* fetch partner's addresses into partnerinfo */
    GASNET_Safe(gasnet_AMRequestShort0((gasnet_node_t)partner, 250));
    GASNET_BLOCKUNTIL(done);
    BARRIER();

    /* test that remote access works will all the various data areas */
    if (gasnet_mynode() == 0) MSG(" --- testgasnet w/ static data area ---");
    doit(partner, (int*)partnerinfo.static_seg);
    if (gasnet_mynode() == 0) MSG(" --- testgasnet w/ common block data area ---");
    doit(partner, (int*)partnerinfo.common_seg);
    if (gasnet_mynode() == 0) MSG(" --- testgasnet w/ malloc data area ---");
    doit(partner, (int*)partnerinfo.malloc_seg);
    if (gasnet_mynode() == 0) MSG(" --- testgasnet w/ sbrk data area ---");
    doit(partner, (int*)partnerinfo.sbrk_seg);
    #ifdef HAVE_MMAP
      if (gasnet_mynode() == 0) MSG(" --- testgasnet w/ mmap'd data area ---");
      doit(partner, (int*)partnerinfo.mmap_seg);
    #endif
    if (gasnet_mynode() == 0) MSG(" --- testgasnet w/ stack data area ---");
    doit(partner, (int*)partnerinfo.stack_seg);
    BARRIER();
  }
#else
  #define EVERYTHING_SEG_HANDLERS()
#endif

#if GASNET_PAR
#if PLATFORM_OS_BGP
  #define NUM_THREADS TEST_MAXTHREADS
#else
  #define NUM_THREADS 10
#endif
#else
  #define NUM_THREADS 1
#endif

void test_threadinfo(int threadid, int numthreads) {
  int i;
  gasnet_threadinfo_t my_ti;
  static gasnet_threadinfo_t all_ti[NUM_THREADS];

  { GASNET_BEGIN_FUNCTION();
    my_ti = GASNET_GET_THREADINFO();
  }
  { gasnet_threadinfo_t ti = GASNET_GET_THREADINFO();
    assert_always(ti == my_ti);
  }
  { GASNET_POST_THREADINFO(my_ti);
    gasnet_threadinfo_t ti = GASNET_GET_THREADINFO();
    assert_always(ti == my_ti);
  }
  assert(threadid < numthreads && numthreads <= NUM_THREADS);
  all_ti[threadid] = my_ti;
  PTHREAD_LOCALBARRIER(numthreads);
  for (i = 0; i < numthreads; i++) {
    if (i != threadid) assert_always(my_ti != all_ti[i]);
  }
  PTHREAD_LOCALBARRIER(numthreads);
}
/* ------------------------------------------------------------------------------------ */
/* test libgasnet-specific gasnet_tools interfaces */
#if GASNET_PAR
  /* thread-parallel gasnet_tools tests */
  #ifdef __cplusplus
    extern "C"
  #endif
  void *test_libgasnetpar_tools(void *p) {
    int idx = (int)(uintptr_t)p;
    PTHREAD_LOCALBARRIER(NUM_THREADS);
    test_threadinfo(idx, NUM_THREADS);
    PTHREAD_LOCALBARRIER(NUM_THREADS);
  #if !GASNETI_ARCH_ALTIX
    gasnett_set_affinity(idx);
  #endif
    PTHREAD_LOCALBARRIER(NUM_THREADS);
    return NULL;
  }
#endif
void test_libgasnet_tools(void) {
  void *p;
  TEST_TRACING_MACROS();
  #ifdef HAVE_MMAP
    p = gasnett_mmap(GASNETT_PAGESIZE);
    assert_always(p);
    assert_always(((uintptr_t)p)%GASNETT_PAGESIZE == 0);
  #endif
  test_threadinfo(0, 1);
  #if GASNET_DEBUG
  { char *ptr = (char *)gasnett_debug_malloc(10); 
    char *ptr2;
    gasnett_heapstats_t hs;
    assert_always(ptr);
    gasnett_debug_memcheck(ptr);
    ptr = (char *)gasnett_debug_realloc(ptr,20);
    assert_always(ptr);
    gasnett_debug_free(ptr);
    ptr = (char *)gasnett_debug_calloc(10,20);
    strcpy(ptr,"testing 1 2 3");
    ptr2 = gasnett_debug_strdup(ptr);
    assert_always(ptr2 && ptr != ptr2 && !strcmp(ptr,ptr2));
    gasnett_debug_free(ptr2);
    ptr2 = gasnett_debug_strndup(ptr,4);
    assert_always(ptr2 && ptr != ptr2 && !strncmp(ptr,ptr2,4) && strlen(ptr2) == 4);
    gasnett_debug_memcheck_one();
    gasnett_debug_memcheck_all(); 
    gasnett_debug_free(ptr2);
    gasnett_debug_free(ptr);
    gasnett_getheapstats(&hs);
  }
  #endif
  #if GASNET_PAR
    test_createandjoin_pthreads(NUM_THREADS, &test_libgasnetpar_tools, NULL, 0);
  #endif
  MSG("*** passed libgasnet_tools test!!");
}
/* ------------------------------------------------------------------------------------ */
int main(int argc, char **argv) {
  int partner;
  
  gasnet_handlerentry_t handlers[] = { EVERYTHING_SEG_HANDLERS() ALLAM_HANDLERS() };

  GASNET_Safe(gasnet_init(&argc, &argv));
  #if GASNET_SEGMENT_EVERYTHING
    assert_always(gasnet_getMaxLocalSegmentSize() == (uintptr_t)-1);
    assert_always(gasnet_getMaxGlobalSegmentSize() == (uintptr_t)-1);
  #else
    assert_always(gasnet_getMaxLocalSegmentSize() >= gasnet_getMaxGlobalSegmentSize());
    assert_always(gasnet_getMaxLocalSegmentSize() % GASNET_PAGESIZE == 0);
    assert_always(gasnet_getMaxGlobalSegmentSize() % GASNET_PAGESIZE == 0);
    assert_always(gasnet_getMaxGlobalSegmentSize() > 0);
  #endif
  GASNET_Safe(gasnet_attach(handlers, sizeof(handlers)/sizeof(gasnet_handlerentry_t), 
                            TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
  test_init("testgasnet",0,"");
  assert(TEST_SEGSZ >= 2*sizeof(int)*NUMHANDLERS_PER_TYPE);

  TEST_PRINT_CONDUITINFO();

  { int smaj = GASNET_SPEC_VERSION_MAJOR;
    int smin = GASNET_SPEC_VERSION_MINOR;
    int rmaj = GASNET_RELEASE_VERSION_MAJOR;
    int rmin = GASNET_RELEASE_VERSION_MINOR;
    int rpat = GASNET_RELEASE_VERSION_PATCH;
    int smaj2 = GASNET_VERSION;
    assert_always(smaj > 0 && smin >= 0 && rmaj > 0 && rmin >= 0 && rpat >= 0);
    assert_always(smaj == smaj2);
  }

  { int i;
    printf("my args: argc=%i argv=[", argc);
    for (i=0; i < argc; i++) {
      printf("%s'%s'",(i>0?" ":""),argv[i]);
    }
    printf("]\n"); fflush(stdout);
  }

  TEST_BACKTRACE_INIT(argv[0]);
  TEST_BACKTRACE();

  test_libgasnet_tools();
  partner = (gasnet_mynode() + 1) % gasnet_nodes();
  #if GASNET_SEGMENT_EVERYTHING
    everything_tests(partner);
  #else
    doit(partner, (int *)TEST_SEG(partner));
  #endif

  MSG("done.");

  gasnet_exit(0);
  return 0;
}

void doit(int partner, int *partnerseg) {
  int mynode = gasnet_mynode();

  BARRIER();
  /*  blocking test */
  { int val1=0, val2=0;
    val1 = mynode + 100;

    gasnet_put(partner, partnerseg, &val1, sizeof(int));
    gasnet_get(&val2, partner, partnerseg, sizeof(int));

    if (val2 == (mynode + 100)) MSG("*** passed blocking test!!");
    else MSG("*** ERROR - FAILED BLOCKING TEST!!!!!");
  }

  BARRIER();
  /*  blocking list test */
  #define iters 100
  { GASNET_BEGIN_FUNCTION();
    gasnet_handle_t handles[iters];
    int val1;
    int vals[iters];
    int success = 1;
    int i;
    for (i = 0; i < iters; i++) {
      val1 = 100 + i + mynode;
      handles[i] = gasnet_put_nb(partner, partnerseg+i, &val1, sizeof(int));
    }
    gasnet_wait_syncnb_all(handles, iters); 
    for (i = 0; i < iters; i++) {
      handles[i] = gasnet_get_nb(&vals[i], partner, partnerseg+i, sizeof(int));
    }
    gasnet_wait_syncnb_all(handles, iters); 
    for (i=0; i < iters; i++) {
      if (vals[i] != 100 + mynode + i) {
        MSG("*** ERROR - FAILED NB LIST TEST!!! vals[%i] = %i, expected %i",
            i, vals[i], 100 + mynode + i);
        success = 0;
      }
    }
    if (success) MSG("*** passed blocking list test!!");
  }

#ifndef TESTGASNET_NO_SPLIT
  doit2(partner, partnerseg);
}
void doit2(int partner, int *partnerseg) {
  int mynode = gasnet_mynode();
#endif

  BARRIER();
  { /*  implicit test */
    GASNET_BEGIN_FUNCTION();
    int vals[100];
    int i, success=1;
    for (i=0; i < 100; i++) {
      int tmp = mynode + i;
      gasnet_put_nbi(partner, partnerseg+i, &tmp, sizeof(int));
    }
    gasnet_wait_syncnbi_puts();
    for (i=0; i < 100; i++) {
      gasnet_get_nbi(&vals[i], partner, partnerseg+i, sizeof(int));
    }
    gasnet_wait_syncnbi_gets();
    for (i=0; i < 100; i++) {
      if (vals[i] != mynode + i) {
        MSG("*** ERROR - FAILED NBI TEST!!! vals[%i] = %i, expected %i",
            i, vals[i], mynode + i);
        success = 0;
      }
    }
    if (success) MSG("*** passed nbi test!!");
  }

#ifndef TESTGASNET_NO_SPLIT
  doit3(partner, partnerseg);
}
void doit3(int partner, int *partnerseg) {
  int mynode = gasnet_mynode();
#endif

  BARRIER();

  { /*  value test */
    GASNET_BEGIN_FUNCTION();
    int i, success=1;
    unsigned char *partnerbase2 = (unsigned char *)(partnerseg+300);
    for (i=0; i < 100; i++) {
      gasnet_put_val(partner, partnerseg+i, 1000 + mynode + i, sizeof(int));
    }
    for (i=0; i < 100; i++) {
      gasnet_wait_syncnb(gasnet_put_nb_val(partner, partnerseg+i+100, 1000 + mynode + i, sizeof(int)));
    }
    for (i=0; i < 100; i++) {
      gasnet_put_nbi_val(partner, partnerseg+i+200, 1000 + mynode + i, sizeof(int));
    }
    gasnet_wait_syncnbi_puts();

    for (i=0; i < 100; i++) {
      int tmp1 = gasnet_get_val(partner, partnerseg+i, sizeof(int));
      int tmp2 = gasnet_get_val(partner, partnerseg+i+200, sizeof(int));
      if (tmp1 != 1000 + mynode + i || tmp2 != 1000 + mynode + i) {
        MSG("*** ERROR - FAILED INT VALUE TEST 1!!!");
        printf("node %i/%i  i=%i tmp1=%i tmp2=%i (1000 + mynode + i)=%i\n", 
          (int)gasnet_mynode(), (int)gasnet_nodes(), 
          i, tmp1, tmp2, 1000 + mynode + i); fflush(stdout); 
        success = 0;
      }
    }
    { gasnet_valget_handle_t handles[100];
      for (i=0; i < 100; i++) {
        handles[i] = gasnet_get_nb_val(partner, partnerseg+i+100, sizeof(int));
      }
      for (i=0; i < 100; i++) {
        int tmp = (int)gasnet_wait_syncnb_valget(handles[i]);
        if (tmp != 1000 + mynode + i) {
          MSG("*** ERROR - FAILED INT VALUE TEST 2!!!");
          printf("node %i/%i  i=%i tmp1=%i (1000 + mynode + i)=%i\n", 
            (int)gasnet_mynode(), (int)gasnet_nodes(), 
            i, tmp, 1000 + mynode + i); fflush(stdout); 
          success = 0;
        }
      }
    }

    for (i=0; i < 100; i++) {
      gasnet_put_val(partner, partnerbase2+i, 100 + mynode + i, sizeof(unsigned char));
    }
    for (i=0; i < 100; i++) {
      gasnet_wait_syncnb(gasnet_put_nb_val(partner, partnerbase2+i+100, 100 + mynode + i, sizeof(unsigned char)));
    }
    for (i=0; i < 100; i++) {
      gasnet_put_nbi_val(partner, partnerbase2+i+200, 100 + mynode + i, sizeof(unsigned char));
    }
    gasnet_wait_syncnbi_puts();

    for (i=0; i < 100; i++) {
      unsigned int tmp1 = (unsigned int)gasnet_get_val(partner, partnerbase2+i, sizeof(unsigned char));
      unsigned int tmp2 = (unsigned int)gasnet_get_val(partner, partnerbase2+i+200, sizeof(unsigned char));
      if (tmp1 != (unsigned char)(100 + mynode + i) || 
          tmp2 != (unsigned char)(100 + mynode + i)) {
        MSG("*** ERROR - FAILED CHAR VALUE TEST 1!!!");
        printf("node %i/%i  i=%i tmp1=%i tmp2=%i (100 + mynode + i)=%i\n", 
          (int)gasnet_mynode(), (int)gasnet_nodes(), 
          i, tmp1, tmp2, 100 + mynode + i); fflush(stdout); 
        success = 0;
      }
    }
    { gasnet_valget_handle_t handles[100];
      for (i=0; i < 100; i++) {
        handles[i] = gasnet_get_nb_val(partner, partnerbase2+i+100, sizeof(unsigned char));
      }
      for (i=0; i < 100; i++) {
        unsigned int tmp = (unsigned int)gasnet_wait_syncnb_valget(handles[i]);
        if (tmp != (unsigned char)(100 + mynode + i)) {
          MSG("*** ERROR - FAILED CHAR VALUE TEST 2!!!");
          printf("node %i/%i  i=%i tmp1=%i (100 + mynode + i)=%i\n", 
            (int)gasnet_mynode(), (int)gasnet_nodes(), 
            i, tmp, 100 + mynode + i); fflush(stdout); 
          success = 0;
        }
      }
    }

    if (success) MSG("*** passed value test!!");
  }

#ifndef TESTGASNET_NO_SPLIT
  doit4(partner, partnerseg);
}
void doit4(int partner, int *partnerseg) {
  /* int mynode = gasnet_mynode(); UNUSED */
#endif

  BARRIER();

  { /*  memset test */
    GASNET_BEGIN_FUNCTION();
    int i, success=1;
    int vals[300];

    gasnet_memset(partner, partnerseg, 0x55, 100*sizeof(int));
    gasnet_wait_syncnb(gasnet_memset_nb(partner, partnerseg+100, 0x66, 100*sizeof(int)));
    gasnet_memset_nbi(partner, partnerseg+200, 0x77, 100*sizeof(int));
    gasnet_wait_syncnbi_puts();

    gasnet_get(&vals, partner, partnerseg, 300*sizeof(int));

    for (i=0; i < 100; i++) {
      unsigned long long five  = 0x5555555555555555ull;
      unsigned long long six   = 0x6666666666666666ull;
      unsigned long long seven = 0x7777777777777777ull;
      if (vals[i] != ((int)five)) {
        MSG("*** ERROR - FAILED MEMSET TEST!!!");
        success = 0;
      }
      if (vals[i+100] != ((int)six)) {
        MSG("*** ERROR - FAILED MEMSET TEST!!!");
        success = 0;
      }
      if (vals[i+200] != ((int)seven)) {
        MSG("*** ERROR - FAILED MEMSET TEST!!!");
        success = 0;
      }
    }
    if (success) MSG("*** passed memset test!!");
  }

#ifndef TESTGASNET_NO_SPLIT
  doit5(partner, partnerseg);
}
void doit5(int partner, int *partnerseg) {
  int mynode = gasnet_mynode();
#endif

  BARRIER();

  /*  put/overwrite/get test */
  #define MAXVALS (1024)
  #define MAXSZ (MAXVALS*8)
  #define SEGSZ (MAXSZ*4)
  #define VAL(sz, iter) \
    (((uint64_t)(sz) << 32) | ((uint64_t)(100 + mynode) << 16) | ((iter) & 0xFF))
  assert(TEST_SEGSZ >= 2*SEGSZ);
  { GASNET_BEGIN_FUNCTION();
    uint64_t *localvals=(uint64_t *)test_malloc(SEGSZ);
    int success = 1;
    int i, sz;
    for (i = 0; i < MAX(1,iters/10); i++) {
      uint64_t *localpos=localvals;
      uint64_t *segpos=(uint64_t *)TEST_MYSEG();
      uint64_t *rsegpos=(uint64_t *)((char*)partnerseg+SEGSZ);
      for (sz = 1; sz <= MAXSZ; sz*=2) {
        gasnet_handle_t handle;
        int elems = sz/8;
        int j;
        uint64_t val = VAL(sz, i); /* setup known src value */
        if (sz < 8) {
          elems = 1;
          memset(localpos, (val & 0xFF), sz);
          memset(segpos, (val & 0xFF), sz);
          memset(&val, (val & 0xFF), sz);
        } else {
          for (j=0; j < elems; j++) {
            localpos[j] = val;
            segpos[j] = val;
          }
        }
        handle = gasnet_put_nb_bulk(partner, rsegpos, localpos, sz);
        gasnet_wait_syncnb(handle);

        handle = gasnet_put_nb(partner, rsegpos+elems, localpos, sz);
        memset(localpos, 0xCC, sz); /* clear */
        gasnet_wait_syncnb(handle);

        handle = gasnet_put_nb_bulk(partner, rsegpos+2*elems, segpos, sz);
        gasnet_wait_syncnb(handle);

        handle = gasnet_put_nb(partner, rsegpos+3*elems, segpos, sz);
        memset(segpos, 0xCC, sz); /* clear */
        gasnet_wait_syncnb(handle);

        gasnet_wait_syncnb(gasnet_get_nb(localpos, partner, rsegpos, sz));
        gasnet_wait_syncnb(gasnet_get_nb_bulk(localpos+elems, partner, rsegpos+elems, sz));
        gasnet_wait_syncnb(gasnet_get_nb(segpos, partner, rsegpos+2*elems, sz));
        gasnet_wait_syncnb(gasnet_get_nb_bulk(segpos+elems, partner, rsegpos+3*elems, sz));

        for (j=0; j < elems*2; j++) {
          int ok;
          ok = localpos[j] == val;
          if (sz < 8) ok = !memcmp(&(localpos[j]), &val, sz);
          if (!ok) {
              MSG("*** ERROR - FAILED OUT-OF-SEG PUT/OVERWRITE TEST!!! sz=%i j=%i (got=%016llx expected=%016llx)", (sz), j,
                  (unsigned long long)localpos[j], (unsigned long long)val);
              success = 0;
          }
          ok = segpos[j] == val;
          if (sz < 8) ok = !memcmp(&(segpos[j]), &val, sz);
          if (!ok) {
              MSG("*** ERROR - FAILED IN-SEG PUT/OVERWRITE TEST!!! sz=%i j=%i (got=%016llx expected=%016llx)", (sz), j,
                  (unsigned long long)segpos[j], (unsigned long long)val);
              success = 0;
          }
        }
      }
    }
    test_free(localvals);
    if (success) MSG("*** passed put/overwrite test!!");
  }

  BARRIER();

  { /* all ams test */
    int i;
    static int base = 0;
    for (i=0; i < 10; i++) {
      ALLAM_REQ(partner);

      GASNET_BLOCKUNTIL(ALLAM_DONE(base+i+1));
    }
    base += i;

    MSG("*** passed AM test!!");
  }

  BARRIER();

  /* Invoke all the atomics, once each.
   * This is a compile/link check, used to ensure that clients can link all the
   * the atomics (especially from c++ when testgasnet is built as textcxx).
   * This is distinct from testtools, which checks that these "do the right thing".
   */
  { gasnett_atomic_t val = gasnett_atomic_init(1);
    gasnett_atomic_val_t utmp = gasnett_atomic_read(&val, 0);
    gasnett_atomic_sval_t stmp = gasnett_atomic_signed(utmp);
    gasnett_atomic_set(&val, stmp, 0);
    gasnett_atomic_increment(&val, 0);
    gasnett_atomic_decrement(&val, 0);
    (void)gasnett_atomic_decrement_and_test(&val, 0);
    #ifdef GASNETT_HAVE_ATOMIC_CAS
      (void)gasnett_atomic_compare_and_swap(&val, 0, 1 ,0);
    #endif
    #ifdef GASNETT_HAVE_ATOMIC_ADD_SUB
      (void)gasnett_atomic_add(&val, 2 ,0);
      (void)gasnett_atomic_subtract(&val, 1 ,0);
    #endif
  }
  { gasnett_atomic32_t val32 = gasnett_atomic32_init(1);
    uint32_t tmp32 = gasnett_atomic32_read(&val32, 0);
    gasnett_atomic32_set(&val32, tmp32, 0);
    (void)gasnett_atomic32_compare_and_swap(&val32, 0, 1 ,0);
  }
  { gasnett_atomic64_t val64 = gasnett_atomic64_init(1);
    uint64_t tmp64 = gasnett_atomic64_read(&val64, 0);
    gasnett_atomic64_set(&val64, tmp64, 0);
    (void)gasnett_atomic64_compare_and_swap(&val64, 0, 1 ,0);
  }
  { /* attempt to generate alignment problems: */
    gasnett_atomic32_t *ptr32;
    uint32_t tmp32;
    gasnett_atomic64_t *ptr64;
    uint64_t tmp64;
    { struct { char c; gasnett_atomic32_t val32; } s = {0, gasnett_atomic32_init(1)};
      ptr32 = &s.val32;
      tmp32 = gasnett_atomic32_read(ptr32, 0);
      gasnett_atomic32_set(ptr32, tmp32, 0);
      (void)gasnett_atomic32_compare_and_swap(ptr32, 0, 1, 0);
    }
    { struct { char c; gasnett_atomic64_t val64; } s = {0, gasnett_atomic64_init(1)};
      ptr64 = &s.val64;
      tmp64 = gasnett_atomic64_read(ptr64, 0);
      gasnett_atomic64_set(ptr64, tmp64, 0);
      (void)gasnett_atomic64_compare_and_swap(ptr64, 0, 1, 0);
    }
    { double dbl = 1.0;
      ptr64 = (gasnett_atomic64_t *)(void *)&dbl; /* (void*) suppresses g++ warning (bug 2158) */
      tmp64 = gasnett_atomic64_read(ptr64, 0);
      gasnett_atomic64_set(ptr64, tmp64, 0);
      (void)gasnett_atomic64_compare_and_swap(ptr64, 0, 1, 0);
    }
    { struct { char c; double dbl; } s = {0, 1.0};
      ptr64 = (gasnett_atomic64_t *)(void *)&s.dbl; /* (void*) suppresses g++ warning (bug 2158) */
      tmp64 = gasnett_atomic64_read(ptr64, 0);
      gasnett_atomic64_set(ptr64, tmp64, 0);
      (void)gasnett_atomic64_compare_and_swap(ptr64, 0, 1, 0);
    }
  }

  /* Serial tests of optional internal 128-bit atomics have
   * moved to gasnet_diagnostic.c (run from testinternal).
   */
  
  BARRIER();
}
