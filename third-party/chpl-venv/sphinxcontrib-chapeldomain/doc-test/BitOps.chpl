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

/**
 *  Bit Manipulation Functions
 */
module BitOps {

  /*
    count leading zeros

    :arg x: unsigned integer of size `bits`; `bits` can be: 8, 16, 32, 64

    :returns: the number of 0 bits before the most significant 1 bit in `x` as
              `x.type`
   */
  inline proc clz(x: uint(?bits)): uint(bits) {
  }

  /*
    count leading zeros

    :arg x: integer of size `bits`; `bits` can be: 8, 16, 32, 64

    :returns: the number of 0 bits before the most significant 1 bit in `x` as
              `x.type`
   */
  inline proc clz(x: int(?bits)) {
  }

  /*
    count trailing zeros

    :arg x: unsigned integer of size `bits`; `bits` can be: 8, 16, 32, 64

    :returns: the number of 0 bits after the least significant 1 bit in `x` as
              `x.type`
   */
  inline proc ctz(x: uint(?bits)) {
  }

  /*
    count trailing zeros

    :arg x: integer of size `bits`; `bits` can be: 8, 16, 32, 64

    :returns: the number of 0 bits after the least significant 1 bit in `x` as
              `x.type`
   */
  inline proc ctz(x: int(?bits)) {
  }

  /*
    population count

    :arg x: unsigned integer of size `bits`; `bits` can be: 8, 16, 32, 64

    :returns: the number of 1 bits set in `x` as `x.type`
   */
  inline proc popcount(x: uint(?bits)): uint(bits) {
  }

  /*
    population count

    :arg x: integer of size `bits`; `bits` can be: 8, 16, 32, 64

    :returns: the number of 1 bits set in `x` as `x.type`
   */
  inline proc popcount(x: int(?bits)): int(bits) {
  }

  // Legacy operations
  // -----------------
  // kyleb: a few tests use these, but I'm not sure if there too much value

  // do a bitwise matrix-multiplication of x and y treating
  // them as 8x8 bit matrices and using | as the combinator
  // function.  There should be a similar version using xor
  // as the combinator, but I'm not sure where that would
  // go to be perfectly truthful
  proc bitMatMultOr(x: uint(64), y: uint(64)): uint(64) {
  }


  inline proc bitRotLeft(x, shift) {
  }


  inline proc bitRotRight(x: uint(64), shift) {
  }
}

/**
 * module to hide the extern procedures
 */
module BitOps_internal {
  extern proc chpl_bitops_popcount_32(x: c_uint) : uint(32);
  extern proc chpl_bitops_popcount_64(x: c_ulonglong) : uint(64);

  extern proc chpl_bitops_clz_32(x: c_uint) : uint(32);
  extern proc chpl_bitops_clz_64(x: c_ulonglong) : uint(64);

  extern proc chpl_bitops_ctz_32(x: c_uint) : uint(32);
  extern proc chpl_bitops_ctz_64(x: c_ulonglong) : uint(64);
}
