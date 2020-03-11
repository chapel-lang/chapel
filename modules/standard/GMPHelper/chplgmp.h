/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef _chplgmp_h_
#define _chplgmp_h_

#include "chpltypes.h"
#include <stdio.h>
#include <stdarg.h>
#include <gmp.h>

#include "chpl-comm-compiler-macros.h"
#include "chpl-comm.h"

// Need this workaround for some compilers until the Chapel
// compiler moves away from representing all C fn pointers as void*
static inline
void chpl_gmp_mp_set_memory_functions(c_fn_ptr alloc,
                                      c_fn_ptr realloc,
                                      c_fn_ptr free) {
  mp_set_memory_functions((void *(*) (size_t)) alloc,
                          (void *(*) (void *, size_t, size_t)) realloc,
                          (void (*) (void *, size_t)) free);
}

static inline
mp_size_t chpl_gmp_mpz_struct_nalloc(__mpz_struct from) {
  return from._mp_alloc;
}

static inline
mp_size_t chpl_gmp_mpz_struct_sign_size(__mpz_struct from) {
  return from._mp_size;
}

static inline
mp_limb_t* chpl_gmp_mpz_struct_limbs(__mpz_struct from) {
  // It's important that this function not dereference the limbs pointer
  // as it might be stored on another locale.
  return from._mp_d;
}

static inline
void chpl_gmp_mpz_set_sign_size(mpz_t dst, mp_size_t sign_size) {
  dst[0]._mp_size = sign_size;
}

// These functions exists so that we can transfer the state in a gmp_randstate_t
// across locales. GMP does not currently provide a public API interface to
// extract the state of a gmp_randstate_t random number generator,
// other than by accessing the _mp_seed field directly as we do here.
static inline
__mpz_struct chpl_gmp_randstate_read_state(__gmp_randstate_struct state) {
  return state._mp_seed[0];
}

static inline
void chpl_gmp_randstate_set_state(gmp_randstate_t state, mpz_t src) {
  state[0]._mp_seed[0]._mp_alloc = src[0]._mp_alloc;
  state[0]._mp_seed[0]._mp_size = src[0]._mp_size;
  state[0]._mp_seed[0]._mp_d = src[0]._mp_d;
}

static inline
int chpl_gmp_randstate_same_algorithm(gmp_randstate_t a,
                                      gmp_randstate_t b) {
  return a[0]._mp_algdata._mp_lc == b[0]._mp_algdata._mp_lc;
}

/*
static void chpl_gmp_mpz_print(mpz_t x) {
  printf("&x=%p\n", x);
  printf("x->_mp_d=%p\n", x[0]._mp_d);
  gmp_printf("x=%Zd\n", x);
}
*/

static
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
static inline int chpl_mpz_cmp_si(const mpz_t op1, long op2) {
  return mpz_cmp_si(op1, op2);
}

static inline int chpl_mpz_cmp_ui(const mpz_t op1, unsigned long op2) {
  return mpz_cmp_ui(op1, op2);
}

static inline int chpl_mpz_sgn(const mpz_t op) {
  return mpz_sgn(op);
}

static inline int chpl_mpz_odd_p(const mpz_t op) {
  return mpz_odd_p(op);
}

static inline int chpl_mpz_even_p(const mpz_t op) {
  return mpz_even_p(op);
}


#endif
