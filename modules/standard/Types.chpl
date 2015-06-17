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

/*
Functions related to predefined types.

This is an automatic module, so these functions are available
to all Chapel programs.
*/
module Types {

pragma "no doc" // joint documentation with the next one
pragma "no instantiation limit"
proc isType(type t) param return true;
/* Returns `true` if the argument is a type. */
pragma "no instantiation limit"
proc isType(e) param return false;

pragma "no doc" // joint documentation with the next one
pragma "no instantiation limit"
proc isParam(type t)  param return false;
pragma "no doc" // joint documentation with the next one
pragma "no instantiation limit"
proc isParam(param p) param return true;
/* Returns `true` if the argument is a param. */
pragma "no instantiation limit"
proc isParam(e)       param return false;

// TODO eliminate this; beware of isPrimitive()
pragma "no doc"
pragma "no instantiation limit"
proc _isPrimitiveType(type t) param return
  isBoolType(t)  ||
  isIntegralType(t) ||
  isRealType(t)     ||
//To allow imag, need to define casts from primitive types into imag.
//isImagType(t)     ||
  (t == c_string);

/*
Returns `true` if the type `t` is a primitive type,
as defined by the language specification.
*/
pragma "no instantiation limit"
proc isPrimitiveType(type t) param return
  isVoidType(t) || isBoolType(t) || isIntType(t) || isUintType(t) ||
  isRealType(t) || isImagType(t) || isComplexType(t) || isStringType(t);

/*
Returns `true` if the type `t` is one the following types, of any width:
`int`, `uint`, `real`, `imag`, `complex`.
*/
pragma "no instantiation limit"
proc isNumericType(type t) param return
  isIntegralType(t) || isFloatType(t) || isComplexType(t);

/*
Returns `true` if the type `t` is one the following types, of any width:
`int`, `uint`.
*/
pragma "no instantiation limit"
proc isIntegralType(type t) param return
  isIntType(t) || isUintType(t);

/*
Returns `true` if the type `t` is one the following types, of any width:
`real`, `imag`.
*/
pragma "no instantiation limit"
proc isFloatType(type t) param return
  isRealType(t) || isImagType(t);

// We document the following functions as one group, for user convenience.
pragma "no instantiation limit"
proc isVoidType(type t) param return t == void;

pragma "no instantiation limit"
proc isBoolType(type t) param return
  (t == bool) || (t == bool(8)) || (t == bool(16)) || (t == bool(32)) || (t == bool(64));

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
  (t == complex(64)) || (t == complex(128));

pragma "no instantiation limit"
proc isRealType(type t) param return
  (t == real(32)) || (t == real(64));

pragma "no instantiation limit"
proc isImagType(type t) param return
  (t == imag(32)) || (t == imag(64));

// Here is a single doc comment for the above.
/*
Each of the above functions returns `true` if its argument is
a corresponding type.
The argument must be a type.
*/
pragma "no instantiation limit"
proc isStringType(type t) param return t == string;

// Returns the unsigned equivalent of the input type.
pragma "no doc"
proc chpl__unsignedType(type t) type 
{
  if ! isIntegralType(t) then
    compilerError("range idxType is non-integral: ", typeToString(t));

  return uint(numBits(t));
}


// Returns the signed equivalent of the input type.
pragma "no doc"
proc chpl__signedType(type t) type 
{
  if ! isIntegralType(t) then
    compilerError("range idxType is non-integral: ", typeToString(t));

  return int(numBits(t));
}

pragma "no doc"
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

pragma "no doc"
proc isBoolValue(e)      param  return isBoolType(e.type);
pragma "no doc"
proc isIntValue(e)       param  return isIntType(e.type);
pragma "no doc"
proc isUintValue(e)      param  return isUintType(e.type);
pragma "no doc"
proc isRealValue(e)      param  return isRealType(e.type);
pragma "no doc"
proc isImagValue(e)      param  return isImagType(e.type);
pragma "no doc"
proc isComplexValue(e)   param  return isComplexType(e.type);
pragma "no doc"
proc isStringValue(e)    param  return isStringType(e.type);
pragma "no doc"
proc isIntegralValue(e)  param  return isIntegralType(e.type);
pragma "no doc"
proc isFloatValue(e)     param  return isFloatType(e.type);
pragma "no doc"
proc isNumericValue(e)   param  return isNumericType(e.type);
pragma "no doc"
proc isPrimitiveValue(e) param  return isPrimitiveType(e.type);
pragma "no doc"
proc isEnumValue(e)      param  return isEnumType(e.type);
//Defined elsewhere:
// isTupleValue
// isHomogeneousTupleValue
pragma "no doc"
proc isClassValue(e)     param  return isClassType(e.type);
pragma "no doc"
proc isRecordValue(e)    param  return isRecordType(e.type);
pragma "no doc"
proc isUnionValue(e)     param  return isUnionType(e.type);
//Defined elsewhere:
// isRangeValue
// isDmapValue
// isDomainValue
// isArrayValue
// isSyncValue
// isSingleValue
pragma "no doc"
proc isAtomicValue(e)    param  return isAtomicType(e.type);


//
// ixXxx() - the argument can be either a type or a value
//

// Set 1 - types.
// In chpldoc, isBool(e) et al. will apply to types as well.
pragma "no doc"
proc isBool(type t)      param  return isBoolType(t);
pragma "no doc"
proc isInt(type t)       param  return isIntType(t);
pragma "no doc"
proc isUint(type t)      param  return isUintType(t);
pragma "no doc"
proc isReal(type t)      param  return isRealType(t);
pragma "no doc"
proc isImag(type t)      param  return isImagType(t);
pragma "no doc"
proc isComplex(type t)   param  return isComplexType(t);
pragma "no doc"
proc isString(type t)    param  return isStringType(t);
pragma "no doc"
proc isIntegral(type t)  param  return isIntegralType(t);
pragma "no doc"
proc isFloat(type t)     param  return isFloatType(t);
pragma "no doc"
proc isNumeric(type t)   param  return isNumericType(t);
pragma "no doc"
proc isPrimitive(type t) param  return isPrimitiveType(t);
pragma "no doc"
proc isEnum(type t)      param  return isEnumType(t);
pragma "no doc"
proc isTuple(type t)     param  return isTupleType(t);
pragma "no doc"
proc isHomogeneousTuple(type t)  param  return isHomogeneousTupleType(t);
pragma "no doc"
proc isClass(type t)     param  return isClassType(t);
pragma "no doc"
proc isRecord(type t)    param  return isRecordType(t);
pragma "no doc"
proc isUnion(type t)     param  return isUnionType(t);
pragma "no doc"
proc isRange(type t)     param  return isRangeType(t);
pragma "no doc"
proc isDmap(type t)      param  return isDmapType(t);
pragma "no doc"
proc isDomain(type t)    param  return isDomainType(t);
pragma "no doc"
proc isArray(type t)     param  return isArrayType(t);
pragma "no doc"
proc isSync(type t)      param  return isSyncType(t);
pragma "no doc"
proc isSingle(type t)    param  return isSingleType(t);
pragma "no doc"
proc isAtomic(type t)    param  return isAtomicType(t);

// Set 2 - values.
/*
Returns `true` if the argument is a primitive type,
as defined by the language specification, or a value of a primitive type.
*/
proc isPrimitive(e) param  return isPrimitiveValue(e);
/*
Returns `true` if the argument is one the following types, of any width:
`int`, `uint`, `real`, `imag`, `complex`, or a value of such a type.
*/
proc isNumeric(e)   param  return isNumericValue(e);
/*
Returns `true` if the argument is one the following types, of any width:
`int`, `uint`, or a value of such a type.
*/
proc isIntegral(e)  param  return isIntegralValue(e);
/*
Returns `true` if the argument is one the following types, of any width:
`real`, `imag`, or a value of such a type.
*/
proc isFloat(e)     param  return isFloatValue(e);
// We document the following functions as one group, for user convenience.
proc isBool(e)      param  return isBoolValue(e);
proc isInt(e)       param  return isIntValue(e);
proc isUint(e)      param  return isUintValue(e);
proc isReal(e)      param  return isRealValue(e);
proc isImag(e)      param  return isImagValue(e);
proc isComplex(e)   param  return isComplexValue(e);
proc isString(e)    param  return isStringValue(e);
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
pragma "no doc"
proc isSync(e: sync)     param  return true; // workaround: not isSyncValue
proc isSync(e)           param  return false;
pragma "no doc"
proc isSingle(e: single) param  return true; // workaround: not isSingleValue
proc isSingle(e)         param  return false;
// Here is a single doc comment for the above.
/*
Each of the above functions returns `true` if its argument is
a corresponding type or a value of such a type.
*/
proc isAtomic(e)    param  return isAtomicValue(e);


// for internal use until we have a better name
pragma "no doc"
proc chpl_isSyncSingleAtomic(e)         param  return false;
pragma "no doc"
proc chpl_isSyncSingleAtomic(e: sync)   param  return true;
pragma "no doc"
proc chpl_isSyncSingleAtomic(e: single) param  return true;
pragma "no doc"
proc chpl_isSyncSingleAtomic(e)  param where isAtomicType(e.type)  return true;


// Is 'sub' a subtype (or equal to) 'super'?
/* Returns `true` if the type `sub` is a subtype of the type `super`. */
proc isSubtype(type sub, type super) param where   sub: super  return true;
pragma "no doc"
proc isSubtype(type sub, type super) param where !(sub: super) return false;

// Is 'sub' a proper subtype of 'super'?
/* Returns `true` if the type `sub` is a subtype of the type `super`
   and is not `super`. */
proc isProperSubtype(type sub, type super) param
  where isSubtype(sub, super) && sub != super
  return true;
pragma "no doc"
proc isProperSubtype(type sub, type super) param
  return false;

// What follows are the type _defaultOf methods, used to initialize types
// Booleans
pragma "no doc"
inline proc _defaultOf(type t) param where (isBoolType(t)) return false:t;

// ints, reals, imags, complexes
pragma "no doc"
inline proc _defaultOf(type t) param where (isIntegralType(t)) return 0:t;
// TODO: In order to make _defaultOf param for reals and imags we had to split
// the cases into their default size and a non-param case.  It is hoped that
// in the future, floating point numbers may be castable whilst param.  In that
// world, we can again shrink these calls into the size-ignorant case.
pragma "no doc"
inline proc _defaultOf(type t) param where t == real return 0.0;
pragma "no doc"
inline proc _defaultOf(type t) where (isRealType(t) && t != real) return 0.0:t;
pragma "no doc"
inline proc _defaultOf(type t) param where t == imag return 0.0i;
pragma "no doc"
inline proc _defaultOf(type t) where (isImagType(t) && t != imag) return 0.0i:t;
// Also, complexes cannot yet be parametized
pragma "no doc"
inline proc _defaultOf(type t): t where (isComplexType(t)) {
  var ret:t = noinit;
  param floatwidth = numBits(t)/2;
  ret.re = 0.0:real(floatwidth);
  ret.im = 0.0:real(floatwidth);
  return ret;
}

// Enums
pragma "no doc"
inline proc _defaultOf(type t) param where (isEnumType(t)) {
  return chpl_enum_first(t);
}

// Classes
pragma "no doc"
inline proc _defaultOf(type t) where (isClassType(t)) return nil:t;

// Various types whose default value is known
pragma "no doc"
inline proc _defaultOf(type t) param where t == void return _void;
pragma "no doc"
inline proc _defaultOf(type t) where t == opaque return _nullOpaque;
pragma "no doc"
inline proc _defaultOf(type t) where t == chpl_taskID_t return chpl_nullTaskID;
pragma "no doc"
inline proc _defaultOf(type t) where t == _sync_aux_t return _nullSyncVarAuxFields;
pragma "no doc"
inline proc _defaultOf(type t) where t == _single_aux_t return _nullSingleVarAuxFields;
pragma "no doc"
inline proc _defaultOf(type t) where t == _task_list return _nullTaskList;


// When I finish removing PRIM_INIT before initialization to a known value, then
// this method should work.  Until then, my stopgap will be an external function
// in the runtime.
//inline proc _defaultOf(type t) where t == memory_order return memory_order_seq_cst;
pragma "no doc"
extern proc _defaultOfMemoryOrder(): memory_order;

pragma "no instantiation limit"
pragma "compiler generated"
pragma "no doc"
inline proc _defaultOf(type t) where t == memory_order
  return _defaultOfMemoryOrder();

pragma "no instantiation limit"
pragma "compiler generated"
pragma "no doc"
inline proc _defaultOf(type t) {
  return nil:t;
}



// Returns true if it is legal to coerce t1 to t2, false otherwise.
pragma "no doc"
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
pragma "no doc"
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
// joint documentation, for user convenience
/*
Returns the number of bits used to store the values of type `t`.
This is available for all numeric types, fixed-width `bool` types,
and ``enum`` types.
It is not available for default-width `bool`.
*/
proc numBits(type t) param where t == bool {
  compilerError("default-width 'bool' does not have a well-defined size");
}
pragma "no doc"
proc numBits(type t) param where t == bool(8) return 8;
pragma "no doc"
proc numBits(type t) param where t == bool(16) return 16;
pragma "no doc"
proc numBits(type t) param where t == bool(32) return 32;
pragma "no doc"
proc numBits(type t) param where t == bool(64) return 64;
pragma "no doc"
proc numBits(type t) param where t == int(8) return 8;
pragma "no doc"
proc numBits(type t) param where t == int(16) return 16;
pragma "no doc"
proc numBits(type t) param where t == int(32) return 32;
pragma "no doc"
proc numBits(type t) param where t == int(64) return 64;
pragma "no doc"
proc numBits(type t) param where t == uint(8) return 8;
pragma "no doc"
proc numBits(type t) param where t == uint(16) return 16;
pragma "no doc"
proc numBits(type t) param where t == uint(32) return 32;
pragma "no doc"
proc numBits(type t) param where t == uint(64) return 64;
pragma "no doc"
proc numBits(type t) param where t == real(32) return 32;
pragma "no doc"
proc numBits(type t) param where t == real(64) return 64;
pragma "no doc"
proc numBits(type t) param where t == imag(32) return 32;
pragma "no doc"
proc numBits(type t) param where t == imag(64) return 64;
pragma "no doc"
proc numBits(type t) param where t == complex(64) return 64;
pragma "no doc"
proc numBits(type t) param where t == complex(128) return 128;

//
// numBytes(type) -- returns the number of bytes in a type
//

pragma "no doc" // do we want to document this one?
param bitsPerByte = 8;

/*
Returns the number of bytes used to store the values of type `t`.
This is available for all numeric types, fixed-width `bool` types,
and ``enum`` types.
It is not available for default-width `bool`.
*/
proc numBytes(type t) param return numBits(t)/8;

// joint documentation, for user convenience
/*
Returns the minimum value the type `t` can store.
`t` can be one of the following types, of any width:
`bool`, `int`, `uint`, `real`, `imag`, `complex`.
When `t` is a `bool` type, it returns `false`.
When `t` is `real`, `imag`, or `complex` type,
it is a non-``param`` function.
*/
proc min(type t) param  where isBool(t)      return false: t;

pragma "no doc"
proc min(type t) param  where t == int(8)    return 0x80: t;
pragma "no doc"
proc min(type t) param  where t == int(16)   return 0x8000: t;
pragma "no doc"
proc min(type t) param  where t == int(32)   return 0x80000000: t;
pragma "no doc"
proc min(type t) param  where t == int(64)   return 0x8000000000000000: t;

pragma "no doc"
proc min(type t) param  where isUint(t)      return 0: t;

pragma "no doc"
proc min(type t) where isFloatType(t)        return __primitive( "_min", t);

pragma "no doc"
proc min(type t) where isComplexType(t) {
  var x: t;
  x.re = min(x.re.type);
  x.im = min(x.im.type);
  return x;
}

// joint documentation, for user convenience
/*
Returns the minimum value the type `t` can store.
`t` can be one of the following types, of any width:
`bool`, `int`, `uint`, `real`, `imag`, `complex`.
When `t` is a `bool` type, it returns `false`.
When `t` is a `real`, `imag`, or `complex` type,
it is a non-``param`` function.
*/
proc max(type t) param  where isBool(t)      return true: t;

pragma "no doc"
proc max(type t) param  where t == int(8)    return 0x7f: t;
pragma "no doc"
proc max(type t) param  where t == int(16)   return 0x7fff: t;
pragma "no doc"
proc max(type t) param  where t == int(32)   return 0x7fffffff: t;
pragma "no doc"
proc max(type t) param  where t == int(64)   return 0x7fffffffffffffff: t;

pragma "no doc"
proc max(type t) param  where t == uint(8)   return 0xff: t;
pragma "no doc"
proc max(type t) param  where t == uint(16)  return 0xffff: t;
pragma "no doc"
proc max(type t) param  where t == uint(32)  return 0xffffffff: t;
pragma "no doc"
proc max(type t) param  where t == uint(64)  return 0xffffffffffffffff: t;

pragma "no doc"
proc max(type t) where isFloatType(t)        return __primitive( "_max", t);

pragma "no doc"
proc max(type t) where isComplexType(t) {
  var x: t;
  x.re = max(x.re.type);
  x.im = max(x.im.type);
  return x;
}

pragma "no doc"
iter chpl_enumerate(type t: enumerated) {
  const enumTuple = chpl_enum_enumerate(t);
  for i in 1..enumTuple.size do
    yield enumTuple(i);
}

// TODO add chpl_ to these functions' names - they are not intended for user.
pragma "no doc"
proc enum_minbits(type t: enumerated) param {
  return __primitive( "enum min bits", t);
}
pragma "no doc"
proc enum_issigned(type t: enumerated) param {
  return __primitive( "enum is signed", t);
}
// TODO - maybe this function can be useful for the user, for C interop?
// If so, give it a different name.
pragma "no doc"
proc enum_mintype(type t: enumerated) type {
  param minbits = enum_minbits(t);
  param signed = enum_issigned(t);
  if signed {
    return int(minbits);
  } else {
    return uint(minbits);
  }
}

pragma "no doc" // documented with the other numBits() above
proc numBits(type t: enumerated) param {
  return numBits(enum_mintype(t));
}

/*
Returns `this`, cast to the type `T`.
Generates a run-time error if `this` cannot be represented by `T`,
for example ``(-1).safeCast(uint)`` or ``256.safeCast(uint(8))``.

This method performs the minimum number of runtime checks.
For example, when casting from `uint(8)` to `uint(64)`,
no checks at all will be done.
*/
inline proc integral.safeCast(type T) : T where isUintType(T) {
  if castChecking {
    if isIntType(this.type) {
      // int(?) -> uint(?)
      if this < 0 then // runtime check
        halt("casting "+typeToString(this.type)+" less than 0 to "+typeToString(T));
    }

    if max(this.type):uint > max(T):uint {
      // [u]int(?) -> uint(?)
      if (this:uint > max(T):uint) then // runtime check
        halt("casting "+typeToString(this.type)+" with a value greater than the maximum of "+
             typeToString(T)+" to "+typeToString(T));
    }
  }
  return this:T;
}

pragma "no doc" // documented with the other safeCast above
inline proc integral.safeCast(type T) : T where isIntType(T) {
  if castChecking {
    if max(this.type):uint > max(T):uint {
      // this isUintType check lets us avoid a runtime check for this < 0
      if isUintType(this.type) {
        // uint(?) -> int(?)
        if this:uint > max(T):uint then // runtime check
          halt("casting "+typeToString(this.type)+" with a value greater than the maximum of "+
               typeToString(T)+" to "+typeToString(T));
      } else {
        // int(?) -> int(?)
        // max(T) <= max(int), so cast to int is safe
        if this:int > max(T):int then // runtime check
          halt("casting "+typeToString(this.type)+" with a value greater than the maximum of "+
               typeToString(T)+" to "+typeToString(T));
      }
    }
    if isIntType(this.type) {
      if min(this.type):int < min(T):int {
        // int(?) -> int(?)
        if this:int < min(T):int then // runtime check
          halt("casting "+typeToString(this.type)+" with a value less than the minimum of "+
               typeToString(T)+" to "+typeToString(T));
      }
    }
  }
  return this:T;
}

pragma "no doc" // documented with the other safeCast above
proc integral.safeCast(type T) {
  compilerError("safeCast is only supported between integral types");
}

//
// identity functions (for reductions)
//
pragma "no doc"
inline proc _prod_id(type t) return 1:t;
pragma "no doc"
inline proc _land_id(type t) return true;
pragma "no doc"
inline proc _lor_id(type t) return false;
pragma "no doc"
inline proc _lxor_id(type t) return false;
pragma "no doc"
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
pragma "no doc"
inline proc _bor_id(type t) return 0:t;
pragma "no doc"
inline proc _bxor_id(type t) return 0:t;

} // module Types
