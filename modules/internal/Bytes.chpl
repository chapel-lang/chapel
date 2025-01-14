/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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
.. type:: bytes

Supports the following methods:
*/
module Bytes {
  private use ChapelStandard;
  private use ByteBufferHelpers;
  private use BytesStringCommon;
  private use CTypes;
  private use ChplConfig only compiledForSingleLocale;

  public use BytesCasts;
  public use BytesStringCommon only decodePolicy;  // expose decodePolicy

  //
  // createBytes* functions
  //

  /*
    Creates a new :type:`bytes` which borrows the internal buffer of
    another :type:`bytes`. If the buffer is freed before the :type:`bytes`
    returned from this function, accessing it is undefined behavior.

    :arg x: The :type:`bytes` to borrow the buffer from

    :returns: A new :type:`bytes`
  */
  @unstable("'createBorrowingBuffer' is unstable and may change in the future")
  inline proc type bytes.createBorrowingBuffer(x: bytes) : bytes {
    var ret: bytes;
    initWithBorrowedBuffer(ret, x);
    return ret;
  }

  /*
    Creates a new :type:`bytes` which borrows the memory allocated for a
    `c_ptr`. If the buffer is freed before the :type:`bytes` returned
    from this function, accessing it is undefined behavior.

    :arg x: `c_ptr` to borrow as a buffer
    :type x: `c_ptr(int(8))` or `c_ptr(uint(8))`

    :arg length: Length of `x`, excluding the optional terminating null byte.
                 Defaults to the number of bytes in `x` before the terminating
                 null byte.
    :type length: `int`

    :returns: A new :type:`bytes`
  */
  @unstable("'createBorrowingBuffer' is unstable and may change in the future")
  inline proc type bytes.createBorrowingBuffer(x: c_ptr(?t),
                                               length=strLen(x)) : bytes {
    return bytes.createBorrowingBuffer(x:bufferType, length=length,
                                       size=length+1);
  }

  /*
    Creates a new :type:`bytes` which borrows the memory allocated for a
    :class:`~CTypes.c_ptrConst`. If the buffer is freed before the :type:`bytes` returned
    from this function, accessing it is undefined behavior.

    :arg x: `c_ptrConst` to borrow as a buffer
    :type x: `c_ptrConst(uint(8))` or `c_ptrConst(int(8))`

    :arg length: Length of `x`, excluding the optional terminating null byte.
                 Defaults to the number of bytes in `x` before the terminating
                 null byte.
    :type length: `int`

    :returns: A new :type:`bytes`
  */
  @unstable("'createBorrowingBuffer' is unstable and may change in the future")
  inline proc type bytes.createBorrowingBuffer(x: c_ptrConst(?t),
                                               length=strLen(x)) : bytes {
    return bytes.createBorrowingBuffer(x:bufferType, length=length,
                                       size=length+1);
  }

  proc chpl_createBytesWithLiteral(buffer: chpl_c_string,
                                   offset: int,
                                   x: chpl_c_string,
                                   length: int) {
    // copy the string to the combined buffer
    var buf = buffer:c_ptr(void):c_ptr(uint(8));
    buf = buf + offset;
    import OS.POSIX.memcpy;
    memcpy(buf:c_ptr(void), x:c_ptr(void), length.safeCast(c_size_t));
    // add null byte
    buf[length] = 0;

    // NOTE: This is a "wellknown" function used by the compiler to create
    // string literals. Inlining this creates some bloat in the AST, slowing the
    // compilation.
    return bytes.createBorrowingBuffer(buf, length);
  }

  /*
     Creates a new :type:`bytes` which borrows the memory allocated for a
     `c_ptr`. If the buffer is freed before the :type:`bytes` returned
     from this function, accessing it is undefined behavior.

     :arg x: Buffer to borrow
     :type x: `c_ptr(uint(8))` or `c_ptr(int(8))`

     :arg length: Length of the buffer `x`, excluding the optional terminating
                  null byte.

     :arg size: Size of memory allocated for `x` in bytes

     :returns: A new :type:`bytes`
  */
  @unstable("'createBorrowingBuffer' is unstable and may change in the future")
  inline proc type bytes.createBorrowingBuffer(x: c_ptr(?t), length: int,
                                                  size: int) : bytes {
    if t != uint(8) && t != int(8) {
      compilerError("Cannot create a bytes with a buffer of ", t:string);
    }
    var ret: bytes;
    initWithBorrowedBuffer(ret, x:bufferType, length, size);
    return ret;
  }

  /*
    Creates a new :type:`bytes` which takes ownership of the memory
    allocated for a `c_ptr`. The buffer will be freed when the
    :type:`bytes` is deinitialized.

    :arg x: The `c_ptr` to take ownership of
    :type x: `c_ptr(uint(8))` or `c_ptr(int(8))`

    :arg length: Length of buffer `x`, excluding the optional terminating null
                 byte. Defaults to the number of bytes in `x` before the
                 terminating null byte.
    :type length: `int`

    :returns: A new :type:`bytes`
  */
  inline proc type bytes.createAdoptingBuffer(x: c_ptr(?t),
                                              length=strLen(x)) : bytes {
    return bytes.createAdoptingBuffer(x: bufferType, length=length,
                                      size=length+1);
  }

    /*
    Creates a new :type:`bytes` which takes ownership of the memory
    allocated for a :class:`~CTypes.c_ptrConst`. The buffer will be freed when the
    :type:`bytes` is deinitialized.

    :arg x: The `c_ptrConst` to take ownership of
    :type x: `c_ptrConst(uint(8))` or `c_ptrConst(int(8))`

    :arg length: Length of `x`'s buffer, excluding the optional terminating null
                 byte. Defaults to the number of bytes in `x` before the
                 terminating null byte.
    :type length: `int`

    :returns: A new :type:`bytes`
  */
  inline proc type bytes.createAdoptingBuffer(x: c_ptrConst(?t),
                                              length=strLen(x)) : bytes {
    return bytes.createAdoptingBuffer(x: bufferType, length=length,
                                      size=length+1);
  }

  /*
     Creates a new :type:`bytes` which takes ownership of the memory
     allocated for a `c_ptr`. The buffer will be freed when the
     :type:`bytes` is deinitialized.

     :arg x: The buffer to take ownership of
     :type x: `c_ptr(uint(8))` or `c_ptr(int(8))`

     :arg length: Length of the buffer `x`, excluding the optional terminating
                  null byte.

     :arg size: Size of memory allocated for `x` in bytes

     :returns: A new :type:`bytes`
  */
  inline proc type bytes.createAdoptingBuffer(x: c_ptr(?t),
                                              length: int,
                                              size: int) : bytes {
    if t != uint(8) && t != int(8) {
      compilerError("Cannot create a bytes with a buffer of ", t:string);
    }
    var ret: bytes;
    initWithOwnedBuffer(ret, x, length, size);
    return ret;
  }

  /*
    Creates a new :type:`bytes` by creating a copy of a buffer

    :arg x: The :class:`~CTypes.c_ptrConst` to copy
    :type x: `c_ptrConst(uint(8))` or `c_ptrConst(int(8))`

    :arg length: Length of buffer `x`, excluding the optional terminating null
                 byte. Defaults to the number of bytes in `x` before the
                 terminating null byte.
    :type length: `int`

    :returns: A new :type:`bytes`
  */
  inline proc type bytes.createCopyingBuffer(x: c_ptrConst(?t),
                                             length=strLen(x)) : bytes {
    return bytes.createCopyingBuffer(x: bufferType, length=length,
                                     size=length+1);
  }

  /*
     Creates a new :type:`bytes` by creating a copy of a buffer.

     :arg x: The buffer to copy
     :type x: `c_ptr(uint(8))` or `c_ptr(int(8))`

     :arg length: Length of buffer `x`, excluding the optional terminating null
                  byte. Defaults to the number of bytes in `x` before the
                  terminating null byte.

     :arg size: Size of memory allocated for `x` in bytes

     :returns: A new :type:`bytes`
  */
  inline proc type bytes.createCopyingBuffer(x: c_ptr(?t),
                                             length=strLen(x),
                                             size=length+1) : bytes {
    if t != uint(8) && t != int(8) {
      compilerError("Cannot create a bytes with a buffer of ", t:string);
    }
    var ret: bytes;
    initWithNewBuffer(ret, x, length, size);
    return ret;
  }

  @chpldoc.nodoc
  record _bytes : writeSerializable, readDeserializable {
    var buffLen: int = 0; // length of string in bytes
    var buffSize: int = 0; // size of the buffer we own
    var buff: bufferType = nil;
    var isOwned: bool = true;
    // We use chpl_nodeID as a shortcut to get at here.id without actually constructing
    // a locale object. Used when determining if we should make a remote transfer.
    var locale_id = chpl_nodeID; // : chpl_nodeID_t
    // remember to update <=> if you add a field

    proc init() {

    }

    proc ref deinit() {
      if isOwned && this.buff != nil {
        on __primitive("chpl_on_locale_num",
                       chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
          chpl_here_free(this.buff);
        }
      }
    }

    proc chpl__serialize() {
      var data : chpl__inPlaceBuffer;
      if buffLen <= CHPL_SHORT_STRING_SIZE {
        chpl_string_comm_get(chpl__getInPlaceBufferDataForWrite(data),
                             locale_id, buff, buffLen);
      }
      return new __serializeHelper(buffLen, buff, buffSize, locale_id, data,
                                   buffLen);
    }

    proc type chpl__deserialize(data) {
      if data.locale_id != chpl_nodeID {
        if data.buffLen <= CHPL_SHORT_STRING_SIZE {
          return bytes.createCopyingBuffer(
                      chpl__getInPlaceBufferData(data.shortData),
                      data.buffLen,
                      data.size);
        } else {
          var localBuff = bufferCopyRemote(data.locale_id, data.buff,
                                           data.buffLen);
          return bytes.createAdoptingBuffer(localBuff, data.buffLen, data.size);
        }
      } else {
        return bytes.createBorrowingBuffer(data.buff, data.buffLen,
                                             data.size);
      }
    }

    proc serialize(writer, ref serializer) throws {
      compilerError("not implemented: serialize");
    }
    proc ref deserialize(reader, ref deserialize) throws {
      compilerError("not implemented: deserialize");
    }

    proc init=(b: bytes) {
      init this;
      initWithNewBuffer(this, b);
    }

    proc init=(b: string) {
      init this;
      initWithNewBuffer(this, b.buff, length=b.numBytes, size=b.numBytes+1);
    }

    inline proc byteIndices do return 0..<size;

    inline proc param size param do
      return __primitive("string_length_bytes", this);

    inline proc param numBytes param do
      return __primitive("string_length_bytes", this);

    inline proc param this(param i: int) param : int {
      if i < 0 || i > this.size-1 then
        compilerError("index " + i:string + " out of bounds for bytes with length " + this.numBytes:string);
      return __primitive("ascii", this, i);
    }

    inline proc param item(param i: int) param : bytes {
      if i < 0 || i > this.size-1 then
        compilerError("index " + i:string + " out of bounds for bytes with length " + this.numBytes:string);
      return __primitive("bytes item", this, i);
    }

    // byteIndex overload provides a nicer interface for string/bytes
    // generic programming
    proc item(i: byteIndex): bytes {
      return this.item(i:int);
    }

    // byteIndex overload provides a nicer interface for string/bytes
    // generic programming
    proc this(i: byteIndex): uint(8) {
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
      return doJoin(this, x);
    }

    inline proc join(const ref x) where isTuple(x) {
      if !isHomogeneousTuple(x) || !isBytes(x[0]) then
        compilerError("join() on tuples only handles homogeneous tuples of bytes");
      return doJoin(this, x);
    }

    inline proc join(ir: _iteratorRecord): bytes {
      return doJoinIterator(this, ir);
    }

  } // end of record bytes

  /*
    :returns: The number of bytes in the :type:`bytes`.
    */
  inline proc bytes.size : int do return buffLen;

  /*
    :returns: The indices that can be used to index into the bytes
              (i.e., the range ``0..<this.size``)
  */
  proc bytes.indices : range do return 0..<size;

  /*
    :returns: The number of bytes in the :type:`bytes`.
    */
  inline proc bytes.numBytes : int do return buffLen;

  /*
     Gets a version of the :type:`bytes` that is on the currently
     executing locale.

     :returns: A shallow copy if the :type:`bytes` is already on the
               current locale, otherwise a deep copy is performed.
  */
  @unstable("bytes.localize() is unstable and may change in a future release")
  inline proc bytes.localize() : bytes {
    if compiledForSingleLocale() || this.locale_id == chpl_nodeID {
      return bytes.createBorrowingBuffer(this);
    } else {
      const x:bytes = this; // assignment makes it local
      return x;
    }
  }


  /*
    Gets a `c_ptrConst(c_char)` from a :type:`bytes`. The returned
    :class:`~CTypes.c_ptrConst` shares the buffer with the :type:`bytes`.

    .. warning::

      This can only be called safely on a :type:`bytes` whose home is
      the current locale.  This property can be enforced by calling
      :proc:`bytes.localize()` before :proc:`~bytes.c_str()`. If the
      bytes is remote, the program will halt.

    For example:

    .. code-block:: chapel

        var myBytes = b"Hello!";
        on differentLocale {
          printf("%s", myBytes.localize().c_str());
        }

    :returns: A `c_ptrConst(c_char)` that points to the underlying buffer used
              by this :type:`bytes`. The returned `c_ptrConst(c_char)` is only
              valid when used on the same locale as the bytes.
   */
  pragma "last resort"
  @deprecated("'bytes.c_str()' has moved to 'CTypes'. Please 'use CTypes' to access ':proc:`~CTypes.bytes.c_str`'")
  inline proc bytes.c_str(): c_ptrConst(c_char) {
    use CTypes only c_str;
    return this.c_str();
  }

  /*
    Gets an ASCII character from the :type:`bytes`

    :arg i: The index

    :returns: A 1-length :type:`bytes`
   */
  proc bytes.item(i: int): bytes {
    if boundsChecking && (i < 0 || i >= this.buffLen)
      then halt("index ", i, " out of bounds for bytes with length ", this.buffLen);
    var (buf, size) = bufferCopy(buf=this.buff, off=i, len=1,
                                 loc=this.locale_id);
    return bytes.createAdoptingBuffer(buf, length=1, size=size);
  }

  /*
    Gets a byte from the :type:`bytes`

    :arg i: The index

    :returns: uint(8)
   */
  proc bytes.this(i: int): uint(8) {
    return this.byte(i);
  }

  /*
    :returns: The value of a single-byte :type:`bytes` as an integer.
  */
  proc bytes.toByte(): uint(8) {
    if this.buffLen != 1 {
      halt("bytes.toByte() only accepts single-byte bytes");
    }
    return bufferGetByte(buf=this.buff, off=0, loc=this.locale_id);
  }

  /*
    Gets a byte from the :type:`bytes`

    :arg i: The index

    :returns: The value of the `i` th byte as an integer.
  */
  proc bytes.byte(i: int): uint(8) {
    if boundsChecking && (i < 0 || i >= this.buffLen)
      then halt("index ", i, " out of bounds for bytes with length ", this.buffLen);
    return bufferGetByte(buf=this.buff, off=i, loc=this.locale_id);
  }
  /*
    Iterates over the :type:`bytes`, yielding ASCII characters.

    :yields: 1-length :type:`bytes`
   */
  iter bytes.items(): bytes {
    if this.isEmpty() then return;
    foreach i in this.indices do
      yield this.item[i];
  }

  /*
    Iterates over the :type:`bytes`

    :yields: uint(8)
   */
  iter bytes.these(): uint(8) {
    for i in this.bytes() do
      yield i;
  }

  /*
    Iterates over the :type:`bytes` byte by byte.

    :yields: uint(8)
  */
  pragma "chpldoc ignore chpl prefix"
  iter bytes.chpl_bytes(): uint(8) {
    foreach i in this.indices do
      yield this.byte(i);
  }

  /*
    Slices the :type:`bytes`. Halts if r is non-empty and not completely
    inside the range ``this.indices`` when compiled with `--checks`.
    `--fast` disables this check.

    :arg r: The range of indices the new :type:`bytes` should be made
            from

    :returns: a new :type:`bytes` that is a slice within
              ``this.indices``. If the length of `r` is zero, an empty
              :type:`bytes` is returned.
   */
  inline proc bytes.this(r: range(?)) : bytes {
    // getSlice with bytes argument never throws
    return try! getSlice(this, r);
  }

  /*
    Checks if the :type:`bytes` is empty.

    :returns: * `true`  -- when empty
              * `false` -- otherwise
   */
  inline proc bytes.isEmpty() : bool {
    return this.buffLen == 0;
  }

  /*
    Checks if the :type:`bytes` starts with any of the given arguments.

    :arg patterns: :type:`bytes` (s) to match against.

    :returns: * `true`--when the :type:`bytes` begins with one or more of
                the `patterns`
              * `false`--otherwise
   */
  inline proc bytes.startsWith(patterns: bytes ...) : bool {
    return startsEndsWith(this, patterns, fromLeft=true);
  }

  /*
    Checks if the :type:`bytes` ends with any of the given arguments.

    :arg patterns: :type:`bytes` (s) to match against.

    :returns: * `true`--when the :type:`bytes` ends with one or more of
                the `patterns`
              * `false`--otherwise
   */
  inline proc bytes.endsWith(patterns: bytes ...) : bool {
    return startsEndsWith(this, patterns, fromLeft=false);
  }

  /*
    Finds the argument in the :type:`bytes`

    :arg pattern: :type:`bytes` to search for

    :arg indices: an optional range defining the indices to search
                 within, default is the whole. Halts if the range is not
                 within ``this.indices``

    :returns: the index of the first occurrence from the left of `pattern`
              within the :type:`bytes`, or -1 if the `pattern` is not in the
              :type:`bytes`.
   */
  inline proc bytes.find(pattern: bytes,
                         indices: range(?) = this.indices) : int {
    return doSearchNoEnc(this, pattern, indices, count=false);
  }

  /*
    Finds the argument in the :type:`bytes`

    :arg pattern: The :type:`bytes` to search for

    :arg indices: an optional range defining the indices to search within,
                 default is the whole. Halts if the range is not
                 within ``this.indices``

    :returns: the index of the first occurrence from the right of `pattern`
              within the :type:`bytes`, or -1 if the `pattern` is not in the
              :type:`bytes`.
   */
  inline proc bytes.rfind(pattern: bytes,
                          indices: range(?) = this.indices) : int {
    return doSearchNoEnc(this, pattern, indices, count=false,
                         fromLeft=false);
  }

  /*
    Counts the number of occurrences of the argument in the :type:`bytes`

    :arg pattern: The :type:`bytes` to search for

    :arg indices: an optional range defining the substring to search within,
                 default is the whole. Halts if the range is not
                 within ``this.indices``

    :returns: the number of times `pattern` occurs in the :type:`bytes`
   */
  inline proc bytes.count(pattern: bytes,
                          indices: range(?) = this.indices) : int {
    return doSearchNoEnc(this, pattern, indices, count=true);
  }


  /*
    Replaces occurrences of a :type:`bytes` with another.

    :arg pattern: The :type:`bytes` to search for

    :arg replacement: The :type:`bytes` to replace `pattern` with

    :arg count: an optional argument specifying the number of replacements to
                make, values less than zero will replace all occurrences

    :returns: a copy of the :type:`bytes` where `replacement` replaces
              `pattern` up to `count` times
   */
  inline proc bytes.replace(pattern: bytes,
                            replacement: bytes,
                            count: int = -1) : bytes {
    return doReplace(this, pattern, replacement, count);
  }

  /*
    Splits the :type:`bytes` on `sep` yielding the bytes between each
    occurrence, up to `maxsplit` times.

    :arg sep: The delimiter used to break the :type:`bytes` into chunks.

    :arg maxsplit: The number of times to split the :type:`bytes`,
                   negative values indicate no limit.

    :arg ignoreEmpty: * `true`-- Empty :type:`bytes` will not be yielded,
                      * `false`-- Empty :type:`bytes` will be yielded

    :yields: :type:`bytes`
   */
  iter bytes.split(sep: bytes, maxsplit: int = -1,
             ignoreEmpty: bool = false): bytes {
    for s in doSplit(this, sep, maxsplit, ignoreEmpty) do yield s;
  }

  /*
    Works as above, but uses runs of whitespace as the delimiter.

    :arg maxsplit: The maximum number of times to split the :type:`bytes`,
                   negative values indicate no limit.

    :yields: :type:`bytes`
   */
  iter bytes.split(maxsplit: int = -1) : bytes {
    for s in doSplitWSNoEnc(this, maxsplit) do yield s;
  }

  /*
    Returns a new :type:`bytes`, which is the concatenation of all of
    the :type:`bytes` passed in with the contents of the method
    receiver inserted between them.

    .. code-block:: chapel

        var myBytes = b"|".join(b"a",b"10",b"d");
        writeln(myBytes); // prints: "a|10|d"

    :arg x: :type:`bytes` values to be joined

    :returns: A :type:`bytes`
  */
  inline proc bytes.join(const ref x: bytes ...) : bytes {
    return doJoin(this, x);
  }

  /*
    Returns a new :type:`bytes`, which is the concatenation of all of
    the :type:`bytes` passed in with the contents of the method
    receiver inserted between them.

    .. code-block:: chapel

        var tup = (b"a",b"10",b"d");
        var myJoinedTuple = b"|".join(tup);
        writeln(myJoinedTuple); // prints: "a|10|d"

        var myJoinedArray = b"|".join([b"a",b"10",b"d"]);
        writeln(myJoinedArray); // prints: "a|10|d"

    :arg x: An array or tuple of :type:`bytes` values to be joined

    :returns: A :type:`bytes`
  */
  inline proc bytes.join(const ref x) : bytes  {
    // this overload serves as a catch-all for unsupported types.
    // for the implementation of array and tuple overloads, see
    // join() methods in the _bytes record.
    compilerError("bytes.join() accepts any number of bytes, homogeneous "
                    + "tuple of bytes, or array of bytes as an argument");
  }

  /*
    Strips given set of leading and/or trailing characters.

    :arg chars: Characters to remove.  Defaults to `b" \\t\\r\\n"`.

    :arg leading: Indicates if leading occurrences should be removed.
                  Defaults to `true`.

    :arg trailing: Indicates if trailing occurrences should be removed.
                    Defaults to `true`.

    :returns: A new :type:`bytes` with `leading` and/or `trailing`
              occurrences of characters in `chars` removed as appropriate.
  */
  proc bytes.strip(chars = b" \t\r\n", leading=true, trailing=true) : bytes {
    return doStripNoEnc(this, chars, leading, trailing);
  }

  /*
    Splits the :type:`bytes` on a given separator

    :arg sep: The separator

    :returns: a `3*bytes` consisting of the section before `sep`,
              `sep`, and the section after `sep`. If `sep` is not found, the
              tuple will contain the whole :type:`bytes`, and then two
              empty :type:`bytes`.
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

      :returns: A new :type:`bytes` with indentation removed.
  */
  @unstable("bytes.dedent is subject to change in the future.")
  proc bytes.dedent(columns=0, ignoreFirst=true): bytes {
    return doDedent(this, columns, ignoreFirst);
  }

  /*
    Returns a UTF-8 string from the given :type:`bytes`. If the data is
    malformed for UTF-8, `policy` argument determines the action.

    :arg policy: - `decodePolicy.strict` raises an error
                  - `decodePolicy.replace` replaces the malformed character
                    with UTF-8 replacement character
                  - `decodePolicy.drop` drops the data silently
                  - `decodePolicy.escape` escapes each illegal byte with
                    private use codepoints

    :throws: Throws a :class:`~Errors.DecodeError` if `decodePolicy.strict` is
      passed to the `policy` argument and the :type:`bytes` contains non-UTF-8
      characters.

    :returns: A UTF-8 string.
  */
  proc bytes.decode(policy=decodePolicy.strict) : string throws {
    // NOTE: In the future this method could support more encodings.
    var localThis: bytes = this.localize();
    return decodeByteBuffer(localThis.buff, localThis.buffLen, policy);
  }

  /*
    Checks if all the characters in the :type:`bytes` are uppercase
    (A-Z) in ASCII.  Ignores uncased (not a letter) and extended ASCII
    characters (decimal value larger than 127)

    :returns: * `true`--there is at least one uppercase and no lowercase characters
              * `false`--otherwise
    */
  proc bytes.isUpper() : bool {
    if this.isEmpty() then return false;
    var result: bool = true;

    on __primitive("chpl_on_locale_num",
                    chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
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
    Checks if all the characters in the :type:`bytes` are lowercase
    (a-z) in ASCII.  Ignores uncased (not a letter) and extended ASCII
    characters (decimal value larger than 127)

    :returns: * `true`--there is at least one lowercase and no uppercase characters
              * `false`--otherwise
    */
  proc bytes.isLower() : bool {
    if this.isEmpty() then return false;
    var result: bool = true;

    on __primitive("chpl_on_locale_num",
                    chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
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
    Checks if all the characters in the :type:`bytes` are whitespace
    (' ', '\\t', '\\n', '\\v', '\\f', '\\r') in ASCII.

    :returns: * `true`  -- when all the characters are whitespace.
              * `false` -- otherwise
    */
  proc bytes.isSpace() : bool {
    if this.isEmpty() then return false;
    var result: bool = true;

    on __primitive("chpl_on_locale_num",
                    chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
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
    Checks if all the characters in the :type:`bytes` are alphabetic
    (a-zA-Z) in ASCII.

    :returns: * `true`  -- when the characters are alphabetic.
              * `false` -- otherwise
    */
  proc bytes.isAlpha() : bool {
    if this.isEmpty() then return false;
    var result: bool = true;

    on __primitive("chpl_on_locale_num",
                    chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
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
    Checks if all the characters in the :type:`bytes` are digits (0-9)
    in ASCII.

    :returns: * `true`  -- when the characters are digits.
              * `false` -- otherwise
    */
  proc bytes.isDigit() : bool {
    if this.isEmpty() then return false;
    var result: bool = true;

    on __primitive("chpl_on_locale_num",
                    chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
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
    Checks if all the characters in the :type:`bytes` are alphanumeric
    (a-zA-Z0-9) in ASCII.

    :returns: * `true`  -- when the characters are alphanumeric.
              * `false` -- otherwise
    */
  proc bytes.isAlnum() : bool {
    if this.isEmpty() then return false;
    var result: bool = true;

    on __primitive("chpl_on_locale_num",
                    chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
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
    Checks if all the characters in the :type:`bytes` are printable in
    ASCII.

    :returns: * `true`  -- when the characters are printable.
              * `false` -- otherwise
    */
  proc bytes.isPrintable() : bool {
    if this.isEmpty() then return false;
    var result: bool = true;

    on __primitive("chpl_on_locale_num",
                    chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
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
                    chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
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
    Creates a new :type:`bytes` with all applicable characters
    converted to lowercase.

    :returns: A new :type:`bytes` with all uppercase characters (A-Z)
              replaced with their lowercase counterpart in ASCII. Other
              characters remain untouched.
  */
  proc bytes.toLower() : bytes {
    var result: bytes = this;
    if result.isEmpty() then return result;
    for (b,i) in zip(result.bytes(), 0..) {
      result.buff[i] = byte_toLower(b); //check is done by byte_toLower
    }
    return result;
  }

  /*
    Creates a new :type:`bytes` with all applicable characters
    converted to uppercase.

    :returns: A new :type:`bytes` with all lowercase characters (a-z)
              replaced with their uppercase counterpart in ASCII. Other
              characters remain untouched.
  */
  proc bytes.toUpper() : bytes {
    var result: bytes = this;
    if result.isEmpty() then return result;
    for (b,i) in zip(result.bytes(), 0..) {
      result.buff[i] = byte_toUpper(b); //check is done by byte_toUpper
    }
    return result;
  }

  /*
    Creates a new :type:`bytes` with all applicable characters
    converted to title capitalization.

    :returns: A new :type:`bytes` with all cased characters(a-zA-Z)
              following an uncased character converted to uppercase, and all
              cased characters following another cased character converted to
              lowercase.
    */
  proc bytes.toTitle() : bytes {
    var result: bytes = this;
    if result.isEmpty() then return result;

    param UN = 0, LETTER = 1;
    var last = UN;
    for (b,i) in zip(result.bytes(), 0..) {
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

  @chpldoc.nodoc
  inline operator :(x: string, type t: bytes) {
    return bytes.createCopyingBuffer(x.buff, length=x.numBytes, size=x.numBytes+1);
  }


  /*
     Appends the :type:`bytes` `rhs` to the :type:`bytes` `lhs`.
  */
  operator bytes.+=(ref lhs: bytes, const ref rhs: bytes) : void {
    doAppend(lhs, rhs);
  }

  /*
     Appends the one or more byte values passed as arguments to
     the :type:`bytes` `this`.
   */
  @unstable("'bytes.appendByteValues' is unstable and may change in the future")
  proc ref bytes.appendByteValues(x: uint(8) ...) : void {
    var buf: c_array(uint(8), x.size);
    for param i in 0..<x.size {
      buf(i) = x(i);
    }

    doAppendSomeBytes(this, x.size, buf, nCodepoints=0);
  }

  /* Convert a nibble into a character in its hexadecimal representation */
  private proc convertNibble(in nib:uint(8), uppercase: bool): uint(8) {
    nib = nib & 0xf;
    if 0 <= nib && nib <= 9 {
      param zero:uint(8) = b"0"(0); // aka 0x30
      return zero + nib;
    } else if 10 <= nib && nib <= 15 {
      param a:uint(8) = b"a"(0); // aka 0x61
      param A:uint(8) = b"A"(0); // aka 0x41
      return (if uppercase then A else a) + nib - 10;
    }

    return 0;
  }

  /*
    Computes a hexadecimal representation for a ``bytes``
    and returns it as a ``bytes``.
   */
  @unstable("'bytes.toHexadecimal' is unstable and may change in the future")
  proc bytes.toHexadecimal(uppercase: bool = false,
                           type resultType = bytes) : resultType {
    var result: resultType;
    for byte in this {
      const nib1 = convertNibble((byte>>4)&0xf, uppercase);
      const nib2 = convertNibble(byte&0xf, uppercase);
      if resultType == bytes {
        result.appendByteValues(nib1, nib2);
      }
      if resultType == string {
        result.appendCodepointValues(nib1, nib2);
      }
    }
    return result;
  }

  /*
     Copies the :type:`bytes` `rhs` into the :type:`bytes` `lhs`.
  */
  operator bytes.=(ref lhs: bytes, rhs: bytes) : void {
    doAssign(lhs, rhs);
  }

  //
  // Concatenation
  //
  /*
     :returns: A new :type:`bytes` which is the result of concatenating
               `s0` and `s1`
  */
  operator bytes.+(s0: bytes, s1: bytes) : bytes {
    return doConcat(s0, s1);
  }

  @chpldoc.nodoc
  inline operator bytes.+(param s0: bytes, param s1: bytes) param do
    return __primitive("string_concat", s0, s1);

  /*
     :returns: A new :type:`bytes` which is the result of repeating `s`
               `n` times.  If `n` is less than or equal to 0, an empty bytes is
               returned.

     The operation is commutative.
     For example:

     .. code-block:: chapel

        writeln(b"Hello! "*3);
        or
        writeln(3*b"Hello! ");

     Results in::

        Hello! Hello! Hello!
  */
  operator *(s: bytes, n: integral) : bytes {
    return doMultiply(s, n);
  }

  @chpldoc.nodoc
  operator *(n: integral, s: bytes) {
    return doMultiply(s, n);
  }

  @chpldoc.nodoc
  operator bytes.==(a: bytes, b: bytes) : bool {
    return doEq(a,b);
  }

  @chpldoc.nodoc
  inline operator bytes.!=(a: bytes, b: bytes) : bool {
    return !doEq(a,b);
  }

  @chpldoc.nodoc
  inline operator bytes.<(a: bytes, b: bytes) : bool {
    return doLessThan(a, b);
  }

  @chpldoc.nodoc
  inline operator bytes.>(a: bytes, b: bytes) : bool {
    return doGreaterThan(a, b);
  }

  @chpldoc.nodoc
  inline operator bytes.<=(a: bytes, b: bytes) : bool {
    return doLessThanOrEq(a, b);
  }
  @chpldoc.nodoc
  inline operator bytes.>=(a: bytes, b: bytes) : bool {
    return doGreaterThanOrEq(a, b);
  }

  @chpldoc.nodoc
  inline operator bytes.==(param s0: bytes, param s1: bytes) param  {
    return __primitive("string_compare", s0, s1) == 0;
  }

  @chpldoc.nodoc
  inline operator bytes.!=(param s0: bytes, param s1: bytes) param {
    return __primitive("string_compare", s0, s1) != 0;
  }

  @chpldoc.nodoc
  inline operator bytes.<=(param a: bytes, param b: bytes) param {
    return (__primitive("string_compare", a, b) <= 0);
  }

  @chpldoc.nodoc
  inline operator bytes.>=(param a: bytes, param b: bytes) param {
    return (__primitive("string_compare", a, b) >= 0);
  }

  @chpldoc.nodoc
  inline operator bytes.<(param a: bytes, param b: bytes) param {
    return (__primitive("string_compare", a, b) < 0);
  }

  @chpldoc.nodoc
  inline operator bytes.>(param a: bytes, param b: bytes) param {
    return (__primitive("string_compare", a, b) > 0);
  }

  //
  // hashing support
  //

  @chpldoc.nodoc
  inline proc bytes.hash(): uint {
    return getHash(this);
  }
  bytes implements hashable;

  @chpldoc.nodoc
  operator bytes.<=>(ref x: bytes, ref y: bytes) {
    if (x.locale_id != y.locale_id) {
      // TODO: could we just change locale_id?
      var tmp = x;
      x = y;
      y = tmp;
    } else {
      x.buffLen <=> y.buffLen;
      x.buffSize <=> y.buffSize;
      x.buff <=> y.buff;
      x.isOwned <=> y.isOwned;
      x.locale_id <=> y.locale_id;
    }
  }
} // end of module Bytes
