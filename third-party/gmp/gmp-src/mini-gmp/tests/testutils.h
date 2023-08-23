/*

Copyright 2013, 2014, 2018, Free Software Foundation, Inc.

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

#include "mini-random.h"

#define numberof(x)  (sizeof (x) / sizeof ((x)[0]))

void testmain (int argc, char **argv);

void testhalves (int count, void (*tested_fun) (int));

void testfree (void *p, size_t size);

void
dump (const char *label, const mpz_t x);

void
mpz_set_str_or_abort (mpz_ptr z, const char *str, int base);

/* Prototype for wrappers to internal functions to be tested. */
int
mpz_lucas_mod (mpz_t V, mpz_t Qk, long Q,
	       mp_bitcnt_t b0, const mpz_t n);
