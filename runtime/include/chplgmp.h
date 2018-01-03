/*
 * Copyright 2004-2018 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "chpltypes.h"

#include <stdarg.h>
#include <stdio.h>

#ifdef CHPL_HAS_GMP
#include <gmp.h>

// set the GMP allocation routines to use chpl_malloc, etc.
void chpl_gmp_init(void);

void chpl_gmp_get_mpz(mpz_t ret, int64_t src_locale, __mpz_struct from);

void chpl_gmp_get_randstate(gmp_randstate_t        not_inited_state,
                            int64_t                src_locale,
                            __gmp_randstate_struct from);

uint64_t chpl_gmp_mpz_nlimbs(__mpz_struct from);

void chpl_gmp_mpz_print(mpz_t x);

c_string chpl_gmp_mpz_get_str(int base, mpz_t x);

// These functions wrap the equivalent GMP macros to support LLVM backend
int chpl_mpz_cmp_si(const mpz_t op1, long op2);
int chpl_mpz_cmp_ui(const mpz_t op1, unsigned long op2);

int chpl_mpz_sgn(const mpz_t op);

int chpl_mpz_odd_p(const mpz_t op);
int chpl_mpz_even_p(const mpz_t op);

#endif

