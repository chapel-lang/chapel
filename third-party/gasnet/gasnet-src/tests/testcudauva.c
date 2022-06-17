/* $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testcudauva.c $
 * Copyright (c) 2021, The Regents of the University of California
 *
 * Description: test of GEX_MK_CLASS_CUDA_UVA
 *
 * This test verifies correctness of gex_MK_Create() for the device class
 * GEX_MK_CLASS_CUDA_UVA.  This includes checking the expected behavior of
 * builds both with and without configure-time enable of support for this
 * device class.
 */

// WARNING: This test exercises one or more EXPERIMENTAL features.
// One should not clone the logic in this test, since details of such features
// may change without notice.

#include <gasnetex.h>
#include <gasnet_mk.h>
#include <gasnet_coll.h>

#if GASNET_HAVE_MK_CLASS_CUDA_UVA
#include <cuda.h>
#endif

static size_t len = 0;
#ifndef TEST_SEGSZ
#define TEST_SEGSZ_EXPR (2*len)
#endif

#include <test.h>

#define check_cudacall(op) do {                 \
  int _retval = (op);                           \
  if_pf(_retval) {                              \
    const char *_errorname;                     \
    cuGetErrorName(_retval, &_errorname);       \
    FATALERR(#op": %s(%i)",_errorname,_retval); \
  }                                             \
} while (0)

static gex_Client_t  myclient;
static gex_EP_t      myep;
static gex_TM_t      myteam;
static gex_Segment_t mysegment;
static gex_Rank_t    myrank;
static gex_Rank_t    nranks;

static uint8_t *cmp_buffer;

#if GASNET_HAVE_MK_CLASS_CUDA_UVA
static int equalDH(uint8_t *d_ptr, uint8_t *h_ptr, size_t len) {
  cuMemcpyDtoH(cmp_buffer, (CUdeviceptr)d_ptr, len);
  int result = !memcmp(cmp_buffer, h_ptr, len);
  if (!result) {
    // Restore expected content to avoid cascading failures
    cuMemcpyHtoD((CUdeviceptr)d_ptr, h_ptr, len);
  }
  return result;
}
#endif

#define CHECK_DEVICE(label,d_ptr,h_ptr,len) \
  do {                                            \
    if (! equalDH(d_ptr,h_ptr,len)) {             \
       ERR(label " verification failed"); \
    } else {                                      \
       MSG(label " verification passed"); \
    }                                             \
  } while (0)

#define CHECK_HOST(label,h_ptr1,h_ptr2,len) \
  do {                                            \
    if (memcmp(h_ptr1,h_ptr2,len)) {              \
       ERR(label " verification failed"); \
    } else {                                      \
       MSG(label " verification passed"); \
    }                                             \
  } while (0)

int main(int argc, char **argv)
{
  int client_segment = 1;
  int seed = 0;
  int rc;

  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testcudauva", &argc, &argv, 0));

  test_init("testcudauva", 0, "[options] (size) (seed)\n"
               " Segment allocation options:\n"
               "     -client-seg:  Test client-allocated GPU segment (default)\n"
               "     -gasnet-seg:  Test GASNet-allocated GPU segment\n"
               " size      length of segment\n"
               " seed      seed for PRNG\n");

  int help = 0;
  int argi = 1;
  while (argc > argi) {
    if (!strcmp(argv[argi], "-client-seg")) {
      client_segment = 1;
      ++argi;
    } else if (!strcmp(argv[argi], "-gasnet-seg")) {
      client_segment = 0;
      ++argi;
    } else if (argv[argi][0] == '-') {
      help = 1;
      ++argi;
    } else break;
  }

  if (argi < argc) { len = atol(argv[argi]); ++argi; }
  if (len == 0) { len = 8*1024*1024; }

  if (argi < argc) { seed = atoi(argv[argi]); ++argi; }
  if (seed == 0) { seed = (((unsigned int)TIME()) & 0xFFFF); }

  if (argi < argc || help) test_usage();

  cmp_buffer = test_malloc(len);
  uint8_t *loc1 = test_malloc(len);
  uint8_t *loc2 = test_malloc(len);
  uint8_t *array1 = test_malloc(len);
  uint8_t *array2 = test_malloc(len);

  myrank = gex_TM_QueryRank(myteam);
  nranks = gex_TM_QuerySize(myteam);
  gex_Rank_t peer = (myrank + 1) % nranks;

  if (nranks == 1) {
    // TODO: remove once loopback kinds works correctly
    MSG0("WARNING: This test requires a minimum of two nodes. Test skipped.\n");
    gasnet_exit(0); // prevents false negatives, such as from test harnesses for smp-conduit
  }

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));

  MSG0("Running CUDA UVA non-local xfer tests with size %lu, PRNG seed %d, and %s-allocated GPU segment",
       (unsigned long)len, seed, client_segment ? "client" : "GASNet");

  const char *cvd = getenv("CUDA_VISIBLE_DEVICES"); // Intentionally NOT gasnet_getenv()
  if (cvd) {
    MSG("CUDA_VISIBLE_DEVICES='%s'", cvd);
  } else {
    MSG("CUDA_VISIBLE_DEVICES is unset");
  }
  const char *nvd = getenv("NVIDIA_VISIBLE_DEVICES"); // Intentionally NOT gasnet_getenv()
  if (nvd) {
    MSG("NVIDIA_VISIBLE_DEVICES='%s'", nvd);
  } else {
    MSG("NVIDIA_VISIBLE_DEVICES is unset");
  }

  TEST_BCAST(&seed, 0, &seed, sizeof(seed));
  TEST_SRAND(seed);
  for (size_t i = 0; i < len; ++i) {
     unsigned int r = TEST_RAND(0,65535);
     array1[i] = r & 0xff;
     array2[i] = (r >> 8) & 0xff;
  }

  BARRIER();

  gex_EP_t gpu1_ep, gpu2_ep;
  gex_MK_t kind;
  gex_MK_Create_args_t args;

  args.gex_flags = 0;
  args.gex_class = GEX_MK_CLASS_CUDA_UVA;
  args.gex_args.gex_class_cuda_uva.gex_CUdevice = 0;

#if GASNET_HAVE_MK_CLASS_CUDA_UVA
  {
    if (GASNET_HAVE_MK_CLASS_MULTIPLE != 1) {
       ERR("Invalid GASNET_HAVE_MK_CLASS_MULTIPLE");
    }
    test_static_assert(GASNET_MAXEPS >= 2);

    int count;
    cuInit(0);
    if (cuDeviceGetCount(&count) || !count) {
      MSG("GEX_MK_CLASS_CUDA_UVA: skipped - could not find a CUDA device");
      // If this lack of a device is NOT a collective property, then we want
      // to at least balance the collective operations (to avoid hanging).
      // However, at least one peer will fail a gex_EP_QueryBoundSegmentNB().
      // For the case all ranks lack a GPU, this test *will* exit gracefully.
      GASNET_Safe( gex_EP_PublishBoundSegment(myteam, NULL, 0, 0) );
      GASNET_Safe( gex_EP_PublishBoundSegment(myteam, NULL, 0, 0) );
      for (int i = 0; i < 4; ++i) BARRIER(); // currently exactly one per case
    } else {
      MSG("cuDeviceGetCount reports %d devices", count);
      CUcontext ctx;
      check_cudacall( cuDevicePrimaryCtxRetain(&ctx, 0) );
      check_cudacall( cuCtxPushCurrent(ctx) );
      CUdevice dev;
      check_cudacall( cuCtxGetDevice(&dev) );

      uint8_t *client_gpu1 = NULL;
      uint8_t *client_gpu2 = NULL;
      if (client_segment) {
        CUdeviceptr dptr;
        check_cudacall( cuMemAlloc(&dptr, TEST_SEGSZ_REQUEST) );
        client_gpu1 = (uint8_t *) dptr;
        check_cudacall( cuMemAlloc(&dptr, TEST_SEGSZ_REQUEST) );
        client_gpu2 = (uint8_t *) dptr;
      }

      // Create and Destroy a kind
      kind = GEX_MK_INVALID;
      GASNET_Safe( gex_MK_Create(&kind, myclient, &args, 0) );
      assert_always(kind != GEX_MK_INVALID);
      gex_MK_Destroy(kind, 0);

      // Create the Kind "for keeps"
      // TODO: if multiple devices, this should optionally create kinds using two devices
      kind = GEX_MK_INVALID;
      GASNET_Safe( gex_MK_Create(&kind, myclient, &args, 0) );
      assert_always(kind != GEX_MK_INVALID);

      // If multiple devices are available switch to context on another
      CUcontext curr_ctx, tmp_ctx;
      if (count > 1) {
        check_cudacall( cuDevicePrimaryCtxRetain(&curr_ctx, 1) );
        check_cudacall( cuCtxPushCurrent(curr_ctx) );
      } else {
        curr_ctx = ctx;
      }

      // Create the first GPU segment
      gex_Segment_t d_segment1 = GEX_SEGMENT_INVALID;
      GASNET_Safe( gex_Segment_Create(&d_segment1, myclient, client_gpu1, TEST_SEGSZ_REQUEST, kind, 0));
      uint8_t *loc_gpu1 = gex_Segment_QueryAddr(d_segment1);
      if (client_segment) assert_always(loc_gpu1 == client_gpu1);

      // Confirm the gex_Segment_Create() did NOT change the current context
      check_cudacall( cuCtxGetCurrent(&tmp_ctx) );
      assert_always(tmp_ctx == curr_ctx);

      // If GASNet performed allocation, check that it did so on the proper context
      if (!client_segment) {
        check_cudacall( cuPointerGetAttribute(&tmp_ctx, CU_POINTER_ATTRIBUTE_CONTEXT, (CUdeviceptr)loc_gpu1) );
        assert_always(tmp_ctx == ctx);
      }

      // Create first GPU endpoint and bind its segment
      GASNET_Safe( gex_EP_Create(&gpu1_ep, myclient, GEX_EP_CAPABILITY_RMA, 0));
      gex_EP_BindSegment(gpu1_ep, d_segment1, 0);
      GASNET_Safe( gex_EP_PublishBoundSegment(myteam, &gpu1_ep, 1, 0) );

      // Repeat to create a second local GPU segment
      gex_Segment_t d_segment2 = GEX_SEGMENT_INVALID;
      GASNET_Safe( gex_Segment_Create(&d_segment2, myclient, client_gpu2, TEST_SEGSZ_REQUEST, kind, 0));
      uint8_t *loc_gpu2 = gex_Segment_QueryAddr(d_segment2);
      if (client_segment) assert_always(loc_gpu2 == client_gpu2);
      check_cudacall( cuCtxGetCurrent(&tmp_ctx) );
      assert_always(tmp_ctx == curr_ctx);
      if (!client_segment) {
        check_cudacall( cuPointerGetAttribute(&tmp_ctx, CU_POINTER_ATTRIBUTE_CONTEXT, (CUdeviceptr)loc_gpu2) );
        assert_always(tmp_ctx == ctx);
      }
      GASNET_Safe( gex_EP_Create(&gpu2_ep, myclient, GEX_EP_CAPABILITY_RMA, 0));
      gex_EP_BindSegment(gpu2_ep, d_segment2, 0);
      GASNET_Safe( gex_EP_PublishBoundSegment(myteam, &gpu2_ep, 1, 0) );

      // TM pairs for several possible pairings
      gex_EP_Index_t host_epidx = gex_EP_QueryIndex(myep);
      gex_EP_Index_t gpu1_epidx = gex_EP_QueryIndex(gpu1_ep);
      gex_EP_Index_t gpu2_epidx = gex_EP_QueryIndex(gpu2_ep);
      assert_always(host_epidx == 0);
      assert_always(gpu1_epidx == 1);
      assert_always(gpu2_epidx == 2);
      gex_TM_t LH_RG1  = gex_TM_Pair(myep,    gpu1_epidx);
      gex_TM_t LH_RG2  = gex_TM_Pair(myep,    gpu2_epidx);
      gex_TM_t LG1_RG1 = gex_TM_Pair(gpu1_ep, gpu1_epidx);
      gex_TM_t LG2_RG2 = gex_TM_Pair(gpu2_ep, gpu2_epidx);
      gex_TM_t LG1_RG2 = gex_TM_Pair(gpu1_ep, gpu2_epidx);
      gex_TM_t LG2_RG1 = gex_TM_Pair(gpu2_ep, gpu1_epidx);

      uint8_t *rem_gpu1;
      size_t queried_len;
      gex_Event_Wait( gex_EP_QueryBoundSegmentNB(LH_RG1, peer, (void**)&rem_gpu1, NULL, &queried_len, 0) );
      assert_always(queried_len == TEST_SEGSZ_REQUEST);

      uint8_t *rem_gpu2;
      gex_Event_Wait( gex_EP_QueryBoundSegmentNB(LH_RG2, peer, (void**)&rem_gpu2, NULL, &queried_len, 0) );
      assert_always(queried_len == TEST_SEGSZ_REQUEST);

// Case 1. Puts - local host to remote gpus
//   BEFORE:  GPU1=uninit:uninit    GPU2=uninit:uninit
//   AFTER:   GPU1=array1:uninit    GPU2=array2:uninit
      gex_RMA_PutNBI(LH_RG1, peer, rem_gpu1, array1, len, GEX_EVENT_DEFER, 0);
      gex_RMA_PutNBI(LH_RG2, peer, rem_gpu2, array2, len, GEX_EVENT_DEFER, 0);
      gex_NBI_Wait(GEX_EC_PUT,0);
      BARRIER();
      CHECK_DEVICE("Case 1a", loc_gpu1, array1, len);
      CHECK_DEVICE("Case 1b", loc_gpu2, array2, len);

// Case 2. Gets - remote gpus to local host
//   BEFORE:  GPU1=array1:uninit    GPU2=array2:uninit
//   AFTER:   GPU1=array1:uninit    GPU2=array2:uninit
      gex_Event_t get_events[2] = {
              gex_RMA_GetNB(LH_RG1, loc1, peer, rem_gpu1, len, 0),
              gex_RMA_GetNB(LH_RG2, loc2, peer, rem_gpu2, len, 0)
          };
      gex_Event_WaitAll(get_events, 2, 0);
      CHECK_HOST("Case 2a", loc1, array1, len);
      CHECK_HOST("Case 2b", loc2, array2, len);
      BARRIER();

// Case 3. Put - local gpus to remote gpus "cross over"
//   BEFORE:  GPU1=array1:uninit    GPU2=array2:uninit
//   AFTER:   GPU1=array1:array2    GPU2=array2:array1
      gex_RMA_PutBlocking(LG1_RG2, peer, rem_gpu2+len, loc_gpu1, len, 0);
      BARRIER();
      CHECK_DEVICE("Case 3a", loc_gpu2+len, array1, len);
      gex_RMA_PutBlocking(LG2_RG1, peer, rem_gpu1+len, loc_gpu2, len, 0);
      BARRIER();
      CHECK_DEVICE("Case 3b", loc_gpu1+len, array2, len);

// Case 4. Get - remote gpus to local gpus
//   BEFORE:  GPU1=array1:array2    GPU2=array2:array1
//   AFTER:   GPU1=array1:array1    GPU2=array2:array2
      gex_RMA_GetBlocking(LG1_RG1, loc_gpu1+len, peer, rem_gpu1, len, 0);
      CHECK_DEVICE("Case 4a", loc_gpu1+len, array1, len);
      gex_RMA_GetBlocking(LG2_RG2, loc_gpu2+len, peer, rem_gpu2, len, 0);
      CHECK_DEVICE("Case 4b", loc_gpu2+len, array2, len);

      if (!test_errs) MSG("GEX_MK_CLASS_CUDA_UVA: success");

      check_cudacall( cuCtxSetCurrent(NULL) );
      check_cudacall( cuDevicePrimaryCtxRelease(0) );
      if (count > 1) {
        check_cudacall( cuDevicePrimaryCtxRelease(1) );
      }
    }

    // TODO: once supported: Destroy Segments, Kinds and Endpoints; free GPU memory
  }
#else
  {
    gex_System_SetVerboseErrors(0);
    int rc = gex_MK_Create(&kind, myclient, &args, 0);
    assert_always(rc == GASNET_ERR_BAD_ARG);
    MSG("GEX_MK_CLASS_CUDA_UVA: correct failure due to missing support");
  }
#endif

  // Just to ensure these exist:
  args.gex_class = GEX_MK_CLASS_HOST;
  kind = GEX_MK_HOST;

  test_free(array2);
  test_free(array1);
  test_free(loc2);
  test_free(loc1);
  test_free(cmp_buffer);

  MSG("done.");

  BARRIER();
  gasnet_exit(0);

  return 0;
}
