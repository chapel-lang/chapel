.. _readme-firstClassFns:

===============================
First-class Functions in Chapel
===============================

.. note::

  First-class functions are under active development and are provided
  in their prototype form as a preview and to gather user feedback.
  The features described in the first part of this document are not
  yet stable enough to consider for use in user applications, outside
  of experimentation.

The first part of this document describes new prototype features which
we are in the process of adding and stabilizing. The remaining sections
describe legacy support for first-class functions which have been
included with Chapel for many releases.

While these legacy features may be considered stable for the time
being, we do not intend to advance them further, and will likely
deprecate them after new prototype features become stable. These
sections have had their headers prefixed with 'Legacy' to indicate
that they describe legacy syntax or semantics.

New Syntax for Constructing Function Types
------------------------------------------

Function types may now be constructed using new syntax which mirrors
the syntax for function definition.

To construct the type of a function which takes two integers and
returns an integer, users may write the following:

.. code-block:: chapel

  type T = proc(x: int, y: int): int;
  writeln(T:string); // 'proc(x: int, y: int): int'

Notice that the formals of the function type have specified names,
``x`` and ``y``. Currently, formal names participate in typing such
that a function defined with formals named ``x`` and ``y`` cannot be
assigned to a function defined with different formal names. In the
below example, assignment of two function values will fail because
the formal names are different:

.. code-block:: chapel

  proc foo(x: int, y: int): int { return x + y; }
  proc bar(a: int, b: int): int { return a + b; }
  writeln(foo.type:string);   // 'proc(x: int, y: int): int'
  var x = foo;                // ^ (same type) ^
  writeln(bar.type:string);   // 'proc(a: int, b: int): int'
  x = bar;                    // Error!

There are scenarios where a user may want to pass around functions
without regard for their formal names. In such cases, anonymous
formals may be used instead.

Formals in Function Types May Be Anonymous
------------------------------------------

A formal in a function type may be anonymous. This may be done by
naming the formal ``_``, similar to what is written to discard
tuple elements when de-tupling.

An anonymous formal in a function type indicates that values of this
type may have formals with any name. The formal name no longer plays
a significant role in typing.

.. code-block:: chapel

  proc foo(x: int, y: int): int { return x + y; }
  proc bar(a: int, b: int): int { return a + b; }

  // Here the formals of 'T' are anonymous.
  type T = proc(_: int, _: int): int;

  var x: T = foo;             // OK, T's formals are anonymous '_'.
  writeln(foo.type:string);   // 'proc(x: int, y: int): int'
  writeln(x.type:string);     // 'proc(_: int, _: int): int'
  writeln(bar.type:string);   // 'proc(a: int, b: int): int'
  x = bar;                    // OK!

In the above example, two functions are declared with different formal
names, but otherwise identical types. A local variable expresses a
function type with anonymous formals, which enables the two functions
to be assigned freely to the variable.

.. note::

  Currently, it is not possible for function definitions to declare
  anonymous formals. It has been indicated that such a feature might
  be useful, so this may change in the future.

New Syntax for Constructing Anonymous Functions
-----------------------------------------------

A new syntax for constructing anonymous functions has been introduced
which more closely mirrors traditional function definition.

.. code-block:: chapel

  // Define a function named 'foo'.
  proc foo(x: int, y: int): int { return x + y; }

  // Define an anonymous function bound to the constant variable 'bar'.
  const bar = proc(a: int, b: int): int { return a + b; };

Legacy: Manipulating first-class functions
------------------------------------------

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

Legacy: Lambda functions
------------------------

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


Legacy: Specifying the type of a first-class function
-----------------------------------------------------

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

Legacy: Reflection
------------------

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

generates the output::

  55

  retType  = int(64)
  argTypes = 1*int(64)

Additionally, first-class functions can be cast to a string to get the
function name or printed to output the function name. For example:

.. code-block:: chapel

  proc myFunc(x:int) { return x + 1; }

  var F = myFunc;
  var Fname = F:string;

  writeln(Fname);
  writeln(F);

generates the output::

    myFunc()
    myFunc()


Future Directions
-----------------

New features for first-class functions are under active development as
of release 1.29. If you have specific feature requests or suggestions,
please let us know on the `Chapel GitHub issues page`_ or community
forums.

.. _Chapel GitHub issues page: https://github.com/chapel-lang/chapel/issues

