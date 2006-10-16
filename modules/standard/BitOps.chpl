//
// bit manipulation functions
//
// note we need an init function so u1, u3, and u7 can be computed
// once and not every time bpop is called
//
def bpop(i : int) {
  var u1 = 0, u3 = 0, u7 = 0, tmp = 0;

  u1 = 1;
  do {
    tmp = u1;
    u1 = 1 + (u1 << 3);
  } while u1 != tmp;
  u3 = 3;
  do {
    tmp = u3;
    u3 = 3 + (u3 << 3);
  } while u3 != tmp;
  u7 = 7;
  do {
    tmp = u7;
    u7 = 7 + (u7 << 9);
  } while u7 != tmp;
  tmp = i - ((i >> 1) & u3) - ((i >> 2) & u1);
  return (((tmp + (tmp >> 3)) & u7) + ((tmp >> 6) & u7)) % 511;
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
  var result: uint(64) = 0;
  for i in bitMatDimDom {
    for j in bitMatDimDom {
      var temp = 0;
      for k in bitMatDimDom {
        temp |= ((bitInd(i,k) & x) != 0) && ((bitInd(k,j) & y) != 0);
      }
      if (temp) {
        result |= bitInd(i,j);
      }
    }
  }
  return result;
}


def bitRotLeft(x, shift) {
  var backshift = bits(x) - shift;
  return (x << shift:int) | (x >> backshift:int);
}


def bitRotRight(x: uint(64), shift) {
  var backshift = bits(x) - shift;
  return (x >> shift:int) | (x << backshift:int);
}
