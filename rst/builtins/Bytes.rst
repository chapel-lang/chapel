.. default-domain:: chpl

.. module:: Bytes
   :synopsis: The following document shows functions and methods used to manipulate and

Bytes
=====
**Usage**

.. code-block:: chapel

   use Bytes;


The following document shows functions and methods used to manipulate and
process Chapel bytes variables. :record:`bytes` is similar to a string but
allows arbitrary data to be stored in it. Methods on bytes that interpret the
data as characters assume that the bytes are ASCII characters.

Creating :record:`bytes`
------------------------

- A :record:`bytes` can be created using the literals similar to strings:

.. code-block:: chapel

   var b = b"my bytes";

- If you need to create :record:`bytes` using a specific buffer (i.e. data in
  another :record:`bytes`, a `c_string` or a C pointer) you can use the
  factory functions shown below, such as :proc:`createBytesWithNewBuffer`.

:record:`bytes` and :record:`~String.string`
--------------------------------------------

As :record:`bytes` can store arbitrary data, any :record:`~String.string` can be
cast to :record:`bytes`. In that event, the bytes will store UTF-8 encoded
character data. However, a :record:`bytes` can contain non-UTF-8 bytes and needs
to be decoded to be converted to string.

.. code-block:: chapel

   var s = "my string";
   var b = s:bytes;  // this is legal

   /*
    The reverse is not. The following is a compiler error:

    var s2 = b:string;
   */

   var s2 = b.decode(); // you need to decode a bytes to convert it to a string

See the documentation for the :proc:`~bytes.decode` method for details.

Similarly, a :record:`bytes` can be initialized using a string:

.. code-block:: chapel

   var s = "my string";
   var b: bytes = s;

Casts from :record:`bytes` to a Numeric Type
--------------------------------------------

This module supports casts from :record:`bytes` to numeric types. Such casts
will interpret the :record:`bytes` as ASCII characters and convert it to the
numeric type and throw an error if the :record:`bytes` does not match the
expected format of a number. For example:

.. code-block:: chapel

  var b = b"a";
  var number = b:int;

throws an error when it is executed, but

.. code-block:: chapel

  var b = b"1";
  var number = b:int;

stores the value ``1`` in ``number``.

To learn more about handling these errors, see the
:ref:`Error Handling technical note <readme-errorHandling>`.
 

.. function:: proc createBytesWithBorrowedBuffer(x: bytes)

   
   Creates a new :record:`bytes` which borrows the internal buffer of another
   :record:`bytes`. If the buffer is freed before the :record:`bytes` returned
   from this function, accessing it is undefined behavior.
   
   :arg s: The :record:`bytes` to borrow the buffer from
   
   :returns: A new :record:`bytes`
   

.. function:: proc createBytesWithBorrowedBuffer(x: c_string, length = x.size)

   
   Creates a new :record:`bytes` which borrows the internal buffer of a
   `c_string`. If the buffer is freed before the :record:`bytes` returned from
   this function, accessing it is undefined behavior.
   
   :arg s: `c_string` to borrow the buffer from
   
   :arg length: Length of `s`'s buffer, excluding the terminating
                null byte.
   :type length: `int`
   
   :returns: A new :record:`bytes`
   

.. function:: proc createBytesWithBorrowedBuffer(x: bufferType, length: int, size: int)

   
   Creates a new :record:`bytes` which borrows the memory allocated for a
   `c_ptr(uint(8))`. If the buffer is freed before the :record:`bytes`
   returned from this function, accessing it is undefined behavior.
   
   :arg s: Buffer to borrow
   :type s: `bufferType` (i.e. `c_ptr(uint(8))`)
   
   :arg length: Length of the buffer `s`, excluding the terminating null byte.
   
   :arg size: Size of memory allocated for `s` in bytes
   
   :returns: A new :record:`bytes`
   

.. function:: proc createBytesWithOwnedBuffer(x: c_string, length = x.size)

   
   Creates a new :record:`bytes` which takes ownership of the internal buffer of a
   `c_string`.The buffer will be freed when the :record:`bytes` is deinitialized.
   
   :arg s: The `c_string` to take ownership of the buffer from
   
   :arg length: Length of `s`'s buffer, excluding the terminating null byte.
   :type length: `int`
   
   :returns: A new :record:`bytes`
   

.. function:: proc createBytesWithOwnedBuffer(x: bufferType, length: int, size: int)

   
   Creates a new :record:`bytes` which takes ownership of the memory allocated
   for a `c_ptr(uint(8))`. The buffer will be freed when the :record:`bytes`
   is deinitialized.
   
   :arg s: The buffer to take ownership of
   :type s: `bufferType` (i.e. `c_ptr(uint(8))`)
   
   :arg length: Length of the buffer `s`, excluding the terminating null byte.
   
   :arg size: Size of memory allocated for `s` in bytes
   
   :returns: A new :record:`bytes`
   

.. function:: proc createBytesWithNewBuffer(x: bytes)

   
   Creates a new :record:`bytes` by creating a copy of the buffer of another
   :record:`bytes`.
   
   :arg s: The :record:`bytes` to copy the buffer from
   
   :returns: A new :record:`bytes`
   

.. function:: proc createBytesWithNewBuffer(x: c_string, length = x.size)

   
   Creates a new :record:`bytes` by creating a copy of the buffer of a
   `c_string`.
   
   :arg s: The `c_string` to copy the buffer from
   
   :arg length: Length of `s`'s buffer, excluding the terminating null byte.
   :type length: `int`
   
   :returns: A new :record:`bytes`
   

.. function:: proc createBytesWithNewBuffer(x: bufferType, length: int, size = length+1)

   
   Creates a new :record:`bytes` by creating a copy of a buffer.
   
   :arg s: The buffer to copy
   :type s: `bufferType` (i.e. `c_ptr(uint(8))`)
   
   :arg length: Length of buffer `s`, excluding the terminating null byte.
   
   :arg size: Size of memory allocated for `s` in bytes
   
   :returns: A new :record:`bytes`
   

.. record:: bytes

   .. method:: proc length

      Deprecated - please use :proc:`bytes.size`. 

   .. method:: proc size

      
      :returns: The number of bytes in the :record:`bytes`.
      

   .. method:: proc indices

      
      :returns: The indices that can be used to index into the bytes
                (i.e., the range ``1..this.size``)
      

   .. method:: proc numBytes

      
      :returns: The number of bytes in the :record:`bytes`.
      

   .. method:: proc localize(): bytes

      
      Gets a version of the :record:`bytes` that is on the currently
      executing locale.
      
      :returns: A shallow copy if the :record:`bytes` is already on the
                current locale, otherwise a deep copy is performed.
      

   .. method:: proc c_str(): c_string

      
      Gets a `c_string` from a :record:`bytes`. The returned `c_string` shares
      the buffer with the :record:`bytes`.
      
      :returns: A `c_string`
      

   .. method:: proc item(i: int): bytes

      
      Gets an ASCII character from the :record:`bytes`
      
      :arg i: The index
      
      :returns: A 1-length :record:`bytes` 
      

   .. method:: proc this(i: int): byteType

      
      Gets a byte from the :record:`bytes`
      
      :arg i: The index
      
      :returns: uint(8)
      

   .. method:: proc toByte(): uint(8)

      
      :returns: The value of a single-byte :record:`bytes` as an integer.
      

   .. method:: proc byte(i: int): byteType

      
      Gets a byte from the :record:`bytes`
      
      :arg i: The index
      
      :returns: The value of the `i` th byte as an integer.
      

   .. itermethod:: iter items(): bytes

      
      Iterates over the :record:`bytes`, yielding ASCII characters.
      
      :yields: 1-length :record:`bytes`
      

   .. itermethod:: iter these(): byteType

      
      Iterates over the :record:`bytes`
      
      :yields: uint(8)
      

   .. itermethod:: iter chpl_bytes(): byteType

      
      Iterates over the :record:`bytes` byte by byte.
      
      :yields: uint(8)
      

   .. method:: proc this(r: range(?)): bytes

      
      Slices the :record:`bytes`. Halts if r is non-empty and not completely
      inside the range ``this.indices`` when compiled with `--checks`.
      `--fast` disables this check.
      
      :arg r: The range of indices the new :record:`bytes` should be made from
      
      :returns: a new :record:`bytes` that is a slice within
                ``this.indices``. If the length of `r` is zero, an empty
                :record:`bytes` is returned.
      

   .. method:: proc isEmpty(): bool

      
      Checks if the :record:`bytes` is empty.
      
      :returns: * `true`  -- when empty
                * `false` -- otherwise
      

   .. method:: proc startsWith(needles: bytes ...): bool

      
      Checks if the :record:`bytes` starts with any of the given arguments.
      
      :arg needles: :record:`bytes` (s) to match against.
      
      :returns: * `true`--when the :record:`bytes` begins with one or more of the `needles`
                * `false`--otherwise
      

   .. method:: proc endsWith(needles: bytes ...): bool

      
      Checks if the :record:`bytes` ends with any of the given arguments.
      
      :arg needles: :record:`bytes` (s) to match against.
      
      :returns: * `true`--when the :record:`bytes` ends with one or more of the `needles`
                * `false`--otherwise
      

   .. method:: proc find(needle: bytes, region: range(?) = 1: idxType..): idxType

      
      Finds the argument in the :record:`bytes`
      
      :arg needle: :record:`bytes` to search for
      
      :arg region: an optional range defining the indices to search
                   within, default is the whole. Halts if the range is not
                   within ``this.indices``
      
      :returns: the index of the first occurrence from the left of `needle`
                within the :record:`bytes`, or 0 if the `needle` is not in the
                :record:`bytes`.
      

   .. method:: proc rfind(needle: bytes, region: range(?) = 1: idxType..): idxType

      
      Finds the argument in the :record:`bytes`
      
      :arg needle: The :record:`bytes` to search for
      
      :arg region: an optional range defining the indices to search within,
                   default is the whole. Halts if the range is not
                   within ``this.indices``
      
      :returns: the index of the first occurrence from the right of `needle`
                within the :record:`bytes`, or 0 if the `needle` is not in the
                :record:`bytes`.
      

   .. method:: proc count(needle: bytes, region: range(?) = this.indices): int

      
      Counts the number of occurrences of the argument in the :record:`bytes`
      
      :arg needle: The :record:`bytes` to search for
      
      :arg region: an optional range defining the substring to search within,
                   default is the whole. Halts if the range is not
                   within ``this.indices``
      
      :returns: the number of times `needle` occurs in the :record:`bytes`
      

   .. method:: proc replace(needle: bytes, replacement: bytes, count: int = -1): bytes

   .. itermethod:: iter split(sep: bytes, maxsplit: int = -1, ignoreEmpty: bool = false): bytes

      
      Splits the :record:`bytes` on `sep` yielding the bytes between each
      occurrence, up to `maxsplit` times.
      
      :arg sep: The delimiter used to break the :record:`bytes` into chunks.
      
      :arg maxsplit: The number of times to split the :record:`bytes`, negative values indicate no limit.
      
      :arg ignoreEmpty: * `true`-- Empty :record:`bytes` will not be yielded,
                        * `false`-- Empty :record:`bytes` will be yielded
      
      :yields: :record:`bytes` 
      

   .. itermethod:: iter split(maxsplit: int = -1): bytes

      
      Works as above, but uses runs of whitespace as the delimiter.
      
      :arg maxsplit: The maximum number of times to split the :record:`bytes`,
                     negative values indicate no limit.
      
      :yields: :record:`bytes` 
      

   .. method:: proc join(const ref S: bytes ...): bytes

      
      Returns a new :record:`bytes`, which is the concatenation of all of
      the :record:`bytes` passed in with the contents of the method
      receiver inserted between them.
      
      .. code-block:: chapel
      
          var x = b"|".join(b"a",b"10",b"d");
          writeln(x); // prints: "a|10|d"
      
      :arg S: :record:`bytes` values to be joined
      
      :returns: A :record:`bytes`
      

   .. method:: proc join(const ref x): bytes

      
      Returns a new :record:`bytes`, which is the concatenation of all of
      the :record:`bytes` passed in with the contents of the method
      receiver inserted between them.
      
      .. code-block:: chapel
      
          var tup = (b"a",b"10",b"d");
          var x = b"|".join(tup);
          writeln(x); // prints: "a|10|d"
      
      :arg S: :record:`bytes` values to be joined
      :type S: tuple or array of :record:`bytes`
      
      :returns: A :record:`bytes`
      

   .. method:: proc strip(chars = b" \t\r\n", leading = true, trailing = true): bytes

      
      Strips given set of leading and/or trailing characters.
      
      :arg chars: Characters to remove.  Defaults to `b" \\t\\r\\n"`.
      
      :arg leading: Indicates if leading occurrences should be removed.
                    Defaults to `true`.
      
      :arg trailing: Indicates if trailing occurrences should be removed.
                     Defaults to `true`.
      
      :returns: A new :record:`bytes` with `leading` and/or `trailing`
                occurrences of characters in `chars` removed as appropriate.
      

   .. method:: proc partition(sep: bytes): 3*(bytes)

      
      Splits the :record:`bytes` on a given separator
      
      :arg sep: The separator
      
      :returns: a `3*bytes` consisting of the section before `sep`,
                `sep`, and the section after `sep`. If `sep` is not found, the
                tuple will contain the whole :record:`bytes`, and then two empty
                :record:`bytes`.
      

   .. method:: proc decode(policy = decodePolicy.strict): string throws

   .. method:: proc isUpper(): bool

      
      Checks if all the characters in the :record:`bytes` are uppercase (A-Z) in
      ASCII.  Ignores uncased (not a letter) and extended ASCII characters
      (decimal value larger than 127)
      
       :returns: * `true`--there is at least one uppercase and no lowercase characters
                 * `false`--otherwise
      

   .. method:: proc isLower(): bool

      
      Checks if all the characters in the :record:`bytes` are lowercase (a-z) in
      ASCII.  Ignores uncased (not a letter) and extended ASCII characters
      (decimal value larger than 127)
      
       :returns: * `true`--there is at least one lowercase and no uppercase characters
                 * `false`--otherwise
      

   .. method:: proc isSpace(): bool

      
      Checks if all the characters in the :record:`bytes` are whitespace (' ',
      '\\t', '\\n', '\\v', '\\f', '\\r') in ASCII.
      
       :returns: * `true`  -- when all the characters are whitespace.
                 * `false` -- otherwise
      

   .. method:: proc isAlpha(): bool

      
      Checks if all the characters in the :record:`bytes` are alphabetic (a-zA-Z)
      in ASCII.
      
       :returns: * `true`  -- when the characters are alphabetic.
                 * `false` -- otherwise
      

   .. method:: proc isDigit(): bool

      
      Checks if all the characters in the :record:`bytes` are digits (0-9) in
      ASCII.
      
       :returns: * `true`  -- when the characters are digits.
                 * `false` -- otherwise
      

   .. method:: proc isAlnum(): bool

      
      Checks if all the characters in the :record:`bytes` are alphanumeric
      (a-zA-Z0-9) in ASCII.
      
       :returns: * `true`  -- when the characters are alphanumeric.
                 * `false` -- otherwise
      

   .. method:: proc isPrintable(): bool

      
      Checks if all the characters in the :record:`bytes` are printable in ASCII.
      
       :returns: * `true`  -- when the characters are printable.
                 * `false` -- otherwise
      

   .. method:: proc isTitle(): bool

      
      Checks if all uppercase characters are preceded by uncased characters,
      and if all lowercase characters are preceded by cased characters in ASCII.
      
      :returns: * `true`  -- when the condition described above is met.
                * `false` -- otherwise
      

   .. method:: proc toLower(): bytes

      
      Creates a new :record:`bytes` with all applicable characters converted to
      lowercase.
      
      :returns: A new :record:`bytes` with all uppercase characters (A-Z)
                replaced with their lowercase counterpart in ASCII. Other
                characters remain untouched.
      

   .. method:: proc toUpper(): bytes

      
      Creates a new :record:`bytes` with all applicable characters converted to
      uppercase.
      
      :returns: A new :record:`bytes` with all lowercase characters (a-z)
                replaced with their uppercase counterpart in ASCII. Other
                characters remain untouched.
      

   .. method:: proc toTitle(): bytes

      
      Creates a new :record:`bytes` with all applicable characters converted to
      title capitalization.
      
      :returns: A new :record:`bytes` with all cased characters(a-zA-Z)
                following an uncased character converted to uppercase, and all
                cased characters following another cased character converted to
                lowercase.
      

.. function:: proc +=(ref lhs: bytes, const ref rhs: bytes): void

   
   Appends the :record:`bytes` `rhs` to the :record:`bytes` `lhs`.
   

.. function:: proc =(ref lhs: bytes, rhs: bytes)

   
   Copies the :record:`bytes` `rhs` into the :record:`bytes` `lhs`.
   

.. function:: proc =(ref lhs: bytes, rhs_c: c_string)

   
   Copies the c_string `rhs_c` into the bytes `lhs`.
   
   Halts if `lhs` is a remote bytes.
   

.. function:: proc +(s0: bytes, s1: bytes)

   
   :returns: A new :record:`bytes` which is the result of concatenating `s0`
             and `s1`
   

.. function:: proc *(s: bytes, n: integral)

   
   :returns: A new :record:`bytes` which is the result of repeating `s` `n`
             times.  If `n` is less than or equal to 0, an empty bytes is
             returned.
   

.. function:: proc comparisonDeprWarn()

