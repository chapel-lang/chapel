// Types.chpl
//
// Standard type routines.
//


//
// Type predicates
//
proc chpl__isType(type t) param return true;
proc chpl__isType(e) param return false;

proc _isPrimitiveType(type t) param return
  _isBooleanType(t) | _isIntegralType(t) | _isRealType(t) |
// BLC: Why aren't imaginaries here?  Someone should try this
  (t == string) | (_isVolatileType(t) && _isPrimitiveType(_volToNon(t)));

proc _isSimpleScalarType(type t) param return
  _isBooleanType(t) | _isIntegralType(t) | _isFloatType(t);

proc _isBooleanType(type t) param return
  (t == bool) | (t == bool(8)) | (t == bool(16)) | (t == bool(32)) | (t == bool(64));

proc _isIntegralType(type t) param return
  _isSignedType(t) || _isUnsignedType(t);

proc _isSignedType(type t) param return
  (t == int(8)) || (t == int(16)) || (t == int(32)) || (t == int(64));


proc _isUnsignedType(type t) param return
  (t == uint(8)) || (t == uint(16)) || (t == uint(32)) || (t == uint(64));

proc _isEnumeratedType(type t) param {
  proc isEnum(type t: enumerated) param return true;
  proc isEnum(type t) param return false;
  return isEnum(t);
}

proc _isComplexType(type t) param return
  (t == complex(64)) | (t == complex(128));

proc _isFloatType(type t) param return
  (t == real(32)) | (t == real(64)) |
  (t == imag(32)) | (t == imag(64));

proc _isRealType(type t) param return
  (t == real(32)) | (t == real(64));

proc _isImagType(type t) param return
  (t == imag(32)) | (t == imag(64));

proc _isVolatileType(type t) param
  return ((t == volatile bool) | (t == volatile bool(8)) | 
          (t == volatile bool(16)) | (t == volatile bool(32)) | 
          (t == volatile bool(64)) |  (t == volatile int) | 
          (t == volatile int(8)) | (t == volatile int(16)) | 
          (t == volatile int(32)) | (t == volatile int(64)) | 
          (t == volatile uint(8)) | (t == volatile uint(16)) | 
          (t == volatile uint(32)) | (t == volatile uint(64)) | 
          (t == volatile real(32)) | (t == volatile real(64)) | 
          (t == volatile imag(32)) | (t == volatile imag(64))
          );
//  (t == volatile string);


// Returns the signed equivalent of the input type.
proc chpl__signedType(type t) type 
{
  if ! _isIntegralType(t) then
    compilerError("range idxType is non-integral: ", typeToString(t));

  return int(numBits(t));
}


// Returns true if it is legal to coerce t1 to t2, false otherwise.
proc chpl__legalIntCoerce(type t1, type t2) param
{
  if (_isSignedType(t2)) {
    if (_isSignedType(t1)) {
      return (numBits(t1) <= numBits(t2));
    } else {
      return (numBits(t1) < numBits(t2));
    }
  } else {
    if (_isSignedType(t1)) {
      return false;
    } else {
      return (numBits(t1) <= numBits(t2));
    }
  }
}


// Returns the type with which both s and t are compatible
// That is, both s and t can be coerced to the returned type.
proc chpl__commonType(type s, type t) type
{
  if ! _isIntegralType(s) then
    compilerError("Type ", typeToString(s) , " is non-integral: ");
  if ! _isIntegralType(t) then
    compilerError("Type ", typeToString(t) , " is non-integral: ");

  if numBits(s) > numBits(t) then return s;
  if numBits(s) < numBits(t) then return t;

  if _isSignedType(s) && ! _isSignedType(t) ||
     _isSignedType(t) && ! _isSignedType(s) then
    compilerError("Types ", typeToString(s) , " and ", typeToString(t), " are incompatible.");

  return s;
}

//
// numBits(type) -- returns the number of bits in a type
//

proc numBits(type t) param where t == bool {
  compilerError("default-width 'bool' does not have a well-defined size");
}
proc numBits(type t) param where t == bool(8) return 8;
proc numBits(type t) param where t == bool(16) return 16;
proc numBits(type t) param where t == bool(32) return 32;
proc numBits(type t) param where t == bool(64) return 64;
proc numBits(type t) param where t == int(8) return 8;
proc numBits(type t) param where t == int(16) return 16;
proc numBits(type t) param where t == int(32) return 32;
proc numBits(type t) param where t == int(64) return 64;
proc numBits(type t) param where t == uint(8) return 8;
proc numBits(type t) param where t == uint(16) return 16;
proc numBits(type t) param where t == uint(32) return 32;
proc numBits(type t) param where t == uint(64) return 64;
proc numBits(type t) param where t == real(32) return 32;
proc numBits(type t) param where t == real(64) return 64;
proc numBits(type t) param where t == imag(32) return 32;
proc numBits(type t) param where t == imag(64) return 64;
proc numBits(type t) param where t == complex(64) return 64;
proc numBits(type t) param where t == complex(128) return 128;

//
// numBytes(type) -- returns the number of bytes in a type
//

param bitsPerByte = 8;

proc numBytes(type t) param return numBits(t)/bitsPerByte;

//
// min(type) -- returns the minimum value a type can store
//

proc min(type t) where _isIntegralType(t) || _isFloatType(t)
  return __primitive( "_min", t);

proc min(type t) where _isComplexType(t) {
  var x: t;
  x.re = min(x.re.type);
  x.im = min(x.im.type);
  return x;
}

//
// max(type) -- returns the maximum value a type can store
//

proc max(type t) where _isIntegralType(t) || _isFloatType(t)
  return __primitive( "_max", t);

proc max(type t) where _isComplexType(t) {
  var x: t;
  x.re = max(x.re.type);
  x.im = max(x.im.type);
  return x;
}

iter chpl_enumerate(type t: enumerated) {
  const enumTuple = _enum_enumerate(t);
  for i in 1..enumTuple.size do
    yield enumTuple(i);
}
