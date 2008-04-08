/* $Id: ipctest.c,v 1.2 1999-07-28 00:47:55 d3h325 Exp $ */
#include <stdio.h>
#include <mpi.h>
#include <assert.h>
#include "armci.h"
#include "shmem.h"
#include "locks.h"

int me, nproc;

void test()
{
    double *a, start=1., end=-1.;
    int len=100;
    long size = len*sizeof(double);
    long idlist[SHMIDLEN];
    int  numlock=10, i;
    lockset_t lockid;

    /* shared memory test */
    if(me==0){
        printf("Test shared memory\n");
        a=(double*)Create_Shared_Region(idlist+1,size,idlist);
        assert(a);
        a[0]= start;
        a[len-1]=end;
    }
    MPI_Bcast(idlist,SHMIDLEN,MPI_LONG,0,MPI_COMM_WORLD);
    if(me){
        a=(double*)Attach_Shared_Region(idlist+1,size,idlist[0]);
        assert(a);
    }

    if(me==nproc-1){
        printf("%d: start=%f end=%f\n",me,a[0],a[len-1]);
        if(a[0]== start && a[len-1]== end) printf("Works!\n");
    }

    /*printf("%d: a=%x\n",me,a); */

    MPI_Barrier(MPI_COMM_WORLD);

    /* allocate locks */
    if(me == 0){
        a[0]=0.;
        CreateInitLocks(numlock, &lockid);
        printf("\nMutual exclusion test\n");
    }
    MPI_Bcast(&lockid,sizeof(lockid),MPI_BYTE,0,MPI_COMM_WORLD);
    if(me)InitLocks(numlock, lockid);

    
    /* mutual exclusion test: 
     * everybody increments shared variable 1000 times
     */
#   define TIMES 1000

    MPI_Barrier(MPI_COMM_WORLD);

    for(i=0;i<TIMES; i++){
        NATIVE_LOCK(0);
        a[0]++;
        NATIVE_UNLOCK(0);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if(me==nproc-1){
        printf("value of shared variable =%f should be %f\n",
            a[0],1.0*nproc*TIMES);
        if(a[0]==1.0*nproc*TIMES ) printf("Works!\n\n");

    }

    /* cleanup of IPC resources */

    if(me==0){
        DeleteLocks(lockid);
        Delete_All_Regions();
    }

    MPI_Barrier(MPI_COMM_WORLD);
}



int main(int argc, char** argv)
{
      MPI_Init(&argc, &argv);
      MPI_Comm_size(MPI_COMM_WORLD, &nproc);
      MPI_Comm_rank(MPI_COMM_WORLD, &me);
      if(me==0)printf("Testing IPCs (%d MPI processes)\n\n",nproc);
      ARMCI_Init();
      test();
      ARMCI_Finalize();
      MPI_Finalize();
      return 0;

}

