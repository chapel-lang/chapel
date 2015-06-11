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

module CPtr {
  /* A Chapel version of a C NULL pointer.
  */
  extern const c_nil:c_void_ptr;

  // To generate legal C prototypes, we have to manually instantiate this
  // prototype for each pointer type that might be associated with 'x'.
  /* :returns: true if the passed value is a NULL pointer (ie 0) */
  pragma "no prototype"
  extern proc is_c_nil(x):bool;

  /*
    A local C pointer class used in C integration. This class represents the
    equivalent to a C language pointer. Instances of this class support
    assignment to other instances or nil, == or != comparison with a c_void_ptr
    or with nil, and casting to another c_ptr type or to the c_void_ptr type.
    In addition, c_ptr works within an if statement directly like so:

    .. code-block:: chapel

      var x: c_ptr;
      if x then do writeln("x is not nil");
      if !x then do writeln("x is nil");

  */
  //   Similar to _ddata from ChapelBase, but differs 
  //   from _ddata because it can never be wide.
  pragma "data class"
  pragma "no object"
  pragma "no default functions"
  pragma "no wide class"
  class c_ptr {
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
  }

  pragma "no doc"
  inline proc =(ref a:c_ptr, b:c_ptr) { __primitive("=", a, b); }

  pragma "no doc"
  inline proc =(ref a:c_ptr, b:_nilType) { __primitive("=", a, c_nil); }

  pragma "no doc"
  inline proc _cast(type t, x) where t:c_ptr && x:_nilType {
    return __primitive("cast", t, x);
  }

  pragma "no doc"
  inline proc _cast(type t, x) where t:c_ptr && x.type:c_ptr {
    return __primitive("cast", t, x);
  }
  pragma "no doc"
  inline proc _cast(type t, x) where t:c_void_ptr && x.type:c_ptr {
    return __primitive("cast", t, x);
  }
  inline proc _cast(type t, x) where t:c_ptr && x.type:c_void_ptr {
    return __primitive("cast", t, x);
  }


  pragma "compiler generated"
  pragma "no doc"
  inline proc _defaultOf(type t) where t == c_void_ptr {
      return __primitive("cast", t, nil);
  }

  pragma "compiler generated"
  pragma "no doc"
  inline proc _defaultOf(type t) where t:c_ptr {
      return __primitive("cast", t, nil);
  }


  pragma "no doc"
  inline proc ==(a: c_ptr, b: c_ptr) where a.eltType == b.eltType {
    return __primitive("ptr_eq", a, b);
  }
  pragma "no doc"
  inline proc ==(a: c_ptr, b: c_void_ptr) {
    return __primitive("ptr_eq", a, b);
  }
  pragma "no doc"
  inline proc ==(a: c_void_ptr, b: c_ptr) {
    return __primitive("ptr_eq", a, b);
  }
  pragma "no doc"
  inline proc ==(a: c_ptr, b: _nilType) {
    return __primitive("ptr_eq", a, c_nil);
  }
  pragma "no doc"
  inline proc ==(a: _nilType, b: c_ptr) {
    return __primitive("ptr_eq", c_nil, b);
  }

  pragma "no doc"
  inline proc !=(a: c_ptr, b: c_ptr) where a.eltType == b.eltType {
    return __primitive("ptr_neq", a, b);
  }
  pragma "no doc"
  inline proc !=(a: c_ptr, b: c_void_ptr) {
    return __primitive("ptr_neq", a, b);
  }
  pragma "no doc"
  inline proc !=(a: c_void_ptr, b: c_ptr) {
    return __primitive("ptr_neq", a, b);
  }
  pragma "no doc"
  inline proc !=(a: c_ptr, b: _nilType) {
    return __primitive("ptr_neq", a, c_nil);
  }
  pragma "no doc"
  inline proc !=(a: _nilType, b: c_ptr) {
    return __primitive("ptr_neq", c_nil, b);
  }

  pragma "no doc"
  inline proc _cond_test(x: c_ptr) return x != c_nil;

  pragma "no doc"
  inline proc !(x: c_ptr) return x == c_nil;

  pragma "no doc"
  extern proc c_pointer_return(ref x:?t):c_ptr(t);


  /* Returns a :type:`c_ptr` to a Chapel rectangular array.
    Note that the existence of this c_ptr has no impact on the lifetime of the
    array. The returned pointer will be invalid if the original array is
    freed or even reallocated. Any domain assignment will probably make
    this c_ptr invalid.

    :arg arr: the array for which we should retrieve a pointer
    :returns: a pointer to the array data
  */
  inline proc c_ptrTo(arr: []) where isRectangularArr(arr) {
    return c_pointer_return(arr[arr.domain.low]);
  }

  /* Returns a :type:`c_ptr` to any Chapel object.
    Note that the existence of the c_ptr has no impact of the lifetime
    of the object. In many cases the object will be stack allocated and
    could go out of scope even if this c_ptr remains.

    :arg x: the by-reference argument to get a pointer to. The argument should
            not be an array or domain (there is a different overload for arrays).
            Records, class instances, integral, real, imag, and complex types are
            supported.
    :returns: a pointer to the argument passed by reference

  */
  inline proc c_ptrTo(ref x:?t):c_ptr(t) {
    if isArrayType(t) then
      compilerError("c_ptrTo unsupported array type", 2);
    if isDomainType(t) then
      compilerError("c_ptrTo domain type not supported", 2);
    // Other cases should be avoided, e.g. sync vars
    return c_pointer_return(x);
  }


  /* Allocate memory that is filled with zeros. This memory should eventually be
    freed with c_free.

    :arg eltType: the type of the elements to allocate
    :arg size: the number of elements to allocate
    :returns: a c_ptr(eltType) to allocated memory
    */
  inline proc c_calloc(type eltType, size: integral) {
    var ret:c_ptr(eltType);
    __primitive("array_alloc", ret, eltType, size);
    init_elts(ret, size, eltType);
    return ret;
  }

  /* Free memory that was allocated with :proc:`c_free`.
    :arg data: the c_ptr to memory that was allocated
    */
  inline proc c_free(data: c_ptr) {
    __primitive("array_free", data);
  }
}
