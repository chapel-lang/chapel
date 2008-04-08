/* $Id: perf_nb.c,v 1.3 2004-03-29 19:14:51 vinod Exp $ */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

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
#   define MP_TIMER          TCGTIME_
#else
#   include <mpi.h>
#   define MP_BARRIER()      MPI_Barrier(MPI_COMM_WORLD)
#   define MP_FINALIZE()     MPI_Finalize()
#   define MP_INIT(arc,argv) MPI_Init(&(argc),&(argv))
#   define MP_MYID(pid)      MPI_Comm_rank(MPI_COMM_WORLD, (pid))
#   define MP_PROCS(pproc)   MPI_Comm_size(MPI_COMM_WORLD, (pproc));
#   define MP_TIMER          MPI_Wtime
#endif

#include "armci.h"

#define DIM1 5
#define DIM2 3
#ifdef __sun
/* Solaris has shared memory shortages in the default system configuration */
# define DIM3 6
# define DIM4 5
# define DIM5 4
#elif defined(__alpha__)
# define DIM3 8
# define DIM4 5
# define DIM5 6
#else
# define DIM3 8
# define DIM4 9
# define DIM5 7
#endif
#define DIM6 3
#define DIM7 2


#define OFF 1
#define EDIM1 (DIM1+OFF)
#define EDIM2 (DIM2+OFF)
#define EDIM3 (DIM3+OFF)
#define EDIM4 (DIM4+OFF)
#define EDIM5 (DIM5+OFF)
#define EDIM6 (DIM6+OFF)
#define EDIM7 (DIM7+OFF)

#define DIMS 4
#define MAXDIMS 7
#define MAX_DIM_VAL 50 
#define LOOP 200

#define BASE 100.
#define MAXPROC 8
#define TIMES 100

#ifdef CRAY
# define ELEMS 800
#else
# define ELEMS 200
#endif

#define MAXELEMS      131072 /* 262144 */
#define MAX_REQUESTS  MAXELEMS
#define PUT 11
#define GET 22
#define ACC 33

int VERIFY=1; /* verifies results */
int DEBUG=0;  /* if debug=1, dump extra messages */

/***************************** macros ************************/
#define COPY(src, dst, bytes) memcpy((dst),(src),(bytes))
#define MAX(a,b) (((a) >= (b)) ? (a) : (b))
#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define ABS(a) (((a) <0) ? -(a) : (a))

/***************************** global data *******************/
int me, nproc;
void* work[MAXPROC]; /* work array for propagating addresses */
double *ddst[MAXPROC];

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

void verify_results(int op, int *elems) {
    int i, j;
      
    switch(op) {

    case PUT:
      if(!(me==0))
	for(j=0; j<elems[1]; j++) {
	  if( ABS(ddst[me][j]-j*1.001) > 0.1) {
	    ARMCI_Error("put failed...Invalid Value Obtained..1", 0);
	  }
	}
      MP_BARRIER();
      if(DEBUG) if(me==0) printf("  verifying put ..O.K.\n");
      break;
      
    case GET:
      if(me==0) {
	for(i=1; i<nproc; i++) {
	  for(j=0; j<elems[1]; j++) {
	    if( ABS(ddst[me][i*elems[1]+j]-j*1.001*(i+1)) > 0.1) 
	      ARMCI_Error("get failed...Invalid Value Obtained..1", 0);
	  }
	}
      }
      MP_BARRIER();
      if(DEBUG) if(me==0) printf("  verifying get ..O.K.\n\n");
      break;
      
    case ACC: 
      if(me==0)
	for(j=0; j<elems[1]; j++) {
	  /*printf("ddst[%d][%d] = %lf\n", me, j, ddst[me][j]);
	    fflush(stdout); */
	  if( ABS(ddst[me][j]-(double)nproc) > 0.1) {
	    ARMCI_Error("accumulate failed...Invalid Value Obtained..1", 0);
	  }
	}
      MP_BARRIER();
      if(DEBUG)if(me==0) printf("  verifying accumulate ..O.K.\n"); 
      break;
      
    default:
      ARMCI_Error("Invalid Operation", 0);
    }
    fflush(stdout);
}


void test_perf_nb(int dry_run) {
  
    int i, j, loop, rc, bytes, elems[2] = {MAXPROC, MAXELEMS};
    int stride, k=0, ntimes;
    double stime, t1, t2, t3, t4, t5, t6, t7, t8, t9;
    double *dsrc[MAXPROC], scale=1.0;
    armci_hdl_t hdl_get, hdl_put, hdl_acc;
        
    create_array((void**)ddst, sizeof(double),2, elems);
    create_array((void**)dsrc, sizeof(double),1, &elems[1]);

    if(!dry_run)if(me == 0) {
      printf("\n\t\t\tRemote 1-D Array Section\n");
      printf("section    get      nbget    wait     put     nbput  ");
      printf("   wait     acc     nbacc     wait\n");
      printf("-------  -------- -------- -------- -------- --------");
      printf(" -------- -------- -------- --------\n");
      fflush(stdout);
    }

    for(loop=1; loop<=MAXELEMS; loop*=2, k++) {

      elems[1] = loop;
      ntimes = (int)sqrt((double)(MAXELEMS/elems[1]));
      if(ntimes <1) ntimes=1;

      /* -------------------------- SETUP --------------------------- */
      /*initializing non-blocking handles,time,src & dst buffers*/
      ARMCI_INIT_HANDLE(&hdl_put);
      ARMCI_INIT_HANDLE(&hdl_get);
      ARMCI_INIT_HANDLE(&hdl_acc);
      t1 = t2 = t3 = t4 = t5 = t6 = t7 = t8 = t9 = 0.0;
      for(i=0; i<elems[1]; i++) dsrc[me][i]=i*1.001*(me+1);
      for(i=0; i<elems[0]*elems[1]; i++) ddst[me][i]=0.0;    
      MP_BARRIER();
      
      /* bytes transfered */
      bytes = sizeof(double)*elems[1]; 
      MP_BARRIER();
      
      /* -------------------------- PUT/GET -------------------------- */    
      if(me == 0) {
	for(i=1; i<nproc; i++) {
	  stime=MP_TIMER();
	  for(j=0; j<ntimes; j++)
	    if((rc=ARMCI_Put(&dsrc[me][0], &ddst[i][me*elems[1]], bytes,i)))
	      ARMCI_Error("armci_nbput failed\n",rc);
	  t1 += MP_TIMER()-stime;
	}
      }
      MP_BARRIER(); ARMCI_AllFence(); MP_BARRIER();
      if(VERIFY) verify_results(PUT, elems);
      for(i=0; i<elems[0]*elems[1]; i++) ddst[me][i]=0.0;
      MP_BARRIER();
      
      if(me == 0) { 
	for(i=1; i<nproc; i++) {
	  stime=MP_TIMER();    
	  for(j=0; j<ntimes; j++)
	    if((rc=ARMCI_Get(&dsrc[i][0], &ddst[me][i*elems[1]], bytes,i)))
	      ARMCI_Error("armci_nbget failed\n",rc);
	  t4 += MP_TIMER()-stime;	
	}
      }    
      MP_BARRIER(); ARMCI_AllFence(); MP_BARRIER();
      if(VERIFY) verify_results(GET, elems);
      for(i=0; i<elems[0]*elems[1]; i++) ddst[me][i]=0.0;
      MP_BARRIER();

      /* ------------------------ nb PUT/GET ------------------------- */    
      if(me == 0) {
	for(i=1; i<nproc; i++) {
	  for(j=0; j<ntimes; j++) {
	    stime=MP_TIMER();    
	    if((rc=ARMCI_NbPut(&dsrc[me][0], &ddst[i][me*elems[1]], bytes,
			       i, &hdl_put)))
	      ARMCI_Error("armci_nbput failed\n",rc);
	    t2 += MP_TIMER()-stime;	stime=MP_TIMER();
	    ARMCI_Wait(&hdl_put);
	    t3 += MP_TIMER()-stime;
	  } 
	}
      }
      MP_BARRIER(); ARMCI_AllFence(); MP_BARRIER();
      if(VERIFY) verify_results(PUT, elems);
      for(i=0; i<elems[0]*elems[1]; i++) ddst[me][i]=0.0;
      MP_BARRIER();

      if(me == 0) {
	for(i=1; i<nproc; i++) {
	  for(j=0; j<ntimes; j++) {
	    stime=MP_TIMER();    
	    if((rc=ARMCI_NbGet(&dsrc[i][0], &ddst[me][i*elems[1]], bytes,
			       i, &hdl_get)))
	      ARMCI_Error("armci_nbget failed\n",rc);
	    t5 += MP_TIMER()-stime;	stime=MP_TIMER();
	    ARMCI_Wait(&hdl_get);
	    t6 += MP_TIMER()-stime;
	  }
	}
      }
      MP_BARRIER(); ARMCI_AllFence(); MP_BARRIER();
      if(VERIFY) verify_results(GET, elems);
      for(i=0; i<elems[0]*elems[1]; i++) ddst[me][i]=0.0; 
      MP_BARRIER();


      /* ------------------------ Accumulate ------------------------- */    
      for(i=0; i<elems[1]; i++) dsrc[me][i]=1.0;  MP_BARRIER();
      stride = elems[1]*sizeof(double); scale  = 1.0;
      for(j=0; j<ntimes; j++) {
	stime=MP_TIMER();
	if((rc=ARMCI_AccS(ARMCI_ACC_DBL, &scale, &dsrc[me][0], &stride, 
			  &ddst[0][0], &stride, &bytes, 0, 0)))
	  ARMCI_Error("armci_acc failed\n",rc);
	t7 += MP_TIMER()-stime;
	
	MP_BARRIER(); ARMCI_AllFence(); MP_BARRIER();
	if(VERIFY) verify_results(ACC, elems);
	for(i=0; i<elems[0]*elems[1]; i++) ddst[me][i]=0.0;
	MP_BARRIER();
      }

#if 1
      /* See the note below why this part is disabled */
      /* ---------------------- nb-Accumulate ------------------------ */    
      for(i=0; i<elems[1]; i++) dsrc[me][i]=1.0;  MP_BARRIER();
      stride = elems[1]*sizeof(double); scale  = 1.0;
      for(j=0; j<ntimes; j++) {
	stime=MP_TIMER();    
	if((rc=ARMCI_NbAccS(ARMCI_ACC_DBL, &scale, &dsrc[me][0], &stride, 
			    &ddst[0][0], &stride, &bytes, 0, 0, &hdl_acc)))
	  ARMCI_Error("armci_nbacc failed\n",rc);
	t8 += MP_TIMER()-stime; stime=MP_TIMER();
	ARMCI_Wait(&hdl_acc);
	t9 += MP_TIMER()-stime;
      
	MP_BARRIER(); ARMCI_AllFence(); MP_BARRIER();
	if(VERIFY) verify_results(ACC, elems);
	for(i=0; i<elems[0]*elems[1]; i++) ddst[me][i]=0.0;
	MP_BARRIER();
      }
#endif

      /* print timings */
     if(!dry_run) if(me==0) printf("%d\t %.2e %.2e %.2e %.2e %.2e %.2e %.2e %.2e %.2e\n", 
		       bytes, t4/ntimes, t5/ntimes, t6/ntimes, t1/ntimes, 
		       t2/ntimes, t3/ntimes, t7/ntimes, t8/ntimes, t9/ntimes);
    }

    ARMCI_AllFence();
    MP_BARRIER();
    
    if(!dry_run)if(me==0){printf("O.K.\n"); fflush(stdout);}
    destroy_array((void **)ddst);
    destroy_array((void **)dsrc);
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

    if(nproc < 2 || nproc> MAXPROC) {
      if(me == 0)
	fprintf(stderr,
		"USAGE: 2 <= processes < %d - got %d\n", MAXPROC, nproc);
      MP_BARRIER();
      MP_FINALIZE();
      exit(0);
    }

    if(me==0){
       printf("ARMCI test program (%d processes)\n",nproc); 
       fflush(stdout);
       sleep(1);
    }
    
    ARMCI_Init();

    if(me==0){
      printf("\n put/get/acc requests (Time in secs)\n\n");
      fflush(stdout);
    }

    test_perf_nb(1);
    test_perf_nb(0);
    
    ARMCI_AllFence();
    MP_BARRIER();
    if(me==0){printf("\nSuccess!!\n"); fflush(stdout);}
    sleep(2);
	
    MP_BARRIER();
    ARMCI_Finalize();
    MP_FINALIZE();
    return(0);
}

/* 
   NOTE: ARMCI_NbAcc fails in opus for buffer sizes greater than 400Kb 
*/
