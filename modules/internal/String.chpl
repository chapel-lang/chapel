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

/*
.. type:: string

supports the following methods:
 */
module String {
  use ChapelStandard;
  use CTypes;
  use ByteBufferHelpers;
  use BytesStringCommon;
  use ChplConfig only compiledForSingleLocale;
  import OS.{errorCode};

  use CString;
  public use StringCasts;
  public use BytesStringCommon only encodePolicy;  // expose encodePolicy

  use NVStringFactory;

  pragma "fn synchronization free"
  private extern proc qio_decode_char_buf(ref chr:int(32),
                                          ref nbytes:c_int,
                                          buf:c_ptrConst(c_char),
                                          buflen:c_ssize_t): errorCode;
  pragma "fn synchronization free"
  private extern proc qio_decode_char_buf_esc(ref chr:int(32),
                                              ref nbytes:c_int,
                                              buf:c_ptrConst(c_char),
                                              buflen:c_ssize_t): errorCode;
  pragma "fn synchronization free"
  private extern proc qio_encode_char_buf(dst:c_ptr(void), chr:int(32)):errorCode;
  pragma "fn synchronization free"
  private extern proc qio_nbytes_char(chr:int(32)):c_int;

  private config param debugStrings = false;

  @chpldoc.nodoc
  config param useCachedNumCodepoints = true;

  pragma "plain old data"
  @chpldoc.nodoc
  record byteIndex : writeSerializable {
    @chpldoc.nodoc
    var _bindex  : int;

    proc init() {
      // Let compiler insert defaults
    }
    proc init(i: int) { _bindex = i; }
    proc init=(other: byteIndex) { _bindex = other._bindex; }
    proc init=(i: int) { _bindex = i; }

    proc serialize(writer, ref serializer) throws {
      writer.write(_bindex);
    }

    operator :(val: byteIndex, type t:string) {
      return val._bindex: string;
    }
  }

  pragma "plain old data"
  @chpldoc.nodoc
  record codepointIndex : writeSerializable {
    @chpldoc.nodoc
    var _cpindex  : int;

    @chpldoc.nodoc
    proc init() {
      // Let compiler insert defaults
    }
    proc init(i: int) { _cpindex = i; }
    proc init=(i: int) { _cpindex = i; }
    proc init=(cpi: codepointIndex) { _cpindex = cpi._cpindex; }

    proc serialize(writer, ref serializer) throws {
      writer.write(_cpindex);
    }

    operator :(val: codepointIndex, type t:string) {
      return val._cpindex: string;
    }
}

  // Helper routines in support of being able to use ranges of indices
  proc chpl_build_bounded_range(low: byteIndex, high: byteIndex) do
    return new range(byteIndex, low=low, high=high);
  proc chpl_build_bounded_range(low: codepointIndex, high: codepointIndex) do
    return new range(codepointIndex, low=low, high=high);

  proc chpl_build_low_bounded_range(low: byteIndex) do
    return new range(low=low);
  proc chpl_build_low_bounded_range(low: codepointIndex) do
    return new range(low=low);

  proc chpl_build_high_bounded_range(high: byteIndex) do
    return new range(high=high);
  proc chpl_build_high_bounded_range(high: codepointIndex) do
    return new range(high=high);


  proc chpl__rangeStrideType(type idxType: byteIndex) type do
    return int;

  proc chpl__rangeStrideType(type idxType: codepointIndex) type do
    return int;

  proc chpl__rangeUnsignedType(type idxType: byteIndex) type do
    return uint;

  proc chpl__rangeUnsignedType(type idxType: codepointIndex) type do
    return uint;

  inline proc chpl__idxToInt(i: byteIndex) do
    return i:int;
  inline proc chpl__idxToInt(i: codepointIndex) do
    return i:int;

  inline proc chpl__intToIdx(type idxType: byteIndex, i: int) do
    return i: byteIndex;

  inline proc chpl__intToIdx(type idxType: codepointIndex, i: int) do
    return i: codepointIndex;

  @chpldoc.nodoc inline operator byteIndex.>(x: byteIndex, y: byteIndex) {
    return x: int > y: int;
  }
  @chpldoc.nodoc
  inline operator codepointIndex.>(x: codepointIndex, y: codepointIndex) {
    return x: int > y: int;
  }

  @chpldoc.nodoc inline operator byteIndex.>(x: byteIndex, y: int) {
    return x: int > y;
  }
  @chpldoc.nodoc
  inline operator codepointIndex.>(x: codepointIndex, y: int) do return x: int > y;

  @chpldoc.nodoc
  inline operator byteIndex.>(x: int, y: byteIndex) do return x > y: int;
  @chpldoc.nodoc
  inline operator codepointIndex.>(x: int, y: codepointIndex) do return x > y: int;
  // End range helper support

  // Index arithmetic support

  // index + int or int + index --> index
  @chpldoc.nodoc
  inline operator byteIndex.+(x: byteIndex, y: int) {
    return (x: int + y): byteIndex;
  }

  @chpldoc.nodoc inline operator codepointIndex.+(x: codepointIndex, y: int) {
    return (x: int + y): codepointIndex;
  }

  @chpldoc.nodoc inline operator byteIndex.+(x: int, y: byteIndex) {
    return (x + y: int): byteIndex;
  }

  @chpldoc.nodoc inline operator codepointIndex.+(x: int, y: codepointIndex) {
    return (x + y: int): codepointIndex;
  }

  @chpldoc.nodoc inline operator +(x: bufferType, y: byteIndex) do return x+(y:int);

  // index - int --> index
  @chpldoc.nodoc inline operator byteIndex.-(x: byteIndex, y: int) {
    return (x: int - y): byteIndex;
  }

  @chpldoc.nodoc inline operator codepointIndex.-(x: codepointIndex, y: int) {
    return (x: int - y): codepointIndex;
  }

  // index - index --> int
  @chpldoc.nodoc inline operator byteIndex.-(x: byteIndex, y: byteIndex) {
    return x: int - y: int;
  }
  @chpldoc.nodoc
  inline operator codepointIndex.-(x: codepointIndex, y: codepointIndex) {
    return x: int - y: int;
  }

  // other relationals
  @chpldoc.nodoc inline operator byteIndex.<(x: byteIndex, y: byteIndex) {
    return x: int < y: int;
  }
  @chpldoc.nodoc
  inline operator codepointIndex.<(x: codepointIndex, y: codepointIndex) {
    return x: int < y: int;
  }

  @chpldoc.nodoc inline operator byteIndex.<(x: byteIndex, y: int) {
    return x: int < y;
  }

  @chpldoc.nodoc inline operator codepointIndex.<(x: codepointIndex, y: int) {
    return x: int < y;
  }

  @chpldoc.nodoc inline operator byteIndex.<(x: int, y: byteIndex) {
    return x < y: int;
  }

  @chpldoc.nodoc inline operator codepointIndex.<(x: int, y: codepointIndex) {
    return x < y: int;
  }

  @chpldoc.nodoc inline operator byteIndex.>=(x: byteIndex, y: byteIndex) {
    return x: int >= y: int;
  }
  @chpldoc.nodoc
  inline operator codepointIndex.>=(x: codepointIndex, y: codepointIndex) {
    return x: int >= y: int;
  }

  @chpldoc.nodoc inline operator byteIndex.>=(x: byteIndex, y: int) {
    return x: int >= y;
  }

  @chpldoc.nodoc inline operator codepointIndex.>=(x: codepointIndex, y: int) {
    return x: int >= y;
  }

  @chpldoc.nodoc inline operator byteIndex.>=(x: int, y: byteIndex) {
    return x >= y: int;
  }

  @chpldoc.nodoc inline operator codepointIndex.>=(x: int, y: codepointIndex) {
    return x >= y: int;
  }

  @chpldoc.nodoc inline operator byteIndex.<=(x: byteIndex, y: byteIndex) {
    return x: int <= y: int;
  }
  @chpldoc.nodoc
  inline operator codepointIndex.<=(x: codepointIndex, y: codepointIndex) {
    return x: int <= y: int;
  }

  @chpldoc.nodoc inline operator byteIndex.<=(x: byteIndex, y: int) {
    return x: int <= y;
  }

  @chpldoc.nodoc inline operator codepointIndex.<=(x: codepointIndex, y: int) {
    return x: int <= y;
  }

  @chpldoc.nodoc inline operator byteIndex.<=(x: int, y: byteIndex) {
    return x <= y: int;
  }

  @chpldoc.nodoc inline operator codepointIndex.<=(x: int, y: codepointIndex) {
    return x <= y: int;
  }

  @chpldoc.nodoc inline operator byteIndex.==(x: byteIndex, y: byteIndex) {
    return (x:int) == (y:int);
  }
  @chpldoc.nodoc
  inline operator codepointIndex.==(x: codepointIndex, y: codepointIndex) {
    return (x:int) == (y:int);
  }

  @chpldoc.nodoc inline operator byteIndex.==(x: byteIndex, y: int) {
    return (x:int) == y;
  }

  @chpldoc.nodoc inline operator codepointIndex.==(x: codepointIndex, y: int) {
    return (x:int) == y;
  }

  @chpldoc.nodoc inline operator byteIndex.==(x: int, y: byteIndex) {
    return x == (y:int);
  }

  @chpldoc.nodoc inline operator codepointIndex.==(x: int, y: codepointIndex) {
    return x == (y:int);
  }

  @chpldoc.nodoc inline operator byteIndex.!=(x: byteIndex, y: byteIndex) {
    return (x:int) != (y:int);
  }
  @chpldoc.nodoc
  inline operator codepointIndex.!=(x: codepointIndex, y: codepointIndex) {
    return (x:int) != (y:int);
  }

  @chpldoc.nodoc inline operator byteIndex.!=(x: byteIndex, y: int) {
    return (x:int) != y;
  }

  @chpldoc.nodoc inline operator codepointIndex.!=(x: codepointIndex, y: int) {
    return (x:int) != y;
  }

  @chpldoc.nodoc inline operator byteIndex.!=(x: int, y: byteIndex) {
    return x != (y:int);
  }

  @chpldoc.nodoc inline operator codepointIndex.!=(x: int, y: codepointIndex) {
    return x != (y:int);
  }

  @chpldoc.nodoc inline operator byteIndex.!(x: byteIndex) do return !(x:int);
  @chpldoc.nodoc inline operator codepointIndex.!(x: codepointIndex) {
    return !(x:int);
  }

  @chpldoc.nodoc inline proc _cond_test(x: byteIndex) do      return x != 0;
  @chpldoc.nodoc inline proc _cond_test(x: codepointIndex) do return x != 0;
  // End index arithmetic support

  private proc validateEncoding(buf, len): int throws {
    extern proc chpl_enc_validate_buf(buf, len, ref numCodepoints) : c_int;

    var numCodepoints: int;

    if chpl_enc_validate_buf(buf, len, numCodepoints) != 0 {
      throw new DecodeError();
    }

    return numCodepoints;
  }

  //
  // createString* functions
  //

  /*
    Creates a new :type:`string` which borrows the internal buffer of another string. If
    the buffer is freed before the string returned from this function, accessing
    it is undefined behavior.

    :arg x: Object to borrow the buffer from
    :type x: `string`

    :returns: A new :type:`string`
  */
  @unstable("'createBorrowingBuffer' is unstable and may change in the future")
  inline proc type string.createBorrowingBuffer(x: string) : string {
    // we don't validate here because `x` must have been validated already
    var ret: string;
    ret.cachedNumCodepoints = x.cachedNumCodepoints;
    initWithBorrowedBuffer(ret, x);
    return ret;
  }

  /*
    Creates a new :type:`string` which borrows the memory allocated for a :class:`~CTypes.c_ptr`. If
    the buffer is freed before the :type:`string` returned from this function, accessing
    it is undefined behavior.

    :arg x: The buffer to borrow from
    :type x: `c_ptr(uint(8))` or `c_ptr(int(8))`

    :arg length: Length of the string stored in `x` in bytes, excluding the optional
                 terminating null byte.
    :type length: `int`

    :throws: A :class:`~Errors.DecodeError`: if `x` contains non-UTF-8
     characters.

    :returns: A new :type:`string`
  */
  @unstable("'createBorrowingBuffer' is unstable and may change in the future")
  inline proc type string.createBorrowingBuffer(x: c_ptr(?t),
                                                length=strLen(x)) : string throws {
    return string.createBorrowingBuffer(x:bufferType,
                                        length=length,
                                        size=length+1);
  }

  /*
    Creates a new :type:`string` which borrows the memory allocated for a :class:`~CTypes.c_ptrConst`. If
    the buffer is freed before the :type:`string` returned from this function, accessing
    it is undefined behavior.

    :arg x: The buffer to borrow from
    :type x: `c_ptrConst(uint(8))` or `c_ptrConst(int(8))`

    :arg length: Length of the string stored in `x` in bytes, excluding the optional
                 terminating null byte.
    :type length: `int`

    :throws: A :class:`~Errors.DecodeError`: if `x` contains non-UTF-8
     characters.

    :returns: A new :type:`string`
  */
  @unstable("'createBorrowingBuffer' is unstable and may change in the future")
  inline proc type string.createBorrowingBuffer(x: c_ptrConst(?t),
                                                length=strLen(x)) : string throws {
    return string.createBorrowingBuffer(x:bufferType,
                                        length=length,
                                        size=length+1);
  }

  proc chpl_createStringWithLiteral(buffer: chpl_c_string,
                                    offset: int,
                                    x: chpl_c_string,
                                    length: int,
                                    numCodepoints: int) : string {
    // copy the string to the combined buffer
    var buf = buffer:c_ptr(void):c_ptr(uint(8));
    buf = buf + offset;
    import OS.POSIX.memcpy;
    memcpy(buf:c_ptr(void), x:c_ptr(void), length.safeCast(c_size_t));
    memcpy(buf:c_ptr(void), x:c_ptr(void), length.safeCast(c_size_t));
    // add null byte
    buf[length] = 0;

    // NOTE: This is a "wellknown" function used by the compiler to create
    // string literals. Inlining this creates some bloat in the AST, slowing the
    // compilation.
    return chpl_createStringWithBorrowedBufferNV(buf,
                                                 length=length,
                                                 size=length+1,
                                                 numCodepoints=numCodepoints);
  }

  /*
     Creates a new :type:`string` which borrows the memory allocated for a :class:`~CTypes.c_ptr`. If
     the buffer is freed before the :type:`string` returned from this function,
     accessing it is undefined behavior.

     :arg x: The buffer to borrow from
     :type x: `c_ptr(uint(8))` or `c_ptr(int(8))`

     :arg length: Length of the string stored in `x` in bytes, excluding the
                  optional terminating null byte.
     :type length: `int`

     :arg size: Size of memory allocated for `x` in bytes
     :type length: `int`

     :throws: A :class:`~Errors.DecodeError`: if `x` contains non-UTF-8
      characters.

     :returns: A new :type:`string`
  */
  @unstable("'createBorrowingBuffer' is unstable and may change in the future")
  proc type string.createBorrowingBuffer(x: c_ptr(?t),
                                         length: int,
                                         size: int) : string throws {
    if t != uint(8) && t != int(8) {
      compilerError("Cannot create a string with a buffer of ", t:string);
    }
    var ret: string;
    ret.cachedNumCodepoints = validateEncoding(x:bufferType, length);
    initWithBorrowedBuffer(ret, x:bufferType, length, size);
    return ret;
  }

  @chpldoc.nodoc
  @deprecated("the type 'c_string' is deprecated; please use the variant of 'string.createBorrowingBuffer' that takes a 'c_ptrConst(c_char)' instead")
  inline proc type string.createBorrowingBuffer(x: chpl_c_string,
                                                length=x.size) : string throws {
    return string.createBorrowingBuffer(x:bufferType,
                                        length=length,
                                        size=length+1);
  }

  /*
    Creates a new :type:`string` which takes ownership of the memory allocated for a
    :class:`~CTypes.c_ptr`. The buffer will be freed when the :type:`string` is deinitialized.

    :arg x: The buffer to take ownership of
    :type x: `c_ptr(uint(8))` or `c_ptr(int(8))`

    :arg length: Length of the string stored in `x` in bytes, excluding the optional
                 terminating null byte.
    :type length: `int`

    :throws: A :class:`~Errors.DecodeError`: if `x` contains non-UTF-8
     characters. In that event, this function does not free `x`; that is
     the caller's responsibility.

    :returns: A new :type:`string`
  */
  proc type string.createAdoptingBuffer(x: c_ptr(?t),
                                        length=strLen(x)) : string throws {
    return string.createAdoptingBuffer(x:bufferType,
                                       length=length,
                                       size=length+1);
  }

  @chpldoc.nodoc
  @deprecated("the type 'c_string' is deprecated; please use the variant of 'string.createAdoptingBuffer' that takes a 'c_ptrConst(c_char)' instead")
  inline proc type string.createAdoptingBuffer(x: chpl_c_string,
                                               length=x.size) : string throws {
    return string.createAdoptingBuffer(x:bufferType,
                                       length=length,
                                       size=length+1);
  }

  /*
    Creates a new :type:`string` which takes ownership of the memory allocated for a
    :class:`~CTypes.c_ptrConst`. The buffer will be freed when the :type:`string`
    is deinitialized.

    :arg x: The buffer to take ownership of
    :type x: `c_ptrConst(uint(8))` or `c_ptrConst(int(8))`

    :arg length: Length of the string stored in `x` in bytes, excluding the optional
                 terminating null byte.
    :type length: `int`

    :throws: A :class:`~Errors.DecodeError`: if `x` contains non-UTF-8
     characters. In that event, this function does not free `x`; that is the
     caller's responsibility.

    :returns: A new :type:`string`
  */
  proc type string.createAdoptingBuffer(x: c_ptrConst(?t),
                                        length=strLen(x)) : string throws {
    return string.createAdoptingBuffer(x:bufferType,
                                       length=length,
                                       size=length+1);
  }

  /*
     Creates a new :type:`string` which takes ownership of the memory allocated for a
     :class:`~CTypes.c_ptr`. The buffer will be freed when the :type:`string` is deinitialized.

     :arg x: The buffer to take ownership of
     :type x: `c_ptr(uint(8))` or `c_ptr(int(8))`

     :arg length: Length of the string stored in `x` in bytes, excluding the
                  optional terminating null byte.
     :type length: `int`

     :arg size: Size of memory allocated for `x` in bytes
     :type length: `int`

     :throws: A :class:`~Errors.DecodeError`: if `x` contains non-UTF-8
      characters. In that event, this function does not free `x`; that is the
      caller's responsibility.

     :returns: A new :type:`string`
  */
  inline proc type string.createAdoptingBuffer(x: c_ptr(?t),
                                               length: int,
                                               size: int) : string throws {
    if t != uint(8) && t != int(8) {
      compilerError("Cannot create a string with a buffer of ", t:string);
    }
    var ret: string;
    ret.cachedNumCodepoints = validateEncoding(x:bufferType, length);
    initWithOwnedBuffer(ret, x:bufferType, length, size);
    return ret;
  }

  /*
    Creates a new :type:`string` by creating a copy of the memory allocated for a :class:`~CTypes.c_ptrConst`.

    :arg x: The buffer to copy
    :type x: `c_ptrConst(uint(8))` or `c_ptrConst(int(8))`

    :arg length: Length of `x` in bytes, excluding the optional terminating null byte.
    :type length: `int`

    :arg policy: - `decodePolicy.strict` raises an error
                 - `decodePolicy.replace` replaces the malformed character with
                   UTF-8 replacement character
                 - `decodePolicy.drop` drops the data silently
                 - `decodePolicy.escape` escapes each illegal byte with private
                   use codepoints

    :throws: A :class:`~Errors.DecodeError`: if `decodePolicy.strict` is
      passed to the `policy` argument and `x` contains non-UTF-8 characters.

    :returns: A new :type:`string`
  */
  inline proc type string.createCopyingBuffer(x: c_ptrConst(?t),
                                              length=strLen(x),
                                              policy=decodePolicy.strict
                                              ) : string throws {
    return string.createCopyingBuffer(x: bufferType,
                                      length=length,
                                      size=length+1,
                                      policy);
  }

  /*
     Creates a new :type:`string` by creating a copy of a buffer.

     :arg x: The buffer to copy
     :type x: `c_ptr(uint(8))` or `c_ptr(int(8))`

     :arg length: Length of the string stored in `x` in bytes, excluding the
                  optional terminating null byte.
     :type length: `int`

     :arg size: Size of memory allocated for `x` in bytes. This argument is
                ignored by this function.
     :type size: `int`

     :arg policy: `decodePolicy.strict` raises an error, `decodePolicy.replace`
                   replaces the malformed character with UTF-8 replacement
                   character, `decodePolicy.drop` drops the data silently,
                   `decodePolicy.escape` escapes each illegal byte with private
                   use codepoints

     :throws: A :class:`~Errors.DecodeError`: if `decodePolicy.strict` is
      passed to the `policy` argument and `x` contains non-UTF-8 characters.

     :returns: A new :type:`string`
  */
  proc type string.createCopyingBuffer(x: c_ptr(?t),
                                       length=strLen(x),
                                       size=length+1,
                                       policy=decodePolicy.strict) : string throws {
    if t != uint(8) && t != int(8) {
      compilerError("Cannot create a string with a buffer of ", t:string);
    }
    // size argument is not used, because we're allocating our own buffer
    // anyways. But it has a default and probably it's good to keep it here for
    // interface consistency
    return decodeByteBuffer(x:bufferType, length, policy);
  }

  @chpldoc.nodoc
  @deprecated("the type 'c_string' is deprecated; please use the variant of 'string.createCopyingBuffer' that takes a 'c_ptrConst(c_char)' instead")
  inline proc type string.createCopyingBuffer(x: chpl_c_string,
                                              length=x.size,
                                              policy=decodePolicy.strict
                                              ) : string throws {
    return string.createCopyingBuffer(x:bufferType,
                                      length=length,
                                      size=length+1,
                                      policy);
  }

  // non-validating string factory functions are in this submodule. This
  // submodule can be `private use`d from other String-supporting modules.
  @chpldoc.nodoc
  module NVStringFactory {
    use ChapelStandard; // For '=' operators between ints
    use BytesStringCommon;
    use ByteBufferHelpers only bufferType;

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

      // 2024/02/15 Lydia NOTE: This avoids a valgrind warning when performing
      // checks about arguments passed by default/const intent when they could
      // be implicitly modified.  The string type has boolean fields, which C
      // inserts padding for but we can't actually initialize the padding
      // without a memset
      if (chpl_warnUnstable && chpl_constArgChecking) {
        var origIsOwned = ret.isOwned;
        var origLocaleID = ret.locale_id;

        __primitive("zero variable", ret);
        ret.isOwned = origIsOwned;
        ret.locale_id = origLocaleID;
      }

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
  @chpldoc.nodoc
  record _string : writeSerializable {
    var buffLen: int = 0; // length of string in bytes
    var buffSize: int = 0; // size of the buffer we own
    var cachedNumCodepoints: int = 0;
    var buff: bufferType = nil;
    var isOwned: bool = true;
    var hasEscapes: bool = false;
    // We use chpl_nodeID as a shortcut to get at here.id without actually constructing
    // a locale object. Used when determining if we should make a remote transfer.
    var locale_id = chpl_nodeID; // : chpl_nodeID_t
    // remember to update <=> if you add a field

    proc init() {
      // Let compiler insert defaults
    }

    proc init=(s: string) {
      init this;
      initWithNewBuffer(this, s);
    }

    proc ref deinit() {
      // Checking for size here isn't sufficient. A string may have been
      // initialized from a c_string allocated from memory but beginning with
      // a null-terminator.
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

    inline proc isASCII() {
      return this.numCodepoints==this.numBytes;
    }

    inline proc byteIndices do return 0..<this.numBytes;

    // TODO: Support param c_ptr/c_ptrConst
    // inline proc param c_str() param : c_ptrConst(c_char) {
    //   return this:c_ptrConst(c_char); // folded out in resolution
    // }


    // assumes that 'this' is already localized
    proc _cpIndexLenHelpNoAdjustment(ref start: int) {
      if boundsChecking {
        if !compiledForSingleLocale() && this.locale_id != chpl_nodeID {
          halt("internal error -- method requires localized string");
        }
      }

      const i = start;

      if this.isASCII() {
        start += 1;
        return (this.buff[i]:int(32), i:byteIndex, 1:int);

      }
      else {
        const (decodeRet, cp, nBytes) = decodeHelp(buff=this.buff,
                                                   buffLen=this.buffLen,
                                                   offset=i,
                                                   allowEsc=true);
        start += nBytes;
        return (cp:int(32), i:byteIndex, nBytes:int);
      }
    }

    /*
      Iterates over the string Unicode character by Unicode character,
      and includes the byte index and byte length of each character.
      Skip characters that begin prior to the specified starting byte index.
      Assume we may accidentally start in the middle of a multibyte character,
      but the string is correctly encoded UTF-8.
    */
    iter _cpIndexLen(start = 0:byteIndex) {
      const localThis = this.localize();
      var i = _findStartOfNextCodepointFromByte(localThis, start);
      while i < localThis.buffLen {
        yield localThis._cpIndexLenHelpNoAdjustment(i);  // this increments i
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
    proc serialize(writer, ref serializer) throws {
      compilerError("not implemented: serialize");
    }

    // assumes that 'this' is already local
    proc doSplitWSUTF8Help(maxsplit: int, ref i: int,
                           const splitCount: int, const noSplits: bool,
                           const limitSplits: bool, const iEnd: byteIndex) {
      if boundsChecking {
        if !compiledForSingleLocale() && this.locale_id != chpl_nodeID {
          halt("internal error -- method requires localized string");
        }
      }

      // note: to improve performance, this code collapses several cases into a
      //       single yield statement, which makes it confusing to read
      var done : bool = false;
      var yieldChunk : bool = false;
      var chunk : string;

      var inChunk : bool = false;
      var chunkStart : byteIndex;

      i = _findStartOfNextCodepointFromByte(this, i:byteIndex);

      while i < this.buffLen {
        const (decodeRet, c, nBytes) = decodeHelp(buff=this.buff,
                                                  buffLen=this.buffLen,
                                                  offset=i,
                                                  allowEsc=true);
        // emit whole string, unless all whitespace
        if noSplits {
          done = true;
          if !this.isSpace() then {
            chunk = this;
            yieldChunk = true;
          }
        } else {
          var cSpace = codepoint_isWhitespace(c);
          // first char of a chunk
          if !(inChunk || cSpace) {
            chunkStart = i;
            inChunk = true;
            if i - 1 + nBytes > iEnd {
              chunk = try! this[chunkStart..];
              yieldChunk = true;
              done = true;
            }
          } else if inChunk {
            // first char out of a chunk
            if cSpace {
              // last split under limit
              if limitSplits && splitCount >= maxsplit {
                chunk = try! this[chunkStart..];
                yieldChunk = true;
                done = true;
              // no limit
              } else {
                chunk = try! this[chunkStart..(i-1):byteIndex];
                yieldChunk = true;
                inChunk = false;
              }
            // out of chars
            } else if i - 1 + nBytes > iEnd {
              chunk = try! this[chunkStart..];
              yieldChunk = true;
              done = true;
            }
          }
        }

        if done {
          i = this.buffLen;
        }
        else {
          i += nBytes;
        }
        if yieldChunk {
          return chunk;
        }
      }
      return "";
    }

    iter doSplitWSUTF8(maxsplit: int) {
      if !this.isEmpty() {
        const localThis = this.localize();
        var splitCount = 0;

        // this is passed to the helper as ref
        var i = 0;

        while i < localThis.buffLen {
          const chunk =  localThis.doSplitWSUTF8Help(
                                  maxsplit, i, splitCount,
                                  noSplits=(maxsplit==0),
                                  limitSplits=(maxsplit>0),
                                  iEnd=(localThis.buffLen-2):byteIndex);
          if !chunk.isEmpty() {
            yield chunk;
            splitCount += 1;
          }
        }
      }
    }

    // Helper function that uses a param bool to toggle between count and find
    //TODO: this could be a much better string search
    //      (Boyer-Moore-Horspool|any thing other than brute force)
    //
    proc doSearchUTF8(pattern: string, indices: range(?),
                      param count: bool, param fromLeft: bool = true) {
      // pattern.len is <= than this.buffLen, so go to the home locale
      var ret: int = -1;
      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
        // any value >= 0 means we have a solution
        // used because we cant break out of an on-clause early
        var localRet: int = -2;
        const nLen = pattern.buffLen;
        const (view, _) = try! getView(this, indices, checkMisaligned=false);
        const thisLen = view.size;

        // Edge cases
        if count {
          if nLen == 0 { // Empty pattern
            if ((indices.hasLowBound() && indices.low.type == byteIndex) ||
                (indices.hasHighBound() && indices.high.type == byteIndex)) {
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
          if nLen == 0 { // Empty pattern
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
          const localPattern: string = pattern.localize();

          // i *is not* an index into anything, it is the order of the element
          // of view we are searching from.
          const numPossible = thisLen - nLen + 1;
          const searchSpace = if fromLeft
              then 0..#(numPossible)
              else 0..#(numPossible) by -1;
          //          writeln("view is ", view);
          for i in searchSpace {
            // j *is* the index into the localPattern's buffer
            for j in 0..#nLen {
              const idx = view.orderToIndex(i+j); // 0s based idx
              if this.buff[idx] != localPattern.buff[j] then break;

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

    inline proc join(const ref x : [] string) : string {
      return doJoin(this, x);
    }

    inline proc join(const ref x) where isTuple(x) {
      if !isHomogeneousTuple(x) || !isString(x[0]) then
        compilerError("join() on tuples only handles homogeneous tuples of strings");
      return doJoin(this, x);
    }

    inline proc join(ir: _iteratorRecord): string {
      return doJoinIterator(this, ir);
    }

    /*
      :returns: A new :type:`string` with the first character in uppercase (if it is a
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

  /*
    :returns: The number of codepoints in the :type:`string`.
  */
  inline proc const string.size : int do return numCodepoints;

  /*
    :returns: The indices that can be used to index into the :type:`string`
              (i.e., the range ``0..<this.size``)
  */
  inline proc string.indices : range do return 0..<size;

  /*
    :returns: The number of bytes in the :type:`string`.
  */
  inline proc string.numBytes : int do return buffLen;

  /*
    :returns: The number of codepoints in the :type:`string`, assuming the
              string is correctly-encoded UTF-8.
  */
  inline proc const string.numCodepoints : int {
    if useCachedNumCodepoints {
      return this.cachedNumCodepoints;
    }
    else {
      return countNumCodepoints(this);
    }
  }

  /*
     Gets a version of the :type:`string` that is on the currently
     executing locale.

     :returns: A shallow copy if the :type:`string` is already on the
               current locale, otherwise a deep copy is performed.
  */
  @unstable("string.localize() is unstable and may change in a future release")
  inline proc string.localize() : string {
    if compiledForSingleLocale() || this.locale_id == chpl_nodeID {
      return string.createBorrowingBuffer(this);
    } else {
      const x:string = this; // assignment makes it local
      return x;
    }
  }

  /*
    Get a `c_ptrConst(c_char)` from a :type:`string`. The returned
    :class:`~CTypes.c_ptrConst` shares the buffer with the :type:`string`.

    .. warning::

        This can only be called safely on a :type:`string` whose home is
        the current locale.  This property can be enforced by calling
        :proc:`string.localize()` before :proc:`string.c_str()`. If the
        string is remote, the program will halt.

    For example:

    .. code-block:: chapel

      var my_string = "Hello!";
      on different_locale {
        printf("%s", my_string.localize().c_str());
      }

    :returns:
        A `c_ptrConst(c_char)` that points to the underlying buffer used by this
        :type:`string`. The returned `c_ptrConst(c_char)` is only valid when used
        on the same locale as the string.
   */
  pragma "last resort"
  @deprecated("'string.c_str()' has moved to 'CTypes'. Please 'use CTypes' to access ':proc:`~CTypes.string.c_str`'")
  inline proc string.c_str() : c_ptrConst(c_char) {
    use CTypes only c_str;
    return this.c_str();
  }

  /*
    Returns a :type:`~Bytes.bytes` from the given :type:`string`. If the
    string contains some escaped non-UTF8 bytes, `policy` argument determines
    the action.

    :arg policy: `encodePolicy.pass` directly copies the (potentially escaped)
                  data, `encodePolicy.unescape` recovers the escaped bytes
                  back.

    :returns: :type:`~Bytes.bytes`
  */
  proc string.encode(policy=encodePolicy.pass) : bytes {
    var localThis: string = this.localize();

    if policy == encodePolicy.pass || this.isASCII() {  // just copy
      return bytes.createCopyingBuffer(localThis.buff, localThis.numBytes);
    }
    else {  // see if there is escaped data in the string
      var (buff, size) = bufferAlloc(localThis.buffLen+1);

      var readIdx = 0;
      var writeIdx = 0;
      while readIdx < localThis.buffLen {
        var multibytes = localThis.buff + readIdx;
        const (decodeRet, cp, nBytes) = decodeHelp(buff=localThis.buff,
                                                   buffLen=localThis.buffLen,
                                                   offset=readIdx,
                                                   allowEsc=true);
        if (0xdc80<=cp && cp<=0xdcff) {
          buff[writeIdx] = (cp-0xdc00):uint(8);
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
      return bytes.createAdoptingBuffer(buff, length=writeIdx, size=size);
    }
  }

  /*
    Iterates over the :type:`string` character by character.

    For example:

    .. code-block:: chapel

      var str = "abcd";
      for c in str.items() {
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

    if localThis.isASCII() {
      for i in localThis.byteIndices {
        var (newBuff, allocSize) = bufferCopyLocal(localThis.buff+i, len=1);
        yield chpl_createStringWithOwnedBufferNV(newBuff, 1, allocSize, 1);
      }
    }
    else {
      var i = 0;
      while i < localThis.buffLen {
        const curPos = localThis.buff+i;
        const (decodeRet, cp, nBytes) = decodeHelp(buff=localThis.buff,
                                                   buffLen=localThis.buffLen,
                                                   offset=i,
                                                   allowEsc=true);
        var (newBuf, newSize) = bufferCopyLocal(curPos, nBytes);
        yield chpl_createStringWithOwnedBufferNV(newBuf, nBytes, newSize, 1);

        i += nBytes;
      }
    }
  }

  /*
    Iterates over the :type:`string` character by character, yielding 1-codepoint
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
    Iterates over the :type:`string` byte by byte.
  */
  pragma "chpldoc ignore chpl prefix"
  iter string.chpl_bytes() : uint(8) {
    var localThis: string = this.localize();

    foreach i in 0..#localThis.buffLen {
      yield localThis.buff[i];
    }
  }

  /*
    Iterates over the :type:`string` Unicode character by Unicode character.
  */
  iter string.codepoints() : int(32) {
    const localThis = this.localize();
    var i = 0;
    while i < localThis.buffLen {
      yield localThis._cpIndexLenHelpNoAdjustment(i)[0];  // this increments i
    }
  }

  /*
    :returns: The value of a single-byte :type:`string` as an integer.
  */
  proc string.toByte() : uint(8) {
    if this.buffLen != 1 then
      halt("string.toByte() only accepts single-byte strings");
    return bufferGetByte(buf=this.buff, off=0, loc=this.locale_id);
  }

  /*
    :returns: The value of the `i` th byte as an integer.
  */
  proc string.byte(i: int) : uint(8) {
    if boundsChecking && (i < 0 || i >= this.buffLen)
      then halt("index ", i, " out of bounds for string with ", this.numBytes, " bytes");
    return bufferGetByte(buf=this.buff, off=i, loc=this.locale_id);
  }

  /*
    :returns: The value of a single-codepoint :type:`string` as an integer.
   */
  proc string.toCodepoint() : int(32) {
    // TODO: Engin: at least we can check whether the length is less than 4
    // bytes before localizing?
    var localThis: string = this.localize();

    if localThis.isEmpty() then
      halt("string.toCodepoint() only accepts single-codepoint strings");

    if this.isASCII() {
      if localThis.numBytes > 1 then
        halt("string.toCodepoint() only accepts single-codepoint strings");
      return this.toByte();
    }
    else {

      const (decodeRet, cp, nBytes) = decodeHelp(buff=localThis.buff,
                                                 buffLen=localThis.buffLen,
                                                 offset=0,
                                                 allowEsc=true);
      if localThis.buffLen != nBytes:int then
        halt("string.toCodepoint() only accepts single-codepoint strings");

      return cp;
    }
  }

  /*
    :returns: The value of the `i` th multibyte character as an integer.
   */
  proc string.codepoint(i: int) : int(32) {
    // TODO: Engin we may need localize here
    const idx = i: int;
    if boundsChecking && idx < 0 then
      halt("index ", idx, " out of bounds for string");

    if this.isASCII() {
      if boundsChecking && idx >= this.numBytes then
        halt("index ", idx, " out of bounds for string with length ", this.size);

      return this.byte(i);
    }
    else {
      var j = 0;
      for cp in this.codepoints() {
        if j == idx then
          return cp;
        j += 1;
      }
      // We have reached the end of the :type:`string` without finding our index.
      if boundsChecking then
        halt("index ", idx, " out of bounds for string with length ", this.size);
      return 0: int(32);
    }
  }

  /*
    Return the codepoint starting at the `i` th byte in the :type:`string`

    :returns: A new :type:`string` with the complete multibyte character starting at the
              specified byte index from ``0..#string.numBytes``
   */
  proc string.this(i: byteIndex) : string {
    var idx = i: int;
    if boundsChecking && (idx < 0 || idx >= this.buffLen)
      then halt("index ", i:int, " out of bounds for string with ", this.buffLen, " bytes");

    if this.isASCII() {
      var (newBuff, allocSize) = bufferCopy(buf=this.buff, off=i:int,
                                            len=1, loc=this.locale_id);
      return chpl_createStringWithOwnedBufferNV(newBuff, 1, allocSize, 1);
    }
    else {
      var maxbytes = (this.buffLen - idx): c_ssize_t;
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
  }

  /*
    Return the `i` th codepoint in the :type:`string`. (A synonym for :proc:`string.item`)

    :returns: A new :type:`string` with the complete multibyte character starting at the
              specified codepoint index from ``0..#string.numCodepoints``
   */
  proc string.this(i: codepointIndex) : string {
    return this.item(i);
  }

  /*
    Return the `i` th codepoint in the :type:`string`. (A synonym for :proc:`string.item`)

    :returns: A new :type:`string` with the complete multibyte character starting at the
              specified codepoint index from ``1..string.numCodepoints``
   */
  inline proc string.this(i: int) : string {
    return this.item(i);
  }

  /*
    Return the `i` th codepoint in the :type:`string`

    :returns: A new :type:`string` with the complete multibyte character starting at the
              specified codepoint index from ``1..string.numCodepoints``
   */
  proc string.item(i: codepointIndex) : string {
    if boundsChecking && i < 0 then
      halt("index ", i:int, " out of bounds for string");

    if this.isEmpty() then return "";
    if this.isASCII() {
      if boundsChecking && i >= this.numBytes then
        halt("index ", i:int, " out of bounds for string with length ", this.size);
      var (newBuff, allocSize) = bufferCopy(buf=this.buff, off=i:int,
                                            len=1, loc=this.locale_id);
      return chpl_createStringWithOwnedBufferNV(newBuff, 1, allocSize, 1);
    }
    else {
      var charCount = 0;
      for (cp, byteIdx, nBytes) in _cpIndexLen() {
        if charCount == i {
          var (newBuff, allocSize) = bufferCopy(buf=this.buff, off=byteIdx:int,
                                                len=nBytes, loc=this.locale_id);
          return chpl_createStringWithOwnedBufferNV(newBuff, nBytes, allocSize, 1);
        }
        charCount += 1;
      }
      if boundsChecking then
        halt("index ", i:int, " out of bounds for string with length ", this.size);
      return "";
    }
  }

  /*
    Return the `i` th codepoint in the :type:`string`

    :returns: A new :type:`string` with the complete multibyte character starting at the
              specified codepoint index from ``0..#string.numCodepoints``
   */
  inline proc string.item(i: int) : string {
    return this[i: codepointIndex];
  }

  /*
    Slice a :type:`string`. Halts if r is non-empty and not completely inside the
    range ``0..<string.size`` when compiled with `--checks`. `--fast`
    disables this check.

    :arg r: range of the indices the new :type:`string` should be made from

    :throws: throws a :class:`~Errors.CodepointSplitError`: if slicing results
      in splitting a multi-byte codepoint.

    :returns: A new :type:`string` that is a substring within ``0..<string.size``. If
              the length of `r` is zero, an empty string is returned.
   */
  inline proc string.this(r: range(?)): string throws where r.idxType == byteIndex {
    return getSlice(this, r);
  }

  @chpldoc.nodoc
  inline proc string.this(r: range(?)): string where r.idxType != byteIndex {
    // codepoint-based slicing should never throw
    return try! getSlice(this, r);
  }

  /*
    :returns: * `true`  -- when the :type:`string` is empty
              * `false` -- otherwise
   */
  inline proc string.isEmpty() : bool {
    return this.buffLen == 0; // this should be enough of a check
  }

  /*
    :arg patterns: A varargs list of strings to match against.

    :returns: * `true`  -- when the :type:`string` begins with one or more of the `patterns`
              * `false` -- otherwise
   */
  inline proc string.startsWith(patterns: string ...) : bool {
    return startsEndsWith(this, patterns, fromLeft=true);
  }

  /*
    :arg patterns: A varargs list of strings to match against.

    :returns: * `true`  -- when the :type:`string` ends with one or more of the `patterns`
              * `false` -- otherwise
   */
  inline proc string.endsWith(patterns: string ...) : bool {
    return startsEndsWith(this, patterns, fromLeft=false);
  }

  /*
    :arg pattern: the :type:`string` to search for
    :arg indices: an optional range defining the substring to search within,
                 default is the whole string. Halts if the range is not
                 within ``0..<string.size``

    :returns: the index of the first occurrence of `pattern` within a
              :type:`string`, or -1 if the `pattern` is not in the string.
   */

  inline proc string.find(pattern: string,
                          indices: range(?) = this.byteIndices:range(byteIndex)) : byteIndex {
    if this.isASCII() then
      return doSearchNoEnc(this, pattern, indices, count=false): byteIndex;
    else
      return doSearchUTF8(pattern, indices, count=false): byteIndex;
  }

  /*
    :arg pattern: the :type:`string` to search for
    :arg indices: an optional range defining the substring to search within,
                 default is the whole string. Halts if the range is not
                 within ``0..<string.size``

    :returns: the index of the first occurrence from the right of `pattern`
              within a string, or -1 if the `pattern` is not in the string.
   */
  inline proc string.rfind(pattern: string,
                           indices: range(?) = this.byteIndices:range(byteIndex)) : byteIndex {
    if this.isASCII() then
      return doSearchNoEnc(this, pattern, indices,
                           count=false, fromLeft=false): byteIndex;
    else
      return doSearchUTF8(pattern, indices,
                          count=false, fromLeft=false): byteIndex;
  }

  /*
    :arg pattern: the :type:`string` to search for
    :arg indices: an optional range defining the substring to search within,
                 default is the whole string. Halts if the range is not
                 within ``0..<string.size``

    :returns: the number of times `pattern` occurs in the string
   */
  inline proc string.count(pattern: string,
                           indices: range(?) = this.indices) : int {
    if this.isASCII() then
      return doSearchNoEnc(this, pattern, indices, count=true);
    else
      return doSearchUTF8(pattern, indices, count=true);
  }

  /*
    :arg pattern: the :type:`string` to search for
    :arg replacement: the :type:`string` to replace `pattern` with
    :arg count: an optional integer specifying the number of replacements to
                make, values less than zero will replace all occurrences

    :returns: a copy of the :type:`string` where `replacement` replaces `pattern` up
              to `count` times
   */
  inline proc string.replace(pattern: string, replacement: string,
                             count: int = -1) : string {
    return doReplace(this, pattern, replacement, count);
  }

  /*
    Splits the :type:`string` on `sep` yielding the substring between each
    occurrence, up to `maxsplit` times.

    :arg sep: The delimiter used to break the string into chunks.
    :arg maxsplit: The number of times to split the string, negative values
                   indicate no limit.
    :arg ignoreEmpty: * When `true`  -- Empty strings will not be yielded,
                                        and will not count towards `maxsplit`
                      * When `false` -- Empty strings will be yielded when
                                        `sep` occurs multiple times in a row.
   */
  iter string.split(sep: string, maxsplit: int = -1,
                    ignoreEmpty: bool = false) : string {
    // TODO: specifying return type leads to un-inited string?
    for s in doSplit(this, sep, maxsplit, ignoreEmpty) do yield s;
  }

  /*
    Works as above, but uses runs of whitespace as the delimiter.

    :arg maxsplit: The number of times to split the :type:`string`, negative values
                   indicate no limit.
   */
  iter string.split(maxsplit: int = -1) : string {
    // TODO: specifying return type leads to un-inited string?
    if this.isASCII() {
      for s in doSplitWSNoEnc(this, maxsplit) do yield s;
    } else {
      for s in doSplitWSUTF8(maxsplit) do yield s;
    }
  }

  /*
    Returns a new :type:`string`, which is the concatenation of all of
    the :type:`string` passed in with the contents of the method
    receiver inserted between them.

    .. code-block:: chapel

        var myString = "|".join("a","10","d");
        writeln(myString); // prints: "a|10|d"

    :arg x: :type:`string` values to be joined

    :returns: A new :type:`string`
  */
  inline proc string.join(const ref x: string ...) : string {
    return doJoin(this, x);
  }

  /*
    Returns a new :type:`string`, which is the concatenation of all of
    the :type:`string` passed in with the contents of the method
    receiver inserted between them.

    .. code-block:: chapel

        var tup = ("a","10","d");
        var myJoinedTuple = "|".join(tup);
        writeln(myJoinedTuple); // prints: "a|10|d"

        var myJoinedArray = "|".join(["a","10","d"]);
        writeln(myJoinedArray); // prints: "a|10|d"

    :arg x: An array or tuple of :type:`string` values to be joined

    :returns: A new :type:`string`
  */
  inline proc string.join(const ref x) : string {
    // this overload serves as a catch-all for unsupported types.
    // for the implementation of array and tuple overloads, see
    // join() methods in the _string record.
    compilerError("string.join() accepts any number of strings, homogeneous "
                  + "tuple of strings, or array of strings as an argument");
  }


  /*
    :arg chars: A :type:`string` containing each character to remove.
                Defaults to `" \\t\\r\\n"`.
    :arg leading: Indicates if leading occurrences should be removed.
                  Defaults to `true`.
    :arg trailing: Indicates if trailing occurrences should be removed.
                    Defaults to `true`.

    :returns: A new :type:`string` with `leading` and/or `trailing` occurrences of
              characters in `chars` removed as appropriate.
  */
  proc string.strip(chars: string = " \t\r\n", leading=true,
                    trailing=true) : string {
    if this.isASCII() {
      return doStripNoEnc(this, chars, leading, trailing);
    } else {
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

      return try! localThis[start..end];
    }
  }

  /*
    Splits the string on `sep` into a `3*string` consisting of the section
    before `sep`, `sep`, and the section after `sep`. If `sep` is not found,
    the tuple will contain the whole string, and then two empty strings.
  */
  inline proc const string.partition(sep: string) : 3*string {
    return doPartition(this, sep);
  }


  /* Remove indentation from each line of a :type:`string`.

      This can be useful when applied to multi-line strings that are indented
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

      :returns: A new :type:`string` with indentation removed.
  */
  @unstable("string.dedent is subject to change in the future.")
  proc string.dedent(columns=0, ignoreFirst=true) : string {
    return doDedent(this, columns, ignoreFirst);
  }

  /*
    Checks if all the characters in the :type:`string` are either uppercase (A-Z) or
    uncased (not a letter).

    :returns: * `true`  -- if the string contains at least one uppercase character and no lowercase characters, ignoring uncased characters.
              * `false` -- otherwise
    */
  proc string.isUpper() : bool {
    if this.isEmpty() then return false;

    var result: bool;
    on __primitive("chpl_on_locale_num",
                    chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
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
    Checks if all the characters in the :type:`string` are either lowercase (a-z) or
    uncased (not a letter).

    :returns: * `true`  -- when there are no uppercase characters in the string.
              * `false` -- otherwise
    */
  proc string.isLower() : bool {
    if this.isEmpty() then return false;

    var result: bool;
    on __primitive("chpl_on_locale_num",
                    chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
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
    Checks if all the characters in the :type:`string` are whitespace ('  ', '\\t',
    '\\n', '\\v', '\\f', '\\r').

    :returns: * `true`  -- when all the characters are whitespace.
              * `false` -- otherwise
    */
  proc string.isSpace() : bool {
    if this.isEmpty() then return false;
    var result: bool = true;

    on __primitive("chpl_on_locale_num",
                    chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
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
    Checks if all the characters in the :type:`string` are alphabetic (a-zA-Z).

    :returns: * `true`  -- when the characters are alphabetic.
              * `false` -- otherwise
    */
  proc string.isAlpha() : bool {
    if this.isEmpty() then return false;
    var result: bool = true;

    on __primitive("chpl_on_locale_num",
                    chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
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
    Checks if all the characters in the :type:`string` are digits (0-9).

    :returns: * `true`  -- when the characters are digits.
              * `false` -- otherwise
    */
  proc string.isDigit() : bool {
    if this.isEmpty() then return false;
    var result: bool = true;

    on __primitive("chpl_on_locale_num",
                    chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
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
    Checks if all the characters in the :type:`string` are alphanumeric (a-zA-Z0-9).

    :returns: * `true`  -- when the characters are alphanumeric.
              * `false` -- otherwise
    */
  proc string.isAlnum() : bool {
    if this.isEmpty() then return false;
    var result: bool = true;

    on __primitive("chpl_on_locale_num",
                    chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
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
    Checks if all the characters in the :type:`string` are printable.

    :returns: * `true`  -- when the characters are printable.
              * `false` -- otherwise
    */
  proc string.isPrintable() : bool {
    if this.isEmpty() then return false;
    var result: bool = true;

    on __primitive("chpl_on_locale_num",
                    chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
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
                    chpl_buildLocaleID(this.locale_id, c_sublocid_none)) {
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
    :returns: A new :type:`string` with all uppercase characters replaced with their
              lowercase counterpart.

    .. note::

      The case change operation is not currently performed on characters whose
      cases take different number of bytes to represent in Unicode mapping.
  */
  proc string.toLower() : string {
    var result: string = this;
    if result.isEmpty() then return result;

    for (cp, i, nBytes) in this._cpIndexLen() {
      var lowCodepoint = codepoint_toLower(cp);
      if lowCodepoint != cp && qio_nbytes_char(lowCodepoint) == nBytes {
        // Use the MacOS approach everywhere:  only change the case if
        // the result does not change the number of encoded bytes.
        qio_encode_char_buf(result.buff + i, lowCodepoint);
      }
    }
    return result;
  }

  /*
    :returns: A new :type:`string` with all lowercase characters replaced with their
              uppercase counterpart.

    .. note::

      The case change operation is not currently performed on characters whose
      cases take different number of bytes to represent in Unicode mapping.
  */
  proc string.toUpper() : string {
    var result: string = this;
    if result.isEmpty() then return result;

    for (cp, i, nBytes) in this._cpIndexLen() {
      var upCodepoint = codepoint_toUpper(cp);
      if upCodepoint != cp && qio_nbytes_char(upCodepoint) == nBytes {
        // Use the MacOS approach everywhere:  only change the case if
        // the result does not change the number of encoded bytes.
        qio_encode_char_buf(result.buff + i, upCodepoint);
      }
    }
    return result;
  }

  /*
    :returns: A new :type:`string` with all cased characters following an uncased
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
    for (cp, i, nBytes) in this._cpIndexLen() {
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
    }
    return result;
  }

  //
  // Assignment functions
  //
  /*
     Copies the int `rhs` into the byteIndex `lhs`.
  */
  @chpldoc.nodoc
  operator byteIndex.=(ref lhs: byteIndex, rhs: int) {
    lhs._bindex = rhs: int;
  }
  @chpldoc.nodoc
  operator byteIndex.=(ref lhs: byteIndex, const ref rhs: byteIndex) {
    lhs._bindex = rhs._bindex;
  }

  /*
     Copies the int `rhs` into the codepointIndex `lhs`.
  */
  @chpldoc.nodoc
  operator codepointIndex.=(ref lhs: codepointIndex, rhs: int) {
    lhs._cpindex = rhs: int;
  }
  @chpldoc.nodoc
  operator codepointIndex.=(ref lhs: codepointIndex,
                            const ref rhs: codepointIndex) {
    lhs._cpindex = rhs._cpindex;
  }


  /*
     Copies the :type:`string` `rhs` into the :type:`string` `lhs`.
  */
  operator =(ref lhs: string, rhs: string) : void {
    doAssign(lhs, rhs);
  }

  //
  // Concatenation
  //
  /*
     :returns: A new :type:`string` which is the result of concatenating `s0` and `s1`
  */
  operator string.+(s0: string, s1: string) : string {
    return doConcat(s0, s1);
  }

  /*
     :returns: A new :type:`string` which is the result of repeating `s` `n` times.
               If `n` is less than or equal to 0, an empty :type:`string` is returned.

     The operation is commutative.
     For example:

     .. code-block:: chapel

        writeln("Hello! " * 3);
        or
        writeln(3 * "Hello! ");

     Results in::

       Hello! Hello! Hello!
  */
  operator *(s: string, n: integral) : string {
    return doMultiply(s, n);
  }

  @chpldoc.nodoc
  operator *(n: integral, s: string) {
    return doMultiply(s, n);
  }

  //
  // Param procs
  //

  @chpldoc.nodoc
  inline operator string.==(param s0: string, param s1: string) param  {
    return __primitive("string_compare", s0, s1) == 0;
  }

  @chpldoc.nodoc
  inline operator string.!=(param s0: string, param s1: string) param {
    return __primitive("string_compare", s0, s1) != 0;
  }

  @chpldoc.nodoc
  inline operator string.<=(param a: string, param b: string) param {
    return (__primitive("string_compare", a, b) <= 0);
  }

  @chpldoc.nodoc
  inline operator string.>=(param a: string, param b: string) param {
    return (__primitive("string_compare", a, b) >= 0);
  }

  @chpldoc.nodoc
  inline operator string.<(param a: string, param b: string) param {
    return (__primitive("string_compare", a, b) < 0);
  }

  @chpldoc.nodoc
  inline operator string.>(param a: string, param b: string) param {
    return (__primitive("string_compare", a, b) > 0);
  }

  @chpldoc.nodoc
  inline operator string.+(param a: string, param b: string) param do
    return __primitive("string_concat", a, b);

  @chpldoc.nodoc
  inline proc param string.toByte() param : uint(8) {
    if this.numBytes != 1 then
      compilerError("string.toByte() only accepts single-byte strings");
    return __primitive("ascii", this);
  }

  @chpldoc.nodoc
  inline proc param string.byte(param i: int) param : uint(8) {
    if i < 0 || i > this.numBytes-1 then
      compilerError("index " + i:string + " out of bounds for string with " + this.numBytes:string + " bytes");
    return __primitive("ascii", this, i);
  }

  @chpldoc.nodoc
  inline proc param string.this(param i: int) param : string {
    if i < 0 || i > this.size-1 then
      compilerError("index " + i:string + " out of bounds for string with length " + this.size:string);
    return __primitive("string item", this, i);
  }

  @chpldoc.nodoc
  inline proc param string.item(param i: int) param : string {
    if i < 0 || i > this.size-1 then
      compilerError("index " + i:string + " out of bounds for string with length " + this.size:string);
    return __primitive("string item", this, i);
  }

  @chpldoc.nodoc
  inline proc param string.numBytes param do
    return __primitive("string_length_bytes", this);

  @chpldoc.nodoc
  inline proc param string.numCodepoints param do
    return __primitive("string_length_codepoints", this);

  @chpldoc.nodoc
  inline proc param string.size param do
    return this.numCodepoints;

  @chpldoc.nodoc
  inline proc _string_contains(param a: string, param b: string) param do
    return __primitive("string_contains", a, b);


  //
  // Append
  //
  /*
     Appends the string `rhs` to the string `lhs`.
  */
  operator string.+=(ref lhs: string, const ref rhs: string) : void {
    doAppend(lhs, rhs);
  }

  /*
     Appends the codepoint values passed to the :type:`string` `this`.

     Any argument not in 0..0x10FFFF is not valid Unicode codepoint.
     This function will append the replacement character 0xFFFD instead of
     such invalid arguments.
   */
  @unstable("'string.appendCodepointValues' is unstable and may change in the future")
  proc ref string.appendCodepointValues(codepoints: int ...) : void {
    var nbytesTotal = 0;
    var buf: c_array(uint(8), 4*codepoints.size);
    // TODO: make c_ptrTo(myCArray) work
    for param i in 0..<codepoints.size {
      var cp = codepoints(i);
      if 0 <= cp && cp <= 0x10FFFF {
        // it is a valid Unicode codepoint
      } else {
        // it is invalid. Use the replacement character.
        cp = 0xFFFD;
      }
      var nbytes = qio_nbytes_char(cp: int(32));
      if boundsChecking {
        assert(0 <= nbytes && nbytes <= 4);
      }
      qio_encode_char_buf(c_ptrTo(buf[nbytesTotal]), cp: int(32));
      nbytesTotal += nbytes;
    }
    doAppendSomeBytes(this, nbytesTotal, buf, nCodepoints=codepoints.size);
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

  @chpldoc.nodoc
  operator string.==(a: string, b: string) : bool {
    // At the moment, this commented out section will not work correctly. If a
    // and b are on the same locale, we will go to that locale, but an autoCopy
    // will localize a and b, before they are placed into the on bundle,
    // causing us to access garbage data inside the doEq routine. Always
    // localize for now.
    //
    /* if a.locale_id == b.locale_id {
      var ret: bool = false;
      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(a.locale_id, c_sublocid_none)) {
        ret = doEq(a, b);
      }
      return ret;
    } else { */
    return doEq(a,b);
  }

  @chpldoc.nodoc
  inline operator string.!=(a: string, b: string) : bool {
    return !doEq(a,b);
  }

  @chpldoc.nodoc
  inline operator string.<(a: string, b: string) : bool {
    return doLessThan(a, b);
  }

  @chpldoc.nodoc
  inline operator string.>(a: string, b: string) : bool {
    return doGreaterThan(a, b);
  }

  @chpldoc.nodoc
  inline operator string.<=(a: string, b: string) : bool {
    return doLessThanOrEq(a, b);
  }
  @chpldoc.nodoc
  inline operator string.>=(a: string, b: string) : bool {
    return doGreaterThanOrEq(a, b);
  }


  //
  // Helper routines
  //

  require "wctype.h";

  // Portability Note:
  // wint_t will normally be a 32-bit int.
  // There may be systems where it is not.
  @chpldoc.nodoc
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
     :returns: A new :type:`string` storing the complete multibyte character sequence
               that corresponds to the codepoint value `i`.
  */
  inline proc codepointToString(i: int(32)) : string {
    const mblength = qio_nbytes_char(i): int;
    var (buffer, mbsize) = bufferAlloc(mblength+1);
    qio_encode_char_buf(buffer, i);
    buffer[mblength] = 0;
    try! {
      return string.createAdoptingBuffer(buffer, mblength, mbsize);
    }
  }

  //
  // Casts (casts to & from other primitive types are in StringCasts)
  //

  // Cast from c_string to string
  @chpldoc.nodoc
  @deprecated("the type 'c_string' is deprecated; please use one of the 'string.create*ingBuffer' methods that takes a 'c_ptrConst(c_char)' instead")
  operator :(cs: c_string, type t: string)  {
    try {
      return string.createCopyingBuffer(cs:c_ptrConst(c_char));
    }
    catch {
      halt("Casting a non-UTF-8 c_string to string");
    }
  }

  // Cast from byteIndex to int
  @chpldoc.nodoc
  inline operator :(cpi: byteIndex, type t: int)  {
    return cpi._bindex;
  }

  // Cast from int to byteIndex
  @chpldoc.nodoc
  inline operator :(i: int, type t: byteIndex)  {
    var cpi: byteIndex;
    cpi._bindex = i;
    return cpi;
  }

  // Cast from codepointIndex to int
  @chpldoc.nodoc
  inline operator :(cpi: codepointIndex, type t: int)  {
    return cpi._cpindex;
  }

  // Cast from int to codepointIndex
  @chpldoc.nodoc
  inline operator :(i: int, type t: codepointIndex) {
    var cpi: codepointIndex;
    cpi._cpindex = i;
    return cpi;
  }

  //
  // hashing support
  //

  @chpldoc.nodoc
  inline proc string.hash(): uint {
    return getHash(this);
  }
  string implements hashable;

  @chpldoc.nodoc
  operator string.<=>(ref x: string, ref y: string) {
    if (x.locale_id != y.locale_id) {
      // TODO: could we just change locale_id?
      var tmp = x;
      x = y;
      y = tmp;
    } else {
      x.buffLen <=> y.buffLen;
      x.buffSize <=> y.buffSize;
      x.cachedNumCodepoints <=> y.cachedNumCodepoints;
      x.buff <=> y.buff;
      x.isOwned <=> y.isOwned;
      x.hasEscapes <=> y.hasEscapes;
      x.locale_id <=> y.locale_id;
    }
  }
}
