#include <stdio.h>
#include <stdlib.h>
#include "armci.h"
#include "message.h"
#include "mp3.h"

#define armci_msg_brdcst__ armci_msg_bcast_lapi

int me, nproc;

#define LOOP 20

void time_gop(double *test,int len)
{
int i;
double t;

  t = MP_TIMER();
  for(i=0; i<LOOP; i++){
    armci_msg_dgop(test, len, "+");
  }
  t = MP_TIMER() -t;

  t /= LOOP;

  if(me==0){
     printf("Time per gop %lf len=%d doubles\n",t, len);
     fflush(stdout);
  }
}


void time_reduce(double *test,int len)
{
int i;
double t;

  t = MP_TIMER();
  for(i=0; i<LOOP; i++){
    armci_msg_reduce(test, len, "+",ARMCI_DOUBLE);
  }
  t = MP_TIMER() -t;

  t /= LOOP;

  if(me==0){
     printf("Time per reduce %lf len=%d doubles\n",t, len);
     fflush(stdout);
  }
}



void TestGlobals()
{
#define MAXLENG 256*1024
  double *dtest;
  int *itest;
  long *ltest;
  int len;
  int ifrom=nproc-1,lfrom=1,dfrom=1;

  if (me == 0) {
    printf("Global test ... broadcast and reduction for int, long, double\n----------\n");
    fflush(stdout);
  }

  if (!(dtest = (double *) malloc((unsigned) (MAXLENG*sizeof(double)))))
    ARMCI_Error("TestGlobals: failed to allocated dtest", MAXLENG);
  if (!(ltest = (long *) malloc((unsigned) (MAXLENG*sizeof(long)))))
    ARMCI_Error("TestGlobals: failed to allocated ltest", MAXLENG);
  if (!(itest = (int *) malloc((unsigned) (MAXLENG*sizeof(int)))))
    ARMCI_Error("TestGlobals: failed to allocated itest", MAXLENG);

  for (len=1; len<MAXLENG; len*=2) {
    int ilen = len*sizeof(int);
    int dlen = len*sizeof(double);
    int llen = len*sizeof(long);
    int i;
   
    ifrom = (ifrom+1)%nproc;
    lfrom = (lfrom+1)%nproc; 
    dfrom = (lfrom+1)%nproc;

#if 0
    printf("%d:ifrom=%d lfrom=%d dfrom=%d\n",me,ifrom,lfrom,dfrom);fflush(stdout);
#endif

    if (me == 0) {
      printf("Test length = %d ... ", len);
      fflush(stdout);
    }

    if(me == ifrom)for (i=0; i<len; i++)itest[i]=i;
    else for (i=0; i<len; i++)itest[i]=0;
    if(me == lfrom)for (i=0; i<len; i++)ltest[i]=(long)i;
    else for (i=0; i<len; i++)ltest[i]=0L;
    if(me == dfrom)for (i=0; i<len; i++)dtest[i]=(double)i;
    else for (i=0; i<len; i++)dtest[i]=0.0;
    
    /* Test broadcast */
    armci_msg_brdcst(itest, ilen, ifrom);
    armci_msg_brdcst(ltest, llen, lfrom);
    armci_msg_brdcst(dtest, dlen, dfrom);
   
    for (i=0; i<len; i++){
      if (itest[i] != i) armci_die2("int broadcast failed", i,itest[i]);
      if (ltest[i] != (long)i) 
                      armci_die2("long broadcast failed", i,(int)ltest[i]);
      if (dtest[i] != (double)i) 
                      armci_die2("double broadcast failed", i,(int)dtest[i]);
    }
      
    if (me == 0) {
      printf("broadcast OK ...");
      fflush(stdout);
    }

    /* Test global sum */
    for (i=0; i<len; i++) {
      itest[i] = i*me;
      ltest[i] = (long) itest[i];
      dtest[i] = (double) itest[i];
    }


    armci_msg_igop(itest, len, "+");
    armci_msg_lgop(ltest, len, "+");
    armci_msg_dgop(dtest, len, "+");
 

    for (i=0; i<len; i++) {
      int iresult = i*nproc*(nproc-1)/2;
      if (itest[i] != iresult || ltest[i] != (long)iresult || 
          dtest[i] != (double) iresult)
        ARMCI_Error("TestGlobals: global sum failed", (int) i);
    }


    if (me == 0) {
      printf("global sums OK\n");
      fflush(stdout);
    }
  }


  /* now we get timing data */
  time_gop(dtest,MAXLENG);
  time_reduce(dtest,MAXLENG);
     
  free((char *) itest);
  free((char *) ltest);
  free((char *) dtest);
}


int main( int argc, char **argv)
{   
    
  MP_INIT(argc,argv);
  MP_MYID(&me);
  MP_PROCS(&nproc);
    
    if(nproc < 2) {
        if(me == 0)
            fprintf(stderr,
                    "USAGE: 2 <= processes < %d\n", nproc);
        MP_BARRIER();
        MP_FINALIZE();
        exit(1);
    }

    if(me == 0){
       printf("Test of ARMCI Wrappers to Basic Message Passing Operations\n");
       fflush(stdout);
    }
   
    /* initialize ARMCI */
    ARMCI_Init();

    MP_BARRIER();
   
    TestGlobals();
   
    /* done */
    ARMCI_Finalize();
    MP_FINALIZE();
    return(0);
}   


