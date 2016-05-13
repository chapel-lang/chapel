/*
 * Copyright 2004-2016 Cray Inc.
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

/* A Chapel version of a C pointer. */
module CPtr {
  /* A Chapel version of a C NULL pointer. */
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
  pragma "c_ptr class"
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
  pragma "no doc"
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
  inline proc ==(a: c_void_ptr, b: _nilType) {
    return __primitive("ptr_eq", a, c_nil);
  }
  pragma "no doc"
  inline proc ==(a: _nilType, b: c_void_ptr) {
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
  inline proc !=(a: c_void_ptr, b: _nilType) {
    return __primitive("ptr_neq", a, c_nil);
  }
  pragma "no doc"
  inline proc !=(a: _nilType, b: c_void_ptr) {
    return __primitive("ptr_neq", c_nil, b);
  }

  pragma "no doc"
  inline proc _cond_test(x: c_ptr) return x != c_nil;

  pragma "no doc"
  inline proc !(x: c_ptr) return x == c_nil;

  pragma "no doc"
  inline proc +(a: c_ptr, b: integral) return __primitive("+", a, b);

  pragma "no doc"
  inline proc -(a: c_ptr, b: integral) return __primitive("-", a, b);


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

  private extern const CHPL_RT_MD_ARRAY_ELEMENTS:chpl_mem_descInt_t;

  private inline proc c_sizeof(type x): size_t {
    extern proc sizeof(type x): size_t;
    return sizeof(x);
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
    const alloc_size = size.safeCast(size_t) * c_sizeof(eltType);
    return chpl_here_calloc(alloc_size, 1, CHPL_RT_MD_ARRAY_ELEMENTS):c_ptr(eltType);
  }

  /*
    Allocate memory that is not initialized. This memory should eventually be
    freed with :proc:`c_free`.

    :arg eltType: the type of the elements to allocate
    :arg size: the number of elements to allocate space for
    :returns: a c_ptr(eltType) to allocated memory
    */
  inline proc c_malloc(type eltType, size: integral) : c_ptr(eltType) {
    const alloc_size = size.safeCast(size_t) * c_sizeof(eltType);
    return chpl_here_alloc(alloc_size, CHPL_RT_MD_ARRAY_ELEMENTS):c_ptr(eltType);
  }

  /* Free memory that was allocated with :proc:`c_calloc` or :proc:`c_malloc`.

    :arg data: the c_ptr to memory that was allocated
    */
  inline proc c_free(data: c_ptr) {
    chpl_here_free(data);
  }


  /*
    Copies n (potentially overlapping) bytes from memory area src to memory
    area dest.

    This is a simple wrapper over the C memmove() function.

    :arg dest: the destination memory area to copy to
    :arg src: the source memory area to copy from
    :arg n: the number of bytes from src to copy to dest
   */
  inline proc c_memmove(dest: c_ptr, const src: c_ptr, n: integral) {
    extern proc memmove(dest: c_void_ptr, const src: c_void_ptr, n: size_t);
    memmove(dest, src, n.safeCast(size_t));
  }

  /*
    Copies n (non-overlapping) bytes from memory area src to memory
    area dest. Use :proc:`c_memmove` if memory areas do overlap.

    This is a simple wrapper over the C memcpy() function.

    :arg dest: the destination memory area to copy to
    :arg src: the source memory area to copy from
    :arg n: the number of bytes from src to copy to dest
   */
  inline proc c_memcpy(dest: c_ptr, const src: c_ptr, n: integral) {
    extern proc memcpy (dest: c_void_ptr, const src: c_void_ptr, n: size_t);
    memcpy(dest, src, n.safeCast(size_t));
  }

  /*
    Compares the first n bytes of memory areas s1 and s2

    This is a simple wrapper over the C memcmp() function.

    :returns: returns an integer less than, equal to, or greater than zero if
              the first n bytes of s1 are found, respectively, to be less than,
              to match, or be greater than the first n bytes of s2.
   */
  inline proc c_memcmp(const s1: c_ptr, const s2: c_ptr, n: integral) {
    extern proc memcmp(const s1: c_void_ptr, const s2: c_ptr, n: size_t) : c_int;
    return memcmp(s1, s2, n.safeCast(size_t)).safeCast(int);
  }
}
