.. default-domain:: chpl

.. index::
   single: string
   single: types; string
.. _Chapter-Strings:

=======
Strings
=======

The following documentation shows functions and methods used to
manipulate and process Chapel strings.

The ``string`` type in Chapel represents a sequence of UTF-8
characters and is most often used to represent textual data.

Methods Available in Standard Modules
-------------------------------------

Besides the functions below, some other modules provide routines that are
useful for working with strings. The :mod:`IO` module provides
:proc:`~FormattedIO.string.format` which creates a string that is the result of
formatting. It also includes functions for reading and writing strings.
The :mod:`Regex` module also provides some routines for searching
within strings.

Casts from String to a Numeric Type
-----------------------------------

The ``string`` type supports casting to numeric types. Such casts
will convert the string to the numeric type and throw an error if the string is
invalid. For example:

.. code-block:: chapel

  var number = "a":int;

throws an error when it is executed, but

.. code-block:: chapel

  var number = "1":int;

stores the value ``1`` in ``number``.

To learn more about handling these errors, see the Language-Specification page
on :ref:`Error Handling <Chapter-Error_Handling>`.


Unicode Support
---------------

Chapel strings use the UTF-8 encoding. Note that ASCII strings are a simple
subset of UTF-8 strings, because every ASCII character is a UTF-8 character with
the same meaning.

.. _string.nonunicode:

Non-Unicode Data and Chapel Strings
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For doing string operations on non-Unicode or arbitrary data, consider using
:type:`~Bytes.bytes` instead of string. However, there may be cases where
``string`` must be used with non-Unicode data. Examples of this are
file system and path operations on systems where UTF-8 file names are not
enforced.


In such scenarios, non-UTF-8 data can be escaped and stored in a *string* in a way
that it can be restored when needed. For example:

.. code-block:: chapel

 var myBytes = b"Illegal \xff sequence";  // \xff is non UTF-8
 var myEscapedString = myBytes.decode(policy=decodePolicy.escape);

will escape the illegal ``0xFF``` byte and store it in the string. The escaping
strategy is similar to Python's "surrogate escapes" and is as follows.

 - Each individual byte in an illegal sequence is bitwise-or'ed with ``0xDC00`` to
   create a 2-byte codepoint.
 - Then, this codepoint is encoded in UTF-8 and stored in the string buffer.

This strategy typically results in storing 3 bytes for each byte in the illegal
sequence. Similarly escaped strings can also be created with
:proc:`~String.string.createCopyingBuffer` using a C buffer.

An escaped data sequence can be reconstructed with :proc:`~String.string.encode`:

.. code-block:: chapel

 var reconstructedBytes = myEscapedString.encode(policy=encodePolicy.unescape);
 writeln(myBytes == reconstructedBytes);  // prints true

Alternatively, escaped sequence can be used as-is without reconstructing the
bytes:

.. code-block:: chapel

 var escapedBytes = myEscapedString.encode(policy=encodePolicy.pass);
 writeln(myBytes == escapedBytes);  // prints false

.. note::

  Strings that contain escaped sequences cannot be directly used with
  unformatted I/O functions such as ``writeln``. :ref:`Formatted I/O
  <about-io-formatted-io>` can be used to print such strings with binary
  formatters such as ``%|s``.

.. note::

  The standard :mod:`FileSystem`, :mod:`Path` and :mod:`IO` modules can use
  escaped strings as described above for paths and file names.


Lengths and Offsets in Unicode Strings
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For Unicode strings, and in particular UTF-8 strings, there are several possible
units for offsets or lengths:

 * bytes
 * codepoints
 * graphemes

Most methods on the Chapel string type currently work with codepoint units by
default. For example, :proc:`~String.string.size` returns the length in
codepoints and `int` values passed into :proc:`~String.string.this` are
offsets in codepoint units.

It is possible to indicate byte or codepoint units for indexing in the
string methods by using arguments of type ``byteIndex`` or
``codepointIndex`` respectively.

For speed of indexing with their result values, :proc:`~String.string.find()`
and :proc:`~String.string.rfind()` return a ``byteIndex``.

.. note::

  Support for grapheme units is not implemented at this time.

Using the ``byteIndex`` and ``codepointIndex`` types
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A value of type ``byteIndex`` or ``codepointIndex`` can be passed to certain
*string* functions to indicate that the function should operate with units of
bytes or codepoints. Passing a ``codepointIndex`` has the same behavior as
passing an integral type. See :proc:`~String.string.this` for an example.

Both of these types can be created from an ``int`` via assignment or cast. They
also support addition and subtraction with ``int``. Finally, values of same
types can be compared.

For example, the following function returns a string containing only the second
byte of the argument:

   .. code-block:: chapel

     proc getSecondByte(arg:string) {
       var offsetInBytes = 1:byteIndex;
       return arg[offsetInBytes];
     }

Whereas the following function returns a string containing only the second
codepoint of the argument:

   .. code-block:: chapel

     proc getSecondCodepoint(arg:string) {
       var offsetInCodepoints = 1:codepointIndex;
       return arg[offsetInCodepoints];
     }


.. _String_Methods:

Predefined Routines on Strings
------------------------------

The *string* type:

.. include:: /builtins/String.rst
