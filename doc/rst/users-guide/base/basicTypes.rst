:title: Users Guide: Basic Types

.. _ug-basicTypes:

Basic Types: booleans, numbers, and strings
===========================================

Chapel supports a variety of basic types to represent logical values,
numbers, and strings.  Specifically, these types provide support for:

* Boolean values;
* Signed and unsigned integers;
* Real, imaginary, and complex floating point values; and
* Strings.

These types, along with the ``void`` type (not covered in this
section), are known as Chapel's *primitive types*.  In previous
sections, we've seen simple uses of string and integer types and
values.


Bit-Widths
----------

Chapel's boolean and numeric types are each supported in a variety of
*bit-widths* which specify how much memory to use for their
representation.  For the numeric types, it also governs the
range/accuracy of the values that the type can represent.

For example, ``int(8)`` specifies an 8-bit integer, capable of storing
values from -128 through 127, while ``int(64)`` specifies a 64-bit
integer, capable of storing values from -9,223,372,036,854,775,808
(-2\ :sup:`63`) through 9,223,372,036,854,775,807 (2\ :sup:`63` - 1).

These types also have a default size that is used when no bit-width is
specified.  For example, integers default to 64 bits, permitting
``int(64)`` to simply be written as ``int``, as in previous examples.

Since Boolean values only require a single bit of data, the use of
different bit-widths for the ``bool`` type only affects memory usage
and layout, not program semantics.  The default bit-width for the
``bool`` type is implementation-dependent.


Basic Type Summary
------------------

The following table summarizes each of the basic types, indicating its
role and default value, along with its supported bit widths and
default bit width, when applicable.

+-------------+--------------------------------+---------------+------------------+--------------------------+
| Type        | Description                    | Default Value | Legal Bit-Widths | Default Bit-Width        |
+=============+================================+===============+==================+==========================+
| ``bool``    | logical value                  | false         | 8, 16, 32, 64    | implementation-dependent |
+-------------+--------------------------------+---------------+------------------+--------------------------+
| ``int``     | signed integer value           | 0             | 8, 16, 32, 64    |  64                      |
+-------------+--------------------------------+---------------+------------------+--------------------------+
| ``uint``    | unsigned integer value         | 0             | 8, 16, 32, 64    |  64                      |
+-------------+--------------------------------+---------------+------------------+--------------------------+
| ``real``    | real floating-point value      | 0.0           | 32, 64           |  64                      |
+-------------+--------------------------------+---------------+------------------+--------------------------+
| ``imag``    | imaginary floating-point value | 0.0i          | 32, 64           |  64                      |
+-------------+--------------------------------+---------------+------------------+--------------------------+
| ``complex`` | complex floating-point value   | 0.0 + 0.0i    | 64, 128          | 128                      |
+-------------+--------------------------------+---------------+------------------+--------------------------+
| ``string``  | character string               | ""            | N/A              | N/A                      |
+-------------+--------------------------------+---------------+------------------+--------------------------+

