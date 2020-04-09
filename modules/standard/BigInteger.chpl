/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

/*
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

*/

module BigInteger {
  use GMP;
  private use HaltWrappers;
  private use SysCTypes;

  enum Round {
    DOWN = -1,
    ZERO =  0,
    UP   =  1
  }

  pragma "ignore noinit"
  record bigint {
    /* The underlying GMP C structure */
    var mpz      : mpz_t;              // A dynamic-vector of C integers

    pragma "no doc"
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
        var mpz_struct = num.mpzStruct();

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

    proc init(str: string, base: int = 0) {
      this.complete();
      const str_  = str.localize().c_str();
      const base_ = base.safeCast(c_int);

      if mpz_init_set_str(this.mpz, str_, base_) != 0 {
        mpz_clear(this.mpz);

        HaltWrappers.initHalt("Error initializing big integer: bad format");
      }

      this.localeId = chpl_nodeID;
    }

    proc init(str: string, base: int = 0, out error: syserr) {
      this.complete();
      const str_  = str.localize().c_str();
      const base_ = base.safeCast(c_int);

      if mpz_init_set_str(this.mpz, str_, base_) != 0 {
        mpz_clear(this.mpz);

        error = EFORMAT;
      } else {
        error = ENOERR;
      }

      this.localeId = chpl_nodeID;
    }

    // Within a given locale, bigint assignment creates a deep copy of the
    // data and so the record "owns" the GMP data.
    //
    // If a bigint is copied to a remote node then it will receive a shallow
    // copy.  The localeId points back the correct locale but the mpz field
    // is meaningless.
    pragma "no doc"
    proc deinit() {
      if _local || this.localeId == chpl_nodeID {
        mpz_clear(this.mpz);
      }
    }

    proc size() : size_t {
      var ret: size_t;

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

    proc sizeinbase(base: int) : uint {
      const base_ = base.safeCast(c_int);
      var   ret: size_t;

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

      return ret;
    }

    proc numLimbs : uint {
      return chpl_gmp_mpz_nlimbs(this.mpz);
    }

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

    proc mpzStruct() : __mpz_struct {
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

    proc get_d_2exp() : (uint(32), real) {
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

      return (exp.safeCast(uint(32)), dbl: real);
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

      writer <~> s;
    }
  }

  //
  // Cast operators
  //
  pragma "no doc"
  inline proc _cast(type toType: bigint, src: integral): bigint {
    return new bigint(src);
  }

  pragma "no doc"
  inline proc _cast(type toType: bigint, src: string): bigint {
    return new bigint(src);
  }

  pragma "no doc"
  inline proc _cast(type t, const ref x: bigint) where isIntType(t) {
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

  pragma "no doc"
  inline proc _cast(type t, const ref x: bigint) where isUintType(t) {
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

  pragma "no doc"
  inline proc _cast(type t, const ref x: bigint) where isRealType(t) {
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

  proc =(ref lhs: bigint, const ref rhs: bigint) {
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

  proc =(ref lhs: bigint, rhs: int) {
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

  proc =(ref lhs: bigint, rhs: uint) {
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
  proc +(const ref a: bigint) {
    return new bigint(a);
  }

  proc -(const ref a: bigint) {
    var c = new bigint(a);

    mpz_neg(c.mpz, c.mpz);

    return c;
  }

  proc ~(const ref a: bigint) {
    var c = new bigint(a);

    mpz_com(c.mpz, c.mpz);

    return c;
  }

  //
  // Binary operators
  //

  // Addition
  proc +(const ref a: bigint, const ref b: bigint) {
    var c = new bigint();

    if _local {
      mpz_add(c.mpz, a.mpz, b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_add(c.mpz, a.mpz, b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      mpz_add(c.mpz, a_.mpz, b_.mpz);
    }

    return c;
  }

  proc +(const ref a: bigint, b: int) {
    var c = new bigint();

    if b >= 0 {
      const b_ = b.safeCast(c_ulong);

      if _local {
        mpz_add_ui(c.mpz, a.mpz,  b_);

      } else if a.localeId == chpl_nodeID {
        mpz_add_ui(c.mpz, a.mpz,  b_);

      } else {
        const a_ = a;

        mpz_add_ui(c.mpz, a_.mpz, b_);
      }

    } else {
      const b_ = (0 - b).safeCast(c_ulong);

      if _local {
        mpz_sub_ui(c.mpz, a.mpz, b_);

      } else if a.localeId == chpl_nodeID {
        mpz_sub_ui(c.mpz, a.mpz, b_);

      } else {
        const a_ = a;

        mpz_sub_ui(c.mpz, a_.mpz, b_);
      }
    }

    return c;
  }

  proc +(a: int, const ref b: bigint) {
    var c = new bigint();

    if a >= 0 {
      const a_ = a.safeCast(c_ulong);

      if _local {
        mpz_add_ui(c.mpz, b.mpz,  a_);

      } else if b.localeId == chpl_nodeID {
        mpz_add_ui(c.mpz, b.mpz,  a_);

      } else {
        const b_ = b;

        mpz_add_ui(c.mpz, b_.mpz, a_);
      }

    } else {
      const a_ = (0 - a).safeCast(c_ulong);

      if _local {
        mpz_sub_ui(c.mpz, b.mpz,  a_);

      } else if b.localeId == chpl_nodeID {
        mpz_sub_ui(c.mpz, b.mpz,  a_);

      } else {
        const b_ = b;

        mpz_sub_ui(c.mpz, b_.mpz, a_);
      }
    }

    return c;
  }

  proc +(const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);
    var   c  = new bigint();

    if _local {
      mpz_add_ui(c.mpz, a.mpz,  b_);

    } else if a.localeId == chpl_nodeID {
      mpz_add_ui(c.mpz, a.mpz,  b_);

    } else {
      const a_ = a;

      mpz_add_ui(c.mpz, a_.mpz, b_);
    }

    return c;
  }

  proc +(a: uint, const ref b: bigint) {
    const a_ = a.safeCast(c_ulong);
    var   c  = new bigint();

    if _local {
      mpz_add_ui(c.mpz, b.mpz,  a_);

    } else if b.localeId == chpl_nodeID {
      mpz_add_ui(c.mpz, b.mpz,  a_);

    } else {
      const b_ = b;

      mpz_add_ui(c.mpz, b_.mpz, a_);
    }

    return c;
  }



  // Subtraction
  proc -(const ref a: bigint, const ref b: bigint) {
    var c = new bigint();

    if _local {
      mpz_sub(c.mpz, a.mpz,  b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_sub(c.mpz, a.mpz,  b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      mpz_sub(c.mpz, a_.mpz, b_.mpz);
    }

    return c;
  }

  proc -(const ref a: bigint, b: int) {
    var c = new bigint();

    if b >= 0 {
      const b_ = b.safeCast(c_ulong);

      if _local {
        mpz_sub_ui(c.mpz, a.mpz,  b_);

      } else if a.localeId == chpl_nodeID {
        mpz_sub_ui(c.mpz, a.mpz,  b_);

      } else {
        const a_ = a;

        mpz_sub_ui(c.mpz, a_.mpz, b_);
      }

    } else {
      const b_ = (0 - b).safeCast(c_ulong);

      if _local {
        mpz_add_ui(c.mpz, a.mpz,  b_);

      } else if a.localeId == chpl_nodeID {
        mpz_add_ui(c.mpz, a.mpz,  b_);

      } else {
        const a_ = a;

        mpz_add_ui(c.mpz, a_.mpz, b_);
      }
    }

    return c;
  }

  proc -(a: int, const ref b: bigint) {
    var c = new bigint();

    if a >= 0 {
      const a_ = a.safeCast(c_ulong);

      if _local {
        mpz_ui_sub(c.mpz, a_, b.mpz);

      } else if b.localeId == chpl_nodeID {
        mpz_ui_sub(c.mpz, a_, b.mpz);

      } else {
        const b_ = b;

        mpz_ui_sub(c.mpz, a_, b_.mpz);
      }

    } else {
      const a_ = (0 - a).safeCast(c_ulong);

      if _local {
        mpz_add_ui(c.mpz, b.mpz,  a_);

      } else if b.localeId == chpl_nodeID {
        mpz_add_ui(c.mpz, b.mpz,  a_);

      } else {
        const b_ = b;

        mpz_add_ui(c.mpz, b_.mpz, a_);
      }
    }

    return c;
  }

  proc -(const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);
    var   c  = new bigint();

    if _local {
      mpz_sub_ui(c.mpz, a.mpz,  b_);

    } else if a.localeId == chpl_nodeID {
      mpz_sub_ui(c.mpz, a.mpz,  b_);

    } else {
      const a_ = a;

      mpz_sub_ui(c.mpz, a_.mpz, b_);
    }

    return c;
  }

  proc -(a: uint, const ref b: bigint) {
    const a_ = a.safeCast(c_ulong);
    var   c  = new bigint();

    if _local {
      mpz_ui_sub(c.mpz, a_, b.mpz);

    } else if b.localeId == chpl_nodeID {
      mpz_ui_sub(c.mpz, a_, b.mpz);

    } else {
      const b_ = b;

      mpz_ui_sub(c.mpz, a_, b_.mpz);
    }

    return c;
  }


  // Multiplication
  proc *(const ref a: bigint, const ref b: bigint) {
    var c = new bigint();

    if _local {
      mpz_mul(c.mpz, a.mpz, b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_mul(c.mpz, a.mpz, b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      mpz_mul(c.mpz, a_.mpz, b_.mpz);
    }

    return c;
  }

  proc *(const ref a: bigint, b: int) {
    const b_ = b.safeCast(c_long);
    var   c  = new bigint();

    if _local {
      mpz_mul_si(c.mpz, a.mpz,  b_);

    } else if a.localeId == chpl_nodeID {
      mpz_mul_si(c.mpz, a.mpz,  b_);

    } else {
      const a_ = a;

      mpz_mul_si(c.mpz, a_.mpz, b_);
    }

    return c;
  }

  proc *(a: int, const ref b: bigint) {
    const a_ = a.safeCast(c_long);
    var   c  = new bigint();

    if _local {
      mpz_mul_si(c.mpz, b.mpz,  a_);

    } else if b.localeId == chpl_nodeID {
      mpz_mul_si(c.mpz, b.mpz,  a_);

    } else {
      const b_ = b;

      mpz_mul_si(c.mpz, b_.mpz, a_);
    }

    return c;
  }

  proc *(const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);
    var   c  = new bigint();

    if _local {
      mpz_mul_ui(c.mpz, a.mpz,  b_);

    } else if a.localeId == chpl_nodeID {
      mpz_mul_ui(c.mpz, a.mpz,  b_);

    } else {
      const a_ = a;

      mpz_mul_ui(c.mpz, a_.mpz, b_);
    }

    return c;
  }

  proc *(a: uint, const ref b: bigint) {
    const a_ = a.safeCast(c_ulong);
    var   c  = new bigint();

    if _local {
      mpz_mul_ui(c.mpz, b.mpz,  a_);

    } else if b.localeId == chpl_nodeID {
      mpz_mul_ui(c.mpz, b.mpz,  a_);

    } else {
      const b_ = b;

      mpz_mul_ui(c.mpz, b_.mpz, a_);
    }

    return c;
  }



  // Division
  proc /(const ref a: bigint, const ref b: bigint) {
    var c = new bigint();

    if _local {
      mpz_tdiv_q(c.mpz, a.mpz, b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_tdiv_q(c.mpz, a.mpz, b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      mpz_tdiv_q(c.mpz, a_.mpz, b_.mpz);
    }

    return c;
  }

  proc /(const ref a: bigint, b: integral) {
    return a / new bigint(b);
  }

  // Exponentiation
  proc **(const ref base: bigint, const ref exp: bigint) {
    var c = new bigint();

    if _local {
      mpz_powm(c.mpz, base.mpz,  exp.mpz,  base.mpz);

    } else if base.localeId == chpl_nodeID && exp.localeId == chpl_nodeID {
      mpz_powm(c.mpz, base.mpz,  exp.mpz,  base.mpz);

    } else {
      const base_ = base;
      const exp_  = exp;

      mpz_powm(c.mpz, base_.mpz, exp_.mpz, base_.mpz);
    }

    return c;
  }

  proc **(const ref base: bigint, exp: int) {
    var c = new bigint();

    if (exp >= 0) {
      const exp_ = exp.safeCast(c_ulong);

      if _local {
        mpz_pow_ui(c.mpz, base.mpz,  exp_);

      } else if base.localeId == chpl_nodeID {
        mpz_pow_ui(c.mpz, base.mpz,  exp_);

      } else {
        const base_ = base;

        mpz_pow_ui(c.mpz, base_.mpz, exp_);
      }

    } else {
      const exp_ = new bigint(exp);

      if _local {
        mpz_powm(c.mpz, base.mpz,  exp_.mpz, base.mpz);

      } else if base.localeId == chpl_nodeID {
        mpz_powm(c.mpz, base.mpz,  exp_.mpz, base.mpz);

      } else {
        const base_ = base;

        mpz_powm(c.mpz, base_.mpz, exp_.mpz, base_.mpz);
      }
    }

    return c;
  }

  proc **(const ref base: bigint, exp: uint) {
    const exp_ = exp.safeCast(c_ulong);
    var   c    = new bigint();

    if _local {
      mpz_pow_ui(c.mpz, base.mpz,  exp_);

    } else if base.localeId == chpl_nodeID {
      mpz_pow_ui(c.mpz, base.mpz,  exp_);

    } else {
      const base_ = base;

      mpz_pow_ui(c.mpz, base_.mpz, exp_);
    }

    return c;
  }



  // Mod
  proc %(const ref a: bigint, const ref b: bigint) {
    var c = new bigint();

    if _local {
      mpz_mod(c.mpz, a.mpz,  b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_mod(c.mpz, a.mpz,  b.mpz);

    } else {
      const a_ = a;

      mpz_mod(c.mpz, a_.mpz, b.mpz);
    }

    return c;
  }

  proc %(const ref a: bigint, b: int) {
    var b_ = 0 : c_ulong;
    var c  = new bigint();

    if b >= 0 then
      b_ = b.safeCast(c_ulong);
    else
      b_ = (0 - b).safeCast(c_ulong);

    if _local {
      mpz_mod_ui(c.mpz, a.mpz,  b_);

    } else if a.localeId == chpl_nodeID {
      mpz_mod_ui(c.mpz, a.mpz,  b_);

    } else {
      const a_ = a;

      mpz_mod_ui(c.mpz, a_.mpz, b_);
    }

    return c;
  }

  proc %(const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);
    var   c  = new bigint();

    if _local {
      mpz_mod_ui(c.mpz, a.mpz,  b_);

    } else if a.localeId == chpl_nodeID {
      mpz_mod_ui(c.mpz, a.mpz,  b_);

    } else {
      const a_ = a;

      mpz_mod_ui(c.mpz, a_.mpz, b_);
    }

    return c;
  }



  // Bit-shift left
  proc <<(const ref a: bigint, b: int) {
    var c = new bigint();

    if b >= 0 {
      const b_ = b.safeCast(mp_bitcnt_t);

      if _local {
        mpz_mul_2exp(c.mpz, a.mpz,  b_);

      } else if a.localeId == chpl_nodeID {
        mpz_mul_2exp(c.mpz, a.mpz,  b_);

      } else {
        const a_ = a;

        mpz_mul_2exp(c.mpz, a_.mpz, b_);
      }

    } else {
      const b_ = (0 - b).safeCast(mp_bitcnt_t);

      if _local {
        mpz_tdiv_q_2exp(c.mpz, a.mpz,  b_);

      } else if a.localeId == chpl_nodeID {
        mpz_tdiv_q_2exp(c.mpz, a.mpz,  b_);

      } else {
        const a_ = a;

        mpz_tdiv_q_2exp(c.mpz, a_.mpz, b_);
      }
    }

    return c;
  }

  proc <<(const ref a: bigint, b: uint) {
    const b_ = b.safeCast(mp_bitcnt_t);
    var   c  = new bigint();

    if _local {
      mpz_mul_2exp(c.mpz, a.mpz,  b_);

    } else if a.localeId == chpl_nodeID {
      mpz_mul_2exp(c.mpz, a.mpz,  b_);

    } else {
      const a_ = a;

      mpz_mul_2exp(c.mpz, a_.mpz, b_);
    }

    return c;
  }



  // Bit-shift right
  proc >>(const ref a: bigint, b: int) {
    var c = new bigint();

    if b >= 0 {
      const b_ = b.safeCast(mp_bitcnt_t);
      var   c  = new bigint();

      if _local {
        mpz_tdiv_q_2exp(c.mpz, a.mpz,  b_);

      } else if a.localeId == chpl_nodeID {
        mpz_tdiv_q_2exp(c.mpz, a.mpz,  b_);

      } else {
        const a_ = a;

        mpz_tdiv_q_2exp(c.mpz, a_.mpz, b_);
      }

    } else {
      const b_ = (0 - b).safeCast(mp_bitcnt_t);

      if _local {
        mpz_mul_2exp(c.mpz, a.mpz,  b_);

      } else if a.localeId == chpl_nodeID {
        mpz_mul_2exp(c.mpz, a.mpz,  b_);

      } else {
        const a_ = a;

        mpz_mul_2exp(c.mpz, a_.mpz, b_);
      }
    }

    return c;
  }

  proc >>(const ref a: bigint, b: uint) {
    const b_ = b.safeCast(mp_bitcnt_t);
    var   c  = new bigint();

    if _local {
      mpz_tdiv_q_2exp(c.mpz, a.mpz,  b_);

    } else if a.localeId == chpl_nodeID {
      mpz_tdiv_q_2exp(c.mpz, a.mpz,  b_);

    } else {
      const a_ = a;

      mpz_tdiv_q_2exp(c.mpz, a_.mpz, b_);
    }

    return c;
  }



  // Bitwise and
  proc &(const ref a: bigint, const ref b: bigint) {
    var c = new bigint();

    if _local {
      mpz_and(c.mpz, a.mpz, b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_and(c.mpz, a.mpz, b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      mpz_and(c.mpz, a_.mpz, b_.mpz);
    }

    return c;
  }



  // Bitwise ior
  proc |(const ref a: bigint, const ref b: bigint) {
    var c = new bigint();

    if _local {
      mpz_ior(c.mpz, a.mpz, b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_ior(c.mpz, a.mpz, b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      mpz_ior(c.mpz, a_.mpz, b_.mpz);
    }

    return c;
  }



  // Bitwise xor
  proc ^(const ref a: bigint, const ref b: bigint) {
    var c = new bigint();

    if _local {
      mpz_xor(c.mpz, a.mpz, b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_xor(c.mpz, a.mpz, b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      mpz_xor(c.mpz, a_.mpz, b_.mpz);
    }

    return c;
  }



  //
  // Comparison Operations
  //

  private inline proc cmp(const ref a: bigint, const ref b: bigint) {
    var ret : c_int;

    if _local {
      ret = mpz_cmp(a.mpz,  b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      ret = mpz_cmp(a.mpz,  b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      ret = mpz_cmp(a_.mpz, b_.mpz);
    }

    return ret;
  }

  private inline proc cmp(const ref a: bigint, b: int) {
    const b_ = b.safeCast(c_long);
    var   ret : c_int;

    if _local {
      ret = mpz_cmp_si(a.mpz,  b_);

    } else if a.localeId == chpl_nodeID {
      ret = mpz_cmp_si(a.mpz,  b_);

    } else {
      const a_ = a;

      ret = mpz_cmp_si(a_.mpz, b_);
    }

    return ret;
  }

  private inline proc cmp(a: int, const ref b: bigint) {
    const a_ = a.safeCast(c_long);
    var   ret : c_int;

    if _local {
      ret = mpz_cmp_si(b.mpz,  a_);

    } else if b.localeId == chpl_nodeID {
      ret = mpz_cmp_si(b.mpz,  a_);

    } else {
      const b_ = b;

      ret = mpz_cmp_si(b_.mpz, a_);
    }

    return 0 - ret;
  }

  private inline proc cmp(const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);
    var   ret : c_int;

    if _local {
      ret = mpz_cmp_ui(a.mpz,  b_);

    } else if a.localeId == chpl_nodeID {
      ret = mpz_cmp_ui(a.mpz,  b_);

    } else {
      const a_ = a;

      ret = mpz_cmp_ui(a_.mpz, b_);
    }

    return ret;
  }

  private inline proc cmp(a: uint, const ref b: bigint) {
    const a_ = a.safeCast(c_ulong);
    var   ret : c_int;

    if _local {
      ret = mpz_cmp_ui(b.mpz,  a_);

    } else if b.localeId == chpl_nodeID {
      ret = mpz_cmp_ui(b.mpz,  a_);

    } else {
      const b_ = b;

      ret = mpz_cmp_ui(b_.mpz, a_);
    }

    return 0 - ret;
  }


  // Equality
  proc ==(const ref a: bigint, const ref b: bigint) {
    return cmp(a, b) == 0;
  }

  proc ==(const ref a: bigint, b: int) {
    return cmp(a, b) == 0;
  }

  proc ==(a: int, const ref b: bigint) {
    return cmp(a, b) == 0;
  }

  proc ==(const ref a: bigint, b: uint) {
    return cmp(a, b) == 0;
  }

  proc ==(a: uint, const ref b: bigint) {
    return cmp(a, b) == 0;
  }



  // Inequality
  proc !=(const ref a: bigint, const ref b: bigint) {
    return cmp(a, b) != 0;
  }

  proc !=(const ref a: bigint, b: int) {
    return cmp(a, b) != 0;
  }

  proc !=(a: int, const ref b: bigint) {
    return cmp(a, b) != 0;
  }

  proc !=(const ref a: bigint, b: uint) {
    return cmp(a, b) != 0;
  }

  proc !=(a: uint, const ref b: bigint) {
    return cmp(a, b) != 0;
  }



  // Greater than
  proc >(const ref a: bigint, const ref b: bigint) {
    return cmp(a, b) > 0;
  }

  proc >(const ref a: bigint, b: int) {
    return cmp(a, b) > 0;
  }

  proc >(a: int, const ref b: bigint) {
    return cmp(a, b) > 0;
  }

  proc >(const ref a: bigint, b: uint) {
    return cmp(a, b) > 0;
  }

  proc >(a: uint, const ref b: bigint) {
    return cmp(a, b) > 0;
  }



  // Less than
  proc <(const ref a: bigint, const ref b: bigint) {
    return cmp(a, b) < 0;
  }

  proc <(const ref a: bigint, b: int) {
    return cmp(a, b) < 0;
  }

  proc <(a: int, const ref b: bigint) {
    return cmp(a, b) < 0;
  }

  proc <(const ref a: bigint, b: uint) {
    return cmp(a, b) < 0;
  }

  proc <(a: uint, const ref b: bigint) {
    return cmp(a, b) < 0;
  }


  // Greater than or equal
  proc >=(const ref a: bigint, const ref b: bigint) {
    return cmp(a, b) >= 0;
  }

  proc >=(const ref a: bigint, b: int) {
    return cmp(a, b) >= 0;
  }

  proc >=(a: int, const ref b: bigint) {
    return cmp(a, b) >= 0;
  }

  proc >=(const ref a: bigint, b: uint) {
    return cmp(a, b) >= 0;
  }

  proc >=(a: uint, const ref b: bigint) {
    return cmp(a, b) >= 0;
  }



  // Less than or equal
  proc <=(const ref a: bigint, const ref b: bigint) {
    return cmp(a, b) <= 0;
  }

  proc <=(const ref a: bigint, b: int) {
    return cmp(a, b) <= 0;
  }

  proc <=(a: int, const ref b: bigint) {
    return cmp(a, b) <= 0;
  }

  proc <=(const ref a: bigint, b: uint) {
    return cmp(a, b) <= 0;
  }

  proc <=(a: uint, const ref b: bigint) {
    return cmp(a, b) <= 0;
  }




  //
  // Compound Assignment Operations
  //

  // +=
  proc +=(ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_add(a.mpz, a.mpz, b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_add(a.mpz, a.mpz, b.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_add(a.mpz, a.mpz, b_.mpz);
      }
    }
  }

  proc +=(ref a: bigint, b: int) {
    if (b >= 0) {
      const b_ = b.safeCast(c_ulong);

      if _local {
        mpz_add_ui(a.mpz, a.mpz, b_);

      } else if a.localeId == chpl_nodeID {
        mpz_add_ui(a.mpz, a.mpz, b_);

      } else {
        const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", aLoc) {
          mpz_add_ui(a.mpz, a.mpz, b_);
        }
      }

    } else {
      const b_ = (0 - b).safeCast(c_ulong);

      if _local {
        mpz_sub_ui(a.mpz, a.mpz, b_);

      } else if a.localeId == chpl_nodeID {
        mpz_sub_ui(a.mpz, a.mpz, b_);

      } else {
        const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", aLoc) {
          mpz_sub_ui(a.mpz, a.mpz, b_);
        }
      }
    }
  }

  proc +=(ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_add_ui(a.mpz, a.mpz, b_);

    } else if a.localeId == chpl_nodeID {
      mpz_add_ui(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_add_ui(a.mpz, a.mpz, b_);
      }
    }
  }



  // -=
  proc -=(ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_sub(a.mpz, a.mpz, b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_sub(a.mpz, a.mpz, b.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_sub(a.mpz, a.mpz, b_.mpz);
      }
    }
  }

  proc -=(ref a: bigint, b: int) {
    if (b >= 0) {
      const b_ = b.safeCast(c_ulong);

      if _local {
        mpz_sub_ui(a.mpz, a.mpz, b_);

      } else if a.localeId == chpl_nodeID {
        mpz_sub_ui(a.mpz, a.mpz, b_);

      } else {
        const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", aLoc) {
          mpz_sub_ui(a.mpz, a.mpz, b_);
        }
      }

    } else {
      const b_ = (0 - b).safeCast(c_ulong);

      if _local {
        mpz_add_ui(a.mpz, a.mpz, b_);

      } else if a.localeId == chpl_nodeID {
        mpz_add_ui(a.mpz, a.mpz, b_);

      } else {
        const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", aLoc) {
          mpz_add_ui(a.mpz, a.mpz, b_);
        }
      }
    }
  }

  proc -=(ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_sub_ui(a.mpz, a.mpz, b_);

    } else if a.localeId == chpl_nodeID {
      mpz_sub_ui(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_sub_ui(a.mpz, a.mpz, b_);
      }
    }
  }



  // *=
  proc *=(ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_mul(a.mpz, a.mpz, b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_mul(a.mpz, a.mpz, b.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_mul(a.mpz, a.mpz, b_.mpz);
      }
    }
  }

  proc *=(ref a: bigint, b: int) {
    const b_ = b.safeCast(c_long);

    if _local {
      mpz_mul_si(a.mpz, a.mpz, b_);

    } else if a.localeId == chpl_nodeID {
      mpz_mul_si(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_mul_si(a.mpz, a.mpz, b_);
      }
    }
  }

  proc *=(ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_mul_ui(a.mpz, a.mpz, b_);

    } else if a.localeId == chpl_nodeID {
      mpz_mul_ui(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_mul_ui(a.mpz, a.mpz, b_);
      }
    }
  }



  // /=
  proc /=(ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_tdiv_q(a.mpz, a.mpz, b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_tdiv_q(a.mpz, a.mpz, b.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_tdiv_q(a.mpz, a.mpz, b_.mpz);
      }
    }
  }

  proc /=(ref a: bigint, b: integral) {
    a /= new bigint(b);
  }



  // **=
  proc **=(ref base: bigint, const ref exp: bigint) {
    if _local {
      mpz_powm(base.mpz, base.mpz,  exp.mpz,  base.mpz);

    } else if base.localeId == chpl_nodeID && exp.localeId == chpl_nodeID {
      mpz_powm(base.mpz, base.mpz,  exp.mpz,  base.mpz);

    } else {
      const base_ = base;
      const exp_  = exp;

      mpz_powm(base.mpz, base_.mpz, exp_.mpz, base_.mpz);
    }
  }

  proc **=(ref base: bigint, exp: int) {
    if (exp >= 0) {
      const exp_ = exp.safeCast(c_ulong);

      if _local {
        mpz_pow_ui(base.mpz, base.mpz,  exp_);

      } else if base.localeId == chpl_nodeID {
        mpz_pow_ui(base.mpz, base.mpz,  exp_);

      } else {
        const base_ = base;

        mpz_pow_ui(base.mpz, base_.mpz, exp_);
      }

    } else {
      const exp_ = new bigint(exp);

      if _local {
        mpz_powm(base.mpz, base.mpz,  exp_.mpz, base.mpz);

      } else if base.localeId == chpl_nodeID {
        mpz_powm(base.mpz, base.mpz,  exp_.mpz, base.mpz);

      } else {
        const base_ = base;

        mpz_powm(base_.mpz, base_.mpz, exp_.mpz, base_.mpz);
      }
    }
  }

  proc **=(ref base: bigint, exp: uint) {
    const exp_ = exp.safeCast(c_ulong);

    if _local {
      mpz_pow_ui(base.mpz, base.mpz, exp_);

    } else if base.localeId == chpl_nodeID {
      mpz_pow_ui(base.mpz, base.mpz, exp_);

    } else {
      const baseLoc = chpl_buildLocaleID(base.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", baseLoc) {
        mpz_pow_ui(base.mpz, base.mpz, exp_);
      }
    }
  }



  // %=
  proc %=(ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_mod(a.mpz, a.mpz, b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_mod(a.mpz, a.mpz, b.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_mod(a.mpz, a.mpz, b_.mpz);
      }
    }
  }

  proc %=(ref a: bigint, b: int) {
    var b_ = 0 : uint;

    if b >= 0 then
      b_ = b       : uint;
    else
      b_ = (0 - b) : uint;

      a %= b_;
  }

  proc %=(ref a: bigint, b: uint) {
    var b_ = b.safeCast(c_ulong);

    if _local {
      mpz_mod_ui(a.mpz, a.mpz, b_);

    } else if a.localeId == chpl_nodeID {
      mpz_mod_ui(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_mod_ui(a.mpz, a.mpz, b_);
      }
    }
  }

  proc &=(ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_and(a.mpz, a.mpz, b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_and(a.mpz, a.mpz, b.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_and(a.mpz, a.mpz, b_.mpz);
      }
    }
  }

  proc |=(ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_ior(a.mpz, a.mpz, b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_ior(a.mpz, a.mpz, b.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_ior(a.mpz, a.mpz, b_.mpz);
      }
    }
  }

  proc ^=(ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_xor(a.mpz, a.mpz, b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_xor(a.mpz, a.mpz, b.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_xor(a.mpz, a.mpz, b_.mpz);
      }
    }
  }


  // <<=
  proc <<=(ref a: bigint, b: int) {
    if b >= 0 {
      const b_ = b.safeCast(mp_bitcnt_t);

      if _local {
        mpz_mul_2exp(a.mpz, a.mpz, b_);

      } else if a.localeId == chpl_nodeID {
        mpz_mul_2exp(a.mpz, a.mpz, b_);

      } else {
        const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", aLoc) {
          mpz_mul_2exp(a.mpz, a.mpz, b_);
        }
      }

    } else {
      const b_ = (0 - b).safeCast(mp_bitcnt_t);

      if _local {
        mpz_tdiv_q_2exp(a.mpz, a.mpz, b_);

      } else if a.localeId == chpl_nodeID {
        mpz_tdiv_q_2exp(a.mpz, a.mpz, b_);

      } else {
        const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", aLoc) {
          mpz_tdiv_q_2exp(a.mpz, a.mpz, b_);
        }
      }
    }
  }

  proc <<=(ref a: bigint, b: uint) {
    const b_ = b.safeCast(mp_bitcnt_t);

    if _local {
      mpz_mul_2exp(a.mpz, a.mpz, b_);

    } else if a.localeId == chpl_nodeID {
      mpz_mul_2exp(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_mul_2exp(a.mpz, a.mpz, b_);
      }
    }
  }



  // >>=
  proc >>=(ref a: bigint, b: int) {
    if b >= 0 {
      const b_ = b.safeCast(mp_bitcnt_t);

      if _local {
        mpz_tdiv_q_2exp(a.mpz, a.mpz, b_);

      } else if a.localeId == chpl_nodeID {
        mpz_tdiv_q_2exp(a.mpz, a.mpz, b_);

      } else {
        const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", aLoc) {
          mpz_tdiv_q_2exp(a.mpz, a.mpz, b_);
        }
      }

    } else {
      const b_ = (0 - b).safeCast(mp_bitcnt_t);

      if _local {
        mpz_mul_2exp(a.mpz, a.mpz, b_);

      } else if a.localeId == chpl_nodeID {
        mpz_mul_2exp(a.mpz, a.mpz, b_);

      } else {
        const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", aLoc) {
          mpz_mul_2exp(a.mpz, a.mpz, b_);
        }
      }
    }
  }

  proc >>=(ref a: bigint, b: uint) {
    const b_ = b.safeCast(mp_bitcnt_t);

    if _local {
      mpz_tdiv_q_2exp(a.mpz, a.mpz, b_);

    } else if a.localeId == chpl_nodeID {
      mpz_tdiv_q_2exp(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_tdiv_q_2exp(a.mpz, a.mpz, b_);
      }
    }
  }


  // Swap
  proc <=>(ref a: bigint, ref b: bigint) {
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
    var ret : c_int;

    if _local {
      ret = mpz_jacobi(a.mpz,  b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      ret = mpz_jacobi(a.mpz,  b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      ret = mpz_jacobi(a_.mpz, b_.mpz);
    }

    return ret.safeCast(int);
  }



  proc legendre(const ref a: bigint, const ref p: bigint) : int {
    var ret : c_int;

    if _local {
      ret = mpz_legendre(a.mpz,  p.mpz);

    } else if a.localeId == chpl_nodeID && p.localeId == chpl_nodeID {
      ret = mpz_legendre(a.mpz,  p.mpz);

    } else {
      const a_ = a;
      const p_ = p;

      ret = mpz_legendre(a_.mpz, p_.mpz);
    }

    return ret.safeCast(int);
  }



  // kronecker
  proc kronecker(const ref a: bigint, const ref b: bigint) : int {
    var ret : c_int;

    if _local {
      ret = mpz_kronecker(a.mpz,  b.mpz);

    } else if a.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      ret = mpz_kronecker(a.mpz,  b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      ret = mpz_kronecker(a_.mpz, b_.mpz);
    }

    return ret.safeCast(int);
  }

  proc kronecker(const ref a: bigint, b: int) : int {
    const b_ = b.safeCast(c_long);
    var  ret : c_int;

    if _local {
      ret = mpz_kronecker_si(a.mpz,  b_);

    } else if a.localeId == chpl_nodeID {
      ret = mpz_kronecker_si(a.mpz,  b_);

    } else {
      const a_ = a;

      ret = mpz_kronecker_si(a_.mpz, b_);
    }

    return ret.safeCast(int);
  }

  proc kronecker(a: int, const ref b: bigint) : int {
    const a_ = a.safeCast(c_long);
    var  ret : c_int;

    if _local {
      ret = mpz_si_kronecker(a_, b.mpz);

    } else if b.localeId == chpl_nodeID {
      ret = mpz_si_kronecker(a_, b.mpz);

    } else {
      const b_ = b;

      ret = mpz_si_kronecker(a_, b_.mpz);
    }

    return ret.safeCast(int);
  }

  proc kronecker(const ref a: bigint, b: uint) : int {
    const b_ = b.safeCast(c_ulong);
    var  ret : c_int;

    if _local {
      ret = mpz_kronecker_ui(a.mpz,  b_);

    } else if a.localeId == chpl_nodeID {
      ret = mpz_kronecker_ui(a.mpz,  b_);

    } else {
      const a_ = a;

      ret = mpz_kronecker_ui(a_.mpz, b_);
    }

    return ret.safeCast(int);
  }

  proc kronecker(a: uint, const ref b: bigint) : int {
    const a_ = b.safeCast(c_ulong);
    var  ret : c_int;

    if _local {
      ret = mpz_ui_kronecker(a_, a.mpz);

    } else if b.localeId == chpl_nodeID {
      ret = mpz_ui_kronecker(a_, a.mpz);

    } else {
      const b_ = b;

      ret = mpz_ui_kronecker(a_, b_.mpz);
    }

    return ret.safeCast(int);
  }




  // divexact

/*
Computes ``n/d`` and stores the result in ``bigint`` instance.

``divexact`` is optimized to handle cases where ``n/d`` results in an integer.
When ``n/d`` does not produce an integer, this method may produce incorrect results.

:arg n: numerator

:type n: bigint

:arg d: denominator

:type d: bigint
*/


  proc bigint.divexact(const ref n: bigint, const ref d: bigint) {
    if _local {
      mpz_divexact(this.mpz, n.mpz, d.mpz);

    } else if this.localeId == chpl_nodeID &&
              n.localeId    == chpl_nodeID &&
              d.localeId    == chpl_nodeID {
      mpz_divexact(this.mpz, n.mpz, d.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const n_ = n;
        const d_ = d;

        mpz_divexact(this.mpz, n_.mpz, d_.mpz);
      }
    }
  }

  proc bigint.divexact(const ref n: bigint, d: integral) {
    this.divexact(n, new bigint(d));
  }



  // divisible_p
  proc bigint.divisible_p(const ref d: bigint) : int {
    var ret: c_int;

    if _local {
      ret = mpz_divisible_p(this.mpz, d.mpz);

    } else if this.localeId == chpl_nodeID && d.localeId == chpl_nodeID {
      ret = mpz_divisible_p(this.mpz, d.mpz);

    } else {
      const t_ = this;
      const d_ = d;

      ret = mpz_divisible_p(this.mpz, d.mpz);
    }

    return ret.safeCast(int);
  }

  proc bigint.divisible_p(d: int) : int {
    var d_ = 0 : c_ulong;
    var ret: c_int;

    if d >= 0 then
      d_ = d.safeCast(c_ulong);
    else
      d_ = (0 - d).safeCast(c_ulong);

    if _local {
      ret = mpz_divisible_ui_p(this.mpz, d_);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_divisible_ui_p(this.mpz, d_);

    } else {
      const t_ = this;

      ret = mpz_divisible_ui_p(t_.mpz,   d_);
    }

    return ret.safeCast(int);
  }

  proc bigint.divisible_p(d: uint) : int {
    const d_ = d.safeCast(c_ulong);
    var   ret: c_int;

    if _local {
      ret = mpz_divisible_ui_p(this.mpz, d_);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_divisible_ui_p(this.mpz, d_);

    } else {
      const t_ = this;

      ret = mpz_divisible_ui_p(t_.mpz,   d_);
    }

    return ret.safeCast(int);
  }

  proc bigint.divisible_2exp_p(b: integral) : int {
    const b_ = b.safeCast(mp_bitcnt_t);
    var   ret: c_int;

    if _local {
      ret = mpz_divisible_2exp_p(this.mpz, b_);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_divisible_2exp_p(this.mpz, b_);

    } else {
      const t_ = this;

      ret = mpz_divisible_2exp_p(t_.mpz,   b_);
    }

    return ret.safeCast(int);
  }



  // congruent_p
  proc bigint.congruent_p(const ref c: bigint, const ref d: bigint) : int {
    var ret: c_int;

    if _local {
      ret = mpz_congruent_p(this.mpz, c.mpz, d.mpz);

    } else if this.localeId == chpl_nodeID &&
              c.localeId    == chpl_nodeID &&
              d.localeId    == chpl_nodeID {
      ret = mpz_congruent_p(this.mpz, c.mpz, d.mpz);

    } else {
      const t_ = this;
      const c_ = c;
      const d_ = d;

      ret = mpz_congruent_p(t_.mpz, c_.mpz, d_.mpz);
    }

    return ret.safeCast(int);
  }

  proc bigint.congruent_p(c: integral, d: integral) : int {
    const c_ = c.safeCast(c_ulong);
    const d_ = d.safeCast(c_ulong);
    var   ret: c_int;

    if _local {
      ret = mpz_congruent_ui_p(this.mpz, c_, d_);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_congruent_ui_p(this.mpz, c_, d_);

    } else {
      const t_ = this;

      ret = mpz_congruent_ui_p(t_.mpz,   c_, d_);
    }

    return ret.safeCast(int);
  }

  proc bigint.congruent_2exp_p(const ref c: bigint, b: integral) : int {
    const b_ = b.safeCast(mp_bitcnt_t);
    var   ret: c_int;

    if _local {
      ret = mpz_congruent_2exp_p(this.mpz, c.mpz, b_);

    } else if this.localeId == chpl_nodeID &&
              c.localeId    == chpl_nodeID {
      ret = mpz_congruent_2exp_p(this.mpz, c.mpz, b_);

    } else {
      const t_ = this;
      const c_ = c;

      ret = mpz_congruent_2exp_p(t_.mpz, c_.mpz, b_);
    }

    return ret.safeCast(int);
  }



  // Exponentiation Functions
  proc bigint.powm(const ref base: bigint,
                   const ref exp:  bigint,
                   const ref mod:  bigint) {
    if _local {
      mpz_powm(this.mpz, base.mpz, exp.mpz, mod.mpz);

    } else if this.localeId == chpl_nodeID &&
              base.localeId == chpl_nodeID &&
              exp.localeId  == chpl_nodeID &&
              mod.localeId  == chpl_nodeID {
      mpz_powm(this.mpz, base.mpz, exp.mpz, mod.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const base_ = base;
        const exp_  = exp;
        const mod_  = mod;

        mpz_powm(this.mpz, base_.mpz, exp_.mpz, mod_.mpz);
      }
    }
  }

  proc bigint.powm(const ref base: bigint,
                             exp:  int,
                   const ref mod:  bigint) {
    if exp >= 0 {
      const exp_ = exp.safeCast(c_ulong);

      if _local {
        mpz_powm_ui(this.mpz, base.mpz, exp_, mod.mpz);

      } else if this.localeId == chpl_nodeID &&
                base.localeId == chpl_nodeID &&
                mod.localeId  == chpl_nodeID {
        mpz_powm_ui(this.mpz, base.mpz, exp_, mod.mpz);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          const base_ = base;
          const mod_  = mod;

          mpz_powm_ui(this.mpz, base_.mpz, exp_, mod_.mpz);
        }
      }

    } else {
      if _local {
        const exp_ = new bigint(exp);

        mpz_powm(this.mpz, base.mpz, exp_.mpz, mod.mpz);

      } else if this.localeId == chpl_nodeID &&
                base.localeId == chpl_nodeID &&
                mod.localeId  == chpl_nodeID {
        const exp_ = new bigint(exp);

        mpz_powm(this.mpz, base.mpz, exp_.mpz, mod.mpz);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          const base_ = base;
          const exp_  = new bigint(exp);
          const mod_  = mod;

          mpz_powm(this.mpz, base_.mpz, exp_.mpz, mod_.mpz);
        }
      }
    }
  }

  proc bigint.powm(const ref base: bigint,
                             exp:  uint,
                   const ref mod:  bigint) {
    const exp_ = exp.safeCast(c_ulong);

    if _local {
      mpz_powm_ui(this.mpz, base.mpz, exp_, mod.mpz);

    } else if this.localeId == chpl_nodeID &&
              base.localeId == chpl_nodeID &&
              mod.localeId  == chpl_nodeID {
      mpz_powm_ui(this.mpz, base.mpz, exp_, mod.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const base_ = base;
        const mod_  = mod;

        mpz_powm_ui(this.mpz, base_.mpz, exp_, mod_.mpz);
      }
    }
  }





  proc bigint.pow(const ref base: bigint, exp: int) {
    if exp >= 0 {
      this.pow(base, exp : uint);

    } else {
      if _local {
        const exp_ = new bigint(exp);

        mpz_powm(this.mpz, base.mpz, exp_.mpz, base.mpz);

      } else if this.localeId == chpl_nodeID &&
                base.localeId == chpl_nodeID {
        const exp_ = new bigint(exp);

        mpz_powm(this.mpz, base.mpz, exp_.mpz, base.mpz);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          const base_ = base;
          const exp_  = new bigint(exp);

          mpz_powm(this.mpz, base_.mpz, exp_.mpz, base_.mpz);
        }
      }
    }
  }

  proc bigint.pow(const ref base: bigint, exp: uint) {
    const exp_ = exp.safeCast(c_ulong);

    if _local {
      mpz_pow_ui(this.mpz, base.mpz, exp_);

    } else if this.localeId == chpl_nodeID &&
              base.localeId == chpl_nodeID {
      mpz_pow_ui(this.mpz, base.mpz, exp_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const base_ = base;

        mpz_pow_ui(this.mpz, base_.mpz, exp_);
      }
    }
  }

  proc bigint.pow(base: int, exp: int) {
    if base >= 0 && exp >= 0 {
      this.pow(base : uint, exp : uint);

    } else if base <  0 && exp >= 0 {
      this.pow((0 - base) : uint, exp : uint);

      if (exp % 2) == 1 {
        this.neg(this);
      }

    } else {
      this.pow(new bigint(base), exp);
    }
  }

  proc bigint.pow(base: uint, exp: uint) {
    const base_ = base.safeCast(c_ulong);
    const exp_  = exp.safeCast(c_ulong);

    if _local {
      mpz_ui_pow_ui(this.mpz, base_, exp_);

    } else if this.localeId == chpl_nodeID {
      mpz_ui_pow_ui(this.mpz, base_, exp_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_ui_pow_ui(this.mpz, base_, exp_);
      }
    }
  }

  // Root Extraction Functions
  proc bigint.root(const ref a: bigint, n: uint) : int {
    const n_  = n.safeCast(c_ulong);
    var   ret: c_int;

    if _local {
      ret = mpz_root(this.mpz, a.mpz, n_);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_root(this.mpz, a.mpz, n_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        ret = mpz_root(this.mpz, a_.mpz, n_);
      }
    }

    return ret.safeCast(int);
  }

  // this gets root, rem gets remainder.
  proc bigint.rootrem(ref rem: bigint, const ref u: bigint, n: uint) {
    const n_  = n.safeCast(c_ulong);

    if _local {
      mpz_rootrem(this.mpz, rem.mpz, u.mpz, n_);

    } else if this.localeId == chpl_nodeID &&
              rem.localeId  == chpl_nodeID &&
              u.localeId    == chpl_nodeID {
      mpz_rootrem(this.mpz, rem.mpz, u.mpz, n_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var   rem_ = rem;
        const u_   = u;

        mpz_rootrem(this.mpz, rem_.mpz, u_.mpz, n_);

        rem = rem_;
      }
    }
  }

  proc bigint.sqrt(const ref a: bigint) {
    if _local {
      mpz_sqrt(this.mpz, a.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
      mpz_sqrt(this.mpz, a.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_sqrt(this.mpz, a_.mpz);
      }
    }
  }

  // this gets root, rem gets remainder of a-root*root.
  proc bigint.sqrtrem(ref rem: bigint, const ref a: bigint) {
    if _local {
      mpz_sqrtrem(this.mpz, rem.mpz, a.mpz);

    } else if this.localeId == chpl_nodeID &&
              rem.localeId  == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
      mpz_sqrtrem(this.mpz, rem.mpz, a.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var   r_ = rem;
        const a_ = a;

        mpz_sqrtrem(this.mpz, r_.mpz, a_.mpz);

        rem = r_;
      }
    }
  }

  proc bigint.perfect_power_p() : int {
    var ret: c_int;

    if _local || this.localeId == chpl_nodeID {
      ret = mpz_perfect_power_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_perfect_power_p(t_.mpz);
    }

    return ret.safeCast(int);
  }

  proc bigint.perfect_square_p() : int {
    var ret: c_int;

    if _local || this.localeId == chpl_nodeID {
      ret = mpz_perfect_square_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_perfect_square_p(t_.mpz);
    }

    return ret.safeCast(int);
  }




  // Number Theoretic Functions

  // returns 2 if definitely prime, 0 if not prime, 1 if likely prime
  // reasonable number of reps is 15-50
  proc bigint.probab_prime_p(reps: int) : int {
    var reps_ = reps.safeCast(c_int);
    var ret: c_int;

    if _local || this.localeId == chpl_nodeID {
      ret = mpz_probab_prime_p(this.mpz, reps_);

    } else {
      var t_ = this;

      ret = mpz_probab_prime_p(t_.mpz, reps_);
    }

    return ret.safeCast(int);
  }

  proc bigint.nextprime(const ref a: bigint) {
    if _local {
      mpz_nextprime(this.mpz, a.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
      mpz_nextprime(this.mpz, a.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var a_ = a;

        mpz_nextprime(this.mpz, a_.mpz);
      }
    }
  }



  // gcd
  proc bigint.gcd(const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_gcd(this.mpz, a.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
      mpz_gcd(this.mpz, a.mpz, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var a_ = a;
        var b_ = b;

        mpz_gcd(this.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  proc bigint.gcd(const ref a: bigint, b: int) {
    if b >= 0 {
      this.gcd(a, b : uint);

    } else {
      this.gcd(a, new bigint(b));
    }
  }

  proc bigint.gcd(const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_gcd_ui(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
      mpz_gcd_ui(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var a_ = a;

        mpz_gcd_ui(this.mpz, a_.mpz, b_);
      }
    }
  }

  // sets this to gcd(a,b)
  // set s and t to to coefficients satisfying a*s + b*t == g
  proc bigint.gcdext(ref s: bigint,
                     ref t: bigint,
                     const ref a: bigint,
                     const ref b: bigint) {

    if _local {
      mpz_gcdext(this.mpz, s.mpz, t.mpz, a.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              s.localeId    == chpl_nodeID &&
              t.localeId    == chpl_nodeID &&
              a.localeId    == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
      mpz_gcdext(this.mpz, s.mpz, t.mpz, a.mpz, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var s_ = s;
        var t_ = t;
        var a_ = a;
        var b_ = b;

        mpz_gcdext(this.mpz, s_.mpz, t_.mpz, a_.mpz, b_.mpz);

        s = s_;
        t = t_;
      }
    }
  }



  // lcm
  proc bigint.lcm(const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_lcm(this.mpz, a.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
      mpz_lcm(this.mpz, a.mpz, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var a_ = a;
        var b_ = b;

        mpz_lcm(this.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  proc bigint.lcm(const ref a: bigint, b: int) {
    var b_ : c_ulong;

    if b >= 0 then
      b_ = b.safeCast(c_ulong);
    else
      b_ = (0 - b).safeCast(c_ulong);

    if _local {
      mpz_lcm_ui(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
      mpz_lcm_ui(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var a_ = a;

        mpz_lcm_ui(this.mpz, a_.mpz, b_);
      }
    }
  }

  proc bigint.lcm(const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_lcm_ui(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
      mpz_lcm_ui(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var a_ = a;

        mpz_lcm_ui(this.mpz, a_.mpz, b_);
      }
    }
  }



  // invert
  proc bigint.invert(const ref a: bigint, const ref b: bigint) : int {
    var ret: c_int;

    if _local {
      ret = mpz_invert(this.mpz, a.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
      ret = mpz_invert(this.mpz, a.mpz, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var a_ = a;
        var b_ = b;

        ret = mpz_invert(this.mpz, a_.mpz, b_.mpz);
      }
    }

    return ret.safeCast(int);
  }


  // remove
  proc bigint.remove(const ref a: bigint, const ref f: bigint) : uint {
    var ret: c_ulong;

    if _local {
      ret = mpz_remove(this.mpz, a.mpz, f.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID &&
              f.localeId    == chpl_nodeID {
      ret = mpz_remove(this.mpz, a.mpz, f.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var a_ = a;
        var f_ = f;

        ret = mpz_remove(this.mpz, a_.mpz, f_.mpz);
      }
    }

    return ret.safeCast(uint);
  }


  // Factorial
  proc bigint.fac(a: integral) {
    const a_ = a.safeCast(c_ulong);

    if _local {
      mpz_fac_ui(this.mpz, a_);

    } else if this.localeId == chpl_nodeID {
      mpz_fac_ui(this.mpz, a_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_fac_ui(this.mpz, a_);
      }
    }
  }



  // Binomial
  proc bigint.bin(const ref n: bigint, k: integral) {
    const k_ = k.safeCast(c_ulong);

    if _local {
      mpz_bin_ui(this.mpz, n.mpz, k_);

    } else if this.localeId == chpl_nodeID && n.localeId == chpl_nodeID {
      mpz_bin_ui(this.mpz, n.mpz, k_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var n_ = n;

        mpz_bin_ui(this.mpz, n_.mpz, k_);
      }
    }
  }

  proc bigint.bin(n: uint, k: integral) {
    if n >= 0 {
      const n_ = n.safeCast(c_ulong);
      const k_ = k.safeCast(c_ulong);

      if _local {
        mpz_bin_uiui(this.mpz, n_, k_);

      } else if this.localeId == chpl_nodeID {
        mpz_bin_uiui(this.mpz, n_, k_);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          mpz_bin_uiui(this.mpz, n_, k_);
        }
      }
    } else {
      this.bin(new bigint(n), k);
    }
  }



  // Fibonacci
  proc bigint.fib(n: integral) {
    const n_ = n.safeCast(c_ulong);

    if _local {
      mpz_fib_ui(this.mpz, n_);

    } else if this.localeId == chpl_nodeID {
      mpz_fib_ui(this.mpz, n_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_fib_ui(this.mpz, n_);
      }
    }
  }

  proc bigint.fib2(ref fnsub1: bigint, n: integral) {
    const n_ = n.safeCast(c_ulong);

    if _local {
      mpz_fib2_ui(this.mpz, fnsub1.mpz, n_);

    } else if this.localeId == chpl_nodeID && fnsub1.localeId == chpl_nodeID {
      mpz_fib2_ui(this.mpz, fnsub1.mpz, n_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var fnsub1_ : bigint;

        mpz_fib2_ui(this.mpz, fnsub1_.mpz, n_);

        fnsub1 = fnsub1_;
      }
    }
  }



  // Lucas Number
  proc bigint.lucnum(n: integral) {
    const n_ = n.safeCast(c_ulong);

    if _local {
      mpz_lucnum_ui(this.mpz, n_);

    } else if _local || this.localeId == chpl_nodeID {
      mpz_lucnum_ui(this.mpz, n_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_lucnum_ui(this.mpz, n_);
      }
    }
  }

  proc bigint.lucnum2(ref fnsub1: bigint, n: integral) {
    const n_ = n.safeCast(c_ulong);

    if _local {
      mpz_lucnum2_ui(this.mpz, fnsub1.mpz, n_);

    } else if this.localeId == chpl_nodeID && fnsub1.localeId == chpl_nodeID {
      mpz_lucnum2_ui(this.mpz, fnsub1.mpz, n_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var fnsub1_ : bigint;

        mpz_lucnum2_ui(this.mpz, fnsub1_.mpz, n_);

        fnsub1 = fnsub1_;
      }
    }
  }



  // Bit operations
  proc bigint.popcount() : uint {
    var ret: c_ulong;

    if _local {
      ret = mpz_popcount(this.mpz);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_popcount(this.mpz);

    } else {
      const t_ = this;

      ret = mpz_popcount(t_.mpz);
    }

    return ret.safeCast(uint);
  }

  proc bigint.hamdist(const ref b: bigint) : uint {
    var ret: c_ulong;

    if _local {
      ret = mpz_hamdist(this.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      ret = mpz_hamdist(this.mpz, b.mpz);

    } else {
      const t_ = this;
      const b_ = b;

      ret = mpz_hamdist(t_.mpz, b_.mpz);
    }

    return ret.safeCast(uint);
  }

  proc bigint.scan0(starting_bit: integral) : uint {
    const sb_ = starting_bit.safeCast(c_ulong);
    var   ret: c_ulong;

    if _local {
      ret = mpz_scan0(this.mpz, sb_);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_scan0(this.mpz, sb_);

    } else {
      const t_ = this;

      ret = mpz_scan0(t_.mpz,   sb_);
    }

    return ret.safeCast(uint);
  }

  proc bigint.scan1(starting_bit: integral) : uint {
    const sb_ = starting_bit.safeCast(c_ulong);
    var   ret: c_ulong;

    if _local {
      ret = mpz_scan1(this.mpz, sb_);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_scan1(this.mpz, sb_);

    } else {
      const t_ = this;

      ret = mpz_scan1(t_.mpz,   sb_);
    }

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
    const bi_ = bit_index.safeCast(c_ulong);
    var  ret: c_int;

    if _local {
      ret = mpz_tstbit(this.mpz, bi_);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_tstbit(this.mpz, bi_);

    } else {
      var t_ = this;

      ret = mpz_tstbit(t_.mpz, bi_);
    }

    return ret.safeCast(int);
  }



  // Miscellaneous Functions
  proc bigint.fits_ulong_p() : int {
    var ret: c_int;

    if _local {
      ret = mpz_fits_ulong_p(this.mpz);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_fits_ulong_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_fits_ulong_p(t_.mpz);
    }

    return ret.safeCast(int);
  }

  proc bigint.fits_slong_p() : int {
    var ret: c_int;

    if _local {
      ret = mpz_fits_slong_p(this.mpz);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_fits_slong_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_fits_slong_p(t_.mpz);
    }

    return ret.safeCast(int);
  }

  proc bigint.fits_uint_p() : int {
    var ret: c_int;

    if _local {
      ret = mpz_fits_uint_p(this.mpz);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_fits_uint_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_fits_uint_p(t_.mpz);
    }

    return ret.safeCast(int);
  }

  proc bigint.fits_sint_p() : int {
    var ret: c_int;

    if _local {
      ret = mpz_fits_sint_p(this.mpz);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_fits_sint_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_fits_sint_p(t_.mpz);
    }

    return ret.safeCast(int);
  }

  proc bigint.fits_ushort_p() : int {
    var ret: c_int;

    if _local {
      ret = mpz_fits_ushort_p(this.mpz);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_fits_ushort_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_fits_ushort_p(t_.mpz);
    }

    return ret.safeCast(int);
  }

  proc bigint.fits_sshort_p() : int {
    var ret: c_int;

    if _local {
      ret = mpz_fits_sshort_p(this.mpz);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_fits_sshort_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_fits_sshort_p(t_.mpz);
    }

    return ret.safeCast(int);
  }

  proc bigint.even_p() : int {
    var ret: c_int;

    if _local {
      ret = mpz_even_p(this.mpz);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_even_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_even_p(t_.mpz);
    }

    return ret.safeCast(int);
  }

  proc bigint.odd_p() : int {
    var ret: c_int;

    if _local {
      ret = mpz_odd_p(this.mpz);

    } else if this.localeId == chpl_nodeID {
      ret = mpz_odd_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_odd_p(t_.mpz);
    }

    return ret.safeCast(int);
  }



  //
  // 5.5 Arithmetic functions
  //

  proc bigint.add(const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_add(this.mpz, a.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
      mpz_add(this.mpz, a.mpz, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;
        const b_ = b;

        mpz_add(this.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  proc bigint.add(const ref a: bigint, b: int) {
    if b >= 0 {
      const b_ = b.safeCast(c_ulong);

      if _local {
        mpz_add_ui(this.mpz, a.mpz, b_);

      } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
        mpz_add_ui(this.mpz, a.mpz, b_);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          const a_ = a;

          mpz_add_ui(this.mpz, a_.mpz, b_);
        }
      }
    } else {
      const b_ = (0 - b).safeCast(c_ulong);

      if _local {
        mpz_sub_ui(this.mpz, a.mpz, b_);

      } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
        mpz_sub_ui(this.mpz, a.mpz, b_);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          const a_ = a;

          mpz_sub_ui(this.mpz, a_.mpz, b_);
        }
      }
    }
  }

  proc bigint.add(const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_add_ui(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
      mpz_add_ui(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_add_ui(this.mpz, a_.mpz, b_);
      }
    }
  }

  proc bigint.sub(const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_sub(this.mpz, a.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
      mpz_sub(this.mpz, a.mpz, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;
        const b_ = b;

        mpz_sub(this.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  proc bigint.sub(const ref a: bigint, b: int) {
    if b >= 0 {
      const b_ = b.safeCast(c_ulong);

      if _local {
        mpz_sub_ui(this.mpz, a.mpz, b_);

      } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
        mpz_sub_ui(this.mpz, a.mpz, b_);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          const a_ = a;

          mpz_sub_ui(this.mpz, a_.mpz, b_);
        }
      }
    } else {
      const b_ = (0 - b).safeCast(c_ulong);

      if _local {
        mpz_add_ui(this.mpz, a.mpz, b_);

      } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
        mpz_add_ui(this.mpz, a.mpz, b_);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          const a_ = a;

          mpz_add_ui(this.mpz, a_.mpz, b_);
        }
      }
    }
  }

  proc bigint.sub(const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_sub_ui(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
      mpz_sub_ui(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_sub_ui(this.mpz, a_.mpz, b_);
      }
    }
  }

  proc bigint.sub(a: int, const ref b: bigint) {
    if a >= 0 {
      const a_ = a.safeCast(c_ulong);

      if _local {
        mpz_ui_sub(this.mpz, a_, b.mpz);

      } else if this.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
        mpz_ui_sub(this.mpz, a_, b.mpz);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          const b_ = b;

          mpz_ui_sub(this.mpz, a_, b_.mpz);
        }
      }
    } else {
      const a_ = (0 - a).safeCast(c_ulong);

      if _local {
        mpz_add_ui(this.mpz, b.mpz, a_);
        mpz_neg(this.mpz, this.mpz);

      } else if this.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
        mpz_add_ui(this.mpz, b.mpz, a_);
        mpz_neg(this.mpz, this.mpz);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          const b_ = b;

          mpz_add_ui(this.mpz, b_.mpz, a_);
          mpz_neg(this.mpz, this.mpz);
        }
      }
    }
  }

  proc bigint.sub(a: uint, const ref b: bigint) {
    const a_ = a.safeCast(c_ulong);

    if _local {
      mpz_ui_sub(this.mpz, a_, b.mpz);

    } else if this.localeId == chpl_nodeID && b.localeId == chpl_nodeID {
      mpz_ui_sub(this.mpz, a_, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const b_ = b;

        mpz_ui_sub(this.mpz, a_, b_.mpz);
      }
    }
  }

  proc bigint.mul(const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_mul(this.mpz, a.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
      mpz_mul(this.mpz, a.mpz, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;
        const b_ = b;

        mpz_mul(this.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  proc bigint.mul(const ref a: bigint, b: int) {
    const b_ = b.safeCast(c_long);

    if _local {
      mpz_mul_si(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
      mpz_mul_si(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_mul_si(this.mpz, a_.mpz, b_);
      }
    }
  }

  proc bigint.mul(const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_mul_ui(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
      mpz_mul_ui(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_mul_ui(this.mpz, a_.mpz, b_);
      }
    }
  }

  proc bigint.addmul(const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_addmul(this.mpz, a.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
      mpz_addmul(this.mpz, a.mpz, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;
        const b_ = b;

        mpz_addmul(this.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  proc bigint.addmul(const ref a: bigint, b: int) {
    if b >= 0 {
      const b_ = b.safeCast(c_ulong);

      if _local {
        mpz_addmul_ui(this.mpz, a.mpz, b_);

      } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
        mpz_addmul_ui(this.mpz, a.mpz, b_);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          const a_ = a;

          mpz_addmul_ui(this.mpz, a_.mpz, b_);
        }
      }
    } else {
      const b_ = (0 - b).safeCast(c_ulong);

      if _local {
        mpz_submul_ui(this.mpz, a.mpz, b_);

      } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
        mpz_submul_ui(this.mpz, a.mpz, b_);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          const a_ = a;

          mpz_submul_ui(this.mpz, a_.mpz, b_);
        }
      }
    }
  }

  proc bigint.addmul(const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_addmul_ui(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
      mpz_addmul_ui(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_addmul_ui(this.mpz, a_.mpz, b_);
      }
    }
  }

  proc bigint.submul(const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_submul(this.mpz, a.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
      mpz_submul(this.mpz, a.mpz, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;
        const b_ = b;

        mpz_submul(this.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  proc bigint.submul(const ref a: bigint, b: int) {
    if b >= 0 {
      const b_ = b.safeCast(c_ulong);

      if _local {
        mpz_submul_ui(this.mpz, a.mpz, b_);

      } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
        mpz_submul_ui(this.mpz, a.mpz, b_);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          const a_ = a;

          mpz_submul_ui(this.mpz, a_.mpz, b_);
        }
      }
    } else {
      const b_ = (0 - b).safeCast(c_ulong);

      if _local {
        mpz_addmul_ui(this.mpz, a.mpz, b_);

      } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
        mpz_addmul_ui(this.mpz, a.mpz, b_);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          const a_ = a;

          mpz_addmul_ui(this.mpz, a_.mpz, b_);
        }
      }
    }
  }

  proc bigint.submul(const ref a: bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_submul_ui(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
      mpz_submul_ui(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_submul_ui(this.mpz, a_.mpz, b_);
      }
    }
  }

  proc bigint.mul_2exp(const ref a: bigint, b: integral) {
    const b_ = b.safeCast(mp_bitcnt_t);

    if _local {
      mpz_mul_2exp(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
      mpz_mul_2exp(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_mul_2exp(this.mpz, a_.mpz, b_);
      }
    }
  }

  proc bigint.neg(const ref a: bigint) {
    if _local {
      mpz_neg(this.mpz, a.mpz);

    } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
      mpz_neg(this.mpz, a.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_neg(this.mpz, a_.mpz);
      }
    }
  }

  proc bigint.abs(const ref a: bigint) {
    if _local {
      mpz_abs(this.mpz, a.mpz);

    } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
      mpz_abs(this.mpz, a.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_abs(this.mpz, a_.mpz);
      }
    }
  }

  // 5.6 Division Functions
  proc bigint.div_q(const ref n: bigint,
                    const ref d: bigint,
                    param     rounding = Round.ZERO) {
    if _local {
      select rounding {
        when Round.UP   do mpz_cdiv_q(this.mpz, n.mpz,  d.mpz);
        when Round.DOWN do mpz_fdiv_q(this.mpz, n.mpz,  d.mpz);
        when Round.ZERO do mpz_tdiv_q(this.mpz, n.mpz,  d.mpz);
      }

    } else if this.localeId == chpl_nodeID &&
              n.localeId    == chpl_nodeID &&
              d.localeId    == chpl_nodeID {
      select rounding {
        when Round.UP   do mpz_cdiv_q(this.mpz, n.mpz,  d.mpz);
        when Round.DOWN do mpz_fdiv_q(this.mpz, n.mpz,  d.mpz);
        when Round.ZERO do mpz_tdiv_q(this.mpz, n.mpz,  d.mpz);
      }

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const n_ = n;
        const d_ = d;

        select rounding {
          when Round.UP   do mpz_cdiv_q(this.mpz, n_.mpz, d_.mpz);
          when Round.DOWN do mpz_fdiv_q(this.mpz, n_.mpz, d_.mpz);
          when Round.ZERO do mpz_tdiv_q(this.mpz, n_.mpz, d_.mpz);
          }
      }
    }
  }

  proc bigint.div_q(const ref n: bigint,
                              d: integral,
                    param     rounding = Round.ZERO) {

    this.div_q(n, new bigint(d), rounding);
  }

  proc bigint.div_r(const ref n: bigint,
                    const ref d: bigint,
                    param     rounding = Round.ZERO) {
    if _local {
      select rounding {
        when Round.UP   do mpz_cdiv_r(this.mpz, n.mpz,  d.mpz);
        when Round.DOWN do mpz_fdiv_r(this.mpz, n.mpz,  d.mpz);
        when Round.ZERO do mpz_tdiv_r(this.mpz, n.mpz,  d.mpz);
      }

    } else if this.localeId == chpl_nodeID &&
              n.localeId    == chpl_nodeID &&
              d.localeId    == chpl_nodeID {
      select rounding {
        when Round.UP   do mpz_cdiv_r(this.mpz, n.mpz,  d.mpz);
        when Round.DOWN do mpz_fdiv_r(this.mpz, n.mpz,  d.mpz);
        when Round.ZERO do mpz_tdiv_r(this.mpz, n.mpz,  d.mpz);
      }

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const n_ = n;
        const d_ = d;

        select rounding {
          when Round.UP   do mpz_cdiv_r(this.mpz, n_.mpz, d_.mpz);
          when Round.DOWN do mpz_fdiv_r(this.mpz, n_.mpz, d_.mpz);
          when Round.ZERO do mpz_tdiv_r(this.mpz, n_.mpz, d_.mpz);
        }
      }
    }
  }

  proc bigint.div_r(const ref n: bigint,
                              d: integral,
                    param     rounding = Round.ZERO) {
    this.div_r(n, new bigint(d), rounding);
  }

  // this gets quotient, r gets remainder
  proc bigint.div_qr(ref       r:        bigint,
                     const ref n:        bigint,
                     const ref d:        bigint,
                     param     rounding = Round.ZERO) {
    if _local {
      select rounding {
        when Round.UP   do mpz_cdiv_qr(this.mpz, r.mpz, n.mpz, d.mpz);
        when Round.DOWN do mpz_fdiv_qr(this.mpz, r.mpz, n.mpz, d.mpz);
        when Round.ZERO do mpz_tdiv_qr(this.mpz, r.mpz, n.mpz, d.mpz);
      }

    } else if this.localeId == chpl_nodeID &&
              r.localeId    == chpl_nodeID &&
              n.localeId    == chpl_nodeID &&
              d.localeId    == chpl_nodeID {
      select rounding {
        when Round.UP   do mpz_cdiv_qr(this.mpz, r.mpz, n.mpz, d.mpz);
        when Round.DOWN do mpz_fdiv_qr(this.mpz, r.mpz, n.mpz, d.mpz);
        when Round.ZERO do mpz_tdiv_qr(this.mpz, r.mpz, n.mpz, d.mpz);
      }

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var   r_ = r;
        const n_ = n;
        const d_ = d;

        select rounding {
          when Round.UP   do mpz_cdiv_qr(this.mpz, r_.mpz, n_.mpz, d_.mpz);
          when Round.DOWN do mpz_fdiv_qr(this.mpz, r_.mpz, n_.mpz, d_.mpz);
          when Round.ZERO do mpz_tdiv_qr(this.mpz, r_.mpz, n_.mpz, d_.mpz);
        }

        r = r_;
      }
    }
  }

  proc bigint.div_qr(ref       r: bigint,
                     const ref n: bigint,
                               d: integral,
                     param     rounding = Round.ZERO) {
    this.div_qr(r, n, new bigint(d), rounding);
  }

  proc bigint.div_q_2exp(const ref n: bigint,
                                   b: integral,
                         param     rounding = Round.ZERO) {
    const b_ = b.safeCast(mp_bitcnt_t);

    if _local {
      select rounding {
        when Round.UP   do mpz_cdiv_q_2exp(this.mpz, n.mpz, b_);
        when Round.DOWN do mpz_fdiv_q_2exp(this.mpz, n.mpz, b_);
        when Round.ZERO do mpz_tdiv_q_2exp(this.mpz, n.mpz, b_);
      }

    } else if this.localeId == chpl_nodeID &&
              n.localeId    == chpl_nodeID {
      select rounding {
        when Round.UP   do mpz_cdiv_q_2exp(this.mpz, n.mpz, b_);
        when Round.DOWN do mpz_fdiv_q_2exp(this.mpz, n.mpz, b_);
        when Round.ZERO do mpz_tdiv_q_2exp(this.mpz, n.mpz, b_);
      }

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const n_ = n;

        select rounding {
          when Round.UP   do mpz_cdiv_q_2exp(this.mpz, n_.mpz, b_);
          when Round.DOWN do mpz_fdiv_q_2exp(this.mpz, n_.mpz, b_);
          when Round.ZERO do mpz_tdiv_q_2exp(this.mpz, n_.mpz, b_);
        }
      }
    }
  }

  proc bigint.div_r_2exp(const ref n: bigint,
                                   b: integral,
                         param     rounding = Round.ZERO) {
    const b_ = b.safeCast(mp_bitcnt_t);

    if _local {
      select rounding {
        when Round.UP   do mpz_cdiv_r_2exp(this.mpz, n.mpz, b_);
        when Round.DOWN do mpz_fdiv_r_2exp(this.mpz, n.mpz, b_);
        when Round.ZERO do mpz_tdiv_r_2exp(this.mpz, n.mpz, b_);
      }

    } else if this.localeId == chpl_nodeID && n.localeId == chpl_nodeID {
      select rounding {
        when Round.UP   do mpz_cdiv_r_2exp(this.mpz, n.mpz, b_);
        when Round.DOWN do mpz_fdiv_r_2exp(this.mpz, n.mpz, b_);
        when Round.ZERO do mpz_tdiv_r_2exp(this.mpz, n.mpz, b_);
      }

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const n_ = n;

        select rounding {
          when Round.UP   do mpz_cdiv_r_2exp(this.mpz, n_.mpz, b_);
          when Round.DOWN do mpz_fdiv_r_2exp(this.mpz, n_.mpz, b_);
          when Round.ZERO do mpz_tdiv_r_2exp(this.mpz, n_.mpz, b_);
        }
      }
    }
  }



  proc bigint.mod(const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_mod(this.mpz, a.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
      mpz_mod(this.mpz, a.mpz, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;
        const b_ = b;

        mpz_mod(this.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  proc bigint.mod(const ref a: bigint, b: integral) : uint {
    var   b_ : c_ulong;
    var   ret: c_ulong;

    if b >= 0 then
      b_ = b.safeCast(c_ulong);
    else
      b_ = (0 - b).safeCast(c_ulong);

    if _local {
      ret = mpz_mod_ui(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
      ret = mpz_mod_ui(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        ret = mpz_mod_ui(this.mpz, a_.mpz, b_);
      }
    }

    return ret.safeCast(uint);
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
        var b_ = b;

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
  proc bigint.and(const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_and(this.mpz, a.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
      mpz_and(this.mpz, a.mpz, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;
        const b_ = b;

        mpz_and(this.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  proc bigint.ior(const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_ior(this.mpz, a.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
      mpz_ior(this.mpz, a.mpz, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;
        const b_ = b;

        mpz_ior(this.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  proc bigint.xor(const ref a: bigint, const ref b: bigint) {
    if _local {
      mpz_xor(this.mpz, a.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
      mpz_xor(this.mpz, a.mpz, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;
        const b_ = b;

        mpz_xor(this.mpz, a_.mpz, b_.mpz);
      }
    }
  }

  proc bigint.com(const ref a: bigint) {
    if _local {
      mpz_com(this.mpz, a.mpz);

    } else if this.localeId == chpl_nodeID && a.localeId == chpl_nodeID {
      mpz_com(this.mpz, a.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_com(this.mpz, a_.mpz);
      }
    }
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
        const mpz_struct = a.mpzStruct();

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
}
