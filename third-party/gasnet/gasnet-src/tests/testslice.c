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

size_t arenasz = 0;
int doforeign = 0;
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR ((uintptr_t)arenasz*(doforeign?4:2))
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

    int crossmachinemode = 0;
    int help = 0;

    /* call startup */
    GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testslice", &argc, &argv, 0));

    /* get SPMD info */
    myproc = gex_TM_QueryRank(myteam);
    numprocs = gex_TM_QuerySize(myteam);

    // Parse cmdline args
    int arg = 1;
    while (argc > arg) {
      if (!strcmp(argv[arg], "-c")) {
        crossmachinemode = 1;
        ++arg;
      } else if (!strcmp(argv[arg], "-f")) {
        doforeign = 1;
        ++arg;
      } else if (argv[arg][0] == '-') {
        help = 1;
        ++arg;
      } else break;
    }

    if (argc > arg) { arenasz = gasnett_parse_int(argv[arg], 1); ++arg; }
    if (!arenasz) arenasz = 1024*1024*16;
    if (argc > arg) { outer_iterations = atoi(argv[arg]); ++arg; }
    if (!outer_iterations) outer_iterations = 10;
    if (argc > arg) { inner_iterations = atoi(argv[arg]); ++arg; }
    if (!inner_iterations) inner_iterations = 10;
    if (argc > arg) { seedoffset = atoi(argv[arg]); ++arg; }

    if (doforeign) {
      gex_Rank_t nbrhd_set_size;
      gex_System_QueryMyPosition(&nbrhd_set_size, NULL, NULL, NULL);
      if (nbrhd_set_size == numprocs) { // all nbrhs are single-process
        MSG0("WARNING: Ignoring '-f' since there are no foreign segments.");
        doforeign = 0;
      }
    }

    GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ));

    test_init("testslice",0, "[options] (arena size) (iterations) (# of sizes per iteration) (seed)\n"
              "  The -c option enables cross-machine pairing, default is nearest neighbor.\n"
              "  The -f option enables use of a 'foreign' GASNet segment (one cross-mapped\n"
              "   from another process) as the 'local' address for communications.");
    if (help || argc > arg) test_usage();
    
    if (crossmachinemode) {
      if ((numprocs%2) && (myproc == numprocs-1)) {
        peerproc = myproc;
      } else {
        gex_Rank_t half = numprocs / 2;
        peerproc = (myproc < half) ? (myproc + half) : (myproc - half);
      }
    } else {
      peerproc = (myproc + 1) % numprocs;
    }

    if (seedoffset == 0) {
      seedoffset = (((unsigned int)TIME()) & 0xFFFF);
      TEST_BCAST(&seedoffset, 0, &seedoffset, sizeof(seedoffset));
    }
    TEST_SRAND(myproc+seedoffset);

    MSG0("Running %s%stest with arena size=%"PRIuSZ" outer iterations=%d inner iterations=%d seed=%d",
         (crossmachinemode ? "cross-machine ": ""),
         (doforeign ? "foreign-segment ": ""),
         arenasz,outer_iterations, inner_iterations, seedoffset);

    // Allocate two shadow regions the same size as the arena
    char *shadow_region_1, *shadow_region_2;
    if (doforeign) {
      // Use cross-mapped (localized) segment of a neighbor if possible
      gex_RankInfo_t *nbrhdinfo = NULL;
      gex_Rank_t nbrhdsize, nbrhdrank;
      gex_System_QueryNbrhdInfo(&nbrhdinfo, &nbrhdsize, &nbrhdrank);
      // Using segment of "left" (-1) neighbor to avoid (when possible) using same as peerproc
      gex_Rank_t nbrproc = nbrhdinfo[(nbrhdrank + nbrhdsize - 1) % nbrhdsize].gex_jobrank;
      char *nbr_seg_local_addr = NULL;
      gex_Event_Wait( gex_EP_QueryBoundSegmentNB(myteam, nbrproc, NULL, (void**)&nbr_seg_local_addr, NULL, 0) );
      assert_always(nbr_seg_local_addr != NULL);
      shadow_region_1 = nbr_seg_local_addr + 2*arenasz;
    } else {
      // Just plain local memory
      shadow_region_1 = (char *) test_malloc(2*arenasz);
    }
    shadow_region_2 = shadow_region_1 + arenasz;

    // Initialize the two shadow regions with random bytes and zeros, respectively
    // We take care to ensure "owner writes" even for foreign segments
    {
      uint32_t *tmp1 = (uint32_t *)
                       (doforeign ? ((char *)TEST_MYSEG() + 2*arenasz)
                                  : shadow_region_1);
      for(size_t k=0;k < arenasz / sizeof(uint32_t);k++) {
        tmp1[k] = TEST_RAND(0, UINT32_MAX);
      }
      char *tmp2 = (doforeign ? ((char *)TEST_MYSEG() + 3*arenasz)
                              : shadow_region_2);
      memset(tmp2,0,arenasz);
    }

    char *local_base  = (char *)TEST_MYSEG();
    char *target_base = (char *)TEST_SEG(peerproc) + arenasz;

    BARRIER();

    /* Big loop performing the following */
    for(int i=0;i < outer_iterations;i++) {
        /* Pick a starting point anywhere in the segment */
        size_t starting_point = TEST_RAND(0,(arenasz-1));
 
        for(int j=0;j < inner_iterations;j++) {
          /* Pick a length */
          size_t len = TEST_RAND(1,arenasz-starting_point);
          size_t remote_starting_point = TEST_RAND(0,arenasz-len);
          size_t local_starting_point_1 = TEST_RAND(0,arenasz-len);
          size_t local_starting_point_2 = TEST_RAND(0,arenasz-len);

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

      BARRIER();
    }
    if (!doforeign) test_free(shadow_region_1);

    if(!failures) {
      MSG("testslice PASSED");
    }
    gasnet_exit(0);

    return 0;

}
/* ------------------------------------------------------------------------------------ */
