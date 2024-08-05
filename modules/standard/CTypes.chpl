/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

  /* The Chapel type corresponding to the C 'float' type */
  extern type c_float = real(32);

  /* The Chapel type corresponding to the C 'double' type */
  extern type c_double = real(64);

  @chpldoc.nodoc
  @deprecated("'cFileTypeHasPointer' is deprecated and no longer affects the behavior of the 'c_FILE' type. A 'FILE*' should be represented by 'c_ptr(c_FILE)'")
  config param cFileTypeHasPointer = false;

  /*
    Chapel type alias for a C ``FILE``

    A ``FILE*`` can be represented with ``c_ptr(c_FILE)``
  */
  extern "_cfiletype" type c_FILE;

  /*
    A Chapel type alias for ``void*`` in C. Casts from integral types to
    ``c_void_ptr`` as well as casts from ``c_void_ptr`` to integral types are
    supported and behave similarly to those operations in C.

  */
  pragma "last resort"
  @deprecated(notes="c_void_ptr is deprecated, use 'c_ptr(void)' instead.")
  type c_void_ptr = c_ptr(void);

  /*

    Represents a local C pointer for the purpose of C integration. This type
    represents the equivalent to a C language pointer ``eltType*``. Instances of
    this type support assignment to other instances or ``nil``, ``==`` or ``!=``
    comparison with ``nil``, and casting to another ``c_ptr`` type.

    ``c_ptr(void)`` represents an opaque pointer with special functionality,
    corresponding to ``void*`` in C. Casts from integral types to
    ``c_ptr(void)`` as well as casts from ``c_ptr(void)`` to integral types are
    supported and behave similarly to those operations in C. Casting a
    ``c_ptr(void)`` to or from a ``c_ptr(t)`` of any pointee type is allowed.

    Casting directly to a ``c_ptr`` of another pointee type is supported, but
    will emit a safety warning for casts that can lead to violation of C's
    strict aliasing rule. Casting to a char pointee type or across signedness,
    or through an intermediate cast to ``c_ptr(void)``, will not generate a
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
  class c_ptr : writeSerializable {
    //   Similar to _ddata from ChapelBase, but differs
    //   from _ddata because it can never be wide.

    /*
       The type that this pointer points to, which can be queried like so:

       .. code-block:: chapel

         var x: c_ptr = c_ptrTo(...);
         if x.eltType == c_int then do writeln("x is an int pointer");
    */
    type eltType;
    /* Retrieve the i'th element (zero based) from a pointer to an array.
      Does the equivalent of ptr[i] in C.
    */
    inline proc this(i: integral) ref {
      if (this.eltType == void) {
        compilerError("Cannot dereference a void pointer; cast to a " +
                      "non-opaque pointee type first.");
      }
      return __primitive("array_get", this, i);
    }
    /* Get element pointed to directly by this pointer. If the pointer
      refers to an array, this will return ptr[0].
    */
    inline proc deref() ref {
      if (this.eltType == void) {
        compilerError("Cannot dereference a void pointer; cast to a " +
                      "non-opaque pointee type first.");
      }
      return __primitive("array_get", this, 0);
    }
    /* Print this pointer */
    inline proc serialize(writer, ref serializer) throws {
      (this:c_ptr(void)).serialize(writer, serializer);
    }
  }

  // TODO: avoid redundant c_ptr pragma with c_ptrConst pragma
  /*
    Like :type:`c_ptr`, but for a pointer to const data. In C, this is
    equivalent to the type `const eltType*`.
  */
  pragma "data class"
  pragma "no object"
  pragma "no default functions"
  pragma "no wide class"
  pragma "c_ptr class"
  pragma "c_ptrConst class"
  class c_ptrConst : writeSerializable {
    /*
       The type that this pointer points to, which can be queried like so:

       .. code-block:: chapel

         var x: c_ptrConst = c_ptrToConst(...);
         if x.eltType == c_int then do writeln("x is a const int pointer");
    */
    type eltType;
    /* Retrieve the i'th element (zero based) from a pointer to an array.
       Does the equivalent of ptr[i] in C.
       Provides a ``const ref`` which cannot be used to modify the element.
    */
    inline proc this(i: integral) const ref {
      if (this.eltType == void) {
        compilerError("Cannot dereference a void pointer; cast to a " +
                      "non-opaque pointee type first.");
      }
      return __primitive("array_get", this, i);
    }
    /* Get element pointed to directly by this pointer. If the pointer
       refers to an array, this will return ptr[0].
       Provides a ``const ref`` which cannot be used to modify the element.
    */
    inline proc deref() const ref {
      if (this.eltType == void) {
        compilerError("Cannot dereference a void pointer; cast to a " +
                      "non-opaque pointee type first.");
      }
      return __primitive("array_get", this, 0);
    }
    /* Print this pointer */
    inline proc serialize(writer, ref serializer) throws {
      (this:c_ptr(void)).serialize(writer, serializer);
    }
  }

  /*
  This type represents a C array with fixed size.  A variable of type
  ``c_array`` can coerce to a ``c_ptr`` with the same element type.  In that
  event, the pointer will be equivalent to ``c_ptrTo(array[0])``.  A ``c_array``
  behaves similarly to a homogeneous tuple except that its indices start at 0
  and it is guaranteed to be stored in contiguous memory.  A ``c_array``
  variable has value semantics. Declaring one as a function-local variable will
  create the array elements in the function's stack. Assigning or copy
  initializing will result in copying the elements (vs resulting in two pointers
  that refer to the same elements).  A ``nil`` ``c_array`` is not representable
  in Chapel.
  */
  pragma "c_array record"
  pragma "default intent is ref if modified"
  record c_array : writeSerializable {
    /*
       The array element type, which can be queried like so:

       .. code-block:: chapel

         var x: c_array = c_ptrToConst(...);
         if x.eltType == c_int then do writeln("x is an array of ints");
    */
    type eltType;
    /*
       The fixed number of elements, which can be queried like so:

       .. code-block:: chapel

         var x: c_array = c_ptrToConst(...);
         writeln("x has ", x.size, " elements.");
    */
    param size;

    /*
      Default-initializes a :type:`c_array`, where each element gets the default value of ``eltType``.
    */
    proc init(type eltType, param size) {
      if eltType == void then
        compilerError("c_array element type cannot be 'void'");
      if eltType == nothing then
        compilerError("c_array element type cannot be 'nothing'");
      this.eltType = eltType;
      this.size = size;
      init this;
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

    @chpldoc.nodoc
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
    @chpldoc.nodoc
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
    @chpldoc.nodoc
    inline proc const ref this(param i: integral) const ref : eltType {
      if i < 0 || i >= size then
        compilerError("c array index out of bounds " + i:string +
                      "(indices are 0.." + (size-1):string + ")");

      return __primitive("array_get", this, i);
    }

    /* Print the elements */
    proc const serialize(writer, ref serializer) throws {
      writer.writeLiteral("[");
      var first = true;
      for i in 0..#size {

        writer.write(this(i));

        if i != size-1 then
          writer.writeLiteral(", ");
      }
      writer.writeLiteral("]");
    }

    /*
      Moves the elements from ``other`` to ``this``.
    */
    proc init=(other: c_array) {
      this.eltType = other.eltType;
      this.size = other.size;
      init this;
      for i in 0..#size {
        pragma "no auto destroy"
        var value: eltType = other[i];
        // this is a move, transferring ownership
        __primitive("=", this(i), value);
      }
    }
  }

  /* Copy the elements from one :type:`c_array` to another.
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

  /*
    Get a pointer to the first element in ``rhs``, essentially decaying the :type:`c_array` to a :type:`c_ptr`.
  */
  operator =(ref lhs:c_ptr, ref rhs:c_array) {
    if lhs.eltType != rhs.eltType && lhs.eltType != void then
      compilerError("element type mismatch in c_array assignment");
    lhs = c_ptrTo(rhs[0]);
  }

  // This type alias is a hack to enable defining serialize on c_ptr(void)
  // specifically, which is necessary to enable serialize on things that
  // implicitly convert to c_ptr(void).
  @chpldoc.nodoc
  type writable_c_ptr_void = c_ptr(void);
  @chpldoc.nodoc
  inline proc writable_c_ptr_void.serialize(writer, ref serializer) throws {
    writer.writef("0x%xu", this:c_uintptr);
  }

  @chpldoc.nodoc
  inline operator c_ptr.=(ref lhs:c_ptr, rhs:c_ptr) {
    if lhs.eltType != rhs.eltType
       && lhs.eltType != void && rhs.eltType != void then
      compilerError("element type mismatch in c_ptr assignment");
    __primitive("=", lhs, rhs);
  }
  pragma "last resort"
  @chpldoc.nodoc
  // specialization needed to assign a raw_c_void_ptr into a c_ptr(void)
  inline operator c_ptr.=(ref lhs:c_ptr(void), rhs:c_ptr(void)) {
    __primitive("=", lhs, rhs);
  }

  @chpldoc.nodoc
  inline operator c_ptrConst.=(ref lhs:c_ptrConst, rhs:c_ptrConst) {
    if lhs.eltType != rhs.eltType
       && lhs.eltType != void && rhs.eltType != void then
      compilerError("element type mismatch in c_ptrConst assignment");
    __primitive("=", lhs, rhs);
  }

  @chpldoc.nodoc
  inline operator c_ptr.=(ref lhs:c_ptr, rhs:_nilType) {
    __primitive("=", lhs, nil);
  }

  @chpldoc.nodoc
  inline operator c_ptrConst.=(ref lhs:c_ptrConst, rhs:_nilType) {
    __primitive("=", lhs, nil);
  }


  @chpldoc.nodoc
  inline operator :(x:c_fn_ptr, type t:c_ptr(void)) {
    return __primitive("cast", c_ptr(void), x);
  }

  // Enable fn_ptr == nil syntax for checking whether a function pointer is NULL
  @chpldoc.nodoc
  inline operator ==(a: c_fn_ptr, b: _nilType) {
    return a:c_ptr(void) == b;
  }
  @chpldoc.nodoc
  inline operator ==(a: _nilType, b: c_fn_ptr) {
    return b == a;
  }
  @chpldoc.nodoc
  inline operator !=(a: c_fn_ptr, b: _nilType) {
    return !(a == b);
  }
  @chpldoc.nodoc
  inline operator !=(a: _nilType, b: c_fn_ptr) {
    return b != a;
  }

  // Note: we rely from nil to pointer types for ptr = nil, nil:ptr cases

  /* Helper function for determining if casting between two types as pointee
     types violates C strict aliasing rules. Only checks for types that have c_*
     equivalents.
   */
  @chpldoc.nodoc
  inline proc pointeeCastStrictAliasingAllowed(type from, type to) param
      : bool {
    // special checking when either to or from is a pointer
    if (isAnyCPtr(from) || isAnyCPtr(to)) {
      if (isAnyCPtr(from) && isAnyCPtr(to)) {
        // if from and to are both pointer types themselves, recurse into their
        // respective pointee types (strip a layer of indirection)
        return pointeeCastStrictAliasingAllowed(from.eltType, to.eltType);
      } else if (from == chpl_c_string) {
        // a c_string can be interpreted as a pointer to c_char for this purpose
        return pointeeCastStrictAliasingAllowed(c_char, to.eltType);
      } else if (to == chpl_c_string) {
        return pointeeCastStrictAliasingAllowed(from.eltType, c_char);
      }
    }
    // allow casting to and from void pointee type
    if (from == void || to == void) {
      return true;
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

  @chpldoc.nodoc
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
  @chpldoc.nodoc
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
  @chpldoc.nodoc
  inline operator c_ptrConst.:(x:c_ptrConst, type t:c_ptr) {
    if (!pointeeCastStrictAliasingAllowed(x.type, t)) {
      compilerWarning(
          "This cast from '" + x.type:string + "' to '" + t:string +
          "' casts a c_ptrConst to a pointer of non-equivalent, non-char " +
          "element type, which can cause undefined behavior.");
    }
    return __primitive("cast", t, x);
  }
  @chpldoc.nodoc
  inline operator c_ptr.:(x:c_ptr, type t:c_ptrConst) {
    if (!pointeeCastStrictAliasingAllowed(x.type, t)) {
      compilerWarning(
          "This cast from '" + x.type:string + "' to '" + t:string +
          "' casts a c_ptr to a pointer of non-equivalent, non-char " +
          "element type, which can cause undefined behavior.");
    }
    return __primitive("cast", t, x);
  }
  @chpldoc.nodoc
  inline operator :(ref x:c_array, type t:c_ptr) {
    if t.eltType == void then
      return __primitive("cast", t, c_ptrTo(x[0]));
    else if x.eltType == t.eltType then
      return c_ptrTo(x[0]);
    else
      compilerError("cast of c_array to c_ptr with a different element type");
  }
  @chpldoc.nodoc
  inline operator :(const ref x:c_array, type t:c_ptrConst) {
    if t.eltType == void then
      return __primitive("cast", t, c_ptrToConst(x[0]));
    else if x.eltType == t.eltType then
      return c_ptrToConst(x[0]);
    else
      compilerError("cast of c_array to c_ptr with a different element type");
  }
  @chpldoc.nodoc
  inline operator :(x:c_ptr, type t:c_ptr(void)) {
    return __primitive("cast", t, x);
  }
  @chpldoc.nodoc
  inline operator :(x:c_ptr, type t:c_ptrConst(void)) {
    return __primitive("cast", t, x);
  }
  @chpldoc.nodoc
  inline operator :(x:c_ptrConst, type t:c_ptr(void)) {
    return __primitive("cast", t, x);
  }
  @chpldoc.nodoc
  inline operator :(x:c_ptrConst, type t:c_ptrConst(void)) {
    return __primitive("cast", t, x);
  }
  // c_ptr(void) specialization of c_ptr->c_ptr cast, to enable implicit
  // conversion of casted value to c_ptr(void)
  @chpldoc.nodoc
  inline operator :(x:c_ptr(void), type t:c_ptr) {
    return __primitive("cast", t, x);
  }
  @chpldoc.nodoc
  inline operator :(x:c_ptr(void), type t:c_ptrConst) {
    return __primitive("cast", t, x);
  }
  @chpldoc.nodoc
  inline operator :(x:c_ptrConst(void), type t:c_ptrConst) {
    return __primitive("cast", t, x);
  }
  @chpldoc.nodoc
  inline operator c_ptr.:(x:c_ptr, type t:string) {
    try! {
      return string.createAdoptingBuffer(__primitive("ref to string", x):c_ptrConst(c_char));
    }
  }
  @chpldoc.nodoc
  inline operator c_ptrConst.:(x:c_ptrConst, type t:string) {
    try! {
      return string.createAdoptingBuffer(__primitive("ref to string", x):c_ptrConst(c_char));
    }
  }
  pragma "last resort"
  @chpldoc.nodoc
  inline operator :(x:c_ptr(void), type t:class) {
    if isUnmanagedClass(t) || isBorrowedClass(t) {
      compilerError("invalid cast from c_ptr(void) to "+ t:string +
                    " - cast to "+ _to_nilable(t):string +" instead");
    } else {
      compilerError("invalid cast from c_ptr(void) to managed type " + t:string);
    }
    return __primitive("cast", t, x);
  }

  @chpldoc.nodoc
  inline operator :(x:c_ptr(void), type t:unmanaged class?) {
    return __primitive("cast", t, x);
  }
  @chpldoc.nodoc
  inline operator :(x:c_ptr(void), type t:borrowed class?) {
    return __primitive("cast", t, x);
  }

  @chpldoc.nodoc
  @deprecated(notes="Casting from class types directly to c_ptr(void) is deprecated. Please use c_ptrTo/c_ptrToConst instead.")
  inline operator :(x:borrowed, type t:c_ptr(void)) {
    return __primitive("cast", t, x);
  }
  @chpldoc.nodoc
  @deprecated(notes="Casting from class types directly to c_ptr(void) is deprecated. Please use c_ptrTo/c_ptrToConst instead.")
  inline operator :(x:unmanaged, type t:c_ptr(void)) {
    return __primitive("cast", t, x);
  }

  @chpldoc.nodoc
  inline operator :(x:c_ptr, type t:_ddata)
      where t.eltType == x.eltType || x.eltType == void {
    return __primitive("cast", t, x);
  }
  // c_ptr(void) specific overload need for types that implicitly convert to it
  @chpldoc.nodoc
  inline operator :(x:c_ptr(void), type t:_ddata) {
    return __primitive("cast", t, x);
  }
  @chpldoc.nodoc
  inline operator :(x:_ddata, type t:c_ptr(void)) {
    return __primitive("cast", t, x);
  }

  // Casts from c pointer to c_intptr / c_uintptr
  // Due to implicit conversion to c_ptr(void), don't need to define c_array or
  // c_ptrConst versions.
  @chpldoc.nodoc
  inline operator :(x:c_ptr(void), type t:c_intptr) do
    return __primitive("cast", t, x);
  @chpldoc.nodoc
  inline operator :(x:c_ptr(void), type t:c_uintptr) do
    return __primitive("cast", t, x);
  // casts from c pointer to int / uint
  // note that these are only used if c_intptr != int / c_uintptr != uint
  @chpldoc.nodoc
  inline operator :(x:c_ptr(void), type t:int) where c_intptr != int do
    return __primitive("cast", t, x);
  @chpldoc.nodoc
  inline operator :(x:c_ptr(void), type t:uint) where c_uintptr != uint do
    return __primitive("cast", t, x);

  // casts from c_intptr / c_uintptr to c_ptr(void)
  @chpldoc.nodoc
  inline operator :(x:c_intptr, type t:c_ptr(void)) do
    return __primitive("cast", t, x);
  @chpldoc.nodoc
  inline operator :(x:c_uintptr, type t:c_ptr(void)) do
    return __primitive("cast", t, x);

  // casts from int / uint to c_ptr(void)
  // note that these are only used if c_intptr != int / c_uintptr != uint
  @chpldoc.nodoc
  inline operator :(x:int, type t:c_ptr(void))
      where c_intptr != int do
    return __primitive("cast", t, x);
  @chpldoc.nodoc
  inline operator :(x:uint, type t:c_ptr(void))
      where c_uintptr != uint do
    return __primitive("cast", t, x);


  @chpldoc.nodoc
  inline operator c_ptr.==(a: c_ptr, b: c_ptr)
      where a.eltType == b.eltType || a.eltType == void || b.eltType == void {
    return __primitive("ptr_eq", a, b);
  }
  // special c_ptr(void) versions of == and != are needed to allow comparing
  // raw_c_void_ptrs, via their implicit conversion to c_ptr(void)
  pragma "last resort"
  @chpldoc.nodoc
  inline operator ==(a: c_ptr(void), b: c_ptr(void)) {
    return __primitive("ptr_eq", a, b);
  }
  pragma "last resort"
  @chpldoc.nodoc
  inline operator !=(a: c_ptr(void), b: c_ptr(void)) {
    return __primitive("ptr_neq", a, b);
  }
  @chpldoc.nodoc
  inline operator ==(a: c_ptr, b: _nilType) {
    return __primitive("ptr_eq", a, b);
  }
  @chpldoc.nodoc
  inline operator ==(a: _nilType, b: c_ptr) {
    return __primitive("ptr_eq", a, b);
  }
  @chpldoc.nodoc
  inline operator c_ptrConst.==(a: c_ptrConst, b: c_ptrConst)
      where a.eltType == b.eltType || a.eltType == void || b.eltType == void {
    return __primitive("ptr_eq", a, b);
  }
  @chpldoc.nodoc
  inline operator ==(a: c_ptrConst, b: _nilType) {
    return __primitive("ptr_eq", a, b);
  }
  @chpldoc.nodoc
  inline operator ==(a: _nilType, b: c_ptrConst) {
    return __primitive("ptr_eq", a, b);
  }

  @chpldoc.nodoc
  inline operator c_ptr.!=(a: c_ptr, b: c_ptr)
      where a.eltType == b.eltType || a.eltType == void || b.eltType == void {
    return __primitive("ptr_neq", a, b);
  }
  @chpldoc.nodoc
  inline operator !=(a: c_ptr, b: _nilType) {
    return __primitive("ptr_neq", a, b);
  }
  @chpldoc.nodoc
  inline operator !=(a: _nilType, b: c_ptr) {
    return __primitive("ptr_neq", a, b);
  }
  @chpldoc.nodoc
  inline operator c_ptrConst.!=(a: c_ptrConst, b: c_ptrConst)
      where a.eltType == b.eltType || a.eltType == void || b.eltType == void {
    return __primitive("ptr_neq", a, b);
  }
  @chpldoc.nodoc
  inline operator c_ptrConst.!=(a: c_ptrConst, b: _nilType) {
    return __primitive("ptr_neq", a, b);
  }
  @chpldoc.nodoc
  inline operator !=(a: _nilType, b: c_ptrConst) {
    return __primitive("ptr_neq", a, b);
  }

  @chpldoc.nodoc
  inline operator c_ptr.!(x: c_ptr) do return x == nil;

  @chpldoc.nodoc
  inline operator c_ptr.+(a: c_ptr, b: integral) do return __primitive("+", a, b);

  @chpldoc.nodoc
  inline operator c_ptr.-(a: c_ptr, b: integral) do return __primitive("-", a, b);

  @chpldoc.nodoc
  inline operator c_ptrConst.!(x: c_ptrConst) do return x == nil;

  @chpldoc.nodoc
  inline operator c_ptrConst.+(a: c_ptrConst, b: integral) do
    return __primitive("+", a, b);

  @chpldoc.nodoc
  inline operator c_ptrConst.-(a: c_ptrConst, b: integral) do
    return __primitive("-", a, b);

  @chpldoc.nodoc
  inline operator c_ptr.-(a: c_ptr(?t), b: c_ptr(t)):c_ptrdiff {
    return c_pointer_diff(a, b, c_sizeof(a.eltType):c_ptrdiff);
  }
  @chpldoc.nodoc
  inline operator c_ptrConst.-(a: c_ptrConst(?t), b: c_ptrConst(t)):c_ptrdiff {
    return c_pointer_diff(a, b, c_sizeof(a.eltType):c_ptrdiff);
  }

  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  @chpldoc.nodoc
  extern proc c_pointer_return(ref x:?t):c_ptr(t);
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  @chpldoc.nodoc
  extern proc c_pointer_return_const(const ref x:?t):c_ptrConst(t);
  pragma "fn synchronization free"
  @chpldoc.nodoc
  extern proc c_pointer_diff(a:c_ptr(void), b:c_ptr(void),
                             eltSize:c_ptrdiff):c_ptrdiff;

  // Transition symbol for 1.31 c_ptrTo behavior deprecations,
  // itself deprecated in 1.33.
  @chpldoc.nodoc
  @deprecated("'cPtrToLogicalValue' is deprecated and no longer affects the behavior of 'c_ptrTo'")
  config param cPtrToLogicalValue = true;

  // Begin c_ptrTo overloads

  /*
    Returns a :type:`c_ptr` to any Chapel object.

    For most types, the returned :type:`c_ptr` will simply point to the memory
    address of the Chapel variable, and have element type matching the type of
    the passed-in variable. However, this procedure has special behavior for
    variables of the following types, and will return a pointer that is
    potentially more useful:

    * ``array`` types: Returns a pointer to the first element of the array, with
      pointer element type matching the array's element type.
    * ``class`` types: Returns a ``c_ptr(void)`` to the heap instance of the
      class variable.
    * ``string`` type: Returns a ``c_ptr(c_uchar)`` to the underlying buffer of
      the ``string``.
    * ``bytes`` type: Returns a ``c_ptr(c_uchar)`` to the underlying buffer of
      the ``bytes``.

    To avoid this special behavior and just get the variable address naively
    regardless of type, use :proc:`c_addrOf` instead. ``c_ptrTo`` has identical
    behavior to ``c_addrOf`` on types other than those with special behavior
    listed above.

    .. note::

      The existence of the ``c_ptr`` has no impact on the lifetime
      of the object it points to. In many cases the object will be stack
      allocated and could go out of scope even if this ``c_ptr`` remains.

    :arg x:   The by-reference argument to get a pointer to. Domains are not
              supported, and will cause a compiler error.
    :returns: A :type:`c_ptr` to the argument passed by reference, with address
              and element type potentially depending on special behavior for the
              the type as described above.

  */
  inline proc c_ptrTo(ref x:?t) {
    if isDomainType(t) then
      compilerError("c_ptrTo domain type not supported");
    return c_addrOf(x);
  }

  @chpldoc.nodoc
  inline proc c_ptrTo(x: c_fn_ptr) {
    return x;
  }

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
  @chpldoc.nodoc
  inline proc c_ptrTo(ref arr: []): c_ptr(arr.eltType) {
    if (!chpl__isDROrDRView(arr)) then
      compilerError("Only single-locale rectangular arrays support c_ptrTo() at present");

    if boundsChecking {
      if (arr._value.locale != here) then
        halt(
            "c_ptrTo() can only be applied to an array from the locale on " +
            "which it lives (array is on locale " + arr._value.locale.id:string +
            ", call was made on locale " + here.id:string + ")");

      if (arr.size == 0) then
        halt("Can't create a C pointer for an array with 0 elements.");
    }

    return c_pointer_return(arr[arr.domain.low]);
  }

  /*
    Returns a :type:`c_ptr` to the underlying buffer of a :type:`~String.string`

    Note that the existence of this ``c_ptr`` has no impact on the lifetime of
    the ``string``.  The returned pointer will be invalid if the ``string`` is
    freed or even reallocated.

    Halts if the ``string`` is empty and bounds checking is enabled.
  */
  @chpldoc.nodoc
  inline proc c_ptrTo(ref s: string): c_ptr(c_uchar) {
    if boundsChecking {
      if (s.buffLen == 0) then
        halt("Can't create a C pointer for an empty string.");
    }
    return c_pointer_return(s.buff[0]);
  }

  /*
    Returns a :type:`c_ptr` to the underlying buffer of a :type:`~Bytes.bytes`

    Note that the existence of this ``c_ptr`` has no impact on the lifetime of
    the ``bytes``.  The returned pointer will be invalid if the ``bytes`` is
    freed or even reallocated.

    Halts if the ``bytes`` is empty and bounds checking is enabled.
  */
  @chpldoc.nodoc
  inline proc c_ptrTo(ref b: bytes): c_ptr(c_uchar) {
    if boundsChecking {
      if (b.buffLen == 0) then
        halt("Can't create a C pointer for an empty bytes.");
    }
    return c_pointer_return(b.buff[0]);
  }

  @chpldoc.nodoc
  inline proc c_ptrTo(c: class): c_ptr(void) {
    return __primitive("cast", c_ptr(void), c.borrow());
  }
  /*
    Returns a ``c_ptr(void)`` to the heap instance of a class type.

    Note that the existence of this ``c_ptr(void)`` has no impact on the
    lifetime of the instance.  The returned pointer will be invalid if the
    instance is freed or even reallocated.
  */
  @chpldoc.nodoc
  inline proc c_ptrTo(c: class?): c_ptr(void) {
    return __primitive("cast", c_ptr(void), c.borrow());
  }

  // End c_ptrTo overloads

  // Begin c_ptrToConst overloads

  /*
    Like :proc:`c_ptrTo`, but returns a :type:`c_ptrConst` which disallows direct
    modification of the pointee.
  */
  inline proc c_ptrToConst(const ref x:?t): c_ptrConst(t) {
    if isDomainType(t) then
      compilerError("c_ptrToConst domain type not supported");
    return c_addrOfConst(x);
  }

  /*
   Like :proc:`c_ptrTo` for arrays, but returns a :type:`c_ptrConst` which
   disallows direct modification of the pointee.
   */
  @chpldoc.nodoc
  inline proc c_ptrToConst(const arr: []): c_ptrConst(arr.eltType) {
    if (!chpl__isDROrDRView(arr)) then
      compilerError("Only single-locale rectangular arrays support c_ptrToConst() at present");

    if boundsChecking {
      if (arr._value.locale != here) then
        halt(
            "c_ptrToConst() can only be applied to an array from the locale on " +
            "which it lives (array is on locale " + arr._value.locale.id:string +
            ", call was made on locale " + here.id:string + ")");

      if (arr.size == 0) then
        halt("Can't create a C pointer for an array with 0 elements.");
    }

    return c_pointer_return_const(arr[arr.domain.low]);
  }

  /*
   Like :proc:`c_ptrTo` for :type:`~String.string`, but returns a
   :type:`c_ptrConst` which disallows direct modification of the pointee.
   */
  @chpldoc.nodoc
  inline proc c_ptrToConst(const ref s: string): c_ptrConst(c_uchar) {
    if boundsChecking {
      if (s.buffLen == 0) then
        halt("Can't create a C pointer for an empty string.");
    }
    return c_pointer_return_const(s.buff[0]);
  }

  /*
   Like :proc:`c_ptrTo` for :type:`~Bytes.bytes`, but returns a
   :type:`c_ptrConst` which disallows direct modification of the pointee.
   */
  @chpldoc.nodoc
  inline proc c_ptrToConst(const ref b: bytes): c_ptrConst(c_uchar) {
    if boundsChecking {
      if (b.buffLen == 0) then
        halt("Can't create a C pointer for an empty bytes.");
    }
    return c_pointer_return_const(b.buff[0]);
  }

  @chpldoc.nodoc
  inline proc c_ptrToConst(const c: class): c_ptrConst(void) {
    return __primitive("cast", c_ptrConst(void), c.borrow());
  }
  /*
   Like :proc:`c_ptrTo` for class types, but also accepts ``const`` data.
   */
  @chpldoc.nodoc
  inline proc c_ptrToConst(const c: class?): c_ptrConst(void) {
    return __primitive("cast", c_ptrConst(void), c.borrow());
  }

  // End c_ptrToConst overloads

  // Begin c_addrOf[Const] definitions

  /*
    Returns a :type:`c_ptr` to the address of an array.

    This is distinct from :func:`c_ptrTo` in that it returns a pointer to
    the array object itself, rather than to the first element of the array's
    buffer.

    Note that the existence of this :type:`c_ptr` has no impact on the lifetime
    of the array. The returned pointer will be invalid if the array is freed.
  */
  @chpldoc.nodoc
  inline proc c_addrOf(ref arr: []) {
    if (boundsChecking && arr._value.locale != here) then
      halt(
          "c_addrOf() can only be applied to an array from the locale on " +
          "which it lives (array is on locale " + arr._value.locale.id:string +
          ", call was made on locale " + here.id:string + ")");

    return c_pointer_return(arr);
  }

  /*
   Like :proc:`c_addrOf` for arrays, but returns a :type:`c_ptrConst` which
   disallows direct modification of the pointee.
  */
  @chpldoc.nodoc
  inline proc c_addrOfConst(const arr: []) {
    if (boundsChecking && arr._value.locale != here) then
      halt(
          "c_addrOfConst() can only be applied to an array from the locale on " +
          "which it lives (array is on locale " + arr._value.locale.id:string +
          ", call was made on locale " + here.id:string + ")");

    return c_pointer_return_const(arr);
  }

  /*
    Returns a :type:`c_ptr` to the address of any Chapel object.

    Note that the behavior of this procedure is identical to :proc:`c_ptrTo`
    for scalar types and records. It only differs for arrays, strings, bytes,
    and class variables; see documentation of :proc:`c_ptrTo` for special
    behavior on those types.
  */
  inline proc c_addrOf(ref x: ?t): c_ptr(t) {
    if isDomainType(t) then
      compilerError("c_addrOf domain type not supported");
    return c_pointer_return(x);
  }

  /*
    Like :proc:`c_addrOf`, but returns a :type:`c_ptrConst` which disallows
    direct modification of the pointee.
  */
  inline proc c_addrOfConst(const ref x: ?t): c_ptrConst(t) {
    if isDomainType(t) then
      compilerError("c_addrOfConst domain type not supported");
    return c_pointer_return_const(x);
  }

  @chpldoc.nodoc
  inline proc c_addrOf(x: c_fn_ptr) {
    return x;
  }

  // End c_addrOf[Const] definitions

  // Offset the CHPL_RT_MD constant in order to preserve the value through
  // calls to chpl_here_alloc. See comments on offset_STR_* in String.chpl
  // for more.
  private proc offset_ARRAY_ELEMENTS {
    extern const CHPL_RT_MD_ARRAY_ELEMENTS:chpl_mem_descInt_t;
    pragma "fn synchronization free"
    extern proc chpl_memhook_md_num(): chpl_mem_descInt_t;
    return CHPL_RT_MD_ARRAY_ELEMENTS - chpl_memhook_md_num();
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

    return c_addrOf(getFieldRef(x, fieldname)):c_size_t - c_addrOf(x):c_size_t;
  }

  @chpldoc.nodoc
  proc c_offsetof(type t, param fieldname: string) where !isRecordType(t) {
    compilerError("Cannot call c_offsetof on type that is not a record");
  }

  /* Allocate memory.

     This uses the Chapel allocator. Memory allocated with this function should
     eventually be freed with :proc:`deallocate`.

    :arg eltType: the type of the elements to allocate
    :arg size: the number of elements to allocate space for
    :arg clear: whether to initialize all bits of allocated memory to 0
    :arg alignment: Memory alignment of the allocation, which must be a power of
                    two and a multiple of `c_sizeof(c_ptr(void))`. Alignment of 0
                    is invalid and taken to mean default alignment.
    :returns: a c_ptr(eltType) to allocated memory
   */
  @unstable("'allocate' is unstable, and may be renamed or moved")
  inline proc allocate(type eltType, size: c_size_t, clear: bool = false,
      alignment: c_size_t = 0) : c_ptr(eltType) {
    const alloc_size = size * c_sizeof(eltType);
    const aligned : bool = (alignment != 0);
    var ptr : c_ptr(void) = nil;

    // pick runtime allocation function based on requested zeroing + alignment
    if (!aligned) {
      if (clear) {
        // normal calloc
        ptr = chpl_here_calloc(alloc_size, 1, offset_ARRAY_ELEMENTS);
      } else {
        // normal malloc
        ptr = chpl_here_alloc(alloc_size, offset_ARRAY_ELEMENTS);
      }
    } else {
      // check alignment, size restriction
      // Alignment of 0 is our sentinel value for no specified alignment,
      // so no need to check for it.
      if boundsChecking {
        use Math;
        var one:c_size_t = 1;
        // Round the alignment up to the nearest power of 2
        var p = log2(alignment); // power of 2 rounded down
        // compute alignment rounded up
        if (one << p) < alignment then
          p += 1;
        assert(alignment <= (one << p));
        if alignment != (one << p) then
          halt("allocate called with non-power-of-2 alignment ", alignment);
        if alignment < c_sizeof(c_ptr(void)) then
          halt("allocate called with alignment smaller than pointer size");
      }

      // normal aligned alloc, whether we clear after or not
      ptr = chpl_here_aligned_alloc(alignment, alloc_size,
          offset_ARRAY_ELEMENTS);

      if (clear) {
        // there is no aligned calloc; have to aligned_alloc + memset to 0
        private use OS.POSIX;
        memset(ptr, 0, alloc_size);
      }
    }

    return ptr : c_ptr(eltType);
  }

  /* Free memory that was allocated with :proc:`allocate`.

    :arg data: the c_ptr to memory that was allocated. Note that both
               `c_ptr(t)` and `c_ptr(void)` can be passed to this argument.
    */
  @unstable("'deallocate' is unstable, and may be renamed or moved")
  inline proc deallocate(data: c_ptr(void)) {
    chpl_here_free(data);
  }

  // For internal use
  @chpldoc.nodoc
  proc isAnyCPtr(type t:c_ptr) param do return true;
  @chpldoc.nodoc
  proc isAnyCPtr(type t:c_ptrConst) param do return true;
  @chpldoc.nodoc
  proc isAnyCPtr(type t) param do return false;

  /*
    Get the number of bytes in a c_ptr(int(8)) or c_ptr(uint(8)), excluding the
    terminating null.

    :arg x: c_ptr(int(8)) or c_ptr(uint(8)) to get length of

    :returns: the number of bytes in x, excluding the terminating null
   */
  @unstable("the strLen function is unstable and may change or go away in a future release")
  inline proc strLen(x:c_ptr(?t)) : int {
     return __primitive("string_length_bytes", x).safeCast(int);
  }

  /*
    Get the number of bytes in a c_ptrConst(int(8)) or c_ptrConst(uint(8)),
    excluding the terminating NULL.

    :arg x: c_ptrConst(int(8)) or c_ptrConst(uint(8)) to get length of

    :returns: the number of bytes in x, excluding the terminating NULL
   */
  @unstable("the strLen function is unstable and may change or go away in a future release")
  inline proc strLen(x:c_ptrConst(?t)): int {
     return __primitive("string_length_bytes", x).safeCast(int);
  }

  /*
    Get a `c_ptrConst(c_char)` from a :type:`~String.string`. The returned
    `c_ptrConst(c_char)` shares the buffer with the :type:`~String.string`.

    .. warning::

        This can only be called safely on a :type:`~String.string` whose home is
        the current locale.  This property can be enforced by calling
        :proc:`~String.string.localize()` before :proc:`string.c_str()`.
        If the string is remote, the program will halt.

    For example:

    .. code-block:: chapel

      var myString = "Hello!";
      on differentLocale {
        writef("%s", myString.localize().c_str());
      }

    .. warning::

        A Chapel :type:`~String.string` is capable of containing NULL characters
        and any C routines relying on NULL terminated buffers may incorrectly
        process the mid-string NULL as the terminating NULL.

    :returns:
        A `c_ptrConst(c_char)` that points to the underlying buffer used by this
        :type:`~String.string`. The returned `c_ptrConst(c_char)` is only valid
        when used on the same locale as the string.
   */
  @unstable("'string.c_str()' is unstable and may change in a future release")
  inline proc string.c_str() : c_ptrConst(c_char) {
    use BytesStringCommon only getCStr;
    return getCStr(this);
  }

 /*
    Gets a `c_ptrConst(c_char)` from a :type:`~Bytes.bytes`. The returned
    `c_ptrConst(c_char)` shares the buffer with the :type:`~Bytes.bytes`.

    .. warning::

      This can only be called safely on a :type:`~Bytes.bytes` whose home is
      the current locale.  This property can be enforced by calling
      :proc:`~Bytes.bytes.localize()` before :proc:`bytes.c_str()`.
      If the bytes is remote, the program will halt.

    For example:

    .. code-block:: chapel

        var myBytes = b"Hello!";
        on differentLocale {
          writef("%s", myBytes.localize().c_str());
        }

    .. warning::

        Chapel :type:`~Bytes.bytes` are capable of containing NULL bytes and
        any C routines relying on NULL terminated buffers may incorrectly
        process the mid-buffer NULL as the terminating NULL.

    :returns: A `c_ptrConst(c_char)` that points to the underlying buffer used
              by this :type:`~Bytes.bytes`. The returned `c_ptrConst(c_char)`
              is only valid when used on the same locale as the bytes.
   */
  @unstable("'bytes.c_str()' is unstable and may change in a future release")
  inline proc bytes.c_str(): c_ptrConst(c_char) {
    use BytesStringCommon only getCStr;
    return getCStr(this);
  }

}
