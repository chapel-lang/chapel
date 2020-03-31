.. default-domain:: chpl

.. module:: String
   :synopsis: The following documentation shows functions and methods used to

Strings
=======
The following documentation shows functions and methods used to
manipulate and process Chapel strings.

Methods Available in Other Modules
----------------------------------

Besides the functions below, some other modules provide routines that are
useful for working with strings. The :mod:`IO` module provides
`IO.string.format` which creates a string that is the result of
formatting. It also includes functions for reading and writing strings.
The :mod:`Regexp` module also provides some routines for searching
within strings.

Casts from String to a Numeric Type
-----------------------------------

This module supports casts from :record:`string` to numeric types. Such casts
will convert the string to the numeric type and throw an error if the string
is invalid. For example:

.. code-block:: chapel

  var number = "a":int;

throws an error when it is executed, but

.. code-block:: chapel

  var number = "1":int;

stores the value ``1`` in ``number``.

To learn more about handling these errors, see the
:ref:`Error Handling technical note <readme-errorHandling>`.


Activating Unicode Support
--------------------------

Chapel strings normally use the UTF-8 encoding. Note that ASCII strings are a
simple subset of UTF-8 strings, because every ASCII character is a UTF-8
character with the same meaning.

Certain environment variables may need to be set in order to enable UTF-8
support. Setting these environment variables may not be necessary at all on
some systems.

.. note::

  For example, Chapel is currently tested with the following settings:

  ``export LANG=en_US.UTF-8``

  ``export LC_COLLATE=C``

  ``unset LC_ALL``

  LANG sets the default character set.  LC_COLLATE overrides it for
  sorting, so that we get consistent results.  Anything in LC_ALL
  would override everything, so we unset it.

.. note::

  Chapel currently relies upon C multibyte character support and may work
  with other settings of these variables that request non-Unicode multibyte
  character sets. However such a configuration is not regularly tested and
  may not work in the future.

Lengths and Offsets in Unicode Strings
--------------------------------------

For Unicode strings, and in particular UTF-8 strings, there are several possible
units for offsets or lengths:

 * bytes
 * codepoints
 * graphemes

Most methods on the Chapel string type currently work with codepoint units by
default. For example, :proc:`~string.size` returns the length in codepoints
and `int` values passed into :proc:`~string.this` are offsets in codepoint
units.

It is possible to indicate byte or codepoint units for indexing in the
string methods by using arguments of type :record:`byteIndex` or
:record:`codepointIndex` respectively.

For speed of indexing with their result values, :proc:`~string.find()`
and :proc:`~string.rfind()` return a :record:`byteIndex`.

.. note::

  Support for grapheme units is not implemented at this time.

 

.. record:: byteIndex

   
   A value of type :record:`byteIndex` can be passed to certain
   `string` functions to indicate that the function should operate
   with units of bytes. See :proc:`~string.this`.
   
   An `int` can be added to a :record:`byteIndex`, producing
   another :record:`byteIndex`.  One :record:`byteIndex`
   can be subtracted from another, producing an `int` distance
   between them.  A :record:`byteIndex` can also be compared
   with another :record:`byteIndex` or with an `int` .
   
   To create or modify a :record:`byteIndex`, cast or assign it from an
   `int`. For example, the following function returns a string
   containing only the second byte of the argument:
   
   .. code-block:: chapel
   
     proc getSecondByte(arg:string) : int {
       var offsetInBytes = 2:byteIndex;
       return arg[offsetInBytes];
     }
   
   


   .. method:: proc init(i: int)

   .. method:: proc init=(other: byteIndex)

   .. method:: proc init=(i: int)

   .. method:: proc writeThis(f) throws

.. record:: codepointIndex

   
   A value of type :record:`codepointIndex` can be passed to certain
   `string` functions to indicate that the function should operate
   with units of codepoints. See :proc:`~string.this`.
   
   An `int` can be added to a :record:`codepointIndex`, producing
   another :record:`codepointIndex`.  One :record:`codepointIndex`
   can be subtracted from another, producing an `int` distance
   between them.  A :record:`codepointIndex` can also be compared
   with another :record:`codepointIndex` or with an `int` .
   
   To create or modify a :record:`codepointIndex`, cast or assign it from an
   `int`. For example, the following function returns a string
   containing only the second codepoint of the argument:
   
   .. code-block:: chapel
   
     proc getSecondCodepoint(arg:string) : int {
       var offsetInCodepoints = 2:codepointIndex;
       return arg[offsetInCodepoints];
     }
   
   


   .. method:: proc init(i: int)

   .. method:: proc init=(i: int)

   .. method:: proc init=(cpi: codepointIndex)

   .. method:: proc writeThis(f) throws

.. function:: proc createStringWithBorrowedBuffer(x: string)

   
   Creates a new string which borrows the internal buffer of another string. If
   the buffer is freed before the string returned from this function, accessing
   it is undefined behavior.
   
   :arg s: Object to borrow the buffer from
   :type s: `string`
   
   :returns: A new `string`
   

.. function:: proc createStringWithBorrowedBuffer(x: c_string, length = x.size) throws

   
   Creates a new string which borrows the internal buffer of a `c_string`. If
   the buffer is freed before the string returned from this function, accessing
   it is undefined behavior.
   
   :arg s: Object to borrow the buffer from
   :type s: `c_string`
   
   :arg length: Length of the `c_string` in bytes, excluding the terminating
                null byte.
   :type length: `int`
   
   :returns: A new `string`
   

.. function:: proc createStringWithBorrowedBuffer(x: bufferType, length: int, size: int) throws

   
   Creates a new string which borrows the memory allocated for a
   `c_ptr(uint(8))`. If the buffer is freed before the string returned from
   this function, accessing it is undefined behavior.
   
   :arg s: Object to borrow the buffer from
   :type s: `bufferType` (i.e. `c_ptr(uint(8))`)
   
   :arg length: Length of the string stored in `s`, excluding the terminating
                null byte.
   :type length: `int`
   
   :arg size: Size of memory allocated for `s` in bytes
   :type length: `int`
   
   :returns: A new `string`
   

.. function:: proc createStringWithOwnedBuffer(x: c_string, length = x.size) throws

   
   Creates a new string which takes ownership of the internal buffer of a
   `c_string`. The buffer will be freed when the string is deinitialized.
   
   :arg s: Object to take ownership of the buffer from
   :type s: `c_string`
   
   :arg length: Length of the string stored in `s`, excluding the terminating
                null byte.
   :type length: `int`
   
   :returns: A new `string`
   

.. function:: proc createStringWithOwnedBuffer(x: bufferType, length: int, size: int) throws

   
   Creates a new string which takes ownership of the memory allocated for a
   `c_ptr(uint(8))`. The buffer will be freed when the string is deinitialized.
   
   :arg s: Object to take ownership of the buffer from
   :type s: `bufferType` (i.e. `c_ptr(uint(8))`)
   
   :arg length: Length of the string stored in `s`, excluding the terminating
                null byte.
   :type length: `int`
   
   :arg size: Size of memory allocated for `s` in bytes
   :type length: `int`
   
   :returns: A new `string`
   

.. function:: proc createStringWithNewBuffer(x: string)

   
   Creates a new string by creating a copy of the buffer of another string.
   
   :arg s: Object to copy the buffer from
   :type s: `string`
   
   :returns: A new `string`
   

.. function:: proc createStringWithNewBuffer(x: c_string, length = x.size, policy = decodePolicy.strict) throws

   
   Creates a new string by creating a copy of the buffer of a `c_string`.
   
   :arg s: Object to copy the buffer from
   :type s: `c_string`
   
   :arg length: Length of the `c_string` in bytes, excluding the terminating
                null byte.
   :type length: `int`
   
   :returns: A new `string`
   

.. function:: proc createStringWithNewBuffer(x: bufferType, length: int, size = length+1, policy = decodePolicy.strict) throws

.. record:: string

   .. method:: proc init=(s: string)

   .. method:: proc init=(cs: c_string)

   .. method:: proc length

      Deprecated - please use :proc:`string.size`. 

   .. method:: proc size

      
      :returns: The number of codepoints in the string.
      

   .. method:: proc indices

      
      :returns: The indices that can be used to index into the string
                (i.e., the range ``1..this.size``)
      

   .. method:: proc numBytes

      
      :returns: The number of bytes in the string.
      

   .. method:: proc numCodepoints

      
      :returns: The number of codepoints in the string, assuming the
                string is correctly-encoded UTF-8.
      

   .. method:: proc localize(): string

      
      Gets a version of the :record:`string` that is on the currently
      executing locale.
      
      :returns: A shallow copy if the :record:`string` is already on the
                current locale, otherwise a deep copy is performed.
      

   .. method:: proc c_str(): c_string

      
      Get a `c_string` from a :record:`string`.
      
      .. warning::
      
          This can only be called safely on a :record:`string` whose home is
          the current locale.  This property can be enforced by calling
          :proc:`string.localize()` before :proc:`~string.c_str()`. If the
          string is remote, the program will halt.
      
      For example:
      
      .. code-block:: chapel
      
          var my_string = "Hello!";
          on different_locale {
            printf("%s", my_string.localize().c_str());
          }
      
      :returns:
          A `c_string` that points to the underlying buffer used by this
          :record:`string`. The returned `c_string` is only valid when used
          on the same locale as the string.
      

   .. method:: proc encode(policy = encodePolicy.pass): bytes

      
      Returns a :record:`~Bytes.bytes` from the given :record:`string`. If the
      string contains some escaped non-UTF8 bytes, `policy` argument determines
      the action.
        
      :arg policy: `encodePolicy.pass` directly copies the (potentially escaped)
                    data, `encodePolicy.unescape` recovers the escaped bytes
                    back.
      
      :returns: :record:`~Bytes.bytes`
      

   .. itermethod:: iter items(): string

      
      Iterates over the string character by character.
      
      For example:
      
      .. code-block:: chapel
      
        var str = "abcd";
        for c in str {
          writeln(c);
        }
      
      Output::
      
        a
        b
        c
        d
      

   .. itermethod:: iter these(): string

      
      Iterates over the string character by character, yielding 1-codepoint
      strings. (A synonym for :iter:`items`)
      
      For example:
      
      .. code-block:: chapel
      
        var str = "abcd";
        for c in str {
          writeln(c);
        }
      
      Output::
      
        a
        b
        c
        d
      

   .. itermethod:: iter chpl_bytes(): byteType

      
      Iterates over the string byte by byte.
      

   .. itermethod:: iter codepoints(): int(32)

      
      Iterates over the string Unicode character by Unicode character.
      

   .. method:: proc toByte(): uint(8)

      
      :returns: The value of a single-byte string as an integer.
      

   .. method:: proc byte(i: int): uint(8)

      
      :returns: The value of the `i` th byte as an integer.
      

   .. method:: proc toCodepoint(): int(32)

      
      :returns: The value of a single-codepoint string as an integer.
      

   .. method:: proc codepoint(i: int): int(32)

      
      :returns: The value of the `i` th multibyte character as an integer.
      

   .. method:: proc this(i: byteIndex): string

      
      Return the codepoint starting at the `i` th byte in the string
      
      :returns: A string with the complete multibyte character starting at the
                specified byte index from ``1..string.numBytes``
      

   .. method:: proc this(i: codepointIndex): string

      
      Return the `i` th codepoint in the string. (A synonym for :proc:`item`)
      
      :returns: A string with the complete multibyte character starting at the
                specified codepoint index from ``1..string.numCodepoints``
      

   .. method:: proc this(i: int): string

      
      Return the `i` th codepoint in the string. (A synonym for :proc:`item`)
      
      :returns: A string with the complete multibyte character starting at the
                specified codepoint index from ``1..string.numCodepoints``
      

   .. method:: proc item(i: codepointIndex): string

      
      Return the `i` th codepoint in the string
      
      :returns: A string with the complete multibyte character starting at the
                specified codepoint index from ``1..string.numCodepoints``
      

   .. method:: proc item(i: int): string

      
      Return the `i` th codepoint in the string
      
      :returns: A string with the complete multibyte character starting at the
                specified codepoint index from ``1..string.numCodepoints``
      

   .. method:: proc this(r: range(?)): string

   .. method:: proc isEmpty(): bool

      
      :returns: * `true`  -- when the string is empty
                * `false` -- otherwise
      

   .. method:: proc startsWith(needles: string ...): bool

      
      :arg needles: A varargs list of strings to match against.
      
      :returns: * `true`  -- when the string begins with one or more of the `needles`
                * `false` -- otherwise
      

   .. method:: proc endsWith(needles: string ...): bool

      
      :arg needles: A varargs list of strings to match against.
      
      :returns: * `true`  -- when the string ends with one or more of the `needles`
                * `false` -- otherwise
      

   .. method:: proc find(needle: string, region: range(?) = 1: byteIndex..): byteIndex

   .. method:: proc rfind(needle: string, region: range(?) = 1: byteIndex..): byteIndex

      
      :arg needle: the string to search for
      :arg region: an optional range defining the substring to search within,
                   default is the whole string. Halts if the range is not
                   within ``1..string.size``
      
      :returns: the index of the first occurrence from the right of `needle`
                within a string, or 0 if the `needle` is not in the string.
      

   .. method:: proc count(needle: string, region: range(?) = 1..): int

      
      :arg needle: the string to search for
      :arg region: an optional range defining the substring to search within,
                   default is the whole string. Halts if the range is not
                   within ``1..string.size``
      
      :returns: the number of times `needle` occurs in the string
      

   .. method:: proc replace(needle: string, replacement: string, count: int = -1): string

   .. itermethod:: iter split(sep: string, maxsplit: int = -1, ignoreEmpty: bool = false)

   .. itermethod:: iter split(maxsplit: int = -1)

   .. method:: proc join(const ref x: string ...): string

      
      Returns a new string, which is the concatenation of all of the strings
      passed in with the receiving string inserted between them.
      
      .. code-block:: chapel
      
          var x = "|".join("a","10","d");
          writeln(x); // prints: "a|10|d"
      

   .. method:: proc join(const ref x): string

      
      Same as the varargs version, but with a homogeneous tuple of strings.
      
      .. code-block:: chapel
      
          var x = "|".join("a","10","d");
          writeln(x); // prints: "a|10|d"
      

   .. method:: proc join(const ref S: [] string): string

      
      Same as the varargs version, but with all the strings in an array.
      
      .. code-block:: chapel
      
          var x = "|".join(["a","10","d"]);
          writeln(x); // prints: "a|10|d"
      

   .. method:: proc strip(chars: string = " \t\r\n", leading = true, trailing = true): string

      
      :arg chars: A string containing each character to remove.
                  Defaults to `" \\t\\r\\n"`.
      :arg leading: Indicates if leading occurrences should be removed.
                    Defaults to `true`.
      :arg trailing: Indicates if trailing occurrences should be removed.
                     Defaults to `true`.
      
      :returns: A new string with `leading` and/or `trailing` occurrences of
                characters in `chars` removed as appropriate.
      

   .. method:: proc partition(sep: string): 3*(string)

      
      Splits the string on `sep` into a `3*string` consisting of the section
      before `sep`, `sep`, and the section after `sep`. If `sep` is not found,
      the tuple will contain the whole string, and then two empty strings.
      

   .. method:: proc isUpper(): bool

      
      Checks if all the characters in the string are either uppercase (A-Z) or
      uncased (not a letter).
      
       :returns: * `true`  -- if the string contains at least one uppercase
                              character and no lowercase characters, ignoring
                              uncased characters.
                 * `false` -- otherwise
      

   .. method:: proc isLower(): bool

      
      Checks if all the characters in the string are either lowercase (a-z) or
      uncased (not a letter).
      
       :returns: * `true`  -- when there are no uppercase characters in the string.
                 * `false` -- otherwise
      

   .. method:: proc isSpace(): bool

      
      Checks if all the characters in the string are whitespace (' ', '\t',
      '\n', '\v', '\f', '\r').
      
       :returns: * `true`  -- when all the characters are whitespace.
                 * `false` -- otherwise
      

   .. method:: proc isAlpha(): bool

      
      Checks if all the characters in the string are alphabetic (a-zA-Z).
      
       :returns: * `true`  -- when the characters are alphabetic.
                 * `false` -- otherwise
      

   .. method:: proc isDigit(): bool

      
      Checks if all the characters in the string are digits (0-9).
      
       :returns: * `true`  -- when the characters are digits.
                 * `false` -- otherwise
      

   .. method:: proc isAlnum(): bool

      
      Checks if all the characters in the string are alphanumeric (a-zA-Z0-9).
      
       :returns: * `true`  -- when the characters are alphanumeric.
                 * `false` -- otherwise
      

   .. method:: proc isPrintable(): bool

      
      Checks if all the characters in the string are printable.
      
       :returns: * `true`  -- when the characters are printable.
                 * `false` -- otherwise
      

   .. method:: proc isTitle(): bool

      
      Checks if all uppercase characters are preceded by uncased characters,
      and if all lowercase characters are preceded by cased characters.
      
      :returns: * `true`  -- when the condition described above is met.
                * `false` -- otherwise
      

   .. method:: proc toLower(): string

      
      :returns: A new string with all uppercase characters replaced with their
                lowercase counterpart.
      
      .. note::
        
        The case change operation is not currently performed on characters whose
        cases take different number of bytes to represent in Unicode mapping.
      

   .. method:: proc toUpper(): string

      
      :returns: A new string with all lowercase characters replaced with their
                uppercase counterpart.
      
      .. note::
        
        The case change operation is not currently performed on characters whose
        cases take different number of bytes to represent in Unicode mapping.
      

   .. method:: proc toTitle(): string

      
      :returns: A new string with all cased characters following an uncased
                character converted to uppercase, and all cased characters
                following another cased character converted to lowercase.
      
      .. note::
        
        The case change operation is not currently performed on characters whose
        cases take different number of bytes to represent in Unicode mapping.
      

.. function:: proc =(ref lhs: byteIndex, rhs: int)

   
   Copies the int `rhs` into the byteIndex `lhs`.
   

.. function:: proc =(ref lhs: codepointIndex, rhs: int)

   
   Copies the int `rhs` into the codepointIndex `lhs`.
   

.. function:: proc =(ref lhs: string, rhs: string)

   
   Copies the string `rhs` into the string `lhs`.
   

.. function:: proc =(ref lhs: string, rhs_c: c_string)

   
   Copies the c_string `rhs_c` into the string `lhs`.
   
   Halts if `lhs` is a remote string.
   

.. function:: proc +(s0: string, s1: string)

   
   :returns: A new string which is the result of concatenating `s0` and `s1`
   

.. function:: proc *(s: string, n: integral)

   
   :returns: A new string which is the result of repeating `s` `n` times.
             If `n` is less than or equal to 0, an empty string is returned.
   
   For example:
   
   .. code-block:: chapel
   
      writeln("Hello! " * 3);
   
   Results in::
   
     Hello! Hello! Hello!
   

.. function:: proc +=(ref lhs: string, const ref rhs: string): void

   
   Appends the string `rhs` to the string `lhs`.
   

.. function:: proc codepointToString(i: int(32))

   
   :returns: A string storing the complete multibyte character sequence
             that corresponds to the codepoint value `i`.
   

