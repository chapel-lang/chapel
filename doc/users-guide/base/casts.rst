:title: Users Guide: Casts: Explicit Type Conversions

.. _ug-casts:

Casts: explicit type conversions
================================

*Casting* refers to explicitly requesting that an expression be
converted to a given type.  In Chapel, casts are specified by applying
the colon operator (``:``) to an expression, followed by the type into
which it should be converted.  Note that casts use the same syntax as
type specifications in declarations, but occur in execution contexts.

As an example, given the following declarations of a string and an
integer:

.. literalinclude:: examples/users-guide/base/casting.chpl
  :caption:
  :language: chapel
  :lines: 2-3

The following code casts the string to an ``int``, storing it in
*myInt*.  It then increments *myInt* and casts it back to a string,
storing it in *myString*:

.. literalinclude:: examples/users-guide/base/casting.chpl
  :language: chapel
  :lines: 5-7

Of course, casts can also be applied to more complex expressions, so
we could have simply written the above as:

.. literalinclude:: examples/users-guide/base/casting.chpl
  :language: chapel
  :lines: 16

In either case, printing out *myString* results in:

.. literalinclude:: examples/users-guide/base/casting.good
  :language: text
  :lines: 1
