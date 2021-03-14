/* mpn_set_str_basecase -- mpn_set_str forced to its basecase.

Copyright 2002 Free Software Foundation, Inc.

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

#define __gmpn_set_str mpn_set_str_basecase
#define __gmpn_bc_set_str mpn_bc_set_str_basecase
#define __gmpn_dc_set_str mpn_dc_set_str_basecase

#include "gmp-impl.h"

#ifndef SIZE_T_MAX
#define SIZE_T_MAX  ((size_t) ULONG_MAX)
#endif

#undef SET_STR_DC_THRESHOLD
#define SET_STR_DC_THRESHOLD           SIZE_T_MAX /* always */
#undef SET_STR_PRECOMPUTE_THRESHOLD
#define SET_STR_PRECOMPUTE_THRESHOLD   SIZE_T_MAX /* always */

#include "mpn/generic/set_str.c"
