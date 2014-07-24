// Types.chpl
//
// Standard type routines.
//

//
// type predicates
//
proc chpl__isType(type t) param return true;
proc chpl__isType(e) param return false;

pragma "no instantiation limit"
proc _isPrimitiveType(type t) param return
  _isBooleanType(t)  ||
  _isIntegralType(t) ||
  _isRealType(t)     ||
//To allow imag, need to define casts from primitive types into imag.
//_isImagType(t)     ||
  (t == c_string);

pragma "no instantiation limit"
proc _isSimpleScalarType(type t) param return
  _isBooleanType(t) | _isIntegralType(t) | _isFloatType(t);

pragma "no instantiation limit"
proc _isBooleanType(type t) param return
  (t == bool) | (t == bool(8)) | (t == bool(16)) | (t == bool(32)) | (t == bool(64));

pragma "no instantiation limit"
proc _isIntegralType(type t) param return
  _isSignedType(t) || _isUnsignedType(t);

pragma "no instantiation limit"
proc _isSignedType(type t) param return
  (t == int(8)) || (t == int(16)) || (t == int(32)) || (t == int(64));


pragma "no instantiation limit"
proc _isUnsignedType(type t) param return
  (t == uint(8)) || (t == uint(16)) || (t == uint(32)) || (t == uint(64));

pragma "no instantiation limit"
proc _isEnumeratedType(type t) param {
  proc isEnum(type t: enumerated) param return true;
  proc isEnum(type t) param return false;
  return isEnum(t);
}

pragma "no instantiation limit"
proc _isComplexType(type t) param return
  (t == complex(64)) | (t == complex(128));

pragma "no instantiation limit"
proc _isFloatType(type t) param return
  (t == real(32)) | (t == real(64)) |
  (t == imag(32)) | (t == imag(64));

pragma "no instantiation limit"
proc _isRealType(type t) param return
  (t == real(32)) | (t == real(64));

pragma "no instantiation limit"
proc _isImagType(type t) param return
  (t == imag(32)) | (t == imag(64));

// Returns the unsigned equivalent of the input type.
proc chpl__unsignedType(type t) type 
{
  if ! _isIntegralType(t) then
    compilerError("range idxType is non-integral: ", typeToString(t));

  return uint(numBits(t));
}


// Returns the signed equivalent of the input type.
proc chpl__signedType(type t) type 
{
  if ! _isIntegralType(t) then
    compilerError("range idxType is non-integral: ", typeToString(t));

  return int(numBits(t));
}

proc chpl__maxIntTypeSameSign(type t) type {
  if ! _isIntegralType(t) then
    compilerError("type t is non-integral: ", typeToString(t));

  if (_isSignedType(t)) then
    return int(64);
  else
    return uint(64);
}


//
// These procedures indicate whether or not a type t is a specific type
//
proc isRangeType(type t) param where   t: range  return true;
proc isRangeType(type t) param where !(t: range) return false;

proc isDmapType(type t) param where   t: _distribution  return true;
proc isDmapType(type t) param where !(t: _distribution) return false;

proc isDomainType(type t) param where   t: _domain  return true;
proc isDomainType(type t) param where !(t: _domain) return false;
  
proc isArrayType(type t) param where   t: _array  return true;
proc isArrayType(type t) param where !(t: _array) return false;


// Is 'sub' a subtype (or equal to) 'super'?
proc isSubtype(type sub, type super) param where   sub: super  return true;
proc isSubtype(type sub, type super) param where !(sub: super) return false;

// Is 'sub' a proper subtype of 'super'?
proc isProperSubtype(type sub, type super) param
  where isSubtype(sub, super) && sub != super
  return true;
proc isProperSubtype(type sub, type super) param
  return false;

// What follows are the type _defaultOf methods, used to initialize types
// Booleans
inline proc _defaultOf(type t) param where (_isBooleanType(t)) return false:t;

// String
// defaultStringValue lives in runtime/include/chpl-string.h
// strings will be records, so param is not possible for them
inline proc _defaultOf(type t) where t: string return defaultStringValue;

// ints, reals, imags, complexes
inline proc _defaultOf(type t) param where (_isIntegralType(t)) return 0:t;
// TODO: In order to make _defaultOf param for reals and imags we had to split
// the cases into their default size and a non-param case.  It is hoped that
// in the future, floating point numbers may be castable whilst param.  In that
// world, we can again shrink these calls into the size-ignorant case.
inline proc _defaultOf(type t) param where t == real(64) return 0.0:t;
inline proc _defaultOf(type t) where (_isRealType(t) && t != real(64)) return 0.0:t;
inline proc _defaultOf(type t) param where t == imag(64) return 0.0i:t;
inline proc _defaultOf(type t) where (_isImagType(t) && t != imag(64)) return 0.0i:t;
// Also, complexes cannot yet be parametized
inline proc _defaultOf(type t): t where (_isComplexType(t)) {
  var ret:t = noinit;
  param floatwidth = numBits(t)/2;
  ret.re = 0.0:real(floatwidth);
  ret.im = 0.0:real(floatwidth);
  return ret;
}

// Enums
inline proc _defaultOf(type t) param where (_isEnumeratedType(t)) {
  return _enum_first(t);
}

// Classes
inline proc _defaultOf(type t) where (isClassType(t)) return nil:t;

// Various types whose default value is known
inline proc _defaultOf(type t) param where t: void return _void;
inline proc _defaultOf(type t) param where t: c_string return "":c_string;
inline proc _defaultOf(type t) where t: opaque return _nullOpaque;
inline proc _defaultOf(type t) where t: chpl_taskID_t return chpl_nullTaskID;
inline proc _defaultOf(type t) where t: _sync_aux_t return _nullSyncVarAuxFields;
inline proc _defaultOf(type t) where t: _single_aux_t return _nullSingleVarAuxFields;
inline proc _defaultOf(type t) where t: _task_list return _nullTaskList;

/* Need new <alias>() for this to function
proc _defaultOf(type t) where t:_distribution {
  var ret: t = noinit;
  type valType = __primitive("query type field", t, "_valueType");
  var typeInstance = new <valType>();
  ret = chpl__buildDistValue(typeInstance);
  return ret;
}
*/

// When I finish removing PRIM_INIT before initialization to a known value, then
// this method should work.  Until then, my stopgap will be an external function
// in the runtime.
//inline proc _defaultOf(type t) where t: memory_order return memory_order_seq_cst;
extern proc _defaultOfMemoryOrder(): memory_order;

pragma "no instantiation limit"
pragma "compiler generated"
inline proc _defaultOf(type t) {
  select t {
    when memory_order {
      return _defaultOfMemoryOrder();
    }
    otherwise {
      return nil:t;
    }
  }
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

proc numBytes(type t) param return numBits(t)/8;

//
// min(type) -- returns the minimum value a type can store
//

proc min(type t) where t == bool
  return false;

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

proc max(type t) where t == bool
  return true;

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

proc enum_minbits(type t: enumerated) param {
  return __primitive( "enum min bits", t);
}
proc enum_issigned(type t: enumerated) param {
  return __primitive( "enum is signed", t);
}
proc enum_mintype(type t: enumerated) type {
  param minbits = enum_minbits(t);
  param signed = enum_issigned(t);
  if signed {
    return int(minbits);
  } else {
    return uint(minbits);
  }
}

proc numBits(type t: enumerated) param {
  return numBits(enum_mintype(t));
}


//
// identity functions (for reductions)
//
inline proc _prod_id(type t) return 1:t;
inline proc _land_id(type t) return true;
inline proc _lor_id(type t) return false;
inline proc _lxor_id(type t) return false;
inline proc _band_id(type t) {
  // MPF - this doesn't really make sense for floating-point types,
  // but old code had it casting MAX_UINT to the same type...
  // I just moved it from the code generator since we don't need
  // a primitive to generate constants.
  if t == bool then return true;
  if t == int(8) then return -1:t;
  if t == int(16) then return -1:t;
  if t == int(32) then return -1:t;
  if t == int(64) then return -1:t;
  if t == real(32) then return max(uint(32)):t;
  if t == real(64) then return max(uint(64)):t;
  if t == imag(32) then return max(uint(32)):t;
  if t == imag(64) then return max(uint(64)):t;
  if t == complex(64) then return (max(uint(32)):real(32), max(uint(32)):real(32)):t;
  if t == complex(128) then return (max(uint(64)):real(64), max(uint(64)):real(64)):t;
  return max(t);
}
inline proc _bor_id(type t) return 0:t;
inline proc _bxor_id(type t) return 0:t;

