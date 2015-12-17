:title: Users Guide: Basic Values (Literals)

Basic Values (Literals)
=======================

Most of the basic types support a corresponding literal format for
specifying values of that type.

* The ``bool`` type supports ``true`` and ``false`` as its two literal
  values.

* The ``int`` type supports base-10 literals via a sequence of decimal
  digits.  It also supports binary, octal, and hexidecimal literals
  via the ``0b``, ``0o``, and ``0x`` prefixes, respectively.  Note
  that there are no negative integer literals, simply applications of
  the unary negation operator (``-``) to integer literals.

* The ``uint`` type does not have a specific literal format, although
  integer literals which are too large for an ``int(64)`` yet fit
  within a ``uint(64)`` are considered to be unsigned integers.

* The ``real`` type's literal format includes both decimal and
  exponential formats.  Unlike some languages, ``5.`` is not a legal
  real value and must be written ``5.0`` due to a parsing ambiguity
  stemming from Chapel's ability to invoke methods on literal values.
  Floating point values may also be specified using a hexidecimal
  floating point format using the ``0x`` prefix.

* The ``imag`` type's literal format is similar to those used for
  ``int`` and ``real`` yet followed by ``i`` to indicate an imaginary
  value.

* The ``complex`` type does not support a literal format.  Complex
  values are most often expressed by adding or subtracting ``real``
  and ``imag`` literals.

* String literals are expressed using either single or double quotes.  
