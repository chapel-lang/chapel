/**
 *  Bit Manipulation Functions
 */
module BitOps {

  inline proc clz(x: uint(?bits)) {
    // the select will be folded out at compile time.
    select bits {
      when 64 do
        return BitOps_internal.chpl_bitops_clz_64(x);
      when 32 do
        return BitOps_internal.chpl_bitops_clz_32(x);
      when 16 do
        // gets promoted to 32bit, subtract the extra leading 0s
        return (BitOps_internal.chpl_bitops_clz_32(x)-16):uint(16);
      when 8 do
        // gets promoted to 32bit, subtract the extra leading 0s
        return (BitOps_internal.chpl_bitops_clz_32(x)-24):uint(8);
      otherwise
        // NOTE: this actually cant happen with how the integer types are setup
        //       right now - leaving it here for the future when we support
        //       >64bit types
        compilerError("clz is not supported for that bit width.");
    }
  }

  inline proc clz(x: int(?bits)) {
    return clz(x:uint(bits)):int(bits);
  }

  inline proc ctz(x: uint(?bits)) {
    // the select will be folded out at compile time.
    select bits {
      when 64 do
        return BitOps_internal.chpl_bitops_ctz_64(x);
      when 32 do
        return BitOps_internal.chpl_bitops_ctz_32(x);
      when 16 do
        return BitOps_internal.chpl_bitops_ctz_32(x):uint(16);
      when 8 do
        return BitOps_internal.chpl_bitops_ctz_32(x):uint(8);
      otherwise
        // NOTE: this actually cant happen with how the integer types are setup
        //       right now - leaving it here for the future when we support
        //       >64bit types
        compilerError("ctz is not supported for that bit width.");
    }
  }

  inline proc ctz(x: int(?bits)) {
    return ctz(x:uint(bits)):int(bits);
  }

  inline proc popcount(x: uint(?bits)) {
    // the select will be folded out at compile time.
    select bits {
      when 64 do
        return BitOps_internal.chpl_bitops_popcount_64(x);
      when 32 do
        return BitOps_internal.chpl_bitops_popcount_32(x);
      when 16 do
        return BitOps_internal.chpl_bitops_popcount_32(x):uint(16);
      when 8 do
        return BitOps_internal.chpl_bitops_popcount_32(x):uint(8);
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
    return popcount(x:uint(bits)):int(bits);
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
    yTranspose = bitRotLeft(yTranspose, 8);
    result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x4020100804020180;
    yTranspose = bitRotLeft(yTranspose, 8);
    result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x2010080402018040;
    yTranspose = bitRotLeft(yTranspose, 8);
    result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x1008040201804020;
    yTranspose = bitRotLeft(yTranspose, 8);
    result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x0804020180402010;
    yTranspose = bitRotLeft(yTranspose, 8);
    result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x0402018040201008;
    yTranspose = bitRotLeft(yTranspose, 8);
    result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x0201804020100804;
    yTranspose = bitRotLeft(yTranspose, 8);
    result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x0180402010080402;
    return result;
  }


  inline proc bitRotLeft(x, shift) {
    var backshift = numBits(x.type) - shift;
    return (x << shift:int) | (x >> backshift:int);
  }


  inline proc bitRotRight(x: uint(64), shift) {
    var backshift = numBits(x.type) - shift;
    return (x >> shift:int) | (x << backshift:int);
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
