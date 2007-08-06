/* Machine-dependent pthreads configuration and inline functions.
   IA-64 version.
   Copyright (C) 1999, 2000 Free Software Foundation, Inc.
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
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifdef __GNUC__

// JN: had to diable PT_EI as it was causing testandset not inline
// when entire library was compiled with gcc

#ifndef PT_EI
# define PT_EI extern inline
#endif

/* Make sure gcc doesn't try to be clever and move things around on
   us. We need to use _exactly_ the address the user gave us, not some
   alias that contains the same information.  */
#define __atomic_fool_gcc(x) (*(volatile struct { int a[100]; } *)x)

/* Spinlock implementation; required.  */
static inline long int
testandset (int *spinlock)
{
  long int ret;

  __asm__ __volatile__(
       "xchg4 %0=%1,%2"
       : "=r"(ret), "+m"(__atomic_fool_gcc (spinlock))
       : "r"(1)
       : "memory");

  return ret;
}
#else
/* for Intel compiler  ecc */
echo FIXME
#endif
