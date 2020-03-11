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
  Bitwise operations implemented using C intrinsics when possible.
 */
module BitOps {
  import BitOps_internal;

  /*
    Count leading zeros in `x`.

    :returns: the number of 0 bits before the most significant 1 bit in `x`
    :rtype: `x.type`
   */
  inline proc clz(x: integral) {
    return BitOps_internal.clz(x);
  }

  /*
    Count trailing zeros in `x`.

    :returns: the number of 0 bits after the least significant 1 bit in `x`
    :rtype: `x.type`
   */
  inline proc ctz(x: integral) {
    return BitOps_internal.ctz(x);
  }

  /*
    Find the population count of `x`.

    :returns: the number of 1 bits set in `x` as `x.type`
    :rtype: `x.type`
   */
  inline proc popcount(x: integral) {
    return BitOps_internal.popcount(x);
  }

  /*
    Find the parity of `x`.

    :returns: * 0 -- when an even number of bits are set in `x`
              * 1 -- when an odd number of bits are set in `x`
    :rtype: `x.type`
   */
  inline proc parity(x: integral) {
    return BitOps_internal.parity(x);
  }

  /*
    Rotate `x` left.

    :arg x: integral of size `bits`
    :arg n: rotation amount, must be less than `bits`

    :returns: `x` rotated left by `n` bits
    :rtype: `x.type`
   */
  inline proc rotl(x: integral, n: integral) {
    return BitOps_internal.rotl(x, n);
  }

  /*
    Rotate `x` right.

    :arg x: integral of size `bits`
    :arg n: rotation amount, must be less than `bits`

    :returns: `x` rotated right by `n` bits
    :rtype: `x.type`
   */
  inline proc rotr(x: integral, n: integral) {
    return BitOps_internal.rotr(x, n);
  }

  // Legacy operations
  // -----------------
  // kyleb: a few tests use these, but I'm not sure if there too much value

  // do a bitwise matrix-multiplication of x and y treating
  // them as 8x8 bit matrices and using | as the combinator
  // function.  There should be a similar version using xor
  // as the combinator, but I'm not sure where that would
  // go to be perfectly truthful
  pragma "no doc"
  proc bitMatMultOr(x: uint(64), y: uint(64)): uint(64) {
    // return the transpose of x, treating it as an 8x8 bit-matrix.
    proc bitMatTrans(x: uint(64))
      return ((x & 0x8040201008040201)       |
              (x & 0x0080402010080402) <<  7 |
              (x & 0x0000804020100804) << 14 |
              (x & 0x0000008040201008) << 21 |
              (x & 0x0000000080402010) << 28 |
              (x & 0x0000000000804020) << 35 |
              (x & 0x0000000000008040) << 42 |
              (x & 0x0000000000000080) << 49 |
              (x >>  7) & 0x0080402010080402 |
              (x >> 14) & 0x0000804020100804 |
              (x >> 21) & 0x0000008040201008 |
              (x >> 28) & 0x0000000080402010 |
              (x >> 35) & 0x0000000000804020 |
              (x >> 42) & 0x0000000000008040 |
              (x >> 49) & 0x0000000000000080);

    // set every eighth bit to one or zero based on the results of the
    // eight reductions of the sets of eight bits
    inline proc byteOrReduce(u: uint(64)) {
      var t1 = u | (u >> 4);
      var t2 = t1 | (t1 >> 2);
      var t3 = t2 | (t2 >> 1);
      return t3 & 0x0101010101010101;
    }

    // set every bit in sets of eight to one if the last one is one
    inline proc byteExpand(u: uint(64))
      return 0x8080808080808080 ^ (0x8080808080808080 - u);

    var result:uint(64) = 0;
    var yTranspose = bitMatTrans(y);
    result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x8040201008040201;
    yTranspose = rotl(yTranspose, 8);
    result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x4020100804020180;
    yTranspose = rotl(yTranspose, 8);
    result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x2010080402018040;
    yTranspose = rotl(yTranspose, 8);
    result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x1008040201804020;
    yTranspose = rotl(yTranspose, 8);
    result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x0804020180402010;
    yTranspose = rotl(yTranspose, 8);
    result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x0402018040201008;
    yTranspose = rotl(yTranspose, 8);
    result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x0201804020100804;
    yTranspose = rotl(yTranspose, 8);
    result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x0180402010080402;
    return result;
  }

}

/**
 * module to hide the extern procedures
 */
private module BitOps_internal {
  private use SysCTypes;
  extern proc chpl_bitops_popcount_32(x: c_uint) : uint(32);
  extern proc chpl_bitops_popcount_64(x: c_ulonglong) : uint(64);

  extern proc chpl_bitops_clz_32(x: c_uint) : uint(32);
  extern proc chpl_bitops_clz_64(x: c_ulonglong) : uint(64);

  extern proc chpl_bitops_ctz_32(x: c_uint) : uint(32);
  extern proc chpl_bitops_ctz_64(x: c_ulonglong) : uint(64);

  extern proc chpl_bitops_parity_32(x: c_uint) : uint(32);
  extern proc chpl_bitops_parity_64(x: c_ulonglong) : uint(64);

  extern proc chpl_bitops_rotl_8(x: uint(8), n: uint(8)) : uint(8);
  extern proc chpl_bitops_rotl_16(x: uint(16), n: uint(16)) : uint(16);
  extern proc chpl_bitops_rotl_32(x: uint(32), n: uint(32)) : uint(32);
  extern proc chpl_bitops_rotl_64(x: uint(64), n: uint(64)) : uint(64);

  extern proc chpl_bitops_rotr_8(x: uint(8), n: uint(8)) : uint(8);
  extern proc chpl_bitops_rotr_16(x: uint(16), n: uint(16)) : uint(16);
  extern proc chpl_bitops_rotr_32(x: uint(32), n: uint(32)) : uint(32);
  extern proc chpl_bitops_rotr_64(x: uint(64), n: uint(64)) : uint(64);

  inline proc clz(x: uint(?bits)) {
    // the select will be folded out at compile time.
    select bits {
      when 64 do
        return chpl_bitops_clz_64(x);
      when 32 do
        return chpl_bitops_clz_32(x);
      when 16 do
        // gets promoted to 32bit, subtract the extra leading 0s
        return (chpl_bitops_clz_32(x)-16):uint(16);
      when 8 do
        // gets promoted to 32bit, subtract the extra leading 0s
        return (chpl_bitops_clz_32(x)-24):uint(8);
      otherwise
        // NOTE: this actually cant happen with how the integer types are setup
        //       right now - leaving it here for the future when we support
        //       >64bit types
        compilerError("clz is not supported for that bit width.");
    }
  }

  inline proc clz(x: int(?bits)) {
    return BitOps_internal.clz(x:uint(bits)):int(bits);
  }

  inline proc ctz(x: uint(?bits)) {
    // the select will be folded out at compile time.
    select bits {
      when 64 do
        return chpl_bitops_ctz_64(x);
      when 32 do
        return chpl_bitops_ctz_32(x);
      when 16 do
        return chpl_bitops_ctz_32(x):uint(16);
      when 8 do
        return chpl_bitops_ctz_32(x):uint(8);
      otherwise
        // NOTE: this actually cant happen with how the integer types are setup
        //       right now - leaving it here for the future when we support
        //       >64bit types
        compilerError("ctz is not supported for that bit width.");
    }
  }

  inline proc ctz(x: int(?bits)) {
    return BitOps_internal.ctz(x:uint(bits)):int(bits);
  }

  inline proc popcount(x: uint(?bits)) {
    // the select will be folded out at compile time.
    select bits {
      when 64 do
        return chpl_bitops_popcount_64(x);
      when 32 do
        return chpl_bitops_popcount_32(x);
      when 16 do
        return chpl_bitops_popcount_32(x):uint(16);
      when 8 do
        return chpl_bitops_popcount_32(x):uint(8);
      // In the future we could also break a large int (128+) into 64bit chucks
      // and add up the results.
      otherwise
        // NOTE: this actually cant happen with how the integer types are setup
        //       right now - leaving it here for the future when we support
        //       >64bit types
        compilerError("popcount is not supported for that bit width.");
    }
  }

  inline proc popcount(x: int(?bits)) {
    return BitOps_internal.popcount(x:uint(bits)):int(bits);
  }

  inline proc parity(x: uint(?bits)) {
    // the select will be folded out at compile time.
    select bits {
      when 64 do
        return chpl_bitops_parity_64(x);
      when 32 do
        return chpl_bitops_parity_32(x);
      when 16 do
        return chpl_bitops_parity_32(x):uint(16);
      when 8 do
        return chpl_bitops_parity_32(x):uint(8);
      otherwise
        compilerError("parity is not supported for that bit width.");
    }
  }

  inline proc parity(x: int(?bits)) {
    return BitOps_internal.parity(x:uint(bits)):int(bits);
  }

  inline proc rotl(x: uint(?bits), n: integral) {
    // the select will be folded out at compile time.
    select bits {
      when 64 do
        return chpl_bitops_rotl_64(x, n:uint(bits));
      when 32 do
        return chpl_bitops_rotl_32(x, n:uint(bits));
      when 16 do
        return chpl_bitops_rotl_16(x, n:uint(bits));
      when 8 do
        return chpl_bitops_rotl_8(x, n:uint(bits));
      otherwise
        compilerError("rotl is not supported for that bit width.");
    }
  }

  inline proc rotl(x: int(?bits), n: integral) {
    return BitOps_internal.rotl(x:uint(bits), n):int(bits);
  }

  inline proc rotr(x: uint(?bits), n: integral) {
    // the select will be folded out at compile time.
    select bits {
      when 64 do
        return chpl_bitops_rotr_64(x, n:uint(bits));
      when 32 do
        return chpl_bitops_rotr_32(x, n:uint(bits));
      when 16 do
        return chpl_bitops_rotr_16(x, n:uint(bits));
      when 8 do
        return chpl_bitops_rotr_8(x, n:uint(bits));
      otherwise
        compilerError("rotr is not supported for that bit width.");
    }
  }

  inline proc rotr(x: int(?bits), n: integral) {
    return BitOps_internal.rotr(x:uint(bits), n):int(bits);
  }
}
