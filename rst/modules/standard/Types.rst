.. default-domain:: chpl

.. module:: Types
   :synopsis: Functions related to predefined types.

Types
=====
**Usage**

.. code-block:: chapel

   use Types;


Functions related to predefined types.

.. note:: All Chapel programs automatically ``use`` this module by default.
          An explicit ``use`` statement is not necessary.


.. function:: proc isType(e) param

   Returns `true` if the argument is a type. 

.. function:: proc isParam(e) param

   Returns `true` if the argument is a param. 

.. function:: proc isPrimitiveType(type t) param

   
   Returns `true` if the type `t` is a primitive type,
   as defined by the language specification.

.. function:: proc isNumericType(type t) param

   
   Returns `true` if the type `t` is one the following types, of any width:
   `int`, `uint`, `real`, `imag`, `complex`.

.. function:: proc isIntegralType(type t) param

   
   Returns `true` if the type `t` is one the following types, of any width:
   `int`, `uint`.

.. function:: proc isFloatType(type t) param

   
   Returns `true` if the type `t` is one the following types, of any width:
   `real`, `imag`.

.. function:: proc isNothingType(type t) param

   Returns `true` if the type `t` is the `nothing` type. 

.. function:: proc isVoidType(type t) param

   Returns `true` if the type `t` is the `void` type. 

.. function:: proc isBoolType(type t) param

   Returns `true` if the type `t` is a `bool` type, of any width. 

.. function:: proc isIntType(type t) param

   Returns `true` if the type `t` is an `int` type, of any width. 

.. function:: proc isUintType(type t) param

   Returns `true` if the type `t` is a `uint` type, of any width. 

.. function:: proc isEnumType(type t) param

   Returns `true` if the type `t` is an `enum` type. 

.. function:: proc isAbstractEnumType(type t) param

   Returns `true` if the type `t` is an abstract `enum` type (one in which
   not all symbols have associated integer values). 

.. function:: proc isComplexType(type t) param

   Returns `true` if the type `t` is a `complex` type, of any width. 

.. function:: proc isRealType(type t) param

   Returns `true` if the type `t` is a `real` type, of any width. 

.. function:: proc isImagType(type t) param

   Returns `true` if the type `t` is an `imag` type, of any width. 

.. function:: proc isStringType(type t) param

   Returns `true` if the type `t` is the `string` type. 

.. function:: proc isBytesType(type t) param

   Returns `true` if the type `t` is the `bytes` type. 

.. function:: proc isPrimitive(e) param

   
   Returns `true` if the argument is a primitive type,
   as defined by the language specification, or a value of a primitive type.

.. function:: proc isNumeric(e) param

   
   Returns `true` if the argument is one the following types, of any width:
   `int`, `uint`, `real`, `imag`, `complex`, or a value of such a type.

.. function:: proc isIntegral(e) param

   
   Returns `true` if the argument is one the following types, of any width:
   `int`, `uint`, or a value of such a type.

.. function:: proc isFloat(e) param

   
   Returns `true` if the argument is one the following types, of any width:
   `real`, `imag`, or a value of such a type.

.. function:: proc isBool(e) param

   Returns `true` if the argument is a `bool` type or value, of any width. 

.. function:: proc isInt(e) param

   Returns `true` if the argument is an `int` type or value, of any width. 

.. function:: proc isUint(e) param

   Returns `true` if the argument is a `uint` type or value, of any width. 

.. function:: proc isReal(e) param

   Returns `true` if the argument is a `real` type or value, of any width. 

.. function:: proc isImag(e) param

   Returns `true` if the argument is an `imag` type or value, of any width. 

.. function:: proc isComplex(e) param

   Returns `true` if the argument is a `complex` type or value, of any width. 

.. function:: proc isBytes(e) param

   Returns `true` if the argument is a bytes or the `bytes` type. 

.. function:: proc isString(e) param

   Returns `true` if the argument is a string or the `string` type. 

.. function:: proc isEnum(e) param

   Returns `true` if the argument is an `enum` type or value, of any width. 

.. function:: proc isTuple(e) param

   Returns `true` if the argument is a tuple type or value. 

.. function:: proc isHomogeneousTuple(e: _tuple) param

   Returns `true` if the argument is a homogeneous tuple.
   The argument must be a tuple or any type. 

.. function:: proc isClass(e) param

   Returns `true` if the argument is a class type or value
   that is not an ``extern`` class, or when the argument is ``nil``. 

.. function:: proc isGeneric(e) param

   Returns `true` if the argument is a generic type, and `false` otherwise. 

.. function:: proc isOwnedClass(e) param

   Returns `true` if the argument is an ``owned`` class type. 

.. function:: proc isSharedClass(e) param

   Returns `true` if the argument is a ``shared`` class type. 

.. function:: proc isUnmanagedClass(e) param

   Returns `true` if the argument is a ``unmanaged`` class type. 

.. function:: proc isBorrowedClass(e) param

   Returns `true` if the argument is a ``borrowed`` class type. 

.. function:: proc isNilableClass(e) param

   Returns `true` if the argument is a class type that can store ``nil``. 

.. function:: proc isNonNilableClass(e) param

   Returns `true` if the argument is a class type that cannot store ``nil``. 

.. function:: proc isRecord(e) param

   Returns `true` if the argument is a record type or value. 

.. function:: proc isUnion(e) param

   Returns `true` if the argument is a union type or value. 

.. function:: proc isRange(e) param

   Returns `true` if the argument is a range type or value. 

.. function:: proc isDmap(e) param

   Returns `true` if the argument is a domain map or a domain map type. 

.. function:: proc isDomain(e) param

   Returns `true` if the argument is a domain or a domain type. 

.. function:: proc isArray(e) param

   Returns `true` if the argument is an array or an array type. 

.. function:: proc isSync(e) param

   Returns `true` if the argument is a `sync` type or a `sync` variable. 

.. function:: proc isSingle(e) param

   Returns `true` if the argument is a `single` type or a `single` variable. 

.. function:: proc isAtomic(e) param

   Returns `true` if the argument is an `atomic` type or an `atomic` variable.

.. function:: proc isCopyable(e) param

   
   
   Returns ``true`` if the argument is a type or an expression of a type
   that can be copy-initialized and ``false`` otherwise.
   
   Note that even if this function returns ``true``, it might be the case that the
   type only supports copy-initialization from mutable values.
   :record:`~OwnedObject.owned` is an example of a type with that behavior.
   
   See also the specification section :ref:`Copy_Initialization_of_Records`.
   

.. function:: proc isConstCopyable(e) param

   
   
   Returns ``true`` if the argument is a type or an expression of a type
   that can be copy-initialized from a ``const`` value and ``false`` otherwise.
   
   Returns ``false`` for :record:`~OwnedObject.owned` because copy-initialization
   for that type leaves the source argument storing ``nil``.
   
   See also the specification section :ref:`Copy_Initialization_of_Records`.
   

.. function:: proc isAssignable(e) param

   
   
   Returns ``true`` if the argument is a type or expression of a type that
   can be assigned from another value and ``false`` otherwise.
   
   Note that even if this function returns ``true``, it might be the case that the
   type only supports assignment from mutable values.
   :record:`~OwnedObject.owned` is an example of a type with that behavior.
   
   See also the specification section :ref:`Record_Assignment`.
   

.. function:: proc isConstAssignable(e) param

   
   
   Returns ``true`` if the argument is a type or expression of a type that
   can be assigned from a ``const`` value and ``false`` otherwise.
   
   Returns ``false`` for  :record:`~OwnedObject.owned` because assignment
   for that type leaves the source argument storing ``nil``.
   
   See also the specification section :ref:`Record_Assignment`.
   

.. function:: proc isDefaultInitializable(e) param

   
   
   Returns ``true`` if the argument is a type or expression of a type that
   can be default initialized and ``false`` otherwise.
   
   Returns ``false`` for non-nilable class types because these types do not
   have a default value.
   
   See also the specification section :ref:`Default_Values_For_Types`.
   

.. function:: proc toNilableIfClassType(type arg) type

   If the argument is a class type, returns its nilable version like `arg?`.
   Otherwise returns the argument unchanged. 

.. function:: proc numBits(type t) param

   
   Returns the number of bits used to store the values of type `t`.
   This is available for all numeric types and fixed-width `bool` types.
   It is not available for default-width `bool`.

.. function:: proc numBytes(type t) param

   
   Returns the number of bytes used to store the values of type `t`.
   This is available for all numeric types and fixed-width `bool` types.
   It is not available for default-width `bool`.

.. function:: proc min(type t) param

   
   Returns the minimum value the type `t` can store.
   `t` can be one of the following types, of any width:
   `bool`, `int`, `uint`, `real`, `imag`, `complex`.
   When `t` is a `bool` type, it returns `false`.
   When `t` is `real`, `imag`, or `complex` type,
   it is a non-``param`` function.

.. function:: proc max(type t) param

   
   Returns the maximum value the type `t` can store.
   `t` can be one of the following types, of any width:
   `bool`, `int`, `uint`, `real`, `imag`, `complex`.
   When `t` is a `bool` type, it returns `true`.
   When `t` is a `real`, `imag`, or `complex` type,
   it is a non-``param`` function.

.. method:: proc integral.safeCast(type T): T

   
   Returns `this`, cast to the type `T`.
   Generates a run-time error if `this` cannot be represented by `T`,
   for example ``(-1).safeCast(uint)`` or ``256.safeCast(uint(8))``.
   
   This method performs the minimum number of runtime checks.
   For example, when casting from `uint(8)` to `uint(64)`,
   no checks at all will be done.

