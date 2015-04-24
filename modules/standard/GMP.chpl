/*
 * Copyright 2004-2015 Cray Inc.
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
sufficient interest, this protype can be expanded to support the full GMP
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
  or the BigInt class (see below for a complete listing).


Using the BigInt class
----------------------

The GMP Chapel module provides a :class:`BigInt` class wrapping GMP integers.
At the present time, only the functions for ``mpz`` (ie signed integer)
GMP types are supported with :class:`BigInt`; future work will be to
extend this support to floating-point types. Also, in the future,
we hope to provide these GMP functions as records that support
operators like ``+``, ``*``, ``-``, etc.

:class:`BigInt` methods all wrap GMP functions with obviously similar names.
The :class:`BigInt` methods are locale aware - so Chapel programs can create a
distributed array of GMP numbers. The method of :class:`BigInt` objects are
setting the receiver, so e.g. myBigInt.add(x,y) sets myBigInt to ``x + y``.

A code example::

 use GMP;

 var a = new BigInt(); // initialize a GMP value, set it to zero

 a.fac_ui(100); // set a to 100!

 writeln(a); // output 100!

 delete a; // free memory used by the GMP value

 var b = new BigInt("48473822929893829847"); // initialize from a decimal string
 b.add_ui(b, 1); // add one to b

 delete b; // free memory used by b

Calling GMP functions directly
------------------------------

The second option for Chapel programs using this module is to call the GMP
functions directly. For a full reference to GMP capabilities, please refer to
the `GMP website <http://gmplib.org>`_ and the
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
  extern type mpz_t = 1*__mpz_struct;

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
     seem suprising. They are that way because identity
     matters and they may get reallocated otherwise;
     ref is currently the only way to avoid that.
   
     Note that GMP defines e.g. gmp_randstate_t with
     typedef struct __gmp_randstate_struct gmp_randstate_t[1],
    
     and the natural way to wrap that in Chapel is with a 1-element
     tuple. These tuples would be passed by value to the extern
     routines if not for ref.
   */

  // Initializing random state 
  /* */
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
  extern proc mpz_init_set(ref ROP: mpz_t, ref OP:mpz_t);
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
  extern proc mpz_add(ref ROP: mpz_t, ref OP1: mpz_t, ref OP2: mpz_t);
  extern proc mpz_add_ui(ref ROP: mpz_t, ref OP1: mpz_t, OP2: c_ulong);

  extern proc mpz_sub(ref ROP: mpz_t, ref OP1: mpz_t, ref OP2: mpz_t);
  extern proc mpz_sub_ui(ref ROP: mpz_t, ref OP1: mpz_t, OP2: c_ulong);
  extern proc mpz_ui_sub(ref ROP: mpz_t, OP1: c_ulong, ref OP2: mpz_t);

  extern proc mpz_mul(ref ROP: mpz_t, ref OP1: mpz_t, ref OP2: mpz_t);
  extern proc mpz_mul_si(ref ROP: mpz_t, ref OP1: mpz_t, OP2: c_long);
  extern proc mpz_mul_ui(ref ROP: mpz_t, ref OP1: mpz_t, OP2: c_ulong);

  extern proc mpz_addmul(ref ROP: mpz_t, ref OP1: mpz_t, ref OP2: mpz_t);
  extern proc mpz_addmul_ui(ref ROP: mpz_t, ref OP1: mpz_t, OP2: c_ulong);

  extern proc mpz_submul(ref ROP: mpz_t, ref OP1: mpz_t, ref OP2: mpz_t);
  extern proc mpz_submul_ui(ref ROP: mpz_t, ref OP1: mpz_t, OP2: c_ulong);

  extern proc mpz_mul_2exp(ref ROP: mpz_t, ref OP1: mpz_t, OP2: c_ulong);

  extern proc mpz_neg(ref ROP: mpz_t, ref OP: mpz_t);
  extern proc mpz_abs(ref ROP: mpz_t, ref OP: mpz_t);

  // Division Functions
  extern proc mpz_cdiv_q(ref Q: mpz_t, ref N: mpz_t, ref D: mpz_t);
  extern proc mpz_fdiv_q(ref Q: mpz_t, ref N: mpz_t, ref D: mpz_t);
  extern proc mpz_tdiv_q(ref Q: mpz_t, ref N: mpz_t, ref D: mpz_t);

  extern proc mpz_cdiv_r(ref R: mpz_t, ref N: mpz_t, ref D: mpz_t);
  extern proc mpz_fdiv_r(ref R: mpz_t, ref N: mpz_t, ref D: mpz_t);
  extern proc mpz_tdiv_r(ref R: mpz_t, ref N: mpz_t, ref D: mpz_t);

  extern proc mpz_cdiv_qr(ref Q: mpz_t, ref R: mpz_t, ref N: mpz_t, ref D: mpz_t);
  extern proc mpz_fdiv_qr(ref Q: mpz_t, ref R: mpz_t, ref N: mpz_t, ref D: mpz_t);
  extern proc mpz_tdiv_qr(ref Q: mpz_t, ref R: mpz_t, ref N: mpz_t, ref D: mpz_t);

  extern proc mpz_cdiv_q_ui(ref Q: mpz_t, ref N: mpz_t, D: c_ulong):c_ulong;
  extern proc mpz_fdiv_q_ui(ref Q: mpz_t, ref N: mpz_t, D: c_ulong):c_ulong;
  extern proc mpz_tdiv_q_ui(ref Q: mpz_t, ref N: mpz_t, D: c_ulong):c_ulong;

  extern proc mpz_cdiv_r_ui(ref R: mpz_t, ref N: mpz_t, D: c_ulong):c_ulong;
  extern proc mpz_fdiv_r_ui(ref R: mpz_t, ref N: mpz_t, D: c_ulong):c_ulong;
  extern proc mpz_tdiv_r_ui(ref R: mpz_t, ref N: mpz_t, D: c_ulong):c_ulong;


  extern proc mpz_cdiv_qr_ui(ref Q: mpz_t, ref R: mpz_t, ref N: mpz_t, D: c_ulong):c_ulong;
  extern proc mpz_fdiv_qr_ui(ref Q: mpz_t, ref R: mpz_t, ref N: mpz_t, D: c_ulong):c_ulong;
  extern proc mpz_tdiv_qr_ui(ref Q: mpz_t, ref R: mpz_t, ref N: mpz_t, D: c_ulong):c_ulong;

  extern proc mpz_cdiv_ui(ref N: mpz_t, D: c_ulong):c_ulong;
  extern proc mpz_fdiv_ui(ref N: mpz_t, D: c_ulong):c_ulong;
  extern proc mpz_tdiv_ui(ref N: mpz_t, D: c_ulong):c_ulong;

  extern proc mpz_cdiv_q_2exp(ref Q:mpz_t, ref N: mpz_t, B: c_ulong);
  extern proc mpz_fdiv_q_2exp(ref Q:mpz_t, ref N: mpz_t, B: c_ulong);
  extern proc mpz_tdiv_q_2exp(ref Q:mpz_t, ref N: mpz_t, B: c_ulong);

  extern proc mpz_cdiv_r_2exp(ref R:mpz_t, ref N: mpz_t, B: c_ulong);
  extern proc mpz_fdiv_r_2exp(ref R:mpz_t, ref N: mpz_t, B: c_ulong);
  extern proc mpz_tdiv_r_2exp(ref R:mpz_t, ref N: mpz_t, B: c_ulong);

  extern proc mpz_mod(ref R: mpz_t, ref N: mpz_t, ref D:mpz_t);
  extern proc mpz_mod_ui(ref R: mpz_t, ref N: mpz_t, D:c_ulong):c_ulong;

  extern proc mpz_divexact(ref Q: mpz_t, ref N: mpz_t, ref D:mpz_t);
  extern proc mpz_divexact_ui(ref Q: mpz_t, ref N: mpz_t, D:c_ulong);

  extern proc mpz_divisible_p(ref N: mpz_t, ref D:mpz_t):c_int;
  extern proc mpz_divisible_ui_p(ref N: mpz_t, D:c_ulong):c_int;
  extern proc mpz_divisible_2exp_p(ref N: mpz_t, B:c_ulong):c_int;

  extern proc mpz_conguent_p(ref N: mpz_t, ref C:mpz_t, ref D:mpz_t):c_int;
  extern proc mpz_congruent_ui_p(ref N: mpz_t, C:c_ulong, D:c_ulong):c_int;
  extern proc mpz_congruent_2exp_p(ref N: mpz_t, ref C:mpz_t, B:c_int):c_int;

  // Exponentiation Functions
  extern proc mpz_powm(ref ROP: mpz_t, ref BASE: mpz_t, ref EXP: mpz_t, ref MOD: mpz_t);
  extern proc mpz_powm_ui(ref ROP: mpz_t, ref BASE: mpz_t, EXP: c_ulong, ref MOD: mpz_t);
  extern proc mpz_pow_ui(ref ROP: mpz_t, ref BASE: mpz_t, EXP: c_ulong);
  extern proc mpz_ui_pow_ui(ref ROP: mpz_t, BASE: c_ulong, EXP: c_ulong);

  // Root Extraction Functions
  extern proc mpz_root(ref ROP: mpz_t, ref OP: mpz_t, N: c_ulong): c_int;
  extern proc mpz_rootrem(ref ROOT: mpz_t, ref REM: mpz_t, ref U: mpz_t, N: c_ulong);
  extern proc mpz_sqrt(ref ROP: mpz_t, ref OP: mpz_t);
  extern proc mpz_sqrtrem(ref ROP1: mpz_t, ref ROP2: mpz_t, ref OP: mpz_t);
  extern proc mpz_perfect_power_p(ref OP: mpz_t):c_int;
  extern proc mpz_perfect_square_p(ref OP: mpz_t):c_int;

  // Number Theoretic Functions
  extern proc mpz_probab_prime_p(ref N: mpz_t, REPS: c_int): c_int;
  extern proc mpz_nextprime(ref ROP: mpz_t, ref OP: mpz_t);
  extern proc mpz_gcd(ref ROP: mpz_t, ref OP1: mpz_t, ref OP2: mpz_t);
  extern proc mpz_gcd_ui(ref ROP: mpz_t, ref OP1: mpz_t, OP2: c_ulong);

  extern proc mpz_gcdext(ref G: mpz_t, ref S: mpz_t, ref T: mpz_t, ref A: mpz_t, ref B: mpz_t);
  extern proc mpz_lcm(ref ROP: mpz_t, ref OP1: mpz_t, ref OP2: mpz_t);
  extern proc mpz_lcm_ui(ref ROP: mpz_t, ref OP1: mpz_t, OP2: c_ulong);
  extern proc mpz_invert(ref ROP: mpz_t, ref OP1: mpz_t, ref OP2: mpz_t):c_int;
  extern proc mpz_jacobi(ref A: mpz_t, ref B: mpz_t):c_int;
  extern proc mpz_legendre(ref A: mpz_t, ref P: mpz_t):c_int;
  extern proc mpz_kronecker(ref A: mpz_t, ref B: mpz_t):c_int;
  extern proc mpz_kronecker_si(ref A: mpz_t, B: c_long):c_int;
  extern proc mpz_kronecker_ui(ref A: mpz_t, B: c_ulong):c_int;
  extern proc mpz_si_kronecker(A: c_long, ref B: mpz_t):c_int;
  extern proc mpz_ui_kronecker(A: c_ulong, ref B: mpz_t):c_int;

  extern proc mpz_remove(ref ROP: mpz_t, ref OP: mpz_t, ref F: mpz_t):c_ulong;
  extern proc mpz_fac_ui(ref ROP: mpz_t, OP: c_ulong);
  extern proc mpz_bin_ui(ref ROP: mpz_t, N: mpz_t, K: c_ulong);
  extern proc mpz_bin_uiui(ref ROP: mpz_t, N: c_ulong, K: c_ulong);
  extern proc mpz_fib_ui(ref FN: mpz_t, N: c_ulong);
  extern proc mpz_fib2_ui(ref FN: mpz_t, FNSUB1: mpz_t, N: c_ulong);
  extern proc mpz_lucnum_ui(ref LN: mpz_t, N: c_ulong);
  extern proc mpz_lucnum2_ui(ref LN: mpz_t, LNSUB1: mpz_t, N: c_ulong);

  // Comparison Functions
  extern proc mpz_cmp(ref OP1: mpz_t, ref OP2: mpz_t) : c_int;
  extern proc mpz_cmp_d(ref OP1: mpz_t, OP2: c_double) : c_int;
  extern proc mpz_cmp_si(ref OP1: mpz_t, OP2: c_long) : c_int;
  extern proc mpz_cmp_ui(ref OP1: mpz_t, OP2: c_ulong) : c_int;
  extern proc mpz_cmpabs(ref OP1: mpz_t, ref OP2: mpz_t) : c_int;
  extern proc mpz_cmpabs_d(ref OP1: mpz_t, OP2: c_double) : c_int;
  extern proc mpz_cmpabs_ui(ref OP1: mpz_t, OP2: c_ulong) : c_int;
  
  extern proc mpz_sgn(ref OP: mpz_t): c_int;

  // Logical and Bit Manipulation Functions
  extern proc mpz_and(ref ROP: mpz_t, ref OP1: mpz_t, ref OP2: mpz_t);
  extern proc mpz_ior(ref ROP: mpz_t, ref OP1: mpz_t, ref OP2: mpz_t);
  extern proc mpz_xor(ref ROP: mpz_t, ref OP1: mpz_t, ref OP2: mpz_t);
  extern proc mpz_com(ref ROP: mpz_t, ref OP: mpz_t);
  extern proc mpz_popcount(ref OP: mpz_t):c_ulong;
  extern proc mpz_hamdist(ref OP1: mpz_t, ref OP2: mpz_t):c_ulong;
  extern proc mpz_scan0(ref OP: mpz_t, STARTING_BIT:c_ulong):c_ulong;
  extern proc mpz_scan1(ref OP: mpz_t, STARTING_BIT:c_ulong):c_ulong;
  extern proc mpz_setbit(ref ROP: mpz_t, BIT_INDEX:c_ulong);
  extern proc mpz_clrbit(ref ROP: mpz_t, BIT_INDEX:c_ulong);
  extern proc mpz_combit(ref ROP: mpz_t, BIT_INDEX:c_ulong);
  extern proc mpz_tstbit(ref OP: mpz_t, BIT_INDEX:c_ulong):c_int;

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
  extern proc mpz_fits_ulong_p(ref OP: mpz_t): c_int;
  extern proc mpz_fits_slong_p(ref OP: mpz_t): c_int;
  extern proc mpz_fits_uint_p(ref OP: mpz_t): c_int;
  extern proc mpz_fits_sint_p(ref OP: mpz_t): c_int;
  extern proc mpz_fits_ushort_p(ref OP: mpz_t): c_int;
  extern proc mpz_fits_sshort_p(ref OP: mpz_t): c_int;

  extern proc mpz_odd_p(ref OP: mpz_t): c_int;
  extern proc mpz_even_p(ref OP: mpz_t): c_int;
  extern proc mpz_sizeinbase(ref OP: mpz_t, BASE: c_int): size_t;

  extern proc mpf_set_default_prec(PREC: mp_bitcnt_t);

  // floating-point functions
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
  private extern proc chpl_gmp_init();
  /* Get an MPZ value stored on another locale */
  private extern proc chpl_gmp_get_mpz(ref ret:mpz_t,src_local:int,from:__mpz_struct);
  /* Get a randstate value stored on another locale */
  private extern proc chpl_gmp_get_randstate(not_inited_state:gmp_randstate_t, src_locale:int, from:__gmp_randstate_struct);
  /* Return the number of limbs in an __mpz_struct */
  private extern proc chpl_gmp_mpz_nlimbs(from:__mpz_struct):uint(64);
  /* Print out an mpz_t (for debugging) */
  extern proc chpl_gmp_mpz_print(x:mpz_t);
  /* Get an mpz_t as a string */
  extern proc chpl_gmp_mpz_get_str(base: c_int, x:mpz_t):c_string_copy;


  enum Round {
    UP = 1,
    DOWN = -1,
    ZERO = 0
  }

  /*
    The BigInt class provides a more Chapel-friendly interface to the
    GMP integer functions. In particular, this class supports GMP
    numbers that are stored in distributed arrays.
    
    All methods on BigInt work with Chapel types. Many of them use the gmp
    functions directly, which use C types. Runtime checks are used to ensure the
    Chapel types can safely be cast to the C types (e.g. when casting a Chapel
    uint it checks that it fits in the C ulong which could be a 32bit type if
    running on linux32 platform).

    The checks are controlled by the compiler options ``--[no-]cast-checks``,
    ``--fast``, etc.

    TODO: When a Chapel will not safely cast to a C type, it would be better to
    promte the Chapel value to a BigInt, then run the operation on that
    BigInt. This would make the BigInt interface consistent across all
    platforms (already true today) _and_ always work regardless of platform
    (not true today).
   */
  class BigInt {
    var mpz:mpz_t;

    // initializing integers (constructors)
    proc BigInt(init2:bool, nbits:uint) {
      mpz_init2(this.mpz, nbits.safeCast(c_ulong));
    }
    proc BigInt(num:int) {
      mpz_init_set_si(this.mpz, num.safeCast(c_long));
    }
    proc BigInt(str:string, base:int=0) {
      var e:c_int;
      e = mpz_init_set_str(this.mpz, str.c_str(), base.safeCast(c_int));
      if e {
        mpz_clear(this.mpz);
        halt("Error initializing big integer: bad format");
      }
    }
    proc BigInt(str:string, base:int=0, out error:syserr) {
      var e:c_int;
      error = ENOERR;
      e = mpz_init_set_str(this.mpz, str.c_str(), base.safeCast(c_int));
      if e {
        mpz_clear(this.mpz);
        error = EFORMAT;
      }
    }
    proc BigInt(num:BigInt) {
      if num.locale == here {
        mpz_init_set(this.mpz, num.mpz);
      } else {
        mpz_init(this.mpz);
        var mpz_struct = num.mpzStruct(); 
        chpl_gmp_get_mpz(this.mpz, num.locale.id, mpz_struct);
      }
    }
    proc BigInt() { mpz_init(this.mpz); }

    // destructor
    proc ~BigInt() { on this do mpz_clear(this.mpz); }

    // utility functions used below.
    proc numLimbs:uint(64) {
      var mpz_struct = this.mpz[1];
      return chpl_gmp_mpz_nlimbs(mpz_struct);
    }

    proc mpzStruct():__mpz_struct {
      var ret:__mpz_struct;
      on this {
        ret = this.mpz[1];
      }
      return ret;
    }


    // returns true if we made a temp copy.
    proc maybeCopy():(bool,BigInt) {
      if this.locale == here {
        return (false,this);
      } else {
        var mpz_struct = this.mpz[1];
        var tmp = new BigInt(true, (mp_bits_per_limb:uint(64))*chpl_gmp_mpz_nlimbs(mpz_struct));
        chpl_gmp_get_mpz(tmp.mpz, this.locale.id, mpz_struct); 
        return (true, tmp);
      }
    }

    // Assignment functions
    proc set(a:BigInt)
    {
      on this {
        if a.locale == here {
          mpz_set(this.mpz, a.mpz);
        } else {
          var mpz_struct = a.mpzStruct(); 
          chpl_gmp_get_mpz(this.mpz, a.locale.id, mpz_struct);
        }
      }
    }
    proc set_ui(num:uint)
    {
      on this do mpz_set_ui(this.mpz, num.safeCast(c_ulong));
    }
    proc set_si(num:int)
    {
      on this do mpz_set_si(this.mpz, num.safeCast(c_long));
    }
    proc set(num:int)
    {
      set_si(num.safeCast(c_long));
    }
    proc set_d(num:real)
    {
      on this do mpz_set_d(this.mpz, num: c_double);
    }
    proc set_str(str:string, base:int=0)
    {
      on this do mpz_set_str(this.mpz, str.c_str(), base.safeCast(c_int));
    }
    proc swap(a:BigInt)
    {
      on this {
        if a.locale == here {
          mpz_swap(this.mpz, a.mpz);
        } else {
          // we have to introduce a temporary..
          var tmp = new BigInt(a);
          // set a to what this
          a.set(this);
          // now tmp is local.
          // set this to a (in tmp)
          mpz_set(this.mpz, tmp.mpz);
        }
      }
    }
    proc get_ui():uint
    {
      var x:c_ulong;
      on this do x = mpz_get_ui(this.mpz);
      return x.safeCast(uint);
    }
    proc get_si():int
    {
      var x:c_long;
      on this do x = mpz_get_si(this.mpz);
      return x.safeCast(int);
    }
    proc get_d():real
    {
      var x:c_double;
      on this do x = mpz_get_d(this.mpz);
      return x:real;
    }
    // returns (exponent, double)
    proc get_d_2exp():(int, real)
    {
      var exp:c_long;
      var dbl:c_double;
      on this {
        var tmp:c_long;
        dbl = mpz_get_d_2exp(tmp, this.mpz);
        exp = tmp;
      }
      return (exp.safeCast(int), dbl: real);
    }
    proc get_str(base:int=10):string
    {
      var ret:string;
      on this {
        var tmp = chpl_gmp_mpz_get_str(base.safeCast(c_int), this.mpz);
        ret = tmp:string;
      }
      return ret;
    }

    // Arithmetic functions
    proc add(a:BigInt, b:BigInt)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        var (bcopy,b_) = b.maybeCopy();
        mpz_add(this.mpz, a_.mpz, b_.mpz);
        if acopy then delete a_;
        if bcopy then delete b_;
      }
    }
    proc add_ui(a:BigInt, b:uint)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        mpz_add_ui(this.mpz, a_.mpz, b.safeCast(c_ulong));
        if acopy then delete a_;
      }
    }
    proc sub(a:BigInt, b:BigInt)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        var (bcopy,b_) = b.maybeCopy();
        mpz_sub(this.mpz, a_.mpz, b_.mpz);
        if acopy then delete a_;
        if bcopy then delete b_;
      }
    }
    proc sub_ui(a:BigInt, b:uint)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        mpz_sub_ui(this.mpz, a_.mpz, b.safeCast(c_ulong));
        if acopy then delete a_;
      }
    }
    proc ui_sub(a:uint, b:BigInt)
    {
      on this {
        var (bcopy,b_) = b.maybeCopy();
        mpz_ui_sub(this.mpz, a.safeCast(c_ulong), b_.mpz);
        if bcopy then delete b_;
      }
    }
    proc mul(a:BigInt, b:BigInt)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        var (bcopy,b_) = b.maybeCopy();
        mpz_mul(this.mpz, a_.mpz, b_.mpz);
        if acopy then delete a_;
        if bcopy then delete b_;
      }
    }
    proc mul_si(a:BigInt, b:int)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        mpz_mul_si(this.mpz, a_.mpz, b.safeCast(c_long));
        if acopy then delete a_;
      }
    }
    proc mul_ui(a:BigInt, b:uint)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        mpz_mul_ui(this.mpz, a_.mpz, b.safeCast(c_ulong));
        if acopy then delete a_;
      }
    }
    proc addmul(a:BigInt, b:BigInt)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        var (bcopy,b_) = b.maybeCopy();
        mpz_addmul(this.mpz, a_.mpz, b_.mpz);
        if acopy then delete a_;
        if bcopy then delete b_;
      }
    }
    proc addmul_ui(a:BigInt, b:uint)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        mpz_addmul_ui(this.mpz, a_.mpz, b.safeCast(c_ulong));
        if acopy then delete a_;
      }
    }
    proc submul(a:BigInt, b:BigInt)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        var (bcopy,b_) = b.maybeCopy();
        mpz_submul(this.mpz, a_.mpz, b_.mpz);
        if acopy then delete a_;
        if bcopy then delete b_;
      }
    }
    proc submul_ui(a:BigInt, b:uint)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        mpz_submul_ui(this.mpz, a_.mpz, b.safeCast(c_ulong));
        if acopy then delete a_;
      }
    }
    proc mul_2exp(a:BigInt, b:uint)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        mpz_mul_2exp(this.mpz, a_.mpz, b.safeCast(c_ulong));
        if acopy then delete a_;
      }
    }
    proc neg(a:BigInt)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        mpz_neg(this.mpz, a_.mpz);
        if acopy then delete a_;
      }
    }
    proc abs(a:BigInt)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        mpz_abs(this.mpz, a_.mpz);
        if acopy then delete a_;
      }
    }

    // Division Functions
    // These functions take in a constant rounding mode.
    proc div_q(param rounding:Round, n:BigInt, d:BigInt)
    {
      on this {
        var (ncopy,n_) = n.maybeCopy();
        var (dcopy,d_) = d.maybeCopy();
        select rounding {
          when Round.UP   do mpz_cdiv_q(this.mpz, n_.mpz, d_.mpz);
          when Round.DOWN do mpz_fdiv_q(this.mpz, n_.mpz, d_.mpz);
          when Round.ZERO do mpz_tdiv_q(this.mpz, n_.mpz, d_.mpz);
        }
        if ncopy then delete n_;
        if dcopy then delete d_;
      }
    }
    proc div_r(param rounding:Round, n:BigInt, d:BigInt)
    {
      on this {
        var (ncopy,n_) = n.maybeCopy();
        var (dcopy,d_) = d.maybeCopy();
        select rounding {
          when Round.UP   do mpz_cdiv_r(this.mpz, n_.mpz, d_.mpz);
          when Round.DOWN do mpz_fdiv_r(this.mpz, n_.mpz, d_.mpz);
          when Round.ZERO do mpz_tdiv_r(this.mpz, n_.mpz, d_.mpz);
        }
        if ncopy then delete n_;
        if dcopy then delete d_;
      }
    }
    // this gets quotient, r gets remainder
    proc div_qr(param rounding:Round, r:BigInt, n:BigInt, d:BigInt)
    {
      on this {
        var (rcopy,r_) = r.maybeCopy();
        var (ncopy,n_) = n.maybeCopy();
        var (dcopy,d_) = d.maybeCopy();
        select rounding {
          when Round.UP   do mpz_cdiv_qr(this.mpz, r_.mpz, n_.mpz, d_.mpz);
          when Round.DOWN do mpz_fdiv_qr(this.mpz, r_.mpz, n_.mpz, d_.mpz);
          when Round.ZERO do mpz_tdiv_qr(this.mpz, r_.mpz, n_.mpz, d_.mpz);
        }
        if rcopy {
          r.set(r_);
          delete r_;
        }
        if ncopy then delete n_;
        if dcopy then delete d_;
      }
    }
    proc div_q_ui(param rounding:Round, n:BigInt, d:uint):uint
    {
      var ret:c_ulong;
      on this {
        var (ncopy,n_) = n.maybeCopy();
        const cd = d.safeCast(c_ulong);
        select rounding {
          when Round.UP   do ret=mpz_cdiv_q_ui(this.mpz, n_.mpz, cd);
          when Round.DOWN do ret=mpz_fdiv_q_ui(this.mpz, n_.mpz, cd);
          when Round.ZERO do ret=mpz_tdiv_q_ui(this.mpz, n_.mpz, cd);
        }
        if ncopy then delete n_;
      }
      return ret.safeCast(uint);
    }
    proc div_r_ui(param rounding:Round, n:BigInt, d:uint):uint
    {
      var ret:c_ulong;
      on this {
        var (ncopy,n_) = n.maybeCopy();
        const cd = d.safeCast(c_ulong);
        select rounding {
          when Round.UP   do ret=mpz_cdiv_r_ui(this.mpz, n_.mpz, cd);
          when Round.DOWN do ret=mpz_fdiv_r_ui(this.mpz, n_.mpz, cd);
          when Round.ZERO do ret=mpz_tdiv_r_ui(this.mpz, n_.mpz, cd);
        }
        if ncopy then delete n_;
      }
      return ret.safeCast(uint);
    }
    // this gets quotient, r gets remainder
    proc div_qr_ui(param rounding:Round, r:BigInt, n:BigInt, d:uint):uint
    {
      var ret:c_ulong;
      const cd = d.safeCast(c_ulong);
      on this {
        var (rcopy,r_) = r.maybeCopy();
        var (ncopy,n_) = n.maybeCopy();
        select rounding {
          when Round.UP   do ret=mpz_cdiv_qr_ui(this.mpz, r_.mpz, n_.mpz, cd);
          when Round.DOWN do ret=mpz_fdiv_qr_ui(this.mpz, r_.mpz, n_.mpz, cd);
          when Round.ZERO do ret=mpz_tdiv_qr_ui(this.mpz, r_.mpz, n_.mpz, cd);
        }
        if rcopy {
          r.set(r_);
          delete r_;
        }
        if ncopy then delete n_;
      }
      return ret.safeCast(uint);
    }
    proc div_ui(param rounding:Round, n:BigInt, d:uint):uint
    {
      var ret:c_ulong;
      const cd = d.safeCast(c_ulong);
      on this {
        var (ncopy,n_) = n.maybeCopy();
        select rounding {
          when Round.UP   do ret=mpz_cdiv_ui(this.mpz, n_.mpz, cd);
          when Round.DOWN do ret=mpz_fdiv_ui(this.mpz, n_.mpz, cd);
          when Round.ZERO do ret=mpz_tdiv_ui(this.mpz, n_.mpz, cd);
        }
        if ncopy then delete n_;
      }
      return ret.safeCast(uint);
    }
    proc div_q_2exp(param rounding:Round, n:BigInt, b:uint)
    {
      on this {
        var (ncopy,n_) = n.maybeCopy();
        const cb = b.safeCast(c_ulong);
        select rounding {
          when Round.UP   do mpz_cdiv_q_2exp(this.mpz, n_.mpz, cb);
          when Round.DOWN do mpz_fdiv_q_2exp(this.mpz, n_.mpz, cb);
          when Round.ZERO do mpz_tdiv_q_2exp(this.mpz, n_.mpz, cb);
        }
        if ncopy then delete n_;
      }
    }
    proc div_r_2exp(param rounding:Round, n:BigInt, b:uint)
    {
      on this {
        var (ncopy,n_) = n.maybeCopy();
        const cb = b.safeCast(c_ulong);
        select rounding {
          when Round.UP   do mpz_cdiv_r_2exp(this.mpz, n_.mpz, cb);
          when Round.DOWN do mpz_fdiv_r_2exp(this.mpz, n_.mpz, cb);
          when Round.ZERO do mpz_tdiv_r_2exp(this.mpz, n_.mpz, cb);
        }
        if ncopy then delete n_;
      }
    }
    proc mod(n:BigInt, d:BigInt)
    {
      on this {
        var (ncopy,n_) = n.maybeCopy();
        var (dcopy,d_) = d.maybeCopy();
        mpz_mod(this.mpz, n_.mpz, d_.mpz);
        if ncopy then delete n_;
        if dcopy then delete d_;
      }
    }
    proc mod_ui(n:BigInt, d:uint):uint
    {
      var ret:c_ulong;
      on this {
        var (ncopy,n_) = n.maybeCopy();
        ret=mpz_mod(this.mpz, n_.mpz, d.safeCast(c_ulong));
        if ncopy then delete n_;
      }
      return ret.safeCast(uint);
    }
    proc divexact(n:BigInt, d:BigInt)
    {
      on this {
        var (ncopy,n_) = n.maybeCopy();
        var (dcopy,d_) = d.maybeCopy();
        mpz_divexact(this.mpz, n_.mpz, d_.mpz);
        if ncopy then delete n_;
        if dcopy then delete d_;
      }
    }
    proc divexact_ui(n:BigInt, d:uint)
    {
      on this {
        var (ncopy,n_) = n.maybeCopy();
        mpz_divexact(this.mpz, n_.mpz, d.safeCast(c_ulong));
        if ncopy then delete n_;
      }
    }
    proc divisible_p(d:BigInt):int
    {
      var ret:c_int;
      on this {
        var (dcopy,d_) = d.maybeCopy();
        ret=mpz_divisible_p(this.mpz, d_.mpz);
        if dcopy then delete d_;
      }
      return ret.safeCast(int);
    }
    proc divisible_ui_p(d:uint):int
    {
      var ret:c_int;
      on this {
        ret=mpz_divisible_ui_p(this.mpz, d.safeCast(c_ulong));
      }
      return ret.safeCast(int);
    }
    proc divisible_2exp_p(b:uint):int
    {
      var ret:c_int;
      on this {
        mpz_divisible_2exp_p(this.mpz, b.safeCast(c_ulong));
      }
      return ret.safeCast(int);
    }
    proc congruent_p(c:BigInt, d:BigInt):int
    {
      var ret:c_int;
      on this {
        var (ccopy,c_) = c.maybeCopy();
        var (dcopy,d_) = d.maybeCopy();
        ret=mpz_congruent_p(this.mpz, c_.mpz, d_.mpz);
        if ccopy then delete c_;
        if dcopy then delete d_;
      }
      return ret.safeCast(int);
    }
    proc congruent_ui_p(c:uint, d:uint):int
    {
      var ret:c_int;
      on this {
        ret=mpz_congruent_ui_p(this.mpz, c.safeCast(c_ulong), d.safeCast(c_ulong));
      }
      return ret.safeCast(int);
    }
    proc congruent_2exp_p(c:BigInt, b:uint):int
    {
      var ret:c_int;
      on this {
        var (ccopy,c_) = c.maybeCopy();
        ret=mpz_congruent_2exp_p(this.mpz, c, b.safeCast(c_ulong));
        if ccopy then delete c_;
      }
      return ret.safeCast(int);
    }


    // Exponentiation Functions
    proc powm(base:BigInt, exp:BigInt, mod:BigInt)
    {
      on this {
        var (bcopy,b_) = base.maybeCopy();
        var (ecopy,e_) = exp.maybeCopy();
        var (mcopy,m_) = mod.maybeCopy();
        mpz_powm(this.mpz, b_.mpz, e_.mpz, m_.mpz);
        if bcopy then delete b_;
        if ecopy then delete e_;
        if mcopy then delete m_;
      }
    }
    proc powm_ui(base:BigInt, exp:uint, mod:BigInt)
    {
      on this {
        var (bcopy,b_) = base.maybeCopy();
        var (mcopy,m_) = mod.maybeCopy();
        mpz_powm_ui(this.mpz, b_.mpz, exp.safeCast(c_ulong), m_.mpz);
        if bcopy then delete b_;
        if mcopy then delete m_;
      }
    }
    proc pow_ui(base:BigInt, exp:uint)
    {
      on this {
        var (bcopy,b_) = base.maybeCopy();
        mpz_pow_ui(this.mpz, b_.mpz, exp.safeCast(c_ulong));
        if bcopy then delete b_;
      }
    }
    proc ui_pow_ui(base:uint, exp:uint)
    {
      on this {
        mpz_ui_pow_ui(this.mpz, base.safeCast(c_ulong), exp.safeCast(c_ulong));
      }
    }

    // Root Extraction Functions
    proc root(a:BigInt, n:uint):int
    {
      var ret:c_int;
      on this {
        var (acopy,a_) = a.maybeCopy();
        ret=mpz_root(this.mpz, a_.mpz, n.safeCast(c_ulong));
        if acopy then delete a_;
      }
      return ret.safeCast(int);
    }
    // this gets root, rem gets remainder.
    proc mpz_rootrem(rem:BigInt, u:BigInt, n:uint)
    {
      on this {
        var (rcopy,r_) = rem.maybeCopy();
        var (ucopy,u_) = u.maybeCopy();
        mpz_rootrem(this.mpz, r_.mpz, u_.mpz, n.safeCast(c_ulong));
        if rcopy {
          rem.set(r_);
          delete r_;
        }
        if ucopy then delete u_;
      }
    }
    proc sqrt(a:BigInt)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        mpz_sqrt(this.mpz, a_.mpz);
        if acopy then delete a_;
      }
    }
    // this gets root, rem gets remainder of a-root*root.
    proc sqrtrem(rem:BigInt, a:BigInt)
    {
      on this {
        var (rcopy,r_) = rem.maybeCopy();
        var (acopy,a_) = a.maybeCopy();
        mpz_sqrtrem(this.mpz, r_.mpz, a_.mpz);
        if rcopy {
          rem.set(r_);
          delete r_;
        }
        if acopy then delete a_;
      }
    }
    proc perfect_power_p():int
    {
      var ret:c_int;
      on this {
        ret=mpz_perfect_power_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc perfect_square():int
    {
      var ret:c_int;
      on this {
        ret=mpz_perfect_square(this.mpz);
      }
      return ret.safeCast(int);
    }

    // Number Theoretic Functions
    proc probab_prime_p(reps: int):int
    {
      var ret:c_int;
      on this {
        ret=mpz_probab_prime_p(this.mpz, reps.safeCast(c_int));
      }
      return ret.safeCast(int);
    }
    proc nextprime(a: BigInt)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        mpz_nextprime(this.mpz, a_.mpz);
        if acopy then delete a_;
      }
    }
    proc gcd(a: BigInt, b: BigInt)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        var (bcopy,b_) = b.maybeCopy();
        mpz_gcd(this.mpz, a_.mpz, b_.mpz);
        if acopy then delete a_;
        if bcopy then delete b_;
      }
    }
    proc gcd_ui(a: BigInt, b: uint)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        mpz_gcd_ui(this.mpz, a_.mpz, b.safeCast(c_ulong));
        if acopy then delete a_;
      }
    }
    // sets this to gcd(a,b)
    // set s and t to to coefficients satisfying a*s + b*t == g
    proc gcdext(s: BigInt, t: BigInt, a: BigInt, b: BigInt)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        var (bcopy,b_) = b.maybeCopy();
        var (scopy,s_) = s.maybeCopy();
        var (tcopy,t_) = t.maybeCopy();
        mpz_gcdext(this.mpz, s_.mpz, t_.mpz, a_.mpz, b_.mpz);
        if acopy then delete a_;
        if bcopy then delete b_;
        if scopy {
          s.set(s_);
          delete s_;
        }
        if tcopy {
          t.set(t_);
          delete t_;
        }
      }
    }
    proc lcm(a: BigInt, b: BigInt)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        var (bcopy,b_) = b.maybeCopy();
        mpz_lcm(this.mpz, a_.mpz, b_.mpz);
        if acopy then delete a_;
        if bcopy then delete b_;
      }
    }
    proc lcm_ui(a: BigInt, b: uint)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        mpz_lcm_ui(this.mpz, a_.mpz, b.safeCast(c_ulong));
        if acopy then delete a_;
      }
    }
    proc invert(a: BigInt, b: BigInt):int
    {
      var ret:c_int;
      on this {
        var (acopy,a_) = a.maybeCopy();
        var (bcopy,b_) = b.maybeCopy();
        ret=mpz_invert(this.mpz, a_.mpz, b_.mpz);
        if acopy then delete a_;
        if bcopy then delete b_;
      }
      return ret.safeCast(int);
    }

    // jacobi, legendre, kronecker are procedures outside this class.

    proc remove(a: BigInt, f: BigInt):uint
    {
      var ret:c_ulong;
      on this {
        var (acopy,a_) = a.maybeCopy();
        var (fcopy,f_) = f.maybeCopy();
        ret=mpz_remove(this.mpz, a_.mpz, f_.mpz);
        if acopy then delete a_;
        if fcopy then delete f_;
      }
      return ret.safeCast(uint);
    }
    proc fac_ui(a: uint)
    {
      on this {
        mpz_fac_ui(this.mpz, a.safeCast(c_ulong));
      }
    }
    proc bin_ui(n: BigInt, k: uint)
    {
      on this {
        var (ncopy,n_) = n.maybeCopy();
        mpz_bin_ui(this.mpz, n_.mpz, k.safeCast(c_ulong));
        if ncopy then delete n_;
      }
    }
    proc bin_uiui(n: uint, k: uint)
    {
      on this {
        mpz_bin_uiui(this.mpz, n.safeCast(c_ulong), k.safeCast(c_ulong));
      }
    }
    proc fib_ui(n: uint)
    {
      on this {
        mpz_fib_ui(this.mpz, n.safeCast(c_ulong));
      }
    }
    proc fib2_ui(fnsub1: BigInt, n: uint)
    {
      on this {
        var (fcopy,f_) = fnsub1.maybeCopy();
        mpz_fib2_ui(this.mpz, f_.mpz, n.safeCast(c_ulong));
        if fcopy {
          fnsub1.set(f_);
          delete f_;
        }
      }
    }
    proc lucnum_ui(n: uint)
    {
      on this {
        mpz_lucnum_ui(this.mpz, n.safeCast(c_ulong));
      }
    }
    proc lucnum2_ui(lnsub1: BigInt, n: uint)
    {
      on this {
        var (fcopy,f_) = lnsub1.maybeCopy();
        mpz_lucnum2_ui(this.mpz, f_.mpz, n.safeCast(c_ulong));
        if fcopy {
          lnsub1.set(f_);
          delete f_;
        }
      }
    }
 

    // Comparison Functions
    proc cmp(b:BigInt):int
    {
      var ret:c_int;
      on this {
        var (bcopy,b_) = b.maybeCopy();
        ret=mpz_cmp(this.mpz,b_.mpz);
        if bcopy then delete b_;
      }
      return ret.safeCast(int);
    }
    proc cmp_d(b:real):int
    {
      var ret:c_int;
      on this {
        ret=mpz_cmp_d(this.mpz, b: c_double);
      }
      return ret.safeCast(int);
    }
    proc cmp_si(b:int):int
    {
      var ret:c_int;
      on this {
        ret=mpz_cmp_si(this.mpz, b.safeCast(c_long));
      }
      return ret.safeCast(int);
    }
    proc cmp_ui(b:uint):int
    {
      var ret:c_int;
      on this {
        ret=mpz_cmp_ui(this.mpz, b.safeCast(c_ulong));
      }
      return ret.safeCast(int);
    }
    proc cmpabs(b:BigInt):int
    {
      var ret:c_int;
      on this {
        var (acopy,b_) = b.maybeCopy();
        ret=mpz_cmpabs(this.mpz,b_.mpz);
        if acopy then delete b_;
      }
      return ret.safeCast(int);
    }
    proc cmpabs_d(b:real):int
    {
      var ret:c_int;
      on this {
        ret=mpz_cmpabs_d(this.mpz, b: c_double);
      }
      return ret.safeCast(int);
    }
    proc cmp_abs_ui(b:uint):int
    {
      var ret:c_int;
      on this {
        ret=mpz_cmpabs_ui(this.mpz, b.safeCast(c_ulong));
      }
      return ret.safeCast(int);
    }
    proc sgn():int
    {
      var ret:c_int;
      on this {
        ret=mpz_sgn(this.mpz);
      }
      return ret.safeCast(int);
    }

    // Logical and Bit Manipulation Functions
    proc and(a:BigInt, b:BigInt)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        var (bcopy,b_) = b.maybeCopy();
        mpz_and(this.mpz, a_.mpz, b_.mpz);
        if acopy then delete a_;
        if bcopy then delete b_;
      }
    }
    proc ior(a:BigInt, b:BigInt)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        var (bcopy,b_) = b.maybeCopy();
        mpz_ior(this.mpz, a_.mpz, b_.mpz);
        if acopy then delete a_;
        if bcopy then delete b_;
      }
    }
    proc xor(a:BigInt, b:BigInt)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        var (bcopy,b_) = b.maybeCopy();
        mpz_xor(this.mpz, a_.mpz, b_.mpz);
        if acopy then delete a_;
        if bcopy then delete b_;
      }
    }
    proc com(a:BigInt)
    {
      on this {
        var (acopy,a_) = a.maybeCopy();
        mpz_com(this.mpz, a_.mpz);
        if acopy then delete a_;
      }
    }
    proc popcount():uint
    {
      var ret:c_ulong;
      on this {
        ret = mpz_popcount(this.mpz);
      }
      return ret.safeCast(uint);
    }
    proc hamdist(b:BigInt):uint
    {
      var ret:c_ulong;
      on this {
        var (bcopy,b_) = b.maybeCopy();
        ret=mpz_hamdist(this.mpz, b_.mpz);
        if bcopy then delete b_;
      }
      return ret.safeCast(uint);
    }
    proc scan0(starting_bit:uint):uint
    {
      var ret:c_ulong;
      on this {
        ret = mpz_scan0(this.mpz, starting_bit.safeCast(c_ulong));
      }
      return ret.safeCast(uint);
    }
    proc scan1(starting_bit:uint):uint
    {
      var ret:c_ulong;
      on this {
        ret = mpz_scan1(this.mpz, starting_bit.safeCast(c_ulong));
      }
      return ret.safeCast(uint);
    }
    proc setbit(bit_index:uint)
    {
      on this {
        mpz_setbit(this.mpz, bit_index.safeCast(c_ulong));
      }
    }
    proc clrbit(bit_index:uint)
    {
      on this {
        mpz_clrbit(this.mpz, bit_index.safeCast(c_ulong));
      }
    }
    proc combit(bit_index:uint)
    {
      on this {
        mpz_combit(this.mpz, bit_index.safeCast(c_ulong));
      }
    }
    proc tstbit(bit_index:uint):int
    {
      var ret:c_int;
      on this {
        ret = mpz_tstbit(this.mpz, bit_index.safeCast(c_ulong));
      }
      return ret.safeCast(int);
    }

    // Miscellaneous Functions
    proc fits_ulong_p():int
    {
      var ret:c_int;
      on this {
        ret = mpz_fits_ulong_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc fits_slong_p():int
    {
      var ret:c_int;
      on this {
        ret = mpz_fits_ulong_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc fits_uint_p():int
    {
      var ret:c_int;
      on this {
        ret = mpz_fits_uint_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc fits_sint_p():int
    {
      var ret:c_int;
      on this {
        ret = mpz_fits_sint_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc fits_ushort_p():int
    {
      var ret:c_int;
      on this {
        ret = mpz_fits_ushort_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc fits_sshort_p():int
    {
      var ret:c_int;
      on this {
        ret = mpz_fits_sshort_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc odd_p():int
    {
      var ret:c_int;
      on this {
        ret = mpz_odd_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc even_p():int
    {
      var ret:c_int;
      on this {
        ret = mpz_even_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc sizeinbase(base:int):uint
    {
      var ret:size_t;
      on this {
        ret = mpz_sizeinbase(this.mpz, base.safeCast(c_int));
      }
      return ret.safeCast(uint);
    }

    // left out integer random functions
    // these are in the GMPRandom class.

    proc realloc2(nbits:uint)
    {
      on this {
        mpz_realloc2(this.mpz, nbits.safeCast(c_ulong));
      }
    }
    proc get_limbn(n:uint):uint
    {
      var ret:mp_limb_t;
      on this {
        ret = mpz_getlimbn(this.mpz, n.safeCast(mp_size_t));
      }
      return ret.safeCast(uint);
    }
    proc size():size_t
    {
      var ret:size_t;
      on this {
        ret = mpz_size(this.mpz);
      }
      return ret;
    }
    proc debugprint() {
      writeln("On locale ",this.locale);
      on this {
        chpl_gmp_mpz_print(this.mpz);
      }
    }
  }


  proc BigInt.writeThis(writer:Writer) {
    var (acopy,a_) = this.maybeCopy();
    var s:string = a_.get_str();
    //gmp_asprintf(s, "%Zd", a_.mpz);
    writer.write(s);
    if acopy then delete a_;
  }


  proc jacobi(a: BigInt, b: BigInt):int
  {
    var ret:c_int;
    on a {
      var (bcopy,b_) = b.maybeCopy();
      ret=mpz_jacobi(a.mpz, b_.mpz);
      if bcopy then delete b_;
    }
    return ret.safeCast(int);
  }
  proc legendre(a: BigInt, p: BigInt):int
  {
    var ret:c_int;
    on a {
      var (pcopy,p_) = p.maybeCopy();
      ret=mpz_legendre(a.mpz, p_.mpz);
      if pcopy then delete p_;
    }
    return ret.safeCast(int);
  }
  proc kronecker(a: BigInt, b: BigInt):int
  {
    var ret:c_int;
    on a {
      var (bcopy,b_) = b.maybeCopy();
      ret=mpz_kronecker(a.mpz, b_.mpz);
      if bcopy then delete b_;
    }
    return ret.safeCast(int);
  }
  proc kronecker_si(a: BigInt, b: int):int
  {
    var ret:c_int;
    on a {
      ret=mpz_kronecker_si(a.mpz, b.safeCast(c_long));
    }
    return ret.safeCast(int);
  }
  proc kronecker_ui(a: BigInt, b: uint):int
  {
    var ret:c_int;
    on a {
      ret=mpz_kronecker_ui(a.mpz, b.safeCast(c_ulong));
    }
    return ret.safeCast(int);
  }
  proc si_kronecker(a: int, b: BigInt):int
  {
    var ret:c_int;
    on b {
      ret=mpz_si_kronecker(b.mpz.safeCast(c_long, a));
    }
    return ret.safeCast(int);
  }
  proc ui_kronecker(a: uint, b: BigInt):int
  {
    var ret:c_int;
    on b {
      ret=mpz_ui_kronecker(b.mpz.safeCast(c_ulong, a));
    }
    return ret.safeCast(int);
  }

  class GMPRandom {
    var state: gmp_randstate_t;
    proc GMPRandom()
    {
      gmp_randinit_default(this.state);
    }
    // Creates a Mersenne Twister (probably same as init_default)
    proc GMPRandom(twister:bool)
    {
      gmp_randinit_mt(this.state);
    }
    proc GMPRandom(a: BigInt, c: uint, m2exp: uint)
    {
      var (acopy,a_) = a.maybeCopy();
      gmp_randinit_lc_2exp(this.state, a_.mpz, c.safeCast(c_ulong), m2exp.safeCast(c_ulong));
      if acopy then delete a_;
    }
    proc GMPRandom(size: uint)
    {
      gmp_randinit_lc_2exp_esize(this.state, size.safeCast(c_ulong));
    }
    proc GMPRandom(a: GMPRandom)
    {
      if a.locale == here {
        gmp_randinit_set(this.state, a.state);
      } else {
        chpl_gmp_get_randstate(this.state, a.locale.id, a.state[1]);
      }
    }
    proc ~GMPRandom()
    {
      on this do gmp_randclear(this.state);
    }
    proc seed(seed: BigInt)
    {
      on this {
        var (scopy,s_) = seed.maybeCopy();
        gmp_randseed(this.state, s_.mpz);
        if scopy then delete s_;
      }
    }
    proc seed(seed: uint)
    {
      on this {
        gmp_randseed_ui(this.state, seed.safeCast(c_ulong));
      }
    }
    proc urandomb_ui(nbits: uint):uint
    {
      var ret: c_ulong;
      on this {
        ret=gmp_urandomb_ui(this.state, nbits.safeCast(c_ulong));
      }
      return ret.safeCast(uint);
    }
    proc urandomm_ui(n: uint):uint
    {
      var ret: c_ulong;
      on this {
        ret=gmp_urandomm_ui(this.state, n.safeCast(c_ulong));
      }
      return ret.safeCast(uint);
    }
    proc urandomb(r: BigInt, nbits: uint)
    {
      on this {
        var (rcopy,r_) = r.maybeCopy();
        mpz_urandomb(r_.mpz, this.state, nbits.safeCast(c_ulong));
        if rcopy {
          r.set(r_);
          delete r_;
        }
      }
    }
    proc urandomm(r: BigInt, n: BigInt)
    {
      on this {
        var (rcopy,r_) = r.maybeCopy();
        var (ncopy,n_) = n.maybeCopy();
        mpz_urandomm(r_.mpz, this.state, n_.mpz);
        if rcopy {
          r.set(r_);
          delete r_;
        }
        if ncopy then delete n_;
      }
    }
    proc rrandomb(r: BigInt, nbits: uint)
    {
      on this {
        var (rcopy,r_) = r.maybeCopy();
        mpz_rrandomb(r_.mpz, this.state, nbits.safeCast(c_ulong));
        if rcopy {
          r.set(r_);
          delete r_;
        }
      }
    }


  }

  /* FUTURE -- GMP numbers with record semantics,
      expression and operator overloads.
      */

  // calls mp_set_memory_functions to use chpl_malloc, etc.
  chpl_gmp_init();

} /* end of module */
