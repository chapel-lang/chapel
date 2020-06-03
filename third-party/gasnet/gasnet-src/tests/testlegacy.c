/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testlegacy.c $
 * Description: General GASNet correctness tests for legacy api
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet.h>
#include <gasnet_tools.h>

/* limit segsz to prevent stack overflows for seg_everything tests */
#define TEST_MAXTHREADS 1
#include <test.h>

#define TEST_GASNET 1
#define SHORT_REQ_BASE 128
#include <testam.h>

/* Define to get one big function that pushes the gcc inliner heursitics */
#undef TESTGASNET_NO_SPLIT

#if PLATFORM_COMPILER_PGI_CXX
  // suppress warnings on PGI C++ 19.10/macos about intentional constant controlling expressions
  #pragma diag_suppress 236
#endif

TEST_BACKTRACE_DECLS();

void doit(int partner, int *partnerseg);
void doit1(int partner, int *partnerseg);
void doit2(int partner, int *partnerseg);
void doit3(int partner, int *partnerseg);
void doit4(int partner, int32_t *partnerseg);
void doit5(int partner, int *partnerseg);
void doit6(int partner, int *partnerseg);
void doit7(int partner, int *partnerseg);

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
  #define MAX_THREADS 10
#else
  #define MAX_THREADS 1
#endif
int num_threads = MAX_THREADS;

void test_threadinfo(int threadid, int numthreads) {
  int i;
  gasnet_threadinfo_t my_ti;
  static gasnet_threadinfo_t all_ti[MAX_THREADS];

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
  assert(threadid < numthreads && numthreads <= MAX_THREADS);
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
    PTHREAD_LOCALBARRIER(num_threads);
    test_threadinfo(idx, num_threads);
    PTHREAD_LOCALBARRIER(num_threads);
  #if GASNETI_ARCH_ALTIX
    /* Don't pin threads because system is either shared or using cgroups */
  #elif GASNETI_ARCH_IBMPE
    /* Don't pin threads because system s/w will have already done so */
  #else
    if (gasnett_getenv_yesno_withdefault("GASNET_TEST_SET_AFFINITY",1)) {
      // We can do little more than test for lack of crash here.
      // We will warn if the call fails on a platforms we support.
      // However, it is an ERROR if the call returns success when
      // GASNETT_SET_AFFINITY_SUPPORT is not defined.
      int rc = gasnett_set_affinity(idx);
    #if GASNETT_SET_AFFINITY_SUPPORT
      if (rc) {
        MSG("*** WARNING - gasnett_set_affinity() failed unexpectedly, possibly due to running in an environment which has already pinned processes.  One may set GASNET_TEST_SET_AFFINITY=0 to skip this test.");
      }
    #else
      if (!rc) {
        MSG("*** ERROR - GASNETT_SET_AFFINITY RETURNED SUCCESS UNEXPECTEDLY!!!!!");
      }
    #endif
    }
  #endif
    PTHREAD_LOCALBARRIER(num_threads);
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
  #if GASNET_DEBUGMALLOC
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
    num_threads = test_thread_limit(num_threads);
    test_createandjoin_pthreads(num_threads, &test_libgasnetpar_tools, NULL, 0);
  #endif
  MSG("*** passed libgasnet_tools test!!");
}
/* ------------------------------------------------------------------------------------ */
int main(int argc, char **argv) {
  uintptr_t local_segsz, global_segsz;
  int partner;
  
  gasnet_handlerentry_t handlers[] = { EVERYTHING_SEG_HANDLERS() ALLAM_HANDLERS() };

  GASNET_Safe(gasnet_init(&argc, &argv));
  local_segsz = gasnet_getMaxLocalSegmentSize();
  global_segsz = gasnet_getMaxGlobalSegmentSize();
  #if GASNET_SEGMENT_EVERYTHING
    assert_always(local_segsz == (uintptr_t)-1);
    assert_always(global_segsz == (uintptr_t)-1);
  #else
    assert_always(local_segsz >= global_segsz);
    assert_always(local_segsz % GASNET_PAGESIZE == 0);
    assert_always(global_segsz % GASNET_PAGESIZE == 0);
    assert_always(global_segsz > 0);
  #endif
#if 0
  assert_always(GASNET_ERR_NOT_INIT == gasnet_init(&argc, &argv)); /* Duplicate init */
#endif

  GASNET_Safe(gasnet_attach(handlers, sizeof(handlers)/sizeof(gasnet_handlerentry_t), 
                            TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
#if 0
  assert_always(GASNET_ERR_NOT_INIT == /* Duplicate attach */
                gasnet_attach(handlers, sizeof(handlers)/sizeof(gasnet_handlerentry_t), 
                              TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
#endif

  test_init("testlegacy",0,"");
  assert(TEST_SEGSZ >= 2*sizeof(int)*NUMHANDLERS_PER_TYPE);

  /* Legacy expansion of TEST_PRINT_CONDUITINFO(): */
  MSG0("%s conduit: v%s GASNET_ALIGNED_SEGMENTS=%i\n"
       " gasnet_AMMaxArgs():        %i\n"
       " gasnet_AMMaxMedium():      %i\n"
       " gasnet_AMMaxLongRequest(): %i\n"
       " gasnet_AMMaxLongReply():   %i" ,
       _STRINGIFY(GASNET_CORE_NAME), _STRINGIFY(GASNET_CORE_VERSION),
       GASNET_ALIGNED_SEGMENTS,
       (int)gasnet_AMMaxArgs(),
       (int)gasnet_AMMaxMedium(),
       (int)gasnet_AMMaxLongRequest(),
       (int)gasnet_AMMaxLongReply());

  { char lstr[50], gstr[50];
    gasnett_format_number(local_segsz, lstr, sizeof(lstr), 1);
    gasnett_format_number(global_segsz, gstr, sizeof(gstr), 1);
    MSG0(" MaxLocalSegmentSize on node0:  %s\n"
         " MaxGlobalSegmentSize:          %s",
         lstr, gstr);
  }
  BARRIER();

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
  BARRIER();

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

  // Verify documented equivalences provided by g2ex
  // DO NOT CHANGE IDENTIFIER PREFIXES IN THIS FUNCTION

  // verify that two types (and optionally two integer constant expressions)
  // are all the same type and fully equal / assignment compatible
  #define assert_equal(t1, t2, c1, c2) do {       \
    static t1 volatile v1 = c2;                   \
    static t2 volatile v2 = c1;                   \
    t1 volatile * const p1 = &v2;                 \
    t2 volatile * const p2 = &v1;                 \
    test_static_assert(sizeof(t1) == sizeof(t2)); \
    assert_always(c1 == c2);                      \
    assert_always(v1 == c1);                      \
    assert_always(v2 == c2);                      \
    v1 = v2; v2 = v1;                             \
    assert_always(v1 == v2);                      \
    assert_always(*p1 == *p2);                    \
  } while (0)

  #define assert_equal_nonscalar(t1, t2, c1, c2) do {       \
    static t1 v1 = c2;                            \
    static t2 v2 = c1;                            \
    t1 * const p1 = &v2;                          \
    t2 * const p2 = &v1;                          \
    test_static_assert(sizeof(t1) == sizeof(t2)); \
    assert_always(!memcmp(p1,p2,sizeof(t1)));     \
    v1 = v2; v2 = v1;                             \
    assert_always(!memcmp(p1,p2,sizeof(t1)));     \
  } while (0)
    
  #define assert_field_compat(t1, f1, t2, f2) do {     \
    static union { t1 v1; t2 v2; } u;                  \
    static t1 v1;                                      \
    static t2 v2;                                      \
    test_static_assert(sizeof(t1) == sizeof(t2));      \
    assert_always((void*)&u.v1 == (void*)&u.v2);       \
    assert_always(sizeof(u.v1.f1) == sizeof(u.v2.f2)); \
    assert_always(&u.v1.f1 == &u.v2.f2);               \
    v1.f1 = v2.f2; v2.f2 = v1.f1;                      \
  } while (0)

  // types
  assert_equal(gasnet_handle_t, gex_Event_t, 
               GASNET_INVALID_HANDLE, GEX_EVENT_INVALID);
  assert_equal(gasnet_node_t, gex_Rank_t, 
               GEX_RANK_INVALID, GEX_RANK_INVALID);
  assert_always(GASNET_MAXNODES <= GEX_RANK_INVALID);
  assert_equal(gasnet_token_t, gex_Token_t, NULL, NULL);
  assert_equal(gasnet_handler_t, gex_AM_Index_t, 0, 0);
  assert_equal(gasnet_handlerarg_t, gex_AM_Arg_t, 0, 0);
  assert_equal(gasnet_register_value_t, gex_RMA_Value_t, 
               SIZEOF_GASNET_REGISTER_VALUE_T, SIZEOF_GEX_RMA_VALUE_T);
  assert_equal_nonscalar(gasnet_hsl_t, gex_HSL_t,
               GASNET_HSL_INITIALIZER, GEX_HSL_INITIALIZER);
  assert_field_compat(gasnet_memvec_t, addr, gex_Memvec_t, gex_addr);
  assert_field_compat(gasnet_memvec_t, len,  gex_Memvec_t, gex_len);

  // AM limits
  assert_always(MIN(gex_AM_LUBRequestMedium(),gex_AM_LUBReplyMedium()) == gasnet_AMMaxMedium());
  assert_always(gex_AM_LUBRequestLong() == gasnet_AMMaxLongRequest());
  assert_always(gex_AM_LUBReplyLong() == gasnet_AMMaxLongReply());

  // Rank-vs-node
  assert_always(gex_System_QueryJobRank() == gasnet_mynode());
  assert_always(gex_System_QueryJobSize() == gasnet_nodes());

  // {Nbrhd,Host}Info and MyPosition vs getNodeInfo
  {
    gasnet_nodeinfo_t *nodeinfo = (gasnet_nodeinfo_t *)
                                  test_malloc(gasnet_nodes() * sizeof(gasnet_nodeinfo_t));
    GASNET_Safe(gasnet_getNodeInfo(nodeinfo, gasnet_nodes()));
    gasnet_node_t mysupernode_id = nodeinfo[mynode].supernode;
    gasnet_node_t mysupernode_size = 0;
    gasnet_node_t mysupernode_rank = GEX_RANK_INVALID;
    gasnet_node_t myhost_id = nodeinfo[mynode].host;
    gasnet_node_t myhost_size = 0;
    gasnet_node_t myhost_rank = GEX_RANK_INVALID;
    for (gasnet_node_t i = 0; i < gasnet_nodes(); ++i) {
      if (i == gasnet_mynode()) {
        mysupernode_rank = mysupernode_size;
        myhost_rank = myhost_size;
      }
      if (nodeinfo[i].supernode == mysupernode_id) mysupernode_size += 1;
      if (nodeinfo[i].host      == myhost_id)      myhost_size      += 1;
    }
    assert_always(mysupernode_size >= 1);
    assert_always(mysupernode_rank != GEX_RANK_INVALID);
    assert_always(myhost_size >= 1);
    assert_always(myhost_rank != GEX_RANK_INVALID);

    gex_RankInfo_t *neighbor_array;
    gex_Rank_t neighbor_size, neighbor_rank;
    gex_System_QueryNbrhdInfo(&neighbor_array, &neighbor_size, &neighbor_rank);
    assert_always(neighbor_size == mysupernode_size);
    assert_always(neighbor_rank == mysupernode_rank);
    assert_always(neighbor_array != NULL);
    for (gasnet_node_t i = 0; i < mysupernode_size; ++i) {
      assert_always(nodeinfo[neighbor_array[i].gex_jobrank].supernode == mysupernode_id);
    }

    gex_RankInfo_t *host_array;
    gex_Rank_t host_size, host_rank;
    gex_System_QueryHostInfo(&host_array, &host_size, &host_rank);
    assert_always(host_size == myhost_size);
    assert_always(host_rank == myhost_rank);
    assert_always(host_array != NULL);
    for (gasnet_node_t i = 0; i < myhost_size; ++i) {
      assert_always(nodeinfo[host_array[i].gex_jobrank].host == myhost_id);
    }

    gasnet_node_t max_supernode = 0;
    gasnet_node_t max_host = 0;
    for (gasnet_node_t i = 0; i < gasnet_nodes(); ++i) {
      max_supernode = MAX(max_supernode, nodeinfo[i].supernode);
      max_host      = MAX(max_host     , nodeinfo[i].host     );
    }
    gex_Rank_t n_size, n_rank, h_size, h_rank;
    gex_System_QueryMyPosition(&n_size, &n_rank, &h_size, &h_rank);
    assert_always(n_size == 1 + max_supernode);
    assert_always(n_rank == nodeinfo[gasneti_mynode].supernode);
    assert_always(h_size == 1 + max_host);
    assert_always(h_rank == nodeinfo[gasneti_mynode].host);

    test_free(nodeinfo);
  }

  // GEX objects
  { gex_Client_t  client;
    gex_EP_t      endpoint;
    gex_TM_t      tm;
    gex_Segment_t segment;
    gasnet_QueryGexObjects(&client, &endpoint, &tm, &segment);
    assert_always(gex_EP_QueryClient(endpoint) == client);
    assert_always(gex_TM_QueryClient(tm) == client);
    assert_always(gex_TM_QueryEP(tm) == endpoint);
  #if !GASNET_SEGMENT_EVERYTHING
    assert_always(gex_Segment_QueryClient(segment) == client);
    assert_always(gex_Segment_QueryAddr(segment) == TEST_MYSEG());
    assert_always(gex_Segment_QuerySize(segment) >= TEST_SEGSZ_REQUEST);
  #endif
  }

  MSG("*** passed object test!!");

#ifndef TESTGASNET_NO_SPLIT
  doit1(partner, partnerseg);
}
void doit1(int partner, int *partnerseg) {
  int mynode = gasnet_mynode();
#endif

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
  doit4(partner, (int32_t *)partnerseg);
}
void doit4(int partner, int32_t *partnerseg) {
  /* int mynode = gasnet_mynode(); UNUSED */
#endif

  BARRIER();

  { /*  memset test */
    GASNET_BEGIN_FUNCTION();
    int i, success=1;
    int32_t vals[300];

    gasnet_memset(partner, partnerseg, 0x55, 100*sizeof(int32_t));
    gasnet_wait_syncnb(gasnet_memset_nb(partner, partnerseg+100, 0x66, 100*sizeof(int32_t)));
    gasnet_memset_nbi(partner, partnerseg+200, 0x77, 100*sizeof(int32_t));
    gasnet_wait_syncnbi_puts();

    gasnet_get(&vals, partner, partnerseg, 300*sizeof(int32_t));

    for (i=0; i < 100; i++) {
      const int32_t five  = 0x55555555;
      const int32_t six   = 0x66666666;
      const int32_t seven = 0x77777777;
      if (vals[i] != five) {
        MSG("*** ERROR - FAILED MEMSET TEST!!!");
        success = 0;
      }
      if (vals[i+100] != six) {
        MSG("*** ERROR - FAILED MEMSET TEST!!!");
        success = 0;
      }
      if (vals[i+200] != seven) {
        MSG("*** ERROR - FAILED MEMSET TEST!!!");
        success = 0;
      }
    }
    if (success) MSG("*** passed memset test!!");
  }

#ifndef TESTGASNET_NO_SPLIT
  doit5(partner, (int *)partnerseg);
}
void doit5(int partner, int *partnerseg) {
  int mynode = gasnet_mynode();
#endif

  BARRIER();

  /* NB and NBI put/overwrite/get tests */
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
              MSG("*** ERROR - FAILED OUT-OF-SEG PUT_NB/OVERWRITE TEST!!! sz=%i j=%i (got=%016" PRIx64 " expected=%016" PRIx64 ")", (sz), j,
                  localpos[j], val);
              success = 0;
          }
          ok = segpos[j] == val;
          if (sz < 8) ok = !memcmp(&(segpos[j]), &val, sz);
          if (!ok) {
              MSG("*** ERROR - FAILED IN-SEG PUT_NB/OVERWRITE TEST!!! sz=%i j=%i (got=%016" PRIx64 " expected=%016" PRIx64 ")", (sz), j,
                  segpos[j], val);
              success = 0;
          }
        }
      }
    }
    test_free(localvals);
    if (success) MSG("*** passed nb put/overwrite test!!");
  }
  { GASNET_BEGIN_FUNCTION();
    uint64_t *localvals=(uint64_t *)test_malloc(SEGSZ);
    int success = 1;
    int i, sz;
    for (i = 0; i < MAX(1,iters/10); i++) {
      uint64_t *localpos=localvals;
      uint64_t *segpos=(uint64_t *)TEST_MYSEG();
      uint64_t *rsegpos=(uint64_t *)((char*)partnerseg+SEGSZ);
      for (sz = 1; sz <= MAXSZ; sz*=2) {
        int elems = sz/8;
        int j;
        uint64_t val = VAL(sz, i+91); /* setup known src value, different from NB test */
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
        gasnet_put_nbi_bulk(partner, rsegpos, localpos, sz);
        gasnet_wait_syncnbi_puts();

        gasnet_put_nbi(partner, rsegpos+elems, localpos, sz);
        memset(localpos, 0xCC, sz); /* clear */
        gasnet_wait_syncnbi_puts();

        gasnet_put_nbi_bulk(partner, rsegpos+2*elems, segpos, sz);
        gasnet_wait_syncnbi_puts();

        gasnet_put_nbi(partner, rsegpos+3*elems, segpos, sz);
        memset(segpos, 0xCC, sz); /* clear */
        gasnet_wait_syncnbi_puts();

        gasnet_get_nbi(localpos, partner, rsegpos, sz);
        gasnet_wait_syncnbi_gets();
        gasnet_get_nbi_bulk(localpos+elems, partner, rsegpos+elems, sz);
        gasnet_wait_syncnbi_gets();
        gasnet_get_nbi(segpos, partner, rsegpos+2*elems, sz);
        gasnet_wait_syncnbi_gets();
        gasnet_get_nbi_bulk(segpos+elems, partner, rsegpos+3*elems, sz);
        gasnet_wait_syncnbi_gets();

        for (j=0; j < elems*2; j++) {
          int ok;
          ok = localpos[j] == val;
          if (sz < 8) ok = !memcmp(&(localpos[j]), &val, sz);
          if (!ok) {
              MSG("*** ERROR - FAILED OUT-OF-SEG PUT_NBI/OVERWRITE TEST!!! sz=%i j=%i (got=%016" PRIx64 " expected=%016" PRIx64 ")", (sz), j,
                  localpos[j], val);
              success = 0;
          }
          ok = segpos[j] == val;
          if (sz < 8) ok = !memcmp(&(segpos[j]), &val, sz);
          if (!ok) {
              MSG("*** ERROR - FAILED IN-SEG PUT_NBI/OVERWRITE TEST!!! sz=%i j=%i (got=%016" PRIx64 " expected=%016" PRIx64 ")", (sz), j,
                  segpos[j], val);
              success = 0;
          }
        }
      }
    }
    test_free(localvals);
    if (success) MSG("*** passed nbi put/overwrite test!!");
  }

#ifndef TESTGASNET_NO_SPLIT
  doit6(partner, (int *)partnerseg);
}
void doit6(int partner, int *partnerseg) {
#endif

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

#ifndef TESTGASNET_NO_SPLIT
  doit7(partner, (int *)partnerseg);
}
void doit7(int partner, int *partnerseg) {
#endif

  BARRIER();

  /* Invoke all the atomics, once each.
   * This is a compile/link check, used to ensure that clients can link all the
   * the atomics (especially from c++ when testgasnet is built as textcxx).
   * This is distinct from testtools, which checks that these "do the right thing".
   */
  #ifndef GASNETT_HAVE_ATOMIC_CAS
    #define gasnett_atomic_compare_and_swap(a,b,c,d) ((void)0)
    #define gasnett_atomic_swap(a,b,c)               ((void)0)
  #endif
  #ifndef GASNETT_HAVE_ATOMIC_ADD_SUB
    #define gasnett_atomic_add(a,b,c)                ((void)0)
    #define gasnett_atomic_subtract(a,b,c)           ((void)0)
  #endif
  #ifndef GASNETT_HAVE_STRONGATOMIC_CAS
    #define gasnett_strongatomic_compare_and_swap(a,b,c,d) ((void)0)
    #define gasnett_strongatomic_swap(a,b,c)               ((void)0)
  #endif
  #ifndef GASNETT_HAVE_STRONGATOMIC_ADD_SUB
    #define gasnett_strongatomic_add(a,b,c)                ((void)0)
    #define gasnett_strongatomic_subtract(a,b,c)           ((void)0)
  #endif
  #define TEST_ATOMICS(scalar,class) do { \
    gasnett_##class##_t val = gasnett_##class##_init(1);             \
    scalar tmp = gasnett_##class##_read(&val, 0);                    \
    gasnett_##class##_set(&val, tmp, 0);                             \
    gasnett_##class##_increment(&val, 0);                            \
    gasnett_##class##_decrement(&val, 0);                            \
    (void) gasnett_##class##_decrement_and_test(&val, 0);            \
    (void) gasnett_##class##_compare_and_swap(&val, 0, 1 ,0);        \
    (void) gasnett_##class##_swap(&val, 1 ,0);                       \
    (void) gasnett_##class##_add(&val, tmp ,0);                      \
    (void) gasnett_##class##_subtract(&val, tmp ,0);                 \
  } while(0)
  {
    gasnett_atomic_sval_t stmp = gasnett_atomic_signed((gasnett_atomic_val_t)0);
    test_mark_used(stmp);
    TEST_ATOMICS(gasnett_atomic_val_t, atomic);
    TEST_ATOMICS(gasnett_atomic_val_t, strongatomic);
    TEST_ATOMICS(uint32_t, atomic32);
    TEST_ATOMICS(uint32_t, strongatomic32);
    TEST_ATOMICS(uint64_t, atomic64);
    TEST_ATOMICS(uint64_t, strongatomic64);
  }

  /* Serial tests of optional internal 128-bit atomics have
   * moved to gasnet_diagnostic.c (run from testinternal).
   */
  
  BARRIER();
}
