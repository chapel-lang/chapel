/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
The following document shows functions and methods used to manipulate and
process Chapel bytes variables. :mod:`bytes <Bytes>` is similar to a string but
allows arbitrary data to be stored in it. Methods on bytes that interpret the
data as characters assume that the bytes are ASCII characters.

Creating :mod:`bytes <Bytes>`
-----------------------------

- A :mod:`bytes <Bytes>` can be created using the literals similar to strings:

.. code-block:: chapel

   var b = b"my bytes";

- If you need to create :mod:`bytes <Bytes>` using a specific buffer (i.e. data
  in another :mod:`bytes <Bytes>`, a `c_string` or a C pointer) you can use the
  factory functions shown below, such as :proc:`createBytesWithNewBuffer`.

:mod:`bytes <Bytes>` and :mod:`string <String>`
-----------------------------------------------

As :mod:`bytes <Bytes>` can store arbitrary data, any :mod:`string <String>` can
be cast to :mod:`bytes <Bytes>`. In that event, the bytes will store UTF-8
encoded character data. However, a :mod:`bytes <Bytes>` can contain non-UTF-8
bytes and needs to be decoded to be converted to string.

.. code-block:: chapel

   var s = "my string";
   var b = s:bytes;  // this is legal

   /*
    The reverse is not. The following is a compiler error:

    var s2 = b:string;
   */

   var s2 = b.decode(); // you need to decode a bytes to convert it to a string

See the documentation for the :proc:`~bytes.decode` method for details.

Similarly, a :mod:`bytes <Bytes>` can be initialized using a string:

.. code-block:: chapel

   var s = "my string";
   var b: bytes = s;

Casts from :mod:`bytes <Bytes>` to a Numeric Type
-------------------------------------------------

This module supports casts from :mod:`bytes <Bytes>` to numeric types. Such
casts will interpret the :mod:`bytes <Bytes>` as ASCII characters and convert it
to the numeric type and throw an error if the :mod:`bytes <Bytes>` does not
match the expected format of a number. For example:

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
 */
module Bytes {
  private use ChapelStandard;
  private use ByteBufferHelpers;
  private use BytesStringCommon;
  private use SysCTypes, CPtr;

  public use BytesCasts;
  public use BytesStringCommon only decodePolicy;  // expose decodePolicy

  pragma "no doc"
  type idxType = int; 

  //
  // createBytes* functions
  //

  /*
    Creates a new :mod:`bytes <Bytes>` which borrows the internal buffer of
    another :mod:`bytes <Bytes>`. If the buffer is freed before the :mod:`bytes
    <Bytes>` returned from this function, accessing it is undefined behavior.

    :arg s: The :mod:`bytes <Bytes>` to borrow the buffer from

    :returns: A new :mod:`bytes <Bytes>`
  */
  inline proc createBytesWithBorrowedBuffer(x: bytes) {
    var ret: bytes;
    initWithBorrowedBuffer(ret, x);
    return ret;
  }

  /*
    Creates a new :mod:`bytes <Bytes>` which borrows the internal buffer of a
    `c_string`. If the buffer is freed before the :mod:`bytes <Bytes>` returned
    from this function, accessing it is undefined behavior.

    :arg s: `c_string` to borrow the buffer from

    :arg length: Length of `s`'s buffer, excluding the terminating
                 null byte.
    :type length: `int`

    :returns: A new :mod:`bytes <Bytes>`
  */
  inline proc createBytesWithBorrowedBuffer(x: c_string, length=x.size) {
    return createBytesWithBorrowedBuffer(x:c_ptr(uint(8)), length=length,
                                                           size=length+1);
  }

  pragma "no doc"
  proc chpl_createBytesWithLiteral(x: c_string, length: int) {
    // NOTE: This is a "wellknown" function used by the compiler to create
    // string literals. Inlining this creates some bloat in the AST, slowing the
    // compilation.
    return createBytesWithBorrowedBuffer(x, length);
  }

  /*
     Creates a new :mod:`bytes <Bytes>` which borrows the memory allocated for a
     `c_ptr`. If the buffer is freed before the :mod:`bytes <Bytes>` returned
     from this function, accessing it is undefined behavior.

     :arg s: Buffer to borrow
     :type x: `c_ptr(uint(8))` or `c_ptr(c_char)`

     :arg length: Length of the buffer `s`, excluding the terminating null byte.

     :arg size: Size of memory allocated for `s` in bytes

     :returns: A new :mod:`bytes <Bytes>`
  */
  inline proc createBytesWithBorrowedBuffer(x: c_ptr(?t), length: int, size: int) {
    if t != byteType && t != c_char {
      compilerError("Cannot create a bytes with a buffer of ", t:string);
    }
    var ret: bytes;
    initWithBorrowedBuffer(ret, x:bufferType, length, size);
    return ret;
  }

  pragma "no doc"
  inline proc createBytesWithOwnedBuffer(s: bytes) {
    // should we allow stealing ownership?
    compilerError("A bytes cannot be passed to createBytesWithOwnedBuffer");
  }

  /*
    Creates a new :mod:`bytes <Bytes>` which takes ownership of the internal
    buffer of a `c_string`.The buffer will be freed when the :mod:`bytes
    <Bytes>` is deinitialized.

    :arg s: The `c_string` to take ownership of the buffer from

    :arg length: Length of `s`'s buffer, excluding the terminating null byte.
    :type length: `int`

    :returns: A new :mod:`bytes <Bytes>`
  */
  inline proc createBytesWithOwnedBuffer(x: c_string, length=x.size) {
    return createBytesWithOwnedBuffer(x: bufferType, length=length,
                                                      size=length+1);
  }

  /*
     Creates a new :mod:`bytes <Bytes>` which takes ownership of the memory
     allocated for a `c_ptr`. The buffer will be freed when the
     :mod:`bytes <Bytes>` is deinitialized.

     :arg s: The buffer to take ownership of
     :type x: `c_ptr(uint(8))` or `c_ptr(c_char)`

     :arg length: Length of the buffer `s`, excluding the terminating null byte.

     :arg size: Size of memory allocated for `s` in bytes

     :returns: A new :mod:`bytes <Bytes>`
  */
  inline proc createBytesWithOwnedBuffer(x: c_ptr(?t), length: int, size: int) {
    if t != byteType && t != c_char {
      compilerError("Cannot create a bytes with a buffer of ", t:string);
    }
    var ret: bytes;
    initWithOwnedBuffer(ret, x, length, size);
    return ret;
  }

  /*
    Creates a new :mod:`bytes <Bytes>` by creating a copy of the buffer of
    another :mod:`bytes <Bytes>`.

    :arg s: The :mod:`bytes <Bytes>` to copy the buffer from

    :returns: A new :mod:`bytes <Bytes>`
  */
  inline proc createBytesWithNewBuffer(x: bytes) {
    var ret: bytes;
    initWithNewBuffer(ret, x);
    return ret;
  }

  /*
    Creates a new :mod:`bytes <Bytes>` by creating a copy of the buffer of a
    `c_string`.

    :arg s: The `c_string` to copy the buffer from

    :arg length: Length of `s`'s buffer, excluding the terminating null byte.
    :type length: `int`

    :returns: A new :mod:`bytes <Bytes>`
  */
  inline proc createBytesWithNewBuffer(x: c_string, length=x.size) {
    return createBytesWithNewBuffer(x: bufferType, length=length,
                                                    size=length+1);
  }

  /*
     Creates a new :mod:`bytes <Bytes>` by creating a copy of a buffer.

     :arg s: The buffer to copy
     :type x: `c_ptr(uint(8))` or `c_ptr(c_char)`

     :arg length: Length of buffer `s`, excluding the terminating null byte.

     :arg size: Size of memory allocated for `s` in bytes

     :returns: A new :mod:`bytes <Bytes>`
  */
  inline proc createBytesWithNewBuffer(x: c_ptr(?t), length: int,
                                       size=length+1) {
    if t != byteType && t != c_char {
      compilerError("Cannot create a bytes with a buffer of ", t:string);
    }
    var ret: bytes;
    initWithNewBuffer(ret, x, length, size);
    return ret;
  }

  pragma "no doc"
  record _bytes {
    var buffLen: int = 0; // length of string in bytes
    var buffSize: int = 0; // size of the buffer we own
    var buff: bufferType = nil;
    var isOwned: bool = true;
    // We use chpl_nodeID as a shortcut to get at here.id without actually constructing
    // a locale object. Used when determining if we should make a remote transfer.
    var locale_id = chpl_nodeID; // : chpl_nodeID_t

    proc init() {

    }

    proc ref deinit() {
      if isOwned && this.buff != nil {
        on __primitive("chpl_on_locale_num",
                       chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
          chpl_here_free(this.buff);
        }
      }
    }

    proc writeThis(f) throws {
      compilerError("not implemented: writeThis");
    }
    proc readThis(f) throws {
      compilerError("not implemented: readThis");
    }

    proc init=(b: bytes) {
      this.complete();
      initWithNewBuffer(this, b);
    }

    proc init=(b: string) {
      this.complete();
      initWithNewBuffer(this, b.buff, length=b.numBytes, size=b.numBytes+1);
    }

    proc init=(b: c_string) {
      this.complete();
      var length = b.size;
      initWithNewBuffer(this, b: bufferType, length=length, size=length+1);
    }

    inline proc byteIndices return 0..<size;

    inline proc param size param
      return __primitive("string_length_bytes", this);

    inline proc param numBytes param
      return __primitive("string_length_bytes", this);

    inline proc param c_str() param : c_string {
      return this:c_string; // folded out in resolution
    }

    // byteIndex overload provides a nicer interface for string/bytes
    // generic programming
    proc item(i: byteIndex): bytes {
      return this.item(i:int);
    }

    // byteIndex overload provides a nicer interface for string/bytes
    // generic programming
    proc this(i: byteIndex): byteType {
      return this.byte(i:int);
    }




    inline proc param toByte() param : uint(8) {
      if this.numBytes != 1 then
        compilerError("bytes.toByte() only accepts single-byte bytes");
      return __primitive("ascii", this);
    }

    inline proc param byte(param i: int) param : uint(8) {
      if i < 0 || i >= this.numBytes then
        compilerError("index " + i: string + " out of bounds for bytes with length " + this.numBytes:string);
      return __primitive("ascii", this, i);
    }

    inline proc join(const ref x: [] bytes) : bytes {
      return _join(x);
    }

    inline proc join(ir: _iteratorRecord): bytes {
      return doJoinIterator(this, ir);
    }

    // TODO: we don't need this
    inline proc _join(const ref S) : bytes where isTuple(S) || isArray(S) {
      return doJoin(this, S);
    }
  } // end of record bytes

  /*
    :returns: The number of bytes in the :mod:`bytes <Bytes>`.
    */
  inline proc bytes.size return buffLen;

  /*
    :returns: The indices that can be used to index into the bytes
              (i.e., the range ``0..<this.size``)
  */
  proc bytes.indices return 0..<size;

  /*
    :returns: The number of bytes in the :mod:`bytes <Bytes>`.
    */
  inline proc bytes.numBytes return buffLen;

  /*
     Gets a version of the :mod:`bytes <Bytes>` that is on the currently
     executing locale.

     :returns: A shallow copy if the :mod:`bytes <Bytes>` is already on the
               current locale, otherwise a deep copy is performed.
  */
  inline proc bytes.localize() : bytes {
    if _local || this.locale_id == chpl_nodeID {
      return createBytesWithBorrowedBuffer(this);
    } else {
      const x:bytes = this; // assignment makes it local
      return x;
    }
  }


  /*
    Gets a `c_string` from a :mod:`bytes <Bytes>`. The returned `c_string`
    shares the buffer with the :mod:`bytes <Bytes>`.

    :returns: A `c_string`
   */
  inline proc bytes.c_str(): c_string {
    return getCStr(this);
  }

  /*
    Gets an ASCII character from the :mod:`bytes <Bytes>`

    :arg i: The index

    :returns: A 1-length :mod:`bytes <Bytes>` 
   */
  proc bytes.item(i: int): bytes {
    if boundsChecking && (i < 0 || i >= this.buffLen)
      then halt("index ", i, " out of bounds for bytes with length ", this.buffLen);
    var (buf, size) = bufferCopy(buf=this.buff, off=i, len=1,
                                 loc=this.locale_id);
    return createBytesWithOwnedBuffer(buf, length=1, size=size);
  }

  /*
    Gets a byte from the :mod:`bytes <Bytes>`

    :arg i: The index

    :returns: uint(8)
   */
  proc bytes.this(i: int): byteType {
    return this.byte(i);
  }

  /*
    :returns: The value of a single-byte :mod:`bytes <Bytes>` as an integer.
  */
  proc bytes.toByte(): uint(8) {
    if this.buffLen != 1 {
      halt("bytes.toByte() only accepts single-byte bytes");
    }
    return bufferGetByte(buf=this.buff, off=0, loc=this.locale_id);
  }

  /*
    Gets a byte from the :mod:`bytes <Bytes>`

    :arg i: The index

    :returns: The value of the `i` th byte as an integer.
  */
  proc bytes.byte(i: int): byteType {
    if boundsChecking && (i < 0 || i >= this.buffLen)
      then halt("index ", i, " out of bounds for bytes with length ", this.buffLen);
    return bufferGetByte(buf=this.buff, off=i, loc=this.locale_id);
  }
  /*
    Iterates over the :mod:`bytes <Bytes>`, yielding ASCII characters.

    :yields: 1-length :mod:`bytes <Bytes>`
   */
  pragma "order independent yielding loops"
  iter bytes.items(): bytes {
    if this.isEmpty() then return;
    for i in this.indices do
      yield this.item[i];
  }

  /*
    Iterates over the :mod:`bytes <Bytes>`

    :yields: uint(8)
   */
  iter bytes.these(): byteType {
    for i in this.bytes() do
      yield i;
  }

  /*
    Iterates over the :mod:`bytes <Bytes>` byte by byte.

    :yields: uint(8)
  */
  pragma "order independent yielding loops"
  iter bytes.chpl_bytes(): byteType {
    for i in this.indices do
      yield this.byte(i);
  }

  /*
    Slices the :mod:`bytes <Bytes>`. Halts if r is non-empty and not completely
    inside the range ``this.indices`` when compiled with `--checks`.
    `--fast` disables this check.

    :arg r: The range of indices the new :mod:`bytes <Bytes>` should be made
            from

    :returns: a new :mod:`bytes <Bytes>` that is a slice within
              ``this.indices``. If the length of `r` is zero, an empty
              :mod:`bytes <Bytes>` is returned.
   */
  inline proc bytes.this(r: range(?)) : bytes {
    return getSlice(this, r);
  }
  
  /*
    Checks if the :mod:`bytes <Bytes>` is empty.

    :returns: * `true`  -- when empty
              * `false` -- otherwise
   */
  inline proc bytes.isEmpty() : bool {
    return this.buffLen == 0;
  }

  /*
    Checks if the :mod:`bytes <Bytes>` starts with any of the given arguments.

    :arg needles: :mod:`bytes <Bytes>` (s) to match against.

    :returns: * `true`--when the :mod:`bytes <Bytes>` begins with one or more of
                the `needles`
              * `false`--otherwise
   */
  inline proc bytes.startsWith(needles: bytes ...) : bool {
    return startsEndsWith(this, needles, fromLeft=true);
  }

  /*
    Checks if the :mod:`bytes <Bytes>` ends with any of the given arguments.

    :arg needles: :mod:`bytes <Bytes>` (s) to match against.

    :returns: * `true`--when the :mod:`bytes <Bytes>` ends with one or more of
                the `needles`
              * `false`--otherwise
   */
  inline proc bytes.endsWith(needles: bytes ...) : bool {
    return startsEndsWith(this, needles, fromLeft=false);
  }

  /*
    Finds the argument in the :mod:`bytes <Bytes>`

    :arg needle: :mod:`bytes <Bytes>` to search for

    :arg region: an optional range defining the indices to search
                 within, default is the whole. Halts if the range is not
                 within ``this.indices``

    :returns: the index of the first occurrence from the left of `needle`
              within the :mod:`bytes <Bytes>`, or -1 if the `needle` is not in the
              :mod:`bytes <Bytes>`.
   */
  inline proc bytes.find(needle: bytes, region: range(?) = this.indices) : idxType {
    return doSearchNoEnc(this, needle, region, count=false): idxType;
  }

  /*
    Finds the argument in the :mod:`bytes <Bytes>`

    :arg needle: The :mod:`bytes <Bytes>` to search for

    :arg region: an optional range defining the indices to search within,
                 default is the whole. Halts if the range is not
                 within ``this.indices``

    :returns: the index of the first occurrence from the right of `needle`
              within the :mod:`bytes <Bytes>`, or -1 if the `needle` is not in the
              :mod:`bytes <Bytes>`.
   */
  inline proc bytes.rfind(needle: bytes, region: range(?) = this.indices) : idxType {
    return doSearchNoEnc(this, needle, region, count=false,
                         fromLeft=false): idxType;
  }

  /*
    Counts the number of occurrences of the argument in the :mod:`bytes <Bytes>`

    :arg needle: The :mod:`bytes <Bytes>` to search for

    :arg region: an optional range defining the substring to search within,
                 default is the whole. Halts if the range is not
                 within ``this.indices``

    :returns: the number of times `needle` occurs in the :mod:`bytes <Bytes>`
   */
  inline proc bytes.count(needle: bytes, region: range(?) = this.indices) : int {
    return doSearchNoEnc(this, needle, region, count=true);
  }

  /*
    Replaces occurrences of a :mod:`bytes <Bytes>` with another.

    :arg needle: The :mod:`bytes <Bytes>` to search for

    :arg replacement: The :mod:`bytes <Bytes>` to replace `needle` with

    :arg count: an optional argument specifying the number of replacements to
                make, values less than zero will replace all occurrences

    :returns: a copy of the :mod:`bytes <Bytes>` where `replacement` replaces
              `needle` up to `count` times
   */
  inline proc bytes.replace(needle: bytes, replacement: bytes, count: int = -1) : bytes {
    return doReplace(this, needle, replacement, count);
  }

  /*
    Splits the :mod:`bytes <Bytes>` on `sep` yielding the bytes between each
    occurrence, up to `maxsplit` times.

    :arg sep: The delimiter used to break the :mod:`bytes <Bytes>` into chunks.

    :arg maxsplit: The number of times to split the :mod:`bytes <Bytes>`,
                   negative values indicate no limit.

    :arg ignoreEmpty: * `true`-- Empty :mod:`bytes <Bytes>` will not be yielded,
                      * `false`-- Empty :mod:`bytes <Bytes>` will be yielded

    :yields: :mod:`bytes <Bytes>` 
   */
  iter bytes.split(sep: bytes, maxsplit: int = -1,
             ignoreEmpty: bool = false): bytes {
    for s in doSplit(this, sep, maxsplit, ignoreEmpty) do yield s;
  }

  /*
    Works as above, but uses runs of whitespace as the delimiter.

    :arg maxsplit: The maximum number of times to split the :mod:`bytes <Bytes>`,
                   negative values indicate no limit.

    :yields: :mod:`bytes <Bytes>` 
   */
  pragma "not order independent yielding loops"
  iter bytes.split(maxsplit: int = -1) : bytes {
    for s in doSplitWSNoEnc(this, maxsplit) do yield s;
  }

    /*
      Returns a new :mod:`bytes <Bytes>`, which is the concatenation of all of
      the :mod:`bytes <Bytes>` passed in with the contents of the method
      receiver inserted between them.

      .. code-block:: chapel

          var x = b"|".join(b"a",b"10",b"d");
          writeln(x); // prints: "a|10|d"

      :arg S: :mod:`bytes <Bytes>` values to be joined

      :returns: A :mod:`bytes <Bytes>`
    */
    inline proc bytes.join(const ref S: bytes ...) : bytes {
      return _join(S);
    }

    /*
      Returns a new :mod:`bytes <Bytes>`, which is the concatenation of all of
      the :mod:`bytes <Bytes>` passed in with the contents of the method
      receiver inserted between them.

      .. code-block:: chapel

          var tup = (b"a",b"10",b"d");
          var x = b"|".join(tup);
          writeln(x); // prints: "a|10|d"

      :arg S: :mod:`bytes <Bytes>` values to be joined
      :type S: tuple or array of :mod:`bytes <Bytes>`

      :returns: A :mod:`bytes <Bytes>`
    */
    inline proc bytes.join(const ref x) : bytes where isTuple(x) {
      if !isHomogeneousTuple(x) || !isBytes(x[1]) then
        compilerError("join() on tuples only handles homogeneous tuples of bytes");
      return _join(x);
    }

    /*
      Strips given set of leading and/or trailing characters.

      :arg chars: Characters to remove.  Defaults to `b" \\t\\r\\n"`.

      :arg leading: Indicates if leading occurrences should be removed.
                    Defaults to `true`.

      :arg trailing: Indicates if trailing occurrences should be removed.
                     Defaults to `true`.

      :returns: A new :mod:`bytes <Bytes>` with `leading` and/or `trailing`
                occurrences of characters in `chars` removed as appropriate.
    */
    proc bytes.strip(chars = b" \t\r\n", leading=true, trailing=true) : bytes {
      return doStripNoEnc(this, chars, leading, trailing);
    }

    /*
      Splits the :mod:`bytes <Bytes>` on a given separator

      :arg sep: The separator

      :returns: a `3*bytes` consisting of the section before `sep`,
                `sep`, and the section after `sep`. If `sep` is not found, the
                tuple will contain the whole :mod:`bytes <Bytes>`, and then two
                empty :mod:`bytes <Bytes>`.
    */
    inline proc const bytes.partition(sep: bytes) : 3*bytes {
      return doPartition(this, sep);
    }

    /* Remove indentation from each line of bytes.

       This can be useful when applied to multi-line bytes that are indented
       in the source code, but should not be indented in the output.

       When ``columns == 0``, determine the level of indentation to remove from
       all lines by finding the common leading whitespace across all non-empty
       lines. Empty lines are lines containing only whitespace. Tabs and spaces
       are the only whitespaces that are considered, but are not treated as
       the same characters when determining common whitespace.

       When ``columns > 0``, remove ``columns`` leading whitespace characters
       from each line. Tabs are not considered whitespace when ``columns > 0``,
       so only leading spaces are removed.

       :arg columns: The number of columns of indentation to remove. Infer
                     common leading whitespace if ``columns == 0``.

       :arg ignoreFirst: When ``true``, ignore first line when determining the
                         common leading whitespace, and make no changes to the
                         first line.

       :returns: A new `bytes` with indentation removed.

       .. warning::

          ``bytes.dedent`` is not considered stable and is subject to change in
          future Chapel releases.
    */
    proc bytes.dedent(columns=0, ignoreFirst=true): bytes {
      if chpl_warnUnstable then
        compilerWarning("bytes.dedent is subject to change in the future.");
      return doDedent(this, columns, ignoreFirst);
    }

    /*
      Returns a UTF-8 string from the given :mod:`bytes <Bytes>`. If the data is
      malformed for UTF-8, `policy` argument determines the action.
      
      :arg policy: - `decodePolicy.strict` raises an error
                   - `decodePolicy.replace` replaces the malformed character
                     with UTF-8 replacement character
                   - `decodePolicy.drop` drops the data silently
                   - `decodePolicy.escape` escapes each illegal byte with
                     private use codepoints
      
      :throws: `DecodeError` if `decodePolicy.strict` is passed to the `policy`
               argument and the :mod:`bytes <Bytes>` contains non-UTF-8 characters.

      :returns: A UTF-8 string.
    */
    proc bytes.decode(policy=decodePolicy.strict): string throws {
      // NOTE: In the future this method could support more encodings.
      var localThis: bytes = this.localize();
      return decodeByteBuffer(localThis.buff, localThis.buffLen, policy);
    }

    /*
     Checks if all the characters in the :mod:`bytes <Bytes>` are uppercase
     (A-Z) in ASCII.  Ignores uncased (not a letter) and extended ASCII
     characters (decimal value larger than 127)

      :returns: * `true`--there is at least one uppercase and no lowercase characters
                * `false`--otherwise
     */
    proc bytes.isUpper() : bool {
      if this.isEmpty() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for b in this.bytes() {
          if !(byte_isUpper(b)) {
            result = false;
            break;
          }
        }
      }
      return result;
    }

    /*
     Checks if all the characters in the :mod:`bytes <Bytes>` are lowercase
     (a-z) in ASCII.  Ignores uncased (not a letter) and extended ASCII
     characters (decimal value larger than 127)

      :returns: * `true`--there is at least one lowercase and no uppercase characters
                * `false`--otherwise
     */
    proc bytes.isLower() : bool {
      if this.isEmpty() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for b in this.bytes() {
          if !(byte_isLower(b)) {
            result = false;
            break;
          }
        }
      }
      return result;

    }

    /*
     Checks if all the characters in the :mod:`bytes <Bytes>` are whitespace
     (' ', '\\t', '\\n', '\\v', '\\f', '\\r') in ASCII.

      :returns: * `true`  -- when all the characters are whitespace.
                * `false` -- otherwise
     */
    proc bytes.isSpace() : bool {
      if this.isEmpty() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for b in this.bytes() {
          if !(byte_isWhitespace(b)) {
            result = false;
            break;
          }
        }
      }
      return result;
    }

    /*
     Checks if all the characters in the :mod:`bytes <Bytes>` are alphabetic
     (a-zA-Z) in ASCII.

      :returns: * `true`  -- when the characters are alphabetic.
                * `false` -- otherwise
     */
    proc bytes.isAlpha() : bool {
      if this.isEmpty() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for b in this.bytes() {
          if !byte_isAlpha(b) {
            result = false;
            break;
          }
        }
      }
      return result;
    }

    /*
     Checks if all the characters in the :mod:`bytes <Bytes>` are digits (0-9)
     in ASCII.

      :returns: * `true`  -- when the characters are digits.
                * `false` -- otherwise
     */
    proc bytes.isDigit() : bool {
      if this.isEmpty() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for b in this.bytes() {
          if !byte_isDigit(b) {
            result = false;
            break;
          }
        }
      }
      return result;
    }

    /*
     Checks if all the characters in the :mod:`bytes <Bytes>` are alphanumeric
     (a-zA-Z0-9) in ASCII.

      :returns: * `true`  -- when the characters are alphanumeric.
                * `false` -- otherwise
     */
    proc bytes.isAlnum() : bool {
      if this.isEmpty() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for b in this.bytes() {
          if !byte_isAlnum(b) {
            result = false;
            break;
          }
        }
      }
      return result;

    }

    /*
     Checks if all the characters in the :mod:`bytes <Bytes>` are printable in
     ASCII.

      :returns: * `true`  -- when the characters are printable.
                * `false` -- otherwise
     */
    proc bytes.isPrintable() : bool {
      if this.isEmpty() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for b in this.bytes() {
          if !byte_isPrintable(b) {
            result = false;
            break;
          }
        }
      }
      return result;

    }

    /*
      Checks if all uppercase characters are preceded by uncased characters,
      and if all lowercase characters are preceded by cased characters in ASCII.

      :returns: * `true`  -- when the condition described above is met.
                * `false` -- otherwise
     */
    proc bytes.isTitle() : bool {
      if this.isEmpty() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        param UN = 0, UPPER = 1, LOWER = 2;
        var last = UN;
        for b in this.bytes() {
          if byte_isLower(b) {
            if last == UPPER || last == LOWER {
              last = LOWER;
            } else { // last == UN
              result = false;
              break;
            }
          }
          else if byte_isUpper(b) {
            if last == UN {
              last = UPPER;
            } else { // last == UPPER || last == LOWER
              result = false;
              break;
            }
          } else {
            // Uncased elements
            last = UN;
          }
        }
      }
      return result;
    }

    /*
      Creates a new :mod:`bytes <Bytes>` with all applicable characters
      converted to lowercase.

      :returns: A new :mod:`bytes <Bytes>` with all uppercase characters (A-Z)
                replaced with their lowercase counterpart in ASCII. Other
                characters remain untouched.
    */
    proc bytes.toLower() : bytes {
      var result: bytes = this;
      if result.isEmpty() then return result;
      for (i,b) in zip(0.., result.bytes()) {
        result.buff[i] = byte_toLower(b); //check is done by byte_toLower
      }
      return result;
    }

    /*
      Creates a new :mod:`bytes <Bytes>` with all applicable characters
      converted to uppercase.

      :returns: A new :mod:`bytes <Bytes>` with all lowercase characters (a-z)
                replaced with their uppercase counterpart in ASCII. Other
                characters remain untouched.
    */
    proc bytes.toUpper() : bytes {
      var result: bytes = this;
      if result.isEmpty() then return result;
      for (i,b) in zip(0.., result.bytes()) {
        result.buff[i] = byte_toUpper(b); //check is done by byte_toUpper
      }
      return result;
    }

    /*
      Creates a new :mod:`bytes <Bytes>` with all applicable characters
      converted to title capitalization.

      :returns: A new :mod:`bytes <Bytes>` with all cased characters(a-zA-Z)
                following an uncased character converted to uppercase, and all
                cased characters following another cased character converted to
                lowercase.
     */
    proc bytes.toTitle() : bytes {
      var result: bytes = this;
      if result.isEmpty() then return result;

      param UN = 0, LETTER = 1;
      var last = UN;
      for (i,b) in zip(0.., result.bytes()) {
        if byte_isAlpha(b) {
          if last == UN {
            last = LETTER;
            result.buff[i] = byte_toUpper(b);
          } else { // last == LETTER
            result.buff[i] = byte_toLower(b);
          }
        } else {
          // Uncased elements
          last = UN;
        }
      }
      return result;
    }

  pragma "no doc"
  inline operator :(x: string, type t: bytes) {
    return createBytesWithNewBuffer(x.buff, length=x.numBytes, size=x.numBytes+1);
  }
  pragma "no doc"
  inline operator :(x: c_string, type t: bytes) {
    var length = x.size;
    return createBytesWithNewBuffer(x: bufferType, length=length, size=length+1);
  }


  /*
     Appends the :mod:`bytes <Bytes>` `rhs` to the :mod:`bytes <Bytes>` `lhs`.
  */
  proc +=(ref lhs: bytes, const ref rhs: bytes) : void {
    doAppend(lhs, rhs);
  }

  /*
     Copies the :mod:`bytes <Bytes>` `rhs` into the :mod:`bytes <Bytes>` `lhs`.
  */
  proc =(ref lhs: bytes, rhs: bytes) {
    doAssign(lhs, rhs);
  }

  /*
     Copies the c_string `rhs_c` into the bytes `lhs`.

     Halts if `lhs` is a remote bytes.
  */
  proc =(ref lhs: bytes, rhs_c: c_string) {
    lhs = createBytesWithNewBuffer(rhs_c);
  }

  //
  // Concatenation
  //
  /*
     :returns: A new :mod:`bytes <Bytes>` which is the result of concatenating
               `s0` and `s1`
  */
  proc +(s0: bytes, s1: bytes) {
    return doConcat(s0, s1);
  }

  pragma "no doc"
  inline proc +(param s0: bytes, param s1: bytes) param
    return __primitive("string_concat", s0, s1);

  /*
     :returns: A new :mod:`bytes <Bytes>` which is the result of repeating `s`
               `n` times.  If `n` is less than or equal to 0, an empty bytes is
               returned.
  */
  proc *(s: bytes, n: integral) {
    return doMultiply(s, n);
  }

  pragma "no doc"
  proc ==(a: bytes, b: bytes) : bool {
    return doEq(a,b);
  }

  pragma "no doc"
  inline proc !=(a: bytes, b: bytes) : bool {
    return !doEq(a,b);
  }

  pragma "no doc"
  inline proc <(a: bytes, b: bytes) : bool {
    return doLessThan(a, b);
  }

  pragma "no doc"
  inline proc >(a: bytes, b: bytes) : bool {
    return doGreaterThan(a, b);
  }

  pragma "no doc"
  inline proc <=(a: bytes, b: bytes) : bool {
    return doLessThanOrEq(a, b);
  }
  pragma "no doc"
  inline proc >=(a: bytes, b: bytes) : bool {
    return doGreaterThanOrEq(a, b);
  }

  pragma "no doc"
  inline proc ==(param s0: bytes, param s1: bytes) param  {
    return __primitive("string_compare", s0, s1) == 0;
  }

  pragma "no doc"
  inline proc !=(param s0: bytes, param s1: bytes) param {
    return __primitive("string_compare", s0, s1) != 0;
  }

  pragma "no doc"
  inline proc <=(param a: bytes, param b: bytes) param {
    return (__primitive("string_compare", a, b) <= 0);
  }

  pragma "no doc"
  inline proc >=(param a: bytes, param b: bytes) param {
    return (__primitive("string_compare", a, b) >= 0);
  }

  pragma "no doc"
  inline proc <(param a: bytes, param b: bytes) param {
    return (__primitive("string_compare", a, b) < 0);
  }

  pragma "no doc"
  inline proc >(param a: bytes, param b: bytes) param {
    return (__primitive("string_compare", a, b) > 0);
  }

  //
  // hashing support
  //

  pragma "no doc"
  inline proc chpl__defaultHash(x : bytes): uint {
    return getHash(x);
  }

} // end of module Bytes
