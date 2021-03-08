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
the ``operator`` keyword.  Such overloads may be defined as standalone
functions, e.g.

.. code-block:: chapel

   operator +(lhs: t1, rhs: t2) { ... }

or as methods defined on a particular type, e.g.

.. code-block:: chapel

   record R {
     var intField: int;

     operator +(lhs: R, rhs: R) {
       return new R(lhs.intField + rhs.intField);
     }
   }

This README will focus on operator methods.

Operator methods are equivalent to type methods - the type on which the operator
is declared causes the operator to have method-like visibility for that type,
and the ``this`` receiver is a ``type``.

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


The method receiver for an operator method will be used to determine when that
operator is visible.  This behavior is most useful when the method receiver
matches the type of at least one of the other arguments to the operator.
However, it is possible to define an operator method where the receiver type
does not match the type for any other argument.

Operator methods can be defined on concrete types, generic types, or particular
instantiations of generic types.

Calling Operator Methods
------------------------

A call to an operator - such as ``a + b`` which calls ``+`` - may resolve to any
visible operator method or standalone operator function.

Operator Method Visibility
--------------------------

Primary and secondary operator methods have similar visibility to other primary
and secondary methods.  In both cases, these methods can be viewed as part of
the type and will be available along with the type.  For regular methods, the
compiler searches for the method using the receiver's type (e.g. ``R`` in
``myR.method()`` supposing ``myR`` has type ``R``) definition point as well as
any type definition points for parent classes.  However, operator invocations
(such as ``a + b``) don't have a method receiver in the same way.  Instead, the
compiler uses te types of all the operator's arguments to find operator methods
defined along with the type.

As with tertiary methods, ``import`` and ``use`` statements can be used to
control the visibility of tertiary operator methods.

Determining Operator Candidate Functions
----------------------------------------

When determining if an operator method or function is an appropriate candidate,
only the arguments to the operator method or function will be considered.  The
presence or absence of a type receiver is only used to determine visibility, and
it will not eliminate an overload from candidate consideration.

Determining More Specific Operators
-----------------------------------

When determining which operator method or function is more specific, only the
arguments to the operator method or function will be considered.  The presence
or absence of a type receiver is only used to determine visibility and does not
impact the process of determining the best function (see
:ref:`Determining_Best_Functions`).
