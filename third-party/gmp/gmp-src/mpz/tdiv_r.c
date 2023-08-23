/* mpz_tdiv_r(rem, dividend, divisor) -- Set REM to DIVIDEND mod DIVISOR.

Copyright 1991, 1993, 1994, 2000, 2001, 2005, 2012, 2021 Free Software
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
#include "longlong.h"

void
mpz_tdiv_r (mpz_ptr rem, mpz_srcptr num, mpz_srcptr den)
{
  mp_size_t ql, n0;
  mp_size_t ns, nl, dl;
  mp_ptr np, dp, qp, rp;
  TMP_DECL;

  ns = SIZ (num);
  nl = ABS (ns);
  dl = ABSIZ (den);
  ql = nl - dl + 1;

  if (UNLIKELY (dl == 0))
    DIVIDE_BY_ZERO;

  if (ql <= 0)
    {
      if (num != rem)
	{
	  SIZ (rem) = ns;
	  rp = MPZ_NEWALLOC (rem, nl);
	  np = PTR (num);
	  MPN_COPY (rp, np, nl);
	}
      return;
    }

  rp = MPZ_REALLOC (rem, dl);

  TMP_MARK;
  qp = TMP_ALLOC_LIMBS (ql);
  np = PTR (num);
  dp = PTR (den);

  /* FIXME: We should think about how to handle the temporary allocation.
     Perhaps mpn_tdiv_qr should handle it, since it anyway often needs to
     allocate temp space.  */

  /* Copy denominator to temporary space if it overlaps with the remainder.  */
  if (dp == rp)
    {
      mp_ptr tp;
      tp = TMP_ALLOC_LIMBS (dl);
      MPN_COPY (tp, dp, dl);
      dp = tp;
    }
  /* Copy numerator to temporary space if it overlaps with the remainder.  */
  if (np == rp)
    {
      mp_ptr tp;
      tp = TMP_ALLOC_LIMBS (nl);
      MPN_COPY (tp, np, nl);
      np = tp;
    }

  for (n0 = 0; *dp == 0; ++dp)
    {
      rp [n0++] = *np++;
      --nl;
    }
  mpn_tdiv_qr (qp, rp + n0, 0L, np, nl, dp, dl - n0);

  MPN_NORMALIZE (rp, dl);

  SIZ (rem) = ns >= 0 ? dl : -dl;
  TMP_FREE;
}
