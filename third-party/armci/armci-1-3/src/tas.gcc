#ifdef __alpha
#   include "tas-alpha.h"
#elif defined(__i386__) || defined(__x86_64__)
#   include "tas-i386.h"
#elif defined(__ppc__)
#   define PT_EI inline
#   include "tas-ppc.h"
#   define testandset(x) (! __compare_and_swap((long int *)(x),0,1))
#elif defined(__ia64)
#   include "tas-ia64.h"

  void _armci_ia64_mb()
  {
     __asm__ __volatile__ ("mf" ::: "memory");
  }
#else

  int testandset(int *spinlock)
  {
    printf("ARMCI NEEDS TESTANDSET FOR THIS PLATFORM\n");
    armci_die("cannot run",0);
  }

#endif

int gcc_testandset(int *spinlock)
{
    int rc=testandset(spinlock);
    return rc;
}

void gcc_clear_spinlock(int *spinlock)
{
#ifdef  RELEASE_SPINLOCK
  RELEASE_SPINLOCK(spinlock);
#else
  *spinlock =0;
#endif
}
