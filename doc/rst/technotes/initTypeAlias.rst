
.. _readme-initTypeAlias:

.. default-domain:: chpl

==================================================
Invoking Initializers with a Generic Instantiation
==================================================

Overview
--------

Initializing a variable with a generic type has historically required users to
pass each ``type`` and ``param`` value to the initializer as arguments. This
can be a tedious and repetitive process if the user is initializing many
variables of the exact same type. One might wish to use type aliases to reduce
the repetitive nature of initializing the same type many times. For example:

.. code-block:: chapel

  record Map { type K, V; ... }

  var x = new Map(string, string, ...);

  // vs.

  type StringMap = Map(string, string);
  var y = new StringMap(...);

This document describes an experimental solution to this problem that unifies
the invocation of default-initialization and these kinds of new-expressions.

An Experimental Solution: Using Named Expressions
-------------------------------------------------

A solution to this problem is for the compiler to generate named-expression
arguments for each instantiated field in the type alias. This means that if the
author of a type wishes to support this pattern, they need to implement an
initializer with arguments that have the same name as the generic fields of the
type. For example:

.. code-block:: chapel

  record R {
    type T;
    var x : T;
  }

  // must be named 'T'
  proc R.init(type T, x : T) { ... }

  type RI = R(int);
  var x = new RI(5);
  // becomes...
  //   var x = new R(T=int, 5);

The use of named-expressions is a trade-off between flexibility of argument
names and flexibility of argument ordering. Consider the following record with
``type`` fields that are declared adjacent to the fields they constrain:

.. code-block:: chapel

  record R {
    type T;
    var x : T;
    type U;
    var y : U;
  }

  // mimics compiler-generated signature
  // proc R.init(type T, x : T, type U, y : U) { ... }

Without named-expressions, the instantiation information would need to be
passed as the first arguments of the initializer. This would prevent usage
of the compiler-generated initializer for types like ``R``:

.. code-block:: chapel

  type RIS = R(int, string);
  var x = new RIS(5, "hello");
  // without named-expressions, compiler  cannot match
  // (type, type, value, value) to (type, value, type, value)
  //   var x = new R(int, string, 5, "hello");

By prioritizing flexibility of argument ordering, type authors may order their
fields as they wish and can still take advantage of the compiler-generated
initializer when using type aliases:

.. code-block:: chapel

  type RIS = R(int, string);
  var y = new RIS(5, "hello");
  // becomes...
  //   var y = new R(T=int, U=string, 5, "hello");

Default Initialization for Instantiated Generic Types
-----------------------------------------------------

When a variable with a record type is declared without an initialization
expression, the compiler "default-initializes" the variable. For non-generic
types, this is as simple as invoking an initializer without arguments.

Historically the situation for generic types has been more complicated. When
the compiler knew it was invoking a compiler-generated initializer it would use
named-expressions to pass generic instantiation information. When invoking a
user-defined initializer it would simply pass the instantiation arguments
in-order. This allowed user-defined initializers to use arbitrary names for
their arguments, but enforced a specific ordering of those arguments. For
example:

.. code-block:: chapel

  // In Chapel 1.18 ...

  record CompilerGenerated {
    type T;
    var x : T;
    type U;
    var y : U;
  }

  record UserDefined {
    // fields identical to CompilerGenerated

    // arguments can have any name, but the 'type' arguments need to be first
    proc UserDefined.init(type FirstType, type SecondType, ...) { ... }
  }

  var x : CompilerGenerated(int, string); // x.init(T=int, U=string);
  var y : UserDefined(int, string); // y.init(int, string);

In the Chapel 1.19 release default-initializing a variable with a generic type
will always invoke the initializer with named-expressions. This has the benefit
of allowing the same initializer to be used for default-initialization and for
new-expressions with a type alias. This is also a simpler rule for users to
remember. Our previous example now requires the user-defined initializer to use
specific names for its arguments:

.. code-block:: chapel

  // fields are now allowed to be in any order, but need specific names
  proc UserDefined.init(type T, type U, ...) { ... }

  var x : CompilerGenerated(int, string); // x.init(T=int, U=string);
  var y : UserDefined(int, string); // y.init(T=int, U=string);

Some pre-existing user-defined initializers will require updating, trading
flexibility in argument naming for flexibility in argument ordering.

Limitations and Future Work
---------------------------

Fully-generic fields, like ``var x;``, present a design challenge when using
a type alias in a new-expression. The compiler-generated initializer, and most
user-defined initializers, expect a value - not a ``type``. It remains an
open question as to how the compiler should invoke such initializers.

In the Chapel 1.19 release this design challenge was not solved and so types
with fully-generic fields may not be used as type aliases in new-expressions.

