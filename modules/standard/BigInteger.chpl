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
Support for GNU Multiple Precision Integer Arithmetic through the Bigint record

This module implements an interface with the GMP library (the GNU Multiple
Precision Arithmetic Library). See the `GMP homepage <https://gmplib.org/>`
for more information on this library.

See the GMP Chapel module for more information on how to use GMP with Chapel.

Using the Bigint record
-----------------------

The BigInteger Chapel module provides a :record:`Bigint` record wrapping GMP
integers. At the present time, only the functions for ``mpz`` (ie signed
integer) GMP types are supported with :record:`Bigint`; future work will be to
extend this support to floating-point types.

:record:`Bigint` methods all wrap GMP functions with obviously similar names.
The :record:`Bigint` methods are locale aware - so Chapel programs can create
a distributed array of GMP numbers. The method of :record:`Bigint` objects are
setting the receiver, so e.g. myBigint.add(x,y) sets myBigint to ``x + y``.

A code example::

 use BigInteger;

 // initialize a GMP value, set it to zero
 var a = new Bigint();

 a.fac(100);                            // set a to 100!
 writeln(a);                            // output   100!

 // initialize from a decimal string
 var b = new Bigint("48473822929893829847");

 b.add(b, 1);                           // add one to b
*/

module BigInteger {
  use GMP;

  /*
    The Bigint record provides arbitrary length integers and a set of
    operator overloads that allow Bigints to be treated consistently
    with conventional fixed width integers.

    The current implementation relies on the GMP library.  This is a
    conventional C-based library and hence does not have direct support
    for multi-locale programs.  The GMP API is centered on the use of
    side-effecting operations on an "object" of type mpz_t.

    The primary benefits of this module include

      1) Support for multi-locale programs

      2) A set of operator overloads

      3) Automatic memory management of GMP data structures

    Wrapping an mpz_t in a Bigint record does not appear to introduce
    a measurable performance impact.  However in the fall of 2016 the
    Chapel operator-based API may introduce a measurable overhead compared
    to the GMP-native API for some applications.  Therefore this module also
    provides lower-level multi-locale aware wrappers for the GMP functions.

    Methods on Bigint are defined for Chapel types e.g int(64) and uint(64)
    which must be converted to underlying C types for use by GMP.  Runtime
    checks are used to ensure the Chapel types can safely be cast to the C
    types (e.g. when casting a Chapel uint it checks that it fits in the C
    ulong which could be a 32 bit type if running on linux32 platform).

    The checks are controlled by the compiler options ``--[no-]cast-checks``,
    ``--fast``, etc.
   */

  pragma "ignore noinit"
  record Bigint {
    var mpz      : mpz_t;              // A dynamic-vector of C integers
    var localeId : chpl_nodeID_t;      // The locale id for the GMP state

    proc Bigint() {
      mpz_init(this.mpz);

      this.localeId = chpl_nodeID;
    }

    proc Bigint(num: Bigint) {
      if _local || num.localeId == chpl_nodeID {
        mpz_init_set(this.mpz, num.mpz);
      } else {
        var mpz_struct = num.mpzStruct();

        mpz_init(this.mpz);

        chpl_gmp_get_mpz(this.mpz, num.localeId, mpz_struct);
      }

      this.localeId = chpl_nodeID;
    }

    proc Bigint(num: int) {
      mpz_init_set_si(this.mpz, num.safeCast(c_long));

      this.localeId = chpl_nodeID;
    }

    proc Bigint(num: uint) {
      mpz_init_set_ui(this.mpz, num.safeCast(c_ulong));

      this.localeId = chpl_nodeID;
    }

    proc Bigint(str: string, base: int = 0) {
      var e = mpz_init_set_str(this.mpz,
                               str.localize().c_str(),
                               base.safeCast(c_int));
      if e != 0 {
        mpz_clear(this.mpz);

        halt("Error initializing big integer: bad format");
      }

      this.localeId = chpl_nodeID;
    }

    proc Bigint(str: string, base: int = 0, out error: syserr) {
      var e = mpz_init_set_str(this.mpz,
                               str.localize().c_str(),
                               base.safeCast(c_int));
      if e != 0 {
        mpz_clear(this.mpz);

        error = EFORMAT;
      } else {
        error = ENOERR;
      }

      this.localeId = chpl_nodeID;
    }

    // Within a given locale, Bigint assignment creates a deep copy of the
    // data and so the record "owns" the GMP data.
    //
    // If a Bigint is copied to a remote node then it will receive a shallow
    // copy.  The localeId points back the correct locale but the mpz field
    // is meaningless.
    proc ~Bigint() {
      if _local || this.localeId == chpl_nodeID {
        mpz_clear(this.mpz);
      }
    }

    proc size() : size_t {
      var ret: size_t;

      if _local || this.localeId == chpl_nodeID {
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

      if _local || this.localeId == chpl_nodeID {
        ret = mpz_sizeinbase(this.mpz, base_);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          ret = mpz_sizeinbase(this.mpz, base_);
        }
      }

      return ret;
    }

    proc numLimbs : uint(64) {
      var mpz_struct = this.mpz[1];

      return chpl_gmp_mpz_nlimbs(mpz_struct);
    }

    proc get_limbn(n: uint) : uint {
      const n_ = n.safeCast(mp_size_t);
      var   ret: mp_limb_t;


      if _local || this.localeId == chpl_nodeID {
        ret = mpz_getlimbn(this.mpz, n_);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          ret = mpz_getlimbn(this.mpz, n_);
        }
      }

      return ret.safeCast(uint);
    }

    proc mpzStruct() : __mpz_struct {
      var ret: __mpz_struct;

      if _local || this.localeId == chpl_nodeID {
        ret = this.mpz[1];

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          ret = this.mpz[1];
        }
      }

      return ret;
    }

    proc get_si() : int {
      var ret: c_long;

      if _local || this.localeId == chpl_nodeID {
        ret = mpz_get_si(this.mpz);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          ret = mpz_get_si(this.mpz);
        }
      }

      return ret.safeCast(int);
    }

    proc get_ui() : uint {
      var ret: c_ulong;

      if _local || this.localeId == chpl_nodeID {
        ret = mpz_get_ui(this.mpz);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          ret = mpz_get_ui(this.mpz);
        }
      }

      return ret.safeCast(uint);
    }

    proc get_d() : real {
      var ret: c_double;

      if _local || this.localeId == chpl_nodeID {
        ret = mpz_get_d(this.mpz);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          ret = mpz_get_d(this.mpz);
        }
      }

      return ret : real;
    }

    proc get_d_2exp() : (uint(32), real) {
      var exp: c_long;
      var dbl: c_double;

      if _local || this.localeId == chpl_nodeID {
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

      if _local || this.localeId == chpl_nodeID {
        var tmpvar = chpl_gmp_mpz_get_str(base_, this.mpz);

        ret = new string(tmpvar, owned = true, needToCopy = false);

      } else {
        const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", thisLoc) {
          var tmpvar = chpl_gmp_mpz_get_str(base_, this.mpz);

          ret = new string(tmpvar, owned = true, needToCopy = false);
        }
      }

      return ret;
    }

    proc writeThis(writer) {
      var s: string;

      if _local || this.localeId == chpl_nodeID {
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

  pragma "init copy fn"
  pragma "no doc"
  proc chpl__initCopy(bir: Bigint) {
    var ret : Bigint;

    if _local || bir.localeId == chpl_nodeID {
      mpz_set(ret.mpz, bir.mpz);
    } else {
      var mpz_struct = bir.mpzStruct();

      chpl_gmp_get_mpz(ret.mpz, bir.localeId, mpz_struct);
    }

    return ret;
  }

  pragma "donor fn"
  pragma "auto copy fn"
  pragma "no doc"
  proc chpl__autoCopy(bir: Bigint) {
    var ret : Bigint;

    if _local || bir.localeId == chpl_nodeID {
      mpz_set(ret.mpz, bir.mpz);
    } else {
      ret.mpz      = bir.mpz;
      ret.localeId = bir.localeId;
    }

    return ret;
  }

  //
  // Locale-aware assignment
  //

  proc =(ref lhs: Bigint, rhs: Bigint) {
    inline proc helper() {
      if _local || rhs.localeId == chpl_nodeID {
        mpz_set(lhs.mpz, rhs.mpz);

      } else {
        chpl_gmp_get_mpz(lhs.mpz, rhs.localeId, rhs.mpz[1]);
      }
    }

    if _local || lhs.localeId == chpl_nodeID {
      helper();

    } else {
      var lhsLoc = chpl_buildLocaleID(lhs.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", lhsLoc) {
        helper();
      }
    }
  }

  proc =(ref lhs: Bigint, rhs: int) {
    if _local || lhs.localeId == chpl_nodeID {
      mpz_set_si(lhs.mpz, rhs.safeCast(c_long));

    } else {
      var lhsLoc = chpl_buildLocaleID(lhs.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", lhsLoc) {
        mpz_set_si(lhs.mpz, rhs.safeCast(c_long));
      }
    }
  }

  proc =(ref lhs: Bigint, rhs: uint) {
    if _local || lhs.localeId == chpl_nodeID {
      mpz_set_si(lhs.mpz, rhs.safeCast(c_long));

    } else {
      var lhsLoc = chpl_buildLocaleID(lhs.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", lhsLoc) {
        mpz_set_ui(lhs.mpz, rhs.safeCast(c_ulong));
      }
    }
  }

  //
  // Operations on Bigints
  //
  // In general we need to think about 3 cases
  //
  //   1) This is a single-locale configuration.
  //      We can invoke the appropriate mpz operator directly.
  //
  //   2) All Bigints are on the current locale.
  //      We can invoke the appropriate mpz operator directly.
  //
  //   3) One or more Bigints are on a remote locale.
  //      This is complicated.  It is tempting to handle all of the
  //      permutations as efficiently as possible but this introduces
  //      a lot of cases esp. for binary operations.
  //
  //      Instead we use just one handler to
  //        a) Move to this.localeId
  //        b) Use assignment operator to copy operands to current locale
  //        c) Invoke the appropriate mpz operator directly
  //
  //      Later we can use profiling to add special cases where appropriate
  //

  //
  // Unary operators
  //
  proc +(a: Bigint) {
    return new Bigint(a);
  }

  proc -(a: Bigint) {
    var c = new Bigint(a);

    mpz_neg(c.mpz, c.mpz);

    return c;
  }

  proc ~(a: Bigint) {
    var c = new Bigint(a);

    mpz_com(c.mpz, c.mpz);

    return c;
  }

  //
  // Binary operators
  //

  // Addition
  proc +(a: Bigint, b: Bigint) {
    var c = new Bigint();

    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      mpz_add(c.mpz, a.mpz, b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      mpz_add(c.mpz, a_.mpz, b_.mpz);
    }

    return c;
  }

  proc +(a: Bigint, b: int) {
    var c = new Bigint();

    if b >= 0 {
      const b_ = b.safeCast(c_ulong);

      if _local || a.localeId == chpl_nodeID {
        mpz_add_ui(c.mpz, a.mpz,  b_);

      } else {
        const a_ = a;

        mpz_add_ui(c.mpz, a_.mpz, b_);
      }

    } else {
      const b1 = abs(b);
      const b2 = b1.safeCast(c_ulong);

      if _local || a.localeId == chpl_nodeID {
        mpz_sub_ui(c.mpz, a.mpz, b2);

      } else {
        const a_ = a;

        mpz_sub_ui(c.mpz, a_.mpz, b2);
      }
    }

    return c;
  }

  proc +(a: int, b: Bigint) {
    var c = new Bigint();

    if a >= 0 {
      const a_ = a.safeCast(c_ulong);

      if _local || b.localeId == chpl_nodeID {
        mpz_add_ui(c.mpz, b.mpz,  a_);

      } else {
        const b_ = b;

        mpz_add_ui(c.mpz, b_.mpz, a_);
      }

    } else {
      const a1 = abs(1);
      const a2 = a1.safeCast(c_ulong);

      if _local || b.localeId == chpl_nodeID {
        mpz_sub_ui(c.mpz, b.mpz,  a2);

      } else {
        const b_ = b;

        mpz_sub_ui(c.mpz, b_.mpz, a2);
      }
    }

    return c;
  }

  proc +(a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);
    var   c  = new Bigint();

    if _local || a.localeId == chpl_nodeID {
      mpz_add_ui(c.mpz, a.mpz,  b_);

    } else {
      const a_ = a;

      mpz_add_ui(c.mpz, a_.mpz, b_);
    }

    return c;
  }

  proc +(a: uint, b: Bigint) {
    const a_ = a.safeCast(c_ulong);
    var   c  = new Bigint();

    if _local || b.localeId == chpl_nodeID {
      mpz_add_ui(c.mpz, b.mpz,  a_);

    } else {
      const b_ = b;

      mpz_add_ui(c.mpz, b_.mpz, a_);
    }

    return c;
  }



  // Subtraction
  proc -(a: Bigint, b: Bigint) {
    var c = new Bigint();

    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      mpz_sub(c.mpz, a.mpz,  b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      mpz_sub(c.mpz, a_.mpz, b_.mpz);
    }

    return c;
  }

  proc -(a: Bigint, b: int) {
    var c = new Bigint();

    if b >= 0 {
      const b_ = b.safeCast(c_ulong);

      if _local || a.localeId == chpl_nodeID {
        mpz_sub_ui(c.mpz, a.mpz,  b_);

      } else {
        const a_ = a;

        mpz_sub_ui(c.mpz, a_.mpz, b_);
      }

    } else {
      const b1 = abs(b);
      const b2 = b1.safeCast(c_ulong);

      if _local || a.localeId == chpl_nodeID {
        mpz_add_ui(c.mpz, a.mpz,  b2);

      } else {
        const a_ = a;

        mpz_add_ui(c.mpz, a_.mpz, b2);
      }
    }

    return c;
  }

  proc -(a: int, b: Bigint) {
    var c = new Bigint();

    if a >= 0 {
      const a_ = a.safeCast(c_ulong);

      if _local || b.localeId == chpl_nodeID {
        mpz_ui_sub(c.mpz, a_, b.mpz);

      } else {
        const b_ = b;

        mpz_ui_sub(c.mpz, a_, b_.mpz);
      }

    } else {
      const a1 = abs(1);
      const a2 = a1.safeCast(c_ulong);

      if _local || b.localeId == chpl_nodeID {
        mpz_add_ui(c.mpz, b.mpz,  a2);

      } else {
        const b_ = b;

        mpz_add_ui(c.mpz, b_.mpz, a2);
      }
    }

    return c;
  }

  proc -(a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);
    var   c  = new Bigint();

    if _local || a.localeId == chpl_nodeID {
      mpz_sub_ui(c.mpz, a.mpz,  b_);

    } else {
      const a_ = a;

      mpz_sub_ui(c.mpz, a_.mpz, b_);
    }

    return c;
  }

  proc -(a: uint, b: Bigint) {
    const a_ = a.safeCast(c_ulong);
    var   c  = new Bigint();

    if _local || b.localeId == chpl_nodeID {
      mpz_ui_sub(c.mpz, a_, b.mpz);

    } else {
      const b_ = b;

      mpz_ui_sub(c.mpz, a_, b_.mpz);
    }

    return c;
  }


  // Multiplication
  proc *(a: Bigint, b: Bigint) {
    var c = new Bigint();

    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      mpz_mul(c.mpz, a.mpz, b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      mpz_mul(c.mpz, a_.mpz, b_.mpz);
    }

    return c;
  }

  proc *(a: Bigint, b: int) {
    const b_ = b.safeCast(c_long);
    var   c  = new Bigint();

    if _local || a.localeId == chpl_nodeID {
      mpz_mul_si(c.mpz, a.mpz,  b_);

    } else {
      const a_ = a;

      mpz_mul_si(c.mpz, a_.mpz, b_);
    }

    return c;
  }

  proc *(a: int, b: Bigint) {
    const a_ = a.safeCast(c_long);
    var   c  = new Bigint();

    if _local || b.localeId == chpl_nodeID {
      mpz_mul_si(c.mpz, b.mpz,  a_);

    } else {
      const b_ = b;

      mpz_mul_si(c.mpz, b_.mpz, a_);
    }

    return c;
  }

  proc *(a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);
    var   c  = new Bigint();

    if _local || a.localeId == chpl_nodeID {
      mpz_mul_ui(c.mpz, a.mpz,  b_);

    } else {
      const a_ = a;

      mpz_mul_ui(c.mpz, a_.mpz, b_);
    }

    return c;
  }

  proc *(a: uint, b: Bigint) {
    const a_ = a.safeCast(c_ulong);
    var   c  = new Bigint();

    if _local || b.localeId == chpl_nodeID {
      mpz_mul_ui(c.mpz, b.mpz,  a_);

    } else {
      const b_ = b;

      mpz_mul_ui(c.mpz, b_.mpz, a_);
    }

    return c;
  }



  // Division
  proc /(a: Bigint, b: Bigint) {
    var c = new Bigint();

    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      mpz_tdiv_q(c.mpz, a.mpz, b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      mpz_tdiv_q(c.mpz, a_.mpz, b_.mpz);
    }

    return c;
  }

  proc /(a: Bigint, b: int) {
    var b_ = 0 : c_ulong;
    var c  = new Bigint();

    if b >= 0 then
      b_ = b.safeCast(c_ulong);
    else
      b_ = (0 - b).safeCast(c_ulong);

    if _local || a.localeId == chpl_nodeID {
      mpz_tdiv_q_ui(c.mpz, a.mpz, b_);

    } else {
      const a_ = a;

      mpz_tdiv_q_ui(c.mpz, a_.mpz, b_);
    }

    if b < 0 then
      mpz_neg(c.mpz, c.mpz);

    return c;
  }

  proc /(a: Bigint, b: uint) {
    var b_ = b.safeCast(c_ulong);
    var c  = new Bigint();

    if _local || a.localeId == chpl_nodeID {
      mpz_tdiv_q_ui(c.mpz, a.mpz, b_);

    } else {
      const a_ = a;

      mpz_tdiv_q_ui(c.mpz, a_.mpz, b_);
    }

    return c;
  }

  proc div(param rounding: Round, n: Bigint, d: uint) : uint {
    const d_ = d.safeCast(c_ulong);
    var   ret: c_ulong;

    if _local || n.localeId == chpl_nodeID {
      select rounding {
        when Round.UP   do ret = mpz_cdiv_ui(n.mpz, d_);
        when Round.DOWN do ret = mpz_fdiv_ui(n.mpz, d_);
        when Round.ZERO do ret = mpz_tdiv_ui(n.mpz, d_);
      }

    } else {
      var nLoc = chpl_buildLocaleID(n.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", nLoc) {
        select rounding {
          when Round.UP   do ret = mpz_cdiv_ui(n.mpz, d_);
          when Round.DOWN do ret = mpz_fdiv_ui(n.mpz, d_);
          when Round.ZERO do ret = mpz_tdiv_ui(n.mpz, d_);
        }
      }
    }

    return ret.safeCast(uint);
  }


  // Exponentiation
  proc **(a: Bigint, b: uint) {
    var c = new Bigint();

    if _local || a.localeId == chpl_nodeID {
      mpz_pow_ui(c.mpz, a.mpz,  b);

    } else {
      const a_ = a;

      mpz_pow_ui(c.mpz, a_.mpz, b);
    }

    return c;
  }



  // Mod
  proc %(a: Bigint, b: Bigint) {
    var c = new Bigint();

    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      mpz_mod(c.mpz, a.mpz,  b.mpz);

    } else {
      const a_ = a;

      mpz_mod(c.mpz, a_.mpz, b.mpz);
    }

    return c;
  }

  proc %(a: Bigint, b: int) {
    var b_ = 0 : c_ulong;
    var c  = new Bigint();

    if b >= 0 then
      b_ = b.safeCast(c_ulong);
    else
      b_ = (0 - b).safeCast(c_ulong);

    if _local || a.localeId == chpl_nodeID {
      mpz_mod_ui(c.mpz, a.mpz,  b_);

    } else {
      const a_ = a;

      mpz_mod_ui(c.mpz, a_.mpz, b_);
    }

    return c;
  }

  proc %(a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);
    var   c  = new Bigint();

    if _local || a.localeId == chpl_nodeID {
      mpz_mod_ui(c.mpz, a.mpz,  b_);

    } else {
      const a_ = a;

      mpz_mod_ui(c.mpz, a_.mpz, b_);
    }

    return c;
  }



  // Bit-shift left
  proc <<(a: Bigint, b: int) {
    var c = new Bigint();

    if b >= 0 {
      const b_ = b.safeCast(c_ulong);

      if _local || a.localeId == chpl_nodeID {
        mpz_mul_2exp(c.mpz, a.mpz,  b_);

      } else {
        const a_ = a;

        mpz_mul_2exp(c.mpz, a_.mpz, b_);
      }

    } else {
      const b_ = (0 - b).safeCast(c_ulong);

      if _local || a.localeId == chpl_nodeID {
        mpz_tdiv_q_2exp(c.mpz, a.mpz,  b_);

      } else {
        const a_ = a;

        mpz_tdiv_q_2exp(c.mpz, a_.mpz, b_);
      }
    }

    return c;
  }

  proc <<(a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);
    var   c  = new Bigint();

    if _local || a.localeId == chpl_nodeID {
      mpz_mul_2exp(c.mpz, a.mpz,  b_);

    } else {
      const a_ = a;

      mpz_mul_2exp(c.mpz, a_.mpz, b_);
    }

    return c;
  }



  // Bit-shift right
  proc >>(a: Bigint, b: int) {
    var c = new Bigint();

    if b >= 0 {
      const b_ = b.safeCast(c_ulong);
      var   c  = new Bigint();

      if _local || a.localeId == chpl_nodeID {
        mpz_tdiv_q_2exp(c.mpz, a.mpz,  b_);

      } else {
        const a_ = a;

        mpz_tdiv_q_2exp(c.mpz, a_.mpz, b_);
      }

    } else {
      const b_ = (0 - b).safeCast(c_ulong);

      if _local || a.localeId == chpl_nodeID {
        mpz_mul_2exp(c.mpz, a.mpz,  b_);

      } else {
        const a_ = a;

        mpz_mul_2exp(c.mpz, a_.mpz, b_);
      }
    }

    return c;
  }

  proc >>(a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);
    var   c  = new Bigint();

    if _local || a.localeId == chpl_nodeID {
      mpz_tdiv_q_2exp(c.mpz, a.mpz,  b_);

    } else {
      const a_ = a;

      mpz_tdiv_q_2exp(c.mpz, a_.mpz, b_);
    }

    return c;
  }



  // Bitwise and
  proc &(a: Bigint, b: Bigint) {
    var c = new Bigint();

    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      mpz_and(c.mpz, a.mpz, b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      mpz_and(c.mpz, a_.mpz, b_.mpz);
    }

    return c;
  }



  // Bitwise ior
  proc |(a: Bigint, b: Bigint) {
    var c = new Bigint();

    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      mpz_and(c.mpz, a.mpz, b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      mpz_ior(c.mpz, a_.mpz, b_.mpz);
    }

    return c;
  }



  // Bitwise xor
  proc ^(a: Bigint, b: Bigint) {
    var c = new Bigint();

    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      mpz_and(c.mpz, a.mpz, b.mpz);

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

  private inline proc cmp(a: Bigint, b: Bigint) {
    var ret : c_int;

    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      ret = mpz_cmp(a.mpz,  b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      ret = mpz_cmp(a_.mpz, b_.mpz);
    }

    return ret;
  }

  private inline proc cmp(a: Bigint, b: int) {
    const b_ = b.safeCast(c_long);
    var   ret : c_int;

    if _local || a.localeId == chpl_nodeID {
      ret = mpz_cmp_si(a.mpz,  b_);

    } else {
      const a_ = a;

      ret = mpz_cmp_si(a_.mpz, b_);
    }

    return ret;
  }

  private inline proc cmp(a: int, b: Bigint) {
    const a_ = a.safeCast(c_long);
    var   ret : c_int;

    if _local || b.localeId == chpl_nodeID {
      ret = mpz_cmp_si(b.mpz,  a_);

    } else {
      const b_ = b;

      ret = mpz_cmp_si(b_.mpz, a_);
    }

    return ret;
  }

  private inline proc cmp(a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);
    var   ret : c_int;

    if _local || a.localeId == chpl_nodeID {
      ret = mpz_cmp_ui(a.mpz,  b_);

    } else {
      const a_ = a;

      ret = mpz_cmp_ui(a_.mpz, b_);
    }

    return ret;
  }

  private inline proc cmp(a: uint, b: Bigint) {
    const a_ = a.safeCast(c_ulong);
    var   ret : c_int;

    if _local || b.localeId == chpl_nodeID {
      ret = mpz_cmp_ui(b.mpz,  a_);

    } else {
      const b_ = b;

      ret = mpz_cmp_ui(b_.mpz, a_);
    }

    return ret;
  }



  // Equality
  proc ==(a: Bigint, b: Bigint) {
    return cmp(a, b) == 0;
  }

  proc ==(a: Bigint, b: int) {
    return cmp(a, b) == 0;
  }

  proc ==(a: int, b: Bigint) {
    return cmp(a, b) == 0;
  }

  proc ==(a: Bigint, b: uint) {
    return cmp(a, b) == 0;
  }

  proc ==(a: uint, b: Bigint) {
    return cmp(a, b) == 0;
  }



  // Inequality
  proc !=(a: Bigint, b: Bigint) {
    return cmp(a, b) != 0;
  }

  proc !=(a: Bigint, b: int) {
    return cmp(a, b) != 0;
  }

  proc !=(a: int, b: Bigint) {
    return cmp(a, b) != 0;
  }

  proc !=(a: Bigint, b: uint) {
    return cmp(a, b) != 0;
  }

  proc !=(a: uint, b: Bigint) {
    return cmp(a, b) != 0;
  }



  // Greater than
  proc >(a: Bigint, b: Bigint) {
    return cmp(a, b) > 0;
  }

  proc >(a: Bigint, b: int) {
    return cmp(a, b) > 0;
  }

  proc >(b: int, a: Bigint) {
    return cmp(a, b) > 0;
  }

  proc >(a: Bigint, b: uint) {
    return cmp(a, b) > 0;
  }

  proc >(b: uint, a: Bigint) {
    return cmp(a, b) > 0;
  }



  // Less than
  proc <(a: Bigint, b: Bigint) {
    return cmp(a, b) < 0;
  }

  proc <(a: Bigint, b: int) {
    return cmp(a, b) < 0;
  }

  proc <(b: int, a: Bigint) {
    return cmp(a, b) < 0;
  }

  proc <(a: Bigint, b: uint) {
    return cmp(a, b) < 0;
  }

  proc <(b: uint, a: Bigint) {
    return cmp(a, b) < 0;
  }


  // Greater than or equal
  proc >=(a: Bigint, b: Bigint) {
    return cmp(a, b) >= 0;
  }

  proc >=(a: Bigint, b: int) {
    return cmp(a, b) >= 0;
  }

  proc >=(b: int, a: Bigint) {
    return cmp(a, b) >= 0;
  }

  proc >=(a: Bigint, b: uint) {
    return cmp(a, b) >= 0;
  }

  proc >=(b: uint, a: Bigint) {
    return cmp(a, b) >= 0;
  }



  // Less than or equal
  proc <=(a: Bigint, b: Bigint) {
    return cmp(a, b) <= 0;
  }

  proc <=(a: Bigint, b: int) {
    return cmp(a, b) <= 0;
  }

  proc <=(b: int, a: Bigint) {
    return cmp(a, b) <= 0;
  }

  proc <=(a: Bigint, b: uint) {
    return cmp(a, b) <= 0;
  }

  proc <=(b: uint, a: Bigint) {
    return cmp(a, b) <= 0;
  }




  //
  // Compound Assignment Operations
  //

  // +=
  proc +=(ref a: Bigint, b: Bigint) {
    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      mpz_add(a.mpz, a.mpz, b.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_add(a.mpz, a.mpz, b_.mpz);
      }
    }
  }

  proc +=(ref a: Bigint, b: int) {
    if (b >= 0) {
      const b_ = b.safeCast(c_ulong);

      if _local || a.localeId == chpl_nodeID {
        mpz_add_ui(a.mpz, a.mpz, b_);

      } else {
        const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", aLoc) {
          mpz_add_ui(a.mpz, a.mpz, b_);
        }
      }

    } else {
      const b_ = (0 - b).safeCast(c_ulong);

      if _local || a.localeId == chpl_nodeID {
        mpz_sub_ui(a.mpz, a.mpz, b_);

      } else {
        const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", aLoc) {
          mpz_sub_ui(a.mpz, a.mpz, b_);
        }
      }
    }
  }

  proc +=(ref a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local || a.localeId == chpl_nodeID {
      mpz_add_ui(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_add_ui(a.mpz, a.mpz, b_);
      }
    }
  }



  // -=
  proc -=(ref a: Bigint, b: Bigint) {
    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      mpz_sub(a.mpz, a.mpz, b.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_sub(a.mpz, a.mpz, b_.mpz);
      }
    }
  }

  proc -=(ref a: Bigint, b: int) {
    if (b >= 0) {
      const b_ = b.safeCast(c_ulong);

      if _local || a.localeId == chpl_nodeID {
        mpz_sub_ui(a.mpz, a.mpz, b_);

      } else {
        const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", aLoc) {
          mpz_sub_ui(a.mpz, a.mpz, b_);
        }
      }

    } else {
      const b_ = (0 - b).safeCast(c_ulong);

      if _local || a.localeId == chpl_nodeID {
        mpz_add_ui(a.mpz, a.mpz, b_);

      } else {
        const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", aLoc) {
          mpz_add_ui(a.mpz, a.mpz, b_);
        }
      }
    }
  }

  proc -=(ref a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local || a.localeId == chpl_nodeID {
      mpz_sub_ui(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_sub_ui(a.mpz, a.mpz, b_);
      }
    }
  }



  // *=
  proc *=(ref a: Bigint, b: Bigint) {
    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      mpz_mul(a.mpz, a.mpz, b.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_mul(a.mpz, a.mpz, b_.mpz);
      }
    }
  }

  proc *=(ref a: Bigint, b: int) {
    const b_ = b.safeCast(c_long);

    if _local || a.localeId == chpl_nodeID {
      mpz_mul_si(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_mul_si(a.mpz, a.mpz, b_);
      }
    }
  }

  proc *=(ref a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local || a.localeId == chpl_nodeID {
      mpz_mul_ui(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_mul_ui(a.mpz, a.mpz, b_);
      }
    }
  }



  // /=
  proc /=(ref a: Bigint, b: Bigint) {
    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      mpz_tdiv_q(a.mpz, a.mpz, b.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_tdiv_q(a.mpz, a.mpz, b_.mpz);
      }
    }
  }

  proc /=(ref a: Bigint, b: int) {
    var b_ = 0 : c_ulong;

    if b >= 0 then
      b_ = b.safeCast(c_ulong);
    else
      b_ = (0 - b).safeCast(c_ulong);

    if _local || a.localeId == chpl_nodeID {
      mpz_tdiv_q_ui(a.mpz, a.mpz, b_);

      if b < 0 then
        mpz_neg(a.mpz, a.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_tdiv_q_ui(a.mpz, a.mpz, b_);

        if b < 0 then
          mpz_neg(a.mpz, a.mpz);
      }
    }
  }

  proc /=(ref a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local || a.localeId == chpl_nodeID {
      mpz_tdiv_q_ui(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_tdiv_q_ui(a.mpz, a.mpz, b_);
      }
    }
  }


  // **=
  proc **=(ref a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local || a.localeId == chpl_nodeID {
      mpz_pow_ui(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_pow_ui(a.mpz, a.mpz, b_);
      }
    }
  }



  // %=
  proc %=(ref a: Bigint, b: Bigint) {
    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      mpz_mod(a.mpz, a.mpz, b.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_mod(a.mpz, a.mpz, b_.mpz);
      }
    }
  }

  proc %=(ref a: Bigint, b: int) {
    var b_ = 0 : c_ulong;

    if b >= 0 then
      b_ = b.safeCast(c_ulong);
    else
      b_ = (0 - b).safeCast(c_ulong);

    if _local || a.localeId == chpl_nodeID {
      mpz_mod_ui(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_mod_ui(a.mpz, a.mpz, b_);
      }
    }
  }

  proc %=(ref a: Bigint, b: uint) {
    var b_ = b.safeCast(c_ulong);

    if _local || a.localeId == chpl_nodeID {
      mpz_mod_ui(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_mod_ui(a.mpz, a.mpz, b_);
      }
    }
  }

  proc &=(ref a: Bigint, b: Bigint) {
    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      mpz_and(a.mpz, a.mpz, b.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_and(a.mpz, a.mpz, b_.mpz);
      }
    }
  }

  proc |=(ref a: Bigint, b: Bigint) {
    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      mpz_ior(a.mpz, a.mpz, b.mpz);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        const b_ = b;

        mpz_ior(a.mpz, a.mpz, b_.mpz);
      }
    }
  }

  proc ^=(ref a: Bigint, b: Bigint) {
    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
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
  proc <<=(ref a: Bigint, b: int) {
    if b >= 0 {
      const b_ = b.safeCast(c_ulong);

      if _local || a.localeId == chpl_nodeID {
        mpz_mul_2exp(a.mpz, a.mpz, b_);

      } else {
        const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", aLoc) {
          mpz_mul_2exp(a.mpz, a.mpz, b_);
        }
      }

    } else {
      const b_ = (0 - b).safeCast(c_ulong);

      if _local || a.localeId == chpl_nodeID {
        mpz_tdiv_q_2exp(a.mpz, a.mpz, b_);

      } else {
        const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", aLoc) {
          mpz_tdiv_q_2exp(a.mpz, a.mpz, b_);
        }
      }
    }
  }

  proc <<=(ref a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local || a.localeId == chpl_nodeID {
      mpz_mul_2exp(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_mul_2exp(a.mpz, a.mpz, b_);
      }
    }
  }



  // >>=
  proc >>=(ref a: Bigint, b: int) {
    if b >= 0 {
      const b_ = b.safeCast(c_ulong);

      if _local || a.localeId == chpl_nodeID {
        mpz_tdiv_q_2exp(a.mpz, a.mpz, b_);

      } else {
        const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", aLoc) {
          mpz_tdiv_q_2exp(a.mpz, a.mpz, b_);
        }
      }

    } else {
      const b_ = (0 - b).safeCast(c_ulong);

      if _local || a.localeId == chpl_nodeID {
        mpz_mul_2exp(a.mpz, a.mpz, b_);

      } else {
        const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

        on __primitive("chpl_on_locale_num", aLoc) {
          mpz_mul_2exp(a.mpz, a.mpz, b_);
        }
      }
    }
  }

  proc >>=(ref a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local || a.localeId == chpl_nodeID {
      mpz_tdiv_q_2exp(a.mpz, a.mpz, b_);

    } else {
      const aLoc = chpl_buildLocaleID(a.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", aLoc) {
        mpz_tdiv_q_2exp(a.mpz, a.mpz, b_);
      }
    }
  }


  // Swap
  proc <=>(ref a: Bigint, ref b: Bigint) {
    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
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
  proc jacobi(a: Bigint, b: Bigint) : int {
    var ret : c_int;

    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      ret = mpz_jacobi(a.mpz,  b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      ret = mpz_jacobi(a_.mpz, b_.mpz);
    }

    return ret.safeCast(int);
  }



  proc legendre(a: Bigint, p: Bigint) : int {
    var ret : c_int;

    if _local || (a.localeId == chpl_nodeID && p.localeId == chpl_nodeID) {
      ret = mpz_legendre(a.mpz,  p.mpz);

    } else {
      const a_ = a;
      const p_ = p;

      ret = mpz_legendre(a_.mpz, p_.mpz);
    }

    return ret.safeCast(int);
  }



  // kronecker
  proc kronecker(a: Bigint, b: Bigint) : int {
    var ret : c_int;

    if _local || (a.localeId == chpl_nodeID && b.localeId == chpl_nodeID) {
      ret = mpz_kronecker(a.mpz,  b.mpz);

    } else {
      const a_ = a;
      const b_ = b;

      ret = mpz_kronecker(a_.mpz, b_.mpz);
    }

    return ret.safeCast(int);
  }

  proc kronecker(a: Bigint, b: int) : int {
    const b_ = b.safeCast(c_long);
    var  ret : c_int;

    if _local || a.localeId == chpl_nodeID {
      ret = mpz_kronecker_si(a.mpz,  b_);

    } else {
      const a_ = a;

      ret = mpz_kronecker_si(a_.mpz, b_);
    }

    return ret.safeCast(int);
  }

  proc kronecker(a: int, b: Bigint) : int {
    const a_ = a.safeCast(c_long);
    var  ret : c_int;

    if _local || b.localeId == chpl_nodeID {
      ret = mpz_si_kronecker(a_, b.mpz);

    } else {
      const b_ = b;

      ret = mpz_si_kronecker(a_, b_.mpz);
    }

    return ret.safeCast(int);
  }

  proc kronecker(a: Bigint, b: uint) : int {
    const b_ = b.safeCast(c_ulong);
    var  ret : c_int;

    if _local || a.localeId == chpl_nodeID {
      ret = mpz_kronecker_ui(a.mpz,  b_);

    } else {
      const a_ = a;

      ret = mpz_kronecker_ui(a_.mpz, b_);
    }

    return ret.safeCast(int);
  }

  proc kronecker(a: uint, b: Bigint) : int {
    const a_ = b.safeCast(c_ulong);
    var  ret : c_int;

    if _local || b.localeId == chpl_nodeID {
      ret = mpz_ui_kronecker(a_, a.mpz);

    } else {
      const b_ = b;

      ret = mpz_ui_kronecker(a_, b_.mpz);
    }

    return ret.safeCast(int);
  }


  // divexact
  proc Bigint.divexact(n: Bigint, d: Bigint) {
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

  proc Bigint.divexact(n: Bigint, d: uint) {
    var d_ = d.safeCast(c_ulong);

    if _local {
      mpz_divexact_ui(this.mpz, n.mpz, d_);

    } else if this.localeId == chpl_nodeID &&
              n.localeId    == chpl_nodeID {
      mpz_divexact_ui(this.mpz, n.mpz, d_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const n_ = n;

        mpz_divexact_ui(this.mpz, n_.mpz, d_);
      }
    }
  }



  // divisible_p
  proc Bigint.divisible_p(d: Bigint) : int {
    var ret: c_int;

    if _local {
      ret = mpz_divisible_p(this.mpz, d.mpz);

    } else if this.localeId == chpl_nodeID &&
              d.localeId    == chpl_nodeID {
      ret = mpz_divisible_p(this.mpz, d.mpz);

    } else {
      const t_ = this;
      const d_ = d;

      ret = mpz_divisible_p(this.mpz, d.mpz);
    }

    return ret.safeCast(int);
  }

  proc Bigint.divisible_p(d: uint) : int {
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

  proc Bigint.divisible_2exp_p(b: uint) : int {
    const b_ = b.safeCast(c_ulong);
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
  proc Bigint.congruent_p(d: Bigint) : int {
    var ret: c_int;

    if _local {
      ret = mpz_congruent_p(this.mpz, d.mpz);

    } else if this.localeId == chpl_nodeID &&
              d.localeId    == chpl_nodeID {
      ret = mpz_congruent_p(this.mpz, d.mpz);

    } else {
      const t_ = this;
      const d_ = d;

      ret = mpz_congruent_p(this.mpz, d.mpz);
    }

    return ret.safeCast(int);
  }

  proc Bigint.congruent_p(c: Bigint, d: Bigint) : int {
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

  proc Bigint.congruent_p(c: uint, d: uint) : int {
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

  proc Bigint.congruent_2exp_p(c: Bigint, b: uint) : int {
    const b_ = b.safeCast(c_ulong);
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
  proc Bigint.powm(base: Bigint,
                   exp:  Bigint,
                   mod:  Bigint) {
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

  proc Bigint.powm(base: Bigint,
                   exp: uint,
                   mod: Bigint) {
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

  proc Bigint.pow(base: Bigint, exp: uint) {
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

  proc Bigint.pow(base: uint, exp: uint) {
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
  proc Bigint.root(a: Bigint, n: uint) : int {
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
  proc Bigint.rootrem(ref rem: Bigint, u: Bigint, n: uint) {
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

  proc Bigint.sqrt(a: Bigint) {
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
  proc Bigint.sqrtrem(ref rem: Bigint, a: Bigint) {
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

  proc Bigint.perfect_power_p() : int {
    var ret: c_int;

    if _local || this.localeId == chpl_nodeID {
      ret = mpz_perfect_power_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_perfect_power_p(t_.mpz);
    }

    return ret.safeCast(int);
  }

  proc Bigint.perfect_square_p() : int {
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
  proc Bigint.probab_prime_p(reps: int) : int {
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

  proc Bigint.nextprime(a: Bigint) {
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
  proc Bigint.gcd(a: Bigint, b: Bigint) {
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

  proc Bigint.gcd(a: Bigint, b: uint) {
    var b_ = b.safeCast(c_ulong);

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
  proc Bigint.gcdext(ref s: Bigint,
                     ref t: Bigint,
                     a: Bigint,
                     b: Bigint) {

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
  proc Bigint.lcm(a: Bigint, b: Bigint) {
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

  proc Bigint.lcm(a: Bigint, b: uint) {
    var b_ = b.safeCast(c_ulong);

    if _local {
      mpz_lcm_ui(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
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
  proc Bigint.invert(a: Bigint, b: Bigint) : int {
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
  proc Bigint.remove(a: Bigint, f: Bigint) : uint {
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
  proc Bigint.fac(a: uint) {
    const a_ = a.safeCast(c_ulong);

    if _local || this.localeId == chpl_nodeID {
      mpz_fac_ui(this.mpz, a_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_fac_ui(this.mpz, a_);
      }
    }
  }



  // Binomial
  proc Bigint.bin(n: Bigint, k: uint) {
    const k_ = k.safeCast(c_ulong);

    if _local {
      mpz_bin_ui(this.mpz, n.mpz, k_);

    } else if this.localeId == chpl_nodeID &&
              n.localeId    == chpl_nodeID {
      mpz_bin_ui(this.mpz, n.mpz, k_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var n_ = n;

        mpz_bin_ui(this.mpz, n_.mpz, k_);
      }
    }
  }

  proc Bigint.bin(n: uint, k: uint) {
    const n_ = n.safeCast(c_ulong);
    const k_ = k.safeCast(c_ulong);

    if _local || this.localeId == chpl_nodeID {
      mpz_bin_uiui(this.mpz, n_, k_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_bin_uiui(this.mpz, n_, k_);
      }
    }
  }



  // Fibonacci
  proc Bigint.fib(n: uint) {
    const n_ = n.safeCast(c_ulong);

    if _local || this.localeId == chpl_nodeID {
      mpz_fib_ui(this.mpz, n_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_fib_ui(this.mpz, n_);
      }
    }
  }

  proc Bigint.fib2(ref fnsub1: Bigint, n: uint) {
    const n_ = n.safeCast(c_ulong);

    if _local {
      mpz_fib2_ui(this.mpz, fnsub1.mpz, n_);

    } else if this.localeId   == chpl_nodeID &&
              fnsub1.localeId == chpl_nodeID {
      mpz_fib2_ui(this.mpz, fnsub1.mpz, n_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var fnsub1_ : Bigint;

        mpz_fib2_ui(this.mpz, fnsub1_.mpz, n_);

        fnsub1 = fnsub1_;
      }
    }
  }



  // Lucas Number
  proc Bigint.lucnum(n: uint) {
    const n_ = n.safeCast(c_ulong);

    if _local || this.localeId == chpl_nodeID {
      mpz_lucnum_ui(this.mpz, n_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_lucnum_ui(this.mpz, n_);
      }
    }
  }

  proc Bigint.lucnum2(ref fnsub1: Bigint, n: uint) {
    const n_ = n.safeCast(c_ulong);

    if _local {
      mpz_lucnum2_ui(this.mpz, fnsub1.mpz, n_);

    } else if this.localeId   == chpl_nodeID &&
              fnsub1.localeId == chpl_nodeID {
      mpz_lucnum2_ui(this.mpz, fnsub1.mpz, n_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var fnsub1_ : Bigint;

        mpz_lucnum2_ui(this.mpz, fnsub1_.mpz, n_);

        fnsub1 = fnsub1_;
      }
    }
  }



  // Bit operations
  proc Bigint.popcount() : uint {
    var ret: c_ulong;

    if _local || this.localeId == chpl_nodeID {
      ret = mpz_popcount(this.mpz);

    } else {
      const t_ = this;

      ret = mpz_popcount(t_.mpz);
    }

    return ret.safeCast(uint);
  }

  proc Bigint.hamdist(b: Bigint) : uint {
    var ret: c_ulong;

    if _local {
      ret = mpz_hamdist(this.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
      ret = mpz_hamdist(this.mpz, b.mpz);

    } else {
      const t_ = this;
      const b_ = b;

      ret = mpz_hamdist(t_.mpz, b_.mpz);
    }

    return ret.safeCast(uint);
  }

  proc Bigint.scan0(starting_bit: uint) : uint {
    const sb_ = starting_bit.safeCast(c_ulong);
    var   ret: c_ulong;

    if _local || this.localeId == chpl_nodeID {
      ret = mpz_scan0(this.mpz, sb_);

    } else {
      const t_ = this;

      ret = mpz_scan0(t_.mpz,   sb_);
    }

    return ret.safeCast(uint);
  }

  proc Bigint.scan1(starting_bit: uint) : uint {
    const sb_ = starting_bit.safeCast(c_ulong);
    var   ret: c_ulong;

    if _local || this.localeId == chpl_nodeID {
      ret = mpz_scan1(this.mpz, sb_);

    } else {
      const t_ = this;

      ret = mpz_scan1(t_.mpz,   sb_);
    }

    return ret.safeCast(uint);
  }



  // Set/Clr bit
  proc Bigint.setbit(bit_index: uint) {
    const bi_ = bit_index.safeCast(c_ulong);

    if _local || this.localeId == chpl_nodeID {
      mpz_setbit(this.mpz, bi_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_setbit(this.mpz, bi_);
      }
    }
  }

  proc Bigint.clrbit(bit_index: uint) {
    const bi_ = bit_index.safeCast(c_ulong);

    if _local || this.localeId == chpl_nodeID {
      mpz_clrbit(this.mpz, bi_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_clrbit(this.mpz, bi_);
      }
    }
  }

  proc Bigint.combit(bit_index: uint) {
    const bi_ = bit_index.safeCast(c_ulong);

    if _local || this.localeId == chpl_nodeID {
      mpz_combit(this.mpz, bi_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        mpz_combit(this.mpz, bi_);
      }
    }
  }

  proc Bigint.tstbit(bit_index: uint) : int {
    const bi_ = bit_index.safeCast(c_ulong);
    var  ret: c_int;

    if _local || this.localeId == chpl_nodeID {
      ret = mpz_tstbit(this.mpz, bi_);

    } else {
      var t_ = this;

      ret = mpz_tstbit(t_.mpz, bi_);
    }

    return ret.safeCast(int);
  }



  // Miscellaneous Functions
  proc Bigint.fits_ulong_p() : int {
    var ret: c_int;

    if _local || this.localeId == chpl_nodeID {
      ret = mpz_fits_ulong_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_fits_ulong_p(t_.mpz);
    }

    return ret.safeCast(int);
  }

  proc Bigint.fits_slong_p() : int {
    var ret: c_int;

    if _local || this.localeId == chpl_nodeID {
      ret = mpz_fits_slong_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_fits_slong_p(t_.mpz);
    }

    return ret.safeCast(int);
  }

  proc Bigint.fits_uint_p() : int {
    var ret: c_int;

    if _local || this.localeId == chpl_nodeID {
      ret = mpz_fits_uint_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_fits_uint_p(t_.mpz);
    }

    return ret.safeCast(int);
  }

  proc Bigint.fits_sint_p() : int {
    var ret: c_int;

    if _local || this.localeId == chpl_nodeID {
      ret = mpz_fits_sint_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_fits_sint_p(t_.mpz);
    }

    return ret.safeCast(int);
  }

  proc Bigint.fits_ushort_p() : int {
    var ret: c_int;

    if _local || this.localeId == chpl_nodeID {
      ret = mpz_fits_ushort_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_fits_ushort_p(t_.mpz);
    }

    return ret.safeCast(int);
  }

  proc Bigint.fits_sshort_p() : int {
    var ret: c_int;

    if _local || this.localeId == chpl_nodeID {
      ret = mpz_fits_sshort_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_fits_sshort_p(t_.mpz);
    }

    return ret.safeCast(int);
  }

  proc Bigint.even_p() : int {
    var ret: c_int;

    if _local || this.localeId == chpl_nodeID {
      ret = mpz_even_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_even_p(t_.mpz);
    }

    return ret.safeCast(int);
  }

  proc Bigint.odd_p() : int {
    var ret: c_int;

    if _local || this.localeId == chpl_nodeID {
      ret = mpz_odd_p(this.mpz);

    } else {
      var t_ = this;

      ret = mpz_odd_p(t_.mpz);
    }

    return ret.safeCast(int);
  }



  //
  // Unary arithmetic functions
  //

  proc Bigint.neg(a: Bigint) {
    if _local {
      mpz_neg(this.mpz, a.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
      mpz_neg(this.mpz, a.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_neg(this.mpz, a_.mpz);
      }
    }
  }

  proc Bigint.abs(a: Bigint) {
    if _local {
      mpz_abs(this.mpz, a.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
      mpz_abs(this.mpz, a.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_abs(this.mpz, a_.mpz);
      }
    }
  }

  //
  // Binary arithmetic functions
  //

  // Addition
  proc Bigint.add(a: Bigint, b: Bigint) {
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

  proc Bigint.add(a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_add_ui(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
      mpz_add_ui(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_add_ui(this.mpz, a_.mpz, b_);
      }
    }
  }



  // Subtraction
  proc Bigint.sub(a: Bigint, b: Bigint) {
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

  proc Bigint.sub(a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_sub_ui(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
      mpz_sub_ui(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_sub_ui(this.mpz, a_.mpz, b_);
      }
    }
  }

  proc Bigint.sub(a: uint, b: Bigint) {
    const a_ = a.safeCast(c_ulong);

    if _local {
      mpz_ui_sub(this.mpz, a_, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
      mpz_ui_sub(this.mpz, a_, b.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const b_ = b;

        mpz_ui_sub(this.mpz, a_, b_.mpz);
      }
    }
  }



  // Multiplication
  proc Bigint.mul(a: Bigint, b: Bigint) {
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

  proc Bigint.mul(a: Bigint, b: int) {
    const b_ = b.safeCast(c_long);

    if _local {
      mpz_mul_si(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
      mpz_mul_si(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_mul_si(this.mpz, a_.mpz, b_);
      }
    }
  }

  proc Bigint.mul(a: Bigint, b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_mul_ui(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
      mpz_mul_ui(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_mul_ui(this.mpz, a_.mpz, b_);
      }
    }
  }

  proc Bigint.mul_2exp(a: Bigint, b: uint) {
    var b_ = b.safeCast(c_ulong);

    if _local {
      mpz_mul_2exp(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
      mpz_mul_2exp(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_mul_2exp(this.mpz, a_.mpz, b_);
      }
    }
  }

  // Division
  proc Bigint.div_q(param rounding: Round,
                    n: Bigint,
                    d: Bigint) {
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

  proc Bigint.div_q(param rounding: Round,
                    n: Bigint,
                    d: uint) : uint {
    const d_ = d.safeCast(c_ulong);
    var   ret: c_ulong;

    if _local {
      select rounding {
        when Round.UP   do ret = mpz_cdiv_q_ui(this.mpz, n.mpz,  d_);
        when Round.DOWN do ret = mpz_fdiv_q_ui(this.mpz, n.mpz,  d_);
        when Round.ZERO do ret = mpz_tdiv_q_ui(this.mpz, n.mpz,  d_);
      }

    } else if this.localeId == chpl_nodeID &&
              n.localeId    == chpl_nodeID {
      select rounding {
        when Round.UP   do ret = mpz_cdiv_q_ui(this.mpz, n.mpz,  d_);
        when Round.DOWN do ret = mpz_fdiv_q_ui(this.mpz, n.mpz,  d_);
        when Round.ZERO do ret = mpz_tdiv_q_ui(this.mpz, n.mpz,  d_);
      }

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const n_ = n;

        select rounding {
          when Round.UP   do ret = mpz_cdiv_q_ui(this.mpz, n_.mpz, d_);
          when Round.DOWN do ret = mpz_fdiv_q_ui(this.mpz, n_.mpz, d_);
          when Round.ZERO do ret = mpz_tdiv_q_ui(this.mpz, n_.mpz, d_);
        }
      }
    }

    return ret.safeCast(uint);
  }

  proc Bigint.div_r(param rounding: Round,
                    n: Bigint,
                    d: Bigint) {
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

  proc Bigint.div_r(param rounding: Round,
                    n: Bigint,
                    d: uint) : uint {
    const d_ = d.safeCast(c_ulong);
    var   ret: c_ulong;

    if _local {
      select rounding {
        when Round.UP   do ret = mpz_cdiv_r_ui(this.mpz, n.mpz,  d_);
        when Round.DOWN do ret = mpz_fdiv_r_ui(this.mpz, n.mpz,  d_);
        when Round.ZERO do ret = mpz_tdiv_r_ui(this.mpz, n.mpz,  d_);
      }

    } else if this.localeId == chpl_nodeID &&
              n.localeId    == chpl_nodeID {
      select rounding {
        when Round.UP   do ret = mpz_cdiv_r_ui(this.mpz, n.mpz,  d_);
        when Round.DOWN do ret = mpz_fdiv_r_ui(this.mpz, n.mpz,  d_);
        when Round.ZERO do ret = mpz_tdiv_r_ui(this.mpz, n.mpz,  d_);
      }

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const n_ = n;

        select rounding {
          when Round.UP   do ret = mpz_cdiv_r_ui(this.mpz, n_.mpz, d_);
          when Round.DOWN do ret = mpz_fdiv_r_ui(this.mpz, n_.mpz, d_);
          when Round.ZERO do ret = mpz_tdiv_r_ui(this.mpz, n_.mpz, d_);
        }
      }
    }

    return ret.safeCast(uint);
  }



  // this gets quotient, r gets remainder
  proc Bigint.div_qr(param rounding: Round,
                     ref r: Bigint,
                     n: Bigint,
                     d: Bigint) {
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

  // this gets quotient, r gets remainder
  proc Bigint.div_qr(param rounding: Round,
                     ref r: Bigint,
                     n: Bigint,
                     d: uint) : uint {
    const d_ = d.safeCast(c_ulong);
    var   ret: c_ulong;

    if _local {
      select rounding {
        when Round.UP   do ret = mpz_cdiv_qr_ui(this.mpz, r.mpz, n.mpz, d_);
        when Round.DOWN do ret = mpz_fdiv_qr_ui(this.mpz, r.mpz, n.mpz, d_);
        when Round.ZERO do ret = mpz_tdiv_qr_ui(this.mpz, r.mpz, n.mpz, d_);
      }

    } else if this.localeId == chpl_nodeID &&
              r.localeId    == chpl_nodeID &&
              n.localeId    == chpl_nodeID {
      select rounding {
        when Round.UP   do ret = mpz_cdiv_qr_ui(this.mpz, r.mpz, n.mpz, d_);
        when Round.DOWN do ret = mpz_fdiv_qr_ui(this.mpz, r.mpz, n.mpz, d_);
        when Round.ZERO do ret = mpz_tdiv_qr_ui(this.mpz, r.mpz, n.mpz, d_);
      }

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var   r_ = r;
        const n_ = n;

        select rounding {
          when Round.UP   do
            ret = mpz_cdiv_qr_ui(this.mpz, r_.mpz, n_.mpz, d_);

          when Round.DOWN do
            ret = mpz_fdiv_qr_ui(this.mpz, r_.mpz, n_.mpz, d_);

          when Round.ZERO do
            ret = mpz_tdiv_qr_ui(this.mpz, r_.mpz, n_.mpz, d_);
        }

        r = r_;
      }
    }

    return ret;
  }

  proc Bigint.div_q_2exp(param rounding: Round,
                         n: Bigint,
                         b: uint) {
    const b_ = b.safeCast(c_ulong);

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

  proc Bigint.div_r_2exp(param rounding: Round,
                         n: Bigint,
                         b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      select rounding {
        when Round.UP   do mpz_cdiv_r_2exp(this.mpz, n.mpz, b_);
        when Round.DOWN do mpz_fdiv_r_2exp(this.mpz, n.mpz, b_);
        when Round.ZERO do mpz_tdiv_r_2exp(this.mpz, n.mpz, b_);
      }

    } else if this.localeId == chpl_nodeID &&
              n.localeId    == chpl_nodeID {
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



  proc Bigint.addmul(a: Bigint,
                     b: Bigint) {
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

  proc Bigint.addmul(a: Bigint,
                     b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_addmul_ui(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID &&
               a.localeId    == chpl_nodeID {
      mpz_addmul_ui(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_addmul_ui(this.mpz, a_.mpz, b_);
      }
    }
  }



  proc Bigint.submul(a: Bigint,
                     b: Bigint) {
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

  proc Bigint.submul(a: Bigint,
                     b: uint) {
    const b_ = b.safeCast(c_ulong);

    if _local {
      mpz_submul_ui(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
      mpz_submul_ui(this.mpz, a.mpz, b_);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const a_ = a;

        mpz_submul_ui(this.mpz, a_.mpz, b_);
      }
    }
  }



  proc Bigint.mod(a: Bigint,
                  b: Bigint) {
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

  proc Bigint.mod(a: Bigint,
                  b: uint) : uint {
    const b_ = b.safeCast(c_ulong);
    var   ret: c_ulong;

    if _local {
      ret = mpz_mod_ui(this.mpz, a.mpz, b_);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
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
  proc Bigint.cmp(b: Bigint) : int {
    var ret: c_int;

    if _local {
      ret = mpz_cmp(this.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
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

  proc Bigint.cmp(b: int) : int {
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

  proc Bigint.cmp(b: uint) : int {
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

  proc Bigint.cmp(b: real) : int {
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



  proc Bigint.cmpabs(b: Bigint) : int {
    var ret: c_int;

    if _local {
      ret = mpz_cmpabs(this.mpz, b.mpz);

    } else if this.localeId == chpl_nodeID &&
              b.localeId    == chpl_nodeID {
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

  proc Bigint.cmpabs(b: uint) : int {
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

  proc Bigint.cmpabs(b: real) : int {
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



  proc Bigint.sgn() : int {
    var ret: c_int;

    if _local || this.localeId == chpl_nodeID {
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
  proc Bigint.and(a: Bigint, b: Bigint) {
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

  proc Bigint.ior(a: Bigint, b: Bigint) {
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

  proc Bigint.xor(a: Bigint, b: Bigint) {
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

  proc Bigint.com(a: Bigint) {
    if _local {
      mpz_com(this.mpz, a.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
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
  proc Bigint.set(a: Bigint) {
    if _local {
      mpz_set(this.mpz, a.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
      mpz_set(this.mpz, a.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        const mpz_struct = a.mpzStruct();

        chpl_gmp_get_mpz(this.mpz, a.localeId, mpz_struct);
      }
    }
  }

  proc Bigint.set(num : int) {
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

  proc Bigint.set(num : uint) {
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

  proc Bigint.set(num: real) {
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

  proc Bigint.set(str: string, base: int = 0) {
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

  proc Bigint.swap(ref a: Bigint) {
    if _local {
      mpz_swap(this.mpz, a.mpz);

    } else if this.localeId == chpl_nodeID &&
              a.localeId    == chpl_nodeID {
      mpz_swap(this.mpz, a.mpz);

    } else {
      const thisLoc = chpl_buildLocaleID(this.localeId, c_sublocid_any);

      on __primitive("chpl_on_locale_num", thisLoc) {
        var tmp = new Bigint(a);

        a.set(this);

        mpz_set(this.mpz, tmp.mpz);
      }
    }
  }
}
