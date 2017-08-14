/* mpf_pow_ui -- Compute b^e.

Copyright 1998, 1999, 2001, 2012 Free Software Foundation, Inc.

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

#include "gmp.h"
#include "gmp-impl.h"

void
mpf_pow_ui (mpf_ptr r, mpf_srcptr b, unsigned long int e)
{
  mpf_t b2;

  mpf_init2 (b2, mpf_get_prec (r));
  mpf_set (b2, b);

  if ((e & 1) != 0)
    mpf_set (r, b);
  else
    mpf_set_ui (r, 1);
  while (e >>= 1)
    {
      mpf_mul (b2, b2, b2);
      if ((e & 1) != 0)
	mpf_mul (r, r, b2);
    }

  mpf_clear (b2);
}
