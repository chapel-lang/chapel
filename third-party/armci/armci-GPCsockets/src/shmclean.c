/* The program was written to address missing "ipcrm" command on Mac X
 * It probes a range of System V shared memory id from 0 to MAXID
 * and if exist, it attempts to delete them.
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/param.h>
#include <errno.h>
#include <stdio.h>

#ifdef SUN
char *shmat();
#endif

    
#define MAXID 1000000
main(int argc, char **argv)
{
int i;
for(i=0; i<MAXID; i++){
   int rc = (int) shmat(i,(char *) NULL, 0);
   if(rc<0) continue;
   printf("found %d\n",i);
   shmdt((void*)rc);
   /* delete segment id */
   if(shmctl(  i, IPC_RMID,(struct shmid_ds *)NULL))
      printf("failed to remove shm id=%d\n",i);
}


}

