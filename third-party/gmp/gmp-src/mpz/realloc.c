/* _mpz_realloc -- make the mpz_t have NEW_ALLOC digits allocated.

Copyright 1991, 1993-1995, 2000, 2001, 2008, 2015, 2021, 2022 Free
Software Foundation, Inc.

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

void *
_mpz_realloc (mpz_ptr m, mp_size_t new_alloc)
{
  mp_ptr mp;

  /* Never allocate zero space. */
  new_alloc = MAX (new_alloc, 1);

  if (sizeof (mp_size_t) == sizeof (int))
    {
      if (UNLIKELY (new_alloc > ULONG_MAX / GMP_NUMB_BITS))
	MPZ_OVERFLOW;
    }
  else
    {
      if (UNLIKELY (new_alloc > INT_MAX))
	MPZ_OVERFLOW;
    }

  if (ALLOC (m) == 0)
    {
      mp = __GMP_ALLOCATE_FUNC_LIMBS (new_alloc);
    }
  else
    {
      mp = __GMP_REALLOCATE_FUNC_LIMBS (PTR (m), ALLOC (m), new_alloc);

      /* Don't create an invalid number; if the current value doesn't fit after
	 reallocation, clear it to 0.  */
      if (UNLIKELY (ABSIZ (m) > new_alloc))
	SIZ (m) = 0;
    }

  PTR (m) = mp;
  ALLOC(m) = new_alloc;
  return (void *) mp;
}
