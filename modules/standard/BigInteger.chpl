/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

/* Provides a 'bigint' type and supporting math operations.

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

 fac(c, 00);
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

The Chapel compiler currently introduces two short lived temporaries for the
intermediate results of the binary operators.

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

*/

module BigInteger {
  use CTypes;
  use GMP;
  use HaltWrappers;
  use OS;


  /*
   Local copy of IO.EFORMAT as it is being phased out and is private in IO
   */
  private extern proc chpl_macro_int_EFORMAT():c_int;

  /*
    .. warning::

       The enum Round is deprecated, please use the enum round instead
  */
  @deprecated(notes="The enum Round is deprecated, please use the enum round instead")
  enum Round {
    DOWN = -1,
    ZERO =  0,
    UP   =  1
  }

  /* An enumeration of the different rounding strategies, for use with e.g.
     :proc:`~bigint.divQ` to determine how to round the quotient when performing
     the computation.

     - ``round.down`` indicates that the quotient should be rounded down towards
       -infinity and any remainder should have the same sign as the denominator.
     - ``round.zero`` indicates that the quotient should be rounded towards zero
       and any remainder should have the same sign as the numerator.
     - ``round.up`` indicates that the quotient should be rounded up towards
       +infinity and any remainder should have the opposite sign as the
       denominator.
   */
  enum round {
    down = -1,
    zero = 0,
    up = 1
  }

  /* A compile-time parameter to control the behavior of bigint initializers
     that take a string argument.

     When ``false``, the deprecated behavior is used (i.e., errors will trigger
     a halt at execution.)

     When ``true``, the new behavior is used (i.e., errors will cause a
     :type:`~OS.BadFormatError` to be thrown)
  */
  config param bigintInitThrows = false;

  // TODO: remove when initializers can throw in their body
  private proc throwingInitWorkaround() throws {
    throw new BadFormatError("Error initializing big integer");
  }

  pragma "ignore noinit"
  record bigint {
    // The underlying GMP C structure
    pragma "no init"
    @chpldoc.nodoc
    var mpz      : mpz_t;              // A dynamic-vector of C integers

    @chpldoc.nodoc
    var localeId : chpl_nodeID_t;      // The locale id for the GMP state

    proc init() {
      this.complete();
      mpz_init(this.mpz);

      this.localeId = chpl_nodeID;
    }

    proc init(const ref num: bigint) {
      this.complete();
      if _local || num.localeId == chpl_nodeID {
        mpz_init_set(this.mpz, num.mpz);
      } else {
        var mpz_struct = num.getImpl();

        mpz_init(this.mpz);

        chpl_gmp_get_mpz(this.mpz, num.localeId, mpz_struct);
      }

      this.localeId = chpl_nodeID;
    }

    proc init=(const ref num: bigint) {
      this.init(num);
    }

    proc init(num: int) {
      this.complete();
      mpz_init_set_si(this.mpz, num.safeCast(c_long));

      this.localeId = chpl_nodeID;
    }

    proc init(num: uint) {
      this.complete();
      mpz_init_set_ui(this.mpz, num.safeCast(c_ulong));

      this.localeId = chpl_nodeID;
    }

    proc init=(num : integral) {
      this.init(num);
    }

    @deprecated(notes="bigint initializers that halt are deprecated, please set the config param :param:`bigintInitThrows` to 'true' to opt in to using the new initializer that throws")
    proc init(str: string, base: int = 0) where bigintInitThrows == false {
      this.complete();
      const str_  = str.localize().c_str();
      const base_ = base.safeCast(c_int);

      if mpz_init_set_str(this.mpz, str_, base_) != 0 {
        mpz_clear(this.mpz);

        HaltWrappers.initHalt("Error initializing big integer: bad format");
      }

      this.localeId = chpl_nodeID;
    }

    @deprecated(notes="bigint initializers that return the errorCode type via an 'out' argument are deprecated, please remove the argument and ensure the config param :param:`bigintInitThrows` is set to 'true' to opt in to using the new initializer that throws")
    proc init(str: string, base: int = 0, out error: errorCode) {

      this.complete();
      const str_  = str.localize().c_str();
      const base_ = base.safeCast(c_int);

      if mpz_init_set_str(this.mpz, str_, base_) != 0 {
        mpz_clear(this.mpz);

        error = chpl_macro_int_EFORMAT();
      } else {
        error = 0;
      }

      this.localeId = chpl_nodeID;
    }

    /* Initialize a :type:`bigint` from a string and optionally a provided base
       to use with the string.  If the string is not a correct base ``base``
       number, will throw a :type:`~OS.BadFormatError`.

       :arg str: The value to be stored in the resulting :type:`bigint`.
       :type str: `string`

       :arg base: The base to use when creating the :type:`bigint` from ``str``.
                  May vary from ``2`` to ``62`` or be ``0``.  Defaults to ``0``,
                  which causes the base to be read from the start of the ``str``
                  itself (``0x`` and ``0X`` will give hexadecimal, ``0b`` and
                  ``0B`` will give binary, ``0`` will give octal, and everything
                  else will be interpreted as decimal).
       :type base: `int`

       :throws BadFormatError: Thrown when ``str`` is not a correctly formatted
                               number in base ``base``.

     */
    proc init(str: string, base: int = 0) throws where bigintInitThrows == true {
      this.complete();
      const str_  = str.localize().c_str();
      const base_ = base.safeCast(c_int);

      if mpz_init_set_str(this.mpz, str_, base_) != 0 {
        mpz_clear(this.mpz);

        throwingInitWorkaround();
      }

      this.localeId = chpl_nodeID;
    }

    // Within a given locale, bigint assignment creates a deep copy of the
    // data and so the record "owns" the GMP data.
    //
    // If a bigint is copied to a remote node then it will receive a shallow
    // copy.  The localeId points back the correct locale but the mpz field
    // is meaningless.
    @chpldoc.nodoc
    proc deinit() {
      if _local || this.localeId == chpl_nodeID {
        mpz_clear(this.mpz);
      }
    }

    /*
      .. warning::

         bigint.size() is @deprecated
    */
    @deprecated(notes="bigint.size() is deprecated")
    proc size() : c_size_t {
      var ret: c_size_t;

      if _local {
        ret = mpz_size(this.mpz);

      } else if this.localeId == chpl_nodeID {
        ret = mpz_size(this.mpz);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          ret = mpz_size(this.mpz);
        }
      }

      return ret;
    }

    /*
      .. warning::

         bigint.sizeinbase() is deprecated, use bigint.sizeInBase() instead
    */
    @deprecated
    ("bigint.sizeinbase() is deprecated, use bigint.sizeInBase() instead")
    proc sizeinbase(base: int) : uint {
      return sizeInBase(base).safeCast(uint);
    }

    /* Determine the size of ``this`` measured in number of digits in the given
       ``base``.  The sign of ``this`` is ignored, only the absolute value is
       used.

       :arg base: The base in which to compute the number of digits used to
                  represent ``this``.  Can be between 2 and 62.
       :type base: ``int``

       :returns: The size of ``this`` measured in number of digits in the given
                 ``base``.  Will either be exact or 1 too big.  If ``base`` is
                 a power of 2, will always be exact.  If ``this`` is 0, will
                 always return 1.
       :rtype: ``int``
     */
    proc sizeInBase(base: int) : int {
      const base_ = base.safeCast(c_int);
      var   ret: c_size_t;

      if _local {
        ret = mpz_sizeinbase(this.mpz, base_);

      } else if this.localeId == chpl_nodeID {
        ret = mpz_sizeinbase(this.mpz, base_);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          ret = mpz_sizeinbase(this.mpz, base_);
        }
      }

      return ret.safeCast(int);
    }

    @deprecated(notes="This method is deprecated, please use :proc:`GMP.chpl_gmp_mpz_nlimbs` on the mpz field instead")
    proc numLimbs : uint {
      return chpl_gmp_mpz_nlimbs(this.mpz);
    }

    @deprecated(notes="This method is deprecated, please use :proc:`GMP.chpl_gmp_mpz_getlimbn` on the mpz field instead")
    proc get_limbn(n: integral) : uint {
      var   ret: uint;

      if _local {
        ret = chpl_gmp_mpz_getlimbn(this.mpz, n);

      } else if this.localeId == chpl_nodeID {
        ret = chpl_gmp_mpz_getlimbn(this.mpz, n);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          ret = chpl_gmp_mpz_getlimbn(this.mpz, n);
        }
      }

      return ret;
    }

    @deprecated(notes="mpzStruct is deprecated, please use :proc:`getImpl` instead")
    proc mpzStruct() : __mpz_struct {
      return getImpl();
    }

    /* Return the underlying implementation of :record:`bigint`.  Currently,
       the type returned is ``__mpz_struct``.

       This method is provided as a convenience but its result may change in the
       future.
    */
    proc getImpl(): __mpz_struct {
      var ret: __mpz_struct;

      if _local {
        ret = this.mpz[0];

      } else if this.localeId == chpl_nodeID {
        ret = this.mpz[0];

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          ret = this.mpz[0];
        }
      }

      return ret;
    }

    @deprecated(notes="get_d_2exp is deprecated in favor of :proc:`bigint.getD2Exp`, which returns (d, exp) instead of (exp, d).  Please use that method instead")
    proc get_d_2exp() : (uint(32), real) {
      var (dbl, exp) = getD2Exp();
      return (exp, dbl);
    }

    /*
      Convert ``this`` to a tuple containing a real (truncated if necessary, by
      rounding towards zero) and the exponent.  The returned tuple fulfills the
      condition ``d * 2^exp == this`` where ``d`` is the first value in the
      tuple and ``exp`` is the second.

      :returns: a tuple representing the number in multiple parts, ``(d, exp)``,
                such that their combination ``d * 2^exp`` is equal to ``this``.

                ``d`` in this case will be in the range ``0.5 <= abs(d) < 1``,
                unless ``this`` is ``0``, in which case ``d == 0.0`` and
                ``exp == 0``.
      :rtype: ``(real, uint(32))``
     */
    proc getD2Exp(): (real, uint(32)) {
      var exp: c_long;
      var dbl: c_double;

      if _local {
        var tmp: c_long;

        dbl = mpz_get_d_2exp(tmp, this.mpz);
        exp = tmp;

      } else if this.localeId == chpl_nodeID {
        var tmp: c_long;

        dbl = mpz_get_d_2exp(tmp, this.mpz);
        exp = tmp;

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          var tmp: c_long;

          dbl = mpz_get_d_2exp(tmp, this.mpz);
          exp = tmp;
        }
      }

      return (dbl: real, exp.safeCast(uint(32)));
    }

    proc get_str(base: int = 10) : string {
      const base_ = base.safeCast(c_int);
      var   ret: string;

      if _local {
        var tmpvar = chpl_gmp_mpz_get_str(base_, this.mpz);

        try! {
          ret = createStringWithOwnedBuffer(tmpvar);
        }

      } else if this.localeId == chpl_nodeID {
        var tmpvar = chpl_gmp_mpz_get_str(base_, this.mpz);

        try! {
          ret = createStringWithOwnedBuffer(tmpvar);
        }

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          var tmpvar = chpl_gmp_mpz_get_str(base_, this.mpz);

          try! {
            ret = createStringWithOwnedBuffer(tmpvar);
          }
        }
      }

      return ret;
    }

    proc writeThis(writer) throws {
      var s: string;

      if _local {
        s = this.get_str();

      } else if this.localeId == chpl_nodeID {
        s = this.get_str();

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          s = this.get_str();
        }
      }

      writer.write(s);
    }
  }

  //
  // Cast operators
  //
  @chpldoc.nodoc
  inline operator :(src: integral, type toType: bigint): bigint {
    return new bigint(src);
  }

  @chpldoc.nodoc
  inline operator :(src: string, type toType: bigint): bigint throws {
    return new bigint(src);
  }

  @chpldoc.nodoc
  inline operator :(const ref x: bigint, type t:numeric) where isIntType(t) {
    var ret: c_long;

    if _local {
      ret = mpz_get_si(x.mpz);

    } else if x.localeId == chpl_nodeID {
        ret = mpz_get_si(x.mpz);

    } else {
      const xLoc = chpl_buildLocaleID(x.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", xLoc) {
        ret = mpz_get_si(x.mpz);
      }
    }

    return ret:t;
  }

  @chpldoc.nodoc
  inline operator :(const ref x: bigint, type t:numeric) where isUintType(t) {
    var ret: c_ulong;

    if _local {
      ret = mpz_get_ui(x.mpz);

    } else if x.localeId == chpl_nodeID {
      ret = mpz_get_ui(x.mpz);

    } else {
      const xLoc = chpl_buildLocaleID(x.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", xLoc) {
        ret = mpz_get_ui(x.mpz);
      }
    }

    return ret:t;
  }

  @chpldoc.nodoc
  inline operator :(const ref x: bigint, type t:numeric) where isRealType(t) {
    var ret: c_double;

    if _local {
      ret = mpz_get_d(x.mpz);

    } else if x.localeId == chpl_nodeID {
      ret = mpz_get_d(x.mpz);

    } else {
      const xLoc = chpl_buildLocaleID(x.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", xLoc) {
        ret = mpz_get_d(x.mpz);
      }
    }

    return ret:t;
  }

  //
  // Locale-aware assignment
  //

  operator bigint.=(ref lhs: bigint, const ref rhs: bigint) {
    inline proc helper() {
      if rhs.localeId == chpl_nodeID {
        mpz_set(lhs.mpz, rhs.mpz);

      } else {
        chpl_gmp_get_mpz(lhs.mpz, rhs.localeId, rhs.mpz[0]);
      }
    }

    if _local {
      mpz_set(lhs.mpz, rhs.mpz);

    } else if lhs.localeId == chpl_nodeID {
      helper();

    } else {
      var lhsLoc = chpl_buildLocaleID(lhs.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", lhsLoc) {
        helper();
      }
    }
  }

  operator bigint.=(ref lhs: bigint, rhs: int) {
    const rhs_ = rhs.safeCast(c_long);

    if _local {
      mpz_set_si(lhs.mpz, rhs_);

    } else if lhs.localeId == chpl_nodeID {
      mpz_set_si(lhs.mpz, rhs_);

    } else {
      var lhsLoc = chpl_buildLocaleID(lhs.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", lhsLoc) {
        mpz_set_si(lhs.mpz, rhs_);
      }
    }
  }

  operator bigint.=(ref lhs: bigint, rhs: uint) {
    const rhs_ = rhs.safeCast(c_ulong);

    if _local {
      mpz_set_ui(lhs.mpz, rhs_);

    } else if lhs.localeId == chpl_nodeID {
      mpz_set_ui(lhs.mpz, rhs_);

    } else {
      var lhsLoc = chpl_buildLocaleID(lhs.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", lhsLoc) {
        mpz_set_ui(lhs.mpz, rhs_);
      }
    }
  }

  //
  // Operations on bigints
  //
  // In general we need to think about 3 cases
  //
  //   1) This is a single-locale configuration.
  //      We can invoke the appropriate mpz operator directly.
  //
  //   2) All bigints are on the current locale.
  //      We can invoke the appropriate mpz operator directly.
  //
  //   3) One or more bigints are on a remote locale.
  //      This is complicated.  It is tempting to handle all of the
  //      permutations as efficiently as possible but this introduces
  //      a lot of cases esp. for binary operations.
  //
  //      Instead we use just one handler to
  //        a) Move to this.localeId
  //        b) Use assignment operator to copy operands to current locale
  //        c) Invoke the appropriate mpz operator directly
  //
  //      Later we can use profiling to add optimizations where appropriate
  //

  //
  // Unary operators
  //
  operator bigint.+(const ref a: bigint): bigint {
    return new bigint(a);
  }

  operator bigint.-(const ref a: bigint): bigint {
    var c = new bigint(a);
    BigInteger.neg(c, a);
    return c;
  }

  operator bigint.~(const ref a: bigint): bigint {
    var c = new bigint(a);
    BigInteger.com(c, a);
    return c;
  }

  //
  // Binary operators
  //

  // Addition
  operator bigint.+(const ref a: bigint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.add(c, a, b);
    return c;
  }

  operator bigint.+(const ref a: bigint, b: int): bigint {
    var c = new bigint();
    BigInteger.add(c, a, b);
    return c;
  }
  operator bigint.+(const ref a: bigint, b: uint): bigint {
    var c = new bigint();
    BigInteger.add(c, a, b);
    return c;
  }

  operator bigint.+(a: int, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.add(c, b, a);
    return c;
  }
  operator bigint.+(a: uint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.add(c, b, a);
    return c;
  }



  // Subtraction
  operator bigint.-(const ref a: bigint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.sub(c, a, b);
    return c;
  }

  operator bigint.-(const ref a: bigint, b: int): bigint {
    var c = new bigint();
    BigInteger.sub(c, a, b);
    return c;
  }
  operator bigint.-(const ref a: bigint, b: uint): bigint {
    var c = new bigint();
    BigInteger.sub(c, a, b);
    return c;
  }

  operator bigint.-(a: int, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.sub(c, a, b);
    return c;
  }
  operator bigint.-(a: uint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.sub(c, a, b);
    return c;
  }

  // Multiplication
  operator bigint.*(const ref a: bigint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.mul(c, a, b);
    return c;
  }

  operator bigint.*(const ref a: bigint, b: int): bigint {
    var c = new bigint();
    BigInteger.mul(c, a, b);
    return c;
  }
  operator bigint.*(const ref a: bigint, b: uint): bigint {
    var c = new bigint();
    BigInteger.mul(c, a, b);
    return c;
  }

  operator bigint.*(a: int, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.mul(c, b, a);
    return c;
  }
  operator bigint.*(a: uint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.mul(c, b, a);
    return c;
  }


  // Division
  // Documented in (bigint, integral) version
  operator bigint./(const ref a: bigint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.divQ(c, a, b, round.zero);

    return c;
  }

  /* Divide ``a`` by ``b``, returning the result.

     :arg a: The numerator of the division operation
     :type a: :record:`bigint`

     :arg b: The denominator of the division operation
     :type b: :record:`bigint` or ``integral``

     :returns: The result of dividing ``a`` by ``b``
     :rtype: :record:`bigint`
   */
  operator bigint./(const ref a: bigint, b: integral): bigint {
    return a / new bigint(b);
  }

  // Exponentiation
  operator bigint.**(const ref base: bigint, const ref exp: bigint): bigint {
    var c = new bigint();

    if exp >= 0 {
      if exp.fitsInto(c_ulong) then
        BigInteger.pow(c, base, exp: c_ulong);
      else
        halt("Exponent too large to compute result.");
    } else {
      if exp.fitsInto(int) then
        BigInteger.pow(c, base, exp: int);
      else
        halt("Exponent too large to compute result.");
    }

    return c;
  }

  operator bigint.**(const ref base: bigint, exp: int): bigint {
    var c = new bigint();

    if exp >= 0 {
      BigInteger.pow(c, base, exp: c_ulong);
    } else {
      BigInteger.pow(c, base, exp: int);
    }

    return c;
  }

  operator bigint.**(const ref base: bigint, exp: uint): bigint {
    const exp_ = exp.safeCast(c_ulong);
    var   c    = new bigint();

    BigInteger.pow(c, base, exp_);

    return c;
  }



  /* Computes the mod operator on the two arguments, defined as
     ``a % b = a - b * trunc(a / b)``.

     The result is always >= 0 if `a` > 0.
     It is an error if `b` == 0.
  */
  operator bigint.%(const ref a: bigint, const ref b: bigint): bigint {
    const a_ = a.localize();
    const b_ = b.localize();
    var c = new bigint();

    mpz_tdiv_r(c.mpz, a_.mpz, b_.mpz);

    return c;
  }

  /* Computes the mod operator on the two arguments, defined as
     ``a % b = a - b * trunc(a / b)``.

     The result is always >= 0 if `a` > 0.
     It is an error if `b` == 0.
  */
  operator bigint.%(const ref a: bigint, b: int): bigint {
    var c = new bigint();
    const a_ = a.localize();
    var b_ : c_ulong;

    if b >= 0 then
      b_ = b.safeCast(c_ulong);
    else
      b_ = (0 - b).safeCast(c_ulong);

    mpz_tdiv_r_ui(c.mpz, a_.mpz, b_);

    return c;
  }

  /* Computes the mod operator on the two arguments, defined as
     ``a % b = a - b * trunc(a / b)``.

     The result is always >= 0 if `a` > 0.
     It is an error if `b` == 0.
  */
  operator bigint.%(const ref a: bigint, b: uint): bigint {
    const a_ = a.localize();
    const b_ = b.safeCast(c_ulong);
    var   c  = new bigint();

    mpz_tdiv_r_ui(c.mpz, a_.mpz, b_);

    return c;
  }


  private inline proc shiftLeft(ref result: bigint, const ref a: bigint, b: int) {
    if b >= 0 {
      shiftLeft(result, a, b:uint);
    } else {
      BigInteger.divQ2Exp(result, a, (0 - b):uint, round.down);
    }
  }
  private inline proc shiftLeft(ref result: bigint, const ref a: bigint, b: uint) {
    BigInteger.mul_2exp(result, a, b);
  }

  // Bit-shift left
  operator bigint.<<(const ref a: bigint, b: int): bigint {
    var c = new bigint();
    BigInteger.shiftLeft(c, a, b);
    return c;
  }
  operator bigint.<<(const ref a: bigint, b: uint): bigint {
    var c = new bigint();
    BigInteger.shiftLeft(c, a, b);
    return c;
  }


  private inline proc shiftRight(ref result: bigint, const ref a: bigint, b: int) {
    if b >= 0 {
      shiftRight(result, a, b:uint);
    } else {
      BigInteger.mul_2exp(result, a, (0 - b):uint);
    }
  }
  private inline proc shiftRight(ref result: bigint, const ref a: bigint, b: uint) {
    BigInteger.divQ2Exp(result, a, b, round.down);
  }


  // Bit-shift right
  operator bigint.>>(const ref a: bigint, b: int): bigint {
    var c = new bigint();
    BigInteger.shiftRight(c, a, b);
    return c;
  }
  operator bigint.>>(const ref a: bigint, b: uint): bigint {
    var c = new bigint();
    BigInteger.shiftRight(c, a, b);
    return c;
  }

  // Bitwise and
  operator bigint.&(const ref a: bigint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.and(c, a, b);
    return c;
  }



  // Bitwise ior
  operator bigint.|(const ref a: bigint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.ior(c, a, b);
    return c;
  }



  // Bitwise xor
  operator bigint.^(const ref a: bigint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.xor(c, a, b);
    return c;
  }



  //
  // Comparison Operations
  //

  private inline proc cmp(const ref a: bigint, const ref b: bigint)
    do return a.cmp(b);

  private inline proc cmp(const ref a: bigint, b: int)
    do return a.cmp(b);
  private inline proc cmp(const ref a: bigint, b: uint)
    do return a.cmp(b);

  private inline proc cmp(a: int, const ref b: bigint)
    do return 0 - (b.cmp(a));
  private inline proc cmp(a: uint, const ref b: bigint)
    do return 0 - (b.cmp(a));


  // Equality
  operator bigint.==(const ref a: bigint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) == 0;

  operator bigint.==(const ref a: bigint, b: int): bool
    do return BigInteger.cmp(a, b) == 0;
  operator bigint.==(const ref a: bigint, b: uint): bool
    do return BigInteger.cmp(a, b) == 0;

  operator bigint.==(a: int, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) == 0;
  operator bigint.==(a: uint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) == 0;


  // Inequality
  operator bigint.!=(const ref a: bigint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) != 0;

  operator bigint.!=(const ref a: bigint, b: int): bool
    do return BigInteger.cmp(a, b) != 0;
  operator bigint.!=(const ref a: bigint, b: uint): bool
    do return BigInteger.cmp(a, b) != 0;

  operator bigint.!=(a: int, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) != 0;
  operator bigint.!=(a: uint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) != 0;


  // Greater than
  operator bigint.>(const ref a: bigint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) > 0;

  operator bigint.>(const ref a: bigint, b: int): bool
    do return BigInteger.cmp(a, b) > 0;
  operator bigint.>(const ref a: bigint, b: uint): bool
    do return BigInteger.cmp(a, b) > 0;

  operator bigint.>(a: int, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) > 0;
  operator bigint.>(a: uint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) > 0;


  // Less than
  operator bigint.<(const ref a: bigint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) < 0;

  operator bigint.<(const ref a: bigint, b: int): bool
    do return BigInteger.cmp(a, b) < 0;
  operator bigint.<(const ref a: bigint, b: uint): bool
    do return BigInteger.cmp(a, b) < 0;

  operator bigint.<(a: int, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) < 0;
  operator bigint.<(a: uint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) < 0;


  // Greater than or equal
  operator bigint.>=(const ref a: bigint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) >= 0;

  operator bigint.>=(const ref a: bigint, b: int): bool
    do return BigInteger.cmp(a, b) >= 0;
  operator bigint.>=(const ref a: bigint, b: uint): bool
    do return BigInteger.cmp(a, b) >= 0;

  operator bigint.>=(a: int, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) >= 0;
  operator bigint.>=(a: uint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) >= 0;


  // Less than or equal
  operator bigint.<=(const ref a: bigint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) <= 0;

  operator bigint.<=(const ref a: bigint, b: int): bool
    do return BigInteger.cmp(a, b) <= 0;
  operator bigint.<=(const ref a: bigint, b: uint): bool
    do return BigInteger.cmp(a, b) <= 0;

  operator bigint.<=(a: int, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) <= 0;
  operator bigint.<=(a: uint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) <= 0;


  //
  // Compound Assignment Operations
  //

  // +=
  operator bigint.+=(ref a: bigint, const ref b: bigint)
    do BigInteger.add(a, a, b);

  operator bigint.+=(ref a: bigint, b: int)
    do BigInteger.add(a, a, b);
  operator bigint.+=(ref a: bigint, b: uint)
    do BigInteger.add(a, a, b);



  // -=
  operator bigint.-=(ref a: bigint, const ref b: bigint)
    do BigInteger.sub(a, a, b);

  operator bigint.-=(ref a: bigint, b: int)
    do BigInteger.sub(a, a, b);
  operator bigint.-=(ref a: bigint, b: uint)
    do BigInteger.sub(a, a, b);



  // *=
  operator bigint.*=(ref a: bigint, const ref b: bigint)
    do BigInteger.mul(a, a, b);

  operator bigint.*=(ref a: bigint, b:  int)
    do BigInteger.mul(a, a, b);
  operator bigint.*=(ref a: bigint, b: uint)
    do BigInteger.mul(a, a, b);



  // /=
  // Documented in (bigint, integral) version
  operator bigint./=(ref a: bigint, const ref b: bigint) {
    BigInteger.divQ(a, a, b, round.zero);
  }

  /* Divide ``a`` by ``b``, storing the result in ``a``.

     :arg a: The numerator of the division operation
     :type a: :record:`bigint`

     :arg b: The denominator of the division operation
     :type b: :record:`bigint` or ``integral``
   */
  operator bigint./=(ref a: bigint, b: integral) {
    a /= new bigint(b);
  }



  // **=
  operator bigint.**=(ref base: bigint, const ref exp: bigint) {
    base = base ** exp;
  }

  operator bigint.**=(ref base: bigint, exp: int) {
    BigInteger.pow(base, base, exp);
  }

  operator bigint.**=(ref base: bigint, exp: uint) {
    BigInteger.pow(base, base, exp);
  }



  /* Mod ``a`` by ``b``, storing the result in ``a``.

     Here, the modulo operation is defined as
     ``a % b = a - b * trunc(a / b)``.

     The result is always >= 0 if `a` > 0.
     It is an error if `b` == 0.
  */
  operator bigint.%=(ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_tdiv_r(a.mpz, a.mpz, b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_tdiv_r(a.mpz, a.mpz, b.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_tdiv_r(a.mpz, a.mpz, b_.mpz);
      }
    }
  }

  /* Mod ``a`` by ``b``, storing the result in ``a``.

     Here, the modulo operation is defined as
     ``a % b = a - b * trunc(a / b)``.

     The result is always >= 0 if `a` > 0.
     It is an error if `b` == 0.
  */
  operator bigint.%=(ref a: bigint, b: int) {
    var b_ = 0 : uint;

    if b >= 0 then
      b_ = b       : uint;
    else
      b_ = (0 - b) : uint;

    a %= b_;
  }

  /* Mod ``a`` by ``b``, storing the result in ``a``.

     Here, the modulo operation is defined as
     ``a % b = a - b * trunc(a / b)``.

     The result is always >= 0 if `a` > 0.
     It is an error if `b` == 0.
  */
  operator bigint.%=(ref a: bigint, b: uint) {
    var b_ = b.safeCast(c_ulong);

    if _local {
      mpz_tdiv_r_ui(a.mpz, a.mpz, b_);

    } else if a.localeId == chpl_nodeID {
      mpz_tdiv_r_ui(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_tdiv_r_ui(a.mpz, a.mpz, b_);
      }
    }
  }

  operator bigint.&=(ref a: bigint, const ref b: bigint) {
    BigInteger.and(a, a, b);
  }

  operator bigint.|=(ref a: bigint, const ref b: bigint) {
    BigInteger.ior(a, a, b);
  }

  operator bigint.^=(ref a: bigint, const ref b: bigint) {
    BigInteger.xor(a, a, b);
  }


  // <<=
  operator bigint.<<=(ref a: bigint, b: int)
    do BigInteger.shiftLeft(a, a, b);
  operator bigint.<<=(ref a: bigint, b: uint)
    do BigInteger.shiftLeft(a, a, b);



  // >>=
  operator bigint.>>=(ref a: bigint, b:  int)
    do BigInteger.shiftRight(a, a, b);
  operator bigint.>>=(ref a: bigint, b: uint)
    do BigInteger.shiftRight(a, a, b);


  // Swap
  operator bigint.<=>(ref a: bigint, ref b: bigint) {
    if _local {
      var t = a;

      mpz_set(a.mpz, b.mpz);
      mpz_set(b.mpz, t.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      var t = a;

      mpz_set(a.mpz, b.mpz);
      mpz_set(b.mpz, t.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);
      const bLoc = chpl_buildLocaleID(b.localeId, c_sublocid_any);

      const aTmp = a;

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_set(a.mpz, b_.mpz);
      }

      on __primitive("chpl_on_locale_num", bLoc) {
        const t_ = aTmp;

        mpz_set(b.mpz, t_.mpz);
      }
    }
  }


  // Special Operations
  proc jacobi(const ref a: bigint, const ref b: bigint) : int {
    const a_ = a.localize();
    const b_ = b.localize();
    var ret : c_int;

    ret = mpz_jacobi(a_.mpz, b_.mpz);

    return ret.safeCast(int);
  }



  proc legendre(const ref a: bigint, const ref p: bigint) : int {
    const a_ = a.localize();
    const p_ = p.localize();
    var ret : c_int;

    ret = mpz_legendre(a_.mpz, p_.mpz);

    return ret.safeCast(int);
  }



  // kronecker
  proc kronecker(const ref a: bigint, const ref b: bigint) : int {
    var ret : c_int;

    const a_ = a.localize();
    const b_ = b.localize();

    ret = mpz_kronecker(a_.mpz, b_.mpz);

    return ret.safeCast(int);
  }

  proc kronecker(const ref a: bigint, b: int) : int {
    const a_ = a.localize();
    const b_ = b.safeCast(c_long);
    var  ret : c_int;

    ret = mpz_kronecker_si(a_.mpz, b_);

    return ret.safeCast(int);
  }

  proc kronecker(a: int, const ref b: bigint) : int {
    const a_ = a.safeCast(c_long);
    const b_ = b.localize();
    var  ret : c_int;

    ret = mpz_si_kronecker(a_, b_.mpz);

    return ret.safeCast(int);
  }

  proc kronecker(const ref a: bigint, b: uint) : int {
    const a_ = a.localize();
    const b_ = b.safeCast(c_ulong);
    var  ret : c_int;

    ret = mpz_kronecker_ui(a_.mpz, b_);

    return ret.safeCast(int);
  }

  proc kronecker(a: uint, const ref b: bigint) : int {
    const a_ = b.safeCast(c_ulong);
    const b_ = b.localize();
    var  ret : c_int;

    ret = mpz_ui_kronecker(a_, b_.mpz);

    return ret.safeCast(int);
  }




  // divexact

  /*
    .. warning::

       n and d are deprecated - please use numer and denom respectively
  */
  pragma "last resort"
  @deprecated
  ("n and d are deprecated - please use numer and denom respectively")
  proc bigint.divexact(const ref n: bigint, const ref d: bigint) {
    BigInteger.divexact(this, numer=n, denom=d);
  }
  /*
    .. warning::

       n and d are deprecated - please use numer and denom respectively
  */
  pragma "last resort"
  @deprecated
  ("n and d are deprecated - please use numer and denom respectively")
  proc bigint.divexact(const ref n: bigint, d: integral) {
    BigInteger.divexact(this, numer=n,denom=new bigint(d));
  }

  // documented in bigint, integral version
  proc divexact(ref result: bigint, const ref numer: bigint, const ref denom: bigint) {
    if (chpl_checkDivByZero) then
      if denom == 0 then
        halt("Attempt to divide by zero");

    if _local {
      mpz_divexact(result.mpz, numer.mpz, denom.mpz);
    } else if result.localeId == chpl_nodeID {
      const numer_ = numer;
      const denom_ = denom;
      mpz_divexact(result.mpz, numer_.mpz, denom_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const numer_ = numer;
        const denom_ = denom;
        mpz_divexact(result.mpz, numer_.mpz, denom_.mpz);
      }
    }
  }

  // documented in bigint, integral version
  @deprecated(notes="bigint.divexact method is deprecated - please use the standalone function :proc:`~BigInteger.divexact`")
  proc bigint.divexact(const ref numer: bigint, const ref denom: bigint) {
    BigInteger.divexact(this, numer, denom);
  }

  /*
    Computes ``numer/denom`` and stores the result in ``result``, which is a
    :record:`bigint` instance.

    .. warning::

       ``divexact`` is optimized to handle cases where ``numer/denom`` results
       in an integer.  When ``numer/denom`` does not produce an integer, this
       method may produce incorrect results.

    :arg result: Where the result is stored
    :type result: :record:`bigint`

    :arg numer: numerator
    :type numer: :record:`bigint`

    :arg denom: denominator
    :type denom: :record:`bigint` or ``integral``
  */
  proc divexact(ref result: bigint, const ref numer: bigint, denom: integral) {
    BigInteger.divexact(result, numer, new bigint(denom));
  }

  /*
    Computes ``numer/denom`` and stores the result in ``this``, which is a
    :record:`bigint` instance.

    .. warning::

       ``divexact`` is optimized to handle cases where ``numer/denom`` results
       in an integer.  When ``numer/denom`` does not produce an integer, this
       method may produce incorrect results.

    :arg numer: numerator

    :type numer: :record:`bigint`

    :arg denom: denominator

    :type denom: :record:`bigint` or ``integral``
  */
  @deprecated(notes="bigint.divexact method is deprecated - please use the standalone function :proc:`~BigInteger.divexact`")
  proc bigint.divexact(const ref numer: bigint, denom: integral) {
    BigInteger.divexact(this, numer, denom);
  }

  // divisible_p
  /*
    .. warning::

       bigint.divisible_p is deprecated, use bigint.isDivisible instead
  */
  @deprecated
  ("bigint.divisible_p is deprecated, use bigint.isDivisible instead")
  proc bigint.divisible_p(const ref d: bigint) : int {
    return this.isDivisible(d);
  }
  /*
    .. warning::

       bigint.divisible_p is deprecated, use bigint.isDivisible instead
  */
  @deprecated
  ("bigint.divisible_p is deprecated, use bigint.isDivisible instead")
  proc bigint.divisible_p(d: int) : int {
    return this.isDivisible(d);
  }
  /*
    .. warning::

       bigint.divisible_p is deprecated, use bigint.isDivisible instead
  */
  @deprecated
  ("bigint.divisible_p is deprecated, use bigint.isDivisible instead")
  proc bigint.divisible_p(d: uint) : int {
    return this.isDivisible(d);
  }

  // divisible_p
  // documented in uint version
  proc bigint.isDivisible(const ref div: bigint) : bool {
    const t_ = this.localize();
    const div_ = div.localize();
    var ret: c_int;

    ret = mpz_divisible_p(this.mpz, div.mpz);

    if ret then
      return true;
    else
      return false;
  }

  // documented in uint version
  proc bigint.isDivisible(div: int) : bool {
    const t_ = this.localize();
    var div_ = 0 : c_ulong;
    var ret: c_int;

    if div >= 0 then
      div_ = div.safeCast(c_ulong);
    else
      div_ = (0 - div).safeCast(c_ulong);

    ret = mpz_divisible_ui_p(t_.mpz,   div_);

    if ret then
      return true;
    else
      return false;
  }

  /*
    Return ``true`` if ``this`` is exactly divisible by ``div``.  ``this`` is
    divisible by ``div`` if there exists an integer ``q`` satisfying ``this =
    q*div``.  Unlike the other division functions, ``0`` is an acceptable value
    for ``div`` and only ``0`` is considered divisible by ``0``.

    :arg div: number to check if ``this`` is divisible by
    :type div: :record:`bigint`, ``int`` or ``uint``

    :return: ``true`` if ``this`` is exactly divisible by ``div``, ``false``
             otherwise
    :rtype: ``bool``
   */
  proc bigint.isDivisible(div: uint) : bool {
    const t_ = this.localize();
    const div_ = div.safeCast(c_ulong);
    var   ret: c_int;

    ret = mpz_divisible_ui_p(t_.mpz, div_);

    if ret then
      return true;
    else
      return false;
  }

  /*
    .. warning::

       bigint.divisible_2exp_p is deprecated, use bigint.isDivisibleBy2Pow instead
  */
  @deprecated
  ("bigint.divisible_2exp_p is deprecated, use bigint.isDivisibleBy2Pow instead")
  proc bigint.divisible_2exp_p(b: integral) : int {
    return this.isDivisibleBy2Pow(b);
  }

  /*
    Return ``true`` if ``this`` is exactly divisible by ``2^exp``.  ``this`` is
    divisible by ``2^exp`` if there exists an integer ``q`` satisfying ``this =
    q*2^exp``.

    :arg exp: power of 2 to check if ``this`` is divisible by
    :type exp: ``integral``

    :return: ``true`` if ``this`` is exactly divisible by ``2^exp``, ``false``
             otherwise
    :rtype: ``bool``
   */
  proc bigint.isDivisibleBy2Pow(exp: integral) : bool {
    const t_ = this.localize();
    const exp_ = exp.safeCast(mp_bitcnt_t);
    var   ret: c_int;

    ret = mpz_divisible_2exp_p(t_.mpz,   exp_);

    if ret then
      return true;
    else
      return false;
  }

  // congruent_p
  /*
    .. warning::

       bigint.congruent_p is deprecated, use bigint.isCongruent instead
  */
  @deprecated
  ("bigint.congruent_p is deprecated, use bigint.isCongruent instead")
  proc bigint.congruent_p(const ref c: bigint, const ref d: bigint) : int {
    return this.isCongruent(c,d);
  }
  /*
    .. warning::

       bigint.congruent_p is deprecated, use bigint.isCongruent instead
  */
  @deprecated
  ("bigint.congruent_p is deprecated, use bigint.isCongruent instead")
  proc bigint.congruent_p(c: integral, d: integral) : int {
    return this.isCongruent(c,d);
  }

  // congruent_p
  // documented in integral, integral version
  proc bigint.isCongruent(const ref con: bigint, const ref mod: bigint) : bool {
    const t_ = this.localize();
    const con_ = con.localize();
    const mod_ = mod.localize();
    var ret: c_int;

    ret = mpz_congruent_p(t_.mpz, con_.mpz, mod_.mpz);

    if ret then
      return true;
    else
      return false;
  }

  /*
    Return ``true`` if ``this`` is congruent to ``con % mod``.  ``this`` is
    congruent to ``con % mod`` if there exists an integer ``q`` satisfying
    ``this = con + q*mod``.  Unlike the other division functions, ``0`` is an
    acceptable value for ``mod``.  As a result ``this`` and ``con`` are
    considered congruent modulo ``0`` only when exactly equal.

    :arg con: number to determine if ``this`` is congruent to, modulo ``mod``
    :type con: :record:`bigint` or ``integral``

    :arg mod: divisor of ``con`` when determining if ``con`` is congruent to
              ``this``
    :type mod: :record:`bigint` or ``integral``

    :return: ``true`` if ``this`` is congruent to ``con`` modulo ``mod``,
             ``false`` otherwise
    :rtype: ``bool``
   */
  proc bigint.isCongruent(con: integral, mod: integral) : bool {
    const t_ = this.localize();
    const con_ = con.safeCast(c_ulong);
    const mod_ = mod.safeCast(c_ulong);
    var   ret: c_int;

    ret = mpz_congruent_ui_p(t_.mpz, con_, mod_);

    if ret then
      return true;
    else
      return false;
  }

  /*
    .. warning::

       bigint.congruent_2exp_p is deprecated, use bigint.isCongruentBy2Pow instead
  */
  @deprecated
  ("bigint.congruent_2exp_p is deprecated, use bigint.isCongruentBy2Pow instead")
  proc bigint.congruent_2exp_p(const ref c: bigint, b: integral) : int {
    return this.isCongruentBy2Pow(c,b);
  }

  /*
    Return ``true`` if ``this`` is congruent to ``con % 2^modExp``.  ``this`` is
    congruent to ``con % 2^modExp`` if there exists an integer ``q`` satisfying
    ``this = con + q*2^modExp``.

    :arg con: number to determine if ``this`` is congruent to, modulo
              ``2^modExp``.
    :type con: :record:`bigint` or ``integral``

    :arg modExp: power of 2 to use as the divisor of ``con`` when determining if
                 ``con`` is congruent to ``this``.
    :type modExp: ``integral``

    :return: ``true`` if ``this`` is congruent to ``con`` modulo ``2^modExp``,
             ``false`` otherwise.
    :rtype: ``bool``
   */
  proc bigint.isCongruentBy2Pow(const ref con: bigint, modExp: integral) : bool {
    const t_ = this.localize();
    const con_ = con.localize();
    const modExp_ = modExp.safeCast(mp_bitcnt_t);
    var   ret: c_int;

    ret = mpz_congruent_2exp_p(t_.mpz, con_.mpz, modExp_);

    if ret then
      return true;
    else
      return false;
  }

  /*
    .. warning::

       bigint.powm is deprecated, use bigint.powMod instead
  */
  @deprecated
  ("bigint.powm is deprecated, use bigint.powMod instead")
  proc bigint.powm(const ref base: bigint,
                   const ref exp:  bigint,
                   const ref mod:  bigint) {
    BigInteger.powMod(this, base, exp, mod);
  }

  /*
    .. warning::

       bigint.powm is deprecated, use bigint.powMod instead
  */
  @deprecated
  ("bigint.powm is deprecated, use bigint.powMod instead")
  proc bigint.powm(const ref base: bigint,
                             exp:  int,
                   const ref mod:  bigint) {
    BigInteger.powMod(this, base, exp, mod);
  }

  /*
    .. warning::

       bigint.powm is deprecated, use bigint.powMod instead
  */
  @deprecated
  ("bigint.powm is deprecated, use bigint.powMod instead")
  proc bigint.powm(const ref base: bigint,
                             exp:  uint,
                   const ref mod:  bigint) {
    BigInteger.powMod(this, base, exp, mod);
  }

  // Exponentiation Functions
  // Note: Documentation on `exp: uint` version
  proc powMod(ref result: bigint,
              const ref base: bigint,
              const ref exp: bigint,
              const ref mod: bigint)  {
    if _local {
      mpz_powm(result.mpz, base.mpz, exp.mpz, mod.mpz);
    } else if result.localeId == chpl_nodeID {
      const base_ = base;
      const exp_ = exp;
      const mod_ = mod;
      mpz_powm(result.mpz, base_.mpz, exp_.mpz, mod_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const base_ = base;
        const exp_ = exp;
        const mod_  = mod;
        mpz_powm(result.mpz, base_.mpz, exp_.mpz, mod_.mpz);
      }
    }
  }

  // Exponentiation Functions
  // Note: Documentation on `exp: uint` version
  @deprecated(notes="bigint.powMod method is deprecated - please use the standalone function :proc:`~BigInteger.powMod`")
  proc bigint.powMod(const ref base: bigint, const ref exp:  bigint,
                     const ref mod:  bigint) {
    BigInteger.powMod(this, base, exp, mod);
  }

  proc powMod(ref result: bigint,
              const ref base: bigint,
              exp: int,
              const ref mod: bigint)  {
    if exp >= 0 {
      BigInteger.powMod(result, base, exp:uint, mod);
    }
    else {
      BigInteger.powMod(result, base, new bigint(exp), mod);
    }
  }

  // Note: Documentation on `exp: uint` version
  @deprecated(notes="bigint.powMod method is deprecated - please use the standalone function :proc:`~BigInteger.powMod`")
  proc bigint.powMod(const ref base: bigint, exp: int, const ref mod: bigint) {
    BigInteger.powMod(this, base, exp, mod);
  }

  /* Set ``result`` to the result of (``base`` raised to ``exp``) modulo ``mod``.

     :arg result: Where the result is stored
     :type result: :record:`bigint`

     :arg base: The value to be raised to the power of ``exp`` before performing
                a modulo operation on.
     :type base: :record:`bigint`

     :arg exp: The exponent to raise ``base`` to the power of prior to the
               modulo operation.  Can be negative if the inverse (1/``base``)
               modulo ``mod`` exists.
     :type exp: :record:`bigint`, ``int``, or ``uint``

     :arg mod: The divisor for the modulo operation.
     :type mod: :record:`bigint`

     .. warning::
        The program behavior is undefined if ``exp`` is negative and the inverse
        (1/``base``) modulo ``mod`` does not exist.
   */
  proc powMod(ref result: bigint,
              const ref base: bigint,
              exp: uint,
              const ref mod: bigint)  {
    const exp_ = exp.safeCast(c_ulong);

    if _local {
      mpz_powm_ui(result.mpz, base.mpz, exp_, mod.mpz);
    } else if result.localeId == chpl_nodeID {
      const base_ = base;
      const mod_ = mod;
      mpz_powm_ui(result.mpz, base_.mpz, exp, mod_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const base_ = base;
        const mod_  = mod;
        mpz_powm_ui(result.mpz, base_.mpz, exp_, mod_.mpz);
      }
    }
  }

  /* Set ``this`` to the result of (``base`` raised to ``exp``) modulo ``mod``.

     :arg base: The value to be raised to the power of ``exp`` before performing
                a modulo operation on.
     :type base: :record:`bigint`

     :arg exp: The exponent to raise ``base`` to the power of prior to the
               modulo operation.  Can be negative if the inverse (1/``base``)
               modulo ``mod`` exists.
     :type exp: :record:`bigint`, ``int``, or ``uint``

     :arg mod: The divisor for the modulo operation.
     :type mod: :record:`bigint`

     .. warning::
        The program behavior is undefined if ``exp`` is negative and the inverse
        (1/``base``) modulo ``mod`` does not exist.
   */
  @deprecated(notes="bigint.powMod method is deprecated - please use the standalone function :proc:`~BigInteger.powMod`")
  proc bigint.powMod(const ref base: bigint, exp: uint, const ref mod: bigint) {
    BigInteger.powMod(this, base, exp, mod);
  }

  // This helper is intended for use only when the exponent argument
  // is negative.  Negative exponents result in integers that are between -1
  // and 1 (but usually 0 unless the base is -1 or 1)
  @chpldoc.nodoc
  proc powNegativeExpHelper(ref result: bigint, const ref base: bigint, exp: int) {
    if (base == 1 || (base == -1 && AutoMath.abs(exp) % 2 == 1)) {
      result = base;
    } else if (base == -1 && AutoMath.abs(exp) % 2 == 0) {
      result = 1;
    } else {
      result = 0;
    }
  }
  @chpldoc.nodoc
  proc powNegativeExpHelper(ref result: bigint, const ref base: bigintWrapper, exp: int) {
    const base1 = mpz_cmp_ui(base.mpz, 1) == 0;
    const baseNeg1 = mpz_cmp_si(base.mpz, -1) == 0;
    if (base1 || (baseNeg1 && AutoMath.abs(exp) % 2 == 1)) {
      mpz_set(result.mpz, base.mpz);
    } else if (baseNeg1 && AutoMath.abs(exp) % 2 == 0) {
      result = 1;
    } else {
      result = 0;
    }
  }

  // Documented in uint, uint version
  proc pow(ref result: bigint, const ref base: bigint, exp: int) {
    if exp >= 0 {
      BigInteger.pow(result, base, exp : uint);
    } else {
      if _local {
        powNegativeExpHelper(result, base, exp);
      } else if result.localeId == chpl_nodeID {
        const base_ = base;
        powNegativeExpHelper(result, base_, exp);
      } else {
        const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
        on __primitive("chpl_on_locale_num", resultLoc) {
          const base_ = base;
          powNegativeExpHelper(result, base_, exp);
        }
      }
    }
  }

  // Documented in uint, uint version
  @deprecated(notes="bigint.pow method is deprecated - please use the standalone function :proc:`~BigInteger.pow`")
  proc bigint.pow(const ref base: bigint, exp: int) {
    BigInteger.pow(this, base, exp);
  }

  // Documented in uint, uint version
  proc pow(ref result: bigint, const ref base: bigint, exp: uint) {
    const exp_ = exp.safeCast(c_ulong);
    if _local {
      mpz_pow_ui(result.mpz, base.mpz, exp_);
    } else if result.localeId == chpl_nodeID {
      const base_ = base;
      mpz_pow_ui(result.mpz, base_.mpz, exp_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const base_ = base;
        mpz_pow_ui(result.mpz, base_.mpz, exp_);
      }
    }
  }

  // Documented in uint, uint version
  @deprecated(notes="bigint.pow method is deprecated - please use the standalone function :proc:`~BigInteger.pow`")
  proc bigint.pow(const ref base: bigint, exp: uint) {
    BigInteger.pow(this, base, exp);
  }

  // Documented in uint, uint version
  proc pow(ref result: bigint, base: int, exp: int) {
    if base >= 0 && exp >= 0 {
      BigInteger.pow(result, base : uint, exp : uint);
    } else if base <  0 && exp >= 0 {
      BigInteger.pow(result, (0 - base) : uint, exp : uint);

      if (exp % 2) == 1 {
        BigInteger.neg(result, result);
      }
    } else {
      BigInteger.pow(result, new bigint(base), exp);
    }
  }

  // Documented in uint, uint version
  @deprecated(notes="bigint.pow method is deprecated - please use the standalone function :proc:`~BigInteger.pow`")
  proc bigint.pow(base: int, exp: int) {
    BigInteger.pow(this, base, exp);
  }

  /* Set ``result`` to the result of ``base`` raised to ``exp``.

     :arg result: Where the result is stored
     :type result: :record:`bigint`

     :arg base: The value to be raised to the power of ``exp``.
     :type base: :record:`bigint`, ``int`` or ``uint``

     :arg exp: The exponent to raise ``base`` to the power of.
     :type exp: ``int`` or ``uint``
   */
  proc pow(ref result: bigint, base: uint, exp: uint) {
    const base_ = base.safeCast(c_ulong);
    const exp_  = exp.safeCast(c_ulong);

    if _local {
      mpz_ui_pow_ui(result.mpz, base_, exp_);
    } else if result.localeId == chpl_nodeID {
      mpz_ui_pow_ui(result.mpz, base_, exp_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        mpz_ui_pow_ui(result.mpz, base_, exp_);
      }
    }
  }

  /* Set ``this`` to the result of ``base`` raised to ``exp``.

     :arg base: The value to be raised to the power of ``exp``.
     :type base: :record:`bigint`, ``int`` or ``uint``

     :arg exp: The exponent to raise ``base`` to the power of.
     :type exp: ``int`` or ``uint``
   */
  @deprecated(notes="bigint.pow method is deprecated - please use the standalone function :proc:`~BigInteger.pow`")
  proc bigint.pow(base: uint, exp: uint) {
    BigInteger.pow(this, base, exp);
  }

  // Root Extraction Functions
  proc root(ref result: bigint, const ref a: bigint, n: uint) : int {
    const n_  = n.safeCast(c_ulong);
    var   ret: c_int;

    if _local {
      ret = mpz_root(result.mpz, a.mpz, n_);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      ret = mpz_root(result.mpz, a_.mpz, n_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        ret = mpz_root(result.mpz, a_.mpz, n_);
      }
    }
    return ret.safeCast(int);
  }

  @deprecated(notes="bigint.root method is deprecated - please use the standalone function :proc:`~BigInteger.root`")
  proc bigint.root(const ref a: bigint, n: uint) : int {
    return BigInteger.root(this, a, n);
  }

  // root gets root, rem gets remainder.
  proc rootrem(ref root: bigint, ref rem: bigint, const ref u: bigint, n: uint) {
    const n_  = n.safeCast(c_ulong);

    if _local {
      mpz_rootrem(root.mpz, rem.mpz, u.mpz, n_);
    } else if root.localeId == chpl_nodeID {
      var rem_: bigint;
      const u_ = u;
      mpz_rootrem(root.mpz, rem_.mpz, u_.mpz, n_);
      rem = rem_;
    } else {
      const rootLoc = chpl_buildLocaleID(root.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", rootLoc) {
        var rem_: bigint;
        const u_ = u;
        mpz_rootrem(root.mpz, rem_.mpz, u_.mpz, n_);
        rem = rem_;
      }
    }
  }

  @deprecated(notes="bigint.rootrem method is deprecated - please use the standalone function :proc:`~BigInteger.rootrem`")
  proc bigint.rootrem(ref rem: bigint, const ref u: bigint, n: uint) {
    BigInteger.rootrem(this, rem, u, n);
  }

  proc sqrt(ref result: bigint, const ref a: bigint) {
    if _local {
      mpz_sqrt(result.mpz, a.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      mpz_sqrt(result.mpz, a_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        mpz_sqrt(result.mpz, a_.mpz);
      }
    }
  }

  @deprecated(notes="bigint.sqrt method is deprecated - please use the standalone function :proc:`~BigInteger.sqrt`")
  proc bigint.sqrt(const ref a: bigint) {
    BigInteger.sqrt(this, a);
  }

  // this gets root, rem gets remainder of a-root*root.
  proc sqrtrem(ref root: bigint, ref rem: bigint, const ref a: bigint) {
    if _local {
      mpz_sqrtrem(root.mpz, rem.mpz, a.mpz);
    } else if root.localeId == chpl_nodeID {
      var rem_ : bigint;
      const a_ = a;
      mpz_sqrtrem(root.mpz, rem_.mpz, a_.mpz);
      rem = rem_;
    } else {
      const rootLoc = chpl_buildLocaleID(root.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", rootLoc) {
        var rem_ : bigint;
        const a_ = a;
        mpz_sqrtrem(root.mpz, rem_.mpz, a_.mpz);
        rem = rem_;
      }
    }
  }

  @deprecated(notes="bigint.sqrtrem method is deprecated - please use the standalone function :proc:`~BigInteger.sqrtrem`")
  proc bigint.sqrtrem(ref rem: bigint, const ref a: bigint) {
    BigInteger.sqrtrem(this, rem, a);
  }

  /*
    .. warning::

       bigint.perfect_power_p is deprecated, use bigint.isPerfectPower instead
  */
  @deprecated
  ("bigint.perfect_power_p is deprecated, use bigint.isPerfectPower instead")
  proc bigint.perfect_power_p() : int {
    return this.isPerfectPower();
  }

  /*
    Return ``true`` if ``this`` is a perfect power, i.e., if there exist
    integers ``a`` and ``b`` with ``b > 1``, such that ``this = a^b``.

    Under this definition both 0 and 1 are considered to be perfect powers.
    Negative values can only be odd perfect powers.

    :return: ``true`` if ``this`` is a perfect power, ``false`` otherwise.
   */
  proc bigint.isPerfectPower () : bool {
    var t_ = this.localize();
    var ret: c_int;

    ret = mpz_perfect_power_p(t_.mpz);

    if ret then
      return true;
    else
      return false;
  }

  /*
    .. warning::

       bigint.perfect_square_p is deprecated, use bigint.isPerfectSquare instead
  */
  @deprecated
  ("bigint.perfect_square_p is deprecated, use bigint.isPerfectSquare instead")
  proc bigint.perfect_square_p() : int {
    return this.isPerfectSquare();
  }

  /*
    Return ``true`` if ``this`` is a perfect square, i.e., if the square root of
    ``this`` is an integer.  Under this definition both ``0`` and ``1`` are
    considered to be perfect squares.

    :return: ``true`` if ``this`` is a perfect square, ``false`` otherwise.
    :rtype: ``bool``
   */
  proc bigint.isPerfectSquare() : bool {
    var t_ = this.localize();
    var ret: c_int;

    ret = mpz_perfect_square_p(t_.mpz);

    if ret then
      return true;
    else
      return false;
  }



  // Number Theoretic Functions

  /*
    .. warning::

       bigint.probab_prime_p is deprecated, use bigint.probablyPrime instead
  */
  @deprecated
  ("bigint.probab_prime_p is deprecated, use bigint.probablyPrime instead")
  proc bigint.probab_prime_p(reps: int) : int {
    var ret = this.probablyPrime(reps):int;
    return ret;
  }

  /* An enumeration of the different possibilities of a number being prime, for use with e.g.
     :proc:`~bigint.probablyPrime` to determine if a number is prime or not.

     - ``primality.notPrime`` indicates that the number is not a prime.
     - ``primality.maybePrime`` indicates that the number may or may not be a prime.
     - ``primality.isPrime`` indicates that the number is a prime.
   */
  enum primality {
    notPrime=0,
    maybePrime,
    isPrime
  };

  /*
    Determine whether ``this`` is prime.  Returns one of the :enum:`primality`
    constants - ``primality.isPrime``, ``primality.maybePrime``, or
    ``primality.notPrime``.

    Performs some trial divisions, a Baillie-PSW probable prime test, and
    reps-24 Miller-Rabin probabilistic primality tests.  A higher ``reps`` value
    will reduce the chances of a non-prime being identified as "probably prime".
    A composite number will be identified as a prime with an asymptotic
    probability of less than ``4^(-reps)``.  Reasonable values of ``reps`` are
    between 15 and 50.

    :arg reps: number of attempts before returning ``primality.maybePrime`` if
               a definitive answer can't be found before then.
    :type reps: ``int``

    :returns: ``primality.isPrime``, ``primality.maybePrime`` or
              ``primality.notPrime``.
    :rtype: :enum:`primality`
   */
  proc bigint.probablyPrime(reps: int) : primality {
    var t_ = this.localize();
    var reps_ = reps.safeCast(c_int);
    var ret: c_int;

    ret = mpz_probab_prime_p(t_.mpz, reps_);

    use primality;
    if ret==0 then
      return notPrime;
    else if ret==1 then
      return maybePrime;
    else
      return isPrime;
  }

  proc nextprime(ref result: bigint, const ref a: bigint) {
    if _local {
      mpz_nextprime(result.mpz, a.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      mpz_nextprime(result.mpz, a_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        mpz_nextprime(result.mpz, a_.mpz);
      }
    }
  }

  @deprecated(notes="bigint.nextprime method is deprecated - please use the standalone function :proc:`~BigInteger.nextprime`")
  proc bigint.nextprime(const ref a: bigint) {
    BigInteger.nextprime(this, a);
  }

  proc gcd(ref result: bigint, const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_gcd(result.mpz, a.mpz, b.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      const b_ = b;
      mpz_gcd(result.mpz, a_.mpz, b_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        const b_ = b;
        mpz_gcd(result.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  @deprecated(notes="bigint.gcd method is deprecated - please use the standalone function :proc:`~BigInteger.gcd`")
  proc bigint.gcd(const ref a: bigint, const ref b: bigint) {
    BigInteger.gcd(this, a, b);
  }

  proc gcd(ref result: bigint, const ref a: bigint, b: int) {
    if b >= 0 {
      BigInteger.gcd(result, a, b : uint);

    } else {
      BigInteger.gcd(result, a, new bigint(b));
    }
  }

  @deprecated(notes="bigint.gcd method is deprecated - please use the standalone function :proc:`~BigInteger.gcd`")
  proc bigint.gcd(const ref a: bigint, b: int) {
    BigInteger.gcd(this, a, b);
  }

  proc gcd(ref result: bigint, const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);
    if _local {
      mpz_gcd_ui(result.mpz, a.mpz, b_);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      mpz_gcd_ui(result.mpz, a_.mpz, b_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        mpz_gcd_ui(result.mpz, a_.mpz, b_);
      }
    }
  }

  @deprecated(notes="bigint.gcd method is deprecated - please use the standalone function :proc:`~BigInteger.gcd`")
  proc bigint.gcd(const ref a: bigint, b: uint) {
    BigInteger.gcd(this, a, b);
  }

  /* Set ``result`` to the greatest common divisor of ``a`` and ``b``, and
     set ``s`` and ``t`` to coefficients such that ``a*s + b*t == result``.

     .. note::
        The result stored in ``result`` is always positive, even if one or
        both of ``a`` and ``b`` are negative (or zero if both are zero).

     This fulfills the same role as the GMP function ``mpz_gcdext``.

     :arg result: Where the result is stored
     :type result: :record:`bigint`

     :arg a: One of the numbers to compute the greatest common divisor of
     :type a: :record:`bigint`

     :arg b: One of the numbers to compute the greatest common divisor of
     :type b: :record:`bigint`

     :arg s: The returned coefficient that can be multiplied by ``a``.
     :type s: :record:`bigint`

     :arg t: The returned coefficient that can be multiplied by ``b``.
     :type t: :record:`bigint`
   */
  proc gcd(ref result: bigint, const ref a: bigint, const ref b: bigint,
                  ref s: bigint, ref t: bigint): void {
    if _local {
      mpz_gcdext(result.mpz, s.mpz, t.mpz, a.mpz, b.mpz);
    } else if result.localeId == chpl_nodeID {
      // TODO: need to revisit this in relation to Cray/chapel-private#4628
      var s_ : bigint;
      var t_ : bigint;
      const a_ = a;
      const b_ = b;
      mpz_gcdext(result.mpz, s_.mpz, t_.mpz, a_.mpz, b_.mpz);
      s = s_;
      t = t_;
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        var s_ : bigint;
        var t_ : bigint;
        const a_ = a;
        const b_ = b;
        mpz_gcdext(result.mpz, s_.mpz, t_.mpz, a_.mpz, b_.mpz);
        s = s_;
        t = t_;
      }
    }
  }

  /* Set ``this`` to the greatest common divisor of ``a`` and ``b``, and
     set ``s`` and ``t`` to coefficients such that ``a*s + b*t == this``.

     .. note::
        The result stored in ``this`` is always positive, even if one or
        both of ``a`` and ``b`` are negative (or zero if both are zero).

     This fulfills the same role as the GMP function ``mpz_gcdext``.

     :arg a: One of the numbers to compute the greatest common divisor of
     :type a: :record:`bigint`

     :arg b: One of the numbers to compute the greatest common divisor of
     :type b: :record:`bigint`

     :arg s: The returned coefficient that can be multiplied by ``a``.
     :type s: :record:`bigint`

     :arg t: The returned coefficient that can be multiplied by ``b``.
     :type t: :record:`bigint`
   */
  @deprecated(notes="bigint.gcd method is deprecated - please use the standalone function :proc:`~BigInteger.gcd`")
  proc bigint.gcd(const ref a: bigint, const ref b: bigint,
                  ref s: bigint, ref t: bigint): void {
    BigInteger.gcd(this, a, b, s, t);
  }

  // sets this to gcd(a,b)
  // set s and t to to coefficients satisfying a*s + b*t == g
  @deprecated(notes="gcdext is deprecated, please use the new overload of :proc:`bigint.gcd` with s and t arguments instead")
  proc bigint.gcdext(ref s: bigint,
                     ref t: bigint,
                     const ref a: bigint,
                     const ref b: bigint) {
    BigInteger.gcd(this, a, b, s, t);
  }

  proc lcm(ref result: bigint, const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_lcm(result.mpz, a.mpz, b.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      const b_ = b;
      mpz_lcm(result.mpz, a_.mpz, b_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        const b_ = b;
        mpz_lcm(result.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  @deprecated(notes="bigint.lcm method is deprecated - please use the standalone function :proc:`~BigInteger.lcm`")
  proc bigint.lcm(const ref a: bigint, const ref b: bigint) {
    BigInteger.lcm(this, a, b);
  }

  proc lcm(ref result: bigint, const ref a: bigint, b: int) {
    if b >= 0 then
      BigInteger.lcm(result, a, b:uint);
    else
      BigInteger.lcm(result, a, (0 - b):uint);
  }

  @deprecated(notes="bigint.lcm method is deprecated - please use the standalone function :proc:`~BigInteger.lcm`")
  proc bigint.lcm(const ref a: bigint, b: int) {
    BigInteger.lcm(this, a, b);
  }

  proc lcm(ref result: bigint, const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_lcm_ui(result.mpz, a.mpz, b_);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      mpz_lcm_ui(result.mpz, a_.mpz, b_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        mpz_lcm_ui(result.mpz, a_.mpz, b_);
      }
    }
  }

  @deprecated(notes="bigint.lcm method is deprecated - please use the standalone function :proc:`~BigInteger.lcm`")
  proc bigint.lcm(const ref a: bigint, b: uint) {
    BigInteger.lcm(this, a, b);
  }

  /*
   An `InversionError` is thrown if a :proc:`bigint.invert()` is attempted with
   invalid arguments that result in a non-existent inverse. Specifically,
   if the arguments cause a divide by zero, this error notifies the caller
   that the internal value of the :record:`bigint` was left in an undefined state.
   */
  class InversionError : Error {

    /* Create an :class:`InversionError` with the default error message: `Inverse does not exist` */
    proc init() {
      super.init("Inverse does not exist");
    }
  }

  /* Set the value of ``result`` to the inverse of ``a`` modulo ``b``

     .. note::
        If an inverse does not exist, an :class:`InversionError` will be thrown,
        and the value of ``result`` will be left undefined

     This fulfills the same role as the GMP number theoretic function ``mpz_invert``.

     :arg result: Where the result is stored
     :type result: :record:`bigint`

     :arg a: The dividend of the modulo operation
     :type a: :record:`bigint`

     :arg b: The divisor of the modulo operation
     :type b: :record:`bigint`

  */
  proc invert(ref result: bigint, const ref a: bigint, const ref b: bigint) throws {
    var ret: c_int;
    if _local {
      ret = mpz_invert(result.mpz, a.mpz, b.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      const b_ = b;
      ret = mpz_invert(result.mpz, a_.mpz, b_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        const b_ = b;
        ret = mpz_invert(result.mpz, a_.mpz, b_.mpz);
      }
    }

    if (ret.safeCast(int) == 0) {
      throw new owned InversionError();
    }
  }

  /* Set the value of ``this`` to the inverse of ``a`` modulo ``b``

     .. note::
        If an inverse does not exist, an :class:`InversionError` will be thrown,
        and the value of ``this`` will be left undefined

     This fulfills the same role as the GMP number theoretic function ``mpz_invert``.

     :arg a: The dividend of the modulo operation
     :type a: :record:`bigint`

     :arg b: The divisor of the modulo operation
     :type b: :record:`bigint`

  */
  @deprecated(notes="bigint.invert method is deprecated - please use the standalone function :proc:`~BigInteger.invert`")
  proc bigint.invert(const ref a: bigint, const ref b: bigint) throws {
    BigInteger.invert(this, a, b);
  }

  // remove
    /*
    .. warning::

       bigint.remove is deprecated, use bigint.removeFactor instead
  */
  @deprecated
  ("bigint.remove is deprecated, use bigint.removeFactor instead")
  proc bigint.remove(const ref a: bigint, const ref f: bigint) : uint {
    return BigInteger.removeFactor(this, a,f);
  }

  // This helper is intended for use only when the factor is 0
  // Division by 0 is undefined and it results in a
  // Floating point exception error.
  @chpldoc.nodoc
  proc removeFactorZeroHelper(ref result: bigint, const ref x: bigint, const ref fac: bigint) : uint {
    result = 0;
    return 0;
  }

  /*
    Remove all occurrences of the factor ``fac`` from ``x`` and store the result
    in ``result``.  Return the number of occurrences removed.

    :arg result: Where the result is stored
    :type result: :record:`bigint`

    :arg x: The value to remove all occurrences of ``fac`` from
    :type x: :record:`bigint`

    :arg fac: The factor to remove from ``x``.
    :type fac: :record:`bigint`

    :return: The number of occurrences of ``fac`` found in ``x``.
    :rtype: ``uint``
   */
  proc removeFactor(ref result: bigint, const ref x: bigint, const ref fac: bigint) : uint {
    var ret: c_ulong;
    if(fac!=0){
      if _local {
        ret = mpz_remove(result.mpz, x.mpz, fac.mpz);
      } else if result.localeId == chpl_nodeID {
          const x_ = x;
          const fac_ = fac;
          ret = mpz_remove(result.mpz, x_.mpz, fac_.mpz);
      } else {
        const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
        on __primitive("chpl_on_locale_num", resultLoc) {
          const x_ = x;
          const fac_ = fac;
          ret = mpz_remove(result.mpz, x_.mpz, fac_.mpz);
        }
      }
      return ret.safeCast(uint);
    }else{
      return removeFactorZeroHelper(result,x,fac);
    }
  }


  /*
    Remove all occurrences of the factor ``fac`` from ``x`` and store the result
    in ``this``.  Return the number of occurrences removed.

    :arg x: The value to remove all occurrences of ``fac`` from
    :type x: :record:`bigint`

    :arg fac: The factor to remove from ``x``.
    :type fac: :record:`bigint`

    :return: The number of occurrences of ``fac`` found in ``x``.
    :rtype: ``uint``
   */
  @deprecated(notes="bigint.removeFactor method is deprecated - please use the standalone function :proc:`~BigInteger.removeFactor`")
  proc bigint.removeFactor(const ref x: bigint, const ref fac: bigint) : uint {
    return BigInteger.removeFactor(this, x, fac);
  }


  // Factorial
  proc fac(ref result: bigint, a: integral) {
    const a_ = a.safeCast(c_ulong);
    if _local {
      mpz_fac_ui(result.mpz, a_);
    } else if result.localeId == chpl_nodeID {
      mpz_fac_ui(result.mpz, a_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        mpz_fac_ui(result.mpz, a_);
      }
    }
  }

  @deprecated(notes="bigint.fac method is deprecated - please use the standalone function :proc:`~BigInteger.fac`")
  proc bigint.fac(a: integral) {
    BigInteger.fac(this, a);
  }



  // Binomial
  proc bin(ref result: bigint, const ref n: bigint, k: integral) {
    const k_ = k.safeCast(c_ulong);
    if _local {
      mpz_bin_ui(result.mpz, n.mpz, k_);
    } else if result.localeId == chpl_nodeID {
      const n_ = n;
      mpz_bin_ui(result.mpz, n_.mpz, k_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const n_ = n;
        mpz_bin_ui(result.mpz, n_.mpz, k_);
      }
    }
  }

  @deprecated(notes="bigint.bin method is deprecated - please use the standalone function :proc:`~BigInteger.bin`")
  proc bigint.bin(const ref n: bigint, k: integral) {
    BigInteger.bin(this, n, k);
  }

  proc bin(ref result: bigint, n: uint, k: integral) {
    if n >= 0 {
      const n_ = n.safeCast(c_ulong);
      const k_ = k.safeCast(c_ulong);

      if _local {
        mpz_bin_uiui(result.mpz, n_, k_);
      } else if result.localeId == chpl_nodeID {
        mpz_bin_uiui(result.mpz, n_, k_);
      } else {
        const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
        on __primitive("chpl_on_locale_num", resultLoc) {
          mpz_bin_uiui(result.mpz, n_, k_);
        }
      }
    } else {
      BigInteger.bin(result, new bigint(n), k);
    }
  }

  @deprecated(notes="bigint.bin method is deprecated - please use the standalone function :proc:`~BigInteger.bin`")
  proc bigint.bin(n: uint, k: integral) {
    BigInteger.bin(this, n, k);
  }



  // Fibonacci
  proc fib(ref result: bigint, n: integral) {
    const n_ = n.safeCast(c_ulong);

    if _local {
      mpz_fib_ui(result.mpz, n_);
    } else if result.localeId == chpl_nodeID {
      mpz_fib_ui(result.mpz, n_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        mpz_fib_ui(result.mpz, n_);
      }
    }
  }

  @deprecated(notes="bigint.fib method is deprecated - please use the standalone function :proc:`~BigInteger.fib`")
  proc bigint.fib(n: integral) {
    BigInteger.fib(this, n);
  }

  proc fib2(ref result: bigint, ref fnsub1: bigint, n: integral) {
    const n_ = n.safeCast(c_ulong);

    if _local {
      mpz_fib2_ui(result.mpz, fnsub1.mpz, n_);
    } else if result.localeId == chpl_nodeID {
        // TODO: need to revisit this in relation to Cray/chapel-private#4628
        var fnsub1_ : bigint;
        mpz_fib2_ui(result.mpz, fnsub1_.mpz, n_);
        fnsub1 = fnsub1_;
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        var fnsub1_ : bigint;
        mpz_fib2_ui(result.mpz, fnsub1_.mpz, n_);
        fnsub1 = fnsub1_;
      }
    }
  }

  @deprecated(notes="bigint.fib2 method is deprecated - please use the standalone function :proc:`~BigInteger.fib2`")
  proc bigint.fib2(ref fnsub1: bigint, n: integral) {
    BigInteger.fib2(this, fnsub1, n);
  }


  proc lucnum(ref result: bigint, n: integral) {
    const n_ = n.safeCast(c_ulong);
    if _local {
      mpz_lucnum_ui(result.mpz, n_);
    } else if result.localeId == chpl_nodeID {
      mpz_lucnum_ui(result.mpz, n_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        mpz_lucnum_ui(result.mpz, n_);
      }
    }
  }

  // Lucas Number
  @deprecated(notes="bigint.lucnum method is deprecated - please use the standalone function :proc:`~BigInteger.lucnum`")
  proc bigint.lucnum(n: integral) {
    BigInteger.lucnum(this, n);
  }

  proc lucnum2(ref result: bigint, ref fnsub1: bigint, n: integral) {
    const n_ = n.safeCast(c_ulong);

    if _local {
      mpz_lucnum2_ui(result.mpz, fnsub1.mpz, n_);
    } else if result.localeId == chpl_nodeID {
      // TODO: need to revisit this in relation to Cray/chapel-private#4628
      var fnsub1_ : bigint;
      mpz_lucnum2_ui(result.mpz, fnsub1_.mpz, n_);
      fnsub1 = fnsub1_;
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        var fnsub1_ : bigint;
        mpz_lucnum2_ui(result.mpz, fnsub1_.mpz, n_);
        fnsub1 = fnsub1_;
      }
    }
  }

  @deprecated(notes="bigint.lucnum2 method is deprecated - please use the standalone function :proc:`~BigInteger.lucnum2`")
  proc bigint.lucnum2(ref fnsub1: bigint, n: integral) {
    BigInteger.lucnum2(this, fnsub1, n);
  }



  // Bit operations
  proc bigint.popcount() : uint {
    const t_ = this.localize();
    var ret: c_ulong;

    ret = mpz_popcount(t_.mpz);

    return ret.safeCast(uint);
  }

  proc bigint.hamdist(const ref b: bigint) : uint {
    const t_ = this.localize();
    const b_ = b.localize();
    var ret: c_ulong;

    ret = mpz_hamdist(t_.mpz, b_.mpz);

    return ret.safeCast(uint);
  }

  pragma "last resort"
  @deprecated(notes="The 'starting_bit' argument is deprecated, please use 'startBitIdx' instead")
  proc bigint.scan0(starting_bit: integral) : uint {
    return this.scan0(startBitIdx = starting_bit);
  }

  /*  Scan ``this``, starting from ``startBitIdx``, towards more significant
      bits until the first ``0`` bit is found.  Return the index of the found
      bit.

      If the bit at ``startBitIdx`` is ``0``, will return ``startBitIdx``.

      :arg startBitIdx: the index of the first bit to start searching for a
                        ``0``
      :type startBitIdx: ``integral``

      :returns: the index of the first ``0`` bit after ``startBitIdx``,
                inclusive
      :rtype: ``uint``
   */
  proc bigint.scan0(startBitIdx: integral): uint {
    const t_ = this.localize();
    const sb_ = startBitIdx.safeCast(c_ulong);
    var   ret: c_ulong;

    ret = mpz_scan0(t_.mpz, sb_);

    return ret.safeCast(uint);
  }

  pragma "last resort"
  @deprecated(notes="The 'starting_bit' argument is deprecated, please use 'startBitIdx' instead")
  proc bigint.scan1(starting_bit: integral) : uint {
    return this.scan1(startBitIdx = starting_bit);
  }

  /*  Scan ``this``, starting from ``startBitIdx``, towards more significant
      bits until the first ``1`` bit is found.  Return the index of the found
      bit.

      If the bit at ``startBitIdx`` is ``1``, will return ``startBitIdx``.

      :arg startBitIdx: the index of the first bit to start searching for a
                        ``1``
      :type startBitIdx: ``integral``

      :returns: the index of the first ``1`` bit after ``startBitIdx``,
                inclusive
      :rtype: ``uint``
   */
  proc bigint.scan1(startBitIdx: integral): uint {
    const t_ = this.localize();
    const sb_ = startBitIdx.safeCast(c_ulong);
    var   ret: c_ulong;

    ret = mpz_scan1(t_.mpz,   sb_);

    return ret.safeCast(uint);
  }



  // Set/Clr bit
  proc bigint.setbit(bit_index: integral) {
    const bi_ = bit_index.safeCast(c_ulong);

    if _local {
      mpz_setbit(this.mpz, bi_);

    } else if this.localeId == chpl_nodeID {
      mpz_setbit(this.mpz, bi_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_setbit(this.mpz, bi_);
      }
    }
  }

  proc bigint.clrbit(bit_index: integral) {
    const bi_ = bit_index.safeCast(c_ulong);

    if _local {
      mpz_clrbit(this.mpz, bi_);

    } else if this.localeId == chpl_nodeID {
      mpz_clrbit(this.mpz, bi_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_clrbit(this.mpz, bi_);
      }
    }
  }

  proc bigint.combit(bit_index: integral) {
    const bi_ = bit_index.safeCast(c_ulong);

    if _local {
      mpz_combit(this.mpz, bi_);

    } else if this.localeId == chpl_nodeID {
      mpz_combit(this.mpz, bi_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_combit(this.mpz, bi_);
      }
    }
  }

  proc bigint.tstbit(bit_index: integral) : int {
    var t_ = this.localize();
    const bi_ = bit_index.safeCast(c_ulong);
    var  ret: c_int;

    ret = mpz_tstbit(t_.mpz, bi_);

    return ret.safeCast(int);
  }

  private proc fits_into(mpz: mpz_t, type t: integral): bool {
    return
      if t == c_long then
        mpz_fits_slong_p(mpz) != 0
      else if t == c_ulong then
        mpz_fits_ulong_p(mpz) != 0
      else if t == c_int then
        mpz_fits_sint_p(mpz) != 0
      else if t == c_uint then
        mpz_fits_uint_p(mpz) != 0
      else if t == c_short then
        mpz_fits_sshort_p(mpz) != 0
      else if t == c_ushort then
        mpz_fits_ushort_p(mpz) != 0
      else if t == c_schar then
        mpz_cmp_si(mpz, -128) >= 0 && mpz_cmp_si(mpz, 127) <= 0
      else if t == c_uchar then
        mpz_cmp_ui(mpz, 0) >= 0 && mpz_cmp_ui(mpz, 255) <= 0
      else
        false;
  }

  // Miscellaneous Functions
  /* Test whether a :record:`bigint` will fit into
    one of the standard integer types

    :arg t: The Integral type to check against.
    :type t: `integral`
  */
  proc bigint.fitsInto(type t: integral): bool {
    var t_ = this.localize();
    return fits_into(t_.mpz, t);
  }

  @deprecated(notes="`fits_ulong_p` is deprecated -  please use `bigint.fitsInto(c_ulong)` instead")
  proc bigint.fits_ulong_p() : int {
    var t_ = this.localize();
    var ret: c_int;

    ret = mpz_fits_ulong_p(t_.mpz);
    return ret.safeCast(int);
  }

  @deprecated(notes="`fits_slong_p` is deprecated -  please use `bigint.fitsInto(c_long)` instead")
  proc bigint.fits_slong_p() : int {
    var t_ = this.localize();
    var ret: c_int;

    ret = mpz_fits_slong_p(t_.mpz);
    return ret.safeCast(int);
  }

  @deprecated(notes="`fits_uint_p` is deprecated -  please use `bigint.fitsInto(c_uint)` instead")
  proc bigint.fits_uint_p() : int {
    var t_ = this.localize();
    var ret: c_int;

    ret = mpz_fits_uint_p(t_.mpz);

    return ret.safeCast(int);
  }

  @deprecated(notes="`fits_sint_p` is deprecated -  please use `bigint.fitsInto(c_int)` instead")
  proc bigint.fits_sint_p() : int {
    var t_ = this.localize();
    var ret: c_int;

    ret = mpz_fits_sint_p(t_.mpz);
    return ret.safeCast(int);
  }

  @deprecated(notes="`fits_ushort_p` is deprecated -  please use `bigint.fitsInto(c_ushort)` instead")
  proc bigint.fits_ushort_p() : int {
    var t_ = this.localize();
    var ret: c_int;

    ret = mpz_fits_ushort_p(t_.mpz);
    return ret.safeCast(int);
  }

  @deprecated(notes="`fits_sshort_p` is deprecated -  please use `bigint.fitsInto(c_short)` instead")
  proc bigint.fits_sshort_p() : int {
    var t_ = this.localize();
    var ret: c_int;

    ret = mpz_fits_sshort_p(t_.mpz);
    return ret.safeCast(int);
  }

  /*
    .. warning::

       bigint.even_p is deprecated, use bigint.isEven instead
  */
  @deprecated
  ("bigint.even_p is deprecated, use bigint.isEven instead")
  proc bigint.even_p() : int {
    return this.isEven();
  }

  /*
    Returns ``true`` if ``this`` is an even number, ``false`` otherwise.
   */
  proc bigint.isEven() : bool {
    var t_ = this.localize();
    var ret: c_int;

    ret = mpz_even_p(t_.mpz);

    if ret then
      return true;
    else
      return false;
  }

  /*
    .. warning::

       bigint.odd_p is deprecated, use bigint.isOdd instead
  */
  @deprecated
  ("bigint.odd_p is deprecated, use bigint.isOdd instead")
  proc bigint.odd_p() : int {
    return this.isOdd();
  }

  /*
    Returns ``true`` if ``this`` is an odd number, ``false`` otherwise.
   */
  proc bigint.isOdd() : bool {
    var t_ = this.localize();
    var ret: c_int;

    ret = mpz_odd_p(t_.mpz);

    if ret then
      return true;
    else
      return false;
  }



  //
  // 5.5 Arithmetic functions
  //

  proc add(ref result: bigint, const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_add(result.mpz, a.mpz, b.mpz);
    }
    else if result.localeId == chpl_nodeID {
      const a_ = a;
      const b_ = b;
      mpz_add(result.mpz, a_.mpz, b_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        const b_ = b;
        mpz_add(result.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  @deprecated(notes="bigint.add method is deprecated - please use the standalone function :proc:`~BigInteger.add`")
  proc bigint.add(const ref a: bigint, const ref b: bigint) {
    BigInteger.add(this, a, b);
  }

  proc add(ref result: bigint, const ref a: bigint, b: int) {
    if b >= 0 {
      BigInteger.add(result, a, b:uint);
    }
    else {
      const b_ = (0 - b).safeCast(c_ulong);

      if _local {
        mpz_sub_ui(result.mpz, a.mpz, b_);
      }
      else if result.localeId == chpl_nodeID {
        const a_ = a;
        mpz_sub_ui(result.mpz, a_.mpz, b_);
      }
      else {
        const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
        on __primitive("chpl_on_locale_num", resultLoc) {
          const a_ = a;
          mpz_sub_ui(result.mpz, a_.mpz, b_);
        }
      }
    }
  }

  @deprecated(notes="bigint.add method is deprecated - please use the standalone function :proc:`~BigInteger.add`")
  proc bigint.add(const ref a: bigint, b: int) {
    BigInteger.add(this, a, b);
  }

  proc add(ref result: bigint, const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);
    if _local {
      mpz_add_ui(result.mpz, a.mpz, b_);
    }
    else if result.localeId == chpl_nodeID {
      const a_ = a;
      mpz_add_ui(result.mpz, a_.mpz, b_);
    }
    else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        mpz_add_ui(result.mpz, a_.mpz, b_);
      }
    }
  }

  @deprecated(notes="bigint.add method is deprecated - please use the standalone function :proc:`~BigInteger.add`")
  proc bigint.add(const ref a: bigint, b: uint) {
    BigInteger.add(this, a, b);
  }

  proc sub(ref result: bigint, const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_sub(result.mpz, a.mpz, b.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      const b_ = b;
      mpz_sub(result.mpz, a_.mpz, b_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        const b_ = b;
        mpz_sub(result.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  @deprecated(notes="bigint.sub method is deprecated - please use the standalone function :proc:`~BigInteger.sub`")
  proc bigint.sub(const ref a: bigint, const ref b: bigint) {
    BigInteger.sub(this, a, b);
  }

  proc sub(ref result: bigint, const ref a: bigint, b: int) {
    if b >= 0 {
      BigInteger.sub(result, a, b:uint);
    } else {
      BigInteger.add(result, a, (0 - b):uint);
    }
  }

  @deprecated(notes="bigint.sub method is deprecated - please use the standalone function :proc:`~BigInteger.sub`")
  proc bigint.sub(const ref a: bigint, b: int) {
    BigInteger.sub(this, a, b);
  }

  proc sub(ref result:bigint, const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_sub_ui(result.mpz, a.mpz, b_);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      mpz_sub_ui(result.mpz, a_.mpz, b_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        mpz_sub_ui(result.mpz, a_.mpz, b_);
      }
    }
  }

  @deprecated(notes="bigint.sub method is deprecated - please use the standalone function :proc:`~BigInteger.sub`")
  proc bigint.sub(const ref a: bigint, b: uint) {
    BigInteger.sub(this, a, b);
  }

  proc sub(ref result: bigint, a: int, const ref b: bigint) {
    if a >= 0 {
      BigInteger.sub(result, a:uint, b);
    } else {
      const a_ = (0 - a).safeCast(c_ulong);

      if _local {
        mpz_add_ui(result.mpz, b.mpz, a_);
        mpz_neg(result.mpz, result.mpz);
      } else if result.localeId == chpl_nodeID {
        const b_ = b;
        mpz_add_ui(result.mpz, b_.mpz, a_);
        mpz_neg(result.mpz, result.mpz);
      } else {
        const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
        on __primitive("chpl_on_locale_num", resultLoc) {
          const b_ = b;
          mpz_add_ui(result.mpz, b_.mpz, a_);
          mpz_neg(result.mpz, result.mpz);
        }
      }
    }
  }

  @deprecated(notes="bigint.sub method is deprecated - please use the standalone function :proc:`~BigInteger.sub`")
  proc bigint.sub(a: int, const ref b: bigint) {
    BigInteger.sub(this, a, b);
  }

  proc sub(ref result: bigint, a: uint, const ref b: bigint) {
    const a_ = a.safeCast(c_ulong);

    if _local {
      mpz_ui_sub(result.mpz, a_, b.mpz);
    } else if result.localeId == chpl_nodeID {
      const b_ = b;
      mpz_ui_sub(result.mpz, a_, b_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const b_ = b;
        mpz_ui_sub(result.mpz, a_, b_.mpz);
      }
    }
  }

  @deprecated(notes="bigint.sub method is deprecated - please use the standalone function :proc:`~BigInteger.sub`")
  proc bigint.sub(a: uint, const ref b: bigint) {
    BigInteger.sub(this, a, b);
  }

  proc mul(ref result: bigint, const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_mul(result.mpz, a.mpz, b.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      const b_ = b;
      mpz_mul(result.mpz, a_.mpz, b_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        const b_ = b;
        mpz_mul(result.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  @deprecated(notes="bigint.mul method is deprecated - please use the standalone function :proc:`~BigInteger.mul`")
  proc bigint.mul(const ref a: bigint, const ref b: bigint) {
    BigInteger.mul(this, a, b);
  }

  proc mul(ref result: bigint, const ref a: bigint, b: int) {
    const b_ = b.safeCast(c_long);

    if _local {
      mpz_mul_si(result.mpz, a.mpz, b_);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      mpz_mul_si(result.mpz, a_.mpz, b_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        mpz_mul_si(result.mpz, a_.mpz, b_);
      }
    }
  }

  @deprecated(notes="bigint.mul method is deprecated - please use the standalone function :proc:`~BigInteger.mul`")
  proc bigint.mul(const ref a: bigint, b: int) {
    BigInteger.mul(this, a, b);
  }

  proc mul(ref result: bigint, const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_long);

    if _local {
      mpz_mul_ui(result.mpz, a.mpz, b_);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      mpz_mul_ui(result.mpz, a_.mpz, b_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        mpz_mul_ui(result.mpz, a_.mpz, b_);
      }
    }
  }

  @deprecated(notes="bigint.mul method is deprecated - please use the standalone function :proc:`~BigInteger.mul`")
  proc bigint.mul(const ref a: bigint, b: uint) {
    BigInteger.mul(this, a, b);
  }

  proc addmul(ref result: bigint, const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_addmul(result.mpz, a.mpz, b.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      const b_ = b;
      mpz_addmul(result.mpz, a_.mpz, b_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        const b_ = b;
        mpz_addmul(result.mpz, a_.mpz, b_.mpz);
      }
    }
  }


  @deprecated(notes="bigint.addmul method is deprecated - please use the standalone function :proc:`~BigInteger.addmul`")
  proc bigint.addmul(const ref a: bigint, const ref b: bigint) {
    BigInteger.addmul(this, a, b);
  }

  proc addmul(ref result: bigint, const ref a: bigint, b: int) {
    if b >= 0 {
      BigInteger.addmul(result, a, b:uint);
    } else {
      BigInteger.submul(result, a, (0 - b):uint);
    }
  }

  @deprecated(notes="bigint.addmul method is deprecated - please use the standalone function :proc:`~BigInteger.addmul`")
  proc bigint.addmul(const ref a: bigint, b: int) {
    BigInteger.addmul(this, a, b);
  }

  proc addmul(ref result: bigint, const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_addmul_ui(result.mpz, a.mpz, b_);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      mpz_addmul_ui(result.mpz, a_.mpz, b_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        mpz_addmul_ui(result.mpz, a_.mpz, b_);
      }
    }
  }

  @deprecated(notes="bigint.addmul method is deprecated - please use the standalone function :proc:`~BigInteger.addmul`")
  proc bigint.addmul(const ref a: bigint, b: uint) {
    BigInteger.addmul(this, a, b);
  }

  proc submul(ref result: bigint, const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_submul(result.mpz, a.mpz, b.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      const b_ = b;
      mpz_submul(result.mpz, a_.mpz, b_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        const b_ = b;
        mpz_submul(result.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  @deprecated(notes="bigint.submul method is deprecated - please use the standalone function :proc:`~BigInteger.submul`")
  proc bigint.submul(const ref a: bigint, const ref b: bigint) {
    BigInteger.submul(this, a, b);
  }

  proc submul(ref result: bigint, const ref a: bigint, b: int) {
    if b >= 0 {
      BigInteger.submul(result, a, b:uint);
    } else {
      BigInteger.addmul(result, a, (0 - b):uint);
    }
  }

  @deprecated(notes="bigint.submul method is deprecated - please use the standalone function :proc:`~BigInteger.submul`")
  proc bigint.submul(const ref a: bigint, b: int) {
    BigInteger.submul(this, a, b);
  }

  proc submul(ref result: bigint, const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_submul_ui(result.mpz, a.mpz, b_);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      mpz_submul_ui(result.mpz, a_.mpz, b_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        mpz_submul_ui(result.mpz, a_.mpz, b_);
      }
    }
  }

  @deprecated(notes="bigint.submul method is deprecated - please use the standalone function :proc:`~BigInteger.submul`")
  proc bigint.submul(const ref a: bigint, b: uint) {
    BigInteger.submul(this, a, b);
  }

  proc mul_2exp(ref result: bigint, const ref a: bigint, b: integral) {
    const b_ = b.safeCast(mp_bitcnt_t);

    if _local {
      mpz_mul_2exp(result.mpz, a.mpz, b_);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      mpz_mul_2exp(result.mpz, a_.mpz, b_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        mpz_mul_2exp(result.mpz, a_.mpz, b_);
      }
    }
  }

  @deprecated(notes="bigint.mul_2exp method is deprecated - please use the standalone function :proc:`~BigInteger.mul_2exp`")
  proc bigint.mul_2exp(const ref a: bigint, b: integral) {
    BigInteger.mul_2exp(this, a, b);
  }

  proc neg(ref result: bigint, const ref a: bigint) {
    if _local {
      mpz_neg(result.mpz, a.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      mpz_neg(result.mpz, a_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        mpz_neg(result.mpz, a_.mpz);
      }
    }
  }

  @deprecated(notes="bigint.neg method is deprecated - please use the standalone function :proc:`~BigInteger.neg`")
  proc bigint.neg(const ref a: bigint) {
    BigInteger.neg(this, a);
  }

  proc abs(ref result: bigint, const ref a: bigint) {
    if _local {
      mpz_abs(result.mpz, a.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      mpz_abs(result.mpz, a_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        mpz_abs(result.mpz, a_.mpz);
      }
    }
  }

  @deprecated(notes="bigint.abs method is deprecated - please use the standalone function :proc:`~BigInteger.abs`")
  proc bigint.abs(const ref a: bigint) {
    BigInteger.abs(this, a);
  }

  /*
    .. warning::

       bigint.div_q using Round is deprecated, use bigint.divQ with round
       instead
  */
  @deprecated
  ("bigint.div_q using Round is deprecated, use bigint.divQ with round instead")
  proc bigint.div_q(const ref n: bigint,
                    const ref d: bigint,
                    param     rounding = Round.ZERO) {
    use Round;
    if (rounding == UP) {
      BigInteger.divQ(this, n, d, round.up);
    } else if (rounding == ZERO) {
      BigInteger.divQ(this, n, d, round.zero);
    } else {
      BigInteger.divQ(this, n, d, round.down);
    }
  }

  /*
    .. warning::

       bigint.div_q using Round is deprecated, use bigint.divQ with round
       instead
  */
  @deprecated
  ("bigint.div_q using Round is deprecated, use bigint.divQ with round instead")
  proc bigint.div_q(const ref n: bigint,
                              d: integral,
                    param     rounding = Round.ZERO) {
    use Round;
    if (rounding == UP) {
      BigInteger.divQ(this, n, d, round.up);
    } else if (rounding == ZERO) {
      BigInteger.divQ(this, n, d, round.zero);
    } else {
      BigInteger.divQ(this, n, d, round.down);
    }
  }

  // 5.6 Division Functions
  // Note: Documentation on `denom: integral` version
  proc divQ(ref result: bigint,
            const ref numer: bigint,
            const ref denom: bigint,
            param rounding = round.zero) {
    if (chpl_checkDivByZero) then
      if denom == 0 then
        halt("Attempt to divide by zero");

    inline proc helper(ref res, const ref n, const ref d, param rnd) {
      select rnd {
        when round.up   do mpz_cdiv_q(res.mpz, n.mpz,  d.mpz);
        when round.down do mpz_fdiv_q(res.mpz, n.mpz,  d.mpz);
        when round.zero do mpz_tdiv_q(res.mpz, n.mpz,  d.mpz);
      }
    }

    if _local {
      helper(result, numer, denom, rounding);
    } else if result.localeId == chpl_nodeID {
      const numer_ = numer;
      const denom_ = denom;
      helper(result, numer_, denom_, rounding);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const numer_ = numer;
        const denom_ = denom;
        helper(result, numer_, denom_, rounding);
      }
    }
  }

  @deprecated(notes="bigint.divQ method is deprecated - please use the standalone function :proc:`~BigInteger.divQ`")
  proc bigint.divQ(const ref numer: bigint,
                   const ref denom: bigint,
                   param rounding = round.zero) {
    BigInteger.divQ(this, numer, denom, rounding);
  }


  /* Divide ``numer`` by ``denom``, forming a quotient and storing it in
     ``result``.

     :arg result: Where the result is stored
     :type result: :record:`bigint`

     :arg numer: The numerator of the division operation to be performed
     :type numer: :record:`bigint`

     :arg denom: The denominator of the division operation to be performed
     :type denom: :record:`bigint`, ``integral``

     :arg rounding: The rounding style to use, see :enum:`round` for a
                    description of what the rounding styles entail.  Defaults to
                    ``zero`` if unspecified
     :type rounding: ``round``

     .. warning::
        If the denominator is zero, the program behavior is undefined.
  */
  proc divQ(ref result: bigint,
            const ref numer: bigint,
            denom: integral,
            param rounding = round.zero) {
    BigInteger.divQ(result, numer, new bigint(denom), rounding);
  }

  /* Divide ``numer`` by ``denom``, forming a quotient and storing it in
     ``this``.

     :arg numer: The numerator of the division operation to be performed
     :type numer: :record:`bigint`

     :arg denom: The denominator of the division operation to be performed
     :type denom: :record:`bigint`, ``integral``

     :arg rounding: The rounding style to use, see :enum:`round` for a
                    description of what the rounding styles entail.  Defaults to
                    ``zero`` if unspecified
     :type rounding: ``round``

     .. warning::
        If the denominator is zero, the program behavior is undefined.
  */
  @deprecated(notes="bigint.divQ method is deprecated - please use the standalone function :proc:`~BigInteger.divQ`")
  proc bigint.divQ(const ref numer: bigint,
                             denom: integral,
                   param     rounding = round.zero) {

    BigInteger.divQ(this, numer, denom, rounding);
  }

  /*
    .. warning::

       bigint.div_r using Round is deprecated, use bigint.divR with round
       instead
  */
  @deprecated
  ("bigint.div_r using Round is deprecated, use bigint.divR with round instead")
  proc bigint.div_r(const ref n: bigint,
                    const ref d: bigint,
                    param     rounding = Round.ZERO) {
    use Round;
    if (rounding == UP) {
      BigInteger.divR(this, n, d, round.up);
    } else if (rounding == ZERO) {
      BigInteger.divR(this, n, d, round.zero);
    } else {
      BigInteger.divR(this, n, d, round.down);
    }

  }

  /*
    .. warning::

       bigint.div_r using Round is deprecated, use bigint.divR with round
       instead
  */
  @deprecated
  ("bigint.div_r using Round is deprecated, use bigint.divR with round instead")
  proc bigint.div_r(const ref n: bigint,
                              d: integral,
                    param     rounding = Round.ZERO) {
    use Round;
    if (rounding == UP) {
      BigInteger.divR(this, n, d, round.up);
    } else if (rounding == ZERO) {
      BigInteger.divR(this, n, d, round.zero);
    } else {
      BigInteger.divR(this, n, d, round.down);
    }
  }

  // Note: documentation on `denom: integral` version
  proc divR(ref result: bigint,
            const ref numer: bigint,
            const ref denom: bigint,
            param rounding = round.zero) {
    if (chpl_checkDivByZero) then
      if denom == 0 then
        halt("Attempt to divide by zero");

    inline proc helper(ref res, const ref n, const ref d, param rnd) {
      select rnd {
        when round.up   do mpz_cdiv_r(res.mpz, n.mpz, d.mpz);
        when round.down do mpz_fdiv_r(res.mpz, n.mpz, d.mpz);
        when round.zero do mpz_tdiv_r(res.mpz, n.mpz, d.mpz);
      }
    }

    if _local {
      helper(result, numer, denom, rounding);
    } else if result.localeId == chpl_nodeID {
      const numer_ = numer;
      const denom_ = denom;
      helper(result, numer_, denom_, rounding);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const numer_ = numer;
        const denom_ = denom;
        helper(result, numer_, denom_, rounding);
      }
    }
  }

  @deprecated(notes="bigint.divR method is deprecated - please use the standalone function :proc:`~BigInteger.divR`")
  proc bigint.divR(const ref numer: bigint,
                   const ref denom: bigint,
                   param     rounding = round.zero) {
    BigInteger.divR(this, numer, denom, rounding);
  }

  /* Divide ``numer`` by ``denom``, forming a remainder and storing it in
     ``result``.  The absolute value of the remainder will always be less than the
     absolute value of the denominator (i.e. ``abs(result) < abs(denom)``).

     :arg result: Where the result is stored
     :type result: :record:`bigint`

     :arg numer: The numerator of the division operation to be performed
     :type numer: :record:`bigint`

     :arg denom: The denominator of the division operation to be performed
     :type denom: :record:`bigint`, ``integral``

     :arg rounding: The rounding style to use, see :enum:`round` for a
                    description of what the rounding styles entail.  Defaults to
                    ``zero`` if unspecified
     :type rounding: ``round``

     .. warning::
        If the denominator is zero, the program behavior is undefined.
  */
  proc divR(ref result: bigint,
            const ref numer: bigint,
            denom: integral,
            param rounding = round.zero) {
    BigInteger.divR(result, numer, new bigint(denom), rounding);
  }

  /* Divide ``numer`` by ``denom``, forming a remainder and storing it in
     ``this``.  The absolute value of the remainder will always be less than the
     absolute value of the denominator (i.e. ``abs(this) < abs(denom)``).

     :arg numer: The numerator of the division operation to be performed
     :type numer: :record:`bigint`

     :arg denom: The denominator of the division operation to be performed
     :type denom: :record:`bigint`, ``integral``

     :arg rounding: The rounding style to use, see :enum:`round` for a
                    description of what the rounding styles entail.  Defaults to
                    ``zero`` if unspecified
     :type rounding: ``round``

     .. warning::
        If the denominator is zero, the program behavior is undefined.
  */
  @deprecated(notes="bigint.divR method is deprecated - please use the standalone function :proc:`~BigInteger.divR`")
  proc bigint.divR(const ref numer: bigint,
                             denom: integral,
                   param     rounding = round.zero) {
    BigInteger.divR(this, numer, denom, rounding);
  }

  /*
    .. warning::

       bigint.div_qr using Round is deprecated, use bigint.divQR with round
       instead
  */
  @deprecated
  ("bigint.div_qr using Round is deprecated, use bigint.divQR with round instead")
  proc bigint.div_qr(ref       r:        bigint,
                     const ref n:        bigint,
                     const ref d:        bigint,
                     param     rounding = Round.ZERO) {
    use Round;
    if (rounding == UP) {
      BigInteger.divQR(this, r, n, d, round.up);
    } else if (rounding == ZERO) {
      BigInteger.divQR(this, r, n, d, round.zero);
    } else {
      BigInteger.divQR(this, r, n, d, round.down);
    }
  }

  /*
    .. warning::

       bigint.div_qr using Round is deprecated, use bigint.divQR with round
       instead
  */
  @deprecated
  ("bigint.div_qr using Round is deprecated, use bigint.divQR with round instead")
  proc bigint.div_qr(ref       r: bigint,
                     const ref n: bigint,
                               d: integral,
                     param     rounding = Round.ZERO) {
    use Round;
    if (rounding == UP) {
      BigInteger.divQR(this, r, n, d, round.up);
    } else if (rounding == ZERO) {
      BigInteger.divQR(this, r, n, d, round.zero);
    } else {
      BigInteger.divQR(this, r, n, d, round.down);
    }
  }

  proc divQR(ref       result: bigint,
             ref       remain: bigint,
             const ref numer: bigint,
             const ref denom: bigint,
             param     rounding = round.zero) {
    if (chpl_checkDivByZero) then
      if denom == 0 then
        halt("Attempt to divide by zero");

    inline proc helper(ref res, ref rem, const ref n, const ref d, param rnd) {
      select rnd {
        when round.up do mpz_cdiv_qr(res.mpz, rem.mpz, n.mpz, d.mpz);
        when round.down do mpz_fdiv_qr(res.mpz, rem.mpz, n.mpz, d.mpz);
        when round.zero do mpz_tdiv_qr(res.mpz, rem.mpz, n.mpz, d.mpz);
      }
    }

    if _local {
      helper(result, remain, numer, denom, rounding);
    } else if result.localeId == chpl_nodeID {
      // TODO: need to revisit this in relation to Cray/chapel-private#4628
      var   remain_ : bigint;
      const numer_ = numer;
      const denom_ = denom;
      helper(result, remain_, numer_, denom_, rounding);
      remain = remain_;
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        var   remain_ : bigint;
        const numer_ = numer;
        const denom_ = denom;
        helper(result, remain_, numer_, denom_, rounding);
        remain = remain_;
      }
    }
  }

  // Note: Documentation on `denom: integral` version
  @deprecated(notes="bigint.divQR method is deprecated - please use the standalone function :proc:`~BigInteger.divQR`")
  proc bigint.divQR(ref       remain: bigint,
                    const ref numer: bigint,
                    const ref denom: bigint,
                    param     rounding = round.zero) {
    BigInteger.divQR(this, remain, numer, denom, rounding);
  }

  /* Divide ``numer`` by ``denom``, forming a quotient and storing it in
     ``result``, and a remainder and storing it in ``remain``.  The quotient and
     remainder will always satisfy ``numer = result*denom + remain`` after the
     operation has finished.  The absolute value of the remainder will always be
     less than the absolute value of the denominator (i.e. ``abs(result) <
     abs(denom)``).

     .. warning::
        If ``result`` is also passed as the ``remain`` argument, the program
        behavior is undefined.

     :arg result: Where the result is stored
     :type result: :record:`bigint`

     :arg remain: Stores the remainder of the division
     :type remain: :record:`bigint`

     :arg numer: The numerator of the division operation to be performed
     :type numer: :record:`bigint`

     :arg denom: The denominator of the division operation to be performed
     :type denom: :record:`bigint`, ``integral``

     :arg rounding: The rounding style to use, see :enum:`round` for a
                    description of what the rounding styles entail.  Defaults to
                    ``zero`` if unspecified
     :type rounding: ``round``

     .. warning::
        If the denominator is zero, the program behavior is undefined.
  */
  proc divQR(ref result: bigint,
             ref remain: bigint,
             const ref numer: bigint,
             denom: integral,
             param rounding = round.zero) {
    BigInteger.divQR(result, remain, numer, new bigint(denom), rounding);
  }

  /* Divide ``numer`` by ``denom``, forming a quotient and storing it in
     ``this``, and a remainder and storing it in ``remain``.  The quotient and
     remainder will always satisfy ``numer = this*denom + remain`` after the
     operation has finished.  The absolute value of the remainder will always be
     less than the absolute value of the denominator (i.e. ``abs(this) <
     abs(denom)``).

     .. warning::
        If ``this`` is also passed as the ``remain`` argument, the program
        behavior is undefined.

     :arg remain: Stores the remainder of the division
     :type remain: :record:`bigint`

     :arg numer: The numerator of the division operation to be performed
     :type numer: :record:`bigint`

     :arg denom: The denominator of the division operation to be performed
     :type denom: :record:`bigint`, ``integral``

     :arg rounding: The rounding style to use, see :enum:`round` for a
                    description of what the rounding styles entail.  Defaults to
                    ``zero`` if unspecified
     :type rounding: ``round``

     .. warning::
        If the denominator is zero, the program behavior is undefined.
  */
  @deprecated(notes="bigint.divQR method is deprecated - please use the standalone function :proc:`~BigInteger.divQR`")
  proc bigint.divQR(ref       remain: bigint,
                    const ref numer: bigint,
                              denom: integral,
                    param     rounding = round.zero) {
    BigInteger.divQR(this, remain, numer, denom, rounding);
  }

  /*
    .. warning::

       bigint.div_q_2exp using Round is deprecated, use bigint.divQ2Exp with
       round instead
  */
  @deprecated
  ("bigint.div_q_2exp using Round is deprecated, use bigint.divQ2Exp with round instead")
  proc bigint.div_q_2exp(const ref n: bigint,
                                   b: integral,
                         param     rounding = Round.ZERO) {
    use Round;
    if (rounding == UP) {
      BigInteger.divQ2Exp(this, n, b, round.up);
    } else if (rounding == ZERO) {
      BigInteger.divQ2Exp(this, n, b, round.zero);
    } else {
      BigInteger.divQ2Exp(this, n, b, round.down);
    }
  }

  /* Divide ``numer`` by ``2^exp``, forming a quotient and storing it in
     ``result``.

     :arg result: Where the result is stored
     :type result: :record:`bigint`

     :arg numer: The numerator of the division operation to be performed
     :type numer: :record:`bigint`

     :arg exp: The exponent that 2 should be raised to before being used as the
               denominator of the division operation to be performed
     :type exp: ``integral``

     :arg rounding: The rounding style to use, see :enum:`round` for a
                    description of what the rounding styles entail.  Defaults to
                    ``zero`` if unspecified
     :type rounding: ``round``
   */
  proc divQ2Exp(ref result: bigint,
                const ref numer: bigint,
                exp: integral,
                param rounding = round.zero) {
    const exp_ = exp.safeCast(mp_bitcnt_t);

    inline proc helper(ref res, const ref n, e, param rnd) {
      select rnd {
        when round.up   do mpz_cdiv_q_2exp(res.mpz, n.mpz, e);
        when round.down do mpz_fdiv_q_2exp(res.mpz, n.mpz, e);
        when round.zero do mpz_tdiv_q_2exp(res.mpz, n.mpz, e);
      }
    }

    if _local {
      helper(result, numer, exp_, rounding);
    } else if result.localeId == chpl_nodeID {
      const numer_ = numer;
      helper(result, numer_, exp_, rounding);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const numer_ = numer;
        helper(result, numer_, exp_, rounding);
      }
    }
  }

  /* Divide ``numer`` by ``2^exp``, forming a quotient and storing it in
     ``this``.

     :arg numer: The numerator of the division operation to be performed
     :type numer: :record:`bigint`

     :arg exp: The exponent that 2 should be raised to before being used as the
               denominator of the division operation to be performed
     :type exp: ``integral``

     :arg rounding: The rounding style to use, see :enum:`round` for a
                    description of what the rounding styles entail.  Defaults to
                    ``zero`` if unspecified
     :type rounding: ``round``
   */
  @deprecated(notes="bigint.divQ2Exp method is deprecated - please use the standalone function :proc:`~BigInteger.divQ2Exp`")
  proc bigint.divQ2Exp(const ref numer: bigint,
                                 exp: integral,
                       param     rounding = round.zero) {
    BigInteger.divQ2Exp(this, numer, exp, rounding);
  }

  /*
    .. warning::

       bigint.div_r_2exp using Round is deprecated, use bigint.divR2Exp with
       round instead
  */
  @deprecated
  ("bigint.div_r_2exp using Round is deprecated, use bigint.divR2Exp with round instead")
  proc bigint.div_r_2exp(const ref n: bigint,
                                   b: integral,
                         param     rounding = Round.ZERO) {
    use Round;
    if (rounding == UP) {
      BigInteger.divR2Exp(this, n, b, round.up);
    } else if (rounding == ZERO) {
      BigInteger.divR2Exp(this, n, b, round.zero);
    } else {
      BigInteger.divR2Exp(this, n, b, round.down);
    }
  }

  /* Divide ``numer`` by ``2^exp``, forming a remainder and storing it in
     ``result``.

     :arg result: Where the result is stored
     :type result: :record:`bigint`

     :arg numer: The numerator of the division operation to be performed
     :type numer: :record:`bigint`

     :arg exp: The exponent that 2 should be raised to before being used as the
               denominator of the division operation to be performed
     :type exp: ``integral``

     :arg rounding: The rounding style to use, see :enum:`round` for a
                    description of what the rounding styles entail.  Defaults to
                    ``zero`` if unspecified
     :type rounding: ``round``
   */
  proc divR2Exp(ref result: bigint,
                const ref numer: bigint,
                exp: integral,
                param     rounding = round.zero) {
    const exp_ = exp.safeCast(mp_bitcnt_t);

    inline proc helper(ref res, const ref n, e, param rnd) {
      select rnd {
        when round.up   do mpz_cdiv_r_2exp(res.mpz, n.mpz, e);
        when round.down do mpz_fdiv_r_2exp(res.mpz, n.mpz, e);
        when round.zero do mpz_tdiv_r_2exp(res.mpz, n.mpz, e);
      }
    }

    if _local {
      helper(result, numer, exp_, rounding);
    } else if result.localeId == chpl_nodeID {
      const numer_ = numer;
      helper(result, numer_, exp_, rounding);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const numer_ = numer;
        helper(result, numer_, exp_, rounding);
      }
    }
  }

  /* Divide ``numer`` by ``2^exp``, forming a remainder and storing it in
     ``this``.

     :arg numer: The numerator of the division operation to be performed
     :type numer: :record:`bigint`

     :arg exp: The exponent that 2 should be raised to before being used as the
               denominator of the division operation to be performed
     :type exp: ``integral``

     :arg rounding: The rounding style to use, see :enum:`round` for a
                    description of what the rounding styles entail.  Defaults to
                    ``zero`` if unspecified
     :type rounding: ``round``
   */
  @deprecated(notes="bigint.divR2Exp method is deprecated - please use the standalone function :proc:`~BigInteger.divR2Exp`")
  proc bigint.divR2Exp(const ref numer: bigint,
                                 exp: integral,
                       param     rounding = round.zero) {
    BigInteger.divR2Exp(this, numer, exp, rounding);
  }


  /* Computes the mod operator on the two arguments, defined as
     ``mod(a, b) = a - b * floor(a / b)``.

     The result is stored in ``result``.

     The result is always >= 0 if `b` > 0.
     It is an error if `b` == 0.
  */
  proc mod(ref result: bigint, const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_fdiv_r(result.mpz, a.mpz, b.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      const b_ = b;
      mpz_fdiv_r(result.mpz, a_.mpz, b_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        const b_ = b;
        mpz_fdiv_r(result.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  /* Computes the mod operator on the two arguments, defined as
     ``mod(a, b) = a - b * floor(a / b)``.

     The result is stored in ``this``.

     The result is always >= 0 if `b` > 0.
     It is an error if `b` == 0.
  */
  @deprecated(notes="bigint.mod method is deprecated - please use the standalone function :proc:`~BigInteger.mod`")
  proc bigint.mod(const ref a: bigint, const ref b: bigint) {
    BigInteger.mod(this, a, b);
  }

  /* Computes the mod operator on the two arguments, defined as
     ``mod(a, b) = a - b * floor(a / b)``.

     If b is of an unsigned type, then
     fewer conditionals will be evaluated at run time.

     The result is stored in ``result`` and returned as an ``int``.

     The result is always >= 0 if `b` > 0.
     It is an error if `b` == 0.
  */
  proc mod(ref result: bigint, const ref a: bigint, b: integral) : int {
    var b_ : c_ulong;
    var rem: c_ulong;

    inline proc helper(ref res, ref rem, const ref a, b) {
      if _local {
        rem = mpz_fdiv_r_ui(res.mpz, a.mpz, b_);
      } else if res.localeId == chpl_nodeID {
        const a_ = a;
        rem = mpz_fdiv_r_ui(res.mpz, a_.mpz, b_);
      } else {
        const resLoc = chpl_buildLocaleID(res.localeId, c_sublocid_any);
        on __primitive("chpl_on_locale_num", resLoc) {
          const a_ = a;
          rem = mpz_fdiv_r_ui(res.mpz, a_.mpz, b_);
        }
      }
    }

    if isNonnegative(b) {
      b_ = b.safeCast(c_ulong);
      helper(result, rem, a, b_);
      return rem.safeCast(int);
    } else {
      if b >= 0 then
        b_ = b.safeCast(c_ulong);
      else
        b_ = (0 - b).safeCast(c_ulong);

      helper(result, rem, a, b_);

      if rem == 0
        then return 0;
      else if b < 0 {
        result += b;
        return rem.safeCast(int) + b;
      } else
        return rem.safeCast(int);
    }
  }

  /* Computes the mod operator on the two arguments, defined as
     ``mod(a, b) = a - b * floor(a / b)``.

     If b is of an unsigned type, then
     fewer conditionals will be evaluated at run time.

     The result is stored in ``this`` and returned as an ``int``.

     The result is always >= 0 if `b` > 0.
     It is an error if `b` == 0.
  */
  @deprecated(notes="bigint.mod method is deprecated - please use the standalone function :proc:`~BigInteger.mod`")
  proc bigint.mod(const ref a: bigint, b: integral) : int {
    return BigInteger.mod(this, a, b);
  }

  // Comparison Functions
  proc bigint.cmp(const ref b: bigint) : int {
    var ret: c_int;

    if _local {
      ret = mpz_cmp(this.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      ret = mpz_cmp(this.mpz, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const b_ = b.localize();

        ret = mpz_cmp(this.mpz, b_.mpz);
      }
    }

    return ret.safeCast(int);
  }

  proc bigint.cmp(b: int) : int {
    const b_ = b.safeCast(c_long);
    var   ret: c_int;

    if _local {
      ret = mpz_cmp_si(this.mpz, b_);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_cmp_si(this.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        ret = mpz_cmp_si(this.mpz, b_);
      }
    }

    return ret.safeCast(int);
  }

  proc bigint.cmp(b: uint) : int {
    const b_ = b.safeCast(c_ulong);
    var   ret: c_int;

    if _local {
      ret = mpz_cmp_ui(this.mpz, b_);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_cmp_ui(this.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        ret = mpz_cmp_ui(this.mpz, b_);
      }
    }

    return ret.safeCast(int);
  }

  proc bigint.cmp(b: real) : int {
    const b_ = b : c_double;
    var   ret: c_int;

    if _local {
      ret = mpz_cmp_d(this.mpz, b_);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_cmp_d(this.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        ret = mpz_cmp_d(this.mpz, b_);
      }
    }

    return ret.safeCast(int);
  }



  proc bigint.cmpabs(const ref b: bigint) : int {
    var ret: c_int;

    if _local {
      ret = mpz_cmpabs(this.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      ret = mpz_cmpabs(this.mpz, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var b_ = b;

        ret = mpz_cmpabs(this.mpz, b_.mpz);
      }
    }

    return ret.safeCast(int);
  }

  proc bigint.cmpabs(b: uint) : int {
    const b_ = b.safeCast(c_ulong);
    var   ret: c_int;

    if _local {
      ret = mpz_cmpabs_ui(this.mpz, b_);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_cmpabs_ui(this.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        ret = mpz_cmpabs_ui(this.mpz, b_);
      }
    }

    return ret.safeCast(int);
  }

  proc bigint.cmpabs(b: real) : int {
    const b_ = b : c_double;
    var   ret: c_int;

    if _local {
      ret = mpz_cmpabs_d(this.mpz, b_);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_cmpabs_d(this.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        ret = mpz_cmpabs_d(this.mpz, b_);
      }
    }

    return ret.safeCast(int);
  }



  proc bigint.sgn() : int {
    var ret: c_int;

    if _local {
      ret = mpz_sgn(this.mpz);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_sgn(this.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        ret = mpz_sgn(this.mpz);
      }
    }

    return ret.safeCast(int);
  }


  // Logical and Bit Manipulation Functions
  proc and(ref result: bigint, const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_and(result.mpz, a.mpz, b.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      const b_ = b;
      mpz_and(result.mpz, a_.mpz, b_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        const b_ = b;
        mpz_and(result.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  @deprecated(notes="bigint.and method is deprecated - please use the standalone function :proc:`~BigInteger.and`")
  proc bigint.and(const ref a: bigint, const ref b: bigint) {
    BigInteger.and(this, a, b);
  }

  proc ior(ref result: bigint, const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_ior(result.mpz, a.mpz, b.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      const b_ = b;
      mpz_ior(result.mpz, a_.mpz, b_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        const b_ = b;
        mpz_ior(result.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  @deprecated(notes="bigint.ior method is deprecated - please use the standalone function :proc:`~BigInteger.ior`")
  proc bigint.ior(const ref a: bigint, const ref b: bigint) {
    BigInteger.ior(this, a, b);
  }

  proc xor(ref result: bigint, const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_xor(result.mpz, a.mpz, b.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      const b_ = b;
      mpz_xor(result.mpz, a_.mpz, b_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        const b_ = b;
        mpz_xor(result.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  @deprecated(notes="bigint.xor method is deprecated - please use the standalone function :proc:`~BigInteger.xor`")
  proc bigint.xor(const ref a: bigint, const ref b: bigint) {
    BigInteger.xor(this, a, b);
  }


  proc com(ref result: bigint, const ref a: bigint) {
    if _local {
      mpz_com(result.mpz, a.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      mpz_com(result.mpz, a_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_any);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        mpz_com(result.mpz, a_.mpz);
      }
    }
  }

  @deprecated(notes="bigint.com method is deprecated - please use the standalone function :proc:`~BigInteger.com`")
  proc bigint.com(const ref a: bigint) {
    BigInteger.com(this, a);
  }



  // Assignment functions
  proc bigint.set(const ref a: bigint) {
    if _local {
      mpz_set(this.mpz, a.mpz);

    } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
      mpz_set(this.mpz, a.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const mpz_struct = a.getImpl();

        chpl_gmp_get_mpz(this.mpz, a.localeId, mpz_struct);
      }
    }
  }

  proc bigint.set(num : int) {
    const num_ = num.safeCast(c_long);

    if _local {
      mpz_set_si(this.mpz, num_);

    } else if this.localeId == chpl_nodeID {
      mpz_set_si(this.mpz, num_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_set_si(this.mpz, num_);
      }
    }
  }

  proc bigint.set(num : uint) {
    const num_ = num.safeCast(c_ulong);

    if _local {
      mpz_set_ui(this.mpz, num_);

    } else if this.localeId == chpl_nodeID {
      mpz_set_ui(this.mpz, num_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_set_ui(this.mpz, num_);
      }
    }
  }

  proc bigint.set(num: real) {
    const num_ = num : c_double;

    if _local {
      mpz_set_d(this.mpz, num_);

    } else if this.localeId == chpl_nodeID {
      mpz_set_d(this.mpz, num_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_set_d(this.mpz, num_);
      }
    }
  }

  proc bigint.set(str: string, base: int = 0) {
    const base_ = base.safeCast(c_int);

    if _local {
      mpz_set_str(this.mpz, str.localize().c_str(), base_);

    } else if this.localeId == chpl_nodeID {
      mpz_set_str(this.mpz, str.localize().c_str(), base_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_set_str(this.mpz, str.localize().c_str(), base_);
      }
    }
  }

  proc bigint.swap(ref a: bigint) {
    if _local {
      mpz_swap(this.mpz, a.mpz);

    } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
      mpz_swap(this.mpz, a.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var tmp = new bigint(a);

        a.set(this);

        mpz_set(this.mpz, tmp.mpz);
      }
    }
  }

  @chpldoc.nodoc
  record bigintWrapper {
    pragma "no init"
    var mpz: mpz_t;
    var isOwned: bool;
    proc init(a: mpz_t) {
      mpz = a;
      isOwned = false;
    }

    proc init(a: bigint) {
      this.complete();
      var mpz_struct = a.getImpl();
      mpz_init(this.mpz);
      chpl_gmp_get_mpz(this.mpz, a.localeId, mpz_struct);
      isOwned = true;
    }

    proc deinit() {
      if isOwned then
        mpz_clear(this.mpz);
    }
  }

  @chpldoc.nodoc
  record __serializeHelper {
    var buff: mpz_t;
    var localeId: chpl_nodeID_t;
  }

  @chpldoc.nodoc
  proc bigint.chpl__serialize() {
    return new __serializeHelper(this.mpz, this.localeId);
  }

  @chpldoc.nodoc
  proc type bigint.chpl__deserialize(data) {
    var ret: bigint;
    if data.localeId == chpl_nodeID {
      mpz_set(ret.mpz, data.buff);
    } else {
      chpl_gmp_get_mpz(ret.mpz, data.localeId, data.buff[0]);
    }
    return ret;
  }

  @chpldoc.nodoc
  inline proc bigint.localize() {
    if _local {
      const ret = new bigintWrapper(this.mpz);
      return ret;
    } else if this.localeId == chpl_nodeID {
      const ret = new bigintWrapper(this.mpz);
      return ret;
    } else {
      const ret = new bigintWrapper(this);
      return ret;
    }
  }

  @chpldoc.nodoc
  inline proc bigint.hash(): uint {
    var ret: uint = this > 0;
    if _local {
      hashHelper();

    } else if this.localeId == chpl_nodeID {
      hashHelper();

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        hashHelper();
      }
    }

    return ret;

    inline proc hashHelper() {
      for i in 0..#(chpl_gmp_mpz_nlimbs(this.mpz)) {
        var limb = chpl_gmp_mpz_getlimbn(this.mpz, i);
        ret = chpl__defaultHashCombine(limb.hash(), ret, i + 1);
      }
    }
  }
}
