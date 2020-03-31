.. default-domain:: chpl

.. module:: CPtr
   :synopsis: This module contains Chapel types that can serve as C pointer

CPtr
====
This module contains Chapel types that can serve as C pointer
types for the purposes of interoperability and methods to work
with them.

See also :ref:`readme-extern`.


.. function:: proc c_nil: c_void_ptr

   A Chapel version of a C NULL pointer. 

.. function:: proc is_c_nil(x): bool

   
   :returns: true if the passed value is a NULL pointer (ie 0).
   

.. class:: c_ptr

   .. attribute:: type eltType

      The type that this pointer points to 

   .. method:: proc this(i: integral) ref

      Retrieve the i'th element (zero based) from a pointer to an array.
      Does the equivalent of ptr[i] in C.
      

   .. method:: proc deref() ref

      Get element pointed to directly by this pointer. If the pointer
      refers to an array, this will return ptr[0].
      

   .. method:: proc writeThis(ch) throws

      Print this pointer 

.. record:: c_array

   
   This class represents a C array with fixed size.  A variable of type c_array
   can coerce to a c_ptr with the same element type.  In that event, the
   pointer will be equivalent to `c_ptrTo(array[0])`.  A c_array behaves
   similarly to a homogeneous tuple except that its indices start at 0 and it is
   guaranteed to be stored in contiguous memory.  A c_array variable has value
   semantics. Declaring one as a function local variable will create the array
   elements in the function's stack. Assigning or copy initializing will result
   in copying the elements (vs resulting in two pointers that refer to the same
   elements).  A `nil` c_array is not representable in Chapel.
   


   .. attribute:: type eltType

      The array element type 

   .. attribute:: param size

      The fixed number of elements 

   .. method:: proc init(type eltType, param size)

   .. method:: proc deinit()

   .. method:: proc ref this(i: integral) ref: eltType

      Retrieve the i'th element (zero based) from the array.
      Does the equivalent of arr[i] in C.
      Includes bounds checking when such checks are enabled.
      

   .. method:: proc ref this(param i: integral) ref: eltType

      As with the previous function, returns the i'th element (zero based)
      from the array. This one emits a compilation error if i is out of bounds.
      

   .. method:: proc writeThis(ch) throws

      Print the elements 

   .. method:: proc length

      
      Deprecated - please use :var:`c_array.size`.
      

   .. method:: proc init=(other: c_array)

.. function:: proc =(ref lhs: c_array, rhs: c_array)

   Copy the elements from one c_array to another.
   Raises an error at compile time if the array sizes or
   element types do not match. 

.. function:: proc =(ref lhs: c_ptr, ref rhs: c_array)

.. function:: proc c_ptrTo(arr: [])

   
   
   Returns a :type:`c_ptr` to a Chapel rectangular array.  Note that the
   existence of this :type:`c_ptr` has no impact on the lifetime of the array.
   The returned pointer will be invalid if the original array is freed or even
   reallocated. Domain assignment could make this :type:`c_ptr` invalid. If
   the array's data is stored in more than one chunk the procedure will halt
   the program with an error message.
   
   :arg arr: the array for which we should retrieve a pointer
   :returns: a pointer to the array data
   

.. function:: proc c_ptrTo(ref x: ?t): c_ptr(t)

   Returns a :type:`c_ptr` to any Chapel object.
   Note that the existence of the :type:`c_ptr` has no impact of the lifetime
   of the object. In many cases the object will be stack allocated and
   could go out of scope even if this :type:`c_ptr` remains.
   
   :arg x: the by-reference argument to get a pointer to. The argument should
           not be an array or domain (there is a different overload for arrays).
           Records, class instances, integral, real, imag, and complex types are
           supported.
   :returns: a pointer to the argument passed by reference
   
   

.. function:: proc c_sizeof(type x): size_t

   
   Return the size in bytes of a type, as with the C ``sizeof`` built-in.
   
   .. warning::
   
     This method is intended for C interoperability. To enhance
     flexibility, it is possible to request the sizes of Chapel types.
     However, be aware:
   
        * Chapel types are not necessarily stored in contiguous memory
        * Behavior of ``c_sizeof`` with Chapel types may change
        * Behavior given a Chapel class type is not well-defined
   

.. function:: proc c_offsetof(type t, param fieldname: string): size_t

   
   Return the offset of a field in a record.
   
   .. warning::
   
     This method is intended for C interoperability.  To enhance flexibility,
     it is possible to request the offset of elements within a Chapel record.
     However, be aware:
   
     * Chapel types are not necessary stored in contiguous memory
     * Behavior of ``c_offsetof`` may change
     * Behavior given a Chapel class type field is not well-defined
   

.. function:: proc c_calloc(type eltType, size: integral): c_ptr(eltType)

   
   Allocate memory and initialize all bits to 0. Note that this simply zeros
   memory, it does not call Chapel initializers (it is meant for primitive
   types and C interoperability only.) This memory should eventually be freed
   with :proc:`c_free`.
   
   :arg eltType: the type of the elements to allocate
   :arg size: the number of elements to allocate space for
   :returns: a c_ptr(eltType) to allocated memory
   

.. function:: proc c_malloc(type eltType, size: integral): c_ptr(eltType)

   
   Allocate memory that is not initialized. This memory should eventually be
   freed with :proc:`c_free`.
   
   :arg eltType: the type of the elements to allocate
   :arg size: the number of elements to allocate space for
   :returns: a c_ptr(eltType) to allocated memory
   

.. function:: proc c_aligned_alloc(type eltType, alignment: integral, size: integral): c_ptr(eltType)

   
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
   

.. function:: proc c_free(data: c_void_ptr)

   Free memory that was allocated with :proc:`c_calloc` or :proc:`c_malloc`.
   
   :arg data: the c_ptr to memory that was allocated. Note that both
              `c_ptr(t)` and `c_void_ptr` can be passed to this argument.
   

.. function:: proc isAnyCPtr(type t: c_ptr) param

   Returns true if t is a c_ptr type or c_void_ptr.
   

.. function:: proc c_memmove(dest: c_void_ptr, const src: c_void_ptr, n: integral)

   
   Copies n potentially overlapping bytes from memory area src to memory
   area dest.
   
   This is a simple wrapper over the C ``memmove()`` function.
   
   :arg dest: the destination memory area to copy to
   :arg src: the source memory area to copy from
   :arg n: the number of bytes from src to copy to dest
   

.. function:: proc c_memcpy(dest: c_void_ptr, const src: c_void_ptr, n: integral)

   
   Copies n non-overlapping bytes from memory area src to memory
   area dest. Use :proc:`c_memmove` if memory areas do overlap.
   
   This is a simple wrapper over the C memcpy() function.
   
   :arg dest: the destination memory area to copy to
   :arg src: the source memory area to copy from
   :arg n: the number of bytes from src to copy to dest
   

.. function:: proc c_memcmp(const s1: c_void_ptr, const s2: c_void_ptr, n: integral)

   
   Compares the first n bytes of memory areas s1 and s2
   
   This is a simple wrapper over the C ``memcmp()`` function.
   
   :returns: returns an integer less than, equal to, or greater than zero if
             the first n bytes of s1 are found, respectively, to be less than,
             to match, or be greater than the first n bytes of s2.
   

.. function:: proc c_memset(s: c_void_ptr, c: integral, n: integral)

   
   Fill bytes of memory with a particular byte value.
   
   This is a simple wrapper over the C ``memset()`` function.
   
   :arg s: the destination memory area to fill
   :arg c: the byte value to use
   :arg n: the number of bytes of s to fill
   
   :returns: s
   

