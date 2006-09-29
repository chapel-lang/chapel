//
// Primitive math funs
//

pragma "inline" def abs(x : int) return if x < 0 then -x else x;
pragma "inline" def acos(x : float) return __primitive("acos", x);
pragma "inline" def acosh(x : float) return __primitive("acosh", x);
pragma "inline" def asin(x : float) return __primitive("asin", x);
pragma "inline" def asinh(x : float) return __primitive("asinh", x);
pragma "inline" def atan(x : float) return __primitive("atan", x);
pragma "inline" def atan2(y : float, x : float) return __primitive("atan2", y, x);
pragma "inline" def atanh(x : float) return __primitive("atanh", x);
pragma "inline" def cbrt(x : float) return __primitive("cbrt", x);
pragma "inline" def ceil(x : float) return __primitive("ceil", x);
pragma "inline" def cos(x : float) return __primitive("cos", x);
pragma "inline" def cosh(x : float) return __primitive("cosh", x);
pragma "inline" def erf(x : float) return __primitive("erf", x);
pragma "inline" def erfc(x : float) return __primitive("erfc", x);
pragma "inline" def exp(x : float) return __primitive("exp", x);
pragma "inline" def exp2(x : float) return __primitive("exp2", x);
pragma "inline" def expm1(x : float) return __primitive("expm1", x);
pragma "inline" def abs(x : float) return __primitive("fabs", x);
pragma "inline" def floor(x : float) return __primitive("floor", x);
pragma "inline" def lgamma(x : float) return __primitive("lgamma", x);
pragma "inline" def log(x : float) return __primitive("log", x);
pragma "inline" def log2(x : float) return __primitive("log2", x);
pragma "inline" def log10(x : float) return __primitive("log10", x);
pragma "inline" def log1p(x : float) return __primitive("log1p", x);
pragma "inline" def nearbyint(x: float) return __primitive("nearbyint", x);
pragma "inline" def rint(x: float) return __primitive("rint", x);
pragma "inline" def round(x: float) return __primitive("round", x);
pragma "inline" def sin(x: float) return __primitive("sin", x);
pragma "inline" def sinh(x : float) return __primitive("sinh", x);
pragma "inline" def sqrt(x : float) return __primitive("sqrt", x);
pragma "inline" def tan(x: float) return __primitive("tan", x);
pragma "inline" def tanh(x : float) return __primitive("tanh", x);
pragma "inline" def tgamma(x : float) return __primitive("tgamma", x);
pragma "inline" def trunc(x : float) return __primitive("trunc", x);


def lg(in x) {
  var retval = -1;
  while (x >= 1) {
    x /= 2;
    retval += 1;
  }

  return retval;
}


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


// bit Matrix Operations -- these should eventually go into an
// optional library of bitwise operations

param bitsPerBitMatDim = 8;
const bitMatDimDom = [1..bitsPerBitMatDim];
const bitMatDom = [1..bitsPerBitMatDim, 1..bitsPerBitMatDim];
var bitInd: [bitMatDom] uint(64);

// BLC: would like to rewrite this as an initializer on bitInd
// using [i in bitMatDom] syntax
for i in bitMatDimDom {
  for j in bitMatDimDom {
    bitInd(i,j) = 0x1u << (((bitsPerBitMatDim-i)*bitsPerBitMatDim)
                           + (bitsPerBitMatDim-j)):uint;
    //    bitMatWrite(bitInd(i,j));
  }
}


def bitMatWrite(x: uint(64)) {
  var mask: uint(64) = 0x8000000000000000u;
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


// reverse numBits low-order bits of x
def bitReverse(x: uint(64), numBits = 64) {
  const mask: uint(64) = 0x0102040810204080u;
  var ndx = bitMatMultOr(mask, bitMatMultOr(x, mask));
  ndx = bitRotLeft(ndx, numBits);
  return ndx;
}



def bitRotLeft(x: uint(64), shift) {
  return (x << shift:uint) | (x >> (64-shift):uint);
}


def bitRotRight(x: uint(64), shift) {
  return (x >> shift:uint) | (x << (64-shift):uint);
}
