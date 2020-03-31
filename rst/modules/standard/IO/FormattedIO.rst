.. default-domain:: chpl

.. module:: FormattedIO
   :synopsis: Support for formatted input and output.

FormattedIO
===========
**Usage**

.. code-block:: chapel

   use IO.FormattedIO;




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
 * when reading - read exactly 17 Unicode codepoints
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
   (e.g. Unicode codepoints) to input.

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
    width. Precision will impact any floating point values output
    in this conversion.
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

 

.. method:: proc channel.writef(fmtStr: ?t, const args ...?k): bool throws

   
   
   Write arguments according to a format. See
   :ref:`about-io-formatted-io`.
   
   :arg fmt: the format as string or bytes
   
   :arg args: 0 or more arguments to write
   :returns: true
   
   :throws IllegalArgumentError: if an unsupported argument type is encountered.
   :throws SystemError: if the arguments could not be written.
   

.. method:: proc channel.writef(fmtStr: ?t): bool throws

.. method:: proc channel.readf(fmtStr: ?t, ref args ...?k): bool throws

   
   
   Read arguments according to a format. See
   :ref:`about-io-formatted-io`.
   
   .. note::
   
      Intents for all arguments except the format string are `ref`. If `readf`
      is used with formats that require an additional argument such as `%*i` and
      `%*S`, then those arguments cannot be constants. Instead, store the value
      into a variable and pass that.
   
   :arg fmt: the format as string or bytes
   :arg args: the arguments to read
   :returns: true if all arguments were read according to the format string,
             false on EOF.
   
   :throws SystemError: Thrown if the arguments could not be read.
   

.. function:: proc readf(fmt: string, ref args ...?k): bool throws

   Call ``stdin.readf``; see :proc:`channel.readf`. 

.. method:: proc channel.skipField() throws

   
   Skip a field in the current aggregate format. This method is currently only
   supported for JSON format and returns ENOTSUP for other formats. In other
   formats, it may not be possible in general to know when a field ends.
   
   The field skipped includes a field name and value but not a following
   separator. For example, for a JSON format channel, given the input:
   
   ::
   
      "fieldName":"fieldValue", "otherField":3
   
   this function will skip to (but leave unread) the comma after
   the first field value.
   
   :throws SystemError: Thrown if the field could not be skipped.
   

.. method:: proc string.format(args ...?k): string throws

   
   
   Return a new string consisting of values formatted according to a
   format string.  See :ref:`about-io-formatted-io`.
   
   :arg this: the format string
   :arg args: the arguments to format
   :returns: the resulting string
   
   :throws SystemError: Thrown if the string could not be formatted.
   

.. method:: proc bytes.format(args ...?k): bytes throws

   
   
   Return a new bytes consisting of values formatted according to a
   format bytes.  See :ref:`about-io-formatted-io`.
   
   :arg this: the format bytes
   :arg args: the arguments to format
   :returns: the resulting bytes
   
   :throws SystemError: Thrown if the bytes could not be formatted.
   

.. method:: proc channel.extractMatch(m: reMatch, ref arg) throws

   Sets arg to the string of a match.
   
   Assumes that the channel has been marked before where
   the captures are being returned. Will change the channel
   position to just after the match. Will not do anything
   if error is set.
   
   :arg m: a :record:`Regexp.reMatch` storing a location that matched
   :arg arg: an argument to retrieve the match into. If it is not a string,
             the string match will be cast to arg.type.
   
   :throws SystemError: Thrown if a match could not be extracted.
   

.. method:: proc channel.search(re: regexp(?), ref captures ...?k): reMatch throws

   Search for an offset in the channel matching the
   passed regular expression, possibly pulling out capture groups.
   If there is a match, leaves the channel position at the
   match. If there is no match, the channel position will be
   advanced to the end of the channel (or end of the file).
   
   Throws a SystemError if an error occurs.
   
   :arg re: a :record:`Regexp.regexp` record representing a compiled
            regular expression.
   :arg captures: an optional variable number of arguments in which to
                  store the regions of the file matching the capture groups
                  in the regular expression.
   :returns: the region of the channel that matched
   

.. method:: proc channel.match(re: regexp(?), ref captures ...?k, ref error: syserr): reMatch

   Match, starting at the current position in the channel,
   against a regexp, possibly pulling out capture groups.
   If there was a match, leaves the channel position at
   the match. If there was no match, leaves the channel
   position where it was at the start of this call.
   
   :arg re: a :record:`Regexp.regexp` record representing a compiled
             regular expression.
   :arg captures: an optional variable number of arguments in which to
                  store the regions of the file matching the capture groups
                  in the regular expression.
   :returns: the region of the channel that matched
   

.. itermethod:: iter channel.matches(re: regexp(?), param captures = 0, maxmatches: int = max(int))

   Enumerates matches in the string as well as capture groups.
   
   Yields tuples of :record:`Regexp.reMatch` objects, the 1st is always
   the match for the whole pattern.
   
   At the time each match is returned, the channel position is
   at the start of that match. Note though that you would have
   to use :proc:`IO.channel.advance` to get to the position of a capture group.
   
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
   

