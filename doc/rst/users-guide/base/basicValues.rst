:title: Users Guide: Literal Values for Basic Types

Literal Values for Basic Types
==============================

Most of Chapel's :ref:`basic types <ug-basicTypes>` support a
corresponding literal format for specifying their values at the
default bit-width.  Values of other bit-widths are obtained via type
conversions (:ref:`casts <ug-casts>` or coercions).

.. TODO: hyperlink coercions when those sections come online


Boolean Values
--------------

The ``bool`` type supports ``true`` and ``false`` as its two literal
values:

.. literalinclude:: examples/users-guide/base/boolValues.chpl
  :caption:
  :language: chapel
  :lines: 1,4,6

Integral Values
---------------
            
Literal values for ``int`` types are typically written as a sequence
of decimal digits:

.. literalinclude:: examples/users-guide/base/intValues.chpl
  :caption:
  :language: chapel
  :lines: 1,5

However, they can also be expressed as binary, octal, or hexadecimal
values via the ``0b``/``0B``, ``0o``/``0O``, and ``0x``/``0X``
prefixes, respectively:

.. literalinclude:: examples/users-guide/base/intValues.chpl
  :language: chapel
  :lines: 9,11,13


where these values correspond to decimal values:

.. literalinclude:: examples/users-guide/base/intValues.good
  :language: text
  :lines: 5-7

It's worth noting that in Chapel, there are no negative integer
literal values, simply applications of the unary negation operator
(``-``) to positive integer literals.

The ``uint`` type does not have its own literal format.  However,
integer literals that do not fit in an ``int(64)`` yet do fit within
a ``uint(64)`` are considered to be unsigned integer values.  Thus, in
the following declarations, the first variable is inferred to be of
type ``int`` while the second is inferred to be of type ``uint``:

.. literalinclude:: examples/users-guide/base/inferredIntValues.chpl
  :caption:
  :language: chapel
  :lines: 1-2

To represent a smaller integral value as a ``uint``, type conversions
(:ref:`casts <ug-casts>` or coercions) must be used.

.. TODO: hyperlink coercions above once that text is written
   

Floating Point Values
---------------------

The ``real`` type supports literal values via either decimal or
exponential formats:

.. literalinclude:: examples/users-guide/base/floatValues.chpl
  :caption:
  :language: chapel
  :lines: 3,6,8

where these literals correspond to the values:

.. literalinclude:: examples/users-guide/base/floatValues.good
  :language: text
  :lines: 3-4
          
In the decimal form, a ``.`` must be used to distinguish the value
from an integral literal.  Note that ``10.`` is not a valid floating
point literal value in Chapel due to a syntactic ambiguity with making
a method call on an integral value.  For this reason, ``10.0`` must be
used instead.

Note that in the exponential form, the base decimal value can be an
integer value and/or capital ``E`` may be used to set off the
exponent.  Thus, the values in the following four assignments are all
equivalent:

.. literalinclude:: examples/users-guide/base/floatValues.chpl
  :language: chapel
  :lines: 10,12,14,16

Floating point values may also be specified using a hexadecimal format
using the ``0x`` prefix.  In such cases, ``p`` is used to indicate the
exponent (avoiding ambiguity with the hexadecimal digit ``e``) and
indicates a power of 2 rather than 10:

.. literalinclude:: examples/users-guide/base/floatValues.chpl
  :language: chapel
  :lines: 18, 20

Thus, these expressions correspond to the floating point values:

.. literalinclude:: examples/users-guide/base/floatValues.good
  :language: text
  :lines: 9-10

Literal values for the ``imag`` type are identical to those for
``int`` and ``real``, yet with an ``i`` suffix.  Thus, the following
assignments demonstrate imaginary literals:

.. literalinclude:: examples/users-guide/base/floatValues.chpl
  :language: chapel
  :lines: 26,29,31,33,35
     
Note that whether using the ``int`` or ``real`` form, imaginary values
are stored using a floating point representation.  Thus, these
expressions correspond to the values:

.. literalinclude:: examples/users-guide/base/floatValues.good
  :language: text
  :lines: 14-17

Note that the standalone expression ``i`` does not refer to the
mathematical value *i*, but rather to a Chapel symbol (e.g., variable,
procedure, etc.) named *i*.  To get the mathematical value, use an
expression like ``1i`` or ``1.0i``.

The ``complex`` type does not support a native format for literal
values.  Instead, complex values are typically expressed by adding or
subtracting ``real`` and ``imag`` values.  For example:

.. literalinclude:: examples/users-guide/base/floatValues.chpl
  :language: chapel
  :lines: 41,44


String Literals
---------------

Literal ``string`` values can be expressed using either single or
double quotes.  For example, the following represent legal string
values:

.. literalinclude:: examples/users-guide/base/stringValues.chpl
  :language: chapel
  :lines: 1,4,6

One impact of this is that the other type of quotes can be used within
a string literal without additional effort:

.. literalinclude:: examples/users-guide/base/stringValues.chpl
  :language: chapel
  :lines: 9

Subsequent sections will cover strings in more detail.

.. TODO: write such a section and hyperlink it


   
