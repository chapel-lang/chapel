.. default-domain:: chpl

.. module:: BigInteger
   :synopsis: The ``bigint`` record supports arithmetic operations on arbitrary

BigInteger
==========
**Usage**

.. code-block:: chapel

   use BigInteger;


The ``bigint`` record supports arithmetic operations on arbitrary
precision integers in a manner that is broadly consistent with
the conventional operations on primitive fixed length integers.

The current implementation is based on the low-level types and
functions defined in the GMP module i.e. it is implemented using the
GNU Multiple Precision Integer Arithmetic library (GMP). More specifically
the record :record:`bigint` wraps the GMP type :type:`~GMP.mpz_t`.

The primary benefits of ``bigint`` over ``mpz_t`` are

  1) support for multi-locale programs

  2) the convenience of arithmetic operator overloads

  3) automatic memory management of GMP data structures

In addition to the expected set of operations, this record provides
a number of methods that wrap GMP functions in a natural way:

.. code-block:: chapel

 use BigInteger;

 var   a = new bigint(234958444);
 const b = new bigint("4847382292989382987395534934347");
 var   c = new bigint();

 writeln(a * b);

 c.fac(100);
 writeln(c);

Casting and declarations can be used to create ``bigint`` records as
well:

.. code-block:: chapel

 use BigInteger;

 var   a = 234958444: bigint;
 const b = "4847382292989382987395534934347": bigint;
 var   c: bigint;

.. warning::

  Creating a ``bigint`` from an integer literal that is larger than
  ``max(uint(64))`` would cause integer overflow before the
  ``bigint`` is created, and so results in a compile-time error.
  Strings should be used instead of integer literals for cases
  like this:

  .. code-block:: chapel

    // These would result in integer overflow and cause compile-time errors
    var bad1 = 4847382292989382987395534934347: bigint;
    var bad2 = new bigint(4847382292989382987395534934347);

    // These give the desired result
    var good1 = "4847382292989382987395534934347": bigint;
    var good2 = new bigint("4847382292989382987395534934347");


Wrapping an ``mpz_t`` in a ``bigint`` record may introduce a
measurable overhead in some cases.

The GMP library defines a low-level API that is based on
side-effecting compound operations.  The documentation recommends that
one prefer to reuse a small number of existing mpz_t structures rather
than using many values of short duration.

Matching this style using ``bigint`` records and the compound
assignment operators is likely to provide comparable performance to an
implementation based on ``mpz_t``.  So, for example:

.. code-block:: chapel

  x  = b
  x *= c;
  x += a;

is likely to achieve better performance than:

.. code-block:: chapel

  x = a + b * c;

In the fall of 2016 the Chapel compiler introduces two short lived
temporaries for the intermediate results of the binary operators.


If peak performance is required, perhaps in a critical loop, then it
is always possible to invoke the GMP functions directly.  For example
one might express:

.. code-block:: chapel

  a = a + b * c;

as:

.. code-block:: chapel

  mpz_addmul(a.mpz, b.mpz, c.mpz);


As usual the details are application specific and it is best to
measure when peak performance is required.

The operators on ``bigint`` include variations that accept Chapel
integers e.g.:

.. code-block:: chapel

  var a = new bigint("9738639463465935");
  var b = 9395739153 * a;

The Chapel int(64) literal is converted to an underlying,
platform-specific C integer, to invoke the underlying GMP primitive
function.  This example is likely to work well on popular 64-bit
platforms but to fail on common 32-bit platforms.  Runtime checks are
used to ensure the Chapel types can safely be cast to the
platform-specific types.  Ths program will halt if the Chapel value
cannot be represented using the GMP scalar type.

The checks are controlled by the compiler options ``--[no-]cast-checks``,
``--fast``, etc.

Casting from ``bigint`` to ``integral`` and ``real`` types is also
supported.  Values that are too large for the resultant type are
truncated.  GMP primitives are used to first cast to platform-specific C
types, which are then cast to Chapel types.  As a result, casting to
64-bit types on 32-bit platforms may result in additional truncation.
Additionally, casting a negative ``bigint`` to a ``uint`` will result in
the absolute value truncated to fit within the type.:

.. code-block:: chapel

  var a = new bigint(-1);
  writeln(a:uint);        // prints "1"

See :mod:`GMP` for more information on how to use GMP with Chapel.


.. enum:: enum Round { DOWN = -1, ZERO = 0, UP = 1 }

.. record:: bigint

   .. attribute:: var mpz: mpz_t

      The underlying GMP C structure 

   .. method:: proc init()

   .. method:: proc init(const ref num: bigint)

   .. method:: proc init=(const ref num: bigint)

   .. method:: proc init(num: int)

   .. method:: proc init(num: uint)

   .. method:: proc init=(num: integral)

   .. method:: proc init(str: string, base: int = 0)

   .. method:: proc init(str: string, base: int = 0, out error: syserr)

   .. method:: proc size(): size_t

   .. method:: proc sizeinbase(base: int): uint

   .. method:: proc numLimbs: uint

   .. method:: proc get_limbn(n: integral): uint

   .. method:: proc mpzStruct(): __mpz_struct

   .. method:: proc get_d_2exp(): (uint(32), real)

   .. method:: proc get_str(base: int = 10): string

   .. method:: proc writeThis(writer) throws

.. function:: proc =(ref lhs: bigint, const ref rhs: bigint)

.. function:: proc =(ref lhs: bigint, rhs: int)

.. function:: proc =(ref lhs: bigint, rhs: uint)

.. function:: proc +(const ref a: bigint)

.. function:: proc -(const ref a: bigint)

.. function:: proc ~(const ref a: bigint)

.. function:: proc +(const ref a: bigint, const ref b: bigint)

.. function:: proc +(const ref a: bigint, b: int)

.. function:: proc +(a: int, const ref b: bigint)

.. function:: proc +(const ref a: bigint, b: uint)

.. function:: proc +(a: uint, const ref b: bigint)

.. function:: proc -(const ref a: bigint, const ref b: bigint)

.. function:: proc -(const ref a: bigint, b: int)

.. function:: proc -(a: int, const ref b: bigint)

.. function:: proc -(const ref a: bigint, b: uint)

.. function:: proc -(a: uint, const ref b: bigint)

.. function:: proc *(const ref a: bigint, const ref b: bigint)

.. function:: proc *(const ref a: bigint, b: int)

.. function:: proc *(a: int, const ref b: bigint)

.. function:: proc *(const ref a: bigint, b: uint)

.. function:: proc *(a: uint, const ref b: bigint)

.. function:: proc /(const ref a: bigint, const ref b: bigint)

.. function:: proc /(const ref a: bigint, b: integral)

.. function:: proc **(const ref base: bigint, const ref exp: bigint)

.. function:: proc **(const ref base: bigint, exp: int)

.. function:: proc **(const ref base: bigint, exp: uint)

.. function:: proc %(const ref a: bigint, const ref b: bigint)

.. function:: proc %(const ref a: bigint, b: int)

.. function:: proc %(const ref a: bigint, b: uint)

.. function:: proc <<(const ref a: bigint, b: int)

.. function:: proc <<(const ref a: bigint, b: uint)

.. function:: proc >>(const ref a: bigint, b: int)

.. function:: proc >>(const ref a: bigint, b: uint)

.. function:: proc &(const ref a: bigint, const ref b: bigint)

.. function:: proc |(const ref a: bigint, const ref b: bigint)

.. function:: proc ^(const ref a: bigint, const ref b: bigint)

.. function:: proc ==(const ref a: bigint, const ref b: bigint)

.. function:: proc ==(const ref a: bigint, b: int)

.. function:: proc ==(a: int, const ref b: bigint)

.. function:: proc ==(const ref a: bigint, b: uint)

.. function:: proc ==(a: uint, const ref b: bigint)

.. function:: proc !=(const ref a: bigint, const ref b: bigint)

.. function:: proc !=(const ref a: bigint, b: int)

.. function:: proc !=(a: int, const ref b: bigint)

.. function:: proc !=(const ref a: bigint, b: uint)

.. function:: proc !=(a: uint, const ref b: bigint)

.. function:: proc >(const ref a: bigint, const ref b: bigint)

.. function:: proc >(const ref a: bigint, b: int)

.. function:: proc >(a: int, const ref b: bigint)

.. function:: proc >(const ref a: bigint, b: uint)

.. function:: proc >(a: uint, const ref b: bigint)

.. function:: proc <(const ref a: bigint, const ref b: bigint)

.. function:: proc <(const ref a: bigint, b: int)

.. function:: proc <(a: int, const ref b: bigint)

.. function:: proc <(const ref a: bigint, b: uint)

.. function:: proc <(a: uint, const ref b: bigint)

.. function:: proc >=(const ref a: bigint, const ref b: bigint)

.. function:: proc >=(const ref a: bigint, b: int)

.. function:: proc >=(a: int, const ref b: bigint)

.. function:: proc >=(const ref a: bigint, b: uint)

.. function:: proc >=(a: uint, const ref b: bigint)

.. function:: proc <=(const ref a: bigint, const ref b: bigint)

.. function:: proc <=(const ref a: bigint, b: int)

.. function:: proc <=(a: int, const ref b: bigint)

.. function:: proc <=(const ref a: bigint, b: uint)

.. function:: proc <=(a: uint, const ref b: bigint)

.. function:: proc +=(ref a: bigint, const ref b: bigint)

.. function:: proc +=(ref a: bigint, b: int)

.. function:: proc +=(ref a: bigint, b: uint)

.. function:: proc -=(ref a: bigint, const ref b: bigint)

.. function:: proc -=(ref a: bigint, b: int)

.. function:: proc -=(ref a: bigint, b: uint)

.. function:: proc *=(ref a: bigint, const ref b: bigint)

.. function:: proc *=(ref a: bigint, b: int)

.. function:: proc *=(ref a: bigint, b: uint)

.. function:: proc /=(ref a: bigint, const ref b: bigint)

.. function:: proc /=(ref a: bigint, b: integral)

.. function:: proc **=(ref base: bigint, const ref exp: bigint)

.. function:: proc **=(ref base: bigint, exp: int)

.. function:: proc **=(ref base: bigint, exp: uint)

.. function:: proc %=(ref a: bigint, const ref b: bigint)

.. function:: proc %=(ref a: bigint, b: int)

.. function:: proc %=(ref a: bigint, b: uint)

.. function:: proc &=(ref a: bigint, const ref b: bigint)

.. function:: proc |=(ref a: bigint, const ref b: bigint)

.. function:: proc ^=(ref a: bigint, const ref b: bigint)

.. function:: proc <<=(ref a: bigint, b: int)

.. function:: proc <<=(ref a: bigint, b: uint)

.. function:: proc >>=(ref a: bigint, b: int)

.. function:: proc >>=(ref a: bigint, b: uint)

.. function:: proc <=>(ref a: bigint, ref b: bigint)

.. function:: proc jacobi(const ref a: bigint, const ref b: bigint): int

.. function:: proc legendre(const ref a: bigint, const ref p: bigint): int

.. function:: proc kronecker(const ref a: bigint, const ref b: bigint): int

.. function:: proc kronecker(const ref a: bigint, b: int): int

.. function:: proc kronecker(a: int, const ref b: bigint): int

.. function:: proc kronecker(const ref a: bigint, b: uint): int

.. function:: proc kronecker(a: uint, const ref b: bigint): int

.. method:: proc bigint.divexact(const ref n: bigint, const ref d: bigint)

   
   Computes ``n/d`` and stores the result in ``bigint`` instance.
   
   ``divexact`` is optimized to handle cases where ``n/d`` results in an integer.
   When ``n/d`` does not produce an integer, this method may produce incorrect results.
   
   :arg n: numerator
   
   :type n: bigint
   
   :arg d: denominator
   
   :type d: bigint

.. method:: proc bigint.divexact(const ref n: bigint, d: integral)

.. method:: proc bigint.divisible_p(const ref d: bigint): int

.. method:: proc bigint.divisible_p(d: int): int

.. method:: proc bigint.divisible_p(d: uint): int

.. method:: proc bigint.divisible_2exp_p(b: integral): int

.. method:: proc bigint.congruent_p(const ref c: bigint, const ref d: bigint): int

.. method:: proc bigint.congruent_p(c: integral, d: integral): int

.. method:: proc bigint.congruent_2exp_p(const ref c: bigint, b: integral): int

.. method:: proc bigint.powm(const ref base: bigint, const ref exp: bigint, const ref mod: bigint)

.. method:: proc bigint.powm(const ref base: bigint, exp: int, const ref mod: bigint)

.. method:: proc bigint.powm(const ref base: bigint, exp: uint, const ref mod: bigint)

.. method:: proc bigint.pow(const ref base: bigint, exp: int)

.. method:: proc bigint.pow(const ref base: bigint, exp: uint)

.. method:: proc bigint.pow(base: int, exp: int)

.. method:: proc bigint.pow(base: uint, exp: uint)

.. method:: proc bigint.root(const ref a: bigint, n: uint): int

.. method:: proc bigint.rootrem(ref rem: bigint, const ref u: bigint, n: uint)

.. method:: proc bigint.sqrt(const ref a: bigint)

.. method:: proc bigint.sqrtrem(ref rem: bigint, const ref a: bigint)

.. method:: proc bigint.perfect_power_p(): int

.. method:: proc bigint.perfect_square_p(): int

.. method:: proc bigint.probab_prime_p(reps: int): int

.. method:: proc bigint.nextprime(const ref a: bigint)

.. method:: proc bigint.gcd(const ref a: bigint, const ref b: bigint)

.. method:: proc bigint.gcd(const ref a: bigint, b: int)

.. method:: proc bigint.gcd(const ref a: bigint, b: uint)

.. method:: proc bigint.gcdext(ref s: bigint, ref t: bigint, const ref a: bigint, const ref b: bigint)

.. method:: proc bigint.lcm(const ref a: bigint, const ref b: bigint)

.. method:: proc bigint.lcm(const ref a: bigint, b: int)

.. method:: proc bigint.lcm(const ref a: bigint, b: uint)

.. method:: proc bigint.invert(const ref a: bigint, const ref b: bigint): int

.. method:: proc bigint.remove(const ref a: bigint, const ref f: bigint): uint

.. method:: proc bigint.fac(a: integral)

.. method:: proc bigint.bin(const ref n: bigint, k: integral)

.. method:: proc bigint.bin(n: uint, k: integral)

.. method:: proc bigint.fib(n: integral)

.. method:: proc bigint.fib2(ref fnsub1: bigint, n: integral)

.. method:: proc bigint.lucnum(n: integral)

.. method:: proc bigint.lucnum2(ref fnsub1: bigint, n: integral)

.. method:: proc bigint.popcount(): uint

.. method:: proc bigint.hamdist(const ref b: bigint): uint

.. method:: proc bigint.scan0(starting_bit: integral): uint

.. method:: proc bigint.scan1(starting_bit: integral): uint

.. method:: proc bigint.setbit(bit_index: integral)

.. method:: proc bigint.clrbit(bit_index: integral)

.. method:: proc bigint.combit(bit_index: integral)

.. method:: proc bigint.tstbit(bit_index: integral): int

.. method:: proc bigint.fits_ulong_p(): int

.. method:: proc bigint.fits_slong_p(): int

.. method:: proc bigint.fits_uint_p(): int

.. method:: proc bigint.fits_sint_p(): int

.. method:: proc bigint.fits_ushort_p(): int

.. method:: proc bigint.fits_sshort_p(): int

.. method:: proc bigint.even_p(): int

.. method:: proc bigint.odd_p(): int

.. method:: proc bigint.add(const ref a: bigint, const ref b: bigint)

.. method:: proc bigint.add(const ref a: bigint, b: int)

.. method:: proc bigint.add(const ref a: bigint, b: uint)

.. method:: proc bigint.sub(const ref a: bigint, const ref b: bigint)

.. method:: proc bigint.sub(const ref a: bigint, b: int)

.. method:: proc bigint.sub(const ref a: bigint, b: uint)

.. method:: proc bigint.sub(a: int, const ref b: bigint)

.. method:: proc bigint.sub(a: uint, const ref b: bigint)

.. method:: proc bigint.mul(const ref a: bigint, const ref b: bigint)

.. method:: proc bigint.mul(const ref a: bigint, b: int)

.. method:: proc bigint.mul(const ref a: bigint, b: uint)

.. method:: proc bigint.addmul(const ref a: bigint, const ref b: bigint)

.. method:: proc bigint.addmul(const ref a: bigint, b: int)

.. method:: proc bigint.addmul(const ref a: bigint, b: uint)

.. method:: proc bigint.submul(const ref a: bigint, const ref b: bigint)

.. method:: proc bigint.submul(const ref a: bigint, b: int)

.. method:: proc bigint.submul(const ref a: bigint, b: uint)

.. method:: proc bigint.mul_2exp(const ref a: bigint, b: integral)

.. method:: proc bigint.neg(const ref a: bigint)

.. method:: proc bigint.abs(const ref a: bigint)

.. method:: proc bigint.div_q(const ref n: bigint, const ref d: bigint, param rounding = Round.ZERO)

.. method:: proc bigint.div_q(const ref n: bigint, d: integral, param rounding = Round.ZERO)

.. method:: proc bigint.div_r(const ref n: bigint, const ref d: bigint, param rounding = Round.ZERO)

.. method:: proc bigint.div_r(const ref n: bigint, d: integral, param rounding = Round.ZERO)

.. method:: proc bigint.div_qr(ref r: bigint, const ref n: bigint, const ref d: bigint, param rounding = Round.ZERO)

.. method:: proc bigint.div_qr(ref r: bigint, const ref n: bigint, d: integral, param rounding = Round.ZERO)

.. method:: proc bigint.div_q_2exp(const ref n: bigint, b: integral, param rounding = Round.ZERO)

.. method:: proc bigint.div_r_2exp(const ref n: bigint, b: integral, param rounding = Round.ZERO)

.. method:: proc bigint.mod(const ref a: bigint, const ref b: bigint)

.. method:: proc bigint.mod(const ref a: bigint, b: integral): uint

.. method:: proc bigint.cmp(const ref b: bigint): int

.. method:: proc bigint.cmp(b: int): int

.. method:: proc bigint.cmp(b: uint): int

.. method:: proc bigint.cmp(b: real): int

.. method:: proc bigint.cmpabs(const ref b: bigint): int

.. method:: proc bigint.cmpabs(b: uint): int

.. method:: proc bigint.cmpabs(b: real): int

.. method:: proc bigint.sgn(): int

.. method:: proc bigint.and(const ref a: bigint, const ref b: bigint)

.. method:: proc bigint.ior(const ref a: bigint, const ref b: bigint)

.. method:: proc bigint.xor(const ref a: bigint, const ref b: bigint)

.. method:: proc bigint.com(const ref a: bigint)

.. method:: proc bigint.set(const ref a: bigint)

.. method:: proc bigint.set(num: int)

.. method:: proc bigint.set(num: uint)

.. method:: proc bigint.set(num: real)

.. method:: proc bigint.set(str: string, base: int = 0)

.. method:: proc bigint.swap(ref a: bigint)

