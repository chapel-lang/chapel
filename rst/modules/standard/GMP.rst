.. default-domain:: chpl

.. module:: GMP
   :synopsis: Support for GNU Multiple Precision Arithmetic

GMP
===
**Usage**

.. code-block:: chapel

   use GMP;



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


.. function:: proc chpl_gmp_alloc(size: size_t): c_void_ptr

.. function:: proc chpl_gmp_realloc(ptr: c_void_ptr, old_size: size_t, new_size: size_t): c_void_ptr

.. function:: proc chpl_gmp_free(ptr: c_void_ptr, old_size: size_t)

.. function:: proc chpl_gmp_init()

.. type:: type mp_bitcnt_t = c_ulong

   The GMP ``mp_bitcnt_t`` type 

.. type:: type mp_size_t = c_long

   The GMP ``mp_size_t``   type 

.. type:: type mp_limb_t

   This is normally uint(64) but can depend on configuration. 

.. data:: const mp_bits_per_limb: c_int

   The GMP `mp_bits_per_limb`` constant 

.. type:: type mpz_t = 1*__mpz_struct

   The GMP ``mpz_t`` type 

.. type:: type mpf_t = 1*__mpf_struct

   The GMP ``mpf_t`` type 

.. type:: type gmp_randstate_t = 1*__gmp_randstate_struct

   The GMP ``gmp_randstate_t`` type 

.. function:: proc mpz_init(ref x: mpz_t)

   

.. function:: proc mpz_init2(ref x: mpz_t, n: mp_bitcnt_t)

.. function:: proc mpz_clear(ref x: mpz_t)

.. function:: proc _mpz_realloc(ref x: mpz_t, new_alloc: mp_size_t)

.. function:: proc mpz_realloc2(ref x: mpz_t, n: mp_bitcnt_t)

.. function:: proc mpz_set(ref rop: mpz_t, const ref op: mpz_t)

.. function:: proc mpz_set_ui(ref rop: mpz_t, op: c_ulong)

.. function:: proc mpz_set_si(ref rop: mpz_t, op: c_long)

.. function:: proc mpz_set_d(ref rop: mpz_t, op: c_double)

.. function:: proc mpz_set_str(ref rop: mpz_t, str: c_string, base: c_int)

.. function:: proc mpz_swap(ref rop1: mpz_t, ref rop2: mpz_t)

.. function:: proc mpz_init_set(ref rop: mpz_t, const ref op: mpz_t)

.. function:: proc mpz_init_set_ui(ref rop: mpz_t, op: c_ulong)

.. function:: proc mpz_init_set_si(ref rop: mpz_t, op: c_long)

.. function:: proc mpz_init_set_d(ref rop: mpz_t, op: c_double)

.. function:: proc mpz_init_set_str(ref rop: mpz_t, str: c_string, base: c_int): c_int

.. function:: proc mpz_get_ui(const ref op: mpz_t): c_ulong

.. function:: proc mpz_get_si(const ref op: mpz_t): c_long

.. function:: proc mpz_get_d(const ref op: mpz_t): c_double

.. function:: proc mpz_get_d_2exp(ref exp: c_long, const ref op: mpz_t): c_double

.. function:: proc mpz_get_str(str: c_string, base: c_int, const ref op: mpz_t): c_string

.. function:: proc mpz_add(ref rop: mpz_t, const ref op1: mpz_t, const ref op2: mpz_t)

.. function:: proc mpz_add_ui(ref rop: mpz_t, const ref op1: mpz_t, op2: c_ulong)

.. function:: proc mpz_sub(ref rop: mpz_t, const ref op1: mpz_t, const ref op2: mpz_t)

.. function:: proc mpz_sub_ui(ref rop: mpz_t, const ref op1: mpz_t, op2: c_ulong)

.. function:: proc mpz_ui_sub(ref rop: mpz_t, op1: c_ulong, const ref op2: mpz_t)

.. function:: proc mpz_mul(ref rop: mpz_t, const ref op1: mpz_t, const ref op2: mpz_t)

.. function:: proc mpz_mul_si(ref rop: mpz_t, const ref op1: mpz_t, op2: c_long)

.. function:: proc mpz_mul_ui(ref rop: mpz_t, const ref op1: mpz_t, op2: c_ulong)

.. function:: proc mpz_addmul(ref rop: mpz_t, const ref op1: mpz_t, const ref op2: mpz_t)

.. function:: proc mpz_addmul_ui(ref rop: mpz_t, const ref op1: mpz_t, op2: c_ulong)

.. function:: proc mpz_submul(ref rop: mpz_t, const ref op1: mpz_t, const ref op2: mpz_t)

.. function:: proc mpz_submul_ui(ref rop: mpz_t, const ref op1: mpz_t, op2: c_ulong)

.. function:: proc mpz_mul_2exp(ref rop: mpz_t, const ref op1: mpz_t, op2: mp_bitcnt_t)

.. function:: proc mpz_neg(ref rop: mpz_t, const ref op: mpz_t)

.. function:: proc mpz_abs(ref rop: mpz_t, const ref op: mpz_t)

.. function:: proc mpz_cdiv_q(ref q: mpz_t, const ref n: mpz_t, const ref d: mpz_t)

.. function:: proc mpz_cdiv_r(ref r: mpz_t, const ref n: mpz_t, const ref d: mpz_t)

.. function:: proc mpz_cdiv_qr(ref q: mpz_t, ref r: mpz_t, const ref n: mpz_t, const ref d: mpz_t)

.. function:: proc mpz_cdiv_q_ui(ref q: mpz_t, const ref n: mpz_t, d: c_ulong): c_ulong

.. function:: proc mpz_cdiv_r_ui(ref r: mpz_t, const ref n: mpz_t, d: c_ulong): c_ulong

.. function:: proc mpz_cdiv_qr_ui(ref q: mpz_t, ref r: mpz_t, const ref n: mpz_t, d: c_ulong): c_ulong

.. function:: proc mpz_cdiv_ui(const ref n: mpz_t, d: c_ulong): c_ulong

.. function:: proc mpz_cdiv_q_2exp(ref q: mpz_t, const ref n: mpz_t, b: mp_bitcnt_t)

.. function:: proc mpz_cdiv_r_2exp(ref r: mpz_t, const ref n: mpz_t, b: mp_bitcnt_t)

.. function:: proc mpz_fdiv_q(ref q: mpz_t, const ref n: mpz_t, const ref d: mpz_t)

.. function:: proc mpz_fdiv_r(ref r: mpz_t, const ref n: mpz_t, const ref d: mpz_t)

.. function:: proc mpz_fdiv_qr(ref q: mpz_t, ref r: mpz_t, const ref n: mpz_t, const ref d: mpz_t)

.. function:: proc mpz_fdiv_q_ui(ref q: mpz_t, const ref n: mpz_t, d: c_ulong): c_ulong

.. function:: proc mpz_fdiv_r_ui(ref r: mpz_t, const ref n: mpz_t, d: c_ulong): c_ulong

.. function:: proc mpz_fdiv_qr_ui(ref q: mpz_t, ref r: mpz_t, const ref n: mpz_t, d: c_ulong): c_ulong

.. function:: proc mpz_fdiv_ui(const ref n: mpz_t, d: c_ulong): c_ulong

.. function:: proc mpz_fdiv_q_2exp(ref q: mpz_t, const ref n: mpz_t, b: mp_bitcnt_t)

.. function:: proc mpz_fdiv_r_2exp(ref r: mpz_t, const ref n: mpz_t, b: mp_bitcnt_t)

.. function:: proc mpz_tdiv_q(ref q: mpz_t, const ref n: mpz_t, const ref d: mpz_t)

.. function:: proc mpz_tdiv_r(ref r: mpz_t, const ref n: mpz_t, const ref d: mpz_t)

.. function:: proc mpz_tdiv_qr(ref q: mpz_t, ref r: mpz_t, const ref n: mpz_t, const ref d: mpz_t)

.. function:: proc mpz_tdiv_q_ui(ref q: mpz_t, const ref n: mpz_t, d: c_ulong): c_ulong

.. function:: proc mpz_tdiv_r_ui(ref r: mpz_t, const ref n: mpz_t, d: c_ulong): c_ulong

.. function:: proc mpz_tdiv_qr_ui(ref q: mpz_t, ref r: mpz_t, const ref n: mpz_t, d: c_ulong): c_ulong

.. function:: proc mpz_tdiv_ui(const ref n: mpz_t, d: c_ulong): c_ulong

.. function:: proc mpz_tdiv_q_2exp(ref q: mpz_t, const ref n: mpz_t, b: mp_bitcnt_t)

.. function:: proc mpz_tdiv_r_2exp(ref r: mpz_t, const ref n: mpz_t, b: mp_bitcnt_t)

.. function:: proc mpz_mod(ref rop: mpz_t, const ref n: mpz_t, const ref d: mpz_t)

.. function:: proc mpz_mod_ui(ref rop: mpz_t, const ref n: mpz_t, d: c_ulong): c_ulong

.. function:: proc mpz_divexact(ref q: mpz_t, const ref n: mpz_t, const ref d: mpz_t)

.. function:: proc mpz_divexact_ui(ref q: mpz_t, const ref n: mpz_t, d: c_ulong)

.. function:: proc mpz_divisible_p(const ref n: mpz_t, const ref d: mpz_t): c_int

.. function:: proc mpz_divisible_ui_p(const ref n: mpz_t, d: c_ulong): c_int

.. function:: proc mpz_divisible_2exp_p(const ref n: mpz_t, b: mp_bitcnt_t): c_int

.. function:: proc mpz_congruent_p(const ref n: mpz_t, const ref c: mpz_t, const ref d: mpz_t): c_int

.. function:: proc mpz_congruent_ui_p(const ref n: mpz_t, c: c_ulong, d: c_ulong): c_int

.. function:: proc mpz_congruent_2exp_p(const ref n: mpz_t, const ref c: mpz_t, b: mp_bitcnt_t): c_int

.. function:: proc mpz_powm(ref rop: mpz_t, const ref base: mpz_t, const ref exp: mpz_t, const ref mod: mpz_t)

.. function:: proc mpz_powm_ui(ref rop: mpz_t, const ref base: mpz_t, exp: c_ulong, const ref mod: mpz_t)

.. function:: proc mpz_powm_sec(ref rop: mpz_t, const ref base: mpz_t, const ref exp: mpz_t, const ref mod: mpz_t)

.. function:: proc mpz_pow_ui(ref rop: mpz_t, const ref base: mpz_t, exp: c_ulong)

.. function:: proc mpz_ui_pow_ui(ref rop: mpz_t, base: c_ulong, exp: c_ulong)

.. function:: proc mpz_root(ref rop: mpz_t, const ref op: mpz_t, n: c_ulong): c_int

.. function:: proc mpz_rootrem(ref root: mpz_t, ref rem: mpz_t, const ref u: mpz_t, n: c_ulong)

.. function:: proc mpz_sqrt(ref rop: mpz_t, const ref op: mpz_t)

.. function:: proc mpz_sqrtrem(ref rop1: mpz_t, ref rop2: mpz_t, const ref op: mpz_t)

.. function:: proc mpz_perfect_power_p(const ref op: mpz_t): c_int

.. function:: proc mpz_perfect_square_p(const ref op: mpz_t): c_int

.. function:: proc mpz_probab_prime_p(ref n: mpz_t, reps: c_int): c_int

.. function:: proc mpz_nextprime(ref rop: mpz_t, const ref op: mpz_t)

.. function:: proc mpz_gcd(ref rop: mpz_t, const ref op1: mpz_t, const ref op2: mpz_t)

.. function:: proc mpz_gcd_ui(ref rop: mpz_t, const ref op1: mpz_t, op2: c_ulong)

.. function:: proc mpz_gcdext(ref g: mpz_t, ref s: mpz_t, ref t: mpz_t, const ref a: mpz_t, const ref b: mpz_t)

.. function:: proc mpz_lcm(ref rop: mpz_t, const ref op1: mpz_t, const ref op2: mpz_t)

.. function:: proc mpz_lcm_ui(ref rop: mpz_t, const ref op1: mpz_t, op2: c_ulong)

.. function:: proc mpz_invert(ref rop: mpz_t, const ref op1: mpz_t, const ref op2: mpz_t): c_int

.. function:: proc mpz_jacobi(const ref a: mpz_t, const ref b: mpz_t): c_int

.. function:: proc mpz_legendre(const ref a: mpz_t, const ref p: mpz_t): c_int

.. function:: proc mpz_kronecker(const ref a: mpz_t, const ref b: mpz_t): c_int

.. function:: proc mpz_kronecker_si(const ref a: mpz_t, b: c_long): c_int

.. function:: proc mpz_kronecker_ui(const ref a: mpz_t, b: c_ulong): c_int

.. function:: proc mpz_si_kronecker(a: c_long, const ref b: mpz_t): c_int

.. function:: proc mpz_ui_kronecker(a: c_ulong, const ref b: mpz_t): c_int

.. function:: proc mpz_remove(ref rop: mpz_t, const ref op: mpz_t, const ref f: mpz_t): c_ulong

.. function:: proc mpz_fac_ui(ref rop: mpz_t, n: c_ulong)

.. function:: proc mpz_2fac_ui(ref rop: mpz_t, n: c_ulong)

.. function:: proc mpz_mfac_uiui(ref rop: mpz_t, n: c_ulong, m: c_ulong)

.. function:: proc mpz_primorial_ui(ref rop: mpz_t, n: c_ulong)

.. function:: proc mpz_bin_ui(ref rop: mpz_t, const ref n: mpz_t, k: c_ulong)

.. function:: proc mpz_bin_uiui(ref rop: mpz_t, n: c_ulong, k: c_ulong)

.. function:: proc mpz_fib_ui(ref fn: mpz_t, n: c_ulong)

.. function:: proc mpz_fib2_ui(ref fn: mpz_t, ref fnsub1: mpz_t, n: c_ulong)

.. function:: proc mpz_lucnum_ui(ref ln: mpz_t, n: c_ulong)

.. function:: proc mpz_lucnum2_ui(ref ln: mpz_t, ref lnsub1: mpz_t, n: c_ulong)

.. function:: proc mpz_cmp(const ref op1: mpz_t, const ref op2: mpz_t): c_int

.. function:: proc mpz_cmp_d(const ref op1: mpz_t, op2: c_double): c_int

.. function:: proc mpz_cmp_si(const ref op1: mpz_t, op2: c_long): c_int

.. function:: proc mpz_cmp_ui(const ref op1: mpz_t, op2: c_ulong): c_int

.. function:: proc mpz_cmpabs(const ref op1: mpz_t, const ref op2: mpz_t): c_int

.. function:: proc mpz_cmpabs_d(const ref op1: mpz_t, op2: c_double): c_int

.. function:: proc mpz_cmpabs_ui(const ref op1: mpz_t, op2: c_ulong): c_int

.. function:: proc mpz_sgn(const ref op: mpz_t): c_int

.. function:: proc mpz_and(ref rop: mpz_t, const ref op1: mpz_t, const ref op2: mpz_t)

.. function:: proc mpz_ior(ref rop: mpz_t, const ref op1: mpz_t, const ref op2: mpz_t)

.. function:: proc mpz_xor(ref rop: mpz_t, const ref op1: mpz_t, const ref op2: mpz_t)

.. function:: proc mpz_com(ref rop: mpz_t, const ref op: mpz_t)

.. function:: proc mpz_popcount(const ref op: mpz_t): c_ulong

.. function:: proc mpz_hamdist(const ref op1: mpz_t, const ref op2: mpz_t): c_ulong

.. function:: proc mpz_scan0(const ref op: mpz_t, starting_bit: mp_bitcnt_t): c_ulong

.. function:: proc mpz_scan1(const ref op: mpz_t, starting_bit: mp_bitcnt_t): c_ulong

.. function:: proc mpz_setbit(ref rop: mpz_t, bit_index: mp_bitcnt_t)

.. function:: proc mpz_clrbit(ref rop: mpz_t, bit_index: mp_bitcnt_t)

.. function:: proc mpz_combit(ref rop: mpz_t, bit_index: mp_bitcnt_t)

.. function:: proc mpz_tstbit(const ref op: mpz_t, bit_index: mp_bitcnt_t): c_int

.. function:: proc mpz_urandomb(ref rop: mpz_t, ref state: gmp_randstate_t, n: mp_bitcnt_t)

.. function:: proc mpz_urandomm(ref rop: mpz_t, ref state: gmp_randstate_t, const ref n: mpz_t)

.. function:: proc mpz_rrandomb(ref rop: mpz_t, ref state: gmp_randstate_t, n: mp_bitcnt_t)

.. function:: proc mpz_random(ref rop: mpz_t, max_size: mp_size_t)

.. function:: proc mpz_random2(ref rop: mpz_t, max_size: mp_size_t)

.. function:: proc mpz_fits_ulong_p(const ref op: mpz_t): c_int

.. function:: proc mpz_fits_slong_p(const ref op: mpz_t): c_int

.. function:: proc mpz_fits_uint_p(const ref op: mpz_t): c_int

.. function:: proc mpz_fits_sint_p(const ref op: mpz_t): c_int

.. function:: proc mpz_fits_ushort_p(const ref op: mpz_t): c_int

.. function:: proc mpz_fits_sshort_p(const ref op: mpz_t): c_int

.. function:: proc mpz_odd_p(const ref op: mpz_t): c_int

.. function:: proc mpz_even_p(const ref op: mpz_t): c_int

.. function:: proc mpz_sizeinbase(const ref op: mpz_t, base: c_int): size_t

.. function:: proc mpz_size(const ref x: mpz_t): size_t

.. function:: proc mpz_limbs_write(ref x: mpz_t, n: mp_size_t): c_ptr(mp_limb_t)

.. function:: proc mpz_limbs_finish(ref x: mpz_t, s: mp_size_t)

.. function:: proc mpf_set_default_prec(prec: mp_bitcnt_t)

.. function:: proc mpf_get_default_prec(): mp_bitcnt_t

.. function:: proc mpf_init(ref x: mpf_t)

.. function:: proc mpf_init2(ref x: mpf_t, prec: mp_bitcnt_t)

.. function:: proc mpf_clear(ref x: mpf_t)

.. function:: proc mpf_get_prec(const ref op: mpf_t): mp_bitcnt_t

.. function:: proc mpf_set_prec(ref rop: mpf_t, prec: mp_bitcnt_t)

.. function:: proc mpf_set_prec_raw(ref rop: mpf_t, prec: mp_bitcnt_t)

.. function:: proc mpf_set(ref rop: mpf_t, const ref op: mpz_t)

.. function:: proc mpf_set_ui(ref rop: mpf_t, op: c_ulong)

.. function:: proc mpf_set_si(ref rop: mpf_t, op: c_long)

.. function:: proc mpf_set_d(ref rop: mpf_t, op: c_double)

.. function:: proc mpf_set_z(ref rop: mpf_t, const ref op: mpz_t)

.. function:: proc mpf_set_q(ref rop: mpf_t, const ref op: mpz_t)

.. function:: proc mpf_set_str(ref rop: mpz_t, str: c_string, base: c_int)

.. function:: proc mpf_swap(ref rop1: mpf_t, ref rop2: mpz_t)

.. function:: proc mpf_init_set(ref rop: mpf_t, const ref op: mpz_t)

.. function:: proc mpf_init_set_ui(ref rop: mpf_t, op: c_ulong)

.. function:: proc mpf_init_set_si(ref rop: mpf_t, op: c_long)

.. function:: proc mpf_init_set_d(ref rop: mpf_t, op: c_double)

.. function:: proc mpf_get_d(const ref op: mpf_t): c_double

.. function:: proc mpf_get_d_2exp(ref exp: c_long, const ref op: mpz_t): c_double

.. function:: proc mpf_get_si(const ref op: mpf_t): c_long

.. function:: proc mpf_get_ui(const ref op: mpf_t): c_ulong

.. function:: proc mpf_add(ref rop: mpf_t, const ref op1: mpf_t, const ref op2: mpf_t)

.. function:: proc mpf_add_ui(ref rop: mpf_t, const ref op1: mpf_t, op2: c_ulong)

.. function:: proc mpf_sub(ref rop: mpf_t, const ref op1: mpf_t, const ref op2: mpf_t)

.. function:: proc mpf_ui_sub(ref rop: mpf_t, op1: c_ulong, const ref op2: mpf_t)

.. function:: proc mpf_sub_ui(ref rop: mpf_t, const ref op1: mpf_t, op2: c_ulong)

.. function:: proc mpf_mul(ref rop: mpf_t, const ref op1: mpf_t, const ref op2: mpf_t)

.. function:: proc mpf_mul_ui(ref rop: mpf_t, const ref op1: mpf_t, op2: c_ulong)

.. function:: proc mpf_div(ref rop: mpf_t, const ref op1: mpf_t, const ref op2: mpf_t)

.. function:: proc mpf_ui_div(ref rop: mpf_t, op1: c_ulong, const ref op2: mpf_t)

.. function:: proc mpf_div_ui(ref rop: mpf_t, const ref op1: mpf_t, op2: c_ulong)

.. function:: proc mpf_sqrt(ref rop: mpf_t, const ref op: mpf_t)

.. function:: proc mpf_sqrt_ui(ref rop: mpf_t, op: c_ulong)

.. function:: proc mpf_pow_ui(ref rop: mpf_t, const ref op1: mpf_t, op2: c_ulong)

.. function:: proc mpf_neg(ref rop: mpf_t, const ref op: mpf_t)

.. function:: proc mpf_abs(ref rop: mpf_t, const ref op: mpf_t)

.. function:: proc mpf_mul_2exp(ref rop: mpf_t, const ref op1: mpf_t, op2: mp_bitcnt_t)

.. function:: proc mpf_div_2exp(ref rop: mpf_t, const ref op1: mpf_t, op2: mp_bitcnt_t)

.. function:: proc mpf_cmp(const ref op1: mpf_t, const ref op2: mpf_t): c_int

.. function:: proc mpf_cmp_z(const ref op1: mpf_t, const ref op2: mpf_t): c_int

.. function:: proc mpf_cmp_d(const ref op1: mpf_t, op2: c_double): c_int

.. function:: proc mpf_cmp_ui(const ref op1: mpf_t, op2: c_ulong): c_int

.. function:: proc mpf_cmp_si(const ref op1: mpf_t, op2: c_long): c_int

.. function:: proc mpf_eq(const ref op1: mpf_t, const ref op2: mpf_t, op3: mp_bitcnt_t): c_int

.. function:: proc mpf_reldiff(const ref rop: mpf_t, const ref op1: mpf_t, const ref op2: mpf_t)

.. function:: proc mpf_sgn(const ref op: mpf_t)

.. function:: proc mpf_out_str(stream: _file, base: c_int, n_digits: size_t, const ref op: mpf_t)

.. function:: proc mpf_inp_str(ref rop: mpf_t, stream: _file, base: c_int)

.. function:: proc mpf_ceil(ref rop: mpf_t, const ref op: mpf_t)

.. function:: proc mpf_floor(ref rop: mpf_t, const ref op: mpf_t)

.. function:: proc mpf_trunc(ref rop: mpf_t, const ref op: mpf_t)

.. function:: proc mpf_integer_p(const ref op: mpf_t): c_int

.. function:: proc mpf_fits_ulong_p(const ref op: mpf_t): c_int

.. function:: proc mpf_fits_slong_p(const ref op: mpf_t): c_int

.. function:: proc mpf_fits_uint_p(const ref op: mpf_t): c_int

.. function:: proc mpf_fits_sint_p(const ref op: mpf_t): c_int

.. function:: proc mpf_fits_ushort_p(const ref op: mpf_t): c_int

.. function:: proc mpf_fits_sshort_p(const ref op: mpf_t): c_int

.. function:: proc mpf_urandomb(ref rop: mpf_t, ref state: gmp_randstate_t, nbits: mp_bitcnt_t)

.. function:: proc gmp_randinit_default(ref state: gmp_randstate_t)

.. function:: proc gmp_randinit_mt(ref state: gmp_randstate_t)

.. function:: proc gmp_randinit_lc_2exp(ref state: gmp_randstate_t, const ref a: mpz_t, c: c_ulong, m2exp: mp_bitcnt_t)

.. function:: proc gmp_randinit_lc_2exp_size(ref state: gmp_randstate_t, size: mp_bitcnt_t)

.. function:: proc gmp_randinit_set(ref rop: gmp_randstate_t, ref op: gmp_randstate_t)

.. function:: proc gmp_randclear(ref state: gmp_randstate_t)

.. function:: proc gmp_randseed(ref state: gmp_randstate_t, const ref seed: mpz_t)

.. function:: proc gmp_randseed_ui(ref state: gmp_randstate_t, seed: c_ulong)

.. function:: proc gmp_urandomb_ui(ref state: gmp_randstate_t, n: c_ulong): c_ulong

.. function:: proc gmp_urandomm_ui(ref state: gmp_randstate_t, n: c_ulong): c_ulong

.. function:: proc gmp_printf(fmt: c_string, arg ...)

.. function:: proc gmp_fprintf(fp: _file, fmt: c_string, arg ...)

.. function:: proc gmp_fprintf(fp: _file, fmt: c_string, arg ...)

.. function:: proc gmp_asprintf(ref ret: c_string, fmt: c_string, arg ...)

.. function:: proc chpl_gmp_get_mpz(ref ret: mpz_t, src_locale: int, in from: __mpz_struct, copy_allocated: bool = false)

   Get an MPZ value stored on another locale 

.. function:: proc chpl_gmp_mpz_nlimbs(const ref from: mpz_t): uint(64)

   Return the number of limbs used in the number 

.. function:: proc chpl_gmp_mpz_getlimbn(const ref from: mpz_t, n: integral): uint(64)

   Return the i'th limb used in the number (counting from 0) 

.. function:: proc chpl_gmp_mpz_get_str(base: c_int, const ref x: mpz_t): c_string

   Get an mpz_t as a string 

.. class:: GMPRandom

   .. attribute:: var state: gmp_randstate_t

   .. method:: proc init()

   .. method:: proc init(twister: bool)

   .. method:: proc init(a: bigint, c: uint, m2exp: uint)

   .. method:: proc init(size: uint)

   .. method:: proc seed(seed: bigint)

   .. method:: proc seed(seed: uint)

   .. method:: proc urandomb(nbits: uint): uint

   .. method:: proc urandomm(n: uint): uint

   .. method:: proc urandomb_ui(nbits: uint): uint

   .. method:: proc urandomm_ui(n: uint): uint

   .. method:: proc urandomb(ref r: bigint, nbits: uint)

   .. method:: proc urandomm(ref r: bigint, n: bigint)

   .. method:: proc rrandomb(ref r: bigint, nbits: uint)

