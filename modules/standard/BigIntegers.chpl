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
Support for GNU Multiple Precision Integer Arithmetic through the BigInt record

This module implements an interface with the GMP libray (the GNU Multiple 
Precision Arithmetic Library). See the `GMP homepage <https://gmplib.org/>` for
more information on this library. See the GMP Chapel module for more information on how to use GMP with Chapel.

Using the BigInt record
-----------------------

The BigIntegers Chapel module provides a :record:`BigInt` record wrapping GMP
integers. At the present time, only the functions for ``mpz`` (ie signed
integer) GMP types are supported with :record:`BigInt`; future work will be to
extend this support to floating-point types. 

:record:`BigInt` methods all wrap GMP functions with obviously similar names.
The :record:`BigInt` methods are locale aware - so Chapel programs can create
a distributed array of GMP numbers. The method of :record:`BigInt` objects are
setting the receiver, so e.g. myBigInt.add(x,y) sets myBigInt to ``x + y``.

A code example::

 use BigIntegers;

 var a = new BigInt(); // initialize a GMP value, set it to zero

 a.fac(100); // set a to 100!

 writeln(a); // output 100!

 // memory used by the GMP value is freed automatically by record destruction

 var b = new BigInt("48473822929893829847"); // initialize from a decimal
 string
 b.add(b, 1); // add one to b
*/

module BigIntegers {
  use GMP;

  // used in the division functions to determine which direction to round in
  enum Round {
    UP = 1,
    DOWN = -1,
    ZERO = 0
  }

  /*
    The BigInt record provides a more Chapel-friendly interface to the
    GMP integer functions. In particular, this record supports GMP
    numbers that are stored in distributed arrays.

    All methods on BigInt work with Chapel types. Many of them use the gmp
    functions directly, which use C types. Runtime checks are used to ensure 
    the Chapel types can safely be cast to the C types (e.g. when casting a 
    Chapel uint it checks that it fits in the C ulong which could be a 32bit 
    type if running on linux32 platform).

    The checks are controlled by the compiler options ``--[no-]cast-checks``,
    ``--fast``, etc.

    TODO: When a Chapel type will not safely cast to a C type, it would be
    better to promote the Chapel value to a BigInt, then run the operation on
    that BigInt. This would make the BigInt interface consistent across all
    platforms (already true today) _and_ always work regardless of platform
    (not true today).
   */

  pragma "ignore noinit" // TODO: Is this pragma still needed?
  record BigInt {
    var mpz : mpz_t;
    var owned : bool; // all user-defined constructors set owned to true
    var locale_id = chpl_nodeID;

    proc BigInt() {
      mpz_init(this.mpz);
      owned = true;
    }
    proc BigInt(init2: bool, nbits: uint) {
      mpz_init2(this.mpz, nbits.safeCast(c_ulong));
      owned = true;
    }
    proc BigInt(num: int) {
      mpz_init_set_si(this.mpz, num.safeCast(c_long));
      owned = true;
    }
    proc BigInt(str: string, base: int=0) {
      var e:c_int;
      e = mpz_init_set_str(this.mpz, str.localize().c_str(), base.safeCast(c_int));
      if e {
        mpz_clear(this.mpz);
        halt("Error initializing big integer: bad format");
      }
      owned = true;
    }
    proc BigInt(str: string, base: int=0, out error:syserr) {
      var e:c_int;
      error = ENOERR;
      e = mpz_init_set_str(this.mpz, str.localize().c_str(), base.safeCast(c_int));
      if e {
        mpz_clear(this.mpz);
        error = EFORMAT;
      }
      owned = true;
    }
    proc BigInt(const ref num: BigInt, owned: bool = true) {
      if num.locale_id == here.id {
        mpz_init_set(this.mpz, num.mpz);
      } else {
        mpz_init(this.mpz);
        var mpz_struct = num.mpzStruct();
        chpl_gmp_get_mpz(this.mpz, num.locale.id, mpz_struct);
      }
      this.owned = owned;
    }

    proc ~BigInt() {
        if this.owned then {
          on Locales[this.locale_id] do mpz_clear(this.mpz);
          this.owned = false;
        }
    }

    // Note: by the time reinit is called, locality has been taken care of
    proc ref reinitBigInt(num : mpz, needToCopy : bool = true) {
      // if the record exists, and is owned, a constructor of some
      // sort was called, and mpz is initialized. 

      // if we own our old num re-use it
      // if we don't, init a new one
      if !(this.owned) {
        on Locales[this.locale_id] do mpz_init(this.mpz);
        this.owned = true;
      }

      // if we don't need to deep copy, free ours, make a shallow copy, get out
      if !needToCopy {
        on Locales[this.locale_id] do mpz_clear(this.mpz);
        this.owned = false;
        this.mpz = num;
        return;
      } else {
        mpz_set(this.mpz, num);
        this.owned = true;
      }
    }

    // utility functions used below.
    proc numLimbs: uint(64) {
      var mpz_struct = this.mpz[1];
      return chpl_gmp_mpz_nlimbs(mpz_struct);
    }
    proc mpzStruct(): __mpz_struct {
      var ret: __mpz_struct;
      on Locales[this.locale_id] {
        ret = this.mpz[1];
      }
      return ret;
    }

    // Assignment functions
    proc set(const ref a: BigInt) {
      on Locales[this.locale_id] {
        if a.locale == here {
          mpz_set(this.mpz, a.mpz);
        } else {
          var mpz_struct = a.mpzStruct();
          chpl_gmp_get_mpz(this.mpz, a.locale.id, mpz_struct);
          this.owned = true;
        }
      }
    }
    proc set(num: uint) {
      on Locales[this.locale_id] do 
        mpz_set_ui(this.mpz, num.safeCast(c_ulong));
    }
    proc set(num:int) {
      on Locales[this.locale_id] do mpz_set_si(this.mpz, num.safeCast(c_long));
    }
    proc set(num: real) {
      on Locales[this.locale_id] do mpz_set_d(this.mpz, num: c_double);
    }
    proc set(str: string, base: int=0) {
      on Locales[this.locale_id] do 
        mpz_set_str(this.mpz, str.localize().c_str(), base.safeCast(c_int));
    }
    proc swap(ref a: BigInt) {
      on Locales[this.locale_id] {
        if a.locale_id == here.id {
          mpz_swap(this.mpz, a.mpz);
        } else {
          // we have to introduce a temporary.
          var tmp = new BigInt(a);
          // set a to what this is
          a.set(this);
          // set this to tmp (which holds the value that a was)
          mpz_set(this.mpz, tmp.mpz);
        }
      }
    }
    // TODO: should these be get(type_to_return) instead?
    proc get_ui():uint {
      var x:c_ulong;
      on Locales[this.locale_id] do x = mpz_get_ui(this.mpz);
      return x.safeCast(uint);
    }
    proc get_si():int {
      var x:c_long;
      on Locales[this.locale_id] do x = mpz_get_si(this.mpz);
      return x.safeCast(int);
    }
    proc get_d():real { // TODO: should this be get_real?
      var x:c_double;
      on Locales[this.locale_id] do x = mpz_get_d(this.mpz);
      return x:real;
    }

    // returns (exponent, double)
    // TODO: compiler chooses an INT(64) for tmp, gets win-pointer mismatch
    // for architectures where long and long long are both 8 bytes, it makes
    // an int64 * for the long, and tries to return it as a long long * instead
    // makes a warning
    proc get_d_2exp():(uint(32), real) {
      var exp:c_long;
      var dbl:c_double;
      on Locales[this.locale_id] {
        var tmp:c_long;
        dbl = mpz_get_d_2exp(tmp, this.mpz);
        exp = tmp;
      }
      return (exp.safeCast(uint(32)), dbl: real);
    }
    proc get_str(base: int=10):string {
      var ret:string;
      on Locales[this.locale_id] {
        var tmpvar = chpl_gmp_mpz_get_str(base.safeCast(c_int), this.mpz);
        ret = new string(tmpvar, owned=true, needToCopy=false);
      }
      return ret;
    }

    // Arithmetic functions
    proc add(const ref a: BigInt, const ref b: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id || here.id != b.locale_id) {
          var a_ = a;
          var b_ = b;
          mpz_add(this.mpz, a_.mpz, b_.mpz);
        } else {
          mpz_add(this.mpz, a.mpz, b.mpz);
        }
      }
    }
    proc add(const ref a: BigInt, b: uint) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id) {
          var a_ = a;
          mpz_add_ui(this.mpz, a_.mpz, b.safeCast(c_ulong));
        } else {
          mpz_add_ui(this.mpz, a.mpz, b.safeCast(c_ulong));
        }
      }
    }
    proc sub(const ref a: BigInt, const ref b: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id || here.id != b.locale_id) {
          var a_ = a;
          var b_ = b;
          mpz_sub(this.mpz, a_.mpz, b_.mpz);
        } else {
          mpz_sub(this.mpz, a.mpz, b.mpz);
        }
      }
    }
    proc sub(const ref a: BigInt, b: uint) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id) {
          var a_ = a;
          mpz_sub_ui(this.mpz, a_.mpz, b.safeCast(c_ulong));
        } else {
          mpz_sub_ui(this.mpz, a.mpz, b.safeCast(c_ulong));
        }
      }
    }
    proc sub(a: uint, const ref b: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != b.locale_id) {
          var b_ = b;
          mpz_ui_sub(this.mpz, a.safeCast(c_ulong), b_.mpz);
        } else {
          mpz_ui_sub(this.mpz, a.safeCast(c_ulong), b.mpz);
        }
      }
    }
    proc mul(const ref a: BigInt, const ref b: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id || here.id != b.locale_id) {
          var a_ = a;
          var b_ = b;
          mpz_mul(this.mpz, a_.mpz, b_.mpz);
        } else {
          mpz_mul(this.mpz, a.mpz, b.mpz);
        }
      }
    }
    proc mul(const ref a: BigInt, b: int) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id) {
          var a_ = a;
          mpz_mul_si(this.mpz, a_.mpz, b.safeCast(c_long));
        } else {
          mpz_mul_si(this.mpz, a.mpz, b.safeCast(c_long));
        }
      }
    }
    proc mul(const ref a: BigInt, b: uint) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id) {
          var a_ = a;
          mpz_mul_ui(this.mpz, a_.mpz, b.safeCast(c_ulong));
        } else {
          mpz_mul_ui(this.mpz, a.mpz, b.safeCast(c_ulong));
        }
      }
    }
    proc addmul(const ref a: BigInt, const ref b: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id || here.id != b.locale_id) {
          var a_ = a;
          var b_ = b;
          mpz_addmul(this.mpz, a_.mpz, b_.mpz);
        } else {
          mpz_addmul(this.mpz, a.mpz, b.mpz);
        }
      }
    }
    proc addmul(const ref a: BigInt, b: uint) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id) {
        var a_ = a;
        mpz_addmul_ui(this.mpz, a_.mpz, b.safeCast(c_ulong));
        } else {
          mpz_addmul_ui(this.mpz, a.mpz, b.safeCast(c_ulong));
        }
      }
    }
    proc submul(const ref a: BigInt, const ref b: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id || here.id != b.locale_id) {
          var a_ = a;
          var b_ = b;
          mpz_submul(this.mpz, a_.mpz, b_.mpz);
        } else {
          mpz_submul(this.mpz, a.mpz, b.mpz);
        }
      }
    }
    proc submul(const ref a: BigInt, b: uint) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id) {
          var a_ = a;
          mpz_submul_ui(this.mpz, a_.mpz, b.safeCast(c_ulong));
        } else {
          mpz_submul_ui(this.mpz, a.mpz, b.safeCast(c_ulong));
        }
      }
    }
    proc mul_2exp(const ref a: BigInt, b: uint) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id) {
        var a_ = a;
        mpz_mul_2exp(this.mpz, a_.mpz, b.safeCast(c_ulong));
        } else {
          mpz_mul_2exp(this.mpz, a.mpz, b.safeCast(c_ulong));
        }
      }
    }
    proc div_2exp(const ref a: BigInt, b:uint) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id) {
        var a_ = a;
        mpz_div_2exp(this.mpz, a_.mpz, b.safeCast(c_ulong));
        } else {
          mpz_div_2exp(this.mpz, a.mpz, b.safeCast(c_ulong));
        }
      }
    }
    proc neg(const ref a: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id) {
          var a_ = a;
          mpz_neg(this.mpz, a_.mpz);
        } else {
          mpz_neg(this.mpz, a.mpz);
        }
      }
    }
    proc abs(const ref a: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id) {
          var a_ = a;
          mpz_abs(this.mpz, a_.mpz);
        } else {
          mpz_abs(this.mpz, a.mpz);
        }
      }
    }

    // Division Functions
    // These functions take in a constant rounding mode.
    proc div_q(param rounding: Round, const ref n: BigInt, const ref d: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != n.locale_id || here.id != d.locale_id) {
          var n_ = n;
          var d_ = d;
          select rounding {
            when Round.UP   do mpz_cdiv_q(this.mpz, n_.mpz, d_.mpz);
            when Round.DOWN do mpz_fdiv_q(this.mpz, n_.mpz, d_.mpz);
            when Round.ZERO do mpz_tdiv_q(this.mpz, n_.mpz, d_.mpz);
          }
        } else {
          select rounding {
            when Round.UP   do mpz_cdiv_q(this.mpz, n.mpz, d.mpz);
            when Round.DOWN do mpz_fdiv_q(this.mpz, n.mpz, d.mpz);
            when Round.ZERO do mpz_tdiv_q(this.mpz, n.mpz, d.mpz);
          }
        }
      }
    }
    proc div_r(param rounding: Round, const ref n: BigInt, const ref d: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != n.locale_id || here.id != d.locale_id) {
          var n_ = n;
          var d_ = d;
          select rounding {
            when Round.UP   do mpz_cdiv_r(this.mpz, n_.mpz, d_.mpz);
            when Round.DOWN do mpz_fdiv_r(this.mpz, n_.mpz, d_.mpz);
            when Round.ZERO do mpz_tdiv_r(this.mpz, n_.mpz, d_.mpz);
          }
        } else {
          select rounding {
            when Round.UP   do mpz_cdiv_r(this.mpz, n.mpz, d.mpz);
            when Round.DOWN do mpz_fdiv_r(this.mpz, n.mpz, d.mpz);
            when Round.ZERO do mpz_tdiv_r(this.mpz, n.mpz, d.mpz);
          }
        }
      }
    }
    // this gets quotient, r gets remainder
    proc ref div_qr(param rounding: Round, ref r: BigInt, const ref n: BigInt, const ref d: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != r.locale_id || here.id != n.locale_id || here.id != d.locale_id) {
          var r_ = r;
          var n_ = n;
          var d_ = d;
          select rounding {
            when Round.UP   do mpz_cdiv_qr(this.mpz, r_.mpz, n_.mpz, d_.mpz);
            when Round.DOWN do mpz_fdiv_qr(this.mpz, r_.mpz, n_.mpz, d_.mpz);
            when Round.ZERO do mpz_tdiv_qr(this.mpz, r_.mpz, n_.mpz, d_.mpz);
          }
          r.set(r_);
        } else {
          select rounding {
            when Round.UP   do mpz_cdiv_qr(this.mpz, r.mpz, n.mpz, d.mpz);
            when Round.DOWN do mpz_fdiv_qr(this.mpz, r.mpz, n.mpz, d.mpz);
            when Round.ZERO do mpz_tdiv_qr(this.mpz, r.mpz, n.mpz, d.mpz);
          }
        }
      }
    }
    proc div_q(param rounding: Round, const ref n: BigInt, d: uint):uint {
      var ret:c_ulong;
      on Locales[this.locale_id] {
        const cd = d.safeCast(c_ulong);
        if (here.id != n.locale_id) {
          var n_ = n;
          select rounding {
            when Round.UP   do ret=mpz_cdiv_q_ui(this.mpz, n_.mpz, cd);
            when Round.DOWN do ret=mpz_fdiv_q_ui(this.mpz, n_.mpz, cd);
            when Round.ZERO do ret=mpz_tdiv_q_ui(this.mpz, n_.mpz, cd);
          }
        } else {
          select rounding {
            when Round.UP   do ret=mpz_cdiv_q_ui(this.mpz, n.mpz, cd);
            when Round.DOWN do ret=mpz_fdiv_q_ui(this.mpz, n.mpz, cd);
            when Round.ZERO do ret=mpz_tdiv_q_ui(this.mpz, n.mpz, cd);
          }
        }
      }
      return ret.safeCast(uint);
    }
    proc div_r(param rounding: Round, const ref n: BigInt, d: uint):uint {
      var ret:c_ulong;
      on Locales[this.locale_id] {
      const cd = d.safeCast(c_ulong);
        if (here.id != n.locale_id) {
          var n_ = n;
          select rounding {
            when Round.UP   do ret=mpz_cdiv_r_ui(this.mpz, n_.mpz, cd);
            when Round.DOWN do ret=mpz_fdiv_r_ui(this.mpz, n_.mpz, cd);
            when Round.ZERO do ret=mpz_tdiv_r_ui(this.mpz, n_.mpz, cd);
          }
        } else {
          select rounding {
            when Round.UP   do ret=mpz_cdiv_r_ui(this.mpz, n.mpz, cd);
            when Round.DOWN do ret=mpz_fdiv_r_ui(this.mpz, n.mpz, cd);
            when Round.ZERO do ret=mpz_tdiv_r_ui(this.mpz, n.mpz, cd);
          }
        }
      }
      return ret.safeCast(uint);
    }
    // this gets quotient, r gets remainder
    proc div_qr(param rounding: Round, ref r: BigInt, const ref n: BigInt, d: uint):uint {
      var ret:c_ulong;
      const cd = d.safeCast(c_ulong);
      on Locales[this.locale_id] {
        if (here.id != n.locale_id || here.id != r.locale_id) {
          var r_ = r;
          var n_ = n;
          select rounding {
            when Round.UP  do ret=mpz_cdiv_qr_ui(this.mpz, r_.mpz, n_.mpz, cd);
            when Round.DOWN do ret=mpz_fdiv_qr_ui(this.mpz, r_.mpz, n_.mpz, cd);
            when Round.ZERO do ret=mpz_tdiv_qr_ui(this.mpz, r_.mpz, n_.mpz, cd);
          }
          r.set(r_);
        } else {
          select rounding {
            when Round.UP  do ret=mpz_cdiv_qr_ui(this.mpz, r.mpz, n.mpz, cd);
            when Round.DOWN do ret=mpz_fdiv_qr_ui(this.mpz, r.mpz, n.mpz, cd);
            when Round.ZERO do ret=mpz_tdiv_qr_ui(this.mpz, r.mpz, n.mpz, cd);
          }
        }
      }
      return ret.safeCast(uint);
    }

    // div for uint defined outside of record scope, as it returns a value
    // and does not set a BigInt

    proc div_q_2exp(param rounding: Round, const ref n: BigInt, b: uint) {
      on Locales[this.locale_id] {
      const cb = b.safeCast(c_ulong);
        if (here.id != n.locale_id) {
          var n_ = n;
          select rounding {
            when Round.UP   do mpz_cdiv_q_2exp(this.mpz, n_.mpz, cb);
            when Round.DOWN do mpz_fdiv_q_2exp(this.mpz, n_.mpz, cb);
            when Round.ZERO do mpz_tdiv_q_2exp(this.mpz, n_.mpz, cb);
          }
        } else {
          select rounding {
            when Round.UP   do mpz_cdiv_q_2exp(this.mpz, n.mpz, cb);
            when Round.DOWN do mpz_fdiv_q_2exp(this.mpz, n.mpz, cb);
            when Round.ZERO do mpz_tdiv_q_2exp(this.mpz, n.mpz, cb);
          }
        }
      }
    }
    proc div_r_2exp(param rounding: Round, const ref n: BigInt, b: uint) {
      on Locales[this.locale_id] {
        const cb = b.safeCast(c_ulong);
        if (here.id != n.locale_id) {
          var n_ = n;
         select rounding {
           when Round.UP   do mpz_cdiv_r_2exp(this.mpz, n_.mpz, cb);
            when Round.DOWN do mpz_fdiv_r_2exp(this.mpz, n_.mpz, cb);
            when Round.ZERO do mpz_tdiv_r_2exp(this.mpz, n_.mpz, cb);
          }
        } else {
          select rounding {
            when Round.UP   do mpz_cdiv_r_2exp(this.mpz, n.mpz, cb);
            when Round.DOWN do mpz_fdiv_r_2exp(this.mpz, n.mpz, cb);
            when Round.ZERO do mpz_tdiv_r_2exp(this.mpz, n.mpz, cb);
          }
        }
      }
    }
    proc mod(const ref n: BigInt, const ref d: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != n.locale_id || here.id != d.locale_id) {
          var n_ = n;
          var d_ = d;
          mpz_mod(this.mpz, n_.mpz, d_.mpz);
        } else {
          mpz_mod(this.mpz, n.mpz, d.mpz);
        }
      }
    }
    proc mod(const ref n: BigInt, d: uint):uint {
      var ret:c_ulong;
      on Locales[this.locale_id] {
        if (here.id != n.locale_id) {
          var n_ = n;
          ret=mpz_mod_ui(this.mpz, n_.mpz, d.safeCast(c_ulong));
        } else {
          ret=mpz_mod_ui(this.mpz, n.mpz, d.safeCast(c_ulong));
        }
      }
      return ret.safeCast(uint);
    }
    proc divexact(const ref n: BigInt, const ref d: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != n.locale_id || here.id != d.locale_id) {
          var n_ = n;
          var d_ = d;
          mpz_divexact(this.mpz, n_.mpz, d_.mpz);
        } else {
          mpz_divexact(this.mpz, n.mpz, d.mpz);
        }
      }
    }
    proc divexact(const ref n: BigInt, d: uint) {
      on Locales[this.locale_id] {
        if (here.id != n.locale_id) {
          var n_ = n;
          mpz_divexact_ui(this.mpz, n_.mpz, d.safeCast(c_ulong));
        } else {
          mpz_divexact_ui(this.mpz, n.mpz, d.safeCast(c_ulong));
        }
      }
    }
    proc divisible_p(const ref d: BigInt):int {
      var ret:c_int;
      on Locales[this.locale_id] {
        if (here.id != d.locale_id) {
        var d_ = d;
        ret=mpz_divisible_p(this.mpz, d_.mpz);
        } else {
          ret=mpz_divisible_p(this.mpz, d.mpz);
        }
      }
      return ret.safeCast(int);
    }
    proc divisible_p(d: uint):int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret=mpz_divisible_ui_p(this.mpz, d.safeCast(c_ulong));
      }
      return ret.safeCast(int);
    }
    proc divisible_2exp_p(b: uint):int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret=mpz_divisible_2exp_p(this.mpz, b.safeCast(c_ulong));
      }
      return ret.safeCast(int);
    }
    proc congruent_p(const ref c: BigInt, const ref d: BigInt):int {
      var ret:c_int;
      on Locales[this.locale_id] {
        if (here.id != c.locale_id || here.id != d.locale_id) {
          var c_ = c;
          var d_ = d;
          ret=mpz_congruent_p(this.mpz, c_.mpz, d_.mpz);
        } else {
          ret=mpz_congruent_p(this.mpz, c.mpz, d.mpz);
        }
      }
      return ret.safeCast(int);
    }
    proc congruent_p(c:uint, d: uint):int {
      var ret: c_int;
      on Locales[this.locale_id] {
        ret=mpz_congruent_ui_p(this.mpz, c.safeCast(c_ulong), d.safeCast(c_ulong));
      }
      return ret.safeCast(int);
    }
    proc congruent_2exp_p(const ref c: BigInt, b: uint):int {
      var ret: c_int;
      on Locales[this.locale_id] {
        if (here.id != c.locale_id) {
          var c_ = c;
          ret=mpz_congruent_2exp_p(this.mpz, c_.mpz, b.safeCast(c_ulong));
        } else {
          ret=mpz_congruent_2exp_p(this.mpz, c.mpz, b.safeCast(c_ulong));
        }
      }
      return ret.safeCast(int);
    }

    // Exponentiation Functions
    proc powm(const ref base: BigInt, const ref exp: BigInt, const ref mod: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != base.locale_id || here.id != exp.locale_id || here.id != mod.locale_id) {
          var b_ = base;
          var e_ = exp;
          var m_ = mod;
          mpz_powm(this.mpz, b_.mpz, e_.mpz, m_.mpz);
        } else {
          mpz_powm(this.mpz, base.mpz, exp.mpz, mod.mpz);
        }
      }
    }
    proc powm(const ref base: BigInt, exp:uint, const ref mod: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != base.locale_id || here.id != mod.locale_id) {
          var b_ = base;
          var m_ = mod;
          mpz_powm_ui(this.mpz, b_.mpz, exp.safeCast(c_ulong), m_.mpz);
        } else {
          mpz_powm_ui(this.mpz, base.mpz, exp.safeCast(c_ulong), mod.mpz);
        }
      }
    }
    proc pow(const ref base: BigInt, exp: uint) {
      on Locales[this.locale_id] {
        if (here.id != base.locale_id) {
        var b_ = base;
        mpz_pow_ui(this.mpz, b_.mpz, exp.safeCast(c_ulong));
        } else {
          mpz_pow_ui(this.mpz, base.mpz, exp.safeCast(c_ulong));
        }
      }
    }
    proc pow(base: uint, exp: uint) {
      on Locales[this.locale_id] {
        mpz_ui_pow_ui(this.mpz, base.safeCast(c_ulong), exp.safeCast(c_ulong));
      }
    }

    // Root Extraction Functions
    proc root(const ref a: BigInt, n: uint):int {
      var ret:c_int;
      on Locales[this.locale_id] {
        if (here.id != a.locale_id) {
        var a_ = a;
        ret=mpz_root(this.mpz, a_.mpz, n.safeCast(c_ulong));
        } else {
          ret=mpz_root(this.mpz, a.mpz, n.safeCast(c_ulong));
        }
      }
      return ret.safeCast(int);
    }
    // this gets root, rem gets remainder.
    proc rootrem(ref rem: BigInt, const ref u: BigInt, n: uint) {
      on Locales[this.locale_id] {
        if (here.id != rem.locale_id || here.id != u.locale_id) {
          var r_ = rem;
          var u_ = u;
          mpz_rootrem(this.mpz, r_.mpz, u_.mpz, n.safeCast(c_ulong));
          rem.set(r_);
        } else {
          mpz_rootrem(this.mpz, rem.mpz, u.mpz, n.safeCast(c_ulong));
        }
      }
    }
    proc sqrt(const ref a: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id) {
          var a_ = a;
          mpz_sqrt(this.mpz, a_.mpz);
        } else {
          mpz_sqrt(this.mpz, a.mpz);
        }
      }
    }
    // this gets root, rem gets remainder of a-root*root.
    proc sqrtrem(ref rem: BigInt, const ref a: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != rem.locale_id || here.id != a.locale_id) {
          var r_ = rem;
          var a_ = a;
          mpz_sqrtrem(this.mpz, r_.mpz, a_.mpz);
          rem.set(r_);
        } else {
          mpz_sqrtrem(this.mpz, rem.mpz, a.mpz);
        }
      }
    }
    proc perfect_power_p():int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret=mpz_perfect_power_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc perfect_square_p():int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret=mpz_perfect_square_p(this.mpz);
      }
      return ret.safeCast(int);
    }

    // Number Theoretic Functions

    // returns 2 if definitely prime, 0 if not prime, 1 if likely prime
    // reasonable number of reps is 15-50
    proc probab_prime_p(const reps: int):int {
      var ret: c_int;
      on Locales[this.locale_id] {
        ret=mpz_probab_prime_p(this.mpz, reps.safeCast(c_int));
      }
      return ret.safeCast(int);
    }
    proc nextprime(const ref a: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id) {
          var a_ = a;
          mpz_nextprime(this.mpz, a_.mpz);
        } else {
          mpz_nextprime(this.mpz, a.mpz);
        }
      }
    }
    proc gcd(const ref a: BigInt, const ref b: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id || here.id != b.locale_id) {
          var a_ = a;
          var b_ = b;
          mpz_gcd(this.mpz, a_.mpz, b_.mpz);
        } else {
          mpz_gcd(this.mpz, a.mpz, b.mpz);
        }
      }
    }
    proc gcd(const ref a: BigInt, b: uint) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id) {
          var a_ = a;
          mpz_gcd_ui(this.mpz, a_.mpz, b.safeCast(c_ulong));
        } else {
          mpz_gcd_ui(this.mpz, a.mpz, b.safeCast(c_ulong));
        }
      }
    }
    // sets this to gcd(a,b)
    // set s and t to to coefficients satisfying a*s + b*t == g
    proc gcdext(ref s: BigInt, ref t: BigInt, const ref a: BigInt, const ref b: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id || here.id != b.locale_id || here.id != s.locale_id || here.id != t.locale_id) {
          var a_ = a;
          var b_ = b;
          var s_ = s;
          var t_ = t;
          mpz_gcdext(this.mpz, s_.mpz, t_.mpz, a_.mpz, b_.mpz);
          s.set(s_);
          t.set(t_);
        } else {
          mpz_gcdext(this.mpz, s.mpz, t.mpz, a.mpz, b.mpz);
        }
      }
    }
    proc lcm(const ref a: BigInt, const ref b: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id || here.id != b.locale_id) {
          var a_ = a;
          var b_ = b;
          mpz_lcm(this.mpz, a_.mpz, b_.mpz);
        } else {
          mpz_lcm(this.mpz, a.mpz, b.mpz);
        }
      }
    }
    proc lcm(const ref a: BigInt, b: uint) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id) {
          var a_ = a;
          mpz_lcm_ui(this.mpz, a_.mpz, b.safeCast(c_ulong));
        } else {
          mpz_lcm_ui(this.mpz, a.mpz, b.safeCast(c_ulong));
        }
      }
    }
    proc invert(const ref a: BigInt, const ref b: BigInt):int {
      var ret:c_int;
      on Locales[this.locale_id] {
        if (here.id != a.locale_id || here.id != b.locale_id) {
          var a_ = a;
          var b_ = b;
          ret=mpz_invert(this.mpz, a_.mpz, b_.mpz);
        } else {
          ret=mpz_invert(this.mpz, a.mpz, b.mpz);
        }
      }
      return ret.safeCast(int);
    }

    // jacobi, legendre, kronecker functions are defined outside this record.

    proc remove(const ref a: BigInt, const ref f: BigInt):uint {
      var ret:c_ulong;
      on Locales[this.locale_id] {
        if (here.id != a.locale_id || here.id != f.locale_id) {
          var a_ = a;
          var f_ = f;
          ret=mpz_remove(this.mpz, a_.mpz, f_.mpz);
        } else {
          ret=mpz_remove(this.mpz, a.mpz, f.mpz);
        }
      }
      return ret.safeCast(uint);
    }
    proc fac(a: uint) {
      on Locales[this.locale_id] {
        mpz_fac_ui(this.mpz, a.safeCast(c_ulong));
      }
    }
    // TODO: functions including mpz_2fac_ui, mpz_primorial_ui, and 
    // mpz_mfac_uiui are not included in this module. 

    proc bin(const ref n: BigInt, k: uint) {
      on Locales[this.locale_id] {
        if (here.id != n.locale_id) {
          var n_ = n;
          mpz_bin_ui(this.mpz, n_.mpz, k.safeCast(c_ulong));
        } else {
          mpz_bin_ui(this.mpz, n.mpz, k.safeCast(c_ulong));
        }
      }
    }
    proc bin(n: uint, k: uint) {
      on Locales[this.locale_id] {
        mpz_bin_uiui(this.mpz, n.safeCast(c_ulong), k.safeCast(c_ulong));
      }
    }
    proc fib(n: uint) {
      on Locales[this.locale_id] {
        mpz_fib_ui(this.mpz, n.safeCast(c_ulong));
      }
    }
    proc fib2(ref fnsub1: BigInt, n: uint) {
      on Locales[this.locale_id] {
        if (here.id != fnsub1.locale_id) {
          var f_ = fnsub1;
          mpz_fib2_ui(this.mpz, f_.mpz, n.safeCast(c_ulong));
          fnsub1.set(f_);
        } else {
          mpz_fib2_ui(this.mpz, fnsub1.mpz, n.safeCast(c_ulong));
        }
      }
    }
    proc lucnum(n: uint) {
      on Locales[this.locale_id] {
        mpz_lucnum_ui(this.mpz, n.safeCast(c_ulong));
      }
    }
    proc lucnum2(ref lnsub1: BigInt, n: uint) {
      on Locales[this.locale_id] {
        if (here.id != lnsub1.locale_id) {
          var f_ = lnsub1;
          mpz_lucnum2_ui(this.mpz, f_.mpz, n.safeCast(c_ulong));
          lnsub1.set(f_);
        } else {
          mpz_lucnum2_ui(this.mpz, lnsub1.mpz, n.safeCast(c_ulong));
        }
      }
    }

    // Comparison Functions
    proc cmp(const ref b: BigInt):int {
      var ret:c_int;
      on Locales[this.locale_id] {
        if (here.id != b.locale_id) {
          var b_ = b;
          ret=mpz_cmp(this.mpz, b_.mpz);
        } else {
          ret=mpz_cmp(this.mpz, b.mpz);
        }
      }
      return ret.safeCast(int);
    }
    proc cmp(b: real):int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret=mpz_cmp_d(this.mpz, b: c_double);
      }
      return ret.safeCast(int);
    }
    proc cmp(b: int):int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret=mpz_cmp_si(this.mpz, b.safeCast(c_long));
      }
      return ret.safeCast(int);
    }
    proc cmp(b: uint):int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret=mpz_cmp_ui(this.mpz, b.safeCast(c_ulong));
      }
      return ret.safeCast(int);
    }
    proc cmpabs(const ref b: BigInt):int {
      var ret:c_int;
      on Locales[this.locale_id] {
        if here.id != b.locale_id {
          var b_ = b;
          ret=mpz_cmpabs(this.mpz, b_.mpz);
        } else {
          ret = mpz_cmpabs(this.mpz, b.mpz);
        }
      }
      return ret.safeCast(int);
    }
    proc cmpabs(b: real):int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret=mpz_cmpabs_d(this.mpz, b: c_double);
      }
      return ret.safeCast(int);
    }
    proc cmpabs(b: uint):int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret=mpz_cmpabs_ui(this.mpz, b.safeCast(c_ulong));
      }
      return ret.safeCast(int);
    }
    proc sgn():int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret=mpz_sgn(this.mpz);
      }
      return ret.safeCast(int);
    }

    // Logical and Bit Manipulation Functions
    proc and(const ref a: BigInt, const ref b: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id || here.id != b.locale_id) {
          var a_ = a;
          var b_ = b;
          mpz_and(this.mpz, a_.mpz, b_.mpz);
        } else {
          mpz_and(this.mpz, a.mpz, b.mpz);
        }
      }
    }
    proc ior(const ref a: BigInt, const ref b: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id || here.id != b.locale_id) {
          var a_ = a;
          var b_ = b;
          mpz_ior(this.mpz, a_.mpz, b_.mpz);
        } else {
          mpz_ior(this.mpz, a.mpz, b.mpz);
        }
      }
    }
    proc xor(const ref a: BigInt, const ref b: BigInt) {
      on Locales[this.locale_id] {
        if (here.id != a.locale_id || here.id != b.locale_id) {
          var a_ = a;
          var b_ = b;
          mpz_xor(this.mpz, a_.mpz, b_.mpz);
        } else {
          mpz_xor(this.mpz, a.mpz, b.mpz);
        }
      }
    }
    proc com(const ref a: BigInt) {
      on Locales[this.locale_id] {
        if here.id != a.locale_id {
          var a_ = a;
          mpz_com(this.mpz, a_.mpz);
        } else {
          mpz_com(this.mpz, a.mpz);
        }
      }
    }
    proc popcount():uint {
      var ret:c_ulong;
      on Locales[this.locale_id] {
        ret = mpz_popcount(this.mpz);
      }
      return ret.safeCast(uint);
    }
    proc hamdist(const ref b: BigInt):uint {
      var ret:c_ulong;
      on Locales[this.locale_id] {
        if here.id != b.locale_id {
          var b_ = b;
          ret=mpz_hamdist(this.mpz, b_.mpz);
        } else {
          ret=mpz_hamdist(this.mpz, b.mpz);
        }
      }
      return ret.safeCast(uint);
    }
    proc scan0(starting_bit: uint):uint {
      var ret:c_ulong;
      on Locales[this.locale_id] {
        ret = mpz_scan0(this.mpz, starting_bit.safeCast(c_ulong));
      }
      return ret.safeCast(uint);
    }
    proc scan1(starting_bit: uint):uint {
      var ret:c_ulong;
      on Locales[this.locale_id] {
        ret = mpz_scan1(this.mpz, starting_bit.safeCast(c_ulong));
      }
      return ret.safeCast(uint);
    }
    proc setbit(bit_index: uint) {
      on Locales[this.locale_id] {
        mpz_setbit(this.mpz, bit_index.safeCast(c_ulong));
      }
    }
    proc clrbit(bit_index: uint) {
      on Locales[this.locale_id] {
        mpz_clrbit(this.mpz, bit_index.safeCast(c_ulong));
      }
    }
    proc combit(bit_index: uint) {
      on Locales[this.locale_id] {
        mpz_combit(this.mpz, bit_index.safeCast(c_ulong));
      }
    }
    proc tstbit(bit_index: uint):int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret = mpz_tstbit(this.mpz, bit_index.safeCast(c_ulong));
      }
      return ret.safeCast(int);
    }

    // Miscellaneous Functions
    proc fits_ulong_p():int {
      var ret: c_int;
      on Locales[this.locale_id] {
        ret = mpz_fits_ulong_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc fits_slong_p():int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret = mpz_fits_slong_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc fits_uint_p():int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret = mpz_fits_uint_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc fits_sint_p():int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret = mpz_fits_sint_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc fits_ushort_p():int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret = mpz_fits_ushort_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc fits_sshort_p():int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret = mpz_fits_sshort_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc odd_p():int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret = mpz_odd_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc even_p():int {
      var ret:c_int;
      on Locales[this.locale_id] {
        ret = mpz_even_p(this.mpz);
      }
      return ret.safeCast(int);
    }
    proc sizeinbase(base: int):uint {
      var ret:size_t;
      on Locales[this.locale_id] {
        ret = mpz_sizeinbase(this.mpz, base.safeCast(c_int));
      }
      return ret.safeCast(uint);
    }

    // Integer random functions are in the GMPRandom class.

    proc realloc2(nbits: uint) {
      on Locales[this.locale_id] {
        mpz_realloc2(this.mpz, nbits.safeCast(c_ulong));
      }
    }

    proc get_limbn(n: uint):uint {
      var ret:mp_limb_t;
      on Locales[this.locale_id] {
        ret = mpz_getlimbn(this.mpz, n.safeCast(mp_size_t));
      }
      return ret.safeCast(uint);
    }
    proc size():size_t {
      var ret:size_t;
      on Locales[this.locale_id] {
        ret = mpz_size(this.mpz);
      }
      return ret;
    }
    proc debugprint() {
      writeln("On locale ",this.locale);
      on Locales[this.locale_id] {
        chpl_gmp_mpz_print(this.mpz);
      }
    }

  }

  // Assignment function -- deep copies `rhs` into `lhs`
  proc =(ref lhs: BigInt, const ref rhs: BigInt) {
    inline proc helpMe(ref lhs: BigInt, rhs: BigInt) {
      if _local || rhs.locale_id == lhs.locale_id {
        lhs.reinitBigInt(rhs.mpz, needToCopy=true);
      } else {
        on Locales[lhs.locale_id] {
          var mpz_struct = rhs.mpz[1];
          chpl_gmp_get_mpz(lhs.mpz, rhs.locale_id, mpz_struct);
          lhs.owned = true;
        }
      }
    }
    if _local || lhs.locale_id == chpl_nodeID {
      helpMe(lhs, rhs);
    } else {
      on __primitive("chpl_on_locale_num", chpl_buildLocaleID(lhs.locale_id, c_sublocid_any)) {
        helpMe(lhs, rhs);
      }
    }
  }

  // assignment operator overload for signed int assignment
  proc =(ref lhs: BigInt, rhs: int) {
    // TODO: does this need an "on Locales[lhs.locale_id]" ?
    lhs.set(rhs);
  }

  // autoCopy and initCopy can be called by the compiler
  pragma "donor fn"
  pragma "auto copy fn"
  pragma "no doc"
  proc chpl__autoCopy(const ref bir: BigInt) {
    // this pragma may not be needed
    pragma "no auto destroy"
    var ret : BigInt;
    if _local || bir.locale_id == chpl_nodeID {
      if bir.owned {
        if ret.owned then on ret.locale do mpz_clear(ret.mpz);
        mpz_init_set(ret.mpz, bir.mpz);
        ret.owned = true;
      } else {
        ret.mpz = bir.mpz; // make a shallow copy
        ret.owned = false;
      }
    } else {
      var remoteMpz = bir.mpz;
      ret.reinitBigInt(remoteMpz, needToCopy=false);
    }
    return ret;
  }

  pragma "init copy fn"
  pragma "no doc"
  proc chpl__initCopy(const ref bir: BigInt) {
    // This pragma may be unnecessary.
    // pragma "no auto destroy"
    var ret : BigInt;
    if _local || bir.locale_id == chpl_nodeID {
      if bir.owned {
        if ret.owned then on Locales[ret.locale_id] do mpz_clear(ret.mpz);
        mpz_init_set(ret.mpz, bir.mpz);
        ret.owned = true;
      } else {
        ret.mpz = bir.mpz; // make a shalow copy
        ret.owned = false;
      }
    } else {
      ret.set(bir);
    }
    return ret;
  }

  proc BigInt.writeThis(writer) {
    var s: string;
    on Locales[this.locale_id] {
      s = this.get_str();
    }
    writer.write(s);
  }

  // Operator Overloads 
  proc +=(ref a: BigInt, const ref b: BigInt) {
    a.add(a, b);
  }
  proc +=(ref a: BigInt, b: int) {
    if (b > 0) {
      a.add(a, b:uint);
    } else {
      a.sub(a, abs(b):uint);
    }
  }
  proc +=(ref a: BigInt, b: uint) {
    a.add(a, b);
  }
  proc -=(ref a: BigInt, const ref b: BigInt) {
    a.sub(a, b);
  }
  proc -=(ref a: BigInt, b: int) {
    if b > 0 {
      a.sub(a, abs(b):uint);
    } else {
      a.add(a, abs(b):uint);
    }
  }
  proc -=(ref a: BigInt, b: uint) {
    a.sub(a, b);
  }
  proc *=(ref a: BigInt, const ref b: BigInt) {
    a.mul(a, b);
  }
  proc *=(ref a: BigInt, b: int) {
    a.mul(a, b);
  }
  proc *=(ref a: BigInt, b: uint) {
    a.mul(a, b);
  }
  proc /=(ref a: BigInt, const ref b: BigInt) {
    a.div_q(Round.DOWN, a, b);
  }
  proc /=(ref a: BigInt, b: int) {
    a.div_q(Round.ZERO, a, abs(b):uint);
    if (b < 0) {
    a.neg(a);
    }
  }
  proc /=(ref a: BigInt, b: uint) {
    a.div_q(Round.DOWN, a, b);
  }
  proc **=(ref a: BigInt, b: uint) {
    a.pow(a, b);
  }
  proc %=(ref a: BigInt, const ref b: BigInt) {
   a.mod(a, b);
  }
  proc %=(ref a: BigInt, b: int) {
    a.mod(a, abs(b):uint); // in C (a mod b) and (a mod -b) are the same
  }
  proc %=(ref a: BigInt, b: uint) {
    a.mod(a, b);
  }
  // Only bitwise operators with pairs of BigInts are supported
  proc &=(ref a: BigInt, const ref b: BigInt) {
    a.and(a, b);
  }
  proc |=(ref a: BigInt, const ref b: BigInt) {
    a.ior(a, b);
  }
  proc ^=(ref a: BigInt, const ref b: BigInt) {
    a.xor(a, b);
  }
  // &&= and ||= are not supported
  proc <<=(ref a: BigInt, b: uint) {
    a.mul_2exp(a, b);
  }
  proc <<=(ref a: BigInt, b: int) {
    if b < 0 {
      a.div_2exp(a, abs(b):uint);
    } else {
      a.mul_2exp(a, b:uint);
    }
  }
  proc >>=(ref a: BigInt, b: uint) {
    a.div_2exp(a, b);
  }
  proc >>=(ref a: BigInt, b: int) {
    if (b < 0) {
      a.mul_2exp(a, abs(b):uint);
    } else {
      a.div_2exp(a, b:uint);
    }
  }
  proc <=>(ref a: BigInt, ref b: BigInt) {
    var c = new BigInt(a);
    a.set(b);
    b.set(c);
  }
  proc **(const ref a: BigInt, b: uint) {
    var c = new BigInt();
    c.pow(a, b);
    return c;
  }
  proc ~(const ref a: BigInt) {
    var c = new BigInt();
    c.com(a); // 1s complement is the same as bitwise negation
    return c;
  }
  proc *(const ref a: BigInt, const ref b: BigInt) {
    var c = new BigInt();
    c.mul(a, b);
    return c;
  }
  proc *(const ref a: BigInt, b: int) {
    var c = new BigInt();
    c.mul(a, b);
    return c;
  }
  proc *(b: int, const ref a: BigInt) {
    var c = new BigInt();
    c.mul(a, b);
    return c;
  }
  proc *(const ref a: BigInt, b: uint) {
    var c = new BigInt();
    c.mul(a, b);
    return c;
  }
  proc *(b: uint, const ref a: BigInt) {
    var c = new BigInt();
    c.mul(a, b);
    return c;
  }
  // TODO: should / take lhs int args?
  proc /(const ref a: BigInt, const ref b: BigInt) {
    var c = new BigInt();
    c.div_q(Round.DOWN, a, b);
    return c;
  }
  proc /(const ref a: BigInt, b: uint) {
    var c = new BigInt();
    c.div_q(Round.DOWN, a, b);
    return c;
  }
  proc /(const ref a: BigInt, b: int) {
    var c = new BigInt();
    c.div_q(Round.DOWN, a, abs(b):uint);
    if b < 0 then c.neg(c);
    return c;
  }
  // TODO: should % take lhs int args?
  proc %(const ref a: BigInt, const ref b: BigInt) {
    var c = new BigInt();
    c.mod(a, b);
    return c;
  }
  proc %(const ref a: BigInt, b: uint) {
    var c = new BigInt();
    c.mod(a, b);
    return c;
  }
  proc %(const ref a: BigInt, b: int) {
    var c = new BigInt();
    c.mod(a, abs(b):uint); // in C (a mod b) and (a mod -b) are the same
    return c;
  }
  proc +(const ref a: BigInt) {
    return new BigInt(a);
  }
  proc -(const ref a: BigInt) {
    var c = new BigInt();
    c.neg(a);
    return c;
  }
  // NOTE: The >> operator is implemented in GMP via division so it rounds 
  // towards 0, which may be unexpected undefined behavior. 
  proc <<(const ref a: BigInt, b: uint) {
    var c = new BigInt();
    c.mul_2exp(a, b);
    return c;
  }
  proc <<(const ref a: BigInt, b: int) {
    var c = new BigInt();
    if b < 0 {
      c.div_2exp(a, abs(b):uint);
    } else {
      c.mul_2exp(a, b:uint);
    }
    return c;
  }
  proc >>(const ref a: BigInt, b: uint) {
    var c = new BigInt();
    c.div_2exp(a, b);
    return c;
  }
  proc >>(const ref a: BigInt, b: int) {
    var c = new BigInt();
    if b < 0 {
      c.mul_2exp(a, abs(b):uint);
    } else {
      c.div_2exp(a, b:uint);
    }
    return c;
  }
  proc &(const ref a: BigInt, const ref b: BigInt) {
    var c = new BigInt();
    c.and(a, b);
    return c;
  }
  proc &(const ref a: BigInt, b: uint) {
    var c = new BigInt(b);
    c.and(a, c);
    return c;
  }
  proc ^(const ref a: BigInt, const ref b: BigInt) {
    var c = new BigInt();
    c.ior(a, b);
    return c;
  }
  proc ^(const ref a: BigInt, b: uint) {
    var c = new BigInt(b);
    c.ior(a, c);
    return c;
  }
  proc |(const ref a: BigInt, const ref b: BigInt) {
    var c = new BigInt();
    c.xor(a, b);
    return c;
  }
  proc |(const ref a: BigInt, b: uint) {
    var c = new BigInt(b);
    c.xor(a, c);
    return c;
  }
  proc +(const ref a: BigInt, const ref b: BigInt) {
    var c = new BigInt();
    c.add(a, b);
    return c;
  }
  proc +(const ref a: BigInt, b: uint) {
    var c = new BigInt();
    c.add(a, b);
    return c;
  }
  proc +(b: uint, const ref a: BigInt) {
    var c = new BigInt();
    c.add(a, b);
    return c;
  }
  proc +(const ref a: BigInt, b: int) {
    if b < 0 {
      return a - abs(b):uint;
    } else {
      return a + b:uint;
    }
  }
  proc +(b: int, const ref a: BigInt) {
    if b < 0 {
      return a - abs(b):uint;
    } else {
      return a + b:uint;
    }
  }
  proc -(const ref a: BigInt, const ref b: BigInt) {
    var c = new BigInt();
    c.sub(a, b);
    return c;
  }
  proc -(const ref a: BigInt, b: uint) {
    var c = new BigInt();
    c.sub(a, b);
    return c;
  }
   proc -(b: uint, const ref a: BigInt) {
    var c = new BigInt();
    c.sub(b, a);
    return c;
  }
  proc -(const ref a: BigInt, b: int) {
        if b < 0 {
      return a + abs(b):uint;
    } else {
      return a - b:uint;
    }
  }
  proc -(b: int, const ref a: BigInt) {
    if b < 0 {
      var c = new BigInt(a);
      c.neg(c);
      return (c + b);
    } else {
      return b:uint - a;
    }
  }
  proc >=(const ref a: BigInt, const ref b: BigInt) {
    var f = a.cmp(b);
    return f >= 0;
  }
  proc >=(const ref a: BigInt, b: int) {
    var f = a.cmp(b);
    return f >= 0;
  }
  proc >=(b: int, const ref a: BigInt) {
    var f = a.cmp(b);
    return f <= 0;
  }
  proc >=(const ref a: BigInt, b: uint) {
    var f = a.cmp(b);
    return f >= 0;
  }
  proc >=(b: uint, const ref a: BigInt) {
    var f = a.cmp(b);
    return f <= 0;
  }
  proc <=(const ref a: BigInt, const ref b: BigInt) {
    var f = a.cmp(b);
    return f <= 0;
  }
  proc <=(const ref a: BigInt, b: int) {
    var f = a.cmp(b);
    return f <= 0;
  }
  proc <=(b: int, const ref a: BigInt) {
    var f = a.cmp(b);
    return f >= 0;
  }
  proc <=(const ref a: BigInt, b: uint) {
    var f = a.cmp(b);
    return f <= 0;
  }
  proc <=(b: uint, const ref a: BigInt) {
    var f = a.cmp(b);
    return f >= 0;
  }
  proc >(const ref a: BigInt, const ref b: BigInt) {
    var f = a.cmp(b);
    return f > 0;
  }
  proc >(const ref a: BigInt, b: int) {
    var f = a.cmp(b);
    return f > 0;
  }
  proc >(b: int, const ref a: BigInt) {
    var f = a.cmp(b);
    return f < 0;
  }
  proc >(const ref a: BigInt, b: uint) {
    var f = a.cmp(b);
    return f > 0;
  }
  proc >(b: uint, const ref a: BigInt) {
    var f = a.cmp(b);
    return f < 0;
  }
  proc <(const ref a: BigInt, const ref b: BigInt) {
    var f = a.cmp(b);
    return f < 0;
  }
  proc <(const ref a: BigInt, b: int) {
    var f = a.cmp(b);
    return f < 0;
  }
  proc <(b: int, const ref a: BigInt) {
    var f = a.cmp(b);
    return f > 0;
  }
  proc <(const ref a: BigInt, b: uint) {
    var f = a.cmp(b);
    return f < 0;
  }
  proc <(b: uint, const ref a: BigInt) {
    var f = a.cmp(b);
    return f > 0;
  }
  proc ==(const ref a: BigInt, const ref b: BigInt) {
    var f = a.cmp(b);
    return f == 0;
  }
  proc ==(const ref a: BigInt, b: int) {
    var f = a.cmp(b);
    return f == 0;
  }
  proc ==(b: int, const ref a: BigInt) {
    var f = a.cmp(b);
    return f == 0;
  }
  proc ==(const ref a: BigInt, b: uint) {
    var f = a.cmp(b);
    return f == 0;
  }
  proc ==(b: uint, const ref a: BigInt) {
    var f = a.cmp(b);
    return f == 0;
  }
  proc !=(const ref a: BigInt, const ref b: BigInt) {
    var f = a.cmp(b);
    return f != 0;
  }
  proc !=(const ref a: BigInt, b: int) {
    var f = a.cmp(b);
    return f != 0;
  }
  proc !=(b: int, const ref a: BigInt) {
    var f = a.cmp(b);
    return f != 0;
  }
  proc !=(const ref a: BigInt, b: uint) {
    var f = a.cmp(b);
    return f != 0;
  }
  proc !=(b: uint, const ref a: BigInt) {
    var f = a.cmp(b);
    return f != 0;
  }

  // The following functions return a value instead of setting a BigInt
  proc div(param rounding: Round, const ref n: BigInt, d: uint):uint {
    var ret:c_ulong;
    const cd = d.safeCast(c_ulong);
    on Locales[n.locale_id] {
      select rounding {
        when Round.UP   do ret=mpz_cdiv_ui(n.mpz, cd);
        when Round.DOWN do ret=mpz_fdiv_ui(n.mpz, cd);
        when Round.ZERO do ret=mpz_tdiv_ui(n.mpz, cd);
      }
    }
    return ret.safeCast(uint);
  }
  proc jacobi(const ref a: BigInt, const ref b: BigInt):int {
    var ret:c_int;
    on Locales[a.locale_id] {
      if here.id != b.locale_id {
        var b_ = b;
        ret=mpz_jacobi(a.mpz, b_.mpz);
      } else {
        ret =mpz_jacobi(a.mpz, b.mpz);
      }
    }
    return ret.safeCast(int);
  }
  proc legendre(const ref a: BigInt, const ref p: BigInt):int {
    var ret:c_int;
    on Locales[a.locale_id] {
      if here.id != p.locale_id {
        var p_ = p;
        ret=mpz_legendre(a.mpz, p_.mpz);
      } else {
        ret=mpz_legendre(a.mpz, p.mpz);
      }
    }
    return ret.safeCast(int);
  }
  proc kronecker(const ref a: BigInt, const ref b: BigInt):int {
    var ret:c_int;
    on Locales[a.locale_id] {
      if here.id != b.locale_id {
        var b_ = b;
        ret=mpz_kronecker(a.mpz, b_.mpz);
      } else {
        ret = mpz_kronecker(a.mpz, b.mpz);
      }
    }
    return ret.safeCast(int);
  }
  proc kronecker(const ref a: BigInt, b: int):int {
    var ret:c_int;
    on Locales[a.locale_id] {
      ret=mpz_kronecker_si(a.mpz, b.safeCast(c_long));
    }
    return ret.safeCast(int);
  }
  proc kronecker(const ref a: BigInt, b: uint):int {
    var ret:c_int;
    on Locales[a.locale_id] {
      ret=mpz_kronecker_ui(a.mpz, b.safeCast(c_ulong));
    }
    return ret.safeCast(int);
  }
  proc kronecker(a: int, const ref b: BigInt):int {
    var ret:c_int;
    on Locales[b.locale_id] {
      ret=mpz_si_kronecker(a.safeCast(c_long), b.mpz);
    }
    return ret.safeCast(int);
  }
  proc kronecker(a: uint, const ref b: BigInt):int {
    var ret:c_int;
    on Locales[b.locale_id] {
      ret=mpz_ui_kronecker(a.safeCast(c_ulong), b.mpz);
    }
    return ret.safeCast(int);
  }

}
