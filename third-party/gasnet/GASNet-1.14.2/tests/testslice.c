/*   $Source: /var/local/cvs/gasnet/tests/testslice.c,v $
 *     $Date: 2010/04/16 22:28:15 $
 * $Revision: 1.8.2.1 $
 * Description: GASNet randomized get/put correctness validation test
 * Copyright 2007, Parry Husbands and Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include <gasnet.h>
#include <gasnet_coll.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

int segsize = 0;
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR ((uintptr_t)segsize)
#endif
#include "test.h"

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
    int numprocs, myproc, peerproc;
    int sender_p;
    char *shadow_region_1, *shadow_region_2;
    int i,j;
    char *local_base, *target_base;

    /* call startup */
    GASNET_Safe(gasnet_init(&argc, &argv));

    /* get SPMD info */
    myproc = gasnet_mynode();
    numprocs = gasnet_nodes();

    if (argc > 1) segsize = atoi(argv[1]);
    if (!segsize) segsize = 1024*1000;
    if (argc > 2) outer_iterations = atoi(argv[2]);
    if (!outer_iterations) outer_iterations = 10;
    if (argc > 3) inner_iterations = atoi(argv[3]);
    if (!inner_iterations) inner_iterations = 10;
    if (argc > 4) seedoffset = atoi(argv[4]);

    GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ, TEST_MINHEAPOFFSET));

    test_init("testslice",0, "(segsize) (iterations) (# of sizes per iteration) (seed)");

    /* parse arguments */
    if (argc > 5) test_usage();
    
    if(numprocs & 1) {
        MSG("WARNING: This test requires an even number of nodes. Test skipped.\n");
        gasnet_exit(0); /* exit 0 to prevent false negatives in test harnesses for smp-conduit */
    }
    sender_p = !(myproc & 1);
    peerproc = myproc ^ 1;

    if (seedoffset == 0) {
      seedoffset = (((unsigned int)TIME()) & 0xFFFF);
      TEST_BCAST(&seedoffset, 0, &seedoffset, sizeof(&seedoffset));
    }
    TEST_SRAND(myproc+seedoffset);

    MSG0("Running with segment size = %d outer iterations=%d inner iterations=%d seed=%d",
         segsize,outer_iterations, inner_iterations, seedoffset);

    BARRIER();

    /* Allocate two shadow regions the same size as the segment */
    shadow_region_1 = (char *) test_malloc(segsize);
    shadow_region_2 = (char *) test_malloc(segsize);
   
    /* Fill up the shadow region with random data */
    for(i=0;i < segsize;i++) {
      shadow_region_1[i] = (char) TEST_RAND(0,255);
    }
    memset(shadow_region_2,0,segsize);

    /* Big loop performing the following */
    for(i=0;i < outer_iterations;i++) {
      if(sender_p) {
        /* Pick a starting point anywhere in the segment */
        int starting_point = TEST_RAND(0,(segsize-1));

        local_base = TEST_SEG(myproc);
        target_base = TEST_SEG(peerproc);
 
        for(j=0;j < inner_iterations;j++) {
          /* Pick a length */
          int len = TEST_RAND(1,segsize-starting_point);
          int remote_starting_point = TEST_RAND(0,segsize-len);
          int local_starting_point_1 = TEST_RAND(0,segsize-len);
          int local_starting_point_2 = TEST_RAND(0,segsize-len);

          /* Perform operations */
          /* Out of segment put from shadow_region 1 to remote */
          gasnet_put(peerproc,target_base+remote_starting_point,shadow_region_1 + starting_point,len); 
  
          /* In segment get from remote to local segment */
          gasnet_get(local_base+local_starting_point_1,peerproc,target_base+remote_starting_point,len); 
  
          /* Verify */
          assert_eq(shadow_region_1 + starting_point, local_base + local_starting_point_1, len,starting_point,i,j,"Out of segment put + in segment get");
  
          /* Out of segment get from remote to shadow_region_2 (starting from 0) */
          gasnet_get(shadow_region_2+local_starting_point_2,peerproc,target_base+remote_starting_point,len); 
  
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
