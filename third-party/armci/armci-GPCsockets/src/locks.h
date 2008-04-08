/* $Id: locks.h,v 1.25.4.6 2007-08-03 19:38:22 manoj Exp $ */
#ifndef _ARMCI_LOCKS_H_
#define _ARMCI_LOCKS_H_
#include <sys/types.h>
#define MAX_LOCKS 1024
#define NUM_LOCKS MAX_LOCKS 

#ifndef EXTERN
#   define EXTERN extern
#endif
#ifdef QUADRICS
#include <elan/elan.h>
#endif

#if !defined(CYGNUS) && !defined(QUADRICS) && !defined(XT3) ||defined(ELAN_ACC)
#include "spinlock.h"
#endif

#if 0
/* code disabled until more tests of pthread locking is done */
#if !defined(SPINLOCK) && (defined(SGI) || defined(DECOSF)) && defined(PTHREADS)
/* check if Pthreads locks in Posix 1003.1c support processes*/
#include <pthread.h>
#include <unistd.h>
#ifdef _POSIX_THREAD_PROCESS_SHARED
#  define PMUTEXES
#endif
#endif
#endif


#if defined(PTHREADS) && !(defined(PMUTEXES) || defined(SPINLOCK)||defined(QUADRICS))
# if defined(LINUX) && defined(__sparc__) && defined(GM)
#    define PMUTEXES
#    include <pthread.h>
# else
     cannot run
# endif
#endif


#if (defined(SPINLOCK) || defined(PMUTEXES) || defined(HITACHI)) && !defined(BGML)
#  include "shmem.h"
   typedef struct {
     long off;
     long idlist[SHMIDLEN];
   }lockset_t;
   extern lockset_t lockid;
#elif defined(BGML)
   typedef int lockset_t;
#endif


#if defined(SPINLOCK) 

# ifdef SGIALTIX
/*
#  define NAT_LOCK(x,p) armci_acquire_spinlock((LOCK_T*)(((void**)_armci_int_mutexes)[]))
#  define NAT_UNLOCK(x,p) armci_release_spinlock((LOCK_T*)(((void**)_armci_int_mutexes)[x]))
*/
#  define NAT_LOCK(x,p) \
    armci_acquire_spinlock((LOCK_T*)( ((PAD_LOCK_T*)(((void**)_armci_int_mutexes)[p]))+x ))
#  define NAT_UNLOCK(x,p) \
    armci_release_spinlock((LOCK_T*)( ((PAD_LOCK_T*)(((void**)_armci_int_mutexes)[p]))+x ))
   EXTERN PAD_LOCK_T *_armci_int_mutexes;
# else
#  define NAT_LOCK(x,p) armci_acquire_spinlock((LOCK_T*)(_armci_int_mutexes+(x)))
#  define NAT_UNLOCK(x,p) armci_release_spinlock((LOCK_T*)(_armci_int_mutexes+(x)))
   EXTERN PAD_LOCK_T *_armci_int_mutexes;
# endif

#elif defined(PMUTEXES)

#  define NAT_LOCK(x,p) pthread_mutex_lock(_armci_int_mutexes +x)
#  define NAT_UNLOCK(x,p) pthread_mutex_unlock(_armci_int_mutexes +x)
#  define LOCK_T pthread_mutex_t
#  define PAD_LOCK_T LOCK_T
   EXTERN PAD_LOCK_T *_armci_int_mutexes;

#elif defined(HITACHI)

   extern void armcill_lock(int mutex, int proc);
   extern void armcill_unlock(int mutex, int proc);

#  define LOCK_T int
#  define PAD_LOCK_T LOCK_T
#  define NAT_LOCK(x,p) armcill_lock((x),(p))
#  define NAT_UNLOCK(x,p) armcill_unlock((x),(p))
   EXTERN PAD_LOCK_T *_armci_int_mutexes;

#elif defined(SGI)

#  define SGI_SPINS 100
#  include <ulocks.h>

   typedef struct {
        int id;
        ulock_t * lock_array[NUM_LOCKS];
   }lockset_t;

   extern lockset_t lockset;
#  define NAT_LOCK(x,p)    (void) uswsetlock(lockset.lock_array[(x)],SGI_SPINS)
#  define NAT_UNLOCK(x,p)  (void) usunsetlock(lockset.lock_array[(x)])

#elif defined(CONVEX)

#  include <sys/cnx_ail.h>
   typedef struct{
        unsigned state;
        unsigned pad[15];
   } lock_t;

   typedef int lockset_t;
   extern lock_t *lock_array;
   extern void setlock(unsigned * volatile lp);
   extern void unsetlock(unsigned  * volatile lp);
#  define NAT_LOCK(x,p)    (void) setlock(&lock_array[x].state)
#  define NAT_UNLOCK(x,p)  (void) unsetlock(&lock_array[(x)].state)

#elif defined(WIN32)

   typedef int lockset_t;
   extern void setlock(int);
   extern void unsetlock(int);
#  define NAT_LOCK(x,p)   setlock(x)
#  define NAT_UNLOCK(x,p)  unsetlock(x)

#elif defined(CRAY_YMP) && !defined(__crayx1)
#  include <tfork.h>

    typedef int lockset_t;
    extern  lock_t cri_l[NUM_LOCKS];
#  pragma  _CRI common cri_l

#  define NAT_LOCK(x,p)   t_lock(cri_l+(x))
#  define NAT_UNLOCK(x,p) t_unlock(cri_l+(x))

#elif defined(QUADRICS) && defined(_ELAN_LOCK_H)

extern void armcill_allocate_locks(int);
extern void armcill_lock(int m, int proc);
extern void armcill_unlock(int m, int proc);
#  undef NUM_LOCKS
#  define NUM_LOCKS 4
#  define NAT_LOCK armcill_lock
#  define NAT_UNLOCK armcill_unlock
   typedef int lockset_t;


#elif defined(CRAY_T3E) || defined(QUADRICS) || defined(__crayx1)\
        || defined(CATAMOUNT) || defined(CRAY_SHMEM) || defined(PORTALS)
#  include <limits.h>
#  if defined(CRAY) || defined(XT3)
#    include <mpp/shmem.h>
#  endif
#if defined(DECOSF) || defined(LINUX64) || defined(__crayx1)\
        || defined(CATAMOUNT)
#  define  _INT_MIN_64 (LONG_MAX-1)
#endif
#  undef NUM_LOCKS
#  define NUM_LOCKS 4
   static long armci_lock_var[4]={0,0,0,0};
   typedef int lockset_t;
#  define INVALID (long)(_INT_MIN_64 +1)
#  define NAT_LOCK(x,p) while( shmem_swap(armci_lock_var+(x),INVALID,(p)) )
#  define NAT_UNLOCK(x,p) shmem_swap(armci_lock_var+(x), 0, (p))


#elif  defined(SYSV) && defined(LAPI) && defined(AIX)

int **_armci_int_mutexes;
#  define NAT_LOCK(x,p)  armci_lapi_lock(_armci_int_mutexes[armci_master]+(x))
#  define NAT_UNLOCK(x,p)  armci_lapi_unlock(_armci_int_mutexes[armci_master]+(x))
   typedef int lockset_t;

#elif defined(CYGNUS)

   typedef int lockset_t;
#  define NAT_LOCK(x,p) armci_die("does not run in parallel",0) 
#  define NAT_UNLOCK(x,p) armci_die("does not run in parallel",0)  

#elif defined(LAPI) && !defined (LINUX)

#  include <pthread.h>
   typedef int lockset_t;
   extern pthread_mutex_t _armci_mutex_thread;
#  define NAT_LOCK(x,p)   pthread_mutex_lock(&_armci_mutex_thread)
#  define NAT_UNLOCK(x,p) pthread_mutex_unlock(&_armci_mutex_thread)


#elif defined(FUJITSU)
   typedef int lockset_t;
#  include "fujitsu-vpp.h"

#elif defined(SYSV) ||defined(MACX)

#  include "semaphores.h"
#  undef NUM_LOCKS
#  define NUM_LOCKS ((MAX_LOCKS< SEMMSL) ? MAX_LOCKS:SEMMSL)

#  define NAT_LOCK(x,p)   P_semaphore(x)
#  define NAT_UNLOCK(x,p)  V_semaphore(x)

#ifndef _LOCKS_C_
#define CreateInitLocks Sem_CreateInitLocks
#define InitLocks Sem_InitLocks
#define DeleteLocks Sem_DeleteLocks
#endif

#else

   typedef int lockset_t;
#  define NAT_LOCK(x,p)  crap
#  define NAT_UNLOCK(x,p) crap2

#endif

extern void CreateInitLocks(int num, lockset_t *id);
extern void InitLocks(int num , lockset_t id);
extern void DeleteLocks(lockset_t id);

#ifdef FUJITSU
#define NATIVE_LOCK(x,p) if(armci_nproc>1) { NAT_LOCK(p); }
#define NATIVE_UNLOCK(x,p) if(armci_nproc>1) { NAT_UNLOCK(p); }
#else
#define NATIVE_LOCK(x,p) if(armci_nproc>1) { NAT_LOCK(x,p); }
#define NATIVE_UNLOCK(x,p) if(armci_nproc>1) { NAT_UNLOCK(x,p); }
#endif

#endif
