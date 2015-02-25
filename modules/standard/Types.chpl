/*
 * Copyright 2004-2015 Cray Inc.
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

// Types.chpl
//
// Standard type routines.
//

//
// type predicates
//
pragma "no instantiation limit"
proc isType(type t) param return true;
pragma "no instantiation limit"
proc isType(e) param return false;

pragma "no instantiation limit"
proc isParam(type t)  param return false;
pragma "no instantiation limit"
proc isParam(param p) param return true;
pragma "no instantiation limit"
proc isParam(e)       param return false;

// TODO eliminate this; beware of isPrimitive()
pragma "no instantiation limit"
proc _isPrimitiveType(type t) param return
  isBoolType(t)  ||
  isIntegralType(t) ||
  isRealType(t)     ||
//To allow imag, need to define casts from primitive types into imag.
//isImagType(t)     ||
  (t == c_string);

// the set of primitive types, as defined by the spec
pragma "no instantiation limit"
proc isPrimitiveType(type t) param return
  isVoidType(t) || isBoolType(t) || isIntType(t) || isUintType(t) ||
  isRealType(t) || isImagType(t) || isComplexType(t) || isStringType(t);

pragma "no instantiation limit"
proc isVoidType(type t) param return t == void;

pragma "no instantiation limit"
proc isBoolType(type t) param return
  (t == bool) | (t == bool(8)) | (t == bool(16)) | (t == bool(32)) | (t == bool(64));

pragma "no instantiation limit"
proc isNumericType(type t) param return
  isIntegralType(t) || isFloatType(t) || isComplexType(t);

pragma "no instantiation limit"
proc isIntegralType(type t) param return
  isIntType(t) || isUintType(t);

pragma "no instantiation limit"
proc isIntType(type t) param return
  (t == int(8)) || (t == int(16)) || (t == int(32)) || (t == int(64));

pragma "no instantiation limit"
proc isUintType(type t) param return
  (t == uint(8)) || (t == uint(16)) || (t == uint(32)) || (t == uint(64));

pragma "no instantiation limit"
proc isEnumType(type t) param {
  proc isEnumHelp(type t: enumerated) param return true;
  proc isEnumHelp(type t) param return false;
  return isEnumHelp(t);
}

pragma "no instantiation limit"
proc isComplexType(type t) param return
  (t == complex(64)) | (t == complex(128));

pragma "no instantiation limit"
proc isFloatType(type t) param return
  isRealType(t) || isImagType(t);

pragma "no instantiation limit"
proc isRealType(type t) param return
  (t == real(32)) | (t == real(64));

pragma "no instantiation limit"
proc isImagType(type t) param return
  (t == imag(32)) | (t == imag(64));

pragma "no instantiation limit"
proc isStringType(type t) param return t == string;

// Returns the unsigned equivalent of the input type.
proc chpl__unsignedType(type t) type 
{
  if ! isIntegralType(t) then
    compilerError("range idxType is non-integral: ", typeToString(t));

  return uint(numBits(t));
}


// Returns the signed equivalent of the input type.
proc chpl__signedType(type t) type 
{
  if ! isIntegralType(t) then
    compilerError("range idxType is non-integral: ", typeToString(t));

  return int(numBits(t));
}

proc chpl__maxIntTypeSameSign(type t) type {
  if ! isIntegralType(t) then
    compilerError("type t is non-integral: ", typeToString(t));

  if (isIntType(t)) then
    return int(64);
  else
    return uint(64);
}


//
// isXxxValue() - trivial implementations
// no isVoid() or isVoidValue() - it might not work
// Let these be for internal consumption for now,
// due to lack of consensus for the name.
//

proc isBoolValue(e)      param  return isBoolType(e.type);
proc isIntValue(e)       param  return isIntType(e.type);
proc isUintValue(e)      param  return isUintType(e.type);
proc isRealValue(e)      param  return isRealType(e.type);
proc isImagValue(e)      param  return isImagType(e.type);
proc isComplexValue(e)   param  return isComplexType(e.type);
proc isStringValue(e)    param  return isStringType(e.type);
proc isIntegralValue(e)  param  return isIntegralType(e.type);
proc isFloatValue(e)     param  return isFloatType(e.type);
proc isNumericValue(e)   param  return isNumericType(e.type);
proc isPrimitiveValue(e) param  return isPrimitiveType(e.type);
proc isEnumValue(e)      param  return isEnumType(e.type);
//Defined elsewhere:
// isTupleValue
// isHomogeneousTupleValue
proc isClassValue(e)     param  return isClassType(e.type);
proc isRecordValue(e)    param  return isRecordType(e.type);
proc isUnionValue(e)     param  return isUnionType(e.type);
//Defined elsewhere:
// isRangeValue
// isDmapValue
// isDomainValue
// isArrayValue
// isSyncValue
// isSingleValue
proc isAtomicValue(e)    param  return isAtomicType(e.type);


//
// ixXxx() - the argument can be either a type or a value
//

// Set 1 - types.
proc isBool(type t)      param  return isBoolType(t);
proc isInt(type t)       param  return isIntType(t);
proc isUint(type t)      param  return isUintType(t);
proc isReal(type t)      param  return isRealType(t);
proc isImag(type t)      param  return isImagType(t);
proc isComplex(type t)   param  return isComplexType(t);
proc isString(type t)    param  return isStringType(t);
proc isIntegral(type t)  param  return isIntegralType(t);
proc isFloat(type t)     param  return isFloatType(t);
proc isNumeric(type t)   param  return isNumericType(t);
proc isPrimitive(type t) param  return isPrimitiveType(t);
proc isEnum(type t)      param  return isEnumType(t);
proc isTuple(type t)     param  return isTupleType(t);
proc isHomogeneousTuple(type t)  param  return isHomogeneousTupleType(t);
proc isClass(type t)     param  return isClassType(t);
proc isRecord(type t)    param  return isRecordType(t);
proc isUnion(type t)     param  return isUnionType(t);
proc isRange(type t)     param  return isRangeType(t);
proc isDmap(type t)      param  return isDmapType(t);
proc isDomain(type t)    param  return isDomainType(t);
proc isArray(type t)     param  return isArrayType(t);
proc isSync(type t)      param  return isSyncType(t);
proc isSingle(type t)    param  return isSingleType(t);
proc isAtomic(type t)    param  return isAtomicType(t);

// Set 2 - values.
proc isBool(e)      param  return isBoolValue(e);
proc isInt(e)       param  return isIntValue(e);
proc isUint(e)      param  return isUintValue(e);
proc isReal(e)      param  return isRealValue(e);
proc isImag(e)      param  return isImagValue(e);
proc isComplex(e)   param  return isComplexValue(e);
proc isString(e)    param  return isStringValue(e);
proc isIntegral(e)  param  return isIntegralValue(e);
proc isFloat(e)     param  return isFloatValue(e);
proc isNumeric(e)   param  return isNumericValue(e);
proc isPrimitive(e) param  return isPrimitiveValue(e);
proc isEnum(e)      param  return isEnumValue(e);
proc isTuple(e)     param  return isTupleValue(e);
proc isHomogeneousTuple(e: _tuple)  param  return isHomogeneousTupleValue(e);
proc isClass(e)     param  return isClassValue(e);
proc isRecord(e)    param  return isRecordValue(e);
proc isUnion(e)     param  return isUnionValue(e);
proc isRange(e)     param  return isRangeValue(e);
proc isDmap(e)      param  return isDmapValue(e);
proc isDomain(e)    param  return isDomainValue(e);
proc isArray(e)     param  return isArrayValue(e);
proc isSync(e: sync)     param  return true; // workaround: not isSyncValue
proc isSync(e)           param  return false;
proc isSingle(e: single) param  return true; // workaround: not isSingleValue
proc isSingle(e)         param  return false;
proc isAtomic(e)    param  return isAtomicValue(e);


// for internal use until we have a better name
proc chpl_isSyncSingleAtomic(e)         param  return false;
proc chpl_isSyncSingleAtomic(e: sync)   param  return true;
proc chpl_isSyncSingleAtomic(e: single) param  return true;
proc chpl_isSyncSingleAtomic(e)  param where isAtomicType(e.type)  return true;


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
inline proc _defaultOf(type t) param where (isBoolType(t)) return false:t;

// ints, reals, imags, complexes
inline proc _defaultOf(type t) param where (isIntegralType(t)) return 0:t;
// TODO: In order to make _defaultOf param for reals and imags we had to split
// the cases into their default size and a non-param case.  It is hoped that
// in the future, floating point numbers may be castable whilst param.  In that
// world, we can again shrink these calls into the size-ignorant case.
inline proc _defaultOf(type t) param where t == real return 0.0;
inline proc _defaultOf(type t) where (isRealType(t) && t != real) return 0.0:t;
inline proc _defaultOf(type t) param where t == imag return 0.0i;
inline proc _defaultOf(type t) where (isImagType(t) && t != imag) return 0.0i:t;
// Also, complexes cannot yet be parametized
inline proc _defaultOf(type t): t where (isComplexType(t)) {
  var ret:t = noinit;
  param floatwidth = numBits(t)/2;
  ret.re = 0.0:real(floatwidth);
  ret.im = 0.0:real(floatwidth);
  return ret;
}

// Enums
inline proc _defaultOf(type t) param where (isEnumType(t)) {
  return chpl_enum_first(t);
}

// Classes
inline proc _defaultOf(type t) where (isClassType(t)) return nil:t;

// Various types whose default value is known
inline proc _defaultOf(type t) param where t: void return _void;
inline proc _defaultOf(type t) where t: opaque return _nullOpaque;
inline proc _defaultOf(type t) where t: chpl_taskID_t return chpl_nullTaskID;
inline proc _defaultOf(type t) where t: _sync_aux_t return _nullSyncVarAuxFields;
inline proc _defaultOf(type t) where t: _single_aux_t return _nullSingleVarAuxFields;
inline proc _defaultOf(type t) where t: _task_list return _nullTaskList;


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
  if (isIntType(t2)) {
    if (isIntType(t1)) {
      return (numBits(t1) <= numBits(t2));
    } else {
      return (numBits(t1) < numBits(t2));
    }
  } else {
    if (isIntType(t1)) {
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
  if ! isIntegralType(s) then
    compilerError("Type ", typeToString(s) , " is non-integral: ");
  if ! isIntegralType(t) then
    compilerError("Type ", typeToString(t) , " is non-integral: ");

  if numBits(s) > numBits(t) then return s;
  if numBits(s) < numBits(t) then return t;

  if isIntType(s) && ! isIntType(t) ||
     isIntType(t) && ! isIntType(s) then
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

proc min(type t) param  where isBool(t)      return false: t;

proc min(type t) param  where t == int(8)    return 0x80: t;
proc min(type t) param  where t == int(16)   return 0x8000: t;
proc min(type t) param  where t == int(32)   return 0x80000000: t;
proc min(type t) param  where t == int(64)   return 0x8000000000000000: t;

proc min(type t) param  where isUint(t)      return 0: t;

proc min(type t) where isFloatType(t)        return __primitive( "_min", t);

proc min(type t) where isComplexType(t) {
  var x: t;
  x.re = min(x.re.type);
  x.im = min(x.im.type);
  return x;
}

//
// max(type) -- returns the maximum value a type can store
//

proc max(type t) param  where isBool(t)      return true: t;

proc max(type t) param  where t == int(8)    return 0x7f: t;
proc max(type t) param  where t == int(16)   return 0x7fff: t;
proc max(type t) param  where t == int(32)   return 0x7fffffff: t;
proc max(type t) param  where t == int(64)   return 0x7fffffffffffffff: t;

proc max(type t) param  where t == uint(8)   return 0xff: t;
proc max(type t) param  where t == uint(16)  return 0xffff: t;
proc max(type t) param  where t == uint(32)  return 0xffffffff: t;
proc max(type t) param  where t == uint(64)  return 0xffffffffffffffff: t;

proc max(type t) where isFloatType(t)        return __primitive( "_max", t);

proc max(type t) where isComplexType(t) {
  var x: t;
  x.re = max(x.re.type);
  x.im = max(x.im.type);
  return x;
}

iter chpl_enumerate(type t: enumerated) {
  const enumTuple = chpl_enum_enumerate(t);
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
// safe up/down casts between all integral types
// performs the minimum number of runtime checks - uint(8)->uint(64) won't
// perform any checks for example
//
inline proc safe_cast(type T, val) : T where isUintType(T) && isIntegralType(val.type) {
  if castChecking {
    if min(val.type) < 0 {
      if val < 0 then // runtime check
        halt("casting "+typeToString(val.type)+" < 0 to "+typeToString(T));
    }
    if max(val.type):uint > max(T):uint {
      if (val:uint > max(T):uint) then // runtime check
        halt("casting "+typeToString(val.type)+" > max("+typeToString(T)+") to "+typeToString(T));
    }
  }
  return val:T;
}

inline proc safe_cast(type T, val) : T where isIntType(T) && isIntegralType(val.type) {
  if castChecking {
    if max(val.type):uint > max(T):uint {
      // this isUintType check lets us avoid a runtime check for val < 0
      if isUintType(val.type) {
        if val:uint > max(T):uint then // runtime check
          halt("casting "+typeToString(val.type)+" > max("+typeToString(T)+") to "+typeToString(T));
      } else {
        // max(T) <= max(int), so cast to int is safe
        if val:int > max(T):int then // runtime check
          halt("casting "+typeToString(val.type)+" > max("+typeToString(T)+") to "+typeToString(T));
      }
    }
    if isIntType(val.type) {
      if min(val.type):int < min(T):int {
        if val:int < min(T):int then // runtime check
          halt("casting "+typeToString(val.type)+" < min("+typeToString(T)+") to "+typeToString(T));
      }
    }
  }
  return val:T;
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

