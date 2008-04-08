/* $Id: test_groups.c,v 1.3 2004-11-22 20:29:53 manoj Exp $ */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef WIN32
#  include <windows.h>
#  define sleep(x) Sleep(1000*(x))
#else
#  include <unistd.h>
#endif

/* ARMCI is impartial to message-passing libs - we handle them with MP macros */
#if defined(PVM)
#   include <pvm3.h>
#   ifdef CRAY
#     define MPGROUP         (char *)NULL
#     define MP_INIT(arc,argv)
#   else
#     define MPGROUP           "mp_working_group"
#     define MP_INIT(arc,argv) pvm_init(arc, argv)
#   endif
#   define MP_FINALIZE()     pvm_exit()
#   define MP_TIMER          armci_timer
#   define MP_BARRIER()      pvm_barrier(MPGROUP,-1)
#   define MP_MYID(pid)      *(pid)   = pvm_getinst(MPGROUP,pvm_mytid())
#   define MP_PROCS(pproc)   *(pproc) = (int)pvm_gsize(MPGROUP)
    void pvm_init(int argc, char *argv[]);
#elif defined(TCGMSG)
#   include <sndrcv.h>
    long tcg_tag =30000;
#   define MP_BARRIER()      SYNCH_(&tcg_tag)
#   define MP_INIT(arc,argv) PBEGIN_((argc),(argv))
#   define MP_FINALIZE()     PEND_()
#   define MP_MYID(pid)      *(pid)   = (int)NODEID_()
#   define MP_PROCS(pproc)   *(pproc) = (int)NNODES_()
#   define MP_TIMER         TCGTIME_
#else
#   include <mpi.h>
#   define MP_BARRIER()      MPI_Barrier(MPI_COMM_WORLD)
#   define MP_FINALIZE()     MPI_Finalize()
#   define MP_INIT(arc,argv) MPI_Init(&(argc),&(argv))
#   define MP_MYID(pid)      MPI_Comm_rank(MPI_COMM_WORLD, (pid))
#   define MP_PROCS(pproc)   MPI_Comm_size(MPI_COMM_WORLD, (pproc));
#   define MP_TIMER         MPI_Wtime
#endif

#include "armci.h"

#define MAXDIMS 7
#define MAXPROC 128
#define MINPROC 4

typedef int ARMCI_Datatype;

/***************************** macros ************************/
#define COPY(src, dst, bytes) memcpy((dst),(src),(bytes))
#define MAX(a,b) (((a) >= (b)) ? (a) : (b))
#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define ABS(a) (((a) <0) ? -(a) : (a))

/***************************** global data *******************/
int me, nproc;
void* work[MAXPROC]; /* work array for propagating addresses */

#ifdef PVM
void pvm_init(int argc, char *argv[])
{
    int mytid, mygid, ctid[MAXPROC];
    int np, i;

    mytid = pvm_mytid();
    if((argc != 2) && (argc != 1)) goto usage;
    if(argc == 1) np = 1;
    if(argc == 2)
        if((np = atoi(argv[1])) < 1) goto usage;
    if(np > MAXPROC) goto usage;

    mygid = pvm_joingroup(MPGROUP);

    if(np > 1)
        if (mygid == 0) 
            i = pvm_spawn(argv[0], argv+1, 0, "", np-1, ctid);

    while(pvm_gsize(MPGROUP) < np) sleep(1);

    /* sync */
    pvm_barrier(MPGROUP, np);
    
    printf("PVM initialization done!\n");
    
    return;

usage:
    fprintf(stderr, "usage: %s <nproc>\n", argv[0]);
    pvm_exit();
    exit(-1);
}
#endif
          
void create_array(void *a[], int elem_size, int ndim, int dims[])
{
     int bytes=elem_size, i, rc;

     assert(ndim<=MAXDIMS);
     for(i=0;i<ndim;i++)bytes*=dims[i];

     rc = ARMCI_Malloc(a, bytes);
     assert(rc==0);
     
     assert(a[me]);
     
}

void destroy_array(void *ptr[])
{
    MP_BARRIER();

    assert(!ARMCI_Free(ptr[me]));
}

#define GNUM_A 3
#define GNUM_B 2
#define ELEMS 10

/* to check if a process belongs to this group */
int chk_grp_membership(int rank, ARMCI_Group *grp, int *memberlist) 
{
    int i,grp_size;
    ARMCI_Group_size(grp, &grp_size);
    for(i=0; i<grp_size; i++) if(rank==memberlist[i]) return 1;
    return 0;
}


void test_groups() {
  
    int pid_listA[MAXPROC]  = {0,1,2};
    int pid_listB[MAXPROC] = {1,3};
    int value = -1, bytes;
    ARMCI_Group groupA, groupB;

    MP_BARRIER();

    ARMCI_Group_create(GNUM_A, pid_listA, &groupA); /* create group 1 */
    ARMCI_Group_create(GNUM_B, pid_listB, &groupB); /* create group 2 */


    /* ------------------------ GROUP A ------------------------- */ 
    if(chk_grp_membership(me, &groupA, pid_listA)) { /* group A */
       int grp_me, grp_size;
       int i,j,src_proc=2,dst_proc=0;
       double *ddst_put[MAXPROC];
       double dsrc[ELEMS];
       int elems[2] = {MAXPROC,ELEMS};

       ARMCI_Group_rank(&groupA, &grp_me);
       ARMCI_Group_size(&groupA, &grp_size);
       if(grp_me==0) printf("GROUP SIZE = %d\n", grp_size);
       printf("%d:group rank = %d\n", me, grp_me);
       
       bytes = ELEMS*sizeof(double);       
       ARMCI_Malloc_group((void **)ddst_put, bytes, &groupA);
       
       for(i=0; i<ELEMS; i++) dsrc[i]=i*1.001*(grp_me+1); 
       for(i=0; i<ELEMS; i++) ddst_put[grp_me][i]=-1.0;
       
       armci_msg_group_barrier(&groupA);
       
       if(grp_me==src_proc) {
          /* NOTE: make sure to specify absolute ids in ARMCI calls */
	  ARMCI_Put(dsrc, &ddst_put[dst_proc][0], bytes,
                    ARMCI_Absolute_id(&groupA,dst_proc));
       }
       
       armci_msg_group_barrier(&groupA);
       /* NOTE: make sure to specify absolute ids in ARMCI calls */
       ARMCI_Fence(ARMCI_Absolute_id(&groupA,dst_proc));
       sleep(1);
       
       
       /* Verify*/
       if(grp_me==dst_proc) {
	  for(j=0; j<ELEMS; j++) {
	     if(ABS(ddst_put[grp_me][j]-j*1.001*(src_proc+1)) > 0.1) {
		printf("\t%d: ddst_put[%d][%d] = %lf and expected value is %lf\n",
                       me, grp_me, j, ddst_put[grp_me][j], j*1.001*(src_proc+1));
		ARMCI_Error("groups: armci put failed...1", 0);
	     }
	  }
	  printf("\n%d: Test O.K. Verified\n", dst_proc);
       }
       armci_msg_group_barrier(&groupA);
       ARMCI_Free_group(ddst_put[grp_me], &groupA);
    }

    MP_BARRIER();
    
    /* ------------------------ GROUP B ------------------------- */ 
    if(chk_grp_membership(me, &groupB, pid_listB)) { /* group B */
       int grp_me, grp_size;
       int i,j,src_proc=1,dst_proc=0;
       double *ddst_put[MAXPROC];
       double dsrc[ELEMS];
       int elems[2] = {MAXPROC,ELEMS};

       ARMCI_Group_rank(&groupB, &grp_me);
       ARMCI_Group_size(&groupB, &grp_size);
       if(grp_me==0) printf("GROUP SIZE = %d\n", grp_size);
       printf("%d:group rank = %d\n", me, grp_me);
       
       bytes = ELEMS*sizeof(double);       
       ARMCI_Malloc_group((void **)ddst_put, bytes, &groupB);
       
       for(i=0; i<ELEMS; i++) dsrc[i]=i*1.001*(grp_me+1); 
       for(i=0; i<ELEMS; i++) ddst_put[grp_me][i]=0.0;
       
       armci_msg_group_barrier(&groupB);

       if(grp_me==src_proc) {
	  ARMCI_Put(dsrc, &ddst_put[dst_proc][0], bytes,
                    ARMCI_Absolute_id(&groupB,dst_proc));
       }
       
       armci_msg_group_barrier(&groupB);
       ARMCI_Fence(ARMCI_Absolute_id(&groupB,dst_proc));
       sleep(1);
       
       /* Verify*/
       if(grp_me==dst_proc) {
	  for(j=0; j<ELEMS; j++) {
	     if(ABS(ddst_put[grp_me][j]-j*1.001*(src_proc+1)) > 0.1) {
		printf("\t%d: ddst_put[%d][%d] = %lf and expected value is %lf\n",
                       me, grp_me, j, ddst_put[grp_me][j], j*1.001*(src_proc+1));
		ARMCI_Error("groups: armci put failed...1", 0);
	     }
	  }
	  printf("\n%d: Test O.K. Verified\n", dst_proc);
       }
       armci_msg_group_barrier(&groupB);
       ARMCI_Free_group(ddst_put[grp_me], &groupB);
    }


    ARMCI_AllFence();
    MP_BARRIER();
    
    if(me==0){printf("O.K.\n"); fflush(stdout);}
}


/* we need to rename main if linking with frt compiler */
#ifdef FUJITSU_FRT
#define main MAIN__
#endif

int main(int argc, char* argv[])
{

    MP_INIT(argc, argv);
    MP_PROCS(&nproc);
    MP_MYID(&me);

/*    printf("nproc = %d, me = %d\n", nproc, me);*/
    
    if( (nproc<MINPROC || nproc>MAXPROC) && me==0)
       ARMCI_Error("Test works for up to %d processors\n",MAXPROC);

    if(me==0){
       printf("ARMCI test program (%d processes)\n",nproc); 
       fflush(stdout);
       sleep(1);
    }
    
    ARMCI_Init();

    if(me==0){
      printf("\n Testing ARMCI Groups!\n\n");
      fflush(stdout);
    }

    test_groups();
    
    ARMCI_AllFence();
    MP_BARRIER();
    if(me==0){printf("\nSuccess!!\n"); fflush(stdout);}
    sleep(2);
	
    MP_BARRIER();
    ARMCI_Finalize();
    MP_FINALIZE();
    return(0);
}
