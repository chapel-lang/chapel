/*
 * Copyright 2004-2017 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*


Support for formatted input and output.


.. _about-io-formatted-io:

Formatted I/O
-------------

See below for a :ref:`sample-based overview <about-io-formatted-io-overview>`
of the format strings. Further below, we describes the format string syntax
:ref:`in detail <about-io-formatted-io-in-detail>`. Finally, we demonstrate the
functionality with :ref:`example function calls
<about-io-formatted-io-examples>`.

.. _about-io-formatted-io-overview:

Overview of Format Strings
++++++++++++++++++++++++++

In a manner similar to C's 'printf' and 'scanf', the IO package includes
:proc:`channel.writef` and :proc:`channel.readf` functions. These functions take
in a format string and some arguments. The :proc:`string.format` method is also
available and is loosely equivalent to C's 'sprintf'. For example, one might do:

.. code-block:: chapel

  writef("My favorite %s is %i\n", "number", 7);

  var s:string = "My favorite %s is %i".format("number", 7);
  writeln(s);

  // prints:
  // My favorite number is 7
  // My favorite number is 7

The following sections offer a tour through the conversions to illustrate the
common cases. A more precise definition follows in the "Format String
Syntax in Detail" section below.

In this file, we use "integral" to refer to the Chapel types int or uint and
"floating-point" to refer to real, imaginary, or complex, of any bit width.

Formatted I/O for C Programmers
+++++++++++++++++++++++++++++++

This table is designed to help C programmers understand the equivalent
Chapel format specifiers.

========  ===========  ==========================================
C         Chapel       Meaning
========  ===========  ==========================================
%i        %i           an integer in decimal
%d        %i           an integer in decimal
%u        %u           an unsigned integer in decimal
%x        %xu          an unsigned integer in hexadecimal
%g        %r           real number in exponential or decimal (if compact)
%7.2g     %7.2r        real, 2 significant digits, padded to 7 columns
%f        %dr          real number always in decimal
%7.3f     %7.3dr       real, 3 digits after ``.``, padded to 7 columns
%e        %er          real number always in exponential
%7.3e     %7.3er       real, 3 digits after ``.``, padded to 7 columns
%s        %s           a string without any quoting
========  ===========  ==========================================

Unlike in C, a value of the wrong type will be cast appropriately - so for
example printing 2 (an ``int``)  with ``%.2dr`` will result in ``2.00``.  Note
that ``%n`` and ``%t`` are equivalent to ``%r`` for real conversions and ``%i``
for numeric conversions; so these are also equivalent to ``%i`` ``%d`` or
``%g`` in C. Also note that Chapel format strings includes many capabilities
not available with C formatted I/O routines - including quoted strings,
binary numbers, complex numbers, and raw binary I/O.

Generic Numeric Conversions
+++++++++++++++++++++++++++

``%{##.###}``
  decimal number padded on the left with spaces to 2 digits before
  the point, rounded to 3 after. Works with integral, real, imaginary,
  or complex arguments.

  In all cases, the output is padded on the left to the total length
  of the conversion specifier (6 in this example).  The output
  can be longer, when needed to accommodate the number.

``%{##}``
  integral value padded out to 2 digits. Also works with real, imaginary
  or complex numbers by rounding them to integers. Numbers with more
  digits will take up more space instead of being truncated.

In both conversions above, an imaginary argument gets an 'i' afterwards
and the entire expression is padded out to the width of ##### digits.
For example:

.. code-block:: chapel

  writef("|${#####}|\n", 2.0i);
       // outputs:
       //   |   2i|

  writef("|%{#####.#}|\n", 2.0i);
       // outputs:
       //   |   2.0i|

Complex arguments are printed in the format a + bi, where each of a and b is
rounded individually as if printed under that conversion on its own. Then, the
formatted complex number is padded to the requested size. For example:

.. code-block:: chapel

  writef("|%{#########}|\n", 1.0+2.0i);
       // outputs:
       //   |   1 + 2i|

  writef("|%{#########.#}|\n", 1.0+2.0i);
       // outputs:
       //   | 1.0 + 2.0i|

See :ref:`about-io-formatted-pound-details` for more details
on this conversion type.

``%n``
  a "number" - equivalent to one of %i, %u, %r, %m, or %z below,
  depending on the type

``%17n``
  a number padded out to 17 columns

``%.4n``
  a number with 4 significant digits or a precision of 4

Integral Conversions
++++++++++++++++++++

``%i`` or ``%di``
  a signed integer in decimal, possibly negative
  (note - when reading an ``%i``, ``-`` is allowed)
``%u`` or ``%du``
  an unsigned decimal integer
  (note - when reading a ``%u``, ``-`` is not allowed)
``%bi``
 a binary signed integer
``%bu``
 a binary unsigned integer
``%@bu``
 a binary unsigned integer prefixed with ``0b``
``%oi``
 an octal signed integer
``%ou``
 an octal unsigned integer
``%@ou``
 an octal unsigned integer prefixed with ``0o``
``%xu``
 a hexadecimal unsigned integer
``%xi``
 a hexadecimal signed integer
``%@xu``
 a hexadecimal unsigned integer prefixed with ``0x``
``%Xu``
 a hexadecimal unsigned integer in uppercase
``%@Xu``
 a hexadecimal unsigned integer prefixed with ``0X`` and uppercase
``%17i``
 a decimal integer padded on the left with spaces to 17 columns
 (That is, it is right-justified in a 17-column field.
 Padding width is ignored when reading integers)
``%*i``
 as with ``%17i`` but read the minimum width from the preceding argument
``%017i``
 a decimal integer padded on the left with zeros to 17 columns
``%-17i``
 a decimal integer left-justified (padded on the right) to 17 columns
``%+i``
 a decimal integer showing ``+`` for positive numbers
``% i``
 a decimal integer with a space for positive numbers
``%|4i``
 output 4 raw, binary bytes of the passed integer in native endianness
``%<4i``
 output 4 raw, binary bytes of the passed integer little endian
``%>4i``
 output 4 raw, binary bytes of the passed integer big endian
``%<8i``
 output 8 raw, binary bytes of the passed integer little endian
 (byte widths of 1, 2, 4, and 8 are supported for integral conversions)

Real Conversions
++++++++++++++++

``%r``
 a real number with decimal or exponential notation, where
 exponential is chosen if the decimal version would be too long

``%6r``
 as with ``%r`` but padded on the left to 6 columns (ie right-justified)
``%-6r``
 as with ``%r`` but padded on the right to 6 columns (ie left-justified)
``%.4r``
 as with ``%r`` but with 4 significant digits
``%.*r``
 as with ``%.4r`` but with significant digits read from preceding argument
``%6.4r``
 as with ``%r`` but padded on the left to 6 columns
 and with 4 significant digits
``%*.*r``
 as with ``%6.4r`` but read minimum width and significant digits from
 preceding arguments

``%dr``
 a real number in decimal notation, e.g. ``12.34``
``%6dr``
 a decimal number padded on the left to 6 columns (right-justified)
``%.4dr``
 a decimal number with 4 digits after the radix point
``%6.4dr``
 a decimal number padded on the left to 6 columns and with 4 digits
 after the radix point
 (width and precision are ignored when reading numbers in readf)

``%er``
 a real number in exponential notation, e.g. ``8.2e-23``
``%Er``
 like %er but with the 'e' in uppercase, e.g. ``8.2E-23``
``%.4er``
 exponential notation with 4 digits after the period, e.g. ``8.2000e-23``

``%xer``
 hexadecimal number using p to mark exponent e.g. ``6c.3f7p-2a``

``%|4r``
 emit 4 raw, binary bytes of the passed number in native endianness
``%<8r``
 emit 8 raw, binary bytes of the passed number in little endian
``%<4r``
 emit 4 raw, binary bytes of the passed number in little endian
 (``<`` ``|`` and ``>`` are supported for widths 4 or 8)

Complex and Imaginary Conversions
+++++++++++++++++++++++++++++++++

``%m``
 an imaginary number, like a real with ``%r`` but ends with an ``i``

``%z``
 print complex number with ``%r`` for each part in the format ``a + bi``
``%@z``
 print complex number with ``%r`` for each part in the format ``(a,b)``
``%6z``
 as with ``%z`` but pad the entire complex number out to 6 columns
``%6.4z``
 print a and b 4 significant digits and pad the entire complex
 number out to 6 columns
``%dz``
 print a and b with ``%dr``
``%ez``
 print a and b with ``%er``

``%|4m``
 same as ``%|4r``
``%|8z``
 emit 8 raw, binary bytes of native-endian complex (a,b are each 4 bytes)
``%<16z``
 emit 16 raw, binary bytes of little-endian complex (a,b each 8 bytes)

String and Bytes Conversions
++++++++++++++++++++++++++++

``%s``
 a string. When reading, read until whitespace.
 Note that if you want to be able to read your string back in,
 you should use one of the quoted or encoded binary versions (see below),
 since generally with %s it's not clear where the string ends.
``%c``
 a single Unicode character (argument should be a string or an integral
 storing the character code)
``%17s``
  * when writing - a string left padded (right justified) to 17 columns
  * when reading - read up to 17 bytes or a whitespace, whichever comes
    first, rounding down to whole characters
``%-17s``
 * when writing - a string right padded (left justified) to 17 columns
``%.17s``
 * when writing - a string truncated to 17 columns. When combined
   with quoting strings, for example ``%.17"S``, the conversion
   will print ... after a string if it was truncated. The
   truncation includes leaving room for the quotes and -
   if needed - the periods, so the shortest truncated
   string is ``""...``  Generally, you won't be able to read
   these back in.
 * when reading - read exactly 17 Unicode code points
``%|17s``
 * when writing - emit string but cause runtime error if length
   does not match
 * when reading - read exactly 17 bytes (error if we read < 17 bytes)
``%|*s``
  as with %17s but the length is specified in the argument before the string.
``%"S``
 use double-quotes to delimit string
``%'S``
 use single-quotes to delimit string
``%cS``
 use any character (c) to delimit string
``%{(S)}``
 quoted string, starting with ``(``, ending with ``)``, where the
 parens could be replaced by arbitrary characters
``%*S``
 quoted string, the arg before the string to specifies quote character
``%|0S``
 write a string null-terminated or read bytes until a null-terminator
``%|*S``
 means read bytes until a terminator byte. The terminator byte is read
 from the argument before the string.
``%|1S`` ``%|2S`` ``%|4S`` and ``%|8S``
  work with encoded strings storing a length
  and then the string data. The digit before ``S`` is
  the number of bytes of length which is by default
  stored native endian. ``<``, ``|``, ``>`` can be used
  to specify the endianness of the length field,
  for example ``%<8S`` is 8 bytes of little-endian length
  and then string data.
``%|vS``
 as with ``%|1S``-``%|8S`` but the string length is encoded using a
 variable-length byte scheme (which is always the same no matter what
 endianness). In this scheme, the high bit of each encoded length byte
 records whether or not there are more length bytes (and the remaining
 bits encode the length in a big-endian manner).

``%|*vS`` or ``%|*0S``
 read an encoded string but limit it to a number of bytes
 read from the argument before the string; when writing
 cause a runtime error if the string is longer than the
 maximum.

``%/a+/``
 where any regular expression can be used instead of ``a+``
 consume one or more 'a's when reading, gives an error when printing,
 and does not assign to any arguments
 (note - regular expression support is dependent on RE2 build;
 see :mod:`Regexp`)

``%/(a+)/``
 consume one or more 'a's and then set the corresponding string
 argument to the read value

``%17/a+/``
 match a regular expression up to 17 bytes
 (note that ``%.17/a+/``, which would mean to match 17 characters,
 is not supported).

``%/*/``
 next argument contains the regular expression to match

.. (comment) the above started a nested comment, so here we end it */

General Conversions
+++++++++++++++++++

``%t``
 read or write the object according to its readThis/writeThis routine
``%jt``
 read or write an object in JSON format using readThis/writeThis
``%ht``
 read or write an object in Chapel syntax using readThis/writeThis
``%|t``
 read or write an object in binary native-endian with readThis/writeThis
``%<t``
 read or write an object little-endian in binary with readThis/writeThis
``%>t``
 read or write an object big-endian in binary with readThis/writeThis

Note About Whitespace
+++++++++++++++++++++

When reading, ``\n`` in a format string matches any zero or more space
characters other than newline and then exactly one newline character. In
contrast, ``" "`` matches at least one space character of any kind.

When writing, whitespace is printed from the format string just like any
other literal would be.

Finally, space characters after a binary conversion will be ignored, so
that a binary format string can appear more readable.

.. _about-io-formatted-io-in-detail:

Format String Syntax in Detail
++++++++++++++++++++++++++++++

Chapel's format strings are simpler than those in C in one way: it is no longer
necessary to specify the types of the arguments in the format string. For
example, in C the l in %ld is specifying the type of the argument for integer
(decimal) conversion. That is not necessary in Chapel since the compiler is
able to use type information from the call.

Format strings in Chapel consist of:

 * conversion specifiers e.g. ``"%xi"`` (described below)
 * newline e.g. ``"\n"``

   * when writing - prints a newline
   * when reading - reads any amount of non-newline whitespace and then
     exactly one newline. Causes the format string not to
     match if it did not read a newline.

 * other whitespace e.g. ``" "``

    * when writing - prints as the specified whitespace
    * when reading - matches at least one character of whitespace, possibly
      including newlines.

 * other text e.g. "test"

    * when writing - prints the specified text
    * when reading - matches the specified text

.. _about-io-formatted-pound-details:

# Specifiers
++++++++++++

All # specifiers must be enclosed in ``%{}`` syntax, for example ``%{#}`` is the
shortest one, and ``%{#.#}`` is a more typical one. The integer portion of the
number will be padded out to match the number of ``#`` s before the decimal
point, and the number of ``#`` s after the decimal point indicate how many
digits to print after the decimal point. In other words, display how many
digits to use when printing a floating-point number by using the # symbol to
stand for digits. The fractional portion of the number will be rounded
appropriately and extra space will be made if the integer portion is too small:

.. code-block:: chapel

  writef("n:%{###.###}\n", 1.2349);
       // outputs:
       // n:  1.235

This syntax also works for numbers without a decimal point by rounding them
appropriately.

A # specifier may start with a ``.``.

.. code-block:: chapel

  writef("%{.##}\n", 0.777);
       // outputs:
       //  0.78

% Specifiers
++++++++++++

Specifiers starting with % offer quite a few options. First, some basic
rules.

``%%``
 means a literal ``%``
``\n``
 means a literal newline
``\\``
 means a single backslash
``%{}``
 curly braces can wrap a ``%`` or ``#`` conversion specifier. That way, even
 odd specifiers can be interpreted unambiguously. Some of the more complex
 features require the use of the ``%{}`` syntax, but it's always
 acceptable to use curly braces to make the format string clearer.
 Curly braces are required for # conversion specifiers.

In general, a ``%`` specifier consists of either text or binary conversions:

::

 %
 [optional endian flag (binary conversions only)]
 [optional flags]
 [optional field width or size in bytes]
 [optional . then precision]
 [optional base flag]
 [optional exponential type]
 [conversion type]

Going through each section for text conversions:


[optional flags]
  ``@``
   means "alternate form". It means to print out a base when not using
   decimal (e.g. ``0xFFF`` or ``0b101011``); and it will format a complex
   number with parens instead of as e.g. ``1.0+2.0i``
  ``+``
   means to show a plus sign when printing positive numbers
  ``0``
   means to pad numeric conversions with 0 instead of space
  ``" "``
   (a space) leaves a blank before a positive number
   (in order to help line up with negative numbers)
  ``-``
   left-justify the converted value instead of right-justifying.
   Note, if both ``0`` and ``-`` are given, the effect is as if only ``-``
   were given.
  ``~``
   when reading a record or class instance, skip over fields in the input not
   present in the Chapel type. This flag currently only works in combination
   with the JSON format.  This flag allows a Chapel program to describe only the
   relevant fields in a record when the input might contain many more fields.


[optional field width]
   When printing numeric or string values, the field width specifies the number
   of *columns* that the conversion should use to display the value. It can be
   ``*``, which means to read the field width from an integral argument before
   the converted value.

   For string conversions in readf (``%s`` ``%"`` ``%'`` ``%//``), the field
   width specifies the maximum number of bytes to read.

   For numeric conversions in readf, the field width is ignored.

[optional . then precision]
   When printing floating point values, the precision is used to control
   the number of decimal digits to print.  For ``%r`` conversions, it
   specifies the number of significant digits to print; for ``%dr`` or ``%er``
   conversions, it specifies the number of digits following the decimal point.
   It can also be ``*``, which means to read the precision from an integral
   argument before the converted value.

   For textual string conversions in writef, (``%s`` ``%"`` ``%'``), the
   precision indicates the maximum number of columns to print - and the result
   will be truncated if it does not fit. In readf for these textual string
   conversions, the precision indicates the maximum number of characters
   (e.g. Unicode code points) to input.

   The precision is silently ignored for integral conversions
   (``%i``, ``%u``, etc) and for ``%//`` conversions.

[optional base flag]
   ``d``
    means decimal (and not exponential for floating-point)
   ``x``
    means lower-case hexadecimal
   ``X``
    means upper-case hexadecimal
   ``o``
    means octal
   ``b``
    means binary
   ``j``
    means JSON-style strings, numbers, and structures
   ``h``
    means Chapel-style strings, numbers, and structures
   ``'``
    means single-quoted string (with \\ and \')
   ``"``
    means double-quoted string (with \\ and \")

[optional exponential type]
   ``e``
    means floating-point conversion printing exponential ``-12.34e+56``
   ``E``
    means floating-point conversion printing uppercase
    exponential ``-12.34E+56``

[conversion type]
   ``t``
    means *type-based* or *thing* - uses writeThis/readThis but ignores
    width and precision
   ``n``
    means type-based number, allowing width and precision
   ``i``
    means integral conversion
   ``u``
    means unsigned integral conversion
   ``r``
    means real conversion (e.g. ``12.23``)
   ``m``
    means imaginary conversion with an ``i`` after it (e.g. ``12.23i``)
   ``z``
    means complex conversion
   ``s``
    means string conversion
   ``S``
    means a quoted string conversion
   ``{cS}``
    means string conversion with quote char *c*
   ``{*S}``
    means string conversion with quote char in argument before the string
   ``{xSy}``
    means string conversion with left and right quote chars *x* and *y*
   ``/.../``
    means a regular expression (for reading only)
   ``{/.../xyz}``
    means regular expression with flags *xyz*
   ``c``
    means a Unicode character - either the first character in a string
    or an integral character code

For binary conversions:

[optional endian flag]
   ``<``
    means little-endian
   ``>``
    means big-endian
   ``|``
    means native-endian

[optional size in bytes]
   This is the number of bytes the format should read or write in this
   conversion. For integral conversions (e.g. ``%|i``) it specifies the number
   of bytes in the integer, and 1, 2, 4, and 8 are supported. For real and
   imaginary conversions, 4 and 8 are supported. For complex conversions,
   8 and 16 are supported. The size in bytes is *required* for binary
   integral and floating-point conversions.

   The size can be ``*``, which means that the number of bytes is read
   from the argument before the conversion.

   For strings, if a terminator or length field is specified, exactly this
   number is the maximum size in bytes; if the terminator or length is not
   specified, the string must be exactly that size (and if the argument is not
   exactly that number of bytes it will cause an error even when writing).

[conversion type]
   ``t``
    means *type-based* or *thing* - to read or write with readThis/writeThis
   ``n``
    means type-based number (size is not mandatory)
   ``i``
    means integral. Note that the size is mandatory for binary integral
    conversions
   ``u``
    means unsigned integral. Note that the size is mandatory for binary
    integral conversions
   ``r``
    means real. Note that the size is mandatory for binary real conversions
   ``m``
    works the same as ``r`` for binary conversions
   ``z``
    means complex. Note that the size is mandatory for binary complex
    conversions
   ``s``
    * means string binary I/O
    * ``%|17s`` means exactly 17 byte string
   ``0S``/``1S``/``2S``/``4S``/``8S``
    * mean encoded string binary I/O:
    * ``%|0S`` means null-terminated string
    * ``%{|S*}`` means  next-argument specifies string terminator byte
    * ``%|1S`` means a one-byte length and then the string
    * ``%|2S`` means a two-byte length and then the string
    * ``%|4S`` means a four-byte length and then the string
    * ``%|8S`` means an eight-byte length and then the string
    * ``%|vS`` means a variable-byte-encoded length and then the string
   ``c``
    means a Unicode character - either the first character in a string
    or an integral character code


.. _about-io-formatted-io-examples:

Formatted I/O Examples
++++++++++++++++++++++

.. code-block:: chapel

  writef("%5i %5s %5r\n", 1, "test", 6.34);
       // outputs:
       //    1  test  6.34

  writef("%2.4z\n", 43.291 + 279.112i);
       // outputs:
       // 43.29 + 279.1i

  writef("%<4u", 0x11223344);
       // outputs:
       // (hexdump of the output)
       // 4433 2211
  writef("%>4u", 0x11223344);
       // outputs:
       // (hexdump of the output)
       // 1122 3344
  writef("%<4i %<4i", 2, 32);
       // outputs:
       // (hexdump of the output -- note that spaces after
       //  a binary format specifier are ignored)
       // 0200 0000 2000 0000


  writef("%|0S\n", "test");
       // outputs:
       // (hexdump of the output)
       // 7465 7374 000a
  writef("%|1S\n", "test");
       // outputs:
       // (hexdump of the output)
       // 0474 6573 740a
  writef("%>2S\n", "test");
       // outputs:
       // (hexdump of the output)
       // 0004 7465 7374 0a
  writef("%>4S\n", "test");
       // outputs:
       // (hexdump of the output)
       // 0000 0004 7465 7374 0a
  writef("%>8S\n", "test");
       // outputs:
       // (hexdump of the output)
       // 0000 0000 0000 0004 7465 7374 0a
  writef("%|vS\n", "test");
       // outputs:
       // (hexdump of the output)
       // 04 7465 7374 0a

  writef('%"S\n', "test \"\" \'\' !");
       // outputs:
       // "test \"\" '' !"
  writef("%'S\n", "test \"\" \'\' !");
       // outputs:
       // 'test "" \'\' !'
  writef("%{(S)}\n", "test ()", "(", ")");
       // outputs:
       // (test (\))


  writef("%40s|\n", "test");
  writef("%-40s|\n", "test");
       // outputs:
       //                                     test|
       // test                                    |

  writef("123456\n");
  writef("%6.6'S\n", "a");
  writef("%6.6'S\n", "abcdefg");
  writef("%.3'S\n", "a");
  writef("%.3'S\n", "abcd");
       // outputs:
       // 123456
       //    'a'
       // 'a'...
       // 'a'
       // ''...


  var s:string;
  var got = readf(" %c", s);
  // if the input is " a", "\na", "  a", etc, s will contain "a"
  // if the input is "b", got will be false and s will contain ""

  var s:string;
  var got = readf("\n%c", s);
  // if the input is "\na", or " \na", s will contain "a"
  // if the input is "b", got will be false and s will be ""

  var got = readf("%/a+/");
  // if the input is "a" or "aa" (and so on), got will return true
  // if the input is "c" got will be false

  var s:string;
  var got = readf("%/a(b+)/", s);
  // if the input is "c" got will be false and s will be ""
  // if the input is "ab", got will be true and s will be "b"
  // if the input is "abb", got will be true and s will be "bb"

FormattedIO Functions and Types
-------------------------------

 */
module FormattedIO {

use SysBasic;
use SysError;
use IO;

// ---------------------------------------------------------------
// ---------------------------------------------------------------
// Starting support for Formatted I/O
// ---------------------------------------------------------------
// ---------------------------------------------------------------

private inline
proc _toIntegral(x:?t) where isIntegralType(t)
{
  return (x, true);
}
private inline
proc _toIntegral(x:?t) where _isIoPrimitiveType(t) && !isIntegralType(t)
{
  return (x:int, true);
}
private inline
proc _toIntegral(x:?t) where !_isIoPrimitiveType(t)
{
  return (0, false);
}

private inline
proc _toSigned(x:?t) where isIntType(t)
{
  return (x, true);
}
private inline
proc _toSigned(x:uint(8))
{
  return (x:int(8), true);
}
private inline
proc _toSigned(x:uint(16))
{
  return (x:int(16), true);
}
private inline
proc _toSigned(x:uint(32))
{
  return (x:int(32), true);
}
private inline
proc _toSigned(x:uint(64))
{
  return (x:int(64), true);
}

private inline
proc _toSigned(x:?t) where _isIoPrimitiveType(t) && !isIntegralType(t)
{
  return (x:int, true);
}
private inline
proc _toSigned(x:?t) where !_isIoPrimitiveType(t)
{
  return (0:int, false);
}

private inline
proc _toUnsigned(x:?t) where isUintType(t)
{
  return (x, true);
}
private inline
proc _toUnsigned(x:int(8))
{
  return (x:uint(8), true);
}
private inline
proc _toUnsigned(x:int(16))
{
  return (x:uint(16), true);
}
private inline
proc _toUnsigned(x:int(32))
{
  return (x:uint(32), true);
}
private inline
proc _toUnsigned(x:int(64))
{
  return (x:uint(64), true);
}


private inline
proc _toUnsigned(x:?t) where _isIoPrimitiveType(t) && !isIntegralType(t)
{
  return (x:uint, true);
}
private inline
proc _toUnsigned(x:?t) where !_isIoPrimitiveType(t)
{
  return (0:uint, false);
}


private inline
proc _toReal(x:?t) where isRealType(t)
{
  return (x, true);
}
private inline
proc _toReal(x:?t) where _isIoPrimitiveType(t) && !isRealType(t)
{
  return (x:real, true);
}
private inline
proc _toReal(x:?t) where !_isIoPrimitiveType(t)
{
  return (0.0, false);
}

private inline
proc _toImag(x:?t) where isImagType(t)
{
  return (x, true);
}
private inline
proc _toImag(x:?t) where _isIoPrimitiveType(t) && !isImagType(t)
{
  return (x:imag, true);
}
private inline
proc _toImag(x:?t) where !_isIoPrimitiveType(t)
{
  return (0.0i, false);
}


private inline
proc _toComplex(x:?t) where isComplexType(t)
{
  return (x, true);
}
private inline
proc _toComplex(x:?t) where _isIoPrimitiveType(t) && !isComplexType(t)
{
  return (x:complex, true);
}
private inline
proc _toComplex(x:?t) where !_isIoPrimitiveType(t)
{
  return (0.0+0.0i, false);
}

private inline
proc _toRealOrComplex(x:?t) where isComplexType(t)
{
  return (x, true);
}
private inline
proc _toRealOrComplex(x:?t) where isFloatType(t)
{
  return (x, true);
}
private inline
proc _toRealOrComplex(x:?t) where _isIoPrimitiveType(t) && !isComplexType(t) && !isFloatType(t)
{
  return (x:real, true);
}
private inline
proc _toRealOrComplex(x:?t) where !_isIoPrimitiveType(t)
{
  return (0.0, false);
}

private inline
proc _toNumeric(x:?t) where isNumericType(t)
{
  return (x, true);
}
private inline
proc _toNumeric(x:?t) where _isIoPrimitiveType(t) && !isNumericType(t)
{
  // enums, bools get cast to int.
  return (x:int, true);
}
private inline
proc _toNumeric(x:?t) where !_isIoPrimitiveType(t)
{
  return (0, false);
}


private inline
proc _toString(x:?t) where t==string
{
  return (x, true);
}
private inline
proc _toString(x:?t) where (_isIoPrimitiveType(t) && t!=string)
{
  return (x:string, true);
}
private inline
proc _toString(x:?t) where !_isIoPrimitiveType(t)
{
  return ("", false);
}

private inline
proc _toChar(x:?t) where isIntegralType(t)
{
  return (x:int(32), true);
}
private inline
proc _toChar(x:?t) where t == string
{
  var chr:int(32);
  var nbytes:c_int;
  var local_x = x.localize();
  qio_decode_char_buf(chr, nbytes, local_x.c_str(), local_x.length:ssize_t);
  return (chr, true);
}
private inline
proc _toChar(x:?t) where !(t==string || isIntegralType(t))
{
  return (0:int(32), false);
}


// If we wanted to give ERANGE if (for example
// var x:int(8); readf("%i", x);
// was given the input 1000, this would be the place to do it.
private inline
proc _setIfPrimitive(ref lhs:?t, rhs:?t2, argi:int):syserr where t==bool&&_isIoPrimitiveType(t2)
{
  var empty:t2;
  if rhs == empty {
    lhs = false;
  } else {
    lhs = true;
  }
  return ENOERR;
}
private inline
proc _setIfPrimitive(ref lhs:?t, rhs:?t2, argi:int):syserr where t!=bool&&_isIoPrimitiveType(t)
{
  //stdout.writeln("setIfPrimitive ", lhs, " ", rhs);
  lhs = rhs:t;
  return ENOERR;
}
private inline
proc _setIfPrimitive(ref lhs:?t, rhs, argi:int):syserr where !_isIoPrimitiveType(t)
{
  return qio_format_error_arg_mismatch(argi);
}

private inline
proc _setIfChar(ref lhs:?t, rhs:int(32)) where t == string
{
  lhs = new ioChar(rhs):string;
}
private inline
proc _setIfChar(ref lhs:?t, rhs:int(32)) where isIntegralType(t)
{
  lhs = rhs:t;
}
private inline
proc _setIfChar(ref lhs:?t, rhs:int(32)) where !(t==string||isIntegralType(t))
{
  // do nothing
}



private inline
proc _toRegexp(x:?t) where t == regexp
{
  return (x, true);
}
private inline
proc _toRegexp(x:?t) where t != regexp
{
  var r:regexp;
  return (r, false);
}

//
// This is an internal use only debug flag, so use with caution.
// Specifically, writef() is not re-entrant, so enabling it may cause
// unexpected failures.
//
private config param _format_debug = false;

pragma "no doc"
class _channel_regexp_info {
  var hasRegexp = false;
  var matchedRegexp = false;
  var releaseRegexp = false;
  var theRegexp = qio_regexp_null();
  var matches: _ddata(qio_regexp_string_piece_t) = nil; // size = ncaptures+1
  var capArr: _ddata(string) = nil; // size = ncaptures
  var capturei: int;
  var ncaptures: int;
  proc clear() {
    if releaseRegexp {
      qio_regexp_release(theRegexp);
    }
    theRegexp = qio_regexp_null();
    hasRegexp = false;
    matchedRegexp = false;
    releaseRegexp = false;
    if matches then _ddata_free(matches);
    for i in 0..#ncaptures do capArr[i] = "";
    if capArr then _ddata_free(capArr);
  }
  proc allocate_captures() {
    ncaptures = qio_regexp_get_ncaptures(theRegexp);
    matches = _ddata_allocate(qio_regexp_string_piece_t, ncaptures+1);
    capArr = _ddata_allocate(string, ncaptures);
    capturei = 0;
  }
  proc deinit() {
    clear();
  }
}

pragma "no doc"
proc channel._match_regexp_if_needed(cur:size_t, len:size_t, ref error:syserr, ref style:iostyle, ref r:_channel_regexp_info)
{
  if _format_debug then stdout.writeln("REGEXP MATCH ENTRY");
  if qio_regexp_ok(r.theRegexp) {
    if r.matchedRegexp then return;
    if _format_debug then stdout.writeln("REGEXP MATCH");
    r.matchedRegexp = true;
    r.allocate_captures(); // also allocates matches and capArr
    var ncaps = r.ncaptures;
    var nm = ncaps + 1;
    var maxlen:int(64) = style.max_width_characters;
    // If we are working on the last part of the format string,
    // and there is a match, we can immediately discard
    // data before any captures in the match (or if there
    // are no captures - the entire match).
    var can_discard = (cur == len);
    if maxlen == max(uint(32)) then maxlen = max(int(64));
    var before_match = qio_channel_offset_unlocked(_channel_internal);
    // Do the actual regexp search.
    // Now read, matching the regexp.
    error = qio_regexp_channel_match(r.theRegexp, false, _channel_internal,
                                     maxlen, QIO_REGEXP_ANCHOR_START,
                                     can_discard,
                                     /* keep_unmatched */ true,
                                     /* keep_whole_pattern */ false,
                                     r.matches, nm);
    var after_match = qio_channel_offset_unlocked(_channel_internal);

    // Now, if there was no match, error=EFORMAT
    // if there was a match, error = no error
    // Either way, we have to handle the next several
    // arguments as capture groups.
    if ! error {
      for j in 0..#ncaps {
        // matches[0] is the whole pattern, and
        // we only want to extract capture groups.
        var m = _to_reMatch(r.matches[1+j]);
        _extractMatch(m, r.capArr[j], error);
        if error then break;
      }
      // And, advance the channel to the end of the match.
      var cur = qio_channel_offset_unlocked(_channel_internal);
      var target = r.matches[0].offset + r.matches[0].len;
      error = qio_channel_advance(false, _channel_internal, target - cur);
      if error {
        if _format_debug then stdout.writeln("ERROR AQB");
      }
    } else {
      // otherwise, clear out caps...
      for j in 0..#ncaps {
        r.capArr[j] = "";
      }
      // ... and put the channel before the match.
      var cur = qio_channel_offset_unlocked(_channel_internal);
      qio_channel_advance(false, _channel_internal, before_match - cur);
      // EFORMAT means the pattern did not match.
      if _format_debug then stdout.writeln("DEBUG AQZ");
    }
  } else {
    error = qio_format_error_bad_regexp();;
    if _format_debug then stdout.writeln("DEBUG AZB");
  }
}

// Reads the next format string that will require argument handling.
// Handles literals and regexps itself; everything else will
// be returned in conv and with gotConv = true.
// Assumes, for a reading channel, that we are within a mark/revert/commit
//  in readf. (used in the regexp handling here).
pragma "no doc"
proc channel._format_reader(
    fmt:c_string, ref cur:size_t, len:size_t, ref error:syserr,
    ref conv:qio_conv_t, ref gotConv:bool, ref style:iostyle,
    ref r:_channel_regexp_info,
    isReadf:bool)
{
  if _format_debug then stdout.writeln("FORMAT READER ENTRY");
  if r != nil then r.hasRegexp = false;
  if !error {
    while cur < len {
      gotConv = false;
      if error then break;
      if _format_debug then stdout.writeln("TOP OF LOOP cur=", cur, " len=", len);
      var end:uint(64);
      error = qio_conv_parse(fmt, cur, end, isReadf, conv, style);
      if error {
        if _format_debug then stdout.writeln("ERROR ACC");
      }
      cur = end:size_t;
      if error then break;
      if _format_debug then stdout.writeln("MIDDLE OF LOOP");
      if conv.argType == QIO_CONV_ARG_TYPE_NONE_LITERAL {
        // Print whitespace or I/O literal.
        // literal string in conv
        if isReadf {
          // Scan whitespace or I/O literal.
          // literal string in conv
          if conv.literal_is_whitespace == 2 {
            if _format_debug then stdout.writeln("NEWLINE");
            // Handle a \n newline in the format string.
            // Other space.
            var offsetA = qio_channel_offset_unlocked(_channel_internal);
            error = qio_channel_skip_past_newline(false, _channel_internal, true);
            var offsetB = qio_channel_offset_unlocked(_channel_internal);
            if (!error) && offsetA == offsetB {
              // didn't really read newline.
              error = EFORMAT;
            }
            if _format_debug then stdout.writeln("AFTER NEWLINE err is ", error:int);
          } else if conv.literal_is_whitespace == 1 {
            if _format_debug then stdout.writeln("WHITESPACE");
            // Other space.
            var offsetA = qio_channel_offset_unlocked(_channel_internal);
            error = qio_channel_scan_literal_2(false, _channel_internal, conv.literal, 0, 1);
            if _format_debug {
             if error then stdout.writeln("DEBUG XZOB");
            }
            var offsetB = qio_channel_offset_unlocked(_channel_internal);
            if (!error) && offsetA == offsetB {
              // didn't really read whitespace.
              error = EFORMAT;
              if _format_debug then stdout.writeln("DEBUG YYZ");
            }
            if _format_debug then stdout.writeln("AFTER WHITESPACE err is ", error:int);
          } else {
            error = qio_channel_scan_literal_2(false, _channel_internal, conv.literal, conv.literal_length:ssize_t, 0);
            if _format_debug then stdout.writeln("AFTER LITERAL err is ", error:int);
          }
        } else {
          // when printing we don't care if it's just whitespace.
          error = qio_channel_print_literal_2(false, _channel_internal, conv.literal, conv.literal_length:ssize_t);
        }
      } else if conv.argType == QIO_CONV_ARG_TYPE_NONE_REGEXP_LITERAL {
        if ! isReadf {
          // It's not so clear what to do when printing
          // a regexp. So we just don't handle it.
          error = qio_format_error_write_regexp();
          if _format_debug then stdout.writeln("DEBUG AZA");
        } else {
          // allocate regexp info if needed
          if r == nil then r = new _channel_regexp_info();
          // clear out old data, if there is any.
          r.clear();
          // Compile a regexp from the format string
          var errstr:string;
          if _format_debug then stdout.writeln("COMPILING REGEXP");
          // build a regexp out of regexp and regexp_flags
          qio_regexp_create_compile_flags_2(conv.regexp, conv.regexp_length, conv.regexp_flags, conv.regexp_flags_length, /* utf8? */ true, r.theRegexp);
          r.releaseRegexp = true;
          if qio_regexp_ok(r.theRegexp) {
            r.hasRegexp = true;
            r.ncaptures = qio_regexp_get_ncaptures(r.theRegexp);
            // If there are no captures, and we don't have arguments
            // to consume, go ahead and match the regexp.
            if r.ncaptures > 0 ||
               conv.preArg1 != QIO_CONV_UNK ||
               conv.preArg2 != QIO_CONV_UNK ||
               conv.preArg3 != QIO_CONV_UNK
            {
              // We need to consume args as part of matching this regexp.
              gotConv = true;
              break;
            } else {
              // No args will be consumed.
              _match_regexp_if_needed(cur, len, error, style, r);
            }
          } else {
            error = qio_format_error_bad_regexp();
            if _format_debug then stdout.writeln("DEBUG AZB");
            //if dieOnError then assert(!error, errstr);
          }
        }
      } else {
        // Some other kind of format specifier... we
        // will return to handle.
        gotConv = true;
        break;
      }
    }
  }
}

pragma "no doc"
proc channel._conv_helper(
    ref error:syserr,
    ref conv:qio_conv_t, ref gotConv:bool,
    ref j:int,
    ref argType)
{
  if error then return;
  if gotConv {
    // Perhaps we need to handle pre/post args
    // that adjust the style
    if conv.preArg1 != QIO_CONV_UNK {
      argType(j) = conv.preArg1;
      j += 1;
    }
    if conv.preArg2 != QIO_CONV_UNK {
      argType(j) = conv.preArg2;
      j += 1;
    }
    if conv.preArg3 != QIO_CONV_UNK {
      argType(j) = conv.preArg3;
      j += 1;
    }
    if conv.argType != QIO_CONV_UNK {
      if argType(j) == QIO_CONV_UNK {
        // Some regexp paths set it earlier..
        argType(j) = conv.argType;
      }
      j += 1;
    }
  }
}

pragma "no doc"
proc channel._conv_sethandler(
    ref error:syserr,
    argtypei:c_int,
    ref style:iostyle,
    i:int, argi,
    isReadf:bool):bool
{
  if error then return false;
  // Now, set style elements based on action
  // at i
  select argtypei {
    when QIO_CONV_SET_MIN_WIDTH_COLS {
      var (t,ok) = _toIntegral(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
        if _format_debug then stdout.writeln("DEBUG AZE");
      } else style.min_width_columns = t:uint(32);
    }
    when QIO_CONV_SET_MAX_WIDTH_COLS {
      var (t,ok) = _toIntegral(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
        if _format_debug then stdout.writeln("DEBUG AZF");
      } else style.max_width_columns = t:uint(32);
    }
    when QIO_CONV_SET_MAX_WIDTH_CHARS {
      var (t,ok) = _toIntegral(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
        if _format_debug then stdout.writeln("DEBUG AZG");
      } else style.max_width_characters = t:uint(32);
    }
    when QIO_CONV_SET_MAX_WIDTH_BYTES {
      var (t,ok) = _toIntegral(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
        if _format_debug then stdout.writeln("DEBUG AZGX");
      } else style.max_width_bytes = t:uint(32);
    }
    when QIO_CONV_SET_PRECISION {
      var (t,ok) = _toIntegral(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
        if _format_debug then stdout.writeln("DEBUG AZH");
      } else style.precision = t:int(32);
    }
    when QIO_CONV_SET_STRINGSTART {
      var (t,ok) = _toChar(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
        if _format_debug then stdout.writeln("DEBUG AZH");
      } else style.string_start = t:style_char_t;
    }
    when QIO_CONV_SET_STRINGEND {
      var (t,ok) = _toChar(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
        if _format_debug then stdout.writeln("DEBUG AZH");
      } else style.string_end = t:style_char_t;
    }
    when QIO_CONV_SET_STRINGSTARTEND {
      var (t,ok) = _toChar(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
        if _format_debug then stdout.writeln("DEBUG AZH");
      } else {
        style.string_start = t:style_char_t;
        style.string_end = t:style_char_t;
      }
    }
    when QIO_CONV_SET_STRINGLEN {
      var (t,ok) = _toIntegral(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
        if _format_debug then stdout.writeln("DEBUG AZH");
      } else {
        style.str_style = t:int(64);
      }
    }
    when QIO_CONV_SET_TERMINATOR {
      var (t,ok) = _toChar(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
        if _format_debug then stdout.writeln("DEBUG AZH");
      } else {
        style.str_style = stringStyleTerminated(t:uint(8));
      }
    }
    when QIO_CONV_SET_DONE {
      // Do nothing. Already handled.
    }
    when QIO_CONV_UNK {
      // Too many arguments.
      error = qio_format_error_too_many_args();
      if _format_debug then stdout.writeln("DEBUG AZK");
    } otherwise {
      return true;
    }
  }
  return false;
}

pragma "no doc"
proc channel._write_signed(width:uint(32), t:int, i:int):syserr
{
  var err:syserr;
  var byteorder = qio_channel_byteorder(_channel_internal);
  select width {
    when 1 {
      var x = t:int(8);
      err = qio_channel_write_int8(false, _channel_internal, x);
    } when 2 {
      var x = t:int(16);
      err = qio_channel_write_int16(false, byteorder, _channel_internal, x);
    } when 4 {
      var x = t:int(32);
      err = qio_channel_write_int32(false, byteorder, _channel_internal, x);
    } when 8 {
      var x = t:int(64);
      err = qio_channel_write_int64(false, byteorder, _channel_internal, x);
    } otherwise err = qio_format_error_arg_mismatch(i);
  }
  return err;
}

pragma "no doc"
proc channel._read_signed(width:uint(32), out t:int, i:int):syserr
{
  var err:syserr;
  var byteorder = qio_channel_byteorder(_channel_internal);
  select width {
    when 1 {
      var x:int(8);
      err = qio_channel_read_int8(false, _channel_internal, x);
      t = x;
    } when 2 {
      var x:int(16);
      err = qio_channel_read_int16(false, byteorder, _channel_internal, x);
      t = x;
    } when 4 {
      var x:int(32);
      err = qio_channel_read_int32(false, byteorder, _channel_internal, x);
      t = x;
    } when 8 {
      var x:int(64);
      err = qio_channel_read_int64(false, byteorder, _channel_internal, x);
      t = x;
    } otherwise err = qio_format_error_arg_mismatch(i);
  }
  return err;
}

pragma "no doc"
proc channel._write_unsigned(width:uint(32), t:uint, i:int)
{
  var err:syserr;
  var byteorder = qio_channel_byteorder(_channel_internal);
  select width {
    when 1 {
      var x = t:uint(8);
      err = qio_channel_write_uint8(false, _channel_internal, x);
    } when 2 {
      var x = t:uint(16);
      err = qio_channel_write_uint16(false, byteorder, _channel_internal, x);
    } when 4 {
      var x = t:uint(32);
      err = qio_channel_write_uint32(false, byteorder, _channel_internal, x);
    } when 8 {
      var x = t:uint(64);
      err = qio_channel_write_uint64(false, byteorder, _channel_internal, x);
    } otherwise err = qio_format_error_arg_mismatch(i);
  }
  return err;
}
pragma "no doc"
proc channel._read_unsigned(width:uint(32), out t:uint, i:int)
{
  var err:syserr;
  var byteorder = qio_channel_byteorder(_channel_internal);
  select width {
    when 1 {
      var x:uint(8);
      err = qio_channel_read_uint8(false, _channel_internal, x);
      t = x;
    } when 2 {
      var x:uint(16);
      err = qio_channel_read_uint16(false, byteorder, _channel_internal, x);
      t = x;
    } when 4 {
      var x:uint(32);
      err = qio_channel_read_uint32(false, byteorder, _channel_internal, x);
      t = x;
    } when 8 {
      var x:uint(64);
      err = qio_channel_read_uint64(false, byteorder, _channel_internal, x);
      t = x;
    } otherwise err = qio_format_error_arg_mismatch(i);
  }
  return err;
}


pragma "no doc"
proc channel._write_real(width:uint(32), t:real, i:int)
{
  var err:syserr;
  var byteorder = qio_channel_byteorder(_channel_internal);
  select width {
    when 4 {
      var x = t:real(32);
      err = qio_channel_write_float32(false, byteorder, _channel_internal, x);
    } when 8 {
      var x = t:real(64);
      err = qio_channel_write_float64(false, byteorder, _channel_internal, x);
    } otherwise err = qio_format_error_arg_mismatch(i);
  }
  return err;
}
pragma "no doc"
proc channel._read_real(width:uint(32), out t:real, i:int)
{
  var err:syserr;
  var byteorder = qio_channel_byteorder(_channel_internal);
  select width {
    when 4 {
      var x:real(32);
      err = qio_channel_read_float32(false, byteorder, _channel_internal, x);
      t = x;
    } when 8 {
      var x:real(64);
      err = qio_channel_read_float64(false, byteorder, _channel_internal, x);
      t = x;
    } otherwise err = qio_format_error_arg_mismatch(i);
  }
  return err;
}


pragma "no doc"
proc channel._write_complex(width:uint(32), t:complex, i:int)
{
  var err:syserr = ENOERR;
  var byteorder = qio_channel_byteorder(_channel_internal);
  select width {
    when 8 {
      var x = t:complex(64);
      var re = x.re;
      var im = x.im;
      err = qio_channel_write_float32(false, byteorder, _channel_internal, re);
      if ! err {
        err = qio_channel_write_float32(false, byteorder, _channel_internal, im);
      }
    } when 16 {
      var x = t:complex(128);
      var re = x.re;
      var im = x.im;
      err = qio_channel_write_float64(false, byteorder, _channel_internal, re);
      if ! err {
        err = qio_channel_write_float64(false, byteorder, _channel_internal, im);
      }
    } otherwise err = qio_format_error_arg_mismatch(i);
  }
  return err;
}

pragma "no doc"
proc channel._read_complex(width:uint(32), out t:complex, i:int)
{
  var err:syserr = ENOERR;
  var byteorder = qio_channel_byteorder(_channel_internal);
  select width {
    when 8 {
      var x:complex(64);
      var re:x.re.type;
      var im:x.im.type;
      err = qio_channel_read_float32(false, byteorder, _channel_internal, re);
      if ! err {
        err = qio_channel_read_float32(false, byteorder, _channel_internal, im);
      }
      x = (re, im):complex(64); // tuple to complex
      t = x;
    } when 16 {
      var x:complex(128);
      var re:x.re.type;
      var im:x.im.type;
      err = qio_channel_read_float64(false, byteorder, _channel_internal, re);
      if ! err {
        err = qio_channel_read_float64(false, byteorder, _channel_internal, im);
      }
      x = (re, im):complex(128); // tuple to complex
      t = x;
    } otherwise err = qio_format_error_arg_mismatch(i);
  }
  return err;
}



/*

   Write arguments according to a format string. See
   :ref:`about-io-formatted-io`.

   :arg fmt: the format string
   :arg args: the arguments to write
   :arg error: optional argument to capture an error code. If this argument
               is not provided and an error is encountered, this function
               will halt with an error message.
 */
proc channel.writef(fmtStr:string, const args ...?k, out error:syserr):bool {
  if !writing then compilerError("writef on read-only channel");
  error = ENOERR;
  const origLocale = this.getLocaleOfIoRequest();
  on this.home {
    this.lock();
    var fmt = fmtStr.localize().c_str();
    var save_style = this._style();
    var cur:size_t = 0;
    var len:size_t = fmt.length:size_t;
    var conv:qio_conv_t;
    var gotConv:bool;
    var style:iostyle;
    var end:size_t;
    var argType:(k+5)*c_int;

    var r:_channel_regexp_info = nil;

    for i in 1..argType.size {
      argType(i) = QIO_CONV_UNK;
    }

    var j = 1;

    for param i in 1..k {
      // The inside of this loop is a bit crazy because
      // we're writing it all in a param for in order to
      // get generic argument handling.

      gotConv = false;

      if j <= i {
        _format_reader(fmt, cur, len, error,
                       conv, gotConv, style, r,
                       false);
      }

      _conv_helper(error, conv, gotConv, j, argType);

      var domore = _conv_sethandler(error, argType(i), style, i,args(i),false);

      if _format_debug then stdout.writeln("domore ", domore, " arg ", argType(i), " arg ", args(i));

      if domore {
        this._set_style(style);
        // otherwise we will consume at least one argument.
        select argType(i) {
          when QIO_CONV_ARG_TYPE_SIGNED, QIO_CONV_ARG_TYPE_BINARY_SIGNED {
            var (t,ok) = _toSigned(args(i));
            if ! ok {
              error = qio_format_error_arg_mismatch(i);
              if _format_debug then stdout.writeln("DEBUG AZK");
            } else {
              if argType(i) == QIO_CONV_ARG_TYPE_BINARY_SIGNED then
                error = _write_signed(style.max_width_bytes, t, i);
              else
                error = _write_one_internal(_channel_internal, iokind.dynamic, t, origLocale);
            }
          } when QIO_CONV_ARG_TYPE_UNSIGNED, QIO_CONV_ARG_TYPE_BINARY_UNSIGNED {
            var (t,ok) = _toUnsigned(args(i));
            if ! ok {
              error = qio_format_error_arg_mismatch(i);
              if _format_debug then stdout.writeln("DEBUG AZK");
            } else {
              if argType(i) == QIO_CONV_ARG_TYPE_BINARY_UNSIGNED then
                error = _write_unsigned(style.max_width_bytes, t, i);
              else
                error = _write_one_internal(_channel_internal, iokind.dynamic, t, origLocale);
            }
          } when QIO_CONV_ARG_TYPE_REAL, QIO_CONV_ARG_TYPE_BINARY_REAL {
            var (t,ok) = _toReal(args(i));
            if ! ok {
              error = qio_format_error_arg_mismatch(i);
              if _format_debug then stdout.writeln("DEBUG AZK");
            } else {
              if argType(i) == QIO_CONV_ARG_TYPE_BINARY_REAL then
                error = _write_real(style.max_width_bytes, t, i);
              else
                error = _write_one_internal(_channel_internal, iokind.dynamic, t, origLocale);
            }
          } when QIO_CONV_ARG_TYPE_IMAG, QIO_CONV_ARG_TYPE_BINARY_IMAG {
            var (t,ok) = _toImag(args(i));
            if ! ok {
              error = qio_format_error_arg_mismatch(i);
              if _format_debug then stdout.writeln("DEBUG AZK");
            } else {
              if argType(i) == QIO_CONV_ARG_TYPE_BINARY_IMAG then
                error = _write_real(style.max_width_bytes, t:real, i);
              else
                error = _write_one_internal(_channel_internal, iokind.dynamic, t, origLocale);
            }
          } when QIO_CONV_ARG_TYPE_COMPLEX, QIO_CONV_ARG_TYPE_BINARY_COMPLEX {
            var (t,ok) = _toComplex(args(i));
            if ! ok {
              error = qio_format_error_arg_mismatch(i);
              if _format_debug then stdout.writeln("DEBUG AZK");
            } else {
              if argType(i) == QIO_CONV_ARG_TYPE_BINARY_COMPLEX then
                error = _write_complex(style.max_width_bytes, t, i);
              else error = _write_one_internal(_channel_internal, iokind.dynamic, t, origLocale);
            }
          } when QIO_CONV_ARG_TYPE_NUMERIC {
            var (t,ok) = _toNumeric(args(i));
            if ! ok {
              error = qio_format_error_arg_mismatch(i);
              if _format_debug then stdout.writeln("DEBUG AZO11");
            } else error = _write_one_internal(_channel_internal, iokind.dynamic, t, origLocale);
          } when QIO_CONV_ARG_TYPE_CHAR {
            var (t,ok) = _toChar(args(i));
            if ! ok {
              error = qio_format_error_arg_mismatch(i);
              if _format_debug then stdout.writeln("DEBUG AZN");
            } else error = _write_one_internal(_channel_internal, iokind.dynamic, new ioChar(t), origLocale);
          } when QIO_CONV_ARG_TYPE_STRING {
            var (t,ok) = _toString(args(i));
            if ! ok {
              error = qio_format_error_arg_mismatch(i);
              if _format_debug then stdout.writeln("DEBUG AZO");
            } else error = _write_one_internal(_channel_internal, iokind.dynamic, t, origLocale);
          } when QIO_CONV_ARG_TYPE_REGEXP {
            // It's not so clear what to do when printing
            // a regexp. So we just don't handle it.
            error = qio_format_error_write_regexp();
            if _format_debug then stdout.writeln("DEBUG AZP");
          } when QIO_CONV_ARG_TYPE_REPR {
            error = _write_one_internal(_channel_internal, iokind.dynamic, args(i), origLocale);
          } otherwise {
            // Unhandled argument type!
            halt("readf/writef internal error ", argType(i));
          }
        }
      }
    }

    if ! error {
      if cur < len {
        var dummy:c_int;
        _format_reader(fmt, cur, len, error,
                       conv, gotConv, style, r,
                       false);
      }

      if cur < len {
        // Mismatched number of arguments!
        error = qio_format_error_too_few_args();
        if _format_debug then stdout.writeln("DEBUG AZR");
      }
    }

    this._set_style(save_style);
    this.unlock();
  }
  return !error;
}


// documented in string error= version
pragma "no doc"
proc channel.writef(fmtStr:string, out error:syserr):bool {
  if !writing then compilerError("writef on read-only channel");
  error = ENOERR;
  on this.home {
    this.lock();
    var fmt = fmtStr.localize().c_str();
    var save_style = this._style();
    var cur:size_t = 0;
    var len:size_t = fmt.length:size_t;
    var conv:qio_conv_t;
    var gotConv:bool;
    var style:iostyle;
    var end:size_t;
    var dummy:c_int;

    var r:_channel_regexp_info = nil;

    _format_reader(fmt, cur, len, error,
                   conv, gotConv, style, r,
                   false);

    if ! error {
      if gotConv {
        error = qio_format_error_too_few_args();
        if _format_debug then stdout.writeln("DEBUG AZZs");
      }
    }

    if ! error {
      if cur < len {
        // Mismatched number of arguments!
        error = qio_format_error_too_few_args();
        if _format_debug then stdout.writeln("DEBUG AZS");
      }
    }

    this._set_style(save_style);
    this.unlock();
  }
  return !error;
}

/*

   Read arguments according to a format string. See
   :ref:`about-io-formatted-io`.

   :arg fmt: the format string
   :arg args: the arguments to read
   :arg error: optional argument to capture an error code. If this argument
               is not provided and an error is encountered, this function
               will halt with an error message.
   :returns: true if all arguments were read according to the format string,
             false on EOF. If the format did not match the input, returns
             false with error=EFORMAT or halts if no error argument was
             provided.
 */

proc channel.readf(fmtStr:string, ref args ...?k, out error:syserr):bool {
  if writing then compilerError("readf on write-only channel");
  error = ENOERR;
  const origLocale = this.getLocaleOfIoRequest();
  on this.home {
    this.lock();
    var fmt = fmtStr.localize().c_str();
    var save_style = this._style();
    var cur:size_t = 0;
    var len:size_t = fmt.length:size_t;
    var conv:qio_conv_t;
    var gotConv:bool;
    var style:iostyle;
    var end:size_t;
    var argType:(k+5)*c_int;

    var r:_channel_regexp_info = nil;

    for i in 1..argType.size {
      argType(i) = QIO_CONV_UNK;
    }

    error = qio_channel_mark(false, _channel_internal);
    if !error {
      var j = 1;

      for param i in 1..k {
        // The inside of this loop is a bit crazy because
        // we're writing it all in a param for in order to
        // get generic argument handling.
        if j <= i {
          _format_reader(fmt, cur, len, error,
                         conv, gotConv, style, r,
                         true);

          if r != nil && r.hasRegexp {
            // We need to handle the next ncaptures arguments.
            if i + r.ncaptures - 1 > k {
              error = qio_format_error_too_few_args();
              if _format_debug then stdout.writeln("DEBUG AXA");
            }
            for z in 0..#r.ncaptures {
              if i+z <= argType.size {
                argType(i+z) = QIO_CONV_SET_CAPTURE;
              }
            }
          }
        }

        _conv_helper(error, conv, gotConv, j, argType);

        var domore = _conv_sethandler(error, argType(i),style,i,args(i),false);

        if domore {
          this._set_style(style);
          // otherwise we will consume at least one argument.
          select argType(i) {
            when QIO_CONV_ARG_TYPE_SIGNED, QIO_CONV_ARG_TYPE_BINARY_SIGNED {
              var (t,ok) = _toSigned(args(i));
              if ! ok {
                error = qio_format_error_arg_mismatch(i);
                if _format_debug then stdout.writeln("DEBUG AXB");
              } else {
                var ti:int;
                if argType(i) == QIO_CONV_ARG_TYPE_BINARY_SIGNED then
                  error = _read_signed(style.max_width_bytes, ti, i);
                else
                  error = _read_one_internal(_channel_internal, iokind.dynamic, ti, origLocale);
                if ! error then error = _setIfPrimitive(args(i),ti,i);
              }
            }
            when QIO_CONV_ARG_TYPE_UNSIGNED, QIO_CONV_ARG_TYPE_BINARY_UNSIGNED {
              var (t,ok) = _toUnsigned(args(i));
              if ! ok {
                error = qio_format_error_arg_mismatch(i);
                if _format_debug then stdout.writeln("DEBUG AXB");
              } else {
                var ti:uint;
                if argType(i) == QIO_CONV_ARG_TYPE_BINARY_UNSIGNED then
                  error = _read_unsigned(style.max_width_bytes, ti, i);
                else
                  error = _read_one_internal(_channel_internal, iokind.dynamic, ti, origLocale);
                if ! error then error = _setIfPrimitive(args(i),ti,i);
              }
            } when QIO_CONV_ARG_TYPE_REAL, QIO_CONV_ARG_TYPE_BINARY_REAL {
              var (t,ok) = _toReal(args(i));
              if ! ok {
                error = qio_format_error_arg_mismatch(i);
                if _format_debug then stdout.writeln("DEBUG AXB");
              } else {
                var ti:real;
                if argType(i) == QIO_CONV_ARG_TYPE_BINARY_REAL then
                  error = _read_real(style.max_width_bytes, ti, i);
                else
                  error = _read_one_internal(_channel_internal, iokind.dynamic, ti, origLocale);
                if ! error then error = _setIfPrimitive(args(i),ti,i);
              }
            } when QIO_CONV_ARG_TYPE_IMAG, QIO_CONV_ARG_TYPE_BINARY_IMAG {
              var (t,ok) = _toImag(args(i));
              if ! ok {
                error = qio_format_error_arg_mismatch(i);
                if _format_debug then stdout.writeln("DEBUG AXB1");
              } else {
                var ti:imag;
                if argType(i) == QIO_CONV_ARG_TYPE_BINARY_IMAG {
                  var tr:real;
                  error = _read_real(style.max_width_bytes, tr, i);
                  ti = tr:imag;
                } else
                  error = _read_one_internal(_channel_internal, iokind.dynamic, ti, origLocale);
                if ! error then error = _setIfPrimitive(args(i),ti,i);
              }
            } when QIO_CONV_ARG_TYPE_COMPLEX, QIO_CONV_ARG_TYPE_BINARY_COMPLEX {
              var (t,ok) = _toComplex(args(i));
              if ! ok {
                error = qio_format_error_arg_mismatch(i);
                if _format_debug then stdout.writeln("DEBUG AXB");
              } else {
                var ti:complex;
                if argType(i) == QIO_CONV_ARG_TYPE_BINARY_COMPLEX then
                 error = _read_complex(style.max_width_bytes, ti, i);
                else
                  error = _read_one_internal(_channel_internal, iokind.dynamic, ti, origLocale);
                if ! error then error = _setIfPrimitive(args(i),ti,i);
              }
            } when QIO_CONV_ARG_TYPE_NUMERIC {
              var (t,ok) = _toNumeric(args(i));
              if ! ok {
                error = qio_format_error_arg_mismatch(i);
                if _format_debug then stdout.writeln("DEBUG AXB1");
              } else {
                var ti = t;
                error = _read_one_internal(_channel_internal, iokind.dynamic, ti, origLocale);
                if ! error then error = _setIfPrimitive(args(i),ti,i);
              }
            } when QIO_CONV_ARG_TYPE_CHAR {
              var (t,ok) = _toChar(args(i));
              var chr = new ioChar(t);
              if ! ok {
                error = qio_format_error_arg_mismatch(i);
                if _format_debug then stdout.writeln("DEBUG AXE");
              } else error = _read_one_internal(_channel_internal, iokind.dynamic, chr, origLocale);
              if ! error then _setIfChar(args(i),chr.ch);
            } when QIO_CONV_ARG_TYPE_STRING {
              var (t,ok) = _toString(args(i));
              if ! ok {
                error = qio_format_error_arg_mismatch(i);
                if _format_debug then stdout.writeln("DEBUG AXF");
              }
              else error = _read_one_internal(_channel_internal, iokind.dynamic, t, origLocale);
              if ! error then error = _setIfPrimitive(args(i),t,i);
            } when QIO_CONV_ARG_TYPE_REGEXP {
              var (t,ok) = _toRegexp(args(i));
              if ! ok {
                error = qio_format_error_arg_mismatch(i);
                if _format_debug then stdout.writeln("DEBUG AXG");
              }
              // match it here.
              if r == nil then r = new _channel_regexp_info();
              r.clear();
              r.theRegexp = t._regexp;
              r.hasRegexp = true;
              r.releaseRegexp = false;
              _match_regexp_if_needed(cur, len, error, style, r);

              // Set the capture groups.
              // We need to handle the next ncaptures arguments.
              if i + r.ncaptures - 1 > k {
                error = qio_format_error_too_few_args();
                if _format_debug then stdout.writeln("DEBUG AXH");
              }
              for z in 0..#r.ncaptures {
                if i+z <= argType.size {
                  argType(i+z+1) = QIO_CONV_SET_CAPTURE;
                }
              }
            } when QIO_CONV_ARG_TYPE_REPR {
              error = _read_one_internal(_channel_internal, iokind.dynamic, args(i), origLocale);
            } when QIO_CONV_SET_CAPTURE {
              if r == nil {
                error = qio_format_error_bad_regexp();
                if _format_debug then stdout.writeln("DEBUG AXI");
              } else {
                _match_regexp_if_needed(cur, len, error, style, r);
                // Set args(i) to the capture at capturei.
                if r.capturei >= r.ncaptures {
                  error = qio_format_error_bad_regexp();
                  if _format_debug then stdout.writeln("DEBUG AXJ");
                } else {
                  // We have a string in captures[capturei] and
                  // we need to set args(i) to that.
                  if _isIoPrimitiveType(args(i).type) {
                    // but only if it's a primitive type
                    // (so that we can avoid problems with string-to-record).
                    args(i) = r.capArr[r.capturei]:args(i).type;
                  }
                  r.capturei += 1;
                }
              }
            } otherwise {
              halt("Internal error in readf/writef");
            }
          }
        }
      }

      if ! error {
        if cur < len {
          var dummy:c_int;
          _format_reader(fmt, cur, len, error,
                         conv, gotConv, style, r,
                         true);
        }
      }

      if ! error {
        if cur < len {
          // Mismatched number of arguments!
          error = qio_format_error_too_few_args();
          if _format_debug then stdout.writeln("DEBUG AXL");
        }
      }

      if r != nil {
        delete r;
      }

      if ! error {
        // commit.
        qio_channel_commit_unlocked(_channel_internal);
      } else {
        // revert
        qio_channel_revert_unlocked(_channel_internal);
      }
    }
    this._set_style(save_style);
    this.unlock();
  }
  return !error;
}

// documented in string error= version
pragma "no doc"
proc channel.readf(fmtStr:string, out error:syserr):bool {
  if writing then compilerError("readf on write-only channel");
  error = ENOERR;
  on this.home {
    this.lock();
    var fmt = fmtStr.localize().c_str();
    var save_style = this._style();
    var cur:size_t = 0;
    var len:size_t = fmt.length:size_t;
    var conv:qio_conv_t;
    var gotConv:bool;
    var style:iostyle;
    var end:size_t;
    var dummy:c_int;

    var r:_channel_regexp_info = nil;

    error = qio_channel_mark(false, _channel_internal);
    if !error {
      if _format_debug then stdout.writeln("DEBUG BBBB");
      _format_reader(fmt, cur, len, error,
                     conv, gotConv, style, r,
                     true);
      if gotConv {
        error = qio_format_error_too_few_args();
        if _format_debug then stdout.writeln("DEBUG ABZOO");
      }
    }
    if !error {
      if cur < len {
        error = qio_format_error_too_few_args();
        if _format_debug then stdout.writeln("DEBUG AXM");
      }
    }
    if ! error {
      // commit.
      qio_channel_commit_unlocked(_channel_internal);
    } else {
      // revert
      qio_channel_revert_unlocked(_channel_internal);
    }

    this._set_style(save_style);
    this.unlock();
  }
  return !error;
}

// documented in string error= version
pragma "no doc"
proc channel.writef(fmt: string, const args ...?k) {
  var e:syserr = ENOERR;
  this.writef(fmt, (...args), error=e);
  if !e then return true;
  else {
    this._ch_ioerror(e, "in channel.writef(fmt:string, ...)");
    return false;
  }
}

// documented in string error= version
pragma "no doc"
proc channel.writef(fmt: string) {
  var e:syserr = ENOERR;
  this.writef(fmt, error=e);
  if !e then return true;
  else {
    this._ch_ioerror(e, "in channel.writef(fmt:string, ...)");
    return false;
  }
}

// documented in string error= version
pragma "no doc"
proc channel.readf(fmt:string, ref args ...?k) {
  var e:syserr = ENOERR;
  this.readf(fmt, (...args), error=e);
  if !e then return true;
  else if e == EEOF then return false;
  else {
    this._ch_ioerror(e, "in channel.readf(fmt:string, ...)");
    return false;
  }
}

// documented in string error= version
pragma "no doc"
proc channel.readf(fmt:string) {
  var e:syserr = ENOERR;
  this.readf(fmt, error=e);
  if !e then return true;
  else if e == EEOF then return false;
  else if e == EFORMAT then return false;
  else {
    this._ch_ioerror(e, "in channel.readf(fmt:string, ...)");
    return false;
  }
}

/* Call ``stdout.writef``; see :proc:`channel.writef`. */
proc writef(fmt:string, const args ...?k):bool {
  return stdout.writef(fmt, (...args));
}
// documented in string version
pragma "no doc"
proc writef(fmt:string):bool {
  return stdout.writef(fmt);
}
/* Call ``stdout.readf``; see :proc:`channel.readf`. */
proc readf(fmt:string, ref args ...?k):bool {
  return stdin.readf(fmt, (...args));
}
// documented in string version
pragma "no doc"
proc readf(fmt:string):bool {
  return stdin.readf(fmt);
}


/*
   Skip a field in the current aggregate format. This method is currently only
   supported for JSON format and returns ENOTSUP for other formats. In other
   formats, it may not be possible in general to know when a field ends.

   The field skipped includes a field name and value but not a following
   separator. For example, for a JSON format channel, given the input:

   ::

      "fieldName":"fieldValue", "otherField":3

   this function will skip to (but leave unread) the comma after
   the first field value.

   :arg error: optional argument to capture an error code. If this argument
               is not provided and an error is encountered, this function
               will halt with an error message.
 */
proc channel.skipField(out error:syserr) {
  on this.home {
    this.lock();
    var st = this.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
    if st == QIO_AGGREGATE_FORMAT_JSON {
      error = qio_channel_skip_json_field(false, _channel_internal);
    } else {
      error = ENOTSUP;
    }
    this.unlock();
  }
}
pragma "no doc"
proc channel.skipField() {
  var err:syserr;
  this.skipField(err);
  if err then this._ch_ioerror(err, "in skipField");
}


private inline proc chpl_do_format(fmt:string, args ...?k, out error:syserr):string {
  // Open a memory buffer to store the result
  var f = openmem();

  var w = f.writer(locking=false);

  w.writef(fmt, (...args), error=error);

  var offset = w.offset();

  var buf = c_malloc(uint(8), offset+1);

  // you might need a flush here if
  // close went away
  w.close();

  var r = f.reader(locking=false);

  r.readBytes(buf, offset:ssize_t);
  // Add the terminating NULL byte to make C string conversion easy.
  buf[offset] = 0;
  r.close();

  f.close();

  return new string(buf, offset, offset+1, owned=true, needToCopy=false);
}


// This function is no longer available.
pragma "no doc"
proc format(fmt:string, args ...?k):string {
  compilerError("use string.format(args ...) not format(fmt, args ...)");
}

/*

  Return a new string consisting of values formatted according to a
  format string.  See :ref:`about-io-formatted-io`.

  :arg this: the format string
  :arg args: the arguments to format
  :arg error: optional argument to capture an error code. If this argument
             is not provided and an error is encountered, this function
             will halt with an error message.
  :returns: the resulting string

 */
proc string.format(args ...?k, out error:syserr):string {
  return chpl_do_format(this, (...args), error);
}

// documented in the error= version
pragma "no doc"
proc string.format(args ...?k):string {
  var err:syserr = ENOERR;
  var ret = chpl_do_format(this, (...args), error=err);
  if err then ioerror(err, "in string.format");
  return ret;
}


// ---------------------------------------------------------------
// ---------------------------------------------------------------
// Starting support for regular expression search on channels
// ---------------------------------------------------------------
// ---------------------------------------------------------------



use Regexp;

private extern proc qio_regexp_channel_match(const ref re:qio_regexp_t, threadsafe:c_int, ch:qio_channel_ptr_t, maxlen:int(64), anchor:c_int, can_discard:bool, keep_unmatched:bool, keep_whole_pattern:bool, submatch:_ddata(qio_regexp_string_piece_t), nsubmatch:int(64)):syserr;

pragma "no doc"
proc channel._extractMatch(m:reMatch, ref arg:reMatch, ref error:syserr) {
  // If the argument is a match record, just return it.
  arg = m;
}

pragma "no doc"
proc channel._extractMatch(m:reMatch, ref arg:string, ref error:syserr) {
  var cur:int(64);
  var target = m.offset;
  var len = m.length;

  // If there was no match, return the default value of the type
  if !m.matched {
    arg = "";
  }

  // Read into a string the appropriate region of the file.
  if !error {
    qio_channel_revert_unlocked(_channel_internal);
    error = qio_channel_mark(false, _channel_internal);
    cur = qio_channel_offset_unlocked(_channel_internal);
  }

  if ! error {
    // There was a match, so we have to read the
    // strings for the capture groups.
    error = qio_channel_advance(false, _channel_internal, target - cur);
  }

  var s:string;
  if ! error {
    var gotlen:int(64);
    var ts: c_string_copy;
    error =
        qio_channel_read_string(false, iokind.native, stringStyleExactLen(len),
                                _channel_internal, ts, gotlen, len: ssize_t);
    s = new string(ts, length=gotlen, needToCopy=false);
  }

  if ! error {
    arg = s;
  } else {
    arg = "";
  }
}

pragma "no doc"
proc channel._extractMatch(m:reMatch, ref arg:?t, ref error:syserr) where t != reMatch && t != string {
  // If there was no match, return the default value of the type
  if !m.matched {
    var empty:arg.type;
    arg = empty;
  }

  // Read into a string the appropriate region of the file.
  var s:string;
  _extractMatch(m, s, error);

  if ! error {
    arg = s:arg.type;
  } else {
    var empty:arg.type;
    arg = empty;
  }
}


/*  Sets arg to the string of a match.

    Assumes that the channel has been marked before where
    the captures are being returned. Will change the channel
    position to just after the match. Will not do anything
    if error is set.

    :arg m: a :record:`Regexp.reMatch` storing a location that matched
    :arg arg: an argument to retrieve the match into. If it is not a string,
              the string match will be cast to arg.type.
    :arg error: optional argument to capture an error code. If this argument
                is not provided and an error is encountered, this function
                will halt with an error message.
 */
proc channel.extractMatch(m:reMatch, ref arg, ref error:syserr) {
  on this.home {
    this.lock();
    _extractMatch(m, arg, error);
    this.unlock();
  }
}
// documented in error= version
pragma "no doc"
proc channel.extractMatch(m:reMatch, ref arg) {
  on this.home {
    this.lock();
    var err:syserr = ENOERR;
    _extractMatch(m, arg, err);
    if err {
      this._ch_ioerror(err, "in channel.extractMatch(m:reMatch, ref " +
                             arg.type:string + ")");
    }
    this.unlock();
  }
}

// Assumes that the channel has been marked where the search began
// (or at least before the capture groups if discarding)
pragma "no doc"
proc channel._ch_handle_captures(matches:_ddata(qio_regexp_string_piece_t),
                                 nmatches:int,
                                 ref captures, ref error:syserr) {
  assert(nmatches >= captures.size);
  for param i in 1..captures.size {
    var m = _to_reMatch(matches[i]);
    _extractMatch(m, captures[i], error);
  }
}

// documented in the error= captures version
pragma "no doc"
proc channel.search(re:regexp, ref error:syserr):reMatch
{
  var m:reMatch;
  on this.home {
    this.lock();
    var nm = 1;
    var matches = _ddata_allocate(qio_regexp_string_piece_t, nm);
    error = qio_channel_mark(false, _channel_internal);
    if !error {
      error = qio_regexp_channel_match(re._regexp,
                                       false, _channel_internal, max(int(64)),
                                       QIO_REGEXP_ANCHOR_UNANCHORED,
                                       /* can_discard */ true,
                                       /* keep_unmatched */ false,
                                       /* keep_whole_pattern */ true,
                                       matches, nm);
    }
    // Don't report "didn't match" errors
    if error == EFORMAT || error == EEOF then error = ENOERR;
    if !error {
      m = _to_reMatch(matches[0]);
      if m.matched {
        // Advance to the match.
        qio_channel_revert_unlocked(_channel_internal);
        var cur = qio_channel_offset_unlocked(_channel_internal);
        var target = m.offset;
        error = qio_channel_advance(false, _channel_internal, target - cur);
      } else {
        // If we didn't match... leave the channel position at EOF
        qio_channel_commit_unlocked(_channel_internal);
      }
    }
    _ddata_free(matches);
    this.unlock();
  }
  return m;
}


// documented in the error= version
pragma "no doc"
proc channel.search(re:regexp):reMatch
{
  var e:syserr = ENOERR;
  var ret = this.search(re, error=e);
  if e then this._ch_ioerror(e, "in channel.search");
  return ret;
}

/*  Search for an offset in the channel matching the
    passed regular expression, possibly pulling out capture groups.
    If there is a match, leaves the channel position at the
    match. If there is no match, the channel position will be
    advanced to the end of the channel (or end of the file).

    :arg re: a :record:`Regexp.regexp` record representing a compiled
             regular expression.
    :arg captures: an optional variable number of arguments in which to
                   store the regions of the file matching the capture groups
                   in the regular expression.
    :arg error: optional argument to capture an error code. If this argument
                is not provided and an error is encountered, this function
                will halt with an error message.
    :returns: the region of the channel that matched
 */

proc channel.search(re:regexp, ref captures ...?k, ref error:syserr):reMatch
{
  var m:reMatch;
  on this.home {
    this.lock();
    var nm = captures.size + 1;
    var matches = _ddata_allocate(qio_regexp_string_piece_t, nm);
    error = qio_channel_mark(false, _channel_internal);
    if ! error {
      error = qio_regexp_channel_match(re._regexp,
                                       false, _channel_internal, max(int(64)),
                                       QIO_REGEXP_ANCHOR_UNANCHORED,
                                       /* can_discard */ true,
                                       /* keep_unmatched */ false,
                                       /* keep_whole_pattern */ true,
                                       matches, nm);
    }
    // Don't report "didn't match" errors
    if error == EFORMAT || error == EEOF then error = ENOERR;
    if !error {
      m = _to_reMatch(matches[0]);
      if m.matched {
        // Extract the capture groups.
        _ch_handle_captures(matches, nm, captures, error);

        // Advance to the match.
        qio_channel_revert_unlocked(_channel_internal);
        var cur = qio_channel_offset_unlocked(_channel_internal);
        var target = m.offset;
        error = qio_channel_advance(false, _channel_internal, target - cur);
      } else {
        // If we didn't match... leave the channel position at EOF
        qio_channel_commit_unlocked(_channel_internal);
      }
    }
    _ddata_free(matches);
    this.unlock();
  }
  return m;
}

// documented in the error= version
pragma "no doc"
proc channel.search(re:regexp, ref captures ...?k):reMatch
{
  var e:syserr = ENOERR;
  var ret = this.search(re, (...captures), error=e);
  if e then this._ch_ioerror(e, "in channel.search");
  return ret;
}


// documented in the capture group version
pragma "no doc"
proc channel.match(re:regexp, ref error:syserr):reMatch
{
  var m:reMatch;
  on this.home {
    this.lock();
    var nm = 1;
    var matches = _ddata_allocate(qio_regexp_string_piece_t, nm);
    error = qio_channel_mark(false, _channel_internal);
    if ! error {
      error = qio_regexp_channel_match(re._regexp,
                                       false, _channel_internal, max(int(64)),
                                       QIO_REGEXP_ANCHOR_START,
                                       /* can_discard */ true,
                                       /* keep_unmatched */ true,
                                       /* keep_whole_pattern */ true,
                                       matches, nm);
    }
    // Don't report "didn't match" errors
    if error == EFORMAT || error == EEOF then error = ENOERR;
    if !error {
      m = _to_reMatch(matches[0]);
      if m.matched {
        // Advance to the match.
        qio_channel_revert_unlocked(_channel_internal);
        var cur = qio_channel_offset_unlocked(_channel_internal);
        var target = m.offset;
        error = qio_channel_advance(false, _channel_internal, target - cur);
      } else {
        // If we didn't match... leave the channel position at start
        qio_channel_revert_unlocked(_channel_internal);
      }
    }
    _ddata_free(matches);
    this.unlock();
  }
  return m;
}

// documented in the error= version
pragma "no doc"
proc channel.match(re:regexp):reMatch
{
  var e:syserr = ENOERR;
  var ret = this.match(re, error=e);
  if e then this._ch_ioerror(e, "in channel.match");
  return ret;
}

/* Match, starting at the current position in the channel,
   against a regexp, possibly pulling out capture groups.
   If there was a match, leaves the channel position at
   the match. If there was no match, leaves the channel
   position where it was at the start of this call.

   :arg re: a :record:`Regexp.regexp` record representing a compiled
             regular expression.
   :arg captures: an optional variable number of arguments in which to
                  store the regions of the file matching the capture groups
                  in the regular expression.
   :arg error: optional argument to capture an error code. If this argument
               is not provided and an error is encountered, this function
               will halt with an error message.
   :returns: the region of the channel that matched

 */

proc channel.match(re:regexp, ref captures ...?k, ref error:syserr):reMatch
{
  var m:reMatch;
  on this.home {
    this.lock();
    var nm = 1 + captures.size;
    var matches = _ddata_allocate(qio_regexp_string_piece_t, nm);
    error = qio_channel_mark(false, _channel_internal);
    if !error {
      error = qio_regexp_channel_match(re._regexp,
                               false, _channel_internal, max(int(64)),
                               QIO_REGEXP_ANCHOR_START,
                               /* can_discard */ true,
                               /* keep_unmatched */ true,
                               /* keep_whole_pattern */ true,
                               matches, nm);
    }
    // Don't report "didn't match" errors
    if error == EFORMAT || error == EEOF then error = ENOERR;
    if !error {
      m = _to_reMatch(matches[0]);
      if m.matched {
        // Extract the capture groups.
        _ch_handle_captures(matches, nm, captures, error);

        // Advance to the match.
        qio_channel_revert_unlocked(_channel_internal);
        var cur = qio_channel_offset_unlocked(_channel_internal);
        var target = m.offset;
        error = qio_channel_advance(false, _channel_internal, target - cur);
      } else {
        // If we didn't match... leave the channel position at start
        qio_channel_revert_unlocked(_channel_internal);
      }
    }
    _ddata_free(matches);
    this.unlock();
  }
  return m;
}
// documented in the error= version
pragma "no doc"
proc channel.match(re:regexp, ref captures ...?k):reMatch
{
  var e:syserr = ENOERR;
  var ret = this.match(re, (...captures), error=e);
  if e then this._ch_ioerror(e, "in channel.match");
  return ret;
}



/* Enumerates matches in the string as well as capture groups.

   Yields tuples of :record:`Regexp.reMatch` objects, the 1st is always
   the match for the whole pattern.

   At the time each match is returned, the channel position is
   at the start of that match. Note though that you would have
   to use :proc:`channel.advance` to get to the position of a capture group.

   After returning each match, advances to just after that
   match and looks for another match. Thus, it will not return
   overlapping matches.

   In the end, leaves the channel position at the end of the
   last reported match (if we ran out of maxmatches)
   or at the end of the channel (if we no longer matched)

   Holds the channel lock for the duration of the search.

   :arg re: a :record:`Regexp.regexp` record representing a compiled
            regular expression.
   :arg captures: an optional compile-time constant representing the number
                  of captures to be yielded in tuple elements.
   :arg maxmatches: the maximum number of matches to report.
   :yields: tuples of :record:`Regexp.reMatch` objects, where the first element
            is the whole pattern.  The tuples will have 1+captures elements.

 */
iter channel.matches(re:regexp, param captures=0, maxmatches:int = max(int))
{
  var m:reMatch;
  var go = true;
  var i = 0;
  var error:syserr = ENOERR;
  param nret = captures+1;
  var ret:nret*reMatch;

  lock();
  on this.home do error = _mark();
  if error then this._ch_ioerror(error, "in channel.matches mark");

  while go && i < maxmatches {
    on this.home {
      var nm = 1 + captures;
      var matches = _ddata_allocate(qio_regexp_string_piece_t, nm);
      if ! error {
        error = qio_regexp_channel_match(re._regexp,
                                 false, _channel_internal, max(int(64)),
                                 QIO_REGEXP_ANCHOR_UNANCHORED,
                                 /* can_discard */ true,
                                 /* keep_unmatched */ false,
                                 /* keep_whole_pattern */ true,
                                 matches, nm);
      }
      if !error {
        m = _to_reMatch(matches[0]);
        if m.matched {
          for param i in 1..nret {
            m = _to_reMatch(matches[i-1]);
            _extractMatch(m, ret[i], error);
          }
          // Advance to the start of the match.
          qio_channel_revert_unlocked(_channel_internal);
          error = qio_channel_mark(false, _channel_internal);
          if !error {
            var cur = qio_channel_offset_unlocked(_channel_internal);
            var target = m.offset;
            error = qio_channel_advance(false, _channel_internal, target - cur);
          }
        } else {
          // Stay at the end of the searched region.
        }
      }
      _ddata_free(matches);
      if error then go = false;
    }
    if ! error then yield ret;
    i += 1;
  }
  _commit();
  unlock();
  // Don't report didn't find or end-of-file errors.
  if error == EFORMAT || error == EEOF then error = ENOERR;
  if error then this._ch_ioerror(error, "in channel.matches");
}

} /* end of module */
