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

or as methods defined on a particular type.  Operator methods are equivalent to
type methods - their behavior is not tied to a particular instance of the type
with which they are associated unless such an instance is provided as one of the
arguments to the method.

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

There is no required relationship between the argument types and the method
receiver.  An operator method can be associated with a type that matches the
type of every argument, or that matches the type of only some of the
arguments, or even a type that does not match the type of any argument (though
the latter case may be of limited value).

Operator methods are not required to be defined on concrete types.  They may be
defined on generic types, or on particular instantiations of generic types.

Calling Operator Methods
------------------------

An operator method call is identical to a call to a standalone operator
overload.  The compiler will distinguish which operator overload is most
appropriate and adjust the call accordingly, if necessary.

Operator Method Visibility
--------------------------

The visibility of operator methods is the same as the visibility of other
methods defined on the type.  The type definition point and any inherited type
definition points of each argument will be searched for operator methods defined
on it, and ``import`` and ``use`` statements can be used to control the
visibility of tertiary operator methods.

Determining Operator Candidate Functions
----------------------------------------

When determining if an operator method or function is an appropriate candidate,
only the arguments to the operator method or function will be considered.  The
presence or absence of a type receiver is only used to determine visibility, it
will not eliminate an overload from candidate consideration.

Determining More Specific Operators
-----------------------------------

When determining which operator method or function is more specific, only the
arguments to the operator method or function will be considered.  The presence
or absence of a type receiver is only used to determine visibility, it will not
make a particular operator method or function be deemed more specific than
another.
