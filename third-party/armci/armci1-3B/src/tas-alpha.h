/* Machine-dependent pthreads configuration and inline functions.
   Alpha version.
Copyright (C) 1996 Free Software Foundation, Inc.
This file is part of the GNU C Library.
Contributed by Richard Henderson <rth@tamu.edu>.

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
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.  */


#ifdef __GNUC__

/************* gcc ******************/

/* Spinlock implementation; required.  */
static inline long testandset(int *spinlock)
{
  long ret, temp;

  asm volatile(
        "1:\t"
        "ldl_l %0,%3\n\t"
        "bne %0,2f\n\t"
        "or $31,1,%1\n\t"
        "stl_c %1,%2\n\t"
        "beq %1,1b\n"
        "2:\tmb\n"
        : "=&r"(ret), "=&r"(temp), "=m"(*spinlock)
        : "m"(*spinlock)
        : "memory");

  return ret;
}

/* Spinlock release; default is just set to zero.  */
#define RELEASE_SPINLOCK(spinlock) \
  asm volatile("mb" : : : "memory"); \
  *spinlock = 0

#else

/************* Compaq compiler ******************/

#include <c_asm.h>

/* Spinlock implementation; required.  */
static long testandset(int *spinlock)
{
  long ret, temp = 0;

  ret = asm ("1:\
	         ldl_l %v0,(%a1);\
	    	 bne %v0 ,2f;\
	    	 or $31,1,%a0;\
	    	 stl_c %a0,(%a1);\
	    	 beq %a0,1b;\
	      2:\
   	         mb;"
	    ,temp, spinlock );

  return ret;
}

/* Spinlock release; default is just set to zero.  */
#define RELEASE_SPINLOCK(spinlock) \
  asm ("mb"); \
  *spinlock = 0

#endif
