/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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
  Bitwise operations implemented using C intrinsics when possible.
 */
module BitOps {
  /*
    Count leading zeros in `x`.

    :returns: the number of 0 bits before the most significant 1 bit in `x`
    :rtype: `x.type`
   */
  inline proc clz(x: integral) {
    param bits = numBits(x.type);
    if bits != 8 && bits != 16 && bits != 32 && bits != 64 {
      compilerError("clz is not supported for that bit width");
    }
    param funcBits = if bits == 8 || bits == 16 then 32 else bits;

    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern "chpl_bitops_clz_" + funcBits:string
    proc innerClz(x: uint(funcBits)): uint(funcBits);

    if bits == 8 then
      return (innerClz(x: uint(bits))-24): uint(bits): x.type;
    else if bits == 16 then
      return (innerClz(x: uint(bits))-16): uint(bits): x.type;
    else
      return innerClz(x: uint(bits)): uint(bits): x.type;
  }

  /*
    Count trailing zeros in `x`.

    :returns: the number of 0 bits after the least significant 1 bit in `x`
    :rtype: `x.type`
   */
  inline proc ctz(x: integral) {
    param bits = numBits(x.type);
    if bits != 8 && bits != 16 && bits != 32 && bits != 64 {
      compilerError("ctz is not supported for that bit width");
    }
    param funcBits = if bits == 8 || bits == 16 then 32 else bits;

    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern "chpl_bitops_ctz_" + funcBits:string
    proc innerCtz(x: uint(funcBits)): uint(funcBits);

    return innerCtz(x: uint(bits)): uint(bits): x.type;
  }

  /*
    Find the population count of `x`.

    :returns: the number of 1 bits set in `x` as `x.type`
    :rtype: `x.type`
   */
  inline proc popCount(x: integral) {
    param bits = numBits(x.type);
    if bits != 8 && bits != 16 && bits != 32 && bits != 64 {
      compilerError("popCount is not supported for that bit width");
    }
    param funcBits = if bits == 8 || bits == 16 then 32 else bits;

    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern "chpl_bitops_popcount_" + funcBits:string
    proc innerPopCount(x: uint(funcBits)): uint(funcBits);

    return innerPopCount(x: uint(bits)): uint(bits): x.type;
  }

  /*
    Find the parity of `x`.

    :returns: * 0 -- when an even number of bits are set in `x`
              * 1 -- when an odd number of bits are set in `x`
    :rtype: `x.type`
   */
  inline proc parity(x: integral) {
    param bits = numBits(x.type);
    if bits != 8 && bits != 16 && bits != 32 && bits != 64 {
      compilerError("parity is not supported for that bit width");
    }
    param funcBits = if bits == 8 || bits == 16 then 32 else bits;

    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern "chpl_bitops_parity_" + funcBits:string
    proc innerParity(x: uint(funcBits)): uint(funcBits);

    return innerParity(x: uint(bits)): uint(bits): x.type;
  }

  /*
    Rotate `x` left.

    :arg x: integral of size `bits`
    :arg n: rotation amount, must be less than `bits`

    :returns: `x` rotated left by `n` bits
    :rtype: `x.type`
   */
  inline proc rotl(x: integral, n: integral) {
    param bits = numBits(x.type);
    if bits != 8 && bits != 16 && bits != 32 && bits != 64 {
      compilerError("rotl is not supported for that bit width");
    }

    if boundsChecking {
      use HaltWrappers;
      if n < 0 || n >= bits {
        HaltWrappers.boundsCheckHalt("rotation amount must be between 0 and " +
                                     (bits-1):string +
                                     " for type of size " +
                                     bits:string + " - got " + n:string);
      }
    }


    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern "chpl_bitops_rotl_" + bits:string
    proc innerRotl(x: uint(bits), n: uint(bits)): uint(bits);

    return innerRotl(x: uint(bits), n: uint(bits)): x.type;
  }

  /*
    Rotate `x` right.

    :arg x: integral of size `bits`
    :arg n: rotation amount, must be less than `bits`

    :returns: `x` rotated right by `n` bits
    :rtype: `x.type`
   */
  inline proc rotr(x: integral, n: integral) {
    param bits = numBits(x.type);
    if bits != 8 && bits != 16 && bits != 32 && bits != 64 {
      compilerError("rotr is not supported for that bit width");
    }

    if boundsChecking {
      use HaltWrappers;
      if n < 0 || n >= bits {
        HaltWrappers.boundsCheckHalt("rotation amount must be between 0 and " +
                                     (bits-1):string +
                                     " for type of size " +
                                     bits:string + " - got " + n:string);
      }
    }


    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern "chpl_bitops_rotr_" + bits:string
    proc innerRotr(x: uint(bits), n: uint(bits)): uint(bits);

    return innerRotr(x: uint(bits), n: uint(bits)): x.type;
  }

  // Legacy operations
  // -----------------
  // kyleb: a few tests use these, but I'm not sure if there too much value

  // do a bitwise matrix-multiplication of x and y treating
  // them as 8x8 bit matrices and using | as the combinator
  // function.  There should be a similar version using xor
  // as the combinator, but I'm not sure where that would
  // go to be perfectly truthful
  @chpldoc.nodoc
  proc bitMatMultOr(x: uint(64), y: uint(64)): uint(64) {
    // return the transpose of x, treating it as an 8x8 bit-matrix.
    proc bitMatTrans(x: uint(64)) do
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
    inline proc byteExpand(u: uint(64)) do
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
