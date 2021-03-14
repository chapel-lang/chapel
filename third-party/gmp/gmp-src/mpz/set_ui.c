/* mpz_set_ui(integer, val) -- Assign INTEGER with a small value VAL.

Copyright 1991, 1993-1995, 2001, 2002, 2004, 2012, 2015 Free Software
Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of either:

  * the GNU Lesser General Public License as published by the Free
    Software Foundation; either version 3 of the License, or (at your
    option) any later version.

or

  * the GNU General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any
    later version.

or both in parallel, as here.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received copies of the GNU General Public License and the
GNU Lesser General Public License along with the GNU MP Library.  If not,
see https://www.gnu.org/licenses/.  */

#include "gmp-impl.h"

void
mpz_set_ui (mpz_ptr dest, unsigned long int val)
{
  mp_size_t size;

  MPZ_NEWALLOC (dest, 1)[0] = val & GMP_NUMB_MASK;
  size = val != 0;

#if BITS_PER_ULONG > GMP_NUMB_BITS  /* avoid warnings about shift amount */
  if (val > GMP_NUMB_MAX)
    {
      MPZ_REALLOC (dest, 2);
      PTR (dest)[1] = val >> GMP_NUMB_BITS;
      size = 2;
    }
#endif

  SIZ (dest) = size;
}
