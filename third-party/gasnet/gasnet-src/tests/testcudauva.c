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


int main(int argc, char **argv)
{
  int client_segment = 1;
  int seed = 0;
  int rc;

#if GASNET_CONDUIT_IBV
  { // These settings are a hack to avoid triggering known bugs/limitations,
    // by disabling multiple paths.  See bug 4148.
    // Both of these variables can have potentially serious negative impacts
    // on performance, and should not be used in general.
    // As noted above "One should not clone the logic in this test".
    setenv("GASNET_SUPERNODE_MAXSIZE", "1", 0 /* NO overwrite if already set */);
    setenv("GASNET_NUM_QPS", "1", 0 /* NO overwrite if already set */);
  }
#endif

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
  if (len == 0) { len = 16*1024*1024; }

  if (argi < argc) { seed = atoi(argv[argi]); ++argi; }
  if (seed == 0) { seed = (((unsigned int)TIME()) & 0xFFFF); }

  if (argi < argc || help) test_usage();

  uint8_t *tmp = test_malloc(len);
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

  TEST_BCAST(&seed, 0, &seed, sizeof(seed));
  TEST_SRAND(seed);
  for (size_t i = 0; i < len; ++i) {
     unsigned int r = TEST_RAND(0,65535);
     array1[i] = r & 0xff;
     array2[i] = (r >> 8) & 0xff;
  }

  BARRIER();

  gex_EP_t gpu_ep;
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

    int count;
    cuInit(0);
    if (cuDeviceGetCount(&count) || !count) {
      MSG("GEX_MK_CLASS_CUDA_UVA: skipped - could not find a CUDA device");
      // If this lack of a device is NOT a collective property, then we want
      // to at least balance the collective operations (to avoid hanging).
      // However, at least one peer will fail a gex_EP_QueryBoundSegmentNB().
      // For the case all ranks lack a GPU, this test *will* exit gracefully.
      GASNET_Safe( gex_EP_PublishBoundSegment(myteam, NULL, 0, 0) );
      for (int i = 0; i < 4; ++i) BARRIER(); // currently exactly one per case
    } else {
      CUcontext ctx;
      check_cudacall( cuDevicePrimaryCtxRetain(&ctx, 0) );
      check_cudacall( cuCtxPushCurrent(ctx) );

      CUdeviceptr dptr;
      uint8_t *client_gpu = NULL;
      if (client_segment) {
        check_cudacall( cuMemAlloc(&dptr, TEST_SEGSZ_REQUEST) );
        client_gpu = (uint8_t *) dptr;
      }

      GASNET_Safe( gex_MK_Create(&kind, myclient, &args, 0) );
      gex_Segment_t d_segment = GEX_SEGMENT_INVALID;
      GASNET_Safe( gex_Segment_Create(&d_segment, myclient, client_gpu, TEST_SEGSZ_REQUEST, kind, 0));
      uint8_t *loc_gpu = gex_Segment_QueryAddr(d_segment);
      if (client_segment) assert_always(loc_gpu == client_gpu);

      GASNET_Safe( gex_EP_Create(&gpu_ep, myclient, GEX_EP_CAPABILITY_RMA, 0));
      gex_EP_BindSegment(gpu_ep, d_segment, 0);
      GASNET_Safe( gex_EP_PublishBoundSegment(myteam, &gpu_ep, 1, 0) );

      // TM (3 of 4 being pairs) for the four possible pairings
      gex_EP_Index_t host_epidx = gex_EP_QueryIndex(myep);
      gex_EP_Index_t gpu_epidx  = gex_EP_QueryIndex(gpu_ep);
      assert_always(host_epidx == 0);
      assert_always(gpu_epidx  == 1);
      gex_TM_t LH_RH = myteam;
      gex_TM_t LH_RG = gex_TM_Pair(myep,   gpu_epidx);
      gex_TM_t LG_RH = gex_TM_Pair(gpu_ep, host_epidx);
      gex_TM_t LG_RG = gex_TM_Pair(gpu_ep, gpu_epidx);

      uint8_t *rem_gpu;
      size_t queried_len;
      gex_Event_Wait( gex_EP_QueryBoundSegmentNB(LH_RG, peer, (void**)&rem_gpu, NULL, &queried_len, 0) );
      assert_always(queried_len == TEST_SEGSZ_REQUEST);
      
// Case 1. Put - local host to remote gpu
      gex_RMA_PutBlocking(LH_RG, peer, rem_gpu, array1, len, 0);
      BARRIER();
      cuMemcpyDtoH(tmp, (CUdeviceptr)loc_gpu, len);
      if (memcmp(tmp, array1, len)) {
         ERR("Case 1 verification failed");
         cuMemcpyHtoD((CUdeviceptr)loc_gpu, array1, len);
      } else {
         MSG("Case 1 verification passed");
      }

// Case 2. Get - remote gpu to local host
      memset(tmp, 0, len);
      gex_RMA_GetBlocking(LH_RG, tmp, peer, rem_gpu, len, 0);
      if (memcmp(tmp, array1, len)) {
         ERR("Case 2 verification failed");
      } else {
         MSG("Case 2 verification passed");
      }
      BARRIER();

// Case 3. Put - local gpu to remote gpu
      gex_RMA_PutBlocking(LG_RG, peer, rem_gpu+len, loc_gpu, len, 0);
      BARRIER();
      cuMemcpyDtoH(tmp, (CUdeviceptr)loc_gpu+len, len);
      if (memcmp(tmp, array1, len)) {
         ERR("Case 3 verification failed");
         cuMemcpyHtoD((CUdeviceptr)loc_gpu+len, array1, len);
      } else {
         MSG("Case 3 verification passed");
      }

// Case 4. Get - remote gpu to local gpu
      cuMemcpyHtoD((CUdeviceptr)loc_gpu, array2, len);
      BARRIER();
      gex_RMA_GetBlocking(LG_RG, loc_gpu+len, peer, rem_gpu, len, 0);
      cuMemcpyDtoH(tmp, (CUdeviceptr)loc_gpu+len, len);
      if (memcmp(tmp, array2, len)) {
         ERR("Case 4 verification failed");
         cuMemcpyHtoD((CUdeviceptr)loc_gpu+len, array2, len);
      } else {
         MSG("Case 4 verification passed");
      }

      if (!test_errs) MSG("GEX_MK_CLASS_CUDA_UVA: success");

      check_cudacall( cuCtxSetCurrent(NULL) );
      check_cudacall( cuDevicePrimaryCtxRelease(0) );
    }
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

  MSG("done.");

  BARRIER();
  gasnet_exit(0);

  return 0;
}
