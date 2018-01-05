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

#ifdef CHPL_HAS_GMP

#include "chplrt.h"

#include "chpl-comm.h"
#include "chpl-mem.h"

#include "chplgmp.h"

#include <stdio.h>
#include <stdarg.h>
#include <gmp.h>

#include "chpl-comm-compiler-macros.h"
#include "chpl-comm.h"

static void* chpl_gmp_alloc(size_t sz) {
  return chpl_mem_alloc(sz, CHPL_RT_MD_GMP, 0, 0);
}

static void* chpl_gmp_realloc(void* ptr, size_t old_size, size_t new_size) {
  return chpl_mem_realloc( ptr, new_size, CHPL_RT_MD_GMP, 0, 0);
}

static void chpl_gmp_free(void* ptr, size_t old_size) {
  chpl_mem_free( ptr, 0, 0);
}

void chpl_gmp_init(void) {
  mp_set_memory_functions(chpl_gmp_alloc, chpl_gmp_realloc, chpl_gmp_free);
}

void chpl_gmp_get_mpz(mpz_t ret, int64_t src_locale, __mpz_struct from) {
  // First, resize our destination appropriately.
  mpz_realloc2(ret, from._mp_alloc * mp_bits_per_limb);

  // Copy the _alloc and _size fields.
  ret[0]._mp_alloc = from._mp_alloc;
  ret[0]._mp_size  = from._mp_size;

  // Next, use GASNET to move the pointer data.
  chpl_gen_comm_get(ret[0]._mp_d,
                    src_locale,
                    from._mp_d,
                    sizeof(mp_limb_t) * ret[0]._mp_alloc,
                    CHPL_TYPE_uint64_t,
                    CHPL_COMM_UNKNOWN_ID,
                    0,
                    0);
}

void chpl_gmp_get_randstate(gmp_randstate_t        not_inited_state,
                            int64_t                src_locale,
                            __gmp_randstate_struct from) {
  // Copy the rand state..
  not_inited_state[0] = from;

  // Clear the seed since it's going to be a local mpz.
  memset(& not_inited_state[0]._mp_seed, 0, sizeof(mpz_t));
  mpz_init(not_inited_state[0]._mp_seed);

  chpl_gmp_get_mpz(not_inited_state[0]._mp_seed, src_locale, from._mp_seed[0]);
}

uint64_t chpl_gmp_mpz_nlimbs(__mpz_struct from) {
  return __GMP_ABS ( from._mp_size );
}

void chpl_gmp_mpz_print(mpz_t x) {
  printf("&x=%p\n", x);
  printf("x->_mp_d=%p\n", x[0]._mp_d);
  gmp_printf("x=%Zd\n", x);
}

c_string chpl_gmp_mpz_get_str(int base, mpz_t x) {
  // The number of *digits* in abs(x);
  size_t numDigits = mpz_sizeinbase(x, base);
  char*  str       = (char*) chpl_mem_calloc(1,
                                             numDigits + 2,
                                             CHPL_RT_MD_GLOM_STRINGS_DATA,
                                             0,
                                             0);

  mpz_get_str(str, base, x);

  return str;
}

//
// These functions wrap the equivalent GMP macros to support LLVM backend
//
int chpl_mpz_cmp_si(const mpz_t op1, long op2) {
  return mpz_cmp_si(op1, op2);
}

int chpl_mpz_cmp_ui(const mpz_t op1, unsigned long op2) {
  return mpz_cmp_ui(op1, op2);
}

int chpl_mpz_sgn(const mpz_t op) {
  return mpz_sgn(op);
}

int chpl_mpz_odd_p(const mpz_t op) {
  return mpz_odd_p(op);
}

int chpl_mpz_even_p(const mpz_t op) {
  return mpz_even_p(op);
}


#endif
