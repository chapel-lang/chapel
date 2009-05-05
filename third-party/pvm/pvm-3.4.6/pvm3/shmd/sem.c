
static char rcsid[] =
	"$Id: sem.c,v 1.3 2001/09/25 17:07:33 pvmsrc Exp $";

/*
Semaphore control code for Binary Semaphores only

GEF UTK/ORNL 
20 Jan 1997
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <unistd.h>	/* needed for usleep */
#include <stdio.h>

#ifdef XXX_IMA_LINUX
/* Seems to be O.K. on latest Linux systems now. JAK - 9/25/01 */
union semun forth_arg;		/* XXXX optional under all OSs except Linux ! */
#endif


void unlock_sem_forced ();

int get_sem (key_t k, int n)
{
	int semid;

	for(;(semid=semget(k, n, 0)) == -1;) usleep (1000);
	return semid;
}


void lock_sem (int sem, int n)
{
	struct sembuf sop;	/* 1 element array */
	sop.sem_num = n;		/* which sem are we operating on */
	sop.sem_op  = -1;		/* -1 locks */
	sop.sem_flg = SEM_UNDO; /* if I crash reset the semaphore */
							/* this is a very important flag to set! */

	semop (sem, &sop, 1);	/* block until resource is free, then lock it */
}

int sem_is (int sem, int n)
{
#ifdef XXX_IMA_LINUX
	/* Seems to be O.K. on latest Linux systems now. JAK - 9/25/01 */
	return (semctl( sem, n, GETVAL, forth_arg));
#else
	return (semctl( sem, n, GETVAL));
#endif
}

void unlock_sem (int sem, int n)
{
	struct sembuf sop;	/* 1 element array */
	sop.sem_num = n;		/* which sem are we operating on */
	sop.sem_op  = 1;		/* +1 locks */
	sop.sem_flg = SEM_UNDO;

	semop (sem, &sop, 1);	/* unblock resource */
}

void unlock_sem_forced (int sem, int n)
{
	struct sembuf sop;	/* 1 element array */
	sop.sem_num = n;		/* which sem are we operating on */
	sop.sem_op  = 1;		/* +1 locks */
	sop.sem_flg = 0;		/* flag =0 NOT SEM_UNDO */

	semop (sem, &sop, 1);	/* unblock resource */
}


int init_sem (key_t k, int n)
{
	int semid, i;

	if((semid=semget (k, n, 0)) !=-1) {
		printf("[init_sem] sem key %lx exists with id of %d\n",
			(long) k, semid);
#ifdef XXX_IMA_LINUX
		/* Seems to be O.K. on latest Linux systems now. JAK - 9/25/01 */
		semctl (semid, 0, IPC_RMID, forth_arg);		/* existing sem destroy */
#else
		semctl (semid, 0, IPC_RMID);		/* existing sem destroy */
#endif
	}
	if((semid=semget (k, n, IPC_CREAT | 0600)) !=-1) {
		/* semaphore are init to zero, i.e. locked so we unlock them */
/* 		for(i=0;i<n;i++) unlock_sem (semid, i); */
		/* as we have SEM_UNDO in unlock_sem, this causes problems when */
		/* we reset, so... */
		/* we use an explicit sem set, hoping of course than nobody has */
		/* attempted to use the semaphore already */
		for(i=0;i<n;i++) unlock_sem_forced (semid, i);
	}

	printf("[init_sem] sem key %lx is now id %d\n", (long) k, semid);
	return semid;
}

void del_sem (int semid)
{

#ifdef XXX_IMA_LINUX
	/* Seems to be O.K. on latest Linux systems now. JAK - 9/25/01 */
	semctl (semid, 0, IPC_RMID, forth_arg);		/* existing sem destroy */
#else
	semctl (semid, 0, IPC_RMID);		/* existing sem destroy */
#endif
}

