/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testslice.c $
 * Description: GASNet randomized get/put correctness validation test
 * Copyright 2007, Parry Husbands and Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include <gasnetex.h>
#include <gasnet_coll.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

size_t segsize = 0;
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR ((uintptr_t)segsize)
#endif
#include "test.h"

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

#define OUTPUT_SUCCESS 0
uint64_t failures = 0;
void assert_eq(char *x, char *y, int len, int start, int i, int j, const char *msg)
{
  int k;
  int error = 0;
  for(k=0;k < len;k++) {
    if(x[k] != y[k]) {
      error=1;
      break;
    }
  }
  if(error) {
    ERR("FAILURE %s outer iteration %d inner iteration %d starting point = %d length = %d",msg,i,j,start,len);
    failures++;
  } else {
#if OUTPUT_SUCCESS
    MSG("SUCCESS %s outer iteration %d inner iteration %d starting point = %d length = %d",msg,i,j,start,len);
#endif
  }
}

int main(int argc, char **argv)
{
    int outer_iterations = 0;
    int inner_iterations = 0;
    int seedoffset = 0;
    int numprocs, myproc;
    int peerproc;
    int sender_p;
    char *shadow_region_1, *shadow_region_2;
    int i,j;
    char *local_base, *target_base;

    /* call startup */
    GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testslice", &argc, &argv, 0));

    /* get SPMD info */
    myproc = gex_TM_QueryRank(myteam);
    numprocs = gex_TM_QuerySize(myteam);

    if (argc > 1) segsize = gasnett_parse_int(argv[1], 1);
    if (!segsize) segsize = 1024*1000;
    if (argc > 2) outer_iterations = atoi(argv[2]);
    if (!outer_iterations) outer_iterations = 10;
    if (argc > 3) inner_iterations = atoi(argv[3]);
    if (!inner_iterations) inner_iterations = 10;
    if (argc > 4) seedoffset = atoi(argv[4]);

    GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ));

    test_init("testslice",0, "(segsize) (iterations) (# of sizes per iteration) (seed)");

    /* parse arguments */
    if (argc > 5) test_usage();
    
    if(numprocs & 1) {
        MSG0("WARNING: This test requires an even number of nodes. Test skipped.\n");
        gasnet_exit(0); /* exit 0 to prevent false negatives in test harnesses for smp-conduit */
    }
    sender_p = !(myproc & 1);
    peerproc = (myproc + 1) % numprocs;

    if (seedoffset == 0) {
      seedoffset = (((unsigned int)TIME()) & 0xFFFF);
      TEST_BCAST(&seedoffset, 0, &seedoffset, sizeof(seedoffset));
    }
    TEST_SRAND(myproc+seedoffset);

    MSG0("Running with segment size = %"PRIuSZ" outer iterations=%d inner iterations=%d seed=%d",
         segsize,outer_iterations, inner_iterations, seedoffset);

    BARRIER();

    /* Allocate two shadow regions the same size as the segment */
    shadow_region_1 = (char *) test_malloc(segsize);
    shadow_region_2 = (char *) test_malloc(segsize);
   
    /* Fill up the shadow region with random data */
    for(size_t k=0;k < segsize / sizeof(uint32_t);k++) {
      ((uint32_t *)shadow_region_1)[k] = TEST_RAND(0, UINT32_MAX);
    }
    memset(shadow_region_2,0,segsize);

    /* Big loop performing the following */
    for(i=0;i < outer_iterations;i++) {
      if(sender_p) {
        /* Pick a starting point anywhere in the segment */
        size_t starting_point = TEST_RAND(0,(segsize-1));

        local_base = TEST_SEG(myproc);
        target_base = TEST_SEG(peerproc);
 
        for(j=0;j < inner_iterations;j++) {
          /* Pick a length */
          size_t len = TEST_RAND(1,segsize-starting_point);
          size_t remote_starting_point = TEST_RAND(0,segsize-len);
          size_t local_starting_point_1 = TEST_RAND(0,segsize-len);
          size_t local_starting_point_2 = TEST_RAND(0,segsize-len);

          /* Perform operations */
          /* Out of segment put from shadow_region 1 to remote */
          gex_RMA_PutBlocking(myteam, peerproc,target_base+remote_starting_point,shadow_region_1 + starting_point,len, 0); 
  
          /* In segment get from remote to local segment */
          gex_RMA_GetBlocking(myteam, local_base+local_starting_point_1,peerproc,target_base+remote_starting_point,len, 0); 
  
          /* Verify */
          assert_eq(shadow_region_1 + starting_point, local_base + local_starting_point_1, len,starting_point,i,j,"Out of segment put + in segment get");
  
          /* Out of segment get from remote to shadow_region_2 (starting from 0) */
          gex_RMA_GetBlocking(myteam, shadow_region_2+local_starting_point_2,peerproc,target_base+remote_starting_point,len, 0); 
  
          /* Verify */
          assert_eq(shadow_region_2+local_starting_point_2, shadow_region_1 + starting_point, len,starting_point,i,j,"Out of segment get");
        }
        TEST_PROGRESS_BAR(i,outer_iterations);
      }
      BARRIER();
    }
    if(sender_p && !failures) {
      MSG("testslice PASSED");
    }
    gasnet_exit(0);

    return 0;

}
/* ------------------------------------------------------------------------------------ */
