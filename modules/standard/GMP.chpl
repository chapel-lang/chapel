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

/*

Support for GNU Multiple Precision Arithmetic

This module provides a low-level interface to a substantial fraction
of the GMP library (the GNU Multiple Precision arithmetic library).
This support includes the C types for GMP integers, floating point
numbers, and random numbers, and nearly every operation on those
types. These types and functions enable efficient multi-precision
computation within a single locale.  See the `GMP homepage
<https://gmplib.org/>`_ for more information on this library.

The module :mod:`BigInteger` leverages this interface to define the
record :record:`~BigInteger.bigint`.  The methods on the record
:record:`~BigInteger.bigint` are locale aware so that Chapel programs
can, for example, create a distributed array of GMP integers. That
record also provides operator overloads for the standard arithmetic
and assignment operators which tend to enable a more natural
expression of some algorithms.  Please see the documentation in
:mod:`BigInteger` for details.

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
  Start using the supported subset of GMP types and routines defined
  in this module or the bigint record (see :mod:`BigInteger`).


Calling GMP functions directly
------------------------------

The low-level option for Chapel programs using multi-precision numbers
is to the GMP functions directly. For a full reference to GMP capabilities,
please refer to the `GMP website <https://gmplib.org>`_ and the
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
  use SysError;
  use BigInteger;
  private use SysCTypes;

  require "GMPHelper/chplgmp.h";

  proc chpl_gmp_alloc(size:size_t) : c_void_ptr {
    pragma "insert line file info"
    extern proc chpl_mem_alloc(size:size_t, md:chpl_mem_descInt_t) : c_void_ptr;
    extern const CHPL_RT_MD_GMP:chpl_mem_descInt_t;
    return chpl_mem_alloc(size, CHPL_RT_MD_GMP);
  }

  proc chpl_gmp_realloc(ptr:c_void_ptr,
                               old_size:size_t, new_size:size_t) : c_void_ptr {
    pragma "insert line file info"
    extern proc chpl_mem_realloc(ptr:c_void_ptr, size:size_t, md:chpl_mem_descInt_t) : c_void_ptr;
    extern const CHPL_RT_MD_GMP:chpl_mem_descInt_t;
    return chpl_mem_realloc(ptr, new_size, CHPL_RT_MD_GMP);
  }

  proc chpl_gmp_free(ptr:c_void_ptr, old_size:size_t) {
    pragma "insert line file info"
      extern proc chpl_mem_free(ptr:c_void_ptr) : void;
    chpl_mem_free(ptr);
  }

  //
  // Initialize GMP to use Chapel's allocator
  //
  proc chpl_gmp_init() {
    extern proc chpl_gmp_mp_set_memory_functions(alloc:c_fn_ptr,
                                                 realloc:c_fn_ptr,
                                                 free:c_fn_ptr);
    chpl_gmp_mp_set_memory_functions(c_ptrTo(chpl_gmp_alloc),
                                     c_ptrTo(chpl_gmp_realloc),
                                     c_ptrTo(chpl_gmp_free));
  }

  // Initialize GMP library on all locales
  coforall loc in Locales {
    on loc {
      chpl_gmp_init();
    }
  }

  /* The GMP ``mp_bitcnt_t`` type */
  extern type mp_bitcnt_t     = c_ulong;

  /* The GMP ``mp_size_t``   type */
  extern type mp_size_t       = c_long;

  /* The GMP ``mp_limb_t``   type. */
  /* This is normally uint(64) but can depend on configuration. */
  extern type mp_limb_t;

  /* The GMP `mp_bits_per_limb`` constant */
  extern const mp_bits_per_limb: c_int;


  //
  // GMP represents a multi-precision integer as an __mpz_struct.
  // This is treated as an internal type by GMP and is not intended to
  // used directly by C developers.  Chapel treats this as an opaque object.
  //
  // In practice the implementation is a dynamically-sized packed vector
  // of platform-specific integers i.e.
  //
  //     typedef struct {
  //       int        _mp_alloc;  // capacity
  //       int        _mp_size;   // current size (sign is sign of the mpz)
  //       mp_limb_t* _mp_d;      // a packed vector of integers
  //     } __mpz_struct;
  //
  //
  // GMP then defines the type mpz_t as
  //
  //     typedef __mpz_struct mpz_t[1];
  //
  // When used to define a C value, this is simply a single __mpz_struct.
  // As an argument to a function, this causes the value to be passed by
  // reference rather than by value as would be expected for a struct.
  //
  //
  // For single locale applications the application programmer is
  // responsible for ensuring that every mpz_t is initialized correctly
  // and that it is cleared/freed when it is no longer needed.
  //
  // For multi-locale applications the application programmer must be aware
  // that Chapel creates shallow copies of this data structure within
  // on-statements; the _mp_d value will not be valid on the remote locale.
  // The developer may invoke chpl_gmp_get_mpz() to create a local copy of
  // the actual GMP data.
  //

  pragma "no doc"
  extern type __mpz_struct;

  /* The GMP ``mpz_t`` type */
  extern type mpz_t           = 1 * __mpz_struct;

  pragma "no doc"
  extern type __mpf_struct;

  /*  The GMP ``mpf_t`` type */
  extern type mpf_t           = 1 * __mpf_struct;

  pragma "no doc"
  extern type __gmp_randstate_struct;


  /* The GMP ``gmp_randstate_t`` type */
  extern type gmp_randstate_t = 1 * __gmp_randstate_struct;

  //
  // The organization of the following interfaces is aligned with
  //
  //      https://gmplib.org/manual/index.html
  //
  // for version 6.1.1


  //
  // 5 Integer Functions
  //

  //
  // 5.1 Initializing Functions
  //

  /* */
  extern proc mpz_init(ref x: mpz_t);

  extern proc mpz_init2(ref x: mpz_t, n: mp_bitcnt_t);

  extern proc mpz_clear(ref x: mpz_t);

  extern proc _mpz_realloc(ref x: mpz_t, new_alloc: mp_size_t);
  extern proc mpz_realloc2(ref x: mpz_t, n: mp_bitcnt_t);


  //
  // 5.2 Assignment Functions
  //

  extern proc mpz_set(ref rop: mpz_t, const ref op: mpz_t);

  extern proc mpz_set_ui(ref rop: mpz_t, op: c_ulong);

  extern proc mpz_set_si(ref rop: mpz_t, op: c_long);

  extern proc mpz_set_d(ref rop: mpz_t, op: c_double);

  extern proc mpz_set_str(ref rop: mpz_t, str: c_string, base: c_int);

  extern proc mpz_swap(ref rop1: mpz_t, ref rop2: mpz_t);


  //
  // 5.3 Combined Initialization and Assignment Functions
  //

  extern proc mpz_init_set(ref rop: mpz_t, const ref op: mpz_t);

  extern proc mpz_init_set_ui(ref rop: mpz_t, op: c_ulong);

  extern proc mpz_init_set_si(ref rop: mpz_t, op: c_long);

  extern proc mpz_init_set_d(ref rop: mpz_t, op: c_double);

  extern proc mpz_init_set_str(ref rop: mpz_t,
                               str: c_string,
                               base: c_int) : c_int;


  //
  // 5.4 Conversion Functions
  //

  extern proc mpz_get_ui(const ref op: mpz_t) : c_ulong;

  extern proc mpz_get_si(const ref op: mpz_t) : c_long;

  extern proc mpz_get_d(const ref op: mpz_t) : c_double;

  extern proc mpz_get_d_2exp(ref exp: c_long,
                             const ref op: mpz_t) : c_double;

  extern proc mpz_get_str(str: c_string,
                          base: c_int,
                          const ref op: mpz_t) : c_string;


  //
  // 5.5 Arithmetic Functions
  //

  extern proc mpz_add(ref rop: mpz_t,
                      const ref op1: mpz_t,
                      const ref op2: mpz_t);

  extern proc mpz_add_ui(ref rop: mpz_t,
                         const ref op1: mpz_t,
                         op2: c_ulong);

  extern proc mpz_sub(ref rop: mpz_t,
                      const ref op1: mpz_t,
                      const ref op2: mpz_t);

  extern proc mpz_sub_ui(ref rop: mpz_t,
                         const ref op1: mpz_t,
                         op2: c_ulong);

  extern proc mpz_ui_sub(ref rop: mpz_t,
                         op1: c_ulong,
                         const ref op2: mpz_t);

  extern proc mpz_mul(ref rop: mpz_t,
                      const ref op1: mpz_t,
                      const ref op2: mpz_t);

  extern proc mpz_mul_si(ref rop: mpz_t,
                         const ref op1: mpz_t,
                         op2: c_long);

  extern proc mpz_mul_ui(ref rop: mpz_t,
                         const ref op1: mpz_t,
                         op2: c_ulong);

  extern proc mpz_addmul(ref rop: mpz_t,
                         const ref op1: mpz_t,
                         const ref op2: mpz_t);

  extern proc mpz_addmul_ui(ref rop: mpz_t,
                            const ref op1: mpz_t,
                            op2: c_ulong);

  extern proc mpz_submul(ref rop: mpz_t,
                         const ref op1: mpz_t,
                         const ref op2: mpz_t);

  extern proc mpz_submul_ui(ref rop: mpz_t,
                            const ref op1: mpz_t,
                            op2: c_ulong);

  extern proc mpz_mul_2exp(ref rop: mpz_t,
                           const ref op1: mpz_t,
                           op2: mp_bitcnt_t);

  extern proc mpz_neg(ref rop: mpz_t,
                      const ref op: mpz_t);

  extern proc mpz_abs(ref rop: mpz_t,
                      const ref op: mpz_t);


  //
  // 5.6 Division Functions
  //

  extern proc mpz_cdiv_q(ref q: mpz_t,
                         const ref n: mpz_t,
                         const ref d: mpz_t);

  extern proc mpz_cdiv_r(ref r: mpz_t,
                         const ref n: mpz_t,
                         const ref d: mpz_t);

  extern proc mpz_cdiv_qr(ref q: mpz_t,
                          ref r: mpz_t,
                          const ref n: mpz_t,
                          const ref d: mpz_t);

  extern proc mpz_cdiv_q_ui(ref q: mpz_t,
                            const ref n: mpz_t,
                            d: c_ulong) : c_ulong;

  extern proc mpz_cdiv_r_ui(ref r: mpz_t,
                            const ref n: mpz_t,
                            d: c_ulong) : c_ulong;

  extern proc mpz_cdiv_qr_ui(ref q: mpz_t,
                             ref r: mpz_t,
                             const ref n: mpz_t,
                             d: c_ulong) : c_ulong;

  extern proc mpz_cdiv_ui(const ref n: mpz_t,
                          d: c_ulong) : c_ulong;

  extern proc mpz_cdiv_q_2exp(ref q: mpz_t,
                              const ref n: mpz_t,
                              b: mp_bitcnt_t);

  extern proc mpz_cdiv_r_2exp(ref r: mpz_t,
                              const ref n: mpz_t,
                              b: mp_bitcnt_t);


  //
  extern proc mpz_fdiv_q(ref q: mpz_t,
                         const ref n: mpz_t,
                         const ref d: mpz_t);

  extern proc mpz_fdiv_r(ref r: mpz_t,
                         const ref n: mpz_t,
                         const ref d: mpz_t);

  extern proc mpz_fdiv_qr(ref q: mpz_t,
                          ref r: mpz_t,
                          const ref n: mpz_t,
                          const ref d: mpz_t);

  extern proc mpz_fdiv_q_ui(ref q: mpz_t,
                            const ref n: mpz_t,
                            d: c_ulong) : c_ulong;

  extern proc mpz_fdiv_r_ui(ref r: mpz_t,
                            const ref n: mpz_t,
                            d: c_ulong) : c_ulong;

  extern proc mpz_fdiv_qr_ui(ref q: mpz_t,
                             ref r: mpz_t,
                             const ref n: mpz_t,
                             d: c_ulong) : c_ulong;

  extern proc mpz_fdiv_ui(const ref n: mpz_t,
                          d: c_ulong) : c_ulong;

  extern proc mpz_fdiv_q_2exp(ref q: mpz_t,
                              const ref n: mpz_t,
                              b: mp_bitcnt_t);

  extern proc mpz_fdiv_r_2exp(ref r: mpz_t,
                              const ref n: mpz_t,
                              b: mp_bitcnt_t);


  //
  extern proc mpz_tdiv_q(ref q: mpz_t,
                         const ref n: mpz_t,
                         const ref d: mpz_t);

  extern proc mpz_tdiv_r(ref r: mpz_t,
                         const ref n: mpz_t,
                         const ref d: mpz_t);

  extern proc mpz_tdiv_qr(ref q: mpz_t,
                          ref r: mpz_t,
                          const ref n: mpz_t,
                          const ref d: mpz_t);

  extern proc mpz_tdiv_q_ui(ref q: mpz_t,
                            const ref n: mpz_t,
                            d: c_ulong) : c_ulong;

  extern proc mpz_tdiv_r_ui(ref r: mpz_t,
                            const ref n: mpz_t,
                            d: c_ulong) : c_ulong;

  extern proc mpz_tdiv_qr_ui(ref q: mpz_t,
                             ref r: mpz_t,
                             const ref n: mpz_t,
                             d: c_ulong) : c_ulong;

  extern proc mpz_tdiv_ui(const ref n: mpz_t,
                          d: c_ulong) : c_ulong;

  extern proc mpz_tdiv_q_2exp(ref q: mpz_t,
                              const ref n: mpz_t,
                              b: mp_bitcnt_t);

  extern proc mpz_tdiv_r_2exp(ref r: mpz_t,
                              const ref n: mpz_t,
                              b: mp_bitcnt_t);


  //
  extern proc mpz_mod(ref rop: mpz_t,
                      const ref n: mpz_t,
                      const ref d: mpz_t);

  extern proc mpz_mod_ui(ref rop: mpz_t,
                         const ref n: mpz_t,
                         d: c_ulong) : c_ulong;

  extern proc mpz_divexact(ref q: mpz_t,
                           const ref n: mpz_t,
                           const ref d: mpz_t);

  extern proc mpz_divexact_ui(ref q: mpz_t,
                              const ref n: mpz_t,
                              d: c_ulong);

  extern proc mpz_divisible_p(const ref n: mpz_t,
                              const ref d: mpz_t) : c_int;

  extern proc mpz_divisible_ui_p(const ref n: mpz_t,
                                 d: c_ulong) : c_int;

  extern proc mpz_divisible_2exp_p(const ref n: mpz_t,
                                   b: mp_bitcnt_t) : c_int;

  extern proc mpz_congruent_p(const ref n: mpz_t,
                              const ref c: mpz_t,
                              const ref d: mpz_t) : c_int;

  extern proc mpz_congruent_ui_p(const ref n: mpz_t,
                                 c: c_ulong,
                                 d: c_ulong) : c_int;

  extern proc mpz_congruent_2exp_p(const ref n: mpz_t,
                                   const ref c: mpz_t,
                                   b: mp_bitcnt_t) : c_int;


  //
  // 5.7 Exponentiation Functions
  //

  extern proc mpz_powm(ref rop: mpz_t,
                       const ref base: mpz_t,
                       const ref exp: mpz_t,
                       const ref mod: mpz_t);

  extern proc mpz_powm_ui(ref rop: mpz_t,
                          const ref base: mpz_t,
                          exp: c_ulong,
                          const ref mod: mpz_t);

  extern proc mpz_powm_sec(ref rop: mpz_t,
                           const ref base: mpz_t,
                           const ref exp: mpz_t,
                           const ref mod:  mpz_t);

  extern proc mpz_pow_ui(ref rop: mpz_t,
                         const ref base: mpz_t,
                         exp: c_ulong);

  extern proc mpz_ui_pow_ui(ref rop: mpz_t,
                            base: c_ulong,
                            exp: c_ulong);


  //
  // 5.8 Root Extraction Functions
  //

  extern proc mpz_root(ref rop: mpz_t,
                       const ref op: mpz_t,
                       n: c_ulong) : c_int;

  extern proc mpz_rootrem(ref root: mpz_t,
                          ref rem: mpz_t,
                          const ref u: mpz_t,
                          n: c_ulong);

  extern proc mpz_sqrt(ref rop: mpz_t,
                       const ref op: mpz_t);

  extern proc mpz_sqrtrem(ref rop1: mpz_t,
                          ref rop2: mpz_t,
                          const ref op: mpz_t);

  extern proc mpz_perfect_power_p(const ref op: mpz_t) : c_int;

  extern proc mpz_perfect_square_p(const ref op: mpz_t) : c_int;


  //
  // 5.9 Number Theoretic Functions
  //

  extern proc mpz_probab_prime_p(ref n: mpz_t,
                                 reps: c_int) : c_int;

  extern proc mpz_nextprime(ref rop: mpz_t,
                            const ref op: mpz_t);

  extern proc mpz_gcd(ref rop: mpz_t,
                      const ref op1: mpz_t,
                      const ref op2: mpz_t);

  extern proc mpz_gcd_ui(ref rop: mpz_t,
                         const ref op1: mpz_t,
                         op2: c_ulong);

  extern proc mpz_gcdext(ref g: mpz_t,
                         ref s: mpz_t,
                         ref t: mpz_t,
                         const ref a: mpz_t,
                         const ref b: mpz_t);

  extern proc mpz_lcm(ref rop: mpz_t,
                      const ref op1: mpz_t,
                      const ref op2: mpz_t);

  extern proc mpz_lcm_ui(ref rop: mpz_t,
                         const ref op1: mpz_t,
                         op2: c_ulong);

  extern proc mpz_invert(ref rop: mpz_t,
                         const ref op1: mpz_t,
                         const ref op2: mpz_t) : c_int;

  extern proc mpz_jacobi(const ref a: mpz_t,
                         const ref b: mpz_t) : c_int;

  extern proc mpz_legendre(const ref a: mpz_t,
                           const ref p: mpz_t) : c_int;

  extern proc mpz_kronecker(const ref a: mpz_t,
                            const ref b: mpz_t) : c_int;

  extern proc mpz_kronecker_si(const ref a: mpz_t,
                               b: c_long) : c_int;

  extern proc mpz_kronecker_ui(const ref a: mpz_t,
                               b: c_ulong) : c_int;

  extern proc mpz_si_kronecker(a: c_long,
                               const ref b: mpz_t) : c_int;

  extern proc mpz_ui_kronecker(a: c_ulong,
                               const ref b: mpz_t) : c_int;

  extern proc mpz_remove(ref rop: mpz_t,
                         const ref op: mpz_t,
                         const ref f: mpz_t) : c_ulong;

  extern proc mpz_fac_ui(ref rop: mpz_t,
                         n: c_ulong);

  extern proc mpz_2fac_ui(ref rop: mpz_t,
                          n: c_ulong);

  extern proc mpz_mfac_uiui(ref rop: mpz_t,
                            n: c_ulong,
                            m: c_ulong);

  extern proc mpz_primorial_ui(ref rop: mpz_t,
                               n: c_ulong);

  extern proc mpz_bin_ui(ref rop: mpz_t,
                         const ref n: mpz_t,
                         k: c_ulong);

  extern proc mpz_bin_uiui(ref rop: mpz_t,
                           n: c_ulong,
                           k: c_ulong);

  extern proc mpz_fib_ui(ref fn: mpz_t,
                         n: c_ulong);

  extern proc mpz_fib2_ui(ref fn: mpz_t,
                          ref fnsub1: mpz_t,
                          n: c_ulong);

  extern proc mpz_lucnum_ui(ref ln: mpz_t,
                            n: c_ulong);

  extern proc mpz_lucnum2_ui(ref ln: mpz_t,
                             ref lnsub1: mpz_t,
                             n: c_ulong);


  //
  // 5.10 Comparison Functions
  //

  extern proc mpz_cmp(const ref op1: mpz_t,
                      const ref op2: mpz_t) : c_int;

  extern proc mpz_cmp_d(const ref op1: mpz_t,
                        op2: c_double) : c_int;

  extern "chpl_mpz_cmp_si"
         proc mpz_cmp_si(const ref op1: mpz_t,
                         op2: c_long) : c_int;

  extern "chpl_mpz_cmp_ui"
         proc mpz_cmp_ui(const ref op1: mpz_t,
                         op2: c_ulong) : c_int;

  extern proc mpz_cmpabs(const ref op1: mpz_t,
                         const ref op2: mpz_t) : c_int;

  extern proc mpz_cmpabs_d(const ref op1: mpz_t,
                           op2: c_double) : c_int;

  extern proc mpz_cmpabs_ui(const ref op1: mpz_t,
                            op2: c_ulong) : c_int;

  extern "chpl_mpz_sgn"
         proc mpz_sgn(const ref op: mpz_t) : c_int;


  //
  // 5.11 Logical and Bit Manipulation Functions
  //

  extern proc mpz_and(ref rop: mpz_t,
                      const ref op1: mpz_t,
                      const ref op2: mpz_t);

  extern proc mpz_ior(ref rop: mpz_t,
                      const ref op1: mpz_t,
                      const ref op2: mpz_t);

  extern proc mpz_xor(ref rop: mpz_t,
                      const ref op1: mpz_t,
                      const ref op2: mpz_t);

  extern proc mpz_com(ref rop: mpz_t,
                      const ref op: mpz_t);

  extern proc mpz_popcount(const ref op: mpz_t) : c_ulong;

  extern proc mpz_hamdist(const ref op1: mpz_t,
                          const ref op2: mpz_t) : c_ulong;

  extern proc mpz_scan0(const ref op: mpz_t,
                        starting_bit: mp_bitcnt_t) : c_ulong;

  extern proc mpz_scan1(const ref op: mpz_t,
                        starting_bit: mp_bitcnt_t) : c_ulong;

  extern proc mpz_setbit(ref rop: mpz_t,
                         bit_index: mp_bitcnt_t);

  extern proc mpz_clrbit(ref rop: mpz_t,
                         bit_index: mp_bitcnt_t);

  extern proc mpz_combit(ref rop: mpz_t,
                         bit_index: mp_bitcnt_t);

  extern proc mpz_tstbit(const ref op: mpz_t,
                         bit_index: mp_bitcnt_t) : c_int;


  //
  // 5.12 Input and Output Functions
  //


  //
  // 5.13 Random Number Functions
  //

  extern proc mpz_urandomb(ref rop: mpz_t,
                           ref state: gmp_randstate_t,
                           n: mp_bitcnt_t);

  extern proc mpz_urandomm(ref rop: mpz_t,
                           ref state: gmp_randstate_t,
                           const ref n: mpz_t);

  extern proc mpz_rrandomb(ref rop: mpz_t,
                           ref state: gmp_randstate_t,
                           n: mp_bitcnt_t);

  extern proc mpz_random(ref rop: mpz_t,
                         max_size: mp_size_t);

  extern proc mpz_random2(ref rop: mpz_t,
                         max_size: mp_size_t);


  //
  // 5.14 Integer Import and Export
  //


  //
  // 5.15 Miscellaneous Functions
  //

  extern proc mpz_fits_ulong_p(const ref op: mpz_t) : c_int;

  extern proc mpz_fits_slong_p(const ref op: mpz_t) : c_int;

  extern proc mpz_fits_uint_p(const ref op: mpz_t) : c_int;

  extern proc mpz_fits_sint_p(const ref op: mpz_t) : c_int;

  extern proc mpz_fits_ushort_p(const ref op: mpz_t) : c_int;

  extern proc mpz_fits_sshort_p(const ref op: mpz_t) : c_int;

  extern "chpl_mpz_odd_p"
         proc mpz_odd_p(const ref op: mpz_t) : c_int;

  extern "chpl_mpz_even_p"
         proc mpz_even_p(const ref op: mpz_t) : c_int;

  extern proc mpz_sizeinbase(const ref op: mpz_t,
                             base: c_int) : size_t;


  //
  // 5.16 Special Functions
  //

  // This is private because you'd have to use a cast
  // primitive to actually use the result.
  // chpl_gmp_mpz_getlimbn might do better.
  private extern proc mpz_getlimbn(const ref op: mpz_t,
                                   n: mp_size_t) : mp_limb_t;

  extern proc mpz_size(const ref x: mpz_t): size_t;

  extern proc mpz_limbs_write(ref x: mpz_t, n: mp_size_t): c_ptr(mp_limb_t);
  extern proc mpz_limbs_finish(ref x: mpz_t, s: mp_size_t);

  //
  // Floating-point Functions
  //


  //
  // 7.1 Initialization Functions
  //

  extern proc mpf_set_default_prec(prec: mp_bitcnt_t);

  extern proc mpf_get_default_prec() : mp_bitcnt_t;

  extern proc mpf_init(ref x: mpf_t);

  extern proc mpf_init2(ref x: mpf_t, prec: mp_bitcnt_t);

  extern proc mpf_clear(ref x: mpf_t);

  extern proc mpf_get_prec(const ref op: mpf_t) : mp_bitcnt_t;

  extern proc mpf_set_prec(ref rop: mpf_t,
                           prec: mp_bitcnt_t);

  extern proc mpf_set_prec_raw(ref rop: mpf_t,
                               prec: mp_bitcnt_t);


  //
  // 7.2 Assignment Functions
  //

  extern proc mpf_set(ref rop: mpf_t,
                      const ref op: mpz_t);

  extern proc mpf_set_ui(ref rop: mpf_t,
                         op: c_ulong);

  extern proc mpf_set_si(ref rop: mpf_t,
                         op: c_long);

  extern proc mpf_set_d(ref rop: mpf_t,
                        op: c_double);

  extern proc mpf_set_z(ref rop: mpf_t,
                        const ref op: mpz_t);

  extern proc mpf_set_q(ref rop: mpf_t,
                        const ref op: mpz_t);

  extern proc mpf_set_str(ref rop: mpz_t,
                          str: c_string,
                          base: c_int);

  extern proc mpf_swap(ref rop1: mpf_t,
                       ref rop2: mpz_t);


  //
  // 7.3 Combined Initialization and Assignment Functions
  //

  extern proc mpf_init_set(ref rop: mpf_t,
                           const ref op: mpz_t);

  extern proc mpf_init_set_ui(ref rop: mpf_t,
                              op: c_ulong);

  extern proc mpf_init_set_si(ref rop: mpf_t,
                              op: c_long);

  extern proc mpf_init_set_d(ref rop: mpf_t,
                             op: c_double);


  //
  // 7.4 Conversion Functions
  //

  extern proc mpf_get_d(const ref op: mpf_t) : c_double;

  extern proc mpf_get_d_2exp(ref exp: c_long,
                             const ref op: mpz_t) : c_double;

  extern proc mpf_get_si(const ref op: mpf_t) : c_long;

  extern proc mpf_get_ui(const ref op: mpf_t) : c_ulong;


  //
  // 7.5 Arithmetic Functions
  //

  extern proc mpf_add(ref rop: mpf_t,
                      const ref op1: mpf_t,
                      const ref op2: mpf_t);

  extern proc mpf_add_ui(ref rop: mpf_t,
                         const ref op1: mpf_t,
                         op2: c_ulong);

  extern proc mpf_sub(ref rop: mpf_t,
                      const ref op1: mpf_t,
                      const ref op2: mpf_t);

  extern proc mpf_ui_sub(ref rop: mpf_t,
                         op1: c_ulong,
                         const ref op2: mpf_t);

  extern proc mpf_sub_ui(ref rop: mpf_t,
                         const ref op1: mpf_t,
                         op2: c_ulong);

  extern proc mpf_mul(ref rop: mpf_t,
                      const ref op1: mpf_t,
                      const ref op2: mpf_t);

  extern proc mpf_mul_ui(ref rop: mpf_t,
                         const ref op1: mpf_t,
                         op2: c_ulong);

  extern proc mpf_div(ref rop: mpf_t,
                      const ref op1: mpf_t,
                      const ref op2: mpf_t);

  extern proc mpf_ui_div(ref rop: mpf_t,
                         op1: c_ulong,
                         const ref op2: mpf_t);

  extern proc mpf_div_ui(ref rop: mpf_t,
                         const ref op1: mpf_t,
                         op2: c_ulong);

  extern proc mpf_sqrt(ref rop: mpf_t,
                       const ref op: mpf_t);

  extern proc mpf_sqrt_ui(ref rop: mpf_t,
                          op: c_ulong);

  extern proc mpf_pow_ui(ref rop: mpf_t,
                         const ref op1: mpf_t,
                         op2: c_ulong);

  extern proc mpf_neg(ref rop: mpf_t,
                      const ref op: mpf_t);

  extern proc mpf_abs(ref rop: mpf_t,
                      const ref op: mpf_t);

  extern proc mpf_mul_2exp(ref rop: mpf_t,
                           const ref op1: mpf_t,
                           op2: mp_bitcnt_t);

  extern proc mpf_div_2exp(ref rop: mpf_t,
                           const ref op1: mpf_t,
                           op2: mp_bitcnt_t);


  //
  // 7.6 Comparison Functions
  //

  extern proc mpf_cmp(const ref op1: mpf_t,
                      const ref op2: mpf_t) : c_int;

  extern proc mpf_cmp_z(const ref op1: mpf_t,
                        const ref op2: mpf_t) : c_int;

  extern proc mpf_cmp_d(const ref op1: mpf_t,
                        op2: c_double) : c_int;

  extern proc mpf_cmp_ui(const ref op1: mpf_t,
                         op2: c_ulong) : c_int;

  extern proc mpf_cmp_si(const ref op1: mpf_t,
                         op2: c_long) : c_int;

  extern proc mpf_eq(const ref op1: mpf_t,
                     const ref op2: mpf_t,
                     op3: mp_bitcnt_t) : c_int;

  extern proc mpf_reldiff(const ref rop: mpf_t,
                          const ref op1: mpf_t,
                          const ref op2: mpf_t);

  extern proc mpf_sgn(const ref op: mpf_t);


  //
  // 7.7 Input and Output Functions
  //

  extern proc mpf_out_str(stream: _file,
                          base: c_int,
                          n_digits: size_t,
                          const ref op: mpf_t);

  extern proc mpf_inp_str(ref rop: mpf_t,
                          stream: _file,
                          base: c_int);


  //
  // 7.8 Miscellaneous Functions
  //

  extern proc mpf_ceil(ref rop: mpf_t,
                       const ref op: mpf_t);

  extern proc mpf_floor(ref rop: mpf_t,
                        const ref op: mpf_t);

  extern proc mpf_trunc(ref rop: mpf_t,
                        const ref op: mpf_t);

  extern proc mpf_integer_p(const ref op: mpf_t) : c_int;

  extern proc mpf_fits_ulong_p(const ref op: mpf_t) : c_int;

  extern proc mpf_fits_slong_p(const ref op: mpf_t) : c_int;

  extern proc mpf_fits_uint_p(const ref op: mpf_t) : c_int;

  extern proc mpf_fits_sint_p(const ref op: mpf_t) : c_int;

  extern proc mpf_fits_ushort_p(const ref op: mpf_t) : c_int;

  extern proc mpf_fits_sshort_p(const ref op: mpf_t) : c_int;

  extern proc mpf_urandomb(ref rop: mpf_t,
                           ref state: gmp_randstate_t,
                           nbits : mp_bitcnt_t);


  //
  // 9 Random Number Functions
  //


  //
  // 9.1 Random State Initialization
  //

  extern proc gmp_randinit_default(ref state: gmp_randstate_t);

  extern proc gmp_randinit_mt(ref state: gmp_randstate_t);

  extern proc gmp_randinit_lc_2exp(ref state: gmp_randstate_t,
                                   const ref a: mpz_t,
                                   c: c_ulong,
                                   m2exp: mp_bitcnt_t);

  extern proc gmp_randinit_lc_2exp_size(ref state: gmp_randstate_t,
                                        size: mp_bitcnt_t);

  extern proc gmp_randinit_set(ref rop: gmp_randstate_t,
                               ref op: gmp_randstate_t);

  extern proc gmp_randclear(ref state: gmp_randstate_t);


  //
  // 9.2 Random State Seeding
  //

  extern proc gmp_randseed(ref state: gmp_randstate_t,
                           const ref seed: mpz_t);

  extern proc gmp_randseed_ui(ref state: gmp_randstate_t,
                              seed: c_ulong);


  //
  // 9.3 Random State Miscellaneous
  //

  extern proc gmp_urandomb_ui(ref state: gmp_randstate_t,
                              n: c_ulong) : c_ulong;

  extern proc gmp_urandomm_ui(ref state: gmp_randstate_t,
                              n: c_ulong) : c_ulong;


  //
  // printf/scanf
  //
  extern proc gmp_printf(fmt: c_string, arg...);

  extern proc gmp_fprintf(fp: _file, fmt: c_string, arg...);

  extern proc gmp_fprintf(fp: _file, fmt: c_string, arg...);

  extern proc gmp_asprintf(ref ret: c_string, fmt: c_string, arg...);


  /* Get an MPZ value stored on another locale */
  proc chpl_gmp_get_mpz(ref ret: mpz_t,
                        src_locale: int,
                        in from: __mpz_struct,
                        copy_allocated:bool = false) {

    // Gather information from the source variable
    var src_nalloc = chpl_gmp_mpz_struct_nalloc(from);
    var src_sign_size = chpl_gmp_mpz_struct_sign_size(from);
    var src_limbs_ptr = chpl_gmp_mpz_struct_limbs(from);

    // Compute the number of limbs used
    var src_size:mp_size_t = 0;
    if src_sign_size < 0 then
      src_size = (-src_sign_size):mp_size_t;
    else
      src_size = src_sign_size:mp_size_t;

    // Compute the size of the number to create & copy
    var new_size:mp_size_t = src_size;
    if copy_allocated then
      new_size = src_nalloc;

    // reallocate the limbs
    _mpz_realloc(ret, new_size);

    // get a pointer to the limbs
    var dst_limbs_ptr = chpl_gmp_mpz_struct_limbs(ret[1]);

    __primitive("chpl_comm_get", dst_limbs_ptr[0],
                                 src_locale, src_limbs_ptr[0],
                                 (new_size:size_t)*c_sizeof(mp_limb_t));

    // Update the sign and size of the number
    chpl_gmp_mpz_set_sign_size(ret, src_sign_size);
  }

  /* Return the number of limbs used in the number */
  proc chpl_gmp_mpz_nlimbs(const ref from: mpz_t) : uint(64) {
    var x = chpl_gmp_mpz_struct_sign_size(from[1]);
    return (abs(x)):uint(64);
  }

  /* Return the i'th limb used in the number (counting from 0) */
  proc chpl_gmp_mpz_getlimbn(const ref from: mpz_t, n:integral) : uint(64) {
    var i = n.safeCast(mp_size_t);
    // OK to cast result to maximal uint for two reasons:
    //  1. GMP always uses unsigned limbs
    //  2. This function is really "getting the bits" so it would be
    //     OK to lose the sign
    // This uses __primitive cast because mp_limb_t is opaque to the Chapel
    // compiler (because it might vary depending on GMP configuration/platform)
    var limb = mpz_getlimbn(from, i);
    var ret = __primitive("cast", uint(64), limb);
    return ret;
  }

  /* Return the number of limbs allocated in an __mpz_struct */
  private extern proc chpl_gmp_mpz_struct_nalloc(from: __mpz_struct) : mp_size_t;
  /* Return the the number of limbs used with the sign of the mpz number
     for an __mpz_struct */
  private extern proc chpl_gmp_mpz_struct_sign_size(from: __mpz_struct) : mp_size_t;
  /* Set the sign and number of fields used in an mpz
     sign taken from sign_size and the number of limbs used is its absolute
     value. */
  private extern proc chpl_gmp_mpz_set_sign_size(ref dst:mpz_t, sign_size:mp_size_t);
  /* Return the pointer to the limbs in an __mpz_struct */
  private extern proc chpl_gmp_mpz_struct_limbs(from: __mpz_struct) : c_ptr(mp_limb_t);

  /* Return the mpz struct storing RNG state */
  private extern proc chpl_gmp_randstate_read_state(src:__gmp_randstate_struct):__mpz_struct;
  /* Sets the mpz storing the RNG state */
  private extern proc chpl_gmp_randstate_set_state(ref dst:gmp_randstate_t, state:mpz_t);
  /* Returns 1 if the two RNGs are using the same algorithm */
  private extern proc
  chpl_gmp_randstate_same_algorithm(a:gmp_randstate_t, b:gmp_randstate_t):c_int;

  /* Get an mpz_t as a string */
  extern proc chpl_gmp_mpz_get_str(base: c_int, const ref x: mpz_t) : c_string;

  class GMPRandom {
    var state: gmp_randstate_t;

    proc init() {
      this.complete();
      gmp_randinit_default(this.state);
    }

    // Creates a Mersenne Twister (probably same as init_default)
    proc init(twister: bool) {
      this.complete();
      gmp_randinit_mt(this.state);
    }

    proc init(a: bigint, c: uint, m2exp: uint) {
      this.complete();
      // Rely on bigint assignment operator to obtain a local copy
      var a_ = a;

      gmp_randinit_lc_2exp(this.state,
                           a_.mpz,
                           c.safeCast(c_ulong),
                           m2exp.safeCast(c_ulong));
    }

    proc init(size: uint) {
      this.complete();
      gmp_randinit_lc_2exp_size(this.state, size.safeCast(c_ulong));
    }

    pragma "no doc"
    proc deinit() {
      on this {
        gmp_randclear(this.state);
      }
    }

    proc seed(seed: bigint) {
      on this {
        // Rely on bigint assignment operator to obtain a local copy
        var s_ = seed;

        gmp_randseed(this.state, s_.mpz);
      }
    }

    proc seed(seed: uint) {
      on this {
        gmp_randseed_ui(this.state, seed.safeCast(c_ulong));
      }
    }

    proc urandomb(nbits: uint) : uint {
      var ret: c_ulong;

      on this {
        ret = gmp_urandomb_ui(this.state, nbits.safeCast(c_ulong));
      }

      return ret.safeCast(uint);
    }

    proc urandomm(n: uint) : uint {
      var ret: c_ulong;

      on this {
        ret = gmp_urandomm_ui(this.state, n.safeCast(c_ulong));
      }

      return ret.safeCast(uint);
    }


    // TO DEPRECATE
    proc urandomb_ui(nbits: uint) : uint {
      var val: c_ulong;

      on this {
        val = gmp_urandomb_ui(this.state, nbits.safeCast(c_ulong));
      }

      return val.safeCast(uint);
    }

    // TO DEPRECATE
    proc urandomm_ui(n: uint) : uint {
      var val: c_ulong;

      on this {
        val = gmp_urandomm_ui(this.state, n.safeCast(c_ulong));
      }

      return val.safeCast(uint);
    }

    proc urandomb(ref r: bigint, nbits: uint) {
      on this {
        // Rely on bigint assignment operator to obtain a local copy
        var r_ = r;

        mpz_urandomb(r_.mpz, this.state, nbits.safeCast(c_ulong));

        r = r_;
      }
    }

    proc urandomm(ref r: bigint, n: bigint) {
      on this {
        // Rely on bigint assignment operator to obtain a local copy
        var r_ = r;
        var n_ = n;

        mpz_urandomm(r_.mpz, this.state, n_.mpz);

        r = r_;
      }
    }

    proc rrandomb(ref r: bigint, nbits: uint) {
      on this {
        // Rely on bigint assignment operator to obtain a local copy
        var r_ = r;

        mpz_rrandomb(r_.mpz, this.state, nbits.safeCast(c_ulong));

        r = r_;
      }
    }
  }

  if CHPL_GMP == "none" {
    compilerError("Cannot use GMP with CHPL_GMP=none");
  }

  // calls mp_set_memory_functions to use chpl_malloc, etc.
  chpl_gmp_init();
}
