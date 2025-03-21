/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
*/
pragma "module included by default"
module Types {
  import HaltWrappers;

pragma "suppress generic actual warning"
@chpldoc.nodoc // joint documentation with the next one
proc isType(type t) param do return true;
/* Returns ``true`` if the argument is a type. */
pragma "suppress generic actual warning"
proc isType(e) param do return false;

@chpldoc.nodoc // joint documentation with the next one
proc isParam(type t)  param do return false;
@chpldoc.nodoc // joint documentation with the next one
proc isParam(param p) param do return true;
/* Returns ``true`` if the argument is a param. */
proc isParam(e)       param do return false;

//
// isXxxType()
//

// TODO eliminate this; beware of isPrimitive()
@chpldoc.nodoc
proc _isPrimitiveType(type t) param do return
  isBoolType(t)  ||
  isIntegralType(t) ||
  isRealType(t)     ||
//To allow imag, need to define casts from primitive types into imag.
//isImagType(t)     ||
  (t == chpl_c_string);

/*
Returns ``true`` if the type ``t`` is a primitive type,
as defined by the language specification.
*/
proc isPrimitiveType(type t) param do return
  isNothingType(t) || isVoidType(t) || isBoolType(t) ||
  isNumericType(t) || isStringType(t) || isBytesType(t);

/*
Returns ``true`` if the type ``t`` is one the following types, of any width:
``int``, ``uint``, ``real``, ``imag``, ``complex``.
*/
proc isNumericType(type t) param do return
  isIntegralType(t) || isRealType(t) || isImagType(t) || isComplexType(t);

/*
Returns ``true`` if the type ``t`` is one the following types, of any width:
``int``, ``uint``.
*/
proc isIntegralType(type t) param do return
  isIntType(t) || isUintType(t);

/* Returns ``true`` if the type ``t`` is the ``nothing`` type. */
proc isNothingType(type t) param do return t == nothing;

/* Returns ``true`` if the type ``t`` is a ``bool`` type, of any width. */
proc isBoolType(type t) param do
  return (t == bool);

/* Returns ``true`` if the type ``t`` is an ``int`` type, of any width. */
proc isIntType(type t) param do return
  (t == int(8)) || (t == int(16)) || (t == int(32)) || (t == int(64));

/* Returns ``true`` if the type ``t`` is a ``uint`` type, of any width. */
proc isUintType(type t) param do return
  (t == uint(8)) || (t == uint(16)) || (t == uint(32)) || (t == uint(64));

/* Returns ``true`` if the type ``t`` is a ``real`` type, of any width. */
proc isRealType(type t) param do return
  (t == real(32)) || (t == real(64));

/* Returns ``true`` if the type ``t`` is an ``imag`` type, of any width. */
proc isImagType(type t) param do return
  (t == imag(32)) || (t == imag(64));

/* Returns ``true`` if the type ``t`` is a ``complex`` type, of any width. */
proc isComplexType(type t) param do return
  (t == complex(64)) || (t == complex(128));

/* Returns ``true`` if the type ``t`` is the ``string`` type. */
proc isStringType(type t) param do return t == string;

/* Returns ``true`` if the type ``t`` is the ``bytes`` type. */
proc isBytesType(type t) param do return t == bytes;

/* Returns ``true`` if the type ``t`` is an ``enum`` type. */
proc isEnumType(type t) param {
  proc isEnumHelp(type t: enum) param do return true;
  proc isEnumHelp(type t) param do return false;
  return isEnumHelp(t);
}

/* Return true if ``t`` is a class type. Otherwise return false. */
pragma "suppress generic actual warning"
proc isClassType(type t) param do return __primitive("is class type", t);

/* Return true if ``t`` is a record type. Otherwise return false. */
pragma "suppress generic actual warning"
proc isRecordType(type t) param {
  if __primitive("is record type", t) == false then
    return false;

  // some non-record types are implemented via records - exclude those
  else if isDmapType(t)   ||
          isDomainType(t) ||
          isArrayType(t)  ||
          isRangeType(t)  ||
          isTupleType(t)  ||
          isSyncType(t)   ||
          isStringType(t) ||
          isAtomicType(t) then
    return false;

  else
    return true;
}

/* Return true if ``t`` is a union type. Otherwise return false. */
pragma "suppress generic actual warning"
proc isUnionType(type t) param do return __primitive("is union type", t);

/* Returns ``true`` if its argument is a tuple type.  */
proc isTupleType(type t) param do
  return __primitive("is tuple type", t);

/* Return true if argument ``t`` is a range type, false otherwise */
proc isRangeType(type t) param {
  return isSubtype(t, range(?));
}

/* Return true if ``t`` is a domain type. Otherwise return false. */
proc isDomainType(type t) param {
  return isSubtype(t, _domain);
}

/* Return true if ``t`` is an array type. Otherwise return false. */
pragma "fn exempt instantiation limit"
proc isArrayType(type t) param {
  return isSubtype(t, _array);
}

/* Return true if ``t`` is a domain map type. Otherwise return false. */
proc isDmapType(type t) param {
  return isSubtype(t, _distribution);
}

/* Returns true if ``t`` is a sync type, false otherwise. */
proc isSyncType(type t) param {
  return isSubtype(t, _syncvar);
}

/* Returns true if ``t`` is an atomic type, false otherwise. */
proc isAtomicType(type t) param do return __primitive("is atomic type", t);

/* Returns ``true`` if the type ``t`` is the ``void`` type. */
proc isVoidType(type t) param do return t == void;

/* Returns ``true`` if the type ``t`` is an abstract ``enum`` type (one in which
   not all symbols have associated integer values). */
proc isAbstractEnumType(type t) param {
  return __primitive("is abstract enum type", t);
}

/* Returns ``true`` if its argument is a homogeneous tuple type. */
proc isHomogeneousTupleType(type t) param do
  return __primitive("is star tuple type", t);

/* Returns ``true`` if the type ``t`` is an extern union type. */
proc isExternUnionType(type t) param do return __primitive("is extern union type", t);

/* Returns ``true`` if the type ``t`` is a ref iter type. */
proc isRefIterType(type t) param do return __primitive("is ref iter type", t);

@chpldoc.nodoc
proc isOwnedClassType(type t)     param do return isSubtype(t, _owned);
@chpldoc.nodoc
proc isSharedClassType(type t)    param do return isSubtype(t, _shared);
@chpldoc.nodoc
proc isUnmanagedClassType(type t) param do return isSubtype(t, unmanaged);
@chpldoc.nodoc
proc isBorrowedClassType(type t)  param {
  return __primitive("is borrowed class type", t);
}

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

User unmanaged/borrowed class types in Chapel are always considered POD types
(because an instance of the class is actually a pointer to the class object,
and this pointer is POD).

c_ptr is a POD type.

Primitive numeric/boolean/enum Chapel types are POD types as well.
 */
@chpldoc.nodoc // I don't think we want to make this public yet
proc isPODType(type t) param {
  use CTypes;
  if isSubtype(t, c_array) then
    return isPODType(t.eltType);

  return __primitive("is pod type", t);
}
@chpldoc.nodoc
proc isCopyableType(type t) param {
  return __primitive("is copyable type", t);
}
@chpldoc.nodoc
proc isConstCopyableType(type t) param {
  return __primitive("is const copyable type", t);
}
@chpldoc.nodoc
proc isAssignableType(type t) param {
  return __primitive("is assignable type", t);
}
@chpldoc.nodoc
proc isConstAssignableType(type t) param {
  return __primitive("is const assignable type", t);
}
@chpldoc.nodoc
proc isDefaultInitializableType(type t) param {
  return __primitive("type has default value", t);
}

// Returns the unsigned equivalent of the input type.
proc chpl__unsignedType(type t) type
{
  return uint(numBits(t));
}


// Returns the signed equivalent of the input type.
proc chpl__signedType(type t) type
{
  return int(numBits(t));
}

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
//

/* Returns ``true`` if the argument is a value of primitive type. */
proc isPrimitiveValue(e) param do  return isPrimitiveType(e.type);

/* Returns ``true`` if the argument is a value of one the following types:
``int``, ``uint``, ``real``, ``imag``, ``complex``. */
proc isNumericValue(e)   param do  return isNumericType(e.type);

/* Returns ``true`` if the argument is a value of one the following types:
``int``, ``uint``. */
proc isIntegralValue(e)  param do  return isIntegralType(e.type);

/* Returns ``true`` if the argument is a ``nothing`` value (i.e., ``none``) */
proc isNothingValue(e)   param do return isNothingType(e.type);

/* Returns ``true`` if the argument is a ``bool`` value. */
proc isBoolValue(e)      param do  return isBoolType(e.type);

/* Returns ``true`` if the argument is a ``int`` value. */
proc isIntValue(e)       param do  return isIntType(e.type);

/* Returns ``true`` if the argument is a ``uint`` value. */
proc isUintValue(e)      param do  return isUintType(e.type);

/* Returns ``true`` if the argument is a ``real`` value. */
proc isRealValue(e)      param do  return isRealType(e.type);

/* Returns ``true`` if the argument is a ``imag`` value. */
proc isImagValue(e)      param do  return isImagType(e.type);

/* Returns ``true`` if the argument is a ``complex`` value. */
proc isComplexValue(e)   param do  return isComplexType(e.type);

/* Returns ``true`` if the argument is a ``string`` value. */
proc isStringValue(e)    param do  return isStringType(e.type);

/* Returns ``true`` if the argument is a ``bytes`` value. */
proc isBytesValue(e)     param do  return isBytesType(e.type);

/* Returns ``true`` if the argument is a ``enum`` value. */
proc isEnumValue(e)      param do  return isEnumType(e.type);

/* Returns ``true`` if the argument is a class value */
proc isClassValue(e)     param do return isClassType(e.type);

/* Returns ``true`` if the argument is a record value */
proc isRecordValue(e)    param do  return isRecordType(e.type);

/* Returns ``true`` if the argument is a union value */
proc isUnionValue(e)     param do  return isUnionType(e.type);

/* Returns ``true`` if the argument is a tuple value */
proc isTupleValue(x)     param do return isTupleType(x.type);

/* Return true if argument ``r`` is a range, false otherwise */
proc isRangeValue(r)     param do return isRangeType(r.type);

/* Return true if ``e`` is a domain. Otherwise return false. */
proc isDomainValue(e)    param do  return isDomainType(e.type);

/* Return true if ``e`` is an array. Otherwise return false. */
pragma "fn exempt instantiation limit"
proc isArrayValue(e)     param do  return isArrayType(e.type);

/* Return true if ``e`` is a domain map. Otherwise return false. */
proc isDmapValue(e)      param do  return isDmapType(e.type);

/* Returns ``true`` if the argument is a sync value */
proc isSyncValue(x)      param do  return isSyncType(x.type);

/* Returns ``true`` if the argument is an atomic value */
proc isAtomicValue(e)    param do  return isAtomicType(e.type);

@chpldoc.nodoc
proc isHomogeneousTupleValue(x) param do return __primitive("is star tuple type", x);
pragma "no borrow convert"
@chpldoc.nodoc
proc isOwnedClassValue(e)     param do return isOwnedClassType(e.type);
pragma "no borrow convert"
@chpldoc.nodoc
proc isSharedClassValue(e)    param do return isSharedClassType(e.type);
pragma "no borrow convert"
@chpldoc.nodoc
proc isUnmanagedClassValue(e) param do return isUnmanagedClassType(e.type);
pragma "no borrow convert"
@chpldoc.nodoc
proc isBorrowedClassValue(e)  param do return isBorrowedClassType(e.type);
@chpldoc.nodoc
proc isNilableClassValue(e)   param do return isNilableClassType(e.type);
@chpldoc.nodoc
proc isNonNilableClassValue(e)   param do return isNonNilableClassType(e.type);
@chpldoc.nodoc
proc isRefIterValue(e)   param do  return isRefIterType(e.type);
@chpldoc.nodoc
proc isPODValue(e)       param do  return isPODType(e.type);
@chpldoc.nodoc
proc isCopyableValue(e)     param do  return isCopyableType(e.type);
@chpldoc.nodoc
proc isConstCopyableValue(e)  param do  return isConstCopyableType(e.type);
@chpldoc.nodoc
proc isAssignableValue(e)   param do  return isAssignableType(e.type);
@chpldoc.nodoc
proc isConstAssignableValue(e)  param do  return isConstAssignableType(e.type);
@chpldoc.nodoc
proc isDefaultInitializableValue(e) param do return isDefaultInitializableType(e.type);

//
// isXxx() - the argument can be either a type or a value
//

// Set 1 - types.
// In chpldoc, isBool(e) et al. will apply to types as well.

@chpldoc.nodoc
proc isPrimitive(type t) param do  return isPrimitiveType(t);
@chpldoc.nodoc
proc isNumeric(type t)   param do  return isNumericType(t);
@chpldoc.nodoc
proc isIntegral(type t)  param do  return isIntegralType(t);

@chpldoc.nodoc
proc isNothing(type t)  param do return isNothingType(t);
@chpldoc.nodoc
proc isBool(type t)      param do  return isBoolType(t);
@chpldoc.nodoc
proc isInt(type t)       param do  return isIntType(t);
@chpldoc.nodoc
proc isUint(type t)      param do  return isUintType(t);
@chpldoc.nodoc
proc isReal(type t)      param do  return isRealType(t);
@chpldoc.nodoc
proc isImag(type t)      param do  return isImagType(t);
@chpldoc.nodoc
proc isComplex(type t)   param do  return isComplexType(t);
@chpldoc.nodoc
proc isString(type t)    param do  return isStringType(t);
@chpldoc.nodoc
proc isBytes(type t)     param do  return isBytesType(t);
@chpldoc.nodoc
proc isEnum(type t)      param do  return isEnumType(t);
@chpldoc.nodoc
proc isClass(type t)     param do  return isClassType(t);
@chpldoc.nodoc
proc isRecord(type t)    param do  return isRecordType(t);
@chpldoc.nodoc
proc isUnion(type t)     param do  return isUnionType(t);
@chpldoc.nodoc
proc isTuple(type t)     param do  return isTupleType(t);
@chpldoc.nodoc
proc isRange(type t)     param do  return isRangeType(t);
@chpldoc.nodoc
proc isDomain(type t)    param do  return isDomainType(t);
@chpldoc.nodoc
proc isArray(type t)     param do  return isArrayType(t);
@chpldoc.nodoc
proc isDmap(type t)      param do  return isDmapType(t);
@chpldoc.nodoc
proc isSync(type t)      param do  return isSyncType(t);
@chpldoc.nodoc
proc isAtomic(type t)    param do  return isAtomicType(t);

pragma "suppress generic actual warning"
@chpldoc.nodoc
proc isGeneric(type t)   param do  return isGenericType(t);

@chpldoc.nodoc
proc isHomogeneousTuple(type t)  param do  return isHomogeneousTupleType(t);
@chpldoc.nodoc
proc isOwnedClass(type t) param do  return isOwnedClassType(t);
@chpldoc.nodoc
proc isSharedClass(type t) param do  return isSharedClassType(t);
@chpldoc.nodoc
proc isUnmanagedClass(type t) param do  return isUnmanagedClassType(t);
@chpldoc.nodoc
proc isBorrowedClass(type t) param do  return isBorrowedClassType(t);
@chpldoc.nodoc
proc isNilableClass(type t) param do  return isNilableClassType(t);
@chpldoc.nodoc
proc isNonNilableClass(type t) param do  return isNonNilableClassType(t);
@chpldoc.nodoc
proc isRefIter(type t)   param do  return isRefIterType(t);
@chpldoc.nodoc
proc isPOD(type t)       param do  return isPODType(t);
@chpldoc.nodoc
proc isCopyable(type t)      param do  return isCopyableType(t);
@chpldoc.nodoc
proc isConstCopyable(type t)   param do  return isConstCopyableType(t);
@chpldoc.nodoc
proc isAssignable(type t)    param do  return isAssignableType(t);
@chpldoc.nodoc
proc isConstAssignable(type t) param do  return isConstAssignableType(t);
@chpldoc.nodoc
proc isDefaultInitializable(type t) param do return isDefaultInitializableType(t);


// Set 2 - values.

/*
Returns ``true`` if the argument is a primitive type,
as defined by the language specification, or a value of a primitive type.
*/
proc isPrimitive(e) param do  return isPrimitiveValue(e);
/*
Returns ``true`` if the argument is one the following types, of any width:
``int``, ``uint``, ``real``, ``imag``, ``complex``, or a value of such a type.
*/
proc isNumeric(e)   param do  return isNumericValue(e);
/*
Returns ``true`` if the argument is one the following types, of any width:
``int``, ``uint``, or a value of such a type.
*/
proc isIntegral(e)  param do  return isIntegralValue(e);

/* Returns ``true`` if the argument is ``none`` or the ``nothing`` type.
 as defined by the language specification.*/
proc isNothing(e)   param do return isNothingType(e.type);
/* Returns ``true`` if the argument is a ``bool`` type or value, of any width. */
proc isBool(e)      param do  return isBoolValue(e);
/* Returns ``true`` if the argument is an ``int`` type or value, of any width. */
proc isInt(e)       param do  return isIntValue(e);
/* Returns ``true`` if the argument is a ``uint`` type or value, of any width. */
proc isUint(e)      param do  return isUintValue(e);
/* Returns ``true`` if the argument is a ``real`` type or value, of any width. */
proc isReal(e)      param do  return isRealValue(e);
/* Returns ``true`` if the argument is an ``imag`` type or value, of any width. */
proc isImag(e)      param do  return isImagValue(e);
/* Returns ``true`` if the argument is a ``complex`` type or value, of any width. */
proc isComplex(e)   param do  return isComplexValue(e);
/* Returns ``true`` if the argument is a string or the ``string`` type. */
proc isString(e)    param do  return isStringValue(e);
/* Returns ``true`` if the argument is a bytes or the ``bytes`` type. */
proc isBytes(e)     param do  return isBytesValue(e);
/* Returns ``true`` if the argument is an ``enum`` type or value, of any width. */
proc isEnum(e)      param do  return isEnumValue(e);
/* Returns ``true`` if the argument is a class type or value
   that is not an ``extern`` class, or when the argument is ``nil``. */
proc isClass(e)     param do  return isClassValue(e);
/* Returns ``true`` if the argument is a record type or value. */
proc isRecord(e)    param do  return isRecordValue(e);
/* Returns ``true`` if the argument is a union type or value. */
proc isUnion(e)     param do  return isUnionValue(e);
/* Returns ``true`` if the argument is a tuple type or value. */
proc isTuple(e)     param do  return isTupleValue(e);
/* Returns ``true`` if the argument is a range type or value. */
proc isRange(e)     param do  return isRangeValue(e);
/* Returns ``true`` if the argument is a domain or a domain type. */
proc isDomain(e)    param do  return isDomainValue(e);
/* Returns ``true`` if the argument is an array or an array type. */
proc isArray(e)     param do  return isArrayValue(e);
/* Returns ``true`` if the argument is a domain map or a domain map type. */
proc isDmap(e)      param do  return isDmapValue(e);
/* Returns ``true`` if the argument is a ``sync`` type or a ``sync`` variable. */
proc isSync(e)      param do  return isSyncValue(e);
/* Returns ``true`` if the argument is a ``single`` type or a ``single`` variable. */
proc isSingle(e)    param do  return isSingleValue(e);
/*Returns ``true`` if the argument is an ``atomic`` type or an ``atomic`` variable.*/
proc isAtomic(e)    param do  return isAtomicValue(e);

/* Returns ``true`` if the argument is a homogeneous tuple.
   The argument must be a tuple or any type. */
proc isHomogeneousTuple(e)  param do  return isHomogeneousTupleValue(e);
/* Returns ``true`` if the argument is a generic type, and ``false`` otherwise. */
proc isGeneric(e)   param do  return false;
/* Returns ``true`` if the argument is an ``owned`` class type. */
pragma "no borrow convert"
proc isOwnedClass(e)     param do  return isOwnedClassValue(e);
/* Returns ``true`` if the argument is a ``shared`` class type. */
pragma "no borrow convert"
proc isSharedClass(e)     param do  return isSharedClassValue(e);
/* Returns ``true`` if the argument is a ``unmanaged`` class type. */
pragma "no borrow convert"
proc isUnmanagedClass(e)     param do  return isUnmanagedClassValue(e);
/* Returns ``true`` if the argument is a ``borrowed`` class type. */
pragma "no borrow convert"
proc isBorrowedClass(e)     param do  return isBorrowedClassValue(e);
/* Returns ``true`` if the argument is a class type that can store ``nil``. */
proc isNilableClass(e)     param do  return isNilableClassValue(e);
/* Returns ``true`` if the argument is a class type that cannot store ``nil``. */
proc isNonNilableClass(e)  param do  return isNonNilableClassValue(e);
@chpldoc.nodoc
proc isRefIter(e)   param do  return isRefIterValue(e);
@chpldoc.nodoc // Not sure how we want to document isPOD* right now
proc isPOD(e)       param do  return isPODValue(e);

/*

Returns ``true`` if the argument is a type or an expression of a type
that can be copy-initialized and ``false`` otherwise.

Note that even if this function returns ``true``, it might be the case that the
type only supports copy-initialization from mutable values.
:type:`~OwnedObject.owned` is an example of a type with that behavior.

See also the specification section :ref:`Copy_Initialization_of_Records`.

*/
proc isCopyable(e) param do return isCopyableValue(e);

/*

Returns ``true`` if the argument is a type or an expression of a type
that can be copy-initialized from a ``const`` value and ``false`` otherwise.

Returns ``false`` for :type:`~OwnedObject.owned` because copy-initialization
for that type leaves the source argument storing ``nil``.

See also the specification section :ref:`Copy_Initialization_of_Records`.

*/
proc isConstCopyable(e) param do return isConstCopyableValue(e);

/*

Returns ``true`` if the argument is a type or expression of a type that
can be assigned from another value and ``false`` otherwise.

Note that even if this function returns ``true``, it might be the case that the
type only supports assignment from mutable values.
:type:`~OwnedObject.owned` is an example of a type with that behavior.

See also the specification section :ref:`Record_Assignment`.

*/
proc isAssignable(e) param do return isCopyableValue(e);

/*

Returns ``true`` if the argument is a type or expression of a type that
can be assigned from a ``const`` value and ``false`` otherwise.

Returns ``false`` for  :type:`~OwnedObject.owned` because assignment
for that type leaves the source argument storing ``nil``.

See also the specification section :ref:`Record_Assignment`.

*/
proc isConstAssignable(e) param do return isConstAssignableValue(e);

/*

Returns ``true`` if the argument is a type or expression of a type that
can be default initialized and ``false`` otherwise.

Returns ``false`` for non-nilable class types because these types do not
have a default value.

See also the specification section :ref:`Default_Values_For_Types`.

*/
proc isDefaultInitializable(e) param do return isDefaultInitializableValue(e);


// for internal use until we have a better name
proc chpl_isSyncSingleAtomic(e: ?t) param do return
  isSyncType(t) ||
  isAtomicType(t);

// isSubtype(), isProperSubtype() are now directly handled by compiler

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

/* If the argument is a class type, returns its nilable version like ``arg?``.
   Otherwise returns the argument unchanged. */
proc toNilableIfClassType(type arg) type {
  if isNonNilableClassType(arg)   // btw #14920
  then return arg?;
  else return arg;
}


//
// numBits(type) -- returns the number of bits in a type
//
// joint documentation, for user convenience
/*
Returns the number of bits used to store the values of type ``t``.
This is available for all numeric types.
*/
pragma "no where doc"
proc numBits(type t) param where t == bool {
  compilerError("default-width 'bool' does not have a well-defined size");
}
@chpldoc.nodoc
proc numBits(type t) param where t == int(8) do return 8;
@chpldoc.nodoc
proc numBits(type t) param where t == int(16) do return 16;
@chpldoc.nodoc
proc numBits(type t) param where t == int(32) do return 32;
@chpldoc.nodoc
proc numBits(type t) param where t == int(64) do return 64;
@chpldoc.nodoc
proc numBits(type t) param where t == uint(8) do return 8;
@chpldoc.nodoc
proc numBits(type t) param where t == uint(16) do return 16;
@chpldoc.nodoc
proc numBits(type t) param where t == uint(32) do return 32;
@chpldoc.nodoc
proc numBits(type t) param where t == uint(64) do return 64;
@chpldoc.nodoc
proc numBits(type t) param where t == real(32) do return 32;
@chpldoc.nodoc
proc numBits(type t) param where t == real(64) do return 64;
@chpldoc.nodoc
proc numBits(type t) param where t == imag(32) do return 32;
@chpldoc.nodoc
proc numBits(type t) param where t == imag(64) do return 64;
@chpldoc.nodoc
proc numBits(type t) param where t == complex(64) do return 64;
@chpldoc.nodoc
proc numBits(type t) param where t == complex(128) do return 128;

//
// numBytes(type) -- returns the number of bytes in a type
//

@chpldoc.nodoc // do we want to document this one?
param bitsPerByte = 8;

/*
Returns the number of bytes used to store the values of type ``t``.
This is available for all numeric types.
*/
proc numBytes(type t) param do return numBits(t)/8;

// joint documentation, for user convenience
/*
Returns the minimum value the type ``t`` can store.
``t`` can be one of the following types, of any width:
``bool``, ``int``, ``uint``, ``real``, ``imag``, ``complex``.
When ``t`` is a ``bool`` type, it returns ``false``.
When ``t`` is ``real``, ``imag``, or ``complex`` type,
it is a non-``param`` function.
*/
pragma "no where doc"
proc min(type t) param  where isBool(t) do      return false: t;

@chpldoc.nodoc
proc min(type t) param  where t == int(8) do    return 0x80: t;
@chpldoc.nodoc
proc min(type t) param  where t == int(16) do   return 0x8000: t;
@chpldoc.nodoc
proc min(type t) param  where t == int(32) do   return 0x80000000: t;
@chpldoc.nodoc
proc min(type t) param  where t == int(64) do   return 0x8000000000000000: t;

@chpldoc.nodoc
proc min(type t) param  where isUint(t) do      return 0: t;

@chpldoc.nodoc
proc min(type t) where isRealType(t) || isImagType(t) do
  return __primitive( "_min", t);

@chpldoc.nodoc
proc min(type t) where isComplexType(t) {
  param floatwidth = numBits(t) / 2;
  return (min(real(floatwidth)), min(real(floatwidth))): t;
}

pragma "last resort" @chpldoc.nodoc
proc min(type t) {
  compilerError("'min(type t)' is not defined for t=", t:string);
}

// joint documentation, for user convenience
/*
Returns the maximum value the type ``t`` can store.
``t`` can be one of the following types, of any width:
``bool``, ``int``, ``uint``, ``real``, ``imag``, ``complex``.
When ``t`` is a ``bool`` type, it returns ``true``.
When ``t`` is a ``real``, ``imag``, or ``complex`` type,
it is a non-``param`` function.
*/
pragma "no where doc"
proc max(type t) param  where isBool(t) do      return true: t;

@chpldoc.nodoc
proc max(type t) param  where t == int(8) do    return 0x7f: t;
@chpldoc.nodoc
proc max(type t) param  where t == int(16) do   return 0x7fff: t;
@chpldoc.nodoc
proc max(type t) param  where t == int(32) do   return 0x7fffffff: t;
@chpldoc.nodoc
proc max(type t) param  where t == int(64) do   return 0x7fffffffffffffff: t;

@chpldoc.nodoc
proc max(type t) param  where t == uint(8) do   return 0xff: t;
@chpldoc.nodoc
proc max(type t) param  where t == uint(16) do  return 0xffff: t;
@chpldoc.nodoc
proc max(type t) param  where t == uint(32) do  return 0xffffffff: t;
@chpldoc.nodoc
proc max(type t) param  where t == uint(64) do  return 0xffffffffffffffff: t;

@chpldoc.nodoc
proc max(type t) where isRealType(t) || isImagType(t) do        return __primitive( "_max", t);

@chpldoc.nodoc
proc max(type t) where isComplexType(t) {
  param floatwidth = numBits(t) / 2;
  return (max(real(floatwidth)), max(real(floatwidth))): t;
}

pragma "last resort" @chpldoc.nodoc
proc max(type t) {
  compilerError("'max(type t)' is not defined for t=", t:string);
}

@chpldoc.nodoc
iter type enum.these(){
  foreach i in 0..<this.size do
    yield chpl__orderToEnum(i, this);
}

@chpldoc.nodoc
proc type enum.first {
  return chpl__orderToEnum(0, this);
}

@chpldoc.nodoc
proc type enum.last {
  return chpl__orderToEnum(this.size-1, this);
}

private proc chpl_enum_minbits(type t: enum) param {
  if t.size <= max(uint(8)) then
    return 8;
  if t.size <= max(uint(16)) then
    return 16;
  if t.size <= max(uint(32)) then
    return 32;
  return 64;
}
// TODO - maybe this function can be useful for the user, for C interop?
// If so, give it a different name.
proc chpl_enum_mintype(type t: enum) type {
  return uint(chpl_enum_minbits(t));
}


/*
The following ``safeCast()`` methods return ``this`` cast to the type ``T``.
At present, these halt the program if ``this`` cannot be represented by ``T``,
for example ``(-1).safeCast(uint)`` or ``256.safeCast(uint(8))``.

These methods perform the minimum number of runtime checks.
For example, when casting from ``uint(8)`` to ``uint(64)``,
no checks at all will be done.
*/
@unstable("integral.safeCast() is unstable and its behavior may change in the future")
inline proc integral.safeCast(type T: integral) : T {
  if castChecking then
    if const error = this.chpl_checkValue(T) then
      HaltWrappers.safeCastCheckHalt(error.message());
  return this:T;
}

/* Return 'nil' if 'this' fits into 'T', an IllegalArgumentError otherwise. */
proc integral.chpl_checkValue(type T: integral): owned IllegalArgumentError? {
    if isUintType(T) {
      if isIntType(this.type) {
        // int(?) -> uint(?)
        if this < 0 then // runtime check
          return new IllegalArgumentError("bad cast from "+this.type:string+
                                         " less than 0 to "+T:string);
      }

      if max(this.type):uint > max(T):uint {
        // [u]int(?) -> uint(?)
        if (this:uint > max(T):uint) then // runtime check
          return new IllegalArgumentError("bad cast from "+this.type:string+
                                         " with a value greater than the maximum of "+ T:string+" to "+T:string);
      }
    }

    if isIntType(T) {
      if max(this.type):uint > max(T):uint {
        // this isUintType check lets us avoid a runtime check for this < 0
        if isUintType(this.type) {
          // uint(?) -> int(?)
          if this:uint > max(T):uint then // runtime check
            return new IllegalArgumentError("bad cast from "+this.type:string+
                                           " with a value greater than the maximum of "+ T:string+" to "+T:string);
        } else {
          // int(?) -> int(?)
          // max(T) <= max(int), so cast to int is safe
          if this:int > max(T):int then // runtime check
            return new IllegalArgumentError("bad cast from "+this.type:string+
                                           " with a value greater than the maximum of "+ T:string+" to "+T:string);
        }
      }
      if isIntType(this.type) {
        if min(this.type):int < min(T):int {
          // int(?) -> int(?)
          if this:int < min(T):int then // runtime check
            return new IllegalArgumentError("bad cast from "+this.type:string+
                                           " with a value less than the minimum of "+ T:string+" to "+T:string);
        }
      }
    }

  return nil;
}

@unstable("integral.safeCast() is unstable and its behavior may change in the future")
proc integral.safeCast(type T: bool) {
  if this != 0 && this != 1 then
    HaltWrappers.safeCastCheckHalt("casting "+this.type:string+" to 'bool' requires it to have a value of either 0 or 1, but the current value is " + this:string);
  return this: bool;
}

@unstable("bool.safeCast() is unstable and its behavior may change in the future")
proc bool.safeCast(type T: integral) {
  return this: T;
}

@unstable("bool.safeCast() is unstable and its behavior may change in the future")
proc bool.safeCast(type T: bool) {
  return this;
}

@chpldoc.nodoc // documented with the other safeCast above
proc integral.safeCast(type T) {
  compilerError("safeCasts are not supported from " + this.type:string +
                " to " + T:string);
}

//
// identity functions (for reductions)
//
@chpldoc.nodoc
inline proc _prod_id(type t) do return 1:t;
@chpldoc.nodoc
inline proc _land_id(type t) do return true;
@chpldoc.nodoc
inline proc _lor_id(type t) do return false;
@chpldoc.nodoc
inline proc _lxor_id(type t) do return false;
@chpldoc.nodoc
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
@chpldoc.nodoc
inline proc _bor_id(type t) do return 0:t;
@chpldoc.nodoc
inline proc _bxor_id(type t) do return 0:t;

// the following functions (isCoercible etc) are handled directly by
// the compiler - so their declarations are marked "docs only"
// and only used for chpldoc.

/* Returns ``true`` if the type ``from`` is coercible to the type ``to``,
   or if ``isSubtype(from, to)`` would return ``true``. See
   :ref:`Implicit_Conversion_Call`.
 */
pragma "docs only"
proc isCoercible(type from, type to) param {
  return __primitive("is_coercible", from, to);
}

/* Returns ``true`` if the type ``sub`` is a subtype of the type ``sup``.
   See also :ref:`Subtype`.

   In particular, returns ``true`` in any of these cases:

     * ``sub`` is the same type as ``sup``
     * ``sub`` is an instantiation of a generic type ``sup``
     * ``sub`` is a class type inheriting from ``sup``
     * ``sub`` is non-nilable class type and ``sup`` is the nilable version of the
       same class type
   */
pragma "docs only"
proc isSubtype(type sub, type sup) param {
  return __primitive("is_subtype", sup, sub);
}

/* Similar to :proc:`isSubtype` but returns ``false`` if
   ``sub`` and ``sup`` refer to the same type.
   */
pragma "docs only"
proc isProperSubtype(type sub, type sup) param {
  return __primitive("is_proper_subtype", sup, sub);
}

} // module Types
