.. _Chapter-Lexical_Structure:

Lexical Structure
=================

This section describes the lexical components of Chapel programs. The
purpose of lexical analysis is to separate the raw input stream into a
sequence of tokens suitable for input to the parser.

.. _Comments:

Comments
--------

Two forms of comments are supported. All text following the consecutive
characters ``//`` and before the end of the line is in a comment. All
text following the consecutive characters ``/*`` and before the
consecutive characters ``*/`` is in a comment. A comment delimited by
``/*`` and ``*/`` can be nested in another comment delimited by ``/*``
and ``*/``

Comments, including the characters that delimit them, do not affect the
behavior of the program (except in delimiting tokens). If the delimiters
that start the comments appear within a bytes or string literal, they do
not start a comment but rather are part of the bytes or string literal.

   *Example*.

   The following program makes use of both forms of comment:
   

   .. code-block:: chapel

      /*
       *  main function
       */
      proc main() {
        writeln("hello, world"); // output greeting with new line
      }

.. _White_Space:

White Space
-----------

White-space characters are spaces, tabs, line feeds, form feeds, and
carriage returns. Along with comments, they delimit tokens, but are
otherwise ignored.

.. _Case_Sensitivity:

Case Sensitivity
----------------

Chapel is a case sensitive language.

   *Example*.

   The following identifiers are considered distinct: ``chapel``,
   ``Chapel``, and ``CHAPEL``.

.. _Tokens:

Tokens
------

Tokens include identifiers, keywords, literals, operators, and
punctuation.

.. _Identifiers:

Identifiers
~~~~~~~~~~~

An identifier in Chapel is a sequence of characters that starts with a
lowercase or uppercase letter or an underscore and is optionally
followed by a sequence of lowercase or uppercase letters, digits,
underscores, and dollar-signs. Identifiers are designated by the
following syntax: 

.. code-block:: syntax

   identifier:
     letter-or-underscore legal-identifier-chars[OPT]

   legal-identifier-chars:
     legal-identifier-char legal-identifier-chars[OPT]

   legal-identifier-char:
     letter-or-underscore
     digit
     `$'

   letter-or-underscore:
     letter
     `_'

   letter: one of
     `A' `B' `C' `D' `E' `F' `G' `H' `I' `J' `K' `L' `M' `N' `O' `P' `Q' `R' `S' `T' `U' `V' `W' `X' `Y' `Z'
     `a' `b' `c' `d' `e' `f' `g' `h' `i' `j' `k' `l' `m' `n' `o' `p' `q' `r' `s' `t' `u' `v' `w' `x' `y' `z'

   digit: one of
     `0' `1' `2' `3' `4' `5' `6' `7' `8' `9'

..

   *Rationale*.

   Why include “$” in the language? The inclusion of the $ character is
   meant to assist programmers using sync and single variables by
   supporting a convention (a $ at the end of such variables) in order
   to help write properly synchronized code. It is felt that marking
   such variables is useful since using such variables could result in
   deadlocks.

   *Example*.

   The following are legal identifiers: ``Cray1``, ``syncvar$``,
   ``legalIdentifier``, and ``legal_identifier``.

.. _Keywords:

Keywords
~~~~~~~~

The following identifiers are reserved as keywords:



.. code-block:: chapel

   _
   align
   as
   atomic
   begin
   bool
   borrowed
   break
   by
   bytes
   catch
   class
   cobegin
   coforall
   complex
   config
   const
   continue
   defer
   delete
   dmapped
   do
   domain
   else
   enum
   except
   export
   extern
   false
   for
   forall
   forwarding
   if
   imag
   in
   index
   inline
   inout
   int
   iter
   label
   let
   lifetime
   local
   locale
   module
   new
   nil
   noinit
   on
   only
   otherwise
   out
   override
   owned
   param
   private
   prototype
   proc
   public
   real
   record
   reduce
   ref
   require
   return
   scan
   select
   serial
   shared
   single
   sparse
   string
   subdomain
   sync
   then
   this
   throw
   throws
   true
   try
   type
   uint
   union
   unmanaged
   use
   var
   when
   where
   while
   with
   yield
   zip

The following identifiers are keywords reserved for future use:



.. code-block:: chapel

   lambda
   pragma
   primitive

.. _Literals:

Literals
~~~~~~~~

Bool literals are designated by the following syntax: 

.. code-block:: syntax

   bool-literal: one of
     `true' `false'

Signed and unsigned integer literals are designated by the following
syntax: 

.. code-block:: syntax

   integer-literal:
     digits
     `0x' hexadecimal-digits
     `0X' hexadecimal-digits
     `0o' octal-digits
     `0O' octal-digits
     `0b' binary-digits
     `0B' binary-digits

   digits:
     digit
     digit separator-digits

   separator-digits:
     digit
     `_'
     digit separator-digits
     `_' separator-digits

   hexadecimal-digits:
     hexadecimal-digit
     hexadecimal-digit separator-hexadecimal-digits

   separator-hexadecimal-digits:
     hexadecimal-digit
     `_'
     hexadecimal-digit separator-hexadecimal-digits
     `_' separator-hexadecimal-digits

   hexadecimal-digit: one of
     `0' `1' `2' `3' `4' `5' `6' `7' `8' `9' `A' `B' `C' `D' `E' `F' `a' `b' `c' `d' `e' `f'

   octal-digits:
     octal-digit
     octal-digit separator-octal-digits

   separator-octal-digits:
     octal-digit
     `_'
     octal-digit separator-octal-digits
     `_' separator-octal-digits

   octal-digit: one of
     `0' `1' `2' `3' `4' `5' `6' `7'

   binary-digits:
     binary-digit
     binary-digit separator-binary-digits

   separator-binary-digits:
     binary-digit
     `_'
     binary-digit separator-binary-digits
     `_' separator-binary-digits

   binary-digit: one of
     `0' `1'

Integer literals in the range 0 to max(\ ``int``),
 :ref:`Signed_and_Unsigned_Integral_Types`, have type ``int`` and
the remaining literals have type ``uint``.

   *Rationale*.

   Why are there no suffixes on integral literals? Suffixes, like those
   in C, are not necessary. Explicit conversions can then be used to
   change the type of the literal to another integer size.

..

   *Rationale*.

   Underscores can be used to group the digits of numbers for
   legibility. For example: 

   .. code-block:: chapel

      var i = 1_234_567_890;
      var x = 0xFF_FF_12_34;

Real literals are designated by the following syntax: 

.. code-block:: syntax

   real-literal:
     digits[OPT] . digits exponent-part[OPT]
     digits .[OPT] exponent-part
     `0x' hexadecimal-digits[OPT] . hexadecimal-digits p-exponent-part[OPT]
     `0X' hexadecimal-digits[OPT] . hexadecimal-digits p-exponent-part[OPT]
     `0x' hexadecimal-digits .[OPT] p-exponent-part
     `0X' hexadecimal-digits .[OPT] p-exponent-part

   exponent-part:
     `e' sign[OPT] digits
     `E' sign[OPT] digits

   p-exponent-part:
     `p' sign[OPT] digits
     `P' sign[OPT] digits


   sign: one of
     + -

..

   *Rationale*.

   Why can’t a real literal end with ’.’? There is a lexical ambiguity
   between real literals ending in ’.’ and the range operator ’..’ that
   makes it difficult to parse. For example, we want to parse ``1..10``
   as a range from 1 to 10 without concern that ``1.`` is a real
   literal.

Hexadecimal real literals are supported with a hexadecimal integer and
fractional part. Because ’e’ could be a hexadecimal character, the
exponent for these literals is instead marked with ’p’ or ’P’. The
exponent value follows and is written in decimal.

The type of a real literal is ``real``. Explicit conversions are
necessary to change the size of the literal.

Imaginary literals are designated by the following syntax:



.. code-block:: syntax

   imaginary-literal:
     real-literal `i'
     integer-literal `i'

The type of an imaginary literal is ``imag``. Explicit conversions are
necessary to change the size of the literal.

There are no complex literals. Rather, a complex value can be specified
by adding or subtracting a real literal with an imaginary literal.
Alternatively, a 2-tuple of integral or real expressions can be cast to
a complex such that the first component becomes the real part and the
second component becomes the imaginary part.

   *Example*.

   The following expressions are identical: ``1.0 + 2.0i`` and
   ``(1.0, 2.0):complex``.

Interpreted string literals are designated by the following syntax:


.. code-block:: syntax

   interpreted-string-literal:
     " double-quote-delimited-characters[OPT] "
     ' single-quote-delimited-characters[OPT] '

   double-quote-delimited-characters:
     string-character double-quote-delimited-characters[OPT]
     ' double-quote-delimited-characters[OPT]

   single-quote-delimited-characters:
     string-character single-quote-delimited-characters[OPT]
     " single-quote-delimited-characters[OPT]

   string-character:
     `any character except the double quote, single quote, or new line'
     simple-escape-character
     hexadecimal-escape-character

   simple-escape-character: one of
     \' \" \? \\ \a \b \f \n \r \t \v

   hexadecimal-escape-character:
     `\x' hexadecimal-digits

Uninterpreted string literals are designated by the following syntax:

.. code-block:: syntax

   uninterpreted-string-literal:
     """ uninterpreted-double-quote-delimited-characters """
     ''' uninterpreted-single-quote-delimited-characters '''

   uninterpreted-double-quote-delimited-characters:
     uninterpreted-double-quote-string-character uninterpreted-double-quote-delimited-characters[OPT]

   uninterpreted-single-quote-delimited-characters:
     uninterpreted-single-quote-string-character uninterpreted-single-quote-delimited-characters[OPT]

   uninterpreted-double-quote-string-character:
     `any character except three double quotes in a row'

   uninterpreted-single-quote-string-character:
     `any character except three single quotes in a row'

Uninterpreted string literals do not interpret their contents, so for
example ``"""\n"""`` is not a newline, but rather two
characters ``\`` and ``n``. Uninterpreted string literals
may span multiple lines and the literal newline characters will be
included in the string.

A string literal can be either interpreted or uninterpreted.


.. code-block:: syntax

   string-literal:
     interpreted-string-literal
     uninterpreted-string-literal

Interpreted bytes literals are designated by the following syntax:


.. code-block:: syntax

   interpreted-bytes-literal:
     b" double-quote-delimited-characters[OPT] "
     b' single-quote-delimited-characters[OPT] '

Uninterpreted bytes literals are designated by the following syntax:


.. code-block:: syntax

   uninterpreted-bytes-literal:
     b""" uninterpreted-double-quote-delimited-characters """
     b''' uninterpreted-single-quote-delimited-characters '''

Uninterpreted bytes literals do not interpret their contents, so for
example ``b"""\n"""`` is not a newline, but rather two
characters ``\`` and ``n``. Uninterpreted bytes literals
may span multiple lines and the literal newline characters will be
included in the bytes.

A bytes literal can be either interpreted or uninterpreted. 

.. code-block:: syntax

   bytes-literal:
     interpreted-bytes-literal
     uninterpreted-bytes-literal

.. _Operators_and_Punctuation:

Operators and Punctuation
~~~~~~~~~~~~~~~~~~~~~~~~~

The following operators and punctuation are defined in the syntax of the
language:

=================================================================================================== =============================
**symbols**                                                                                         **use**
=================================================================================================== =============================
``=``                                                                                               assignment
``+=`` ``-=`` ``*=`` ``/=`` ``**=`` ``%=`` ``&=`` ``|=`` ``^=`` ``&&=`` ``||=`` ``<<=`` ``>>=``     compound assignment
``<=>``                                                                                             swap
``<~>``                                                                                             I/O
``..`` ``..<``                                                                                      range specifier
``by``                                                                                              range/domain stride specifier
``#``                                                                                               range count operator
``...``                                                                                             variable argument lists
``&&`` ``||`` ``!`` ``&`` ``|`` ``^`` ``~`` ``<<`` ``>>``                                           logical/bitwise operators
``==`` ``!=`` ``<=`` ``>=`` ``<`` ``>``                                                             relational operators
``+`` ``-`` ``*`` ``/`` ``%`` ``**``                                                                arithmetic operators
``:``                                                                                               type specifier
``;``                                                                                               statement separator
``,``                                                                                               expression separator
``.``                                                                                               member access
``?``                                                                                               type query
``" '``                                                                                             string delimiters
=================================================================================================== =============================

.. _Grouping_Tokens:

Grouping Tokens
~~~~~~~~~~~~~~~

The following braces are part of the Chapel language:

========== ===================================================================
**braces** **use**
========== ===================================================================
``( )``    parenthesization, function calls, and tuples
``[ ]``    array literals, array types, forall expressions, and function calls
``{ }``    domain literals, block statements
========== ===================================================================

