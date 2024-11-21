/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

The ``bigint`` type supports arithmetic operations on arbitrary
precision integers in a manner that is broadly consistent with
the conventional operations on primitive fixed length integers.

The current implementation is based on the low-level types and
functions defined in the GMP module i.e. it is implemented using the
GNU Multiple Precision Integer Arithmetic library (GMP). More specifically,
:type:`bigint` wraps the GMP type :type:`~GMP.mpz_t`.

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
  use ChplConfig only compiledForSingleLocale;

  /*
   Local copy of IO.EFORMAT as it is being phased out and is private in IO
   */
  private extern proc chpl_macro_int_EFORMAT():c_int;

  // TODO: remove when initializers can throw in their body
  private proc throwingInitWorkaround() throws {
    throw new BadFormatError("Error initializing big integer");
  }

  /*
    The `bigint` type supports arithmetic operations on arbitrary
    precision integers across multiple locales.
  */
  pragma "ignore noinit"
  record bigint : serializable {
    // The underlying GMP C structure
    pragma "no init"
    @chpldoc.nodoc
    var mpz      : mpz_t;              // A dynamic-vector of C integers

    @chpldoc.nodoc
    var localeId : chpl_nodeID_t;      // The locale id for the GMP state

    /*
      Initializes a :record:`bigint` to an initial value of ``0``.
    */
    proc init() {
      init this;
      mpz_init(this.mpz);

      this.localeId = chpl_nodeID;
    }

    /*
      Initializes a :record:`bigint` to the value of ``x``.

      :arg x: The value to be stored in the resulting :record:`bigint`.
      :type x: :record:`bigint`, ``int``, ``uint``
    */
    proc init(const ref x: bigint) {
      init this;
      if compiledForSingleLocale() || x.localeId == chpl_nodeID {
        mpz_init_set(this.mpz, x.mpz);
      } else {
        var mpz_struct = x.getImpl();

        mpz_init(this.mpz);

        chpl_gmp_get_mpz(this.mpz, x.localeId, mpz_struct);
      }

      this.localeId = chpl_nodeID;
    }

    /* See :proc:`init` */
    proc init(x: int) {
      init this;
      mpz_init_set_si(this.mpz, x.safeCast(c_long));

      this.localeId = chpl_nodeID;
    }

    /* See :proc:`init` */
    proc init(x: uint) {
      init this;
      mpz_init_set_ui(this.mpz, x.safeCast(c_ulong));

      this.localeId = chpl_nodeID;
    }

    /* Initialize a :record:`bigint` from a string and optionally a provided base
       to use with the string.  If the string is not a correct base ``base``
       number, will throw a :type:`~OS.BadFormatError`.

       :arg x: The value to be stored in the resulting :record:`bigint`.
       :type x: :type:`~String.string`
       :arg base: The base to use when creating the :record:`bigint` from ``x``.
                  May vary from ``2`` to ``62`` or be ``0``.  Defaults to ``0``,
                  which causes the base to be read from the start of the ``x``
                  itself (``0x`` and ``0X`` will give hexadecimal, ``0b`` and
                  ``0B`` will give binary, ``0`` will give octal, and everything
                  else will be interpreted as decimal).
       :type base: ``int``

       :throws BadFormatError: Thrown when ``x`` is not a correctly formatted
                               number in base ``base``.

     */
    proc init(x: string, base: int = 0) throws {
      init this;
      const ref x_ = x.localize().c_str();
      const base_ = base.safeCast(c_int);

      if mpz_init_set_str(this.mpz, x_, base_) != 0 {
        mpz_clear(this.mpz);

        throwingInitWorkaround();
      }

      this.localeId = chpl_nodeID;
    }

    /*
      Copy initializes a :record:`bigint` to the value of ``x``.

      :arg x: The value to be stored in the resulting :record:`bigint`.
      :type x: :record:`bigint`, ``integral``
    */
    proc init=(const ref x: bigint) do this.init(x);

    /* See :proc:`init=` */
    proc init=(x : integral) do this.init(x);

    // Within a given locale, bigint assignment creates a deep copy of the
    // data and so the record "owns" the GMP data.
    //
    // If a bigint is copied to a remote node then it will receive a shallow
    // copy.  The localeId points back the correct locale but the mpz field
    // is meaningless.
    @chpldoc.nodoc
    proc ref deinit() {
      if compiledForSingleLocale() || this.localeId == chpl_nodeID {
        mpz_clear(this.mpz);
      }
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

       .. seealso::
          :proc:`GMP.mpz_sizeinbase` and
          `mpz_sizeinbase <https://gmplib.org/manual/Miscellaneous-Integer-Functions#index-mpz_005fsizeinbase>`_.
    */
    proc sizeInBase(base: int) : int {
      const base_ = base.safeCast(c_int);
      var   ret: c_size_t;

      if compiledForSingleLocale() {
        ret = mpz_sizeinbase(this.mpz, base_);

      } else if this.localeId == chpl_nodeID {
        ret = mpz_sizeinbase(this.mpz, base_);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_none);

        on __primitive("chpl_on_locale_num", thisLoc) {
          ret = mpz_sizeinbase(this.mpz, base_);
        }
      }

      return ret.safeCast(int);
    }

    /*
      Return the underlying implementation of :record:`bigint`.  Currently,
      the type returned is ``__mpz_struct``.
    */
    @unstable("getImpl is provided as a convenience but its result may change in the future")
    proc getImpl(): __mpz_struct {
      var ret: __mpz_struct;

      if compiledForSingleLocale() {
        ret = this.mpz[0];

      } else if this.localeId == chpl_nodeID {
        ret = this.mpz[0];

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_none);

        on __primitive("chpl_on_locale_num", thisLoc) {
          ret = this.mpz[0];
        }
      }

      return ret;
    }

    /*
      Convert ``this`` to a tuple containing a ``real`` (truncated if necessary, by
      rounding towards zero) and the exponent.  The returned tuple fulfills the
      condition ``d * 2^exp == this`` where ``d`` is the first value in the
      tuple and ``exp`` is the second.

      :returns: a tuple representing the number in multiple parts, ``(d, exp)``,
                such that their combination ``d * 2^exp`` is equal to ``this``.

                ``d`` in this case will be in the range ``0.5 <= abs(d) < 1``,
                unless ``this`` is ``0``, in which case ``d == 0.0`` and
                ``exp == 0``.
      :rtype: ``(real, uint(32))``

      .. seealso::
         :proc:`GMP.mpz_get_d_2exp` and
         `mpz_get_d_2exp <https://gmplib.org/manual/Converting-Integers#index-mpz_005fget_005fd_005f2exp>`_.
     */
    proc getD2Exp(): (real, uint(32)) {
      var exp: c_long;
      var dbl: c_double;

      if compiledForSingleLocale() {
        var tmp: c_long;

        dbl = mpz_get_d_2exp(tmp, this.mpz);
        exp = tmp;

      } else if this.localeId == chpl_nodeID {
        var tmp: c_long;

        dbl = mpz_get_d_2exp(tmp, this.mpz);
        exp = tmp;

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_none);

        on __primitive("chpl_on_locale_num", thisLoc) {
          var tmp: c_long;

          dbl = mpz_get_d_2exp(tmp, this.mpz);
          exp = tmp;
        }
      }

      return (dbl: real, exp.safeCast(uint(32)));
    }

    // private method
    @chpldoc.nodoc
    proc getStr(base: int = 10): string {
      const base_ = base.safeCast(c_int);
      var   ret: string;

      if compiledForSingleLocale() || this.localeId == chpl_nodeID {
        var tmpvar = chpl_gmp_mpz_get_str(base_, this.mpz);
        try! ret = string.createAdoptingBuffer(tmpvar);
      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_none);
        on __primitive("chpl_on_locale_num", thisLoc) {
          var tmpvar = chpl_gmp_mpz_get_str(base_, this.mpz);
          try! ret = string.createAdoptingBuffer(tmpvar);
        }
      }

      return ret;
    }

    /* Writes this number to a :type:`~IO.fileWriter` */
    proc serialize(writer, ref serializer) throws {
      var s: string;
      s = this.getStr();
      writer.write(s);
    }

    /* Read this number from a :type:`~IO.fileReader` */
    proc ref deserialize(reader, ref deserializer) throws {
      var s: string;
      reader.read(s);
      this = new bigint(s);
    }
  }

  /* An enumeration of the different rounding strategies, for use with e.g.
     :proc:`~BigInteger.div` to determine how to round the quotient when performing
     the computation.
   */
  enum roundingMode {
    /*
      Indicates that the quotient should be rounded down towards -infinity and
      any remainder should have the same sign as the denominator.
    */
    down = -1,
    /*
      Indicates that the quotient should be rounded towards zero and any
      remainder should have the same sign as the numerator.
    */
    zero = 0,
    /*
      Indicates that the quotient should be rounded up towards +infinity and any
      remainder should have the opposite sign as the denominator.
    */
    up = 1
  }

  /* Constructs a new :record:`bigint` from ``x``, see :proc:`bigint.init`. */
  inline operator :(x: integral, type t: bigint): bigint {
    return new bigint(x);
  }

  /*
    Constructs a new :record:`bigint` from ``x``,
    see the :proc:`bigint.init` overload which takes a :type:`~String.string`.
  */
  inline operator :(x: string, type t: bigint): bigint throws {
    return new bigint(x);
  }

  /* Constructs a new :record:`bigint` from ``x``, see :proc:`bigint.init`. */
  inline operator :(x: bool, type t: bigint): bigint throws {
    return new bigint(x:int);
  }

  /*
    Convert ``x`` to a signed integer. If ``x`` is larger than ``t``, the value
    returned is the least significant part of ``x`` with the same sign as ``x``.

    .. seealso::
       :proc:`GMP.mpz_get_si` and
       `mpz_get_si <https://gmplib.org/manual/Converting-Integers#index-mpz_005fget_005fsi>`_.
  */
  inline operator :(const ref x: bigint, type t:numeric) where isIntType(t) {
    var ret: c_long;

    if compiledForSingleLocale() || x.localeId == chpl_nodeID {
      ret = mpz_get_si(x.mpz);
    } else {
      const xLoc = chpl_buildLocaleID(x.localeId, c_sublocid_none);

      on __primitive("chpl_on_locale_num", xLoc) {
        ret = mpz_get_si(x.mpz);
      }
    }

    return ret:t;
  }

  /*
    Convert ``x`` to an unsigned integer. If ``x`` is larger than ``t``, the value
    returned is the least significant part of ``x`` ignoring the sign of ``x``.

    .. seealso::
       :proc:`GMP.mpz_get_ui` and
       `mpz_get_ui <https://gmplib.org/manual/Converting-Integers#index-mpz_005fget_005fui>`_.
  */
  inline operator :(const ref x: bigint, type t:numeric) where isUintType(t) {
    var ret: c_ulong;

    if compiledForSingleLocale() || x.localeId == chpl_nodeID {
      ret = mpz_get_ui(x.mpz);
    } else {
      const xLoc = chpl_buildLocaleID(x.localeId, c_sublocid_none);

      on __primitive("chpl_on_locale_num", xLoc) {
        ret = mpz_get_ui(x.mpz);
      }
    }

    return ret:t;
  }

  /*
    Convert ``x`` to a ``real`` with type ``t`` (truncated if necessary, by
    rounding towards zero).

    .. warning::
       If the resulting exponent from the conversion is too big, the result is
       system dependent. If supported, an infinity may be returned. A hardware
       overflow trap may also occur.

    .. seealso::
       :proc:`GMP.mpz_get_d` and
       `mpz_get_d <https://gmplib.org/manual/Converting-Integers#index-mpz_005fget_005fd>`_.
  */
  inline operator :(const ref x: bigint, type t:numeric) where isRealType(t) {
    var ret: c_double;

    if compiledForSingleLocale() || x.localeId == chpl_nodeID {
      ret = mpz_get_d(x.mpz);
    } else {
      const xLoc = chpl_buildLocaleID(x.localeId, c_sublocid_none);

      on __primitive("chpl_on_locale_num", xLoc) {
        ret = mpz_get_d(x.mpz);
      }
    }

    return ret:t;
  }

  /* Convert ``x`` to a string representation. */
  inline operator :(const ref x: bigint, type t: string) {
    return x.getStr();
  }

  /* See :proc:`bigint.set` */
  operator bigint.=(ref lhs: bigint, const ref rhs: bigint) {
    lhs.set(rhs);
  }

  /* See :proc:`bigint.set` */
  operator bigint.=(ref lhs: bigint, rhs: int) {
    lhs.set(rhs);
  }

  /* See :proc:`bigint.set` */
  operator bigint.=(ref lhs: bigint, rhs: uint) {
    lhs.set(rhs);
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

  /* See :proc:`bigint.init` */
  operator bigint.+(const ref a: bigint): bigint {
    return new bigint(a);
  }

  /* See :proc:`~BigInteger.neg` */
  operator bigint.-(const ref a: bigint): bigint {
    var c = new bigint(a);
    BigInteger.neg(c, a);
    return c;
  }

  /* See :proc:`~BigInteger.com` */
  operator bigint.~(const ref a: bigint): bigint {
    var c = new bigint(a);
    BigInteger.com(c, a);
    return c;
  }

  /* See :proc:`~BigInteger.add` */
  operator bigint.+(const ref a: bigint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.add(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.add` */
  operator bigint.+(const ref a: bigint, b: int): bigint {
    var c = new bigint();
    BigInteger.add(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.add` */
  operator bigint.+(const ref a: bigint, b: uint): bigint {
    var c = new bigint();
    BigInteger.add(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.add` */
  operator bigint.+(a: int, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.add(c, b, a);
    return c;
  }

  /* See :proc:`~BigInteger.add` */
  operator bigint.+(a: uint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.add(c, b, a);
    return c;
  }

  /* See :proc:`~BigInteger.sub` */
  operator bigint.-(const ref a: bigint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.sub(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.sub` */
  operator bigint.-(const ref a: bigint, b: int): bigint {
    var c = new bigint();
    BigInteger.sub(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.sub` */
  operator bigint.-(const ref a: bigint, b: uint): bigint {
    var c = new bigint();
    BigInteger.sub(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.sub` */
  operator bigint.-(a: int, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.sub(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.sub` */
  operator bigint.-(a: uint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.sub(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.mul` */
  operator bigint.*(const ref a: bigint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.mul(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.mul` */
  operator bigint.*(const ref a: bigint, b: int): bigint {
    var c = new bigint();
    BigInteger.mul(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.mul` */
  operator bigint.*(const ref a: bigint, b: uint): bigint {
    var c = new bigint();
    BigInteger.mul(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.mul` */
  operator bigint.*(a: int, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.mul(c, b, a);
    return c;
  }

  /* See :proc:`~BigInteger.mul` */
  operator bigint.*(a: uint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.mul(c, b, a);
    return c;
  }

  /* See :proc:`~BigInteger.div` */
  operator bigint./(const ref a: bigint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.div(c, a, b, roundingMode.zero);

    return c;
  }

  /* See :proc:`~BigInteger.div` */
  operator bigint./(const ref a: bigint, b: integral): bigint {
    return a / new bigint(b);
  }

  /* See :proc:`~BigInteger.pow` */
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

  /* See :proc:`~BigInteger.pow` */
  operator bigint.**(const ref base: bigint, exp: int): bigint {
    var c = new bigint();

    if exp >= 0 {
      BigInteger.pow(c, base, exp: c_ulong);
    } else {
      BigInteger.pow(c, base, exp: int);
    }

    return c;
  }

  /* See :proc:`~BigInteger.pow` */
  operator bigint.**(const ref base: bigint, exp: uint): bigint {
    const exp_ = exp.safeCast(c_ulong);
    var   c    = new bigint();

    BigInteger.pow(c, base, exp_);

    return c;
  }

  // helper for % and %=, which is different from `mod`
  private inline proc modTrunc(ref result: bigint,
                               const ref x: bigint,
                               const ref y: bigint)
    do BigInteger.rem(result, x, y, rounding=roundingMode.zero);

  // helper for % and %=, which is different from `mod`
  private inline proc modTrunc(ref result: bigint, const ref x: bigint, y: integral) {
    if (chpl_checkDivByZero) then
      if y == 0 then
        halt("Attempt to divide by zero");

    inline proc helper(ref res, const ref x, y) {
      if compiledForSingleLocale() {
        mpz_tdiv_r_ui(res.mpz, x.mpz, y);

      } else if res.localeId == chpl_nodeID {
        const x_ = x;
        mpz_tdiv_r_ui(res.mpz, x_.mpz, y);

      } else {
        const resLoc = chpl_buildLocaleID(res.localeId, c_sublocid_none);
        on __primitive("chpl_on_locale_num", resLoc) {
          const x_ = x;
          mpz_tdiv_r_ui(res.mpz, x_.mpz, y);
        }
      }
    }

    if y.type == uint {
      helper(result, x, y);
    }
    else {
      var y_ : c_ulong;
      if y >= 0
        then y_ = y.safeCast(c_ulong);
        else y_ = (0 - y).safeCast(c_ulong);
      helper(result, x, y_);
    }

  }


  /*
    Computes the mod operator on the two arguments, defined as
    ``a % b = a - b * trunc(a / b)``.

    The result is always >= 0 if `a` > 0.
    It is an error if `b` == 0.

    See :proc:`~BigInteger.rem`
  */
  operator bigint.%(const ref a: bigint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.modTrunc(c, a, b);
    return c;
  }

  /* See :proc:`bigint.%` */
  operator bigint.%(const ref a: bigint, b: int): bigint {
    var c = new bigint();
    BigInteger.modTrunc(c, a, b);
    return c;
  }

  /* See :proc:`bigint.%` */
  operator bigint.%(const ref a: bigint, b: uint): bigint {
    var c = new bigint();
    BigInteger.modTrunc(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.shiftLeft` */
  operator bigint.<<(const ref a: bigint, b: int): bigint {
    var c = new bigint();
    BigInteger.shiftLeft(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.shiftLeft` */
  operator bigint.<<(const ref a: bigint, b: uint): bigint {
    var c = new bigint();
    BigInteger.shiftLeft(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.shiftRight` */
  operator bigint.>>(const ref a: bigint, b: int): bigint {
    var c = new bigint();
    BigInteger.shiftRight(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.shiftRight` */
  operator bigint.>>(const ref a: bigint, b: uint): bigint {
    var c = new bigint();
    BigInteger.shiftRight(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.and` */
  operator bigint.&(const ref a: bigint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.and(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.or` */
  operator bigint.|(const ref a: bigint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.or(c, a, b);
    return c;
  }

  /* See :proc:`~BigInteger.xor` */
  operator bigint.^(const ref a: bigint, const ref b: bigint): bigint {
    var c = new bigint();
    BigInteger.xor(c, a, b);
    return c;
  }

  private inline proc cmp(const ref x: bigint, const ref y: bigint)
    do return x.cmp(y);

  private inline proc cmp(const ref x: bigint, y: int)
    do return x.cmp(y);
  private inline proc cmp(const ref x: bigint, y: uint)
    do return x.cmp(y);

  private inline proc cmp(x: int, const ref y: bigint)
    do return 0 - (y.cmp(x));
  private inline proc cmp(x: uint, const ref y: bigint)
    do return 0 - (y.cmp(x));


  /* See :proc:`bigint.cmp` */
  operator bigint.==(const ref a: bigint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) == 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.==(const ref a: bigint, b: int): bool
    do return BigInteger.cmp(a, b) == 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.==(const ref a: bigint, b: uint): bool
    do return BigInteger.cmp(a, b) == 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.==(a: int, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) == 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.==(a: uint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) == 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.!=(const ref a: bigint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) != 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.!=(const ref a: bigint, b: int): bool
    do return BigInteger.cmp(a, b) != 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.!=(const ref a: bigint, b: uint): bool
    do return BigInteger.cmp(a, b) != 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.!=(a: int, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) != 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.!=(a: uint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) != 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.>(const ref a: bigint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) > 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.>(const ref a: bigint, b: int): bool
    do return BigInteger.cmp(a, b) > 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.>(const ref a: bigint, b: uint): bool
    do return BigInteger.cmp(a, b) > 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.>(a: int, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) > 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.>(a: uint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) > 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.<(const ref a: bigint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) < 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.<(const ref a: bigint, b: int): bool
    do return BigInteger.cmp(a, b) < 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.<(const ref a: bigint, b: uint): bool
    do return BigInteger.cmp(a, b) < 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.<(a: int, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) < 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.<(a: uint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) < 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.>=(const ref a: bigint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) >= 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.>=(const ref a: bigint, b: int): bool
    do return BigInteger.cmp(a, b) >= 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.>=(const ref a: bigint, b: uint): bool
    do return BigInteger.cmp(a, b) >= 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.>=(a: int, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) >= 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.>=(a: uint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) >= 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.<=(const ref a: bigint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) <= 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.<=(const ref a: bigint, b: int): bool
    do return BigInteger.cmp(a, b) <= 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.<=(const ref a: bigint, b: uint): bool
    do return BigInteger.cmp(a, b) <= 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.<=(a: int, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) <= 0;

  /* See :proc:`bigint.cmp` */
  operator bigint.<=(a: uint, const ref b: bigint): bool
    do return BigInteger.cmp(a, b) <= 0;

  /* See :proc:`~BigInteger.add` */
  operator bigint.+=(ref a: bigint, const ref b: bigint)
    do BigInteger.add(a, a, b);

  /* See :proc:`~BigInteger.add` */
  operator bigint.+=(ref a: bigint, b: int)
    do BigInteger.add(a, a, b);

  /* See :proc:`~BigInteger.add` */
  operator bigint.+=(ref a: bigint, b: uint)
    do BigInteger.add(a, a, b);

  /* See :proc:`~BigInteger.sub` */
  operator bigint.-=(ref a: bigint, const ref b: bigint)
    do BigInteger.sub(a, a, b);

  /* See :proc:`~BigInteger.sub` */
  operator bigint.-=(ref a: bigint, b: int)
    do BigInteger.sub(a, a, b);

  /* See :proc:`~BigInteger.sub` */
  operator bigint.-=(ref a: bigint, b: uint)
    do BigInteger.sub(a, a, b);

  /* See :proc:`~BigInteger.mul` */
  operator bigint.*=(ref a: bigint, const ref b: bigint)
    do BigInteger.mul(a, a, b);

  /* See :proc:`~BigInteger.mul` */
  operator bigint.*=(ref a: bigint, b:  int)
    do BigInteger.mul(a, a, b);

  /* See :proc:`~BigInteger.mul` */
  operator bigint.*=(ref a: bigint, b: uint)
    do BigInteger.mul(a, a, b);

  /* See :proc:`~BigInteger.div` */
  operator bigint./=(ref a: bigint, const ref b: bigint) {
    BigInteger.div(a, a, b, roundingMode.zero);
  }

  /* See :proc:`~BigInteger.div` */
  operator bigint./=(ref a: bigint, b: integral) {
    a /= new bigint(b);
  }

  /* See :proc:`~BigInteger.pow` */
  operator bigint.**=(ref base: bigint, const ref exp: bigint) {
    base = base ** exp;
  }

  /* See :proc:`~BigInteger.pow` */
  operator bigint.**=(ref base: bigint, exp: int) {
    BigInteger.pow(base, base, exp);
  }

  /* See :proc:`~BigInteger.pow` */
  operator bigint.**=(ref base: bigint, exp: uint) {
    BigInteger.pow(base, base, exp);
  }

  /* See :proc:`bigint.%` */
  operator bigint.%=(ref a: bigint, const ref b: bigint)
    do BigInteger.modTrunc(a, a, b);

  /* See :proc:`bigint.%` */
  operator bigint.%=(ref a: bigint, b: int)
    do BigInteger.modTrunc(a, a, b);

  /* See :proc:`bigint.%` */
  operator bigint.%=(ref a: bigint, b: uint)
    do BigInteger.modTrunc(a, a, b);

  /* See :proc:`~BigInteger.and` */
  operator bigint.&=(ref a: bigint, const ref b: bigint) {
    BigInteger.and(a, a, b);
  }

  /* See :proc:`~BigInteger.or` */
  operator bigint.|=(ref a: bigint, const ref b: bigint) {
    BigInteger.or(a, a, b);
  }

  /* See :proc:`~BigInteger.xor` */
  operator bigint.^=(ref a: bigint, const ref b: bigint) {
    BigInteger.xor(a, a, b);
  }

  /* See :proc:`~BigInteger.shiftLeft` */
  operator bigint.<<=(ref a: bigint, b: int)
    do BigInteger.shiftLeft(a, a, b);

  /* See :proc:`~BigInteger.shiftLeft` */
  operator bigint.<<=(ref a: bigint, b: uint)
    do BigInteger.shiftLeft(a, a, b);

  /* See :proc:`~BigInteger.shiftRight` */
  operator bigint.>>=(ref a: bigint, b:  int)
    do BigInteger.shiftRight(a, a, b);

  /* See :proc:`~BigInteger.shiftRight` */
  operator bigint.>>=(ref a: bigint, b: uint)
    do BigInteger.shiftRight(a, a, b);

  /* See :proc:`bigint.swap` */
  operator bigint.<=>(ref a: bigint, ref b: bigint) {
    if compiledForSingleLocale() ||
      (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      var t = a;

      mpz_set(a.mpz, b.mpz);
      mpz_set(b.mpz, t.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_none);
      const bLoc = chpl_buildLocaleID(b.localeId, c_sublocid_none);

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


  /*
    Returns the Jacobi symbol ``a/b``, which is defined only when ``b`` is odd.

    :rtype: ``int``

    .. seealso::
       :proc:`GMP.mpz_jacobi` and
       `mpz_jacobi <https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005fjacobi>`_.
  */
  @unstable("jacobi is unstable and may change in the future")
  proc jacobi(const ref a: bigint, const ref b: bigint) : int {
    const a_ = a.localize();
    const b_ = b.localize();
    var ret : c_int;

    ret = mpz_jacobi(a_.mpz, b_.mpz);

    return ret.safeCast(int);
  }



  /*
    Returns the Legendre symbol ``a/p``, which is defined only when ``p`` is
    an odd positive prime number.

    :rtype: ``int``

    .. seealso::
       :proc:`GMP.mpz_legendre` and
       `mpz_legendre <https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005flegendre>`_.
  */
  @unstable("legendre is unstable and may change in the future")
  proc legendre(const ref a: bigint, const ref p: bigint) : int {
    const a_ = a.localize();
    const p_ = p.localize();
    var ret : c_int;

    ret = mpz_legendre(a_.mpz, p_.mpz);

    return ret.safeCast(int);
  }



  /*
    Returns the Jacobi symbol ``a/b`` with the Kronecker extension. When
    ``b`` is odd this is the same as the Jacobi symbol.

    :rtype: ``int``

    .. seealso::
       :proc:`GMP.mpz_kronecker` and
       `mpz_kronecker <https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005fkronecker>`_.
  */
  @unstable("kronecker is unstable and may change in the future")
  proc kronecker(const ref a: bigint, const ref b: bigint) : int {
    var ret : c_int;

    const a_ = a.localize();
    const b_ = b.localize();

    ret = mpz_kronecker(a_.mpz, b_.mpz);

    return ret.safeCast(int);
  }

  /* See :proc:`kronecker` */
  @unstable("kronecker is unstable and may change in the future")
  proc kronecker(const ref a: bigint, b: int) : int {
    const a_ = a.localize();
    const b_ = b.safeCast(c_long);
    var  ret : c_int;

    ret = mpz_kronecker_si(a_.mpz, b_);

    return ret.safeCast(int);
  }

  /* See :proc:`kronecker` */
  @unstable("kronecker is unstable and may change in the future")
  proc kronecker(a: int, const ref b: bigint) : int {
    const a_ = a.safeCast(c_long);
    const b_ = b.localize();
    var  ret : c_int;

    ret = mpz_si_kronecker(a_, b_.mpz);

    return ret.safeCast(int);
  }

  /* See :proc:`kronecker` */
  @unstable("kronecker is unstable and may change in the future")
  proc kronecker(const ref a: bigint, b: uint) : int {
    const a_ = a.localize();
    const b_ = b.safeCast(c_ulong);
    var  ret : c_int;

    ret = mpz_kronecker_ui(a_.mpz, b_);

    return ret.safeCast(int);
  }

  /* See :proc:`kronecker` */
  @unstable("kronecker is unstable and may change in the future")
  proc kronecker(a: uint, const ref b: bigint) : int {
    const a_ = a.safeCast(c_ulong);
    const b_ = b.localize();
    var  ret : c_int;

    ret = mpz_ui_kronecker(a_, b_.mpz);

    return ret.safeCast(int);
  }

  /*
    Computes ``numer/denom`` and stores the result in ``result``, which is a
    :record:`bigint` instance.

    .. warning::
       ``divExact`` is optimized to handle cases where ``numer/denom`` results
       in an integer.  When ``numer/denom`` does not produce an integer, this
       method may produce incorrect results.

    :arg result: Where the result is stored
    :type result: :record:`bigint`
    :arg numer: numerator
    :type numer: :record:`bigint`
    :arg denom: denominator
    :type denom: :record:`bigint` or ``integral``

    .. seealso::
       :proc:`GMP.mpz_divexact` and
       `mpz_divexact <https://gmplib.org/manual/Integer-Division#index-mpz_005fdivexact>`_.
  */
  proc divExact(ref result: bigint, const ref numer: bigint, const ref denom: bigint) {
    if (chpl_checkDivByZero) then
      if denom == 0 then
        halt("Attempt to divide by zero");

    if compiledForSingleLocale() {
      mpz_divexact(result.mpz, numer.mpz, denom.mpz);
    } else if result.localeId == chpl_nodeID {
      const numer_ = numer;
      const denom_ = denom;
      mpz_divexact(result.mpz, numer_.mpz, denom_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const numer_ = numer;
        const denom_ = denom;
        mpz_divexact(result.mpz, numer_.mpz, denom_.mpz);
      }
    }
  }

  /* See :proc:`~BigInteger.divExact` */
  proc divExact(ref result: bigint, const ref numer: bigint, denom: integral)
    do BigInteger.divExact(result, numer, new bigint(denom));

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

    .. seealso::
       :proc:`GMP.mpz_divisible_p` and
       `mpz_divisible_p <https://gmplib.org/manual/Integer-Division#index-mpz_005fdivisible_005fp>`_.
   */
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

  /* See :proc:`~bigint.isDivisible` */
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

  /* See :proc:`~bigint.isDivisible` */
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
    Return ``true`` if ``this`` is exactly divisible by ``2^exp``.  ``this`` is
    divisible by ``2^exp`` if there exists an integer ``q`` satisfying ``this =
    q*2^exp``.

    :arg exp: power of 2 to check if ``this`` is divisible by
    :type exp: ``integral``

    :return: ``true`` if ``this`` is exactly divisible by ``2^exp``, ``false``
             otherwise
    :rtype: ``bool``

    .. seealso::
       :proc:`GMP.mpz_divisible_2exp_p` and
       `mpz_divisible_2exp_p <https://gmplib.org/manual/Integer-Division#index-mpz_005fdivisible_005f2exp_005fp>`_.
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

    .. seealso::
       :proc:`GMP.mpz_congruent_p` and
       `mpz_congruent_p <https://gmplib.org/manual/Integer-Division#index-mpz_005fcongruent_005fp>`_.
   */
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

  /* See :proc:`~bigint.isCongruent` */
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

    .. seealso::
       :proc:`GMP.mpz_congruent_2exp_p` and
       `mpz_congruent_2exp_p <https://gmplib.org/manual/Integer-Division#index-mpz_005fcongruent_005f2exp_005fp>`_.
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
    Set ``result`` to the result of ``(base**exp) modulo mod``.

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
       ``(1/base) modulo mod`` does not exist.

    .. seealso::
       :proc:`GMP.mpz_powm` and
       `mpz_powm <https://gmplib.org/manual/Integer-Exponentiation#index-mpz_005fpowm>`_.
  */
  proc powMod(ref result: bigint,
              const ref base: bigint,
              const ref exp: bigint,
              const ref mod: bigint)  {
    if compiledForSingleLocale() {
      mpz_powm(result.mpz, base.mpz, exp.mpz, mod.mpz);
    } else if result.localeId == chpl_nodeID {
      const base_ = base;
      const exp_ = exp;
      const mod_ = mod;
      mpz_powm(result.mpz, base_.mpz, exp_.mpz, mod_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const base_ = base;
        const exp_ = exp;
        const mod_  = mod;
        mpz_powm(result.mpz, base_.mpz, exp_.mpz, mod_.mpz);
      }
    }
  }

  /* See :proc:`~BigInteger.powMod` */
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

  /* See :proc:`~BigInteger.powMod` */
  proc powMod(ref result: bigint,
              const ref base: bigint,
              exp: uint,
              const ref mod: bigint)  {
    const exp_ = exp.safeCast(c_ulong);

    if compiledForSingleLocale() {
      mpz_powm_ui(result.mpz, base.mpz, exp_, mod.mpz);
    } else if result.localeId == chpl_nodeID {
      const base_ = base;
      const mod_ = mod;
      mpz_powm_ui(result.mpz, base_.mpz, exp, mod_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const base_ = base;
        const mod_  = mod;
        mpz_powm_ui(result.mpz, base_.mpz, exp_, mod_.mpz);
      }
    }
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

  /*
    Set ``result`` to the result of ``base`` raised to ``exp``.

    :arg result: Where the result is stored
    :type result: :record:`bigint`
    :arg base: The value to be raised to the power of ``exp``.
    :type base: :record:`bigint`, ``int`` or ``uint``
    :arg exp: The exponent to raise ``base`` to the power of.
    :type exp: ``int`` or ``uint``

    .. seealso::
       :proc:`GMP.mpz_pow_ui` and
       `mpz_pow_ui <https://gmplib.org/manual/Integer-Exponentiation#index-mpz_005fpow_005fui>`_.
  */
  proc pow(ref result: bigint, const ref base: bigint, exp: int) {
    if exp >= 0 {
      BigInteger.pow(result, base, exp : uint);
    } else {
      if compiledForSingleLocale() {
        powNegativeExpHelper(result, base, exp);
      } else if result.localeId == chpl_nodeID {
        const base_ = base;
        powNegativeExpHelper(result, base_, exp);
      } else {
        const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
        on __primitive("chpl_on_locale_num", resultLoc) {
          const base_ = base;
          powNegativeExpHelper(result, base_, exp);
        }
      }
    }
  }

  /* See :proc:`~BigInteger.pow` */
  proc pow(ref result: bigint, const ref base: bigint, exp: uint) {
    const exp_ = exp.safeCast(c_ulong);
    if compiledForSingleLocale() {
      mpz_pow_ui(result.mpz, base.mpz, exp_);
    } else if result.localeId == chpl_nodeID {
      const base_ = base;
      mpz_pow_ui(result.mpz, base_.mpz, exp_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const base_ = base;
        mpz_pow_ui(result.mpz, base_.mpz, exp_);
      }
    }
  }

  /* See :proc:`~BigInteger.pow` */
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

  /* See :proc:`~BigInteger.pow` */
  proc pow(ref result: bigint, base: uint, exp: uint) {
    const base_ = base.safeCast(c_ulong);
    const exp_  = exp.safeCast(c_ulong);

    if compiledForSingleLocale() {
      mpz_ui_pow_ui(result.mpz, base_, exp_);
    } else if result.localeId == chpl_nodeID {
      mpz_ui_pow_ui(result.mpz, base_, exp_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        mpz_ui_pow_ui(result.mpz, base_, exp_);
      }
    }
  }

  /*
     Sets ``result`` to the truncated integer ``n`` th root of ``x``.

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg x: Number to take the root of
     :type x: :record:`bigint`
     :arg n: Which root to take
     :type n: ``uint``

     .. seealso::
        :proc:`GMP.mpz_root` and
        `mpz_root <https://gmplib.org/manual/Integer-Roots#index-mpz_005froot>`_.
  */
  proc root(ref result: bigint, const ref x: bigint, n: uint) : int {
    const n_ = n.safeCast(c_ulong);
    var   ret: c_int;

    if compiledForSingleLocale() {
      ret = mpz_root(result.mpz, x.mpz, n_);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      ret = mpz_root(result.mpz, x_.mpz, n_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        ret = mpz_root(result.mpz, x_.mpz, n_);
      }
    }
    return ret.safeCast(int);
  }

  /* Sets ``result`` to the truncated integer ``n`` th root of ``x``. Stores
     the remainder in ``remain``.

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg remain: Where the remainder is stored
     :type remain: :record:`bigint`
     :arg x: Number to take the root of
     :type x: :record:`bigint`
     :arg n: Which root to take
     :type n: ``uint``

     .. seealso::
        :proc:`GMP.mpz_rootrem` and
        `mpz_rootrem <https://gmplib.org/manual/Integer-Roots#index-mpz_005frootrem>`_.
  */
  proc rootRem(ref result: bigint, ref remain: bigint, const ref x: bigint, n: uint) {
    const n_  = n.safeCast(c_ulong);

    if compiledForSingleLocale() {
      mpz_rootrem(result.mpz, remain.mpz, x.mpz, n_);
    } else if result.localeId == chpl_nodeID {
      var remain_: bigint;
      const x_ = x;
      mpz_rootrem(result.mpz, remain_.mpz, x_.mpz, n_);
      remain = remain_;
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        var remain_: bigint;
        const x_ = x;
        mpz_rootrem(result.mpz, remain_.mpz, x_.mpz, n_);
        remain = remain_;
      }
    }
  }

  /*
     Sets ``result`` to the truncated integer square root of ``x``.

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg x: Number to take the square root of
     :type x: :record:`bigint`

     .. seealso::
        :proc:`GMP.mpz_sqrt` and
        `mpz_sqrt <https://gmplib.org/manual/Integer-Roots#index-mpz_005fsqrt>`_.
  */
  proc sqrt(ref result: bigint, const ref x: bigint) {
    if compiledForSingleLocale() {
      mpz_sqrt(result.mpz, x.mpz);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      mpz_sqrt(result.mpz, x_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        mpz_sqrt(result.mpz, x_.mpz);
      }
    }
  }

  /* Sets ``result`` to the truncated integer square root of ``x``. Stores
     the remainder in ``remain``.

     .. warning::
        If ``result`` is also passed as the ``remain`` argument, the program
        behavior is undefined.

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg remain: Where the remainder is stored
     :type remain: :record:`bigint`
     :arg x: Number to take the square root of
     :type x: :record:`bigint`

     .. seealso::
        :proc:`GMP.mpz_sqrtrem` and
        `mpz_sqrtrem <https://gmplib.org/manual/Integer-Roots#index-mpz_005fsqrtrem>`_.
  */
  proc sqrtRem(ref result: bigint, ref remain: bigint, const ref x: bigint) {
    if compiledForSingleLocale() {
      mpz_sqrtrem(result.mpz, remain.mpz, x.mpz);
    } else if result.localeId == chpl_nodeID {
      var remain_ : bigint;
      const x_ = x;
      mpz_sqrtrem(result.mpz, remain_.mpz, x_.mpz);
      remain = remain_;
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        var remain_ : bigint;
        const x_ = x;
        mpz_sqrtrem(result.mpz, remain_.mpz, x_.mpz);
        remain = remain_;
      }
    }
  }

  /*
    Return ``true`` if ``this`` is a perfect power, i.e., if there exist
    integers ``a`` and ``b`` with ``b > 1``, such that ``this = a^b``.

    Under this definition both 0 and 1 are considered to be perfect powers.
    Negative values can only be odd perfect powers.

    :return: ``true`` if ``this`` is a perfect power, ``false`` otherwise.
    :rtype: ``bool``

    .. seealso::
       :proc:`GMP.mpz_perfect_power_p` and
       `mpz_perfect_power_p <https://gmplib.org/manual/Integer-Roots#index-mpz_005fperfect_005fpower_005fp>`_.
  */
  proc bigint.isPerfectPower() : bool {
    var t_ = this.localize();
    var ret: c_int;

    ret = mpz_perfect_power_p(t_.mpz);

    if ret then
      return true;
    else
      return false;
  }

  /*
    Return ``true`` if ``this`` is a perfect square, i.e., if the square root of
    ``this`` is an integer.

    Under this definition both ``0`` and ``1`` are considered to be perfect squares.

    :return: ``true`` if ``this`` is a perfect square, ``false`` otherwise.
    :rtype: ``bool``

    .. seealso::
       :proc:`GMP.mpz_perfect_square_p` and
       `mpz_perfect_square_p <https://gmplib.org/manual/Integer-Roots#index-mpz_005fperfect_005fsquare_005fp>`_.
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
    An enumeration of the different possibilities of a number being prime, for
    use with e.g. :proc:`~bigint.probablyPrime` to determine if a number is
    prime or not.
   */
  enum primality {
    /* Indicates that the number is not a prime. */
    notPrime=0,
    /* Indicates that the number may or may not be a prime. */
    maybePrime,
    /* Indicates that the number is a prime. */
    isPrime
  }

  /*
    Determine whether ``this`` is prime.  Returns one of the :enum:`primality`
    constants - :enumconstant:`~primality.isPrime`,
    :enumconstant:`~primality.maybePrime`, or
    :enumconstant:`~primality.notPrime`.

    Performs some trial divisions, a Baillie-PSW probable prime test, and
    reps-24 Miller-Rabin probabilistic primality tests.  A higher ``reps`` value
    will reduce the chances of a non-prime being identified as "probably prime".
    A composite number will be identified as a prime with an asymptotic
    probability of less than ``4^(-reps)``.  Reasonable values of ``reps`` are
    between 15 and 50.

    :arg reps: number of attempts before returning
               :enumconstant:`~primality.maybePrime` if a definitive answer
               can't be found before then.
    :type reps: ``int``
    :returns: :enumconstant:`~primality.isPrime`,
              :enumconstant:`~primality.maybePrime`, or
              :enumconstant:`~primality.notPrime`.
    :rtype: :enum:`primality`

    .. seealso::
       :proc:`GMP.mpz_probab_prime_p` and
       `mpz_probab_prime_p <https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005fprobab_005fprime_005fp>`_.
   */
  @unstable("bigint.probablyPrime is unstable and may change in the future")
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

  /* Set ``result`` to the next prime number greater than ``x``.

     .. note::
        This is a probabilistic function and in an unlikely case may set
        ``result`` to a composite number.

     :arg result: return value that will contain the next prime number
     :type result: :record:`bigint`
     :arg x: the ``result`` will be a prime number bigger than this value
     :type x: :record:`bigint`

     .. seealso::
        :proc:`GMP.mpz_nextprime` and
        `mpz_nextprime <https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005fnextprime>`_.
  */
  @unstable("nextPrime is unstable and may change in the future")
  proc nextPrime(ref result: bigint, const ref x: bigint) {
    if compiledForSingleLocale() {
      mpz_nextprime(result.mpz, x.mpz);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      mpz_nextprime(result.mpz, x_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        mpz_nextprime(result.mpz, x_.mpz);
      }
    }
  }

  /* Set ``result`` to the greatest common divisor of ``a`` and ``b``

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg a: One of the numbers to compute the greatest common divisor of
     :type a: :record:`bigint`
     :arg b: One of the numbers to compute the greatest common divisor of
     :type b: :record:`bigint`, ``int``, ``uint``

     .. seealso::
        :proc:`GMP.mpz_gcd` and
        `mpz_gcd <https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005fgcd>`_.
  */
  @unstable("gcd is unstable and may change in the future")
  proc gcd(ref result: bigint, const ref a: bigint, const ref b: bigint) {
    if compiledForSingleLocale() {
      mpz_gcd(result.mpz, a.mpz, b.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      const b_ = b;
      mpz_gcd(result.mpz, a_.mpz, b_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        const b_ = b;
        mpz_gcd(result.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  /* See :proc:`gcd` */
  @unstable("gcd is unstable and may change in the future")
  proc gcd(ref result: bigint, const ref a: bigint, b: int) {
    if b >= 0 {
      BigInteger.gcd(result, a, b : uint);

    } else {
      BigInteger.gcd(result, a, new bigint(b));
    }
  }

  /* See :proc:`gcd` */
  @unstable("gcd is unstable and may change in the future")
  proc gcd(ref result: bigint, const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);
    if compiledForSingleLocale() {
      mpz_gcd_ui(result.mpz, a.mpz, b_);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      mpz_gcd_ui(result.mpz, a_.mpz, b_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        mpz_gcd_ui(result.mpz, a_.mpz, b_);
      }
    }
  }

  /* Set ``result`` to the greatest common divisor of ``a`` and ``b``, and
     set ``s`` and ``t`` to coefficients such that ``a*s + b*t == result``.

     .. note::
        The result stored in ``result`` is always positive, even if one or
        both of ``a`` and ``b`` are negative (or zero if both are zero).

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

     .. seealso::
        :proc:`GMP.mpz_gcdext` and
        `mpz_gcdext <https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005fgcdext>`_.
   */
  @unstable("gcd is unstable and may change in the future")
  proc gcd(ref result: bigint, const ref a: bigint, const ref b: bigint,
                  ref s: bigint, ref t: bigint): void {
    if compiledForSingleLocale() {
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
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
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

  /* Set ``result`` to the least common multiple of ``a`` and ``b``

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg a: One of the numbers to compute the least common multiple of
     :type a: :record:`bigint`
     :arg b: One of the numbers to compute the least common multiple of
     :type b: :record:`bigint`, ``int``, ``uint``

     .. seealso::
        :proc:`GMP.mpz_lcm` and
        `mpz_lcm <https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005flcm>`_.
  */
  @unstable("lcm is unstable and may change in the future")
  proc lcm(ref result: bigint, const ref a: bigint, const ref b: bigint) {
    if compiledForSingleLocale() {
      mpz_lcm(result.mpz, a.mpz, b.mpz);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      const b_ = b;
      mpz_lcm(result.mpz, a_.mpz, b_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        const b_ = b;
        mpz_lcm(result.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  /* See :proc:`lcm` */
  @unstable("lcm is unstable and may change in the future")
  proc lcm(ref result: bigint, const ref a: bigint, b: int) {
    if b >= 0 then
      BigInteger.lcm(result, a, b:uint);
    else
      BigInteger.lcm(result, a, (0 - b):uint);
  }

  /* See :proc:`lcm` */
  @unstable("lcm is unstable and may change in the future")
  proc lcm(ref result: bigint, const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if compiledForSingleLocale() {
      mpz_lcm_ui(result.mpz, a.mpz, b_);
    } else if result.localeId == chpl_nodeID {
      const a_ = a;
      mpz_lcm_ui(result.mpz, a_.mpz, b_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const a_ = a;
        mpz_lcm_ui(result.mpz, a_.mpz, b_);
      }
    }
  }

  /*
   An :class:`InversionError` is thrown if a :proc:`invert()` is attempted with
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

  /*
     Set the value of ``result`` to the inverse of ``x`` modulo ``y``

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg x: The dividend of the modulo operation
     :type x: :record:`bigint`
     :arg y: The divisor of the modulo operation
     :type y: :record:`bigint`
     :throws InversionError: Thrown when the inverse does not exist and the value of ``result`` will be left undefined.

     .. seealso::
        :proc:`GMP.mpz_invert` and
        `mpz_invert <https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005finvert>`_.
  */
  proc invert(ref result: bigint, const ref x: bigint, const ref y: bigint) throws {
    var ret: c_int;
    if compiledForSingleLocale() {
      ret = mpz_invert(result.mpz, x.mpz, y.mpz);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      const y_ = y;
      ret = mpz_invert(result.mpz, x_.mpz, y_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        const y_ = y;
        ret = mpz_invert(result.mpz, x_.mpz, y_.mpz);
      }
    }

    if (ret.safeCast(int) == 0) {
      throw new owned InversionError();
    }
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

    .. seealso::
       :proc:`GMP.mpz_remove` and
        `mpz_remove <https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005fremove>`_.
   */
  proc removeFactor(ref result: bigint, const ref x: bigint, const ref fac: bigint) : uint {
    var ret: c_ulong;
    if(fac!=0){
      if compiledForSingleLocale() {
        ret = mpz_remove(result.mpz, x.mpz, fac.mpz);
      } else if result.localeId == chpl_nodeID {
          const x_ = x;
          const fac_ = fac;
          ret = mpz_remove(result.mpz, x_.mpz, fac_.mpz);
      } else {
        const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
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
    Set ``result`` to the factorial of ``a``.

    :arg result: Where the result is stored
    :type result: :record:`bigint`
    :arg a: Number to take the factorial of
    :type a: ``integral``

    .. seealso::
       :proc:`GMP.mpz_fac_ui` and
       `mpz_fac_ui <https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005ffac_005fui>`_.
  */
  @unstable("fac is unstable and may change in the future")
  proc fac(ref result: bigint, a: integral) {
    const a_ = a.safeCast(c_ulong);
    if compiledForSingleLocale() {
      mpz_fac_ui(result.mpz, a_);
    } else if result.localeId == chpl_nodeID {
      mpz_fac_ui(result.mpz, a_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        mpz_fac_ui(result.mpz, a_);
      }
    }
  }

  /*
    Set ``result`` to the binomial coefficient of ``n`` over ``k``.

    :arg result: Where the result is stored
    :type result: :record:`bigint`
    :arg n: Top number of the binomial
    :type n: :record:`bigint` or ``uint``
    :arg k: Bottom number of the binomial
    :type k: ``integral``

    .. seealso::
       :proc:`GMP.mpz_bin_ui` and
       `mpz_bin_ui <https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005fbin_005fui>`_.
  */
  @unstable("bin is unstable and may change in the future")
  proc bin(ref result: bigint, const ref n: bigint, k: integral) {
    const k_ = k.safeCast(c_ulong);
    if compiledForSingleLocale() {
      mpz_bin_ui(result.mpz, n.mpz, k_);
    } else if result.localeId == chpl_nodeID {
      const n_ = n;
      mpz_bin_ui(result.mpz, n_.mpz, k_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const n_ = n;
        mpz_bin_ui(result.mpz, n_.mpz, k_);
      }
    }
  }

  /* See :proc:`bin` */
  @unstable("bin is unstable and may change in the future")
  proc bin(ref result: bigint, n: uint, k: integral) {
    if n >= 0 {
      const n_ = n.safeCast(c_ulong);
      const k_ = k.safeCast(c_ulong);

      if compiledForSingleLocale() || result.localeId == chpl_nodeID {
        mpz_bin_uiui(result.mpz, n_, k_);
      } else {
        const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
        on __primitive("chpl_on_locale_num", resultLoc) {
          mpz_bin_uiui(result.mpz, n_, k_);
        }
      }
    } else {
      BigInteger.bin(result, new bigint(n), k);
    }
  }

  /*
    Set ``result`` to the ``n`` th Fibonacci number.

    :arg result: return value that will contain the Fibonacci number
    :type result: :record:`bigint`
    :arg n: which Fibonacci number to compute for ``result``.
    :type n: ``integral``

    .. seealso::
       :proc:`GMP.mpz_fib_ui` and
       `mpz_fib_ui <https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005ffib_005fui>`_.
  */
  @unstable("fib is unstable and may change in the future")
  proc fib(ref result: bigint, n: integral) {
    const n_ = n.safeCast(c_ulong);

    if compiledForSingleLocale() || result.localeId == chpl_nodeID {
      mpz_fib_ui(result.mpz, n_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        mpz_fib_ui(result.mpz, n_);
      }
    }
  }

  /*
    Set ``result`` to the ``n`` th Fibonacci number and set ``fnsub1`` to the
    ``n-1`` th Fibonacci number.

    :arg result: return value that will contain the Fibonacci number
    :type result: :record:`bigint`
    :arg fnsub1: return value that will contain the previous Fibonacci number
    :type fnsub1: :record:`bigint`
    :arg n: which Fibonacci number to compute for ``result``. ``fnsub1`` is set
            to the ``n-1`` Fibonacci number.
    :type n: ``integral``

    .. seealso::
       :proc:`GMP.mpz_fib2_ui` and
       `mpz_fib2_ui <https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005ffib2_005fui>`_.
  */
  @unstable("fib2 is unstable and may change in the future")
  proc fib2(ref result: bigint, ref fnsub1: bigint, n: integral) {
    const n_ = n.safeCast(c_ulong);

    if compiledForSingleLocale() {
      mpz_fib2_ui(result.mpz, fnsub1.mpz, n_);
    } else if result.localeId == chpl_nodeID {
      // TODO: need to revisit this in relation to Cray/chapel-private#4628
      var fnsub1_ : bigint;
      mpz_fib2_ui(result.mpz, fnsub1_.mpz, n_);
      fnsub1 = fnsub1_;
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        var fnsub1_ : bigint;
        mpz_fib2_ui(result.mpz, fnsub1_.mpz, n_);
        fnsub1 = fnsub1_;
      }
    }
  }

  /*
    Set ``result`` to the ``n`` th Lucas number.

    :arg result: return value that will contain the Lucas number
    :type result: :record:`bigint`
    :arg n: which Lucas number to compute
    :type n: ``integral``

    .. seealso::
       :proc:`GMP.mpz_lucnum_ui` and
       `mpz_lucnum_ui <https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005flucnum_005fui>`_.
  */
  @unstable("lucNum is unstable and may change in the future")
  proc lucNum(ref result: bigint, n: integral) {
    const n_ = n.safeCast(c_ulong);
    if compiledForSingleLocale() || result.localeId == chpl_nodeID {
      mpz_lucnum_ui(result.mpz, n_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        mpz_lucnum_ui(result.mpz, n_);
      }
    }
  }

  /*
    Set ``result`` to the ``n`` th Lucas number and set ``fnsub1`` to the
    ``n-1`` th Lucas number.

    :arg result: return value that will contain the Lucas number
    :type result: :record:`bigint`
    :arg fnsub1: return value that will contain the previous Lucas number
    :type fnsub1: :record:`bigint`
    :arg n: which Lucas number to compute for ``result``. ``fnsub1`` is set to
            the ``n-1`` Lucas number.
    :type n: ``integral``

    .. seealso::
       :proc:`GMP.mpz_lucnum2_ui` and
       `mpz_lucnum2_ui <https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005flucnum2_005fui>`_.
  */
  @unstable("lucNum2 is unstable and may change in the future")
  proc lucNum2(ref result: bigint, ref fnsub1: bigint, n: integral) {
    const n_ = n.safeCast(c_ulong);

    if compiledForSingleLocale() {
      mpz_lucnum2_ui(result.mpz, fnsub1.mpz, n_);
    } else if result.localeId == chpl_nodeID {
      // TODO: need to revisit this in relation to Cray/chapel-private#4628
      var fnsub1_ : bigint;
      mpz_lucnum2_ui(result.mpz, fnsub1_.mpz, n_);
      fnsub1 = fnsub1_;
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        var fnsub1_ : bigint;
        mpz_lucnum2_ui(result.mpz, fnsub1_.mpz, n_);
        fnsub1 = fnsub1_;
      }
    }
  }

  /*
    Returns the number of ``1`` bits in ``this``. If ``this`` is negative, the
    number of ``1`` bits is infinite and the return value is the largest
    possible :type:`~GMP.mp_bitcnt_t`.

    :returns: The number of ``1`` bits in ``this``
    :rtype: ``uint``

    .. seealso::
       :proc:`GMP.mpz_popcount` and
       `mpz_popcount <https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling#index-mpz_005fpopcount>`_.
  */
  proc bigint.popCount() : uint {
    const t_ = this.localize();
    var ret: c_ulong;

    ret = mpz_popcount(t_.mpz);

    return ret.safeCast(uint);
  }

  /*
    Returns the number of bit positions that differ between ``this`` and ``x``.
    If ``this`` and ``x`` have different signs, the number of bits that differ
    is infinite and the return value is the largest possible
    :type:`~GMP.mp_bitcnt_t`.

    :arg x: value to compare ``this`` against
    :type x: :record:`bigint`
    :returns: The number of bits that differ
    :rtype: ``uint``

    .. seealso::
       :proc:`GMP.mpz_hamdist` and
       `mpz_hamdist <https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling#index-mpz_005fhamdist>`_.
  */
  @unstable("bigint.hammingDistance is unstable and may change in the future")
  proc bigint.hammingDistance(const ref x: bigint): uint {
    const t_ = this.localize();
    const x_ = x.localize();
    var ret: c_ulong;

    ret = mpz_hamdist(t_.mpz, x_.mpz);

    return ret.safeCast(uint);
  }

  /*
    Returns the index of the first ``0`` bit found, starting from
    ``startBitIdx`` and searching towards the more significant bits.

    If the bit at ``startBitIdx`` is ``1``, will return ``startBitIdx``.

    :arg startBitIdx: The index of the first bit to start searching for a ``0``
    :type startBitIdx: ``integral``
    :returns: The index of the first ``0`` bit after ``startBitIdx``, inclusive
    :rtype: ``uint``

    .. seealso::
       :proc:`GMP.mpz_scan0` and
       `mpz_scan0 <https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling#index-mpz_005fscan0>`_.
  */
  proc bigint.findNext0(startBitIdx: integral): uint {
    const t_ = this.localize();
    const sb_ = startBitIdx.safeCast(c_ulong);
    var   ret: c_ulong;

    ret = mpz_scan0(t_.mpz, sb_);

    return ret.safeCast(uint);
  }

  /*
    Returns the index of the first ``1`` bit found, starting from
    ``startBitIdx`` and searching towards the more significant bits.

    If the bit at ``startBitIdx`` is ``1``, will return ``startBitIdx``.

    :arg startBitIdx: The index of the first bit to start searching for a ``1``
    :type startBitIdx: ``integral``
    :returns: The index of the first ``1`` bit after ``startBitIdx``, inclusive
    :rtype: ``uint``

    .. seealso::
       :proc:`GMP.mpz_scan1` and
       `mpz_scan1 <https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling#index-mpz_005fscan1>`_.
  */
  proc bigint.findNext1(startBitIdx: integral): uint {
    const t_ = this.localize();
    const sb_ = startBitIdx.safeCast(c_ulong);
    var   ret: c_ulong;

    ret = mpz_scan1(t_.mpz,   sb_);

    return ret.safeCast(uint);
  }

  /*
    Set the bit at ``idx`` of ``this``.

    :arg idx: The index of the bit to be set
    :type idx: ``integral``

    .. seealso::
       :proc:`GMP.mpz_setbit` and
       `mpz_setbit <https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling#index-mpz_005fsetbit>`_.
  */
  proc ref bigint.setBit(idx: integral) {
    const bi_ = idx.safeCast(c_ulong);

    if compiledForSingleLocale() || this.localeId == chpl_nodeID {
      mpz_setbit(this.mpz, bi_);
    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_none);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_setbit(this.mpz, bi_);
      }
    }
  }

  /*
    Clear the bit at ``idx`` of ``this``.

    :arg idx: The index of the bit to be cleared
    :type idx: ``integral``

    .. seealso::
       :proc:`GMP.mpz_clrbit` and
       `mpz_clrbit <https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling#index-mpz_005fclrbit>`_.
  */
  proc ref bigint.clearBit(idx: integral) {
    const bi_ = idx.safeCast(c_ulong);

    if compiledForSingleLocale() || this.localeId == chpl_nodeID {
      mpz_clrbit(this.mpz, bi_);
    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_none);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_clrbit(this.mpz, bi_);
      }
    }
  }

  /*
    Toggle the bit at ``idx`` of ``this``. If the bit was 1, set it to 0. If
    the bit was 0, set it to 1.

    :arg idx: The index of the bit to be toggled
    :type idx: ``integral``

    .. seealso::
       :proc:`GMP.mpz_combit` and
       `mpz_combit <https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling#index-mpz_005fcombit>`_.
  */
  proc ref bigint.toggleBit(idx: integral) {
    const bi_ = idx.safeCast(c_ulong);

    if compiledForSingleLocale() || this.localeId == chpl_nodeID {
      mpz_combit(this.mpz, bi_);
    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_none);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_combit(this.mpz, bi_);
      }
    }
  }

  /*
    Get the bit at ``idx`` of ``this``.

    :arg idx: The index of the bit to be returned
    :type idx: ``integral``
    :returns: The bit at index ``idx``
    :rtype: ``int``

    .. seealso::
       :proc:`GMP.mpz_tstbit` and
       `mpz_tstbit <https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling#index-mpz_005ftstbit>`_.
  */
  proc bigint.getBit(idx: integral): int {
    var t_ = this.localize();
    const bi_ = idx.safeCast(c_ulong);
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

  /*
    Test whether a :record:`bigint` will fit into one of the standard integer types.

    :arg t: The Integral type to check against.
    :type t: ``integral``
    :rtype: ``bool``

    .. seealso::
       `mpz_fits_* <https://gmplib.org/manual/Miscellaneous-Integer-Functions>`_.
  */
  proc bigint.fitsInto(type t: integral): bool {
    var t_ = this.localize();
    return fits_into(t_.mpz, t);
  }

  /*
    Returns ``true`` if ``this`` is an even number, ``false`` otherwise.

    :rtype: ``bool``

    .. seealso::
       :proc:`GMP.mpz_even_p` and
       `mpz_even_p <https://gmplib.org/manual/Miscellaneous-Integer-Functions#index-mpz_005feven_005fp>`_.
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
    Returns ``true`` if ``this`` is an odd number, ``false`` otherwise.

    :rtype: ``bool``

    .. seealso::
       :proc:`GMP.mpz_odd_p` and
       `mpz_odd_p <https://gmplib.org/manual/Miscellaneous-Integer-Functions#index-mpz_005fodd_005fp>`_.
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

  /*
     Sets ``result`` to the sum of ``x`` and ``y``.

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg x: The first operand of the sum
     :type x: :record:`bigint`
     :arg y: The second operand of the sum
     :type y: :record:`bigint`, ``uint``, ``int``

     .. seealso::
        :proc:`GMP.mpz_add`,
        :proc:`GMP.mpz_add_ui`, and
        `mpz_add <https://gmplib.org/manual/Integer-Arithmetic#index-mpz_005fadd>`_.
  */
  proc add(ref result: bigint, const ref x: bigint, const ref y: bigint) {
    if compiledForSingleLocale() {
      mpz_add(result.mpz, x.mpz, y.mpz);
    }
    else if result.localeId == chpl_nodeID {
      const x_ = x;
      const y_ = y;
      mpz_add(result.mpz, x_.mpz, y_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        const y_ = y;
        mpz_add(result.mpz, x_.mpz, y_.mpz);
      }
    }
  }

  /* See :proc:`add` */
  proc add(ref result: bigint, const ref x: bigint, y: int) {
    if y >= 0 {
      BigInteger.add(result, x, y:uint);
    }
    else {
      const y_ = (0 - y).safeCast(c_ulong);

      if compiledForSingleLocale() {
        mpz_sub_ui(result.mpz, x.mpz, y_);
      }
      else if result.localeId == chpl_nodeID {
        const x_ = x;
        mpz_sub_ui(result.mpz, x_.mpz, y_);
      }
      else {
        const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
        on __primitive("chpl_on_locale_num", resultLoc) {
          const x_ = x;
          mpz_sub_ui(result.mpz, x_.mpz, y_);
        }
      }
    }
  }

  /* See :proc:`add` */
  proc add(ref result: bigint, const ref x: bigint, y: uint) {
    const y_ = y.safeCast(c_ulong);
    if compiledForSingleLocale() {
      mpz_add_ui(result.mpz, x.mpz, y_);
    }
    else if result.localeId == chpl_nodeID {
      const x_ = x;
      mpz_add_ui(result.mpz, x_.mpz, y_);
    }
    else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        mpz_add_ui(result.mpz, x_.mpz, y_);
      }
    }
  }

  /*
     Sets ``result`` to the difference of ``x`` and ``y``.

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg x: The first operand of the difference
     :type x: :record:`bigint`, ``uint``, ``int``
     :arg y: The second operand of the difference
     :type y: :record:`bigint`, ``uint``, ``int``

     .. seealso::
        :proc:`GMP.mpz_sub`,
        :proc:`GMP.mpz_sub_ui`, and
        `mpz_sub <https://gmplib.org/manual/Integer-Arithmetic#index-mpz_005fsub>`_.
  */
  proc sub(ref result: bigint, const ref x: bigint, const ref y: bigint) {
    if compiledForSingleLocale() {
      mpz_sub(result.mpz, x.mpz, y.mpz);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      const y_ = y;
      mpz_sub(result.mpz, x_.mpz, y_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        const y_ = y;
        mpz_sub(result.mpz, x_.mpz, y_.mpz);
      }
    }
  }

  /* See :proc:`sub` */
  proc sub(ref result: bigint, const ref x: bigint, y: int) {
    if y >= 0 {
      BigInteger.sub(result, x, y:uint);
    } else {
      BigInteger.add(result, x, (0 - y):uint);
    }
  }

  /* See :proc:`sub` */
  proc sub(ref result:bigint, const ref x: bigint, y: uint) {
    const y_ = y.safeCast(c_ulong);

    if compiledForSingleLocale() {
      mpz_sub_ui(result.mpz, x.mpz, y_);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      mpz_sub_ui(result.mpz, x_.mpz, y_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        mpz_sub_ui(result.mpz, x_.mpz, y_);
      }
    }
  }

  /* See :proc:`sub` */
  proc sub(ref result: bigint, x: int, const ref y: bigint) {
    if x >= 0 {
      BigInteger.sub(result, x:uint, y);
    } else {
      const x_ = (0 - x).safeCast(c_ulong);

      if compiledForSingleLocale() {
        mpz_add_ui(result.mpz, y.mpz, x_);
        mpz_neg(result.mpz, result.mpz);
      } else if result.localeId == chpl_nodeID {
        const y_ = y;
        mpz_add_ui(result.mpz, y_.mpz, x_);
        mpz_neg(result.mpz, result.mpz);
      } else {
        const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
        on __primitive("chpl_on_locale_num", resultLoc) {
          const y_ = y;
          mpz_add_ui(result.mpz, y_.mpz, x_);
          mpz_neg(result.mpz, result.mpz);
        }
      }
    }
  }

  /* See :proc:`sub` */
  proc sub(ref result: bigint, x: uint, const ref y: bigint) {
    const x_ = x.safeCast(c_ulong);

    if compiledForSingleLocale() {
      mpz_ui_sub(result.mpz, x_, y.mpz);
    } else if result.localeId == chpl_nodeID {
      const y_ = y;
      mpz_ui_sub(result.mpz, x_, y_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const y_ = y;
        mpz_ui_sub(result.mpz, x_, y_.mpz);
      }
    }
  }

  /*
     Sets ``result`` to the product of ``x`` and ``y``.

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg x: The first operand of the product
     :type x: :record:`bigint`
     :arg y: The second operand of the product
     :type y: :record:`bigint`, ``uint``, ``int``

     .. seealso::
        :proc:`GMP.mpz_mul`,
        :proc:`GMP.mpz_mul_ui`,
        :proc:`GMP.mpz_mul_si`, and
        `mpz_mul <https://gmplib.org/manual/Integer-Arithmetic#index-mpz_005fmul>`_.
  */
  proc mul(ref result: bigint, const ref x: bigint, const ref y: bigint) {
    if compiledForSingleLocale() {
      mpz_mul(result.mpz, x.mpz, y.mpz);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      const y_ = y;
      mpz_mul(result.mpz, x_.mpz, y_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        const y_ = y;
        mpz_mul(result.mpz, x_.mpz, y_.mpz);
      }
    }
  }

  /* See :proc:`mul` */
  proc mul(ref result: bigint, const ref x: bigint, y: int) {
    const y_ = y.safeCast(c_long);

    if compiledForSingleLocale() {
      mpz_mul_si(result.mpz, x.mpz, y_);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      mpz_mul_si(result.mpz, x_.mpz, y_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        mpz_mul_si(result.mpz, x_.mpz, y_);
      }
    }
  }

  /* See :proc:`mul` */
  proc mul(ref result: bigint, const ref x: bigint, y: uint) {
    const y_ = y.safeCast(c_ulong);

    if compiledForSingleLocale() {
      mpz_mul_ui(result.mpz, x.mpz, y_);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      mpz_mul_ui(result.mpz, x_.mpz, y_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        mpz_mul_ui(result.mpz, x_.mpz, y_);
      }
    }
  }

  /*
     Adds the product of ``x`` and ``y`` to ``result``
     (``result = result + (x * y)``).

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg x: The first operand of the product
     :type x: :record:`bigint`
     :arg y: The second operand of the product
     :type y: :record:`bigint`, ``uint``, ``int``

     .. seealso::
        :proc:`GMP.mpz_addmul`,
        :proc:`GMP.mpz_addmul_ui`, and
        `mpz_addmul <https://gmplib.org/manual/Integer-Arithmetic#index-mpz_005faddmul>`_.
  */
  proc addMul(ref result: bigint, const ref x: bigint, const ref y: bigint) {
    if compiledForSingleLocale() {
      mpz_addmul(result.mpz, x.mpz, y.mpz);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      const y_ = y;
      mpz_addmul(result.mpz, x_.mpz, y_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        const y_ = y;
        mpz_addmul(result.mpz, x_.mpz, y_.mpz);
      }
    }
  }

  /* See :proc:`addMul` */
  proc addMul(ref result: bigint, const ref x: bigint, y: int) {
    if y >= 0
      then addMul(result, x, y:uint);
      else subMul(result, x, (0 - y):uint);
  }

  /* See :proc:`addMul` */
  proc addMul(ref result: bigint, const ref x: bigint, y: uint) {
    const y_ = y.safeCast(c_ulong);

    if compiledForSingleLocale() {
      mpz_addmul_ui(result.mpz, x.mpz, y_);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      mpz_addmul_ui(result.mpz, x_.mpz, y_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        mpz_addmul_ui(result.mpz, x_.mpz, y_);
      }
    }
  }

  /*
     Subtracts the product of ``x`` and ``y`` from ``result``
     (``result = result - (x * y)``).

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg x: The first operand of the product
     :type x: :record:`bigint`
     :arg y: The second operand of the product
     :type y: :record:`bigint`, ``uint``, ``int``

     .. seealso::
        :proc:`GMP.mpz_submul`,
        :proc:`GMP.mpz_submul_ui`, and
        `mpz_submul <https://gmplib.org/manual/Integer-Arithmetic#index-mpz_005fsub>`_.
  */
  proc subMul(ref result: bigint, const ref x: bigint, const ref y: bigint) {
    if compiledForSingleLocale() {
      mpz_submul(result.mpz, x.mpz, y.mpz);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      const y_ = y;
      mpz_submul(result.mpz, x_.mpz, y_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        const y_ = y;
        mpz_submul(result.mpz, x_.mpz, y_.mpz);
      }
    }
  }

  /* See :proc:`addMul` */
  proc subMul(ref result: bigint, const ref x: bigint, y: int) {
    if y >= 0
      then subMul(result, x, y:uint);
      else addMul(result, x, (0 - y):uint);
  }

  /* See :proc:`addMul` */
  proc subMul(ref result: bigint, const ref x: bigint, y: uint) {
    const y_ = y.safeCast(c_ulong);

    if compiledForSingleLocale() {
      mpz_submul_ui(result.mpz, x.mpz, y_);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      mpz_submul_ui(result.mpz, x_.mpz, y_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        mpz_submul_ui(result.mpz, x_.mpz, y_);
      }
    }
  }

  /*
    Computes ``x*(2**exp)`` and stores the result in ``result``.

    This is the same as performing a left bit shift of ``x`` by ``exp`` bits.

    :arg result: Where the result is stored
    :type result: :record:`bigint`
    :arg x: The number to be multiplied
    :type x: :record:`bigint`
    :arg exp: The exponent that 2 should be raised to before being used
    :type exp: ``integral``

    .. seealso::
       :proc:`GMP.mpz_mul_2exp` and
       `mpz_mul_2exp <https://gmplib.org/manual/Integer-Arithmetic#index-mpz_005fmul_005f2exp>`_.
  */
  @unstable("mul2Exp is unstable and may change in the future")
  proc mul2Exp(ref result: bigint, const ref x: bigint, exp: integral) {
    const exp_ = exp.safeCast(mp_bitcnt_t);

    if compiledForSingleLocale() {
      mpz_mul_2exp(result.mpz, x.mpz, exp_);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      mpz_mul_2exp(result.mpz, x_.mpz, exp_);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        mpz_mul_2exp(result.mpz, x_.mpz, exp_);
      }
    }
  }

  /*
     Sets ``result`` to the negation of ``x``.

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg x: The number to be negated
     :type x: :record:`bigint`

     .. seealso::
        :proc:`GMP.mpz_neg` amd
        `mpz_neg <https://gmplib.org/manual/Integer-Arithmetic#index-mpz_005fneg>`_.
  */
  proc neg(ref result: bigint, const ref x: bigint) {
    if compiledForSingleLocale() {
      mpz_neg(result.mpz, x.mpz);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      mpz_neg(result.mpz, x_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        mpz_neg(result.mpz, x_.mpz);
      }
    }
  }

  /*
     Sets ``result`` to the absolute value of ``x``.

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg x: The number to take the absolute value of
     :type x: :record:`bigint`

     .. seealso::
        :proc:`GMP.mpz_abs` amd
        `mpz_abs <https://gmplib.org/manual/Integer-Arithmetic#index-mpz_005fabs>`_.
  */
  proc abs(ref result: bigint, const ref x: bigint) {
    if compiledForSingleLocale() {
      mpz_abs(result.mpz, x.mpz);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      mpz_abs(result.mpz, x_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        mpz_abs(result.mpz, x_.mpz);
      }
    }
  }

  /* Divide ``numer`` by ``denom``, forming a quotient and storing it in
     ``result``.

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg numer: The numerator of the division operation to be performed
     :type numer: :record:`bigint`
     :arg denom: The denominator of the division operation to be performed
     :type denom: :record:`bigint`, ``integral``
     :arg rounding: The rounding style to use, see :enum:`roundingMode` for a
                    description of what the rounding styles entail.  Defaults to
                    :enumconstant:`~roundingMode.zero` if unspecified
     :type rounding: :enum:`roundingMode`

     .. warning::
        If the denominator is zero, the program behavior is undefined.

     .. seealso::
        :proc:`GMP.mpz_cdiv_q`,
        :proc:`GMP.mpz_fdiv_q`,
        :proc:`GMP.mpz_tdiv_q`, and
        `mpz_*div_q <https://gmplib.org/manual/Integer-Division#index-Division-functions>`_.
  */
  proc div(ref       result: bigint,
           const ref numer: bigint,
           const ref denom: bigint,
           param     rounding = roundingMode.zero) {
    if (chpl_checkDivByZero) then
      if denom == 0 then
        halt("Attempt to divide by zero");

    inline proc helper(ref res, const ref n, const ref d, param rnd) {
      select rnd {
        when roundingMode.up   do mpz_cdiv_q(res.mpz, n.mpz,  d.mpz);
        when roundingMode.down do mpz_fdiv_q(res.mpz, n.mpz,  d.mpz);
        when roundingMode.zero do mpz_tdiv_q(res.mpz, n.mpz,  d.mpz);
      }
    }

    if compiledForSingleLocale() {
      helper(result, numer, denom, rounding);
    } else if result.localeId == chpl_nodeID {
      const numer_ = numer;
      const denom_ = denom;
      helper(result, numer_, denom_, rounding);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const numer_ = numer;
        const denom_ = denom;
        helper(result, numer_, denom_, rounding);
      }
    }
  }

  /* See :proc:`~BigInteger.div` */
  proc div(ref       result: bigint,
           const ref numer: bigint,
                     denom: integral,
           param     rounding = roundingMode.zero)
    do BigInteger.div(result, numer, new bigint(denom), rounding);

  /* Divide ``numer`` by ``denom``, forming a remainder and storing it in
     ``result``.  The absolute value of the remainder will always be less than the
     absolute value of the denominator (i.e. ``abs(result) < abs(denom)``).

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg numer: The numerator of the division operation to be performed
     :type numer: :record:`bigint`
     :arg denom: The denominator of the division operation to be performed
     :type denom: :record:`bigint`, ``integral``
     :arg rounding: The rounding style to use, see :enum:`roundingMode` for a
                    description of what the rounding styles entail.  Defaults to
                    :enumconstant:`~roundingMode.zero` if unspecified
     :type rounding: :enum:`roundingMode`

     .. warning::
        If the denominator is zero, the program behavior is undefined.

     .. note::
        When ``rounding`` is :enumconstant:`~roundingMode.down`, this procedure
        is equivalent to :proc:`~BigInteger.mod`.

     .. seealso::
        :proc:`GMP.mpz_cdiv_r`,
        :proc:`GMP.mpz_fdiv_r`,
        :proc:`GMP.mpz_tdiv_r`, and
        `mpz_*div_r <https://gmplib.org/manual/Integer-Division#index-Division-functions>`_.
  */
  proc rem(ref       result: bigint,
           const ref numer: bigint,
           const ref denom: bigint,
           param     rounding = roundingMode.zero) {
    if (chpl_checkDivByZero) then
      if denom == 0 then
        halt("Attempt to divide by zero");

    inline proc helper(ref res, const ref n, const ref d, param rnd) {
      select rnd {
        when roundingMode.up   do mpz_cdiv_r(res.mpz, n.mpz, d.mpz);
        when roundingMode.down do mpz_fdiv_r(res.mpz, n.mpz, d.mpz);
        when roundingMode.zero do mpz_tdiv_r(res.mpz, n.mpz, d.mpz);
      }
    }

    if compiledForSingleLocale() {
      helper(result, numer, denom, rounding);
    } else if result.localeId == chpl_nodeID {
      const numer_ = numer;
      const denom_ = denom;
      helper(result, numer_, denom_, rounding);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const numer_ = numer;
        const denom_ = denom;
        helper(result, numer_, denom_, rounding);
      }
    }
  }

  /* See :proc:`~BigInteger.rem` */
  proc rem(ref       result: bigint,
           const ref numer: bigint,
                     denom: integral,
           param     rounding = roundingMode.zero)
    do BigInteger.rem(result, numer, new bigint(denom), rounding);

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
     :arg rounding: The rounding style to use, see :enum:`roundingMode` for a
                    description of what the rounding styles entail.  Defaults to
                    :enumconstant:`~roundingMode.zero` if unspecified
     :type rounding: :enum:`roundingMode`

     .. warning::
        If the denominator is zero, the program behavior is undefined.

     .. seealso::
        :proc:`GMP.mpz_cdiv_qr`,
        :proc:`GMP.mpz_fdiv_qr`,
        :proc:`GMP.mpz_tdiv_qr`, and
        `mpz_*div_qr <https://gmplib.org/manual/Integer-Division#index-Division-functions>`_.
  */
  proc divRem(ref       result: bigint,
              ref       remain: bigint,
              const ref numer: bigint,
              const ref denom: bigint,
              param     rounding = roundingMode.zero) {
    if (chpl_checkDivByZero) then
      if denom == 0 then
        halt("Attempt to divide by zero");

    inline proc helper(ref res, ref rem, const ref n, const ref d, param rnd) {
      select rnd {
        when roundingMode.up do mpz_cdiv_qr(res.mpz, rem.mpz, n.mpz, d.mpz);
        when roundingMode.down do mpz_fdiv_qr(res.mpz, rem.mpz, n.mpz, d.mpz);
        when roundingMode.zero do mpz_tdiv_qr(res.mpz, rem.mpz, n.mpz, d.mpz);
      }
    }

    if compiledForSingleLocale() {
      helper(result, remain, numer, denom, rounding);
    } else if result.localeId == chpl_nodeID {
      // TODO: need to revisit this in relation to Cray/chapel-private#4628
      var   remain_ : bigint;
      const numer_ = numer;
      const denom_ = denom;
      helper(result, remain_, numer_, denom_, rounding);
      remain = remain_;
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        var   remain_ : bigint;
        const numer_ = numer;
        const denom_ = denom;
        helper(result, remain_, numer_, denom_, rounding);
        remain = remain_;
      }
    }
  }

  /* See :proc:`~BigInteger.divRem` */
  proc divRem(ref       result: bigint,
              ref       remain: bigint,
              const ref numer: bigint,
                        denom: integral,
              param     rounding = roundingMode.zero)
    do BigInteger.divRem(result, remain, numer, new bigint(denom), rounding);

  /* Divide ``numer`` by ``2^exp``, forming a quotient and storing it in
     ``result``.

     This is the same as performing a right bit shift of ``numer`` by ``exp``
     bits when ``rounding`` is :enumconstant:`~roundingMode.down`.

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg numer: The numerator of the division operation to be performed
     :type numer: :record:`bigint`
     :arg exp: The exponent that 2 should be raised to before being used as the
               denominator of the division operation to be performed
     :type exp: ``integral``
     :arg rounding: The rounding style to use, see :enum:`roundingMode` for a
                    description of what the rounding styles entail.  Defaults to
                    :enumconstant:`~roundingMode.zero` if unspecified
     :type rounding: :enum:`roundingMode`

     .. seealso::
        :proc:`GMP.mpz_cdiv_q_2exp`,
        :proc:`GMP.mpz_fdiv_q_2exp`,
        :proc:`GMP.mpz_tdiv_q_2exp`, and
        `mpz_*div_q_2exp <https://gmplib.org/manual/Integer-Division#index-Division-functions>`_.
  */
  @unstable("div2Exp is unstable and may change in the future")
  proc div2Exp(ref       result: bigint,
               const ref numer: bigint,
                         exp: integral,
               param     rounding = roundingMode.zero) {
    const exp_ = exp.safeCast(mp_bitcnt_t);

    inline proc helper(ref res, const ref n, e, param rnd) {
      select rnd {
        when roundingMode.up   do mpz_cdiv_q_2exp(res.mpz, n.mpz, e);
        when roundingMode.down do mpz_fdiv_q_2exp(res.mpz, n.mpz, e);
        when roundingMode.zero do mpz_tdiv_q_2exp(res.mpz, n.mpz, e);
      }
    }

    if compiledForSingleLocale() {
      helper(result, numer, exp_, rounding);
    } else if result.localeId == chpl_nodeID {
      const numer_ = numer;
      helper(result, numer_, exp_, rounding);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const numer_ = numer;
        helper(result, numer_, exp_, rounding);
      }
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
     :arg rounding: The rounding style to use, see :enum:`roundingMode` for a
                    description of what the rounding styles entail.  Defaults to
                    :enumconstant:`~roundingMode.zero` if unspecified
     :type rounding: :enum:`roundingMode`

     .. seealso::
        :proc:`GMP.mpz_cdiv_r_2exp`,
        :proc:`GMP.mpz_fdiv_r_2exp`,
        :proc:`GMP.mpz_tdiv_r_2exp`, and
        `mpz_*div_r_2exp <https://gmplib.org/manual/Integer-Division#index-Division-functions>`_.
  */
  @unstable("rem2Exp is unstable and may change in the future")
  proc rem2Exp(ref       result: bigint,
               const ref numer: bigint,
                         exp: integral,
               param     rounding = roundingMode.zero) {
    const exp_ = exp.safeCast(mp_bitcnt_t);

    inline proc helper(ref res, const ref n, e, param rnd) {
      select rnd {
        when roundingMode.up   do mpz_cdiv_r_2exp(res.mpz, n.mpz, e);
        when roundingMode.down do mpz_fdiv_r_2exp(res.mpz, n.mpz, e);
        when roundingMode.zero do mpz_tdiv_r_2exp(res.mpz, n.mpz, e);
      }
    }

    if compiledForSingleLocale() {
      helper(result, numer, exp_, rounding);
    } else if result.localeId == chpl_nodeID {
      const numer_ = numer;
      helper(result, numer_, exp_, rounding);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const numer_ = numer;
        helper(result, numer_, exp_, rounding);
      }
    }
  }

  /* Stores ``x`` shifted left by ``n`` bits in ``result``. Negative ``n`` will
     result in a right shift.

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg x: The number to be shifted
     :type x: :record:`bigint`
     :arg n: The number of bits to be shifted
     :type n: ``integral``

     .. seealso::
        :proc:`~BigInteger.mul2Exp` and :proc:`~BigInteger.div2Exp`
  */
  inline proc shiftLeft(ref result: bigint, const ref x: bigint, n: integral) {
    if n >= 0
      then BigInteger.mul2Exp(result, x, n);
      else BigInteger.div2Exp(result, x, (0 - n):uint, roundingMode.down);
  }

  /* Stores ``x`` shifted right by ``n`` bits in ``result``. Negative ``n`` will
     result in a left shift.

     :arg result: Where the result is stored
     :type result: :record:`bigint`
     :arg x: The number to be shifted
     :type x: :record:`bigint`
     :arg n: The number of bits to be shifted
     :type n: ``integral``

     .. seealso::
        :proc:`~BigInteger.div2Exp` and :proc:`~BigInteger.mul2Exp`
  */
  inline proc shiftRight(ref result: bigint, const ref x: bigint, n: integral) {
    if n >= 0
      then BigInteger.div2Exp(result, x, n, roundingMode.down);
      else BigInteger.mul2Exp(result, x, (0 - n):uint);
  }

  /*
    Computes the mod operator on the two arguments, defined as
    ``mod(x, y) = x - y * floor(x / y)``.

    The result is always >= 0 if `y` > 0. It is an error if `y` == 0.

    :arg result: Where the result is stored
    :type result: :record:`bigint`
    :arg x: The dividend
    :type x: :record:`bigint`
    :arg y: The divisor
    :type y: :record:`bigint` or ``integral``

    .. note::
       If ``y`` is a ``uint``, then fewer conditionals will be evaluated at
       run time.

    .. note::
       This procedure is equivalent to calling :proc:`~BigInteger.rem` with
       ``rounding`` set to :enumconstant:`~roundingMode.down`.
  */
  proc mod(ref result: bigint, const ref x: bigint, const ref y: bigint)
    do BigInteger.rem(result, x, y, rounding=roundingMode.down);

  /* See :proc:`~BigInteger.mod` */
  proc mod(ref result: bigint, const ref x: bigint, y: integral) : int {
    if (chpl_checkDivByZero) then
      if y == 0 then
        halt("Attempt to divide by zero");

    inline proc helper(ref res, ref rem, const ref x, y) {
      if compiledForSingleLocale() {
        rem = mpz_fdiv_r_ui(res.mpz, x.mpz, y);
      } else if res.localeId == chpl_nodeID {
        const x_ = x;
        rem = mpz_fdiv_r_ui(res.mpz, x_.mpz, y);
      } else {
        const resLoc = chpl_buildLocaleID(res.localeId, c_sublocid_none);
        on __primitive("chpl_on_locale_num", resLoc) {
          const x_ = x;
          rem = mpz_fdiv_r_ui(res.mpz, x_.mpz, y);
        }
      }
    }

    var y_ : c_ulong;
    var rem: c_ulong;

    if isNonnegative(y) {
      y_ = y.safeCast(c_ulong);
      helper(result, rem, x, y_);
      return rem.safeCast(int);
    } else {
      if y >= 0 then
        y_ = y.safeCast(c_ulong);
      else
        y_ = (0 - y).safeCast(c_ulong);

      helper(result, rem, x, y_);

      if rem == 0
        then return 0;
      else if y < 0 {
        result += y;
        return rem.safeCast(int) + y;
      } else
        return rem.safeCast(int);
    }
  }

  /*
     Compares ``this`` and ``x``.

     :arg x: The number to compare against
     :type x: :record:`bigint`, ``uint``, ``int``, ``real``
     :returns: Returns a positive value if ``this`` is greater than ``x``,
               a negative value if ``this`` is less than ``x``, or zero if they
               are equal.
     :rtype: ``int``

     .. seealso::
        :proc:`GMP.mpz_cmp` and
        `mpz_cmp <https://gmplib.org/manual/Integer-Comparisons#index-mpz_005fcmp>`_.
  */
  proc bigint.cmp(const ref x: bigint) : int {
    const this_ = this.localize();
    const x_ = x.localize();
    var ret : c_int;

    ret = mpz_cmp(this_.mpz, x_.mpz);

    return ret.safeCast(int);
  }

  /* See :proc:`~bigint.cmp` */
  proc bigint.cmp(x: int) : int {
    const this_ = this.localize();
    const x_ = x.safeCast(c_long);
    var ret : c_int;

    ret = mpz_cmp_si(this_.mpz, x_);

    return ret.safeCast(int);
  }

  /* See :proc:`~bigint.cmp` */
  proc bigint.cmp(x: uint) : int {
    const this_ = this.localize();
    const x_ = x.safeCast(c_ulong);
    var ret : c_int;

    ret = mpz_cmp_ui(this_.mpz, x_);

    return ret.safeCast(int);
  }

  /* See :proc:`~bigint.cmp` */
  proc bigint.cmp(x: real) : int {
    const this_ = this.localize();
    const x_ = x : c_double;
    var ret : c_int;

    ret = mpz_cmp_d(this_.mpz, x_);

    return ret.safeCast(int);
  }

  /*
     Compares the absolute value of ``this`` and the absolute value of ``x``.

     :arg x: The number to compare against
     :type x: :record:`bigint`, ``uint``, ``real``
     :returns: Returns a positive value if ``abs(this)`` is greater than
               ``abs(x)``, a negative value if ``abs(this)`` is less than
               ``abs(x)``, or zero if they are equal.
     :rtype: ``int``

     .. seealso::
        :proc:`GMP.mpz_cmpabs` and
        `mpz_cmpabs <https://gmplib.org/manual/Integer-Comparisons#index-mpz_005fcmpabs>`_.
  */
  proc bigint.cmpabs(const ref x: bigint) : int {
    const this_ = this.localize();
    const x_ = x.localize();
    var ret : c_int;

    ret = mpz_cmpabs(this_.mpz, x_.mpz);

    return ret.safeCast(int);
  }

  /* See :proc:`~bigint.cmpabs` */
  proc bigint.cmpabs(x: uint) : int {
    const this_ = this.localize();
    const x_ = x.safeCast(c_ulong);
    var ret : c_int;

    ret = mpz_cmpabs_ui(this_.mpz, x_);

    return ret.safeCast(int);
  }

  /* See :proc:`~bigint.cmpabs` */
  proc bigint.cmpabs(x: real) : int {
    const this_ = this.localize();
    const x_ = x : c_double;
    var ret : c_int;

    ret = mpz_cmpabs_d(this_.mpz, x_);

    return ret.safeCast(int);
  }

  /*
     Returns the sign of ``this``.

     :returns: Returns 1 if positive, -1 if negative, and 0 if zero.
     :rtype: ``int``

     .. seealso::
        :proc:`GMP.mpz_sgn` and
        `mpz_sgn <https://gmplib.org/manual/Integer-Comparisons#index-mpz_005fsgn>`_.
  */
  @unstable("bigint.sgn is unstable and may change its name and return type in the future")
  proc bigint.sgn() : int {
    const this_ = this.localize();
    var ret : c_int;

    ret = mpz_sgn(this_.mpz);

    return ret.safeCast(int);
  }

  /*
    Compute the bitwise and of ``x`` and ``y`` and store it in ``result``.

    :arg result: Where the result is stored
    :type result: :record:`bigint`
    :arg x: First operand
    :type x: :record:`bigint`
    :arg y: Second operand
    :type y: :record:`bigint`

    .. seealso::
       :proc:`GMP.mpz_and` and
       `mpz_and <https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling#index-mpz_005fand>`_.
  */
  proc and(ref result: bigint, const ref x: bigint, const ref y: bigint) {
    if compiledForSingleLocale() {
      mpz_and(result.mpz, x.mpz, y.mpz);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      const y_ = y;
      mpz_and(result.mpz, x_.mpz, y_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        const y_ = y;
        mpz_and(result.mpz, x_.mpz, y_.mpz);
      }
    }
  }

  /*
    Compute the bitwise inclusive or of ``x`` and ``y`` and store it in
    ``result``.

    :arg result: Where the result is stored
    :type result: :record:`bigint`
    :arg x: First operand
    :type x: :record:`bigint`
    :arg y: Second operand
    :type y: :record:`bigint`

    .. seealso::
       :proc:`GMP.mpz_ior` and
       `mpz_ior <https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling#index-mpz_005fior>`_.
  */
  proc or(ref result: bigint, const ref x: bigint, const ref y: bigint) {
    if compiledForSingleLocale() {
      mpz_ior(result.mpz, x.mpz, y.mpz);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      const y_ = y;
      mpz_ior(result.mpz, x_.mpz, y_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        const y_ = y;
        mpz_ior(result.mpz, x_.mpz, y_.mpz);
      }
    }
  }

  /*
    Compute the bitwise exclusive or of ``x`` and ``y`` and store it in
    ``result``.

    :arg result: Where the result is stored
    :type result: :record:`bigint`
    :arg x: First operand
    :type x: :record:`bigint`
    :arg y: Second operand
    :type y: :record:`bigint`

    .. seealso::
       :proc:`GMP.mpz_xor` and
       `mpz_xor <https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling#index-mpz_005fxor>`_.
  */
  proc xor(ref result: bigint, const ref x: bigint, const ref y: bigint) {
    if compiledForSingleLocale() {
      mpz_xor(result.mpz, x.mpz, y.mpz);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      const y_ = y;
      mpz_xor(result.mpz, x_.mpz, y_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        const y_ = y;
        mpz_xor(result.mpz, x_.mpz, y_.mpz);
      }
    }
  }

  /*
    Compute the bitwise one's complement of ``x`` and store it in ``result``.

    :arg result: Where the result is stored
    :type result: :record:`bigint`
    :arg x: Number to be complemented
    :type x: :record:`bigint`

    .. seealso::
       :proc:`GMP.mpz_com` and
       `mpz_com <https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling#index-mpz_005fcom>`_.
  */
  proc com(ref result: bigint, const ref x: bigint) {
    if compiledForSingleLocale() {
      mpz_com(result.mpz, x.mpz);
    } else if result.localeId == chpl_nodeID {
      const x_ = x;
      mpz_com(result.mpz, x_.mpz);
    } else {
      const resultLoc = chpl_buildLocaleID(result.localeId, c_sublocid_none);
      on __primitive("chpl_on_locale_num", resultLoc) {
        const x_ = x;
        mpz_com(result.mpz, x_.mpz);
      }
    }
  }

  /*
    Assign ``x`` to ``this``

    :arg x: Number to be assigned
    :type x: :record:`bigint`

    .. seealso::
       :proc:`GMP.mpz_set` and
       `mpz_set <https://gmplib.org/manual/Assigning-Integers#index-mpz_005fset>`_.
  */
  proc ref bigint.set(const ref x: bigint) {
    // single locale or both bigints on the current locale
    if compiledForSingleLocale() ||
      (this.localeId == chpl_nodeID && x.localeId == chpl_nodeID) {
      mpz_set(this.mpz, x.mpz);
    } else if (this.localeId == chpl_nodeID) {
      // `this` is on the current locale and `x` is remote
      chpl_gmp_get_mpz(this.mpz, x.localeId, x.mpz[0]);
    }
    else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_none);

      on __primitive("chpl_on_locale_num", thisLoc) {
        if x.localeId == chpl_nodeID {
          // both on the same locale
          mpz_set(this.mpz, x.mpz);
        } else {
          // `x` is somewhere else
          chpl_gmp_get_mpz(this.mpz, x.localeId, x.mpz[0]);
        }
      }
    }
  }

  /* See :proc:`bigint.set` */
  proc ref bigint.set(x : int) {
    const x_ = x.safeCast(c_long);

    if compiledForSingleLocale() || this.localeId == chpl_nodeID {
      mpz_set_si(this.mpz, x_);
    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_none);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_set_si(this.mpz, x_);
      }
    }
  }

  /* See :proc:`bigint.set` */
  proc ref bigint.set(x : uint) {
    const x_ = x.safeCast(c_ulong);

    if compiledForSingleLocale() || this.localeId == chpl_nodeID {
      mpz_set_ui(this.mpz, x_);
    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_none);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_set_ui(this.mpz, x_);
      }
    }
  }

  /* See :proc:`bigint.set` */
  proc ref bigint.set(x: real) {
    const x_ = x : c_double;

    if compiledForSingleLocale() || this.localeId == chpl_nodeID {
      mpz_set_d(this.mpz, x_);
    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_none);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_set_d(this.mpz, x_);
      }
    }
  }

  /* See :proc:`bigint.set` */
  proc ref bigint.set(x: string, base: int = 0) {
    const base_ = base.safeCast(c_int);

    if compiledForSingleLocale() || this.localeId == chpl_nodeID {
      mpz_set_str(this.mpz, x.localize().c_str(), base_);
    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_none);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_set_str(this.mpz, x.localize().c_str(), base_);
      }
    }
  }

  /*
    Swaps ``this`` and ``x``

    :arg x: Number to be swapped
    :type x: :record:`bigint`

    .. seealso::
       :proc:`GMP.mpz_swap` and
       `mpz_swap <https://gmplib.org/manual/Assigning-Integers#index-mpz_005fswap>`_.
  */
  proc ref bigint.swap(ref x: bigint) {
    if compiledForSingleLocale() ||
      (this.localeId == chpl_nodeID && x.localeId == chpl_nodeID) {
      mpz_swap(this.mpz, x.mpz);
    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_none);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var tmp = new bigint(x);

        x.set(this);

        mpz_set(this.mpz, tmp.mpz);
      }
    }
  }

  @chpldoc.nodoc
  record bigintWrapper {
    pragma "no init"
    var mpz: mpz_t;
    var isOwned: bool;
    proc init(x: mpz_t) {
      mpz = x;
      isOwned = false;
    }

    proc init(x: bigint) {
      init this;
      var mpz_struct = x.getImpl();
      mpz_init(this.mpz);
      chpl_gmp_get_mpz(this.mpz, x.localeId, mpz_struct);
      isOwned = true;
    }

    proc ref deinit() {
      if isOwned then
        mpz_clear(this.mpz);
    }
  }

  @chpldoc.nodoc
  record __serializeHelper {
    var buff: mpz_t;
    var localeId: chpl_nodeID_t;
  }

  proc bigint.chpl__serialize() {
    return new __serializeHelper(this.mpz, this.localeId);
  }

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
    if compiledForSingleLocale() || this.localeId == chpl_nodeID {
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
    if compiledForSingleLocale() || this.localeId == chpl_nodeID {
      hashHelper();
    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_none);

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
  bigint implements hashable;
}
