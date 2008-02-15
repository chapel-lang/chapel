#if defined(LINUX) || defined(CYGWIN) || defined(BGML)

#if defined(PPC) && !defined(XLCLINUX) || defined(BGML)
#include "tas-ppc.h"
#define SPINLOCK  
#define TESTANDSET(x) (! __compare_and_swap((long int *)(x),0,1)) 
#endif

#if defined(__i386__) || defined(__alpha) || defined(__ia64) || defined(__x86_64__)
#  define SPINLOCK 
#  if defined(__GNUC__) && !defined(__INTEL_COMPILER)
#     if defined(__i386__) || defined(__x86_64__) 
#          include "tas-i386.h"
#     elif  defined(__ia64)
#          include "tas-ia64.h"
#     else
#          include "tas-alpha.h"
#     endif
#     define TESTANDSET testandset
#  else
#     define TESTANDSET gcc_testandset
#     define RELEASE_SPINLOCK gcc_clear_spinlock
#  endif
   extern int gcc_testandset();
   extern void gcc_clear_spinlock();
#endif

#elif defined(DECOSF)
#include "tas-alpha.h"
#define SPINLOCK 
#define TESTANDSET testandset

#elif defined(SGI)
#include <mutex.h>
#define SPINLOCK 
#define TESTANDSET(x) __lock_test_and_set((x), 1)
#define RELEASE_SPINLOCK __lock_release

#elif defined(AIX)
#include <sys/atomic_op.h>
#define SPINLOCK 
#define TESTANDSET(x) (_check_lock((x), 0, 1)==TRUE) 
#define RELEASE_SPINLOCK(x) _clear_lock((x),0) 

#elif defined(SOLARIS)
#define SPINLOCK  
#define TESTANDSET(x) (!_lock_try((x))) 
#define RELEASE_SPINLOCK _lock_clear 

#elif defined(MACX)
#  define SPINLOCK  
#  if defined(__GNUC__)
#    if defined(__i386__)
#      include "tas-i386.h"
#      define TESTANDSET testandset
#    else
#      include "tas-ppc.h"
#      define TESTANDSET(x) (krspin_lock((long int *)(x)))
#    endif
#  else
#    define TESTANDSET gcc_testandset
#    define RELEASE_SPINLOCK gcc_clear_spinlock
     extern int gcc_testandset();
     extern void gcc_clear_spinlock();
#  endif

#elif defined(HPUX__)
extern int _acquire_lock();
extern void _release_lock();
#define SPINLOCK  
#define TESTANDSET(x) (!_acquire_lock((x))) 
#define RELEASE_SPINLOCK _release_lock 

#elif defined(HPUX) && defined(__ia64) /* HPUX on IA64, non gcc */
#  define SPINLOCK
#  define USE_GCC_SPINLOCK 0 /*If gcc available, define USE_GCC_SPINLOCK=1 */
#  if USE_GCC_SPINLOCK || defined(__GNUC__)
#    define TESTANDSET gcc_testandset
#    define RELEASE_SPINLOCK gcc_clear_spinlock
     extern int gcc_testandset();
     extern void gcc_clear_spinlock();
#  else 
     typedef unsigned int slock_t;
#    include <ia64/sys/inline.h>
#    define TESTANDSET(lock) _Asm_xchg(_SZ_W, lock, 1, _LDHINT_NONE)
#    define RELEASE_SPINLOCK(lock) (*((volatile LOCK_T *) (lock)) = 0)
#  endif

#elif defined(NEC)
extern ullong ts1am_2me();
#define LOCK_T ullong
#define _LKWD (1ULL << 63)
#define SPINLOCK  
#define TESTANDSET(x) ((_LKWD & ts1am_2me(_LKWD, 0xffULL, (ullong)(x))))
#define MEMORY_BARRIER mpisx_clear_cache 
extern void mpisx_clear_cache();
#define RELEASE_SPINLOCK(x) ts1am_2me(0ULL, 0xffULL, (ullong)x); 

#endif



#ifdef SPINLOCK

#include <stdio.h>
#include <unistd.h>

#ifndef DBL_PAD
#   define DBL_PAD 16
#endif

/* make sure that locks are not sharing the same cache line */
typedef struct{
double  lock[DBL_PAD];
}pad_lock_t;

#ifndef LOCK_T
#define LOCK_T int
#endif
#define PAD_LOCK_T pad_lock_t

/* we got problems on IA64/Linux64 with Elan if inlining is used */
#if defined(__GNUC__) && !defined(QUADRICS)
#   define INLINE inline 
#else
#   define INLINE 
#endif


static INLINE void armci_init_spinlock(LOCK_T *mutex)
{
  *mutex =0;
}

static INLINE void armci_acquire_spinlock(LOCK_T *mutex)
{
#ifdef BGML
   return;
#else
int loop=0, maxloop =10;

   while (TESTANDSET(mutex)){
      loop++;
      if(loop==maxloop){ 
#if 0
         extern int armci_me;
         printf("%d:spinlock sleeping\n",armci_me); fflush(stdout);
#endif
         usleep(1);
         loop=0;
      }
  }
#endif
}



#ifdef  RELEASE_SPINLOCK
#ifdef MEMORY_BARRIER
#  define  armci_release_spinlock(x) MEMORY_BARRIER ();RELEASE_SPINLOCK(x)
#else
#  define  armci_release_spinlock(x) RELEASE_SPINLOCK(x)
#endif
#else
static INLINE void armci_release_spinlock(LOCK_T *mutex)
{
#ifdef BGML
   return;
#else
#ifdef MEMORY_BARRIER
  MEMORY_BARRIER ();
#endif
  *mutex =0;
#if (defined(MACX)||defined(LINUX)) && defined(__GNUC__) && defined(__ppc__)  
  __asm__ __volatile__ ("isync" : : : "memory");
#endif
#endif
}

#endif

#endif
