/* Machine-dependent pthreads configuration and inline functions.
   powerpc version.
   Copyright (C) 1996, 1997, 1998, 2000, 2001 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If
   not, write to the Free Software Foundation, Inc.,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

/* These routines are from Appendix G of the 'PowerPC 601 RISC Microprocessor
   User's Manual', by IBM and Motorola.  */

#ifndef PT_EI
# define PT_EI extern inline
#endif

/* For multiprocessor systems, we want to ensure all memory accesses
   are completed before we reset a lock.  On other systems, we still
   need to make sure that the compiler has flushed everything to memory.  */
#define MEMORY_BARRIER() __asm__ __volatile__ ("sync" : : : "memory")

/* Compare-and-swap for semaphores. */
/* note that test-and-set(x) is the same as !compare-and-swap(x, 0, 1) */

#define HAS_COMPARE_AND_SWAP_WITH_RELEASE_SEMANTICS
#define IMPLEMENT_TAS_WITH_CAS

PT_EI int
__compare_and_swap (long int *p, long int oldval, long int newval)
{
  int ret;

  __asm__ __volatile__ (
           "0:    lwarx %0,0,%1 ;"
           "      xor. %0,%3,%0;"
           "      bne 1f;"
           "      stwcx. %2,0,%1;"
           "      bne- 0b;"
           "1:    "
        : "=&r"(ret)
        : "r"(p), "r"(newval), "r"(oldval)
        : "cr0", "memory");
  /* This version of __compare_and_swap is to be used when acquiring
     a lock, so we don't need to worry about whether other memory
     operations have completed, but we do need to be sure that any loads
     after this point really occur after we have acquired the lock.  */
  __asm__ __volatile__ ("isync" : : : "memory");
  return ret == 0;
}

PT_EI int
__compare_and_swap_with_release_semantics (long int *p,
                                           long int oldval, long int newval)
{
  int ret;

  MEMORY_BARRIER ();
  __asm__ __volatile__ (
           "0:    lwarx %0,0,%1 ;"
           "      xor. %0,%3,%0;"
           "      bne 1f;"
           "      stwcx. %2,0,%1;"
           "      bne- 0b;"
           "1:    "
        : "=&r"(ret)
        : "r"(p), "r"(newval), "r"(oldval)
        : "cr0", "memory");
  return ret == 0;
}

static int  krspin_lock(long int *p)
{
        unsigned long tmp;
        int ret;
        __asm__ __volatile__(
        "b      1f              # spin_lock\n\
2:      lwzx    %0,0,%1\n\
        cmpwi   0,%0,0\n\
        bne+    2b\n\
1:      lwarx   %0,0,%1\n\
        cmpwi   0,%0,0\n\
        bne-    2b\n"
"       stwcx.  %2,0,%1\n\
        bne-    2b\n\
        isync"
        : "=&r"(tmp)
        : "r"(p), "r"(1)
        : "cr0", "memory");
  return ret == 0;
}

