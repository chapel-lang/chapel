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
 * NOTES:
 *
 * - An empty string is represented by len == 0 and buff == nil.
 *
 * - It is assumed the bufferType is a local-only type, so we never
 *   make a remote copy of one passed in by the user, though remote
 *   copies are made of internal bufferType variables.
 *
 * - Lots of things are marked with TODO, a few glaring issues have been marked
 *   with BUG
 *
 */

// Note - the I/O module has
// :proc:`string.format` and :proc:`stringify`.
// It might be worth moving them here for documentation - KB Feb 2016

/*
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

This module supports casts from :mod:`string <String>` to numeric types. Such
casts will convert the string to the numeric type and throw an error if the
string is invalid. For example:

.. code-block:: chapel

  var number = "a":int;

throws an error when it is executed, but

.. code-block:: chapel

  var number = "1":int;

stores the value ``1`` in ``number``.

To learn more about handling these errors, see the
:ref:`Error Handling technical note <readme-errorHandling>`.


Unicode Support
---------------

Chapel strings use the UTF-8 encoding. Note that ASCII strings are a simple
subset of UTF-8 strings, because every ASCII character is a UTF-8 character with
the same meaning.

UTF-8 strings might not work properly if a UTF-8 environment is not used. See
:ref:`character set environment <readme-chplenv.character_set>` for more
information.

.. _string.nonunicode:

Non-Unicode Data and Chapel Strings
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For doing string operations on non-Unicode or arbitrary data, consider using
:mod:`bytes <Bytes>` instead of string. However, there may be cases where
:mod:`string <String>` must be used with non-Unicode data. Examples of this are
file system and path operations on systems where UTF-8 file names are not
enforced.


In such scenarios, non-UTF-8 data can be escaped and stored in a string in a way
that it can be restored when needed. For example:

.. code-block:: chapel

 var myBytes = b"Illegal \xff sequence";  // \xff is non UTF-8
 var myEscapedString = myBytes.decode(policy=decodePolicy.escape);

will escape the illegal `0xFF` byte and store it in the string. The escaping
strategy is similar to Python's "surrogate escapes" and is as follows.

 - Each individual byte in an illegal sequence is bitwise-or'ed with `0xDC00` to
   create a 2-byte codepoint.
 - Then, this codepoint is encoded in UTF-8 and stored in the string buffer.

This strategy typically results in storing 3 bytes for each byte in the illegal
sequence. Similarly escaped strings can also be created with
:proc:`createStringWithNewBuffer` using a C buffer.

An escaped data sequence can be reconstructed with :proc:`~string.encode`:

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
  strings described above for paths and file names.


Lengths and Offsets in Unicode Strings
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

 */
module String {
  private use ChapelStandard;
  private use SysCTypes;
  private use ByteBufferHelpers;
  private use BytesStringCommon;
  private use SysBasic;

  public use CString;
  public use StringCasts;
  public use BytesStringCommon only encodePolicy;  // expose encodePolicy

  private use NVStringFactory;

  pragma "fn synchronization free"
  private extern proc qio_decode_char_buf(ref chr:int(32),
                                          ref nbytes:c_int,
                                          buf:c_string,
                                          buflen:ssize_t): syserr;
  pragma "fn synchronization free"
  private extern proc qio_decode_char_buf_esc(ref chr:int(32),
                                              ref nbytes:c_int,
                                              buf:c_string,
                                              buflen:ssize_t): syserr;
  pragma "fn synchronization free"
  private extern proc qio_encode_char_buf(dst:c_void_ptr, chr:int(32)):syserr;
  pragma "fn synchronization free"
  private extern proc qio_nbytes_char(chr:int(32)):c_int;

  pragma "no doc"
  extern const CHPL_SHORT_STRING_SIZE : c_int;

  pragma "no doc"
  extern record chpl__inPlaceBuffer {};

  pragma "fn synchronization free"
  pragma "no doc"
  extern proc chpl__getInPlaceBufferData(const ref data : chpl__inPlaceBuffer) : bufferType;

  // Signal to the Chapel compiler that the actual argument may be modified.
  pragma "fn synchronization free"
  pragma "no doc"
  extern proc chpl__getInPlaceBufferDataForWrite(ref data : chpl__inPlaceBuffer) : bufferType;

  private config param debugStrings = false;

  pragma "no doc"
  record __serializeHelper {
    var buffLen: int;
    var buff: bufferType;
    var size: int;
    var locale_id: chpl_nodeID.type;
    var shortData: chpl__inPlaceBuffer;
    var cachedNumCodepoints: int;
  }

  /*
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

   */
  pragma "plain old data"
  record byteIndex {
    pragma "no doc"
    var _bindex  : int;

    pragma "no doc"
    proc init() {
      // Let compiler insert defaults
    }
    proc init(i: int) { _bindex = i; }
    proc init=(other: byteIndex) { _bindex = other._bindex; }
    proc init=(i: int) { _bindex = i; }

    proc writeThis(f) throws {
      f <~> _bindex;
    }
  }

  /*
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

   */
  pragma "plain old data"
  record codepointIndex {
    pragma "no doc"
    var _cpindex  : int;

    pragma "no doc"
    proc init() {
      // Let compiler insert defaults
    }
    proc init(i: int) { _cpindex = i; }
    proc init=(i: int) { _cpindex = i; }
    proc init=(cpi: codepointIndex) { _cpindex = cpi._cpindex; }

    proc writeThis(f) throws {
      f <~> _cpindex;
    }
  }

  // Helper routines in support of being able to use ranges of indices
  pragma "no doc"
  proc chpl_build_bounded_range(low: ?t, high: t)
    where t == byteIndex || t == codepointIndex
    return new range(t, _low=low, _high=high);

  pragma "no doc"
  proc chpl_build_low_bounded_range(low: ?t)
    where t == byteIndex || t == codepointIndex
    return new range(t, BoundedRangeType.boundedLow, _low=low);

  pragma "no doc"
  proc chpl_build_high_bounded_range(high: ?t)
    where t == byteIndex || t == codepointIndex
    return new range(t, BoundedRangeType.boundedHigh, _high=high);

  pragma "no doc"
  proc chpl__rangeStrideType(type idxType: byteIndex) type
    return int;

  pragma "no doc"
  proc chpl__rangeStrideType(type idxType: codepointIndex) type
    return int;

  pragma "no doc"
  proc chpl__rangeUnsignedType(type idxType: byteIndex) type
    return uint;

  pragma "no doc"
  proc chpl__rangeUnsignedType(type idxType: codepointIndex) type
    return uint;

  pragma "no doc"
  inline proc chpl__idxToInt(i: ?t)
    where t == byteIndex || t == codepointIndex
    return i:int;

  pragma "no doc"
  inline proc chpl__intToIdx(type idxType: byteIndex, i: int)
    return i: byteIndex;

  pragma "no doc"
  inline proc chpl__intToIdx(type idxType: codepointIndex, i: int)
    return i: codepointIndex;

  pragma "no doc"
  proc chpl__idxTypeToIntIdxType(type idxType: byteIndex) type
    return int;

  pragma "no doc"
  proc chpl__idxTypeToIntIdxType(type idxType: codepointIndex) type
    return int;

  pragma "no doc"
  inline proc >(x: ?t, y: t)
    where t == byteIndex || t == codepointIndex
    return x: int > y: int;

  pragma "no doc"
  inline proc >(x: ?t, y: int)
    where t == byteIndex || t == codepointIndex
    return x: int > y;

  pragma "no doc"
  inline proc >(x: int, y: ?t)
    where t == byteIndex || t == codepointIndex
    return x > y: int;
  // End range helper support

  // Index arithmetic support
  // index + int or int + index --> index
  pragma "no doc"
  inline proc +(x: ?t, y: int)
    where t == byteIndex || t == codepointIndex
    return (x: int + y): t;

  pragma "no doc"
  inline proc +(x: int, y: ?t)
    where t == byteIndex || t == codepointIndex
    return (x + y: int): t;

  pragma "no doc"
  inline proc +(x: bufferType, y: byteIndex) {
    return x+(y:int);
  }

  // index - int --> index
  pragma "no doc"
  inline proc -(x: ?t, y: int)
    where t == byteIndex || t == codepointIndex
    return (x: int - y): t;

  // index - index --> int
  pragma "no doc"
  inline proc -(x: ?t, y: t)
    where t == byteIndex || t == codepointIndex
    return x: int - y: int;

  // other relationals
  pragma "no doc"
  inline proc <(x: ?t, y: t)
    where t == byteIndex || t == codepointIndex
    return x: int < y: int;

  pragma "no doc"
  inline proc <(x: ?t, y: int)
    where t == byteIndex || t == codepointIndex
    return x: int < y;

  pragma "no doc"
  inline proc <(x: int, y: ?t)
    where t == byteIndex || t == codepointIndex
    return x < y: int;

  pragma "no doc"
  inline proc >=(x: ?t, y: t)
    where t == byteIndex || t == codepointIndex
    return x: int >= y: int;

  pragma "no doc"
  inline proc >=(x: ?t, y: int)
    where t == byteIndex || t == codepointIndex
    return x: int >= y;

  pragma "no doc"
  inline proc >=(x: int, y: ?t)
    where t == byteIndex || t == codepointIndex
    return x >= y: int;

  pragma "no doc"
  inline proc <=(x: ?t, y: t)
    where t == byteIndex || t == codepointIndex
    return x: int <= y: int;

  pragma "no doc"
  inline proc <=(x: ?t, y: int)
    where t == byteIndex || t == codepointIndex
    return x: int <= y;

  pragma "no doc"
  inline proc <=(x: int, y: ?t)
    where t == byteIndex || t == codepointIndex
    return x <= y: int;

  pragma "no doc"
  inline proc ==(x: ?t, y: t)
    where t == byteIndex || t == codepointIndex
    return (x:int) == (y:int);

  pragma "no doc"
  inline proc ==(x: ?t, y: int)
    where t == byteIndex || t == codepointIndex
    return (x:int) == y;

  pragma "no doc"
  inline proc ==(x: int, y: ?t)
    where t == byteIndex || t == codepointIndex
    return x == (y:int);

  pragma "no doc"
  inline proc !=(x: ?t, y: t)
    where t == byteIndex || t == codepointIndex
    return (x:int) != (y:int);

  pragma "no doc"
  inline proc !=(x: ?t, y: int)
    where t == byteIndex || t == codepointIndex
    return (x:int) != y;

  pragma "no doc"
  inline proc !=(x: int, y: ?t)
    where t == byteIndex || t == codepointIndex
    return x != (y:int);

  pragma "no doc"
  inline proc !(x: ?t)
    where t == byteIndex || t == codepointIndex
    return !(x:int);

  pragma "no doc"
  inline proc _cond_test(x: byteIndex)
    return x != 0;

  pragma "no doc"
  inline proc _cond_test(x: codepointIndex)
    return x != 0;
  // End index arithmetic support

  private proc validateEncoding(buf, len): int throws {
    extern proc chpl_enc_validate_buf(buf, len, ref numCodepoints) : c_int;
    
    var numCodepoints: int;
    
    if chpl_enc_validate_buf(buf, len, numCodepoints) != 0 {
      throw new DecodeError();
    }
    
    return numCodepoints;
  }

  private proc stringFactoryArgDepr() {
    compilerWarning("createStringWith* with formal argument `s` is deprecated. ",
                    "Use argument name `x` instead");
  }

  private proc joinArgDepr() {
    compilerWarning("string.join with formal argument `S` is deprecated. ",
                    "Use argument name `x` instead");
  }

  //
  // createString* functions
  //

  /*
    Creates a new string which borrows the internal buffer of another string. If
    the buffer is freed before the string returned from this function, accessing
    it is undefined behavior.

    :arg x: Object to borrow the buffer from
    :type x: `string`

    :returns: A new `string`
  */
  inline proc createStringWithBorrowedBuffer(x: string) {
    // we don't validate here because `x` must have been validated already
    var ret: string;
    ret.cachedNumCodepoints = x.cachedNumCodepoints;
    initWithBorrowedBuffer(ret, x);
    return ret;
  }

  pragma "last resort"
  pragma "no doc"
  inline proc createStringWithBorrowedBuffer(s: string) {
    stringFactoryArgDepr();
    return createStringWithBorrowedBuffer(x=s);
  }

  /*
    Creates a new string which borrows the internal buffer of a `c_string`. If
    the buffer is freed before the string returned from this function, accessing
    it is undefined behavior.

    :arg x: Object to borrow the buffer from
    :type x: `c_string`

    :arg length: Length of the string stored in `x` in bytes, excluding the
                 terminating null byte.
    :type length: `int`

    :throws: `DecodeError` if `x` contains non-UTF-8 characters.

    :returns: A new `string`
  */
  inline proc createStringWithBorrowedBuffer(x: c_string, length=x.size) throws {
    return createStringWithBorrowedBuffer(x:c_ptr(uint(8)), length=length,
                                                            size=length+1);
  }

  pragma "last resort"
  pragma "no doc"
  inline proc createStringWithBorrowedBuffer(s: c_string, length=s.size) throws {
    stringFactoryArgDepr();
    return createStringWithBorrowedBuffer(x=s, length);
  }

  pragma "no doc"
  proc chpl_createStringWithLiteral(x: c_string, length: int, numCodepoints: int) {
    // NOTE: This is a "wellknown" function used by the compiler to create
    // string literals. Inlining this creates some bloat in the AST, slowing the
    // compilation.
    return chpl_createStringWithBorrowedBufferNV(x:c_ptr(uint(8)),
                                                 length=length,
                                                 size=length+1,
                                                 numCodepoints=numCodepoints);
  }

  /*
     Creates a new string which borrows the memory allocated for a
     `c_ptr(uint(8))`. If the buffer is freed before the string returned from
     this function, accessing it is undefined behavior.

     :arg x: Object to borrow the buffer from
     :type x: `bufferType` (i.e. `c_ptr(uint(8))`)

     :arg length: Length of the string stored in `x` in bytes, excluding the
                  terminating null byte.
     :type length: `int`

     :arg size: Size of memory allocated for `x` in bytes
     :type length: `int`

     :throws: `DecodeError` if `x` contains non-UTF-8 characters.

     :returns: A new `string`
  */
  inline proc createStringWithBorrowedBuffer(x: bufferType,
                                             length: int, size: int) throws {
    var ret: string;
    ret.cachedNumCodepoints = validateEncoding(x, length);
    initWithBorrowedBuffer(ret, x, length,size);
    return ret;
  }

  pragma "last resort"
  pragma "no doc"
  inline proc createStringWithBorrowedBuffer(s: bufferType,
                                             length: int, size: int) throws {
    stringFactoryArgDepr();
    return createStringWithBorrowedBuffer(x=s, length, size);
  }


  pragma "no doc"
  inline proc createStringWithOwnedBuffer(x: string) {
    // should we allow stealing ownership?
    compilerError("A Chapel string cannot be passed to createStringWithOwnedBuffer");
  }

  pragma "last resort"
  pragma "no doc"
  inline proc createStringWithOwnedBuffer(s: string) {
    stringFactoryArgDepr();
    return createStringWithOwnedBuffer(x=s);
  }

  /*
    Creates a new string which takes ownership of the internal buffer of a
    `c_string`. The buffer will be freed when the string is deinitialized.

    :arg x: Object to take ownership of the buffer from
    :type x: `c_string`

    :arg length: Length of the string stored in `x` in bytes, excluding the
                 terminating null byte.
    :type length: `int`

     :throws: `DecodeError` if `x` contains non-UTF-8 characters.

    :returns: A new `string`
  */
  inline proc createStringWithOwnedBuffer(x: c_string, length=x.size) throws {
    return createStringWithOwnedBuffer(x: bufferType, length=length,
                                                      size=length+1);
  }

  pragma "last resort"
  pragma "no doc"
  inline proc createStringWithOwnedBuffer(s: c_string, length=s.size) throws {
    stringFactoryArgDepr();
    return createStringWithOwnedBuffer(x=s, length);
  }

  /*
     Creates a new string which takes ownership of the memory allocated for a
     `c_ptr(uint(8))`. The buffer will be freed when the string is deinitialized.

     :arg x: Object to take ownership of the buffer from
     :type x: `bufferType` (i.e. `c_ptr(uint(8))`)

     :arg length: Length of the string stored in `x` in bytes, excluding the
                  terminating null byte.
     :type length: `int`

     :arg size: Size of memory allocated for `x` in bytes
     :type length: `int`

     :throws: `DecodeError` if `x` contains non-UTF-8 characters.

     :returns: A new `string`
  */
  inline proc createStringWithOwnedBuffer(x: bufferType,
                                          length: int, size: int) throws {
    var ret: string;
    ret.cachedNumCodepoints = validateEncoding(x, length);
    initWithOwnedBuffer(ret, x, length, size);
    return ret;
  }

  pragma "last resort"
  pragma "no doc"
  inline proc createStringWithOwnedBuffer(s: bufferType,
                                          length: int, size: int) throws {
    stringFactoryArgDepr();
    return createStringWithOwnedBuffer(x=s, length, size);
  }

  /*
    Creates a new string by creating a copy of the buffer of another string.

    :arg x: Object to copy the buffer from
    :type x: `string`

    :returns: A new `string`
  */
  inline proc createStringWithNewBuffer(x: string) {
    // we don't validate here because `x` must have been validated already
    var ret: string;
    ret.cachedNumCodepoints = x.numCodepoints;
    initWithNewBuffer(ret, x);
    return ret;
  }

  pragma "last resort"
  pragma "no doc"
  inline proc createStringWithNewBuffer(s: string) {
    stringFactoryArgDepr();
    return createStringWithNewBuffer(x=s);
  }

  /*
    Creates a new string by creating a copy of the buffer of a `c_string`.

    :arg x: Object to copy the buffer from
    :type x: `c_string`

    :arg length: Length of the string stored in `x` in bytes, excluding the
                 terminating null byte.
    :type length: `int`

    :arg policy: - `decodePolicy.strict` raises an error
                 - `decodePolicy.replace` replaces the malformed character with
                   UTF-8 replacement character
                 - `decodePolicy.drop` drops the data silently
                 - `decodePolicy.escape` escapes each illegal byte with private
                   use codepoints

    :throws: `DecodeError` if `decodePolicy.strict` is passed to the `policy`
             argument and `x` contains non-UTF-8 characters.

    :returns: A new `string`
  */
  inline proc createStringWithNewBuffer(x: c_string, length=x.size,
                                        policy=decodePolicy.strict) throws {
    return createStringWithNewBuffer(x: bufferType, length=length,
                                     size=length+1, policy);
  }

  pragma "last resort"
  pragma "no doc"
  inline proc createStringWithNewBuffer(s: c_string, length=s.size,
                                        policy=decodePolicy.strict) throws {
    stringFactoryArgDepr();
    return createStringWithNewBuffer(x=s, length, policy);
  }

  /*
     Creates a new string by creating a copy of a buffer.

     :arg x: The buffer to copy
     :type x: `bufferType` (i.e. `c_ptr(uint(8))`)

     :arg length: Length of the string stored in `x` in bytes, excluding the
                  terminating null byte.
     :type length: `int`

     :arg size: Size of memory allocated for `x` in bytes. This argument is
                ignored by this function.
     :type size: `int`

      :arg policy: `decodePolicy.strict` raises an error, `decodePolicy.replace`
                   replaces the malformed character with UTF-8 replacement
                   character, `decodePolicy.drop` drops the data silently,
                   `decodePolicy.escape` escapes each illegal byte with private
                   use codepoints

     :throws: `DecodeError` if `x` contains non-UTF-8 characters.

     :returns: A new `string`
  */
  inline proc createStringWithNewBuffer(x: bufferType,
                                        length: int, size=length+1,
                                        policy=decodePolicy.strict) throws {
    // size argument is not used, because we're allocating our own buffer
    // anyways. But it has a default and probably it's good to keep it here for
    // interface consistency
    return decodeByteBuffer(x, length, policy);
  }

  pragma "last resort"
  pragma "no doc"
  inline proc createStringWithNewBuffer(s: bufferType,
                                        length: int, size=length+1,
                                        policy=decodePolicy.strict) throws {
    stringFactoryArgDepr();
    return createStringWithNewBuffer(x=s, length, size, policy);
  }

  // non-validating string factory functions are in this submodule. This
  // submodule can be `private use`d from other String-supporting modules.
  pragma "no doc"
  module NVStringFactory {
    private use BytesStringCommon;
    private use ByteBufferHelpers only bufferType;

    inline proc chpl_createStringWithNewBufferNV(x: bufferType,
                                                 length: int,
                                                 size: int,
                                                 numCodepoints: int) {
      var ret: string;
      initWithNewBuffer(ret, x, length, size);
      ret.cachedNumCodepoints = numCodepoints;
      return ret;
    }

    inline proc chpl_createStringWithBorrowedBufferNV(x: bufferType,
                                                      length: int,
                                                      size: int,
                                                      numCodepoints: int) {
      // NOTE: This is similar to chpl_createStringWithLiteral, but only used
      // internally by the String module. These two functions cannot have the
      // same names, because "wellknown" implementation in the compiler does not
      // allow overloads.
      var ret: string;
      initWithBorrowedBuffer(ret, x, length, size);
      ret.cachedNumCodepoints = numCodepoints;
      return ret;
    }

    inline proc chpl_createStringWithOwnedBufferNV(x: bufferType,
                                                   length: int,
                                                   size: int,
                                                   numCodepoints: int) {
      var ret: string;
      initWithOwnedBuffer(ret, x, length, size);
      ret.cachedNumCodepoints = numCodepoints;
      return ret;
    }
  }

  //
  // String Implementation
  //
  // TODO: We should be able to remove "ignore noinit", but doing so causes
  // memory leaks in various places. Investigate why later and add noinit back
  // in when possible.
  pragma "ignore noinit"
  pragma "no default functions" // avoid the default (read|write)This routines
  pragma "no doc"
  record _string {
    var buffLen: int = 0; // length of string in bytes
    var buffSize: int = 0; // size of the buffer we own
    var cachedNumCodepoints: int = 0;
    var buff: bufferType = nil;
    var isOwned: bool = true;
    var hasEscapes: bool = false;
    // We use chpl_nodeID as a shortcut to get at here.id without actually constructing
    // a locale object. Used when determining if we should make a remote transfer.
    var locale_id = chpl_nodeID; // : chpl_nodeID_t

    proc init() {
      // Let compiler insert defaults
    }

    proc init=(s: string) {
      this.complete();
      initWithNewBuffer(this, s);
    }

    proc init=(cs: c_string) {
      this.complete();
      initWithNewBuffer(this, cs:bufferType, length=cs.size, size=cs.size+1);
    }

    proc ref deinit() {
      // Checking for size here isn't sufficient. A string may have been
      // initialized from a c_string allocated from memory but beginning with
      // a null-terminator.
      if isOwned && this.buff != nil {
        on __primitive("chpl_on_locale_num",
                       chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
          chpl_here_free(this.buff);
        }
      }
    }
    
    proc chpl__serialize() {
      var data : chpl__inPlaceBuffer;
      if buffLen <= CHPL_SHORT_STRING_SIZE {
        chpl_string_comm_get(chpl__getInPlaceBufferDataForWrite(data), locale_id, buff, buffLen);
      }
      return new __serializeHelper(buffLen, buff, buffSize, locale_id, data,
                                   cachedNumCodepoints);
    }
    
    proc type chpl__deserialize(data) {
      if data.locale_id != chpl_nodeID {
        if data.buffLen <= CHPL_SHORT_STRING_SIZE {
          return chpl_createStringWithNewBufferNV(
                      chpl__getInPlaceBufferData(data.shortData),
                      data.buffLen,
                      data.size,
                      data.cachedNumCodepoints);
        } else {
          var localBuff = bufferCopyRemote(data.locale_id, data.buff, data.buffLen);
          return chpl_createStringWithOwnedBufferNV(localBuff,
                                                    data.buffLen,
                                                    data.size,
                                                    data.cachedNumCodepoints);
        }
      } else {
        return chpl_createStringWithBorrowedBufferNV(data.buff,
                                                     data.buffLen,
                                                     data.size,
                                                     data.cachedNumCodepoints);
      }
    }

    inline proc byteIndices return 0..<this.numBytes;

    inline proc param c_str() param : c_string {
      return this:c_string; // folded out in resolution
    }

    /*
      Iterates over the string Unicode character by Unicode character,
      and includes the byte index and byte length of each character.
      Skip characters that begin prior to the specified starting byte index.
      Assume we may accidentally start in the middle of a multibyte character,
      but the string is correctly encoded UTF-8.
    */
    iter _cpIndexLen(start = 0:byteIndex) {
      var localThis: string = this.localize();

      var i = start:int;
      if i > 0 then
        while i < localThis.buffLen && !isInitialByte(localThis.buff[i]) do
          i += 1; // in case `start` is in the middle of a multibyte character
      while i < localThis.buffLen {
        const (decodeRet, cp, nBytes) = decodeHelp(buff=localThis.buff,
                                                   buffLen=localThis.buffLen,
                                                   offset=i,
                                                   allowEsc=true);
        yield (cp:int(32), i:byteIndex, nBytes:int);
        i += nBytes;
      }
    }

    /*
      Iterates over the string Unicode character by Unicode character,
      and returns the byte index and byte length of each character.
      Skip characters that begin prior to the specified starting byte index.
      Assume we may accidentally start in the middle of a multibyte character,
      but the string is correctly encoded UTF-8.
    */
    iter _indexLen(start = 0:byteIndex) {
      var localThis: string = this.localize();

      var i = start:int;
      if i > 0 then
        while i < localThis.buffLen && !isInitialByte(localThis.buff[i]) do
          i += 1; // in case `start` is in the middle of a multibyte character
      while i < localThis.buffLen {
        var j = i + 1;
        while j < localThis.buffLen && !isInitialByte(localThis.buff[j]) do
          j += 1;
        yield (i:byteIndex, j - i);
        i = j;
      }
    }
    
    inline proc substring(i: int) {
      compilerError("substring removed: use string[index]");
    }

    inline proc substring(r: range) {
      compilerError("substring removed: use string[range]");
    }

    // These should never be called (but are default functions for records)
    proc writeThis(f) throws {
      compilerError("not implemented: writeThis");
    }

    proc readThis(f) throws {
      compilerError("not implemented: readThis");
    }

    // Helper function that uses a param bool to toggle between count and find
    //TODO: this could be a much better string search
    //      (Boyer-Moore-Horspool|any thing other than brute force)
    //
    inline proc _search_helper(needle: string, region: range(?),
                               param count: bool, param fromLeft: bool = true) {
      // needle.len is <= than this.buffLen, so go to the home locale
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
            if ((region.hasLowBound() && region.low.type == byteIndex) ||
                (region.hasHighBound() && region.high.type == byteIndex)) {
              // Byte indexed, so count the number of bytes in the view
              localRet = thisLen;
            } else {
              // Count the number of codepoints in the view + 1
              var nCodepoints = 0;
              var nextIdx = 0;
              for i in view {
                if i >= nextIdx {
                  nCodepoints += 1;
                  const (decodeRet, cp, nBytes) = decodeHelp(buff=this.buff,
                                                             buffLen=this.buffLen,
                                                             offset=i,
                                                             allowEsc=true);
                  nextIdx = i + nBytes;
                }
              }
              localRet = nCodepoints;
            }
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
          const localNeedle: string = needle.localize();

          // i *is not* an index into anything, it is the order of the element
          // of view we are searching from.
          const numPossible = thisLen - nLen + 1;
          const searchSpace = if fromLeft
              then 0..#(numPossible)
              else 0..#(numPossible) by -1;
          //          writeln("view is ", view);
          for i in searchSpace {
            // j *is* the index into the localNeedle's buffer
            for j in 0..#nLen {
              const idx = view.orderToIndex(i+j); // 0s based idx
              if this.buff[idx] != localNeedle.buff[j] then break;

              if j == nLen-1 {
                if count {
                  localRet += 1;
                } else { // find
                  localRet = view.orderToIndex(i);
                }
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
    inline proc join(const ref S) : string where isTuple(S) {
      joinArgDepr();
      return join(S);
    }

    pragma "last resort"
    inline proc join(const ref S: [] string) : string {
      joinArgDepr();
      return join(S);
    }

    inline proc join(ir: _iteratorRecord): string {
      return doJoinIterator(this, ir);
    }

    // TODO: we don't need this
    inline proc _join(const ref S) : string where isTuple(S) || isArray(S) {
      return doJoin(this, S);
    }

    /*
      :returns: A new string with the first character in uppercase (if it is a
                case character), and all other case characters in lowercase.
                Uncased characters are copied with no changes.
    */
    proc capitalize() : string {
      var result: string = this.toLower();
      if result.isEmpty() then return result;
      const (decodeRet, cp, nBytes) = decodeHelp(buff=result.buff,
                                                 buffLen=result.buffLen,
                                                 offset=0,
                                                 allowEsc=false);
      
      var upCodepoint = codepoint_toUpper(cp);
      if upCodepoint != cp && qio_nbytes_char(upCodepoint) == nBytes {
        // Use the MacOS approach everywhere:  only change the case if
        // the result does not change the number of encoded bytes.
        qio_encode_char_buf(result.buff, upCodepoint);
      }
      return result;
    }

  } // end record string
                                        
  /* Deprecated - please use :proc:`string.size`. */
  inline proc string.length {
    compilerWarning("'string.length' is deprecated - " +
                    "please use 'string.size' instead");
    return numCodepoints;
  }

  /*
    :returns: The number of codepoints in the string.
  */
  inline proc const string.size return numCodepoints;

  /*
    :returns: The indices that can be used to index into the string
              (i.e., the range ``0..<this.size``)
  */
  inline proc string.indices return 0..<size;

  /*
    :returns: The number of bytes in the string.
  */
  inline proc string.numBytes return buffLen;

  /*
    :returns: The number of codepoints in the string, assuming the
              string is correctly-encoded UTF-8.
  */
  inline proc const string.numCodepoints {
    const n = this.cachedNumCodepoints;
    if boundsChecking {
      if n != countNumCodepoints(this) {
        halt("Encountered corrupt string metadata");
      }
    }
    return n;
  }
  
  /*
     Gets a version of the :mod:`string <String>` that is on the currently
     executing locale.

     :returns: A shallow copy if the :mod:`string <String>` is already on the
               current locale, otherwise a deep copy is performed.
  */
  inline proc string.localize() : string {
    if _local || this.locale_id == chpl_nodeID {
      return createStringWithBorrowedBuffer(this);
    } else {
      const x:string = this; // assignment makes it local
      return x;
    }
  }

  /*
    Get a `c_string` from a :mod:`string <String>`.

    .. warning::

        This can only be called safely on a :mod:`string <String>` whose home is
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
        :mod:`string <String>`. The returned `c_string` is only valid when used
        on the same locale as the string.
   */
  inline proc string.c_str(): c_string {
    return getCStr(this);
  }
  
  /*
    Returns a :mod:`bytes <Bytes>` from the given :mod:`string <String>`. If the
    string contains some escaped non-UTF8 bytes, `policy` argument determines
    the action.
        
    :arg policy: `encodePolicy.pass` directly copies the (potentially escaped)
                  data, `encodePolicy.unescape` recovers the escaped bytes
                  back.

    :returns: :mod:`bytes <Bytes>`
  */
  proc string.encode(policy=encodePolicy.pass): bytes {
    var localThis: string = this.localize();

    if policy == encodePolicy.pass {  // just copy
      return createBytesWithNewBuffer(localThis.buff, localThis.numBytes);
    }
    else {  // see if there is escaped data in the string
      var (buff, size) = bufferAlloc(this.buffLen+1);

      var readIdx = 0;
      var writeIdx = 0;
      while readIdx < localThis.buffLen {
        var multibytes = localThis.buff + readIdx;
        const (decodeRet, cp, nBytes) = decodeHelp(buff=localThis.buff,
                                                   buffLen=localThis.buffLen,
                                                   offset=readIdx,
                                                   allowEsc=true);
        if (0xdc80<=cp && cp<=0xdcff) {
          buff[writeIdx] = (cp-0xdc00):byteType;
          writeIdx += 1;
        }
        else if (decodeRet != 0) {
          // the string contains invalid data
          // at this point this can only happen due to a failure in our
          // implementation of string encoding/decoding
          // simply copy the data out
          bufferMemcpyLocal(dst=(buff+writeIdx), src=multibytes, len=nBytes);
          writeIdx += nBytes;
        }
        else {
          bufferMemcpyLocal(dst=(buff+writeIdx), src=multibytes, len=nBytes);
          writeIdx += nBytes;
        }
        readIdx += nBytes;
      }
      buff[writeIdx] = 0;
      return createBytesWithOwnedBuffer(buff, length=writeIdx, size=size);
    }
  }

  /*
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
   */
  iter string.items() : string {
    var localThis: string = this.localize();

    var i = 0;
    while i < localThis.buffLen {
      const curPos = localThis.buff+i;
      const (decodeRet, cp, nBytes) = decodeHelp(buff=localThis.buff,
                                                   buffLen=localThis.buffLen,
                                                   offset=i,
                                                   allowEsc=true);
      var (newBuf, newSize) = bufferCopyLocal(curPos, nBytes);
      newBuf[nBytes] = 0;

      yield chpl_createStringWithOwnedBufferNV(newBuf, nBytes, newSize, 1);

      i += nBytes;
    }
  }

  /*
    Iterates over the string character by character, yielding 1-codepoint
    strings. (A synonym for :iter:`string.items`)

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
   */
  iter string.these() : string {
    for c in this.items() do
      yield c;
  }

  /*
    Iterates over the string byte by byte.
  */
  iter string.chpl_bytes(): byteType {
    var localThis: string = this.localize();

    for i in 0..#localThis.buffLen {
      yield localThis.buff[i];
    }
  }

  /*
    Iterates over the string Unicode character by Unicode character.
  */
  iter string.codepoints(): int(32) {
    var localThis: string = this.localize();

    var i = 0;
    while i < localThis.buffLen {
      const (decodeRet, cp, nBytes) = decodeHelp(buff=localThis.buff,
                                                 buffLen=localThis.buffLen,
                                                 offset=i,
                                                 allowEsc=true);
      yield cp;
      i += nBytes;
    }
  }

  /*
    :returns: The value of a single-byte string as an integer.
  */
  proc string.toByte(): uint(8) {
    if this.buffLen != 1 then
      halt("string.toByte() only accepts single-byte strings");
    return bufferGetByte(buf=this.buff, off=0, loc=this.locale_id);
  }

  /*
    :returns: The value of the `i` th byte as an integer.
  */
  proc string.byte(i: int): uint(8) {
    if boundsChecking && (i < 0 || i >= this.buffLen)
      then halt("index ", i, " out of bounds for string with ", this.numBytes, " bytes");
    return bufferGetByte(buf=this.buff, off=i, loc=this.locale_id);
  }

  /*
    :returns: The value of a single-codepoint string as an integer.
   */
  proc string.toCodepoint(): int(32) {
    // TODO: Engin: at least we can check whether the length is less than 4
    // bytes before localizing?
    var localThis: string = this.localize();

    if localThis.isEmpty() then
      halt("string.toCodepoint() only accepts single-codepoint strings");

    const (decodeRet, cp, nBytes) = decodeHelp(buff=localThis.buff,
                                               buffLen=localThis.buffLen,
                                               offset=0,
                                               allowEsc=true);
    if localThis.buffLen != nBytes:int then
      halt("string.toCodepoint() only accepts single-codepoint strings");

    return cp;
  }

  /*
    :returns: The value of the `i` th multibyte character as an integer.
   */
  proc string.codepoint(i: int): int(32) {
    // TODO: Engin we may need localize here
    const idx = i: int;
    if boundsChecking && idx < 0 then
      halt("index ", idx, " out of bounds for string");

    var j = 0;
    for cp in this.codepoints() {
      if j == idx then
        return cp;
      j += 1;
    }
    // We have reached the end of the string without finding our index.
    if boundsChecking then
      halt("index ", idx, " out of bounds for string with length ", this.size);
    return 0: int(32);
  }

  /*
    Return the codepoint starting at the `i` th byte in the string

    :returns: A string with the complete multibyte character starting at the
              specified byte index from ``0..#string.numBytes``
   */
  proc string.this(i: byteIndex) : string {
    var idx = i: int;
    if boundsChecking && (idx < 0 || idx >= this.buffLen)
      then halt("index ", i, " out of bounds for string with ", this.buffLen, " bytes");

    var maxbytes = (this.buffLen - idx): ssize_t;
    if maxbytes < 0 || maxbytes > 4 then
      maxbytes = 4;
    var (newBuff, allocSize) = bufferCopy(buf=this.buff, off=idx,
                                          len=maxbytes, loc=this.locale_id);

    const (decodeRet, cp, nBytes) = decodeHelp(buff=newBuff,
                                               buffLen=maxbytes,
                                               offset=0,
                                               allowEsc=true);
    return chpl_createStringWithOwnedBufferNV(newBuff, nBytes, allocSize, 1);
  }

  /*
    Return the `i` th codepoint in the string. (A synonym for :proc:`string.item`)

    :returns: A string with the complete multibyte character starting at the
              specified codepoint index from ``0..#string.numCodepoints``
   */
  proc string.this(i: codepointIndex) : string {
    return this.item(i);
  }

  /*
    Return the `i` th codepoint in the string. (A synonym for :proc:`string.item`)

    :returns: A string with the complete multibyte character starting at the
              specified codepoint index from ``1..string.numCodepoints``
   */
  inline proc string.this(i: int) : string {
    return this.item(i);
  }

  /*
    Return the `i` th codepoint in the string

    :returns: A string with the complete multibyte character starting at the
              specified codepoint index from ``1..string.numCodepoints``
   */
  proc string.item(i: codepointIndex) : string {
    if this.isEmpty() then return "";
    const idx = i: int;
    return codepointToString(this.codepoint(idx));
  }

  /*
    Return the `i` th codepoint in the string

    :returns: A string with the complete multibyte character starting at the
              specified codepoint index from ``0..#string.numCodepoints``
   */
  inline proc string.item(i: int) : string {
    return this[i: codepointIndex];
  }

  /*
    Slice a string. Halts if r is non-empty and not completely inside the
    range ``0..<string.size`` when compiled with `--checks`. `--fast`
    disables this check.

    :arg r: range of the indices the new string should be made from

    :returns: a new string that is a substring within ``0..<string.size``. If
              the length of `r` is zero, an empty string is returned.
   */
  inline proc string.this(r: range(?)) : string {
    return getSlice(this, r);
  }

  /*
    :returns: * `true`  -- when the string is empty
              * `false` -- otherwise
   */
  inline proc string.isEmpty() : bool {
    return this.buffLen == 0; // this should be enough of a check
  }

  /*
    :arg needles: A varargs list of strings to match against.

    :returns: * `true`  -- when the string begins with one or more of the `needles`
              * `false` -- otherwise
   */
  inline proc string.startsWith(needles: string ...) : bool {
    return startsEndsWith(this, needles, fromLeft=true);
  }

  /*
    :arg needles: A varargs list of strings to match against.

    :returns: * `true`  -- when the string ends with one or more of the `needles`
              * `false` -- otherwise
   */
  inline proc string.endsWith(needles: string ...) : bool {
    return startsEndsWith(this, needles, fromLeft=false);
  }

  /*
    :arg needle: the string to search for
    :arg region: an optional range defining the substring to search within,
                 default is the whole string. Halts if the range is not
                 within ``0..<string.size``

    :returns: the index of the first occurrence of `needle` within a
              string, or -1 if the `needle` is not in the string.
   */
  inline proc string.find(needle: string,
                   region: range(?) = this.byteIndices:range(byteIndex)) : byteIndex {
    // TODO: better name than region?
    return _search_helper(needle, region, count=false): byteIndex;
  }

  /*
    :arg needle: the string to search for
    :arg region: an optional range defining the substring to search within,
                 default is the whole string. Halts if the range is not
                 within ``0..<string.size``

    :returns: the index of the first occurrence from the right of `needle`
              within a string, or -1 if the `needle` is not in the string.
   */
  inline proc string.rfind(needle: string,
                    region: range(?) = this.byteIndices:range(byteIndex)) : byteIndex {
    return _search_helper(needle, region, count=false, fromLeft=false): byteIndex;
  }

  /*
    :arg needle: the string to search for
    :arg region: an optional range defining the substring to search within,
                 default is the whole string. Halts if the range is not
                 within ``0..<string.size``

    :returns: the number of times `needle` occurs in the string
   */
  inline proc string.count(needle: string, region: range(?) = this.indices) : int {
    return _search_helper(needle, region, count=true);
  }

  /*
    :arg needle: the string to search for
    :arg replacement: the string to replace `needle` with
    :arg count: an optional integer specifying the number of replacements to
                make, values less than zero will replace all occurrences

    :returns: a copy of the string where `replacement` replaces `needle` up
              to `count` times
   */
  inline proc string.replace(needle: string, replacement: string, count: int = -1) : string {
    return doReplace(this, needle, replacement, count);
  }

  /*
    Splits the string on `sep` yielding the substring between each
    occurrence, up to `maxsplit` times.

    :arg sep: The delimiter used to break the string into chunks.
    :arg maxsplit: The number of times to split the string, negative values
                   indicate no limit.
    :arg ignoreEmpty: * When `true`  -- Empty strings will not be yielded,
                                        and will not count towards `maxsplit`
                      * When `false` -- Empty strings will be yielded when
                                        `sep` occurs multiple times in a row.
   */
    iter string.split(sep: string, maxsplit: int = -1, ignoreEmpty: bool = false) /* : string */ {
    // TODO: specifying return type leads to un-inited string?
    for s in doSplit(this, sep, maxsplit, ignoreEmpty) do yield s;
  }

  /*
    Works as above, but uses runs of whitespace as the delimiter.

    :arg maxsplit: The number of times to split the string, negative values
                   indicate no limit.
   */
  iter string.split(maxsplit: int = -1) /* : string */ {
    // note: to improve performance, this code collapses several cases into a
    //       single yield statement, which makes it confusing to read
    // TODO: specifying return type leads to un-inited string?
    if !this.isEmpty() {
      const localThis: string = this.localize();
      var done : bool = false;
      var yieldChunk : bool = false;
      var chunk : string;

      const noSplits : bool = maxsplit == 0;
      const limitSplits : bool = maxsplit > 0;
      var splitCount: int = 0;
      const iEnd: byteIndex = localThis.buffLen - 2;

      var inChunk : bool = false;
      var chunkStart : byteIndex;

      for (c, i, nBytes) in localThis._cpIndexLen() {
        // emit whole string, unless all whitespace
        if noSplits {
          done = true;
          if !localThis.isSpace() then {
            chunk = localThis;
            yieldChunk = true;
          }
        } else {
          var cSpace = codepoint_isWhitespace(c);
          // first char of a chunk
          if !(inChunk || cSpace) {
            chunkStart = i;
            inChunk = true;
            if i - 1 + nBytes > iEnd {
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
            } else if i - 1 + nBytes > iEnd {
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
    Returns a new string, which is the concatenation of all of the strings
    passed in with the receiving string inserted between them.

    .. code-block:: chapel

        var x = "|".join("a","10","d");
        writeln(x); // prints: "a|10|d"
   */
  inline proc string.join(const ref x: string ...) : string {
    return _join(x);
  }

  /*
    Same as the varargs version, but with a homogeneous tuple of strings.

    .. code-block:: chapel

        var x = "|".join("a","10","d");
        writeln(x); // prints: "a|10|d"
   */
  inline proc string.join(const ref x) : string where isTuple(x) {
    if !isHomogeneousTuple(x) || !isString(x[1]) then
      compilerError("join() on tuples only handles homogeneous tuples of strings");
    return _join(x);
  }

  /*
    Same as the varargs version, but with all the strings in an array.

    .. code-block:: chapel

        var x = "|".join(["a","10","d"]);
        writeln(x); // prints: "a|10|d"
   */
  inline proc string.join(const ref S: [] string) : string {
    return _join(S);
  }

    /*
      :arg chars: A string containing each character to remove.
                  Defaults to `" \\t\\r\\n"`.
      :arg leading: Indicates if leading occurrences should be removed.
                    Defaults to `true`.
      :arg trailing: Indicates if trailing occurrences should be removed.
                     Defaults to `true`.

      :returns: A new string with `leading` and/or `trailing` occurrences of
                characters in `chars` removed as appropriate.
    */
    proc string.strip(chars: string = " \t\r\n", leading=true, trailing=true) : string {
      if this.isEmpty() then return "";
      if chars.isEmpty() then return this;

      const localThis: string = this.localize();
      const localChars: string = chars.localize();

      var start: byteIndex = 0;
      var end: byteIndex = localThis.buffLen-1;

      if leading {
        label outer for (thisChar, i, nBytes) in localThis._cpIndexLen() {
          for removeChar in localChars.codepoints() {
            if thisChar == removeChar {
              start = i + nBytes;
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
        end = -1;
        label outer for (thisChar, i, nBytes) in localThis._cpIndexLen(start) {
          for removeChar in localChars.codepoints() {
            if thisChar == removeChar {
              continue outer;
            }
          }
          // This was not a character to be removed, so update tentative end.
          end = i + nBytes-1;
        }
      }

      return localThis[start..end];
    }

    /*
      Splits the string on `sep` into a `3*string` consisting of the section
      before `sep`, `sep`, and the section after `sep`. If `sep` is not found,
      the tuple will contain the whole string, and then two empty strings.
    */
    inline proc const string.partition(sep: string) : 3*string {
      return doPartition(this, sep);
    }

    /*
     Checks if all the characters in the string are either uppercase (A-Z) or
     uncased (not a letter).

      :returns: * `true`  -- if the string contains at least one uppercase
                             character and no lowercase characters, ignoring
                             uncased characters.
                * `false` -- otherwise
     */
    proc string.isUpper() : bool {
      if this.isEmpty() then return false;

      var result: bool;
      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        var locale_result = false;
        for cp in this.codepoints() {
          if codepoint_isLower(cp) {
            locale_result = false;
            break;
          } else if !locale_result && codepoint_isUpper(cp) {
            locale_result = true;
          }
        }
        result = locale_result;
      }
      return result;
    }

    /*
     Checks if all the characters in the string are either lowercase (a-z) or
     uncased (not a letter).

      :returns: * `true`  -- when there are no uppercase characters in the string.
                * `false` -- otherwise
     */
    proc string.isLower() : bool {
      if this.isEmpty() then return false;

      var result: bool;
      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        var locale_result = false;
        for cp in this.codepoints() {
          if codepoint_isUpper(cp) {
            locale_result = false;
            break;
          } else if !locale_result && codepoint_isLower(cp) {
            locale_result = true;
          }
        }
        result = locale_result;
      }
      return result;
    }

    /*
     Checks if all the characters in the string are whitespace (' ', '\t',
     '\n', '\v', '\f', '\r').

      :returns: * `true`  -- when all the characters are whitespace.
                * `false` -- otherwise
     */
    proc string.isSpace() : bool {
      if this.isEmpty() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for cp in this.codepoints() {
          if !(codepoint_isWhitespace(cp)) {
            result = false;
            break;
          }
        }
      }
      return result;
    }

    /*
     Checks if all the characters in the string are alphabetic (a-zA-Z).

      :returns: * `true`  -- when the characters are alphabetic.
                * `false` -- otherwise
     */
    proc string.isAlpha() : bool {
      if this.isEmpty() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for cp in this.codepoints() {
          if !codepoint_isAlpha(cp) {
            result = false;
            break;
          }
        }
      }
      return result;
    }

    /*
     Checks if all the characters in the string are digits (0-9).

      :returns: * `true`  -- when the characters are digits.
                * `false` -- otherwise
     */
    proc string.isDigit() : bool {
      if this.isEmpty() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for cp in this.codepoints() {
          if !codepoint_isDigit(cp) {
            result = false;
            break;
          }
        }
      }
      return result;
    }

    /*
     Checks if all the characters in the string are alphanumeric (a-zA-Z0-9).

      :returns: * `true`  -- when the characters are alphanumeric.
                * `false` -- otherwise
     */
    proc string.isAlnum() : bool {
      if this.isEmpty() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for cp in this.codepoints() {
          if !(codepoint_isAlpha(cp) || codepoint_isDigit(cp)) {
            result = false;
            break;
          }
        }
      }
      return result;
    }

    /*
     Checks if all the characters in the string are printable.

      :returns: * `true`  -- when the characters are printable.
                * `false` -- otherwise
     */
    proc string.isPrintable() : bool {
      if this.isEmpty() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        for cp in this.codepoints() {
          if !codepoint_isPrintable(cp) {
            result = false;
            break;
          }
        }
      }
      return result;
    }

    /*
      Checks if all uppercase characters are preceded by uncased characters,
      and if all lowercase characters are preceded by cased characters.

      :returns: * `true`  -- when the condition described above is met.
                * `false` -- otherwise
     */
    proc string.isTitle() : bool {
      if this.isEmpty() then return false;
      var result: bool = true;

      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_any)) {
        param UN = 0, UPPER = 1, LOWER = 2;
        var last = UN;
        for cp in this.codepoints() {
          if codepoint_isLower(cp) {
            if last == UPPER || last == LOWER {
              last = LOWER;
            } else { // last == UN
              result = false;
              break;
            }
          }
          else if codepoint_isUpper(cp) {
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
      :returns: A new string with all uppercase characters replaced with their
                lowercase counterpart.

      .. note::
        
        The case change operation is not currently performed on characters whose
        cases take different number of bytes to represent in Unicode mapping.
    */
    proc string.toLower() : string {
      var result: string = this;
      if result.isEmpty() then return result;

      var i = 0;
      while i < result.buffLen {
        const (decodeRet, cp, nBytes) = decodeHelp(buff=result.buff,
                                                   buffLen=result.buffLen,
                                                   offset=i,
                                                   allowEsc=false);
        var lowCodepoint = codepoint_toLower(cp);
        if lowCodepoint != cp && qio_nbytes_char(lowCodepoint) == nBytes {
          // Use the MacOS approach everywhere:  only change the case if
          // the result does not change the number of encoded bytes.
          qio_encode_char_buf(result.buff + i, lowCodepoint);
        }
        i += nBytes;
      }
      return result;
    }

    /*
      :returns: A new string with all lowercase characters replaced with their
                uppercase counterpart.

      .. note::
        
        The case change operation is not currently performed on characters whose
        cases take different number of bytes to represent in Unicode mapping.
    */
    proc string.toUpper() : string {
      var result: string = this;
      if result.isEmpty() then return result;

      var i = 0;
      while i < result.buffLen {
        const (decodeRet, cp, nBytes) = decodeHelp(buff=result.buff,
                                                   buffLen=result.buffLen,
                                                   offset=i,
                                                   allowEsc=false);
        var upCodepoint = codepoint_toUpper(cp);
        if upCodepoint != cp && qio_nbytes_char(upCodepoint) == nBytes {
          // Use the MacOS approach everywhere:  only change the case if
          // the result does not change the number of encoded bytes.
          qio_encode_char_buf(result.buff + i, upCodepoint);
        }
        i += nBytes;
      }
      return result;
    }

  /*
    :returns: A new string with all cased characters following an uncased
              character converted to uppercase, and all cased characters
              following another cased character converted to lowercase.

    .. note::

      The case change operation is not currently performed on characters whose
      cases take different number of bytes to represent in Unicode mapping.
   */
  proc string.toTitle() : string {
    var result: string = this;
    if result.isEmpty() then return result;

    param UN = 0, LETTER = 1;
    var last = UN;
    var i = 0;
    while i < result.buffLen {
      const (decodeRet, cp, nBytes) = decodeHelp(buff=result.buff,
                                                 buffLen=result.buffLen,
                                                 offset=i,
                                                 allowEsc=false);
      if codepoint_isAlpha(cp) {
        if last == UN {
          last = LETTER;
          var upCodepoint = codepoint_toUpper(cp);
          if upCodepoint != cp && qio_nbytes_char(upCodepoint) == nBytes {
            // Use the MacOS approach everywhere:  only change the case if
            // the result does not change the number of encoded bytes.
            qio_encode_char_buf(result.buff + i, upCodepoint);
          }
        } else { // last == LETTER
          var lowCodepoint = codepoint_toLower(cp);
          if lowCodepoint != cp && qio_nbytes_char(lowCodepoint) == nBytes {
            // Use the MacOS approach everywhere:  only change the case if
            // the result does not change the number of encoded bytes.
            qio_encode_char_buf(result.buff + i, lowCodepoint);
          }
        }
      } else {
        // Uncased elements
        last = UN;
      }
      i += nBytes;
    }
    return result;
  }

  //
  // Assignment functions
  //
  /*
     Copies the int `rhs` into the byteIndex `lhs`.
  */
  proc =(ref lhs: byteIndex, rhs: int) {
    lhs._bindex = rhs: int;
  }
  pragma "no doc"
  proc =(ref lhs: byteIndex, const ref rhs: byteIndex) {
    lhs._bindex = rhs._bindex;
  }

  /*
     Copies the int `rhs` into the codepointIndex `lhs`.
  */
  proc =(ref lhs: codepointIndex, rhs: int) {
    lhs._cpindex = rhs: int;
  }
  pragma "no doc"
  proc =(ref lhs: codepointIndex, const ref rhs: codepointIndex) {
    lhs._cpindex = rhs._cpindex;
  }


  /*
     Copies the string `rhs` into the string `lhs`.
  */
  proc =(ref lhs: string, rhs: string) {
    doAssign(lhs, rhs);
  }

  /*
     Copies the c_string `rhs_c` into the string `lhs`.

     Halts if `lhs` is a remote string.
  */
  proc =(ref lhs: string, rhs_c: c_string) {
    // I want to use try! but got tripped over by #14465
    try {
      lhs = createStringWithNewBuffer(rhs_c);
    }
    catch {
      halt("Assigning a c_string with non-UTF-8 data");
    }
  }

  //
  // Concatenation
  //
  /*
     :returns: A new string which is the result of concatenating `s0` and `s1`
  */
  proc +(s0: string, s1: string) {
    return doConcat(s0, s1);
  }

  /*
     :returns: A new string which is the result of repeating `s` `n` times.
               If `n` is less than or equal to 0, an empty string is returned.

     For example:

     .. code-block:: chapel

        writeln("Hello! " * 3);

     Results in::

       Hello! Hello! Hello!
  */
  proc *(s: string, n: integral) {
    return doMultiply(s, n);
  }

  //
  // Param procs
  //

  pragma "no doc"
  inline proc ==(param s0: string, param s1: string) param  {
    return __primitive("string_compare", s0, s1) == 0;
  }

  pragma "no doc"
  inline proc !=(param s0: string, param s1: string) param {
    return __primitive("string_compare", s0, s1) != 0;
  }

  pragma "no doc"
  inline proc <=(param a: string, param b: string) param {
    return (__primitive("string_compare", a, b) <= 0);
  }

  pragma "no doc"
  inline proc >=(param a: string, param b: string) param {
    return (__primitive("string_compare", a, b) >= 0);
  }

  pragma "no doc"
  inline proc <(param a: string, param b: string) param {
    return (__primitive("string_compare", a, b) < 0);
  }

  pragma "no doc"
  inline proc >(param a: string, param b: string) param {
    return (__primitive("string_compare", a, b) > 0);
  }

  pragma "no doc"
  inline proc +(param a: string, param b: string) param
    return __primitive("string_concat", a, b);

  pragma "no doc"
  inline proc param string.toByte() param : uint(8) {
    if this.numBytes != 1 then
      compilerError("string.toByte() only accepts single-byte strings");
    return __primitive("ascii", this);
  }

  pragma "no doc"
  inline proc param string.byte(param i: int) param : uint(8) {
    if i < 0 || i > this.numBytes-1 then
      compilerError("index " + i:string + " out of bounds for string with " + this.numBytes:string + " bytes");
    return __primitive("ascii", this, i);
  }

  pragma "no doc"
  inline proc param string.numBytes param
    return __primitive("string_length_bytes", this);

  pragma "no doc"
  inline proc param string.numCodepoints param
    return __primitive("string_length_codepoints", this);

  pragma "no doc"
  inline proc param string.length param {
    compilerWarning("'string.length' is deprecated - " +
                    "please use 'string.size' instead");
    return this.numCodepoints;
  }

  pragma "no doc"
  inline proc param string.size param
    return this.numCodepoints;

  pragma "no doc"
  inline proc _string_contains(param a: string, param b: string) param
    return __primitive("string_contains", a, b);


  //
  // Append
  //
  /*
     Appends the string `rhs` to the string `lhs`.
  */
  proc +=(ref lhs: string, const ref rhs: string) : void {
    doAppend(lhs, rhs);
  }

  //
  // Relational operators
  // TODO: all relational ops other than == and != are broken for unicode
  // TODO: It probably will be faster to work on a.locale or b.locale rather
  //       than here. Especially if a or b is large
  //
  // When strings are compared, they compare equal up to the point the
  // characters in the string differ.   At that point, if the encoding of the
  // next character is numerically smaller it will compare less, otherwise
  // greater.  If the encoding of one character uses fewer bits, the smaller
  // one is zero-extended to match the length of the longer one.
  // For purposes of comparison, if one compared is shorter than the
  // other, we fake in a NUL (character code 0x0).  Thus, for example:
  //  "1000" < "101" < "1010".
  //

  pragma "no doc"
  proc ==(a: string, b: string) : bool {
    // At the moment, this commented out section will not work correctly. If a
    // and b are on the same locale, we will go to that locale, but an autoCopy
    // will localize a and b, before they are placed into the on bundle,
    // causing us to access garbage data inside the doEq routine. Always
    // localize for now.
    //
    /* if a.locale_id == b.locale_id {
      var ret: bool = false;
      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(a.locale_id, c_sublocid_any)) {
        ret = doEq(a, b);
      }
      return ret;
    } else { */
    return doEq(a,b);
  }

  pragma "no doc"
  inline proc !=(a: string, b: string) : bool {
    return !doEq(a,b);
  }

  pragma "no doc"
  inline proc <(a: string, b: string) : bool {
    return doLessThan(a, b);
  }

  pragma "no doc"
  inline proc >(a: string, b: string) : bool {
    return doGreaterThan(a, b);
  }

  pragma "no doc"
  inline proc <=(a: string, b: string) : bool {
    return doLessThanOrEq(a, b);
  }
  pragma "no doc"
  inline proc >=(a: string, b: string) : bool {
    return doGreaterThanOrEq(a, b);
  }


  //
  // Helper routines
  //

  require "wctype.h";

  // Portability Note:
  // wint_t will normally be a 32-bit int.
  // There may be systems where it is not.
  pragma "no doc"
  extern type wint_t = int(32);

  private inline proc codepoint_isUpper(c: int(32)) : bool {
    pragma "fn synchronization free"
    extern proc iswupper(wc: wint_t): c_int;
    return iswupper(c: wint_t) != 0;
  }

  private inline proc codepoint_isLower(c: int(32)) : bool {
    pragma "fn synchronization free"
    extern proc iswlower(wc: wint_t): c_int;
    return iswlower(c: wint_t) != 0;
  }

  private inline proc codepoint_isAlpha(c: int(32)) : bool {
    pragma "fn synchronization free"
    extern proc iswalpha(wc: wint_t): c_int;
    return iswalpha(c: wint_t) != 0;
  }

  private inline proc codepoint_isDigit(c: int(32)) : bool {
    pragma "fn synchronization free"
    extern proc iswdigit(wc: wint_t): c_int;
    return iswdigit(c) != 0;
  }

  private inline proc codepoint_isWhitespace(c: int(32)) : bool {
    pragma "fn synchronization free"
    extern proc iswspace(wc: wint_t): c_int;
    return iswspace(c) != 0;
  }

  private inline proc codepoint_isPrintable(c: int(32)) : bool {
    pragma "fn synchronization free"
    extern proc iswprint(wc: wint_t): c_int;
    return iswprint(c) != 0;
  }

  private inline proc codepoint_toLower(c: int(32)) : int(32) {
    pragma "fn synchronization free"
    extern proc towlower(wc: wint_t): wint_t;
    return towlower(c: wint_t): int(32);
  }

  private inline proc codepoint_toUpper(c: int(32)) : int(32) {
    pragma "fn synchronization free"
    extern proc towupper(wc: wint_t): wint_t;
    return towupper(c: wint_t): int(32);
  }

  /*
     :returns: A string storing the complete multibyte character sequence
               that corresponds to the codepoint value `i`.
  */
  inline proc codepointToString(i: int(32)) {
    const mblength = qio_nbytes_char(i): int;
    var (buffer, mbsize) = bufferAlloc(mblength+1);
    qio_encode_char_buf(buffer, i);
    buffer[mblength] = 0;
    try! {
      return createStringWithOwnedBuffer(buffer, mblength, mbsize);
    }
  }

  //
  // Casts (casts to & from other primitive types are in StringCasts)
  //

  pragma "no doc"
  inline proc _cast(type t, cs: c_string) where t == bufferType {
    return __primitive("cast", t, cs);
  }

  // Cast from c_string to string
  pragma "no doc"
  proc _cast(type t, cs: c_string) where t == string {
    try {
      return createStringWithNewBuffer(cs);
    }
    catch {
      halt("Casting a non-UTF-8 c_string to strign");
    }
  }

  // Cast from byteIndex to int
  pragma "no doc"
  inline proc _cast(type t: int, cpi: byteIndex) {
    return cpi._bindex;
  }

  // Cast from int to byteIndex
  pragma "no doc"
  inline proc _cast(type t: byteIndex, i: int) {
    var cpi: byteIndex;
    cpi._bindex = i;
    return cpi;
  }

  // Cast from codepointIndex to int
  pragma "no doc"
  inline proc _cast(type t: int, cpi: codepointIndex) {
    return cpi._cpindex;
  }

  // Cast from int to codepointIndex
  pragma "no doc"
  inline proc _cast(type t: codepointIndex, i: int) {
    var cpi: codepointIndex;
    cpi._cpindex = i;
    return cpi;
  }

  //
  // hashing support
  //

  pragma "no doc"
  inline proc chpl__defaultHash(x : string): uint {
    return getHash(x);
  }
}
