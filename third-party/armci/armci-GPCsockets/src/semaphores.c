/* $Id: semaphores.c,v 1.12 2005-03-10 19:11:23 vinodtipparaju Exp $ */
#include "semaphores.h"
#include <stdio.h>
#include <unistd.h>

int num_sem_alloc=0;
void perror();
#ifdef SUN
int  fprintf();
void fflush();
int semget(),semctl();
#endif

extern void armci_die(char*, int);

struct sembuf sops;
int semaphoreID;

/* follows LINUX semctl manpage */
#if (defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)) ||\
    defined(SGI) || defined(SUN) || defined(__FreeBSD__) || defined(INTERIX) || defined(MACX)
    /* union semun is defined by including <sys/sem.h> */
#   else
    /* according to X/OPEN we have to define it ourselves */
    union semun {
            int val;                    /* value for SETVAL */
            struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
            unsigned short int *array;  /* array for GETALL, SETALL */
            struct seminfo *__buf;      /* buffer for IPC_INFO */
    };
#endif



int SemGet(num_sem)
    int num_sem;
{
  semaphoreID = semget(IPC_PRIVATE,num_sem, IPC_CREAT | 0600);
  if(semaphoreID<0){
    fprintf(stderr," Semaphore Allocation Failed \nsuggestions to fix the problem: \n");
    fprintf(stderr," 1. run ipcs and ipcrm -s commands to clean any semaphore ids\n");
    fprintf(stderr," 2. verify if constant SEMMSL defined in file semaphore.h is set correctly for your system\n");
    fprintf(stderr," 3. recompile semaphore.c\n");
       sleep(1);
       perror("Error message from failed semget:");
       armci_die(" exiting ...", num_sem);
    }
       
    num_sem_alloc = num_sem;
    return(semaphoreID);
}

void SemInit(id,value)
    int id,value;
{
  int i, semid, num_sem;
  union semun semctl_arg;

    semctl_arg.val = value;

    if(id == ALL_SEMS){ semid = 0; num_sem = num_sem_alloc;}
      else { semid = id; num_sem = 1;}

    for(i=0; i< num_sem; i++){ 
       if( semctl(semaphoreID, semid, SETVAL,semctl_arg )<0){ 
         perror((char*)0);
         armci_die("SemInit error",id);
       }
       semid++;
    }
}


/*  release semaphore(s) */
void SemDel()
{
    union semun dummy;

    /* this is only to avoid compiler whinning about the unitialized variable*/
    dummy.val=0; 

    (void) semctl(semaphoreID,0,IPC_RMID,dummy);
}


void Sem_CreateInitLocks(int num, lockset_t *id)
{
     *id = SemGet(num);
     SemInit(ALL_SEMS,1);
}


void Sem_InitLocks(int num, lockset_t id)
{
    semaphoreID = id;
    num_sem_alloc = num;
}


void Sem_DeleteLocks(lockset_t id)
{
    union semun dummy;

    /* this is only to avoid compiler whinning about the unitialized variable*/
    dummy.val=0; 

    (void) semctl(id,0,IPC_RMID,dummy);
}


