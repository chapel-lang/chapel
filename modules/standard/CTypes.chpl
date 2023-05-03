/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

//
// CTypes.chpl
//

/*

Defines C types and related routines to support interoperability.

This module provides access to common C types for the purpose of
calling between Chapel and C, declaring variables using C's types,
etc.  It also contains routines in support of working with C types.

See :ref:`readme-extern`, and :ref:`Chapter-Interoperability` for
additional information about interoperating between Chapel and C (or
other languages).

.. include:: ChapelSysCTypes.rst

*/

module CTypes {
  use HaltWrappers;
  public use ChapelSysCTypes;

  pragma "no doc"
  proc chpl_typeMoveWarning(param name: string, param mod: string,
                            param newmod: string = "CTypes") {
    compilerWarning("type '" + name + "' has moved from '" + mod +
                    "' to '" + newmod + "'; please update your " +
                    "'use'/'import' statements accordingly.", errorDepth=2);
  }

  /* The Chapel type corresponding to the C 'float' type */
  extern type c_float = real(32);

  /* The Chapel type corresponding to the C 'double' type */
  extern type c_double = real(64);

  /* The Chapel type corresponding to the C 'FILE*' type defined in <stdio.h> */
  extern "_cfile" type c_FILE;

  // Former CPtr contents start here

  /*

    A Chapel type alias for ``void*`` in C. Casts from integral types to
    ``c_void_ptr`` as well as casts from ``c_void_ptr`` to integral types are
    supported and behave similarly to those operations in C.

  */
  extern type c_void_ptr = chpl__c_void_ptr;


  /* A Chapel version of a C NULL pointer. */
  inline proc c_nil {
    // TODO: this routine should be deprecated
    return nil;
  }

  /*
     :returns: true if the passed value is a NULL pointer (ie 0).
   */
  inline proc is_c_nil(x):bool {
    // TODO: this routine should be deprecated
    return __primitive("cast", c_void_ptr, x) == c_nil;
  }

  /*

    Represents a local C pointer for the purpose of C integration. This class
    represents the equivalent to a C language pointer. Instances of this class
    support assignment to other instances or nil, == or != comparison with a
    ``c_void_ptr`` or with ``nil``, and casting to another ``c_ptr`` type or to
    the ``c_void_ptr`` type.

    Casting directly to a ``c_ptr`` of another pointee type is supported, but
    will emit a safety warning for casts that can lead to violation of C's
    strict aliasing rule. Casting to a char pointee type or across signedness,
    or through an intermediate cast to ``c_void_ptr``, will not generate a
    warning.

    As with a Chapel class, a ``c_ptr`` can be tested non-nil simply
    by including it in an if statement conditional, like so:

    .. code-block:: chapel

      var x: c_ptr = c_ptrTo(...);
      if x then do writeln("x is not nil");
      if !x then do writeln("x is nil");

    Additionally, a ``c_ptr`` can be output like so:

    .. code-block:: chapel

      var x: c_ptr = c_ptrTo(...);
      writeln(x); // outputs nil or e.g. 0xabc123000000

  */

  pragma "data class"
  pragma "no object"
  pragma "no default functions"
  pragma "no wide class"
  pragma "c_ptr class"
  class c_ptr {
    //   Similar to _ddata from ChapelBase, but differs
    //   from _ddata because it can never be wide.

    /* The type that this pointer points to */
    type eltType;
    /* Retrieve the i'th element (zero based) from a pointer to an array.
      Does the equivalent of ptr[i] in C.
    */
    inline proc this(i: integral) ref {
      return __primitive("array_get", this, i);
    }
    /* Get element pointed to directly by this pointer. If the pointer
      refers to an array, this will return ptr[0].
    */
    inline proc deref() ref {
      return __primitive("array_get", this, 0);
    }
    /* Print this pointer */
    inline proc writeThis(ch) throws {
      (this:c_void_ptr).writeThis(ch);
    }
  }

  /*
    Like ``c_ptr``, but for a pointer to const data. In C, this is equivalent to
    the type `const eltType*`.
  */
  // TODO: avoid redundant c_ptr pragma with c_ptrConst pragma
  pragma "data class"
  pragma "no object"
  pragma "no default functions"
  pragma "no wide class"
  pragma "c_ptr class"
  pragma "c_ptrConst class"
  class c_ptrConst {
    type eltType;
    inline proc this(i: integral) const ref {
      return __primitive("array_get", this, i);
    }
    inline proc deref() const ref {
      return __primitive("array_get", this, 0);
    }
    inline proc writeThis(ch) throws {
      (this:c_void_ptr).writeThis(ch);
    }
  }

  /*
  This class represents a C array with fixed size.  A variable of type c_array
  can coerce to a c_ptr with the same element type.  In that event, the
  pointer will be equivalent to `c_ptrTo(array[0])`.  A c_array behaves
  similarly to a homogeneous tuple except that its indices start at 0 and it is
  guaranteed to be stored in contiguous memory.  A c_array variable has value
  semantics. Declaring one as a function local variable will create the array
  elements in the function's stack. Assigning or copy initializing will result
  in copying the elements (vs resulting in two pointers that refer to the same
  elements).  A `nil` c_array is not representable in Chapel.
  */
  pragma "c_array record"
  pragma "default intent is ref if modified"
  record c_array {
    /* The array element type */
    type eltType;
    /* The fixed number of elements */
    param size;

    proc init(type eltType, param size) {
      this.eltType = eltType;
      this.size = size;
      this.complete();
      var i = 0;
      while i < size {
        // create a default value we'll transfer into the element
        pragma "no auto destroy"
        var default: eltType;
        // this use of primitive works around an order-of-resolution issue.
        ref eltRef = __primitive("array_get", this, i);
        // this is a move, transferring ownership
        __primitive("=", eltRef, default);
        i += 1;
      }
    }

    proc deinit() {
      var i = 0;
      while i < size {
        // this use of primitive works around an order-of-resolution issue.
        chpl__autoDestroy(__primitive("array_get", this, i));
        i += 1;
      }
    }

    /* Retrieve the i'th element (zero based) from the array.
       Does the equivalent of arr[i] in C.
       Includes bounds checking when such checks are enabled.
    */
    inline proc ref this(i: integral) ref : eltType {
      if boundsChecking then
        if i < 0 || i >= size then
          HaltWrappers.boundsCheckHalt("c array index out of bounds " + i:string +
                                       "(indices are 0.." + (size-1):string + ")");

      return __primitive("array_get", this, i);
    }
    pragma "no doc"
    inline proc const ref this(i: integral) const ref : eltType {
      if boundsChecking then
        if i < 0 || i >= size then
          HaltWrappers.boundsCheckHalt("c array index out of bounds " + i:string +
                                       "(indices are 0.." + (size-1):string + ")");

      return __primitive("array_get", this, i);
    }

    /* As with the previous function, returns the i'th element (zero based)
        from the array. This one emits a compilation error if i is out of bounds.
    */
    inline proc ref this(param i: integral) ref : eltType {
      if i < 0 || i >= size then
        compilerError("c array index out of bounds " + i:string +
                      "(indices are 0.." + (size-1):string + ")");

      return __primitive("array_get", this, i);
    }
    pragma "no doc"
    inline proc const ref this(param i: integral) const ref : eltType {
      if i < 0 || i >= size then
        compilerError("c array index out of bounds " + i:string +
                      "(indices are 0.." + (size-1):string + ")");

      return __primitive("array_get", this, i);
    }


    /* Print the elements */
    proc writeThis(ch) throws {
      ch.readWriteLiteral("[");
      var first = true;
      for i in 0..#size {

        ch.write(this(i));

        if i != size-1 then
          ch.readWriteLiteral(", ");
      }
      ch.readWriteLiteral("]");
    }

    proc init=(other: c_array) {
      this.eltType = other.eltType;
      this.size = other.size;
      this.complete();
      for i in 0..#size {
        pragma "no auto destroy"
        var value: eltType = other[i];
        // this is a move, transferring ownership
        __primitive("=", this(i), value);
      }
    }
  }

  /* Copy the elements from one c_array to another.
     Raises an error at compile time if the array sizes or
     element types do not match. */
  operator c_array.=(ref lhs:c_array, rhs:c_array) {
    if lhs.eltType != rhs.eltType then
      compilerError("element type mismatch in c_array assignment");
    if lhs.size != rhs.size then
      compilerError("size mismatch in c_array assignment");

    for i in 0..#lhs.size {
      lhs[i] = rhs[i];
    }
  }
  operator =(ref lhs:c_ptr, ref rhs:c_array) {
    if lhs.eltType != rhs.eltType then
      compilerError("element type mismatch in c_array assignment");
    lhs = c_ptrTo(rhs[0]);
  }

  pragma "no doc"
  inline proc c_void_ptr.writeThis(ch) throws {
    ch.writef("0x%xu", this:c_uintptr);
  }

  pragma "no doc"
  inline operator c_ptr.=(ref lhs:c_ptr, rhs:c_ptr) {
    if lhs.eltType != rhs.eltType then
      compilerError("element type mismatch in c_ptr assignment");
    __primitive("=", lhs, rhs);
  }

  pragma "no doc"
  inline operator c_ptrConst.=(ref lhs:c_ptrConst, rhs:c_ptrConst) {
    if lhs.eltType != rhs.eltType then
      compilerError("element type mismatch in c_ptrConst assignment");
    __primitive("=", lhs, rhs);
  }

  pragma "no doc"
  inline operator c_ptr.=(ref lhs:c_ptr, rhs:_nilType) {
    __primitive("=", lhs, nil);
  }

  pragma "no doc"
  inline operator c_ptrConst.=(ref lhs:c_ptrConst, rhs:_nilType) {
    __primitive("=", lhs, nil);
  }


  pragma "no doc"
  inline operator :(x:c_fn_ptr, type t:c_void_ptr) {
    return __primitive("cast", c_void_ptr, x);
  }

  // Note: we rely from nil to pointer types for ptr = nil, nil:ptr cases

  /* Helper function for determining if casting between two types as pointee
     types violates C strict aliasing rules. Only checks for types that have c_*
     equivalents.
   */
  pragma "no doc"
  inline proc pointeeCastStrictAliasingAllowed(type from, type to) param
      : bool {
    // special checking when either to or from is a pointer
    if (isAnyCPtr(from) || isAnyCPtr(to)) {
      // allow casting to and from void pointer pointee type
      if (from == c_void_ptr || to == c_void_ptr) {
        return true;
      } else if (isAnyCPtr(from) && isAnyCPtr(to)) {
        // if from and to are both pointer types themselves, recurse into their
        // respective pointee types (strip a layer of indirection)
        return pointeeCastStrictAliasingAllowed(from.eltType, to.eltType);
      } else if (from == c_string) {
        // a c_string can be interpreted as a pointer to c_char for this purpose
        return pointeeCastStrictAliasingAllowed(c_char, to.eltType);
      } else if (to == c_string) {
        return pointeeCastStrictAliasingAllowed(from.eltType, c_char);
      }
    }
    // allow identical types
    if (from == to) {
      return true;
    }
    // allow aliasing to any char type
    if (to == int(8) || to == uint(8)) {
      return true;
    }
    // allow types differing only in signedness
    if ((isIntegralType(from) && isIntegralType(to) ||
         isRealType(from) && isRealType(to)) &&
        numBytes(from) == numBytes(to)) {
      return true;
    }
    // otherwise, return false
    return false;
  }

  pragma "no doc"
  inline operator c_ptr.:(x:c_ptr, type t:c_ptr) {
    // emit warning for C strict aliasing violations
    if (!pointeeCastStrictAliasingAllowed(x.eltType, t.eltType)) {
      compilerWarning(
          "This cast from '" + x.type:string + "' to '" + t:string +
          "' casts a c_ptr to a pointer of non-equivalent, non-char " +
          "element type, which can cause undefined behavior.");
    }
    return __primitive("cast", t, x);
  }
  pragma "no doc"
  inline operator c_ptrConst.:(x:c_ptrConst, type t:c_ptrConst) {
    // emit warning for C strict aliasing violations
    if (!pointeeCastStrictAliasingAllowed(x.type, t)) {
      compilerWarning(
          "This cast from '" + x.type:string + "' to '" + t:string +
          "' casts a c_ptrConst to a pointer of non-equivalent, non-char " +
          "element type, which can cause undefined behavior.");
    }
    return __primitive("cast", t, x);
  }
  // Also need const to non-const and vice-versa versions; although coercion
  // makes the casting extraneous, it is needed for strict aliasing warnings
  pragma "no doc"
  inline operator c_ptrConst.:(x:c_ptrConst, type t:c_ptr) {
    if (!pointeeCastStrictAliasingAllowed(x.type, t)) {
      compilerWarning(
          "This cast from '" + x.type:string + "' to '" + t:string +
          "' casts a c_ptrConst to a pointer of non-equivalent, non-char " +
          "element type, which can cause undefined behavior.");
    }
    return __primitive("cast", t, x);
  }
  pragma "no doc"
  inline operator c_ptr.:(x:c_ptr, type t:c_ptrConst) {
    if (!pointeeCastStrictAliasingAllowed(x.type, t)) {
      compilerWarning(
          "This cast from '" + x.type:string + "' to '" + t:string +
          "' casts a c_ptr to a pointer of non-equivalent, non-char " +
          "element type, which can cause undefined behavior.");
    }
    return __primitive("cast", t, x);
  }
  pragma "no doc"
  inline operator :(ref x:c_array, type t:c_ptr(?e)) where x.eltType == e {
    return c_ptrTo(x[0]);
  }
  pragma "no doc"
  inline operator :(const ref x:c_array, type t:c_ptrConst(?e))
      where x.eltType == e {
    return c_ptrTo(x[0]):c_ptrConst(e);
  }
  pragma "no doc"
  inline operator :(ref x:c_array, type t:c_void_ptr) {
    return c_ptrTo(x[0]):c_void_ptr;
  }
  pragma "no doc"
  inline operator :(x:c_ptr, type t:c_void_ptr) {
    return __primitive("cast", t, x);
  }
  pragma "no doc"
  inline operator :(x:c_ptrConst, type t:c_void_ptr) {
    return __primitive("cast", t, x);
  }
  pragma "no doc"
  inline operator :(x:c_void_ptr, type t:c_ptr) {
    return __primitive("cast", t, x);
  }
  pragma "no doc"
  inline operator :(x:c_void_ptr, type t:c_ptrConst) {
    return __primitive("cast", t, x);
  }
  pragma "no doc"
  inline operator c_void_ptr.:(x:c_void_ptr, type t:string) {
    try! {
      return createStringWithOwnedBuffer(__primitive("ref to string", x));
    }
  }
  pragma "no doc"
  inline operator c_ptr.:(x:c_ptr, type t:string) {
    try! {
      return createStringWithOwnedBuffer(__primitive("ref to string", x));
    }
  }
  pragma "no doc"
  inline operator c_ptrConst.:(x:c_ptrConst, type t:string) {
    try! {
      return createStringWithOwnedBuffer(__primitive("ref to string", x));
    }
  }
  pragma "last resort"
  pragma "no doc"
  inline operator c_void_ptr.:(x:c_void_ptr, type t:_anyManagementAnyNilable) {
    if isUnmanagedClass(t) || isBorrowedClass(t) {
      compilerError("invalid cast from c_void_ptr to "+ t:string +
                    " - cast to "+ _to_nilable(t):string +" instead");
    } else {
      compilerError("invalid cast from c_void_ptr to managed type " + t:string);
    }
    return __primitive("cast", t, x);
  }

  pragma "no doc"
  inline operator c_void_ptr.:(x:c_void_ptr, type t:unmanaged class?) {
    return __primitive("cast", t, x);
  }
  pragma "no doc"
  inline operator c_void_ptr.:(x:c_void_ptr, type t:borrowed class?) {
    return __primitive("cast", t, x);
  }

  pragma "no doc"
  inline operator c_void_ptr.:(x:borrowed, type t:c_void_ptr) {
    return __primitive("cast", t, x);
  }
  pragma "no doc"
  inline operator c_void_ptr.:(x:unmanaged, type t:c_void_ptr) {
    return __primitive("cast", t, x);
  }

  pragma "no doc"
  inline operator c_ptr.:(x:c_ptr, type t:_ddata) where t.eltType == x.eltType {
    return __primitive("cast", t, x);
  }
  pragma "no doc"
  inline operator c_void_ptr.:(x:c_void_ptr, type t:_ddata) {
    return __primitive("cast", t, x);
  }
  pragma "no doc"
  inline operator c_void_ptr.:(x:_ddata, type t:c_void_ptr) {
    return __primitive("cast", t, x);
  }

  // casts from c pointer to c_intptr / c_uintptr
  pragma "no doc"
  inline operator :(x:c_void_ptr, type t:c_intptr) do
    return __primitive("cast", t, x);
  pragma "no doc"
  inline operator :(x:c_void_ptr, type t:c_uintptr) do
    return __primitive("cast", t, x);

  pragma "no doc"
  inline operator :(x:c_ptr, type t:c_intptr) do
    return __primitive("cast", t, x);
  pragma "no doc"
  inline operator :(x:c_ptr, type t:c_uintptr) do
    return __primitive("cast", t, x);
  pragma "no doc"
  inline operator :(x:c_ptrConst, type t:c_intptr) do
    return __primitive("cast", t, x);
  pragma "no doc"
  inline operator :(x:c_ptrConst, type t:c_uintptr) do
    return __primitive("cast", t, x);


  // casts from c pointer to int / uint
  // note that these are only used if c_intptr != int / c_uintptr != uint
  pragma "no doc"
  inline operator c_void_ptr.:(x:c_void_ptr, type t:int) where c_uintptr != int do
    return __primitive("cast", t, x);
  pragma "no doc"
  inline operator c_void_ptr.:(x:c_void_ptr, type t:uint) where c_uintptr != uint do
    return __primitive("cast", t, x);

  pragma "no doc"
  inline operator c_ptr.:(x:c_ptr, type t:int) where c_intptr != int do
    return __primitive("cast", t, x);
  pragma "no doc"
  inline operator c_ptr.:(x:c_ptr, type t:uint) where c_uintptr != uint do
    return __primitive("cast", t, x);
  pragma "no doc"
  inline operator c_ptrConst.:(x:c_ptrConst, type t:int) where c_intptr != int do
    return __primitive("cast", t, x);
  pragma "no doc"
  inline operator c_ptrConst.:(x:c_ptrConst, type t:uint) where c_uintptr != uint do
    return __primitive("cast", t, x);

  // casts from c_intptr / c_uintptr to c_void_ptr
  pragma "no doc"
  inline operator :(x:c_intptr, type t:c_void_ptr) do
    return __primitive("cast", t, x);
  pragma "no doc"
  inline operator :(x:c_uintptr, type t:c_void_ptr) do
    return __primitive("cast", t, x);

  // casts from int / uint to c_void_ptr
  // note that these are only used if c_intptr != int / c_uintptr != uint
  pragma "no doc"
  inline operator c_void_ptr.:(x:int, type t:c_void_ptr) where c_intptr != int do
    return __primitive("cast", t, x);
  pragma "no doc"
  inline operator c_void_ptr.:(x:uint, type t:c_void_ptr) where c_uintptr != uint do
    return __primitive("cast", t, x);


  pragma "no doc"
  inline operator c_ptr.==(a: c_ptr, b: c_ptr) where a.eltType == b.eltType {
    return __primitive("ptr_eq", a, b);
  }
  pragma "no doc"
  inline operator c_ptrConst.==(a: c_ptrConst, b: c_ptrConst)
      where a.eltType == b.eltType {
    return __primitive("ptr_eq", a, b);
  }

  pragma "no doc"
  inline operator ==(a: c_ptr, b: c_void_ptr) {
    return __primitive("ptr_eq", a, b);
  }
  pragma "no doc"
  inline operator ==(a: c_void_ptr, b: c_ptr) {
    return __primitive("ptr_eq", a, b);
  }
  pragma "no doc"
  inline operator ==(a: c_ptrConst, b: c_void_ptr) {
    return __primitive("ptr_eq", a, b);
  }
  pragma "no doc"
  inline operator ==(a: c_void_ptr, b: c_ptrConst) {
    return __primitive("ptr_eq", a, b);
  }
  // Don't need _nilType versions -
  // Rely on coercions from nil to c_ptr / c_void_ptr

  pragma "no doc"
  inline operator c_ptr.!=(a: c_ptr, b: c_ptr) where a.eltType == b.eltType {
    return __primitive("ptr_neq", a, b);
  }
  pragma "no doc"
  inline operator c_ptrConst.!=(a: c_ptrConst, b: c_ptrConst)
      where a.eltType == b.eltType {
    return __primitive("ptr_neq", a, b);
  }
  pragma "no doc"
  inline operator !=(a: c_ptr, b: c_void_ptr) {
    return __primitive("ptr_neq", a, b);
  }
  pragma "no doc"
  inline operator !=(a: c_void_ptr, b: c_ptr) {
    return __primitive("ptr_neq", a, b);
  }
  pragma "no doc"
  inline operator !=(a: c_ptrConst, b: c_void_ptr) {
    return __primitive("ptr_neq", a, b);
  }
  pragma "no doc"
  inline operator !=(a: c_void_ptr, b: c_ptrConst) {
    return __primitive("ptr_neq", a, b);
  }

  pragma "no doc"
  inline operator c_ptr.!(x: c_ptr) do return x == c_nil;

  pragma "no doc"
  inline operator c_ptr.+(a: c_ptr, b: integral) do return __primitive("+", a, b);

  pragma "no doc"
  inline operator c_ptr.-(a: c_ptr, b: integral) do return __primitive("-", a, b);

  pragma "no doc"
  inline operator c_ptrConst.!(x: c_ptrConst) do return x == c_nil;

  pragma "no doc"
  inline operator c_ptrConst.+(a: c_ptrConst, b: integral) do
    return __primitive("+", a, b);

  pragma "no doc"
  inline operator c_ptrConst.-(a: c_ptrConst, b: integral) do
    return __primitive("-", a, b);

  pragma "no doc"
  inline operator c_ptr.-(a: c_ptr(?t), b: c_ptr(t)):c_ptrdiff {
    return c_pointer_diff(a, b, c_sizeof(a.eltType):c_ptrdiff);
  }

  pragma "no doc"
  inline operator c_ptrConst.-(a: c_ptrConst(?t), b: c_ptrConst(t)):c_ptrdiff {
    return c_pointer_diff(a, b, c_sizeof(a.eltType):c_ptrdiff);
  }

  pragma "no doc"
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc c_pointer_return(ref x:?t):c_ptr(t);
  pragma "no doc"
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc c_pointer_return_const(const ref x:?t):c_ptrConst(t);
  pragma "no doc"
  pragma "fn synchronization free"
  extern proc c_pointer_diff(a:c_void_ptr, b:c_void_ptr,
                             eltSize:c_ptrdiff):c_ptrdiff;

  /*
    Returns a :type:`c_ptr` to the elements of a non-distributed
    Chapel rectangular array.  Note that the existence of this
    :type:`c_ptr` has no impact on the lifetime of the array.  The
    returned pointer will be invalid if the original array is freed or
    even reallocated. Domain assignment could make this :type:`c_ptr`
    invalid.

    :arg arr: the array for which a pointer should be returned
    :returns: a pointer to the array's elements
  */
  inline proc c_ptrTo(arr: []): c_ptr(arr.eltType) {
    if (!arr.isRectangular() || !arr.domain.dist._value.dsiIsLayout()) then
      compilerError("Only single-locale rectangular arrays support c_ptrTo() at present");

    if (arr._value.locale != here) then
      halt(
          "c_ptrTo() can only be applied to an array from the locale on " +
          "which it lives (array is on locale " + arr._value.locale.id:string +
          ", call was made on locale " + here.id:string + ")");

    if boundsChecking {
      if (arr.size == 0) then
        halt("Can't create a C pointer for an array with 0 elements.");
    }

    return c_pointer_return(arr[arr.domain.low]);
  }
  /*
   Like c_ptrTo for arrays, but returns a :type:`c_ptrConst` which disallows
   direct modification of the pointee.
   */
  inline proc c_ptrToConst(const arr: []): c_ptrConst(arr.eltType) {
    if (!arr.isRectangular() || !arr.domain.dist._value.dsiIsLayout()) then
      compilerError("Only single-locale rectangular arrays support c_ptrToConst() at present");

    if (arr._value.locale != here) then
      halt(
          "c_ptrToConst() can only be applied to an array from the locale on " +
          "which it lives (array is on locale " + arr._value.locale.id:string +
          ", call was made on locale " + here.id:string + ")");

    if boundsChecking {
      if (arr.size == 0) then
        halt("Can't create a C pointer for an array with 0 elements.");
    }

    return c_pointer_return_const(arr[arr.domain.low]);
  }

  /*
    Returns a :type:`c_ptr` to the underlying buffer of a :type:`~String.string`

    Note that the existence of this ``c_ptr`` has no impact on the lifetime of
    the ``string``.  The returned pointer will be invalid if the ``string`` is
    freed or even reallocated.

    Halts if the ``string`` is empty and bounds checking is enabled.
  */
  inline proc c_ptrTo(ref s: string): c_ptr(c_uchar) {
    if boundsChecking {
      if (s.buffLen == 0) then
        halt("Can't create a C pointer for an empty string.");
    }
    return c_pointer_return(s.buff[0]);
  }

  /*
   Like ``c_ptrTo`` for :type:`~String.string`, but returns a :type:`c_ptrConst`
   which disallows direct modification of the pointee.
   */
  inline proc c_ptrToConst(const ref s: string): c_ptrConst(c_uchar) {
    if boundsChecking {
      if (s.buffLen == 0) then
        halt("Can't create a C pointer for an empty string.");
    }
    return c_pointer_return_const(s.buff[0]);
  }

  /*
    Returns a :type:`c_ptr` to the underlying buffer of a :type:`~Bytes.bytes`

    Note that the existence of this ``c_ptr`` has no impact on the lifetime of
    the ``bytes``.  The returned pointer will be invalid if the ``bytes`` is
    freed or even reallocated.

    Halts if the ``bytes`` is empty and bounds checking is enabled.
  */
  inline proc c_ptrTo(ref b: bytes): c_ptr(c_uchar) {
    if boundsChecking {
      if (b.buffLen == 0) then
        halt("Can't create a C pointer for an empty bytes.");
    }
    return c_pointer_return(b.buff[0]);
  }

  /*
   Like ``c_ptrTo`` for :type:`~Bytes.bytes`, but returns a :type:`c_ptrConst`
   which disallows direct modification of the pointee.
   */
  inline proc c_ptrToConst(const ref b: bytes): c_ptrConst(c_uchar) {
    if boundsChecking {
      if (b.buffLen == 0) then
        halt("Can't create a C pointer for an empty bytes.");
    }
    return c_pointer_return_const(b.buff[0]);
  }

  /*
    Returns a :type:`c_ptr` to any Chapel object.
    Note that the existence of the :type:`c_ptr` has no impact of the lifetime
    of the object. In many cases the object will be stack allocated and
    could go out of scope even if this :type:`c_ptr` remains.

    :arg x: the by-reference argument to get a pointer to. Domains are not
            supported, and will cause a compiler error. Records, class
            instances, integral, real, imag, and complex types are supported.
            For arrays, strings, or bytes, separate overloads should be used.
    :returns: a pointer to the argument passed by reference

  */
  inline proc c_ptrTo(ref x:?t):c_ptr(t) {
    return c_addrOf(x);
  }

  /*
    Like c_ptrTo, but returns a :type:`c_ptrConst` which disallows direct
    modification of the pointee.
  */
  inline proc c_ptrToConst(const ref x:?t): c_ptrConst(t) {
    return c_addrOfConst(x);
  }

  @chpldoc.nodoc
  inline proc c_ptrTo(x: c_fn_ptr) {
    return x;
  }

  /*
    Returns a :type:`c_ptr` to the address of an array.

    This is distinct from :func:`c_ptrTo` in that it returns a pointer to
    the array object itself, rather than to the first element of the array's
    buffer.

    Note that the existence of this :type:`c_ptr` has no impact on the lifetime
    of the array. The returned pointer will be invalid if the array is freed.
  */
  inline proc c_addrOf(arr: []) {
    if (!arr.isRectangular() || !arr.domain.dist._value.dsiIsLayout()) then
      compilerError("Only single-locale rectangular arrays support c_addrOf() at present");

    if (arr._value.locale != here) then
      halt(
          "c_addrOf() can only be applied to an array from the locale on " +
          "which it lives (array is on locale " + arr._value.locale.id:string +
          ", call was made on locale " + here.id:string + ")");

    return c_pointer_return(arr);
  }

  /*
   Like c_addrOf for arrays, but returns a :type:`c_ptrConst` which disallows
   direct modification of the pointee.
  */
  inline proc c_addrOfConst(arr: []) {
    if (!arr.isRectangular() || !arr.domain.dist._value.dsiIsLayout()) then
      compilerError("Only single-locale rectangular arrays support c_addrOfConst() at present");

    if (arr._value.locale != here) then
      halt(
          "c_addrOfConst() can only be applied to an array from the locale on " +
          "which it lives (array is on locale " + arr._value.locale.id:string +
          ", call was made on locale " + here.id:string + ")");

    return c_pointer_return_const(arr);
  }

  /*
    Returns a :type:`c_ptr` to the address of any chapel object.

    Note that the behavior of this procedure is identical to :func:`c_ptrTo`
    for scalar types. It only differs for arrays, strings, and bytes.
  */
  inline proc c_addrOf(ref x: ?t): c_ptr(t) {
    if isDomainType(t) then
      compilerError("c_addrOf domain type not supported", 2);
    return c_pointer_return(x);
  }

  /*
    Like c_addrOf, but returns a :type:`c_ptrConst` which disallows direct
    modification of the pointee.
  */
  inline proc c_addrOfConst(const ref x: ?t): c_ptrConst(t) {
    if isDomainType(t) then
      compilerError("c_addrOfConst domain type not supported", 2);
    return c_pointer_return_const(x);
  }

  @chpldoc.nodoc
  inline proc c_addrOf(x: c_fn_ptr) {
    return x;
  }

  // Offset the CHPL_RT_MD constant in order to preserve the value through
  // calls to chpl_here_alloc. See comments on offset_STR_* in String.chpl
  // for more.
  private proc offset_ARRAY_ELEMENTS {
    extern const CHPL_RT_MD_ARRAY_ELEMENTS:chpl_mem_descInt_t;
    pragma "fn synchronization free"
    extern proc chpl_memhook_md_num(): chpl_mem_descInt_t;
    return CHPL_RT_MD_ARRAY_ELEMENTS - chpl_memhook_md_num();
  }

  pragma "last resort"
  @deprecated(notes="c_sizeof with argument name 'x' is deprecated; please use c_sizeof(type t) instead")
  inline proc c_sizeof(type x): c_size_t {
    return c_sizeof(x);
  }

  /*
    Return the size in bytes of a type, as with the C ``sizeof`` built-in.

    .. warning::

      This method is intended for C interoperability. To enhance
      flexibility, it is possible to request the sizes of Chapel types.
      However, be aware:

         * Chapel types are not necessarily stored in contiguous memory
         * Behavior of ``c_sizeof`` with Chapel types may change
         * Behavior given a Chapel class type is not well-defined
   */
  inline proc c_sizeof(type t): c_size_t {
    pragma "fn synchronization free"
    extern proc sizeof(type t): c_size_t;
    return sizeof(t);
  }

  /*
    Return the offset of a field in a record.

    .. warning::

      This method is intended for C interoperability.  To enhance flexibility,
      it is possible to request the offset of elements within a Chapel record.
      However, be aware:

      * Chapel types are not necessary stored in contiguous memory
      * Behavior of ``c_offsetof`` may change
      * Behavior given a Chapel class type field is not well-defined
   */
  proc c_offsetof(type t, param fieldname : string): c_size_t where isRecordType(t) {
    use Reflection;
    pragma "no auto destroy"
    pragma "no init"
    var x: t;

    return c_ptrTo(getFieldRef(x, fieldname)):c_size_t - c_ptrTo(x):c_size_t;
  }

  pragma "no doc"
  proc c_offsetof(type t, param fieldname: string) where !isRecordType(t) {
    compilerError("Cannot call c_offsetof on type that is not a record");
  }

  /*
    Allocate memory and initialize all bits to 0. Note that this simply zeros
    memory, it does not call Chapel initializers (it is meant for primitive
    types and C interoperability only.) This memory should eventually be freed
    with :proc:`c_free`.

    :arg eltType: the type of the elements to allocate
    :arg size: the number of elements to allocate space for
    :returns: a c_ptr(eltType) to allocated memory
    */
  inline proc c_calloc(type eltType, size: integral) : c_ptr(eltType) {
    const alloc_size = size.safeCast(c_size_t) * c_sizeof(eltType);
    return chpl_here_calloc(alloc_size, 1, offset_ARRAY_ELEMENTS):c_ptr(eltType);
  }

  /*
    Allocate memory that is not initialized. This memory should eventually be
    freed with :proc:`c_free`.

    :arg eltType: the type of the elements to allocate
    :arg size: the number of elements to allocate space for
    :returns: a c_ptr(eltType) to allocated memory
    */
  inline proc c_malloc(type eltType, size: integral) : c_ptr(eltType) {
    const alloc_size = size.safeCast(c_size_t) * c_sizeof(eltType);
    return chpl_here_alloc(alloc_size, offset_ARRAY_ELEMENTS):c_ptr(eltType);
  }

  /*
    Allocate aligned memory that is not initialized. This memory
    should be eventually freed with :proc:`c_free`.

    This function is intended to behave similarly to the C17
    function aligned_alloc.

    :arg eltType: the type of the elements to allocate
    :arg alignment: the memory alignment of the allocation
                    which must be a power of two and a multiple
                    of ``c_sizeof(c_void_ptr)``.
    :arg size: the number of elements to allocate space for
    :returns: a ``c_ptr(eltType)`` to allocated memory
    */
  inline proc c_aligned_alloc(type eltType,
                              alignment : integral,
                              size: integral) : c_ptr(eltType) {
    // check alignment, size restriction
    if boundsChecking {
      var one:c_size_t = 1;
      // Round the alignment up to the nearest power of 2
      var aln = alignment.safeCast(c_size_t);
      if aln == 0 then
        halt("c_aligned_alloc called with alignment of 0");
      var p = log2(aln); // power of 2 rounded down
      // compute alignment rounded up
      if (one << p) < aln then
        p += 1;
      assert(aln <= (one << p));
      if aln != (one << p) then
        halt("c_aligned_alloc called with non-power-of-2 alignment ", aln);
      if alignment < c_sizeof(c_void_ptr) then
        halt("c_aligned_alloc called with alignment smaller than pointer size");
    }

    const alloc_size = size.safeCast(c_size_t) * c_sizeof(eltType);
    return chpl_here_aligned_alloc(alignment.safeCast(c_size_t),
                                   alloc_size,
                                   offset_ARRAY_ELEMENTS):c_ptr(eltType);
  }

  /* Free memory that was allocated with :proc:`c_calloc` or :proc:`c_malloc`.

    :arg data: the c_ptr to memory that was allocated. Note that both
               `c_ptr(t)` and `c_void_ptr` can be passed to this argument.
    */
  inline proc c_free(data: c_void_ptr) {
    chpl_here_free(data);
  }

  pragma "no doc"
  proc isAnyCPtr(type t:c_ptr) param do return true;
  pragma "no doc"
  proc isAnyCPtr(type t:c_ptrConst) param do return true;
  pragma "no doc"
  proc isAnyCPtr(type t:c_void_ptr) param do return true;
  /*
     Returns true if t is a c_ptr, c_ptrConst, or c_void_ptr type.
   */
  proc isAnyCPtr(type t) param do return false;

  /*
    Copies n potentially overlapping bytes from memory area src to memory
    area dest.

    This is a simple wrapper over the C ``memmove()`` function.

    :arg dest: the destination memory area to copy to
    :arg src: the source memory area to copy from
    :arg n: the number of bytes from src to copy to dest
   */
  pragma "fn synchronization free"
  inline proc c_memmove(dest:c_void_ptr, const src:c_void_ptr, n: integral) {
    pragma "fn synchronization free"
    extern proc memmove(dest: c_void_ptr, const src: c_void_ptr, n: c_size_t);
    memmove(dest, src, n.safeCast(c_size_t));
  }

  /*
    Copies n non-overlapping bytes from memory area src to memory
    area dest. Use :proc:`c_memmove` if memory areas do overlap.

    This is a simple wrapper over the C memcpy() function.

    :arg dest: the destination memory area to copy to
    :arg src: the source memory area to copy from
    :arg n: the number of bytes from src to copy to dest
   */
  pragma "fn synchronization free"
  inline proc c_memcpy(dest:c_void_ptr, const src:c_void_ptr, n: integral) {
    pragma "fn synchronization free"
    extern proc memcpy (dest: c_void_ptr, const src: c_void_ptr, n: c_size_t);
    memcpy(dest, src, n.safeCast(c_size_t));
  }

  /*
    Compares the first n bytes of memory areas s1 and s2

    This is a simple wrapper over the C ``memcmp()`` function.

    :returns: returns an integer less than, equal to, or greater than zero if
              the first n bytes of s1 are found, respectively, to be less than,
              to match, or be greater than the first n bytes of s2.
   */
  pragma "fn synchronization free"
  inline proc c_memcmp(const s1:c_void_ptr, const s2:c_void_ptr, n: integral) {
    pragma "fn synchronization free"
    extern proc memcmp(const s1: c_void_ptr, const s2: c_void_ptr, n: c_size_t) : c_int;
    return memcmp(s1, s2, n.safeCast(c_size_t)).safeCast(int);
  }

  /*
    Fill bytes of memory with a particular byte value.

    This is a simple wrapper over the C ``memset()`` function.

    :arg s: the destination memory area to fill
    :arg c: the byte value to use
    :arg n: the number of bytes of s to fill

    :returns: s
   */
  pragma "fn synchronization free"
  inline proc c_memset(s:c_void_ptr, c:integral, n: integral) {
    pragma "fn synchronization free"
    extern proc memset(s: c_void_ptr, c: c_int, n: c_size_t) : c_void_ptr;
    memset(s, c.safeCast(c_int), n.safeCast(c_size_t));
    return s;
  }
}
