/* Exercise mpz_probab_prime_p.

Copyright 2002, 2018-2019, 2022 Free Software Foundation, Inc.

This file is part of the GNU MP Library test suite.

The GNU MP Library test suite is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

The GNU MP Library test suite is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along with
the GNU MP Library test suite.  If not, see https://www.gnu.org/licenses/.  */

#include <stdio.h>
#include <stdlib.h>
#include "gmp-impl.h"
#include "tests.h"


/* Enhancements:

   - Test some big primes don't come back claimed to be composite.
   - Test some big composites don't come back claimed to be certainly prime.
   - Test some big composites with small factors are identified as certainly
     composite.  */


/* return 2 if prime, 0 if composite */
int
isprime (unsigned long n)
{
  if (n < 4)
    return (n & 2);
  if ((n & 1) == 0)
    return 0;

  for (unsigned long i = 3; i*i <= n; i+=2)
    if ((n % i) == 0)
      return 0;

  return 2;
}

void
check_one (mpz_srcptr n, int want)
{
  int  got;

  got = mpz_probab_prime_p (n, 25);

  /* "definitely prime" (2) is fine if we only wanted "probably prime" (1) */
  if ((got != want) && (got != want * 2))
    {
      printf ("mpz_probab_prime_p\n");
      mpz_trace ("  n    ", n);
      printf    ("  got =%d", got);
      printf    ("  want=%d", want);
      abort ();
    }
}

void
check_pn (mpz_ptr n, int want)
{
  check_one (n, want);
  mpz_neg (n, n);
  check_one (n, want);
}

/* expect certainty for small n */
void
check_small (void)
{
  mpz_t  n;
  long   i;

  mpz_init (n);

  for (i = 0; i < 300; i++)
    {
      mpz_set_si (n, i);
      check_pn (n, isprime (i));
    }

  mpz_clear (n);
}

void
check_composites (int count)
{
  int i;
  mpz_t a, b, n, bs;
  unsigned long size_range, size;
  gmp_randstate_ptr rands = RANDS;

  mpz_init (a);
  mpz_init (b);
  mpz_init (n);
  mpz_init (bs);

  static const char * const composites[] = {
    "225670644213750121",	/* n=61*C16, if D < 61, (n/D) = 1.	*/
    "2386342059899637841",	/* n=61*C17, if D < 61, (n/D) = 1.	*/
    "1194649",	/* A square, but strong base-2 pseudoprime,	*/
    "12327121",	/* another base-2 pseudoprime square.	*/
    "18446744066047760377",	/* Should trigger Fibonacci's test;	*/
    "10323769",			/* &3==1, Lucas' test with D=37;	*/
    "1397419",			/* &3==3, Lucas' test with D=43;	*/
    "11708069165918597341",	/* &3==1, Lucas' test with large D=107;	*/
    "395009109077493751",	/* &3==3, Lucas' test with large D=113.	*/
    NULL
  };

  for (i = 0; composites[i]; i++)
    {
      mpz_set_str_or_abort (n, composites[i], 0);
      check_one (n, 0);
    }

  for (i = 0; i < count; i++)
    {
      mpz_urandomb (bs, rands, 32);
      size_range = mpz_get_ui (bs) % 13 + 1; /* 0..8192 bit operands */

      mpz_urandomb (bs, rands, size_range);
      size = mpz_get_ui (bs);
      mpz_rrandomb (a, rands, size);

      mpz_urandomb (bs, rands, 32);
      size_range = mpz_get_ui (bs) % 13 + 1; /* 0..8192 bit operands */
      mpz_rrandomb (b, rands, size);

      /* Exclude trivial factors */
      if (mpz_cmp_ui (a, 1) == 0)
	mpz_set_ui (a, 2);
      if (mpz_cmp_ui (b, 1) == 0)
	mpz_set_ui (b, 2);

      mpz_mul (n, a, b);

      check_pn (n, 0);
    }
  mpz_clear (a);
  mpz_clear (b);
  mpz_clear (n);
  mpz_clear (bs);
}

static void
check_primes (void)
{
  static const char * const primes[] = {
    "2", "53", "1234567891",
    "2055693949", "1125899906842597", "16412292043871650369",
    "18446744075358702679",	/* Lucas' test with large D=107.	*/
    /* diffie-hellman-group1-sha1, also "Well known group 2" in RFC
       2412, 2^1024 - 2^960 - 1 + 2^64 * { [2^894 pi] + 129093 } */
    "0xFFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1"
    "29024E088A67CC74020BBEA63B139B22514A08798E3404DD"
    "EF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245"
    "E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED"
    "EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE65381"
    "FFFFFFFFFFFFFFFF",
    NULL
  };

  mpz_t n;
  int i;

  mpz_init (n);

  for (i = 0; primes[i]; i++)
    {
      mpz_set_str_or_abort (n, primes[i], 0);
      check_one (n, 1);
    }
  mpz_clear (n);
}

static void
check_fermat_mersenne (int count)
{
  int fermat_exponents [] = {1, 2, 4, 8, 16};
  int mersenne_exponents [] = {2, 3, 5, 7, 13, 17, 19, 31, 61, 89,
			       107, 127, 521, 607, 1279, 2203, 2281,
			       3217, 4253, 4423, 9689, 9941, 11213,
			       19937, 21701, 23209, 44497, 86243};
  mpz_t pp;
  int i, j, want;

  mpz_init (pp);
  count = MIN (110000, count);

  for (i=1; i<count; ++i)
    {
      mpz_set_ui (pp, 1);
      mpz_setbit (pp, i); /* 2^i + 1 */
      want = 0;
      for (j = 0; j < numberof (fermat_exponents); j++)
	if (fermat_exponents[j] == i)
	  {
	    /* Fermat's primes are small enough for a definite answer. */
	    want = 2;
	    break;
	  }
      check_one (pp, want);

      mpz_sub_ui (pp, pp, 2); /* 2^i - 1 */
      want = 0;
      for (j = 0; j < numberof (mersenne_exponents); j++)
	if (mersenne_exponents[j] == i)
	  {
	    want = 1 << (i < 50);
	    break;
	  }
      check_one (pp, want);
    }
  mpz_clear (pp);
}

int
main (int argc, char **argv)
{
  int count = 1000;

  TESTS_REPS (count, argv, argc);

  tests_start ();

  check_small ();
  check_fermat_mersenne (count >> 3);
  check_composites (count);
  check_primes ();

  tests_end ();
  exit (0);
}
