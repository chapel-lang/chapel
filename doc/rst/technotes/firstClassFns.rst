.. _readme-firstClassFns:

===============================
First-class Functions in Chapel
===============================

This README describes some initial support that we have added to the
compiler for first-class functions in Chapel.  This mechanism should
be considered a stopgap technology until we have developed and
implemented a more robust story, which is why it's being described in
this README rather than the language specification.


Manipulating first-class functions
----------------------------------

Functions defined with parentheses may be captured as values by
referring to them by name without parentheses.  Once captured, these
values may be passed around as other value types.

For example:

.. code-block:: chapel

  proc myfunc(x:int) { return x + 1; }
  var f = myfunc;
  writeln(f(3));  // outputs: 4

To be captured, a function must not be any of the following:

- A generic function (all captured functions must be fully-qualified
  with no generic arguments)

- A function with special return types (type, param)

- An iterator

- The method of an object

- An operator

- An overloaded function

- A function referring to outer variable, other than globals

Rationale. Generic functions would require manipulating generic,
uninstantiated types, which is currently not available in Chapel.
Functions with compile-time return types like type and param would
require the ability to have param classes, to fit with the current
implementation. Param classes are not currently part of Chapel.
Iterators would require a new type of capture, one that works
similarly to the current implementation but respects the yielding that
occurs inside an iterator. Method capture requires the currying of
the object as the first argument to the first-class function.
Operators and overloaded functions require a type-based multiple
dispatch mechanism. Functions referring to outer non-global variables
are not currently supported in the implementation.


.. _readme-lambdaFns:

Lambda functions
----------------

Lambda functions are anonymous first-class function objects. In other
words, they are expressions rather than formally-defined named
functions. They are available with the following syntax:

.. code-block:: none

  lambda-declaration-expression:
    lambda argument-list return-type_opt function-body

where ``lambda`` is a Chapel keyword and ``return-type_opt`` is an optional
return-type.

For example:

.. code-block:: chapel

  var f = lambda(x:int, y:int) { return x + y; };
  writeln(f(1,2));  // outputs: 3


Specifying the type of a first-class function
---------------------------------------------

The previous examples rely on type inference to determine the type
for those variables that can be assigned to a first-class function.
Chapel provides three type functions, all named ``func``, that return the
type that corresponds to a function signature as follows:

.. code-block:: chapel

  // Returns the type for a function of no arguments and void return type (returns no value)
  proc func() type

  // Returns the type for a function of no arguments and return type 'retType'
  proc func(type retType) type

  // Returns the type for a function with arguments argTypes and return type 'retType'.
  proc func(type argTypes...?n, type retType) type

These can be used to declare the type of a variable that can be
assigned to values of a function type.  For example:

.. code-block:: chapel

  // Two ways to define a function with no arguments and no return value
  var f1: func();
  var f2: func(void);

  // A function with no arguments, returning int
  var g : func(int);

 // A function with two bool arguments, returning int
  var h : func(bool, bool, int);

Reflection
----------

First-class functions define a type method ``retType`` that returns the type
of the value that would be returned if the function were to be invoked, and
a type method ``argTypes`` that returns a tuple of the types of each formal.

For example:

.. code-block:: chapel

  var F = lambda (x: int) { return x + 42; };

  writeln(F(13));
  writeln();

  writeln("retType  = ", F.retType  : string);
  writeln("argTypes = ", F.argTypes : string);
  writeln();

generates the output::

  55

  retType  = int(64)
  argTypes = 1*int(64)



Future Directions
-----------------

Over time, we will be improving the support for first-class functions
and their syntax.  If you have specific feature requests or
suggestions, please let us know on the `Chapel GitHub issues page`_ or
community forums.

.. _Chapel GitHub issues page: https://github.com/chapel-lang/chapel/issues
