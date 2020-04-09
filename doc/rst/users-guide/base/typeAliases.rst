:title: Users Guide: Type Aliases

.. _ug-typeAliases:

Type Aliases
============

In addition to :ref:`declaring variables <ug-vars>` using ``var`` and
:ref:`constants <ug-constParam>` using ``const`` and ``param``, Chapel
supports the declaration of *type aliases* using the ``type`` keyword.
These type aliases are used to create new names for existing types—in
particular, they do not create new [sub]types.

As an example of a simple type alias, consider the following
statement:

.. literalinclude:: examples/users-guide/base/typeAliases.chpl
  :caption:
  :language: chapel
  :lines: 1

This declaration says to create a type alias named *age* which is
equivalent to the ``uint(8)`` type.  Imagine that we'll use this type
to store peoples' ages, since they tend not to live more than 256
years.  We can then use this alias like any other type.  For example,
the following statements use it to declare some variables and to
declare the formal type of a procedure argument:

.. literalinclude:: examples/users-guide/base/typeAliases.chpl
  :language: chapel
  :lines: 2-9

Type aliases like *age* are useful for:

* **creating self-documenting code:** Using a type alias like *age*
  provides more information about a variable than directly using the
  ``uint(8)`` type would.

* **minimizing the number of code changes required to represent a
  certain class of values:** If we decided that we wanted to represent
  other plants and animals in addition to people, we'd probably want
  to redefine *age* to be a larger integer size in order to store the
  ages of long-lived organisms like giant redwoods.  It's easier to do
  this by changing a single type alias's definition than by finding
  all references to ``uint(8)`` and determining whether or not they
  represent ages.

* **providing a shorthand for a longer type name:** Typing *age* saves
  keystrokes as compared to typing ``uint(8)``.

Like the other declaration forms we've seen so far, the ``type``
keyword can be used to declare multiple aliases by comma-separating
them.  For example, the following statement creates *float* and
*double* as aliases to Chapel's ``real(32)`` and ``real(64)`` types:

.. literalinclude:: examples/users-guide/base/typeAliases.chpl
  :language: chapel
  :lines: 14-15

Type aliases can be used not just for simple scalar types like
integers and floating point values, but for any Chapel type
expression.  Though we haven't introduced more complex types yet, the
following statements create aliases corresponding to a 3-tuple and
100-element array of ``real``:

.. literalinclude:: examples/users-guide/base/typeAliases.chpl
  :language: chapel
  :lines: 23-24
