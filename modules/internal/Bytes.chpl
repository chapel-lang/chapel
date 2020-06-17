/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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
  private use SysCTypes;

  public use BytesCasts;
  public use BytesStringCommon only decodePolicy;  // expose decodePolicy

  pragma "no doc"
  type idxType = int; 

  private proc bytesFactoryArgDepr() {
    compilerWarning("createBytesWith* with formal argument `s` is deprecated. ",
                    "Use argument name `x` instead");
  }

  private proc joinArgDepr() {
    compilerWarning("bytes.join with formal argument `S` is deprecated. ",
                    "Use argument name `x` instead");
  }
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

  pragma "last resort"
  pragma "no doc"
  inline proc createBytesWithBorrowedBuffer(s: bytes) {
    bytesFactoryArgDepr();
    return createBytesWithBorrowedBuffer(x=s);
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
  proc createBytesWithBorrowedBuffer(x: c_string, length=x.size) {
    //NOTE: This function is heavily used by the compiler to create bytes
    //literals. So, inlining this causes some bloat in the AST that increases
    //the compilation time slightly. Therefore, currently we are keeping this
    //one non-inlined.
    return createBytesWithBorrowedBuffer(x:c_ptr(uint(8)), length=length,
                                                           size=length+1);
  }

  pragma "last resort"
  pragma "no doc"
  proc createBytesWithBorrowedBuffer(s: c_string, length=s.size) {
    bytesFactoryArgDepr();
    return createBytesWithBorrowedBuffer(x=s, length);
  }

  /*
     Creates a new :mod:`bytes <Bytes>` which borrows the memory allocated for a
     `c_ptr(uint(8))`. If the buffer is freed before the :mod:`bytes <Bytes>`
     returned from this function, accessing it is undefined behavior.

     :arg s: Buffer to borrow
     :type s: `bufferType` (i.e. `c_ptr(uint(8))`)

     :arg length: Length of the buffer `s`, excluding the terminating null byte.

     :arg size: Size of memory allocated for `s` in bytes

     :returns: A new :mod:`bytes <Bytes>`
  */
  inline proc createBytesWithBorrowedBuffer(x: bufferType, length: int, size: int) {
    var ret: bytes;
    initWithBorrowedBuffer(ret, x, length,size);
    return ret;
  }

  pragma "last resort"
  pragma "no doc"
  inline proc createBytesWithBorrowedBuffer(s: bufferType, length: int, size: int) {
    bytesFactoryArgDepr();
    return createBytesWithBorrowedBuffer(x=s, length, size);
  }

  pragma "no doc"
  inline proc createBytesWithOwnedBuffer(s: bytes) {
    // should we allow stealing ownership?
    compilerError("A bytes cannot be passed to createBytesWithOwnedBuffer");
  }

  pragma "last resort"
  pragma "no doc"
  inline proc createBytesWithOwnedBuffer(s: bytes) {
    bytesFactoryArgDepr();
    return createBytesWithOwnedBuffer(x=s);
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

  pragma "last resort"
  pragma "no doc"
  inline proc createBytesWithOwnedBuffer(s: c_string, length=s.size) {
    bytesFactoryArgDepr();
    return createBytesWithOwnedBuffer(x=s, length);
  }

  /*
     Creates a new :mod:`bytes <Bytes>` which takes ownership of the memory
     allocated for a `c_ptr(uint(8))`. The buffer will be freed when the
     :mod:`bytes <Bytes>` is deinitialized.

     :arg s: The buffer to take ownership of
     :type s: `bufferType` (i.e. `c_ptr(uint(8))`)

     :arg length: Length of the buffer `s`, excluding the terminating null byte.

     :arg size: Size of memory allocated for `s` in bytes

     :returns: A new :mod:`bytes <Bytes>`
  */
  inline proc createBytesWithOwnedBuffer(x: bufferType, length: int, size: int) {
    var ret: bytes;
    initWithOwnedBuffer(ret, x, length, size);
    return ret;
  }

  pragma "last resort"
  pragma "no doc"
  inline proc createBytesWithOwnedBuffer(s: bufferType, length: int, size: int) {
    bytesFactoryArgDepr();
    return createBytesWithOwnedBuffer(x=s, length, size);
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

  pragma "last resort"
  pragma "no doc"
  inline proc createBytesWithNewBuffer(s: bytes) {
    bytesFactoryArgDepr();
    return createBytesWithNewBuffer(x=s);
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

  pragma "last resort"
  pragma "no doc"
  inline proc createBytesWithNewBuffer(s: c_string, length=s.size) {
    bytesFactoryArgDepr();
    return createBytesWithNewBuffer(x=s, length);
  }

  /*
     Creates a new :mod:`bytes <Bytes>` by creating a copy of a buffer.

     :arg s: The buffer to copy
     :type s: `bufferType` (i.e. `c_ptr(uint(8))`)

     :arg length: Length of buffer `s`, excluding the terminating null byte.

     :arg size: Size of memory allocated for `s` in bytes

     :returns: A new :mod:`bytes <Bytes>`
  */
  inline proc createBytesWithNewBuffer(x: bufferType, length: int,
                                       size=length+1) {
    var ret: bytes;
    initWithNewBuffer(ret, x, length, size);
    return ret;
  }

  pragma "last resort"
  pragma "no doc"
  inline proc createBytesWithNewBuffer(s: bufferType, length: int,
                                       size=length+1) {
    bytesFactoryArgDepr();
    return createBytesWithNewBuffer(x=s, length, size);
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

    inline proc byteIndices return 0..<size;

    inline proc param length param {
      compilerWarning("'bytes.length' is deprecated - " +
                      "please use 'bytes.size' instead");
      return size;
    }

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

    // Helper function that uses a param bool to toggle between count and find
    //TODO: this could be a much better string search
    //      (Boyer-Moore-Horspool|any thing other than brute force)
    //
    inline proc _search_helper(needle: bytes, region: range(?),
                               param count: bool, param fromLeft: bool = true) {
      // needle.buffLen is <= than this.buffLen, so go to the home locale
      var ret: int = -1;
      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        // any value >= 0 means we have a solution
        // used because we cant break out of an on-clause early
        var localRet: int = -2;
        const nLen = needle.buffLen;
        const (view, _) = getView(this, region);
        const thisLen = view.size;

        // Edge cases
        if count {
          if nLen == 0 { // Empty needle
            localRet = view.size;
          }
        } else { // find
          if nLen == 0 { // Empty needle
            if fromLeft {
              localRet = -1;
            } else {
              localRet = if thisLen == 0
                then -1
                else thisLen;
            }
          }
        }

        if nLen > thisLen {
          localRet = -1;
        }

        if localRet == -2 {
          localRet = -1;
          const localNeedle = needle.localize();
          const needleLen = localNeedle.buffLen;

          // i *is not* an index into anything, it is the order of the element
          // of view we are searching from.
          const numPossible = thisLen - nLen + 1;
          const searchSpace = if fromLeft
              then 0..#(numPossible)
              else 0..#(numPossible) by -1;
          for i in searchSpace {
            const bufIdx = view.orderToIndex(i);
            const found = bufferEqualsLocal(buf1=this.buff, off1=bufIdx,
                                            buf2=localNeedle.buff, off2=0,
                                            len=needleLen);
            if found {
              if count {
                localRet += 1;
              } else { // find
                localRet = view.orderToIndex(i);
              }
            }
            if !count && localRet != -1 then break;
          }
        }
        if count then localRet += 1;
        ret = localRet;
      }
      return ret;
    }

    pragma "last resort"
    inline proc join(const ref S) : bytes where isTuple(S) {
      joinArgDepr();
      return join(x=S);
    }

    inline proc join(const ref x: [] bytes) : bytes {
      return _join(x);
    }

    pragma "last resort"
    inline proc join(const ref S: [] bytes) : bytes {
      joinArgDepr();
      return join(x=S);
    }

    inline proc join(ir: _iteratorRecord): bytes {
      return doJoinIterator(this, ir);
    }

    // TODO: we don't need this
    inline proc _join(const ref S) : bytes where isTuple(S) || isArray(S) {
      return doJoin(this, S);
    }

    // to capture the deprecated formal name "errors"
    pragma "last resort"
    proc decode(errors=decodePolicy.strict): string throws {
      compilerWarning("'errors' argument to bytes.decode is deprecated. ",
                      "Use 'policy' instead.");
      return this.decode(policy=errors);
    }

    // just to capture the deprecated decodePolicy.ignore and give a compiler
    // warning
    pragma "last resort"
    proc decode(param errors: decodePolicy): string throws {
      compilerWarning("'errors' argument to bytes.decode is deprecated. ",
                      "Use 'policy' instead.");
      if errors == decodePolicy.ignore then
        compilerWarning("decodePolicy.ignore is deprecated. ",
                        "Use decodePolicy.drop instead");

      // have to repeat this as above to avoid recursion. That's the cleanest
      // way I could think of.
      var localThis: bytes = this.localize();
      return decodeByteBuffer(localThis.buff, localThis.buffLen, errors);
    }

     proc decode(param policy: decodePolicy): string throws {
      if policy == decodePolicy.ignore then
        compilerWarning("decodePolicy.ignore is deprecated. ",
                        "Use decodePolicy.drop instead");

      // have to repeat this as above to avoid recursion. That's the cleanest
      // way I could think of.
      var localThis: bytes = this.localize();
      return decodeByteBuffer(localThis.buff, localThis.buffLen, policy);
    }
  } // end of record bytes

  /* Deprecated - please use :proc:`bytes.size`. */
  inline proc bytes.length {
    compilerWarning("'bytes.length' is deprecated - " +
                    "please use 'bytes.size' instead");
    return buffLen;
  }

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
    return _search_helper(needle, region, count=false): idxType;
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
    return _search_helper(needle, region, count=false,
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
    return _search_helper(needle, region, count=true);
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
  iter bytes.split(maxsplit: int = -1) : bytes {
    if !this.isEmpty() {
      const localThis: bytes = this.localize();
      var done : bool = false;
      var yieldChunk : bool = false;
      var chunk : bytes;

      const noSplits : bool = maxsplit == 0;
      const limitSplits : bool = maxsplit > 0;
      var splitCount: int = 0;
      const iEnd: idxType = localThis.buffLen - 2;

      var inChunk : bool = false;
      var chunkStart : idxType;

      for (i,c) in zip(this.indices, localThis.bytes()) {
        // emit whole string, unless all whitespace
        // TODO Engin: Why is this inside the loop?
        if noSplits {
          done = true;
          if !localThis.isSpace() then {
            chunk = localThis;
            yieldChunk = true;
          }
        } else {
          var cSpace = byte_isWhitespace(c);
          // first char of a chunk
          if !(inChunk || cSpace) {
            chunkStart = i;
            inChunk = true;
            if i > iEnd {
              chunk = localThis[chunkStart..];
              yieldChunk = true;
              done = true;
            }
          } else if inChunk {
            // first char out of a chunk
            if cSpace {
              splitCount += 1;
              // last split under limit
              if limitSplits && splitCount > maxsplit {
                chunk = localThis[chunkStart..];
                yieldChunk = true;
                done = true;
              // no limit
              } else {
                chunk = localThis[chunkStart..i-1];
                yieldChunk = true;
                inChunk = false;
              }
            // out of chars
            } else if i > iEnd {
              chunk = localThis[chunkStart..];
              yieldChunk = true;
              done = true;
            }
          }
        }

        if yieldChunk {
          yield chunk;
          yieldChunk = false;
        }
        if done then
          break;
      }
    }

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
      if this.isEmpty() then return b"";
      if chars.isEmpty() then return this;

      const localThis: bytes = this.localize();
      const localChars: bytes = chars.localize();

      var start: idxType = 0;
      var end: idxType = localThis.buffLen-1;

      if leading {
        label outer for (i, thisChar) in zip(this.indices, localThis.bytes()) {
          for removeChar in localChars.bytes() {
            if thisChar == removeChar {
              start = i + 1;
              continue outer;
            }
          }
          break;
        }
      }

      if trailing {
        // Because we are working with codepoints whose starting byte index
        // is not initially known, it is faster to work forward, assuming we
        // are already past the end of the string, and then update the end
        // point as we are proven wrong.
        end = 0;
        label outer for (i, thisChar) in zip(this.indices, localThis.bytes()) {
          for removeChar in localChars.bytes() {
            if thisChar == removeChar {
              continue outer;
            }
          }
          // This was not a character to be removed, so update tentative end.
          end = i;
        }
      }

      return localThis[start..end];

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
  inline proc _cast(type t: bytes, x: string) {
    return createBytesWithNewBuffer(x.buff, length=x.numBytes, size=x.numBytes+1);
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
  proc comparisonDeprWarn() {
    compilerWarning("Comparison between bytes and string is deprecated. " +
                    "Cast the string to bytes first");
  }

  pragma "no doc"
  proc ==(a: bytes, b: string) : bool {
    comparisonDeprWarn();
    return doEq(a,b);
  }

  pragma "no doc"
  proc ==(a: string, b: bytes) : bool {
    comparisonDeprWarn();
    return doEq(a,b);
  }

  pragma "no doc"
  inline proc !=(a: bytes, b: string) : bool {
    comparisonDeprWarn();
    return !doEq(a,b);
  }

  pragma "no doc"
  inline proc !=(a: string, b: bytes) : bool {
    comparisonDeprWarn();
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

  // character-wise operation helpers

  require "ctype.h";

  private inline proc byte_isAscii(c: byteType): bool {
    pragma "fn synchronization free"
    extern proc isascii(c: c_int): c_int;
    return isascii(c: c_int) != 0;
  }

  private inline proc byte_isWhitespace(c: byteType): bool {
    pragma "fn synchronization free"
    extern proc isspace(c: c_int): c_int;
    return isspace(c: c_int) != 0;
  }

  private inline proc byte_isPrintable(c: byteType): bool {
    pragma "fn synchronization free"
    extern proc isprint(c: c_int): c_int;
    return isprint(c: c_int) != 0;
  }

  private inline proc byte_isAlpha(c: byteType): bool {
    pragma "fn synchronization free"
    extern proc isalpha(c: c_int): c_int;
    return isalpha(c: c_int) != 0;
  }

  private inline proc byte_isUpper(c: byteType): bool {
    pragma "fn synchronization free"
    extern proc isupper(c: c_int): c_int;
    return isupper(c: c_int) != 0;
  }

  private inline proc byte_isLower(c: byteType): bool {
    pragma "fn synchronization free"
    extern proc islower(c: c_int): c_int;
    return islower(c: c_int) != 0;
  }

  private inline proc byte_isDigit(c: byteType): bool {
    pragma "fn synchronization free"
    extern proc isdigit(c: c_int): c_int;
    return isdigit(c: c_int) != 0;
  }

  private inline proc byte_isAlnum(c: byteType): bool {
    pragma "fn synchronization free"
    extern proc isalnum(c: c_int): c_int;
    return isalnum(c: c_int) != 0;
  }

  private inline proc byte_toUpper(c: byteType): byteType {
    pragma "fn synchronization free"
    extern proc toupper(c: c_int): c_int;
    return toupper(c: c_int):byteType;
  }

  private inline proc byte_toLower(c: byteType): byteType {
    pragma "fn synchronization free"
    extern proc tolower(c: c_int): c_int;
    return tolower(c: c_int):byteType;
  }

  //
  // hashing support
  //

  pragma "no doc"
  inline proc chpl__defaultHash(x : bytes): uint {
    return getHash(x);
  }

} // end of module Bytes
