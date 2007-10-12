//
// bit manipulation functions
//

// reimplementation note: one bitPop routine when numBits is a parameter
def bitPop(i: integral) where (i.type == int(64)) | (i.type == uint(64)) {
  const u1 = 10540996613548315209:uint(64);
  const u3 = 13176245766935394011:uint(64);
  const u7 = 9349719599003471367:uint(64);
  var ii = i: uint(64);
  var tmp = ii - ((ii >> 1) & u3) - ((ii >> 2) & u1);
  return ((((tmp + (tmp >> 3)) & u7) + ((tmp >> 6) & u7)) % 511):int;
}

def bitPop(i: integral) where (i.type == int(32)) | (i.type == uint(32)) {
  const u1 = 10540996613548315209:uint(32);
  const u3 = 13176245766935394011:uint(32);
  const u7 = 9349719599003471367:uint(32);
  var ii = i: uint(32);
  var tmp = ii - ((ii >> 1) & u3) - ((ii >> 2) & u1);
  return ((((tmp + (tmp >> 3)) & u7) + ((tmp >> 6) & u7)) % 511):int;
}

def bitPop(i: integral) where (i.type == int(16)) | (i.type == uint(16)) {
  const u1 = 10540996613548315209:uint(16);
  const u3 = 13176245766935394011:uint(16);
  const u7 = 9349719599003471367:uint(16);
  var ii = i: uint(16);
  var tmp = ii - ((ii >> 1) & u3) - ((ii >> 2) & u1);
  return ((((tmp + (tmp >> 3)) & u7) + ((tmp >> 6) & u7)) % 511):int;
}

def bitPop(i: integral) where (i.type == int(8)) | (i.type == uint(8)) {
  const u1 = 10540996613548315209:uint(8);
  const u3 = 13176245766935394011:uint(8);
  const u7 = 9349719599003471367:uint(8);
  var ii = i: uint(8);
  var tmp = ii - ((ii >> 1) & u3) - ((ii >> 2) & u1);
  return ((((tmp + (tmp >> 3)) & u7) + ((tmp >> 6) & u7)) % 511):int;
}

param bitsPerBitMatDim = 8;
const bitMatDimDom = [1..bitsPerBitMatDim];
const bitMatDom = [1..bitsPerBitMatDim, 1..bitsPerBitMatDim];
var bitInd: [bitMatDom] uint(64);

// BLC: would like to rewrite this as an initializer on bitInd
// using [i in bitMatDom] syntax
for i in bitMatDimDom {
  for j in bitMatDimDom {
    bitInd(i,j) = 0x1:uint(64) << (((bitsPerBitMatDim-i)*bitsPerBitMatDim)
                                   + (bitsPerBitMatDim-j)):int;
    //    bitMatWrite(bitInd(i,j));
  }
}




def bitMatWrite(x: uint(64)) {
  var mask: uint(64) = 0x8000000000000000;
  for i in bitMatDimDom {
    for j in bitMatDimDom {
      if ((x & mask) != 0) {
        write("1");
      } else {
        write("0");
      }
      // BLC: should be: mask >>= 1;
      mask = mask >> 1;
    }
    writeln();
  }
  writeln();
}


// do a bitwise matrix-multiplication of x and y treating
// them as 8x8 bit matrices and using | as the combinator
// function.  There should be a similar version using xor
// as the combinator, but I'm not sure where that would
// go to be perfectly truthful
def bitMatMultOr(x: uint(64), y: uint(64)): uint(64) {
  def transpose(x: uint(64)) {
   // Return the transpose of x, treating it as an 8x8 bit-matrix.
   return  x & 0x8040201008040201        |
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
          (x >> 49) & 0x0000000000000080;
  }
  var yTranspose = transpose(y);
  var result:uint(64) = 0;
  for i in 0..7:uint(64) {
    var left = (x & (0xFF:uint(64) << (i*8))) >> (i*8);
    for param j in 0..7:uint(64) {
      if (0 != left & ((yTranspose & (0xFF:uint(64) << (j*8))) >> (j*8))) then
        result |= 1:uint(64) << (i*8 + j):uint(64);
    }
  }
  return result;
}


def bitRotLeft(x, shift) {
  var backshift = numBits(x) - shift;
  return (x << shift:int) | (x >> backshift:int);
}


def bitRotRight(x: uint(64), shift) {
  var backshift = numBits(x) - shift;
  return (x >> shift:int) | (x << backshift:int);
}
