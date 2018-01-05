/*
 * Copyright 2004-2018 Cray Inc.
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

.. note:: All Chapel programs automatically ``use`` this module by default.
          An explicit ``use`` statement is not necessary.

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

/* Returns `true` if the type `t` is the `void` type. */
pragma "no instantiation limit"
proc isVoidType(type t) param return t == void;

/* Returns `true` if the type `t` is a `bool` type, of any width. */
pragma "no instantiation limit"
proc isBoolType(type t) param return
  (t == bool) || (t == bool(8)) || (t == bool(16)) || (t == bool(32)) || (t == bool(64));

/* Returns `true` if the type `t` is an `int` type, of any width. */
pragma "no instantiation limit"
proc isIntType(type t) param return
  (t == int(8)) || (t == int(16)) || (t == int(32)) || (t == int(64));

/* Returns `true` if the type `t` is a `uint` type, of any width. */
pragma "no instantiation limit"
proc isUintType(type t) param return
  (t == uint(8)) || (t == uint(16)) || (t == uint(32)) || (t == uint(64));

/* Returns `true` if the type `t` is an `enum` type. */
pragma "no instantiation limit"
proc isEnumType(type t) param {
  proc isEnumHelp(type t: enumerated) param return true;
  proc isEnumHelp(type t) param return false;
  return isEnumHelp(t);
}

/* Returns `true` if the type `t` is a `complex` type, of any width. */
pragma "no instantiation limit"
proc isComplexType(type t) param return
  (t == complex(64)) || (t == complex(128));

/* Returns `true` if the type `t` is a `real` type, of any width. */
pragma "no instantiation limit"
proc isRealType(type t) param return
  (t == real(32)) || (t == real(64));

/* Returns `true` if the type `t` is an `imag` type, of any width. */
pragma "no instantiation limit"
proc isImagType(type t) param return
  (t == imag(32)) || (t == imag(64));

/* Returns `true` if the type `t` is the `string` type. */
pragma "no instantiation limit"
proc isStringType(type t) param return t == string;

/*
POD stands for Plain Old Data and roughly corresponds to the meaning of Plain
Old Data in C++.

A record, tuple, or union type in Chapel is a POD type if:

  * it does not set pragma "ignore noinit"
  * it has only a compiler generated autoCopy
  * it has only a compiler generated autoDestroy
  * it has only a compiler generated default initialization/construction
    routine
  * it has only a compiler generated = routine (when the left hand side
    and right hand side have the same type. Assignment overloads covering cases
    where left hand side and right hand side are different types are allowed
    for POD types)
  * it contains only POD type fields

User class types in Chapel are always considered POD types (because an instance
of the class is actually a pointer to the class object, and this pointer is
POD).

c_ptr is a POD type.

Primitive numeric/boolean/enumerated Chapel types are POD types as well.
 */
pragma "no instantiation limit"
pragma "no doc" // I don't think we want to make this public yet
proc isPODType(type t) param {
  return __primitive("is pod type", t);
}

// Returns the unsigned equivalent of the input type.
pragma "no doc"
proc chpl__unsignedType(type t) type 
{
  if ! isIntegralType(t) then
    compilerError("range idxType is non-integral: ", t:string);

  return uint(numBits(t));
}


// Returns the signed equivalent of the input type.
pragma "no doc"
proc chpl__signedType(type t) type 
{
  if ! isIntegralType(t) then
    compilerError("range idxType is non-integral: ", t:string);

  return int(numBits(t));
}

pragma "no doc"
proc chpl__maxIntTypeSameSign(type t) type {
  if ! isIntegralType(t) then
    compilerError("type t is non-integral: ", t:string);

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
pragma "no doc"
proc isRefIterValue(e)   param  return isRefIterType(e.type);
pragma "no doc"
proc isPODValue(e)       param  return isPODType(e.type);


//
// isXxx() - the argument can be either a type or a value
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
pragma "no doc"
proc isRefIter(type t)   param  return isRefIterType(t);
pragma "no doc"
proc isPOD(type t)       param  return isPODType(t);

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

/* Returns `true` if the argument is a `bool` type or value, of any width. */
proc isBool(e)      param  return isBoolValue(e);
/* Returns `true` if the argument is an `int` type or value, of any width. */
proc isInt(e)       param  return isIntValue(e);
/* Returns `true` if the argument is a `uint` type or value, of any width. */
proc isUint(e)      param  return isUintValue(e);
/* Returns `true` if the argument is a `real` type or value, of any width. */
proc isReal(e)      param  return isRealValue(e);
/* Returns `true` if the argument is an `imag` type or value, of any width. */
proc isImag(e)      param  return isImagValue(e);
/* Returns `true` if the argument is a `complex` type or value, of any width. */
proc isComplex(e)   param  return isComplexValue(e);
/* Returns `true` if the argument is a string or the `string` type. */
proc isString(e)    param  return isStringValue(e);
/* Returns `true` if the argument is an `enum` type or value, of any width. */
proc isEnum(e)      param  return isEnumValue(e);
/* Returns `true` if the argument is a tuple type or value. */
proc isTuple(e)     param  return isTupleValue(e);
/* Returns `true` if the argument is a homogeneous tuple.
   The argument must be a tuple or any type. */
proc isHomogeneousTuple(e: _tuple)  param  return isHomogeneousTupleValue(e);
/* Returns `true` if the argument is a class type or value
   that is not an ``extern`` class, or when the argument is ``nil``. */
proc isClass(e)     param  return isClassValue(e);
/* Returns `true` if the argument is a record type or value. */
proc isRecord(e)    param  return isRecordValue(e);
/* Returns `true` if the argument is a union type or value. */
proc isUnion(e)     param  return isUnionValue(e);
/* Returns `true` if the argument is a range type or value. */
proc isRange(e)     param  return isRangeValue(e);
/* Returns `true` if the argument is a domain map or a domain map type. */
proc isDmap(e)      param  return isDmapValue(e);
/* Returns `true` if the argument is a domain or a domain type. */
proc isDomain(e)    param  return isDomainValue(e);
/* Returns `true` if the argument is an array or an array type. */
proc isArray(e)     param  return isArrayValue(e);
pragma "no doc"
proc isSync(e: sync)     param  return true; // workaround: not isSyncValue
/* Returns `true` if the argument is a `sync` type or a `sync` variable. */
proc isSync(e)           param  return false;
pragma "no doc"
proc isSingle(e: single) param  return true; // workaround: not isSingleValue
/* Returns `true` if the argument is a `single` type or a `single` variable. */
proc isSingle(e)         param  return false;
/*Returns `true` if the argument is an `atomic` type or an `atomic` variable.*/
proc isAtomic(e)    param  return isAtomicValue(e);

pragma "no doc"
proc isRefIter(e)   param  return isRefIterValue(e);

pragma "no doc" // Not sure how we want to document isPOD* right now
proc isPOD(e)       param  return isPODValue(e);

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
private proc chpl__commonType(type s, type t) type
{
  if ! isIntegralType(s) then
    compilerError("Type ", s:string , " is non-integral: ");
  if ! isIntegralType(t) then
    compilerError("Type ", t:string , " is non-integral: ");

  if numBits(s) > numBits(t) then return s;
  if numBits(s) < numBits(t) then return t;

  if isIntType(s) && ! isIntType(t) ||
     isIntType(t) && ! isIntType(s) then
    compilerError("Types ", s:string , " and ", t:string, " are incompatible.");

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
  param floatwidth = numBits(t) / 2;
  return (min(real(floatwidth)), min(real(floatwidth))): t;
}

// joint documentation, for user convenience
/*
Returns the maximum value the type `t` can store.
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
  param floatwidth = numBits(t) / 2;
  return (max(real(floatwidth)), max(real(floatwidth))): t;
}

pragma "no doc"
iter chpl_enumerate(type t: enumerated) {
  const enumTuple = chpl_enum_enumerate(t);
  for i in 1..enumTuple.size do
    yield enumTuple(i);
}
pragma "no doc"
iter type enumerated.these(){
  for i in chpl_enumerate(this) do
    yield i;
}

// TODO add chpl_ to these functions' names - they are not intended for user.
private proc enum_minbits(type t: enumerated) param {
  return __primitive( "enum min bits", t);
}
private proc enum_issigned(type t: enumerated) param {
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
        halt("casting "+this.type:string+" less than 0 to "+T:string);
    }

    if max(this.type):uint > max(T):uint {
      // [u]int(?) -> uint(?)
      if (this:uint > max(T):uint) then // runtime check
        halt("casting "+this.type:string+" with a value greater than the maximum of "+
             T:string+" to "+T:string);
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
          halt("casting "+this.type:string+" with a value greater than the maximum of "+
               T:string+" to "+T:string);
      } else {
        // int(?) -> int(?)
        // max(T) <= max(int), so cast to int is safe
        if this:int > max(T):int then // runtime check
          halt("casting "+this.type:string+" with a value greater than the maximum of "+
               T:string+" to "+T:string);
      }
    }
    if isIntType(this.type) {
      if min(this.type):int < min(T):int {
        // int(?) -> int(?)
        if this:int < min(T):int then // runtime check
          halt("casting "+this.type:string+" with a value less than the minimum of "+
               T:string+" to "+T:string);
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
  if      isBoolType(t) then return true:t;
  else if isIntType(t)  then return -1:t;
  else if isUintType(t) then return max(t);
  else if t == real(32) then return max(uint(32)):t;
  else if t == real(64) then return max(uint(64)):t;
  else if t == imag(32) then return max(uint(32)):t;
  else if t == imag(64) then return max(uint(64)):t;
  else if t == complex(64) then return (max(uint(32)):real(32), max(uint(32)):real(32)):t;
  else if t == complex(128) then return (max(uint(64)):real(64), max(uint(64)):real(64)):t;
  else if isArrayType(t) {
    var result: t;
    type elT = result.eltType;
    forall elm in result do
      // Todo performance consideration: if elT is an array, we want to have
      // just one array per locale or numa domain.
      elm = _band_id(elT);
    return result;
  } else
    compilerError("Identity value for & over ", t:string, "is not available");
}
pragma "no doc"
inline proc _bor_id(type t) return 0:t;
pragma "no doc"
inline proc _bxor_id(type t) return 0:t;

} // module Types
