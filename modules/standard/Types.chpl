//
// numBits(type) -- returns the number of bits in a type
//

def numBits(type t) param where t == bool return 1;
def numBits(type t) param where t == int(8) return 8;
def numBits(type t) param where t == int(16) return 16;
def numBits(type t) param where t == int(32) return 32;
def numBits(type t) param where t == int(64) return 64;
def numBits(type t) param where t == uint(8) return 8;
def numBits(type t) param where t == uint(16) return 16;
def numBits(type t) param where t == uint(32) return 32;
def numBits(type t) param where t == uint(64) return 64;
def numBits(type t) param where t == real(32) return 32;
def numBits(type t) param where t == real(64) return 64;
def numBits(type t) param where t == imag(32) return 32;
def numBits(type t) param where t == imag(64) return 64;
def numBits(type t) param where t == complex(64) return 64;
def numBits(type t) param where t == complex(128) return 128;

//
// numBytes(type) -- returns the number of bytes in a type
//

param bitsPerByte = 8;

def numBytes(type t) param return numBits(t)/bitsPerByte;

//
// min(type) -- returns the minimum value a type can store
//

def min(type t) where _isIntegralType(t) | _isFloatType(t)
  return __primitive( "_min", t);

def min(type t) where _isComplexType(t) {
  var x: t;
  x.re = min(x.re.type);
  x.im = min(x.im.type);
  return x;
}

//
// min(type) -- returns the maximum value a type can store
//

def max(type t) where _isIntegralType(t) | _isFloatType(t)
  return __primitive( "_max", t);

def max(type t) where _isComplexType(t) {
  var x: t;
  x.re = max(x.re.type);
  x.im = max(x.im.type);
  return x;
}
