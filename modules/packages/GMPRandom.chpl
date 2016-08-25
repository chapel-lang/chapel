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

This module provides an interface with the GMP random number functions.

Dependencies
------------
This module is dependent on GMP (the GNU Multiple Precision Arithmetic 
Library). See the `GMP homepage <https://gmplib.org/>`_ for more information on
this library. See the GMP Chapel module for information on configuring GMP 
with Chapel. 

*/

module GMPRandom {
  use GMP;
  use BigIntegers;
    class GMPRandom {
    var state: gmp_randstate_t;
    proc GMPRandom()
    {
      gmp_randinit_default(this.state);
    }
    // Creates a Mersenne Twister (probably same as init_default)
    proc GMPRandom(twister: bool)
    {
      gmp_randinit_mt(this.state);
    }
    proc GMPRandom(ref a: BigInt, c: uint, m2exp: uint)
    {
      if here != a.locale {
        var a_ = a;
        gmp_randinit_lc_2exp(this.state, a_.mpz, c.safeCast(c_ulong), m2exp.safeCast(c_ulong));
      } else {
         gmp_randinit_lc_2exp(this.state, a.mpz, c.safeCast(c_ulong), m2exp.safeCast(c_ulong));
      }
    }
    proc GMPRandom(size: uint)
    {
      gmp_randinit_lc_2exp_size(this.state, size.safeCast(c_ulong));
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
    proc seed(ref seed: BigInt)
    {
      on this {
        if here != seed.locale {
          var s_ = seed;
          gmp_randseed(this.state, s_.mpz);
        } else {
          gmp_randseed(this.state, seed.mpz);
        }
      }
    }
    proc seed(seed: uint)
    {
      on this {
        gmp_randseed_ui(this.state, seed.safeCast(c_ulong));
      }
    }
    proc urandomb(nbits: uint):uint
    {
      var ret: c_ulong;
      on this {
        ret=gmp_urandomb_ui(this.state, nbits.safeCast(c_ulong));
      }
      return ret.safeCast(uint);
    }
    proc urandomm(n: uint):uint
    {
      var ret: c_ulong;
      on this {
        ret=gmp_urandomm_ui(this.state, n.safeCast(c_ulong));
      }
      return ret.safeCast(uint);
    }
    proc urandomb(ref r: BigInt, nbits: uint)
    {
      on this {
        if here != r.locale {
          var r_ = r;
          mpz_urandomb(r_.mpz, this.state, nbits.safeCast(c_ulong));
          r.set(r_);
        } else {
          mpz_urandomb(r.mpz, this.state, nbits.safeCast(c_ulong));
        }
      }
    }
    proc urandomm(ref r: BigInt, ref n: BigInt)
    {
      on this {
        if (here != r.locale || here != n.locale){
          var r_ = r;
          var n_ = n;
          mpz_urandomm(r_.mpz, this.state, n_.mpz);
          r.set(r_);
        } else {
          mpz_urandomm(r.mpz, this.state, n.mpz);
        }
      }
    }
    proc rrandomb(ref r: BigInt, nbits: uint)
    {
      on this {
        if here != r.locale {
          var r_ = r;
          mpz_rrandomb(r_.mpz, this.state, nbits.safeCast(c_ulong));
          r.set(r_);
        } else {
          mpz_rrandomb(r.mpz, this.state, nbits.safeCast(c_ulong));
        }
      }
    } 
  }
}
