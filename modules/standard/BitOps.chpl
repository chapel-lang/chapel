//
// bit manipulation functions
//

// reimplementation note: one bitPop routine when numBits is a parameter
proc bitPop(i: integral) where (i.type == int(64)) || (i.type == uint(64)) {
  const u1 = 10540996613548315209:uint(64);
  const u3 = 13176245766935394011:uint(64);
  const u7 = 9349719599003471367:uint(64);
  var ii = i: uint(64);
  var tmp = ii - ((ii >> 1) & u3) - ((ii >> 2) & u1);
  return ((((tmp + (tmp >> 3)) & u7) + ((tmp >> 6) & u7)) % 511):int;
}

proc bitPop(i: integral) where (i.type == int(32)) || (i.type == uint(32)) {
  const u1 = 10540996613548315209:uint(32);
  const u3 = 13176245766935394011:uint(32);
  const u7 = 9349719599003471367:uint(32);
  var ii = i: uint(32);
  var tmp = ii - ((ii >> 1) & u3) - ((ii >> 2) & u1);
  return ((((tmp + (tmp >> 3)) & u7) + ((tmp >> 6) & u7)) % 511):int;
}

proc bitPop(i: integral) where (i.type == int(16)) || (i.type == uint(16)) {
  const u1 = 10540996613548315209:uint(16);
  const u3 = 13176245766935394011:uint(16);
  const u7 = 9349719599003471367:uint(16);
  var ii = i: uint(16);
  var tmp = ii - ((ii >> 1) & u3) - ((ii >> 2) & u1);
  return ((((tmp + (tmp >> 3)) & u7) + ((tmp >> 6) & u7)) % 511):int;
}

proc bitPop(i: integral) where (i.type == int(8)) || (i.type == uint(8)) {
  const u1 = 10540996613548315209:uint(8);
  const u3 = 13176245766935394011:uint(8);
  const u7 = 9349719599003471367:uint(8);
  var ii = i: uint(8);
  var tmp = ii - ((ii >> 1) & u3) - ((ii >> 2) & u1);
  return ((((tmp + (tmp >> 3)) & u7) + ((tmp >> 6) & u7)) % 511):int;
}


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
