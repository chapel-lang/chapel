/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testgasnet.c $
 * Description: General GASNet correctness tests
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>
#include <gasnet_ratomic.h>
#include <gasnet_tools.h>

/* limit segsz to prevent stack overflows for seg_everything tests */
#define TEST_MAXTHREADS 1
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ (128*1024) /* for put/overwrite test */
#endif
#include <test.h>

#define TEST_GASNETEX 1
#define SHORT_REQ_BASE GEX_AM_INDEX_BASE
test_static_assert_file(GEX_AM_INDEX_BASE <= 128);
#include <testam.h>

/* Define to get one big function that pushes the gcc inliner heursitics */
#undef TESTGASNET_NO_SPLIT

#if !defined(GASNET_HIDDEN_AM_CONCURRENCY_LEVEL)
#error Missing GASNET_HIDDEN_AM_CONCURRENCY_LEVEL definition
#endif

#if PLATFORM_COMPILER_PGI_CXX
  // suppress warnings on PGI C++ 19.10/macos about intentional constant controlling expressions
  #pragma diag_suppress 236
#endif

TEST_BACKTRACE_DECLS();

void doit(int partner, int *partnerseg);
void doit0(int partner, int *partnerseg);
void doit1(int partner, int *partnerseg);
void doit2(int partner, int *partnerseg);
void doit3(int partner, int *partnerseg);
/*void doit4(int partner, int *partnerseg); -- removed along with the memset*() calls */
void doit5(int partner, int *partnerseg);
void doit6(int partner, int *partnerseg);
void doit7(int partner, int *partnerseg);

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

static gex_Rank_t myrank;
static gex_Rank_t numranks;

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
  GASNETT_EXTERNC void seg_everything_reqh(gex_Token_t token) {
    gex_AM_ReplyMedium0(token, 251, &myinfo, sizeof(test_everything_seginfo_t), GEX_EVENT_NOW, 0);
  }
  GASNETT_EXTERNC void seg_everything_reph(gex_Token_t token, void *buf, size_t nbytes) {
    assert(nbytes == sizeof(test_everything_seginfo_t));
    memcpy(&partnerinfo, buf, nbytes);
    gasnett_local_wmb();
    done = 1;
  }
  #define EVERYTHING_SEG_HANDLERS() \
    { 250, (handler_fn_t)seg_everything_reqh, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT, 0, NULL, NULL }, \
    { 251, (handler_fn_t)seg_everything_reph, GEX_FLAG_AM_REPLY|GEX_FLAG_AM_MEDIUM, 0, NULL, NULL },

  char _static_seg[TEST_SEGSZ+PAGESZ] = {1};
  char _common_seg[TEST_SEGSZ+PAGESZ];
  void everything_tests(int partner) {
    char _stack_seg[TEST_SEGSZ+PAGESZ];

    if (myrank == 0) MSG("*** gathering data segment info for SEGMENT_EVERYTHING tests...");
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
    gex_AM_RequestShort0(myteam, (gex_Rank_t)partner, 250, 0);
    GASNET_BLOCKUNTIL(done);
    BARRIER();

    /* test that remote access works will all the various data areas */
    if (myrank == 0) MSG(" --- testgasnet w/ static data area ---");
    doit(partner, (int*)partnerinfo.static_seg);
    if (myrank == 0) MSG(" --- testgasnet w/ common block data area ---");
    doit(partner, (int*)partnerinfo.common_seg);
    if (myrank == 0) MSG(" --- testgasnet w/ malloc data area ---");
    doit(partner, (int*)partnerinfo.malloc_seg);
    if (myrank == 0) MSG(" --- testgasnet w/ sbrk data area ---");
    doit(partner, (int*)partnerinfo.sbrk_seg);
    #ifdef HAVE_MMAP
      if (myrank == 0) MSG(" --- testgasnet w/ mmap'd data area ---");
      doit(partner, (int*)partnerinfo.mmap_seg);
    #endif
    if (myrank == 0) MSG(" --- testgasnet w/ stack data area ---");
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
  #if GASNETI_ARCH_IBMPE
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
static const char *clientname = "testgasnet";
static const gex_Flags_t clientflags = 0;
int main(int argc, char **argv) {
  uintptr_t local_segsz, global_segsz;
  int partner;
  
  TEST_SRAND(((unsigned int)TIME()) & 0xFFFF);

  gex_AM_Entry_t handlers[] = { EVERYTHING_SEG_HANDLERS() ALLAM_HANDLERS() };

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, clientname, &argc, &argv, clientflags));
  if (GEX_SEGMENT_INVALID != gex_EP_QuerySegment(myep)) {
    MSG("*** ERROR - FAILED EP NO-SEGMENT TEST!!!!!");
  }

  myrank = gex_TM_QueryRank(myteam);
  numranks = gex_TM_QuerySize(myteam);

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

  { uintptr_t size = (uintptr_t)-5;
    void *owneraddr = (void*)&size;
    void *localaddr = (void*)&size;

    // No segments have been created/bound yet.
    // Local and remote bound-segment queries must return non-zero and preserve output locations.
    gex_Rank_t peer = (myrank + 1) % numranks;
    if (!gex_Segment_QueryBound(myteam, myrank, &owneraddr, &localaddr, &size) ||
        !gex_Segment_QueryBound(myteam, peer,   &owneraddr, &localaddr, &size) ||
        owneraddr != (void*)&size || localaddr != (void*)&size || size != (uintptr_t)-5) {
      MSG("*** ERROR - FAILED NO BOUND SEGMENT TEST!!!!!");
    }
    BARRIER();
  }

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  GASNET_Safe(gex_EP_RegisterHandlers(myep, handlers, sizeof(handlers)/sizeof(gex_AM_Entry_t)));

  test_init("testgasnet",0,"");
  assert(TEST_SEGSZ >= 2*sizeof(int)*NUMHANDLERS_PER_TYPE);

  TEST_PRINT_CONDUITINFO();
  { char lstr[50], gstr[50];
    gasnett_format_number(local_segsz, lstr, sizeof(lstr), 1);
    gasnett_format_number(global_segsz, gstr, sizeof(gstr), 1);
    MSG0(" MaxLocalSegmentSize on node0:  %s\n"
         " MaxGlobalSegmentSize:          %s",
         lstr, gstr);
  }
  BARRIER();

  { int smaj = GEX_SPEC_VERSION_MAJOR;
    int smin = GEX_SPEC_VERSION_MINOR;
    int rmaj = GASNET_RELEASE_VERSION_MAJOR;
    int rmin = GASNET_RELEASE_VERSION_MINOR;
    int rpat = GASNET_RELEASE_VERSION_PATCH;
    // TODO-EX: (smaj > 0) when we reach 1.0
    assert_always(smaj >= 0 && smin >= 0 && rmaj > 0 && rmin >= 0 && rpat >= 0);
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
  partner = (myrank + 1) % numranks;
  #if GASNET_SEGMENT_EVERYTHING
    everything_tests(partner);
  #else
    doit(partner, (int *)TEST_SEG(partner));
  #endif

  MSG("done.");

  gasnet_exit(0);
  return 0;
}

gex_Event_t *am_lcopt[] = { GEX_EVENT_NOW, GEX_EVENT_GROUP, NULL };
gex_Flags_t  am_flags[] = { GEX_FLAG_IMMEDIATE, 0,
                            GEX_FLAG_IMMEDIATE | GEX_FLAG_AM_PREPARE_LEAST_CLIENT,
                            GEX_FLAG_AM_PREPARE_LEAST_CLIENT,
                            GEX_FLAG_IMMEDIATE | GEX_FLAG_AM_PREPARE_LEAST_ALLOC,
                            GEX_FLAG_AM_PREPARE_LEAST_ALLOC };
#define AM_LCOPT_CNT ((int)(sizeof(am_lcopt)/sizeof(am_lcopt[0])))
#define AM_FLAGS_CNT ((int)(sizeof(am_flags)/sizeof(am_flags[0])))
typedef struct { 
  uint32_t RequestMedium[AM_LCOPT_CNT][AM_FLAGS_CNT]; // uint32_t to ensure the struct fits under 512 bytes
  uint32_t ReplyMedium[AM_LCOPT_CNT][AM_FLAGS_CNT];
  size_t RequestLong[AM_LCOPT_CNT][AM_FLAGS_CNT];
  size_t ReplyLong[AM_LCOPT_CNT][AM_FLAGS_CNT];
} amsz_t;

extern gex_AM_Entry_t sizecheck_handlers[];
GASNETT_EXTERNC void sizecheck_reqh(gex_Token_t token, void *buf, size_t nbytes, gex_AM_Arg_t args) {
  gex_Rank_t r = test_msgsource(token);
  assert_always(r < numranks);
  assert_always(args >= 0 && args <= (gex_AM_Arg_t)gex_AM_MaxArgs());
  assert_always(nbytes == sizeof(amsz_t));
  amsz_t *max = (amsz_t *)buf;
  
  // verify that AMMax*() return symmetric results in both directions
  for (int lci = 0; lci < AM_LCOPT_CNT; lci++) {
    for (int flagsi = 0; flagsi < AM_FLAGS_CNT; flagsi++) {
      #define CHECK_MAX(cat) do {                                                             \
        gex_Flags_t flags = am_flags[flagsi];                                                 \
        gex_Event_t *lcopt = am_lcopt[lci];                                                   \
        if ((lcopt == GEX_EVENT_GROUP) && strstr(#cat, "Reply")) break;                       \
        size_t val = gex_AM_Max##cat(myteam, r, lcopt, flags, args);                          \
        if (val != max->cat[lci][flagsi])                                                     \
              MSG("*** ERROR - FAILED MAX SYMMETRY TEST! args=%i lci=%i flagsi=%i",           \
                  args,lci,flagsi);                                                           \
        if (flags & (GEX_FLAG_AM_PREPARE_LEAST_CLIENT |                                       \
                     GEX_FLAG_AM_PREPARE_LEAST_ALLOC)) break; /* exclude from LUB */          \
        size_t lubval = gex_AM_LUB##cat();                                                    \
        if (val < lubval)                                                                     \
             MSG("*** ERROR - FAILED HANDLER LUB/MAX TEST! args=%i rank=%i lci=%i flagsi=%i", \
                  args,(int)r,lci,flagsi);                                                    \
      } while (0)
      CHECK_MAX(RequestMedium);
      CHECK_MAX(ReplyMedium);
      CHECK_MAX(RequestLong);
      CHECK_MAX(ReplyLong);
    } // flags
  } // lc
  #undef CHECK_MAX
  gex_AM_ReplyShort0(token, sizecheck_handlers[1].gex_index, 0);
}
gasnett_atomic_t sizecheck_ack = gasnett_atomic_init(0);
GASNETT_EXTERNC void sizecheck_reph(gex_Token_t token) {
  assert_always(gasnett_atomic_read(&sizecheck_ack,0) > 0);
  gasnett_atomic_decrement(&sizecheck_ack,0);
}
gex_AM_Entry_t sizecheck_handlers[] = { // deliberately registered as don't-care indexes
 { 0, (handler_fn_t)sizecheck_reqh, GEX_FLAG_AM_MEDIUM|GEX_FLAG_AM_REQUEST, 1, 0, "sizecheck_reqh" },
 { 0, (handler_fn_t)sizecheck_reph, GEX_FLAG_AM_SHORT|GEX_FLAG_AM_REPLY, 0, 0, "sizecheck_reph" },
};

void doit(int partner, int *partnerseg) {
  BARRIER();

  // check predefined object constants
  #define CHECK_ZERO_CONSTANT(type, constant) do { \
    static type vz;                                \
    type v = constant;                             \
    test_static_assert(sizeof(constant) == sizeof(type));  \
    assert_always(sizeof(constant) == sizeof(v));  \
    assert_always(!memcmp(&v,&vz,sizeof(type)));   \
  } while (0)
  CHECK_ZERO_CONSTANT(gex_Segment_t, GEX_SEGMENT_INVALID);

  if (strcmp(clientname, gex_Client_QueryName(myclient))) {
    MSG("*** ERROR - FAILED CLIENT NAME TEST!!!!!");
  }
  if (clientflags != gex_Client_QueryFlags(myclient)) {
    MSG("*** ERROR - FAILED CLIENT FLAGS TEST!!!!!");
  }
  if (myclient != gex_EP_QueryClient(myep)) {
    MSG("*** ERROR - FAILED EP CLIENT TEST!!!!!");
  }
  if (myclient != gex_TM_QueryClient(myteam)) {
    MSG("*** ERROR - FAILED TM CLIENT TEST!!!!!");
  }
  if (myep != gex_TM_QueryEP(myteam)) {
    MSG("*** ERROR - FAILED TM EP TEST!!!!!");
  }

  #define TEST_CDATA(type,var) do {                                       \
    static char *cdata_##type = 0;                                        \
    if ((char *)gex_##type##_QueryCData(var) != cdata_##type)             \
      MSG("*** ERROR - FAILED %s TEST!!!!!", "gex_" #type "_QueryCData"); \
    cdata_##type = strdup(#type " cdata");                                \
    gex_##type##_SetCData(var, cdata_##type);                             \
    char *temp = (char *)gex_##type##_QueryCData(var);                    \
    if (temp != cdata_##type)                                             \
      MSG("*** ERROR - FAILED %s TEST!!!!!", "gex_" #type "_SetCData");   \
  } while(0)

  TEST_CDATA(Client,myclient);
  TEST_CDATA(EP,myep);
  TEST_CDATA(TM,myteam);

#if GASNET_SEGMENT_EVERYTHING
  // test.h intercepted gex_Segment_Attach() but does not fake a gex_Segment_t
#else
  if (myclient != gex_Segment_QueryClient(mysegment)) {
    MSG("*** ERROR - FAILED SEGMENT CLIENT TEST!!!!!");
  }
  if (mysegment != gex_EP_QuerySegment(myep)) {
    MSG("*** ERROR - FAILED EP SEGMENT TEST!!!!!");
  }
  TEST_CDATA(Segment,mysegment);

  { void *owneraddr, *localaddr;
    uintptr_t size;

    // Local bound-segment query must return 0 and give same data as direct queries
    if (gex_Segment_QueryBound(myteam, myrank, &owneraddr, &localaddr, &size) ||
        size      != gex_Segment_QuerySize(mysegment) ||
        owneraddr != gex_Segment_QueryAddr(mysegment) ||
        owneraddr != localaddr) {
      MSG("*** ERROR - FAILED LOCAL BOUND SEGMENT TEST!!!!!");
    }

    {
      gex_Rank_t peer = (myrank + 1) % numranks;
      size = 0;
      owneraddr = NULL;
      localaddr = (void*)&size;
      // Remote bound-segment query must return 0 and set all outputs to "plausible" values
      if (gex_Segment_QueryBound(myteam, peer, &owneraddr, &localaddr, &size) ||
          !size || !owneraddr || localaddr == (void*)&size) {
        MSG("*** ERROR - FAILED REMOTE BOUND SEGMENT TEST!!!!!");
      }
    }
  }

  // To be removed:
  assert(gex_Segment_QueryAddr(mysegment) == TEST_MYSEG());
  assert(gex_Segment_QuerySize(mysegment) >= TEST_SEGSZ_REQUEST);
#endif

#if !GASNET_CONDUIT_SMP
  {
    CHECK_ZERO_CONSTANT(gex_AD_t, GEX_AD_INVALID);

    gex_AD_t       ad;
    gex_DT_t       domain_type  = GEX_DT_U32;
    gex_OP_t       domain_ops   = GEX_OP_FADD|GEX_OP_SWAP;
    gex_Flags_t    domain_flags = GEX_FLAG_PEER_SEG_OFFSET;
    gex_AD_Create(&ad, myteam, domain_type, domain_ops, domain_flags);

    if (myteam != gex_AD_QueryTM(ad)) {
      MSG("*** ERROR - FAILED ATOMIC DOMAIN TM TEST!!!!!");
    }
    if (domain_type != gex_AD_QueryDT(ad)) {
      MSG("*** ERROR - FAILED ATOMIC DOMAIN DATATYPE TEST!!!!!");
    }
    if (domain_ops != gex_AD_QueryOps(ad)) {
      MSG("*** ERROR - FAILED ATOMIC DOMAIN OPS TEST!!!!!");
    }
    if (domain_flags != gex_AD_QueryFlags(ad)) {
      MSG("*** ERROR - FAILED ATOMIC DOMAIN FLAGS TEST!!!!!");
    }

    // Don't use TEST_CDATA because EVERYTHING reaches here multiple times w/ different objects
    if (NULL != gex_AD_QueryCData(ad)) {
       MSG("*** ERROR - FAILED gex_AD_QueryCData TEST!!!!!");
    }
    gex_AD_SetCData(ad, (void*) ad);
    if (ad != gex_AD_QueryCData(ad)) {
       MSG("*** ERROR - FAILED gex_AD_QueryCData TEST!!!!!");
    }

    gex_AD_Destroy(ad);
  }
#endif

  {
    gex_RankInfo_t *neighbor_array;
    gex_Rank_t neighbor_size, neighbor_rank;
    gex_System_QueryNbrhdInfo(&neighbor_array, &neighbor_size, &neighbor_rank);

    assert_always(neighbor_array != NULL);
    assert_always((neighbor_size > 0) && (neighbor_size <= gex_System_QueryJobSize()));
    assert_always(neighbor_rank < neighbor_size);
    assert_always(neighbor_array[neighbor_rank].gex_jobrank == gex_System_QueryJobRank());

    for (gex_Rank_t i = 0; i < neighbor_size; ++i) {
      // Check sort:
      assert_always(!i || (neighbor_array[i].gex_jobrank > neighbor_array[i-1].gex_jobrank));
    }

    gex_RankInfo_t *host_array;
    gex_Rank_t host_size, host_rank;
    gex_System_QueryHostInfo(&host_array, &host_size, &host_rank);

    assert_always(host_array != NULL);
    assert_always((host_size > 0) && (host_size <= gex_System_QueryJobSize()));
    assert_always(host_rank < host_size);
    assert_always(host_array[host_rank].gex_jobrank == gex_System_QueryJobRank());

    for (gex_Rank_t i = 0; i < host_size; ++i) {
      // Check sort:
      assert_always(!i || (host_array[i].gex_jobrank > host_array[i-1].gex_jobrank));
    }

    // Nbrhd must be a subset of Host:
    // Note that since both arrays are sorted this check is linear in time
    for (gex_Rank_t nidx = 0, hidx = 0; nidx < neighbor_size; ++nidx) {
      for (/*empty*/; hidx < host_size; ++hidx) {
        if (neighbor_array[nidx].gex_jobrank == host_array[hidx].gex_jobrank) break;
      }
      assert_always(hidx < host_size);  // fail if nbrhd member not found in host_array
    }

  #if !GASNET_SEGMENT_EVERYTHING
    // Exercise sharing to validate ranks in neighbor_array
    BARRIER();
    for (gex_Rank_t i = 0; i < neighbor_size; ++i) {
      gex_Rank_t *crossmap = NULL;
      int rc = gex_Segment_QueryBound(myteam, neighbor_array[i].gex_jobrank, NULL, (void**)&crossmap, NULL);
      assert_always(rc == 0);
      assert_always(crossmap != NULL);
      crossmap[neighbor_rank] = myrank;
    }
    BARRIER();
    gex_Rank_t *myseg = (gex_Rank_t *)TEST_MYSEG();
    for (gex_Rank_t i = 0; i < neighbor_size; ++i) {
      assert_always(neighbor_array[i].gex_jobrank == myseg[i]);
    }
    BARRIER();
  #endif
  }

  {
    gex_Rank_t n_proc = gex_System_QueryJobSize();
    gex_Rank_t n_size, n_rank, h_size, h_rank;
    gex_System_QueryMyPosition(&n_size, &n_rank, &h_size, &h_rank);
    // Ranks in both sets must be less than set size:
    assert_always(n_size > n_rank);
    assert_always(h_size > h_rank);
    // #proc >= #nbrhd >= #host:
    assert_always(n_proc >= n_size && n_size >= h_size);
  }

  /* width-independent computation of an integer variable with unknown unsigned type */
  #if PLATFORM_ARCH_LITTLE_ENDIAN
    #define compute_uint_val(lval_u64,var) do {          \
      lval_u64 = 0;                                      \
      for (size_t i=0; i < sizeof(var); i++) {           \
        lval_u64 <<= 8;                                  \
        lval_u64 |= *(((uint8_t*)(&(var)+1)) - 1 - i);   \
      }                                                  \
    } while (0)
  #else
    #define compute_uint_val(lval_u64,var) do {          \
      lval_u64 = 0;                                      \
      for (size_t i=0; i < sizeof(var); i++) {           \
        lval_u64 <<= 8;                                  \
        lval_u64 |= *(((uint8_t*)&(var)) + i);           \
      }                                                  \
    } while (0)
  #endif
    
  #define assert_inttype(type) do {              \
    volatile char a[(type)1.1f];                 \
    volatile type v = (signed char)0x55; /* warnings here mean non-compliance */ \
    assert_always((double)(type)1.1f < 1.1f);    \
    uint64_t val; compute_uint_val(val,v);       \
    assert_always(val == 0x55);                  \
  } while (0)
  #define assert_signed(type)  do {              \
    volatile type v = 0; /* prevent warnings */  \
    assert_inttype(type);                        \
    assert_always((type)(v-1) < v);              \
    test_static_assert((type)(-1) < (type)0);    \
  } while (0)
  #define assert_unsigned(type)  do {            \
    volatile type v = 0; /* prevent warnings */  \
    assert_inttype(type);                        \
    assert_always((type)(v-1) > v);              \
    test_static_assert((type)(-1) > (type)0);    \
  } while (0)
  #define assert_arr_unaliased(type, arr) do {   \
    size_t const cnt = sizeof(arr)/sizeof(type); \
    for (size_t i = 0; i < cnt; i++) {           \
      type other = 0;                            \
      for (size_t j = 0; j < cnt; j++) {         \
        if (i != j) {                            \
          other |= arr[j];                       \
        }                                        \
      }                                          \
      /* arr[i] has at least one unique bit: */  \
      assert_always((other | arr[i]) != other);  \
    }                                            \
  } while (0)
  #define assert_arr_nonzero(type, arr) do {     \
    size_t const cnt = sizeof(arr)/sizeof(type); \
    for (size_t i = 0; i < cnt; i++) {           \
      assert_always(arr[i] != 0);                \
    }                                            \
  } while (0)
  #define assert_arr_all_val(type, arr, allval) do { \
    size_t const cnt = sizeof(arr)/sizeof(type); \
    type some = 0;                               \
    for (size_t i = 0; i < cnt; i++) {           \
      some |= arr[i];                            \
    }                                            \
    assert_always((some & ~allval) == 0);        \
  } while (0)

  // Format one random mask of each possible popcount() including 0
  #define test_format(type,array,format_fn) do { \
    const int elems = sizeof(array)/sizeof(type); \
    type val = 0;                                 \
    for (int i = 0; i <= elems; ++i) {            \
      if (i) {                                    \
        type prev = val;                          \
        do {                                      \
          val |= array[TEST_RAND(0,elems-1)];     \
        } while (val == prev);                    \
      }                                           \
      size_t sz = format_fn(NULL, val);           \
      char *buf = (char*) test_malloc(sz);        \
      size_t rc = format_fn(buf, val);            \
      assert_always(rc <= sz);                    \
      assert_always(strlen(buf) < sz);            \
      test_free(buf);                             \
    }                                             \
  } while (0)

  /* sanity check macros and system types */
  assert_signed(int8_t);
  assert_signed(int16_t);
  assert_signed(int32_t);
  assert_signed(int64_t);
  assert_signed(intptr_t);
  assert_signed(ssize_t);
  assert_signed(ptrdiff_t);
  assert_unsigned(uint8_t);
  assert_unsigned(uint16_t);
  assert_unsigned(uint32_t);
  assert_unsigned(uint64_t);
  assert_unsigned(uintptr_t);
  assert_unsigned(size_t);

  /* team/rank tests */
  assert_unsigned(gex_Rank_t);
  assert(myrank == gex_TM_QueryRank(myteam));
  assert(numranks == gex_TM_QuerySize(myteam));
  assert_always(myrank == gex_System_QueryJobRank());
  assert_always(numranks == gex_System_QueryJobSize());
  assert_always(myrank < numranks);
  assert_always(numranks < GEX_RANK_INVALID);

  /* AM limit tests */
  assert_always(gex_AM_MaxArgs() >= 2*MAX(sizeof(int),sizeof(void*)));
  assert_always(gex_AM_LUBRequestMedium() >= 512);
  assert_always(gex_AM_LUBReplyMedium() >= 512);
  assert_always(gex_AM_LUBRequestLong() >= 512);
  assert_always(gex_AM_LUBReplyLong() >= 512);

  static int firsttime = 1;
  if (firsttime) {
    size_t numhand = sizeof(sizecheck_handlers)/sizeof(gex_AM_Entry_t);
    GASNET_Safe(gex_EP_RegisterHandlers(myep, sizecheck_handlers, numhand));
    const int maxidx = 255 - test_num_am_handlers; // Offset by any don't care registrations in test.h
    for (size_t i = 0; i < numhand; i++) assert_always(sizecheck_handlers[i].gex_index == maxidx - i);
    firsttime = 0;
    BARRIER();
  }
  /* verify Max >= LUB and is non-increasing as args grows */
  amsz_t lub;
  memset(&lub,-1,sizeof(lub));
  assert(sizeof(amsz_t) <= 512);
  assert(sizeof(amsz_t) <= gex_AM_LUBRequestMedium());
  for (int args = 0; args <= (int)gex_AM_MaxArgs(); args += (int)gex_AM_MaxArgs()) {
    amsz_t ranklub;
    memset(&ranklub,-1,sizeof(ranklub));
    for (gex_Rank_t d = 0; d <= numranks; d++) {
      gex_Rank_t r;
      if (d == numranks) r = GEX_RANK_INVALID; // min of maxes
      else r = (myrank + d) % numranks;
      amsz_t max;
      for (int lci = 0; lci < AM_LCOPT_CNT; lci++) {
        for (int flagsi = 0; flagsi < AM_FLAGS_CNT; flagsi++) {
          #define GET_MAX(cat) do {                                                              \
            gex_Flags_t flags = am_flags[flagsi];                                                \
            gex_Event_t *lcopt = am_lcopt[lci];                                                  \
            if ((lcopt == GEX_EVENT_GROUP) && strstr(#cat, "Reply")) break;                      \
            size_t val = gex_AM_Max##cat(myteam, r, lcopt, flags, args);                         \
            if (args) {                                                                          \
              size_t more_args = val;                                                            \
              for (int j = args-1; j >= 0; --j) {                                                \
                size_t less_args = gex_AM_Max##cat(myteam, r, lcopt, flags, j);                  \
                if (less_args < more_args) {                                                     \
                  MSG("*** ERROR - FAILED MAX ARGS MONOTONICITY TEST! "                          \
                      "args=%i rank=%i lci=%i flagsi=%i", j,(int)r,lci,flagsi);                  \
                  break;                                                                         \
                }                                                                                \
                more_args = less_args;                                                           \
              }                                                                                  \
            }                                                                                    \
            max.cat[lci][flagsi] = val;                                                          \
            assert_always(max.cat[lci][flagsi] == val); /* check overflow */                     \
            if (r < GEX_RANK_INVALID) {                                                          \
              ranklub.cat[lci][flagsi] = MIN(val,ranklub.cat[lci][flagsi]);                      \
            } else if (val != ranklub.cat[lci][flagsi]) {                                        \
              MSG("*** ERROR - FAILED ALL-RANK LUB TEST! args=%i lci=%i flagsi=%i",              \
                  args,lci,flagsi);                                                              \
            }                                                                                    \
            if (flags & (GEX_FLAG_AM_PREPARE_LEAST_CLIENT |                                      \
                         GEX_FLAG_AM_PREPARE_LEAST_ALLOC)) break; /* exclude from LUB */         \
            lub.cat[0][0] = MIN(val,lub.cat[0][0]);                                              \
            size_t lubval = gex_AM_LUB##cat();                                                   \
            if (val < lubval)                                                                    \
              MSG("*** ERROR - FAILED LUB/MAX TEST! args=%i rank=%i lci=%i flagsi=%i",           \
                  args,(int)r,lci,flagsi);                                                       \
          } while (0)
          GET_MAX(RequestMedium);
          GET_MAX(ReplyMedium);
          GET_MAX(RequestLong);
          GET_MAX(ReplyLong);
        } // flags
      } // lc
      if (r == GEX_RANK_INVALID) break;
      else {
        gasnett_atomic_increment(&sizecheck_ack,0);
        gex_AM_RequestMedium1(myteam, r, sizecheck_handlers[0].gex_index, &max, sizeof(max), GEX_EVENT_NOW, 0, args);
      }
    } // rank
  } // args
  #define CHECK_LUB(cat) do {                \
    size_t lubval = gex_AM_LUB##cat();       \
    if (lub.cat[0][0] != lubval) {           \
      MSG("*** ERROR - FAILED LUB TEST!");   \
    }                                        \
  } while (0)
  CHECK_LUB(RequestMedium);
  CHECK_LUB(ReplyMedium);
  CHECK_LUB(RequestLong);
  CHECK_LUB(ReplyLong);
  #undef CHECK_LUB
  #undef GET_MAX
  GASNET_BLOCKUNTIL(gasnett_atomic_read(&sizecheck_ack,0) == 0);
  BARRIER();

  /* Event tests */
  gex_Event_t invalid = GEX_EVENT_INVALID;
  gex_Event_t noop = GEX_EVENT_NO_OP;
  assert_always(invalid == 0);
  assert_always(noop != invalid);
  gex_Event_t lc = noop;
  size_t sz = MIN(8192,TEST_SEGSZ/2);
  //gex_Event_t rc = gex_RMA_PutNB(myteam, partner, sz, 0, sz, &lc, GEX_FLAG_SELF_SEG_OFFSET | GEX_FLAG_PEER_SEG_OFFSET); // TODO-EX
  gex_Event_t rc = gex_RMA_PutNB(myteam, partner, (char *)partnerseg + sz, TEST_MYSEG(), sz, &lc, GEX_FLAG_SELF_SEG_BOUND | GEX_FLAG_PEER_SEG_BOUND);
  assert_always(rc != noop);
  assert_always(lc != noop);
  if (rc) {
    gex_Event_t qlc = gex_Event_QueryLeaf(rc, GEX_EC_LC);
    if (lc && qlc) assert_always(lc == qlc);
    gex_Event_Wait(lc);
    assert_always(!gex_Event_Test(lc));
    assert_always(!gex_Event_TestSome(&lc,1,0));
    assert_always(!gex_Event_TestAll(&lc,1,0));
    assert_always(!gex_Event_Test(qlc));
    assert_always(!gex_Event_TestSome(&qlc,1,0));
    assert_always(!gex_Event_TestAll(&qlc,1,0));
    gex_Event_t qlc2 = gex_Event_QueryLeaf(rc, GEX_EC_LC);
    if (lc && qlc2) assert_always(lc == qlc2);
    assert_always(!gex_Event_Test(qlc2));
    assert_always(!gex_Event_TestSome(&qlc2,1,0));
    assert_always(!gex_Event_TestAll(&qlc2,1,0));
    gex_Event_Wait(rc);
  }

#ifndef TESTGASNET_NO_SPLIT
  doit0(partner, partnerseg);
}
void doit0(int partner, int *partnerseg) {
#endif

  /* misc type tests */
  assert_inttype(gex_Flags_t);
  // flags used in calls to initiate communication
  #define COMM_INIT_FLAGS          \
    GEX_FLAG_IMMEDIATE,            \
                                   \
    GEX_FLAG_SELF_SEG_UNKNOWN,     \
    GEX_FLAG_SELF_SEG_SOME,        \
    GEX_FLAG_SELF_SEG_BOUND,       \
    GEX_FLAG_SELF_SEG_OFFSET,      \
    GEX_FLAG_PEER_SEG_UNKNOWN,     \
    GEX_FLAG_PEER_SEG_SOME,        \
    GEX_FLAG_PEER_SEG_BOUND,       \
    GEX_FLAG_PEER_SEG_OFFSET,      \
    /*GEX_FLAG_LC_COPY_YES, */     \
    /*GEX_FLAG_LC_COPY_NO,  */

  static gex_Flags_t const flags_arr[] = { // ensure all the flags exist
    COMM_INIT_FLAGS

    GEX_FLAG_AM_PREPARE_LEAST_CLIENT,
    GEX_FLAG_AM_PREPARE_LEAST_ALLOC,

    GEX_FLAG_AD_MY_RANK,
    GEX_FLAG_AD_MY_NBRHD,

    GEX_FLAG_AD_ACQ,
    GEX_FLAG_AD_REL,

    GEX_FLAG_AD_FAVOR_MY_RANK,
    GEX_FLAG_AD_FAVOR_MY_NBRHD,
    GEX_FLAG_AD_FAVOR_REMOTE,

    GEX_FLAG_AM_SHORT,
    GEX_FLAG_AM_MEDIUM,
    GEX_FLAG_AM_LONG,
    GEX_FLAG_AM_MEDLONG,
    GEX_FLAG_AM_REQUEST,
    GEX_FLAG_AM_REPLY,
    GEX_FLAG_AM_REQREP,

    GEX_FLAG_ENABLE_LEAF_LC,

    GEX_FLAG_TM_SCRATCH_SIZE_MIN,
    GEX_FLAG_TM_SCRATCH_SIZE_RECOMMENDED,

    GEX_FLAG_RANK_IS_JOBRANK,
  };
  assert_arr_nonzero(gex_Flags_t, flags_arr); // No zero values

  // Ensure lack of aliasing within groups of flags potentially passed togther
  static gex_Flags_t const flags_rma[] = { // gex_RMA_* initiation
    COMM_INIT_FLAGS
  };
  assert_arr_unaliased(gex_Flags_t, flags_rma);
  static gex_Flags_t const flags_ammax[] = { // gex_AM_Max* prepare-specific
    GEX_FLAG_AM_PREPARE_LEAST_CLIENT,
    GEX_FLAG_AM_PREPARE_LEAST_ALLOC,
  };
  assert_arr_unaliased(gex_Flags_t, flags_ammax);
  static gex_Flags_t const flags_adc[] = { // gex_AD_Create
    GEX_FLAG_AD_FAVOR_MY_RANK,
    GEX_FLAG_AD_FAVOR_MY_NBRHD,
    GEX_FLAG_AD_FAVOR_REMOTE,
  };
  assert_arr_unaliased(gex_Flags_t, flags_adc);
  static gex_Flags_t const flags_ad[] = { // gex_AD_Op* initiation
    COMM_INIT_FLAGS

    GEX_FLAG_AD_MY_RANK,
    GEX_FLAG_AD_MY_NBRHD,

    GEX_FLAG_AD_ACQ,
    GEX_FLAG_AD_REL,

    GEX_FLAG_RANK_IS_JOBRANK,
  };
  assert_arr_unaliased(gex_Flags_t, flags_ad);
  static gex_Flags_t const flags_amreg[] = { // gex_EP_RegisterHandlers
    GEX_FLAG_AM_SHORT,
    GEX_FLAG_AM_MEDIUM,
    GEX_FLAG_AM_LONG,
    // GEX_FLAG_AM_MEDLONG is an intentional alias
    GEX_FLAG_AM_REQUEST,
    GEX_FLAG_AM_REPLY,
    // GEX_FLAG_AM_REQREP is an intentional alias
  };
  assert_arr_unaliased(gex_Flags_t, flags_amreg);
  static gex_Flags_t const flags_vis[] = { // gex_VIS_* initiation
    COMM_INIT_FLAGS

    GEX_FLAG_ENABLE_LEAF_LC,
  };
  assert_arr_unaliased(gex_Flags_t, flags_vis);
  static gex_Flags_t const flags_tm[] = { // gex_TM_Split
    GEX_FLAG_TM_SCRATCH_SIZE_MIN,
    GEX_FLAG_TM_SCRATCH_SIZE_RECOMMENDED,
  };
  assert_arr_unaliased(gex_Flags_t, flags_tm);

  assert_inttype(gex_EC_t);
  static gex_EC_t const ec_all = GEX_EC_ALL;
  static gex_EC_t const ec_arr[] = { // all the flags but _ALL
     GEX_EC_GET, GEX_EC_PUT, GEX_EC_AM, GEX_EC_LC, GEX_EC_RMW
  };
  assert_arr_nonzero(gex_EC_t, ec_arr); // No zero values
  assert_arr_all_val(gex_EC_t, ec_arr, ec_all); // ALL includes them all

  assert_inttype(gex_TI_t);
  static gex_TI_t const ti_all = GEX_TI_ALL;
  static gex_TI_t const ti_arr[] = { // all flags but _ALL
          GEX_TI_SRCRANK, GEX_TI_EP, GEX_TI_ENTRY, GEX_TI_IS_REQ, GEX_TI_IS_LONG
      };
  // TI constants should not alias, because they are used to indicate
  // field validity, and thus cannot be safely conflated in general
  // in particular, each flag needs at least one unique bit
  assert_arr_unaliased(gex_TI_t, ti_arr);
  assert_arr_all_val(gex_TI_t, ti_arr, ti_all); // ALL includes them all
  test_format(gex_TI_t, ti_arr, gasnett_format_ti);

  gex_RMA_Value_t val = 0; test_mark_used(val);
  test_static_assert(sizeof(gex_RMA_Value_t) == SIZEOF_GEX_RMA_VALUE_T);
  test_static_assert(sizeof(gex_RMA_Value_t) >= sizeof(void *));
  test_static_assert(sizeof(gex_RMA_Value_t) >= sizeof(long));
  assert_unsigned(gex_RMA_Value_t);

  gex_AM_Index_t ind = 0; test_mark_used(ind);
  assert_unsigned(gex_AM_Index_t);

  gex_AM_Arg_t arg = 0; test_mark_used(arg);
  test_static_assert(sizeof(gex_AM_Arg_t) >= 4);
  assert_signed(gex_AM_Arg_t);
  
  gex_AM_SrcDesc_t sd = 0; test_mark_used(sd);
  CHECK_ZERO_CONSTANT(gex_AM_SrcDesc_t, GEX_AM_SRCDESC_NO_OP);

  assert_inttype(gex_DT_t);
  static gex_DT_t const datatypes_arr[] = { // ensure all the specified values exist
    GEX_DT_I32, GEX_DT_U32,
    GEX_DT_I64, GEX_DT_U64,
    GEX_DT_FLT, GEX_DT_DBL,
    GEX_DT_USER
  };
  assert_arr_unaliased(gex_DT_t, datatypes_arr); // verify alias-free
  test_format(gex_DT_t, datatypes_arr, gasnett_format_dt);

  assert_inttype(gex_OP_t);
  static gex_OP_t const ops_arr[] = { // ensure all the specfied values exist
    GEX_OP_AND,  GEX_OP_OR,   GEX_OP_XOR,
    GEX_OP_ADD,  GEX_OP_SUB,  GEX_OP_MULT,
    GEX_OP_MIN,  GEX_OP_MAX,
    GEX_OP_INC,  GEX_OP_DEC,
    GEX_OP_SET,  GEX_OP_CAS,

    GEX_OP_FAND, GEX_OP_FOR,  GEX_OP_FXOR,
    GEX_OP_FADD, GEX_OP_FSUB, GEX_OP_FMULT,
    GEX_OP_FMIN, GEX_OP_FMAX,
    GEX_OP_FINC, GEX_OP_FDEC,
    GEX_OP_SWAP, GEX_OP_FCAS,

    GEX_OP_GET,
    GEX_OP_USER, GEX_OP_USER_NC
  };
  assert_arr_unaliased(gex_OP_t, ops_arr); // verify alias-free
  test_format(gex_OP_t, ops_arr, gasnett_format_op);
  for (size_t i = 0; ; i++) {
    gex_OP_t nfop = ops_arr[i];
    gex_OP_t fop =  ops_arr[i+12];
    assert_always(GEX_OP_TO_FETCHING(nfop) == fop);
    assert_always(GEX_OP_TO_NONFETCHING(fop) == nfop);
    if (nfop == GEX_OP_SET) break;
  }

  #define typeissigned   <
  #define typeisunsigned >
  #define assert_field_int(structtype, fieldtype, fieldname, signedop)  do { \
    static volatile structtype S;                                            \
    assert_inttype(fieldtype);                                               \
    assert_always(sizeof(S.fieldname) == sizeof(fieldtype));                 \
    assert_always((fieldtype)(S.fieldname-1) signedop (fieldtype)0);         \
  } while (0)

  #define assert_field_int_unspec(structtype, fieldname)  do { \
    static volatile structtype S;                                            \
    S.fieldname = (signed char)0x55;/* warnings here mean non-compliance */  \
    assert_always(S.fieldname > 1); /* warnings here mean non-compliance */  \
    uint64_t val; compute_uint_val(val,S.fieldname);                         \
    assert_always(val == 0x55);                                              \
  } while (0)

  #define assert_field_pointer(structtype, fieldtype, fieldname)  do {       \
    static structtype S;                                                     \
    static fieldtype v;                                                      \
    S.fieldname = v; /* warnings here mean non-compliance */                 \
    v = S.fieldname; /* warnings here mean non-compliance */                 \
    static fieldtype *p;                                                     \
    p = &(S.fieldname); /* warnings here mean non-compliance */              \
    S.fieldname = *p;   /* warnings here mean non-compliance */              \
    assert_always(sizeof(S.fieldname) == sizeof(fieldtype));                 \
  } while (0)

  #define assert_field_object(structtype, fieldtype, fieldname)  do {        \
    static structtype S;                                                     \
    static fieldtype *p;                                                     \
    S.fieldname = (fieldtype)0; /* warnings here mean non-compliance */      \
    p = &(S.fieldname); /* warnings here mean non-compliance */              \
    S.fieldname = *p;   /* warnings here mean non-compliance */              \
    assert_always(sizeof(S.fieldname) == sizeof(fieldtype));                 \
  } while (0)

  #define assert_field_constint(structtype, fieldtype, fieldname, signedop)  do { \
    static volatile union {                                                  \
      char _x;                                                               \
      structtype S;                                                          \
    } U = { 0 };                                                             \
    assert_inttype(fieldtype);                                               \
    assert_always(sizeof(U.S.fieldname) == sizeof(fieldtype));               \
    assert_always((fieldtype)(U.S.fieldname-1) signedop (fieldtype)0);       \
  } while (0)


  assert_field_int(gex_AM_Entry_t,     gex_AM_Index_t, gex_index, typeisunsigned);
  assert_field_int(gex_AM_Entry_t,     gex_Flags_t,    gex_flags, typeisunsigned);
  assert_field_int(gex_AM_Entry_t,     unsigned int,   gex_nargs, typeisunsigned);
  assert_field_pointer(gex_AM_Entry_t, gex_AM_Fn_t,    gex_fnptr);
  assert_field_pointer(gex_AM_Entry_t, const void *,   gex_cdata);
  assert_field_pointer(gex_AM_Entry_t, const char *,   gex_name);

  assert_field_int(gex_Token_Info_t,     gex_Rank_t,             gex_srcrank, typeisunsigned);
  assert_field_object(gex_Token_Info_t, gex_EP_t, gex_ep);
  assert_field_pointer(gex_Token_Info_t, const gex_AM_Entry_t *, gex_entry);
  assert_field_int_unspec(gex_Token_Info_t, gex_is_req);
  assert_field_int_unspec(gex_Token_Info_t, gex_is_long);

  assert_field_constint(gex_RankInfo_t, gex_Rank_t, gex_jobrank, typeisunsigned);

  MSG("*** passed object test!!");

#ifndef TESTGASNET_NO_SPLIT
  doit1(partner, partnerseg);
}
void doit1(int partner, int *partnerseg) {
#endif

  BARRIER();
  /*  blocking test */
  { int val1=0, val2=0;
    val1 = myrank + 100;

    gex_RMA_PutBlocking(myteam, partner, partnerseg, &val1, sizeof(int), 0);
    gex_RMA_GetBlocking(myteam, &val2, partner, partnerseg, sizeof(int), 0);

    if (val2 == (int)(myrank + 100)) MSG("*** passed blocking test!!");
    else MSG("*** ERROR - FAILED BLOCKING TEST!!!!!");
  }

  BARRIER();
  /*  blocking list test */
  #define iters 100
  { GASNET_BEGIN_FUNCTION();
    gex_Event_t events[iters];
    int val1;
    int vals[iters];
    int success = 1;
    int i;
    for (i = 0; i < iters; i++) {
      val1 = 100 + i + myrank;
      events[i] = gex_RMA_PutNB(myteam, partner, partnerseg+i, &val1, sizeof(int), GEX_EVENT_NOW, 0);
    }
    gex_Event_WaitAll(events, iters, 0);
    for (i = 0; i < iters; i++) {
      events[i] = gex_RMA_GetNB(myteam, &vals[i], partner, partnerseg+i, sizeof(int), 0);
    }
    gex_Event_WaitAll(events, iters, 0);
    for (i=0; i < iters; i++) {
      if (vals[i] != 100 + (int)myrank + i) {
        MSG("*** ERROR - FAILED NB LIST TEST!!! vals[%i] = %i, expected %i",
            i, vals[i], 100 + myrank + i);
        success = 0;
      }
    }
    if (success) MSG("*** passed blocking list test!!");
  }

#ifndef TESTGASNET_NO_SPLIT
  doit2(partner, partnerseg);
}
void doit2(int partner, int *partnerseg) {
#endif

  BARRIER();
  { /*  implicit test */
    GASNET_BEGIN_FUNCTION();
    int vals[100];
    int i, success=1;
    for (i=0; i < 100; i++) {
      int tmp = myrank + i;
      gex_RMA_PutNBI(myteam, partner, partnerseg+i, &tmp, sizeof(int), GEX_EVENT_NOW, 0);
    }
    gex_NBI_Wait(GEX_EC_PUT,0);
    for (i=0; i < 100; i++) {
      gex_RMA_GetNBI(myteam, &vals[i], partner, partnerseg+i, sizeof(int), 0);
    }
    gex_NBI_Wait(GEX_EC_GET,0);
    for (i=0; i < 100; i++) {
      if (vals[i] != (int)myrank + i) {
        MSG("*** ERROR - FAILED NBI TEST!!! vals[%i] = %i, expected %i",
            i, vals[i], myrank + i);
        success = 0;
      }
    }
    if (success) MSG("*** passed nbi test!!");
  }

#ifndef TESTGASNET_NO_SPLIT
  doit3(partner, partnerseg);
}
void doit3(int partner, int *partnerseg) {
#endif

  BARRIER();

  { /*  value test */
    GASNET_BEGIN_FUNCTION();
    int i, success=1;
    unsigned char *partnerbase2 = (unsigned char *)(partnerseg+300);
    for (i=0; i < 100; i++) {
      gex_RMA_PutBlockingVal(myteam, partner, partnerseg+i, 1000 + myrank + i, sizeof(int), 0);
    }
    for (i=0; i < 100; i++) {
      gex_Event_Wait(gex_RMA_PutNBVal(myteam, partner, partnerseg+i+100, 1000 + myrank + i, sizeof(int), 0));
    }
    for (i=0; i < 100; i++) {
      gex_RMA_PutNBIVal(myteam, partner, partnerseg+i+200, 1000 + myrank + i, sizeof(int), 0);
    }
    gex_NBI_Wait(GEX_EC_PUT,0);

    for (i=0; i < 100; i++) {
      int tmp1 = gex_RMA_GetBlockingVal(myteam, partner, partnerseg+i, sizeof(int), 0);
      int tmp2 = gex_RMA_GetBlockingVal(myteam, partner, partnerseg+i+200, sizeof(int), 0);
      if (tmp1 != 1000 + (int)myrank + i || tmp2 != 1000 + (int)myrank + i) {
        MSG("*** ERROR - FAILED INT VALUE TEST 1!!!");
        printf("node %i/%i  i=%i tmp1=%i tmp2=%i (1000 + myrank + i)=%i\n", 
          (int)myrank, (int)numranks, 
          i, tmp1, tmp2, 1000 + myrank + i); fflush(stdout); 
        success = 0;
      }
    }

    for (i=0; i < 100; i++) {
      gex_RMA_PutBlockingVal(myteam, partner, partnerbase2+i, 100 + myrank + i, sizeof(unsigned char), 0);
    }
    for (i=0; i < 100; i++) {
      gex_Event_Wait(gex_RMA_PutNBVal(myteam, partner, partnerbase2+i+100, 100 + myrank + i, sizeof(unsigned char), 0));
    }
    for (i=0; i < 100; i++) {
      gex_RMA_PutNBIVal(myteam, partner, partnerbase2+i+200, 100 + myrank + i, sizeof(unsigned char), 0);
    }
    gex_NBI_Wait(GEX_EC_PUT,0);

    for (i=0; i < 100; i++) {
      unsigned int tmp1 = (unsigned int)gex_RMA_GetBlockingVal(myteam, partner, partnerbase2+i, sizeof(unsigned char), 0);
      unsigned int tmp2 = (unsigned int)gex_RMA_GetBlockingVal(myteam, partner, partnerbase2+i+200, sizeof(unsigned char), 0);
      if (tmp1 != (unsigned char)(100 + myrank + i) || 
          tmp2 != (unsigned char)(100 + myrank + i)) {
        MSG("*** ERROR - FAILED CHAR VALUE TEST 1!!!");
        printf("node %i/%i  i=%i tmp1=%i tmp2=%i (100 + myrank + i)=%i\n", 
          (int)myrank, (int)numranks, 
          i, tmp1, tmp2, 100 + myrank + i); fflush(stdout); 
        success = 0;
      }
    }

    if (success) MSG("*** passed value test!!");
  }

#ifndef TESTGASNET_NO_SPLIT
  doit5(partner, partnerseg);
}
void doit5(int partner, int *partnerseg) {
#endif

  BARRIER();

  /* NB and NBI put/overwrite/get tests */
  #define MAXVALS (1024)
  #define MAXSZ (MAXVALS*8)
  #define INSEGCHUNKS 3
  #define NUMCHUNKS 6
  #define SEGSZ (MAXSZ*NUMCHUNKS)
  #define VAL(sz, chunkid, iter) \
    (((uint64_t)(sz) << 36) | ((uint64_t)(chunkid) << 32) | ((uint64_t)(100 + myrank) << 16) | ((iter) & 0xFF))
  assert(TEST_SEGSZ >= 2*SEGSZ);
  { GASNET_BEGIN_FUNCTION();
    uint64_t *localpos=(uint64_t *)test_malloc(SEGSZ);
    int success = 1;
    int i, sz;
    for (i = 0; i < MAX(1,iters/10); i++) {
      uint64_t *segpos=(uint64_t *)TEST_MYSEG();
      uint64_t *rsegpos=(uint64_t *)((char*)partnerseg+SEGSZ);
      for (sz = 1; sz <= MAXSZ; sz*=2) {
        gex_Event_t event;
        gex_Event_t lcevt;
        int elems = sz/8;
        uint64_t val[NUMCHUNKS];
        for (int chunk=0; chunk < NUMCHUNKS; chunk++) {
          val[chunk] = VAL(sz, chunk, i); /* setup known src value */
          if (sz < 8) {
            elems = 1;
            memset(localpos+chunk*elems, (val[chunk] & 0xFF), sz);
            memset(segpos+chunk*elems, (val[chunk] & 0xFF), sz);
            memset(&val[chunk], (val[chunk] & 0xFF), sz);
          } else {
            for (int j=0; j < elems; j++) {
              (localpos+chunk*elems)[j] = val[chunk];
              (segpos+chunk*elems)[j] = val[chunk];
            }
          }
        }
        event = gex_RMA_PutNB(myteam, partner, rsegpos, localpos, sz, GEX_EVENT_DEFER, 0);
        gex_Event_Wait(event);
        memset(localpos, 0xAA, sz); /* clear */

        event = gex_RMA_PutNB(myteam, partner, rsegpos+elems, localpos+elems, sz, GEX_EVENT_NOW, 0);
        memset(localpos+elems, 0xBB, sz); /* clear */
        gex_Event_Wait(event);

        lcevt = GEX_EVENT_INVALID;
        event = gex_RMA_PutNB(myteam, partner, rsegpos+2*elems, localpos+2*elems, sz, &lcevt, 0);
        gex_Event_Wait(lcevt);
        memset(localpos+2*elems, 0xCC, sz); /* clear */
        gex_Event_Wait(event);

        event = gex_RMA_PutNB(myteam, partner, rsegpos+3*elems, segpos+3*elems, sz, GEX_EVENT_DEFER, 0);
        gex_Event_Wait(event);
        memset(segpos+3*elems, 0xDD, sz); /* clear */

        event = gex_RMA_PutNB(myteam, partner, rsegpos+4*elems, segpos+4*elems, sz, GEX_EVENT_NOW, 0);
        memset(segpos+4*elems, 0xEE, sz); /* clear */
        gex_Event_Wait(event);

        lcevt = GEX_EVENT_INVALID;
        event = gex_RMA_PutNB(myteam, partner, rsegpos+5*elems, segpos+5*elems, sz, &lcevt, 0);
        gex_Event_Wait(lcevt);
        memset(segpos+5*elems, 0xFF, sz); /* clear */
        gex_Event_Wait(event);

        for (int chunk=0; chunk < NUMCHUNKS; chunk++) {
          gex_RMA_GetBlocking(myteam, localpos, partner, rsegpos+chunk*elems, sz, 0);

          for (int j=0; j < elems; j++) {
            int ok = (localpos[j] == val[chunk]);
            if (sz < 8) ok = !memcmp(&(localpos[j]), &val[chunk], sz);
            if (!ok) {
              MSG("*** ERROR - FAILED %s-SEG PUT_NB/OVERWRITE TEST!!! sz=%i j=%i (got=%016" PRIx64 " expected=%016" PRIx64 ")",
                  (chunk < INSEGCHUNKS ? "IN" : "OUT-OF"), sz, j, localpos[j], val[chunk]);
              success = 0;
            }
          }
        }
      }
    }
    test_free(localpos);
    if (success) MSG("*** passed nb put/overwrite test!!");
  }
  { GASNET_BEGIN_FUNCTION();
    uint64_t *localpos=(uint64_t *)test_malloc(SEGSZ);
    int success = 1;
    int i, sz;
    for (i = 0; i < MAX(1,iters/10); i++) {
      uint64_t *segpos=(uint64_t *)TEST_MYSEG();
      uint64_t *rsegpos=(uint64_t *)((char*)partnerseg+SEGSZ);
      for (sz = 1; sz <= MAXSZ; sz*=2) {
        int elems = sz/8;
        uint64_t val[NUMCHUNKS];
        for (int chunk=0; chunk < NUMCHUNKS; chunk++) {
          val[chunk] = VAL(sz, chunk, i+91); /* setup known src value, different from NB test */
          if (sz < 8) {
            elems = 1;
            memset(localpos+chunk*elems, (val[chunk] & 0xFF), sz);
            memset(segpos+chunk*elems, (val[chunk] & 0xFF), sz);
            memset(&val[chunk], (val[chunk] & 0xFF), sz);
          } else {
            for (int j=0; j < elems; j++) {
              (localpos+chunk*elems)[j] = val[chunk];
              (segpos+chunk*elems)[j] = val[chunk];
            }
          }
        }
        gex_RMA_PutNBI(myteam, partner, rsegpos, localpos, sz, GEX_EVENT_DEFER, 0);
        gex_NBI_Wait(GEX_EC_PUT,0);
        memset(localpos, 0xAA, sz); /* clear */

        gex_RMA_PutNBI(myteam, partner, rsegpos+elems, localpos+elems, sz, GEX_EVENT_NOW, 0);
        memset(localpos+elems, 0xBB, sz); /* clear */

        gex_RMA_PutNBI(myteam, partner, rsegpos+2*elems, localpos+2*elems, sz, GEX_EVENT_GROUP, 0);
        gex_NBI_Wait(GEX_EC_LC, 0);
        memset(localpos+2*elems, 0xCC, sz); /* clear */

        gex_RMA_PutNBI(myteam, partner, rsegpos+3*elems, segpos+3*elems, sz, GEX_EVENT_DEFER, 0);
        gex_NBI_Wait(GEX_EC_PUT,0);
        memset(segpos+3*elems, 0xDD, sz); /* clear */

        gex_RMA_PutNBI(myteam, partner, rsegpos+4*elems, segpos+4*elems, sz, GEX_EVENT_NOW, 0);
        memset(segpos+4*elems, 0xEE, sz); /* clear */

        gex_RMA_PutNBI(myteam, partner, rsegpos+5*elems, segpos+5*elems, sz, GEX_EVENT_GROUP, 0);
        gex_NBI_Wait(GEX_EC_LC, 0);
        memset(segpos+5*elems, 0xFF, sz); /* clear */
        gex_NBI_Wait(GEX_EC_PUT,0);

        for (int chunk=0; chunk < NUMCHUNKS; chunk++) {
          gex_RMA_GetBlocking(myteam, localpos, partner, rsegpos+chunk*elems, sz, 0);

          for (int j=0; j < elems; j++) {
            int ok = (localpos[j] == val[chunk]);
            if (sz < 8) ok = !memcmp(&(localpos[j]), &val[chunk], sz);
            if (!ok) {
              MSG("*** ERROR - FAILED %s-SEG PUT_NBI/OVERWRITE TEST!!! sz=%i j=%i (got=%016" PRIx64 " expected=%016" PRIx64 ")",
                  (chunk < INSEGCHUNKS ? "IN" : "OUT-OF"), sz, j, localpos[j], val[chunk]);
              success = 0;
            }
          }
        }
      }
    }
    test_free(localpos);
    if (success) MSG("*** passed nbi put/overwrite test!!");
  }

#ifndef TESTGASNET_NO_SPLIT
  doit6(partner, partnerseg);
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
  doit7(partner, partnerseg);
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
    gasnett_atomic_increment((gasnett_atomic_t*)&stmp,0);
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
