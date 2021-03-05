.. _readme-operator-methods:

.. default-domain:: chpl

================
Operator Methods
================

This README describes support in the Chapel compiler for defining operators
as methods on types.  These features are still in the process of being improved.
This README is intended to cover the expected behavior, but the expected
behavior may change as experience with the feature is gained.

Defining Operators
------------------

Operators may be overloaded to support new behavior on one or more types using
the `operator` keyword.  Such overloads may be defined as stand-alone functions,
e.g.

.. code-block:: chapel

   operator +(lhs: t1, rhs: t2) { ... }

or as methods defined on a particular type.  Operator methods are equivalent to
type methods - their behavior is not tied to a particular instance of the type
with which they are associated.

Operator methods may be defined as primary, secondary, or tertiary methods.  For
example, the following code defines a primary ``+`` operator and a secondary
``-`` operator on a record:

.. code-block:: chapel

   record R {
     var intField: int;

     operator +(lhs: R, rhs: R) {
       return new R(lhs.intField + rhs.intField);
     }
   }

   operator R.-(lhs: R, rhs: R) {
     return new R(lhs.intField - rhs.intField);
   }


.. warning::

   Standalone operator overloads defined without the ``operator`` keyword are
   expected to be deprecated soon.  Operator methods may not be defined without
   the ``operator`` keyword.

Operator methods are not required to be defined on the same type as a particular
argument or any argument to the function.
