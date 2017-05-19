Point of Instantiation and Private Scoping
==========================================

Status
  Draft

Authors
  Lydia Duncan
  Michael Ferguson


Abstract
--------

This document discusses an approach to limit the point-of-instantiation
rule and describes a simple design for private scoping that can work
with that design.

Rationale
---------

The manner in which instantiations of generic functions can call other
functions available at the call site is a language design decision with
several implications. Having functions from the call site available to
the generic instantiation is known as a "point of instantiation" rule.

A decision must be reached on this area of the language design in order
to make progress with ``private use`` or to improve the function resolution
phase of the compiler.

Description
-----------

Consider a program that uses the ``Sort`` module. One would like to be able
to provide a sorting function that can be called. For example:

.. code-block:: chapel

  module Sort {

    proc sort(A) {
       ... A[i] < B[i] ...
    }
  }

  module Test {
    use Sort;
    record MyType { ... }
    proc < (a:MyType, b:MyType) { ... }
    var A:[1..100] MyType;
    ...
    sort(A); // programmer indends it to call < declared above
  }
 
However, the ``<`` function declared in Test is not visible to the definition
point of ``proc sort``. In order to enable patterns like this, the generic
instantiation process uses a *point of instantiation* rule in which the generic
instantiation of ``sort`` can use symbols available only at the call site. That
enables the ``<`` function to be found and resolved.

Problems in the Implementation
------------------------------

Let's consider another program to demonstrate a problem in the implementation
of the point-of-instantiation rule with 1.15.

.. code-block:: chapel

  module MyGeneric {
    // uses foo(param)
    proc callFoo(param x) {
      foo(x);
    }
  }

  module A {
    use MyGeneric;
    proc foo(param x) {
      writeln("In A.foo(", x, ")");
    }
    proc testA() {
      callFoo(1);
    }
  }

  module B {
    use MyGeneric;
    proc foo(param x) {
      writeln("In B.foo(", x, ")");
    }
    proc testB() {
      callFoo(1);
    }
  }

  module Main {
    proc main() {
      A.testA();
      B.testB();
    }
  }

If the point-of-instantiation strategy is used, since the functions available
at the call site of a generic instantiation are available during instantiation
the above program should output:

.. code-block::

  In A.foo(1)
  In B.foo(1)

However it currently outputs:

.. code-block::

  In A.foo(1)
  In A.foo(1)

The compiler behaves that way right now because generic instantiation in Chapel
uses a cache, so that generic functions are only instantiated once for a given
set of types and params. However, this cache is not aware of scoping. That
means that in this case, the version instantiated first is re-used for the
second call, even though it should be re-instantiated if we completely use a
point-of-instantiation rule.

Why does public/private interact with point-of-instantiation?
-------------------------------------------------------------

.. code-block:: chapel

  module Test {
    private use Sort;
    record MyType { ... }
    private proc < (a:MyType, b:MyType) { ... }
    var A:[1..100] MyType;
    ...
    sort(A); // programmer indends it to call < declared above
  }

In this example, should the ``sort`` call be able to find the ``<`` routine?
Certainly the instantiation of ``sort`` should have access to any private symbols
in the ``Sort`` module. One might argue that it additionally should have access to
private symbols from the call site. However, enabling such access would mean
that instantiations can use private symbols from the point of instantiation,
which causes new problems as discussed below.

Consider the following program:

.. code-block:: chapel

  module DefineFoo {
    proc foo(param a) {
      writeln(x());
    }
  }

  module UseFoo {
    use DefineFoo;

    proc x() {
      return 22;
    }

    proc main() {
      foo(6);
    }
  }

This program compiles and runs with Chapel 1.15. The ``x()`` call in ``DefineFoo``
resolves to the ``proc x()`` in ``UseFoo``. But what would happen if ``proc x()``
were declared as private? Would the program be valid?

.. code-block:: chapel

  module DefineFoo {
    proc foo(param a) {
      writeln(x());
    }
  }

  module UseFoo {
    use DefineFoo;

    private proc x() {
      return 22;
    }

    proc main() {
      foo(6);
    }
  }


In 1.15, it results in a compilation error. That might make sense: if ``proc
x()`` is private, it is not visible outside of the module it is declared in. In
particular, it is not visible in ``DefineFoo``. However, one might interpret the
point-of-instantiation rule as indicating that such a call to a ``private proc
x()`` should be valid.  The main drawback to interpreting the
point-of-instantiation rule in that manner is that ``private proc x()`` would no
longer make ``x`` actually private; it could be called from any generic function
called from the module in which it is declared.

This is not a problem if the caller was aware that ``foo`` would rely on its
private functions, but having this reliance depend on function calls is very
subtle - if the writer of the function wanted to depend on outside functions, it
is best to specify that dependency explicitly as part of ``foo``s declaration,
either via an interface requirement (see CHIP 2) or by taking the function it
relies upon in as a first-class function argument.

Function Hijacking
------------------

The point-of-instantiation rule is also related to a *function hijacking*
behavior that is described in this excerpt from CHIP 2:

Another problem with the current design for generics in Chapel concerns the
visibility of other functions from inside generic functions.

Suppose that a library developer creates the following module in which the
generic function named ``print_hello_world`` makes a call to another auxiliary
generic function named ``helper``.

.. code-block:: chapel

  module M1 {
    proc helper(x) {
      writeln("hello, world!");
    }
    proc print_hello_world(x) {
      helper(x);
    }
  }

Then suppose that an application programmer decides to use ``M1`` and writes
the following code. It just so happens that somewhere in the application, there
is another function named ``helper``.


.. code-block:: chapel

  module Test {
    use M1;
    proc helper(x : int) {
      writeln("you've been hijacked!");
    }
    proc main() {
      M1.print_hello_world(1);
    }
  }

With the current Chapel function visibility rules, the result of this program
is:

::

  you've been hijacked!


The above is a toy example, but this problem has come up in large C++
applications that use the Boost template libraries.  The most troubling aspect
of this problem is that there may be no immediate indication that something has
gone wrong, and the programmer may only find out much later and after lots of
debugging, that things are not as they seem.

These problems are the same as problems encountered by the C++ community when
working with templates. The strategy proposed for C++ with *concepts* can be
applied to Chapel.

Specific Proposal
-----------------

As we have seen above, point-of-instantiation is problematic because:
 * it can result in surprising behavior
 * it interferes with improvements to ``private``
 * the implementation is challenging to build

Here, we propose that the point-of-instantiation rule be limited to a
last-resort function resolution and that it only apply when:

  1. There is no type-compatible function available at the point of
     definition
  2. There is one or more type-compatible function available at the point
     of instantiation
  3. The best match from the point of instantiation is not marked
     private.

Changing the generic instantiations to prefer the point-of-definition
in this way removes the function hijacking problem.

Then, the expectation would be that functions that need the calling
context to provide functions for use during generic instantiation would
either:

 1. Meet the strict requirements above (e.g. public, none defined at
    point of definiton)
 2. Use ``implements`` clauses to explicitly provide the functions
    to the generic function - see CHIP 2.
 3. Require these dependencies as first-class function arguments.

Implications
++++++++++++

The ``<`` function is still passable to the ``Sort`` module, including when
multiple ``<`` functions are declared at different scopes at the point of
instantiation.

The caching strategy for generic instantiations would need to be improved
to take into account scoping in any case where the generic function
relied on the point-of-instantiation rule.

Under this proposal, the existing caching strategy for generic
instantiations is sufficient, because it's not possible to have more than
one function.

Potential Alternatives
++++++++++++++++++++++

Once CHIP 2 is implemented, we could move to always using
point-of-definition and using ``implements`` to pass around function
requirements.

If first-class functions support gets re-implemented, generic functions
which today rely on point-of-instantiation would be able to explicitly
take in the functions they rely on that aren't necessarily visible at their
definition point.  In that situation, we could also move to always using
point-of-definition.
