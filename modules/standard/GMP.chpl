/*
 * Copyright 2004-2016 Cray Inc.
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

/*
Support for GNU Multiple Precision Arithmetic

This module supports integration with the GMP library (the GNU Multiple
Precision Arithmetic Library. See the `GMP homepage <https://gmplib.org/>`_
for more information on this library.

This module is a prototype implementation of a standard GMP (GNU
Multiple Precision Arithmetic Library) module in Chapel.  It should be
considered incomplete in that (a) only a subset of the full GMP interface is
supported, and (b) the performance is currently lacking due to extraneous
copies in the Chapel code that have not yet been optimized away.  If there is
sufficient interest, this prototype can be expanded to support the full GMP
interface and performance.

This prototype GMP module has been used to implement a port of the standard GMP
Chudnovsky algorithm for computing pi to arbitrary digits.  If you are
interested in receiving a copy of this Chapel program, or simply in expressing
your support for GMP within Chapel, please contact us at chapel_info@cray.com.


Using the GMP Module
--------------------

Step 1:
  Build Chapel with GMP

  .. code-block:: sh

            # To use the already-installed GMP
            export CHPL_GMP=system
            # To use the distributed GMP
            export CHPL_GMP=gmp
            # From $CHPL_HOME
            make clean; make


Step 2:
  Have your Chapel program ``use`` the standard GMP module

  .. code-block:: chapel

            use GMP;   // put this statement in your Chapel program


Step 3:
  Start using the supported subset of GMP types and routines
  or the BigInt record (see below for a complete listing).


Calling GMP functions directly
------------------------------

The second option for Chapel programs using this module is to call the GMP
functions directly. For a full reference to GMP capabilities, please refer to
the `GMP website <https://gmplib.org>`_ and the
`GMP documentation <https://gmplib.org/manual/>`_.


At present, Chapel's GMP module supports the following GMP types:

  * :type:`mp_bitcnt_t`
  * :type:`mpf_t`
  * :type:`mpz_t`

And all :type:`mpz_t` GMP routines, as well as the following routines:

  * :proc:`gmp_fprintf()`
  * :proc:`gmp_printf()`
  * :proc:`mpf_add()`
  * :proc:`mpf_clear()`
  * :proc:`mpf_div_2exp()`
  * :proc:`mpf_get_d()`
  * :proc:`mpf_get_prec()`
  * :proc:`mpf_init()`
  * :proc:`mpf_mul()`
  * :proc:`mpf_mul_ui()`
  * :proc:`mpf_out_str()`
  * :proc:`mpf_set_d()`
  * :proc:`mpf_set_default_prec()`
  * :proc:`mpf_set_prec_raw()`
  * :proc:`mpf_set_z()`
  * :proc:`mpf_sub()`
  * :proc:`mpf_ui_div()`
  * :proc:`mpf_ui_sub()`

*/
module GMP {
  use SysBasic;
  use Error;

  pragma "no doc"
  extern type __mpf_struct;

  /*  The GMP ``mpf_t`` type */
  extern type mpf_t = 1*__mpf_struct;

  pragma "no doc"
  extern type __mpz_struct;

  /* The GMP ``mpz_t`` type */
  extern type mpz_t = 1*__mpz_struct; // can we use a struct and not a tuple?

  pragma "no doc"
  extern type __gmp_randstate_struct;

  /* The GMP ``gmp_randstate_t`` type */
  extern type gmp_randstate_t = 1*__gmp_randstate_struct;

  /* The GMP ``mp_bitcnt_t`` type */
  extern type mp_bitcnt_t = c_ulong;
  /* The GMP ``mp_size_t`` type */
  extern type mp_size_t = size_t;
  /* The GMP ``mp_limb_t`` type */
  extern type mp_limb_t=uint(64);
  /* The GMP ``mp_ptr`` type */
  extern type mp_ptr; // mp_limb_t *

  /* The GMP `mp_bits_per_limb`` constant */
  extern const mp_bits_per_limb: c_int;

  /* All these external functions are ref, which may
     seem surprising. They are that way because identity
     matters and they may get reallocated otherwise;
     ref is currently the only way to avoid that.

     Note that GMP defines e.g. gmp_randstate_t with
     typedef struct __gmp_randstate_struct gmp_randstate_t[1],

     and the natural way to wrap that in Chapel is with a 1-element
     tuple. These tuples would be passed by value to the extern
     routines if not for ref.
   */

  // Initializing random state
  extern proc gmp_randinit_default(ref STATE: gmp_randstate_t);
  extern proc gmp_randinit_mt(ref STATE: gmp_randstate_t);
  extern proc gmp_randinit_lc_2exp(ref STATE: gmp_randstate_t, ref A:mpz_t, C: c_ulong, M2EXP: c_ulong);
  extern proc gmp_randinit_lc_2exp_size(ref STATE: gmp_randstate_t, SIZE: c_ulong);
  extern proc gmp_randinit_set(ref ROP: gmp_randstate_t, ref OP: gmp_randstate_t);
  extern proc gmp_randclear(ref STATE: gmp_randstate_t);

  // Initializing integers
  extern proc mpz_init2(ref X: mpz_t, N: c_ulong);
  extern proc mpz_init(ref X: mpz_t);
  extern proc mpz_clear(ref X: mpz_t);
  extern proc mpz_realloc2(ref X: mpz_t, NBITS: c_ulong);

  // Assignment functions
  extern proc mpz_set(ref ROP: mpz_t, OP: mpz_t);
  extern proc mpz_set_ui(ref ROP: mpz_t, OP: c_ulong);
  extern proc mpz_set_si(ref ROP: mpz_t, OP: c_long);
  extern proc mpz_set_d(ref ROP: mpz_t, OP: c_double);
  // mpz_set_q once rationals are supported
  // mpz_set_f once mpf supported
  extern proc mpz_set_str(ref ROP: mpz_t, STR: c_string, BASE: c_int);
  extern proc mpz_swap(ref ROP1: mpz_t, ref ROP2: mpz_t);

  // Combined initialization and setting
  extern proc mpz_init_set(ref ROP: mpz_t, const ref OP:mpz_t);
  extern proc mpz_init_set_ui(ref ROP: mpz_t, OP:c_ulong);
  extern proc mpz_init_set_si(ref ROP: mpz_t, OP:c_long);
  extern proc mpz_init_set_d(ref ROP: mpz_t, OP:c_double);
  extern proc mpz_init_set_str(ref ROP: mpz_t, STR: c_string, BASE:c_int):c_int;

  // Conversion functions
  extern proc mpz_get_ui(ref OP: mpz_t): c_ulong;
  extern proc mpz_get_si(ref OP: mpz_t): c_long;
  extern proc mpz_get_d(ref OP: mpz_t): c_double;
  extern proc mpz_get_d_2exp(ref exp:c_long, ref OP: mpz_t): c_double;
  extern proc mpz_get_str(STR:c_string, BASE:c_int, ref OP: mpz_t): c_string;

  // Arithmetic functions
  extern proc mpz_add(ref ROP: mpz_t, const ref OP1: mpz_t, const ref OP2: mpz_t);
  extern proc mpz_add_ui(ref ROP: mpz_t, const ref OP1: mpz_t, OP2: c_ulong);

  extern proc mpz_sub(ref ROP: mpz_t, const ref OP1: mpz_t, const ref OP2: mpz_t);
  extern proc mpz_sub_ui(ref ROP: mpz_t, const ref OP1: mpz_t, OP2: c_ulong);
  extern proc mpz_ui_sub(ref ROP: mpz_t, OP1: c_ulong, const ref OP2: mpz_t);

  extern proc mpz_mul(ref ROP: mpz_t, const ref OP1: mpz_t, const ref OP2: mpz_t);
  extern proc mpz_mul_si(ref ROP: mpz_t, const ref OP1: mpz_t, OP2: c_long);
  extern proc mpz_mul_ui(ref ROP: mpz_t, const ref OP1: mpz_t, OP2: c_ulong);

  extern proc mpz_addmul(ref ROP: mpz_t, const ref OP1: mpz_t, const ref OP2: mpz_t);
  extern proc mpz_addmul_ui(ref ROP: mpz_t, const ref OP1: mpz_t, OP2: c_ulong);

  extern proc mpz_submul(ref ROP: mpz_t, const ref OP1: mpz_t, const ref OP2: mpz_t);
  extern proc mpz_submul_ui(ref ROP: mpz_t, const ref OP1: mpz_t, const OP2: c_ulong);

  extern proc mpz_mul_2exp(ref ROP: mpz_t, const ref OP1: mpz_t, OP2: c_ulong);
  extern proc mpz_div_2exp(ref ROP: mpz_t, const ref OP1: mpz_t, OP2: c_ulong);

  extern proc mpz_neg(ref ROP: mpz_t, const ref OP: mpz_t);
  extern proc mpz_abs(ref ROP: mpz_t, const ref OP: mpz_t);

  // Division Functions
  extern proc mpz_cdiv_q(ref Q: mpz_t, const ref N: mpz_t, const ref D: mpz_t);
  extern proc mpz_fdiv_q(ref Q: mpz_t, const ref N: mpz_t, const ref D: mpz_t);
  extern proc mpz_tdiv_q(ref Q: mpz_t, const ref N: mpz_t, const ref D: mpz_t);

  extern proc mpz_cdiv_r(ref R: mpz_t, const ref N: mpz_t, const ref D: mpz_t);
  extern proc mpz_fdiv_r(ref R: mpz_t, const ref N: mpz_t, const ref D: mpz_t);
  extern proc mpz_tdiv_r(ref R: mpz_t, const ref N: mpz_t, const ref D: mpz_t);

  extern proc mpz_cdiv_qr(ref Q: mpz_t, ref R: mpz_t, const ref N: mpz_t, const ref D: mpz_t);
  extern proc mpz_fdiv_qr(ref Q: mpz_t, ref R: mpz_t, const ref N: mpz_t, const ref D: mpz_t);
  extern proc mpz_tdiv_qr(ref Q: mpz_t, ref R: mpz_t, const ref N: mpz_t, const ref D: mpz_t);

  extern proc mpz_cdiv_q_ui(ref Q: mpz_t, const ref N: mpz_t, D: c_ulong):c_ulong;
  extern proc mpz_fdiv_q_ui(ref Q: mpz_t, const ref N: mpz_t, D: c_ulong):c_ulong;
  extern proc mpz_tdiv_q_ui(ref Q: mpz_t, const ref N: mpz_t, D: c_ulong):c_ulong;

  extern proc mpz_cdiv_r_ui(ref R: mpz_t, const ref N: mpz_t, D: c_ulong):c_ulong;
  extern proc mpz_fdiv_r_ui(ref R: mpz_t, const ref N: mpz_t, D: c_ulong):c_ulong;
  extern proc mpz_tdiv_r_ui(ref R: mpz_t, const ref N: mpz_t, D: c_ulong):c_ulong;

  extern proc mpz_cdiv_qr_ui(ref Q: mpz_t, ref R: mpz_t, const ref N: mpz_t, D: c_ulong):c_ulong;
  extern proc mpz_fdiv_qr_ui(ref Q: mpz_t, ref R: mpz_t, const ref N: mpz_t, D: c_ulong):c_ulong;
  extern proc mpz_tdiv_qr_ui(ref Q: mpz_t, ref R: mpz_t, const ref N: mpz_t, D: c_ulong):c_ulong;

  extern proc mpz_cdiv_ui(const ref N: mpz_t, D: c_ulong):c_ulong;
  extern proc mpz_fdiv_ui(const ref N: mpz_t, D: c_ulong):c_ulong;
  extern proc mpz_tdiv_ui(const ref N: mpz_t, D: c_ulong):c_ulong;

  extern proc mpz_cdiv_q_2exp(ref Q:mpz_t, const ref N: mpz_t, B: c_ulong);
  extern proc mpz_fdiv_q_2exp(ref Q:mpz_t, const ref N: mpz_t, B: c_ulong);
  extern proc mpz_tdiv_q_2exp(ref Q:mpz_t, const ref N: mpz_t, B: c_ulong);

  extern proc mpz_cdiv_r_2exp(ref R:mpz_t, const ref N: mpz_t, B: c_ulong);
  extern proc mpz_fdiv_r_2exp(ref R:mpz_t, const ref N: mpz_t, B: c_ulong);
  extern proc mpz_tdiv_r_2exp(ref R:mpz_t, const ref N: mpz_t, B: c_ulong);

  extern proc mpz_mod(ref R: mpz_t, const ref N: mpz_t, const ref D:mpz_t);
  extern proc mpz_mod_ui(ref R: mpz_t, const ref N: mpz_t, D:c_ulong):c_ulong;

  extern proc mpz_divexact(ref Q: mpz_t, const ref N: mpz_t, const ref D:mpz_t);
  extern proc mpz_divexact_ui(ref Q: mpz_t, const ref N: mpz_t, D:c_ulong);

  extern proc mpz_divisible_p(ref N: mpz_t, const ref D:mpz_t):c_int;
  extern proc mpz_divisible_ui_p(ref N: mpz_t, D:c_ulong):c_int;
  extern proc mpz_divisible_2exp_p(ref N: mpz_t, B:c_ulong):c_int;

  extern proc mpz_congruent_p(ref N: mpz_t, const ref C:mpz_t, const ref D:mpz_t):c_int;
  extern proc mpz_congruent_ui_p(ref N: mpz_t, C:c_ulong, D:c_ulong):c_int;
  extern proc mpz_congruent_2exp_p(ref N: mpz_t, const ref C:mpz_t, B:c_ulong):c_int;

  // Exponentiation Functions
  extern proc mpz_powm(ref ROP: mpz_t, const ref BASE: mpz_t, const ref EXP: mpz_t, const ref MOD: mpz_t);
  extern proc mpz_powm_ui(ref ROP: mpz_t, const ref BASE: mpz_t, EXP: c_ulong, const ref MOD: mpz_t);
  extern proc mpz_pow_ui(ref ROP: mpz_t, const ref BASE: mpz_t, EXP: c_ulong);
  extern proc mpz_ui_pow_ui(ref ROP: mpz_t, BASE: c_ulong, EXP: c_ulong);

  // Root Extraction Functions
  extern proc mpz_root(ref ROP: mpz_t, const ref OP: mpz_t, N: c_ulong): c_int;
  extern proc mpz_rootrem(ref ROOT: mpz_t, ref REM: mpz_t, const ref U: mpz_t, N: c_ulong);
  extern proc mpz_sqrt(ref ROP: mpz_t, const ref OP: mpz_t);
  extern proc mpz_sqrtrem(ref ROP1: mpz_t, ref ROP2: mpz_t, const ref OP: mpz_t);
  extern proc mpz_perfect_power_p(const ref OP: mpz_t):c_int;
  extern proc mpz_perfect_square_p(const ref OP: mpz_t):c_int;

  // Number Theoretic Functions
  extern proc mpz_probab_prime_p(const ref N: mpz_t, REPS: c_int): c_int;
  extern proc mpz_nextprime(ref ROP: mpz_t, const ref OP: mpz_t);
  extern proc mpz_gcd(ref ROP: mpz_t, const ref OP1: mpz_t, const ref OP2: mpz_t);
  extern proc mpz_gcd_ui(ref ROP: mpz_t, const ref OP1: mpz_t, const OP2: c_ulong);

  extern proc mpz_gcdext(ref G: mpz_t, ref S: mpz_t, ref T: mpz_t, const ref A: mpz_t, const ref B: mpz_t);
  extern proc mpz_lcm(ref ROP: mpz_t, const ref OP1: mpz_t, const ref OP2: mpz_t);
  extern proc mpz_lcm_ui(ref ROP: mpz_t, const ref OP1: mpz_t, OP2: c_ulong);
  extern proc mpz_invert(ref ROP: mpz_t, const ref OP1: mpz_t, const ref OP2: mpz_t):c_int;
  extern proc mpz_jacobi(const ref A: mpz_t, const ref B: mpz_t):c_int;
  extern proc mpz_legendre(const ref A: mpz_t, const ref P: mpz_t):c_int;
  extern proc mpz_kronecker(const ref A: mpz_t, const ref B: mpz_t):c_int;
  extern proc mpz_kronecker_si(const ref A: mpz_t, B: c_long):c_int;
  extern proc mpz_kronecker_ui(const ref A: mpz_t, B: c_ulong):c_int;
  extern proc mpz_si_kronecker(A: c_long, const ref B: mpz_t):c_int;
  extern proc mpz_ui_kronecker(A: c_ulong, const ref B: mpz_t):c_int;

  extern proc mpz_remove(ref ROP: mpz_t, const ref OP: mpz_t, const ref F: mpz_t):c_ulong;
  extern proc mpz_fac_ui(ref ROP: mpz_t, OP: c_ulong);
  extern proc mpz_bin_ui(ref ROP: mpz_t, const ref N: mpz_t, K: c_ulong);
  extern proc mpz_bin_uiui(ref ROP: mpz_t, N: c_ulong, K: c_ulong);
  extern proc mpz_fib_ui(ref FN: mpz_t, N: c_ulong);
  extern proc mpz_fib2_ui(ref FN: mpz_t, ref FNSUB1: mpz_t, N: c_ulong);
  extern proc mpz_lucnum_ui(ref LN: mpz_t, N: c_ulong);
  extern proc mpz_lucnum2_ui(ref LN: mpz_t, LNSUB1: mpz_t, N: c_ulong);

  // Comparison Functions
  extern proc mpz_cmp(const ref OP1: mpz_t, const ref OP2: mpz_t) : c_int;
  extern proc mpz_cmp_d(const ref OP1: mpz_t, OP2: c_double) : c_int;
  extern proc mpz_cmp_si(const ref OP1: mpz_t, OP2: c_long) : c_int;
  extern proc mpz_cmp_ui(const ref OP1: mpz_t, OP2: c_ulong) : c_int;
  extern proc mpz_cmpabs(const ref OP1: mpz_t, const ref OP2: mpz_t) : c_int;
  extern proc mpz_cmpabs_d(const ref OP1: mpz_t, OP2: c_double) : c_int;
  extern proc mpz_cmpabs_ui(const ref OP1: mpz_t, OP2: c_ulong) : c_int;
  
  extern proc mpz_sgn(const ref OP: mpz_t): c_int;

  // Logical and Bit Manipulation Functions
  extern proc mpz_and(ref ROP: mpz_t, const ref OP1: mpz_t, const ref OP2: mpz_t);
  extern proc mpz_ior(ref ROP: mpz_t, const ref OP1: mpz_t, const ref OP2: mpz_t);
  extern proc mpz_xor(ref ROP: mpz_t, const ref OP1: mpz_t, const ref OP2: mpz_t);
  extern proc mpz_com(ref ROP: mpz_t, const ref OP: mpz_t);
  extern proc mpz_popcount(const ref OP: mpz_t):c_ulong;
  extern proc mpz_hamdist(const ref OP1: mpz_t, const ref OP2: mpz_t):c_ulong;
  extern proc mpz_scan0(const ref OP: mpz_t, STARTING_BIT:c_ulong):c_ulong;
  extern proc mpz_scan1(const ref OP: mpz_t, STARTING_BIT:c_ulong):c_ulong;
  extern proc mpz_setbit(const ref ROP: mpz_t, BIT_INDEX:c_ulong);
  extern proc mpz_clrbit(const ref ROP: mpz_t, BIT_INDEX:c_ulong);
  extern proc mpz_combit(const ref ROP: mpz_t, BIT_INDEX:c_ulong);
  extern proc mpz_tstbit(const ref OP: mpz_t, BIT_INDEX:c_ulong):c_int;

  // Input and Output Functions

  // Random Number Functions
  extern proc mpz_urandomb(ref ROP: mpz_t, STATE: gmp_randstate_t, N: c_ulong);
  extern proc mpz_urandomm(ref ROP: mpz_t, STATE: gmp_randstate_t, N: mpz_t);
  extern proc mpz_rrandomb(ref ROP: mpz_t, STATE: gmp_randstate_t, N: c_ulong);

  extern proc gmp_randseed(ref STATE: gmp_randstate_t, SEED: mpz_t);
  extern proc gmp_randseed_ui(ref STATE: gmp_randstate_t, SEED: c_ulong);

  extern proc gmp_urandomb_ui(ref STATE: gmp_randstate_t, N: c_ulong):c_ulong;
  extern proc gmp_urandomm_ui(ref STATE: gmp_randstate_t, N: c_ulong):c_ulong;

  // Integer import and export

  // Miscellaneous Functions
  extern proc mpz_fits_ulong_p(const ref OP: mpz_t): c_int;
  extern proc mpz_fits_slong_p(const ref OP: mpz_t): c_int;
  extern proc mpz_fits_uint_p(const ref OP: mpz_t): c_int;
  extern proc mpz_fits_sint_p(const ref OP: mpz_t): c_int;
  extern proc mpz_fits_ushort_p(const ref OP: mpz_t): c_int;
  extern proc mpz_fits_sshort_p(const ref OP: mpz_t): c_int;

  extern proc mpz_odd_p(const ref OP: mpz_t): c_int;
  extern proc mpz_even_p(const ref OP: mpz_t): c_int;
  extern proc mpz_sizeinbase(const ref OP: mpz_t, BASE: c_int): size_t;
  extern proc mpz_size(const ref X: mpz_t): size_t;
  extern proc mpf_set_default_prec(PREC: mp_bitcnt_t);
  extern proc mpz_getlimbn(const ref OP: mpz_t, n:mp_size_t): mp_limb_t; 

  // floating-point functions
  // TODO: These probably also should have const ref OPs
  extern proc mpf_init(ref X: mpf_t);
  extern proc mpf_set_z(ref ROP: mpf_t, ref OP: mpz_t);
  extern proc mpf_get_prec(ref OP: mpf_t): mp_bitcnt_t;
  extern proc mpf_get_d(ref OP: mpf_t): c_double;
  extern proc mpf_set_d(ref ROP: mpf_t, OP: c_double);
  extern proc mpf_set_prec_raw(ref ROP: mpf_t, PREC: mp_bitcnt_t);
  extern proc mpf_ui_div(ref ROP: mpf_t, OP1: c_ulong, ref OP2: mpf_t);
  extern proc mpf_mul(ref ROP: mpf_t, ref OP1: mpf_t, ref OP2: mpf_t);
  extern proc mpf_ui_sub(ref ROP: mpf_t, OP1: c_ulong, ref OP2: mpf_t);
  extern proc mpf_add(ref ROP: mpf_t, ref OP1: mpf_t, ref OP2: mpf_t);
  extern proc mpf_sub(ref ROP: mpf_t, ref OP1: mpf_t, ref OP2: mpf_t);
  extern proc mpf_mul_ui(ref ROP: mpf_t, ref OP1: mpf_t, OP2: c_ulong);
  extern proc mpf_div_2exp(ref ROP: mpf_t, ref OP1: mpf_t, OP2: mp_bitcnt_t);
  extern proc mpf_out_str(STREAM: _file, BASE: c_int, N_DIGITS: size_t, ref OP: mpf_t);
  extern proc mpf_clear(ref X: mpf_t);

  // printf/scanf
  extern proc gmp_printf(fmt: c_string, arg...);
  extern proc gmp_fprintf(fp: _file, fmt: c_string, arg...);
  extern proc gmp_fprintf(fp: _file, fmt: c_string, arg...);
  extern proc gmp_asprintf(ref ret: c_string, fmt: c_string, arg...);

  // Initialize GMP to use Chapel's allocator
  // some of these used to be private, now can't because BigInt needs them
  extern proc chpl_gmp_init();
  /* Get an MPZ value stored on another locale */
  extern proc chpl_gmp_get_mpz(ref ret:mpz_t,src_local:int,from:__mpz_struct);
  /* Get a randstate value stored on another locale */
  extern proc chpl_gmp_get_randstate(not_inited_state:gmp_randstate_t, src_locale:int, from:__gmp_randstate_struct);
  /* Return the number of limbs in an __mpz_struct */
  extern proc chpl_gmp_mpz_nlimbs(from:__mpz_struct):uint(64);
  /* Print out an mpz_t (for debugging) */
  extern proc chpl_gmp_mpz_print(x:mpz_t);
  /* Get an mpz_t as a string */
  extern proc chpl_gmp_mpz_get_str(base: c_int, x:mpz_t):c_string;

  // calls mp_set_memory_functions to use chpl_malloc, etc.
  chpl_gmp_init();
}
