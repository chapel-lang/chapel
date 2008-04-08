/* $Id: shmtest.c,v 1.6 2003-10-22 22:12:21 d3h325 Exp $ */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/param.h>
#include <errno.h>
#include <stdio.h>

#ifdef SUN
char *shmat();
#endif

int armci_test_allocate(long size)
{
   char *ptr;
   long id = (long)shmget(IPC_PRIVATE, (size_t) size, (int) (IPC_CREAT |00600));
   if (id <0L) return 0;
#if 0
   /* attach to segment */
   ptr =  shmat((int) id, (char *) NULL, 0);
#else 
   ptr = (char *) NULL;
#endif

   /* delete segment id */
   if(shmctl( (int) id, IPC_RMID,(struct shmid_ds *)NULL))
      fprintf(stderr,"failed to remove shm id=%d\n",id);

   /* test pointer */
   if (((long)ptr) == -1L) return 0;
   else return 1;
}

    
/* parameters that define range and granularity of search for shm segment size  
 * UBOUND is chosen to be < 2GB to avoid overflowing on 32-bit systems
 * smaller PAGE gives more accurate results but with more search steps
 * LBOUND  is set to amount that is considered insufficient for our purposes 
 */

#define PAGE 131072L
#define UBOUND 2*4096*PAGE
#define LBOUND 4*PAGE

int verbose=1;

/*\ determine the max shmem segment size using bisection
\*/
void armci_shmem_init()
{
long x,i, y=0L;
long upper_bound=UBOUND;
long lower_bound=0;
     x = upper_bound;
     for(i=1;;i++){
        long step;
        int rc = armci_test_allocate(x);
        if(rc){
          if(verbose)printf("test %d size=%ld bytes: success\n",i,x);
          y=lower_bound = x;
          step = (upper_bound -x)>>1;
          if(step < 16*PAGE) break;
          x += step;
        }else{
          if(verbose)printf("test %d size=%ld bytes: failed\n",i,x);
          upper_bound = x;
          step = (x-lower_bound)>>1;
          if(step< PAGE || x < LBOUND) break;
          x -= step;
        }
      }
      if(verbose){
        if(x<LBOUND)
          printf("no usable amount (%d bytes) of shared memory available\n",LBOUND);
        else printf("%ld bytes segment size, %d calls \n",y,i);
      }else{
         printf("%d\n",y);
      }
}


main(int argc, char **argv)
{
  if(argc>1) verbose=0;
  if(verbose)printf("Searching for max shared memory segment size (SHMMAX) using bisection\n");
  armci_shmem_init();
}

