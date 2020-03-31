.. default-domain:: chpl

.. module:: UtilMisc_forDocs
   :synopsis: Additional utilities

Misc Functions
==============

Additional utilities


.. function:: proc compilerError(param msg: string ...?n)

   
   Generate a compile-time error.
   The error text is a concatenation of the arguments.
   

.. function:: proc compilerError(param msg: string ...?n, param errorDepth: int)

   
   Generate a compile-time error.
   The error text is a concatenation of the string arguments.
   
   :arg errorDepth: controls the depth of the error stack trace
   

.. function:: proc compilerWarning(param msg: string ...?n)

   
   Generate a compile-time warning.
   The warning text is a concatenation of the arguments.
   

.. function:: proc compilerWarning(param msg: string ...?n, param errorDepth: int)

   
   Generate a compile-time warning.
   The warning text is a concatenation of the string arguments.
   
   :arg errorDepth: controls the depth of the error stack trace
   

.. function:: proc compilerAssert(param test: bool)

   Generate a compile-time error if the `test` argument is false.
   

.. function:: proc compilerAssert(param test: bool, param errorDepth: int)

   Generate a compile-time error if the `test` argument is false.
   
   :arg errorDepth: controls the depth of the error stack trace
   

.. function:: proc compilerAssert(param test: bool, param msg: string ...?n)

   Generate a compile-time error if the `test` argument is false.
   The warning text is a concatenation of the string arguments.
   

.. function:: proc compilerAssert(param test: bool, param msg: string ...?n, param errorDepth: int)

   Generate a compile-time error if the `test` argument is false.
   The warning text is a concatenation of the string arguments.
   
   :arg errorDepth: controls the depth of the error stack trace
   

.. function:: proc min(x, y ...)

   Compute the minimum value of 2 or more arguments
   using the ``<`` operator for comparison.
   If one of the arguments is :proc:`Math.NAN`, the result is also NAN. 

.. function:: proc max(x, y ...)

   Compute the maximum value of 2 or more arguments
   using the ``>`` operator for comparison.
   If one of the arguments is :proc:`Math.NAN`, the result is also NAN. 

.. function:: proc exit(status: int)

   
   Exit the program
   
   :arg status: The exit code for the program
   

.. function:: proc isCoercible(type from, type to) param

   Returns `true` if the type `from` is coercible to the type `to`,
   or if ``isSubtype(from, to)`` would return `true`.
   

.. function:: proc isSubtype(type sub, type super) param

   Returns `true` if the type `sub` is a subtype of the type `super`.
   In particular, returns `true` in any of these cases:
   
     * `sub` is the same type as `super`
     * `sub` is an instantiation of a generic type `super`
     * `sub` is a class type inheriting from `super`
   
   Note that ``isSubtype(a,b)`` can also be written as
   ``a <= b`` or ``b >= a``.
   

.. function:: proc isProperSubtype(type sub, type super) param

   Similar to :proc:`isSubtype` but returns `false` if
   `sub` and `super` refer to the same type.
   
   Note that ``isProperSubtype(a,b)`` can also be written
   as ``a < b`` or ``b > a``.
   

.. function:: proc <(type a, type b) param

   :returns: isProperSubtype(a,b) 

.. function:: proc <=(type a, type b) param

   :returns: isSubtype(a,b) 

.. function:: proc >(type a, type b) param

   :returns: isProperSubtype(b,a) 

.. function:: proc >=(type a, type b) param

   :returns: isSubtype(b,a) 

