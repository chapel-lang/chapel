#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/* how many semaphores are available ? */
#ifndef SEMMSL
#   ifdef AIX
#         define SEMMSL 8094
#   else
#         define SEMMSL 16
#   endif
#endif

/* on HPUX 10.2 SEMMSL is much bigger than realistically we can allocate */  
#ifdef HPUX
#undef SEMMSL
#define SEMMSL 64
#endif

extern struct sembuf sops;
extern int semaphoreID;
int semop();
#define ALL_SEMS -1

#define _P_code      -1
#define _V_code       1
#define P_semaphore(s)  \
{\
  sops.sem_num = (s);\
  sops.sem_op  =  _P_code;\
  sops.sem_flg =  0; \
  semop(semaphoreID,&sops,1);\
}
#define V_semaphore(s) \
{\
  sops.sem_num = (s);\
  sops.sem_op  =  _V_code;\
  sops.sem_flg =  0; \
  semop(semaphoreID,&sops,1);\
}

typedef int lockset_t;
