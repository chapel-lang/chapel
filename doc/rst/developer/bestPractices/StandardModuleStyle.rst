Standard Module Style
=====================

This document describes general guidance for style when writing a
standard module. Of course there will be exceptions to this guidance when
there is a good reason.

PascalCase and camelCase
------------------------

Prefer PascalCase or camelCase to separating_with_underscores. The rest
of this document will use PascalCase or camelCase to signify the pattern
of indicating new words with an upper case letter and starting the
identifier; where PascalCase starts the name with an upper case letter
and camelCase starts with a lower case letter.

Modules
-------

Module names should be PascalCase.

.. note::

  Chapel does not use a separate namespace for types vs. module names. As
  a result, it's generally a goal to avoid using the same name for, say,
  a type and a module.

Classes
-------

Class type names should be PascalCase. The idea is that starting with an
uppercase letter is the convention for by-reference types.

Records
-------

Record type names should be camelCase. The idea is that starting with a
lowercase letter is the convention for by-value types.

Enums
-----

Enum type names should be camelCase. The enum values should also be
camelCase.

Functions and Methods
---------------------

Function and method names should be camelCase.

Generally speaking, it's desireable to use methods on a value (vs
functions that aren't methods) when there is a clear type responsible for
the operation.

Use parentheses-less methods only for returning properties that could be
reasonably implemented as fields. However, if such a method is named
named isXYZ or hasXYZ it should use parentheses (so, use
`proc isReal() { ...  }` rather than `proc isReal { ... }`).
Parentheses-less functions that aren't methods should be avoided.

Many paren-ful methods take some notable action. Try to make these
methods method names be a verb. In particular, a method that modifies an
argument in-place should be a verb.

Other Identifiers
-----------------

Variables, fields, and argument names should be camelCase or CamelCase.
